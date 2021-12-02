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
 * $RCSfile: divx_drm.c,v $
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
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "drm/drm_dbg.h"
#include "drm/divx_drm/divx_drm.h"
#include "drm/divx_drm/divx_drm_adp.h"
#include "drm/divx_drm/x_divx_drm.h"

#ifdef DIVX_DRM_40
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpApi.h"
#elif defined(DIVX_DRM_51)
#include "drm/divx_drm/divx_drm_51/DrmApi.h"
#endif

#else

#include "x_os.h"
#include "x_common.h"
#include "x_dbg.h"  // !!! megaa 20090202 for DBG_INFO
#include "divx_drm.h"
#include "divx_drm_adp.h"
#include "x_divx_drm.h"
#include "../drm_dbg.h"
#include "divx_drm.h"

#ifdef DIVX_DRM_40
#include "divx_drm_40/AdpLib/DrmAdpApi.h"
#elif defined(DIVX_DRM_51)
#include "divx_drm_51/DrmApi.h"
#endif

#endif

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
 * Name: divx_drm_open
 *
 * Description: This API opens a divx drm object according to the drm command set.
 *
 * Inputs: pt_drm_cmd_sets  Specify the command set to initialize divx drm.
 *         ppv_obj  Specify pointer to retrive the divx drm object.
 *
 * Outputs: ppv_obj  Specify pointer contains the divx drm object.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INV_COMMAND  Fail.
 *          DRMR_OUT_OF_MEMORY  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_open (
    DRM_COMMAND_T* pt_drm_cmd_sets,
    VOID** ppv_obj)
{
    INT32 i4_ret;
    UINT32 ui4_idx = 0;
    DIVX_DRM_OBJ_T* pt_divx_drm_obj = NULL;
    DRM_COMMAND_T* pt_iter = pt_drm_cmd_sets;    
    DRM_COMMAND_T t_drm_cmd_strd = {0};
    DRM_COMMAND_T t_drm_cmd_registration = {0};
#if 0
    DRM_COMMAND_T t_drm_cmd_deactivation = {0};    
#endif

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_drm_cmd_sets == NULL)
    {
        return DRMR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* get command */    
    while (pt_iter->e_code != DRM_CMD_CODE_END)
    {
        switch (pt_iter->e_code)
        {
            case DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK:
                t_drm_cmd_strd.u.pv_data = pt_iter->u.pv_data;
                break;

            case DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE:
                t_drm_cmd_registration.u.ps_text = pt_iter->u.ps_text;
                break;
#if 0
            case DRM_CMD_CODE_DIVX_DRM_DEACTIVATION_CODE:
                t_drm_cmd_deactivation.u.ps_text = pt_iter->u.ps_text;
                break;
#endif
            default:
                return DRMR_INV_COMMAND;
        }
        pt_iter = &pt_drm_cmd_sets[++ui4_idx];        
    }

    /* check command */
    if ((t_drm_cmd_strd.u.pv_data == NULL) && (t_drm_cmd_registration.u.ps_text == NULL) /* && (t_drm_cmd_deactivation.u.ps_text == NULL) */)
    {
        return DRMR_INV_ARG;
    }

    /* parse strd chunk */
    if (t_drm_cmd_strd.u.pv_data != NULL)
    {
        /* alloc memory for the divx drm object */
        pt_divx_drm_obj = (DIVX_DRM_OBJ_T*)(*ppv_obj);
        if (pt_divx_drm_obj)
        {
            x_mem_free(pt_divx_drm_obj);
        }
        pt_divx_drm_obj = x_mem_alloc(sizeof(DIVX_DRM_OBJ_T));
        if (pt_divx_drm_obj == NULL)
        {
            return DRMR_OUT_OF_MEMORY;
        }
        x_memset(pt_divx_drm_obj, 0, sizeof(DIVX_DRM_OBJ_T));
        *ppv_obj = (VOID*)pt_divx_drm_obj;

        /* alloc memory for the divx drm handler object */    
        if (pt_divx_drm_obj->pv_handler_obj)
        {
            x_mem_free(pt_divx_drm_obj->pv_handler_obj);
        }
        pt_divx_drm_obj->pv_handler_obj = x_mem_alloc(
#ifdef DIVX_DRM_40
        getPlaybackContextSize()
#elif defined(DIVX_DRM_51)
        drmGetDrmContextSize()
#endif
        );
        if (pt_divx_drm_obj->pv_handler_obj == NULL)
        {
            x_mem_free(pt_divx_drm_obj);    

            return DRMR_OUT_OF_MEMORY;
        }
        x_memset(pt_divx_drm_obj->pv_handler_obj, 0, 
#ifdef DIVX_DRM_40
        getPlaybackContextSize()
#elif defined(DIVX_DRM_51)
        drmGetDrmContextSize()
#endif
        );

        /* start to parse the strd chunk */    
        i4_ret = divxDrmDecryptNInit(pt_divx_drm_obj, (UINT8*)t_drm_cmd_strd.u.pv_data+8);
        if (i4_ret != DRMR_OK)
        {
            x_mem_free(pt_divx_drm_obj->pv_handler_obj);    
            x_mem_free(pt_divx_drm_obj);

            return i4_ret;
        }        
    }

    /* get registration code */
    if (t_drm_cmd_registration.u.ps_text != NULL)
    {
        i4_ret = divxDrmGetRegistrationCode(t_drm_cmd_registration.u.ps_text);
        if (i4_ret != DRMR_OK)
        {
            if (pt_divx_drm_obj != NULL)
            {
                x_mem_free(pt_divx_drm_obj->pv_handler_obj);    
                x_mem_free(pt_divx_drm_obj);            
            }            
            return i4_ret;
        }

        if (pt_divx_drm_obj != NULL)
        {
            x_strncpy(pt_divx_drm_obj->t_divx_drm_registration_info.s_registration_code, t_drm_cmd_registration.u.ps_text, DRM_REGISTRATION_CODE_BYTES);
        }
    }

#if 0
    /* get deactivation code */
    if (t_drm_cmd_deactivation.u.ps_text != NULL)
    {
        i4_ret = divxDrmGetDeactivationCode(t_drm_cmd_deactivation.u.ps_text);
        if (i4_ret != DRMR_OK)
        {
            if (pt_divx_drm_obj != NULL)
            {
                x_mem_free(pt_divx_drm_obj->pv_handler_obj);    
                x_mem_free(pt_divx_drm_obj);            
            }            
            return i4_ret;
        }

        if (pt_divx_drm_obj != NULL)
        {
            x_strncpy(pt_divx_drm_obj->t_divx_drm_deactivation_info.s_deactivation_code, t_drm_cmd_deactivation.u.ps_text, 9);
        }
    }
#endif

    return DRMR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: divx_drm_close
 *
 * Description: This API closes the divx drm object.
 *
 * Inputs: pv_obj  Specify the divx drm object to be closed.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_close ( 
    VOID* pv_obj)
{
    DIVX_DRM_OBJ_T* pt_divx_drm_obj = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        return DRMR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_divx_drm_obj = (DIVX_DRM_OBJ_T*) pv_obj;
    
    /* free memory for the divx drm handler object */    
    if (pt_divx_drm_obj->pv_handler_obj)
    {
#ifdef DVIX_DRM_50
        drmFinalizePlayback((UINT8*)pt_divx_drm_obj->pv_handler_obj);
#endif
        x_mem_free(pt_divx_drm_obj->pv_handler_obj);
    }

    /* free memory for the divx drm object */
    if (pt_divx_drm_obj)
    {
        x_mem_free(pt_divx_drm_obj);
    }

    return DRMR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: divx_drm_get_info
 *
 * Description: This API retrieves information of the divx drm object.
 *
 * Inputs: pv_obj  Specify the divx drm object.
 *         t_drm_get_type  Specify the get information type of divx drm.
 *         pv_buf  Specify memory buffer to retrive divx drm information.
 *         z_buf_leng  Specify length of the pv_buf. If it is smaller than required, return DRM_INSUFFICENT_MEM.
 *
 * Outputs: pv_buf  Specify memory buffer contains the divx drm information.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INSUFFICIENT_BUFFER  Fail.
 *          DRMR_INFO_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_get_info( 
    VOID* pv_obj, 
    DRM_GET_TYPE_T t_drm_get_type, 
    VOID* pv_buf, 
    SIZE_T z_buf_leng)
{
#if 0
    INT32 i4_ret;
#endif
    DIVX_DRM_OBJ_T* pt_divx_drm_obj = NULL;
    DIVX_DRM_UNION_INFO_T* pt_divx_drm_info = NULL;
   
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return DRMR_INV_ARG;
    }
    
    /* check buffer */
    if ((pv_buf == NULL) || (z_buf_leng < sizeof(DIVX_DRM_UNION_INFO_T)))
    {
        return DRMR_INSUFFICIENT_BUFFER;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_divx_drm_obj = (DIVX_DRM_OBJ_T*) pv_obj;
    pt_divx_drm_info = (DIVX_DRM_UNION_INFO_T*) pv_buf;   

    switch (t_drm_get_type)
    {
        case DRM_GET_TYPE_DIVX_DRM_BASIC_INFO:
            pt_divx_drm_info->pt_divx_drm_basic_info = &(pt_divx_drm_obj->t_divx_drm_basic_info);

            /* check rental expired for stop-to-play */
            if ((pt_divx_drm_info->pt_divx_drm_basic_info->ui1_use_count == pt_divx_drm_info->pt_divx_drm_basic_info->ui1_use_limit) &&
                (pt_divx_drm_info->pt_divx_drm_basic_info->ui1_use_limit != 0))
            {
                DBG_INFO((_ERROR_HEADER"%s: Screen 3 (rental expired) for stop-to-play\n",__FUNCTION__));
                
                /* fill data, rental expired */
                pt_divx_drm_info->pt_divx_drm_basic_info->ui1_flag |= DIVX_DRM_RENTAL_EXPIRED;
            }
            break; 
            
        case DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO:
            if (((pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag & DIVX_DRM_AUTH_ERROR) == 0) &&
                ((pt_divx_drm_obj->t_divx_drm_basic_info.ui1_flag & DIVX_DRM_RENTAL_EXPIRED) == 0))
            {
                pt_divx_drm_info->pt_divx_drm_crypto_info = &(pt_divx_drm_obj->t_divx_drm_crypto_info);
            }
            break;
            
        case DRM_GET_TYPE_OUTPUT_SIGNAL_PROTECTION_INFO:
            pt_divx_drm_info->pt_drm_output_signal_protection_info = &(pt_divx_drm_obj->t_drm_output_signal_protection_info);
            break;
            
        default:
            return DRMR_INFO_NOT_SUPPORT;
    }

    return DRMR_OK;
} 

/*-----------------------------------------------------------------------------
 * Name: divx_drm_set_info
 *
 * Description: This API sets information of the divx drm object.
 *
 * Inputs: pv_obj  Specify the divx drm object.
 *         t_drm_set_type  Specify the set information type of the divx drm.
 *         pv_buf  Specify memory buffer to set the divx drm information.
 *         z_buf_leng  Specify length of the pv_buf.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_RENTAL_EXPIRED  Fail.    
 *          DRMR_INFO_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_set_info( 
    VOID* pv_obj, 
    DRM_SET_TYPE_T t_drm_set_type, 
    VOID* pv_buf, 
    SIZE_T z_buf_leng)
{
    DIVX_DRM_OBJ_T* pt_divx_drm_obj = NULL;
   
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return DRMR_INV_ARG;
    }
        
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_divx_drm_obj = (DIVX_DRM_OBJ_T*) pv_obj;

    switch (t_drm_set_type)
    {
        case DRM_SET_TYPE_DIVX_DRM_PLAYBACK:
             /* commit playback */
            return divxDrmCommitPlayback(pt_divx_drm_obj);

        default:
            return DRMR_INFO_NOT_SUPPORT;
    }

    /* statement is unreachable */
    /* return DRMR_OK; */
} 

/*-----------------------------------------------------------------------------
 * Name: divx_drm_get_generic_info
 *
 * Description: This API retrieves generic information of the divx drm.
 *
 * Inputs: t_drm_get_type  Specify the get information type of divx drm.
 *         pv_buf  Specify memory buffer to retrive divx drm information.
 *         z_buf_leng  Specify length of the pv_buf. If it is smaller than required, return DRM_INSUFFICENT_MEM.
 *
 * Outputs: pv_buf  Specify memory buffer contains the divx drm information.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INSUFFICIENT_BUFFER  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_INFO_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_get_generic_info (
    DRM_GET_TYPE_T t_drm_get_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    /* check buffer */
    if ((pv_buf == NULL) || (z_buf_leng < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
    {
        return DRMR_INSUFFICIENT_BUFFER;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    switch (t_drm_get_type)
    {
        case DRM_GET_TYPE_DIVX_DRM_GENERIC_REGISTRATION_INFO:        
            return divxDrmGetRegistrationCode(pv_buf);
            
        case DRM_GET_TYPE_DIVX_DRM_GENERIC_UI_HELP_INFO:
            return divxDrmGetUIHelpInfo(pv_buf);
            
        default:
            return DRMR_INFO_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_set_generic_info
 *
 * Description: This API sets generic information of the divx drm.
 *
 * Inputs: t_drm_set_type  Specify the set information type of the divx drm.
 *         pv_buf  Specify memory buffer to set the divx drm information.
 *         z_buf_leng  Specify length of the pv_buf.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INSUFFICIENT_BUFFER  Fail. 
 *          DRMR_INTERNAL_ERROR  Fail.
 *          DRMR_FCT_NOT_SUPPORT  Fail.
 *          DRMR_INFO_NOT_SUPPORT  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_set_generic_info (
    DRM_SET_TYPE_T t_drm_set_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng)
{
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    switch (t_drm_set_type)
    {
        case DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION:
            if ((pv_buf == NULL) || (z_buf_leng < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
            {
                return DRMR_INSUFFICIENT_BUFFER;
            }
            
            return divxDrmGetDeactivationCode(pv_buf);
            
        default:
            return DRMR_INFO_NOT_SUPPORT;
    }
}

