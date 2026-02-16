/*
 *
 * remote_config_client.c
 * Description:
 * remote config mrpc client driver
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
#include <asm-arm/arch-avalanche/generic/avalanche_pp_api.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

#define REMOTE_CONFIG_CLIENT_MRPC_CALL_TIMEOUT_MS (1000) /* Timeout is needed due to delays in remote config server */

/**************************************************************************/
/*      LOCAL VARIABLES:                                                  */
/**************************************************************************/

static dev_t dev;
static struct cdev *c_dev;
static struct class *cl;

/* driver private database */
struct remote_config_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_client *mrpc;       /* mrpc client handle */
};

enum {
    REMOTE_CONFIG_SET_DEV_ADDR = 0
};

static ssize_t status_show(struct device *dev, struct device_attribute *attr, char *buf);
static int remote_config_mrpc_probe(struct platform_device *pdev);
static int remote_config_mrpc_remove(struct platform_device *pdev);
static inline int remote_config_mrpc_call(__u8 procid, avalanche_pp_local_dev_addr_ioctl_params_t *usr_params);

static struct remote_config_private *this;
static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *remote_config_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group remote_config_attrs_group = {
    .attrs = remote_config_attrs,
};

static struct platform_driver remote_config_driver = {
    .driver = {
        .name   = "remote_config",
    },
    .probe = remote_config_mrpc_probe,
    .remove = remote_config_mrpc_remove,
};

static struct platform_device *remote_config_device;

struct remote_config_args {
    avalanche_pp_local_dev_addr_ioctl_params_t usr_params;
};

static inline int remote_config_mrpc_call(__u8 procid,
                                          avalanche_pp_local_dev_addr_ioctl_params_t *usr_params)
{
    struct remote_config_private *priv = this;
    struct remote_config_args args;
    unsigned int reply;
    int ret, errcode;

    if (!priv) {
        pr_err("ERROR: mrpc remote_config not initialized");
        return -1;
    }

    args.usr_params.addr_type = cpu_to_be32(usr_params->addr_type);
    args.usr_params.op_type = cpu_to_be32(usr_params->op_type);

    switch (usr_params->addr_type)
    {
        /* MAC Address - no need to change endianity */
        case MTA_MAC_ADDR:
        case GW_MAC_ADDR:
        case LAN0_MAC_ADDR:
        case RND_MAC_ADDR:
        case WAN_MAC_ADDR:
        {
            memcpy(&args.usr_params.u.mac_addr, &usr_params->u.mac_addr, 6);
            break;
        }

        /* IPv4 - Change endianity */
        case MTA_RTP_IPV4_ADDR:
        case MTA_MNG_IPV4_ADDR:
        case MULTI_DROP_IPV4_ADDR:
        {
            args.usr_params.u.ipv4 = cpu_to_be32(usr_params->u.ipv4);
            break;
        }

        /* IPv6 - Change endianity */
        case MTA_RTP_IPV6_ADDR:
        case MTA_MNG_IPV6_ADDR:
        case MULTI_DROP_IPV6_ADDR:
        {
            args.usr_params.u.ipv6[0] = cpu_to_be32(usr_params->u.ipv6[0]);
            args.usr_params.u.ipv6[1] = cpu_to_be32(usr_params->u.ipv6[1]);
            args.usr_params.u.ipv6[2] = cpu_to_be32(usr_params->u.ipv6[2]);
            args.usr_params.u.ipv6[3] = cpu_to_be32(usr_params->u.ipv6[3]);
            break;
        }

        default:
        {
            pr_err("ERROR: mrpc remote_config not initialized");
            return -1;
        }
    }

    ret = mrpc_call(priv->mrpc,
                    procid,
                    &args,
                    sizeof(struct remote_config_args),
                    &reply,
                    sizeof(unsigned int),
                    REMOTE_CONFIG_CLIENT_MRPC_CALL_TIMEOUT_MS,
                    &errcode);

    if (ret || errcode) {
        pr_err("ERROR: ret=%d, errcode=%d", ret, errcode);
        return -1;
    }

    return reply;
}

/**************************************************************************
 * FUNCTION NAME : AVALANCHE_PP_RET_e AVALANCHE_PP_RET_e avalanche_pp_local_dev_addr(avalanche_pp_local_dev_addr_ioctl_params_t *param)
 **************************************************************************
 * DESCRIPTION   :
 *  The function sets local devices MAC/IPv4/IPv6 address
 *  for the packet processor.
 *  param[in] param - Local device address data structure
 * RETURNS       :
 *      0  -   Success
 *      >0  -   Error
 **************************************************************************/
AVALANCHE_PP_RET_e avalanche_pp_local_dev_addr(avalanche_pp_local_dev_addr_ioctl_params_t *param)
{
    return remote_config_mrpc_call(REMOTE_CONFIG_SET_DEV_ADDR, param);
}
EXPORT_SYMBOL(avalanche_pp_local_dev_addr);

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct remote_config_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

/**
 * remote_config_mrpc_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int remote_config_mrpc_probe(struct platform_device *pdev)
{
    struct remote_config_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &remote_config_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_client_register(MRPC_RESERVED_ID_REMOTE_CONFIG, "remote_config");
    if (!priv->mrpc) {
        pr_err("failed to register remote_config");
        ret = -ENODEV;
        goto out_remove_group;
    }

    this = priv;
    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &remote_config_attrs_group);
    return 0;
}

/**
 * remote_config_mrpc_remove
 *
 * This function is called when the remote config mrpc driver is
 * removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int remote_config_mrpc_remove(struct platform_device *pdev)
{
    struct remote_config_private *priv = platform_get_drvdata(pdev);
    mrpc_client_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &remote_config_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;

    return 0;
}

static int __init remote_config_mrpc_init(void)
{
    int ret;

    ret = platform_driver_register(&remote_config_driver);
    if (ret < 0) {
        pr_err("Failed to register remote_config_ platform driver: %d\n", ret);
        return ret;
    }

    remote_config_device = platform_device_register_simple("remote_config", -1, NULL, 0);
    if (IS_ERR(remote_config_device)) {
        pr_err("Failed to register remote_config platform device\n");
        platform_driver_unregister(&remote_config_driver);
        return PTR_ERR(remote_config_device);
    }

    return 0;
}
static void __exit remote_config_mrpc_exit(void)
{
    platform_device_unregister(remote_config_device);
    platform_driver_unregister(&remote_config_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(remote_config_mrpc_init);
module_exit(remote_config_mrpc_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Ofir Bitton ofir1.bitton@intel.com");
MODULE_DESCRIPTION("remote config mrpc client");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
