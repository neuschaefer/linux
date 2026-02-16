/*
 *
 * led_manager.c
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

/******************************************************************************
 * FILE PURPOSE:     - LED manager module Source
 ******************************************************************************
 * FILE NAME:     led_manager.c
 *
 * DESCRIPTION:   Linux LED manager character driver implementation
 *
 *
 *******************************************************************************/

/* Start - Header Includes */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/version.h>
#include <linux/proc_fs_macros.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/spinlock.h> 
#include <asm-arm/arch-avalanche/generic/avalanche_intc.h>
#include <asm-arm/arch-avalanche/generic/led_hal.h>
#include <asm-arm/arch-avalanche/generic/led_platform.h>
#include <asm-arm/arch-avalanche/generic/led_manager.h>

/* End - Header Includes */                                                                                   
#include <linux/moduleparam.h>
#include <linux/platform_device.h> 
#include <linux/cdev.h>

#define TI_LED_VERSION          "2.5"

dev_t tiled_dev_num; /* the dynamically allocated devive number*/


struct class *tiled_class; /* the class structure for adding to sysfs*/
struct device *tiled_class_dev; /* the class_device structure for adding device to sysfs*/

/* LED sysfs entry pointer */      
static struct platform_device *tiled_device = NULL;

/* sysfs version support */
static ssize_t show_driver_version_led(struct device_driver * dev, char *buf)
{
    return sprintf (buf ,"%s\n" ,TI_LED_VERSION);
}

static DRIVER_ATTR(version, S_IRUGO, show_driver_version_led, NULL);

/* dummy function. Necessary while initializing the device_driver structure
 * for sysfs*/
static int led_probe_dev(struct device *dev)
{
    return 0;
}

struct device_driver tiled_driver = {
        .name       = "led",
        .bus        = NULL,
        .probe      = led_probe_dev,
        .remove     = NULL,
        .suspend    = NULL,
        .resume     = NULL,
};


static int init_state = 0; // not initialized as yet.

static int _led_manager_init(void)
{
    if(!init_state)
    {
        led_hal_init();
        init_state = 1;
    }

    return(0);
}

int led_manager_init(void)
{
    return (_led_manager_init());
}

MOD_OBJ_HND *led_manager_register_module(char *module_name, int instance )
{
    return (led_hal_register(module_name,instance));
}

void led_manager_unregister_module( MOD_OBJ_HND *module_handle )
{
    led_hal_unregister(module_handle);
}

int led_manager_led_action( MOD_OBJ_HND *module_handle, int state_id )
{
    return (led_hal_action(module_handle,state_id));
}

LED_OBJ_HND * led_manager_install_callbacks( LED_FUNCS_T *funcs )
{
    return (led_hal_install_callbacks(funcs));
}

int led_manager_uninstall_callbacks( LED_OBJ_HND *led_handle)
{
    return (led_hal_uninstall_callbacks(led_handle));
}

int led_manager_cfg_mod( MOD_CFG_T *mod_cfg)
{
    if(mod_cfg)
        return (led_hal_configure_mod(mod_cfg));

    return (-1);
}

static long led_ioctl(struct file * file, unsigned int cmd, unsigned long arg )                     
{
    int ret = 0;

    switch ( cmd )
    {
        case LED_MANAGER_CONFIG:
        {
            MOD_CFG_T mod_cfg;

            if (copy_from_user((char *)&mod_cfg, (char *)arg, sizeof(mod_cfg)))
            {
                ret = -EFAULT;
                break;
            }
            ret = led_hal_configure_mod(&mod_cfg);
        }
        break;

        case LED_MANAGER_REGISTER:
        {
            struct led_manager_user_module user_mod;

            if (copy_from_user((char *)&user_mod, (char *)arg, sizeof(user_mod)))
            {
                ret =  -EFAULT;
                break;
            }

            user_mod.handle =  (unsigned int) led_manager_register_module(user_mod.name, user_mod.instance);
            if(!user_mod.handle)
            {
                ret =  -EFAULT;
                break;
            }

            if (copy_to_user((char *)arg, (char *)(&user_mod), sizeof(user_mod)))
            {
                ret =  -EFAULT;
                break;
            }
        }
        break;

        case LED_MANAGER_UNREGISTER:
        {
            struct led_manager_user_module user_mod;

            /* Copy module structure from user space */
            if (copy_from_user((char*)&user_mod, (char*)arg, sizeof(user_mod)))
            {
               ret = -EFAULT;
               break;
            }
           /* Unregister module */
           led_manager_unregister_module( ( void *)user_mod.handle );
        }
        break;

        case LED_MANAGER_ACTION:
        {
            struct led_manager_user_action user_act;

            if (copy_from_user((char *)&user_act, (char *)arg, sizeof(user_act)))
            {
                ret =  -EFAULT;
                break;
            }

            led_manager_led_action((void *)user_act.handle,user_act.state_id);
        }
        break;

        default:
            ret = -EINVAL;
    }

    return ret;
}

static int led_open( struct inode * inode, struct file * file )
{
    return 0;
}

static int led_close( struct inode * inode, struct file * file )
{
    return 0;
}

struct file_operations led_fops = {
    unlocked_ioctl:    led_ioctl,
              open:    led_open,
           release:    led_close
};


/* Proc function to display driver version */
static int
led_ver_info(struct seq_file *m, void *v)
{
    seq_printf(m,"\nTI Linux LED Driver Version %s\n",TI_LED_VERSION);

    return 0;
}

/* proc interface /proc/avalanche/led  */
int led_cfg_info(struct seq_file *m, void *v)
{
    return led_hal_dump_cfg_info(m);
}

DECLARE_PROCFS_READ_ENTRY(led_ver_info, led_ver_info)
DECLARE_PROCFS_READ_ENTRY(led_cfg_info, led_cfg_info)

int __init led_drv_init(void)
{

    int tiled_major_num,error_num;

    tiled_device = platform_device_register_simple("led", -1, NULL, 0);

    if (IS_ERR(tiled_device)) {
        printk ("platform device register failed from TI LED\n");
        error_num = PTR_ERR(tiled_device);
        goto exit_init;
    }

    tiled_driver.bus = platform_bus_type_ptr;
    error_num = driver_register(&tiled_driver);

    if (error_num < 0)
    {
        printk ("driver register to sysfs failed for TI LED\n");
        goto exit_init;
    }

    error_num = driver_create_file(&tiled_driver, &driver_attr_version);
    if (error_num < 0)
    {
        printk ("driver create sysfs file failed for TI LED\n");
        goto exit_init;
    }

    tiled_major_num = register_chrdev (0,"led", &led_fops);
    if (tiled_major_num < 0)
    {
        printk ("could not create device for ti led\n");
        goto exit_init;
    }

    tiled_dev_num =  MKDEV(tiled_major_num, 0);

    tiled_class = class_create (THIS_MODULE, "led");


    if (IS_ERR(tiled_class))
    {
        printk ("could not create class for TI Led\n");
        goto exit_init;
    }

    tiled_class_dev = device_create(tiled_class, NULL,
                                tiled_dev_num, &tiled_device->dev,
                                "%s%d", "led", 0);

    if(IS_ERR(tiled_class_dev))
    {
        printk ("could not create class_device for TI Led\n");
        goto exit_init;
    }

#if 0
    if (class_device_add (tiled_class_dev) < 0)
    {
        printk ("class device add failed for TI Led\n");
        goto exit_init;
    }
#endif

    _led_manager_init();

    /* create proc entry */
    proc_create("avalanche/led_cfg", 0, NULL, &led_cfg_info_proc_fops);
    proc_create("avalanche/led_ver", 0, NULL, &led_ver_info_proc_fops);
    printk("TI LED driver initialized [major=%d]\n", tiled_major_num);
    return 0;

exit_init:
#if 0
    if (tiled_class_dev)
        class_device_del (tiled_class_dev);
#endif
    if(tiled_dev_num > 0)
        /*class_device_destroy (tiled_class, tiled_dev_num);*/
        device_destroy (tiled_class, tiled_dev_num);
    if (!IS_ERR(tiled_class))
        class_destroy (tiled_class);
    if (tiled_device)
        platform_device_unregister(tiled_device);
    driver_remove_file(&tiled_driver, &driver_attr_version);

    unregister_chrdev(MAJOR(tiled_dev_num),"led");
    driver_unregister(&tiled_driver);

    return 0;
}

void led_drv_exit(void)
{
#if 0
    if (tiled_class_dev)
        class_device_del (tiled_class_dev);
#endif
    if(tiled_dev_num > 0)
        /*class_device_destroy (tiled_class, tiled_dev_num);*/
        device_destroy (tiled_class, tiled_dev_num);

    if (tiled_class)
        class_destroy (tiled_class);
    if (tiled_device)
        platform_device_unregister(tiled_device);
    driver_remove_file(&tiled_driver, &driver_attr_version);

    unregister_chrdev(MAJOR(tiled_dev_num),"led");
    driver_unregister(&tiled_driver);

    remove_proc_entry("avalanche/led_cfg", NULL);
    remove_proc_entry("avalanche/led_ver", NULL);
    led_hal_exit();
}

module_init(led_drv_init);
module_exit(led_drv_exit);


EXPORT_SYMBOL(led_manager_register_module);
EXPORT_SYMBOL(led_manager_led_action);
EXPORT_SYMBOL(led_manager_cfg_mod);
EXPORT_SYMBOL(led_manager_unregister_module);
EXPORT_SYMBOL(led_manager_install_callbacks);
EXPORT_SYMBOL(led_manager_uninstall_callbacks);

