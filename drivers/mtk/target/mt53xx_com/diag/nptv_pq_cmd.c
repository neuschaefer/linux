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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_pq_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_mid.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_di.h"
#include "drv_tdc.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_ycproc.h"
#include "drv_contrast.h"
#include "drv_scpos.h"
#include "api_backlt.h"
#include "hal_ycproc.h"
#include "drv_pwm.h"
#include "u_drv_cust.h"
//#include "color_param.h"
#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

// for adaptive luma adjustment
static INT32 _i4AlGetLumaHist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetLumaInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetChromaHist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlSetChromaHistBound(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetRGBMAX(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlSetRGBMAXBound(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i4SetOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetALPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetBSPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetWSPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4GetALInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetDynBSPara(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i4GetYGamma(INT32 i4Argc, const CHAR **szArgv);


static INT32 _i4YcprocGammaMute(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGammaUnMute(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGammaCompenstion(INT32 i4Argc, const CHAR **szArgv);


// for adaptive backlight function.
static INT32 _i4AbEn(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbLog(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbSetLightSensor(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbSetLvl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbPreSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbSetDim(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbSetBoost(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AbSetLock(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocPC422En(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocHDMIColorSpace(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocContrast(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocBrightness(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocHue(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSaturation(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocAAF(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocCtrlSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocCoring(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSCETable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSCE(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocCTI(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGamma(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocOfst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSharpness(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocTDSReadYTbl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocTDSReadLCTbl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocChromaLineAvg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocOSMatrix(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5363
static INT32 _i4YcprocLCE(INT32 i4Argc, const CHAR **szArgv);
#endif
INT32 _i4YcprocSharpnessPatGen(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocSCEPatGen(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocContMeter(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPre(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPost(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocOS(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPQ(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocAngSCE(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocAngReset(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocPeUiDebug(INT32 i4Argc, const CHAR **szArgv);

#ifdef SUPPORT_FLASH_PQ
static INT32 _i4YcprocPqVerify(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocPqQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocPqver(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocPanelID(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSetSmartPic(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocQtyDft(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocQtyMinMax(INT32 i4Argc, const CHAR **szArgv);
#ifdef THE_3RD_PARTY_SW_SUPPORT
static INT32 _i4YcprocFS(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSceGain(INT32 i4Argc, const CHAR **szArgv);
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
#endif // #ifdef SUPPORT_FLASH_PQ

/* Functions for MJC */
static INT32 _i4MjcOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcYComp(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcFixSTA(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcMVDownScaleFRM(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcMVMCInk(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcMbistGo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4MjcCompStressTest(INT32 i4Argc, const CHAR **szArgv);


/* Function for Tdc */
static INT32 _TdcStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcLength(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcTop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcBottom(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcDramBit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcFrameNum(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcDramComp(INT32 i4Argc, const CHAR **szArgv);

/* Functions for NR */
static INT32 _NRHeight(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRMeter(INT32 i4Argc, const CHAR **szArgv);
static INT32 _3DNR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _2DNR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _3DNR_ManualWeight(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRDebugDataMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRHPSTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NROverscanTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRSetFreese(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRSet3DInkOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRGetNRStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NRSet2DLevel(INT32 i4Argc, const CHAR **szArgv);
INT32 _NRSetNRForceBypass(INT32 i4Argc, const CHAR **szArgv);

#if 0
/* Functions for Matrix_4 */
static INT32 _i4ContrastMatrix(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4BrightnessMatrix(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SaturationMatrix(INT32 i4Argc, const CHAR **szArgv);
#endif

CLIMOD_DEBUG_FUNCTIONS(NR)

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

CLI_EXEC_T arAdaptiveLuma[] = {
#ifdef CC_CLI
    {"Get Luma Hist",	"lhist",	_i4AlGetLumaHist,	NULL,	"Get luma histogram", CLI_GUEST},
    {"Get Luma Info",	"linfo",	_i4AlGetLumaInfo,	NULL,	"Get luma infomation", CLI_GUEST},
    {"Get Chroma Hist",	"chist",	_i4AlGetChromaHist,	NULL,	"Get chroma histogram", CLI_SUPERVISOR},
    {"Set Chroma Bound",	"chistb",	_i4AlSetChromaHistBound,	NULL,	"Set chroma histogram boundary and window range", CLI_SUPERVISOR},
    {"Get RGBMAX",	"rgbmax",	_i4AlGetRGBMAX,	NULL,	"Get RGBMAX", CLI_SUPERVISOR},
    {"Set RGBMAX",	"rgbmaxb",	_i4AlSetRGBMAXBound,	NULL,	"Set RGBMAX boundary and window range", CLI_SUPERVISOR},
    {"Set OnOff", "s", _i4SetOnOff, NULL, "Set ON OFF", CLI_GUEST},
    {"Set Adaptive Luma", "sal", _i4SetALPara, NULL, "Set Adaptive Luma Param", CLI_GUEST},
    {"Set BS", "sbs", _i4SetBSPara, NULL, "Set Black Stretch Param", CLI_GUEST},
    {"Set WS", "sws", _i4SetWSPara, NULL, "Set White Stretch Param", CLI_GUEST},
    {"Set WS", "sdbs", _i4SetDynBSPara, NULL, "Set Dynamic BS Param", CLI_SUPERVISOR},
    {"Get Info", "g", _i4GetALInfo, NULL, "Get Info", CLI_GUEST},
    {"Get YGamma", "gy", _i4GetYGamma, NULL, "Get YGamma", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_EXEC_T arAdaptiveBacklight[] = {
#ifdef CC_CLI
	{"Enable",	"en",	_i4AbEn,		NULL,	"Enable/disable", CLI_GUEST},
       {"LOG",  "log",   _i4AbLog,        NULL,   "Enable/disable LOG", CLI_GUEST},
       {"LightSensor",  "ls",   _i4AbSetLightSensor,        NULL,   "Set light sensor value", CLI_GUEST},
	{"Set level",	"lvl",	_i4AbSetLvl,	NULL,	"Set level", CLI_GUEST},
	{"Preset",	"pre",	_i4AbPreSet,	NULL,	"Preset curve", CLI_GUEST},
	{"Set Dimmer",	"dim",	_i4AbSetDim,	NULL,	"Set Dimmer", CLI_GUEST},
	{"Set Booster",	"boost",	_i4AbSetBoost,	NULL,	"Set Booster", CLI_GUEST},
	{"Set Lock",	"lock",	_i4AbSetLock,	NULL,	"Set Pwm lock", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#if 0
// Table for Matrix_4 command entry
CLI_EXEC_T arMatrixCmdTbl[] = {
#ifdef CC_CLI
  	{"Contrast 3x3", "contr", _i4ContrastMatrix, NULL, "Set Contrast using OS 3x3 Matrix", CLI_GUEST},
	{"Brightness 3x3", "brit", _i4BrightnessMatrix, NULL, "Set Brightness using OS 3x3 Matrix", CLI_GUEST},
	{"Saturation 3x3", "satr", _i4SaturationMatrix, NULL, "Set Saturation using OS 3x3 Matrix", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

// Table for YC processing command entry
CLI_EXEC_T arYcprocCmdTbl[] = {
#ifdef CC_CLI
	{"PC Video Timing", "PC422", _i4YcprocPC422En, NULL, "Set PC422 Mode", CLI_SUPERVISOR},
	{"HDMI Video Timing", "hdmiCS", _i4YcprocHDMIColorSpace, NULL, "Set HDMI Color Space Mode", CLI_SUPERVISOR},
	{"Contrast", "contr", _i4YcprocContrast, NULL, "Set Contrast", CLI_GUEST},
	{"Brightness", "brit", _i4YcprocBrightness, NULL, "Set Brightness", CLI_GUEST},
	{"Hue", "hue", _i4YcprocHue, NULL, "Set Hue", CLI_GUEST},
	{"Saturation", "satr", _i4YcprocSaturation, NULL, "Set Saturation", CLI_GUEST},
	{"AAF", "aaf", _i4YcprocAAF, NULL, "Enable/disable AAF", CLI_GUEST},
	{"ChromaCoring", "coring", _i4YcprocCoring, NULL, "Enable/disable Chroma Coring", CLI_GUEST},
	{"Control Sel", "ctrlsel", _i4YcprocCtrlSel, NULL, "3x3 Matrix Control Mode Sel", CLI_SUPERVISOR},
	{"SCE table", "scetbl", _i4YcprocSCETable, NULL, "Load SCE Table", CLI_SUPERVISOR},
	{"SCE", "sce", _i4YcprocSCE, NULL, "Enable/disable SCE", CLI_GUEST},
	{"SCE PatGen", "scepat", _i4YcprocSCEPatGen, NULL, "SCE Pattern Generator", CLI_SUPERVISOR},
	{"CTI", "cti", _i4YcprocCTI, NULL, "Enable/disable CTI", CLI_GUEST},
	{"Gamma", "gamma", _i4YcprocGamma, NULL, "Select gamma mode", CLI_SUPERVISOR},
	{"Gamam", "gmc", _i4YcprocGammaCompenstion, NULL, "Select gamma Compensation", CLI_SUPERVISOR},
	{"RGB Gain", "gain", _i4YcprocGain, NULL, "Select R/G/B gain", CLI_GUEST},
	{"RGB Offset", "ofst", _i4YcprocOfst, NULL, "Select R/G/B offset", CLI_GUEST},
	{"Sharpness", "sharp", _i4YcprocSharpness, NULL, "Enable/disable sharpness", CLI_GUEST},
	{"Sharp PatGen", "sharppat", _i4YcprocSharpnessPatGen, NULL, "Sharpness Pattern Generator", CLI_SUPERVISOR},
       {"Read TDS Y Tbl", "TDSytbl", _i4YcprocTDSReadYTbl, NULL, "Read TDSharp Y level table", CLI_SUPERVISOR},
       {"Read TDS LC Tbl", "TDSlctbl", _i4YcprocTDSReadLCTbl, NULL, "Read TDSharp Local table", CLI_SUPERVISOR},       
	{"ChromaLineAvg", "cla", _i4YcprocChromaLineAvg, NULL, "Chroma Line Average", CLI_GUEST},
#ifdef SUPPORT_FLASH_PQ
	{"pqverify", "pqv", _i4YcprocPqVerify, NULL, "PQ verify", CLI_SUPERVISOR},
	{"ver", "ver", _i4YcprocPqver, NULL, "PQ ver", CLI_SUPERVISOR},
	{"pqQuery", "pqq", _i4YcprocPqQuery, NULL, "PQ Query", CLI_SUPERVISOR},
	{"panelid", "pid", _i4YcprocPanelID, NULL, "Get Panel ID", CLI_SUPERVISOR},
	{"SmartPic", "sp", _i4YcprocSetSmartPic, NULL, "Set Qty Dft tbl", CLI_SUPERVISOR},
	{"qtydft", "qdft", _i4YcprocQtyDft, NULL, "Set Qty Dft tbl", CLI_SUPERVISOR},
	{"qty min max", "qmm", _i4YcprocQtyMinMax, NULL, "Set Qty Min/Max tbl", CLI_SUPERVISOR},
#ifdef THE_3RD_PARTY_SW_SUPPORT
	{"featureSel", "fs", _i4YcprocFS, NULL, "Feature Selection", CLI_SUPERVISOR},
	{"SceGain", "sceg", _i4YcprocSceGain, NULL, "SCE gain", CLI_SUPERVISOR},
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
#endif // #ifdef SUPPORT_FLASH_PQ
	{"Mute", "mute", _i4YcprocGammaMute, NULL, "Mute", CLI_SUPERVISOR},
	{"UnMute", "unmute", _i4YcprocGammaUnMute, NULL, "Mute", CLI_SUPERVISOR},
	{"AdaptiveLuma", "al",  NULL, arAdaptiveLuma, "AdaptiveLuma Command",  CLI_GUEST},
	{"AdaptiveBacklight", "ab",  NULL, arAdaptiveBacklight, "AdaptiveBacklight Command",  CLI_SUPERVISOR},
	{"OS Matrix", "osm",  _i4YcprocOSMatrix, NULL, "Enable/disable Output Stage Matrix",  CLI_SUPERVISOR},
	{"ContentMeter", "cm",  _i4YcprocContMeter, NULL, "Enable/disable Content Meter",  CLI_SUPERVISOR},
       {"PreProc", "pre", _i4YcprocPre, NULL, "Enable/Disable Pre-Proc", CLI_SUPERVISOR},
       {"PostProc", "post", _i4YcprocPost, NULL, "Enable/Disable Post-Proc", CLI_SUPERVISOR},
       {"OutputStage", "os", _i4YcprocOS, NULL, "Enable/Disable Output Stage", CLI_SUPERVISOR},
       {"AngularSCE", "asce", _i4YcprocAngSCE, NULL, "Angular SCE", CLI_SUPERVISOR},
       {"AngularReset", "arst", _i4YcprocAngReset, NULL, "Reset Angular SCE", CLI_SUPERVISOR},
       {"PeUiDebug", "ui", _i4YcprocPeUiDebug, NULL, "Enable/Disable PE UI Debug Message", CLI_GUEST},

#ifdef CC_MT5363
       {"LCE", "lce", _i4YcprocLCE, NULL, "Enable/Disable Auto LCE", CLI_SUPERVISOR},
#endif
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for TDC command entry
CLI_EXEC_T arTdcCmdTbl[] = {
#ifdef CC_CLI
    {"Active left",	    "left",     _TdcStart,	NULL,	"Active window left position",	CLI_GUEST},
    {"Active right",	"right",	_TdcLength,	NULL,	"Active window right position",	CLI_GUEST},
    {"Active top",	    "top",      _TdcTop,	NULL,	"Active window top position",	CLI_GUEST},
    {"Active bottom",	"bottom",	_TdcBottom,	NULL,	"Active window bottom position",CLI_GUEST},
    {"Dram bit",        "bit",	    _TdcDramBit,	NULL,	"TDC Bit",			            CLI_SUPERVISOR},
    {"Dram comp",       "comp",	    _TdcDramComp,	NULL,	"TDC Dram compress",			CLI_SUPERVISOR},
    {"Frame number",    "frame",	_TdcFrameNum,	NULL,	"TDC frame number",			CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for NR command entry
CLI_EXEC_T arNRCmdTbl[] = {
#ifdef CC_CLI
    {"3DNR",  "3dnr",	 _3DNR,  NULL,	 "3DNR OnOff", 		 CLI_SUPERVISOR},
    {"2DNR",  "2dnr",   _2DNR,  NULL,   "2DNR OnOff",		   CLI_SUPERVISOR},
    {"NR_Height",	"height",	_NRHeight,	NULL,	"NR active window height",			CLI_SUPERVISOR},
    {"NR_meter",	"meter",	_NRMeter,	NULL,	"Noise meter",			CLI_GUEST},
    {"3D Manual Weight", "3dmw", _3DNR_ManualWeight, NULL, "3D NR Manual Weight", CLI_SUPERVISOR},
    {"NR Data mode", "datamode", _NRDebugDataMode, NULL, "3D NR Data Mode", CLI_SUPERVISOR},
    {"NR HPS test", "hpstest", _NRHPSTest, NULL, "3D NR test in diff. HPS factor", CLI_SUPERVISOR},
    {"NR Overscan test", "ostest", _NROverscanTest, NULL, "3D NR test in diff. overscan ratio", CLI_SUPERVISOR},
    {"NR Freeze mode", "freeze", _NRSetFreese, NULL, "3D NR Freeze mode on/off", CLI_SUPERVISOR},
    {"NR 3D INK debug mode", "3dink", _NRSet3DInkOnOff, NULL, "3D INK debug mode on/off", CLI_SUPERVISOR},
    {"Query NR Status", "q", _NRGetNRStatus, NULL, "Query NR current status", CLI_SUPERVISOR},
    {"Set 2DNR Level", "2dlevel", _NRSet2DLevel, NULL, "Set 2D NR filter and blending level", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(NR),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for MJC command entry
CLI_EXEC_T arYcprocMjcTbl[] = {
#ifdef CC_CLI
    {"OnOff", "oo", _i4MjcOnOff, NULL, "Enable MJC", CLI_GUEST},
    {"YComp", "ycomp", _i4MjcYComp, NULL, "MJC Y Compression", CLI_SUPERVISOR},
    {"STA", "sta", _i4MjcFixSTA, NULL, "MJC enable Fix STA weighting", CLI_SUPERVISOR},
    {"MVDSFRM", "mvdsf", _i4MjcMVDownScaleFRM, NULL, "MJC downscale frame base", CLI_SUPERVISOR},
    {"INC", "ink", _i4MjcMVMCInk, NULL, "MJC INK", CLI_SUPERVISOR},
    {"MJC Comp Test", "mqpt", _i4MjcCompStressTest, NULL, "MJC Comp Stress Test", CLI_SUPERVISOR},
    {"MJC Mbist", "mbist", _i4MjcMbistGo, NULL, "MJC Mbist Proc", CLI_SUPERVISOR},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _TdcStart(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc ==1)
  {
    Printf("TDC start is: 0x%x\n", wDrvTDCGetStart());
  }
  else if(i4Argc == 2)
  {
  	vDrvTDCSetStart((UINT16)StrToInt(szArgv[1]));
	Printf("Set TDC start as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: start start_position\n");
  }

  return 0;
}

static INT32 _TdcLength(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc ==1)
  {
    Printf("TDC length is: 0x%x\n", wDrvTDCGetLength());
  }
  else if(i4Argc == 2)
  {
  	vDrvTDCSetLength((UINT16)StrToInt(szArgv[1]));
	Printf("Set TDC length as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: length length_width\n");
  }

  return 0;
}

static INT32 _TdcTop(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc ==1)
    {
        Printf("TDC top is: 0x%x\n", wDrvTDCGetTop());
    }
    else if(i4Argc == 2)
    {
        vDrvTDCSetTop((UINT16)StrToInt(szArgv[1]));
        Printf("Set TDC top as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
    }
    else
    {
        Printf("Usage: top top_position\n");
    }

    return 0;
}

static INT32 _TdcBottom(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc ==1)
    {
        Printf("TDC bottom is: 0x%x\n", wDrvTDCGetBottom());
    }
    else if(i4Argc == 2)
    {
        vDrvTDCSetBottom((UINT16)StrToInt(szArgv[1]));
        Printf("Set TDC bottom as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
    }
    else
    {
        Printf("Usage: bottom_width\n");
    }

    return 0;
}

static INT32 _TdcDramBit(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc == 2)
    {
        if ((UINT16)StrToInt(szArgv[1])== 10)
        {
            vDrvTDCSetDramMode(1);
            Printf("Set TDC dram bit as: 10\n");
            return 0;
        }
        else if ((UINT16)StrToInt(szArgv[1])== 9)
        {
            vDrvTDCSetDramMode(0);
            Printf("Set TDC dram bit as: 9\n");
            return 0;
        }
    }
  	Printf("Usage: %s <10/9>\n", szArgv[0]);

    return 1;
}

static INT32 _TdcFrameNum(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc == 2)
    {
        if ((UINT16)StrToInt(szArgv[1])== 3)
        {
            vDrvTDCSetDAMode(0); // 1w3r
            Printf("Set TDC access as 1w3r\n");
            return 0;
        }
        else if ((UINT16)StrToInt(szArgv[1])== 2)
        {
            vDrvTDCSetDAMode(1); // 1w2r
            Printf("Set TDC access as 1w2r\n");
            return 0;
        }
    }
  	Printf("Usage: %s <3/2>\n", szArgv[0]);

    return 1;
}

static INT32 _TdcDramComp(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc == 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
#if defined(CC_MT5365) || defined(CC_MT5395)
		Printf("Not Support\n");
#else        	
//            vDrvTDCSaveBW(1);
            Printf("Set TDC dram compress on\n");
#endif //#if defined(CC_MT5365) || defined(CC_MT5395)            
            return 0;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
#if defined(CC_MT5365) || defined(CC_MT5395)
		Printf("Not Support\n");
#else        	
//            vDrvTDCSaveBW(0);
            Printf("Set TDC dram compress off\n");
#endif //#if defined(CC_MT5365) || defined(CC_MT5395)                        
            return 0;
        }
    }
  	Printf("Usage: %s <on/off>\n", szArgv[0]);

    return 1;
}

static INT32 _NRGetNRStatus(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvNRGetNRStatus();
    return 1;
}

INT32 _NRSetNRForceBypass(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc !=2)
    {
        printf("Usage: n.byp.nr [0/1]\n");
        printf("1: Force NR bypass\n");
        printf("0: NR bypass determined by F/W\n");
    }
    else
    {
        UINT8 bOnOff;
        bOnOff = (UINT8)StrToInt(szArgv[1]);

        vDrvNRSetForceBypass(bOnOff);
    }
    return 1;
}

static INT32 _NRSet2DLevel(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc !=3 && i4Argc!=6)
    {
        printf("Usage1: n.nr.2dlevel [Filter level] [Blending level]\n");
        printf("Usage2: n.nr.2dlevel [Smooth area level] [Mess area level] [Edge area level] [Mosquito area level] [Blending level]\n");
        printf("[Filter level] 0~63\n");
        printf("[Blending level] 0~8 \n");
    }
    else if(i4Argc == 3)
    {
        UINT8 filterLevel;
        UINT8 blendLevel;

        filterLevel = (UINT8)StrToInt(szArgv[1]);
        blendLevel = (UINT8)StrToInt(szArgv[2]);

        if(filterLevel > 0x3f)
        {
            printf("[Filter level] 0~63\n");
            return 1;
        }

        if(blendLevel > 8)
        {
            printf("[Blending level] 0~8 \n");
            return 1;
        }

        vDrvNRSet2DLevel(filterLevel,filterLevel,filterLevel,filterLevel,blendLevel);
    }
    else if(i4Argc == 6)
    {
        UINT8 smoothLevel;
        UINT8 messLevel;
        UINT8 edgeLevel;
        UINT8 mosquitoLevel;
        UINT8 blendLevel;

        smoothLevel = (UINT8)StrToInt(szArgv[1]);
        messLevel = (UINT8)StrToInt(szArgv[2]);
        edgeLevel = (UINT8)StrToInt(szArgv[3]);
        mosquitoLevel = (UINT8)StrToInt(szArgv[4]);
        blendLevel = (UINT8)StrToInt(szArgv[5]);

        if(smoothLevel > 0x3f || messLevel > 0x3f || edgeLevel > 0x3f || mosquitoLevel > 0x3f)
        {
            printf("[Filter level] 0~63\n");
            return 1;
        }

        if(blendLevel > 8)
        {
            printf("[Blending level] 0~8 \n");
            return 1;
        }

        vDrvNRSet2DLevel(smoothLevel,messLevel,edgeLevel,mosquitoLevel,blendLevel);
    }
    return 1;
}

static INT32 _NRSet3DInkOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bOnOff;

    if(i4Argc !=2)
    {
        printf("Usage: n.nr.3dink [OnOff]\n");
        printf("[path] 0: 3D INK debug mode OFF\n");
        printf("[path] 1: 3D INK debug mode ON \n");
    }
    else
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        if(bOnOff > 1)
        {
            printf("Wrong arguments:\n");
            printf("[OnOff] 0~1\n");
            return 1;
        }

        vDrvNRSet3DINKDebugMode(bOnOff);
    }

    return 1;
}

static INT32 _NRSetFreese(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bOnOff;
    if(i4Argc !=2)
    {
        printf("Usage: n.nr.freeze [OnOff]\n");
        printf("[path] 0: Freeze OFF\n");
        printf("[path] 1: Freeze ON \n");
    }
    else
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        if(bOnOff > 1)
        {
            printf("Wrong arguments:\n");
            printf("[OnOff] 0~1\n");
            return 1;
        }

        vDrvNRSetFreeze(bOnOff);
    }

    return 1;
}

static INT32 _NRHPSTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT32 hpsFactor;
    if(i4Argc !=2)
    {
        printf("Usage: n.nr.hpstest [path]\n");
        printf("[path] 0: Main\n");
        printf("[path] 1: PIP \n");
    }
    else
    {
        bPath = (UINT8)StrToInt(szArgv[1]);

        if(bPath > 1)
        {
            printf("Wrong arguments:\n");
            printf("[path] 0~1\n");
            return 1;
        }

        for(hpsFactor = 0x8000;hpsFactor>0;hpsFactor-=0x400)
        {
            _VDP_SetHorizontalPreScaleDownFactor(bPath, hpsFactor);
            printf("Set hps to (0x%x)\n", hpsFactor);
            x_thread_delay(1000);
            vDrvNRSet3DNROnOff(SV_ON);
            x_thread_delay(1000);
            vDrvNRSet3DNROnOff(SV_OFF);
            x_thread_delay(1000);
        }

        _VDP_SetHorizontalPreScaleDownFactor(bPath, 0x0);
    }

    return 1;
}

static INT32 _NROverscanTest(INT32 i4Argc, const CHAR **szArgv)
{
	UCHAR ucVdpId;
	UINT16 overscan;
	VDP_OVERSCAN_REGION_T rSrcRegion;

	if (i4Argc < 2 || szArgv == NULL || szArgv[1] == NULL)
	{
		Printf("Arg: ucVdpId\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);

	//top
	rSrcRegion.u4Top = 0;
	rSrcRegion.u4Bottom = 0;
	rSrcRegion.u4Left = 0;
	rSrcRegion.u4Right = 0;

	for(overscan = 0; overscan < 0x1000; overscan+=0x100)
    {
    	rSrcRegion.u4Top = overscan;
    	VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK;
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_ON);
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_OFF);
        x_thread_delay(500);
    }

	//bottom
	rSrcRegion.u4Top = 0;
	rSrcRegion.u4Bottom = 0;
	rSrcRegion.u4Left = 0;
	rSrcRegion.u4Right = 0;

	for(overscan = 0; overscan < 0x1000; overscan+=0x100)
    {
    	rSrcRegion.u4Bottom= overscan;
    	VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK;
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_ON);
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_OFF);
        x_thread_delay(500);
    }
	//left
	rSrcRegion.u4Top = 0;
	rSrcRegion.u4Bottom = 0;
	rSrcRegion.u4Left = 0;
	rSrcRegion.u4Right = 0;

	for(overscan = 0; overscan < 0x1000; overscan+=0x100)
    {
    	rSrcRegion.u4Left= overscan;
    	VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK;
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_ON);
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_OFF);
        x_thread_delay(500);
    }
	//right
	rSrcRegion.u4Top = 0;
	rSrcRegion.u4Bottom = 0;
	rSrcRegion.u4Left = 0;
	rSrcRegion.u4Right = 0;

	for(overscan = 0; overscan < 0x1000; overscan+=0x100)
    {
    	rSrcRegion.u4Right= overscan;
    	VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK;
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_ON);
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_OFF);
        x_thread_delay(500);
    }
	//all
	rSrcRegion.u4Top = 0;
	rSrcRegion.u4Bottom = 0;
	rSrcRegion.u4Left = 0;
	rSrcRegion.u4Right = 0;

	//top
	for(overscan = 0; overscan < 0x1000; overscan+=0x100)
    {
    	rSrcRegion.u4Top = overscan;
		rSrcRegion.u4Bottom = overscan;
		rSrcRegion.u4Left = overscan;
		rSrcRegion.u4Right = overscan;
    	VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK;
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_ON);
        x_thread_delay(500);
        vDrvNRSet3DNROnOff(SV_OFF);
        x_thread_delay(500);
    }

	return 0;
}

static INT32 _NRDebugDataMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    E_NR_DMODE bDatamode;
    if(i4Argc !=3)
    {
        printf("Usage: n.nr.datamode [path] [datamode]\n");
        printf("[path] 0: Main\n");
        printf("[path] 1: PIP \n");
        printf("[path] 2: Disable NR datamode debug function\n");
        printf("[datamode] 0: NR_OFF\n");
        printf("[datamode] 1: NR_Y_ONLY\n");
        printf("[datamode] 2: NR_YC_420\n");
        printf("[datamode] 3: NR_YC_422\n");
        printf("[datamode] 4: NR_RGB\n");
        printf("[datamode] 5: NR_UNKNOWN\n");
    }
    else
    {
        bPath = (UINT8)StrToInt(szArgv[1]);
        bDatamode = (E_NR_DMODE)StrToInt(szArgv[2]);

        if(bPath > 2 || bDatamode > 5)
        {
            printf("Wrong arguments:\n");
            printf("[path] 0~2\n");
            printf("[datamode] 0~5\n");
            return 1;
        }

        if(bPath == 2)
        {
            printf("Disable NR datamode debug function\n");
            vDrvDISetNRDebugDataMode(SV_VP_MAIN,E_DI_NR_UNKNOWN);
            vDrvDISetNRDebugDataMode(SV_VP_PIP,E_DI_NR_UNKNOWN);
            vDrvNRSetDebugPath(SV_VP_NA);
        }
        else
        {
            printf("Set NR path to ");
            printf(bPath == 0?"[Main]":"[PIP]");
            printf(" and data mode to ");
            switch(bDatamode)
            {
                case E_DI_NR_OFF:
                    printf("NR_OFF");
                    break;
                case E_DI_NR_Y_ONLY:
                    printf("NR_Y_ONLY");
                    break;
                case E_DI_NR_YC_420:
                    printf("NR_YC_420");
                    break;
                case E_DI_NR_YC_422:
                    printf("NR_YC_422");
                    break;
                case E_DI_NR_RGB:
                    printf("NR_RGB");
                    break;
                case E_DI_NR_UNKNOWN:
                    printf("NR_UNKNOWN");
                    break;
                default:
                break;
            }

            printf("\n");
            vDrvDISetNRDebugDataMode(bPath,bDatamode);
            vDrvNRSetDebugPath(bPath);
        }
    }
    return 1;
}

static INT32 _3DNR_ManualWeight(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bOnOff = SV_OFF;
    UINT8 bWeight = 0;

    if(i4Argc == 1)
    {
        printf("n.nr.3dnrmw on/off weight\n");
    }
    else if(i4Argc ==2)
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        bWeight = 0;
    }
    else if(i4Argc == 3)
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        bWeight = (UINT8)StrToInt(szArgv[2]);
    }

    DrvNRSetManualWeight(bOnOff,bWeight);

    return 0;
}

INT32 _PreprocEnablePattern(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 3)
    {
        printf("Usage: n.ptn.pre [path] [pat num]\n");
        printf("[path] 0: main 1:sub\n");
        printf("[pat num]:\n");
        printf("0: source\n");
        printf("1: color bar\n");
        printf("2: cross pattern\n");
        printf("3: ramp\n");
        printf("4: AWGN signal\n");
        printf("5: motion cross pattern\n");
        printf("6: white level\n");
    }
    else
    {
        UINT8 bPath;
        UINT8 bPattern;
        bPath = (UINT8)StrToInt(szArgv[1]);
        bPattern = (UINT8)StrToInt(szArgv[2]);

        vDrvPreprocEnablePattern(bPath,bPattern);
    }
    return 1;
}

static INT32 _3DNR(INT32 i4Argc, const CHAR **szArgv)
{
/*
	UINT8 bMode;
	UINT8 b3DNR_En;
	bMode = bDrv3DNRGetMode();
	b3DNR_En = bDrv3DNRGet3DNRONOFF();

  if (i4Argc !=1)
  {
     if (b3DNR_En == 0)
     {
    	Printf("Y_NR is OFF, C_NR is OFF > \n");
     }
     else
     {
		if (bMode == 0)
		{
			Printf("Y_NR is ON, C_NR is OFF > \n");
		}
		else if (bMode == 1)
		{
			Printf("Y_NR is OFF, C_NR is ONF > \n");
		}
		else if(bMode == 2)
		{
			Printf("Y_NR is ON, C_NR is ON > \n");
		}
     }
    Printf("Usage: %s < OFF(0) / Y(1) / C(2) / Y+C(3) > \n");
  }
  else if(i4Argc == 2)
  {
  	vDrv3DNRCliOnOff((UINT8)StrToInt(szArgv[1]));
  }
*/
  return 0;
}

static INT32 _2DNR(INT32 i4Argc, const CHAR **szArgv)
{
/*
	if (i4Argc != 2)
	{
		switch(bDrv2DNRGetMode())
		{
		case 0:
			Printf("Set 2DNR OFF\n" );
			break;
		case 1:
			Printf("Set 2DNR 3x3 mode\n" );
			break;
		case 2:
			Printf("Set 2DNR 3x5 mode\n" );
			break;
		case 3:
			Printf("Set 2DNR 3x3x2 mode\n" );
			break;
		case 4:
			Printf("Set 2DNR 3x5x2 mode\n" );
			break;
		default:
			break;
		}
		Printf("Usage: %s < OFF(0) / 3x3(1) / 3x5(2) / 3x3x2(3) / 3x5x2(4) > \n", szArgv[0]);
		return 0;
	}

	switch(StrToInt(szArgv[1]))
	{
		case 0:
			Printf("Set 2DNR OFF\n" );
			break;
		case 1:
			Printf("Set 2DNR 3x3 mode\n" );
			break;
		case 2:
			Printf("Set 2DNR 3x5 mode\n" );
			break;
		case 3:
			Printf("Set 2DNR 3x3x2 mode\n" );
			break;
		case 4:
			Printf("Set 2DNR 3x5x2 mode\n" );
			break;
		default:
			Printf("Usage: %s < OFF(0) / 3x3(1) / 3x5(2) / 3x3x2(3) / 3x5x2(4) > \n", szArgv[0]);
			break;
	}
	vDrv2DNRCliOnOff((UINT8)StrToInt(szArgv[1]));
*/
  return 0;
}

static INT32 _NRHeight(INT32 i4Argc, const CHAR **szArgv)
{
/*
  if (i4Argc ==1)
  {
    Printf("NR height is: 0x%x\n", wDrvNRGetHeight());
  }
  else if(i4Argc == 2)
  {
  	vDrvNRSetHeight((UINT16)StrToInt(szArgv[1]));
	Printf("Set NR height as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: height length_width\n");
  }
*/
  return 0;
}

static INT32 _NRMeter(INT32 i4Argc, const CHAR **szArgv)
{

#if ANM_ENABLE
	if (i4Argc == 1)
	{
		Printf("meter 0: off\n");
		Printf("meter 1: measurement only\n");
		Printf("meter 2: measurement & apply\n");
	}
	else if (i4Argc == 2)
	{
		if(x_strcmp(szArgv[1], "0") == 0)
		{
		    NMEnable = FALSE;
		    _bNMChkState=NM_INIT;

            vDrvSetAutoNROnOff(SV_OFF);
		    bApiVideoProc(bDrvNRGetCurrentPath(), PE_ARG_3D_NR);
		}
		else if(x_strcmp(szArgv[1], "1") == 0)
		{
            vDrvSetAutoNROnOff(SV_OFF);
		    NMEnable = TRUE;

		}
		else if(x_strcmp(szArgv[1], "2") == 0)
		{
            vDrvSetAutoNROnOff(SV_ON);
		}
	}

#endif

  	return 0;
}


#if 0
////////////////////////////////////////////////////////////////////////////////
// Matrix_4 path control related functions
////////////////////////////////////////////////////////////////////////////////

static INT32 _i4ContrastMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 3)
	{
		Printf("Usage: %s <m/p> value\n", szArgv[0]);

		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vHalVideoContrastMatrix(u1Path , (UINT16)StrToInt(szArgv[2]));

	Printf("Set contrast to %d\n", (UINT16)StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4BrightnessMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 3)
	{
		Printf("Usage: %s <m/p> value\n", szArgv[0]);

		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vHalVideoBrightMatrix(u1Path , (UINT16)StrToInt(szArgv[2]));

	Printf("Set brightness to %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4SaturationMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 3)
	{
		Printf("Usage: %s <m/p> value\n", szArgv[0]);

		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vHalVideoSaturationMatrix(u1Path , (UINT8)StrToInt(szArgv[2]));

	Printf("Set saturation to %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}
#endif

static INT32 _i4SetOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 index, value;

    if (i4Argc == 3)
    {
		index = (UINT8)StrToInt(szArgv[1]);
		value = (UINT8)StrToInt(szArgv[2]);

		switch (index)
    	{
            case 0:
                vHalVideoAdaptiveLumaOnOff(SV_VP_MAIN, value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_BWS_ON_1_OFF_0, value);
                vHalVideoAdaptiveLumaSetDefault();
                break;
            case 2:
                wWriteQualityTable(QUALITY_BWS_BLACK_ON_1_OFF_0, value);
                vHalVideoAdaptiveLumaSetDefault();
                break;
            case 3:
                wWriteQualityTable(QUALITY_BWS_WHITE_ON_1_OFF_0, value);
                vHalVideoAdaptiveLumaSetDefault();
                break;
            case 4:
                vDrvAutoContrastFreeze(value);
                break;
            case 5:
                vDrvSetYGammaCoeff(value);
                break;
            default:
                break;
    	}
	}
    else
    {
        Printf("Usage : set <AL(0)/BWS(1)/BS(2)/WS(3)/Freeze(4)/YGamma(5)> <OFF(0)/ON(1)>\n" );
    }

    return 0;
}

static INT32 _i4GetYGamma(INT32 i4Argc, const CHAR **szArgv)
{
    EXTERN UINT8 vDrvGetYGammaCoeff(void);
    vDrvGetYGammaCoeff();
    return 0;
}



static INT32 _i4SetALPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 index;
	UINT16 value;

    if (i4Argc == 3)
    {
		index = (UINT8)StrToInt(szArgv[1]);
		value = (UINT16)StrToInt(szArgv[2]);

		switch (index)
        {
            case 0:
                wWriteQualityTable(QUALITY_AL_ON_1_OFF_0, value);
                Printf("QUALITY_AL_ON_1_OFF_0 = %d\n", value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_AL_GAIN, value);
                Printf("QUALITY_AL_GAIN = %d\n", value);
                break;
            case 2:
                wWriteQualityTable(QUALITY_AL_OFFSET, value);
                Printf("QUALITY_AL_OFFSET = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_AL_LIMIT, value);
                Printf("QUALITY_AL_LIMIT = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_AL_METHOD, value);
                Printf("QUALITY_AL_METHOD = %d\n", value);
                break;
            case 5:
                wWriteQualityTable(QUALITY_AL_SCENE_CHANGE_MAX_THD, value);
                Printf("QUALITY_AL_SCENE_CHANGE_MAX_THD = %d\n", value);
                break;
            case 6:
                wWriteQualityTable(QUALITY_AL_SCENE_CHANGE_TOTAL_THD, value);
                Printf("QUALITY_AL_SCENE_CHANGE_TOTAL_THD = %d\n", value);
                break;
            case 7:
                wWriteQualityTable(QUALITY_AL_NOISE_THRESHOLD, value);
                Printf("QUALITY_AL_NOISE_THRESHOLD = %d\n", value);
                break;
            default:
                break;
        }
		vHalVideoAdaptiveLumaSetDefault();
	}
    else
    {
        Printf("Usage : sal < OnOff(0)/Gain(1)/Offset(2)/Limit(3)/Method(4) > < value >\n" );
        Printf("Usage : sal < MaxDiff(5)/TotalDiff(6)/NoiseThd(7) > < value >\n" );
    }

    return 0;
}


static INT32 _i4SetBSPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 index;
	UINT16 value;

    if (i4Argc == 3)
    {
		index = (UINT8)StrToInt(szArgv[1]);
		value = (UINT16)StrToInt(szArgv[2]);

		switch (index)
    	{
            case 0:
                wWriteQualityTable(QUALITY_BWS_BLACK_LEVEL, value);
                Printf("QUALITY_BWS_BLACK_LEVEL = %d\n", value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_BWS_BLACK_GAIN, value);
                Printf("QUALITY_BWS_BLACK_GAIN = %d\n", value);
                break;
            case 2:
                wWriteQualityTable(QUALITY_BWS_BLACK_OFFSET, value);
                Printf("QUALITY_BWS_BLACK_OFFSET = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_BWS_BLACK_RATIO, value);
                Printf("QUALITY_BWS_BLACK_RATIO = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_BWS_BLACK_LIMIT, value);
                Printf("QUALITY_BWS_BLACK_LIMIT = %d\n", value);
                break;
            default:
                break;
    	}

		vHalVideoAdaptiveLumaSetDefault();
	}
    else
    {
        Printf("Usage : sbws < BLevel(0)/BGain(1)/BOffset(2)/BRatio(3)/BLimit(4) > < value >\n" );
    }

    return 0;
}

static INT32 _i4SetWSPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 index;
	UINT16 value;

    if (i4Argc == 3)
    {
		index = (UINT8)StrToInt(szArgv[1]);
		value = (UINT16)StrToInt(szArgv[2]);

		switch (index)
    	{
            case 0:
                wWriteQualityTable(QUALITY_BWS_WHITE_LEVEL, value);
                Printf("QUALITY_BWS_WHITE_LEVEL = %d\n", value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_BWS_WHITE_GAIN, value);
                Printf("QUALITY_BWS_WHITE_GAIN = %d\n", value);
                break;
            case 2:
                wWriteQualityTable(QUALITY_BWS_WHITE_OFFSET, value);
                Printf("QUALITY_BWS_WHITE_OFFSET = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_BWS_WHITE_RATIO, value);
                Printf("QUALITY_BWS_WHITE_RATIO = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_BWS_WHITE_LIMIT, value);
                Printf("QUALITY_BWS_WHITE_LIMIT = %d\n", value);
                break;
            default:
                break;
    	}

		vHalVideoAdaptiveLumaSetDefault();
	}
    else
    {
        Printf("Usage : sbws < WLevel(0)/WGain(1)/WOffset(2)/WRatio(3)/WLimit(4) > < value >\n" );
    }

    return 0;
}

static INT32 _i4SetDynBSPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 index;
	UINT16 value;

    if (i4Argc == 3)
    {
		index = (UINT8)StrToInt(szArgv[1]);
		value = (UINT16)StrToInt(szArgv[2]);

		switch (index)
        {
            case 0:
                wWriteQualityTable(QUALITY_DYNBS_ONOFF, value);
                Printf("QUALITY_DYNBS_ONOFF = %d\n", value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_DYNBS_RANGE, value);
                Printf("QUALITY_DYNBS_RANGE = %d\n", value);
                break;
            case 2:
                wWriteQualityTable(QUALITY_DYNBS_DBG, value);
                Printf("QUALITY_DYNBS_DBG = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_DYNBS_PXL_THD1, value);
                Printf("QUALITY_DYNBS_PXL_THD1 = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_DYNBS_PXL_THD2, value);
                Printf("QUALITY_DYNBS_PXL_THD2 = %d\n", value);
                break;
            case 5:
                wWriteQualityTable(QUALITY_DYNBS_DIST_THDH, value);
                Printf("QUALITY_DYNBS_DIST_THDH = %d\n", value);
                break;
            case 6:
                wWriteQualityTable(QUALITY_DYNBS_DIST_THDM, value);
                Printf("QUALITY_DYNBS_DIST_THDM = %d\n", value);
                break;
            case 7:
                wWriteQualityTable(QUALITY_DYNBS_DIST_THDL, value);
                Printf("QUALITY_DYNBS_DIST_THDL = %d\n", value);
                break;
            default:
                break;
        }
		vHalVideoAdaptiveLumaSetDefault();
	}
    else
    {
        Printf("Usage : sdbs < OnOff(0)/Range(1)/Debug(2) > < value >\n" );
        Printf("Usage : sdbs < LvThd1(3)/LvThd2(4) > < value >\n" );
        Printf("Usage : sdbs < GainThdH(5)/GainThdM(6)/GainThdL(7) > < value >\n" );
    }

    return 0;
}

extern void vDrvGetAdaptiveInfo(UINT8 bindex);
static INT32 _i4GetALInfo(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 index;

    if (i4Argc == 2)
	{
		index = (UINT8)StrToInt(szArgv[1]);
		vDrvGetAdaptiveInfo(index);
	}
    else
    {
        Printf("Usage : g < LumaInfo(0)/Hist(1)/Target(2)/DynCurve(3)/LumaCurve(4)/General(5) >\n" );
    }

    return 0;
}


static INT32 _i4AlGetLumaHist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 wHist[32];
	UINT8 i;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <16/32>\n", szArgv[0]);

		return 0;
	}

	if ((UINT8)StrToInt(szArgv[1]) == 16)
	{
		bDrvGetNormHist(wHist);
		for (i=0; i<16; i++)
		{
			Printf("%d ", wHist[i]);
		}
	}
	else if ((UINT8)StrToInt(szArgv[1]) == 32)
	{
		bDrvGetNormHist32(wHist);
		for (i=0; i<32; i++)
		{
			Printf("%d ", wHist[i]);
		}
	}
	Printf("\n");

	return 0;
}

static INT32 _i4AlGetLumaInfo(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);

		return 0;
	}

	Printf("Input  Luma APL  %d    max  %d    min  %d \n",
	    bDrvGetAPL(), bDrvGetLumaMax(), bDrvGetLumaMin());
    Printf("Output Luma APL  %d    max  %d    min  %d \n",
	    bDrvGetOutputAPL(SV_VP_MAIN), bDrvGetOutputLumaMax(SV_VP_MAIN), bDrvGetOutputLumaMin(SV_VP_MAIN));

	return 0;
}


static INT32 _i4YcprocGammaMute(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 wGammaMuteR, wGammaMuteG, wGammaMuteB;

	if (i4Argc != 4)
	{
		Printf("Usage: %s <R> <G> <B>\n", szArgv[0]);

		return 1;
	}

	wGammaMuteR = (UINT16) StrToInt(szArgv[1]);
	wGammaMuteG = (UINT16) StrToInt(szArgv[2]);
	wGammaMuteB = (UINT16) StrToInt(szArgv[3]);

    vDrvGammaMute(wGammaMuteR, wGammaMuteG, wGammaMuteB);

	return 0;
}

static INT32 _i4YcprocGammaUnMute(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvGammaUnmute();

   	return 0;
}


static INT32 _i4AlGetChromaHist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u2aHist[8];
	UINT8 i;

	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);

		return 0;
	}

	vHalGetChromaHist(u2aHist);

	for (i=0; i<8; i++)
	{
		Printf("%d ", u2aHist[i]);
	}
	Printf("\n");

	return 0;
}

static INT32 _i4AlSetChromaHistBound(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bBoundary[7];
	UINT16 wWindow[4];
	UINT8 i;

	if (i4Argc != 12)
	{
		Printf("Usage: %s <L0 L1 L2 L3 L4 L5 L6 Win_x0 Win_y0 Win_x1 Win_y1>\n", szArgv[0]);
		return 0;
	}

	if  (i4Argc == 12)
	{
		for (i=0; i<7; i++)
		{
			bBoundary[i] = (UINT16)StrToInt(szArgv[i+1]);
		}

		for (i=0; i<4; i++)
		{
			wWindow[i] = (UINT16)StrToInt(szArgv[i+8]);
		}

		vDrvSetChromaHistBoundary(bBoundary);
    	vDrvSetChromaHistWindow(wWindow);
	}

	return 0;
}

static INT32 _i4AlGetRGBMAX(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 wHist[NUM_RGB_MAX_HIST_BIN];
	UINT8 i;

	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);

		return 0;
	}

	vHalGetRGBMax(wHist);

	for (i=0; i<NUM_RGB_MAX_HIST_BIN; i++)
	{
		Printf("%d ", wHist[i]);
	}
	Printf("\n");

	return 0;
}

static INT32 _i4AlSetRGBMAXBound(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bBoundary[8];
	UINT16 wWindow[4];
	UINT8 i;

	if (i4Argc != 9)
	{
		Printf("Usage: %s <L0 L1 L2 L3 Win_x0 Win_y0 Win_x1 Win_y1>\n", szArgv[0]);
		return 0;
	}

	if  (i4Argc == 9)
	{
		for (i=0; i<4; i++)
		{
			bBoundary[i] = (UINT16)StrToInt(szArgv[i+1]);
		}
    	vDrvSetRGBMaxBoundary(bBoundary);

		for (i=0; i<4; i++)
		{
			wWindow[i] = (UINT16)StrToInt(szArgv[i+5]);
		}
    	vDrvSetRGBMaxWindow(wWindow);
	}

	return 0;
}

static INT32 _i4AbEn(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bMode;

	if (i4Argc != 2)
	{
		Printf("Usage: Mode <0-3>\n", szArgv[0]);

		return 0;
	}

    bMode = (UINT8)StrToInt(szArgv[1]);

	vHalVideoSetAdaptiveBacklightMode(bMode);

	Printf("Set adaptive backlight to %d\n", bMode);
    return 0;
}

static INT32 _i4AbLog(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bOnOff;

	if (i4Argc != 2)
	{
		Printf("Usage: Enabel Log <ON(1)/OFF(0)>\n", szArgv[0]);

		return 0;
	}

    bOnOff = (UINT8)StrToInt(szArgv[1]);
    #ifndef CC_MT8223
//    vRegWriteFldAlign(DUMMY_03, bOnOff, DCR_LOG);//remove DCR for 8223 porting
    #endif
    Printf("Set Log %d\n", bOnOff);
    return 0;
}

static INT32 _i4AbSetLightSensor(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bValue;

    if (i4Argc != 2)
    {
        Printf("Usage: Set Light Sensor <value>\n", szArgv[0]);
        return 0;
    }

    bValue = (UINT8)StrToInt(szArgv[1]);
        #ifndef CC_MT8223
//    vRegWriteFldAlign(DUMMY_03, bValue, LIGHT_SENSOR);//remove LightSensor for 8223 porting
        #endif

    Printf("Set Light Sensor %d\n", bValue);
    return 0;
}

static INT32 _i4AbSetLvl(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <0/1/2>\n", szArgv[0]);

		return 0;
	}

	vHalVideoAdaptiveBackLightParam((UINT8)StrToInt(szArgv[1]));

	Printf("Set adaptive backlight level to %d\n", (UINT8)StrToInt(szArgv[1]));
    return 0;
}

static INT32 _i4AbPreSet(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 7)
	{
		Printf("Usage: %s y0 x1 y1 x2 y2 y3\n", szArgv[0]);

		return 0;
	}
/*
	vDrvBacklightPreSet(
		(UINT8)StrToInt(szArgv[1]),	 // y0
		(UINT8)StrToInt(szArgv[2]),	 // x1
		(UINT8)StrToInt(szArgv[3]),	 // y1
		(UINT8)StrToInt(szArgv[4]),	 // x2
		(UINT8)StrToInt(szArgv[5]),	 // y2
		(UINT8)StrToInt(szArgv[6]),	 // x3
		(UINT8)StrToInt(szArgv[7]),	 // y3
		(UINT8)StrToInt(szArgv[8]),	 // x4
		(UINT8)StrToInt(szArgv[9]),	 // y4
		(UINT8)StrToInt(szArgv[10])); // y5

	Printf("Preset adaptive backlight curve as %d %d %d %d %d %d %d %d %d %d\n",
		(UINT8)StrToInt(szArgv[1]),	 // y0
		(UINT8)StrToInt(szArgv[2]),	 // x1
		(UINT8)StrToInt(szArgv[3]),	 // y1
		(UINT8)StrToInt(szArgv[4]),	 // x2
		(UINT8)StrToInt(szArgv[5]),	 // y2
		(UINT8)StrToInt(szArgv[6]),	 // x3
		(UINT8)StrToInt(szArgv[7]),	 // y3
		(UINT8)StrToInt(szArgv[8]),	 // x4
		(UINT8)StrToInt(szArgv[9]),	 // y4
		(UINT8)StrToInt(szArgv[10])); // y5
*/
    return 0;
}

static INT32 _i4AbSetDim(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s <freq> <duty>", szArgv[0]);

		return 0;
	}

	vApiSetDimming((UINT16)StrToInt(szArgv[1]), (UINT16)StrToInt(szArgv[2]));

	Printf("freq=%d, duty=%d", (UINT16)StrToInt(szArgv[1]), (UINT16)StrToInt(szArgv[2]));
	return 0;
}

static INT32 _i4AbSetBoost(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <level>", szArgv[0]);

		return 0;
	}

	vApiSetBooster((UINT16)StrToInt(szArgv[1]));

	Printf("level=%d", (UINT16)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _i4AbSetLock(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s <PWM1:1/PWM2:2> <on:1/off:0>\n", szArgv[0]);

		return 0;
	}

	vDrvSetLock((UINT32)StrToInt(szArgv[1]), (UINT32)StrToInt(szArgv[2]));

	return 0;
}
extern void vSetVGA422En(UINT8 bOnOff);
static INT32 _i4YcprocPC422En(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 bParam;
    bParam = (UINT32)StrToInt(szArgv[1]);

	if (i4Argc != 2 && (bParam > 2))
	{
	    Printf("Usage: PC422 <PC444:0/PC422:1> \n");
		return 0;
	}

    switch(bParam)
    {
        case 0:
            vSetVGA422En(SV_FALSE);
            vVgaTriggerModeChange();
            Printf("Set PC444\n");
            break;
    	case 1:
    	    vSetVGA422En(SV_TRUE);
            vVgaTriggerModeChange();
     	    Printf("Set PC422\n");
    	    break;
    	default:
    	    break;
    }

	return 0;
}

extern UINT8 u1HdmiColorMode;
extern void vApiHdmiColorModeChg(UINT8 bPath);
static INT32 _i4YcprocHDMIColorSpace(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1NewHdmiColorMode;
    UINT32 bParam;

    bParam = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc != 2 && ((bParam > 3) || (bParam <1)))
	{
		Printf("Usage: hdmiCS <auto:1/graphic:2/video:3>\n");
		return 0;
	}

    switch(bParam)
    {
    	case 2: //VID_PLA_HDMI_MODE_GRAPHIC
    		u1NewHdmiColorMode = SV_HDMI_MODE_GRAPHIC ;
    	break;
    	case 3: //VID_PLA_HDMI_MODE_VIDEO
    		u1NewHdmiColorMode = SV_HDMI_MODE_VIDEO ;
    	break;
    	case 1: //VID_PLA_HDMI_MODE_AUTO
    	default:
    		u1NewHdmiColorMode = SV_HDMI_MODE_AUTO ;
    	break;
    }

	if(u1NewHdmiColorMode != u1HdmiColorMode)
	{
		u1HdmiColorMode = u1NewHdmiColorMode;
		vApiHdmiColorModeChg(SV_VP_MAIN);
	}

	return 0;
}
static INT32 _i4YcprocContrast(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT16 u2Contrast;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <m/p> value\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u2Contrast = (UINT16)StrToInt(szArgv[2]);
    vHalVideoContrast(u1Path , u2Contrast);

    Printf("Set contrast to %d\n", u2Contrast);
    return 0;
}

static INT32 _i4YcprocBrightness(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1Brightness;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <m/p> value\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1Brightness = (UINT8)StrToInt(szArgv[2]);
    vHalVideoBrightness(u1Path , u1Brightness);

    Printf("Set brightness to %d\n", u1Brightness);
    return 0;
}

static INT32 _i4YcprocHue(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT8 u1Hue;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <m/p> value\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u1Hue = (UINT8)StrToInt(szArgv[2]);
    vHalVideoHue(u1Path , u1Hue);

    Printf("Set hue to %d\n", u1Hue);
    return 0;
}

static INT32 _i4YcprocSaturation(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    UINT16 u2Saturation;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <m/p> value\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    u2Saturation = (UINT16)StrToInt(szArgv[2]);
    vHalVideoSaturation(u1Path , u2Saturation);

    Printf("Set saturation to %d\n", u2Saturation);
    return 0;
}

static INT32 _i4YcprocAAF(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <Main(m)/PIP(p)> <OFF(0)/ON(1)>\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvAntiAliasingFilterOnOff(u1Path , (UINT8)StrToInt(szArgv[2]));

    Printf("Set AAF %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4YcprocCoring(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if (i4Argc != 3)
    {
        Printf("Usage: %s <Main(m)/PIP(p)> <OFF(0)/ON(1)>\n", szArgv[0]);
        return 0;
    }

    u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
    vDrvChromaCoringOnOff(u1Path , (UINT8)StrToInt(szArgv[2]));

    Printf("Set Chroma Coring %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4YcprocCtrlSel(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 3)
	{
		Printf("Usage: %s <Main(m)/PIP(p)> <OFF(0)/VGA(1)/Mute(2)/Default(4)>\n", szArgv[0]);
		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vDrvMatrixCtrlSel(u1Path , (UINT8)StrToInt(szArgv[2]));

	Printf("Set Control Mode %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}


static INT32 _i4YcprocSCETable(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 6)
	{
		Printf("Usage: %s <m/p> table_number sce_luma_gain sce_sat_gain sce_hue_gain\n", szArgv[0]);

		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vHalVideoSCELoadTable(u1Path , (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]), (UINT8)StrToInt(szArgv[4]), (UINT8)StrToInt(szArgv[5]));

	Printf("Load SCE table number %d with luma_gain = %d sat_gain=%d, hue_gain=%d\n", (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]), (UINT8)StrToInt(szArgv[4]), (UINT8)StrToInt(szArgv[5]));
    return 0;
}

static INT32 _i4YcprocSCE(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1OnOff;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <on/off>\n", szArgv[0]);
		return 0;
	}

	u1OnOff = ((x_strcmp(szArgv[1], "on") == 0) ? SV_ON : SV_OFF);

	vDrvSCEAllOnOff(u1OnOff);

	if (u1OnOff == SV_ON)
	{
		Printf("Enable SCE\n");
	}
	else
	{
		Printf("Disable SCE\n");
	}
    return 0;
}

INT32 _i4YcprocSCEPatGen(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff = StrToInt(szArgv[1]);
    UINT16 u2Ydata = StrToInt(szArgv[2]);

    if (i4Argc != 3)
    {
        Printf("Usage: %s <ON/OFF(0/1) Ydata(0-1023)>\n", szArgv[0]);
        return 0;
    }
    vHalSetSCEPat(u1OnOff, u2Ydata);

    return 0;
}

static INT32 _i4YcprocCTI(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1OnOff;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <on/off>\n", szArgv[0]);
		return 0;
	}

	u1OnOff = ((x_strcmp(szArgv[1], "on") == 0) ? SV_ON : SV_OFF);

	vHalVideoCTIOnOff(SV_VP_MAIN , u1OnOff);

	if (u1OnOff == SV_ON)
	{
		Printf("Enable CTI\n");
	}
	else
	{
		Printf("Disable CTI\n");
	}
    return 0;
}

static INT32 _i4YcprocGamma(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <bright/middle/dark/off>\n", szArgv[0]);

		return 0;
	}

	if (x_strcmp(szArgv[1], "bright") == 0)
	{
		u1Mode = SV_V_GAMMA_BRIGHT;
		Printf("Use gamma table BRIGHT\n");
	}
	else if (x_strcmp(szArgv[1], "middle") == 0)
	{
		u1Mode = SV_V_GAMMA_MIDDLE;
		Printf("Use gamma table MIDDLE\n");
	}
	else if (x_strcmp(szArgv[1], "dark") == 0)
	{
		u1Mode = SV_V_GAMMA_DARK;
		Printf("Use gamma table DARK\n");
	}
	else
	{
		u1Mode = SV_V_GAMMA_OFF;
		Printf("Disable gamma\n");
	}

	vHalVideoGammaLoadTable(u1Mode);
    return 0;
}


INT32 _i4YcprocGammaCompenstion(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode=0;

    void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel);

	if (i4Argc != 2)
	{
		Printf("Usage: %s <0/1>  0)No Compensation 1)Compensate to 0.9", szArgv[0]);

		return 0;
	}

	if (x_strcmp(szArgv[1], "1") == 0)
	{
		u1Mode = 1;
		Printf("Compensate Gamma to 0.9\n");
    }else if(x_strcmp(szArgv[1], "2") == 0){
		u1Mode = 2;
		Printf("Compensate Gamma to 0.875\n");
    }else if(x_strcmp(szArgv[1], "3") == 0){
		u1Mode = 3;
		Printf("Compensate Gamma to 0.850\n");
    }else if(x_strcmp(szArgv[1], "4") == 0){
		u1Mode = 4;
		Printf("Compensate Gamma to .825\n");
	}else{
		Printf("No Compensation\n");
	}
	DRVCUST_PANEL_GAMMA_REMAP(u1Mode);
    return 0;
}
static INT32 _i4YcprocGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

    UNUSED(u1Mode);
	if (i4Argc != 3)
	{
		Printf("Usage: %s <r/g/b> value\n", szArgv[0]);

		return 0;
	}

	if (x_strcmp(szArgv[1], "r") == 0)
	{
		u1Mode = SV_RGB_R;
	}
	else if (x_strcmp(szArgv[1], "g") == 0)
	{
		u1Mode = SV_RGB_G;
	}
	else
	{
		u1Mode = SV_RGB_B;
	}

	vHalVideoRGBGain(SV_VP_MAIN, u1Mode, StrToInt(szArgv[2]));

	Printf("Set gain to %d\n", StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4YcprocOfst(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

    UNUSED(u1Mode);
	if (i4Argc != 3)
	{
		Printf("Usage: %s <r/g/b> value\n", szArgv[0]);

		return 0;
	}

	if (x_strcmp(szArgv[1], "r") == 0)
	{
		u1Mode = SV_RGB_R;
	}
	else if (x_strcmp(szArgv[1], "g") == 0)
	{
		u1Mode = SV_RGB_G;
	}
	else
	{
		u1Mode = SV_RGB_B;
	}

	vHalVideoRGBOffset(SV_VP_MAIN, u1Mode, StrToInt(szArgv[2]));

	Printf("Set offset to %d\n", StrToInt(szArgv[2]));
    return 0;
}


static INT32 _i4YcprocSharpness(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1OnOff;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <on/off>\n", szArgv[0]);
		return 0;
	}

	u1OnOff = ((x_strcmp(szArgv[1], "on") == 0) ? SV_ON : SV_OFF);
	vHalVideoSharpnessOnOff(SV_VP_MAIN , u1OnOff);

	if (u1OnOff == SV_ON)
	{
		Printf("Enable Sharpness\n");
	}
	else
	{
		Printf("Disable Sharpness\n");
	}
    return 0;
}

INT32 _i4YcprocSharpnessPatGen(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode = StrToInt(szArgv[1]);

    if (i4Argc != 2)
    {
        Printf("Usage: %s <OFF(0)/ON(1)>\n", szArgv[0]);
        return 0;
    }

    if (u1Mode <= 1)
    {
        if(u1Mode != 0)
        {
            Printf("Enable Sharpness Pat Gen \n");
        }
        else
        {
            Printf("Disable Sharpness Pat Gen\n");
        }
        vDrvMainSharpPatGen(u1Mode);
    }
    else
    {
        Printf("Usage: %s <OFF(0)/ON(1)>\n", szArgv[0]);
        return 0;
    }
    return 0;
}


INT32 _i4YcprocTDSReadYTbl(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s \n", szArgv[0]);
        return 0;
    }
    vDrvReadYLevTable();
    return 0;
}

INT32 _i4YcprocTDSReadLCTbl(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s \n", szArgv[0]);
        return 0;
    }
    vDrvReadLocalTable();
    return 0;
}



static INT32 _i4YcprocChromaLineAvg(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <OFF(0)/UP(1)/DOWN(2)/AVG121(3)> \n", szArgv[0]);
		return 0;
	}

	vDrvNRSetChromaLineAvgMode(SV_VP_MAIN, (NR_CHROMA_LINE_AVERAGE_MODE)StrToInt(szArgv[1]));
	return 0;
}


static INT32 _i4YcprocOSMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Path;

	if (i4Argc != 3)
	{
		Printf("Usage: %s <Main(m)/PIP(p)> <OFF(0)/ON(1)>\n", szArgv[0]);

		return 0;
	}

	u1Path = ((x_strcmp(szArgv[1], "m") == 0) ? SV_VP_MAIN : SV_VP_PIP);
	vDrvOSMatrixOnOff(u1Path , (UINT8)StrToInt(szArgv[2]));

	Printf("Set OS Matrix %d\n", (UINT8)StrToInt(szArgv[2]));
    return 0;
}

static INT32 _i4YcprocPeUiDebug(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: Enable PE UI Debug Message <OFF(0)/ON(1)>\n");
		return 0;
	}

	vApiSetPeUiDebugMode((UINT8)StrToInt(szArgv[1]));
	Printf("Set PE UI Debug Message %d\n", (UINT8)StrToInt(szArgv[1]));
    return 0;
}

#ifdef CC_MT5363
static INT32 _i4YcprocLCE(INT32 i4Argc, const CHAR **szArgv)
{

	if (i4Argc != 2)
	{
		Printf("Usage: Auto LCE <OFF(0)/ON(1)>\n");

		return 0;
	}

	vDrvAutoLCEOnOff((UINT8)StrToInt(szArgv[1]));

	Printf("Set Auto LCE %d\n", (UINT8)StrToInt(szArgv[1]));
    return 0;
}
#endif

static INT32 _i4YcprocContMeter(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <OFF(0)/ON(1)> \n", szArgv[0]);
		return 0;
	}

	vDrvContMeterOnOff(StrToInt(szArgv[1]));
	return 0;
}

INT32 _i4YcprocPre(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path, u1OnOff;

    if (i4Argc != 3)
    {
        Printf("Usage: Pre-Proc <Main(m/0)/PIP(p/1)> <Bypass(0)/Enable(1)> \n");
        return 0;
    }

    u1Path = (((x_strcmp(szArgv[1], "m") == 0) ||(StrToInt(szArgv[1])==0)) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((UINT8)StrToInt(szArgv[2]) == 0) ? SV_OFF : SV_ON;

    vDrvAntiAliasingFilterOnOff(u1Path , u1OnOff);
    vDrvMatrixCtrlSel(u1Path , u1OnOff ? 0 : 4);
    vDrvChromaCoringOnOff(u1Path , u1OnOff);

    return 0;
}

INT32 _i4YcprocPost(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;

    if (i4Argc != 2)
    {
        Printf("Usage: Post-Proc <Bypass(0)/Enable(1)> \n");
        return 0;
    }

    u1OnOff = ((UINT8)StrToInt(szArgv[1]) == 0) ? SV_OFF : SV_ON;

    vHalVideoSharpnessOnOff(SV_VP_MAIN , u1OnOff);
    vHalVideoLTIOnOff(SV_VP_MAIN , u1OnOff);
    vHalVideoCTIOnOff(SV_VP_MAIN , u1OnOff);
    vDrvSCEAllOnOff(u1OnOff);

    return 0;
}

INT32 _i4YcprocOS(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path, u1OnOff;

    if (i4Argc != 3)
    {
        Printf("Usage: Output-Stage <Main(m/0)/PIP(p/1)> <Bypass(0)/Enable(1)> \n");
        return 0;
    }

    u1Path = (((x_strcmp(szArgv[1], "m") == 0) ||(StrToInt(szArgv[1])==0)) ? SV_VP_MAIN : SV_VP_PIP);
    u1OnOff = ((UINT8)StrToInt(szArgv[2]) == 0) ? SV_OFF : SV_ON;

    vDrvOSMatrixOnOff(u1Path , u1OnOff);
    vDrvGammaOnOff(u1OnOff);
    if (u1OnOff == SV_OFF)
    {
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 512);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 512);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 512);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0);
    }

    return 0;
}

INT32 _i4YcprocPQ(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;

    if (i4Argc != 2)
    {
        Printf("Usage: PQ <OFF(0)/ON(1)> \n");
        return 0;
    }

    u1OnOff = ((UINT8)StrToInt(szArgv[1]) == 0) ? SV_OFF : SV_ON;

    vVideoAllPQItemOnOff(u1OnOff);

    return 0;
}

INT32 _i4YcprocAngSCE(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Phase, u1Value;

    if (i4Argc != 4)
    {
        Printf("Usage: asce <phase(0-5)> <sat(s)/hue(h)> <value>\n");
        return 0;
    }

    u1Phase = (UINT8)StrToInt(szArgv[1]);
    u1Value =  (UINT8)StrToInt(szArgv[3]);

    if (x_strcmp(szArgv[2], "s") == 0)
    {
        vHalMainSCEAngularSetSatVal(u1Phase, u1Value);
    }
    else if (x_strcmp(szArgv[2], "h") == 0)
    {
        vHalMainSCEAngularSetHueVal(u1Phase, u1Value);
    }
    else
    {
        Printf("Usage: asce <phase> <sat(s)/hue(h)> <value>\n");
    }

    return 0;
}

INT32 _i4YcprocAngReset(INT32 i4Argc, const CHAR **szArgv)
{
    vHalMainSCEAngularReset();
    return 0;
}

#ifdef SUPPORT_FLASH_PQ
extern UINT8 bApiFlashPqUpdateSmartPic(void);

static INT32 _i4YcprocPqVerify(INT32 i4Argc, const CHAR **szArgv)
{
	switch (StrToInt(szArgv[1]))
	{
	default:
	case 0:
		Printf("PQ Offset:%d\n", DRVCUST_OptGet(eFlashPqBaseAddress));
		break;
	case 1:
		bApiFlashPqVerifyHeader();
		break;
	case 2:
		bApiFlashPqUpdateGamma();
		break;
	case 3:
		bApiFlashPqUpdateSCE();
		break;
	case 4:
		bApiFlashPqUpdateQtyDft();
		break;
	case 10:
		Printf("SmartPic Offset:%d\n", DRVCUST_OptGet(eFlagFlashPqSmartPicOffset));
		break;
#ifdef THE_3RD_PARTY_SW_SUPPORT
	case 11:
		bApiFlashPqUpdateSmartPic();
		break;
#endif //#ifdef THE_3RD_PARTY_SW_SUPPORT
	}
    return 0;
}

static INT32 _i4YcprocPqQuery(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("[%d]=%d (0x%08X)\n",
		StrToInt(szArgv[1]),
		*((UINT32*)((UINT32)StrToInt(szArgv[1]))),
		*((UINT32*)((UINT32)StrToInt(szArgv[1]))));
    return 0;
}

static INT32 _i4YcprocPqver(INT32 i4Argc, const CHAR **szArgv)
{
    char sz[24];
    d_custom_vid_get(DRV_CUSTOM_DISP_TYPE_PQ_VER,
        DRV_CUSTOM_VID_INP_FLAG_AV,
        sz, (SIZE_T *)NULL);
	Printf("DRV_CUSTOM_DISP_TYPE_PQ_VER: %s\n", sz);

    d_custom_vid_get(DRV_CUSTOM_DISP_TYPE_GAMMA_VER,
        DRV_CUSTOM_VID_INP_FLAG_AV,
        sz, (SIZE_T *)NULL);
	Printf("DRV_CUSTOM_DISP_TYPE_GAMMA_VER: %s\n", sz);

    d_custom_vid_get(DRV_CUSTOM_DISP_TYPE_SCE_VER,
        DRV_CUSTOM_VID_INP_FLAG_AV,
        sz, (SIZE_T *)NULL);
	Printf("DRV_CUSTOM_DISP_TYPE_SCE_VER: %s\n", sz);

    d_custom_vid_get(DRV_CUSTOM_DISP_TYPE_QTY_VER,
        DRV_CUSTOM_VID_INP_FLAG_AV,
        sz, (SIZE_T *)NULL);
	Printf("DRV_CUSTOM_DISP_TYPE_QTY_VER: %s\n", sz);

    return 0;
}

#ifdef THE_3RD_PARTY_SW_SUPPORT
extern void vApiFlashPqProcFeatureSel(UINT16 * wFS);
static INT32 _i4YcprocFS(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 wFS;
	wFS = (UINT16)StrToInt(szArgv[1]);
	vApiFlashPqProcFeatureSel(&wFS);
    return 0;
}

extern void vApiVideoSCEGain(const UINT32 *pdwTable, UINT8 bSceGain);
static INT32 _i4YcprocSceGain(INT32 i4Argc, const CHAR **szArgv)
{
//	vApiVideoSCEGain(DEFAULT_SCE_TBL, (UINT8)StrToInt(szArgv[1]));
    return 0;
}


#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT

static INT32 _i4YcprocPanelID(INT32 i4Argc, const CHAR **szArgv)
{
	LOG(0, "PANEL_GetEepromPanelIndex=%d\n",
		PANEL_GetEepromPanelIndex());
	LOG(0, "PANEL_TranslateEepromToFlashIndex=%d\n",
		PANEL_TranslateEepromToFlashIndex(PANEL_GetEepromPanelIndex()));
    return 0;
}

static INT32 _i4YcprocSetSmartPic(INT32 i4Argc, const CHAR **szArgv)
{
	vApiFlashPqSetSmartPicId((UINT8)StrToInt(szArgv[1]));
    return 0;
}

static INT32 _i4YcprocQtyDft(INT32 i4Argc, const CHAR **szArgv)
{
	bApiFlashPqUpdateQtyDft();
    return 0;
}

static INT32 _i4YcprocQtyMinMax(INT32 i4Argc, const CHAR **szArgv)
{
	bApiFlashPqUpdateQtyMinMax((UINT8)StrToInt(szArgv[1]));
    return 0;
}
#endif // #ifdef SUPPORT_FLASH_PQ


////////////////////////////////////////////////////////////////////////////////
// Memory control related functions
////////////////////////////////////////////////////////////////////////////////

#if 0
EXTERN void vDrvMJCOnOff(UINT8 u1OnOff);
EXTERN void vDrvMJCSetLumaComp(UINT8 bOnOff, UINT8 fgYIs8Bit, UINT8 bCompSize);
EXTERN void vDrvMJCSetFixSTAWeight(UINT8 bOnOff, UINT8 bClipGain, UINT8 GainValue);
EXTERN void vDrvMJCSetMVDownScaleFRM(UINT8 bOnOff, UINT8 GainValueX, UINT8 GainValueY,
                                     UINT8 bClip, UINT8 bClipSEL, UINT16 ClipValueX, UINT16 ClipValueY);
EXTERN void vDrvMJCSetInk(UINT8 bScnChgInk, UINT8 bMCInk, UINT8 bBK22VDBGMode, UINT8 bBK88VDBGMode);
EXTERN UINT8 bDrvMJCMbistGo(void);
EXTERN UINT8 bDrvMJCCustQPTblStressTest(UINT32 dwTimes);
#endif
static INT32 _i4MjcOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <on/off>\n", szArgv[0]);

		return 1;
	}

	//vDrvMJCOnOff(u1OnOff);

	return 0;
}

static INT32 _i4MjcYComp(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bCompSize;

	if (i4Argc != 4)
	{
		Printf("Usage: %s <on/off> <fgIs8bit> <com size>\n", szArgv[0]);

		return 1;
	}

	bCompSize = (UINT8) StrToInt(szArgv[3]);

	if (bCompSize > 5 || bCompSize < 1)
	{
		Printf("1 <= com size <= 5\n");
		return 1;
	}

	//vDrvMJCSetLumaComp(bOnOff, fgYIs8bit, bCompSize);

	return 0;
}

static INT32 _i4MjcFixSTA(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: %s <on/off> <clip gain on/off> <gain value>\n", szArgv[0]);
		return 1;
	}

	//vDrvMJCSetFixSTAWeight(bOnOff, bClipGain, GainValue);
	return 0;
}

static INT32 _i4MjcMVDownScaleFRM(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 5)
	{
		Printf("Usage: %s <on/off> <gain Y-X> <clip SEL-on/off> <clip Y-X>\n", szArgv[0]);
		return 1;
	}


    //vDrvMJCSetMVDownScaleFRM(bOnOff, GainValueX, GainValueY, bClip, bClipSEL, ClipValueX, ClipValueY);
	return 0;
}

static INT32 _i4MjcMVMCInk(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 5)
	{
		Printf("Usage: %s <scene change on/off> <MC Ink on/off> <BK22 VDBG Mode> <BK88 VDBG Mode>\n", szArgv[0]);
		return 1;
	}

	//vDrvMJCSetInk(bScnChgInk, bMCInk, bBK22VDBGMode, bBK88VDBGMode);
	return 0;
}

static INT32 _i4MjcMbistGo(INT32 i4Argc, const CHAR **szArgv)
{
	#if 0
	if (bDrvMJCMbistGo())
	{
		Printf("MJC Mbist: Normal Process.\n");
	}
	else
	{
		Printf("MJC Mbist: Fatal Error.\n");
	}
	#endif
	return 0;
}

static INT32 _i4MjcCompStressTest(INT32 i4Argc, const CHAR **szArgv)
{

    //UNUSED(bDrvMJCCustQPTblStressTest((i4Argc != 2) ? 1 : StrToInt(szArgv[1])));
    return 0;
}



#endif //#ifdef CC_CLI

