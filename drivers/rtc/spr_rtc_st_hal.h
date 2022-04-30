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


#ifndef __RTC_HAL_H
#define __RTC_HAL_H


#ifdef __cplusplus
extern  "C" {           /* C declarations in C++ */
#endif

#include <asm/arch/hcl_defs.h>

typedef volatile struct
{
  unsigned int TIME_REG;       /* 30000C00 */
  unsigned int DATE_REG;       /* 30000C04 */
  unsigned int ALARM_TIME_REG; /* 30000C08 */
  unsigned int ALARM_DATE_REG; /* 30000C0C */
  unsigned int CONTROL_REG;    /* 30000C10 */
  unsigned int STATUS_REG;     /* 30000C14 */
} t_rtc_register;


/* Fields position */

/* TIME_REG & ALARM_TIME_REG*/
#define SECONDS_UNITS    0x0000000f  /* seconds units position */
#define SECONDS_TENS     0x00000070  /* seconds tens position */
#define MINUTES_UNITS    0x00000f00  /* minutes units position */
#define MINUTES_TENS     0x00007000  /* minutes tens position */
#define HOURS_UNITS      0x000f0000  /* hours units position */
#define HOURS_TENS       0x00300000  /* hours tens position */

/* MASK SHIFT TIME_REG & ALARM_TIME_REG*/
#define SECONDS_SHIFT    0x00000000  /* seconds units */
#define MINUTES_SHIFT    0x00000008  /* minutes units position */
#define HOURS_SHIFT      0x00000010  /* hours units position */

/* DATE_REG & ALARM_DATE_REG */
#define DAYS_UNITS       0x0000000f /* days units position */
#define DAYS_TENS        0x00000030 /* days tens position */
#define MONTHS_UNITS     0x00000f00 /* months units position */
#define MONTHS_TENS      0x00001000 /* months tens position */
#define YEARS_UNITS      0x000f0000 /* years units position */
#define YEARS_TENS       0x00f00000 /* years tens position */
#define YEARS_HUNDREDS   0x0f000000 /* years hundereds position */
#define YEARS_MILLENIUMS 0xf0000000 /* years milleniums position */

/* MASK SHIFT TIME_REG & ALARM_TIME_REG*/
#define DAYS_SHIFT 	 0x00000000      /* seconds units */
#define MONTHS_SHIFT	 0x00000008      /* minutes units position */
#define YEARS_SHIFT      0x00000010      /* hours units position */


/* Control values */

/* CONTROL_REG */
#define NO_MASK          0x00000000 /* mask bits are 0x0000000f */
#define MASK_SEC         0x00000001 /* mask bit 0 */ 
#define MASK_MIN         0x00000002 /* mask bit 1 */
#define MASK_HOUR        0x00000004 /* mask bit 2 */
#define MASK_DAY         0x00000008 /* mask bit 3 */
#define MASK_MONTH       0x00000010 /* mask bit 4 */
#define MASK_YEAR        0x00000020 /* mask bit 5 */
#define PRESCALER_BYP    0x00000100 /* bypass the prescaler: for debug only */
#define TIME_BYP         0x00000200 /* date reg equal to time reg: for debug only */
#define INT_ENA          0x80000000 /* interrupt enable */

/* STATUS_REG */
#define CLK_UNCONNECTED  0x00000001 /* timer isolated, time and date can be write */
#define PEND_WR_TIME     0x00000004 /* a write on time_reg is pending */
#define PEND_WR_DATE     0x00000008 /* a write on date_reg is pending */
#define LOST_WR_TIME     0x00000010 /* a write on time_reg is lost */
#define LOST_WR_DATE     0x00000020 /* a write on date_reg is lost */
#define RTC_INT_STATUS   0x80000000 /* clear: int clear; read: int status */

PUBLIC void	RTC_SetBaseAddress (t_logical_address );
void RTC_SetTime(unsigned int );
void RTC_SetDate(unsigned int );
void RTC_GetDate(unsigned int *);
void RTC_GetTime(unsigned int *);
void RTC_SetAlarmTime(unsigned int );
void RTC_SetAlarmDate(unsigned int );
void RTC_GetAlarmDate(unsigned int *);
void RTC_GetAlarmTime(unsigned int *);
void RTC_IntEnable(void);
void RTC_IntDisable(void);
unsigned int RTC_GetIntStatus(void);
void RTC_ClearInt(void);
void RTC_SetComparisionMask(unsigned int val);


#ifdef __cplusplus
} /* C declarations in C++ */
#endif

#endif 
