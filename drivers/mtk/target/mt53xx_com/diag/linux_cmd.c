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

#include "x_printf.h"
#include "x_stl_lib.h"

#include "x_mid.h"

#ifdef __KERNEL__
#include <linux/sched.h>
#include <linux/version.h>
#endif /* __KERNEL__ */

#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
#include "x_timer.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)            // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)            // Info 818: Pointer parameter 'szArgv'

/******************************************************************************
* Function prototype
******************************************************************************/

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef __KERNEL__
static UINT32 _Align(UINT32 u4Addr, UINT32 u4Alignment)
{
    UINT32 u4Unaligned;

    if (u4Alignment <= 1)
    {
        return u4Addr;
    }

    u4Unaligned = u4Addr % u4Alignment;
    if (u4Unaligned != 0)
    {
        u4Addr += u4Alignment - u4Unaligned;
    }

    return u4Addr;
}

extern INT32 mt53xx_get_rodata_addr(UINT32 *start, UINT32 *end);
extern INT32 mt53xx_get_rwdata_addr(UINT32 *start, UINT32 *end);
static INT32 i4CmdProtectRoData(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 start, end, u4Region;
    INT32 ret;

    mt53xx_get_rodata_addr(&start, &end);

    if (!(ret = MID_Init()))
    {
        Printf("Failed to initialize.\n");
        return ret;
    }
    
    if (!(ret = MID_AllocateFreeRegion(TRUE, &u4Region)))
    {
        Printf("Failed to find a free region.\n");
        return ret;
    }

    /* Align start and end address */
    start = _Align(start, MID_ADDR_ALIGNMENT);
    end = _Align(end, MID_ADDR_ALIGNMENT);   
    
    if (!(ret = MID_SetRegionEx(u4Region, MID_AGENT_NONE, start, end, 0)))
    {
        Printf("Failed to set region.\n");        
        return ret;
    }

    if (!(ret = MID_EnableRegionProtect(u4Region)))
    {
        Printf("Failed to enable region.\n");        
        return ret;
    }
    
    mt53xx_get_rwdata_addr(&start, &end);
    
    if (!(ret = MID_AllocateFreeRegion(FALSE, &u4Region)))
    {
        Printf("Failed to find a free region.\n");
        return ret;
    }

    /* Align start and end address */
    start = _Align(start, MID_ADDR_ALIGNMENT);
    end = _Align(end, MID_ADDR_ALIGNMENT);   
    
    if (!(ret = MID_SetRegionEx(u4Region, MID_AGENT_CPU, start, end, 0)))
    {
        Printf("Failed to set region.\n");        
        return ret;
    }

    if (!(ret = MID_EnableRegionProtect(u4Region)))
    {
        Printf("Failed to enable region.\n");        
        return ret;
    }

    return 0;
}

extern unsigned long u4MtalLogMask;
static INT32 i4CmdChangeMtalDebugMask(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4DbgMsk;

    if (i4Argc == 1)
    {
        Printf("Usage: mtal_dbg_msk value\n");
        return 0;
    }

    u4DbgMsk = (UINT32) StrToInt(szArgv[1]);
    u4MtalLogMask = u4DbgMsk;
    
    return 0;
}

static int TestFakeThreadMain(void *data)
{
    HANDLE_T h_th_hdl = NULL_HANDLE;
    daemonize("TestFake");
    allow_signal(SIGTERM);

    x_thread_self(&h_th_hdl);
    if (data)
    {
        printk("PID=%d, handle=0x%x\n", current->pid, (int)h_th_hdl);
    }

    return 0;
}

static INT32 i4CmdTestFakeThread(INT32 i4Argc, const CHAR** szArgv)
{
    INT32 i4Count;
    int i;
    int thread_id;

    if (i4Argc == 1)
    {
        Printf("Usage: tfthd count\n");
        return 0;
    }

    i4Count = StrToInt(szArgv[1]);

    for (i=0; i<i4Count; i++)
    {
        if (!(i % 1000))
        {
            thread_id = kernel_thread(TestFakeThreadMain, (void *) 0xFFFFFFFF, 0);
        }
        else
        {
            thread_id = kernel_thread(TestFakeThreadMain, (void *) NULL, 0);
        }
        //x_thread_delay(1);
    }

    return 0;
}

static void _ShowThreadStackUsage(struct task_struct *p)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27)
    unsigned long end_of_stack = (unsigned long)(p->stack);
    unsigned long start_of_stack = (unsigned long)(p->stack) + THREAD_SIZE;
    unsigned long *pMagicWord;
    
    /* check from end of stack, also skip the possible STACK_END_MAGIC for kernel overflow detection */
    pMagicWord = (unsigned long *) (end_of_stack + sizeof(struct thread_info) + 4);
   
    while (1)
    {
        if (*pMagicWord != 0xDEAD0118)
        {
            break;
        }

        pMagicWord++;

        if (pMagicWord > (unsigned long*)start_of_stack)
        {
            break;
        }
    }

    Printf("Thread %8d, start: 0x%x, end: 0x%x, usage: %8d\n", p->pid, start_of_stack, end_of_stack, start_of_stack - ((unsigned long) pMagicWord) + 4);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 27)
    unsigned long end_of_stack = (unsigned long)(p->thread_info);
    unsigned long start_of_stack = (unsigned long)(p->thread_info) + THREAD_SIZE;
    unsigned long *pMagicWord;
    
    /* check from end of stack */
    pMagicWord = (unsigned long *) (end_of_stack + sizeof(struct thread_info));
   
    while (1)
    {
        if (*pMagicWord != 0xDEAD0118)
        {
            break;
        }

        pMagicWord++;

        if (pMagicWord > (unsigned long*)start_of_stack)
        {
            break;
        }
    }

    Printf("Thread %8d, start: 0x%x, end: 0x%x, usage: %8d\n", p->pid, start_of_stack, end_of_stack, start_of_stack - ((unsigned long) pMagicWord) + 4);
#endif

}

static INT32 i4CmdListThreadStackUsage(INT32 i4Argc, const CHAR** szArgv)
{
    struct task_struct *g, *p;

    // Can't use tasklist_lock since they are not exported by kernel.
    // This might be a little dangerous, but should be OK since this is debug code.
    //read_lock(&tasklist_lock);

    do_each_thread(g, p)
    {
        _ShowThreadStackUsage(p);
    } while_each_thread(g, p);

    //read_unlock(&tasklist_lock); 
    
    return 0;
}

#endif /* __KERNEL__ */

static INT32 i4CmdTestXThreadDelay(INT32 i4Argc, const CHAR** szArgv)
{
    HAL_TIME_T rTime1, rTime2, rTimeResult;

    if (i4Argc == 1)
    {
        Printf("Usage: txtd value\n");
        return 0;
    }

    //Printf("Start x_thread_delay %d ms\n", StrToInt(szArgv[1]));
    HAL_GetTime(&rTime1);

    x_thread_delay(StrToInt(szArgv[1]));

    HAL_GetTime(&rTime2);
    //Printf("Finish x_thread_delay %d ms\n", StrToInt(szArgv[1]));

    HAL_GetDeltaTime(&rTimeResult, &rTime1, &rTime2);
    if (rTimeResult.u4Seconds == 0)
    {
        Printf("Elapsed %d us\n", rTimeResult.u4Micros);
    }
    
    return 0;
}

static INT32 i4CmdTestAllocDmaMem(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Size, u4Align, u4Count, i;
    UINT32 pMem;

    if (i4Argc <= 3)
    {
        Printf("Usage: tadm size align count\n");
        return 0;
    }

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Align = (UINT32) StrToInt(szArgv[2]);
    u4Count = (UINT32) StrToInt(szArgv[3]);

    for (i=0; i<u4Count; i++)
    {
        pMem = BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        Printf("Returned MEM pointer = 0x%x\n", pMem);
    }
    
    return 0;
}

/******************************************************************************
* CLI Command Table
******************************************************************************/
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(LINUX)
LINT_RESTORE

static CLI_EXEC_T _arLinuxCmdTbl[] =
{
#ifdef __KERNEL__
    {"protect_rodata",       "pr",   i4CmdProtectRoData, NULL, "Linux protect RO data", CLI_SUPERVISOR},
    {"change_mtal_debug_mask", "mtal_dbg_msk", i4CmdChangeMtalDebugMask, NULL, "Change MTAL debug mask", CLI_SUPERVISOR},
    {"test_fake_thread", "tfthd", i4CmdTestFakeThread, NULL, "Test fake thread creation", CLI_SUPERVISOR},
    {"list_thread_stack_usage", "ltsu", i4CmdListThreadStackUsage, NULL, "List thread stack usage", CLI_SUPERVISOR},
#endif /* __KERNEL__ */
    {"test_x_thread_delay", "txtd", i4CmdTestXThreadDelay, NULL, "Test x_thread_delay", CLI_SUPERVISOR},
    {"test_alloc_dma_mem", "tadm", i4CmdTestAllocDmaMem, NULL, "Test BSP_AllocAlignedDmaMemory", CLI_SUPERVISOR},
    
    CLIMOD_DEBUG_CLIENTRY(LINUX),

    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(linux)
{
    "linux",
    NULL,
    NULL,
    _arLinuxCmdTbl,
    "Linux commands",
    CLI_SUPERVISOR
};
LINT_RESTORE


