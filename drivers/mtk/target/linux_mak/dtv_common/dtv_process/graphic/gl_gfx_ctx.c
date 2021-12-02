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
 *         This file contains implementations of the graphics component.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
 
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "gl_dfb.h"
#include "x_gl.h"
#include "gl_dfb_surf.h"
#include "gl_surface.h"

#ifndef INLINE
#define INLINE
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MIN_NUM_VERTEX      ((UINT32) 3)
#define DEFAULT_PEN_WIDTH   ((UINT16) 1)

typedef struct __GRAPHICS_T
{
    GL_DFB_OBJ_T    super;
/*    HANDLE_T        h_lock;*/
    GL_HSURFACE_T   h_surface;
    GL_HCLIPPER_T   h_clipper;
    INT32           i4_trans_x;
    INT32           i4_trans_y;
    GL_COLOR_T      t_color;
    UINT16          ui2_pen_width;
} _GRAPHICS_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INLINE _GRAPHICS_T* _new_graphics_obj(VOID);
static BOOL _free_graphics_handle(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle);

static INT32 _paint_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc,
    GL_STYLE_T          t_style);
    
static INT32 _paint_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    GL_STYLE_T          t_style);
    
static INT32 _paint_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    GL_STYLE_T          t_style);

static BOOL _free_graphics_handle(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    _GRAPHICS_T*    pt_graphics = pv_obj;

/*    GL_DFB_BEGIN_FREE(h_handle, pv_obj)

        if (pt_graphics->h_clipper != NULL_HANDLE)
        {
            x_gl_obj_free(pt_graphics->h_clipper);
        }
        
        if (pt_graphics->h_surface != NULL_HANDLE)
        {
            x_gl_obj_free(pt_graphics->h_surface);
        }
        gl_dfb_obj_uninit(pv_obj);
        GL_FREE(pv_obj);

    GL_DFB_END_FREE(h_handle, pv_obj)*/
	if (pv_obj)
	   {
		   pt_graphics = (_GRAPHICS_T*) pv_obj;
	
		   if (pt_graphics->h_clipper != NULL_HANDLE)
		   {
			   x_gl_obj_free(pt_graphics->h_clipper);
		   }
		   
		   if (pt_graphics->h_surface != NULL_HANDLE)
		   {
			   x_gl_obj_free(pt_graphics->h_surface);
		   }
		  gl_dfb_obj_uninit(pv_obj);
          GL_FREE(pv_obj);
		   
		   return TRUE;
	   }
	
	return FALSE;

    
   /* return TRUE;*/
}

static INLINE _GRAPHICS_T* _new_graphics_obj(VOID)
{
    void * p_obj;
    p_obj = GL_MALLOC(sizeof(_GRAPHICS_T));
	GL_MEMSET(p_obj, 0, sizeof(_GRAPHICS_T));
    gl_dfb_obj_init(GLT_OBJ_GRAPHICS, p_obj);
    return p_obj;
}


static INT32 _paint_curved_shape(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    GL_STYLE_T          t_style,
    BOOL                b_conn_center)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    GL_RECT_T       t_rect;
    
    CHECK_VALID(pt_bound, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);
    RETURN_RET_IF_FAIL(i4_ret);
    t_rect = *pt_bound;
    MOVE_RECT(&t_rect, pt_graphics->i4_trans_x, pt_graphics->i4_trans_y);
    
    if (b_conn_center)
    {
        i4_ret = x_gl_surface_draw_pie(
            pt_graphics->h_surface, 
            &t_rect,
            i4_deg_start,
            i4_deg_end,
            &pt_graphics->t_color,
            pt_graphics->ui2_pen_width,
            t_style, 
            0);
    }
    else
    {
        i4_ret = x_gl_surface_draw_arc(
            pt_graphics->h_surface, 
            &t_rect,
            i4_deg_start,
            i4_deg_end,
            &pt_graphics->t_color,
            pt_graphics->ui2_pen_width,
            t_style, 
            0);
    }
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    /*x_sema_unlock(pt_graphics->h_lock);*/

    return (i4_ret);
}

static INT32 _paint_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    GL_STYLE_T          t_style)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    GL_RECT_T       t_rect;
    
    CHECK_VALID(pt_bound, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    t_rect = *pt_bound;
    MOVE_RECT(&t_rect, pt_graphics->i4_trans_x, pt_graphics->i4_trans_y);
    
    i4_ret = x_gl_surface_draw_ellipse(
        pt_graphics->h_surface, 
        &t_rect, 
        &pt_graphics->t_color,
        pt_graphics->ui2_pen_width,
        t_style, 
        0);

    gl_dfb_handle_unlock_obj(pt_graphics);

    return (i4_ret);
}

static INT32 _paint_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    GL_STYLE_T          t_style)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    GL_POINT_T*     pt_vertex;
    UINT32          i;
    
    CHECK_VALID(ui4_count >= MIN_NUM_VERTEX && at_vertex, GLR_INV_ARG);

   pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    pt_vertex = (GL_POINT_T*) GL_MALLOC(ui4_count * sizeof(GL_POINT_T));
    
    if (pt_vertex)
    {
        for (i = 0; i < ui4_count; i++)
        {
            pt_vertex[i].i4_x = at_vertex[i].i4_x + pt_graphics->i4_trans_x;
            pt_vertex[i].i4_y = at_vertex[i].i4_y + pt_graphics->i4_trans_y;
        }
        
        i4_ret = x_gl_surface_draw_polygon(
            pt_graphics->h_surface, 
            ui4_count,
            pt_vertex,
            &pt_graphics->t_color,
            pt_graphics->ui2_pen_width,
            t_style, 
            0);
    }
    else
    {
        i4_ret = GLR_NO_MEMORY;
    }
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    GL_FREE(pt_vertex);
    
    return (i4_ret);
}

static INT32 _paint_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc,
    GL_STYLE_T          t_style)
{
    _GRAPHICS_T*    pt_graphics;
    GL_ARC_DESC_T   t_arc;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    GL_RECT_T       t_rect;

    CHECK_VALID(pt_bound, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    t_rect = *pt_bound;
    MOVE_RECT(&t_rect, pt_graphics->i4_trans_x, pt_graphics->i4_trans_y);
        
    if (pt_arc)
    {
        t_arc.at_arc[0].ui4_width  = pt_arc->ui4_width;
        t_arc.at_arc[0].ui4_height = pt_arc->ui4_height;
        t_arc.ui1_arc_idx          = GL_ARC_IDX(0, 0, 0, 0);
        
        i4_ret = x_gl_surface_draw_round_rect(
            pt_graphics->h_surface, 
            &t_rect, 
            &t_arc, 
            &pt_graphics->t_color, 
            pt_graphics->ui2_pen_width,
            t_style, 
            0);
    }
    else
    {
        i4_ret = x_gl_surface_draw_rect(
            pt_graphics->h_surface, 
            &t_rect, 
            &pt_graphics->t_color,
            pt_graphics->ui2_pen_width,
            t_style,
            0);
    }
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}
    
INT32 x_gl_bitblt(
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
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);
  */  
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_blt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_blend(
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
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);
  */  
   pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    i4_ret = x_gl_surface_blend(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha,
        0);
    
   gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_compose(
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
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_compose(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_pre_alpha,
        e_rule,
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_change_surface(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surface)
{
    INT32               i4_ret;
    _GRAPHICS_T         *pt_graphics = NULL;
    
    CHECK_VALID(h_graphics, GLR_INV_ARG);
    CHECK_VALID(h_surface, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  
    
    if (pt_graphics->h_surface != NULL_HANDLE)
    {
        gl_dfb_free_handle(pt_graphics->h_surface);
    }

    pt_graphics->h_surface = gl_surface_duplicate_handle(h_surface);;
    
    gl_dfb_handle_unlock_obj(pt_graphics);
    return GLR_OK;
}

INT32 x_gl_create_graphics(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_translation,
    const GL_RECT_T*    pt_clip,
    GL_HGRAPHICS_T*     ph_graphics)
{
    /*HANDLE_T        h_lock;*/
    GL_HCLIPPER_T   h_clipper;
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
    
    pt_graphics = _new_graphics_obj();
    CHECK_VALID(pt_graphics, GLR_NO_MEMORY);
    
    /*pt_graphics->h_lock    = NULL_HANDLE;*/
    pt_graphics->h_clipper = NULL_HANDLE;
    pt_graphics->h_surface = NULL_HANDLE;

    /*if (OSR_OK != x_sema_create(&h_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK))
    {
        i4_ret = GLR_NO_MEMORY;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Creating semaphore failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _CREATE_GRAPHICS_RET;
    }
    pt_graphics->h_lock = h_lock;*/
    
    pt_graphics->h_surface = gl_surface_duplicate_handle(h_surf);
    if (pt_graphics->h_surface == NULL_HANDLE)
    {
        i4_ret = GLR_OUT_OF_HANDLES;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Out of handle handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _CREATE_GRAPHICS_RET;
    }
    
    if (pt_translation)
    {
        pt_graphics->i4_trans_x = pt_translation->i4_x;
        pt_graphics->i4_trans_y = pt_translation->i4_y;
    }
    else
    {
        pt_graphics->i4_trans_x = 0;
        pt_graphics->i4_trans_y = 0;
    }

    if (pt_clip)
    {
        i4_ret = x_gl_clipper_create(1, pt_clip, &h_clipper);
        if (GLR_OK != i4_ret)
        {
            GL_DFB_TRACE(("{GRAPHIC} ERR: Creating clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _CREATE_GRAPHICS_RET;
        }
        pt_graphics->h_clipper = h_clipper;
    }
    else
    {
        pt_graphics->h_clipper = NULL_HANDLE;
    }

    i4_ret = x_gl_surface_set_clipper(pt_graphics->h_surface, pt_graphics->h_clipper);
    if (GLR_OK != i4_ret)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Setting clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _CREATE_GRAPHICS_RET;
    }

    pt_graphics->t_color.a = pt_graphics->t_color.u1.r
                           = pt_graphics->t_color.u2.g
                           = pt_graphics->t_color.u3.b
                           = 0;

    pt_graphics->ui2_pen_width = DEFAULT_PEN_WIDTH;
    
    if (HR_OK == handle_alloc(
                    GLT_OBJ_GRAPHICS, 
                    pt_graphics, 
                    NULL, 
                    _free_graphics_handle, 
                    ph_graphics))
    {
        return GLR_OK;
    }
    /**ph_graphics = gl_dfb_new_handle(GLT_OBJ_GRAPHICS, pt_graphics, NULL, _free_graphics_handle);*/
    if(*ph_graphics != NULL_HANDLE)
    {
        return GLR_OK;
    }
    i4_ret = GLR_OUT_OF_HANDLES;
    GL_DFB_TRACE(("{GRAPHIC} ERR: Out of handle handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    
_CREATE_GRAPHICS_RET:
    
    if (pt_graphics)
    {
        if (pt_graphics->h_clipper != NULL_HANDLE)
        {
            gl_dfb_free_handle(pt_graphics->h_clipper);
        }
        
        if (pt_graphics->h_surface != NULL_HANDLE)
        {
            gl_dfb_free_handle(pt_graphics->h_surface);
        }

        /*if (pt_graphics->h_lock != NULL_HANDLE)
        {
            x_sema_delete(pt_graphics->h_lock);
        }*/
        
        GL_FREE(pt_graphics);
    }

    return (i4_ret);
}

INT32 x_gl_draw_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (_paint_curved_shape(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end, 
        GL_STYLE_OUTLINED,
        FALSE));
}

INT32 x_gl_draw_drcs_string(
    GL_HGRAPHICS_T          h_graphics,
    INT32                   i4_x,
    INT32                   i4_y,
    SM_TTX_WGL_DRCS_PTU_T   at_drcs_str[],
    UINT16                  ui2_str_len,
    UINT8                   ui1_width,
    UINT8                   ui1_height,
    GL_DRCS_INFO_T*         pt_info)
{
    _GRAPHICS_T*            pt_graphics;
    INT32                   i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    i4_ret = x_gl_surface_draw_drcs_string(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        at_drcs_str,
        ui2_str_len,
        ui1_width,
        ui1_height,
        pt_info);
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    return (i4_ret);
}

INT32 x_gl_draw_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (_paint_ellipse(h_graphics, pt_bound, GL_STYLE_OUTLINED));
}

INT32 x_gl_draw_line(
    GL_HGRAPHICS_T      h_graphics,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    GL_POINT_T      t_start, t_end;
    
    CHECK_VALID(pt_start && pt_end, GLR_INV_ARG);
    
     pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    t_start.i4_x = pt_start->i4_x + pt_graphics->i4_trans_x;
    t_start.i4_y = pt_start->i4_y + pt_graphics->i4_trans_y;
    t_end.i4_x   = pt_end->i4_x + pt_graphics->i4_trans_x;
    t_end.i4_y   = pt_end->i4_y + pt_graphics->i4_trans_y;
    
    i4_ret = x_gl_surface_draw_line(
        pt_graphics->h_surface, 
        &t_start, 
        &t_end,
        &pt_graphics->t_color,
        pt_graphics->ui2_pen_width,
        0);
    
   gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_draw_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (_paint_curved_shape(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end, 
        GL_STYLE_OUTLINED,
        TRUE));
}
INT32 x_gl_draw_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[])
{
    return (_paint_polygon(h_graphics, ui4_count, at_vertex, GL_STYLE_OUTLINED));
}

INT32 x_gl_draw_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (_paint_rect(h_graphics, pt_bound, NULL, GL_STYLE_OUTLINED));
}

INT32 x_gl_draw_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc)
{
    return (_paint_rect(h_graphics, pt_bound, pt_arc, GL_STYLE_OUTLINED));
}

INT32 x_gl_draw_subset_ustring(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_index_num,
    UINT16*             aui2_index)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
    if (aui2_index == NULL || ui2_index_num > ui2_strlen)
    {
        GL_DFB_TRACE(("{GRAPHIC} %s invalid argument. Line: %d\n", __FUNCTION__, __LINE__));
        return GLR_INV_ARG;
    }
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_draw_subset_ustring(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        w2s_str,
        ui2_strlen,
        w2s_skip,
        ui2_skiplen,
        h_fnt,
        pt_fg_clr,
        b_fixed,
        ui2_ch_w,
        ui2_ch_w,
        GL_DRAW_STR_DEFAULT,
        ui2_index_num,
        aui2_index);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_draw_ttx_string(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    GL_TTX_COLOR_T*     pt_ttx_clr,
    GL_TTX_STR_SIZE_T   e_ttx_size)
{
    _GRAPHICS_T*        pt_graphics;
    INT32               i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    i4_ret = x_gl_surface_draw_ttx_string(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        w2s_str,
        ui2_strlen,
        h_fnt,
        pt_ttx_clr,
        e_ttx_size);
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    return (i4_ret);
}

INT32 x_gl_draw_ustring(
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
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
     pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_draw_ustring_ex(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        i4_bg_y + pt_graphics->i4_trans_y,
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
        e_draw_str_mode);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_draw_ustring_ex(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_draw_ustring_ex(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        i4_bg_y + pt_graphics->i4_trans_y,
        pv_str,
        ui2_strlen,
        pv_skip,
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
        GL_DRAW_STR_DEFAULT);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_draw_ustring_skip_underline(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    VOID*               pv_skip_line,
    UINT16              ui2_skip_line_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode)
{
    
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  

    i4_ret = x_gl_surface_draw_ustring_skip_underline(
        pt_graphics->h_surface,
        i4_x + pt_graphics->i4_trans_x,
        i4_y + pt_graphics->i4_trans_y,
        i4_bg_y + pt_graphics->i4_trans_y,
        pv_str,
        ui2_strlen,
        pv_skip,
        ui2_skiplen,
        pv_skip_line,
        ui2_skip_line_len,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_edge_clr,
        b_fixed,
        ui2_ch_w,
        ui2_ch_w,
        ui2_bg_h,
        i4_bg_adj_x,
        e_draw_str_mode);
    
     gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);

}

INT32 x_gl_fill_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (_paint_curved_shape(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end, 
        GL_STYLE_FILLED,
        FALSE));
}

INT32 x_gl_fill_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (_paint_ellipse(h_graphics, pt_bound, GL_STYLE_FILLED));
}

INT32 x_gl_fill_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end)
{
    return (_paint_curved_shape(
        h_graphics, 
        pt_bound, 
        i4_deg_start, 
        i4_deg_end, 
        GL_STYLE_FILLED,
        TRUE));
}
INT32 x_gl_fill_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[])
{
    return (_paint_polygon(h_graphics, ui4_count, at_vertex, GL_STYLE_FILLED));
}

INT32 x_gl_fill_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound)
{
    return (_paint_rect(h_graphics, pt_bound, NULL, GL_STYLE_FILLED));
}

INT32 x_gl_fill_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc)
{
    return (_paint_rect(h_graphics, pt_bound, pt_arc, GL_STYLE_FILLED));
}

INT32 x_gl_get_clip(
    GL_HGRAPHICS_T      h_graphics,
    GL_RECT_T*          pt_clip)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret = GLR_OK;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    if (pt_graphics->h_clipper == NULL_HANDLE)
    {
        GL_SURF_INFO_T t_surf_info;

        i4_ret = x_gl_surface_get_info(
            pt_graphics->h_surface,
            &t_surf_info);

        if (GLR_OK == i4_ret)
        {
            SET_RECT_BY_SIZE(
                pt_clip,
                0,
                0,
                t_surf_info.ui4_width,
                t_surf_info.ui4_height);
        }
        else
        {
            SET_RECT(pt_clip, 0, 0, 0, 0);
            
            GL_DFB_TRACE(("{GRAPHIC} ERR: Getting surface info failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }
    else
    {
        UINT16 ui2_cnt;
        
        i4_ret = x_gl_clipper_get_clip_list(
                    pt_graphics->h_clipper, 
                    &ui2_cnt, 
                    pt_clip);
        
        if (GLR_OK != i4_ret)
        {
            GL_DFB_TRACE(("{GRAPHIC} ERR: Getting clip list failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        }
    }

    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_get_color(
    GL_HGRAPHICS_T      h_graphics,
    GL_COLOR_T*         pt_color)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32 i4_ret;

    CHECK_VALID(pt_color, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    *pt_color = pt_graphics->t_color;
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    return GLR_OK;
}

INT32 x_gl_get_translation(
    GL_HGRAPHICS_T      h_graphics, 
    INT32*              pi4_trans_x, 
    INT32*              pi4_trans_y)
{
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    
    INT32 i4_ret; 
    
    CHECK_VALID(pi4_trans_x && pi4_trans_y, GLR_INV_ARG);
    
     pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    *pi4_trans_x = pt_graphics->i4_trans_x;
    *pi4_trans_y = pt_graphics->i4_trans_y;
    
   gl_dfb_handle_unlock_obj(pt_graphics);

    return GLR_OK;
}

INT32 x_gl_patblt(
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
    _GRAPHICS_T*    pt_graphics;
    /*HANDLE_TYPE_T   e_type;*/
    INT32           i4_ret;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    
    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_patblt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_alpha_assigned,
        0);
    
   gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_pat_compose(
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
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
    
    
    
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    
    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_pat_compose(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_pre_alpha,
        e_rule,
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_set_clip(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_clip)
{
    _GRAPHICS_T*    pt_graphics;
    GL_HCLIPPER_T   h_clipper;
    INT32           i4_ret = GLR_OK;
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    if (pt_graphics->h_clipper == NULL_HANDLE)
    {
        if (pt_clip)
        {
            i4_ret = x_gl_clipper_create(1, pt_clip, &h_clipper);
            if (GLR_OK != i4_ret)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Creating clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _SET_CLIP_RET;
            }

            i4_ret = x_gl_surface_set_clipper(pt_graphics->h_surface, h_clipper);
            if (GLR_OK != i4_ret)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Setting clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                x_gl_obj_free(h_clipper);
                goto _SET_CLIP_RET;
            }

            pt_graphics->h_clipper = h_clipper;
        }
    }
    else
    {
        if (!pt_clip)
        {
            i4_ret = x_gl_surface_set_clipper(pt_graphics->h_surface, NULL_HANDLE);
            if (GLR_OK != i4_ret)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Setting clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                goto _SET_CLIP_RET;
            }
            x_gl_obj_free(pt_graphics->h_clipper);
            pt_graphics->h_clipper = NULL_HANDLE;
        }
        else
        {
            i4_ret = x_gl_clipper_set_clip_list(
                        pt_graphics->h_clipper, 
                        1, 
                        pt_clip);
            if (GLR_OK != i4_ret)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Setting clip list failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }

            i4_ret = x_gl_surface_set_clipper(pt_graphics->h_surface, pt_graphics->h_clipper);
            if (GLR_OK != i4_ret)
            {
                GL_DFB_TRACE(("{GRAPHIC} ERR: Setting clipper failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            }
        }
    }

_SET_CLIP_RET:
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_set_color(
    GL_HGRAPHICS_T      h_graphics,
    const GL_COLOR_T*   pt_color)
{
    _GRAPHICS_T*    pt_graphics;
    INT32 i4_ret;
    
    CHECK_VALID(pt_color, GLR_INV_ARG);
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  
    pt_graphics->t_color = *pt_color;
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return GLR_OK;
}

INT32 x_gl_set_pen_width(
    GL_HGRAPHICS_T      h_graphics,
    UINT16              ui2_width)
{
    _GRAPHICS_T*    pt_graphics;
    INT32 i4_ret; 
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  

    pt_graphics->ui2_pen_width = ui2_width;
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return GLR_OK;
}

INT32 x_gl_stretch_blt(
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
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
  
    
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);*/
    
   pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    RETURN_RET_IF_FAIL(i4_ret);  
    
    i4_ret = x_gl_surface_stretch_blt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_adv_stretch_blt_ex(
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
    GL_STRETCH_OPTION   e_option)
{
    return x_gl_adv_stretch_blt(
        h_graphics,
        h_surf,
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h);
}
INT32 x_gl_adv_stretch_blt(
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
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
  
    
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);
    */
   pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

    
    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_adv_stretch_blt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    return (i4_ret);
}

INT32 x_gl_translate(
    GL_HGRAPHICS_T      h_graphics, 
    INT32               i4_trans_x, 
    INT32               i4_trans_y)
{
    _GRAPHICS_T*    pt_graphics;
    INT32 i4_ret; 
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    pt_graphics->i4_trans_x = i4_trans_x;
    pt_graphics->i4_trans_y = i4_trans_y;
    
    gl_dfb_handle_unlock_obj(pt_graphics);


    return GLR_OK;
}


INT32 x_gl_get_surface_on_graphice(
	GL_HGRAPHICS_T      h_graphics,
	GL_HSURFACE_T*      ph_surf)
{
	_GRAPHICS_T*	pt_graphics;
	INT32 i4_ret;

	if (ph_surf == NULL)
	{
	    return GLR_INV_ARG;
	}
	
	pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);

	RETURN_RET_IF_FAIL(i4_ret);

	*ph_surf = pt_graphics->h_surface;

	gl_dfb_handle_unlock_obj(pt_graphics);

	return GLR_OK;
}


INT32 x_gl_transp_blt(
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
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
    
   
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);*/
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_transp_blt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        b_off_mask,
        pt_color_min,
        pt_color_max,
        0);
    
   gl_dfb_handle_unlock_obj(pt_graphics);


    return (i4_ret);
}

INT32 x_gl_transp_patblt(
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
    _GRAPHICS_T*    pt_graphics;
    INT32           i4_ret;
    
   
    
/*    CHECK_VALID(
        HR_OK == x_handle_get_type(h_surf, &e_type) && GLT_OBJ_SURFACE == e_type,
        GLR_INV_HANDLE);*/
    
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    i4_ret = x_gl_surface_transp_patblt(
        pt_graphics->h_surface,
        h_surf,
        pt_graphics->i4_trans_x + i4_dst_x,
        pt_graphics->i4_trans_y + i4_dst_y,
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
        0);
    
    gl_dfb_handle_unlock_obj(pt_graphics);

    return (i4_ret);
}


#ifndef MW_GL_NO_COMPRESSED_IMG


INT32 x_gl_sw_nhv_pattern_paint(
    GL_HGRAPHICS_T  h_graphics,
    GL_COLORMODE_T  e_colormode,
    GL_COLOR_T      *pt_colorkey,
    UINT8           *pui1_compressed_img,
    GL_RECT_T       *pt_work_rect)
{
    INT32           i4_ret;
    _GRAPHICS_T     *pt_graphics;
    
    /* get graphics */
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    /* transform to the global coordinate system */
    MOVE_RECT(pt_work_rect, pt_graphics->i4_trans_x, pt_graphics->i4_trans_y);
    
    i4_ret = gl_surface_sw_nhv_ptn_paint(
        pt_graphics->h_surface,
        e_colormode,
        pt_colorkey,
        pui1_compressed_img,
        pt_work_rect);
    
    /* end of get graphics */
    /*x_sema_unlock(pt_graphics->h_lock);*/
    gl_dfb_handle_unlock_obj(pt_graphics);

    return i4_ret;
}

INT32 x_gl_sw_nhv_pattern_paint_compose(
    GL_HGRAPHICS_T  h_graphics,
    GL_COLORMODE_T  e_colormode,
    GL_COLOR_T      *pt_colorkey,
    UINT8           *pui1_compressed_img,
    GL_RECT_T       *pt_work_rect)
{
    INT32           i4_ret;
    _GRAPHICS_T     *pt_graphics;
    
    /* get graphics */
    pt_graphics = gl_dfb_handle_lock_obj(GLT_OBJ_GRAPHICS, h_graphics, &i4_ret);


    RETURN_RET_IF_FAIL(i4_ret);  
    /* transform to the global coordinate system */
    MOVE_RECT(pt_work_rect, pt_graphics->i4_trans_x, pt_graphics->i4_trans_y);
    
    i4_ret = gl_surface_sw_nhv_ptn_paint_compose(
        pt_graphics->h_surface,
        e_colormode,
        pt_colorkey,
        pui1_compressed_img,
        pt_work_rect);
    
    /* end of get graphics */
    /*x_sema_unlock(pt_graphics->h_lock);*/
    gl_dfb_handle_unlock_obj(pt_graphics);

    return i4_ret;
}

INT32 x_gl_nhv_cache_flush(VOID)
{
    return gl_nhv_cache_flush();
}

#endif


