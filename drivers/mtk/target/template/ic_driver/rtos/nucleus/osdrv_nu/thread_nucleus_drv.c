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
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#include "drv_common.h"
#include "driver_os.h"
#include "nucleus.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_5381.h"

//---------------------------------------------------------------------

// Default time slices: 5 ticks
#define DEFAULT_TASK_SLICE      5

// Time interval per timer tick
#define MS_PER_TICK             (1000 / NU_PLUS_Ticks_Per_Second)

// Thread signature
#define THREAD_SIGNATURE        0x8712fe56


//---------------------------------------------------------------------

// Thread control block in OS driver
typedef struct
{
    NU_TASK                     ntId;
    UINT32                      u4Signature;
    SIZE_T                      zStackSize;
    void*                       pvStack;
    x_os_drv_thread_main_fct    pfnThread;
    UINT8                       u1Priority;
    VOID*                       pvTag;
} OS_DRV_THREAD_T;

//---------------------------------------------------------------------

// Task that performs thread exit reaping
static NU_TASK s_taskExitReaper;

// Mailbox used by exit reaper task
static NU_MAILBOX s_mbThread;

//============================================================================
// Access of thread control block list

/*----------------------------------------------------------------------------
 * Function: _FindThread
 *
 * Description:
 *      Given a NU_TASK, find its corresponding thread in thread list
 *
 * Inputs:
 *      pTask: The task to be found
 *
 * Outputs: -
 *
 * Returns:
 *      The found thread pointer, or NULL if not found
 *---------------------------------------------------------------------------*/
static OS_DRV_THREAD_T* _FindThread(const NU_TASK* pTask)
{
    OS_DRV_THREAD_T* pThread;

    if (pTask == NULL)
    {
        return NULL;
    }

    pThread = (OS_DRV_THREAD_T*)pTask;
    if (pThread->u4Signature != THREAD_SIGNATURE)
    {
        pThread = NULL;
    }

    return pThread;
}

//============================================================================

/*----------------------------------------------------------------------------
 * Function: ExitReaperProc
 *
 * Description:
 *      The thread body of the exit reaper, which perform the real termination
 *      operations for exiting threads.
 *
 * Inputs:
 *      argc: Unused
 *      argc: Unused
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID ExitReaperProc(UNSIGNED argc, VOID* argv)
{
    UNSIGNED au4Message[4];
    OS_DRV_THREAD_T* pThread;

    UNUSED(argc);
    UNUSED(argv);

    while (1)
    {
        // Wait for exiting request
        VERIFY(NU_Receive_From_Mailbox(&s_mbThread, au4Message, NU_SUSPEND)
            == NU_SUCCESS);

        // Extract thread to be terminated
        pThread = (OS_DRV_THREAD_T*)au4Message[0];
        ASSERT(pThread != NULL);

        // Terminate the thread
        VERIFY(NU_Terminate_Task(&pThread->ntId) == NU_SUCCESS);
        VERIFY(NU_Delete_Task(&pThread->ntId) == NU_SUCCESS);

        // Release resources
        DriverMemFree(pThread->pvStack);
        DriverMemFree(pThread);
    }
}


/*----------------------------------------------------------------------------
 * Function: ThreadExit
 *
 * Description:
 *      Terminate current thread, by sending a message to the exit reaper,
 *      then suspending itself waiting for reaping.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID ThreadExit(VOID)
{
    NU_TASK* pTask;
    OS_DRV_THREAD_T* pThread;
    UNSIGNED au4Message[4];

    pTask = NU_Current_Task_Pointer();
    if (pTask == NU_NULL)
    {
        // No task is active, or in an HISR context currently
        return;
    }

    pThread = _FindThread(pTask);
    if (pThread == NULL)
    {
        // No thread control block associates with current task. Current
        // task was created by calling Nucleus native interface, rather
        // than through OS Support interface.
        ASSERT(0);
    }

    // Send a message to exit management thread to terminate this thread
    au4Message[0] = (UNSIGNED)pThread;
    VERIFY(NU_Send_To_Mailbox(&s_mbThread, au4Message, NU_SUSPEND)
        == NU_SUCCESS);

    // Suspend this thread to wait termination
    VERIFY(NU_Suspend_Task(pTask) == NU_SUCCESS);
}


/*----------------------------------------------------------------------------
 * Function: ThreadProc
 *
 * Description:
 *      Body of threads
 *      1. Translate thread arguments between prototypes of OS Support and
 *         Nucleus.
 *      2. Invoke user thread body.
 *      3. Perform thread termination, reclaim resources.
 *
 * Inputs:
 *      argc: Should be 1
 *      argc: A pointer to the thread control block of current thread
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID ThreadProc(UNSIGNED argc, VOID* argv)
{
    OS_DRV_THREAD_T* pThread;

    ASSERT(argc == 1);
    pThread = (OS_DRV_THREAD_T*)argv;
    ASSERT(pThread != NULL);

    // Invoke the original thread function
    pThread->pfnThread(NULL);

    // Terminate thread
    ThreadExit();
}

//============================================================================

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_create
 *
 * Description:
 *      this API creates a thread. it performs
 *      1. allocate memory for thread's wait struct to be used for suspend
 *         and resume.
 *      2. initialize thread's attributes and set scheduling to be realtime,
 *         round-robin (SCHED_RR).
 *      3. scale ui1_pri to fall in underlying OS priority range.
 *      4. call OS driver thread create routine passing the generic main
 *         function "os_drv_thread_main" and thread's wait struct as argument.
 *
 * Inputs:
 *      ps_name: name of this thread, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *      z_stacksize: stack size this thread will run on.
 *      ui1_pri: priority of this thread, 1- highest, 255 - lowest.
 *      ui2_flags: thread flag
 *      pf_main: main entry of this thread.
 *
 * Outputs:
 *      pph_th_id: pointer to hold returned thread id.
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NO_RESOURCE: no resources (memory).
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_create(VOID         **ppv_th_id,        /* thread id from driver */
                       const CHAR   *ps_name,
                       SIZE_T       z_stacksize,
                       UINT8        ui1_pri,            /* thread priority */
                       UINT16       ui2_flags,          /* thread flags */
                       VOID*        pv_tag,
                       x_os_drv_thread_main_fct pf_main)
{
    OS_DRV_THREAD_T* pThread;
    void* pvStack;
    INT32 i4Ret = OSR_DRV_NO_RESOURCE;
    CHAR szTaskName[NU_MAX_NAME];

    ASSERT(ppv_th_id != NULL);
    UNUSED(ui2_flags);

    // Make sure the stack size is aligned
    z_stacksize = ((z_stacksize + MEMORY_ALIGNMENT) - 1) & (~(MEMORY_ALIGNMENT - 1));

    // Allocate thread control block and stack
    pThread = DriverMemAlloc(sizeof(OS_DRV_THREAD_T));
    pvStack = DriverMemAlloc(z_stacksize);
    if ((pThread == NULL) || (pvStack == NULL))
    {
        goto ERR_RESOURCE;
    }

    // Construct thread control block, add to thread list
    pThread->u4Signature = THREAD_SIGNATURE;
    pThread->pvStack = pvStack;
    pThread->zStackSize = z_stacksize;
    pThread->pfnThread = pf_main;
    pThread->u1Priority = ui1_pri;
    pThread->pvTag = pv_tag;

    // NU_Create_Task() will check signature in NU_TASK structure to decide if
    // the structure had been used already. Thus it's necessary to destroy the
    // signature here.
    x_memset(&pThread->ntId, 0, sizeof(NU_TASK));

    // Zero stack, for stack coverage checking
    x_memset(pvStack, 0, z_stacksize);

    // Set thread id. This must be done before creating the actual thread.
    *ppv_th_id = (VOID*)&pThread->ntId;

    // Copy task name, since the length limitations in OSAI and Nucleus are
    // different
    UNUSED(x_strncpy(szTaskName, ps_name, NU_MAX_NAME - 1));
    szTaskName[NU_MAX_NAME - 1] = '\0';

     // Create thread
    if (NU_Create_Task(&pThread->ntId, szTaskName, ThreadProc, 1, pThread,
        pvStack, z_stacksize, ui1_pri, DEFAULT_TASK_SLICE, NU_PREEMPT, NU_START)
        != NU_SUCCESS)
    {
        goto ERR_THREAD;
    }

    return OSR_DRV_OK;

ERR_THREAD:

    i4Ret = OSR_DRV_FAIL;

ERR_RESOURCE:

    DriverMemFree(pvStack);
    DriverMemFree(pThread);

    return i4Ret;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_exit
 *
 * Description:
 *      this API exits a thread. it performs
 *      1. get thread id.
 *      2. guard the hash table that houses thread's wait struct.
 *      3. remove thread's wait struct from hash table.
 *      4. call underlying OS thread exit routine.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_exit(VOID)
{
    ThreadExit();
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_delay
 *
 * Description:
 *      this API allows a thread to give up CPU for a duration. it performs
 *      1. convert ui4_delay to seconds and microseconds.
 *      2. call nanosleep() to delay for the duration.
 *
 * Inputs: -
 *      ui4_delay: in millisecond (ms) the thread gives up CPU.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_delay(UINT32 u4_delay_in_ms)
{
    UINT32 u4Ticks;

    if (u4_delay_in_ms == 0)
    {
        // Delaying 0 means yielding execution
        NU_Relinquish();
        return;
    }

    u4Ticks = ((u4_delay_in_ms + MS_PER_TICK) - 1) / MS_PER_TICK; // Round up
    NU_Sleep(u4Ticks);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_self
 *
 * Description:
 *      this API gets the calling thread id. it performs
 *      1. call underlying OS thread self.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *      OSR_DRV_OK : routine was successful.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_self(VOID** ppv_th_id, VOID** ppv_tag)
{
    NU_TASK* pTask;
    OS_DRV_THREAD_T* pThread;

    ASSERT(ppv_th_id != NULL);
    ASSERT(ppv_tag != NULL);

    pTask = NU_Current_Task_Pointer();
    *ppv_th_id = (VOID*)pTask;
    *ppv_tag = NULL;

    pThread = _FindThread(pTask);
    if (pThread != NULL)
    {
        *ppv_tag = pThread->pvTag;
    }

    return OSR_DRV_OK;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_get_pri
 *
 * Description:
 *      this API gets a thread's priority. it performs
 *      1. call underlying OS scheduling parameter routine.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs:
 *      *pui1_pri: pointer to hold returned thread priority.
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_get_pri(VOID *pv_th_id, UINT8 *pui1_pri)
{
    NU_TASK* pTask;
    OS_DRV_THREAD_T* pThread;

    ASSERT(pui1_pri != NULL);

    pTask = (NU_TASK*)pv_th_id;
    pThread = _FindThread(pTask);
    if (pThread == NULL)
    {
        // No such thread
        return OSR_DRV_FAIL;
    }

    *pui1_pri = pThread->u1Priority;

    return OSR_DRV_OK;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_set_pri
 *
 * Description:
 *      this API sets a thread's priority. it performs
 *      1. scale ui1_pri to fall in underlying OS priority range.
 *      2. call underlying OS scheduling parameter routine to set priority.
 *      4. save ui1_pri in wait struct as back up use.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *      ui1_pri: new thread priority, 1-highest, 255-lowest.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_INV_ARG : invalid thread id, pv_th_id.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_set_pri(VOID *pv_th_id,
                        UINT8 ui1_pri)
{
    NU_TASK* pTask;
    OS_DRV_THREAD_T* pThread;
    OPTION opOldPriority;

    pTask = (NU_TASK*)pv_th_id;
    pThread = _FindThread(pTask);
    if (pThread == NULL)
    {
        // No such thread
        return OSR_DRV_FAIL;
    }

    opOldPriority = NU_Change_Priority(pTask, ui1_pri);
    UNUSED(opOldPriority);
    pThread->u1Priority = ui1_pri;  // No need to enter critical section?

    return OSR_DRV_OK;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_suspend
 *
 * Description:
 *      this API suspends a thread. it performs
 *      1. get thread id.
 *      2. look up for thread's wait struct based on thread id.
 *      3. set thread wait to lock state.
 *      4. call underlying OS semop with opcode 0 which blocks thread since
 *         thread wait is in lock state.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_suspend(VOID)
{
    NU_TASK* pTask;

    pTask = NU_Current_Task_Pointer();
    if (pTask)
    {
        VERIFY(NU_Suspend_Task(pTask) == NU_SUCCESS);
    }
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_resume
 *
 * Description:
 *      this API resumes a thread. it performs
 *      1. look up for thread's wait struct based on thread id.
 *      2. set thread wait to unlock state.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_THREAD_ACTIVE: Thread already active
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_resume(VOID *pv_th_id)
{
    STATUS status;
    INT32 i4Ret;
    NU_TASK* pTask = (NU_TASK*)pv_th_id;

    // No need to check if the given task in the thread list?

    i4Ret = OSR_DRV_FAIL;
    status = NU_Resume_Task(pTask);
    if (status == NU_INVALID_RESUME)
    {
        // Here the assumption is that the thread can be only in unconditional
        // suspended state or in ready state. Thread suspended on semaphore,
        // message queue, etc. cannot be resumed. Thus if the status is
        // NU_INVALID_RESUME, the only reason is the thread is already in
        // ready state.
        //
        i4Ret = OSR_DRV_THREAD_ACTIVE;
    }
    else if (status == NU_SUCCESS)
    {
        i4Ret = OSR_DRV_OK;
    }

    return i4Ret;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_stack_stats
 *
 * Description:
 *      This function returns the stack size and maximum used stack of a given
 *      thread
 *
 * Inputs
 *      pv_thread_id: The thread to get stack states
 *
 * Outputs:
 *      pz_alloc_stack: Pointer of the thread stack size
 *      pz_max_used_stack: Pointer of the maximum used stack size
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL : OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_stack_stats (VOID*    pv_thread_id,
                             SIZE_T*  pz_alloc_stack,
                             SIZE_T*  pz_max_used_stack)
{
    NU_TASK* pTask;
    OS_DRV_THREAD_T* pThread;
    UINT32* pu4Stack;
    SIZE_T i;

    // Find the corresponding thread data
    pTask = (NU_TASK*)pv_thread_id;
    pThread = _FindThread(pTask);
    if (pThread == NULL)
    {
        return OSR_DRV_FAIL;
    }

    // Probe stack to determine used stack size
    pu4Stack = (UINT32*)(pThread->pvStack);
    for (i = 0; i < (pThread->zStackSize / 4); i++)
    {
        if (pu4Stack[i] != 0)
        {
            break;
        }
    }

    ASSERT(pz_max_used_stack != NULL);
    *pz_max_used_stack = pThread->zStackSize - (i * 4);
    ASSERT(pz_alloc_stack != NULL);
    *pz_alloc_stack = pThread->zStackSize;

    return OSR_DRV_OK;
}


//---------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Function: os_drv_thread_init
 *
 * Description:
 *      this routine is called as part of os_drv_init(). it performs
 *      1. get underlying OS min. max. scheduling priorities.
 *      2. create a semaphore to guard thread hash table.
 *      3. allocate space for OS driver thread hash table.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NO_RESOURCE : no resource (memory).
 *      OSR_DRV_FAIL : OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
os_drv_thread_init(void)
{
    void* pvStack;

    // Initialize mailbox used to perform thread exiting
    x_memset(&s_mbThread, 0, sizeof(NU_MAILBOX));
    if (NU_Create_Mailbox(&s_mbThread, "OSDRV", NU_FIFO) != NU_SUCCESS)
    {
        return OSR_DRV_FAIL;
    }

    // Allocate stack for reaper stack
    // Note that OS is not ready yet in this stage, thus memory should be
    // allocated from BSP directly
    pvStack = BSP_AllocateReserved(REAPER_STACK_SIZE);
    if (pvStack == NULL)
    {
        return OSR_DRV_FAIL;
    }
    x_memset(pvStack, 0, REAPER_STACK_SIZE);  // Zero stack for coverage checking

    // Create the exit reaper thread
    x_memset(&s_taskExitReaper, 0, sizeof(NU_TASK));
    if (NU_Create_Task(&s_taskExitReaper, REAPER_THREAD_NAME, ExitReaperProc,
        0, NULL, pvStack, REAPER_STACK_SIZE, REAPER_PRIORITY,
        DEFAULT_TASK_SLICE, NU_PREEMPT, NU_START) != NU_SUCCESS)
    {
        return OSR_DRV_FAIL;
    }

    return OSR_DRV_OK;
}


/*----------------------------------------------------------------------------
 * Function: os_drv_thread_release
 *
 * Description:
 *      this routine is called as part of os_drv_exit() to release resources
 *      in thread driver
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NO_RESOURCE : no resource (memory).
 *      OSR_DRV_FAIL : OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
os_drv_thread_release(void)
{
    INT32 i4Ret = OSR_DRV_OK;

    // Destroy the reaper thread
    if (NU_Terminate_Task(&s_taskExitReaper) == NU_SUCCESS)
    {
        if (NU_Delete_Task(&s_taskExitReaper) != NU_SUCCESS)
        {
            i4Ret = OSR_DRV_FAIL;
        }
    }
    else
    {
        i4Ret = OSR_DRV_FAIL;
    }

    // Release mailbox
    if (NU_Delete_Mailbox(&s_mbThread) != NU_SUCCESS)
    {
        i4Ret = OSR_DRV_FAIL;
    }

    return i4Ret;
}


