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
*  OS Diagnostic command
*****************************************************************************/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_os.h"
#include "x_hal_arm.h"

#ifdef __KERNEL__
#include "x_linux.h"
#include <asm/io.h>
#endif

#define DEFINE_IS_LOG   OS_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)            // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)            // Info 818: Pointer parameter 'szArgv'

//#define ENABLE_MAX_DRAM_WRITE


/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _OsShowSemaphoreCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsCreateMaxDramWriteCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsCreateModifyDramPriorityCmd (INT32 i4Argc, const CHAR ** szArgv);
extern INT32 os_cli_show_sema_all(INT32 i4Argc, const CHAR ** szArgv);

/******************************************************************************
* CLI Command Table
******************************************************************************/
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(OS)
LINT_RESTORE

static CLI_EXEC_T _arOsCmdTbl[] =
{
    {"show semaphore all",       "ssa",   _OsShowSemaphoreCmd, NULL, "Show all semaphore information", CLI_SUPERVISOR},
    {"max dram write",           "mdw",   _OsCreateMaxDramWriteCmd, NULL, "Create a thread to write dram at full speed", CLI_SUPERVISOR},
    {"stress dram prio",         "sdp",   _OsCreateModifyDramPriorityCmd, NULL, "Create a thread to dynamic change dram priority", CLI_SUPERVISOR},
    
LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(OS),
LINT_RESTORE

    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Os)
{
    "os",
    NULL,
    NULL,
    _arOsCmdTbl,
    "OS command",
    CLI_SUPERVISOR
};
LINT_RESTORE

#define MAX_MEM_WRITE_SIZE (1024*1024)

/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _OsShowSemaphoreCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    /* FIXME for linux verison */
#ifndef __KERNEL__
    UNUSED(os_cli_show_sema_all(0, NULL));
#endif
    
    return 0;
}

/*
 * pu4Dst: Destination memory address
 * u4Count: Number of 1K bytes
 */
void _FastMemSet(UINT32 * pu4Dst, UINT32 u4Count)
{
#ifdef ENABLE_MAX_DRAM_WRITE
    INT32 i;
    register UINT32 u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9;

    u4r2 = u4r3 = u4r4 = u4r5 = u4r6 = u4r7 = u4r8 = u4r9 = 0xAABBCCDD;
    
    for (i=0; i<u4Count; i++) {

        #ifdef __KERNEL__
        // store 4K bytes at a time 
        __asm__
        (
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"

            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"

            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"

            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}\n\t"
            "stmia   %0!, {%2, %3, %4, %5, %6, %7, %8, %9}"
            : "=r" (pu4Dst)
            : "0" (pu4Dst), "r" (u4r2), "r" (u4r3), "r" (u4r4), "r" (u4r5), "r" (u4r6), "r" (u4r7), "r" (u4r8), "r" (u4r9));

        #else
            
        __asm {
            /* store 4K bytes at a time */
            //1
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}

            //2
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}

            //3
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            
            //4
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
            stmia pu4Dst!, {u4r2, u4r3, u4r4, u4r5, u4r6, u4r7, u4r8, u4r9}
        }
        #endif
    }
#else
    UNUSED(pu4Dst);
    UNUSED(u4Count);
#endif //ENABLE_MAX_DRAM_WRITE
}

// PSR bit definitions
#ifndef PSR_IRQ_DISABLE
#define PSR_IRQ_DISABLE             (1 << 7)
#endif
#ifndef PSR_FIQ_DISABLE
#define PSR_FIQ_DISABLE             (1 << 6)
#endif

#ifndef __KERNEL__
static void _ModifyDRAMPrio(UINT32 u4Prio1, UINT32 u4Prio2)
{
    register UINT32 u4Reg, u4RegBase;
    register UINT32 u4Flags, u4Stat;

    __asm
    {
        MRS    u4Flags, CPSR
        ORR    u4Stat, u4Flags, #(PSR_IRQ_DISABLE | PSR_FIQ_DISABLE)
        MSR    CPSR_c, u4Stat
        MOV    u4RegBase, #0xF0007000
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP        
        LDR    u4Reg, [u4RegBase, #0x180]
        ORR    u4Reg, u4Reg, #0x20000000
        STR    u4Reg, [u4RegBase, #0x180]
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP        
        STR    u4Prio1, [u4RegBase, #0x100]
        STR    u4Prio2, [u4RegBase, #0x104]
        AND    u4Reg, u4Reg, #-0x30000000
        STR    u4Reg, [u4RegBase, #0x180]
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        MSR    CPSR_c, u4Flags
    }

#if 0
    // Stop DRAM agent access
    u4Reg = *(volatile UINT32 *) 0XF0007180;
    u4Reg |= 0x20000000;
    *(volatile UINT32 *) 0XF0007180 = u4Reg;

    for (i=0; i<10; i++)
    {
    }

    // Modify priority
    *(volatile UINT32 *) 0XF0007100 = u4Prio1;
    *(volatile UINT32 *) 0XF0007104 = u4Prio2;

    // Resume DRAM agent access
    u4Reg &= 0xD0000000;
    *(volatile UINT32 *) 0XF0007180 = u4Reg;

    __asm
    {
        MSR     CPSR_c, u4Flags
    }

    for (i=0; i<10; i++)
    {
    }
#endif
}

#else

// Linux version
static void _ModifyDRAMPrio(UINT32 u4Prio1, UINT32 u4Prio2)
{
    UNUSED(u4Prio1);
    UNUSED(u4Prio2);
}

#endif

static void _Copy_ModifyDRAMPrio(void * pvDest)
{
    x_memcpy((void*) pvDest, (void*)_ModifyDRAMPrio, 1024);
    HalFlushInvalidateDCache();
}

typedef void FUNC_PTR(UINT32 u4Prio1, UINT32 u4Prio2);

static void _DramModifyPriorityThread(void *pvArgv)
{
    static UINT32 u4Prio1=0x49362510, u4Prio2=0x78ffffff;
    FUNC_PTR * SramModifyDRAMPrio = (FUNC_PTR *) 0xFB004000;
    
    UNUSED(pvArgv);

#ifdef __KERNEL__
    SramModifyDRAMPrio = (FUNC_PTR *) ioremap_nocache((int)0xFB004000, 4096);
#endif // __KERNEL__

    Printf("Stress test dynamic change dram priority.\n");

    if (SramModifyDRAMPrio)
    {
        _Copy_ModifyDRAMPrio((void*)SramModifyDRAMPrio);
    }

    while (1)
    {
        if (SramModifyDRAMPrio)
        {
            SramModifyDRAMPrio(u4Prio1, u4Prio2);
        }
        u4Prio1 = ((u4Prio1 & 0xF0000000) >> 28) | (u4Prio1 << 4);
        u4Prio2 = ((u4Prio2 & 0xF0000000) >> 28) | (u4Prio2 << 4);
    }
}

static UINT32 _u4CPUMaxWriteLoop = 1;
static UINT32 _u4CPUMaxWriteLoopDelay = 0;

static void _DramMaxWriteThread(void *pvArgv)
{
    UINT32 * pMem;
    
    UNUSED(pvArgv);

#ifndef ENABLE_MAX_DRAM_WRITE
    Printf("Define ENABLE_MAX_DRAM_WRITE to turn on the function.\n");
#endif // ENABLE_MAX_DRAM_WRITE

    pMem = (UINT32 *) x_mem_alloc(MAX_MEM_WRITE_SIZE);
    if (!pMem)
    {
        return;
    }

    Printf("Max dram write thread is created.\n");

    while (_u4CPUMaxWriteLoop)
    {
        _FastMemSet(pMem, MAX_MEM_WRITE_SIZE/4096);
        x_thread_delay(_u4CPUMaxWriteLoopDelay);
    }

    x_mem_free(pMem);
}

static INT32 _OsCreateMaxDramWriteCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc > 1) 
    { 
        _u4CPUMaxWriteLoop = (UINT32)StrToInt(szArgv[1]);
        if (i4Argc > 2)
        {
            _u4CPUMaxWriteLoopDelay =  (UINT32)StrToInt(szArgv[2]);
        }
        return 0;
    } 

    UNUSED(x_thread_create(&hThread, "DRAM_MW", 4096, 255, _DramMaxWriteThread, 0, NULL));
    
    return 0;
}

static INT32 _OsCreateModifyDramPriorityCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    UNUSED(x_thread_create(&hThread, "DRAM_CHG_PRIO", 4096, 255, _DramModifyPriorityThread, 0, NULL));
    
    return 0;
}

