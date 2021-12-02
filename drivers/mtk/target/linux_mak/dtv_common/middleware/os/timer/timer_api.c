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
 * $RCSfile: timer_api.c,v $
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
#include "handle/handle.h"

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

extern UINT32 ui4_systick;
extern OS_TIMER_T *pt_timer_list;

/* ffr  2005-04-29.  Perform SW system time / date handling. */
#define NUM_MSEC_IN_SEC  ((UINT16) 1000)
#define NUM_SW_TIMER     4

static TIME_T  t_sys_td_sec[NUM_SW_TIMER];

static UINT16  ui2_sys_td_msec[NUM_SW_TIMER];

static BOOL  b_sys_td_valid[NUM_SW_TIMER] = {FALSE, FALSE, FALSE, FALSE};

static BOOL  b_sys_td_reserved[NUM_SW_TIMER] = {FALSE, FALSE, FALSE, FALSE};


/*----------------------------------------------------------------------------
 * Function: x_timer_create()
 *
 * Description:
 *      this API creates timer and its handle. it performs
 *      1. check if timer mudule is initiated.
 *      2. parse argumets.
 *      3. allocate memory for timer struct.
 *      4. initialize, allocate, and link a messaage haandle.
 *      4. initialize timer struct.
 *
 * Inputs: -
 *
 * Outputs:
 *      *ph_timer_hdl: pointer to hold returned timer handle.
 *                     the handle is valid only if this API returns OSR_OK.
 *                     the handle is used to access the created timer.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
x_timer_create(HANDLE_T *ph_timer_hdl)      /* timer handle */
{
    INT32           i4_ret;
    OS_TIMER_OBJ_T* pt_timer;      /* timer object */


    if (ph_timer_hdl == NULL)
    {
        return(OSR_INV_ARG);
    }

    if (b_timer_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* create a timer object */
    pt_timer = (OS_TIMER_OBJ_T *) x_mem_alloc(sizeof(OS_TIMER_OBJ_T));
    if (pt_timer == NULL)
    {
        return(OSR_NO_RESOURCE);
    }

    /* create a hnadle */
    i4_ret = handle_alloc(HT_GROUP_OS_TIMER,
                          pt_timer,
                          NULL,
                          timer_free_cb,
                          ph_timer_hdl);

    if (i4_ret != HR_OK)
    {
        x_mem_free(pt_timer);
        return(HR_TO_OSR(i4_ret));
    }

    /* initialize timer obj */
    pt_timer->pt_next = NULL;
    pt_timer->pt_prev = NULL;
    pt_timer->h_timer_hdl = *ph_timer_hdl;
    pt_timer->pf_cb = NULL;
    pt_timer->ui4_delay = 0;
    pt_timer->ui4_expire = OS_TIMER_NEVER_EXPIRE;
    pt_timer->e_flags = X_TIMER_FLAG_ONCE;
    pt_timer->ui1_status = TIMER_ST_IDLE;
    pt_timer->ui1_flag = (UINT8) 0;

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_timer_start()
 *
 * Description:
 *      this API starts a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. parse argumets.
 *      3. check handle validity.
 *      4. fill up timer struct and put timer struct in the global timer
 *         doubly linked list.
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *      ui4_delay: timer pops after this delay has passed.
 *      e_flags: either X_TIMER_FLAG_ONCE or X_TIMER_FLAG_REPEAT.
 *      pf_cb: callback function to be executed after "ui4_delay" had passed.
 *      *pv_tag: argument to callback function pf_cb.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_timer_start(HANDLE_T          h_timer_hdl,    /* timer handle */
                    UINT32            ui4_delay,
                    TIMER_FLAG_T      e_flags,
                    x_os_timer_cb_fct pf_cb,    /* callback function */
                    VOID*             pv_tag)
{
    CRIT_STATE_T    t_state;
    INT32           i4_ret;
    OS_TIMER_OBJ_T* pt_timer;      /* timer object */


    if ((pf_cb == NULL) || (ui4_delay == 0) ||
        ((e_flags != X_TIMER_FLAG_ONCE) && (e_flags != X_TIMER_FLAG_REPEAT)))
    {
        return(OSR_INV_ARG);
    }

    t_state = x_crit_start();

    /* check if h_timer_hdl has the right type and get its object */
    i4_ret = OSR_INV_HANDLE;

    if ((os_check_handle (h_timer_hdl, HT_GROUP_OS_TIMER, (VOID**) &pt_timer) == OSR_OK) &&
        (x_handle_set_tag (h_timer_hdl, pv_tag) == HR_OK))
    {
        i4_ret = OSR_NOT_INIT;

        if (b_timer_inited)
        {
            timer_remove_timer_obj (pt_timer, TIMER_ST_IDLE);

            pt_timer->e_flags     = e_flags;
            pt_timer->h_timer_hdl = h_timer_hdl;
            pt_timer->pf_cb       = pf_cb;

            if ((pt_timer->ui4_delay = ui4_delay / pt_timer_list->ui4_tick_res) == 0)
            {
                pt_timer->ui4_delay = 1;    /* at lease 1 tick */
            }

            timer_insert_timer_obj (pt_timer, pt_timer->ui4_delay);

            i4_ret = OSR_OK;
        }
    }

    x_crit_end(t_state);

    return (i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_timer_stop()
 *
 * Description:
 *      this API stops a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. remove timer struct from the global timer doubly linked list.
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_timer_stop (HANDLE_T h_timer_hdl)
{
    CRIT_STATE_T    t_state;
    INT32           i4_ret;
    OS_TIMER_OBJ_T* pt_timer;


    i4_ret = OSR_INV_HANDLE;

    t_state = x_crit_start();

    /* check if h_timer_hdl has the right type and get its object */
    if (os_check_handle (h_timer_hdl, HT_GROUP_OS_TIMER, (VOID**) &pt_timer) == OSR_OK)
    {
        i4_ret = OSR_NOT_INIT;

        if (b_timer_inited)
        {
            timer_remove_timer_obj (pt_timer, TIMER_ST_STOPPED);

            i4_ret = OSR_OK;
        }
    }

    x_crit_end(t_state);

    return (i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_timer_delete()
 *
 * Description:
 *      this API deleted a timer and its handle. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. remove timer struct from the global timer doubly linked list.
 *      4. free timer handle.
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_timer_delete (HANDLE_T h_timer_hdl)
{
    CRIT_STATE_T    t_state;
    OS_TIMER_OBJ_T  *pt_timer;
    INT32 i4_ret;


    i4_ret = OSR_INV_HANDLE;

    t_state = x_crit_start();

    /* check if h_timer_hdl has the right type and get its object */
    if (os_check_handle (h_timer_hdl, HT_GROUP_OS_TIMER, (VOID**) &pt_timer) == OSR_OK)
    {
        i4_ret = OSR_NOT_INIT;

        if (b_timer_inited)
        {
            timer_remove_timer_obj (pt_timer, TIMER_ST_IDLE);

            /* timer object still in timer list, free timer handle and object */
            /* mark to indicate from x_timer_delete */
            /* to distinguish from direct call of handle_free */
            pt_timer->ui1_flag |= FLAG_TM_CLOSE;
            
            x_crit_end(t_state); /* !! SHOULD do this before handle_free  */
            
            handle_free (h_timer_hdl, TRUE);

            i4_ret = OSR_OK;
        }
    }
    else
    {
        x_crit_end(t_state);
    }

    return (i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_timer_resume()
 *
 * Description:
 *      this API resumes a previoulsy stopped timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. pause the timer
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 x_timer_resume (HANDLE_T h_timer_hdl)
{
    CRIT_STATE_T    t_state;
    INT32           i4_ret;
    OS_TIMER_OBJ_T* pt_timer;


    i4_ret = OSR_INV_HANDLE;

    t_state = x_crit_start();

    /* check if h_timer_hdl has the right type and get its object */
    if (os_check_handle (h_timer_hdl, HT_GROUP_OS_TIMER, (VOID**) &pt_timer) == OSR_OK)
    {
        i4_ret = OSR_NOT_INIT;

        if (b_timer_inited)
        {
            i4_ret = OSR_FAIL;

            /* Only allow timers, which were successfully */
            /* stopped, to be resumed.                    */
            if (pt_timer->ui1_status == TIMER_ST_STOPPED)
            {
                timer_insert_timer_obj (pt_timer,
                                        ((pt_timer->ui4_expire > pt_timer->ui4_stop) ? (pt_timer->ui4_expire - pt_timer->ui4_stop) : 1));

                i4_ret = OSR_OK;
            }
        }
    }

    x_crit_end(t_state);

    return (i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_tick()
 *
 * Description:
 *      this API is called by system tick ISR or similar mechanism. it performs
 *      1. check if timer mudule is initiated. 
 *      2. guard global timer doubly linked list.
 *      3. check if the head of global timer doubly linked list should have
 *         expired, and if so, remove it from list.
 *      4. call callback pf_cb.
 *      5. put the timer struct in the list if it is a REPEAT type.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_tick()
{
    HANDLE_TYPE_T       e_type;
    VOID*               pv_tag;
    VOID*               pv_obj;
    INT32               i4_i;
    INT32               i4_ret;
    CRIT_STATE_T        t_crit;
    OS_TIMER_OBJ_T*     pt_timer;
    x_os_timer_cb_fct   pf_cb;
    HANDLE_T            h_timer_hdl;

    ++ui4_systick;      /* increment system tick */

    if (b_timer_inited == TRUE)
    {
        t_crit = x_crit_start();

        for (i4_i = 0; i4_i < NUM_SW_TIMER; i4_i++)
        {
            if (b_sys_td_reserved[i4_i] == TRUE)
            {
                /* ffr  2005-04-29. Do SW system time / date update. */
                ui2_sys_td_msec[i4_i] += (UINT16) (pt_timer_list->ui4_tick_res);
                if (ui2_sys_td_msec[i4_i] >= NUM_MSEC_IN_SEC)
                {
                    t_sys_td_sec[i4_i]++;
                    ui2_sys_td_msec[i4_i] -= NUM_MSEC_IN_SEC;
                }
            }
        }

        /* And cycle through the linked list of timer objects.    */
        /* Remeber that all objects are inserted in chronological */
        /* orser (ealiest first, latest last).                    */

        do
        {
            pt_timer = pt_timer_list->pt_head;

            if ((pt_timer != NULL)                     &&
                (pt_timer->ui4_expire <= ui4_systick))
            {
                timer_remove_timer_obj (pt_timer, TIMER_ST_IDLE);

                if (pt_timer->pf_cb != NULL)
                {
                    i4_ret = handle_get_type_obj_tag (pt_timer->h_timer_hdl, &e_type, &pv_obj, &pv_tag);

                    if (i4_ret == HR_OK && e_type == HT_GROUP_OS_TIMER)
                    {
                        pf_cb = pt_timer->pf_cb;
                        h_timer_hdl = pt_timer->h_timer_hdl;
                        if (pt_timer->e_flags == X_TIMER_FLAG_REPEAT)
                        {
                            if (pt_timer->ui1_status != TIMER_ST_ACTIVE)
                            {
                                timer_insert_timer_obj (pt_timer, pt_timer->ui4_delay);
                            }
                        }
                        x_crit_end(t_crit);
                        (pf_cb) (h_timer_hdl, pv_tag);
                        t_crit = x_crit_start();
                    }
                }
            }
            else
            {
                break;
            }
        } while(TRUE);

        x_crit_end(t_crit);
    }
}


/*----------------------------------------------------------------------------
 * Function: os_get_sys_tick()
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      Middleware tick count.
 *---------------------------------------------------------------------------*/
UINT32
os_get_sys_tick()
{
    return(ui4_systick);
}


/*----------------------------------------------------------------------------
 * Function: os_get_fine_tick()
 *
 * Description: This API is mostly used for profiling and returns the value of
 *              a high speed hardware 32 bit counter.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      Middleware high speed tick counte value.
 *---------------------------------------------------------------------------*/
UINT32 os_get_fine_tick (VOID)
{
    return (x_os_drv_get_fine_tick ());
}


/*----------------------------------------------------------------------------
 * Function: os_reserve_sys_td()
 *
 * Description: This API returns the software system time / date values.
 *
 * Inputs: -
 *
 * Outputs: pui2_index: index to a clock entry.
 *
 * Returns:
 *      OSR_OK: succeed; a valid index in returned in pui2_index.
 *      OSR_INV_ARG: failed; pui2_index is NULL.
 *      OSR_NO_RESOURCE: failed; no more clock entry available.
 *---------------------------------------------------------------------------*/
INT32 os_reserve_sys_td (UINT16 *pui2_index)
{
    INT32 i4_ret, i4_i;
    CRIT_STATE_T    t_crit;

    i4_ret = OSR_INV_ARG;
    if (pui2_index != NULL)
    {
        i4_ret = OSR_NO_RESOURCE;
        t_crit = x_crit_start();
        for (i4_i = 0; i4_i < NUM_SW_TIMER; i4_i++)
        {
            if (b_sys_td_reserved[i4_i] == FALSE)
            {
                b_sys_td_reserved[i4_i] = TRUE;
                b_sys_td_valid[i4_i] = FALSE;
                *(UINT16 *) pui2_index = (UINT16) i4_i;
                i4_ret = OSR_OK;
                break;
            }
        }
        x_crit_end(t_crit);
    }
    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: os_release_sys_td()
 *
 * Description: This API returns the software system time / date values.
 *
 * Inputs: ui2_index: index to a clock entry.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK: succeed; clock via ui2_index is released.
 *      OSR_INV_ARG: failed; ui2_index is invalid
 *---------------------------------------------------------------------------*/
INT32 os_release_sys_td (UINT16 ui2_index)
{
    INT32 i4_ret;
    CRIT_STATE_T    t_crit;

    i4_ret = OSR_INV_ARG;
    if (ui2_index < NUM_SW_TIMER)
    {
        t_crit = x_crit_start();
        b_sys_td_reserved[ui2_index] = FALSE;
        b_sys_td_valid[ui2_index] = FALSE;
        x_crit_end(t_crit);
        i4_ret = OSR_OK;
    }
    return(i4_ret);
}

/*----------------------------------------------------------------------------
 * Function: os_set_sys_td()
 *
 * Description: This API sets the software system time / date counters.
 *
 * Inputs: t_td_sec     Contains the time / date in multiples of seconds.
 *         ui2_td_msec  Contains the number of msecs.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID os_set_sys_td (TIME_T  t_td_sec,
                    UINT16  ui2_td_msec,
                    UINT16  ui2_index)
{
    CRIT_STATE_T t_crit;


    if (ui2_index < NUM_SW_TIMER)
    {
        /* Protect time / date access. */
        t_crit = x_crit_start();

        if (b_sys_td_reserved[ui2_index] == TRUE)
        {
            t_sys_td_sec[ui2_index]    = t_td_sec;
            ui2_sys_td_msec[ui2_index] = ui2_td_msec;

            b_sys_td_valid[ui2_index] = TRUE;
        }

        /* And unprotect this part again. */
        x_crit_end (t_crit);
    }
}


/*----------------------------------------------------------------------------
 * Function: os_get_sys_td()
 *
 * Description: This API returns the software system time / date values.
 *
 * Inputs: -
 *
 * Outputs: pui2_td_msec  Will contains the msec part.
 *
 * Returns: The number of seconds or '0'.
 *---------------------------------------------------------------------------*/
TIME_T os_get_sys_td (UINT16*  pui2_td_msec,
                      UINT16   ui2_index)
{
    TIME_T       t_td_sec;
    CRIT_STATE_T t_crit;


    /* Initialize the return values. */
    t_td_sec = NULL_TIME;

    if (pui2_td_msec != NULL)
    {
        (*pui2_td_msec) = 0;
    }

    if (ui2_index < NUM_SW_TIMER)
    {
        /* Protect time / date access. */
        t_crit = x_crit_start();

        if (b_sys_td_valid[ui2_index])
        {
            t_td_sec = t_sys_td_sec[ui2_index];

            if (pui2_td_msec != NULL)
            {
                (*pui2_td_msec) = ui2_sys_td_msec[ui2_index];
            }
        }

        /* And unprotect this part again. */
        x_crit_end (t_crit);
    }

    return (t_td_sec);
}

/*----------------------------------------------------------------------------
 * Function: x_os_get_sys_tick
 *
 * Description: This API returns the system tick count.
 *
 * Inputs: - None
 *
 * Outputs: None
 *
 * Returns: The system tick value
 *---------------------------------------------------------------------------*/
UINT32 x_os_get_sys_tick (VOID)
{
    return os_get_sys_tick();
}

/*----------------------------------------------------------------------------
 * Function: x_os_get_sys_tick_period
 *
 * Description: This API returns the period (in ms) between system tick.
 *
 * Inputs: - None
 *
 * Outputs: None
 *
 * Returns: The period (in ms) between system tick.
 *---------------------------------------------------------------------------*/
UINT32 x_os_get_sys_tick_period (VOID)
{
    return x_os_drv_get_tick_period();
}

/*----------------------------------------------------------------------------
 * Function: x_os_set_timestamp
 *
 * Description: This API set time stamp toget system boot up time.
 *
 * Inputs: - String tag.
 *
 * Outputs: None
 *
 * Returns: None
 *---------------------------------------------------------------------------*/
VOID x_os_set_timestamp (CHAR *szString)
{
    x_os_drv_set_timestamp(szString);
}

