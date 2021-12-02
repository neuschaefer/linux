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
/*****************************************************************************
*  Video Plane: Diagnostic command
*****************************************************************************/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
 #ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "vdp_drvif.h"
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "nptv_if.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_serial.h"
#include "x_rand.h"
#include "x_hal_5381.h"
#include "vdp_if.h"
#include "drv_video.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "drv_scpos.h"
#include "srm_drvif.h"
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "hw_scpos.h"
#endif
#include "vdo_misc.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)            // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)            // Info 818: Pointer parameter 'szArgv'


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifndef CC_CLI
#define CC_CLI
#endif

// 656 Module of FPGA

#define HAL_5371_656_REG_NUM 12

typedef struct _HAL_5371_656_FIELD_T
{
	/* 80, 0x26200 */
	UINT32			u4DispHt			:	12;
	UINT32								:	4;
	UINT32			u4DispVt			:	11;
	UINT32								:	5;

	/* 81, 0x26204 */
	UINT32			u4DispSyncHPos		:	13;
	UINT32								:	3;
	UINT32			u4DispSyncVPos		:	11;
	UINT32								:	5;

	/* 82, 0x26208 */
	UINT32			u4DispVs1Start		:	11;
	UINT32								:	1;
	UINT32			u4Mode				:	2;
	UINT32			fg422to444			:	1;
	UINT32								:	1;
	UINT32			u4DispVs1End		:	11;
	UINT32								:	5;
	
	/* 83, 0x2620C */
	UINT32			u4DispVs2Start		:	11;
	UINT32								:	1;
	UINT32			fgVs2Blns			:	1;
	UINT32			fgVs2En				:	1;
	UINT32								:	2;
	UINT32			u4DispVs2End		:	11;
	UINT32								:	5;
	
	/* 84, 0x26210 */
	UINT32			u4DispSyncHStart	:	12;
	UINT32								:	4;
	UINT32			u4DispSyncHEnd		:	12;
	UINT32								:	4;
	
	/* 85, 0x26214 */
	UINT32			u4DispDataEnStart	:	12;
	UINT32								:	4;
	UINT32			u4DispDataEnEnd		:	12;
	UINT32								:	4;
	
	/* 86, 0x26218 */
	UINT32			u4DispFldTgl0		:	11;
	UINT32								:	5;
	UINT32			u4DispFldTgl1		:	11;
	UINT32								:	5;
	
	/* 87, 0x2621C */
	UINT32			u4DispVdeTgl0		:	11;
	UINT32								:	5;
	UINT32			u4DispVdeTgl1		:	11;
	UINT32								:	5;
	
	/* 88, 0x26220 */
	UINT32			u4DispVdeTgl2		:	11;
	UINT32								:	5;
	UINT32			u4DispVdeTgl3		:	11;
	UINT32								:	5;
	
	/* 89, 0x26224 */
	UINT32			fgIvsP				:	1;
	UINT32			fgIhsP				:	1;
	UINT32			fgIfldP				:	1;
	UINT32			fgIfldEn			:	1;
	UINT32			fgOfldP				:	1;
	UINT32			fgOvsP				:	1;
	UINT32			fgOhsP				:	1;
	UINT32			fgOclkP				:	1;

	UINT32			fgOut656			:	1;
	UINT32			fgOutB2rSd			:	1;
	UINT32			fgB2rSdTo656		:	1;
	UINT32			fg656InputClkP		:	1;
	UINT32			fg656Clk27M			:	1;
	UINT32								:	19;
	
	/* 8a, 0x26228 */
	UINT32			u4Cb				:	8;
	UINT32			u4Y					:	8;
	UINT32			u4Cr				:	8;
	UINT32								:	8;
	
	/* 8b, 0x2622C */
	UINT32								:	24;
	UINT32			fgHen				:	1;
	UINT32			fgVen				:	1;
	UINT32								:	6;
	
} HAL_5371_656_FIELD_T;

typedef struct _HAL_5371_656_REG_T
{
	UINT32			au4Reg[HAL_5371_656_REG_NUM];
} HAL_5371_656_REG_T;

#if SUPPORT_DOT_TO_DOT_PATTERN
//extern UINT16 _uDisableFunction;//whether need to change back to original state
extern void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif
#if SUPPORT_SBS_CONVERT
extern void VDP_SBSConVert(UINT8 ucOnOff);
#endif

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void vDrvMainHSharpFOnOff(UINT8 bOnOff);
extern void vDrvMainHSharpROnOff(UINT8 bOnOff);
extern void vDrvMainVSharpOnOff(UINT8 bOnOff);
extern void vDrvETIOnOff(UINT8 bPath, UINT8 bOnOff);

extern void vDrvPipHSharpFOnOff(UINT8 bOnOff);

#if !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389)
extern void _VDP_SetVspTune(UINT32 u4Vsp, UINT32 u4Tune);
extern void _VDP_DispModeTune(UINT32 u4Auto, UINT32 u4Up, UINT32 u4Print);
#endif

extern void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucOnOff);
extern void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode);
extern void vScpipSetForcedHpsFactor(UINT32 u4VdpId, UINT32 u4Factor);

/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _VdpInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpStopCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpFreezeCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpNonlinearCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpNewNonlinearCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpResetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpInfoCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpEnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpDisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdMode (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdAlpha (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdInput (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdOverScan (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdSrcRegion (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdOutRegion (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdBg (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdSrcSize (INT32 i4Argc, const CHAR ** szArgv);
#ifdef SUPPORT_DRAM_DUMP
static INT32 _VdpLastStageDumpCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpScalerDumpCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpScalerDramAverageCmd(INT32 i4Argc, const CHAR ** szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
static INT32 _VdpScalerDumpEnCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif
#endif
static INT32 _VdpLBoxDetectMode (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdpCmdFillScalerDRAM(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdpTestCmdScale (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpTestCmdStress (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpB2R2Test (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpB2RPause(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpB2RDumpRegister(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdpSetCmdNr (INT32 i4Argc, const CHAR ** szArgv);
INT32 _VdpSetCmd121(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdDynamicScaling(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdLBox(INT32 i4Argc, const CHAR ** szArgv);
INT32 _VdpSetForcedHPSD(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdDispMode(INT32 i4Argc, const CHAR ** szArgv);

#if !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389)   //5396 & 5368 don't have this cli command
static INT32 _VdpTuneCmdDispMode(INT32 i4Argc, const CHAR ** szArgv);
#endif

//static INT32 _VdpSetCmdPictureEnhance(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdFrmCrop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetNoOverScan(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetChkPts(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdPattern (INT32 i4Argc, const CHAR ** szArgv);
#if SUPPORT_DOT_TO_DOT_PATTERN
static INT32 _CmdDotToDotPattern (INT32 i4Argc, const CHAR ** szArgv);
#endif
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _CmdPanelVsyncTest(INT32 i4Argc, const CHAR ** szArgv);
#endif

#if SUPPORT_SBS_CONVERT
static INT32 _CmdSBSConVert (INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _VdpSetCmd10BitMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdpSetCmdGame(INT32 i4Argc, const CHAR ** szArgv);


#ifdef CC_656_EN
 
static void _656RegRead (void);
static void _656RegWrite (void);
static INT32 _656InitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656EnableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656DisableCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656RegisterCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656SyncCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656SetCmdMode (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656SetCmdBg (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _656SetCmdInput (INT32 i4Argc, const CHAR ** szArgv);

#endif

#if defined(__linux__)
//static INT32 _VdpSetCmdArg(INT32 i4Argc, const CHAR **aszArgv);
static INT32 _VdpSetCmdForceRescale(INT32 i4Argc, const CHAR **aszArgv);
#endif /* __LINUX__ */

/******************************************************************************
* CLI Command Table
******************************************************************************/
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(VDP)
LINT_RESTORE

#ifdef CC_656_EN
static CLI_EXEC_T _ar656SetCmdTbl[] =
{
#ifdef CC_CLI
 	{"mode",		"m",	_656SetCmdMode, NULL,		"656 mode", CLI_GUEST},
 	{"bg",			NULL, _656SetCmdBg, NULL,			"656 background", CLI_GUEST},
 	{"input",		"i", _656SetCmdInput, NULL,		    "656 input", CLI_GUEST},
#endif/*#ifdef CC_CLI*/

    DECLARE_END_ITEM(),
};

static CLI_EXEC_T _ar656CmdTbl[] =
{
#ifdef CC_CLI
 	{"init",		"i",	_656InitCmd, NULL,			"656 init", CLI_SUPERVISOR},
 	{"enable",		"e",	_656EnableCmd, NULL,		"656 enable", CLI_SUPERVISOR},
 	{"disable",		"d",	_656DisableCmd, NULL,		"656 disable", CLI_SUPERVISOR},
 	{"set",			"s",	NULL, _ar656SetCmdTbl,		"656 parameter set", CLI_GUEST},
 	{"sync",		NULL,	_656SyncCmd, NULL,			"656 do sync", CLI_SUPERVISOR},

 	{"reg",			NULL,	_656RegisterCmd, NULL,		"656 register read", CLI_SUPERVISOR},
#endif/*#ifdef CC_CLI*/ 	

    DECLARE_END_ITEM(),
};
#endif

static CLI_EXEC_T _arVdpTestCmdTbl[] =
{
#ifdef CC_CLI
 	{"scale",		"s",	_VdpTestCmdScale, NULL,		"Vdp scale test", CLI_SUPERVISOR},
 	{"stress",		NULL,	_VdpTestCmdStress, NULL,	"Vdp stress test", CLI_SUPERVISOR},
 	{"b2r2",		"b2r2",	_VdpB2R2Test, NULL,		"B2R SD path test", CLI_SUPERVISOR},
    {"p",           "pause", _VdpB2RPause, NULL,     "B2R set pause", CLI_SUPERVISOR},
    {"dump",		"d",	_VdpB2RDumpRegister,   NULL,   "B2R dump registers", CLI_SUPERVISOR},
#endif/*#ifdef CC_CLI*/ 	

    DECLARE_END_ITEM(),
};

static CLI_EXEC_T _arVdpSetCmdTbl[] =
{
#ifdef CC_CLI
 	{"mode",		"m",	_VdpSetCmdMode, NULL,		"Vdp mode", CLI_SUPERVISOR},
 	{"alpha",		"a",	_VdpSetCmdAlpha, NULL,		"Vdp alpha", CLI_SUPERVISOR},
 	{"in",			"i",	_VdpSetCmdInput, NULL,		"\tVdp input", CLI_SUPERVISOR},
 	{"overscan",    NULL,	_VdpSetCmdOverScan, NULL,	"Vdp overscan", CLI_GUEST},
 	{"srcr",		NULL,	_VdpSetCmdSrcRegion, NULL,	"Vdp source region", CLI_GUEST},
 	{"outr",		NULL,	_VdpSetCmdOutRegion, NULL,	"Vdp output region", CLI_GUEST},
 	{"bg",			NULL,	_VdpSetCmdBg, NULL,			"Vdp background", CLI_GUEST},
 	{"size",		"s",	_VdpSetCmdSrcSize, NULL,	"Vdp source size", CLI_SUPERVISOR},
 	{"nr",		    NULL,	_VdpSetCmdNr, NULL,    	"Vdp NR", CLI_SUPERVISOR},
 	{"121",		    NULL,	_VdpSetCmd121, NULL,    	"Vdp 121 scaling", CLI_SUPERVISOR},
 	{"ds",          NULL,	_VdpSetCmdDynamicScaling, NULL,    "Vdp Dynamic Scaling", CLI_SUPERVISOR},
 	{"lbox",    NULL,	_VdpSetCmdLBox, NULL,    "Vdp LetterBox Detection", CLI_SUPERVISOR},
 	{"lboxmode",    NULL,	_VdpLBoxDetectMode, NULL,    "LetterBox Detection Mode", CLI_SUPERVISOR},
 	{"hps",    NULL,	_VdpSetForcedHPSD, NULL,    "Horizontal Pre-Scaling", CLI_SUPERVISOR},
 	{"10bit",    NULL,	_VdpSetCmd10BitMode, NULL,    "Scpos 10-bit Mode", CLI_SUPERVISOR},
 	{"dispmode",    NULL,	_VdpSetCmdDispMode, NULL,    "Vdp Display Mode", CLI_SUPERVISOR},
#if !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389) //5396 & 5368 don't have this cli command
 	{"tdispmode",   NULL,	_VdpTuneCmdDispMode, NULL,    "Vdp Display Mode Tune", CLI_SUPERVISOR},
#endif 	
 	//{"pe",          NULL,	_VdpSetCmdPictureEnhance, NULL, "Vdp picture enhance (PE)", CLI_SUPERVISOR}, 
 	{"crop",          NULL,	_VdpSetCmdFrmCrop, NULL, "Vdp Frame Cropping", CLI_SUPERVISOR},
 	{"nover",          NULL,	_VdpSetNoOverScan, NULL, "Vdp No Overscan", CLI_SUPERVISOR},
 	{"bpts",          NULL,	_VdpSetChkPts, NULL, "B2R PTS check on PTS value", CLI_SUPERVISOR},
 	{"gamemode",          "game",	_VdpSetCmdGame, NULL, "Vdp Game Mode", CLI_GUEST},
#if defined(__linux__)
 //	{"arg",		"arg",	_VdpSetCmdArg, NULL,	"Vdp video processing argument", CLI_SUPERVISOR},
	{"rescale",	NULL,	_VdpSetCmdForceRescale, NULL,	"Vdp force rescale", CLI_SUPERVISOR},
#endif /* __LINUX__ */
#endif/*#ifdef CC_CLI*/ 	

    DECLARE_END_ITEM(),
};

static CLI_EXEC_T _arVdpCmdTbl[] =
{
#ifdef CC_CLI
 	{"init",		"i",	_VdpInitCmd, NULL,			"Vdp init", CLI_GUEST},
 	{"enable",		"e",	_VdpEnableCmd, NULL,		"Vdp enable", CLI_GUEST},
 	{"disable",		"d",	_VdpDisableCmd, NULL,		"Vdp disable", CLI_GUEST},
 	{"reset",		"r",	_VdpResetCmd, NULL,			"Vdp reset", CLI_GUEST},
 	{"query",		"q",	_VdpQueryCmd, NULL,			"Vdp status query", CLI_GUEST},
 	{"info",		NULL,	_VdpInfoCmd, NULL,			"DTV info query", CLI_GUEST},
 	{"stop",		NULL,	_VdpStopCmd, NULL,			"Vdp stop", CLI_SUPERVISOR},
 	{"freeze",		"f",	_VdpFreezeCmd, NULL,		"Vdp freeze", CLI_SUPERVISOR},
 	{"nonlinear",	"nl",	_VdpNonlinearCmd, NULL,		"Vdp nonlinear", CLI_GUEST},
       {"newnonlinear",   "nnl",   _VdpNewNonlinearCmd, NULL,     "Vdp new nonlinear", CLI_GUEST},        
 	{"set",			"s",	NULL, _arVdpSetCmdTbl,		"\tVdp parameter set", CLI_GUEST},
 	{"test",		"t",	NULL, _arVdpTestCmdTbl,		"Vdp test", CLI_SUPERVISOR},

 	{"pattern",		"pt",	 _CmdPattern, NULL,		    "Vdp Test Pattern", CLI_SUPERVISOR},
	#if SUPPORT_DOT_TO_DOT_PATTERN 	
	{"DotToDotPattern",		"dpt",	 _CmdDotToDotPattern, NULL,		    "Vdp DotToDot Pattern", CLI_SUPERVISOR}, 	
	#endif
    #if defined(CC_MT5365) || defined(CC_MT5395)
    {"PanelVsyncTest",		"pvt",	 _CmdPanelVsyncTest, NULL,		    "Vdp Panel Vsync Test", CLI_SUPERVISOR}, 	
    #endif
    #if SUPPORT_SBS_CONVERT
    {"SBSConVert",		"sbs",	 _CmdSBSConVert, NULL,		    "SBS output", CLI_SUPERVISOR}, 
    #endif
#ifdef SUPPORT_DRAM_DUMP
  	{"lastdump",		"ld",	 _VdpLastStageDumpCmd, NULL,		    "Vdp Last Stage Dump", CLI_SUPERVISOR},
  	{"scalerdump",		"sd",	 _VdpScalerDumpCmd, NULL,		    "Vdp Scaler Dump", CLI_SUPERVISOR},
  	{"scalerAverage",		"sa",	 _VdpScalerDramAverageCmd, NULL, "Vdp Scaler Dram Average", CLI_SUPERVISOR},
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
    {"scalerdumpen",      "sden",    _VdpScalerDumpEnCmd, NULL,           "Vdp Scaler Dump", CLI_SUPERVISOR},
#endif
#endif
 	{"fillrect",		"fillrect",	 _VdpCmdFillScalerDRAM, NULL,		    "Fill scaler dram", CLI_SUPERVISOR},


#ifdef CC_656_EN
 	{"656",		    NULL,	 NULL, _ar656CmdTbl,		"656 output", CLI_SUPERVISOR},
#endif

LINT_SAVE_AND_DISABLE
	CLIMOD_DEBUG_CLIENTRY(VDP),
LINT_RESTORE

#endif/*#ifdef CC_CLI*/ 		

    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Vdp)
{
	"vdp",
	NULL,
	NULL,
	_arVdpCmdTbl,
	"Video plane command",
	CLI_GUEST
};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


#ifdef CC_656_EN

static volatile HAL_5371_656_FIELD_T* const _pr656HwReg = (HAL_5371_656_FIELD_T*) (0x20026400);
static HAL_5371_656_FIELD_T r656SwReg;

#endif


/******************************************************************************
* Local Function
******************************************************************************/
#ifdef CC_CLI
static INT32 _VdpInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	VDP_Init();
	return 0;
}

static INT32 _VdpStopCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	VDP_Stop();	
	return 0;
}

static INT32 _VdpFreezeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucMode;
	UINT32 u4YStart, u4CStart, u4LineSize, u4VActive;

    SRM_FREEZE_MODE_T eFreezeMode;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucFreeze (Freeze Mode)\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucMode = (UCHAR) StrToInt(szArgv[2]);

	if(i4Argc == 4)
	{
	    /* Freeze Mode specified */
	    eFreezeMode = (SRM_FREEZE_MODE_T) StrToInt(szArgv[3]);
	    if(eFreezeMode == SRM_FREEZE_MODE_MPEG)
	    {
	        VDP_FrcSetFreeze(ucVdpId, ucMode);
	    }
#ifdef CC_B2R_SKYPE_SUPPORT
        VDP_GetYCStart(ucVdpId,&u4YStart, &u4CStart, &u4LineSize, &u4VActive);
#else
	    VDP_GetYCStart(&u4YStart, &u4CStart, &u4LineSize, &u4VActive);
#endif
        Printf("%d %d %d %d %d %d\n",u4YStart,(u4CStart-u4YStart),u4CStart,(u4CStart-u4YStart)/2,u4LineSize,u4VActive);
	    //Printf("Y Start:0x%X, Size: 0x%X\n", u4YStart, (u4CStart-u4YStart));
	    //Printf("C Start:0x%X, Size: 0x%X\n", u4CStart, (u4CStart-u4YStart)/2);
	    //Printf("Width:  %d\n", u4LineSize);
	    //Printf("Height: %d\n", u4VActive);
	    return 0;
	}

	if (VDP_SetFreeze(ucVdpId, ucMode) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}

static INT32 _VdpNonlinearCmd (INT32 i4Argc, const CHAR ** szArgv)
{
LINT_SAVE_AND_DISABLE
    UCHAR ucVdpId;
    UCHAR ucOnOff;
    UCHAR ucMode;
    INT32 i4Error;

    i4Error = 0;
    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("AutoMode Arg: ucVdpId ucOnOff 1 ucAdaptiveOnOff u4Slope u4StopFactor\n");
        Printf("Non-AutoMode Arg: ucVdpId ucOnOff 0 u4StartFactor u4Slope u4MidPoint u4StopFactor\n");
        return 0;
    }

    ucVdpId = (UCHAR) StrToInt(szArgv[1]);
    ucOnOff = (UCHAR) StrToInt(szArgv[2]);
    ucMode = 0;
    if (ucOnOff)
    {
        if ((i4Argc < 7) || (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL) || (szArgv[6] == NULL))
        {
            i4Error = 1;
        }
        else
        {
            ucMode = (UCHAR) StrToInt(szArgv[3]);
            if (ucMode == 0)  // manual mode
            {
                if ((i4Argc < 8) || (szArgv[7] == NULL))
                {
                    i4Error = 1;
                }
            }
        }
    }	
    if (i4Error)
    {
        Printf("AutoMode Arg: ucVdpId ucOnOff 1 ucAdaptiveOnOff u4Slope u4StopFactor\n");
        Printf("Non-AutoMode Arg: ucVdpId ucOnOff 0 u4StartFactor u4Slope u4MidPoint u4StopFactor\n");
        return 0;
    }

    if (VDP_SetNonlinear(ucVdpId, ucOnOff) != VDP_SET_OK)
    {
        return -1;
    }

    i4Error = 0;
    if (ucOnOff)
    {
        if (ucMode)  // auto mode
        {
             VDP_AUTO_NONLINEAR_PRM_T rNonlinearPrm;

             rNonlinearPrm.u4Adaptive = StrToInt(szArgv[4]);             
             rNonlinearPrm.u4EndFactor = StrToInt(szArgv[6]);
             rNonlinearPrm.u4Slope = StrToInt(szArgv[5]);   
             #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
             rNonlinearPrm.u1Mode = VDO_NONLINEAR_AUTO;
             #else
             rNonlinearPrm.u1Mode = 0; // unused, fix compiler warning
             #endif
             if (VDP_SetAutoNonLinear((UINT32)ucVdpId, rNonlinearPrm) != VDP_SET_OK)
             {
                 i4Error = -1;
             }
        }
        else // manual mode
        {
            VDP_NONLINEAR_PRM_T rNonlinearPrm;
            
            rNonlinearPrm.u4End = StrToInt(szArgv[7]);
            rNonlinearPrm.u4MiddlePoint = StrToInt(szArgv[6]);
            rNonlinearPrm.u4Slope = StrToInt(szArgv[5]);
            rNonlinearPrm.u4Start = StrToInt(szArgv[4]);
            if (VDP_SetManualNonLinear((UINT32)ucVdpId, rNonlinearPrm) != VDP_SET_OK)
            {
                i4Error = -1;
            }
        }
    }

    return i4Error;
LINT_RESTORE
}

static INT32 _VdpNewNonlinearCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucVdpId;
    UCHAR ucOnOff;
    VDP_NEW_NONLINEAR_PRM_T rNonlPrm;
    
    x_memset(&rNonlPrm, 0, sizeof(VDP_NEW_NONLINEAR_PRM_T));

    if((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Usage: ucVdpId ucOnOff u4SrcMiddle u4OutMiddle\n");
        return 0;
    }

    ucVdpId = StrToInt(szArgv[1]);
    ucOnOff = StrToInt(szArgv[2]);

    if (VDP_SetNonlinear(ucVdpId, ucOnOff) != VDP_SET_OK)
    {
        return -1;
    }
    
    if ((i4Argc == 5) && (szArgv[3] != NULL) && (szArgv[4] != NULL))
    {
        rNonlPrm.u4SrcMiddleRatio = StrToInt(szArgv[3]);
        rNonlPrm.u4OutMiddleRatio = StrToInt(szArgv[4]); 
    }
    else
    {
        return 0;
    }
    
    return VDP_SetNewNonLinear(ucVdpId, rNonlPrm);    
}

static INT32 _VdpResetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	VDP_Reset(VDP_1);	
	VDP_Reset(VDP_2);	
	return 0;
}

static INT32 _VdpQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	VDP_QueryStatus();	
	return 0;
}

static INT32 _VdpInfoCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	VDP_PrintDtvInfo();	
	return 0;
}

static INT32 _VdpEnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId)
	UCHAR ucVdpId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucVdpId\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);

	if (VDP_SetEnable(ucVdpId, 1) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}

static INT32 _VdpDisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId)
	UCHAR ucVdpId;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucVdpId\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);

	if (VDP_SetEnable(ucVdpId, 0) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}


static INT32 _VdpSetCmdMode (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId, UCHAR ucMode)
	UCHAR ucVdpId;
	UCHAR ucMode;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucMode\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucMode = (UCHAR) StrToInt(szArgv[2]);

	if (VDP_SetMode(ucVdpId, ucMode) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}

static INT32 _VdpSetCmdAlpha (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId, UCHAR ucAlphaValue)
	UCHAR ucVdpId;
	UCHAR ucAlphaValue;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucAlphaValue\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucAlphaValue = (UCHAR) StrToInt(szArgv[2]);

	if (PMX_SetAlpha(ucVdpId, (INT16)ucAlphaValue) == VDP_SET_OK)
	{
		return 0;
	}
    
	return -1;
}

static INT32 _VdpSetCmdInput (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId, UCHAR ucFbgId, UCHAR ucPort)
	UCHAR ucVdpId;
	UCHAR ucEsId;
	UCHAR ucPort;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucEsId ucPort\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEsId = (UCHAR) StrToInt(szArgv[2]);
	ucPort = 0;
	if ((i4Argc >= 4) && (szArgv[3] != NULL))
	{
		ucPort = (UCHAR) StrToInt(szArgv[3]);
	}

	if (VDP_SetInput(ucVdpId, ucEsId, ucPort) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}

static INT32 _VdpSetCmdOverScan (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg : (UCHAR ucVdpId, UCHAR ucSrcFullRegion, VDP_REGION_T rSrcRegion)
	UCHAR ucVdpId;
	VDP_OVERSCAN_REGION_T rSrcRegion;

	if ((i4Argc < 6) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId T B L R\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	rSrcRegion.u4Top = StrToInt(szArgv[2]);
	rSrcRegion.u4Bottom = StrToInt(szArgv[3]);
	rSrcRegion.u4Left = StrToInt(szArgv[4]);
	rSrcRegion.u4Right = StrToInt(szArgv[5]);
	
	if (VDP_SetOverScan(ucVdpId, rSrcRegion) == VDP_SET_OK)
	{
		return 0;
	}

	return 0;
}

static INT32 _VdpSetCmdSrcRegion (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg : (UCHAR ucVdpId, UCHAR ucSrcFullRegion, VDP_REGION_T rSrcRegion)
	UCHAR ucVdpId;
	UCHAR ucSrcFullRegion;
	VDP_REGION_T rSrcRegion;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucSrcFullRegion\n");
		Printf("Arg: ucVdpId X Y W H\n");
		return 0;
	}

	if ((i4Argc >= 6) && (szArgv[3] != NULL) && (szArgv[4] != NULL) && (szArgv[5] != NULL))
	{
		ucVdpId = (UCHAR) StrToInt(szArgv[1]);
		ucSrcFullRegion = 0;
		rSrcRegion.u4X = StrToInt(szArgv[2]);
		rSrcRegion.u4Y = StrToInt(szArgv[3]);
		rSrcRegion.u4Width = StrToInt(szArgv[4]);
		rSrcRegion.u4Height = StrToInt(szArgv[5]);
	}
	else
	{
		ucVdpId = (UCHAR) StrToInt(szArgv[1]);
		ucSrcFullRegion = 1;

		rSrcRegion.u4X = 0;
		rSrcRegion.u4Y = 0;
		rSrcRegion.u4Width = 0;
		rSrcRegion.u4Height = 0;
	}
	
	if (VDP_SetSrcRegion(ucVdpId, ucSrcFullRegion, rSrcRegion) == VDP_SET_OK)
	{
		return 0;
	}

	return 0;
}

static INT32 _VdpSetCmdOutRegion (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg : (UCHAR ucVdpId, UCHAR ucOutFullRegion, VDP_REGION_T rOutRegion)
	UCHAR ucVdpId;
	UCHAR ucOutFullRegion;
	VDP_REGION_T rOutRegion;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucOutFullRegion\n");
		Printf("Arg: ucVdpId X Y W H\n");
		return 0;
	}

	if ((i4Argc >= 6) && (szArgv[3] != NULL) && (szArgv[4] != NULL) && (szArgv[5] != NULL))
	{
		ucVdpId = (UCHAR) StrToInt(szArgv[1]);
		ucOutFullRegion = 0;
		rOutRegion.u4X = StrToInt(szArgv[2]);
		rOutRegion.u4Y = StrToInt(szArgv[3]);
		rOutRegion.u4Width = StrToInt(szArgv[4]);
		rOutRegion.u4Height = StrToInt(szArgv[5]);
	}
	else
	{
		ucVdpId = (UCHAR) StrToInt(szArgv[1]);
		ucOutFullRegion = 1;

		rOutRegion.u4X = 0;
		rOutRegion.u4Y = 0;
		rOutRegion.u4Width = 0;
		rOutRegion.u4Height = 0;
	}
	
	if (VDP_SetOutRegion(ucVdpId, ucOutFullRegion, rOutRegion) == VDP_SET_OK)
	{
		return 0;
	}

	return 0;
}

static INT32 _VdpSetCmdBg (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId, UINT32 u4BgColor)
	UCHAR ucVdpId;
	UINT32 u4BgColor;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4MuteColor\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	u4BgColor = StrToInt(szArgv[2]);

	if (VDP_SetBg(ucVdpId, u4BgColor) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}


static INT32 _VdpSetCmdSrcSize (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR ucVdpId, UCHAR ucColorMode)
	UCHAR ucVdpId;
	UINT32 u4SrcWidth;
	UINT32 u4SrcHeight;

	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucVdpId u4SrcWidth u4SrcHeight\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	u4SrcWidth = StrToInt(szArgv[2]);
	u4SrcHeight = StrToInt(szArgv[3]);

	if (VDP_SetSrcSize(ucVdpId, u4SrcWidth, u4SrcHeight) == VDP_SET_OK)
	{
		return 0;
	}

	return -1;
}


#ifdef SUPPORT_DRAM_DUMP
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
UINT8 _u1LastOnOff = 0;
#endif
static INT32 _VdpLastStageDumpCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
    	UCHAR ucVdpId;
    	UINT32 u4Xoffset;
    	UINT32 u4Yoffset;
    	UINT32 u4R=0, u4B=0, u4G=0;
        UINT8 u1OnOff;
    	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    	{
    		Printf("Arg: ucVdpId ucOnOff\n");
    		return 0;
    	}
    	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
        u1OnOff = StrToInt(szArgv[2]);
        if(u1OnOff != _u1LastOnOff)
        {
            if (ucVdpId != SV_VP_MAIN)
        	{
        	    Printf("Do not Support Last Stage Dump for Sub\n");
        	    return -1;
        	}
            if(u1OnOff)
            {
                if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL))
        	      {
            		Printf("Arg: ucVdpId ucOnOff u4Xoffset u4Yoffset\n");
            		return 0;
        	      }
                
            }
        	u4Xoffset = StrToInt(szArgv[3]);
        	u4Yoffset = StrToInt(szArgv[4]);
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if(_arScalePrm[VDP_1].u4DispMode != VDP_SCPOS_DISPMODE_OFF)
            {
                Printf("Only support dram mode\n");
        	    return -1;
            }
            #endif
            if(u1OnOff)
            {
            	bInitLastStageDump();
                _u1LastOnOff = u1OnOff;
            	if (bGetLastStageDRAM(u4Xoffset, u4Yoffset, &u4R, &u4B, &u4G) == VDP_SET_OK)
            	{
                    Printf("u4R,u4B,u4G,(%d,%d,%d)\n",u4R,u4B,u4G);
                  // 	bReleaseLastStageDump();
            		return 0;
            	}
                return -1;
            }
            else
            {
                bReleaseLastStageDump();
                _u1LastOnOff = u1OnOff;
                return 0;
            }
       }
        Printf("Need to close/open dump first \n");
    	return -1;
    #else
        UCHAR ucVdpId;
    	UINT32 u4Xoffset;
    	UINT32 u4Yoffset;
    	UINT32 u4R, u4B, u4G;

    	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    	{
    		Printf("Arg: ucVdpId u4Xoffset u4Yoffset\n");
    		return 0;
    	}

    	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
    	if (ucVdpId != SV_VP_MAIN)
    	{
    	    Printf("Do not Support Last Stage Dump for Sub\n");
    	    return -1;
    	}
    	u4Xoffset = StrToInt(szArgv[2]);
    	u4Yoffset = StrToInt(szArgv[3]);

    	bInitLastStageDump();
    	if (bGetLastStageDRAM(u4Xoffset, u4Yoffset, &u4R, &u4B, &u4G) == VDP_SET_OK)
    	{
           	bReleaseLastStageDump();
    		return 0;
    	}

    	return -1;
    #endif
}


#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
static INT32 _VdpScalerDumpEnCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucOnOff;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucOnOff\n");
        return 0;
	}
    
	ucOnOff = (UCHAR) StrToInt(szArgv[1]);    
    fgForceScalerDramDump(SV_VP_MAIN, ucOnOff);

    return 0;
}
#endif  
static INT32 _VdpScalerDumpCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UINT32 u4Xoffset;
	UINT32 u4Yoffset;
    UINT32 u4R=0, u4B=0, u4G=0;

	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucVdpId u4Xoffset u4Yoffset\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	if (ucVdpId != SV_VP_MAIN)
	{
	    Printf("Do not Support Scaler Dump for Sub\n");
	    return -1;
	}
	u4Xoffset = StrToInt(szArgv[2]);
	u4Yoffset = StrToInt(szArgv[3]);

	bInitScalerDump();
	if (bGetScalerDRAM(u4Xoffset, u4Yoffset, &u4R, &u4B, &u4G) == VDP_SET_OK)
	{
        Printf("(%d, %d) R=%d, B=%d, G=%d\n", u4Xoffset, u4Yoffset, u4R, u4B, u4G);
       	bReleaseScalerDump();
		return 0;
	}
	return -1;
}

static INT32 _VdpScalerDramAverageCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret;
	UINT32 u4X, u4Y,  u4Width, u4Height;
	UINT32 u4R, u4B, u4G;

	if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: u2Xoffset u2Yoffset u2Width u2Height\n");
		return 0;
	}

	u4X = StrToInt(szArgv[1]);
	u4Y = StrToInt(szArgv[2]);
	u4Width = StrToInt(szArgv[3]);
	u4Height = StrToInt(szArgv[4]);
    
	bInitScalerDump();
	i4Ret= bGetScalerDRAMAverage(u4X, u4Y, u4Width, u4Height, &u4R, &u4B, &u4G); 
    bReleaseScalerDump();

	if (i4Ret == VDP_SET_OK)
	{
		Printf("Scalar average data: Pos(%d,%d),Size(%dx%d),Data(YG:%d, UB:%d, VR:%d)\n", (UINT16)u4X, (UINT16)u4Y, (UINT16)u4Width, (UINT16)u4Height, (UINT16)u4G,(UINT16)u4B,(UINT16)u4R);
		return 0;
	}
	else
	{
	    Printf("Get scalar data fail \n");
	    return -1;
	}	
}
#endif

static INT32 _VdpCmdFillScalerDRAM(INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc == 2) && ((szArgv != NULL) && (szArgv[1] != NULL)))
    {
    	  // internal pattern
    	  VDP_SetScalerPattern(0, StrToInt(szArgv[1]));
    	  return 0;
    }
    else if ((i4Argc < 6) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
		Printf("Arg: u4X u4Y u4Width u4Height u4GBRYCbCr\n");
 		return 0;
    }
    VDP_SetScalerDRAM(0, StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]), StrToInt(szArgv[4]), StrToInt(szArgv[5]));

    return 0;
}

static INT32 _VdpB2R2Test (INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("B2R SD path test\n");
    VDP_TestB2RSdPath();
    return 0;
}

static INT32 _VdpB2RPause(INT32 i4Argc, const CHAR ** szArgv)
{
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: 1/0 (pause/resume)\n");
		return 0;
	}
    VDP_SetPauseMm(0, (BOOL)StrToInt(szArgv[1]));
    return 0;
}

static INT32 _VdpB2RDumpRegister(INT32 i4Argc, const CHAR ** szArgv)
{
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ON(1)/OFF(0)\n");
		return 0;
	}
    VDP_B2RDumpRegister((BOOL)StrToInt(szArgv[1]));
    return 0;
}

static INT32 _VdpTestCmdScale (INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Case;
    UINT32 u4Step;
    VDP_REGION_T rRegion;
    VDP_OVERSCAN_REGION_T rSrcRegion;
    UINT32 u4VdpId;
    UINT32 u4Count;
    UINT32 u4Index;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
		Printf("Arg: u4VdpId u4Count\n");
 		return 0;
    }

    u4VdpId = StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]); 

    if(u4VdpId > VDP_MAX_NS)
    {
        Printf("Arg: u4VdpId should be < %d\n",VDP_MAX_NS);
    }
    
    u4Case = 0;
    u4Step = 0;
    u4Index = 0;

    while(u4Index < u4Count)
    {        
        x_thread_delay(500);
    
        if (u4Case == 0)    //u4Case == 0 => test Overscan
        {
            if (u4Step == 0)
            {
                Printf("Round(%d/%d)\n",u4Index+1,u4Count);
                Printf("OverScan Test\n");
            
            	if (VDP_SetOutRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

            	if (VDP_SetSrcRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

                rSrcRegion.u4Top= u4Step;
                rSrcRegion.u4Bottom = u4Step;
                rSrcRegion.u4Left = u4Step;
                rSrcRegion.u4Right= u4Step;

            	if (VDP_SetOverScan(u4VdpId, rSrcRegion) == VDP_SET_OK)
            	{
            	}
    
                u4Step += 25;
            }       
            else
            {
                rSrcRegion.u4Top= u4Step;
                rSrcRegion.u4Bottom = u4Step;
                rSrcRegion.u4Left = u4Step;
                rSrcRegion.u4Right= u4Step;

            	if (VDP_SetOverScan(u4VdpId, rSrcRegion) == VDP_SET_OK)
            	{
            	}

                u4Step += 25;

                if (u4Step >= 475)
                {
                    u4Case = 1;
                    u4Step = 0;
                }
            }
        }
        else
        if (u4Case == 1)    //u4Case == 1 => test Source Region
        {
            if (u4Step == 0)
            {
                Printf("Source Region Test\n");
            
            	if (VDP_SetOutRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

            	if (VDP_SetSrcRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

                rSrcRegion.u4Top= 0;
                rSrcRegion.u4Bottom = 0;
                rSrcRegion.u4Left = 0;
                rSrcRegion.u4Right= 0;

            	if (VDP_SetOverScan(u4VdpId, rSrcRegion) == VDP_SET_OK)
            	{
            	}
    
                u4Step += 25;
            }       
            else
            {
                rRegion.u4Height = 1000 - u4Step;
                rRegion.u4Width = 1000 - u4Step;
                rRegion.u4X = u4Step >> 1;
                rRegion.u4Y = u4Step >> 1;

            	if (VDP_SetSrcRegion(u4VdpId, 0, rRegion) == VDP_SET_OK)
            	{
            	}

                u4Step += 25;

                if (u4Step >= 950)
                {
                    u4Case = 2;
                    u4Step = 0;
                }
            }
        }
        else
        if (u4Case == 2)    //u4Case == 2 => test output region
        {
            if (u4Step == 0)
            {
                Printf("Output Region Test\n");
            
            	if (VDP_SetOutRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

            	if (VDP_SetSrcRegion(u4VdpId, 1, rRegion) == VDP_SET_OK)
            	{
            	}

                rSrcRegion.u4Top= 0;
                rSrcRegion.u4Bottom = 0;
                rSrcRegion.u4Left = 0;
                rSrcRegion.u4Right= 0;

            	if (VDP_SetOverScan(u4VdpId, rSrcRegion) == VDP_SET_OK)
            	{
            	}
    
                u4Step += 25;
            }       
            else
            {
                rRegion.u4Height = 1000 - u4Step;
                rRegion.u4Width = 1000 - u4Step;
                rRegion.u4X = u4Step >> 1;
                rRegion.u4Y = u4Step >> 1;

            	if (VDP_SetOutRegion(u4VdpId, 0, rRegion) == VDP_SET_OK)
            	{
            	}

                u4Step += 25;

                if (u4Step >= 950)
                {
                    u4Case = 0;
                    u4Step = 0;
                    u4Index++;
                }
            }
        }        
    }

    u4Count = 0;
    u4Index = 0;

    return 0;
#else
    UNUSED(i4Argc);
    UNUSED(szArgv);
	return -1;
#endif	
}

static INT32 _VdpTestCmdStress (INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    while(1)
    {
        CLI_Parser("n.ms av1");
        x_thread_delay(2500);
        CLI_Parser("n.ms dtv1");
        x_thread_delay(2500);
    }

//    return 0;

#else
    UNUSED(i4Argc);
    UNUSED(szArgv);
	return -1;
#endif	
}

static INT32 _VdpSetCmdNr (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucEn;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEn = (UCHAR) StrToInt(szArgv[2]);

    UNUSED(ucVdpId);
    vDrvNRSetForceBypass(!ucEn);

	return 0;
}

INT32 _VdpSetCmd121 (INT32 i4Argc, const CHAR ** szArgv)
{

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)

	UCHAR ucVdpId;
	UCHAR ucEn;
	UCHAR ucHPos;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En\n");
		Printf("Arg: ucVdpId u4En u2HPos(0:LEFT, 1:MIDDLE, 2:RIGHT)\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEn = (UCHAR) StrToInt(szArgv[2]);
	ucHPos= (UCHAR) StrToInt(szArgv[3]);
	
    if(i4Argc == 4)
    {
    	if(ucHPos <3)
    	{
    		VDP_Set121MapHPos(ucVdpId, ucHPos);
    	}
    	else
    	{
    		Printf("Display window H position NOT support\n");
    		Printf("Arg: ucVdpId u4En u2HPos(0:LEFT, 1:MIDDLE, 2:RIGHT)\n");
    	}
    }
    VDP_Set121Map(ucVdpId, ucEn);
    
#else
       UCHAR ucVdpId;
	UCHAR ucEn;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEn = (UCHAR) StrToInt(szArgv[2]);
	
       VDP_Set121Map(ucVdpId, ucEn);

#endif
	return 0;
}

static INT32 _VdpSetCmdDynamicScaling(INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucOnOff;
	UINT32 u4ResponseTime;

	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucVdpId ucOnOff u4ResponseTime\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucOnOff = (UCHAR) StrToInt(szArgv[2]);
	u4ResponseTime = (UINT32) StrToInt(szArgv[3]);

    VDP_SetDynamiceScaler(ucVdpId, ucOnOff, u4ResponseTime);

	return 0;
}

static INT32 _VdpSetCmdLBox (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucEn;
	UCHAR ucFixed;

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId u4En u4Fixed\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucEn = (UCHAR) StrToInt(szArgv[2]);

    ucFixed = 0;
    if (i4Argc >= 4)
    {
    	ucFixed = (UCHAR) StrToInt(szArgv[3]);
    }

    VDP_SetLBoxDetection(ucVdpId, ucEn, ucFixed);

	return 0;
}

static INT32 _VdpLBoxDetectMode (INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UCHAR ucPeriod;
	UCHAR ucBound;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucVdpId ucPeriod\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucPeriod = (UCHAR) StrToInt(szArgv[2]);
       ucBound = (UCHAR) StrToInt(szArgv[3]);
       
       VDP_LBoxDetectMode(ucVdpId, ucPeriod, ucBound);

	return 0;
}

INT32 _VdpSetForcedHPSD(INT32 i4Argc, const CHAR ** szArgv)
{
#if !defined(CC_MT5398) && !defined(CC_MT5880) && !defined(CC_MT5860)
    UCHAR ucVdpId;
    UINT32 u4Factor;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
		Printf("Arg: ucVdpId u4Factor\n");
		return 0;
    }

    ucVdpId = (UCHAR) StrToInt(szArgv[1]);
    u4Factor = (UINT32) StrToInt(szArgv[2]);

    vScpipSetForcedHpsFactor(ucVdpId, u4Factor); 
#endif
    
    return 0;
}

static INT32 _VdpSetCmd10BitMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Mode;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: ucVdpId u4Mode\n");
        Printf("0: FW auto, 1: 8 bit mode, 2: 10 bit mode\n");
        return 0;
    }

    u4VdpId = (UINT32) StrToInt(szArgv[1]);
    u4Mode = (UINT32) StrToInt(szArgv[2]);
    
    if (u4Mode == 2)
    {
        u4Mode = VDP_SCPOS_10BitMode;
    }
    else if (u4Mode == 1)
    {
        u4Mode = VDP_SCPOS_8BitMode;
    }
    else 
    {
        u4Mode = 0;
    }
#if defined(CC_MT5365)||defined(CC_MT5395) || defined(CC_MT5396)
    VDP_SetScaler10BitMode(u4VdpId, u4Mode);
#else
    _VDP_Set10bitMode(u4VdpId, u4Mode);
#endif

    return 0;
}

static INT32 _VdpSetCmdGame(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Mode;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: ucVdpId u4OnOff\n");
        return 0;
    }

    u4VdpId = (UINT32) StrToInt(szArgv[1]);
    u4Mode = (UINT32) StrToInt(szArgv[2]);
    if (u4Mode)
    {
        u4Mode = 1;
    }
    else
    {
        u4Mode = 0;
    }

    VDP_SetGameMode(u4VdpId, (UINT8)u4Mode);

    return 0;

}

static INT32 _VdpSetCmdDispMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucVdpId;
    UCHAR ucMode;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        Printf("Arg: ucVdpId u4Mode (0: normal mode; )\n");
        return 0;
    }

    ucVdpId = (UCHAR) StrToInt(szArgv[1]);
    ucMode = (UCHAR) StrToInt(szArgv[2]);

    VDP_SetDispmode(ucVdpId, ucMode);
    return 0;
}

#if !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389)     //5396 & 5368 don't have this cli command
static INT32 _VdpTuneCmdDispMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Auto;
    UINT32 u4Up;
    UINT32 u4Step;

    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
		Printf("Arg: u4Auto=1 u4UpDown u4Step\n");
		Printf("Arg: u4Auto=0 u4VSP u4Tune\n");
		return 0;
    }

    u4Up = 0;
    u4Step = 1;

    u4Auto = (UINT32) StrToInt(szArgv[1]);
    u4Up = (UINT32) StrToInt(szArgv[2]);
    u4Step = (UINT32) StrToInt(szArgv[3]);

    if (u4Auto)
    {
        if (u4Step < 1)
        {
           u4Step = 1;
        }
        if (u4Up == 0)
        {
           u4Step |= 0x80000000;
        }
    }
    
    if (u4Auto != 0)
    {
        UINT32 u4Idx;
        UINT32 u4Loop;
    
        // Set Tune = 0
   //     HAL_REG32(SCPOS_BASE +   0x30) &= 0xFFC0FFFF;
//        HAL_REG32(SCPOS_BASE +  0x30) &= 0xFFC0FFFF;
//        HAL_REG32(SCPOS_BASE +  0x164) &= 0x0FFFFFFF;

        u4Loop = 0x400 / (u4Step & 0x7fffffff);

        for (u4Idx = 0; u4Idx < u4Loop; u4Idx++)
        {
            _VDP_DispModeTune(0, u4Step, 0);    // Set VSP Tune
            x_thread_delay(300);  
            _VDP_DispModeTune(0, 0, 1);   // Get status
        }    
    }
    else
    {
//        VDP_TuneDispMode(0, u4Up, u4Step);
        _VDP_SetVspTune(u4Up, u4Step);
        x_thread_delay(300);
        _VDP_DispModeTune(0, 0, 1);
    }

	return 0;
}
#endif

static INT32 _VdpSetCmdFrmCrop(INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;
	UINT32 u4FrmCropTop;
	UINT32 u4FrmCropBtm;	
    UINT32 u4Ret;	

	if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
	{
		Printf("Arg: ucVdpId u4FrmCropTop u4FrmCropBtm\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	u4FrmCropTop = (UINT32) StrToInt(szArgv[2]);
	u4FrmCropBtm = (UINT32) StrToInt(szArgv[3]);	

    u4Ret = VDP_SetFrmCrop(ucVdpId, u4FrmCropTop, u4FrmCropBtm);    

    UNUSED(u4Ret);

	return 0;
}



static INT32 _VdpSetNoOverScan(INT32 i4Argc, const CHAR ** szArgv)
{
	UCHAR ucVdpId;	
	UCHAR ucNoOverScan;
    UINT32 u4Ret;	

	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: ucVdpId ucNoOverScan\n");
		return 0;
	}

	ucVdpId = (UCHAR) StrToInt(szArgv[1]);
	ucNoOverScan = (UINT32) StrToInt(szArgv[2]);

    u4Ret = VDP_SetNoOverScan(ucVdpId, ucNoOverScan);    

    UNUSED(u4Ret);

	return 0;
}

static INT32 _VdpSetChkPts(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Pts;
    BOOL fgTriggerEnc = FALSE;
    
    if (i4Argc < 2)
    {
        Printf("Arg: PTS\n");
        return 0;
    }

    if(i4Argc == 3)
    {
        fgTriggerEnc = (BOOL) StrToInt(szArgv[2]);
        VDP_SetEncTrigger(fgTriggerEnc);
    }
    
    u4Pts   = (UINT32) StrToInt(szArgv[1]);
    VDP_SetChkPTS(u4Pts);

    return 0;
}

static INT32 _CmdPattern (INT32 i4Argc, const CHAR ** szArgv)
{
LINT_SAVE_AND_DISABLE
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
    UINT32 u4Arg4;

    u4Arg1 = 0;
    u4Arg2 = 0;
    u4Arg3 = 0;
    u4Arg4 = 0;

    UNUSED(u4Arg4);

    if (i4Argc >= 2)
    {
    	u4Arg1 = (UINT32) StrToInt(szArgv[1]);
    }

    if (i4Argc >= 3)
    {
    	u4Arg2 = (UINT32) StrToInt(szArgv[2]);
    }

    if (i4Argc >= 4)
    {
    	u4Arg3 = (UINT32) StrToInt(szArgv[3]);
    }

    if (i4Argc >= 5)
    {
    	u4Arg4 = (UINT32) StrToInt(szArgv[4]);
    }

    VDP_Pattern(u4Arg1, u4Arg2, u4Arg3,u4Arg4);

    return 0;
LINT_RESTORE
}

#if SUPPORT_DOT_TO_DOT_PATTERN
static INT32 _CmdDotToDotPattern (INT32 i4Argc, const CHAR ** szArgv)
{
LINT_SAVE_AND_DISABLE    
	UINT8 u4Arg1;    
	UINT32 u4Arg2;       
	u4Arg1 = 0;    
	u4Arg2 = 0;
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )	
	{		
		Printf("Arg: ucOnOff  Index       Pattern Index 0~9\n");		
		//Printf("Arg: ucOnOff  Index       Pattern Index 0~9\n");	
		Printf("\n  0:     	1line,3line the same   0-W-0-W,     2line,4line the same   W-0-W-0-W 	Black Withe dot  \n");	
		Printf("  1:     	1line,3line the same   0-G-0-G,     2line,4line the same   G-0-G-0 		Black Green dot \n");	
		Printf("  2:     	1line,3line the same   0-B-0-B,     2line,4line the same   B-0-B-0 		Black Blue dot \n");	
		Printf("  3:     	1line,3line the same   0-R-0-R,     2line,4line the same   R-0-R-0		 Black Red dot \n");	
		
		Printf("  4:     	1line,2line the same   0-G-0-G,     3line 4line the same   G-0-G-0 		Black Green dot \n");	
		Printf("  5:      	1line,2line the same   0-B-0-B,     3line 4line the same   B-0-B-0  		Black Blue dot \n");	
		Printf("  6:     	1line,2line the same   0-R-0-R,     3line 4line the same   R-0-R-0 		Black Red dot \n");	
		
		Printf("  7:		1line   0-G-0-G,     2line 3line the same   G-0-G-0 ,     4line 5line the same   0-G-0-G     Black Green dot\n");	
		Printf("  8:		1line   0-B-0-B,     2line 3line the same   B-0-B-0 ,     4line 5line the same   0-B-0-B     Black Blue dot \n");	
		Printf("  9:		1line   0-R-0-R,     2line 3line the same   R-0-R-0 ,      4line 5line the same   0-R-0-R     Black Red dot \n");	
		Printf("  Default:      1line   0-G-0-G,     2line 3line the same   G-0-G-0 ,     4line 5line the same   0-G-0-G    Black Green  dot \n");	
		return 0;	
	}	   
	u4Arg1 = (UINT8) StrToInt(szArgv[1]);	   
	u4Arg2= (UINT32) StrToInt(szArgv[2]);
	if((u4Arg1==1)&&(i4Argc == 3))   	
	{		 
		VDP_DotToDotPattern(u4Arg1, u4Arg2);	       
		//_uDisableFunction=1;	   
		return 0;   	
	}	
	//else if((u4Arg1==0)&&(_uDisableFunction==1)) //do not change back to original state when  _uDisableFunction=0
	else if(u4Arg1==0) //do not change back to original state when  _uDisableFunction=0	
	{   	    
		VDP_DotToDotPattern(0, 0);		
		return 0;   	
	}	
	else	
	{	     
	       return 0;	
	}	
LINT_RESTORE
}
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)

static INT32 _CmdPanelVsyncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    LINT_SAVE_AND_DISABLE 
    UINT8 u4Arg1; 
    UINT8 u4Char;
    
    u4Arg1 = (UINT8) StrToInt(szArgv[1]);	   
    if((i4Argc == 2) && (u4Arg1 == 1))
	{ 
	    UINT32 u4Pos_up;
        UINT32 u4Pos_down;
        UINT32 u4Pos_old;
        UINT32 u4FrontPorch;
        UINT32 u4BackPorch;
        UINT32 u4Vtotal;
        UINT8 u4Vs_width;
        UINT8 u4Vs_delsel2;
        u4Pos_old = RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS);
        u4Pos_up = u4Pos_old;
        u4Pos_down = u4Pos_old;
        u4Vtotal = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN);
        u4Vs_width = RegReadFldAlign(OUTSTG_OS_18, OS_18_VS_WIDTH) + 1;
        u4Vs_delsel2 = RegReadFldAlign(OUTSTG_OS_75, OS_75_VSYNC_DELSEL2);
        Printf("Begin to panel vsync test \n");
        Printf("Please enter 'u' when do up test \n");
        Printf("Please enter 'd' when do down test \n");
        while(1)
        {
            u4Char = SerPollGetChar();
            if((u4Char== 'u') || (u4Char== 'U'))
            {
                if(u4Char== 'u')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
                u4Pos_up += 1;
                u4BackPorch = 1024 - u4Vs_width + u4Vs_delsel2 - u4Pos_up;
                u4FrontPorch = (u4Vtotal + 1 )- PANEL_GetPanelHeight() - u4BackPorch - u4Vs_width;
                Printf("Current V_POS is /************ (%d) ***********/ \n",u4Pos_up);
                Printf("Current u4Vs_delsel2 is (%d) \n",u4Vs_delsel2);
                Printf("Current u4Vtotal is (%d) \n",u4Vtotal);
                Printf("Current u4Vs_width is (%d) \n",u4Vs_width);
                Printf("Current Back Porch is (%d) \n",u4BackPorch);
                Printf("Current Front Porch is (%d) \n",u4FrontPorch);
                
                vRegWriteFldAlign(OUTSTG_OS_18, u4Pos_up, OS_18_V_POS);
            }
            else if((u4Char== 'd') || (u4Char== 'D'))
            {
                if(u4Char== 'd')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
                u4Pos_down -= 2;
                u4BackPorch = 1024 - u4Vs_width + u4Vs_delsel2 - u4Pos_down;
                u4FrontPorch = (u4Vtotal + 1 )- PANEL_GetPanelHeight() - u4BackPorch - u4Vs_width;
                Printf("Current V_POS is /************ (%d) ***********/ \n",u4Pos_down);
                Printf("Current u4Vs_delsel2 is (%d) \n",u4Vs_delsel2);
                Printf("Current u4Vtotal is (%d) \n",u4Vtotal);
                Printf("Current u4Vs_width is (%d) \n",u4Vs_width);
                Printf("Current Back Porch is (%d) \n",u4BackPorch);
                Printf("Current Front Porch is (%d) \n",u4FrontPorch);
                
                vRegWriteFldAlign(OUTSTG_OS_18, u4Pos_down, OS_18_V_POS);
            }
            else if((u4Char==13) || (u4Char==10)) // ncstool, enter
            {
               continue;
            }
            else if(u4Char==27) // escape, end of auto pattern
	        {
	          Printf("Quit Panel Vsync Test \n");
              vRegWriteFldAlign(OUTSTG_OS_18, u4Pos_old, OS_18_V_POS);
	     	  return 0;
	        }	 
        }
	}
    else if((i4Argc == 2) && (u4Arg1 == 0))
    {
        Printf("Turn off Panel Vsync Test\n");	
        return 0;
    }
    else
    {
	     Printf("Arg: ucOnOff  (Panel Vsync Test)\n");	
	     return 0;
    }     		
    
    LINT_RESTORE

}
#endif


#if SUPPORT_SBS_CONVERT
static INT32 _CmdSBSConVert (INT32 i4Argc, const CHAR ** szArgv)
{
LINT_SAVE_AND_DISABLE    
	UINT8 u4Arg1;       
	u4Arg1 = 0;    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )	
	{		
		Printf("Arg: ucOnOff (SBSOoutput Mode)\n");		
		return 0;	
	}	   
	u4Arg1 = (UINT8) StrToInt(szArgv[1]);	   
	VDP_SBSConVert(u4Arg1);	       
	  return 0;		
LINT_RESTORE
}
#endif


#ifdef CC_656_EN
 
void _656RegRead (void)
{
	UINT32 u4RegIdx;

	HAL_5371_656_REG_T* prSwReg;
	HAL_5371_656_REG_T* prHwReg;

	prHwReg = (HAL_5371_656_REG_T*) _pr656HwReg;
	prSwReg = (HAL_5371_656_REG_T*) (&r656SwReg);

	for (u4RegIdx = 0; u4RegIdx < HAL_5371_656_REG_NUM; u4RegIdx++)
	{
		prSwReg->au4Reg[u4RegIdx] = prHwReg->au4Reg[u4RegIdx];
	}
}

void _656RegWrite (void)
{
	UINT32 u4RegIdx;

	HAL_5371_656_REG_T* prSwReg;
	HAL_5371_656_REG_T* prHwReg;

	prHwReg = (HAL_5371_656_REG_T*) _pr656HwReg;
	prSwReg = (HAL_5371_656_REG_T*) (&r656SwReg);

	for (u4RegIdx = 1; u4RegIdx < HAL_5371_656_REG_NUM; u4RegIdx++)
	{
		prHwReg->au4Reg[u4RegIdx] = prSwReg->au4Reg[u4RegIdx];
	}
	prHwReg->au4Reg[0] = prSwReg->au4Reg[0];
	// ̫~U 0x26200, í
}

static INT32 _656InitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	_656RegRead();

	r656SwReg.fgOut656 = 0;

	_656RegWrite();

 	return 0;
}

static INT32 _656EnableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	Printf("Do Nothing\n");
	return 0;
}

static INT32 _656DisableCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	Printf("Do Nothing\n");
	return 0;
}

static INT32 _656RegisterCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	CLI_Parser_Arg("r 0x20026400 %d", (HAL_5371_656_REG_NUM * 4));

	return 0;
}

static INT32 _656SyncCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	HAL_5371_656_REG_T* prHwReg;

	prHwReg = (HAL_5371_656_REG_T*) _pr656HwReg;

	prHwReg->au4Reg[1] = 0;

	return 0;
}

static INT32 _656SetCmdMode (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR uc656Id, UCHAR ucMode)
	UCHAR ucMode;
	UCHAR ucVdpId;
	VDP_OUT_INFO_T rOutInfo;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: ucMode\n");
		return 0;
	}

	ucMode = (UCHAR) StrToInt(szArgv[1]);

	switch(ucMode)
	{
		case 0:	// 480i;
		_656RegRead();

		r656SwReg.u4DispVt = 524;
		r656SwReg.u4DispHt = 1715;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 0;		// 656, 8 Bit

		r656SwReg.u4DispVs2Start = 263;
		r656SwReg.u4DispVs2End = 266;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 1;

		r656SwReg.u4DispSyncHStart = 31;
		r656SwReg.u4DispSyncHEnd = 95;

		r656SwReg.u4DispDataEnStart = 270;
		r656SwReg.u4DispDataEnEnd = 1714;

		r656SwReg.u4DispFldTgl0 = 2;
		r656SwReg.u4DispFldTgl1 = 264;

		r656SwReg.u4DispVdeTgl0 = 524;
		r656SwReg.u4DispVdeTgl1 = 21;
		r656SwReg.u4DispVdeTgl2 = 261;
		r656SwReg.u4DispVdeTgl3 = 284;

		r656SwReg.fgIfldP = 0;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;

		r656SwReg.fg656Clk27M = 1;

		_656RegWrite();

		Printf("480i/27M/8Bit\n");

		rOutInfo.ucPrg = 0;
		rOutInfo.ucFrameRate = 60;
		rOutInfo.u4Width = 720;
		rOutInfo.u4Height = 480;
		rOutInfo.u4TotalWidth = 858;
		rOutInfo.u4TotalHeight = 525;

		break;

		case 1:	// 576i;
		
		_656RegRead();

		r656SwReg.u4DispVt = 624;
		r656SwReg.u4DispHt = 1727;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 0;		// 656, 8 Bit

		r656SwReg.u4DispVs2Start = 313;
		r656SwReg.u4DispVs2End = 316;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 1;

		r656SwReg.u4DispSyncHStart = 23;
		r656SwReg.u4DispSyncHEnd = 87;

		r656SwReg.u4DispDataEnStart = 282;
		r656SwReg.u4DispDataEnEnd = 1726;

		r656SwReg.u4DispFldTgl0 = 311;
		r656SwReg.u4DispFldTgl1 = 624;

		r656SwReg.u4DispVdeTgl0 = 622;
		r656SwReg.u4DispVdeTgl1 = 21;
		r656SwReg.u4DispVdeTgl2 = 309;
		r656SwReg.u4DispVdeTgl3 = 334;

		r656SwReg.fgIfldP = 0;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 0;

		r656SwReg.fg656Clk27M = 1;

		_656RegWrite();

		Printf("576i/27M/8Bit\n");

		rOutInfo.ucPrg = 0;
		rOutInfo.ucFrameRate = 50;
		rOutInfo.u4Width = 720;
		rOutInfo.u4Height = 576;
		rOutInfo.u4TotalWidth = 864;
		rOutInfo.u4TotalHeight = 625;
		break;

		case 2:	// 480p;

		_656RegRead();

		r656SwReg.u4DispVt = 524;
		r656SwReg.u4DispHt = 857;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 3;		// 656, 24 Bit

		r656SwReg.u4DispVs2Start = 263;
		r656SwReg.u4DispVs2End = 266;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 0;

		r656SwReg.u4DispSyncHStart = 15;
		r656SwReg.u4DispSyncHEnd = 47;

		r656SwReg.u4DispDataEnStart = 134;
//		r656SwReg.u4DispDataEnEnd = 856;
		r656SwReg.u4DispDataEnEnd = 857;

		r656SwReg.u4DispFldTgl0 = 1023;
		r656SwReg.u4DispFldTgl1 = 1023;

		r656SwReg.u4DispVdeTgl0 = 44;
		r656SwReg.u4DispVdeTgl1 = 524;
		r656SwReg.u4DispVdeTgl2 = 1023;
		r656SwReg.u4DispVdeTgl3 = 1023;

		r656SwReg.fgIfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 0;

		r656SwReg.fg422to444 = 1;

		r656SwReg.fg656Clk27M = 1;

		_656RegWrite();

		Printf("480p/27M/24Bit\n");

		rOutInfo.ucPrg = 1;
		rOutInfo.ucFrameRate = 60;
		rOutInfo.u4Width = 720;
		rOutInfo.u4Height = 480;
		rOutInfo.u4TotalWidth = 858;
		rOutInfo.u4TotalHeight = 525;
		break;

		case 3:	// 576p;

		_656RegRead();

		r656SwReg.u4DispVt = 624;
		r656SwReg.u4DispHt = 863;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 3;		// 656, 24 Bit

		r656SwReg.u4DispVs2Start = 313;
		r656SwReg.u4DispVs2End = 316;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 0;

		r656SwReg.u4DispSyncHStart = 11;
		r656SwReg.u4DispSyncHEnd = 43;

		r656SwReg.u4DispDataEnStart = 140;
//		r656SwReg.u4DispDataEnEnd = 862;
		r656SwReg.u4DispDataEnEnd = 863;

		r656SwReg.u4DispFldTgl0 = 1023;
		r656SwReg.u4DispFldTgl1 = 1023;

		r656SwReg.u4DispVdeTgl0 = 43;
		r656SwReg.u4DispVdeTgl1 = 619;
		r656SwReg.u4DispVdeTgl2 = 1023;
		r656SwReg.u4DispVdeTgl3 = 1023;

		r656SwReg.fgIfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 0;

		r656SwReg.fg422to444 = 1;

		r656SwReg.fg656Clk27M = 1;

		_656RegWrite();

		Printf("576p/27M/24Bit\n");

		rOutInfo.ucPrg = 1;
		rOutInfo.ucFrameRate = 50;
		rOutInfo.u4Width = 720;
		rOutInfo.u4Height = 576;
		rOutInfo.u4TotalWidth = 864;
		rOutInfo.u4TotalHeight = 625;
		break;

		case 4:	// 720p;

		_656RegRead();

		r656SwReg.u4DispVt = 749;
		r656SwReg.u4DispHt = 1649;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 3;		// 656, 24 Bit

		r656SwReg.u4DispVs2Start = 375;
		r656SwReg.u4DispVs2End = 378;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 0;

		r656SwReg.u4DispSyncHStart = 11;
		r656SwReg.u4DispSyncHEnd = 91;

		r656SwReg.u4DispDataEnStart = 366;
//		r656SwReg.u4DispDataEnEnd = 862;
		r656SwReg.u4DispDataEnEnd = 1649;

		r656SwReg.u4DispFldTgl0 = 1023;
		r656SwReg.u4DispFldTgl1 = 1023;

		r656SwReg.u4DispVdeTgl0 = 43;
		r656SwReg.u4DispVdeTgl1 = 749;
		r656SwReg.u4DispVdeTgl2 = 1023;
		r656SwReg.u4DispVdeTgl3 = 1023;

		r656SwReg.fgIfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 0;

		r656SwReg.fg422to444 = 1;

		r656SwReg.fg656Clk27M = 0;

		_656RegWrite();
		
		Printf("720p/74.25M/24Bit\n");

		rOutInfo.ucPrg = 1;
		rOutInfo.ucFrameRate = 60;
		rOutInfo.u4Width = 1280;
		rOutInfo.u4Height = 720;
		rOutInfo.u4TotalWidth = 1650;
		rOutInfo.u4TotalHeight = 750;
		break;

		case 5:	// 1080i;

		_656RegRead();

		r656SwReg.u4DispVt = 1124;
		r656SwReg.u4DispHt = 2199;

		r656SwReg.u4DispVs1Start = 0;
		r656SwReg.u4DispVs1End = 3;

		r656SwReg.u4Mode = 3;		// 656, 24 Bit

		r656SwReg.u4DispVs2Start = 561;
		r656SwReg.u4DispVs2End = 564;
		r656SwReg.fgVs2Blns = 1;
		r656SwReg.fgVs2En = 1;

		r656SwReg.u4DispSyncHStart = 88;
		r656SwReg.u4DispSyncHEnd = 152;

//		r656SwReg.u4DispDataEnStart = 276;
//		r656SwReg.u4DispDataEnEnd = 2199;
		r656SwReg.u4DispDataEnStart = 0xB4;
		r656SwReg.u4DispDataEnEnd = 0x847;

		r656SwReg.u4DispFldTgl0 = 2;
		r656SwReg.u4DispFldTgl1 = 562;

		r656SwReg.u4DispVdeTgl0 = 1124;
		r656SwReg.u4DispVdeTgl1 = 21;
		r656SwReg.u4DispVdeTgl2 = 559;
		r656SwReg.u4DispVdeTgl3 = 580;

		r656SwReg.fgIfldP = 0;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;
		r656SwReg.fgOfldP = 1;

		r656SwReg.fg422to444 = 1;

		r656SwReg.fg656Clk27M = 0;

		_656RegWrite();

		Printf("1080i/74.25M/24Bit\n");

		rOutInfo.ucPrg = 0;
		rOutInfo.ucFrameRate = 60;
		rOutInfo.u4Width = 1920;
		rOutInfo.u4Height = 1080;
		rOutInfo.u4TotalWidth = 2200;
		rOutInfo.u4TotalHeight = 1125;
		break;

		default:
		Printf("Unknown mode\n");
		break;
	}

	rOutInfo.ucPmxId = PMX_1;

	for (ucVdpId = 0; ucVdpId < VDP_NS; ucVdpId++)
	{
		if (VDP_SetOutputInfo(ucVdpId, &rOutInfo) == VDP_SET_ERROR)
		{
			ASSERT(0);
		}
	}

	return 0;
}

static INT32 _656SetCmdBg (INT32 i4Argc, const CHAR ** szArgv)
{
 	UINT32 u4BgColor;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
	{
		Printf("Arg:  u4BgColor\n");
		return 0;
	}

 	u4BgColor = StrToInt(szArgv[1]);

	_656RegRead();
	
	r656SwReg.u4Y = (u4BgColor >> 16) & 0xFF;
	r656SwReg.u4Cb = (u4BgColor >> 8) & 0xFF;
	r656SwReg.u4Cr = (u4BgColor) & 0xFF;

	r656SwReg.fgHen = 1;
	r656SwReg.fgVen = 1;

	_656RegWrite();

	return 0;
}

static INT32 _656SetCmdInput (INT32 i4Argc, const CHAR ** szArgv)
{
	// arg: (UCHAR uc656Id, UINT32 u4BgColor)
 	UINT32 u4Input;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
	{
		Printf("Arg: u4Input\n");
		return 0;
	}

 	u4Input = StrToInt(szArgv[1]);

	_656RegRead();

	switch (u4Input)
	{
		case 0:
		Printf("Input: B2R HD\n");
		r656SwReg.fgB2rSdTo656 = 0;
		break;

		case 1:
		Printf("Input: B2R SD\n");
		r656SwReg.fgB2rSdTo656 = 1;
		break;

		default:
		Printf("Unknown Input\n");
		return 0;
	}	

	_656RegWrite();

	return 0;
}

#endif

/* For Linux demo's temp usage, we want set video processing parameter via VDP_SetArg CLI */
#if defined(__linux__)
#if 0
static INT32 _VdpSetCmdArg(INT32 i4Argc, const CHAR **aszArgv)
{
    UINT8 ucVdpId;
    UINT8 ucArg;
    INT16 i2Value;
    if (i4Argc != 4)
    {
        Printf("Usage: %s 0 <VDP_ARG_TYPE> <mode_number>\n", aszArgv[0]);
        return -1;
    }
    ucVdpId = StrToInt(aszArgv[1]);
    if(ucVdpId > 2)
    {
        Printf("Usage: %s 0 <VDP_ARG_TYPE> <mode_number>\n", aszArgv[0]);
        return -1;
    }
    ucArg = StrToInt(aszArgv[2]);
    if(ucArg >= VDP_ARG_NS)
    {
        Printf("Usage: %s 0 <VDP_ARG_TYPE> <mode_number>\n", aszArgv[0]);
        return -1;
    }
    i2Value = (INT16)StrToInt(aszArgv[3]);
    if(VDP_SetArg(ucVdpId, ucArg, i2Value) != VDP_SET_OK)
    {
        Printf("Usage: %s 0 <VDP_ARG_TYPE> <mode_number>\n", aszArgv[0]);
        return -1;    
    }
    return 0;
}
#endif
static INT32 _VdpSetCmdForceRescale(INT32 i4Argc, const CHAR **aszArgv)
{
    UINT8 ucVdpId;
    if (i4Argc != 2)
    {
        Printf("Usage: %s <VDP_ID>\n", aszArgv[0]);
        return -1;
    }
    ucVdpId = StrToInt(aszArgv[1]);
    if(ucVdpId > 2)
    {
        Printf("Usage: %s <VDP_ID>\n", aszArgv[0]);
        return -1;
    }

    vDrvVideoFixedPreScaler(ucVdpId, 1, 1);
    
    return 0;
}
#endif /* __LINUX__ */ 
#endif/*#ifdef CC_CLI*/ 	

#if 0
/******************************************************************************
* Global Function
******************************************************************************/
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetVdpMwCmdTbl(void)
#else
CLI_EXEC_T* GetVdpCmdTbl(void)
#endif
{
	return &_rVdpModCmdTbl;
}
#endif

