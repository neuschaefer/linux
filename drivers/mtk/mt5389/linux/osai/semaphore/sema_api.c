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
 * $RCSfile: sema_api.c,v $
 * $Revision: #2 $
 * $Date: 2012/11/27 $
 * $Author: yingjoe.chen $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


/* Middleware related header files */
#include "x_linux.h"
#include "x_hal_arm.h"
#include "os.h"
#include "sema_lib.h"
#include "../thread/thread_lib.h"
#include "../timer/timer_lib.h"
#include "../msgQ/msgq_lib.h"

#ifdef CLI_SUPPORT
static struct _os_cli_sema_q    os_cli_sema_q[3] =
{
    {NULL,  NULL_HANDLE,    0},
    {NULL,  NULL_HANDLE,    0},
    {NULL,  NULL_HANDLE,    0}
};
#endif

static BOOL b_sema_inited = TRUE;



/*----------------------------------------------------------------------------
 * Function: x_sema_create()
 *
 * Description:
 *      this API creates a semaphore and its handle. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for sema struct.
 *      4. call OS driver semaphore create and returns a driver sema id.
 *      5. allocate a handle for semaphore.
 *
 * Inputs:
 *      e_type: either X_SEMA_TYPE_BINARY or X_SEMA_TYPE_MUTEX.
 *      ui4_init_value: inital value of this semaphore.
 *
 * Outputs:
 *      *ph_sema_hdl: pointer to hold returned semaphore handle.
 *                   the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
x_sema_create(HANDLE_T      *ph_sema_hdl,        /* semaphore handle */
              SEMA_TYPE_T   e_type,
              UINT32        ui4_init_value)
{
    OS_SEMID pt_sema;           /* semaphore object */
    OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
    HANDLE_TYPE_T  e_hdl_type;
    INT32 i4_ret;
#ifdef CLI_SUPPORT
    struct _os_cli_sema_q *pt_cli;
#endif

    if (b_sema_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* check arguments */
    if ((ph_sema_hdl == NULL) ||
        ((e_type != X_SEMA_TYPE_BINARY) && (e_type != X_SEMA_TYPE_MUTEX) &&
         (e_type != X_SEMA_TYPE_COUNTING)))
    {
        return(OSR_INV_ARG);
    }

    if (((e_type == X_SEMA_TYPE_BINARY) || (e_type == X_SEMA_TYPE_MUTEX)) &&
         (ui4_init_value != X_SEMA_STATE_LOCK) &&
         (ui4_init_value != X_SEMA_STATE_UNLOCK))
    {
        return(OSR_INV_ARG);
    }

    /* create semaphore object */
    if (e_type == X_SEMA_TYPE_MUTEX)
    {
#ifdef CLI_SUPPORT
        pt_cli = &(os_cli_sema_q[1]);
#endif
        pt_sema = (OS_SEMID) x_mem_alloc(sizeof(OS_MUTEX_SEMA_T));
        e_hdl_type = HT_GROUP_OS_MUTEX_SEMA;    
    }
    else if (e_type == X_SEMA_TYPE_BINARY)
    {
#ifdef CLI_SUPPORT
        pt_cli = &(os_cli_sema_q[0]);
#endif
        pt_sema = (OS_SEMID) x_mem_alloc(sizeof(OS_SEMA_T));
        e_hdl_type = HT_GROUP_OS_BIN_SEMA;    
    }
    else
    {
#ifdef CLI_SUPPORT
        pt_cli = &(os_cli_sema_q[2]);
#endif
        pt_sema = (OS_SEMID) x_mem_alloc(sizeof(OS_COUNT_SEMA_T));
        e_hdl_type = HT_GROUP_OS_COUNTING_SEMA;    
    }

    if (pt_sema == NULL)
    {
        return(OSR_NO_RESOURCE);
    }

    pt_sema->pv_sema_id = (VOID *) NULL;    /* sema_id returned from driver */
    pt_sema->ui1_flag = (UINT8) 0;

    if (e_type == X_SEMA_TYPE_MUTEX)
    {
        pt_sema->i2_selfcount = 0;      /* # of ref by the 1st thread (mutex) */
        pt_sema->h_th_hdl = NULL_HANDLE;   /* owner of the semaphore */
    }
    else if (e_type == X_SEMA_TYPE_COUNTING)
    {
        pt_sem = (OS_COUNT_SEMA_T *) pt_sema;
        pt_sem->i4_selfcount = ui4_init_value;
        if (ui4_init_value != X_SEMA_STATE_LOCK)
        {
            ui4_init_value = X_SEMA_STATE_UNLOCK;
        }
    }

    /* allocate a handle */
    i4_ret = handle_alloc(e_hdl_type, 
                          pt_sema,
                          pt_sema,
                          sema_free_cb,
                          ph_sema_hdl);
    if (i4_ret != HR_OK)
    {
        x_mem_free(pt_sema);
        return(HR_TO_OSR(i4_ret));
    }

    i4_ret = x_os_drv_sema_create(&pt_sema->pv_sema_id, ui4_init_value);
    if (i4_ret != OSR_DRV_OK)
    {
        pt_sema->ui1_flag |= FLAG_SEMA_CLOSE;
        handle_free(*ph_sema_hdl, TRUE);
        return(OSR_FAIL);
    }

#ifdef CLI_SUPPORT
    pt_sema->pv_thread = NULL;
    pt_sema->pt_prev = NULL;
    OS_CLI_SEMA_ENQ(pt_cli, pt_sema);
#endif

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_sema_delete()
 *
 * Description:
 *      this API deletes a semaphore and its handle. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. call OS driver semaphore delete.
 *      4. free sema handle.
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via x_sema_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_sema_delete(HANDLE_T h_sema_hdl)          /* semaphore handle */
{
    OS_SEMID pt_sema;     /* semaphore object */
    HANDLE_TYPE_T e_type;
    INT32 i4_ret;
#ifdef CLI_SUPPORT
    struct _os_cli_sema_q *pt_cli;
#endif

    if (b_sema_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* get the handle type */
    i4_ret = x_handle_get_type(h_sema_hdl, &e_type);
    if (i4_ret != HR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    if ((e_type != HT_GROUP_OS_BIN_SEMA) &&
        (e_type != HT_GROUP_OS_MUTEX_SEMA) &&
        (e_type != HT_GROUP_OS_COUNTING_SEMA))
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = handle_get_obj(h_sema_hdl, (VOID *) &pt_sema);
    if ((i4_ret != HR_OK) || (pt_sema == NULL))
    {
        return(OSR_INV_HANDLE);
    }

    /* call driver delete function */
    i4_ret = x_os_drv_sema_delete(pt_sema->pv_sema_id);
    if (i4_ret != OSR_DRV_OK)
    {
        return(OSR_FAIL);
    }

#ifdef CLI_SUPPORT
    if (e_type == HT_GROUP_OS_BIN_SEMA)
    {
        pt_cli = &(os_cli_sema_q[0]);
    }
    else if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        pt_cli = &(os_cli_sema_q[1]);
    }
    else
    {
        pt_cli = &(os_cli_sema_q[2]);
    }
    OS_CLI_SEMA_DEQ(pt_cli, pt_sema);
#endif

    /* mark to indicate from x_sema_delete */
    /* to distinguish from direct call of handle_free */
    pt_sema->ui1_flag |= FLAG_SEMA_CLOSE;
    i4_ret = handle_free(h_sema_hdl, TRUE);

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_sema_lock()
 *
 * Description:
 *      this API locks (acquires) a sema. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. get handle type and check its validity.
 *      4. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema.
 *      5. call OS driver semaphore lock.
 *      6. if sema is of type MUTEX, register as the owner of sema if no
 *         ownership is established.
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via x_sema_create().
 *      e_option: either X_SEMA_OPTION_WAIT or X_SEMA_OPTION_NOWAIT.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_WOULD_BLOCK: if eoption is X_SEMA_OPTION_NOWAIT and the calling
 *                       thread would have been blocked to lock the sema.
 *---------------------------------------------------------------------------*/
INT32
x_sema_lock(HANDLE_T h_sema_hdl,        /* semaphore handle */
            SEMA_OPTION_T e_option)
{
    OS_SEMID pt_sema;     /* semaphore object */
    HANDLE_T h_th_hdl = NULL_HANDLE;
    HANDLE_TYPE_T e_type;
    INT32 i4_ret;
#ifdef CLI_SUPPORT
    VOID *pv_th_id;
#endif

    if (b_sema_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    if ((e_option != X_SEMA_OPTION_WAIT) && (e_option != X_SEMA_OPTION_NOWAIT))
    {
        return(OSR_INV_ARG);
    }

    /* get the handle type */
    i4_ret = x_handle_get_type(h_sema_hdl, &e_type);
    if (i4_ret != HR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    if ((e_type != HT_GROUP_OS_BIN_SEMA) &&
        (e_type != HT_GROUP_OS_MUTEX_SEMA) &&
        (e_type != HT_GROUP_OS_COUNTING_SEMA))
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = handle_get_obj(h_sema_hdl, (VOID *) &pt_sema);
    if ((i4_ret != HR_OK) || (pt_sema == NULL))
    {
        return(OSR_INV_HANDLE);
    }

    /*
     * mutex semaphore allows the same thread to acquire the semaphore
     * multiple times and release the semaphore with the same numbers
     */
    if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        i4_ret = x_thread_self(&h_th_hdl);      /* the calling thread */
        if (i4_ret != OSR_OK)
        {
            h_th_hdl = SEMA_RANDOM_HANDLE;     /* random #, pre-history */
        }

        /* check if this thread is the owner of the semaphore */
        if (/* (pt_sema->h_th_hdl != NULL_HANDLE) && */ 
            (pt_sema->h_th_hdl == h_th_hdl))
        {
            pt_sema->i2_selfcount++;
            return(OSR_OK);
        }
    }
    else if (e_type == HT_GROUP_OS_COUNTING_SEMA)
    {
        OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
        CRIT_STATE_T t_crit;

        pt_sem = (OS_COUNT_SEMA_T *) pt_sema;
        t_crit = x_crit_start();
        pt_sem->i4_selfcount--;
        if (pt_sem->i4_selfcount > (int)X_SEMA_STATE_LOCK)
        {
            x_crit_end(t_crit);
            return(OSR_OK);
        }
        x_crit_end(t_crit);
    }

    i4_ret = x_os_drv_sema_lock(pt_sema->pv_sema_id, e_option);
    if (i4_ret != OSR_DRV_OK)
    {
        if (e_type == HT_GROUP_OS_COUNTING_SEMA)
        {
            OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
            CRIT_STATE_T t_crit;

            pt_sem = (OS_COUNT_SEMA_T *) pt_sema;

            t_crit = x_crit_start();
            pt_sem->i4_selfcount++;
            x_crit_end(t_crit);
        }

        if (i4_ret == OSR_DRV_WOULD_BLOCK)
        {
            return(OSR_WOULD_BLOCK);
        }
        else
        {
            return(OSR_FAIL);
        }
    }

    if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        /* now, only the 1st came through, others are blocked */
        if (pt_sema->h_th_hdl == NULL_HANDLE)
        {
            pt_sema->h_th_hdl = h_th_hdl;   /* mark the 1st thread the owner */
            pt_sema->i2_selfcount = 1;
        }
        else
        {
            /* this should not happen since other threads should have been */
            /* blocked til the owner of the semaphore unlocks the semaphore */

            i4_ret = x_os_drv_sema_lock(pt_sema->pv_sema_id, e_option);
            if (i4_ret != OSR_DRV_OK)
            {
                return(OSR_FAIL);
            }
        }
    }

#ifdef CLI_SUPPORT
    if (x_os_drv_thread_self(&pv_th_id, ((VOID**) &(pt_sema->pv_thread))) != OSR_OK)
    {
        pt_sema->pv_thread = NULL;
    }
#endif

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_sema_lock_timeout()
 *
 * Description:
 *      this API locks (acquires) a sema. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. get handle type and check its validity.
 *      3. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema.
 *      4. call OS driver semaphore lock timeout API.
 *      5. if sema is of type MUTEX, register as the owner of sema if no
 *         ownership is established.
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via x_sema_create().
 *      ui4_time: time duration in ms before thie API bails out.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_TIMEOUT: failed to lock the semaphore within "ui4_time" duration.
 *---------------------------------------------------------------------------*/
INT32
x_sema_lock_timeout(HANDLE_T h_sema_hdl,        /* semaphore handle */
                    UINT32   ui4_time)
{
    OS_SEMID pt_sema;     /* semaphore object */
    HANDLE_T h_th_hdl = NULL_HANDLE;
    HANDLE_TYPE_T e_type;
    INT32 i4_ret;
#ifdef CLI_SUPPORT
    VOID *pv_th_id;
#endif

    if (b_sema_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* get the handle type */
    i4_ret = x_handle_get_type(h_sema_hdl, &e_type);
    if (i4_ret != HR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    if ((e_type != HT_GROUP_OS_BIN_SEMA) &&
        (e_type != HT_GROUP_OS_MUTEX_SEMA) &&
        (e_type != HT_GROUP_OS_COUNTING_SEMA))
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = handle_get_obj(h_sema_hdl, (VOID *) &pt_sema);
    if ((i4_ret != HR_OK) || (pt_sema == NULL))
    {
        return(OSR_INV_HANDLE);
    }

    /*
     * mutex semaphore allows the same thread to acquire the semaphore
     * multiple times and release the semaphore with the same numbers
     */
    if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        i4_ret = x_thread_self(&h_th_hdl);      /* the calling thread */
        if (i4_ret != OSR_OK)
        {
            h_th_hdl = SEMA_RANDOM_HANDLE;     /* random #, pre-history */
        }

        /* check if this thread is the owner of the semaphore */
        if (/* (pt_sema->h_th_hdl != NULL_HANDLE) && */ 
            (pt_sema->h_th_hdl == h_th_hdl))
        {
            pt_sema->i2_selfcount++;
            return(OSR_OK);
        }
    }
    else if (e_type == HT_GROUP_OS_COUNTING_SEMA)
    {
        OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
        CRIT_STATE_T t_crit;

        pt_sem = (OS_COUNT_SEMA_T *) pt_sema;
        t_crit = x_crit_start();
        pt_sem->i4_selfcount--;
        if (pt_sem->i4_selfcount > (int)X_SEMA_STATE_LOCK)
        {
            x_crit_end(t_crit);
            return(OSR_OK);
        }
        x_crit_end(t_crit);
    }

    i4_ret = x_os_drv_sema_lock_timeout(pt_sema->pv_sema_id, ui4_time);
    if (i4_ret != OSR_DRV_OK)
    {
        if (e_type == HT_GROUP_OS_COUNTING_SEMA)
        {
            OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
            CRIT_STATE_T t_crit;

            pt_sem = (OS_COUNT_SEMA_T *) pt_sema;

            t_crit = x_crit_start();
            pt_sem->i4_selfcount++;
            x_crit_end(t_crit);
        }

        if (i4_ret == OSR_DRV_TIMEOUT)
        {
            return(OSR_TIMEOUT);
        }
        else
        {
            return(OSR_FAIL);
        }
    }

    if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        /* now, only the 1st came through, others are blocked */
        if (pt_sema->h_th_hdl == NULL_HANDLE)
        {
            pt_sema->h_th_hdl = h_th_hdl;   /* mark the 1st thread the owner */
            pt_sema->i2_selfcount = 1;
        }
        else
        {
            /* this should not happen since other threads should have been */
            /* blocked til the owner of the semaphore unlocks the semaphore */

            i4_ret = x_os_drv_sema_lock_timeout(pt_sema->pv_sema_id, ui4_time);
            if (i4_ret != OSR_DRV_OK)
            {
                if (i4_ret == OSR_DRV_TIMEOUT)
                {
                    return(OSR_TIMEOUT);
                }
                else
                {
                    return(OSR_FAIL);
                }
            }
        }
    }

#ifdef CLI_SUPPORT
    if (x_os_drv_thread_self(&pv_th_id, ((VOID**) &(pt_sema->pv_thread))) != OSR_OK)
    {
        pt_sema->pv_thread = NULL;
    }
#endif

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_sema_unlock()
 *
 * Description:
 *      this API unlocks (releases) a semaphore. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. get handle type and check its validity.
 *      3. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema and decrement refcount if so and return.
 *      5. call OS driver semaphore unlock if refcount reaches 0 on MUTEX sema.
 *      6. if sema is of type MUTEX, de-register as thalling thread as owner of
 *         sema.
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via x_sema_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_sema_unlock(HANDLE_T h_sema_hdl)          /* semaphore handle */
{
    OS_SEMID pt_sema;       /* semaphore object */
    HANDLE_T h_th_hdl;      /* thread handle */
    HANDLE_TYPE_T e_type;
    INT32 i4_ret;

    if (b_sema_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* get the handle type */
    i4_ret = x_handle_get_type(h_sema_hdl, &e_type);
    if (i4_ret != HR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    if ((e_type != HT_GROUP_OS_BIN_SEMA) &&
        (e_type != HT_GROUP_OS_MUTEX_SEMA) &&
        (e_type != HT_GROUP_OS_COUNTING_SEMA))
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = handle_get_obj(h_sema_hdl, (VOID *) &pt_sema);
    if ((i4_ret != HR_OK) || (pt_sema == NULL))
    {
        return(OSR_INV_HANDLE);
    }

    /*
     * mutex semaphore allows the same thread to acquire the semaphore
     * multiple times and unlock the semaphore with the same numbers
     */
    if (e_type == HT_GROUP_OS_MUTEX_SEMA)
    {
        i4_ret = x_thread_self(&h_th_hdl);
        if (i4_ret != OSR_OK)
        {
            h_th_hdl = SEMA_RANDOM_HANDLE;
        }

        if ((pt_sema->h_th_hdl == NULL_HANDLE) ||
            (pt_sema->h_th_hdl == h_th_hdl))
        {
            if (pt_sema->i2_selfcount > 1)
            {
                pt_sema->i2_selfcount--;
                return(OSR_OK);
            }
            else
            {
                pt_sema->h_th_hdl = NULL_HANDLE;
                pt_sema->i2_selfcount = 0;
            }
        }
        else
        {
            /* the thread to unlock the semaphore is not the owner */
            /* something wrong, cannot unlock the semaphore */
            return(OSR_FAIL);
        }
    }
    else if (e_type == HT_GROUP_OS_COUNTING_SEMA)
    {
        OS_COUNT_SEMA_T *pt_sem;    /* counting semaphore object */
        CRIT_STATE_T t_crit;

        pt_sem = (OS_COUNT_SEMA_T *) pt_sema;
        t_crit = x_crit_start();
        pt_sem->i4_selfcount++;
        x_crit_end(t_crit);
    }

#ifdef CLI_SUPPORT
    pt_sema->pv_thread = NULL;
#endif

    i4_ret = x_os_drv_sema_unlock(pt_sema->pv_sema_id);
    if (i4_ret != OSR_DRV_OK)
    {
        return(OSR_INV_ARG);
    }

    return(OSR_OK);
}


BOOL
sema_free_cb(HANDLE_T       h_hdl,
             HANDLE_TYPE_T  e_type,
             VOID *         pv_obj,
             VOID *         pv_tag,
             BOOL           b_req_hdl)
{
    OS_SEMID pt_sema;    /* semaphore object */

    if ((e_type != HT_GROUP_OS_BIN_SEMA) &&
        (e_type != HT_GROUP_OS_MUTEX_SEMA) &&
        (e_type != HT_GROUP_OS_COUNTING_SEMA))
    {
        return(FALSE);
    }

    pt_sema = (OS_SEMID) pv_obj;
    if (pt_sema->ui1_flag & FLAG_SEMA_CLOSE)
    {
        x_mem_free(pt_sema);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}


#ifdef CLI_SUPPORT
INT32
os_cli_show_sema_all(INT32        i4_argc,
                     const CHAR   **pps_argv)
{
    OS_SEMID pt_sema;
    struct _os_cli_sema_q *pt_cli;
    INT32 i4_i;

    
    pt_cli = &(os_cli_sema_q[0]);
    if (pt_cli->i4_count != 0)
    {
        x_dbg_stmt("%d Binary semaphores\n", pt_cli->i4_count);
        i4_i = 1;
        (VOID) x_sema_lock(pt_cli->h_qhdl, X_SEMA_OPTION_WAIT);
        pt_sema = pt_cli->pt_head;
        while (pt_sema != NULL)
        {
            if (pt_sema->pv_thread != NULL)
            {
                x_dbg_stmt("[%d] Binary sema id 0x%x, LOCKED BY %s\n",
                        i4_i, pt_sema->pv_sema_id,
                        (((OS_THREAD_T *) (pt_sema->pv_thread))->s_name));
            }
            else
            {
                x_dbg_stmt("[%d] Binary sema id 0x%x, NOT locked\n",
                        i4_i, pt_sema->pv_sema_id);
            }

            pt_sema = (OS_SEMID) (pt_sema->pt_next);
            ++i4_i;
        }
        (VOID) x_sema_unlock(pt_cli->h_qhdl);
        x_dbg_stmt("\n");
    }
    
    pt_cli = &(os_cli_sema_q[1]);
    if (pt_cli->i4_count != 0)
    {
        x_dbg_stmt("%d Mutex semaphores\n", pt_cli->i4_count);
        i4_i = 1;
        (VOID) x_sema_lock(pt_cli->h_qhdl, X_SEMA_OPTION_WAIT);
        pt_sema = pt_cli->pt_head;
        while (pt_sema != NULL)
        {
            if (pt_sema->pv_thread != NULL)
            {
                x_dbg_stmt("[%d] Mutex sema id 0x%x, LOCKED %d times BY %s\n",
                            i4_i, pt_sema->pv_sema_id, pt_sema->i2_selfcount,
                            (((OS_THREAD_T *) (pt_sema->pv_thread))->s_name));
            }
            else
            {
                x_dbg_stmt("[%d] Mutex sema id 0x%x, NOT locked\n",
                            i4_i, pt_sema->pv_sema_id);
            }

            pt_sema = (OS_SEMID) (pt_sema->pt_next);
            ++i4_i;
        }
        (VOID) x_sema_unlock(pt_cli->h_qhdl);
        x_dbg_stmt("\n");
    }
    
    pt_cli = &(os_cli_sema_q[2]);
    if (pt_cli->i4_count != 0)
    {
        OS_COUNT_SEMA_T *pt_sem;

        x_dbg_stmt("%d Counting semaphores\n", pt_cli->i4_count);
        i4_i = 1;
        (VOID) x_sema_lock(pt_cli->h_qhdl, X_SEMA_OPTION_WAIT);
        pt_sem = (OS_COUNT_SEMA_T *) pt_cli->pt_head;
        while (pt_sem != NULL)
        {
            if (pt_sem->pv_thread != NULL)
            {
                x_dbg_stmt("[%d] Counting sema id 0x%x, LOCKED by %s\n",
                            i4_i, pt_sem->pv_sema_id,
                            (((OS_THREAD_T *) (pt_sem->pv_thread))->s_name));
            }
            else
            {
                x_dbg_stmt("[%d] Counting sema id 0x%x, NOT locked, count %d \n",
                            i4_i, pt_sem->pv_sema_id, pt_sem->ui4_selfcount);
            }

            pt_sem = (OS_COUNT_SEMA_T *) (pt_sem->pt_next);
            ++i4_i;
        }
        (VOID) x_sema_unlock(pt_cli->h_qhdl);
    }

    return(OSR_OK);
}
#endif
