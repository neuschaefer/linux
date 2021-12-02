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
 * $RCSfile: dt_timer.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains utility function for calendar dates
 *         calculation.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dt/x_dt.h"
#include "dbg/x_dbg.h"
#include "dt/_dt.h"


/*
  DT timer.

  This timer will be used to notify client of Datetime library.

  For example:  send notification when the Day-Light-Saving time
                change over (to true or false) has occured.
*/
static HANDLE_T    h_dt_timer    = NULL_HANDLE;


/*--------------------------------------------------------------------------
 * Name: dt_timer_create
 *
 * Description: This API creates the DT library timer.
 *
 * Inputs:  -
 *   None
 *
 * Outputs: -
 *   None.
 *
 * Returns: - 
 *   EASR_FAIL
 -------------------------------------------------------------------------*/
INT32 dt_timer_create
(
    VOID
)
{
    INT32   i4_rc = DTR_OK;
    
    if ( h_dt_timer == NULL_HANDLE )
    {
        if ( x_timer_create(&h_dt_timer) != OSR_OK )
        {
            h_dt_timer = NULL_HANDLE;
            i4_rc = DTR_FAIL;
        }
    }
    return i4_rc ;
}

/*-----------------------------------------------------------------------------
 * Name: dt_timer_reg_action
 *
 * Description: This function registers a user provided function that 
 *   will be executed after specified time has passed. If another action 
 *   is registered before the current action is executed, then the new
 *   action overides the previous action. The previous action is lost.
 *              
 * Inputs:  -
 *   ui4_delay  Time (in sec) to wait before the user specified function 
 *              is executed.
 *
 *   pf_cb      Function to be executed. NOTE: because the funciton 
 *              will be called in the ISR context, this function should 
 *              send a message to caller.
 *
 *              The prototype for the callback function:
 *
 *                  VOID (*cb) (timer_handle, pv_tag)
 *
 *   pv_tag     User specified tag value.
 *
 * Outputs: -
 *   None.
 *
 * Returns: - 
 *   DTR_FAIL
 ----------------------------------------------------------------------------*/
INT32 dt_timer_reg_action
(
    UINT32              ui4_delay,
    x_os_timer_cb_fct   pf_cb,
    VOID*               pv_tag
)
{
    INT32   i4_rc = DTR_FAIL;

    i4_rc = dt_timer_create();

    if ( i4_rc == DTR_OK )
    {
        /* convert to milli-seconds. */
        ui4_delay = ui4_delay * 1000 ;

        /* When notification is immedidate, e.g., delay is 0 sec,
           we still need a slight delay due to x_time_start API
           will not accept delay of 0.
        */
        if ( ui4_delay == 0 )
        {
            ui4_delay = 10;
        }
    
        if ( x_timer_start(h_dt_timer,
                           ui4_delay,
                           X_TIMER_FLAG_ONCE,
                           pf_cb, pv_tag) == OSR_OK )
        {
            i4_rc = DTR_OK;
        }
    }
    
    return i4_rc ;
}

/*--------------------------------------------------------------------------
 * Name: dt_timer_cancel_action
 *
 * Description: This API cancels the registered function from executing
 *   by the timer.
 *
 * Inputs:  -
 *   None
 *
 * Outputs: -
 *   None.
 *
 * Returns: - 
 *   EASR_FAIL
 -------------------------------------------------------------------------*/
INT32 dt_timer_cancel_action
(
    VOID
)
{
    INT32   i4_rc = DTR_OK;
    if ( h_dt_timer != NULL_HANDLE )
    {
        if ( x_timer_stop(h_dt_timer) != OSR_OK )
        {
            i4_rc = DTR_FAIL;
        }
    }
    return i4_rc ;
}





