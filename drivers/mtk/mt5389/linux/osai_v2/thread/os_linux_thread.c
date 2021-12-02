/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *^M
 *                                                                            *^M
 *   This software/firmware and related documentation ("MediaTek Software")   *^M
 * are protected under international and related jurisdictions'copyright laws *^M
 * as unpublished works. The information contained herein is confidential and *^M
 * proprietary to MediaTek Inc. Without the prior written permission of       *^M
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *^M
 * MediaTek Software, and information contained herein, in whole or in part,  *^M
 * shall be strictly prohibited.                                              *^M
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *^M
 *                                                                            *^M
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *^M
 * AGREES TO THE FOLLOWING:                                                   *^M
 *                                                                            *^M
 *   1)Any and all intellectual property rights (including without            *^M
 * limitation, patent, copyright, and trade secrets) in and to this           *^M
 * Software/firmware and related documentation ("MediaTek Software") shall    *^M
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *^M
 * property rights (including without limitation, patent, copyright, and      *^M
 * trade secrets) in and to any modifications and derivatives to MediaTek     *^M
 * Software, whoever made, shall also remain the exclusive property of        *^M
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *^M
 * title to any intellectual property right in MediaTek Software to Receiver. *^M
 *                                                                            *^M
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *^M
 * representatives is provided to Receiver on an "AS IS" basis only.          *^M
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *^M
 * including but not limited to any implied warranties of merchantability,    *^M
 * non-infringement and fitness for a particular purpose and any warranties   *^M
 * arising out of course of performance, course of dealing or usage of trade. *^M
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *^M
 * software of any third party which may be used by, incorporated in, or      *^M
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *^M
 * such third parties for any warranty claim relating thereto.  Receiver      *^M
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *^M
 * from any third party all proper licenses contained in or delivered with    *^M
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *^M
 * releases made to Receiver's specifications or to conform to a particular   *^M
 * standard or open forum.                                                    *^M
 *                                                                            *^M
 *   3)Receiver further acknowledge that Receiver may, either presently       *^M
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *^M
 * development and the implementation, in accordance with Receiver's designs, *^M
 * of certain softwares relating to Receiver's product(s) (the "Services").   *^M
 * Except as may be otherwise agreed to in writing, no warranties of any      *^M
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *^M
 * to the Services provided, and the Services are provided on an "AS IS"      *^M
 * basis. Receiver further acknowledges that the Services may contain errors  *^M
 * that testing is important and it is solely responsible for fully testing   *^M
 * the Services and/or derivatives thereof before they are used, sublicensed  *^M
 * or distributed. Should there be any third party action brought against     *^M
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *^M
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *^M
 * mutually agree to enter into or continue a business relationship or other  *^M
 * arrangement, the terms and conditions set forth herein shall remain        *^M
 * effective and, unless explicitly stated otherwise, shall prevail in the    *^M
 * event of a conflict in the terms in any agreements entered into between    *^M
 * the parties.                                                               *^M
 *                                                                            *^M
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *^M
 * cumulative liability with respect to MediaTek Software released hereunder  *^M
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *^M
 * MediaTek Software at issue.                                                *^M
 *                                                                            *^M
 *   5)The transaction contemplated hereunder shall be construed in           *^M
 * accordance with the laws of Singapore, excluding its conflict of laws      *^M
 * principles.  Any disputes, controversies or claims arising thereof and     *^M
 * related thereto shall be settled via arbitration in Singapore, under the   *^M
 * then current rules of the International Chamber of Commerce (ICC).  The    *^M
 * arbitration shall be conducted in English. The awards of the arbitration   *^M
 * shall be final and binding upon both parties and shall be entered and      *^M
 * enforceable in any court of competent jurisdiction.                        *^M
 *---------------------------------------------------------------------------*/


#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/cb_data.h>
#include "linux/mt53xx_sc.h"
#include "cb_low.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"

//---------------------------------------------------------------------


#define MEMORY_ALIGNMENT        8

#define THREAD_NAME_LEN         16
#define THREAD_PRI_RANGE_LOW    (UINT8) 1
#define THREAD_PRI_RANGE_HIGH   (UINT8) 254
#define DEFAULT_TASK_SLICE      5

// Thread user space priority, higher number means higher priority
#define MAX_THREAD_PRIORITY  (99)
#define MIN_THREAD_PRIORITY  (36)

// User space nice values, lower number means higher priority
// Max and min "number" of nice values
#define MAX_USER_NICE_VALUE  (20)
#define MMIN_USER_NICE_VALUE (-20)

typedef struct os_thread_pvt_light
{
    struct os_thread_pvt_light *previous;
    struct os_thread_pvt_light *next;

    UINT32  ui4_key;
    x_os_thread_pvt_del_fct  pf_pvt_del;
    VOID *pv_pvt;
} OS_THREAD_PVT_LIGHT_T;


typedef struct os_thread_light
{
    struct os_thread_light *previous;
    struct os_thread_light *next;
    struct task_struct *task;
    //void *pv_stack;
    //SIZE_T z_stack_size;
    CHAR s_name[THREAD_NAME_LEN + 1];
    UINT8 ui1_priority;
    x_os_thread_main_fct pf_main_rtn;
    OS_THREAD_PVT_LIGHT_T *pt_pvt;
    SIZE_T z_arg_size;
    UINT8 au1_arg_local[1];
} OS_THREAD_LIGHT_T;


//static DEFINE_SPINLOCK(x_thread_lock);
static DEFINE_RWLOCK(x_thread_lock);

//---------------------------------------------------------------------


static OS_THREAD_LIGHT_T *s_thread_list;


static void thread_list_add(OS_THREAD_LIGHT_T *pt_thread)
{
    if (s_thread_list != NULL)
    {
        pt_thread->previous = s_thread_list->previous;
        pt_thread->next = s_thread_list;
        s_thread_list->previous->next = pt_thread;
        s_thread_list->previous = pt_thread;
    }
    else
    {
        s_thread_list = pt_thread->next = pt_thread->previous = pt_thread;
    }
}


static void thread_list_remove(OS_THREAD_LIGHT_T *pt_thread)
{
    if (pt_thread->previous == pt_thread)
    {
        s_thread_list = NULL;
    }
    else
    {
        pt_thread->previous->next = pt_thread->next;
        pt_thread->next->previous = pt_thread->previous;
        if (s_thread_list == pt_thread)
        {
            s_thread_list = pt_thread->next;
        }
    }
}


static OS_THREAD_LIGHT_T *thread_find_handle(HANDLE_T h_th_hdl)
{
    OS_THREAD_LIGHT_T *pt_thread = s_thread_list;
    if (pt_thread == NULL)
    {
        return NULL;
    }
    do
    {
        if (pt_thread == (OS_THREAD_LIGHT_T *)(h_th_hdl))
        {
            return pt_thread;
        }
        pt_thread = pt_thread->next;
    } while (pt_thread != s_thread_list);

    return NULL;
}


static OS_THREAD_LIGHT_T *thread_find_obj(const CHAR *ps_name)
{
    OS_THREAD_LIGHT_T *pt_thread = s_thread_list;
    if (pt_thread == NULL)
    {
        return NULL;
    }
    do
    {
        if (strncmp(pt_thread->s_name, ps_name, THREAD_NAME_LEN) == 0)
        {
            return pt_thread;
        }
        pt_thread = pt_thread->next;
    } while (pt_thread != s_thread_list);

    return NULL;
}


static void ThreadExit(void)
{
    unsigned long flags;
    OS_THREAD_LIGHT_T *pt_thread;
    struct task_struct *task;

    VERIFY(x_thread_self((HANDLE_T *)&pt_thread) == OSR_OK);

    //local_irq_save(flags);
    write_lock_irqsave(&x_thread_lock, flags);
    thread_list_remove(pt_thread);
    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_thread_lock, flags);

    task = pt_thread->task;

    //kfree(pt_thread->pv_stack);
    kfree(pt_thread);

    complete_and_exit(NULL, 0);
}


static int ThreadProc(void *arg)
{
    OS_THREAD_LIGHT_T *pt_thread = (OS_THREAD_LIGHT_T *)arg;
    ASSERT(pt_thread != NULL);

    /*
     * Mapping priorities from 0-255 to 1-64 (user prio 99-36)
     * Priority 255 is special, we move it to traditional priority.
     * Also, it can't call x_thread_set_pri().
     */
    if (pt_thread->ui1_priority < 255)
    {
        SC_CB_T rSysCallCb;
        rSysCallCb.i4Fct = 0;
        rSysCallCb.i4Arg0 = pt_thread->task->pid;
        rSysCallCb.i4Arg1 = MAX_THREAD_PRIORITY - (((UINT32)pt_thread->ui1_priority) >> 2);
        ASSERT((rSysCallCb.i4Arg1 >= MIN_THREAD_PRIORITY) && (rSysCallCb.i4Arg1 <= MAX_THREAD_PRIORITY));
        _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);
    }
    else
    {
        SC_CB_T rSysCallCb;
        rSysCallCb.i4Fct = 1;
        rSysCallCb.i4Arg0 = pt_thread->task->pid;
        rSysCallCb.i4Arg1 = MAX_USER_NICE_VALUE;
        _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);
    }

    // Invoke the original thread function
    pt_thread->pf_main_rtn(pt_thread->z_arg_size != 0 ? pt_thread->au1_arg_local : NULL);

    // Terminate thread
    ThreadExit();

    return 0;
}


INT32 x_thread_create (HANDLE_T*             ph_th_hdl,
                       const CHAR*           ps_name,
                       SIZE_T                z_stack_size,
                       UINT8                 ui1_priority,
                       x_os_thread_main_fct  pf_main_rtn,
                       SIZE_T                z_arg_size,
                       VOID*                 pv_arg)
{
    OS_THREAD_LIGHT_T *pt_thread;
    //void *pv_stack;
    unsigned long flags;

    if (pv_arg == NULL)
    {
        z_arg_size = 0;
    }

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_th_hdl == NULL) ||
        //(z_stack_size == 0) ||
        (pf_main_rtn == NULL) ||
        (ui1_priority < THREAD_PRI_RANGE_LOW) || (ui1_priority > THREAD_PRI_RANGE_HIGH) ||
        ((pv_arg != NULL) && (z_arg_size == 0)) ||
        ((pv_arg == NULL) && (z_arg_size != 0)))
    {
        return OSR_INV_ARG;
    }

    // Make sure the stack size is aligned
    //z_stack_size = (z_stack_size + MEMORY_ALIGNMENT - 1) & (~(MEMORY_ALIGNMENT - 1));

    pt_thread = kcalloc(1, sizeof(OS_THREAD_LIGHT_T) - sizeof(UINT8) + z_arg_size, GFP_KERNEL);
    //pv_stack = kcalloc(1, z_stack_size);
    if (pt_thread == NULL /*|| pv_stack == NULL*/)
    {
        //kfree(pv_stack);
        kfree(pt_thread);
        return OSR_NO_RESOURCE;
    }
    //FILL_CALLER(pt_thread);
    //FILL_CALLER(pv_stack);

    //pt_thread->pv_stack = pv_stack;
    strncpy(pt_thread->s_name, ps_name, THREAD_NAME_LEN);
    pt_thread->s_name[THREAD_NAME_LEN] = 0;
    pt_thread->pf_main_rtn = pf_main_rtn;
    pt_thread->z_arg_size = z_arg_size;
    if (z_arg_size != 0)
    {
        memcpy(pt_thread->au1_arg_local, pv_arg, z_arg_size);
    }

    //local_irq_save(flags);
    write_lock_irqsave(&x_thread_lock, flags);
    if (thread_find_obj(ps_name) != NULL)
    {
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_thread_lock, flags);
        //kfree(pv_stack);
        kfree(pt_thread);
        return OSR_EXIST;
    }

    thread_list_add(pt_thread);
    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_thread_lock, flags);

    pt_thread->task = kthread_create(&ThreadProc, pt_thread, ps_name);
    if (pt_thread->task == ERR_PTR(-ENOMEM))
    {
        //local_irq_save(flags);
        write_lock_irqsave(&x_thread_lock, flags);
        thread_list_remove(pt_thread);
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_thread_lock, flags);
        //kfree(pv_stack);
        kfree(pt_thread);
        return OSR_NO_RESOURCE;
    }
    else if (IS_ERR(pt_thread->task))
    {
        //local_irq_save(flags);
        write_lock_irqsave(&x_thread_lock, flags);
        thread_list_remove(pt_thread);
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_thread_lock, flags);
        //kfree(pv_stack);
        kfree(pt_thread);
        return OSR_FAIL;
    }
    else
    {
        pt_thread->ui1_priority = ui1_priority;
    }

    *ph_th_hdl = (HANDLE_T)(pt_thread);

    wake_up_process(pt_thread->task);
    
    return OSR_OK;
}


VOID x_thread_exit (VOID)
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
 * Function: x_thread_delay()
 *
 * Description:
 *      this API allows a thread to give up CPU for a duration. it performs
 *      1. check if thread mudule is initiated. ^F
 *      2. call OS driver thread delay routine.
 *
 * Inputs:
 *      ui4_delay: in millisecond (ms) the thread gives up CPU.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_thread_delay(UINT32 ui4_delay)
{

    if (ui4_delay != 0)
    {
        x_os_drv_thread_delay(ui4_delay);
    }
}

INT32 x_thread_set_pri (HANDLE_T  h_th_hdl,
                        UINT8     ui1_new_pri)
{
    OS_THREAD_LIGHT_T *pt_thread;
    unsigned long flags;
    SC_CB_T rSysCallCb;

    if ((ui1_new_pri < THREAD_PRI_RANGE_LOW) || (ui1_new_pri > THREAD_PRI_RANGE_HIGH))
    {
        return OSR_INV_ARG;
    }

    //local_irq_save(flags);
    write_lock_irqsave(&x_thread_lock, flags);
    pt_thread = thread_find_handle(h_th_hdl);
    if (pt_thread == NULL)
    {
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_thread_lock, flags);
        return OSR_INV_HANDLE;
    }

    rSysCallCb.i4Fct = 0;
    rSysCallCb.i4Arg0 = pt_thread->task->pid;
    rSysCallCb.i4Arg1 = MAX_THREAD_PRIORITY - (((UINT32)pt_thread->ui1_priority) >> 2);
    ASSERT((rSysCallCb.i4Arg1 >= MIN_THREAD_PRIORITY) && (rSysCallCb.i4Arg1 <= MAX_THREAD_PRIORITY));
    _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);

    pt_thread->ui1_priority = ui1_new_pri;

    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_thread_lock, flags);

    return OSR_OK;
}



INT32 x_thread_get_pri (HANDLE_T  h_th_hdl,
                        UINT8*    pui1_pri)
{
    OS_THREAD_LIGHT_T *pt_thread;
    unsigned long flags;

    if (pui1_pri == NULL)
    {
        return OSR_INV_ARG;
    }

    //local_irq_save(flags);
    read_lock_irqsave(&x_thread_lock, flags);
    pt_thread = thread_find_handle(h_th_hdl);
    if (pt_thread == NULL)
    {
        //local_irq_restore(flags);
        read_unlock_irqrestore(&x_thread_lock, flags);
        return OSR_INV_HANDLE;
    }
    *pui1_pri = pt_thread->ui1_priority;
    //local_irq_restore(flags);
    read_unlock_irqrestore(&x_thread_lock, flags);

    return OSR_OK;
}


INT32 x_thread_get_name (HANDLE_T  h_th_hdl,
                        UINT32* s_name)
{
    OS_THREAD_LIGHT_T *pt_thread;
    unsigned long flags;

    //local_irq_save(flags);
    read_lock_irqsave(&x_thread_lock, flags);
    pt_thread = thread_find_handle(h_th_hdl);
    if (pt_thread == NULL)
    {
        //local_irq_restore(flags);
        read_unlock_irqrestore(&x_thread_lock, flags);
        return OSR_INV_HANDLE;
    }
    *s_name =(UINT32) &pt_thread->s_name[0];
    //local_irq_restore(flags);
    read_unlock_irqrestore(&x_thread_lock, flags);

    return OSR_OK;
}


VOID x_thread_suspend (VOID)
{
    ASSERT(0);
}


INT32 x_thread_resume (HANDLE_T  h_th_hdl)
{
    return OSR_NOT_SUPPORT;
}


INT32 x_thread_self (HANDLE_T *ph_th_hdl)
{
    OS_THREAD_LIGHT_T *pt_thread;
    unsigned long flags;

    if (ph_th_hdl == NULL)
    {
        return OSR_INV_ARG;
    }

    //local_irq_save(flags);
    read_lock_irqsave(&x_thread_lock, flags);
    pt_thread = s_thread_list;
    if (pt_thread == NULL)
    {
        return OSR_NOT_EXIST;
    }
    do
    {
        if (pt_thread->task == current)
        {
            //local_irq_restore(flags);
            read_unlock_irqrestore(&x_thread_lock, flags);
            *ph_th_hdl = (HANDLE_T)(pt_thread);
            return OSR_OK;
        }
        pt_thread = pt_thread->next;
    } while (pt_thread != s_thread_list);
    //local_irq_restore(flags);
    read_unlock_irqrestore(&x_thread_lock, flags);

    //FIXME: need to fake one?
    *ph_th_hdl = (HANDLE_T) current;

    //return OSR_NOT_EXIST;
    return OSR_OK;
}


INT32 x_thread_stack_stats (HANDLE_T  h_th_hdl,
                            SIZE_T*   pz_alloc_stack,
                            SIZE_T*   pz_max_used_stack)
{
    return OSR_NOT_SUPPORT;
}


INT32 x_thread_set_pvt (UINT32                   ui4_key,
                        x_os_thread_pvt_del_fct  pf_pvt_del,
                        VOID*                    pv_pvt)
{
    return OSR_NOT_SUPPORT;
}


INT32 x_thread_get_pvt (UINT32  ui4_key,
                        VOID**  ppv_pvt)
{
    return OSR_NOT_SUPPORT;
}


INT32 x_thread_del_pvt (UINT32  ui4_key)
{
    return OSR_NOT_SUPPORT;
}


INT32 os_thread_init(VOID)
{
    return OSR_OK;
}


INT32
os_cli_show_thread_all(INT32        i4_argc,
                       const CHAR   **pps_argv)
{
    return OSR_OK;
}


/*----------------------------------------------------------------------------
 * Function: x_thread_set_affinity()
 *
 * Description:
 *      this API sets a thread's affinity mask
 *      the thread will only run on specified cpu.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *      cpu_mask: new mask specify CPU to run
 *
 * Outputs: -
 *
 * NOTES:
 *      This function is only avaliable in Linux kernel mode.
 *      Empty function in Nucleus.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_thread_set_affinity (HANDLE_T  h_th_hdl,
                             UINT32    cpu_mask)
{
#ifdef CONFIG_SMP
    OS_THREAD_LIGHT_T *pt_thread;
    SC_CB_T rSysCallCb;
    unsigned long flags;
    
    if (cpu_mask == 0)
    {
        return(OSR_INV_ARG);
    }

    //local_irq_save(flags);
    read_lock_irqsave(&x_thread_lock, flags);
    pt_thread = thread_find_handle(h_th_hdl);
    if (pt_thread == NULL)
    {
        //local_irq_restore(flags);
        read_unlock_irqrestore(&x_thread_lock, flags);
        return OSR_INV_HANDLE;
    }

    //local_irq_restore(flags);
    read_unlock_irqrestore(&x_thread_lock, flags);

    ASSERT(!in_interrupt());
    ASSERT(cpu_mask);

    /* Fire callback event to user space */
    rSysCallCb.i4Fct = SC_FCT_SETAFFINITY;
    rSysCallCb.i4Arg0 = pt_thread->task->pid;
    rSysCallCb.i4Arg1 = cpu_mask;
    _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);

    return OSR_OK;
#endif /* CONFIG_SMP */

    return(OSR_OK);
}

/* FIXME: ask user to stop use it */
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
    ASSERT(ppv_th_id != NULL);
    ASSERT(ppv_tag != NULL);
    ASSERT(!in_interrupt());

    *ppv_th_id = (VOID*)current->pid;
    *ppv_tag = (VOID*)current->pid;

    return OSR_DRV_OK;
}

/* Called by MW */

INT32
_thread_init(void)
{
    return(OSR_OK);
}

INT32 ioctl_thread_end(void)
{
    return(OSR_OK);
}

