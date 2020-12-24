/* linux/include/asm-arm/arch-wpcm450/system.h
 *
 * (c) 2006 Winbond Electronics
 *  shirley yu <clyu2@winbond.com>
 *
 * WPCM450 - System function defines and includes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 */

#include <asm/hardware.h>
#include <asm/io.h>

#include <asm/arch/map.h>
#include <asm/arch/idle.h>

#include <asm/arch/regs-watchdog.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-timer.h>

/* chtsai 20071009 
 * Use software reset to reset cpu.
 */
//#define FIX_REBOOT

#ifdef FIX_REBOOT
#if 1
/* Due to the overlapped memory mapping in map.h, define the register here. */
#define WPCM450_PWCON			(WPCM450_VA_GCR+0x200+(0x14))
#else
#define WPCM450_PWCON			WPCM450_CLKPMCON
#endif

#endif

/* Software Reset Control Bit in PMCON */
#define WPCM450_PMCON_RESET		(1<<3)

void (*wpcm450_idle)(void);

void wpcm450_default_idle(void)
{
#if 0//clyu
	/* idle the system by using the idle mode which will wait for an
	 * interrupt to happen before restarting the system.
	 */

	/* Warning: going into idle state upsets jtag scanning */

	__raw_writel(__raw_readl(reg) | (1<<2), reg);

	/* the winbond port seems to do a loop and then unset idle.. */
	for (i = 0; i < 50; i++) {
		tmp += __raw_readl(reg); /* ensure loop not optimised out */
	}

	/* this bit is not cleared on re-start... */

	__raw_writel(__raw_readl(reg) & ~(1<<2), reg);
#endif
}

static void arch_idle(void)
{
	if (wpcm450_idle != NULL)
		(wpcm450_idle)();
	else
		wpcm450_default_idle();
}


static void
arch_reset(char mode)
{
	if (mode == 's') {
		cpu_reset(0);
	}
#ifdef FIX_REBOOT
	/* Software Reset */
	__raw_writel(__raw_readl(WPCM450_PWCON) | WPCM450_PMCON_RESET,
				 WPCM450_PWCON);
#endif

	printk("arch_reset: attempting watchdog reset\n");

	__raw_writel(__raw_readl(WPCM450_WTCR) & ~WTCR_WTE, WPCM450_WTCR);	  /* disable watchdog, to be safe  */

	printk(KERN_EMERG  "Trigger HW watchdog reset!\n");

	__raw_writel(WTCR_WTE | WTCR_WTIE | WTCR_WTRE | WTCR_WTR, WPCM450_WTCR);

	printk(KERN_ERR "Watchdog reset failed to assert reset\n");

	/* we'll take a jump through zero as a poor second */
	cpu_reset(0);
}
