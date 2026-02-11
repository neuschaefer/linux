/*****************************************************************************
* Copyright 2003 - 2009 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/
/**
*
*  @file    csx_amxr.c
*
*  @brief   amxr CSX module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/semaphore.h>

#include <linux/broadcom/amxr.h>
#include <linux/broadcom/amxr_port.h>
#include <linux/broadcom/csx.h>
#include <linux/broadcom/csx_framework.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define CSX_AMXR_SEMAPHORE_TIME_WAIT_MS        10
#define CSX_AMXR_SEMAPHORE_TIME_WAIT_JIFFIES   (msecs_to_jiffies(CSX_AMXR_SEMAPHORE_TIME_WAIT_MS))

/* ---- Private Function Prototypes -------------------------------------- */

static int csx_amxr_set_point( int csx_device_id,
                                   int csx_point_id,
                                   CSX_IO_POINT_FNCS *csx_io_point_fncs,
                                   void *csx_priv );

static int csx_amxr_set_frame_sync( CSX_FRAME_SYNC_FP frame_sync_fncp );

/* ---- Private Variables ------------------------------------------------ */

static char banner[] __initdata = KERN_INFO "CSX amxr: 1.00 (built on "__DATE__" "__TIME__")\n";

/* Module function pointers to register */
static CSX_MODULE_FNCS gModuleFncs =
{
   .csx_module_set_frame_sync = csx_amxr_set_frame_sync,
   .csx_module_set_point = csx_amxr_set_point,
};

DEFINE_SEMAPHORE(csx_amxr_sem);

/* amxr specific information */
static int gNumPorts;
static AMXR_HDL gAmxrHdl;

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/
/**
*  Set a CSX I/O debug point with given device and point identification.
*
*  @return
*     0           - success
*     -ve         - failure code
*/
static int csx_amxr_set_point( int csx_device_id,
                                   int csx_point_id,
                                   CSX_IO_POINT_FNCS *csx_io_point_fncs,
                                   void *csx_priv )
{
   int err;
   /* Validate point identifier. Device identifier is a void* pointer. */
   if ( csx_point_id < AMXR_CSX_POINT_SYNCPORT_SRC_PREMIX || csx_point_id >= AMXR_NUM_CSX_PORT_POINTS )
   {
   	printk( KERN_ERR "%s CSX point ID (%d) not found\n", __FUNCTION__, csx_point_id);
      return -EINVAL;
   }

   err = down_timeout( &csx_amxr_sem, CSX_AMXR_SEMAPHORE_TIME_WAIT_JIFFIES );
   if ( !err )
   {
      err = amxrSetCsxIoPoints( (AMXR_PORT_ID)csx_device_id, csx_point_id, csx_io_point_fncs, csx_priv );
      up( &csx_amxr_sem );

   }

   return err;
}

/***************************************************************************/
/**
*  Set CSX frame sync callback function pointer.
*
*  @return
*     0           - success
*     -ve         - failure code
*/
static int csx_amxr_set_frame_sync( CSX_FRAME_SYNC_FP frame_sync_fncp )
{
   int err;
   err = amxrSetCsxFrameSync( frame_sync_fncp, (void *)CSX_IO_MODULE_AMXR );
   return err;
}

/***************************************************************************/
/**
*  Initialize csx_amxr.  Gathers information from amxr module.
*
*  @return
*     0           - success
*     -ve         - failure code
*/
static int __init csx_amxr_init( void )
{
   int err;
   AMXR_INFO amxrinfo;

   printk( banner );

   gAmxrHdl = amxrAllocateClient();

   if ( IS_ERR( (void*)gAmxrHdl ))
   {
      printk( KERN_ERR "%s: failed to allocate HAL Audio client err=%li\n", __FUNCTION__, PTR_ERR( (void*)gAmxrHdl ));
      return PTR_ERR( (void*)gAmxrHdl );
   }

   err = amxrGetInfo(gAmxrHdl,&amxrinfo);
   if ( err )
   {
      return err;
   }

   gNumPorts = amxrinfo.ports;

   /* Register this module */
   err = csx_register_module( CSX_IO_MODULE_AMXR, &gModuleFncs );

   return err;
}

/***************************************************************************/
/**
*  Clears all CSX I/O points and free the HAL Audio handle.
*
*  @return
*     0           - success
*     -ve         - failure code
*/
static void __exit csx_amxr_exit( void )
{
   CSX_IO_POINT_FNCS csx_ops;
   int pid;
   int point;

   /* Clear all CSX callbacks */
   memset( &csx_ops, 0, sizeof(csx_ops) );

   for ( pid = 0; pid < gNumPorts; pid++ )
   {
   	for ( point = 0; point < AMXR_NUM_CSX_PORT_POINTS; point++ )
   	{
   		amxrSetCsxIoPoints((AMXR_PORT_ID)pid, point, &csx_ops, NULL);
   	}
   }

   /* Clear framesync callback */
   csx_amxr_set_frame_sync( NULL );

   /* De-register ourselves from CSX framework */
   csx_deregister_module( CSX_IO_MODULE_AMXR );

   /* Free the Hal Audio handle */
   amxrFreeClient( gAmxrHdl );

}

module_init( csx_amxr_init );
module_exit( csx_amxr_exit );

MODULE_AUTHOR( "Broadcom" );
MODULE_DESCRIPTION( "CSX amxr module" );
MODULE_LICENSE( "GPL v2" );

