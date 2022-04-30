/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/
#ifndef SPEAR_WDT_H
#define SPEAR_WDT_H

#include "spr_wdt_sp805_hal.h"
#include <asm/arch/kernel_defs.h>

#define SPEAR_WDT_BASE 'G'
#define WDTIOC_START_TIMER _IOWR(SPEAR_WDT_BASE,1, uint)
#define WDTIOC_REFRESH_TIMER _IOWR(SPEAR_WDT_BASE,2,uint)
#define WDTIOC_STOP_TIMER _IOWR(SPEAR_WDT_BASE,3,uint)

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This registers the Driver
static int __init SPEAR_WDT_Init(void);

//This unregisters the Driver
static void __exit SPEAR_WDT_Exit(void);

//This API is used to Start the Timer.
t_WDTError SPEAR_WDT_StartTimer(uint ResetTime);

//This API is used to Refresh the Timer.
void SPEAR_WDT_RefreshTimer(void);

//This API is used to Stop the Timer.
void SPEAR_WDT_StopTimer(void);

//This API is used to Get the time left to reset the system.
t_WDTError SPEAR_WDT_GetTimeLeft(uint *ResetTimeLeft);

//Interrupt handler.
//static irqreturn_t SPEAR_WDT_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg);

#endif // SPEAR_WDT_H
