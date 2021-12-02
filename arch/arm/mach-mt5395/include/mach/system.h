/*
 *  linux/include/asm-arm/arch-mt5391/system.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <mach/mt5395.h>
#include <asm/io.h>

static inline void arch_idle(void)
{
	/*
	 * switch off the system and wait for interrupt
	 */
	cpu_do_idle();
}

static inline void arch_reset(char mode, const char *cmd)
{
	/*
	 * use powerdown watch dog to reset system
	 * __raw_writel( 0xff000000, BIM_BASE + WATCHDOG_TIMER_COUNT_REG );
	 * __raw_writel( 1, BIM_BASE + WATCHDOG_TIMER_CTRL_REG);
	 */
	 __raw_writel( 0xff000000, PDWNC_VIRT + REG_RW_WATCHDOG_TMR );
	 __raw_writel( 1, PDWNC_VIRT + REG_RW_WATCHDOG_EN);
}

#endif
