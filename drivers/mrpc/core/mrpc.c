/*
 *
 * mrpc.c
 * Description:
 * MRPC (RPC over HW Mailbox) driver implementation
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#define pr_fmt(fmt) KBUILD_MODNAME ":%s:%d " fmt "\n", __func__, __LINE__

/************************************************/
/**                    Includes                 */
/************************************************/
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/byteorder/generic.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/err.h>
#include <linux/mrpc/mrpc.h>
#include "puma7_npcpu_appcpu_defs.h"
#include "hw_mbox_drv.h"
#include "hw_mbox_defs.h"

#define DRIVER_VERSION "1.0.0"

/************************************************/
/**           Structures & Enums                */
/************************************************/

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

#define create_highpriority_workqueue(name) \
        alloc_workqueue("%s", WQ_HIGHPRI, 0, (name))

#define MAX_NAME_SIZE                       50
#define DEFAULT_CLIENT_WAIT_TIMEOUT_MS      200

#ifdef CONFIG_ARM_AVALANCHE_SOC
#define MRPC_HW_MBOX_THIS HW_MBOX_MASTER_NP_CPU
#define MRPC_HW_MBOX_OTHER HW_MBOX_MASTER_APP_CPU
#else
#define MRPC_HW_MBOX_THIS HW_MBOX_MASTER_APP_CPU
#define MRPC_HW_MBOX_OTHER HW_MBOX_MASTER_NP_CPU
#endif

enum mrpc_msg_type {
    MRPC_CALL_MSG  = 0,     /* client call message */
    MRPC_REPLY_MSG,         /* server reply message */
    MRPC_ERROR_MSG
};

/* mrpc header structure */
struct mrpc_hdr {
    __be32 signature;       /* mrpc signature */
    __be32 cookie;	    /* message cookie */
    __u8 msg_type;          /* message type */
    __be32 id;              /* client/server id */
    __u8 proc;              /* proc id */
    __be32 ret;             /* proc return val */
    union {
        struct {
            __be32 arglen;  /* argument length [bytes] */
            __be32 replen;  /* reply length [bytes] */
            __u8 data[0];   /* data starts here */
        } payload;
        struct {
            __be32 errcode; /* signed error code */
        } error;
    } u;
/* macros to easily access mrpc_hdr union members */
#define h_arglen u.payload.arglen
#define h_arg u.payload.data
#define h_replen u.payload.replen
#define h_rep u.payload.data
#define h_errcode u.error.errcode
};

/* structure representing an MRPC client */
struct mrpc_client {
    struct list_head list;
    struct mrpc_private *mrpc;  /* mrpc driver database pointer */
    struct kobject kobj;        /* kobject for sysfs */
    unsigned int id;            /* Client ID */
    char name[MAX_NAME_SIZE];   /* Client name */
    struct mutex lock;          /* Client mutex (synchronize calls) */
    wait_queue_head_t wait;     /* Client wait queue for replies */
    unsigned long timeout;      /* default timeout[ms] when using mrpc_call */
    struct mrpc_hdr *rep;       /* Client reply pointer */
    struct mrpc_hdr req;        /* Client current request header */
    spinlock_t spinlock;        /* Client spinlock */
    unsigned int seq;		/* Client sequence counter */
};
#define to_mrpc_client(x) container_of(x, struct mrpc_client, kobj)

/* structure representing a server work to run a remote procedure */
struct mrpc_server_work {
    struct work_struct work;
    struct mrpc_server *server; /* server pointer */
    __u8 proc;                  /* procedure id */
    __be32 cookie;              /* client cookie */
    void *arg;                  /* argument pointer */
    ssize_t arg_len;            /* argument length */
    void *rep;                  /* reply pointer */
    ssize_t rep_len;            /* reply length */
};

/* structure representing an MRPC server */
struct mrpc_server {
    struct list_head list;
    struct mrpc_private *mrpc;      /* mrpc driver database pointer */
    struct kobject kobj;            /* kobj for sysfs */
    unsigned int id;                /* server id */
    char name[MAX_NAME_SIZE];       /* server name */
    struct mutex lock;              /* server lock */
    struct workqueue_struct *wq;    /* server work queue for serving client calls */
    struct mrpc_procinfo *procs;    /* server procs */
    u32 num_procs;                  /* server number of procs */
};
#define to_mrpc_server(x) container_of(x, struct mrpc_server, kobj)

/* structre representing the mrpc driver */
struct mrpc_private {
    struct platform_device *pdev;       /* platform device pointer */
    struct mutex lock;                  /* mrpc lock for lists access */
    struct list_head clients;           /* registered clients list */
    struct list_head servers;           /* registered servers list */
    struct kset *clients_kset;          /* kset for clients sysfs */
    struct kset *servers_kset;          /* kset for servers sysfs */
    hw_mbox_Masters_e this_cpu;         /* this cpu HW_MBOX master */
    hw_mbox_Masters_e other_cpu;        /* other cpu HW_MBOX master */
    hw_mbox_channelConfig_t txch_cfg;   /* tx channel configuration */
    hw_mbox_channelConfig_t rxch_cfg;   /* rx channel configuration */
    Uint32 tag;                         /* MRPC driver HW MBOX tag */
};

/************************************************/
/**                Globals                      */
/************************************************/
static struct mrpc_private *this;   /* global pointer for private database */

/************************************************/
/**                    Implementation           */
/************************************************/

/* copy h2 to h1 and convert endianess to *host* byte order */
static inline void copy_header(struct mrpc_hdr *h1, struct mrpc_hdr *h2)
{
    BUG_ON(!h1);
    BUG_ON(!h2);

    h1->signature = ntohl(h2->signature);
    h1->proc = h2->proc;
    h1->ret = ntohl(h2->ret);
    h1->id = ntohl(h2->id);
    h1->cookie = ntohl(h2->cookie);
    h1->msg_type = h2->msg_type;
    if (h1->msg_type == MRPC_ERROR_MSG) {
        h1->h_errcode = ntohl(h2->h_errcode);
    } else {
        h1->h_arglen = ntohl(h2->h_arglen);
        h1->h_replen = ntohl(h2->h_replen);
    }
}

/* dump mrpc header in host byte order */
static inline void dump_header(struct mrpc_hdr *h, bool ntoh)
{
    pr_err("dump header");
    pr_err("signature: 0x%08x", ntoh ? ntohl(h->signature) : h->signature);
    pr_err("id: %d", ntoh ? ntohl(h->id) : h->id);
    pr_err("msg_type: %d", h->msg_type);
    pr_err("proc: %d", h->proc);
    pr_err("ret: %d", ntoh ? ntohl(h->ret): h->ret);
    pr_err("errcode: %d", ntoh ? ntohl(h->h_errcode) : h->h_errcode);
}

/* simple sanity check for incoming messages */
static inline int validate_header(struct mrpc_hdr *hdr)
{
    /* sanity checks */
    if (!hdr) {
        pr_err("NULL pointer received");
        return MRPC_ERR_INVALID_HEADER;
    }
    if (ntohl(hdr->signature) != 0x4d525043) {
        pr_err("mrpc signature error");
        return MRPC_ERR_INVALID_SIGNATURE;
    }

    return 0;
}

/**
 * server_send_error
 * sends an error back to the mrpc client
 *
 * @param s      server handle
 * @param proc   ptoc id
 * @param err    error code to send
 *
 * @return 0 for success, hw_mbox error code otherwise
 */
static inline int server_send_error(struct mrpc_server *s, u8 proc, int err)
{
    unsigned int reply_len;
    struct mrpc_hdr hdr = {
        .id = htonl(s->id),
        .msg_type = MRPC_ERROR_MSG,
        .proc = proc,
        .signature = htonl(0x4d525043),
        .h_errcode = htonl(err)
    };
    int ret;

    mutex_lock(&s->lock);
    ret = hwMbox_sendOpcode(s->mrpc->other_cpu, s->mrpc->tag, (Uint8 *)&hdr,
                            sizeof(hdr) + sizeof(hwMbox_opcodeMessage_t),
                            sizeof(hdr), &reply_len);
    mutex_unlock(&s->lock);

    return ret;
}

/**
 * server_send_reply
 * send a reply back to the client
 *
 * @param s      server handle
 * @param proc   proc id
 * @param rep    reply pointer
 * @param replen reply length (bytes)
 * @param retval remote procedure return value
 *
 * @return 0 for success, error code otherwise
 */
static inline int server_send_reply(struct mrpc_server *s, u8 proc,
				    __be32 cookie, void *rep, ssize_t replen,
                                    int retval)
{
    struct mrpc_hdr *hdr;
    int reply_len, ret = 0;
    ssize_t size, max_size;
    int errcode;

    BUG_ON(!s);

    max_size = s->mrpc->txch_cfg.memBuff.size;
    size = sizeof(*hdr) + replen;
    if (size > max_size) {
        pr_err("server %s(%d) message too big", s->name, s->id);
        return server_send_error(s, proc, MRPC_ERR_MSG_TOO_BIG);
    }

    hdr = kzalloc(size + sizeof(hwMbox_opcodeMessage_t), GFP_KERNEL);
    if (!hdr) {
        pr_err("memory allocation failed (%d bytes)",
               size + sizeof(hwMbox_opcodeMessage_t));
        return server_send_error(s, proc, -ENOMEM);
    }

    hdr->signature = htonl(0x4d525043); /* MRPC in ASCII */
    hdr->msg_type = MRPC_REPLY_MSG;
    hdr->id = htonl(s->id);
    hdr->cookie = htonl(cookie);
    hdr->proc = proc;
    hdr->ret = htonl(retval);
    hdr->h_replen = htonl(replen);
    memcpy(hdr->h_rep, rep, replen);
    mutex_lock(&s->lock);
    DBG("%s: send reply [cookie=0x%08x, proc=%d]", s->name, cookie, proc);
    ret = hwMbox_sendOpcode(s->mrpc->other_cpu, s->mrpc->tag, (Uint8 *)hdr,
                            size + sizeof(hwMbox_opcodeMessage_t),
                            size, &reply_len);
    mutex_unlock(&s->lock);
    if (ret) {
        pr_err("hw_mbox error %d", ret);
        if (reply_len > 0) {
            errcode = ((hwMbox_opcodeMessage_t *)hdr)->msgData.msgStatus.status;
            pr_err("hw_mbox errcode=%d", errcode);
            server_send_error(s, proc, errcode);
        }
    }

    kfree(hdr);
    return ret;
}

/**
 * mrpc_server_worker
 * worker function for server remote procedure calls
 *
 * @param w      work structure containing the proc id, argument and argument length
 */
static void mrpc_server_worker(struct work_struct *w)
{
    struct mrpc_server_work *work = container_of(w, struct mrpc_server_work,
                                                 work);
    struct mrpc_server *s = work->server;
    proc_t cb = s->procs[work->proc].p_func;
    int ret;

    DBG("S: %s(%d) worker: calling proc %d [cookie=0x%08x]",
	    s->name, s->id, work->proc, work->cookie);

    ret = cb(work->arg, work->arg_len, work->rep, work->rep_len);

    server_send_reply(s, work->proc, work->cookie, work->rep, work->rep_len, ret);

    kfree(work->arg);
    kfree(work->rep);
    kfree(work);
}

/**
 * server_handle_call
 * dispatch mrpc call requests on the server side to server workers
 *
 * @param hdr    mrpc_hdr of the incoming request
 *
 * @return 0 for success, error code otherwise
 */
static int server_handle_call(struct mrpc_hdr *hdr)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_server_work *work;
    struct mrpc_server *p, *server = NULL;
    struct mrpc_procinfo procinfo;
    int err = 0;

    BUG_ON(!hdr);

    mutex_lock(&mrpc->lock);
    list_for_each_entry(p, &mrpc->servers, list)
        if (p->id == ntohl(hdr->id))
            server = p;
    mutex_unlock(&mrpc->lock);

    if (!server) {
        pr_err("invalid server (id=%d)", ntohl(hdr->id));
        err = MRPC_ERR_SERVER_NOT_READY;
        goto out_err;
    }

    if (hdr->proc > server->num_procs - 1) {
        pr_err("[S%d (%s)] invalid proc (%d)",
		server->id, server->name, hdr->proc);
        err = MRPC_ERR_INVALID_PROC;
        goto out_err;
    }

    procinfo = server->procs[hdr->proc];
    if (procinfo.p_arglen != ntohl(hdr->h_arglen) &&
        procinfo.p_arglen != -1) {
        pr_err("[S%d (%s)] invalid proc arglen (%d)",
        server->id, server->name, ntohl(hdr->h_arglen));
        err = MRPC_ERR_ARGLEN_MISMATCH;
        goto out_err;
    }

    if (procinfo.p_replen != ntohl(hdr->h_replen) &&
        procinfo.p_replen != -1) {
        pr_err("[S%d (%s)] invalid proc replen (%d)",
        server->id, server->name, ntohl(hdr->h_replen));
        err = MRPC_ERR_REPLEN_MISMATCH;
        goto out_err;
    }

    work = kzalloc(sizeof(*work), GFP_KERNEL);
    if (!work) {
        pr_err("[S%d (%s)] memory allocation failed", server->id, server->name);
        err = -ENOMEM;
        goto out_err;
    }

    work->server = server;
    work->proc = hdr->proc;
    work->cookie = ntohl(hdr->cookie);
    work->arg_len = ntohl(hdr->h_arglen);
    work->arg = kzalloc(work->arg_len, GFP_KERNEL);
    if (!work->arg) {
        pr_err("[S%d (%s)] memory allocation failed", server->id, server->name);
        err = -ENOMEM;
        goto free_work;
    }
    memcpy(work->arg, hdr->h_arg, work->arg_len);
    work->rep_len = ntohl(hdr->h_replen);
    work->rep = kzalloc(work->rep_len, GFP_KERNEL);
    if (!work->rep) {
        pr_err("[S%d (%s)] memory allocation failed", server->id, server->name);
        err = -ENOMEM;
        goto free_arg;
    }

    INIT_WORK(&work->work, mrpc_server_worker);
    queue_work(server->wq, &work->work);
    DBG("[S%d (%s)] call message queued [cookie=0x%08x]",
	 server->id, server->name, work->cookie);
    return 0;

free_arg:
    kfree(work->arg);
free_work:
    kfree(work);
out_err:
    return err;
}

/**
 * client_handle_reply
 * parse the reply on client's side and wake up waiting client.
 *
 * @param hdr    mrpc_hdr containing the server reply
 *
 * @return 0 for succes, error code otherwise
 */
static int client_handle_reply(struct mrpc_hdr *hdr)
{
    struct mrpc_client *p, *client = NULL;
    struct mrpc_private *mrpc = this;
    struct mrpc_hdr *rep;
    ssize_t replen, size = sizeof(*hdr);

    BUG_ON(!hdr);
    replen = ntohl(hdr->h_replen);

    mutex_lock(&mrpc->lock);
    list_for_each_entry(p, &mrpc->clients, list)
        if (p->id == ntohl(hdr->id))
            client = p;
    mutex_unlock(&mrpc->lock);

    if (!client) {
        pr_err("invalid client (id=%d)", ntohl(hdr->id));
        return MRPC_ERR_INVALID_CLIENT;
    }

    if (hdr->msg_type == MRPC_REPLY_MSG)
        size += replen;

    rep = kzalloc(size, GFP_KERNEL);
    if (!rep) {
        pr_err("[C%d (%s)] memory allocation failed", client->id, client->name);
        return -ENOMEM;
    }

    copy_header(rep, hdr);
    if (hdr->msg_type == MRPC_REPLY_MSG)
        memcpy(rep->h_rep, hdr->h_rep, replen);

    spin_lock(&client->spinlock);
    if (!client->req.signature) {
	pr_err("[C%d (%s)] DROP response: rep->cookie=0x%08x", client->id,
		client->name, rep->cookie);
	goto drop;
    }
    if (rep->cookie != ntohl(client->req.cookie)) {
	pr_err("[C%d (%s)] DROP response: rep->cookie=0x%08x, req.cookie=0x%08x",
		client->id, client->name, rep->cookie, ntohl(client->req.cookie));
	goto drop;
    }

    BUG_ON(client->rep); 
    client->rep = rep;
    spin_unlock(&client->spinlock);

    wake_up(&client->wait);

    DBG("[C%d (%s)] message reply receive success", client->id, client->name);
    return 0;

drop:
    spin_unlock(&client->spinlock);
    kfree(rep);
    return 0;
}

/**
 * hw_mailbox_callback
 * callback function registered by the mrpc driver to the hw mailbox,
 * dispatches all server replies and client calls.
 *
 * @param commander this cpu
 * @param dataBuf   pointer to the data buffer
 * @param dataLen   data buffer length (bytes)
 * @param token     unused
 *
 * @return 0 for success, error code otherwise
 */
static int hw_mbox_callback(hw_mbox_Masters_e commander, Uint8 *dataBuf,
                            Uint32 dataLen, Uint32 *token)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_hdr *hdr = (struct mrpc_hdr *)dataBuf;
    ssize_t datalen = dataLen - sizeof(*hdr);
    int err = 0;

    DBG("dataBuf=%p, dataLen=%d, datalen=%d, token=%p",
        dataBuf, dataLen, datalen, token);

    err = validate_header(hdr);
    if (err)
        goto out_err;

    switch (hdr->msg_type) {
    case MRPC_REPLY_MSG:
    case MRPC_ERROR_MSG:
        err = client_handle_reply(hdr);
        break;
    case MRPC_CALL_MSG:
        err = server_handle_call(hdr);
        break;
    default: BUG();
    }

    if (err) {
        err = htonl(err);
        goto out_err;
    }

    hwMbox_sendAckOpcode(mrpc->other_cpu);
    return 0;

out_err:
    hwMbox_sendReplyOpcode(mrpc->other_cpu, (Uint8 *)&err, sizeof(err));
    return 0;
}

/* simple client sanity checks */
static inline int validate_client(struct mrpc_client *client)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_client *p;

    if (!client) {
        pr_err("invalid client pointer");
        return MRPC_ERR_INVALID_CLIENT;
    }

    mutex_lock(&mrpc->lock);
    list_for_each_entry(p, &mrpc->clients, list)
        if (p == client) {
            mutex_unlock(&mrpc->lock);
            DBG("client %s id %d is valid", client->name, client->id);
            return 0;
        }

    mutex_unlock(&mrpc->lock);
    pr_err("invalid client %p", client);
    return MRPC_ERR_INVALID_CLIENT;
}

/* mrpc call API function - see linux/mrpc.h */
int mrpc_call(struct mrpc_client *c, __u8 proc, void *arg,
              ssize_t arg_len, void *rep, ssize_t rep_len,
              unsigned long timeout, int *errcode)
{
    struct mrpc_hdr *hdr;
    ssize_t size, max_size, reply_len;
    int ret = 0;

    if (!errcode)
        return -EINVAL;

    *errcode = 0;
    ret = validate_client(c);
    if (ret)
        return ret;

    /* build mrpc header + data */
    size = arg_len + sizeof(*hdr);
    max_size = c->mrpc->txch_cfg.memBuff.size;
    if (size + sizeof(hwMbox_opcodeMessage_t) > max_size) {
        pr_err("[C%d (%s)] message too long: arg_len = %d",
               c->id, c->name, arg_len);
        return MRPC_ERR_MSG_TOO_BIG;
    }

    hdr = kzalloc(size + sizeof(hwMbox_opcodeMessage_t), GFP_KERNEL);
    if (!hdr) {
        pr_err("[C%d (%s)] memory allocation failed "
	       "(header_len(%d)+arg_len(%d)=%d bytes",
               c->id, c->name, sizeof(*hdr), arg_len, sizeof(*hdr) + arg_len);
        return -ENOMEM;
    }

    hdr->signature = htonl(0x4d525043); /* MRPC in ASCII */
    hdr->msg_type = MRPC_CALL_MSG;
    hdr->id = htonl(c->id);
    hdr->cookie = htonl(c->seq++);
    hdr->proc = proc;
    hdr->h_arglen = htonl(arg_len);
    hdr->h_replen = htonl(rep_len);
    memcpy(hdr->h_arg, arg, arg_len);

    mutex_lock(&c->lock);
    BUG_ON(c->rep);
    BUG_ON(c->req.signature); 
    memcpy(&c->req, hdr, sizeof(*hdr));
    mb();
    ret = hwMbox_sendOpcode(c->mrpc->other_cpu, c->mrpc->tag, (Uint8 *)hdr,
                            size + sizeof(hwMbox_opcodeMessage_t),
                            size, &reply_len);
    if (ret) {
        pr_err("[C%d (%s)] hwMbox_sendOpcode returned error, ret=%d",
		c->id, c->name, ret);
        if (reply_len > 0) {
            *errcode = ((hwMbox_opcodeMessage_t *)hdr)->msgData.msgStatus.status;
            pr_err("[C%d (%s)] hw_mbox return error %d",
		    c->id, c->name, *errcode);
            ret = MRPC_ERR_HWMBOX_ERROR;
        }
        goto out_err;
    }

    if (reply_len > 0) {
        pr_err("[C%d (%s)] server error", c->id, c->name);
        ret = MRPC_ERR_SERVER_ERROR;
        *errcode = ntohl(*(int *)hdr);
        goto out_err;
    }

    timeout = timeout ? timeout : c->timeout;
    ret = wait_event_timeout(c->wait, c->rep,
                             msecs_to_jiffies(timeout));
    spin_lock(&c->spinlock);
    if (ret == 0) {
	DBG("[C%d (%s)] wait_event timeout [cookie=0x%08x]", c->id, c->name,
	     ntohl(c->req.cookie));
	/* The response might have arrived after we timed out. If not, it is really a time out event */
	if (!c->rep) {
		pr_err("[C%d (%s)] time out waiting for proc %d [cookie=0x%08x]",
			c->id, c->name, proc, ntohl(c->req.cookie));
		ret = -ETIMEDOUT;
		memset(&c->req, 0, sizeof(c->req));
		spin_unlock(&c->spinlock);
		goto out_err;
	}
    }

    BUG_ON(!c->rep);
    BUG_ON(c->rep->cookie != ntohl(c->req.cookie));
    BUG_ON(c->rep->id != c->id);
    BUG_ON(c->rep->proc != proc);

    /* parse reply */
    switch (c->rep->msg_type) {
    case MRPC_ERROR_MSG:
        ret = MRPC_ERR_SERVER_ERROR;
        *errcode = c->rep->h_errcode;
        break;
    case MRPC_REPLY_MSG:
        ret = c->rep->ret;
        BUG_ON(c->rep->h_replen != rep_len);
        memcpy(rep, c->rep->h_rep, rep_len);
        break;
    default:
        pr_err("[C%d (%s)] unknown msg_type=%d", c->id, c->name, c->rep->msg_type);
        dump_header(c->rep, false);
        BUG();
    }

    kfree(c->rep);
    c->rep = NULL;
    spin_unlock(&c->spinlock);
out_err:
    memset(&c->req, 0, sizeof(c->req));
    mutex_unlock(&c->lock);
    kfree(hdr);
    DBG("[C%d (%s)] call proc %d done (ret=%d, errcode=%d)",
        c->id, c->name, proc, ret, *errcode);
    return ret;
}
EXPORT_SYMBOL(mrpc_call);

static void client_release(struct kobject *kobj)
{
    struct mrpc_client *c = to_mrpc_client(kobj);
    kfree(c);
}

ssize_t client_timeout_store(struct kobject *kobj, struct kobj_attribute *attr,
                             const char *buf, size_t count)
{
    struct mrpc_client *client = to_mrpc_client(kobj);
    unsigned long val;

    BUG_ON(!client);

    if (kstrtoul(buf, 10, &val))
        return -EINVAL;
    client->timeout = val;

    return count;
}

ssize_t client_timeout_show(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
    struct mrpc_client *client = to_mrpc_client(kobj);

    BUG_ON(!client);

    return scnprintf(buf, PAGE_SIZE, "%lu[ms]\n", client->timeout);
}

static struct kobj_attribute client_timeout_attr =
    __ATTR(timeout, S_IWUSR|S_IRUGO, client_timeout_show, client_timeout_store);

static struct attribute *client_default_attrs[] = {
    &client_timeout_attr.attr,
	NULL,
};

static struct kobj_type client_ktype = {
	.sysfs_ops = &kobj_sysfs_ops,
	.release = client_release,
	.default_attrs = client_default_attrs,
};

/* mrpc client registration API function - see linux/mrpc.h */
struct mrpc_client *mrpc_client_register(unsigned int id, const char *name)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_client *client;
    int ret;

    if (!mrpc) {
        pr_err("mrpc driver not initialized");
        return NULL;
    }

    mutex_lock(&mrpc->lock);
    list_for_each_entry(client, &mrpc->clients, list)
        if (client->id == id) {
            pr_err("client %s (id=%d) already registered", name, id);
            goto out_unlock;
        }

    client = kzalloc(sizeof(*client), GFP_KERNEL);
    if (!client) {
        pr_err("memory allocation failed");
        goto out_unlock;
    }

    mutex_init(&client->lock);
    spin_lock_init(&client->spinlock);
    init_waitqueue_head(&client->wait);
    client->mrpc = mrpc;
    client->id = id;
    client->timeout = DEFAULT_CLIENT_WAIT_TIMEOUT_MS;
    strlcpy(client->name, name, MAX_NAME_SIZE);

    client->kobj.kset = mrpc->clients_kset;
    ret = kobject_init_and_add(&client->kobj, &client_ktype, NULL, "%s", name);
    if (ret) {
        pr_err("kobject_init_and_add failed with ret=%d", ret);
        kobject_put(&client->kobj);
        goto out_kfree;
    }
    kobject_uevent(&client->kobj, KOBJ_ADD);
    list_add(&client->list, &mrpc->clients);
    mutex_unlock(&mrpc->lock);

    pr_info("client %s registered with id %d", name, id);
    return client;

out_kfree:
    kfree(client);
out_unlock:
    mutex_unlock(&mrpc->lock);

    pr_err("failed to register client %s (id %d)", name, id);
    return NULL;
}
EXPORT_SYMBOL(mrpc_client_register);

/* mrpc client unregisteration API function - see linux/mrpc.h */
int mrpc_client_unregister(struct mrpc_client *client)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_client *p;

    if (!mrpc)
        return -ENODEV;
    if (!client)
        return -EINVAL;

    mutex_lock(&mrpc->lock);
    list_for_each_entry(p, &mrpc->clients, list)
        if (p == client) {
            pr_err("client %s(%d) unregistered", p->name, p->id);
            list_del(&p->list);
            mutex_unlock(&mrpc->lock);
            wake_up_interruptible(&p->wait);
            kobject_put(&p->kobj);
            return 0;
        }
    mutex_unlock(&mrpc->lock);

    pr_err("ERROR - client not found");
    return -ENODEV;
}
EXPORT_SYMBOL(mrpc_client_unregister);

static void server_release(struct kobject *kobj)
{
    struct mrpc_server *s = to_mrpc_server(kobj);
    kfree(s);
}

static ssize_t server_procs_show(struct kobject *kobj,
                                 struct kobj_attribute *attr, char *buf)
{
    struct mrpc_server *server = to_mrpc_server(kobj);
    ssize_t len;
    int i;

    if (!server)
        return -ENODEV;

    len = scnprintf(buf, PAGE_SIZE, "%s procs:\n", server->name);

    mutex_lock(&server->lock);
    len += scnprintf(buf+len, PAGE_SIZE - len,
                     "id   name   arg_len   rep_len   ptr\n");
    for (i = 0; i < server->num_procs; i++) {
        len += scnprintf(buf+len, PAGE_SIZE - len,
                         "%d   %s   %d   %d   %p\n",
                         server->procs[i].procid,
                         server->procs[i].p_name,
                         server->procs[i].p_arglen,
                         server->procs[i].p_replen,
                         server->procs[i].p_func);
    }
    mutex_unlock(&server->lock);
    return len;
}

static struct kobj_attribute server_procs_attr =
    __ATTR(procs, S_IRUGO, server_procs_show, NULL);

static struct attribute *server_default_attrs[] = {
    &server_procs_attr.attr,
	NULL,
};

static struct kobj_type server_ktype = {
	.sysfs_ops = &kobj_sysfs_ops,
	.release = server_release,
	.default_attrs = server_default_attrs,
};

/* mrpc server registration API function - <linux/mrpc.h */
struct mrpc_server *mrpc_server_register(unsigned int id, const char *name,
                                         struct mrpc_procinfo *procs,
                                         u32 num_procs)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_server *server;
    char worker_name[30];
    ssize_t max_arglen, max_replen, hdrlen;
    int i, ret;

    if (!mrpc) {
        pr_err("mrpc driver not initialized");
        return NULL;
    }

    hdrlen = sizeof(struct mrpc_hdr) + sizeof(hwMbox_opcodeMessage_t);
    max_arglen = mrpc->rxch_cfg.memBuff.size - hdrlen;
    max_replen = mrpc->txch_cfg.memBuff.size - hdrlen;

    /* verify procs */
    for (i = 0; i < num_procs; i++) {
        if (procs[i].p_arglen > max_arglen) {
            pr_warning("WARNING: %s: proc %d (%s) arglen(%d) too big (max %d)",
                   name, i, procs[i].p_name, procs[i].p_arglen, max_arglen);
        }
        if (procs[i].p_replen > max_replen) {
            pr_warning("WARNING: %s: proc %d (%s) replen(%d) too big (max %d)",
                   name, i, procs[i].p_name, procs[i].p_replen, max_replen);
        }
    }

    mutex_lock(&mrpc->lock);
    list_for_each_entry(server, &mrpc->servers, list)
        if (server->id == id) {
            pr_err("server %s (id=%d) already registered", name, id);
            goto out_unlock;
        }

    server = kzalloc(sizeof(*server), GFP_KERNEL);
    if (!server) {
        pr_err("memory allocation failed");
        goto out_unlock;
    }

    mutex_init(&server->lock);
    server->mrpc = mrpc;
    snprintf(worker_name, 30, "%s:%d mrpc server worker", name, id);
//    server->wq = create_singlethread_workqueue(worker_name);
    server->wq = create_highpriority_workqueue(worker_name);
    if (!server->wq) {
        pr_err("create highpriority workqueue failed");
        goto out_kfree;
    }
    pr_info("MRPC Server: High Priority Workqueue Created");

    server->id = id;
    server->procs = procs;
    server->num_procs = num_procs;
    strlcpy(server->name, name, MAX_NAME_SIZE);

    server->kobj.kset = mrpc->servers_kset;
    ret = kobject_init_and_add(&server->kobj, &server_ktype, NULL, "%s", name);
    if (ret) {
        pr_err("kobject_init_and_add failed with ret=%d", ret);
        kobject_put(&server->kobj);
        goto out_workqueue;
    }
    kobject_uevent(&server->kobj, KOBJ_ADD);
    list_add(&server->list, &mrpc->servers);
    mutex_unlock(&mrpc->lock);

    pr_info("server %s registered with id %d", name, id);
    return server;

out_workqueue:
    flush_workqueue(server->wq);
    destroy_workqueue(server->wq);
out_kfree:
    kfree(server);
out_unlock:
    mutex_unlock(&mrpc->lock);

    pr_err("failed to register server %s (id %d)", name, id);
    return NULL;
}
EXPORT_SYMBOL(mrpc_server_register);

/* mrpc server unregistration API function - <linux/mrpc.h */
int mrpc_server_unregister(struct mrpc_server *server)
{
    struct mrpc_private *mrpc = this;
    struct mrpc_server *p;

    if (!mrpc)
        return -ENODEV;
    if (!server)
        return -EINVAL;

    mutex_lock(&mrpc->lock);
    list_for_each_entry(p, &mrpc->servers, list)
        if (p == server) {
            pr_info("server %s(%d) unregistered", p->name, p->id);
            list_del(&p->list);
            mutex_unlock(&mrpc->lock);
            flush_workqueue(p->wq);
            destroy_workqueue(p->wq);
            kobject_put(&p->kobj);
            return 0;
        }
    mutex_unlock(&mrpc->lock);

    pr_err("ERROR - server not found");
    return -ENODEV;
}
EXPORT_SYMBOL(mrpc_server_unregister);

static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct mrpc_private *mrpc = dev_get_drvdata(dev);
    struct mrpc_client *client;
    struct mrpc_server *server;
    ssize_t len;

    if (!mrpc)
        return -EINVAL;

    len = scnprintf(buf, PAGE_SIZE, "mrpc driver status:\n");
    len += scnprintf(buf + len, PAGE_SIZE - len, "version: %s\n"
						 "cpu: %s\n"
                                                 "tag: 0x%x\n"
                                                 "max_tx_msg_sz: %d[bytes]\n"
                                                 "max_rx_msg_sz: %d[bytes]\n",
		     DRIVER_VERSION,
                     mrpc->this_cpu == HW_MBOX_MASTER_NP_CPU ? "NP-CPU" : "APP-CPU",
                     mrpc->tag,
                     mrpc->txch_cfg.memBuff.size,
                     mrpc->rxch_cfg.memBuff.size);

    mutex_lock(&mrpc->lock);

    len += scnprintf(buf+len, PAGE_SIZE - len, "client   id\n");
    list_for_each_entry(client, &mrpc->clients, list)
        len += scnprintf(buf+len, PAGE_SIZE - len, "%s   %d\n",
                         client->name, client->id);

    len += scnprintf(buf+len, PAGE_SIZE - len, "server   id\n");
    list_for_each_entry(server, &mrpc->servers, list)
        len += scnprintf(buf+len, PAGE_SIZE - len, "%s   %d\n",
                         server->name, server->id);

    mutex_unlock(&mrpc->lock);

    return len;
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *mrpc_attrs[] = {
    &dev_attr_status.attr,
    NULL
};
static struct attribute_group mrpc_attrs_group = {
    .attrs = mrpc_attrs,
};

/**
 * mrpc_probe
 * initialize internal database, register to hw_mailbox driver
 *
 * @param pdev   platform device pointer
 *
 * @return 0 for success, error code otherwise
 */
static int mrpc_probe(struct platform_device *pdev)
{
    struct mrpc_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    INIT_LIST_HEAD(&priv->clients);
    INIT_LIST_HEAD(&priv->servers);
    mutex_init(&priv->lock);
    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;
    ret = sysfs_create_group(&priv->pdev->dev.kobj, &mrpc_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->clients_kset = kset_create_and_add("clients", NULL, &priv->pdev->dev.kobj);
    if (!priv->clients_kset) {
        pr_err("memory allocation failed");
        goto out_remove_group;
    }

    priv->servers_kset = kset_create_and_add("servers", NULL, &priv->pdev->dev.kobj);
    if (!priv->servers_kset) {
        pr_err("memory allocation failed");
        goto out_kset_clients;
    }

    if (hwMbox_isReady() < 0) {
        pr_err("HW Mailbox driver not ready");
        ret = -ENODEV;
        goto out_kset_servers;
    }

    priv->tag = NPCPU_APPCPU_HW_MBOX_TAG_MRPC;
    priv->this_cpu = MRPC_HW_MBOX_THIS;
    priv->other_cpu = MRPC_HW_MBOX_OTHER;

    hwMbox_getChannelConfig(priv->other_cpu, hw_mbox_opcode, False,
                            &priv->txch_cfg);
    hwMbox_getChannelConfig(priv->other_cpu, hw_mbox_opcode, True,
                            &priv->rxch_cfg);

    ret = hwMbox_registerRecvOpcode(priv->other_cpu, hw_mbox_callback, priv->tag,
                                    (unsigned int)priv);
    if (ret) {
        pr_err("Failed to register hwMbox_registerRecvOpcode fails");
        ret = -EPERM;
        goto out_kset_servers;
    }

    this = priv;

    return 0;

out_kset_servers:
    kset_unregister(priv->servers_kset);
out_kset_clients:
    kset_unregister(priv->clients_kset);
out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &mrpc_attrs_group);
    return ret;
}

/**
 * mrpc_remove
 * remove the mrpc_driver
 *
 * @param pdev   platform device pointer
 *
 * @return always succeeds
 */
static int mrpc_remove(struct platform_device *pdev)
{
    struct mrpc_private *priv = platform_get_drvdata(pdev);

     /*TODO call unregister hwMbox RecvOpcode (not existant at the moment) */
    kset_unregister(priv->servers_kset);
    kset_unregister(priv->clients_kset);
    sysfs_remove_group(&priv->pdev->dev.kobj, &mrpc_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;

    pr_info("mrpc remove done.");
    return 0;
}

static struct platform_driver mrpc_driver = {
    .driver = {
        .name   = "mrpc",
    },
    .probe = mrpc_probe,
    .remove = mrpc_remove,
};

static struct platform_device *mrpc_device;

static int __init mrpc_init(void)
{
    int ret;

    ret = platform_driver_register(&mrpc_driver);
    if (ret < 0) {
        pr_err("Failed to register mrpc platform driver: %d\n", ret);
        return ret;
    }

    mrpc_device = platform_device_register_simple("mrpc", -1, NULL, 0);
    if (IS_ERR(mrpc_device)) {
        pr_err("Failed to register mrpc platform device\n");
        platform_driver_unregister(&mrpc_driver);
        return PTR_ERR(mrpc_device);
    }

    pr_info("mrpc init done.");
    return 0;
}

static void __exit mrpc_exit(void)
{
    platform_device_unregister(mrpc_device);
    platform_driver_unregister(&mrpc_driver);
    pr_info("mrpc exit done.");
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
#ifdef MODULE
module_init(mrpc_init);
module_exit(mrpc_exit);
#else
subsys_initcall(mrpc_init);
#endif

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Tomer Eliyahu tomer.b.eliyahu@intel.com");
MODULE_DESCRIPTION("RPC over HW Mailbox Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
