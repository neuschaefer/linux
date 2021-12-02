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
 * $RCSfile: c_mm_svctx.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: a7b5992f37975582d6d8dbacc02ac58d $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#ifdef ENABLE_MULTIMEDIA
#include "svctx/x_svctx.h"
#include "handle/handle.h"
#include "svctx/svctx_dbg.h"
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_open
 * Description 
 *  
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_open(
                    HANDLE_T                        h_svctx,
                    MM_SVC_COMMAND_T*               pt_cmd,
                    x_svctx_media_prc_nfy_fct       pf_nfy,
                    VOID*                           pv_tag,
                    HANDLE_T*                       ph_media                      
                    )
{
    
    return x_svctx_media_open(
                    h_svctx,
                    pt_cmd,
                    pf_nfy,
                    pv_tag,
                    ph_media
                   );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_close
 * Description 
 *      This API is used to perfrom media file or clip close function 
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_close(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media
                    )
{
    return x_svctx_media_close(
                   h_svctx,
                   h_media
                 );
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_get_info
 * Description 
 *      This API is used to get media information.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_get_info(
    HANDLE_T h_svctx,                  
    HANDLE_T h_media,
    SVCTX_GET_TYPE_T t_svctx_get_type,
    VOID* pv_get_info,
    SIZE_T z_get_info_size)
{
    
    return x_svctx_media_get_info(
                    h_svctx,
                    h_media,
                    t_svctx_get_type,
                    pv_get_info,
                    z_get_info_size
                   );
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_set_info
 * Description 
 *      This API is used to get media information.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_set_info(
                             HANDLE_T           h_svctx,                  
                             HANDLE_T           h_media,
                             SVCTX_SET_TYPE_T   t_svctx_set_type,
                             VOID*              pv_set_info,
                             SIZE_T             z_set_info_size)
{
    
    return x_svctx_media_set_info(
                                  h_svctx,
                                  h_media,
                                  t_svctx_set_type,
                                  pv_set_info,
                                  z_set_info_size
                                 );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_generic_get_info
 * Description 
 *      This API is used to get generic media information. It doesn't need to open media first.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_generic_get_info(
    HANDLE_T h_svctx,                  
    HANDLE_T h_media,
    SVCTX_GET_TYPE_T t_svctx_get_type,
    VOID* pv_get_info,
    SIZE_T z_get_info_size)
{
    
    return x_svctx_generic_get_info(
                    h_svctx,
                    h_media,
                    t_svctx_get_type,
                    pv_get_info,
                    z_get_info_size
                   );
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_generic_set_info
 * Description 
 *      This API is used to set generic media information. It doesn't need to open media first.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_generic_set_info(
    HANDLE_T h_svctx,                  
    HANDLE_T h_media,
    SVCTX_SET_TYPE_T t_svctx_set_type,
    VOID* pv_set_info,
    SIZE_T z_set_info_size)
{
    
    return x_svctx_generic_set_info(
                    h_svctx,
                    h_media,
                    t_svctx_set_type,
                    pv_set_info,
                    z_set_info_size
                   );
}


/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_seek
 * Description 
 *      This API is used to perfrom media search function 
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_seek(
                    HANDLE_T                        h_svctx,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_tag,
                    MM_SVC_POS_INFO_T*              pt_pos_info
                    )
{
    return x_svctx_media_seek(
                   h_svctx,
                   pf_nfy,
                   pv_tag,
                   pt_pos_info
                );
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_load_thumbnail
 * Description 
 *      This API is used to load video thumbnail
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media,
                    SVCTX_VIDEO_THUMBNAIL_INFO_T*   pt_thumbnail_info
                    )
{
    return x_svctx_media_load_thumbnail(
                    h_svctx,
                    h_media,
                    pt_thumbnail_info );
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_sync_cancel_load_thumbnail
 * Description 
 *      This API is used to sync cancel the loading of video thumbnail
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_sync_cancel_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media
                    )
{
    return x_svctx_media_sync_cancel_load_thumbnail(
                    h_svctx,
                    h_media );
}
#endif

