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
 * $RCSfile: dt_day_sav_time.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: fb57c46bfb65504baccbada437f6b6c9 $
 *
 * Description: 
 *         This file contains the APIs to set and get the internal
 *         Day-Light-Saving states.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "os/inc/x_os.h"
#include "dt/dt.h"
#include "dt/dt_cli.h"
#include "dt/dt_dbg.h"
#include "dt/dt_timer.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* -------------------------------------------------------------
  Typedefs   
----------------------------------------------------------------- */


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
/*
  Internal state for the current day-light-saving
*/
static DAY_SAV_TIME_T  DT_DAY_SAV_STATE = DS_FALSE ;

/*
  Time of the Day-Light-Saving Time Transition.
*/
static TIME_T         TIME_DS_TRANS_TO_TRUE  = 0;

static TIME_T         TIME_DS_TRANS_TO_FALSE = 0;

/*
  Flag indicating if the Day-Light-Saving control should be applied for
  the current locality.
*/
static BOOL           B_DST_CONTROL = TRUE;


/*
  Day-Ligth-Saving time offset.  In ATSC, this value is default
  to 1 hr. But in DVB, we will compute this value from the local
  offset.
*/
static TIME_T         DT_DST_OFFSET = 3600 ;


/*
  Flag to specify which DST state should be used for the UTC to local
  time conversion.

  For example, if specified UTC time to be coverted to local is a time 
  in the future (e.g., after the DST transition has taken place), but
  currently (e.g., at the time when API is called to make this UTC to
  local conversion), the DST transition has not take place.

  This flag specifies whether to use the current DST state at the time
  when API conversion is called or to use future DST state of the specified
  UTC time.
*/
static UINT32          dt_dst_ctrl = DT_USE_DST_AT_SPECIFIED_TIME ;


/*
  Last effective time sync source table ID
*/
static UINT8           DT_LAST_SYNC_TBL_ID = 0;



/*
    UTC time (in sec) whee DST change will occur.
*/
static TIME_T       DT_DS_CHANGE = 0;

/*
    next local time offset
*/
static TIME_T       DT_DS_OFFSET = 0;



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------
 * Name: dt_timer_cb_fct
 *
 * Description:  x_os_timer_cb_fct function that will be called
 *   by the timer when the Day-Light-Saving transition time
 *   is reached. 
 * 
 * Inputs:
 *   t_tm_handle    handle to the timer.
 *
 *   pv_tag          Client tag.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:  The transition time into next state for Day-Light-Saving
 *            in UTC seconds.
 *
 -------------------------------------------------------------------------*/
static VOID dt_timer_cb_fct
(
    HANDLE_T  t_tm_handle,
    VOID*     pv_tag
)
{
    DT_NFY_MSG_T       t_msg;

    t_msg.t_msg_type = DT_DAY_LGT_SAV_CHANGED_NOTIFICATION;
    t_msg.ui32_val = (UINT32) pv_tag;

    /* Send msg to the DT notification thread. */
    dt_send_nfy_msg(&t_msg);
    
    return;
}
 /*-----------------------------------------------------------------------
 * Name: dt_get_trans_time
 *
 * Description:  This function determine the future Day-Light-Saving
 *               transition time (in UTC seconds).
 * 
 * Inputs:
 *   t_utc_sec       the current time in UTC seconds
 * 
 *   ui1_ds_status   0: standard time is in-effect.
 *                   1: day-light-saving is in-effect.
 *
 *   ui1_ds_day_of_mon  The day which transition into/out-of Day-Light-Saving
 *                      take effect. 
 *
 *   ui1_ds_hr          The hours which transition into/out-of Day-Light-Saving
 *                      take effect.  In ATSC STT table, this hour is
 *                      specified in local time hour.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:  The transition time into next state for Day-Light-Saving
 *            in UTC seconds.
 *
 -------------------------------------------------------------------------*/

static TIME_T   dt_get_trans_time
(
    TIME_T          t_utc_sec,
    UINT8           ui1_ds_status,
    UINT8           ui1_ds_day_of_mon,
    UINT8           ui1_ds_hr
)
{
    TIME_T          t_trans_sec;
    TIME_T          t_local_sec;
    TIME_T          t_dst_offset;
    DTG_T           t_dtg;

    t_dst_offset = dt_get_dst_offset();
    
    /*
      Determine when the transition will occur.
      First we convert the current UTC seconds
      received to local secods.

      Note, we must use the
      DS status flag received from STT to compute the local
      sec of the current time.  Call x_dt_utc_sec_to_local_sec()
      will not be correct. This is because we are trying to determine
      the future transition time, where as x_dt_utc_sec_to_local_sec()
      is using the current transition time setting.
    */
    
    if ( ui1_ds_status > 0 )
    {
        t_local_sec =  t_utc_sec + t_dst_offset + x_dt_get_tz();
    }
    else
    {
        t_local_sec =  t_utc_sec + x_dt_get_tz();
    }

    /*
       Convert local adjusted seconds into DTG structure,
       call the UTC seconds to DTG conversion, and then
       set the b_gmt field to FALSE, because
       this is a local time.
    */
    x_dt_utc_sec_to_dtg(t_local_sec, &t_dtg);
    t_dtg.b_gmt=FALSE;

    /*
      Determine the month (current) or the next month
      when the DS transition will happen in local time.
    */
    if ( ( t_dtg.ui1_day > ui1_ds_day_of_mon ) 
    	#if 0
    	||
         ( (t_dtg.ui1_day == ui1_ds_day_of_mon) && (t_dtg.ui1_hr > ui1_ds_hr) ) ||
         (
           (t_dtg.ui1_day == ui1_ds_day_of_mon) && (t_dtg.ui1_hr == ui1_ds_hr) &&
           ( (t_dtg.ui1_min > 0) || (t_dtg.ui1_sec > 0) )
         )
      #endif
       )
    {
        t_dtg.ui1_mo++;
        if ( t_dtg.ui1_mo > 12 )
        {
            t_dtg.ui1_mo=1;
            t_dtg.ui2_yr++;
        }
    }
    t_dtg.ui1_day = ui1_ds_day_of_mon;
    t_dtg.ui1_hr  = ui1_ds_hr;

    /* Zero out the minute and seconds of the transition time. */
    t_dtg.ui1_min = 0;
    t_dtg.ui1_sec = 0;
    
    /*
      Convert the transition time form yymmddhh back to number of
      local seconds.
    */
    t_local_sec = x_dt_dtg_to_sec(&t_dtg);

    /* Convert the local seconds of the Day-Light-Saving transition
       time back to UTC.
    */
    if (  ui1_ds_status > 0 )
    {
        t_trans_sec = t_local_sec - x_dt_get_tz() - t_dst_offset ;
    }
    else
    {
        t_trans_sec = t_local_sec - x_dt_get_tz()  ;
    }

    DBG_INFO(("{DT} The transition time (local) is: yr: %d mon: %d day: %d hr: %d\n",
               t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day, t_dtg.ui1_hr));

    DBG_INFO(("{DT} The transition time in UTC seconds: %ld\n", t_trans_sec));

#ifdef DEBUG    
    x_dt_utc_sec_to_dtg(t_trans_sec, &t_dtg);

    DBG_INFO(("{DT} The transition time (UTC) is: yr: %d mon: %d day: %d hr: %d\n",
               t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day, t_dtg.ui1_hr));
#endif    
   
    return t_trans_sec;
}

/*-----------------------------------------------------------------------
 * Name: dt_dst_change_nfy
 *
 * Description:  API to set up timer to notify client of the DST
 *   change.
 * 
 * Inputs:
 *   ui4_delay    The time to elapse before notifying the client
 *                of the change in DST status.
 *   
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:  Success
 *
 -------------------------------------------------------------------------*/
INT32 dt_dst_change_nfy
(
    UINT32      ui4_delay,
    VOID*       pv_tag
)
{
    /* Set up time notfication when DST change time is reached. */
    dt_timer_cancel_action();
    dt_timer_reg_action(ui4_delay,
                        dt_timer_cb_fct,
                        pv_tag);
    return DTR_OK;
}

/*-----------------------------------------------------------------------
 * Name: dt_reset_dst_timer_nfy
 *
 * Description:  This function re-set Day-Light-Saving timer notification
 *   due to a jump in the current UTC time.
 *      
 * Inputs:
 *   t_utc_sec     The new current UTC time received from TOT table.
 * 
 *   t_new_cond    If 'DT_SYNC_DISCONT', it means the new current time
 *                 't_utc_sec' setting has jump (+/-) larger than the
 *                 DELTA (~2 sec).
 *
 *
 * Outputs: 
 *   None.
 *
 * Returns:
 *   DTR_OK:  The transition state and time of transition is set.
 *
 -------------------------------------------------------------------------*/
INT32 dt_reset_dst_timer_nfy
(
    TIME_T          t_utc_sec,
    DT_COND_T       t_new_cond
)  
{
    DAY_SAV_TIME_T  current_dst_state ;
    DAY_SAV_TIME_T  future_dst_state;
    TIME_T          tm_ds_trans_to_true ;
    TIME_T          tm_ds_trans_to_false ;
    TIME_T          t_trans_sec = 0;

    CRIT_STATE_T    t_state;
    
    t_state=x_crit_start();
    current_dst_state         = DT_DAY_SAV_STATE ;
    tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
    tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
    x_crit_end(t_state);

    future_dst_state = current_dst_state;
    
    if ( current_dst_state == DS_TRANS_TO_TRUE )
    {
        if ( tm_ds_trans_to_true != 0 )
        {
            t_trans_sec = tm_ds_trans_to_true ;
            future_dst_state = DS_TRUE;
        }
    }

    if ( current_dst_state == DS_TRANS_TO_FALSE )
    {
        if ( tm_ds_trans_to_false != 0 )
        {
            t_trans_sec = tm_ds_trans_to_false ;
            future_dst_state = DS_FALSE;
        }
    }
    
    if ( t_trans_sec != 0 )
    {
        UINT32   ui4_delay;
        
        /* Set up time notfication when DST change time is reached. */
        if ( t_trans_sec > t_utc_sec )
        {   
            ui4_delay = (UINT32)(t_trans_sec - t_utc_sec) ;
        }
        else
        {
            ui4_delay = 0;
        }
        dt_dst_change_nfy(ui4_delay, (VOID*) future_dst_state);
    }
    return DTR_OK;
}

#ifdef LINUX_TURNKEY_SOLUTION
 
 VOID dt_set_linux_tz(VOID)
 {
     TIME_T    t_tz_offset;
     TIME_T    t_dst_offset;
     BOOL      b_dst_state;

     struct timeval  tv;
     struct timezone tz; 

     gettimeofday(&tv,&tz);
     
     t_tz_offset = x_dt_get_tz();
     t_tz_offset = -t_tz_offset;
     b_dst_state = x_dt_get_dst();
 
     if(b_dst_state==TRUE)
     {
         t_dst_offset = dt_get_dst_offset();
         t_tz_offset -= t_dst_offset;
     }
         
     tz.tz_minuteswest = (int)(t_tz_offset/60);
     tz.tz_dsttime = 0;
     settimeofday(&tv,&tz);
     
     return;
 }
 
#endif

 /*-----------------------------------------------------------------------
 * Name: dt_set_day_sav_state
 *
 * Description:  This function determine the current Day-Light-Saving state
 *               (FALSE, TRUE, TRANSITIION to TRUE, and TRANSITION to FALSE.
 * 
 * Inputs:
 *   t_utc_sec       the current time in UTC seconds
 * 
 *   ui1_ds_status   0: standard time is in-effect.
 *                   1: day-light-saving is in-effect.
 *
 *   ui1_ds_day_of_mon  The day which transition into/out-of Day-Light-Saving
 *                      take effect.
 *
 *   ui1_ds_hr          The hours which transition into/out-of Day-Light-Saving
 *                      take effect. This hours is specified in local time.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:  The transition state and time of transition is set.
 *
 -------------------------------------------------------------------------*/
INT32 dt_set_day_sav_state
(
    TIME_T          t_utc_sec,
    UINT8           ui1_ds_status,
    UINT8           ui1_ds_day_of_mon,
    UINT8           ui1_ds_hr
)  
{
    DAY_SAV_TIME_T  old_dst_state = DT_DAY_SAV_STATE ;
    TIME_T          tm_ds_trans_to_true = 0 ;
    TIME_T          tm_ds_trans_to_false = 0 ;

    DAY_SAV_TIME_T  t_curr_dst_state = DT_DAY_SAV_STATE ;
    TIME_T          t_curr_tm_ds_trans_to_true = 0 ;
    TIME_T          t_curr_tm_ds_trans_to_false = 0 ;
    
    TIME_T          t_trans_sec = 0;
    CRIT_STATE_T    t_state;

    if ( ui1_ds_status     == 0        &&
         ui1_ds_day_of_mon == 0       &&
         ui1_ds_hr         == 0
        )
    {
        DBG_INFO(("{DT}--------------------------------------------\n"));
        DBG_INFO(("{DT} Current UTC second received is %lld\n", t_utc_sec));
        DBG_INFO(("{DT} Day Saving is FALSE, no transition time to TRUE received from the stream.\n"));
        DBG_INFO(("{DT}--------------------------------------------\n"));
                 
        t_state=x_crit_start();

        /* save the DST current state information. */
        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;

        
        DT_DAY_SAV_STATE = DS_FALSE ;
        TIME_DS_TRANS_TO_TRUE  = 0;
        TIME_DS_TRANS_TO_FALSE = 0; /* Reset trans_to_false to 0. This
                                       is needed in case another prior
                                       channel has set this value to
                                       other than 0
                                    */
        
        /* save the new current DST state. */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    if ( ui1_ds_status      ==  0      &&
         ui1_ds_day_of_mon  >   0      &&
         ui1_ds_hr          >   0
        )
    {

        DBG_INFO(("{DT} -------------------------------------------- \n"));
        DBG_INFO(("{DT} Current UTC second received is %lld\n", t_utc_sec));
        DBG_INFO(("{DT} Day Saving is in transition to TRUE\n"));
         
        t_trans_sec = dt_get_trans_time(t_utc_sec, ui1_ds_status, ui1_ds_day_of_mon, ui1_ds_hr);
                 
        DBG_INFO(("{DT} -------------------------------------------- \n"));
          
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;

        #if 0
        DT_DAY_SAV_STATE = DS_TRANS_TO_TRUE ;
        TIME_DS_TRANS_TO_TRUE  = t_trans_sec ;
        #else
        if(t_trans_sec > t_utc_sec) 
        {
            DT_DAY_SAV_STATE = DS_TRANS_TO_TRUE ;
            TIME_DS_TRANS_TO_TRUE = t_trans_sec; 
        }
        else
        {
            DT_DAY_SAV_STATE = DS_TRUE ;
            TIME_DS_TRANS_TO_TRUE = 0 ;
        }
        #endif
        TIME_DS_TRANS_TO_FALSE = 0; /* Reset trans_to_false to 0. This
                                       is needed in case another prior
                                       channel has set this value to
                                       other than 0.
                                    */
        
        /* save the new current DST state. */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    if ( ui1_ds_status     > 0        &&
         ui1_ds_day_of_mon == 0       &&
         ui1_ds_hr         == 0
        )
    {

        DBG_INFO(("{DT} -------------------------------------------- \n"));
        DBG_INFO(("{DT} Current UTC second received is %lld\n", t_utc_sec));
        DBG_INFO(("{DT} Day Saving is TRUE, no transition time to FALSE received from the stream.\n"));
        DBG_INFO(("{DT} -------------------------------------------- \n"));

        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
        
        DT_DAY_SAV_STATE = DS_TRUE ;
        TIME_DS_TRANS_TO_FALSE = 0;
        TIME_DS_TRANS_TO_TRUE  = 0;  /*  
                                       Note:
                                       Need to reset trans_to_true time to 0.
                                       If this is not done, if a prior channel
                                       set this value to other than 0,
                                       then it can affect DS status if wrong
                                       data are sent from other channel.
                                     */
        
        /* save the new current DST state. */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    if ( ui1_ds_status      >   0      &&
         ui1_ds_day_of_mon  >   0      &&
         ui1_ds_hr          >   0
        )
    {
           
        DBG_INFO(("{DT} ------------------------------------------------ \n"));
        DBG_INFO(("{DT} Current UTC second received is %lld\n", t_utc_sec));
        DBG_INFO(("{DT} Day Saving is in transition to FALSE\n"));
        
        t_trans_sec = dt_get_trans_time(t_utc_sec, ui1_ds_status, ui1_ds_day_of_mon, ui1_ds_hr);
                 
        DBG_INFO(("{DT} ---------------------------------------------------- \n"));
        
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
        
        DT_DAY_SAV_STATE = DS_TRANS_TO_FALSE ;
        TIME_DS_TRANS_TO_FALSE = t_trans_sec;
        TIME_DS_TRANS_TO_TRUE  = 0; /*  
                                       Note:
                                       Need to reset trans_to_true time to 0.
                                       If this is not done, if a prior channel
                                       set this value to other than 0,
                                       then it can affect DS status if wrong
                                       data are sent from other channel.
                                     */

        /* save the new current DST state. */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }    

    /*  Check if the DST state has changed. If the DST state has changed,
        e.g., DST is on or off, or the time of transition changed.

        If there is a change, then set up the notification timer.
    */
    if ( (old_dst_state != t_curr_dst_state)   ||
         (tm_ds_trans_to_true != t_curr_tm_ds_trans_to_true) ||
         (tm_ds_trans_to_false != t_curr_tm_ds_trans_to_false)
        )
    {
        UINT32           ui4_delay;
        DAY_SAV_TIME_T   t_future_dst_state = t_curr_dst_state;
        
        
        /* Set up time notfication when DST change time is reached. */
        if ( t_trans_sec > t_utc_sec )
        {   
            ui4_delay = (UINT32)(t_trans_sec - t_utc_sec) ;
            
            if ( (t_curr_dst_state == DS_TRANS_TO_TRUE)  ||
                 (t_curr_dst_state == DS_FALSE)
                )
            {
                /* next state when the notification occure is: */
                t_future_dst_state = DS_TRUE;
            }

            if ( (t_curr_dst_state == DS_TRANS_TO_FALSE)   ||
                 (t_curr_dst_state == DS_TRUE)
                )
            {
                t_future_dst_state = DS_FALSE;
            }

            dt_dst_change_nfy(ui4_delay, (VOID*) t_future_dst_state);
        }
        else
        {
            dt_dst_change_nfy(0, (VOID*) t_curr_dst_state);
        }
    }
    
    return DTR_OK;
}


/*-----------------------------------------------------------------------
 * Name: dt_set_day_sav_state_from_tot
 *
 * Description:  This function set Day-Light-Saving state from the
 *               TOT data.
 *               (TRANSITIION to TRUE, and TRANSITION to FALSE)
 * Inputs:
 *   t_utc_sec     The new current UTC time received from TOT table.
 * 
 *   ui1_ds_status   0: standard time is in-effect.
 *                   1: day-light-saving is in-effect.
 *
 *   t_trans_sec   The time which transition into/out-of Day-Light-Saving
 *
 *   t_new_cond    If 'DT_SYNC_DISCONT', it means the new current time
 *                 't_utc_sec' setting has jump (+/-) larger than the
 *                 DELTA (~2 sec).
 *
 *
 * Outputs: 
 *   None.
 *
 * Returns:
 *   DTR_OK:  The transition state and time of transition is set.
 *
 -------------------------------------------------------------------------*/
INT32 dt_set_day_sav_state_from_tot
(
    TIME_T          t_utc_sec,
    UINT8           ui1_ds_status,
    TIME_T          t_trans_sec,
    TIME_T          t_ds_offset,
    DT_COND_T       t_new_cond
)  
{
    DAY_SAV_TIME_T  old_dst_state = DT_DAY_SAV_STATE ;
    TIME_T          tm_ds_trans_to_true = 0 ;
    TIME_T          tm_ds_trans_to_false = 0 ;

    DAY_SAV_TIME_T  t_curr_dst_state = DT_DAY_SAV_STATE ;
    TIME_T          t_curr_tm_ds_trans_to_true = 0 ;
    TIME_T          t_curr_tm_ds_trans_to_false = 0 ;

    CRIT_STATE_T    t_state;
    BOOL            b_ds_offset_changed;

    TIME_T          t_old_offset = dt_get_dst_offset();
    /*
       For DVB TOT only, this is to handle the cases where
       the current and next local time offset value in TOT table are
       other than 1 hrs.  (Nordig certification)

       The reason for this is that in normal TOT processing, we record
       the change (time) for current and next local time offset. We record
       this event as change of DST setting, and assume the offset due to
       Day-Light-Saving is 1 hrs.  In Nordig test case, the difference
       between current and future local time offset is not 1 hrs. Therefore
       this cause an issue.  As work around, we reset the offset due to Day-Light-
       Saving to the difference between the TOT current and next local time offset.

    */
    dt_set_dst_offset(t_ds_offset);
    DT_DS_CHANGE = t_trans_sec;
    DT_DS_OFFSET = t_ds_offset;
    
    b_ds_offset_changed = (t_old_offset != t_ds_offset);

    if ( (ui1_ds_status == 0) && (t_trans_sec > t_utc_sec) )
    {

        DBG_INFO(("{DT} -------------------------------------------- \n"));
        DBG_INFO(("{DT} Day Saving is in transition to TRUE\n"));
        DBG_INFO(("{DT} -------------------------------------------- \n"));
          
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
    
        DT_DAY_SAV_STATE = DS_TRANS_TO_TRUE ;
        TIME_DS_TRANS_TO_TRUE  = t_trans_sec ;
        TIME_DS_TRANS_TO_FALSE = 0; /* Reset trans_to_false to 0. This
                                       is needed in case another prior
                                       channel has set this value to
                                       other than 0.
                                    */
        
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    if ( (ui1_ds_status == 0) && (t_trans_sec <= t_utc_sec) )
    {

        DBG_INFO(("{DT} -------------------------------------------- \n"));
        DBG_INFO(("{DT} Day Saving is FALSE\n"));
        DBG_INFO(("{DT} -------------------------------------------- \n"));
          
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
    
        DT_DAY_SAV_STATE = DS_FALSE ;
        TIME_DS_TRANS_TO_FALSE= t_trans_sec ;
        TIME_DS_TRANS_TO_TRUE= 0; /* Reset trans_to_true to 0. This
                                       is needed in case another prior
                                       channel has set this value to
                                       other than 0.
                                    */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }
    
    if ( (ui1_ds_status > 0) && (t_trans_sec > t_utc_sec) )
    {
           
        DBG_INFO(("{DT} ------------------------------------------------ \n"));
        DBG_INFO(("{DT} Day Saving is in transition to FALSE\n"));
        DBG_INFO(("{DT} ---------------------------------------------------- \n"));
        
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
        
        DT_DAY_SAV_STATE = DS_TRANS_TO_FALSE ;
        TIME_DS_TRANS_TO_FALSE = t_trans_sec;
        TIME_DS_TRANS_TO_TRUE  = 0; /*  
                                       Note:
                                       Need to reset trans_to_true time to 0.
                                       If this is not done, if a prior channel
                                       set this value to other than 0,
                                       then it can affect DS status if wrong
                                       data are sent from other channel.
                                     */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    if ( (ui1_ds_status > 0) && (t_trans_sec <= t_utc_sec) )
    {
        DBG_INFO(("{DT} ------------------------------------------------ \n"));
        DBG_INFO(("{DT} Day Saving is TRUE\n"));
        DBG_INFO(("{DT} ---------------------------------------------------- \n"));
        
        t_state=x_crit_start();

        old_dst_state         = DT_DAY_SAV_STATE ;
        tm_ds_trans_to_true   = TIME_DS_TRANS_TO_TRUE ;
        tm_ds_trans_to_false  = TIME_DS_TRANS_TO_FALSE ;
        
        DT_DAY_SAV_STATE = DS_TRUE ;
        TIME_DS_TRANS_TO_TRUE= t_trans_sec;
        TIME_DS_TRANS_TO_FALSE= 0; /*  
                                       Note:
                                       Need to reset trans_to_false time to 0.
                                       If this is not done, if a prior channel
                                       set this value to other than 0,
                                       then it can affect DS status if wrong
                                       data are sent from other channel.
                                     */
        t_curr_dst_state =  DT_DAY_SAV_STATE;
        t_curr_tm_ds_trans_to_true = TIME_DS_TRANS_TO_TRUE;
        t_curr_tm_ds_trans_to_false = TIME_DS_TRANS_TO_FALSE;
        
        x_crit_end(t_state);
    }

    /*  Check if the DST state has changed. If the DST state has changed,
        e.g., DST is on or off, or the time of transition changed.

        If there is a change, then set up the notification timer.
    */
    if ( (old_dst_state != t_curr_dst_state)   ||
         (tm_ds_trans_to_true != t_curr_tm_ds_trans_to_true) ||
         (tm_ds_trans_to_false != t_curr_tm_ds_trans_to_false)
        )
    {
        UINT32           ui4_delay;
        DAY_SAV_TIME_T   t_future_dst_state = t_curr_dst_state;
        
        
        /* Set up time notfication when DST change time is reached. */
        if ( t_trans_sec > t_utc_sec )
        {   
            ui4_delay = (UINT32)(t_trans_sec - t_utc_sec) ;
            
            if ( (t_curr_dst_state == DS_TRANS_TO_TRUE)  ||
                 (t_curr_dst_state == DS_FALSE)
                )
            {
                /* next state when the notification occure is: */
                t_future_dst_state = DS_TRUE;
            }

            if ( (t_curr_dst_state == DS_TRANS_TO_FALSE)   ||
                 (t_curr_dst_state == DS_TRUE)
                )
            {
                t_future_dst_state = DS_FALSE;
            }

            dt_dst_change_nfy(ui4_delay, (VOID*) t_future_dst_state);
        }
        else if ((old_dst_state != t_curr_dst_state) || b_ds_offset_changed)
        {
            dt_dst_change_nfy(0, (VOID*) t_curr_dst_state);
        }
    }
    
    return DTR_OK;
}

/*---------------------------------------------------------------------
 *
 * Name: dt_get_dst_stat_for_utc_sec
 *
 * Description: Get the Day-Light-Saving status for a specified UTC time.
 *              If the Day-Light-Saving control is FALSE, then
 *              it implies for the current locality, the day-light-saving
 *              should not be applied.
 *
 * Input:
 *     t_utc_sec:  Number of seconds since epoch (0000Z, 1 Jan, 1970)
 *
 * Output:
 *     None
 *
 * Return:
 *     TRUE:  If the specified UTC time falls into DST time period.
 *     FALSE: If the specified UTC time do not fall into DST time period.
----------------------------------------------------------------------*/

BOOL dt_get_dst_stat_for_utc_sec(TIME_T  t_utc_sec)
{
    DAY_SAV_TIME_T   t_dst_info;
    TIME_T           t_utc_time_to_ds_true;
    TIME_T           t_utc_time_to_ds_false;
    CRIT_STATE_T     t_state;
    BOOL             b_result;
    UINT32           ui4_flag;
    

    t_state=x_crit_start();
    t_dst_info                  = DT_DAY_SAV_STATE ;
    t_utc_time_to_ds_true       = TIME_DS_TRANS_TO_TRUE ;
    t_utc_time_to_ds_false      = TIME_DS_TRANS_TO_FALSE ;
    ui4_flag                    = dt_dst_ctrl;
    x_crit_end(t_state);

    if ( ui4_flag == DT_USE_DST_AT_CURRENT_TIME )
    {
        switch (t_dst_info)
        {
            case DS_TRUE:
            case DS_TRANS_TO_FALSE:    
            {
                b_result = TRUE ;
            }
            break;

            case DS_FALSE:
            case DS_TRANS_TO_TRUE:
            {
                b_result = FALSE;
            }
            break;

            default:
            {
                b_result = FALSE;
            }
            break;   
        }
    }      
    else
    {
        switch (t_dst_info)
        {
            case DS_TRUE:
            {
                /*
                  check if the UTC time is before
                  the transition out of Day-Light-Saving.
                */
                if ( t_utc_sec < t_utc_time_to_ds_true )
                {
                    b_result = FALSE;
                }
                else
                {
                    b_result = TRUE;
                }
            }
            break;

            case DS_TRANS_TO_FALSE:
            {
                if ( t_utc_sec < t_utc_time_to_ds_false )
                {
                    b_result = TRUE;
                }
                else
                {
                    b_result = FALSE;
                }
            }
            break;

            case DS_FALSE:
            {
                if ( t_utc_sec < t_utc_time_to_ds_false )
                {
                    b_result = TRUE;
                }
                else
                {
                    b_result = FALSE;
                }
            }
            break;

            case DS_TRANS_TO_TRUE:
            {
                if ( t_utc_sec < t_utc_time_to_ds_true )
                {
                    b_result = FALSE;
                }
                else
                {
                    b_result = TRUE;
                }
            }
            break;
        
            default:
            {
                b_result = FALSE;
            }
            break;
        }
        #if 0
        if (t_utc_sec <= 1238288400L)
        {
            x_dbg_stmt("{DT}: dst stat for UTC %lld is %d, t_dst_info: %d, trans time: 2T: %lld, 2F:%lld\r\n",
                t_utc_sec, b_result, t_dst_info, t_utc_time_to_ds_true, t_utc_time_to_ds_false);
    }
        #endif
            
    }
    
    return (b_result);
}

/*--------------------------------------------------------------------
 *
 * Name:  x_dt_set_dst
 *
 * Description: This API allow user to manually set/unset the current
 *              status of Daylight-Saving-Time.
 *        
 *              When manually setting the Day-Light-Saving status,
 *              User don't know the the transition times ('transition
 *              to false' and 'transition to true'),  so we will set
 *              both transition time to 0.  Therefore, the API
 *              dt_get_dst_stat_for_utc_sec() will always returns TRUE
 *              when day-light-saving is on and FALSE when day-light-saving
 *              is off, for any input UTC time greater than 0. 
 *              
 * Inputs:
 *      b_dls: Flag to enable/disable DST.
 *
 *                TRUE   enable Daylight-Saving-Time.
 *                FALSE  disable Daylight-Saving-Time.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *     VOID
-----------------------------------------------------------------------*/
VOID x_dt_set_dst(BOOL b_dls)
{
    CRIT_STATE_T   t_state;
    DAY_SAV_TIME_T  t_curr_dst_state ; 

    t_state=x_crit_start();
    if ( b_dls == TRUE )
    {
        t_curr_dst_state = DS_TRUE ;
    }
    else
    {
        t_curr_dst_state = DS_FALSE ;
    }
    TIME_DS_TRANS_TO_FALSE = 0;
    TIME_DS_TRANS_TO_TRUE  = 0;
    x_crit_end(t_state);
    
#ifdef LINUX_TURNKEY_SOLUTION
    dt_set_linux_tz();
#endif

    //For dst control is off,also send MSG DT_DAY_LGT_SAV_CHANGED_NOTIFICATION 
    if (x_dt_get_dst_ctrl() == FALSE) 
    {
        dt_dst_change_nfy(0, (VOID*) t_curr_dst_state);
    }
    
    return;
}

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_dst
 *
 * Description: API to get the current status of Daylight-Saving-Time.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns: 
 *      TRUE   Daylight-Saving-Time on.
 *      FALSE  Daylight-Saving-Time off.
------------------------------------------------------------------------*/
BOOL x_dt_get_dst(VOID)
{
    BOOL   b_dst_setting;
    CRIT_STATE_T   t_state;

    t_state=x_crit_start();
    if ( DT_DAY_SAV_STATE == DS_TRUE   ||
         DT_DAY_SAV_STATE == DS_TRANS_TO_FALSE
        )
    {
        b_dst_setting = TRUE;
    }
    else
    {
        b_dst_setting = FALSE;
    }
    x_crit_end(t_state);
    return b_dst_setting;
}

/*--------------------------------------------------------------------
 *
 * Name:  x_dt_set_dst_ctrl
 *
 * Description: Set/Unset if Daylight-Saving-Time should be apply to
 *              this locality. In some states (Arizona, Hawaii, and
 *              Indiana) the Day-Light-Saving is not used.  Thus for
 *              these locality, the DST control should be set to
 *              FALSE, and DST set to false (x_dt_set_dst(FALSE))
 *              so that any daylight-saving parameters received
 *              from the broadcast stream will be ignored.
 *
 * Inputs:
 *      b_dls: Flag to enable/disable DST.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *     TRUE   enable Daylight-Saving-Time control.
 *     FALSE  disable Daylight-Saving-Time control.
-----------------------------------------------------------------------*/
VOID x_dt_set_dst_ctrl(BOOL b_dls)
{
    CRIT_STATE_T   t_state;

    t_state=x_crit_start();
    B_DST_CONTROL = b_dls;
    x_crit_end(t_state);    
    return;
}

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_dst_ctrl
 *
 * Description: Get the flag indicating if the Daylight-Saving-Time
 *              receive from the broadcast table should
 *              be applied for this locality.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns: 
 *      TRUE   Daylight-Saving-Time control is on for this location.
 *      FALSE  Daylight-Saving-Time control is off for this location.
------------------------------------------------------------------------*/
BOOL x_dt_get_dst_ctrl(VOID)
{
    CRIT_STATE_T   t_state;
    BOOL           b_dst_ctrl_setting;
    
    t_state=x_crit_start();
    b_dst_ctrl_setting = B_DST_CONTROL ;
    x_crit_end(t_state);
    return b_dst_ctrl_setting;
}

VOID dt_set_ds_info(TIME_T t_ds_change,TIME_T t_ds_offset)
{
    dt_set_day_sav_state_from_tot(x_dt_get_utc(NULL, NULL),
                                  (DT_DAY_SAV_STATE == DS_TRUE || DT_DAY_SAV_STATE == DS_TRANS_TO_FALSE),
                                  DT_DS_CHANGE,
                                  DT_DS_OFFSET,
                                  0);
}

TIME_T x_dt_get_ds_change(VOID)
{
    return DT_DS_CHANGE;
}

VOID x_dt_set_ds_change(TIME_T t_ds_change)
{
    DT_DS_CHANGE = t_ds_change;
    dt_set_ds_info(DT_DS_CHANGE,DT_DS_OFFSET);
    return;
}



TIME_T x_dt_get_ds_offset(VOID)
{
    return DT_DS_OFFSET;
}

VOID x_dt_set_ds_offset(TIME_T t_ds_offset)
{
    DT_DS_OFFSET = t_ds_offset;
    dt_set_ds_info(DT_DS_CHANGE,DT_DS_OFFSET);
    return;
}



#if 0
/*---------------------------------------------------------------------
 *
 * Name: x_dt_utc_sec_to_local_sec
 *
 * Description: Convert UTC seconds to local seconds, take into account
 *              of the timezone difference and Day-Light-Saving.
 *
 *               local_sec = utc_sec + time_zone + 3600 (if DST is in-effect)
 *
 * Input:
 *     t_utc_sec:  Number of UTC seconds since epoch (0000Z, 1 Jan, 1970)
 *
 * Output:
 *     None
 *
 * Return:
 *     The number of seconds since UTC epoch (0000Z, 01 Jan 1970), adjusted
 *     for timezone and Day-Light saving.
----------------------------------------------------------------------*/

TIME_T x_dt_utc_sec_to_local_sec(TIME_T  t_utc_sec)
{
    TIME_T   t_dst_offset;

    t_dst_offset = dt_get_dst_offset();

    if ( dt_get_dst_stat_for_utc_sec(t_utc_sec) == TRUE )
    {
        t_utc_sec = t_utc_sec +  t_dst_offset ;
    }

    /* Adjust for the local time zone.
       Do this last, because we have already modified value of
       local variable 't_utc_sec'.
    */
    t_utc_sec = t_utc_sec + x_dt_get_tz();
    
    return (t_utc_sec);
}
#endif

/*---------------------------------------------------------------------
 *
 * Name: dt_get_dst_offset
 *
 * Description: Get the offset to use if the Day-Light-Saving is
 *              in effect. For ATSC, this value is 1 hrs. But for
 *              DVB we will compute this value from the local offset.
 * Input:
 *     None
 *
 * Output:
 *     None
 *
 * Return:
 *     The DST offset in seconds.
 *     
----------------------------------------------------------------------*/

TIME_T dt_get_dst_offset(VOID)
{
    CRIT_STATE_T   t_state;
    TIME_T         t_offset;
    
    t_state=x_crit_start();
    t_offset = DT_DST_OFFSET ;
    x_crit_end(t_state);

    return t_offset;
}

/*---------------------------------------------------------------------
 *
 * Name: dt_set_dst_offset
 *
 * Description: Set the offset to use if the Day-Light-Saving is
 *              in effect. For ATSC, this value is 1 hrs. But for
 *              DVB we will compute this value from the local offset.
 * Input:
 *     None
 *
 * Output:
 *     None
 *
 * Return:
 *     The DST offset in seconds.
 *     
----------------------------------------------------------------------*/

TIME_T dt_set_dst_offset(TIME_T   t_offset)
{
    CRIT_STATE_T   t_state;
    
    t_state=x_crit_start();
    DT_DST_OFFSET = t_offset ;
    x_crit_end(t_state);

    return t_offset;
}

/*---------------------------------------------------------------------
 *
 * Name: dt_set_dst_config
 *
 * Description: Set the offset to use if the Day-Light-Saving is
 *              in effect. For ATSC, this value is 1 hrs. But for
 *              DVB we will compute this value from the local offset.
 * Input:
 *     None
 *
 * Output:
 *     None
 *
 * Return:
 *     The DST offset in seconds.
 *     
----------------------------------------------------------------------*/

INT32 dt_set_dst_config(UINT32   ui4_flag)
{
    CRIT_STATE_T   t_state;
    
    t_state=x_crit_start();
    
    dt_dst_ctrl = ui4_flag;
    
    x_crit_end(t_state);

    return DTR_OK;
}


/*-----------------------------------------------------------------------
 *
 * Name: dt_update_dst
 *
 * Description: internal API to set the current status of Daylight-Saving-Time.
 *   This function is called when timer notify us that there is a DST change.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns: 
 *      TRUE   Daylight-Saving-Time on.
 *      FALSE  Daylight-Saving-Time off.
------------------------------------------------------------------------*/
BOOL dt_update_dst(DAY_SAV_TIME_T t_new_dst)
{
    CRIT_STATE_T   t_state;
    BOOL           b_updated = FALSE;
    t_state=x_crit_start();
    if ( DT_DAY_SAV_STATE != t_new_dst )
    {
        DT_DAY_SAV_STATE = t_new_dst;
        b_updated = TRUE;
    }
    x_crit_end(t_state);
#ifdef LINUX_TURNKEY_SOLUTION
    dt_set_linux_tz();
#endif
    if(b_updated==TRUE)
    {
         return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*---------------------------------------------------------------------
 *
 * Name: x_dt_get_last_sync_tbl_id
 *
 * Description: Get the last effective table ID used for time sync'ing.
 *              For ATSC, result may be STT (in/out-band); for DVB, 
 *              result may be TDT or TOT; for analog, result may be VBI
 *              teletext. If DT never sync'ed, return zero.
 * Input:
 *     None
 *
 * Output:
 *     None
 *
 * Return:
 *     DT_STT_IN_BAND_TBL_ID        -   STT (in-band)
 *     DT_STT_OUT_OF_BAND_TBL_ID    -   STT (out-band)
 *     DT_DVB_TDT_TBL_ID            -   TDT
 *     DT_DVB_TOT_TBL_ID            -   TOT
 *     DT_VBI_TELETEXT_TBL_ID       -   VBI Teletext
 *     
----------------------------------------------------------------------*/

UINT8 x_dt_get_last_sync_tbl_id(VOID)
{
    CRIT_STATE_T   t_state;
    UINT8          ui1_tbl_id;
    
    t_state=x_crit_start();
    ui1_tbl_id = DT_LAST_SYNC_TBL_ID ;
    x_crit_end(t_state);

    return ui1_tbl_id;
}

/*---------------------------------------------------------------------
 *
 * Name: dt_set_last_sync_tbl_id
 *
 * Description: Set the last effective table ID used for time sync'ing.
 *
 * Input:
 *     ui1_tbl_id       -       Table ID used for time sync'ing.
 *
 * Output:
 *     None
 *
 * Return:
 *     None
 *     
----------------------------------------------------------------------*/

VOID dt_set_last_sync_tbl_id(UINT8  ui1_tbl_id)
{
    CRIT_STATE_T   t_state;
    
    t_state=x_crit_start();
    DT_LAST_SYNC_TBL_ID = ui1_tbl_id ;
    x_crit_end(t_state);
}

