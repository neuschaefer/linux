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
 * $RCSfile: c_sema_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains implementation of the semaphore
 *         application API's. 
 *         
 *---------------------------------------------------------------------------*/


/* Middleware related header files */
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee.h"


/*----------------------------------------------------------------------------
 * Function: c_sema_create()
 *
 * Description:
 *      this API creates a semaphore and return a handle to the semaphore.
 *
 * Inputs:
 *      e_type: can be X_SEMA_TYPE_BINARY, X_SEMA_TYPE_MUTEX
 *              or X_SEMA_TYPE_COUNTING
 *      ui4_init_value: inital value of this semaphore.
 *
 * Outputs:
 *      *ph_sema_hdl: pointer to hold returned semaphore handle.
 *                   the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *      OSR_AEE_OUT_OF_RESOURCES: can not allocate AEE resources
 *                                for the semaphore.
 *      OSR_AEE_NO_RIGHTS: No permission to create the semaphore.
 *---------------------------------------------------------------------------*/
INT32 c_sema_create
(
    HANDLE_T      *ph_sema_hdl,
    SEMA_TYPE_T   e_type,
    UINT32        ui4_init_value
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_resources(AEE_FLAG,
                               0,               /* memory resource */
                               0,               /* file size */
                               0,               /* number of files */
                               1,               /* number of handle. */
                               0,               /* number of threads */
                               1,               /* number of semaphore */
                               0,               /* number of msg queues */
                               &h_aux);
    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_sema_create(ph_sema_hdl, e_type, ui4_init_value);
            
            if (i4_rc == OSR_OK)
            {
                handle_link_to_aux(h_aux, *ph_sema_hdl);
            }
            else
            {
                aee_release_resources
                    (
                        0,       /* memory resource */
                        0,       /* file size */
                        0,       /* number of files */
                        1,       /* number of handle. */
                        0,       /* number of threads */
                        1,       /* number of semaphore */
                        0        /* number of msg queues */
                    );
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
 * Function: c_sema_delete()
 *
 * Description:
 *      this API deletes a semaphore and its handle. 
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via c_sema_create().
 *
 * Outputs: None.
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32 c_sema_delete
(
    HANDLE_T h_sema_hdl
)          
{
    INT32  i4_rc;
    
    i4_rc=x_sema_delete(h_sema_hdl);
    if ( i4_rc == OSR_OK )
    {
        aee_release_resources
            (
                        0,       /* memory resource */
                        0,       /* file size */
                        0,       /* number of files */
                        0,       /* number of handle. */
                        0,       /* number of threads */
                        1,       /* number of semaphore */
                        0        /* number of msg queues */
                );
    }
    return i4_rc;    
}


/*----------------------------------------------------------------------------
 * Function: c_sema_lock()
 *
 * Description:
 *      this API locks (acquires) a sema. 
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via c_sema_create().
 *      e_option: either X_SEMA_OPTION_WAIT or X_SEMA_OPTION_NOWAIT.
 *
 * Outputs: None
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_WOULD_BLOCK: if eoption is X_SEMA_OPTION_NOWAIT and the calling
 *                       thread would have been blocked to lock the sema.
 *---------------------------------------------------------------------------*/
INT32 c_sema_lock
(
    HANDLE_T h_sema_hdl,
    SEMA_OPTION_T e_option
)
{
    return x_sema_lock(h_sema_hdl, e_option);
}


/*----------------------------------------------------------------------------
 * Function: c_sema_lock_timeout()
 *
 * Description:
 *      this API attempts to lock (acquires) a semaphore,  if the semaphore
 *      has been acquired by others, then this API will only block for a
 *      specified amount of time before returning. 
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via c_sema_create().
 *      ui4_time: time duration in ms before thie API bails out.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_TIMEOUT: failed to lock the semaphore within "ui4_time" duration.
 *---------------------------------------------------------------------------*/
INT32 c_sema_lock_timeout
(
    HANDLE_T h_sema_hdl,
    UINT32   ui4_time
)
{
    return x_sema_lock_timeout(h_sema_hdl, ui4_time);
}


/*----------------------------------------------------------------------------
 * Function: c_sema_unlock()
 *
 * Description:
 *      this API unlocks (releases) a semaphore. 
 *
 * Inputs:
 *      h_sema_hdl: semaphore handle returned via c_sema_create().
 *
 * Outputs: None
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : semaphore module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
c_sema_unlock
(
    HANDLE_T h_sema_hdl
)    
{
    return x_sema_unlock(h_sema_hdl);
}
