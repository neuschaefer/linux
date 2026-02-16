/*
 *
 * wifi_proxy_server.c
 * Description:
 * wifi_proxy over MRPC server driver
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
#include <linux/mrpc/wifi_proxy_server.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

/* driver private database */
struct wifi_proxy_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
    struct wifi_proxy_funcs *callbacks;
};

struct wifi_proxy_args {
    wifi_dev_id_e   devId;
    wifi_vap_id_e   vapId;
    Uint32          rate;
    Uint32          pps;
};

#define WIFI_PROXY_PROC(proc, func)                   \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,   						  \
        .p_arglen = sizeof(struct wifi_proxy_args),   \
        .p_replen = sizeof(unsigned int),             \
        .p_name   = #func,                            \
}

enum {
    WIFI_PROXY_PID_CREATE_PROC = 0,
    WIFI_PROXY_PID_DELETE_PROC,
    WIFI_PROXY_VAP_CREATE_PROC,
    WIFI_PROXY_VAP_DELETE_PROC,
};

static struct wifi_proxy_private *this;

static int mrpc_wifi_proxy_pid_create(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    int     retCode;
    Int32   *reply = (Int32*)rep;
    Uint8   pid_handle = 0xFF;
    struct  wifi_proxy_args *args = arg;
    struct  wifi_proxy_private *priv = this;

    if (priv->callbacks == NULL)
    {
         pr_err("priv->callbacks is NULL");
         return 0;
    }

    /* Create new wifi_proxy pid according to devId, return actual pid number to be placed in descriptor */
    retCode = priv->callbacks->wifi_proxy_pid_create(be32_to_cpu(args->devId), &pid_handle);
    if (pid_handle == 0xFF)
    {
        *reply = -1;
    }
    else
    {
        *reply = pid_handle;
    }

    *reply = cpu_to_be32(*reply);
    return retCode;
}

static int mrpc_wifi_proxy_pid_delete(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    int    retCode;
    Int32  *reply = (Int32*)rep;
    struct wifi_proxy_args *args = arg;
    struct wifi_proxy_private *priv = this;

    pr_info("call wifi_proxy_pid_delete");
    BUG_ON(!priv->callbacks);
    retCode = priv->callbacks->wifi_proxy_pid_delete(be32_to_cpu(args->devId), reply);

    *reply = cpu_to_be32(*reply);
    return retCode;
}

static int mrpc_wifi_proxy_vap_create(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    int     retCode;
    Int32   *reply = (Int32*)rep;
    Uint8   vpid_handle = 0xFF;
    struct  wifi_proxy_args *args = arg;
    struct  wifi_proxy_private *priv = this;

    pr_info("call wifi_proxy_vap_create");
    BUG_ON(!priv->callbacks);

    /* Create new wifi_proxy sub interface according to devId and vapId, return vpid_handle  */
    retCode = priv->callbacks->wifi_proxy_vap_create(be32_to_cpu(args->devId), be32_to_cpu(args->vapId), be32_to_cpu(args->rate), be32_to_cpu(args->pps), &vpid_handle);
    if (vpid_handle == 0xFF)
    {
        *reply = -1;
    }
    else
    {
        *reply = vpid_handle;
    }

    *reply = cpu_to_be32(*reply);
    return retCode;
}

static int mrpc_wifi_proxy_vap_delete(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    int    retCode;
    Int32  *reply = (Int32*)rep;
    struct wifi_proxy_args *args = arg;
    struct wifi_proxy_private *priv = this;

    pr_info("call wifi_proxy_vap_delete");
    BUG_ON(!priv->callbacks);
    retCode = priv->callbacks->wifi_proxy_vap_delete(be32_to_cpu(args->devId), be32_to_cpu(args->vapId), reply); 

    *reply = cpu_to_be32(*reply);
    return retCode;
}

static struct mrpc_procinfo wifi_proxy_server_procs[] = {
    WIFI_PROXY_PROC(WIFI_PROXY_PID_CREATE_PROC, mrpc_wifi_proxy_pid_create),
    WIFI_PROXY_PROC(WIFI_PROXY_PID_DELETE_PROC, mrpc_wifi_proxy_pid_delete),
    WIFI_PROXY_PROC(WIFI_PROXY_VAP_CREATE_PROC, mrpc_wifi_proxy_vap_create),
    WIFI_PROXY_PROC(WIFI_PROXY_VAP_DELETE_PROC, mrpc_wifi_proxy_vap_delete),

};

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct wifi_proxy_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *wifi_proxy_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group wifi_proxy_attrs_group = {
    .attrs = wifi_proxy_attrs,
};

/**
 * mrpc_wifi_proxy_server_unregister
 * 
 * unregister wifi_proxy callbacks
 *
 * @return 0 for success, error code otherwise
 */
void mrpc_wifi_proxy_server_unregister(void)
{
    BUG_ON(!this);
    BUG_ON(!this->callbacks);

    kfree(this->callbacks);
    this->callbacks = NULL;

    pr_info("callback functions unregistered\n");
}
EXPORT_SYMBOL(mrpc_wifi_proxy_server_unregister);

/**
 * mrpc_wifi_proxy_server_register
 *
 * register wifi_proxy callbacks
 *
 * @param wifi_proxy_funcs   callback functions
 *
 * @return 0 for success, error code otherwise
 */
int mrpc_wifi_proxy_server_register(struct wifi_proxy_funcs *callbacks)
{
    struct wifi_proxy_private *priv = this;

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
EXPORT_SYMBOL(mrpc_wifi_proxy_server_register);

/**
 * wifi_proxy_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int wifi_proxy_probe(struct platform_device *pdev)
{
    struct wifi_proxy_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &wifi_proxy_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_WIFI_PROXY_FUNC_CALL,
                                      "Wifi Proxy",
                                      wifi_proxy_server_procs,
                                      ARRAY_SIZE(wifi_proxy_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register wifi_proxy");
        ret = -ENODEV;
        goto out_remove_group;
    }

    this = priv;
    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &wifi_proxy_attrs_group);
    return 0;
}

/**
 * wifi_proxy_remove
 *
 * This function is called when the wifi_proxy driver is
 * removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int wifi_proxy_remove(struct platform_device *pdev)
{
    struct wifi_proxy_private *priv = platform_get_drvdata(pdev);

    if (priv->callbacks)
        kfree(priv->callbacks);
    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &wifi_proxy_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;

    return 0;
}

static struct platform_driver wifi_proxy_driver = {
    .driver = {
        .name   = "wifi_proxy_mrpc_server",
    },
    .probe = wifi_proxy_probe,
    .remove = wifi_proxy_remove,
};

static struct platform_device *wifi_proxy_device;

static int __init wifi_proxy_server_init(void)
{
    int ret;

    printk("wifi_proxy_server_init: wifi_proxy_args_size=%d\n", sizeof(struct wifi_proxy_args));
    ret = platform_driver_register(&wifi_proxy_driver);
    if (ret < 0) {
        pr_err("Failed to register wifi_proxy platform driver: %d\n", ret);
        return ret;
    }

    wifi_proxy_device = platform_device_register_simple("wifi_proxy_mrpc_server", -1, NULL, 0);
    if (IS_ERR(wifi_proxy_device)) {
        pr_err("Failed to register wifi_proxy platform device\n");
        platform_driver_unregister(&wifi_proxy_driver);
        return PTR_ERR(wifi_proxy_device);
    }

    return 0;
}

static void __exit wifi_proxy_server_exit(void)
{
    platform_device_unregister(wifi_proxy_device);
    platform_driver_unregister(&wifi_proxy_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(wifi_proxy_server_init);
module_exit(wifi_proxy_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Ofir Bitton ofir1.bitton@intel.com");
MODULE_DESCRIPTION("wifi_proxy mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
