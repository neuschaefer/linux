/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file cecp_ddi.h
 *
 *  ADEC DD를 제어하는 DDI 함수 header 파일.
 *  AUDIO DDI와 ADEC DD를 연결하는 함수들로 주로 AUDIO DDI와 task에서 호출된다.
 *
 *  @author		Park Jeong-Gun(alwaysok@lge.com)
 *  @version	1.0
 *  @date		2006.04.24
 *  @note
 *  @see		cecp_ddi.c
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _CECP_DDI_H_
#define _CECP_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#include "branches.h"
#include "common.h"
#include "osa_api.h"
#include "global_configurations.h"

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

DTV_STATUS_T DDI_CECP_SetMicomReady(void);

#endif /* _CECP_DDI_H_ */

