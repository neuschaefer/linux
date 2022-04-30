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
#ifndef SPEAR_WDT_HAL_H
#define SPEAR_WDT_HAL_H

#include <linux/types.h>
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
//Base Address of WDT..
#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAR_WDT_APB_BASE VA_SPEARBASIC_START_APB_WDT
#else
#define SPEAR_WDT_APB_BASE VA_SPEARPLUS_START_APB_WDT
#endif

//WDT Registers Base Address Mapping
#define WDTLOAD 	((volatile uint *)(SPEAR_WDT_APB_BASE + 0x000))
#define WDTVALUE	((volatile uint *)(SPEAR_WDT_APB_BASE + 0x004))
#define WDTCONTROL	((volatile uint *)(SPEAR_WDT_APB_BASE + 0x008))
#define WDTINTCLR	((volatile uint *)(SPEAR_WDT_APB_BASE + 0x00C))
#define WDTRIS		((volatile uint *)(SPEAR_WDT_APB_BASE + 0x010))
#define WDTMIS		((volatile uint *)(SPEAR_WDT_APB_BASE + 0x014))
#define WDTLOCK		((volatile uint *)(SPEAR_WDT_APB_BASE + 0xC00))


//Constants for configuring WDT Registers
typedef enum
{
	INT_ENABLE		= 0x00000001,
	INT_DISABLE		= ~(INT_ENABLE),
	RESET_ENABLE		= 0x00000002,
	RESET_DISABLE		= ~(RESET_ENABLE),
	INT_CLEAR		= 0x00000001,
	INT_STATUS		= 0x00000001,
	REG_LOCKED		= 0x00000001,
	REG_UNLOCKED		= 0x00000000,
	UNLOCK_REG		= 0x1ACCE551,
	LOCK_REG		= 0x00000001
}t_WDTReg;

#define MAX_RESET_TIME 0xFFFFFFFF

//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	WDT_OK = 0,
	WDT_INVALID_RESET_TIME,
	WDT_NULL_POINTER,
	WDT_RESET_TIME_NOT_SET,
	WDT_UNKNOWN_IOCTL_CMD
}t_WDTError;


//typedef for a pointer to a interrupt handler
typedef void (* t_WDTIntHandler)(void);


/***********************************************************************************
 * PUBLIC Functions
***********************************************************************************/
//This API is used to Start the Timer.
t_WDTError WDT_StartTimer(uint ResetTime);

//This API is used to Refresh the Timer.
void WDT_RefreshTimer(void);

//This API is used to Stop the Timer.
void WDT_StopTimer(void);

//This API is used to Get the time left to reset the system.
t_WDTError WDT_GetTimeLeft(uint *ResetTimeLeft);

#endif // SPEAR_WDT_HAL_H
