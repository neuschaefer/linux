/******************************************************************************
*	DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA.
*	Copyright(c) 1999 by LG Electronics Inc.
*
*	 All rights reserved. No part of this work may be reproduced, stored in a
*	 retrieval system, or transmitted by any means without prior written
*	 permission of LG Electronics Inc.
*****************************************************************************/

/** @file ucom_ddi.h
*
*	Internal Micom Interface header file( ONLY for MSTAR(SATURN)platform).
*
*	@author Dong-Jun Lee(athens@lge.com) 	- modifier
*	@author Kun-IL Lee(dreamer@lge.com) 	- modifier
*	@version	1.0
*	@date	  2006.07.08
*	@note
*/

/*---------------------------------------------------------
    (Header File Guarder )
---------------------------------------------------------*/
#ifndef _UCOM_DDI_H_
#define _UCOM_DDI_H_

/*---------------------------------------------------------
    Control 상수 정의
    (Control Constants)
---------------------------------------------------------*/

/*---------------------------------------------------------
    #include 파일들
    (File Inclusions)
---------------------------------------------------------*/
#include "global_configurations.h"

/*---------------------------------------------------------
    상수 정의
    (Constant Definitions)
---------------------------------------------------------*/

// moved to global_configurations.h
// #define MICOM_SIZE_OF_POWER_OFF_DATA			(128)	// 0x80: index(1) + power off status(127)

/*---------------------------------------------------------
    매크로 함수 정의
    (Macro Definitions)
---------------------------------------------------------*/

/*---------------------------------------------------------
    Type 정의
    (Type Definitions)
---------------------------------------------------------*/

typedef UINT32 (* UCOM_KEY_CALLBACK_PTR_T) (UINT8 value, UINT8 repeat );
typedef UINT32 (* UCOM_TV_LINK_CALLBACK_PTR_T) (UINT8* pData );

/*---------------------------------------------------------
    함수 선언
    (Function Declaration)
---------------------------------------------------------*/

/*=======================================================
	UCOM Interface Function 선언 (ucom_ddi.c)
=======================================================*/

extern DTV_STATUS_T	DDI_UCOM_Initialize( UCOM_KEY_CALLBACK_PTR_T pIRCallback, UCOM_KEY_CALLBACK_PTR_T pLKCallback);
extern DTV_STATUS_T	DDI_UCOM_ReadCommand(  UINT8 cmd,   UINT16 dataSize, UINT8 *pData );
extern DTV_STATUS_T	DDI_UCOM_WriteCommand( UINT8 *pCmd, UINT16 size );

/*	Get KEY(IR & LOCAL) EVENT */
extern DTV_STATUS_T	DDI_UCOM_GetKeyEvent( UINT32* pEvent, UINT32 timeout );
extern DTV_STATUS_T	DDI_UCOM_WakeUpKeyEvent( void );

/*	TV LINK MODE */
extern DTV_STATUS_T	DDI_UCOM_EnableTVLinkMode( BOOLEAN bEnabled, UCOM_TV_LINK_CALLBACK_PTR_T pTVLinkCallback );

/*	RGB EDID */
extern DTV_STATUS_T	DDI_UCOM_ReadRGBEDID( UINT32 tIndex, UINT32 tSize, UINT8 *pRdData );
extern DTV_STATUS_T	DDI_UCOM_WriteRGBEDID( UINT32 tIndex, UINT32 tSize, UINT8 *pWrData  );

#endif /* #ifndef _UCOM_DDI_H_ */
