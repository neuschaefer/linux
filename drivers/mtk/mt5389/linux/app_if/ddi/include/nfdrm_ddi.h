
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file nfdrm_ddi.h
 *
 * This source file defines the DDI functions related to NFDRM Driver control
 *
 *  @author	Kim, Sang Tae(sangeh@lge.com)
 *  @version	0.1
 *  @date		2009.08.25
 *  @note		Additional information.
 *  @see		nfdrm_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_NFDRM_DDI_H_
#define	_NFDRM_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM 
#include "common.h"
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
/*APIs for Getting Pre-Loaded Keys. Used at Netflix Add On*/
DTV_STATUS_T DDI_NFDRM_InitIDFile(char *pInputFile);
DTV_STATUS_T DDI_NFDRM_GetKpe(UINT8 *pKpe);
DTV_STATUS_T DDI_NFDRM_GetKph(UINT8 *pKph);
DTV_STATUS_T DDI_NFDRM_GetEsn(UINT8 *pEsn);
DTV_STATUS_T DDI_NFDRM_FinalizeIDFile(void);

/*APIs for Encrypt/Decrypt Run Time Keys. Used at Netflix Add On*/
#if(PLATFORM_TYPE == BCM_PLATFORM)
DTV_STATUS_T DDI_NFDRM_BCM_InitializeKeySlot(void);				//BRCM Only
DTV_STATUS_T DDI_NFDRM_BCM_FinalizeKeySlot(void);				//BRCM Only
#endif
DTV_STATUS_T DDI_NFDRM_Encrypt(UINT8 *pBuf, UINT32 size);
DTV_STATUS_T DDI_NFDRM_Decrypt(UINT8 *pBuf, UINT32 size);
DTV_STATUS_T DDI_NFDRM_Random(UINT8 *pBuf, UINT32 size);

/*APIs for Secure Firmware Update. Used at Only DTV Native*/
#if(PLATFORM_TYPE == BCM_PLATFORM)
DTV_STATUS_T DDI_NFDRM_BCM_InitializeSFU(char *pInputFile);					//BRCM Only
DTV_STATUS_T DDI_NFDRM_BCM_FinalizeSFU(void);								//BRCM Only
#endif
DTV_STATUS_T DDI_NFDRM_GetKpub(UINT8 *pKpub);
DTV_STATUS_T DDI_NFDRM_GetKu(UINT8 *pKu);

#endif

