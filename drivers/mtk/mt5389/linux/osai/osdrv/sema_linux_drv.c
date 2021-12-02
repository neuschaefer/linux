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
 * $RCSfile: mtk_mod.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "x_linux.h"
#include "driver_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_timer.h"

#include <linux/interrupt.h>
#include <linux/version.h>

#define SEMAPHORE semaphore

typedef struct {
    struct SEMAPHORE   rSem;     // OS semaphore
    INT32              i4Count;  // 0: locked, 1: unlocked
    spinlock_t         spinlock; // counter maintenance lock
} SEM_T;

extern void msleep(unsigned int msecs);

/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_create()
 *
 * Description:
 *      this API creates a semaphore. it performs
 *      1. call underlying OS semget to create a semaphore.
 *      2. call underlying OS semctl to set the semaphore to ui4_init_value.
 *
 * Inputs:
 *      ui4_init_value: inital value of this semaphore.
 *
 * Outputs:
 *      ppv_sema_id: pointer to hold returned semaphore id.
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_create(VOID   **ppv_sema_id,    /* semaphore id */
                     UINT32 ui4_init_value)
{
    SEM_T *sem;

    ASSERT(!in_interrupt());

    sem = kmalloc(sizeof(SEM_T), GFP_KERNEL);
    ASSERT(sem);

    if (!sem)
    {
        return OSR_DRV_FAIL;
    }

    spin_lock_init(&sem->spinlock);
    
    if(ui4_init_value == X_SEMA_STATE_LOCK)
    {
        sema_init(&(sem->rSem), 0);
        sem->i4Count = 0;
    }
    else if(ui4_init_value == X_SEMA_STATE_UNLOCK)
    {
        sema_init(&(sem->rSem), 1);
        sem->i4Count = 1;
    }
    else
    {
        ASSERT(0);
    }

    *ppv_sema_id = (VOID*)sem;

    return OSR_DRV_OK;
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_delete()
 *
 * Description:
 *      this API deletes a semaphore. it performs
 *      1. unlock semaphore.
 *      2. call underlying OS semctl to removed semaphore.
 *
 * Inputs:
 *      pv_sema_id: semaphore id returned via x_os_drv_sema_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_delete(VOID *pv_sema_id)
{
    SEM_T *sem = (SEM_T *)pv_sema_id;

    ASSERT(!in_interrupt());

    kfree(sem);
    return OSR_DRV_OK;
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_lock()
 *
 * Description:
 *      this API locks a semaphore. it performs
 *      1. call underlying OS semop with proper flags.
 *
 * Inputs:
 *      pv_sema_id: semaphore id returned via x_os_drv_sema_create().
 *      e_option: either X_SEMA_OPTION_WAIT or X_SEMA_OPTION_NOWAIT.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *      OSR_DRV_WOULD_BLOCK: semaphore in lock state while X_SEMA_OPTION_NOWAIT
 *                           is specified.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_lock(VOID            *pv_sema_id,
                   SEMA_OPTION_T   e_option)
{
    SEM_T *sem = (SEM_T *)pv_sema_id;
    unsigned long flags;

    if(e_option == X_SEMA_OPTION_WAIT)
    {
        ASSERT(!in_interrupt());
        down(&(sem->rSem));
    }
    else if(e_option == X_SEMA_OPTION_NOWAIT)
    {
        if(down_trylock(&(sem->rSem)))
        {
            return OSR_DRV_WOULD_BLOCK;
        }
    }
    else
    {
        return OSR_DRV_FAIL;
    }

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);

    return OSR_DRV_OK;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void process_sema_timeout(unsigned long __data)
{
    struct task_struct *task = (struct task_struct *)__data;
    set_tsk_sema_timeout(task);
    wake_up_process(task);
}
#endif

/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_lock_timeout()
 *
 * Description:
 *      this API locks a semaphore. it performs
 *      1. call underlying OS semop with proper flags.
 *
 * Inputs:
 *      pv_sema_id: semaphore id returned via x_os_drv_sema_create().
 *      ui4_time: time duration in ms before this function bails out
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *      OSR_DRV_TIMEOUT: failed to lock the semaphore within the given time period
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_lock_timeout(VOID     *pv_sema_id,
                           UINT32   ui4_time)
{
    INT32 i4Ret;
    SEM_T *sem;
    unsigned long flags;

    ASSERT(!in_interrupt());

    sem = (SEM_T *)pv_sema_id;
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)

    i4Ret=down_timeout(&(sem->rSem),msecs_to_jiffies(ui4_time));
    if(i4Ret==-ETIME)
    	return OSR_DRV_TIMEOUT;

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);
    
    return OSR_DRV_OK;
#else
    struct timer_list timer;
    unsigned long expire;

    expire = msecs_to_jiffies(ui4_time) + jiffies;
    setup_timer(&timer, process_sema_timeout, (unsigned long)current);
    __mod_timer(&timer, expire);

    while (1)
    {
        /* Got semaphore before timeout */
        if (!down_interruptible_timeout(&(sem->rSem)))
        {
            i4Ret = OSR_DRV_OK;
            break;
        }
        
        if (sema_timeout(current))
        {
            i4Ret = OSR_DRV_TIMEOUT;
            break;
        }
    }
    del_singleshot_timer_sync(&timer);
    clear_tsk_sema_timeout(current);

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);

    return i4Ret;
#endif
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_unlock()
 *
 * Description:
 *      this API unlocks a semaphore. it performs
 *      1. call underlying OS semctl to set semaphore in unlock state.
 *
 * Inputs:
 *      pv_sema_id: semaphore id returned via x_os_drv_sema_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_unlock(VOID *pv_sema_id)
{
    SEM_T *sem = (SEM_T *)pv_sema_id;
    unsigned long flags;
    int to_wake_up = 0;

    // Lost wakeup:
    // Init as locked, ISR unlock, thread lock for waiting ISR => OK
    // Thread lock/unlock => OK
    // Thread lock/multiple ISR unlock => lost one unlock between thread got lock and not change i4Count to 0
    
    spin_lock_irqsave(&sem->spinlock, flags);
    if (likely(sem->i4Count == 0))
    {
        sem->i4Count = 1;
        to_wake_up = 1;
    }
    spin_unlock_irqrestore(&sem->spinlock, flags);

    if (to_wake_up)
    {
        up(&sem->rSem);
    }

    return OSR_DRV_OK;
}
