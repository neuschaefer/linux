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

#include <linux/interrupt.h>
#include <linux/slab.h>
#include "vmcs_alsa.h"
#include <vc_vchi_audioserv.h>

/* hardware definition */
static struct snd_pcm_hardware vc_alsa_playback_hw = {
   .info = (SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_BLOCK_TRANSFER),
   .formats = SNDRV_PCM_FMTBIT_U8 | SNDRV_PCM_FMTBIT_S16_LE,
   .rates = SNDRV_PCM_RATE_CONTINUOUS | SNDRV_PCM_RATE_8000_48000,
   .rate_min = 8000,
   .rate_max = 48000,
   .channels_min = 1,
   .channels_max = 2,
   .buffer_bytes_max = (4 * 8 - 1) * 1024, //less than audioplay buffer size
   .period_bytes_min = 8 * 1024,
   .period_bytes_max = (4 * 8 - 1) * 1024,
   .periods_min = 1,
   .periods_max = 4 * 8 - 1,
};

static void vc_alsa_playback_free(struct snd_pcm_runtime *runtime)
{
   LOG_DBG("stream %d ..IN", ((vc_alsa_stream_t*)runtime->private_data)->idx);
   if (runtime->private_data)
      kfree(runtime->private_data);
   runtime->private_data = NULL;
}

static irqreturn_t vc_alsa_playback_fifo_irq(int irq, void *dev_id)
{
   vc_alsa_stream_t *alsa_stream = (vc_alsa_stream_t *) dev_id;
   uint32_t consumed = 0;
   int new_period = 0;

   LOG_DBG("stream %d %p ..IN", alsa_stream->idx, alsa_stream);

   if (alsa_stream->open)
      consumed = vc_vchiq_alsa_retrieve_buffers(alsa_stream);

   /* 
    * We get called only if playback was triggered, So, the number of buffers
    * we retrieve in each iteration are the buffers that have been played out
    * already
    */

   if (alsa_stream->period_size)
   {
      if ((alsa_stream->pos / alsa_stream->period_size) !=
                     ((alsa_stream->pos + consumed) / alsa_stream->period_size))
         new_period = 1;
   }
   LOG_DBG("stream %d ..updating pos cur: %d + %d max:%d new_period:%d", 
           alsa_stream->idx, alsa_stream->pos,
           (consumed /** AUDIO_IPC_BLOCK_BUFFER_SIZE*/ ),
           alsa_stream->buffer_size, new_period);
   if (alsa_stream->buffer_size)
   {
      alsa_stream->pos += consumed;
      alsa_stream->pos %= alsa_stream->buffer_size;
   }
   if (alsa_stream->substream)
   {
      if (new_period)
         snd_pcm_period_elapsed(alsa_stream->substream);
   }
   else
      LOG_ERR(" unexpected NULL substream");

   LOG_DBG("stream %d ..OUT", alsa_stream->idx);

   return IRQ_HANDLED;
}

/* open callback */
static int vc_alsa_playback_open(struct snd_pcm_substream *substream)
{
   vc_alsa_chip_t *chip = snd_pcm_substream_chip(substream);
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = NULL;
   int idx;
   int err;

   LOG_DBG("stream %d ..IN", substream->number);

   idx = substream->number;

   if (idx > MAX_SUBSTREAMS) {
      LOG_ERR("stream %d ..dev doesn't exist", idx);
      err = -ENODEV;
      goto out;
   }

   /* Check if we are ready */
   if (!(chip->avail_substreams & (1 << idx)))
   {
      /* We are not ready yet */
      LOG_ERR("stream %d ..device is not ready yet", idx);
      err = -EAGAIN;
      goto out;
   }

   alsa_stream = kzalloc(sizeof(vc_alsa_stream_t), GFP_KERNEL);
   if (alsa_stream == NULL) 
      return -ENOMEM;

   /* Initialise alsa_stream */
   alsa_stream->chip = chip;
   alsa_stream->substream = substream;
   alsa_stream->idx = idx;
   chip->alsa_stream[idx] = alsa_stream;

   sema_init(&alsa_stream->buffers_update_sem, 0);
   sema_init(&alsa_stream->control_sem, 0);
   spin_lock_init(&alsa_stream->lock);

   /* Enabled in start trigger, called on each "fifo irq" after that */
   alsa_stream->enable_fifo_irq = 0;
   alsa_stream->fifo_irq_handler = vc_alsa_playback_fifo_irq;

   runtime->private_data = alsa_stream;
   runtime->private_free = vc_alsa_playback_free;
   runtime->hw = vc_alsa_playback_hw;

   /* minimum 16 bytes alignment (for vchiq bulk transfers) */
   snd_pcm_hw_constraint_step(runtime, 0, SNDRV_PCM_HW_PARAM_PERIOD_BYTES, 16);

   err = vc_vchiq_alsa_open(alsa_stream);
   if (err != 0) {
      kfree(alsa_stream);
      return err;
   }

   alsa_stream->open = 1;
   alsa_stream->draining = 1;

out:
   LOG_DBG("stream %d %p..OUT =%d", substream->number, alsa_stream, err);

   return err;
}

/* close callback */
static int vc_alsa_playback_close(struct snd_pcm_substream *substream)
{
   /* the hardware-specific codes will be here */
   int err;
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;

   LOG_DBG("stream %d %p ..IN", alsa_stream->idx, alsa_stream);

   /*
    * Call stop if it's still running. This happens when app
    * is force killed and we don't get a stop trigger.
    */
   if (alsa_stream->running)
   {
      err = vc_vchiq_alsa_stop(alsa_stream);
      alsa_stream->running = 0;
      if (err != 0)
         LOG_ERR("stream %d %p ..failed to STOP alsa device",
                 alsa_stream->idx, alsa_stream);
   }

   alsa_stream->period_size = 0;
   alsa_stream->buffer_size = 0;

   if (alsa_stream->open)
   {
      alsa_stream->open = 0;
      err = vc_vchiq_alsa_close(alsa_stream);
      if (err != 0)
         LOG_DBG("stream %d %p..failed to close alsa device",
                 alsa_stream->idx, alsa_stream);
   }
   if (alsa_stream->chip)
      alsa_stream->chip->alsa_stream[alsa_stream->idx] = NULL;
   /*
    * Do not free up alsa_stream here, it will be freed up by
    * runtime->private_free callback we registered in *_open above
    */

   LOG_DBG("stream %d ..OUT", alsa_stream->idx);

   return 0;
}

/* hw_params callback */
static int vc_alsa_pcm_hw_params(struct snd_pcm_substream *substream,
                 struct snd_pcm_hw_params *params)
{
   int err;
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream =
       (vc_alsa_stream_t *) runtime->private_data;

   LOG_DBG("stream %d %p ..IN", alsa_stream->idx, alsa_stream);

   err = snd_pcm_lib_malloc_pages(substream, params_buffer_bytes(params));
   if (err < 0)
   {
      LOG_ERR("stream %d ..pcm_lib_malloc failed to allocated pages for buffers",
              alsa_stream->idx);
      goto out;
   }
   err = vc_vchiq_alsa_set_params(alsa_stream, params_channels(params), 
                                  params_rate(params),
                                  snd_pcm_format_width(params_format(params)));
   if (err < 0)
   {
      LOG_ERR("stream %d error setting hw params", alsa_stream->idx);
      goto out;
   }

   err = vc_vchiq_alsa_set_ctls_chan(alsa_stream, alsa_stream->chip);
   if (err < 0)
   {
      LOG_ERR("stream %d error setting hw params", alsa_stream->idx);
      goto out;
   }   
   
   vc_vchiq_alsa_setup(alsa_stream);
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
out:
   return err;
}

/* hw_free callback */
static int vc_alsa_pcm_hw_free(struct snd_pcm_substream *substream)
{
   LOG_DBG("..IN");
   return snd_pcm_lib_free_pages(substream);
}

/* prepare callback */
static int vc_alsa_pcm_prepare(struct snd_pcm_substream *substream)
{
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   alsa_stream->buffer_size = snd_pcm_lib_buffer_bytes(substream);
   alsa_stream->period_size = snd_pcm_lib_period_bytes(substream);
   alsa_stream->pos = 0;

   LOG_DBG("stream %d buffer_size=%d, period_size=%d pos=%d frame_bits=%d", 
           alsa_stream->idx, alsa_stream->buffer_size, alsa_stream->period_size,
           alsa_stream->pos, runtime->frame_bits);
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
   return 0;
}

/* trigger callback */
static int vc_alsa_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;
   int err = 0;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   switch (cmd)
   {
      case SNDRV_PCM_TRIGGER_START:
         LOG_DBG("stream %d AUDIO_TRIGGER_START running=%d", alsa_stream->idx, 
                 alsa_stream->running);
         if (!alsa_stream->running)
         {
            err = vc_vchiq_alsa_start(alsa_stream);
            if (err == 0)
            {
               alsa_stream->running = 1;
               alsa_stream->draining = 1;
            }
            else
               LOG_ERR("Could not start alsa_stream");
         }
         break;
      case SNDRV_PCM_TRIGGER_STOP:
         LOG_DBG("stream %d AUDIO_TRIGGER_STOP running=%d draining=%d",
                 alsa_stream->idx,
                 runtime->status->state == SNDRV_PCM_STATE_DRAINING,
                 alsa_stream->running);
         if (runtime->status->state == SNDRV_PCM_STATE_DRAINING)
         {
            LOG_DBG("DRAINING");
            alsa_stream->draining = 1;
         }
         else
         {
            LOG_DBG("DROPPING");
            alsa_stream->draining = 0;
         }
         if (alsa_stream->running)
         {
            err = vc_vchiq_alsa_stop(alsa_stream);
            if (err != 0)
               LOG_ERR("stream %d Failed to stop device", alsa_stream->idx);
            alsa_stream->running = 0;
         }
         break;
      default:
         err = -EINVAL;
   }

   LOG_DBG(" ..OUT");
   return err;
}

/* pointer callback */
static snd_pcm_uframes_t vc_alsa_pcm_pointer(struct snd_pcm_substream *substream)
{
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;
   
   LOG_DBG("stream %d ..IN", alsa_stream->idx);

   LOG_DBG("stream %d pcm_pointer(%d) hwptr=%d appl=%d pos=%d", 0, 
           alsa_stream->idx, frames_to_bytes(runtime, runtime->status->hw_ptr),
           frames_to_bytes(runtime, runtime->control->appl_ptr), 
           alsa_stream->pos);

   LOG_DBG("stream %d  ..OUT", alsa_stream->idx);
   return bytes_to_frames(runtime, alsa_stream->pos);
}

static int vc_alsa_pcm_copy(struct snd_pcm_substream *substream,
            int channel, snd_pcm_uframes_t pos, void *src,
            snd_pcm_uframes_t count)
{
   int ret;
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);
   LOG_DBG("stream %d copy(%d) hwptr=%d appl=%d pos=%d", alsa_stream->idx, 
           frames_to_bytes(runtime, count),
           frames_to_bytes(runtime, runtime->status->hw_ptr),
           frames_to_bytes(runtime, runtime->control->appl_ptr),
           alsa_stream->pos);
   
   ret = vc_vchiq_alsa_write(alsa_stream, frames_to_bytes(runtime, count),src);
   
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
   return ret;
}

static int vc_alsa_pcm_silence(struct snd_pcm_substream *substream,
               int channel, snd_pcm_uframes_t post,
               snd_pcm_uframes_t count)
{
   int ret;
   struct snd_pcm_runtime *runtime = substream->runtime;
   vc_alsa_stream_t *alsa_stream = runtime->private_data;

   LOG_DBG("stream %d ..IN", alsa_stream->idx);
   LOG_DBG("stream %d silence(%d) hwptr=%d appl=%d pos=%d", alsa_stream->idx, 
           frames_to_bytes(runtime, count),
           frames_to_bytes(runtime, runtime->status->hw_ptr),
           frames_to_bytes(runtime, runtime->control->appl_ptr),
           alsa_stream->pos);
   
   ret = vc_vchiq_alsa_write(alsa_stream, frames_to_bytes(runtime, count), NULL);
   
   LOG_DBG("stream %d ..OUT", alsa_stream->idx);
   return ret;
}

static int vc_alsa_pcm_lib_ioctl(struct snd_pcm_substream *substream,
                 unsigned int cmd, void *arg)
{
   int ret = snd_pcm_lib_ioctl(substream, cmd, arg);
   LOG_DBG("substream=%p cmd=%d, arg=%p (%x) ret=%d", substream, cmd, arg, 
           arg ? *(unsigned *)arg : 0, ret);
   return ret;
}

/* operators */
static struct snd_pcm_ops vc_alsa_playback_ops = {
   .open = vc_alsa_playback_open,
   .close = vc_alsa_playback_close,
   .ioctl = vc_alsa_pcm_lib_ioctl,
   .hw_params = vc_alsa_pcm_hw_params,
   .hw_free = vc_alsa_pcm_hw_free,
   .prepare = vc_alsa_pcm_prepare,
   .trigger = vc_alsa_pcm_trigger,
   .pointer = vc_alsa_pcm_pointer,
   .copy = vc_alsa_pcm_copy,
   .silence = vc_alsa_pcm_silence,
};

/* create a pcm device */
int __init vc_alsa_new_pcm(vc_alsa_chip_t * chip)
{
   struct snd_pcm *pcm;
   int err, i;

   LOG_DBG(" ..IN");
   err = snd_pcm_new(chip->card, "capri ALSA", 0, MAX_SUBSTREAMS, 0, &pcm);
   if (err < 0)
   {
     LOG_ERR("Could not create PCM");
     return err;
   }
   pcm->private_data = chip;
   strcpy(pcm->name, "capri ALSA pcm device");
   chip->pcm = pcm;
   LOG_DBG( "setting audio destination to HDMI" );
	for (i = 0; i < MAX_SUBSTREAMS; i++)
	{
		chip->dest[i] = AUDIO_DEST_AUTO;
		chip->volume[i] = 0;
	}
   /* set operators */
   snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK,
         &vc_alsa_playback_ops);

   /* pre-allocation of buffers */
   /* NOTE: this may fail */
   snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_CONTINUOUS,
                     snd_dma_continuous_data
                     (GFP_KERNEL), 64 * 1024,
                     64 * 1024);

   LOG_DBG(" ..OUT");

   return 0;
}
