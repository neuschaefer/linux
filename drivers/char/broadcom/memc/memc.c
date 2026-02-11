/*****************************************************************************
*  Copyright 2011 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <linux/version.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/vmalloc.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

/* ---- Private Constants and Types -------------------------------------- */

#define MEMC_DEVICE_NAME       "memc"
#define MEMC_BUFFER_MAX_SIZE   256

/**
* Debug traces
*/
#define MEMC_ENABLE_KNLLOG        0
#if MEMC_ENABLE_KNLLOG
#include <linux/broadcom/knllog.h>
#define MEMC_KNLLOG               KNLLOG
#else
#define MEMC_KNLLOG(...)
#endif

/* ---- Private Function Prototypes -------------------------------------- */

static int     memc_probe( struct platform_device *pdev );
static int     memc_remove( struct platform_device *pdev );

#ifdef CONFIG_SYSFS
static int     memc_proc_read( char *page, char **start, off_t off, int count, int *eof, void *data );
static int     memc_proc_write( struct file *file, const char __user *buffer, unsigned long count, void *data );
#endif

/* ---- Private Variables ------------------------------------------------- */

#ifdef CONFIG_SYSFS
static struct proc_dir_entry * memc_proc_entry = NULL;
#endif

/* ---- Public Variables ------------------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

extern int do_memc_qos (char * const cmd);
extern int do_memc_bwc (char * const cmd);
extern int do_memc_spr (char * const cmd);
extern int do_memc_apal(char * const cmd);
extern int do_memc_regdump(char * const cmd);
extern int do_memc_crc(char * const cmd);

#ifdef CONFIG_SYSFS
static int memc_proc_read( char *page, char **start, off_t off, int count, int *eof, void *data )
{
   int len = 0;

   len += sprintf( &page[len], "Display memory controller settings.\n\n" );
   len += sprintf( &page[len], "Usage: \'echo <action> <unit>\' > /proc/%s\n", MEMC_DEVICE_NAME );
   len += sprintf( &page[len], "<action> : qos|bwc|spr|apal|regdump|crc\n" );
   len += sprintf( &page[len], "<unit>   : sys|vc4|*\n" );
   *eof = 1;

   return len;
}


static int memc_proc_write( struct file *file, const char __user *buffer, unsigned long count, void *data )
{
   unsigned char *local_buffer = NULL;
   int ret = -EINVAL;
   char cmd[MEMC_BUFFER_MAX_SIZE];
   char unit[MEMC_BUFFER_MAX_SIZE];

   local_buffer = kzalloc ( MEMC_BUFFER_MAX_SIZE * sizeof(unsigned char), GFP_KERNEL );
   if ( local_buffer == NULL )
   {
      printk(KERN_ERR "[%s]: failed allocating proc buffer\n", __FUNCTION__ );
      return -ENOMEM;
   }

   if ( count > MEMC_BUFFER_MAX_SIZE - 1 )
   {
      printk( KERN_ERR "[%s]: failed, max length = %d\n", __FUNCTION__, sizeof(local_buffer) );
      return -ENOMEM;
   }

   memset( local_buffer, 0, MEMC_BUFFER_MAX_SIZE );
   memset( cmd, 0, MEMC_BUFFER_MAX_SIZE );
   memset( unit, 0, MEMC_BUFFER_MAX_SIZE );

   if ( copy_from_user( local_buffer, buffer, count ) )
   {
      return -EFAULT;
   }

   if ( sscanf( local_buffer, "%s %s", cmd, unit ) == 2 )
   {
      //printk( KERN_INFO "[%s]: command \'%s\', unit \'%s\'\n\n", __FUNCTION__, cmd, unit );
      if ( strcmp( cmd, "qos" ) == 0 )
      {
         ret = do_memc_qos( unit );
      }
      else if ( strcmp( cmd, "bwc" ) == 0 )
      {
         ret = do_memc_bwc( unit );
      }
      else if ( strcmp( cmd, "spr" ) == 0 )
      {
         ret = do_memc_spr( unit );
      }
      else if ( strcmp( cmd, "apal" ) == 0 )
      {
         ret = do_memc_apal( unit );
      }
      else if ( strcmp( cmd, "regdump" ) == 0 )
      {
         ret = do_memc_regdump( unit );
      }
      else if ( strcmp( cmd, "crc" ) == 0 )
      {
         ret = do_memc_crc( unit );
      }
      else
      {
         ret = -EINVAL;
      }
   }

   if ( ret != 0 )
   {
      printk( KERN_ERR "[%s]: invalid input, use \'cat /proc/%s\' for usage.\n\n", __FUNCTION__, MEMC_DEVICE_NAME );
   }

   kfree ( local_buffer );
   local_buffer = NULL;

   return count;
}
#endif


/***************************************************************************/
/**
*  Platform support constructor
*/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int __init memc_probe( struct platform_device *pdev )
#else
static int memc_probe( struct platform_device *pdev )
#endif
{
   int err = 0;

#ifdef CONFIG_SYSFS
   memc_proc_entry = create_proc_entry( MEMC_DEVICE_NAME,
                                        (S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP ),
                                        NULL );
   if ( memc_proc_entry )
   {
      memc_proc_entry->write_proc = memc_proc_write;
      memc_proc_entry->read_proc  = memc_proc_read;
   }
   else
   {
      printk( KERN_ERR "[%s]: proc entry creation failure\n", __FUNCTION__ );
      err = -EFAULT;
      goto err_ret;
   }
#else
   goto err_ret;
#endif


err_ret:
   return err;
}

/***************************************************************************/
/**
*  Platform support destructor
*/
static int memc_remove( struct platform_device *pdev )
{
#ifdef CONFIG_SYSFS
   if ( memc_proc_entry )
   {
      remove_proc_entry( MEMC_DEVICE_NAME, memc_proc_entry );
      memc_proc_entry = NULL;
   }
#endif

   return 0;
}

/* Platform driver */
static struct platform_driver memc_driver =
{
   .driver     =
   {
      .name    = "bcm-memc",
      .owner   = THIS_MODULE,
   },
   .probe  = memc_probe,
   .remove = memc_remove,
};

static int __init memc_init( void )
{
   MEMC_KNLLOG( "%s: called...\n", __FUNCTION__ );

   return platform_driver_register( &memc_driver );
}

static void __exit memc_exit( void )
{
   MEMC_KNLLOG( "%s: called...\n", __FUNCTION__ );

   platform_driver_unregister( &memc_driver );
}

module_init( memc_init );
module_exit( memc_exit );
MODULE_AUTHOR( "Broadcom" );
MODULE_DESCRIPTION( "Memory Controller QOS Settings." );
MODULE_LICENSE( "GPL" );
