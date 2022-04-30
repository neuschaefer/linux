/*
 * linux/include/asm/arch/preempt.h
 *
 *  Copyright (C) ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :   <xxx.yyy@st.com> */

#ifndef _ASM_ARCH_PREEMT_H
#define _ASM_ARCH_PREEMT_H

static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
	return (stop - start);
}

extern unsigned long versatile_timer_read(int nr);

/*
 * timer 1 runs @ 1Mhz, 1 tick = 1 microsecond
 * and is configured as a count down timer.
 */

#define TICKS_PER_USEC		1 
#define ARCH_PREDEFINES_TICKS_PER_USEC
#define readclock()		(~versatile_timer_read(1))
#define clock_to_usecs(x)	((x) / TICKS_PER_USEC)
#define INTERRUPTS_ENABLED(x)	(!(x & PSR_I_BIT))

#endif

/* End of file - preempt.h */
