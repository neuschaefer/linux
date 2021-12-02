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
 * $RCSfile: c_thread_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains implementation of the thread application
 *         API's.
 *---------------------------------------------------------------------------*/

#include "os/inc/x_os.h"
#include "inc/c_pvt_key.h"

/*----------------------------------------------------------------------------
 * Function: x_thread_delay()
 *
 * Description:
 *      this API allows a thread to give up CPU for a duration. it performs
 *      1. check if thread mudule is initiated. 
 *      2. call OS driver thread delay routine.
 *
 * Inputs:
 *      ui4_delay: in millisecond (ms) the thread gives up CPU.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
c_thread_delay(UINT32 ui4_delay)
{

    if (ui4_delay != 0)
    {
        x_thread_delay(ui4_delay);
    }
    return;
}

/*----------------------------------------------------------------------------
 * Function: c_thread_set_pvt()
 *
 * Description:
 *      This API sets some private data, associated with a thread and key.
 *      Such private data can again be extracted using API
 *      "x_thread_get_pvt ()". Note that the key must have the flag
 *      PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 * Inputs:
 *      ui4_key     Contains the key associated with the private data.
 *      pf_pvt_del  References a callback function which is called when the
 *                  private data is being deleted or the thread is being
 *                  terminated.
 *      pv_pvt      Contains or references the private data.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK :          Routine was successful.
 *      OSR_INV_ARG :     Invalid argument.
 *      OSR_NOT_INIT :    Thread module has not been initiated.
 *      OSR_NOT_EXIST :   Invalid or unknown OS thread.
 *      OSR_DUP_KEY :     Duplicate key.
 *      OSR_NO_RESOURCE : Out of resources (e.g. memory etc.).
 *---------------------------------------------------------------------------*/
INT32 c_thread_set_pvt (UINT32                   ui4_key,
                        x_os_thread_pvt_del_fct  pf_pvt_del,
                        VOID*                    pv_pvt)
{
    INT32 i4_return;


    i4_return = OSR_INV_ARG;

    if ((ui4_key & PVT_KEY_FLAG_APPL_ACCESS_ALLOWED) != 0)
    {
        i4_return = x_thread_set_pvt (ui4_key, pf_pvt_del, pv_pvt);
    }

    return (i4_return);
}


/*----------------------------------------------------------------------------
 * Function: c_thread_get_pvt()
 *
 * Description:
 *      This API gets some private data, associated with a thread and key.
 *      Such private data was set using API "x_thread_set_pvt ()". Note that
 *      the key must have the flag PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 * Inputs:
 *      ui4_key  Contains the key associated with the private data.
 *
 * Outputs:
 *      ppv_pvt  Contains or references the private data.
 *
 * Returns:
 *      OSR_OK :            Routine was successful.
 *      OSR_INV_ARG :       Invalid argument.
 *      OSR_NOT_INIT :      Thread module has not been initiated.
 *      OSR_NOT_EXIST :     Invalid or unknown OS thread.
 *      OSR_KEY_NOT_FOUND : The specified key in argument "ui4_key" was not
 *                          found.
 *---------------------------------------------------------------------------*/
INT32 c_thread_get_pvt (UINT32  ui4_key,
                        VOID**  ppv_pvt)
{
    INT32 i4_return;


    i4_return = OSR_INV_ARG;

    if ((ui4_key & PVT_KEY_FLAG_APPL_ACCESS_ALLOWED) != 0)
    {
        i4_return = x_thread_get_pvt (ui4_key, ppv_pvt);
    }

    return (i4_return);
}


/*----------------------------------------------------------------------------
 * Function: c_thread_del_pvt()
 *
 * Description:
 *      This API deletes some private data, associated with a thread and key.
 *      Such private data was set using API "x_thread_set_pvt ()". Note that
 *      the key must have the flag PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 * Inputs:
 *      ui4_key  Contains the key associated with the private data.
 *
 * Outputs:
 *      None.
 *
 * Returns:
 *      OSR_OK :            Routine was successful.
 *      OSR_NOT_INIT :      Thread module has not been initiated.
 *      OSR_NOT_EXIST :     Invalid or unknown OS thread.
 *      OSR_KEY_NOT_FOUND : The specified key in argument "ui4_key" was not
 *                          found.
 *---------------------------------------------------------------------------*/
INT32 c_thread_del_pvt (UINT32  ui4_key)
{
    INT32 i4_return;


    i4_return = OSR_INV_ARG;

    if ((ui4_key & PVT_KEY_FLAG_APPL_ACCESS_ALLOWED) != 0)
    {
        i4_return = x_thread_del_pvt (ui4_key);
    }

    return (i4_return);
}

