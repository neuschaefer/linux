/*
 *
 * cppi_client.c
 * Description:
 * cppi mrpc client driver
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2017 Intel Corporation.
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
#include <linux/skbuff.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

/* driver private database */
struct cppi_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_client *mrpc;       /* mrpc client handle */
};

static struct cppi_private *this;

struct cppi_pp_buf_init_args {
    Uint32 size;
};

struct mrpc_cppi_get_skb_shared_info_size_args {
    Uint32 sizeof_skb_shared_info;
};

typedef enum {
    SKB_SHARED_INFO_STATUS_OK = 0,
    SKB_SHARED_INFO_STATUS_NOK,
} skb_shared_info_status_e;

enum {
    CPPI_INIT_PP_BUFFER_POOL = 0,
    CPPI_GET_SKB_SHARED_INFO,
};

static inline int cppi_mrpc_call(__u8 procid, void *args, ssize_t args_size )
{
    struct cppi_private *priv = this;
    int ret, errcode;
    Uint32 rep;

    if (!priv) {
        pr_err("ERROR: mrpc cppi not initialized");
        return -EFAULT;
    }

    ret = mrpc_call(priv->mrpc, procid, args, args_size, &rep, sizeof(rep),
								   0, &errcode);

    if (ret || errcode) {
        pr_err("ERROR: ret=%d, errcode=%d", ret, errcode);
        return -EFAULT;
    }

    return be32_to_cpu(rep);
}

Uint32 avalanche_cppi_alloc_pp_buffer_pool(Uint32 size)
{
    struct cppi_pp_buf_init_args args;
    Uint32 res;
    DBG("MRPC client avalanche_cppi_init_pp_buffer_pool, size: %d\n", size);

    args.size = be32_to_cpu(size);             
    res = cppi_mrpc_call(CPPI_INIT_PP_BUFFER_POOL, &args, sizeof(args));

    if (res == NULL || res == -EFAULT)
    {
        return NULL;
    }

    return res;
}
EXPORT_SYMBOL(avalanche_cppi_alloc_pp_buffer_pool);

Int32 avalanche_cppi_get_skb_shared_info_size( void )
{
    struct mrpc_cppi_get_skb_shared_info_size_args args;
    Uint32 res;
                    
    res = cppi_mrpc_call(CPPI_GET_SKB_SHARED_INFO, &args, sizeof(args));

    DBG("MRPC client avalanche_cppi_get_skb_shared_info_size, res: %d\n", res);

    if (res == NULL || res == -EFAULT)
    {
        printk(KERN_ERR "MRPC client avalanche_cppi_get_skb_shared_info_size failed\n");
        return 0;
    }

    return res;
}
EXPORT_SYMBOL(avalanche_cppi_get_skb_shared_info_size);

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct cppi_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *cppi_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group cppi_attrs_group = {
    .attrs = cppi_attrs,
};

/**
 * cppi_mrpc_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int cppi_mrpc_probe(struct platform_device *pdev)
{
    struct cppi_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_client_register(MRPC_RESERVED_ID_CPPI, "cppi_client");
    if (!priv->mrpc) {
        pr_err("failed to register cppi");
        ret = -ENODEV;
        goto out_remove_group;
    }

    this = priv;
    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    return 0;
}

/**
 * cppi_mrpc_remove
 *
 * This function is called when the cppi mrpc driver is
 * removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int cppi_mrpc_remove(struct platform_device *pdev)
{
    struct cppi_private *priv = platform_get_drvdata(pdev);
    mrpc_client_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;

    return 0;
}

static struct platform_driver cppi_driver = {
    .driver = {
        .name   = "cppi_client",
    },
    .probe = cppi_mrpc_probe,
    .remove = cppi_mrpc_remove,
};

static struct platform_device *cppi_device;

static int __init cppi_mrpc_init(void)
{
    int ret;

    ret = platform_driver_register(&cppi_driver);
    if (ret < 0) {
        pr_err("Failed to register cppi platform driver: %d\n", ret);
        return ret;
    }

    cppi_device = platform_device_register_simple("cppi_client", -1, NULL, 0);
    if (IS_ERR(cppi_device)) {
        pr_err("Failed to register cppi platform device\n");
        platform_driver_unregister(&cppi_driver);
        return PTR_ERR(cppi_device);
    }

    return 0;
}

static void __exit cppi_mrpc_exit(void)
{
    platform_device_unregister(cppi_device);
    platform_driver_unregister(&cppi_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(cppi_mrpc_init);
module_exit(cppi_mrpc_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Yair Weiss yair.weiss@intel.com");
MODULE_DESCRIPTION("cppi mrpc client");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
