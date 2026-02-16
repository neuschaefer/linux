/*
 * keypad_drv.c
 * Description:
 * Keypad Module Driver Source
 *
 *  Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
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
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

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
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/spinlock.h> 
#include <pformCfg.h>
#include <avalanche_intc.h>


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#include <linux/moduleparam.h>
#include <linux/device.h> 
#include <linux/cdev.h>
#endif

/* Need to define this type since this before including
 * keypad_hal.h
 */
typedef void KPAD_HAL_OBJ_T;


#include <keypad.h>
#include "keypad_hal.h"

#define TI_KEYPAD_VERSION                 "0.4"
int thread_exit=0;
unsigned int avalanche_get_vbus_freq(void);


/*********************TYPEDEFs*****************************************/


/* Type definition of driver object
 */
typedef struct kpad_dev {                                                    
	KEY_EVENT  *buffer;    /* Circular buffer to log key events */                                                      
	wait_queue_head_t inq; /* Queue for processes waiting for key event */                                               
	void *kpad_handle;     /* handle for keypad hal */                               
	struct fasync_struct *async_queue; /* handle for async notification */ 
    dev_t tikpad_dev_num; /* the dynamically allocated devive number*/
    char devname[10];     /* the keypad device name */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    struct class_simple * tikpad_class; /* the class_simple structure for
                                           adding to sysfs*/
#else
    devfs_handle_t fs_handle; /* used for 2.4.x devfs support*/
#endif
	spinlock_t  lock;      /* mutex lock */               
	int write_index;       /* write pointer to circular buffer */                                                     
	int last_key_pressed;  /* Used in identifying the key during release event */ 
	int usage_count;       /* Indicates the module usage count */                                                  
}KPAD_DEV;                                                                  


/*****************STATIC Declarations *********************/

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
static irqreturn_t kpad_driver_isr(int isr,void *data, struct pt_regs *reg);
#else
static void  kpad_driver_isr(int isr,void *data, struct pt_regs *reg);
#endif

static KPAD_DEV  *keypad_dev = NULL;

static int row_map=ROW_MAP;
static int column_map=COLUMN_MAP;
static int debounce_time=DEBOUNCE_TIME;

/* KEYPAD proc entry pointer */      
static struct proc_dir_entry *p_kpad_proc;
                                    
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
static devfs_handle_t devfs_dir_handle = NULL;
#else
/* KEYPAD sysfs entry pointer */      
static struct platform_device *tikpad_device = NULL;
#endif

/* Proc function to display driver version */                                                                       
static int kpad_read_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{                                                                                              
	int len=0;                                                                                 
                                                                                               
	len += sprintf(buf +len,"\nTI Linux Keypad Driver Version %s\n",TI_KEYPAD_VERSION);         
	return len;                                                                                
}                                                                                              
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
/* sysfs version support */
static ssize_t show_driver_version_kpad(struct device_driver * dev, char *buf)
{
    return sprintf (buf ,"%s\n" ,TI_KEYPAD_VERSION);
}

static DRIVER_ATTR(version, S_IRUGO, show_driver_version_kpad, NULL);

/* dummy function. Necessary while initializing the device_driver structure
 * for sysfs*/
static int tikpad_probe_dev(struct device *dev)
{
    return 0;    
}

static struct device_driver tikpad_driver = {
	.name		= "tikpad",
	.bus		= NULL,
	.probe		= tikpad_probe_dev,
	.remove		= NULL,
	.suspend	= NULL,
	.resume		= NULL,
};

#endif

static ssize_t tikpad_read( struct file * file, char * buf,
                                                  size_t count, loff_t *ppos )
{
	int read_index;
	int write_index;
	int max_events;
	int req_events;
	KPAD_DEV  *kpad_dev=file->private_data;

	read_index = *ppos; 
	req_events = count / sizeof(KEY_EVENT); 

	if(read_index == kpad_dev->write_index)
	{ 
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;

		/* buffer is empty, so wait */
		if (wait_event_interruptible(kpad_dev->inq, (read_index != kpad_dev->write_index)))
			return -ERESTARTSYS;
	}

	write_index = kpad_dev->write_index;
	if(write_index > read_index)
	{
		/* There is no wrap around */
		max_events = write_index - read_index;
		req_events = (req_events > max_events)? max_events: req_events;
       
		if (copy_to_user(buf, (char *)&kpad_dev->buffer[read_index], req_events * sizeof(KEY_EVENT))) 
			return -EFAULT;
	}
	else
	{
		int num_events = 0;

		/* There is wrap around. We may have to 
		 * perform two separate copies
		 */
		max_events = (BUFFER_SIZE - read_index) + write_index;
		req_events = (req_events > max_events)? max_events: req_events;
			
		num_events = ((BUFFER_SIZE - read_index) > req_events)?req_events:(BUFFER_SIZE - read_index); 
		if (copy_to_user(buf, (char *)&kpad_dev->buffer[read_index], num_events * sizeof(KEY_EVENT))) 
			return  -EFAULT;
        
		if(req_events > num_events)
		{
			/* Copy from start of the buffer */
			if (copy_to_user(buf, (char *)&kpad_dev->buffer[0], (req_events - num_events) * sizeof(KEY_EVENT))) 
				return  -EFAULT;
		}    
	}

	read_index = (read_index + req_events) % BUFFER_SIZE;
	*ppos = read_index;

	return (req_events * sizeof(KEY_EVENT));
}


static int tikpad_ioctl( struct inode * inode, struct file * file,
				             unsigned int cmd, unsigned long arg )
{
	KPAD_DEV  *kpad_dev=file->private_data;
	int ret;
	int flags;

	spin_lock_irqsave(&kpad_dev->lock, flags); 

	if (cmd == TI_KEY_DEBOUNCE_VALUE)
		ret=  ti_kpad_hal_ioctl(kpad_dev->kpad_handle,cmd,arg);

	else
		ret = -EINVAL;

	spin_unlock_irqrestore(&kpad_dev->lock, flags);
	return ret;

}


static int tikpad_open( struct inode * inode, struct file * file )
{
	KPAD_DEV  *kpad_dev;
	int flags;

	file->private_data = keypad_dev;
	kpad_dev=file->private_data;

	spin_lock_irqsave(&kpad_dev->lock, flags); 
	if(!kpad_dev->usage_count)
	{
		ti_kpad_hal_start(kpad_dev->kpad_handle);
	
	}
	
	kpad_dev->usage_count++;
	file->f_pos = kpad_dev->write_index;

	spin_unlock_irqrestore(&kpad_dev->lock, flags);

	/* code for registering irq */
	request_irq(LNXINTNUM(AVALANCHE_KPAD_CNTL_INT),kpad_driver_isr,0,"keypad",kpad_dev);
        
	return 0;
}

static int tikpad_release( struct inode * inode, struct file * file )
{
	KPAD_DEV  *kpad_dev=file->private_data;
	int flags;

	spin_lock_irqsave(&kpad_dev->lock, flags); 
	keypad_dev->usage_count--;
	if(!keypad_dev->usage_count)
	{
		kpad_dev->write_index = 0;
		ti_kpad_hal_stop(kpad_dev->kpad_handle);
		free_irq(LNXINTNUM(AVALANCHE_KPAD_CNTL_INT),kpad_dev);
	}
	
	spin_unlock_irqrestore(&kpad_dev->lock, flags);
	return 0;
}

int tikpad_fasync(int fd, struct file *file, int mode)
{
	KPAD_DEV *p_kpad_dev=file->private_data;
	return fasync_helper(fd, file, mode, &p_kpad_dev->async_queue);
}

static int tikpad_flush(struct file *file)
{
	/* remove the current process from async queue */
	/*tikpad_fasync(-1, file, 0);*/
	return 0;
}

struct file_operations tikpad_fops = {
	owner:  THIS_MODULE,
	read:   tikpad_read,
	ioctl:	tikpad_ioctl,
	fasync: tikpad_fasync,
	flush:  tikpad_flush,
	open:	tikpad_open,
	release:  tikpad_release
};

static inline void notify_event(KPAD_DEV  *kpad_dev)
{
	wake_up_interruptible(&kpad_dev->inq);

	/* Now there is some data to read
	 * Send SIGIO signal to waiting processes */
	if (kpad_dev->async_queue)
		kill_fasync(&kpad_dev->async_queue, SIGIO, POLL_IN);

}


static inline void kpad_add_event(KPAD_DEV *kpad_dev,int key_pressed, int key_press_time)
{	int flags;

	spin_lock_irqsave(&kpad_dev->lock, flags); 
	
	kpad_dev->buffer[kpad_dev->write_index].row = key_pressed & 0xff;
	kpad_dev->buffer[kpad_dev->write_index].column = (key_pressed & 0xff00) >> 8;
	kpad_dev->buffer[kpad_dev->write_index].key_press_time = key_press_time;
	kpad_dev->write_index = (kpad_dev->write_index + 1) % BUFFER_SIZE;
	notify_event(kpad_dev);

	spin_unlock_irqrestore(&kpad_dev->lock, flags);
	
}

//#if defined(CONFIG_KEYPAD_TASKLET_MODE)
inline void keypad_bh(unsigned long data_ptr)
{
	KPAD_DEV  *kpad_dev = (*((KPAD_DEV **)data_ptr));
	int key_pressed;

	key_pressed=ti_kpad_hal_key_scan(kpad_dev->kpad_handle);
	kpad_dev->last_key_pressed = key_pressed;       
	if(key_pressed >= 0){
		kpad_add_event(kpad_dev,key_pressed,0);
	}

	/* enable interrupt */ 
	ti_kpad_hal_ioctl(kpad_dev->kpad_handle,TI_KEY_RELEASE_DETECT,0);
}

#if defined(CONFIG_KEYPAD_TASKLET_MODE)
DECLARE_TASKLET(keypad_tasklet, keypad_bh, (unsigned long)&keypad_dev);

DECLARE_WAIT_QUEUE_HEAD(keypad_press_queue);
DECLARE_WAIT_QUEUE_HEAD(keypad_release_queue);
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
static irqreturn_t kpad_driver_isr(int isr,void *data, struct pt_regs *reg)
#else
static void  kpad_driver_isr(int isr,void *data, struct pt_regs *reg)
#endif
{
    KPAD_DEV  *kpad_dev=data;
    int ret;
    static unsigned int prev_jiffies;

    ret=ti_kpad_hal_isr(kpad_dev->kpad_handle);
 
    switch(ret)
    {
    	case 1: /* key press event */

        prev_jiffies = jiffies;
#if defined(CONFIG_KEYPAD_TASKLET_MODE)
    		/* call bottom half */
        tasklet_schedule(&keypad_tasklet);
#else
        keypad_bh ((unsigned long) &keypad_dev);
#if 0
        key_pressed=ti_kpad_hal_key_scan(kpad_dev->kpad_handle);
        kpad_dev->last_key_pressed = key_pressed;       
        if(key_pressed >= 0){
	    kpad_add_event(kpad_dev,key_pressed,0);
        }

        /* enable interrupt */ 
        ti_kpad_hal_ioctl(kpad_dev->kpad_handle,TI_KEY_RELEASE_DETECT,0);
#endif

#endif         
        break;

        case 2: /* key release event */
        {
            int key_pressed = kpad_dev->last_key_pressed;
            int key_press_time = (jiffies - prev_jiffies) * (1000/HZ); // in milisecs
            if ( kpad_dev->last_key_pressed >= 0)
            {
        	kpad_add_event(kpad_dev,key_pressed,key_press_time);
            }
			
        }

        /* enable interrupt */ 
        ti_kpad_hal_ioctl(kpad_dev->kpad_handle,TI_KEY_PRESS_DETECT,0); 
        break;
                      
        default:

        /* enable interrupt */ 
        ti_kpad_hal_ioctl(kpad_dev->kpad_handle,TI_KEY_PRESS_DETECT,0); 
        }
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
        return IRQ_HANDLED;
#endif
}
int __init
tikpad_init(void)
{
    KEYPAD_DEV_INFO_T kpad_info;
    int error_num;    
    int tikpad_major_num;
    
    if ((error_num = PAL_sysProbeAndPrep(AVALANCHE_KEYPAD_HW_MODULE_REV, AVALANCHE_KEYPAD_CONTROL_BASE, NULL)) < 0) {
        printk("KEYPAD: PAL_sysProbeAndPrep failed\n");
        return -EINVAL;
    }
	
    keypad_dev = kmalloc(sizeof(KPAD_DEV), GFP_KERNEL);
    keypad_dev->buffer=kmalloc(BUFFER_SIZE * sizeof(KEY_EVENT) , GFP_KERNEL);
    keypad_dev->write_index = 0;
    keypad_dev->usage_count = 0;
    kpad_info.row_map = row_map;
    kpad_info.column_map = column_map;
    kpad_info.debounce_time = debounce_time;
    kpad_info.base_address = AVALANCHE_KEYPAD_CONTROL_BASE;
    kpad_info.module_freq = avalanche_get_vbus_freq();
   
    keypad_dev->kpad_handle = ti_kpad_hal_init(&kpad_info);
    if(!keypad_dev->kpad_handle)
    { 
    	printk("Error: hal not initialized\n");
    	return -EIO;
    }

    sprintf(keypad_dev->devname, "keypad");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    tikpad_device = ti_platform_device_register_simple(keypad_dev->devname, -1, NULL, 0);

    if (IS_ERR(tikpad_device)) {
        printk ("platform device register failed from TI keypad\n");
    	error_num = PTR_ERR(tikpad_device);
        goto exit_init;        
    }
    
    tikpad_driver.bus = platform_bus_type_ptr;
    error_num = ti_driver_register(&tikpad_driver); 

    if (error_num < 0)
    {
        printk ("driver register to sysfs failed for TI Keypad\n");
        goto exit_init;
    }
    ti_driver_create_file(&tikpad_driver, &driver_attr_version);
#endif

    tikpad_major_num = register_chrdev (0, keypad_dev->devname, &tikpad_fops);
    if (tikpad_major_num < 0)
    {
        printk ("could not create device for ti keypad\n");
        goto exit_init;
    }
    
    keypad_dev->tikpad_dev_num =  MKDEV(tikpad_major_num, 0);
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_mk_cdev(keypad_dev->tikpad_dev_num, S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP |
            S_IWGRP, keypad_dev->devname);
#else
    devfs_dir_handle = devfs_mk_dir(NULL, "keypad", NULL);
    sprintf(keypad_dev->devname, "0");
    keypad_dev->fs_handle=devfs_register(devfs_dir_handle, keypad_dev->devname,
            DEVFS_FL_AUTO_DEVNUM, 0, 0, S_IFCHR | S_IRUGO | S_IWUGO,
                                        &tikpad_fops,keypad_dev);
#endif
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    keypad_dev->tikpad_class = class_simple_create (THIS_MODULE, keypad_dev->devname);


    if (keypad_dev->tikpad_class == NULL)
    {
        printk ("could not create class for TI Keypad\n");
        goto exit_init;
    }

   
    if (class_simple_device_add (keypad_dev->tikpad_class, keypad_dev->tikpad_dev_num,
            &tikpad_device->dev, "%s%d", keypad_dev->devname, 0) == NULL)
    {
        printk ("class simplae device add failed for TI Keypad\n");
        goto exit_init;
    }
#endif 
    
    keypad_dev->lock = SPIN_LOCK_UNLOCKED;
    init_waitqueue_head(&keypad_dev->inq);
    keypad_dev->async_queue = NULL;
	
    /* Creating proc entry for the devices */                                                
    p_kpad_proc = create_proc_read_entry("avalanche/keypad_ver", 0, NULL, kpad_read_proc, NULL);
    return 0;

exit_init:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (keypad_dev->tikpad_dev_num > 0)
        class_simple_device_remove (keypad_dev->tikpad_dev_num);
    if (keypad_dev->tikpad_class)
        class_simple_destroy (keypad_dev->tikpad_class);
    if (tikpad_device)
        ti_platform_device_unregister(tikpad_device);
    ti_driver_remove_file(&tikpad_driver, &driver_attr_version);
#endif
    if (keypad_dev->tikpad_dev_num > 0)
        unregister_chrdev (keypad_dev->tikpad_dev_num, keypad_dev->devname);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_remove("%s%d", keypad_dev->devname, 0);
#else
    if (keypad_dev->fs_handle)
        devfs_unregister(keypad_dev->fs_handle);
    if (devfs_dir_handle)
        devfs_unregister(devfs_dir_handle);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    ti_driver_unregister(&tikpad_driver);
#endif
    if (keypad_dev->kpad_handle)
        ti_kpad_hal_cleanup(keypad_dev->kpad_handle);
    if (keypad_dev->buffer)
        kfree(keypad_dev->buffer);
    if (keypad_dev)
        kfree(keypad_dev);
    remove_proc_entry("avalanche/keypad_ver", NULL);
    return -ENOMEM;
}

void __exit tikpad_exit(void)
{

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (keypad_dev->tikpad_dev_num > 0)
        class_simple_device_remove (keypad_dev->tikpad_dev_num);
    if (keypad_dev->tikpad_class)
        class_simple_destroy (keypad_dev->tikpad_class);
    if (tikpad_device)
        ti_platform_device_unregister(tikpad_device);
    ti_driver_remove_file(&tikpad_driver, &driver_attr_version);
#endif
    if (keypad_dev->tikpad_dev_num > 0)
        unregister_chrdev (keypad_dev->tikpad_dev_num, keypad_dev->devname);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_remove("%s%d", keypad_dev->devname, 0);
#else
    if (keypad_dev->fs_handle)
        devfs_unregister(keypad_dev->fs_handle);
    if (devfs_dir_handle)
        devfs_unregister(devfs_dir_handle);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    ti_driver_unregister(&tikpad_driver);
#endif
    if (keypad_dev->kpad_handle)
        ti_kpad_hal_cleanup(keypad_dev->kpad_handle);
    if (keypad_dev->buffer)
        kfree(keypad_dev->buffer);
    if (keypad_dev)
        kfree(keypad_dev);
    thread_exit=1;
    remove_proc_entry("avalanche/keypad_ver", NULL);
}

MODULE_PARM(row_map,"i");
MODULE_PARM(column_map,"i");
MODULE_PARM(debounce_time,"i");

MODULE_DESCRIPTION("Driver for TI KEYPAD");
MODULE_AUTHOR("Maintainer: Sharath Kumar <krs@ti.com>");
MODULE_LICENSE("Texas Instruments");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
MODULE_VERSION(TI_KEYPAD_VERSION);
#endif

module_init(tikpad_init);
module_exit(tikpad_exit);
