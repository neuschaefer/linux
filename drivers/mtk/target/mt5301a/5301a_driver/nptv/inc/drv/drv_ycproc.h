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

#define NUM_CHROMA_HIST_BIN         8
#define NUM_RGB_MAX_HIST_BIN        16
#define LUMA_HIST_LEVEL             16
#define CHROMA_HIST_BOUNDARY_SIZE	14

#define Y2REXTLVL   1

#define PREPROC_ENABLE_SYNC_REGEN 1

#define ADAPTIVE_SHARP_GAIN_ENABLE 0//remove Hsharp adaptive for 8223 porting

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

typedef struct {
    UINT16 wHStart;
    UINT16 wHEnd;
    UINT16 wVStart;
    UINT16 wVEnd;
    UINT8 u1OnOff;
}Region;

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

extern UINT8 _bMAIN_COLOR_SYS;
extern UINT8 _bPIP_COLOR_SYS;

/*****************************************************************************************/
/*********************************  General   ****************************************/
/*****************************************************************************************/

//Ycproc Main Loop
void vDrvYcprocProc(void);

/*****************************************************************************************/
/*********************************   Anti-Aliasing Filter   ****************************************/
/*****************************************************************************************/
void vDrvAntiAliasingFilterSet(UINT8 bPath, UINT8 bFilterIndex);
void vDrvAntiAliasingFilterOnOff(UINT8 bPath, UINT8 bOnOff);

/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
void vDrvColorTransform(UINT8 bPath);
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable);
void vLoadOSColorMatrix(UINT8 bPath, INT32 * pwTable);
void vDrvYCCtoRGBExtLvl(UINT8 bPath, UINT16 wGain, UINT16 wOffset);
void vDrvMatrixCtrlSel(UINT8 bPath, UINT8 bMode);
void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff);

/*****************************************************************************************/
/*************************************   CTI   ******************************************/
/*****************************************************************************************/
//void vDrvCTIFOnOff(UINT8 bOnOff);//remove ECTIF for 8223 porting
//void vDrvCTIFInit(void);//remove EFCTI for 8223 proting
//void vDrvCTIFParam(void);//remove EFCTI for 8223 porting
void vDrvCTIROnOff(UINT8 bOnOff);
void vDrvCTIRInit(void);
void vDrvCTIRParam(void);
#if 0 //remove CDS for 8223 porting
/*****************************************************************************************/
/*************************************   CDS   ******************************************/
/*****************************************************************************************/
//void vDrvCDSOnOff(UINT8 bOnOff);//remove CDS for 8223 porting
void vDrvCDSInit(void);
void vDrvCDSParam(void);
#endif
/*****************************************************************************************/
/*************************************   LTI   ******************************************/
/*****************************************************************************************/
void vDrvLTIOnOff(UINT8 bOnOff);
void vDrvLTIInit(void);
void vDrvLTIParam(void);

/*****************************************************************************************/
/*********************************  HSHARP FRONT *****************************************/
/*****************************************************************************************/
#if 0 //remove Hsharp for 8223 porting
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvPreSharpInit(void);
void vDrvPreSharpParam(UINT8 bPath);
#endif
/*****************************************************************************************/
/************************************  2DSHARP *******************************************/
/*****************************************************************************************/
void vDrvPostSharpOnOff(UINT8 bOnOff);
void vDrvPostSharpInit(void);
void vDrvPostSharpParam(void);
void vDrvLoadLocalTable(void);
void vDrvReadLocalTable(void);
void vDrvLoadYLevTable(void);
void vDrvReadYLevTable(void);
void vDrvCalYLevTable(void);
void vDrvCalAdapYLevTable(void);
void vDrvSetAdapYLevOnOff(UINT8 bOnOff);
void vDrvMainSharpPatGen(UINT8 bOnOff);

#ifdef CC_MT5363
/*****************************************************************************************/
/*************************************   LCE   ******************************************/
/*****************************************************************************************/
void vDrvLCEOnOff(UINT8 bOnOff);
void vDrvLCEInit(void);
void vDrvAutoLCEOnOff(UINT8 bOnOff);
UINT16 vDrvLCEMapping(UINT8 bIn, UINT8 bInMax, UINT8 bInMin, UINT16 wOutMax, UINT16 wOutMin);
#endif

/*****************************************************************************************/
/***********************************   CONTRAST   ****************************************/
/*****************************************************************************************/
void vDrvMainContrast(UINT16 u2Value);

/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
void vDrvMainBrightness(UINT16 wValue);

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
void vDrvMainSaturation(UINT16 u2Value);

/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
void vDrvMainHue(UINT16 wValue);

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vDrvSetSCELevel(UINT8 bLevel,UINT8 bSceForceLoad);


void vDrvMainSCEOnOff(UINT8 bOnOff);
void vDrvPipSCEOnOff(UINT8 bOnOff);
void vDrvSCELoadTable(UINT8 bPath);
void vDrvSCEInit(UINT8 bPath);
//void vDrvSCEFindColor(void);//remove for 8223 proting
void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata);
void vDrvSCEAllOnOff(UINT8 bOnOff);

/*****************************************************************************************/
/********************************   CONTENT METER   ****************************************/
/*****************************************************************************************/
void vDrvContMeterOnOff(UINT8 bOnOff);
void vDrvContMeterInit(void);
UINT8 vDrvContMeterIsHD(void);

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvColorDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
//void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);//remove Hsharp for 8223 porting
void vDrv2DSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrvECTIDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
enum{
    E_GAMMA_DARK,
    E_GAMMA_MIDDLE,
    E_GAMMA_BRIGHT
};

void vDrvGammaOnOff(UINT8 bOnOff);
UINT8 bDrvGammaReadTable(UINT8 bMode, UINT8 SV_V_GAMMA_IDX);
void vDrvGammaSetDefaultTable(const UINT8 bTable, UINT8 * bGammaTable);
void vDrvGammaLoadTable(const UINT8 bTable);
void vDrvGammaSetTopPoint(UINT16 wR, UINT16 wG, UINT16 wB);
void vDrvGammaSLTTest(UINT8 bPattern);
void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve);
void vDrvSetSWGammaFlg(UINT8 bFlag);
UINT8 bDrvGetSWGammaFlg(void);

void vDrvSetGammaRemap(UINT8 bForceLoadGamma);

/*****************************************************************************************/
/************************************  AMBILIGHT  ****************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff);
void vDrvAmbilightSetWindow(UINT8 bWindowNum, const structAmbilightWindow *pAmbiWindow);


/*****************************************************************************************/
/*****************************  Saturation/Hue Histogram *********************************/
/*****************************************************************************************/
void vDrvChromaHistInit(void);

void vDrvSetChromaHistWindow(UINT16 * u2Window);

void vDrvGetChromaHist(UINT16 * u2aHist, BOOL bSupportNorm, UINT16 u2NormBase);
void vDrvSetChromaHistBoundary(UINT8 * bBoundary);

//void vDrvGetHueHist(UINT16 * u2aHist, BOOL bSupportNorm, UINT16 u2NormBase);//remove HUE Hist for 8223 porting
//void vDrvSetHueHistBoundary(UINT8 * bBoundary);//remove HUE Hist for 8223 porting

UINT32 u4RetRGBOfHuePercentage(UINT16 * u2aHist);
UINT8 u1RetASL(UINT8 * bBoundary, UINT16 * u2aHist, UINT16 u2NormBase);
UINT8 u1GetChannelGainFromASL(UINT8 u1ASL, UINT8 u1Color);
UINT8 u1GetWhiteDecayFromASL(UINT16 * u2aHist);

/*****************************************************************************************/
/************************************  RGB Max Histogram  *********************************/
/*****************************************************************************************/
void vDrvRGBMaxInit(void);
void vDrvSetRGBMaxBoundary(UINT8 * bBoundary);
void vDrvSetRGBMaxWindow(UINT16 * u2Boundary);
void vDrvGetRGBMax(UINT32 *dwHist);
void vDrvGetRGBMaxNormHist(UINT16 * dwHist);

/*****************************************************************************************/
/*************************************  Misc  ********************************************/
/*****************************************************************************************/
void vDrvYcprocInit(void);
void vDrvMain422(UINT8 bOnOff);
void vDrvPip422(UINT8 bOnOff);
void vDrvSetPipColorDomain(void);
void vDrvDummyToRegister(void);
void vDrvPreprocSetFieldResync(UINT8 bPath);
UINT8 bDrvGetPeUiDebugMode(void);
void vDrvSetPeUiDebugMode(UINT8 bOnOff);

#if PREPROC_ENABLE_SYNC_REGEN
//void vDrvPreprocSetSyncRegen(UINT8 bPath);
void vDrvRegenClip(UINT8 bPath);
void vDrvPreprocSetSyncRegen(UINT8 bPath,UINT16 wXOffset,UINT16 wYOffset,UINT16 wWidth,UINT16 wHeight);
#endif
/*****************************************************************************************/
/******************************* VGA Brightness,Contrast,Hue,Saturation Process*****************/
/*****************************************************************************************/
void vDrvVideoColorMatrix(UINT8 bPath);
void vDrvCalVideoColorMatrix(UINT8 bPath,UINT16 bBriValue,UINT16 bContValue,UINT16 bSatValue,UINT16 bHueValue );

/*****************************************************************************************/
/*************************************  xvYCC  *******************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff);
void vDrvxvYCCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCC3x3OnOff(UINT8 bOnOff);
void vDrvxvYCCSetPath(UINT8 bPath);
void vDrvxvYCCSetGainOffset(UINT16 u2Offset1, UINT16 u2Gain,UINT16 u2Offset2);
void vDrvxvYCCLoad3x3Matrix(const UINT16 * pwTable);
void vDrvSetxvYCCPatGenOnOff(UINT8 bOnOff);
void vDrvSetxvYCCPatGenRGB(UINT16 wMuteR, UINT16 wMuteG, UINT16 wMuteB);
BOOL fgDrvGetxvYCCOnOff(void);

/*****************************************************************************************/
/************************************   CHROMA CORING*************************************/
/*****************************************************************************************/
void vDrvChromaCoringInit(void);
void vDrvChromaCoringOnOff(UINT8 bPath, UINT8 fgIsOn);

/*****************************************************************************************/
/************************************BLUE STRETCH    *************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bOnOff);
void vDrvSetBlueStretchParam(BSParam bsParam);
BOOL fgDrvGetBlueStretchOnOff(void);

/*****************************************************************************************/
/************************************RGB GAIN Offset *************************************/
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
void vDrvMainSCEAngularSetHueVal(UINT8 bColor, UINT16 wHueOffset);
void vDrvMainSCEAngularSetSatVal(UINT8 bColor, UINT8 bSatGain);
void vDrvMainSCEAngularSetBriVal(UINT8 bColor, UINT16 wBriOffset);
UINT8 bDrvIsAngularControlPt(UINT8 bColor);
void vDrvMainSCEAngularIntp(UINT8 bType);
void vDrvSCEApplyAngularSatResult(UINT8 bPath);
void vDrvSCEApplyAngularHueResult(UINT8 bPath);
void vDrvSCEApplyAngularBriResult(UINT8 bPath);

void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern);

void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB);
void vDrvGammaUnmute(void);
/*****************************************************************************************/
/********************************** CHROMA BOOST *****************************************/
/*****************************************************************************************/
void vDrvChromaBoostOnOff(UINT8 bOnOff);
void vDrvChromaBoostGain(UINT8 bGain);
void vDrvChromaBoostRangeSel(UINT8 bRangeSel);
void vDrvChromaBoostMinusEn(UINT8 bMinusEn);
void vDrvChromaBoostParam(void);


/*****************************************************************************************/
/********************************** ADAPTIVE SHARPNESS***********************************/
/*****************************************************************************************/
#if ADAPTIVE_SHARP_GAIN_ENABLE
extern UINT8 bAPL;
void vDrvAdapShpTableInit(void);
UINT8 bDrvIsAdaptiveSharpGainEnabled(void);
void vDrvUpdateAdaptiveSharpGain(void);
void vDrvKeepHSharpGain(UINT8 bPath);
void vDrvSetAdaptiveSharpGain(UINT8 bPath);
UINT8 bDrvGetMultipliedAdpShpGain(UINT8 originalGain);
UINT16 vDrvGetAdapShpGainTablePoint(UINT8 index);
UINT8 vDrvGetAdapShpGainTableWeight(UINT8 index);
#endif

/*****************************************************************************************/
/********************************** CUSTOMIZE DRIVER*************************************/
/*****************************************************************************************/
UINT16 DRVCUST_Y2RMap(void);



#endif //_DRV_YCPROC_H_
