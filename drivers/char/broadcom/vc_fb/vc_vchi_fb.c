/*****************************************************************************
 * Copyright 2011 Broadcom Corporation.  All rights reserved.
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

// ---- Include Files --------------------------------------------------------
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#include <vc_fb_defs.h>
#include "vc_fb.h"
#include <vc_vchi_fb.h>

#include <vchiq_if.h>
#include <vchiq_core.h>
#include <vchiq_arm.h>
#include <vchiq_connected.h>

// ---- Private Constants and Types ------------------------------------------

#define FB_VC_COUNTS_EQUAL  fb_vchiq_instance->scrn_info->fb_update_count == fb_vchiq_instance->scrn_info->vc_update_count
#define FB_VSYNC_WAIT_QUEUE fb_vchiq_instance->scrn_info->wait_for_vc_status
#define FB_VC_UPDATE_COUNT  fb_vchiq_instance->scrn_info->vc_update_count
#define FB_FB_UPDATE_COUNT  fb_vchiq_instance->scrn_info->fb_update_count
#define FB_FB_NUM           fb_vchiq_instance->scrn_info->fb_num

typedef struct
{
   VCHIQ_SERVICE_HANDLE_T service;

   struct mutex vchiq_mutex;
   struct semaphore msg_avail;
   struct semaphore service_control;

   SCRN_INFO_T *scrn_info;

   VC_FB_MSG_REPLY_T reply;

} FB_VCHIQ_INSTANCE_T;


// ---- Private Variables ----------------------------------------------------
static VCHIQ_INSTANCE_T g_instance = NULL;

// ---- Private Function Prototypes ------------------------------------------

// ---- Private Functions ----------------------------------------------------

static VCHIQ_STATUS_T fb_send_message( FB_VCHIQ_INSTANCE_T* fb_vchiq_instance,
                                       VC_FB_MSG_RQUEST_T* msg )
{
   VCHIQ_ELEMENT_T elem;

   elem.size = sizeof( *msg );
   elem.data = msg;
   return vchiq_queue_message( fb_vchiq_instance->service, &elem, 1 );

}

static VCHIQ_STATUS_T fb_vchiq_callback( VCHIQ_REASON_T reason,
                                         VCHIQ_HEADER_T *header,
                                         VCHIQ_SERVICE_HANDLE_T service,
                                         void *userdata )
{

   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance;
   VC_FB_MSG_REPLY_T *reply;
   VCHIQ_STATUS_T status = VCHIQ_SUCCESS;


   fb_vchiq_instance = 
   (FB_VCHIQ_INSTANCE_T*)VCHIQ_GET_SERVICE_USERDATA(service);
   if ( !fb_vchiq_instance)
      return VCHIQ_ERROR;

   switch ( reason )
   {
      case VCHIQ_SERVICE_OPENED:
         LOG_DBG(  "Service opened" );
         //up(&audio_instance->service_control );
         break;
      case VCHIQ_SERVICE_CLOSED:
         LOG_DBG( "fb=%u: Closed vchiq service", FB_FB_NUM );
         up( &fb_vchiq_instance->service_control );
         break;
      case VCHIQ_MESSAGE_AVAILABLE:
         reply = (VC_FB_MSG_REPLY_T*)(header->data );
         switch (reply->type)
         {
            case VC_FB_MSG_TYPE_PAN:
               LOG_DBG( "fb=%u: type=PAN, success=%d %d %u %u", FB_FB_NUM,
                        reply->success, FB_FB_UPDATE_COUNT, FB_VC_UPDATE_COUNT,
                        reply->u.result.update_count  );
               FB_VC_UPDATE_COUNT = reply->u.result.update_count;
               if ( FB_VC_COUNTS_EQUAL )
                  wake_up_all( &FB_VSYNC_WAIT_QUEUE );
               break;
            case VC_FB_MSG_TYPE_GET_SCRN_INFO:
            case VC_FB_MSG_TYPE_ALLOC:
            case VC_FB_MSG_TYPE_FREE:
            case VC_FB_MSG_TYPE_SWAP:
            case VC_FB_MSG_TYPE_CFG:
               LOG_DBG( "fb=%u: type=%d, success=%d", FB_FB_NUM, reply->type,
                        reply->success );
               fb_vchiq_instance->reply = (*reply);
               up( &fb_vchiq_instance->msg_avail );
               break;
            default:
               LOG_ERR( "fb=%u: unexpected msg=%d", FB_FB_NUM, reply->type );
               status = VCHIQ_ERROR;
               break;
         }
         vchiq_release_message( fb_vchiq_instance->service, header );
         break;
         default:
            break;
   }
   return status;
}

static void vc_vchiq_fb_connected_init( void )
{
   if ( !g_instance)
   {
      /* Initialise an instance of the VCHIQ and connect to it */
      if ( vchiq_initialise( &g_instance ) != VCHIQ_SUCCESS )
      {
         LOG_ERR( "Could not vchiq_initialise" );
         return;
      }
      LOG_INFO("Initialized vchiq" );
      if ( vchiq_connect( g_instance ) != VCHIQ_SUCCESS )
      {
         LOG_ERR( "Could not vchiq_connect" );
         vchiq_shutdown( g_instance );
         g_instance = NULL;
         return;
      }
   }
}

void vc_vchiq_fb_init( void )
{
   vchiq_add_connected_callback( vc_vchiq_fb_connected_init );
}

void vc_vchiq_fb_deinit( void )
{
   vchiq_shutdown( g_instance );
   g_instance = NULL;   
}

int32_t vc_vchiq_fb_open( SCRN_INFO_T* scrn_info )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T* fb_vchiq_instance = NULL;
   VCHIQ_SERVICE_PARAMS_T service_params;

   if ( scrn_info->instance != NULL )
   {
      LOG_ERR( "fb=%u device already opened", scrn_info->fb_num );
      ret = -ENODEV;
      goto out;
   }   

   //Allocate memory for fb service instance
   fb_vchiq_instance = kzalloc(sizeof( FB_VCHIQ_INSTANCE_T), GFP_KERNEL );
   if ( fb_vchiq_instance == NULL )
   {
      LOG_ERR( "fb=%u Could not allocate audio_instance", scrn_info->fb_num );
      ret = -ENOMEM;
      goto out;
   }
   /* Create the synch objects */
   mutex_init( &fb_vchiq_instance->vchiq_mutex );
   sema_init( &fb_vchiq_instance->msg_avail, 0 );
   sema_init( &fb_vchiq_instance->service_control, 0 );

   //Open service at arm and vc both
   mutex_lock( &fb_vchiq_instance->vchiq_mutex );
   memset( &service_params, 0, sizeof( service_params ) );
   service_params.fourcc = VCHIQ_MAKE_FOURCC('F', 'B', 'D', scrn_info->fb_num );
   service_params.version = VC_FB_VER;
   service_params.version_min = VC_FB_VER_MIN;
   service_params.callback = fb_vchiq_callback;
   service_params.userdata = (void*)fb_vchiq_instance;

   if ( vchiq_open_service( g_instance, &service_params,
                            &fb_vchiq_instance->service ) != VCHIQ_SUCCESS )
   {
      LOG_ERR( "fb=%u Could not open vchiq service", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

   /* Tell VCHIQ that this service is now in use */
   if ( vchiq_use_service( fb_vchiq_instance->service ) != VCHIQ_SUCCESS )
   {
      LOG_ERR( "fb=%u Could not use vchiq service", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }
   fb_vchiq_instance->scrn_info = scrn_info;
   scrn_info->instance = fb_vchiq_instance;
unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );
out:
   return ret;
}

int32_t vc_vchiq_fb_close( SCRN_INFO_T* scrn_info )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T* fb_vchiq_instance = NULL;

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u device already closed", scrn_info->fb_num );
      ret = -ENODEV;
      goto out;
   }
   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );

   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   if ( vchiq_release_service(fb_vchiq_instance->service) != VCHIQ_SUCCESS )
   {
      LOG_ERR( "fb=%u Could not release vchiq service", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }
   vchiq_remove_service( fb_vchiq_instance->service );
   ret = down_interruptible( &fb_vchiq_instance->service_control );
   if (ret == -EINTR)
	   LOG_ERR("fb=%u failed to close. FORCING\n", scrn_info->fb_num);
   fb_vchiq_instance->scrn_info = NULL;
   scrn_info->instance = NULL;

unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );
   kfree(fb_vchiq_instance );
out:
   return ret;
}

int32_t vc_vchiq_fb_get_scrn_info( SCRN_INFO_T* scrn_info,
                                   VC_FB_SCRN_INFO_T *info )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance = NULL;
   VC_FB_MSG_RQUEST_T msg;

   if ( scrn_info->scrn >= VC_FB_SCRN_MAX )
   {
      LOG_ERR( "fb=%u: invalid screen", scrn_info->fb_num );
      return -1;
   }

   if ( info == NULL)
   {
      LOG_ERR( "fb=%u: invalid info pointer", scrn_info->fb_num );
      return -1;
   }

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u: invalid instance pointer", scrn_info->fb_num );
      return -1;
   }

   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );

   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   msg.type = VC_FB_MSG_TYPE_GET_SCRN_INFO;
   msg.u.get_scrn_info.scrn = scrn_info->scrn;  

   //Wipe out the result from previous operation
   memset( &fb_vchiq_instance->reply, 0, sizeof( VC_FB_MSG_REPLY_T ) );

   // Send the message to the videocore
   if ( fb_send_message(fb_vchiq_instance, &msg) != VCHIQ_SUCCESS)
   {
      LOG_ERR( "fb=%u: failed to queue message", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

   // We are expecting a reply from the videocore
   ret = down_interruptible( &fb_vchiq_instance->msg_avail );
   if (ret == -EINTR) {
	   LOG_ERR("fb=%u operation interrupted in get screen info.\n",
		   scrn_info->fb_num);
	   goto unlock;
   }

   if ( fb_vchiq_instance->reply.success != 0 )
   {
      LOG_ERR( "fb=%u: videocore operation failed (success=%d)", 
               scrn_info->fb_num, fb_vchiq_instance->reply.success );
      ret = -EFAULT;
      goto unlock;
   }
   *info = fb_vchiq_instance->reply.u.scrn_info;
unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );

   return ret;
}

int32_t vc_vchiq_fb_alloc( SCRN_INFO_T *scrn_info,
                           VC_FB_ALLOC_T* alloc,
                           VC_FB_ALLOC_RESULT_T *alloc_result )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance = NULL;
   VC_FB_MSG_RQUEST_T msg;

   if ( scrn_info->scrn >= VC_FB_SCRN_MAX )
   {
      LOG_ERR( "fb=%u: invalid screen", scrn_info->fb_num );
      return -1;
   }

   if ( alloc == NULL)
   {
      LOG_ERR( "fb=%u: invalid info pointer", scrn_info->fb_num );
      return -1;
   }

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u: invalid instance pointer", scrn_info->fb_num );
      return -1;
   }

   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );
   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   msg.type = VC_FB_MSG_TYPE_ALLOC;
   msg.u.alloc = *alloc;

   //Wipe out the result from previous operation
   memset( &fb_vchiq_instance->reply, 0, sizeof( VC_FB_MSG_REPLY_T ) );

   // Send the message to the videocore
   if ( fb_send_message(fb_vchiq_instance, &msg) != VCHIQ_SUCCESS)
   {
      LOG_ERR( "fb=%u: failed to queue message", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

   // We are expecting a reply from the videocore
   ret = down_interruptible( &fb_vchiq_instance->msg_avail );
   if (ret == -EINTR) {
	   LOG_ERR("fb=%u operation interrupted in alloc\n",
		   scrn_info->fb_num);
	   goto unlock;
   }

   if ( fb_vchiq_instance->reply.success != 0 )
   {
      LOG_ERR( "fb=%u: videocore operation failed (success=%d)",
               scrn_info->fb_num, fb_vchiq_instance->reply.success );
      ret = -EFAULT;
      goto unlock;
   }
   *alloc_result = fb_vchiq_instance->reply.u.alloc_result;

unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );
   return ret;
}

int32_t vc_vchiq_fb_free( SCRN_INFO_T* scrn_info )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance = NULL;
   VC_FB_MSG_RQUEST_T msg;


   if ( scrn_info->scrn >= VC_FB_SCRN_MAX )
   {
      LOG_ERR( "fb=%u: invalid screen", scrn_info->fb_num );
      return -1;
   }

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u: invalid instance pointer", scrn_info->fb_num );
      return -1;
   }

   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );
   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   msg.type = VC_FB_MSG_TYPE_FREE;
   msg.u.free.res_handle = scrn_info->res_handle;

   //Wipe out the result from previous operation
   memset( &fb_vchiq_instance->reply, 0, sizeof( VC_FB_MSG_REPLY_T ) );

   // Send the message to the videocore
   if ( fb_send_message(fb_vchiq_instance, &msg) != VCHIQ_SUCCESS)
   {
      LOG_ERR( "fb=%u: failed to queue message", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

   // We are expecting a reply from the videocore
   ret = down_interruptible( &fb_vchiq_instance->msg_avail );
   if (ret == -EINTR) {
	   LOG_ERR("fb=%u interrupted in free\n", scrn_info->fb_num);
	   goto unlock;
   }

   if ( fb_vchiq_instance->reply.success != 0 )
   {
      LOG_ERR( "fb=%u: videocore operation failed (success=%d)",
               scrn_info->fb_num, fb_vchiq_instance->reply.success );
      ret = -EFAULT;
      goto unlock;
   }

unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );

   return ret;
}

int32_t vc_vchiq_fb_pan( SCRN_INFO_T* scrn_info, VC_FB_PAN_T* pan )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance = NULL;
   VC_FB_MSG_RQUEST_T msg;

   if ( scrn_info->scrn >= VC_FB_SCRN_MAX )
   {
      LOG_ERR( "fb=%u: invalid screen", scrn_info->fb_num );
      return -1;
   }

   if ( pan == NULL)
   {
      LOG_ERR( "fb=%u: invalid info pointer", scrn_info->fb_num );
      return -1;
   }

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u: invalid instance pointer", scrn_info->fb_num );
      return -1;
   }

   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );
   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   msg.type = VC_FB_MSG_TYPE_PAN;
   msg.u.pan = *pan;

   // Send the message to the videocore
   if ( fb_send_message(fb_vchiq_instance, &msg) != VCHIQ_SUCCESS)
   {
      LOG_ERR( "fb=%u: failed to queue message", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );
   return ret;
}

#if 0
int32_t vc_vchiq_fb_swap(uint32_t res_handle,
uint32_t active_frame,
int32_t connection)
{
   int ret;
   int32_t success;
   uint32_t msg_len;
   VC_FB_MSG_HDR_T *msg_hdr;
   VC_FB_SWAP_T *swap;
   VC_FB_RESULT_T result;

   /*
    * Do nothing for LCD at present; 
    */
   if ( connection < 0)
   {
      LOG_ERR ("Invalid fb service connection id" );
      return -1;
      }

      mutex_lock( &instance->vchi_mutex[connection] );
      vchi_service_use(instance->vchi_handle[connection] );

      msg_len = sizeof( *msg_hdr ) + sizeof( *swap );
      mutex_lock( &instance->msg_buf[connection][0], 0, msg_len );

      msg_hdr = (VC_FB_MSG_HDR_T *)(&instance->msg_buf[connection][0] );
      msg_hdr->type = VC_FB_MSG_TYPE_SWAP;

      swap = (VC_FB_SWAP_T *)msg_hdr->body;
      swap->res_handle = res_handle;
      swap->active_frame = active_frame;

      // Send the message to the videocore
      success = vchi_msg_queue(instance->vchi_handle[connection],
      &instance->msg_buf[connection][0], msg_len,
      VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL );
      if (  success != 0 )
      {
         LOG_ERR( "%s: failed to queue message (success=%d)",
         __func__, success );

         ret = -1;
         goto unlock;
         }

         // We are expecting a reply from the videocore
         ret = down_interruptible( &instance->msg_avail[connection] );
	 if (ret == -EINTR) {
		 LOG_ERR("fb=%u interrupted in swap\n", scrn_info->fb_num);
		 goto unlock;
	 }

         success = vchi_msg_dequeue(instance->vchi_handle[connection],
         &result, sizeof( result ),
         &msg_len, VCHI_FLAGS_NONE );
         if (  success != 0 )
         {
            LOG_ERR( "%s: videocore operation failed (success=%d)",
            __func__, success );
   
            ret = -1;
            goto unlock;
            }
            else if (  msg_len != sizeof( result))
            {
               LOG_ERR( "%s: incorrect msg length %u (expected=%u)", __func__, msg_len,
               sizeof( result ) );
      
               ret = -1;
               goto unlock;
            }
   
            ret = result.success ? -1 : 0;
   
            unlock:
            vchi_service_release(instance->vchi_handle[connection] );
            mutex_unlock( &instance->vchi_mutex[connection] );
   
            return ret;
            }
            #endif
   
int32_t vc_vchiq_fb_cfg( SCRN_INFO_T* scrn_info,
                         VC_FB_CFG_T *cfg,
                         VC_FB_CFG_RESULT_T *cfg_result )
{
   int ret = 0;
   FB_VCHIQ_INSTANCE_T *fb_vchiq_instance = NULL;
   VC_FB_MSG_RQUEST_T msg;

   if ( scrn_info->scrn >= VC_FB_SCRN_MAX )
   {
      LOG_ERR( "fb=%u: invalid screen", scrn_info->fb_num );
      return -1;
   }

   if ( cfg == NULL)
   {
      LOG_ERR( "fb=%u: invalid info pointer", scrn_info->fb_num );
      return -1;
   }

   if ( scrn_info->instance == NULL )
   {
      LOG_ERR( "fb=%u: invalid instance pointer", scrn_info->fb_num );
      return -1;
   }

   fb_vchiq_instance = (FB_VCHIQ_INSTANCE_T*)(scrn_info->instance );
   mutex_lock( &fb_vchiq_instance->vchiq_mutex );

   msg.type = VC_FB_MSG_TYPE_CFG;
   msg.u.cfg = *cfg;

   //Wipe out the result from previous operation
   memset( &fb_vchiq_instance->reply, 0, sizeof( VC_FB_MSG_REPLY_T ) );

   // Send the message to the videocore
   if ( fb_send_message(fb_vchiq_instance, &msg) != VCHIQ_SUCCESS)
   {
      LOG_ERR( "fb=%u: failed to queue message", scrn_info->fb_num );
      ret = -EIO;
      goto unlock;
   }

   // We are expecting a reply from the videocore
   ret = down_interruptible( &fb_vchiq_instance->msg_avail );
   if (ret == -EINTR) {
	   LOG_ERR("fb=%u interrupted in cfg\n", scrn_info->fb_num);
	   goto unlock;
   }

   if ( fb_vchiq_instance->reply.success != 0 )
   {
      LOG_ERR( "fb=%u: videocore operation failed (success=%d)",
      scrn_info->fb_num, fb_vchiq_instance->reply.success );
      ret = -EFAULT;
      goto unlock;
   }
   *cfg_result = fb_vchiq_instance->reply.u.cfg_result;

   unlock:
   mutex_unlock( &fb_vchiq_instance->vchiq_mutex );

   return ret;
}
