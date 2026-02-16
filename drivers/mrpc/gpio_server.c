/*
 *
 * gpio.c
 * Description:
 * gpio over MRPC server driver
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
#include <puma7_gpio_ctrl.h>
#include <pal.h>

/* driver private database */
struct gpio_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
};

#define GPIO_PROC(proc, func)                         \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,   						  \
        .p_arglen = sizeof(struct gpio_user_info),    \
        .p_replen = 0,                                \
        .p_name   = #func,                            \
}

enum {
    GPIO_SET_DIR_PROC = 0,
    GPIO_SET_VAL_PROC
};

static int mrpc_gpio_set_dir(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct gpio_user_info *p_arg = arg;
    int gpio = ntohl(p_arg->gpio_pin);
    int dir = ntohl(p_arg->pin_direction);
    pr_debug("--->");

    return PAL_sysGpioCtrlSetDir(gpio, dir);
}

static int mrpc_gpio_set_val(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    struct gpio_user_info *p_arg = arg;
    int gpio = ntohl(p_arg->gpio_pin);
    int val = ntohl(p_arg->value);
    pr_debug("--->");

    return PAL_sysGpioOutBit(gpio, val);
}

static struct mrpc_procinfo gpio_server_procs[] = {
    GPIO_PROC(GPIO_SET_DIR_PROC, mrpc_gpio_set_dir),
    GPIO_PROC(GPIO_SET_VAL_PROC, mrpc_gpio_set_val)
};

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct gpio_private *priv = dev_get_drvdata(dev);
    pr_debug("--->");

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *gpio_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group gpio_attrs_group = {
    .attrs = gpio_attrs,
};

/**
 * gpio_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int gpio_probe(struct platform_device *pdev)
{
    struct gpio_private *priv;
    int ret;

    pr_debug("--->");

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &gpio_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_GPIO, "gpio",
                                    gpio_server_procs,
                                    ARRAY_SIZE(gpio_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register gpio");
        ret = -ENODEV;
        goto out_remove_group;
    }

    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &gpio_attrs_group);
    return 0;
}

/**
 * gpio_remove
 *
 * This function is called when the gpio driver is removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int gpio_remove(struct platform_device *pdev)
{
    struct gpio_private *priv = platform_get_drvdata(pdev);
    pr_debug("--->");

    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &gpio_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);

    return 0;
}

static struct platform_driver gpio_driver = {
    .driver = {
        .name   = "gpio",
    },
    .probe = gpio_probe,
    .remove = gpio_remove,
};

static struct platform_device *gpio_device;

static int __init gpio_server_init(void)
{
    int ret;
    pr_debug("--->");

    ret = platform_driver_register(&gpio_driver);
    if (ret < 0) {
        pr_err("Failed to register gpio platform driver: %d\n", ret);
        return ret;
    }
    pr_debug("Done to register gpio platform driver: %d\n", ret);

    gpio_device = platform_device_register_simple("gpio", -1, NULL, 0);
    if (IS_ERR(gpio_device)) {
        pr_err("Failed to register gpio platform device\n");
        platform_driver_unregister(&gpio_driver);
        return PTR_ERR(gpio_device);
    }
    pr_debug("Done to register gpio platform device\n");

    return 0;
}

static void __exit gpio_server_exit(void)
{
    pr_debug("--->");

    platform_device_unregister(gpio_device);
    platform_driver_unregister(&gpio_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(gpio_server_init);
module_exit(gpio_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("gpio mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
