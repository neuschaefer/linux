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
 * $RCSfile: thread_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "inc/x_pvt_key.h"
#include "os/inc/os.h"
#include "os/memory/mem_lib.h"
#include "os/semaphore/sema_lib.h"
#include "os/thread/thread_lib.h"
#include "os/timer/timer_lib.h"
#include "os/msgQ/msgq_lib.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define PVT_KEY_FLAG_MASK_UNUSED  ((UINT32) 0x3f000000)

#ifdef SYS_MEM_CHK
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#undef x_mem_free
#undef x_thread_create
#undef x_thread_exit 

extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);

extern INT32 x_thread_create(HANDLE_T    *ph_th_hdl,
                             const CHAR  *ps_name,
                             SIZE_T       z_stacksize,
                             UINT8        ui1_pri,
                             x_os_thread_main_fct pf_main,
                             SIZE_T       z_arg_size,
                             VOID         *pv_arg);
                             
extern VOID x_thread_exit(VOID);             
#endif

static INT32 thread_resume_x(HANDLE_T h_th_hdl, BOOL b_ret_state);


/*----------------------------------------------------------------------------
 * Function: x_thread_create()
 *
 * Description:
 *      this API creates a thread. it performs
 *      1. check if thread module is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for thread struct and for the specified argument.
 *      4. check existence of the thread (based on msg Q name).
 *      5. initialize, allocate, and link a messaage haandle.
 *      6. call OS driver thread create routine passing the middleware
 *         generic main function "thread_main".
 *
 * Inputs:
 *      ps_name: name of this thread, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *      z_stacksize: stack size this thread will run on.
 *      ui1_pri: priority of this thread, 1- highest, 255 - lowest.
 *      pf_main: main entry of this thread.
 *      z_arg_size: size in byte of argument of pf_main.
 *      pv_arg: argument of pf_main.
 *
 * Outputs:
 *      *ph_th_hdl: pointer to a handle to be returned by this API.
 *                  the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_EXIST: thread exists (ps_name collision).
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
x_thread_create(HANDLE_T    *ph_th_hdl,         /* thread handle */
                const CHAR  *ps_name,
                SIZE_T       z_stacksize,
                UINT8        ui1_pri,           /* thread priority */
                x_os_thread_main_fct pf_main,   /* thread's main function */
                SIZE_T       z_arg_size,
                VOID         *pv_arg)
{
    OS_THREAD_T *pt_thread;         /* thread object */
    VOID *pv_arg_local = NULL;      /* local copy of argument */
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_th_hdl == NULL) ||
        (z_stacksize == 0) || (pf_main == NULL) ||
        (ui1_pri < THREAD_PRI_RANGE_LOW) || (ui1_pri > THREAD_PRI_RANGE_HIGH) ||
        ((pv_arg != NULL) && (z_arg_size == 0)))
    {
        return(OSR_INV_ARG);
    }

    /* check for naming collision */
    if (thread_get_th_obj_by_name(ps_name) == TRUE)
    {
        return(OSR_EXIST);
    }

    if ((pv_arg != NULL) && (z_arg_size != 0))  /* make a local copy of arg */
    {
        pv_arg_local = x_mem_alloc(z_arg_size);
        if (pv_arg_local != NULL)
        {
            x_memcpy(pv_arg_local, pv_arg, z_arg_size);
        }
        else
        {
            return(OSR_NO_RESOURCE);
        }
    }

    /* create thread object */
    pt_thread = (OS_THREAD_T *) x_mem_alloc(sizeof(OS_THREAD_T));
    if (pt_thread == NULL)
    {
        x_mem_free(pv_arg_local);
        return(OSR_NO_RESOURCE);
    }

    pt_thread->pt_ev_grp_waiting = NULL;
    pt_thread->e_ev_grp_wait_op = X_EV_OP_CLEAR;
    pt_thread->e_ev_grp_event_wait = (EV_GRP_EVENT_T) 0;
    pt_thread->e_ev_grp_event_got = (EV_GRP_EVENT_T) 0;

    pt_thread->pf_main = pf_main;
    pt_thread->pv_arg = pv_arg_local;       /* TODO: free upon exit */
    pt_thread->z_stacksize = z_stacksize;
    pt_thread->ui1_priority = ui1_pri;
    pt_thread->ui1_flag = (UINT8) 0;
    pt_thread->pt_pvt_head = NULL;
    x_strncpy(pt_thread->s_name, ps_name, THREAD_NAME_LEN);
    pt_thread->s_name[THREAD_NAME_LEN] = '\0';

    /* store this thread for later look up */
    if (thread_put_th_obj(pt_thread) != OSR_OK)
    {
        x_mem_free(pv_arg_local);
        x_mem_free(pt_thread);
        return(OSR_EXIST);
    }

    i4_ret = handle_alloc(HT_GROUP_OS_THREAD,
                          pt_thread,
                          pt_thread,
                          thread_free_cb,
                          ph_th_hdl);
    if (i4_ret != HR_OK)
    {
        x_mem_free(pv_arg_local);
        x_mem_free(pt_thread);
        return(HR_TO_OSR(i4_ret));
    }
    pt_thread->h_th_hdl = *ph_th_hdl;

    /* create a semaphore for msgQ send/receive */
    pt_thread->h_sema_hdl = (HANDLE_T) NULL_HANDLE;
    i4_ret = x_sema_create(&(pt_thread->h_sema_hdl),
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        pt_thread->ui1_flag |= FLAG_TH_CLOSE;
        handle_free(*ph_th_hdl, TRUE);
        return(i4_ret);
    }

    pt_thread->h_sema_evg = (HANDLE_T) NULL_HANDLE;
    i4_ret = x_sema_create(&(pt_thread->h_sema_evg),
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        (VOID) x_sema_delete(pt_thread->h_sema_hdl);
        pt_thread->ui1_flag |= FLAG_TH_CLOSE;
        handle_free(*ph_th_hdl, TRUE);
        return(i4_ret);
    }

    i4_ret = x_os_drv_thread_create(&pt_thread->pv_th_id,
                                    pt_thread->s_name,
                                    z_stacksize,
                                    ui1_pri,
                                    0,
                                    ((VOID*) pt_thread),
                                    (x_os_drv_thread_main_fct) thread_main);

    if (i4_ret != OSR_DRV_OK)
    {
        /* mark to indicate from x_sema_exit */
        /* to distinguish from direct call of handle_free */
        pt_thread->ui1_flag |= FLAG_TH_CLOSE;
        handle_free(*ph_th_hdl, TRUE);
        return(OSR_FAIL);
    }

    /* unlock semaphore so that the created thread continues */
    i4_ret = x_sema_unlock(pt_thread->h_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        pt_thread->ui1_flag |= FLAG_TH_CLOSE;   /* info created thread */
        return(i4_ret);
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_exit()
 *
 * Description:
 *      this API exits a thread. it performs
 *      1. check if thread mudule is initiated. 
 *      2. get thread handle, thread struct, and call handle_free().
 *      3. call OS driver thread exit routine.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_thread_exit(VOID)
{
    HANDLE_T h_th_hdl;          /* thread handle */
    OS_THREAD_T *pt_thread;     /* thread object */
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NOT_INIT);
    }

    i4_ret = x_thread_self(&h_th_hdl);      /* get self handle */
    if ((i4_ret != OSR_OK) || (h_th_hdl == NULL_HANDLE))
    {
        OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NO_HANDLE);
    }

    /* get the handle object */
    i4_ret = handle_get_obj(h_th_hdl, (VOID **) &pt_thread);
    if ((i4_ret != HR_OK) || (pt_thread == NULL))
    {
        OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NO_OBJECT);
    }

    /* Free any allocated private data, associated with the thread. */
    while (pt_thread->pt_pvt_head != NULL)
    {
        thread_del_pvt_key (pt_thread, pt_thread->pt_pvt_head);
    }

    /* mark to indicate from x_sema_exit */
    /* to distinguish from direct call of handle_free */
    pt_thread->ui1_flag |= FLAG_TH_CLOSE;
    (VOID) handle_free(h_th_hdl, TRUE);

    /*
     * calling driver exit routine may result in actual exit (termination)
     * of the thread (never return)
     */

    x_os_drv_thread_exit();
    OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NOT_EXIT);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_delay()
 *
 * Description:
 *      this API allows a thread to give up CPU for a duration. it performs
 *      1. check if thread mudule is initiated. 
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

    x_os_drv_thread_delay(ui4_delay);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_get_pri()
 *
 * Description:
 *      this API gets a thread's priority. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. check validity of thread handle.
 *      4. call OS driver thread get priority routine.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *
 * Outputs:
 *      *pui1_pri: pointer to hold returned thread priority.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_thread_get_pri(HANDLE_T   h_th_hdl,
                 UINT8      *pui1_pri)
{
    OS_THREAD_T *pt_thread;    /* thread object */
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    if (pui1_pri == NULL)
    {
        return(OSR_INV_ARG);
    }

    /* check if h_th_hdl has the right type and get its object */
    i4_ret = os_check_handle(h_th_hdl, HT_GROUP_OS_THREAD, (VOID **) &pt_thread);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = x_os_drv_thread_get_pri(pt_thread->pv_th_id, pui1_pri);
    if (i4_ret != OSR_DRV_OK)
    {
        return(OSR_FAIL);
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_set_pri()
 *
 * Description:
 *      this API sets a thread's priority. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. check validity of thread handle.
 *      4. call OS driver thread set priority routine.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *      ui1_pri: new thread priority, 1-highest, 255-lowest.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_thread_set_pri(HANDLE_T   h_th_hdl,   /* thread handle */
                 UINT8      ui1_pri)
{
    OS_THREAD_T *pt_thread = NULL;    /* thread object */
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    if ((ui1_pri < THREAD_PRI_RANGE_LOW) || (ui1_pri > THREAD_PRI_RANGE_HIGH))
    {
        return(OSR_INV_ARG);
    }

    /* check if h_th_hdl has the right type and get its object */
    i4_ret = os_check_handle(h_th_hdl, HT_GROUP_OS_THREAD, (VOID **) &pt_thread);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = x_os_drv_thread_set_pri(pt_thread->pv_th_id, ui1_pri);
    if (i4_ret != OSR_DRV_OK)
    {
        return(OSR_FAIL);
    }

    pt_thread->ui1_priority = ui1_pri;

    return(OSR_OK);
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
    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_suspend()
 *
 * Description:
 *      this API suspends a thread. it performs
 *      1. check if thread mudule is initiated. 
 *      2. call OS driver thread suspend routine.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_thread_suspend(VOID)
{

    if (b_thread_inited != TRUE)
    {
        return;
    }

    x_os_drv_thread_suspend();
}


/*----------------------------------------------------------------------------
 * Function: x_thread_resume()
 *
 * Description:
 *      this API resumes a thread. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. check validity of thread handle.
 *      4. call OS driver thread resume routine.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_thread_resume(HANDLE_T h_th_hdl)
{
    return(thread_resume_x(h_th_hdl, FALSE));
}


/*----------------------------------------------------------------------------
 * Function: x_thread_resume_with_state()
 *
 * Description:
 *      this API resumes a thread. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. check validity of thread handle.
 *      4. call OS driver thread resume routine.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_THREAD_ACTIVE : routine was successful. thread is active
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_thread_resume_with_state(HANDLE_T h_th_hdl)
{
    return(thread_resume_x(h_th_hdl, TRUE));
}


static INT32
thread_resume_x(HANDLE_T h_th_hdl, BOOL b_ret_state)
{
    OS_THREAD_T *pt_thread;    /* thread object */
    INT32 i4_ret;


    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* check if h_th_hdl has the right type and get its object */
    i4_ret = os_check_handle(h_th_hdl, HT_GROUP_OS_THREAD, (VOID **) &pt_thread);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    i4_ret = x_os_drv_thread_resume(pt_thread->pv_th_id);

    if (i4_ret == OSR_DRV_OK)
    {
        return(OSR_OK);
    }
    else if (i4_ret == OSR_DRV_THREAD_ACTIVE)
    {
        return((b_ret_state != TRUE) ? OSR_OK : OSR_THREAD_ACTIVE);
    }
    else
    {
        return(OSR_FAIL);
    }
}


/*----------------------------------------------------------------------------
 * Function: x_thread_self()
 *
 * Description:
 *      this API gets calling thread's handle. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. call OS driver thread self routine to get driver's thread id.
 *      4. look for thread struct via thread id.
 *      5. check validity of thread handle in thread struct..
 *
 * Inputs: -
 *
 * Outputs:
 *      *ph_th_hdl: pointer to hold returned thread handle.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_NOT_EXIST: thread not exist.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_thread_self(HANDLE_T *ph_th_hdl)
{
    OS_THREAD_T *pt_thread;     /* thread object */
    VOID *pv_th_id;             /* thread id of underlying OS */
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    if (ph_th_hdl == NULL)
    {
        return(OSR_INV_ARG);
    }

    i4_ret  = x_os_drv_thread_self(&pv_th_id, ((VOID**) &pt_thread));

    if (pt_thread == NULL)
    {
        return(OSR_NOT_EXIST);
    }

    i4_ret = os_check_handle(pt_thread->h_th_hdl,
                             HT_GROUP_OS_THREAD,
                             (VOID **) NULL);

    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    *ph_th_hdl = pt_thread->h_th_hdl;

    return(OSR_OK);
}

/*----------------------------------------------------------------------------
 * Function: x_thread_stack_stats()
 *
 * Description:
 *      This API gets a thread's alloacted stack size as well as the used size.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *
 * Outputs:
 *      pz_alloc_stack     Contains the allocated stack size.
 *      pz_max_used_stack  Contains the maximal used stack size.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_thread_stack_stats (HANDLE_T  h_th_hdl,
                            SIZE_T*   pz_alloc_stack,
                            SIZE_T*   pz_max_used_stack)
{
    OS_THREAD_T* pt_thread;


    if (b_thread_inited != TRUE)
    {
        return (OSR_NOT_INIT);
    }

    if ((pz_alloc_stack    == NULL)  ||
        (pz_max_used_stack == NULL))
    {
        return (OSR_INV_ARG);
    }

    /* check if h_th_hdl has the right type and get its object */
    if (os_check_handle (h_th_hdl, HT_GROUP_OS_THREAD, ((VOID**) &pt_thread)) != OSR_OK)
    {
        return (OSR_INV_HANDLE);
    }

    if (x_os_drv_thread_stack_stats (pt_thread->pv_th_id,
                                     pz_alloc_stack,
                                     pz_max_used_stack) != OSR_DRV_OK)
    {
        return (OSR_FAIL);
    }

    return (OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_set_pvt()
 *
 * Description:
 *      This API sets some private data, associated with a thread and key.
 *      Such private data can again be extracted using API
 *      "x_thread_get_pvt ()".
 *
 * Inputs:
 *      ui4_key     Contains the key associated with the private data.
 *      pf_pvt_del  References a callback function which is called when the
 *                  private data is being deleted or the thread is being
 *                  terminated.
 *      pv_pvt      Contains or references the private data.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK :          Routine was successful.
 *      OSR_INV_ARG :     Invalid argument.
 *      OSR_NOT_INIT :    Thread module has not been initiated.
 *      OSR_NOT_EXIST :   Invalid or unknown OS thread.
 *      OSR_DUP_KEY :     Duplicate key.
 *      OSR_NO_RESOURCE : Out of resources (e.g. memory etc.).
 *---------------------------------------------------------------------------*/
INT32 x_thread_set_pvt (UINT32                   ui4_key,
                        x_os_thread_pvt_del_fct  pf_pvt_del,
                        VOID*                    pv_pvt)
{
    INT32          i4_return;
    VOID*          pv_th_id;
    OS_THREAD_T*   pt_thread;
    OS_PVT_CTRL_T* pt_pvt_ctrl;


    i4_return = OSR_NOT_INIT;

    if (b_thread_inited)
    {
        i4_return = OSR_INV_ARG;

        if ((ui4_key != PVT_KEY_ILLEGAL)                 &&
            ((ui4_key & PVT_KEY_FLAG_MASK_UNUSED) == 0))
        {
            i4_return = OSR_NOT_EXIST;

            if (x_os_drv_thread_self (&pv_th_id, ((VOID**) &pt_thread)) == OSR_DRV_OK)
            {
                if (pt_thread == NULL)
                {
                    return OSR_NOT_EXIST;
                }
                i4_return = OSR_DUP_KEY;

                /* Do not allow duplicate keys. */
                if ((pt_pvt_ctrl = thread_find_pvt_key (pt_thread, ui4_key)) == NULL)
                {
                    i4_return = OSR_NO_RESOURCE;

                    /* Allocate space for the OS private control block. */
                    if ((pt_pvt_ctrl = (OS_PVT_CTRL_T*) x_mem_alloc (sizeof (OS_PVT_CTRL_T))) != NULL)
                    {
                        i4_return = OSR_OK;

                        pt_pvt_ctrl->pt_next    = NULL;
                        pt_pvt_ctrl->pt_prev    = NULL;
                        pt_pvt_ctrl->pf_pvt_del = pf_pvt_del;
                        pt_pvt_ctrl->pv_pvt     = pv_pvt;
                        pt_pvt_ctrl->ui4_key    = ui4_key;

                        thread_add_pvt_key (pt_thread, pt_pvt_ctrl);
                    }
                }
            }
        }
    }

    return (i4_return);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_get_pvt()
 *
 * Description:
 *      This API gets some private data, associated with a thread and key.
 *      Such private data was set using API "x_thread_set_pvt ()".
 *
 * Inputs:
 *      ui4_key  Contains the key associated with the private data.
 *
 * Outputs:
 *      ppv_pvt  Contains or references the private data.
 *
 * Returns:
 *      OSR_OK :            Routine was successful.
 *      OSR_INV_ARG :       Invalid argument.
 *      OSR_NOT_INIT :      Thread module has not been initiated.
 *      OSR_NOT_EXIST :     Invalid or unknown OS thread.
 *      OSR_KEY_NOT_FOUND : The specified key in argument "ui4_key" was not
 *                          found.
 *---------------------------------------------------------------------------*/
INT32 x_thread_get_pvt (UINT32  ui4_key,
                        VOID**  ppv_pvt)
{
    INT32          i4_return;
    VOID*          pv_th_id;
    OS_THREAD_T*   pt_thread;
    OS_PVT_CTRL_T* pt_pvt_ctrl;


    i4_return = OSR_NOT_INIT;

    if (b_thread_inited)
    {
        i4_return = OSR_INV_ARG;

        if (ppv_pvt != NULL)
        {
            i4_return = OSR_NOT_EXIST;

            if (x_os_drv_thread_self (&pv_th_id, ((VOID**) &pt_thread)) == OSR_DRV_OK)
            {
                if (pt_thread == NULL)
                {
                    return OSR_NOT_EXIST;
                }
                i4_return = OSR_KEY_NOT_FOUND;

                /* Find and return the specified key. */
                if ((pt_pvt_ctrl = thread_find_pvt_key (pt_thread, ui4_key)) != NULL)
                {
                    i4_return = OSR_OK;

                    (*ppv_pvt) = pt_pvt_ctrl->pv_pvt;
                }
            }
        }
    }

    return (i4_return);
}


/*----------------------------------------------------------------------------
 * Function: x_thread_del_pvt()
 *
 * Description:
 *      This API deletes some private data, associated with a thread and key.
 *      Such private data was set using API "x_thread_set_pvt ()".
 *
 * Inputs:
 *      ui4_key  Contains the key associated with the private data.
 *
 * Outputs:
 *      None.
 *
 * Returns:
 *      OSR_OK :            Routine was successful.
 *      OSR_NOT_INIT :      Thread module has not been initiated.
 *      OSR_NOT_EXIST :     Invalid or unknown OS thread.
 *      OSR_KEY_NOT_FOUND : The specified key in argument "ui4_key" was not
 *                          found.
 *---------------------------------------------------------------------------*/
INT32 x_thread_del_pvt (UINT32  ui4_key)
{
    INT32          i4_return;
    VOID*          pv_th_id;
    OS_THREAD_T*   pt_thread;
    OS_PVT_CTRL_T* pt_pvt_ctrl;


    i4_return = OSR_NOT_INIT;

    if (b_thread_inited)
    {
        i4_return = OSR_NOT_EXIST;

        if (x_os_drv_thread_self (&pv_th_id, ((VOID**) &pt_thread)) == OSR_DRV_OK)
        {
            if (pt_thread == NULL)
            {
                return OSR_NOT_EXIST;
            }

            i4_return = OSR_KEY_NOT_FOUND;

            /* Find and delete the specified key. */
            if ((pt_pvt_ctrl = thread_find_pvt_key (pt_thread, ui4_key)) != NULL)
            {
                i4_return = OSR_OK;

                thread_del_pvt_key (pt_thread, pt_pvt_ctrl);

                x_mem_free ((VOID*) pt_pvt_ctrl);
            }
        }
    }

    return (i4_return);
}

