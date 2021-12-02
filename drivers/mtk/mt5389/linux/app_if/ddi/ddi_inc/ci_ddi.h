/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 		ci_ddi.h
 *
 *  header of Common Interface DDI Fuction.
 *
 *
 *  @author		Oh, kyongjoo(kjoh@lge.com)
 *  @author
 *  @version	1.0
 *  @date		2007.5.1
 *  @date
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _CI_DDI_H_
#define _CI_DDI_H_

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <osa_api.h>
#include <common.h>

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
DTV_STATUS_T DDI_CI_Init(SINT32 *pDeviceHandle);
BOOLEAN DDI_CI_DetectCard(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_Reset(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_BypassOn(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_BypassOff(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_CheckCIS(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_WriteCOR(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_Read(SINT32 deviceHandle, UINT8 *pData, UINT16 *pSize);
DTV_STATUS_T DDI_CI_Write(SINT32 deviceHandle, UINT8 *pData, UINT16 size);
DTV_STATUS_T DDI_CI_NegoBuf(SINT32 deviceHandle, UINT16 *pBufSize);
SINT32 DDI_CI_ReadDAStatus(SINT32 deviceHandle);

#endif /*_CI_DDI_H_ */

