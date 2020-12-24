/* linux/include/asm/arch-wpcm450/regs0watchdog.h
 *
 * Copyright (c) 2003 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 Watchdog timer control
 *
 *  Changelog:
 *    21-06-2003     BJD     Created file
 *    12-03-2004     BJD     Updated include protection
 *    10-03-2005     LCVR    Changed WPCM450_VA to WPCM450_VA
*/


#ifndef __ASM_ARCH_REGS_WATCHDOG_H
#define __ASM_ARCH_REGS_WATCHDOG_H "$Id$"

#define WPCM450_WDOGREG(x) ((x) + WPCM450_VA_WATCHDOG)

#define WPCM450_WTCON	   WPCM450_WDOGREG(0x00)
#define WPCM450_WTDAT	   WPCM450_WDOGREG(0x04)
#define WPCM450_WTCNT	   WPCM450_WDOGREG(0x08)

/* the watchdog can either generate a reset pulse, or an
 * interrupt.
 */

#define WPCM450_WTCON_RSTEN   (0x01)
#define WPCM450_WTCON_INTEN   (1<<2)
#define WPCM450_WTCON_ENABLE  (1<<5)

#define WPCM450_WTCON_DIV16   (0<<3)
#define WPCM450_WTCON_DIV32   (1<<3)
#define WPCM450_WTCON_DIV64   (2<<3)
#define WPCM450_WTCON_DIV128  (3<<3)

#define WPCM450_WTCON_PRESCALE(x) ((x) << 8)
#define WPCM450_WTCON_PRESCALE_MASK (0xff00)

#endif /* __ASM_ARCH_REGS_WATCHDOG_H */


