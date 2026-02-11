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

#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <sound/core.h>
#include <sound/control.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/rawmidi.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "vmcs_alsa.h"
#include <vc_vchi_audioserv.h>

static int
vc_alsa_ctl_info( struct snd_kcontrol *kcontrol,
                  struct snd_ctl_elem_info *uinfo )
{
	int num_id = uinfo->id.numid;
   if ( num_id <= VOLUME_CONTROL_MAX_ID )
   {
      uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
      uinfo->count = 1;
      uinfo->value.integer.min = -10240;
      uinfo->value.integer.max = 2303;
   }
   else if ( num_id <= MUTE_CONTROL_MAX_ID )
   {
      uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
      uinfo->count = 1;
      uinfo->value.integer.min = 0;
      uinfo->value.integer.max = 1;
   }
   else if ( num_id <= ROUTE_CONTROL_MAX_ID )
   {
      uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
      uinfo->count = 1;
      uinfo->value.integer.min = 0;
      uinfo->value.integer.max = AUDIO_DEST_MAX-0;
   }
   
   return 0;
}

static int
vc_alsa_ctl_get( struct snd_kcontrol *kcontrol,
                 struct snd_ctl_elem_value *ucontrol )
{
   struct vc_alsa_chip *chip = snd_kcontrol_chip( kcontrol );
	int num_id = ucontrol->id.numid;
	int index = 0;
   
   BUG_ON( !chip && !( chip->avail_substreams & AVAIL_SUBSTREAMS_MASK ) );
   
   if ( num_id <= VOLUME_CONTROL_MAX_ID )
	{
		index = num_id - VOLUME_CONTROL_MIN_ID;
      ucontrol->value.integer.value[index] = chip->volume[index];
	}
   else if ( num_id <= MUTE_CONTROL_MAX_ID )
	{
		index = num_id - MUTE_CONTROL_MIN_ID;
      ucontrol->value.integer.value[index] = chip->mute[index];
	}
   else if ( num_id <= ROUTE_CONTROL_MAX_ID )
	{
		index = num_id - ROUTE_CONTROL_MIN_ID;
      ucontrol->value.integer.value[index] = chip->dest[index];
	}
   
   return 0;
}

static int
vc_alsa_ctl_put( struct snd_kcontrol *kcontrol,
                 struct snd_ctl_elem_value *ucontrol )
{
   struct vc_alsa_chip *chip = snd_kcontrol_chip( kcontrol );
   int changed = 0;
	int num_id = ucontrol->id.numid;
	int index = 0;
   
   if ( num_id <= VOLUME_CONTROL_MAX_ID )
   {
		index = num_id - VOLUME_CONTROL_MIN_ID;
      if ( chip->mute[index] )
      {
         chip->mute[index] = 0;
         changed = 1;
      }
      if ( changed || ( ucontrol->value.integer.value[index] != chip->volume[index] ) )
      {
         int atten;
         chip->volume[index] = ucontrol->value.integer.value[index];
         changed = 1;
         atten = -( ( chip->volume[index] << 8 ) / 100 );
         chip->volume[index] = atten;
      }
   }
   else if ( num_id <= MUTE_CONTROL_MAX_ID )
   {
		index = num_id - MUTE_CONTROL_MIN_ID;
      /* Not implemented */
      if ( ucontrol->value.integer.value[index] != chip->mute[index] )
      {
         chip->mute[index] = ucontrol->value.integer.value[index];
         changed = 0;
      }
   }
   else if ( num_id <= ROUTE_CONTROL_MAX_ID )
   {
		index = num_id - ROUTE_CONTROL_MIN_ID;
      if ( ucontrol->value.integer.value[index] != chip->dest[index] )
      {
         chip->dest[index] = ucontrol->value.integer.value[index];
         changed = 1;
      }
   }
   
   if ( changed )
   {
      if ( vc_vchiq_alsa_set_ctls( chip ) )
         LOG_ERR( "failed to set ALSA controls" );
   }
   
   return changed;
}

static DECLARE_TLV_DB_SCALE( vc_alsa_db_scale, -10240, 1, 1 );

static struct snd_kcontrol_new vc_alsa_ctl[] __devinitdata = {
   {
      .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
      .name = "PCM Playback Volume",
      .index = 0,
      .access =
      SNDRV_CTL_ELEM_ACCESS_READWRITE | SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE,
      .private_value = PCM_PLAYBACK_VOLUME,
      .info = vc_alsa_ctl_info,
      .get = vc_alsa_ctl_get,
      .put = vc_alsa_ctl_put,
      .count = MAX_SUBSTREAMS,
      .tlv = {.p = vc_alsa_db_scale}
   },
   {
      .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
      .name = "PCM Playback Switch",
      .index = 0,
      .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,
      .private_value = PCM_PLAYBACK_MUTE,
      .info = vc_alsa_ctl_info,
      .get = vc_alsa_ctl_get,
      .put = vc_alsa_ctl_put,
      .count = MAX_SUBSTREAMS,
   },
   {
      .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
      .name = "PCM Playback Route",
      .index = 0,
      .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,
      .private_value = PCM_PLAYBACK_DEVICE,
      .info = vc_alsa_ctl_info,
      .get = vc_alsa_ctl_get,
      .put = vc_alsa_ctl_put,
      .count = MAX_SUBSTREAMS,
   },
};

int __init
vc_alsa_new_ctl( vc_alsa_chip_t * chip )
{
   int err;
   unsigned int idx;
   
   strcpy( chip->card->mixername, "Broadcom Mixer" );
   for ( idx = 0; idx < ARRAY_SIZE(vc_alsa_ctl); idx++ )
   {
      err = snd_ctl_add( chip->card, snd_ctl_new1( &vc_alsa_ctl[idx], chip ) );
      if ( err < 0 )
         return err;
   }
   return 0;
}
