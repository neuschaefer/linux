/* linux/include/asm/arch-wpcm450/regs-rtc.h
 *
 * Copyright (c) 2003 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 Internal RTC register definition
 *
 *  Changelog:
 *    19-06-2003     BJD     Created file
 *    12-03-2004     BJD     Updated include protection
 *    15-01-2005     LCVR    Changed WPCM450_VA to WPCM450_VA (s3c2400 support)
*/

#ifndef __ASM_ARCH_REGS_RTC_H
#define __ASM_ARCH_REGS_RTC_H __FILE__

#define WPCM450_RTCREG(x) ((x) + WPCM450_VA_RTC)

#define WPCM450_RTCCON	      WPCM450_RTCREG(0x40)
#define WPCM450_RTCCON_RTCEN  (1<<0)
#define WPCM450_RTCCON_CLKSEL (1<<1)
#define WPCM450_RTCCON_CNTSEL (1<<2)
#define WPCM450_RTCCON_CLKRST (1<<3)

#define WPCM450_TICNT	      WPCM450_RTCREG(0x44)
#define WPCM450_TICNT_ENABLE  (1<<7)

#define WPCM450_RTCALM	      WPCM450_RTCREG(0x50)
#define WPCM450_RTCALM_ALMEN  (1<<6)
#define WPCM450_RTCALM_YEAREN (1<<5)
#define WPCM450_RTCALM_MONEN  (1<<4)
#define WPCM450_RTCALM_DAYEN  (1<<3)
#define WPCM450_RTCALM_HOUREN (1<<2)
#define WPCM450_RTCALM_MINEN  (1<<1)
#define WPCM450_RTCALM_SECEN  (1<<0)

#define WPCM450_RTCALM_ALL \
  WPCM450_RTCALM_ALMEN | WPCM450_RTCALM_YEAREN | WPCM450_RTCALM_MONEN |\
  WPCM450_RTCALM_DAYEN | WPCM450_RTCALM_HOUREN | WPCM450_RTCALM_MINEN |\
  WPCM450_RTCALM_SECEN


#define WPCM450_ALMSEC	      WPCM450_RTCREG(0x54)
#define WPCM450_ALMMIN	      WPCM450_RTCREG(0x58)
#define WPCM450_ALMHOUR	      WPCM450_RTCREG(0x5c)

#define WPCM450_ALMDATE	      WPCM450_RTCREG(0x60)
#define WPCM450_ALMMON	      WPCM450_RTCREG(0x64)
#define WPCM450_ALMYEAR	      WPCM450_RTCREG(0x68)

#define WPCM450_RTCRST	      WPCM450_RTCREG(0x6c)

#define WPCM450_RTCSEC	      WPCM450_RTCREG(0x70)
#define WPCM450_RTCMIN	      WPCM450_RTCREG(0x74)
#define WPCM450_RTCHOUR	      WPCM450_RTCREG(0x78)
#define WPCM450_RTCDATE	      WPCM450_RTCREG(0x7c)
#define WPCM450_RTCDAY	      WPCM450_RTCREG(0x80)
#define WPCM450_RTCMON	      WPCM450_RTCREG(0x84)
#define WPCM450_RTCYEAR	      WPCM450_RTCREG(0x88)


#endif /* __ASM_ARCH_REGS_RTC_H */
