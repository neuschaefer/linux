/*
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

/*******************************************************************************   
 * FILE PURPOSE:    LCD Module Driver Source                                       
 *******************************************************************************   
 * FILE NAME:       lcd_drv.c                                                   
 *                                                                                 
 * DESCRIPTION:     Source code for Linux LCD Driver                             
 *                                                                                 
 * REVISION HISTORY:  
 *   
 * Date           Description                               Author
 *-----------------------------------------------------------------------------
 * 27 Aug 2003    Initial Creation                          Sharath Kumar  
 * 
 * 16 Dec 2003    Updates for 5.7                           Sharath Kumar                                                          
 *                                                                                 
 * (C) Copyright 2003, Texas Instruments, Inc                                      
 ******************************************************************************/                   
#include <linux/module.h>                
#include <linux/init.h>                  
#include <linux/types.h>                 
#include <linux/errno.h>                 
#include <linux/slab.h>                  
#include <linux/ioport.h>                
#include <linux/fcntl.h>                 
#include <linux/interrupt.h>             
#include <linux/sched.h>                 
#include <linux/proc_fs.h>
#include <linux/mm.h>                    
#include <linux/version.h>                    
#include <asm/io.h>                      
#include <asm/uaccess.h>                 
#include <asm/system.h>                  
#include <linux/delay.h>                 
#include <linux/devfs_fs_kernel.h>       
#include <linux/wait.h>
#include <pal.h> 
#include <lcd.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#include <linux/moduleparam.h>                
#include <linux/cdev.h>
#include <linux/device.h>
#endif

typedef void LIDD_HAL_OBJ_T;
#include "lidd_hal.h"

#define    TI_LCD_VERSION                 "0.2"
#define    MAX_LCD_SIZE                    (MAX_ROWS)*(MAX_COLS)

/* LCD seek origin positions*/
#define  SEEK_CUR    1
#define  SEEK_END    2
#define  SEEK_SET    0

/* TYPEDEF for LCD dev object */
typedef struct lcd_dev 
{                                            
    LIDD_HAL_OBJ_T *hal_handle;	/* handle for hal */             
    dev_t lcd_device_number; /* dynamicaly allocated LCD device number */
    char devname[10]; /* LCD device name */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    struct class_simple *tilcd_class; /* For registering with sysfs fs */
#else
    devfs_handle_t fs_handle; /* used for 2.4.x devfs support*/
#endif

    struct semaphore sem; /* mutual exclusion semaphore */      
    int rows;                                                   
    int columns;                                                
} LCD_DEV;                                                          

static LCD_DEV  *lcd_dev;
static int rows=DEFAULT_ROWS;
static int columns=DEFAULT_COLS;

/* LCD proc entry pointer */      
static struct proc_dir_entry *p_lcd_proc;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
static devfs_handle_t devfs_dir_handle = NULL;
#else
/* LCD sysfs entry pointer */      
static struct platform_device *tilcd_device = NULL;
#endif

/* Proc function to display driver version */                                                                       
static int lcd_read_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data)        
{                                                                                              
	int len=0;                                                                                 
	len += sprintf(buf+len,"\nTI Linux LCD Driver Version %s\n",TI_LCD_VERSION);
	return len;                                                                                
}                                                                                              
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
static ssize_t show_driver_version_lcd(struct device_driver * dev, char *buf)
{
    return sprintf (buf, "%s\n", TI_LCD_VERSION);
}

static DRIVER_ATTR(version, S_IRUGO, show_driver_version_lcd, NULL);

static int tilcd_probe_dev(struct device *dev)
{
    return 0;    
}

static struct device_driver tilcd_driver = {
	.name		= "tilcd",
	.bus		= NULL,
	.probe		= tilcd_probe_dev,
	.remove		= NULL,
	.suspend	= NULL,
	.resume		= NULL,
};
#endif

static ssize_t tilcd_read( struct file * file, char * buf,
                                                  size_t count, loff_t *ppos )
{
    LCD_DEV  *lcd_dev=file->private_data;
    int ret = -1, max = lcd_dev->columns * lcd_dev->rows;
    char temp_buf[MAX_LCD_SIZE];
    int x_pos, y_pos;
    
   
    if( *ppos >= max )
        return 0;

    memset( temp_buf, 0, max );
   
	/* Limit the count to max LCD size if it is greater than that */
    count = (count > max)? max:count;
 
    if(down_interruptible(&lcd_dev->sem)) 
    {
    	return -ERESTARTSYS;
    }
 
    /* syncing file offset and cursor position*/ 
    x_pos = ((long)*ppos) / lcd_dev->columns;
    y_pos = ((long)*ppos) % lcd_dev->columns;
    ti_lidd_hal_ioctl (lcd_dev->hal_handle, TI_LIDD_GOTO_XY, (y_pos << 8) |
            x_pos); 

    ret = ti_lidd_hal_read(lcd_dev->hal_handle,temp_buf,count);

    if(ret > 0)
    {
        if(copy_to_user(buf,temp_buf,ret))
	        ret = -EFAULT;
    }

    if (ret >= 0)
    {
        *ppos += ret;

        /* syncing file offset and cursor position*/ 
        x_pos = ((long)*ppos) / lcd_dev->columns;
        y_pos = ((long)*ppos) % lcd_dev->columns;
        ti_lidd_hal_ioctl (lcd_dev->hal_handle, TI_LIDD_GOTO_XY, (y_pos << 8) |
            x_pos); 
    }

    up(&lcd_dev->sem);
    return ret;
}

static ssize_t tilcd_write( struct file * file, const char * buf,
                                                    size_t count, loff_t *ppos )
{
    LCD_DEV  *lcd_dev=file->private_data;
    int ret = -1, max = lcd_dev->columns * lcd_dev->rows;
    char temp_buf[MAX_LCD_SIZE];
    int x_pos, y_pos;

    if (down_interruptible(&lcd_dev->sem)) 
    {
        return -ERESTARTSYS;
    }
        
    /* Limit the count to max LCD size if it is greater than that */
    count = (count > max)? max:count;

    /* syncing file offset and cursor position*/ 
    x_pos = ((long)*ppos) / lcd_dev->columns;
    y_pos = ((long)*ppos) % lcd_dev->columns;
    ti_lidd_hal_ioctl (lcd_dev->hal_handle, TI_LIDD_GOTO_XY, (y_pos << 8) |
            x_pos); 

    if(copy_from_user(temp_buf,buf,count))
        ret = -EFAULT;
    else
    {    
        ret = ti_lidd_hal_write(lcd_dev->hal_handle, temp_buf, count);
    }

    if (ret >= 0)
    {
        *ppos += ret;
   
        /* syncing file offset and cursor position*/ 
        x_pos = (((long)*ppos) / lcd_dev->columns);
        y_pos = ((long)*ppos) % lcd_dev->columns;
        ti_lidd_hal_ioctl (lcd_dev->hal_handle, TI_LIDD_GOTO_XY, (y_pos << 8) |
                x_pos); 
    }

    up(&lcd_dev->sem);
    return ret;
}

static int tilcd_ioctl( struct inode * inode, struct file * file,
				             unsigned int cmd, unsigned long arg )
{
    LCD_DEV  *lcd_dev=file->private_data;
    int ret = 0;
    LCD_POS lcd_pos;
    LCD_PULSE_ARG pulse_arg;
    unsigned long hal_arg = arg;
    unsigned int read_val;

    if (down_interruptible(&lcd_dev->sem)) 
    {
        return -ERESTARTSYS;
    }

    if(cmd == TI_LIDD_GOTO_XY)
    {
        if(copy_from_user(&lcd_pos,(char *)arg,sizeof(LCD_POS)))
            ret = -EFAULT;
        /* syncing file offset and cursor position*/ 
        file->f_pos = lcd_pos.row * lcd_dev->columns + lcd_pos.column; 
        hal_arg = lcd_pos.row | (lcd_pos.column << 8);
    }     

    if(cmd == TI_LIDD_PULSE_CMD)
    {
        if(copy_from_user(&pulse_arg, (char *)arg, sizeof(LCD_PULSE_ARG)))
            ret = -EFAULT;

        hal_arg = pulse_arg.cnt | (pulse_arg.is_up << 8);

        PAL_sysProbeAndPrep(AVALANCHE_LCD_HW_MODULE_REV, AVALANCHE_LCD_CONTROL_BASE, &hal_arg);
    }     

    /* The commands below require something to be written to user buffer */
    if(cmd == TI_LIDD_RD_CMD || cmd == TI_LIDD_RD_CHAR)
    {
        hal_arg = (unsigned long )&read_val;
    }

    ret = ti_lidd_hal_ioctl(lcd_dev->hal_handle,cmd,hal_arg);

    switch(cmd)
    {
        case TI_LIDD_RD_CMD:
        case TI_LIDD_RD_CHAR:
            if(copy_to_user((char *)arg,&read_val,sizeof(int)))
                ret = -EFAULT;
            break;

        default:
            break;
    }
        
    up(&lcd_dev->sem);
    return ret;
}

static int tilcd_open( struct inode * inode, struct file * file )
{
   /* Set the private data. cannot be done in init through cdev*/ 
    file->private_data = lcd_dev;

    ti_lidd_hal_open(lcd_dev->hal_handle);
  
    return 0;
}

static int tilcd_release( struct inode * inode, struct file * file )
{
    ti_lidd_hal_close(lcd_dev->hal_handle);
    return 0;
}

static loff_t tilcd_lseek(struct file *file, loff_t offset, int orig)
{

    LCD_DEV  *lcd_dev=file->private_data;
    int max = lcd_dev->columns * lcd_dev->rows;
    int x_pos, y_pos;

	if (down_interruptible(&lcd_dev->sem)) 
    {
        return -ERESTARTSYS;
    }

	switch (orig) {
		case SEEK_END:
			offset += max;
			break;
			
		case SEEK_CUR:
			offset += file->f_pos;
		case SEEK_SET:
			break;

		default:
            up(&lcd_dev->sem);
			return -EINVAL;
	}

    (long)offset %= max;
    
	file->f_pos = offset;

    x_pos = (((long)offset) / lcd_dev->columns);
    y_pos = ((long)offset) % lcd_dev->columns;
    ti_lidd_hal_ioctl (lcd_dev->hal_handle, TI_LIDD_GOTO_XY, (y_pos << 8) |
            x_pos); 

    
    up(&lcd_dev->sem);
	return file->f_pos;
}

struct file_operations tilcd_fops = {
	owner:  THIS_MODULE,
	read:   tilcd_read,
	write:  tilcd_write,
	ioctl:	tilcd_ioctl,
	open:	tilcd_open,
	release:  tilcd_release,
	llseek: tilcd_lseek              
};

static int __init tilcd_init(void)
{
    TI_LIDD_INFO_T lcd_info;
    int error_num;
    int lcd_major_num;

   if((error_num = PAL_sysProbeAndPrep(AVALANCHE_LCD_HW_MODULE_REV, AVALANCHE_LCD_CONTROL_BASE, NULL)) < 0) {
        printk("LCD: PAL_sysProbeAndPrep failed\n");
        return -EINVAL;
    }
	
    /* take module out of reset */
    PAL_sysResetCtrl(AVALANCHE_LCD_CTRL_RESET_BIT, OUT_OF_RESET);
  
    /* Set LCD clock to 50 MHz */
    PAL_sysClkcSetFreq(CLKC_LCD,CLK_MHZ(50));

  if(PAL_osMemAlloc(0, sizeof(LCD_DEV), 0, ( void *)&lcd_dev) != PAL_SOK) 
  {
    printk( "LCD: Memory allocation failed\n" );
    return -EIO;
  }

    PAL_osMemSet(lcd_dev,0,sizeof(LCD_DEV));

    lcd_info.base_addr = AVALANCHE_LCD_CONTROL_BASE;
    lcd_info.vbus_freq = PAL_sysClkcGetFreq( CLKC_VBUS );
    lcd_info.cpu_freq = PAL_sysClkcGetFreq( CLKC_MIPS );
    lcd_dev->rows = lcd_info.disp_row=rows;
    lcd_dev->columns = lcd_info.disp_col=columns;
    lcd_info.line_wrap = 1;
    lcd_info.cursor_blink = 1;
    lcd_info.cursor_show = 1;
    lcd_info.lcd_type = 4; /* HITACHI */
    lcd_info.num_lcd = NO_LCD_DEVICES;
    lcd_dev->hal_handle = ti_lidd_hal_init(&lcd_info);

    if(!lcd_dev->hal_handle)
    {
        printk("LCD: hal not initialized\n");
        return -EIO;
    }
	
    sprintf(lcd_dev->devname, "lcd");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    tilcd_device = ti_platform_device_register_simple(lcd_dev->devname, -1, NULL, 0);
   
    if (IS_ERR(tilcd_device)) {
    	error_num = PTR_ERR(tilcd_device);
        printk ("platform device register failed in LCD\n");
        goto exit_init;        
    }
       
    tilcd_driver.bus = platform_bus_type_ptr;
    error_num = ti_driver_register(&tilcd_driver); 

    if (error_num < 0)
    {
        printk ("driver register to sysfs failed for TI LCD\n");
        goto exit_init;
    }
    ti_driver_create_file(&tilcd_driver, &driver_attr_version);
#endif 

    lcd_major_num = register_chrdev (0, lcd_dev->devname, &tilcd_fops);
    if (lcd_major_num < 0)
    {
        printk ("could not create device for ti LCD\n");
        goto exit_init;
    }
    
    lcd_dev->lcd_device_number =  MKDEV(lcd_major_num, 0);
 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_mk_cdev(lcd_dev->lcd_device_number, S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP |
            S_IWGRP, lcd_dev->devname);
#else

    devfs_dir_handle = devfs_mk_dir(NULL, "lcd", NULL);
    sprintf(lcd_dev->devname, "0");
    
    lcd_dev->fs_handle = devfs_register(devfs_dir_handle, lcd_dev->devname,DEVFS_FL_AUTO_DEVNUM, 0, 0, 
						S_IFCHR | S_IRUGO | S_IWUGO,&tilcd_fops,lcd_dev);        
#endif 
        
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    lcd_dev->tilcd_class = class_simple_create (THIS_MODULE, lcd_dev->devname);

    if (lcd_dev->tilcd_class == NULL)
    {
        printk ("could not create class for TI LCD\n");
        goto exit_init;
    }

    if (class_simple_device_add (lcd_dev->tilcd_class, lcd_dev->lcd_device_number,
            &tilcd_device->dev, "%s%d", lcd_dev->devname, 0) == NULL)
    {
        printk ("class simplae device add failed for TI LCD\n");
        goto exit_init;
    }
#endif 
    sema_init(&lcd_dev->sem,1);

    /* Creating proc entry for the devices */                                                
    p_lcd_proc = create_proc_read_entry("avalanche/lcd_ver", 0, NULL, lcd_read_proc, NULL);

    return 0;
exit_init:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (lcd_dev->lcd_device_number > 0)
        class_simple_device_remove (lcd_dev->lcd_device_number);

    if (lcd_dev->tilcd_class)
        class_simple_destroy (lcd_dev->tilcd_class);
#endif

    if (lcd_dev->lcd_device_number > 0)
        unregister_chrdev (lcd_dev->lcd_device_number, lcd_dev->devname);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_remove("%s%d", lcd_dev->devname, 0);
#else
    if (lcd_dev->fs_handle)
        devfs_unregister(lcd_dev->fs_handle);

    if (devfs_dir_handle)
        devfs_unregister(devfs_dir_handle);
#endif
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (tilcd_device)
        ti_platform_device_unregister(tilcd_device);

    ti_driver_remove_file(&tilcd_driver, &driver_attr_version);
    ti_driver_unregister(&tilcd_driver);
#endif
    if (lcd_dev->hal_handle) 
        ti_lidd_hal_cleanup(lcd_dev->hal_handle);

    if (lcd_dev)
        kfree(lcd_dev);

    remove_proc_entry("avalanche/lcd_ver",NULL);
    return -ENOMEM;
}

static void __exit tilcd_exit(void)
{

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (lcd_dev->lcd_device_number > 0)
        class_simple_device_remove (lcd_dev->lcd_device_number);

    if (lcd_dev->tilcd_class)
        class_simple_destroy (lcd_dev->tilcd_class);
#endif

    if (lcd_dev->lcd_device_number > 0)
        unregister_chrdev (lcd_dev->lcd_device_number, lcd_dev->devname);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    devfs_remove("%s%d", lcd_dev->devname, 0);
#else
    if (lcd_dev->fs_handle)
        devfs_unregister(lcd_dev->fs_handle);

    if (devfs_dir_handle)
        devfs_unregister(devfs_dir_handle);
#endif
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    if (tilcd_device)
        ti_platform_device_unregister(tilcd_device);
    ti_driver_remove_file(&tilcd_driver, &driver_attr_version);
    ti_driver_unregister(&tilcd_driver);
#endif
    
    if (lcd_dev->hal_handle) 
        ti_lidd_hal_cleanup(lcd_dev->hal_handle);

    if (lcd_dev)
        kfree(lcd_dev);

    remove_proc_entry("avalanche/lcd_ver",NULL);
}

MODULE_PARM(rows,"i");
MODULE_PARM(columns,"i");

MODULE_LICENSE("Texas Instruments");
MODULE_DESCRIPTION("Driver for TI CHARACTER LCD");
MODULE_AUTHOR("Maintainer: Sharath Kumar <krs@ti.com>");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
MODULE_VERSION(TI_LCD_VERSION);
#endif

module_init(tilcd_init);
module_exit(tilcd_exit);

