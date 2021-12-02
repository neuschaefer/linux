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
 * $RCSfile: ev_grp_lib.c,v $
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
#include "os/event_group/ev_grp_lib.h"

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

static OS_EV_GRP_T  *pt_event_group_head = NULL;
static HANDLE_T     h_evg_sema_hdl = (HANDLE_T) NULL_HANDLE;
BOOL                b_event_group_inited = FALSE;


/*----------------------------------------------------------------------------
 * Function: ev_grp_init()
 *
 * Description:
 *      marks event group module as being initialized
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *---------------------------------------------------------------------------*/
INT32
ev_grp_init()
{
    INT32 i4_ret;

    if (h_evg_sema_hdl == NULL_HANDLE)
    {
        i4_ret = x_sema_create(&h_evg_sema_hdl,
                               X_SEMA_TYPE_BINARY,
                               X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            return(i4_ret);
        }
    }

    b_event_group_inited = TRUE;
    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: ev_grp_find_obj()
 *
 * Description:
 *      look for an event group from an existing list using its name
 *
 * Inputs:
 *      ps_name: name of an event group
 *      pt_evg_new: new event_group obj to be enqueued if not found.
 *
 * Outputs: -
 *
 * Returns:
 *      NULL : not found and new event_group obj enqueued if any.
 *      non-NULL : reference to an event group
 *---------------------------------------------------------------------------*/
OS_EV_GRP_T *
ev_grp_find_obj(const CHAR     *ps_name, OS_EV_GRP_T *pt_evg_new)
{
    OS_EV_GRP_T *pt_tmp;
    INT32 i4_ret;


    i4_ret = x_sema_lock(h_evg_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(NULL);
    }

    pt_tmp = pt_event_group_head;
    while (pt_tmp != NULL)
    {
        if (x_strncmp(pt_tmp->s_name, ps_name, EV_GRP_NAME_LEN) == 0)
        {
            break;
        }

        pt_tmp = pt_tmp->pt_next;
    }

    if ((pt_tmp == NULL) && (pt_evg_new != NULL))
    {
        pt_evg_new->pt_next = pt_event_group_head;
        pt_event_group_head = pt_evg_new;
    }

    (VOID) x_sema_unlock(h_evg_sema_hdl);

    return(pt_tmp);
}


/*----------------------------------------------------------------------------
 * Function: ev_grp_remove_obj()
 *
 * Description:
 *      this routine called to get a msg Q struct by its name. it performs
 *      1. check if msg Q mudule is initiated.
 *      2. guard global msg Q link list.
 *      3. go through global msg Q link list to look for a msg Q based on
 *         "ps_name", up to 16 characters.
 *
 * Inputs:
 *      pt_ev_grp: references an event group
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OR: routine is OK
 *      OSR_NOT_INIT : event group has not been initiated
 *---------------------------------------------------------------------------*/
INT32
ev_grp_remove_obj(OS_EV_GRP_T   *pt_ev_grp)
{
    OS_EV_GRP_T *pt_tmp, *pt_prev;
    INT32 i4_ret;


    i4_ret = x_sema_lock(h_evg_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    i4_ret = OSR_OK;
    pt_prev = NULL;
    pt_tmp = pt_event_group_head;
    while (pt_tmp != NULL)
    {
        if (pt_tmp == pt_ev_grp)
        {
            if (pt_prev == NULL)
            {
                pt_event_group_head = pt_tmp->pt_next;
            }
            else
            {
                pt_prev->pt_next = pt_tmp->pt_next;
            }

            break;
        }

        pt_prev = pt_tmp;
        pt_tmp = pt_tmp->pt_next;
    }

    (VOID) x_sema_unlock(h_evg_sema_hdl);

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: ev_group_wait_event()
 *
 * Description:
 *      an common routine for API x_ev_group_wait_event() and
 *      x_ev_group_wait_event_timeout()
 *
 * Inputs:
 *      h_hdl: handle of an event group to memory buffer.
 *      e_events_req: requested events to be received
 *      e_op: operation over "e_events_req".
 *            one of X_EV_OP_AND, X_EV_OP_AND_CONSUME, X_EV_OP_OR,
 *            X_EV_OP_OR_CONSUME
 *      ui4_time: time delay in milliseconds before bailing out
 *
 * Outputs:
 *      pe_events_got: actual events received
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_NOT_EXIST: error with OS thread.
 *      OSR_DELETED: event group has been deleted.
 *      OSR_TIMEOUT: bail out, time delay has passed.
 *---------------------------------------------------------------------------*/
INT32
ev_group_wait_event(HANDLE_T              h_hdl,
                    EV_GRP_EVENT_T        e_events_req,
                    EV_GRP_EVENT_T        *pe_events_got,
                    EV_GRP_OPERATION_T    e_op,
                    UINT32                ui4_timeout)
{
    OS_EV_GRP_T *pt_ev_grp;
    OS_THREAD_T *pt_th, *pt_prev, *pt_tmp;
    VOID *pv_th_id;
    UINT32 ui4_elapsed, ui4_ticks, ui4_tick_period;
    UINT32 ui4_before, ui4_after;
    INT32 i4_ret;
    EV_GRP_EVENT_T e_ev;
    CRIT_STATE_T t_crit;
    HANDLE_TYPE_T e_type;
    BOOL b_match;


    if (((e_op == X_EV_OP_AND) || (e_op == X_EV_OP_OR) ||
         (e_op == X_EV_OP_AND_CONSUME) || (e_op == X_EV_OP_OR_CONSUME)) &&
        pe_events_got)
    {
        if ((handle_get_type_obj(h_hdl, &e_type, (VOID **) &pt_ev_grp) == HR_OK) &&
            (e_type == HT_GROUP_OS_EVENT_GROUP))
        {
            pt_th = NULL;
            ui4_elapsed = (UINT32) 0;
            ui4_ticks = ui4_timeout;
            ui4_tick_period = x_os_drv_get_tick_period();

            t_crit = x_os_drv_crit_start();
            do
            {
                if (pt_ev_grp->ui1_refcount)
                {
                    e_ev = pt_ev_grp->e_cur_events & e_events_req;

                    if ((e_op == X_EV_OP_AND) || (e_op == X_EV_OP_AND_CONSUME))
                    {
                        b_match = (e_ev == e_events_req) ? TRUE : FALSE;
                    }
                    else    /* X_EV_OP_OR || X_EV_OP_OR_CONSUME */
                    {
                        b_match = (e_ev) ? TRUE : FALSE;
                    }

                    if (b_match == TRUE)
                    {
                        i4_ret = OSR_OK;
                        *pe_events_got = e_ev;
                        if ((e_op == X_EV_OP_OR_CONSUME) || (e_op == X_EV_OP_AND_CONSUME))
                        {
                            pt_ev_grp->e_cur_events &= ~e_ev;
                        }
                        break;
                    }
                    else
                    {
                        i4_ret = OSR_TIMEOUT;
                        if ((ui4_elapsed < ui4_ticks) /*&& (pt_th->pt_ev_grp_waiting == NULL)*/)
                        {
                            if (!pt_th)
                            {
                                (VOID) x_os_drv_thread_self(&pv_th_id, ((VOID**) &pt_th));
                                if (pt_th == NULL)
                                {
                                   i4_ret = OSR_NOT_EXIST;
                                   break;
                                }
                                pt_th->pt_ev_grp_waiting = NULL;
                                pt_th->e_ev_grp_event_got = (EV_GRP_EVENT_T) 0;
                                pt_th->e_ev_grp_wait_op = e_op;
                                pt_th->e_ev_grp_event_wait = e_events_req;

                                if (ui4_timeout != (UINT32) 0xffffffff)
                                {
                                    ui4_ticks = ((ui4_timeout + 1) / ui4_tick_period);
                                }
                            }

                            /* enqueue this thread on WAIT option or */
                            /* timeout has not expired */
                            pt_prev = NULL;
                            pt_tmp = pt_ev_grp->pt_first_waiting;
                            while (pt_tmp)
                            {
                                if (pt_th->ui1_priority < pt_tmp->ui1_priority)
                                {
                                    break;
                                }

                                pt_prev = pt_tmp;
                                pt_tmp = pt_tmp->pt_ev_grp_waiting;
                            }

                            pt_th->pt_ev_grp_waiting = pt_tmp;
                            if (pt_prev)
                            {
                                pt_prev->pt_ev_grp_waiting = pt_th;
                            }
                            else
                            {
                                pt_ev_grp->pt_first_waiting = pt_th;
                            }

                            pt_ev_grp->ui1_num_waiting++;
                            x_os_drv_crit_end(t_crit);

                            if (ui4_timeout == (UINT32) 0xffffffff)
                            {
                                i4_ret = x_sema_lock(pt_th->h_sema_evg, X_SEMA_OPTION_WAIT);
                            }
                            else
                            {
                                ui4_before = os_get_sys_tick();
                                i4_ret = x_sema_lock_timeout(pt_th->h_sema_evg, ui4_timeout);
                                ui4_after = os_get_sys_tick();
                                if (ui4_after < ui4_before)
                                {
                                    ui4_elapsed += ((((UINT32) 0xffffffff) - ui4_before) + ui4_after);
                                }
                                else
                                {
                                    ui4_elapsed += (ui4_after - ui4_before);
                                }

                                if (/*(i4_ret != OSR_TIMEOUT) &&*/ (ui4_elapsed < ui4_ticks))
                                {
                                    ui4_timeout = (ui4_ticks - ui4_elapsed) * ui4_tick_period;
                                }
                            }

                            t_crit = x_os_drv_crit_start();
                            pt_ev_grp->ui1_num_waiting--;
                        }

                        if (i4_ret == OSR_TIMEOUT)
                        {
                            *pe_events_got = (EV_GRP_EVENT_T) 0;
                            if (pt_th)
                            {
                                /* timeout occurred, dequeue this thread */
                                pt_prev = NULL;
                                pt_tmp = pt_ev_grp->pt_first_waiting;
                                while (pt_tmp)
                                {
                                    if (pt_tmp == pt_th)
                                    {
                                        if (pt_prev)
                                        {
                                            pt_prev->pt_ev_grp_waiting = pt_tmp->pt_ev_grp_waiting;
                                        }
                                        else
                                        {
                                            pt_ev_grp->pt_first_waiting = pt_tmp->pt_ev_grp_waiting;
                                        }

                                        pt_tmp->pt_ev_grp_waiting = NULL;
                                        break;
                                    }

                                    pt_prev = pt_tmp;
                                    pt_tmp = pt_tmp->pt_ev_grp_waiting;
                                }
                            }
                            break;
                        }
                    }
                }
                else
                {
                    i4_ret = OSR_DELETED;
                    break;
                }
            } while (TRUE);

            x_os_drv_crit_end(t_crit);

            return(i4_ret);
        }
        else
        {
            return(OSR_INV_HANDLE);
        }
    }
    else
    {
        return(OSR_INV_ARG);
    }
}


/*----------------------------------------------------------------------------
 * Function: ev_grp_free_cb()
 *
 * Description:
 *      free routine to free a handle object
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
 *      TRUE : object is freed
 *      FALSE : object is not freed
 *---------------------------------------------------------------------------*/
BOOL
ev_grp_free_cb(HANDLE_T       h_hdl,
               HANDLE_TYPE_T  e_type,
               VOID *         pv_obj,
               VOID *         pv_tag,
               BOOL           b_req_hdl)
{
    OS_EV_GRP_T *pt_ev_grp;
    OS_THREAD_T *pt_th, *pt_th_tmp;
    CRIT_STATE_T t_crit;


    if (e_type != HT_GROUP_OS_EVENT_GROUP)
    {
        return(FALSE);
    }

    pt_ev_grp = (OS_EV_GRP_T *) pv_obj;

    pt_th = (OS_THREAD_T *) NULL;
    t_crit = x_os_drv_crit_start();
    if (pt_ev_grp->ui1_flags & EV_GRP_FLAG_CLOSE)
    {
        pt_ev_grp->ui1_flags &= ~EV_GRP_FLAG_CLOSE;
        (VOID) handle_delink(&(pt_ev_grp->t_hdl_link), h_hdl);

        if (pt_ev_grp->ui1_refcount != (UINT8) 0)
        {
            pt_ev_grp->ui1_refcount--;

            if (pt_ev_grp->ui1_refcount == (UINT8) 0)
            {
                pt_th = pt_ev_grp->pt_first_waiting;
                pt_ev_grp->pt_first_waiting = NULL;
            }
        }

        x_os_drv_crit_end(t_crit);

        if (pt_th)
        {
            (VOID) ev_grp_remove_obj(pt_ev_grp);
            x_mem_free(pt_ev_grp);

            do
            {
                pt_th_tmp = pt_th->pt_ev_grp_waiting;
                pt_th->pt_ev_grp_waiting = NULL;
                pt_th->ui1_flag |= FLAG_TH_EV_GRP_CLOSE;

                (VOID) x_sema_unlock(pt_th->h_sema_evg);

                pt_th = pt_th_tmp;
            } while (pt_th);
        }

        return(TRUE);
    }
    else
    {
        x_os_drv_crit_end(t_crit);
        return(FALSE);
    }
}
