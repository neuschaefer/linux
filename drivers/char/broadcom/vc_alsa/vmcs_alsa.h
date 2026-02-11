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

#ifndef __VMCS_ALSA_H
#define __VMCS_ALSA_H

#define SUBSTREAM_NUM 1

#include <linux/device.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <linux/workqueue.h>
#include <linux/semaphore.h>

/* #define DUMP_RAW_DATA */
//#define AUDIO_DEBUG_ENABLE

/* Debug macros */
#ifdef AUDIO_DEBUG_ENABLE
#define LOG_DBG(fmt, arg... )  printk(KERN_INFO "[D] " "%s:"fmt"\n", __func__, ##arg )
#else
#define LOG_DBG(fmt, arg...) ;
#endif /* AUDIO_DEBUG_ENABLE */

#define LOG_INFO(fmt, arg... )  printk(KERN_INFO "[I] " "%s:"fmt "\n", __func__, ##arg )
#define LOG_ERR(fmt, arg... )  printk(KERN_ERR  "[E] " "%s:"fmt "\n", __func__, ##arg )

#define MAX_SUBSTREAMS         (8)
#define AVAIL_SUBSTREAMS_MASK      (0xff)

/* Some constants for values .. */
typedef enum {
   AUDIO_DEST_AUTO = 0,
   AUDIO_DEST_HEADPHONES = 1,
   AUDIO_DEST_HDMI = 2,
   AUDIO_DEST_MAX,
} SND_VC_ALSA_ROUTE_T;

#define VOLUME_CONTROL_MIN_ID (1)
#define VOLUME_CONTROL_MAX_ID (MAX_SUBSTREAMS)
#define MUTE_CONTROL_MIN_ID (VOLUME_CONTROL_MAX_ID+1)
#define MUTE_CONTROL_MAX_ID (VOLUME_CONTROL_MAX_ID+MAX_SUBSTREAMS)
#define ROUTE_CONTROL_MIN_ID (MUTE_CONTROL_MAX_ID+1)
#define ROUTE_CONTROL_MAX_ID (MUTE_CONTROL_MAX_ID+MAX_SUBSTREAMS)

typedef enum {
   PCM_PLAYBACK_VOLUME,
   PCM_PLAYBACK_MUTE,
   PCM_PLAYBACK_DEVICE,
} SND_VC_ALSA_CTRL_T;

/* this struct is tightly packed - its size is 16bytes */
typedef struct {
   uint32_t buffer_id;
   uint32_t buffer_size;
   uint32_t buffer_ptr;
   uint32_t spare;

} AUDIO_FIFO_ENTRY_T;

/* definition of the chip-specific record */
typedef struct vc_alsa_chip {
   struct snd_card *card;
   struct snd_pcm *pcm;
   
   struct vc_alsa_stream *alsa_stream[MAX_SUBSTREAMS];
   
   uint32_t avail_substreams;
   int32_t volume[MAX_SUBSTREAMS];
   int32_t dest[MAX_SUBSTREAMS];
   int32_t mute[MAX_SUBSTREAMS];
} vc_alsa_chip_t;

typedef struct vc_alsa_buffer {
   uint32_t buffer_id;
   phys_addr_t bus_addr;
   uint8_t __iomem *start;
   uint32_t size;
   uint32_t data_left;
   struct list_head link;

} vc_alsa_buffer_t;

typedef struct vc_alsa_stream {
   vc_alsa_chip_t *chip;
   struct snd_pcm_substream *substream;

   struct semaphore buffers_update_sem;
   struct semaphore control_sem;
   spinlock_t lock;
   volatile uint32_t control;
   volatile uint32_t status;

   int32_t open;
   int32_t running;
   int32_t draining;

#ifdef DUMP_RAW_DATA
   /* for debug */
   int32_t file;
#endif
   uint32_t pos;
   uint32_t buffer_size;
   uint32_t period_size;

   uint32_t enable_fifo_irq;
   irq_handler_t fifo_irq_handler;

   atomic_t retrieved;
   void *instance;   
   uint8_t idx;
} vc_alsa_stream_t;


int32_t vc_alsa_new_pcm(vc_alsa_chip_t * chip);
int32_t vc_alsa_new_ctl(vc_alsa_chip_t * chip);
#endif /* __VMCS_ALSA_H */
