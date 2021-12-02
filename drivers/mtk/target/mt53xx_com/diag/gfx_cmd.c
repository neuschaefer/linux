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
 * $RCSfile: gfx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_cmd.c
 *  This file contains implementation of CLI CMD for GFX
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e960 -e961 */
#include "x_printf.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG   GFX_IsLog       // for LOG use
#include "x_debug.h"

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "drvcust_if.h"
/*lint -restore */

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 */
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _GFXCMDTEXT2ENUM_T
{
    CHAR *szText;
    INT32 i4Value;
} GFXCMDTEXT2ENUM_T;


typedef enum _ENUM_CLIGFX_ERR_CODE_T
{
    E_CLIGFX_OK          = 0,
    E_CLIGFX_ERR_GENERAL = 1000,
    E_CLIGFX_ERR_PARAM   = 1001,
    // add here
    E_CLIGFX_UNKNOWN     = 1999
} ENUM_CLIGFX_ERR_CODE_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define GFXCMD_CHK_NUM_MIN_PARAM(N)         \
    if (i4Argc < ((N)+1))                   \
    {                                       \
        return -(INT32)E_CLIGFX_ERR_PARAM;  \
    }


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

extern INT32 CLI_Parser(const CHAR* szCmdBuf);

// gfx_debug.h
extern void GFX_DbgDumpInfo(void);

// gfx_hw.h
extern void GFX_HwSetEngClkSrc(INT32 i4ClkSrc);

// gfx_cmdque.h
extern void GFX_CmdQueDbgInfo(void);
extern void GFX_CmdQueSetNewConfig(INT32 i4NewConfig);

// gfx_dif.h
extern void GFX_DifSetMode(UINT32 u4GfxMode);


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _GfxSetCmdDst          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdSrc          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdIdx2DirSrc   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdBltOpt       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdAlpha        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdColor        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdCnvFmt       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdCnvSrc       (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _GfxSetCmdRopCode      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdIdx2DirOpt   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxSetCmdH2VLineOpt   (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _GfxOpCmdRect          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdHline         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdVline         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdBitblt        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdBlend         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdCompose       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdCompPass      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdColorCnv      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdStretchBlt    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdAlphaMapBlt   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdRop           (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmd2DMemCompare  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdComposeLoop   (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdIdx2DirBitblt (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdHori2VertLine (INT32 i4Argc, const CHAR ** szArgv);


// normal driver
#if !defined(GFX_MINI_DRIVER)
static INT32 _GfxSetCmdGradOpt      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdGrad          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdDma           (INT32 i4Argc, const CHAR ** szArgv);
static INT8 _Get71FixPoint(const CHAR *szVal);
#endif // #if !defined(GFX_MINI_DRIVER)


static INT32 _GfxOpCmdStartRectTest (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxOpCmdExitRectTest  (INT32 i4Argc, const CHAR ** szArgv);

#if defined(GFX_ENABLE_SW_MODE)
static INT32 _GfxCmdEnCm            (INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _GfxCmdInit            (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdReset           (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdFlush           (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdQuery           (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdSetEngClkSrc    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdSetQueConfig    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdSetCrashDetect  (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdMemCompare      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdMemSet          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdRotateBmp       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _GfxCmdSlt             (INT32 i4Argc, const CHAR ** szArgv);

static UINT8* _GetAddress(const CHAR *szAddress);
static INT32 _GetEnum(const CHAR *szText, const GFXCMDTEXT2ENUM_T* prText2Enum);
static void _GfxOpCmdRectTestMainLoop(void *pvArg);
static INT32 _GfxSlt(UINT32 u4Loop, UINT32 u4SwEn);

CLIMOD_DEBUG_FUNCTIONS(GFX)     // for LOG use


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static INT32 _i4DoRectTest = (INT32) FALSE;
static UINT8 *_pu1RectTestDst = NULL;
static UINT8 *_pu1RectTestAlignedDst = NULL;
static UINT32 _u4RectTestDstPitch;
static UINT32 _u4RectTestWidth;
static UINT32 _u4RectTestHeight;

// set table
static CLI_EXEC_T _arGfxSetCmdTbl[] =
{
    {
        "dst", "d", _GfxSetCmdDst, NULL, 
        "Set destination buffer", CLI_SUPERVISOR
    },
    {
        "src", "s", _GfxSetCmdSrc, NULL, 
        "Set source buffer", CLI_SUPERVISOR
    },
    {
        "idx2dirsrc", NULL, _GfxSetCmdIdx2DirSrc, NULL, 
        "Set idx2dir source buffer", CLI_SUPERVISOR
    },
    {
        "bltopt", "bopt", _GfxSetCmdBltOpt, NULL, 
        "Set bitblt option", CLI_SUPERVISOR
    },
    {
        "alpha", "a", _GfxSetCmdAlpha, NULL, 
        "Set alpha value", CLI_SUPERVISOR
    },
    {
        "color", "c", _GfxSetCmdColor, NULL, 
        "Set color value", CLI_SUPERVISOR
    },
    {
        "cnvfmt", "cfmt", _GfxSetCmdCnvFmt, NULL, 
        "Set color conversion format", CLI_SUPERVISOR
    },
    {
        "cnvsrc", "csrc", _GfxSetCmdCnvSrc, NULL, 
        "Set color conversion source", CLI_SUPERVISOR
    },
    {
        "ropcode", "rcode", _GfxSetCmdRopCode, NULL, 
        "Set ROP op code", CLI_SUPERVISOR
    },
    {
        "idx2diropt", NULL, _GfxSetCmdIdx2DirOpt, NULL, 
        "Set Index to direct color bitblt option", CLI_SUPERVISOR
    },
    {
        "h2vlineopt", NULL, _GfxSetCmdH2VLineOpt, NULL, 
        "Set hori lint to vert line option (Is90dCcw)", CLI_SUPERVISOR
    },


// normal driver
#if !defined(GFX_MINI_DRIVER)
    {
        "gradopt", "gopt", _GfxSetCmdGradOpt, NULL, 
        "Set gradient fill option", CLI_SUPERVISOR
    },
#endif // #if !defined(GFX_MINI_DRIVER)

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// operation table
static CLI_EXEC_T _arGfxOpCmdTbl[] =
{
    {
        "rect", "r", _GfxOpCmdRect, NULL, 
        "Draw rectangle", CLI_SUPERVISOR
    },
    {
        "hline", "h", _GfxOpCmdHline, NULL, 
        "Draw h-line",  CLI_SUPERVISOR
    },
    {
        "vline", "v", _GfxOpCmdVline, NULL, 
        "Draw v-line", CLI_SUPERVISOR
    },
    {
        "bitblt", "blt", _GfxOpCmdBitblt, NULL, 
        "Bitblt", CLI_SUPERVISOR
    },
    {
        "blend", "b", _GfxOpCmdBlend, NULL, 
        "Alpha blending bitblt", CLI_SUPERVISOR
    },
    {
        "compose", "c", _GfxOpCmdCompose, NULL, 
        "Alpha composition bitblt", CLI_SUPERVISOR
    },
    {
        "comppass", "cp", _GfxOpCmdCompPass, NULL, 
        "Alpha composition pass", CLI_SUPERVISOR
    },
    {
        "colorcnv", "cnv", _GfxOpCmdColorCnv, NULL, 
        "Color format convert", CLI_SUPERVISOR
    },
    {
        "stretchblt", "s", _GfxOpCmdStretchBlt, NULL, 
        "Stretch bitblt", CLI_SUPERVISOR
    },
    {
        "alphamapblt", "am", _GfxOpCmdAlphaMapBlt, NULL, 
        "Alpha map bitblt", CLI_SUPERVISOR
    },
    {
        "rop", NULL, _GfxOpCmdRop, NULL, 
        "Raster Operation", CLI_SUPERVISOR
    },
    {
        "2dmemcomp", NULL, _GfxOpCmd2DMemCompare, NULL, 
        "2D Memory Compare Operation", CLI_SUPERVISOR
    },
    {
        "composeloop", "cl", _GfxOpCmdComposeLoop, NULL, 
        "Alpha composition loop bitblt", CLI_SUPERVISOR
    },
    {
        "idx2dirblt", NULL, _GfxOpCmdIdx2DirBitblt, NULL, 
        "Index to direct color bitblt", CLI_SUPERVISOR
    },
    {
        "h2vline", NULL, _GfxOpCmdHori2VertLine, NULL, 
        "Horizontal line to vertical line operation", CLI_SUPERVISOR
    },
    

// normal driver
#if !defined(GFX_MINI_DRIVER)
    {
        "grad", "g", _GfxOpCmdGrad, NULL, 
        "Draw gradient fill", CLI_SUPERVISOR
    },
    {
        "dma", "d", _GfxOpCmdDma, NULL, 
        "1-D bitblt", CLI_SUPERVISOR
    },
#endif // #if !defined(GFX_MINI_DRIVER)

    {
        "srt", NULL, _GfxOpCmdStartRectTest, NULL, 
        "Start Rect-fill Test (av.dram measurement)", CLI_SUPERVISOR
    },
    {
        "ert", NULL, _GfxOpCmdExitRectTest, NULL, 
        "Exit Rect-fill Test (av.dram measurement)", CLI_SUPERVISOR
    },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


// cmd table
static CLI_EXEC_T _arGfxCmdTbl[] =
{
#if defined(GFX_ENABLE_SW_MODE)
    {
        "encm", NULL, _GfxCmdEnCm, NULL, 
        "Enable GFX C-model", CLI_SUPERVISOR
    },
#endif
    {
        "slt", NULL, _GfxCmdSlt, NULL, 
        "Gfx SLT", CLI_SUPERVISOR
    },
    {
        "init", "i", _GfxCmdInit, NULL, 
        "Gfx init", CLI_SUPERVISOR
    },
    {
        "reset", "res", _GfxCmdReset, NULL, 
        "Gfx reset", CLI_SUPERVISOR
    },
    {
        "flush", "f", _GfxCmdFlush, NULL, 
        "Gfx command queue flush (auto on off)", CLI_SUPERVISOR
    },
    {
        "query", "q", _GfxCmdQuery, NULL, 
        "Gfx Query HW status", CLI_SUPERVISOR
    },
    {
        "clksrc", "clk", _GfxCmdSetEngClkSrc, NULL, 
        "Gfx engine clock source", CLI_SUPERVISOR
    },
    {
        "cmdqcfg", "qcfg", _GfxCmdSetQueConfig, NULL, 
        "Gfx engine cmdque configuration", CLI_SUPERVISOR
    },
    {
        "crashd", NULL, _GfxCmdSetCrashDetect, NULL, 
        "Gfx engine crash detection", CLI_SUPERVISOR
    },
    {
        "memcomp", NULL, _GfxCmdMemCompare, NULL, 
        "Memory Compare Operation", CLI_SUPERVISOR
    },
    {
        "memset", NULL, _GfxCmdMemSet, NULL, 
        "Memory Set Operation", CLI_SUPERVISOR
    },
    {
        "rotate", NULL, _GfxCmdRotateBmp, NULL, 
        "Rotate Operation", CLI_SUPERVISOR
    },
    {
        "set", "s", NULL, _arGfxSetCmdTbl, 
        "Gfx parameter set", CLI_SUPERVISOR
    },
    {
        "op", NULL, NULL, _arGfxOpCmdTbl, 
        "Gfx parameter set", CLI_SUPERVISOR
    },

    CLIMOD_DEBUG_CLIENTRY(GFX),     // for LOG use

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static GFXCMDTEXT2ENUM_T _arColorModeEnumTbl[] =
{
    {"argb8888",    14},
    {"argb4444",    13},
    {"argb1555",    12},
    {"rgb565",      11},
    {"rgb8",        10},
    {"rgb4",         9},
    {"rgb2",         8},
    {"aycbcr8888",   6},
    {"ycbycr422",    5},
    {"cbycry422",    4},
    {"ycbcr8",       2},
    {"ycbcr4",       1},
    {"ycbcr2",       0},
    {"ARGB8888",    14},
    {"ARGB4444",    13},
    {"ARGB1555",    12},
    {"RGB565",      11},
    {"RGB8",        10},
    {"RGB4",         9},
    {"RGB2",         8},
    {"AYCBCR8888",   6},
    {"YCBYCR422",    5},
    {"CBYCRY422",    4},
    {"YCBCR8",       2},
    {"YCBCR4",       1},
    {"YCBCR2",       0},
    {NULL,          14}
};


static GFXCMDTEXT2ENUM_T _arBltOptEnumTbl[] =
{
    {"transparent", D_GFXFLAG_TRANSPARENT},
    {"keynot",      D_GFXFLAG_KEYNOT},
    {"colorchange", D_GFXFLAG_COLORCHANGE},
    {"keysdsel",    D_GFXFLAG_KEYSDSEL},
    {"flip",        D_GFXFLAG_FLIP},
    {"mirror",      D_GFXFLAG_MIRROR},
    {"overlap",      D_GFXFLAG_OVERLAP},    
    {"TRANSPARENT", D_GFXFLAG_TRANSPARENT},
    {"COLORCHANGE", D_GFXFLAG_COLORCHANGE},
    {"KEYNOT",      D_GFXFLAG_KEYNOT},
    {"KEYSDSEL",    D_GFXFLAG_KEYSDSEL},
    {"FLIP",        D_GFXFLAG_FLIP},
    {"MIRROR",      D_GFXFLAG_MIRROR},
    {"OVERLAP",      D_GFXFLAG_OVERLAP},
    {NULL,          0}
};


static GFXCMDTEXT2ENUM_T _arYcFmtEnumTbl[] =
{
    {"420mb",   (INT32)E_YCFMT_420MB},
    {"420lin",  (INT32)E_YCFMT_420LINEAR},
    {"422lin",  (INT32)E_YCFMT_422LINEAR},
    {"420MB",   (INT32)E_YCFMT_420MB},
    {"420LIN",  (INT32)E_YCFMT_420LINEAR},
    {"422LIN",  (INT32)E_YCFMT_422LINEAR},
    {NULL,      (INT32)E_YCFMT_420MB}
};


static GFXCMDTEXT2ENUM_T _arSwapModeEnumTbl[] =
{
    {"mergetop",    (INT32)E_SWAP_MERGETOP},
    {"swap",        (INT32)E_SWAP_SWAP},
    {"block",       (INT32)E_SWAP_BLOCK},
    {"MERGETOP",    (INT32)E_SWAP_MERGETOP},
    {"SWAP",        (INT32)E_SWAP_SWAP},
    {"BLOCK",       (INT32)E_SWAP_BLOCK},
    {"0",           (INT32)E_SWAP_0},
    {"1",           (INT32)E_SWAP_1},
    {"2",           (INT32)E_SWAP_2},
    {NULL,          (INT32)E_SWAP_BLOCK}
};


static GFXCMDTEXT2ENUM_T _arVidStdEnumTbl[] =
{
    {"bt601",   (INT32)E_VSTD_BT601},
    {"bt709",   (INT32)E_VSTD_BT709},
    {"BT601",   (INT32)E_VSTD_BT601},
    {"BT709",   (INT32)E_VSTD_BT709},
    {"601",     (INT32)E_VSTD_BT601},
    {"709",     (INT32)E_VSTD_BT709},
    {NULL,      0}
};


static GFXCMDTEXT2ENUM_T _arVidSysEnumTbl[] =
{
    {"vid",         (INT32)E_VSYS_VID},
    {"comp",        (INT32)E_VSYS_COMP},
    {"video",       (INT32)E_VSYS_VID},
    {"computer",    (INT32)E_VSYS_COMP},
    {"VID",         (INT32)E_VSYS_VID},
    {"COMP",        (INT32)E_VSYS_COMP},
    {"VIDEO",       (INT32)E_VSYS_VID},
    {"COMPUTER",    (INT32)E_VSYS_COMP},
    {NULL,          (INT32)E_VSYS_VID}
};

static GFXCMDTEXT2ENUM_T _arBmpCmEnumTbl[] =
{
    {"1bit",    (INT32)E_BMP_CM_1BIT},
    {"2bit",    (INT32)E_BMP_CM_2BIT},
    {"4bit",    (INT32)E_BMP_CM_4BIT},
    {"8bit",    (INT32)E_BMP_CM_8BIT},
    {"1BIT",    (INT32)E_BMP_CM_1BIT},
    {"2BIT",    (INT32)E_BMP_CM_2BIT},
    {"4BIT",    (INT32)E_BMP_CM_4BIT},
    {"8BIT",    (INT32)E_BMP_CM_8BIT},
    {"1",       (INT32)E_BMP_CM_1BIT},
    {"2",       (INT32)E_BMP_CM_2BIT},
    {"4",       (INT32)E_BMP_CM_4BIT},
    {"8",       (INT32)E_BMP_CM_8BIT},
    {NULL,      (INT32)E_BMP_CM_8BIT}
};

// normal driver
#if !defined(GFX_MINI_DRIVER)
static GFXCMDTEXT2ENUM_T _arGradFillModeEnumTbl[] =
{
    {"h",           (INT32)E_GRAD_HOR},
    {"v",           (INT32)E_GRAD_VER},
    {"b",           (INT32)E_GRAD_BOTH},
    {"hor",         (INT32)E_GRAD_HOR},
    {"ver",         (INT32)E_GRAD_VER},
    {"both",        (INT32)E_GRAD_BOTH},
    {"horizontal",  (INT32)E_GRAD_HOR},
    {"verical",     (INT32)E_GRAD_VER},
    {"H",           (INT32)E_GRAD_HOR},
    {"V",           (INT32)E_GRAD_VER},
    {"B",           (INT32)E_GRAD_BOTH},
    {"HOR",         (INT32)E_GRAD_HOR},
    {"VER",         (INT32)E_GRAD_VER},
    {"BOTH",        (INT32)E_GRAD_BOTH},
    {"HORIZONTAL",  (INT32)E_GRAD_HOR},
    {"VERICAL",     (INT32)E_GRAD_VER},
    {NULL,          (INT32)E_GRAD_BOTH}
};
#endif // #if !defined(GFX_MINI_DRIVER)


static INT32 _i4FlushMode = 1;


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GfxSetCmdDst
 *  gfx.set.dst {buff_base_address} {color_mode} {pitch_in_bytes}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdDst(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Base;
    UINT32 u4ColorMode, u4Pitch;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return -1;
    }
    
    pu1Base     = _GetAddress(szArgv[1]);
    u4ColorMode = (UINT32)_GetEnum(szArgv[2], _arColorModeEnumTbl);
    u4Pitch     = (UINT32)StrToInt(szArgv[3]);

    GFX_SetDst(pu1Base, u4ColorMode, u4Pitch);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdSrc
 *  gfx.set.src {buff_base_address} {color_mode} {pitch_in_bytes}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdSrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Base;
    UINT32 u4ColorMode, u4Pitch;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return -1;
    }

    pu1Base     = _GetAddress(szArgv[1]);
    u4ColorMode = (UINT32)_GetEnum(szArgv[2], _arColorModeEnumTbl);
    u4Pitch     = (UINT32)StrToInt(szArgv[3]);

    GFX_SetSrc(pu1Base, u4ColorMode, u4Pitch);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdIdx2DirSrc
 *  gfx.set.idx2dirsrc {buff_base_address} {char_cm}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdIdx2DirSrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Base;
    UINT32 u4CharCM;
    GFXCMD_CHK_NUM_MIN_PARAM(2);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return -1;
    }

    pu1Base  = _GetAddress(szArgv[1]);
    u4CharCM = (UINT32)_GetEnum(szArgv[2], _arBmpCmEnumTbl);

    GFX_SetIdx2DirSrc(pu1Base, u4CharCM);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdBltOpt
 *  gfx.set.bltopt {color_min} {color_max} [{opt1} [{opt2} ...]]
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdBltOpt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Index;
    UINT32 u4Switches, u4ColorMin, u4ColorMax;
    GFXCMD_CHK_NUM_MIN_PARAM(2);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return -1;
    }
    
    u4ColorMin = (UINT32)StrToInt(szArgv[1]);
    u4ColorMax = (UINT32)StrToInt(szArgv[2]);
    u4Switches = 0;
    i4Index    = 3;    // opts start from 3
    i4Argc    -= 3;
    while (i4Argc)
    {
        u4Switches |= (UINT32)_GetEnum(szArgv[i4Index], _arBltOptEnumTbl);
        i4Index++;
        i4Argc--;
    }

    GFX_SetBltOpt(u4Switches, u4ColorMin, u4ColorMax);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdAlpha
 *  gfx.set.alpha {alpha}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Alpha;
    GFXCMD_CHK_NUM_MIN_PARAM(1);

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    u4Alpha = StrToInt(szArgv[1]);
    if (u4Alpha > 255)
    {
        u4Alpha = 255;
    }

    GFX_SetAlpha(u4Alpha);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdColor
 *  gfx.set.color {color}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdColor(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Color;
    GFXCMD_CHK_NUM_MIN_PARAM(1);

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }

    u4Color = (UINT32)StrToInt(szArgv[1]);

    GFX_SetColor(u4Color);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdCnvFmt
 *  gfx.set.cnvfmt {yc_fmt} {swap_mode} {vid_std} {vid_sys}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdCnvFmt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4YcFmt, u4SwapMode, u4VidStd, u4VidSys;
    GFXCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || 
        (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        return -1;
    }
    
    u4YcFmt    = (UINT32)_GetEnum(szArgv[1], _arYcFmtEnumTbl);
    u4SwapMode = (UINT32)_GetEnum(szArgv[2], _arSwapModeEnumTbl);
    u4VidStd   = (UINT32)_GetEnum(szArgv[3], _arVidStdEnumTbl);
    u4VidSys   = (UINT32)_GetEnum(szArgv[4], _arVidSysEnumTbl);

    GFX_SetColCnvFmt(u4YcFmt, u4SwapMode, u4VidStd, u4VidSys);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdCnvSrc
 *  gfx.set.cnvsrc {luma} {luma_pitch} {chroma} {chroma_pitch} [{field}]
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdCnvSrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Luma, *pu1Chroma;
    UINT32 u4LumaPitch, u4ChromaPitch;
    UINT32 u4FieldPic = 0;
    GFXCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || 
        (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        return -1;
    }
    
    pu1Luma       = _GetAddress(szArgv[1]);
    u4LumaPitch   = (UINT32) StrToInt(szArgv[2]);
    pu1Chroma     = _GetAddress(szArgv[3]);
    u4ChromaPitch = (UINT32) StrToInt(szArgv[4]);

    if (i4Argc >= 5)
    {
        if ((x_strncmp("field", szArgv[5], 5) == 0) ||
            (x_strncmp("FIELD", szArgv[5], 5) == 0))
        {
            u4FieldPic = 1;
        }
    }

    GFX_SetColCnvSrc(pu1Luma, u4LumaPitch, pu1Chroma, u4ChromaPitch, u4FieldPic);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdRopCode
 *  gfx.set.ropcode {opcode}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdRopCode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4RopCode;
    GFXCMD_CHK_NUM_MIN_PARAM(1);

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }

    u4RopCode = StrToInt(szArgv[1]);

    if (u4RopCode >= (UINT32)E_ROP_MAX)
    {
        return -1;
    }
    else
    {
        GFX_SetRopOpt(u4RopCode);
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdIdx2DirOpt
 *  gfx.set.idx2diropt {palet_addr} {msb_left} {start_byte_align}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdIdx2DirOpt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Base;
    UINT32 u4MsbLeft, u4StByteAl;

    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }
    
    pu1Base    = _GetAddress(szArgv[1]);
    u4MsbLeft  = (UINT32) StrToInt(szArgv[2]);
    u4StByteAl = (UINT32) StrToInt(szArgv[3]);

    GFX_SetIdx2DirOpt(pu1Base, u4MsbLeft, u4StByteAl);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxSetCmdH2VLineOpt
 *  gfx.set.h2vlineopt {Is90dCcw}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdH2VLineOpt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Is90dCcw;
    GFXCMD_CHK_NUM_MIN_PARAM(1);

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }

    u4Is90dCcw = StrToInt(szArgv[1]);

    if (u4Is90dCcw >= (UINT32)E_H2VLINE_MAX)
    {
        return -1;
    }
    else
    {
        GFX_SetHoriToVertLineOpt(u4Is90dCcw);
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdRect
 *  gfx.op.rect {x} {y} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdRect(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 x, y, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        return -1;
    }
    
    x = (UINT32) StrToInt(szArgv[1]);
    y = (UINT32) StrToInt(szArgv[2]);
    w = (UINT32) StrToInt(szArgv[3]);
    h = (UINT32) StrToInt(szArgv[4]);

    GFX_Fill(x, y, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdHline
 *  gfx.op.hline {x} {y} {w}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdHline(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 x, y, w;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }

    x = (UINT32) StrToInt(szArgv[1]);
    y = (UINT32) StrToInt(szArgv[2]);
    w = (UINT32) StrToInt(szArgv[3]);

    GFX_HLine(x, y, w);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdVline
 *  gfx.op.vline {x} {y} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdVline(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 x, y, h;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }

    x = (UINT32) StrToInt(szArgv[1]);
    y = (UINT32) StrToInt(szArgv[2]);
    h = (UINT32) StrToInt(szArgv[3]);
    
    GFX_VLine(x, y, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdBitblt
 *  gfx.op.bitblt {sx} {sy} {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdBitblt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);

    GFX_BitBlt(sx, sy, dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdBlend
 *  gfx.op.blend {sx} {sy} {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdBlend(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL))
    {
        return -1;
    }

    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);

    GFX_Blend(sx, sy, dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdCompose
 *  gfx.op.compose {sx} {sy} {dx} {dy} {w} {h} {ar} {mode}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdCompose(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h, ar, u4Mode;
    GFXCMD_CHK_NUM_MIN_PARAM(8);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || (szArgv[8] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);
    ar = (UINT32) StrToInt(szArgv[7]);
    u4Mode = (UINT32) StrToInt(szArgv[8]);

    _GFX_Compose(sx, sy, dx, dy, w, h, ar, u4Mode);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdCompPass
 *  gfx.op.comppass {sx} {sy} {dx} {dy} {w} {h} {pass} {param}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdCompPass(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h, pass, param;
    GFXCMD_CHK_NUM_MIN_PARAM(8);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || (szArgv[8] == NULL))
    {
        return -1;
    }

    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);
    pass  = (UINT32) StrToInt(szArgv[7]);
    param = (UINT32) StrToInt(szArgv[8]);

    GFX_AlphaComposePass(sx, sy, dx, dy, w, h, pass, param);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdColorCnv
 *  gfx.op.colorcnv {x} {y} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdColorCnv(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 x, y, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL))
    {
        return -1;
    }
    
    x = (UINT32) StrToInt(szArgv[1]);
    y = (UINT32) StrToInt(szArgv[2]);
    w = (UINT32) StrToInt(szArgv[3]);
    h = (UINT32) StrToInt(szArgv[4]);

    GFX_ColConv(x, y, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdStretchBlt
 *  gfx.op.stretchblt {sx} {sy} {sw} {sh} {dx} {dy} {dw} {dh}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdStretchBlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, sw, sh, dx, dy, dw, dh;
    GFXCMD_CHK_NUM_MIN_PARAM(8);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || (szArgv[8] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    sw = (UINT32) StrToInt(szArgv[3]);
    sh = (UINT32) StrToInt(szArgv[4]);
    dx = (UINT32) StrToInt(szArgv[5]);
    dy = (UINT32) StrToInt(szArgv[6]);
    dw = (UINT32) StrToInt(szArgv[7]);
    dh = (UINT32) StrToInt(szArgv[8]);

    GFX_StretchBlt(sx, sy, sw, sh, dx, dy, dw, dh);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdAlphaMapBlt
 *  gfx.op.am {sx} {sy} {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdAlphaMapBlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);

    GFX_AlphaMapBitBlt(sx, sy, dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdRop
 *  gfx.op.rop {sx} {sy} {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdRop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);

    GFX_RopBitBlt(sx, sy, dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmd2DMemCompare
 *  gfx.op.2dmemcomp {sx} {sy} {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmd2DMemCompare(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);

    GFX_2DMemCompare(sx, sy, dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdComposeLoop
 *  gfx.op.composeloop {sx} {sy} {dx} {dy} {w} {h} {ar} {op} {rt}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdComposeLoop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w, h, ar, op, rt;
    GFXCMD_CHK_NUM_MIN_PARAM(9);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || 
        (szArgv[8] == NULL) || (szArgv[9] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);
    h  = (UINT32) StrToInt(szArgv[6]);
    ar = (UINT32) StrToInt(szArgv[7]);
    op = (UINT32) StrToInt(szArgv[8]);  // OpCode
    rt = (UINT32) StrToInt(szArgv[9]);  // RectSrc

    GFX_ComposeLoop(sx, sy, dx, dy, w, h, ar, op, rt);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdIdx2DirBitblt
 *  gfx.op.idx2dirblt {dx} {dy} {w} {h}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdIdx2DirBitblt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 dx, dy, w, h;
    GFXCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL))
    {
        return -1;
    }
    
    dx = (UINT32) StrToInt(szArgv[1]);
    dy = (UINT32) StrToInt(szArgv[2]);
    w  = (UINT32) StrToInt(szArgv[3]);
    h  = (UINT32) StrToInt(szArgv[4]);

    GFX_Idx2DirBitBlt(dx, dy, w, h);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdHori2VertLine
 *  gfx.op.h2vline {srcx} {srcy} {dstx} {dstx} {width}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdHori2VertLine(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sx, sy, dx, dy, w;
    GFXCMD_CHK_NUM_MIN_PARAM(5);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
        return -1;
    }
    
    sx = (UINT32) StrToInt(szArgv[1]);
    sy = (UINT32) StrToInt(szArgv[2]);
    dx = (UINT32) StrToInt(szArgv[3]);
    dy = (UINT32) StrToInt(szArgv[4]);
    w  = (UINT32) StrToInt(szArgv[5]);

    GFX_HoriToVertLine(sx, sy, dx, dy, w);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}



// normal driver
#if !defined(GFX_MINI_DRIVER)
//-------------------------------------------------------------------------
/** _GfxSetCmdGradOpt
 *  gfx.set.gradopt {inc_x} {inc_y} {dx_a} {dx_r} {dx_g} {dx_b} {dy_a} {dy_r} {dy_g} {dy_b}
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCmdGradOpt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    INT8 ai1Dx[4], ai1Dy[4];    //7.1 fix point
    UINT32 au4Dx[4], au4Dy[4];
    UINT32 u4IncX, u4IncY;
    GFXCMD_CHK_NUM_MIN_PARAM(10);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || 
        (szArgv[8] == NULL) || (szArgv[9] == NULL) || (szArgv[10] == NULL))
    {
        return -1;
    }
    
    u4IncX = StrToInt(szArgv[1]);
    u4IncY = StrToInt(szArgv[2]);

    ai1Dx[3] = _Get71FixPoint(szArgv[3]);
    ai1Dx[2] = _Get71FixPoint(szArgv[4]);
    ai1Dx[1] = _Get71FixPoint(szArgv[5]);
    ai1Dx[0] = _Get71FixPoint(szArgv[6]);
    
    ai1Dy[3] = _Get71FixPoint(szArgv[7]);
    ai1Dy[2] = _Get71FixPoint(szArgv[8]);
    ai1Dy[1] = _Get71FixPoint(szArgv[9]);
    ai1Dy[0] = _Get71FixPoint(szArgv[10]);

    for (i = 0; i < 4; i++)
    {
        au4Dx[i] = (UINT32)(ai1Dx[i] & 0xff);
        au4Dy[i] = (UINT32)(ai1Dy[i] & 0xff);
    }

    _GFX_SetGradOpt(u4IncX, u4IncY, au4Dx, au4Dy);
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdGrad
 *  gfx.op.grad {x} {y} {w} {h} {mode}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdGrad(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 x, y, w, h, u4Mode;
    GFXCMD_CHK_NUM_MIN_PARAM(5);

    if ((szArgv == NULL) || 
        (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
        return -1;
    }

    x = (UINT32) StrToInt(szArgv[1]);
    y = (UINT32) StrToInt(szArgv[2]);
    w = (UINT32) StrToInt(szArgv[3]);
    h = (UINT32) StrToInt(szArgv[4]);
    u4Mode = (UINT32) _GetEnum(szArgv[5], _arGradFillModeEnumTbl);

    _GFX_GradFill(x, y, w, h, u4Mode);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdDma
 *  gfx.op.dma {dst} {src} {nword}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdDma(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 *pu4Dst, *pu4Src, u4NumWord;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }
    
    pu4Dst    = (UINT32*) _GetAddress(szArgv[1]);
    pu4Src    = (UINT32*) _GetAddress(szArgv[2]);
    u4NumWord = (UINT32) StrToInt(szArgv[3]);

    _GFX_DMA(pu4Dst, pu4Src, u4NumWord);
    if (_i4FlushMode)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _Get71FixPoint
 *  
 */
//-------------------------------------------------------------------------
static INT8 _Get71FixPoint(const CHAR *szVal)
{
    INT8 i1Value71 = 0;
    INT8 i1Sign = 1;
    INT8 i1Point = 0;

    if (NULL == szVal)
    {
        return 0;
    }


    if (*szVal == '-')
    {
        i1Sign = -1;
        szVal++;
    }

    while (((*szVal >= (INT8)'0') && (*szVal <= (INT8)'9')) || (*szVal == '.'))
    {
        if (*szVal == '.')
        {
            i1Point = ((szVal[1] >= '5') && (szVal[1] <= '9')) ? 1 : 0;
            break;
        }
        else
        {
            i1Value71 = ((i1Value71 * 10) + (*szVal - (INT8)'0'));
            if (((i1Sign == 1) && (i1Value71 >= 63)) || ((i1Sign == -1) && (i1Value71 >= 64)))
            {
                break;
            }
            szVal++;
        }
    }

    return i1Sign * ((i1Value71 * 2) + i1Point);
}
#endif // #if !defined(GFX_MINI_DRIVER)



//-------------------------------------------------------------------------
/** _GfxOpCmdStartRectTest
 *  gfx.op.srt {thread_delay}
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdStartRectTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ThreadPriority = 240;
    UINT32 u4DstBufSize, u4Align, u4Delay, u4Dst;
    UINT32 u4BytePerPix = 4;    // 32-bpp (argb8888)
    SIZE_T zStakSize = 4096;
    SIZE_T zArgSize = 4;
    HANDLE_T hThread;

    //GFXCMD_CHK_NUM_MIN_PARAM(3);

    if (i4Argc < 4)
    {
        Printf("arg: {w} {h} {thread_delay}\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }

    _u4RectTestWidth  = StrToInt(szArgv[1]);
    _u4RectTestHeight = StrToInt(szArgv[2]);
    u4Delay           = StrToInt(szArgv[3]);

    if (_pu1RectTestDst == NULL)
    {
        u4Align = 128;
        _u4RectTestDstPitch = (_u4RectTestWidth * u4BytePerPix);
        u4DstBufSize = (_u4RectTestWidth * _u4RectTestHeight * u4BytePerPix);
        _pu1RectTestDst = (UINT8 *)x_mem_alloc(u4DstBufSize + 512);
        VERIFY(_pu1RectTestDst != NULL);

        // alignment
        u4Dst = (UINT32)_pu1RectTestDst;
        u4Dst = (u4Dst + u4Align) & (~(u4Align - 1));
        _pu1RectTestAlignedDst = (UINT8 *)u4Dst;

        //Printf("        Dst Buffer Addr = 0x%08x\n", _pu1RectTestDst);
        //Printf("Aligned Dst Buffer Addr = 0x%08x\n", _pu1RectTestAlignedDst);
    }

    // create a thread
    _i4DoRectTest = (INT32) TRUE;
    VERIFY(x_thread_create(&hThread, "GFX_RECT_TEST", zStakSize, u1ThreadPriority,  
        _GfxOpCmdRectTestMainLoop, zArgSize, ((void *)&u4Delay)) == OSR_OK);

    return (INT32)E_CLIGFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdExitRectTest
 *  gfx.op.ert
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpCmdExitRectTest(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv != NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    _i4DoRectTest = (INT32) FALSE;
    return (INT32)E_CLIGFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxCmdInit
 *  gfx.init
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    GFX_Init();
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdEnCm
 *  gfx.encm
 */
//-------------------------------------------------------------------------
#if defined(GFX_ENABLE_SW_MODE)
static INT32 _GfxCmdEnCm(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4EnCm = 0;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc < 2)
    {
        Printf("arg: EnCm (1:on, 0:off)\n");
        return -1;
    }

    u4EnCm = (UINT32) StrToInt(szArgv[1]);
    
    if (u4EnCm)
    {
        GFX_Reset();
        GFX_DifSetMode((UINT32)E_GFX_SW_MOD);
        Printf("Enable GFX C-model\n");
    }
    else
    {
        GFX_Reset();
        GFX_DifSetMode((UINT32)E_GFX_HW_MOD);
        Printf("Disable GFX C-model\n");
    }

    return 0;
}
#endif


//-------------------------------------------------------------------------
/** _GfxCmdReset
 *  gfx.reset
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    // prevent emulation program from resetting gfx engine before returning interrupt
    GFX_Wait();

    GFX_Reset();
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdFlush
 *  gfx.flush [{auto, delayed|off}]
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdFlush(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    if ((i4Argc >= 2) &&
        ((x_strncmp("auto", szArgv[1], 4) == 0) ||
         (x_strncmp("AUTO", szArgv[1], 4) == 0)))
    {
        _i4FlushMode = 1;
    }
    else
    {
        _i4FlushMode = 0;
    }

    if (i4Argc == 1)
    {
        GFX_Flush();
        GFX_Wait();
    }
    return (INT32)E_CLIGFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxCmdQuery
 *  gfx.query
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdQuery           (INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("current hw status = %s\n", GFX_QueryHwIdle() ? "idle" : "busy");

#if defined(GFX_DEBUG_MODE)
    Printf("    << In GFX Debug Mode >>\n");
    Printf("(if gfx engine status is idle, "    \
        "flush count must be euqal to interrupt count)\n");

    GFX_DbgDumpInfo();
   // GFX_CmdQu_eDbgInfo();
#endif // #if defined(GFX_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdSetEngClkSrc
 *  gfx.clksrc
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdSetEngClkSrc    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(GFX_DEBUG_MODE)

    INT32 i4ClkSrc = 0;
    INT32 i4ClkSrcMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: clk_src_num\n");
        Printf("   0 : xtal clk\n");
        Printf("   1 : syspll / 2  (162 / 216 MHz)\n");
        Printf("   2 : tvdpll / 3  (180   MHz)\n");
        Printf("   3 : tvdpll / 5  (108   MHz)\n");
        Printf("   4 : dtdpll / 3  (200   MHz)\n");
        Printf("   5 : dtdpll / 4  (150   MHz)\n");
        Printf("   6 : pspll  / 1  (148.5 MHz)\n");
        Printf("   7 : mem\n");
        return -1;
    }
    
    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4ClkSrc  = (INT32) StrToInt(szArgv[1]);

    if (i4ClkSrc >= i4ClkSrcMax)
    {
        Printf("Error: clk src = %d\n", i4ClkSrc);
        return -1;
    }
    
    GFX_HwSetEngClkSrc(i4ClkSrc);
    Printf("GRA_CLK_ADDR : 0x2000D224\n");
#endif // #if defined(GFX_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdSetQueConfig
 *  gfx.cmdqcfg
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdSetQueConfig    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(GFX_DEBUG_MODE)

    INT32 i4QueCfg = 0;
    INT32 i4QueCfgMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: cmdq_cfg\n");
        Printf("   0 : cmdq_002_KB\n");
        Printf("   1 : cmdq_004_KB\n");
        Printf("   2 : cmdq_008_KB\n");
        Printf("   3 : cmdq_016_KB\n");
        Printf("   4 : cmdq_032_KB\n");
        Printf("   5 : cmdq_064_KB\n");
        Printf("   6 : cmdq_128_KB\n");
        Printf("   7 : cmdq_256_KB\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4QueCfg  = (INT32) StrToInt(szArgv[1]);

    if (i4QueCfg >= i4QueCfgMax)
    {
        Printf("Error: cmdq_cfg = %d\n", i4QueCfg);
        return -1;
    }
    
    GFX_CmdQueSetNewConfig(i4QueCfg);
#endif // #if defined(GFX_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdSetCrashDetect
 *  gfx.crashd
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdSetCrashDetect    (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value = 0;

    if (i4Argc < 2)
    {
        Printf("\narg:\n");
        Printf("   0 : disable crash detection\n");
        Printf("   1 : enable  crash detection\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    u4Value = (UINT32) StrToInt(szArgv[1]);
    
    _GFX_SetCrashDetection(u4Value);

    Printf("GFX engine crash detection = %s \n", 
        _GFX_GetCrashDetection() ? "Enable" : "Disable");

    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdMemCompare
 *  gfx.memcomp {src_addr} {dst_addr} {size}
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdMemCompare(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1SrcBase, *pu1DstBase;
    INT32 i4Ret;
    UINT32 u4Size;
    GFXCMD_CHK_NUM_MIN_PARAM(3);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return -1;
    }
    
    pu1SrcBase = _GetAddress(szArgv[1]);
    pu1DstBase = _GetAddress(szArgv[2]);
    u4Size     = (UINT32) StrToInt(szArgv[3]);

    i4Ret = GFX_MemCompare(pu1SrcBase, pu1DstBase, u4Size);

    if (i4Ret == (INT32)E_GFX_OK)
    {
        Printf("Memory Compare OK !!\n");
    }
    else
    {
        Printf("Memory Compare FAIL !!\n");
    }

    return i4Ret;
}


//-------------------------------------------------------------------------
/** _GfxCmdMemSet
 *  gfx.memset {dst_addr} {width} {height} {color}
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdMemSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1Base;
    UINT32 u4Width, u4Height, u4Value;

    if (i4Argc < 5)
    {
        Printf("arg: dst_addr, width, height, value\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) || (szArgv[4] == NULL))
    {
        return -1;
    }
    
    pu1Base  = _GetAddress(szArgv[1]);
    u4Width  = (UINT32) StrToInt(szArgv[2]);
    u4Height = (UINT32) StrToInt(szArgv[3]);
    u4Value  = (UINT32) StrToInt(szArgv[4]);

    GFX_Memset(pu1Base, u4Width, u4Height, u4Value);
    
    Printf("Memory Set OK !!\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdRotateBmp
 *  gfx.rotatebmp {dst_addr} {src_addr} {width} {height} {cm} {color_mode}
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdRotateBmp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1DstBase, *pu1SrcBase;
    UINT32 u4Width, u4Height, u4ColorMode, u4Is90Ccw;
    UINT32 u4SrcPitch, u4DstPitch;

    if (i4Argc < 9)
    {
        Printf("arg: dstaddr dpitch srcaddr spitch width height colormode is90Ccw\n");
        Printf("   is90Ccw = 0 (90 clockwise)\n");
        Printf("   is90Ccw = 1 (90 counter-clockwise)\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL) || (szArgv[5] == NULL) || 
        (szArgv[6] == NULL) || (szArgv[7] == NULL) || 
        (szArgv[8] == NULL))
    {
        return -1;
    }
    
    pu1DstBase  = _GetAddress(szArgv[1]);
    u4DstPitch  = (UINT32) StrToInt(szArgv[2]);
    pu1SrcBase  = _GetAddress(szArgv[3]);
    u4SrcPitch  = (UINT32) StrToInt(szArgv[4]);
    u4Width     = (UINT32) StrToInt(szArgv[5]);
    u4Height    = (UINT32) StrToInt(szArgv[6]);
    u4ColorMode = (UINT32)_GetEnum(szArgv[7], _arColorModeEnumTbl);
    u4Is90Ccw   = (UINT32) StrToInt(szArgv[8]);

    GFX_RotateBmp(pu1DstBase, u4DstPitch, pu1SrcBase, u4SrcPitch, 
        u4Width, u4Height, u4ColorMode, u4Is90Ccw);
    
    Printf("Rotate Bmp OK !!\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _GfxCmdSlt
 *  gfx.slt
 */
//-------------------------------------------------------------------------
static INT32 _GfxCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4SwEn = 0;   // c-model enabled
    UINT32 u4Loop = 10;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc == 2)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
    }

    if (i4Argc == 3)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
        u4SwEn = (UINT32) StrToInt(szArgv[2]);
    }

    i4Ret = _GfxSlt(u4Loop, u4SwEn);

    if ((INT32)E_CLIGFX_OK == i4Ret)
    {
        Printf("GFX SLT (%lu loops)......... SUCCESS\n", u4Loop);
        return 0;
    }
    else
    {
        Printf("GFX SLT (%lu loops)......... FAILED\n", u4Loop);
        return -1;
    }
}


//-------------------------------------------------------------------------
/** _SLT_Gfx
 *
 */
//-------------------------------------------------------------------------
static INT32 _GfxSlt(UINT32 u4Loop, UINT32 u4SwEn)
{
    INT32 i4Flag = 0, i4FailCount;
    UINT32 i, j = 0;
    UINT32 u4FbmStartAddr, u4FbmTotalSize;
    UINT32 u4Pitch32bpp, u4Pitch16bpp, u4Pitch8bpp;
    UINT32 u4CanvasWidth, u4CanvasHeight;
    UINT32 u4Width, u4Height;
    UINT32 u4WidthD2, u4HeightD2;
    UINT32 u4BytePerPix, u4Sum, u4SltAns, u4Ar, u4OpCode;
    UINT32 u4CanvasBufSize, u4CanvasPitch;
    UINT8 *pu1HwSrc, *pu1HwDst, *pu1HwChroma;
    UINT8 *pu1Ptr;

    
    #if defined(CC_MT8223)
    u4SltAns = 0x102c3ad3;
    #else
    u4SltAns = 0x11b3af06;
    #endif
    pu1HwChroma = NULL;
    pu1HwDst = NULL;
    pu1HwSrc = NULL;
    u4BytePerPix = 4;   // 32-bpp (argb8888)
    
    u4CanvasWidth = 768;
    u4CanvasHeight = 512;
    u4CanvasPitch = (u4CanvasWidth * u4BytePerPix);
    
    u4Width = 720;
    u4Height = 496;
    u4WidthD2 = (u4Width / 2);
    u4HeightD2 = (u4Height / 2);

    u4CanvasBufSize = (u4CanvasWidth * u4CanvasHeight * u4BytePerPix);
    
    u4Pitch32bpp = (u4Width * u4BytePerPix);
    u4Pitch16bpp = (u4Pitch32bpp / 2);
    u4Pitch8bpp  = (u4Pitch32bpp / 4);

    u4Ar = 239;     // global alpha
    u4OpCode = 7;

    u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    u4FbmTotalSize = DRVCUST_OptGet(eFbmMemSize);
    
    // assign 3 buffers (src, dst, cbcr)
    pu1HwSrc = (UINT8 *)(u4FbmStartAddr);
    VERIFY(pu1HwSrc != NULL);
    
    pu1HwDst = (UINT8 *)(u4FbmStartAddr + u4CanvasBufSize);
    VERIFY(pu1HwDst != NULL);
    
    pu1HwChroma = (UINT8 *)(u4FbmStartAddr + (2 * u4CanvasBufSize));
    VERIFY(pu1HwChroma != NULL);

    Printf("new SLT test\n");
    Printf("---------------------- [GFX SLT] -----------------------\n");
    Printf("FBM start addr = 0x%lx\n", u4FbmStartAddr);
    Printf("FBM total size = %lu (MB)\n", (u4FbmTotalSize / 1048576));
    Printf("SRC    start addr = 0x%lx\n", (UINT32)pu1HwSrc);
    Printf("DST    start addr = 0x%lx\n", (UINT32)pu1HwDst);
    Printf("Chroma start addr = 0x%lx\n", (UINT32)pu1HwChroma);

    // initialization
    GFX_Init();

    // Enter SW mode
    if (u4SwEn)
    {
        GFX_Reset();
        GFX_DifSetMode((UINT32)E_GFX_SW_MOD);
    }
    else
    {
        GFX_Reset();
        GFX_DifSetMode((UINT32)E_GFX_HW_MOD);
    }
    
    // clear 3 buffers
    GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwChroma, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_Flush();
    GFX_Wait();

    i4Flag = 0;
    i4FailCount = 0;
    for (j = 0; j < u4Loop; j++)
    {
        // HW Action
        // 1. rect fill (src)
        GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetColor(0xfa6359be);
        GFX_Fill(0, 0, u4Width, u4Height);
        GFX_SetColor(0xFFFFEBCD);    //blanchedalmond
        GFX_Fill(20,20,140,120);
        GFX_SetColor(0xFFFAEBD7);    //antiquewhite
        GFX_Fill(200, 20, 140, 120);
        GFX_SetColor(0xFF8A2BE2);    //blueviolet
        GFX_Fill(380, 20, 140, 120);
        GFX_SetColor(0xFF7FFFD4);    //aquamarine
        GFX_Fill(560, 20, 140, 120);
        GFX_SetColor(0xFFF0FFFF);    //azure
        GFX_Fill(20, 180, 140, 120);
        GFX_SetColor(0xFFF5F5DC);    //beige
        GFX_Fill(200, 180, 140, 120);
        GFX_SetColor(0xFFFFE4C4);    //bisque
        GFX_Fill(380, 180, 140, 120);
        GFX_SetColor(0xFFF0F8FF);    //aliceblue
        GFX_Fill(560, 180, 140, 120);
        GFX_SetColor(0xFFDEB887);    //burlywood
        GFX_Fill(20, 340, 140, 120);
        GFX_SetColor(0xFF00FFFF);    //aqua
        GFX_Fill(200, 340, 140, 120);
        GFX_SetColor(0xFFA52A2A);    //brown
        GFX_Fill(380, 340, 140, 120);
        GFX_SetColor(0xFF0000FF);    //blue
        GFX_Fill(560, 340, 140, 120);
        GFX_SetColor(0xFFFFFFFF);
        GFX_Fill(0, 0, 720, 2);
        GFX_Fill(0, 159, 720, 3);
        GFX_Fill(0, 319, 720, 3);
        GFX_Fill(0, 478, 720, 2);
        GFX_Fill(0, 0, 2, 480);
        GFX_Fill(179, 0, 3, 480);
        GFX_Fill(359, 0, 3, 480);
        GFX_Fill(539, 0, 3, 480);
        GFX_Fill(718, 0, 2, 480);
        GFX_Flush();
        GFX_Wait();

        // 2. rect fill (dst)
        GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetColor(0xabcdef90);
        GFX_Fill(0, 0, u4Width, u4Height);
        GFX_SetColor(0xFFF0F8FF);    //aliceblue
        GFX_Fill(20,20,140,120);
        GFX_SetColor(0xFFFAEBD7);    //antiquewhite
        GFX_Fill(200, 20, 140, 120);
        GFX_SetColor(0xFF00FFFF);    //aqua
        GFX_Fill(380, 20, 140, 120);
        GFX_SetColor(0xFF7FFFD4);    //aquamarine
        GFX_Fill(560, 20, 140, 120);
        GFX_SetColor(0xFFF0FFFF);    //azure
        GFX_Fill(20, 180, 140, 120);
        GFX_SetColor(0xFFF5F5DC);    //beige
        GFX_Fill(200, 180, 140, 120);
        GFX_SetColor(0xFFFFE4C4);    //bisque
        GFX_Fill(380, 180, 140, 120);
        GFX_SetColor(0xFFFFEBCD);    //blanchedalmond
        GFX_Fill(560, 180, 140, 120);
        GFX_SetColor(0xFF0000FF);    //blue
        GFX_Fill(20, 340, 140, 120);
        GFX_SetColor(0xFF8A2BE2);    //blueviolet
        GFX_Fill(200, 340, 140, 120);
        GFX_SetColor(0xFFA52A2A);    //brown
        GFX_Fill(380, 340, 140, 120);
        GFX_SetColor(0xFFDEB887);    //burlywood
        GFX_Fill(560, 340, 140, 120);
        GFX_SetColor(0xFFFFFFFF);
        GFX_Fill(0, 0, 720, 2);
        GFX_Fill(0, 159, 720, 3);
        GFX_Fill(0, 319, 720, 3);
        GFX_Fill(0, 478, 720, 2);
        GFX_Fill(0, 0, 2, 480);
        GFX_Fill(179, 0, 3, 480);
        GFX_Fill(359, 0, 3, 480);
        GFX_Fill(539, 0, 3, 480);
        GFX_Fill(718, 0, 2, 480);
        GFX_Flush();
        GFX_Wait();

        // 3. hori line to vert line (dst -> chroma)
        GFX_SetDst(pu1HwChroma, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetColor(0xffffffff);
        GFX_Fill(0, 0, u4Width, u4Height);
        GFX_RotateBmp(pu1HwChroma, u4Pitch32bpp, pu1HwDst, u4Pitch32bpp,
            u4Height, u4Height, (UINT32)CM_ARGB8888_DIRECT32, 1);

        // 4. alpha blending
        GFX_SetSrc(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetAlpha(u4Ar);
        GFX_Blend(u4WidthD2, 0, u4WidthD2, 0, u4WidthD2, u4Height);
        GFX_Flush();
        GFX_Wait();

        // 5. alpha compose loop
        GFX_SetSrc(pu1HwSrc, (UINT32)CM_ARGB4444_DIRECT16, u4Pitch16bpp);
        GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB4444_DIRECT16, u4Pitch16bpp);
        GFX_SetAlpha(u4Ar);
        GFX_ComposeLoop(0, 0, 0, 0, u4Width, u4Height, u4Ar, u4OpCode, 0);
        GFX_Flush();
        GFX_Wait();

        // 6. normal bitblt (dst -> src)
        GFX_SetSrc(pu1HwDst, (UINT32)CM_RGB_CLUT8, u4Pitch8bpp);
        GFX_SetDst(pu1HwSrc, (UINT32)CM_RGB_CLUT8, u4Pitch8bpp);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(u4WidthD2, 0, u4WidthD2, 0, u4WidthD2, u4HeightD2);
        GFX_BitBlt(0, u4HeightD2, 0, u4HeightD2, u4WidthD2, u4HeightD2);
        GFX_Flush();
        GFX_Wait();

        // 7. rop bitblt (src XNOR chroma)
        GFX_SetSrc(pu1HwSrc,    (UINT32)CM_ARGB4444_DIRECT16, u4Pitch16bpp);
        GFX_SetDst(pu1HwChroma, (UINT32)CM_ARGB4444_DIRECT16, u4Pitch16bpp);
        GFX_SetRopOpt(u4OpCode);
        GFX_RopBitBlt(0, 0, 0, 0, u4Width, u4Height);
        GFX_Flush();
        GFX_Wait();

        // 8. normal bitblt (dst -> chroma)
        GFX_SetSrc(pu1HwDst,    (UINT32)CM_RGB_CLUT8, u4Pitch8bpp);
        GFX_SetDst(pu1HwChroma, (UINT32)CM_RGB_CLUT8, u4Pitch8bpp);
        GFX_SetBltOpt(0, 0, 0);
        GFX_BitBlt(0, 0, 0, 0, u4WidthD2, u4HeightD2);
        GFX_BitBlt(u4WidthD2, u4HeightD2, u4WidthD2, u4HeightD2, 
            u4WidthD2, u4HeightD2);
        GFX_Flush();
        GFX_Wait();

        // 9. ycbcr to rgb (420-linear, bt709, video system)
        GFX_SetColCnvFmt((UINT32)E_YCFMT_420LINEAR, (UINT32)E_SWAP_BLOCK, 
            (UINT32)E_VSTD_BT709, (UINT32)E_VSYS_COMP);
        GFX_SetColCnvSrc(pu1HwSrc, u4Pitch8bpp, pu1HwChroma, u4Pitch8bpp, 0);
        GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_ColConv(0, 0, u4WidthD2, u4HeightD2);
        GFX_Flush();
        GFX_Wait();

        #if !defined(CC_MT8223)
        // 10. index to direct color bitblt
        GFX_SetIdx2DirSrc(pu1HwSrc, (UINT32)E_BMP_CM_8BIT);
        GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetIdx2DirOpt(pu1HwChroma, 0, 0);
        GFX_Idx2DirBitBlt(u4WidthD2, u4HeightD2, u4WidthD2, u4HeightD2);
        GFX_Flush();
        GFX_Wait();
        #endif

        // physical -> virtual address
        pu1Ptr = (UINT8 *)VIRTUAL((UINT32)pu1HwDst);
        
        // check sum
        u4Sum = 0;
        for (i = 0; i < u4CanvasBufSize; i++)
        {
            u4Sum += ((UINT32)(*(pu1Ptr + i)));
        }

        Printf("[Run %ld]\n", j);
        Printf("u4Sum    = 0x%lx\n", u4Sum);
        Printf("u4SltAns = 0x%lx\n", u4SltAns);

        if (u4Sum != u4SltAns)
        {
            i4Flag = 1;
            i4FailCount++;
//            break;
        }
        //Printf("0x%x\n", IO_READ32(0xf0004000, 0xe4));
    } // ~for (j = 0; j < i4Loop; j++)


    if (i4Flag == 1)
    {
        Printf("[GFX] Fail Count = %ld\n", i4FailCount);
        return -1;
    }
    else
    {
        return (INT32)E_CLIGFX_OK;
    }
}


//-------------------------------------------------------------------------
/** _GetAddress
 *  
 */
//-------------------------------------------------------------------------
static UINT8* _GetAddress(const CHAR *szAddress)
{
    if (NULL == szAddress)
    {
        return NULL;
    }

    return (UINT8*)StrToInt(szAddress);
}


//-------------------------------------------------------------------------
/** _GetEnum
 *  
 */
//-------------------------------------------------------------------------
static INT32 _GetEnum(const CHAR *szText, const GFXCMDTEXT2ENUM_T* prText2Enum)
{
    if ((NULL == szText) ||
        (NULL == prText2Enum))
    {
        return -(INT32)E_CLIGFX_ERR_PARAM;
    }

    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText, x_strlen(prText2Enum->szText)) == 0)
        {
            break;
        }
        else
        {
            prText2Enum++;
        }
    }
    return prText2Enum->i4Value;
}


//-------------------------------------------------------------------------
/** _GfxOpCmdRectTestMainLoop
 *  
 */
//-------------------------------------------------------------------------
static void _GfxOpCmdRectTestMainLoop(void *pvArg)
{
    UINT32 u4Delay;

    if (pvArg == NULL)
    {
        return;
    }
    
    u4Delay = *((UINT32 *)pvArg);
    Printf("\nRect Test Thread Delay = %d (ms)\n", u4Delay);

    while (_i4DoRectTest)
    {
        // do rect fill
        GFX_SetDst(_pu1RectTestAlignedDst, (UINT32)CM_ARGB8888_DIRECT32, 
            _u4RectTestDstPitch);
        GFX_SetColor(0xFFFFFFFF);
        GFX_Fill(0, 0, _u4RectTestWidth, _u4RectTestHeight);
        GFX_Flush();
        GFX_Wait();

        x_thread_delay(u4Delay);
    }

    // exit thread
    if (_pu1RectTestDst != NULL)
    {
        x_mem_free(_pu1RectTestDst);
        _pu1RectTestDst = NULL;
    }

    Printf("_GfxOpCmdRectTestMainLoop() Exit!!\n");
    x_thread_exit();
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

CLI_MAIN_COMMAND_ITEM(Gfx)
{
    "gfx",
    NULL,
    NULL,
    _arGfxCmdTbl,
    "Gfx command",
    CLI_SUPERVISOR
};
/*lint -restore */


