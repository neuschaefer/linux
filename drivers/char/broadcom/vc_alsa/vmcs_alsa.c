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
#include <linux/slab.h>

#include "vmcs_alsa.h"
#include <vc_vchi_audioserv.h>


/* module parameters (see "Module Parameters") */
/* SNDRV_CARDS: maximum number of cards supported by this module */
static int index[MAX_SUBSTREAMS] = {[0 ... (MAX_SUBSTREAMS - 1)] = -1 };
static char *id[MAX_SUBSTREAMS] = {[0 ... (MAX_SUBSTREAMS - 1)] = NULL };
static int enable[MAX_SUBSTREAMS] = {[0 ... (MAX_SUBSTREAMS - 1)] = 1 };

/* HACKY global pointers needed for successive probes to work : ssp
 * But compared against the changes we will have to do in VC audio_ipc code
 * to export 8 audio_ipc devices as a single IPC device and then monitor all
 * four devices in a thread, this gets things done quickly and should be easier
 * to debug if we run into issues
 */

static struct snd_card *vc_alsa_card = NULL;
static vc_alsa_chip_t *vc_alsa_chip = NULL;

static int vc_alsa_free(vc_alsa_chip_t * chip)
{
   kfree(chip);
   return 0;
}

/* component-destructor
 * (see "Management of Cards and Components")
 */
static int vc_alsa_dev_free(struct snd_device *device)
{
   return vc_alsa_free(device->device_data);
}


/*Need to be called from somewhere, not functional as at present*/
#if 0
static int vc_alsa_remove(int dev_num)
{
   if (vc_alsa_card != NULL)
   {
      BUG_ON(!vc_alsa_chip);
      vc_alsa_chip->avail_substreams &= ~(1 << dev_num);
      /*
       * There should be atleast one substream registered
       * after we are done here, as it wil be removed when
       * the *remove* is called for the card device
       */
      BUG_ON(!vc_alsa_chip->avail_substreams);
   }  
   return 0;
}
#endif

static int __init vc_alsa_init(void)
{
   int err, ret, dev_num;
   static struct snd_device_ops ops = {
      .dev_free = vc_alsa_dev_free,
   };
   
   
   LOG_INFO("Creating global card object");
   
   err = snd_card_create(index[0], id[0], THIS_MODULE, 0, &vc_alsa_card);
   if (err < 0)
   {
      LOG_ERR("Failed to create sound card");
      ret = -ENOMEM;
      goto out_error;
   }
   //snd_card_set_dev(vc_alsa_card, &pdev->dev);

   //strcpy(vc_alsa_card->driver, "BRCM capri ALSA Driver");
   strncpy(vc_alsa_card->driver, "BRCM capri ALSA", sizeof(vc_alsa_card->driver)-1); // limited to 16 chars
   vc_alsa_card->driver[ sizeof( vc_alsa_card->driver)-1 ] = 0; // 0 terminate just in case
   strncpy(vc_alsa_card->shortname, "capri ALSA", sizeof(vc_alsa_card->shortname)-1);
   vc_alsa_card->shortname[sizeof(vc_alsa_card->shortname)-1] = 0;

   sprintf(vc_alsa_card->longname, "%s", vc_alsa_card->shortname);
   
   LOG_INFO("Creating device/chip for alsa device");
   vc_alsa_chip = kzalloc(sizeof(vc_alsa_chip_t), GFP_KERNEL);
   if (vc_alsa_chip == NULL)
   {
      LOG_ERR ("Failed to create chip");
      ret = -ENOMEM;
      goto out_error;
   }
   
   vc_alsa_chip->card = vc_alsa_card;
   
   err = snd_device_new(vc_alsa_card, SNDRV_DEV_LOWLEVEL, vc_alsa_chip, &ops);
   if (err < 0)
   {
      LOG_ERR("Failed to create new sound device");
      ret = -ENOMEM;
      goto out_error;
   }
   
   vc_vchiq_alsa_init();
   
   //Indicate the streams are available
   for(dev_num = 0 ; dev_num < MAX_SUBSTREAMS ; dev_num++)
   {
      if (enable[dev_num])
         vc_alsa_chip->avail_substreams |= (1 << dev_num);
   }
   
   LOG_INFO("Creating pcm device");
   err = vc_alsa_new_pcm(vc_alsa_chip);
   if (err < 0)
   {
      LOG_ERR("Failed to create new pcm device");
      ret = err;
      goto out_clean_vchi;
   }
   
   LOG_INFO("Adding controls");
   err = vc_alsa_new_ctl(vc_alsa_chip);
   if (err < 0)
   {
      LOG_ERR("Failed to create new ctl device");
      ret = err;
      goto out_clean_vchi;
   }
   /*
   LOG_INFO("Setting default destination to HDMI");
   vc_alsa_chip->dest = AUDIO_DEST_HDMI;
   LOG_INFO("Setting default volume to 100");
   vc_alsa_chip->volume = 100;
   */
   LOG_INFO("Registering card");
   err = snd_card_register(vc_alsa_card);
   if (err < 0)
   {
      LOG_ERR("Failed to register capri ALSA card");
      ret = err;
      goto out_clean_vchi;
   }
   
   ret = 0;
   goto out;

out_clean_vchi:
   vc_vchiq_alsa_deinit();
out_error:
   if (vc_alsa_card)
   {
      if (snd_card_free(vc_alsa_card))
         LOG_ERR("Failed to free Registered alsa card");
      vc_alsa_card = NULL;
   }
if (vc_alsa_chip)
      kfree(vc_alsa_chip);
out:  
   return ret;
   
}

static void __exit vc_alsa_exit(void)
{
   if (vc_alsa_card != NULL)
   {
      BUG_ON(!vc_alsa_chip);
      LOG_INFO("Freeing alsa chip");
      snd_device_free(vc_alsa_card, vc_alsa_chip);
      LOG_INFO("Freeing alsa card");
      snd_card_free(vc_alsa_card);
      vc_alsa_card = NULL;
      vc_alsa_chip = NULL;
      vc_vchiq_alsa_deinit();
   }    
}

late_initcall(vc_alsa_init);
module_exit(vc_alsa_exit);

MODULE_AUTHOR( "Broadcom" );
MODULE_DESCRIPTION( "VC Alsa Driver" );
MODULE_LICENSE( "GPL" );
MODULE_ALIAS("platform:capri_alsa");
