/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_ttd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_TTD_H_
#define _DRV_TTD_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
//#include "hw_mjc.h"
//#include "drv_mjc.h"

#include "typedef.h"
#include "video_def.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#define SUPPORT_A1_DEMO 0

#define CONVERGENCE_DEPTH_TTD 128
#define CONVERGENCE_DEPTH_3D 206
#define DISPARITY_GAIN_TTD 63
#define DISPARITY_GAIN_3D 60
#define DEPTH_RANGE_RATIO_TTD 37
#define DEPTH_RANGE_RATIO_3D 15

#define OCCLUSION_GAIN_TTD 0
#define MAX_TTD_RST_COUNTER 3 
#define DIBR_DELAYOUT 250
#define DIBR_DELAYOUT_PR 245 // MT5368 + PR = 100, MT8283 + PR = 400
#define VP_HIST_DIST_TH 300
#define VP_LETTER_BOX_HIST_TH 200
#define VP_CID_INV_TH 70
#define STEREO_COMFORT_FACTOR 40 // Set this value < parallax (for children = 50 mm)


#if defined(CC_MT5396) || defined(CC_MT5389)
#define TTD_TRANS_STAT_MAX 0x20 
#endif
#if defined(CC_MT5368)
#define TTD_TRANS_STAT_MAX 0x8 
#endif

#define TTD_REGTBL_END 0xffffffff
/*-----------------------------------------------------------------------------------------------------------*
    SCL_2D_TO_PR : For General 2D->3D,  2D -> [SCL] -> VDS 1/2 + Line Repeat  -> [TTD] -> PR
    SCL_PR_TO_PR : For 3D Global Shift, 3D -> [SCL] -> PR -> [TTD] -> PR
    SCL_2D_TO_FS : For +MT8283 2D->3D,  2D -> [SCL] -> VDS 1/2 + Frame Repeat -> [TTD] -> FS 
    MJC_2D_TO_FS : For General 2D->3D,  2D -> [MJC] -> Frame Repeat  -> [TTD] -> FS
    MJC_FS_TO_FS : For 3D Global Shift, 3D -> [MJC] -> FS -> [TTD] -> FS
 *-----------------------------------------------------------------------------------------------------------*/
enum
{
    SCL_2D_TO_PR = 0,	// MT5389, MT5368MP, MT5368ES
    SCL_PR_TO_PR,		// MT5389, MT5368MP
    SCL_2D_TO_FS,		// MT5389, MT5368MP, MT5396ES, MT5396MP
    MJC_2D_TO_FS,		// MT5396ES, MT5396MP
    MJC_FS_TO_FS		// MT5396ES, MT5396MP
};

enum
{
    VER_5368_ES = 0,
    VER_5396_ES,
    VER_5389,
    VER_5368_MP,
    VER_5396_MP
};



typedef struct
{
    UINT8 ttdEnabled;
    UINT8 ttdModeChangeFlag;
    UINT8 ttdRstCounter;
    UINT16 ttdInputWidth;
    UINT16 ttdInputHeight;
    UINT8 ttdFwStatus;
    UINT8 ttdSceneMode;
    UINT16 ttdVpX;
    UINT16 ttdVpY;
    UINT8 ttdDgBias;    
    UINT8 ttdConvergenceDepth;
    UINT8 ttdGlobalWeight;
    UINT8 ttdLocalWeight;
    UINT8 ttdFgLocalInverse;
    UINT32 ttdPreviousImageAvg;
    UINT32 ttdCurrentImageAvg;
    UINT32 ttdSceneChangeThr;
    UINT16 ttdHist[32];
}TTD_INFO;

typedef struct
{
    UINT32  u2Addr;
    UINT32  u4Value;
    UINT32  u4Mask;
} TTD_REGTBL_T;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
EXTERN UINT8 g_u1ImageSafetyLevel; // 0: Off, 1: Low, 2: Middle, 3: High
EXTERN UINT8 g_u1DepthOfField; // 0~32
EXTERN UINT8 g_u1ProtrudeFromScreen; // 0~32
EXTERN UINT8 g_u1DistanceToTV; // 2~18
EXTERN UINT8 g_u1LREyeInverse; // 0, 1
EXTERN INT32 cur_convg;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void vDrvTTDInit(void);
void vDrvTTDSetMode(UINT8 bMode);
void vDrvTTDSet3DInput(void);
void vDrvTTDSet2DInput(void);
void vDrvTTDSetFGBGDetect(UINT8 u1OnOff);
void vDrvTTDSet3DDepthCtrl(UINT8 u1OnOff);
void vDrvTTDActGlobalShift(UINT8 u1OnOff);
void vDrvTTDSetEnable(UINT8 bEnable);
//UINT8 bDrvTTDIsModeChanged(void);
void vDrvTTDSetPicSize(UINT16 u2Width, UINT16 u2Height);
void vDrvTTDSetQualityISR(void);
void vDrvTTDProc(void);
void vDrvTTDSetFWOnOff(UINT8 bOnOff);
void vDrvTTDCheckRstCtrl(void);
void vDrvTTDSetStereoComfortRange(UINT8 u1Factor);
void vDrvTTDSetImageSafetyLevel(UINT8 u1Level);
void vDrvTTDSetDepthOfField(UINT8 u1Depth);
void vDrvTTDSetProtrudeFromScreen(UINT8 u1Protrude);
void vDrvTTDSetDistanceToTV(UINT8 u1Distance);
void vDrvTTDSetLREyeInverse(UINT8 u1OnOff);
INT32 i4DrvTTDDepthToGain(INT32 i4MaxGain, INT32 i4MinGain, INT32 i4Depth, INT32 i4Scale);
INT32 i4DrvTTDProtrudeToLRShift(INT32 i4Prdtrude, INT32 i4Scale);
INT32 i4DrvTTDProtrudeToConvDepth(INT32 i4Prdtrude, INT32 i4Scale);
INT32 i4DrvTTDCalculateSafetyScale(void);
void vDrvTTDUiValueUpdate(INT32 i4Scale);
void vDrvTTDSetUiProc(void);
void vDrvTTDCalculateDisparityGain(UINT8 u1Depth, UINT8 u1Distance);
void vDrvTTDSetStrength(UINT8 strength);
void vDrvTTDSetConvergenceDepthUI(UINT8 depth);
void vDrvTTDSetLRSyncSwitch(UINT8 bEnable);
void vDrvTTDSetLRDataSwitch(UINT8 bEnable);
void vDrvTTDSetV3GlobalShift(INT8 i1Shift);
void vDrvTTDSetNewGlobalShift(UINT8 u1Shift);
void vDrvTTDSetLvdsTxGlobalShift(INT8 i1Shift);
void vDrvTTDSetPrInfo(UINT8 u1OnOff);
UINT8 u1DrvTTDGetPrInfo(void);

// -- Image Safety Implementation Start --//
UINT8 u1DrvTTDGetSafetyDisparityGain(UINT16 u2PixSize, UINT8 u1UiDist, UINT8 u1UiGlobalDepth);
UINT8 u1DrvTTDGetSafetyLocalDepth(UINT16 u2PixSize, UINT8 u1UiDist, UINT8 u1UiGlobalDepth);
UINT32 u4DrvTTDGetSafetyDisparity(UINT16 u2PixSize, UINT8 u1Factor);
INT32 i4DrvDepthToDisparity(UINT8 u1Depth, UINT8 u1DsprtGain, UINT8 u1RangeRatio, UINT8 u1ConvDepth);
INT32 i4DrvTTDGetGlobalMaxDisparity(UINT8 u1UiGlobalDepth);
INT32 i4DrvTTDGetGlobalMinDisparity(UINT8 u1UiGlobalDepth);
UINT8 u1DrvTTDGetDisparityGainUpperBound(UINT32 u4SafetyDisparity, INT32 i4LDMax, INT32 i4LDMin, INT32 i4GDMax, INT32 i4GDMin);
UINT8 u1DrvTTDGetLocalDepthUpperBound(UINT32 u4SafetyDisparity, INT32 i4LDMax, INT32 i4LDMin, INT32 i4GDMax, INT32 i4GDMin);
// -- Image Safety Implementation End --//

void vDrvTTDLSVInit(void);
void vDrvTTDLSVProc(void);
void vDrvTTDSet3DDepthCtrl(UINT8 u1OnOff);
UINT8 u1DrvTTDGet3DDepthCtrl(void);
void vDrvTTDSetLocalWeight(UINT8 u1Value);
void vDrvTTDSetDepthBias(UINT8 u1Value);

 
#endif //#ifndef _DRV_TTD_H_
