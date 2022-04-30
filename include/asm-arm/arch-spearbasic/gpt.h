#ifndef __GPT_H
#define __GPT_H

/*------------------------------------------------------------------------------
-- ST GENERAL PURPOSE TIMER
--------------------------------------------------------------------------------
The Timer module contains a 16-bit counter/timer, clocked by a dedicated
3.456Mhz clock, supporting different operating modes: Auto-reload, Single-shot,
and Capture function.
It connects to the Advanced Peripheral Bus (APB) and has a series of
memory-mapped locations that allow the state of the timer module to be read
from and written to via the APB.
------------------------------------------------------------------------------*/

#define APB_TIMER_CLOCK				48000000 /* shiraz check */


#define APB_TIMER_OFFSET_CONTROL	0x00	/* RW: Controls activity */
#define APB_TIMER_OFFSET_STATUS		0x04	/* R-: Status of interrupt sources
											   (REDGCAPT, FEDGCAPT, MATCH) */

/* W-: Acknowledges interrupt source */
#define APB_TIMER_OFFSET_COMPARE	0x08	/* RW: Set comparison threshold */
#define APB_TIMER_OFFSET_COUNT		0x0C	/* R-: internal counter value */
#define APB_TIMER_OFFSET_CAPT_RE	0x10	/* R-: counter value captured under the
									rising edge */
#define APB_TIMER_OFFSET_CAPT_FE	0x14	/* R-: counter value captured under the
									falling edge */

typedef struct apb_timer {
	u32 control;			/* RW: Controls activity */
	u32 status;			/* R-: Status of interrupt sources
						(REDGCAPT, FEDGCAPT, MATCH) */
					/* W-: Acknowledges interrupt source */
	u32 compare;			/* RW: Set comparison threshold */
	u32 count;			/* R-: internal counter value */
	u32 capt_re;			/* R-: counter value captured under the
								rising edge */
	u32 capt_fe;			/* R-: counter value captured under the
								falling edge */
} apb_timer_t;

/*------------------------------------------------------------------------------
-- APB_TIMER_CONTROL register settings
------------------------------------------------------------------------------*/

#define APB_TIMER_PRESCALER_MASK	0x000F	/* divider for internal GPT clock */
#define APB_TIMER_PRESCALER_48MHZ	0x0000	/* 20.83 ns, not suitable for linux ticks */
#define APB_TIMER_PRESCALER_24MHZ	0x0001	/* 41.67 ns, not suitable for linux ticks */
#define APB_TIMER_PRESCALER_12MHZ	0x0002	/* 83.33 ns, not suitable for linux ticks */
#define APB_TIMER_PRESCALER_6MHZ	0x0003	/* 166.7 ns */
#define APB_TIMER_PRESCALER_3MHZ	0x0004	/* 333.3 ns */
#define APB_TIMER_PRESCALER_1500KHZ	0x0005	/* 666.7 ns */
#define APB_TIMER_PRESCALER_750KHZ	0x0006	/* 1333 ns */
#define APB_TIMER_PRESCALER_375KHZ	0x0007	/* 2667 ns */
#define APB_TIMER_PRESCALER_187_5KHZ	0x0008	/* 5333 ns */

#define APB_TIMER_MODE_STATUS		0x0010	/* mode bit */
#define APB_TIMER_MODE_SINGLE_SHOT	0x0010	/* single shot mode */
#define APB_TIMER_MODE_AUTO_RELOAD	0x0000	/* auto reload mode */

#define APB_TIMER_ENABLE		0x0020	/* enable timer */

#define APB_TIMER_CAPT_MODE_MASK	0x00C0	/* capture mode mask */
#define APB_TIMER_CAPT_MODE_NONE	0x0000	/* no capture */
#define APB_TIMER_CAPT_MODE_RE		0x0040	/* capture on rising edge */
#define APB_TIMER_CAPT_MODE_FE		0x0080	/* capture on falling edge */
#define APB_TIMER_CAPT_MODE_BOTH	0x00C0	/* capture on both edges */

#define APB_TIMER_INT_MATCH		0x0100	/* enable ints */

#define APB_TIMER_INT_FE		0x0200	/* enable ints on falling edge */

#define APB_TIMER_INT_RE		0x0400	/* enable ints on rising edge */

/*------------------------------------------------------------------------------
-- APB_TIMER_STATUS register settings
------------------------------------------------------------------------------*/

#define APB_TIMER_STS_MATCH		0x0001	/* enable ints */
#define APB_TIMER_STS_FE		0x0002	/* enable ints on falling edge */
#define APB_TIMER_STS_RE		0x0004	/* enable ints on rising edge */

/*------------------------------------------------------------------------------
-- APB_TIMER_COMPARE register settings
------------------------------------------------------------------------------*/

#define APB_TIMER_FREE_RUNNING		0xFFFF	/* free running mode */

/*------------------------------------------------------------------------------
-- APB TIMER default settings
------------------------------------------------------------------------------*/

#define APB_TIMER_DEFAULT_CONTROL	(	APB_TIMER_PRESCALER_375KHZ | \
						APB_TIMER_MODE_AUTO_RELOAD | \
						APB_TIMER_ENABLE )

#define APB_TIMER_TICKS_CONTROL		(	APB_TIMER_PRESCALER_6MHZ | \
						APB_TIMER_MODE_AUTO_RELOAD | \
						APB_TIMER_ENABLE )

#define APB_TIMER_DEFAULT_INTERRUPTS	(	APB_TIMER_CAPT_MODE_RE | \
						APB_TIMER_INT_MATCH | \
						APB_TIMER_INT_FE )

#endif /* __GPT_H */
