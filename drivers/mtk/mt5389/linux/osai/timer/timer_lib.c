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
 * $RCSfile: timer_lib.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#include "os.h"
#include "x_hal_arm.h"
#include "../thread/thread_lib.h"
#include "timer_lib.h"
#include "../msgQ/msgq_lib.h"


BOOL b_timer_inited = FALSE;

UINT32 ui4_systick = 0;             /* current system tick in middleware */
OS_TIMER_T *pt_timer_list = NULL;


/*----------------------------------------------------------------------------
 * Function: _timer_init()
 *
 * Description:
 *      this routine is called as part of os_init(). it performs
 *      1. allocate memory for timer list struct (OS_TIMER_T).
 *      2. create a semaphore to guard timer list struct.
 *      3. get tick period (resolution).
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_FAIL : OS driver related error.
 *      OSR_NO_RESOURCE: no resource (memory).
 *---------------------------------------------------------------------------*/
INT32
_timer_init()
{
    INT32 i4_ret;

    /* create timer object list */
    i4_ret = OSR_OK;
    if (pt_timer_list == NULL)
    {
        pt_timer_list = (OS_TIMER_T *) x_mem_alloc(sizeof(OS_TIMER_T));
        if (pt_timer_list != NULL)
        {
            SET_LINK_HEAD(pt_timer_list->pt_head, NULL);
            pt_timer_list->h_sema_hdl = (HANDLE_T) NULL_HANDLE;
            pt_timer_list->h_th_hdl = (HANDLE_T) NULL_HANDLE;
            pt_timer_list->ui4_tick_res = 0;
        }
        else
        {
            return(OSR_NO_RESOURCE);
        }
    }

    if (pt_timer_list->h_sema_hdl == (HANDLE_T) NULL_HANDLE)
    {
        i4_ret = x_sema_create(&pt_timer_list->h_sema_hdl,
                               X_SEMA_TYPE_BINARY,
                               X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            return(i4_ret);
        }
    }

    if (i4_ret != OSR_OK)
    {
        pt_timer_list->ui4_tick_res = 0;
    }

    pt_timer_list->ui4_tick_res = x_os_drv_get_tick_period ();

    b_timer_inited = TRUE;

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: timer_put_timer_obj()
 *
 * Description:
 *      this routine is called to start a timer. it performs
 *      1. remove the timer from timer list which implicitly cancels
 *         previously scheduled timer if any.
 *      2. start crit protection.
 *      3. insert timer struct into timer list.
 *
 * Inputs:
 *      pt_timer : pointer to timer struct.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *---------------------------------------------------------------------------*/
INT32
timer_put_timer_obj(OS_TIMER_OBJ_T  *pt_timer)
{
    CRIT_STATE_T t_crit;

    /* remove it and insert it again based upon its new expired tick */
    timer_remove_timer_obj(pt_timer, TIMER_ST_STOP);

    pt_timer->ui4_expire = ui4_systick + pt_timer->ui4_delay;

    t_crit = x_crit_start();

    timer_insert_timer_obj(pt_timer);

    x_crit_end(t_crit);

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: timer_insert_timer_obj()
 *
 * Description:
 *      this routine is called to insert a timer to timer list. it performs
 *      1. mark timer struct as start.
 *      2. insert timer struct into timer list in ascending order of
 *         expire tick.
 *
 * Inputs:
 *      pt_timer : pointer to timer struct.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
timer_insert_timer_obj(OS_TIMER_OBJ_T  *pt_timer)
{
    OS_TIMER_OBJ_T *pt_tmp;

    /* reset the following status */
    pt_timer->ui1_status |= TIMER_ST_START;

    GET_LINK_HEAD(pt_tmp, pt_timer_list->pt_head);
    if (LINK_IS_EMPTY(pt_tmp, NULL))
    {
        /* insert as the only timer */
        SET_LINK_NEXT(pt_timer, pt_timer);                  /* link to self */
        SET_LINK_PREV(pt_timer, pt_timer);                  /* link to self */
        SET_LINK_HEAD(pt_timer_list->pt_head, pt_timer);    /* set as head */
    }
    else
    {
        if (pt_timer->ui4_expire < pt_tmp->pt_prev->ui4_expire)
        {
            /* pt_timer is not the longest to expire */
            do
            {
                if (pt_timer->ui4_expire < pt_tmp->ui4_expire)
                {
                    break;
                }

                GET_LINK_NEXT(pt_tmp, pt_tmp);
            } while (pt_tmp != pt_timer_list->pt_head);
        }

        /* insert the timer before pt_tmp */
        SET_LINK_NEXT(pt_timer, pt_tmp);
        SET_LINK_PREV(pt_timer, pt_tmp->pt_prev);
        SET_LINK_NEXT(pt_tmp->pt_prev, pt_timer);
        SET_LINK_PREV(pt_tmp, pt_timer);

        /* adjust the head and the timer ticks before the next timer pops */
        if (pt_timer->ui4_expire < pt_timer_list->pt_head->ui4_expire)
        {
            SET_LINK_HEAD(pt_timer_list->pt_head, pt_timer);
        }
    }
}


/*----------------------------------------------------------------------------
 * Function: timer_remove_timer_obj()
 *
 * Description:
 *      this routine is called to remove a timer from timer list. it performs
 *      1. start crit protection.
 *      2. remove the timer from timer list by adjusting prev and next pointers
 *
 * Inputs:
 *      pt_timer : pointer to timer struct.
 *      ui1_status : timer status.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *---------------------------------------------------------------------------*/
INT32
timer_remove_timer_obj(OS_TIMER_OBJ_T *pt_timer, UINT8 ui1_status)
{
    CRIT_STATE_T t_crit;

    t_crit = x_crit_start();

    if (!(pt_timer->ui1_status & TIMER_ST_START)
        || (pt_timer->pt_next == NULL) || (pt_timer->pt_prev == NULL))
    {
        x_crit_end(t_crit);
        return(OSR_OK);
    }

    /* to sync with system tick ISR */

    if (LINK_IS_EMPTY(pt_timer_list->pt_head, NULL))
    {
        x_crit_end(t_crit);
        return(OSR_OK);
    }

    /* this timer has not poped yet */
    if (pt_timer_list->pt_head->pt_next == pt_timer_list->pt_head)
    {
        /* this is the only one in the list */
        SET_LINK_HEAD(pt_timer_list->pt_head, NULL);
    }
    else
    {
        SET_LINK_NEXT(pt_timer->pt_prev, pt_timer->pt_next);
        SET_LINK_PREV(pt_timer->pt_next, pt_timer->pt_prev);
        if (pt_timer == pt_timer_list->pt_head)     /* remove head? */
        {
            pt_timer_list->pt_head = pt_timer->pt_next;
        }
    }

    SET_LINK_NEXT(pt_timer, NULL);                  /* isolate this timer */
    SET_LINK_PREV(pt_timer, NULL);

    if (ui1_status & (TIMER_ST_STOP | TIMER_ST_DELETE))
    {
        pt_timer->ui1_status = TIMER_ST_NONE;       /* reset this timer */
    }

    x_crit_end(t_crit);
    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: timer_free_cb()
 *
 * Description:
 *      this routine is called via handle_free(). it performs
 *      1. check e_type is Timer.
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
timer_free_cb(HANDLE_T       h_hdl,
              HANDLE_TYPE_T  e_type,
              VOID *         pv_obj,
              VOID *         pv_tag,
              BOOL           b_req_hdl)
{
    OS_TIMER_OBJ_T *pt_timer;    /* timer object */

    if (e_type != HT_GROUP_OS_TIMER)
    {
        return(FALSE);
    }

    pt_timer = (OS_TIMER_OBJ_T *) pv_obj;
    if (pt_timer->ui1_flag & FLAG_TM_CLOSE)
    {
        x_mem_free(pt_timer);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}
