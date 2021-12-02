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
 * $RCSfile: drm.c,v $
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
#include "util/x_lnk_list.h"
#include "drm/x_drm.h"
#include "drm/drm.h"
#include "drm/drm_cli.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_lnk_list.h"
#include "x_drm.h"
#include "drm.h"
#include "drm_cli.h"

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
static SLIST_T(_DRM_HANDLER_REG_T) t_drm_handler_reg_list = {NULL};
static BOOL b_drm_init = FALSE;

/* semaphore to protect drm x-api */
HANDLE_T gh_drm_sema = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: drm_register_handler
 *
 * Description: This API registers the drm handler.
 *
 * Inputs: ps_name  Specify sixteen characters of a handler name, and the character is case sensitive.
 *         t_drm_type  Specify the drm type.
 *         pt_handler_fct_tbl  Specify the handler function table.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_INV_ARG  Fail.
 *          DRMR_HANDLER_NAME_EXIST  Fail.
 *          DRMR_OUT_OF_MEMORY  Fail.
 *          DRMR_INITIAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 drm_register_handler (  
    const CHAR* ps_name,    
    const DRM_TYPE_T t_drm_type,
    const DRM_HANDLER_FCT_TBL_T* pt_handler_fct_tbl)
{
    DRM_HANDLER_REG_T* pt_handler_reg = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_name == NULL) || (pt_handler_fct_tbl == NULL))
    {
        return DRMR_INV_ARG;
    }
    
    /************************************************************************/
    /* Perform register handler                                              */
    /************************************************************************/
    /* check if the engine exist */
    SLIST_FOR_EACH(pt_handler_reg, &t_drm_handler_reg_list, t_link)
    {
        if (x_strncmp(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN) == 0)
        {
            return DRMR_HANDLER_NAME_EXIST;
        }
    }

    /* malloc node */
    pt_handler_reg = x_mem_alloc(sizeof(DRM_HANDLER_REG_T));
    if (pt_handler_reg == NULL)
    {
        return DRMR_OUT_OF_MEMORY;
    }
    x_memset(pt_handler_reg, 0, sizeof(DRM_HANDLER_REG_T));

    /* fill data */
    x_strncpy(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN);
    pt_handler_reg->t_drm_type = t_drm_type;
    /* pf_mopen and pf_close must be set */
    if (pt_handler_fct_tbl->pf_mopen != NULL && pt_handler_fct_tbl->pf_close != NULL)
    {
        pt_handler_reg->t_handler_fct_tbl.pf_mopen = pt_handler_fct_tbl->pf_mopen;
        pt_handler_reg->t_handler_fct_tbl.pf_close = pt_handler_fct_tbl->pf_close;
    } 
    else
    {
       return DRMR_INITIAL_ERROR;
    }
    pt_handler_reg->t_handler_fct_tbl.pf_get_info = pt_handler_fct_tbl->pf_get_info;
    pt_handler_reg->t_handler_fct_tbl.pf_set_info = pt_handler_fct_tbl->pf_set_info;
    pt_handler_reg->t_handler_fct_tbl.pf_get_generic_info = pt_handler_fct_tbl->pf_get_generic_info;
    pt_handler_reg->t_handler_fct_tbl.pf_set_generic_info = pt_handler_fct_tbl->pf_set_generic_info;

    /* add new node */
    SLIST_INSERT_HEAD(pt_handler_reg, &t_drm_handler_reg_list, t_link);

    return DRMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: drm_match_handler
 *
 * Description: This API matches the drm type and retrieves the handler function table.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *         pt_handler_fct_tbl  Specify pointer to retrive the handler function table which match the drm type. 
 *
 * Outputs: pt_handler_fct_tbl  Specify pointer contains the handler function table which match the drm type. 
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed. 
 *          DRMR_INV_ARG  Fail.
 *          DRMR_HANDLER_NAME_EXIST  Fail.
 ----------------------------------------------------------------------------*/
INT32 drm_match_handler (
    const DRM_TYPE_T t_drm_type, 
    DRM_HANDLER_FCT_TBL_T* pt_handler_fct_tbl)
{
    DRM_HANDLER_REG_T* pt_handler_reg = NULL; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_handler_fct_tbl == NULL)
    {
        return DRMR_INV_ARG;
    }

    /************************************************************************/
    /* Perform match handler                                              */
    /************************************************************************/
    SLIST_FOR_EACH(pt_handler_reg, &t_drm_handler_reg_list, t_link)
    {
        if (t_drm_type == pt_handler_reg->t_drm_type)
        {
            *pt_handler_fct_tbl = pt_handler_reg->t_handler_fct_tbl;
            return DRMR_OK;
        }
    }

    return DRMR_HANDLER_NOT_SUPPORT;
}

/*-----------------------------------------------------------------------------
 * Name: drm_init
 *
 * Description: This API is a selector's API and is called by system initial procedure when system boot-up to init drm module.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The routine was successful and the operation has been completed.
 *          DRMR_CLI_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 drm_init (
    DRM_TYPE_T t_drm_type)
{
    /* could be called multiple times */
#ifdef CLI_SUPPORT
    /* initialize drm CLI component */
    if (drm_cli_init(t_drm_type) != DRMR_OK)
    {
        return DRMR_CLI_ERROR;
    } 
#endif

    if (b_drm_init == FALSE)
    {
        /* create semaphore */   
        if (x_sema_create(&gh_drm_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            return DRMR_INTERNAL_ERROR;
        }
    
        b_drm_init = TRUE;
    }
    
    return DRMR_OK;
}

