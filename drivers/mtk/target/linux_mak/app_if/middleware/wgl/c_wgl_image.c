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
 * $RCSfile: c_wgl_image.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 5c7f9b779bd3e3428d24b3fb6f494fb5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee.h"
#include "wgl/_wgl_image.h"
#include "wgl/x_wgl_image.h"
#include "graphic/c_gl.h"
#include "dbg/c_dbg.h"
#include "wgl/wgl.h"
#include "wgl/wgl_sets_cli.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_image_data_clone
 * Description
 *          please call c_gl_obj_free() to free ph_surface when you don't need it.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 c_wgl_img_tpl_image_data_clone(
    WGL_HIMG_TPL_T                          h_img_tpl,
    WGL_IMG_ST_T*                           pe_type,
    GL_HSURFACE_T*                          ph_surface)
{
    INT32                           i4_ret;
    WGL_IMG_TPL_CLONE_FCT_TABLE_T   t_clone_fct_tbl = {c_gl_surface_create,
                                                       c_gl_surface_blt,
                                                       c_gl_obj_free};

    DBG_API(("{WGL(IMG)} %s: h_img_tpl = 0x%.8x \tpe_type = 0x%.8x \tph_surface = 0x%.8x\n", 
            __FUNCTION__, 
            h_img_tpl, 
            pe_type,
            ph_surface));

    i4_ret = wgl_img_tpl_image_data_clone(&t_clone_fct_tbl, h_img_tpl, pe_type, ph_surface);
    WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to clone image data from template image. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_create
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
extern INT32 c_wgl_img_tpl_create(
    WGL_IMG_ST_T                    e_segment_type,
    const WGL_IMG_T*                pt_image,
    WGL_HIMG_TPL_T*                 ph_img_tpl)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \tpt_image = 0x%.8x \tph_img_tpl = 0x%.8x\n", 
            __FUNCTION__, 
            e_segment_type, 
            pt_image,
            ph_img_tpl));

    /*get a free aux*/
    i4_ret = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);
    if(AEER_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: No free handle for this thread. (AEER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_OUT_OF_HANDLES;
    }

    /*create image template*/
    i4_ret = x_wgl_img_tpl_create(e_segment_type, pt_image, ph_img_tpl);
        
    if (i4_ret == WGLR_OK)
    {
        handle_link_to_aux(h_aux, *ph_img_tpl);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

extern INT32 c_wgl_img_tpl_create_ex(
    WGL_IMG_ST_T                    e_segment_type,
    const WGL_IMG_T*                pt_image,
    WGL_HIMG_TPL_T*                 ph_img_tpl)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \tpt_image = 0x%.8x \tph_img_tpl = 0x%.8x\n", 
            __FUNCTION__, 
            e_segment_type, 
            pt_image,
            ph_img_tpl));

    /*get a free aux*/
    i4_ret = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);
    if(AEER_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: No free handle for this thread. (AEER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_OUT_OF_HANDLES;
    }

    /*create image template*/
    i4_ret = x_wgl_img_tpl_create_ex(e_segment_type, pt_image, ph_img_tpl);
        
    if (i4_ret == WGLR_OK)
    {
        handle_link_to_aux(h_aux, *ph_img_tpl);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}


/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_create_by_surf
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
extern INT32 c_wgl_img_tpl_create_by_surf(
    WGL_IMG_ST_T                          e_segment_type,
    GL_HSURFACE_T                         h_src_surf,
    WGL_HIMG_TPL_T*                       ph_img_tpl)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \th_src_surf = 0x%.8x \tph_img_tpl = 0x%.8x\n", 
            __FUNCTION__, 
            e_segment_type, 
            h_src_surf,
            ph_img_tpl));

    /*get a free aux*/
    i4_ret = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);
    if(AEER_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: No free handle for this thread. (AEER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_OUT_OF_HANDLES;
    }

    /*create image template*/
    i4_ret = x_wgl_img_tpl_create_by_surf(e_segment_type, h_src_surf, ph_img_tpl);
        
    if (i4_ret == WGLR_OK)
    {
        handle_link_to_aux(h_aux, *ph_img_tpl);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}


/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_destroy
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 c_wgl_img_tpl_destroy(
    WGL_HIMG_TPL_T                  h_img_tpl)
{
    return x_wgl_img_tpl_destroy(h_img_tpl);
}


INT32 c_wgl_img_tpl_paint(
    WGL_HIMG_TPL_T             h_img_tpl,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method)
{
    return x_wgl_img_tpl_paint(
        h_img_tpl,
        pt_output_rect,
        pt_invalid_rect,
        h_graphics,
        pt_cvsst,
        e_paint_method);
}

/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_update_by_surf
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 c_wgl_img_tpl_update_by_surf(
    WGL_HIMG_TPL_T                 h_dest_img_tpl,
    GL_HSURFACE_T                  h_src_surf)
{
    return x_wgl_img_tpl_update_by_surf(h_dest_img_tpl, h_src_surf);
}

/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_get_size
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 c_wgl_img_tpl_get_size(
    WGL_HIMG_TPL_T              h_img_tpl,
    UINT32*                     pui4_width,
    UINT32*                     pui4_height)
{
    return x_wgl_img_tpl_get_size(h_img_tpl, pui4_width, pui4_height);
}



/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_create_surface
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 c_wgl_img_tpl_create_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs)
{
    return x_wgl_img_tpl_create_surface(ph_img_tpl, ui2_img_nbs);
}

/*******************************************************************************
 * Name
 *          c_wgl_img_tpl_destroy_surface
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 c_wgl_img_tpl_destroy_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs)
{
    return x_wgl_img_tpl_destroy_surface(ph_img_tpl, ui2_img_nbs);
}
INT32 c_wgl_img_cache_flush(VOID)
{
    return x_wgl_img_cache_flush();
}

INT32 c_wgl_set_img_decode_function(WGL_IMG_DECODE_FUNCTION_TABLE_T* pt_dec_table)
{
    return  x_wgl_set_img_decode_function(pt_dec_table);
}

INT32 c_wgl_set_invert_height(WGL_HIMG_TPL_T                          h_img_tpl,
                                     UINT16                                  ui2_invert_lines)
{
    return x_wgl_set_invert_height(h_img_tpl, ui2_invert_lines);
}

INT32 c_wgl_set_image_decode_sw_force(BOOL fg_sw)
{
	return x_wgl_set_image_decode_sw_force(fg_sw);
}


