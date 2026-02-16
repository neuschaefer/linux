/*
 *
 * pp_server.c
 * Description:
 * Packet Processor over MRPC server driver
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
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/mrpc/mrpc.h>
#include <linux/mrpc/pp_server.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

/* driver private database */
struct pp_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
    struct pp_funcs *callbacks;
};

struct pp_args {
    AVALANCHE_PP_PID_t pid;
    AVALANCHE_PP_PID_RANGE_t pid_range;
    __u8 pid_number;
    unsigned int flags;
    Uint8 vpid_handle;
    Uint8 list_id;
};

#define PP_PROC(proc, func)                           \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,   						  \
        .p_arglen = sizeof(struct pp_args),           \
        .p_replen = 0,                                \
        .p_name   = #func,                            \
}

enum {
    PP_PID_CREATE_PROC = 0,
    PP_PID_SET_FLAGS_PROC,
    PP_PID_CONFIG_RANGE_PROC,
    PP_FLUSH_SESSION_PROC,
};

static struct pp_private *this;

static int mrpc_pp_pid_create(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct pp_args *args = arg;
    struct pp_private *priv = this;
    AVALANCHE_PP_PID_t pid;
    int i;

    /* copy pid from network */
    pid.pid_handle = args->pid.pid_handle;
    pid.priv_flags = args->pid.priv_flags;
    pid.type = args->pid.type;
    pid.ingress_framing = args->pid.ingress_framing;
    pid.dflt_pri_drp = args->pid.dflt_pri_drp;
    pid.pri_mapping = args->pid.pri_mapping;
    pid.dflt_fwd_q = ntohs(args->pid.dflt_fwd_q);
    pid.dflt_host_q = ntohs(args->pid.dflt_host_q);
    pid.dflt_infra_q = ntohs(args->pid.dflt_infra_q);
    pid.dflt_dst_tag = ntohs(args->pid.dflt_dst_tag);
    for (i = 0; i < ARRAY_SIZE(pid.tx_pri_q_map); i++)
        pid.tx_pri_q_map[i] = ntohs(args->pid.tx_pri_q_map[i]);
    memcpy(pid.tx_hw_data, args->pid.tx_hw_data, ARRAY_SIZE(pid.tx_hw_data));
    pid.tx_hw_data_len = args->pid.tx_hw_data_len;

    pr_info("call avalanche_pp_pid_create");
    BUG_ON(!priv->callbacks);
    return priv->callbacks->avalanche_pp_pid_create(&pid, NULL);
}

static int mrpc_pp_pid_set_flags(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct pp_args *args = arg;
    struct pp_private *priv = this;

    pr_info("call avalanche_pp_pid_set_flags(pid_number=%d, flags=%x)",
            args->pid_number, ntohl(args->flags));
    BUG_ON(!priv->callbacks);
    return priv->callbacks->avalanche_pp_pid_set_flags(args->pid_number, ntohl(args->flags)); 
}

static int mrpc_pp_pid_config_range(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct pp_args *args = arg;
    struct pp_private *priv = this;

    pr_info("call avalanche_pp_pid_config_range(port_num=%d, type=%d, count=%d, base_index=%d)",
            args->pid_range.port_num, args->pid_range.type,
            args->pid_range.count, args->pid_range.base_index);
    BUG_ON(!priv->callbacks);
    return priv->callbacks->avalanche_pp_pid_config_range(&args->pid_range);
}

static int mrpc_pp_flush_sessions(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct pp_args *flush_args = arg;
    struct pp_private *priv = this;
    Uint8  vpid_handle;
    PP_LIST_ID_e list_id;

    pr_info("call avalanche_pp_flush_sessions(vpid_handle=%d, list_id=%d)",
            flush_args->vpid_handle, ntohl(flush_args->list_id));
    vpid_handle = flush_args->vpid_handle;
    list_id = flush_args->list_id;
    BUG_ON(!priv->callbacks);
    return priv->callbacks->avalanche_pp_flush_sessions(vpid_handle, list_id);
}
static struct mrpc_procinfo pp_server_procs[] = {
    PP_PROC(PP_PID_CREATE_PROC, mrpc_pp_pid_create),
    PP_PROC(PP_PID_SET_FLAGS_PROC, mrpc_pp_pid_set_flags),
    PP_PROC(PP_PID_CONFIG_RANGE_PROC, mrpc_pp_pid_config_range),
    PP_PROC(PP_FLUSH_SESSION_PROC, mrpc_pp_flush_sessions),

};

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct pp_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *pp_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group pp_attrs_group = {
    .attrs = pp_attrs,
};

/**
 * mrpc_pp_server_unregister
 * 
 * unregister pp callbacks
 *
 * @return 0 for success, error code otherwise
 */
void mrpc_pp_server_unregister(void)
{
    BUG_ON(!this);
    BUG_ON(!this->callbacks);

    kfree(this->callbacks);
    this->callbacks = NULL;

    pr_info("callback functions unregistered\n");
}
EXPORT_SYMBOL(mrpc_pp_server_unregister);

/**
 * mrpc_pp_server_register
 *
 * register pp callbacks
 *
 * @param pp_funcs   callback functions
 *
 * @return 0 for success, error code otherwise
 */
int mrpc_pp_server_register(struct pp_funcs *callbacks)
{
    struct pp_private *priv = this;

    BUG_ON(!this);
    BUG_ON(!callbacks);
    BUG_ON(this->callbacks);

    this->callbacks = kzalloc(sizeof(*callbacks), GFP_KERNEL);
    if (!this->callbacks) {
        pr_err("Failed to allocate memory");
        return -ENOMEM;
    }

    memcpy(this->callbacks, callbacks, sizeof(*callbacks));

    pr_info("callback functions registered");
    return 0;
}
EXPORT_SYMBOL(mrpc_pp_server_register);

/**
 * pp_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int pp_probe(struct platform_device *pdev)
{
    struct pp_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &pp_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_PP_FUNC_CALL,
                                      "Packet Processor",
                                      pp_server_procs,
                                      ARRAY_SIZE(pp_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register pp");
        ret = -ENODEV;
        goto out_remove_group;
    }

    this = priv;
    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &pp_attrs_group);
    return 0;
}

/**
 * pp_remove
 *
 * This function is called when the pp driver is removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int pp_remove(struct platform_device *pdev)
{
    struct pp_private *priv = platform_get_drvdata(pdev);

    if (priv->callbacks)
        kfree(priv->callbacks);
    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &pp_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;

    return 0;
}

static struct platform_driver pp_driver = {
    .driver = {
        .name   = "pp_mrpc_server",
    },
    .probe = pp_probe,
    .remove = pp_remove,
};

static struct platform_device *pp_device;

static int __init pp_server_init(void)
{
    int ret;

    ret = platform_driver_register(&pp_driver);
    if (ret < 0) {
        pr_err("Failed to register pp platform driver: %d\n", ret);
        return ret;
    }

    pp_device = platform_device_register_simple("pp_mrpc_server", -1, NULL, 0);
    if (IS_ERR(pp_device)) {
        pr_err("Failed to register pp platform device\n");
        platform_driver_unregister(&pp_driver);
        return PTR_ERR(pp_device);
    }

    return 0;
}

static void __exit pp_server_exit(void)
{
    platform_device_unregister(pp_device);
    platform_driver_unregister(&pp_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(pp_server_init);
module_exit(pp_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Tomer Eliyahu tomer.b.eliyahu@intel.com");
MODULE_DESCRIPTION("pp mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
