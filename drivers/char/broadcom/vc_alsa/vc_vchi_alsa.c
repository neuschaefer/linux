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

#include <linux/device.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <vc_mem.h>

#include "vmcs_alsa.h"
#include <vc_vchi_audioserv.h>
#include <vchiq_core.h>
#include <vchiq_ioctl.h>
#include <vchiq_arm.h>
#include <vchiq_connected.h>

extern void vc_loopaudio_set_mailbox(void* p, uint32_t phys);

/* ---- Private Constants and Types ----------------------------------------- */

#define VC_ALSA_VERSION       1
#define VC_ALSA_VERSION_MIN   0

/** How long, in milliseconds, we're prepared to wait for a semaphore **/
#define AUD_SEMAPHORE_TIMEOUT_MS         ( 10000 )

/** The down_timeout() function wants time in jiffies **/
#define AUD_SEMAPHORE_TIMEOUT_J   (msecs_to_jiffies(AUD_SEMAPHORE_TIMEOUT_MS))
#define AUDIO_MAKE_FOURCC(num) VCHIQ_MAKE_FOURCC('A', 'U', 'D', 'num##')

typedef struct opaque_AUDIO_INSTANCE_T
{
   VCHIQ_SERVICE_HANDLE_T service;

   struct mutex vchiq_mutex;
   struct semaphore msg_avail;
   struct semaphore bulk_transmit_done;
   struct semaphore service_control;
   struct workqueue_struct *alsa_strm_wq;

   vc_alsa_stream_t *alsa_stream;

   int32_t result;
} AUDIO_INSTANCE_T;

typedef struct 
{
   struct work_struct my_work;
   AUDIO_INSTANCE_T *audio_instance;
   int32_t x;
} CAPRI_ALSA_WORK_T;

/* ---- Private Variables --------------------------------------------------- */
static VCHIQ_INSTANCE_T g_instance = NULL;


/* ---- Private Function Prototypes ----------------------------------------- */
static int32_t vc_vchiq_alsa_stop_worker(AUDIO_INSTANCE_T * audio_instance);
static int32_t vc_vchiq_alsa_start_worker(AUDIO_INSTANCE_T * audio_instance);

/* ---- Private Functions --------------------------------------------------- */

static uint32_t audio_make_fourcc(uint8_t num)
{
   return VCHIQ_MAKE_FOURCC('A', 'U', 'D', num);
}

static void alsa_strm_wq_function(struct work_struct *work)
{
   CAPRI_ALSA_WORK_T *w = (CAPRI_ALSA_WORK_T *) work;
   int32_t ret = -9;
   LOG_DBG("stream %d ..IN %d", w->audio_instance->alsa_stream->idx, w->x);
   switch (w->x)
   {
      case 1:
         ret = vc_vchiq_alsa_start_worker(w->audio_instance);
         break;  
      case 2:
         ret = vc_vchiq_alsa_stop_worker(w->audio_instance);
         break;
      default:
         LOG_ERR("stream %d unexpected work:%d", 
                 w->audio_instance->alsa_stream->idx, w->x);
         break;
   }
   kfree((void *)work);
   LOG_DBG("stream %d ..OUT ret=%d", w->audio_instance->alsa_stream->idx, ret);
}

static void alsa_strm_wq_init(AUDIO_INSTANCE_T * audio_instance)
{
   LOG_DBG("audio instance %p ..IN", audio_instance);
   audio_instance->alsa_strm_wq = create_workqueue("alsa_strm_wq");
   LOG_DBG("audio instance %p ..OUT", audio_instance);
}

static void alsa_strm_wq_quit(AUDIO_INSTANCE_T * audio_instance)
{
   if (audio_instance->alsa_strm_wq)
   {
      flush_workqueue(audio_instance->alsa_strm_wq);
      destroy_workqueue(audio_instance->alsa_strm_wq);
      audio_instance->alsa_strm_wq = NULL;
   }
}

int32_t vc_vchiq_alsa_start(vc_alsa_stream_t * alsa_stream)
{
   int32_t ret = -1;
   AUDIO_INSTANCE_T *audio_instance;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("stream %d Invalid audio instance", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }

   if (audio_instance->alsa_strm_wq)
   {
      CAPRI_ALSA_WORK_T *work = kmalloc(sizeof(CAPRI_ALSA_WORK_T), GFP_ATOMIC);
      /* Queue some work (item 1) */
      if (work)
      {
         INIT_WORK((struct work_struct *)work, alsa_strm_wq_function);
         work->audio_instance = audio_instance;
         work->x = 1;
         if (queue_work(audio_instance->alsa_strm_wq, (struct work_struct*)work))
            ret = 0;  
      }
      else
         LOG_ERR("stream %d.. Error: NULL work kmalloc",alsa_stream->idx);
   }
   out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;  
}

int32_t vc_vchiq_alsa_stop(vc_alsa_stream_t * alsa_stream)
{
   int32_t ret = -1;
   AUDIO_INSTANCE_T *audio_instance;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("stream %d Invalid audio instance", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }
   if (audio_instance->alsa_strm_wq)
   {
      CAPRI_ALSA_WORK_T *work = kmalloc(sizeof(CAPRI_ALSA_WORK_T), GFP_ATOMIC);
      /* Queue some work (item 1) */
      if (work)
      {
         INIT_WORK((struct work_struct *)work, alsa_strm_wq_function);
         work->audio_instance = audio_instance;
         work->x = 2;
         if (queue_work(audio_instance->alsa_strm_wq, (struct work_struct*)work))
            ret = 0;
      }
      else
         LOG_ERR("stream %d.. Error: NULL work kmalloc", alsa_stream->idx);
   }
out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

static VCHIQ_STATUS_T audio_vchiq_callback(VCHIQ_REASON_T reason,
                                           VCHIQ_HEADER_T *header,
                                           VCHIQ_SERVICE_HANDLE_T service,
                                           void *userdata)
{
   AUDIO_INSTANCE_T *audio_instance;
   VC_AUDIO_MSG_T *msg;
   VCHIQ_STATUS_T status = VCHIQ_SUCCESS;


   audio_instance = (AUDIO_INSTANCE_T *)VCHIQ_GET_SERVICE_USERDATA(service);
   if (!audio_instance)
      return VCHIQ_ERROR;

   switch(reason)
   {
      case VCHIQ_SERVICE_OPENED:
         LOG_DBG("Service opened");
         //up(&audio_instance->service_control);
         break;
      case VCHIQ_SERVICE_CLOSED:
         LOG_DBG("stream %d Closed vchiq service", 
                 audio_instance->alsa_stream->idx);
         up(&audio_instance->service_control);
         break;
      case VCHIQ_MESSAGE_AVAILABLE:
         msg = (VC_AUDIO_MSG_T*)(header->data);
         switch (msg->type)
         {
            case VC_AUDIO_MSG_TYPE_RESULT:
               LOG_DBG("stream %d m.type=VC_AUDIO_MSG_TYPE_RESULT, success=%d",
                       audio_instance->alsa_stream->idx, msg->u.result.success);
               audio_instance->result = msg->u.result.success;
               up(&audio_instance->msg_avail);
               break;
            case VC_AUDIO_MSG_TYPE_COMPLETE:
               LOG_DBG("stream %d m.type=VC_AUDIO TYPE_COMPLETE, complete=%d",
                       audio_instance->alsa_stream->idx, msg->u.complete.count);
               atomic_add(msg->u.complete.count, 
                          &(audio_instance->alsa_stream->retrieved));
               (audio_instance->alsa_stream->fifo_irq_handler)
                                             (0, audio_instance->alsa_stream);
               break;
            case VC_AUDIO_MSG_TYPE_LOOPAUDIO_ALLOC: {
               // loopaudio mailbox has been allocated by videocore.
               uint32_t phys_addr = mm_vc_mem_phys_addr + 
                            ((uint32_t)msg->u.loopaudio.res_mem & 0x3FFFFFFF);
               struct resource* res = request_mem_region(phys_addr,
                                     sizeof(VC_LOOPAUDIO_MAILBOX_T), "loopaudio");
               if (!res) {
                   LOG_ERR("cannot alloc loopaudio resource");
                   status = VCHIQ_ERROR;
               } else {
                   void* ptr = ioremap_nocache(res->start, resource_size(res));
                   if (!ptr) {
                      LOG_ERR("cannot map loopaudio mailbox");
                      status = VCHIQ_ERROR;
                   } else {
                       vc_loopaudio_set_mailbox(ptr, phys_addr);
                   }
               }
               break; }
            default:
               LOG_ERR("stream %d unexpected message=%d", 
                       audio_instance->alsa_stream->idx, msg->type);
               status = VCHIQ_ERROR;
               break;
         }
         vchiq_release_message(audio_instance->service, header);
         break;
      case VCHIQ_BULK_TRANSMIT_DONE:
         LOG_DBG("stream %d reason=VCHIQ_BULK_TRANSMIT_DONE", 
                 audio_instance->alsa_stream->idx);
         up(&audio_instance->bulk_transmit_done);
         break;
      default:
         break;
   }
   return status;
}

static void vc_vchiq_alsa_connected_init( void )
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

void vc_vchiq_alsa_init(void)
{
   vchiq_add_connected_callback( vc_vchiq_alsa_connected_init );
}

void vc_vchiq_alsa_deinit(void)
{
   vchiq_shutdown(g_instance);
   g_instance = NULL;
}

int32_t vc_vchiq_alsa_open(vc_alsa_stream_t * alsa_stream)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   AUDIO_INSTANCE_T *audio_instance=NULL;
   VCHIQ_SERVICE_PARAMS_T service_params;
   int32_t ret = 0;

   LOG_DBG("stream %d ..IN",alsa_stream->idx);

   if (alsa_stream->instance)
   {
       LOG_ERR("stream %d stream already opened", alsa_stream->idx);
       ret = -ENODEV;
       goto out;
   }
   
   //Allocate memory for audio service instance
   audio_instance = kzalloc(sizeof(AUDIO_INSTANCE_T), GFP_KERNEL);
   if (audio_instance == NULL)
   {
      LOG_ERR("stream %d Could not allocate audio_instance", alsa_stream->idx);
      ret = -ENOMEM;
      goto out;
   }
   
   /* Create the synch objects */
   mutex_init(&audio_instance->vchiq_mutex);
   sema_init(&audio_instance->msg_avail, 0);
   sema_init(&audio_instance->bulk_transmit_done, 0);
   sema_init(&audio_instance->service_control, 0);

   //Open service at arm and vc both
   memset(&service_params, 0, sizeof(service_params));
   service_params.fourcc = audio_make_fourcc(alsa_stream->idx);
   service_params.callback = audio_vchiq_callback;
   service_params.userdata = (void*)audio_instance;
   service_params.version = VC_ALSA_VERSION;
   service_params.version_min = VC_ALSA_VERSION_MIN;

   if (vchiq_open_service(g_instance, &service_params, 
                                 &audio_instance->service) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not open vchiq service", alsa_stream->idx);
      ret = -EIO;
      goto out;
   }

   //Lock mutex to send a message to vc
   mutex_lock(&audio_instance->vchiq_mutex);

   //Init work queue_work
   alsa_strm_wq_init(audio_instance);
   
   /* Tell VCHIQ that this service is now in use */
   if (vchiq_use_service(audio_instance->service) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not use vchiq service", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   audio_instance->alsa_stream = alsa_stream;
   alsa_stream->instance = audio_instance;

   //Wipe out the results from previous operation
   audio_instance->result = -1;

   // Form the message to be sent to videocore
   msg.type = VC_AUDIO_MSG_TYPE_OPEN;
   msg.u.open.dummy = alsa_stream->idx;
   elem.size = sizeof(msg);
   elem.data = &msg;
   
   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not send open request", alsa_stream->idx);
      ret = -EIO;
      audio_instance->alsa_stream = NULL;
      alsa_stream->instance = NULL;
      goto unlock;
   }

   //Wait for response from videocore
   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait\n", alsa_stream->idx);
	   goto unlock;
   }
   /* MJF: I think an interupt means we don't know if the service is
    * open or not. 
    * 
    */
   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", alsa_stream->idx, audio_instance->result);
      ret = -EIO;
      audio_instance->alsa_stream = NULL;
      alsa_stream->instance = NULL;
      goto unlock;
   }

unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
out:
   if ((ret != 0) && audio_instance)
      kfree(audio_instance);
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

int32_t vc_vchiq_alsa_close(vc_alsa_stream_t * alsa_stream)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   AUDIO_INSTANCE_T *audio_instance;
   int32_t ret = 0;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   //Find out our audio_instance from alsa_stream
   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("stream %d Invalid audio instance", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }

   //Flush the work queue first
   alsa_strm_wq_quit(audio_instance);

   mutex_lock(&audio_instance->vchiq_mutex);

   msg.type = VC_AUDIO_MSG_TYPE_CLOSE;
   msg.u.close.dummy = alsa_stream->idx;
   elem.size = sizeof(msg);
   elem.data = &msg;

   // Wipe out the results from previous operation
   audio_instance->result = -1;

   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not send close request", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   ret = down_interruptible(&audio_instance->msg_avail);
   /* audio instance won't have a valid result, but kill
    * the service anyway.
    */
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait (close)\n",
		   audio_instance->alsa_stream->idx);
	   goto release;
   }
   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", audio_instance->alsa_stream->idx, 
              audio_instance->result);
      ret = -EIO;
      goto unlock;
   }

release:
   if (vchiq_release_service(audio_instance->service) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not release vchiq service", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   vchiq_remove_service(audio_instance->service);
   ret = down_interruptible(&audio_instance->service_control);

   /* MJF: It's a close.  Seriously.  close and move on.
    *
    */
   if (ret == -EINTR)
	   LOG_ERR("stream %d interrupted wait (release)\n",
		   alsa_stream->idx);


   /* Destroy presence of alsastream and audio instance from each other*/
   audio_instance->alsa_stream = NULL;
   alsa_stream->instance = NULL;

unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
   /*
    * We destroy the mutex here but, if an start/stop/config/control is called
    * on the same stream, the behaviour is undefined
    */
   kfree(audio_instance);
out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

static int32_t vc_vchiq_alsa_start_worker(AUDIO_INSTANCE_T *audio_instance)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   int32_t ret = 0;

   LOG_DBG("stream %d ..IN", audio_instance->alsa_stream->idx);

   mutex_lock(&audio_instance->vchiq_mutex);

   msg.type = VC_AUDIO_MSG_TYPE_START;
   msg.u.start.dummy = audio_instance->alsa_stream->idx;
   elem.size = sizeof(msg);
   elem.data = &msg;

   // Wipe out the results from previous operation
   audio_instance->result = -1;

   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not send start request",
              audio_instance->alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait\n",
		   audio_instance->alsa_stream->idx);
	   goto unlock;
   }
   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", audio_instance->alsa_stream->idx,
              audio_instance->result);
      ret = -EIO;
      goto unlock;
   }

unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
   LOG_DBG("stream %d ..OUT ret=%d", audio_instance->alsa_stream->idx, ret);
   return ret;
}

static int32_t vc_vchiq_alsa_stop_worker(AUDIO_INSTANCE_T *audio_instance)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   int32_t ret;

   LOG_DBG("stream %d ..IN", audio_instance->alsa_stream->idx);

   mutex_lock(&audio_instance->vchiq_mutex);

   msg.type = VC_AUDIO_MSG_TYPE_STOP;
   msg.u.stop.draining = audio_instance->alsa_stream->draining;
   elem.size = sizeof(msg);
   elem.data = &msg;

   // Wipe out the results from previous operation
   audio_instance->result = -1;

   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("stream %d Could not send stop request", 
              audio_instance->alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait\n",
		   audio_instance->alsa_stream->idx);
	   goto unlock;
   }

   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", audio_instance->alsa_stream->idx,
              audio_instance->result);
      ret = -EIO;
      goto unlock;
   }

   ret = 0;
unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
   LOG_DBG("stream %d ..OUT ret=%d", audio_instance->alsa_stream->idx, ret);
   return ret;
}

int32_t vc_vchiq_alsa_set_ctls_chan(vc_alsa_stream_t * alsa_stream, 
                                    vc_alsa_chip_t * chip)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   AUDIO_INSTANCE_T *audio_instance;
   int32_t ret;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   //Find out our audio_instance from alsa_stream
   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("alsa stream %d Invalid audio instance", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }

   mutex_lock(&audio_instance->vchiq_mutex);

   //Form the message to be sent to videocore
   msg.type = VC_AUDIO_MSG_TYPE_CONTROL;
   msg.u.control.dest = chip->dest[alsa_stream->idx];
   msg.u.control.volume = chip->volume[alsa_stream->idx];
   elem.size = sizeof(msg);
   elem.data = &msg;

   // Wipe out the results from previous operation
   audio_instance->result = -1;

   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("alsa stream %d Could not send control request", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait\n", alsa_stream->idx);
	   goto unlock;
   }

   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", alsa_stream->idx, audio_instance->result);
      ret = -EIO;
      goto unlock;
   }
   ret = 0;
unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

int32_t vc_vchiq_alsa_set_ctls(vc_alsa_chip_t * chip)
{
   int32_t i;
   int32_t ret = 0;

   LOG_DBG("..IN");
   /* change ctls for all substreams */
   for (i = 0; i < MAX_SUBSTREAMS; i++)
   {
      if (chip->avail_substreams & (1 << i))
      {
         if (!chip->alsa_stream[i])
            ret = 0;
         else if (vc_vchiq_alsa_set_ctls_chan(chip->alsa_stream[i], chip) != 0)
            ret = -1; 
      }
   }

   LOG_DBG("..OUT ret=%d", ret);
   return ret;
}

int32_t vc_vchiq_alsa_set_params(vc_alsa_stream_t * alsa_stream,
                                 uint32_t channels,
                                 uint32_t samplerate,
                                 uint32_t bps)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   AUDIO_INSTANCE_T *audio_instance;
   int32_t ret;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   //Find out our audio_instance from alsa_stream
   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("alsa stream %d Invalid audio instance", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }

   mutex_lock(&audio_instance->vchiq_mutex);

   // Form the message to be sent to videocore
   msg.type = VC_AUDIO_MSG_TYPE_CONFIG;
   msg.u.config.channels = channels;
   msg.u.config.samplerate = samplerate;
   msg.u.config.bps = bps;
   elem.size = sizeof(msg);
   elem.data = &msg;

   // Wipe out the results from previous operation
   audio_instance->result = -1;

   /* Send the message to the videocore */
   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("alsa stream %d Could not send config request", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }

   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   LOG_ERR("stream %d interrupted wait\n", alsa_stream->idx);
	   goto unlock;
   }
   if (audio_instance->result != 0)
   {
      LOG_ERR("stream %d result=%d", alsa_stream->idx, audio_instance->result);
      ret = -EIO;
      goto unlock;
   }
   ret = 0;
unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

int32_t vc_vchiq_alsa_setup(vc_alsa_stream_t * alsa_stream)
{
   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   LOG_DBG("stream %d ..OUT", alsa_stream->idx);

   return 0;
}

int32_t vc_vchiq_alsa_write(vc_alsa_stream_t * alsa_stream,
                            uint32_t count,
                            void *src)
{
   VC_AUDIO_MSG_T msg;
   VCHIQ_ELEMENT_T elem;
   AUDIO_INSTANCE_T *audio_instance;
   int32_t ret = 0;
   int retries = 0;

   LOG_DBG("stream %d ..IN %d bytes from %p", alsa_stream->idx, count, src);

   //Find out our audio_instance from alsa_stream
   audio_instance = (AUDIO_INSTANCE_T*)alsa_stream->instance;
   if (!audio_instance)
   {
      LOG_ERR ("Invalid audio instance for stream %d", alsa_stream->idx);
      ret = -EINVAL;
      goto out;
   }

   /*
    * Check if the stream is already started, wait otherwise
    */

   mutex_lock(&audio_instance->vchiq_mutex);

   msg.type = VC_AUDIO_MSG_TYPE_WRITE;
   msg.u.write.count = count;
   msg.u.write.silence = src == NULL;
   elem.size = sizeof(msg);
   elem.data = &msg;

   /* Send the message to the videocore */

   if (vchiq_queue_message(audio_instance->service, &elem, 1) != VCHIQ_SUCCESS)
   {
      LOG_ERR("alsa stream %d Could not send write request", alsa_stream->idx);
      ret = -EIO;
      goto unlock;
   }
   LOG_DBG("stream %d ... sent header", alsa_stream->idx);

retry:
   /* MJF: Try hard, because we don't know if the request
    * failed or not unless the down returns
    */
   ret = down_interruptible(&audio_instance->msg_avail);
   if (ret == -EINTR) {
	   retries++;
	   LOG_ERR("stream %d interrupted wait %s\n", alsa_stream->idx,
		   retries < 3 ? "retrying" : "not retrying");
	   if (retries < 3)
		   goto retry;
	   goto unlock;
   }
   if (audio_instance->result == 0)
   {
      if (!msg.u.write.silence)
      {
         int ret;
         /* Send the message to the videocore */
         if ((ret = vchiq_queue_bulk_transmit(audio_instance->service, src, count, 
                                       audio_instance)) != VCHIQ_SUCCESS)
         {
            LOG_ERR("stream %d Could not send bulk data: %d", alsa_stream->idx, ret);
            ret = -EIO;
            goto unlock;
         }
         ret = down_interruptible(&audio_instance->bulk_transmit_done);
	 if (ret == -EINTR) {
		 LOG_ERR("stream %d interrupted wait\n", alsa_stream->idx);
		 goto unlock;
	 }
         LOG_DBG("stream %d ... sent bulk data", alsa_stream->idx);
         goto unlock;
      }
   }
   else
   {
      LOG_DBG("stream %d Could not send bulk data result=%d", alsa_stream->idx,
              audio_instance->result);
   }

unlock:
   mutex_unlock(&audio_instance->vchiq_mutex);
out:
   LOG_DBG("stream %d ..OUT ret=%d", alsa_stream->idx, ret);
   return ret;
}

/*
 * Returns all buffers from arm->vc
 */
void vc_vchiq_alsa_flush_buffers(vc_alsa_stream_t * alsa_stream)
{
   LOG_DBG("stream %d ..IN", alsa_stream->idx);
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
   return;
}

/*
 * Forces VC to flush(drop) its filled playback buffers and 
 * return them the us. (VC->ARM)
 */
void vc_vchiq_alsa_flush_playback_buffers(vc_alsa_stream_t * alsa_stream)
{
   LOG_DBG("stream %d..IN", alsa_stream->idx);
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
}

uint32_t vc_vchiq_alsa_retrieve_buffers(vc_alsa_stream_t * alsa_stream)
{
   uint32_t count = atomic_read(&alsa_stream->retrieved);
   atomic_sub(count, &alsa_stream->retrieved);
   return count;
}
