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
 * $RCSfile: pmx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.c
 *  This header file includes CLI function definitions of PMX driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "pmx_drvif.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "sv_const.h"
//#include "scpos_reg.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#include "api_backlt.h"

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_ckgen.h"
#include "x_hal_arm.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG PMX_IsLog
#endif /* CC_MTK_LOADER */
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


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void vDrvOutputStageInit(void);
EXTERN void vDrvSetLCDTiming(void);
EXTERN INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static INT32 _PmxInitCmd(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxQueryCmd(INT32 i4Argc, const CHAR ** szArgv);

//static INT32 _PmxSetCmdGamma(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdBrightness(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdContrast(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdBacklt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _PmxSetCmdPlaneOrder(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdPlaneOrderArray(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPanel(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPowerSequence(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxDiagCmdGammaTable(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxDiagCmdOutputStage(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxDiagCmdWriteback(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxDiagCmdDumpScpos(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdSpreadSpectrum(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxListPanelCmd(INT32 i4Argc, const CHAR **szArgv);

#ifndef CC_MTK_LOADER
CLIMOD_DEBUG_FUNCTIONS(PMX)
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CLI_EXEC_T _arPmxSetCmdTbl[] =
{
//    {"gamma", "g", _PmxSetCmdGamma, NULL, "gamma curve", CLI_SUPERVISOR},
//    {"bright", "b", _PmxSetCmdBrightness, NULL, "brightness", CLI_SUPERVISOR},
//    {"contrast", "c", _PmxSetCmdContrast, NULL, "contrast", CLI_SUPERVISOR},
    {"bg", NULL, _PmxSetCmdBg, NULL, "background color", CLI_SUPERVISOR},
//    {"po", NULL, _PmxSetCmdPlaneOrder, NULL, "plane order", CLI_SUPERVISOR},
//    {"poa", NULL, _PmxSetCmdPlaneOrderArray, NULL, "plane order array",
 //    CLI_SUPERVISOR},
//    {"backlt", "bl", _PmxSetCmdBacklt, NULL, "backlight", CLI_SUPERVISOR},
//    {"driving", "d", _PmxSetCmdDrivingCurrent, NULL, "lvds driving current",
//     CLI_GUEST},
//    {"spread", "s", _PmxSetCmdSpreadSpectrum, NULL, "lvds spread spectrum",
//     CLI_GUEST},

    {"panel", "p", _PmxSetCmdPanel, NULL, "panel resolution", CLI_GUEST },
    {"powersequence", "ps", _PmxSetCmdPowerSequence, NULL,
     "adjust power sequence", CLI_GUEST },
//    {"xtal", "xtal", _PmxSetCmdXtal, NULL, "panel resolution", CLI_GUEST },
//    {"ns", "ns", _PmxSetCmdNs, NULL, "NS lvds format", CLI_GUEST },
//    {"jeida", "jeida", _PmxSetCmdJeida, NULL, "JEIDA lvds format", CLI_GUEST },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//static CLI_EXEC_T _arPmxDiagCmdTbl[] =
//{
//    {"gt", NULL, _PmxDiagCmdGammaTable, NULL, "gamma table", CLI_SUPERVISOR},
//    {"os", NULL, _PmxDiagCmdOutputStage, NULL, "output stage", CLI_SUPERVISOR},
//    {"wb", NULL, _PmxDiagCmdWriteback, NULL, "write back", CLI_SUPERVISOR},
//    {"dump", NULL, _PmxDiagCmdDumpScpos, NULL, "dump scpos", CLI_SUPERVISOR},
//
//    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
//};

static CLI_EXEC_T _arPmxCmdTbl[] =
{
    {"init", "i", _PmxInitCmd, NULL, "init", CLI_SUPERVISOR},
//    {"enable", "e", _PmxEnableCmd, NULL, "enable/disable LVDS", CLI_GUEST},
    {"pattern", "p", _PmxPatternCmd, NULL, "enable/disable test pattern", CLI_GUEST},
    {"list", "l", _PmxListPanelCmd, NULL, "show panel list", CLI_GUEST},
    {"query", "q", _PmxQueryCmd, NULL, "dump pmx(scpos) info", CLI_GUEST},
    {"set", "s", NULL, _arPmxSetCmdTbl, "set parameter", CLI_GUEST},
//    {"diag", "d", NULL, _arPmxDiagCmdTbl, "verify hardware", CLI_SUPERVISOR},

#ifndef CC_MTK_LOADER
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
    "pmx (scpos) command",
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

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "fgEnable");

    if (PMX_SetEnable(PMX_1, PARAM(0)) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}
#endif

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

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdGamma(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1, "ucGamma{0:off, 1:dark, 2:middle, 3:bright}");

    if (PMX_SetArg(PMX_1, PMX_ARG_GAMMA, PARAM(0)) == PMX_SET_OK)
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
static INT32 _PmxSetCmdBrightness(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1, "ucBrightness");

    if (PMX_SetArg(PMX_1, PMX_ARG_BRIGHTNESS, PARAM(0)) == PMX_SET_OK)
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
static INT32 _PmxSetCmdContrast(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1, "ucContrast");

    if (PMX_SetArg(PMX_1, PMX_ARG_CONTRAST, PARAM(0)) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}

#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "u4BgColor(RGB)");
    PMX_SetBg(PARAM(0), FALSE);

    return 0;
}

#if 0
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

    if (PMX_SetPlaneOrder(PMX_1, PARAM(0), PARAM(1)) == PMX_SET_OK)
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
    au4PlaneOrder[2] = 0;
    au4PlaneOrder[3] = 0;
    if (PMX_SetPlaneOrderArray(PMX_1, au4PlaneOrder) == PMX_SET_OK)
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
    CHK_NUM_MIN_PARAM(1, "backlight(0~255)");

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
#ifndef CC_MTK_LOADER
    UINT32 u4Permillage = DDDS_GetSpreadSpectrum();
    if (i4Argc == 1)
    {
        Printf("current value=%d\n", u4Permillage);
    }

    CHK_NUM_MIN_PARAM(1, "value(0~1000)");

    vDrvSetVOSpread(PARAM(0));
#endif
    return 0;
}

#endif
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
                      "u4HTotal u4HSyncLen u4VTotal\n"
                      "u4HSyncWidth u4VSyncWidth u4HPos u4VPos}\n");
    if (i4Argc == 2)
    {
        SelectPanel(PARAM(0));
        // it's a very dirty hack here...
        // bcuz vDrvSetLCDTiming wiil choose 50Hz as default
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;

        WritePanelIndexToEeprom(PARAM(0));
    }
    else
    {
        UINT32 u4DispClk = PARAM(0);
        UINT32 u4DispWidth = PARAM(1);
        UINT32 u4DispHeight = PARAM(2);
        UINT32 u4HTotal = PARAM(3);
        UINT32 u4HSyncLen = PARAM(4);
        UINT32 u4VTotal = PARAM(5);
        UINT32 u4HSyncWidth = PARAM(6);
        UINT32 u4VSyncWidth = PARAM(7);
        UINT32 u4HPos = PARAM(8);
        UINT32 u4VPos = PARAM(9);

        if (u4DispClk > 0)
        {
            PANEL_SetPixelClk60Hz(u4DispClk);
            PANEL_SetPixelClk50Hz(u4DispClk);
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
            PANEL_SetHTotal60Hz(u4HTotal);
            PANEL_SetHTotal50Hz(u4HTotal);
        }

	UNUSED(u4HSyncLen);
//        if (u4HSyncLen > 0)
//        {
//            PANEL_SetHSyncLen60Hz(u4HSyncLen);
//            PANEL_SetHSyncLen50Hz(u4HSyncLen);
//        }

        if (u4VTotal > 0)
        {
            PANEL_SetVTotal60Hz(u4VTotal);
            PANEL_SetVTotal50Hz(u4VTotal);
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
    vDrvVOPLLSet(PANEL_GetPixelClk60Hz());
    vDrvSetLCDTiming();


    vApiPanelPowerSequence(TRUE);
//    PMX_SetEnable(PMX_1, FALSE);

//    vDrvOutputStageInit();
    //vDrvVOFreqSet(PANEL_GetPixelClk60Hz());
//    if (LVDS_OUTPUT_PORT == DUAL_PORT)
//    {
//        vDrvVOPLLSet(PANEL_GetPixelClk60Hz());
//    }
//    else
//    {
//        vDrvVOPLLSet(PANEL_GetPixelClk60Hz());
//    }
//    vDrvSetLCDTiming();

//    PMX_SetEnable(PMX_1, TRUE);

    DumpPanelAttribute(PANEL_DUMP_CURRENT);

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

#if 0

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Current = bReadSCPOSMsk(0x4, 0xf);
    if (i4Argc == 1)
    {
        Printf("current value=%d\n", u4Current);
    }

    CHK_NUM_MIN_PARAM(1, "value(0~15)");
    u4Current = PARAM(0) & 0xf;
    vWriteSCPOSMsk(0x4, u4Current, 0xf);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDiagCmdGammaTable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Pattern = 0;
    for (; u4Pattern < PMX_GT_PATTERN_MAX; u4Pattern++)
    {
        if (PMX_DIAG_TestGammaTable(u4Pattern))
        {
            Printf("_PmxDiagCmdGammaTable PASS, u4Pattern=%d\n", u4Pattern);
        }
        else
        {
            Printf("_PmxDiagCmdGammaTable FAIL, u4Pattern=%d\n", u4Pattern);
            return -1;
        }
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDiagCmdOutputStage(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2, "type{0:all, 1:offset, 2:gain, 3:backlight},"
                         " delay(ms)");
    PMX_DIAG_TestOutputStage(PARAM(0), PARAM(1));
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDiagCmdWriteback(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_MT5391
    CHK_NUM_MIN_PARAM(1, "u4Addr");
    UINT32 vclip=0, hclip=0, address, area;
    area = PANEL_GetPanelWidth() * PANEL_GetPanelHeight();

    if (GetCurrentPanelIndex() == PANEL_LG_37_WU1)
    {
        vclip = 0x4;
        hclip = 0x8;
    }
    else if (GetCurrentPanelIndex() == PANEL_CMO_27_W1)
    {
        vclip = 0x1e;
        hclip = 0x121;
    }

    // set pscan clk as oclk
    IO_WRITE32MSK(CKGEN_BASE, 0x2a0, 0x3, 0x3);

    // soft reset
    IO_WRITE32(PSCAN_BASE, 0x4c0, 0x1c);

    // Y address, B
    address = PARAM(0);
    IO_WRITE32(PSCAN_BASE, 0x110, address);
    LOG(5, "B address=0x%x\n", address);

    // C address, R
    address = PARAM(0) + area;
    IO_WRITE32(PSCAN_BASE, 0x114, address);
    LOG(5, "R address=0x%x\n", address);

    // resolution
    IO_WRITE32(PSCAN_BASE, 0x148, (PANEL_GetPanelHeight() << 16) |
               PANEL_GetPanelWidth());
    // 10-bit mode
    IO_WRITE32(PSCAN_BASE, 0x14c, 0);
    // 422 write mode
    IO_WRITE32(PSCAN_BASE, 0x150, (0x25 << 24 )| ((vclip & 0xfff) << 12) | (hclip & 0xfff));
    // max buffer number
    IO_WRITE32(PSCAN_BASE, 0x154, 0);
    // ...
    IO_WRITE32(PSCAN_BASE, 0x158, 0);
    // max write limit addr
    IO_WRITE32(PSCAN_BASE, 0x160, 0xffffff0);
    // release soft reset
    IO_WRITE32(PSCAN_BASE, 0x4c0, 0);

    HalFlushInvalidateDCache();

    // enable dma for R,G
    IO_WRITE32(PSCAN_BASE, 0x100, 0x416);
    // remember to turn off MMU in ICE!
    x_thread_delay(area >> 12);
    // disable dma write
    IO_WRITE32(PSCAN_BASE, 0x100, 0x410);

    // Y address, dummy
    address = PARAM(0) + area*2;
    IO_WRITE32(PSCAN_BASE, 0x110, address);

    // C address, G
    address = PARAM(0) + area*3;
    IO_WRITE32(PSCAN_BASE, 0x114, address);
    LOG(5, "G address=0x%x\n", address);

    // enable dma for B,dummy
    IO_WRITE32(PSCAN_BASE, 0x100, 0x41a);
    x_thread_delay(area >> 12);
    // disable dma write
    IO_WRITE32(PSCAN_BASE, 0x100, 0x410);
#else
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on), u4Addr");
    PMX_DIAG_TestWriteback(PARAM(0), PARAM(1));
#endif

    return 0;
}


#endif
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 cmd;
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on)");

    cmd = PARAM(0);
    if (cmd == 0)
    {
        PMX_ShowPattern(FALSE);
        IO_WRITE32MSK(SCPOS_BASE, 0xfc, 0, (1 << 15) | (1 << 13));
        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x7fc, 0, (0xf << 12) | (0xf << 20));
    }
    else if (cmd == 1)
    {
        PMX_ShowPattern(TRUE);
    }
    else if (cmd == 2)
    {
        IO_WRITE32MSK(SCPOS_BASE, 0xfc, 1 << 15, 1 << 15);
    }
    else if (cmd == 3)
    {
        IO_WRITE32MSK(SCPOS_BASE, 0xfc, 1 << 13, 1 << 13);
    }
    else if (cmd == 4)
    {
        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x7fc, 0xf << 12, 0xf << 12);
    }
    else if (cmd == 5)
    {
        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x7fc, 0xf << 20, 0xf << 20);
    }
    return 0;
}


#if 0
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
static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv)
{
#ifndef CC_MTK_LOADER
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
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv)
{
    vWriteSCPOS(RW_LVDS_OMC2, bReadSCPOS(RW_LVDS_OMC2) | MSB_SW);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv)
{
    vWriteSCPOS(RW_LVDS_OMC2, bReadSCPOS(RW_LVDS_OMC2) & (~MSB_SW));
    return 0;
}

#endif
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


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
/*
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetPmxMwCmdTbl(void)
#else
CLI_EXEC_T* GetPmxCmdTbl(void)
#endif
{
    return &_rPmxModCmdTbl;
}
*/


