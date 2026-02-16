/*
 *
 * client_example.c
 * Description:
 * MRPC (RPC over HW Mailbox) example client
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
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/byteorder/generic.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/random.h>
#include <linux/mrpc/mrpc.h>
#include "mrpc_examples.h"

#define MAX_NAME_SIZE   50

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

struct client {
    struct list_head list;
    char name[MAX_NAME_SIZE];
    struct mrpc_client *mrpc;
};

struct clients_database {
    struct list_head clients;
    struct mutex lock;
    struct kobject *kobj;
};

typedef int (*client_func_t)(struct client *c, __u8 proc_id);

static struct clients_database db;
static unsigned int num_clients = 1;

/**
 * demo_proc_0 - client side call for proc id 0
 *  
 * This example shows how to call an mrpc procedure
 * with no arguments and no reply.
 * 
 * @param c      client pointer
 * @param procid proc id
 * 
 * @return 0 for success, -1 otherwise
 */
static int demo_proc_0(struct client *c, __u8 procid)
{
    int ret, errcode = 0;

    if (!c)
        return -ENODEV;

    DBG("%s: call id=%d (no arg, no reply)", c->name, procid);
    ret = mrpc_call(c->mrpc, procid, NULL, 0, NULL, 0, 0, &errcode);
    if (ret || errcode) {
        pr_err("%s: ERROR: ret=%d, errcode=%d", c->name, ret, errcode);
        return -1;
    }

    DBG("%s: SUCCESS", c->name);

    return 0;
}

/**
 * demo_proc_1 - client side call for proc id 1 
 *  
 * This example shows how to call an mrpc procedure with an 
 * argument and a reply. Both the argument and the reply are 
 * unsigned int random generated numbers. The reply argument is 
 * generated at the server side. 
 * 
 * @param c      client pointer
 * @param procid proc id
 * 
 * @return 0 for success, -1 otherwise
 */
static int demo_proc_1(struct client *c, __u8 procid)
{
    unsigned int rand, arg, rep;
    int ret, errcode = 0;

    if (!c)
        return -ENODEV;

    rand = get_random_int();
    arg = htonl(rand);
    DBG("%s: call id=%d with arg=%u", c->name, procid, rand);
    ret = mrpc_call(c->mrpc, procid, &arg, sizeof(unsigned int),
                    &rep, sizeof(unsigned int), 0, &errcode);
    if (ret || errcode) {
        pr_err("%s: ERROR: ret=%d, errcode=%d", c->name, ret, errcode);
        return -1;
    }

    DBG("%s: SUCCESS - reply=%u", c->name, ntohl(rep));

    return 0;
}

/**
 * demo_proc_2 - client side call for proc id 2
 * 
 * This example shows how to call an mrpc procedure with an
 * argument and a reply.
 * Both the argument and the reply are structures defined in the common
 * mrpc_examples.h header file.
 * 
 * @param c      client pointer
 * @param procid proc id
 * 
 * @return 0 for success, -1 otherwise
 */
static int demo_proc_2(struct client *c, __u8 procid)
{
    struct demo2_arg_struct arg;
    struct demo2_rep_struct rep;
    int ret, errcode = 0;

    if (!c)
        return -ENODEV;

    arg.a = htonl(get_random_int());
    strncpy(arg.str1, "abcdefghijklmnopqrstuvwxyz", sizeof("abcdefghijklmnopqrstuvwxyz"));
    strncpy(arg.str2, "1234567890", sizeof("1234567890"));
    arg.x = htonl(get_random_int());

    DBG("%s: call id=%d:  arg.a=%d, arg.str1=%s, arg.str2=%s, arg.x=%u",
            c->name, procid, ntohl(arg.a), arg.str1, arg.str2, ntohl(arg.x));
    ret = mrpc_call(c->mrpc, procid, &arg, sizeof(arg),
                    &rep, sizeof(rep), 0, &errcode);
    if (ret || errcode) {
        pr_err("%s: ERROR: ret=%d, errcode=%d", c->name, ret, errcode);
        return -1;
    }

    DBG("%s: SUCCESS: rep.b=%u, rep.str1=%s, rep.str2=%s, rep.y=%d",
            c->name, ntohl(rep.b), rep.str1, rep.str2, ntohs(rep.y));

    return 0;
}

/**
 * demo_proc_3 - client side call for proc id 3
 * 
 * This example is identical to the demo_proc_0 example, e.g. no
 * argument and no reply, but the server will simulate an error causing
 * the call to return the error code.
 * 
 * @param c      client pointer
 * @param procid proc id
 * 
 * @return 0 for success, -1 otherwise
 */
static int demo_proc_3(struct client *c, __u8 procid)
{
    int ret, errcode = 0;

    DBG("%s: call id=%d", c->name, procid);
    ret = mrpc_call(c->mrpc, procid, NULL, 0, NULL, 0, 0, &errcode);
    if (ret || errcode) {
        DBG("%s: ERROR: ret=%d, errcode=%d", c->name, ret, errcode); /* simulated error */
        return -1;
    }

    DBG("%s: SUCCESS", c->name);

    return 0;
}

/**
 * demo_proc_4 - client side call for proc id 4
 * 
 * This example shows the client side of an MRPC proc with
 * "dynamic" reply:
 * The client sends a random number betweem 1 to 10 as the argument.
 * The server receives this number and sends a #arg numver
 * of integers as a reply, hence the "dynamic".
 * 
 * Note - The registered proc replen member at the server side
 * MUST be -1.
 * 
 * @param c      client pointer
 * @param procid proc id
 * 
 * @return 0 for success, -1 otherwise
 */
static int demo_proc_4(struct client *c, __u8 procid)
{
    int arg, ret, errcode = 0;
    int i, rep[10] = {0};

    arg = htonl(get_random_int() % 10);
    DBG("%s: call id=%d, arg=%d", c->name, procid, ntohl(arg));
    ret = mrpc_call(c->mrpc, procid, &arg, sizeof(int), rep, sizeof(rep), 0, &errcode);
    if (ret || errcode) {
        pr_err("%s: ERROR: ret=%d, errcode=%d", c->name, ret, errcode);
        return -1;
    }

    for (i = 0; i < ntohl(arg); i++)
        DBG("%s: rep[%d]=%d", c->name, i, rep[i]);
    DBG("%s: SUCCESS", c->name);

    return 0;
}

static client_func_t example_client_functions[] = {
    demo_proc_0,
    demo_proc_1,
    demo_proc_2,
    demo_proc_3,
    demo_proc_4,
};

/**
 * client_call
 * 
 * Initiate an mrpc_call to proc <num> for all registered clients.
 * 
 * Usage:
 * echo <num> > /sys/mrpc_client_example/call
 * 
 * @param kobj
 * @param attr
 * @param buf
 * @param n
 * 
 * @return 0 for successs, error code otherwise
 */
static ssize_t client_call(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           const char *buf, size_t n)
{
    struct client *c;
    client_func_t func;
    unsigned long val;

    if (kstrtoul(buf, 10, &val))
        return -EINVAL;

    if (val > ARRAY_SIZE(example_client_functions) - 1)
        return -EFAULT;

    list_for_each_entry(c, &db.clients, list) {
        func = example_client_functions[val];
        func(c, (__u8)val);
    }

    return n;
}

/**
 * client_call_all
 * 
 * Initiate <num> repititions of mrpc calls to all procs for
 * all registered clients.
 * 
 * Usage:
 * echo <num> > /sys/mrpc_client_example/call_all
 * 
 * @param kobj
 * @param attr
 * @param buf
 * @param n
 * 
 * @return 0 for success, error code otherwise
 */
static ssize_t client_call_all(struct kobject *kobj,
                               struct kobj_attribute *attr,
                               const char *buf, size_t n)
{
    struct client *c;
    client_func_t func;
    unsigned long val;
    int ret, i, j;

    if (kstrtoul(buf, 10, &val))
        return -EINVAL;

    pr_info("calling all procs (%u repetitions)...", val);

    for (j = 0; j < val; j++) {
        pr_info("%d iteration:", j);
        list_for_each_entry(c, &db.clients, list)
            for (i = 0; i < ARRAY_SIZE(example_client_functions); i++) {
                func = example_client_functions[i];
                func(c, (__u8)i);
            }
    }

    return n;
}

static struct kobj_attribute call_attribute =
    __ATTR(call, S_IWUGO, NULL, client_call);
static struct kobj_attribute call_all_attribute =
    __ATTR(call_all, S_IWUGO, NULL, client_call_all);

static struct attribute *client_attrs[] = {
    &call_attribute.attr,
    &call_all_attribute.attr,
    NULL
};

static struct attribute_group client_attrs_group = {
    .attrs = client_attrs,
};

/**
 * client_example_init
 * 
 * Initialize the client example driver
 * 
 * @return 0 for success, error code otherwise
 */
static int __init client_example_init(void)
{
    struct client *c, *tmp;
    int i, ret;

    mutex_init(&db.lock);
    INIT_LIST_HEAD(&db.clients);
    for (i = 0; i < num_clients; i++) {
        c = kzalloc(sizeof(*c), GFP_KERNEL);
        if (!c) {
            pr_err("memory allocation failed");
            ret = -ENOMEM;
            goto out_mrpc_unreg;
        }
        snprintf(c->name, MAX_NAME_SIZE, "demo-client%d", i);
        list_add(&c->list, &db.clients);
        c->mrpc = mrpc_client_register(i, c->name);
        if (!c->mrpc) {
            pr_err("failed to register %s", c->name);
            ret = -EACCES;
            goto out_mrpc_unreg;
        }
    }

    db.kobj = kobject_create_and_add("mrpc_client_example", NULL);
    if (!db.kobj) {
        ret = -ENOMEM;
        goto out_mrpc_unreg;
    }

    ret = sysfs_create_group(db.kobj, &client_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        goto out_kobject_put;
    }

    return 0;

out_kobject_put:
    kobject_put(db.kobj);
out_mrpc_unreg:
    list_for_each_entry_safe(c, tmp, &db.clients, list) {
        mrpc_client_unregister(c->mrpc);
        list_del(&c->list);
        kfree(c);
    }

    return ret;
}

/**
 * client_example_exit
 * 
 * Remove the client example driver
 * 
 * @return 0 for success, error code otherwise
 */
static void __exit client_example_exit(void)
{
    struct client *c, *tmp;

    sysfs_remove_group(db.kobj, &client_attrs_group);
    kobject_put(db.kobj);
    list_for_each_entry_safe(c, tmp, &db.clients, list) {
        mrpc_client_unregister(c->mrpc);
        list_del(&c->list);
        kfree(c);
    }
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(client_example_init);
module_exit(client_example_exit);
module_param(num_clients, uint, 0644);
MODULE_PARM_DESC(num_clients, "number of clients");

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Tomer Eliyahu tomer.b.eliyahu@intel.com");
MODULE_DESCRIPTION("RPC over Mailbox Client example");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
