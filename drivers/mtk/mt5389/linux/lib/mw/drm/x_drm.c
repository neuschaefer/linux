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
 * $RCSfile: x_drm.c,v $
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

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "drm/x_drm.h"
#include "drm/drm.h"
#include "drm/drm_dbg.h"

#else

#include "x_os.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_common.h"
#include "x_dbg.h"
#include "x_drm.h"
#include "drm.h"
#include "drm_dbg.h"
#include "x_debug.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/* semaphore to protect drm x-api */
extern HANDLE_T gh_drm_sema;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _drm_free_handle
 *
 * Description: This API is a callback function to free the drm handle object.
 *
 * Inputs: h_handle  Specify the handle to be freed.
 *         e_type  Specify the handle type.
 *         pv_obj  Specify the handle object to be freed.
 *         pv_tag  Specify the handle tag.
 *         b_req_handle  It sets to TRUE if the handle free was initiated with this handle else FALSE.
 *
 * Outputs: NONE.
 *          
 * Returns: TRUE  Success.
 *          FLASE  Fail.
 ----------------------------------------------------------------------------*/
static BOOL _drm_free_handle (
    HANDLE_T h_handle,
    HANDLE_TYPE_T e_handle_type,
    VOID* pv_obj,
    VOID* pv_tag,
    BOOL b_req_handle)
{
    if ((pv_obj != NULL) && (e_handle_type == HT_DRM_MEM_TYPE))
    {
        x_mem_free(pv_obj);
        pv_obj = NULL;

        return TRUE;
    }
    
    return FALSE;
} 

/*-----------------------------------------------------------------------------
 * Name: x_drm_open
 *
 * Description: This API opens a drm handle according to the drm command set.
 *
 * Inputs: pt_drm_cmd_sets  Specify the command set to initialize drm. 
 *         t_drm_type  Specify the drm type.
 *         ph_drm  Specify handle pointer to retrieve a drm handle.
 *
 * Outputs: ph_drm  Specify handle pointer contains the drm handle.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_OUT_OF_MEMORY  Fail. 
 *          DRMR_TYPE_NOT_SUPPORT  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail. 
 *          DRMR_OUT_OF_HANDLE  Fail. 
 *          DRMR_UNSPECIFIED_ERROR  Fail.  Caller function should call x_drm_get_info() to get reason.
 ----------------------------------------------------------------------------*/
INT32 x_drm_open (
    DRM_COMMAND_T* pt_drm_cmd_sets,
    DRM_TYPE_T t_drm_type,
    HANDLE_T* ph_drm)
{
    INT32 i4_ret;
    DRM_OBJ_T* pt_drm_obj = NULL;
    drm_handler_mem_open_fct pf_mem_open = NULL;

    DBG_API((_API_HEADER"%s: pt_drm_cmd_sets:%x, t_drm_type:%x, ph_drm:%x \n\r"
        , __FUNCTION__, pt_drm_cmd_sets, t_drm_type, ph_drm));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_drm_cmd_sets == NULL) || (ph_drm == NULL))
    {
        return DRMR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/    
    /* allocate object memory */
    LOG(0, "x_drm_open do memory allocate\n");
    pt_drm_obj = x_mem_alloc(sizeof(DRM_OBJ_T));
    if (pt_drm_obj == NULL)
    {
        return DRMR_OUT_OF_MEMORY;
    }
    x_memset(pt_drm_obj, 0, sizeof(DRM_OBJ_T));
 
    LOG(0, "x_drm_open do select handler\n");
    /* select handler */
    if (drm_match_handler(t_drm_type, &pt_drm_obj->t_handler_fct_tbl) != DRMR_OK)
    {
        LOG(0, "fail to x_drm_open do select handler\n");
        x_mem_free(pt_drm_obj);
        return DRMR_TYPE_NOT_SUPPORT;
    }

    LOG(0, "pf_mem_open = pt_drm_obj->t_handler_fct_tbl.pf_mopen\n");
    pf_mem_open = pt_drm_obj->t_handler_fct_tbl.pf_mopen;
    if (pf_mem_open)
    {
        LOG(0, "Perform mem open\n");
        i4_ret = pf_mem_open(pt_drm_cmd_sets, (VOID*)&pt_drm_obj->pv_specific_drm_obj);
        LOG(0, "After Perform mem open\n");
        if ((i4_ret != DRMR_OK) && (i4_ret != DRMR_UNSPECIFIED_ERROR))
        {
            LOG(0, "(i4_ret != DRMR_OK) && (i4_ret != DRMR_UNSPECIFIED_ERROR)\n");
            x_mem_free(pt_drm_obj);
            return i4_ret;
        }
    }
    else
    {
        LOG(0, "!pf_mem_open\n");
        x_mem_free(pt_drm_obj);
        return DRMR_FCT_NOT_SUPPORT;
    }

    LOG(0, "x_drm_open do allocate handler\n");
    /* allocate handle */
    if (handle_alloc(HT_DRM_MEM_TYPE, pt_drm_obj, NULL, _drm_free_handle, ph_drm) != HR_OK)
    {
        x_mem_free(pt_drm_obj);
        return DRMR_OUT_OF_HANDLE;
    }     
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_drm_close
 *
 * Description: This API closes the drm handle.
 *
 * Inputs: h_drm  Specify the drm handle to be closed.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INTERNAL_ERROR  Fail. 
 *          DRMR_INV_HANDLE  Fail.
 ----------------------------------------------------------------------------*/
INT32 x_drm_close (
    HANDLE_T h_drm)
{
    INT32 i4_ret;
    HANDLE_TYPE_T e_handle_type;
    DRM_OBJ_T* pt_drm_obj = NULL;
    drm_handler_close_fct pf_close = NULL;

    DBG_API((_API_HEADER"%s: h_drm:%x \n\r"
        , __FUNCTION__, h_drm));  
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_drm == NULL_HANDLE)
    {
        return DRMR_INV_ARG;
    }

    if (x_sema_lock(gh_drm_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }

    i4_ret = handle_get_type_obj(h_drm, &e_handle_type, (VOID**)&pt_drm_obj);
    if ((i4_ret != HR_OK) || (e_handle_type != HT_DRM_MEM_TYPE) || (pt_drm_obj == NULL))
    {
        if (x_sema_unlock(gh_drm_sema) != OSR_OK)
        {
            return DRMR_INTERNAL_ERROR;
        }

        return DRMR_INV_HANDLE;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/ 
    pf_close = pt_drm_obj->t_handler_fct_tbl.pf_close;
    if (pf_close)
    {
        pf_close(pt_drm_obj->pv_specific_drm_obj);
    }    

    /* free handle */                                
    handle_free(h_drm, TRUE); 

    if (x_sema_unlock(gh_drm_sema) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }    

    return DRMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_drm_get_info
 *
 * Description: This API retrieves information of the drm handle.
 *
 * Inputs: h_drm  Specify the drm handle.
 *         t_drm_get_type  Specify the get information type of drm.
 *         pv_buf  Specify memory buffer to retrive the drm information.
 *         z_buf_leng  Specify length of the pv_buf. If it is smaller than required, return DRM_INSUFFICENT_MEM.
 *
 * Outputs: pv_buf  Specify memory buffer contains the drm information.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INSUFFICIENT_BUFFER  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.  
 *          DRMR_INV_HANDLE  Fail.
 *          DRMR_INFO_NOT_SUPPORT  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 x_drm_get_info (
    HANDLE_T h_drm,
    DRM_GET_TYPE_T t_drm_get_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    INT32 i4_ret;
    HANDLE_TYPE_T e_handle_type;
    DRM_OBJ_T* pt_drm_obj = NULL;
    drm_handler_get_info_fct pf_get_info = NULL;
        
    DBG_API((_API_HEADER"%s: h_drm:%x, t_drm_get_type:%x, pv_buf:%x, z_buf_leng:%x \n\r"
        , __FUNCTION__, h_drm, t_drm_get_type, pv_buf, z_buf_leng));  
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_drm == NULL_HANDLE)
    {
        return DRMR_INV_ARG;
    }

    if ((pv_buf == NULL) || (z_buf_leng == 0))
    {
        return DRMR_INSUFFICIENT_BUFFER;
    }

    if (x_sema_lock(gh_drm_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }
    
    i4_ret = handle_get_type_obj(h_drm, &e_handle_type, (VOID**)&pt_drm_obj);
    if ((i4_ret != HR_OK) || (e_handle_type != HT_DRM_MEM_TYPE) || (pt_drm_obj == NULL))
    {
        if (x_sema_unlock(gh_drm_sema) != OSR_OK)
        {
            return DRMR_INTERNAL_ERROR;
        }

        return DRMR_INV_HANDLE;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_get_info = pt_drm_obj->t_handler_fct_tbl.pf_get_info;
    if (pf_get_info)
    {
        i4_ret = pf_get_info(pt_drm_obj->pv_specific_drm_obj, t_drm_get_type, pv_buf, z_buf_leng);
    }
    else
    {
        i4_ret = DRMR_FCT_NOT_SUPPORT;
    }

    if (x_sema_unlock(gh_drm_sema) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }    
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_drm_set_info
 *
 * Description: This API sets information of the drm handle.
 *
 * Inputs: h_drm  Specify the drm handle.
 *         t_drm_set_type  Specify the set information type of drm.
 *         pv_buf  Specify memory buffer to set the drm information.
 *         z_buf_leng  Specify length of the pv_buf.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INTERNAL_ERROR  Fail. 
 *          DRMR_INV_HANDLE  Fail.
 *          DRMR_RENTAL_EXPIRED  Fail.    
 *          DRMR_INFO_NOT_SUPPORT  Fail. 
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 *          DRMR_UNSPECIFIED_ERROR  Fail.  Caller function should call x_drm_get_info() to get reason.
 ----------------------------------------------------------------------------*/
INT32 x_drm_set_info (
    HANDLE_T h_drm,
    DRM_SET_TYPE_T t_drm_set_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    INT32 i4_ret;
    HANDLE_TYPE_T e_handle_type;
    DRM_OBJ_T* pt_drm_obj = NULL;
    drm_handler_set_info_fct pf_set_info = NULL;    
    
    DBG_API((_API_HEADER"%s: h_drm:%x, t_drm_set_type:%x, pv_buf:%x, z_buf_leng:%x \n\r"
        , __FUNCTION__, h_drm, t_drm_set_type, pv_buf, z_buf_leng));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_drm == NULL_HANDLE)
    {
        return DRMR_INV_ARG;
    }

    if (x_sema_lock(gh_drm_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }
    
    i4_ret = handle_get_type_obj(h_drm, &e_handle_type, (VOID**)&pt_drm_obj);
    if ((i4_ret != HR_OK) || (e_handle_type != HT_DRM_MEM_TYPE) || (pt_drm_obj == NULL))
    {
        if (x_sema_unlock(gh_drm_sema) != OSR_OK)
        {
            return DRMR_INTERNAL_ERROR;
        }

        return DRMR_INV_HANDLE;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pf_set_info = pt_drm_obj->t_handler_fct_tbl.pf_set_info;
    if (pf_set_info)
    {
        i4_ret = pf_set_info(pt_drm_obj->pv_specific_drm_obj, t_drm_set_type, pv_buf, z_buf_leng);
    }
    else
    {
        i4_ret = DRMR_FCT_NOT_SUPPORT;
    }

    if (x_sema_unlock(gh_drm_sema) != OSR_OK)
    {
        return DRMR_INTERNAL_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_drm_get_generic_info
 *
 * Description: This API retrieves generic information of the specific drm type.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *         t_drm_get_type  Specify the get information type of drm.
 *         pv_buf  Specify memory buffer to retrive the drm information.
 *         z_buf_leng  Specify length of the pv_buf. If it is smaller than required, return DRM_INSUFFICENT_MEM.
 *
 * Outputs: pv_buf  Specify memory buffer contains the drm information.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INSUFFICIENT_BUFFER  Fail.
 *          DRMR_TYPE_NOT_SUPPORT  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 x_drm_get_generic_info (
    DRM_TYPE_T t_drm_type,
    DRM_GET_TYPE_T t_drm_get_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    DRM_HANDLER_FCT_TBL_T t_handler_fct_tbl;
    drm_handler_get_generic_info_fct pf_get_generic_info = NULL;    

    DBG_API((_API_HEADER"%s: t_drm_get_type:%x, pv_buf:%x, z_buf_leng:%x \n\r"
        , __FUNCTION__, t_drm_get_type, pv_buf, z_buf_leng));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_buf == NULL) || (z_buf_leng == 0))
    {
        return DRMR_INSUFFICIENT_BUFFER;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* select handler */
    if (drm_match_handler(t_drm_type, &t_handler_fct_tbl) != DRMR_OK)
    {
        return DRMR_TYPE_NOT_SUPPORT;
    }
    
    pf_get_generic_info = t_handler_fct_tbl.pf_get_generic_info;
    if (pf_get_generic_info)
    {
        return pf_get_generic_info(t_drm_get_type, pv_buf, z_buf_leng);
    }    
    
    return DRMR_FCT_NOT_SUPPORT;
}

/*-----------------------------------------------------------------------------
 * Name: x_drm_set_generic_info
 *
 * Description: This API sets generic information of the specific drm type.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *         t_drm_set_type  Specify the set information type of drm.
 *         pv_buf  Specify memory buffer to set the drm information.
 *         z_buf_leng  Specify length of the pv_buf.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_TYPE_NOT_SUPPORT  Fail.
 *          DRMR_INSUFFICIENT_BUFFER  Fail. 
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 *          DRMR_INFO_NOT_SUPPORT  Fail. 
 ----------------------------------------------------------------------------*/
INT32 x_drm_set_generic_info (
    DRM_TYPE_T t_drm_type,
    DRM_SET_TYPE_T t_drm_set_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    DRM_HANDLER_FCT_TBL_T t_handler_fct_tbl;
    drm_handler_set_generic_info_fct pf_set_generic_info = NULL;    

    DBG_API((_API_HEADER"%s: t_drm_set_type:%x, pv_buf:%x, z_buf_leng:%x \n\r"
        , __FUNCTION__, t_drm_set_type, pv_buf, z_buf_leng));

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* select handler */
    if (drm_match_handler(t_drm_type, &t_handler_fct_tbl) != DRMR_OK)
    {
        return DRMR_TYPE_NOT_SUPPORT;
    }
    
    pf_set_generic_info = t_handler_fct_tbl.pf_set_generic_info;
    if (pf_set_generic_info)
    {
        return pf_set_generic_info(t_drm_set_type, pv_buf, z_buf_leng);
    }    
    
    return DRMR_FCT_NOT_SUPPORT;
}

