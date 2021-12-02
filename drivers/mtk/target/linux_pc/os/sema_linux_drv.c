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
 * $RCSfile: sema_linux_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/* driver-OS header files */
#include "driver_os.h"
#include "thread_linux_drv.h"
#include "sema_linux_drv.h"

/* Linux thread does not support non-zero (shared) in the second argument */
/* of sem_init() */
#define NO_SHARE    0   /* 2nd argument of sem_init() */


extern void *malloc();
extern void free();

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
    sem_t   *pt_sema;
    unsigned int ui4_val;
    int i4_ret;

    pt_sema = (sem_t *) malloc(sizeof(sem_t));
    if (pt_sema == (sem_t *) NULL)
    {
        return(OSR_DRV_FAIL);
    }

    ui4_val = (ui4_init_value == (UINT32) X_SEMA_STATE_LOCK) ?
              (unsigned int) SEMA_LOCK_VALUE : (unsigned int) SEMA_UNLOCK_VALUE;

    i4_ret = sem_init(pt_sema, NO_SHARE, ui4_val);
    if (i4_ret ==0)
    {
        /* NOTE: free ppv_sema_id when semaphore is destroyed */
        *ppv_sema_id = (VOID *) pt_sema;     /* return driver semaphore id */
        return(OSR_DRV_OK);
    }
    else
    {
        return(OSR_DRV_FAIL);
    }
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
    int i4_ret;

    i4_ret = sem_destroy(pv_sema_id);

    if (i4_ret == 0)
    {
        free(pv_sema_id);
        return(OSR_DRV_OK);
    }
    else
    {
        return(OSR_DRV_FAIL);
    }
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
    int i4_ret;

    if (e_option == X_SEMA_OPTION_WAIT)
    {
        do {
            /* man page says sem_wait() always returns 0 */
            /* it is not true. so we still have to check errno */
            i4_ret = sem_wait((sem_t *) pv_sema_id);

            if ((i4_ret < 0) && (errno == EINTR))
            {
                continue;
            }
            else
            {
                break;
            }
        } while (TRUE);

        return(OSR_DRV_OK);
    }
    else
    {
        i4_ret = sem_trywait((sem_t *) pv_sema_id);
        if (i4_ret == 0)
        {
            return(OSR_DRV_OK);
        }
        else
        {
            if ((i4_ret < 0) && (errno == EAGAIN))
            {
                return(OSR_DRV_WOULD_BLOCK);
            }
            else
            {
                return(OSR_DRV_FAIL);
            }
        }
    }
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_sema_lock_timeout()
 *
 * Description:
 *      this API locks a semaphore. it performs
 *      1. call OS semaphore nowait routine.
 *      2. if errno is EAGAIN, delay the thraed and go back to step 1 until
 *         duration is expired, otherwise, return a proper code. 
 *
 * Inputs:
 *      pv_sema_id: semaphore id returned via x_os_drv_sema_create().
 *      ui4_time: time duration in ms before thie API bails out.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful. 
 *      OSR_DRV_FAIL: OS driver related errors.
 *      OSR_DRV_TIMEOUT: failed to lock semaphore within duration.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_sema_lock_timeout(VOID     *pv_sema_id,
                           UINT32   ui4_time)
{
    int i4_ret;
    UINT32 ui4_tick_period;
    UINT32 ui4_end, ui4_now;
    extern UINT32 ui4_drv_tick;


    ui4_tick_period = x_os_drv_get_tick_period();
    ui4_end = ui4_drv_tick + (ui4_time / ui4_tick_period);
    if ((ui4_time % ui4_tick_period) >= (ui4_tick_period >> 1))
    {
        ++ui4_end;
    }

    do
    {
        i4_ret = sem_trywait((sem_t *) pv_sema_id);     /* nowait */
        ui4_now = ui4_drv_tick;
        if (i4_ret < 0)
        {
            if (errno == EAGAIN)
            {
                if (ui4_now >= ui4_end)
                {
                    return(OSR_DRV_TIMEOUT);
                }

                (VOID) x_os_drv_thread_delay((UINT32) ui4_tick_period);
            }
            else
            {
                return(OSR_DRV_FAIL);
            }
        }
        else
        {
            return(OSR_DRV_OK);
        }
    } while (TRUE);
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
    int i4_ret;
                                                                                
    i4_ret = sem_post((sem_t *) pv_sema_id);

    if (i4_ret == 0)
    {
        return(OSR_DRV_OK);
    }
    else
    {
        return(OSR_DRV_FAIL);
    }
}
