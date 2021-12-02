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
 * $RCSfile: dt_chk_input_utc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *   This file contains APIs to perform gross error check on
 *   nput UTC values received from the stream.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dt/x_dt.h"
#include "dbg/x_dbg.h"
#include "dt/_dt.h"

#include "dt/dt_cli.h"
#include "dt/dt_dbg.h"

/* Internal Input time check-engine. */

static UINT32  ui4_nb_consecut_good_val  = 0;
static UINT32  ui4_nb_consecut_val_to_chk = 1; /* configure-able by the client. */
static TIME_T  t_time_delta_adjustment = 2;    /* configure-able by the client. */
static TIME_T  t_last_txt_utc = 0;
static TIME_T  t_last_sys_clock_utc = 0;
static BOOL    b_check_input_time = FALSE;
static BOOL    b_check_state_started = FALSE;

/*
  The gross error check value for the receive table time value
  (STT, TOT, or TDT).  Time older than this time is rejected.
*/
static TIME_T  t_reject_brdcst_tm_before_this_tm = 0;  /* configure-able by the client. */
static TIME_T  t_last_stt_time = 0;


/*----------------------------------------------------------------------
 *
 * Name:  dt_reset_check_input_time
 *
 * Description:
 *   Reset the internal check engine to its initial state before
 *   receiving the first input time value.
 *
 * Inputs: VOID
 *
 * Outputs: VOID
 *
 * Returns: 
 *   DTR_OK
------------------------------------------------------------------------*/
INT32 dt_reset_check_input_time(VOID)
{
    /* reset all parameters. */
    b_check_state_started = FALSE;
    ui4_nb_consecut_good_val  = 0;
    t_last_txt_utc = 0;
    t_last_sys_clock_utc = 0;
    
    return DTR_OK;
}

INT32 dt_reset_last_stt_time(VOID)
{
    t_last_stt_time  = 0;   
    return DTR_OK;
}


/*----------------------------------------------------------------------
 *
 * Name:   x_dt_check_input_time
 *
 * Description:
 *   enable or dis-able the check input function.
 *  
 * Inputs:
 *   b_enable:   TRUE: enable checking of input time.
 *               FALSE: dis-able checking of input time. (Every time
 *               value received are accepted.)
 *
 * Outputs: VOID
 *
 * Returns: 
 *   DTR_OK
------------------------------------------------------------------------*/
INT32 x_dt_check_input_time(BOOL b_enable)
{
    if ( b_enable == TRUE )
    {
        b_check_input_time = TRUE;
    }
    else
    {
        b_check_input_time = FALSE;
        
    }
    /* reset all parameters. */
    return dt_reset_check_input_time();
}

/*----------------------------------------------------------------------
 *
 * Name:   x_dt_config_check_input_time
 *
 * Description:
 *   Configure the check time engine. 
 *  
 * Inputs:
 *   e_set_parm:   Specifies the parameter to set in the next argument:
 *
 *                 DT_USE_DEFAULT_CONFIG:  revert to default parameters.
 *
 *                 DT_SET_CONSECUTIVE_VAL_TO_CHK: specifies number of
 *                 of consecutive good input value to be processed before
 *                 accepting the next good input time value.
 *               
 *                 DT_SET_TIME_WINDOW_ADJUSTMENT: specifies the delta value
 *                 (tolerance) between the time difference between the current
 *                 and previous one received for the input time, and the 
 *                 time difference between the current and last reading of
 *                 system time values.
 *
 *   pv_value:     Pointer to the variable containg value to set.
 *                 For DT_SET_CONSECUTIVE_VAL_TO_CHK and
 *                 DT_SET_TIME_WINDOW_ADJUSTMENT, this pointer
 *                 refer to a 'INT32' variable containing the set values.
 *
 *                 For DT_USE_DEFAULT_CONFIG, this pointer is ignored.
 *
 * Outputs: VOID
 *
 * Returns: 
 *   DTR_OK
------------------------------------------------------------------------*/
INT32 x_dt_config_check_input_time
(
    DT_CHECK_TIME_CONFIG  e_set_parm,
    VOID*                 pv_value
)
{
    INT32      i4_val;
    INT32      i4_rc = DTR_OK;
    
    switch (e_set_parm)
    {
        case DT_USE_DEFAULT_CONFIG:
        {
            ui4_nb_consecut_val_to_chk = 1 ;
            t_time_delta_adjustment = 2 ;
        }
        break;

        case DT_SET_CONSECUTIVE_VAL_TO_CHK:
        {
            if ( pv_value != NULL )
            {
                i4_val = *((INT32*)pv_value);
                /* minimum of consecutive good input time value is 1. */
                if ( i4_val < 1 )
                {
                    i4_val = 1 ;
                }
                ui4_nb_consecut_val_to_chk = (UINT32) i4_val;
            }
            else
            {
                i4_rc = DTR_INV_ARG;
            }
        }
        break;

        case DT_SET_TIME_WINDOW_ADJUSTMENT:
        {
            if ( pv_value != NULL )
            {
                i4_val = *((INT32*)pv_value);
                /* minimum delta value is 0.  A value of
                   0 specifies exact match to the second.*/
                if ( i4_val < 0 )
                {
                    i4_val = 0 ;
                }
                t_time_delta_adjustment = i4_val;
            }
            else
            {
                i4_rc = DTR_INV_ARG;
            }
        }
        break;

        case DT_REJECT_BRDCST_TIME_BEFORE_THIS_DATE:
        {
            if ( pv_value != NULL )
            {
                /* convert DTG value to UTC sec */
                t_reject_brdcst_tm_before_this_tm = 
                    x_dt_dtg_to_sec((DTG_T*)pv_value);
            }
            else
            {
                i4_rc = DTR_INV_ARG;
            }
        }
        break;
        
        default:
        {
            i4_rc = DTR_INV_ARG;
        }
        break;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 *
 * Name: dt_check_input_utc_time  
 *
 * Description:
 *   Check the received input time value with the system clock time
 *   to determine if the received input time is a reasonable value
 *   and should be accepted.
 *  
 * Inputs:
 *   t_txt_utc:  Input time received from broadcast stream.  
 *
 *   t_sys_clock_utc:  Current system clock time.
 *
 * Outputs: VOID
 *
 * Returns: 
 *   TRUE:    The input time passed the check.
 *   FALSE:   The input time failed the gross error check.
------------------------------------------------------------------------*/
BOOL dt_check_input_utc_time(TIME_T  t_txt_utc, TIME_T t_sys_clock_utc)
{
    BOOL  b_rc = TRUE;
    
    if ( b_check_input_time == TRUE )
    {
        if ( b_check_state_started == FALSE )
        {
            /* start the checking by recording the current UTC values. */
            t_last_txt_utc = t_txt_utc;
            t_last_sys_clock_utc = t_sys_clock_utc ;
            b_check_state_started = TRUE ;
            b_rc = FALSE;

            DBG_INFO(("{DT} Reject UTC value: UTC is initial input. \n"));
        }
        else
        {
            /* time should be increasing. */
            if ( t_txt_utc <= t_last_txt_utc )
            {
                t_last_txt_utc = t_txt_utc;
                t_last_sys_clock_utc = t_sys_clock_utc ;
                b_rc = FALSE;
                DBG_INFO(("{DT} Reject UTC value: UTC sec is not increasing.\n"));
            }
            else
            {
                /* perform the consistency check. */
                TIME_T    t_delta_txt_utc;
                TIME_T    t_delta_sys_clock_utc;
                TIME_T    t_adjustment;
                

                t_delta_txt_utc = t_txt_utc - t_last_txt_utc ;
                t_delta_sys_clock_utc = t_sys_clock_utc - t_last_sys_clock_utc ;

                /* adjust the time window delta to count for variation.
                   use 5% window.
                */
                t_adjustment = (TIME_T) (t_delta_sys_clock_utc * 0.05) ;
                if ( t_adjustment < t_time_delta_adjustment )
                {
                    t_adjustment = t_time_delta_adjustment;
                }

                if ( t_delta_txt_utc <= (t_delta_sys_clock_utc+t_adjustment) )
                {
                    /* the input txt time is reasoable. */
                    ui4_nb_consecut_good_val++;

                    if ( ui4_nb_consecut_good_val >= ui4_nb_consecut_val_to_chk )
                    {
                        b_rc = TRUE;
                    }
                    else
                    {
                        b_rc = FALSE;
                        DBG_INFO(("{DT} Reject UTC value: UTC sec did not pass consective good value test.\n"));
                    }
                }
                else
                {
                    /* reject input txt time, clear the consecutive
                       good value counter
                    */
                    ui4_nb_consecut_good_val = 0;
                    b_rc = FALSE;
                    DBG_INFO(("{DT} Reject UTC value: UTC sec did not pass value incrementing test.\n"));
                }
                t_last_txt_utc = t_txt_utc;
                t_last_sys_clock_utc = t_sys_clock_utc ;
            }
        }
    }
    return b_rc;
}

/*----------------------------------------------------------------------
 *
 * Name: dt_gross_error_check_on_input_time  
 *
 * Description:
 *   Check the received input time value with a specified threshold time value.
 *   If the received input time value is older than threshold time value,
 *   then reject the input time value.
 *
 * Inputs:
 *   t_input_utc:  Input time received from broadcast stream.  
 *
 * Outputs: VOID
 *
 * Returns: 
 *   TRUE:    The input time passed the gross error check.
 *   FALSE:   The input time failed the gross error check.
------------------------------------------------------------------------*/
BOOL dt_gross_error_check_on_input_time(TIME_T  t_input_utc)
{
    BOOL      b_rc = TRUE;
    
    if ( (t_input_utc < t_reject_brdcst_tm_before_this_tm) ||  (t_input_utc == t_last_stt_time) )
    {
        b_rc = FALSE;
    }	
	else
	{
		t_last_stt_time = t_input_utc;
		b_rc = TRUE;
	}
    return b_rc;
}

