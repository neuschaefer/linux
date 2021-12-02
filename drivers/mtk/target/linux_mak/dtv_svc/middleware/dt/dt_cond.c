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
 * $RCSfile: dt_cond.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains function to set/get the Datetime lib
 *         system time condition. 
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "_dt.h"
#include "dt/dt_dbg.h"
/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/*
  system time clock condition. Initially, the condition is
  DT_NOT_RUNNING.

  Internally, we use two variables to store the DT running condition and
  DT sync source condition. 

  The first internal condition variable only store if the DT is
  in RUNNING or NOT_RUNNING state.  If the system is in the NOT_RUNNING state,
  then this condition is always returned (e.g., it supercedes the sync
  source condition).  Once the DT switch to RUNNING state, it will never
  go back to NOT_RUNNING state.

  The second internal condition variable stores the DT system time
  synchronization condition.   Once the DT is running, only the sync
  source condition (FREE RUNNING, SYNC_RUNNING, NO_SYNC_SRC, etc) are used.

  Note: externally, application/caller will only see combined running and
  sync source condition.

*/
static DT_COND_T  t_sys_tm_cond  [DT_NB_OF_CLOCKS] = { DT_NOT_RUNNING,
                                                       DT_NOT_RUNNING };

static DT_COND_T  t_sync_src_cond[DT_NB_OF_CLOCKS] = { DT_NO_SYNC_SRC,
                                                       DT_NO_SYNC_SRC } ;


static BOOL       b_sync_with_brdcst_utc  =  TRUE ;

/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */
static INT32 clock_id_to_internal_idx(INT32 i4_clock_id)
{
    INT32  i4_idx = -1;
    
    if ( i4_clock_id == ((INT32)DT_CLOCK_BRDCST_UTC) )
    {
        i4_idx = 0;
    }
    else if ( i4_clock_id == ((INT32)DT_CLOCK_USER_UTC) )
    {
        i4_idx = 1;
    }
    else
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INVALID_CLOCK_ID);
    }
    
    return i4_idx;
}    
    
/*-----------------------------------------------------------------------------
 * Name: dt_set_cond_unprotected
 *
 * Description: This API set the internal system clock and sync source
 *              condition.  Note this function needs to called inside
 *              critical condition for proctection.
 *
 * Inputs:
 *   t_cond     Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING, DT_NOT_RUNNING,
 *              DT_IS_RUNNING, DT_SYNC_DISCONT, DT_NO_SYNC_SRC.
 *
 * Outputs: None
 *
 * Returns:
 *
 *   DTR_WILL_NOTIFY:   If the DT is running, and the sync source condition's value
 *                      has changed, then Datetime lib will need to
 *                      notify the clients.
 *
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 ---------------------------------------------------------------------------*/
static INT32 _dt_set_cond_unproctected (INT32 i4_clock_id, DT_COND_T t_cond)
{
    INT32          i4_rc;
    INT32          i4_clock;
    
    i4_rc = DTR_OK;

    i4_clock=clock_id_to_internal_idx(i4_clock_id);

    if ( t_cond == DT_NOT_RUNNING || t_cond == DT_IS_RUNNING )
    {
        t_sys_tm_cond[i4_clock] = t_cond ;
    }
    else
    {
        if ( t_sync_src_cond[i4_clock] != t_cond )
        {
            /* If the 't_cond' is DT_SYNC_DISCONT, then we
               will convert the condition to DT_SYNC_RUNNING,
               but will return DTR_WILL_NOTIFY so the client
               is notified.
               
               This way if we detect multiple discontinuity
               consectively, we will continue to notify client with
               DT_SYNC_DISCONT.

               Note:  DT_SYNC_DISCONT is only used for notify the
               client, but will not be saved as an internal
               condition.
            */
            if ( t_cond == DT_SYNC_DISCONT )
            {
                t_cond = DT_SYNC_RUNNING;
            }
            t_sync_src_cond[i4_clock] = t_cond;
            i4_rc = DTR_WILL_NOTIFY;

            /*
                   If this clock is being sync but it is in the
                   'not running state' then we also need to change
                   its state to 'running'.

                   This condition can happen if RTC fail to run
                   during at boot up, thus the first time the
                   software clocks are set is when the broadcast
                   tables are received.
            */
            if ( (t_cond == DT_SYNC_RUNNING) &&
                 (t_sys_tm_cond[i4_clock] == DT_NOT_RUNNING) )
            {
                t_sys_tm_cond[i4_clock] = DT_IS_RUNNING;
            }
        }
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: dt_set_cond
 *
 * Description: This API set the internal system clock and sync source
 *              condition
 *
 * Inputs:
 *   t_cond     Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING, DT_NOT_RUNNING,
 *              DT_IS_RUNNING, DT_SYNC_DISCONT, DT_NO_SYNC_SRC.
 *
 * Outputs: None
 *
 * Returns:
 *
 *   DTR_WILL_NOTIFY:   If the DT is running, and the sync source condition's value
 *                      has changed, then Datetime lib will need to
 *                      notify the clients.
 *
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 ---------------------------------------------------------------------------*/
INT32 dt_set_cond (INT32 ui4_clock, DT_COND_T   t_cond)
{
    CRIT_STATE_T   t_state;
    INT32          i4_rc;
    
    t_state=x_crit_start();

    i4_rc = _dt_set_cond_unproctected (ui4_clock, t_cond);
    
    x_crit_end(t_state);
    
    return i4_rc;
}
    
/*--------------------------------------------------------------------------
 * Name: dt_set_utc_and_cond
 *
 * Description -    This API set the system condition and the UTC seconds
 *                  This is an atomic operation
 *
 * Input 
 *
 *   t_sec          The UTC seconds to set the sysetm clock.
 *
 *   ui2_milli_sec  Millisecond to set the system clock.
 *
 *   t_cond         Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *                  DT_NOT_RUNNING 
 *
 * Output -- None.
 *
 * Return
 *
 *   DTR_WILL_NOTIFY:   The condition is set and the value condition has
 *                      has changed, therefore Datetime lib will need to
 *                      notify the clients. 
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 * 
 --------------------------------------------------------------------------*/

INT32 dt_set_utc_and_cond
(
    INT32           ui4_clock,
    TIME_T          t_sec,
    UINT16          ui2_milli_sec,
    DT_COND_T       t_cond
)
{
    CRIT_STATE_T   t_state;
    INT32          i4_rc;

    i4_rc = DTR_OK;
    
    /* Setting the UTC second and system clock condition must be
       atomic
    */
    t_state=x_crit_start();

    i4_rc = _dt_set_cond_unproctected (ui4_clock, t_cond);
    
    os_set_sys_td(t_sec, ui2_milli_sec, (UINT16)ui4_clock);
    
    x_crit_end(t_state);

    /* Setting of Real-Time clock only when this operation is
       setting the user clock condition and time.
       
       Not this operation is done outside the critical section.
    */
    if ( ui4_clock == ((INT32)DT_CLOCK_USER_UTC) )
    {
        dt_rtc_set_time(t_sec);
    }

    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: _dt_get_cond_unprotected
 *
 * Description -    This API get the system condition. If the DT is not running, 
 *                  then DT_NOT_RUNNING condition is returned.  If the DT is
 *                  running, then sync source condition is returned.
 *                  
 *                  This static function must be called inside critical section
 *                  for protection.
 * Input:
 *   None
 * 
 * Output:
 *   None        
 *
 * Return
 *
 *   pointer to variable containing the system clock
 *   condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *   DT_NOT_RUNNING, DT_NO_SYNC_SRC.
 *                  
 * 
 --------------------------------------------------------------------------*/
static DT_COND_T  _dt_get_cond_unprotected
(
    INT32 i4_clock_id
)
{
    DT_COND_T       t_cx;
    INT32           i4_clock;

    i4_clock=clock_id_to_internal_idx(i4_clock_id);
    
    if ( t_sys_tm_cond[i4_clock] == DT_NOT_RUNNING )
    {
        t_cx = t_sys_tm_cond[i4_clock] ;
    }
    else
    {
        t_cx = t_sync_src_cond[i4_clock];
    }
    
    return  t_cx;
}

/*--------------------------------------------------------------------------
 * Name: dt_get_cond
 *
 * Description -    This API get the system condition. If the DT is not running, 
 *                  then DT_NOT_RUNNING condition is returned.  If the DT is
 *                  running, then sync source condition is returned.
 *                  
 *                  This is an atomic operation.
 * Input:
 *   None
 * 
 * Output:
 *
 *   pt_cond         pointer to variable containing the system clock
 *                   condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *                   DT_NOT_RUNNING, DT_NO_SYNC_SRC.
 *
 * Return
 *
 *   DTR_OK         success
 *                  
 * 
 --------------------------------------------------------------------------*/
INT32 dt_get_cond
(
    INT32           ui4_clock,
    DT_COND_T*      pt_cond
)
{
    CRIT_STATE_T    t_state;
    DT_COND_T       t_cx;

    /* Getting the UTC second and system clock condition must be
       atomic
    */
    t_state=x_crit_start();

    t_cx = _dt_get_cond_unprotected(ui4_clock);
    
    x_crit_end(t_state);

    if ( pt_cond != NULL )
    {
        *pt_cond  = t_cx ;
    }
    
    return DTR_OK;
}

/*--------------------------------------------------------------------------
 * Name: dt_get_utc_and_cond
 *
 * Description -    This API get the system condition and the UTC seconds
 *                  This is an atomic operation
 *
 * Input:
 *   None
 * 
 * Output:
 *
 *   pui2_milli_sec  pointer to variable holding the current millisecond
 *                   value.
 *
 *   pt_cond         pointer to variable containing the system clock
 *                   condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *                   DT_NOT_RUNNING 
 *
 * Return
 *
 *   UTC sec         the current UTC seconds.
 *                  
 * 
 --------------------------------------------------------------------------*/

TIME_T dt_get_utc_and_cond
(
    INT32           ui4_clock,
    UINT16*         pui2_milli_sec, 
    DT_COND_T*      pt_cond
)
{
    CRIT_STATE_T    t_state;
    TIME_T          t_utc;
    DT_COND_T       t_cx;

    /* Getting the UTC second and system clock condition must be
       atomic
    */
    t_state=x_crit_start();
    
    t_cx  = _dt_get_cond_unprotected(ui4_clock);
    t_utc = os_get_sys_td(pui2_milli_sec, (UINT16)ui4_clock);
    
    x_crit_end(t_state);

    if ( pt_cond != NULL )
    {
        *pt_cond  = t_cx ;
    }
    return t_utc;
}


/*--------------------------------------------------------------------------
 * Name: x_dt_sync_with_brdcst_utc
 *
 * Description -    This API specifies if the user software clock
 *                  should be sync with the broadcast time clock. 
 *                  
 * Input:
 *   b_sync_flag:    TRUE:  x_dt_get_utc(..) and x_dt_get_brdcst_utc(..)
 *                          will return the same UTC and condition.
 *                   FALSE: x_dt_get_utc(..) and x_dt_get_brdcst_utc(..)
 *                          will return the different UTC and condition.
 * 
 * Output:
 *   None.
 *
 * Return
 *
 *  DTR_OK:          Success.
 *  DTR_FAIL:        Internal error.
 --------------------------------------------------------------------------*/

INT32   x_dt_sync_with_brdcst_utc(BOOL b_sync_flag)
{
    CRIT_STATE_T    t_state;
    
    t_state=x_crit_start();

    b_sync_with_brdcst_utc = b_sync_flag ;

    if ( b_sync_with_brdcst_utc == FALSE )
    {
        /*
          Change the user clock sync cond to FREE_RUNNING.
        */
        t_sync_src_cond[clock_id_to_internal_idx((INT32)DT_CLOCK_USER_UTC)]
            = DT_FREE_RUNNING;
    }
    
    x_crit_end(t_state);
    
    return DTR_OK ;
}

/*--------------------------------------------------------------------------
 * Name: dt_user_clock_sync_with_brdcst_clock
 *
 * Description -    This API get the sync status of the user software clock
 *                  with the broadcast time clock.
 *                  
 * Input:
 *   b_sync_flag:    
 * 
 * Output:
 *   None.
 *
 * Return
 *
 *  TRUE:  user clock and brdcst clock are synchronized.
 *  FALSE: user clock and brdcst clock are not synchronized.
 *
 --------------------------------------------------------------------------*/

BOOL dt_user_clock_sync_with_brdcst_clock(VOID)
{
    CRIT_STATE_T    t_state;
    BOOL            b_result;
    
    t_state=x_crit_start();

    b_result = b_sync_with_brdcst_utc ;
    
    x_crit_end(t_state);

    return b_result ;
}



