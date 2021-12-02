/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2008 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file hdmisw_ddi.h
 *
 * This source file defines the DDI functions related to HDMI Switch Device Driver control
 *
 *  @author	Lee, byung gul[ashton](lachesis@lge.com)
 *  @version	0.1
 *  @date		2008.07.19
 *  @note		Additional information.
 *  @see		hdmisw_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_HDMISW_DDI_H_
#define	_HDMISW_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM //Modified by ruthkyj: fix include
#include "common.h"
#include "video_ddi.h"
#else
#include "common_win32.h"
#endif

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
   로컬 상수 정의 (Local Constant Definitions)
******************************************************************************/

/******************************************************************************
    로컬 형 정의 (Local Type Definitions)
******************************************************************************/

/**
 * for other hdmi switch.
*/
typedef enum
{
	HDMI_PORT1,
	HDMI_PORT2,
	HDMI_PORT3,
	HDMI_PORT4,
	RGB_PORT, //ieeum_rgb
} HDMI_PORT_T;

/**
 * for other hdmi switch.
*/
typedef enum
{
	USE_ONLY_HPD,
	USE_HPD_DVICLOCK_DDC,
	USE_HPD_DVICLOCK_DDC_NODELAY,

} HPD_TYPE_T;


/******************************************************************************
    전역 형 정의 (Global Type Definitions)
******************************************************************************/

/******************************************************************************
    Extern 전역변수와 함수 prototype 선언
    (Extern Variables & Function Prototype Declarations)
******************************************************************************/

/******************************************************************************
    Static 변수와 함수 prototype 선언
    (Static Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    로컬 변수와 함수 prototype 선언
    (Local Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    전역 변수와 함수 prototype 선언
    (Global Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
	함수 정의 (Function Definitions)
******************************************************************************/

DTV_STATUS_T DDI_HDMISW_Initalize(void);
DTV_STATUS_T DDI_HDMISW_SelectInput(UINT8 uiHdmiPort);
UINT8	DDI_HDMISW_ReadHPD(UINT8 uiHdmiPort);
DTV_STATUS_T DDI_HDMISW_SelectHPD(UINT8 uiHdmiPort, UINT8 hpdctrl, UINT32 param);
DTV_STATUS_T DDI_HDMISW_IsConnected(UINT8 uiHdmiPort, UINT8 *pData);
DTV_STATUS_T DDI_HDMISW_PeriodicTask(void);
DTV_STATUS_T DDI_HDMISW_ReadHDMIEDID(UINT8 data[256], UINT8 port);
DTV_STATUS_T DDI_HDMISW_ReadRGBEDID(UINT8 data[128]);
UINT8 DDI_HDMISW_ReadEEPROMData(UINT8 page, UINT8 offset);		// for NXP switch
void DDI_HDMISW_DownloadHDMIEDID(UINT8 data[256]);
void DDI_HDMISW_DownloadRGBEDID(UINT8 pRGBEDIDData[256]);
DTV_STATUS_T DDI_HDMISW_EraseEDID(UINT8 type);
void DDI_HDMISW_DEBUG(void);
DTV_STATUS_T DDI_HDMISW_SetEQ(UINT8 strength);

UINT8 DDI_HDMISW_MatchToRealHWPort(UINT8 uiHdmiPort);
void DDI_HDMISW_ContorlHPD(UINT8 hpd_time);



#ifdef INCLUDE_WIRELESS_READY
void DDI_HDMISW_SetWirelessHDMIPort(UINT8 port);
#endif

#endif
