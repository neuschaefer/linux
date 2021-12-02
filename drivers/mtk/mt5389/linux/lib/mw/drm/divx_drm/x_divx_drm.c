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
 * $RCSfile: x_divx_drm.c,v $
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

#include "inc/x_common.h"
#include "drm/drm.h"
#include "drm/divx_drm/divx_drm.h"
#include "drm/divx_drm/divx_drm_mem.h"
#include "drm/divx_drm/divx_drm_glue.h"
#include "drm/divx_drm/x_divx_drm.h"

#else

#include "x_common.h"
#include "../drm.h"
#include "divx_drm.h"
#include "divx_drm_glue.h"
#include "x_divx_drm.h"

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
 * Name: x_divx_drm_init
 *
 * Description: This API initializes the divx drm.
 *
 * Inputs: pt_divx_drm_init_config  Reference contains the divx drm configuration structure.
 *          
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success. The API is successful. 
 *          DRMR_INV_ARG  Fail.
 *          DRMR_CLI_ERROR  Fail.
 *          DRMR_INITIAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/                            
INT32 x_divx_drm_init (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config)
{
    INT32 i4_ret;
    DRM_HANDLER_FCT_TBL_T t_handler_fct_tbl;
    DRM_TYPE_T t_drm_type;

    if (NULL == pt_divx_drm_init_config)
    {
        return DRMR_INV_ARG;
    }

    /* initialize drm */
    i4_ret = drm_init(DRM_TYPE_DIVX_DRM);
    if (DRMR_OK != i4_ret)
    {
        return i4_ret;
    }

    /* initialize divx drm memory and model id */
    i4_ret = divx_drm_init_glue(pt_divx_drm_init_config);
    if (DRMR_OK != i4_ret)
    {
        return i4_ret;    
    }

    t_handler_fct_tbl.pf_mopen = divx_drm_open;
    t_handler_fct_tbl.pf_close = divx_drm_close;
    t_handler_fct_tbl.pf_get_info = divx_drm_get_info;
    t_handler_fct_tbl.pf_set_info = divx_drm_set_info;    
    t_handler_fct_tbl.pf_get_generic_info = divx_drm_get_generic_info;
    t_handler_fct_tbl.pf_set_generic_info = divx_drm_set_generic_info;        
    t_drm_type = DRM_TYPE_DIVX_DRM;

    /* register divx drm */
    i4_ret = drm_register_handler(DIVX_DRM_NAME, t_drm_type, &t_handler_fct_tbl);
    if (DRMR_OK != i4_ret)
    {
        return DRMR_INITIAL_ERROR;
    }

    return DRMR_OK;
}

#ifndef __KERNEL__
/*-----------------------------------------------------------------------------
 * Name: x_divx_drm_mem_info
 *
 * Description: This API provides the divx drm memory info.
 *
 * Inputs: pui2_divx_drm_memory_offset  Reference to retrieve the divx drm offset in whole eeprom.
 *         pui2_divx_drm_memory_size_byte  Reference to retrieve the divx drm size in byte.
 *
 * Outputs: pui2_divx_drm_memory_offset  Reference contains the divx drm offset in whole eeprom.
 *          pui2_divx_drm_memory_size_byte  Reference contains the divx drm size in byte.
 *
 * Returns: NONE.
 ----------------------------------------------------------------------------*/                            
VOID x_divx_drm_mem_info (
    UINT16* pui2_divx_drm_memory_offset,
    UINT16* pui2_divx_drm_memory_size_byte)
{
    if (NULL == pui2_divx_drm_memory_offset ||
        NULL == pui2_divx_drm_memory_size_byte)
    {
        return;
    }
    
    divx_drm_mem_info(pui2_divx_drm_memory_offset, pui2_divx_drm_memory_size_byte);
}
#endif

