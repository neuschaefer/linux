/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_display.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: jiewen.zhou $
 *
 * $Modtime: 04/03/23 7:08p $
 *
 * $Revision: #3 $
*************************************************************************/

#ifndef _DRV_DISPLAY_H_
#define _DRV_DISPLAY_H_

#include "typedef.h"
#include "feature.h"

enum eSyncLockMode {
    eSYNCLOCK_OFF,
    eSYNCLOCK_ON,
    eSYNCLOCK_IGNORED
};

enum eDisplayShiftMode {
    eDisplayShift_Line,
    eDisplayShift_Frame
};

enum eDispRFieldAlign_Source{
    eDispRFieldAlign_S_Field,
	eDispRFieldAlign_S_DispR
};

enum eDispRFieldAlign_Mode{
    eDispRFieldAlign_M_VSFall,
	eDispRFieldAlign_M_VSRise,
	eDispRFieldAlign_M_Delay,	
	eDispRFieldAlign_M_Bypass
};

enum e3DGlasses_Pin{
    e3DGlasses_P_3DR,
	e3DGlasses_P_3DL,
	e3DGlasses_P_3DBL
};

enum e3DGlasses_Source{
    e3DGlasses_S_2EYE,
	e3DGlasses_S_REYE,
	e3DGlasses_S_LEYE,
	e3DGlasses_S_BL,	
	e3DGlasses_S_DISPR,
	e3DGlasses_S_FIELD,
	e3DGlasses_S_IV_VS,
	e3DGlasses_S_IV_HS,
	e3DGlasses_S_BL2,		
};

enum e3DGlasses_Pos{
    e3DGlasses_POS_1stR,
	e3DGlasses_POS_2ndR
};

enum e3DGlasses_Pol{
    e3DGlasses_POl_Bypass,
	e3DGlasses_POl_Reverse
};

extern void vDrvDisplayInit(void) ;
extern void vDrvSetLCDTiming(void) ;
//extern UINT32 vDrvWithOutDramPixelRate(UINT16 DV_TOTAL) 	;
//extern UINT8 vDrvSetDynamicDisplay(UINT16 DV_TOTAL) ;
extern UINT8 bDrvDispSupport(UINT16 wOutputVTotal, UINT16 wOutputFrameRate);
extern UINT8 bDrvDispSupportFlag(UINT16 DV_TOTAL) ;
extern UINT8 bDrvDispSupportTiming(UINT16 wHeight, UINT16 wVTotal, UINT16 wFrameRate);
extern void vDrvSetDynamicDisplay(UINT8 BiMode) ;
extern void vDrvOutputStageInit(void);
extern void vDrvLCDSetFrame(UINT16 wFrameRate);
extern UINT8 vDrvGetLCDFreq(void) ; 
extern void vDrvLCDFreqSet(UINT8 bDCLKType, UINT8 u1FrameRate) ;
extern void vDrvTubeFreqSet(UINT8 bDCLKType) ;
extern void vDrvSetMTKGoodDclk(UINT16 wVERT_LINE, UINT8 bFrameRate) ;
//extern void vSetMTKGoodDclkNSTD(void) ; // for non-std signal
extern void vDrvSwitchMTKGoodDclk(UINT8 bOn_Off) ;
extern UINT8 bDrvCheckMTKGoodStatus(void) ;
extern UINT16 wDrvGetOutputHTotal(void) ;
extern UINT16 wDrvGetOutputVTotal(void) ;
extern void vDrvLVDSPowerOnOff(BOOL fgOnOff);
extern void vDrvLVDSSignalOnOff(BOOL fgOnOff);
extern void vDrvSetErrorLimit(UINT32 u4Error);
extern void vDrvChkVcounter(void);
extern void vDrvVsyncHFpAdj(void);
extern void vDrvSetAllMute(UINT8 bOnOff);

extern void vDrvVideoSetMuteOnOff(UINT8 ucVdpId, UINT8 fgEnable); //Caihua 20120227
extern void vDrvVideoSetMuteColor(UINT8 bPath, UINT32 u4Bg);

extern void vOstgNormProc(void);
extern void OSTG_OnOutputVSync(void);

extern void vDrvSwitchImportProtection(UINT8 bOn_Off);
extern void vDrvFireImportPortection(void);
#ifdef CC_FAST_INIT
extern void vDrvDisplayInit_pm_resume(void);
#endif

void vDrvTurnOnPanel(void);
void vDrvTurnOffPanel(void);

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_SW_FRAME_CHECK
extern void vDrvVDistanceCheck(void);
extern void vSetSWFrameTrackOnOff(UINT32 u4OnOff);
extern void vSetSWFrameTrackStep(UINT32 u4Step);
extern void vSetSWFrameTrackLockRange(UINT32 u4Upper, UINT32 u4Lower);
#endif
#ifdef SUPPORT_DDDS_STEP_TRACKING
void vDrvDDDSStepTrackingOnOff(UINT16 u2OnOff);
void vDrvUpdateDisplayCenterCW(UINT32 u4CW);
#endif
#ifdef CC_SCPOS_3DTV_SUPPORT 
UINT32 u4DrvGetDisp3DModeDclk(void);
UINT8 u1DrvGetDisp3DModeRefreshRate(void);
#endif

void vDDDSLCDSetting(UINT16 u2InVTotal, UINT8 bRefreshRate);
void vDddsSettingCheck(void);
#endif



//extern UINT16   wDISPLAY_WIDTH;
//extern UINT16   wDISPLAY_HEIGHT;
extern UINT8   bDISPLAY_INTERLACE_ENABLE;
//extern UINT32   dwSI_DISPLAY_DCLK_TYPICAL;
//extern UINT32   dwSI_DISPLAY_DCLK_50HZ;
extern UINT8   bSI_DISPLAY_DCLK_TYPE;


extern UINT8 u1SetFlipMirrorConfig(BOOL fgMirrorEn, BOOL fgFlipEn);
extern UINT8 u1GetFlipMirrorConfig(void);
extern UINT32 u4GetFlipMirrorModule(UINT32 u4VdpId);

//Flag for system flip/mirror config
#define SYS_FLIP_CONFIG_ON (1U << 0)
#define	SYS_MIRROR_CONFIG_ON (1U << 1)

#define CC_OSD_MUTE_WA    1

#if defined(CC_MT5396)
#define CC_WXGA_TTD_WA    0
#else
#define CC_WXGA_TTD_WA    1
extern UINT8 _u1IsTTDWxgaWA;
#endif


#define INTR_TWO_EYE_RISING     (1U << 0)
#define INTR_TWO_EYE_FALLING    (1U << 1)
#define INTR_LEFT_EYE_RISING    (1U << 2)
#define INTR_LEFT_EYE_FALLING   (1U << 3)
#define INTR_RIGHT_EYE_RISING   (1U << 4)
#define INTR_RIGHT_EYE_FALLING  (1U << 5)
#define INTR_OUTPUT_LINE        (1U << 6)

void vDrvSetTCONIrqClear(UINT32 u4INTRSource);
void vDrvGetTCONIrqStatus(UINT32 * u4INTRStatus);
extern void vDrvVsyncISRSetting(UINT16 u2Line, UINT16 u2Mask);
extern void vDrvVsyncISRStatus(void);


#if CC_OSD_MUTE_WA
extern UINT8 _bDddsDelayFlag;
#endif

extern UINT32 _u4NSDHTotal;
extern UINT32 _u4NSDVTotal;
extern UINT32 _u4NSDFrameRate;

extern BOOL _fgFRS_ENA;

UINT16 u2DrvGetInternalFrameRate(void);
void vDrvSetInternalFrameRate(UINT16 frameRate);
UINT8 bDrvUpdateInternalFrameRate(void);

UINT32 fgDrvSetMJCToOSTG(UINT32 fgOnOff);
UINT32 fgIsMJCToOSTG(void);
void Panel_SetDisplayShift(BOOL fgEnable, UINT8 u1ShiftMode,UINT8 u1L1DisplayShift, UINT8 u1R2DisplayShift);
void Panel_SetDisplayShiftColor(UINT8 u1ShiftColorB,UINT8 u1ShiftColorG, UINT8 u1ShiftColorR);
void Panel_DISPR_FIELD_Delay(UINT8 u1Tpye, UINT8 u1AlignType,UINT16 u1DelayLine);
void Panel_3DGlassesCTL(UINT8 u1Pin, UINT8 u1Tpye, BOOL fgPol,BOOL fgPos, UINT8 u1DelayFrame, UINT16 u2DelayLine);
void Panel_3DGlassesRSTCTL(UINT8 u1Tpye, BOOL fgOnOff, UINT16 u2RSTLine);
void Panel_3DGlassesDutyCTL(UINT8 u1Tpye, UINT8 u1DutyH);
void Panel_3DGlassesCTLEn(UINT8 u1Pin, BOOL fgonfff);

extern void vDrvVideoDitherInit(void);
extern UINT16 wDrvGetOutputHTotal(void);
extern void wDrvSetOutputHTotal(UINT16 u2HTotal);
extern UINT16 wDrvGetOutputVTotal(void);
extern void wDrvSetOutputVTotal(UINT16 u2VToal);
extern void wDrvSetOutputHActive(UINT16 u2HActive);
extern UINT16 wDrvGetOutputHActive(void);
extern void wDrvSetOutputVActive(UINT16 u2VActive);
extern UINT16 wDrvGetOutputVActive(void);
extern void vDrvSetHsyncFp(UINT16 u2Fp);
extern void vDrvSetHsyncBp(UINT16 u2Bp); 
extern UINT16 wDrvGetHsyncBp(void); 
extern UINT16 wDrvGetHsyncFp(void);
extern void vDrvSetVsyncFp(UINT16 u2VFp);
extern void vDrvSetVsyncBp(UINT16 u2VBp);
extern UINT16 wDrvGetVsyncFp(void);
extern UINT16 wDrvGetVsyncBp(void);
extern void vGetPostScalerStatus(void);
extern void vGetDISP_RStatus(void);
extern void u1SetDISP_REncodeEn(UINT8 u1Enable);
extern void vGetLVDS_HVEncodeStatus(void);
extern void u1SetLVDS_HVEncodeValue(UINT8 u1Enable);

extern void vDrvSetSyncLockMode(enum eSyncLockMode eMode);

extern void vDrvSetDDDSFrameTrackTarget(UINT8 u1SlowFast, UINT32 u4TargetVTotal) ;
extern void vDrvSetDDDSFrameTrackMode(UINT8 u1Mode) ;
extern void SetPanel(UINT32 i4panelid);
#endif
