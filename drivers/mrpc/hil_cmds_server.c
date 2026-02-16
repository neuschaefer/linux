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
#include <linux/ti_hil.h>
#include <linux/mrpc/hil_cmds.h>
#include <linux/mrpc/conntrack_server.h>
#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

#define CMD_LEN 200

/* driver private database */
struct hil_cmds_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
    struct ct_pp_funcs *pp_dev_callbacks;
    struct hil_funcs *pp_hil_callbacks;
};

struct hil_cmds_args {
    Uint8 pp_cmd[CMD_LEN];
    unsigned int count;
};

static int hil_write_cmds(void *args, ssize_t arglen, void *rep, ssize_t replen);

#define PP_PROC(proc, func)                           \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,                     \
        .p_arglen = sizeof(struct hil_cmds_args),     \
        .p_replen = 0,                                \
        .p_name   = #func,                            \
}

enum {
HIL_CMDS_HANDLE_PROC = 0,
};

static struct hil_cmds_private *this;


static struct mrpc_procinfo hil_cmds_server_procs[] = {
    PP_PROC(HIL_CMDS_HANDLE_PROC, hil_write_cmds),
};

static ssize_t hil_write_cmds(void *args, ssize_t arglen, void *rep, ssize_t replen)
{
    struct hil_cmds_args *cmd_args = args;
    Int8    pp_cmd[200];
    Int8*   argv[10];
    Int32   argc = 0;
    Int8*   ptr_cmd;
    Int8*   delimitters = " \n\t";
    char*   ptr_next_tok;
    unsigned int count;
    struct hil_cmds_private *priv = this;
    count = ntohl(cmd_args->count);
    
    memset ((void *)&pp_cmd[0], 0, count+1);
    memset ((void *)&argv[0], 0, sizeof(argv));
    memcpy(&pp_cmd, &cmd_args->pp_cmd, count);
    if (pp_cmd[0] == 0)
    {
         pr_info("pp_cmd not valid ARM");
         return -EINVAL;
    }

    /* Extract the first command. */
    ptr_next_tok = pp_cmd;

    /* Parse all the commands typed. */
    while (1)
    {
        ptr_cmd = strsep(&ptr_next_tok, delimitters);
        if (ptr_cmd == NULL)
        {
            /* 'strsep' returns null if there was no tok when it gets to '\0' */
            break;
        }
        argv[argc++] = ptr_cmd;

        if (ptr_next_tok == NULL)
        {
            /* no next tok*/
            break;
        }
        /* Validate if the user entered more commands.*/
        if (argc >=5)
        {
            printk(KERN_ERR "ERROR: Incorrect too many parameters dropping the command\n");
            return -EFAULT;
        }
    }

    /* cable_pp: disable/enable capability */
    if (strcmp(argv[0], "enable") == 0)
    {
        printk(KERN_NOTICE "HIL PP Enable invoked from ATOM.\n");
        priv->pp_hil_callbacks->pp_hil_set_disable_state(False);
    }
    else if (strcmp(argv[0], "disable") == 0)
    {
        printk(KERN_NOTICE "HIL PP Disable invoked from ATOM.\n");
        priv->pp_hil_callbacks->pp_hil_set_disable_state(True);
        priv->pp_dev_callbacks->avalanche_pp_flush_sessions(AVALANCHE_PP_MAX_VPID, 4);
    }
    else if (strcmp(argv[0], "flush_all_sessions") == 0)
    {
        printk(KERN_NOTICE "HIL PP flush_all_sessions invoked from ATOM.\n");
        priv->pp_dev_callbacks->avalanche_pp_flush_sessions(AVALANCHE_PP_MAX_VPID, 4);
    }

    return 0;
}
/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct hil_cmds_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *hil_cmds_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group hil_cmds_attrs_group = {
    .attrs = hil_cmds_attrs,
};

/**
 * mrpc_hil_cmds_server_unregister
 * 
 * unregister pp callbacks
 *
 * @return 0 for success, error code otherwise
 */
void mrpc_hil_cmds_server_unregister(int module)
{
    BUG_ON(!this);
    /*mrpc callbacks unregistered by pp_drv module */
    if (module == PP_DEV)
    {
       BUG_ON(!this->pp_dev_callbacks);
       kfree(this->pp_dev_callbacks);
       this->pp_dev_callbacks = NULL;
       pr_info("PP_DEV: callback functions unregistered\n");
    }
    /*mrpc callbacks unregistered by hil_drv */
    else if (module == PP_HIL)
    {
       BUG_ON(!this->pp_hil_callbacks);
       kfree(this->pp_hil_callbacks);
       this->pp_hil_callbacks = NULL;
       pr_info("PP_HIL: callback functions unregistered\n");
    }
}
EXPORT_SYMBOL(mrpc_hil_cmds_server_unregister);

/**
 * mrpc_hil_cmds_server_register
 *
 * register pp callbacks
 *
 * @param pp_funcs   callback functions
 *
 * @return 0 for success, error code otherwise
 */
int mrpc_hil_cmds_server_register(void *callbacks, int module)
{
    struct hil_cmds_private *priv = this;

    BUG_ON(!this);
    /*mrpc callbacks registered by pp_drv module */
    if (module == PP_DEV)
    {      
       BUG_ON(this->pp_dev_callbacks);
       this->pp_dev_callbacks = kzalloc(sizeof(struct ct_pp_funcs), GFP_KERNEL);
    
       if (!this->pp_dev_callbacks) {
           pr_err("PP_DEV Callback Register: Failed to allocate memory");
           return -ENOMEM;
       }
       memcpy(this->pp_dev_callbacks, callbacks, sizeof(struct ct_pp_funcs));
       pr_info("PP_DEV: hil_cmds callback functions registered");
    }
    /*mrpc callbacks registered by hil_drv */
    else if (module == PP_HIL)
    {
       BUG_ON(this->pp_hil_callbacks);
       this->pp_hil_callbacks = kzalloc(sizeof(struct hil_funcs), GFP_KERNEL);

       if (!this->pp_hil_callbacks) {
           pr_err("PP_HIL Callback Register: Failed to allocate memory");
           return -ENOMEM;
       }
       memcpy(this->pp_hil_callbacks, callbacks, sizeof(struct hil_funcs));
       pr_info("PP_HIL: hil_cmds callback functions registered");
    }
    return 0;
}
EXPORT_SYMBOL(mrpc_hil_cmds_server_register);

/**
 * hil_cmds_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int hil_cmds_probe(struct platform_device *pdev)
{
    struct hil_cmds_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &hil_cmds_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_HILCMDS,
                                      "Hil Cmds Handle",
                                      hil_cmds_server_procs,
                                      ARRAY_SIZE(hil_cmds_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register hil cmds handle");
        ret = -ENODEV;
        goto out_remove_group;
    }

    this = priv;
    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &hil_cmds_attrs_group);
    return 0;
}

/**
 * hil_cmds_remove
 *
 * This function is called when the pp driver is removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int hil_cmds_remove(struct platform_device *pdev)
{
    struct hil_cmds_private *priv = platform_get_drvdata(pdev);

    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &hil_cmds_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);
    this = NULL;
    return 0;
}

static struct platform_driver hil_cmds_driver = {
    .driver = {
        .name   = "hil_cmds_mrpc_server",
    },
    .probe = hil_cmds_probe,
    .remove = hil_cmds_remove,
};

static struct platform_device *hil_cmds_device;

static int __init hil_cmds_server_init(void)
{
    int ret;

    ret = platform_driver_register(&hil_cmds_driver);
    if (ret < 0) {
        pr_err("Failed to register hil cmds handle platform driver: %d\n", ret);
        return ret;
    }

    hil_cmds_device = platform_device_register_simple("hil_cmds_mrpc_server", -1, NULL, 0);
    if (IS_ERR(hil_cmds_device)) {
        pr_err("Failed to register hil cmds platform device\n");
        platform_driver_unregister(&hil_cmds_driver);
        return PTR_ERR(hil_cmds_device);
    }

    return 0;
}

static void __exit hil_cmds_server_exit(void)
{
    platform_device_unregister(hil_cmds_device);
    platform_driver_unregister(&hil_cmds_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(hil_cmds_server_init);
module_exit(hil_cmds_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Arunkumar Velayutham arunkumar.velayutham@intel.com");
MODULE_DESCRIPTION("Hil cmds handle mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
