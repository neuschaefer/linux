/*
 * xmos.h  --  XMOS Soc Audio driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _XMOS_H
#define _XMOS_H

/* XMOS register space */

#define XMOS_DSP_DAC_VOL 0x02
#define XMOS_DSP_DAC_MUTE 0x04
#define XMOS_DSP_SPDIF_VOL 0x06
#define XMOS_DSP_SPDIF_MUTE 0x08
#define XMOS_DSP_HEADP_ONOFF 0x10
#define XMOS_DSP_SPEAK_ONOFF 0x12
#define XMOS_DSP_MODE 0x14
#define XMOS_DSP_MIC_MUTE 0x16

#define XMOS_DSP_MODE_MUTE 0
#define XMOS_DSP_MODE_MUSIC 1
#define XMOS_DSP_MODE_PHONE 2
#define XMOS_DSP_MODE_TTS 3
#define XMOS_DSP_MODE_HP 4



#define XMOS_CACHEREGNUM 	56

#define XMOS_LINVOL		0x0
#define XMOS_RINVOL		0x1
#define XMOS_LOUT1		0x2
#define XMOS_ROUT1		0x3
#define XMOS_CLOCK1		0x4
#define XMOS_DACCTL1		0x5
#define XMOS_DACCTL2		0x6
#define XMOS_IFACE1		0x7
#define XMOS_CLOCK2		0x8
#define XMOS_IFACE2		0x9
#define XMOS_DAC		0xa
#define XMOS_RDAC		0xb

#define XMOS_RESET		0xf
#define XMOS_3D		0x10
#define XMOS_ALC1		0x11
#define XMOS_ALC2		0x12
#define XMOS_ALC3		0x13
#define XMOS_NOISEG		0x14
#define XMOS_LADC		0x15
#define XMOS_RADC		0x16
#define XMOS_ADDCTL1		0x17
#define XMOS_ADDCTL2		0x18
#define XMOS_POWER1		0x19
#define XMOS_POWER2		0x1a
#define XMOS_ADDCTL3		0x1b
#define XMOS_APOP1		0x1c
#define XMOS_APOP2		0x1d

#define XMOS_LINPATH		0x20
#define XMOS_RINPATH		0x21
#define XMOS_LOUTMIX		0x22

#define XMOS_ROUTMIX		0x25
#define XMOS_MONOMIX1		0x26
#define XMOS_MONOMIX2		0x27
#define XMOS_LOUT2		0x28
#define XMOS_ROUT2		0x29
#define XMOS_MONO		0x2a
#define XMOS_INBMIX1		0x2b
#define XMOS_INBMIX2		0x2c
#define XMOS_BYPASS1		0x2d
#define XMOS_BYPASS2		0x2e
#define XMOS_POWER3		0x2f
#define XMOS_ADDCTL4		0x30
#define XMOS_CLASSD1		0x31

#define XMOS_CLASSD3		0x33
#define XMOS_PLL1		0x34
#define XMOS_PLL2		0x35
#define XMOS_PLL3		0x36
#define XMOS_PLL4		0x37


/*
 * XMOS Clock dividers
 */
#define XMOS_SYSCLKDIV 		0
#define XMOS_DACDIV			1
#define XMOS_OPCLKDIV			2
#define XMOS_DCLKDIV			3
#define XMOS_TOCLKSEL			4

#define XMOS_SYSCLK_DIV_1		(0 << 1)
#define XMOS_SYSCLK_DIV_2		(2 << 1)

#define XMOS_SYSCLK_MCLK		(0 << 0)
#define XMOS_SYSCLK_PLL		(1 << 0)
#define XMOS_SYSCLK_AUTO		(2 << 0)

#define XMOS_DAC_DIV_1		(0 << 3)
#define XMOS_DAC_DIV_1_5		(1 << 3)
#define XMOS_DAC_DIV_2		(2 << 3)
#define XMOS_DAC_DIV_3		(3 << 3)
#define XMOS_DAC_DIV_4		(4 << 3)
#define XMOS_DAC_DIV_5_5		(5 << 3)
#define XMOS_DAC_DIV_6		(6 << 3)

#define XMOS_DCLK_DIV_1_5		(0 << 6)
#define XMOS_DCLK_DIV_2		(1 << 6)
#define XMOS_DCLK_DIV_3		(2 << 6)
#define XMOS_DCLK_DIV_4		(3 << 6)
#define XMOS_DCLK_DIV_6		(4 << 6)
#define XMOS_DCLK_DIV_8		(5 << 6)
#define XMOS_DCLK_DIV_12		(6 << 6)
#define XMOS_DCLK_DIV_16		(7 << 6)

#define XMOS_TOCLK_F19		(0 << 1)
#define XMOS_TOCLK_F21		(1 << 1)

#define XMOS_OPCLK_DIV_1		(0 << 0)
#define XMOS_OPCLK_DIV_2		(1 << 0)
#define XMOS_OPCLK_DIV_3		(2 << 0)
#define XMOS_OPCLK_DIV_4		(3 << 0)
#define XMOS_OPCLK_DIV_5_5		(4 << 0)
#define XMOS_OPCLK_DIV_6		(5 << 0)

#endif
