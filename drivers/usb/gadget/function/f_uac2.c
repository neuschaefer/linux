/*
 * f_uac2.c -- USB Audio Class 2.0 Function
 *
 * Copyright (C) 2011
 *    Yadwinder Singh (yadi.brar01@gmail.com)
 *    Jaswinder Singh (jaswinder.singh@linaro.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/usb/audio.h>
#include <linux/usb/audio-v2.h>
#include <linux/platform_device.h>
#include <linux/module.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>

#include "u_uac2.h"

/* Keep everyone on toes */
#define USB_XFERS	2

/*
 * The driver implements a simple UAC_2 topology.
 * USB-OUT -> IT_1 -> OT_3 -> ALSA_Capture
 * ALSA_Playback -> IT_2 -> OT_4 -> USB-IN
 * Capture and Playback sampling rates are independently
 *  controlled by two clock sources :
 *    CLK_5 := c_srate, and CLK_6 := p_srate
 */
#define USB_OUT_IT_ID	1
#define IO_IN_IT_ID	2
#define IO_OUT_OT_ID	3
#define USB_IN_OT_ID	4
#define USB_OUT_CLK_ID	5
#define USB_IN_CLK_ID	6

#define USB_FEATURE_UNIT_ID	8
#define USB_OUT_CLK_SEL_ID	40
#define USB_IN_CLK_SEL_ID	41

#define CONTROL_ABSENT	0
#define CONTROL_RDONLY	1
#define CONTROL_RDWR	3

#define CLK_FREQ_CTRL	0
#define CLK_VLD_CTRL	2

#define COPY_CTRL	0
#define CONN_CTRL	2
#define OVRLD_CTRL	4
#define CLSTR_CTRL	6
#define UNFLW_CTRL	8
#define OVFLW_CTRL	10

static struct audio_dev *agdev;
static const char *uac2_name = "snd_uac2";

static u8 r_high[] = {
	0x09, 0x00, /* wNumSubRanges */

	0x00, 0x7D, 0x00, 0x00, /* dMIN 32000Hz */
	0x00, 0x7D, 0x00, 0x00, /* dMAX 32000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 32000Hz */

	0x44, 0xAC, 0x00, 0x00, /* dMIN 44100Hz */
	0x44, 0xAC, 0x00, 0x00, /* dMAX 44100Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 44100Hz */

	0x80, 0xBB, 0x00, 0x00, /* dMIN 48000Hz */
	0x80, 0xBB, 0x00, 0x00, /* dMAX 48000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 48000Hz */

	0x88, 0x58, 0x01, 0x00, /* dMIN 88200Hz */
	0x88, 0x58, 0x01, 0x00, /* dMAX 88200Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 88200Hz */

	0x00, 0x77, 0x01, 0x00, /* dMIN 96000Hz */
	0x00, 0x77, 0x01, 0x00, /* dMAX 96000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 96000Hz */

	0x10, 0xB1, 0x02, 0x00, /* dMIN 176400Hz */
	0x10, 0xB1, 0x02, 0x00, /* dMAX 176400Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 176400Hz */

	0x00, 0xEE, 0x02, 0x00, /* dMIN 192000Hz */
	0x00, 0xEE, 0x02, 0x00, /* dMAX 192000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 192000Hz */

	0x20, 0x62, 0x05, 0x00, /* dMIN 352800Hz */
	0x20, 0x62, 0x05, 0x00, /* dMAX 352800Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 352800Hz */

	0x00, 0xDC, 0x05, 0x00, /* dMIN 384000Hz */
	0x00, 0xDC, 0x05, 0x00, /* dMAX 384000Hz */
	0x00, 0x00, 0x00, 0x00	/* dRES 384000Hz */
};

static u8 r_full[] = {
	0x05, 0x00, /* wNumSubRanges */

	0x00, 0x7D, 0x00, 0x00, /* dMIN 32000Hz */
	0x00, 0x7D, 0x00, 0x00, /* dMAX 32000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 32000Hz */

	0x44, 0xAC, 0x00, 0x00, /* dMIN 44100Hz */
	0x44, 0xAC, 0x00, 0x00, /* dMAX 44100Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 44100Hz */

	0x80, 0xBB, 0x00, 0x00, /* dMIN 48000Hz */
	0x80, 0xBB, 0x00, 0x00, /* dMAX 48000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 48000Hz */

	0x88, 0x58, 0x01, 0x00, /* dMIN 88200Hz */
	0x88, 0x58, 0x01, 0x00, /* dMAX 88200Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 88200Hz */

	0x00, 0x77, 0x01, 0x00, /* dMIN 96000Hz */
	0x00, 0x77, 0x01, 0x00, /* dMAX 96000Hz */
	0x00, 0x00, 0x00, 0x00, /* dRES 96000Hz */
};


#define UAC_CLOCK_SELECTOR_DES(ch)	\
struct uac_clock_selector_descriptor_##ch {	\
	__u8 bLength;	\
	__u8 bDescriptorType;	\
	__u8 bDescriptorSubtype;	\
	__u8 bClockID;	\
	__u8 bNrInPins;	\
	__u8 baCSourceID[ch];	\
} __attribute__ ((packed))

UAC_CLOCK_SELECTOR_DES(3);

#define UAC_CLOCK_SELECTOR_DES_SIZE(ch) (sizeof(struct uac_clock_selector_descriptor) + (ch))

#define UAC2_FEATURE_UNIT_DES(ch)	\
struct uac2_feature_unit_descriptor_##ch {	\
	__u8 bLength;	\
	__u8 bDescriptorType;	\
	__u8 bDescriptorSubtype;	\
	__u8 bUnitID;	\
	__u8 bSourceID;	\
	__u8 bmaControls[ch];	\
} __attribute__((packed))

UAC2_FEATURE_UNIT_DES(13);

#define UAC2_FEATURE_UNIT_DES_SIZE(ch) (sizeof(struct uac2_feature_unit_descriptor) + (ch))

struct uac2_req {
	struct uac2_rtd_params *pp; /* parent param */
	struct usb_request *req;
};

struct uac2_rtd_params {
	struct snd_uac2_chip *uac2; /* parent chip */
	bool ep_enabled; /* if the ep is enabled */
	/* Size of the ring buffer */
	size_t dma_bytes;
	unsigned char *dma_area;

	struct snd_pcm_substream *ss;

	/* Ring buffer */
	ssize_t hw_ptr;

	void *rbuf;

	size_t period_size;

	unsigned max_psize;
	struct uac2_req ureq[USB_XFERS];

	spinlock_t lock;
};

struct snd_uac2_chip {
	struct platform_device pdev;
	struct platform_driver pdrv;

	struct uac2_rtd_params p_prm;
	struct uac2_rtd_params c_prm;

	struct snd_card *card;
	struct snd_pcm *pcm;

	/* timekeeping for the playback endpoint */
	unsigned int p_interval;
	unsigned int p_residue;

	/* pre-calculated values for playback iso completion */
	unsigned int p_pktsize;
	unsigned int p_pktsize_residue;
	unsigned int p_framesize;
};

#define BUFF_SIZE_MAX	(PAGE_SIZE * 16)
#define PRD_SIZE_MAX	PAGE_SIZE
#define MIN_PERIODS	4

static struct snd_pcm_hardware uac2_pcm_hardware = {
	.info = SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_BLOCK_TRANSFER
		 | SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID
		 | SNDRV_PCM_INFO_PAUSE | SNDRV_PCM_INFO_RESUME,
	.rates = SNDRV_PCM_RATE_CONTINUOUS,
	.periods_max = BUFF_SIZE_MAX / PRD_SIZE_MAX,
	.buffer_bytes_max = BUFF_SIZE_MAX,
	.period_bytes_max = PRD_SIZE_MAX,
	.periods_min = MIN_PERIODS,
};

struct audio_dev {
	u8 ac_intf, ac_alt;
	u8 as_out_intf, as_out_alt;
	u8 as_in_intf, as_in_alt;

	struct usb_ep *in_ep, *out_ep, *fb_ep;
	struct usb_function func;

	/* The ALSA Sound Card it represents on the USB-Client side */
	struct snd_uac2_chip uac2;
	struct f_uac2_hw_params params;

	int gspeed;

	/* Control Set command */
	struct list_head cs;
	u8 set_cmd;
	struct usb_audio_control *set_con;
};

static inline
struct audio_dev *func_to_agdev(struct usb_function *f)
{
	return container_of(f, struct audio_dev, func);
}

static inline
struct audio_dev *uac2_to_agdev(struct snd_uac2_chip *u)
{
	return container_of(u, struct audio_dev, uac2);
}

static inline
struct snd_uac2_chip *pdev_to_uac2(struct platform_device *p)
{
	return container_of(p, struct snd_uac2_chip, pdev);
}

static inline
uint num_channels(uint chanmask)
{
	uint num = 0;

	while (chanmask) {
		num += (chanmask & 1);
		chanmask >>= 1;
	}

	return num;
}

static void
agdev_iso_complete(struct usb_ep *ep, struct usb_request *req)
{
	unsigned pending;
	unsigned long flags;
	unsigned int hw_ptr;
	bool update_alsa = false;
	int status = req->status;
	struct uac2_req *ur = req->context;
	struct snd_pcm_substream *substream;
	struct uac2_rtd_params *prm = ur->pp;
	struct snd_uac2_chip *uac2 = prm->uac2;

	/* i/f shutting down */
	if (!prm->ep_enabled || req->status == -ESHUTDOWN)
		return;

	/*
	 * We can't really do much about bad xfers.
	 * Afterall, the ISOCH xfers could fail legitimately.
	 */
	if (status)
		pr_debug("%s: iso_complete status(%d) %d/%d\n",
			__func__, status, req->actual, req->length);

	substream = prm->ss;

	/* Do nothing if ALSA isn't active */
	if (!substream)
		goto exit;

	spin_lock_irqsave(&prm->lock, flags);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		/*
		 * For each IN packet, take the quotient of the current data
		 * rate and the endpoint's interval as the base packet size.
		 * If there is a residue from this division, add it to the
		 * residue accumulator.
		 */
		req->length = uac2->p_pktsize;
		uac2->p_residue += uac2->p_pktsize_residue;

		/*
		 * Whenever there are more bytes in the accumulator than we
		 * need to add one more sample frame, increase this packet's
		 * size and decrease the accumulator.
		 */
		if (uac2->p_residue / uac2->p_interval >= uac2->p_framesize) {
			req->length += uac2->p_framesize;
			uac2->p_residue -= uac2->p_framesize *
					   uac2->p_interval;
		}

		req->actual = req->length;
	}

	pending = prm->hw_ptr % prm->period_size;
	pending += req->actual;
	if (pending >= prm->period_size)
		update_alsa = true;

	hw_ptr = prm->hw_ptr;
	prm->hw_ptr = (prm->hw_ptr + req->actual) % prm->dma_bytes;

	spin_unlock_irqrestore(&prm->lock, flags);

	/* Pack USB load in ALSA ring buffer */
	pending = prm->dma_bytes - hw_ptr;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (unlikely(pending < req->actual)) {
			memcpy(req->buf, prm->dma_area + hw_ptr, pending);
			memcpy(req->buf + pending, prm->dma_area,
			       req->actual - pending);
		} else {
			memcpy(req->buf, prm->dma_area + hw_ptr, req->actual);
		}
	} else {
		if (unlikely(pending < req->actual)) {
			memcpy(prm->dma_area + hw_ptr, req->buf, pending);
			memcpy(prm->dma_area, req->buf + pending,
			       req->actual - pending);
		} else {
			memcpy(prm->dma_area + hw_ptr, req->buf, req->actual);
		}
	}

exit:
	if (usb_ep_queue(ep, req, GFP_ATOMIC))
		dev_err(&uac2->pdev.dev, "%d Error!\n", __LINE__);

	if (update_alsa)
		snd_pcm_period_elapsed(substream);

	return;
}

static int
uac2_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_uac2_chip *uac2 = snd_pcm_substream_chip(substream);
	struct uac2_rtd_params *prm;
	unsigned long flags;
	int err = 0;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prm = &uac2->p_prm;
	else
		prm = &uac2->c_prm;

	spin_lock_irqsave(&prm->lock, flags);

	/* Reset */
	prm->hw_ptr = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
		prm->ss = substream;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		prm->ss = NULL;
		break;
	default:
		err = -EINVAL;
	}

	spin_unlock_irqrestore(&prm->lock, flags);

	/* Clear buffer after Play stops */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK && !prm->ss)
		memset(prm->rbuf, 0, prm->max_psize * USB_XFERS);

	return err;
}

static snd_pcm_uframes_t uac2_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_uac2_chip *uac2 = snd_pcm_substream_chip(substream);
	struct uac2_rtd_params *prm;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prm = &uac2->p_prm;
	else
		prm = &uac2->c_prm;

	return bytes_to_frames(substream->runtime, prm->hw_ptr);
}

static int uac2_pcm_hw_params(struct snd_pcm_substream *substream,
			       struct snd_pcm_hw_params *hw_params)
{
	struct snd_uac2_chip *uac2 = snd_pcm_substream_chip(substream);
	struct uac2_rtd_params *prm;
	int err;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prm = &uac2->p_prm;
	else
		prm = &uac2->c_prm;

	err = snd_pcm_lib_malloc_pages(substream,
					params_buffer_bytes(hw_params));
	if (err >= 0) {
		prm->dma_bytes = substream->runtime->dma_bytes;
		prm->dma_area = substream->runtime->dma_area;
		prm->period_size = params_period_bytes(hw_params);
	}

	return err;
}

static int uac2_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_uac2_chip *uac2 = snd_pcm_substream_chip(substream);
	struct uac2_rtd_params *prm;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prm = &uac2->p_prm;
	else
		prm = &uac2->c_prm;

	prm->dma_area = NULL;
	prm->dma_bytes = 0;
	prm->period_size = 0;

	return snd_pcm_lib_free_pages(substream);
}

static int uac2_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_uac2_chip *uac2 = snd_pcm_substream_chip(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct audio_dev *audio_dev;
	struct f_uac2_hw_params *hwparams;
	int p_ssize, c_ssize;
	int p_srate, c_srate;
	int p_chmask, c_chmask;

	audio_dev = uac2_to_agdev(uac2);

	if ((audio_dev->as_in_alt == 0) &&
		(audio_dev->as_out_alt == 0)) {
		pr_err("UAC2 open fail: there is no avild alt ");
		pr_err("please Check Host default output and input\n");
		return -EPERM;
	}

	hwparams = &audio_dev->params;
	p_ssize = hwparams->p_ssize;
	c_ssize = hwparams->c_ssize;
	p_srate = hwparams->p_srate;
	c_srate = hwparams->c_srate;
	p_chmask = hwparams->p_chmask;
	c_chmask = hwparams->c_chmask;
	uac2->p_residue = 0;

	runtime->hw = uac2_pcm_hardware;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		pr_info("uac2 open host in, sample[%uHz], bit[0x%x0] ch[0x%x]\n",
			p_srate, (p_ssize * 8), p_chmask);
		spin_lock_init(&uac2->p_prm.lock);
		runtime->hw.rate_min = p_srate;
		switch (p_ssize) {
		case 3:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S24_3LE;
			break;
		case 4:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S32_LE;
			break;
		default:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S16_LE;
			break;
		}
		runtime->hw.channels_min = num_channels(p_chmask);
		runtime->hw.period_bytes_min = 2 * uac2->p_prm.max_psize
						/ runtime->hw.periods_min;
	} else {
		pr_info("uac2 open host out, sample[%uHz], bit[%d] ch[0x%x]\n",
			c_srate, (c_ssize * 8), c_chmask);
		spin_lock_init(&uac2->c_prm.lock);
		runtime->hw.rate_min = c_srate;
		switch (c_ssize) {
		case 3:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S24_3LE;
			break;
		case 4:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S32_LE;
			break;
		default:
			runtime->hw.formats = SNDRV_PCM_FMTBIT_S16_LE;
			break;
		}
		runtime->hw.channels_min = num_channels(c_chmask);
		runtime->hw.period_bytes_min = 2 * uac2->c_prm.max_psize
						/ runtime->hw.periods_min;
	}

	runtime->hw.rate_max = runtime->hw.rate_min;
	runtime->hw.channels_max = runtime->hw.channels_min;

	snd_pcm_hw_constraint_integer(runtime, SNDRV_PCM_HW_PARAM_PERIODS);

	return 0;
}

/* ALSA cries without these function pointers */
static int uac2_pcm_null(struct snd_pcm_substream *substream)
{
	return 0;
}

static struct snd_pcm_ops uac2_pcm_ops = {
	.open = uac2_pcm_open,
	.close = uac2_pcm_null,
	.ioctl = snd_pcm_lib_ioctl,
	.hw_params = uac2_pcm_hw_params,
	.hw_free = uac2_pcm_hw_free,
	.trigger = uac2_pcm_trigger,
	.pointer = uac2_pcm_pointer,
	.prepare = uac2_pcm_null,
};

static int snd_uac2_probe(struct platform_device *pdev)
{
	struct snd_uac2_chip *uac2 = pdev_to_uac2(pdev);
	struct snd_card *card;
	struct snd_pcm *pcm;
	struct audio_dev *audio_dev;
	int err;
	int p_chmask, c_chmask;

	audio_dev = uac2_to_agdev(uac2);
	p_chmask = audio_dev->params.p_chmask;
	c_chmask = audio_dev->params.c_chmask;

	/* Choose any slot, with no id */
	err = snd_card_new(&pdev->dev, -1, NULL, THIS_MODULE, 0, &card);
	if (err < 0)
		return err;

	uac2->card = card;

	/*
	 * Create first PCM device
	 * Create a substream only for non-zero channel streams
	 */
	err = snd_pcm_new(uac2->card, "UAC2 PCM", 0,
			       p_chmask ? 1 : 0, c_chmask ? 1 : 0, &pcm);
	if (err < 0)
		goto snd_fail;

	strcpy(pcm->name, "UAC2 PCM");
	pcm->private_data = uac2;

	uac2->pcm = pcm;

	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &uac2_pcm_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &uac2_pcm_ops);

	strcpy(card->driver, "UAC2_Gadget");
	strcpy(card->shortname, "UAC2_Gadget");
	sprintf(card->longname, "UAC2_Gadget %i", pdev->id);

	snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_CONTINUOUS,
		snd_dma_continuous_data(GFP_KERNEL), 0, BUFF_SIZE_MAX);

	err = snd_card_register(card);
	if (!err) {
		platform_set_drvdata(pdev, card);
		return 0;
	}

snd_fail:
	snd_card_free(card);

	uac2->pcm = NULL;
	uac2->card = NULL;

	return err;
}

static int snd_uac2_remove(struct platform_device *pdev)
{
	struct snd_card *card = platform_get_drvdata(pdev);

	if (card)
		return snd_card_free(card);

	return 0;
}

static void snd_uac2_release(struct device *dev)
{
	dev_dbg(dev, "releasing '%s'\n", dev_name(dev));
}

static int alsa_uac2_init(struct audio_dev *agdev)
{
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	int err;

	uac2->pdrv.probe = snd_uac2_probe;
	uac2->pdrv.remove = snd_uac2_remove;
	uac2->pdrv.driver.name = uac2_name;

	uac2->pdev.id = 0;
	uac2->pdev.name = uac2_name;
	uac2->pdev.dev.release = snd_uac2_release;

	/* Register snd_uac2 driver */
	err = platform_driver_register(&uac2->pdrv);
	if (err)
		return err;

	/* Register snd_uac2 device */
	err = platform_device_register(&uac2->pdev);
	if (err)
		platform_driver_unregister(&uac2->pdrv);

	return err;
}

static void alsa_uac2_exit(struct audio_dev *agdev)
{
	struct snd_uac2_chip *uac2 = &agdev->uac2;

	platform_driver_unregister(&uac2->pdrv);
	platform_device_unregister(&uac2->pdev);
}


/* --------- USB Function Interface ------------- */

enum {
	STR_ASSOC,
	STR_IF_CTRL,
	STR_CLKSRC_IN,
	STR_CLKSRC_OUT,
	STR_USB_IT,
	STR_IO_IT,
	STR_USB_OT,
	STR_IO_OT,
	STR_AS_OUT_ALT0,
	STR_AS_OUT_ALT1,
	STR_AS_OUT_ALT2,
	STR_AS_IN_ALT0,
	STR_AS_IN_ALT1,
	STR_AS_IN_ALT2,
};

enum {
	ALT_OUT = 1,
	ALT_IN,
};

static char clksrc_in[8];
static char clksrc_out[8];

static struct usb_string strings_fn[] = {
	[STR_ASSOC].s = "Source/Sink",
	[STR_IF_CTRL].s = "Topology Control",
	[STR_CLKSRC_IN].s = clksrc_in,
	[STR_CLKSRC_OUT].s = clksrc_out,
	[STR_USB_IT].s = "USBH Out",
	[STR_IO_IT].s = "USBD Out",
	[STR_USB_OT].s = "USBH In",
	[STR_IO_OT].s = "USBD In",
	[STR_AS_OUT_ALT0].s = "Playback Inactive",
	[STR_AS_OUT_ALT1].s = "Playback 2ch+16bit",
	[STR_AS_OUT_ALT2].s = "Playback 8ch+16bit",
	[STR_AS_IN_ALT0].s = "Capture Inactive",
	[STR_AS_IN_ALT1].s = "Capture 2ch+16bit",
	[STR_AS_IN_ALT2].s = "Capture 8ch+16bit",
	{ },
};

static struct usb_gadget_strings str_fn = {
	.language = 0x0409,	/* en-us */
	.strings = strings_fn,
};

static struct usb_gadget_strings *fn_strings[] = {
	&str_fn,
	NULL,
};

static struct usb_qualifier_descriptor devqual_desc = {
	.bLength = sizeof devqual_desc,
	.bDescriptorType = USB_DT_DEVICE_QUALIFIER,

	.bcdUSB = cpu_to_le16(0x200),
	.bDeviceClass = USB_CLASS_MISC,
	.bDeviceSubClass = 0x02,
	.bDeviceProtocol = 0x01,
	.bNumConfigurations = 1,
	.bRESERVED = 0,
};

static struct usb_interface_assoc_descriptor iad_desc = {
	.bLength = sizeof iad_desc,
	.bDescriptorType = USB_DT_INTERFACE_ASSOCIATION,

	.bFirstInterface = 0,
	.bInterfaceCount = 3,
	.bFunctionClass = USB_CLASS_AUDIO,
	.bFunctionSubClass = UAC2_FUNCTION_SUBCLASS_UNDEFINED,
	.bFunctionProtocol = UAC_VERSION_2,
};

/* Audio Control Interface */
static struct usb_interface_descriptor std_ac_if_desc = {
	.bLength = sizeof std_ac_if_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOCONTROL,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Clock source for IN traffic */
static struct uac_clock_source_descriptor in_clk_src_desc = {
	.bLength = sizeof in_clk_src_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC2_CLOCK_SOURCE,
	.bClockID = USB_IN_CLK_ID,
	.bmAttributes = UAC_CLOCK_SOURCE_TYPE_INT_PROG,
	.bmControls = 7,
	.bAssocTerminal = 0,
};

/* Clock source for OUT traffic */
static struct uac_clock_source_descriptor out_clk_src_desc = {
	.bLength = sizeof out_clk_src_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC2_CLOCK_SOURCE,
	.bClockID = USB_OUT_CLK_ID,
	.bmAttributes = UAC_CLOCK_SOURCE_TYPE_INT_PROG,
	.bmControls = 7,
	.bAssocTerminal = 0,
};

/* Clock Selector for OUT traffic */
static struct uac_clock_selector_descriptor_3 out_clk_sel_desc = {
	.bLength = UAC_CLOCK_SELECTOR_DES_SIZE(3),
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC2_CLOCK_SELECTOR,
	.bClockID = USB_OUT_CLK_SEL_ID,
	.bNrInPins = 0x01,
	.baCSourceID[0] = USB_OUT_CLK_ID,
	.baCSourceID[1] = 0x3,
};

/* Clock Selector for IN traffic */
static struct uac_clock_selector_descriptor_3 in_clk_sel_desc = {
	.bLength = UAC_CLOCK_SELECTOR_DES_SIZE(3),
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC2_CLOCK_SELECTOR,
	.bClockID = USB_IN_CLK_SEL_ID,
	.bNrInPins = 0x01,
	.baCSourceID[0] = USB_IN_CLK_ID,
	.baCSourceID[1] = 0x3,
};

/* Input Terminal for USB_OUT */
static struct uac2_input_terminal_descriptor usb_out_it_desc = {
	.bLength = sizeof usb_out_it_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_INPUT_TERMINAL,
	.bTerminalID = USB_OUT_IT_ID,
	.wTerminalType = cpu_to_le16(UAC_INPUT_TERMINAL_UNDEFINED),
	.bAssocTerminal = 0,
	.bCSourceID = USB_OUT_CLK_SEL_ID,
	.iChannelNames = 0,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Input Terminal for I/O-In */
static struct uac2_input_terminal_descriptor io_in_it_desc = {
	.bLength = sizeof io_in_it_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_INPUT_TERMINAL,
	.bTerminalID = IO_IN_IT_ID,
	.wTerminalType = cpu_to_le16(UAC_INPUT_TERMINAL_UNDEFINED),
	.bAssocTerminal = 0,
	.bCSourceID = USB_OUT_CLK_SEL_ID,
	.iChannelNames = 0,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Ouput Terminal for USB_IN */
static struct uac2_output_terminal_descriptor usb_in_ot_desc = {
	.bLength = sizeof usb_in_ot_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
	.bTerminalID = USB_IN_OT_ID,
	.wTerminalType = cpu_to_le16(UAC_OUTPUT_TERMINAL_SPEAKER),
	.bAssocTerminal = 0,
	.bSourceID = USB_FEATURE_UNIT_ID,
	.bCSourceID = USB_IN_CLK_SEL_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

/* Ouput Terminal for I/O-Out */ //CANCEL
static struct uac2_output_terminal_descriptor io_out_ot_desc = {
	.bLength = sizeof io_out_ot_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
	.bTerminalID = IO_OUT_OT_ID,
	.wTerminalType = cpu_to_le16(UAC_OUTPUT_TERMINAL_UNDEFINED),
	.bAssocTerminal = 0,
	.bSourceID = USB_OUT_IT_ID,
	.bCSourceID = USB_IN_CLK_SEL_ID,
	.bmControls = (CONTROL_RDWR << COPY_CTRL),
};

static struct uac2_feature_unit_descriptor_13 funit_desc = {
	.bLength = UAC2_FEATURE_UNIT_DES_SIZE(13),
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FEATURE_UNIT,
	.bUnitID = USB_FEATURE_UNIT_ID,
	.bSourceID = USB_OUT_IT_ID,
	/* .bmaControls[]= */
	{0x0f, 0x00, 0x00, 0x00,
	 0x0f, 0x00, 0x00, 0x00,
	 0x0f, 0x00, 0x00, 0x00,
	 0x00}
	,			/* variable length */
};

static int generic_set_cmd(struct usb_audio_control *con, u8 cmd, int value)
{
	con->data[cmd] = value;

	return 0;
}

static int generic_get_cmd(struct usb_audio_control *con, u8 cmd)
{
	return con->data[cmd];
}

static struct usb_audio_control_selector feature_unit = {
	.list = LIST_HEAD_INIT(feature_unit.list),
	.id = USB_FEATURE_UNIT_ID,
	.name = "Mute & Volume Control",
	.type = UAC_FEATURE_UNIT,
	.desc = (struct usb_descriptor_header *)&funit_desc,
};

static struct usb_audio_control mute_control = {
	.list = LIST_HEAD_INIT(mute_control.list),
	.name = "Mute Control",
	.type = UAC_FU_MUTE,
	/* Todo: add real Mute control code */
	.set = generic_set_cmd,
	.get = generic_get_cmd,
};

static struct usb_audio_control volume_control = {
	.list = LIST_HEAD_INIT(volume_control.list),
	.name = "Volume Control",
	.type = UAC_FU_VOLUME,
	/* Todo: add real Volume control code */
	.set = generic_set_cmd,
	.get = generic_get_cmd,
};

static struct uac2_ac_header_descriptor ac_hdr_desc = {
	.bLength = sizeof ac_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_MS_HEADER,
	.bcdADC = cpu_to_le16(0x200),
	.bCategory = UAC2_FUNCTION_IO_BOX,
	.wTotalLength = sizeof in_clk_src_desc + sizeof out_clk_src_desc
			 + sizeof usb_out_it_desc + sizeof io_in_it_desc
			+ sizeof usb_in_ot_desc + sizeof ac_hdr_desc
			+ UAC2_FEATURE_UNIT_DES_SIZE(13) + UAC_CLOCK_SELECTOR_DES_SIZE(3)
			+ UAC_CLOCK_SELECTOR_DES_SIZE(3),
	.bmControls = 0,
};

/* Audio Streaming OUT Interface - Alt0 */
static struct usb_interface_descriptor std_as_out_if0_desc = {
	.bLength = sizeof std_as_out_if0_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming OUT Interface - Alt1 */
static struct usb_interface_descriptor std_as_out_if1_desc = {
	.bLength = sizeof std_as_out_if1_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 1,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming OUT Interface - Alt2 */
static struct usb_interface_descriptor std_as_out_if2_desc = {
	.bLength = sizeof std_as_out_if2_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 2,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream OUT Intface Desc */
static struct uac2_as_header_descriptor as_out_hdr_desc = {
	.bLength = sizeof as_out_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = 0,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

static struct uac2_as_header_descriptor as_out_hdr_desc_8 = {
	.bLength = sizeof as_out_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_OUT_IT_ID,
	.bmControls = 0,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

/* Audio USB_OUT Format */
static struct uac2_format_type_i_descriptor as_out_fmt1_desc = {
	.bLength = sizeof as_out_fmt1_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
};

/* STD AS ISO OUT Endpoint */
static struct usb_endpoint_descriptor fs_epout_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1023),
	.bInterval = 1,
};

/* STD AS ISO IN Endpoint */
struct usb_endpoint_descriptor fs_epin_fb_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_USAGE_FEEDBACK,
	.wMaxPacketSize = 4,
	.bInterval = 4,
};

static struct usb_endpoint_descriptor hs_epout_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1024),
	.bInterval = 4,
};

/* CS AS ISO OUT Endpoint */
static struct uac2_iso_endpoint_descriptor as_iso_out_desc = {
	.bLength = sizeof as_iso_out_desc,
	.bDescriptorType = USB_DT_CS_ENDPOINT,

	.bDescriptorSubtype = UAC_EP_GENERAL,
	.bmAttributes = 0,
	.bmControls = 0,
	.bLockDelayUnits = 0x02, //0,
	.wLockDelay = 0x08, //0,
};

/* Audio Streaming IN Interface - Alt0 */
static struct usb_interface_descriptor std_as_in_if0_desc = {
	.bLength = sizeof std_as_in_if0_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming IN Interface - Alt1 */
static struct usb_interface_descriptor std_as_in_if1_desc = {
	.bLength = sizeof std_as_in_if1_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 1,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Streaming IN Interface - Alt2 */
static struct usb_interface_descriptor std_as_in_if2_desc = {
	.bLength = sizeof std_as_in_if2_desc,
	.bDescriptorType = USB_DT_INTERFACE,

	.bAlternateSetting = 2,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_AUDIO,
	.bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
	.bInterfaceProtocol = UAC_VERSION_2,
};

/* Audio Stream IN Intface Desc */
static struct uac2_as_header_descriptor as_in_hdr_desc = {
	.bLength = sizeof as_in_hdr_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_IN_OT_ID,
	.bmControls = 0,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

/* Audio Stream IN Intface Desc */
static struct uac2_as_header_descriptor as_in_hdr_desc_8 = {
	.bLength = sizeof as_in_hdr_desc_8,
	.bDescriptorType = USB_DT_CS_INTERFACE,

	.bDescriptorSubtype = UAC_AS_GENERAL,
	.bTerminalLink = USB_IN_OT_ID,
	.bmControls = 0,
	.bFormatType = UAC_FORMAT_TYPE_I,
	.bmFormats = cpu_to_le32(UAC_FORMAT_TYPE_I_PCM),
	.iChannelNames = 0,
};

/* Audio USB_IN Format */
static struct uac2_format_type_i_descriptor as_in_fmt1_desc = {
	.bLength = sizeof as_in_fmt1_desc,
	.bDescriptorType = USB_DT_CS_INTERFACE,
	.bDescriptorSubtype = UAC_FORMAT_TYPE,
	.bFormatType = UAC_FORMAT_TYPE_I,
};

/* STD AS ISO IN Endpoint */
static struct usb_endpoint_descriptor fs_epin_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1023),
	.bInterval = 1,
};

static struct usb_endpoint_descriptor hs_epin_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_ASYNC,
	.wMaxPacketSize = cpu_to_le16(1024),
	.bInterval = 4,
};

/* CS AS ISO IN Endpoint */
static struct uac2_iso_endpoint_descriptor as_iso_in_desc = {
	.bLength = sizeof as_iso_in_desc,
	.bDescriptorType = USB_DT_CS_ENDPOINT,

	.bDescriptorSubtype = UAC_EP_GENERAL,
	.bmAttributes = 0,
	.bmControls = 0,
	.bLockDelayUnits = 0,
	.wLockDelay = 0,
};

static struct usb_descriptor_header *fs_audio_desc[] = {
	(struct usb_descriptor_header *)&iad_desc,
	(struct usb_descriptor_header *)&std_ac_if_desc,

	(struct usb_descriptor_header *)&ac_hdr_desc,
	(struct usb_descriptor_header *)&in_clk_src_desc,
	(struct usb_descriptor_header *)&in_clk_sel_desc,
	(struct usb_descriptor_header *)&out_clk_src_desc,
	(struct usb_descriptor_header *)&out_clk_sel_desc,
	(struct usb_descriptor_header *)&io_in_it_desc,
	(struct usb_descriptor_header *)&funit_desc,
	(struct usb_descriptor_header *)&usb_in_ot_desc,
	(struct usb_descriptor_header *)&usb_out_it_desc,
	//(struct usb_descriptor_header *)&io_out_ot_desc,

	(struct usb_descriptor_header *)&std_as_out_if0_desc,

	(struct usb_descriptor_header *)&std_as_out_if1_desc,
	(struct usb_descriptor_header *)&as_out_hdr_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,
	(struct usb_descriptor_header *)&fs_epin_fb_desc,

	(struct usb_descriptor_header *)&std_as_out_if2_desc,
	(struct usb_descriptor_header *)&as_out_hdr_desc_8,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,
	(struct usb_descriptor_header *)&fs_epin_fb_desc,

	(struct usb_descriptor_header *)&std_as_in_if0_desc,

	(struct usb_descriptor_header *)&std_as_in_if1_desc,
	(struct usb_descriptor_header *)&as_in_hdr_desc,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,

	(struct usb_descriptor_header *)&std_as_in_if2_desc,
	(struct usb_descriptor_header *)&as_in_hdr_desc_8,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&fs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,

	NULL,
};

static struct usb_descriptor_header *hs_audio_desc[] = {
	(struct usb_descriptor_header *)&iad_desc,
	(struct usb_descriptor_header *)&std_ac_if_desc,

	(struct usb_descriptor_header *)&ac_hdr_desc,
	(struct usb_descriptor_header *)&in_clk_src_desc,
	(struct usb_descriptor_header *)&in_clk_sel_desc,
	(struct usb_descriptor_header *)&out_clk_src_desc,
	(struct usb_descriptor_header *)&out_clk_sel_desc,
	(struct usb_descriptor_header *)&io_in_it_desc,
	(struct usb_descriptor_header *)&funit_desc,
	(struct usb_descriptor_header *)&usb_in_ot_desc,
	(struct usb_descriptor_header *)&usb_out_it_desc,
	//(struct usb_descriptor_header *)&io_out_ot_desc,

	(struct usb_descriptor_header *)&std_as_out_if0_desc,

	(struct usb_descriptor_header *)&std_as_out_if1_desc,
	(struct usb_descriptor_header *)&as_out_hdr_desc,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,
	(struct usb_descriptor_header *)&fs_epin_fb_desc,

	(struct usb_descriptor_header *)&std_as_out_if2_desc,
	(struct usb_descriptor_header *)&as_out_hdr_desc_8,
	(struct usb_descriptor_header *)&as_out_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epout_desc,
	(struct usb_descriptor_header *)&as_iso_out_desc,
	(struct usb_descriptor_header *)&fs_epin_fb_desc,

	(struct usb_descriptor_header *)&std_as_in_if0_desc,

	(struct usb_descriptor_header *)&std_as_in_if1_desc,
	(struct usb_descriptor_header *)&as_in_hdr_desc,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,

	(struct usb_descriptor_header *)&std_as_in_if2_desc,
	(struct usb_descriptor_header *)&as_in_hdr_desc_8,
	(struct usb_descriptor_header *)&as_in_fmt1_desc,
	(struct usb_descriptor_header *)&hs_epin_desc,
	(struct usb_descriptor_header *)&as_iso_in_desc,

	NULL,
};

struct cntrl_cur_lay3 {
	__u32	dCUR;
};

struct cntrl_range_lay3 {
	__u16	wNumSubRanges;
	__u32	dMIN;
	__u32	dMAX;
	__u32	dRES;
} __packed;

static inline void
free_ep(struct uac2_rtd_params *prm, struct usb_ep *ep)
{
	struct snd_uac2_chip *uac2 = prm->uac2;
	int i;

	if (!prm->ep_enabled)
		return;

	prm->ep_enabled = false;

	for (i = 0; i < USB_XFERS; i++) {
		if (prm->ureq[i].req) {
			usb_ep_dequeue(ep, prm->ureq[i].req);
			usb_ep_free_request(ep, prm->ureq[i].req);
			prm->ureq[i].req = NULL;
		}
	}

	if (usb_ep_disable(ep))
		dev_err(&uac2->pdev.dev,
			"%s:%d Error!\n", __func__, __LINE__);
}

static int
afunc_bind(struct usb_configuration *cfg, struct usb_function *fn)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_composite_dev *cdev = cfg->cdev;
	struct usb_gadget *gadget = cdev->gadget;
	struct device *dev = &uac2->pdev.dev;
	struct uac2_rtd_params *prm;
	struct f_uac2_hw_params *uac2_params;
	struct usb_string *us;
	int ret;

	uac2_params = &agdev->params;

	us = usb_gstrings_attach(cdev, fn_strings, ARRAY_SIZE(strings_fn));
	if (IS_ERR(us))
		return PTR_ERR(us);
	iad_desc.iFunction = us[STR_ASSOC].id;
	std_ac_if_desc.iInterface = us[STR_IF_CTRL].id;
	in_clk_src_desc.iClockSource = us[STR_CLKSRC_IN].id;
	out_clk_src_desc.iClockSource = us[STR_CLKSRC_OUT].id;
	usb_out_it_desc.iTerminal = us[STR_USB_IT].id;
	io_in_it_desc.iTerminal = us[STR_IO_IT].id;
	usb_in_ot_desc.iTerminal = us[STR_USB_OT].id;
	io_out_ot_desc.iTerminal = us[STR_IO_OT].id;
	std_as_out_if0_desc.iInterface = us[STR_AS_OUT_ALT0].id;
	std_as_out_if1_desc.iInterface = us[STR_AS_OUT_ALT1].id;
	std_as_out_if2_desc.iInterface = us[STR_AS_OUT_ALT2].id;
	std_as_in_if0_desc.iInterface = us[STR_AS_IN_ALT0].id;
	std_as_in_if1_desc.iInterface = us[STR_AS_IN_ALT1].id;
	std_as_in_if2_desc.iInterface = us[STR_AS_IN_ALT2].id;

	/* Initialize the configurable parameters */
	usb_out_it_desc.bNrChannels = num_channels(uac2_params->c_chmask);
	usb_out_it_desc.bmChannelConfig = cpu_to_le32(uac2_params->c_chmask);
	io_in_it_desc.bNrChannels = num_channels(uac2_params->p_chmask);
	io_in_it_desc.bmChannelConfig = cpu_to_le32(uac2_params->p_chmask);
	as_out_hdr_desc.bNrChannels = num_channels(uac2_params->c_chmask);
	as_out_hdr_desc.bmChannelConfig = cpu_to_le32(uac2_params->c_chmask);
	as_out_hdr_desc_8.bNrChannels = num_channels(0xff);
	as_out_hdr_desc_8.bmChannelConfig = cpu_to_le32(0xff);
	as_in_hdr_desc.bNrChannels = num_channels(uac2_params->p_chmask);
	as_in_hdr_desc.bmChannelConfig = cpu_to_le32(uac2_params->p_chmask);
	as_in_hdr_desc_8.bNrChannels = num_channels(0xff);
	as_in_hdr_desc_8.bmChannelConfig = cpu_to_le32((0xff));
	as_out_fmt1_desc.bSubslotSize = uac2_params->c_ssize;
	as_out_fmt1_desc.bBitResolution = uac2_params->c_ssize * 8;
	as_in_fmt1_desc.bSubslotSize = uac2_params->p_ssize;
	as_in_fmt1_desc.bBitResolution = uac2_params->p_ssize * 8;

	snprintf(clksrc_in, sizeof(clksrc_in), "%uHz", uac2_params->p_srate);
	snprintf(clksrc_out, sizeof(clksrc_out), "%uHz", uac2_params->c_srate);

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		return ret;
	}
	std_ac_if_desc.bInterfaceNumber = ret;
	agdev->ac_intf = ret;
	agdev->ac_alt = 0;

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		return ret;
	}
	std_as_out_if0_desc.bInterfaceNumber = ret;
	std_as_out_if1_desc.bInterfaceNumber = ret;
	std_as_out_if2_desc.bInterfaceNumber = ret;
	agdev->as_out_intf = ret;
	agdev->as_out_alt = 0;

	ret = usb_interface_id(cfg, fn);
	if (ret < 0) {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		return ret;
	}
	std_as_in_if0_desc.bInterfaceNumber = ret;
	std_as_in_if1_desc.bInterfaceNumber = ret;
	std_as_in_if2_desc.bInterfaceNumber = ret;
	agdev->as_in_intf = ret;
	agdev->as_in_alt = 0;

	agdev->out_ep = usb_ep_autoconfig(gadget, &fs_epout_desc);
	if (!agdev->out_ep) {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		goto err;
	}

	agdev->in_ep = usb_ep_autoconfig(gadget, &fs_epin_desc);
	if (!agdev->in_ep) {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		goto err;
	}

	uac2->p_prm.uac2 = uac2;
	uac2->c_prm.uac2 = uac2;

	agdev->fb_ep = usb_ep_autoconfig(gadget, &fs_epin_fb_desc);

	hs_epout_desc.bEndpointAddress = fs_epout_desc.bEndpointAddress;
	hs_epin_desc.bEndpointAddress = fs_epin_desc.bEndpointAddress;

	ret = usb_assign_descriptors(fn, fs_audio_desc, hs_audio_desc, NULL);
	if (ret)
		goto err;

	prm = &agdev->uac2.c_prm;
	prm->max_psize = hs_epout_desc.wMaxPacketSize;
	prm->rbuf = kzalloc(prm->max_psize * USB_XFERS, GFP_KERNEL);
	if (!prm->rbuf) {
		prm->max_psize = 0;
		goto err_free_descs;
	}

	prm = &agdev->uac2.p_prm;
	prm->max_psize = hs_epin_desc.wMaxPacketSize;
	prm->rbuf = kzalloc(prm->max_psize * USB_XFERS, GFP_KERNEL);
	if (!prm->rbuf) {
		prm->max_psize = 0;
		goto err_free_descs;
	}

	ret = alsa_uac2_init(agdev);
	if (ret)
		goto err_free_descs;
	return 0;

err_free_descs:
	usb_free_all_descriptors(fn);
err:
	kfree(agdev->uac2.p_prm.rbuf);
	kfree(agdev->uac2.c_prm.rbuf);
	return -EINVAL;
}

static void
afunc_alt_match_params(struct f_uac2_alt *value, u8 alt, u8 dir)
{
	if (dir == ALT_OUT) {
		switch(alt) {
			case 0: value->chmask = 0; value->slot = 0; break;
			case 1: value->chmask = 0x03; value->slot = 2; break;
			case 2: value->chmask = 0xff; value->slot = 2; break;
			default: 
				pr_err("uac2 alt error %d, dir %d!\n", alt, dir);
				break;
		}
	} else if (dir == ALT_IN) {
		switch(alt) {
			case 0: value->chmask = 0; value->slot = 0; break;
			case 1: value->chmask = 0x03; value->slot = 2; break;
			case 2: value->chmask = 0xff; value->slot = 2; break;
			default: 
				pr_err("uac2 alt error %d, dir %d!\n", alt, dir);
				break;
		}
	} else {
		pr_err("uac2 alt %d, dir error %d!\n", alt, dir);
	}
}

static void
afunc_update_hw_params(struct audio_dev *agdev, u8 alt, u8 dir)
{
	struct f_uac2_hw_params *hwparams = &agdev->params;
	struct f_uac2_alt value;

	memset(&value, 0, sizeof(struct f_uac2_alt));

	afunc_alt_match_params(&value, alt, dir);

	if (dir == ALT_OUT) {
		hwparams->c_chmask = value.chmask;
		hwparams->c_ssize = value.slot;
	} else if (dir == ALT_IN) {
		hwparams->p_chmask = value.chmask;
		hwparams->p_ssize = value.slot;
	} else {
		pr_err("uac2 alt %d, dir error!!! %d!\n", alt, dir);
	}
}

static int
afunc_set_alt(struct usb_function *fn, unsigned intf, unsigned alt)
{
	struct usb_composite_dev *cdev = fn->config->cdev;
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_gadget *gadget = cdev->gadget;
	struct device *dev = &uac2->pdev.dev;
	struct usb_request *req;
	struct usb_ep *ep;
	struct uac2_rtd_params *prm;
	int req_len, i;

	pr_info("uac2 intf %d, alt %d\n", intf, alt);

	if (intf == agdev->ac_intf) {
		/* Control I/f has only 1 AltSetting - 0 */
		if (alt) {
			dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
			return -EINVAL;
		}
		return 0;
	}

	if (intf == agdev->as_out_intf) {
		agdev->gspeed= gadget->speed;
		ep = agdev->fb_ep;
		config_ep_by_speed(gadget, fn, ep);
		ep = agdev->out_ep;
		prm = &uac2->c_prm;
		config_ep_by_speed(gadget, fn, ep);
		agdev->as_out_alt = alt;
		req_len = prm->max_psize;
		afunc_update_hw_params(agdev, alt, ALT_OUT);
	} else if (intf == agdev->as_in_intf) {
		unsigned int factor, rate;
		struct usb_endpoint_descriptor *ep_desc;

		ep = agdev->in_ep;
		prm = &uac2->p_prm;
		config_ep_by_speed(gadget, fn, ep);
		agdev->as_in_alt = alt;
		afunc_update_hw_params(agdev, alt, ALT_IN);

		/* pre-calculate the playback endpoint's interval */
		if (gadget->speed == USB_SPEED_FULL) {
			ep_desc = &fs_epin_desc;
			factor = 1000;
		} else {
			ep_desc = &hs_epin_desc;
			factor = 8000;
		}

		/* pre-compute some values for iso_complete() */
		uac2->p_framesize = agdev->params.p_ssize *
				    num_channels(agdev->params.p_chmask);
		rate = agdev->params.p_srate * uac2->p_framesize;
		uac2->p_interval = factor / (1 << (ep_desc->bInterval - 1));
		uac2->p_pktsize = min_t(unsigned int, rate / uac2->p_interval,
					prm->max_psize);

		if (uac2->p_pktsize < prm->max_psize)
			uac2->p_pktsize_residue = rate % uac2->p_interval;
		else
			uac2->p_pktsize_residue = 0;

		req_len = uac2->p_pktsize;
		uac2->p_residue = 0;
	} else {
		dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
		return -EINVAL;
	}

	if (alt == 0) {
		free_ep(prm, ep);
		if (intf == agdev->as_out_intf)
			usb_ep_disable(agdev->fb_ep);
		return 0;
	}

	prm->ep_enabled = true;
	usb_ep_enable(ep);
	if (intf == agdev->as_out_intf)
		usb_ep_enable(agdev->fb_ep);

	for (i = 0; i < USB_XFERS; i++) {
		if (!prm->ureq[i].req) {
			req = usb_ep_alloc_request(ep, GFP_ATOMIC);
			if (req == NULL)
				return -ENOMEM;

			prm->ureq[i].req = req;
			prm->ureq[i].pp = prm;

			req->zero = 0;
			req->context = &prm->ureq[i];
			req->length = req_len;
			req->complete = agdev_iso_complete;
			req->buf = prm->rbuf + i * prm->max_psize;
		} else {
			continue;
		}

		if (usb_ep_queue(ep, prm->ureq[i].req, GFP_ATOMIC))
			dev_err(dev, "%s:%d Error!\n", __func__, __LINE__);
	}

	return 0;
}

static int
afunc_get_alt(struct usb_function *fn, unsigned intf)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;

	if (intf == agdev->ac_intf)
		return agdev->ac_alt;
	else if (intf == agdev->as_out_intf)
		return agdev->as_out_alt;
	else if (intf == agdev->as_in_intf)
		return agdev->as_in_alt;
	else
		dev_err(&uac2->pdev.dev,
			"%s:%d Invalid Interface %d!\n",
			__func__, __LINE__, intf);

	return -EINVAL;
}

static void
afunc_disable(struct usb_function *fn)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;

	free_ep(&uac2->p_prm, agdev->in_ep);
	agdev->as_in_alt = 0;

	free_ep(&uac2->c_prm, agdev->out_ep);
	agdev->as_out_alt = 0;

	free_ep(&uac2->c_prm, agdev->fb_ep);
}

static void
uac2_set_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_uac2_hw_params *hwparams = &agdev->params;
	struct usb_ctrlrequest *cr;
	u32 data = 0;
	u16 w_index;
	u8 entity_id;

	if (req->context != NULL) {
		cr = req->context;
		w_index = le16_to_cpu(cr->wIndex);
		entity_id = (w_index >> 8) & 0xff;
		if (entity_id == USB_IN_CLK_ID) {
			memcpy(&hwparams->p_srate, req->buf, req->actual);
			pr_info("host in [0x%x]\n", hwparams->p_srate);
			kfree(req->context);
			req->context = NULL;
		} else if (entity_id == USB_OUT_CLK_ID) {
			memcpy(&hwparams->c_srate, req->buf, req->actual);
			pr_info("host out [0x%x]\n", hwparams->c_srate);
			kfree(req->context);
			req->context = NULL;
		}
	} else if (agdev->set_con) {
		memcpy(&data, req->buf, req->length);
		agdev->set_con->set(agdev->set_con, agdev->set_cmd,
				le16_to_cpu(data));
		agdev->set_con = NULL;
	}
}

static int
in_rq_cur(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_request *req = fn->config->cdev->req;
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_index = le16_to_cpu(cr->wIndex);
	u16 w_value = le16_to_cpu(cr->wValue);
	u8 entity_id = (w_index >> 8) & 0xff;
	u8 control_selector = w_value >> 8;
	u8 cmd = (cr->bRequest & 0x0F);
	int value = -EOPNOTSUPP;
	int p_srate, c_srate;

	p_srate = agdev->params.p_srate;
	c_srate = agdev->params.c_srate;

	list_for_each_entry(cs, &agdev->cs, list) {
		if (cs->id == entity_id) {
			list_for_each_entry(con, &cs->control, list) {
				if (con->type == control_selector && con->get) {
					value = con->get(con, cmd);
					memcpy(req->buf, &value, w_length);
				}
			}
		}
		break;
	}

	if (value != -EOPNOTSUPP) {
		req->complete = uac2_set_complete;
		req->context = NULL;
		value = w_length;
		return value;
	} 

	if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
		struct cntrl_cur_lay3 c;
		memset(&c, 0, sizeof(struct cntrl_cur_lay3));

		if (entity_id == USB_IN_CLK_ID)
			c.dCUR = p_srate;
		else if (entity_id == USB_OUT_CLK_ID)
			c.dCUR = c_srate;
		else if ((entity_id == USB_OUT_CLK_SEL_ID) ||
				(entity_id == USB_IN_CLK_SEL_ID)) {
			*(u8 *) req->buf = 1;
			value = min_t(unsigned, w_length, 1);
			return value;
		} else {
			return value;
		}

		value = min_t(unsigned, w_length, sizeof c);
		memcpy(req->buf, &c, value);
	} else if (control_selector == UAC2_CS_CONTROL_CLOCK_VALID) {
		*(u8 *)req->buf = 1;
		value = min_t(unsigned, w_length, 1);
	} else {
		dev_err(&uac2->pdev.dev,
			"%s:%d control_selector=%d TODO!\n",
			__func__, __LINE__, control_selector);
	}

	return value;
}

static int
in_rq_range(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_request *req = fn->config->cdev->req;
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_value = le16_to_cpu(cr->wValue);
	u8 entity_id = ((le16_to_cpu(cr->wIndex) >> 8) & 0xFF);
	u8 control_selector = w_value >> 8;
	u8 cmd = (cr->bRequest & 0x0F);
	u8 i;
	u16 *tmp;
	int value = -EOPNOTSUPP;

	if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
		if (agdev->gspeed == USB_SPEED_HIGH) {
			value = min_t(unsigned, w_length, sizeof(r_high));
			memcpy(req->buf, &r_high, value);
		} else {
			value = min_t(unsigned, w_length, sizeof(r_full));
			memcpy(req->buf, &r_full, value);
		}
	} else {
		list_for_each_entry(cs, &agdev->cs, list) {
			if (cs->id == entity_id) {
				list_for_each_entry(con, &cs->control, list) {
					if (con->type == control_selector && con->get) {
						tmp = req->buf;
						cmd = 1;
						for (i = 0; i < 4; i++, cmd++, tmp++) {
							value = con->get(con, cmd);
							pr_err("cmd[%d] value[0x%x], w_length[0x%x], sizeof[%d]\n", (int)cmd, value, w_length, (int)sizeof(u16));
							memcpy(tmp, &value, sizeof(u16));
						}
					}
				}
				break;
			}
		}
	
		if (value != -EOPNOTSUPP) {
			req->complete = uac2_set_complete;
			req->context = NULL;
			value = w_length;
		} else {
			dev_err(&uac2->pdev.dev,
				"%s:%d control_selector=%d TODO!\n",
				__func__, __LINE__, control_selector);
		}
	}

	return value;
}

static int
ac_rq_in(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	if (cr->bRequest == UAC2_CS_CUR)
		return in_rq_cur(fn, cr);
	else if (cr->bRequest == UAC2_CS_RANGE)
		return in_rq_range(fn, cr);
	else
		return -EOPNOTSUPP;
}

static int
out_rq_cur(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct usb_request *req = fn->config->cdev->req;
	struct usb_audio_control_selector *cs;
	struct usb_audio_control *con;
	u16 w_length = le16_to_cpu(cr->wLength);
	u16 w_value = le16_to_cpu(cr->wValue);
	u16 w_index = le16_to_cpu(cr->wIndex);
	u8 entity_id = (w_index >> 8) & 0xff;
	u8 control_selector = w_value >> 8;
	u8 cmd = (cr->bRequest & 0x0F);

	list_for_each_entry(cs, &agdev->cs, list) {
		if (cs->id == entity_id) {
			list_for_each_entry(con, &cs->control, list) {
				if (con->type == control_selector) {
					agdev->set_con = con;
					break;
				}
			}
			break;
		}
	}

	if (agdev->set_con != NULL) {
		agdev->set_cmd = cmd;
		req->complete = uac2_set_complete;
		req->context = NULL;
		return w_length;
	}

	if (control_selector == UAC2_CS_CONTROL_SAM_FREQ) {
		if ((cr->bRequestType & (0x01 << 7)) == USB_DIR_OUT)	{
			if ((entity_id == USB_OUT_CLK_ID) ||
				(entity_id == USB_IN_CLK_ID)) {
				pr_info("uac2 host %s sample rate:",
					(entity_id == USB_OUT_CLK_ID) ? "playback" : "capture");
				req->context = kzalloc(sizeof(struct usb_ctrlrequest), GFP_ATOMIC);
				if (!req->context) {
					pr_err("alloc error %s\n", __func__);
					return -ENOMEM;
				}
				memcpy(req->context, cr, sizeof(struct usb_ctrlrequest));
				req->complete = uac2_set_complete;
			}
		}
		return w_length;
	}

	return -EOPNOTSUPP;
}

static int
setup_rq_inf(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	u16 w_index = le16_to_cpu(cr->wIndex);
	u8 intf = w_index & 0xff;

	if (intf != agdev->ac_intf) {
		dev_err(&uac2->pdev.dev,
			"%s:%d Error!\n", __func__, __LINE__);
		return -EOPNOTSUPP;
	}

	if (cr->bRequestType & USB_DIR_IN)
		return ac_rq_in(fn, cr);
	else if (cr->bRequest == UAC2_CS_CUR)
		return out_rq_cur(fn, cr);

	return -EOPNOTSUPP;
}

static int
afunc_setup(struct usb_function *fn, const struct usb_ctrlrequest *cr)
{
	struct usb_composite_dev *cdev = fn->config->cdev;
	struct audio_dev *agdev = func_to_agdev(fn);
	struct snd_uac2_chip *uac2 = &agdev->uac2;
	struct usb_request *req = cdev->req;
	u16 w_length = le16_to_cpu(cr->wLength);
	int value = -EOPNOTSUPP;

	/* Only Class specific requests are supposed to reach here */
	if ((cr->bRequestType & USB_TYPE_MASK) != USB_TYPE_CLASS)
		return -EOPNOTSUPP;

	if ((cr->bRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)
		value = setup_rq_inf(fn, cr);
	else
		dev_err(&uac2->pdev.dev, "%s:%d Error!\n", __func__, __LINE__);

	if (value >= 0) {
		req->length = value;
		req->zero = value < w_length;
		value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
		if (value < 0) {
			dev_err(&uac2->pdev.dev,
				"%s:%d Error!\n", __func__, __LINE__);
			req->status = 0;
		}
	}

	return value;
}

static void afunc_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct audio_dev *agdev = func_to_agdev(f);
	struct uac2_rtd_params *prm;

	alsa_uac2_exit(agdev);

	prm = &agdev->uac2.p_prm;
	kfree(prm->rbuf);

	prm = &agdev->uac2.c_prm;
	kfree(prm->rbuf);
	usb_free_all_descriptors(f);
}

int control_selector_init(struct audio_dev *audio)
{
	INIT_LIST_HEAD(&audio->cs);
	list_add(&feature_unit.list, &audio->cs);

	INIT_LIST_HEAD(&feature_unit.control);
	list_add(&mute_control.list, &feature_unit.control);
	list_add(&volume_control.list, &feature_unit.control);

	volume_control.data[UAC__CUR] = 0x0001;
	volume_control.data[UAC__MIN] = 0x8100;
	volume_control.data[UAC__MAX] = 0x0000;
	volume_control.data[UAC__RES] = 0x0100;

	mute_control.data[0x1] = 0x00;

	return 0;
}

static int uac2_bind_config(struct usb_configuration *cfg)
{
	int res;

	agdev = kzalloc(sizeof(*agdev), GFP_KERNEL);
	if (agdev == NULL)
		return -ENOMEM;

	agdev->func.name = "uac2_func";
	agdev->func.bind = afunc_bind;
	agdev->func.unbind = afunc_unbind;
	agdev->func.set_alt = afunc_set_alt;
	agdev->func.get_alt = afunc_get_alt;
	agdev->func.disable = afunc_disable;
	agdev->func.setup = afunc_setup;

	agdev->params.p_chmask = UAC2_DEF_PCHMASK;
	agdev->params.p_srate = UAC2_DEF_PSRATE;
	agdev->params.p_ssize = UAC2_DEF_PSSIZE;
	agdev->params.c_chmask = UAC2_DEF_CCHMASK;
	agdev->params.c_srate = UAC2_DEF_CSRATE;
	agdev->params.c_ssize = UAC2_DEF_CSSIZE;

	control_selector_init(agdev);

	res = usb_add_function(cfg, &agdev->func);
	if (res < 0)
		kfree(agdev);

	return res;
}

static void uac2_unbind_config(struct usb_configuration *cfg)
{
	kfree(agdev);
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Android Composite USB Driver");

