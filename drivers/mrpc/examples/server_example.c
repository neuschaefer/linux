/*
 *
 * server_example.c
 * Description:
 * MRPC (RPC over HW Mailbox) example server
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
#include <linux/delay.h>
#include <linux/mrpc/mrpc.h>
#include "mrpc_examples.h"

#define MAX_NAME_SIZE   50

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

struct server {
    struct list_head list;
    char name[MAX_NAME_SIZE];
    struct mrpc_server *mrpc;
};

struct servers_database {
    struct list_head servers;
    struct mutex lock;
    struct kobject *kobj;
};

static struct servers_database db;
static unsigned int num_servers = 1;
static unsigned int delay = 0;

/**
 * demo_proc_0 - server side implementation of proc id 0
 * 
 * This is the server side procedure which is called when
 * the MRPC client calls proc id 0.
 * 
 * @param arg    argument pointer, should be NULL
 * @param arglen argument length, should be 0
 * @param rep    reply pointer, should be NULL
 * @param replen reply length, should be 0
 * 
 * @return always succeeds
 */
static int demo_proc_0(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    DBG("SUCCESS");
    ssleep(delay);
    return 0;
}

/**
 * demo_proc_1 - server side implementation of proc id 1
 * 
 * This is the server side procedure which is called when
 * the MRPC client calls proc id 1.
 * It prints the received argument number and sends a random unsigned
 * integer reply.
 * 
 * @param arg    argument pointer
 * @param arglen argument length
 * @param rep    reply pointer
 * @param replen reply length
 * 
 * @return always succeeds
 */
static int demo_proc_1(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    unsigned int *p_arg = arg;
    unsigned int *p_rep = rep;

    *p_rep = htonl(get_random_int());

    DBG("SUCCESS: arg=%u, arglen=%d, sending unsigned int random reply=%u",
           ntohl(*p_arg), arglen, ntohl(*p_rep));

    ssleep(delay);
    return 0;
}

/**
 * demo_proc_2 - server side implementation of proc id 2
 * 
 * This is the server side procedure which is called when
 * the MRPC client calls proc id 2.
 * 
 * It prints the argument structure members and sends a reply
 * structure with random integer and a string.
 * 
 * @param arg    argument pointer
 * @param arglen argument length
 * @param rep    reply pointer
 * @param replen reply length
 * 
 * @return always succeeds
 */
static int demo_proc_2(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct demo2_arg_struct *p_arg = arg;
    struct demo2_rep_struct *p_rep = rep;

    p_rep->b = htonl(get_random_int());
    strncpy(p_rep->str1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", sizeof("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    strncpy(p_rep->str2, "0987654321", sizeof("0987654321"));
    p_rep->y = htons(get_random_int());

    DBG("SUCCESS: arg->a=%d, arg->str1=%s, arg->str2=%s, arg->x=%u\n"
           "         rep->b=%u, rep->str1=%s, rep->str2=%s, rep->y=%d",
           ntohl(p_arg->a), p_arg->str1, p_arg->str2, ntohl(p_arg->x),
           ntohl(p_rep->b), p_rep->str1, p_rep->str2, ntohs(p_rep->y));

    ssleep(delay);
    return 0;
}

/**
 * demo_proc_3 - server side implementation of proc id 3
 * 
 * This is the server side procedure which is called when
 * the MRPC client calls proc id 3.
 * It simulates a "device not found" error.
 * 
 * @param arg    argument pointer
 * @param arglen argument length
 * @param rep    reply pointer
 * @param replen reply length
 * 
 * @return always fails with -ENODEV
 */
static int demo_proc_3(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    DBG("SIMULATE ERROR - no device");
    ssleep(delay);
    return -ENODEV;
}

/**
 * demo_proc_4 - server side implementation of proc id 4
 * 
 * This is the server side procedure which is called when
 * the MRPC client calls proc id 4.
 * It recieves a number and sends back this number of integers
 * from a local integer array.
 * 
 * @param arg    argument pointer
 * @param arglen argument length
 * @param rep    reply pointer
 * @param replen reply length
 * 
 * @return always succeeds
 */
static int demo_proc_4(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    static int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unsigned int *p_arg = arg;
    int *p_rep = rep;
    int i, num = ntohl(*p_arg);

    for (i = 0; i < num; i++) {
        p_rep[i] = htonl(data[i]);
    }

    DBG("SUCCESS: Sent back %d elements", num);
    ssleep(delay);
    return 0;
}

static struct mrpc_procinfo example_server_procs[] = {
    PROC(DEMO_PROC_0, demo_proc_0),
    PROC(DEMO_PROC_1, demo_proc_1),
    PROC(DEMO_PROC_2, demo_proc_2),
    PROC(DEMO_PROC_3, demo_proc_3),
    PROC(DEMO_PROC_4, demo_proc_4),
};

static ssize_t delay_get(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	return scnprintf(buf, PAGE_SIZE, "%d seconds\n", delay);
}

static ssize_t delay_set(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         const char *buf, size_t n)
{
	unsigned long val;

	if (kstrtoul(buf, 10, &val))
	    return -EINVAL;

	delay = val;

	return n;
}

static struct kobj_attribute delay_attribute =
    __ATTR(delay, S_IWUGO|S_IRUGO, delay_get, delay_set);

static struct attribute *server_attrs[] = {
    &delay_attribute.attr,
    NULL,
};

static struct attribute_group server_attrs_group = {
    .attrs = server_attrs,
};

/**
 * server_example_init
 * 
 * Initialize the server example driver
 * 
 * @return 0 for success, error code otherwise
 */
static int __init server_example_init(void)
{
    struct server *s, *tmp;
    int i, ret;

    mutex_init(&db.lock);
    INIT_LIST_HEAD(&db.servers);
    for (i = 0; i < num_servers; i++) {
        s = kzalloc(sizeof(*s), GFP_KERNEL);
        if (!s) {
            pr_err("memory allocation failed");
            ret = -ENOMEM;
            goto out_mrpc_unreg;
        }
        snprintf(s->name, MAX_NAME_SIZE, "demo-server%d", i);
        list_add(&s->list, &db.servers);
        s->mrpc = mrpc_server_register(i, s->name,
                                    example_server_procs,
                                    ARRAY_SIZE(example_server_procs));
        if (!s->mrpc) {
            pr_err("failed to register %s", s->name);
            ret = -EACCES;
            goto out_mrpc_unreg;
        }
    }

    db.kobj = kobject_create_and_add("mrpc_server_example", NULL);
    if (!db.kobj) {
        ret = -ENOMEM;
        goto out_mrpc_unreg;
    }

    ret = sysfs_create_group(db.kobj, &server_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        goto out_kobject_put;
    }

    return 0;

out_kobject_put:
    kobject_put(db.kobj);
out_mrpc_unreg:
    list_for_each_entry_safe(s, tmp, &db.servers, list) {
        mrpc_server_unregister(s->mrpc);
        list_del(&s->list);
        kfree(s);
    }

    return ret;
}

/**
 * server_example_exit
 * 
 * Remove the server example driver
 * 
 * @return 0 for success, error code otherwise
 */
static void __exit server_example_exit(void)
{
    struct server *s, *tmp;

    sysfs_remove_group(db.kobj, &server_attrs_group);
    kobject_put(db.kobj);
    list_for_each_entry_safe(s, tmp, &db.servers, list) {
        mrpc_server_unregister(s->mrpc);
        list_del(&s->list);
        kfree(s);
    }
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(server_example_init);
module_exit(server_example_exit);
module_param(num_servers, uint, 0644);
MODULE_PARM_DESC(num_servers, "number of servers");

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Tomer Eliyahu tomer.b.eliyahu@intel.com");
MODULE_DESCRIPTION("RPC over Mailbox Server example");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
