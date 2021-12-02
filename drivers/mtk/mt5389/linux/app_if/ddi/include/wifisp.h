/******************************************************************************
* DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
* Copyright(c) 1999 by LG Electronics Inc.
*
* All rights reserved. No part of this work may be reproduced, stored in a
* retrieval system, or transmitted by any means without prior written
* permission of LG Electronics Inc.
******************************************************************************/

/** @file wifisp.h
*
* This file is API header for HDCP Wi-Fi Remote Controller mouse service
* @author   Younghun, Ha(younghun.ha@lge.com)
* @version  1.0
* @date     2010.07.26
* @note
* @see
*/

/*****************************************************************************
    Header File Guarder
******************************************************************************/
#ifndef _WIFISP_H_
#define _WIFISP_H_

/****************************************************************************
    File Inlclusion
****************************************************************************/
#include "common.h"

/**
 *  Wi-Fi remocon mouse state
 */
typedef enum
{
    WIFISP_MOUSE_ST_IDLE        =   0,
    WIFISP_MOUSE_ST_SLEEP       =   1,
    WIFISP_MOUSE_ST_ACTIVE      =   2,
    WIFISP_MOUSE_ST_IR_ON       =   3,
    WIFISP_MOUSE_ST_MM_ON       =   4
} WIFISP_MOUSE_STATE_T;

typedef void (*pfnSendWiFiMouseToUI)(SINT32 nPosX, SINT32 nPosY, UINT32 nButton, UINT32 gesture_ptr);

#ifdef __cplusplus
extern "C" {
#endif

void DDI_WIFISP_SetMouseState(WIFISP_MOUSE_STATE_T state);
void DDI_WIFISP_InitializeMouseState(void);
WIFISP_MOUSE_STATE_T DDI_WIFISP_GetMouseState(void);
WIFISP_MOUSE_STATE_T DDI_WIFISP_GetPrevMouseState(void);
void DDI_WIFISP_EnableIR(void);
void DDI_WIFISP_EnableMM(void);
void DDI_WIFISP_Initialize(void);
void DDI_WIFISP_Destroy(void);
void DDI_WIFISP_RegisterWiFiMouseCallback(pfnSendWiFiMouseToUI pfnWiFiMouseToUI);
void DDI_WIFISP_RequestCursorVisible(void);
void DDI_WIFISP_RequestCursorInvisible(void);
void DDI_WIFISP_RequestCursorMove(SINT32 x, SINT32 y);
void DDI_WIFISP_RequestCursorClick(void);

#ifdef __cplusplus
}
#endif

#endif	/* _WIFISP_H_ */
