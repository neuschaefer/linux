/*
 *
 * File Name: time_capture_drv.c
 * Description:
 * Used to measure the boot time of certain events in the system
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/proc_fs_macros.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/sys.h>
#include <linux/time.h>

#include "time_capture_drv.h"
#include "_tistdtypes.h"

#define DEV_NAME  "time_capture"

#define IS_STAMP_TAKEN(eventId)           ( (ctimes[eventId].tv_sec != 0) || (ctimes[eventId].tv_nsec != 0) )
#define SET_TIME_STAMP(eventId)           get_monotonic_boottime(&ctimes[eventId])

/* used by IOCTL */
static struct cdev *                time_capture_cdev;
static dev_t                        time_capture_dev_t;
static struct class *               time_capture_udev_class;
static unsigned int                 count = 1;

/* Array used to hold capturing times for each event */
static struct timespec ctimes[TIME_CAPTURE_EVENTS_COUNT];

// Proc Dump
int time_capture_dump(struct seq_file *m, void *v) 
{
    int id;

    seq_printf(m, "\n-----------------------------------------------------------\n\n");
    seq_printf(m, "                   Boot time information\n");
    seq_printf(m, "\n-----------------------------------------------------------\n\n");
    seq_printf(m, " * Time measurement starts with kernel initialization,\n   Bootram & UBoot boot times are not measured.\n");
    seq_printf(m, "\n-----------------------------------------------------------\n\n");
    seq_printf(m, "%-35s%s\n\n", " Name", " Value");
    for (id = 0; id < TIME_CAPTURE_EVENTS_COUNT; id++)
    {
        if ( IS_STAMP_TAKEN(id) ) 
        {
            seq_printf(m, " %-35s%d.%d %s\n", eventsToString[id], (int)(ctimes[id].tv_sec), (int)(ctimes[id].tv_nsec/10000000), "sec");
        } 
        else 
        {
            seq_printf(m, " %-35s%s\n", eventsToString[id], "N/A");
        }
    }
    seq_printf(m, "\n-----------------------------------------------------------\n\n");
    return 0;
}

DECLARE_PROCFS_READ_ENTRY(time_capture, time_capture_dump)

/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           IOCTL Implementation                                       */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/**************************************************************************/
/*! \fn static int time_capture_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function handles device requests.
 *  \param struct file *filp - the device file pointer
 *  \param unsigned int cmd - the command to be performed
 *  \param unsigned long arg - pointer to the user request
 *  \return int - 0 on success else negative number.
 **************************************************************************/
static int time_capture_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;

    switch ( cmd ) 
    {
        case TIME_CAPTURE_EVENT_CMD:
        {
            ret = TC_setEventTime( (TIME_CAPTURE_EVENT_e) arg );
            break;
        }
        default:
        {
            printk(KERN_ERR "%s:%d Invalid IOCTL(0x%08X) has been received \n", __FUNCTION__, __LINE__, cmd);
            ret = -1;
            break;
        }
    }
    return ret;
}

/**************************************************************************/
/*! \fn int TC_setEventTime(TIME_CAPTURE_EVENT_e event)
 **************************************************************************
 *  \brief  take a time stamp measure and add it to TIME CAPTURE times db
 *
 *  \param[in] event - enum of time event in the system
 *  \return 0 if ok -1 if error
 **************************************************************************/
int TC_setEventTime(TIME_CAPTURE_EVENT_e event)
{
    /* Check if event is valid */
    if ( (event < 0) || (event >= TIME_CAPTURE_EVENTS_COUNT) )
    {
        return -1;
    }

    if ( !IS_STAMP_TAKEN(event) )
    {
        SET_TIME_STAMP(event);
    }

    return 0;
}

/* Structure to map driver functions to kernel */
struct file_operations time_capture_drv_fops =
{
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = time_capture_ioctl,
        .open    = NULL,
        .release = NULL,
};

static int __init time_capture_init(void) 
{
    /* Initialize capturing times db */
    memset(ctimes, 0, sizeof(ctimes));

    printk(KERN_DEBUG "TIME-CAPTURE: Initializing Intel(R) TIME-CAPTURE driver\n");

    if (alloc_chrdev_region(&time_capture_dev_t, 0, count, DEV_NAME) < 0)
    { 
        /* count indicates how many minors we get */
        printk(KERN_ERR "%s:%d Failed to register character device region %s\n", __FUNCTION__, __LINE__, DEV_NAME);
        return (-1);
    }

    if (!(time_capture_cdev = cdev_alloc()))
    {
        printk(KERN_ERR "%s:%d Failed to allocate character device %s\n", __FUNCTION__, __LINE__, DEV_NAME);
        unregister_chrdev_region(time_capture_dev_t, count);
        return (-1);
    }

    /* Connect the file operations with the cdev */
    cdev_init(time_capture_cdev, &time_capture_drv_fops);

    /* Connect the major/minor number to the cdev  - Activates the device */
    if (cdev_add(time_capture_cdev, time_capture_dev_t, count) < 0)
    {
        printk(KERN_ERR "%s:%d Failed to add character device %s\n", __FUNCTION__, __LINE__, DEV_NAME);
        cdev_del(time_capture_cdev);
        unregister_chrdev_region(time_capture_dev_t, count);
        return (-1);
    }

    /* connection to the udev */
    /* creates a class directory under /sys/class */
    time_capture_udev_class = class_create(THIS_MODULE, DEV_NAME);

    /* creates a class directory under /sys/class/DEV_NAME named DEV_NAME */
    /* creates 3 files: dev, uevent, subsystem */
    device_create(time_capture_udev_class, NULL, time_capture_dev_t, NULL, "%s", DEV_NAME);

    printk(KERN_DEBUG "TIME_CAPTURE: Create device: /dev/%s\n", DEV_NAME);
    
    /* Proc filesystem utilities.... */
    if (NULL == (proc_create( "time_capture" , 0, NULL, &time_capture_proc_fops )))
    {
        printk(KERN_ERR "%s:%d ERROR ....\n", __FUNCTION__, __LINE__);
        return -1;
    }        
    printk(KERN_DEBUG "TIME_CAPTURE: Create proc file: /proc/%s\n", "time_capture");

    return 0;
}

static void __exit time_capture_exit(void)
{
    printk(KERN_INFO "Exit Time Capture\n");
}

module_init(time_capture_init);
module_exit(time_capture_exit);

/* Driver identification */
MODULE_DESCRIPTION("Time Capture Device Driver");
MODULE_LICENSE("GPL");
