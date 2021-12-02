/*
 *  Dummy soundcard
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/hrtimer.h>
#include <linux/math64.h>
#include <linux/moduleparam.h>
#include <sound/core.h>
#include <sound/control.h>
#include <sound/tlv.h>
#include <sound/pcm.h>
#include <sound/rawmidi.h>
#include <sound/info.h>
#include <sound/initval.h>

MODULE_AUTHOR("MTK");
MODULE_DESCRIPTION("MTK soundcard (/dev/dsp)");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("{{ALSA,MTK soundcard}}");

#define MAX_PCM_DEVICES		4
#define MAX_PCM_SUBSTREAMS	16 //MTK
#define MAX_MIDI_DEVICES	2

////////////////////////////////////////
//MTK ALSA Audio Interface
////////////////////////////////////////

#define ALSA_DBG_MSG 0
#define ALSA_DBG_MSG_MIXER 0

#define AUD_PLAYBACK_USE_MIXSND 1

int record_src = 1; // 0: Line-In, 1: Upload, 2: SBC

typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef signed int INT32;
typedef bool BOOL;

extern UINT32 u4Virtual(UINT32 u4Addr);

extern UINT32 u4GetAFIFOStart(UINT8 uDecIndex);
extern UINT32 u4GetAFIFOEnd(UINT8 uDecIndex);
extern UINT32 u4GetABufPnt(UINT8 uDecIndex);
extern UINT32 u4GetAWritePnt(UINT8 uDecIndex);
extern void vSetAWritePnt(UINT32 u4WritePointer);

extern void AUD_InitALSAPlayback_MixSnd(void);
extern void AUD_DeInitALSAPlayback_MixSnd(void);
extern UINT32 u4GetMixSndFIFOStart(void);
extern UINT32 u4GetMixSndFIFOEnd(void);
extern UINT32 u4GetMixSndReadPtr(void);
extern void vSetMixSndWritePtr(UINT32 u4WritePtr);

extern UINT32 u4GetSBCEncFIFOStart(void);
extern UINT32 u4GetSBCEncFIFOEnd(void);
extern UINT32 u4GetSBCEncWritePnt(void);

extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);

extern void AUD_InitALSAPlayback(void);
extern void AUD_DeInitALSAPlayback(void);
extern void AUD_InitALSARecordLineIn(void);
extern void AUD_DeInitALSARecordLineIn(void);
extern void AUD_InitALSARecordSpeaker(void);
extern void AUD_DeInitALSARecordSpeaker(void);
extern void AUD_InitALSARecordSBC(void);
extern void AUD_DeInitALSARecordSBC(void);
extern void AUD_PlayMixSndRingFifo(UINT32 u4SampleRate, UINT8 u1StereoOnOff, UINT8 u1BitDepth, UINT32 u4BufferSize);

#if AUD_PLAYBACK_USE_MIXSND
#define vInitAlsaPlayback() AUD_InitALSAPlayback_MixSnd()
#define vDeInitAlsaPlayback() AUD_DeInitALSAPlayback_MixSnd()
#define u4GetPlayBufSA() u4GetMixSndFIFOStart()
#define u4GetPlayBufEA() u4GetMixSndFIFOEnd()
#define u4GetPlayBufCA() u4GetMixSndReadPtr()
#define vSetPlayBufWA(wp)   vSetMixSndWritePtr(wp) 
#define vSetPlayParm(u4SampleRate,u1StereoOnOff,u1BitDepth,u4BufferSize) AUD_PlayMixSndRingFifo(u4SampleRate,u1StereoOnOff,u1BitDepth,u4BufferSize)
#else
#define vInitAlsaPlayback() AUD_InitALSAPlayback()
#define vDeInitAlsaPlayback() AUD_DeInitALSAPlayback()
#define u4GetPlayBufSA() u4GetAFIFOStart(0)
#define u4GetPlayBufEA() u4GetAFIFOEnd(0)
#define u4GetPlayBufCA() u4GetABufPnt(0)
#define vSetPlayBufWA(wp)   vSetAWritePnt(wp) 
#define vSetPlayParm(u4SampleRate,u1StereoOnOff,u1BitDepth,u4BufferSize)
#endif

#if 1
UINT32 u4GetRecBufSA(void)
{
    if (record_src == 1)
    {
        return u4GetUploadFIFOStart();
    }
    else if (record_src == 2)
    {
        return u4GetSBCEncFIFOStart();
    }
    else
    {
    #if 0
        //USe AFIFO1 for test
        return u4GetAFIFOStart(0);
    #else
        //USe AFIFO2 for test    
        return u4GetAFIFOStart(1);
    #endif    
    }
}

UINT32 u4GetRecBufEA(void)
{
    UINT32 u4FifoSA;
    UINT32 u4FifoSZ;
    if (record_src == 1)
    {
        u4FifoSA = u4GetUploadFIFOStart();
        u4FifoSZ = ((u4GetUploadFIFOEnd() - u4GetUploadFIFOStart())); //&0xffff0000); //size 4096 bytes aligned
    }
    else if (record_src == 2)
    {
        u4FifoSA = u4GetSBCEncFIFOStart();
        u4FifoSZ = ((u4GetSBCEncFIFOEnd() - u4GetSBCEncFIFOStart())); //&0xffff0000); //size 4096 bytes aligned
    }
    else
    {
    #if 0
        //USe AFIFO1 for test    
        u4FifoSA = u4GetAFIFOStart(0);
        u4FifoSZ = (u4GetAFIFOEnd(0) - u4GetAFIFOStart(0)); //size 4096 bytes aligned
    #else
        //USe AFIFO2 for test    
        u4FifoSA = u4GetAFIFOStart(1);
        u4FifoSZ = ((u4GetAFIFOEnd(1) - u4GetAFIFOStart(1))&0xffff0000); //size 4096 bytes aligned
    #endif
    }

    return u4FifoSA + u4FifoSZ;
}

UINT32 u4GetRecBufCA(void)
{
    if (record_src == 1)
    {
        return u4GetUploadWritePnt();
    }
    else if (record_src == 2)
    {
        return u4GetSBCEncWritePnt();
    }
    else
    {
        return u4GetAWritePnt(1);
    }
}

void AUD_InitALSARecord(void)
{
    if (record_src == 1)
    {
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ record_src - speaker\n"); //0513
        #endif    
        AUD_InitALSARecordSpeaker();
    }
    else if (record_src == 2)
    {
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ record_src - sbc\n"); //0513
        #endif        
        AUD_InitALSARecordSBC();
    }
    else
    {
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ record_src - line-in\n"); //0513
        #endif        
        AUD_InitALSARecordLineIn();
    }
}

void AUD_DeInitALSARecord(void)
{
    if (record_src == 1)
    {
        AUD_DeInitALSARecordSpeaker();
    }
    else if (record_src == 2)
    {
        AUD_DeInitALSARecordSBC();
    }
    else
    {
        AUD_DeInitALSARecordLineIn();
    }
}
#else
#define u4GetRecBufSA() u4GetAFIFOStart(1)
#define u4GetRecBufEA() u4GetAFIFOEnd(1)
#define u4GetRecBufCA() u4GetAWritePnt(1)

#define AUD_InitALSARecord() AUD_InitALSARecordLineIn()
#define AUD_DeInitALSARecord() AUD_DeInitALSARecordLineIn()
#endif

////////////////////////////////////////
//End
////////////////////////////////////////

/* defaults */
#define MAX_BUFFER_SIZE		(64*1024)
#define MIN_PERIOD_SIZE		64
#define MAX_PERIOD_SIZE		MAX_BUFFER_SIZE
#define USE_FORMATS 		SNDRV_PCM_FMTBIT_S16_LE
#define USE_RATE                (SNDRV_PCM_RATE_CONTINUOUS | SNDRV_PCM_RATE_8000_48000) //SNDRV_PCM_RATE_48000
#define USE_RATE_MIN		8000
#define USE_RATE_MAX		48000
#define USE_CHANNELS_MIN 	1
#define USE_CHANNELS_MAX 	2
#define USE_PERIODS_MIN 	1
#define USE_PERIODS_MAX 	1024

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;	/* Index 0-MAX */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;	/* ID for this card */
static int enable[SNDRV_CARDS] = {1, [1 ... (SNDRV_CARDS - 1)] = 0};
static char *model[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = NULL};
static int pcm_devs[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 1};
static int pcm_substreams[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 8};
//static int midi_devs[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 2};
#ifdef CONFIG_HIGH_RES_TIMERS
static int hrtimer = 0;
#endif
static int fake_buffer = 0; //MTK

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for dummy soundcard.");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for dummy soundcard.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable this dummy soundcard.");
module_param_array(model, charp, NULL, 0444);
MODULE_PARM_DESC(model, "Soundcard model.");
module_param_array(pcm_devs, int, NULL, 0444);
MODULE_PARM_DESC(pcm_devs, "PCM devices # (0-4) for dummy driver.");
module_param_array(pcm_substreams, int, NULL, 0444);
MODULE_PARM_DESC(pcm_substreams, "PCM substreams # (1-16) for dummy driver."); //MTK
//module_param_array(midi_devs, int, NULL, 0444);
//MODULE_PARM_DESC(midi_devs, "MIDI devices # (0-2) for dummy driver.");
module_param(fake_buffer, bool, 0444);
MODULE_PARM_DESC(fake_buffer, "Fake buffer allocations.");
#ifdef CONFIG_HIGH_RES_TIMERS
module_param(hrtimer, bool, 0644);
MODULE_PARM_DESC(hrtimer, "Use hrtimer as the timer source.");
#endif

static struct platform_device *devices[SNDRV_CARDS];

#define MIXER_ADDR_MASTER	0
#define MIXER_ADDR_LINE		1
#define MIXER_ADDR_MIC		2
#define MIXER_ADDR_SYNTH	3
#define MIXER_ADDR_CD		4
#define MIXER_ADDR_LAST		4

struct dummy_timer_ops 
{
	int (*create)(struct snd_pcm_substream *);
	void (*free)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
	int (*start)(struct snd_pcm_substream *);
	int (*stop)(struct snd_pcm_substream *);
	snd_pcm_uframes_t (*pointer)(struct snd_pcm_substream *);
};

struct snd_dummy 
{
	struct snd_card *card;
	struct snd_pcm *pcm;
	struct snd_pcm_hardware pcm_hw;
	spinlock_t mixer_lock;
	int mixer_volume[MIXER_ADDR_LAST+1][2];
	int capture_source[MIXER_ADDR_LAST+1][2];
	const struct dummy_timer_ops *timer_ops;
};

/*
 * system timer interface
 */
struct dummy_systimer_pcm 
{
	spinlock_t lock;
	struct timer_list timer;
	unsigned long base_time;
	unsigned int frac_pos;	/* fractional sample position (based HZ) */
	unsigned int frac_period_rest;
	unsigned int frac_buffer_size;	/* buffer_size * HZ */
	unsigned int frac_period_size;	/* period_size * HZ */
	unsigned int rate;
	int elapsed;
	struct snd_pcm_substream *substream;
};

static void dummy_systimer_rearm(struct dummy_systimer_pcm *dpcm)
{
	dpcm->timer.expires = jiffies +
		(dpcm->frac_period_rest + dpcm->rate - 1) / dpcm->rate;
	add_timer(&dpcm->timer);
}

static void dummy_systimer_update(struct dummy_systimer_pcm *dpcm)
{
	unsigned long delta;

	delta = jiffies - dpcm->base_time;
	if (!delta)
		return;
	dpcm->base_time += delta;
	delta *= dpcm->rate;
	dpcm->frac_pos += delta;
	while (dpcm->frac_pos >= dpcm->frac_buffer_size)
		dpcm->frac_pos -= dpcm->frac_buffer_size;
	while (dpcm->frac_period_rest <= delta) {
		dpcm->elapsed++;
		dpcm->frac_period_rest += dpcm->frac_period_size;
	}
	dpcm->frac_period_rest -= delta;
}

static int dummy_systimer_start(struct snd_pcm_substream *substream)
{
	struct dummy_systimer_pcm *dpcm = substream->runtime->private_data;
	spin_lock(&dpcm->lock);
	dpcm->base_time = jiffies;
	dummy_systimer_rearm(dpcm);
	spin_unlock(&dpcm->lock);
	return 0;
}

static int dummy_systimer_stop(struct snd_pcm_substream *substream)
{
	struct dummy_systimer_pcm *dpcm = substream->runtime->private_data;
	spin_lock(&dpcm->lock);
	del_timer(&dpcm->timer);
	spin_unlock(&dpcm->lock);
	return 0;
}

static int dummy_systimer_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct dummy_systimer_pcm *dpcm = runtime->private_data;

	dpcm->frac_pos = 0;
	dpcm->rate = runtime->rate;
	dpcm->frac_buffer_size = runtime->buffer_size * HZ;
	dpcm->frac_period_size = runtime->period_size * HZ;
	dpcm->frac_period_rest = dpcm->frac_period_size;
	dpcm->elapsed = 0;

	return 0;
}

static void dummy_systimer_callback(unsigned long data)
{
	struct dummy_systimer_pcm *dpcm = (struct dummy_systimer_pcm *)data;
	unsigned long flags;
	int elapsed = 0;
	
	spin_lock_irqsave(&dpcm->lock, flags);
	dummy_systimer_update(dpcm);
	dummy_systimer_rearm(dpcm);
	elapsed = dpcm->elapsed;
	dpcm->elapsed = 0;
	spin_unlock_irqrestore(&dpcm->lock, flags);
	if (elapsed)
		snd_pcm_period_elapsed(dpcm->substream);
}

static snd_pcm_uframes_t
dummy_systimer_pointer(struct snd_pcm_substream *substream)
{
	struct dummy_systimer_pcm *dpcm = substream->runtime->private_data;
	snd_pcm_uframes_t pos;

	spin_lock(&dpcm->lock);
	dummy_systimer_update(dpcm);
	pos = dpcm->frac_pos / HZ;
	spin_unlock(&dpcm->lock);
	return pos;
}

static int dummy_systimer_create(struct snd_pcm_substream *substream)
{
	struct dummy_systimer_pcm *dpcm;

	dpcm = kzalloc(sizeof(*dpcm), GFP_KERNEL);
	if (!dpcm)
		return -ENOMEM;
	substream->runtime->private_data = dpcm;
	init_timer(&dpcm->timer);
	dpcm->timer.data = (unsigned long) dpcm;
	dpcm->timer.function = dummy_systimer_callback;
	spin_lock_init(&dpcm->lock);
	dpcm->substream = substream;
	return 0;
}

static void dummy_systimer_free(struct snd_pcm_substream *substream)
{
	kfree(substream->runtime->private_data);
}

static struct dummy_timer_ops dummy_systimer_ops = {
	.create =	dummy_systimer_create,
	.free =		dummy_systimer_free,
	.prepare =	dummy_systimer_prepare,
	.start =	dummy_systimer_start,
	.stop =		dummy_systimer_stop,
	.pointer =	dummy_systimer_pointer,
};

#ifdef CONFIG_HIGH_RES_TIMERS
/*
 * hrtimer interface
 */

struct dummy_hrtimer_pcm {
	ktime_t base_time;
	ktime_t period_time;
	atomic_t running;
	struct hrtimer timer;
	struct tasklet_struct tasklet;
	struct snd_pcm_substream *substream;
};

static void dummy_hrtimer_pcm_elapsed(unsigned long priv)
{
	struct dummy_hrtimer_pcm *dpcm = (struct dummy_hrtimer_pcm *)priv;
	if (atomic_read(&dpcm->running))
		snd_pcm_period_elapsed(dpcm->substream);
}

static enum hrtimer_restart dummy_hrtimer_callback(struct hrtimer *timer)
{
	struct dummy_hrtimer_pcm *dpcm;

	dpcm = container_of(timer, struct dummy_hrtimer_pcm, timer);
	if (!atomic_read(&dpcm->running))
		return HRTIMER_NORESTART;
	tasklet_schedule(&dpcm->tasklet);
	hrtimer_forward_now(timer, dpcm->period_time);
	return HRTIMER_RESTART;
}

static int dummy_hrtimer_start(struct snd_pcm_substream *substream)
{
	struct dummy_hrtimer_pcm *dpcm = substream->runtime->private_data;

	dpcm->base_time = hrtimer_cb_get_time(&dpcm->timer);
	hrtimer_start(&dpcm->timer, dpcm->period_time, HRTIMER_MODE_REL);
	atomic_set(&dpcm->running, 1);
	return 0;
}

static int dummy_hrtimer_stop(struct snd_pcm_substream *substream)
{
	struct dummy_hrtimer_pcm *dpcm = substream->runtime->private_data;

	atomic_set(&dpcm->running, 0);
	hrtimer_cancel(&dpcm->timer);
	return 0;
}

static inline void dummy_hrtimer_sync(struct dummy_hrtimer_pcm *dpcm)
{
	tasklet_kill(&dpcm->tasklet);
}

static snd_pcm_uframes_t
dummy_hrtimer_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct dummy_hrtimer_pcm *dpcm = runtime->private_data;
	u64 delta;
	u32 pos;

	delta = ktime_us_delta(hrtimer_cb_get_time(&dpcm->timer),
			       dpcm->base_time);
	delta = div_u64(delta * runtime->rate + 999999, 1000000);
	div_u64_rem(delta, runtime->buffer_size, &pos);
	return pos;
}

static int dummy_hrtimer_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct dummy_hrtimer_pcm *dpcm = runtime->private_data;
	unsigned int period, rate;
	long sec;
	unsigned long nsecs;

	dummy_hrtimer_sync(dpcm);
	period = runtime->period_size;
	rate = runtime->rate;
	sec = period / rate;
	period %= rate;
	nsecs = div_u64((u64)period * 1000000000UL + rate - 1, rate);
	dpcm->period_time = ktime_set(sec, nsecs);

	return 0;
}

static int dummy_hrtimer_create(struct snd_pcm_substream *substream)
{
	struct dummy_hrtimer_pcm *dpcm;

	dpcm = kzalloc(sizeof(*dpcm), GFP_KERNEL);
	if (!dpcm)
		return -ENOMEM;
	substream->runtime->private_data = dpcm;
	hrtimer_init(&dpcm->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	dpcm->timer.function = dummy_hrtimer_callback;
	dpcm->substream = substream;
	atomic_set(&dpcm->running, 0);
	tasklet_init(&dpcm->tasklet, dummy_hrtimer_pcm_elapsed,
		     (unsigned long)dpcm);
	return 0;
}

static void dummy_hrtimer_free(struct snd_pcm_substream *substream)
{
	struct dummy_hrtimer_pcm *dpcm = substream->runtime->private_data;
	dummy_hrtimer_sync(dpcm);
	kfree(dpcm);
}

static struct dummy_timer_ops dummy_hrtimer_ops = {
	.create =	dummy_hrtimer_create,
	.free =		dummy_hrtimer_free,
	.prepare =	dummy_hrtimer_prepare,
	.start =	dummy_hrtimer_start,
	.stop =		dummy_hrtimer_stop,
	.pointer =	dummy_hrtimer_pointer,
};

#endif /* CONFIG_HIGH_RES_TIMERS */

/*
 * PCM interface
 */

static int dummy_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);

    switch (cmd) 
    {
    case SNDRV_PCM_TRIGGER_START:
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_pcm_trigger - start\n"); //0513
        #endif
        return dummy->timer_ops->start(substream);
    case SNDRV_PCM_TRIGGER_RESUME:
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_pcm_trigger - resume\n"); //0513
        #endif
        return dummy->timer_ops->start(substream);
    case SNDRV_PCM_TRIGGER_STOP:
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_pcm_trigger - stop\n"); //0513
        #endif
        return dummy->timer_ops->stop(substream);
    case SNDRV_PCM_TRIGGER_SUSPEND:
        #if ALSA_DBG_MSG
        printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_pcm_trigger - suspend\n"); //0513                       
        #endif
        return dummy->timer_ops->stop(substream);
    }
	return -EINVAL;    
}

//MTK
UINT32 aapl_ptr = 0;
UINT32 arp = 0;
int repeat_cnt = 0;
//MTK

static int dummy_pcm_playback_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
	UINT8 u1StereoOnOff = 0;

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_playback_pcm_prepare\n"); //0513
    #endif

	if(2 == runtime->channels)
	{
		u1StereoOnOff = 1;
	}
	else
	{
		u1StereoOnOff = 0;
	}

    vSetPlayParm(runtime->rate, u1StereoOnOff, 16, snd_pcm_lib_buffer_bytes(substream));

    if (runtime->dma_area == 0)
    {
        runtime->dma_area = (unsigned char*)u4Virtual(u4GetPlayBufSA());
        runtime->dma_addr = 0;
        runtime->dma_bytes = snd_pcm_lib_buffer_bytes(substream);
    }

    printk(KERN_ERR "format: %d rate: %d channels: %d\n", runtime->format, runtime->rate, runtime->channels);
    printk(KERN_ERR "runtime->buffer_size: %x\n", (unsigned int)(runtime->buffer_size));
    printk(KERN_ERR "runtime->period_size: %x\n", (unsigned int)(runtime->period_size));
    //printk(KERN_ERR "dpcm->pcm_bps: %x\n", dpcm->pcm_bps);
    //printk(KERN_ERR "dpcm->pcm_hz: %x\n", dpcm->pcm_hz);
    printk(KERN_ERR "snd_pcm_lib_buffer_bytes(substream): %x\n", snd_pcm_lib_buffer_bytes(substream));
    printk(KERN_ERR "snd_pcm_lib_period_bytes(substream): %x\n", snd_pcm_lib_period_bytes(substream));
    #if ALSA_DBG_MSG
    printk(KERN_ERR "runtime->dma_area: 0x%08x\n", runtime->dma_area);
    printk(KERN_ERR "runtime->dma_addr: 0x%08x\n", runtime->dma_addr);
    printk(KERN_ERR "runtime->dma_bytes: 0x%08x\n", runtime->dma_bytes);
    #endif

    aapl_ptr = 0;
    arp = 0;
    repeat_cnt = 0;

	return dummy->timer_ops->prepare(substream);
}

static int dummy_pcm_capture_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_record_pcm_prepare\n"); //0513
    #if 0
    if (dpcm->dummy)
    {
        printk(KERN_ERR "  RECSRC MASTER: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_MASTER][0]);
        printk(KERN_ERR "  RECSRC LINE: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_LINE][0]);
        printk(KERN_ERR "  RECSRC MIC: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_MIC][0]);
        printk(KERN_ERR "  RECSRC SYNTH: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_SYNTH][0]);
        printk(KERN_ERR "  RECSRC CD: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_LAST][0]);
    }
    else
    {
        printk(KERN_ERR "  dpcm->dummy is null pointer !!!\n");
    }
    #endif
    #endif

    //snd_pcm_format_set_silence(runtime->format, runtime->dma_area, bytes_to_samples(runtime, runtime->dma_bytes));
    if (runtime->dma_area == 0)
    {
        runtime->dma_area = (unsigned char*)(u4Virtual(u4GetRecBufSA()));
        runtime->dma_addr = 0;
        runtime->dma_bytes = u4GetRecBufEA() - u4GetRecBufSA();

        AUD_InitALSARecord();
    }

	return dummy->timer_ops->prepare(substream);
}

//MTK
#define FIRST_RECORD_DBG 0

#if FIRST_RECORD_DBG
int first_record = 0;
#endif
//MTK

static snd_pcm_uframes_t dummy_pcm_playback_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

#if 1
    UINT32 u4Start = u4GetPlayBufSA();
    UINT32 u4ABufPnt = u4GetPlayBufCA();
    UINT32 u4WritePnt;
    if(2 == runtime->channels)
    {
        u4WritePnt = u4Start + (((runtime->control->appl_ptr * 4) % snd_pcm_lib_buffer_bytes(substream))&0xffffff00);
    }
    else
    {
        u4WritePnt = u4Start + (((runtime->control->appl_ptr * 2) % snd_pcm_lib_buffer_bytes(substream))&0xffffff00);
    }

    if (runtime->control->appl_ptr == aapl_ptr)
    {
        repeat_cnt++;
        if (repeat_cnt == 2)
        {
            if(2 == runtime->channels)
            {
                //u4WritePnt = u4Start + ((((runtime->control->appl_ptr-0x100) * 4) % snd_pcm_lib_buffer_bytes(substream))&0xffffff00);
            }
            else
            {
                //u4WritePnt = u4Start + ((((runtime->control->appl_ptr-0x100) * 2) % snd_pcm_lib_buffer_bytes(substream))&0xffffff00);
            }
            repeat_cnt = 0;
            //printk(KERN_ERR "u4ABufPnt: 0x%08x appl_ptr: 0x%08x\n", u4ABufPnt, runtime->control->appl_ptr);            
        }        
    }
    else
    {
        repeat_cnt = 0;
    }
    aapl_ptr = runtime->control->appl_ptr;
    
  #if AUD_PLAYBACK_USE_MIXSND
    if (u4ABufPnt == u4WritePnt)
    {
        //printk(KERN_ERR "u4ABufPnt: 0x%08x", u4WritePnt);
    }
    else
    {
        vSetPlayBufWA(u4WritePnt);    
    }
  #else
    vSetPlayBufWA(u4WritePnt);      
    if ((aapl_ptr == runtime->control->appl_ptr) && (arp == u4ABufPnt))
    {
        repeat_cnt++;
        if (repeat_cnt > 3)
        {
            #if ALSA_DBG_MSG
            printk(KERN_ERR "aapl_ptr: 0x%08x arp: 0x%08x\n", aapl_ptr, arp);            
            #endif
            //u4ABufPnt += 0x400;
            u4ABufPnt = u4Start + ((runtime->control->appl_ptr * 4) % snd_pcm_lib_buffer_bytes(substream));
        }
    }
    else
    {
        aapl_ptr = runtime->control->appl_ptr;
        arp = u4ABufPnt;
        repeat_cnt = 0;
    } 
  #endif
    {
      #if 0
        if(2 == runtime->channels)
        {   
            return ((u4ABufPnt - u4Start)>>2);
        }
        else
        {
            return ((u4ABufPnt - u4Start)>>1);
        }
      #else
        struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
        return dummy->timer_ops->pointer(substream);
      #endif        
    }    
#else
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);

	return dummy->timer_ops->pointer(substream);
#endif
}

static snd_pcm_uframes_t dummy_pcm_capture_pointer(struct snd_pcm_substream *substream)
{
#if 1
	struct snd_pcm_runtime *runtime = substream->runtime;

    UINT32 u4Start = u4GetRecBufSA();
    UINT32 u4ABufPnt = u4GetRecBufCA();
    //printk(KERN_ERR "SA: 0x%08x WP: 0x%08x\n", u4Start, u4ABufPnt);
    #if FIRST_RECORD_DBG
    if (!first_record)
    {
        UINT32 ptr;
        
        printk(KERN_ERR "u4Start: 0x%08x u4ABufPnt: 0x%08x\n", u4Start, u4ABufPnt);  
        ptr = u4Virtual(u4Start);
        printk(KERN_ERR "%08x %08x %08x %08x\n", *((UINT32*)ptr), *((UINT32*)(ptr+4)), *((UINT32*)(ptr+8)), *((UINT32*)(ptr+12)));
        ptr = u4Virtual(u4ABufPnt);
        printk(KERN_ERR "%08x %08x %08x %08x\n", *((UINT32*)ptr), *((UINT32*)(ptr+4)), *((UINT32*)(ptr+8)), *((UINT32*)(ptr+12)));

        first_record = 1;
    }
    #endif
    
    {
      #if 1
        if(2 == runtime->channels)
        {   
            return ((u4ABufPnt - u4Start)>>2);
        }
        else
        {
            return ((u4ABufPnt - u4Start)>>1);        
        }
      #else
    	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);    
    	return dummy->timer_ops->pointer(substream);
      #endif
    }
#else
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);

	return dummy->timer_ops->pointer(substream);
#endif
}

static struct snd_pcm_hardware dummy_pcm_hardware = {
	.info =			(SNDRV_PCM_INFO_MMAP |
				 SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_RESUME |
				 SNDRV_PCM_INFO_MMAP_VALID),
	.formats =		USE_FORMATS,
	.rates =		USE_RATE,
	.rate_min =		USE_RATE_MIN,
	.rate_max =		USE_RATE_MAX,
	.channels_min =		USE_CHANNELS_MIN,
	.channels_max =		USE_CHANNELS_MAX,
	.buffer_bytes_max =	MAX_BUFFER_SIZE,
	.period_bytes_min =	MIN_PERIOD_SIZE,
	.period_bytes_max =	MAX_PERIOD_SIZE,
	.periods_min =		USE_PERIODS_MIN,
	.periods_max =		USE_PERIODS_MAX,
	.fifo_size =		0,
};

static struct snd_pcm_hardware dummy_pcm_playback_hardware = {
	.info =			(SNDRV_PCM_INFO_MMAP |
				 SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_RESUME |
				 SNDRV_PCM_INFO_MMAP_VALID),
	.formats =		USE_FORMATS,
	.rates =		USE_RATE,
	.rate_min =		USE_RATE_MIN,
	.rate_max =		USE_RATE_MAX,
	.channels_min =		USE_CHANNELS_MIN,
	.channels_max =		USE_CHANNELS_MAX,
	.buffer_bytes_max =	MAX_BUFFER_SIZE,
	.period_bytes_min =	MIN_PERIOD_SIZE,
	.period_bytes_max =	MAX_PERIOD_SIZE,
	.periods_min =		USE_PERIODS_MIN,
	.periods_max =		USE_PERIODS_MAX,
	.fifo_size =		0,
};

static struct snd_pcm_hardware dummy_pcm_capture_hardware = {
	.info =			(SNDRV_PCM_INFO_MMAP |
				 SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_RESUME |
				 SNDRV_PCM_INFO_MMAP_VALID),
	.formats =		USE_FORMATS,
	.rates =		USE_RATE,
	.rate_min =		USE_RATE_MIN,
	.rate_max =		USE_RATE_MAX,
	.channels_min =		USE_CHANNELS_MIN,
	.channels_max =		USE_CHANNELS_MAX,
	.buffer_bytes_max =	MAX_BUFFER_SIZE,
	.period_bytes_min =	MIN_PERIOD_SIZE,
	.period_bytes_max =	MAX_PERIOD_SIZE,
	.periods_min =		USE_PERIODS_MIN,
	.periods_max =		USE_PERIODS_MAX,
	.fifo_size =		0,
};

static int dummy_pcm_hw_params(struct snd_pcm_substream *substream,
			       struct snd_pcm_hw_params *hw_params)
{
    int ret;
    //struct snd_pcm_runtime *runtime = substream->runtime;    

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_hw_params\n"); //0513
    printk(KERN_ERR "params_buffer_bytes(hw_params): 0x%08x\n", params_buffer_bytes(hw_params));
    //printk(KERN_ERR "[Before]\n");
    //printk(KERN_ERR "runtime->dma_area: 0x%08x\n", (UINT32)(runtime->dma_area));
    //printk(KERN_ERR "runtime->dma_addr: 0x%08x\n", (UINT32)(runtime->dma_addr));
    //printk(KERN_ERR "runtime->dma_bytes: 0x%08x\n", (UINT32)(runtime->dma_bytes));
    #endif

#if 1
    ret = 0;
#else
	if (fake_buffer) {
		/* runtime->dma_bytes has to be set manually to allow mmap */
		substream->runtime->dma_bytes = params_buffer_bytes(hw_params);
		ret = 0;
	}
    else
    {
	  ret = snd_pcm_lib_malloc_pages(substream,	params_buffer_bytes(hw_params));
    } 
#endif
    #if ALSA_DBG_MSG
    //printk(KERN_ERR "[After] ret: %d\n", ret);
    //printk(KERN_ERR "runtime->dma_area: 0x%08x\n", (UINT32)(runtime->dma_area));
    //printk(KERN_ERR "runtime->dma_addr: 0x%08x\n", (UINT32)(runtime->dma_addr));
    //printk(KERN_ERR "runtime->dma_bytes: 0x%08x\n", (UINT32)(runtime->dma_bytes));   
    {
        int i;
        printk(KERN_ERR "flags: %08x\n", hw_params->flags);
        for (i=SNDRV_PCM_HW_PARAM_FIRST_MASK;i<=SNDRV_PCM_HW_PARAM_LAST_MASK;i++)
            printk(KERN_ERR "masks[%d]: %08x\n", i, hw_params->masks[i].bits[0]);
        for (i=SNDRV_PCM_HW_PARAM_FIRST_INTERVAL;i<=SNDRV_PCM_HW_PARAM_LAST_INTERVAL;i++)
            printk(KERN_ERR "internals[%d]: (%d,%d) omin=%d omax=%d int=%d empty=%d\n",
                            i,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].min,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].max,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].openmin,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].openmax,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].integer,
                            hw_params->intervals[i-SNDRV_PCM_HW_PARAM_FIRST_INTERVAL].empty);
        printk(KERN_ERR "rmask: %08x\n", hw_params->rmask);
        printk(KERN_ERR "cmask: %08x\n", hw_params->cmask);
        printk(KERN_ERR "info: %08x\n", hw_params->info);
        printk(KERN_ERR "msbits: %d\n", hw_params->msbits);
        printk(KERN_ERR "rate: %d/%d\n", hw_params->rate_num,hw_params->rate_den);
        printk(KERN_ERR "fifo: %d\n", hw_params->fifo_size);
    }
    #endif

    return ret;
}

static int dummy_pcm_hw_free(struct snd_pcm_substream *substream)
{
    int ret;
    struct snd_pcm_runtime *runtime = substream->runtime;

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_hw_free\n"); //0513
    printk(KERN_ERR "[Before]\n");
    printk(KERN_ERR "runtime->dma_area: 0x%08x\n", (UINT32)(runtime->dma_area));
    printk(KERN_ERR "runtime->dma_addr: 0x%08x\n", (UINT32)(runtime->dma_addr));
    printk(KERN_ERR "runtime->dma_bytes: 0x%08x\n", (UINT32)(runtime->dma_bytes));
    #endif

#if 1
    ret = 0;
    if (runtime->dma_area)
    {
        runtime->dma_area = 0;
        runtime->dma_addr = 0;
        runtime->dma_bytes = 0;
    }
#else
	if (fake_buffer)
		ret = 0;
    else
        ret = snd_pcm_lib_free_pages(substream);
#endif

    #if ALSA_DBG_MSG
    printk(KERN_ERR "[After] ret: %d\n", ret);
    printk(KERN_ERR "runtime->dma_area: 0x%08x\n", (UINT32)(runtime->dma_area));
    printk(KERN_ERR "runtime->dma_addr: 0x%08x\n", (UINT32)(runtime->dma_addr));
    printk(KERN_ERR "runtime->dma_bytes: 0x%08x\n", (UINT32)(runtime->dma_bytes));
    #endif
    return ret;
}

static int dummy_pcm_playback_open(struct snd_pcm_substream *substream)
{
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;    
	int err;

	dummy->timer_ops = &dummy_systimer_ops;
#ifdef CONFIG_HIGH_RES_TIMERS
	if (hrtimer)
		dummy->timer_ops = &dummy_hrtimer_ops;
#endif

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_playback_open\n"); //0513
    printk(KERN_ERR "  substream->number: %d\n", substream->number);
    printk(KERN_ERR "  substream->stream: %d\n", substream->stream);
    printk(KERN_ERR "AFIFO start addr: 0x%08x\n", u4GetPlayBufSA());
    printk(KERN_ERR "AFIFO end addr: 0x%08x\n", u4GetPlayBufEA());
    printk(KERN_ERR "[VIRTUAL ADDRESS]\n");
    printk(KERN_ERR "AFIFO start addr: 0x%08x\n", u4Virtual(u4GetPlayBufSA()));
    printk(KERN_ERR "AFIFO end addr: 0x%08x\n", u4Virtual(u4GetPlayBufEA()));
    #endif

    vInitAlsaPlayback();

    dummy_pcm_playback_hardware.buffer_bytes_max = u4GetPlayBufEA() - u4GetPlayBufSA();
    dummy_pcm_playback_hardware.period_bytes_max = u4GetPlayBufEA() - u4GetPlayBufSA();
    dummy_pcm_hardware.buffer_bytes_max = u4GetPlayBufEA() - u4GetPlayBufSA();
    dummy_pcm_hardware.period_bytes_max = u4GetPlayBufEA() - u4GetPlayBufSA();

	err = dummy->timer_ops->create(substream);
	if (err < 0)
		return err;

	//runtime->hw = dummy->pcm_hw;
    runtime->hw = dummy_pcm_playback_hardware; //MTK

	if (substream->pcm->device & 1) {
		runtime->hw.info &= ~SNDRV_PCM_INFO_INTERLEAVED;
		runtime->hw.info |= SNDRV_PCM_INFO_NONINTERLEAVED;
	}
	if (substream->pcm->device & 2)
		runtime->hw.info &= ~(SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID);

	return 0;
}

static int dummy_pcm_capture_open(struct snd_pcm_substream *substream)
{
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	int err;

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_capture_open\n"); //0513
    printk(KERN_ERR "  substream->number: %d\n", substream->number);
    printk(KERN_ERR "  substream->stream: %d\n", substream->stream);    
    printk(KERN_ERR "[PHYSICAL ADDRESS]\n");
    printk(KERN_ERR "AFIFO start addr: 0x%08x\n", u4GetRecBufSA());
    printk(KERN_ERR "AFIFO end addr: 0x%08x\n", u4GetRecBufEA());
    printk(KERN_ERR "[VIRTUAL ADDRESS]\n");
    printk(KERN_ERR "AFIFO start addr: 0x%08x\n", u4Virtual(u4GetRecBufSA()));
    printk(KERN_ERR "AFIFO end addr: 0x%08x\n", u4Virtual(u4GetRecBufEA()));
    #if 0
    if (dpcm->dummy)
    {
        printk(KERN_ERR "  RECSRC MASTER: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_MASTER][0]);
        printk(KERN_ERR "  RECSRC LINE: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_LINE][0]);
        printk(KERN_ERR "  RECSRC MIC: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_MIC][0]);
        printk(KERN_ERR "  RECSRC SYNTH: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_SYNTH][0]);
        printk(KERN_ERR "  RECSRC CD: %d\n", dpcm->dummy->capture_source[MIXER_ADDR_CD][0]);
        if (dpcm->dummy->capture_source[MIXER_ADDR_CD][0])
        {
            record_src = 2; //SBC
        }
        else if (dpcm->dummy->capture_source[MIXER_ADDR_SYNTH])
        {
            record_src = 1; //Upload
        }
        else
        {
            record_src = 0; //line-in
        }
    }
    else
    {
        printk(KERN_ERR "  dpcm->dummy is null pointer !!!\n");
    }
    #endif
    #endif

    //AUD_InitALSARecord();

    dummy_pcm_capture_hardware.buffer_bytes_max = u4GetRecBufEA() - u4GetRecBufSA();
    dummy_pcm_capture_hardware.period_bytes_max = u4GetRecBufEA() - u4GetRecBufSA();

    #if FIRST_RECORD_DBG
    first_record = 0;
    #endif

	dummy->timer_ops = &dummy_systimer_ops;
#ifdef CONFIG_HIGH_RES_TIMERS
	if (hrtimer)
		dummy->timer_ops = &dummy_hrtimer_ops;
#endif

	err = dummy->timer_ops->create(substream);
	if (err < 0)
		return err;

	//runtime->hw = dummy->pcm_hw;
	runtime->hw = dummy_pcm_capture_hardware; //MTK
	if (substream->pcm->device & 1) {
		runtime->hw.info &= ~SNDRV_PCM_INFO_INTERLEAVED;
		runtime->hw.info |= SNDRV_PCM_INFO_NONINTERLEAVED;
	}
	if (substream->pcm->device & 2)
		runtime->hw.info &= ~(SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID);

	return 0;
}

static int dummy_pcm_playback_close(struct snd_pcm_substream *substream)
{
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
	dummy->timer_ops->free(substream);

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_playback_close\n"); //0513
    #endif

    vDeInitAlsaPlayback();

	return 0;
}

static int dummy_pcm_capture_close(struct snd_pcm_substream *substream)
{
	struct snd_dummy *dummy = snd_pcm_substream_chip(substream);
	dummy->timer_ops->free(substream);

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_capture_close\n"); //0513    
    #endif

    AUD_DeInitALSARecord();

	return 0;
}

/*
 * dummy buffer handling
 */

static void *dummy_page[2];

static void free_fake_buffer(void)
{
	if (fake_buffer) {
		int i;
		for (i = 0; i < 2; i++)
			if (dummy_page[i]) {
				free_page((unsigned long)dummy_page[i]);
				dummy_page[i] = NULL;
			}
	}
}

static int alloc_fake_buffer(void)
{
	int i;

	if (!fake_buffer)
		return 0;
	for (i = 0; i < 2; i++) {
		dummy_page[i] = (void *)get_zeroed_page(GFP_KERNEL);
		if (!dummy_page[i]) {
			free_fake_buffer();
			return -ENOMEM;
		}
	}
	return 0;
}

static int dummy_pcm_copy(struct snd_pcm_substream *substream,
			  int channel, snd_pcm_uframes_t pos,
			  void __user *dst, snd_pcm_uframes_t count)
{
	return 0; /* do nothing */
}

static int dummy_pcm_silence(struct snd_pcm_substream *substream,
			     int channel, snd_pcm_uframes_t pos,
			     snd_pcm_uframes_t count)
{
	return 0; /* do nothing */
}

static struct page *dummy_pcm_page(struct snd_pcm_substream *substream,
				   unsigned long offset)
{
	return virt_to_page(dummy_page[substream->stream]); /* the same page */
}

static struct snd_pcm_ops dummy_pcm_playback_ops = {
	.open =		dummy_pcm_playback_open,
	.close =	dummy_pcm_playback_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	dummy_pcm_hw_params,
	.hw_free =	dummy_pcm_hw_free,
	.prepare =	dummy_pcm_playback_prepare,
	.trigger =	dummy_pcm_trigger,
	.pointer =	dummy_pcm_playback_pointer,
};

static struct snd_pcm_ops dummy_pcm_capture_ops = {
	.open =		dummy_pcm_capture_open,
	.close =	dummy_pcm_capture_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	dummy_pcm_hw_params,
	.hw_free =	dummy_pcm_hw_free,
	.prepare =	dummy_pcm_capture_prepare,
	.trigger =	dummy_pcm_trigger,
	.pointer =	dummy_pcm_capture_pointer,
};

static struct snd_pcm_ops dummy_pcm_playback_ops_no_buf = {
	.open =		dummy_pcm_playback_open,
	.close =	dummy_pcm_playback_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	dummy_pcm_hw_params,
	.hw_free =	dummy_pcm_hw_free,
	.prepare =	dummy_pcm_playback_prepare,
	.trigger =	dummy_pcm_trigger,
	.pointer =	dummy_pcm_playback_pointer,
	.copy =		dummy_pcm_copy,
	.silence =	dummy_pcm_silence,
	.page =		dummy_pcm_page,
};

static struct snd_pcm_ops dummy_pcm_capture_ops_no_buf = {
	.open =		dummy_pcm_capture_open,
	.close =	dummy_pcm_capture_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	dummy_pcm_hw_params,
	.hw_free =	dummy_pcm_hw_free,
	.prepare =	dummy_pcm_capture_prepare,
	.trigger =	dummy_pcm_trigger,
	.pointer =	dummy_pcm_capture_pointer,
	.copy =		dummy_pcm_copy,
	.silence =	dummy_pcm_silence,
	.page =		dummy_pcm_page,
};

static int __devinit snd_card_dummy_pcm(struct snd_dummy *dummy, int device,
					int substreams)
{
	struct snd_pcm *pcm;
	struct snd_pcm_ops *playback_ops;
    struct snd_pcm_ops *capture_ops;
	int err;

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_pcm\n");
    #endif

	err = snd_pcm_new(dummy->card, "Dummy PCM", device,
			       substreams, substreams, &pcm);
	if (err < 0)
		return err;
	dummy->pcm = pcm;
	if (fake_buffer)
    {
		playback_ops = &dummy_pcm_playback_ops_no_buf;
        capture_ops = &dummy_pcm_capture_ops_no_buf;
    }
	else
    {
		playback_ops = &dummy_pcm_playback_ops;
		capture_ops = &dummy_pcm_capture_ops;
    }
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, playback_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, capture_ops);
	pcm->private_data = dummy;
	pcm->info_flags = 0;
	strcpy(pcm->name, "Dummy PCM");
	if (!fake_buffer) 
    {
		snd_pcm_lib_preallocate_pages_for_all(pcm,
			SNDRV_DMA_TYPE_CONTINUOUS,
			snd_dma_continuous_data(GFP_KERNEL),
			0, 64*1024);
	}
	return 0;
}

/*
 * mixer interface
 */

#define DUMMY_VOLUME(xname, xindex, addr) \
{ .iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
  .access = SNDRV_CTL_ELEM_ACCESS_READWRITE | SNDRV_CTL_ELEM_ACCESS_TLV_READ, \
  .name = xname, .index = xindex, \
  .info = snd_dummy_volume_info, \
  .get = snd_dummy_volume_get, .put = snd_dummy_volume_put, \
  .private_value = addr, \
  .tlv = { .p = db_scale_dummy } }

static int snd_dummy_volume_info(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_info *uinfo)
{
#if ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# snd_dummy_volume_info\n");
#endif

	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 2;
	uinfo->value.integer.min = -50;
	uinfo->value.integer.max = 100;
	return 0;
}
 
static int snd_dummy_volume_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_dummy *dummy = snd_kcontrol_chip(kcontrol);
	int addr = kcontrol->private_value;

#if ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# snd_dummy_volume_get (addr=%d)\n", addr);
    printk(KERN_ERR "!@#!@# dummy->mixer_volume[%d][0]: %d\n", addr, dummy->mixer_volume[addr][0]);
    printk(KERN_ERR "!@#!@# dummy->mixer_volume[%d][1]: %d\n", addr, dummy->mixer_volume[addr][1]);
#endif

	spin_lock_irq(&dummy->mixer_lock);
	ucontrol->value.integer.value[0] = dummy->mixer_volume[addr][0];
	ucontrol->value.integer.value[1] = dummy->mixer_volume[addr][1];
	spin_unlock_irq(&dummy->mixer_lock);
	return 0;
}

static int snd_dummy_volume_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_dummy *dummy = snd_kcontrol_chip(kcontrol);
	int change, addr = kcontrol->private_value;
	int left, right;

#if ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# snd_dummy_volume_put (addr=%d)\n", addr);
    printk(KERN_ERR "!@#!@# ucontrol->value.integer.value[0]: %d\n", ucontrol->value.integer.value[0]);
    printk(KERN_ERR "!@#!@# ucontrol->value.integer.value[1]: %d\n", ucontrol->value.integer.value[1]);
#endif

	left = ucontrol->value.integer.value[0];
	if (left < -50)
		left = -50;
	if (left > 100)
		left = 100;
	right = ucontrol->value.integer.value[1];
	if (right < -50)
		right = -50;
	if (right > 100)
		right = 100;
	spin_lock_irq(&dummy->mixer_lock);
	change = dummy->mixer_volume[addr][0] != left ||
	         dummy->mixer_volume[addr][1] != right;
	dummy->mixer_volume[addr][0] = left;
	dummy->mixer_volume[addr][1] = right;
	spin_unlock_irq(&dummy->mixer_lock);
	return change;
}

static const DECLARE_TLV_DB_SCALE(db_scale_dummy, -4500, 30, 0);

#define DUMMY_CAPSRC(xname, xindex, addr) \
{ .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, .index = xindex, \
  .info = snd_dummy_capsrc_info, \
  .get = snd_dummy_capsrc_get, .put = snd_dummy_capsrc_put, \
  .private_value = addr }

#define snd_dummy_capsrc_info	snd_ctl_boolean_stereo_info
 
static int snd_dummy_capsrc_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_dummy *dummy = snd_kcontrol_chip(kcontrol);
	int addr = kcontrol->private_value;

#if ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# snd_dummy_capsrc_get (addr=%d)\n", addr);
    printk(KERN_ERR "!@#!@# dummy->capture_source[%d][0]: %d\n", addr, dummy->capture_source[addr][0]);
    printk(KERN_ERR "!@#!@# dummy->capture_source[%d][1]: %d\n", addr, dummy->capture_source[addr][1]);
#endif

	spin_lock_irq(&dummy->mixer_lock);
	ucontrol->value.integer.value[0] = dummy->capture_source[addr][0];
	ucontrol->value.integer.value[1] = dummy->capture_source[addr][1];
	spin_unlock_irq(&dummy->mixer_lock);
	return 0;
}

static int snd_dummy_capsrc_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_dummy *dummy = snd_kcontrol_chip(kcontrol);
	int change, addr = kcontrol->private_value;
	int left, right;

#if ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# snd_dummy_capsrc_put (addr=%d)\n",addr);
    printk(KERN_ERR "!@#!@# ucontrol->value.integer.value[0]: %d\n", ucontrol->value.integer.value[0]);
    printk(KERN_ERR "!@#!@# ucontrol->value.integer.value[1]: %d\n", ucontrol->value.integer.value[1]);
#endif

	left = ucontrol->value.integer.value[0] & 1;
	right = ucontrol->value.integer.value[1] & 1;
	spin_lock_irq(&dummy->mixer_lock);
	change = dummy->capture_source[addr][0] != left &&
	         dummy->capture_source[addr][1] != right;
	dummy->capture_source[addr][0] = left;
	dummy->capture_source[addr][1] = right;
    //TEST
    if (dummy->capture_source[MIXER_ADDR_CD][0])
    {
        record_src = 2; //SBC
    }
    else if (dummy->capture_source[MIXER_ADDR_SYNTH][0])
    {
        record_src = 1; //SPEAKER
    }
    else
    {
        record_src = 0; //LINE-IN
    }
    //TEST
	spin_unlock_irq(&dummy->mixer_lock);
	return change;
}

static struct snd_kcontrol_new snd_dummy_controls[] = 
{
DUMMY_VOLUME("Master Volume", 0, MIXER_ADDR_MASTER),
DUMMY_CAPSRC("Master Capture Switch", 0, MIXER_ADDR_MASTER),
DUMMY_VOLUME("Synth Volume", 0, MIXER_ADDR_SYNTH),
DUMMY_CAPSRC("Synth Capture Switch", 0, MIXER_ADDR_SYNTH),
DUMMY_VOLUME("Line Volume", 0, MIXER_ADDR_LINE),
DUMMY_CAPSRC("Line Capture Switch", 0, MIXER_ADDR_LINE),
DUMMY_VOLUME("Mic Volume", 0, MIXER_ADDR_MIC),
DUMMY_CAPSRC("Mic Capture Switch", 0, MIXER_ADDR_MIC),
DUMMY_VOLUME("CD Volume", 0, MIXER_ADDR_CD),
DUMMY_CAPSRC("CD Capture Switch", 0, MIXER_ADDR_CD)
};

static int __devinit snd_card_dummy_new_mixer(struct snd_dummy *dummy)
{
	struct snd_card *card = dummy->card;
	unsigned int idx;
	int err;

    #if ALSA_DBG_MSG || ALSA_DBG_MSG_MIXER
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_card_dummy_new_mixer\n");
    #endif

	spin_lock_init(&dummy->mixer_lock);
	strcpy(card->mixername, "Dummy Mixer");

	for (idx = 0; idx < ARRAY_SIZE(snd_dummy_controls); idx++) 
    {
		err = snd_ctl_add(card, snd_ctl_new1(&snd_dummy_controls[idx], dummy));
		if (err < 0)
			return err;
	}
	return 0;
}

#if defined(CONFIG_SND_DEBUG) && defined(CONFIG_PROC_FS)
/*
 * proc interface
 */
static void print_formats(struct snd_dummy *dummy,
			  struct snd_info_buffer *buffer)
{
	int i;

	for (i = 0; i < SNDRV_PCM_FORMAT_LAST; i++) {
		if (dummy->pcm_hw.formats & (1ULL << i))
			snd_iprintf(buffer, " %s", snd_pcm_format_name(i));
	}
}

static void print_rates(struct snd_dummy *dummy,
			struct snd_info_buffer *buffer)
{
	static int rates[] = {
		5512, 8000, 11025, 16000, 22050, 32000, 44100, 48000,
		64000, 88200, 96000, 176400, 192000,
	};
	int i;

	if (dummy->pcm_hw.rates & SNDRV_PCM_RATE_CONTINUOUS)
		snd_iprintf(buffer, " continuous");
	if (dummy->pcm_hw.rates & SNDRV_PCM_RATE_KNOT)
		snd_iprintf(buffer, " knot");
	for (i = 0; i < ARRAY_SIZE(rates); i++)
		if (dummy->pcm_hw.rates & (1 << i))
			snd_iprintf(buffer, " %d", rates[i]);
}

#define get_dummy_int_ptr(dummy, ofs) \
	(unsigned int *)((char *)&((dummy)->pcm_hw) + (ofs))
#define get_dummy_ll_ptr(dummy, ofs) \
	(unsigned long long *)((char *)&((dummy)->pcm_hw) + (ofs))

struct dummy_hw_field {
	const char *name;
	const char *format;
	unsigned int offset;
	unsigned int size;
};
#define FIELD_ENTRY(item, fmt) {		   \
	.name = #item,				   \
	.format = fmt,				   \
	.offset = offsetof(struct snd_pcm_hardware, item), \
	.size = sizeof(dummy_pcm_hardware.item) }

static struct dummy_hw_field fields[] = {
	FIELD_ENTRY(formats, "%#llx"),
	FIELD_ENTRY(rates, "%#x"),
	FIELD_ENTRY(rate_min, "%d"),
	FIELD_ENTRY(rate_max, "%d"),
	FIELD_ENTRY(channels_min, "%d"),
	FIELD_ENTRY(channels_max, "%d"),
	FIELD_ENTRY(buffer_bytes_max, "%ld"),
	FIELD_ENTRY(period_bytes_min, "%ld"),
	FIELD_ENTRY(period_bytes_max, "%ld"),
	FIELD_ENTRY(periods_min, "%d"),
	FIELD_ENTRY(periods_max, "%d"),
};

static void dummy_proc_read(struct snd_info_entry *entry,
			    struct snd_info_buffer *buffer)
{
	struct snd_dummy *dummy = entry->private_data;
	int i;

	for (i = 0; i < ARRAY_SIZE(fields); i++) {
		snd_iprintf(buffer, "%s ", fields[i].name);
		if (fields[i].size == sizeof(int))
			snd_iprintf(buffer, fields[i].format,
				*get_dummy_int_ptr(dummy, fields[i].offset));
		else
			snd_iprintf(buffer, fields[i].format,
				*get_dummy_ll_ptr(dummy, fields[i].offset));
		if (!strcmp(fields[i].name, "formats"))
			print_formats(dummy, buffer);
		else if (!strcmp(fields[i].name, "rates"))
			print_rates(dummy, buffer);
		snd_iprintf(buffer, "\n");
	}
}

static void dummy_proc_write(struct snd_info_entry *entry,
			     struct snd_info_buffer *buffer)
{
	struct snd_dummy *dummy = entry->private_data;
	char line[64];

	while (!snd_info_get_line(buffer, line, sizeof(line))) {
		char item[20];
		const char *ptr;
		unsigned long long val;
		int i;

		ptr = snd_info_get_str(item, line, sizeof(item));
		for (i = 0; i < ARRAY_SIZE(fields); i++) {
			if (!strcmp(item, fields[i].name))
				break;
		}
		if (i >= ARRAY_SIZE(fields))
			continue;
		snd_info_get_str(item, ptr, sizeof(item));
		if (strict_strtoull(item, 0, &val))
			continue;
		if (fields[i].size == sizeof(int))
			*get_dummy_int_ptr(dummy, fields[i].offset) = val;
		else
			*get_dummy_ll_ptr(dummy, fields[i].offset) = val;
	}
}

static void __devinit dummy_proc_init(struct snd_dummy *chip)
{
	struct snd_info_entry *entry;

	if (!snd_card_proc_new(chip->card, "dummy_pcm", &entry)) {
		snd_info_set_text_ops(entry, chip, dummy_proc_read);
		entry->c.text.write = dummy_proc_write;
		entry->mode |= S_IWUSR;
		entry->private_data = chip;
	}
}
#else
#define dummy_proc_init(x)
#endif /* CONFIG_SND_DEBUG && CONFIG_PROC_FS */

static int __devinit snd_dummy_probe(struct platform_device *devptr)
{
	struct snd_card *card;
	struct snd_dummy *dummy;
	int idx, err;
	int dev = devptr->id;

    #if ALSA_DBG_MSG
    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ snd_dummy_probe\n");
    #endif

	err = snd_card_create(index[dev], id[dev], THIS_MODULE,
			      sizeof(struct snd_dummy), &card);
	if (err < 0)
		return err;
	dummy = card->private_data;
	dummy->card = card;

	for (idx = 0; idx < MAX_PCM_DEVICES && idx < pcm_devs[dev]; idx++) {
		if (pcm_substreams[dev] < 1)
			pcm_substreams[dev] = 1;
		if (pcm_substreams[dev] > MAX_PCM_SUBSTREAMS)
			pcm_substreams[dev] = MAX_PCM_SUBSTREAMS;
		err = snd_card_dummy_pcm(dummy, idx, pcm_substreams[dev]);
		if (err < 0)
			goto __nodev;
	}

	dummy->pcm_hw = dummy_pcm_hardware;

	err = snd_card_dummy_new_mixer(dummy);
	if (err < 0)
		goto __nodev;
	strcpy(card->driver, "Dummy");
	strcpy(card->shortname, "Dummy");
	sprintf(card->longname, "Dummy %i", dev + 1);

	dummy_proc_init(dummy);

	snd_card_set_dev(card, &devptr->dev);

	err = snd_card_register(card);
	if (err == 0) 
    {
		platform_set_drvdata(devptr, card);
		return 0;
	}
      __nodev:
	snd_card_free(card);
	return err;
}

static int __devexit snd_dummy_remove(struct platform_device *devptr)
{
	snd_card_free(platform_get_drvdata(devptr));
	platform_set_drvdata(devptr, NULL);
	return 0;
}

#ifdef CONFIG_PM
static int snd_dummy_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct snd_card *card = platform_get_drvdata(pdev);
	struct snd_dummy *dummy = card->private_data;

	snd_power_change_state(card, SNDRV_CTL_POWER_D3hot);
	snd_pcm_suspend_all(dummy->pcm);
	return 0;
}
	
static int snd_dummy_resume(struct platform_device *pdev)
{
	struct snd_card *card = platform_get_drvdata(pdev);

	snd_power_change_state(card, SNDRV_CTL_POWER_D0);
	return 0;
}
#endif

#define SND_DUMMY_DRIVER	"snd_dummy"

static struct platform_driver snd_dummy_driver = 
{
	.probe		= snd_dummy_probe,
	.remove		= __devexit_p(snd_dummy_remove),
#ifdef CONFIG_PM
	.suspend	= snd_dummy_suspend,
	.resume		= snd_dummy_resume,
#endif
	.driver		= 
    {
		.name	= SND_DUMMY_DRIVER
	},
};

static void snd_dummy_unregister_all(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(devices); ++i)
		platform_device_unregister(devices[i]);
	platform_driver_unregister(&snd_dummy_driver);
	free_fake_buffer();
}

static int __init alsa_card_dummy_init(void)
{
	int i, cards, err;

    printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ alsa_card_dummy_init 201105032315\n");
	if (fake_buffer == 0)
	{
		printk(KERN_ERR "!@#!@# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ fake_buffer=0\n");
	}

	err = platform_driver_register(&snd_dummy_driver);
	if (err < 0)
		return err;

	err = alloc_fake_buffer();
	if (err < 0) 
    {
		platform_driver_unregister(&snd_dummy_driver);
        #if ALSA_DBG_MSG
        printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ opm_driver_register fail\n");
        #endif
		return err;
	}

	cards = 0;
	for (i = 0; i < SNDRV_CARDS; i++) 
    {
		struct platform_device *device;

        #if ALSA_DBG_MSG
        printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ try - %d\n", i);	    
        #endif
		if (! enable[i])
        {
            #if ALSA_DBG_MSG
            printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ !enable[i] continue - %d\n", i);	    
            #endif
			continue;
        }
		device = platform_device_register_simple(SND_DUMMY_DRIVER,i, NULL, 0);
        #if ALSA_DBG_MSG
        //printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ device 0x%08x 0x%08x 0x%08x %s - %d\n", device, device->dev, device->dev.driver_data, device->name, i);
        #endif

		if (IS_ERR(device))
		{
            #if ALSA_DBG_MSG
            printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ IS_ERR(device) continue - %d\n", i);	    
            #endif
			continue;
		}
		if (!platform_get_drvdata(device)) 
        {
            #if ALSA_DBG_MSG
            printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ opm_get_drvdata fail - %d\n", i);
            #endif
			platform_device_unregister(device);
			continue;
		}
		devices[i] = device;
		cards++;
	}
	if (!cards) 
    {
        #if ALSA_DBG_MSG
        printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Dummy soundcard not found or device busy\n");	    
        #endif
#ifdef MODULE
		printk(KERN_ERR "Dummy soundcard not found or device busy\n");
#endif
		snd_dummy_unregister_all();
		return -ENODEV;
	}

    printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ alsa_card_dummy_init end\n");

	return 0;
}

static void __exit alsa_card_dummy_exit(void)
{
	snd_dummy_unregister_all();
}

module_init(alsa_card_dummy_init)
module_exit(alsa_card_dummy_exit)
