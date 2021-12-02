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
 * $RCSfile: aee_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: ed41ed3dcc5552c4ee8b06801ef05511 $
 *
 * Description: 
 *         This file contains the implementation of the exported APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "aee/x_aee.h"
#include "aee/_aee.h"


/*-----------------------------------------------------------------------------
 * Name: x_aee_create
 *
 * Description: This API creates a new application handle and spawn a thread
 *              to run the application's main function.
 *
 * Inputs:  pt_desc  References the application's description (resources, etc).
 *          ps_name  References the application's name.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_FAIL            The AEE cannot be created (undefined reason).
 *          AEER_OUT_OF_HANDLES  Out of handles.
 *          AEER_OUT_OF_MEMORY   Out of memory.
 *          AEER_INV_ARG         Invalid argument.
 *          AEER_INV_NAME        Invalid name.
 ----------------------------------------------------------------------------*/
INT32 x_aee_create (const AEE_APP_DESC_T*  pt_desc,
                    const CHAR*            ps_name,
                    HANDLE_T*              ph_app)
{
    INT32  i4_res;
    
    if ((ph_app == NULL)
        || (ps_name == NULL)
        || (pt_desc == NULL)
        || (pt_desc->pf_main == NULL)
        || (pt_desc->t_max_res.ui2_max_threads == 0)
        || (pt_desc->t_max_res.z_min_memory > pt_desc->t_max_res.z_max_memory))
    {
        i4_res = AEER_INV_ARG;
    }
    else
    {
        i4_res = aee_create_op((AEE_APP_DESC_T*)pt_desc,
                               ps_name,
                               ph_app);
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_aee_get_handle
 *
 * Description: This API returns a handle to an application (different from the
 *              handle created when the AEE was created).
 *
 * Inputs:  ps_name  References the name of the AEE.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_FAIL            No application with such name exists.
 *          AEER_OUT_OF_HANDLES  Out of handles.
 *          AEER_INV_ARG         Invalid argument.
 *          AEER_INV_NAME        Invalid name.
 ----------------------------------------------------------------------------*/
INT32 x_aee_get_handle (const CHAR*  ps_name,
                        HANDLE_T*    ph_app)
{
    INT32  i4_res;

    if (ps_name == NULL)
    {
        i4_res = AEER_INV_NAME;
    }
    else if (ph_app == NULL)
    {
        i4_res = AEER_INV_ARG;
    }
    else
    {
        i4_res = aee_get_handle_op(ps_name, ph_app);
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_aee_send_data
 *
 * Description: This API calls the application's receive_data function.
 *
 * Inputs:  h_app       Contains the application handle.
 *          ui4_type    Contains the type of data.
 *          pv_data     References the data to send.
 *          z_data_len  Contains the size (in bytes) of the pv_data buffer.
 *
 * Outputs: -
 *
 * Returns: AEER_OK          Routine successful.
 *          AEER_INV_HANDLE  Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_aee_send_data (HANDLE_T  h_app,
                       UINT32    ui4_type,
                       VOID*     pv_data,
                       SIZE_T    z_data_len)
{
    INT32  i4_res;
    
    i4_res = aee_send_data_op(h_app,
                              ui4_type,
                              pv_data,
                              z_data_len);
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_aee_get_resource_info
 *
 * Description: This API returns information about the AEE (e.g. maximum number
 *              of handles the application can allocate, etc)
 *
 * Inputs:  h_app     Contains the application handle.
 *          e_res_id  Contains the id of the resource information to get.
 *
 * Outputs: pv_res    Contains the value of the resource information.
 *
 * Returns: AEER_OK       Routine successful.
 *          AEER_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_aee_get_resource_info (HANDLE_T           h_app,
                               AEE_RESOURCE_ID_T  e_res_id,
                               VOID*              pv_res)
{
    INT32  i4_res;
    
    if (pv_res == NULL)
    {
        i4_res = AEER_INV_ARG;
    }
    else
    {
        i4_res = aee_get_resource_info_op(h_app,
                                          e_res_id,
                                          pv_res);
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_aee_thread_create
 *
 * Description: This API creates a new thread that belongs to the AEE.
 *
 * Inputs:  ps_name      References the thread name.
 *          z_stacksize  Specifies the thread's stack size.
 *          ui1_pri      Contains the thread priority.
 *          pf_main      Contains the thread's entry point.
 *          z_arg_size   Contains the size in bytes of what pv_arg points to.
 *          pv_arg       References the argument of pf_main.
 *
 * Outputs: ph_th_hdl    Contains thread handle.
 *
 * Returns: AEER_OK       Routine successful.
 *          AEER_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_aee_thread_create (HANDLE_T*             ph_th_hdl,
                           CHAR*                 ps_name,
                           SIZE_T                z_stacksize,
                           UINT8                 ui1_pri,
                           x_os_thread_main_fct  pf_main,
                           SIZE_T                z_arg_size,
                           VOID*                 pv_arg)
{
    INT32  i4_res;
    
    if ((ph_th_hdl == NULL) ||
        (ps_name == NULL) ||
        (pf_main == NULL))
    {
        i4_res = AEER_INV_ARG;
    }
    else
    {
        i4_res = aee_thread_create(ph_th_hdl,
                                   ps_name,
                                   z_stacksize,
                                   ui1_pri,
                                   pf_main,
                                   z_arg_size,
                                   pv_arg);
    }
    
    return i4_res;
}
