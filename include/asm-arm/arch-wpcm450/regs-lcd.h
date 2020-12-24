/* linux/include/asm/arch-wpcm450/regs-lcd.h
 *
 * Copyright (c) 2003 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 *
 *  Changelog:
 *    12-06-2003     BJD     Created file
 *    26-06-2003     BJD     Updated LCDCON register definitions
 *    12-03-2004     BJD     Updated include protection
 *    10-03-2005     LCVR    Changed WPCM450_VA to WPCM450_VA
*/


#ifndef ___ASM_ARCH_REGS_LCD_H
#define ___ASM_ARCH_REGS_LCD_H "$Id$"

#define WPCM450_LCDREG(x) ((x) + WPCM450_VA_LCD)

/* LCD control registers */
#define WPCM450_LCDCON1	    WPCM450_LCDREG(0x00)
#define WPCM450_LCDCON2	    WPCM450_LCDREG(0x04)
#define WPCM450_LCDCON3	    WPCM450_LCDREG(0x08)
#define WPCM450_LCDCON4	    WPCM450_LCDREG(0x0C)
#define WPCM450_LCDCON5	    WPCM450_LCDREG(0x10)

#define WPCM450_LCDCON1_CLKVAL(x)  ((x) << 8)
#define WPCM450_LCDCON1_MMODE	   (1<<7)
#define WPCM450_LCDCON1_DSCAN4	   (0<<5)
#define WPCM450_LCDCON1_STN4	   (1<<5)
#define WPCM450_LCDCON1_STN8	   (2<<5)
#define WPCM450_LCDCON1_TFT	   (3<<5)

#define WPCM450_LCDCON1_STN1BPP	   (0<<1)
#define WPCM450_LCDCON1_STN2GREY   (1<<1)
#define WPCM450_LCDCON1_STN4GREY   (2<<1)
#define WPCM450_LCDCON1_STN8BPP	   (3<<1)
#define WPCM450_LCDCON1_STN12BPP   (4<<1)

#define WPCM450_LCDCON1_TFT1BPP	   (8<<1)
#define WPCM450_LCDCON1_TFT2BPP	   (9<<1)
#define WPCM450_LCDCON1_TFT4BPP	   (10<<1)
#define WPCM450_LCDCON1_TFT8BPP	   (11<<1)
#define WPCM450_LCDCON1_TFT16BPP   (12<<1)
#define WPCM450_LCDCON1_TFT24BPP   (13<<1)

#define WPCM450_LCDCON1_ENVID	   (1)

#define WPCM450_LCDCON1_MODEMASK    0x1E

#define WPCM450_LCDCON2_VBPD(x)	    ((x) << 24)
#define WPCM450_LCDCON2_LINEVAL(x)  ((x) << 14)
#define WPCM450_LCDCON2_VFPD(x)	    ((x) << 6)
#define WPCM450_LCDCON2_VSPW(x)	    ((x) << 0)

#define WPCM450_LCDCON2_GET_VBPD(x) ( ((x) >> 24) & 0xFF)
#define WPCM450_LCDCON2_GET_VFPD(x) ( ((x) >>  6) & 0xFF)
#define WPCM450_LCDCON2_GET_VSPW(x) ( ((x) >>  0) & 0x3F)

#define WPCM450_LCDCON3_HBPD(x)	    ((x) << 19)
#define WPCM450_LCDCON3_WDLY(x)	    ((x) << 19)
#define WPCM450_LCDCON3_HOZVAL(x)   ((x) << 8)
#define WPCM450_LCDCON3_HFPD(x)	    ((x) << 0)
#define WPCM450_LCDCON3_LINEBLANK(x)((x) << 0)

#define WPCM450_LCDCON3_GET_HBPD(x) ( ((x) >> 19) & 0x7F)
#define WPCM450_LCDCON3_GET_HFPD(x) ( ((x) >>  0) & 0xFF)

#define WPCM450_LCDCON4_MVAL(x)	    ((x) << 8)
#define WPCM450_LCDCON4_HSPW(x)	    ((x) << 0)
#define WPCM450_LCDCON4_WLH(x)	    ((x) << 0)

#define WPCM450_LCDCON4_GET_HSPW(x) ( ((x) >>  0) & 0xFF)

#define WPCM450_LCDCON5_BPP24BL	    (1<<12)
#define WPCM450_LCDCON5_FRM565	    (1<<11)
#define WPCM450_LCDCON5_INVVCLK	    (1<<10)
#define WPCM450_LCDCON5_INVVLINE    (1<<9)
#define WPCM450_LCDCON5_INVVFRAME   (1<<8)
#define WPCM450_LCDCON5_INVVD	    (1<<7)
#define WPCM450_LCDCON5_INVVDEN	    (1<<6)
#define WPCM450_LCDCON5_INVPWREN    (1<<5)
#define WPCM450_LCDCON5_INVLEND	    (1<<4)
#define WPCM450_LCDCON5_PWREN	    (1<<3)
#define WPCM450_LCDCON5_ENLEND	    (1<<2)
#define WPCM450_LCDCON5_BSWP	    (1<<1)
#define WPCM450_LCDCON5_HWSWP	    (1<<0)

/* framebuffer start addressed */
#define WPCM450_LCDSADDR1   WPCM450_LCDREG(0x14)
#define WPCM450_LCDSADDR2   WPCM450_LCDREG(0x18)
#define WPCM450_LCDSADDR3   WPCM450_LCDREG(0x1C)

#define WPCM450_LCDBANK(x)	((x) << 21)
#define WPCM450_LCDBASEU(x)	(x)

#define WPCM450_OFFSIZE(x)	((x) << 11)
#define WPCM450_PAGEWIDTH(x)	(x)

/* colour lookup and miscellaneous controls */

#define WPCM450_REDLUT	   WPCM450_LCDREG(0x20)
#define WPCM450_GREENLUT   WPCM450_LCDREG(0x24)
#define WPCM450_BLUELUT	   WPCM450_LCDREG(0x28)

#define WPCM450_DITHMODE   WPCM450_LCDREG(0x4C)
#define WPCM450_TPAL	   WPCM450_LCDREG(0x50)

#define WPCM450_TPAL_EN		(1<<24)

/* interrupt info */
#define WPCM450_LCDINTPND  WPCM450_LCDREG(0x54)
#define WPCM450_LCDSRCPND  WPCM450_LCDREG(0x58)
#define WPCM450_LCDINTMSK  WPCM450_LCDREG(0x5C)
#define WPCM450_LCDINT_FIWSEL	(1<<2)
#define	WPCM450_LCDINT_FRSYNC	(1<<1)
#define WPCM450_LCDINT_FICNT	(1<<0)

#define WPCM450_LPCSEL	   WPCM450_LCDREG(0x60)

#define WPCM450_TFTPAL(x)  WPCM450_LCDREG((0x400 + (x)*4))

#endif /* ___ASM_ARCH_REGS_LCD_H */



