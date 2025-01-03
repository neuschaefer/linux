/*
 * ALSA SoC ES7210 adc driver
 *
 * Author:      
 * Copyright:   
 *
 * Based on sound/soc/codecs/es7243.c by David Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *  ES7210 is a 4-ch ADC 
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
#include "es7210.h"

#define ENABLE		1
#define DISABLE		0

#define MIC_CHN_16	16
#define MIC_CHN_14	14
#define MIC_CHN_12	12
#define MIC_CHN_10	10
#define MIC_CHN_8	8
#define MIC_CHN_6	6
#define MIC_CHN_4	4
#define MIC_CHN_2	2

#define ES7210_TDM_ENABLE	ENABLE
#define ES7210_CHANNELS_MAX	MIC_CHN_8

#if ES7210_CHANNELS_MAX == MIC_CHN_2
	#define ADC_DEV_MAXNUM	1
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_4
        #define ADC_DEV_MAXNUM  1
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_6
        #define ADC_DEV_MAXNUM  2
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_8
        #define ADC_DEV_MAXNUM  2
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_10
        #define ADC_DEV_MAXNUM  3
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_12
        #define ADC_DEV_MAXNUM  3
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_14
        #define ADC_DEV_MAXNUM  4
#endif
#if ES7210_CHANNELS_MAX == MIC_CHN_16
        #define ADC_DEV_MAXNUM  4
#endif

#define ES7210_TDM_1LRCK_DSPA                 0
#define ES7210_TDM_1LRCK_DSPB                 1
#define ES7210_TDM_1LRCK_I2S                  2
#define ES7210_TDM_1LRCK_LJ                   3
#define ES7210_TDM_NLRCK_DSPA                 4
#define ES7210_TDM_NLRCK_DSPB                 5
#define ES7210_TDM_NLRCK_I2S                  6
#define ES7210_TDM_NLRCK_LJ                   7
#define ES7210_NORMAL_I2S			8
#define ES7210_NORMAL_LJ			9
#define ES7210_NORMAL_DSPA			10
#define ES7210_NORMAL_DSPB			11

#define ES7210_WORK_MODE    ES7210_TDM_1LRCK_DSPA


#define ES7210_I2C_BUS_NUM 		0
#define ES7210_CODEC_RW_TEST_EN		0
#define ES7210_IDLE_RESET_EN		1	//reset ES7210 when in idle time
#define ES7210_MATCH_DTS_EN		0	//ES7210 match method select: 0: i2c_detect, 1:of_device_id

struct i2c_client *i2c_clt1[ADC_DEV_MAXNUM];

/* codec private data */
struct es7210_priv {
	struct regmap *regmap;
	struct i2c_client *i2c;		//i2c client for es7210
	/*
	  enable or disable pdm dmic interface, must be initialized in i2c_probe()
	  pdm_dmic_enanle=1, pdm dmic interface enabled
	  pdm_dmic_enanle=0, pdm dmic interface disabled
	*/
	#if ES7210_CHANNELS_MAX > 0
		unsigned int pdm_dmic_1_2_enable;	
		unsigned int pdm_dmic_3_4_enable;
	#endif	
	#if ES7210_CHANNELS_MAX > 4
		unsigned int pdm_dmic_5_6_enable;	
		unsigned int pdm_dmic_7_8_enable;
	#endif
	#if ES7210_CHANNELS_MAX > 8
		unsigned int pdm_dmic_9_10_enable;	
		unsigned int pdm_dmic_11_12_enable;	
	#endif
	#if ES7210_CHANNELS_MAX > 12
		unsigned int pdm_dmic_13_14_enable;	
		unsigned int pdm_dmic_15_16_enable;
	#endif
	
	unsigned int sysclk;
	struct snd_pcm_hw_constraint_list *sysclk_constraints;
	unsigned int tdm_mode;
	struct delayed_work pcm_pop_work;
	bool sclkinv;
};
struct snd_soc_codec *tron_codec1[ADC_DEV_MAXNUM];

int es7210_init_reg = 0;
static int es7210_codec_num = 0;

static const struct regmap_config es7210_regmap_config = {
	.reg_bits = 8,	//Number of bits in a register address
	.val_bits = 8,	//Number of bits in a register value
};
/*
* ES7210 register cache
*/
static const u8 es7210_reg[] = {
	0x32, 0x40, 0x02, 0x04,	0x01, 0x00, 0x00, 0x20,	/* 0 - 7 */
	0x10, 0x40, 0x40, 0x00,	0x00, 0x09, 0x00, 0x00,	/* 8 - F */
	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	/* 10 - 17 */
	0xf7, 0xf7, 0x00, 0xbf,	0xbf, 0xbf, 0xbf, 0x00,	/* 18 - 1f */
	0x26, 0x26, 0x06, 0x26,	0x00, 0x00, 0x00, 0x00,	/* 20 - 27 */
	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	/* 28 - 2f */
	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	/* 30 - 37 */
	0x00, 0x00, 0x00, 0x00,	0x00, 0x72, 0x10, 0x00,	/* 38 - 3f */
	0x80, 0x71, 0x71, 0x00,	0x00, 0x00, 0x00, 0x00,	/* 40 - 47 */
	0x00, 0x00, 0x00, 0xff,	0xff,			/* 48 - 4c */	
};
static const struct reg_default es7210_reg_defaults[] = {
        { 0x00, 0x32 }, //0
	{ 0x01, 0x40 }, 
	{ 0x02, 0x02 }, 
	{ 0x03, 0x04 }, 
	{ 0x04, 0x01 }, 
	{ 0x05, 0x00 }, 
	{ 0x06, 0x00 }, 
	{ 0x07, 0x20 }, 
        { 0x08, 0x10 }, 
	{ 0x09, 0x40 }, 
	{ 0x0a, 0x40 }, 
	{ 0x0b, 0x00 }, 
	{ 0x0c, 0x00 }, 
	{ 0x0d, 0x09 }, 
	{ 0x0e, 0x00 }, 
	{ 0x0f, 0x00 },
        { 0x10, 0x00 }, 
	{ 0x11, 0x00 }, 
	{ 0x12, 0x00 }, 
	{ 0x13, 0x00 }, 
	{ 0x14, 0x00 }, 
	{ 0x15, 0x00 }, 
	{ 0x16, 0x00 }, 
	{ 0x17, 0x00 }, 
        { 0x18, 0xf7 }, 
	{ 0x19, 0xf7 }, 
	{ 0x1a, 0x00 }, 
	{ 0x1b, 0xbf }, 
	{ 0x1c, 0xbf }, 
	{ 0x1d, 0xbf }, 
	{ 0x1e, 0xbf }, 
	{ 0x1f, 0x00 }, 
        { 0x20, 0x26 }, 
	{ 0x21, 0x26 }, 
	{ 0x22, 0x06 },
	{ 0x23, 0x26 }, 
        { 0x3d, 0x72 }, 
	{ 0x3e, 0x10 }, 
	{ 0x3f, 0x00 }, 
        { 0x40, 0x80 }, 
	{ 0x41, 0x71 }, 
	{ 0x42, 0x71 }, 
	{ 0x43, 0x00 }, 
	{ 0x44, 0x00 }, 
	{ 0x45, 0x00 }, 
	{ 0x46, 0x00 }, 
	{ 0x47, 0x00 }, 
        { 0x48, 0x00 }, 
	{ 0x49, 0x00 }, 
	{ 0x4a, 0x00 }, 
	{ 0x4b, 0xff }, 
	{ 0x4c, 0xff },                   
};
struct es7210_reg_config {
	unsigned char reg_addr;
	unsigned char reg_v;
};
static const struct es7210_reg_config es7210_tdm_reg_common_cfg1[] =  {
	{ 0x00, 0xFF },
	{ 0x00, 0x32 },
	{ 0x09, 0x30 },
	{ 0x0A, 0x30 },
	{ 0x23, 0x2a },
	{ 0x22, 0x0a },
	{ 0x21, 0x2a },
	{ 0x20, 0x0a },
};
static const struct es7210_reg_config es7210_tdm_reg_fmt_cfg[] =  {
	{ 0x11, 0x63 },
	{ 0x12, 0x01 },
};
static const struct es7210_reg_config es7210_tdm_reg_common_cfg2[] =  {
	{ 0x40, 0xC3 },
	{ 0x41, 0x70 },
	{ 0x42, 0x70 },
	{ 0x43, 0x1e },
	{ 0x44, 0x1e },
	{ 0x45, 0x1e },
	{ 0x46, 0x1e },
	{ 0x47, 0x08 },
	{ 0x48, 0x08 },
	{ 0x49, 0x08 },
	{ 0x4A, 0x08 },
	{ 0x07, 0x20 },
};
static const struct es7210_reg_config es7210_tdm_reg_mclk_cfg[] =  {
	{ 0x02, 0xC1 },
};
static const struct es7210_reg_config es7210_tdm_reg_common_cfg3[] =  {
	{ 0x06, 0x04 },
	{ 0x4B, 0x0F },
	{ 0x4C, 0x0F },
	{ 0x00, 0x71 },
	{ 0x00, 0x41 },
};
static int es7210_read(u8 reg, u8 *rt_value, struct i2c_client *client)
{
	int ret;
	u8 read_cmd[3] = {0};
	u8 cmd_len = 0;
	
	read_cmd[0] = reg;
	cmd_len = 1;
	
	if (client->adapter == NULL)
		pr_err("es7210_read client->adapter==NULL\n");
	
	ret = i2c_master_send(client, read_cmd, cmd_len);
	if (ret != cmd_len) {
		pr_err("es7210_read error1\n");
		return -1;
	}
	
	ret = i2c_master_recv(client, rt_value, 1);
	if (ret != 1) {
		pr_err("es7210_read error2, ret = %d.\n", ret);
		return -1;
	}
	
	return 0;
}
static int es7210_write(u8 reg, unsigned char value, struct i2c_client *client)
{
	int ret = 0;
	u8 write_cmd[2] = {0};
	
	write_cmd[0] = reg;
	write_cmd[1] = value;
	
	ret = i2c_master_send(client, write_cmd, 2);
	if (ret != 2) {
		pr_err("es7210_write error->[REG-0x%02x,val-0x%02x]\n",reg,value);
		return -1;
	}
	
	return 0;
}
static int es7210_update_bits(u8 reg, u8 mask, u8 value, struct i2c_client *client)
{
	u8 val_old,val_new;

	es7210_read(reg, &val_old, client);
	val_new = (val_old & ~mask) | (value & mask);
	if(val_new != val_old){
		es7210_write(reg, val_new, client);
	}

	return 0;
}
/*
static int es7210_multi_chips_read(u8 reg, unsigned char *rt_value)
{
	u8 i;

	for(i=0; i< ADC_DEV_MAXNUM; i++){
		es7210_read(reg, rt_value++, i2c_clt1[i]);
	}

	return 0;
}
*/
static int es7210_multi_chips_write(u8 reg, unsigned char value)
{
	u8 i;

	for(i=0; i< ADC_DEV_MAXNUM; i++){
		es7210_write(reg, value, i2c_clt1[i]);
	}
	
	return 0;
}

static int es7210_multi_chips_update_bits(u8 reg, u8 mask, u8 value)
{
	u8 i;
	
	for(i=0; i< ADC_DEV_MAXNUM; i++){
		es7210_update_bits(reg, mask, value, i2c_clt1[i]);
	}

	return 0;
}

/*
* Note that this should be called from init rather than from hw_params.
*/
static int es7210_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
	return 0;
}

static int es7210_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	return 0;
}
/*
* to initialize es7210 for tdm mode
*/
static void es7210_tdm_init_codec(u8 mode)
{
	int cnt, channel;

	for(cnt = 0; cnt < sizeof(es7210_tdm_reg_common_cfg1)/sizeof(es7210_tdm_reg_common_cfg1[0]); cnt++) {
		es7210_multi_chips_write(es7210_tdm_reg_common_cfg1[cnt].reg_addr, 
						es7210_tdm_reg_common_cfg1[cnt].reg_v);
	}
	switch(mode) {
		case ES7210_TDM_1LRCK_DSPA: 
			/*
			* Here to set TDM format for DSP-A mode
			*/
			for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
				es7210_write(ES7210_SDP_CFG1_REG11, 0x63, i2c_clt1[cnt]); 
			}
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG2_REG12, 0x01, i2c_clt1[cnt]);
                        }
			break;
                case ES7210_TDM_1LRCK_DSPB: 
                        /*
                        * Here to set TDM format for DSP-B mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x73, i2c_clt1[cnt]); 
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG2_REG12, 0x01, i2c_clt1[cnt]);
                        }

			break;
                case ES7210_TDM_1LRCK_I2S: 
                        /*
                        * Here to set TDM format for I2S mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x60, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG2_REG12, 0x02, i2c_clt1[cnt]);
                        }

			break;
                case ES7210_TDM_1LRCK_LJ: 
                        /*
                        * Here to set TDM format for Left Justified mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x61, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG2_REG12, 0x02, i2c_clt1[cnt]);
                        }
			break;
                case ES7210_TDM_NLRCK_DSPA: 
                        /*
                        * Here to set TDM format for DSP-A with multiple LRCK TDM mode
                        */
			channel = ES7210_CHANNELS_MAX;
			/*
			* Set the microphone numbers in array
			*/
			switch(channel) {
                        	case 2:
					es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x10);
					break;
				case 4:
					es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x20);
					break;
				case 6:
					es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x30);
					break;
				case 8:
					es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x40);
					break;
                                case 10: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x50);
                                        break;
                                case 12: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x60);
                                        break;
                                case 14: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x70);
                                        break;
                                case 16: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x80);
                                        break;
				default:
					break;
			}
			/*
			* set format, dsp-a with multiple LRCK tdm mode
			*/
			for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x63, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
				if(cnt == 0) {
					/*
					* set tdm flag in the interface chip
					*/
                                	es7210_write(ES7210_SDP_CFG2_REG12, 0x07, i2c_clt1[cnt]);
				} else {
					es7210_write(ES7210_SDP_CFG2_REG12, 0x03, i2c_clt1[cnt]);
				}
                        }
                        
			break;
                case ES7210_TDM_NLRCK_DSPB:
                        /*
                        * Here to set TDM format for DSP-B with multiple LRCK TDM mode
                        */
                        channel = ES7210_CHANNELS_MAX;
                        /*
                        * Set the microphone numbers in array
                        */
                        switch(channel) {
                                case 2:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x10);
                                        break;
                                case 4:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x20);
                                        break;
                                case 6:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x30);
                                        break;
                                case 8:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x40);
                                        break;
                                case 10: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x50);
                                        break;
                                case 12: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x60);
                                        break;
                                case 14: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x70);
                                        break;
                                case 16: 
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x80);
                                        break;
                                default:
                                        break;
                        }
                        /*
                        * set format, dsp-b with multiple LRCK tdm mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x73, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                if(cnt == 0) {
                                        /*
                                        * set tdm flag in the interface chip
                                        */
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x07, i2c_clt1[cnt]);
                                } else {
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x03, i2c_clt1[cnt]);
                                }
                        }
                        
			break;
                case ES7210_TDM_NLRCK_I2S:
                        /*
                        * Here to set TDM format for I2S with multiple LRCK TDM mode
                        */
                        channel = ES7210_CHANNELS_MAX;
                        /*
                        * Set the microphone numbers in array
                        */
                        switch(channel) {
                                case 2:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x10);
                                        break;
                                case 4:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x20);
                                        break;
                                case 6:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x30);
                                        break;
                                case 8:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x40);
                                        break;
                                case 10:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x50);
                                        break;
                                case 12:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x60);
                                        break;
                                case 14:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x70);
                                        break;
                                case 16:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x80);
                                        break;
                                default:
                                        break;
                        }
                        /*
                        * set format, I2S with multiple LRCK tdm mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x60, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                if(cnt == 0) {
                                        /*
                                        * set tdm flag in the interface chip
                                        */
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x07, i2c_clt1[cnt]);
                                } else {
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x03, i2c_clt1[cnt]);
                                }
                        }

			break;
                case ES7210_TDM_NLRCK_LJ:
                        /*
                        * Here to set TDM format for left justified with multiple LRCK TDM mode
                        */
                        channel = ES7210_CHANNELS_MAX;
                        /*
                        * Set the microphone numbers in array
                        */
                        switch(channel) {
                                case 2:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x10);
                                        break;
                                case 4:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x20);
                                        break;
                                case 6:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x30);
                                        break;
                                case 8:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x40);
                                        break;
                                case 10:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x50);
                                        break;
                                case 12:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x60);
                                        break;
                                case 14:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x70);
                                        break;
                                case 16:
                                        es7210_multi_chips_write(ES7210_MODE_CFG_REG08, 0x80);
                                        break;
                                default:
                                        break;
                        }
                        /*
                        * set format, left justified with multiple LRCK tdm mode
                        */
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                es7210_write(ES7210_SDP_CFG1_REG11, 0x61, i2c_clt1[cnt]);
                        }
                        for(cnt = 0; cnt < ADC_DEV_MAXNUM; cnt++) {
                                if(cnt == 0) {
                                        /*
                                        * set tdm flag in the interface chip
                                        */
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x07, i2c_clt1[cnt]);
                                } else {
                                        es7210_write(ES7210_SDP_CFG2_REG12, 0x03, i2c_clt1[cnt]);
                                }
                        }

                        break;
		case ES7210_NORMAL_I2S:
			/*
			* here to disable tdm and set i2s-16bit for normal mode
			*/
			es7210_multi_chips_write(ES7210_SDP_CFG1_REG11, 0x60); //i2s-16bits
			es7210_multi_chips_write(ES7210_SDP_CFG2_REG12, 0x00); //disable tdm
			break;
                case ES7210_NORMAL_LJ:
                        /*
                        * here to disable tdm and set left justified-16bit for normal mode
                        */
                        es7210_multi_chips_write(ES7210_SDP_CFG1_REG11, 0x61); //left justified-16bits
                        es7210_multi_chips_write(ES7210_SDP_CFG2_REG12, 0x00); //disable tdm
                        break;
                case ES7210_NORMAL_DSPA:
                        /*
                        * here to disable tdm and set dspa-16bit for normal mode
                        */
                        es7210_multi_chips_write(ES7210_SDP_CFG1_REG11, 0x63); //dspa-16bits
                        es7210_multi_chips_write(ES7210_SDP_CFG2_REG12, 0x00); //disable tdm
                        break;
                case ES7210_NORMAL_DSPB:
                        /*
                        * here to disable tdm and set dspb-16bit for normal mode
                        */
                        es7210_multi_chips_write(ES7210_SDP_CFG1_REG11, 0x73); //dspb-16bits
                        es7210_multi_chips_write(ES7210_SDP_CFG2_REG12, 0x00); //disable tdm
                        break;
		default:
	                /*
                        * here to disable tdm and set i2s-16bit for normal mode
                        */
                        es7210_multi_chips_write(ES7210_SDP_CFG1_REG11, 0x60); //i2s-16bits
                        es7210_multi_chips_write(ES7210_SDP_CFG2_REG12, 0x00); //disable tdm
                        break;

	}
        for(cnt = 0; cnt < sizeof(es7210_tdm_reg_common_cfg2)/sizeof(es7210_tdm_reg_common_cfg2[0]); cnt++) {
                es7210_multi_chips_write(es7210_tdm_reg_common_cfg2[cnt].reg_addr,
                                                es7210_tdm_reg_common_cfg2[cnt].reg_v);
        }
        switch(mode) {
                case ES7210_TDM_1LRCK_DSPA:
		case ES7210_TDM_1LRCK_DSPB:
		case ES7210_TDM_1LRCK_I2S:
		case ES7210_TDM_1LRCK_LJ:
			/*
			* to set internal mclk
			* here, we assume that cpu/soc always provides 256FS i2s clock to es7210.
			* dll bypassed, use clock doubler to get double frequency for internal modem which need
			* 512FS clock. the clk divider ratio is 1.
			* user must modify the setting of register0x02 according to FS ratio provided by CPU/SOC.
			*/
	                es7210_multi_chips_write(ES7210_MCLK_CTL_REG02, 0xc1);
			break;
		case ES7210_TDM_NLRCK_DSPA:
		case ES7210_TDM_NLRCK_DSPB:
		case ES7210_TDM_NLRCK_I2S:
		case ES7210_TDM_NLRCK_LJ:
                        /*
                        * to set internal mclk
                        * here, we assume that cpu/soc always provides 256FS i2s clock to es7210 and there is four
			* es7210 devices in tdm link. so the internal FS in es7210 is only FS/4; 
                        * dll bypassed, clock doubler bypassed. the clk divider ratio is 2. so the clock of internal
			* modem equals to (256FS / (FS/4) / 2) * FS = 512FS 
                        * user must modify the setting of register0x02 according to FS ratio provided by CPU/SOC.
                        */

			es7210_multi_chips_write(ES7210_MCLK_CTL_REG02, 0x82);
			break;
		default:
                        /*
                        * to set internal mclk for normal mode
                        * here, we assume that cpu/soc always provides 256FS i2s clock to es7210.
                        * dll bypassed, use clock doubler to get double frequency for internal modem which need
                        * 512FS clock. the clk divider ratio is 1.
                        * user must modify the setting of register0x02 according to FS ratio provided by CPU/SOC.
                        */
                        es7210_multi_chips_write(ES7210_MCLK_CTL_REG02, 0xc1);

			break;
	}
        for(cnt = 0; cnt < sizeof(es7210_tdm_reg_common_cfg3) / 
		sizeof(es7210_tdm_reg_common_cfg3[0]); cnt++) {
                es7210_multi_chips_write(es7210_tdm_reg_common_cfg3[cnt].reg_addr,
                                                es7210_tdm_reg_common_cfg3[cnt].reg_v);
        }
	/*
	* Mute All ADC
	*/	
	es7210_multi_chips_update_bits(ES7210_ADC34_MUTE_REG14, 0x03, 0x03);
	es7210_multi_chips_update_bits(ES7210_ADC12_MUTE_REG15, 0x03, 0x03);
	/*
	* Set Direct DB Gain
	*/
	es7210_multi_chips_update_bits(ES7210_ALC_COM_CFG2_REG1A,
			 0x03, 0x00);	//independent direct DB Gain Control for 4ch ADCs
	es7210_multi_chips_write(ES7210_ALC1_MAX_GAIN_REG1E, 0xBF); //0DB Direct Gain
	es7210_multi_chips_write(ES7210_ALC2_MAX_GAIN_REG1D, 0xBF); //0DB Direct Gain
	es7210_multi_chips_write(ES7210_ALC3_MAX_GAIN_REG1C, 0xBF); //0DB Direct Gain
	es7210_multi_chips_write(ES7210_ALC4_MAX_GAIN_REG1B, 0xBF); //0DB Direct Gain
}
static void es7210_unmute(void)
{
	printk("enter into %s\n", __func__);
        es7210_multi_chips_update_bits(ES7210_ADC34_MUTE_REG14, 0x03, 0x00);
        es7210_multi_chips_update_bits(ES7210_ADC12_MUTE_REG15, 0x03, 0x00);
}

static void pcm_pop_work_events(struct work_struct *work)
{
	printk("enter into %s\n", __func__);
	es7210_unmute();
	es7210_init_reg = 1;
}
static int es7210_mute(struct snd_soc_dai *dai, int mute)
{
	printk("enter into %s, mute = %d\n", __func__, mute);
	if (mute) {
		es7210_multi_chips_update_bits(ES7210_ADC34_MUTE_REG14, 0x03, 0x03);
		es7210_multi_chips_update_bits(ES7210_ADC12_MUTE_REG15, 0x03, 0x03);
        } else {
                es7210_multi_chips_update_bits(ES7210_ADC34_MUTE_REG14, 0x03, 0x00);
	        es7210_multi_chips_update_bits(ES7210_ADC12_MUTE_REG15, 0x03, 0x00);
	}
        return 0;
}

static int es7210_pcm_startup(struct snd_pcm_substream *substream,
			      struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;
        struct es7210_priv *es7210 = snd_soc_codec_get_drvdata(codec);

	if(es7210_init_reg == 0) {
		schedule_delayed_work(&es7210->pcm_pop_work, msecs_to_jiffies(100));
	}
	return 0;
}
static int es7210_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{ 
	return 0;
}

#define es7210_RATES SNDRV_PCM_RATE_8000_96000

#define es7210_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE)

static struct snd_soc_dai_ops es7210_ops = {
	.startup = es7210_pcm_startup,
	.hw_params = es7210_pcm_hw_params,
	.set_fmt = es7210_set_dai_fmt,
	.set_sysclk = es7210_set_dai_sysclk,
	.digital_mute = es7210_mute,
};
#if ES7210_CHANNELS_MAX > 0
static struct snd_soc_dai_driver es7210_dai0 = {
	.name = "ES7210 4CH ADC 0",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 4,
		.rates = es7210_RATES,
		.formats = es7210_FORMATS,
	 },
	.ops = &es7210_ops,
	.symmetric_rates = 1,
};
#endif
#if ES7210_CHANNELS_MAX > 4
static struct snd_soc_dai_driver es7210_dai1 = {
        .name = "ES7210 4CH ADC 1",
        .capture = {
                .stream_name = "Capture",
                .channels_min = 1,
                .channels_max = 4,
                .rates = es7210_RATES,
                .formats = es7210_FORMATS,
         },
        .ops = &es7210_ops,
        .symmetric_rates = 1,
};
#endif
#if ES7210_CHANNELS_MAX > 8
static struct snd_soc_dai_driver es7210_dai2 = {
        .name = "ES7210 4CH ADC 2",
        .capture = {
                .stream_name = "Capture",
                .channels_min = 1,
                .channels_max = 4,
                .rates = es7210_RATES,
                .formats = es7210_FORMATS,
         },
        .ops = &es7210_ops,
        .symmetric_rates = 1,
};
#endif
#if ES7210_CHANNELS_MAX > 12
static struct snd_soc_dai_driver es7210_dai3 = {
        .name = "ES7210 4CH ADC 3",
        .capture = {
                .stream_name = "Capture",
                .channels_min = 1,
                .channels_max = 4,
                .rates = es7210_RATES,
                .formats = es7210_FORMATS,
         },
        .ops = &es7210_ops,
        .symmetric_rates = 1,
};
#endif
static struct snd_soc_dai_driver *es7210_dai[] = {
#if ES7210_CHANNELS_MAX > 0
        &es7210_dai0,
#endif
#if ES7210_CHANNELS_MAX > 4
        &es7210_dai1,
#endif
#if ES7210_CHANNELS_MAX > 8
        &es7210_dai2,
#endif
#if ES7210_CHANNELS_MAX > 12
        &es7210_dai3,
#endif
};

static int es7210_suspend(struct snd_soc_codec *codec)
{

	return 0;
}

static int es7210_resume(struct snd_soc_codec *codec)
{
	snd_soc_cache_sync(codec);

	return 0;
}

static int es7210_probe(struct snd_soc_codec *codec)
{
	struct es7210_priv *es7210 = snd_soc_codec_get_drvdata(codec);
	int ret = 0;
#if !ES7210_CODEC_RW_TEST_EN
        //ret = snd_soc_codec_set_cache_io(codec, 8, 8, SND_SOC_I2C);//8,8
#else
        codec->control_data = devm_regmap_init_i2c(es7210->i2c, &es7210_regmap_config);
        ret = PTR_RET(codec->control_data);
#endif
	
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set cache I/O: %d\n", ret);
		return ret;
	}	
	printk("begin->>>>>>>>>>%s!\n",__func__);

	tron_codec1[es7210_codec_num++] = codec;
	INIT_DELAYED_WORK(&es7210->pcm_pop_work, pcm_pop_work_events);
	es7210_tdm_init_codec(es7210->tdm_mode);
	/*
	* Disable or Enable PDM DMIC interface
	*/

#if ES7210_CHANNELS_MAX > 0
	if(es7210->pdm_dmic_1_2_enable == ENABLE)
    	    es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x40, i2c_clt1[0]);
	else	
    	    es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x00, i2c_clt1[0]);

        if(es7210->pdm_dmic_3_4_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x80, i2c_clt1[0]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x00, i2c_clt1[0]);
#endif
#if ES7210_CHANNELS_MAX > 4
        if(es7210->pdm_dmic_5_6_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x40, i2c_clt1[1]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x00, i2c_clt1[1]);

        if(es7210->pdm_dmic_7_8_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x80, i2c_clt1[1]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x00, i2c_clt1[1]);
#endif
#if ES7210_CHANNELS_MAX > 8
        if(es7210->pdm_dmic_9_10_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x40, i2c_clt1[2]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x00, i2c_clt1[2]);

        if(es7210->pdm_dmic_11_12_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x80, i2c_clt1[2]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x00, i2c_clt1[2]);
#endif
#if ES7210_CHANNELS_MAX > 12
        if(es7210->pdm_dmic_13_14_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x40, i2c_clt1[3]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x40, 0x00, i2c_clt1[3]);

        if(es7210->pdm_dmic_15_16_enable == ENABLE)
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x80, i2c_clt1[3]);
        else
            es7210_update_bits(ES7210_DMIC_CTL_REG10, 0x80, 0x00, i2c_clt1[3]);
#endif
	
	if(es7210->sclkinv == true) {
		es7210_multi_chips_update_bits(ES7210_MODE_CFG_REG08, 0x08, 0x08);
	}
	/*
	* Schedule a delay work-quenue to avoid pop noise
	*/
	INIT_DELAYED_WORK(&es7210->pcm_pop_work, pcm_pop_work_events);
	return 0;
}

static int es7210_remove(struct snd_soc_codec *codec)
{
	return 0;
}

static const DECLARE_TLV_DB_SCALE(mic_boost_tlv, 0, 300, 0);
static const DECLARE_TLV_DB_SCALE(direct_gain_tlv, -9550, 50, 0);
#if ES7210_CHANNELS_MAX > 0
static int es7210_micboost1_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
    	es7210_update_bits(0x43, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[0]);
	return 0;
}

static int es7210_micboost1_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
    	u8 val;
	es7210_read(0x43, &val, i2c_clt1[0]);
	ucontrol->value.integer.value[0] = val;
    	return 0;
}

static int es7210_micboost2_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x44, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_micboost2_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x44, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_micboost3_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x45, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_micboost3_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x45, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost4_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x46, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_micboost4_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x46, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_adc1_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x01,
			 ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_adc1_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc2_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_adc2_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc3_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_adc3_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc4_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[0]);
        return 0;
}

static int es7210_adc4_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif

#if ES7210_CHANNELS_MAX > 4
static int es7210_micboost5_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x43, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_micboost5_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x43, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost6_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x44, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_micboost6_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x44, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost7_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x45, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_micboost7_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x45, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost8_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x46, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_micboost8_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x46, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_adc5_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_adc5_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc6_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_adc6_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_adc7_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_adc7_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc8_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[1]);
        return 0;
}

static int es7210_adc8_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif
#if ES7210_CHANNELS_MAX > 8
static int es7210_micboost9_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x43, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_micboost9_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x43, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost10_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x44, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_micboost10_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x44, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0; 
}
static int es7210_micboost11_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x45, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_micboost11_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x45, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0; 
}
static int es7210_micboost12_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x46, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_micboost12_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x46, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0; 
}

static int es7210_adc9_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_adc9_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc10_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_adc10_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc11_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_adc11_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc12_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[2]);
        return 0;
}

static int es7210_adc12_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
#endif
#if ES7210_CHANNELS_MAX > 12
static int es7210_micboost13_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x43, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_micboost13_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x43, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost14_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x44, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_micboost14_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x44, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost15_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x45, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_micboost15_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x45, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_micboost16_setting_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x46, 0x0F, ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_micboost16_setting_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x46, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc13_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_adc13_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc14_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC12_MUTE_REG15, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_adc14_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC12_MUTE_REG15, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc15_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x01,
                         ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_adc15_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_adc16_mute_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(ES7210_ADC34_MUTE_REG14, 0x02,
                         ucontrol->value.integer.value[0], i2c_clt1[3]);
        return 0;
}

static int es7210_adc16_mute_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(ES7210_ADC34_MUTE_REG14, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

#endif

////////////////////////////////////////////////////////////////
#if ES7210_CHANNELS_MAX > 0
static int es7210_direct_gain_1_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1e, 0xFF, ucontrol->value.integer.value[0], 
			i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_1_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1e, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_2_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1D, 0xFF, ucontrol->value.integer.value[0], 
                        i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_2_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1D, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_3_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1C, 0xFF, ucontrol->value.integer.value[0], 
                        i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_3_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1C, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_4_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1B, 0xFF, ucontrol->value.integer.value[0], 
                        i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_4_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1B, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_12_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x01, ucontrol->value.integer.value[0], 
                        i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_12_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_34_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x02, ucontrol->value.integer.value[0],
                        i2c_clt1[0]);
        return 0;
}

static int es7210_direct_gain_34_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[0]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
#endif

#if ES7210_CHANNELS_MAX > 4
static int es7210_direct_gain_5_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1e, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_5_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1e, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_6_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1D, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_6_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1D, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_7_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1C, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_7_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1C, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_8_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1B, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_8_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1B, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_56_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x01, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_56_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_78_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x02, ucontrol->value.integer.value[0],
                        i2c_clt1[1]);
        return 0;
}

static int es7210_direct_gain_78_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[1]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
#endif

#if ES7210_CHANNELS_MAX > 8
static int es7210_direct_gain_9_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1e, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}

static int es7210_direct_gain_9_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1e, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_10_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1D, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}

static int es7210_direct_gain_10_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1D, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_11_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1C, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}
static int es7210_direct_gain_11_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1C, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_12_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1B, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}

static int es7210_direct_gain_12_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1B, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_9_10_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x01, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}

static int es7210_direct_gain_9_10_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_11_12_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x02, ucontrol->value.integer.value[0],
                        i2c_clt1[2]);
        return 0;
}

static int es7210_direct_gain_11_12_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[2]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
#endif

#if ES7210_CHANNELS_MAX > 12
static int es7210_direct_gain_13_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1e, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}

static int es7210_direct_gain_13_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1e, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_14_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1D, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}

static int es7210_direct_gain_14_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1D, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_15_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1C, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}
static int es7210_direct_gain_15_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1C, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_16_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1B, 0xFF, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}

static int es7210_direct_gain_16_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1B, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}

static int es7210_direct_gain_13_14_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x01, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}

static int es7210_direct_gain_13_14_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
static int es7210_direct_gain_15_16_sameon_set(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        es7210_update_bits(0x1a, 0x02, ucontrol->value.integer.value[0],
                        i2c_clt1[3]);
        return 0;
}

static int es7210_direct_gain_15_16_sameon_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
        u8 val;
        es7210_read(0x1a, &val, i2c_clt1[3]);
        ucontrol->value.integer.value[0] = val;
        return 0;
}
#endif

static const struct snd_kcontrol_new es7210_snd_controls[] = {
#if ES7210_CHANNELS_MAX > 0
	SOC_SINGLE_EXT_TLV("PGA1_setting", 0x43, 0, 0x0F, 0,
                       	es7210_micboost1_setting_get, 
			es7210_micboost1_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA2_setting", 0x44, 0, 0x0F, 0,
                       	es7210_micboost2_setting_get, 
			es7210_micboost2_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA3_setting", 0x45, 0, 0x0F, 0,
                       	es7210_micboost3_setting_get, 
			es7210_micboost3_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA4_setting", 0x46, 0, 0x0F, 0,
                       	es7210_micboost4_setting_get, 
			es7210_micboost4_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT("ADC1_MUTE", ES7210_ADC12_MUTE_REG15, 0, 1, 0,
			es7210_adc1_mute_get, es7210_adc1_mute_set),
    	SOC_SINGLE_EXT("ADC2_MUTE", ES7210_ADC12_MUTE_REG15, 1, 1, 0,
			es7210_adc2_mute_get, es7210_adc2_mute_set),
    	SOC_SINGLE_EXT("ADC3_MUTE", ES7210_ADC34_MUTE_REG14, 0, 1, 0,
                        es7210_adc3_mute_get, es7210_adc3_mute_set),
    	SOC_SINGLE_EXT("ADC4_MUTE", ES7210_ADC34_MUTE_REG14, 1, 1, 0,
                        es7210_adc4_mute_get, es7210_adc4_mute_set),
    
    	SOC_SINGLE_EXT_TLV("ADC1_DIRECT_GAIN", ES7210_ALC1_MAX_GAIN_REG1E, 0, 0xff, 0,
                        es7210_direct_gain_1_get, es7210_direct_gain_1_set,
			direct_gain_tlv),
    	SOC_SINGLE_EXT_TLV("ADC2_DIRECT_GAIN", ES7210_ALC2_MAX_GAIN_REG1D, 0, 0xff, 0,
                        es7210_direct_gain_2_get, es7210_direct_gain_2_set,
                        direct_gain_tlv),
    	SOC_SINGLE_EXT_TLV("ADC3_DIRECT_GAIN", ES7210_ALC3_MAX_GAIN_REG1C, 0, 0xff, 0,
                        es7210_direct_gain_3_get, es7210_direct_gain_3_set,
                        direct_gain_tlv),
    	SOC_SINGLE_EXT_TLV("ADC4_DIRECT_GAIN", ES7210_ALC4_MAX_GAIN_REG1B, 0, 0xff, 0,
                        es7210_direct_gain_4_get, es7210_direct_gain_4_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT("DIRECT_1_2_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 0, 1, 0,
                        es7210_direct_gain_12_sameon_get, 
			es7210_direct_gain_12_sameon_set),
        SOC_SINGLE_EXT("DIRECT_3_4_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 1, 1, 0,
                        es7210_direct_gain_34_sameon_get, 
			es7210_direct_gain_34_sameon_set),
#endif

#if ES7210_CHANNELS_MAX > 4
    	SOC_SINGLE_EXT_TLV("PGA5_setting", 0x43, 0, 0x0F, 0,
                       	es7210_micboost5_setting_get, 
			es7210_micboost5_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA6_setting", 0x44, 0, 0x0F, 0,
                       	es7210_micboost6_setting_get, 
			es7210_micboost6_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA7_setting", 0x45, 0, 0x0F, 0,
                       	es7210_micboost7_setting_get, 
			es7210_micboost7_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA8_setting", 0x46, 0, 0x0F, 0,
                       	es7210_micboost8_setting_get, 
			es7210_micboost8_setting_set,
                       	mic_boost_tlv),

    	SOC_SINGLE_EXT("ADC5_MUTE", ES7210_ADC12_MUTE_REG15, 0, 1, 0,
                        es7210_adc5_mute_get, es7210_adc5_mute_set),
    	SOC_SINGLE_EXT("ADC6_MUTE", ES7210_ADC12_MUTE_REG15, 1, 1, 0,
                        es7210_adc6_mute_get, es7210_adc6_mute_set),
    	SOC_SINGLE_EXT("ADC7_MUTE", ES7210_ADC34_MUTE_REG14, 0, 1, 0,
                        es7210_adc7_mute_get, es7210_adc7_mute_set),
	SOC_SINGLE_EXT("ADC8_MUTE", ES7210_ADC34_MUTE_REG14, 1, 1, 0,
                        es7210_adc8_mute_get, es7210_adc8_mute_set),

        SOC_SINGLE_EXT_TLV("ADC5_DIRECT_GAIN", ES7210_ALC1_MAX_GAIN_REG1E, 0, 0xff, 0,
                        es7210_direct_gain_5_get, es7210_direct_gain_5_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC6_DIRECT_GAIN", ES7210_ALC2_MAX_GAIN_REG1D, 0, 0xff, 0,
                        es7210_direct_gain_6_get, es7210_direct_gain_6_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC7_DIRECT_GAIN", ES7210_ALC3_MAX_GAIN_REG1C, 0, 0xff, 0,
                        es7210_direct_gain_7_get, es7210_direct_gain_7_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC8_DIRECT_GAIN", ES7210_ALC4_MAX_GAIN_REG1B, 0, 0xff, 0,
                        es7210_direct_gain_8_get, es7210_direct_gain_8_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT("DIRECT_5_6_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 0, 1, 0,
                        es7210_direct_gain_56_sameon_get, 
			es7210_direct_gain_56_sameon_set),
        SOC_SINGLE_EXT("DIRECT_7_8_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 1, 1, 0,
                        es7210_direct_gain_78_sameon_get, 
			es7210_direct_gain_78_sameon_set),
#endif
#if ES7210_CHANNELS_MAX > 8
    	SOC_SINGLE_EXT_TLV("PGA9_setting", 0x43, 0, 0x0F, 0,
                       	es7210_micboost9_setting_get, 
			es7210_micboost9_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA10_setting", 0x44, 0, 0x0F, 0,
                       	es7210_micboost10_setting_get, 
			es7210_micboost10_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA11_setting", 0x45, 0, 0x0F, 0,
                       	es7210_micboost11_setting_get, 
			es7210_micboost11_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA12_setting", 0x46, 0, 0x0F, 0,
                       	es7210_micboost12_setting_get, 
			es7210_micboost12_setting_set,
                       	mic_boost_tlv),

    	SOC_SINGLE_EXT("ADC9_MUTE", ES7210_ADC12_MUTE_REG15, 0, 1, 0,
                        es7210_adc9_mute_get, es7210_adc9_mute_set),
    	SOC_SINGLE_EXT("ADC10_MUTE", ES7210_ADC12_MUTE_REG15, 1, 1, 0,
                        es7210_adc10_mute_get, es7210_adc10_mute_set),
    	SOC_SINGLE_EXT("ADC11_MUTE", ES7210_ADC34_MUTE_REG14, 0, 1, 0,
                        es7210_adc11_mute_get, es7210_adc11_mute_set),
    	SOC_SINGLE_EXT("ADC12_MUTE", ES7210_ADC34_MUTE_REG14, 1, 1, 0,
                        es7210_adc12_mute_get, es7210_adc12_mute_set),

        SOC_SINGLE_EXT_TLV("ADC9_DIRECT_GAIN", ES7210_ALC1_MAX_GAIN_REG1E, 0, 0xff, 0,
                        es7210_direct_gain_9_get, es7210_direct_gain_9_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC10_DIRECT_GAIN", ES7210_ALC2_MAX_GAIN_REG1D, 0, 0xff, 0,
                        es7210_direct_gain_10_get, es7210_direct_gain_10_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC11_DIRECT_GAIN", ES7210_ALC3_MAX_GAIN_REG1C, 0, 0xff, 0,
                        es7210_direct_gain_11_get, es7210_direct_gain_11_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC12_DIRECT_GAIN", ES7210_ALC4_MAX_GAIN_REG1B, 0, 0xff, 0,
                        es7210_direct_gain_12_get, es7210_direct_gain_12_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT("DIRECT_9_10_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 0, 1, 0,
                        es7210_direct_gain_9_10_sameon_get, 
			es7210_direct_gain_9_10_sameon_set),
        SOC_SINGLE_EXT("DIRECT_11_12_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 1, 1, 0,
                        es7210_direct_gain_11_12_sameon_get, 
			es7210_direct_gain_11_12_sameon_set),

#endif
#if ES7210_CHANNELS_MAX > 12
    	SOC_SINGLE_EXT_TLV("PGA13_setting", 0x43, 0, 0x0F, 0,
                       	es7210_micboost13_setting_get, 
			es7210_micboost13_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA14_setting", 0x44, 0, 0x0F, 0,
                       	es7210_micboost14_setting_get, 
			es7210_micboost14_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA15_setting", 0x45, 0, 0x0F, 0,
                       	es7210_micboost15_setting_get, 
			es7210_micboost15_setting_set,
                       	mic_boost_tlv),
    	SOC_SINGLE_EXT_TLV("PGA16_setting", 0x46, 0, 0x0F, 0,
                       	es7210_micboost16_setting_get, 
		       	es7210_micboost16_setting_set,
                       	mic_boost_tlv),
    	
	SOC_SINGLE_EXT("ADC13_MUTE", ES7210_ADC12_MUTE_REG15, 0, 1, 0,
                        es7210_adc13_mute_get, es7210_adc13_mute_set),
    	SOC_SINGLE_EXT("ADC14_MUTE", ES7210_ADC12_MUTE_REG15, 1, 1, 0,
                        es7210_adc14_mute_get, es7210_adc14_mute_set),
    	SOC_SINGLE_EXT("ADC15_MUTE", ES7210_ADC34_MUTE_REG14, 0, 1, 0,
                        es7210_adc15_mute_get, es7210_adc15_mute_set),
    	SOC_SINGLE_EXT("ADC16_MUTE", ES7210_ADC34_MUTE_REG14, 1, 1, 0,
                        es7210_adc16_mute_get, es7210_adc16_mute_set),

        SOC_SINGLE_EXT_TLV("ADC13_DIRECT_GAIN", ES7210_ALC1_MAX_GAIN_REG1E, 0, 0xff, 0,
                        es7210_direct_gain_13_get, es7210_direct_gain_13_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC14_DIRECT_GAIN", ES7210_ALC2_MAX_GAIN_REG1D, 0, 0xff, 0,
                        es7210_direct_gain_14_get, es7210_direct_gain_14_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC15_DIRECT_GAIN", ES7210_ALC3_MAX_GAIN_REG1C, 0, 0xff, 0,
                        es7210_direct_gain_15_get, es7210_direct_gain_15_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT_TLV("ADC16_DIRECT_GAIN", ES7210_ALC4_MAX_GAIN_REG1B, 0, 0xff, 0,
                        es7210_direct_gain_16_get, es7210_direct_gain_16_set,
                        direct_gain_tlv),
        SOC_SINGLE_EXT("DIRECT_13_14_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 0, 1, 0,
                        es7210_direct_gain_13_14_sameon_get, 
			es7210_direct_gain_13_14_sameon_set),
        SOC_SINGLE_EXT("DIRECT_15_16_GAIN_SAME_ON", ES7210_ALC_COM_CFG2_REG1A, 1, 1, 0,
                        es7210_direct_gain_15_16_sameon_get, 
			es7210_direct_gain_15_16_sameon_set),


#endif

};

static struct snd_soc_codec_driver soc_codec_dev_es7210 = {
	.probe = es7210_probe,
	.remove = es7210_remove,
	.suspend = es7210_suspend,
	.resume = es7210_resume,
	.reg_word_size = sizeof(u8),
	.reg_cache_default = es7210_reg_defaults,
	.reg_cache_size = ARRAY_SIZE(es7210_reg_defaults),
	.controls = es7210_snd_controls,
	.num_controls = ARRAY_SIZE(es7210_snd_controls),
};

static ssize_t es7210_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int val=0, flag=0;
	u8 i=0, reg, num, value_w, value_r;
	
	struct es7210_priv *es7210 = dev_get_drvdata(dev);
	val = simple_strtol(buf, NULL, 16);
	flag = (val >> 16) & 0xFF;
	
	if (flag) {
		reg = (val >> 8) & 0xFF;
		value_w = val & 0xFF;
		printk("\nWrite: start REG:0x%02x,val:0x%02x,count:0x%02x\n", reg, value_w, flag);
		while(flag--) {
			es7210_write(reg, value_w, es7210->i2c);
			printk("Write 0x%02x to REG:0x%02x\n", value_w, reg);
			reg++;
		}
	} else {
		reg = (val >> 8) & 0xFF;
		num = val & 0xff;
		printk("\nRead: start REG:0x%02x,count:0x%02x\n", reg, num);
		do {
			value_r = 0;
			es7210_read(reg, &value_r, es7210->i2c);
			printk("REG[0x%02x]: 0x%02x;  ", reg, value_r);
			reg++;
			i++;
			if ((i==num) || (i%4==0))	printk("\n");
		} while (i<num);
	}
	
	return count;
}

static ssize_t es7210_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("echo flag|reg|val > es7210\n");
	printk("eg read star addres=0x06,count 0x10:echo 0610 >es7210\n");
	printk("eg write star addres=0x90,value=0x3c,count=4:echo 4903c >es7210\n");
	//printk("eg write value:0xfe to address:0x06 :echo 106fe > es7243\n");
	return 0;
}

static DEVICE_ATTR(es7210, 0644, es7210_show, es7210_store);

static struct attribute *es7210_debug_attrs[] = {
	&dev_attr_es7210.attr,
	NULL,
};

static struct attribute_group es7210_debug_attr_group = {
	.name   = "es7210_debug",
	.attrs  = es7210_debug_attrs,
};

/*
 * If the i2c layer weren't so broken, we could pass this kind of data
 * around
 */
static int es7210_i2c_probe(struct i2c_client *i2c,
				   const struct i2c_device_id *i2c_id)
{
	struct es7210_priv *es7210;
	int ret;
	
	printk("begin->>>>>>>>>>%s!\n",__func__);
	
	es7210 = devm_kzalloc(&i2c->dev, sizeof(struct es7210_priv), GFP_KERNEL);
	if (es7210 == NULL)
		return -ENOMEM;
  	es7210->i2c = i2c;
	es7210->sclkinv = true;  //sclk inverted
	es7210->tdm_mode =  ES7210_WORK_MODE;  //to set tdm mode or normal mode
	#if ES7210_CHANNELS_MAX > 0
		es7210->pdm_dmic_1_2_enable = DISABLE;	// disable pdm dmic interface
		es7210->pdm_dmic_3_4_enable = DISABLE;	// disable pdm dmic interface
	#endif
	#if ES7210_CHANNELS_MAX > 4
		es7210->pdm_dmic_5_6_enable = DISABLE;	// disable pdm dmic interface
		es7210->pdm_dmic_7_8_enable = DISABLE;	// disable pdm dmic interface
	#endif
	#if ES7210_CHANNELS_MAX > 8
		es7210->pdm_dmic_9_10_enable = DISABLE;	// disable pdm dmic interface
		es7210->pdm_dmic_11_12_enable = DISABLE;// disable pdm dmic interface
	#endif
	#if ES7210_CHANNELS_MAX > 12
		es7210->pdm_dmic_13_14_enable = DISABLE;// disable pdm dmic interface
		es7210->pdm_dmic_15_16_enable = DISABLE;// disable pdm dmic interface
	#endif
	dev_set_drvdata(&i2c->dev, es7210);
	if (i2c_id->driver_data < ADC_DEV_MAXNUM) {
		i2c_clt1[i2c_id->driver_data] = i2c;
		ret = snd_soc_register_codec(&i2c->dev,&soc_codec_dev_es7210, 
						es7210_dai[i2c_id->driver_data], 1);
		if (ret < 0) {
			kfree(es7210);
			return ret;
		}
	}
	ret = sysfs_create_group(&i2c->dev.kobj, &es7210_debug_attr_group);
	if (ret) {
		pr_err("failed to create attr group\n");
	}
	return ret;
}
static int __exit es7210_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
	kfree(i2c_get_clientdata(i2c));
	return 0;
}

static int es7210_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if (adapter->nr == ES7210_I2C_BUS_NUM) {
		if(client->addr == 0x40) {
			strlcpy(info->type, "MicArray_0", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x43) {
			strlcpy(info->type, "MicArray_1", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x42) {
			strlcpy(info->type, "MicArray_2", I2C_NAME_SIZE);
			return 0;
		} else if (client->addr == 0x41) {
			strlcpy(info->type, "MicArray_3", I2C_NAME_SIZE);
			return 0;
		}
	}

	return -ENODEV;
}

static const unsigned short es7210_i2c_addr[] = {
#if ES7210_CHANNELS_MAX > 0
	0x40,
#endif

#if ES7210_CHANNELS_MAX > 4
	0x43,
#endif

#if ES7210_CHANNELS_MAX > 8
	0x42,
#endif

#if ES7210_CHANNELS_MAX > 12
	0x41,
#endif

	I2C_CLIENT_END,
};

/*
* device tree source or i2c_board_info both use to transfer hardware information to linux kernel, 
* use one of them wil be OK
*/
static struct i2c_board_info es7210_i2c_board_info[] = {
#if ES7210_CHANNELS_MAX > 0
	{I2C_BOARD_INFO("MicArray_0", 0x40),},//es7210_0
#endif

#if ES7210_CHANNELS_MAX > 4
	{I2C_BOARD_INFO("MicArray_1", 0x43),},//es7210_1
#endif

#if ES7210_CHANNELS_MAX > 8
	{I2C_BOARD_INFO("MicArray_2", 0x42),},//es7210_2
#endif

#if ES7210_CHANNELS_MAX > 12
	{I2C_BOARD_INFO("MicArray_3", 0x41),},//es7210_3
#endif
};

static const struct i2c_device_id es7210_i2c_id[] = {
#if ES7210_CHANNELS_MAX > 0
	{ "MicArray_0", 0 },//es7210_0
#endif

#if ES7210_CHANNELS_MAX > 4
	{ "MicArray_1", 1 },//es7210_1
#endif

#if ES7210_CHANNELS_MAX > 8
	{ "MicArray_2", 2 },//es7210_2
#endif

#if ES7210_CHANNELS_MAX > 12
	{ "MicArray_3", 3 },//es7210_3
#endif
	{ }
};
MODULE_DEVICE_TABLE(i2c, es7210_i2c_id);

static const struct of_device_id es7210_dt_ids[] = {
#if ES7210_CHANNELS_MAX > 0
	{ .compatible = "MicArray_0", },//es7210_0
#endif

#if ES7210_CHANNELS_MAX > 4
	{ .compatible = "MicArray_1", },//es7210_1
#endif

#if ES7210_CHANNELS_MAX > 8
	{ .compatible = "MicArray_2", },//es7210_2
#endif

#if ES7210_CHANNELS_MAX > 12
	{ .compatible = "MicArray_3", },//es7210_3
#endif
};
MODULE_DEVICE_TABLE(of, es7210_dt_ids);

static struct i2c_driver es7210_i2c_driver = {
	.driver = {
		   	.name = "es7210",
			.owner = THIS_MODULE,
		#if ES7210_MATCH_DTS_EN
				.of_match_table = es7210_dt_ids,
		#endif		   
		   },
	.probe = es7210_i2c_probe,
	.remove = __exit_p(es7210_i2c_remove),
	.class  = I2C_CLASS_HWMON,
	.id_table = es7210_i2c_id,
#if !ES7210_MATCH_DTS_EN
	.address_list = es7210_i2c_addr,
	.detect = es7210_i2c_detect,
#endif
};


static int __init es7210_modinit(void)
{
	int ret,i;
	struct i2c_adapter *adapter;
    	struct i2c_client *client;
	printk("%s enter es7210\n",__func__);

	adapter = i2c_get_adapter(ES7210_I2C_BUS_NUM);
    	if (!adapter) {
        	printk("i2c_get_adapter() fail!\n");
        	return -ENODEV;
    	}
	printk("%s() begin0000",__func__);

    	for(i = 0; i < ADC_DEV_MAXNUM; i++) {
        	client = i2c_new_device(adapter, &es7210_i2c_board_info[i]);
        	printk("%s() i2c_new_device\n",__func__);
        	if (!client)
            	return -ENODEV;
    	}
	i2c_put_adapter(adapter);
	ret = i2c_add_driver(&es7210_i2c_driver);
	if (ret != 0)
		pr_err("Failed to register es7210 i2c driver : %d \n", ret);
	return ret;
}
module_init(es7210_modinit);
static void __exit es7210_exit(void)
{
	i2c_del_driver(&es7210_i2c_driver);
}
module_exit(es7210_exit);
MODULE_DESCRIPTION("ASoC ES7210 audio adc driver");
MODULE_AUTHOR("David Yang");
MODULE_LICENSE("GPL v2");

