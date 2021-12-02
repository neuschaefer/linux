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
 * $RCSfile: c_timer_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains implementation of application level timer
 *         functions.
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "os/inc/os.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee.h"
/*------------------------------------------------------------------------
                    functions implementation
 ------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Function: c_timer_create()
 *
 * Description:
 *      this API creates timer and a handle to the timer.
 *
 * Inputs:
 *    None.
 *
 * Outputs:
 *    ph_timer: pointer to hold returned timer handle.
 *              the handle is valid only if this API returns OSR_OK.
 *              the handle is used to access the created timer.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *      OSR_AEE_OUT_OF_RESOURCES: can not allocate AEE resources for the timer.
 *      OSR_AEE_NO_RIGHTS: No permission to create the timer.
 *---------------------------------------------------------------------------*/
INT32 c_timer_create
(
    HANDLE_T*  ph_timer
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_timer_create(ph_timer);

            if (i4_rc == OSR_OK)
            {
                handle_link_to_aux(h_aux, *ph_timer);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = OSR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = OSR_AEE_NO_RIGHTS;
        }
        break;
    }

    return i4_rc;
}

/*----------------------------------------------------------------------------
 * Function: c_timer_start()
 *
 * Description:
 *      this API starts a timer.
 *
 * Inputs: -
 *    h_timer: timer handle returned from x_timer_create().
 *    ui4_delay: timer pops after this delay has passed.
 *    e_flags: either X_TIMER_FLAG_ONCE or X_TIMER_FLAG_REPEAT.
 *    pf_callback: callback function to be executed after "ui4_delay"
 *                 had passed.
 *    pv_tag: argument to callback function pf_cb.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 c_timer_start
(
    HANDLE_T           h_timer,
    UINT32             ui4_delay,
    TIMER_FLAG_T       e_flags,
    x_os_timer_cb_fct  pf_callback,
    VOID*              pv_tag
)
{
    return x_timer_start(h_timer, ui4_delay, e_flags, pf_callback, pv_tag);
}

/*----------------------------------------------------------------------------
 * Function: c_timer_stop()
 *
 * Description:
 *      this API stops a timer.
 *
 * Inputs: -
 *    h_timer: timer handle returned from x_timer_create().
 *
 * Outputs:
 *    None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 c_timer_stop
(
    HANDLE_T  h_timer
)
{
    return x_timer_stop(h_timer);
}

/*--------------------------------------------------------------------------
 * Function: c_timer_delete()
 *
 * Description:
 *    this API deleted a timer and its handle.
 *
 * Inputs: -
 *    h_timer: timer handle returned from x_timer_create().
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *-------------------------------------------------------------------------*/
INT32 c_timer_delete
(
    HANDLE_T h_timer
)
{
    INT32  i4_rc;
    i4_rc = x_timer_delete(h_timer);
    return i4_rc;
}


/*----------------------------------------------------------------------------
 * Function: c_timer_resume()
 *
 * Description:
 *      this API resumes a previoulsy stopped timer.
 *
 * Inputs: -
 *    h_timer: timer handle returned from x_timer_create().
 *
 * Outputs:
 *    None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 c_timer_resume
(
    HANDLE_T  h_timer
)
{
    return x_timer_resume(h_timer);
}

/*----------------------------------------------------------------------------
 * Function: c_os_get_sys_tick
 *
 * Description: This API returns the system tick count.
 *
 * Inputs: - None
 *
 * Outputs: None
 *
 * Returns: The system tick value
 *---------------------------------------------------------------------------*/
UINT32 c_os_get_sys_tick (VOID)
{
    return os_get_sys_tick();
}

/*----------------------------------------------------------------------------
 * Function: c_os_get_sys_tick_period
 *
 * Description: This API returns the period (in ms) between system tick.
 *
 * Inputs: - None
 *
 * Outputs: None
 *
 * Returns: The period (in ms) between system tick.
 *---------------------------------------------------------------------------*/
UINT32 c_os_get_sys_tick_period (VOID)
{
    return x_os_drv_get_tick_period();
}

/*----------------------------------------------------------------------------
 * Function: c_os_set_timestamp
 *
 * Description: This API set time stamp toget system boot up time.
 *
 * Inputs: - String tag. 
 *
 * Outputs: None
 *
 * Returns: None
 *---------------------------------------------------------------------------*/
VOID c_os_set_timestamp (CHAR *szString)
{
    x_os_drv_set_timestamp(szString);
}

