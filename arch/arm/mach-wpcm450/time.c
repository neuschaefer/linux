/* linux/arch/arm/mach-wpcm450/time.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
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
 */

#include <linux/autoconf.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/clk.h>

#include <asm/system.h>
#include <asm/leds.h>
#include <asm/mach-types.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/arch/map.h>
#include <asm/arch/regs-timer.h>
#include <asm/arch/regs-irq.h>
#include <asm/mach/time.h>

#include "clock.h"
#include "cpu.h"

//static unsigned long timer_startval;
//static unsigned long timer_usec_ticks;

//#define TIMER_USEC_SHIFT 16

/* we use the shifted arithmetic to work out the ratio of timer ticks
 * to usecs, as often the peripheral clock is not a nice even multiple
 * of 1MHz.
 *
 * shift of 14 and 15 are too low for the 12MHz, 16 seems to be ok
 * for the current HZ value of 200 without producing overflows.
 *
 * Original patch by Dimitry Andric, updated by Ben Dooks
*/


/* timer_mask_usec_ticks
 *
 * given a clock and divisor, make the value to pass into timer_ticks_to_usec
 * to scale the ticks into usecs
*/

//static inline unsigned long
//timer_mask_usec_ticks(unsigned long scaler, unsigned long pclk)
//{
//	unsigned long den = pclk / 1000;
//
//	return ((1000 << TIMER_USEC_SHIFT) * scaler + (den >> 1)) / den;
//}

/* timer_ticks_to_usec
 *
 * convert timer ticks to usec.
*/

//static inline unsigned long timer_ticks_to_usec(unsigned long ticks)
//{
//	unsigned long res;
//
//	res = ticks * 10000; /* 1 jiffies = 10ms = 10000us */
//	//res += 1 << (TIMER_USEC_SHIFT - 4);	/* round up slightly */
//
//	return res;// >> TIMER_USEC_SHIFT;
//}

/***
 * Returns microsecond  since last clock interrupt.  Note that interrupts
 * will have been disabled by do_gettimeoffset()
 * IRQs are disabled before entering here from do_gettimeofday()
 */

#define SRCPND_TIMER4 (1<<(IRQ_TIMER4 - IRQ_EINT0))

static unsigned long wpcm450_gettimeoffset (void)
{
	unsigned long tdone;
	unsigned long irqpend;
	unsigned long tval;
    
	/* work out how many ticks have gone since last timer interrupt */

    tval =  __raw_readl(WPCM450_TDR0);
	tdone = 10000 - tval;

	/* check to see if there is an interrupt pending */

	irqpend = __raw_readl(WPCM450_TISR);
	if (irqpend & 0x01) {
		/* re-read the timer, and try and fix up for the missed
		 * interrupt. Note, the interrupt may go off before the
		 * timer has re-loaded from wrapping.
		 */

		tval =  __raw_readl(WPCM450_TDR0);
		tdone = 10000 - tval;

		if (tval != 0)
			tdone += 10000;
	}
    
    //Justin
    //printk("wpcm450_gettimeoffset\n");
    
	return tdone;//timer_ticks_to_usec(tdone);
}


/*
 * IRQ handler for the timer
 */
static irqreturn_t
wpcm450_timer_interrupt(int irq, void *dev_id)
{
	//volatile unsigned int data;

	//data = __raw_readl(WPCM450_TISR);  

	/*----- check channel 0 -----*/
	//if (data & 0x00000001)
	//{
		write_seqlock(&xtime_lock);
		timer_tick();
		write_sequnlock(&xtime_lock);
		__raw_writel(0x01, WPCM450_TISR); /* clear TIF0, timer interrupt flag */ 
		return IRQ_HANDLED;
	//}
	//return IRQ_NONE;
}

static struct irqaction wpcm450_timer_irq = {
	.name		= "WPCM450 Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= wpcm450_timer_interrupt,
};

/*
 * Set up timer interrupt, and return the current time in seconds.
 *
 * Currently we only use timer4, as it is the only timer which has no
 * other function that can be exploited externally
 */
//unsigned int sysclk=15000000;
static void wpcm450_timer_setup (void)
{
	/*----- disable timer -----*/
	__raw_writel(0, WPCM450_TCSR0);
	__raw_writel(0, WPCM450_TCSR1);
	__raw_writel(0, WPCM450_TCSR2);
	__raw_writel(0, WPCM450_TCSR3);
	__raw_writel(0, WPCM450_TCSR4);
	
	/*----- clear interrupt flag bit -----*/
   __raw_writel(0x1F, WPCM450_TISR);  /* clear for safty */   
   
   /*----- timer 0 : periodic mode, 100 tick/sec  -----*/
   //__raw_writel(1000000/HZ, WPCM450_TICR0);
   /* timeout = 1 tick = 10 ms */
   __raw_writel(10000, WPCM450_TICR0);
   
   //Justin
   //__raw_writel(0x68000000|(sysclk/1000000), WPCM450_TCSR0);
   /* input clock for timer 0 is 1us */
   __raw_writel(0x68000000 | ((sysclk / 1000000) - 1), WPCM450_TCSR0);
   
   //Justin
   //timer_usec_ticks = (1000000 / HZ); /* 1 jiffies = 10ms = 10000us */
}

static void __init wpcm450_timer_init (void)
{
	wpcm450_timer_setup();
	setup_irq(IRQ_TIMER0, &wpcm450_timer_irq);//time0 of wpcm450
}

struct sys_timer wpcm450_timer = {
	.init		= wpcm450_timer_init,
	.offset		= wpcm450_gettimeoffset,
	.resume		= wpcm450_timer_setup
};
