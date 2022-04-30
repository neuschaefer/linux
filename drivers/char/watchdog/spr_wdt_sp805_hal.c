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

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include "spr_wdt_sp805_hal.h"

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

#ifndef NULL
#define NULL 0
#endif

static int CurrentResetTime;

//Clock Rate of WDT
extern uint WDT_CLOCK_TICK_RATE;

/****************************************************************************/
/* 			 WDT_StartTimer()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Starts the Timer. The Reset time for CPU is given in   */
/*      milliseconds.						            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ResetTime: Reset Time in Milliseconds	                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_WDTError WDT_StartTimer(uint ResetTime)
{
	unsigned long TimerCount;

	//If the reset time sent was Zero.
	if(!ResetTime)
		return WDT_INVALID_RESET_TIME;

	//Calculate TimerCount.
	TimerCount = ((WDT_CLOCK_TICK_RATE/2000) * ResetTime);

	//If Timer count exceeds the max programmable amount then set the time as the max programmable amount.
	if(TimerCount > MAX_RESET_TIME)
		TimerCount = MAX_RESET_TIME;

	CurrentResetTime= ResetTime;

	//Unlock the registers.
	*WDTLOCK = UNLOCK_REG;

	//Stop Timer.
	*WDTCONTROL &= INT_DISABLE;
	*WDTCONTROL &= RESET_DISABLE;

	//Set Reset Time.
	*WDTLOAD = TimerCount;

	//Clears the Interrupt.
	*WDTINTCLR = INT_CLEAR;

	//Start Timer.
	*WDTCONTROL = INT_ENABLE | RESET_ENABLE;

	//Lock the registers.
	*WDTLOCK = LOCK_REG;

	return WDT_OK;
}


/****************************************************************************/
/* NAME: 		WDT_RefreshTimer                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Refresh the Timer.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void WDT_RefreshTimer(void)
{
	WDT_StopTimer();
	WDT_StartTimer(CurrentResetTime);
}

/****************************************************************************/
/* NAME: 		WDT_StopTimer	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Stops the Timer.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void WDT_StopTimer(void)
{
	//Unlock the registers.
	*WDTLOCK = UNLOCK_REG;

	//Stop Timer.
	*WDTCONTROL &= INT_DISABLE;
	*WDTCONTROL &= RESET_DISABLE;

	//Reset Count Register.
	*WDTLOAD = 0;

	//Clears the Interrupt.
	*WDTINTCLR = INT_CLEAR;

	//Lock the registers.
	*WDTLOCK = LOCK_REG;
}


/****************************************************************************/
/* NAME: 		WDT_GetTimeLeft	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Returns the Time Left for the CPU to reset.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/*                                                                          */
/* OUT : none                                                               */
/*      - ResetTime: Reset Time in Milliseconds	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                   	                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_WDTError WDT_GetTimeLeft(uint *ResetTimeLeft)
{
	uint TimerCount = 0;

	//If the pointer to ResetTimeLeft is NULL.
	if(ResetTimeLeft == NULL)
		return WDT_NULL_POINTER;

	//Get Count Left in Value Register.
	TimerCount = *WDTVALUE;

	if(TimerCount == *WDTLOAD)
		return WDT_RESET_TIME_NOT_SET;

	//If the interrupt is inactive then time left is WDTValue + WDTLoad.
	if(!(*WDTMIS & INT_STATUS))
		TimerCount = TimerCount + *WDTLOAD;

	//Calculate Time Left.
	*ResetTimeLeft = TimerCount/(WDT_CLOCK_TICK_RATE/1000);

	return WDT_OK;
}
