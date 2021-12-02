/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file common.h
 *
 *	Common Used Type Definitions.
 *
 *	@author 	Changwook Lim (changwook.im@lge.com)
 *	@version	1.0
 *	@date		2005. 6. 1
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _XLIBC_TIME_H_
#define _XLIBC_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include "dbgfrwk_types.h"

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/

/******************************************************************************
	매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/*--------------------------------------------------------------------------+
| Constant Define
+---------------------------------------------------------------------------*/
#define	XLIBC_TIME_CLOCK_RESOURCE_MAX		(8)	/** 사용 가능한(Clock은 생성하는 것이 아님) Clock의 최대 개수 */

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
/**
 *	Time structure type definitions
 *
 *	\- year 	: 1970 ~ 65535 <P>
 *	\- month	: 1 ~ 12 <P>
 *	\- day		: 1 ~ 31 <P>
 *	\- hour 	: 0 ~ 23 <P>
 *	\- minute	: 0 ~ 59 <P>
 *	\- second	: 0 ~ 59 <P>
 */
typedef 	struct
{
	unsigned short		year;
	unsigned char		month;
	unsigned char 		day;
	unsigned char 		hour;
	unsigned char 		minute;
	unsigned char 		second;
}xlibc_time_t;


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int xlibc_time_getelapsedms (uint64_t *pMiliSec);
extern int xlibc_time_convert_sec2time (xlibc_time_t *pTime, uint64_t sec);
extern int xlibc_time_convert_time2sec (xlibc_time_t *pTime, uint64_t *pSec);
extern int xlibc_time_setclock(unsigned int clockId, xlibc_time_t *pTime);
extern int xlibc_time_getclock(unsigned int clockId, xlibc_time_t *pTime, unsigned short *miliSec);


/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_TIME_H_ */
