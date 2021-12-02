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

#if !defined (GL_NO_MW_FONT)



#include "inc/x_common.h"

#include "u_gl.h"
#include "x_gl.h"
#include "gl.h"
#include "gl_dfb.h"
#include "handle/u_handle.h"
#include "stdio.h"

#include "gl_dfb_sw_draw.h"
#include "gl_dfb_surf.h"
#if 0
#include <directfb.h>
#endif
#include "font/x_fe.h"
#include "ft2build.h"
#include "freetype/ftglyph.h"
#include "ft_bitmap.h"
#include "unicode/x_uc_str.h"
#include "dbg/x_dbg.h"

#if 0
#define INTL_BUF_LEN                ((UINT32) (1280 * 4 * 8))
#else
#define INTL_BUF_LEN                ((UINT32) (1024 * 2 * 8))
#endif
#define PITCH_UNIT                  ((UINT32) 256)

#define PRINT_LINE_DBG /**/
/*printf("^^^^^^^^^^^^%s in %s, #%d\n", __FUNCTION__, __FILE__, __LINE__);*/


#define __LOG   x_dbg_stmt("%s[%d] ",__FILE__,__LINE__);x_dbg_stmt

#define CHECK_EXP_RET(x,info,ret)              \
    do {                                        \
        if ( (x) ){                             \
            __LOG (info);                       \
            return ret;                         \
        }                                       \
    }while(0)
        

typedef struct _FONT_ENG
{
    FT_Library    library;
    FT_Face       face;
}FONT_ENG;

FONT_ENG            t_g_gl_font_eng;
static UINT16       ui2_g_gl_text_max_width;
static UINT16       ui2_g_gl_text_max_height;
static HANDLE_T     h_g_gl_isdb_sema_hdl;



#define MAX_BUF_HEIGHT 32
#define MAX_BUF_WIDTH  1920

static const UINT8  _s_ui1_pixel_bits[] = {
    2, 4, 8, 16, 16, 32, 2, 4, 8, 16, 16, 16, 32 };
static UINT8 _shf_bit(GL_COLORMODE_T e_mode)
{
    static const UINT8 s_aui1_shf_bit[] = {
        0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 2 };

    return (s_aui1_shf_bit[e_mode]);
}


#define             INTL_SURF_LEN       (INTL_BUF_LEN + 2 * PITCH_UNIT / 8)
static UINT8        *_aui1_buf = NULL;
/*For DFB there is difficult to build a same size surface with bliting with different pitch like doing that via MW RM*/
static UINT32        _ui4_buf_pitch;
static UINT32        _ui4_buf_width  = MAX_BUF_WIDTH;
static UINT32        _ui4_buf_height = MAX_BUF_HEIGHT;
static GL_COLORMODE_T  _e_color_mode = GL_COLORMODE_ARGB_D4444;


typedef enum
{
    GL_STR_LEFT_2_RIGHT,
    GL_STR_TOP_2_BOTTOM
} GL_STR_DIR_T;

typedef struct{ 
    INT32       i4_begin;
    INT32       i4_width;
    INT32       i4_adv;
    }
_CHAR_POS_INFO;

typedef struct
{
    UINT16              ui2_display_idx_num;
    UINT16              *aui2_display_idx;
    UINT16              *aui2_orient_idx;
    UINT16              ui2_orient_idx_num;
    GL_CHAR_ORIENT_T    e_char_orientation;
    GL_STR_DIR_T        e_str_direction;
    VOID                *pv_skip_line;
    UINT16              ui2_skip_line_len;
} GL_STR_AUX_T;



static GL_HSURFACE_T    _h_intl_surf = NULL_HANDLE;

static _CHAR_POS_INFO   *_pt_skip_char_info = NULL;
static INT32            _i4_skip_char_count = 0;

#define CHECK_THAI_PRESENTATION(t_ch_prv, t_ch)                             \
 (((((t_ch_prv) >= 0x0E01 && (t_ch_prv) <= 0x0E2E)  ||  /* Consonants */    \
    ((t_ch_prv) >= 0x0E38 && (t_ch_prv) <= 0x0E3A)) &&  /* lower Vowels */  \
   (((t_ch) >= 0x0E48 && (t_ch) <= 0x0E4B)          ||  /* Tone markks */   \
    ((t_ch) >= 0x0E4C && (t_ch) <= 0x0E4E))))           /* Signs */         \

#define CHECK_THAI_PRESENTATION_NEED_LEFT_SHIFT(t_ch_prv, t_ch)          \
    (((t_ch_prv == 0x0E1B) || (t_ch_prv == 0x0E1D) ||(t_ch_prv == 0x0E1F) ) && \
    (((t_ch >= 0x0E34) && (t_ch <= 0x0E37)) || (t_ch == 0x0E47) || (t_ch == 0x0E31))  )
    
#define CHECK_THAI_PRESENTATION_NEED_TOP_SHIFT(t_ch)           \
    ((t_ch == 0x0E31) || (t_ch == 0x0E31) || (t_ch == 0x0E47) \
    || ((t_ch >= 0x0E34) && (t_ch <= 0x0E37) ) || (t_ch == 0x0E4D) )

#define CHECK_THAI_PRESENTATION_NEED_TOPPEST_SHIFT(t_ch)           \
    ((t_ch == 0x0E49) || (t_ch == 0x0E4A) )


static VOID _check_n_swap(GL_RECT_T* pt_rect);

static VOID _fill_intl_buf(
    VOID*   pv_buf,
    UINT32  ui4_pitch,
    INT32   i4_left,
    INT32   i4_top,
    INT32   i4_w,
    INT32   i4_h,
    UINT8   ui1_c)
{
    UINT8   *pui1_buf_top, *pui1_buf_bottom;

    pui1_buf_top = (UINT8 *)pv_buf + (INT32)ui4_pitch * i4_top + i4_left;
    pui1_buf_bottom = pui1_buf_top + (INT32)ui4_pitch * (i4_h - 1);

    while (pui1_buf_top <= pui1_buf_bottom)
    {
        x_memset(pui1_buf_top, ui1_c, (SIZE_T)i4_w *(_s_ui1_pixel_bits[_e_color_mode]/8));
        x_memset(pui1_buf_bottom, ui1_c, (SIZE_T)i4_w*(_s_ui1_pixel_bits[_e_color_mode]/8));

        pui1_buf_top += ui4_pitch;
        pui1_buf_bottom -= ui4_pitch;
    }
}


static BOOL _check_display_idx_inside(UINT16 ui2_index, UINT16 *aui2_array, UINT16 ui2_num)
{
    UINT16  ui2_i;
    
    for (ui2_i = 0; ui2_i < ui2_num; ui2_i++)
    {
        if (aui2_array[ui2_i] == ui2_index)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

VOID x_gl_surface_get_intl_surface_buf(GL_COLORMODE_T e_mode, UINT8 **ppui1_buf)
{
    gl_dfb_global_lock();

    if (_h_intl_surf != NULL_HANDLE)
    {
        x_gl_obj_free(_h_intl_surf);
        _h_intl_surf = NULL_HANDLE;
    }
    
    if (_h_intl_surf == NULL_HANDLE)
    {
        INT32               i4_ret;
        GL_SURFACE_DESC_T   t_desc;
        
        GL_LOCK_INFO_T      t_lock_info;

        t_desc.e_clrmode  = e_mode;
        t_desc.h_clipper  = NULL_HANDLE;
        t_desc.h_palette  = NULL_HANDLE;
        t_desc.ui4_width  = _ui4_buf_width;
        t_desc.ui4_height = _ui4_buf_height;
        

        i4_ret = x_gl_surface_create(&t_desc, &_h_intl_surf);
        
        if (i4_ret == GLR_OK)
        {
            _e_color_mode = e_mode;
            i4_ret = gl_dfb_surface_lock(_h_intl_surf, NULL, GL_SYNC, &t_lock_info);
            PRINT_LINE_DBG;

            if (i4_ret == GLR_OK)
            {
                *ppui1_buf = (UINT8 *)(t_lock_info.at_subplane[0].pv_bits);
                _ui4_buf_pitch = t_lock_info.at_subplane[0].ui4_pitch;          
            }
            else
            {
                *ppui1_buf = NULL;
                
                GL_DFB_TRACE("x_gl_surface_get_intl_surface_buf() failed.\n");
            }
        }
        else
        {
            *ppui1_buf = NULL;
            
            GL_DFB_TRACE("x_gl_surface_get_intl_surface_buf() failed.\n");
        }
    }

    gl_dfb_global_unlock();
}

VOID gl_cal_buf_param_by_height(
    GL_COLORMODE_T  e_mode, 
    UINT32          ui4_height, 
    VOID**          ppv_buf, 
    UINT32*         pui4_pitch, 
    UINT32*         pui4_width)
{
    if (_aui1_buf == NULL)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, *pui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, ui4_height);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }
    else if (_e_color_mode != e_mode)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, *pui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, ui4_height);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }
    else if (_ui4_buf_height < ui4_height || _ui4_buf_width < *pui4_width)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, *pui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, ui4_height);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }

    *pui4_pitch = _ui4_buf_pitch;
    *pui4_width = _ui4_buf_width;

    *ppv_buf = (VOID *)_aui1_buf;
}

VOID gl_cal_buf_param_by_width(
    GL_COLORMODE_T  e_mode, 
    UINT32          ui4_width, 
    VOID**          ppv_buf, 
    UINT32*         pui4_pitch, 
    UINT32*         pui4_height)
{
    if (_aui1_buf == NULL)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, ui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, *pui4_pitch);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }
    else if (_e_color_mode != e_mode)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, ui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, *pui4_pitch);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }
    else if (_ui4_buf_height < *pui4_pitch || _ui4_buf_width < ui4_width)
    {
        _ui4_buf_width = MAX(_ui4_buf_width, ui4_width);
        _ui4_buf_height = MAX(_ui4_buf_height, *pui4_pitch);
        x_gl_surface_get_intl_surface_buf(e_mode, &_aui1_buf);
    }
    
    *pui4_pitch = _ui4_buf_pitch;
    *ppv_buf = (VOID *)_aui1_buf;
    *pui4_height = _ui4_buf_height;
}

UINT8 gl_bit_count(GL_COLORMODE_T e_mode);


/* blend the foreground and edge color */
#define _BLEND_CLR(fg_clr, fg_alpha, edge_clr, edge_alpha) \
    (((fg_clr * fg_alpha) + (edge_clr * edge_alpha)) / /*0x100*/(fg_alpha + edge_alpha))
    /* we should use (fg_alpha + edge_alpha) as the denominator,
       but it costs too much performance */
       

static VOID _draw_char(
    VOID*               pv_dst,
    const GL_RECT_T*    pt_rc_dst,
    UINT32              ui4_dst_pitch,
    GL_COLORMODE_T      e_dst_mode,
    VOID*               pv_src,
    const GL_RECT_T*    pt_rc_src,
    UINT32              ui4_src_pitch,
    FE_PIXEL_MODE_T     e_src_mode,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    GL_DRAW_STR_MODE_T  e_draw_str_mode)
{
    #define THRESHOLD   127
    
    UINT32  x, y, w, h;
    UINT8*  pui1_src;
    UINT8   ui1_alpha;

    pui1_src = (UINT8*) pv_src + pt_rc_src->i4_top * ui4_src_pitch + pt_rc_src->i4_left;
        
    pv_dst = ((UINT8*) pv_dst + 
               pt_rc_dst->i4_top * ui4_dst_pitch + 
               (pt_rc_dst->i4_left * gl_bit_count(e_dst_mode) >> 3));
    
    w = RECT_W(pt_rc_src);
    h = RECT_H(pt_rc_src);
    
    if (e_src_mode == FE_PIXEL_MODE_GRAY)
    {
        
        switch (e_dst_mode)
        {
            case GFX_COLORMODE_ARGB_CLUT8:
            case GFX_COLORMODE_AYCbCr_CLUT8:
            {
                UINT8* pui1_dst   = (UINT8*) pv_dst;
                UINT8  ui1_fg_idx = pt_fg_clr->u3.index;
                UINT8  ui1_bg_idx = ((pt_bg_clr) ? pt_bg_clr->u3.index : 0);
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        ui1_alpha = pui1_src[x];
                        
                        if (pui1_src[x] > THRESHOLD)
                        {
                            pui1_dst[x] = ui1_fg_idx;
                        }
                        else if (pt_bg_clr)
                        {
                            pui1_dst[x] = ui1_bg_idx;
                        }
                    }

                    pui1_dst += ui4_dst_pitch;
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
                
            case GFX_COLORMODE_RGB_D565:
            {
                UINT16* pui2_dst   = (UINT16*) pv_dst;
                UINT16  ui2_fg_clr = (UINT16) GL_COLOR_RGB565(pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                UINT16  ui2_bg_clr = 
                    ((pt_bg_clr) ? (UINT16) GL_COLOR_RGB565(pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b) : 0);
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        ui1_alpha = pui1_src[x];

                        if (ui1_alpha > THRESHOLD)
                        {
                            pui2_dst[x] = ui2_fg_clr;
                        }
                        else if (pt_bg_clr)
                        {
                            pui2_dst[x] = ui2_bg_clr;
                        }
                    }

                    pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
                
            case GFX_COLORMODE_ARGB_D1555:
            {
                UINT16* pui2_dst = (UINT16*) pv_dst;
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        ui1_alpha = pui1_src[x];

                        if (ui1_alpha > THRESHOLD)
                        {
                            pui2_dst[x] = (UINT16) GL_COLOR_ARGB1555(
                                ui1_alpha, pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                        }
                        else if (pt_bg_clr)
                        {
                            pui2_dst[x] = (UINT16) GL_COLOR_ARGB1555(
                                pt_bg_clr->a, pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b);
                        }
                    }

                    pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
                
            case GFX_COLORMODE_ARGB_D4444:
            {
                PRINT_LINE_DBG;
                UINT16* pui2_dst;
                UINT8*  pui1_edge;
                pui2_dst = (UINT16*) pv_dst;
                pui1_edge = pui1_src + h * w;
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {

                        ui1_alpha = pui1_src[x];

                        if (ui1_alpha > 0)
                        {
                            if (e_draw_str_mode == GL_DRAW_STR_BITBLT)
                            {
                                ui1_alpha = pt_fg_clr->a;
                            }
                            
                            if (pt_edge_clr && pui1_edge[x] > 0)
                            {
                                pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(
                                                ui1_alpha | pui1_edge[x],
                                                _BLEND_CLR(pt_fg_clr->u1.r, ui1_alpha, pt_edge_clr->u1.r, pui1_edge[x]),
                                                _BLEND_CLR(pt_fg_clr->u2.g, ui1_alpha, pt_edge_clr->u2.g, pui1_edge[x]),
                                                _BLEND_CLR(pt_fg_clr->u3.b, ui1_alpha, pt_edge_clr->u3.b, pui1_edge[x]));
                            }
                            else
                            {
                                pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(
                                        ui1_alpha, pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                            }
                        }
                        else if (pt_edge_clr && (pui1_edge[x] > 0))
                        {
                            pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(
                                    pui1_edge[x], pt_edge_clr->u1.r, pt_edge_clr->u2.g, pt_edge_clr->u3.b);
                        }
                        else if (pt_bg_clr)
                        {
                            pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(
                                    pt_bg_clr->a, pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b);
                        }
                        
                    }

                    pui2_dst   = (UINT16*) ((UINT8*) pui2_dst + ui4_dst_pitch);
                    pui1_src  += ui4_src_pitch;
                    pui1_edge += ui4_src_pitch;
                }
                
                break;
            }
                
            case GFX_COLORMODE_AYCbCr_D8888:
            case GFX_COLORMODE_ARGB_D8888:
            {
                UINT32* pui4_dst = (UINT32*) pv_dst;
                UINT8 * pui1_edge = pui1_src + h * w;
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        ui1_alpha = pui1_src[x];

                        if (ui1_alpha > 0)
                        {
							if (e_draw_str_mode == GL_DRAW_STR_BITBLT)
                            {
                                ui1_alpha = pt_fg_clr->a;
                            }
                            if (pt_edge_clr && pui1_edge[x] > 0)
                            {
                                pui4_dst[x] = GL_COLOR_ARGB8888(
                                                ui1_alpha | pui1_edge[x],
                                                _BLEND_CLR(pt_fg_clr->u1.r, ui1_alpha, pt_edge_clr->u1.r, pui1_edge[x]),
                                                _BLEND_CLR(pt_fg_clr->u2.g, ui1_alpha, pt_edge_clr->u2.g, pui1_edge[x]),
                                                _BLEND_CLR(pt_fg_clr->u3.b, ui1_alpha, pt_edge_clr->u3.b, pui1_edge[x]));
                            }
                            else
                            {
                                pui4_dst[x] = GL_COLOR_ARGB8888(
                                    ui1_alpha, pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                            }
                        }
                        else if (pt_edge_clr && (pui1_edge[x] > 0))
                        {
                            pui4_dst[x] = GL_COLOR_ARGB8888(
                                    pui1_edge[x], pt_edge_clr->u1.r, pt_edge_clr->u2.g, pt_edge_clr->u3.b);
                        }
                        else if (pt_bg_clr)
                        {
                            pui4_dst[x] = GL_COLOR_ARGB8888(
                                pt_bg_clr->a, pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b);
                        }
                    }

                    pui4_dst  = (UINT32*) ((UINT8*) pui4_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                    pui1_edge += ui4_src_pitch;
                }
                
                break;
            }
            
            default:
                break;
        }
    }
    else if (e_src_mode == FE_PIXEL_MODE_MONO)
    {
        switch (e_dst_mode)
        {
            case GFX_COLORMODE_ARGB_CLUT8:
            case GFX_COLORMODE_AYCbCr_CLUT8:
            {
                UINT8*  pui1_dst   = (UINT8*) pv_dst;
                UINT8   ui1_fg_idx = pt_fg_clr->u3.index;
                UINT8   ui1_bg_idx = ((pt_bg_clr) ? pt_bg_clr->u3.index : 0);
                UINT8*  pui1_edge = pui1_src + h * ui4_src_pitch;
                UINT8   ui1_edge_idx = ((pt_edge_clr) ? pt_edge_clr->u3.index : 0);

                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        if (0 != ((pui1_src[x >> 3] >> (7 - (x & 0x7))) & 1))
                        {
                            pui1_dst[x] = ui1_fg_idx;
                        }
                        else if (pt_edge_clr && (0 != ((pui1_edge[x >> 3] >> (7 - (x & 0x7))) & 1)))
                        {
                            pui1_dst[x] = ui1_edge_idx;
                        }
                        else if (pt_bg_clr)
                        {
                            pui1_dst[x] = ui1_bg_idx;
                        }
                    }

                    pui1_dst  += ui4_dst_pitch;
                    pui1_src  += ui4_src_pitch;
                    pui1_edge += ui4_src_pitch;
                }

                break;
            }

            case GFX_COLORMODE_RGB_D565:
            {
                UINT16* pui2_dst   = (UINT16*) pv_dst;
                UINT16  ui2_fg_clr = (UINT16) GL_COLOR_RGB565(pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                UINT16  ui2_bg_clr = 
                    ((pt_bg_clr) ? (UINT16) GL_COLOR_RGB565(pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b) : 0);
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        if (0 != ((pui1_src[x >> 3] >> (7 - (x & 0x7))) & 1))
                        {
                            pui2_dst[x] = ui2_fg_clr;
                        }
                        else if (pt_bg_clr)
                        {
                            pui2_dst[x] = ui2_bg_clr;
                        }
                    }

                    pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
            
            case GFX_COLORMODE_ARGB_D4444:
            {
                UINT16* pui2_dst   = (UINT16*) pv_dst;
                UINT16  ui2_fg_clr = (UINT16) GL_COLOR_ARGB4444(255, pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                UINT16  ui2_bg_clr = 
                    ((pt_bg_clr) ? (UINT16) GL_COLOR_ARGB4444(255, pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b) : 0);
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        if (0 != ((pui1_src[x >> 3] >> (7 - (x & 0x7))) & 1))
                        {
                            pui2_dst[x] = ui2_fg_clr;
                        }
                        else if (pt_bg_clr)
                        {
                            pui2_dst[x] = ui2_bg_clr;
                        }
                    }

                    pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
            
            case GFX_COLORMODE_ARGB_D8888:
            {
                UINT32* pui4_dst   = (UINT32*) pv_dst;
                UINT32  ui4_fg_clr = (UINT32) GL_COLOR_ARGB8888(255, pt_fg_clr->u1.r, pt_fg_clr->u2.g, pt_fg_clr->u3.b);
                UINT32  ui4_bg_clr = 
                    ((pt_bg_clr) ? (UINT32) GL_COLOR_ARGB8888(255, pt_bg_clr->u1.r, pt_bg_clr->u2.g, pt_bg_clr->u3.b) : 0);
                
                for (y = 0; y < h; y++)
                {
                    for (x = 0; x < w; x++)
                    {
                        if (0 != ((pui1_src[x >> 3] >> (7 - (x & 0x7))) & 1))
                        {
                            pui4_dst[x] = ui4_fg_clr;
                        }
                        else if (pt_bg_clr)
                        {
                            pui4_dst[x] = ui4_bg_clr;
                        }
                    }

                    pui4_dst  = (UINT32*) ((UINT8*) pui4_dst + ui4_dst_pitch);
                    pui1_src += ui4_src_pitch;
                }
                
                break;
            }
            
            default:
                GL_DFB_TRACE((
                    "{GRAPHIC} ERR: Unsupported combination of destination color mode (%d) and bitmap pixel mode (%d) at L%d\r\n\r\n", 
                    e_dst_mode,
                    e_src_mode,
                    __LINE__));
                break;

        }
    }
    else
    {
        GL_DFB_TRACE((
            "{GRAPHIC} ERR: Unsupported pixel mode (%d) at L%d\r\n\r\n", 
            e_src_mode,
            __LINE__));
    }
}

VOID gl_surface_get_clip_area(VOID* pv_obj, GL_RECT_T* pt_clip);
static INT32 _draw_char_array(
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
    INT32               i4_base_x, i4_base_y;
    INT32               i4_adj_x, i4_adj_y;
    INT32               i4_adv_x, i4_adv_y;
    INT32               i4_ret;
    UINT16              i;
    BOOL                b_has_kern;
    UTF16_T             w2_ch, w2_ch_prv;
    FE_GLYPH_T          t_glyph;
    INT16               i2_descend, i2_height;
    GL_DFB_SURFACE_T*         pt_surf;
    GL_LOCK_INFO_T      t_lock_info;
    GL_RECT_T           t_clip, t_dst, t_src, t_rc_str;
    FE_FNT_STYLE        t_fnt_style;
    FE_RENDER_MODE_T    e_render_mode;

    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

    i4_ret = GLR_OK;
    
    /* find clipping area */
    gl_surface_get_clip_area(pt_surf, &t_clip);
 
    if (RECT_EMPTY(&t_clip))
    {
    
        GL_DFB_TRACE(("{GRAPHIC} The surface clip area is empty [%d,%d,%d,%d].\r\n\r\n",
            t_clip.i4_left,
            t_clip.i4_top,
            t_clip.i4_right,
            t_clip.i4_bottom));
        goto _DRAW_USTRING_EXIT_2;
    }

    /* get surface memory raw pointer */
    i4_ret = gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);
    if (GL_FAILED(i4_ret))
    {
        PRINT_LINE_DBG;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Locking surface failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EXIT_2;
    }
    
    PRINT_LINE_DBG;

    if (FER_OK != x_fe_get_height(h_fnt, &i2_height) ||
        FER_OK != x_fe_get_descender(h_fnt, &i2_descend))
    {
        PRINT_LINE_DBG;
        i4_ret = GLR_FE_ERROR;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting height/descender failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EXIT_1;
    }
        
    /* set up font rendering mode */
    if (GL_IS_PALETTE_MODE(t_lock_info.e_clrmode) ||
        GL_COLORMODE_RGB_D565 == t_lock_info.e_clrmode)
    {
        PRINT_LINE_DBG;
        e_render_mode = FE_RENDER_MODE_MONO;
    }
    else
    {
        PRINT_LINE_DBG;
        e_render_mode = FE_RENDER_MODE_NORMAL;
    }
    
    /* draw each char onto the bitmap */
    i4_base_x  = 0;
    i4_base_y  = 0;
    w2_ch_prv  = 0;
    b_has_kern = x_fe_has_kerning(h_fnt);

    SET_RECT(&t_rc_str, 0x7fffffff, 0x7fffffff, -0x7fffffff, -0x7fffffff);
    
    for(i = 0; i < ui2_strlen; i++)
    {
        PRINT_LINE_DBG;
        /* load character */
        w2_ch  = x_uc_w2s_get(w2s_str, i);
        PRINT_LINE_DBG;

        /* get kerning info, and move base */
        if (b_has_kern)
        {
           /* i4_ret = x_fe_get_kerning(
                h_fnt,
                (CHAR_CODE) w2_ch_prv,
                (CHAR_CODE) w2_ch,
                &i4_adj_x,
                &i4_adj_y);*/
                i4_adj_x = 0;
            i4_adj_y = 0;
            
            if (i4_ret == FER_OK)
            {
                i4_base_x += i4_adj_x;
                i4_base_y += i4_adj_y;
            }
            else
            {
                GL_DFB_TRACE(("{GRAPHIC} No kerning available for pair (%d, %d)", w2_ch_prv, w2_ch));
            }
        }
        
        PRINT_LINE_DBG;
        /* get character advance */
        i4_ret = x_fe_get_glyph_advance(
            h_fnt,
            (CHAR_CODE) w2_ch,
            &i4_adv_x,
            &i4_adv_y);
        
        PRINT_LINE_DBG;
        if (i4_ret != FER_OK)
        {
            PRINT_LINE_DBG;
            i4_ret = GLR_FE_ERROR;
            GL_DFB_TRACE(("{GRAPHIC} ERR: Getting glyph advance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DRAW_USTRING_EXIT_1;
        }
        
        /* calculate the possible maxumum bounding-box of the specified char */
        i4_adj_x = i4_x + i4_base_x;
        i4_adj_y = i4_y + i4_base_y;

        SET_RECT(
            &t_dst, 
            i4_adj_x, 
            i4_adj_y, 
            i4_adj_x + i4_adv_x,
            i4_adj_y + (INT32) i2_height);
        PRINT_LINE_DBG;

        _check_n_swap(&t_dst);
        
        INTERSECT_RECT(&t_dst, &t_clip, &t_dst);

        if (!RECT_EMPTY(&t_dst)) /* not empty, the char might fall within the clipping rectangle */
        {
            PRINT_LINE_DBG;
            /* load and rener char */
            t_glyph.e_render_mode = e_render_mode;
            i4_ret = x_fe_load_char(h_fnt, (CHAR_CODE) w2_ch, &t_glyph);
            
            PRINT_LINE_DBG;
            if (i4_ret != FER_OK)
            {
                i4_ret = GLR_FE_ERROR;
                GL_DFB_TRACE(("{GRAPHIC} ERR: Loading character [%d] failed (%d) at L%d\r\n\r\n", w2_ch, i4_ret, __LINE__));
                goto _DRAW_USTRING_EXIT_1;
            }

            /* calculate the left-top corner of the char destination */
            i4_adj_x = i4_x + i4_base_x + t_glyph.i4_left;
            i4_adj_y = i4_y + i4_base_y + (i2_height + i2_descend - 1) - t_glyph.i4_top;
            
            SET_RECT(
                &t_dst, 
                i4_adj_x, 
                i4_adj_y, 
                i4_adj_x + t_glyph.t_bmp.i4_width,
                i4_adj_y + t_glyph.t_bmp.i4_height);
                
            _check_n_swap(&t_dst);
            
            PRINT_LINE_DBG;
            INTERSECT_RECT(&t_dst, &t_clip, &t_dst);

            if (!RECT_EMPTY(&t_dst))
            {
                t_src = t_dst;
                MOVE_RECT(&t_src, - i4_adj_x, - i4_adj_y);
            
                PRINT_LINE_DBG;
                /* draw the char on bitmap */
                _draw_char(
                    t_lock_info.at_subplane[0].pv_bits,
                    &t_dst,
                    t_lock_info.at_subplane[0].ui4_pitch,
                    t_lock_info.e_clrmode,
                    t_glyph.t_bmp.pui1_buf,
                    &t_src,
                    (UINT32) t_glyph.t_bmp.i4_pitch,
                    t_glyph.t_bmp.e_mode,
                    pt_fg_clr,
                    pt_bg_clr,
                    NULL,
                    GL_DRAW_STR_DEFAULT);
                PRINT_LINE_DBG;

                UNION_RECT(&t_rc_str, &t_rc_str, &t_dst);
            }
        }
        
        /* move base */

        i4_base_x += i4_adv_x;
        i4_base_y += i4_adv_y;

        w2_ch_prv = w2_ch;
    }

    if ((FER_OK == x_fe_get_style(h_fnt, &t_fnt_style)) &&
        (t_fnt_style & FE_FNT_STYLE_UNDERLINE))
    {
        INT16 i2_ul_pos, i2_ul_thickness;
        PRINT_LINE_DBG;

        if ((FER_OK == x_fe_get_underline_thickness(h_fnt, &i2_ul_thickness)) &&
            (FER_OK == x_fe_get_underline_position(h_fnt, &i2_ul_pos)))
        {
            GL_RECT_T t_ul;
            PRINT_LINE_DBG;

            SET_RECT_BY_SIZE(
                &t_ul, 
                t_rc_str.i4_left, 
                i4_y + (INT32) (i2_height + i2_descend - i2_ul_pos),
                RECT_W(&t_rc_str), 
                i2_ul_thickness);

            INTERSECT_RECT(&t_ul, &t_ul, &t_clip);

            if (!RECT_EMPTY(&t_ul))
            {
                /*gl_gpu_fill_rect(*/

                PRINT_LINE_DBG;
                gl_dfb_locked_fill_rect(
                    t_lock_info.e_clrmode,
                    t_lock_info.at_subplane[0].pv_bits,
                    t_lock_info.at_subplane[0].ui4_pitch,
                    t_ul.i4_left,
                    t_ul.i4_top,
                    RECT_W(&t_ul),
                    RECT_H(&t_ul),
                    gl_dfb_compose_packed_color(pt_fg_clr, t_lock_info.e_clrmode));
                PRINT_LINE_DBG;

                /*gl_surface_flush(pt_surf, 0);*/
            }
        }
    }

    if (pt_rc_str)
    {
        PRINT_LINE_DBG;
        if (RECT_EMPTY(&t_rc_str))
        {
            SET_RECT(pt_rc_str, 0, 0, 0, 0);
        }
        else
        {
            PRINT_LINE_DBG;
            *pt_rc_str = t_rc_str;
        }
    }
    
    PRINT_LINE_DBG;
    i4_ret = GLR_OK;
    
_DRAW_USTRING_EXIT_1:
    
    PRINT_LINE_DBG;
    gl_dfb_surface_unlock(h_surf, NULL);

_DRAW_USTRING_EXIT_2:
    PRINT_LINE_DBG;

    gl_dfb_obj_unlock(pt_surf);
    
    return (i4_ret);
}

static BOOL _is_skip_char(
    CHAR_CODE           t_ch, 
    VOID*               pv_skip, 
    UINT16              ui2_skiplen,
    FE_ENCODING_TYPE_T  e_encoding)
{
    UTF16_T     *w2s_str = (UTF16_T *)pv_skip, *w2s_end;

    w2s_end = w2s_str + ui2_skiplen;

    for ( ; w2s_str < w2s_end; ++w2s_str)
    {
        if (/*x_fe_get_char(pv_skip, i, e_encoding)*/(*w2s_str) == (UTF16_T)t_ch)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

static BOOL _is_skip_char_no_encoding(
    CHAR_CODE           t_ch, 
    VOID*               pv_skip, 
    UINT16              ui2_skiplen)
{
    UTF16_T     *w2s_str = (UTF16_T *)pv_skip, *w2s_end;

    w2s_end = w2s_str + ui2_skiplen;

    for ( ; w2s_str < w2s_end; ++w2s_str)
    {
        if (/*x_fe_get_char(pv_skip, i, e_encoding)*/(*w2s_str) == (UTF16_T)t_ch)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

static VOID _check_n_swap(GL_RECT_T* pt_rect)
{
    INT32 i4_tmp;

    if (pt_rect->i4_right < pt_rect->i4_left)
    {
        i4_tmp = pt_rect->i4_left;
        pt_rect->i4_left  = pt_rect->i4_right;
        pt_rect->i4_right = i4_tmp;
    }
    else if (pt_rect->i4_bottom < pt_rect->i4_top)
    {
        i4_tmp = pt_rect->i4_top;
        pt_rect->i4_top    = pt_rect->i4_bottom;
        pt_rect->i4_bottom = i4_tmp;
    }
}

static INT32 _draw_ustring_ex(
    GL_HSURFACE_T       h_surf,
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
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode,
    GL_STR_AUX_T        *pt_aux_data)
{
    INT32                       i4_base_x, i4_base_y;
    INT32                       i4_adj_x, i4_adj_y;
    INT32                       i4_dst_x, i4_dst_y;
    INT32                       i4_prv_dst_x, i4_prv_dst_y;
    INT32                       i4_offset_x, i4_offset_y;
    INT32                       i, j, i4_skip_x, i4_strlen, i4_ret;
    BOOL                        b_has_kern;
    CHAR_CODE                   t_ch, t_ch_prv;
    FE_GLYPH_T                  t_glyph;
    INT16                       i2_descend, i2_height, i2_width;
    /*HANDLE_TYPE_T               e_type;*/
    GL_DFB_SURFACE_T*                 pt_surf;
    GL_LOCK_INFO_T              t_lock_info;
    GL_RECT_T                   t_clip, t_dst, t_src, t_stripe = {0}, t_rc_str;

    INT32                       i4_skiplen;
    UINT32                      ui4_buf_w, ui4_buf_h, ui4_buf_pitch;
    VOID*                       pv_buf;
    /*GFX_ALPHA_COMPOSITION_T     t_compose;
    GFX_TRANSPARENT_BITBLT_T    t_tp_blt;
    GFX_FILL_T                  t_fill;*/
    UINT32                      ui4_bg_color = 0;
    INT32                       i4_bg_ys = 0, i4_bg_ye = 0;
    INT32                       i4_adv_x, i4_adv_y;
    BOOL                        b_has_cmd, b_locked, b_null_glyph;
    FE_FNT_STYLE                t_fnt_style;
    UINT8                       ui1_text_bg, ui1_shf;
    FE_RENDER_MODE_T            e_render_mode;
    UINT16                      ui2_num_run;
    FE_ENCODING_TYPE_T          e_encoding;
    _CHAR_POS_INFO              *pt_char_info;  
    INT32                       i4_prev_index = 0;

    UTF16_T                     *w2s_str, *w2s_skip;

    /* patch for CR2073 */
    GL_RECT_T                   t_rc_buf;
    /* patch for CR2073 */

    /*CHECK_VALID(gl_is_init(), GLR_NOT_INIT);*/
    PRINT_LINE_DBG;

    w2s_str  = (UTF16_T *)pv_str;
    w2s_skip = (UTF16_T *)pv_skip;

    CHECK_VALID(pt_fg_clr, GLR_INV_ARG);
    CHECK_VALID(pv_str && (ui2_strlen > 0), GLR_INV_ARG);
    CHECK_VALID(
        (pv_skip && (ui2_skiplen > 0)) || (!pv_skip && (ui2_skiplen == 0)), 
        GLR_INV_ARG);

    CHECK_VALID(FER_OK == x_fe_get_encoding(h_fnt, &e_encoding), GLR_INV_HANDLE);

    //i4_strlen = x_fe_strlen((UINT8 *)pv_str, e_encoding, (SIZE_T)ui2_strlen);
    i4_strlen = x_uc_w2s_strlen(w2s_str);
    
    /*printf("i4_strlen (src) is %d\n", i4_strlen);*/
    
    i4_strlen = MIN(i4_strlen, (INT32) ui2_strlen);
    
    CHECK_VALID(i4_strlen > 0, GLR_INV_ARG);
    ui2_strlen = (UINT16)ui2_strlen;
    b_locked = FALSE;

    /* check string of ignorance */
    if (pv_skip)
    {
        //i4_skiplen = x_fe_strlen((UINT8 *)pv_skip, e_encoding, (SIZE_T)ui2_skiplen);
        i4_skiplen = x_uc_w2s_strlen(w2s_skip);
        i4_skiplen = MIN(i4_skiplen, (INT32) ui2_skiplen);
    }
    else
    {
        i4_skiplen = 0;
    }
    ui2_skiplen = (UINT16)i4_skiplen;
    if (pt_aux_data->pv_skip_line)
    {        
        pt_char_info = (_CHAR_POS_INFO *)x_mem_alloc(i4_strlen * sizeof(_CHAR_POS_INFO));
        
        if (pt_char_info == NULL)       
        {            
            return GLR_NO_MEMORY;        
        }        

        x_memset((VOID *)pt_char_info, 0, i4_strlen * sizeof(_CHAR_POS_INFO));
    }
    else    
    {        
        pt_char_info = NULL;    
    }

    PRINT_LINE_DBG;

    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

    i4_ret = GLR_OK;
    
    /* find clipping area */
    /*t_clip = pt_surf->t_clip_rect;*/
    gl_surface_get_clip_area(pt_surf, &t_clip);
    if (RECT_EMPTY(&t_clip) || i4_x >= t_clip.i4_right || i4_y >= t_clip.i4_bottom)
    {

        PRINT_LINE_DBG;
        GL_DFB_TRACE("{GRAPHIC} The surface clip area is empty [%d,%d,%d,%d].\r\n\r\n",
            t_clip.i4_left,
            t_clip.i4_top,
            t_clip.i4_right,
            t_clip.i4_bottom);
        goto _DRAW_USTRING_EX_EXIT_2;
    }
        

    /* get surface memory raw pointer */
    i4_ret = gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);


    PRINT_LINE_DBG;
    if (GL_FAILED(i4_ret))
    {

        GL_DFB_TRACE(("{GRAPHIC} ERR: Locking surface failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_2;
    }

    if (FER_OK != x_fe_get_max_advance(h_fnt, &i2_width, &i2_height) ||
        FER_OK != x_fe_get_height(h_fnt, &i2_height) ||
        FER_OK != x_fe_get_descender(h_fnt, &i2_descend))
    {

        i4_ret = GLR_FE_ERROR;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting height/descender failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_1;
    }
        

    SET_RECT(&t_rc_str, 0x7fffffff, 0x7fffffff, -0x7fffffff, -0x7fffffff);

    if (pt_bg_clr)
    {
        ui4_bg_color = gl_dfb_compose_packed_color(pt_bg_clr, t_lock_info.e_clrmode);
    }

    /* draw each char onto the bitmap */
    i4_base_x  = i4_x;
    i4_base_y  = i4_y;
    t_ch_prv   = 0;
    b_has_kern = x_fe_has_kerning(h_fnt);

    i4_bg_ys = MAX(i4_bg_y, t_clip.i4_top);
    i4_bg_ye = MIN(i4_bg_y + ui2_bg_h, t_clip.i4_bottom);
    if (i4_bg_ys >= i4_bg_ye)
    {
        ui2_bg_h = 0;
    }
    else
    {
        ui2_bg_h = (UINT16) (i4_bg_ye - i4_bg_ys);
    }

    /* [20808] patch for the (i2_height + i2_descend) / 4 */
    ui4_buf_h = MAX((UINT32) (i2_height + (i2_height + i2_descend) / 4), (UINT32) ui2_bg_h);
#if 0
    if(ui4_buf_h > MAX_BUF_HEIGHT)
    {
        GL_DFB_TRACE("Waring:Outofbufbounds:H %d\n", ui4_buf_h);
        ui4_buf_h = MAX_BUF_HEIGHT;
    }
#endif
    ui4_buf_w = (UINT32)(i4_x + RECT_W(&t_clip));
    /* check edge effect */
    if (FER_OK != x_fe_get_style(h_fnt, &t_fnt_style))
    {
        i4_ret = GLR_FE_ERROR;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting style failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_1;
    }
    if (!(t_fnt_style & FE_FNT_STYLE_EDGE_EFFECT))
    {
        pt_edge_clr = NULL;
    }

    gl_cal_buf_param_by_height(
        t_lock_info.e_clrmode, 
        ui4_buf_h, 
        &pv_buf, 
        &ui4_buf_pitch, 
        &ui4_buf_w);

    SET_RECT_BY_SIZE(
        &t_rc_buf,
        0,
        0,
        ui4_buf_w,
        ui4_buf_h);
    
    b_locked = FALSE;

    i = 0;
    PRINT_LINE_DBG;

    ui1_shf = _shf_bit(t_lock_info.e_clrmode);
    
    ui1_text_bg = 
        (GL_IS_PALETTE_MODE(t_lock_info.e_clrmode)) ? (pt_fg_clr->u3.index + 1) : 0;

    /* set up font rendering mode */
    if (GL_IS_PALETTE_MODE(t_lock_info.e_clrmode) ||
        GL_COLORMODE_RGB_D565 == t_lock_info.e_clrmode)
    {
        e_render_mode = FE_RENDER_MODE_MONO;
    }
    else
    {
        e_render_mode = FE_RENDER_MODE_NORMAL;
    }
    
    ui2_num_run = 0;
    i4_prev_index = 0;

    while (1)
    {
        INT32       i4_stripe_start = 0, i4_stripe_end = 0;
        INT32       i4_effect_offset = 0;
        
        i4_prv_dst_x = i4_dst_x = i4_prv_dst_y = 0;
        i4_dst_y  = 0;
        i4_skip_x = 0;
        b_has_cmd = FALSE;

        if (!b_locked)
        {
            b_locked = TRUE;
        }
        PRINT_LINE_DBG;
            

        for (j = 0; i < i4_strlen; j++)
        {
            BOOL b_skip_line = FALSE;
            /* MHF Linux - Chun Jiang for fix \t display*/
            if(/*((INT16 *)pv_str)*/w2s_str[i] == 0x9)
            {
                i ++;
                break;
            }
            
            /* load character */
            //t_ch = x_fe_get_char(pv_str, i, e_encoding);
            t_ch = x_uc_w2s_get(w2s_str, i);
            
            if (pt_aux_data->pv_skip_line)   
            {
                b_skip_line = (FE_IS_CHAR_NO_UNDERLINE(t_ch) || _is_skip_char(t_ch, pt_aux_data->pv_skip_line, pt_aux_data->ui2_skip_line_len, e_encoding)) ? TRUE : FALSE;            
            }
            
            PRINT_LINE_DBG;
            if (pv_skip && _is_skip_char(t_ch, pv_skip, ui2_skiplen, e_encoding))
            {   /* handle skip character */
                if (pt_char_info)
                {
                    if (!b_skip_line)
                    {
                        pt_char_info[i].i4_begin = i4_stripe_end;
                        pt_char_info[i].i4_width = i4_skip_x;
                    }
                    else
                    {
                        pt_char_info[i].i4_begin = 0;
                        pt_char_info[i].i4_width = 0;
                    }
                }
                i++;
                t_ch_prv = 0;
                i4_skip_x = (INT32) ui2_skip_w;
                break;
            }
            else
            {
                /* get character advance */
                i4_adv_x = 0;
                i4_adv_y = 0;
                if (t_ch == FE_CC_LEADING_CHAR ||
                    (t_ch >= FE_UTF16_SPACE_USER_START && t_ch <= FE_UTF16_SPACE_USER_END))
                {
                        /* fake character */
                        t_ch = (CHAR_CODE)0x20;
                }

                
                PRINT_LINE_DBG;
                i4_ret = x_fe_get_glyph_advance(
                    h_fnt,
                    t_ch,
                    &i4_adv_x,
                    &i4_adv_y);
                
                if (i4_ret != FER_OK)
                {PRINT_LINE_DBG;
                    i4_ret = GLR_FE_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Getting glyph advance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                    goto _DRAW_USTRING_EX_EXIT_1;
                }
                    
                PRINT_LINE_DBG;
                /* load glyph bitmap */
                t_glyph.e_render_mode = e_render_mode;
                i4_ret = x_fe_load_char(h_fnt, t_ch, &t_glyph);
                
                PRINT_LINE_DBG;
                if (i4_ret != FER_OK)
                {
                    PRINT_LINE_DBG;
                    i4_ret = GLR_FE_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Loading character [%d] failed (%d) at L%d\r\n\r\n", t_ch, i4_ret, __LINE__));
                    goto _DRAW_USTRING_EX_EXIT_1;
                }

                PRINT_LINE_DBG;
                b_null_glyph = ((t_glyph.t_bmp.i4_width == 0) || (t_glyph.t_bmp.i4_height == 0));
                if (b_fixed)
                {
                    if (b_null_glyph)
                    {
                        i4_adj_x = i4_dst_x;
                    }
                    else
                    {
                        /* calculate the left-top corner of the char destination */
                        i4_adj_x = i4_dst_x + (ui2_ch_w - t_glyph.t_bmp.i4_width) / 2;
                    }
                    
                    i4_adv_x = (INT32) ui2_ch_w;
                }
                else
                {
                    /* get kerning info, and move base */
                    if (b_has_kern)
                    {
                        i4_ret = x_fe_get_kerning(
                            h_fnt,
                            t_ch_prv,
                            t_ch,
                            &i4_adj_x,
                            &i4_adj_y);
                        
                        if (i4_ret == FER_OK)
                        {
                            i4_dst_x += i4_adj_x;
                            i4_dst_y += i4_adj_y;
                        }
                        else
                        {
                            GL_DFB_TRACE("{GRAPHIC} No kerning available for pair (%d, %d)\n", t_ch_prv, t_ch);
                        }

                    }
                
                    if (b_null_glyph)
                    {
                        i4_adj_x = i4_dst_x;
                    }
                    else
                    {
                        /* calculate the left-top corner of the char destination */
                        i4_adj_x = i4_dst_x + t_glyph.i4_left;
                    }
                }

                if (b_null_glyph)
                {
                    i4_adj_y = i4_dst_y + (i2_height + i2_descend);
                }
                else
                {
                    i4_adj_y = i4_dst_y + (i2_height + i2_descend) - t_glyph.i4_top;
                }
                   
                if (b_null_glyph)
                {
                    SET_RECT(
                        &t_dst, 
                        i4_adj_x, 
                        i4_adj_y, 
                        i4_adj_x + i4_adv_x,
                        i4_adj_y + i4_adv_y);
                }
                else
                {
                    /* set character rendering rectangle */
                    SET_RECT(
                        &t_dst, 
                        i4_adj_x, 
                        i4_adj_y, 
                        i4_adj_x + t_glyph.t_bmp.i4_width,
                        i4_adj_y + t_glyph.t_bmp.i4_height);
                }


                PRINT_LINE_DBG;
                _check_n_swap(&t_dst);

                if (t_dst.i4_right >= ui4_buf_w)
                {
                    break;
                }
                
                if (j == 0)
                {
                    t_stripe = t_dst;
                }
                else
                {
                    UNION_RECT(&t_stripe, &t_stripe, &t_dst);
                }

                /* adjust with the advance of the current character */
                i4_dst_x += i4_adv_x;
                i4_dst_y += i4_adv_y;
                    
                if (t_dst.i4_left < 0)
                {
                    i4_offset_x     = t_dst.i4_left;
                    t_dst.i4_left   = 0;
                    t_dst.i4_right -= i4_offset_x;

                    i4_dst_x       -= i4_offset_x;
                }

                if (t_dst.i4_top < 0)
                {
                    i4_offset_y      = t_dst.i4_top;
                    t_dst.i4_top     = 0;
                    t_dst.i4_bottom -= i4_offset_y;
                    
                    i4_dst_y        -= i4_offset_y;
                }
                
                /* patch for CR2073 */
                INTERSECT_RECT(&t_dst, &t_dst, &t_rc_buf);

                /* check if this string is going to be drawn out */
                {
                    if (i4_prv_dst_x < t_dst.i4_right)
                    {
                        _fill_intl_buf(
                            pv_buf,
                            ui4_buf_pitch,
                            (i4_prv_dst_x << ui1_shf),
                            0,
                            ((t_dst.i4_right - i4_prv_dst_x) << ui1_shf),
                            ui4_buf_h,
                            ui1_text_bg);
                    }

                    if (j == 0)
                    {
                        i4_stripe_start = i4_prv_dst_x;
                    }

                    i4_prv_dst_x = t_dst.i4_right;
                }

                PRINT_LINE_DBG;
                if (!b_null_glyph)
                {
                    SET_RECT_BY_SIZE(
                        &t_src, 
                        0,
                        0,
                        RECT_W(&t_dst),
                        RECT_H(&t_dst));
                    PRINT_LINE_DBG;

                    /* check if this character is going to be drawn out */
                    if (pt_aux_data->aui2_display_idx == NULL || 
                        _check_display_idx_inside(i, pt_aux_data->aui2_display_idx, pt_aux_data->ui2_display_idx_num))
                    {
                        PRINT_LINE_DBG;
                        /* patch for CR2073 */
                        /* step 1: draw the char on internal buffer */
                        _draw_char(
                            pv_buf,
                            &t_dst,
                            ui4_buf_pitch,
                            t_lock_info.e_clrmode,
                            t_glyph.t_bmp.pui1_buf,
                            &t_src,
                            (UINT32) t_glyph.t_bmp.i4_pitch,
                            t_glyph.t_bmp.e_mode,
                            pt_fg_clr,
                            NULL,
                            pt_edge_clr,
                            e_draw_str_mode);
                            
                        if (pt_char_info)
                        {
                            if (!b_skip_line)
                            {
                                pt_char_info[i].i4_begin = t_dst.i4_left;
                                pt_char_info[i].i4_width = i4_adv_x;
                            }
                            else
                            {
                                pt_char_info[i].i4_begin = 0;
                                pt_char_info[i].i4_width = 0;
                            }
                        }
                    }

                    i4_stripe_end = t_dst.i4_right;
                }
                
                else
                {
                    if (pt_char_info)
                    {
                        if (!b_skip_line)
                        {
                            pt_char_info[i].i4_begin = t_dst.i4_left;
                            pt_char_info[i].i4_width = i4_adv_x;
                        }
                        else
                        {
                            pt_char_info[i].i4_begin = 0;
                            pt_char_info[i].i4_width = 0;
                        }
                    }
                }
                
                t_ch_prv = t_ch;
                i++;
            }

        }
        PRINT_LINE_DBG;

        if (j > 0)
        {PRINT_LINE_DBG;
            if (pt_bg_clr && ((ui2_bg_h > 0) && (i4_dst_x > i4_bg_adj_x)))
            {
                {
                    INT32   i4_added_width;
                    i4_effect_offset = (i4_stripe_end - i4_stripe_start) - i4_dst_x;

                    i4_effect_offset = (i4_effect_offset < 0)? 0: i4_effect_offset;
                    PRINT_LINE_DBG;
                    x_fe_get_add_width_by_synthesis(h_fnt, &i4_added_width);

                    SET_RECT_BY_SIZE(
                        &t_dst, 
                        i4_base_x + i4_bg_adj_x, 
                        i4_bg_ys, 
                        (UINT32) (i4_dst_x - i4_bg_adj_x) + i4_effect_offset + i4_added_width,   
                        ui2_bg_h);
                }
                PRINT_LINE_DBG;

                INTERSECT_RECT(&t_dst, &t_clip, &t_dst);
                /*Fix me: CC left a black line in bottom and top*/
                /*t_dst.i4_top ++;
                t_dst.i4_bottom --;
                End fix ,It seems oK now?    
                */
                if (!RECT_EMPTY(&t_dst))
                {PRINT_LINE_DBG;
                    /* check if it's in the subset mode */
                    if (pt_aux_data->aui2_display_idx == NULL)
                    {
                        PRINT_LINE_DBG;
                        gl_dfb_surface_unlock(h_surf, NULL);
                        gl_dfb_surface_draw_rect(
                                h_surf,
                                &t_dst,
                                pt_bg_clr,
                                1,
                                GL_STYLE_FILLED,
                                0);
                        /*gl_dfb_locked_fill_rect(
                            t_lock_info.e_clrmode,
                            t_lock_info.at_subplane[0].pv_bits,
                            t_lock_info.at_subplane[0].ui4_pitch,
                            t_dst.i4_left,
                            t_dst.i4_top,
                            (UINT32) RECT_W(&t_dst),
                            (UINT32) RECT_H(&t_dst),
                            ui4_bg_color);*/
                        gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);
                           
                    }
                    PRINT_LINE_DBG;

                    SET_RECT_BY_SIZE(
                        &t_dst, 
                        i4_base_x, 
                        i4_bg_ys, 
                        (UINT32) i4_dst_x, 
                        ui2_bg_h);

                    PRINT_LINE_DBG;
                    UNION_RECT(&t_rc_str, &t_rc_str, &t_dst);
                }
            }
            
            if (t_stripe.i4_left < 0)
            {
                INT32   i4_offset;
                PRINT_LINE_DBG;
                if (0 == ui2_num_run)
                {
                    /* [DTV00011723] */
                    /* for 'j' (which has a negative left) standing in the first character of each run */
                    if (j == 1)
                    {
                        i4_offset = t_stripe.i4_left;

                        t_stripe.i4_left  = 0;
                        t_stripe.i4_right -= i4_offset;
                    }
                    else
                    {
                        t_stripe.i4_left = 0;
                    }
                }
                else
                {
                    /* [DTV00011723] */
                    if (j > 1)
                    {
                        t_stripe.i4_right += t_stripe.i4_left;
                    }
                }
            }
            PRINT_LINE_DBG;
            if (t_stripe.i4_top < 0)
            {
                /* should the patch for [11723] be applied here for drawing vertically? */
                t_stripe.i4_bottom += t_stripe.i4_top;
            }
            PRINT_LINE_DBG;
            if (!b_fixed)
            {
                INT32 i4_tmp = i4_dst_x;
                
                i4_dst_x  = ((t_stripe.i4_left < 0) ? (t_stripe.i4_right + t_stripe.i4_left) : t_stripe.i4_right);
                i4_dst_x += (i4_tmp - t_stripe.i4_right);
                
                i4_dst_y += ((t_stripe.i4_top < 0) ? (t_stripe.i4_top) : 0);
            }
                PRINT_LINE_DBG;
            i4_offset_x = ((t_stripe.i4_left < 0) ? 0 : t_stripe.i4_left);
            i4_offset_y = ((t_stripe.i4_top < 0) ? 0 : t_stripe.i4_top);
            
            MOVE_RECT(&t_stripe, i4_base_x, i4_base_y);
            INTERSECT_RECT(&t_dst, &t_clip, &t_stripe);
            
            /* adjust the char starting position */
            if (pt_char_info)
            {
                INT32       i4_i;
                INT32       i4_offset;

                i4_offset = t_dst.i4_left /*- (t_stripe.i4_left - i4_base_x)*/ - (i4_offset_x + (t_dst.i4_left - t_stripe.i4_left));

                for (i4_i = i4_prev_index; i4_i < i; i4_i++)
                {
                    pt_char_info[i4_i].i4_begin += i4_offset;
                }
            }
            PRINT_LINE_DBG;
            if (!RECT_EMPTY(&t_dst))
            {
                PRINT_LINE_DBG;
                gl_dfb_surface_unlock(h_surf, NULL);
                /* step 3: compose the internal buffer (text foreground) to the target surface */
                /* use bitblt for BDF fonts drawing */
               
                PRINT_LINE_DBG;
                if ((e_draw_str_mode == GL_DRAW_STR_DEFAULT && 
                     (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_CLUT8 || 
                      t_lock_info.e_clrmode == GL_COLORMODE_AYUV_CLUT8 || 
                      e_render_mode == FE_RENDER_MODE_MONO)) ||
                    e_draw_str_mode == GL_DRAW_STR_BITBLT)
                {
                    GL_COLOR_T t_clr_k;
                    SET_COLOR(&t_clr_k, ui1_text_bg, 0, 0, 0);
                    
                    PRINT_LINE_DBG;
                    gl_dfb_surface_transp_blt(
                        h_surf,
                        _h_intl_surf, 
                        t_dst.i4_left,
                        t_dst.i4_top,
                        i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                        i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                        RECT_W(&t_dst),
                        RECT_H(&t_dst),
                        255,
                        FALSE,
                        &t_clr_k,
                        &t_clr_k,
                        0);
                }
                else
                {
                    PRINT_LINE_DBG;
                    
                    gl_dfb_surface_compose(
                        h_surf, 
                        _h_intl_surf, 
                        t_dst.i4_left,
                        t_dst.i4_top,
                        i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                        i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                        RECT_W(&t_dst),
                        RECT_H(&t_dst),
                        pt_fg_clr->a,
                        GL_SRC_OVER,
                        0);
                    
                }
                PRINT_LINE_DBG;
                i4_ret = gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);
                /*if (RMR_OK == i4_ret)
                {
                    b_has_cmd = TRUE;
                }
                else
                {
                    i4_ret = GLR_DRV_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Driver error (compose fg-text rect) failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }*/
                PRINT_LINE_DBG;
            }
        }

        /* step 4: if there are any commands sent, flush them*/
       /* if (b_has_cmd)
        {
            _flush_cmd(1, &(pt_surf->h_signal), GL_SYNC);
            b_locked = FALSE;
        }*/
        
        if (i >= i4_strlen)
        {
            break;
        }

        i4_base_x += i4_dst_x + i4_skip_x;
        i4_base_y += i4_dst_y + 0;

        i4_bg_adj_x = i4_effect_offset;
        i4_prev_index = i;

        ui2_num_run++;
    }
    PRINT_LINE_DBG;

    if ((FER_OK == x_fe_get_style(h_fnt, &t_fnt_style)) &&
        (t_fnt_style & FE_FNT_STYLE_UNDERLINE))
    {
        INT16 i2_ul_pos, i2_ul_thickness;
        PRINT_LINE_DBG;

        if ((FER_OK == x_fe_get_underline_thickness(h_fnt, &i2_ul_thickness)) &&
            (FER_OK == x_fe_get_underline_position(h_fnt, &i2_ul_pos)))
        {PRINT_LINE_DBG;
            GL_RECT_T t_ul;

            {PRINT_LINE_DBG;
                SET_RECT_BY_SIZE(
                    &t_ul, 
                    t_rc_str.i4_left, 
                    i4_y + (INT32) (i2_height + i2_descend - i2_ul_pos),
                    RECT_W(&t_rc_str), 
                    i2_ul_thickness);
            }

            INTERSECT_RECT(&t_ul, &t_ul, &t_clip);
            PRINT_LINE_DBG;

            if (!RECT_EMPTY(&t_ul))
            {
                /* check if it's in the subset mode */
                if (pt_aux_data->aui2_display_idx == NULL && pt_aux_data->pv_skip_line == NULL)
                {
                    PRINT_LINE_DBG;
                    gl_dfb_locked_fill_rect(
                        t_lock_info.e_clrmode,
                        t_lock_info.at_subplane[0].pv_bits,
                        t_lock_info.at_subplane[0].ui4_pitch,
                        t_ul.i4_left,
                        t_ul.i4_top,
                        RECT_W(&t_ul),
                        RECT_H(&t_ul),
                        gl_dfb_compose_packed_color(pt_fg_clr, t_lock_info.e_clrmode));
                    PRINT_LINE_DBG;

                    /*gl_surface_flush(pt_surf, 0);*/
                }
                 else if (pt_aux_data->pv_skip_line != NULL)
                {
                    {
                        INT32       i4_i;
                        UINT32      ui4_color = gl_dfb_compose_packed_color(pt_fg_clr, t_lock_info.e_clrmode);
                        GL_RECT_T   t_out;

                        for (i4_i = 0; i4_i < i4_strlen; i4_i++)
                        {
                            if (i4_i + 1 < i4_strlen &&
                                pt_char_info[i4_i + 1].i4_width != 0 &&
                                pt_char_info[i4_i].i4_width != 0)
                            {
                                pt_char_info[i4_i].i4_width = pt_char_info[i4_i + 1].i4_begin - pt_char_info[i4_i].i4_begin;
                            }

                            if (i4_i == 0)
                            {
                                pt_char_info[i4_i].i4_width += (pt_char_info[i4_i].i4_begin - t_ul.i4_left);
                                pt_char_info[i4_i].i4_begin = t_ul.i4_left;
                            }

                            SET_RECT_BY_SIZE(&t_out, pt_char_info[i4_i].i4_begin, t_ul.i4_top, pt_char_info[i4_i].i4_width, RECT_H(&t_ul));
                            INTERSECT_RECT(&t_out, &t_out, &t_clip);

                            if (!RECT_EMPTY(&t_out))
                            {
                                gl_dfb_locked_fill_rect(
                                    t_lock_info.e_clrmode,
                                    t_lock_info.at_subplane[0].pv_bits,
                                    t_lock_info.at_subplane[0].ui4_pitch,
                                    t_out.i4_left,
                                    t_out.i4_top,
                                    RECT_W(&t_out),
                                    RECT_H(&t_out),
                                    ui4_color);
                            }
                        }

                        /*gl_surface_flush(pt_surf, 0);*/
                    }
                #if 0
                    else /* if (pt_aux_data->e_str_direction == GL_STR_TOP_2_BOTTOM) */
                    {
                    }
                #endif
                }
            }
        }
    }
    i4_ret = GLR_OK;

_DRAW_USTRING_EX_EXIT_1:
    
    PRINT_LINE_DBG;
    if (b_locked)
    {
        PRINT_LINE_DBG;
        /* unlock internal buffer */
        /*_flush_cmd(0, NULL, 0);*/
    }
    
    gl_dfb_surface_unlock(h_surf, NULL);

_DRAW_USTRING_EX_EXIT_2:
    PRINT_LINE_DBG;

    gl_dfb_obj_unlock(pt_surf);

    PRINT_LINE_DBG;
    if (pt_char_info)   
    { 
        x_mem_free(pt_char_info);    
    }
#if 0
    DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &i4_ret);
#endif
    return (i4_ret);
}

INT32 x_gl_surface_draw_drcs_string(
    GL_HSURFACE_T           h_surf,
    INT32                   i4_x,
    INT32                   i4_y,
    SM_TTX_WGL_DRCS_PTU_T   at_drcs_str[],
    UINT16                  ui2_str_len,
    UINT8                   ui1_width,
    UINT8                   ui1_height,
    GL_DRCS_INFO_T*         pt_info)
{
    INT32                   i4_ret;
    GL_DFB_SURFACE_T        *pt_surf = NULL;
    GL_LOCK_INFO_T          t_lock_info;
    
    CHECK_VALID(at_drcs_str, GLR_INV_ARG);
    CHECK_VALID(i4_x >= 0 && i4_y >= 0, GLR_INV_ARG);
    CHECK_VALID(h_surf != NULL_HANDLE, GLR_INV_ARG);

    /* get object pointer */
    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

    i4_ret = GLR_OK;

    if (pt_surf == NULL)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        gl_dfb_obj_unlock(pt_surf);
        return (i4_ret);
    }

    i4_ret = gl_dfb_surface_lock(h_surf, NULL, GL_SYNC, &t_lock_info);

    if (i4_ret == GLR_OK)
    {
        UINT32          ui4_dst_w = t_lock_info.at_subplane[0].ui4_width;
        UINT32          ui4_dst_h = t_lock_info.at_subplane[0].ui4_height;
        DRCS_PROFILE    e_profile = pt_info->e_profile;

        if (e_profile == DRCS_TTX_25)
        {
            UINT8       ui1_ch_w, ui1_ch_h;

            ui1_ch_w = 12;
            ui1_ch_h = 10;
            
            if (((UINT32)(i4_x + ui2_str_len * ui1_ch_w) > ui4_dst_w) ||
                ((UINT32)(i4_y + ui1_ch_h * 2) > ui4_dst_h))
            {
                i4_ret = GLR_INV_ARG;

                gl_dfb_surface_unlock(h_surf, NULL);
                gl_dfb_obj_unlock(pt_surf);
                return i4_ret;
            }

            if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_CLUT8)
            {
                UINT16      ui2_i;
                UINT8       ui1_j, ui1_k;
                UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT8);
                UINT8       *pui1_dst = (UINT8 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;

                for (ui1_j = 0; ui1_j < ui1_ch_h; ui1_j++)
                {
                    UINT8       *pui1_row, *pui1_row2;

                    pui1_row  = pui1_dst;
                    pui1_row2 = pui1_dst + ui4_pitch;

                    for (ui2_i = 0; ui2_i < ui2_str_len; ui2_i++)
                    {
                        UINT8               ui1_odd_byte, ui1_even_byte;
                        UINT8               *pui1_drcs_ch;
                        GL_COLOR_T          *pt_fg_clr, *pt_bg_clr;

                        pui1_drcs_ch = &at_drcs_str[ui2_i].aui1_ptu[2 * ui1_j];
                        pt_fg_clr    = &at_drcs_str[ui2_i].t_fg_clr;
                        pt_bg_clr    = &at_drcs_str[ui2_i].t_bg_clr;

                        ui1_even_byte = *pui1_drcs_ch;
                        ui1_odd_byte  = *(pui1_drcs_ch + 1);
                        
                        for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                        {
                            UINT8       ui1_pixel;
                            UINT8       ui1_color;
                            GL_COLOR_T  *pt_color;
                            
                            if (ui1_k < (UINT8)(ui1_ch_w / 2))
                            {
                                ui1_pixel = ((ui1_even_byte >> (6 - 1 - ui1_k)) & 0x1);
                            }
                            else
                            {
                                ui1_pixel = ((ui1_odd_byte >> (12 - 1 - ui1_k)) & 0x1);
                            }

                            pt_color = (ui1_pixel)? pt_fg_clr: pt_bg_clr;

                            ui1_color = (UINT8)gl_dfb_compose_packed_color(pt_color, GL_COLORMODE_ARGB_CLUT8);
                            
                            *(pui1_row++)  = ui1_color;
                            *(pui1_row2++) = ui1_color;
                        }
                    }

                    pui1_dst += ui4_pitch * 2;
                }
            }
            else if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D4444)
            {
                UINT16      ui2_i;
                UINT8       ui1_j, ui1_k;
                UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT16);
                UINT16      *pui2_dst = (UINT16 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;

                for (ui1_j = 0; ui1_j < ui1_ch_h; ui1_j++)
                {
                    UINT16      *pui2_row, *pui2_row2;

                    pui2_row  = pui2_dst;
                    pui2_row2 = pui2_dst + ui4_pitch;
                    
                    for (ui2_i = 0; ui2_i < ui2_str_len; ui2_i++)
                    {
                        UINT8               ui1_odd_byte, ui1_even_byte;
                        UINT8               *pui1_drcs_ch;
                        GL_COLOR_T          *pt_fg_clr, *pt_bg_clr;

                        pui1_drcs_ch = &at_drcs_str[ui2_i].aui1_ptu[2 * ui1_j];
                        pt_fg_clr    = &at_drcs_str[ui2_i].t_fg_clr;
                        pt_bg_clr    = &at_drcs_str[ui2_i].t_bg_clr;

                        ui1_even_byte = *pui1_drcs_ch;
                        ui1_odd_byte  = *(pui1_drcs_ch + 1);

                        for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                        {
                            UINT8       ui1_pixel;
                            UINT16      ui2_color;
                            GL_COLOR_T  *pt_color;
                            
                            if (ui1_k < (UINT8)(ui1_ch_w / 2))
                            {
                                ui1_pixel = ((ui1_even_byte >> (6 - 1 - ui1_k)) & 0x1);
                            }
                            else
                            {
                                ui1_pixel = ((ui1_odd_byte >> (12 - 1 - ui1_k)) & 0x1);
                            }

                            pt_color = (ui1_pixel)? pt_fg_clr: pt_bg_clr;

                            ui2_color = (UINT16)gl_dfb_compose_packed_color(pt_color, GL_COLORMODE_ARGB_D4444);
                            
                            *(pui2_row++)  = ui2_color;
                            *(pui2_row2++) = ui2_color;
                        }
                    }

                    pui2_dst += ui4_pitch * 2;
                }
            }
            else if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D8888)
            {
                UINT16      ui2_i;
                UINT8       ui1_j, ui1_k;
                UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT32);
                UINT32      *pui4_dst = (UINT32 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;

                for (ui1_j = 0; ui1_j < ui1_ch_h; ui1_j++)
                {
                    UINT32      *pui4_row, *pui4_row2;

                    pui4_row  = pui4_dst;
                    pui4_row2 = pui4_dst + ui4_pitch;
                    
                    for (ui2_i = 0; ui2_i < ui2_str_len; ui2_i++)
                    {
                        UINT8               ui1_odd_byte, ui1_even_byte;
                        UINT8               *pui1_drcs_ch;
                        GL_COLOR_T          *pt_fg_clr, *pt_bg_clr;

                        pui1_drcs_ch = &at_drcs_str[ui2_i].aui1_ptu[2 * ui1_j];
                        pt_fg_clr    = &at_drcs_str[ui2_i].t_fg_clr;
                        pt_bg_clr    = &at_drcs_str[ui2_i].t_bg_clr;

                        ui1_even_byte = *pui1_drcs_ch;
                        ui1_odd_byte  = *(pui1_drcs_ch + 1);

                        for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                        {
                            UINT8       ui1_pixel;
                            UINT32      ui4_color;
                            GL_COLOR_T  *pt_color;
                            
                            if (ui1_k < (UINT8)(ui1_ch_w / 2))
                            {
                                ui1_pixel = ((ui1_even_byte >> (6 - 1 - ui1_k)) & 0x1);
                            }
                            else
                            {
                                ui1_pixel = ((ui1_odd_byte >> (12 - 1 - ui1_k)) & 0x1);
                            }

                            pt_color = (ui1_pixel)? pt_fg_clr: pt_bg_clr;

                            ui4_color = (UINT32)gl_dfb_compose_packed_color(pt_color, GL_COLORMODE_ARGB_D8888);
                            
                            *(pui4_row++)  = ui4_color;
                            *(pui4_row2++) = ui4_color;
                        }
                    }

                    pui4_dst += ui4_pitch * 2;
                }
            }
            
        }
    }
    else
    {
        GL_DFB_TRACE(("{GRAPHIC} lock surface failed (%d). %s line: %d.\n", i4_ret, __FUNCTION__, __LINE__));
    }

    gl_dfb_surface_unlock(h_surf, NULL);
    gl_dfb_obj_unlock(pt_surf);

    return i4_ret;
}

INT32 x_gl_surface_draw_ttx_string(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    GL_TTX_COLOR_T*     pt_ttx_clr,
    GL_TTX_STR_SIZE_T   e_ttx_size)
{
    INT32               i4_ret;
    GL_DFB_SURFACE_T    *pt_surf = NULL;
    GL_LOCK_INFO_T      t_lock_info;
    
    CHECK_VALID(w2s_str, GLR_INV_ARG);
    CHECK_VALID(i4_x >= 0 && i4_y >= 0, GLR_INV_ARG);
    CHECK_VALID(h_surf != NULL_HANDLE, GLR_INV_ARG);
    CHECK_VALID(w2s_str != NULL, GLR_INV_ARG);
    CHECK_VALID(ui2_strlen <= GL_MAX_TTX_STR_LEN, GLR_INV_ARG);
    CHECK_VALID(h_fnt != NULL_HANDLE, GLR_INV_ARG);
    CHECK_VALID(pt_ttx_clr != NULL, GLR_INV_ARG);

    /* get object pointer */
    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

    i4_ret = GLR_OK;

    if (pt_surf == NULL)
    {
        GL_DFB_TRACE(("{GRAPHIC} ERR: Invalid surface handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        gl_dfb_obj_unlock(pt_surf);
        return (i4_ret);
    }

    i4_ret = gl_dfb_surface_lock(h_surf, NULL, GL_SYNC, &t_lock_info);

    if (i4_ret == GLR_OK)
    {
        UINT32          ui4_dst_w = t_lock_info.at_subplane[0].ui4_width;
        UINT32          ui4_dst_h = t_lock_info.at_subplane[0].ui4_height;
        UINT16          ui2_i;
        FE_GLYPH_T      t_glyph;
        UINT8           ui1_ch_w, ui1_ch_h, ui1_w_scale, ui1_h_scale;
        BOOL            b_double_w, b_double_h;
        UINT32          ui4_pitch_offset;
        UINT8           ui1_j, ui1_t;

        /* load characters */
        t_glyph.e_render_mode = FE_RENDER_MODE_MONO;
        
        ui1_ch_w = 12;
        ui1_ch_h = 10;
        ui1_j    = 0;

        b_double_w = (e_ttx_size & GL_TTX_STR_DBL_WIDTH )? TRUE: FALSE;
        b_double_h = (e_ttx_size & GL_TTX_STR_DBL_HEIGHT)? TRUE: FALSE;

        ui1_w_scale = (UINT8)((b_double_w)? 2: 1);
        ui1_h_scale = (UINT8)((b_double_h)? 2: 1);

        if (b_double_h && (e_ttx_size & GL_TTX_STR_UPPER))
        {
            ui1_j    = 0;
            ui1_ch_h = 5;
        }
        else if (b_double_h && (e_ttx_size & GL_TTX_STR_LOWER))
        {
            ui1_j    = 5;
            ui1_ch_h = 10;
        }
        
        if (((UINT32)(i4_x + ui2_strlen * ui1_ch_w * ui1_w_scale) > ui4_dst_w) ||
            ((UINT32)(i4_y + ui1_ch_h * 2 * ui1_h_scale) > ui4_dst_h))
        {
            i4_ret = GLR_INV_ARG;

            gl_dfb_surface_unlock(h_surf, NULL);
            gl_dfb_obj_unlock(pt_surf);
            return i4_ret;
        }

        ui4_pitch_offset = (UINT32)((b_double_h)? 2: 1);

        if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_CLUT8)
        {
            UINT8       ui1_k;
            UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT8);
            UINT8       *pui1_row = NULL, *pui1_row2 = NULL, *pui1_row3 = NULL, *pui1_row4 = NULL;
            
            for (ui2_i = 0; ui2_i < ui2_strlen; ui2_i++)
            {
                UINT8           *pui1_dst = (UINT8 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;
                GL_COLOR_T      *apt_clr[2];

                apt_clr[1]  = &pt_ttx_clr[ui2_i].t_fg_clr;
                apt_clr[0]  = &pt_ttx_clr[ui2_i].t_bg_clr;
            
                i4_ret = x_fe_load_char(h_fnt, w2s_str[ui2_i], &t_glyph);

                if (i4_ret != FER_OK)
                {
                    GL_DFB_TRACE(("{GRAPHIC} Load Character failed (%d). %s line: %d.\n", i4_ret, __FUNCTION__, __LINE__));
                }                
				if (t_glyph.t_bmp.pui1_buf == NULL)
				{
					continue;
				}

                for (ui1_t = ui1_j  ; ui1_t < ui1_ch_h; ui1_t++)
                {
                    UINT16          ui2_word; 
                    UINT8           *pui1_bitmap;

                    if (b_double_w)
                    {
                        pui1_row  = pui1_dst + ui2_i * ui1_ch_w * 2;
                    }
                    else
                    {
                        pui1_row  = pui1_dst + ui2_i * ui1_ch_w;
                    }

                    pui1_row2 = pui1_row + ui4_pitch;

                    if (b_double_h)
                    {
                        pui1_row3 = pui1_row2 + ui4_pitch;
                        pui1_row4 = pui1_row3 + ui4_pitch;
                    }

                    pui1_bitmap = t_glyph.t_bmp.pui1_buf + (ui1_t * 2) * 2;
                    ui2_word    = (UINT16)(((*pui1_bitmap) << 8) | (*(pui1_bitmap + 1)));
                    
                    for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                    {
                        UINT8       ui1_color;
                        GL_COLOR_T  *pt_color;

                        pt_color  = apt_clr[((ui2_word & 0x8000) != 0)];
                        ui1_color = pt_color->u3.index;
                        ui2_word  = ui2_word << 1;
                        
                        *(pui1_row++)  = ui1_color;
                        *(pui1_row2++) = ui1_color;

                        if (b_double_h)
                        {
                            *(pui1_row3++) = ui1_color;
                            *(pui1_row4++) = ui1_color;
                        }

                        if (b_double_w)
                        {
                            *(pui1_row++)  = ui1_color;
                            *(pui1_row2++) = ui1_color;

                            if (b_double_h)
                            {
                                *(pui1_row3++) = ui1_color;
                                *(pui1_row4++) = ui1_color;
                            }
                        }
                    }
                    pui1_dst += (ui4_pitch << ui4_pitch_offset);
                }
            }
        }
        else if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D4444)
        {
            UINT8       ui1_k;
            UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT16);
            UINT16      *pui2_row = NULL, *pui2_row2 = NULL, *pui2_row3 = NULL, *pui2_row4 = NULL;
             for (ui2_i = 0; ui2_i < ui2_strlen; ui2_i++)
            {
                UINT16          *pui2_dst = (UINT16 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;
                GL_COLOR_T      *apt_clr[2];

                apt_clr[1]  = &pt_ttx_clr[ui2_i].t_fg_clr;
                apt_clr[0]  = &pt_ttx_clr[ui2_i].t_bg_clr;

                i4_ret = x_fe_load_char(h_fnt, w2s_str[ui2_i], &t_glyph);

                if (i4_ret != FER_OK)
                {
                    GL_DFB_TRACE(("{GRAPHIC} Load Character failed (%d). %s line: %d.\n", i4_ret, __FUNCTION__, __LINE__));
                }
				
    
				if (t_glyph.t_bmp.pui1_buf == NULL)
				{
					continue;
				}

                for ( ui1_t = ui1_j ; ui1_t < ui1_ch_h; ui1_t++)
                {
                    UINT16          ui2_word; 
                    UINT8           *pui1_bitmap;

                    if (b_double_w)
                    {
                        pui2_row  = pui2_dst + ui2_i * ui1_ch_w * 2;
                    }
                    else
                    {
                        pui2_row  = pui2_dst + ui2_i * ui1_ch_w;
                    }

                    pui2_row2 = pui2_row + ui4_pitch;

                    if (b_double_h)
                    {
                        pui2_row3 = pui2_row2 + ui4_pitch;
                        pui2_row4 = pui2_row3 + ui4_pitch;
                    }

                    pui1_bitmap = t_glyph.t_bmp.pui1_buf + (ui1_t * 2) * 2;
                    ui2_word    = (UINT16)(((*pui1_bitmap) << 8) | (*(pui1_bitmap + 1)));                  

                    for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                    {
                        UINT16      ui2_color;
                        GL_COLOR_T  *pt_color;
                        
                        pt_color  = apt_clr[((ui2_word & 0x8000) != 0)];
                        ui2_color = GL_COLOR_ARGB4444(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b);
                        ui2_word  = ui2_word << 1;
                        
                        *(pui2_row++)  = ui2_color;
                        *(pui2_row2++) = ui2_color;

                        if (b_double_h)
                        {
                            *(pui2_row3++) = ui2_color;
                            *(pui2_row4++) = ui2_color;
                        }

                        if (b_double_w)
                        {
                            *(pui2_row++)  = ui2_color;
                            *(pui2_row2++) = ui2_color;

                            if (b_double_h)
                            {
                                *(pui2_row3++) = ui2_color;
                                *(pui2_row4++) = ui2_color;
                            }
                        }
                    }
                    pui2_dst += (ui4_pitch << ui4_pitch_offset);
                }
            }
        }
        else if (t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D8888)
        {
            UINT8       ui1_k;
            UINT32      ui4_pitch = t_lock_info.at_subplane[0].ui4_pitch / sizeof(UINT32);
            UINT32      *pui4_row = NULL, *pui4_row2 = NULL, *pui4_row3 = NULL, *pui4_row4 = NULL;
             for (ui2_i = 0; ui2_i < ui2_strlen; ui2_i++)
            {
                UINT32      *pui4_dst = (UINT32 *)t_lock_info.at_subplane[0].pv_bits + i4_y * ui4_pitch + i4_x;
                GL_COLOR_T      *apt_clr[2];
                apt_clr[1]  = &pt_ttx_clr[ui2_i].t_fg_clr;
                apt_clr[0]  = &pt_ttx_clr[ui2_i].t_bg_clr;

                i4_ret = x_fe_load_char(h_fnt, w2s_str[ui2_i], &t_glyph);
                if (i4_ret != FER_OK)
                {
                    GL_DFB_TRACE(("{GRAPHIC} Load Character failed (%d). %s line: %d.\n", i4_ret, __FUNCTION__, __LINE__));
                }
    
				if (t_glyph.t_bmp.pui1_buf == NULL)
				{
					continue;
				}

                for ( ui1_t = ui1_j ; ui1_t < ui1_ch_h; ui1_t++)
                {
                    UINT16          ui2_word; 
                    UINT8           *pui1_bitmap;

                    if (b_double_w)
                    {
                        pui4_row  = pui4_dst + ui2_i * ui1_ch_w * 2;
                    }
                    else
                    {
                        pui4_row  = pui4_dst + ui2_i * ui1_ch_w;
                    }

                    pui4_row2 = pui4_row + ui4_pitch;

                    if (b_double_h)
                    {
                        pui4_row3 = pui4_row2 + ui4_pitch;
                        pui4_row4 = pui4_row3 + ui4_pitch;
                    }

                    pui1_bitmap = t_glyph.t_bmp.pui1_buf + (ui1_t * 2) * 2;
                    ui2_word    = (UINT16)(((*pui1_bitmap) << 8) | (*(pui1_bitmap + 1)));                  
                    for (ui1_k = 0; ui1_k < ui1_ch_w; ui1_k++)
                    {
                        UINT32      ui4_color;
                        GL_COLOR_T  *pt_color;
                        
                        pt_color  = apt_clr[((ui2_word & 0x8000) != 0)];
                        ui4_color = GL_COLOR_ARGB8888(pt_color->a, pt_color->u1.r, pt_color->u2.g, pt_color->u3.b);
                        ui2_word  = ui2_word << 1;
                        
                        *(pui4_row++)  = ui4_color;
                        *(pui4_row2++) = ui4_color;

                        if (b_double_h)
                        {
                            *(pui4_row3++) = ui4_color;
                            *(pui4_row4++) = ui4_color;
                        }

                        if (b_double_w)
                        {
                            *(pui4_row++)  = ui4_color;
                            *(pui4_row2++) = ui4_color;

                            if (b_double_h)
                            {
                                *(pui4_row3++) = ui4_color;
                                *(pui4_row4++) = ui4_color;
                            }
                        }
                    }
                    pui4_dst += (ui4_pitch << ui4_pitch_offset);
                }
            }
        }
    }
    else
    {
        GL_DFB_TRACE(("{GRAPHIC} lock surface failed (%d). %s line: %d.\n", i4_ret, __FUNCTION__, __LINE__));
    }

    gl_dfb_surface_unlock(h_surf, NULL);

    gl_dfb_obj_unlock(pt_surf);
   
    return i4_ret;
}

INT32 x_gl_surface_draw_uchars(
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
    HANDLE_TYPE_T e_type;
    
/*    CHECK_VALID(gl_is_init(), GLR_NOT_INIT);*/

    CHECK_VALID(pt_fg_clr, GLR_INV_ARG);
    CHECK_VALID(aw2_chars && ui2_len > 0, GLR_INV_ARG);
    CHECK_VALID(
        HR_OK == x_handle_get_type(h_fnt, &e_type) && e_type == FE_FNT_OBJ,
        GLR_INV_HANDLE);
    PRINT_LINE_DBG;

    return (_draw_char_array(
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

INT32 x_gl_surface_draw_ustring(
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
    HANDLE_TYPE_T   e_type;
    INT32           i4_strlen;
    
/*    CHECK_VALID(gl_is_init(), GLR_NOT_INIT);*/

    CHECK_VALID(pt_fg_clr, GLR_INV_ARG);
    CHECK_VALID(w2s_str && ui2_strlen > 0, GLR_INV_ARG);
    CHECK_VALID(
        HR_OK == x_handle_get_type(h_fnt, &e_type) && e_type == FE_FNT_OBJ,
        GLR_INV_HANDLE);

    i4_strlen = x_uc_w2s_strlen(w2s_str);
    i4_strlen = MIN(i4_strlen, (INT32) ui2_strlen);

    CHECK_VALID(i4_strlen > 0, GLR_INV_ARG);

    return (_draw_char_array(
        h_surf,
        i4_x,
        i4_y,
        w2s_str,
        (UINT16) i4_strlen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_rc_str));
}

INT32 x_gl_surface_draw_ustring_ex(
    GL_HSURFACE_T       h_surf,
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
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode)
{
    INT32                       i4_base_x, i4_base_y;
    INT32                       i4_adj_x, i4_adj_y;
    INT32                       i4_dst_x, i4_dst_y;
    INT32                       i4_prv_dst_x, i4_prv_dst_y;
    INT32                       i4_offset_x, i4_offset_y;
    INT32                       i, j, i4_skip_x, i4_strlen, i4_ret;
    BOOL                        b_has_kern;
    CHAR_CODE                   t_ch, t_ch_prv;
    FE_GLYPH_T                  t_glyph;
    INT16                       i2_descend, i2_height, i2_width;
    /*HANDLE_TYPE_T               e_type;*/
    GL_DFB_SURFACE_T*           pt_surf;
    GL_LOCK_INFO_T              t_lock_info;
    GL_RECT_T                   t_clip, t_dst, t_src, t_stripe = {0}, t_rc_str;
    INT32                       i4_skiplen;
    UINT32                      ui4_buf_w, ui4_buf_h, ui4_buf_pitch;
    VOID*                       pv_buf;
    /*GFX_ALPHA_COMPOSITION_T     t_compose;
    GFX_TRANSPARENT_BITBLT_T    t_tp_blt;
    GFX_FILL_T                  t_fill;*/
    UINT32                      ui4_bg_color = 0;
    INT32                       i4_bg_ys = 0, i4_bg_ye = 0;
    INT32                       i4_adv_x, i4_adv_y;
    BOOL                        b_has_cmd, b_locked, b_null_glyph;
    FE_FNT_STYLE                t_fnt_style;
    UINT8                       ui1_text_bg, ui1_shf;
    FE_RENDER_MODE_T            e_render_mode;
    UINT16                      ui2_num_run;
    _CHAR_POS_INFO              *pt_char_info;  
    INT32                       i4_prev_index = 0;

    UTF16_T                     *w2s_str, *w2s_skip;

    /* patch for CR2073 */
    GL_RECT_T                   t_rc_buf;
    /* patch for CR2073 */

    /*CHECK_VALID(gl_is_init(), GLR_NOT_INIT);*/
    PRINT_LINE_DBG;

//    return 0;

    w2s_str  = (UTF16_T *)pv_str;
    w2s_skip = (UTF16_T *)pv_skip;

    CHECK_VALID(pt_fg_clr, GLR_INV_ARG);
    CHECK_VALID(pv_str && (ui2_strlen > 0), GLR_INV_ARG);
    CHECK_VALID(
        (pv_skip && (ui2_skiplen > 0)) || (!pv_skip && (ui2_skiplen == 0)), 
        GLR_INV_ARG);

    /*CHECK_VALID(FER_OK == x_fe_get_encoding(h_fnt, &e_encoding), GLR_INV_HANDLE);*/

    //i4_strlen = x_fe_strlen((UINT8 *)pv_str, e_encoding, (SIZE_T)ui2_strlen);
    i4_strlen = x_uc_w2s_strlen(w2s_str);
    
    /*printf("i4_strlen (src) is %d\n", i4_strlen);*/
    
    i4_strlen = MIN(i4_strlen, (INT32) ui2_strlen);
    
    CHECK_VALID(i4_strlen > 0, GLR_INV_ARG);
    ui2_strlen = (UINT16)ui2_strlen;
    b_locked = FALSE;

    /* check string of ignorance */
    if (pv_skip)
    {
        //i4_skiplen = x_fe_strlen((UINT8 *)pv_skip, e_encoding, (SIZE_T)ui2_skiplen);
        i4_skiplen = x_uc_w2s_strlen(w2s_skip);
        i4_skiplen = MIN(i4_skiplen, (INT32) ui2_skiplen);
    }
    else
    {
        i4_skiplen = 0;
    }
    ui2_skiplen = (UINT16)i4_skiplen;
    pt_char_info = NULL;    

    PRINT_LINE_DBG;

    pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

    i4_ret = GLR_OK;
    
    /* find clipping area */
    /*t_clip = pt_surf->t_clip_rect;*/
    gl_surface_get_clip_area(pt_surf, &t_clip);
    if (RECT_EMPTY(&t_clip) || i4_x >= t_clip.i4_right || i4_y >= t_clip.i4_bottom)
    {

        PRINT_LINE_DBG;
        GL_DFB_TRACE("{GRAPHIC} The surface clip area is empty [%d,%d,%d,%d].\r\n\r\n",
            t_clip.i4_left,
            t_clip.i4_top,
            t_clip.i4_right,
            t_clip.i4_bottom);
        goto _DRAW_USTRING_EX_EXIT_2;
    }
        
    /* get surface memory raw pointer */
    /*
    i4_ret = gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);

    PRINT_LINE_DBG;
    if (GL_FAILED(i4_ret))
    {

        GL_DFB_TRACE(("{GRAPHIC} ERR: Locking surface failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_2;
    }
    */

	/* check the rendering offset */
    i4_ret = x_fe_get_rendering_offset(h_fnt, &i4_offset_y);

    if (i4_ret == FER_OK)
    {
        i4_y += i4_offset_y;
    }
    
    if (FER_OK != x_fe_get_max_advance(h_fnt, &i2_width, &i2_height) ||
        FER_OK != x_fe_get_height(h_fnt, &i2_height) ||
        FER_OK != x_fe_get_descender(h_fnt, &i2_descend))
    {

        i4_ret = GLR_FE_ERROR;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting height/descender failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_1;
    }
        

    SET_RECT(&t_rc_str, 0x7fffffff, 0x7fffffff, -0x7fffffff, -0x7fffffff);

    if (pt_bg_clr)
    {
        ui4_bg_color = gl_dfb_compose_packed_color(pt_bg_clr, pt_surf->t_desc.e_clrmode);
    }

    /* draw each char onto the bitmap */
    i4_base_x  = i4_x;
    i4_base_y  = i4_y;
    t_ch_prv   = 0;
    b_has_kern = x_fe_has_kerning(h_fnt);

    i4_bg_ys = MAX(i4_bg_y, t_clip.i4_top);
    i4_bg_ye = MIN(i4_bg_y + ui2_bg_h, t_clip.i4_bottom);
    if (i4_bg_ys >= i4_bg_ye)
    {
        ui2_bg_h = 0;
    }
    else
    {
        ui2_bg_h = (UINT16) (i4_bg_ye - i4_bg_ys);
    }

    /* [20808] patch for the (i2_height + i2_descend) / 4 */
    ui4_buf_h = MAX((UINT32) (i2_height + (i2_height + i2_descend) / 4), (UINT32) ui2_bg_h);
#if 0
    if(ui4_buf_h > MAX_BUF_HEIGHT)
    {
        GL_DFB_TRACE("Waring:Outofbufbounds:H %d\n", ui4_buf_h);
        ui4_buf_h = MAX_BUF_HEIGHT;
    } 
#endif
    ui4_buf_w = (UINT32)ABS((i4_x + RECT_W(&t_clip)));
    
    PRINT_LINE_DBG;

    /* check edge effect */
    if (FER_OK != x_fe_get_style(h_fnt, &t_fnt_style))
    {PRINT_LINE_DBG;
        i4_ret = GLR_FE_ERROR;
        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting style failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
        goto _DRAW_USTRING_EX_EXIT_1;
    }
    if (!(t_fnt_style & FE_FNT_STYLE_EDGE_EFFECT))
    {
        pt_edge_clr = NULL;
    }

    gl_cal_buf_param_by_height(
        pt_surf->t_desc.e_clrmode, 
        ui4_buf_h, 
        &pv_buf, 
        &ui4_buf_pitch, 
        &ui4_buf_w);

    SET_RECT_BY_SIZE(
        &t_rc_buf,
        0,
        0,
        ui4_buf_w,
        ui4_buf_h);
    
    b_locked = FALSE;

    i = 0;
    PRINT_LINE_DBG;

    ui1_shf = _shf_bit(pt_surf->t_desc.e_clrmode);
    
    ui1_text_bg = 
        (GL_IS_PALETTE_MODE(pt_surf->t_desc.e_clrmode)) ? (pt_fg_clr->u3.index + 1) : 0;

    /* set up font rendering mode */
    if (GL_IS_PALETTE_MODE(pt_surf->t_desc.e_clrmode) ||
        GL_COLORMODE_RGB_D565 == pt_surf->t_desc.e_clrmode)
    {
        e_render_mode = FE_RENDER_MODE_MONO;
    }
    else
    {
        e_render_mode = FE_RENDER_MODE_NORMAL;
    }
    
    ui2_num_run = 0;
    i4_prev_index = 0;

    while (1)
    {
        INT32       i4_stripe_start = 0, i4_stripe_end = 0;
        INT32       i4_effect_offset = 0;
        
        i4_prv_dst_x = i4_dst_x = i4_prv_dst_y = 0;
        i4_dst_y  = 0;
        i4_skip_x = 0;
        b_has_cmd = FALSE;

        if (!b_locked)
        {
            /*gl_lock_buf();*/
    #if 0            
            /* clean internal buffer */
            x_memset(pv_buf, ui1_text_bg, ui4_buf_pitch * ui4_buf_h);
    #endif
            b_locked = TRUE;
        }
        PRINT_LINE_DBG;
            
        for (j = 0; i < i4_strlen; j++)
        {
            t_ch = w2s_str[i];
            /* MHF Linux - Chun Jiang for fix \t display*/
            if(/*((INT16 *)pv_str)*/t_ch == 0x9)
            {
                i ++;
                break;
            }
            
            /* load character */
            //t_ch = x_fe_get_char(pv_str, i, e_encoding);
            
            PRINT_LINE_DBG;
            if (pv_skip && _is_skip_char_no_encoding(t_ch, pv_skip, ui2_skiplen))
            {   /* handle skip character */
                i++;
                t_ch_prv = 0;
                i4_skip_x = (INT32) ui2_skip_w;
                break;
            }
            else
            {
                /* get character advance */
                i4_adv_x = 0;
                i4_adv_y = 0;
                if (t_ch == FE_CC_LEADING_CHAR ||
                    (t_ch >= FE_UTF16_SPACE_USER_START && t_ch <= FE_UTF16_SPACE_USER_END))
                {
                        /* fake character */
                        t_ch = (CHAR_CODE)0x20;
                }

                
                PRINT_LINE_DBG;
                i4_ret = x_fe_get_glyph_advance(
                    h_fnt,
                    t_ch,
                    &i4_adv_x,
                    &i4_adv_y);
                
                if (i4_ret != FER_OK)
                {PRINT_LINE_DBG;
                    i4_ret = GLR_FE_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Getting glyph advance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                    goto _DRAW_USTRING_EX_EXIT_1;
                }
                    
                PRINT_LINE_DBG;
                /* load glyph bitmap */
                t_glyph.e_render_mode = e_render_mode;
                i4_ret = x_fe_load_char(h_fnt, t_ch, &t_glyph);
                
                PRINT_LINE_DBG;
                if (i4_ret != FER_OK)
                {
                    PRINT_LINE_DBG;
                    i4_ret = GLR_FE_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Loading character [%d] failed (%d) at L%d\r\n\r\n", t_ch, i4_ret, __LINE__));
                    goto _DRAW_USTRING_EX_EXIT_1;
                }
                
                /* Thai representation */
                if (CHECK_THAI_PRESENTATION((CHAR_CODE)t_ch_prv, (CHAR_CODE)t_ch))
                {
                    t_glyph.i4_top = t_glyph.i4_top * 77 / 100;
                }
                PRINT_LINE_DBG;
                b_null_glyph = ((t_glyph.t_bmp.i4_width == 0) || (t_glyph.t_bmp.i4_height == 0));
                if (b_fixed)
                {
                    if (b_null_glyph)
                    {
                        i4_adj_x = i4_dst_x;
                    }
                    else
                    {
                        /* calculate the left-top corner of the char destination */
                        i4_adj_x = i4_dst_x + (ui2_ch_w - t_glyph.t_bmp.i4_width) / 2;
                        if (CHECK_THAI_PRESENTATION_NEED_LEFT_SHIFT((CHAR_CODE)t_ch_prv , (CHAR_CODE)t_ch))
                        {
                            INT32 i4_prv_adv_x = 0;
                            INT32 i4_prv_adv_y = 0;
                            i4_ret = x_fe_get_glyph_advance(
                                                                h_fnt,
                                                                (CHAR_CODE) t_ch_prv,
                                                                &i4_prv_adv_x,
                                                                &i4_prv_adv_y);
                            i4_adj_x = i4_adj_x - (i4_prv_adv_x + t_glyph.i4_left);
                        }
                    }
                    
                    i4_adv_x = (INT32) ui2_ch_w;
                }
                else
                {PRINT_LINE_DBG;
                    /* get kerning info, and move base */
                    if (b_has_kern)
                    {
                        PRINT_LINE_DBG;

                        i4_ret = x_fe_get_kerning(
                            h_fnt,
                            t_ch_prv,
                            t_ch,
                            &i4_adj_x,
                            &i4_adj_y);
                        
                        if (i4_ret == FER_OK)
                        {
                            i4_dst_x += i4_adj_x;
                            i4_dst_y += i4_adj_y;
                        }
                        else
                        {
                            GL_DFB_TRACE("{GRAPHIC} No kerning available for pair (%d, %d)\n", t_ch_prv, t_ch);
                        }

                    }
                
                    if (b_null_glyph)
                    {
                        i4_adj_x = i4_dst_x;
                    }
                    else
                    {
                        /* calculate the left-top corner of the char destination */
                        i4_adj_x = i4_dst_x + t_glyph.i4_left;
                    }
                }

                if (b_null_glyph)
                {
                    i4_adj_y = i4_dst_y + (i2_height + i2_descend);
                }
                else
                {
                    i4_adj_y = i4_dst_y + (i2_height + i2_descend) - t_glyph.i4_top;
                }
                   
                if (b_null_glyph)
                {
                    SET_RECT(
                        &t_dst, 
                        i4_adj_x, 
                        i4_adj_y, 
                        i4_adj_x + i4_adv_x,
                        i4_adj_y + i4_adv_y);
                }
                else
                {
                    /* set character rendering rectangle */
                    if (t_ch == 0x0E1B && i4_adv_x > 1)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x + 1, 
                            i4_adj_y, 
                            i4_adj_x + t_glyph.t_bmp.i4_width + 1,
                            i4_adj_y + t_glyph.t_bmp.i4_height);
                    }
                    /*modify for thai character  0x0E49 must buttom flush 1 pixen after 0x0E31 (jason) */
                    else if (t_ch == 0x0E49 && (t_ch_prv == 0x0E31 || t_ch_prv == 0x0E37))
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y + 1, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            i4_adj_y + t_glyph.t_bmp.i4_height + 1);
                    }
                    else if (CHECK_THAI_PRESENTATION_NEED_TOPPEST_SHIFT(t_ch)
                        && (CHECK_THAI_PRESENTATION_NEED_TOP_SHIFT(t_ch_prv)))
                    {
                       SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            0, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            t_glyph.t_bmp.i4_height);
                    }
                    else if (t_ch == 0x0E49 && t_ch_prv == 0x0E1B)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            1, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            t_glyph.t_bmp.i4_height + 1);
                    }
                    else if (t_ch == 0x0E48 && t_ch_prv == 0x0E38)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x - 3, 
                            i4_adj_y, 
                            i4_adj_x + t_glyph.t_bmp.i4_width - 3,
                            i4_adj_y + t_glyph.t_bmp.i4_height);
                    }
                     /*modify for thai character  0x0E34 must top flush 1 pixel*/
                    else if (CHECK_THAI_PRESENTATION_NEED_TOP_SHIFT(t_ch) && i4_adj_y >= 1)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y - 1, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            i4_adj_y + t_glyph.t_bmp.i4_height - 1);
                    }
                     /*modify for thai character  0x0E34 must top flush 1 pixel*/
                    else if (t_ch == 0xE49)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y - 3, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            i4_adj_y + t_glyph.t_bmp.i4_height - 3);
                    }
                    else
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            i4_adj_y + t_glyph.t_bmp.i4_height);
                    }                           
                }


                PRINT_LINE_DBG;
                _check_n_swap(&t_dst);

                if (t_dst.i4_right >= ui4_buf_w)
                {
                    break;
                }
                 if (t_dst.i4_bottom > ui4_buf_h)
                {
                    x_dbg_stmt("t_dst.i4_bottom > ui4_buf_h, may lead to some char is cut in vertical.\n");
                    t_dst.i4_bottom = ui4_buf_h;
                } 

                /* adjust with the advance of the current character */
                i4_dst_x += i4_adv_x;
                i4_dst_y += i4_adv_y;
                    
                if (t_dst.i4_left < 0)
                {
                    i4_offset_x     = t_dst.i4_left;
                    t_dst.i4_left   = 0;
                    t_dst.i4_right -= i4_offset_x;
                }
				if (j > 0 && t_glyph.i4_left < 0)
				{
                    i4_dst_x   += t_glyph.i4_left; //advance has already counted offset value. But for not first char, it should be remove when draw string.
				}

                if (t_dst.i4_top < 0)
                {
                    i4_offset_y      = t_dst.i4_top;
                    t_dst.i4_top     = 0;
                    t_dst.i4_bottom -= i4_offset_y;
                    
                    i4_dst_y        -= i4_offset_y;
                }

				if (j == 0)
                {
                    t_stripe = t_dst;
                }
                else
                {
                    UNION_RECT(&t_stripe, &t_stripe, &t_dst);
                }
				
                /* patch for CR2073 */
                INTERSECT_RECT(&t_dst, &t_dst, &t_rc_buf);

                /* check if this string is going to be drawn out */
                {
                    PRINT_LINE_DBG;
                    if (i4_prv_dst_x < t_dst.i4_right)
                    {
                        PRINT_LINE_DBG;
                        _fill_intl_buf(
                            pv_buf,
                            ui4_buf_pitch,
                            (i4_prv_dst_x << ui1_shf),
                            0,
                            ((t_dst.i4_right - i4_prv_dst_x) << ui1_shf),
                            ui4_buf_h,
                            ui1_text_bg);
                    }

                    if (j == 0)
                    {
                        i4_stripe_start = i4_prv_dst_x;
                    }

                    i4_prv_dst_x = t_dst.i4_right;
                }

                PRINT_LINE_DBG;
                if (!b_null_glyph)
                {
                    SET_RECT_BY_SIZE(
                        &t_src, 
                        0,
                        0,
                        RECT_W(&t_dst),
                        RECT_H(&t_dst));
                    PRINT_LINE_DBG;

                    /* check if this character is going to be drawn out */
                    {
                        PRINT_LINE_DBG;
                        /* patch for CR2073 */
                        /* step 1: draw the char on internal buffer */
                        _draw_char(
                            pv_buf,
                            &t_dst,
                            ui4_buf_pitch,
                            pt_surf->t_desc.e_clrmode,
                            t_glyph.t_bmp.pui1_buf,
                            &t_src,
                            (UINT32) t_glyph.t_bmp.i4_pitch,
                            t_glyph.t_bmp.e_mode,
                            pt_fg_clr,
                            NULL,
                            pt_edge_clr,
                            e_draw_str_mode);
                    }

                    i4_stripe_end = t_dst.i4_right;
                }
                                
                t_ch_prv = t_ch;
                i++;
            }

        }
        PRINT_LINE_DBG;

        if (j > 0)
        {PRINT_LINE_DBG;
            if (pt_bg_clr && ((ui2_bg_h > 0) && (i4_dst_x > i4_bg_adj_x)))
            {
                {
                    INT32   i4_added_width;
                    i4_effect_offset = (i4_stripe_end - i4_stripe_start) - i4_dst_x;

                    i4_effect_offset = (i4_effect_offset < 0)? 0: i4_effect_offset;
                    PRINT_LINE_DBG;
                    x_fe_get_add_width_by_synthesis(h_fnt, &i4_added_width);

                    SET_RECT_BY_SIZE(
                        &t_dst, 
                        i4_base_x + i4_bg_adj_x, 
                        i4_bg_ys, 
                        (UINT32) (i4_dst_x - i4_bg_adj_x) + i4_effect_offset + i4_added_width,   
                        ui2_bg_h);
                }
                PRINT_LINE_DBG;

                INTERSECT_RECT(&t_dst, &t_clip, &t_dst);
                /*Fix me: CC left a black line in bottom and top*/
                /*t_dst.i4_top ++;
                t_dst.i4_bottom --;
                End fix ,It seems oK now?    
                */
                if (!RECT_EMPTY(&t_dst))
                {
                    PRINT_LINE_DBG;
                    gl_dfb_surface_draw_rect(
                            h_surf,
                            &t_dst,
                            pt_bg_clr,
                            1,
                            GL_STYLE_FILLED,
                            0);
                    /*gl_dfb_locked_fill_rect(
                        t_lock_info.e_clrmode,
                        t_lock_info.at_subplane[0].pv_bits,
                        t_lock_info.at_subplane[0].ui4_pitch,
                        t_dst.i4_left,
                        t_dst.i4_top,
                        (UINT32) RECT_W(&t_dst),
                        (UINT32) RECT_H(&t_dst),
                        ui4_bg_color);*/
                       
                    PRINT_LINE_DBG;

                    SET_RECT_BY_SIZE(
                        &t_dst, 
                        i4_base_x, 
                        i4_bg_ys, 
                        (UINT32) i4_dst_x, 
                        ui2_bg_h);

                    PRINT_LINE_DBG;
                    UNION_RECT(&t_rc_str, &t_rc_str, &t_dst);
                }
            }
            
            if (t_stripe.i4_left < 0)
            {
                INT32   i4_offset;
                PRINT_LINE_DBG;
                if (0 == ui2_num_run)
                {
                    /* [DTV00011723] */
                    /* for 'j' (which has a negative left) standing in the first character of each run */
                    if (j == 1)
                    {
                        i4_offset = t_stripe.i4_left;

                        t_stripe.i4_left  = 0;
                        t_stripe.i4_right -= i4_offset;
                    }
                    else
                    {
                        t_stripe.i4_left = 0;
                    }
                }
                else
                {
                    /* [DTV00011723] */
                    if (j > 1)
                    {
                        t_stripe.i4_right += t_stripe.i4_left;
                    }
                }
            }
            PRINT_LINE_DBG;
            if (t_stripe.i4_top < 0)
            {
                /* should the patch for [11723] be applied here for drawing vertically? */
                t_stripe.i4_bottom += t_stripe.i4_top;
            }
            PRINT_LINE_DBG;
            if (!b_fixed)
            {
                INT32 i4_tmp = i4_dst_x;
                
                i4_dst_x  = ((t_stripe.i4_left < 0) ? (t_stripe.i4_right + t_stripe.i4_left) : t_stripe.i4_right);
                i4_dst_x += (i4_tmp - t_stripe.i4_right);
                
                i4_dst_y += ((t_stripe.i4_top < 0) ? (t_stripe.i4_top) : 0);
            }
                PRINT_LINE_DBG;
            i4_offset_x = ((t_stripe.i4_left < 0) ? 0 : t_stripe.i4_left);
            i4_offset_y = ((t_stripe.i4_top < 0) ? 0 : t_stripe.i4_top);
            
            MOVE_RECT(&t_stripe, i4_base_x, i4_base_y);
            INTERSECT_RECT(&t_dst, &t_clip, &t_stripe);
            
            PRINT_LINE_DBG;
            if (!RECT_EMPTY(&t_dst))
            {
                PRINT_LINE_DBG;
                /* step 3: compose the internal buffer (text foreground) to the target surface */
                /* use bitblt for BDF fonts drawing */
               
                PRINT_LINE_DBG;
                if ((e_draw_str_mode == GL_DRAW_STR_DEFAULT && 
                     (pt_surf->t_desc.e_clrmode == GL_COLORMODE_ARGB_CLUT8 || 
                      pt_surf->t_desc.e_clrmode == GL_COLORMODE_AYUV_CLUT8 || 
                      e_render_mode == FE_RENDER_MODE_MONO)) ||
                    e_draw_str_mode == GL_DRAW_STR_BITBLT)
                {
                    GL_COLOR_T t_clr_k;
                    SET_COLOR(&t_clr_k, ui1_text_bg, 0, 0, 0);
                    
                    PRINT_LINE_DBG;
                    gl_dfb_surface_transp_blt(
                        h_surf,
                        _h_intl_surf, 
                        t_dst.i4_left,
                        t_dst.i4_top,
                        i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                        i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                        RECT_W(&t_dst),
                        RECT_H(&t_dst),
                        255,
                        FALSE,
                        &t_clr_k,
                        &t_clr_k,
                        0);
                }
                else
                {
                    PRINT_LINE_DBG;
                    
                    gl_dfb_surface_compose(
                        h_surf, 
                        _h_intl_surf, 
                        t_dst.i4_left,
                        t_dst.i4_top,
                        i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                        i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                        RECT_W(&t_dst),
                        RECT_H(&t_dst),
                        pt_fg_clr->a,
                        GL_SRC_OVER,
                        0);
                }
                PRINT_LINE_DBG;
                /*if (RMR_OK == i4_ret)
                {
                    b_has_cmd = TRUE;
                }
                else
                {
                    i4_ret = GLR_DRV_ERROR;
                    GL_DFB_TRACE(("{GRAPHIC} ERR: Driver error (compose fg-text rect) failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                }*/
            }
			UNION_RECT(&t_rc_str, &t_rc_str, &t_dst);
        }

        /* step 4: if there are any commands sent, flush them*/
       /* if (b_has_cmd)
        {
            _flush_cmd(1, &(pt_surf->h_signal), GL_SYNC);
            b_locked = FALSE;
        }*/
        
        if (i >= i4_strlen)
        {
            break;
        }

        i4_base_x += i4_dst_x + i4_skip_x;
        i4_base_y += i4_dst_y + 0;

        i4_bg_adj_x = i4_effect_offset;
        i4_prev_index = i;

        ui2_num_run++;
    }
    PRINT_LINE_DBG;

    if ((FER_OK == x_fe_get_style(h_fnt, &t_fnt_style)) &&
        (t_fnt_style & FE_FNT_STYLE_UNDERLINE))
    {
        INT16 i2_ul_pos, i2_ul_thickness;
        PRINT_LINE_DBG;

        if ((FER_OK == x_fe_get_underline_thickness(h_fnt, &i2_ul_thickness)) &&
            (FER_OK == x_fe_get_underline_position(h_fnt, &i2_ul_pos))        &&
            (GLR_OK  == gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info)))
        {PRINT_LINE_DBG;
            GL_RECT_T t_ul;

            {PRINT_LINE_DBG;
                SET_RECT_BY_SIZE(
                    &t_ul, 
                    t_rc_str.i4_left, 
                    i4_y + (INT32) (i2_height + i2_descend - i2_ul_pos),
                    RECT_W(&t_rc_str), 
                    i2_ul_thickness);
            }

            INTERSECT_RECT(&t_ul, &t_ul, &t_clip);
            PRINT_LINE_DBG;

            if (!RECT_EMPTY(&t_ul))
            {
                PRINT_LINE_DBG;
                gl_dfb_locked_fill_rect(
                    t_lock_info.e_clrmode,
                    t_lock_info.at_subplane[0].pv_bits,
                    t_lock_info.at_subplane[0].ui4_pitch,
                    t_ul.i4_left,
                    t_ul.i4_top,
                    RECT_W(&t_ul),
                    RECT_H(&t_ul),
                    gl_dfb_compose_packed_color(pt_fg_clr, t_lock_info.e_clrmode));
            }
            gl_dfb_surface_unlock(h_surf, NULL);
        }
    }
    i4_ret = GLR_OK;

_DRAW_USTRING_EX_EXIT_1:
    
    PRINT_LINE_DBG;
    if (b_locked)
    {
        PRINT_LINE_DBG;
        /* unlock internal buffer */
        /*_flush_cmd(0, NULL, 0);*/
    }

_DRAW_USTRING_EX_EXIT_2:
    PRINT_LINE_DBG;

    gl_dfb_obj_unlock(pt_surf);

    return (i4_ret);
}


INT32 x_gl_surface_draw_ustring_skip_underline(
    GL_HSURFACE_T       h_surf,
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

    GL_COLOR_T                  t_fg_clr_use;
    INT32                       i4_ret;

    CHECK_VALID(pt_fg_clr, GLR_INV_ARG);
    CHECK_VALID(pv_str && (ui2_strlen > 0), GLR_INV_ARG);
    CHECK_VALID(
        (pv_skip && (ui2_skiplen > 0)) || (!pv_skip && (ui2_skiplen == 0)), 
        GLR_INV_ARG);

    //CHECK_VALID(FER_OK == x_fe_get_encoding(h_fnt, &e_encoding), GLR_INV_HANDLE);

    t_fg_clr_use.a  = pt_fg_clr->a;
    t_fg_clr_use.u1.r  = pt_fg_clr->u1.r;
    t_fg_clr_use.u2.g  = pt_fg_clr->u2.g;
    t_fg_clr_use.u3.b  = pt_fg_clr->u3.b;
    if ((t_fg_clr_use.a < 16) && (t_fg_clr_use.u1.r < 16)
        && (t_fg_clr_use.u2.g < 16) && (t_fg_clr_use.u3.b < 16))
    {
        t_fg_clr_use.u1.r = 100;
    }
    
    {
        INT32                       i4_base_x, i4_base_y;
        INT32                       i4_adj_x, i4_adj_y;
        INT32                       i4_dst_x, i4_dst_y;
        INT32                       i4_prv_dst_x, i4_prv_dst_y;
        INT32                       i4_offset_x, i4_offset_y;
        INT32                       i, j, i4_skip_x, i4_strlen;
        BOOL                        b_has_kern;
        CHAR_CODE                   t_ch, t_ch_prv;
        FE_GLYPH_T                  t_glyph;
        INT16                       i2_descend, i2_height, i2_width;
        GL_DFB_SURFACE_T*                 pt_surf;
        GL_LOCK_INFO_T              t_lock_info;
        GL_RECT_T                   t_clip, t_dst, t_src, t_stripe = {0}, t_rc_str;

        INT32                       i4_skiplen;
        UINT32                      ui4_buf_w, ui4_buf_h, ui4_buf_pitch;
        VOID*                       pv_buf;
        UINT32                      ui4_bg_color = 0;
        INT32                       i4_bg_ys = 0, i4_bg_ye = 0;
        INT32                       i4_adv_x, i4_adv_y;
        BOOL                        b_has_cmd, b_locked, b_null_glyph;
        FE_FNT_STYLE                t_fnt_style;
        UINT8                       ui1_text_bg, ui1_shf;
        FE_RENDER_MODE_T            e_render_mode;
        UINT16                      ui2_num_run;        
        _CHAR_POS_INFO              *pt_char_info;  
        INT32                       i4_prev_index = 0;

        UTF16_T                     *w2s_str, *w2s_skip;
      
        /* patch for CR2073 */
        GL_RECT_T                   t_rc_buf;
        /* patch for CR2073 */

        w2s_str  = (UTF16_T *)pv_str;
        w2s_skip = (UTF16_T *)pv_skip;

        /*i4_strlen = x_fe_strlen((UINT8 *)pv_str, e_encoding, (SIZE_T)ui2_strlen);*/
        i4_strlen = x_uc_w2s_strlen(w2s_str);
        /*printf("i4_strlen (src) is %d\n", i4_strlen);*/
        
        i4_strlen = MIN(i4_strlen, (INT32) ui2_strlen);
        
        CHECK_VALID(i4_strlen > 0, GLR_INV_ARG);
        ui2_strlen = (UINT16)ui2_strlen;
        b_locked = FALSE;

        /* check string of ignorance */
        if (pv_skip)
        {
            /*i4_skiplen = x_fe_strlen((UINT8 *)pv_skip, e_encoding, (SIZE_T)ui2_skiplen);*/
            i4_skiplen = x_uc_w2s_strlen(w2s_skip);
            i4_skiplen = MIN(i4_skiplen, (INT32) ui2_skiplen);
        }
        else
        {
            i4_skiplen = 0;
        }
        ui2_skiplen = (UINT16)i4_skiplen;
        if (pv_skip_line)
        {        
            if (_i4_skip_char_count < i4_strlen)
            {
                _pt_skip_char_info = (_CHAR_POS_INFO *)x_mem_realloc(_pt_skip_char_info, i4_strlen * sizeof(_CHAR_POS_INFO));
            }
            
            pt_char_info = _pt_skip_char_info;
            
            if (pt_char_info == NULL)       
            {            
                return GLR_NO_MEMORY;        
            }        

            _i4_skip_char_count = i4_strlen;
            x_memset((VOID *)pt_char_info, 0, i4_strlen * sizeof(_CHAR_POS_INFO));
        }
        else    
        {        
            pt_char_info = NULL;    
        }

        PRINT_LINE_DBG;

        pt_surf = gl_dfb_handle_lock_obj(GLT_OBJ_SURFACE, h_surf, &i4_ret);

        i4_ret = GLR_OK;
        
        /* find clipping area */
        /*t_clip = pt_surf->t_clip_rect;*/
        gl_surface_get_clip_area(pt_surf, &t_clip);
        if (RECT_EMPTY(&t_clip) || i4_x >= t_clip.i4_right || i4_y >= t_clip.i4_bottom)
        {

            PRINT_LINE_DBG;
            GL_DFB_TRACE("{GRAPHIC} The surface clip area is empty [%d,%d,%d,%d].\r\n\r\n",
                t_clip.i4_left,
                t_clip.i4_top,
                t_clip.i4_right,
                t_clip.i4_bottom);
            goto _DRAW_USTRING_EX_EXIT_2;
        }
            

        /* get surface memory raw pointer */
        //i4_ret = gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info);


        PRINT_LINE_DBG;
        if (GL_FAILED(i4_ret))
        {

            GL_DFB_TRACE(("{GRAPHIC} ERR: Locking surface failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DRAW_USTRING_EX_EXIT_2;
        }

        if (FER_OK != x_fe_get_max_advance(h_fnt, &i2_width, &i2_height) ||
            FER_OK != x_fe_get_height(h_fnt, &i2_height) ||
            FER_OK != x_fe_get_descender(h_fnt, &i2_descend))
        {

            i4_ret = GLR_FE_ERROR;
            GL_DFB_TRACE(("{GRAPHIC} ERR: Getting height/descender failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DRAW_USTRING_EX_EXIT_1;
        }
            

        SET_RECT(&t_rc_str, 0x7fffffff, 0x7fffffff, -0x7fffffff, -0x7fffffff);

        if (pt_bg_clr)
        {
            ui4_bg_color = gl_dfb_compose_packed_color(pt_bg_clr, pt_surf->t_desc.e_clrmode);
        }

        /* draw each char onto the bitmap */
        i4_base_x  = i4_x;
        i4_base_y  = i4_y;
        t_ch_prv   = 0;
        b_has_kern = x_fe_has_kerning(h_fnt);

        
        i4_bg_ys = MAX(i4_bg_y, t_clip.i4_top);
        i4_bg_ye = MIN(i4_bg_y + ui2_bg_h, t_clip.i4_bottom);
        if (i4_bg_ys >= i4_bg_ye)
        {
            ui2_bg_h = 0;
        }
        else
        {
            ui2_bg_h = (UINT16) (i4_bg_ye - i4_bg_ys);
        }

        /* [20808] patch for the (i2_height + i2_descend) / 4 */
        ui4_buf_h = MAX((UINT32) (i2_height + (i2_height + i2_descend) / 4), (UINT32) ui2_bg_h);
#if 0
        if(ui4_buf_h > MAX_BUF_HEIGHT)
        {
            GL_DFB_TRACE("Waring:Outofbufbounds:H %d\n", ui4_buf_h);
            ui4_buf_h = MAX_BUF_HEIGHT;
        }    
#endif        
        ui4_buf_w = (UINT32)(i4_x + RECT_W(&t_clip));
            

        /* check edge effect */
        if (FER_OK != x_fe_get_style(h_fnt, &t_fnt_style))
        {
            i4_ret = GLR_FE_ERROR;
            GL_DFB_TRACE(("{GRAPHIC} ERR: Getting style failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
            goto _DRAW_USTRING_EX_EXIT_1;
        }
        if (!(t_fnt_style & FE_FNT_STYLE_EDGE_EFFECT))
        {
            pt_edge_clr = NULL;
        }
        PRINT_LINE_DBG;
        //gl_prepare_buf(0, &pv_buf);

            
        gl_cal_buf_param_by_height(
            pt_surf->t_desc.e_clrmode, 
            ui4_buf_h, 
            &pv_buf, 
            &ui4_buf_pitch, 
            &ui4_buf_w);

//        x_memset(pv_buf, 0, ui4_buf_pitch * ui4_buf_h);

        SET_RECT_BY_SIZE(
            &t_rc_buf,
            0,
            0,
            ui4_buf_w,
            ui4_buf_h);
        
        b_locked = FALSE;

        i = 0;
        
        ui1_shf = _shf_bit(pt_surf->t_desc.e_clrmode);
        
        ui1_text_bg = 
            (GL_IS_PALETTE_MODE(pt_surf->t_desc.e_clrmode)) ? (t_fg_clr_use.u3.index + 1) : 0;

        /* set up font rendering mode */
        if (GL_IS_PALETTE_MODE(pt_surf->t_desc.e_clrmode) ||
            GL_COLORMODE_RGB_D565 == pt_surf->t_desc.e_clrmode)
        {
            e_render_mode = FE_RENDER_MODE_MONO;
        }
        else
        {
            e_render_mode = FE_RENDER_MODE_NORMAL;
        }
        
        ui2_num_run = 0;
        i4_prev_index = 0;

        while (1)
        {
            INT32       i4_stripe_start = 0, i4_stripe_end = 0;
            INT32       i4_effect_offset = 0;
            
            i4_prv_dst_x = i4_dst_x = i4_prv_dst_y = 0;
            i4_dst_y  = 0;
            i4_skip_x = 0;
            b_has_cmd = FALSE;

            if (!b_locked)
            {
                b_locked = TRUE;
            }
            PRINT_LINE_DBG;
                

            for (j = 0; i < i4_strlen; j++)
            {
                BOOL b_skip_line = FALSE;
                t_ch = (CHAR_CODE)w2s_str[i];
                /* MHF Linux - Chun Jiang for fix \t display*/
                if (t_ch == 0x9)
                {
                    i ++;
                    break;
                }
                
                /* load character */
                //t_ch = x_fe_get_char(pv_str, i, e_encoding);
                //t_ch = (CHAR_CODE)x_uc_w2s_get(w2s_str, i);
                 if (pv_skip_line)   
                 {
                    b_skip_line = (FE_IS_CHAR_NO_UNDERLINE(t_ch) || _is_skip_char_no_encoding(t_ch, pv_skip_line, ui2_skip_line_len)) ? TRUE : FALSE;
                 }
                
                PRINT_LINE_DBG;
                if (pv_skip && _is_skip_char_no_encoding(t_ch, pv_skip, ui2_skiplen))
                {   /* handle skip character */
                    if (pt_char_info)
                    {
                        if (!b_skip_line)
                        {
                            pt_char_info[i].i4_begin = i4_stripe_end;
                            pt_char_info[i].i4_width = i4_skip_x;
                        }
                        else
                        {
                            pt_char_info[i].i4_begin = 0;
                            pt_char_info[i].i4_width = 0;
                        }
                    }
                    i++;
                    t_ch_prv = 0;
                    i4_skip_x = (INT32) ui2_skip_w;
                    break;
                }
                else
                {
                    /* get character advance */
                    i4_adv_x = 0;
                    i4_adv_y = 0;
                    if (t_ch == FE_CC_LEADING_CHAR ||
                        (t_ch >= FE_UTF16_SPACE_USER_START && t_ch <= FE_UTF16_SPACE_USER_END))
                    {
                            /* fake character */
                        t_ch = (CHAR_CODE)0x20;
                    }

                    
                    i4_ret = x_fe_get_glyph_advance_cc(
                        h_fnt,
                        t_ch,
                        &i4_adv_x,
                        &i4_adv_y);
                    
                    if (i4_ret != FER_OK)
                    {PRINT_LINE_DBG;
                        i4_ret = GLR_FE_ERROR;
                        GL_DFB_TRACE(("{GRAPHIC} ERR: Getting glyph advance failed (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
                        goto _DRAW_USTRING_EX_EXIT_1;
                    }
                        
                    PRINT_LINE_DBG;
                    /* load glyph bitmap */
                    t_glyph.e_render_mode = e_render_mode;
                    i4_ret = x_fe_load_char(h_fnt, t_ch, &t_glyph);
                    
                    PRINT_LINE_DBG;
                    if (i4_ret != FER_OK)
                    {
                        PRINT_LINE_DBG;
                        i4_ret = GLR_FE_ERROR;
                        GL_DFB_TRACE(("{GRAPHIC} ERR: Loading character [%d] failed (%d) at L%d\r\n\r\n", t_ch, i4_ret, __LINE__));
                        goto _DRAW_USTRING_EX_EXIT_1;
                    }

                    b_null_glyph = ((t_glyph.t_bmp.i4_width == 0) || (t_glyph.t_bmp.i4_height == 0));
                    
                    if (pt_char_info != NULL)
                    {
                        pt_char_info[i].i4_adv = i4_adv_x;
                        if (!b_skip_line)
                        {
                            pt_char_info[i].i4_begin = i4_dst_x;
                            pt_char_info[i].i4_width = i4_adv_x;
                        }
                        else
                        {
                            pt_char_info[i].i4_begin = 0;
                            pt_char_info[i].i4_width = 0;
                        }
    
                        if (((i - 1) >= 0) && (i == (i4_strlen - 1)))
                        {
                            if (pt_char_info[i - 1].i4_width != 0)
                            {
                                pt_char_info[i - 1].i4_width = i4_dst_x - pt_char_info[i - 1].i4_begin;
                            }
                        }
                    }
                    
                    if (b_fixed)
                    {
                        if (b_null_glyph)
                        {
                            i4_adj_x = i4_dst_x;
                        }
                        else
                        {
                            /* calculate the left-top corner of the char destination */
                            i4_adj_x = i4_dst_x + (ui2_ch_w - t_glyph.t_bmp.i4_width) / 2;
                        }
                        
                        i4_adv_x = (INT32) ui2_ch_w;
                    }
                    else
                    {
                        /* get kerning info, and move base */
                        if (b_has_kern)
                        {

                            i4_ret = x_fe_get_kerning(
                                h_fnt,
                                t_ch_prv,
                                t_ch,
                                &i4_adj_x,
                                &i4_adj_y);

                            
                            if (i4_ret == FER_OK)
                            {
                                i4_dst_x += i4_adj_x;
                                i4_dst_y += i4_adj_y;
                            }
                            else
                            {
                                GL_DFB_TRACE("{GRAPHIC} No kerning available for pair (%d, %d)\n", t_ch_prv, t_ch);
                            }

                        }
                    
                        if (b_null_glyph)
                        {
                            i4_adj_x = i4_dst_x;
                        }
                        else
                        {
                            /* calculate the left-top corner of the char destination */
                            i4_adj_x = i4_dst_x + t_glyph.i4_left;
                        }
                    }

                    if (b_null_glyph)
                    {
                        i4_adj_y = i4_dst_y + (i2_height + i2_descend);
                    }
                    else
                    {
                        i4_adj_y = i4_dst_y + (i2_height + i2_descend) - t_glyph.i4_top;
                    }
                    
                       
                    if (b_null_glyph)
                    {
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y, 
                            i4_adj_x + i4_adv_x,
                            i4_adj_y + i4_adv_y);
                    }
                    else
                    {
                        /* set character rendering rectangle */
                        SET_RECT(
                            &t_dst, 
                            i4_adj_x, 
                            i4_adj_y, 
                            i4_adj_x + t_glyph.t_bmp.i4_width,
                            i4_adj_y + t_glyph.t_bmp.i4_height);
                    }


                    PRINT_LINE_DBG;
                    _check_n_swap(&t_dst);

                    
                    if (t_dst.i4_right >= ui4_buf_w)
                    {
                        x_dbg_stmt("[Graphic] Internal buffer is small !\n");
                        break;
                    }
                    
                    
                    if (j == 0)
                    {
                        t_stripe = t_dst;
                    }
                    else
                    {
                        UNION_RECT(&t_stripe, &t_stripe, &t_dst);
                    }

                    /* adjust with the advance of the current character */
                    i4_dst_x += i4_adv_x;
                    i4_dst_y += i4_adv_y;
                        
                    if (t_dst.i4_left < 0)
                    {
                        i4_offset_x     = t_dst.i4_left;
                        t_dst.i4_left   = 0;
                        t_dst.i4_right -= i4_offset_x;

                        i4_dst_x       -= i4_offset_x;
                    }

                    if (t_dst.i4_top < 0)
                    {
                        i4_offset_y      = t_dst.i4_top;
                        t_dst.i4_top     = 0;
                        t_dst.i4_bottom -= i4_offset_y;
                        
                        i4_dst_y        -= i4_offset_y;
                    }
                    
                    /* patch for CR2073 */
                    INTERSECT_RECT(&t_dst, &t_dst, &t_rc_buf);

                    if (i4_prv_dst_x < t_dst.i4_right)
                    {
                        PRINT_LINE_DBG;
                        _fill_intl_buf(
                            pv_buf,
                            ui4_buf_pitch,
                            (i4_prv_dst_x << ui1_shf),
                            0,
                            ((t_dst.i4_right - i4_prv_dst_x) << ui1_shf),
                            ui4_buf_h,
                            ui1_text_bg);
                    }


                    if (j == 0)
                    {
                        i4_stripe_start = i4_prv_dst_x;
                    }

                    i4_prv_dst_x = t_dst.i4_right;
                
                    if (!b_null_glyph)
                    {
                        SET_RECT_BY_SIZE(
                            &t_src, 
                            0,
                            0,
                            RECT_W(&t_dst),
                            RECT_H(&t_dst));
                        
                       
                            /* patch for CR2073 */
                            /* step 1: draw the char on internal buffer */
                        _draw_char(
                            pv_buf,
                            &t_dst,
                            ui4_buf_pitch,
                            pt_surf->t_desc.e_clrmode,
                            t_glyph.t_bmp.pui1_buf,
                            &t_src,
                            (UINT32) t_glyph.t_bmp.i4_pitch,
                            t_glyph.t_bmp.e_mode,
                            &t_fg_clr_use,
                            NULL,
                            pt_edge_clr,
                            e_draw_str_mode);
                            
                        
                        i4_stripe_end =  t_dst.i4_right;
                    }
                    
                    t_ch_prv = t_ch;
                    i++;
                }

            }
            

            if (j > 0)
            {
                if (pt_bg_clr && ((ui2_bg_h > 0) && (i4_dst_x > i4_bg_adj_x)))
                {
                    
                    INT32   i4_added_width;
                    i4_effect_offset = (i4_stripe_end - i4_stripe_start) - i4_dst_x;

                    i4_effect_offset = (i4_effect_offset < 0)? 0: i4_effect_offset;
                    PRINT_LINE_DBG;
                    x_fe_get_add_width_by_synthesis(h_fnt, &i4_added_width);

                    SET_RECT_BY_SIZE(
                        &t_dst, 
                        i4_base_x + i4_bg_adj_x, 
                        i4_bg_ys, 
                        (UINT32) (i4_dst_x - i4_bg_adj_x) + i4_effect_offset + i4_added_width,   
                        ui2_bg_h);
                    

                    INTERSECT_RECT(&t_dst, &t_clip, &t_dst);
                    /*Fix me: CC left a black line in bottom and top*/
                    /*t_dst.i4_top ++;
                    t_dst.i4_bottom --;
                    End fix ,It seems oK now?    
                    */
                    if (!RECT_EMPTY(&t_dst))
                    {
                        /* check if it's in the subset mode */
                        
                        gl_dfb_surface_draw_rect(
                                h_surf,
                                &t_dst,
                                pt_bg_clr,
                                1,
                                GL_STYLE_FILLED,
                                0);
                        
                        SET_RECT_BY_SIZE(
                            &t_dst, 
                            i4_base_x, 
                            i4_bg_ys, 
                            (UINT32) i4_dst_x, 
                            ui2_bg_h);
                       
                        UNION_RECT(&t_rc_str, &t_rc_str, &t_dst);
                    }
                }
                
                if (t_stripe.i4_left < 0)
                {
                    INT32   i4_offset;
                    
                    if (0 == ui2_num_run)
                    {
                        /* [DTV00011723] */
                        /* for 'j' (which has a negative left) standing in the first character of each run */
                        if (j == 1)
                        {
                            i4_offset = t_stripe.i4_left;

                            t_stripe.i4_left  = 0;
                            t_stripe.i4_right -= i4_offset;
                        }
                        else
                        {
                            t_stripe.i4_left = 0;
                        }
                    }
                    else
                    {
                        /* [DTV00011723] */
                        if (j > 1)
                        {
                            t_stripe.i4_right += t_stripe.i4_left;
                        }
                    }
                }
                
                if (t_stripe.i4_top < 0)
                {
                    /* should the patch for [11723] be applied here for drawing vertically? */
                    t_stripe.i4_bottom += t_stripe.i4_top;
                }
                
                if (!b_fixed)
                {
                    INT32 i4_tmp = i4_dst_x;
                    
                    i4_dst_x  = ((t_stripe.i4_left < 0) ? (t_stripe.i4_right + t_stripe.i4_left) : t_stripe.i4_right);
                    i4_dst_x += (i4_tmp - t_stripe.i4_right);
                    
                    i4_dst_y += ((t_stripe.i4_top < 0) ? (t_stripe.i4_top) : 0);
                }
                    
                i4_offset_x = ((t_stripe.i4_left < 0) ? 0 : t_stripe.i4_left);
                i4_offset_y = ((t_stripe.i4_top < 0) ? 0 : t_stripe.i4_top);
                
                MOVE_RECT(&t_stripe, i4_base_x, i4_base_y);
                INTERSECT_RECT(&t_dst, &t_clip, &t_stripe);
                
                /* adjust the char starting position */
                if (pt_char_info)
                {
                    INT32       i4_i;
                    INT32       i4_offset;
    
                    i4_offset = t_dst.i4_left /*- (t_stripe.i4_left - i4_base_x)*/ - (i4_offset_x + (t_dst.i4_left - t_stripe.i4_left));
    
                    for (i4_i = i4_prev_index; i4_i < i; i4_i++)
                    {
                        pt_char_info[i4_i].i4_begin += i4_offset;
                    }
                }
                
                if (!RECT_EMPTY(&t_dst))
                {
                    /* step 3: compose the internal buffer (text foreground) to the target surface */
                    /* use bitblt for BDF fonts drawing */
                   
                    if ((e_draw_str_mode == GL_DRAW_STR_DEFAULT && 
                         (pt_surf->t_desc.e_clrmode == GL_COLORMODE_ARGB_CLUT8 || 
                          pt_surf->t_desc.e_clrmode == GL_COLORMODE_AYUV_CLUT8 || 
                          e_render_mode == FE_RENDER_MODE_MONO)) ||
                        e_draw_str_mode == GL_DRAW_STR_BITBLT)
                    {
                        GL_COLOR_T t_clr_k;
                        SET_COLOR(&t_clr_k, ui1_text_bg, 0, 0, 0);
                        
                        gl_dfb_surface_transp_blt(
                            h_surf,
                            _h_intl_surf, 
                            t_dst.i4_left,
                            t_dst.i4_top,
                            i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                            i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                            RECT_W(&t_dst),
                            RECT_H(&t_dst),
                            255,
                            FALSE,
                            &t_clr_k,
                            &t_clr_k,
                            0);
                    }
                    else
                    {
                        gl_dfb_surface_compose(
                            h_surf, 
                            _h_intl_surf, 
                            t_dst.i4_left,
                            t_dst.i4_top,
                            i4_offset_x + (t_dst.i4_left - t_stripe.i4_left),
                            i4_offset_y + (t_dst.i4_top - t_stripe.i4_top),
                            RECT_W(&t_dst),
                            RECT_H(&t_dst),
                            t_fg_clr_use.a,
                            GL_SRC_OVER,
                            0);
                        
                    }
                }
            }            
            if (i >= i4_strlen)
            {
                break;
            }

            i4_base_x += i4_dst_x +  i4_skip_x;
            i4_base_y += i4_dst_y;

            i4_bg_adj_x = i4_effect_offset;
            i4_prev_index = i;

            ui2_num_run++;
        }
        
        if ((FER_OK == x_fe_get_style(h_fnt, &t_fnt_style)) &&
            (t_fnt_style & FE_FNT_STYLE_UNDERLINE))
        {
            INT16 i2_ul_pos, i2_ul_thickness;
            PRINT_LINE_DBG;

            if ((FER_OK == x_fe_get_underline_thickness(h_fnt, &i2_ul_thickness)) &&
                (FER_OK == x_fe_get_underline_position(h_fnt, &i2_ul_pos))        &&
                (gl_dfb_surface_lock(h_surf, NULL, 0, &t_lock_info) == GLR_OK))
            {PRINT_LINE_DBG;
                GL_RECT_T t_ul;

                
                SET_RECT_BY_SIZE(
                    &t_ul, 
                    t_rc_str.i4_left, 
                    i4_y + (INT32) (i2_height + i2_descend - i2_ul_pos - i2_ul_thickness / 2),
                    RECT_W(&t_rc_str), 
                    i2_ul_thickness);
               

                INTERSECT_RECT(&t_ul, &t_ul, &t_clip);
                

                if (!RECT_EMPTY(&t_ul))
                {
                    /* check if it's in the subset mode */
                    if (pv_skip_line == NULL)
                    {
                        gl_dfb_locked_fill_rect(
                            t_lock_info.e_clrmode,
                            t_lock_info.at_subplane[0].pv_bits,
                            t_lock_info.at_subplane[0].ui4_pitch,
                            t_ul.i4_left,
                            t_ul.i4_top,
                            RECT_W(&t_ul),
                            RECT_H(&t_ul),
                            gl_dfb_compose_packed_color(&t_fg_clr_use, t_lock_info.e_clrmode));
                        
                    }
                     else 
                    {
                        
                        INT32       i4_i;
                        UINT32      ui4_color = gl_dfb_compose_packed_color(&t_fg_clr_use, t_lock_info.e_clrmode);
                        GL_RECT_T   t_out;

                        for (i4_i = 0; i4_i < i4_strlen; i4_i++)
                        {
                            if (i4_i + 1 < i4_strlen &&
                                pt_char_info[i4_i + 1].i4_width != 0 &&
                                pt_char_info[i4_i].i4_width != 0)
                            {
                                pt_char_info[i4_i].i4_width = pt_char_info[i4_i + 1].i4_begin - pt_char_info[i4_i].i4_begin;
                            }

                            if (i4_i == 0)
                            {
                                pt_char_info[i4_i].i4_width += (pt_char_info[i4_i].i4_begin - t_ul.i4_left);
                                pt_char_info[i4_i].i4_begin = t_ul.i4_left;
                            }

                            SET_RECT_BY_SIZE(&t_out, pt_char_info[i4_i].i4_begin, t_ul.i4_top, pt_char_info[i4_i].i4_width, RECT_H(&t_ul));
                            INTERSECT_RECT(&t_out, &t_out, &t_clip);

                            if (!RECT_EMPTY(&t_out))
                            {
                                gl_dfb_locked_fill_rect(
                                    t_lock_info.e_clrmode,
                                    t_lock_info.at_subplane[0].pv_bits,
                                    t_lock_info.at_subplane[0].ui4_pitch,
                                    t_out.i4_left,
                                    t_out.i4_top,
                                    RECT_W(&t_out),
                                    RECT_H(&t_out),
                                    ui4_color);
                            }
                        }
                    }
                }
                gl_dfb_surface_unlock(h_surf, NULL);
            }
        }
        i4_ret = GLR_OK;

    _DRAW_USTRING_EX_EXIT_1:
        
        PRINT_LINE_DBG;
        if (b_locked)
        {
            PRINT_LINE_DBG;
            /* unlock internal buffer */
            /*_flush_cmd(0, NULL, 0);*/
        }

    _DRAW_USTRING_EX_EXIT_2:
        PRINT_LINE_DBG;

        gl_dfb_obj_unlock(pt_surf);

        PRINT_LINE_DBG;
//        if (pt_char_info)   
//        { 
//            x_mem_free(pt_char_info);    
//        }
//        DFBCHECK(g_p_dfb->WaitIdle(g_p_dfb), &i4_ret);
    }
    return i4_ret;
}

INT32 x_gl_surface_draw_ustring_top_2_down(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_x,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_h,
    UINT16              ui2_skip_h,
    UINT16              ui2_bg_w,
    INT32               i4_bg_adj_y,
    UINT16              *aui2_ch_orient_idx,
    UINT16              ui2_ch_orient_num,
    GL_CHAR_ORIENT_T    e_ch_orientation)
{
    GL_STR_AUX_T        t_aux_data;
    INT32 i4_ret;
    _TIME_BEGIN

    x_memset(&t_aux_data, 0, sizeof(GL_STR_AUX_T));
    t_aux_data.aui2_display_idx   = NULL;
    t_aux_data.e_str_direction    = GL_STR_TOP_2_BOTTOM;
    t_aux_data.aui2_orient_idx    = aui2_ch_orient_idx;
    t_aux_data.ui2_orient_idx_num = ui2_ch_orient_num;
    t_aux_data.e_char_orientation = e_ch_orientation;

    i4_ret = _draw_ustring_ex(
        h_surf,
        i4_x,
        i4_y,
        i4_bg_x,
        pv_str,
        ui2_strlen,
        pv_skip,
        ui2_skiplen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_edge_clr,
        b_fixed,
        ui2_ch_h,
        ui2_skip_h,
        ui2_bg_w,
        i4_bg_adj_y,
        GL_DRAW_STR_DEFAULT,
        &t_aux_data);
    _TIME_END
    return  i4_ret;
}

INT32 x_gl_surface_draw_subset_ustring(
    GL_HSURFACE_T       h_surf,
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
    GL_DRAW_STR_MODE_T  e_draw_str_mode,
    UINT16              ui2_index_num,
    UINT16*             aui2_index)
{
    GL_STR_AUX_T        t_aux_data;
    INT32 i4_ret;
    x_memset(&t_aux_data, 0, sizeof(GL_STR_AUX_T));

    _TIME_BEGIN 
    t_aux_data.ui2_display_idx_num = ui2_index_num;
    t_aux_data.aui2_display_idx    = aui2_index;
    t_aux_data.e_str_direction     = GL_STR_LEFT_2_RIGHT;
    t_aux_data.aui2_orient_idx     = NULL;
 
    i4_ret =  _draw_ustring_ex(
        h_surf,
        i4_x,
        i4_y,
        i4_y,
        w2s_str,
        ui2_strlen,
        w2s_skip,
        ui2_skiplen,
        h_fnt,
        pt_fg_clr,
        NULL,
        NULL,
        b_fixed,
        ui2_ch_w,
        ui2_skip_w,
        0,
        0,
        GL_DRAW_STR_DEFAULT,
        &t_aux_data);   
    _TIME_END
    return i4_ret;
}


INT32 x_gl_isdb_draw_init(unsigned char* pt_buf, UINT16 ui2_max_width, UINT16 ui2_max_height)
{
    UINT32 ui4_font_size = GET_UINT32_FROM_PTR_BIG_END (pt_buf);
    FT_Error            error;
    INT32           i4_ret;

    error = FT_Init_FreeType( &(t_g_gl_font_eng.library)); 
    CHECK_EXP_RET((error),"FT_Init_FreeType error\n",0);


    error = FT_New_Memory_Face(t_g_gl_font_eng.library,
        pt_buf+4,                                           /* first byte in memory */
        (FT_Long)ui4_font_size,                             /* size in bytes        */
        0,                                                  /* face_index           */
        &(t_g_gl_font_eng.face) );

    CHECK_EXP_RET((error),"FT_New_Memory_Face error\n",0);

    ui2_g_gl_text_max_width  = ui2_max_width;
    ui2_g_gl_text_max_height = ui2_max_height;

    i4_ret = x_sema_create( &h_g_gl_isdb_sema_hdl, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK );
    if ( i4_ret!=OSR_OK || h_g_gl_isdb_sema_hdl==NULL_HANDLE )
    {
        h_g_gl_isdb_sema_hdl = NULL_HANDLE;
        return GLR_OUT_OF_RESOURCE;
    }
    
    return error;
}


INT32 x_gl_isdb_get_char_width(
            UINT16                      ui2_char_code,
            UINT16                      ui2_char_w,
            UINT16                      ui2_char_h,
            UINT16*                     pui2_act_width )
{
    FT_Face                     t_face = t_g_gl_font_eng.face;
    FT_Glyph                    t_glyph;
    FT_BitmapGlyph              glyph_bitmap;
    FT_GlyphSlot                t_slot;
    UINT32                      u4_char_idx;
    UINT16                      u2_char_idx;

    x_sema_lock( h_g_gl_isdb_sema_hdl, X_SEMA_OPTION_WAIT );

    if (0x20 == ui2_char_code)
    {
        *pui2_act_width = ui2_char_w/(UINT16)3;
    }
    else
    {
        FT_Set_Pixel_Sizes( t_face, ui2_char_w, ui2_char_h );

        t_slot = t_face->glyph;

        u4_char_idx = FT_Get_Char_Index( t_face, ui2_char_code );

        u2_char_idx = (UINT16)u4_char_idx;
        FT_Load_Glyph( t_face, u2_char_idx, FT_LOAD_NO_BITMAP);

        
        FT_Get_Glyph( t_slot, &t_glyph );

        if (FT_GLYPH_FORMAT_BITMAP != t_glyph->format)
        {
            FT_Glyph_To_Bitmap( &t_glyph, FT_RENDER_MODE_NORMAL, NULL, 1 );
        }

        glyph_bitmap = (FT_BitmapGlyph)t_glyph;
        
        *pui2_act_width = (UINT16)glyph_bitmap->bitmap.width;

        FT_Done_Glyph( t_glyph );
    }
    
    x_sema_unlock( h_g_gl_isdb_sema_hdl );
    
    return GLR_OK;
}


INT32 x_gl_isdb_draw_char(
            GL_HSURFACE_T               h_grid_surf,
            GL_COLORMODE_T              e_clrmode,
            UINT16                      ui2_char_code,
            GL_ISDB_GRID_T*             pt_grid,
            GL_COLOR_T*                 pt_char_color,
            GL_COLOR_T*                 pt_ORN_color,
            BOOL                        fg_is_equal_width,
            UINT16*                     pui2_grid_act_width )
{
    FT_Face                     t_face = t_g_gl_font_eng.face;
    FT_Glyph                    t_glyph;
    FT_BitmapGlyph              glyph_bitmap;
    FT_GlyphSlot                t_slot;
    UINT32                      u4_char_idx;
    UINT16                      u2_char_idx;
    UINT16                      ui2_width;
    UINT16                      ui2_height;
    BOOL                        fg_need_ORN;

    x_sema_lock( h_g_gl_isdb_sema_hdl, X_SEMA_OPTION_WAIT );

    fg_need_ORN = (NULL == pt_ORN_color) ? FALSE:TRUE;
        
    FT_Set_Pixel_Sizes( t_face, pt_grid->ui2_char_h, pt_grid->ui2_char_v );

    t_slot = t_face->glyph;

    u4_char_idx = FT_Get_Char_Index( t_face, ui2_char_code );

    u2_char_idx = (UINT16)u4_char_idx;

    if (fg_need_ORN)
    {
        FT_Load_Glyph( t_face, u2_char_idx, FT_LOAD_RENDER);
        FT_Bitmap_Uniform( t_slot->library, &t_slot->bitmap, 1, 1 );
    }
    else
    {
        FT_Load_Glyph( t_face, u2_char_idx, FT_LOAD_NO_BITMAP);
    }

    #if 0
    FT_GlyphSlot_Embolden(slot);    /*BOLD*/
    FT_GlyphSlot_Oblique(slot);     /*ITALIC*/
    #endif

    FT_Get_Glyph( t_slot, &t_glyph );

    if (FT_GLYPH_FORMAT_BITMAP != t_glyph->format)
    {
        FT_Glyph_To_Bitmap( &t_glyph, FT_RENDER_MODE_NORMAL, NULL, 1 );
    }

    glyph_bitmap = (FT_BitmapGlyph)t_glyph;
    
    ui2_width    = (UINT16)glyph_bitmap->bitmap.width;
    ui2_height   = (UINT16)glyph_bitmap->bitmap.rows;

    if (!(ui2_g_gl_text_max_width >= ui2_width && ui2_g_gl_text_max_height >= ui2_height))
    {
        ui2_width    = (UINT16)(t_slot->metrics.width  / 64);
        ui2_height   = (UINT16)(t_slot->metrics.height / 64);
    
        if (!(ui2_g_gl_text_max_width >= ui2_width && ui2_g_gl_text_max_height >= ui2_height))
        {
            x_sema_unlock( h_g_gl_isdb_sema_hdl );
            return GLR_OK;
        }
    }

    {
        GL_COLOR_T      t_color = {0};
        GL_RECT_T       t_rect;

        t_rect.i4_left   = t_rect.i4_top = 0;
        t_rect.i4_right  = t_rect.i4_left + pt_grid->ui2_grid_h;
        t_rect.i4_bottom = t_rect.i4_top  + pt_grid->ui2_grid_v;
        
        x_gl_surface_draw_rect(
                    h_grid_surf,
                    &t_rect,
                    &t_color,
                    0,
                    GL_STYLE_FILLED,
                    GL_SYNC );
    }

    /* draw char */
    if (0x20 == ui2_char_code)
    {
        ui2_width = pt_grid->ui2_char_h/(UINT16)3;
    }
    else
    {
        UINT32                      ui4_ymax;
        
        UINT8*                      pu1_src_buf;
        UINT8*                      pu1_src_edge_buf    = NULL;
        INT16                       i2_adjust;
        UINT16                      w, h, ui2_left, ui2_top;
        GL_LOCK_INFO_T              t_surf_info;
        UINT8*                      pu1_dst_buf;
        UINT32                      ui4_dst_pitch;
        UINT8*                      pui1_pb_dst;
        UINT16                      ui2_fg_ARGB_glph_a;
        UINT32                      ui4_fg_ARGB_glph_a;

        {
            /* u4CharHeight == face->size->metrics.y_ppem */
            /* the last 6 bits indicate float number */
            /* u4_ymax is an ajustment value in V direction */
            /* the adjustment make position center and character block enter are one */
            ui4_ymax    = (UINT32)(((t_face->size->metrics.y_ppem << 6) +
                                    t_face->size->metrics.ascender +
                                    t_face->size->metrics.descender + 64) >> 7);

            i2_adjust   = (INT16)ui4_ymax;
            i2_adjust   -= glyph_bitmap->top;

            /* calc the top position */
            if ((i2_adjust < 0) && ((UINT16)(i2_adjust*(-1)) > pt_grid->ui2_t_space_v)) /* check the boundary, and the top value is valid */
            {
                ui2_top = 0;
            }
            else                                                                        /* the top value is not invalid  */
            {
                ui2_top = (UINT16)((INT32)pt_grid->ui2_t_space_v + i2_adjust);
            }
            /* calc the left position */
            ui2_left    = pt_grid->ui2_l_space_h;

            if (fg_is_equal_width)
            {
                if (pt_grid->ui2_char_h >= ui2_width)
                {
                    ui2_left += (pt_grid->ui2_char_h - ui2_width) / 2;
                }
            }
        }
        
        pu1_src_buf  = glyph_bitmap->bitmap.buffer;
        pu1_src_edge_buf = pu1_src_buf + glyph_bitmap->bitmap.pitch * glyph_bitmap->bitmap.rows;

        x_gl_surface_lock( h_grid_surf, NULL, GL_SYNC, &t_surf_info );

        ui4_dst_pitch =  t_surf_info.at_subplane[0].ui4_pitch;
        pu1_dst_buf   =  t_surf_info.at_subplane[0].pv_bits;
        pu1_dst_buf   += ui2_top * ui4_dst_pitch;

        switch (e_clrmode)
        {
            case GL_COLORMODE_ARGB_D4444:
                if (fg_need_ORN)
                {
                    for (h = 0; h < ui2_height; h++)
                    {
                        pui1_pb_dst = pu1_dst_buf + ui2_left * 2;
                        for (w = 0; w < ui2_width; w++)
                        {
                            if (pu1_src_buf[w])
                            {
                                if (pu1_src_edge_buf[w] > 0)
                                {
                                    ui2_fg_ARGB_glph_a = (UINT16) GL_COLOR_ARGB4444(
                                                    pu1_src_buf[w] | pu1_src_edge_buf[w],
                                                    _BLEND_CLR(pt_char_color->u1.r, pu1_src_buf[w], pt_ORN_color->u1.r, pu1_src_edge_buf[w]),
                                                    _BLEND_CLR(pt_char_color->u2.g, pu1_src_buf[w], pt_ORN_color->u2.g, pu1_src_edge_buf[w]),
                                                    _BLEND_CLR(pt_char_color->u3.b, pu1_src_buf[w], pt_ORN_color->u3.b, pu1_src_edge_buf[w]));
                                }
                                else
                                {
                                    ui2_fg_ARGB_glph_a = (UINT16)GL_COLOR_ARGB4444( pu1_src_buf[w], pt_char_color->u1.r, pt_char_color->u2.g, pt_char_color->u3.b );
                                }
                                
                                *((UINT16 *)pui1_pb_dst) = ui2_fg_ARGB_glph_a;
                            }
                            else if (pu1_src_edge_buf[w] > 0)
                            {
                                ui2_fg_ARGB_glph_a = (UINT16)GL_COLOR_ARGB4444( pu1_src_edge_buf[w], pt_ORN_color->u1.r, pt_ORN_color->u2.g, pt_ORN_color->u3.b );
                                *((UINT16 *)pui1_pb_dst) = ui2_fg_ARGB_glph_a;
                            }

                            pui1_pb_dst += 2;
                        }

                        pu1_src_buf         += ui2_width;
                        pu1_src_edge_buf    += ui2_width;
                        pu1_dst_buf         += ui4_dst_pitch;
                    }
                }
                else
                {
                    for (h = 0; h < ui2_height; h++)
                    {
                        pui1_pb_dst = pu1_dst_buf + ui2_left * 2;
                        for (w = 0; w < ui2_width; w++)
                        {
                            ui2_fg_ARGB_glph_a = (UINT16)GL_COLOR_ARGB4444( pu1_src_buf[w], pt_char_color->u1.r, pt_char_color->u2.g, pt_char_color->u3.b );
                            *((UINT16 *)pui1_pb_dst) = ui2_fg_ARGB_glph_a;

                            pui1_pb_dst += 2;
                        }

                        pu1_src_buf += ui2_width;
                        pu1_dst_buf += ui4_dst_pitch;
                    }
                }
                break;

            case GL_COLORMODE_ARGB_D8888:
                if (fg_need_ORN)
                {
                    for (h = 0; h < ui2_height; h++)
                    {
                        pui1_pb_dst = pu1_dst_buf + ui2_left * 4;
                        for (w = 0; w < ui2_width; w++)
                        {
                            if (pu1_src_buf[w])
                            {
                                if (pu1_src_edge_buf[w] > 0)
                                {
                                    ui4_fg_ARGB_glph_a = (UINT32) GL_COLOR_ARGB8888(
                                                    pu1_src_buf[w] | pu1_src_edge_buf[w],
                                                    _BLEND_CLR(pt_char_color->u1.r, pu1_src_buf[w], pt_ORN_color->u1.r, pu1_src_edge_buf[w]),
                                                    _BLEND_CLR(pt_char_color->u2.g, pu1_src_buf[w], pt_ORN_color->u2.g, pu1_src_edge_buf[w]),
                                                    _BLEND_CLR(pt_char_color->u3.b, pu1_src_buf[w], pt_ORN_color->u3.b, pu1_src_edge_buf[w]));
                                }
                                else
                                {
                                    ui4_fg_ARGB_glph_a = (UINT32)GL_COLOR_ARGB8888( pu1_src_buf[w], pt_char_color->u1.r, pt_char_color->u2.g, pt_char_color->u3.b );
                                }
                                
                                *((UINT32 *)pui1_pb_dst) = ui4_fg_ARGB_glph_a;
                            }
                            else if (pu1_src_edge_buf[w] > 0)
                            {
                                ui4_fg_ARGB_glph_a = (UINT32)GL_COLOR_ARGB8888( pu1_src_edge_buf[w], pt_ORN_color->u1.r, pt_ORN_color->u2.g, pt_ORN_color->u3.b );
                                *((UINT32 *)pui1_pb_dst) = ui4_fg_ARGB_glph_a;
                            }

                            pui1_pb_dst += 4;
                        }

                        pu1_src_buf         += ui2_width;
                        pu1_src_edge_buf    += ui2_width;
                        pu1_dst_buf         += ui4_dst_pitch;
                    }
                }
                else
                {
                    for (h = 0; h < ui2_height; h++)
                    {
                        pui1_pb_dst = pu1_dst_buf + ui2_left * 4;
                        for (w = 0; w < ui2_width; w++)
                        {
                            ui4_fg_ARGB_glph_a = (UINT32)GL_COLOR_ARGB8888( pu1_src_buf[w], pt_char_color->u1.r, pt_char_color->u2.g, pt_char_color->u3.b );
                            *((UINT32 *)pui1_pb_dst) = ui4_fg_ARGB_glph_a;

                            pui1_pb_dst += 4;
                        }

                        pu1_src_buf += ui2_width;
                        pu1_dst_buf += ui4_dst_pitch;
                    }
                }
                break;

            default:
                break;
        }

        x_gl_surface_unlock( h_grid_surf, NULL );
    }

    FT_Done_Glyph( t_glyph );

    if (fg_is_equal_width)
    {
        *pui2_grid_act_width = pt_grid->ui2_grid_h;
    }
    else
    {
        *pui2_grid_act_width = pt_grid->ui2_grid_h - pt_grid->ui2_char_h + ui2_width;
    }

    x_sema_unlock( h_g_gl_isdb_sema_hdl );

    return GLR_OK; 
}


#endif
