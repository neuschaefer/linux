/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/
/** @file panelpdp_ddi.h
 *
 *  Definition of PDP Module's drivers
 *
 *  @author   SangSun Lee(oxygenman@lge.com)
 *  @version  1.0
 *  @date	      2008.02.12
 *  @note
 *  @see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _PANELPDP_DDI_H_
#define _PANELPDP_DDI_H_


/*==========================================================
	User Type Definition
==========================================================*/

// [LGE jykim 20071128 : Fresh White]
/**
 * Fresh White
 */
typedef struct FRESH_WHITE_DATA
{
	UINT8	panelpdpDCTC1;
	UINT8	panelpdpDCTC2;
	UINT8	panelpdpDCTC3;
	UINT8	panelpdpDCTC4;
	UINT8	panelpdpDCTC5;

}	PANELPDP_FRESH_WHITE_DATA_T;

/**
 * GAMMA SET TABLE MODE.
 */
typedef enum
{
	PANELPDP_GAMMA_TABLE1	= 0,
	PANELPDP_GAMMA_TABLE2	= 1,
	PANELPDP_GAMMA_TABLE3	= 2,
	PANELPDP_GAMMA_TABLE4	= 3

}	PANELPDP_GAMMA_CTRL_T;

/**
 * ISM(화면 잔상 방지) MODE.
 */
typedef enum
{
	PANELPDP_ISM_OFF			= 0,
	PANELPDP_ISM_MOVE			= 1,
//	PANELPDP_ISM_FLIP			= 2,
	PANELPDP_ISM_COLORWASH		=2,
	PANELPDP_ISM_WHITEWASH	= 3

}	PANELPDP_ISM_T;

/**
 * PS MODE.
 */
typedef enum
{
	PANELPDP_PS_MODE0	= 0,
	PANELPDP_PS_MODE1	= 1,
	PANELPDP_PS_MODE2	= 2,
	PANELPDP_PS_MODE3	= 3,
	PANELPDP_PS_MODE4	= 4,
	PANELPDP_PS_MODE5	= 5,
	PANELPDP_PS_MODE6	= 6,
	PANELPDP_PS_MODE7	= 7,
	PANELPDP_PS_MODE8	= 8,
	PANELPDP_PS_MODE9	= 9,
	PANELPDP_PS_MODE10	= 10,
	PANELPDP_PS_MODE11	= 11,
	PANELPDP_PS_MODE12 = 12,
	PANELPDP_PS_MODE13 = 13,
	PANELPDP_PS_MODE14 = 14
}	PANELPDP_PS_MODE_T;

/**
 * XD PLASMA Version 정보.
 */
typedef enum {
	XD_PLASMA_NON,
	XD_PLASMA_V1,	
	XD_PLASMA_V2, 
	XD_PLASMA_V3,
	XD_PLASMA_V4,
}PANELPDP_XD_VERSION_T;


/**
 * PDP 100 Step WB Structure
 *
 * @see
 */
typedef struct PANELPDP_100STEPWB
{
	UINT8	uWBMinGain[3]; 			/** UI = -50(6000K)일 때의 WB Gain >>> 0:Red, 1:Green, 2:Blue	*/
	UINT8	uWBMaxGain[3]; 			/** UI = +50(14000K)일 때의 WB Gain >>> 0:Red, 1:Green, 2:Blue*/
	UINT8	uWBBypassGain[3]; 		/** WB Bypass 일 때의 WB Gain >>> 0:Red, 1:Green, 2:Blue		*/
	SINT8	nWBUiBypass;			/** WB Bypass 일 때의 색온도 적정 UI 값. */	
}PANELPDP_100STEPWB_T;

/**
 * Define  PDP Panel Motion Threshold.
 *
*/
typedef enum
{
	PANELPDP_MOTION_TH_2D			=0,
	PANELPDP_MOTION_TH_3D			=1,
	PANELPDP_MOTION_TH_CP			=2
} PANELPDP_MOTION_TH_T;

/*==========================================================
	Function Prototype Definition
==========================================================*/
extern	void 					DDI_PANELPDP_Init(void);
extern DTV_STATUS_T 			DDI_PANELPDP_I2C_Read(UINT8 subAddr, UINT8 *pTxData);
extern DTV_STATUS_T 			DDI_PANELPDP_I2C_Write(UINT8 subAddr, UINT8 txData);

extern void						DDI_PANELPDP_SetPowerSaving(PANELPDP_PS_MODE_T psMode, BOOLEAN bOnOff);
extern void						DDI_PANELPDP_SetPowerSave(PANELPDP_PS_MODE_T psMode);

extern DTV_STATUS_T				DDI_PANELPDP_SetAVorPCMode(UINT8 avPcMode, BOOLEAN bIsPc);
extern DTV_STATUS_T				DDI_PANELPDP_SetOrbiter( UINT8 scrollMode, UINT8 orbiterTime,
														UINT8 orbiterPixel, UINT8 orbiterStep );
extern DTV_STATUS_T				DDI_PANELPDP_SetGammaMode(PANELPDP_GAMMA_CTRL_T gammaMode);
extern DTV_STATUS_T				DDI_PANELPDP_SetISM(PANELPDP_ISM_T ismMode, UINT8 ismCtrl);

extern DTV_STATUS_T				DDI_PANELPDP_ReadASICName(UINT8 *pReadASICName,UINT8 mode);
extern DTV_STATUS_T				DDI_PANELPDP_ReadMODULEName(UINT8 *pReadMODULEName, UINT8 mode);
extern DTV_STATUS_T				DDI_PANELPDP_ReadMODULERomVer(UINT8 *pReadRomVer, UINT8 mode);
extern DTV_STATUS_T				DDI_PANELPDP_ReadMODULETemp( float *pReadCurrTemp);

extern BOOLEAN					DDI_PANELPDP_SetInvTime(UINT8 invTime);
extern void						DDI_PANELPDP_SetFreshWhiteData(PANELPDP_FRESH_WHITE_DATA_T freshWhiteReg);
extern void						DDI_PANELPDP_SetFreshWhite(UINT8 onOff);
extern void						DDI_PANELPDP_SetBright(UINT8 brMode);
extern void						DDI_PANELPDP_SetFMC(UINT8 bOnOff);
extern void						DDI_PANELPDP_SetDPS(UINT8 onOff);
extern void						DDI_PANELPDP_GetDPS(BOOLEAN *pData);
extern void						DDI_PANELPDP_SetPatternGen(BOOLEAN bOnOff);
extern BOOLEAN					DDI_PANELPDP_GetPatternGen(void);
extern void 				 	DDI_PANELPDP_InitXDPlasma(void);			//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
extern DTV_STATUS_T 	 	 	DDI_PANELPDP_WriteXDReg(UINT8 *pXDTbl);	//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
extern void 				 	DDI_PANELPDP_InitXDFreshWhiteVal(void);	//[vivakjh] 2009/03/10 PDP XD Fresh White 동작불 수정
extern PANELPDP_XD_VERSION_T	DDI_PANELPDP_GetXDPlasmaVersion(void);
extern void 					DDI_PANELPDP_SetPanelColorWash(UINT8 bOnOff);
extern DTV_STATUS_T 			DDI_PANELPDP_SetXDPAdaptiveWB(UINT8 uAdaptiveWB);
extern DTV_STATUS_T 			DDI_PANELPDP_SetXDPOnOff(BOOLEAN bOnOff);
extern DTV_STATUS_T 			DDI_PANELPDP_SetXDPSharpnessOnOff(BOOLEAN bOnOff);
extern DTV_STATUS_T 			DDI_PANELPDP_SetXDPColorEnhanceOnOff(BOOLEAN bOnOff);
extern DTV_STATUS_T 			DDI_PANELPDP_SetXDPWhiteBalanceOnOff(BOOLEAN bOnOff);
extern void 					DDI_PANELPDP_InitDataForPQ(void);
extern UINT8 					DDI_PANELPDP_GetAPSContrastGain(void);
extern void 					DDI_PANELPDP_SetAPSContrastGain(UINT8 uAPSContrast);
extern void 					DDI_PANELPDP_Get100StepWBParam(PANELPDP_100STEPWB_T *pstWBParam);
extern void 					DDI_PANELPDP_GetDefaultPSMode(UINT8 *puCinemaPSMode, UINT8 *puBrightPSMode);
extern UINT8 					DDI_PANELPDP_GetStartPSModeInAPS(void);
extern UINT8 					DDI_PANELPDP_GetStartPSStepInAPS(void);
extern UINT8 					DDI_PANELPDP_GetEndPSModeInAPS(void);
extern UINT8 					DDI_PANELPDP_GetStartPSModeInEyeQ(void);
extern UINT8 					DDI_PANELPDP_GetEndPSModeInEyeQ(void);
extern UINT8 					DDI_PANELPDP_GetMinPSMode(void);
extern UINT8 					DDI_PANELPDP_GetMedPSMode(void);
extern UINT8 					DDI_PANELPDP_GetMinimumPSRatio(void);
extern UINT8 					DDI_PANELPDP_GetEnergySavingUI(UINT8 uPsMode);
extern void						DDI_PANELPDP_SetTempControl(UINT8 onOff);
extern void						DDI_PANELPDP_Set3DMode(BOOLEAN bIs3D);
extern BOOLEAN 					DDI_PANELPDP_Is2DOnlyModule(void);

extern BOOLEAN					DDI_PANELPDP_IsEPA4dot1nEcoFlower(void);

extern void						DDI_PANELPDP_SetGPR(UINT8 onOff);
extern void						DDI_PANELPDP_SetALM(UINT8 onOff);
extern void						DDI_PANELPDP_SetModuleOSD(UINT8 uOSDMode);
extern void						DDI_PANELPDP_SetPSRatioControl(UINT8 uPSRatioPercent);
extern UINT8 					DDI_PANELPDP_GetAPSStartPSStepForUS(void);
extern DTV_STATUS_T				DDI_PANELPDP_SetMotionThreshold(PANELPDP_MOTION_TH_T uMotionTh);
extern PANELPDP_MOTION_TH_T		DDI_PANELPDP_GetMotionThreshold(void);

#if 0
extern BOOLEAN					DDI_PANELPDP_IsEcoFlower(void);
extern BOOLEAN					DDI_PANELPDP_IsNewConsumption(void);
#endif

#endif	/* end of _PANELPDP_DDI_H_ */

