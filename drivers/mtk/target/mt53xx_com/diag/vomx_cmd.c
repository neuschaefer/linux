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
#include "srm_drvif.h"

#ifdef __KERNEL__
#include "source_table.h"  // nptv

#include <linux/fb.h>
#endif

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_typedef.h"
#include "x_os.h"

#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)            // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)            // Info 818: Pointer parameter 'szArgv'


/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _VomxInitCmd (INT32 i4Argc, const CHAR ** szArgv);
#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)
extern INT32 _i4VConv_PrintStatus(HANDLE_T h_Handle);
extern INT32 _i4VConv_Pause (HANDLE_T h_Handle);
extern INT32 _i4VConv_Resume (HANDLE_T h_Handle);
extern BOOL i4VDOOmxPrintStatus(UINT32 u4InstId);
extern BOOL i4VDOOmxSetInfo(UINT32 u4InstId, UINT32 u4Info, UINT32 u4Para1, UINT32 u4Para2, UINT32 u4Para3);

static INT32 _VomxQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VomxSetDelayCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VomxVConvPauseCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VomxVConvResumeCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VomxVConvQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

/******************************************************************************
* CLI Command Table
******************************************************************************/
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(VOMX)
LINT_RESTORE

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)
static CLI_EXEC_T _arVomxVConvCmdTbl[] =
{
 	{"pause",		NULL,	_VomxVConvPauseCmd, NULL, "pause", CLI_SUPERVISOR},
 	{"resume",		NULL,	_VomxVConvResumeCmd, NULL, "resume", CLI_SUPERVISOR},
 	{"query",		"q",	_VomxVConvQueryCmd, NULL, "vconv query", CLI_SUPERVISOR},
 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arVomxCmdTbl[] =
{
    {"init",        "i",    _VomxInitCmd, NULL,          "Vomx init", CLI_SUPERVISOR},
#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)
    {"query",       "q",    _VomxQueryCmd, NULL,          "Vomx query", CLI_SUPERVISOR},
    {"delay",       "dly",    _VomxSetDelayCmd, NULL,       "Vomx set delay", CLI_SUPERVISOR},
 	{"vconv",	    "vc",	NULL, _arVomxVConvCmdTbl, "vconv", CLI_SUPERVISOR},
#endif

LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(VOMX),
LINT_RESTORE

    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Vomx)
{
    "vomx",
    NULL,
    NULL,
    _arVomxCmdTbl,
    "Video OMX command",
    CLI_SUPERVISOR
};
LINT_RESTORE


/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _VomxInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    LOG(0, "_VomxInitCmd test cmd\n");

    return 0;
}

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)

static INT32 _VomxVConvPauseCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InstId = 0;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "Arg: instance id, use 0\n");
    }
    else
    {
        u4InstId = (UINT32)StrToInt(szArgv[1]);
    }
    if(u4InstId >= 2)
    {
        LOG(0, "u4InstId >= 2\n");
        return 0;
    }
    _i4VConv_Pause((HANDLE_T)u4InstId);
    return 0;
}

static INT32 _VomxVConvResumeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InstId = 0;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "Arg: instance id, use 0\n");
    }
    else
    {
        u4InstId = (UINT32)StrToInt(szArgv[1]);
    }
    if(u4InstId >= 2)
    {
        LOG(0, "u4InstId >= 2\n");
        return 0;
    }
    _i4VConv_Resume((HANDLE_T)u4InstId);
    return 0;
}

static INT32 _VomxVConvQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InstId = 0;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "Arg: instance id, use 0\n");
    }
    else
    {
        u4InstId = (UINT32)StrToInt(szArgv[1]);
    }
    if(u4InstId >= 2)
    {
        LOG(0, "u4InstId >= 2\n");
        return 0;
    }
    UNUSED(_i4VConv_PrintStatus((HANDLE_T)u4InstId));
    return 0;
}


static INT32 _VomxQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InstId = 0;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "Arg: instance id, use 0\n");
    }
    else
    {
        u4InstId = (UINT32)StrToInt(szArgv[1]);
    }
    if(u4InstId >= 2)
    {
        LOG(0, "u4InstId >= 2\n");
        return 0;
    }
    UNUSED(i4VDOOmxPrintStatus(u4InstId));
    return 0;
}


static INT32 _VomxSetDelayCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4InstId = 0;
    UINT32 u4Delay = 0;
    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "Arg: instance id, delay time\n");
        return 0;
    }
    u4InstId = (UINT32)StrToInt(szArgv[1]);
    if(u4InstId >= 2)
    {
        LOG(0, "u4InstId >= 2\n");
        return 0;
    }
    u4Delay = (UINT32)StrToInt(szArgv[2]);
    // VOMX_IO_SET_OUTPUT_DELAY = 0
    UNUSED(i4VDOOmxSetInfo(u4InstId, 0, u4Delay, 0, 0));
    return 0;
}

#endif


