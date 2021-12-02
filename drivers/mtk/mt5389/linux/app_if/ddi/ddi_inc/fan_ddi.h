/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
*******************************************************************************/

/** @file fan_ddi.h
 *
 *	This header file defines the DDI functions related to Fan Driver
 *
 *	@author 	Lee, SangSun(oxygenman@lge.com)
 *	@version	0.1
 *	@date		2006.07.01
 *	@date		2006.12.21 - Modified
 *	@note
 *	@see
 */

#ifndef _FAN_DDI_H_
#define _FAN_DDI_H_


/*==========================================================
	User Type Definition
==========================================================*/
/**
 * Fan Speed Definition
 *
 */
typedef enum
{
	FAN_OFF 	  	= 0,
	FAN_LOW_SPEED 	= 1,
	FAN_HIGH_SPEED	= 2

}	FAN_MODE_T;

/**
 * Number of FAN Definition
 *
 */
typedef enum
{
	FAN_TYPE_4_PANEL	= 0x30,	/* ES에서 Fan P1607 하나만 사용 */
	FAN_TYPE_1_PANEL	= 0x20,	/* ES에서 Fan P1607 하나만 사용 */
	FAN_TYPE_POD	= 0x04
}	FAN_TYPE_T;


/*==========================================================
	Function Prototype Definition
==========================================================*/
DTV_STATUS_T DDI_FAN_Initialize(void);
SINT8 DDI_FAN_GetTemperature( void );
void DDI_FAN_SetFanStopFlag(BOOLEAN flag);

#endif	/* end of _FAN_DDI_H_ */

