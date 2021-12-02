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
 * $RCSfile: divx_drm_hw_key.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

#ifndef __KERNEL__
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "drm/drm_dbg.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm_elem_def.h"
#include "res_mngr/drv/x_drm_drv.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _no_use_rm_nfy
 *
 * Description: Callback function for rm_open_comp().
 *
 * Inputs:  h_obj  -
 *          ui1_port  -
 *          e_nfy_cond  -
 *          pv_tag  -
 *          ui4_data  -
 *
 * Outputs:  - 
 *
 * Returns:  -
 ----------------------------------------------------------------------------*/
static VOID _no_use_rm_nfy (
    HANDLE_T h_obj,
    UINT8 ui1_port,
    RM_COND_T e_nfy_cond,
    VOID* pv_tag,
    UINT32 ui4_data)
{
    /* no use */
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_hw_secret_key
 *
 * Description: This API retrieves the divx drm hardware secret key.
 *
 * Inputs: pui1_divx_drm_hw_secret_key  Reference to retrieve the divx drm hardware secret key.
 *
 * Outputs: pui1_divx_drm_hw_secret_key  Reference contains the divx drm hardware secret key.
 *
 * Returns: TRUE  Success.
 *          FALSE  Fail.
 ----------------------------------------------------------------------------*/
BOOL divx_drm_hw_secret_key (
    UINT8* pui1_divx_drm_hw_secret_key)
{
    INT32 i4_ret;
    HANDLE_T h_divx_drm;
    RM_COND_T e_rm_cond;
    SIZE_T z_size;
    DIVX_DRM_HW_SECRET_KEY_T t_divx_drm_hw_secret_key;
    
    i4_ret = rm_open_comp(
        NULL_HANDLE,
        OBJ_COMP_REL_IGNORE,
        DRVT_DRM,
        0,
        NULL,
        CTRL_TYPE_SINGLE,
        NULL,
        NULL,
        NULL,
        _no_use_rm_nfy,
        &h_divx_drm,
        &e_rm_cond);
    if ((i4_ret != RMR_OK) || (e_rm_cond & ELEM_STATE_CTRL_SHARED))
    {
        DBG_ERROR((_ERROR_HEADER"%s: RM opens component failed, return=%d\n", __FUNCTION__, i4_ret)); 
        printk("RM opens component failed\n");

        return FALSE;
    }

    z_size = DIVX_DRM_HW_SECRET_KEY_SIZE_BYTE;
    i4_ret = rm_get(
        h_divx_drm,
        FALSE,
        ALL_PORTS,
        DIVX_DRM_GET_TYPE_HW_SECRET_KEY,
        (VOID*)t_divx_drm_hw_secret_key.aui1_divx_drm_hw_secret_key,
        &z_size,
        NULL);
    if (i4_ret != RMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: RM gets the divx drm hardware secret key failed, return=%d\n", __FUNCTION__, i4_ret)); 
        printk("RM gets the divx drm hardware secret key failed\n");

        rm_close(h_divx_drm, FALSE, &e_rm_cond);
        
        return FALSE;
    }

    i4_ret = rm_close(h_divx_drm, FALSE, &e_rm_cond);
    if (i4_ret != RMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: RM closes component failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("RM closes component failed\n");

        return FALSE;
    }

    x_memcpy(pui1_divx_drm_hw_secret_key, t_divx_drm_hw_secret_key.aui1_divx_drm_hw_secret_key, DIVX_DRM_HW_SECRET_KEY_SIZE_BYTE);

    return TRUE;
}

#endif

