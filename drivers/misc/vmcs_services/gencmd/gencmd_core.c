/*
 *  gencmd_kernel.c - core gencmd driver
 *
 *  Copyright (C) 2010 Broadcom
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This device provides a shared mechanism for writing to the mailboxes,
 * semaphores, doorbells etc. that are shared between the ARM and the VideoCore
 * processor
 */
#include <linux/module.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>
#include <linux/proc_fs.h>
#include <linux/semaphore.h>
#include <asm/sizes.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/stat.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <interface/vchi/vchi.h>
#include "vchiq_connected.h"

#include "gencmd.h"
#include "gencmd_regs.h"
#include "gencmd_driver.h"

#define VC_GENCMD_VER   1 // check vc_gencmd_defs.h
#define GENCMD_MAX_RESPONSE_SIZE 128

/* macros */
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((unsigned int)(unsigned char)(ch0) \
		| ((unsigned int)(unsigned char)(ch1) << 8) \
		| ((unsigned int)(unsigned char)(ch2) << 16) \
		| ((unsigned int)(unsigned char)(ch3) << 24 ))

#define GENCMD_FOURCC MAKEFOURCC( 'G', 'E', 'N', 'C')

/* structure decalaration */
static struct {
   VCHI_INSTANCE_T       vchi_instance;
   VCHI_CONNECTION_T     *vchi_connection;
   VCHI_SERVICE_HANDLE_T vchi_handle;
   struct semaphore      msg_avail;
   struct mutex          vchi_mutex;
   int                   initialized;
   struct proc_dir_entry *dump_info;
} gencmd_state;

/* global function */

struct gencmd_buffers {
        char cmd[GENCMD_CMD_SIZE];
        char resp[GENCMD_RESP_SIZE];
        int  resp_len;
        struct semaphore buf_sem;
};

static void gencmd_callback( void *callback_param,
                             const VCHI_CALLBACK_REASON_T reason,
                             void *msg_handle )
{
   (void)msg_handle;
   if (reason != VCHI_CALLBACK_MSG_AVAILABLE)
   {
      printk(KERN_ERR"[%s] callback for unknown reason %d\n", __func__, reason);
      return;
   }

   up(&gencmd_state.msg_avail);
}

static int vc_vchi_gencmd_init(void)
{
   int32_t success = 0;
   SERVICE_CREATION_T gencmd_parameters = { VCHI_VERSION(VC_GENCMD_VER),
         MAKE_FOURCC("GCMD"), gencmd_state.vchi_connection, 0, 0,
         gencmd_callback, NULL, 0, 0, 0 };

   // record the number of connections
   success = vchi_initialise(&gencmd_state.vchi_instance);
   if ( success != 0 )
   {
      printk(KERN_ERR"[%s] failed to vchi initialize (success=%d)\n",__func__, success);
      goto out;
   }

   success = vchi_connect(NULL, 0, gencmd_state.vchi_instance);
   if ( success != 0 )
   {
      printk(KERN_ERR"[%s] failed to connect VCHI instance (success=%d)\n", __func__, success);
      goto out;
   }

   mutex_init(&gencmd_state.vchi_mutex);
   sema_init(&gencmd_state.msg_avail, 0);

   success = vchi_service_open(gencmd_state.vchi_instance, &gencmd_parameters, &gencmd_state.vchi_handle);
   if (success != 0)
   {
      printk(KERN_ERR"[%s] failed to open service (success=%d)\n", __func__, success);
      goto out;
   }

   success = vchi_service_release(gencmd_state.vchi_handle);
   if (success != 0)
   {
      printk(KERN_ERR"[%s] failed to release service (success=%d)\n", __func__, success);
      goto out;
   }
out:
   return success;
}

int vc_gencmd_send_list ( const char *format, va_list a )
{
   char command_buffer[GENCMD_CMD_SIZE+1];
   int success = -1;
   int length;
   // Obtain the lock and keep it so no one else can butt in while we await the response.

   length = vsnprintf( command_buffer, GENCMD_CMD_SIZE, format, a );

   if (length == 0 && length > GENCMD_CMD_SIZE)
   {
      printk(KERN_ERR"[%s] wrong length %d\n", __func__, length);
      goto out;
   }
   gencmd_print("-queue %s\n", gencmd_state.command_buffer);
   success = vchi_msg_queue( gencmd_state.vchi_handle,
         command_buffer, length+1, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL );

   if (success != 0) {
      printk(KERN_ERR"[%s] failed vchi msg queue\n", __func__);
   }
out:
   return success;
}

int vc_gencmd_read_response (char *response, int maxlen)
{
   char response_buffer[GENCMD_MAX_RESPONSE_SIZE];
   int response_length = 0;
   int success = 0;
   int ret = 0;
   int ret_code;

   ret = down_interruptible(&gencmd_state.msg_avail);
   if (ret < 0) {
     printk(KERN_ERR"[%s] failed to get the semaphore error=%d\n", __func__, ret);
     goto out;
   }

   success = vchi_msg_dequeue( gencmd_state.vchi_handle, response_buffer,
         sizeof(response_buffer), &response_length, VCHI_FLAGS_NONE);
   if(success == 0) {
      ret_code = ( *(int *)response_buffer );
   }
   else {
      ret = 0;
      goto out;
   }

   if(response_length) {
      response_length -= sizeof(int); //first word is error code
      response_length = min((int)response_length, (int)maxlen);
      memcpy(response, response_buffer+sizeof(int), (size_t)response_length);
      ret = response_length;
   }

out:
   return ret;
}

int vc_gencmd(char *response, int maxlen, const char *format, ...)
{
   va_list args;
   int response_len = 0;

   if (0 == gencmd_state.initialized) {
      printk("GENCMD driver not initialized yet!\n");
      return 0; 
   }

   mutex_lock(&gencmd_state.vchi_mutex);
   if(0 != vchi_service_use(gencmd_state.vchi_handle)) {
      printk(KERN_ERR"[%s] failed to use service\n", __func__);
      goto out;
   }

   va_start(args, format);
   if(0 == vc_gencmd_send_list(format, args)) {
      response_len = vc_gencmd_read_response(response, maxlen);
   }
   else{
      printk(KERN_ERR"[%s] failed to send command\n", __func__);
   }
   va_end (args);

   if(0 != vchi_service_release(gencmd_state.vchi_handle)) {
      printk(KERN_ERR"[%s] failed to release service\n", __func__);
   }

out:
   mutex_unlock(&gencmd_state.vchi_mutex);
   return response_len;
}
EXPORT_SYMBOL(vc_gencmd);

/* static functions */
int proc_vc_gencmd(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char gencmd_req[200], gencmd_resp[100];
	int ret;

	if( count > (sizeof(gencmd_req) - 1) ) {
		printk(KERN_ERR"%s:gencmd max length=%d\n", __func__, sizeof(gencmd_req));
		return -ENOMEM;
	}
	/* write data to buffer */
	if( copy_from_user( gencmd_req, buffer, count) ) 
		return -EFAULT;
	gencmd_req[count] = '\0';

	printk("gencmd: %s\n",gencmd_req);
	ret = vc_gencmd(gencmd_resp, sizeof(gencmd_resp), gencmd_req);

	if ( ret > 0)
		printk(KERN_ERR"response: %s\n", gencmd_resp);

	return count;
}

static struct platform_driver bcm2708_gencmd_driver = {
	.driver		= {
		.name = "bcm2835_GENC",
		.owner = THIS_MODULE,
	},
};

static char __initdata banner[] = "VC4 Gencmd Driver,(C) 2010 Broadcom Corp\n";

/* Videocore connected.
*/
static void vc_gencmd_connected_init(void)
{
        /* initialize the gencmd struct */
        memset(&gencmd_state, 0, sizeof(gencmd_state));

        /* initialize the VCHIQ interface */
        if (vc_vchi_gencmd_init() < 0 )
        {
                printk(KERN_ERR"[%s] failed to vchi initialize \n", __func__);
                return;
        }

        /* register the driver */
        if( 0 > gencmd_driver_init())
        {
                printk(KERN_ERR"[%s] failed to initialize gencmd \n", __func__);
                return;
        }

        gencmd_state.initialized = 1;
}

static int __init bcm2708_gencmd_init( void )
{
	printk(KERN_ERR"%s", banner);

        vchiq_add_connected_callback(vc_gencmd_connected_init);

	return platform_driver_register(&bcm2708_gencmd_driver);
}

static void __exit bcm2708_gencmd_exit( void )
{
	
	platform_driver_unregister(&bcm2708_gencmd_driver);
}

subsys_initcall(bcm2708_gencmd_init);
module_exit(bcm2708_gencmd_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("GenCmd Kernel Driver");

/* ************************************ The End ***************************************** */
