/*
 * mt8167_afe_regs.h  --  Mediatek audio register definitions
 *
 * Copyright (c) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MT8167_AFE_REGS_H_
#define _MT8167_AFE_REGS_H_

#include <linux/bitops.h>

/*****************************************************************************
 *                  R E G I S T E R       D E F I N I T I O N
 *****************************************************************************/
#define AUDIO_TOP_CON0		0x0000
#define AUDIO_TOP_CON1		0x0004
#define AUDIO_TOP_CON3		0x000c
#define AFE_DAC_CON0		0x0010
#define AFE_DAC_CON1		0x0014
#define AFE_I2S_CON		0x0018
#define AFE_I2S_CON1		0x0034
#define AFE_I2S_CON2		0x0038
#define AFE_I2S_CON3		0x004c
#define AFE_DAIBT_CON0          0x001c
#define AFE_MRGIF_CON           0x003c
#define AFE_CONN_24BIT		0x006c

#define AFE_CONN0		0x0020
#define AFE_CONN1		0x0024
#define AFE_CONN2		0x0028
#define AFE_CONN3		0x002C
#define AFE_CONN4		0x0030
#define AFE_CONN5		0x005C
#define AFE_HDMI_CONN0		0x0390
#define AFE_CONN_TDMIN_CON	0x039C

#define AFE_GAIN1_CON0		0x0410
#define AFE_GAIN1_CON1		0x0414
#define AFE_GAIN1_CON2		0x0418
#define AFE_GAIN1_CON3		0x041c
#define AFE_GAIN1_CONN		0x0420
#define AFE_GAIN1_CUR		0x0424
#define AFE_GAIN1_CONN2		0x0448

/* Memory interface */
#define AFE_DL1_BASE		0x0040
#define AFE_DL1_CUR		0x0044
#define AFE_DL1_END		0x0048
#define AFE_DL2_BASE		0x0050
#define AFE_DL2_CUR		0x0054
#define AFE_DL2_END             0x0058
#define AFE_AWB_BASE		0x0070
#define AFE_AWB_END             0x0078
#define AFE_AWB_CUR		0x007c
#define AFE_VUL_BASE		0x0080
#define AFE_VUL_CUR		0x008c
#define AFE_VUL_END		0x0088
#define AFE_DAI_BASE		0x0090
#define AFE_DAI_END		0x0098
#define AFE_DAI_CUR		0x009c
#define AFE_MOD_PCM_BASE	0x0330
#define AFE_MOD_PCM_END		0x0338
#define AFE_MOD_PCM_CUR		0x033c
#define AFE_HDMI_OUT_BASE	0x0374
#define AFE_HDMI_OUT_CUR	0x0378
#define AFE_HDMI_OUT_END	0x037c
#define AFE_SPDIF_OUT_BASE  0x0384
#define AFE_SPDIF_OUT_END   0x038c
#define AFE_SPDIF_OUT_CUR   0x0388

#define AFE_MEMIF_MSB           0x00cc
#define AFE_MEMIF_MON0          0x00d0
#define AFE_MEMIF_MON1          0x00d4
#define AFE_MEMIF_MON2          0x00d8
#define AFE_MEMIF_MON3          0x00dc

#define AFE_ADDA_DL_SRC2_CON0   0x0108
#define AFE_ADDA_DL_SRC2_CON1   0x010c
#define AFE_ADDA_UL_SRC_CON0    0x0114
#define AFE_ADDA_UL_SRC_CON1    0x0118
#define AFE_ADDA_TOP_CON0	0x0120
#define AFE_ADDA_UL_DL_CON0     0x0124
#define AFE_ADDA_NEWIF_CFG0     0x0138
#define AFE_ADDA_NEWIF_CFG1     0x013c
#define AFE_ADDA_PREDIS_CON0    0x0260
#define AFE_ADDA_PREDIS_CON1    0x0264

#define AFE_SGEN_CON0		0x01f0
#define AFE_SINEGEN_CON_TDM	0x01f8
#define AFE_SINEGEN_CON_TDM_IN	0x01fc

#define AFE_HDMI_OUT_CON0	0x0370
#define AFE_SPDIF_OUT_CON0  0x0380
#define AFE_HDMI_CONN0      0x0390

#define AFE_IEC_CFG         0x0480
#define AFE_IEC_NSNUM       0x0484
#define AFE_IEC_BURST_INFO  0x0488
#define AFE_IEC_BURST_LEN   0x048C
#define AFE_IEC_NSADR       0x0490
#define AFE_IEC_CHL_STAT0   0x04A0
#define AFE_IEC_CHL_STAT1   0x04A4
#define AFE_IEC_CHR_STAT0   0x04A8
#define AFE_IEC_CHR_STAT1   0x04AC
#define IEC_BURST_NOT_READY_MASK (1 << 16)
#define IEC_BURST_NOT_READY      (1 << 16)
#define IEC_BURST_READY          (0 << 16)

/*AFE_SPDIF_OUT_CON0*/
#define SPDIF_OUT_CLOCK_ON_OFF_MASK  1
#define SPDIF_OUT_CLOCK_ON       1
#define SPDIF_OUT_CLOCK_OFF      0
#define SPDIF_OUT_MEMIF_ON_OFF_MASK  (1 << 1)
#define SPDIF_OUT_MEMIF_ON       (1 << 1)
#define SPDIF_OUT_MEMIF_OFF      0
#define IEC_NEXT_SAM_NUM_MASK    0x3FFF
#define IEC_INT_SAM_NUM_MASK     (0x3FFF << 16)
#define IEC_BURST_INFO_MASK      0xFFFF
#define IEC_BURST_LEN_MASK    0x7FFFF

/* AFE_IEC_CFG */
#define IEC_RAW_SEL_MASK       (0x1)
#define IEC_RAW_SEL_COOK   (0x0)
#define IEC_RAW_SEL_RAW    (0x1)
#define IEC_PCM_SEL_MASK       (0x2)
#define IEC_PCM_SEL_PCM    (0x0)
#define IEC_PCM_SEL_ENC    (0x2)
#define IEC_MUTE_DATA_MASK     (0x1 << 3)
#define IEC_MUTE_DATA_NORMAL (0x0)
#define IEC_MUTE_DATA_MUTE   (0x1 << 3)
#define IEC_VALID_DATA_MASK    (0x1 << 4)
#define IEC_INVALID_DATA   (0x0 << 4)
#define IEC_VALID_DATA     (0x1 << 4)
#define IEC_RAW_24BIT_MODE_MASK            (0x1 << 5)
#define IEC_RAW_24BIT_MODE_ON          (0x1 << 5)
#define IEC_RAW_24BIT_MODE_OFF         (0x0 << 5)
#define IEC_RAW_24BIT_SWITCH_MODE_MASK     (0x1 << 6)
#define IEC_RAW_24BIT_SWITCH_MODE_ON   (0x1 << 6)
#define IEC_RAW_24BIT_SWITCH_MODE_OFF  (0x0 << 6)
#define IEC_MAT_LEN_MASK       (0x1 << 7)
#define IEC_MAT_CRC_EN_MASK    (0x1 << 8)
#define IEC_DST_BIT_REV_MASK   (0x1 << 9)
#define IEC_EN_MASK            (0x1 << 16)
#define IEC_ENABLE             (0x1 << 16)
#define IEC_DISABLE            (0x0 << 16)
#define IEC_RISC_SWAP_IEC_BYTE_MASK (0x1 << 17)
#define IEC_DOWN_SAMPLE_MASK   (0x3 << 18)
#define IEC_DOWN_SAMPLE_0  (0x0 << 18)
#define IEC_DOWN_SAMPLE_2  (0x1 << 18)
#define IEC_DOWN_SAMPLE_4  (0x3 << 18)
#define IEC_FORCE_UPDATE_MASK  (0x1 << 20)
#define IEC_FORCE_UPDATE       (0x1 << 20)
#define IEC_MUTE_MASK          (0x1 << 21)
#define IEC_REG_LOCK_EN_MASK   (0x1 << 22)
#define IEC_REG_LOCK_EN_OFF   (0x0 << 22)
#define IEC_SW_RST_MASK        (0x1 << 23)
#define IEC_SW_NORST           (0x1 << 23)
#define IEC_SW_RST             (0x0 << 23)
#define IEC_FORCE_UPDATE_SIZE_MASK (0xFF << 24)


#define AFE_IRQ_MCU_CON		0x03a0
#define AFE_IRQ_STATUS		0x03a4
#define AFE_IRQ_CLR		0x03a8
#define AFE_IRQ_CNT1		0x03ac
#define AFE_IRQ_CNT2		0x03b0
#define AFE_IRQ_MCU_EN		0x03b4
#define AFE_IRQ_CNT5		0x03bc
#define AFE_IRQ_CNT7		0x03dc
#define AFE_IRQ_CNT13		0x0408
#define AFE_IRQ1_MCU_CNT_MON    0x03c0
#define AFE_IRQ2_MCU_CNT_MON    0x03c4
#define AFE_IRQ_MCU_CON2	0x03f8

#define AFE_MEMIF_PBUF_SIZE	0x03d8
#define AFE_MEMIF_PBUF2_SIZE	0x03ec

#define AFE_APLL1_TUNER_CFG	0x03f0
#define AFE_APLL2_TUNER_CFG	0x03f4

#define AFE_ASRC_CON0		0x0500

#define AFE_ASRC_CON13		0x0550
#define AFE_ASRC_CON14		0x0554
#define AFE_ASRC_CON15		0x0558
#define AFE_ASRC_CON16		0x055c
#define AFE_ASRC_CON17		0x0560
#define AFE_ASRC_CON18		0x0564
#define AFE_ASRC_CON19		0x0568
#define AFE_ASRC_CON20		0x056c
#define AFE_ASRC_CON21		0x0570

#define AFE_TDM_CON1		0x0548
#define AFE_TDM_CON2		0x054c

#define AFE_TDM_IN_CON1		0x0588
#define AFE_TDM_IN_MON2		0x0594
#define AFE_IRQ_CNT10		0x08dc

#define AFE_HDMI_IN_2CH_CON0	0x09c0
#define AFE_HDMI_IN_2CH_BASE	0x09c4
#define AFE_HDMI_IN_2CH_END	0x09c8
#define AFE_HDMI_IN_2CH_CUR	0x09cc

#define AFE_MEMIF_MON15		0x0d7c
#define ABB_AFE_SDM_TEST	0x0f4c

#define AFE_IRQ_STATUS_BITS	0x13ff

/* AUDIO_TOP_CON0 (0x0000) */
#define AUD_TCON0_PDN_DAC_PREDIS	BIT(26)
#define AUD_TCON0_PDN_DAC		BIT(25)
#define AUD_TCON0_PDN_ADC		BIT(24)
#define AUD_TCON0_PDN_SPDF		BIT(21)
#define AUD_TCON0_PDN_HDMI		BIT(20)
#define AUD_TCON0_PDN_APLL_TUNER	BIT(19)
#define AUD_TCON0_PDN_APLL2_TUNER	BIT(18)
#define AUD_TCON0_PDN_INTDIR_CK		BIT(15)
#define AUD_TCON0_PDN_24M		BIT(9)
#define AUD_TCON0_PDN_22M		BIT(8)
#define AUD_TCON0_PDN_I2S		BIT(6)
#define AUD_TCON0_PDN_AFE		BIT(2)

/* AUDIO_TOP_CON3 (0x000C) */
#define AUD_TCON3_HDMI_BCK_INV		BIT(3)

/* AFE_I2S_CON (0x0018) */
#define AFE_I2S_CON_PHASE_SHIFT_FIX	BIT(31)
#define AFE_I2S_CON_BCK_INV			BIT(29)
#define AFE_I2S_CON_FROM_IO_MUX		BIT(28)
#define AFE_I2S_CON_LOW_JITTER_CLK	BIT(12)
#define AFE_I2S_CON_LRCK_INV		BIT(5)
#define AFE_I2S_CON_FORMAT_I2S		BIT(3)
#define AFE_I2S_CON_SRC_SLAVE		BIT(2)
#define AFE_I2S_CON_WLEN_32BIT		BIT(1)
#define AFE_I2S_CON_EN			BIT(0)

/* AFE_DAIBT_CON0 (0x001c) */
#define AFE_DAIBT_CON0_USE_MRG_INPUT    BIT(12)
#define AFE_DAIBT_CON0_DATA_DRY         BIT(3)

/* AFE_CONN1 (0x0024) */
#define AFE_CONN1_I03_O03_S		BIT(19)

/* AFE_CONN2 (0x0028) */
#define AFE_CONN2_I04_O04_S		BIT(4)
#define AFE_CONN2_I03_O04_S		BIT(3)

/* AFE_I2S_CON1 (0x0034) */
#define AFE_I2S_CON1_I2S2_TO_PAD	(1 << 18)
#define AFE_I2S_CON1_TDMOUT_TO_PAD	(0 << 18)
#define AFE_I2S_CON1_TDMOUT_MUX_MASK	GENMASK(18, 18)
#define AFE_I2S_CON1_LOW_JITTER_CLK	BIT(12)
#define AFE_I2S_CON1_RATE(x)		(((x) & 0xf) << 8)
#define AFE_I2S_CON1_FORMAT_I2S		BIT(3)
#define AFE_I2S_CON1_WLEN_32BIT		BIT(1)
#define AFE_I2S_CON1_EN			BIT(0)

/* AFE_I2S_CON2 (0x0038) */
#define AFE_I2S_CON2_LOW_JITTER_CLK	BIT(12)
#define AFE_I2S_CON2_RATE(x)		(((x) & 0xf) << 8)
#define AFE_I2S_CON2_FORMAT_I2S		BIT(3)
#define AFE_I2S_CON2_WLEN_32BIT		BIT(1)
#define AFE_I2S_CON2_EN			BIT(0)

/* AFE_I2S_CON3 (0x004C) */
#define AFE_I2S_CON3_LOW_JITTER_CLK	BIT(12)
#define AFE_I2S_CON3_RATE(x)		(((x) & 0xf) << 8)
#define AFE_I2S_CON3_FORMAT_I2S		BIT(3)
#define AFE_I2S_CON3_WLEN_32BIT		BIT(1)
#define AFE_I2S_CON3_EN			BIT(0)

/* AFE_CONN_24BIT (0x006c) */
#define AFE_CONN_24BIT_O10		BIT(10)
#define AFE_CONN_24BIT_O09		BIT(9)
#define AFE_CONN_24BIT_O06		BIT(6)
#define AFE_CONN_24BIT_O05		BIT(5)
#define AFE_CONN_24BIT_O04		BIT(4)
#define AFE_CONN_24BIT_O03		BIT(3)
#define AFE_CONN_24BIT_O02		BIT(2)
#define AFE_CONN_24BIT_O01		BIT(1)
#define AFE_CONN_24BIT_O00		BIT(0)

/* AFE_ADDA_DL_SRC2_CON0 (0x0108) */
#define AFE_ADDA_DL_8X_UPSAMPLE		(BIT(25) | BIT(24))
#define AFE_ADDA_DL_MUTE_OFF		(BIT(12) | BIT(11))
#define AFE_ADDA_DL_VOICE_DATA		BIT(5)
#define AFE_ADDA_DL_DEGRADE_GAIN	BIT(1)

/* AFE_SINEGEN_CON_TDM (0x01f8) */
#define AFE_SINEGEN_CON_TDM_OUT_EN	BIT(28)

/* AFE_SINEGEN_CON_TDM_IN (0x01fc) */
#define AFE_SINEGEN_CON_TDM_IN_EN	BIT(28)

/* AFE_HDMI_OUT_CON0 (0x0370) */
#define AFE_HDMI_OUT_CON0_CH_MASK	GENMASK(7, 4)

/* AFE_HDMI_CONN0 (0x0390) */
#define AFE_HDMI_CONN0_O35_I35		(0x7 << 21)
#define AFE_HDMI_CONN0_O34_I34		(0x6 << 18)
#define AFE_HDMI_CONN0_O33_I33		(0x5 << 15)
#define AFE_HDMI_CONN0_O32_I32		(0x4 << 12)
#define AFE_HDMI_CONN0_O31_I30		(0x2 << 9)
#define AFE_HDMI_CONN0_O31_I31		(0x3 << 9)
#define AFE_HDMI_CONN0_O30_I31		(0x3 << 6)
#define AFE_HDMI_CONN0_O30_I30		(0x2 << 6)
#define AFE_HDMI_CONN0_O29_I29		(0x1 << 3)
#define AFE_HDMI_CONN0_O28_I28		(0x0 << 0)

/* AFE_CONN_TDMIN_CON (0x039c) */
#define AFE_CONN_TDMIN_O41_I41		(0x1 << 3)
#define AFE_CONN_TDMIN_O41_I40		(0x0 << 3)
#define AFE_CONN_TDMIN_O40_I41		(0x1 << 0)
#define AFE_CONN_TDMIN_O40_I40		(0x0 << 0)
#define AFE_CONN_TDMIN_CON0_MASK	GENMASK(5, 0)

/* AFE_APLL1_TUNER_CFG (0x03ec) */
#define AFE_APLL1_TUNER_CFG_MASK	GENMASK(15, 1)
#define AFE_APLL1_TUNER_CFG_EN_MASK	GENMASK(0, 0)

/* AFE_APLL2_TUNER_CFG (0x03f0) */
#define AFE_APLL2_TUNER_CFG_MASK	GENMASK(15, 1)
#define AFE_APLL2_TUNER_CFG_EN_MASK	GENMASK(0, 0)

/* AFE_GAIN1_CON0 (0x0410) */
#define AFE_GAIN1_CON0_EN_MASK		GENMASK(0, 0)
#define AFE_GAIN1_CON0_MODE_MASK	GENMASK(7, 4)
#define AFE_GAIN1_CON0_SAMPLE_PER_STEP_MASK		GENMASK(15, 8)

/* AFE_GAIN1_CON1 (0x0414) */
#define AFE_GAIN1_CON1_MASK		GENMASK(19, 0)

/* AFE_GAIN1_CUR (0x0424) */
#define AFE_GAIN1_CUR_MASK		GENMASK(19, 0)

/* AFE_ASRC_CON0 (0x0500) */
#define AFE_ASRC_CON0_ASM_ON		BIT(0)
#define AFE_ASRC_CON0_STR_CLR_MASK		GENMASK(6, 4)
#define AFE_ASRC_CON0_CLR_TX		(0x1 << 4)
#define AFE_ASRC_CON0_CLR_RX		(0x2 << 4)
#define AFE_ASRC_CON0_CLR_I2S		(0x4 << 4)

/* AFE_ASRC_CON13 (0x0550) */
#define AFE_ASRC_CON13_16BIT		BIT(19)
#define AFE_ASRC_CON13_MONO		BIT(16)

/* AFE_ASRC_CON16 (0x055c) */
#define AFE_ASRC_CON16_FC2_CYCLE_MASK		GENMASK(31, 16)
#define AFE_ASRC_CON16_FC2_CYCLE(x)		(((x) - 1) << 16)
#define AFE_ASRC_CON16_FC2_AUTO_RST		BIT(14)
#define AFE_ASRC_CON16_TUNE_FREQ5		BIT(12)
#define AFE_ASRC_CON16_COMP_FREQ_EN		BIT(11)
#define AFE_ASRC_CON16_FC2_SEL		GENMASK(9, 8)
#define AFE_ASRC_CON16_FC2_I2S_IN		(0x1 << 8)
#define AFE_ASRC_CON16_FC2_DGL_BYPASS		BIT(7)
#define AFE_ASRC_CON16_FC2_AUTO_RESTART		BIT(2)
#define AFE_ASRC_CON16_FC2_FREQ		BIT(1)
#define AFE_ASRC_CON16_FC2_EN		BIT(0)

/* AFE_TDM_CON1 (0x0548) */
#define AFE_TDM_CON1_LRCK_WIDTH(x)	(((x) - 1) << 24)
#define AFE_TDM_CON1_32_BCK_CYCLES	(0x2 << 12)
#define AFE_TDM_CON1_16_BCK_CYCLES	(0x0 << 12)
#define AFE_TDM_CON1_8CH_PER_SDATA	(0x2 << 10)
#define AFE_TDM_CON1_4CH_PER_SDATA	(0x1 << 10)
#define AFE_TDM_CON1_2CH_PER_SDATA	(0x0 << 10)
#define AFE_TDM_CON1_WLEN_32BIT		BIT(9)
#define AFE_TDM_CON1_WLEN_16BIT		BIT(8)
#define AFE_TDM_CON1_MSB_ALIGNED	BIT(4)
#define AFE_TDM_CON1_1_BCK_DELAY	BIT(3)
#define AFE_TDM_CON1_LRCK_INV		BIT(2)
#define AFE_TDM_CON1_EN			BIT(0)

/* AFE_TDM_CON2 (0x054c) */
#define AFE_TDM_CON2_SOUT_MASK		GENMASK(14, 0)

/* AFE_TDM_IN_CON1 (0x0588) */
#define AFE_TDM_IN_CON1_LRCK_WIDTH(x)		(((x) - 1) << 24)
#define AFE_TDM_IN_CON1_DISABLE_CH67		BIT(19)
#define AFE_TDM_IN_CON1_DISABLE_CH01		BIT(18)
#define AFE_TDM_IN_CON1_DISABLE_CH23		BIT(17)
#define AFE_TDM_IN_CON1_DISABLE_CH45		BIT(16)
#define AFE_TDM_IN_CON1_FAST_LRCK_CYCLE_32BCK	(0x2 << 12)
#define AFE_TDM_IN_CON1_FAST_LRCK_CYCLE_24BCK	(0x1 << 12)
#define AFE_TDM_IN_CON1_FAST_LRCK_CYCLE_16BCK	(0x0 << 12)
#define AFE_TDM_IN_CON1_8CH_PER_SDATA		(0x2 << 10)
#define AFE_TDM_IN_CON1_4CH_PER_SDATA		(0x1 << 10)
#define AFE_TDM_IN_CON1_2CH_PER_SDATA		(0x0 << 10)
#define AFE_TDM_IN_CON1_WLEN_32BIT		(0x3 << 8)
#define AFE_TDM_IN_CON1_WLEN_24BIT		(0x2 << 8)
#define AFE_TDM_IN_CON1_WLEN_16BIT		(0x1 << 8)
#define AFE_TDM_IN_CON1_I2S			BIT(3)
#define AFE_TDM_IN_CON1_LRCK_INV		BIT(2)
#define AFE_TDM_IN_CON1_BCK_INV			BIT(1)
#define AFE_TDM_IN_CON1_EN			BIT(0)

/* AFE_TDM_IN_MON2 (0x0594) */
#define AFE_TDM_IN_MON2_FAST_LRCK		BIT(28)

/*AFE_SPDIF_IN*/
#define AUDIO_BASE  0

#define AUDIO_HW_PHYSICAL_BASE  AUDIO_BASE
#define AFE_BASE                (AUDIO_HW_PHYSICAL_BASE)

#define AFE_SPDIFIN_CFG0         (0 + 0x0900)
    #define SPDIFIN_EN_MASK          0x1 << 0
    #define SPDIFIN_EN               0x1 << 0
    #define SPDIFIN_DIS              0x0 << 0
    #define SPDIFIN_FLIP_EN_MASK     0x1 << 1
    #define SPDIFIN_FLIP_EN          0x1 << 1
    #define SPDIFIN_FLIP_DIS         0x0 << 1
    #define SPDIFIN_INT_EN_MASK      0x1 << 6
    #define SPDIFIN_INT_EN           0x1 << 6
    #define SPDIFIN_INT_DIS          0x0 << 6
    #define SPDIFIN_DE_CNT_MASK      0x1F << 8
    #define SPDIFIN_DE_SEL_MASK      0x3 << 13
    #define SPDIFIN_DE_SEL_3SAMPLES  0x0
    #define SPDIFIN_DE_SEL_14SAMPLES 0x1 << 13
    #define SPDIFIN_DE_SEL_30SAMPLES 0x2 << 13
    #define SPDIFIN_DE_SEL_DECNT     0x3 << 13
    #define MAX_LEN_NUM_MASK         0xFF << 16
    #define SPDIFIN_INT_CHSTLR_MASK  0x1 << 28
    #define SPDIFIN_INT_CHSTLR_EN    0x1 << 28
    #define SPDIFIN_INT_CHSTLR_DIS   0x0 << 28

#define AFE_SPDIFIN_CFG1         (0 + 0x0904)
    #define SPDIFIN_DATA_FROM_LOOPBACK_EN_MASK  0x1 << 14
    #define SPDIFIN_DATA_FROM_LOOPBACK_EN       0x1 << 14
    #define SPDIFIN_DATA_FROM_LOOPBACK_DIS      0x0 << 14
    #define SPDIFIN_INT_ERR_EN_MASK             0xFFF << 20

    #define AFE_SPDIFIN_REAL_OPTICAL            (0x0 << 14)
    #define AFE_SPDIFIN_SWITCH_REAL_OPTICAL     (0x0 << 15)
    #define SEL_BCK_SPDIFIN                     (0x1 << 16)
    #define AFE_SPDIFIN_BYPASS_SPDIFOUT_MASK    (0x1 << 10)
    #define AFE_SPDIFIN_BYPASS_SPDIFOUT_EN      (0x1 << 10)
    #define AFE_SPDIFIN_BYPASS_SPDIFOUT_DIS     (0x0 << 10)
    #define AFE_SPDIFIN_SEL_SPDIFIN_EN          (0x1 << 0)
    #define AFE_SPDIFIN_SEL_SPDIFIN_DIS         (0x0 << 0)

    #define AFE_SPDIFIN_SEL_SPDIFIN_CLK_DIS     (0x0 << 1)
    #define AFE_SPDIFIN_FIFOSTARTPOINT_5        (0x1 << 4)

    #define SPDIFIN_PRE_ERR_NON_EN              (0x1 << 20)
    #define SPDIFIN_PRE_ERR_NON_DIS             (0x0 << 20)
    #define SPDIFIN_PRE_ERR_B_EN                (0x1 << 21)
    #define SPDIFIN_PRE_ERR_B_DIS               (0x0 << 21)
    #define SPDIFIN_PRE_ERR_M_EN                (0x1 << 22)
    #define SPDIFIN_PRE_ERR_M_DIS               (0x0 << 22)
    #define SPDIFIN_PRE_ERR_W_EN                (0x1 << 23)
    #define SPDIFIN_PRE_ERR_W_DIS               (0x0 << 23)
    #define SPDIFIN_PRE_ERR_BITCNT_EN           (0x1 << 24)
    #define SPDIFIN_PRE_ERR_BITCNT_DIS          (0x0 << 24)
    #define SPDIFIN_PRE_ERR_PARITY_EN           (0x1 << 25)
    #define SPDIFIN_PRE_ERR_PARITY_DIS          (0x0 << 25)

    #define SPDIFIN_FIFO_ERR_EN                 (0x3 << 26)
    #define SPDIFIN_FIFO_ERR_DIS                (0x0 << 26)

    #define SPDIFIN_TIMEOUT_INT_EN              (0x1 << 28)
    #define SPDIFIN_TIMEOUT_INT_DIS             (0x0 << 28)
    #define SPDIFIN_CHSTS_PREAMPHASIS_EN        (0x1 << 29) // channel status and emphasis
    #define SPDIFIN_CHSTS_PREAMPHASIS_DIS       (0x0 << 29)
    #define SPDIFIN_CHSTS_COLLECTION_MASK       (0x1 << 31) // channel status and emphasis
    #define SPDIFIN_CHSTS_COLLECTION_EN         (0x1 << 31) // channel status and emphasis
    #define SPDIFIN_CHSTS_COLLECTION_DIS        (0x0 << 31)
    #define SPDIFIN_ALL_ERR_INT_EN              (SPDIFIN_PRE_ERR_NON_EN|SPDIFIN_PRE_ERR_B_EN|SPDIFIN_PRE_ERR_M_EN| \
                                                 SPDIFIN_PRE_ERR_W_EN|SPDIFIN_PRE_ERR_BITCNT_EN|SPDIFIN_PRE_ERR_PARITY_EN|\
                                                 SPDIFIN_TIMEOUT_INT_EN|SPDIFIN_CHSTS_PREAMPHASIS_EN | SPDIFIN_CHSTS_COLLECTION_EN)

    #define SPDIFIN_ALL_ERR_INT_DIS             (SPDIFIN_PRE_ERR_NON_DIS|SPDIFIN_PRE_ERR_B_DIS|SPDIFIN_PRE_ERR_M_DIS|\
                                                 SPDIFIN_PRE_ERR_W_DIS|SPDIFIN_PRE_ERR_BITCNT_DIS|SPDIFIN_PRE_ERR_PARITY_DIS|\
                                                 SPDIFIN_TIMEOUT_INT_DIS|SPDIFIN_CHSTS_PREAMPHASIS_DIS | SPDIFIN_CHSTS_COLLECTION_DIS)

#define AFE_SPDIFIN_CHSTS1       (AFE_BASE + 0x0908)
#define AFE_SPDIFIN_CHSTS2       (AFE_BASE + 0x090C)
#define AFE_SPDIFIN_CHSTS3       (AFE_BASE + 0x0910)
#define AFE_SPDIFIN_CHSTS4       (AFE_BASE + 0x0914)
#define AFE_SPDIFIN_CHSTS5       (AFE_BASE + 0x0918)
#define AFE_SPDIFIN_CHSTS6       (AFE_BASE + 0x091C)
#define AFE_SPDIFIN_DEBUG1       (0 + 0x0920)
    #define SPDIFIN_DATA_LATCH_ERR    1 << 10

#define AFE_SPDIFIN_DEBUG2       (0 + 0x0924)
    #define SPDIFIN_CHSTS_INT_MASK     (0x1 << 26)
    #define SPDIFIN_CHSTS_INT_EN       (0x1 << 26)
    #define SPDIFIN_CHSTS_INT_FLAG     (0x1 << 26)
    #define SPDIFIN_CHSTS_INT_DIS      (0x0 << 26)
    #define SPDIFIN_FIFO_ERR_STS       (0x3 << 30)

#define AFE_SPDIFIN_DEBUG3       (0 + 0x0928)
    #define SPDIFIN_PRE_ERR_NON_STS        (0x1 << 0)
    #define SPDIFIN_PRE_ERR_B_STS          (0x1 << 1)
    #define SPDIFIN_PRE_ERR_M_STS          (0x1 << 2)
    #define SPDIFIN_PRE_ERR_W_STS          (0x1 << 3)
    #define SPDIFIN_PRE_ERR_BITCNT_STS     (0x1 << 4)
    #define SPDIFIN_PRE_ERR_PARITY_STS     (0x1 << 5)
    #define SPDIFIN_TIMEOUT_ERR_STS        (0x1 << 6)
    #define SPDIFIN_CHSTS_PREAMPHASIS_STS  (0x1 << 7)
    #define SPDIFIN_ALL_ERR_ERR_STS        (SPDIFIN_PRE_ERR_NON_STS|SPDIFIN_PRE_ERR_B_STS|SPDIFIN_PRE_ERR_M_STS|\
                                            SPDIFIN_PRE_ERR_W_STS|SPDIFIN_PRE_ERR_BITCNT_STS|\
                                            SPDIFIN_PRE_ERR_PARITY_STS|SPDIFIN_TIMEOUT_ERR_STS)

#define AFE_SPDIFIN_DEBUG4       (AFE_BASE + 0x092C)
#define AFE_SPDIFIN_EC           (0 + 0x0930)
#if 0
    #define SPDIFIN_INT_ERR_CLEAR_MASK 0x7FFFF
#else
    #define SPDIFIN_INT_ERR_CLEAR_MASK 0xFFF
#endif
    #define SPDIFIN_PRE_ERR_CLEAR               0x1 << 0
    #define SPDIFIN_PRE_ERR_B_CLEAR             0x1 << 1
    #define SPDIFIN_PRE_ERR_M_CLEAR             0x1 << 2
    #define SPDIFIN_PRE_ERR_W_CLEAR             0x1 << 3
    #define SPDIFIN_PRE_ERR_BITCNT_CLEAR        0x1 << 4
    #define SPDIFIN_PRE_ERR_PARITY_CLEAR        0x1 << 5
    #define SPDIFIN_FIFO_ERR_CLEAR			    0x3 << 6
    #define SPDIFIN_TIMEOUT_INT_CLEAR           0x1 << 8
    #define SPDIFIN_CHSTS_PREAMPHASIS_CLEAR     0x1 << 9  // channel status and emphasis
    #define SPDIFIN_CHSTS_INT_CLR_MASK          (0x1 << 11) // channel status int clear
    #define SPDIFIN_CHSTS_COLLECTION_CLEAR         (0x1 << 11)
    #define SPDIFIN_CHSTS_INT_CLR_EN            (0x1 << 11) // channel status
    #define SPDIFIN_DATA_LRCK_CHANGE_CLEAR      0x1 << 16
    #define SPDIFIN_DATA_LATCH_CLEAR            0x1 << 17
#if 0
    #define SPDIFIN_INT_CLEAR_ALL   SPDIFIN_INT_ERR_CLEAR_MASK
#else
    #define SPDIFIN_INT_CLEAR_ALL               (SPDIFIN_PRE_ERR_CLEAR|SPDIFIN_PRE_ERR_B_CLEAR|SPDIFIN_PRE_ERR_M_CLEAR|\
                                                 SPDIFIN_PRE_ERR_W_CLEAR|SPDIFIN_FIFO_ERR_CLEAR|SPDIFIN_PRE_ERR_BITCNT_CLEAR|\
                                                 SPDIFIN_PRE_ERR_PARITY_CLEAR|SPDIFIN_TIMEOUT_INT_CLEAR|SPDIFIN_CHSTS_PREAMPHASIS_CLEAR|\
                                                 SPDIFIN_DATA_LATCH_CLEAR|SPDIFIN_CHSTS_COLLECTION_CLEAR)
#endif


#define AFE_SPDIFIN_CLK_CFG          (0 + 0x0934)
#define  SPDIFIN_AUTO_LOCK_MASK               (0x1 << 23)
#define  SPDIFIN_AUTO_LOCK               (0x1 << 23)
#define  SPDIFIN_AUTO_LOCK_DIS               (0x0 << 23)

#define  SPDIFIN_AUTOLOCK_EN_MASK             (0x1<<0)
#define  SPDIFIN_AUTOLOCK_EN             (0x1<<0)
#define  SPDIFIN_AUTOLOCK_DIS            (0x0<<0)
#define  SPDIFIN_IND_ACC_EXPONENT           (0x7<<1)


#define AFE_SPDIFIN_BR           (0 + 0x093C)
    #define AFE_SPDIFIN_BRE_MASK           0x1 << 0
    #define AFE_SPDIFIN_BR_FS_MASK         0x7 << 4
    #define AFE_SPDIFIN_BR_FS_256          0x3 << 4
    #define AFE_SPDIFIN_BR_SUBFRAME_MASK   0xF << 8
    #define AFE_SPDIFIN_BR_SUBFRAME_256    0x8 << 8
    #define AFE_SPDIFIN_BR_LOWBOUND_MASK   0x1F << 12
    #define AFE_SPDIFIN_BR_TUNE_MODE_MASK  0x3 << 17
    #define AFE_SPDIFIN_BR_TUNE_MODE0      0x0 << 17
    #define AFE_SPDIFIN_BR_TUNE_MODE1      0x1 << 17
    #define AFE_SPDIFIN_BR_TUNE_MODE2      0x2 << 17
    #define AFE_SPDIFIN_BR_TUNE_MODE1_D    0x3 << 17

#define AFE_SPDIFIN_BR_DBG1      (AFE_BASE + 0x0940)

#define AFE_SPDIFIN_INT_EXT      (0 + 0x0948)
    #define MULTI_INPUT_DETECT_SEL_MASK    0xF << 8
    #define MULTI_INPUT_DETECT_SEL_OPT     0x1 << 8
    #define MULTI_INPUT_DETECT_SEL_COA     0x2 << 8
    #define MULTI_INPUT_DETECT_SEL_ARC     0x4 << 8
    #define MULTI_INPUT_SEL_MASK           0x3 << 14
    #define MULTI_INPUT_SEL_OPT            0x0 << 14
    #define MULTI_INPUT_SEL_COA            0x1 << 14
    #define MULTI_INPUT_SEL_ARC            0x2 << 14
    #define MULTI_INPUT_SEL_LOW            0x3 << 14
    #define SPDIFIN_DATALATCH_ERR_EN_MASK  0x1 << 17
    #define SPDIFIN_DATALATCH_ERR_EN       0x1 << 17
    #define SPDIFIN_DATALATCH_ERR_DIS      0x0 << 17
    #define MULTI_INPUT_STATUS_MASK        0xF << 28
    #define MULTI_INPUT_STATUS_OPT         0x1 << 28
    #define MULTI_INPUT_STATUS_COA         0x2 << 28
    #define MULTI_INPUT_STATUS_ARC         0x4 << 28

#define AFE_SPDIFIN_INT_EXT2     (0 + 0x094C)
    #define SPDIFIN_LRC_MASK             0x7FFF
    #define SPDIFIN_LRC_COMPARE_594M     0x173
    #define SPDIFIN_LRCK_CHG_INT_MASK    0x1 << 15
    #define SPDIFIN_LRCK_CHG_INT_EN      0x1 << 15
    #define SPDIFIN_LRCK_CHG_INT_DIS     0x0 << 15
    #define SPDIFIN_432MODE_MASK         0x1 << 16
    #define SPDIFIN_432MODE_EN           0x1 << 16
    #define SPDIFIN_432MODE_DIS          0x0 << 16

    #define SPDIFIN_MODE_CLK_MASK        0x3 << 16

    #define SPDIFIN_594MODE_MASK         0x1 << 17
    #define SPDIFIN_594MODE_EN           0x1 << 17
    #define SPDIFIN_594MODE_DIS          0x0 << 17
    #define SPDIFIN_LRCK_CHG_INT_STS     0x1 << 27
    #define SPDIFIN_ROUGH_FS_MASK        0xF << 28
    #define SPDIFIN_ROUGH_FS_32K         0x1 << 28
    #define SPDIFIN_ROUGH_FS_44K         0x2 << 28
    #define SPDIFIN_ROUGH_FS_48K         0x3 << 28
    #define SPDIFIN_ROUGH_FS_64K         0x4 << 28
    #define SPDIFIN_ROUGH_FS_88K         0x5 << 28
    #define SPDIFIN_ROUGH_FS_96K         0x6 << 28
    #define SPDIFIN_ROUGH_FS_128K        0x7 << 28
    #define SPDIFIN_ROUGH_FS_144K        0x8 << 28
    #define SPDIFIN_ROUGH_FS_176K        0x9 << 28
    #define SPDIFIN_ROUGH_FS_192K        0xA << 28
    #define SPDIFIN_ROUGH_FS_216K        0xB << 28

#define SPDIFIN_FREQ_INFO        (0 + 0x0950)
#define SPDIFIN_FREQ_INFO_2      (0 + 0x0954)
#define SPDIFIN_FREQ_INFO_3      (0 + 0x0958)
#define SPDIFIN_FREQ_STATUS      (AFE_BASE + 0x095C)
#define SPDIFIN_FREQ_USERCODE1   (AFE_BASE + 0x0960)
#define SPDIFIN_FREQ_USERCODE2   (AFE_BASE + 0x0964)
#define SPDIFIN_FREQ_USERCODE3   (AFE_BASE + 0x0968)
#define SPDIFIN_FREQ_USERCODE4   (AFE_BASE + 0x096C)
#define SPDIFIN_FREQ_USERCODE5   (AFE_BASE + 0x0970)
#define SPDIFIN_FREQ_USERCODE6   (AFE_BASE + 0x0974)
#define SPDIFIN_FREQ_USERCODE7   (AFE_BASE + 0x0978)
#define SPDIFIN_FREQ_USERCODE8   (AFE_BASE + 0x097C)
#define SPDIFIN_FREQ_USERCODE9   (AFE_BASE + 0x0980)
#define SPDIFIN_FREQ_USERCODE10  (AFE_BASE + 0x0984)
#define SPDIFIN_FREQ_USERCODE11  (AFE_BASE + 0x0988)
#define SPDIFIN_FREQ_USERCODE12  (AFE_BASE + 0x098C)
#define SPDIFIN_MEMIF_CON0       (0x0990)
    #define SPDIFIN_IN_MEMIF_EN_MASK    0x1 << 0
    #define SPDIFIN_IN_MEMIF_EN         0x1 << 0
    #define SPDIFIN_IN_MEMIF_DIS        0x0 << 0

#define SPDIFIN_BASE_ADR         (0x0994)
#define SPDIFIN_END_ADR          (0x0998)
#define SPDIFIN_APLL_TUNER_CFG   (AFE_BASE + 0x09A0)
#define SPDIFIN_APLL_TUNER_CFG1  (AFE_BASE + 0x09A4)
#define SPDIFIN_APLL2_TUNER_CFG  (AFE_BASE + 0x09A8)
#define SPDIFIN_APLL2_TUNER_CFG1 (AFE_BASE + 0x09AC)
#define SPDIFIN_TYPE_DET         (AFE_BASE + 0x09B0)
#define SPDIFIN_STREAM_TYPE_MASK       (0x3 << 16)

#define MPHONE_MULTI_CON0        (0x09B4)
    #define MULTI_HW_EN_MASK         0x1
    #define MULTI_HW_EN              0x1
    #define MULTI_HW_DIS             0x0
    #define MULTI_STORE_TYPE_MASK    0x1 << 1
    #define MULTI_STORE_24BIT        0x1 << 1
    #define MULTI_STORE_16BIT        0x0
    #define MULTI_INT_PERIOD_MASK    0x3 << 4
    #define MULTI_INT_PERIOD_64      0x1 << 4
    #define MULTI_INT_PERIOD_128     0x2 << 4
    #define MULTI_INT_PERIOD_256     0x3 << 4

#define SPDIFIN_CUR_ADR          (0x09B8)
#define AFE_SINEGEN_CON_SPDIFIN  (AFE_BASE + 0x09BC)

#endif
