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
 * $RCSfile: feeder_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file muxer_cmd.c
 *  This file contains implementation of CLI CMD for Muxer
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 -e960 -e641
             -e613 -e836 */

#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"

#ifdef CC_DRIVER_DEMO
#include "drv_t32.h"
#endif

#define DEFINE_IS_LOG	MUXER_IsLog
#include "x_debug.h"

#include "muxer_if.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(MUXER)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _MuxerCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdStop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdProd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdStcRead(INT32 i4Argc, const CHAR **szArgv);
static INT32 _MuxerCmdStcWrite(INT32 i4Argc, const CHAR **szArgv);
static INT32 _MuxerCmdTestTS(INT32 i4Argc, const char **szArgv);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


// cmd table
static CLI_EXEC_T _arMuxerCmdTbl[] =
{
    {"init",    "i",    _MuxerCmdInit,        NULL, "Muxer test init",   CLI_SUPERVISOR},
    {"stop",    "s",    _MuxerCmdStop,        NULL, "Muxer test stop",   CLI_SUPERVISOR},
    {"prod",    "p",    _MuxerCmdProd,        NULL, "Muxer test produce output file",   CLI_SUPERVISOR},
    {"stcr",    "sr",   _MuxerCmdStcRead,     NULL, "Muxer test STC read",   CLI_SUPERVISOR},
    {"stcw",    "sw",   _MuxerCmdStcWrite,    NULL, "Muxer test STC write",   CLI_SUPERVISOR},
    {"tts",     "tts",  _MuxerCmdTestTS,      NULL, "Muxer test ATV to TS",   CLI_SUPERVISOR},

    CLIMOD_DEBUG_CLIENTRY(MUXER),     // for LOG use

    {NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static INT32 _MuxerCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Fmt;
    
    Printf("Muxer test init\n");
    
    if (i4Argc == 1)
    {
        u4Fmt = 4;  // TS
    }
    else
    {
        u4Fmt = StrToInt(szArgv[1]);
    }
    
    MUXER_CLI_TEST_Init(u4Fmt);
    
    return 0;
}


static INT32 _MuxerCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Muxer test stop\n");
    
    MUXER_Stop(0);
    
    return 0;
}


extern UCHAR *_pucMxBuf;
extern UINT32 _u4MxBufWp;

static INT32 _MuxerCmdProd(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Muxer test produce output file\n");
    
#ifdef CC_DRIVER_DEMO
    T32_HostExec(E_T32_EXEC, "d.save.binary \"d:/5395/mx_ts.ts\" 0x%08x--0x%08x", (UINT32)_pucMxBuf, (UINT32)_pucMxBuf + _u4MxBufWp - 1);
#endif
    
    return 0;
}


#ifdef CC_ATV_PVR_SUPPORT
extern UINT32 _AUD_ReadPsrStc1(void);
extern void _AUD_SetPsrStc1(UINT32 u4Val);
#endif

static INT32 _MuxerCmdStcRead(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef CC_ATV_PVR_SUPPORT
    Printf("Muxer test STC read: %d\n", _AUD_ReadPsrStc1());
#endif
    
    return 0;
}

static INT32 _MuxerCmdStcWrite(INT32 i4Argc, const char **szArgv)
{
#ifdef CC_ATV_PVR_SUPPORT
    UINT32 u4Val;
    
    if (i4Argc == 1)
    {
        u4Val = 0;
    }
    else
    {
        u4Val = StrToInt(szArgv[1]);
    }
    
    Printf("Muxer test write STC: %d\n", u4Val);
    _AUD_SetPsrStc1(u4Val);
#endif
    
    return 0;
}

static INT32 _MuxerCmdTestTS(INT32 i4Argc, const char **szArgv)
{
    BOOL fgStart = TRUE;
    BOOL fgVidOnly = TRUE;
    
    if (i4Argc >= 2)
    {
        if (szArgv[1][0] == '0')
            fgStart = TRUE;
        else
            fgStart = FALSE;
    }
    if (i4Argc >= 3)
    {
        if (szArgv[2][0] == '0')
            fgVidOnly = TRUE;
        else
            fgVidOnly = FALSE;
    }

    if (fgStart)
    {
        CLI_Parser("muxer.i");
        CLI_Parser("venc.i");
        CLI_Parser("venc.ld");
        CLI_Parser("venc.sfr 25");
        CLI_Parser("venc.sl 720 576");
        CLI_Parser("venc.sst 25 3600");
        CLI_Parser("venc.r");
        if (!fgVidOnly)
        {
            CLI_Parser("aud.stop");
            CLI_Parser("aud.uop.upi");
            CLI_Parser("aud.uop.up 1");
            CLI_Parser("aud.play");
        }
    }
    else
    {
        CLI_Parser("venc.s");
        if (!fgVidOnly)
        {
            CLI_Parser("aud.stop");
        }
        //CLI_Parser("muxer.s");
    }
    
    return 0;
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Muxer)
{
    "muxer",
    NULL,
    NULL,
    _arMuxerCmdTbl,
    "MUXER command",
    CLI_SUPERVISOR
};
LINT_RESTORE

/*lint -restore */



