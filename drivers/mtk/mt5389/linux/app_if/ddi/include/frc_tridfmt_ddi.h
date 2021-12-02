/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file frc_tridfmt_ddi.h
 *
 * This source file defines the DDI functions related to Formatter Device Driver control
 *
 *  @author	Lee, Yong Uk (yonguk@lge.com)
 *  @version	0.1
 *  @date	2010.0721
 *  @note	Additional information.
 *  @see frc_tridfmt_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_FRC_TRIDFMT_DDI_H_
#define	_FRC_TRIDFMT_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
*****************************************************************************/
#ifndef WIN32_SIM
#include "common.h"
#else
#include "common_win32.h"
#endif

#include "global_configurations.h"

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
   로컬 상수 정의 (Local Constant Definitions)
******************************************************************************/

/*****************************************************************************4b
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

/**
 * pointer strut for FRC function.
*/
typedef struct FRC_TRIDFMT_DDI_CHIP
{
	//Initialization.
	DTV_STATUS_T (*pfnInitialize)(void); // Formatter Initialization

	//Basic function.
	DTV_STATUS_T (*pfnSetScreenOnOff)(BOOLEAN bOnOff); // Formatter Screen On Off
	DTV_STATUS_T (*pfnSetFMTMode)(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
	DTV_STATUS_T (*pfnSet3DModeOnOff)(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);//(UINT8 u8Ctrl); // Formatter On Off

	//Input configuration
	DTV_STATUS_T (*pfnToggleInputLRFrame)(TRIDTV_INPUT_LR_SEQ_T type); // Formatter LR Frame toggle
	DTV_STATUS_T (*pfnSelectInputFormat)(TRIDTV_INPUT_TYPE_T type);  //	Input output StereFormat Selection
	DTV_STATUS_T (*pfnSetInputFrameRate)(TRIDTV_INPUT_FREQ_T type); // 60hz,120hz,240hz

	//Output configuration
	DTV_STATUS_T (*pfnToggleOutputLRFrame)(TRIDTV_OUTPUT_LR_SEQ_T type); // Formatter LR Frame toggle
	DTV_STATUS_T (*pfnSelectOutputFormat)(TRIDTV_OUTPUT_TYPE_T type); // Input output StereFormat Selection
	DTV_STATUS_T (*pfnSetOutputFrameRate)(TRIDTV_OUTPUT_FREQ_T type); // 60hz,120hz,240hz

	//Config.
	DTV_STATUS_T (*pfnGet3DCfgInfo)(TRIDTV_CFG_ITEMS_T n3DCfgItems, UINT8 *nCurCfgInfo);
	DTV_STATUS_T (*pfnSet3DCfgInfo)(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
	UINT8 (*pfnGetFMTVersion)(void);//2010_03_23

	//3D Convergence Value Control
	DTV_STATUS_T (*pfnSetConvValue)(SINT8 u16ConvValue);
	DTV_STATUS_T (*pfnSetRFEmitterSync)(UINT8 u8EmitterSync);//(float fEmitterSync);
	DTV_STATUS_T (*pfnCtrlDepth)(UINT8 u8DepthLevel);

	//2Dto3D
	DTV_STATUS_T (*pfnSet2d3dInit)(void);
	DTV_STATUS_T (*pfnCtrl2d3dMode)(UINT8 u8Ctrl);
	DTV_STATUS_T (*pfnCtrl2d3dDepth)(UINT8 u8DepthLevel);
	DTV_STATUS_T (*pfnCtrl2d3dConv)(SINT8 u8ConvLevel);
	DTV_STATUS_T (*pfnCtrl2d3dDepthInSrc)(UINT8 u8SrcType, UINT8 u8DepthLevel);		//PDP LG8300 Only
	DTV_STATUS_T (*pfn2d3dToggleInputLRFrame)(TRIDTV_INPUT_LR_SEQ_T type); // 2D TO 3D LR Frame toggle
	DTV_STATUS_T (*pfn2d3dToggleInputFrameRate)(TRIDTV_INPUT_FREQ_T type); // 2D TO 3D Frame rate
	DTV_STATUS_T (*pfn2d3dSetFMTMode)(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);

	//PDP LG8300 Only
	DTV_STATUS_T (*pfnSetLVDS_RS)(UINT8 u8Ctrl);
	DTV_STATUS_T (*pfnGetVersionFromReg)(UINT8 *pu8Ver);
	DTV_STATUS_T (*pfnCheckI2CError)(UINT8 *pu8Ver);
	DTV_STATUS_T (*pfnSetStableVSyncCount)(UINT16 u16Cnt);
	DTV_STATUS_T (*pfnSetAPLCtrl)(UINT8 u8val);
	DTV_STATUS_T (*pfnHDMIVSIData)(TRIDTV_HDMI_VSI_INFO_T *pstHdmiVsiInfo);
	/* indirect i2c Acc */
	DTV_STATUS_T (*pfnWriteRegisterMask)(UINT8 u8Addr, UINT8 *pu8Val, UINT8 *pu8Mask);
	DTV_STATUS_T (*pfnWriteRegister)(UINT8 u8Addr, UINT16 u16Val);
	DTV_STATUS_T (*pfnReadRegister)(UINT8 u8Addr, UINT8 *pu8buf);
	/* direct i2c Acc */
	DTV_STATUS_T (*pfnDirectI2CWrite)(UINT16 u16Addr, UINT32 u32Val);
	DTV_STATUS_T (*pfnDirectI2CRead)(UINT16 u16Addr, UINT32 *pu32Val);
	DTV_STATUS_T (*pfnUpdateFirmWare)(UINT8 *puBuffer, ULONG dataSize);
	/* XD Plasma */
	DTV_STATUS_T (*pfnSet3DLRCorrection)(BOOLEAN onOff);
	DTV_STATUS_T (*pfnSetColorWash)(BOOLEAN onOff);

} FRC_TRIDFMT_DDI_CHIP;

/******************************************************************************
	함수 정의 (Function Definitions)
******************************************************************************/

//Initialization function.
DTV_STATUS_T DDI_FRC_TRIDFMT_Initialize(void); // Formatter Initialization
//Basic function.
DTV_STATUS_T DDI_FRC_TRIDFMT_SetScreenOnOff(BOOLEAN bOnOff); // Formatter Screen On Off
DTV_STATUS_T DDI_FRC_TRIDFMT_SetFMTMode(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
//Input configuration.
DTV_STATUS_T DDI_FRC_TRIDFMT_ToggleInputLRFrame(TRIDTV_INPUT_LR_SEQ_T type);
//Output configuration.

//Ect.
DTV_STATUS_T DDI_FRC_TRIDFMT_Get3DCfgInfo(TRIDTV_CFG_ITEMS_T n3DCfgItems, UINT8 *nCurCfgInfo);
DTV_STATUS_T DDI_FRC_TRIDFMT_Set3DCfgInfo(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
//3D Convergence Control.
DTV_STATUS_T DDI_FRC_TRIDFMT_SetConvCtrl(SINT8 n8ConvValue);
DTV_STATUS_T DDI_FRC_TRIDFMT_SetDepthCtrl(UINT8 n8DepthValue);

//RF Emitter Sync Control.
DTV_STATUS_T DDI_FRC_TRIDFMT_SetRFEmitterSync(UINT8 u8EmitterSync);//(float fEmitterSync)
DTV_STATUS_T DDI_FRC_TRIDFMT_GetVersionFromReg(UINT8 *u8Val);
//2Dto3D
DTV_STATUS_T DDI_FRC_2DTO3D_Initialize(void);
DTV_STATUS_T DDI_FRC_2DTO3D_SetDepthCtrl(UINT8 u8DepthLevel);
DTV_STATUS_T DDI_FRC_2DTO3D_SetConvCtrl(SINT8 u8ConvLevel);
DTV_STATUS_T DDI_FRC_2DTO3D_ToggleInputLRFrame(TRIDTV_INPUT_LR_SEQ_T type);
//PDP 3D Formatter Functions
DTV_STATUS_T DDI_FRC_TRIDFMT_UpdataFirmWare(UINT8 *pBuffer, ULONG dataSize);
DTV_STATUS_T DDI_FRC_TRIDFMT_Set3DLRCorrection(BOOLEAN onOff);
DTV_STATUS_T DDI_FRC_TRIDFMT_SetColorWash(BOOLEAN onOff);
DTV_STATUS_T DDI_FRC_TRIDFMT_WriteXDReg(UINT16 *puXDTbl);
DTV_STATUS_T DDI_FRC_TRIDFMT_SetXDPAdaptiveWB(UINT8 uAdaptiveWB);
DTV_STATUS_T DDI_FRC_TRIDFMT_SetXDPOnOff(BOOLEAN bOnOff);
DTV_STATUS_T DDI_FRC_TRIDFMT_SetSpreadSpectrum(UINT8 u8Domain, BOOLEAN bEnable, UINT16 u16Percent, UINT16 u16Period);
DTV_STATUS_T DDI_FRC_TRIDFMT_Set3DOverscanOnOff(BOOLEAN bEnable);
DTV_STATUS_T DDI_FRC_TRIDFMT_Get3DAutoFmtDetection(TRIDTV_INPUT_TYPE_T *p3DInputType);

//Internal 3D Formatter DDI
DTV_STATUS_T DDI_VIDEO_TRIDFMT_Initialize(void);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SetScreenOnOff(BOOLEAN bOnOff);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SetFMTMode(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_Set3DModeOnOff(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_ToggleInputLRFrame(TRIDTV_INPUT_LR_SEQ_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SelectInputFormat(TRIDTV_INPUT_TYPE_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SetInputFrameRate(TRIDTV_INPUT_FREQ_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_ToggleOutputLRFrame(TRIDTV_OUTPUT_LR_SEQ_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SelectOutputFormat(TRIDTV_OUTPUT_TYPE_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SetOutputFrameRate(TRIDTV_OUTPUT_FREQ_T type);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_Get3DCfgInfo(TRIDTV_CFG_ITEMS_T n3DCfgItems, UINT8 *nCurCfgInfo);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_Set3DCfgInfo(TRIDTV_INPUT_CONFIG_INFO_T *p3DModeInfo);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_SetConvValue(SINT8 u16ConvValue);
DTV_STATUS_T DDI_VIDEO_TRIDFMT_CtrlDepth(UINT8 u8DepthLevel);
// Internal 2D to 3D DDI
DTV_STATUS_T DDI_VIDEO_2DTO3D_Init(void);
DTV_STATUS_T DDI_VIDEO_2DTO3D_CtrlMode(UINT8 u8Ctrl);
DTV_STATUS_T DDI_VIDEO_2DTO3D_CtrlDepth(UINT8 u8DepthLevel);
DTV_STATUS_T DDI_VIDEO_2DTO3D_CtrlConv(SINT8 u8ConvLevel);
DTV_STATUS_T DDI_VIDEO_2DTO3D_CtrlDepthInSrc(UINT8 u8SrcType, UINT8 u8DepthLevel);
DTV_STATUS_T DDI_VIDEO_2DTO3D_ToggleInputLRFrame(TRIDTV_INPUT_LR_SEQ_T type);
DTV_STATUS_T DDI_VIDEO_2DTO3D_ToggleInputFrameRate(TRIDTV_INPUT_FREQ_T type);

//Debug function.
void DDI_FRC_TRIDFMT_Debug(void);

#endif /* _FRC_TRIDFMT_DDI_H_ */
