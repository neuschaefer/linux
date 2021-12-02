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
 * $Author: youneng.xu $
 * $Date: 2013/01/15 $
 * $RCSfile: pmx_cmd.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.c
 *  This header file includes CLI function definitions of PMX driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "sv_const.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_lvds.h"
#if defined(CC_MT5365) || defined(CC_MT5395)
#include "drv_tcon.h"
#endif
#include "ostg_if.h"
#include "sif_if.h"
#include "drv_ycproc.h"
#include "vdp_if.h"
#include "video_def.h"
#include "pe_if.h"

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_ckgen.h"
#include "x_hal_arm.h"
#include "x_gpio.h"
#if defined(CC_MT5365) || defined(CC_MT5395)
#include "frametrack_drvif.h"
#include "drv_od.h"
#endif
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#ifdef CC_5391_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_5391_LOADER */
#define DEFINE_IS_LOG PMX_IsLog
#endif /* CC_5391_LOADER */
#include "x_debug.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHK_NUM_MIN_PARAM(NUN, USAGE) \
    do \
    { \
        if (i4Argc < ((NUN) + 1)) \
        { \
            Printf("Args: %s\n", USAGE); \
            return -1; \
        } \
    } while (0)

#define PARAM(IDX) (UINT32)StrToInt(szArgv[IDX+1])


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

EXTERN UINT8 bSI_DISPLAY_DCLK_TYPE;
EXTERN BOOL _fgGammaStressTest;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void vDrvOutputStageInit(void);
EXTERN void vDrvSetLCDTiming(void);
EXTERN INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN void vHalVideoGammaLoadTable(UINT8 bMode);
EXTERN BOOL bHalVideoVerifyGammaTable(UINT8 bMode);
EXTERN UINT8 bApiVideoVerifyGammaTable(UINT8 bMode);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static INT32 _PmxInitCmd(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _PmxODInitCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOnOffCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODBypassCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOverflowResetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOverflowGetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODCheckCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODStressTestCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODResetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODTableSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLSourceCmd(INT32 i4Argc, const CHAR **szArgv);

//automation
static INT32 _PmxLVDS_ATERM_ATVO_Setting(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ATERM_ATVO_Restore(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ATVO_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_APSRC_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ANSRC_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_PADPD_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_RESET_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxMLVDS_LS_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxMLVDS_TestMODE_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxDrvVCOPhase_SEL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSET_DDDSFreeRun(INT32 i4Argc, const CHAR **szArgv);


#endif
static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR ** szArgv);
INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxQueryCmd(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _PmxSetCmdBacklt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdESDProtect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdPlaneOrder(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPlaneOrderArray(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPanel(INT32 i4Argc, const CHAR ** szArgv);
INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPowerSequence(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxDiagCmdDumpScpos(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdSpreadSpectrum(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv);
#if !defined(CC_MT5387) && !defined(CC_MT5363)
static INT32 _PmxSetCmdSpecialNs(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _PmxListPanelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdDnie(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmd10bit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdSwap(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdLvdsMod(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdQueryLvdsMod(INT32 i4Argc, const CHAR **szArgv);
#ifdef SUPPORT_CW_CHANGE_BY_STEP
static INT32 _NptvSetCtrlWordStep(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvOutputStageDumpEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvForceFrameRate(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _PmxSetBorder(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetBorderColor(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetBorderParam(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetPanelDisplayTypeMsk(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5395
static INT32 _PmxSetCmdVb1(INT32 i4Argc, const CHAR **szArgv);
#endif // 5395
#endif
static INT32 _PmxSetCmdCtlWord(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _PmxSetScanningPWM(INT32 i4Argc, const CHAR **szArgv);
#endif

#ifndef CC_5391_LOADER
CLIMOD_DEBUG_FUNCTIONS(PMX)
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static CLI_EXEC_T _arPmxSetCmdTbl[] =
{
	{"esdp", NULL, _PmxSetCmdESDProtect, NULL, "ESD Protect flag", CLI_SUPERVISOR},
    {"bg", NULL, _PmxSetCmdBg, NULL, "background color", CLI_SUPERVISOR},
    {"po", NULL, _PmxSetCmdPlaneOrder, NULL, "plane order", CLI_SUPERVISOR},
    {"poa", NULL, _PmxSetCmdPlaneOrderArray, NULL, "plane order array",
     CLI_SUPERVISOR},
    {"backlt", "bl", _PmxSetCmdBacklt, NULL, "backlight", CLI_GUEST},
    {"driving", "d", _PmxSetCmdDrivingCurrent, NULL, "lvds driving current",
     CLI_GUEST},
    {"spread", "s", _PmxSetCmdSpreadSpectrum, NULL, "lvds spread spectrum",
     CLI_GUEST},

    {"panel", "p", _PmxSetCmdPanel, NULL, "panel resolution", CLI_GUEST },
    {"powersequence", "ps", _PmxSetCmdPowerSequence, NULL,
     "adjust power sequence", CLI_GUEST },
    //{"xtal", "xtal", _PmxSetCmdXtal, NULL, "panel resolution", CLI_GUEST },
    {"ns", "ns", _PmxSetCmdNs, NULL, "NS lvds format", CLI_GUEST },
    {"jeida", "jeida", _PmxSetCmdJeida, NULL, "JEIDA lvds format", CLI_GUEST },
#if !defined(CC_MT5387) && !defined(CC_MT5363)
    {"SpecialNs", "sns", _PmxSetCmdSpecialNs, NULL, "Special NS lvds format", CLI_GUEST },
#endif
    {"dnie", "dnie", _PmxSetCmdDnie, NULL, "ByPass DNIE", CLI_GUEST },
    {"10bit", "10bit", _PmxSetCmd10bit, NULL, "switch LVDS to 10bit", CLI_GUEST },
    {"swap", "swap", _PmxSetCmdSwap, NULL, "even odd channel swap", CLI_GUEST },
    {"lvdsmod", NULL, _PmxSetCmdLvdsMod, NULL, "set LVDS Tx mode", CLI_GUEST},
    {"qlvdsmod", NULL, _PmxSetCmdQueryLvdsMod, NULL, "query LVDS Tx mode", CLI_GUEST},
#ifdef SUPPORT_CW_CHANGE_BY_STEP
    {"SetCtrlWordStep",  "cws",	_NptvSetCtrlWordStep,  NULL,	"Nptv Set Control Word Step",  CLI_SUPERVISOR},
#endif
    {"OutputStageDumpEnable",  "osdumpen",   _NptvOutputStageDumpEnable,   NULL,           "Enable OutputStage Dump",    CLI_SUPERVISOR},     
    {"ForceFrameRate",	"ffr",	_NptvForceFrameRate,  NULL,  "Nptv force frame rate",  CLI_SUPERVISOR},
#if defined(CC_MT5365) || defined(CC_MT5395)
    {"borderonoff",	"boo",	_PmxSetBorder,  NULL,  "OSTG border",  CLI_GUEST},
    {"bordercolor",	"bc", 	_PmxSetBorderColor,  NULL,  "OSTG border color",  CLI_GUEST},
    {"borderparameter",	"bparam",	_PmxSetBorderParam,  NULL,  "OSTG border position",  CLI_GUEST},
    {"panelifmask", "pim", _PmxSetPanelDisplayTypeMsk, NULL, "Set Panel interface allowed mask", CLI_SUPERVISOR},

    //automation
    {"ATERM_ATVO", "ATERM_ATVO", _PmxLVDS_ATERM_ATVO_Setting, NULL, "PmxLVDS_ATERM_ATVO_Setting", CLI_SUPERVISOR},
    {"ATERM_ATVO_Restore", "ATERM_ATVO_Restore", _PmxLVDS_ATERM_ATVO_Restore, NULL, "PmxLVDS_ATERM_ATVO_Restore", CLI_SUPERVISOR},
    {"ATVO_Set", "ATVO_Set", _PmxLVDS_ATVO_Set, NULL, "PmxLVDS_ATVO_Set", CLI_SUPERVISOR},
    {"APSRC_Set", "APSRC_Set", _PmxLVDS_APSRC_Set, NULL, "PmxLVDS_APSRC_Set", CLI_SUPERVISOR},
    {"ANSRC_Set", "ANSRC_Set", _PmxLVDS_ANSRC_Set, NULL, "PmxLVDS_ANSRC_Set", CLI_SUPERVISOR},
    {"PADPD_Set", "PADPD_Set", _PmxLVDS_PADPD_Set, NULL, "PmxLVDS_PADPD_Set", CLI_SUPERVISOR},
    {"RESET_Set", "RESET_Set", _PmxLVDS_RESET_Set, NULL, "PmxLVDS_RESET_Set", CLI_SUPERVISOR},
    {"LS_Set", "LS_Set", _PmxMLVDS_LS_Set, NULL, "PmxMLVDS_LS_Set", CLI_SUPERVISOR},
    {"TestMODE_Set", "TestMODE_Set", _PmxMLVDS_TestMODE_Set, NULL, "PmxMLVDS_TestMODE_Set", CLI_SUPERVISOR},
    {"VCOPhase_SEL", "VCOPhase_SEL", _PmxDrvVCOPhase_SEL, NULL, "PmxDrvVCOPhase_SEL", CLI_SUPERVISOR},
    {"DDDSFreeRun", "DDDSFreeRun", _PmxSET_DDDSFreeRun, NULL, "PmxSET_DDDSFreeRun", CLI_SUPERVISOR},

    
#ifdef CC_MT5395
    {"vbone", "vb1", _PmxSetCmdVb1, NULL, "Set Panel vb1 flag", CLI_SUPERVISOR},
#endif
#endif
    {"controlword", "ctlw",  _PmxSetCmdCtlWord, NULL, "Set control word", CLI_SUPERVISOR},
#ifdef CC_SCPOS_3DTV_SUPPORT
    {"scanningpwm", "scanpwm",  _PmxSetScanningPWM, NULL, "Set scanning pwm", CLI_SUPERVISOR},
#endif 	 	
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxDiagCmdTbl[] =
{
    {"dump", NULL, _PmxDiagCmdDumpScpos, NULL, "dump scpos", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#if defined(CC_MT5365) || defined(CC_MT5395)

static CLI_EXEC_T _arPmxODCmdTbl[] =
{
    {"init", "i", _PmxODInitCmd, NULL, "OD initial", CLI_SUPERVISOR},
    {"onoff", "on", _PmxODOnOffCmd, NULL, "OD enable", CLI_SUPERVISOR},
    {"bypass", "bypass", _PmxODBypassCmd, NULL, "OD bypass", CLI_SUPERVISOR},
    {"overflow reset", "oreset", _PmxODOverflowResetCmd, NULL, "OD overflow reset", CLI_SUPERVISOR},
    {"overflow get", "oget", _PmxODOverflowGetCmd, NULL, "OD overflow get", CLI_SUPERVISOR},
    {"Check", "chk", _PmxODCheckCmd, NULL, "OD check", CLI_SUPERVISOR},
    {"stress test", "st", _PmxODStressTestCmd, NULL, "OD stress test", CLI_SUPERVISOR},
     {"od reset", "reset", _PmxODResetCmd, NULL, "OD reset test", CLI_SUPERVISOR},
     {"od table select", "sel", _PmxODTableSelCmd, NULL, "OD table select", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxVOPLLCmdTbl[] =
{
    {"status", "s", _PmxVOPLLStatusCmd, NULL, "vopll status", CLI_SUPERVISOR},
    {"xtal", "xtal", _PmxVOPLLSourceCmd, NULL, "vopll source from xtal", CLI_SUPERVISOR},    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#endif

static CLI_EXEC_T _arPmxCmdTbl[] =
{
    {"init", "i", _PmxInitCmd, NULL, "init", CLI_SUPERVISOR},
    {"enable", "e", _PmxEnableCmd, NULL, "enable/disable LVDS", CLI_GUEST},
    {"pattern", "p", _PmxPatternCmd, NULL, "enable/disable test pattern", CLI_GUEST},
    {"OSTG in pattern", "ostgpt", _PmxSetCmdOSTGPt, NULL, "ostg paten", CLI_GUEST },
    {"list", "l", _PmxListPanelCmd, NULL, "show panel list", CLI_GUEST},
    {"query", "q", _PmxQueryCmd, NULL, "dump pmx(scpos) info", CLI_GUEST},
    {"set", "s", NULL, _arPmxSetCmdTbl, "set parameter", CLI_GUEST},
    {"diag", "d", NULL, _arPmxDiagCmdTbl, "verify hardware", CLI_SUPERVISOR},
#if defined(CC_MT5365) || defined(CC_MT5395)    
    {"od", "od",NULL, _arPmxODCmdTbl, "od parameters", CLI_SUPERVISOR},
    {"vopll", "vopll",NULL, _arPmxVOPLLCmdTbl, "vopll parameters", CLI_SUPERVISOR},    
#endif

#ifndef CC_5391_LOADER
    CLIMOD_DEBUG_CLIENTRY(PMX),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Pmx)
{
    "pmx",
    NULL,
    NULL,
    _arPmxCmdTbl,
    "pmx command",
    CLI_GUEST
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    PMX_Init();

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "fgEnable");

    vApiPanelPowerSequence(PARAM(0));
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    PMX_DIAG_DumpScpos();
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdESDProtect(INT32 i4Argc, const CHAR **szArgv)
{
	_fgESDProtect = PARAM(0);

	return 0;
}

static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "u4BgColor(RGB)");
    PMX_SetBg(PARAM(0), FALSE);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPlaneOrder(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2, "ucVdpId(0:main,2:osd1,3:osd2)\n"
                      "ucPlaneOrder(0:top~2:bottom)");

    if (PMX_SetPlaneOrder(PARAM(0), PARAM(1)) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPlaneOrderArray(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2, "order1(top), order2(bot)\n"
                      "(0:main,3:osd2)");

    UINT32 au4PlaneOrder[4];
    au4PlaneOrder[0] = PARAM(0);
    au4PlaneOrder[1] = PARAM(1);
    au4PlaneOrder[2] = PARAM(2);
    au4PlaneOrder[3] = PARAM(3);
    if (PMX_SetPlaneOrderArray(au4PlaneOrder) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdBacklt(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1, "backlight(0~255) [freq(Hz)]");

    UINT32 u4Bright = PARAM(0);
    UINT32 u4Freq = (i4Argc == 3)?PARAM(1):BACKLT_GetDimmingFrequency();

    if (IsPwmLowPanelBright() == PWM_LOW_PANEL_BRIGHT)
    {
        u4Bright = 255 - u4Bright;
    }
    BACKLT_SetDimming(u4Freq, u4Bright);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdSpreadSpectrum(INT32 i4Argc, const CHAR ** szArgv)
{

#if 1
    UINT32 u4Permillage = DDDS_GetSpreadSpectrum();
    UINT32 u4Freq = DDDS_GetSpreadSpectrumFreq();
    if (i4Argc == 1)
    {
        //Printf("current value=%d\n", u4Permillage);
        if (u4Permillage == 0)
        {   
               Printf("current range = 0\n");
               Printf("current modulation rate = 0\n");
        }
        else
        {
               Printf("current range = %d\n", u4Permillage);
               Printf("current modulation rate = %d\n", u4Freq);
        }
    }

    //CHK_NUM_MIN_PARAM(1, "value(0~1000) | {period(us), range(0.1%)}");
    CHK_NUM_MIN_PARAM(1, "{value(20K~100K), value(0~60)} | {rate(Hz), range(0.1%)}");

    if (i4Argc == 2)
    {
        vDrvSetVOSpread(PARAM(0));
    }

    if (i4Argc == 3)
    {
        DDDS_SetSpreadSpectrum(PARAM(0), PARAM(1));
    }
#else
Printf("CLI not ready\n");
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPanel(INT32 i4Argc, const CHAR ** szArgv)
{
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    CHK_NUM_MIN_PARAM(1, "u4PanelIndex | \n"
                      "{u4DispClk u4PanelWidth u4PanelHeight\n"
                      "u4HTotal u4VTotal\n"
                      "u4HSyncWidth u4VSyncWidth u4HPos u4VPos target}\n");

    if (i4Argc == 2 || i4Argc == 3)
    {
        if (PARAM(0) > 0xff)
        {
            LoadPanelIndex();
            DumpPanelAttribute(PANEL_DUMP_CURRENT);
            return 0;
        }
        SelectPanel(PARAM(0));
        if (i4Argc == 3)
        {
            return 0;
        }

        WritePanelIndexToEeprom(PARAM(0));
    }
    else
    {
        UINT32 u4DispClk = PARAM(0);
        UINT32 u4DispWidth = PARAM(1);
        UINT32 u4DispHeight = PARAM(2);
        UINT32 u4HTotal = PARAM(3);
        UINT32 u4VTotal = PARAM(4);
        UINT32 u4HSyncWidth = PARAM(5);
        UINT32 u4VSyncWidth = PARAM(6);
        UINT32 u4HPos = PARAM(7);
        UINT32 u4VPos = PARAM(8);
        UINT32 u4Target = PARAM(9);

        if ((u4Target != 50) && (u4Target != 60))
        {
            u4Target = 0;
        }

        if (u4DispClk > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetPixelClk60Hz(u4DispClk);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetPixelClk50Hz(u4DispClk);
            }
        }

        if (u4DispWidth > 0)
        {
            PANEL_SetPanelWidth(u4DispWidth);
        }

        if (u4DispHeight > 0)
        {
            PANEL_SetPanelHeight(u4DispHeight);
        }

        if (u4HTotal > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetHTotal60Hz(u4HTotal);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetHTotal50Hz(u4HTotal);
            }
        }
        if (u4VTotal > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetVTotal60Hz(u4VTotal);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetVTotal50Hz(u4VTotal);
            }
        }

        if (u4HSyncWidth > 0)
        {
            PANEL_SetHSyncWidth(u4HSyncWidth);
        }

        if (u4VSyncWidth > 0)
        {
            PANEL_SetVSyncWidth(u4VSyncWidth);
        }

        if (u4HPos > 0)
        {
            PANEL_SetHPosition60Hz(u4HPos);
        }

        if (u4VPos > 0)
        {
            PANEL_SetVPosition(u4VPos);
        }
    }

    // turn off panel
    vApiPanelPowerSequence(FALSE);

    vDrvOutputStageInit();
    vDrvVOFreqSet(
        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
            (PANEL_GetPixelClk50Hz()):
#ifdef SUPPORT_PANEL_48HZ
            (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
            (PANEL_GetPixelClk48Hz()):
#endif
            (PANEL_GetPixelClk60Hz()));
    vDrvSetLCDTiming();
    #if defined(CC_MT5365) || defined(CC_MT5395)
    vErrorHandleSetByPanel();
    #endif
    OSTG_OnOutputVSync();
#if !defined(CC_MT5368) && !defined(CC_MT5396)
    vFrameTrackConfig(3, 20, 4);
#endif
    IGNORE_RET(OSD_UpdateTiming());

    vApiPanelPowerSequence(TRUE);

    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    
#ifdef CC_MT5395
    if(PANEL_IsSupportVb1()==FALSE)
#endif
    {
        //LVDS 7->4 FIFO enable 
        vDrvLVDS7To4FifoEnable();
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPowerSequence(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(4, "LvdsOn, BacklightOn, BacklightOff, LvdsOff (unit:10ms)");
    PANEL_SetLvdsOnDalay(PARAM(0));
    PANEL_SetBacklightOnDelay(PARAM(1));
    PANEL_SetBacklightOffDelay(PARAM(2));
    PANEL_SetLvdsOffDalay(PARAM(3));
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Current =(UINT32)vDrvLVDSGetDriving();
    if (i4Argc == 1)
    {
        Printf("current value=%d\n", u4Current);
    }

    CHK_NUM_MIN_PARAM(1, "value(0~15)");
    u4Current = PARAM(0) & 0xf;
    //vWriteSCPOSMsk(0x4, u4Current, 0xf);
    vDrvLVDSSetDriving(u4Current);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 cmd;
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on)");

    cmd = PARAM(0);

    vDrvSetOSTGOutPattern(cmd);
    return 0;
}

INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 id, cmd;
    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) fgEnable(0:off, 1:on)");

    id = PARAM(0);
    cmd = PARAM(1);

    #if defined(CC_MT5363) || defined(CC_MT5387) || defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
    vDrvSetOSTGInPattern(id, cmd);
    #endif
 
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDiagCmdDumpScpos(INT32 i4Argc, const CHAR ** szArgv)
{
    PMX_DIAG_DumpScpos();
    if (i4Argc == 1)
    {
        DumpPanelAttribute(PANEL_DUMP_CURRENT);
    }
    if (i4Argc == 2)
    {
        DumpPanelAttribute(PARAM(0));
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#if defined(CC_MT5365) || defined(CC_MT5395)

static INT32 _PmxODInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvODInit();

    return 0;
}

static INT32 _PmxODOnOffCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: On / 0:Off)");

    vDrvODOnOff(PARAM(0));

    return 0;
}

static INT32 _PmxODBypassCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: bypass / 0:not bypass)");

    vDrvODBypass(PARAM(0));

    return 0;
}

static INT32 _PmxODOverflowResetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    vDrvODDramOverflowReset();

    return 0;
}
static INT32 _PmxODOverflowGetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    UNUSED(u4DrvDramOverflow());
 
    return 0;
}


static INT32 _PmxODStressTestCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "st number)");

    vDrvODStressTest(PARAM(0));

    return 0;
}
static INT32 _PmxODCheckCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvODCheckEnable();

    return 0;
}

static INT32 _PmxODResetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvODReset();
    return 0;
}

static INT32 _PmxODTableSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
    //DRVCUST_ODTableSel(PARAM(0));
    return 0;
}

static INT32 _PmxVOPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvVOPLLStatus();

    return 0;
}

static INT32 _PmxVOPLLSourceCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("On: 1 / Off: 0\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1VOPLLSource(u1arg1);

    return 0;
}

#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#if 0
static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on)");

    if (PARAM(0) == 0)
    {
        CKGEN_SetVOPLLInputAsDDDS(TRUE);
        // power down sdds
//        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 0, 1 << 30);
    }
    else
    {
        CKGEN_SetVOPLLInputAsDDDS(FALSE);
        // power on sdds
//        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 1 << 30, 1 << 30);
    }
    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv)
{
    //vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_MSB_INV);
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = ((u4ControlWord | LVDS_MSB_SW_ON)&0xFFFFFD);//should turn off Special NS

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig();
    
    return 0;
}

#if !defined(CC_MT5387) && !defined(CC_MT5363)
static INT32 _PmxSetCmdSpecialNs(INT32 i4Argc, const CHAR **szArgv)
{
    //vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_MSB_INV);
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = ((u4ControlWord | LVDS_SPECIAL_NS)&0xFFFFFE);//should turn off NS

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig();
    
    return 0;
}
#endif
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord =(( u4ControlWord & (~LVDS_MSB_SW_ON))&0xFFFFFD);//should turn off Special NS

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig(); 
    
//    vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_MSB_INV);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxListPanelCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    DumpPanelAttribute(3);
    return 0;
}


static INT32 _PmxSetCmdDnie(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 au1Buf[2] = {0x00, 0x20};

    LOG(1, "Bypassing DNIE SDP64 ...\n");

    GPIO_SetOut(GPIO(8), 0);
    x_thread_delay(100);
    GPIO_SetOut(GPIO(8), 1);

    if (SIF_WriteMultipleSubAddr(0x100, 0xC8, 2, 0x01FA, au1Buf, 2) == 0)
    {
        LOG(1, "Write DNIE SDP64 Fail!\n");
    }
    else
    {
        LOG(1, "Write DNIE SDP64 Success (%d, %d)!\n", au1Buf[0], au1Buf[1]);
    }

    return 0;
}


static INT32 _PmxSetCmd10bit(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1:10bit, 0:8bit)");
#if 0     //todo
    #ifdef CC_MT5387
    if (PARAM(0) == 0)
    {
        vRegWriteFldAlign(OUTSTG_OS_2F, 0, OS_2F_10B_EN);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_2F, 1, OS_2F_10B_EN);
    }
    #else
    if (PARAM(0) == 0)
    {
        vRegWriteFldAlign(OUTSTG_OS_2F, 0, OUTSTG_10B_EN);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_2F, 1, OUTSTG_10B_EN);
    }
    #endif
#endif    
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = u4ControlWord & (~DISP_BIT_MSK);

    if (PARAM(0) == 0)
    {
        u4ControlWord |= DISP_24BIT;
    }
    else
    {
        u4ControlWord |= DISP_30BIT;
    }

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSOn();     

    return 0;
}

static INT32 _PmxSetCmdSwap(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: LVDS_ODD_SW_ON, 0: LVDS_ODD_SW_OFF)");

    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = u4ControlWord & (~LVDS_ODD_SW_ON);

    if (PARAM(0) == 0)
    {
        u4ControlWord |= LVDS_ODD_SW_OFF;
    }
    else
    {
        u4ControlWord |= LVDS_ODD_SW_ON;
    }

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig(); 

    return 0;
}

#ifdef SUPPORT_CW_CHANGE_BY_STEP
extern UINT32 _u4CWStep;
static INT32 _NptvSetCtrlWordStep(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc==2)
	{
		_u4CWStep = StrToInt(szArgv[1]);
	}
	else
	{
		Printf("Arg: _u4CWStep, 0xfffffff:disable, Now=0x%x\n", _u4CWStep);
	}

	return 0;	
}
#endif
#if !( defined(CC_MT5365) || defined(CC_MT5395))
EXTERN UINT32 _u4OsDumpEnable;
#endif
static INT32 _NptvOutputStageDumpEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR ucOnOff;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: 1(On)/0(Off)\n");
		return 0;
	}

	ucOnOff = (UCHAR) StrToInt(szArgv[1]);
	ucOnOff = ((ucOnOff>0)?SV_TRUE : SV_FALSE);
	#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
	vScpipSetDumpCtrl(ucOnOff);
       #else
       _u4OsDumpEnable = ucOnOff;
       #endif
    
    return 0;    
}

extern UINT8 _bForceFrameRate;
static INT32 _NptvForceFrameRate(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		_bForceFrameRate = StrToInt(szArgv[1]);
		vSetMainFlg(MAIN_FLG_MODE_CHG);
	}
	else
	{
		Printf("Arg: ubFrameRage, 0:disable\n");
	}

	return 0;	

}


static INT32 _PmxSetCmdLvdsMod(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef DRV_SUPPORT_EXTMJC
    CHK_NUM_MIN_PARAM(1, "\n0: Normal\n1: Yuv422 one port(LVDS 10-bit)\n2: Rgb444 one port(LVDS 10-bit)\n3: Yuv422 two port(LVDS 10-bit)\n4: Rgb444 two port(LVDS 10-bit)\n5: Yuv422 one port(LVDS 8-bit)\n6: Rgb444 one port(LVDS 8-bit)\n7: Yuv422 two port(LVDS 8-bit)");

    UINT8   u1ColorDomain;
    UINT8   u1LvdsMode;

    u1LvdsMode = (UINT8)PARAM(0);

    if (u1LvdsMode > 7)
    {
        LOG(0, "invalid parameters\n");
        return 0;
    }

    if ((u1LvdsMode == eLvdsVidYuv422OnePort8Bit) || (u1LvdsMode == eLvdsVidYuv422TwoPort8Bit) || (u1LvdsMode == eLvdsVidYuv422TwoPort) || (u1LvdsMode == eLvdsVidYuv422OnePort))
    {
        u1ColorDomain = eColorDomainYUV;
    }
    else
    {
        u1ColorDomain = eColorDomainRGB;
    }
    
    // set current LVDS mode
    vDrvSetCurLvdsMode(u1LvdsMode);    

    vDrvSetUniColorDomain(u1ColorDomain, u1LvdsMode);
    vDrvSetLvdsColorDomain(u1ColorDomain);
#else
    LOG(0, "Not Support\n");
#endif
    return 0;
}

static INT32 _PmxSetCmdQueryLvdsMod(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef DRV_SUPPORT_EXTMJC
    // get current LVDS mode
    LOG(1, "Current LVDS mode = %u\n", vDrvGetCurLvdsMode());
#else
    LOG(0, "Not Support\n");
#endif
    
    return 0;
}

#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _PmxSetBorder(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) fgEnable(0:off, 1:on)");

    vOSTGSetBorderOnOff(PARAM(0), PARAM(1));
    
    return 0;
}

static INT32 _PmxSetBorderColor(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) u4RGB(0xRRGGBB)");

    UINT32 id = PARAM(0);
    
    vOSTGSetBorderOnOff(id, 0);
    vOSTGSetBorderColor(id, PARAM(1));
    vOSTGSetBorderOnOff(id, 1);

    return 0;
}

static INT32 _PmxSetBorderParam(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(6, "VdpId(0:main, 1:pip) u4X, u4Y, u4Width, u4Height, u4Thickness");

    UINT32 id = PARAM(0);
    
    vOSTGSetBorderOnOff(id, 0);
    vOSTGSetBorderParam(id, PARAM(1), PARAM(2), PARAM(3), PARAM(4), PARAM(5));
    vOSTGSetBorderOnOff(id, 1);

    return 0;
}

static INT32 _PmxSetPanelDisplayTypeMsk(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PanelProtTypeMsk;

    if(i4Argc < 2)
    {
        Printf("Usage: %s <display type mask> ([0]:LVDS, [2]:MLVDS"
#ifdef CC_MT5395 // 95 support VB1
", [1]:VBI, [3]:EPI"
#endif
")\ncurr allowed mask = %d\n", szArgv[0], PANEL_SetProtDisplayTypeMsk(NULL));
        return 0;
    }

    u4PanelProtTypeMsk = PARAM(0);

    PANEL_SetProtDisplayTypeMsk(&u4PanelProtTypeMsk);
    Printf("curr allowed mask = %d\n", PANEL_SetProtDisplayTypeMsk(NULL));

    return 0;
}

#ifdef CC_MT5395
static INT32 _PmxSetCmdVb1(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();

    if (PARAM(0) == 0)
    {
        u4ControlWord &= ~DISP_TYPE_VB1;
    }
    else
    {
        u4ControlWord |= DISP_TYPE_VB1;
    }

    PANEL_SetControlWord(u4ControlWord);

    return 0;
}
#endif //5395
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//automation
static INT32 _PmxLVDS_ATERM_ATVO_Setting(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;
  
  if(x_strcmp(szArgv[1], "1") ==0 )
  { 
    mode = 1;
  }
  else if(x_strcmp(szArgv[1], "2") ==0 )
  {
    mode = 2;
  }
  else if(x_strcmp(szArgv[1], "3") ==0 )
  {
    mode = 3;
  }


  LVDS_ATERM_ATVO_Setting(mode);
    
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_ATERM_ATVO_Restore(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 ATERN=0,ATVO=0;

  if(i4Argc==3)
  { 
    ATERN = StrToInt(szArgv[1]); 
    ATVO  = StrToInt(szArgv[2]); 
  }

  LVDS_ATERM_ATVO_Restore(ATERN,ATVO);
  Printf("ATERN=%d, ATVO=%d\n",ATERN,ATVO);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_ATVO_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_ATVO_Set(mode);
  Printf("LVDS_ATVO_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_APSRC_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_APSRC_Set(mode);
  Printf("LVDS_APSRC_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_ANSRC_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_ANSRC_Set(mode);
  Printf("LVDS_ANSRC_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_PADPD_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_PADPD_Set(mode);
  Printf("LVDS_PADPD_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxLVDS_RESET_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_RESET_Set(mode);
  Printf("LVDS_RESET_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxMLVDS_LS_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  MLVDS_LS_Set(mode);
  Printf("MLVDS_LS_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxMLVDS_TestMODE_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  MLVDS_TestMODE_Set(mode);
  Printf("MLVDS_TestMODE_Set=%d\n",mode);     
     
  return 0;
}
////////////////////////////////////////////////
static INT32 _PmxDrvVCOPhase_SEL(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 phase=0;

  if(i4Argc==2)
  { 
    phase = StrToInt(szArgv[1]); 
   
  }

  vDrvVCOPhase_SEL(phase);
  Printf("vDrvVCOPhase_SEL=%d\n",phase);     
     
  return 0;
}

////////////////////////////////////////////////
static INT32 _PmxSET_DDDSFreeRun(INT32 i4Argc, const CHAR **szArgv)
{
  SET_DDDSFreeRun();  
  Printf("SET_DDDSFreeRun"); 
     
  return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
#endif
static INT32 _PmxSetCmdCtlWord(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4CtlWord;

    u4CtlWord = PANEL_GetControlWord();
    Printf("Curr panel controlword = %08x\n", u4CtlWord);
    if(i4Argc == 2)
    {
        u4CtlWord = StrToInt(szArgv[1]);
        Printf("Set controlword to %08x\n", u4CtlWord);
        PANEL_SetControlWord(u4CtlWord);
    }
    return 0;
}

#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _PmxSetScanningPWM(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT32 u4Start;
    UINT32 u4High;
    
    if (i4Argc < 5)
    {
        Printf("Usage: %s <HTotal><VTotal><Start><High>\n", szArgv[0]);
        return 1;
    }
    u2HTotal = StrToInt(szArgv[1]);
    u2VTotal = StrToInt(szArgv[2]);
    u4Start = StrToInt(szArgv[3]);
    u4High = StrToInt(szArgv[4]);
    
    vApiSetScanningPWM(u2HTotal, u2VTotal, u4Start, u4High);

    return 0;
}
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

