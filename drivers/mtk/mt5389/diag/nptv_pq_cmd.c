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
 * $Author: pingan.du $
 * $Date: 2012/06/09 $
 * $RCSfile: nptv_pq_cmd.c,v $
 * $Revision: #4 $
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
#include "drv_lcdim.h"
#include "drv_pqstress.h"
#include "drv_contrast.h"
#include "drv_scpos.h"
#include "api_backlt.h"
#include "hal_ycproc.h"
#include "drv_pwm.h"
#include "u_drv_cust.h"
#include "drv_meter.h"
#include "drv_noisemeter.h"
#include "drv_ldmspi.h"
#include "pe_if.h"
//#include "color_param.h"
#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

// for local dimming info
static INT32 _i4LcDimSPIInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSPISet1BReadWrite(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSPISetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSPILoopEn(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSPIGetInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSetMaxThd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimContentInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimSetPanelInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4LcDimInitQuality(INT32 i4Argc, const CHAR **szArgv);

// for adaptive luma adjustment
static INT32 _i4AlGetLumaHist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetLumaHistLR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetSatHist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetHueHist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetLumaInfo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetRGBMAX(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AlGetDist(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i4SetOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetALPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetBSPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4SetWSPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4GetALInfo(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i4YcprocGammaMute(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGammaUnMute(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGammaCompenstion(INT32 i4Argc, const CHAR **szArgv);

// for adaptive backlight function.
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
static INT32 _i4YcprocSCELevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSCESkinLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSCE(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSCEIdx(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocCTI(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGamma(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocOfst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocSharpness(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocTDSReadYTbl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocChromaLineAvg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocOSMatrix(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocSharpnessPatGen(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocSCEPatGen(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPre(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPost(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocOS(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocPQ(INT32 i4Argc, const CHAR **szArgv);
INT32 _i4YcprocAngSCE(INT32 i4Argc, const CHAR **szArgv);
#ifndef NDEBUG
static INT32 _i4YcprocPqSlt(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _i4YcprocPeUiDebug(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4YcprocIsrLog(INT32 i4Argc, const CHAR **szArgv);

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

/* Function for Tdc */
static INT32 _TdcOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdcGameMode(INT32 i4Argc, const CHAR **szArgv);


/* Functions for NR */
static INT32 _i4NRInk(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4NRNMDbg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4NRGetNRStatus(INT32 i4Argc, const CHAR **szArgv);
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

CLI_EXEC_T arLocalDim[] = {
#ifdef CC_CLI
    {"Init LcDim Quality",	"i",	_i4LcDimInitQuality,	NULL,	"Init local dimming quality", CLI_GUEST},                
    {"Set Panel Info",	"p",	_i4LcDimSetPanelInfo,	NULL,	"Set panel info", CLI_GUEST},                
    {"Get Content Info",	"c",	_i4LcDimContentInfo,	NULL,	"Get block content info", CLI_GUEST},
    {"Set Max Thd",	"maxthd",	_i4LcDimSetMaxThd,	NULL,	"Set max threshold", CLI_GUEST},        
    {"SPI Init",	"si",	_i4LcDimSPIInit,	NULL,	"SPI Init", CLI_GUEST},        
    {"SPI Set R/W",	"rw",	_i4LcDimSPISet1BReadWrite,	NULL,	"SPI Init", CLI_GUEST},                        
    {"SPI Set Command",	"ss",	_i4LcDimSPISetCmd,	NULL,	"SPI Set Command", CLI_GUEST},       
    {"SPI Get Info",	"gi",	_i4LcDimSPIGetInfo,	NULL,	"SPI Get Info", CLI_GUEST},                       
    {"SPI Loop OnOff",	"loop",	_i4LcDimSPILoopEn,	NULL,	"SPI Loop OnOff", CLI_GUEST},                  
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


CLI_EXEC_T arAdaptiveLuma[] = {
#ifdef CC_CLI
    {"Get Luma Hist",	"lhist",	_i4AlGetLumaHist,	NULL,	"Get luma histogram", CLI_GUEST},
    {"Get LR Luma Hist",	"lhistlr",	_i4AlGetLumaHistLR,	NULL,	"Get LR luma histogram", CLI_GUEST},        
    {"Get Luma Info",	"linfo",	_i4AlGetLumaInfo,	NULL,	"Get luma infomation", CLI_GUEST},
    {"Get Sat Hist",	"shist",	_i4AlGetSatHist,	NULL,	"Get Sat histogram", CLI_SUPERVISOR},
    {"Get Hue Hist",	"hhist",	_i4AlGetHueHist,	NULL,	"Get Hue histogram", CLI_SUPERVISOR},
    {"Get RGBMAX",	"rgbmax",	_i4AlGetRGBMAX,	NULL,	"Get RGBMAX", CLI_SUPERVISOR},
    {"Get Distribution",	"dist",	_i4AlGetDist,	NULL,	"Get Distribution", CLI_SUPERVISOR},
    {"Set OnOff", "s", _i4SetOnOff, NULL, "Set ON OFF", CLI_GUEST},
    {"Set Adaptive Luma", "sal", _i4SetALPara, NULL, "Set Adaptive Luma Param", CLI_GUEST},
    {"Set BS", "sbs", _i4SetBSPara, NULL, "Set Black Stretch Param", CLI_GUEST},
    {"Set WS", "sws", _i4SetWSPara, NULL, "Set White Stretch Param", CLI_GUEST},
    {"Get Info", "g", _i4GetALInfo, NULL, "Get Info", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_EXEC_T arAdaptiveBacklight[] = {
#ifdef CC_CLI
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
    {"Control Sel", "ctrlsel", _i4YcprocCtrlSel, NULL, "3x3 Matrix Control Mode Sel", CLI_SUPERVISOR},
    {"SCE OnOff", "sce", _i4YcprocSCE, NULL, "Enable/disable SCE", CLI_GUEST},
    {"SCE Level", "scelvl", _i4YcprocSCELevel, NULL, "Set SCE Level", CLI_SUPERVISOR},
    {"SCE Skin Level", "skin", _i4YcprocSCESkinLevel, NULL, "Set SCE Skin Level", CLI_SUPERVISOR},
    {"SCE Angular", "asce", _i4YcprocAngSCE, NULL, "Set SCE Angular", CLI_SUPERVISOR},
    {"SCE Idx", "sceidx", _i4YcprocSCEIdx, NULL, "Update SCE Table by Using Function and Index", CLI_GUEST},
    {"SCE PatGen", "scepat", _i4YcprocSCEPatGen, NULL, "SCE Pattern Generator", CLI_SUPERVISOR},
    {"CTI", "cti", _i4YcprocCTI, NULL, "Enable/disable CTI", CLI_GUEST},
    {"Gamma", "gamma", _i4YcprocGamma, NULL, "Select gamma mode", CLI_SUPERVISOR},
    {"Gamam", "gmc", _i4YcprocGammaCompenstion, NULL, "Select gamma Compensation", CLI_SUPERVISOR},
    {"RGB Gain", "gain", _i4YcprocGain, NULL, "Select R/G/B gain", CLI_GUEST},
    {"RGB Offset", "ofst", _i4YcprocOfst, NULL, "Select R/G/B offset", CLI_GUEST},
    {"Sharpness", "sharp", _i4YcprocSharpness, NULL, "Enable/disable sharpness", CLI_GUEST},
    {"Sharp PatGen", "sharppat", _i4YcprocSharpnessPatGen, NULL, "Sharpness Pattern Generator", CLI_SUPERVISOR},
    {"Read TDS Y Tbl", "TDSytbl", _i4YcprocTDSReadYTbl, NULL, "Read TDSharp Y level table", CLI_SUPERVISOR},
    {"ChromaLineAvg", "cla", _i4YcprocChromaLineAvg, NULL, "Chroma Line Average", CLI_GUEST},
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
	{"Mute", "mute", _i4YcprocGammaMute, NULL, "Mute", CLI_SUPERVISOR},
	{"UnMute", "unmute", _i4YcprocGammaUnMute, NULL, "Mute", CLI_SUPERVISOR},
	{"AdaptiveLuma", "al",  NULL, arAdaptiveLuma, "AdaptiveLuma Command",  CLI_GUEST},
	{"Local Dimming", "lcdim",  NULL, arLocalDim, "Local Dimming Command",  CLI_GUEST},	    
	{"AdaptiveBacklight", "ab",  NULL, arAdaptiveBacklight, "AdaptiveBacklight Command",  CLI_SUPERVISOR},
	{"OS Matrix", "osm",  _i4YcprocOSMatrix, NULL, "Enable/disable Output Stage Matrix",  CLI_SUPERVISOR},
       {"PreProc", "pre", _i4YcprocPre, NULL, "Enable/Disable Pre-Proc", CLI_SUPERVISOR},
       {"PostProc", "post", _i4YcprocPost, NULL, "Enable/Disable Post-Proc", CLI_SUPERVISOR},
       {"OutputStage", "os", _i4YcprocOS, NULL, "Enable/Disable Output Stage", CLI_SUPERVISOR},
#ifndef NDEBUG
    {"PQ SRAM SLT", "pqslt", _i4YcprocPqSlt, NULL, "Enable/Disable PE SRAM Stress Test", CLI_SUPERVISOR},        
#endif      
    {"PeUiDebug", "ui", _i4YcprocPeUiDebug, NULL, "Enable/Disable PE UI Debug Message", CLI_GUEST},
    {"IsrLog", "ilog", _i4YcprocIsrLog, NULL, "ISR Debug Message", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for TDC command entry
CLI_EXEC_T arTdcCmdTbl[] = {
#ifdef CC_CLI
    {"TDC Enable",	"TdcOnOff",	_TdcOnOff,	NULL,	"TDC Enable Function",CLI_GUEST},
    {"TDC GameMode", "GameMode",  _TdcGameMode, NULL, "TDC GameMode Enable",CLI_GUEST},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for NR command entry
CLI_EXEC_T arNRCmdTbl[] = {
#ifdef CC_CLI
    {"3DNR Ink", "ink", _i4NRInk, NULL, "3DNR ink", CLI_SUPERVISOR},
    {"NM debug", "dbg", _i4NRNMDbg, NULL, "NM debug", CLI_SUPERVISOR},
    {"Query NR Status", "q", _i4NRGetNRStatus, NULL, "Query NR current status", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(NR),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _TdcOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bOnOff;
    if(i4Argc !=2)
    {
        printf("Usage: TdcOnOff [OnOff]\n");
    }
    else
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        vDrvTDCOnOff(bOnOff);
	}	
    return 1;
}
extern const FLASH_PQ_SW_INFO aFlashPqSWInfo[FLASHPQ_NUM];
extern const FLASH_PQ_FLASH_INFO aFlashPqFlashInfo[FLASHPQ_NUM];

static INT32 _TdcGameMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bOnOff;
    UINT8 bIndex;
    if(i4Argc !=2)
    {
        printf("Usage: GameMode [OnOff]\n");
        for(bIndex=0; bIndex<FLASHPQ_NUM; bIndex++)           //------------------------
    {
        Printf("ddddddddddddddddddddd\n");
        if (bApiFlashPqVerifyHeader(bIndex) == SV_TRUE)
        {
            // If not support SmartPic, update QtyTable at boot-up. Otherwise, it will be done at each mode change done.
    		//Prevent 1st time boot up mode change done before flashPQ valued loaded.	

	        if (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) == SV_FALSE) 
	        {
	            if (bApiFlashPqUpdateQtyDft(bIndex) == SV_FALSE)
	            {
	                LOG(0, "Restore FlashPQ Qty fail\n");
	            }
                
	        }
            
    	}
        else
        {
            LOG(0, "FlashPQ<%d> verify Header fail\n", bIndex);
        }
        bApiFlashPqUpdateGamma();
        bApiFlashPqUpdateSCE();
    }
    }
    else
    {
        bOnOff = (UINT8)StrToInt(szArgv[1]);
        vDrvSetTDCGameMode(bOnOff);
	}	
    return 1;
}

static INT32 _i4NRInk(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc == 1)
    {
        vDrvNRSetInk(0, 0);
    }
    else if(i4Argc == 2)
    {
        vDrvNRSetInk(StrToInt(szArgv[1]), 0xFF);        
    }
    else if(i4Argc == 3)
    {
        vDrvNRSetInk(StrToInt(szArgv[1]), StrToInt(szArgv[2]));
    }

    return 0;
}

EXTERN void vDrvNRSetNMDebugInk(UINT32 u4Enable);
static INT32 _i4NRNMDbg(INT32 i4Argc, const CHAR **szArgv)
{
    
    if (i4Argc !=2)
    {
        Printf("Usage: %s [0(On)/1(Off)]\n", szArgv[0]);
    }
    else
    {
        UINT8 bOnOff = (UINT8)StrToInt(szArgv[1]);
        vDrvNRSetNMDebugInk(bOnOff);
	}	
    
    return 0;
}

static INT32 _i4NRGetNRStatus(INT32 i4Argc, const CHAR **szArgv)
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

#if 0
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
#endif

INT32 _PreprocEnablePattern(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 3)
    {
        printf("Usage: n.ptn.pre [path] [pat num]\n");
        printf("[path] 0: main 1:sub\n");
        printf("[pat num]:\n");
        printf("0: pattern off\n");
        printf("1: 100% color bar\n");
        printf("2: 75% color bar\n");
        printf("3: ramp\n");
        printf("4: 10 step\n");
        printf("5: raster\n");
        printf("6: cross hatch\n");
    }
    else
    {
        UINT8 bPath;
        UINT8 bPattern;
        bPath = (UINT8)StrToInt(szArgv[1]);
        bPattern = (UINT8)StrToInt(szArgv[2]);

        vDrvPreprocEnablePattern(bPath,bPattern);
    }
    return 0;
}

#if 0
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
#endif


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
extern UINT8 bAdlVersion;
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
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_ON_1_OFF_0, value);
                wWriteQualityTable(QUALITY_AL_BWS_BK_EN, value);
                vHalVideoAdaptiveLumaParam();
                break;
            case 2:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_ON_1_OFF_0, value);
                wWriteQualityTable(QUALITY_AL_BWS_WT_EN, value);
                vHalVideoAdaptiveLumaParam();
                break;
            case 3:
                vDrvAutoContrastFreeze(value);
                break;
            default:
                break;
    	}
	}
    else
    {
        Printf("Usage : set <AL(0)/BS(1)/WS(2)/Freeze(3> <OFF(0)/ON(1)>\n" );
    }

    return 0;
}

static INT32 _i4SetALPara(INT32 i4Argc, const CHAR **szArgv)
{
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

        if(bAdlVersion == SV_OFF)
        {            
		switch (index)
    	{
            case 0:
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_LEVEL, value);
                Printf("QUALITY_BWS_BLACK_LEVEL = %d\n", value);
                break;
            case 1:
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_GAIN, value);
                Printf("QUALITY_BWS_BLACK_GAIN = %d\n", value);
                break;				
            case 2:
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_OFFSET, value);
                Printf("QUALITY_BWS_BLACK_OFFSET = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_RATIO, value);
                Printf("QUALITY_BWS_BLACK_RATIO = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_AL_BWS_BLACK_LIMIT, value);
                Printf("QUALITY_BWS_BLACK_LIMIT = %d\n", value);
                break;
            default:
                break;
    	}

        }
        else
        {
            switch (index)
            {
                case 0:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_LEVEL, value);
                    Printf("QUALITY_BWS_BLACK_LEVEL = %d\n", value);
                    break;
                case 1:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_AC_GAIN_RATIO, value);
                    Printf("QUALITY_BWS_BLACK_AC_GAIN_RATIO = %d\n", value);
                    break;              
                case 2:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_DC_GAIN_RATIO, value);
                    Printf("QUALITY_BWS_BLACK_DC_GAIN_RATIO = %d\n", value);
                    break;
                case 3:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_RATIO, value);
                    Printf("QUALITY_BWS_BLACK_RATIO = %d\n", value);
                    break;
                case 4:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_ADPTLMT_EN, value);
                    Printf("QUALITY_BWS_BLACK_ADPTLMT_ON_1_OFF_0 = %d\n", value);
                    break;
                case 5:
                    wWriteQualityTable(QUALITY_AL_BWS_BK_STROFT_EN, value);
                    Printf("QUALITY_BWS_BLACK_STROFT_ON_1_OFF_0 %d\n", value);
                    break;
                default:
                    break;
            }

        }

		vHalVideoAdaptiveLumaParam();
	}
    else
    {
        if(bAdlVersion == SV_OFF)
        {
        Printf("Usage : sbws < BLevel(0)/BGain(1)/BOffset(2)/BRatio(3)/BLimit(4) > < value >\n" );
    }
        else
        {
            Printf("Usage : sbws < BLevel(0)/BACGain(1)/BDCGain(2)/BRatio(3)/BADPTLimit_EN(4)/BStrongOffset_EN(5) > < value >\n" );
        }

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

        if(bAdlVersion == SV_OFF)
        {
		switch (index)
    	{
			case 0:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_LEVEL, value);
                Printf("QUALITY_BWS_WHITE_LEVEL = %d\n", value);
				 break;
			case 1:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_GAIN, value);
                Printf("QUALITY_BWS_WHITE_GAIN = %d\n", value);
                break;
            case 2:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_OFFSET, value);
                Printf("QUALITY_BWS_WHITE_OFFSET = %d\n", value);
                break;
            case 3:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_RATIO, value);
                Printf("QUALITY_BWS_WHITE_RATIO = %d\n", value);
                break;
            case 4:
                wWriteQualityTable(QUALITY_AL_BWS_WHITE_LIMIT, value);
                Printf("QUALITY_BWS_WHITE_LIMIT = %d\n", value);
                break;
            default:
                break;
    	}
        }
        else
        {
            switch (index)
            {
                case 0:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_LEVEL, value);
                    Printf("QUALITY_BWS_WHITE_LEVEL = %d\n", value);
                     break;
                case 1:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_AC_GAIN_RATIO, value);
                    Printf("QUALITY_BWS_WHITE_AC_GAIN_RATIO = %d\n", value);
                    break;
                case 2:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_DC_GAIN_RATIO, value);
                    Printf("QUALITY_BWS_WHITE_DC_GAIN_RATIO = %d\n", value);
                    break;
                case 3:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_RATIO, value);
                    Printf("QUALITY_BWS_WHITE_RATIO = %d\n", value);
                    break;
                case 4:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_ADPTLMT_EN, value);
                    Printf("QUALITY_BWS_WHITE_ADPTLMT_ON_1_OFF_0 %d\n", value);
                    break;
                case 5:
                    wWriteQualityTable(QUALITY_AL_BWS_WT_STROFT_EN, value);
                    Printf("QUALITY_BWS_WHITE_STROFT_ON_1_OFF_0 %d\n", value);
                    break;
                default:
                    break;
            }

        }

		vHalVideoAdaptiveLumaParam();
	}
    else
    {
        if(bAdlVersion == SV_OFF)
        {
        Printf("Usage : sbws < WLevel(0)/WGain(1)/WOffset(2)/WRatio(3)/WLimit(4) > < value >\n" );
        }
        else
        {
            Printf("Usage : sbws < WLevel(0)/WACGain(1)/WDCGain(2)/WRatio(3)/WADPTLimit_EN(4)/WStrongOffset_EN(5) > < value >\n" );

        }        

    }

	return 0;
}

static INT32 _i4GetALInfo(INT32 i4Argc, const CHAR **szArgv)
{
    return 0;
}

static INT32 _i4LcDimContentInfo(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <0-2> ([0] max [1] average [2] blending [3] dimming)\n", szArgv[0]);
		return 0;
	}
	

    if(StrToInt(szArgv[1]) == 0)
    {
        vDrvUpdateLcDimContentInfo(1);
        Printf("\n\n[Local Dimming Content Max Info]\n");
    }
    else if (StrToInt(szArgv[1]) == 1)
    {
        vDrvUpdateLcDimContentInfo(1);
        Printf("\n\n[Local Dimming Content Average Info]\n");
    }
    else if (StrToInt(szArgv[1]) == 2)
    {
        vDrvUpdateLcDimContentInfo(1);
        Printf("\n\n[Local Dimming Content Index Info]\n");        
    }
    else if (StrToInt(szArgv[1]) == 3)
    {
        vDrvUpdateLcDimContentInfo(0);
        Printf("\n\n[Local Dimming Dimming Result]\n");        
    }    
    else
    {
        Printf("Usage: cinfo <0-3> ([0] max [1] average [2] blending [3] dimming)\n");
        return 0;
    }
    
    vDrvLcDimLog((UINT8)StrToInt(szArgv[1]));

	return 0;
}

static INT32 _i4LcDimInitQuality(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s [init]\n", szArgv[0]);
		return 0;
	}

    vDrvLcDinQualityInit();

	return 0;
}

static INT32 _i4LcDimSPIInit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s [init]\n", szArgv[0]);
		return 0;
	}

    if(bDrvLcdimSpiInit() == SV_FAIL)
    {
        Printf("SPI Initialization Fail\n");
    }

	return 0;
}
static INT32 _i4LcDimSPIGetInfo(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);

		return 0;
	}

    vDrvLcdimSpiGetSystemInfo();

	return 0;
}

static INT32 _i4LcDimSPISet1BReadWrite(INT32 i4Argc, const CHAR **szArgv)
{
	if ((i4Argc != 3) && (i4Argc != 4))
	{
		Printf("Usage: %s [read(0)/write(1)] [addr] [data]\n", szArgv[0]);
		return 0;
	}

	if (i4Argc == 3)
	{
	    if((UINT8)StrToInt(szArgv[1]) == 0)
	    {
            vDrvLcdimSpi1BRead((UINT8)StrToInt(szArgv[2]));
	    }
	    else
	    {
    		Printf("Usage: %s [read(0)] [addr] \n", szArgv[0]);
	    }
		return 0;	    
	}

	if (i4Argc == 4)
	{
	    if((UINT8)StrToInt(szArgv[1]) == 1)
	    {
            vDrvLcdimSpi1BWrite((UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));
	    }
	    else
	    {
    		Printf("Usage: %s [write(1)] [addr] [data]\n", szArgv[0]);
	    }
		return 0;	    
	}	
    return 0;	
}

static INT32 _i4LcDimSPISetCmd(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s [command set]\n", szArgv[0]);
		Printf("[0] write [1] read [2] light up [3] logoff [4] set scanning\n");
		return 0;
	}

    vDrvLcdimSpiCommandGen((UINT8)StrToInt(szArgv[1]));

	return 0;
}

static INT32 _i4LcDimSPILoopEn(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s [on/off]\n", szArgv[0]);
		return 0;
	}

    vDrvLcdimSpiSetLoopAllOnOff((UINT8)StrToInt(szArgv[1]));

	return 0;
}

static INT32 _i4LcDimSetMaxThd(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s [max threshold]\n", szArgv[0]);
		return 0;
	}

    vDrvLcDimSetMaxThd((UINT32)StrToInt(szArgv[1]));

	return 0;
}


static INT32 _i4LcDimSetPanelInfo(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s [horitontal block num] [vertical block num]\n", szArgv[0]);
		return 0;
	}

    if(bDrvLcDimSetPanelInfo((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]))==SV_SUCCESS)
    {
		Printf("[LDM] Panel Block Initialization Success\n");        
    }

	return 0;
}

static INT32 _i4AlGetLumaHist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i, u2Base = 0;
	UINT16 wHist[LUMA_HIST_NUM];

    if (i4Argc == 2)
	{
        u2Base = (UINT16)StrToInt(szArgv[1]);
	}

	u1DrvGetLumaHist(wHist, u2Base);
	for (i=0; i<LUMA_HIST_NUM; i++)
	{
		Printf("%d ", wHist[i]);
	}
	
	Printf("\n");

	return 0;
}

static INT32 _i4AlGetLumaHistLR(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i, u2Base = 0;
	UINT16 wHistL[LUMA_HIST_NUM];
	UINT16 wHistR[LUMA_HIST_NUM];    

    if (i4Argc == 2)
	{
        u2Base = (UINT16)StrToInt(szArgv[1]);
	}

	vDrvGetLumaHistLR(wHistL, wHistR, u2Base);
	Printf("L Hist ");    
	for (i=0; i<LUMA_HIST_NUM; i++)
	{
		Printf("%d ", wHistL[i]);
	}
	
	Printf("\n");
	Printf("R Hist ");    
	for (i=0; i<LUMA_HIST_NUM; i++)
	{
		Printf("%d ", wHistR[i]);
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

	Printf("Input  Luma APL  %d\n", bDrvGetAPL());
    Printf("Output Luma max  %d    min  %d \n",bDrvGetOutputLumaMax(SV_VP_MAIN), bDrvGetOutputLumaMin(SV_VP_MAIN));

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

static INT32 _i4AlGetHueHist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i, u2Base = 0, u2aHist[HUE_HIST_NUM];
    UINT32 u4HuePxlCnt = 0;
    if (i4Argc == 2)
    {
        u2Base = (UINT16)StrToInt(szArgv[1]);
    }
    
	u1DrvGetHueHist(u2aHist, &u4HuePxlCnt, u2Base);

    Printf("HUE Hist = ");
	for (i=0; i<HUE_HIST_NUM; i++)
	{
		Printf("%d ", u2aHist[i]);
	}
    if(u2Base)
    {
        Printf("\n Normalize Pixel count = %d \n", u4HuePxlCnt);
    }
    else
    {
        Printf("\n Pixel count = %d \n", u4HuePxlCnt);
    }
	return 0;
}

static INT32 _i4AlGetSatHist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i, u2Base = 0, u2aHist[SAT_HIST_NUM];

    if (i4Argc == 2)
    {
        u2Base = (UINT16)StrToInt(szArgv[1]);
    }

	u1DrvGetSatHist(u2aHist, u2Base );

    Printf("ASL = %d \n", u1DrvGetASL());
    Printf("Saturation Hist = ");
	for (i=0; i<SAT_HIST_NUM; i++)
	{
		Printf("%d ", u2aHist[i]);
	}

	return 0;
}

static INT32 _i4AlGetRGBMAX(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i, wHist[RGB_MAX_HIST_NUM], u2Base = 0;

    if (i4Argc == 2)
    {
        u2Base = (UINT16)StrToInt(szArgv[1]);
    }

	u1DrvGetRGBHist(wHist, u2Base);

	for (i=0; i<RGB_MAX_HIST_NUM; i++)
	{
		Printf("%d ", wHist[i]);
	}
	Printf("\n");

	return 0;
}

static INT32 _i4AlGetDist(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u2Base = 0;

    if (i4Argc == 2)
    {
        u2Base = (UINT16)StrToInt(szArgv[1]);
    }
    Printf("Win1 %d \nWin2 %d \nWin3 %d", 
        u4DrvGetSatHue2DWindowResult(0, u2Base),
        u4DrvGetSatHue2DWindowResult(1, u2Base),
        u4DrvGetSatHue2DWindowResult(2, u2Base));
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
    //vRegWriteFldAlign(DUMMY_03, bOnOff, DCR_LOG);
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
    //vRegWriteFldAlign(DUMMY_03, bValue, LIGHT_SENSOR);

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

	DRVCUST_AdaptiveBacklightParam((UINT8)StrToInt(szArgv[1]));

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
	vDrvAdaptiveBacklightSetCurve(
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


static INT32 _i4YcprocSCELevel(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: %s <LumaGain> <SatGain> <HueGain>\n", szArgv[0]);

		return 0;
	}

	vDrvSetSCELevel((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));

	Printf("Load SCE luma_gain = %d sat_gain=%d, hue_gain=%d\n", (UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));
    return 0;
}

static INT32 _i4YcprocSCESkinLevel(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: %s <LumaGain> <SatGain> <HueGain>\n", szArgv[0]);

		return 0;
	}

	vDrvSetSkinLevel((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));

	Printf("Load SCE Skin luma_gain = %d sat_gain=%d, hue_gain=%d\n", (UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));
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

static INT32 _i4YcprocSCEIdx(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: %s <FunIdx> <HueIdx> <Value>\n", szArgv[0]);
		return 0;
	}
	
	if (vDrvSCELoadIdx(StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3])))
    {
        Printf("FunIdx %d, HueIdx %d, Value %d\n", StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));
    }
    else
    {
		Printf("Usage: %s <FunIdx> <HueIdx> <Value>\n", szArgv[0]);
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

	vHalVideoGammaTblUpdate(u1Mode);
	vHalVideoGammaLoadTable();
	
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
    bDrvReadYLevTable();
    return 0;
}

static INT32 _i4YcprocChromaLineAvg(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <OFF(0)/UP(1)/DOWN(2)/AVG121(3)> \n", szArgv[0]);
		return 0;
	}

	vDrvNRSetChromaLineAvgModeReg(SV_VP_MAIN, (NR_CHROMA_LINE_AVERAGE_MODE)StrToInt(szArgv[1]));
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

#ifndef NDEBUG
static INT32 _i4YcprocPqSlt(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 5)
    {
        Printf("Usage: Enable PE SRAM SLT <OFF(0)/ON(1)> <MODULE> <PATTERN> <ROUND>\n");
        return 0;
    }
    bDrvPeSramStressTest((UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]), (UINT32)StrToInt(szArgv[4]));
    return 0;
}
#endif

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

static INT32 _i4YcprocIsrLog(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc == 2)
	{
		vApiIsrLogEnable((UINT32)StrToInt(szArgv[1]));
		return 0;
	}

	if (i4Argc != 6)
	{        
		Printf("Usage: %s <disable(0)/enable(1)> \n", szArgv[0]);        
		Printf("Usage: %s <ISR: VDOIN(1)/PSCAN(2)/VDOOUT(3)/MJC(4)> <Addr> <low> <high> <count>\n", szArgv[0]);
        vApiIsrLogEnable(0);
        return 0;
	}
    
    vApiConfigIsrLog((UINT32)StrToInt(szArgv[1]), (UINT32)StrToInt(szArgv[2]), 
        (UINT32)StrToInt(szArgv[3]), (UINT32)StrToInt(szArgv[4]), (UINT32)StrToInt(szArgv[5]));

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

    vApiVideoAllPQItemOnOff(u1OnOff);

    return 0;
}

INT32 _i4YcprocAngSCE(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1module, u1Phase, u1Value;

    if (i4Argc != 4)
    {
        Printf("Usage: asce <Y(0)/S(1)/H(2)/Reset(3)> <phase(0-5)> <value>\n");
        return 0;
    }

    u1module = (UINT8)StrToInt(szArgv[1]);
    u1Phase = (UINT8)StrToInt(szArgv[2]);
    u1Value =  (UINT8)StrToInt(szArgv[3]);

    vHalSetSCEAngular(u1module, u1Phase, u1Value);

    return 0;
}

extern UINT8 bApiFlashPqUpdateSmartPic(void);

static INT32 _i4YcprocPqVerify(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("pqv <(0)PartID/(1)Header/(2)Gamma/(3)SCE/(4)Qty>\n");
        return 0;
    }

	switch (StrToInt(szArgv[1]))
	{
	default:
        Printf("pqv <(0)PartID/(1)Header/(2)Gamma/(3)SCE/(4)Qty>\n");
	case 0:
		Printf("Partition ID: %d\n", DRVCUST_OptGet(eFlashPqBasePartition));
		break;
	case 1:
		bApiFlashPqVerifyHeader(0);
		break;
	case 2:
		bApiFlashPqUpdateGamma();
		break;
	case 3:
		bApiFlashPqUpdateSCE();
		break;
	case 4:
		bApiFlashPqUpdateQtyDft(0);
		break;
    case 5:        
        vApiUiDftRangeVerify();        
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
	bApiFlashPqUpdateQtyDft(0);
    return 0;
}

static INT32 _i4YcprocQtyMinMax(INT32 i4Argc, const CHAR **szArgv)
{
	bApiFlashPqUpdateQtyMinMax((UINT8)StrToInt(szArgv[1]), 0);
    return 0;
}

#endif //#ifdef CC_CLI

