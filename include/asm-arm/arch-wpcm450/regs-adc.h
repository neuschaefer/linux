/* linux/include/asm/arch-wpcm450/regs-adc.h
 *
 * Copyright (c) 2004 Shannon Holland <holland@loser.net>
 *
 * This program is free software; yosu can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 ADC registers
 *
 *  Changelog:
 *    27-09-2004     SAH     Created file
*/

#ifndef __ASM_ARCH_REGS_ADC_H
#define __ASM_ARCH_REGS_ADC_H "regs-adc.h"

#define WPCM450_ADCREG(x) (x)

#define WPCM450_ADCCON	   WPCM450_ADCREG(0x00)
#define WPCM450_ADCTSC	   WPCM450_ADCREG(0x04)
#define WPCM450_ADCDLY	   WPCM450_ADCREG(0x08)
#define WPCM450_ADCDAT0	   WPCM450_ADCREG(0x0C)
#define WPCM450_ADCDAT1	   WPCM450_ADCREG(0x10)


/* ADCCON Register Bits */
#define WPCM450_ADCCON_ECFLG		(1<<15)
#define WPCM450_ADCCON_PRSCEN		(1<<14)
#define WPCM450_ADCCON_PRSCVL(x)	(((x)&0xFF)<<6)
#define WPCM450_ADCCON_PRSCVLMASK	(0xFF<<6)
#define WPCM450_ADCCON_SELMUX(x)	(((x)&0x7)<<3)
#define WPCM450_ADCCON_MUXMASK		(0x7<<3)
#define WPCM450_ADCCON_STDBM		(1<<2)
#define WPCM450_ADCCON_READ_START	(1<<1)
#define WPCM450_ADCCON_ENABLE_START	(1<<0)
#define WPCM450_ADCCON_STARTMASK	(0x3<<0)


/* ADCTSC Register Bits */
#define WPCM450_ADCTSC_YM_SEN		(1<<7)
#define WPCM450_ADCTSC_YP_SEN		(1<<6)
#define WPCM450_ADCTSC_XM_SEN		(1<<5)
#define WPCM450_ADCTSC_XP_SEN		(1<<4)
#define WPCM450_ADCTSC_PULL_UP_DISABLE	(1<<3)
#define WPCM450_ADCTSC_AUTO_PST		(1<<2)
#define WPCM450_ADCTSC_XY_PST		(0x3<<0)

/* ADCDAT0 Bits */
#define WPCM450_ADCDAT0_UPDOWN		(1<<15)
#define WPCM450_ADCDAT0_AUTO_PST	(1<<14)
#define WPCM450_ADCDAT0_XY_PST		(0x3<<12)
#define WPCM450_ADCDAT0_XPDATA_MASK	(0x03FF)

/* ADCDAT1 Bits */
#define WPCM450_ADCDAT1_UPDOWN		(1<<15)
#define WPCM450_ADCDAT1_AUTO_PST	(1<<14)
#define WPCM450_ADCDAT1_XY_PST		(0x3<<12)
#define WPCM450_ADCDAT1_YPDATA_MASK	(0x03FF)

#endif /* __ASM_ARCH_REGS_ADC_H */


