/*
 * ALSA SoC ES7243 adc driver
 *
 * Author:  
 *
 * Based on sound/soc/codecs/wm8731.c by Richard Purdie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *  ES7243 is a stereo ADC
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>
#include <sound/initval.h>
#include <linux/regmap.h>

#include "es7243.h"

#define ES7243_TDM_ENABLE  1
#define ES7243_CHANNELS_MAX 8
#define ES7243_I2C_BUS_NUM 0
#define ES7243_CODEC_RW_TEST_EN	0
#define ES7243_IDLE_RESET_EN		1		//reset ES7243 when in idle time
#define ES7243_MATCH_DTS_EN		0		//ES7243 match method select: 0: i2c_detect, 1:of_device_id

struct i2c_client *i2c_clt[(ES7243_CHANNELS_MAX)/2];

/* codec private data */
struct es7243_priv {
	struct regmap *regmap;
	struct i2c_client *i2c;
	unsigned int dmic_amic;
	unsigned int sysclk;
	struct snd_pcm_hw_constraint_list *sysclk_constraints;
	unsigned int tdm;
	struct delayed_work pcm_pop_work;
	struct delayed_work pcm_pop_work1;
};
struct snd_soc_codec *tron_codec[4];
int es7243_init_reg = 0;
static int es7243_codec_num = 0;

static const struct regmap_config es7243_regmap_config = {
	.reg_bits = 8,	//Number of bits in a register address
	.val_bits = 8,	//Number of bits in a register value
};
/*
* ES7243 register cache
*/
static const u8 es7243_reg[] = {
	0x00, 0x00, 0x10, 0x04,	/* 0 */
	0x02, 0x13, 0x00, 0x3f,	/* 4 */
	0x11, 0x00, 0xc0, 0xc0,	/* 8 */
	0x12, 0xa0, 0x40, 	/* 12 */
};
static const struct reg_default es7243_reg_defaults[] = {
	{0x00, 0x00}, {0x01, 0x00}, {0x02, 0x10}, {0x03, 0x04},	/* 0 */
	{0x04, 0x02}, {0x05, 0x13}, {0x06, 0x00}, {0x07, 0x3f},	/* 4 */
	{0x08, 0x11}, {0x09, 0x00}, {0x0a, 0xc0}, {0x0b, 0xc0},	/* 8 */
	{0x0c, 0x12}, {0x0d, 0xa0}, {0x0e, 0x40}, 		/* 12 */
};

static int es7243_read(u8 reg, u8 *rt_value, struct i2c_client *client)
{
	int ret;
	u8 read_cmd[3] = {0};
	u8 cmd_len = 0;
	
	read_cmd[0] = reg;
	cmd_len = 1;
	
	if (client->adapter == NULL)
		pr_err("es7243_read client->adapter==NULL\n");
	
	ret = i2c_master_send(client, read_cmd, cmd_len);
	if (ret != cmd_len) {
		pr_err("es7243_read error1\n");
		return -1;
	}
	
	ret = i2c_master_recv(client, rt_value, 1);
	if (ret != 1) {
		pr_err("es7243_read error2, ret = %d.\n", ret);
		return -1;
	}
	
	return 0;
}

static int es7243_write(u8 reg, unsigned char value, struct i2c_client *client)
{
	int ret = 0;
	u8 write_cmd[2] = {0};
	
	write_cmd[0] = reg;
	write_cmd[1] = value;
	
	ret = i2c_master_send(client, write_cmd, 2);
	if (ret != 2) {
		pr_err("es7243_write error->[REG-0x%02x,val-0x%02x]\n",reg,value);
		return -1;
	}
	
	return 0;
}

static int es7243_update_bits(u8 reg, u8 mask, u8 value, struct i2c_client *client)
{
	u8 val_old,val_new;

	es7243_read(reg, &val_old, client);
	val_new = (val_old & ~mask) | (value & mask);
	if(val_new != val_old){
		es7243_write(reg, val_new, client);
	}

	return 0;
}
#if 0
static int es7243_multi_chips_read(u8 reg, unsigned char *rt_value)
{
	u8 i;

	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		es7243_read(reg, rt_value++, i2c_clt[i]);
	}

	return 0;
}


static int es7243_multi_chips_write(u8 reg, unsigned char value)
{
	u8 i;

	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		es7243_write(reg, value, i2c_clt[i]);
	}
	
	return 0;
}
#endif

#if 0
static int es7243_multi_chips_update_bits(u8 reg, u8 mask, u8 value)
{
	u8 i;
	
	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		es7243_update_bits(reg, mask, value, i2c_clt[i]);
	}

	return 0;
}
#endif
struct _coeff_div {
	u32 mclk;       //mclk frequency
	u32 sr_rate;    //sample rate
	u8 speedmode;	//speed mode,0=single,1=double,2=quad
	u8 adc_clk_div; //adcclk and dacclk divider
	u8 lrckdiv;     //adclrck divider and daclrck divider
	u8 bclkdiv;     //sclk divider
	u8 osr;         //adc osr
};

/* codec hifi mclk clock divider coefficients */
static const struct _coeff_div coeff_div[] = {
	/* 12.288MHZ */
	{12288000, 8000  , 0, 0x0c , 0x60, 24, 32},
	{12288000, 12000 , 0, 0x08 , 0x40, 16, 32},
	{12288000, 16000 , 0, 0x06 , 0x30, 12, 32},
	{12288000, 24000 , 0, 0x04 , 0x20, 8 , 32},
	{12288000, 32000 , 0, 0x03 , 0x18, 6 , 32},
	{12288000, 48000 , 0, 0x02 , 0x10, 4 , 32},
	{12288000, 64000 , 1, 0x03 , 0x0c, 3 , 32},
	{12288000, 96000 , 1, 0x02 , 0x08, 2 , 32},
	/* 11.2896MHZ */
	{11289600, 11025 , 0, 0x08 , 0x40, 16, 32},
	{11289600, 22050 , 0, 0x04 , 0x20, 8 , 32},
	{11289600, 44100 , 0, 0x02 , 0x10, 4 , 32},
	{11289600, 88200 , 1, 0x02 , 0x08, 2 , 32},

	/* 12.000MHZ */
	{12000000, 8000  , 0, 0x0c , 0xbc, 30, 31},
	{12000000, 11025 , 0, 0x08 , 0x44, 17, 34},
	{12000000, 12000 , 0, 0x08 , 0xaa, 20, 31},
	{12000000, 16000 , 0, 0x06 , 0x9e, 15, 31},
	{12000000, 22050 , 0, 0x04 , 0x22, 8 , 34},
	{12000000, 24000 , 0, 0x04 , 0x94, 10, 31},
	{12000000, 32000 , 0, 0x03 , 0x8a, 5 , 31},
	{12000000, 44100 , 0, 0x02 , 0x11, 4 , 34},
	{12000000, 48000 , 0, 0x02 , 0x85, 5 , 31},
	{12000000, 96000 , 1, 0x02 , 0x85, 1 , 31},
};
static inline int get_coeff(int mclk, int rate)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(coeff_div); i++) {
		if (coeff_div[i].sr_rate == rate && coeff_div[i].mclk == mclk)
			return i;
	}

	return -EINVAL;
}

/* The set of rates we can generate from the above for each SYSCLK */

static unsigned int rates_12288[] = {
	8000, 12000, 16000, 24000, 24000, 32000, 48000, 96000,
};

static struct snd_pcm_hw_constraint_list constraints_12288 = {
	.count	= ARRAY_SIZE(rates_12288),
	.list	= rates_12288,
};

static unsigned int rates_112896[] = {
	8000, 11025, 22050, 44100,
};

static struct snd_pcm_hw_constraint_list constraints_112896 = {
	.count	= ARRAY_SIZE(rates_112896),
	.list	= rates_112896,
};

static unsigned int rates_12[] = {
	8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000,
	48000, 88235, 96000,
};

static struct snd_pcm_hw_constraint_list constraints_12 = {
	.count	= ARRAY_SIZE(rates_12),
	.list	= rates_12,
};

/*
* Note that this should be called from init rather than from hw_params.
*/
static int es7243_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct es7243_priv *es7243 = snd_soc_codec_get_drvdata(codec);

	switch (freq) {
	case 11289600:
	case 22579200:
		es7243->sysclk_constraints = &constraints_112896;
		es7243->sysclk = freq;
		return 0;
	case 12288000:
	case 24576000:
		es7243->sysclk_constraints = &constraints_12288;
		es7243->sysclk = freq;
		return 0;
	case 12000000:
	case 24000000:
		es7243->sysclk_constraints = &constraints_12;
		es7243->sysclk = freq;
		return 0;
	}
	return -EINVAL;
}

static int es7243_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	//struct snd_soc_codec *codec = codec_dai->codec;
	u8 iface = 0;
	u8 adciface = 0;
	u8 i;
	
	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++) {
		es7243_read(ES7243_SDPFMT_REG01, &adciface, i2c_clt[i]);
		es7243_read(ES7243_MODECFG_REG00, &iface, i2c_clt[i]);

		/* set master/slave audio interface */
		switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
			case SND_SOC_DAIFMT_CBM_CFM:    // MASTER MODE
					iface |= 0x02;
			break;
			case SND_SOC_DAIFMT_CBS_CFS:    // SLAVE MODE
					iface &= 0xfd;
			break;
			default:
			return -EINVAL;
		}


		/* interface format */

		switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
      			case SND_SOC_DAIFMT_I2S:
				adciface &= 0xFC;                   
				break;
     			case SND_SOC_DAIFMT_RIGHT_J:
				return -EINVAL;
      			case SND_SOC_DAIFMT_LEFT_J:
      	   			adciface &= 0xFC;                   
           			adciface |= 0x01;                             
				break;
      			case SND_SOC_DAIFMT_DSP_A:
        	 		adciface &= 0xDC;                   
           			adciface |= 0x03;                   
				break;
      			case SND_SOC_DAIFMT_DSP_B:
        			adciface &= 0xDC;                   
        			adciface |= 0x23;                   
      				break;
      			default:
          			return -EINVAL;
    	}

    	/* clock inversion */
    	adciface &= 0xbF; 
    	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
    		case SND_SOC_DAIFMT_NB_NF:  
			adciface &= 0xbF;
        		break;
        	case SND_SOC_DAIFMT_IB_IF:          
            		adciface |= 0x60;
        		break;
        	case SND_SOC_DAIFMT_IB_NF:
            		adciface |= 0x40;
        		break;
        	case SND_SOC_DAIFMT_NB_IF:
            		adciface |= 0x20;            
        		break;
        	default:
            		return -EINVAL;
    	}
    	
    	//es7243_update_bits(ES7243_MODECFG_REG00, 0x02, iface, i2c_clt[i]);
    	//es7243_update_bits(ES7243_SDPFMT_REG01, 0x03, adciface, i2c_clt[i]);
  }  	
	return 0;
}
static void es7243_init_codec(struct snd_soc_codec *codec, u8 i)
{
		es7243_write(ES7243_MODECFG_REG00, 0x01, i2c_clt[i]);//enter into hardware mode
		es7243_write(ES7243_STATECTL_REG06, 0x18, i2c_clt[i]); //soft reset codec
		//es7243_write(ES7243_SDPFMT_REG01, 0xd3, i2c_clt[i]); //dsp for tdm mode, DSP-A, 32BIT, bclk invert
		es7243_write(ES7243_SDPFMT_REG01, 0xCF, i2c_clt[i]); //dsp for tdm mode, DSP-A, 16BIT, bclk invert
		es7243_write(ES7243_LRCDIV_REG02, 0x10, i2c_clt[i]); 
		es7243_write(ES7243_BCKDIV_REG03, 0x04, i2c_clt[i]); 
		es7243_write(ES7243_CLKDIV_REG04, 0x02, i2c_clt[i]); 
		es7243_write(ES7243_MUTECTL_REG05, 0x1a, i2c_clt[i]);
		es7243_write(ES7243_ANACTL1_REG08, 0x4b, i2c_clt[i]);//enable microphone input and pga gain for 27db
		es7243_write(ES7243_ANACTL2_REG09, 0x3F, i2c_clt[i]); 
		es7243_write(ES7243_STATECTL_REG06, 0x00, i2c_clt[i]);
		es7243_write(ES7243_MUTECTL_REG05, 0x12, i2c_clt[i]); 
		if(i == 3) {
			es7243_write(ES7243_ANACTL1_REG08, 0x19, i2c_clt[i]);
		} 
		else {
			es7243_write(ES7243_ANACTL1_REG08, 0x4b, i2c_clt[i]);	//enable microphone input and pga gain for 27db
		}
		es7243_write(ES7243_STATECTL_REG06, 0x00, i2c_clt[i]); 
}
static void es7243_unmute(struct snd_soc_codec *codec, u8 i)
{
		es7243_write(ES7243_MUTECTL_REG05, 0x13, i2c_clt[i]); 
}
static void pcm_pop_work1_events(struct work_struct *work)
{
	int i;
	
	printk("es8316--------pcm_pop_work_events\n");

	for(i = 0; i < (ES7243_CHANNELS_MAX)/2; i++){
		es7243_unmute(tron_codec[3-i], 3-i);
	}

	es7243_init_reg = 1;
}
static void pcm_pop_work_events(struct work_struct *work)
{
	int i;
	printk("es8316--------pcm_pop_work_events\n");

	for(i = 0; i < (ES7243_CHANNELS_MAX)/2; i++){

		es7243_init_codec(tron_codec[i], i);
	}
	es7243_init_reg = 1;
}

static int es7243_pcm_startup(struct snd_pcm_substream *substream,
			      struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;
	struct es7243_priv *es7243 = snd_soc_codec_get_drvdata(codec);
	return 0;
        
	/* The set of sample rates that can be supported depends on the
	 * MCLK supplied to the CODEC - enforce this.
	 */
	if (!es7243->sysclk) {
		dev_err(codec->dev,
			"No MCLK configured, call set_sysclk() on init\n");
		return -EINVAL;
	}

	snd_pcm_hw_constraint_list(substream->runtime, 0,
				   SNDRV_PCM_HW_PARAM_RATE,
				   es7243->sysclk_constraints);

	return 0;
}
static int es7243_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{ 
	//struct snd_soc_pcm_runtime *rtd = substream->private_data;
	//struct snd_soc_codec *codec = rtd->codec;
	//struct es7243_priv *es7243 = snd_soc_codec_get_drvdata(codec);
	u8 i,val;

	if(es7243_init_reg == 0) {
           printk("==================>>>>>>>>es8316_pcm_startup es8316_init_reg=0\n");
	#if 0
           //schedule_delayed_work(&es7243->pcm_pop_work, msecs_to_jiffies(100));
           schedule_delayed_work(&es7243->pcm_pop_work,0);
	   mdelay(50);
           schedule_delayed_work(&es7243->pcm_pop_work1, msecs_to_jiffies(700));
	#endif
        for(i = 0; i < (ES7243_CHANNELS_MAX)/2; i++){

                es7243_init_codec(tron_codec[i], i);
        }
        es7243_init_reg = 1;
	mdelay(100);
        for(i = 0; i < (ES7243_CHANNELS_MAX)/2; i++){
                es7243_unmute(tron_codec[3-i], 3-i);
        }

    }

	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		es7243_read(ES7243_MODECFG_REG00, &val, i2c_clt[i]);//enter into hardware mode
		printk("%d -- Reg 00 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_STATECTL_REG06, &val, i2c_clt[i]); //soft reset codec
		printk("%d -- Reg 06 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_SDPFMT_REG01, &val, i2c_clt[i]); //dsp for tdm mode, DSP-A, 16BIT
		printk("%d -- Reg 01 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_LRCDIV_REG02, &val, i2c_clt[i]);
		printk("%d -- Reg 02 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_BCKDIV_REG03, &val, i2c_clt[i]); 
		printk("%d -- Reg 03 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_CLKDIV_REG04, &val, i2c_clt[i]); 
		printk("%d -- Reg 04 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[i]); 
		printk("%d -- Reg 05 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_ANACTL1_REG08, &val, i2c_clt[i]);	//enable microphone input and pga gain for 27db
		printk("%d -- Reg 08 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_ANACTL2_REG09, &val, i2c_clt[i]); 
		printk("%d -- Reg 09 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_STATECTL_REG06, &val, i2c_clt[i]); 
		printk("%d -- Reg 06 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_ANACTL0_REG07, &val, i2c_clt[i]); //power up adc and analog input
		printk("%d -- Reg 07 ----> 0x%x!\n",__LINE__,val);
		es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[i]); 
		printk("%d -- Reg 05 ----> 0x%x!\n",__LINE__,val);
	}

	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){

		/* bit size */
		switch (params_format(params)) {
			case SNDRV_PCM_FORMAT_S16_LE:
				es7243_update_bits(ES7243_SDPFMT_REG01, 0x1c, 0x0c, i2c_clt[i]);
				break;
			case SNDRV_PCM_FORMAT_S20_3LE:
				es7243_update_bits(ES7243_SDPFMT_REG01, 0x1c, 0x04, i2c_clt[i]);
				break;
			case SNDRV_PCM_FORMAT_S24_LE:
				es7243_update_bits(ES7243_SDPFMT_REG01, 0x1c, 0x00, i2c_clt[i]);
				break;
			case SNDRV_PCM_FORMAT_S32_LE:
				es7243_update_bits(ES7243_SDPFMT_REG01, 0x1c, 0x10, i2c_clt[i]);
				break;
		}

	}
	return 0;
}

static int es7243_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u8 i;
	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		dev_dbg(codec->dev, "%s %d\n", __func__, mute);
	
		if (mute) 
		{		
			es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x08, i2c_clt[i]);
		} 
		else 
		{
			es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x00, i2c_clt[i]);
		}
	}
	return 0;
}

static int es7243_set_bias_level(struct snd_soc_codec *codec,
				 enum snd_soc_bias_level level)
{        
	u8 i;
		
	for(i=0; i<(ES7243_CHANNELS_MAX)/2; i++){
		switch (level) {
		case SND_SOC_BIAS_ON:
			dev_dbg(codec->dev, "%s on\n", __func__);
			break;
		case SND_SOC_BIAS_PREPARE:
			dev_dbg(codec->dev, "%s prepare\n", __func__);
			break;
		case SND_SOC_BIAS_STANDBY:
			dev_dbg(codec->dev, "%s standby\n", __func__);
			es7243_write(ES7243_MODECFG_REG00, 0x01, i2c_clt[i]);//enter into hardware mode
                	es7243_write(ES7243_STATECTL_REG06, 0x18, i2c_clt[i]); //soft reset codec
			es7243_write(ES7243_ANACTL0_REG07, 0x80, i2c_clt[i]);
                	//es7243_write(ES7243_SDPFMT_REG01, 0xd3, i2c_clt[i]); //dsp for tdm mode, DSP-A, 32BIT, bclk invert
                	es7243_write(ES7243_SDPFMT_REG01, 0xCF, i2c_clt[i]); //dsp for tdm mode, DSP-A, 16BIT, bclk invert
                	es7243_write(ES7243_LRCDIV_REG02, 0x10, i2c_clt[i]);
                	es7243_write(ES7243_BCKDIV_REG03, 0x04, i2c_clt[i]);
                	es7243_write(ES7243_CLKDIV_REG04, 0x02, i2c_clt[i]); 
                	es7243_write(ES7243_MUTECTL_REG05, 0x1a, i2c_clt[i]);
                	es7243_write(ES7243_ANACTL1_REG08, 0x4b, i2c_clt[i]);//enable microphone input and pga gain for 27db
                	es7243_write(ES7243_ANACTL2_REG09, 0x3F, i2c_clt[i]); 
                	es7243_write(ES7243_STATECTL_REG06, 0x00, i2c_clt[i]);
                	es7243_write(ES7243_MUTECTL_REG05, 0x12, i2c_clt[i]);
	                if(i == 3) {
        	                es7243_write(ES7243_ANACTL1_REG08, 0x19, i2c_clt[i]);
                	}
               		else {
                        	es7243_write(ES7243_ANACTL1_REG08, 0x4b, i2c_clt[i]);   //enable microphone input and pga gain for 27db
                	}

			break;
		case SND_SOC_BIAS_OFF:
			dev_dbg(codec->dev, "%s off\n", __func__);
			es7243_write(ES7243_STATECTL_REG06, 0x05, i2c_clt[i]);
			es7243_write(ES7243_MUTECTL_REG05, 0x1b, i2c_clt[i]);
			es7243_write(ES7243_STATECTL_REG06, 0x5c, i2c_clt[i]);
			es7243_write(ES7243_ANACTL0_REG07, 0x3f, i2c_clt[i]);
			es7243_write(ES7243_ANACTL1_REG08, 0x4b, i2c_clt[i]);
			es7243_write(ES7243_ANACTL2_REG09, 0x9F, i2c_clt[i]); 
			break;
		}
	}
	//codec->dapm.bias_level = level;
	return 0;
}

#define es7243_RATES SNDRV_PCM_RATE_8000_96000

#define es7243_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE)

static struct snd_soc_dai_ops es7243_ops = {
	.startup = es7243_pcm_startup,
	.hw_params = es7243_pcm_hw_params,
	.set_fmt = es7243_set_dai_fmt,
	.set_sysclk = es7243_set_dai_sysclk,
	.digital_mute = es7243_mute,
};

static struct snd_soc_dai_driver es7243_dai0 = {
	.name = "ES7243 HiFi 0",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = es7243_RATES,
		.formats = es7243_FORMATS,
	 },
	.ops = &es7243_ops,
	.symmetric_rates = 1,
};
static struct snd_soc_dai_driver es7243_dai1 = {
	.name = "ES7243 HiFi 1",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = es7243_RATES,
		.formats = es7243_FORMATS,
	 },
	.ops = &es7243_ops,
	.symmetric_rates = 1,
};
static struct snd_soc_dai_driver es7243_dai2 = {
	.name = "ES7243 HiFi 2",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = es7243_RATES,
		.formats = es7243_FORMATS,
	 },
	.ops = &es7243_ops,
	.symmetric_rates = 1,
};
static struct snd_soc_dai_driver es7243_dai3 = {
	.name = "ES7243 HiFi 3",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = es7243_RATES,
		.formats = es7243_FORMATS,
	 },
	.ops = &es7243_ops,
	.symmetric_rates = 1,
};

static struct snd_soc_dai_driver *es7243_dai[] = {
#if ES7243_CHANNELS_MAX > 0
	&es7243_dai0,
#endif
#if ES7243_CHANNELS_MAX > 2
	&es7243_dai1,
#endif
#if ES7243_CHANNELS_MAX > 4
	&es7243_dai2,
#endif
#if ES7243_CHANNELS_MAX > 6
	&es7243_dai3,
#endif
};
static int es7243_suspend(struct snd_soc_codec *codec)
{
	es7243_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

static int es7243_resume(struct snd_soc_codec *codec)
{
	es7243_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}

static int es7243_probe(struct snd_soc_codec *codec)
{
	struct es7243_priv *es7243 = snd_soc_codec_get_drvdata(codec);
	int ret = 0;
#if !ES7243_CODEC_RW_TEST_EN
	//ret = snd_soc_codec_set_cache_io(codec, 8, 8, SND_SOC_I2C);//8,8
#else
	codec->control_data = devm_regmap_init_i2c(es7243->i2c, &es7243_regmap_config);
	ret = PTR_RET(codec->control_data);
#endif
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set cache I/O: %d\n", ret);
		return ret;
	}	
	printk("begin->>>>>>>>>>%s!\n",__func__);

	tron_codec[es7243_codec_num++] = codec;
	INIT_DELAYED_WORK(&es7243->pcm_pop_work, pcm_pop_work_events);
	INIT_DELAYED_WORK(&es7243->pcm_pop_work1, pcm_pop_work1_events);
	return 0;
}

static int es7243_remove(struct snd_soc_codec *codec)
{
	es7243_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

#if ES7243_CHANNELS_MAX > 0
static int es7243_adc1_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        //printk("adc1 ucontrol->value.integer.value[0] = %ld", ucontrol->value.integer.value[0]);
	if(ucontrol->value.integer.value[0]) 	
		es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x08, i2c_clt[0]);
	else
		es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x00, i2c_clt[0]);
        return 0;
}
static int es7243_adc1_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc1_digital_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x06, 0x18, i2c_clt[0]);
	} 
	 else {
                es7243_write(0x06, 0x00, i2c_clt[0]);
	}
        return 0;
}
static int es7243_adc1_digital_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
	es7243_read(0x06, &val, i2c_clt[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc1_analog_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x07, 0x3f, i2c_clt[0]);
        } 
         else {
                es7243_write(0x07, 0x80, i2c_clt[0]);
        }
        return 0;
}
static int es7243_adc1_analog_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x07, &val, i2c_clt[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif
#if ES7243_CHANNELS_MAX > 2
static int es7243_adc2_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0])
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x08, i2c_clt[1]);
        else
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x00, i2c_clt[1]);
        
	return 0;
}
static int es7243_adc2_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc2_digital_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x06, 0x18, i2c_clt[1]);
        }
         else {
                es7243_write(0x06, 0x00, i2c_clt[1]);
        }
        return 0;
}
static int es7243_adc2_digital_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x06, &val, i2c_clt[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc2_analog_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x07, 0x3f, i2c_clt[1]);
        }
         else {
                es7243_write(0x07, 0x80, i2c_clt[1]);
        }
        return 0;
}
static int es7243_adc2_analog_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x07, &val, i2c_clt[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}


#endif
#if ES7243_CHANNELS_MAX > 4
static int es7243_adc3_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0])
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x08, i2c_clt[2]);
        else
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x00, i2c_clt[2]);
        
	return 0;
}
static int es7243_adc3_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc3_digital_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x06, 0x18, i2c_clt[2]);
        }
         else {
                es7243_write(0x06, 0x00, i2c_clt[2]);
        }
        return 0;
}
static int es7243_adc3_digital_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x06, &val, i2c_clt[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc3_analog_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x07, 0x3f, i2c_clt[2]);
        }
         else {
                es7243_write(0x07, 0x80, i2c_clt[2]);
        }
        return 0;
}
static int es7243_adc3_analog_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x07, &val, i2c_clt[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif
#if ES7243_CHANNELS_MAX > 6
static int es7243_adc4_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        
        if(ucontrol->value.integer.value[0])
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x08, i2c_clt[3]);
        else
                es7243_update_bits(ES7243_MUTECTL_REG05, 0x08, 0x00, i2c_clt[3]);

	return 0;
}
static int es7243_adc4_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(ES7243_MUTECTL_REG05, &val, i2c_clt[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc4_digital_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x06, 0x18, i2c_clt[3]);
        }
         else {
                es7243_write(0x06, 0x00, i2c_clt[3]);
        }
        return 0;
}
static int es7243_adc4_digital_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x06, &val, i2c_clt[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7243_adc4_analog_standby_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        if(ucontrol->value.integer.value[0]) {
                es7243_write(0x07, 0x3f, i2c_clt[3]);
        }
         else {
                es7243_write(0x07, 0x80, i2c_clt[3]);
        }
        return 0;
}
static int es7243_adc4_analog_standby_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7243_read(0x07, &val, i2c_clt[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif
static const struct snd_kcontrol_new es7243_snd_controls[] = {
#if ES7243_CHANNELS_MAX > 0
    SOC_SINGLE_EXT("ADC1_MUTE", ES7243_MUTECTL_REG05, 3, 8, 1,
                        es7243_adc1_mute_get, es7243_adc1_mute_set),
    SOC_SINGLE_EXT("ADC1_DIGITAL_STANDBY", ES7243_STATECTL_REG06, 7, 0x80, 1,
                        es7243_adc1_digital_standby_get, 
			es7243_adc1_digital_standby_set),
    SOC_SINGLE_EXT("ADC1_ANALOG_STANDBY", ES7243_ANACTL0_REG07, 3, 0x08, 1,
                        es7243_adc1_analog_standby_get,
                        es7243_adc1_analog_standby_set),

#endif
#if ES7243_CHANNELS_MAX > 2
    SOC_SINGLE_EXT("ADC2_MUTE", ES7243_MUTECTL_REG05, 3, 8, 1,
                        es7243_adc2_mute_get, es7243_adc2_mute_set),
    SOC_SINGLE_EXT("ADC2_DIGITAL_STANDBY", ES7243_STATECTL_REG06, 7, 0x80, 1,
                        es7243_adc2_digital_standby_get,
                        es7243_adc2_digital_standby_set),
    SOC_SINGLE_EXT("ADC2_ANALOG_STANDBY", ES7243_ANACTL0_REG07, 3, 0x08, 1,
                        es7243_adc2_analog_standby_get,
                        es7243_adc2_analog_standby_set),

#endif
#if ES7243_CHANNELS_MAX > 4
    SOC_SINGLE_EXT("ADC3_MUTE", ES7243_MUTECTL_REG05, 3, 8, 1,
                        es7243_adc3_mute_get, es7243_adc3_mute_set),
    SOC_SINGLE_EXT("ADC3_DIGITAL_STANDBY", ES7243_STATECTL_REG06, 7, 0x80, 1,
                        es7243_adc3_digital_standby_get,
                        es7243_adc3_digital_standby_set),
    SOC_SINGLE_EXT("ADC3_ANALOG_STANDBY", ES7243_ANACTL0_REG07, 3, 0x08, 1,
                        es7243_adc3_analog_standby_get,
                        es7243_adc3_analog_standby_set),

#endif
#if ES7243_CHANNELS_MAX > 6
    SOC_SINGLE_EXT("ADC4_MUTE", ES7243_MUTECTL_REG05, 3, 8, 1,
                        es7243_adc4_mute_get, es7243_adc4_mute_set),
    SOC_SINGLE_EXT("ADC4_DIGITAL_STANDBY", ES7243_STATECTL_REG06, 7, 0x80, 1,
                        es7243_adc4_digital_standby_get,
                        es7243_adc4_digital_standby_set),
    SOC_SINGLE_EXT("ADC4_ANALOG_STANDBY", ES7243_ANACTL0_REG07, 3, 0x08, 1,
                        es7243_adc4_analog_standby_get,
                        es7243_adc4_analog_standby_set),

#endif
};

static struct snd_soc_codec_driver soc_codec_dev_es7243 = {
	.probe = es7243_probe,
	.remove = es7243_remove,
	.suspend = es7243_suspend,
	.resume = es7243_resume,
	.set_bias_level = es7243_set_bias_level,
	.idle_bias_off = true,
	.reg_word_size = sizeof(u8),
	.controls = es7243_snd_controls,
	.num_controls = ARRAY_SIZE(es7243_snd_controls),
#if ES7243_CODEC_RW_TEST_EN
	.read = es7243_codec_read,
	.write = es7243_codec_write,
#endif

};

static ssize_t es7243_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int val=0, flag=0;
	u8 i=0, reg, num, value_w, value_r;
	
	struct es7243_priv *es7243 = dev_get_drvdata(dev);
	val = simple_strtol(buf, NULL, 16);
	flag = (val >> 16) & 0xFF;
	
	if (flag) {
		reg = (val >> 8) & 0xFF;
		value_w = val & 0xFF;
		printk("\nWrite: start REG:0x%02x,val:0x%02x,count:0x%02x\n", reg, value_w, flag);
		while(flag--) {
			es7243_write(reg, value_w, es7243->i2c);
			printk("Write 0x%02x to REG:0x%02x\n", value_w, reg);
			reg++;
		}
	} else {
		reg = (val >> 8) & 0xFF;
		num = val & 0xff;
		printk("\nRead: start REG:0x%02x,count:0x%02x\n", reg, num);
		do {
			value_r = 0;
			es7243_read(reg, &value_r, es7243->i2c);
			printk("REG[0x%02x]: 0x%02x;  \n", reg, value_r);
			reg++;
			i++;
			if ((i==num) || (i%4==0))	printk("\n");
		} while (i<num);
	}
	
	return count;
}

static ssize_t es7243_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("echo flag|reg|val > es7243\n");
	printk("eg read star addres=0x06,count 0x10:echo 0610 >es7243\n");
	printk("eg write star addres=0x90,value=0x3c,count=4:echo 4903c >es7243\n");
	//printk("eg write value:0xfe to address:0x06 :echo 106fe > es7243\n");
	return 0;
}

static DEVICE_ATTR(es7243, 0644, es7243_show, es7243_store);

static struct attribute *es7243_debug_attrs[] = {
	&dev_attr_es7243.attr,
	NULL,
};

static struct attribute_group es7243_debug_attr_group = {
	.name   = "es7243_debug",
	.attrs  = es7243_debug_attrs,
};

//#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
/*
 * If the i2c layer weren't so broken, we could pass this kind of data
 * around
 */
static int es7243_i2c_probe(struct i2c_client *i2c,
				   const struct i2c_device_id *i2c_id)
{
	struct es7243_priv *es7243;
	int ret;
//	struct device_node *np = i2c->dev.of_node;
	
	printk("begin->>>>>>>>>>%s!\n",__func__);
	
	es7243 = devm_kzalloc(&i2c->dev, sizeof(struct es7243_priv), GFP_KERNEL);
	if (es7243 == NULL)
		return -ENOMEM;
  	es7243->i2c = i2c;
	es7243->tdm =  ES7243_TDM_ENABLE;  //to initialize tdm mode
	dev_set_drvdata(&i2c->dev, es7243);
	//i2c_set_clientdata(i2c, es7243);
	//es7243->regmap = devm_regmap_init_i2c(i2c, &es7243_regmap);
  	//      if (IS_ERR(es7243->regmap)) {
	//	ret = PTR_ERR(es7243->regmap);
	//	dev_err(&i2c->dev, "Failed to allocate register map: %d\n",
	//	ret);
  	//      return ret;
  	//      }
	if (i2c_id->driver_data < (ES7243_CHANNELS_MAX)/2) {
		i2c_clt[i2c_id->driver_data] = i2c;
		ret = snd_soc_register_codec(&i2c->dev,&soc_codec_dev_es7243, es7243_dai[i2c_id->driver_data], 1);
		if (ret < 0) {
			kfree(es7243);
			return ret;
		}
	}
	ret = sysfs_create_group(&i2c->dev.kobj, &es7243_debug_attr_group);
	if (ret) {
		pr_err("failed to create attr group\n");
	}
	return ret;
}
static int __exit es7243_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
	kfree(i2c_get_clientdata(i2c));
	return 0;
}

static int es7243_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if (adapter->nr == ES7243_I2C_BUS_NUM) {
		if(client->addr == 0x10) {
			strlcpy(info->type, "MicArray_0", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x11) {
			strlcpy(info->type, "MicArray_1", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x12) {
			strlcpy(info->type, "MicArray_2", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x13) {
			strlcpy(info->type, "MicArray_3", I2C_NAME_SIZE);
			return 0;
		}
	}

	return -ENODEV;
}

static const unsigned short es7243_i2c_addr[] = {
#if ES7243_CHANNELS_MAX > 0
	0x10,
#endif

#if ES7243_CHANNELS_MAX > 2
	0x11,
#endif

#if ES7243_CHANNELS_MAX > 4
	0x12,
#endif

#if ES7243_CHANNELS_MAX > 6
	0x13,
#endif

	I2C_CLIENT_END,
};

/*
* device tree source or i2c_board_info both use to transfer hardware information to linux kernel, 
* use one of them wil be OK
*/
static struct i2c_board_info es7243_i2c_board_info[] = {
#if ES7243_CHANNELS_MAX > 0
	{I2C_BOARD_INFO("MicArray_0", 0x10),},//es7243_0
#endif

#if ES7243_CHANNELS_MAX > 2
	{I2C_BOARD_INFO("MicArray_1", 0x11),},//es7243_1
#endif

#if ES7243_CHANNELS_MAX > 4
	{I2C_BOARD_INFO("MicArray_2", 0x12),},//es7243_2
#endif

#if ES7243_CHANNELS_MAX > 6
	{I2C_BOARD_INFO("MicArray_3", 0x13),},//es7243_3
#endif
};

static const struct i2c_device_id es7243_i2c_id[] = {
#if ES7243_CHANNELS_MAX > 0
	{ "MicArray_0", 0 },//es7243_0
#endif

#if ES7243_CHANNELS_MAX > 2
	{ "MicArray_1", 1 },//es7243_1
#endif

#if ES7243_CHANNELS_MAX > 4
	{ "MicArray_2", 2 },//es7243_2
#endif

#if ES7243_CHANNELS_MAX > 6
	{ "MicArray_3", 3 },//es7243_3
#endif
	{ }
};
MODULE_DEVICE_TABLE(i2c, es7243_i2c_id);

static const struct of_device_id es7243_dt_ids[] = {
#if ES7243_CHANNELS_MAX > 0
	{ .compatible = "MicArray_0", },//es7243_0
#endif

#if ES7243_CHANNELS_MAX > 2
	{ .compatible = "MicArray_1", },//es7243_1
#endif

#if ES7243_CHANNELS_MAX > 4
	{ .compatible = "MicArray_2", },//es7243_2
#endif

#if ES7243_CHANNELS_MAX > 6
	{ .compatible = "MicArray_3", },//es7243_3
#endif
};
MODULE_DEVICE_TABLE(of, es7243_dt_ids);

static struct i2c_driver es7243_i2c_driver = {
	.driver = {
		   .name = "es7243",
				.owner = THIS_MODULE,
		#if ES7243_MATCH_DTS_EN
				.of_match_table = es7243_dt_ids,
		#endif		   
		   },
	.probe = es7243_i2c_probe,
	.remove = __exit_p(es7243_i2c_remove),
	.class  = I2C_CLASS_HWMON,
	.id_table = es7243_i2c_id,
#if !ES7243_MATCH_DTS_EN
	.address_list = es7243_i2c_addr,
	.detect = es7243_i2c_detect,
#endif
};


static int __init es7243_modinit(void)
{
	int ret,i;
	struct i2c_adapter *adapter;
    	struct i2c_client *client;
	printk("%s enter es7243\n",__func__);

	//i2c_new_device(adapter, &tlv320_i2c_board_info[i]);
	adapter = i2c_get_adapter(ES7243_I2C_BUS_NUM);
    	if (!adapter) {
        	printk("i2c_get_adapter() fail!\n");
        	return -ENODEV;
    	}
	printk("%s() begin0000",__func__);

    	for(i = 0; i < 4; i++) {
        	client = i2c_new_device(adapter, &es7243_i2c_board_info[i]);
        	printk("%s() i2c_new_device\n",__func__);
        	if (!client)
            	return -ENODEV;
    	}
	i2c_put_adapter(adapter);
	ret = i2c_add_driver(&es7243_i2c_driver);
	if (ret != 0)
		pr_err("Failed to register es7243 i2c driver : %d \n", ret);
	return ret;
}
module_init(es7243_modinit);
static void __exit es7243_exit(void)
{
//#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
	i2c_del_driver(&es7243_i2c_driver);
//#endif
}
module_exit(es7243_exit);
MODULE_DESCRIPTION("ASoC ES7243 audio adc driver");
MODULE_AUTHOR("David Yang");
MODULE_LICENSE("GPL v2");

