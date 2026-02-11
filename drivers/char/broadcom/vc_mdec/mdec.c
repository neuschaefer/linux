/*
 * Copyright (c) 2012 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/****************************************************************************
	System Includes
 ****************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/semaphore.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <asm/bitops.h>

/****************************************************************************
	Project Includes
 ****************************************************************************/

#include "vchiq_core.h"
#include "vchiq_ioctl.h"
#include "vchiq_arm.h"

#include "mdec_codecs.h"
#include "mdec.h"
#include "mdec_msg.h"

/*****************************************************************************
  Private typedefs, macros and constants.
 *****************************************************************************/

#define VC_MDEC_VERSION       1
#define VC_MDEC_VERSION_MIN   0

/** Return the MINimum of two scalar values **/
#define MIN(a,b)                          ((a)<(b)?(a):(b))

/** Return the MAXimum fo two scalar values **/
#define MAX(a,b)                          ((a)>(b)?(a):(b))

/** The device name used throughout the driver **/
#define DEVICE_NAME                       "mdec"

/** Override the default prefix, which would be mdec (from the filename) **/
#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX DEVICE_NAME   "."

/** Specify the depth of the callback FIFO **/
#define MDEC_USER_CALLBACK_FIFO_DEPTH     ( 64 )

/** How long, in milliseconds, we're prepared to wait for a semaphore **/
#define MDEC_SEMAPHORE_TIMEOUT_MS         ( 10000 )

/** The down_timeout() function wants time in jiffies **/
#define MDEC_SEMAPHORE_TIMEOUT_J   (msecs_to_jiffies(MDEC_SEMAPHORE_TIMEOUT_MS))

/** How many times to sample playback progess **/
#define MDEC_PLAYBACK_TIME_SAMPLES        ( 10 )

/** How long to wait in schedule timeouts **/
#define MDEC_SCHEDULE_TIMEOUT_MS          ( 100 )

/** How long to wait if no buffers free on VideoCore **/
#define MDEC_NO_BUFFERS_DELAY_MS				( 10 )

/** Check if no free buffers **/
#define NO_FREE_BUFFERS(b)						( 0xFFFFFFFFU == (b) )

/** Identify the stream types **/
typedef enum {
   AUDIO_STREAM = 0x0,
   VIDEO_STREAM,
} media_stream_t;

/** MediaDec service context descriptor **/
typedef struct {
   VCHIQ_SERVICE_HANDLE_T service;
   struct semaphore sem_lock; /* Lock on the mdec context */
   struct semaphore sem_msg;  /* Raised when a VCHIQ message is received. */

   /* Record the response from the last VCHIQ control message */
   int  response_status;
   void *response;
   size_t response_size;
	
	/* Bulk Rx control for frame capture */
	struct semaphore sem_bulk_rx;
	struct semaphore sem_frame_lock;
	int bulk_rx_status;
	
	unsigned long bitflags;
#define AUDIO_BUFFER_AVAILABLE		( 0 )
#define VIDEO_BUFFER_AVAILABLE		( 1 )	

   /* User callback reason and FIFO */
   struct {
      struct semaphore sem_user_callback;
      struct semaphore sem_lock;
      MDEC_WAIT_FOR_CALLBACK_T fifo[MDEC_USER_CALLBACK_FIFO_DEPTH];
      unsigned int head, tail;
   } cb;
} MDEC_SERVICE_T;

/****************************************************************************
	Private Function Declarations.  Declare as static.
 ****************************************************************************/

/** File operations **/
static int     mdec_open(struct inode *inode, struct file *file_id);
static int     mdec_release(struct inode *inode, struct file *file_id );
static long    mdec_ioctl(struct file *file_id, unsigned int cmd,
                          unsigned long arg );

/** IOCTL action handlers **/
static int player_setup(MDEC_SERVICE_T *mdec_svc, MDEC_SETUP_T *setup_cmd);
static int player_send_avdata(MDEC_SERVICE_T *mdec_svc,
                              MDEC_SEND_DATA_T *send_data_cmd,
                              media_stream_t stream_type);
static int player_send_video_data(MDEC_SERVICE_T *mdec_svc,
                             MDEC_SEND_DATA_T *send_data_cmd);
static int player_send_audio_data(MDEC_SERVICE_T *mdec_svc,
                             MDEC_SEND_DATA_T *send_data_cmd);
static int player_stop(MDEC_SERVICE_T *mdec_svc);
static int player_set_paused(MDEC_SERVICE_T *mdec_svc, int paused);
static int player_set_volume(MDEC_SERVICE_T *mdec_svc, MDEC_SET_VOLUME_T *data);
static int player_set_sync_offset(MDEC_SERVICE_T *mdec_svc, MDEC_SYNC_OFFSET_T *data);
static int player_set_fade(MDEC_SERVICE_T *mdec_svc, MDEC_SET_FADE_T *data);
static int player_get_fade(MDEC_SERVICE_T *mdec_svc, MDEC_GET_FADE_T *data);
static int player_set_muted(MDEC_SERVICE_T *mdec_svc, MDEC_SET_MUTED_T * data);
static int player_get_property(MDEC_SERVICE_T *mdec_svc,
                               MDEC_GET_PROPERTY_T *data);
static int player_set_src_region(MDEC_SERVICE_T *mdec_svc,
                                 MDEC_SET_SOURCE_REGION_T *data);
static int player_set_dest_region(MDEC_SERVICE_T *mdec_svc,
                                  MDEC_SET_DEST_REGION_T *data);
static int player_set_transparency(MDEC_SERVICE_T *mdec_svc,
                                   MDEC_SET_TRANSPARENCY_T *data);
static int player_get_frame(MDEC_SERVICE_T *mdec_svc, MDEC_GET_FRAME_T *data);
static int player_wait_for_callback(MDEC_SERVICE_T *mdec_svc,
                                    MDEC_WAIT_FOR_CALLBACK_T *data);
static int player_stop_callback_dispatch(MDEC_SERVICE_T *mdec_svc);

/** VCHIQ handler functions **/
static VCHIQ_STATUS_T service_callback(VCHIQ_REASON_T reason,
                                       VCHIQ_HEADER_T *header,
                                       VCHIQ_SERVICE_HANDLE_T service,
                                       void *userdata);
static int send_ctrl_message_nolock(MDEC_SERVICE_T *mdec_svc, unsigned int cmd,
                                    const void *data, size_t size,
                                    void *resp, size_t resp_size);
static int send_ctrl_message(MDEC_SERVICE_T *mdec_svc, unsigned int cmd,
                             const void *data, size_t size,
                             void *resp, size_t resp_size);
static int send_bulk_data_nolock(MDEC_SERVICE_T *mdec_svc,
                                 const void *buf,
                                 unsigned int len);
static int recv_bulk_data_nolock(MDEC_SERVICE_T *mdec_svc,
                                 void *buf,
                                 unsigned int len);
static int poll_wait_for_state_nolock(MDEC_SERVICE_T *mdec_svc, uint32_t target);

/****************************************************************************
	Private Data.  Declare as static.
 ****************************************************************************/

static VCHIQ_INSTANCE_T g_instance = NULL;   

/**
    File operations descriptor
**/
static const struct file_operations mdec_fops = {
   .owner          = THIS_MODULE,
   .open           = mdec_open,
   .release        = mdec_release,
   .unlocked_ioctl = mdec_ioctl
};

/**
    Miscellanous device descriptor
**/
struct miscdevice mdec_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &mdec_fops
};

/****************************************************************************
	Private Functions.  Declare as static.
 ****************************************************************************/

/****************************************************************************/
/**
	VCHIQ service callback.

	@param reason           The reason for the call
   @param header           Pointer to message header
   @param service          Service descriptor for this call
   @param userdata         As specified when the service was opened.

	@return VCHIQ_SUCCESS if the call was successul, else VCHIQ_ERROR.
**/
static VCHIQ_STATUS_T service_callback(VCHIQ_REASON_T reason,
                                       VCHIQ_HEADER_T *header,
                                       VCHIQ_SERVICE_HANDLE_T service,
                                       void *userdata)
{
   MDEC_SERVICE_T *mdec_svc = (MDEC_SERVICE_T *)VCHIQ_GET_SERVICE_USERDATA(service);
   MDEC_SERVER_CTRL_MESSAGE_T *msg;
   size_t tocopy = 0;
   VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

   switch (reason)
   {
   case VCHIQ_SERVICE_OPENED:
   case VCHIQ_SERVICE_CLOSED:
      /* do we expect these?  would they be called if we closed the service? */
      break;

   case VCHIQ_MESSAGE_AVAILABLE:
      msg = (MDEC_SERVER_CTRL_MESSAGE_T *)header->data;

      /* The minimal header size is two words.  Check if this the case.  If it
       *  isn't, log it and ignore this message.
       */
      if (header->size < 2*sizeof(uint32_t)) {
         printk(KERN_WARNING DEVICE_NAME ":: short VCHIQ control message\n");
         break;
      }

      /* Otherwise process each message type */
      switch (msg->type)
      {
      case MDEC_SERVER_MSG_RESPONSE:
         /* A response message comprises a response status, optionally followed
          *  by a response data payload.
          */

         /* ***** IMPORTANT NOTE *****
          *
          * There is no lock around this little bit of code, even though it
          *  accessed the shared data in mdec_svc.  The justification for this
          *  extreme naughtiness is that on a per-service basis we should _only_
          *  be getting response messages in reply to command messages.  Since
          *  the code that sends command messages sits waiting for the response
          *  and already has the lock, no other agent can send command messages
          *  to ellicit response messages.
          *
          * At least, that's the theory....
          *
          */

         mdec_svc->response_status = msg->response;

         /* If the client has provided a buffer then work out how much to copy,
          *  which may be nothing if there is no payload.
          */
         if (NULL != mdec_svc->response && mdec_svc->response_size) {
            if (header->size > offsetof(MDEC_SERVER_CTRL_MESSAGE_T, u.buf))
               tocopy = header->size - offsetof(MDEC_SERVER_CTRL_MESSAGE_T, u.buf);
            tocopy = MIN(tocopy, mdec_svc->response_size);
         }

         /* Then do the copy itself.  Note that if tocopy is zero when there
          *  is nothing to copy, and memcpy() does nothing.
          */
         memcpy(mdec_svc->response, msg->u.buf, tocopy);

         /* Now we set the response pointer to NULL.  This in effect locks
          *  out any other response copying until such time as another call
          *  expecting a response sets it up for us.
          */
         mdec_svc->response = NULL;

         /* Tell the client the size of response they got (may be 0) */
         mdec_svc->response_size = tocopy;

         /* Finally, flag that a new message is available */
         up(&mdec_svc->sem_msg);
         break;

      case MDEC_SERVER_MSG_CALLBACK:
         if (down_timeout(&mdec_svc->cb.sem_lock, MDEC_SEMAPHORE_TIMEOUT_J)) {
            printk(KERN_WARNING DEVICE_NAME ":: timeout in VCHIQ callback\n" );
         } else {
            /* Extract the first field, and also keep a local copy for later */
            uint32_t reason = *(uint32_t *)&msg->u.buf[0];

            /* Indicate to anyone waiting for a buffer */
            if (reason == MDEC_CALLBACK_REASON_AUDIO_OWNERSHIP_RETURN)
               set_bit(AUDIO_BUFFER_AVAILABLE, &mdec_svc->bitflags);
            else if (reason == MDEC_CALLBACK_REASON_VIDEO_OWNERSHIP_RETURN)
               set_bit(VIDEO_BUFFER_AVAILABLE, &mdec_svc->bitflags);
            else do { /*------------------ BEGIN CRITICAL REGION ---------------------*/
				/* Manually unpack struct fields to account for differences 
				 * between VideoCore and ARM.
				 */
               mdec_svc->cb.fifo[mdec_svc->cb.head].mReason = reason;
									 
				/* The next field is the 64-bitter.  Unpack each half as if we try
				 * to access it directly from the ARM we'll get address violations
				 * when trying to read a 64-bit value at a 32-bit aligned address.
				 */
				mdec_svc->cb.fifo[mdec_svc->cb.head].mData = (uint64_t)(*(uint32_t *)&msg->u.buf[4]) 
				                                             | ((uint64_t)(*(uint32_t *)&msg->u.buf[8]) << 32);

				/* Finally, the last field, 32-bit again so we can read it from
             * memory directly.
				 */
				mdec_svc->cb.fifo[mdec_svc->cb.head].mContext = *(uint32_t *)&msg->u.buf[12];
				
            mdec_svc->cb.head = (mdec_svc->cb.head+1) % MDEC_USER_CALLBACK_FIFO_DEPTH;
            up(&mdec_svc->cb.sem_user_callback);
				
         } while (0); /*----------- END CRITICAL REGION ----------------------*/
         up(&mdec_svc->cb.sem_lock);
         }
         break;

      default:
         printk(KERN_WARNING DEVICE_NAME ":: unknown ctrl message in VCHIQ callback\n" );
         break;
      }

      vchiq_release_message(service, header);
      break;

   case VCHIQ_BULK_TRANSMIT_DONE:
      mdec_svc->response_status = MDEC_RESPONSE_ACK;
      up(&mdec_svc->sem_msg);
      break;

   case VCHIQ_BULK_RECEIVE_DONE:
      //mdec_svc->response_status = MDEC_RESPONSE_ACK;
      //up(&mdec_svc->sem_msg);
		mdec_svc->bulk_rx_status = MDEC_RESPONSE_ACK;
		up(&mdec_svc->sem_bulk_rx);
      break;

   case VCHIQ_BULK_TRANSMIT_ABORTED:
      mdec_svc->response_status = MDEC_RESPONSE_NAK;
      up(&mdec_svc->sem_msg);
      break;

   case VCHIQ_BULK_RECEIVE_ABORTED:
      mdec_svc->bulk_rx_status = MDEC_RESPONSE_NAK;
		up(&mdec_svc->sem_bulk_rx);
      break;

   default:
      mdec_svc->response_status = MDEC_RESPONSE_NAK;
      up(&mdec_svc->sem_msg);
      break;
   }

   return status;
}

/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*                           Helper functions                               */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/**
	Send a CTRL message to the MDEC server and await the response.

	** This version does not lock the service control block **

	@param mdec_svc         Connection descriptor
   @param cmd              Command code
   @param data             Pointer to data block.  NULL if no data block.
   @param size             Size of data block
   @param resp             Pointer to reponse buffer.  May be NULL.
   @param resp_size        Size of response buffer.  Ignored if resp is NULL.

	@retval 0 if successful
   @retval -ETIME if we didn't get a response in a reasonable time
   @retval -EFAULT if something bad went wrong
   @retval -EIO if comms problem
**/
static int send_ctrl_message_nolock(MDEC_SERVICE_T *mdec_svc, unsigned int cmd,
                                    const void *data, size_t size,
                                    void *resp, size_t resp_size)
{
   VCHIQ_STATUS_T vcstatus;
   int ret;
   VCHIQ_ELEMENT_T element[2]; /* We can have at most 2 elements */
   int n_elems = 0;

   /* Save the optional response buffer parameters ready for the response,
    *  if any, to be stored by the callback.
    */
   mdec_svc->response      = resp;
   mdec_svc->response_size = resp_size;

   /* Construct the message, consisting of the command code, and optionally
    *  any data.
    */
   element[n_elems].data = &cmd;
   element[n_elems].size = sizeof(cmd);
   ++n_elems;

   if (data) {
      element[n_elems].data = data;
      element[n_elems].size = size;
      ++n_elems;
   }

   /* Send the message.  If this fails, we return immediately to the caller
    *  with an EFAULT.
    */
   vcstatus = vchiq_queue_message(mdec_svc->service, element, n_elems);
   if (VCHIQ_SUCCESS != vcstatus)
      return -EFAULT;

   /* Now we wait for the response.  If one doesn't turn up after some time
    *  we abandon waiting and return with ETIMEDOUT.
    * The callback handler is responsible for storing the response data into
    *  the recipient's buffers.
    */
   ret = down_timeout(&mdec_svc->sem_msg, MDEC_SEMAPHORE_TIMEOUT_J);
   if (ret) {
      printk(KERN_ERR "%s mdec-timeout cmd %08x ret %d\n", __func__, cmd, ret);
      return ret;
   }

   return (mdec_svc->response_status == MDEC_RESPONSE_ACK ? 0 : -EFAULT);
}

/****************************************************************************/
/**
	Send a CTRL message to the MDEC server and await the response.

	@param mdec_svc         Connection descriptor
   @param cmd              Command code
   @param data             Pointer to data block.  NULL if no data block.
   @param size             Size of data block
   @param resp             Pointer to reponse buffer.  May be NULL.
   @param resp_size        Size of response buffer.  Ignored if resp is NULL.

	@retval 0 if successful
   @retval -ETIMEDOUT if we didn't get a response in a reasonable time
   @retval -EFAULT if something bad went wrong
   @retval -EIO if comms problem
**/
static int send_ctrl_message(MDEC_SERVICE_T *mdec_svc, unsigned int cmd,
                             const void *data, size_t size,
                             void *resp, size_t resp_size)
{
   int ret;

   if (down_timeout(&mdec_svc->sem_lock, MDEC_SEMAPHORE_TIMEOUT_J)) {
      printk(KERN_ERR "%s mdec-timeout cmd %08x\n", __func__, cmd);
      return -ETIMEDOUT;
   }

   ret = send_ctrl_message_nolock(mdec_svc, cmd, data, size, resp, resp_size);
   up(&mdec_svc->sem_lock);
   return ret;
}

/****************************************************************************/
/**
	Send a block of data to the VC using BULK transfer.

    ** This version does not lock the service control block **

	@param mdec_svc         Media Dec service descriptor
   @param buf              Pointer to a buffer of data to send
   @param len              Number of bytes from buf to send

   @retval 0 if successful
   @retval -EINVAL if any parameters are bad
   @retval -EIO if comms failed
   @retval -EFAULT if something went wrong on the server
**/
static int send_bulk_data_nolock(MDEC_SERVICE_T *mdec_svc,
                                 const void     *buf,
                                 unsigned int    len)
{
   VCHIQ_STATUS_T vcstatus;
   int ret;

   vcstatus = vchiq_queue_bulk_transmit(mdec_svc->service, buf, len, NULL );
   if (VCHIQ_SUCCESS != vcstatus)
      return -EIO;

   ret = down_timeout(&mdec_svc->sem_msg, MDEC_SEMAPHORE_TIMEOUT_J);
   if (ret)
      return -ETIMEDOUT;

   return (mdec_svc->response_status == MDEC_RESPONSE_ACK ? 0 : -EFAULT);
}

/****************************************************************************/
/**
	Get a block of data from the VC using BULK transfer.

	** This version does not lock the service control block **

	@param mdec_svc         Media Dec service descriptor
   @param buf              Pointer to a buffer to store the data
   @param len              Size of the buffer in bytes.

   @retval 0 if successful
   @retval -EINVAL if any parameters are bad
   @retval -EIO if comms failed
   @retval -EFAULT if something went wrong on the server
**/
static int recv_bulk_data_nolock(MDEC_SERVICE_T *mdec_svc,
                                 void           *buf,
                                 unsigned int    len)
{
   VCHIQ_STATUS_T vcstatus;
   int ret;
	
	msleep(20);

   vcstatus = vchiq_queue_bulk_receive(mdec_svc->service, buf, len, NULL);
   if (VCHIQ_SUCCESS != vcstatus)
      return -EIO;

   ret = down_timeout(&mdec_svc->sem_bulk_rx, MDEC_SEMAPHORE_TIMEOUT_J);
   if (ret)
      return -ETIMEDOUT;

   return (mdec_svc->bulk_rx_status == MDEC_RESPONSE_ACK ? 0 : -EFAULT);
}

/****************************************************************************/
/**
	Poll the media dec server until either a specified state is reached OR
    we time out.

	** This version does not lock the service control block **

	@param mdec_svc         Media Dec service descriptor
   @param state            Required state

   @retval 0 if successful
   @retval -EINVAL if any parameters are bad
   @retval -EIO if comms failed
   @retval -ETIMEDOUT if the operation timed out
**/
static int poll_wait_for_state_nolock(MDEC_SERVICE_T *mdec_svc, uint32_t target)
{
   uint32_t mdec_state;
   int ret = 0;
   int countdown = MDEC_PLAYBACK_TIME_SAMPLES;

   do {
      ret = send_ctrl_message_nolock(mdec_svc, MDEC_CMD_GET_STATE,
                                     NULL, 0,
                                     &mdec_state, sizeof(mdec_state));

      /* If an error occured then abort immediately. */
      if (ret)
         break;

      /* If we are in the desired target state then finish. */
      if (mdec_state == target)
         break;
			
		/* Otherwise wait a little bit and go again. */
      schedule_timeout(MDEC_SCHEDULE_TIMEOUT_MS);
   } while (--countdown);

   if (!countdown)
      ret = -ETIMEDOUT;

   return ret;
}

/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*                           Media Dec actions                              */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/**
	Set up the player to default state.

   @param mdec_svc         Media Dec service descriptor
	@param setup_cmd        Pointer to setup command block.

	@return 0 if successful, else -EFAULT.
**/
static int player_setup(MDEC_SERVICE_T *mdec_svc, MDEC_SETUP_T *setup_cmd)
{
   /* Set up the src width as 0xFFFFFFFF (ignore) */
   MDEC_SET_SOURCE_REGION_T region = {
      .x      = -1,
      .y      = -1,
      .width  = -1,
      .height = -1
   };

   int result = send_ctrl_message(mdec_svc, MDEC_CMD_SETUP,
                                  setup_cmd, sizeof(*setup_cmd),
                                  NULL, 0);
   if (result)
      return result;

   return player_set_src_region(mdec_svc, &region);
}

/****************************************************************************/
/**
	Send a block of A/V data to the VC using bulk transfer.

   Data is sent in chunks, whose size is notified by the VideoCore side.
   A header is sent over the CTRL channel, followed by one or more data chunks
   over the BULK channel.  Since we maintain the channel lock during the
   transfer this ensures that no other thread can intermingle data chunks ON
   THIS CHANNEL.

   If it all goes pear-shaped we abort the transfer and release the lock.  In
   this case its probably best to close down the entire channel and start again
   as its possible the VideoCore has got its knickers in a twist.

	@param mdec_svc         Media Dec service descriptor
   @param send_data_cmd    Command data description
   @param stream_type      Specifies the media stream type for this transfer

   @retval 0 if successful
   @retval -EINVAL if any parameters are bad
   @retval -ETIMEDOUT if the operation failed on a timeout
   @retval -EIO if comms failed
**/
static int player_send_avdata(MDEC_SERVICE_T   *mdec_svc,
                              MDEC_SEND_DATA_T *send_data_cmd,
                              media_stream_t    stream_type)
{
   int ret = 0;
	uint32_t blksize;
   uint8_t * pdata = (uint8_t *)send_data_cmd->data_buf;
   unsigned int cmd = stream_type == AUDIO_STREAM ? MDEC_CMD_SEND_AUDIO_DATA
                                                  : MDEC_CMD_SEND_VIDEO_DATA;
																  
   /* Successfully ignore empty transfers, where "empty" includes the flags. */                                               
   if (0 == send_data_cmd->data_size && 0 == send_data_cmd->flags)
      return 0;

   do { 
	   if (down_timeout(&mdec_svc->sem_lock, MDEC_SEMAPHORE_TIMEOUT_J)) {
         printk(KERN_ERR "%s mdec-timeout\n", __func__);
         return -ETIMEDOUT;
       }
			
      do { /*------------------ BEGIN CRITICAL REGION ------------------------*/
         ret = send_ctrl_message_nolock(mdec_svc, cmd,
                                        send_data_cmd, sizeof(*send_data_cmd),
                                        &blksize, sizeof(blksize));

         if (ret || NO_FREE_BUFFERS(blksize))
            break;

         if (blksize) {
            ret = send_bulk_data_nolock(mdec_svc, pdata, blksize);
            if (ret)
               break;

            pdata += blksize;
            send_data_cmd->data_size -= blksize;
         }
	   } while (0); /*------------- END CRITICAL REGION -----------------------*/
      up(&mdec_svc->sem_lock);
		
		if (ret)
			break;
		
		/* If there were no free buffers we pause until notified that a buffer
		 *  of the appropriate type is available.
		 */
		if (NO_FREE_BUFFERS(blksize)) {
			int nr = stream_type == AUDIO_STREAM ? AUDIO_BUFFER_AVAILABLE
                                              : VIDEO_BUFFER_AVAILABLE;
			
			while (!test_and_clear_bit(nr, &mdec_svc->bitflags))
			    if (msleep_interruptible(1))
					return -EINTR;
			continue;
		}
			
		/* After sending the first chunk of data to the server we clear out a
		 * number of client flags, and set the TIME_UNKNOWN flag as we don't
		 * know the timing of trailing chunks.
		 */
		send_data_cmd->flags &= ~(MEDIA_DEC_FLAGS_STARTTIME 
										| MEDIA_DEC_FLAGS_DISCONTINUITY 
								| MEDIA_DEC_FLAGS_CALLBACK);
		send_data_cmd->flags |= MEDIA_DEC_FLAGS_TIME_UNKNOWN;

      /* Do not make this a condition of the while loop or it will affect
       * continue. It needs to be able to wrap around in cases where data_size
       * is 0 but other conditions are satisfied (like when attempting to
       * obtain a buffer for END_OF_STREAM).
       */
      if (send_data_cmd->data_size == 0) break;

   } while (true);
		
   return ret;
}

/****************************************************************************/
/**
	Send video data to the VideoCore

	@param mdec_svc            Media Dec service descriptor
	@param send_data_cmd       Command from the user

	@return See player_send_avdata().
**/
static int player_send_video_data(MDEC_SERVICE_T   *mdec_svc,
                                  MDEC_SEND_DATA_T *send_data_cmd)
{
   return player_send_avdata(mdec_svc, send_data_cmd, VIDEO_STREAM);
}

/****************************************************************************/
/**
	Send audio data to the VideoCore

	@param mdec_svc            Media Dec service descriptor
	@param send_data_cmd       Command from the user

	@return See player_send_avdata().
**/
static int player_send_audio_data(MDEC_SERVICE_T   *mdec_svc,
                                  MDEC_SEND_DATA_T *send_data_cmd)
{
   return player_send_avdata(mdec_svc, send_data_cmd, AUDIO_STREAM);
}

/****************************************************************************/
/**
	Stop playback.

    We need to allow time for media dec to stop playing.  But eventually we must
    timeout.  The timeout is specified in whole seconds, although we actually
    sample the state at 10x this rate.

	@param mdec_svc     Media Dec service descriptor

	@return As returned by send_ctrl_message()
**/
static int player_stop(MDEC_SERVICE_T *mdec_svc)
{
   int ret = 0;

   if (down_timeout(&mdec_svc->sem_lock, MDEC_SEMAPHORE_TIMEOUT_J)) {
      printk(KERN_ERR "%s mdec-timeout\n", __func__);
      return -ETIMEDOUT;
   }

   do { /*------------------ BEGIN CRITICAL REGION ---------------------------*/
      /* Send the STOP command.  Abort if anything went wrong rather than carry
       *  on.  Let the client decide how to deal with this.
       */
      ret = send_ctrl_message_nolock(mdec_svc, MDEC_CMD_STOP, NULL, 0, NULL, 0);
      if (ret)
         break;

      ret = poll_wait_for_state_nolock(mdec_svc, MDEC_STATE_IDLE);
   } while (0); /*------------- END CRITICAL REGION --------------------------*/

   up(&mdec_svc->sem_lock);
	
   return ret;
}

/****************************************************************************/
/**
	Pause or unpause playback.

	@param mdec_svc     Media Dec service descriptor
	@param paused       1 to pause, 0 to resume play

	@retval -EALREADY if pausing when paused, or resuming when resumed
   @retval -EIO      if media dec not enabled
   @retval otherwise returned by send_ctrl_message()
**/
static int player_set_paused(MDEC_SERVICE_T *mdec_svc, int paused)
{
   return send_ctrl_message(mdec_svc,
                            paused ? MDEC_CMD_PAUSE : MDEC_CMD_PLAY,
                            NULL, 0, NULL, 0);
}

/****************************************************************************/
/**
	Set the volume for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to volume config data

	@return As returned by send_ctrl_message().
**/
static int player_set_volume(MDEC_SERVICE_T *mdec_svc, MDEC_SET_VOLUME_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_VOLUME,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Set the clock for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to clock config data

	@return As returned by send_ctrl_message().
**/
static int player_set_clock(MDEC_SERVICE_T *mdec_svc, MDEC_SET_CLOCK_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_CLOCK,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Step the video one frame

   @param mdec_svc     Media Dec service descriptor

	@return As returned by send_ctrl_message().
**/
static int player_step(MDEC_SERVICE_T *mdec_svc)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_STEP,
                            NULL, 0, NULL, 0);
}

/****************************************************************************/
/**
	Clear one frame

   @param mdec_svc     Media Dec service descriptor

	@return As returned by send_ctrl_message().
**/
static int player_clear(MDEC_SERVICE_T *mdec_svc)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_CLEAR,
                            NULL, 0, NULL, 0);
}

/****************************************************************************/
/**
 *         Set AV sync offset
 *
 *            @param mdec_svc     Media Dec service descriptor
 *                    @param data         Pointer to config data
 *
 *                            @return As returned by send_ctrl_message().
 *                            **/
static int player_set_sync_offset(MDEC_SERVICE_T *mdec_svc, MDEC_SYNC_OFFSET_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_SYNC_OFFSET,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Set the fade for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to config data

	@return As returned by send_ctrl_message().
**/
static int player_set_fade(MDEC_SERVICE_T *mdec_svc, MDEC_SET_FADE_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_FADE,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Get the fade for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to config data

	@return As returned by send_ctrl_message().
**/
static int player_get_fade(MDEC_SERVICE_T *mdec_svc, MDEC_GET_FADE_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_GET_FADE,
                            data, sizeof(*data), data, sizeof(*data));
}

/****************************************************************************/
/**
	Set the volume muted state for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to mute config data

	@return As returned by send_ctrl_message().
**/
static int player_set_muted(MDEC_SERVICE_T *mdec_svc, MDEC_SET_MUTED_T * data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_MUTED,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Query the Media Dec server on the VideoCore

	@param mdec_svc     Media Dec service data
	@param data         Pointer to get_property data buffer

	@return As returned by send_ctrl_message().
**/
static int player_get_property(MDEC_SERVICE_T *mdec_svc,
                               MDEC_GET_PROPERTY_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_GET_PROPERTY,
                            data, sizeof(*data),
                            data, sizeof(*data));
}

/****************************************************************************/
/**
	Set the source region for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to source region config data

	@return As returned by send_ctrl_message().
**/
static int player_set_src_region(MDEC_SERVICE_T *mdec_svc,
                                 MDEC_SET_SOURCE_REGION_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_SOURCE_REGION,
                            data, sizeof(*data),
                            NULL, 0);
}

/****************************************************************************/
/**
	Set the destination region for this connection.

   @param mdec_svc     Media Dec service descriptor
	@param data         Pointer to destination region config data

	@return As returned by send_ctrl_message().
**/
static int player_set_dest_region(MDEC_SERVICE_T *mdec_svc,
                                  MDEC_SET_DEST_REGION_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_DEST_REGION,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Set the transparency alpha level for this connection.

   @param mdec_svc     Media Dec service data
	@param data         Pointer to transparency config data

	@return As returned by send_ctrl_message().
**/
static int player_set_transparency(MDEC_SERVICE_T *mdec_svc,
                                   MDEC_SET_TRANSPARENCY_T *data)
{
   return send_ctrl_message(mdec_svc, MDEC_CMD_SET_TRANSPARENCY,
                            data, sizeof(*data), NULL, 0);
}

/****************************************************************************/
/**
	Get a frame of data from the currently playing stream.

	@param mdec_svc     Media Dec service descriptor
	@param data         Pointer to frame grab data

	@return As returned by send_ctrl_message().
**/
static int player_get_frame(MDEC_SERVICE_T *mdec_svc, MDEC_GET_FRAME_T *data)
{
   int ret = 0;
	uint32_t blksize;

   /* Check the input parameters make sense */
   if (data->height * data->stride < data->bytes
       || data->stride < data->width * 2)
      return -EINVAL;

   /* Check that we have write access and enough space in the user-space buffer
    *  for this call.
    */
   if (!access_ok(VERIFY_WRITE, data->buffer, data->bytes))
      return -EFAULT;
		
	if (down_timeout(&mdec_svc->sem_frame_lock, MDEC_SEMAPHORE_TIMEOUT_J))
	   return -ETIMEDOUT;

   if (down_timeout(&mdec_svc->sem_lock, MDEC_SEMAPHORE_TIMEOUT_J)) {
	   up(&mdec_svc->sem_frame_lock);
      return -ETIMEDOUT;
	}

   do { /*------------------ BEGIN CRITICAL REGION ---------------------------*/
      ret = send_ctrl_message_nolock(mdec_svc, MDEC_CMD_GET_FRAME,
                                     data, sizeof(*data),
                                     &blksize, sizeof(blksize));
      //if (ret)
      //   break;

      //ret = recv_bulk_data_nolock(mdec_svc, data->buffer, data->bytes);
   } while (0); /*------------- END CRITICAL REGION --------------------------*/

   up(&mdec_svc->sem_lock);
	
	if (!ret)
	   ret = recv_bulk_data_nolock(mdec_svc, data->buffer, blksize);
	
	up(&mdec_svc->sem_frame_lock);
	
   return ret;
}

/****************************************************************************/
/**
	Block waiting for some event to take place.

	@param mdec_svc     Media Dec service descriptor
	@param data         Pointer to callback data

	@return 0 if successful, -EINTR if interrupted, -ETIMEDOUT if lock failed.
**/
static int player_wait_for_callback(MDEC_SERVICE_T *mdec_svc,
                                    MDEC_WAIT_FOR_CALLBACK_T *data)
{
   int ret = 0;

   if (down_interruptible(&mdec_svc->cb.sem_user_callback))
      return -EINTR;

   if (down_timeout(&mdec_svc->cb.sem_lock,
                    msecs_to_jiffies(MDEC_SEMAPHORE_TIMEOUT_MS))) {
		/* Couldn't get lock in time. Restore CB count and bail out. */
		up(&mdec_svc->cb.sem_user_callback);	
      return -ETIMEDOUT;
	}

   do { /*------------------ BEGIN CRITICAL REGION ---------------------------*/
		*data = mdec_svc->cb.fifo[mdec_svc->cb.tail];
      mdec_svc->cb.tail = (mdec_svc->cb.tail+1) % MDEC_USER_CALLBACK_FIFO_DEPTH;
   } while (0); /*------------- END CRITICAL REGION --------------------------*/

   up(&mdec_svc->cb.sem_lock);
   return ret;
}

/****************************************************************************/
/**
	Kick the callback semaphore so the userspace dispatch thread can wake up and
    terminate.

	@param mdec_svc     Media Dec service descriptor

   @retval -ETIMEDOUT if we couldn't access the service in time.
	@retval 0 if successful.
**/
static int player_stop_callback_dispatch(MDEC_SERVICE_T *mdec_svc)
{
   int ret = 0;

   MDEC_WAIT_FOR_CALLBACK_T dummy = { MDEC_CALLBACK_REASON_USER_STOPPED, 0, 0 };

   if (down_timeout(&mdec_svc->cb.sem_lock, MDEC_SEMAPHORE_TIMEOUT_J))
      return -ETIMEDOUT;

   do { /*------------------ BEGIN CRITICAL REGION ---------------------------*/
      mdec_svc->cb.fifo[mdec_svc->cb.head] = dummy;
      mdec_svc->cb.head = (mdec_svc->cb.head+1) % MDEC_USER_CALLBACK_FIFO_DEPTH;
      up(&mdec_svc->cb.sem_user_callback);
   } while (0); /*------------- END CRITICAL REGION --------------------------*/

   up(&mdec_svc->cb.sem_lock);
   return ret;
}

/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*                       Inode File Operations                              */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/**
	OPEN handler

	@param 	inode           Pointer to inode
    @param  file            Pointer to file descriptor

	@return 	0 if success, else negative error code
**/
static int mdec_open(struct inode *inode, struct file *file)
{
   MDEC_SERVICE_T *mdec_svc;
   VCHIQ_SERVICE_PARAMS_T params;
	
	if (!g_instance) {
		/* Initialise an instance of the VCHIQ and connect to it */
		if (vchiq_initialise(&g_instance) != VCHIQ_SUCCESS)
			return -EFAULT;

		/* Try to connect to the other end. */
		if (vchiq_connect(g_instance) != VCHIQ_SUCCESS) {
			vchiq_shutdown(g_instance);
			g_instance = NULL;
			return -EFAULT;
		}
	}

   /* Create a context for this service, tag it on to the file, and
    *  do any setups (semaphores, etc).
    */
   mdec_svc = kzalloc(sizeof(MDEC_SERVICE_T), GFP_KERNEL);
   if (mdec_svc == NULL) {
      return -ENOMEM;
   }
   file->private_data = (void *)mdec_svc;
   sema_init(&mdec_svc->sem_msg, 0);
   sema_init(&mdec_svc->sem_lock, 0);
   sema_init(&mdec_svc->cb.sem_user_callback, 0);
   sema_init(&mdec_svc->cb.sem_lock, 0);
	sema_init(&mdec_svc->sem_frame_lock, 1);
	sema_init(&mdec_svc->sem_bulk_rx, 0);
	

   /* Open a channel to the MDEC service on the VideoCore */
   memset(&params, 0, sizeof(params));
   params.fourcc   = MDEC_FOURCC;
   params.callback = service_callback;
   params.userdata = (void *)mdec_svc;
   params.version = VC_MDEC_VERSION;
   params.version_min = VC_MDEC_VERSION_MIN;

   if (vchiq_open_service(g_instance, &params, &mdec_svc->service) != VCHIQ_SUCCESS){
	   printk(KERN_ERR DEVICE_NAME ":: failed to open MDEC vchiq service\n");
      kfree(mdec_svc);
      return -EFAULT;
   }

   /* Tell VCHIQ that this service is now in use */
   vchiq_use_service(mdec_svc->service);

   /* Unlock the data structures ready for use */
   up(&mdec_svc->sem_lock);
   up(&mdec_svc->cb.sem_lock);

   return 0;
}

/****************************************************************************/
/**
	RELEASE (close) handler

	@param 	inode           Pointer to inode
    @param  file            Pointer to file descriptor

	@return 	0 if success, else negative error code
**/
static int mdec_release(struct inode *inode, struct file *file)
{
   MDEC_SERVICE_T *mdec_svc = (MDEC_SERVICE_T *)(file->private_data);

   player_stop(mdec_svc);

   vchiq_release_service(mdec_svc->service);
   vchiq_remove_service(mdec_svc->service);
   kfree(mdec_svc);

   return 0;
}

/****************************************************************************/
/**
	IOCTL handler.

    The main dispatch for ioctl calls coming in to the driver.
    WARNING: there are multiple threads that could be calling this function,
             so it MUST BE REENTRANT!

   @param  file        Pointer to file descriptor
   @param  cmd         The IOCTL command
   @param  arg         IOCTL argument (pointer or number)

	@return 	0 if successful, else an error code.
**/
static long mdec_ioctl( struct file *file,
                       unsigned int cmd, unsigned long arg )
{
   MDEC_SERVICE_T *mdec_svc = (MDEC_SERVICE_T *)(file->private_data);
   int ret = 0;
   unsigned char ioctl_cmd_buf[MAX_MDEC_IOCTL_CMD_SIZE];

   /* Copy command buffer in if it's necessary */
   if (((_IOC_DIR(cmd) & _IOC_WRITE) != 0) && (_IOC_SIZE(cmd) > 0)) {
      /* Make sure we don't overrun the cmd buffer! */
      if (_IOC_SIZE(cmd) > sizeof(ioctl_cmd_buf))
         return -EINVAL;
      if (copy_from_user(ioctl_cmd_buf, (void *)arg, _IOC_SIZE(cmd)) != 0)
         return -EFAULT;
   }

   switch (cmd)
   {
   case MDEC_IOCTL_PLAYER_SETUP:
      ret = player_setup(mdec_svc, (MDEC_SETUP_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SEND_VIDEO_DATA:
      ret = player_send_video_data(mdec_svc, (MDEC_SEND_DATA_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SEND_AUDIO_DATA:
      ret = player_send_audio_data(mdec_svc, (MDEC_SEND_DATA_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_STOP:
      ret = player_stop(mdec_svc);
      break;

   case MDEC_IOCTL_PLAYER_PAUSE:
      ret = player_set_paused(mdec_svc, -1);
      break;

   case MDEC_IOCTL_PLAYER_RESUME:
      ret = player_set_paused(mdec_svc, 0);
      break;

   case MDEC_IOCTL_PLAYER_SET_VOLUME:
      ret = player_set_volume(mdec_svc, (MDEC_SET_VOLUME_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_CLOCK:
      ret = player_set_clock(mdec_svc, (MDEC_SET_CLOCK_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_STEP:
      ret = player_step(mdec_svc);
      break;

   case MDEC_IOCTL_PLAYER_CLEAR:
      ret = player_clear(mdec_svc);
      break;

   case MDEC_IOCTL_PLAYER_SET_SYNC_OFFSET:
      ret = player_set_sync_offset(mdec_svc, (MDEC_SYNC_OFFSET_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_FADE:
      ret = player_set_fade(mdec_svc, (MDEC_SET_FADE_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_GET_FADE:
      ret = player_get_fade(mdec_svc, (MDEC_GET_FADE_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_MUTED:
      ret = player_set_muted(mdec_svc, (MDEC_SET_MUTED_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_GET_PROPERTY:
      ret = player_get_property(mdec_svc, (MDEC_GET_PROPERTY_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_SOURCE_REGION:
      ret = player_set_src_region(mdec_svc, (MDEC_SET_SOURCE_REGION_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_DEST_REGION:
      ret = player_set_dest_region(mdec_svc, (MDEC_SET_DEST_REGION_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_SET_TRANSPARENCY:
      ret = player_set_transparency(mdec_svc, (MDEC_SET_TRANSPARENCY_T *)ioctl_cmd_buf);
      break;

   case MDEC_IOCTL_PLAYER_GET_FRAME:
      ret = player_get_frame(mdec_svc, (MDEC_GET_FRAME_T *)ioctl_cmd_buf);
      break;

   /* Note: this will be coming in from a separate thread in libmedia_dec that will then
      dispatch callbacks back to the user level. Hence why blocking here is appropriate
      (and safe). */
   case MDEC_IOCTL_PLAYER_WAIT_FOR_CALLBACK:
      ret = player_wait_for_callback(mdec_svc, (MDEC_WAIT_FOR_CALLBACK_T *)ioctl_cmd_buf);
      break;

   /* This call should be coming in from the main thread, to request that we release
      any blocking ioctl call on a separate thread for WAIT_FOR_CALLBACK */
   case MDEC_IOCTL_PLAYER_STOP_CALLBACK_DISPATCH:
      ret = player_stop_callback_dispatch(mdec_svc);
      break;

   default:
      printk(KERN_ERR DEVICE_NAME ":: wrong IOCTL cmd\n");
      ret = -EFAULT;
      break;
   }

   if (ret)
      return ret;

   if (((_IOC_DIR(cmd) & _IOC_READ) != 0) && (_IOC_SIZE(cmd) > 0)) {
      /* Make sure we only copy out the maximum allowed, and no more */
      unsigned long n = MIN(_IOC_SIZE(cmd), sizeof(ioctl_cmd_buf));
      if (copy_to_user((void *)arg, ioctl_cmd_buf, n) != 0)
         return -EFAULT;
   }

   return ret;
}

/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*                         Driver init/exit                                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/**
	Initialise the MDEC driver.
**/
static int __init mdec_init(void)
{
   int err = 0;

   err = misc_register(&mdec_misc_dev);
   if (err != 0)
      goto failed_reg_miscdev;

   printk(KERN_INFO DEVICE_NAME ":: driver initialised.\n");
   return 0;

   /* Error handlers and exit point */

failed_reg_miscdev:
   printk(KERN_ALERT DEVICE_NAME ":: could not load driver\n");
   return err;
}

/****************************************************************************/
/**
	Close down the MDEC driver.
**/
static void __exit mdec_exit(void)
{
   misc_deregister(&mdec_misc_dev);
   vchiq_shutdown(g_instance);
	g_instance = NULL;
}

/****************************************************************************/
/* Epilogue */
/****************************************************************************/

module_init(mdec_init);
module_exit(mdec_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("media_dec client driver");
MODULE_VERSION("dev");

/* ========================================================================== */
/* ========================================================================== */
/* ========================================================================== */
