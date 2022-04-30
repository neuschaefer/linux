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


#include "spr_rtc_st_hal.h"


PRIVATE t_rtc_register 	*pRtcReg;

/****************************************************************************/
/* NAME: RTC_SetBaseAddress                                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*       Sets the base address of RTC                                       */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    RTC_BADD                                                        */
/*                                                                          */
/* OUT :    none                                                            */
/*                                                                          */
/*                                                                          */
/* RETURN:    none                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

PUBLIC void	
RTC_SetBaseAddress  ( t_logical_address RTC_BADD )  
{
/* Initialize the RTC registers structure */
	pRtcReg =  ( t_rtc_register * )   RTC_BADD;
}


/****************************************************************************/
/* NAME: RTC_SetTime                                                       */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Sets the RTC time                                                   */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     val                                                            */
/*                                                                          */
/* OUT :      none                                                          */
/*                                                                          */
/*                                                                          */
/* RETURN:    none                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_SetTime ( unsigned int val )  
{
	pRtcReg->TIME_REG = val;
}


/****************************************************************************/
/* NAME:  RTC_SetDate                                                      */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*       Set the RTC date                                                   */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    val                                                             */
/*                                                                          */
/* OUT :    none                                                            */
/*                                                                          */
/*                                                                          */
/* RETURN:  none                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_SetDate ( unsigned int val )  
{
	pRtcReg->DATE_REG = val;
}


/****************************************************************************/
/* NAME: RTC_GetDate                                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*       Read the date from RTC                                             */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     *pDate                                                         */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_GetDate ( unsigned int *pDate )  
{
	*pDate = pRtcReg->DATE_REG;
}


/****************************************************************************/
/* NAME:  RTC_GetTime                                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     *ptime                                                         */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN:  none                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_GetTime ( unsigned int *pTime )  
{
	*pTime = pRtcReg->TIME_REG;
}


/****************************************************************************/
/* NAME:  RTC_SetAlarmTime                                                  */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :  val                                                               */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void
RTC_SetAlarmTime ( unsigned int val )  
{
	pRtcReg->ALARM_TIME_REG = val;
}


/****************************************************************************/
/* NAME:  RTC_SetAlarmDate                                                  */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    val                                                             */
/*                                                                          */
/* OUT :    none                                                            */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_SetAlarmDate ( unsigned int val )  
{
	pRtcReg->ALARM_DATE_REG = val;
}


/****************************************************************************/
/* NAME:  RTC_GetAlarmDate                                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     *pdate                                                         */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_GetAlarmDate ( unsigned int *pDate )  
{
	*pDate = pRtcReg->ALARM_DATE_REG;
}


/****************************************************************************/
/* NAME:  RTC_GetAlarmTime                                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     *ptime                                                         */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_GetAlarmTime ( unsigned int *pTime )  
{
	*pTime = pRtcReg->ALARM_TIME_REG;
}


/****************************************************************************/
/* NAME:  RTC_IntEnable                                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     none                                                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN:  none                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_IntEnable ( )  
{
	pRtcReg->CONTROL_REG |= RTC_INT_STATUS; //interrupt enable
}


/****************************************************************************/
/* NAME:   RTC_IntDisable                                                      */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     none                                                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN:  none                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_IntDisable ( )  
{
	pRtcReg->CONTROL_REG &= ~RTC_INT_STATUS; //interrupt clear
}


/****************************************************************************/
/* NAME:     RTC_GetIntStatus                                                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     none                                                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN: unsigned int                                                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

unsigned int 
RTC_GetIntStatus (  )  
{
	return pRtcReg->STATUS_REG;
}


/****************************************************************************/
/* NAME:  RTC_ClearInt                                                     */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     none                                                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN:   none                                                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_ClearInt (  )  
{
	pRtcReg->STATUS_REG = RTC_INT_STATUS;
}


/****************************************************************************/
/* NAME:   RTC_SetComparisionMask                                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     val                                                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN: none                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
RTC_SetComparisionMask ( unsigned int val )  
{
	pRtcReg->CONTROL_REG = val;
}
