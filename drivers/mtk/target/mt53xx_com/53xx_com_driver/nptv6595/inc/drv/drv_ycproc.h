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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_ycproc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_YCPROC_H_
#define _DRV_YCPROC_H_

#include "typedef.h"
#include "x_typedef.h"

#define PREPROC_ENABLE_SYNC_REGEN   1
#define ADAPTIVE_SHARP_GAIN_ENABLE  0

typedef enum
{
	BASE_COLOR_M = 0,
	BASE_COLOR_R,
	BASE_COLOR_Y,
	BASE_COLOR_G,
	BASE_COLOR_C,
	BASE_COLOR_B,
	BASE_COLOR_MAX
} BASE_COLOR_INDEX_T;

enum
{
    ENUM_Y_SLOPE = 0,
    
    ENUM_S_GAIN1 = 1  ,
    ENUM_S_GAIN2 = 2  ,
    ENUM_S_GAIN3 = 3  ,
    ENUM_S_P1 = 4  ,
    ENUM_S_P2 = 5  ,

    ENUM_S_Y000 = 6  ,
    ENUM_S_Y064 = 7  ,
    ENUM_S_Y128 = 8  ,
    ENUM_S_Y192 = 9  ,
    ENUM_S_Y256 = 10  ,

    ENUM_H_FTN = 11  ,

    ENUM_H_Y000 = 12  ,
    ENUM_H_Y064 = 13  ,
    ENUM_H_Y128 = 14  ,
    ENUM_H_Y192 = 15  ,
    ENUM_H_Y256 = 16  ,
    
    ENUM_H_S000 = 17  ,
    ENUM_H_S032 = 18  ,
    ENUM_H_S064 = 19  ,
    ENUM_H_S096 = 20  ,
    ENUM_H_S128 = 21  ,

    ENUM_END = 22
};

typedef struct structAmbilightWindow {
    UINT16 wX_Start;
    UINT16 wX_End;
    UINT16 wY_Start;
    UINT16 wY_End;
    UINT8 bAttenuate;
} structAmbilightWindow;

typedef struct structChromaCoring{
	UINT8 fgTableEN;
	UINT8 fgPROT_MODE;
	UINT8 fgPROT_ACTION;
	UINT8 bMETHOD_SEL;
	UINT8 bYBound;
	UINT8 bC_CORING_RANGE;
	UINT8 bChromaCoringData[16];
	UINT8 fgC_SUPRESS_EN;
	UINT8 bSUPRESS_THR;
	UINT8 bSUPRESS_VALUE;
}structChromaCoring;

typedef struct BSParam{
    UINT8 bRGain;//7 Bit Gain
    UINT8 bGGain;//7 Bit Gain
    UINT8 bBGain;//7 Bit Gain
    UINT8 bLumaThr;//8 bit Thr
    UINT8 bChromaThr;//7 Bit Thr
}BSParam;

enum
{
    AmbilightRSel,
    AmbilightGSel,
    AmbilightBSel
};

#if PREPROC_ENABLE_SYNC_REGEN
typedef struct {
    UINT16 wHStart;
    UINT16 wHEnd;
    UINT16 wVStart;
    UINT16 wVEnd;
    UINT8 u1OnOff;
}ClipParam;
#endif

typedef struct {
    UINT16 wHStart;
    UINT16 wHEnd;
    UINT16 wVStart;
    UINT16 wVEnd;
    UINT8 u1OnOff;
}Region;

typedef struct {
    UINT8 u1AppType;
    UINT16 u2SrcWidth;
    UINT16 u2SrcHeight;
    UINT16 u2OutWidth;
    UINT16 u2OutHeight;
    UINT32 u4BitRate;
}MMAppInfo;


enum
{
    R_DITHER,
    E_DITHER,
    E_R_DITHER,
    LFSR_DITHER,
    ROUNDING,
    DITHER_OFF    
};

EXTERN UINT8 TDS_YLEV[65];

/*****************************************************************************************/
/**************************  General Register Access   ***********************************/
/*****************************************************************************************/
UINT8 bDrvCheckPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvSetPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvGetPqReg(UINT32* pu4Table, UINT32 u4Size);

/*****************************************************************************************/
/****************************************   Dither   *************************************/
/*****************************************************************************************/
void vDrvDitherInit(UINT8 bDitherMode);
/*****************************************************************************************/
/****************************************  General   ****************************************/
/*****************************************************************************************/
void vDrvYcprocInit(void);
void vDrvYcprocProc(void);
void vDrvGammaProc(void);
void vMMAPPorc(void);
void vDrvOPCProc(void);  
void vDrvDummyToRegister(void);

/*****************************************************************************************/
/*********************************   Anti-Aliasing Filter   **************************************/
/*****************************************************************************************/
void vDrvAntiAliasingFilterSet(UINT8 bPath, UINT8 bFilterIndex);
void vDrvAntiAliasingFilterOnOff(UINT8 bPath, UINT8 bOnOff);

/*****************************************************************************************/
/************************************   3x3 Matrix   *****************************************/
/*****************************************************************************************/
void vDrvColorTransform(UINT8 bPath);
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable);
void vLoadOSColorMatrix(UINT8 bPath, const INT32 * pwTable);
void vDrvYCCtoRGBExtLvl(UINT8 bPath, UINT16 wGain, UINT16 wOffset);
void vDrvMatrixCtrlSel(UINT8 bPath, UINT8 bMode);
void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff);
void vSetForceLoadMatrix(UINT32 u4OnOff);
UINT32 u4GetForceLoadMatrix(void);
void vDrvSetPedOnOff(UINT32 u4OnOff);
UINT32 u4DrvGetPedOnOff(void);

/*****************************************************************************************/
/**************************************   CTI   *********************************************/
/*****************************************************************************************/
void vDrvCTIFOnOff(UINT8 bOnOff);
void vDrvCTIFInit(void);
void vDrvCTIFParam(void);
void vDrvCTIROnOff(UINT8 bOnOff);
void vDrvCTIRInit(void);
void vDrvCTIRParam(void);

/*****************************************************************************************/
/*************************************   CBE   ******************************************/
/*****************************************************************************************/
#ifdef CC_MT5395
void vDrvCBEOnOff(UINT8 bOnOff);
void vDrvCBEInit(void);
void vDrvCBELoadParam(void);
#endif

/*****************************************************************************************/
/***************************************   LTI   ********************************************/
/*****************************************************************************************/
void vDrvLTIOnOff(UINT8 bOnOff);
void vDrvLTIInit(void);
void vDrvLTIParam(void);
void vDrvLTIParamHorizontal(void);
void vDrvLTIParamVertical(void);

/*****************************************************************************************/
/*********************************  HSHARP FRONT *****************************************/
/*****************************************************************************************/
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvPreSharpInit(void);
void vDrvPreSharpParam(UINT8 bPath);

/*****************************************************************************************/
/************************************  2DSHARP *******************************************/
/*****************************************************************************************/
void vDrvPostSharpOnOff(UINT8 bOnOff);
void vDrvPostSharpInit(void);
void vDrvPostSharpParam(void);
void vDrvPostSharpParamHorizontal(void);
void vDrvPostSharpParamVertical(void);
void vDrvLoadLocalTable(void);
BOOL bDrvReadLocalTable(void);
void vDrvLoadYLevTable(void);
BOOL bDrvReadYLevTable(void);
void vDrvCalYLevTable(void);
void vDrvCalYLevAdlTable(void);
void vDrvCalYLevAplTable(void);
void vDrvSetAdapYLevOnOff(UINT8 bOnOff);
void vDrvMainSharpPatGen(UINT8 bOnOff);

/*****************************************************************************************/
/*******************************   GLOBAL PQ ADJUSTMENT   **********************************/
/*****************************************************************************************/
void vDrvSetContrast(UINT8 bPath, UINT16 u2Value);
void vDrvSetBrightness(UINT8 bPath, UINT16 wValue);
void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value);
void vDrvSetHue(UINT8 bPath, UINT16 wValue);
void vDrvVideoColorMatrix(UINT8 bPath);
void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 wBriValue, UINT16 wContValue, UINT16 wSatValue, UINT16 wHueValue );

/*****************************************************************************************/
/***************************************   SCE   *******************************************/
/*****************************************************************************************/
void vDrvSetSCELevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvSetSkinLevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvSCECalculate(void);
void vDrvMainSCEOnOff(UINT8 bOnOff);
void vDrvPipSCEOnOff(UINT8 bOnOff);
void vDrvSCELoadTable(UINT8 bPath);
BOOL bDrvSCEVerifyTable(void);
void vDrvSCEInit(UINT8 bPath);
void vDrvSCEFindColor(void);
void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata);
void vDrvSCEAllOnOff(UINT8 bOnOff);
UINT32 vDrvSCELoadIdx(UINT32 u4FunIdx, UINT32 u4HueIdx, UINT32 u4Value);

/*****************************************************************************************/
/********************************   CONTENT METER   ****************************************/
/*****************************************************************************************/
void vDrvContMeterOnOff(UINT8 bOnOff);
void vDrvContMeterInit(void);
UINT8 vDrvContMeterIsHD(void);

/*****************************************************************************************/
/***********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvColorDemo(UINT8 bOnOff, Region bRegion);
void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion);
void vDrvECTIDemo(UINT8 bOnOff, Region rRegion);
void vDemoSetRegion(Region rDemoRegion);

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vDrvGammaOnOff(UINT8 bOnOff);
UINT8 bDrvGammaReadTable(UINT8 bMode);
void vDrvGammaSetDefaultTable(const UINT8 bTable, UINT8 * bGammaTable);
void vDrvGammaDecodeTable(void);
void vDrvGammaWriteTable(void);
BOOL bDrvGammaVerifyTable(void);
void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve);
void vDrvSetSWGammaFlg(UINT8 bFlag);
UINT8 bDrvGetSWGammaFlg(void);
void vDrvSetGammaRemap(UINT8 bForceLoadGamma);
void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB);
void vDrvGammaUnmute(void);
void vDrvSetOstgOptPos(UINT8 bOpt);

/*****************************************************************************************/
/************************************  AMBILIGHT  *****************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff);
void vDrvAmbilightSetWindow(UINT8 bWindowNum, const structAmbilightWindow *pAmbiWindow);

/*****************************************************************************************/
/**************************************  xvYCC  *******************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff);
void vDrvxvYCCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCC3x3OnOff(UINT8 bOnOff);
void vDrvxvYCCSetPath(UINT8 bPath);
void vDrvxvYCCSetGain(UINT16* u2RgbGain);
void vDrvxvYCCSetOffset1(UINT16* u2RgbOffset);
void vDrvxvYCCSetOffset2(UINT16* u2RgbOffset);
void vDrvxvYCCLoad3x3Matrix(UINT16 * pwTable);
void vDrvSetxvYCCPatGenOnOff(UINT8 bOnOff);
void vDrvSetxvYCCPatGenRGB(UINT16 wMuteR, UINT16 wMuteG, UINT16 wMuteB);
BOOL fgDrvGetxvYCCOnOff(void);
void vDrvxvYCCSetClip(UINT8 bOnOff);

/*****************************************************************************************/
/************************************   CHROMA CORING ************************************/
/*****************************************************************************************/
void vDrvChromaCoringInit(void);
void vDrvChromaCoringOnOff(UINT8 bPath, UINT8 fgIsOn);
void vDrvChromaCoringYbound(UINT8 bPath, UINT8 bYbound);
void vDrvSetChromaCoringTbl(UINT32 u4Path, const UINT8 * CbCoringTbl, const UINT8 * CrCoringTbl);

/*****************************************************************************************/
/************************************ BLUE STRETCH ** **************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bOnOff);
void vDrvSetBlueStretchParam(BSParam bsParam);
BOOL fgDrvGetBlueStretchOnOff(void);

/*****************************************************************************************/
/************************************ RGB GAIN Offset **************************************/
/*****************************************************************************************/
void vDrvSetRGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetGGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetBGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetROffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetGOffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetBOffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetROffset12(UINT8 bPath, UINT16 wValue);
void vDrvSetGOffset12(UINT8 bPath, UINT16 wValue);
void vDrvSetBOffset12(UINT8 bPath, UINT16 wValue);
UINT16 wDrvGetRGain1(UINT8 bPath);
UINT16 wDrvGetGGain1(UINT8 bPath);
UINT16 wDrvGetBGain1(UINT8 bPath);
UINT32 dwDrvGetROffset11(UINT8 bPath);
UINT32 dwDrvGetGOffset11(UINT8 bPath);
UINT32 dwDrvGetBOffset11(UINT8 bPath);
UINT32 dwDrvGetROffset12(UINT8 bPath);
UINT32 dwDrvGetGOffset12(UINT8 bPath);
UINT32 dwDrvGetBOffset12(UINT8 bPath);

/*****************************************************************************************/
/********************************** ANGULAR SCE MODEL ************************************/
/*****************************************************************************************/
void vDrvMainSCEAngularReset(void);
void vDrvMainSCEAngularSetHueVal(UINT8 bColor, UINT8 bHueOffset);
void vDrvMainSCEAngularSetSatVal(UINT8 bColor, UINT8 bSatGain);
void vDrvMainSCEAngularSetBriVal(UINT8 bColor, UINT8 bBriOffset);

/*****************************************************************************************/
/********************************** CHROMA BOOST ****************************************/
/*****************************************************************************************/
void vDrvChromaBoostParam(void);

/*****************************************************************************************/
/***************************************  Misc  *********************************************/
/*****************************************************************************************/
void vDrvMain422(UINT8 bOnOff);
void vDrvPip422(UINT8 bOnOff);
UINT8 bDrvGetPeUiDebugMode(void);
void vDrvSetPeUiDebugMode(UINT8 bOnOff);
void vDrvSetPipColorDomain(void);
void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern);
void vDrvPreprocSetFieldResync(UINT8 bPath);
#if PREPROC_ENABLE_SYNC_REGEN
//void vDrvPreprocSetSyncRegen(UINT8 bPath);
void vDrvRegenClip(UINT8 bPath);
void vDrvPreprocSetSyncRegen(UINT8 bPath,UINT16 wXOffset,UINT16 wYOffset,UINT16 wWidth,UINT16 wHeight);
#endif

void vDrvSceDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2YCbCrIn[3], UINT16 u2YCbCrOut[3]);
void vDrvOsDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2RGB[3]);

/*****************************************************************************************/
/********************************** CUSTOMIZE DRIVER *************************************/
/*****************************************************************************************/
UINT16 DRVCUST_Y2RMap(void);
UINT8 DRVCUST_ColorTransform(UINT8 bPath, UINT8 bColorSys, UINT8 bHdtv);
UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming);
void DRVCUST_MMAPProc(void);
void DRVCUST_SetPedestalCtrl(UINT8 bLevel);


#endif //_DRV_YCPROC_H_
