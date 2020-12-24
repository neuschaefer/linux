/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * AESS Hardware Watchdog Driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#ifndef VWDT_H
#define VWDT_H

#include <asm/arch/map.h>

#define VWDT_MAX_DEVS		1				/* Define supporting WDTs */
#define WPCM450_BASE_WDT_REG    WPCM450_VA_TMR

#define WDT_REG_CONTROL_ADDR    			(WPCM450_BASE_WDT_REG + 0x1c)

#define REG_WDT_CTRL			0x1c			/* watchdog control Register		*/

#define WDT_017SEC_COUNT	0 /* 0.17sec */
#define WDT_067SEC_COUNT	1 /* 0.67sec */
#define WDT_268SEC_COUNT	2 /* 2.68sec */
#define WDT_107SEC_COUNT	3 /* 10.7sec */

#define WDT_RESET_BIT0	0 /* WTR: watchdog timer reset */
#define WDT_RESET_ENABLE_BIT1	1 /* WTRE: watchdog timer reset enable */
#define WDT_RESET_FLAG_BIT2 2 /* WTRF: watchdog timer reset flag */
#define WDT_INTERRUPT_FLAG_BIT3	3 /* WTIF: watchdog timer interrupt flag */
#define WDT_INTERVAL_SELECT_BIT4	4 /* WTIS: watchdog timer interval select */
#define WDT_INTERVAL_SELECT_BIT5	5
#define WDT_INTERRUPT_ENABLE_BIT6	6 /* WTIE: watchdog timer interrupt enable */
#define WDT_TIMER_ENABLE_BIT7	7 /* WTE: watchdog timer enable */
#define WDT_TEST_MODE_ENABLE_BIT8	8 /* WTTME: watchdog timer test mode enable */
#define WDT_DEBUG_MODE_BIT9 9 /* FREEZE_EN: ICE debug mode acknowledge enable */
#define WDT_CLOCK_BIT10 10 /* WTCLK: watchdog timer clock */

/* TBD: For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1

enum {
	WDT_CLOSE_NOT_ALLOW,
	WDT_CLOSE_ALLOW
};

struct vwdt_private
{
	int base_addr;		/* HW WDT base addr			*/
	unsigned long used;	/* WDT Used data for lock	*/
	int cstat;			/* WDT allow close status	*/
	int count;			/* WDT count for HW Register*/
};

#endif /* VWDT_H */

