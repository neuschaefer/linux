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
 * Description: 
 *         This file contains some internal APIs used by the Graphics Library.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "graphic/c_gl.h"
#include "graphic/x_gl.h"
#include "aee/aee.h"
#include "handle/handle.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
INT32 c_gl_get_gpu_capability(
    GL_GPU_CAPS_T       *pt_caps)
{
    return (x_gl_get_gpu_capability(pt_caps));
}


INT32 c_gl_obj_free(GL_HOBJ_T h_obj)
{
    return (x_gl_obj_free(h_obj));
}

INT32 c_gl_screen_open(
    const CHAR*         ps_name,
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_screen_open(ps_name, pf_screen_nfy, pv_nfy_tag, ph_screen);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_screen);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_screen_get_panel_size(
    GL_HSCREEN_T        h_screen,
    UINT32              *pui4_w,
    UINT32              *pui4_h)
{
/*    *pui4_w = 1280;
    *pui4_h = 720; */
    
    return x_gl_screen_get_panel_size(h_screen, pui4_w, pui4_h);
}

INT32 c_gl_screen_get_status(
    GL_HSCREEN_T        h_screen, 
    GL_SCREEN_STATUS_T* pt_scr_status)
{
    return (x_gl_screen_get_status(h_screen, pt_scr_status));
}

/* plane component API */
INT32 c_gl_plane_config(
    HANDLE_T            h_plane,
    GL_PLANE_CFG_T*     pt_pln_cfg)
{
    return (x_gl_plane_config(h_plane, pt_pln_cfg));
}

INT32 c_gl_plane_num(
    UINT16*             pui2_num)
{
    return (x_gl_plane_num(pui2_num));
}

INT32 c_gl_plane_enum(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx, 
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
    return (x_gl_plane_enum(h_screen, ui2_idx, pt_pln_caps));
}

INT32 c_gl_plane_open(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx,
    GL_HPLANE_T*        ph_plane)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_plane_open(h_screen, ui2_idx, ph_plane);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_plane);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_plane_link(
    GL_HPLANE_T         h_plane,
    GL_HSCREEN_T        h_screen,
    BOOL                b_forced)
{
    return (x_gl_plane_link(h_plane, h_screen, b_forced));
}

INT32 c_gl_plane_reorder(
    GL_HPLANE_T         h_plane, 
    UINT32              e_reorder_type, 
    INT32               i4_param)
{
    return (x_gl_plane_reorder(h_plane, e_reorder_type, i4_param));
}

INT32 c_gl_plane_enable(
    GL_HPLANE_T         h_plane,
    BOOL                b_enable)
{
    return (x_gl_plane_enable(h_plane, b_enable));
}

INT32 c_gl_plane_clear(GL_HPLANE_T         h_plane)
{
    return x_gl_plane_clear(h_plane);
}
INT32 c_gl_plane_change_resolution(
        GL_HPLANE_T  h_plane,
        UINT32      ui4_width,
        UINT32      ui4_height)
{
    return x_gl_plane_change_resolution(h_plane,ui4_width,ui4_height);
}
INT32 c_gl_plane_change_resolution_cm(
        GL_HPLANE_T    h_plane,
        UINT32         ui4_width,
        UINT32         ui4_height,
        GL_COLORMODE_T e_cm)
{
    return  x_gl_plane_change_resolution_cm(
                                h_plane,
                                ui4_width,
                                ui4_height,
                                e_cm);
}

INT32 c_gl_plane_flip(
    GL_HPLANE_T         h_plane,
    GL_HVIEW_T          h_view,
    BOOL                b_forced)
{
    return (x_gl_plane_flip(h_plane, h_view, b_forced));
}

INT32 c_gl_plane_get_capability(
    GL_HPLANE_T         h_plane, 
    GL_HSCREEN_T        h_screen,
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
    return (x_gl_plane_get_capability(h_plane, h_screen, pt_pln_caps));
}

INT32 c_gl_plane_get_linked_screen(
    GL_HPLANE_T         h_plane, 
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_plane_get_linked_screen(
            h_plane, 
            pf_screen_nfy,
            pv_nfy_tag,
            ph_screen);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_screen);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_plane_get_order(
    GL_HPLANE_T         h_plane, 
    INT32               *pi4_order)
{
    return x_gl_plane_get_order(h_plane, pi4_order);
}

INT32 c_gl_plane_get_status(
    GL_HPLANE_T         h_plane, 
    GL_PLANE_STATUS_T*  pt_pln_sts)
{
    return (x_gl_plane_get_status(h_plane, pt_pln_sts));
}

INT32 c_gl_plane_set_alpha(
    GL_HPLANE_T         h_plane,
    UINT8               ui1_alpha)
{
    return (x_gl_plane_set_alpha(h_plane, ui1_alpha));
}

INT32 c_gl_plane_enable_colorkey(
	GL_HPLANE_T         h_plane,
    BOOL                b_new_enable,
    BOOL*               old_enable_fg)
{
    return x_gl_plane_enable_colorkey(h_plane, b_new_enable, old_enable_fg);
}


#ifdef MW_PHOTO_SHOW_ON_VDP
INT32 c_gl_plane_get_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf)
{
    return x_gl_plane_get_resource( h_plane, h_on_screen_surf);
}
INT32 c_gl_plane_release_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf)
{
    return x_gl_plane_release_resource( h_plane, h_on_screen_surf);
}

INT32 c_gl_plane_set_vdp_cropping(GL_HPLANE_T h_plane, 
                                          BOOL        b_3d_mode,
                                          UINT32      ui4_bottom,
                                          UINT32      ui4_top,
                                          UINT32      ui4_left,
                                          UINT32      ui4_right)
{
    return x_gl_plane_set_vdp_cropping(h_plane,b_3d_mode,ui4_bottom,ui4_top,ui4_left,ui4_right);
}

INT32 c_gl_surface_vdp_strech_blend(
    GL_HSURFACE_T       h_surf_dst, 
    GL_HSURFACE_T       h_surf_src_pic, 
    GL_HSURFACE_T       h_surf_src_black,
    UINT8               ui1_alpha)
{
	return x_gl_surface_vdp_strech_blend(h_surf_dst,h_surf_src_pic,h_surf_src_black,ui1_alpha);
}


#endif


INT32 c_gl_view_create(
    UINT32                  ui4_w,
    UINT32                  ui4_h,
    BOOL                    b_auto_scale,
    GL_HVIEW_T*             ph_view)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_view_create(ui4_w, ui4_h, b_auto_scale, ph_view);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_view);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_view_create_viewport(
    GL_HVIEW_T              h_view, 
    GL_HSURFACE_T           h_surface, 
    const GL_DISPLAY_OPT_T* pt_disp_opt,
    UINT32*                 pui4_id)
{
    return (x_gl_view_create_viewport(h_view, h_surface, pt_disp_opt, pui4_id));
}

INT32 c_gl_view_delete_viewport(
    GL_HVIEW_T      h_view,
    UINT32          ui4_id)
{
    return (x_gl_view_delete_viewport(h_view, ui4_id));
}

INT32 c_gl_view_reconfig_viewport(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    UINT32          ui4_viewport_id,
    INT32           i4_view_x,
    INT32           i4_view_y,
    UINT32          ui4_width,
    UINT32          ui4_height,
    UINT32          ui4_surf_offset_x,
    UINT32          ui4_surf_offset_y,
    GL_FIX16_T      fx_shrink_x,
    GL_FIX16_T      fx_shrink_y)
{
    return x_gl_view_reconfig_viewport(
        h_plane,
        h_view,
        ui4_viewport_id,
        i4_view_x,
        i4_view_y,
        ui4_width,
        ui4_height,
        ui4_surf_offset_x,
        ui4_surf_offset_y,
        fx_shrink_x,
        fx_shrink_y);
}

INT32 c_gl_create_graphics(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_translation,
    const GL_RECT_T*    pt_clip,
    GL_HGRAPHICS_T*     ph_graphics)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_create_graphics(h_surf, pt_translation, pt_clip, ph_graphics);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_graphics);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_translate(
    GL_HGRAPHICS_T      h_graphics, 
    INT32               i4_trans_x, 
    INT32               i4_trans_y)
{
    return (x_gl_translate(h_graphics, i4_trans_x, i4_trans_y));
}

INT32 c_gl_set_clip(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_clip)
{
    return (x_gl_set_clip(h_graphics, pt_clip));
}
        
INT32 c_gl_set_color(
    GL_HGRAPHICS_T      h_graphics,
    const GL_COLOR_T*   pt_color)
{
    return (x_gl_set_color(h_graphics, pt_color));
}

INT32 c_gl_set_pen_width(
    GL_HGRAPHICS_T      h_graphics,
    UINT16              ui2_width)
{
    return (x_gl_set_pen_width(h_graphics, ui2_width));
}

INT32 c_gl_draw_line(
    GL_HGRAPHICS_T      h_graphics,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end)
{
    return (x_gl_draw_line(h_graphics, pt_start, pt_end));
}

INT32 c_gl_draw_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (x_gl_draw_rect(h_graphics, pt_bound));
}
    
INT32 c_gl_fill_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (x_gl_fill_rect(h_graphics, pt_bound));
}
    
INT32 c_gl_draw_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc)
{
    return (x_gl_draw_round_rect(h_graphics, pt_bound, pt_arc));
}
    
INT32 c_gl_fill_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc)
{
    return (x_gl_fill_round_rect(h_graphics, pt_bound, pt_arc));
}

INT32 c_gl_draw_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[])
{
    return (x_gl_draw_polygon(h_graphics, ui4_count, at_vertex));
}
    
INT32 c_gl_fill_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[])
{
    return (x_gl_fill_polygon(h_graphics, ui4_count, at_vertex));
}

INT32 c_gl_draw_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (x_gl_draw_ellipse(h_graphics, pt_bound));
}
    
INT32 c_gl_fill_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (x_gl_fill_ellipse(h_graphics, pt_bound));
}

INT32 c_gl_draw_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (x_gl_draw_arc(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end));
}

INT32 c_gl_fill_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (x_gl_fill_arc(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end));
}

INT32 c_gl_draw_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (x_gl_draw_pie(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end));
}

INT32 c_gl_fill_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (x_gl_fill_pie(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end));
}

INT32 c_gl_bitblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned)
{
    return (x_gl_bitblt(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned));
}

INT32 c_gl_patblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned)
{
    return (x_gl_patblt(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_alpha_assigned));
}

INT32 c_gl_transp_blt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max)
{
    return (x_gl_transp_blt(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        b_off_mask,
        pt_color_min,
        pt_color_max));
}

INT32 c_gl_transp_patblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max)
{
    return (x_gl_transp_patblt(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_alpha_assigned,
        b_off_mask,
        pt_color_min,
        pt_color_max));
}

INT32 c_gl_stretch_blt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h)
{
    return (x_gl_stretch_blt(
        h_graphics,
        h_surf, 
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h));
}

INT32 c_gl_blend(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha)
{
    return (x_gl_blend(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha));
}

INT32 c_gl_compose(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule)
{
    return (x_gl_compose(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_pre_alpha,
        e_rule));
}

INT32 c_gl_pat_compose(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule)
{
    return (x_gl_pat_compose(
        h_graphics, 
        h_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w, 
        ui4_dst_h, 
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui1_pre_alpha, 
        e_rule));
}

INT32 c_gl_draw_ustring_ex(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x)
{
    return (x_gl_draw_ustring_ex(
        h_graphics,
        i4_x,
        i4_y,
        i4_bg_y,
        w2s_str,
        ui2_strlen,
        w2s_skip,
        ui2_skiplen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_edge_clr,
        b_fixed,
        ui2_ch_w,
        ui2_ch_w,
        ui2_bg_h,
        i4_bg_adj_x));
}

INT32 c_gl_surface_create(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_surface_create(pt_surf_desc, ph_surf);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_surf);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}
#ifdef MW_PHOTO_SHOW_ON_VDP

INT32 c_gl_surface_create_from_vdp(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_surface_create_from_vdp(pt_surf_desc, ph_surf);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_surf);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}
#endif
INT32 c_gl_surface_create_from_fbm(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_surface_create_from_fbm(pt_surf_desc, ph_surf);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_surf);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_surface_get_bitmap_max_leng(
    GL_HSURFACE_T       h_surf,
    UINT32*             pui4_max_leng)
{
    /*return x_gl_surface_get_bitmap_max_leng(h_surf, pui4_max_leng);*/
    return 0;
}

INT32 c_gl_surface_lock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect, 
    UINT32              ui4_flag, 
    GL_LOCK_INFO_T*     pt_lock_info)
{
    return (x_gl_surface_lock(h_surf, pt_rect, ui4_flag, pt_lock_info));
}

INT32 c_gl_surface_unlock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect)
{
    return (x_gl_surface_unlock(h_surf, pt_rect));
}

INT32 c_gl_surface_get_info(
    GL_HSURFACE_T       h_surf, 
    GL_SURF_INFO_T*     pt_surf_info)
{
    return (x_gl_surface_get_info(h_surf, pt_surf_info));
}

INT32 c_gl_surface_blt_ex(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    const GL_BLT_EX_T*  pt_blt_ex,
    UINT8               ui1_blt_ex_opt,
    UINT32              ui4_flag)
{
    return (x_gl_surface_blt_ex(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        i4_src_x, 
        i4_src_y, 
        ui4_width, 
        ui4_height, 
        pt_blt_ex, 
        ui1_blt_ex_opt, 
        ui4_flag));
}
    
INT32 c_gl_surface_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    return (x_gl_surface_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        i4_src_x, 
        i4_src_y, 
        ui4_width, 
        ui4_height, 
        ui1_alpha_assigned, 
        ui4_flag));
}

INT32 c_gl_surface_draw_line(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_line(
        h_surf,
        pt_start,
        pt_end,
        pt_color,
        ui2_line_width,
        ui4_flag));
}

INT32 c_gl_surface_draw_rect(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_rect(
        h_surf,
        pt_rect,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag));
}

INT32 c_gl_surface_draw_round_rect(
    GL_HSURFACE_T           h_surf,
    const GL_RECT_T*        pt_rect,
    const GL_ARC_DESC_T*    pt_arc,
    const GL_COLOR_T*       pt_color,
    UINT16                  ui2_line_width,
    GL_STYLE_T              t_style,
    UINT32                  ui4_flag)
{
    return (x_gl_surface_draw_round_rect(
        h_surf,
        pt_rect,
        pt_arc,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag));
}

INT32 c_gl_surface_draw_ellipse(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_ellipse(
        h_surf, 
        pt_rect, 
        pt_color, 
        ui2_line_width,
        t_style, 
        ui4_flag));
}

INT32 c_gl_surface_draw_polygon(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_polygon(
        h_surf, 
        ui4_count, 
        at_vertex, 
        pt_color, 
        ui2_line_width,
        t_style, 
        ui4_flag));
}

INT32 c_gl_surface_draw_arc(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_arc(
        h_surf,
        pt_rect,
        i4_deg_start, 
        i4_deg_end,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag));
}

INT32 c_gl_surface_draw_pie(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    return (x_gl_surface_draw_pie(
        h_surf,
        pt_rect,
        i4_deg_start, 
        i4_deg_end,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag));
}

INT32 c_gl_surface_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    return (x_gl_surface_patblt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w, 
        ui4_dst_h, 
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui1_alpha_assigned, 
        ui4_flag));
}

INT32 c_gl_surface_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag)
{
    return (x_gl_surface_stretch_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w, 
        ui4_dst_h, 
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui4_flag));
}

INT32 c_gl_surface_adv_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag)
{
    return (x_gl_surface_adv_stretch_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w, 
        ui4_dst_h, 
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui4_flag));
}

INT32 c_gl_surface_transp_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    return (x_gl_surface_transp_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        i4_src_x, 
        i4_src_y, 
        ui4_width, 
        ui4_height, 
        ui1_alpha_assigned, 
        b_off_mask, 
        pt_color_min,
        pt_color_max,
        ui4_flag));
}

INT32 c_gl_surface_rotate_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_rot_op,
    UINT32              ui4_flag)
{
    return (x_gl_surface_rotate_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        ui4_rot_op,
        ui4_flag));
}

INT32 c_gl_surface_transp_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    return (x_gl_surface_transp_patblt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui1_alpha_assigned, 
        b_off_mask, 
        pt_color_min,
        pt_color_max,
        ui4_flag));
}

INT32 c_gl_surface_blend(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha,
    UINT32              ui4_flag)
{
    return (x_gl_surface_blend(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        i4_src_x, 
        i4_src_y, 
        ui4_width, 
        ui4_height, 
        ui1_alpha, 
        ui4_flag));
}


INT32 c_gl_surface_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    return (x_gl_surface_compose(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        i4_src_x, 
        i4_src_y, 
        ui4_width, 
        ui4_height, 
        ui1_pre_alpha, 
        e_rule, 
        ui4_flag));
}

INT32 c_gl_surface_pat_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    return (x_gl_surface_pat_compose(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x, 
        i4_dst_y, 
        ui4_dst_w, 
        ui4_dst_h, 
        i4_src_x, 
        i4_src_y, 
        ui4_src_w, 
        ui4_src_h, 
        ui1_pre_alpha,
        e_rule,
        ui4_flag));
}

extern INT32 c_gl_surface_draw_uchars(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T             aw2_chars[],
    UINT16              ui2_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    GL_RECT_T*          pt_rc_str)
{
    return (x_gl_surface_draw_uchars(
        h_surf, 
        i4_x, 
        i4_y, 
        aw2_chars, 
        ui2_len, 
        h_fnt, 
        pt_fg_clr, 
        pt_bg_clr,
        pt_rc_str));
}

INT32 c_gl_surface_draw_ustring(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    GL_RECT_T*          pt_rc_str)
{
    return (x_gl_surface_draw_ustring(
        h_surf, 
        i4_x, 
        i4_y, 
        w2s_str, 
        ui2_strlen, 
        h_fnt, 
        pt_fg_clr, 
        pt_bg_clr,
        pt_rc_str));
}

INT32 c_gl_surface_draw_ustring_ex(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x)
{
    return (x_gl_surface_draw_ustring_ex(
        h_surf,
        i4_x,
        i4_y,
        i4_bg_y,
        w2s_str,
        ui2_strlen,
        w2s_skip,
        ui2_skiplen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_edge_clr,
        b_fixed,
        ui2_ch_w,
        ui2_ch_w,
        ui2_bg_h,
        i4_bg_adj_x,
        GL_DRAW_STR_DEFAULT));
}

INT32 c_gl_surface_fill(
    GL_HSURFACE_T       h_surf, 
    const GL_COLOR_T*   pt_color,
    UINT32              ui4_flag)
{
    return (x_gl_surface_fill(h_surf, pt_color, ui4_flag));
}

INT32 c_gl_surface_reconfig(
    GL_HSURFACE_T   h_surf,
    UINT32          ui4_width,
    UINT32          ui4_height,
    GL_COLORMODE_T  e_mode)
{
    return (x_gl_surface_reconfig(h_surf, ui4_width, ui4_height, e_mode));
}

INT32 c_gl_surface_set_palette(
    GL_HSURFACE_T       h_surf, 
    GL_HPALETTE_T       h_palette)
{
    return (x_gl_surface_set_palette(h_surf, h_palette));
}

INT32 c_gl_surface_set_clipper(
    GL_HSURFACE_T       h_surf, 
    GL_HCLIPPER_T       h_clipper)
{
    return (x_gl_surface_set_clipper(h_surf, h_clipper));
}

INT32 c_gl_clipper_create(
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect, 
    GL_HCLIPPER_T*      ph_clipper)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_clipper_create(ui2_count, pt_rect, ph_clipper);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_clipper);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_clipper_set_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect)
{
    return (x_gl_clipper_set_clip_list(h_clipper, ui2_count, pt_rect));
}

INT32 c_gl_clipper_get_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16*             pui2_count, 
    GL_RECT_T*          pt_rect)
{
    return (x_gl_clipper_get_clip_list(h_clipper, pui2_count, pt_rect));
}

INT32 c_gl_palette_create(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    GL_HPALETTE_T*      ph_palette)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_palette_create(e_clrmode, ui2_count, at_color, ph_palette);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_palette);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_palette_create_with_id(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    UINT32              ui4_id,
    GL_HPALETTE_T*      ph_palette)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_gl_palette_create_with_id(e_clrmode, ui2_count, at_color, ui4_id, ph_palette);
        
        if (i4_res == GLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_palette);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = GLR_OUT_OF_HANDLES;
    }
    
    return (i4_res);
}

INT32 c_gl_palette_set_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    const GL_COLOR_T    at_color[])
{
    return (x_gl_palette_set_entries(h_palette, ui2_base, ui2_count, at_color));
}

INT32 c_gl_palette_get_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    GL_COLOR_T          at_color[])
{
    return (x_gl_palette_get_entries(h_palette, ui2_base, ui2_count, at_color));
}

INT32 c_gl_palette_match_color(
    GL_HPALETTE_T       h_palette,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{
    return (x_gl_palette_match_color(
        h_palette,
        b_cmp_alpha,
        pt_to_match,
        pt_matched,
        pui2_index));
}

INT32 c_gl_palette_match_color_ex(
    GL_HPALETTE_T       h_palette,
    UINT16              ui2_base,
    UINT16              ui2_count,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{
    return (x_gl_palette_match_color_ex(
        h_palette,
        ui2_base,
        ui2_count,
        b_cmp_alpha,
        pt_to_match,
        pt_matched,
        pui2_index));
}

INT32 c_gl_get_fbm_surface_info(
                    UINT8* pui1_cnt, 
                    UINT32* pui4_mem_size)
{ 
    return x_gl_get_fbm_surface_info(pui1_cnt, pui4_mem_size);
}

#if 0
extern INT32 c_gl_cursor_enable(
    BOOL                b_enable);

extern INT32 c_gl_cursor_get_capability(
    GL_PLANE_CAPS_T*    pt_pln_caps);

extern INT32 c_gl_cursor_get_pos(
    INT32*              pi4_x,
    INT32               pi4_y);

extern INT32 c_gl_cursor_load(
    GL_HSURFACE_T       h_surf);

extern INT32 c_gl_cursor_load_stocked(
    UINT32              ui4_id);

extern INT32 c_gl_cursor_set_pos(
    INT32               i4_x,
    INT32               i4_y);
#endif
