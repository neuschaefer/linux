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
 * $RCSfile: x_img.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the image library interface APIs  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "inc/common.h"
#include "inc/u_sys_name.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/scc/_scc_vid.h"
#include "strm_mngr/scc/_scc_disp.h"
#include "res_mngr/rm.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_gfx.h"
#include "res_mngr/drv/x_vid_dec.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "cap/cap.h"
#include "cap/u_cap.h"
#include "cap/cap_cli.h"
#include "img/x_img.h"
#include "graphic/x_gl.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#ifdef ANDROID
#include <sys/shm.h>
#include "stdio.h"
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
//BOOL b_test = 0;
//static HANDLE_T    h_cap_stop_sema = NULL_HANDLE;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 * Name
 *      _cap_handle_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _cap_handle_free_fct(
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    BOOL                        b_req_handle
                    )
{
    if(e_type != HT_CAP_TYPE)
    {
        return FALSE;
    }
    x_mem_free(pv_obj);
    
    return TRUE;
}

static INT32 _cap_get_obj_by_handle(HANDLE_T             h_cap,
                                    CAP_OBJ_T**          ppt_cap_obj)
{
   HANDLE_TYPE_T                    e_type;
   INT32                            i4_ret;

   i4_ret = handle_get_type_obj(h_cap, &e_type, (VOID**)ppt_cap_obj);
   if ( i4_ret != HR_OK 
        || e_type != HT_CAP_TYPE 
        || *ppt_cap_obj == NULL)
   {
       if (i4_ret == HR_INV_HANDLE)
       {
           i4_ret = CAPR_INV_HANDLE;
       }
       return i4_ret;
   }
   else
   {
       return CAPR_OK;
   }
}

/*------------------------------------------------------------------*/
/*! @brief  x_cap_open
 *             This function will freeze the screen , and return the capture handle. 
 *  @param [in] e_source             Specifies the source which will be captured.
 *  @param [in] pf_nfy               Specified the callback function to receive the notification of image library.
 *  @param [in] pv_nfy_tag           The client data specified by the client and the callback function will send it again.
 *  @param [in] pf_cust              Specified the function to customize user's operation when creating handle.
 *  @param [in] pv_cust_tag          The client data specified by the client and the callback function will send it again.
 *  @param [out] ph_cap              Specified the capture handle which retrieve capture information about the source.
 *  @return Return the error code.
 *  @retval CAPR_OK                  Success. The capture open operation is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_open (
                  CAP_SRC_TYPE_T  e_source,        /* (I) specifies the open mode of the open operation*/ 
                  x_cap_nfy_fct   pf_nfy,          /* (I) specify the callback function to receive the notification of image library. */ 
                  const VOID*     pv_nfy_tag,      /* (I) The client data specified by the client and the callback function will send it again. */ 
                  x_cap_cust_fct  pf_cust,         /* (I) Specified the function to customize user's operation when creating handle. */ 
                  const VOID*     pv_cust_tag,     /* (I) The client data specified by the client and the callback function will send it again. */ 
                  HANDLE_T*       ph_cap)          /* (O) Specified the capture handle which retrieve capture information about the source. */
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    do
    {
        pt_cap_obj = x_mem_alloc(sizeof(CAP_OBJ_T));
        if(pt_cap_obj == NULL)
        {
           i4_ret = CAPR_OUT_OF_MEMORY;
           break;
        }
        x_memset(pt_cap_obj, 0, sizeof(CAP_OBJ_T));

        pt_cap_obj->e_source    = e_source;
        pt_cap_obj->pf_nfy      = pf_nfy;
        pt_cap_obj->pv_nfy_tag  = pv_nfy_tag;
        pt_cap_obj->pf_cust     = pf_cust;
        pt_cap_obj->pv_cust_tag = pv_cust_tag;
        
        i4_ret = x_sema_create(&(pt_cap_obj->h_cap_cancel_exe_sema), 
                                X_SEMA_TYPE_MUTEX, 
                                X_SEMA_STATE_UNLOCK);
        if(i4_ret != OSR_OK)
        {
            i4_ret = CAPR_OUT_OF_HANDLE;
            break;
        }
        
        i4_ret = handle_alloc(  HT_CAP_TYPE,
                                pt_cap_obj,
                                NULL,
                                _cap_handle_free_fct,
                                ph_cap);
        if(i4_ret != HR_OK)
        {
            i4_ret = CAPR_OUT_OF_HANDLE;
            break;
        }
        pt_cap_obj->h_cap = *ph_cap;
    }while(0);

    if(i4_ret != HR_OK)
    {
        if(pt_cap_obj != NULL)
        {
            if(pt_cap_obj->h_cap_cancel_exe_sema != NULL_HANDLE)
            {
                x_sema_delete(pt_cap_obj->h_cap_cancel_exe_sema);
                pt_cap_obj->h_cap_cancel_exe_sema = NULL_HANDLE;
            }
            x_mem_free(pt_cap_obj);
        }
    }
    return i4_ret;
}


static VOID cap_video_plane_rm_nfy (    HANDLE_T   h_comp,
                                     UINT8      ui1_port,
                                     RM_COND_T  e_nfy_cond,
                                     VOID*      pv_tag,
                                     UINT32     ui4_data)
{
    /* nothing */
}
    
static INT32 _get_video_cap_inp_info(HANDLE_T   h_cap)
{
    INT32       i4_ret      = 0;
    UINT16      ui2_index   = 0;
    UINT16      ui2_id      = 0;
    SIZE_T      z_size      = 0;
    RM_COND_T   e_comp_cond = 0;
    CAP_OBJ_T*  pt_cap_obj  = NULL;
    
    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;
    }
    
    i4_ret = rm_get_comp_info(DRVT_VID_PLANE,
                              ui2_index,
                              &ui2_id,
                              NULL, 
                              NULL, 
                              NULL);
    if(i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] rm_get_comp_info fail to get video id! i4_ret = %d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }
    else
    {
        HANDLE_T   h_rm_vid_plane;
        RM_COND_T  e_rm_cond;
        
        i4_ret = rm_open_comp(NULL_HANDLE,
                              OBJ_COMP_REL_IGNORE,
                              DRVT_VID_PLANE,
                              ui2_id,
                              NULL,
                              CTRL_TYPE_MONITOR,
                              NULL, /* pv_comp_sel_data */
                              NULL, /* pf_comp_sel */
                              NULL, /* pv_tag */
                              cap_video_plane_rm_nfy,
                              & h_rm_vid_plane,
                              & e_rm_cond);
        
        if (i4_ret == RMR_OK)
        {
            z_size = sizeof(CAP_INPUT_T);
            i4_ret = rm_get( h_rm_vid_plane,
                             FALSE,
                             ANY_PORT_NUM,
                             VID_PLA_GET_TYPE_DRAMINF,
                             (VOID*)(&(pt_cap_obj->t_cap_input)),
                             &z_size,
                             &e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                x_dbg_stmt("[cap] rm_get_comp_info fail to get video id! i4_ret=%d\r\n", i4_ret);
                return CAPR_INTERNAL_ERR;
            }
            rm_close(h_rm_vid_plane, FALSE, NULL);
        }
        else
        {
            x_dbg_stmt("[cap]Could not open audio decoder %u (ret=%d)\n", ui2_id, i4_ret);
            return CAPR_INTERNAL_ERR;
        }
    }
    
#ifdef APP_CAPTURE_SPECIAL_AREA_SUPPORT
    pt_cap_obj->t_cap_input.t_rect.i4_left = (UINT32) pt_cap_obj->t_cap_input.t_rect.i4_left * pt_cap_obj->t_cap_input.ui4_src_width * 1.0 / pt_cap_obj->ui4_moveable_width;
    pt_cap_obj->t_cap_input.t_rect.i4_top  = (UINT32) pt_cap_obj->t_cap_input.t_rect.i4_top * pt_cap_obj->t_cap_input.ui4_src_height * 1.0 / pt_cap_obj->ui4_moveable_height;
    pt_cap_obj->t_cap_input.t_rect.i4_right= (UINT32) pt_cap_obj->t_cap_input.t_rect.i4_right * pt_cap_obj->t_cap_input.ui4_src_width * 1.0 / pt_cap_obj->ui4_moveable_width;
    pt_cap_obj->t_cap_input.t_rect.i4_bottom= (UINT32)pt_cap_obj->t_cap_input.t_rect.i4_bottom * pt_cap_obj->t_cap_input.ui4_src_height * 1.0 / pt_cap_obj->ui4_moveable_height;

	pt_cap_obj->t_cap_input.t_rect.i4_left = (pt_cap_obj->t_cap_input.t_rect.i4_left >> 1) << 1;
	pt_cap_obj->t_cap_input.t_rect.i4_right = (pt_cap_obj->t_cap_input.t_rect.i4_right >> 1) << 1;
    if (pt_cap_obj->t_cap_input.t_rect.i4_right == 0 || pt_cap_obj->t_cap_input.t_rect.i4_bottom == 0)
    {
        pt_cap_obj->t_cap_input.t_rect.i4_left = 0;
        pt_cap_obj->t_cap_input.t_rect.i4_top  = 0;
        pt_cap_obj->t_cap_input.t_rect.i4_right= (INT32)pt_cap_obj->t_cap_input.ui4_src_width;
        pt_cap_obj->t_cap_input.t_rect.i4_bottom= (INT32)pt_cap_obj->t_cap_input.ui4_src_height;
    }
#else
    {
        pt_cap_obj->t_cap_input.t_rect.i4_left = 0;
        pt_cap_obj->t_cap_input.t_rect.i4_top  = 0;
        pt_cap_obj->t_cap_input.t_rect.i4_right= (INT32)pt_cap_obj->t_cap_input.ui4_src_width;
        pt_cap_obj->t_cap_input.t_rect.i4_bottom= (INT32)pt_cap_obj->t_cap_input.ui4_src_height;
    }
#endif

    return CAPR_OK;
}


static INT32 _get_photo_cap_inp_info(HANDLE_T   h_cap)
{
    INT32               i4_ret  = 0;
    UINT8               ui1_bytes_per_pixel = 0;
    CAP_OBJ_T*          pt_cap_obj  = NULL;
    GL_SURF_INFO_T      t_surf_info;
    GL_LOCK_INFO_T      t_lock_info;

    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;
    }

    /*----------------Resize image first-------------------------*/
    i4_ret = x_gl_surface_get_info(
                                pt_cap_obj->h_src_surf, 
                                &t_surf_info);
    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("x_gl_surface_get_info Fail i4_ret = %d!\r\n", i4_ret);
        return i4_ret;
    }

    if(pt_cap_obj->e_res_type == CAP_OUT_RES_TYPE_USER)
    {
        i4_ret = x_gl_surface_stretch_blt(
                                            pt_cap_obj->h_working_surf, 
                                            pt_cap_obj->h_src_surf, 
                                            0,
                                            0,
                                            pt_cap_obj->t_cap_output.ui4_dst_width,
                                            pt_cap_obj->t_cap_output.ui4_dst_height,
                                            0,
                                            0,
                                            t_surf_info.ui4_width,
                                            t_surf_info.ui4_height,
                                            0);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("x_gl_surface_stretch_blt Fail i4_ret = %d!\r\n", i4_ret);
            return i4_ret;
        }
        /*----------------Get Image Data buffer----------------------*/
        i4_ret = x_gl_surface_lock(pt_cap_obj->h_working_surf,
                                   NULL,
                                   GL_SYNC,
                                   &t_lock_info);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("[cap] x_gl_surface_lock fail! i4_ret=%d\r\n", i4_ret);
            return CAPR_INTERNAL_ERR;
        }
    }
    else
    {
        /*----------------Get Image Data buffer----------------------*/
        i4_ret = x_gl_surface_lock(pt_cap_obj->h_src_surf,
                                   NULL,
                                   GL_SYNC,
                                   &t_lock_info);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("[cap] x_gl_surface_lock fail! i4_ret=%d\r\n", i4_ret);
            return CAPR_INTERNAL_ERR;
        }
    }
    
    
    pt_cap_obj->t_cap_input.puac_src_buffer = (UCHAR*)(t_lock_info.at_subplane[0].pv_bits);

    pt_cap_obj->t_cap_input.e_color_format
        = (CAP_COLOR_FORMAT_T)(t_surf_info.e_clrmode + PHOTO_COLOR_MODE_START);
    pt_cap_obj->t_cap_input.ui4_src_width = t_surf_info.ui4_width;
    pt_cap_obj->t_cap_input.ui4_src_height= t_surf_info.ui4_height;
    switch((GFX_COLORMODE_T)t_surf_info.e_clrmode)
    {  
    case GFX_COLORMODE_AYCbCr_CLUT2:
    case GFX_COLORMODE_AYCbCr_CLUT4:
    case GFX_COLORMODE_ARGB_CLUT2:
    case GFX_COLORMODE_ARGB_CLUT4:
    case GFX_COLORMDOE_YUV_420_BLK:
    case GFX_COLORMODE_YUV_420_RS:
        ui1_bytes_per_pixel = 0;
        break;
    case GFX_COLORMODE_AYCbCr_CLUT8:
    case GFX_COLORMODE_CbYCrY_16:
    case GFX_COLORMODE_YCbYCr_16:
    case GFX_COLORMODE_ARGB_CLUT8:
    case GFX_COLORMODE_RGB_D565:
    case GFX_COLORMODE_ARGB_D1555:
    case GFX_COLORMODE_ARGB_D4444: 
        ui1_bytes_per_pixel = 2; 
        break;
    case GFX_COLORMODE_AYCbCr_D8888:
    case GFX_COLORMODE_ARGB_D8888:
    case GFX_COLORMDOE_YUV_422_BLK:
    case GFX_COLORMODE_YUV_422_RS:
    case GFX_COLORMDOE_YUV_444_BLK:
    case GFX_COLORMODE_YUV_444_RS:
        ui1_bytes_per_pixel = 4;
        break;
    }

    pt_cap_obj->t_cap_input.ui4_src_pitch = t_lock_info.at_subplane[0].ui4_pitch;
    pt_cap_obj->t_cap_input.ui4_src_width = t_surf_info.ui4_width;
    pt_cap_obj->t_cap_input.ui4_buffer_length
        = pt_cap_obj->t_cap_input.ui4_src_pitch * t_surf_info.ui4_width;
    pt_cap_obj->t_cap_input.t_rect.i4_left = 0;
    pt_cap_obj->t_cap_input.t_rect.i4_top = 0;
    pt_cap_obj->t_cap_input.t_rect.i4_right = (INT32)t_surf_info.ui4_width;
    pt_cap_obj->t_cap_input.t_rect.i4_bottom = (INT32)t_surf_info.ui4_height;

    return CAPR_OK;
}

/*------------------------------------------------------------------*/
/*! @brief  c_cap_capture
 *              This function will open an image memory block, parse the image header, and return the image handle.
 *  @param [in] h_cap               Specifies which handle will do the operation.
 *  @param [in] t_cap_info          Specifies the output image detail information.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The capture operation is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_capture ( 
                     HANDLE_T                       h_cap,           /* (I) Specifies which handle will do the operation. */ 
                     const CAP_CAPTURE_INFO_T*      pt_cap_info)      /* (I) Specifies the output image detail information. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

	x_dbg_stmt("[mw/CAP] start x_cap_capture.\n");
    if((h_cap == NULL_HANDLE) || (pt_cap_info == NULL))
    {
        return CAPR_INV_ARG;
    }
#ifdef APP_CAPTURE_SPECIAL_AREA_SUPPORT
	//check rectangle
	if (pt_cap_info->t_rec.i4_bottom < pt_cap_info->t_rec.i4_top ||
		pt_cap_info->t_rec.i4_right < pt_cap_info->t_rec.i4_left)
	{
		x_dbg_stmt("[CAP] capture rectangle from app is wrong.\n");
		return CAPR_INV_ARG;
	}
#endif
    /*----------------- Save capture input info-----------------------*/
    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;;
    }
	
    pt_cap_obj->b_cancel_exe_flag = FALSE;

    if(pt_cap_info->e_format == CAP_IMG_FMT_TYPE_JPEG)
    {
        pt_cap_obj->t_cap_output.e_out_pic_fmt = CAP_OUTPUT_PIC_FORMAT_JPEG;
    }
    else/*default is jpeg*/
    {
        pt_cap_obj->t_cap_output.e_out_pic_fmt = CAP_OUTPUT_PIC_FORMAT_MPEG;
    }       
    pt_cap_obj->e_res_type = pt_cap_info->e_res_type;
    pt_cap_obj->t_cap_input.ui4_max_len = pt_cap_info->ui4_max_size;
    pt_cap_obj->t_cap_input.ui4_quality = pt_cap_info->ui4_quality;
    pt_cap_obj->h_src_surf = pt_cap_info->h_src_surf;
    pt_cap_obj->h_working_surf = pt_cap_info->h_working_surf;

#ifdef APP_CAPTURE_SPECIAL_AREA_SUPPORT
    pt_cap_obj->t_cap_input.t_rect = pt_cap_info->t_rec;
    pt_cap_obj->ui4_moveable_height = pt_cap_info->ui4_moveable_height;
    pt_cap_obj->ui4_moveable_width = pt_cap_info->ui4_moveable_width;
#endif
    pt_cap_obj->t_cap_output.ui4_dst_width = pt_cap_info->u.t_resolution.ui4_res_width;
    pt_cap_obj->t_cap_output.ui4_dst_height = pt_cap_info->u.t_resolution.ui4_res_height;

	pt_cap_obj->t_cap_input.ui1_video_path = pt_cap_info->ui1_video_path;

#ifdef ANDROID
	//for android photo
	if(pt_cap_obj->e_source == CAP_SRC_TYPE_MM_IMAGE_ANDROID)
	{
		pt_cap_obj->t_cap_input.e_color_format = pt_cap_info->e_colormode;
		pt_cap_obj->t_cap_input.ui4_src_width = pt_cap_info->ui4_image_width;
		pt_cap_obj->t_cap_input.ui4_src_height = pt_cap_info->ui4_image_height;
		pt_cap_obj->t_cap_input.ui4_src_pitch = pt_cap_info->ui4_image_pitch;

		{
			INT64 p_map = 0;
			p_map = (INT64)shmat(pt_cap_info->i4_shm_id, NULL, 0);
			if (p_map <= 0)
			{
				x_dbg_stmt("[CAP]{%s %d}shmat error;\n", __FUNCTION__, __LINE__);
				return CAPR_INTERNAL_ERR;
			}

			x_dbg_stmt("[CAP]{%s %d}shmat ok. p_map=0x%x.\n", __FUNCTION__, __LINE__, p_map);

			pt_cap_obj->t_cap_input.puac_src_buffer = (UCHAR*)p_map;
		}

		x_dbg_stmt("[CAP]Capture android image. color_foramt=%d, src_width=%d, src_height=%d, src_pitch=%d.\n",
					pt_cap_obj->t_cap_input.e_color_format, pt_cap_obj->t_cap_input.ui4_src_width, pt_cap_obj->t_cap_input.ui4_src_height, pt_cap_obj->t_cap_input.ui4_src_pitch);

	}
#endif
    cap_logo_send_notify_msg (pt_cap_obj->h_cap,
                              CAP_LOGO_MSG_ASYNC_EXEC_TYPE,
                              CAP_EVENT_TYPE_DO_CAPTURING);
    return i4_ret;
}

INT32 x_cap_do_capturing(HANDLE_T   h_cap)
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;
    UCHAR*              puc_buf = NULL;

    if(h_cap == NULL_HANDLE)
    {
        return CAPR_INV_ARG;
    }
    
    do
    {
        /*----------------- Step 1: Get capture input info-----------------------*/
        i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
        if (i4_ret != CAPR_OK)
        {
            x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
            break;
        }
        
        if((pt_cap_obj->e_source == CAP_SRC_TYPE_TV_VIDEO) ||
           (pt_cap_obj->e_source == CAP_SRC_TYPE_MM_VIDEO))
        {
            i4_ret = _get_video_cap_inp_info(h_cap);
        }
        else if(pt_cap_obj->e_source == CAP_SRC_TYPE_MM_IMAGE)
        {
            i4_ret = _get_photo_cap_inp_info(h_cap);
        }
#ifdef ANDROID
		else if(pt_cap_obj->e_source ==  CAP_SRC_TYPE_MM_IMAGE_ANDROID)
		{
			pt_cap_obj->t_cap_input.ui1_data_src = 0; //image
			pt_cap_obj->t_cap_input.ui4_buffer_length
		        = pt_cap_obj->t_cap_input.ui4_src_pitch * pt_cap_obj->t_cap_input.ui4_src_width;
		    pt_cap_obj->t_cap_input.t_rect.i4_left = 0;
		    pt_cap_obj->t_cap_input.t_rect.i4_top = 0;
		    pt_cap_obj->t_cap_input.t_rect.i4_right = (INT32)pt_cap_obj->t_cap_input.ui4_src_width;
		    pt_cap_obj->t_cap_input.t_rect.i4_bottom = (INT32)pt_cap_obj->t_cap_input.ui4_src_height;

			pt_cap_obj->t_cap_input.ui4_src_pitch >>= 4;
			x_dbg_stmt("[CAP] source type is CAP_SRC_TYPE_MM_IMAGE_ANDROID.\n");
        }
#endif
        else
        {
            x_dbg_stmt("[CAP] source type not allowed\r\n");
            i4_ret = CAPR_INTERNAL_ERR;
            break;
        }
        
        if (i4_ret != SMR_OK) 
        {
            x_dbg_stmt("[CAP] Get capture input info failed, ret=%d\r\n", i4_ret);
            i4_ret = CAPR_INTERNAL_ERR;
            break;
        }

        /*----------------- Step 2: Prepare capture output info------------------*/
        puc_buf = x_mem_alloc(sizeof(UCHAR) * pt_cap_obj->t_cap_input.ui4_max_len);
        if(puc_buf == NULL)
        {
            x_dbg_stmt("[CAP] Allocate mem failed\r\n");
           i4_ret = CAPR_OUT_OF_MEMORY;
           break;
        }
        pt_cap_obj->t_cap_input.puac_dst_buffer = puc_buf;
        pt_cap_obj->t_cap_output.ui4_buffer_len 
                                = pt_cap_obj->t_cap_input.ui4_buffer_length;
        
        /*----------------- Step 3: Do compress  ------------------*/
        i4_ret = x_img_capture( &pt_cap_obj->t_cap_input,
                                &pt_cap_obj->t_cap_output);
        if(i4_ret != CAPR_OK)
        {
            x_dbg_stmt("[CAP]x_img_capture() failed, ret=%d\r\n", i4_ret);
            break;
        }
        else if(pt_cap_obj->b_cancel_exe_flag)
        {
            x_dbg_stmt("[CAP]x_img_capture return OK but have canceled\r\n");
        }
    }while(0);

    if((i4_ret == CAPR_OK) && !pt_cap_obj->b_cancel_exe_flag)
    {
        x_dbg_stmt("[CAP]x_cap_do_capturing() successful !!\r\n");
        pt_cap_obj->e_cur_event = CAP_EVENT_TYPE_CAP_DONE;
        pt_cap_obj->pf_nfy( pt_cap_obj->h_cap,
                            pt_cap_obj->pv_nfy_tag,
                            CAP_EVENT_TYPE_CAP_DONE,
                            (const VOID*)i4_ret);
    }
    else
    {
        x_dbg_stmt("[CAP]x_cap_do_capturing() failed !!\r\n");
        pt_cap_obj->e_cur_event = CAP_EVENT_TYPE_CAP_ERR;
        pt_cap_obj->pf_nfy( pt_cap_obj->h_cap,
                            pt_cap_obj->pv_nfy_tag,
                            CAP_EVENT_TYPE_CAP_ERR,
                            (const VOID*)i4_ret);
    }

    return i4_ret;
}


/*------------------------------------------------------------------*/
/*! @brief  x_cap_get_capture_data
 *             After the capture operation done, save the image to the specified path.
 *  @param [in]  h_cap               Specified the capture operation handle.
 *  @param [out] pc_buf              Specified the buffer to be output.
 *  @param [out] pui4_len            Specified the length to be output.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The image file opening is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
INT32 x_cap_get_capture_data (
                              HANDLE_T      h_cap,     /* (I) Specified the capture operation handle. */ 
                              UCHAR**       pc_buf,    /* (I) Specified the buffer to save. */ 
                              UINT32*       pui4_len)  /* (I) Specified the buffer length to save. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    if((h_cap == NULL_HANDLE) || (pc_buf == NULL) || (pui4_len == NULL))
    {
        return CAPR_INV_ARG;
    }
    if(*pui4_len < pt_cap_obj->t_cap_output.ui4_buffer_len)
    {
        return CAPR_INV_ARG;
    }
    
    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return CAPR_INTERNAL_ERR;
    }
    if(pt_cap_obj->e_cur_event != CAP_EVENT_TYPE_CAP_DONE)
    {
        return CAPR_FCT_NOT_SUPPORT;
    }
    *pc_buf     = pt_cap_obj->t_cap_output.puac_dst_buffer;
    *pui4_len   = pt_cap_obj->t_cap_output.ui4_buffer_len;
  
    return i4_ret;
}

/*------------------------------------------------------------------*/
/*! @brief  x_cap_save
 *             After the capture operation done, save the image to the specified path.
 *  @param [in] h_cap               Specified the capture operation handle.
 *  @param [in] ps_path             Specified the path to save.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The image file opening is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
INT32 x_cap_save (
                  HANDLE_T                     h_cap,      /* (I) Specified the capture operation handle. */ 
                  const CAP_LOGO_SAVE_INFO_T*  pt_save_info)    /* (I) Specified the path to save. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    if((h_cap == NULL_HANDLE) || (pt_save_info == NULL))
    {
        return CAPR_INV_ARG;
    }

	if (pt_save_info->e_device_type == CAP_DEVICE_TYPE_EXTERNAL)
    {
        x_dbg_stmt("[mw cap]%s \r\n", pt_save_info->u.ps_path);
    }
	
    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return CAPR_INTERNAL_ERR;
    }
    x_memcpy(&(pt_cap_obj->t_save_info), pt_save_info, sizeof(CAP_LOGO_SAVE_INFO_T));

    cap_logo_send_notify_msg (pt_cap_obj->h_cap,
                              CAP_LOGO_MSG_ASYNC_EXEC_TYPE,
                              CAP_EVENT_TYPE_DO_SAVING);
    return i4_ret;
}

INT32 x_cap_do_saving(HANDLE_T  h_cap)
{
    UINT8               ui1_i;
    INT32               i4_ret;
    UINT32              ui4_write_suc   = 0;
    HANDLE_T            h_file          = NULL_HANDLE;
#ifdef ANDROID
    FILE*               file;
#endif
    CAP_OBJ_T*          pt_cap_obj      = NULL;

    if(h_cap == NULL_HANDLE)
    {
        return CAPR_INV_ARG;
    }
    
    do
    {
        i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
        if (i4_ret != CAPR_OK)
        {
            x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
            break;
        }
        if(pt_cap_obj->b_cancel_exe_flag)
        {
            break;
        }
        
        x_sema_lock(pt_cap_obj->h_cap_cancel_exe_sema, X_SEMA_OPTION_WAIT);
    
        if((pt_cap_obj->t_cap_output.puac_dst_buffer == NULL) ||
           (pt_cap_obj->t_cap_output.ui4_buffer_len == 0))
        {
            x_dbg_stmt("%s(), raw data is NULL!\r\n", __func__);
            i4_ret = CAPR_INTERNAL_ERR;
            break;
        }
        if(pt_cap_obj->t_save_info.e_device_type == CAP_DEVICE_TYPE_INTERNAL)
        {
            INT64                           i8_offset;
            UINT64                          ui8_pos;
            SIZE_T                          z_size_1;
            SIZE_T                          z_size_2;
            DRV_CUST_OPERATION_INFO_T       t_op_info;            
            VID_DEC_BOOT_LOGO_GET_T         t_vid_dec_boot_logo_get;
            VID_DEC_BOOT_LOGO_SET_T         t_vid_dec_boot_logo_set;

            for(ui1_i=0; ui1_i<100; ui1_i++)
            {
                x_thread_delay(50);
                if(pt_cap_obj->b_cancel_exe_flag)
                {
                   break;
                }
            }
            if(pt_cap_obj->b_cancel_exe_flag)
            {
               break;
            }
            /*---------------Step 1: Open partition --------------------------*/
            i4_ret = x_fm_open(FM_ROOT_HANDLE,
                               "/dev/"SN_FLASH_CAPTURE_LOGO,
                               FM_WRITE_ONLY,
                               0777,
                               FALSE,
                               &h_file);

            if (i4_ret != FMR_OK)
            {
                CHAR      *ps_path = "/dev/"SN_FLASH_CAPTURE_LOGO;
                x_dbg_stmt("%s(), line %d ret %d, with path = %s !\r\n", 
                                __func__, __LINE__, i4_ret, ps_path);
                i4_ret = CAPR_IO_ERROR;
                break;
            }
            
            /*---------------Step 2: Get Partition information----------------*/
            x_memset(&t_op_info, 
                     0, 
                     sizeof(DRV_CUST_OPERATION_INFO_T));
            x_memset(&t_vid_dec_boot_logo_get, 
                     0, 
                     sizeof(VID_DEC_BOOT_LOGO_GET_T));
            z_size_1 = sizeof(VID_DEC_BOOT_LOGO_GET_T);
            t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET;
            t_op_info.u.t_vid_dec_get_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
            t_op_info.u.t_vid_dec_get_info.pv_get_info = &t_vid_dec_boot_logo_get;
            t_op_info.u.t_vid_dec_get_info.pz_size = &z_size_1;
        
            z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
            i4_ret = x_rm_get_comp(
                        DRVT_CUST_DRV,
                        DRV_CUST_COMP_ID,
                        FALSE,
                        ANY_PORT_NUM,
                        0,
                        &t_op_info,
                        &z_size_2
                        );
            if (i4_ret != RMR_OK)
            {
                x_dbg_stmt("[cap] VID_DEC_GET Fail! i4_ret=%d\r\n", i4_ret);
                i4_ret = x_fm_close(h_file);
                return CAPR_INTERNAL_ERR;
            }
        
            /*---------------Step 3: Seek to the right position---------------*/
            if(t_vid_dec_boot_logo_get.c_slot_num == 0)
            {
                i4_ret = CAPR_INTERNAL_ERR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }
            if(t_vid_dec_boot_logo_get.ui4_flash_size < 128*1024)
            {
                x_dbg_stmt("\r\nERROR: [cap] Please ask SI to alloc logo partition size! \r\n");
                i4_ret = CAPR_INTERNAL_ERR;
                break;
            }
            if(t_vid_dec_boot_logo_get.ui4_valid >= (1<<t_vid_dec_boot_logo_get.c_slot_num))
            {
                t_vid_dec_boot_logo_get.ui4_valid = 0;
            }
            i8_offset = (INT64)(pt_cap_obj->t_save_info.u.ui4_logo_id *
                    t_vid_dec_boot_logo_get.ui4_flash_size / t_vid_dec_boot_logo_get.c_slot_num);
            i4_ret = x_fm_lseek(h_file,
                                i8_offset,
                                FM_SEEK_BGN,
                                &ui8_pos);
            if((i4_ret != FMR_OK) || (i8_offset != (INT64)ui8_pos))
            {
                i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }
        
            /*---------------Step 4: Write Data-------------------------------*/
            i4_ret = x_fm_write(h_file,
                                pt_cap_obj->t_cap_output.puac_dst_buffer,
                                sizeof(UCHAR)*pt_cap_obj->t_cap_output.ui4_buffer_len,
                                &ui4_write_suc);
            if((i4_ret != FMR_OK) || (ui4_write_suc != pt_cap_obj->t_cap_output.ui4_buffer_len))
            {
                i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }

            i4_ret = x_fm_close(h_file);
            if(FMR_OK != i4_ret)
            {
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }
#if 0
        {
            i4_ret = x_fm_open(FM_ROOT_HANDLE,
                               "/mnt/usbdisk/test.bin",
                               FM_READ_WRITE | FM_OPEN_CREATE,
                               0777,
                               FALSE,
                               &h_file);

            if (i4_ret != FMR_OK)
            {
                CHAR      *ps_path = "/mnt/usbdisk/test.bin";
                x_dbg_stmt("======= %s(), line %d ret %d, with path = %s !\r\n", 
                                __func__, __LINE__, i4_ret, ps_path);
            }
            else
            {
                i4_ret = x_fm_write(h_file,
                                    pt_cap_obj->t_cap_output.puac_dst_buffer,
                                    sizeof(UCHAR)*pt_cap_obj->t_cap_output.ui4_buffer_len,
                                    &ui4_write_suc);
                if((i4_ret != FMR_OK) || (ui4_write_suc != pt_cap_obj->t_cap_output.ui4_buffer_len))
                {
                    x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                }

                i4_ret = x_fm_close(h_file);
                if(FMR_OK != i4_ret)
                {
                    x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                }
            }
        }
#endif
            /*---------------Step 5: Update Valid Tag-------------------------*/
            x_memset(&t_op_info, 
                     0, 
                     sizeof(DRV_CUST_OPERATION_INFO_T));
            x_memset(&t_vid_dec_boot_logo_set, 
                     0, 
                     sizeof(VID_DEC_BOOT_LOGO_SET_T));
            t_vid_dec_boot_logo_set.b_default = t_vid_dec_boot_logo_get.b_default;
            t_vid_dec_boot_logo_set.b_onoff = t_vid_dec_boot_logo_get.b_onoff;
            t_vid_dec_boot_logo_set.c_data_select = t_vid_dec_boot_logo_get.c_data_select;

            if(pt_cap_obj->b_cancel_exe_flag)
            {
                t_vid_dec_boot_logo_set.ui4_valid
                    = t_vid_dec_boot_logo_get.ui4_valid & ~(1<<pt_cap_obj->t_save_info.u.ui4_logo_id);
                if(pt_cap_obj->t_save_info.u.ui4_logo_id == t_vid_dec_boot_logo_get.c_data_select)
                {/*Disable current logo display*/
                    VID_DEC_BOOT_LOGO_ADDR_SET_T    t_vid_dec_boot_logo_addr_set;
                    
                    x_memset(&t_op_info, 
                             0, 
                             sizeof(DRV_CUST_OPERATION_INFO_T));
                    x_memset(&t_vid_dec_boot_logo_addr_set, 
                             0, 
                             sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T));
                    t_vid_dec_boot_logo_addr_set.c_data_type = (UINT8)VID_DEC_CODEC_MPEG_2;
                     t_vid_dec_boot_logo_addr_set.ui4_data_size = 0;/*set addr/size to 0 to disable logo*/
                    t_vid_dec_boot_logo_addr_set.ui4_addr = 0;

                    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T);
                    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET;
                    t_op_info.u.t_vid_dec_set_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR;
                    t_op_info.u.t_vid_dec_set_info.pv_set_info = &t_vid_dec_boot_logo_addr_set;
                    t_op_info.u.t_vid_dec_set_info.z_size = z_size_1;
                    /* b_store 0:not write to EEPROM, work before reboot, 1:write to EEPROM*/
                    t_op_info.u.t_vid_dec_set_info.b_store = TRUE;
                    
                    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
                    i4_ret = x_rm_set_comp(
                                DRVT_CUST_DRV,
                                DRV_CUST_COMP_ID,
                                FALSE,
                                ANY_PORT_NUM,
                                0,
                                &t_op_info,
                                z_size_2
                                );
                    if (i4_ret != RMR_OK)
                    {
                        x_dbg_stmt("[cap] VID_DEC_SET Fail! i4_ret=%d\r\n", i4_ret);
                        return CAPR_INTERNAL_ERR;
                    }
                }
            }
            else
            {
                t_vid_dec_boot_logo_set.ui4_valid
                    = t_vid_dec_boot_logo_get.ui4_valid | (1<<pt_cap_obj->t_save_info.u.ui4_logo_id);
                if(pt_cap_obj->t_save_info.u.ui4_logo_id == t_vid_dec_boot_logo_get.c_data_select)
                {/*Disable current logo display*/
                    VID_DEC_BOOT_LOGO_ADDR_SET_T    t_vid_dec_boot_logo_addr_set;
                    
                    x_memset(&t_op_info, 
                             0, 
                             sizeof(DRV_CUST_OPERATION_INFO_T));
                    x_memset(&t_vid_dec_boot_logo_addr_set, 
                             0, 
                             sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T));
                    t_vid_dec_boot_logo_addr_set.c_data_type = (UINT8)VID_DEC_CODEC_MPEG_2;
                    t_vid_dec_boot_logo_addr_set.ui4_data_size
                        = t_vid_dec_boot_logo_get.ui4_flash_size / t_vid_dec_boot_logo_get.c_slot_num;
                    t_vid_dec_boot_logo_addr_set.ui4_addr = /*t_vid_dec_boot_logo_get.ui4_flash_addr :no use
                        + */(t_vid_dec_boot_logo_get.c_data_select * t_vid_dec_boot_logo_get.ui4_flash_size
                            / t_vid_dec_boot_logo_get.c_slot_num);
                    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T);
                    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET;
                    t_op_info.u.t_vid_dec_set_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR;
                    t_op_info.u.t_vid_dec_set_info.pv_set_info = &t_vid_dec_boot_logo_addr_set;
                    t_op_info.u.t_vid_dec_set_info.z_size = z_size_1;
                    /* b_store 0:not write to EEPROM, work before reboot, 1:write to EEPROM*/
                    t_op_info.u.t_vid_dec_set_info.b_store = TRUE;
                    
                    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
                    i4_ret = x_rm_set_comp(
                                DRVT_CUST_DRV,
                                DRV_CUST_COMP_ID,
                                FALSE,
                                ANY_PORT_NUM,
                                0,
                                &t_op_info,
                                z_size_2
                                );
                    if (i4_ret != RMR_OK)
                    {
                        x_dbg_stmt("[cap] VID_DEC_SET Fail! i4_ret=%d\r\n", i4_ret);
                        return CAPR_INTERNAL_ERR;
                    }
                }
            }

            z_size_1 = sizeof(t_vid_dec_boot_logo_set);
            t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET;
            t_op_info.u.t_vid_dec_set_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
            t_op_info.u.t_vid_dec_set_info.pv_set_info = &t_vid_dec_boot_logo_set;
            t_op_info.u.t_vid_dec_set_info.z_size = z_size_1;
            t_op_info.u.t_vid_dec_set_info.b_store = TRUE;
            
            z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
            i4_ret = x_rm_set_comp(
                        DRVT_CUST_DRV,
                        DRV_CUST_COMP_ID,
                        FALSE,
                        ANY_PORT_NUM,
                        0,
                        &t_op_info,
                        z_size_2
                        );
            if (i4_ret != RMR_OK)
            {
                x_dbg_stmt("[cap] VID_DEC_SET Fail! i4_ret=%d\r\n", i4_ret);
                return CAPR_INTERNAL_ERR;
            }
        }
        else if(pt_cap_obj->t_save_info.e_device_type == CAP_DEVICE_TYPE_EXTERNAL)
        {
            x_sema_lock(pt_cap_obj->h_cap_cancel_exe_sema, X_SEMA_OPTION_WAIT);
            
            for(ui1_i=0; ui1_i<100; ui1_i++)
            {
                x_thread_delay(50);
                if(pt_cap_obj->b_cancel_exe_flag)
                {
                   break;
                }
            }
            if(pt_cap_obj->b_cancel_exe_flag)
            {
               break;
            }
#ifndef ANDROID
            i4_ret = x_fm_open(FM_ROOT_HANDLE,
                               pt_cap_obj->t_save_info.u.ps_path,
                               FM_WRITE_ONLY | FM_OPEN_CREATE,
                               0777,
                               FALSE,
                               &h_file);

            if (i4_ret != FMR_OK)
            {
                i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }
            
            i4_ret = x_fm_write(h_file,
                                pt_cap_obj->t_cap_output.puac_dst_buffer,
                                sizeof(UCHAR)*pt_cap_obj->t_cap_output.ui4_buffer_len,
                                &ui4_write_suc);
            if((i4_ret != FMR_OK) || (ui4_write_suc != pt_cap_obj->t_cap_output.ui4_buffer_len))
            {
                i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
            }
            i4_ret = x_fm_close(h_file);
#else
			file = fopen(pt_cap_obj->t_save_info.u.ps_path, "w");
			if (file <= 0)
			{
				i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
                break;
			}

			i4_ret = fwrite(pt_cap_obj->t_cap_output.puac_dst_buffer, sizeof(UCHAR), sizeof(UCHAR)*pt_cap_obj->t_cap_output.ui4_buffer_len, file);
			if (i4_ret != (INT32)pt_cap_obj->t_cap_output.ui4_buffer_len)
			{
				i4_ret = CAPR_IO_ERROR;
                x_dbg_stmt("%s(), line %d !\r\n", __func__, __LINE__);
				fclose(file);
                break;
			}
			fclose(file);
			i4_ret = CAPR_OK;
#endif
        }
    }while(0);
    
    x_sema_unlock(pt_cap_obj->h_cap_cancel_exe_sema);
    
    if((i4_ret == CAPR_OK) && !pt_cap_obj->b_cancel_exe_flag)
    {
        x_dbg_stmt("[CAP]x_cap_do_saving() successful !!\r\n");
        pt_cap_obj->pf_nfy( pt_cap_obj->h_cap,
                            pt_cap_obj->pv_nfy_tag,
                            CAP_EVENT_TYPE_SAVE_DONE,
                            (const VOID*)i4_ret);
    }
    else
    {
        x_dbg_stmt("[CAP]x_cap_do_saving() failed !!\r\n");
        pt_cap_obj->pf_nfy( pt_cap_obj->h_cap,
                            pt_cap_obj->pv_nfy_tag,
                            CAP_EVENT_TYPE_SAVE_ERROR,
                            (const VOID*)i4_ret);
    }
#if 0
    {
        CAP_LOGO_SELECT_INFO_T      t_select_info;

        t_select_info.e_device_type = CAP_DEVICE_TYPE_INTERNAL;
        t_select_info.u.ui4_logo_id = pt_cap_obj->t_save_info.u.ui4_logo_id;
        
        x_cap_select_as_bootlogo (&t_select_info);
    }
#endif
    return i4_ret;
}

/*------------------------------------------------------------------*/
/*! @brief   x_cap_close
 *               This function close the capture handle and free the resource.
 *  @param [in] h_cap               Specified the already opened image handle.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_INV_ARG            FAIL. The pathe is NULL.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
INT32 x_cap_close (HANDLE_T          h_cap) /* (O) retrieve the height of the image. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;
    }

    if(pt_cap_obj->e_source == CAP_SRC_TYPE_MM_IMAGE)
    {
        if(pt_cap_obj->e_res_type == CAP_OUT_RES_TYPE_USER)
        {
            i4_ret = x_gl_surface_unlock(pt_cap_obj->h_working_surf,
                                         NULL);
        }
        else
        {
            i4_ret = x_gl_surface_unlock(pt_cap_obj->h_src_surf,
                                         NULL);
        }
    }
#ifdef ANDROID
	if (pt_cap_obj->e_source == CAP_SRC_TYPE_MM_IMAGE_ANDROID)
	{
		if (pt_cap_obj->t_cap_input.puac_src_buffer)
		{
			if (shmdt(pt_cap_obj->t_cap_input.puac_src_buffer) < 0)
			{
				x_dbg_stmt("[CAP]{%s %d} shmdt error.\n", __FUNCTION__, __LINE__);
				return CAPR_INTERNAL_ERR;
			}
		}
	}
#endif
    if(pt_cap_obj->t_cap_input.puac_dst_buffer != NULL)
    {
        x_mem_free(pt_cap_obj->t_cap_input.puac_dst_buffer);
        x_memset(&(pt_cap_obj->t_cap_input), 0, sizeof(CAP_INPUT_T));
        x_memset(&(pt_cap_obj->t_cap_output), 0, sizeof(CAP_OUTPUT_T));
    }
    
    x_sema_delete(pt_cap_obj->h_cap_cancel_exe_sema);
    
    i4_ret = x_handle_free(h_cap);
    if(i4_ret != HR_OK)
    {
        return CAPR_INTERNAL_ERR;
    }
    
    return CAPR_OK;
}

                                                     
/*------------------------------------------------------------------*/
/*! @brief      x_cap_sync_stop_capture
 *                  This function sync stops the capture operation.
 *  @param [in] h_cap       Specified the capture handle that want to stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
INT32 x_cap_sync_stop_capture (HANDLE_T     h_cap)            /* (I) specify the capture handle that want to stop. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;
    }
    pt_cap_obj->b_cancel_exe_flag = TRUE;
    
    x_img_cap_stop();

    return CAPR_OK;
}

                        
/*------------------------------------------------------------------*/
/*! @brief      x_cap_sync_stop_save
 *                  This function can stop these types IMG_STOP_OP_TYPE_T of image operation.
 *  @param [in] h_cap               Specified the image handle that want to stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
INT32 x_cap_sync_stop_save (HANDLE_T     h_cap)            /* (I) Specify the capture handle that want to stop. */ 
{
    INT32               i4_ret;
    CAP_OBJ_T*          pt_cap_obj = NULL;

    i4_ret = _cap_get_obj_by_handle(h_cap, &pt_cap_obj);
    if (i4_ret != CAPR_OK)
    {
        x_dbg_stmt("%s(), pt_cap_obj is NULL!\r\n", __func__);
        return i4_ret;
    }
    pt_cap_obj->b_cancel_exe_flag = TRUE;
    
    x_sema_lock(pt_cap_obj->h_cap_cancel_exe_sema, X_SEMA_OPTION_WAIT);
    
    return CAPR_OK;
}


/* This API will start decoding the primary image or thumbnail to the target surface h_dst_surf, and it will not return until decoding image is complete. */          
/*------------------------------------------------------------------*/
/*! @brief   x_cap_async_stop_capture
 *               This API will async stop capture operation.
 *  @param [in] h_cap                   Specified the capture handle that want to async stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_async_stop_capture ( HANDLE_T             h_cap)         /* (I) specify the capture handle that want to async stop. */ 
{
    return CAPR_OK;
}

                        
/*------------------------------------------------------------------*/
/*! @brief   x_cap_async_stop_save
 *               This API will async stop saving operation.
 *  @param [in] h_cap           Specified the image handle that want to decode.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_async_stop_save ( HANDLE_T             h_cap)         /* (I) Specify the image handle that want to aysnc stop saving. */ 
{
    return CAPR_OK;
}
                            
                            
/*------------------------------------------------------------------*/
/*! @brief   x_cap_set_boot_logo
 *               This API will select specified picture captured as bootup logo.
 *  @param [in] ps_path                 Specified the image path name that want to set as bootup logo.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_select_as_bootlogo ( const CAP_LOGO_SELECT_INFO_T* pt_select_info)         /* (I) Specified the image path name that want to set as bootup logo. */ 
{/*None: Default: 0: 1:*/
    INT32                           i4_ret;
    SIZE_T                          z_size_1;
    SIZE_T                          z_size_2;
    DRV_CUST_OPERATION_INFO_T       t_op_info;
    
    VID_DEC_BOOT_LOGO_GET_T         t_vid_dec_boot_logo_get;
    /*VID_DEC_BOOT_LOGO_ADDR_GET_T    t_vid_dec_boot_logo_addr_get;*/
    
    VID_DEC_BOOT_LOGO_SET_T         t_vid_dec_boot_logo_set;
    VID_DEC_BOOT_LOGO_ADDR_SET_T    t_vid_dec_boot_logo_addr_set;

    /*-------------Step 1: Get current logo info--------------------*/
    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    x_memset(&t_vid_dec_boot_logo_get, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_GET_T));
    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_GET_T);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET;
    t_op_info.u.t_vid_dec_get_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
    t_op_info.u.t_vid_dec_get_info.pv_get_info = &t_vid_dec_boot_logo_get;
    t_op_info.u.t_vid_dec_get_info.pz_size = &z_size_1;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_get_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                &z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_GET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }


    /*-------------Step 2: Set current logo info--------------------*/    
    if(t_vid_dec_boot_logo_get.c_slot_num == 0)
    {
        return CAPR_INTERNAL_ERR;
    }

    if(((UINT8)pt_select_info->u.ui4_logo_id >= t_vid_dec_boot_logo_get.c_slot_num) 
        && ((UINT8)pt_select_info->u.ui4_logo_id != 0xFF)
        && ((UINT8)pt_select_info->u.ui4_logo_id != 0xFFFF))/*Clear all: reset valid bit*/
    {
        x_dbg_stmt("[cap] Index exceed MAX! \r\n");
        return CAPR_INV_ARG;
    }
    if(t_vid_dec_boot_logo_get.ui4_valid >= (1<<t_vid_dec_boot_logo_get.c_slot_num))
    {
        t_vid_dec_boot_logo_get.ui4_valid = 0;
    }
    
    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    x_memset(&t_vid_dec_boot_logo_set, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_SET_T));
    if(pt_select_info->e_device_type == CAP_DEVICE_TYPE_DEFAULT)
    {/*----------Use Default Logo--------------*/
        t_vid_dec_boot_logo_set.b_default = TRUE;
    }
    else
    {
        t_vid_dec_boot_logo_set.b_default = FALSE;
    }
    if(pt_select_info->u.ui4_logo_id == 0xFF)
    {
        t_vid_dec_boot_logo_set.b_onoff = FALSE;
    }
    else if(pt_select_info->u.ui4_logo_id == 0xFFFF)/*clear all logo*/
    {
        t_vid_dec_boot_logo_set.b_onoff     = FALSE;
        t_vid_dec_boot_logo_get.ui4_valid   = 0;
    }
    else
    {
        t_vid_dec_boot_logo_set.b_onoff = TRUE;
    }
    t_vid_dec_boot_logo_set.c_data_select = (UINT8)pt_select_info->u.ui4_logo_id;    
    t_vid_dec_boot_logo_set.ui4_valid = t_vid_dec_boot_logo_get.ui4_valid;

    z_size_1 = sizeof(t_vid_dec_boot_logo_set);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET;
    t_op_info.u.t_vid_dec_set_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
    t_op_info.u.t_vid_dec_set_info.pv_set_info = &t_vid_dec_boot_logo_set;
    t_op_info.u.t_vid_dec_set_info.z_size = z_size_1;
    /* b_store 0:not write to EEPROM, work before reboot, 1:write to EEPROM*/
    t_op_info.u.t_vid_dec_set_info.b_store = TRUE;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_set_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_SET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }
    
    /*-------------Step 3: --------------------*/
    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    x_memset(&t_vid_dec_boot_logo_addr_set, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T));
    t_vid_dec_boot_logo_addr_set.c_data_type = (UINT8)VID_DEC_CODEC_MPEG_2;
    t_vid_dec_boot_logo_addr_set.ui4_data_size
        = t_vid_dec_boot_logo_get.ui4_flash_size / t_vid_dec_boot_logo_get.c_slot_num;
    t_vid_dec_boot_logo_addr_set.ui4_addr = /*t_vid_dec_boot_logo_get.ui4_flash_addr :no use
        + */(pt_select_info->u.ui4_logo_id * t_vid_dec_boot_logo_get.ui4_flash_size
            / t_vid_dec_boot_logo_get.c_slot_num);

    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_ADDR_SET_T);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET;
    t_op_info.u.t_vid_dec_set_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR;
    t_op_info.u.t_vid_dec_set_info.pv_set_info = &t_vid_dec_boot_logo_addr_set;
    t_op_info.u.t_vid_dec_set_info.z_size = z_size_1;
    /* b_store 0:not write to EEPROM, work before reboot, 1:write to EEPROM*/
    t_op_info.u.t_vid_dec_set_info.b_store = TRUE;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_set_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_SET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }
    
        x_dbg_stmt("[cap] Select logo id=%d successful!!!\r\n", pt_select_info->u.ui4_logo_id);
#if 0
    /*-------------Step 4: Check result--------------------*/
    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    x_memset(&t_vid_dec_boot_logo_addr_get, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_ADDR_GET_T));
    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_ADDR_GET_T);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET;
    t_op_info.u.t_vid_dec_get_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR;
    t_op_info.u.t_vid_dec_get_info.pv_get_info = &t_vid_dec_boot_logo_addr_get;
    t_op_info.u.t_vid_dec_get_info.pz_size = &z_size_1;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_get_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                &z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_GET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }

    ASSERT(t_vid_dec_boot_logo_addr_get.c_data_type == t_vid_dec_boot_logo_addr_set.c_data_type);
    ASSERT(t_vid_dec_boot_logo_addr_get.ui4_data_size == t_vid_dec_boot_logo_addr_set.ui4_data_size);
    ASSERT(t_vid_dec_boot_logo_addr_get.ui4_addr == t_vid_dec_boot_logo_addr_set.ui4_addr);

    x_memset(&t_vid_dec_boot_logo_get, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_GET_T));    
    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_GET_T);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET;
    t_op_info.u.t_vid_dec_get_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
    t_op_info.u.t_vid_dec_get_info.pv_get_info = &t_vid_dec_boot_logo_get;
    t_op_info.u.t_vid_dec_get_info.pz_size = &z_size_1;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_get_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                &z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_GET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }

    ASSERT(t_vid_dec_boot_logo_get.b_onoff == t_vid_dec_boot_logo_set.b_onoff);
    ASSERT((t_vid_dec_boot_logo_get.c_data_select == t_vid_dec_boot_logo_set.c_data_select)
            && (t_vid_dec_boot_logo_set.c_data_select != 0xFF));//maybe 0x1f == 0xff
    ASSERT(t_vid_dec_boot_logo_get.ui4_valid == t_vid_dec_boot_logo_set.ui4_valid);
#endif
    return CAPR_OK;
}

/*------------------------------------------------------------------*/
/*! @brief   x_cap_query_capabilty
 *               This API will select specified picture captured as bootup logo.
 *  @param [in] pt_cap_info                 Specified the address to store the logo info.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
INT32 x_cap_query_capabilty(CAP_CAPABILITY_INFO_T*  pt_cap_info)
{
    INT32                           i4_ret;
    SIZE_T                          z_size_1;
    SIZE_T                          z_size_2;
    DRV_CUST_OPERATION_INFO_T       t_op_info;    
    VID_DEC_BOOT_LOGO_GET_T         t_vid_dec_boot_logo_get;

    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    x_memset(&t_vid_dec_boot_logo_get, 
             0, 
             sizeof(VID_DEC_BOOT_LOGO_GET_T));
    z_size_1 = sizeof(VID_DEC_BOOT_LOGO_GET_T);
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET;
    t_op_info.u.t_vid_dec_get_info.e_vid_dec_type = DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL;
    t_op_info.u.t_vid_dec_get_info.pv_get_info = &t_vid_dec_boot_logo_get;
    t_op_info.u.t_vid_dec_get_info.pz_size = &z_size_1;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_get_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                &z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[cap] VID_DEC_GET Fail! i4_ret=%d\r\n", i4_ret);
        return CAPR_INTERNAL_ERR;
    }

    pt_cap_info->b_default = t_vid_dec_boot_logo_get.b_default;
    pt_cap_info->b_default_exist = t_vid_dec_boot_logo_get.b_default_exist;
    pt_cap_info->ui1_cur_logo_index = t_vid_dec_boot_logo_get.c_data_select;
    pt_cap_info->ui1_nums_logo_slots = t_vid_dec_boot_logo_get.c_slot_num;
    if(t_vid_dec_boot_logo_get.ui4_valid >= (1<<t_vid_dec_boot_logo_get.c_slot_num))
    {
        t_vid_dec_boot_logo_get.ui4_valid = 0;
    }
    pt_cap_info->ui2_logo_valid_tag = (UINT16)t_vid_dec_boot_logo_get.ui4_valid;


    x_dbg_stmt("b_default=%d, b_default_exist=%d, ui1_cur_logo_index=%d, ui1_nums_logo_slots=%d, ui2_logo_valid_tag=0x%x",
        pt_cap_info->b_default,
        pt_cap_info->b_default_exist,
        pt_cap_info->ui1_cur_logo_index,
        pt_cap_info->ui1_nums_logo_slots,
        pt_cap_info->ui2_logo_valid_tag);

    return CAPR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_cap_init
 *
 * Description: This function is an cap engine selector's API. This function 
 *              is called by system initial procedure when system boot-up. It will
 *              create a thread and a message queue for asynchronous loading image.
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: CAPR_OK                 The API is successful. 
 *          CAPR_INITIAL_ERR        The image library initialize failed.
 *          CAPR_CLI_ERROR          Initial CLI function for compression library is failed.
 *          CAPR_AUTOFREE_ERROR     Adding auto free function to AEE is failed.
 ----------------------------------------------------------------------------*/                            
INT32 x_cap_init (void)
{
    INT32 i4_ret;
    
    i4_ret = cap_init();
    
    return i4_ret;
}


