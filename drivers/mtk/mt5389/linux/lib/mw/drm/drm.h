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
 * $RCSfile: drm.h,v $
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

#ifndef _DRM_H_
#define _DRM_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "util/x_lnk_list.h"
#include "drm/x_drm.h"

#else

#include "x_common.h"
#include "x_lnk_list.h"
#include "x_drm.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* The following lists all handles for drm module. */
#define HT_DRM_MEM_TYPE (HT_GROUP_DRM + (HANDLE_TYPE_T)0)

/* This defines maximum length of handler name. */
#define MAX_HANDLER_NAME_LEN 16

/* This API opens a drm object according to the drm command set. */
typedef INT32 (*drm_handler_mem_open_fct) (DRM_COMMAND_T* pt_cmd_sets, VOID** ppv_obj);

/* This API closes the drm object. */
typedef INT32 (*drm_handler_close_fct) (VOID* pv_obj); 

/* This API retrives information of the drm object. */
typedef INT32 (*drm_handler_get_info_fct) (VOID* pv_obj, DRM_GET_TYPE_T e_info_type, VOID* pv_buf, SIZE_T z_buf_leng); 

/* This API sets information of the drm object. */
typedef INT32 (*drm_handler_set_info_fct) (VOID* pv_obj, DRM_SET_TYPE_T e_info_type, VOID* pv_buf, SIZE_T z_buf_leng); 

/* This API retrives information of the specific drm type. */
typedef INT32 (*drm_handler_get_generic_info_fct) (DRM_GET_TYPE_T e_info_type, VOID* pv_buf, SIZE_T z_buf_leng); 

/* This API sets information of the specific drm type. */
typedef INT32 (*drm_handler_set_generic_info_fct) (DRM_SET_TYPE_T e_info_type, VOID* pv_buf, SIZE_T z_buf_leng); 

/* This structure holds the function table for drm handler registration. */
typedef struct _DRM_HANDLER_FCT_TBL_T
{
    drm_handler_mem_open_fct pf_mopen;
    drm_handler_close_fct pf_close;
    drm_handler_get_info_fct pf_get_info;
    drm_handler_set_info_fct pf_set_info;
    drm_handler_get_generic_info_fct pf_get_generic_info;
    drm_handler_set_generic_info_fct pf_set_generic_info;
} DRM_HANDLER_FCT_TBL_T;

/* This structure holds the register table for drm handler registration. */
typedef struct _DRM_HANDLER_REG_T
{
    CHAR s_handler_name[MAX_HANDLER_NAME_LEN+1];
    DRM_TYPE_T t_drm_type;
    DRM_HANDLER_FCT_TBL_T t_handler_fct_tbl;    
    SLIST_ENTRY_T(_DRM_HANDLER_REG_T) t_link;
} DRM_HANDLER_REG_T;

/* This structure defines the drm object. */
typedef struct _DRM_OBJ_T
{
    DRM_HANDLER_FCT_TBL_T t_handler_fct_tbl;    

    /* specific drm object */
    VOID* pv_specific_drm_obj;    
} DRM_OBJ_T;

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This API registers the drm handler. */
extern INT32 drm_register_handler (  
    const CHAR* ps_name,    
    const DRM_TYPE_T t_drm_type,
    const DRM_HANDLER_FCT_TBL_T* pt_handler_fct_tbl);

/* This API matches the drm type and retrieves the handler function table. */
extern INT32 drm_match_handler (
    const DRM_TYPE_T  t_drm_type, 
    DRM_HANDLER_FCT_TBL_T* pt_handler_fct_tbl);

/* This API is a selector's API and is called by system initial procedure when system boot-up to init drm module. */
extern INT32 drm_init (
    DRM_TYPE_T t_drm_type);

#ifdef __cplusplus
}
#endif

#endif /* _DRM_H_ */

