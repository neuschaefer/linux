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
 * $RCSfile: dmx_emu_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_emu_cmd.c
 *  Demux Emulation CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm_if.h"
#include "dmx_emu_if.h"
#include "dmx_drm_drvif.h"
#include "dmx_drm_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN


#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_rand.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_bim.h"

//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG    CLI_IsLog
#define DEFINE_IS_LOG    DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//LINT_SAVE_AND_DISABLE
//CLIMOD_DEBUG_FUNCTIONS(DMX)
//LINT_RESTORE

LINT_SUPPRESS_SYMBOL(818, aszArgv)


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// TS index
UINT8 _u1TsIndex = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//=====================================================================
// CLI command table
//=====================================================================

static INT32 _CmdDdiTransfer0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s type[0-1] [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u1Type = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 0 test\n");
    Printf("Type: %u, TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        u1Type, _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer0(u1Type, _u1TsIndex, u4Size))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDdiTransfer1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 1 test\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    if (!DMX_EMU_DDI_Init(_u1TsIndex))
    {
        Printf("Fail to initialize DDI!\n");
        return -1;
    }

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer1(0, u4Size))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if (!DMX_EMU_DDI_Exit())
    {
        Printf("Fail to release DDI!\n");
        return -1;
    }

    return (fgRet ? 0 : -1);
}

static INT32 _CmdDdiTransfer2(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s type[0-2] [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u1Type = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 1 test\n");
    Printf("Type: %u, TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        u1Type, _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        //u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
        //    188);

        //u4Size = random(u4MaxSize - u4MinSize) + u4MinSize;

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer2(u1Type, _u1TsIndex, u4Size))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart Pull TS test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer0(_u1TsIndex, u4Size, u2Pid))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop(0=infinite) [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart Pull TS test 1\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer1(_u1TsIndex, u4Size, u2Pid))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest2(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop(0=infinite) [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart pull TS test 2\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    if (!DMX_EMU_InitPullTsBuffers(_u1TsIndex))
    {
        Printf("Fail to initialize buffers!\n");
        return -1;
    }

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer2(u4Size))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if (!DMX_EMU_ReleasePullTsBuffers())
    {
        Printf("Fail to release buffers!\n");
        return -1;
    }

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullEsTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart Pull ES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullEsTransfer0(_u1TsIndex, u4Size, u2Pid))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullEsTest1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart Pull ES test 1\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullEsTransfer1(_u1TsIndex, u4Size, u2Pid))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if(!DMX_Init())
    {
        return -1;
    }
    if(!DMX_DDI_Init())
    {
        return -1;
    }

    return (fgRet ? 0 : -1);
}


static INT32 _CmdMulti2CBC(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop = 1;
    UINT8 u1Type = 0, u1SetType;

    if (i4Argc < 2)
    {
        Printf("%s loop type\n", aszArgv[0]);
        Printf("   type, 0:play, 1:rec, 2:rec_enc, 3:mix\n");
        return -1;
    }

    if(i4Argc > 1)
    {
        u4Loop = StrToInt(aszArgv[1]);
    }

    if(i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    for(i=0; i<u4Loop; i++)
    {
        Printf("Loop: %d\n", i);
        if(u1Type == 3)
        {
            u1SetType = (UINT8)i % 3;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch(u1SetType)
        {
        case 0:
            Printf("   Play test\n");
            break;
        case 1:
            Printf("   Rec Level1 test\n");
            break;
        case 2:
            Printf("   Rec Level2 test\n");
            break;
        }
        if(!DMX_EMU_Multi2Test(_u1TsIndex, u1SetType))
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}


static INT32 _CmdDesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT8 u1Type = 0;
    UINT8 u1SetType = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("    type, 0:play, 1:rec_l2, 2:rec_l2, 3:combine\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if(i4Argc > 2)
    {
        u1Type = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if(u1Type == 3)
        {
            u1SetType = u4Loop % 3;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch(u1SetType)
        {
        case 0:
            Printf("   play\n");
            break;
        case 1:
            Printf("   Rec Level1\n");
            break;
        case 2:
            Printf("   REc Level2\n");
            break;
        }

        if (!DMX_EMU_Des0(_u1TsIndex, u4Size, u2Pid, u1SetType))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdAesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type = 0;
    UINT8 u1SetType = 0;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("   type, 0:play, 1:rec_L1, 2:rec_L2, 3:combine\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if(i4Argc > 2)
    {
        u1Type = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart AES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if(u1Type == 3)
        {
            u1SetType = (UINT8)u4Loop % 3;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch(u1SetType)
        {
        case 0:
            Printf("   Play test\n");
            break;
        case 1:
            Printf("   Rec Level1 test\n");
            break;
        case 2:
            Printf("   Rec Level2 test\n");
            break;
        }

        if (!DMX_EMU_Aes0(_u1TsIndex, u4Size, u2Pid, u1SetType))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdSMS4Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT8 u1Type;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("    type, 0:play, 1:rec, 2:mix\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart SMS4 test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;

    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (!DMX_EMU_SMS4(_u1TsIndex, u4Size, u2Pid, u1Type))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


#ifdef TEST_MODE
static INT32 _CmdVuduAesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size = 188 * 16 * 16;
    UINT16 u2Pid = 0;
    BOOL fgRet, fgSCTE = TRUE;

    Printf("\nStart AES test 0\n");

    fgRet = DMX_EMU_VuduAes0(_u1TsIndex, u4Size, u2Pid, fgSCTE);

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}
#endif

#if 0  // Not use in MT5368
static void _ShowDrmTest0Usage(const CHAR* szCmd, UINT32 u4Max, UINT32 u4Min)
{
    Printf("Usage: %s mode[bypass|aes|aesscte52|nd|random] [loop(1, 0=infinite)]"
        " [max_size=0x%x] [min_size=0x%x]\n",
        szCmd, u4Max, u4Min);
}

static INT32 _CmdDrmTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x1000, u4MinSize = 0x100;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    DMX_DRM_MODE_T eMode;

    if (i4Argc == 1)
    {
        _ShowDrmTest0Usage(aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "bypass") == 0)
    {
        eMode = DMX_DRM_MODE_BYPASS;
    }
    else if (x_strcmp(aszArgv[1], "aes") == 0)
    {
        eMode = DMX_DRM_MODE_AES;
    }
    else if (x_strcmp(aszArgv[1], "aese") == 0)
    {
        eMode = DMX_DRM_MODE_AES_ENCRYPTION;
    }
    else if (x_strcmp(aszArgv[1], "aesscte52") == 0)
    {
        eMode = DMX_DRM_MODE_AES_SCTE52;
    }
    else if (x_strcmp(aszArgv[1], "nd") == 0)
    {
        eMode = DMX_DRM_MODE_ND;
    }
    else if (x_strcmp(aszArgv[1], "random") == 0)
    {
        eMode = DMX_DRM_MODE_UNKNOWN;
    }
    else
    {
        _ShowDrmTest0Usage(aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DRM test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 1,
            1);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (!DMX_EMU_Drm0(_u1TsIndex, u4Size, u2Pid, eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDrmPerfTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;
    BOOL fgSw = FALSE;
    UINT32 u4Size = 1 * 1024 * 1024;
    DMX_DRM_MODE_T eMode = DMX_DRM_MODE_AES;

    if (i4Argc > 4 || i4Argc == 1)
    {
        Printf("Usage: %s [aes|aese|nd|pd] [size=0x%x] [SW(0|1)]\n", aszArgv[0], u4Size);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "aes") == 0)
    {
        eMode = DMX_DRM_MODE_AES;
    }
    else if (x_strcmp(aszArgv[1], "aese") == 0)
    {
        eMode = DMX_DRM_MODE_AES_ENCRYPTION;
    }
    else if (x_strcmp(aszArgv[1], "nd") == 0)
    {
        eMode = DMX_DRM_MODE_ND;
    }
    else if (x_strcmp(aszArgv[1], "pd") == 0)
    {
        eMode = DMX_DRM_MODE_PD;
    }
    else
    {
        Printf("Usage: %s [aes|aese|nd|pd] [size=0x%x] [SW(0|1)]\n", aszArgv[0], u4Size);
        return -1;
    }

    if (i4Argc == 3)
    {
        u4Size = StrToInt(aszArgv[2]);
    }

    if (i4Argc == 4)
    {
        fgSw = StrToInt(aszArgv[3]);
    }

    {
        UINT16 u2Pid;

        u2Pid = random(0x2000);

        fgRet = DMX_EMU_DRM_Perf(_u1TsIndex, u4Size, u2Pid, eMode, fgSw);
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}
#endif

#ifdef TEST_MODE
static INT32 _CmdDrmNdSelfTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;

    Printf("DRM-ND self test ...\n");

    fgRet = DMX_EMU_DrmNdSelfTest();
    if (!fgRet)
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("Pass!\n");

    return 0;
}
#endif  // DMX_SUPPORT_NDDRM

static INT32 _CmdPatternMatch(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x2000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    HAL_RAW_TIME_T rawTime;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart Pattern Match test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    HAL_GetRawTime(&rawTime);
    srand(rawTime.u4Cycles);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188, 188);
        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        // FIXME!!!
#if 0
        if (!DMX_EMU_PM_Transfer(_u1TsIndex, u4Size))
#endif
        if (!DMX_EMU_PM_Transfer(_u1TsIndex))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


EXTERN BOOL _DMX_EMU_PSI_Test(UINT8 u1TsIdx, BOOL fgGenPat, DMX_FILTER_MODE_T eMode);
static INT32 _CmdPSITest(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;

    if(i4Argc != 3)
    {
        Printf("%s [loop] [mode]\n", aszArgv[0]);
        Printf("    0: short mode\n");
        Printf("    1: long mode\n");
        Printf("    2: pos_neg mode\n");
        Printf("    3: generic mode\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);

    if(u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if(u4Mode == 1)
    {
        Printf("Not support this kind filter\n");
        return -1;
    }
    else if(u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if(u4Mode == 3)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    for(i=0; i<u4Loop; i++)
    {
        if(!_DMX_EMU_PSI_Test(_u1TsIndex, TRUE, eMode))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i);
    }

    return 0;
}


EXTERN BOOL _DMX_EMU_MulMatch_Test(UINT8 u1TsIdx);
static INT32 _CmdMultipleMatchTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;

    if(i4Argc != 2)
    {
        Printf("%s loop\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    for(i=0; i<u4Loop; i++)
    {
        if(!_DMX_EMU_MulMatch_Test(_u1TsIndex))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i);
    }

    return 0;
}


EXTERN BOOL _DMX_EMU_PSICombo_Test(UINT8 u1TsIdx, BOOL fgGenPat, DMX_FILTER_MODE_T eMode, UINT32 u4Hw);
static INT32 _CmdPSIComboTest(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode, u4Hw;

    if(i4Argc != 4)
    {
        Printf("%s [loop] [mode] [hw]\n", aszArgv[0]);
        Printf("   mode:  \n");
        Printf("    0: short mode\n");
        Printf("    1: long mode\n");
        Printf("    2: pos_neg mode\n");
        Printf("    3: generic mode\n");
        Printf("   hw:  \n");
        Printf("       0: play only\n");
        Printf("       1: record only\n");
        Printf("       2: play and record\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);
    u4Hw = StrToInt(aszArgv[3]);

    if(u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if(u4Mode == 1)
    {
        Printf("Not support this kind filter\n");
        return -1;
    }
    else if(u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if(u4Mode == 3)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    for(i=0; i<u4Loop; i++)
    {
        if(!_DMX_EMU_PSICombo_Test(_u1TsIndex, TRUE, eMode, u4Hw))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i);
    }

    return 0;
}

EXTERN BOOL _DMX_EMU_FVR_PES_Test(UINT8 u1TsIndex);
static INT32 _CmdFVRPesTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;

    fgRet = _DMX_EMU_FVR_PES_Test(_u1TsIndex);
    if(fgRet)
    {
        Printf("OK\n");
    }
    else
    {
        Printf("Fail\n");
    }

    return 0;
}


#if 0
static INT32 _CmdStartTSOutput(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet = FALSE;
    UINT32 u4MaxLoop, u4Loop = 0;
    UINT8 u1TsIndex;
    UINT32 u4Size = 0;

    if (i4Argc != 3 && i4Argc != 4)
    {
        Printf("Usage: %s ts_index data_size [loop(0=infinite)]\n", aszArgv[0]);
        return -1;
    }

    u1TsIndex = (UINT8) StrToInt(aszArgv[1]);
    u4Size = StrToInt(aszArgv[2]);
    u4MaxLoop = StrToInt(aszArgv[3]);

    u4Size -= (u4Size % 188);

    while (1)
    {
        Printf("Round: %u\n", u4Loop);

        if (!DMX_EMU_TSOut_Init(u1TsIndex, u4Size))
        {
            return -1;
        }

        if (!DMX_EMU_TSOut_StressTest())
        {
            break;
        }

        if ((++u4Loop >= u4MaxLoop) && (u4MaxLoop != 0))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS!" : "FAIL!");

    return (fgRet ? 0 : -1);
}
#endif

EXTERN BOOL _DMX_EMU_FVR_TimestampOnOffTest(UINT8 u1TsIndex, BOOL fgTimeStampOn);
static INT32 _CmdFVRTimestampTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgTimestampOn = FALSE;
    UINT32 i, u4Loop;

    if(i4Argc != 3)
    {
        Printf("%s loop on|off\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    if(x_strcmp(aszArgv[2], "on") == 0)
    {
        fgTimestampOn = TRUE;
    }
    else if(x_strcmp(aszArgv[2], "off") == 0)
    {
        fgTimestampOn = FALSE;
    }
    else
    {
        Printf("%s loop on|off\n", aszArgv[0]);
        return -1;
    }

    for(i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d -----\n", i+1);
        if(_DMX_EMU_FVR_TimestampOnOffTest(_u1TsIndex, fgTimestampOn))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

EXTERN BOOL _DMX_EMU_FVR_TimestampPauseTest(UINT8 u1TsIndex);
static INT32 _CmdFVRTimestampPauseTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;

    if(i4Argc != 2)
    {
        Printf("%s loop\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    for(i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d -----\n", i+1);
        if(_DMX_EMU_FVR_TimestampPauseTest(_u1TsIndex))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

BOOL _DMX_EMU_FVR_TimestampErrTest(UINT8 u1TsIndex);
static INT32 _CmdFVRTimestampErrTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;

    if(i4Argc != 2)
    {
        Printf("%s loop\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    for(i=0; i<u4Loop; i++)
    {
        Printf(" -----Test %d -----\n", i+1);
        if(_DMX_EMU_FVR_TimestampErrTest(_u1TsIndex))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

EXTERN BOOL _DMX_EMU_FVR_TimestampResetTest(UINT8 u1TsIndex);
static INT32 _CmdFVRTimestampResetTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;

    if(i4Argc != 2)
    {
        Printf("%s loop\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    for(i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d -----\n", i+1);
        if(_DMX_EMU_FVR_TimestampResetTest(_u1TsIndex))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
        }
    }

    return 0;
}


extern BOOL DMX_EMU_DDI_Bypass_Test(UINT8 u1TsIndex, BOOL fgRec);
static INT32 _CmdBypassTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop, u4Rec;
    BOOL fgRec;

    if(i4Argc != 3)
    {
        Printf("%s loop rec\n", aszArgv[0]);
        Printf("   play:0, rec:1, mix:2\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Rec = StrToInt(aszArgv[2]);
    for(i=0; i<u4Loop; i++)
    {
        if(u4Rec == 0)
        {
            fgRec = FALSE;
        }
        else if(u4Rec == 1)
        {
            fgRec = TRUE;
        }
        else
        {
            fgRec = (random(2) == 1) ? TRUE : FALSE;
        }

        Printf("----- Test %d -----\n", i+1);
        if(fgRec)
        {
            Printf("Rec bypass:\n");
        }
        else
        {
            Printf("Play bypass:\n");
        }
        if(DMX_EMU_DDI_Bypass_Test(_u1TsIndex, fgRec))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}


extern BOOL DMX_EMU_PreByte_Test(UINT8 u1TsIndex, BOOL fgExtSync, UINT32 u4Size, UINT16 u2PktSize);
static INT32 _CmdPreByteTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop, u4Size, u4Sync;
    UINT16 u2PktSize;
    BOOL fgRandom = FALSE;

    if((i4Argc != 2) && (i4Argc != 5))
    {
        Printf("%s loop sync size pkt_size\n", aszArgv[0]);
        Printf("    sync, 0: internal, 1: external\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    if(i4Argc >= 5)
    {
        u4Sync = StrToInt(aszArgv[2]);
        u4Size = StrToInt(aszArgv[3]);
        u2PktSize = StrToInt(aszArgv[4]);
    }
    else
    {
        fgRandom = TRUE;
    }

    for(i=0; i<u4Loop; i++)
    {
        if(fgRandom)
        {
            u4Sync = random(2);
            u4Size = random(0xF0000) + 0x10000;
            u2PktSize = 188 + random(10) * 4;

            Printf("Sync: %s, size: 0x%x, pkt:%d\n", (u4Sync == 1) ? "Ext" : "Int", u4Size, u2PktSize);
        }

        Printf("----- Test %d -----\n", i+1);
        if(DMX_EMU_PreByte_Test(_u1TsIndex, (u4Sync == 1), u4Size, u2PktSize))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_TsOutput
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TsOutput(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_TSOUT_ROUTE_T eRoute;
    DMX_TSOUT_ENDIAN_T eEndian;
    DMX_TSOUT_SYNC_CYCLE_T eSyncCycle;
    DMX_TSOUT_LATCH_EDGE_T eLatchEdge;
    DMX_TSOUT_DATAWIDTH_T eDataWidth;
    DMX_TSOUT_DATAWIDTH_T eTsInDataWidth;
    UINT32 u4Size, u4ValidSpace;
    UINT8 u1Tsindex = 2;

    if ((i4Argc < 3) || (i4Argc > 10))
    {
        Printf("%s size tsindex [route] TSout:[1|2|8bit] TSIn:[1|2|8bit] [msb|lsb] [sync] [valid] [pos|neg]\n", aszArgv[0]);
        Printf("    route: loopback | ext(ex CI) | int(int CI) -> ckgen\n");
        Printf("     sync: 8 | 1\n");
        Printf("    valid: 0-3\n");
        Printf("  default: loopback 1 1 msb 8 0 neg 2\n");
        Printf("    fpga use ts index 1\n");
        return -1;
    }

    // Get size
    if (i4Argc >= 2)
    {
        u4Size = (UINT32)StrToInt(aszArgv[1]);
    }

    if (i4Argc >= 3)
    {
        u1Tsindex = (UINT8)StrToInt(aszArgv[2]);
    }

    // Set route.
    if (i4Argc >= 4)
    {
        if (x_strncmp(aszArgv[3], "loopback", sizeof("loopback"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_DBM_LOOPBACK;
        }
        else if (x_strncmp(aszArgv[3], "ext", sizeof("ext"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_EXTERNAL_CI;
        }
        else if (x_strncmp(aszArgv[3], "int", sizeof("int"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_INTERNAL_CI;
        }
        else
        {
            Printf("Unknown route!\n");
            return -1;
        }
    }
    else
    {
        eRoute = DMX_TSOUT_ROUTE_DBM_LOOPBACK;
    }

    if (i4Argc >= 5)
    {
        if (x_strncmp(aszArgv[4], "1", sizeof("1"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_1;
        }
        else if (x_strncmp(aszArgv[4], "2", sizeof("2"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_2;
        }
        else if (x_strncmp(aszArgv[4], "8", sizeof("8"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_8;
        }
        else
        {
            Printf("Unknown data width!\n");
            return -1;
        }
    }
    else
    {
        eDataWidth = DMX_TSOUT_DATAWIDTH_1;
    }

    if (i4Argc >= 6)
    {
        if (x_strncmp(aszArgv[5], "1", sizeof("1"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_1;
        }
        else if (x_strncmp(aszArgv[5], "2", sizeof("2"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_2;
        }
        else if (x_strncmp(aszArgv[5], "8", sizeof("8"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_8;
        }
        else
        {
            Printf("Unknown data width!\n");
            return -1;
        }
    }
    else
    {
        eTsInDataWidth = DMX_TSOUT_DATAWIDTH_1;
    }

    // Set endian mode.
    if (i4Argc >= 7)
    {
        if (x_strncmp(aszArgv[6], "msb", sizeof("msb"))==0)
        {
            eEndian = DMX_TSOUT_ENDIAN_MSB;
        }
        else if (x_strncmp(aszArgv[6], "lsb", sizeof("lsb"))==0)
        {
            eEndian = DMX_TSOUT_ENDIAN_LSB;
        }
        else
        {
            Printf("Unknown endian mode!\n");
            return -1;
        }
    }
    else
    {
        eEndian = DMX_TSOUT_ENDIAN_MSB;
    }

    // Set sync cycle.
    if (i4Argc >= 8)
    {
        if (x_strncmp(aszArgv[7], "8", sizeof("8"))==0)
        {
            eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
        }
        else if (x_strncmp(aszArgv[7], "1", sizeof("1"))==0)
        {
            eSyncCycle = DMX_TSOUT_SYNC_CYCLE_1;
        }
        else
        {
            Printf("Unknown Sync cycle!\n");
            return -1;
        }
    }
    else
    {
        eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
    }

    // Set valid space.
    if (i4Argc >= 9)
    {
        u4ValidSpace = (UINT32)StrToInt(aszArgv[8]);
        if (u4ValidSpace >= 4)
        {
            Printf("Spacing %u is out of range!\n", u4ValidSpace);
            return -1;
        }
    }
    else
    {
        u4ValidSpace = 0;
    }

    // Set latch edge.
    if (i4Argc >= 10)
    {
        if (x_strncmp(aszArgv[9], "pos", sizeof("pos"))==0)
        {
            eLatchEdge = DMX_TSOUT_LATCH_EDGE_POSITIVE;
        }
        else if (x_strncmp(aszArgv[9], "neg", sizeof("neg"))==0)
        {
            eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;
        }
        else
        {
            Printf("Unknown latch edge!\n");
            return -1;
        }
    }
    else
    {
        eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;
    }

    if (!DMX_TsOut_SetRoute(u1Tsindex, eRoute))
    {
        Printf("Failed to set route!\n");
        return -1;
    }
    if (!DMX_TsOut_SetDataWidth(eDataWidth))
    {
        Printf("Failed to set data width!\n");
        return -1;
    }
    if (!DMX_TsIn_SetDataWidth(u1Tsindex, eTsInDataWidth))
    {
        Printf("Failed to set data width!\n");
        return -1;
    }
    if (!DMX_TsOut_SetEndianess(eEndian))
    {
        Printf("Failed to set endian mode!\n");
        return -1;
    }
    if (!DMX_TsOut_SetSyncCycle(eSyncCycle))
    {
        Printf("Failed to set sync cycle!\n");
        return -1;
    }
    if (!DMX_TsOut_SetValidCycle(u4ValidSpace))
    {
        Printf("Failed to set valid space!\n");
        return -1;
    }
    if (!DMX_TsOut_SetLatchEdge(eLatchEdge))
    {
        Printf("Failed to set Latch edge!\n");
        return -1;
    }

    if (!DMX_TsOut_TransmitData(u1Tsindex, u4Size))
    {
        Printf("Failed\n");
    }
    else
    {
        Printf("OK\n");
    }

    return 0;
}


extern BOOL DMX_TsOut_BackStop_TransmitData(UINT8 u1TsIndex, UINT32 u4Size);
//-----------------------------------------------------------------------------
/** _DMXCLI_TsOutBackStop
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TsOutBackStop(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 i, u4Size, u4Loop;
    UINT8 u1Tsindex = 1;
    DMX_TSOUT_DATAWIDTH_T eDataWidth;

    if (i4Argc != 5)
    {
        Printf("%s loop size ts_index data_width\n", aszArgv[0]);
        Printf("    data_width: 1bit: 0, 2bit: 1, 8bit: 2\n");
        Printf("    fpga use ts index 1\n");
        return -1;
    }

    u4Loop = (UINT32)StrToInt(aszArgv[1]);
    u4Size = (UINT32)StrToInt(aszArgv[2]);
    u1Tsindex = (UINT8)StrToInt(aszArgv[3]);
    eDataWidth = (DMX_TSOUT_DATAWIDTH_T)StrToInt(aszArgv[4]);

    for(i=0; i<u4Loop; i++)
    {
        Printf("------ Test time: %d ------\n", i);
        if (!DMX_TsOut_SetRoute(u1Tsindex, DMX_TSOUT_ROUTE_DBM_LOOPBACK))
        {
            Printf("Failed to set route!\n");
            return -1;
        }
        if (!DMX_TsOut_SetDataWidth(eDataWidth))
        {
            Printf("Failed to set data width!\n");
            return -1;
        }
        if (!DMX_TsIn_SetDataWidth(u1Tsindex, eDataWidth))
        {
            Printf("Failed to set data width!\n");
            return -1;
        }
        if (!DMX_TsOut_SetEndianess(DMX_TSOUT_ENDIAN_MSB))
        {
            Printf("Failed to set endian mode!\n");
            return -1;
        }
        if (!DMX_TsOut_SetSyncCycle(DMX_TSOUT_SYNC_CYCLE_8))
        {
            Printf("Failed to set sync cycle!\n");
            return -1;
        }
        if (!DMX_TsOut_SetValidCycle(0))
        {
            Printf("Failed to set valid space!\n");
            return -1;
        }
        if (!DMX_TsOut_SetLatchEdge(DMX_TSOUT_LATCH_EDGE_NEGATIVE))
        {
            Printf("Failed to set Latch edge!\n");
            return -1;
        }

        if(!DMX_TsOut_BackStop_TransmitData(u1Tsindex, u4Size))
        {
            Printf("Failed\n");
            return -1;
        }
        else
        {
            Printf("OK\n");
        }
    }

    return 0;
}


extern BOOL DMX_TsOut_Framer_TransmitData(UINT8 u1TsIndex, UINT32 u4Size, UINT8 u1PktSize, BOOL fgErrTest);
//-----------------------------------------------------------------------------
/** _DMXCLI_TsOutFramer
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TsOutFramer(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 i, u4Size, u4Loop;
    UINT8 u1Tsindex = 2, u1PktSize;
    DMX_TSOUT_DATAWIDTH_T eDataWidth;
    UINT32 u4ErrTest;

    if (i4Argc < 4)
    {
        //Printf("%s loop ts_idx size data_width sync pktsize\n", aszArgv[0]);
        Printf("%s loop width pktsize [fgerr,0|1]\n", aszArgv[0]);
        Printf("    width, 0:1bit, 1:2bit, 2:8bit\n");
        return -1;
    }

    u4Loop = (UINT32)StrToInt(aszArgv[1]);
    eDataWidth = (DMX_TSOUT_DATAWIDTH_T)StrToInt(aszArgv[2]);
    u1PktSize = (UINT8)StrToInt(aszArgv[3]);
    if(i4Argc > 4)
    {
        u4ErrTest = (UINT8)StrToInt(aszArgv[4]);
    }

    u4Size = random(0x80000) + 0x20000;

    for(i=0; i<u4Loop; i++)
    {
        Printf("------ Test time: %d ------\n", i);
        if (!DMX_TsOut_SetRoute(u1Tsindex, DMX_TSOUT_ROUTE_DBM_LOOPBACK))
        {
            Printf("Failed to set route!\n");
            return -1;
        }
        if (!DMX_TsOut_SetDataWidth(eDataWidth))
        {
            Printf("Failed to set data width!\n");
            return -1;
        }
        if (!DMX_TsIn_SetDataWidth(u1Tsindex, eDataWidth))
        {
            Printf("Failed to set data width!\n");
            return -1;
        }
        if (!DMX_TsOut_SetEndianess(DMX_TSOUT_ENDIAN_MSB))
        {
            Printf("Failed to set endian mode!\n");
            return -1;
        }
        if (!DMX_TsOut_SetSyncCycle(DMX_TSOUT_SYNC_CYCLE_8))
        {
            Printf("Failed to set sync cycle!\n");
            return -1;
        }
        if (!DMX_TsOut_SetValidCycle(0))
        {
            Printf("Failed to set valid space!\n");
            return -1;
        }
        if (!DMX_TsOut_SetLatchEdge(DMX_TSOUT_LATCH_EDGE_NEGATIVE))
        {
            Printf("Failed to set Latch edge!\n");
            return -1;
        }

        if(!DMX_TsOut_Framer_TransmitData(u1Tsindex, u4Size, u1PktSize, (u4ErrTest == 1)))
        {
            Printf("Failed\n");
            return -1;
        }
        else
        {
            Printf("OK\n");
        }
    }

    return 0;
}


static INT32 _CmdDvbTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop = 1;
    UINT8 u1Type = 0;

    if (i4Argc < 2)
    {
        Printf("%s loop type\n", aszArgv[0]);
        Printf("   type, 0:play, 1:rec, 2:rec_enc\n");
        return -1;
    }

    if(i4Argc > 1)
    {
        u4Loop = StrToInt(aszArgv[1]);
    }

    if(i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    for(i=0; i<u4Loop; i++)
    {
        Printf("Loop: %d\n", i);
        if(!DMX_EMU_DvbMain(_u1TsIndex, u1Type))
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}


EXTERN BOOL _DMX_EMU_PSILoopback_Test(UINT8 u1TsIdex, BOOL fgGenPat, DMX_FILTER_MODE_T eMode);
static INT32 _CmdPSILoopbackTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;

    if(i4Argc != 3)
    {
        Printf("%s [loop] [mode]\n", aszArgv[0]);
        Printf("   mode:  \n");
        Printf("    0: short mode\n");
        Printf("    1: long mode\n");
        Printf("    2: pos_neg mode\n");
        Printf("    3: generic mode\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);

    if(u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if(u4Mode == 1)
    {
        Printf("Not support this kind filter\n");
        return -1;
    }
    else if(u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if(u4Mode == 3)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    for(i=0; i<u4Loop; i++)
    {
        if(!_DMX_EMU_PSILoopback_Test(_u1TsIndex, TRUE, eMode))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i + 1);
    }

    return 0;
}


EXTERN BOOL _DMX_EMU_PSICALoopback_Test(UINT8 u1TsIdex, BOOL fgGenPat, DMX_FILTER_MODE_T eMode);
static INT32 _CmdPSICALoopbackTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;

    if(i4Argc != 3)
    {
        Printf("%s [loop] [mode]\n", aszArgv[0]);
        Printf("   mode:  \n");
        Printf("    0: short mode\n");
        Printf("    2: pos_neg mode\n");
        Printf("    3: generic mode\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);

    if(u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if(u4Mode == 1)
    {
        Printf("Not support this kind filter\n");
        return -1;
    }
    else if(u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if(u4Mode == 3)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    for(i=0; i<u4Loop; i++)
    {
        if(!_DMX_EMU_PSICALoopback_Test(_u1TsIndex, TRUE, eMode))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i + 1);
    }

    return 0;
}


// Test RP and interrupt FIFO already empty
extern BOOL DMX_EMU_DDI_ResetTransfer(UINT8 u1TsIndex, UINT32 u4Size, BOOL fgIntr);
extern BOOL DMX_EMU_PVR_ResetTransfer(UINT8 u1TsIndex, UINT32 u4Size, BOOL fgIntr);
static INT32 _CmdDMAFIFOTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type;
    BOOL fgRet, fgIntr = FALSE;
    BOOL fgDDI;

    if (i4Argc == 1)
    {
        Printf("Usage: %s type [loop] [intr] [max_size=0x%x]"
            " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("    type, 0:ddi, 1:pvr, 2:auto\n");
        Printf("    intr, 0:polling, 1:intr\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u1Type = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        fgIntr = ((UINT32)StrToInt(aszArgv[3]) == 1);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
            u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 0 test\n");
    Printf("Type: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
        u1Type, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
            188);

        if(u1Type == 0)
        {
            fgDDI = TRUE;
        }
        else if(u1Type == 1)
        {
            fgDDI = FALSE;
        }
        else if(u1Type == 2)
        {
            UINT32 u4Sel = u4Loop % 4;
            switch(u4Sel)
            {
            case 0:
                fgDDI = TRUE;
                fgIntr = FALSE;
                break;
            case 1:
                fgDDI = TRUE;
                fgIntr = TRUE;
                break;
            case 2:
                fgDDI = FALSE;
                fgIntr = FALSE;
                break;
            case 3:
                fgDDI = FALSE;
                fgIntr = TRUE;
                break;
            }
        }
        else
        {
            return -1;
        }

        Printf("Round %u, %s, %s - size: %u\n", u4Loop, fgDDI ? "DDI" : "PVR",
                                        fgIntr ? "Intr" : "Polling", u4Size);

        if(fgDDI)
        {
            if (!DMX_EMU_DDI_ResetTransfer(0, u4Size, fgIntr))  // FPGA 0 is DDI
            {
                break;
            }
        }
        else
        {
            if(!DMX_EMU_PVR_ResetTransfer(1, u4Size, fgIntr))   // FPGA 1 is TSOut
            {
                break;
            }
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static CLI_EXEC_T _arDmxEmuCmdTbl[] =
{
    {
        "dditransfer0", "ddit0", _CmdDdiTransfer0, NULL, "", CLI_SUPERVISOR
    },

    {
        "dditransfer1", "ddit1", _CmdDdiTransfer1, NULL, "", CLI_SUPERVISOR
    },

    {
        "dditransfer2", "ddit2", _CmdDdiTransfer2, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts0", "pt0", _CmdPullTsTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts1", "pt1", _CmdPullTsTest1, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts2", "pt2", _CmdPullTsTest2, NULL, "", CLI_SUPERVISOR
    },

    {
        "pulles0", "pe0", _CmdPullEsTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "pulles1", "pe1", _CmdPullEsTest1, NULL, "", CLI_SUPERVISOR
    },

    {
        "multi2cbc", "m2cbc", _CmdMulti2CBC, NULL, "", CLI_SUPERVISOR
    },

	{
        "des0", "d0", _CmdDesTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "aes0", "a0", _CmdAesTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "sms4", "sms4", _CmdSMS4Test, NULL, "", CLI_SUPERVISOR
    },
#ifdef TEST_MODE
    {
        "vuduaes0", "va0", _CmdVuduAesTest0, NULL, "", CLI_SUPERVISOR
    },
#endif
/*
    {
        "drm0", "dr0", _CmdDrmTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "drmperf", "drmp", _CmdDrmPerfTest, NULL, "", CLI_SUPERVISOR
    },
*/
#ifdef TEST_MODE
#ifdef DMX_SUPPORT_NDDRM
    {
        "drmndselftest", "dnst", _CmdDrmNdSelfTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "drmndmultipasstest", "dnpt", _CmdDrmNdMultipassTest, NULL, "", CLI_SUPERVISOR
    },
#endif
#endif
#ifdef TEST_MODE
    {
        "drmndselftest", "dnst", _CmdDrmNdSelfTest, NULL, "", CLI_SUPERVISOR
    },
#endif
    {
        "patternmatch", "pm", _CmdPatternMatch, NULL, "", CLI_SUPERVISOR
    },

    {
        "psitest", "psitest", _CmdPSITest, NULL, "", CLI_SUPERVISOR
    },

    {
        "mulmatch", "mmatch", _CmdMultipleMatchTest, NULL, "", CLI_SUPERVISOR
    },

	{
		"psicombotest", "psictest", _CmdPSIComboTest, NULL, "", CLI_SUPERVISOR
	},
/*
    {
        "starttsoutput", "stso", _CmdStartTSOutput, NULL, "", CLI_SUPERVISOR
    },
*/
    {
        "tsout", "tsout", _DMXCLI_TsOutput, NULL, "Diagnose CI with TS-output", CLI_SUPERVISOR
    },

    {
        "tsoutbackstop", "tsoutbs", _DMXCLI_TsOutBackStop, NULL, "", CLI_SUPERVISOR
    },

    {
        "tsoutframer", "tsoutf", _DMXCLI_TsOutFramer, NULL, "", CLI_SUPERVISOR
    },

    {
        "dvbtest", "dvb", _CmdDvbTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpestest", "ppt", _CmdFVRPesTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpsitest", "pvrpsi", _CmdPSILoopbackTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtimestamp", "pvrts", _CmdFVRTimestampTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtspause", "pvrtsp", _CmdFVRTimestampPauseTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtserr", "pvrtserr", _CmdFVRTimestampErrTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtsreset", "pvrtsr", _CmdFVRTimestampResetTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpsicatest", "pvrpsica", _CmdPSICALoopbackTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "bypass", "bypass", _CmdBypassTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "prebyte", "prebyte", _CmdPreByteTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "dmafifotest", "dftest", _CmdDMAFIFOTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(DmxEmu)
{
    "dmxemu", "demu", NULL, _arDmxEmuCmdTbl, "Demux emulation commands", CLI_GUEST

};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DMX_EMU_SetTsIndex(UINT8 u1TsIndex)
{
    UINT32 u4Reg = 0, u4CkGenReg = 0x260;

    _u1TsIndex = u1TsIndex;

    u4CkGenReg = 0x24c;

    switch (u1TsIndex)
    {
    case 0:
        u4Reg = 0x00020002;
        break;

    case 1:
        u4Reg = 0x00000000;
        break;

    case 2:
        LOG(3, "TS2 is not for normal input!\n");
        return;

    case 3:
        u4Reg = 0x00020002;
        break;

    default:
        LOG(3, "Unknown TS index number!\n");
        return;
    }

    CKGEN_WRITE32(u4CkGenReg, u4Reg);   // Select DMX clock as TS0 clock source
}


#if 0
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetDmxEmuMwCmdTbl(void)
#else
CLI_EXEC_T* GetDmxEmuCmdTbl(void)
#endif
{
    return _arDmxEmuCmd;
}

#endif
