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
 *         This file contains implementation of software drawing routines.
 *---------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*
 *
 *
 * From MW implementation gl_draw with that drawing ellipses, arcs and ....(some headache and meaningless functions).
 *
 * */


#include "inc/x_common.h"

#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "graphic/gl_dfb.h"
#include "handle/u_handle.h"
#include "stdio.h"

#include "graphic/gl_dfb_sw_draw.h"
#if 0
#include <directfb.h>
#endif
/*TMP todo rollback:#include "graphic/util/x_MemPool.h"*/

#define MEM_NewObject(u4Size) GL_MALLOC(u4Size)
#define MEM_FreeObject(pvObj, u4Size) GL_FREE(pvObj)

/*TMP*/


/*#include "dbg/dbg.h"*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FULL_ANGLE              360
#define ELLIPSE_ANGLE_INCR      9

typedef VOID (*draw_pixel_fct)(VOID* pv_dst, INT32 x, UINT32 ui4_color);

#define IS_PNT_IN_RECT(x, y, rect)      ((UINT32)((x) - (rect)->i4_left) <= (rect)->i4_right - (rect)->i4_left && (UINT32)((y) - (rect)->i4_top) <= (rect)->i4_bottom - (rect)->i4_top)

typedef struct {
    INT32  i4_min;
    INT32  i4_max;
} GL_MINMAX_T;



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/



extern UINT8     gl_bit_count(GL_COLORMODE_T e_mode);
static VOID _draw_pixel_8b(VOID* pv_dst, INT32 x, UINT32 ui4_color);
static VOID _draw_pixel_16b(VOID* pv_dst, INT32 x, UINT32 ui4_color);
static VOID _draw_pixel_32b(VOID* pv_dst, INT32 x, UINT32 ui4_color);
static VOID _draw_line(
    VOID* pv_bits, UINT32 ui4_pitch, 
    INT32 x1, INT32 y1, INT32 x2, INT32 y2, GL_RECT_T *pt_clip, UINT16 ui2_line_width, UINT32 ui4_color,
    GL_MASK_T *pt_mask,
    draw_pixel_fct pf_draw_pixel);

static INT32 _fill_half_trapezoid(
    GL_COLORMODE_T  e_mode, 
    VOID*           pv_bits, 
    UINT32          ui4_pitch, 
    INT32           cx, 
    INT32           x1, 
    INT32           y1, 
    INT32           x2, 
    INT32           y2, 
    INT32           i4_left, 
    INT32           i4_right, 
    INT32           i4_top, 
    INT32           i4_bottom,
    UINT32          ui4_color);

static INT32 _fill_symemtrical_trapezoid(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch, 
    GL_FIX32_T          fcx, 
    GL_FIX32_T          fcy,
    INT32               x1, 
    INT32               y1, 
    INT32               x2, 
    INT32               y2, 
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT32              ui4_color);

static VOID _calc_vertex_array(
    const GL_RECT_T*    pt_area, 
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    UINT32              ui4_cnt,
    GL_POINT_T*         pt_vertex);

INT32 gl_dfb_locked_fill_rect(
        GL_COLORMODE_T e_mode, 
        void * pv_bits, 
        UINT32 ui4_pitch, 
        INT32 x,
        INT32 y, 
        INT32 w,
        INT32 h, 
        UINT32 ui4_color)
{
    /*p_cur_dfb_surf->SetColor(p_cur_dfb_surf, 
            (ui4_color&0xff0000)>>16,
            (ui4_color&0xff00)>>8,
            ui4_color&0xff,
            (ui4_color&0xff000000)>>24

            );
    p_cur_dfb_surf->FillRectangle(p_cur_dfb_surf, x, y, w, h + 1);*/
    int i, j;
    char * p;
    char * ps;
    int bp;
    
    bp = gl_bit_count(e_mode) >> 3;
    if(bp == 0)bp = 1;
    ps = (char *)pv_bits + y * ui4_pitch + x * bp;
    for(i = 0; i < h; i++)
    {
        for(p = ps, j = 0; j < w; j++)
        {
            GL_MEMCPY(p, &ui4_color, bp); 
            p += bp;
        }
        ps += ui4_pitch;
    } 
    return GLR_OK;
}
static VOID _get_arc_position(
    INT32               i4_w2,
    INT32               i4_h2,
    INT32               i4_wh,
    INT32               i4_angle,
    INT32               *i4_x,
    INT32               *i4_y);

static UINT32 _i8_sqrt(UINT64 ui8_x);

/*-----------------------------------------------------------------------------
 * Name: gl_compose_packed_color
 *
 * Description: This API transforms the given color into the packed color format.
 *
 * Inputs:  pt_color    References a GL_COLOR_T structure, which specifies A, R, 
 *                      G and B color component separately.
 *          e_mode      Contains the target color mode.
 *
 * Outputs: -
 *
 * Returns: Returns the color in packed color format (ARGB in 32 bit).
 ----------------------------------------------------------------------------*/
/*UINT32 gl_compose_packed_color(const GL_COLOR_T* pt_color, GL_COLORMODE_T e_mode)
{
    switch (e_mode)
    {
        case GL_COLORMODE_ARGB_CLUT2:
        case GL_COLORMODE_AYUV_CLUT2:
            return ((pt_color->u3.index) & 0x3);
            
        case GL_COLORMODE_ARGB_CLUT4:
        case GL_COLORMODE_AYUV_CLUT4:
            return ((pt_color->u3.index) & 0xf);

        case GL_COLORMODE_ARGB_CLUT8:
        case GL_COLORMODE_AYUV_CLUT8:
            return (pt_color->u3.index);

        case GL_COLORMODE_YUYV_16:
            return (GFX_COLOR_ARGB8888(pt_color->u1.y, pt_color->u2.u, pt_color->u1.y, pt_color->u3.v));
            
        case GL_COLORMODE_UYVY_16:
            return (GFX_COLOR_ARGB8888(pt_color->u2.u, pt_color->u1.y, pt_color->u3.v, pt_color->u1.y));

        case GL_COLORMODE_RGB_D565:
            return (GFX_COLOR_RGB565(pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));
            
        case GL_COLORMODE_ARGB_D1555:
            return (GFX_COLOR_ARGB1555(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_ARGB_D4444:
            return (GFX_COLOR_ARGB4444(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_ARGB_D8888:
            return (GFX_COLOR_ARGB8888(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b));

        case GL_COLORMODE_AYUV_D8888:
            return (GFX_COLOR_ARGB8888(pt_color->a, pt_color->u1.y, pt_color->u2.u, pt_color->u3.v));
            
        default:
            return 0;
    }
}*/

/*-----------------------------------------------------------------------------
 * Name: gl_calc_blt_area
 *
 * Description: This API calculates the intersection among the source rectangle, 
 *              destination rectangle and the blitting rectangle.
 *
 * Inputs:  pt_src_clip     References a GL_RECT_T structure, which specifies 
 *                          the source rectangle in the source coordinate system.
 *          i4_src_x        Contains the x-coorninate of the left-top coorner of 
 *                          the blitting rectangle (in source coordiantes).
 *          i4_src_x        Contains the y-coorninate of the left-top coorner of 
 *                          the blitting rectangle (in source coordiantes).
 *          pt_dst_clip     References a GL_RECT_T structure, which specifies 
 *                          the destination rectangle in the destination coordinate
 *                          system.
 *          i4_dst_x        Contains the x-coorninate of the left-top coorner of 
 *                          the blitting rectangle (in destination coordiantes).
 *          i4_dst_x        Contains the y-coorninate of the left-top coorner of 
 *                          the blitting rectangle (in destination coordiantes).
 *          ui4_width       Contains the width of the blitting rectangle.
 *          ui4_height      Contains the height of the blitting rectangle.
 *
 * Outputs: ppt_src         References a GL_POINT_T structure which conains the 
 *                          coordinates of the left-top corner of the intersected 
 *                          rectangle (in source coornanates).
 *          ppt_dst         References a GL_POINT_T structure which conains the 
 *                          coordinates of the left-top corner of the intersected 
 *                          rectangle (in destination coornanates).
 *          ppt_size        References a GL_POINT_T structure which contains the 
 *                          dimension of the intersected rectangle.
 *
 * Returns: TRUE            The intersected rectangle is not empty.
 *          FALSE           The intersected rectangle is empty.
 ----------------------------------------------------------------------------*/
BOOL gl_calc_blt_area(
    const GL_RECT_T*    pt_src_clip, 
    INT32               i4_src_x, 
    INT32               i4_src_y, 
    const GL_RECT_T*    pt_dst_clip, 
    INT32               i4_dst_x, 
    INT32               i4_dst_y,
    UINT32              ui4_width, 
    UINT32              ui4_height,
    GL_POINT_T*         ppt_src, 
    GL_POINT_T*         ppt_dst, 
    GL_POINT_T*         ppt_size)
{
    GL_RECT_T   t_src_copy, t_dst_copy;
    GL_RECT_T   t_rect;
    INT32       i4_x, i4_y;

    /* find the intersection of the specified rectangle and the source bitmap */
    t_src_copy.i4_left   = i4_src_x;
    t_src_copy.i4_top    = i4_src_y;
    t_src_copy.i4_right  = i4_src_x + (INT32) ui4_width;
    t_src_copy.i4_bottom = i4_src_y + (INT32) ui4_height;

    INTERSECT_RECT(&t_src_copy, pt_src_clip, &t_src_copy);
    if (RECT_EMPTY(&t_src_copy))
    {
        return FALSE;
    }

    /* find the intersection of the specified rectangle and the destination bitmap */
    t_dst_copy.i4_left   = i4_dst_x;
    t_dst_copy.i4_top    = i4_dst_y;
    t_dst_copy.i4_right  = i4_dst_x + (INT32) ui4_width;
    t_dst_copy.i4_bottom = i4_dst_y + (INT32) ui4_height;

    INTERSECT_RECT(&t_dst_copy, pt_dst_clip, &t_dst_copy);
    if (RECT_EMPTY(&t_dst_copy))
    {
        return FALSE;
    }
    
    t_rect = t_src_copy;
    i4_x   = i4_dst_x - i4_src_x;
    i4_y   = i4_dst_y - i4_src_y;

    MOVE_RECT(&t_rect, i4_x, i4_y);
    INTERSECT_RECT(&t_rect, &t_dst_copy, &t_rect);
    if (RECT_EMPTY(&t_rect))
    {
        return FALSE;
    }

    ppt_src->i4_x  = t_rect.i4_left - i4_x;
    ppt_src->i4_y  = t_rect.i4_top - i4_y;

    ppt_dst->i4_x  = t_rect.i4_left;
    ppt_dst->i4_y  = t_rect.i4_top;

    ppt_size->i4_x = RECT_W(&t_rect);
    ppt_size->i4_y = RECT_H(&t_rect);
    
    return TRUE;
}

BOOL gl_calc_blt_area_ex(
    GL_RECT_T*          pt_src_clip, 
    INT32               i4_src_x, 
    INT32               i4_src_y, 
    UINT32              ui4_src_w, 
    UINT32              ui4_src_h,
    GL_RECT_T*          pt_dst_clip, 
    INT32               i4_dst_x, 
    INT32               i4_dst_y,
    UINT32              ui4_dst_w, 
    UINT32              ui4_dst_h)
{
    GL_RECT_T   t_src_copy, t_dst_copy;
    GL_FIX32_T  t_ratio_x;
    GL_FIX32_T  t_ratio_y;

    /* find the intersection of the specified rectangle and the source bitmap */
    t_src_copy.i4_left   = i4_src_x;
    t_src_copy.i4_top    = i4_src_y;
    t_src_copy.i4_right  = i4_src_x + (INT32) ui4_src_w;
    t_src_copy.i4_bottom = i4_src_y + (INT32) ui4_src_h;

    INTERSECT_RECT(&t_src_copy, pt_src_clip, &t_src_copy);
    if (RECT_EMPTY(&t_src_copy))
    {
        return FALSE;
    }

    t_ratio_x = (GL_FIX32_T)((ui4_dst_w << GL_F32_FRAC) / ui4_src_w);
    t_ratio_y = (GL_FIX32_T)((ui4_dst_h << GL_F32_FRAC) / ui4_src_h);

    /* find the intersection of the specified rectangle and the destination bitmap */
    t_dst_copy.i4_left   = (((t_src_copy.i4_left   - i4_src_x) * t_ratio_x) >> GL_F32_FRAC) + i4_dst_x;
    t_dst_copy.i4_top    = (((t_src_copy.i4_top    - i4_src_y) * t_ratio_y) >> GL_F32_FRAC) + i4_dst_y;
    t_dst_copy.i4_right  = (((t_src_copy.i4_right  - i4_src_x) * t_ratio_x + (MAKE_BIT_MASK_32((GL_F32_FRAC - 1)))) >> GL_F32_FRAC) + i4_dst_x;
    t_dst_copy.i4_bottom = (((t_src_copy.i4_bottom - i4_src_y) * t_ratio_y + (MAKE_BIT_MASK_32((GL_F32_FRAC - 1)))) >> GL_F32_FRAC) + i4_dst_y;

    if (t_dst_copy.i4_left < pt_dst_clip->i4_left)
    {
        t_dst_copy.i4_left = pt_dst_clip->i4_left;
        t_src_copy.i4_left = MAX(
            t_src_copy.i4_left,
            ((t_dst_copy.i4_left - i4_dst_x) << GL_F32_FRAC) / t_ratio_x + i4_src_x);
    }

    if (t_dst_copy.i4_top < pt_dst_clip->i4_top)
    {
        t_dst_copy.i4_top = pt_dst_clip->i4_top;
        t_src_copy.i4_top = MAX(
            t_src_copy.i4_top,
            ((t_dst_copy.i4_top - i4_dst_y) << GL_F32_FRAC) / t_ratio_y + i4_src_y);
    }
    
    if (t_dst_copy.i4_right > pt_dst_clip->i4_right)
    {
        t_dst_copy.i4_right = pt_dst_clip->i4_right;
        t_src_copy.i4_right = MIN(
            t_src_copy.i4_right,
            ((t_dst_copy.i4_right - i4_dst_x) << GL_F32_FRAC) / t_ratio_x + i4_src_x);
    }

    if (t_dst_copy.i4_bottom > pt_dst_clip->i4_bottom)
    {
        t_dst_copy.i4_bottom = pt_dst_clip->i4_bottom;
        t_src_copy.i4_bottom = MIN(
            t_src_copy.i4_bottom,
            ((t_dst_copy.i4_bottom - i4_dst_y) << GL_F32_FRAC) / t_ratio_y + i4_src_y);
    }

    if (RECT_EMPTY(&t_dst_copy) || RECT_EMPTY(&t_src_copy))
    {
        return FALSE;
    }
    
    *pt_src_clip = t_src_copy;
    *pt_dst_clip = t_dst_copy;
    
    return TRUE;
}

BOOL gl_calc_blt_area_with_rotation(
    const GL_RECT_T*    pt_src_clip, 
    INT32               i4_src_x, 
    INT32               i4_src_y, 
    const GL_RECT_T*    pt_dst_clip, 
    INT32               i4_dst_x, 
    INT32               i4_dst_y,
    UINT32              ui4_width, 
    UINT32              ui4_height,
    UINT32              ui4_angle,
    GL_POINT_T*         ppt_src, 
    GL_POINT_T*         ppt_dst, 
    GL_POINT_T*         ppt_size)
{
    GL_RECT_T   t_src_copy, t_dst_copy;
    GL_RECT_T   t_rect;
    INT32       i4_x, i4_y;
    BOOL        b_90_degree = (ui4_angle == GL_BLT_ROTATE_CW90 || ui4_angle == GL_BLT_ROTATE_CW270);

    /* find the intersection of the specified rectangle and the source bitmap */
    t_src_copy.i4_left   = i4_src_x;
    t_src_copy.i4_top    = i4_src_y;
    t_src_copy.i4_right  = i4_src_x + (INT32) ui4_width;
    t_src_copy.i4_bottom = i4_src_y + (INT32) ui4_height;

    INTERSECT_RECT(&t_src_copy, pt_src_clip, &t_src_copy);
    if (RECT_EMPTY(&t_src_copy))
    {
        return FALSE;
    }

    /* find the intersection of the specified rectangle and the destination bitmap */
    t_dst_copy.i4_left   = i4_dst_x;
    t_dst_copy.i4_top    = i4_dst_y;
    t_dst_copy.i4_right  = i4_dst_x + ((b_90_degree)? ui4_height: ui4_width);
    t_dst_copy.i4_bottom = i4_dst_y + ((b_90_degree)? ui4_width : ui4_height);

    INTERSECT_RECT(&t_dst_copy, pt_dst_clip, &t_dst_copy);
    if (RECT_EMPTY(&t_dst_copy))
    {
        return FALSE;
    }

    if (!b_90_degree)
    {
        t_rect = t_src_copy;
        i4_x   = i4_dst_x - i4_src_x;
        i4_y   = i4_dst_y - i4_src_y;

        MOVE_RECT(&t_rect, i4_x, i4_y);
        INTERSECT_RECT(&t_rect, &t_dst_copy, &t_rect);
        if (RECT_EMPTY(&t_rect))
        {
            return FALSE;
        }

        ppt_src->i4_x  = t_rect.i4_left - i4_x;
        ppt_src->i4_y  = t_rect.i4_top - i4_y;

        ppt_dst->i4_x  = t_rect.i4_left;
        ppt_dst->i4_y  = t_rect.i4_top;

        ppt_size->i4_x = RECT_W(&t_rect);
        ppt_size->i4_y = RECT_H(&t_rect);
        
        return TRUE;
    }
    /* GL_BLT_ROTATE_CW90 || GL_BLT_ROTATE_CW270 */
    else
    {
        INT32       i4_offset_x, i4_offset_y;
        GL_RECT_T   t_rot_src;

        i4_offset_x = t_src_copy.i4_left - i4_src_x;
        i4_offset_y = t_src_copy.i4_top  - i4_src_y;

        SET_RECT_BY_SIZE(
            &t_rot_src,
            (i4_dst_x + ui4_height) - (i4_offset_y + RECT_H(&t_src_copy)),
            i4_dst_y + i4_offset_x,
            RECT_H(&t_src_copy),
            RECT_W(&t_src_copy));

        INTERSECT_RECT(&t_rot_src, &t_rot_src, &t_dst_copy);

        if (RECT_EMPTY(&t_rot_src))
        {
            return FALSE;
        }

        ppt_src->i4_x  = i4_src_x + (t_rot_src.i4_top - i4_dst_y);
        ppt_src->i4_y  = i4_src_y + (ui4_height - (t_rot_src.i4_left - i4_dst_x + RECT_W(&t_rot_src)));

        ppt_dst->i4_x  = t_rot_src.i4_left;
        ppt_dst->i4_y  = t_rot_src.i4_top;

        ppt_size->i4_x = RECT_H(&t_rot_src);
        ppt_size->i4_y = RECT_W(&t_rot_src);

        return TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: gl_identity_matrix
 *
 * Description: This API sets the given matrix to an identity matrix.
 *
 * Inputs:  -
 *
 * Outputs: pt_matrix   References a GL_MATRIX2D_T structure which is an identity 
 *                      matrix.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID gl_identity_matrix(GL_MATRIX2D_T* pt_matrix)
{
}

/*-----------------------------------------------------------------------------
 * Name: gl_clip_1d
 *
 * Description: This API performs a one-dimensional line clipping (x or y direction).
 *
 * Inputs:  px0             References the x coordinate of the first end of the line.
 *          py0             References the y coordinate of the first end of the line.
 *          px1             References the x coordinate of the second end of the line.
 *          py1             References the y coordinate of the second end of the line.
 *          min_boundary    Contains the minimum value of the clipping boundary. 
 *          max_boundary    Contains the maximum value of the clipping boundary. 
 *
 * Outputs: px0             References the x coordinate of the first end of the line 
 *                          after clipping.
 *          py0             References the y coordinate of the first end of the line
 *                          after clipping.
 *          px1             References the x coordinate of the second end of the line
 *                          after clipping.
 *          py1             References the y coordinate of the second end of the line
 *                          after clipping.
 *
 * Returns: TRUE            The line intersects with the clipping area completely 
 *                          or partially.
 *          FALSE           The line falls outside the clipping boundary completely.
 ----------------------------------------------------------------------------*/
BOOL gl_clip_1d(
    INT32* px0, INT32* py0, INT32* px1, INT32* py1, 
    INT32 min_boundary, INT32 max_boundary)
{
    GL_FIX32_T  m;
   
    if (*px0 < min_boundary)  {          /* divide line at the min_boundary */
        if (*px1 < min_boundary)
            return FALSE;
        
        m  = INT_TO_F32(*py1 - *py0) / (*px1 - *px0); /* x1 >= min_boundary */
        *py0 += F32_TO_INT(m * (min_boundary  - *px0));
        *px0  = min_boundary;
        if (*px1 > max_boundary) {
            *py1 += F32_TO_INT(m * (max_boundary - *px1));
            *px1  = max_boundary;
        }
    }
    else if (*px0 > max_boundary)  {     /* divide line at the max_boundary */
        if (*px1 > max_boundary)
            return FALSE;
        
        m  = INT_TO_F32(*py1 - *py0) / (*px1 - *px0); /* x1 <= max_boundary */
        *py0 += F32_TO_INT(m * (max_boundary  - *px0));
        *px0  = max_boundary;
        if (*px1 < min_boundary) {
            *py1 += F32_TO_INT(m * (min_boundary - *px1));
            *px1  = min_boundary;
        }
    }
    else {  /* x0 is inside the window */
      if (*px1 > max_boundary) {
            *py1 += (*py1 - *py0) * (max_boundary - *px1) / (*px1 - *px0);
         *px1  = max_boundary;
      }
      else if ( *px1 < min_boundary ) {
            *py1 += (*py1 - *py0) * (min_boundary - *px1) / (*px1 - *px0) ;
         *px1  = min_boundary;
      }
   }

   return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: gl_clip_2d
 *
 * Description: This API performs a two-dimensional line clipping (x and y direction).
 *
 * Inputs:  px0             References the x coordinate of the first end of the line.
 *          py0             References the y coordinate of the first end of the line.
 *          px1             References the x coordinate of the second end of the line.
 *          py1             References the y coordinate of the second end of the line.
 *          x_left          Contains the minimum x value of the clipping rectangle. 
 *          x_right         Contains the maximum x value of the clipping rectangle. 
 *          y_top           Contains the minimum y value of the clipping rectangle. 
 *          y_bottom        Contains the maximum y value of the clipping rectangle. 
 *
 * Outputs: px0             References the x coordinate of the first end of the line 
 *                          after clipping.
 *          py0             References the y coordinate of the first end of the line
 *                          after clipping.
 *          px1             References the x coordinate of the second end of the line
 *                          after clipping.
 *          py1             References the y coordinate of the second end of the line
 *                          after clipping.
 *
 * Returns: TRUE            The line intersects with the clipping area completely 
 *                          or partially.
 *          FALSE           The line falls outside the clipping boundary completely.
 ----------------------------------------------------------------------------*/
BOOL gl_clip_2d(
    INT32* px0, INT32* py0, INT32* px1, INT32* py1, 
    INT32 x_left, INT32 x_right, INT32 y_top, INT32 y_bottom)
{
    if (!gl_clip_1d(px0, py0, px1, py1, x_left, x_right))
        return FALSE;
    
    if (!gl_clip_1d(py0, px0, py1, px1, y_top, y_bottom))
        return FALSE;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: gl_allocate_mask
 *
 * Description: This API allocates a mask for the primitive drawing with a pen shape.
 *
 * Inputs:  ui2_width       Radius of the pen.
 *
 * Outputs: -
 *
 * Returns: The pointer of the mask. NULL for a failed allocation.
 ----------------------------------------------------------------------------*/
GL_MASK_T* gl_allocate_mask(UINT16 ui2_w)
{
    INT32       i;
    UINT16      ui2_width;
    GL_MASK_T*  pt_result;
    BOOL*       pb_head;
    
    ui2_width = (UINT16)(ui2_w / 2);

    if (ui2_width <= 0)
    {
        return NULL;
    }

    pt_result = (GL_MASK_T*) x_mem_alloc(sizeof(GL_MASK_T));

    if (pt_result == NULL)
    {
        return NULL;
    }

    ui2_width = (UINT16)(2 * ui2_width + 1);
    pt_result->ui2_width = ui2_width;

    pt_result->ppb_mask = (BOOL**) x_mem_alloc((SIZE_T) ui2_width * sizeof(BOOL*));

    if (NULL == pt_result->ppb_mask)
    {
        x_mem_free(pt_result);
        return NULL;
    }

    i = (INT32)((SIZE_T) ui2_width * (SIZE_T) ui2_width * sizeof(BOOL));
    pb_head = (BOOL*) x_mem_alloc((SIZE_T)i);

    if (NULL == pb_head)
    {
        x_mem_free(pt_result->ppb_mask);
        x_mem_free(pt_result);
        return NULL;
    }

    x_memset(pb_head, 0, (SIZE_T)i);
    
    for (i = 0; i < (INT32)ui2_width; i++)
    {
        pt_result->ppb_mask[i] = pb_head;
        pb_head += ui2_width;
    }

    return (pt_result);
}

/*-----------------------------------------------------------------------------
 * Name: gl_set_circle_mask
 *
 * Description: This API sets the mask as a circle mask
 *
 * Inputs:  pt_mask     The mask.
 *
 * Outputs: -
 *
 * Returns: NULL for failure.
 ----------------------------------------------------------------------------*/
INT32 gl_set_circle_mask(GL_MASK_T *pt_mask)
{
    INT32           i4_x, i4_y, i4_d, i4_delta_e, i4_delta_se;
    INT32           i4_radius;
    INT32           i4_i, i4_j;
    BOOL            **ppb_kernel;
    GL_MINMAX_T     *pt_minmax;
    UINT8*          pb_line;

    if (pt_mask == NULL)
    {
        return GLR_INV_ARG;
    }

    ppb_kernel  = pt_mask->ppb_mask;
    i4_radius   = ((INT32)pt_mask->ui2_width - 1) / 2;
    i4_x        = 0;
    i4_y        = i4_radius;
    i4_d        = 1 - i4_radius;
    i4_delta_e  = 3;
    i4_delta_se = -2 * i4_radius + 5;

    /* pt_minmax is used to record the min and max boundaries of a scanline in this circle */
    pt_minmax = (GL_MINMAX_T *)GL_MALLOC((SIZE_T)pt_mask->ui2_width * sizeof(GL_MINMAX_T));

    if (pt_minmax == NULL)
    {
        gl_free_mask(pt_mask);
        return GLR_NO_MEMORY;
    }

    for (i4_i = 0; i4_i < (INT32)pt_mask->ui2_width; i4_i++)
    {
        pt_minmax[i4_i].i4_max = -1;
        pt_minmax[i4_i].i4_min = (INT32)pt_mask->ui2_width;
    }

    #define CIRCLE_POINT(x, y)                                          \
    {                                                                   \
        ppb_kernel[i4_radius + (i4_y)][i4_radius + (i4_x)] = TRUE;      \
        ppb_kernel[i4_radius + (i4_x)][i4_radius + (i4_y)] = TRUE;      \
        ppb_kernel[i4_radius - (i4_x)][i4_radius + (i4_y)] = TRUE;      \
        ppb_kernel[i4_radius - (i4_y)][i4_radius + (i4_x)] = TRUE;      \
        ppb_kernel[i4_radius - (i4_y)][i4_radius - (i4_x)] = TRUE;      \
        ppb_kernel[i4_radius - (i4_x)][i4_radius - (i4_y)] = TRUE;      \
        ppb_kernel[i4_radius + (i4_x)][i4_radius - (i4_y)] = TRUE;      \
        ppb_kernel[i4_radius + (i4_y)][i4_radius - (i4_x)] = TRUE;      \
                                                                        \
        if (pt_minmax[i4_radius + (i4_y)].i4_max < i4_radius + (i4_x))  \
        {                                                               \
            pt_minmax[i4_radius + (i4_y)].i4_max = i4_radius + (i4_x);  \
        }                                                               \
        if (pt_minmax[i4_radius + (i4_y)].i4_min > i4_radius - (i4_x))  \
        {                                                               \
            pt_minmax[i4_radius + (i4_y)].i4_min = i4_radius - (i4_x);  \
        }                                                               \
                                                                        \
        if (pt_minmax[i4_radius + (i4_x)].i4_max < i4_radius + (i4_y))  \
        {                                                               \
            pt_minmax[i4_radius + (i4_x)].i4_max = i4_radius + (i4_y);  \
        }                                                               \
        if (pt_minmax[i4_radius + (i4_x)].i4_min > i4_radius - (i4_y))  \
        {                                                               \
            pt_minmax[i4_radius + (i4_x)].i4_min = i4_radius - (i4_y);  \
        }                                                               \
                                                                        \
        if (pt_minmax[i4_radius - (i4_x)].i4_max < i4_radius + (i4_y))  \
        {                                                               \
            pt_minmax[i4_radius - (i4_x)].i4_max = i4_radius + (i4_y);  \
        }                                                               \
        if (pt_minmax[i4_radius - (i4_x)].i4_min > i4_radius - (i4_y))  \
        {                                                               \
            pt_minmax[i4_radius - (i4_x)].i4_min = i4_radius - (i4_y);  \
        }                                                               \
                                                                        \
        if (pt_minmax[i4_radius - (i4_y)].i4_max < i4_radius + (i4_x))  \
        {                                                               \
            pt_minmax[i4_radius - (i4_y)].i4_max = i4_radius + (i4_x);  \
        }                                                               \
        if (pt_minmax[i4_radius - (i4_y)].i4_min > i4_radius - (i4_x))  \
        {                                                               \
            pt_minmax[i4_radius - (i4_y)].i4_min = i4_radius - (i4_x);  \
        }                                                               \
    }

    CIRCLE_POINT(i4_x, i4_y);

    while (i4_y > i4_x)
    {
        if (i4_d < 0)
        {
            i4_d        = i4_d + i4_delta_e;
            i4_delta_e  = i4_delta_e + 2;
            i4_delta_se = i4_delta_se + 2;
        }
        else
        {
            i4_d        = i4_d + i4_delta_se;
            i4_delta_e  = i4_delta_e + 2;
            i4_delta_se = i4_delta_se + 4;
            i4_y        = i4_y - 1;
        }

        i4_x = i4_x + 1;
        
        CIRCLE_POINT(i4_x, i4_y);
    }

    /* fill this circle */
    for (i4_i = 0; i4_i < (INT32)pt_mask->ui2_width; i4_i++)
    {
        pb_line = &(pt_mask->ppb_mask[i4_i][pt_minmax[i4_i].i4_min + 1]);
        
        for (i4_j = pt_minmax[i4_i].i4_min + 1; i4_j < pt_minmax[i4_i].i4_max; i4_j++)
        {
            *pb_line++ = TRUE;
        }
    }

    GL_FREE(pt_minmax);

    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_free_mask
 *
 * Description: This API releases the memory allocated to the mask
 *
 * Inputs:  pt_mask     The mask being released.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 gl_free_mask(GL_MASK_T *pt_mask)
{
    if (pt_mask == NULL)
    {
        return GLR_INV_ARG;
    }

    x_mem_free(pt_mask->ppb_mask[0]);
    x_mem_free(pt_mask->ppb_mask);
    x_mem_free(pt_mask);

    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_draw_line
 *
 * Description: This API draws a line.
 *
 * Inputs:  e_mode              Contains the color mode.
 *          pv_bits             Contains the address of the destination buffer.
 *          ui4_pitch           Contains the pitch of the destination buffer.
 *          x0                  Contains the x coordinate of the first end of the line.
 *          y0                  Contains the y coordinate of the first end of the line.
 *          x1                  Contains the x coordinate of the second end of the line.
 *          y1                  Contains the y coordinate of the second end of the line.
 *          pt_clip             Contains the clipping region.
 *          ui2_line_width      Contains the line width.
 *          pt_mask             Contains the mask for the pixel shape
 *          ui4_color           Contains the line color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK              Drawing line succeeded.
 *          GLR_INV_COLORMODE   The specified color mode is not supported.
 ----------------------------------------------------------------------------*/
INT32 gl_draw_line(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    INT32               x0, 
    INT32               y0, 
    INT32               x1, 
    INT32               y1,
    GL_RECT_T           *pt_clip,
    UINT16              ui2_line_width,
    GL_MASK_T           *pt_mask,
    UINT32              ui4_color)
{
    switch (gl_bit_count(e_mode))
    {
        case 8:
            _draw_line(pv_bits, ui4_pitch, x0, y0, x1, y1, pt_clip, ui2_line_width, ui4_color, pt_mask, _draw_pixel_8b);
            return GLR_OK;

        case 16:
            _draw_line(pv_bits, ui4_pitch, x0, y0, x1, y1, pt_clip, ui2_line_width, ui4_color, pt_mask, _draw_pixel_16b);
            return GLR_OK;

        case 32:
            _draw_line(pv_bits, ui4_pitch, x0, y0, x1, y1, pt_clip, ui2_line_width, ui4_color, pt_mask, _draw_pixel_32b);
            return GLR_OK;

        default:
            return GLR_INV_COLORMODE;
    }
}

INT32 gl_fill_round_rect(
    GL_COLORMODE_T  e_mode, 
    VOID*           pv_bits, 
    UINT32          ui4_pitch,
    const GL_RECT_T at_corner[],
    INT32           i4_left, 
    INT32           i4_right, 
    INT32           i4_top, 
    INT32           i4_bottom,
    UINT32          ui4_color)
{
    INT32       w, h;
    INT32       x, y;
    INT32       i, k;
    INT32       lx, ly;
    INT32       i4_deg, cx;
    INT32       x0, y0, x1, y1;
    INT32       i4_ret = GLR_OK;
    INT32       i4_line_cnt;
    GL_FIX32_T  fcx, fcy;

    cx = at_corner[CORNER_LEFT_TOP].i4_left + 
        ((at_corner[CORNER_RIGHT_BOTTOM].i4_right - at_corner[CORNER_LEFT_TOP].i4_left) >> 1);
    
    for (i = 0, i4_deg = 0; i < CORNER_MAX; i++, i4_deg += 90)
    {
        /* minus one to compensate for the central point */
        w = at_corner[i].i4_right - at_corner[i].i4_left;
        h = at_corner[i].i4_bottom - at_corner[i].i4_top;
        
        if (w > 0 && h > 0)
        {
            fcx = INT_TO_F32(at_corner[i].i4_left) + (INT_TO_F32(w) >> 1);
            fcy = INT_TO_F32(at_corner[i].i4_top) + (INT_TO_F32(h) >> 1);

            lx = F32_TO_INT(((g_fx_cos_tbl[i4_deg] * w) >> 1) + fcx);
            ly = F32_TO_INT(((g_fx_sin_tbl[i4_deg] * h) >> 1) + fcy);
        
            i4_line_cnt = 0;

            i4_line_cnt += _fill_half_trapezoid(
                e_mode, 
                pv_bits, 
                ui4_pitch, 
                cx, 
                lx, 
                ly, 
                lx, 
                ly + ((i & 2) - 1), 
                i4_left, 
                i4_right, 
                i4_top, 
                i4_bottom, 
                ui4_color);
        
            for (k = i4_deg + 1; k <= i4_deg + 90; k++)
            {
                x = F32_TO_INT(((g_fx_cos_tbl[k] * w) >> 1) + fcx);
                y = F32_TO_INT(((g_fx_sin_tbl[k] * h) >> 1) + fcy);

                if ((x != lx) || (y != ly))
                {
                    i4_line_cnt += _fill_half_trapezoid(
                        e_mode, 
                        pv_bits, 
                        ui4_pitch,
                        cx, 
                        lx, 
                        ly, 
                        x, 
                        y, 
                        i4_left, 
                        i4_right, 
                        i4_top, 
                        i4_bottom, 
                        ui4_color);
                    
                    lx = x;
                    ly = y;
                }
            }
        }
    }

    /* fill left remaining rectangle */
    x0 = MAX(at_corner[CORNER_LEFT_TOP].i4_left, i4_left);
    y0 = MAX(at_corner[CORNER_LEFT_TOP].i4_top + 
             ((at_corner[CORNER_LEFT_TOP].i4_bottom - at_corner[CORNER_LEFT_TOP].i4_top) >> 1),
             i4_top);
    
    x1 = MIN(cx, i4_right);
    y1 = MIN(at_corner[CORNER_LEFT_BOTTOM].i4_top + 
             ((at_corner[CORNER_LEFT_BOTTOM].i4_bottom - at_corner[CORNER_LEFT_BOTTOM].i4_top) >> 1),
             i4_bottom);

    if (gl_clip_2d(&x0, &y0, &x1, &y1, i4_left, i4_right, i4_top, i4_bottom))
    {
        i4_ret = gl_dfb_locked_fill_rect( 
            e_mode, pv_bits, ui4_pitch, x0, y0, (UINT32)(x1 - x0 + 1), (UINT32)(y1 - y0 + 1), ui4_color);
    }
    
    /* fill right remaining rectangle */
    x0 = MAX(cx + 1, i4_left);
    y0 = MAX(at_corner[CORNER_RIGHT_TOP].i4_top + 
             ((at_corner[CORNER_RIGHT_TOP].i4_bottom - at_corner[CORNER_RIGHT_TOP].i4_top) >> 1) - 1,
             i4_top);

    x1 = MIN(at_corner[CORNER_RIGHT_TOP].i4_right - 1, i4_right);
    y1 = MIN(at_corner[CORNER_RIGHT_BOTTOM].i4_top + 
             ((at_corner[CORNER_RIGHT_BOTTOM].i4_bottom - at_corner[CORNER_RIGHT_BOTTOM].i4_top) >> 1),
             i4_bottom);

    if (x0 <= x1 && y0 <= y1)
    {
        i4_ret = gl_dfb_locked_fill_rect( 
            e_mode, pv_bits, ui4_pitch, x0, y0, (UINT32)(x1 - x0 + 1), (UINT32)(y1 - y0 + 1), ui4_color);
    }
    
    return (i4_ret);
}

INT32 gl_draw_round_rect(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    const GL_RECT_T     at_corner[],
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT16              ui2_line_width,
    UINT32              ui4_color)
{
    INT32       i;
    INT32       i4_deg;
    INT32       x0, y0, x1, y1;
    INT32       i4_ret = GLR_OK;
    GL_RECT_T   t_thick_line, t_clip;

    t_clip.i4_left   = i4_left;
    t_clip.i4_top    = i4_top;
    t_clip.i4_right  = i4_right;
    t_clip.i4_bottom = i4_bottom;

    for (i = 0, i4_deg = 0; i < CORNER_MAX; i++, i4_deg += 90)
    {
        if (!RECT_EMPTY(&at_corner[i]))
        {
            gl_draw_arc(
                e_mode, 
                pv_bits, 
                ui4_pitch,
                &(at_corner[i]), 
                i4_deg, 
                i4_deg + 90,
                i4_left, 
                i4_right, 
                i4_top, 
                i4_bottom,
                ui2_line_width,
                ui4_color,
                FALSE);
        }

        /* draw line */        
        switch (i)
        {
            case CORNER_RIGHT_BOTTOM:
                x0 = at_corner[CORNER_RIGHT_BOTTOM].i4_left + ((at_corner[CORNER_RIGHT_BOTTOM].i4_right - at_corner[CORNER_RIGHT_BOTTOM].i4_left) >> 1);
                x1 = at_corner[CORNER_LEFT_BOTTOM].i4_left + ((at_corner[CORNER_LEFT_BOTTOM].i4_right - at_corner[CORNER_LEFT_BOTTOM].i4_left) >> 1);
                y0 = y1 = at_corner[CORNER_RIGHT_BOTTOM].i4_bottom - 1;
                break;
                
            case CORNER_LEFT_BOTTOM:
                x0 = x1 = at_corner[CORNER_LEFT_BOTTOM].i4_left;
                y0 = at_corner[CORNER_LEFT_BOTTOM].i4_top + ((at_corner[CORNER_LEFT_BOTTOM].i4_bottom - at_corner[CORNER_LEFT_BOTTOM].i4_top) >> 1);
                y1 = at_corner[CORNER_LEFT_TOP].i4_top + ((at_corner[CORNER_LEFT_TOP].i4_bottom - at_corner[CORNER_LEFT_TOP].i4_top) >> 1);
                break;
                
            case CORNER_LEFT_TOP:
                x0 = at_corner[CORNER_LEFT_TOP].i4_left + ((at_corner[CORNER_LEFT_TOP].i4_right - at_corner[CORNER_LEFT_TOP].i4_left) >> 1);
                x1 = at_corner[CORNER_RIGHT_TOP].i4_left + ((at_corner[CORNER_RIGHT_TOP].i4_right - at_corner[CORNER_RIGHT_TOP].i4_left) >> 1);
                y0 = y1 = at_corner[CORNER_LEFT_TOP].i4_top;
                break;
                
            case CORNER_RIGHT_TOP:
                x0 = x1 = at_corner[CORNER_RIGHT_TOP].i4_right - 1;
                y0 = at_corner[CORNER_RIGHT_BOTTOM].i4_top + ((at_corner[CORNER_RIGHT_BOTTOM].i4_bottom - at_corner[CORNER_RIGHT_BOTTOM].i4_top) >> 1);
                y1 = at_corner[CORNER_RIGHT_TOP].i4_top + ((at_corner[CORNER_RIGHT_TOP].i4_bottom - at_corner[CORNER_RIGHT_TOP].i4_top) >> 1);
                break;

            default:
                break;
        }

        if (ui2_line_width > 1)
        {
            switch (i)
            {
                case CORNER_RIGHT_BOTTOM:
                    y1 = y1 - (INT32)(ui2_line_width - 1);
                    y0 = y0 + (INT32)(ui2_line_width - 1);
                    break;

                case CORNER_LEFT_BOTTOM:
                    x0 = x0 + (INT32)(ui2_line_width - 1);
                    x1 = x1 - (INT32)(ui2_line_width - 1);
                    break;

                case CORNER_LEFT_TOP:
                    y1 = y1 + (INT32)(ui2_line_width - 1);
                    y0 = y0 - (INT32)(ui2_line_width - 1);
                    break;

                case CORNER_RIGHT_TOP:
                    x1 = x1 - (INT32)(ui2_line_width - 1);
                    x0 = x0 + (INT32)(ui2_line_width - 1);
                    break;

                default:
                    break;
            }
        }

        if (ui2_line_width == 1)
        {
            if (gl_clip_2d(&x0, &y0, &x1, &y1, i4_left, i4_right, i4_top, i4_bottom))
            {
                i4_ret = gl_dfb_locked_fill_rect(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    MIN(x0, x1), 
                    MIN(y0, y1), 
                    (UINT32)(ABS(x1 - x0) + 1), 
                    (UINT32)(ABS(y1 - y0) + 1),
                    ui4_color);
            }
        }
        else
        {
            if (x0 < x1)
            {
                t_thick_line.i4_left  = x0;
                t_thick_line.i4_right = x1;
            }
            else
            {
                t_thick_line.i4_left  = x1;
                t_thick_line.i4_right = x0;
            }

            if (y0 < y1)
            {
                t_thick_line.i4_top    = y0;
                t_thick_line.i4_bottom = y1;
            }
            else
            {
                t_thick_line.i4_top    = y1;
                t_thick_line.i4_bottom = y0;
            }
            INTERSECT_RECT(&t_thick_line, &t_thick_line, &t_clip);

            if (!RECT_EMPTY(&t_thick_line))
            {
                i4_ret = gl_dfb_locked_fill_rect(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    t_thick_line.i4_left, 
                    t_thick_line.i4_top, 
                    (UINT32)(t_thick_line.i4_right - t_thick_line.i4_left + 1), 
                    (UINT32)(t_thick_line.i4_bottom - t_thick_line.i4_top + 1),
                    ui4_color);
            }
        }
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: gl_check_corner_arc
 *
 * Description: This API checks the validity of the corner arc settings.
 *
 * Inputs:  pt_bound    References GL_RECT_T structure which contains the 
 *                      dimsnsion of the rectangle.
 *          pt_arc_desc References a GL_ARC_DESC_T structure which describes the 
 *                      arc size of each corner.
 *
 * Outputs: -
 *
 * Returns: TRUE        The corner arc settings are valid.
 *          FALSE       The corner arc settings are invalid.
 ----------------------------------------------------------------------------*/
BOOL gl_check_corner_arc(
    const GL_RECT_T*        pt_bound, 
    const GL_ARC_DESC_T*    pt_arc_desc)
{
    UINT32              ui4_min_len;
    UINT8               ui1_idx;
    
    ui4_min_len = (UINT32) MIN(pt_bound->i4_right - pt_bound->i4_left, pt_bound->i4_bottom - pt_bound->i4_top);

    ui1_idx = pt_arc_desc->ui1_arc_idx;

    return (((pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_BOTTOM << 1)) & 3].ui4_width + 
              pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_BOTTOM << 1)) & 3].ui4_width) <= ui4_min_len) &&
            ((pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_TOP << 1)) & 3].ui4_width + 
              pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_TOP << 1)) & 3].ui4_width) <= ui4_min_len) &&
            ((pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_BOTTOM << 1)) & 3].ui4_height + 
              pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_TOP << 1)) & 3].ui4_height) <= ui4_min_len) &&
            ((pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_BOTTOM << 1)) & 3].ui4_height + 
              pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_TOP << 1)) & 3].ui4_height) <= ui4_min_len));

}

/*-----------------------------------------------------------------------------
 * Name: gl_set_corner_arc
 *
 * Description: This API checks the validity of the corner arc settings.
 *
 * Inputs:  pt_bound        References GL_RECT_T structure which contains the 
 *                          dimsnsion of the rectangle.
 *          pt_arc_desc     References a GL_ARC_DESC_T structure which describes the 
 *                          arc size of each corner.
 *          ui2_line_width  Contains the line width information
 *
 * Outputs: at_arc          References an array of GL_RECT_T structures which 
 *                          describes the size and position of each corner arc.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID gl_set_corner_arc(
    const GL_RECT_T*        pt_bound, 
    const GL_ARC_DESC_T*    pt_arc_desc,
    const UINT16            ui2_line_width,
    GL_RECT_T               at_arc[])
{
    UINT8               ui1_idx;
    const GL_SIZE_T*    pt_arc_sz;
    UINT16              ui2_offset;

    if (pt_arc_desc == NULL)
    {
        return;
    }
    
    /* if the line width is 1, then we do not adjust the arc boundary */
    ui2_offset = (UINT16)((ui2_line_width == 1)? 0: ui2_line_width - 1);

    ui1_idx = pt_arc_desc->ui1_arc_idx;
    
    pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_BOTTOM << 1)) & 3]);
    at_arc[CORNER_RIGHT_BOTTOM].i4_right    = pt_bound->i4_right  - ui2_offset;
    at_arc[CORNER_RIGHT_BOTTOM].i4_bottom   = pt_bound->i4_bottom - ui2_offset;
    
    pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_BOTTOM << 1)) & 3]);
    at_arc[CORNER_LEFT_BOTTOM].i4_left      = pt_bound->i4_left   + ui2_offset;
    at_arc[CORNER_LEFT_BOTTOM].i4_bottom    = pt_bound->i4_bottom - ui2_offset;

    pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_TOP << 1)) & 3]);
    at_arc[CORNER_LEFT_TOP].i4_left         = pt_bound->i4_left + ui2_offset;
    at_arc[CORNER_LEFT_TOP].i4_top          = pt_bound->i4_top  + ui2_offset;

    pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_TOP << 1)) & 3]);
    at_arc[CORNER_RIGHT_TOP].i4_right       = pt_bound->i4_right - ui2_offset;
    at_arc[CORNER_RIGHT_TOP].i4_top         = pt_bound->i4_top   + ui2_offset;
        
    if (pt_arc_desc)
    {
        pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_BOTTOM << 1)) & 3]);
        at_arc[CORNER_RIGHT_BOTTOM].i4_left     = at_arc[CORNER_RIGHT_BOTTOM].i4_right  - (pt_arc_sz->ui4_width << 1);
        at_arc[CORNER_RIGHT_BOTTOM].i4_top      = at_arc[CORNER_RIGHT_BOTTOM].i4_bottom - (pt_arc_sz->ui4_height << 1);
        
        pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_BOTTOM << 1)) & 3]);
        at_arc[CORNER_LEFT_BOTTOM].i4_right     = at_arc[CORNER_LEFT_BOTTOM].i4_left   + (pt_arc_sz->ui4_width << 1);
        at_arc[CORNER_LEFT_BOTTOM].i4_top       = at_arc[CORNER_LEFT_BOTTOM].i4_bottom - (pt_arc_sz->ui4_height << 1);

        pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_LEFT_TOP << 1)) & 3]);
        at_arc[CORNER_LEFT_TOP].i4_right        = at_arc[CORNER_LEFT_TOP].i4_left + (pt_arc_sz->ui4_width << 1);
        at_arc[CORNER_LEFT_TOP].i4_bottom       = at_arc[CORNER_LEFT_TOP].i4_top  + (pt_arc_sz->ui4_height << 1);

        pt_arc_sz = &(pt_arc_desc->at_arc[(ui1_idx >> (CORNER_RIGHT_TOP << 1)) & 3]);
        at_arc[CORNER_RIGHT_TOP].i4_left        = at_arc[CORNER_RIGHT_TOP].i4_right - (pt_arc_sz->ui4_width << 1);
        at_arc[CORNER_RIGHT_TOP].i4_bottom      = at_arc[CORNER_RIGHT_TOP].i4_top   + (pt_arc_sz->ui4_height << 1);
    }
    else
    {
        at_arc[CORNER_RIGHT_BOTTOM].i4_left     = at_arc[CORNER_RIGHT_BOTTOM].i4_right;
        at_arc[CORNER_RIGHT_BOTTOM].i4_top      = at_arc[CORNER_RIGHT_BOTTOM].i4_bottom;
        
        at_arc[CORNER_LEFT_BOTTOM].i4_right     = at_arc[CORNER_LEFT_BOTTOM].i4_left;
        at_arc[CORNER_LEFT_BOTTOM].i4_top       = at_arc[CORNER_LEFT_BOTTOM].i4_bottom;

        at_arc[CORNER_LEFT_TOP].i4_bottom       = at_arc[CORNER_LEFT_TOP].i4_left;
        at_arc[CORNER_LEFT_TOP].i4_right        = at_arc[CORNER_LEFT_TOP].i4_top;

        at_arc[CORNER_RIGHT_TOP].i4_left        = at_arc[CORNER_RIGHT_TOP].i4_right;
        at_arc[CORNER_RIGHT_TOP].i4_bottom      = at_arc[CORNER_RIGHT_TOP].i4_top;  
    }
}

/*-----------------------------------------------------------------------------
 * Name: gl_calc_bound_box
 *
 * Description: This API calculates the bounding rectangle of the specified 
 *              polygon object.
 *
 * Inputs:  ui4_vert_num    Contains the number of vertexes.
 *          at_vertex       References an array of the polygon vertexes.
 *
 * Outputs: pt_bound        References a GL_RECT_T structure which contains the 
 *                          boundaries of the polygon.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID gl_calc_bound_box(
    UINT32              ui4_vert_num, 
    const GL_POINT_T    at_vertex[], 
    GL_RECT_T*          pt_bound)
{
    UINT32  i;
    INT32   i4_min_x, i4_max_x, i4_min_y, i4_max_y;
    INT32   i4_tst_x, i4_tst_y;
    
    i4_min_x = i4_max_x = at_vertex[0].i4_x;
    i4_min_y = i4_max_y = at_vertex[0].i4_y;
    
    for (i = 1; i < ui4_vert_num; i++)
    {
        i4_tst_x = at_vertex[i].i4_x;
        if (i4_tst_x < i4_min_x)
            i4_min_x = i4_tst_x;
        if (i4_tst_x > i4_max_x)
            i4_max_x = i4_tst_x;
        
        i4_tst_y = at_vertex[i].i4_y;
        if (i4_tst_y < i4_min_y)
            i4_min_y = i4_tst_y;
        if (i4_tst_y > i4_max_y)
            i4_max_y = i4_tst_y;
    }

    pt_bound->i4_left   = i4_min_x;
    pt_bound->i4_top    = i4_min_y;
    pt_bound->i4_right  = i4_max_x + 1;
    pt_bound->i4_bottom = i4_max_y + 1;
}

/*-----------------------------------------------------------------------------
 * Name: gl_draw_arc
 *
 * Description: This API draws the outline of an arc or a pie.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          pt_area         References a GL_RECT_T structure which specifies the 
 *                          position and size of the ellipse containg the arc. The 
 *                          position is specified in the source coordinate system.
 *          i4_deg_start    Contains the starting angle in degree.
 *          i4_deg_end      Contains the ending angle in degree.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui2_line_width  Contains the line width.
 *          ui4_color       Contains the line color in the packed format.
 *          b_conn_center   Indicates whether the center of pt_area should be 
 *                          connected or not.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Drawing arc succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_draw_arc(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    const GL_RECT_T*    pt_area, 
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT16              ui2_line_width,
    UINT32              ui4_color,
    BOOL                b_conn_center)
{
    INT32       lx, ly;
    INT32       i, i4_rem;
    INT32       x, y;
    INT32       w, h, w2, h2, wh;
    INT32       sx, sy, ex, ey;
    GL_FIX32_T  cx, cy;
    GL_RECT_T   t_clip;
    GL_MASK_T   *pt_mask = NULL;

    t_clip.i4_left   = i4_left;
    t_clip.i4_top    = i4_top;
    t_clip.i4_right  = i4_right;
    t_clip.i4_bottom = i4_bottom;
    
    while (i4_deg_end < i4_deg_start)
    {
        i4_deg_end += FULL_ANGLE;
    }

    while (i4_deg_start < 0)
    {
        i4_deg_start += FULL_ANGLE;
        i4_deg_end   += FULL_ANGLE;
    }
    
    /* minus one to compensate for the central point */
    w = pt_area->i4_right - pt_area->i4_left - 1;
    h = pt_area->i4_bottom - pt_area->i4_top - 1;
    
    cx = INT_TO_F32(pt_area->i4_left) + (INT_TO_F32(w) >> 1);
    cy = INT_TO_F32(pt_area->i4_top) + (INT_TO_F32(h) >> 1);

    i4_rem = (i4_deg_start % FULL_ANGLE);

    w2 = w * w;
    h2 = h * h;
    wh = w * h;
    
    _get_arc_position(w2, h2, wh, i4_rem, &lx, &ly);

    lx = F32_TO_INT(INT_TO_F32(lx) + cx);
    ly = F32_TO_INT(cy - INT_TO_F32(ly));

    pt_mask = gl_allocate_mask(ui2_line_width);
    if (gl_set_circle_mask(pt_mask) != GLR_OK)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Set circle mask failed at L%d\r\n\r\n", __LINE__));
    }

    if (b_conn_center)
    {
        sx = F32_TO_INT(cx);
        sy = F32_TO_INT(cy);
        ex = lx; 
        ey = ly;
        
        if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
        {
            gl_draw_line(e_mode, pv_bits, ui4_pitch, sx, sy, ex, ey, &t_clip, ui2_line_width, pt_mask, ui4_color);
        }
    }
    
    for (i = i4_deg_start + 1; (i <= i4_deg_end); i++)
    {
        i4_rem = (i % FULL_ANGLE);

        _get_arc_position(w2, h2, wh, i4_rem, &x, &y);
        
        x = F32_TO_INT(INT_TO_F32(x) + cx);
        y = F32_TO_INT(cy - INT_TO_F32(y));
        
        sx = lx;
        sy = ly;
        ex = x; 
        ey = y;
        
        if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
        {
            gl_draw_line(e_mode, pv_bits, ui4_pitch, sx, sy, ex, ey, &t_clip, ui2_line_width, pt_mask, ui4_color);
        }

        lx = x;
        ly = y;
    }

    if (b_conn_center)
    {
        sx = F32_TO_INT(cx);
        sy = F32_TO_INT(cy);
        ex = lx; 
        ey = ly;
        
        if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
        {
            gl_draw_line(e_mode, pv_bits, ui4_pitch, sx, sy, ex, ey, &t_clip, ui2_line_width, pt_mask, ui4_color);
        }
    }

    gl_free_mask(pt_mask);
    
    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_fill_arc
 *
 * Description: This API fills the interior an arc or a pie.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_dst_x        Contsins the x coordinate of the left-top corner on the 
 *                          destination buffer.
 *          i4_dst_y        Contsins the y coordinate of the left-top corner on the 
 *                          destination buffer.
 *          pt_area         References a GL_RECT_T structure which specifies the 
 *                          position and size of the ellipse containg the arc. The 
 *                          position is specified in the source coordinate system.
 *          i4_deg_start    Contains the starting angle in degree.
 *          i4_deg_end      Contains the ending angle in degree.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui4_color       Contains the line color in the packed format.
 *          b_conn_center   Indicates whether the center of pt_area should be 
 *                          connected or not.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Drawing arc succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_fill_arc(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    const GL_RECT_T*    pt_area, 
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT32              ui4_color,
    BOOL                b_conn_center)
{
    UINT32      ui4_vcnt;
    GL_POINT_T* pt_vertex;
    INT32       i4_ret;
    UINT32      ui4_vert_ex = (UINT32)(b_conn_center ? 1 : 0);
    
    while (i4_deg_end < i4_deg_start)
    {
        i4_deg_end += FULL_ANGLE;
    }

    while (i4_deg_start < 0)
    {
        i4_deg_start += FULL_ANGLE;
        i4_deg_end   += FULL_ANGLE;
    }
    
    ui4_vcnt = (UINT32)(i4_deg_end - i4_deg_start + 1 + ui4_vert_ex);
    pt_vertex = (GL_POINT_T*) MEM_NewObject(ui4_vcnt * sizeof(GL_POINT_T));
    if (!pt_vertex)
    {
        return GLR_NO_MEMORY;
    }
    
    _calc_vertex_array(
        pt_area,
        i4_deg_start,
        i4_deg_end,
        ui4_vcnt - ui4_vert_ex,
        pt_vertex);

    if (b_conn_center)
    {
        pt_vertex[ui4_vcnt-1].i4_x = pt_area->i4_left + ((pt_area->i4_right  - pt_area->i4_left - 1) >> 1);
        pt_vertex[ui4_vcnt-1].i4_y = pt_area->i4_top  + ((pt_area->i4_bottom - pt_area->i4_top - 1)  >> 1);
    }
    
    i4_ret = gl_fill_polygon(
        e_mode, 
        pv_bits, 
        ui4_pitch,
        ui4_vcnt,
        pt_vertex,
        i4_left, 
        i4_right, 
        i4_top, 
        i4_bottom,
        ui4_color);
    
    MEM_FreeObject(pt_vertex, ui4_vcnt * sizeof(GL_POINT_T));

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: gl_draw_ellipse
 *
 * Description: This API draws the outline of an ellipse.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_dst_x        Contsins the x coordinate of the left-top corner on the 
 *                          destination buffer.
 *          i4_dst_y        Contsins the y coordinate of the left-top corner on the 
 *                          destination buffer.
 *          pt_area         References a GL_RECT_T structure which specifies the 
 *                          position and size of the ellipse. The position is 
 *                          specified in the source coordinate system.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui2_line_width  Contains the line width.
 *          ui4_color       Contains the line color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Drawing ellipse succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_draw_ellipse(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    const GL_RECT_T*    pt_area,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT16              ui2_line_width,
    UINT32              ui4_color)
{
    INT32       lx, ly;
    INT32       i;
    INT32       x, y;
    INT32       w, h;
    INT32       sx, sy, ex, ey;
    GL_FIX32_T  cx, cy;
    GL_RECT_T   t_clip;
    GL_MASK_T   *pt_mask = NULL;

    t_clip.i4_left   = i4_left;
    t_clip.i4_top    = i4_top;
    t_clip.i4_right  = i4_right;
    t_clip.i4_bottom = i4_bottom;
    
    /* minus one to compensate for the central point */
    w = pt_area->i4_right - pt_area->i4_left - 1;
    h = pt_area->i4_bottom - pt_area->i4_top - 1;
    
    cx = INT_TO_F32(pt_area->i4_left) + (INT_TO_F32(w) >> 1);
    cy = INT_TO_F32(pt_area->i4_top) + (INT_TO_F32(h) >> 1);

    lx = F32_TO_INT(((g_fx_cos_tbl[0] * w) >> 1) + cx);
    ly = F32_TO_INT(cy - ((g_fx_sin_tbl[0] * h) >> 1));

    pt_mask = gl_allocate_mask(ui2_line_width);
    if (gl_set_circle_mask(pt_mask) != GLR_OK)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Set circle mask failed at L%d\r\n\r\n", __LINE__));
    }
        
    for (i = ELLIPSE_ANGLE_INCR; i <= 90; i += ELLIPSE_ANGLE_INCR)
    {
        x = F32_TO_INT(((g_fx_cos_tbl[i] * w) >> 1) + cx);
        y = F32_TO_INT(cy - ((g_fx_sin_tbl[i] * h) >> 1));

        if ((x != lx) || (y != ly))
        {
            sx = lx;
            sy = ly;
            ex = x; 
            ey = y;
            if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
            {
                gl_draw_line(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    sx, 
                    sy, 
                    ex, 
                    ey,
                    &t_clip,
                    ui2_line_width,
                    pt_mask,
                    ui4_color);
            }
            
            sx = lx;
            sy = F32_TO_INT(cy << 1) - ly;
            ex = x; 
            ey = F32_TO_INT(cy << 1) - y;
            if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
            {
                gl_draw_line(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    sx, 
                    sy, 
                    ex, 
                    ey,
                    &t_clip,
                    ui2_line_width,
                    pt_mask,
                    ui4_color);
            }
            
            sx = F32_TO_INT(cx << 1) - lx;
            sy = F32_TO_INT(cy << 1) - ly;
            ex = F32_TO_INT(cx << 1) - x;
            ey = F32_TO_INT(cy << 1) - y;
            if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
            {
                gl_draw_line(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    sx, 
                    sy, 
                    ex, 
                    ey,
                    &t_clip,
                    ui2_line_width,
                    pt_mask,
                    ui4_color);
            }

            sx = F32_TO_INT(cx << 1) - lx;
            sy = ly;
            ex = F32_TO_INT(cx << 1) - x;
            ey = y;
            if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
            {
                gl_draw_line(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    sx, 
                    sy, 
                    ex, 
                    ey,
                    &t_clip,
                    ui2_line_width,
                    pt_mask,
                    ui4_color);
            }

            lx = x;
            ly = y;
        }
    }

    gl_free_mask(pt_mask);

    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_fill_ellipse
 *
 * Description: This API fills an ellipse.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_dst_x        Contsins the x coordinate of the left-top corner on the 
 *                          destination buffer.
 *          i4_dst_y        Contsins the y coordinate of the left-top corner on the 
 *                          destination buffer.
 *          pt_area         References a GL_RECT_T structure which specifies the 
 *                          position and size of the ellipse. The position is 
 *                          specified in the source coordinate system.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui4_color       Contains the polygon color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Filling ellipse succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_fill_ellipse(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    const GL_RECT_T*    pt_area,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT32              ui4_color)
{
    INT32       lx, ly;
    INT32       i;
    INT32       x, y;
    INT32       w, h;
    GL_FIX32_T  fcx, fcy;
    INT32       i4_line_cnt = 0;
    
    /* minus one to compensate for the central point */
    w = pt_area->i4_right - pt_area->i4_left - 1;
    h = pt_area->i4_bottom - pt_area->i4_top - 1;

    fcx = INT_TO_F32(pt_area->i4_left) + (INT_TO_F32(w) >> 1);
    fcy = INT_TO_F32(pt_area->i4_top) + (INT_TO_F32(h) >> 1);

    lx = F32_TO_INT(((g_fx_cos_tbl[0] * w) >> 1) + fcx);
    ly = F32_TO_INT(fcy - ((g_fx_sin_tbl[0] * h) >> 1));
        
    i4_line_cnt += _fill_symemtrical_trapezoid(
        e_mode, 
        pv_bits, 
        ui4_pitch,
        fcx, 
        fcy, 
        lx, 
        ly, 
        lx, 
        ly + 1, 
        i4_left, 
        i4_right, 
        i4_top, 
        i4_bottom, 
        ui4_color);

    for (i = ELLIPSE_ANGLE_INCR; i <= 90; i += ELLIPSE_ANGLE_INCR)
    {
        x = F32_TO_INT(((g_fx_cos_tbl[i] * w) >> 1) + fcx);
        y = F32_TO_INT(fcy - ((g_fx_sin_tbl[i] * h) >> 1));
        
        if (y != ly)
        {
            i4_line_cnt += _fill_symemtrical_trapezoid(
                e_mode, 
                pv_bits, 
                ui4_pitch,
                fcx, 
                fcy, 
                lx, 
                ly, 
                x, 
                y, 
                i4_left, 
                i4_right, 
                i4_top, 
                i4_bottom, 
                ui4_color);
            
            lx = x;
            ly = y;
        }
        else
        {
            lx = MAX(x, lx);
        }
    }

    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_fill_polygon
 *
 * Description: This API fills a polygon.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_dst_x        Contsins the x coordinate of the left-top corner on the 
 *                          destination buffer.
 *          i4_dst_y        Contsins the y coordinate of the left-top corner on the 
 *                          destination buffer.
 *          pt_poly         References a polygon object.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui4_color       Contains the polygon color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Filling polygon succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_fill_polygon(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    UINT32              ui4_vert_num,
    const GL_POINT_T*   pt_vertex,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT32              ui4_color)
{
    INT32       x1, x2, y1, y2;
    INT32       i, j, y;
    INT32       i4_ints, i4_index;
    INT32       i4_ind1, i4_ind2;
    INT32       i4_min_x, i4_max_x;
    INT32       i4_line_cnt = 0;
    INT32*      pi4_intersect;

    pi4_intersect = (INT32*) MEM_NewObject(ui4_vert_num * sizeof(INT32));
    if (!pi4_intersect)
    {
        return GLR_NO_MEMORY;
    }
    
    for (y = i4_top; y <= i4_bottom; y++)
    {
        for (i4_ints = 0, i = 0; i < (INT32) ui4_vert_num; i++)
	    {
	        if (i == 0)
	        {
	            i4_ind1 = (INT32) ui4_vert_num - 1;
	            i4_ind2 = 0;
	        }
	        else
	        {
	            i4_ind1 = i - 1;
	            i4_ind2 = i;
	        }
            
            y1 = pt_vertex[i4_ind1].i4_y;
            y2 = pt_vertex[i4_ind2].i4_y;
            
            if (y1 < y2)
	        {
	            x1 = pt_vertex[i4_ind1].i4_x;
	            x2 = pt_vertex[i4_ind2].i4_x;
	        }
	        else if (y1 > y2)
	        {
                y2 = pt_vertex[i4_ind1].i4_y;
                y1 = pt_vertex[i4_ind2].i4_y;
                x2 = pt_vertex[i4_ind1].i4_x;
                x1 = pt_vertex[i4_ind2].i4_x;
            }
	        else
	        {
	            continue;
	        }

	        if (((y >= y1) && (y < y2)) || ((y == i4_bottom) && (y > y1) && (y <= y2)))
	        {
                pi4_intersect[i4_ints++] = HPF_TO_INT(INT_TO_HPF((y - y1) * (x2 - x1)) / (y2 - y1)) + x1;
	        }
	    }

        /* sort the intersection list */
        for (i = 1; (i < i4_ints); i++)
        {
            i4_index = pi4_intersect[i];
            j = i;
            while ((j > 0) && (pi4_intersect[j - 1] > i4_index))
            {
                pi4_intersect[j] = pi4_intersect[j - 1];
                j--;
            }
            pi4_intersect[j] = i4_index;
        }

        i4_ints = ((i4_ints >> 1) << 1);
        
        for (i = 0; i < i4_ints; i += 2)
	    {
            i4_min_x = pi4_intersect[i];
            i4_max_x = pi4_intersect[i + 1];

            /* perform post-clipping */
            if ((i4_min_x > i4_right) || (i4_max_x < i4_left))
                continue;

            if (i4_min_x < i4_left)
                i4_min_x = i4_left;
            
            if (i4_max_x > i4_right)
                i4_max_x = i4_right;
            
            gl_dfb_locked_fill_rect(
                e_mode, 
                pv_bits, 
                ui4_pitch, 
                i4_min_x, 
                y, 
                (UINT32)(i4_max_x - i4_min_x + 1), 
                1,
                ui4_color);
            
            i4_line_cnt++;
	    }
    }
    
    MEM_FreeObject(pi4_intersect, ui4_vert_num * sizeof(INT32));
    
    return GLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: gl_draw_polygon
 *
 * Description: This API draws the outline of an polygon.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_dst_x        Contsins the x coordinate of the left-top corner on the 
 *                          destination buffer.
 *          i4_dst_y        Contsins the y coordinate of the left-top corner on the 
 *                          destination buffer.
 *          ui4_vert_num    Contains the number of vertex.
 *          pt_vertex       References an array of GL_POINT_T structure, which 
 *                          specified the coordinates of each vertex (in source 
 *                          coordinate).
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui2_line_width  Contains the line width.
 *          ui4_color       Contains the line color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Drawing polygon succeeded.
 ----------------------------------------------------------------------------*/
INT32 gl_draw_polygon(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch,
    UINT32              ui4_vert_num, 
    const GL_POINT_T*   pt_vertex,
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT16              ui2_line_width,
    UINT32              ui4_color,
    BOOL                b_closed)
{
    UINT32      i;
    INT32       sx, sy, ex, ey;
    GL_RECT_T   t_clip;
    GL_MASK_T   *pt_mask = NULL;

    t_clip.i4_left   = i4_left;
    t_clip.i4_top    = i4_top;
    t_clip.i4_right  = i4_right;
    t_clip.i4_bottom = i4_bottom;

    pt_mask = gl_allocate_mask(ui2_line_width);
    if (gl_set_circle_mask(pt_mask) != GLR_OK)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Set circle mask failed at L%d\r\n\r\n", __LINE__));
    }

    for (i = 0; i < ui4_vert_num - 1; i++)
    {
        sx = pt_vertex[i].i4_x;
        sy = pt_vertex[i].i4_y;
        ex = pt_vertex[i + 1].i4_x;
        ey = pt_vertex[i + 1].i4_y;
        if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
        {
            gl_draw_line(
                e_mode, 
                pv_bits, 
                ui4_pitch, 
                sx, 
                sy, 
                ex, 
                ey,
                &t_clip,
                ui2_line_width,
                pt_mask,
                ui4_color);
        }
    }
    
    if (b_closed)
    {
        sx = pt_vertex[i].i4_x;
        sy = pt_vertex[i].i4_y;
        ex = pt_vertex[0].i4_x;
        ey = pt_vertex[0].i4_y;

        if ((sx != ex) || (sy != ey))
        {
            if (gl_clip_2d(&sx, &sy, &ex, &ey, i4_left, i4_right, i4_top, i4_bottom))
            {
                gl_draw_line(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    sx, 
                    sy, 
                    ex, 
                    ey,
                    &t_clip,
                    ui2_line_width,
                    pt_mask,
                    ui4_color);
            }
        }
    }
    
    gl_free_mask(pt_mask);

    return GLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: gl_bit_count
 *
 * Description: This API gives the number of bits for a single pixel.
 *
 * Inputs:  e_mode      Contains the specified color mode.
 *
 * Outputs: -
 *
 * Returns: Number of bits used to represent a single pixel for the specified
 *          color mode.
 ----------------------------------------------------------------------------*/
UINT8 gl_bit_count(GL_COLORMODE_T e_mode)
{
    static UINT8 s_aui1gl_bit_count[] = { 2, 4, 8, 16, 16, 32, 2, 4, 8, 16, 16, 16, 32 };

    return (s_aui1gl_bit_count[e_mode]);
}

/*-----------------------------------------------------------------------------
 * Name: _draw_pixel_8b
 *
 * Description: This API draws a single pixel for 8-bit color mode.
 *
 * Inputs:  pv_dst      References the beginning of the line of the target buffer.
 *          x           Contains the offset (in unit of 8-bit) from pv_dst.
 *          ui4_color   Contains the specified color.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _draw_pixel_8b(VOID* pv_dst, INT32 x, UINT32 ui4_color)
{
    ((UINT8*) pv_dst)[x] = (UINT8) ui4_color;
}

/*-----------------------------------------------------------------------------
 * Name: _draw_pixel_16b
 *
 * Description: This API draws a single pixel for 16-bit color mode.
 *
 * Inputs:  pv_dst      References the beginning of the line of the target buffer.
 *          x           Contains the offset (in unit of 16-bit) from pv_dst.
 *          ui4_color   Contains the specified color.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _draw_pixel_16b(VOID* pv_dst, INT32 x, UINT32 ui4_color)
{
    ((UINT16*) pv_dst)[x] = (UINT16) ui4_color;
}

/*-----------------------------------------------------------------------------
 * Name: _draw_pixel_32b
 *
 * Description: This API draws a single pixel for 32-bit color mode.
 *
 * Inputs:  pv_dst      References the beginning of the line of the target buffer.
 *          x           Contains the offset (in unit of 32-bit) from pv_dst.
 *          ui4_color   Contains the specified color.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _draw_pixel_32b(VOID* pv_dst, INT32 x, UINT32 ui4_color)
{
    ((UINT32*) pv_dst)[x] = ui4_color;
#if 0
    ((UINT32*) pv_dst)[x + 1] = ui4_color;
    ((UINT32*) pv_dst)[x + 2] = ui4_color;
    ((UINT32*) pv_dst)[x + 3] = ui4_color;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _draw_line
 *
 * Description: This API draws a line by iteratively calling the pixel drawing routine.
 *
 * Inputs:  pv_bits             Contains the address of the destination buffer.
 *          ui4_pitch           Contains the pitch of the destination buffer.
 *          x1                  Contains the x coordinate of the first end of the line.
 *          y1                  Contains the y coordinate of the first end of the line.
 *          x2                  Contains the x coordinate of the second end of the line.
 *          y2                  Contains the y coordinate of the second end of the line.
 *          pt_clip             Contains the clipping region.
 *          ui2_line_width      Contains the line width.
 *          ui4_color           Contains the line color in the packed format.
 *          pt_mask             Contains the mask for the pixel shape
 *          pf_draw_pixel       References the pixel drawing routine.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _draw_line(
    VOID*           pv_bits, 
    UINT32          ui4_pitch, 
    INT32           x1, 
    INT32           y1, 
    INT32           x2, 
    INT32           y2,
    GL_RECT_T       *pt_clip,
    UINT16          ui2_line_width,
    UINT32          ui4_color,
    GL_MASK_T       *pt_mask,
    draw_pixel_fct  pf_draw_pixel)
{
    INT32   dx, dy, incr1, incr2, incr3, d, x, y, xend, yend, xdirflag, ydirflag;
    UINT8*  pui1_line;
    INT32   i4_i, i4_j;
    INT32   i4_x, i4_y;
    UINT8   *pui1_old_line;
    UINT32  ui4_buffer_offset = 0;
    BOOL    *pb_row_mask;
    
    dx = ABS (x2 - x1);
    dy = ABS (y2 - y1);

    if (pt_mask != NULL)
    {
        ui4_buffer_offset = (UINT32)(((INT32)pt_mask->ui2_width / 2) * ui4_pitch);
    }
    
    if (dy <= dx)
    {
        d     = 2 * dy - dx;
        incr1 = 2 * dy;
        incr2 = 2 * (dy - dx);
        if (x1 > x2)
	    {
	        x        = x2;
	        y        = y2;
	        ydirflag = (-1);
	        xend     = x1;
	    }
        else
	    {
	        x        = x1;
	        y        = y1;
	        ydirflag = 1;
	        xend     = x2;
	    }

        pui1_line = (UINT8*) (pv_bits) + ui4_pitch * y;
        pf_draw_pixel(pui1_line, x, ui4_color);

        #define WIDEN_PIXEL()                                                                               \
        {                                                                                                   \
            if (pt_mask != NULL)                                                                            \
            {                                                                                               \
                INT32 i4_l = (INT32) pt_mask->ui2_width;                                                    \
                INT32 i4_r = i4_l / 2;                                                                      \
                i4_y = y + i4_r;                                                                            \
                pui1_old_line = pui1_line;                                                                  \
                pui1_line = pui1_line + ui4_buffer_offset;                                                  \
                for (i4_i = i4_l - 1; i4_i >= 0; i4_i--)                                                    \
                {                                                                                           \
                    i4_x = x + i4_r;                                                                        \
                    pb_row_mask = pt_mask->ppb_mask[i4_i];                                                  \
                    for (i4_j = i4_l - 1; i4_j >= 0; i4_j--)                                                \
                    {                                                                                       \
                        if (pb_row_mask[i4_j] == TRUE && WITHIN_RECT(pt_clip, i4_x, i4_y))                  \
                        {                                                                                   \
                            pf_draw_pixel(pui1_line, i4_x, ui4_color);                                      \
                        }                                                                                   \
                                                                                                            \
                        i4_x = i4_x - 1;                                                                    \
                    }                                                                                       \
                                                                                                            \
                    i4_y = i4_y - 1;                                                                        \
                    pui1_line = pui1_line - ui4_pitch;                                                      \
                }                                                                                           \
                pui1_line = pui1_old_line;                                                                  \
            }                                                                                               \
        }

        WIDEN_PIXEL();

        incr3 = (((y2 - y1) * ydirflag) > 0) ? 1 : (-1);
        while (x < xend)
        {
            x++;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
	            y += incr3;
	            d += incr2;
                pui1_line += (incr3 > 0) ? (INT32)ui4_pitch : (- (INT32) ui4_pitch);
	        }

            pf_draw_pixel(pui1_line, x, ui4_color);

            WIDEN_PIXEL();
        }
    }
    else
    {
        d     = 2 * dx - dy;
        incr1 = 2 * dx;
        incr2 = 2 * (dx - dy);
        if (y1 > y2)
	    {
	        y        = y2;
	        x        = x2;
	        yend     = y1;
	        xdirflag = (-1);
	    }
        else
	    {
	        y        = y1;
            x        = x1;
            yend     = y2;
            xdirflag = 1;
	    }

        pui1_line = (UINT8*) (pv_bits) + ui4_pitch * y;
        pf_draw_pixel(pui1_line, x, ui4_color);

        WIDEN_PIXEL();

        incr3 = (((x2 - x1) * xdirflag) > 0) ? 1 : (-1);

        while (y < yend)
        {
            y++;
            pui1_line += ui4_pitch;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
	            x += incr3;
	            d += incr2;
	        }

            pf_draw_pixel(pui1_line, x, ui4_color);

            WIDEN_PIXEL();
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _fill_symemtrical_trapezoid
 *
 * Description: This API fills a symemtrical trapezoid.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_x_diff       Contains the x offset between source and destination
 *                          coordinate system.
 *          i4_y_diff       Contains the y offset between source and destination
 *                          coordinate system.
 *          fcx             Contains the x coordinate of the center.
 *          fcy             Contains the y coordinate of the center.
 *          x1              Contains the x coordinate of the first end of the 
 *                          right side.
 *          y1              Contains the y coordinate of the first end of the
 *                          right side.
 *          x2              Contains the x coordinate of the second end of the
 *                          right side.
 *          y2              Contains the y coordinate of the second end of the
 *                          right side.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui4_color       Contains the line color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Filling ellipse succeeded.
 ----------------------------------------------------------------------------*/
static INT32 _fill_symemtrical_trapezoid(
    GL_COLORMODE_T      e_mode, 
    VOID*               pv_bits, 
    UINT32              ui4_pitch, 
    GL_FIX32_T          fcx, 
    GL_FIX32_T          fcy,
    INT32               x1, 
    INT32               y1, 
    INT32               x2, 
    INT32               y2, 
    INT32               i4_left, 
    INT32               i4_right, 
    INT32               i4_top, 
    INT32               i4_bottom,
    UINT32              ui4_color)
{
    INT32   dx, dy, incr1, incr2, incr3, d, x, y, xend, yend, xdirflag, ydirflag;
    INT32   i4_min_x, i4_max_x;
    INT32   i4_rel_y;
    INT32   i4_prv_x;
    INT32   i4_line_cnt = 0;
    
    dx = ABS (x2 - x1);
    dy = ABS (y2 - y1);
    
    if (dy <= dx)
    {
        d = 2 * dy - dx;
        incr1 = 2 * dy;
        incr2 = 2 * (dy - dx);
        if (x1 > x2)
	    {
	        x = x2;
	        y = y2;
	        ydirflag = (-1);
	        xend = x1;
	    }
        else
	    {
	        x = x1;
	        y = y1;
	        ydirflag = 1;
	        xend = x2;
	    }
        
        incr3 = (((y2 - y1) * ydirflag) > 0) ? 1 : (-1);
        while (x < xend)
        {
            i4_prv_x = x++;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
                i4_min_x = MAX(i4_left, F32_TO_INT(fcx << 1) - i4_prv_x);
                i4_max_x = MIN(i4_right, i4_prv_x);
                if (i4_min_x <= i4_max_x)
                {
                    if (i4_top <= y && y <= i4_bottom)
                    {
                        gl_dfb_locked_fill_rect(
                            e_mode, 
                            pv_bits, 
                            ui4_pitch, 
                            i4_min_x, 
                            y, 
                            (UINT32)(i4_max_x - i4_min_x + 1), 
                            1, 
                            ui4_color);
                        i4_line_cnt++;
                    }

                    i4_rel_y = F32_TO_INT(fcy << 1) - y;
                    if (i4_top <= i4_rel_y && i4_rel_y <= i4_bottom)
                    {
                        gl_dfb_locked_fill_rect(
                            e_mode, 
                            pv_bits, 
                            ui4_pitch, 
                            i4_min_x, 
                            i4_rel_y, 
                            (UINT32)(i4_max_x - i4_min_x + 1), 
                            1, 
                            ui4_color);
                        i4_line_cnt++;
                    }
                }
                
	            y += incr3;
	            d += incr2;
	        }
        }
/*
        if (i4_top <= y && y <= i4_bottom)
        {
            i4_min_x = MAX(i4_left, F32_TO_INT(fcx << 1) - x);
            i4_max_x = MIN(i4_right, x);
            if (i4_min_x <= i4_max_x)
            {
                gl_dfb_locked_fill_rect(e_mode, pv_bits, ui4_pitch, 
                    i4_min_x + i4_x_diff, y + i4_y_diff, 
                    (i4_max_x - i4_min_x + 1), 1, ui4_color);
            }
            i4_line_cnt++;
        }
*/        
    }
    else
    {
        d = 2 * dx - dy;
        incr1 = 2 * dx;
        incr2 = 2 * (dx - dy);
        if (y1 > y2)
	    {
	        y = y2;
	        x = x2;
	        yend = MIN(y1, i4_bottom + 1);
	        xdirflag = (-1);
	    }
        else
	    {
	        y = y1;
            x = x1;
            yend = MIN(y2, i4_bottom + 1);
            xdirflag = 1;
	    }
#if 0
        if (y > i4_bottom || yend < i4_top)
        {
            return (i4_line_cnt);
        }
#endif
/*        
        if (i4_top <= y)
        {
            i4_min_x = MAX(i4_left, F32_TO_INT(fcx << 1) - x);
            i4_max_x = MIN(i4_right, x);
            if (i4_min_x <= i4_max_x)
            {
                gl_dfb_locked_fill_rect(e_mode, pv_bits, ui4_pitch, 
                    i4_min_x + i4_x_diff, y + i4_y_diff, 
                    (i4_max_x - i4_min_x + 1), 1, ui4_color);
                i4_line_cnt++;
            }
        }
*/
        incr3 = (((x2 - x1) * xdirflag) > 0) ? 1 : (-1);
        while (y < yend)
        {
            i4_min_x = MAX(i4_left, F32_TO_INT(fcx << 1) - x);
            i4_max_x = MIN(i4_right, x);
            if (i4_min_x <= i4_max_x)
            {
                if (i4_top <= y && y <= i4_bottom)
                {
                    gl_dfb_locked_fill_rect(
                        e_mode, 
                        pv_bits, 
                        ui4_pitch, 
                        i4_min_x, 
                        y, 
                        (UINT32)(i4_max_x - i4_min_x + 1), 
                        1, 
                        ui4_color);
                    i4_line_cnt++;
                }

                i4_rel_y = F32_TO_INT(fcy << 1) - y;
                if (i4_top <= i4_rel_y && i4_rel_y <= i4_bottom)
                {
                    gl_dfb_locked_fill_rect(
                        e_mode, 
                        pv_bits, 
                        ui4_pitch, 
                        i4_min_x, 
                        i4_rel_y, 
                        (UINT32)(i4_max_x - i4_min_x + 1), 
                        1, 
                        ui4_color);
                    i4_line_cnt++;
                }
            }
            
            y++;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
	            x += incr3;
	            d += incr2;
	        }
        }
    }

    return (i4_line_cnt);
}

/*-----------------------------------------------------------------------------
 * Name: _fill_half_trapezoid
 *
 * Description: This API fills a symemtrical trapezoid.
 *
 * Inputs:  e_mode          Contains the color mode.
 *          pv_bits         Contains the address of the destination buffer.
 *          ui4_pitch       Contains the pitch of the destination buffer.
 *          i4_x_diff       Contains the x offset between source and destination
 *                          coordinate system.
 *          i4_y_diff       Contains the y offset between source and destination
 *                          coordinate system.
 *          cx              Contains the x coordinate of the center.
 *          x1              Contains the x coordinate of the first end of one 
 *                          side.
 *          y1              Contains the y coordinate of the first end of one
 *                          side.
 *          x2              Contains the x coordinate of the second end of one
 *                          side.
 *          y2              Contains the y coordinate of the second end of one
 *                          side.
 *          i4_left         Contains the minimum x value of the clipping rectangle
 *                          (in source coordinate).
 *          i4_right        Contains the maximum x value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_top          Contains the minimum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          i4_bottom       Contains the maximum y value of the clipping rectangle 
 *                          (in source coordinate).
 *          ui4_color       Contains the line color in the packed format.
 *
 * Outputs: -
 *
 * Returns: GLR_OK          Filling ellipse succeeded.
 ----------------------------------------------------------------------------*/
static INT32 _fill_half_trapezoid(
    GL_COLORMODE_T  e_mode, 
    VOID*           pv_bits, 
    UINT32          ui4_pitch, 
    INT32           cx, 
    INT32           x1, 
    INT32           y1, 
    INT32           x2, 
    INT32           y2, 
    INT32           i4_left, 
    INT32           i4_right, 
    INT32           i4_top, 
    INT32           i4_bottom,
    UINT32          ui4_color)
{
    INT32   dx, dy, incr1, incr2, incr3, d, x, y, xend, yend, xdirflag, ydirflag;
    INT32   i4_min_x, i4_max_x;
    INT32   i4_prv_x_min, i4_prv_x_max;
    INT32   i4_line_cnt = 0;
    
    dx = ABS (x2 - x1);
    dy = ABS (y2 - y1);
    
    if (dy <= dx)
    {
        d = 2 * dy - dx;
        incr1 = 2 * dy;
        incr2 = 2 * (dy - dx);
        if (x1 > x2)
	    {
	        x = x2;
	        y = y2;
	        ydirflag = (-1);
	        xend = x1;
	    }
        else
	    {
	        x = x1;
	        y = y1;
	        ydirflag = 1;
	        xend = x2;
	    }
        
        incr3 = (((y2 - y1) * ydirflag) > 0) ? 1 : (-1);

        i4_prv_x_min = i4_prv_x_max = x;
        
        while (x < xend)
        {
            i4_prv_x_max = x++;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
                if (cx < i4_prv_x_min)
                {
                    i4_min_x = MAX(i4_left, cx);
                    i4_max_x = MIN(i4_right, i4_prv_x_max);
                }
                else
                {
                    i4_min_x = MAX(i4_left, i4_prv_x_min);
                    i4_max_x = MIN(i4_right, cx);
                }
                
                if (i4_min_x <= i4_max_x)
                {
                    if (i4_top <= y && y <= i4_bottom)
                    {
                        gl_dfb_locked_fill_rect(
                            e_mode, 
                            pv_bits, 
                            ui4_pitch, 
                            i4_min_x, 
                            y, 
                            (UINT32)(i4_max_x - i4_min_x + 1), 
                            1, 
                            ui4_color);
                        i4_line_cnt++;
                    }
                }
                
                i4_prv_x_min = x;

	            y += incr3;
	            d += incr2;
	        }
        }

        if (i4_top <= y && y <= i4_bottom)
        {
            if (cx < i4_prv_x_min)
            {
                i4_min_x = MAX(i4_left, cx);
                i4_max_x = MIN(i4_right, i4_prv_x_max);
            }
            else
            {
                i4_min_x = MAX(i4_left, i4_prv_x_min);
                i4_max_x = MIN(i4_right, cx);
            }
            
            if (i4_min_x <= i4_max_x)
            {
                gl_dfb_locked_fill_rect(
                    e_mode, 
                    pv_bits, 
                    ui4_pitch, 
                    i4_min_x, 
                    y, 
                    (UINT32)(i4_max_x - i4_min_x + 1), 
                    1, 
                    ui4_color);
                i4_line_cnt++;
            }
        }
        
    }
    else
    {
        d = 2 * dx - dy;
        incr1 = 2 * dx;
        incr2 = 2 * (dx - dy);
        if (y1 > y2)
	    {
	        y = y2;
	        x = x2;
	        yend = MIN(y1, i4_bottom + 1);
	        xdirflag = (-1);
	    }
        else
	    {
	        y = y1;
            x = x1;
            yend = MIN(y2, i4_bottom + 1);
            xdirflag = 1;
	    }

        if (y > i4_bottom || yend < i4_top)
        {
            return (i4_line_cnt);
        }
/*        
        if (i4_top <= y)
        {
            i4_min_x = MAX(i4_left, F32_TO_INT(fcx << 1) - x);
            i4_max_x = MIN(i4_right, x);
            if (i4_min_x <= i4_max_x)
            {
                gl_dfb_locked_fill_rect(e_mode, pv_bits, ui4_pitch, 
                    i4_min_x + i4_x_diff, y + i4_y_diff, 
                    (i4_max_x - i4_min_x + 1), 1, ui4_color);
                i4_line_cnt++;
            }
        }
*/
        incr3 = (((x2 - x1) * xdirflag) > 0) ? 1 : (-1);
        if (cx < x)
        {
            i4_min_x = MAX(i4_left, cx);
            i4_max_x = MIN(i4_right, x);
        }
        else
        {
            i4_min_x = MAX(i4_left, x);
            i4_max_x = MIN(i4_right, cx);
        }
            
        while (y < yend)
        {
            if (i4_min_x <= i4_max_x)
            {
                if (i4_top <= y && y <= i4_bottom)
                {
                    gl_dfb_locked_fill_rect(
                        e_mode, 
                        pv_bits, 
                        ui4_pitch, 
                        i4_min_x, 
                        y, 
                        (UINT32)(i4_max_x - i4_min_x + 1), 
                        1, 
                        ui4_color);
                    i4_line_cnt++;
                }
            }
            
            y++;
            if (d < 0)
	        {
	            d += incr1;
	        }
            else
	        {
	            x += incr3;
	            d += incr2;
                
                if (cx < x)
                {
                    i4_min_x = MAX(i4_left, cx);
                    i4_max_x = MIN(i4_right, x);
                }
                else
                {
                    i4_min_x = MAX(i4_left, x);
                    i4_max_x = MIN(i4_right, cx);
                }
	        }
        }
    }

    return (i4_line_cnt);
}

static VOID _calc_vertex_array(
    const GL_RECT_T*    pt_area, 
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    UINT32              ui4_cnt,
    GL_POINT_T*         pt_vertex)
{
    INT32       i, j;
    INT32       w, h, w2, h2, wh;
    GL_FIX32_T  cx, cy;
    INT32       i4_rem;
    
    /* minus one to compensate for the central point */
    w = pt_area->i4_right - pt_area->i4_left - 1;
    h = pt_area->i4_bottom - pt_area->i4_top - 1;
    
    cx = INT_TO_F32(pt_area->i4_left) + (INT_TO_F32(w) >> 1);
    cy = INT_TO_F32(pt_area->i4_top) + (INT_TO_F32(h) >> 1);

    w2 = w * w;
    h2 = h * h;
    wh = w * h;

    for (i = i4_deg_start, j = 0; (i <= i4_deg_end); i++, j++)
    {
        i4_rem = (i % FULL_ANGLE);

        _get_arc_position(w2, h2, wh, i4_rem, &pt_vertex[j].i4_x, &pt_vertex[j].i4_y);

        pt_vertex[j].i4_x = F32_TO_INT(INT_TO_F32(pt_vertex[j].i4_x) + cx);
        pt_vertex[j].i4_y = F32_TO_INT(cy - INT_TO_F32(pt_vertex[j].i4_y));
    }
}

GL_FIX32_T g_fx_cos_tbl[] = {
    4096,
    4095,
    4093,
    4090,
    4086,
    4080,
    4073,
    4065,
    4056,
    4045,
    4033,
    4020,
    4006,
    3991,
    3974,
    3956,
    3937,
    3917,
    3895,
    3872,
    3848,
    3823,
    3797,
    3770,
    3741,
    3712,
    3681,
    3649,
    3616,
    3582,
    3547,
    3510,
    3473,
    3435,
    3395,
    3355,
    3313,
    3271,
    3227,
    3183,
    3137,
    3091,
    3043,
    2995,
    2946,
    2896,
    2845,
    2793,
    2740,
    2687,
    2632,
    2577,
    2521,
    2465,
    2407,
    2349,
    2290,
    2230,
    2170,
    2109,
    2048,
    1985,
    1922,
    1859,
    1795,
    1731,
    1665,
    1600,
    1534,
    1467,
    1400,
    1333,
    1265,
    1197,
    1129,
    1060,
    990,
    921,
    851,
    781,
    711,
    640,
    570,
    499,
    428,
    356,
    285,
    214,
    142,
    71,
    0,
    -71,
    -142,
    -214,
    -285,
    -356,
    -428,
    -499,
    -570,
    -640,
    -711,
    -781,
    -851,
    -921,
    -990,
    -1060,
    -1129,
    -1197,
    -1265,
    -1333,
    -1400,
    -1467,
    -1534,
    -1600,
    -1665,
    -1731,
    -1795,
    -1859,
    -1922,
    -1985,
    -2047,
    -2109,
    -2170,
    -2230,
    -2290,
    -2349,
    -2407,
    -2465,
    -2521,
    -2577,
    -2632,
    -2687,
    -2740,
    -2793,
    -2845,
    -2896,
    -2946,
    -2995,
    -3043,
    -3091,
    -3137,
    -3183,
    -3227,
    -3271,
    -3313,
    -3355,
    -3395,
    -3435,
    -3473,
    -3510,
    -3547,
    -3582,
    -3616,
    -3649,
    -3681,
    -3712,
    -3741,
    -3770,
    -3797,
    -3823,
    -3848,
    -3872,
    -3895,
    -3917,
    -3937,
    -3956,
    -3974,
    -3991,
    -4006,
    -4020,
    -4033,
    -4045,
    -4056,
    -4065,
    -4073,
    -4080,
    -4086,
    -4090,
    -4093,
    -4095,
    -4096,
    -4095,
    -4093,
    -4090,
    -4086,
    -4080,
    -4073,
    -4065,
    -4056,
    -4045,
    -4033,
    -4020,
    -4006,
    -3991,
    -3974,
    -3956,
    -3937,
    -3917,
    -3895,
    -3872,
    -3848,
    -3823,
    -3797,
    -3770,
    -3741,
    -3712,
    -3681,
    -3649,
    -3616,
    -3582,
    -3547,
    -3510,
    -3473,
    -3435,
    -3395,
    -3355,
    -3313,
    -3271,
    -3227,
    -3183,
    -3137,
    -3091,
    -3043,
    -2995,
    -2946,
    -2896,
    -2845,
    -2793,
    -2740,
    -2687,
    -2632,
    -2577,
    -2521,
    -2465,
    -2407,
    -2349,
    -2290,
    -2230,
    -2170,
    -2109,
    -2048,
    -1985,
    -1922,
    -1859,
    -1795,
    -1731,
    -1665,
    -1600,
    -1534,
    -1467,
    -1400,
    -1333,
    -1265,
    -1197,
    -1129,
    -1060,
    -990,
    -921,
    -851,
    -781,
    -711,
    -640,
    -570,
    -499,
    -428,
    -356,
    -285,
    -214,
    -142,
    -71,
    0,
    71,
    142,
    214,
    285,
    356,
    428,
    499,
    570,
    640,
    711,
    781,
    851,
    921,
    990,
    1060,
    1129,
    1197,
    1265,
    1333,
    1400,
    1467,
    1534,
    1600,
    1665,
    1731,
    1795,
    1859,
    1922,
    1985,
    2047,
    2109,
    2170,
    2230,
    2290,
    2349,
    2407,
    2465,
    2521,
    2577,
    2632,
    2687,
    2740,
    2793,
    2845,
    2896,
    2946,
    2995,
    3043,
    3091,
    3137,
    3183,
    3227,
    3271,
    3313,
    3355,
    3395,
    3435,
    3473,
    3510,
    3547,
    3582,
    3616,
    3649,
    3681,
    3712,
    3741,
    3770,
    3797,
    3823,
    3848,
    3872,
    3895,
    3917,
    3937,
    3956,
    3974,
    3991,
    4006,
    4020,
    4033,
    4045,
    4056,
    4065,
    4073,
    4080,
    4086,
    4090,
    4093,
    4095,
    4096
};
    
GL_FIX32_T g_fx_sin_tbl[] = {
    0,
    71,
    142,
    214,
    285,
    356,
    428,
    499,
    570,
    640,
    711,
    781,
    851,
    921,
    990,
    1060,
    1129,
    1197,
    1265,
    1333,
    1400,
    1467,
    1534,
    1600,
    1665,
    1731,
    1795,
    1859,
    1922,
    1985,
    2047,
    2109,
    2170,
    2230,
    2290,
    2349,
    2407,
    2465,
    2521,
    2577,
    2632,
    2687,
    2740,
    2793,
    2845,
    2896,
    2946,
    2995,
    3043,
    3091,
    3137,
    3183,
    3227,
    3271,
    3313,
    3355,
    3395,
    3435,
    3473,
    3510,
    3547,
    3582,
    3616,
    3649,
    3681,
    3712,
    3741,
    3770,
    3797,
    3823,
    3848,
    3872,
    3895,
    3917,
    3937,
    3956,
    3974,
    3991,
    4006,
    4020,
    4033,
    4045,
    4056,
    4065,
    4073,
    4080,
    4086,
    4090,
    4093,
    4095,
    4096,
    4095,
    4093,
    4090,
    4086,
    4080,
    4073,
    4065,
    4056,
    4045,
    4033,
    4020,
    4006,
    3991,
    3974,
    3956,
    3937,
    3917,
    3895,
    3872,
    3848,
    3823,
    3797,
    3770,
    3741,
    3712,
    3681,
    3649,
    3616,
    3582,
    3547,
    3510,
    3473,
    3435,
    3395,
    3355,
    3313,
    3271,
    3227,
    3183,
    3137,
    3091,
    3043,
    2995,
    2946,
    2896,
    2845,
    2793,
    2740,
    2687,
    2632,
    2577,
    2521,
    2465,
    2407,
    2349,
    2290,
    2230,
    2170,
    2109,
    2048,
    1985,
    1922,
    1859,
    1795,
    1731,
    1665,
    1600,
    1534,
    1467,
    1400,
    1333,
    1265,
    1197,
    1129,
    1060,
    990,
    921,
    851,
    781,
    711,
    640,
    570,
    499,
    428,
    356,
    285,
    214,
    142,
    71,
    0,
    -71,
    -142,
    -214,
    -285,
    -356,
    -428,
    -499,
    -570,
    -640,
    -711,
    -781,
    -851,
    -921,
    -990,
    -1060,
    -1129,
    -1197,
    -1265,
    -1333,
    -1400,
    -1467,
    -1534,
    -1600,
    -1665,
    -1731,
    -1795,
    -1859,
    -1922,
    -1985,
    -2047,
    -2109,
    -2170,
    -2230,
    -2290,
    -2349,
    -2407,
    -2465,
    -2521,
    -2577,
    -2632,
    -2687,
    -2740,
    -2793,
    -2845,
    -2896,
    -2946,
    -2995,
    -3043,
    -3091,
    -3137,
    -3183,
    -3227,
    -3271,
    -3313,
    -3355,
    -3395,
    -3435,
    -3473,
    -3510,
    -3547,
    -3582,
    -3616,
    -3649,
    -3681,
    -3712,
    -3741,
    -3770,
    -3797,
    -3823,
    -3848,
    -3872,
    -3895,
    -3917,
    -3937,
    -3956,
    -3974,
    -3991,
    -4006,
    -4020,
    -4033,
    -4045,
    -4056,
    -4065,
    -4073,
    -4080,
    -4086,
    -4090,
    -4093,
    -4095,
    -4096,
    -4095,
    -4093,
    -4090,
    -4086,
    -4080,
    -4073,
    -4065,
    -4056,
    -4045,
    -4033,
    -4020,
    -4006,
    -3991,
    -3974,
    -3956,
    -3937,
    -3917,
    -3895,
    -3872,
    -3848,
    -3823,
    -3797,
    -3770,
    -3741,
    -3712,
    -3681,
    -3649,
    -3616,
    -3582,
    -3547,
    -3510,
    -3473,
    -3435,
    -3395,
    -3355,
    -3313,
    -3271,
    -3227,
    -3183,
    -3137,
    -3091,
    -3043,
    -2995,
    -2946,
    -2896,
    -2845,
    -2793,
    -2740,
    -2687,
    -2632,
    -2577,
    -2521,
    -2465,
    -2407,
    -2349,
    -2290,
    -2230,
    -2170,
    -2109,
    -2048,
    -1985,
    -1922,
    -1859,
    -1795,
    -1731,
    -1665,
    -1600,
    -1534,
    -1467,
    -1400,
    -1333,
    -1265,
    -1197,
    -1129,
    -1060,
    -990,
    -921,
    -851,
    -781,
    -711,
    -640,
    -570,
    -499,
    -428,
    -356,
    -285,
    -214,
    -142,
    -71,
    0
};


GL_FIX32_T g_fx_cot2_tbl[] = {
    -2147483648,
    13443644,
    3358863,
    1491311,
    837669,
    535126,
    370782,
    271689,
    207374,
    163280,
    131741,
    108406,
    90658,
    76847,
    65889,
    57049,
    49815,
    43820,
    38797,
    34547,
    30919,
    27797,
    25092,
    22732,
    20663,
    18837,
    17218,
    15777,
    14488,
    13330,
    12288,
    11345,
    10490,
    9712,
    9002,
    8354,
    7759,
    7213,
    6710,
    6246,
    5817,
    5420,
    5052,
    4710,
    4392,
    4096,
    3819,
    3561,
    3320,
    3095,
    2883,
    2685,
    2500,
    2325,
    2162,
    2008,
    1863,
    1727,
    1599,
    1478,
    1365,
    1258,
    1158,
    1063,
    974,
    890,
    811,
    738,
    668,
    603,
    542,
    485,
    432,
    382,
    336,
    294,
    254,
    218,
    185,
    154,
    127,
    102,
    80,
    61,
    45,
    31,
    20,
    11,
    4,
    1,
    0,
    1,
    4,
    11,
    20,
    31,
    45,
    61,
    80,
    102,
    127,
    154,
    185,
    218,
    254,
    294,
    336,
    382,
    432,
    485,
    542,
    603,
    668,
    738,
    811,
    890,
    974,
    1063,
    1158,
    1258,
    1365,
    1478,
    1599,
    1727,
    1863,
    2008,
    2162,
    2325,
    2500,
    2685,
    2883,
    3095,
    3320,
    3561,
    3819,
    4095,
    4392,
    4710,
    5052,
    5420,
    5817,
    6246,
    6710,
    7213,
    7759,
    8354,
    9002,
    9712,
    10490,
    11345,
    12288,
    13330,
    14488,
    15777,
    17218,
    18837,
    20663,
    22732,
    25092,
    27797,
    30919,
    34547,
    38797,
    43820,
    49815,
    57049,
    65889,
    76847,
    90658,
    108406,
    131741,
    163280,
    207374,
    271689,
    370782,
    535126,
    837669,
    1491311,
    3358863,
    13443644,
    -2147483648,
    13443644,
    3358863,
    1491311,
    837669,
    535126,
    370782,
    271689,
    207374,
    163280,
    131741,
    108406,
    90658,
    76847,
    65889,
    57049,
    49815,
    43820,
    38797,
    34547,
    30919,
    27797,
    25092,
    22732,
    20663,
    18837,
    17218,
    15777,
    14488,
    13330,
    12287,
    11345,
    10490,
    9712,
    9002,
    8354,
    7759,
    7213,
    6710,
    6246,
    5817,
    5420,
    5052,
    4710,
    4392,
    4096,
    3819,
    3561,
    3320,
    3095,
    2883,
    2685,
    2500,
    2325,
    2162,
    2008,
    1863,
    1727,
    1599,
    1478,
    1365,
    1258,
    1158,
    1063,
    974,
    890,
    811,
    738,
    668,
    603,
    542,
    485,
    432,
    382,
    336,
    294,
    254,
    218,
    185,
    154,
    127,
    102,
    80,
    61,
    45,
    31,
    20,
    11,
    4,
    1,
    0,
    1,
    4,
    11,
    20,
    31,
    45,
    61,
    80,
    102,
    127,
    154,
    185,
    218,
    254,
    294,
    336,
    382,
    432,
    485,
    542,
    603,
    668,
    738,
    811,
    890,
    974,
    1063,
    1158,
    1258,
    1365,
    1478,
    1599,
    1727,
    1863,
    2008,
    2162,
    2325,
    2500,
    2685,
    2883,
    3095,
    3320,
    3561,
    3819,
    4095,
    4392,
    4710,
    5052,
    5420,
    5817,
    6246,
    6710,
    7213,
    7759,
    8354,
    9002,
    9712,
    10490,
    11345,
    12287,
    13330,
    14488,
    15777,
    17218,
    18837,
    20663,
    22732,
    25092,
    27797,
    30919,
    34547,
    38797,
    43820,
    49815,
    57049,
    65889,
    76847,
    90658,
    108406,
    131741,
    163280,
    207374,
    271689,
    370782,
    535126,
    837669,
    1491311,
    3358863,
    13443644
};

GL_FIX32_T g_fx_tan2_tbl[] = {
    0,
    1,
    4,
    11,
    20,
    31,
    45,
    61,
    80,
    102,
    127,
    154,
    185,
    218,
    254,
    294,
    336,
    382,
    432,
    485,
    542,
    603,
    668,
    738,
    811,
    890,
    974,
    1063,
    1158,
    1258,
    1365,
    1478,
    1599,
    1727,
    1863,
    2008,
    2162,
    2325,
    2500,
    2685,
    2883,
    3095,
    3320,
    3561,
    3819,
    4095,
    4392,
    4710,
    5052,
    5420,
    5817,
    6246,
    6710,
    7213,
    7759,
    8354,
    9002,
    9712,
    10490,
    11345,
    12287,
    13330,
    14488,
    15777,
    17218,
    18837,
    20663,
    22732,
    25092,
    27797,
    30919,
    34547,
    38797,
    43820,
    49815,
    57049,
    65889,
    76847,
    90658,
    108406,
    131741,
    163280,
    207374,
    271689,
    370782,
    535126,
    837669,
    1491311,
    3358863,
    13443644,
    -2147483648,
    13443644,
    3358863,
    1491311,
    837669,
    535126,
    370782,
    271689,
    207374,
    163280,
    131741,
    108406,
    90658,
    76847,
    65889,
    57049,
    49815,
    43820,
    38797,
    34547,
    30919,
    27797,
    25092,
    22732,
    20663,
    18837,
    17218,
    15777,
    14488,
    13330,
    12288,
    11345,
    10490,
    9712,
    9002,
    8354,
    7759,
    7213,
    6710,
    6246,
    5817,
    5420,
    5052,
    4710,
    4392,
    4096,
    3819,
    3561,
    3320,
    3095,
    2883,
    2685,
    2500,
    2325,
    2162,
    2008,
    1863,
    1727,
    1599,
    1478,
    1365,
    1258,
    1158,
    1063,
    974,
    890,
    811,
    738,
    668,
    603,
    542,
    485,
    432,
    382,
    336,
    294,
    254,
    218,
    185,
    154,
    127,
    102,
    80,
    61,
    45,
    31,
    20,
    11,
    4,
    1,
    0,
    1,
    4,
    11,
    20,
    31,
    45,
    61,
    80,
    102,
    127,
    154,
    185,
    218,
    254,
    294,
    336,
    382,
    432,
    485,
    542,
    603,
    668,
    738,
    811,
    890,
    974,
    1063,
    1158,
    1258,
    1365,
    1478,
    1599,
    1727,
    1863,
    2008,
    2162,
    2325,
    2500,
    2685,
    2883,
    3095,
    3320,
    3561,
    3819,
    4095,
    4392,
    4710,
    5052,
    5420,
    5817,
    6246,
    6710,
    7213,
    7759,
    8354,
    9002,
    9712,
    10490,
    11345,
    12287,
    13330,
    14488,
    15777,
    17218,
    18837,
    20663,
    22732,
    25092,
    27797,
    30919,
    34547,
    38797,
    43820,
    49815,
    57049,
    65889,
    76847,
    90658,
    108406,
    131741,
    163280,
    207374,
    271689,
    370782,
    535126,
    837669,
    1491311,
    3358863,
    13443644,
    -2147483648,
    13443644,
    3358863,
    1491311,
    837669,
    535126,
    370782,
    271689,
    207374,
    163280,
    131741,
    108406,
    90658,
    76847,
    65889,
    57049,
    49815,
    43820,
    38797,
    34547,
    30919,
    27797,
    25092,
    22732,
    20663,
    18837,
    17218,
    15777,
    14488,
    13330,
    12287,
    11345,
    10490,
    9712,
    9002,
    8354,
    7759,
    7213,
    6710,
    6246,
    5817,
    5420,
    5052,
    4710,
    4392,
    4096,
    3819,
    3561,
    3320,
    3095,
    2883,
    2685,
    2500,
    2325,
    2162,
    2008,
    1863,
    1727,
    1599,
    1478,
    1365,
    1258,
    1158,
    1063,
    974,
    890,
    811,
    738,
    668,
    603,
    542,
    485,
    432,
    382,
    336,
    294,
    254,
    218,
    185,
    154,
    127,
    102,
    80,
    61,
    45,
    31,
    20,
    11,
    4,
    1
};

static VOID _get_arc_position(
    INT32               i4_w2,
    INT32               i4_h2,
    INT32               i4_wh,
    INT32               i4_angle,   /* i4_angle lies in [0, 359] */
    INT32               *i4_x,
    INT32               *i4_y)
{
    INT32   i4_w, i4_h;
    UINT32  ui4_deno;
    UINT64  ui8_sum;
    
    if (i4_angle == 0 || i4_angle == 90 || i4_angle == 180 || i4_angle == 270)
    {
        i4_w = (INT32)_i8_sqrt((UINT64)i4_w2);
        i4_h = (INT32)_i8_sqrt((UINT64)i4_h2);
        
        *i4_x = F32_TO_INT((g_fx_cos_tbl[i4_angle] * i4_w) / 2);
        *i4_y = F32_TO_INT((g_fx_sin_tbl[i4_angle] * i4_h) / 2);

        return;
    }

#if 0
    ui8_sum  = (UINT64)INT_TO_F32(i4_h2) + (i4_w2 * (UINT64)g_fx_tan2_tbl[i4_angle]);
    ui4_deno = _i8_sqrt(ui8_sum);

    *i4_x = (((UINT64)i4_wh) << (GL_F32_FRAC / 2)) / ((UINT64)ui4_deno * 2);
#else
    ui8_sum  = (UINT64)INT_TO_HPF(i4_h2) + F32_TO_HPF(i4_w2 * (UINT64)g_fx_tan2_tbl[i4_angle]);
    ui4_deno = _i8_sqrt(ui8_sum);

    *i4_x = (INT32)((((UINT64)i4_wh) << (F32_HALF_FRAC / 2)) / ((UINT64)ui4_deno * 2));
#endif

    if (i4_angle > 90 && i4_angle < 270)
    {
        *i4_x = -*i4_x;
    }

#if 0
    ui8_sum  = (i4_h2 * (UINT64)g_fx_cot2_tbl[i4_angle]) + (UINT64)INT_TO_F32(i4_w2);
    ui4_deno = _i8_sqrt(ui8_sum);
    
    *i4_y = (((UINT64)i4_wh) << (GL_F32_FRAC / 2)) / ((UINT64)ui4_deno * 2);
#else
    ui8_sum  = F32_TO_HPF(i4_h2 * (UINT64)g_fx_cot2_tbl[i4_angle]) + (UINT64)INT_TO_HPF(i4_w2);
    ui4_deno = _i8_sqrt(ui8_sum);
    
    *i4_y = (INT32)((((UINT64)i4_wh) << (F32_HALF_FRAC / 2)) / ((UINT64)ui4_deno * 2));
#endif

    if (i4_angle > 180)
    {
        *i4_y = -*i4_y;
    }
}



/*
 * Wilco Dijkstra's method
 * it produces optimized ARM code which takes 4 cycles per bit
 *
 * output 32-bit square root
 *
 */
#define ITER1(N)                                \
{                                               \
    ui8_try = ui8_root + ((UINT64)1 << (N));    \
                                                \
    if (ui8_x >= ui8_try << (N))                \
    {                                           \
        ui8_x -= ui8_try << (N);                \
        ui8_root |= (UINT64)2 << (N);           \
    }                                           \
}

static UINT32 _i8_sqrt(UINT64 ui8_x)
{
    UINT64  ui8_root, ui8_try;

    ui8_root = 0;

    ITER1(31);  ITER1(30);  ITER1(29);  ITER1(28);
    ITER1(27);  ITER1(26);  ITER1(25);  ITER1(24);
    ITER1(23);  ITER1(22);  ITER1(21);  ITER1(20);
    ITER1(19);  ITER1(18);  ITER1(17);  ITER1(16);
    ITER1(15);  ITER1(14);  ITER1(13);  ITER1(12);
    ITER1(11);  ITER1(10);  ITER1( 9);  ITER1( 8);
    ITER1( 7);  ITER1( 6);  ITER1( 5);  ITER1( 4);
    ITER1( 3);  ITER1( 2);  ITER1( 1);  ITER1( 0);

    return (UINT32)(ui8_root >> 1);
}

