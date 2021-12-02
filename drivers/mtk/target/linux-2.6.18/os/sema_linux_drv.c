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
 * $Revision: #2 $
 * $Date: 2012/11/22 $
 * $Author: zink.hsin $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/* driver-OS header files */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "driver_os.h"
#include "thread_linux_drv.h"
#include "sema_linux_drv.h"

typedef struct os_binary_semaphore_t {
    pthread_cond_t  cond;
    pthread_mutex_t mutex;
    UINT32          ui4_value;
}OS_BINARY_SEMAPHORE_T;

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
    OS_BINARY_SEMAPHORE_T *pt_bin_sem;
    pthread_condattr_t condattr;
    int ret1 = 0;
    int ret2 = 0;

    if (ppv_sema_id == NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_create ppv_sema_id=NULL\n");
        return OSR_DRV_FAIL;
    }

    pt_bin_sem = malloc(sizeof(OS_BINARY_SEMAPHORE_T));
    if (pt_bin_sem == (OS_BINARY_SEMAPHORE_T *) NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_create allocate sem fail\n");
        return OSR_DRV_FAIL;
    }

    memset (pt_bin_sem, 0, sizeof(OS_BINARY_SEMAPHORE_T));

    pt_bin_sem->ui4_value = ui4_init_value;

    pthread_condattr_init(&condattr);

#ifndef ANDROID
    pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
#endif

    if (((ret1=pthread_cond_init(&pt_bin_sem->cond, &condattr)) != 0) ||
        ((ret2=pthread_mutex_init(&pt_bin_sem->mutex, 0)) != 0))
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_create fail %d %d\n", ret1, ret2);
        free (pt_bin_sem);
        return OSR_DRV_FAIL;
    }

    *ppv_sema_id = (VOID *) pt_bin_sem;

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
    OS_BINARY_SEMAPHORE_T *pt_bin_sem = (OS_BINARY_SEMAPHORE_T *)(pv_sema_id);
    int ret1 = 0;
    int ret2 = 0;

    if (pt_bin_sem == NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_delete pt_bin_sem=NULL\n");
        return OSR_DRV_INV_ARG;
    }

    if (((ret1=pthread_mutex_destroy(&pt_bin_sem->mutex)) != 0) ||
        ((ret2=pthread_cond_destroy(&pt_bin_sem->cond)) != 0))
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_delete fail %d %d\n", ret1, ret2);
        free(pt_bin_sem);
        return OSR_DRV_FAIL;
    }

    free(pt_bin_sem);

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
    OS_BINARY_SEMAPHORE_T *pt_bin_sem = (OS_BINARY_SEMAPHORE_T *)(pv_sema_id);
    int ret;

    if (pt_bin_sem == NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock pt_bin_sem=NULL\n");
        return OSR_DRV_INV_ARG;
    }

    if ((ret=pthread_mutex_lock(&pt_bin_sem->mutex))!=0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_mutex_lock fail %d\n", ret);
        return OSR_DRV_FAIL;
    }

    while (!pt_bin_sem->ui4_value)
    {
        if (e_option == X_SEMA_OPTION_NOWAIT)
        {
            if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
            {
                fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_mutex_unlock fail a %d\n", ret);
                return OSR_DRV_FAIL;
            }

            return OSR_DRV_WOULD_BLOCK;
        }

        if ((ret=pthread_cond_wait(&pt_bin_sem->cond, &pt_bin_sem->mutex)) != 0)
        {
            fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_cond_wait fail %d\n", ret);
            if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
            {
                fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_mutex_unlock fail b %d\n", ret);
                return OSR_DRV_FAIL;
            }
            return OSR_DRV_FAIL;
        }
    }
    pt_bin_sem->ui4_value -= 1;
    
    if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_mutex_unlock fail c %d\n", ret);
        return OSR_DRV_FAIL;
    }

    return OSR_DRV_OK;
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
    OS_BINARY_SEMAPHORE_T *pt_bin_sem = (OS_BINARY_SEMAPHORE_T *)(pv_sema_id);
    struct timespec abstime;
    int ret;

    if (pt_bin_sem == NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout pt_bin_sem=NULL\n");
        return OSR_DRV_INV_ARG;
    }

    if (0 == ui4_time)
    {
        INT32 i4_ret = x_os_drv_sema_lock (pv_sema_id, X_SEMA_OPTION_NOWAIT);
        return i4_ret != OSR_DRV_WOULD_BLOCK ? i4_ret : OSR_DRV_TIMEOUT;
    }

    if ((ret=clock_gettime(CLOCK_MONOTONIC, &abstime)) != 0 )
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout clock_gettime fail %d\n", ret);
        return OSR_DRV_FAIL;
    }

    abstime.tv_sec  += ui4_time / 1000; ui4_time %= 1000;
    abstime.tv_nsec += ui4_time * 1000000;
    if (abstime.tv_nsec >= 1000000000)
    {
        abstime.tv_nsec -= 1000000000;
        abstime.tv_sec++;
    }

    if ((ret=pthread_mutex_lock(&pt_bin_sem->mutex))!=0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout pthread_mutex_lock fail %d\n", ret);
        return OSR_DRV_FAIL;
    }

    while (pt_bin_sem->ui4_value == 0)
    {
#ifndef ANDROID
        if ((ret=pthread_cond_timedwait(&pt_bin_sem->cond, &pt_bin_sem->mutex, &abstime)) != 0)
#else
        if ((ret=pthread_cond_timedwait_monotonic(&pt_bin_sem->cond, &pt_bin_sem->mutex, &abstime)) != 0)
#endif
        {
            if (ETIMEDOUT == ret)
            {
                if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
                {
                    fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout pthread_mutex_unlock fail a %d\n", ret);
                    return OSR_DRV_FAIL;
                }
                return OSR_DRV_TIMEOUT;
            }
            else if (EINTR == ret)
            {
                continue;
            }

            fprintf (stderr, "[OSAI] x_os_drv_sema_lock pthread_cond_timedwait fail %d\n", ret);
            if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
            {
                fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout pthread_mutex_unlock fail b %d\n", ret);
                return OSR_DRV_FAIL;
            }
            return OSR_DRV_FAIL;
        }
    }

    pt_bin_sem->ui4_value--;

    if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_lock_timeout pthread_mutex_unlock fail b %d\n", ret);
        return OSR_DRV_FAIL;
    }

    return OSR_DRV_OK;
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
    OS_BINARY_SEMAPHORE_T *pt_bin_sem = (OS_BINARY_SEMAPHORE_T *)(pv_sema_id);
    int ret;

    if (pt_bin_sem == NULL)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_unlock pt_bin_sem=NULL\n");
        return OSR_DRV_INV_ARG;
    }

    if ((ret=pthread_mutex_lock(&pt_bin_sem->mutex))!=0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_unlock pthread_mutex_lock fail %d\n", ret);
        return OSR_DRV_FAIL;
    }

    if (pt_bin_sem->ui4_value == 1)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_unlock already 1\n");
        pthread_mutex_unlock(&pt_bin_sem->mutex);
        return OSR_DRV_OK;;
    }
    pt_bin_sem->ui4_value += 1;

    if ((ret=pthread_cond_signal(&pt_bin_sem->cond)) != 0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_unlock pthread_cond_signal fail %d\n", ret);
        if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
        {
            fprintf (stderr, "[OSAI] x_os_drv_sema_unlock pthread_mutex_unlock fail a %d\n", ret);
            return OSR_DRV_FAIL;
        }
        return OSR_DRV_FAIL;
    }
    
    if ((ret=pthread_mutex_unlock(&pt_bin_sem->mutex)) != 0)
    {
        fprintf (stderr, "[OSAI] x_os_drv_sema_unlock pthread_mutex_unlock fail b %d\n", ret);
        return OSR_DRV_FAIL;
    }

    return OSR_DRV_OK;;
}
