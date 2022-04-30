/*
 * linux/drivers/char/wdt.h
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
 * AUTHOR :  Ashwini Pahuja<ashwini.pahuja@st.com> */


#ifndef __WDT_H
#define __WDT_H
#include "hcl_defs.h"
/*------------------------------------------------------------------------------
 ST WATCHDOG TIMER
--------------------------------------------------------------------------------
The watchdog timer generates an hot reset when it overflows, this will restart
the ARM but the code will not be downloaded again. The timer should be cleared
by the software before it overflows.
It is based on a 8 bit counter which is clocked by a slow signal coming from a
17 bit prescaler clocked by the system clock.
So the elapsing time of the watchdog timer depend on the system clock:
		RST_TIME = SYS_CLK / 2^25
------------------------------------------------------------------------------*/

#define APB_WDOG_OFFSET_CONTROL	   0x00	/* RW: controls the watchdog timer */
#define APB_WDOG_OFFSET_STATUS	   0x04	/* RW: allows the software reset handler
					 	 to determine the reset source */

typedef struct apb_wdog
{
    t_uint32 control;		/* RW: controls the watchdog timer */
    t_uint32 status;		/* RW: allows the software reset handler
				   to determine the reset source */
    t_uint32 match;
    t_uint32 count;
} apb_wdog_t;

/*------------------------------------------------------------------------------
 APB_WDOG_CONTROL register settings
------------------------------------------------------------------------------*/

#define APB_WDOG_CLEAR		0x0001	/* clear counters & prescaler */
#define APB_WDOG_ENABLE		0x0002	/* enable timer */
#define APB_WDOG_FREEZE		0x0004	/* freeze timer if arm core in debug state */
#define APB_WDOG_FAST		0x0008	/* when set, elapsing time is 16 time faster */

/*------------------------------------------------------------------------------
 APB_WDOG_STATUS register settings
------------------------------------------------------------------------------*/

#define APB_WDOG_RESET		0x0001	/* reset occured */

#endif /* __WDT_H */

/* End if file - wdt.h */
