/*
 * asm-arm/arch-mt5391/preempt.h
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

#ifndef _ASM_ARCH_PREEMT_H
#define _ASM_ARCH_PREEMT_H

static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
	return (stop - start);
}

extern unsigned long mt5391_timer_read(int nr);

/*
 * timer 1 runs @ 100Mhz, 100 tick = 1 microsecond
 * and is configured as a count down timer.
 */

#define TICKS_PER_USEC                  100
#define ARCH_PREDEFINES_TICKS_PER_USEC
#define readclock()                     (~mt5391_timer_read(1))
#define clock_to_usecs(x)               ((x) / TICKS_PER_USEC)
#define INTERRUPTS_ENABLED(x)           (!(x & PSR_I_BIT))

#endif

