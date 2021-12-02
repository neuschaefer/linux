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
 * $RCSfile: ev_grp_api.c,v $
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

extern HANDLE_T     h_event_group_sema;
extern OS_EV_GRP_T  *pt_event_group_head;
extern BOOL         b_event_group_inited;


/*----------------------------------------------------------------------------
 * Function: x_ev_group_create()
 *
 * Description:
 *      API creates an event group and sets its current events according
 *
 * Inputs:
 *      ps_name: name of an event group, 16 bytes excluding '\0'
 *      e_init_events: nitial current events of an event group
 *
 * Outputs:
 *      ph_hdl: handle references the created event group
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_EXIST: event group exists
 *      OSR_NO_RESOURCE: out of resource such as memory, etc.
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_create(HANDLE_T           *ph_hdl,
                  const CHAR         *ps_name,
                  EV_GRP_EVENT_T     e_init_events)
{
    OS_EV_GRP_T *pt_ev_grp;
    INT32 i4_ret;


    if (b_event_group_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_hdl == NULL))
    {
        return(OSR_INV_ARG);
    }

    pt_ev_grp = (OS_EV_GRP_T *) x_mem_alloc(sizeof(OS_EV_GRP_T));
    if (pt_ev_grp == NULL)
    {
        return(OSR_NO_RESOURCE);
    }

    pt_ev_grp->pt_next = NULL;
    pt_ev_grp->pt_first_waiting = NULL;
    pt_ev_grp->e_cur_events = e_init_events;
    pt_ev_grp->ui1_num_waiting = (UINT8) 0;
    pt_ev_grp->ui1_refcount = (UINT8) 1;
    pt_ev_grp->ui1_flags = (EV_GRP_EVENT_T) 0;

    x_strncpy(pt_ev_grp->s_name, ps_name, EV_GRP_NAME_LEN);
    pt_ev_grp->s_name[EV_GRP_NAME_LEN] = '\0';

    i4_ret = handle_link_init(&pt_ev_grp->t_hdl_link);
    if (i4_ret != HR_OK)
    {
        x_mem_free(pt_ev_grp);
        return(OSR_INV_HANDLE);
    }

    /* create a handle */
    i4_ret = handle_alloc(HT_GROUP_OS_EVENT_GROUP,
                          pt_ev_grp,
                          pt_ev_grp,
                          ev_grp_free_cb,
                          ph_hdl);

    if (i4_ret != HR_OK)
    {
        x_mem_free(pt_ev_grp);
        return(HR_TO_OSR(i4_ret));
    }

    i4_ret = handle_link(&(pt_ev_grp->t_hdl_link), *ph_hdl);
    if (i4_ret != HR_OK)
    {
        pt_ev_grp->ui1_flags |= EV_GRP_FLAG_CLOSE;
        handle_free(*ph_hdl, TRUE);
        return(OSR_INV_HANDLE);
    }

    if (ev_grp_find_obj(pt_ev_grp->s_name, pt_ev_grp) != NULL)
    {
        pt_ev_grp->ui1_flags |= EV_GRP_FLAG_CLOSE;
        handle_free(*ph_hdl, TRUE);
        return(OSR_EXIST);
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_ev_group_attach()
 *
 * Description:
 *      API attaches the calling thread to an event group
 *
 * Inputs:
 *      ps_name: name of an event group, 16 bytes excluding '\0'
 *
 * Outputs:
 *      ph_hdl: handle references the created event group
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_NOT_EXIST: event group does not exist
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_attach(HANDLE_T     *ph_hdl,
                  const CHAR   *ps_name)
{
    OS_EV_GRP_T *pt_ev_grp;
    INT32 i4_ret;
    CRIT_STATE_T t_crit;


    if (b_event_group_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* arguments check */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_hdl == NULL))
    {
        return(OSR_INV_ARG);
    }

    pt_ev_grp = ev_grp_find_obj(ps_name, NULL);
    if (pt_ev_grp == NULL)
    {
        return(OSR_NOT_EXIST);
    }

    /* create a handle */
    i4_ret = handle_alloc(HT_GROUP_OS_EVENT_GROUP,
                          pt_ev_grp,
                          pt_ev_grp,
                          ev_grp_free_cb,
                          ph_hdl);
    if (i4_ret != HR_OK)
    {
        return(HR_TO_OSR(i4_ret));
    }

    t_crit = x_os_drv_crit_start();
    pt_ev_grp->ui1_refcount++;

    i4_ret = handle_link(&(pt_ev_grp->t_hdl_link), *ph_hdl);
    if (i4_ret != HR_OK)
    {
        pt_ev_grp->ui1_flags |= EV_GRP_FLAG_CLOSE;
        x_os_drv_crit_end(t_crit);
        handle_free(*ph_hdl, TRUE);
        return(OSR_INV_HANDLE);
    }

    x_os_drv_crit_end(t_crit);

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_ev_grp_delete()
 *
 * Description:
 *      API deletes an event group
 *
 * Inputs:
 *      h_hdl: handle of an event group to memory buffer.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_delete(HANDLE_T    h_hdl)
{
    OS_EV_GRP_T *pt_ev_grp;
    INT32 i4_ret;
    CRIT_STATE_T t_crit;


    if (b_event_group_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = os_check_handle(h_hdl,
                             HT_GROUP_OS_EVENT_GROUP,
                             (VOID **) &pt_ev_grp);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    t_crit = x_os_drv_crit_start();
    pt_ev_grp->ui1_flags |= EV_GRP_FLAG_CLOSE;
    x_os_drv_crit_end(t_crit);

    handle_free(h_hdl, TRUE);

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_ev_group_set_event()
 *
 * Description:
 *      API requests an events to be received (satisfied) from an event group.
 *
 * Inputs:
 *      h_hdl: handle of an event group to memory buffer.
 *      e_events: events to be set into current events of an event group
 *      e_op: operation over "e_events".
 *            one of X_EV_OP_CLEAR, X_EV_OP_AND, X_EV_OP_OR
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_DELETED: event_group was closed.
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_set_event(HANDLE_T           h_hdl,
                     EV_GRP_EVENT_T     e_events,
                     EV_GRP_OPERATION_T e_op)
{
    OS_EV_GRP_T *pt_ev_grp;
    OS_THREAD_T *pt_th, *pt_prev, *pt_tmp, *pt_wakeup, *pt_cur;
    EV_GRP_EVENT_T e_cur, e_ev;
    HANDLE_TYPE_T e_type;
    CRIT_STATE_T t_crit;
    BOOL b_match;


    if ((handle_get_type_obj(h_hdl, &e_type, (VOID **) &pt_ev_grp) == HR_OK) &&
        (e_type == HT_GROUP_OS_EVENT_GROUP))
    {
        t_crit = x_os_drv_crit_start();
        if (pt_ev_grp->ui1_refcount)
        {
            if (e_op == X_EV_OP_AND)
            {
                pt_ev_grp->e_cur_events &= e_events;
            }
            else if (e_op == X_EV_OP_OR)
            {
                pt_ev_grp->e_cur_events |= e_events;
            }
            else if (e_op == X_EV_OP_CLEAR)
            {
                pt_ev_grp->e_cur_events = (EV_GRP_EVENT_T) 0;
            }
            else
            {
                x_os_drv_crit_end(t_crit);
                return(OSR_INV_ARG);
            }

            pt_cur = pt_wakeup = pt_prev = NULL;
            e_cur = pt_ev_grp->e_cur_events;
            pt_th = pt_ev_grp->pt_first_waiting;
            while (pt_th && e_cur)
            {
                b_match = FALSE;
                e_ev = e_cur & pt_th->e_ev_grp_event_wait;

                if ((pt_th->e_ev_grp_wait_op == X_EV_OP_AND) ||
                    (pt_th->e_ev_grp_wait_op == X_EV_OP_AND_CONSUME))
                {
                    if (e_ev == pt_th->e_ev_grp_event_wait)
                    {
                        b_match = TRUE;
                    }
                }
                else    /* X_EV_OP_OR or X_EV_OP_OR_CONSUME */
                {
                    if (e_ev)
                    {
                        b_match = TRUE;
                    }
                }

                if (b_match == TRUE)
                {
                    pt_tmp = pt_th->pt_ev_grp_waiting;
                    if (pt_prev)
                    {
                        pt_prev->pt_ev_grp_waiting = pt_tmp;
                    }
                    else
                    {
                        pt_ev_grp->pt_first_waiting = pt_tmp;
                    }

                    pt_th->pt_ev_grp_waiting = NULL;

                    if (pt_wakeup)
                    {
                        pt_cur->pt_ev_grp_waiting = pt_th;
                    }
                    else
                    {
                        pt_wakeup = pt_th;
                    }
                    pt_cur = pt_th;

                    if ((pt_th->e_ev_grp_wait_op == X_EV_OP_AND_CONSUME) ||
                        (pt_th->e_ev_grp_wait_op == X_EV_OP_OR_CONSUME))
                    {
                        e_cur &= ~e_ev;
                    }

                    pt_th = pt_tmp;
                }
                else
                {
                    pt_prev = pt_th;
                    pt_th = pt_th->pt_ev_grp_waiting;
                }
            }

            x_os_drv_crit_end(t_crit);

            while (pt_wakeup)
            {
                pt_th = pt_wakeup;
                pt_wakeup = pt_th->pt_ev_grp_waiting;
                pt_th->pt_ev_grp_waiting = (OS_THREAD_T *) NULL;
                (VOID) x_sema_unlock(pt_th->h_sema_evg);
            }

            return(OSR_OK);
        }
        else
        {
            x_os_drv_crit_end(t_crit);
            return(OSR_DELETED);
        }
    }
    else
    {
        return(OSR_INV_HANDLE);
    }
}


/*----------------------------------------------------------------------------
 * Function: x_ev_group_wait_event()
 *
 * Description:
 *      API requests an events to be received (satisfied) from an event group.
 *
 * Inputs:
 *      h_hdl: handle of an event group to memory buffer.
 *      e_events_req: requested events to be received
 *      e_op: operation over "e_events_req".
 *            one of X_EV_OP_AND, X_EV_OP_AND_CONSUME, X_EV_OP_OR,
 *            X_EV_OP_OR_CONSUME
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
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_wait_event(HANDLE_T              h_hdl,
                      EV_GRP_EVENT_T        e_events_req,
                      EV_GRP_EVENT_T        *pe_events_got,
                      EV_GRP_OPERATION_T    e_op)
{

    return( ev_group_wait_event(h_hdl,
                                 e_events_req,
                                 pe_events_got,
                                 e_op,
                                 (UINT32) 0xffffffff) );
}


/*----------------------------------------------------------------------------
 * Function: x_ev_group_wait_event_timeout()
 *
 * Description:
 *      API requests an events to be received (satisfied) from an event group.
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
x_ev_group_wait_event_timeout(HANDLE_T              h_hdl,
                              EV_GRP_EVENT_T        e_events_req,
                              EV_GRP_EVENT_T        *pe_events_got,
                              EV_GRP_OPERATION_T    e_op,
                              UINT32                ui4_time)
{

    return( ev_group_wait_event(h_hdl,
                                 e_events_req,
                                 pe_events_got,
                                 e_op,
                                 ui4_time) );
}


/*----------------------------------------------------------------------------
 * Function: x_ev_group_get_info()
 *
 * Description:
 *      API extracts a snapshot of some statistics in an event group
 *
 * Inputs:
 *      h_hdl: handle of an event group to memory buffer.
 *
 * Outputs:
 *      pe_cur_events: current events in an event group
 *      pui1_num_thread_waiting: no. of threads waiting to receive events
 *                               with an event group
 *      ps_ev_group_name: name of an event group, 16 bytes excluding '\0'
 *      ps_first_wait_thread: name of the first thread waiting,
 *                            16 bytes excluding '\0'
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : event group has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_DELETED: event_group was closed.
 *---------------------------------------------------------------------------*/
INT32
x_ev_group_get_info(HANDLE_T          h_hdl,
                    EV_GRP_EVENT_T    *pe_cur_events,
                    UINT8             *pui1_num_thread_waiting,
                    CHAR              *ps_ev_group_name,
                    CHAR              *ps_first_wait_thread)
{
    OS_EV_GRP_T *pt_ev_grp;
    OS_THREAD_T *pt_th;
    INT32 i4_ret;
    CRIT_STATE_T t_crit;


    if (b_event_group_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    if ((pe_cur_events == NULL) ||
        (pui1_num_thread_waiting == NULL) ||
        (ps_ev_group_name == NULL) ||
        (ps_first_wait_thread == NULL))
    {
        return(OSR_INV_ARG);
    }

    i4_ret = os_check_handle(h_hdl,
                             HT_GROUP_OS_EVENT_GROUP,
                             (VOID **) &pt_ev_grp);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    t_crit = x_os_drv_crit_start();
    if (pt_ev_grp->ui1_refcount == (UINT8) 0)
    {
        x_os_drv_crit_end(t_crit);
        return(OSR_DELETED);
    }

    *pe_cur_events = pt_ev_grp->e_cur_events;
    *pui1_num_thread_waiting = pt_ev_grp->ui1_num_waiting;
    (VOID) x_strncpy(ps_ev_group_name, pt_ev_grp->s_name, EV_GRP_NAME_LEN);
    ps_ev_group_name[EV_GRP_NAME_LEN] = '\0';

    pt_th = pt_ev_grp->pt_first_waiting;
    if (pt_th != NULL)
    {
        (VOID) x_strncpy(ps_first_wait_thread, pt_th->s_name, THREAD_NAME_LEN);
    }
    x_os_drv_crit_end(t_crit);

    return(OSR_OK);
}
