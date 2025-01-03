/*
 * ALSA SoC ES7243 codec driver
 *
 * Author:   
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * About How to integrate ES7243 to rootfs
 * Step 1:
 *  vi src/sound/soc/codecs/Makefile
 *  Remove "obj-$(CONFIG_SND_SOC_TLV320WN)	+= tlv320wn.o"
 *  Add "obj-y += es7243.o"
 * Step 2:
 *  vi src/sound/soc/mediatek/mt8167/mt8516-p1v2.c
 *  search "mt8516_p1v2_dais"
 *  change change the codec_name of TDM Capture to snd-soc-dummy
 *  change change the codec_dai_name of TDM Capture to snd-soc-dummy-dai
 *  change the codec_name and codec_dai_name of TDM IN BE as the follows.
 *      {
 *           .name = "TDM IN BE",
 *           .cpu_dai_name = "TDM_IN_IO",
 *           .no_pcm = 1,
 *           .codec_name = "es7243.0-0010",
 *           .codec_dai_name = "ES7243 HiFi 0",
 *           .dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
 *                      SND_SOC_DAIFMT_CBS_CFS,
 *           .dpcm_capture = 1,
 *       },
 */

#ifndef _ES7243_H
#define _ES7243_H

/* Codec TLV320AIC23 */
#define ES7243_MODECFG_REG00		0x00
#define ES7243_SDPFMT_REG01		0x01
#define ES7243_LRCDIV_REG02		0x02
#define ES7243_BCKDIV_REG03		0x03
#define ES7243_CLKDIV_REG04		0x04
#define ES7243_MUTECTL_REG05		0x05
#define ES7243_STATECTL_REG06			0x06
#define ES7243_ANACTL0_REG07		0x07
#define ES7243_ANACTL1_REG08		0x08
#define ES7243_ANACTL2_REG09		0x09
#define ES7243_ANACHARG_REG0A		0x0A
#define ES7243_INISTATE_REG0B		0x0B
#define ES7243_BIAS_REG0C		0x0C
#define ES7243_STMOSR_REG0D		0x0D
#define ES7243_CHIPID_REG0E		0x0E

#endif /* _ES7243_H_ */