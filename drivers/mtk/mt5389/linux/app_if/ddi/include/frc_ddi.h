
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file frc_ddi.h
 *
 * This source file defines the DDI functions related to FRC Driver control
 *
 *  @author	Hong, Young Ho(longsky@lge.com)
 *  @version	0.1
 *  @date		2009.06.25
 *  @note		Additional information.
 *  @see		frc_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_FRC_DDI_H_
#define	_FRC_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM // modified by dilly97 : fix include
#include "common.h"
#else
#include "common_win32.h"
#endif
#include "i2c_ddi.h"
#include "global_configurations.h"
#include "pql_ddi.h" //YKKim 100907 because of setting WB, GammaTable.


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

/**
 * pointer strut for FRC function.
*/
typedef struct FRC_DDI_CHIP
{
	DTV_STATUS_T	(*pfnUpdateFirmwareFRC) (UINT8 *pbuffer, UINT32 size);
	DTV_STATUS_T	(*pfnInitializeFRC) (void);
	DTV_STATUS_T	(*pfnResetSWFRC) (void);
	DTV_STATUS_T	(*pfnMotionCompensationOnOffFRC) (BOOLEAN bCtrl);
	DTV_STATUS_T	(*pfnSetMotionCompensationFRC) (UINT8 blurLevel, UINT8 judderLevel, FRC_MC_TYPE_T motion);
	DTV_STATUS_T	(*pfnSetTrueMotionDemoFRC) (BOOLEAN bOnOff);
	DTV_STATUS_T	(*pfnSetDemoBarFRC) (BOOLEAN bOnOff, UINT8 r, UINT8 g, UINT8 b);
	DTV_STATUS_T	(*pfnSetVideoBlockFRC) (FRC_MUTE_TYPE_T type, BOOLEAN bOnOff);
	DTV_STATUS_T	(*pfnSetDisplayOutputFRC) (BOOLEAN bCtrl);
	DTV_STATUS_T	(*pfnSetBypassMEMCRegionFRC) (BOOLEAN bOnOff, FRC_REGION_T  region, FRC_RECT_T rect);
	DTV_STATUS_T	(*pfnGetFWVersionFRC) (UINT16 *u16Version);
	DTV_STATUS_T	(*pfnSetSpreadSpectrumFRC) (UINT16 u16Period, UINT16 u16Percent, BOOLEAN bEnable, BOOLEAN bMiuLVDS);
	DTV_STATUS_T	(*pfnSetDebugBlockFRC) (BOOLEAN bEnable);
	DTV_STATUS_T	(*pfnSetReverseControlFRC) (UINT8 u8Mode);
	DTV_STATUS_T	(*pfnSetRGB444Mode) (UINT8 u8mode);
	DTV_STATUS_T	(*pfnDebugMenu) (void);
	// PQL, BLU
	DTV_STATUS_T	(*pfnSetODCTableFRC) (const UINT8* pODTbl);
	DTV_STATUS_T	(*pfnSetWBColorTempFRC) (PQL_COLOR_TEMP_T cscPcCtData);
	DTV_STATUS_T	(*pfnSetWBColorTempFRCAtFactory) (PQL_COLOR_TEMP_T cscPcCtData);
	DTV_STATUS_T	(*pfnSetGammaTableFRC) (PQL_PATH_T pqlPath, UINT32 *pRedGammaTable, UINT32 *pGreenGammaTable, UINT32 *pBlueGammaTable);
	DTV_STATUS_T	(*pfnLowDelayMode) (UINT8 type);
	// 3D.
	DTV_STATUS_T	(*pfnGpioControl) (UINT8 u8GpioNum, BOOLEAN bData);
	DTV_STATUS_T	(*pfnGpioRead) (UINT8 u8GpioNum, UINT8 *bData);
} FRC_DDI_CHIP_T;

/**
 * Runtime binding을 위한 structure
 */
typedef struct FRC_DDI_RUNBIND
{
	FRC_CHIP_TYPE_T	frcType;
	FRC_DDI_CHIP_T	*pFuncSet;
} FRC_DDI_RUNBIND_T;

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
DTV_STATUS_T DDI_FRC_UpdateFirmware(UINT8 *pbuffer, UINT32 size);
DTV_STATUS_T DDI_FRC_Initialize(void);
DTV_STATUS_T DDI_FRC_ResetSW(void);
DTV_STATUS_T DDI_FRC_MotionCompensationOnOff(BOOLEAN bOnOff);
DTV_STATUS_T DDI_FRC_SetMotionCompensation(UINT8 blurLevel, UINT8 judderLevel,FRC_MC_TYPE_T motion);
DTV_STATUS_T DDI_FRC_SetTrueMotionDemo(BOOLEAN bOnOff);
DTV_STATUS_T DDI_FRC_SetDemoBar(BOOLEAN bOnOff, UINT8 r, UINT8 g, UINT8 b);
DTV_STATUS_T DDI_FRC_SetVideoBlock (FRC_MUTE_TYPE_T type, BOOLEAN bOnOff);
DTV_STATUS_T DDI_FRC_SetDisplayOutput (BOOLEAN bOnOff);
DTV_STATUS_T DDI_FRC_SetBypassMEMCRegion(BOOLEAN bOnOff, FRC_REGION_T region, FRC_RECT_T rect);
DTV_STATUS_T DDI_FRC_GetFirmwareVersion (UINT16 *pVersion);
DTV_STATUS_T DDI_FRC_SetSpreadSpectrum(BOOLEAN bEnable, UINT16 u16Percent, UINT16 u16Period);
DTV_STATUS_T DDI_FRC_SetODCTable(const UINT8* pODTbl);
DTV_STATUS_T DDI_FRC_FreezeControl(UINT8 type);
DTV_STATUS_T DDI_FRC_LowDelayMode(UINT8 type);
DTV_STATUS_T DDI_FRC_GpioControl(UINT8 u8GpioNum,BOOLEAN bData);
DTV_STATUS_T DDI_FRC_GpioRead(UINT8 u8GpioNum,UINT8 *bData);
DTV_STATUS_T DDI_FRC_SetRGB444Mode(UINT8 u8mode);
DTV_STATUS_T DDI_FRC_SetWBColorTemp(PQL_COLOR_TEMP_T cscPcCtData);
DTV_STATUS_T DDI_FRC_SetWBColorTempAtFactory(PQL_COLOR_TEMP_T cscPcCtData);
void DDI_FRC_Debug(void);

//Internal FRC Control function.
DTV_STATUS_T	DDI_VIDEO_FRC_Initialize(void);
DTV_STATUS_T 	DDI_VIDEO_FRC_MotionCompensationOnOff(BOOLEAN bOnOff);
DTV_STATUS_T 	DDI_VIDEO_FRC_SetMotionCompensation(UINT8 blurLevel, UINT8 judderLevel, FRC_MC_TYPE_T motion);
DTV_STATUS_T 	DDI_VIDEO_FRC_SetTrueMotionDemo(BOOLEAN bOnOff);
DTV_STATUS_T 	DDI_VIDEO_FRC_SetDemoBar(BOOLEAN bOnOff, UINT8 r, UINT8 g, UINT8 b);
DTV_STATUS_T	DDI_VIDEO_FRC_GetFirmwareVersion(UINT16 *pVersion);
DTV_STATUS_T	DDI_VIDEO_FRC_SetBypassMEMCRegion(BOOLEAN bOnOff, FRC_REGION_T  region, FRC_RECT_T rect);
DTV_STATUS_T	DDI_VIDEO_FRC_DEBUG(void);

#endif
