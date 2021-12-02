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
 * $RCSfile: thread_lib.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#include "os/inc/os.h"
#include "os/memory/mem_lib.h"
#include "os/semaphore/sema_lib.h"
#include "os/thread/thread_lib.h"
#include "os/timer/timer_lib.h"
#include "os/msgQ/msgq_lib.h"

#ifdef SYS_MEM_CHK
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#undef x_mem_free

extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#endif

BOOL b_thread_inited = FALSE;      /* exported to mem_init() */

static OS_THREAD_T *pt_os_thread = NULL;
static HANDLE_T h_th_sema_hdl = (HANDLE_T) NULL_HANDLE;



/*----------------------------------------------------------------------------
 * Function: thread_init()
 *
 * Description:
 *      this routine is called as part of os_init() and creates a semaphore
 *      to guard global thread struct link list.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      others (not OSR_OK): routine failed.
 *---------------------------------------------------------------------------*/
INT32
thread_init()
{
    INT32 i4_ret;

    if (h_th_sema_hdl == NULL_HANDLE)
    {
        i4_ret = x_sema_create(&h_th_sema_hdl,
                               X_SEMA_TYPE_BINARY,
                               X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            return(i4_ret);
        }
    }

    b_thread_inited = TRUE;

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: thread_put_th_obj()
 *
 * Description:
 *      this routine is called to put a thread struct in global thread list.
 *      it performs
 *      1. check if thread mudule is initiated.
 *      2. guard global thread link list.
 *      3. enqueue the thread struct at the beginning of the list.
 *
 * Inputs:
 *      pt_thread: pointer to a thread struct.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : routine was successful.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
INT32
thread_put_th_obj(OS_THREAD_T *pt_thread)
{
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = x_sema_lock(h_th_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    /*
     * insert this thread at the beginning of its hash
     */
    SET_LINK_NEXT(pt_thread, pt_os_thread);
    SET_LINK_HEAD(pt_os_thread, pt_thread);

    i4_ret = x_sema_unlock(h_th_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: thread_get_th_obj_by_name()
 *
 * Description:
 *      this routine is called to get a thread struct by its name. it performs
 *      1. check if thread mudule is initiated.
 *      2. guard global thread link list.
 *      3. go through the global thread list to look for a thread struct using
 *         thread's name as key.
 *
 * Inputs:
 *      ps_name: name of the thread.
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE : thread was found.
 *      FALSE : thread was not found.
 *---------------------------------------------------------------------------*/
BOOL
thread_get_th_obj_by_name(const CHAR *ps_name)
{
    OS_THREAD_T *pt_thread;
    INT32 i4_ret;
    BOOL b_found = FALSE;

    if (b_thread_inited != TRUE)
    {
        return(b_found);
    }

    i4_ret = x_sema_lock(h_th_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(b_found);
    }

    GET_LINK_HEAD(pt_thread, pt_os_thread);
    while (LINK_NOT_EMPTY(pt_thread, NULL))
    {
        i4_ret = x_strncmp(pt_thread->s_name, ps_name, THREAD_NAME_LEN);
        if (i4_ret == 0)
        {
            b_found = TRUE;
            break;
        }

        GET_LINK_NEXT(pt_thread, pt_thread);
    }

    i4_ret = x_sema_unlock(h_th_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(b_found);
}


/*----------------------------------------------------------------------------
 * Function: thread_get_th_obj_by_id()
 *
 * Description:
 *      this routine is called to get a thread struct by its thread id.
 *      it performs
 *      1. check if thread mudule is initiated.
 *      2. guard global thread link list.
 *      3. go through the global thread list to look for a thread struct using
 *         thread's name as key.
 *
 * Inputs:
 *      pv_th_id: thread id (at OS driver).
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL : thread struct.
 *      NULL : thread was not found.
 *---------------------------------------------------------------------------*/
OS_THREAD_T *
thread_get_th_obj_by_id(VOID *pv_th_id)
{
    OS_THREAD_T *pt_thread;
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(NULL);
    }

    i4_ret = x_sema_lock(h_th_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(NULL);
    }

    GET_LINK_HEAD(pt_thread, pt_os_thread);
    while (LINK_NOT_EMPTY(pt_thread, NULL))
    {
        if (pv_th_id == pt_thread->pv_th_id)
        {
            break;
        }

        GET_LINK_NEXT(pt_thread, pt_thread);
    }

    i4_ret = x_sema_unlock(h_th_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(pt_thread);
}


/*----------------------------------------------------------------------------
 * Function: thread_remove_th_obj()
 *
 * Description:
 *      this routine is called to remove a thread struct from global thread list
 *      it performs
 *      1. check if thread mudule is initiated.
 *      2. guard global thread link list.
 *      3. go through global thread list to look for a thread struct using the
 *         address of thread struct as key.
 *
 * Inputs:
 *      pt_thread: pointer to a thread struct.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
thread_remove_th_obj(OS_THREAD_T *pt_thread)
{
    OS_THREAD_T *pt_cur, *pt_prev;
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NOT_INIT);
    }

    pt_prev = NULL;

    i4_ret = x_sema_lock(h_th_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_LOCK_FAIL);
    }

    GET_LINK_HEAD(pt_cur, pt_os_thread);
    while (LINK_NOT_EMPTY(pt_cur, NULL))
    {
        if (pt_cur == pt_thread)
        {
            if (LINK_IS_EMPTY(pt_prev, NULL))
            {
                SET_LINK_HEAD(pt_os_thread, pt_thread->pt_next);
            }
            else
            {
                SET_LINK_NEXT(pt_prev, pt_thread->pt_next);
            }

            SET_LINK_NEXT(pt_thread, NULL);
            break;
        }

        pt_prev = pt_cur;
        GET_LINK_NEXT(pt_cur, pt_cur);
    }

    i4_ret = x_sema_unlock(h_th_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }
}


/*----------------------------------------------------------------------------
 * Function: thread_main()
 *
 * Description:
 *      this routine is a generic thread main entry function at Middleware.
 *      this routine is called when the thread is created in the system.
 *      the routine then calls thread main entry function passed in
 *      x_thread_create().
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
x_os_drv_thread_main_fct
thread_main()
{
    OS_THREAD_T *pt_thread;
    VOID *pv_th_id;             /* thread id of underlying OS */

    (VOID) x_os_drv_thread_self(&pv_th_id, ((VOID**) &pt_thread));

    if (pt_thread != NULL)
    {
        (VOID) x_sema_lock(pt_thread->h_sema_hdl, X_SEMA_OPTION_WAIT);

        if (!(pt_thread->ui1_flag & FLAG_TH_CLOSE))
        {
            (pt_thread->pf_main)(pt_thread->pv_arg);
        }

        x_thread_exit();    /* so that application won't have to do this */
    }
    else
    {
        OS_DBG_ABORT(DBG_CAT_THREAD, DBG_THREAD_NO_OBJECT);
    }

    return((x_os_drv_thread_main_fct) NULL);
}


/*----------------------------------------------------------------------------
 * Function: os_check_handle()
 *
 * Description:
 *      this routine is called to check if passed in handle has proper handle
 *      type and return its object if a holder is provided. it performs
 *      1. call handle_get_type to get handle type and compare it against
 *         expected type.
 *      2. if a handle object holder is provided, call handle_get_obj to
 *         return handle object.
 *
 * Inputs:
 *      h_hdl : handle.
 *      e_hdl_type : handle type.
 *      pv_obj : pointer to hold handle object.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_INV_HANDLE : routine failed.
 *---------------------------------------------------------------------------*/
INT32
os_check_handle(HANDLE_T h_hdl, HANDLE_TYPE_T e_hdl_type, VOID **ppt_obj)
{
    VOID* pv_obj;
    HANDLE_TYPE_T e_type;

    if ((handle_get_type_obj (h_hdl, &e_type, &pv_obj) != HR_OK) ||
        (e_type != e_hdl_type)                                   ||
        (pv_obj == NULL))
    {
        return(OSR_INV_HANDLE);
    }

    if (ppt_obj != NULL)
    {
        (*ppt_obj) = pv_obj;
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: thread_free_cb()
 *
 * Description:
 *      this routine is called via handle_free(). it performs
 *      1. check e_type is THREAD.
 *      2. check timer flag to make sure it comes from proper path of
 *         calling handle_free() and then free timer struct object.
 *
 * Inputs:
 *      h_hdl : handle.
 *      e_type : handle type.
 *      pv_obj : handle object.
 *      pv_tag : handle private tag.
 *      b_req_hdl : TRUE or FALSE.
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE : routine was successful.
 *      FALSE : routine failed.
 *---------------------------------------------------------------------------*/
BOOL
thread_free_cb(HANDLE_T       h_hdl,
               HANDLE_TYPE_T  e_type,
               VOID *         pv_obj,
               VOID *         pv_tag,
               BOOL           b_req_hdl)
{
    OS_THREAD_T *pt_thread;    /* thread object */

    if (e_type != HT_GROUP_OS_THREAD)
    {
        return(FALSE);
    }

    pt_thread = (OS_THREAD_T *) pv_obj;

    if (pt_thread->ui1_flag & FLAG_TH_CLOSE)
    {
        /* free semaphore if created */
        if (pt_thread->h_sema_hdl != NULL_HANDLE)
        {
            (VOID) x_sema_delete(pt_thread->h_sema_hdl);
            pt_thread->h_sema_hdl = NULL_HANDLE;
        }

        if (pt_thread->h_sema_evg != NULL_HANDLE)
        {
            (VOID) x_sema_delete(pt_thread->h_sema_evg);
            pt_thread->h_sema_evg = NULL_HANDLE;
        }

        (VOID) thread_remove_th_obj(pt_thread);
        x_mem_free(pt_thread->pv_arg);
        x_mem_free(pt_thread);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}


/*----------------------------------------------------------------------------
 * Function: thread_add_pvt_key()
 *
 * Description:
 *      This API adds a private data control block to the thread control
 *      block.
 *
 * Inputs:
 *      pt_thread    References the thread control block.
 *      pv_pvt_ctrl  References the private data control block.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      None.
 *---------------------------------------------------------------------------*/
VOID thread_add_pvt_key (OS_THREAD_T*    pt_thread,
                         OS_PVT_CTRL_T*  pt_pvt_ctrl)
{
    /* Link the private data to the threda. */
    if ((pt_pvt_ctrl->pt_next = pt_thread->pt_pvt_head) != NULL)
    {
        (pt_pvt_ctrl->pt_next)->pt_prev = pt_pvt_ctrl;
    }

    pt_pvt_ctrl->pt_prev   = NULL;
    pt_thread->pt_pvt_head = pt_pvt_ctrl;
}


/*----------------------------------------------------------------------------
 * Function: thread_del_pvt_key()
 *
 * Description:
 *      This API deletes a private data control block from the thread control
 *      block. If the private data delete function is not NULL it will also
 *      executed.
 *
 * Inputs:
 *      pt_thread    References the thread control block.
 *      pv_pvt_ctrl  References the private data control block.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      None.
 *---------------------------------------------------------------------------*/
VOID thread_del_pvt_key (OS_THREAD_T*    pt_thread,
                         OS_PVT_CTRL_T*  pt_pvt_ctrl)
{
    /* Delink the private control block. */
    if (pt_pvt_ctrl->pt_next != NULL)
    {
        (pt_pvt_ctrl->pt_next)->pt_prev = pt_pvt_ctrl->pt_prev;
    }

    if (pt_pvt_ctrl->pt_prev != NULL)
    {
        (pt_pvt_ctrl->pt_prev)->pt_next = pt_pvt_ctrl->pt_next;
    }
    else
    {
        pt_thread->pt_pvt_head = pt_pvt_ctrl->pt_next;
    }

    pt_pvt_ctrl->pt_next = NULL;
    pt_pvt_ctrl->pt_prev = NULL;

    /* If a private data delete callback function is available then execute it. */
    if (pt_pvt_ctrl->pf_pvt_del != NULL)
    {
        pt_pvt_ctrl->pf_pvt_del (pt_pvt_ctrl->ui4_key, pt_pvt_ctrl->pv_pvt);
    }
}


/*----------------------------------------------------------------------------
 * Function: thread_find_pvt_key()
 *
 * Description:
 *      This API searches through the private data control blocks attached to
 *      a thread for one with a matching key value.
 *
 * Inputs:
 *      pt_thread  References the thread control block.
 *      ui4_key    Contains the key to be found.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      A reference to the found private data control block or NULL.
 *---------------------------------------------------------------------------*/
OS_PVT_CTRL_T* thread_find_pvt_key (OS_THREAD_T*  pt_thread,
                                    UINT32        ui4_key)
{
    OS_PVT_CTRL_T* pt_pvt_ctrl;

    /* Find the private control block with matching key. */
    pt_pvt_ctrl = pt_thread->pt_pvt_head;

    while ((pt_pvt_ctrl          != NULL)     &&
           (pt_pvt_ctrl->ui4_key != ui4_key))
    {
        pt_pvt_ctrl = pt_pvt_ctrl->pt_next;
    }

    return (pt_pvt_ctrl);
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: os_cli_show_thread_all
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32
os_cli_show_thread_all(INT32        i4_argc,
                       const CHAR   **pps_argv)
{
    OS_THREAD_T *pt_th;
    INT32 i4_i;


    i4_i = 1;
    (VOID) x_sema_lock(h_th_sema_hdl, X_SEMA_OPTION_WAIT);
    pt_th = pt_os_thread;
    while (pt_th != NULL)
    {
        x_dbg_stmt("[%d] thread obj 0x%x, name: %s\n",
                    i4_i, pt_th, pt_th->s_name);

        pt_th = pt_th->pt_next;
        ++i4_i;
    }
    (VOID) x_sema_unlock(h_th_sema_hdl);

    return(OSR_OK);
}
#endif
