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

#include "driver_os.h"
#include "x_list.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"
#include "os.h"
#include "linux/mt53xx_sc.h"
#include <linux/cb_data.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
#include <linux/sched.h>
#endif
#include "x_linux.h"
#include <linux/interrupt.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
#include <linux/pid.h>
#include <linux/nsproxy.h>
#endif
//---------------------------------------------------------------------

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
#define DECLARE_MUTEX(mutex)  DEFINE_SEMAPHORE(mutex)
#endif

// Thread user space priority, higher number means higher priority
#define MAX_THREAD_PRIORITY  (99)
#define MIN_THREAD_PRIORITY  (36)

// User space nice values, lower number means higher priority
// Max and min "number" of nice values
#define MAX_USER_NICE_VALUE  (20)
#define MMIN_USER_NICE_VALUE (-20)

#define THREAD_LIST_MAX      (64)
#define THREAD_LIST_BITS     (6)

#define GET_THREAD_LIST_INDEX(pid) (((UINT32)(pid)) & (((UINT32)1<<THREAD_LIST_BITS)-1))

// List of thread control blocks.
static LIST_T s_arlistThread[THREAD_LIST_MAX];

static DEFINE_SPINLOCK(s_list_lock);

typedef struct
{
    pid_t                       pid;
    VOID*                       pv_arg;
    CHAR*                       ps_name;
    x_os_drv_thread_main_fct    pf_main_rtn;
    UINT8                       ui1_pri;
    struct semaphore            r_sem;
    struct semaphore            r_sem_sr;
} OS_DRV_THREAD_T;

extern VOID x_thread_faked_main_func(VOID*);
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

/*----------------------------------------------------------------------------
 * Function: _AddThread
 *
 * Description:
 *      Add a thread to thread list
 *
 * Inputs:
 *      pThread: The thread pointer to be added
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Failed
 *---------------------------------------------------------------------------*/
static BOOL _AddThread(OS_DRV_THREAD_T* pThread)
{
    BOOL fgOk;
    unsigned long flags;

    spin_lock_irqsave(&s_list_lock, flags);

    fgOk = ListAddTail(&s_arlistThread[GET_THREAD_LIST_INDEX(pThread->pid)], pThread);

    spin_unlock_irqrestore(&s_list_lock, flags);

    return fgOk;
}


/*----------------------------------------------------------------------------
 * Function: _RemoveThread
 *
 * Description:
 *      Remove thread from thread list
 *
 * Inputs:
 *      pThread: The thread pointer to be removed
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Failed
 *---------------------------------------------------------------------------*/
static BOOL _RemoveThread(OS_DRV_THREAD_T* pThread)
{
    POSITION pos;
    VOID * pRet;
    INT32 i4Index;
    unsigned long flags;

    spin_lock_irqsave(&s_list_lock, flags);
    
    i4Index = GET_THREAD_LIST_INDEX(pThread->pid);

    pos = ListFind(&s_arlistThread[i4Index], pThread);
    ASSERT(pos);

    pRet = ListRemoveAt(&s_arlistThread[i4Index], pos);
    ASSERT((unsigned long)pRet == (unsigned long)pThread);

    spin_unlock_irqrestore(&s_list_lock, flags);

    return TRUE;
}


/*----------------------------------------------------------------------------
 * Function: _FindThread
 *
 * Description:
 *      Given a task_struct, find its corresponding thread in thread list
 *
 * Inputs:
 *      pId: The task id to be found
 *
 * Outputs: -
 *
 * Returns:
 *      The found thread pointer, or NULL if not found
 *---------------------------------------------------------------------------*/
static OS_DRV_THREAD_T* _FindThread(const pid_t pId)
{
    POSITION pos;
    OS_DRV_THREAD_T* pThreadFound = NULL;
    INT32 i4Index;
    unsigned long flags;

    spin_lock_irqsave(&s_list_lock, flags);

    i4Index = GET_THREAD_LIST_INDEX(pId);

    pos = ListGetHeadPosition(&s_arlistThread[i4Index]);
    while (pos)
    {
        OS_DRV_THREAD_T* pThread;

        pThread = (OS_DRV_THREAD_T*)ListGetNext(&s_arlistThread[i4Index], &pos);
        if (pThread->pid == pId)
        {
            pThreadFound = pThread;
            break;
        }
    }

    spin_unlock_irqrestore(&s_list_lock, flags);

    return pThreadFound;
}

static int thread_code(void *data)
{
    OS_DRV_THREAD_T *pThread = (OS_DRV_THREAD_T *)data;

    daemonize(pThread->ps_name);
    allow_signal(SIGTERM);
    down(&pThread->r_sem);

    /*
     * Mapping priorities from 0-255 to 1-64 (user prio 99-36)
     * Priority 255 is special, we move it to traditional priority.
     * Also, it can't call x_thread_set_pri().
     */
    if (pThread->ui1_pri < 255)
    {
        SC_CB_T rSysCallCb;
        /* Fire callback event to user space */
        rSysCallCb.i4Fct = 0;
        rSysCallCb.i4Arg0 = pThread->pid;
        /* Map to user space priority */
        rSysCallCb.i4Arg1 = MAX_THREAD_PRIORITY - (((UINT32)pThread->ui1_pri) >> 2);
        ASSERT((rSysCallCb.i4Arg1 >= MIN_THREAD_PRIORITY) && (rSysCallCb.i4Arg1 <= MAX_THREAD_PRIORITY));
        _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);
    }
    else
    {
        SC_CB_T rSysCallCb;
        /* Fire callback event to user space */
        rSysCallCb.i4Fct = 1;
        rSysCallCb.i4Arg0 = pThread->pid;
        /* Set to lowest user space priority */
        rSysCallCb.i4Arg1 = MAX_USER_NICE_VALUE;
        _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);
    }

    pThread->pf_main_rtn(pThread->pv_arg);

    /* thread calls x_thread_exit() or the thread_main() will do exit */
    up(&pThread->r_sem);

    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
struct task_struct* x_os_drv_thread_by_pid(pid_t pid)
{
    struct task_struct *pTask, *pThread;

    do_each_thread(pTask, pThread)
    {
        if (task_pid_nr(pThread) == pid)
        {
            return pThread;
        }
    }
    while_each_thread(pTask, pThread);
    return NULL;
}
#endif

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
    OS_DRV_THREAD_T *pThread;
    int thread_id;
    INT32 i4_pri;
    BOOL fgFakedThread = FALSE;

    ASSERT(!in_interrupt());

    pThread = (OS_DRV_THREAD_T *)kmalloc(sizeof(OS_DRV_THREAD_T), GFP_KERNEL);
    if (!pThread)
    {
        ASSERT(pThread);
        return OSR_DRV_NO_RESOURCE;
    }

    pThread->ps_name = (CHAR *)kmalloc(strlen(ps_name)+1, GFP_KERNEL);
    if (!pThread->ps_name)
    {
        kfree(pThread);
        return OSR_DRV_NO_RESOURCE;
    }

    strcpy(pThread->ps_name, ps_name);
    pThread->pf_main_rtn = pf_main;
    pThread->pv_arg      = pv_tag;
    pThread->ui1_pri     = ui1_pri;
    sema_init(&pThread->r_sem, 0);
    sema_init(&pThread->r_sem_sr, 0);

    if ((UINT32) pf_main == (UINT32) x_thread_faked_main_func)
    {
        fgFakedThread = TRUE;
        thread_id = current->pid;
    }
    else
    {
        //thread_id = kernel_thread(thread_code, (void*)pThread, CLONE_KERNEL|CLONE_STOPPED);
        thread_id = kernel_thread(thread_code, (void*)pThread, CLONE_KERNEL);
        if (thread_id == 0)
        {
            kfree(pThread->ps_name);
            kfree(pThread);
            return OSR_DRV_NO_RESOURCE;
        }
    }

    pThread->pid = (pid_t)thread_id;

    if (!_AddThread(pThread))
    {
        ASSERT(0);;
    }

    *ppv_th_id = (VOID*)thread_id;
   
    if (!fgFakedThread)
    {
        struct task_struct *pTask;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
        pTask = x_os_drv_thread_by_pid(thread_id);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
        pTask = find_task_by_vpid((pid_t)*ppv_th_id);
#else
        pTask = find_task_by_pid((pid_t)*ppv_th_id);
#endif
        if (pTask)
        {
            /* Set user nice priority first */
            i4_pri = ((UINT32)ui1_pri) * 40 / 256 - 20;
            i4_pri = i4_pri < -20 ? -20 : (i4_pri > 19 ? 19 : i4_pri);

            set_user_nice(pTask, i4_pri);

            up(&pThread->r_sem);
        }
    }

    return OSR_DRV_OK;
}

void x_os_drv_free_fake_thread(void)
{
    OS_DRV_THREAD_T* pThread;
    BOOL fgRet;

    pThread = _FindThread(current->pid);
    if (!pThread)
    {
        ASSERT(pThread);
        return;
    }
    ASSERT (pThread->pid == current->pid);
    ASSERT(!in_interrupt());

    fgRet = _RemoveThread(pThread);
    ASSERT(fgRet == TRUE);
    kfree(pThread->ps_name);
    kfree(pThread);
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_exit_specified
 *
 * Description:
 *      this API exits a specified thread. it performs
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
x_os_drv_thread_exit_specified(pid_t pid, HANDLE_T h_th_hdl)
{
    OS_DRV_THREAD_T* pThread;
    BOOL fgRet;

    /* make sure the task is still there */
    pThread = _FindThread(pid);
    if (!pThread)
    {
        ASSERT(pThread);
        return;
    }
    ASSERT(pThread->pid == pid);
    ASSERT(!in_interrupt());
    fgRet = _RemoveThread(pThread);
    ASSERT(fgRet == TRUE);
    kfree(pThread->ps_name);
    kfree(pThread);

    /* Free handle. Must do this after thread info is removed from 
       Linux OSDRV thread list. Otherwise the thread info might still
       get accessed if someone looking up the pid */
    handle_free(h_th_hdl, TRUE);

    /* Do exit if this is not for fake thread. */
    if (pid == current->pid)
    {
        complete_and_exit(NULL, 0);
        /* never returns */
        ASSERT(0);
    }
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

static void x_os_drv_msleep(unsigned int msecs)
{
    unsigned long timeout = msecs_to_jiffies(msecs);
    HAL_TIME_T rTime1, rTime2, rTimeResult;

    HAL_GetTime(&rTime1);

    while (timeout)
    {
        timeout = schedule_timeout_uninterruptible(timeout);
    }

    HAL_GetTime(&rTime2);
    HAL_GetDeltaTime(&rTimeResult, &rTime1, &rTime2);

    msecs -= (rTimeResult.u4Seconds * 1000);
    if ((int) msecs < 0)
    {
        return;
    }
    if ((msecs * 1000) < rTimeResult.u4Micros)
    {
        return;
    }
    
    schedule_timeout_uninterruptible(1);       
}

VOID
x_os_drv_thread_delay(UINT32 u4_delay_in_ms)
{
    ASSERT(!in_interrupt());

    if(u4_delay_in_ms == 0)
    {
        yield();
    }
    else
    {
        x_os_drv_msleep((unsigned int)u4_delay_in_ms);
    }
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
    OS_DRV_THREAD_T* pThread;

    ASSERT(ppv_th_id != NULL);
    ASSERT(ppv_tag != NULL);
    ASSERT(!in_interrupt());

    *ppv_th_id = (VOID*)current->pid;
    *ppv_tag = NULL;

    pThread = _FindThread(current->pid);
    if (!pThread || !pThread->pv_arg)
        return OSR_DRV_FAIL;

    *ppv_tag = pThread->pv_arg;
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
    OS_DRV_THREAD_T* pThread;

    pThread = _FindThread((pid_t)pv_th_id);
    if (!pThread)
    {
        ASSERT(pThread);
        return OSR_DRV_FAIL;
    }
    
    *pui1_pri = pThread->ui1_pri;
    
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
    OS_DRV_THREAD_T* pThread;
    SC_CB_T rSysCallCb;

    pThread = _FindThread((pid_t)pv_th_id);
    if (!pThread)
    {
        ASSERT(pThread);
        return OSR_DRV_FAIL;
    }

    ASSERT(!in_interrupt());
    ASSERT((pThread->ui1_pri < 255) && (ui1_pri < 255));

    /* Fire callback event to user space */
    rSysCallCb.i4Fct = 0;
    rSysCallCb.i4Arg0 = pThread->pid;
    /* Map to user space priority */
    rSysCallCb.i4Arg1 = MAX_THREAD_PRIORITY - (((UINT32)pThread->ui1_pri) >> 2);
    ASSERT((rSysCallCb.i4Arg1 >= MIN_THREAD_PRIORITY) && (rSysCallCb.i4Arg1 <= MAX_THREAD_PRIORITY));
    _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);

    pThread->ui1_pri = ui1_pri;

    return OSR_DRV_OK;
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_set_pri
 *
 * Description:
 *      this API sets a thread's affinity mask.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *      cpu_mask: specify cpu to run.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
#ifdef CONFIG_SMP
INT32 x_os_drv_thread_set_affinity (VOID   *pv_th_id,
                                    UINT32  cpu_mask)
{
    OS_DRV_THREAD_T* pThread;
    SC_CB_T rSysCallCb;

    pThread = _FindThread((pid_t)pv_th_id);
    if (!pThread)
    {
        ASSERT(pThread);
        return OSR_DRV_FAIL;
    }

    ASSERT(!in_interrupt());
    ASSERT(cpu_mask);

    /* Fire callback event to user space */
    rSysCallCb.i4Fct = SC_FCT_SETAFFINITY;
    rSysCallCb.i4Arg0 = pThread->pid;
    rSysCallCb.i4Arg1 = cpu_mask;
    _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);

    return OSR_DRV_OK;
}
#endif /* CONFIG_SMP */

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
    OS_DRV_THREAD_T* pThread;

    ASSERT(!in_interrupt());
    pThread = _FindThread(current->pid);
    ASSERT (pThread);
    ASSERT (pThread->pid == current->pid);

    down(&pThread->r_sem_sr);

    /* return from suspend */
    sema_init(&pThread->r_sem_sr, 0);
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
    pid_t pid;
    OS_DRV_THREAD_T* pThread;

    pid = (pid_t)pv_th_id;
    pThread = _FindThread(pid);
    if (!pThread)
    {
        ASSERT(pThread);
        return OSR_DRV_FAIL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
    if (!list_empty(&pThread->r_sem_sr.wait_list))
    {
        up(&pThread->r_sem_sr);
        return OSR_DRV_OK;
    }
#else
    if (sema_count(&pThread->r_sem_sr) == -1)
    {
        up(&pThread->r_sem_sr);
        up(&pThread->r_sem_sr);

        return OSR_DRV_OK;
    }
#endif
    
    return OSR_DRV_THREAD_ACTIVE;
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
    *pz_max_used_stack = 0;
    *pz_alloc_stack = 0;

    return OSR_DRV_OK;
}

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
void *DriverMemAlloc(UINT32 size)
{
    ASSERT(!in_interrupt());
    return kmalloc(size, GFP_KERNEL);
}

void DriverMemFree(void *ptr)
{
    kfree(ptr);
}

INT32 os_drv_thread_init(void)
{
    INT32 i4Index;

    for (i4Index=0; i4Index < THREAD_LIST_MAX; i4Index++)
    {
        // Initialize list of thread control block
        if (!ListInit(&s_arlistThread[i4Index], DriverMemAlloc, DriverMemFree))
        {
            return OSR_DRV_FAIL;
        }
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
    return OSR_DRV_OK;
}
