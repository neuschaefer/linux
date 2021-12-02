/******************************************************************************
	 
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	 
 *   Copyright(c) 2008 by LG Electronics Inc.
	 
 *
	 
 *   All rights reserved. No part of this work may be reproduced, stored in a
	 
 *   retrieval system, or transmitted by any means without prior written
	 
 *   permission of LG Electronics Inc.
	 
 *****************************************************************************/

/** @file sys_ddi.h
  *
  *  System related header file 
  *
  *  @author		dhjung(dhjung77@lge.com)
  *  @version		 1.0
  *  @date		2009.10.08
  *  @note
  *  @see			 sys_ddi.c
*/

#ifndef _SYS_DDI_H_
#define _SYS_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "osa_api.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/
#define MAX_AVAIL_MMAP		10

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
/**
 * Available Memory information
 */
typedef struct
{
	UINT32	avail_mmapcount;
	UINT8 	*start_addr[MAX_AVAIL_MMAP];
	UINT32 	size[MAX_AVAIL_MMAP];
} AVAIL_MMAP_T;

/******************************************************************************
	Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
*******************************************************************************/
AVAIL_MMAP_T *DDI_SYS_GetMmapAvailable(void);
AVAIL_MMAP_T *DDI_SYS_GetMmapAvailable_FactoryOTA(void);
void *DDI_SYS_GetMmapBtPool(UINT32 ssize);
BOOLEAN DDI_SYS_ReadSpiBoot(UINT32 size, UINT8* buf, UINT32 addr);
BOOLEAN DDI_SYS_WriteSpiBoot(UINT32 size, UINT8* buf, UINT32 addr);
BOOLEAN DDI_SYS_EraseSpiBoot(UINT32 size, UINT32 addr);
DTV_STATUS_T DDI_SYS_AddMtd(void);

#endif  /* _SYS_DDI_H_ */
