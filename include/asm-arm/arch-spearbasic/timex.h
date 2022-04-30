/*
 * linux/include/asm/arch/timex.h
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */


#ifndef __ASM_ARCH_SPEARBASIC_TIMEX_H
#define __ASM_ARCH_SPEARBASIC_TIMEX_H

#ifdef PLL_SPEARHEAD 
#define CLOCK_TICK_RATE		(get_PCLK())

#else
//#define CLOCK_TICK_RATE			83250000 
#define CLOCK_TICK_RATE			48000000 //shiraz: check
#endif

extern unsigned long spearbasic_timer_read (int nr);

/*extern unsigned int get_PCLK ();*/

static inline cycles_t

get_cycles (void)
{
       return ~spearbasic_timer_read (1);	/* calling timer 0 */
}

#endif 

/* End of file - timex.h */
