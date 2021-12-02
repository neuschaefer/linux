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
 * $Author: caihua.lei $
 * $Date: 2012/07/24 $
 * $RCSfile: drv_ycproc.h,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_YCPROC_H_
#define _DRV_YCPROC_H_

#include "typedef.h"
#include "x_typedef.h"

#define EXTMJC_SW_TO_FRONT (IO32ReadFldAlign(PEUI_00, PEUI_EXTMJC_SWITCH_TO_FRONT) == 1)
#define GAMMA_IN_DRAM (IO32ReadFldAlign(PEUI_00, PEUI_INIT_GAMMA_TBL_IN_DRAM) == 1)
#define SET_GAMMA_IN_DRAM(u1OnOff) (vIO32WriteFldAlign(PEUI_00, (u1OnOff)?1:0, PEUI_INIT_GAMMA_TBL_IN_DRAM))
#define ISSCALER444(u1Path) ((u1Path) ? (IO32ReadFldAlign(MATRIX_00, SCALER444_PIP)) : (IO32ReadFldAlign(MATRIX_00, SCALER444_MAIN)))
#define SET_FORCE_LOAD_MATRIX(u1OnOff) (vIO32WriteFldAlign(MATRIX_00, u1OnOff, FORCE_LOAD_MATRIX))
#define GET_FORCE_LOAD_MATRIX (IO32ReadFldAlign(MATRIX_00, FORCE_LOAD_MATRIX))
#define SET_MATRIX_PED(u1OnOff) (vIO32WriteFldAlign(MATRIX_00, u1OnOff, MATRIX_PED_EN))
#define GET_MATRIX_PED (IO32ReadFldAlign(MATRIX_00, MATRIX_PED_EN))
#define GET_BLK_LVL(u1Path) ((u1Path) ? (IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_P)) : (IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_M)))
#define GET_MATRIX_HD_709 (IO32ReadFldAlign(MATRIX_00, MATRIX_HD_709))

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

    ENUM_S_GAIN1 = 1,
    ENUM_S_GAIN2 = 2,
    ENUM_S_GAIN3 = 3,
    ENUM_S_P1 = 4,
    ENUM_S_P2 = 5,

    ENUM_S_Y000 = 6,
    ENUM_S_Y064 = 7,
    ENUM_S_Y128 = 8,
    ENUM_S_Y192 = 9,
    ENUM_S_Y256 = 10,

    ENUM_H_FTN = 11,

    ENUM_H_Y000 = 12,
    ENUM_H_Y064 = 13,
    ENUM_H_Y128 = 14,
    ENUM_H_Y192 = 15,
    ENUM_H_Y256 = 16,

    ENUM_H_S000 = 17,
    ENUM_H_S032 = 18,
    ENUM_H_S064 = 19,
    ENUM_H_S096 = 20,
    ENUM_H_S128 = 21,

    ENUM_LCMP_M11M = 22,
    ENUM_LCMP_M11L = 23,
    ENUM_LCMP_M12M = 24,
    ENUM_LCMP_M12L = 25,
    ENUM_LCMP_M13M = 26,
    ENUM_LCMP_M13L = 27,
    ENUM_LCMP_M21M = 28,
    ENUM_LCMP_M21L = 29,
    ENUM_LCMP_M22M = 30,
    ENUM_LCMP_M22L = 31,
    ENUM_LCMP_M23M = 32,
    ENUM_LCMP_M23L = 33,

    ENUM_END = 34
};

typedef struct structAmbilightWindow {
    UINT16 wX_Start;
    UINT16 wX_End;
    UINT16 wY_Start;
    UINT16 wY_End;
    UINT8 bAttenuate;
} structAmbilightWindow;

enum
{
    AmbilightRSel,
    AmbilightGSel,
    AmbilightBSel
};

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

typedef struct {
    UINT8 u1OnOff;
    UINT16 u2SrcWidth;
    UINT16 u2SrcHeight;
    UINT8 u1X2Mode;    // 0x0:X1 mode; 0x1:X2 mode
    UINT8 u1SrcMode;    // 0x0:SBS; 0x1;FS
}sDQConfig;

typedef struct {
    UINT8 u1OnOff;
    UINT16 u2SrcWidth;
    UINT8 u1VI;
}sCBConfig;

typedef struct {
    UINT8 u1OnOff;
}sLIConfig;


enum
{
    R_DITHER,
    E_DITHER,
    E_R_DITHER,
    LFSR_DITHER,
    ROUNDING,
    DITHER_OFF    
};

typedef enum
{
    DUMP_ENG_NONE   = 0x00,
    DUMP_ENG_OD     = 0x01,
    DUMP_ENG_PSCAN  = 0x02,
    DUMP_ENG_MJC    = 0x04,
    DUMP_ENG_X      = 0x08,     // not use now
    DUMP_ENG_Y      = 0x10,     // not use now
    DUMP_ENG_Z      = 0x20,     // not use now
    DUMP_ENG_MAX
} DUMP_ENG_SEL_T;

typedef enum
{
	DUMP_STATE_INIT = 0,
	DUMP_STATE_START,
	DUMP_STATE_END,
} DUMP_STATE_T;

typedef enum
{
    DUMP_FMT_NONE            = 0x00,
    DUMP_FMT_RGB_444_10BIT   = 0x01,
    DUMP_FMT_Y_8BIT          = 0x02,
    DUMP_FMT_YCBCR_444_10BIT = 0x04,
    DUMP_FMT_MAX
} DUMP_FMT_T;


/*****************************************************************************************/
/**************************  General Register Access   ***********************************/
/*****************************************************************************************/
UINT8 bDrvCheckPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvSetPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvGetPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvDemoSetRegion(Region rDemoRegion);

/*****************************************************************************************/
/*************************************  General   ****************************************/
/*****************************************************************************************/
void vDrvYcprocInit(void);
void vDrvYcprocProc(void);
void vDrvMMAPPorc(void);
void vDrvDBLInit(void);
void vDrvOPCProc(void);  
void vDrvDBLSWInit(void);
void vDrvDBLHWInit(void);

/*****************************************************************************************/
/***************************   Anti-Aliasing Filter   ************************************/
/*****************************************************************************************/
void vDrvAntiAliasingFilterSet(UINT8 bPath, UINT8 bFilterIndex);
void vDrvAntiAliasingFilterOnOff(UINT8 bPath, UINT8 bOnOff);

/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
void vVdoSetColorTransform(UINT8 bPath);
void vForceColorSettingCheck(UINT8 bPath);
void vDrvColorTransform(UINT8 bPath);
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable);
void vLoadOSColorMatrix(UINT8 bPath, const INT32 * pwTable);
void vDrvMatrixCtrlSel(UINT8 bPath, UINT8 bMode);
void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvGlobalPQAdj(void);

/*****************************************************************************************/
/************************************   CTI   ********************************************/
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
#ifdef CC_MT5396
void vDrvCBEOnOff(UINT8 bOnOff);
void vDrvCBEInit(void);
void vDrvCBELoadParam(void);
#endif

/*****************************************************************************************/
/************************************   LTI   ********************************************/
/*****************************************************************************************/
void vDrvLTIOnOff(UINT8 bOnOff);
void vDrvLTIInit(void);
void vDrvLTIParam(void);

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
void vDrvLIPostSharpOnOff(UINT8 bOnOff);
void vDrvPostSharpHorizontalOnOff(UINT8 bOnOff);
void vDrvPostSharpVerticalOnOff(UINT8 bOnOff);
void vDrvPostSharpDiagonalOnOff(UINT8 bOnOff);
void vDrvPostSharpInit(void);
void vDrvPostSharpParam(void);
void vDrvPostSharpParamHorizontal(void);
void vDrvPostSharpParamVertical(void);
void vDrvPostSharpParamDiagonal(void);
void vDrvSetAdapYLevOnOff(UINT8 bOnOff);
void vDrvYlevSharpInit(void);
void vDrvLoadYLevTable(void);
BOOL bDrvReadYLevTable(void);
void vDrvCalYLevTable(void);
void vDrvCalYLevAdlTable(void);
void vDrvCalYLevAplTable(void);
void vDrvAdapYLev(void);
UINT8 vDrvMoSharpXLevelLUT(UINT8 LEVEL);
UINT8 vDrvMoSharpYLevelLUT(UINT8 LEVEL);
UINT8 vDrvGetMoSharpXLevel(void);
UINT8 vDrvGetMoSharpYLevel(void);
UINT16 vDrvGetMoSharpValue(UINT16 wInput, UINT16 wOutput, UINT8 LEVEL);
void vDrvSetMoSharpValue(void);
void vDrvMainSharpPatGen(UINT8 bOnOff);

/*****************************************************************************************/
/*****************************   GLOBAL PQ ADJUSTMENT   **********************************/
/*****************************************************************************************/
void vDrvSetContrast(UINT8 bPath, UINT16 u2Value);
void vDrvSetBrightness(UINT8 bPath, UINT16 wValue);
void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value);
void vDrvSetHue(UINT8 bPath, UINT16 wValue);
void vDrvVideoColorMatrix(UINT8 bPath);
void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 wBriValue, UINT16 wContValue, UINT16 wSatValue, UINT16 wHueValue);

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vDrvSCEInit(void);
void vDrvSCEParam(void);
void vDrvSCELoadTable(void);
void vDrvSCECalculate(void);
void vDrvSCEFindColor(void);
void vDrvSCEAngularReset(void);
void vDrvSCEAllOnOff(UINT8 bOnOff);
void vDrvSetSCELevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvSetSkinLevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvGetSCELevel(void);        //Add for vestel --- pingan 0606
void vDrvGet3DSCELevel(void);     //Add for vestel --- pingan 0606
void vDrvGetSkinLevel(void);        //Add for vestel --- pingan 0606
void vDrvGet3DSkinLevel(void);     //Add for vestel --- pingan 0606
void vDrvChromaBoostParam(void);//Add for vestel --- pingan 0610

    
void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata);
void vDrvSCEAngularSetVal(UINT8 bmodule, UINT8 bColor, UINT8 bValue);
UINT8 bDrvSCEVerifyTable(void);
UINT32 vDrvSCELoadIdx(UINT32 u4FunIdx, UINT32 u4HueIdx, UINT32 u4Value);

//Begin Add for Vestel ---pingan 0606
void vDrvSetSixColorRegFromQtyTable(void);
//Caihua Add 20120327
void vDrvSetTdtvSixColorRegFromQtyTable(void);
void vDrvSetTdtvSceRegFromQtyTable(void);
void vDrvSetTdtvSkinToneRegFromQtyTable(void);

//Caihua Add 20120407
void vDrvSetSceRegFromQtyTable(void);
void vDrvSetTdtvSceRegFromQtyTable(void);
void vDrvSetSkinToneRegFromQtyTable(void);
void vDrvSetTdtvSkinToneRegFromQtyTable(void);
//End Add for Vestel ---pingan 0606

/*****************************************************************************************/
/**************************************   DQ/CB   ****************************************/
/*****************************************************************************************/
void vDrvSetDQ(sDQConfig *sConfig);
void vDrvSetCB(sCBConfig *sConfig);
void vDrvSetLI(sLIConfig *sConfig);

/*****************************************************************************************/
/***********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvColorDemo(UINT8 bOnOff, Region bRegion);
void vDrvSetPostSharpDemoRange(Region rRegion);
void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion);
void vDrvECTIDemo(UINT8 bOnOff, Region rRegion);

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vDrvGammaOnOff(UINT8 bOnOff);
void vDrvGammaDecodeTable(void);
void vDrvGammaWriteTable(void);
void vDrvGammaReadTable(UINT16 u2GammaTable[3][257]);
UINT8 bDrvGammaVerifyTable(void);
void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB);
void vDrvGammaUnmute(void);
void  vDrvGammaKickPingPong(void);
void vDrvSetGammaWritePingPongFlag(BOOL bfgOn);
BOOL bDrvGetGammaPingPongFlag(void);
void vDrvGammaInterpolation(UINT32 u4Count, INT32* iCtlPos, INT32* iCtlValue, INT32* OutputLut);
void vDrvGammaProc(void);
/*****************************************************************************************/
/************************************  AMBILIGHT  ****************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff);
void vDrvAmbilightSetWindow(UINT8 bWindowNum, const structAmbilightWindow *pAmbiWindow);

/*****************************************************************************************/
/**********************************   AMBILIGHT V2 ***************************************/
/*****************************************************************************************/
void vDrvAmbiLitInit(void);
void vDrvAmbiLitSetMaskMode(UINT8 u1MaskMode);
void vDrvAmbiLitUpdateLine(UINT16 * u2AmbiLitArray, UINT8 * u1AmbilBlk, UINT8 u1Line);
UINT8 u1DrvAmbiLitGetStrapID(void);

/*****************************************************************************************/
/**************************************  xvYCC  *******************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff);
void vDrvxvYCCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCC3x3OnOff(UINT8 bOnOff);
void vDrvxvYCCSetPath(UINT8 bPath);
void vDrvxvYCCSetGain(UINT16 u2GainR, UINT16 u2GainG, UINT16 u2GainB);
void vDrvxvYCCSetOffset1(UINT16 u2Offset1R, UINT16 u2Offset1G, UINT16 u2Offset1B);
void vDrvxvYCCSetOffset2(UINT16 u2Offset2R, UINT16 u2Offset2G, UINT16 u2Offset2B);
void vDrvxvYCCLoad3x3Matrix(const UINT16 * pwTable);
UINT8 bDrvGetxvYCCOnOff(void);
void vDrvxvYCCSetClip(UINT8 bOnOff);

/*****************************************************************************************/
/************************************ BLUE STRETCH ** **************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvSetBlueStretchParam(void);
UINT8 bDrvGetBlueStretchOnOff(void);
void vDrvSetBlueStretchBase(void);

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

//Begin Add for Vestel ---pingan 0606
void vDrvSetRGain1FromQtyTable(void);
void vDrvSetGGain1FromQtyTable(void);
void vDrvSetBGain1FromQtyTable(void);
void vDrvSetTDTVRGain1FromQtyTable(void);
void vDrvSetTDTVGGain1FromQtyTable(void);
void vDrvSetTDTVBGain1FromQtyTable(void);
//End Add for Vestel ---pingan 0606
/*****************************************************************************************/
/********************************  Internal Driver *********************************************/
/*****************************************************************************************/
void vDrvSceDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2YCbCrIn[3], UINT16 u2YCbCrOut[3]);
void vDrvOsDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2RGB[3]);

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
void vDrvSetOstgOptPos(UINT8 bOpt);
void vDrvSetDefaultMatrix(UINT8 bPath, BOOL OnOff);

/*****************************************************************************************/
/********************************** CUSTOMIZE DRIVER *************************************/
/*****************************************************************************************/
UINT16 DRVCUST_Y2RMap(void);
UINT8 DRVCUST_ColorTransform(UINT8 bPath, UINT8 bColorSys, UINT8 bHdtv);
UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming);
void DRVCUST_MMAPProc(void);
void DRVCUST_SetPedestalCtrl(UINT8 bLevel);

void vDrvAmbilightISRInit(void);
void vDrvAmbiLPrintContent(UINT8 u1Color);
#endif //_DRV_YCPROC_H_
