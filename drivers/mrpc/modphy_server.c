/*
 *
 * modphy.c
 * Description:
 * modphy over MRPC server driver
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
#include <asm-arm/arch-avalanche/puma7/modphy_api.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

/* driver private database */
struct modphy_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
};

struct modphy_args {
    unsigned int client;
    unsigned int lane;
};

#define MODPHY_PROC(proc, func)                       \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,   						  \
        .p_arglen = sizeof(struct modphy_args),       \
        .p_replen = 0,                                \
        .p_name   = #func,                            \
}

enum {
    MODPHY_INIT_PROC = 0,
    MODPHY_LANE_CONFIGURE_PROC,
    MODPHY_TX_DISABLE_PROC,
    MODPHY_RESET_K_ALIGN_PROC,
    MODPHY_RESET_CLIENT_PROC,
};

static int mrpc_modphy_init(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct modphy_args *p_arg = arg;
    ModphyController_e client = ntohl(p_arg->client);

    pr_info("call modphy_init for client %d", client);
    return modphy_init(client);
}

static int mrpc_modphy_lane_configure(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct modphy_args *p_arg = arg;
    ModphyController_e client = ntohl(p_arg->client);
    ModphyLanes_e lane = ntohl(p_arg->lane);

    pr_info("call modphy_lane_configure for client %d, lane %d", client, lane);
    return modphy_lane_configure(lane, client);
}

static int mrpc_modphy_tx_disable(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct modphy_args *p_arg = arg;
    ModphyController_e client = ntohl(p_arg->client);
    ModphyLanes_e lane = ntohl(p_arg->lane);

    pr_info("call modphy_tx_disable for client %d, lane %d", client, lane);
    return modphy_tx_disable(lane, client);
}

static int mrpc_modphy_reset_k_align(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct modphy_args *p_arg = arg;
    ModphyController_e client = ntohl(p_arg->client);

    pr_info("call modphy_reset_k_align for client %d", client);
    return modphy_reset_k_align(client);
}

static int mrpc_modphy_reset_client(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct modphy_args *p_arg = arg;
    ModphyController_e client = ntohl(p_arg->client);

    pr_info("call modphy_reset_client for client %d", client);
    modphy_reset_client(client);
    return 0;
}

static struct mrpc_procinfo modphy_server_procs[] = {
    MODPHY_PROC(MODPHY_INIT_PROC, mrpc_modphy_init),
    MODPHY_PROC(MODPHY_LANE_CONFIGURE_PROC, mrpc_modphy_lane_configure),
    MODPHY_PROC(MODPHY_TX_DISABLE_PROC, mrpc_modphy_tx_disable),
    MODPHY_PROC(MODPHY_RESET_K_ALIGN_PROC, mrpc_modphy_reset_k_align),
    MODPHY_PROC(MODPHY_RESET_CLIENT_PROC, mrpc_modphy_reset_client),
};

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct modphy_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *modphy_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group modphy_attrs_group = {
    .attrs = modphy_attrs,
};

/**
 * modphy_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int modphy_probe(struct platform_device *pdev)
{
    struct modphy_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &modphy_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_MODPHY, "modphy",
                                    modphy_server_procs,
                                    ARRAY_SIZE(modphy_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register modphy");
        ret = -ENODEV;
        goto out_remove_group;
    }

    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &modphy_attrs_group);
    return 0;
}

/**
 * modphy_remove
 *
 * This function is called when the modphy driver is removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int modphy_remove(struct platform_device *pdev)
{
    struct modphy_private *priv = platform_get_drvdata(pdev);

    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &modphy_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);

    return 0;
}

static struct platform_driver modphy_driver = {
    .driver = {
        .name   = "modphy",
    },
    .probe = modphy_probe,
    .remove = modphy_remove,
};

static struct platform_device *modphy_device;

static int __init modphy_server_init(void)
{
    int ret;

    ret = platform_driver_register(&modphy_driver);
    if (ret < 0) {
        pr_err("Failed to register modphy platform driver: %d\n", ret);
        return ret;
    }

    modphy_device = platform_device_register_simple("modphy", -1, NULL, 0);
    if (IS_ERR(modphy_device)) {
        pr_err("Failed to register modphy platform device\n");
        platform_driver_unregister(&modphy_driver);
        return PTR_ERR(modphy_device);
    }

    return 0;
}

static void __exit modphy_server_exit(void)
{
    platform_device_unregister(modphy_device);
    platform_driver_unregister(&modphy_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(modphy_server_init);
module_exit(modphy_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Tomer Eliyahu tomer.b.eliyahu@intel.com");
MODULE_DESCRIPTION("modphy mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
