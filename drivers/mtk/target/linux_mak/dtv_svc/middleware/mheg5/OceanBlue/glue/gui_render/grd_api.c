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
#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/u_common.h"
#include "handle/x_handle.h"
#include "graphic/gl.h"
#include "graphic/x_gl.h"
#include "graphic/u_gl_nc.h"
#include "wgl/x_wgl.h"
#include "wgl/u_wgl_sets.h"
#include "strm_mngr/scc/x_scc.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/OceanBlue/glue/gui_render/gfx_rnd.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "chip_spec/u_chip_spec.h"
#include "chip_spec/x_chip_spec.h"

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_gui_get_dbg_level()

static UINT16    _ui2_mheg5_gui_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#endif

/*-----------------------------------------------------------------------------
* structure, constants, macro definitions
*---------------------------------------------------------------------------*/
#define MHEG5_MAIN_FRAME_ALPHA          ((UINT8) 255)
#define MHEG5_MAIN_FRAME_ALPHA_LIGHTER  ((UINT8) 100)

#define MHEG5_DIVIDE_WITH_ROUND(dividend, divisor)        (divisor==0? ((dividend) + ((divisor) >> 1)) :(((dividend) + ((divisor) >> 1)) / (divisor)))

/*-----------------------------------------------------------------------------
* variable declarations
*---------------------------------------------------------------------------*/
static HANDLE_T                     _h_mheg5_canvas           = NULL_HANDLE;
static HANDLE_T                     _h_sbtl_canvas            = NULL_HANDLE;
static HANDLE_T                     _h_main_frm               = NULL_HANDLE;
static GL_COLOR_T                   _t_color_key              = GL_COLOR_DEF(0, 0, 0, 0);
static x_mheg5_eng_state_nfy_fct    _pf_state_handler_nfy     = NULL;
static VOID*                        _pv_state_handler_nfy_tag = NULL;
static UINT32                       _ui4_mheg5_canvas_w;
static UINT32                       _ui4_mheg5_canvas_h;
static GL_RECT_T                    _t_rc_ui;
static GL_RECT_T                    _t_rs_ui;
static GL_RECT_T                    _t_update_region;
static SCC_VID_OVER_SCAN_CLIPPER_T  _t_dflt_ovs_clip          = {0, 0, 0, 0};

/* used for adjusting the widget order */
static BOOL                         _b_order_adjusted = FALSE;
static HANDLE_T                     _h_ref_widget;
static WGL_INSERT_MODE_T            _e_insert_mode;

static INT32                        _i4_adjust_for_none_auto = 0;


/*-----------------------------------------------------------------------------
* private methods declarations
*---------------------------------------------------------------------------*/
static VOID _calc_osd_adj_auto(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h);

static VOID _calc_osd_adj_force_4x3(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h);

static VOID _calc_osd_adj_fullscreen(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h);

static UINT8* _gen_pal_cnv_tbl(GL_HPALETTE_T h_pal);

static INT32 _main_frame_proc(
    HANDLE_T    h_widget,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2);

static INT32 _paint(
    HANDLE_T        h_widget, 
    GL_RECT_T*      pt_rect,
    GL_HGRAPHICS_T  h_graphics, 
    VOID*           pv_param);

static VOID _calc_osd_adj_auto(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h)
{
    switch (pt_cnv_cfg->e_wss)
    {
        case MHEG5_WSS_4_3FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = (_ui4_mheg5_canvas_w * 3) >> 2;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            break;
            
        case MHEG5_WSS_16_9FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16_9 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = ((_ui4_mheg5_canvas_h * 3) >> 2);
            }
            break;

        case MHEG5_WSS_14_9:
        case MHEG5_WSS_4_3_SP_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w * 6 / 7;
                *pui4_h = _ui4_mheg5_canvas_h * 7 / 6;
            }
            break;
                
        case MHEG5_WSS_16_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h * 4 / 3;
            }
            break;
        
        default:
            break;
    }
}

static VOID _calc_osd_adj_force_4x3(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h)
{
    if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
    {
        *pui4_w = _ui4_mheg5_canvas_w;
        *pui4_h = _ui4_mheg5_canvas_h;
    }
    else
    {
        *pui4_w = (_ui4_mheg5_canvas_w * 3) >> 2;
        *pui4_h = _ui4_mheg5_canvas_h;
    }
}

static VOID _calc_osd_adj_fullscreen(
    const MHEG5_CNV_CFG_T*  pt_cnv_cfg, 
    UINT32*                 pui4_w, 
    UINT32*                 pui4_h)
{
    switch (pt_cnv_cfg->e_wss)
    {
        case MHEG5_WSS_4_3FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            break;
            
        case MHEG5_WSS_16_9FF:
            if (MHEG5_DISPLAY_ASPECT_RATIO_16_9 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            
            break;

        case MHEG5_WSS_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;

                if (MHEG5_DECFC_LETTER_BOX_14_9 == pt_cnv_cfg->e_decfc)
                {
                    *pui4_h = _ui4_mheg5_canvas_h * 7 / 6;
                }
                else
                {
                    *pui4_h = _ui4_mheg5_canvas_h;
                }
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h * 7 / 6;
            }
            break;
            
        case MHEG5_WSS_4_3_SP_14_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h;
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h * 7 / 6;
            }
            break;
                
        case MHEG5_WSS_16_9:
            if (MHEG5_DISPLAY_ASPECT_RATIO_4_3 == pt_cnv_cfg->e_display_ar)
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                
                if (MHEG5_DECFC_LETTER_BOX == pt_cnv_cfg->e_decfc)
                {
                    *pui4_h = (_ui4_mheg5_canvas_h << 2) / 3;
                }
                else
                {
                    *pui4_h = _ui4_mheg5_canvas_h;
                }
            }
            else
            {
                *pui4_w = _ui4_mheg5_canvas_w;
                *pui4_h = _ui4_mheg5_canvas_h * 4 / 3;
            }
            break;
            
        default:
                break;
            
    }
}

/*-----------------------------------------------------------------------------
* Name: _convert_rgb_2_yuv
*
* Description: This API convert rgb value to yuv value.
*
* Inputs:   pt_src_color    Pointer to source color(rgb).
*
* Outputs:  pt_tgt_color.   Pointer to target color(yuv).
*
* Returns:  None
*
----------------------------------------------------------------------------*/
static VOID 
_convert_rgb_2_yuv( GL_COLOR_T* pt_tgt_color, const GL_COLOR_T* pt_src_color )
{
    INT32 i4_y = (INT32)(0.299 * pt_src_color->u1.r + 
        0.875 * pt_src_color->u2.g + 0.114 * pt_src_color->u3.b);
    INT32 i4_u = (INT32)(-0.147 * pt_src_color->u1.r - 
        0.289 * pt_src_color->u2.g + 0.436 * pt_src_color->u3.b);
    INT32 i4_v = (INT32)(0.615 * pt_src_color->u1.r -
        0.515 * pt_src_color->u2.g + 0.100 * pt_src_color->u3.b);

    pt_tgt_color->a     = pt_src_color->a;
    pt_tgt_color->u1.y  = (UINT8)(i4_y < 0 ? 0 : (i4_y > 255 ? 255 : (UINT8)i4_y));
    pt_tgt_color->u2.u  = (UINT8)(i4_u < 0 ? 0 : (i4_u > 255 ? 255 : (UINT8)i4_u));
    pt_tgt_color->u3.v  = (UINT8)(i4_v < 0 ? 0 : (i4_v > 255 ? 255 : (UINT8)i4_v));
}

/*-----------------------------------------------------------------------------
* Name: _convert_yuv_2_rgb
*
* Description: This API convert rgb value to yuv value.
*
* Inputs:   pt_src_color    Pointer to source color(rgb).
*
* Outputs:  pt_tgt_color.   Pointer to target color(yuv).
*
* Returns:  None
*
----------------------------------------------------------------------------*/
static VOID 
_convert_yuv_2_rgb( GL_COLOR_T* pt_tgt_color, const GL_COLOR_T* pt_src_color )
{
#if 1
    INT32 i4_r = (INT32)((INT32)pt_src_color->u1.y + 1.4075 * ((INT32)pt_src_color->u3.v - 128));
    INT32 i4_g = (INT32)((INT32)pt_src_color->u1.y - 0.3455 * ((INT32)pt_src_color->u2.u - 128) - 0.7169 * ((INT32)pt_src_color->u3.v - 128));
    INT32 i4_b = (INT32)((INT32)pt_src_color->u1.y + 1.779 * ((INT32)pt_src_color->u2.u - 128));
#else
    INT32 i4_r = (INT32)((INT32)pt_src_color->u1.y + 1.13983 * ((INT32)pt_src_color->u3.v));
    INT32 i4_g = (INT32)((INT32)pt_src_color->u1.y - 0.39465 * ((INT32)pt_src_color->u2.u) - 0.58060 * ((INT32)pt_src_color->u3.v - 128));
    INT32 i4_b = (INT32)((INT32)pt_src_color->u1.y + 2.03211 * ((INT32)pt_src_color->u2.u));
#endif
    pt_tgt_color->a     = pt_src_color->a;
    pt_tgt_color->u1.y  = (UINT8)(i4_r < 0 ? 0 : (i4_r > 255 ? 255 : (UINT8)i4_r));
    pt_tgt_color->u2.u  = (UINT8)(i4_g < 0 ? 0 : (i4_g > 255 ? 255 : (UINT8)i4_g));
    pt_tgt_color->u3.v  = (UINT8)(i4_b < 0 ? 0 : (i4_b > 255 ? 255 : (UINT8)i4_b));
}

/*-----------------------------------------------------------------------------
* private methods implementations
*---------------------------------------------------------------------------*/
static UINT8* _gen_pal_cnv_tbl(GL_HPALETTE_T h_pal)
{
#if 0
    #define PAL_CNV_TBL_SIZE    ((INT32) 65536)
    #define NUM_COLOR_PER_COMP  ((INT32) 256)
    #define COLOR_INCR          ((INT32) 17)
    #define INT_VAL             ((INT32) 0)
    
    UINT8*      pui1_tbl;
    GL_COLOR_T  t_color;
    UINT16      ui2_index;
    UINT16      ui2_color;
    INT32       a, r, g, b;
    
    pui1_tbl = (UINT8*) x_mem_alloc(sizeof(UINT8) * PAL_CNV_TBL_SIZE);

    if (pui1_tbl)
    {
        for (r = INT_VAL; r < NUM_COLOR_PER_COMP; r += COLOR_INCR)
        {
            t_color.u1.r = (UINT8) r;
            
            for (g = INT_VAL; g < NUM_COLOR_PER_COMP; g += COLOR_INCR)
            {
                t_color.u2.g = (UINT8) g;
                
                for (b = INT_VAL; b < NUM_COLOR_PER_COMP; b += COLOR_INCR)
                {
                    t_color.u3.b = (UINT8) b;
                    
                    for (a = INT_VAL; a < NUM_COLOR_PER_COMP; a += COLOR_INCR)
                    {
                        t_color.a = (UINT8) a;

                        if (a <= 24)
                        {
                            ui2_index = 64;
                        }
                        else if (a > 24 && a <= 230)
                        {
                            x_gl_palette_match_color_ex(
                                h_pal, 
                                65,
                                48,
                                FALSE, 
                                &t_color, 
                                NULL, 
                                &ui2_index);
                        }
                        else
                        {
                            INT32   d_rg = r - g, d_gb = g - b, d_rb = r - b;

                            if (d_rg < 0)
                            {
                                d_rg = -d_rg;
                            }

                            if (d_gb < 0)
                            {
                                d_gb = -d_gb;
                            }
                            
                            if (d_rb < 0)
                            {
                                d_rb = -d_rb;
                            }
                            

                            if (d_rg < COLOR_INCR && d_gb < COLOR_INCR && d_rb < COLOR_INCR)
                            {
                                x_gl_palette_match_color_ex(
                                    h_pal, 
                                    113,
                                    139,
                                    FALSE, 
                                    &t_color, 
                                    NULL, 
                                    &ui2_index);
                            }
                            else
                            {
                                x_gl_palette_match_color_ex(
                                    h_pal, 
                                    117,
                                    135,
                                    FALSE, 
                                    &t_color, 
                                    NULL, 
                                    &ui2_index);
                            }
                        }
                        
                        ui2_color = GL_COLOR_ARGB4444(t_color.a, t_color.u1.r, t_color.u2.g, t_color.u3.b);
                        pui1_tbl[ui2_color] = (UINT8) ui2_index;
                    }
                }
            }
        }
    }

    for (a = INT_VAL; a < NUM_COLOR_PER_COMP; a++)
    {
        if (a >= 25 && a <= 230)
        {
            g_aui1_alpha_cnv_tbl[a] = (UINT8) (((UINT32) a - 25) / 205 * 191 + 32);
        }
        else
        {
            g_aui1_alpha_cnv_tbl[a] = a;
        }
    }

    for (a = INT_VAL; a < NUM_COLOR_PER_COMP; a++)
    {
        if (a >= 25 && a <= 230)
        {
            g_aui1_alpha_cnv_tbl[a] = (UINT8) (((UINT32) a - 25) * 191 / 205 + 32);
        }
        else
        {
            g_aui1_alpha_cnv_tbl[a] = a;
        }
    }
#if 0
    {
        UINT8* pui1_tmp = pui1_tbl;
        
        for (r = 0; r < PAL_CNV_TBL_SIZE; r += 16)
        {
            for (a = 0 ; a < 16; a++)
            {
                x_dbg_stmt("0x%2x, ", pui1_tmp[a]);
            }

            x_dbg_stmt("\r\n");
            
            pui1_tmp += 16;
        }

        x_dbg_stmt("\r\n\r\n*******************************************************\r\n\r\n");
                
        pui1_tmp = g_aui1_alpha_cnv_tbl;
        
        for (r = INT_VAL; r < NUM_COLOR_PER_COMP; r += 16)
        {
            for (a = 0 ; a < 16; a++)
            {
                x_dbg_stmt("0x%2x, ", pui1_tmp[a]);
            }

            x_dbg_stmt("\r\n");
            
            pui1_tmp += 16;
        }
    }
#endif
    return (pui1_tbl);
#else
    return (g_aui1_mheg5_pal_cnv_tbl);
#endif
}

static INT32 _main_frame_proc(
    HANDLE_T    h_widget,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    INT32 i4_ret;
    
    switch (ui4_msg)
    {
        case WGL_MSG_GET_FOCUS:
            break;
            
        case WGL_MSG_LOSE_FOCUS:
            break;
            
        case WGL_MSG_KEY_DOWN:
            if (_pf_state_handler_nfy)
            {
                (*_pf_state_handler_nfy)(MHEG5_MSG_GRD_KEY_DOWN, pv_param1, _pv_state_handler_nfy_tag);
            }
            return WGLR_OK;
            
        case WGL_MSG_PAINT:
        {
            GL_RECT_T   t_rect;
            UINT32      ui4_num;
            
            i4_ret = x_wgl_get_update_region(h_widget, &ui4_num, &t_rect);

            if (WGLR_OK == i4_ret)
            {
                i4_ret = _paint(
                    h_widget, 
                    &t_rect, 
                    (GL_HGRAPHICS_T) pv_param1,
                    NULL);
            }

            return (i4_ret);
        }

        case WGL_MSG_CHANGE_COLOR_CFG:
        {
            WGL_CVS_CLR_CFG_T* pt_cvs_cfg = (WGL_CVS_CLR_CFG_T*) pv_param1;
            
            if (pt_cvs_cfg->ui4_flag & WGL_CANVAS_CFG_PALETTE)
            {
                MHEG5_DBG_INFO(("{MHEG-5 Stack} Canvas palette is changed!\r\n"));
            }
            
            if (pt_cvs_cfg->ui4_flag & WGL_CANVAS_CFG_COLORKEY)
            {
                MHEG5_DBG_INFO(("{MHEG-5 Stack} Canvas colorkey is changed!\r\n"));
            }
        }

        break;

        
        default:
            break;
    }

    i4_ret = x_wgl_default_msg_proc(
        h_widget, 
        ui4_msg, 
        pv_param1, 
        pv_param2);
    
    return (i4_ret);
}

static VOID _notify_state_change(
    UINT32      ui4_msg, 
    VOID*       pv_param)
{
    _pf_state_handler_nfy(ui4_msg, pv_param, _pv_state_handler_nfy_tag);
}

static INT32 _paint(
    HANDLE_T        h_widget, 
    GL_RECT_T*      pt_rect,
    GL_HGRAPHICS_T  h_graphics, 
    VOID*           pv_param)
{
    INT32           i4_res;
    CRIT_STATE_T    t_cs;
    GL_RECT_T       t_rc;
    GL_RECT_T       t_rs;
    GL_RECT_T       t_old_clip;
    BOOL            b_notify = FALSE;
    BOOL            b_state  = FALSE;
    INT32           i4_x_offset;
    INT32           i4_adjust_for_none_auto;
    HANDLE_T        h_src_surf = NULL_HANDLE;
    
    if (RECT_EMPTY(&_t_update_region))
    {
        return WGLR_OK;
    }
    
    t_cs = x_crit_start();
    t_rc = _t_rc_ui;
    t_rs = _t_rs_ui;
    i4_adjust_for_none_auto = _i4_adjust_for_none_auto;
    x_crit_end(t_cs);

    if (!RECT_INCLUDED(pt_rect, &_t_update_region))
    {
        x_gl_set_color(
            h_graphics,
            &_t_color_key);
        
        i4_res = x_gl_fill_rect(
            h_graphics, 
            pt_rect);
    }

    x_sema_lock(g_t_surf.h_lock, X_SEMA_OPTION_WAIT);
    h_src_surf = g_t_surf.h_surf;
    
    /* set update clip */
    i4_res = x_gl_get_clip(h_graphics, &t_old_clip);

    i4_res = x_gl_set_clip(h_graphics, &_t_update_region);

    /* for MT5360B scaler workaround (use offset := 1) */
    i4_x_offset = 0;

    if(i4_adjust_for_none_auto == 1)
    {
        GL_COLOR_T t_color = GL_COLOR_DEF(0, 0, 0, 0);
       
        x_gl_surface_fill(g_t_surf.h_surf_3rd, &t_color, 0);
            
        x_gl_surface_stretch_blt(
                    g_t_surf.h_surf_3rd, 
                    g_t_surf.h_surf, 
                    g_t_surf.ui4_width / 8,
                    0,
                    g_t_surf.ui4_width * 3/ 4,
                    g_t_surf.ui4_height,
                    0,
                    0,
                    g_t_surf.ui4_width,
                    g_t_surf.ui4_height,
                    0);
        h_src_surf = g_t_surf.h_surf_3rd;

    }
    #if 0
    else if(i4_adjust_for_none_auto == 2)
    {
        GL_COLOR_T t_color = GL_COLOR_DEF(0, 0, 0, 0);

        x_gl_surface_fill(g_t_surf.h_surf_3rd, &t_color, 0);

        x_gl_surface_stretch_blt(
                    g_t_surf.h_surf_3rd, 
                    g_t_surf.h_surf, 
                    0,
                    0,
                    g_t_surf.ui4_width,
                    g_t_surf.ui4_height,
                    0,
                    g_t_surf.ui4_height/8,
                    g_t_surf.ui4_width,
                    g_t_surf.ui4_height * 3 /4,
                    0);
         
        h_src_surf = g_t_surf.h_surf_3rd;
    
    }
    #endif

    if (NULL_HANDLE != g_t_surf.h_surf)
    {
        if (g_t_surf.e_ture_col_mode == GL_COLORMODE_ARGB_CLUT8 &&
            g_t_surf.h_surf_bak != NULL_HANDLE)
        {
            i4_res = x_gl_stretch_blt(
                h_graphics, 
                g_t_surf.h_surf_bak, 
                t_rc.i4_left - i4_x_offset,
                t_rc.i4_top,
                (UINT32)(RECT_W(&t_rc) - i4_x_offset),
                (UINT32)RECT_H(&t_rc),
                t_rs.i4_left,
                t_rs.i4_top,
                (UINT32)RECT_W(&t_rs),
                (UINT32)RECT_H(&t_rs));
        }
        else
        {
            i4_res = x_gl_adv_stretch_blt(
                h_graphics, 
                h_src_surf, 
                t_rc.i4_left - i4_x_offset,
                t_rc.i4_top,
                (UINT32)(RECT_W(&t_rc) - i4_x_offset),
                (UINT32)RECT_H(&t_rc),
                t_rs.i4_left,
                t_rs.i4_top,
                (UINT32)RECT_W(&t_rs),
                (UINT32)RECT_H(&t_rs));
        }
    }
   
    i4_res = x_gl_set_clip(h_graphics, &t_old_clip);

    MHEG5_DBG_INFO((
        "\n{MHEG5 Stack} v = %d, s = %d\n", 
        g_t_surf.b_view_rendered, 
        g_t_surf.b_surf_rendered));

    if (g_t_surf.b_view_rendered != g_t_surf.b_surf_rendered)
    {
        MHEG5_DBG_INFO(("\n{MHEG5 Stack} status changed, send notify...\n\n"));
        g_t_surf.b_view_rendered = g_t_surf.b_surf_rendered;
        b_state  = g_t_surf.b_view_rendered;
        b_notify = TRUE;
    }

	g_t_surf.h_dest_graphics = h_graphics;
    
    x_sema_unlock(g_t_surf.h_lock);
    
    if (b_notify)
    {
        _notify_state_change(MHEG5_MSG_GRD_RENDERED, (VOID*)((UINT32)b_state));
    }
    else if (g_t_surf.b_view_rendered)
    {
        _notify_state_change(MHEG5_MSG_GRD_DRAWN, (VOID*) TRUE);
    }

    return ((GLR_OK == i4_res) ? WGLR_OK : WGLR_INTERNAL_ERROR);
}


/*-----------------------------------------------------------------------------
* public methods implementations
*---------------------------------------------------------------------------*/
VOID grd_adjust_order(HANDLE_T h_ref_widget, WGL_INSERT_MODE_T e_insert)
{
    _b_order_adjusted = TRUE;
    _h_ref_widget     = h_ref_widget;
    _e_insert_mode    = e_insert;
}

INT32 grd_adjust_osd(const MHEG5_CNV_CFG_T* pt_cnv_cfg)
{
    INT32           i4_x, i4_y;
    UINT32          ui4_w, ui4_h;
    CRIT_STATE_T    t_cs;
    GL_RECT_T       t_rc;
    
    ui4_w = _ui4_mheg5_canvas_w;
    ui4_h = _ui4_mheg5_canvas_h;
    
    switch (pt_cnv_cfg->e_vid_mode)
    {
        case MHEG5_VIDEO_MODE_AUTO:
            _calc_osd_adj_auto(pt_cnv_cfg, &ui4_w, &ui4_h);
            break;

        case MHEG5_VIDEO_MODE_FULLSCREEN:
            _calc_osd_adj_fullscreen(pt_cnv_cfg, &ui4_w, &ui4_h);
            break;

        case MHEG5_VIDEO_MODE_FORCE_4x3:
        case MHEG5_VIDEO_MODE_GEN_LETTER_BOX:
            _calc_osd_adj_force_4x3(pt_cnv_cfg, &ui4_w, &ui4_h);
            break;
            
        default:
            _calc_osd_adj_fullscreen(pt_cnv_cfg, &ui4_w, &ui4_h);
            break;
    }
    
    i4_x = ((INT32) _ui4_mheg5_canvas_w - (INT32) ui4_w) / 2;
    i4_y = ((INT32) _ui4_mheg5_canvas_h - (INT32) ui4_h) / 2;

    SET_RECT_BY_SIZE(&t_rc, i4_x, i4_y, (INT32) ui4_w, (INT32) ui4_h);

    SET_RECT_BY_SIZE(&_t_update_region, i4_x, i4_y, (INT32)ui4_w, (INT32)ui4_h);
    /* In order to avoid rounding error, expand the UI rectangle a little bit */
    EXPAND_RECT(&_t_update_region, 1);
    
    if (NULL_HANDLE != pt_cnv_cfg->h_scc_video)
    {
        SCC_VID_OVER_SCAN_CLIPPER_T t_ovs_clip;
        
        if (SMR_OK == x_scc_vid_get_overscan_clipper(pt_cnv_cfg->h_scc_video, &t_ovs_clip))
        {
            INT32 i4_sum, i4_len, x1, x2;
            
            if ((0 == t_ovs_clip.ui4_left)  &&
                (0 == t_ovs_clip.ui4_right) &&
                (0 == t_ovs_clip.ui4_top)   &&
                (0 == t_ovs_clip.ui4_bottom))
            {
                t_ovs_clip = _t_dflt_ovs_clip;
            }

            i4_len = (INT32) ui4_w;
            i4_sum = (INT32) t_ovs_clip.ui4_left + (INT32) t_ovs_clip.ui4_right;
            x1 = (INT32)(((((INT64)(i4_len * t_ovs_clip.ui4_left)) << 16) / (INT64)(VSH_REGION_MAX_WIDTH - i4_sum) + 0x8000) >> 16);
            x2 = (INT32)(((((INT64)(i4_len * t_ovs_clip.ui4_right)) << 16) / (INT64)(VSH_REGION_MAX_WIDTH - i4_sum)) >> 16);

            t_rc.i4_left  = - x1;
            t_rc.i4_right = i4_len + x2;
            
            i4_len = (INT32) ui4_h;
            i4_sum = (INT32) t_ovs_clip.ui4_top + (INT32) t_ovs_clip.ui4_bottom;
            x1 = (INT32)(((((INT64)(i4_len * t_ovs_clip.ui4_top)) << 16) / (INT64)(VSH_REGION_MAX_HEIGHT - i4_sum) + 0x8000) >> 16);
            x2 = (INT32)(((((INT64)(i4_len * t_ovs_clip.ui4_bottom)) << 16) / (INT64)(VSH_REGION_MAX_HEIGHT - i4_sum)) >> 16);

            t_rc.i4_top  = - x1;
            t_rc.i4_bottom = i4_len + x2;

            MOVE_RECT(&t_rc, i4_x, i4_y);
        }
    }
    
    /* In order to avoid rounding error, expand the UI rectangle a little bit */
    EXPAND_RECT(&t_rc, 1);
    
    t_cs = x_crit_start();
    
    _t_rc_ui = t_rc;
    
    _t_rs_ui.i4_left= 0;
    _t_rs_ui.i4_top= 0; 
    _t_rs_ui.i4_right = g_t_surf.ui4_width;
    _t_rs_ui.i4_bottom = g_t_surf.ui4_height;
    
    _i4_adjust_for_none_auto = 0;
        
    x_crit_end(t_cs);
    x_wgl_repaint(_h_main_frm, NULL, FALSE);
    
    return MHEG5R_OK;
}

INT32 grd_adjust_osd_ex(
    const MHEG5_CNV_CFG_T*        pt_cnv_cfg,
    const VSH_REGION_INFO_T*      pt_orig_src_reg,
    const VSH_REGION_INFO_T*      pt_full_src_reg,
    const VSH_REGION_INFO_T*      pt_full_dst_reg,
    const VSH_REGION_INFO_T*      pt_xlat_src_reg,
    const VSH_REGION_INFO_T*      pt_xlat_dst_reg)
{
    CRIT_STATE_T    t_cs;
    GL_RECT_T       t_rc_dst;
    GL_RECT_T       t_rc_src;
    
    VSH_REGION_INFO_T   t_src_reg;
    VSH_REGION_INFO_T   t_dst_reg;
    VSH_REGION_INFO_T   t_src_af_overscan;
    VSH_REGION_INFO_T   t_final_src_reg;
    VSH_REGION_INFO_T   t_final_dst_reg;
    
    if((NULL == pt_cnv_cfg)         ||
       (NULL == pt_orig_src_reg)    ||
       (NULL == pt_full_src_reg)    ||
       (NULL == pt_full_dst_reg)    ||
       (NULL == pt_xlat_src_reg)    ||
       (NULL == pt_xlat_dst_reg))
    {
        return MHEG5R_INV_ARG;
    }
    #if 0
    x_dbg_stmt("[1]grd_adjust_osd_ex:pt_xlat_src_reg[x:%lu, y:%lu, w:%lu, h:%lu],pt_xlat_dst_reg[x:%lu, y:%lu, w:%lu, h:%lu]\n",
        pt_xlat_src_reg->ui4_x,
        pt_xlat_src_reg->ui4_y,
        pt_xlat_src_reg->ui4_width,
        pt_xlat_src_reg->ui4_height,
        pt_xlat_dst_reg->ui4_x,
        pt_xlat_dst_reg->ui4_y,
        pt_xlat_dst_reg->ui4_width,
        pt_xlat_dst_reg->ui4_height);
    #endif

    x_memset(&t_src_reg, 0, sizeof(VSH_REGION_INFO_T));
    x_memset(&t_dst_reg, 0, sizeof(VSH_REGION_INFO_T));
    x_memset(&t_src_af_overscan, 0, sizeof(VSH_REGION_INFO_T));
    x_memset(&t_final_src_reg, 0, sizeof(VSH_REGION_INFO_T));
    x_memset(&t_final_dst_reg, 0, sizeof(VSH_REGION_INFO_T));
    
    t_src_reg.ui4_x      = pt_full_src_reg->ui4_x      + ((pt_xlat_src_reg->ui4_x      * pt_full_src_reg->ui4_width)  / VSH_REGION_MAX_WIDTH);
    t_src_reg.ui4_y      = pt_full_src_reg->ui4_y      + ((pt_xlat_src_reg->ui4_y      * pt_full_src_reg->ui4_height) / VSH_REGION_MAX_HEIGHT);
    t_src_reg.ui4_width  = ((pt_xlat_src_reg->ui4_width  * pt_full_src_reg->ui4_width)  / VSH_REGION_MAX_WIDTH);
    t_src_reg.ui4_height = ((pt_xlat_src_reg->ui4_height * pt_full_src_reg->ui4_height) / VSH_REGION_MAX_HEIGHT);

    t_dst_reg.ui4_x      = pt_full_dst_reg->ui4_x      + ((pt_xlat_dst_reg->ui4_x      * pt_full_dst_reg->ui4_width)  / VSH_REGION_MAX_WIDTH);
    t_dst_reg.ui4_y      = pt_full_dst_reg->ui4_y      + ((pt_xlat_dst_reg->ui4_y      * pt_full_dst_reg->ui4_height) / VSH_REGION_MAX_HEIGHT);
    t_dst_reg.ui4_width  = ((pt_xlat_dst_reg->ui4_width  * pt_full_dst_reg->ui4_width)  / VSH_REGION_MAX_WIDTH);
    t_dst_reg.ui4_height = ((pt_xlat_dst_reg->ui4_height * pt_full_dst_reg->ui4_height) / VSH_REGION_MAX_HEIGHT);
    
    if (x_handle_valid(pt_cnv_cfg->h_scc_video))
    {
        SCC_VID_OVER_SCAN_CLIPPER_T t_ovs_clip;
        x_memset(&t_ovs_clip, 0, sizeof(SCC_VID_OVER_SCAN_CLIPPER_T));
        
        if (SMR_OK == x_scc_vid_get_overscan_clipper(pt_cnv_cfg->h_scc_video, &t_ovs_clip))
        {
            t_src_af_overscan.ui4_x      =  g_t_surf.ui4_width  * t_ovs_clip.ui4_left / VSH_REGION_MAX_WIDTH;
            t_src_af_overscan.ui4_y      =  g_t_surf.ui4_height * t_ovs_clip.ui4_top  / VSH_REGION_MAX_HEIGHT;
            t_src_af_overscan.ui4_width  =  g_t_surf.ui4_width  - (g_t_surf.ui4_width * t_ovs_clip.ui4_left / VSH_REGION_MAX_WIDTH)  - (g_t_surf.ui4_width * t_ovs_clip.ui4_right  / VSH_REGION_MAX_WIDTH);
            t_src_af_overscan.ui4_height =  g_t_surf.ui4_height - (g_t_surf.ui4_height* t_ovs_clip.ui4_top  / VSH_REGION_MAX_HEIGHT) - (g_t_surf.ui4_height * t_ovs_clip.ui4_bottom / VSH_REGION_MAX_HEIGHT);
        }
        else
        {
            x_dbg_stmt("{MHEG5} get over scan clipper failed \n");
        }
    }
    else
    {
        x_dbg_stmt("{MHEG5} invalid scc video handle \n");
    }

    t_final_src_reg.ui4_x      = t_src_af_overscan.ui4_x + MHEG5_DIVIDE_WITH_ROUND(t_src_af_overscan.ui4_width  * t_src_reg.ui4_x, VSH_REGION_MAX_WIDTH);
    t_final_src_reg.ui4_y      = t_src_af_overscan.ui4_y + MHEG5_DIVIDE_WITH_ROUND(t_src_af_overscan.ui4_height * t_src_reg.ui4_y, VSH_REGION_MAX_HEIGHT);
    t_final_src_reg.ui4_width  = MHEG5_DIVIDE_WITH_ROUND(t_src_af_overscan.ui4_width  * t_src_reg.ui4_width,  VSH_REGION_MAX_WIDTH);
    t_final_src_reg.ui4_height = MHEG5_DIVIDE_WITH_ROUND(t_src_af_overscan.ui4_height * t_src_reg.ui4_height, VSH_REGION_MAX_HEIGHT);
    
    t_final_dst_reg.ui4_x      = MHEG5_DIVIDE_WITH_ROUND(_ui4_mheg5_canvas_w * t_dst_reg.ui4_x, VSH_REGION_MAX_WIDTH);
    t_final_dst_reg.ui4_y      = MHEG5_DIVIDE_WITH_ROUND(_ui4_mheg5_canvas_h * t_dst_reg.ui4_y, VSH_REGION_MAX_HEIGHT);
    t_final_dst_reg.ui4_width  = MHEG5_DIVIDE_WITH_ROUND(_ui4_mheg5_canvas_w * t_dst_reg.ui4_width,  VSH_REGION_MAX_WIDTH);
    t_final_dst_reg.ui4_height = MHEG5_DIVIDE_WITH_ROUND(_ui4_mheg5_canvas_h * t_dst_reg.ui4_height, VSH_REGION_MAX_HEIGHT);

    #if 0
    x_dbg_stmt("[2]grd_adjust_osd_ex:t_final_src_reg[x:%lu, y:%lu, w:%lu, h:%lu],t_final_dst_reg[x:%lu, y:%lu, w:%lu, h:%lu]\n",
        t_final_src_reg.ui4_x,
        t_final_src_reg.ui4_y,
        t_final_src_reg.ui4_width,
        t_final_src_reg.ui4_height,
        t_final_dst_reg.ui4_x,
        t_final_dst_reg.ui4_y,
        t_final_dst_reg.ui4_width,
        t_final_dst_reg.ui4_height);
    #endif

    t_rc_dst.i4_left   = t_final_dst_reg.ui4_x;
    t_rc_dst.i4_top    = t_final_dst_reg.ui4_y;
    t_rc_dst.i4_bottom = t_final_dst_reg.ui4_y + t_final_dst_reg.ui4_height; 
    t_rc_dst.i4_right  = t_final_dst_reg.ui4_x + t_final_dst_reg.ui4_width;
#ifdef MW_MHEG5_OSD_ADJ_SOLUTION_BY_CHUN
    t_rc_src.i4_left   = t_final_src_reg.ui4_x;
    t_rc_src.i4_top    = t_final_src_reg.ui4_y;
    t_rc_src.i4_bottom = t_final_src_reg.ui4_y + t_final_src_reg.ui4_height; 
    t_rc_src.i4_right  = t_final_src_reg.ui4_x + t_final_src_reg.ui4_width;
#endif
    
    t_cs = x_crit_start();
                                                                                                      
    if((pt_cnv_cfg->e_scene_ar == MHEG5_ASPECT_RATIO_4_3) && 
       (pt_cnv_cfg->e_src_aspect_ratio == MHEG5_ASPECT_RATIO_16_9))
    {
        if(pt_cnv_cfg->e_wam == MHEG5_WAM_CENTRE_CUT_OUT)
        {
            _i4_adjust_for_none_auto = 1;
        }
        else
        {
            _i4_adjust_for_none_auto = 2;
            #ifdef MW_MHEG5_OSD_ADJ_SOLUTION_BY_CHUN

            INT32 i4_dh, i4_sh;

            i4_dh = t_rc_dst.i4_bottom - t_rc_dst.i4_top;
            i4_sh = t_rc_src.i4_bottom - t_rc_src.i4_top;

            if (t_rc_dst.i4_top > 0)
            {
                t_rc_src.i4_top = t_rc_src.i4_top - i4_sh * t_rc_dst.i4_top / i4_dh;
                t_rc_dst.i4_top = 0;
            }
            
            if (t_rc_dst.i4_bottom < (INT32)_ui4_mheg5_canvas_h)
            {
                i4_sh = i4_sh * _ui4_mheg5_canvas_h / i4_dh;
                t_rc_dst.i4_bottom = _ui4_mheg5_canvas_h; 
            }
            
            t_rc_src.i4_top = g_t_surf.ui4_height / 8 + t_rc_src.i4_top * 3 / 4;
            t_rc_src.i4_bottom = t_rc_src.i4_top + i4_sh * 3 / 4;;

            #else

            INT32 i4_height  = 0;
            
            i4_height = t_rc_dst.i4_bottom - t_rc_dst.i4_top;

            t_rc_dst.i4_top    = t_rc_dst.i4_top - MHEG5_DIVIDE_WITH_ROUND(i4_height, 6);
            t_rc_dst.i4_bottom = t_rc_dst.i4_top + MHEG5_DIVIDE_WITH_ROUND(i4_height * 4, 3); 

            t_final_src_reg.ui4_y = MHEG5_DIVIDE_WITH_ROUND((g_t_surf.ui4_height + (t_final_src_reg.ui4_y * 6) - t_final_src_reg.ui4_height), 8);

            #endif
        }
    }
    else
    {
        _i4_adjust_for_none_auto = 0;
    }
    #ifndef MW_MHEG5_OSD_ADJ_SOLUTION_BY_CHUN
    
    t_rc_src.i4_left   = t_final_src_reg.ui4_x;
    t_rc_src.i4_top    = t_final_src_reg.ui4_y;
    t_rc_src.i4_bottom = t_final_src_reg.ui4_y + t_final_src_reg.ui4_height; 
    t_rc_src.i4_right  = t_final_src_reg.ui4_x + t_final_src_reg.ui4_width;

    #endif

    _t_rs_ui = t_rc_src;
    _t_rc_ui = t_rc_dst;
    
    SET_RECT_BY_SIZE(&_t_update_region, 0, 0, (INT32)_ui4_mheg5_canvas_w, (INT32)_ui4_mheg5_canvas_h);

    x_crit_end(t_cs);

    x_wgl_repaint(_h_main_frm, NULL, FALSE);
    
    return MHEG5R_OK;
}


VOID grd_exit(VOID)
{
    if (NULL_HANDLE != g_t_surf.h_surf)
    {
        x_gl_obj_free(g_t_surf.h_surf);
        g_t_surf.h_surf = NULL_HANDLE;
    }
    if (NULL_HANDLE != g_t_surf.h_surf_buf)
    {
        x_gl_obj_free(g_t_surf.h_surf_buf);
        g_t_surf.h_surf_buf = NULL_HANDLE;
    }

    if (NULL_HANDLE != g_t_surf.h_palette)
    {
        x_gl_obj_free(g_t_surf.h_palette);
        g_t_surf.h_palette = NULL_HANDLE;
    }
    
    g_t_surf.b_surf_rendered = FALSE;
    g_t_surf.b_view_rendered = FALSE;
    g_t_surf.ui4_width  = (INT32) 0;
    g_t_surf.ui4_height = (INT32) 0;
}

HANDLE_T grd_get_mheg5_canvas(VOID)
{
    return (_h_mheg5_canvas);
}

HANDLE_T grd_get_sbtl_canvas(VOID)
{
    return (_h_sbtl_canvas);
}

HANDLE_T grd_get_main_frame(VOID)
{
    return (_h_main_frm);
}

INT32 grd_init(
    UINT32                              ui4_eng_code,
    const SCC_VID_OVER_SCAN_CLIPPER_T*  pt_ovs_clip)
{
    INT32 i4_ret = MHEG5R_INIT_FAIL;

    if (pt_ovs_clip)
    {
        _t_dflt_ovs_clip = *((SCC_VID_OVER_SCAN_CLIPPER_T*) pt_ovs_clip);

        i4_ret = x_sema_create(
            &(g_t_surf.h_lock), 
            X_SEMA_TYPE_MUTEX, 
            X_SEMA_STATE_UNLOCK);
        
        if (OSR_OK == i4_ret)
        {
            i4_ret = MHEG5R_OK;
        }
        else
        {
            i4_ret = MHEG5R_INIT_FAIL;
        }
    }

    return (i4_ret);
}
INT32    grd_reg_state_nfy(
    x_mheg5_eng_state_nfy_fct   pf_nfy, 
    VOID*                       pv_tag)
{
    _pf_state_handler_nfy     = pf_nfy;
    _pv_state_handler_nfy_tag = pv_tag;

    return MHEG5R_OK;
}

INT32 grd_init_osd(
    HANDLE_T                    h_mheg5_canvas,
    HANDLE_T                    h_sbtl_canvas,
    GL_HPALETTE_T               h_palette,
    const GL_COLOR_T*           pt_color_key)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    
    i4_ret = MHEG5R_INV_ARG;

    if (HR_OK == x_handle_get_type(h_mheg5_canvas, &e_type) && (HT_WGL_CANVAS == e_type)
        &&HR_OK == x_handle_get_type(h_sbtl_canvas, &e_type)  && (HT_WGL_CANVAS == e_type))
    {
        _h_mheg5_canvas  = h_mheg5_canvas;   
        _h_sbtl_canvas   = h_sbtl_canvas;

        if (NULL_HANDLE == h_palette)
        {
            g_t_surf.h_palette   = NULL_HANDLE;
            _t_color_key = *pt_color_key;
        }
        else
        {
            UINT16 ui2_index;
            
            g_t_surf.h_palette = gl_palette_duplicate_handle(h_palette);
            x_gl_palette_match_color_ex(
                h_palette, 
                64,
                188,
                TRUE, 
                pt_color_key, 
                NULL, 
                &ui2_index);

            _t_color_key.u3.index = (UINT8) ui2_index;
        }
       
        i4_ret = x_wgl_get_canvas_size(
            h_mheg5_canvas, 
            &_ui4_mheg5_canvas_w,
            &_ui4_mheg5_canvas_h);

        if (WGLR_OK == i4_ret)
        {
            SET_RECT_BY_SIZE(&_t_rc_ui, 0, 0, (INT32)_ui4_mheg5_canvas_w, (INT32)_ui4_mheg5_canvas_h);
            
            i4_ret = x_wgl_create_widget(
                h_mheg5_canvas, 
                HT_WGL_WIDGET_FRAME, 
                WGL_CONTENT_FRAME_DEF,
                WGL_BORDER_NULL,
                &_t_rc_ui, 
                _main_frame_proc, 
                MHEG5_MAIN_FRAME_ALPHA, 
                (VOID*) (WGL_STL_GL_NO_BK|WGL_STL_FRM_NO_OPTIMIZE),
                NULL,
                &_h_main_frm);
        }
        
        i4_ret = ((WGLR_OK == i4_ret) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
    }
    
    return (i4_ret);
}

INT32 grd_reconfig()
{
    INT32 i4_ret;
    WGL_CANVAS_INFO_T   t_cvs_info;
    GL_SURFACE_DESC_T   t_surf_desc;
    BOOL  b_is_bond_enable = FALSE;

#ifndef MHEG5_HD_SUPPORT
    b_is_bond_enable = FALSE;
    i4_ret = CHIPSPECR_ERROR;
#else
    i4_ret = x_chip_spec_get_MHEGHD_bond(&b_is_bond_enable);
#endif
    if (i4_ret == CHIPSPECR_OK && b_is_bond_enable)
    {
        g_t_surf.b_hd_support = TRUE;
        g_t_surf.ui4_width  = (INT32) 1280;
        g_t_surf.ui4_height = (INT32) 720;
#ifdef MW_MHEG5_ARGB8888_SUPPORT
        g_t_surf.e_internal_col_mode = GL_COLORMODE_ARGB_D8888;
#else
        g_t_surf.e_internal_col_mode = GL_COLORMODE_ARGB_D4444;
#endif
        
        t_surf_desc.ui4_width  = g_t_surf.ui4_width;
        t_surf_desc.ui4_height = g_t_surf.ui4_height;
    }
    else
    {
        g_t_surf.b_hd_support = FALSE;
        g_t_surf.ui4_width  = (INT32) 720;
        g_t_surf.ui4_height = (INT32) 576;
#ifdef MW_MHEG5_ARGB8888_SUPPORT
        g_t_surf.e_internal_col_mode = GL_COLORMODE_ARGB_D8888;
#else
        g_t_surf.e_internal_col_mode = GL_COLORMODE_ARGB_D4444;
#endif

        t_surf_desc.ui4_width  = g_t_surf.ui4_width;
        t_surf_desc.ui4_height = g_t_surf.ui4_height;
    }

    t_surf_desc.e_clrmode  = g_t_surf.e_internal_col_mode;
    t_surf_desc.h_clipper  = NULL_HANDLE;
    t_surf_desc.h_palette  = NULL_HANDLE;
    
    i4_ret = x_gl_surface_create(&t_surf_desc, &g_t_surf.h_surf);
    if (i4_ret != GLR_OK)
    {
        x_dbg_stmt("[MHEG5-GRD] %s #%d, create surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret);
        return i4_ret;
    }
    else
    {
        GL_COLOR_T t_color = GL_COLOR_DEF(0, 0, 0, 0);
        
        x_gl_surface_fill(g_t_surf.h_surf, &t_color, 0);

        i4_ret = x_gl_surface_create(&t_surf_desc, &g_t_surf.h_surf_buf);
        if (i4_ret != GLR_OK)
        {
            x_gl_obj_free(g_t_surf.h_surf);
            g_t_surf.h_surf = NULL_HANDLE;
            g_t_surf.h_surf_buf = NULL_HANDLE;
            return i4_ret;
        }
    }
    {
        GL_COLOR_T t_color = GL_COLOR_DEF(0, 0, 0, 0);
        
        i4_ret = x_gl_surface_create(&t_surf_desc, &g_t_surf.h_surf_3rd);
        if (i4_ret != GLR_OK)
        {
            x_gl_obj_free(g_t_surf.h_surf);
            g_t_surf.h_surf = NULL_HANDLE;
            g_t_surf.h_surf_buf = NULL_HANDLE;
            return i4_ret;
        }
        
        x_gl_surface_fill(g_t_surf.h_surf_3rd, &t_color, 0);
    }
     
    if (NULL_HANDLE != _h_mheg5_canvas)
    {
        i4_ret = x_wgl_get_canvas_info(_h_mheg5_canvas, &t_cvs_info);
        if (i4_ret == WGLR_OK)
        {
            g_t_surf.e_ture_col_mode = t_cvs_info.e_clr_mode;
            
            if (GL_COLORMODE_ARGB_CLUT8 == t_cvs_info.e_clr_mode)
            {
                t_surf_desc.e_clrmode  = GL_COLORMODE_ARGB_CLUT8;
                t_surf_desc.h_clipper  = NULL_HANDLE;
                t_surf_desc.h_palette  = g_t_surf.h_palette;

                i4_ret = x_gl_surface_create(&t_surf_desc, &g_t_surf.h_surf_bak);
                if (i4_ret != GLR_OK)
                {
                    x_gl_obj_free(g_t_surf.h_surf);
                    g_t_surf.h_surf = NULL_HANDLE;
                    x_gl_obj_free(g_t_surf.h_surf_buf);
                    g_t_surf.h_surf_buf = NULL_HANDLE;
                    x_dbg_stmt("[MHEG5-GRD] %s #%d, create surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret);
                    return i4_ret;
                }
            }
        }
        else
        {
            x_gl_obj_free(g_t_surf.h_surf);
            g_t_surf.h_surf = NULL_HANDLE;
            x_gl_obj_free(g_t_surf.h_surf_buf);
            g_t_surf.h_surf_buf = NULL_HANDLE;

            return i4_ret;
        }
    }
        

    if (NULL_HANDLE != g_t_surf.h_palette &&
        NULL_HANDLE != g_t_surf.h_surf_bak &&
        GL_COLORMODE_ARGB_CLUT8 == g_t_surf.e_ture_col_mode)
    {
        /* "_t_color_key is" background color actually, not a color key */
        
        GL_COLOR_T      t_color_key = {0, {0}, {0}, {0}};
        UINT16          ui2_index;

        if (NULL == g_t_surf.pui1_pal_cnv_tbl && NULL_HANDLE != g_t_surf.h_palette)
        {
            g_t_surf.pui1_pal_cnv_tbl = _gen_pal_cnv_tbl(g_t_surf.h_palette);
        }

        x_gl_palette_match_color_ex(
            g_t_surf.h_palette, 
            64,
            188,
            TRUE, 
            &t_color_key, 
            NULL, 
            &ui2_index);

        t_color_key.u3.index = (UINT8)ui2_index;
        x_gl_surface_fill(g_t_surf.h_surf_bak, &t_color_key, 0);

        x_gl_palette_match_color_ex(
            g_t_surf.h_palette, 
            64,
            188,
            TRUE, 
            &_t_color_key, 
            NULL, 
            &ui2_index);

        _t_color_key.u3.index = (UINT8) ui2_index;

    }

    _t_rs_ui.i4_left = 0;
    _t_rs_ui.i4_top = 0;
    _t_rs_ui.i4_right = g_t_surf.ui4_width;
    _t_rs_ui.i4_bottom = g_t_surf.ui4_height;

    return MHEG5R_OK;
    
}

INT32 grd_set_default_overscan(SCC_VID_OVER_SCAN_CLIPPER_T *pt_clipper)
{
    DBG_INFO(("{MHEG5-GRD} %s ....\n", __FUNCTION__));

    if (pt_clipper)
    {
        _t_dflt_ovs_clip = *pt_clipper;
    }

    return MHEG5R_OK;
}

INT32 grd_set_visibility(WGL_SW_CMD_T e_sw_cmd)
{
    INT32       i4_ret;
    
    DBG_INFO(("{MHEG5-GRD} %s ....\n", __FUNCTION__));
    
    i4_ret = x_wgl_set_visibility(_h_main_frm, e_sw_cmd);

    return (i4_ret == WGLR_OK)? MHEG5R_OK: MHEG5R_INTERNAL_ERROR;
}

INT32 grd_show(
    MHEG5_COEXIST_DISPLAY_T e_display,
    BOOL                    b_get_focus)
{
    HANDLE_T            h_frm;
    GL_HPALETTE_T       h_pal;
    INT32               i4_ret;
    
    i4_ret = MHEG5R_INTERNAL_ERROR;
    
    h_frm = _h_main_frm;
    
    DBG_INFO(("{MHEG5-GRD} %s, e_display = %d\n", __FUNCTION__, (UINT8)e_display));
        
    if (NULL_HANDLE != h_frm)
    {
        i4_ret = MHEG5R_INV_ARG;
        h_pal = g_t_surf.h_palette;
        
        if (MHEG5_HIDE_GUI               == e_display ||
            MHEG5_OVERLAY_GUI            == e_display ||
            MHEG5_OVERLAY_GUI_LIGHTER    == e_display ||
            MHEG5_OVERLAY_GUI_BOTTOMMOST == e_display)
        {
            i4_ret = MHEG5R_OK;
            
            if (b_get_focus)
            {
                x_wgl_set_focus(h_frm, FALSE);
            }

            if (_b_order_adjusted)
            {
                x_wgl_insert(h_frm, _h_ref_widget, _e_insert_mode, WGL_NO_AUTO_REPAINT);
            }
            
            switch (e_display)
            {
                case MHEG5_HIDE_GUI:
                    x_wgl_show(h_frm, WGL_SW_HIDE);
                    g_t_surf.b_view_rendered = FALSE;
                    sct_reset_state();
                    
                    break;

                case MHEG5_OVERLAY_GUI_LIGHTER:
                    if (NULL_HANDLE == h_pal) /* direct colormode */
                    {
                        x_wgl_set_alpha(h_frm, MHEG5_MAIN_FRAME_ALPHA_LIGHTER);
                        x_wgl_show(h_frm, WGL_SW_NORMAL);
                    }
                    else /* indirect colormode */
                    {
                        x_wgl_show(h_frm, WGL_SW_HIDE);
                    }
                    break;
                    
                case MHEG5_OVERLAY_GUI:
                    x_wgl_set_alpha(h_frm, MHEG5_MAIN_FRAME_ALPHA);
                    x_wgl_show(h_frm, WGL_SW_NORMAL);
                    break;

                case MHEG5_OVERLAY_GUI_BOTTOMMOST:
                    x_wgl_insert(h_frm, NULL_HANDLE, WGL_INSERT_BOTTOMMOST, WGL_SYNC_AUTO_REPAINT);
                    break;
            }
        }
    }

    return (i4_ret);
}

INT32 grd_update(
    GL_RECT_T*  pt_rect, 
    BOOL        b_sync)
{
    #define FRACTION    16
    
    INT32 i4_ret = MHEG5R_NOT_INIT;
    
    if (NULL_HANDLE != _h_main_frm)
    {
        CRIT_STATE_T    t_cs;
        GL_RECT_T       t_rc;
        UINT32          ui4_ratio_w, ui4_ratio_h;
    
        t_cs = x_crit_start();
        t_rc = _t_rc_ui;
        x_crit_end(t_cs);

        ui4_ratio_w = (UINT32)((RECT_W(&t_rc) << FRACTION) / PAL_W);
        ui4_ratio_h = (UINT32)((RECT_H(&t_rc) << FRACTION) / PAL_H);
        
        t_rc.i4_right   = (((pt_rect->i4_right  * ui4_ratio_w) + (1 << (FRACTION - 1))) >> FRACTION) + t_rc.i4_left;
        t_rc.i4_bottom  = (((pt_rect->i4_bottom * ui4_ratio_h) + (1 << (FRACTION - 1))) >> FRACTION) + t_rc.i4_top;
        t_rc.i4_left   += ((pt_rect->i4_left * ui4_ratio_w) >> FRACTION);
        t_rc.i4_top    += ((pt_rect->i4_top  * ui4_ratio_h) >> FRACTION);

        if (WGLR_OK == x_wgl_repaint(_h_main_frm, &t_rc, b_sync))
        {
            i4_ret = MHEG5R_OK;
        }
        else
        {
            i4_ret = MHEG5R_INTERNAL_ERROR;
        }
    }
    
    return (i4_ret);
}


/*-----------------------------------------------------------------------------
* Name: osd_create_tmp_sfc
*
* Description: This API create a temp surface.
*
* Inputs:   i4_width    the surface width.
*           i4_height   the surface height.
*           e_clrmode   the surface color mode.
*           h_palette   the surface palette.
*
* Outputs:  ph_sfc.     Pointer to the created surface.
*
* Returns:  None
*
----------------------------------------------------------------------------*/
INT32 
create_tmp_sfc( UINT32 ui4_width, 
                UINT32 ui4_height,
                GL_HSURFACE_T* ph_sfc )
{
    INT32               i4_res;
    GL_SURFACE_DESC_T   t_desc;

    t_desc.e_clrmode     = g_t_surf.e_internal_col_mode;
    t_desc.h_clipper     = NULL_HANDLE;
    t_desc.h_palette     = g_t_surf.h_palette;
    t_desc.ui4_width     = ui4_width;
    t_desc.ui4_height    = ui4_height;

    i4_res = x_gl_surface_create( &t_desc, ph_sfc );

    return i4_res;
}


/*-----------------------------------------------------------------------------
* Name: match_color
*
* Description: This API match mheg5 color to the specificed foramt.
*
* Inputs:   t_in_color      the mheg5 color.
*           e_clrmode       the color mode.
*           h_palette       the palette.
*
* Outputs:  pt_out_color.   Pointer to the converted color.
*
* Returns:  None
*
----------------------------------------------------------------------------*/
void 
match_color( OSDColor       t_in_color, 
            GL_COLORMODE_T  e_color_mode, 
            GL_HPALETTE_T   h_palette, 
            GL_COLOR_T*     pt_out_color)
{
    GL_COLOR_T  t_src_color;
    GL_COLOR_T  t_tgt_color;
    UINT16      ui2_idx;
    MHEG5_assert( NULL != pt_out_color );

    t_src_color.a     = (UINT8) (t_in_color >> 24);
    t_src_color.u1.r  = (UINT8) ((t_in_color >> 16) & 0xFF);
    t_src_color.u2.g  = (UINT8) ((t_in_color >> 8) & 0xFF);
    t_src_color.u3.b  = (UINT8) (t_in_color & 0xFF);

    switch (e_color_mode)
    {
    case GL_COLORMODE_ARGB_CLUT2:
    case GL_COLORMODE_ARGB_CLUT4:
    case GL_COLORMODE_ARGB_CLUT8:
        x_gl_palette_match_color(
            h_palette, 
            TRUE,
            &t_src_color,
            NULL,
            &ui2_idx );

        pt_out_color->u3.index = (UINT8) ui2_idx;
        break;
    case GL_COLORMODE_ARGB_D4444:
        *pt_out_color = t_src_color;
        pt_out_color->a = GET_MHEG5_ADJUSTED_ALPHA(t_src_color.a);
        break;
        
    case GL_COLORMODE_RGB_D565:
    case GL_COLORMODE_ARGB_D1555:
    case GL_COLORMODE_ARGB_D8888:
        *pt_out_color = t_src_color;
        break;    
    case GL_COLORMODE_UYVY_16:
    case GL_COLORMODE_YUYV_16:
    case GL_COLORMODE_AYUV_D8888:        
        _convert_rgb_2_yuv( &t_tgt_color, &t_src_color );
        *pt_out_color = t_tgt_color;
        break;
    case GL_COLORMODE_AYUV_CLUT2:
    case GL_COLORMODE_AYUV_CLUT4:
    case GL_COLORMODE_AYUV_CLUT8:        
        _convert_rgb_2_yuv( &t_tgt_color, &t_src_color );  
        x_gl_palette_match_color(
            h_palette, 
            TRUE, 
            &t_tgt_color,
            NULL, 
            &ui2_idx );
        pt_out_color->u3.index = (UINT8) ui2_idx;
        break;
    default:
        MHEG5_assert( 0 );
    }
}

INT32 grd_cnv_argb_32b_2_argb_16b(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    const GL_RECT_T*    pt_area)
{
    GL_LOCK_INFO_T  t_lock_dst;
    GL_LOCK_INFO_T  t_lock_src;
    INT32           i4_ret;
    
    i4_ret = x_gl_surface_lock(h_src_surf, NULL, 0, &t_lock_src);

    if (GLR_OK == i4_ret)
    {
        i4_ret = x_gl_surface_lock(h_dst_surf, NULL, 0, &t_lock_dst);
        
        if (GLR_OK == i4_ret)
        {
            UINT8*  pui1_src;
            UINT16* pui2_dst;
            UINT32* pui4_src;
            UINT32  w, h;
            UINT32  x, y;

            w = (UINT32)RECT_W(pt_area);
            h = (UINT32)RECT_H(pt_area);
            
            pui2_dst = (UINT16*)((UINT8*) t_lock_dst.at_subplane[0].pv_bits + 
                t_lock_dst.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            pui4_src = (UINT32*)((UINT8*) t_lock_src.at_subplane[0].pv_bits + 
                t_lock_src.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    pui1_src = (UINT8*)(pui4_src + x);
                    pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(pui1_src[3], pui1_src[2], pui1_src[1], pui1_src[0]);
                }

                pui2_dst = (UINT16*) ((UINT8*) pui2_dst + t_lock_dst.at_subplane[0].ui4_pitch);
                pui4_src = (UINT32*) ((UINT8*) pui4_src + t_lock_src.at_subplane[0].ui4_pitch);
            }
            
            x_gl_surface_unlock(h_dst_surf, NULL);
        }
        else
        {
            x_gl_surface_unlock(h_src_surf, NULL);
        }

        x_gl_surface_unlock(h_src_surf, NULL);
    }
    
    return ((GLR_OK == i4_ret) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
}


INT32 grd_cnv_ayuv_32b_2_argb_32b(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    const GL_RECT_T*    pt_area)
{
    GL_LOCK_INFO_T  t_lock_dst;
    GL_LOCK_INFO_T  t_lock_src;
    INT32           i4_ret;
    
    i4_ret = x_gl_surface_lock(h_src_surf, NULL, 0, &t_lock_src);

    if (GLR_OK == i4_ret)
    {
        i4_ret = x_gl_surface_lock(h_dst_surf, NULL, 0, &t_lock_dst);
        
        if (GLR_OK == i4_ret)
        {
            UINT8*  pui1_src;
            UINT32* pui4_dst;
            UINT32* pui4_src;
            GL_COLOR_T t_tgt_color, t_src_color;
            UINT32  w, h;
            UINT32  x, y;

            w = (UINT32)RECT_W(pt_area);
            h = (UINT32)RECT_H(pt_area);
            
            pui4_dst = (UINT32*)((UINT8*) t_lock_dst.at_subplane[0].pv_bits + 
                t_lock_dst.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            pui4_src = (UINT32*)((UINT8*) t_lock_src.at_subplane[0].pv_bits + 
                t_lock_src.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    pui1_src = (UINT8*)(pui4_src + x);
                    t_src_color.a = pui1_src[3];
                    t_src_color.u1.y = pui1_src[2];
                    t_src_color.u2.u = pui1_src[1];
                    t_src_color.u3.v = pui1_src[0];
                    _convert_yuv_2_rgb(&t_tgt_color, &t_src_color);
                    pui4_dst[x] = (UINT16) GL_COLOR_ARGB8888(t_tgt_color.a, t_tgt_color.u1.r, t_tgt_color.u2.g, t_tgt_color.u3.b);
                }

                pui4_dst = (UINT32*) ((UINT8*) pui4_dst + t_lock_dst.at_subplane[0].ui4_pitch);
                pui4_src = (UINT32*) ((UINT8*) pui4_src + t_lock_src.at_subplane[0].ui4_pitch);
            }
            
            x_gl_surface_unlock(h_dst_surf, NULL);
        }
        else
        {
            x_gl_surface_unlock(h_src_surf, NULL);
        }

        x_gl_surface_unlock(h_src_surf, NULL);
    }
    
    return ((GLR_OK == i4_ret) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
}

INT32 grd_cnv_ayuv_32b_2_argb_16b(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    const GL_RECT_T*    pt_area)
{
    GL_LOCK_INFO_T  t_lock_dst;
    GL_LOCK_INFO_T  t_lock_src;
    INT32           i4_ret;
    
    i4_ret = x_gl_surface_lock(h_src_surf, NULL, 0, &t_lock_src);

    if (GLR_OK == i4_ret)
    {
        i4_ret = x_gl_surface_lock(h_dst_surf, NULL, 0, &t_lock_dst);
        
        if (GLR_OK == i4_ret)
        {
            UINT8*  pui1_src;
            UINT16* pui2_dst;
            UINT32* pui4_src;
            GL_COLOR_T t_tgt_color, t_src_color;
            UINT32  w, h;
            UINT32  x, y;

            w = (UINT32)RECT_W(pt_area);
            h = (UINT32)RECT_H(pt_area);
            
            pui2_dst = (UINT16*)((UINT8*) t_lock_dst.at_subplane[0].pv_bits + 
                t_lock_dst.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            pui4_src = (UINT32*)((UINT8*) t_lock_src.at_subplane[0].pv_bits + 
                t_lock_src.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    pui1_src = (UINT8*)(pui4_src + x);
                    t_src_color.a = pui1_src[3];
                    t_src_color.u1.y = pui1_src[2];
                    t_src_color.u2.u = pui1_src[1];
                    t_src_color.u3.v = pui1_src[0];
                    _convert_yuv_2_rgb(&t_tgt_color, &t_src_color);
                    pui2_dst[x] = (UINT16) GL_COLOR_ARGB4444(t_tgt_color.a, t_tgt_color.u1.r, t_tgt_color.u2.g, t_tgt_color.u3.b);
                }

                pui2_dst = (UINT16*) ((UINT8*) pui2_dst + t_lock_dst.at_subplane[0].ui4_pitch);
                pui4_src = (UINT32*) ((UINT8*) pui4_src + t_lock_src.at_subplane[0].ui4_pitch);
            }
            
            x_gl_surface_unlock(h_dst_surf, NULL);
        }
        else
        {
            x_gl_surface_unlock(h_src_surf, NULL);
        }

        x_gl_surface_unlock(h_src_surf, NULL);
    }
    
    return ((GLR_OK == i4_ret) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
}

INT32 grd_cnv_argb_16b_2_pal_8b(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    const GL_RECT_T*    pt_area,
    const UINT8*        pui1_cnv_tbl)
{
    GL_LOCK_INFO_T  t_lock_dst;
    GL_LOCK_INFO_T  t_lock_src;
    INT32           i4_ret;
    
    i4_ret = x_gl_surface_lock(h_src_surf, NULL, 0, &t_lock_src);

    if (GLR_OK == i4_ret)
    {
        i4_ret = x_gl_surface_lock(h_dst_surf, NULL, 0, &t_lock_dst);
        
        if (GLR_OK == i4_ret)
        {
            UINT8*  pui1_dst;
            UINT16* pui2_src;
            UINT32  w, h;
            UINT32  x, y;

            w = (UINT32)RECT_W(pt_area);
            h = (UINT32)RECT_H(pt_area);
            
            pui1_dst = (UINT8*) t_lock_dst.at_subplane[0].pv_bits + 
                t_lock_dst.at_subplane[0].ui4_pitch * pt_area->i4_top + pt_area->i4_left;
            pui2_src = (UINT16*) ((UINT8*) t_lock_src.at_subplane[0].pv_bits + 
                t_lock_src.at_subplane[0].ui4_pitch * pt_area->i4_top) + pt_area->i4_left;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    pui1_dst[x] = pui1_cnv_tbl[pui2_src[x]];
                }

                pui1_dst += t_lock_dst.at_subplane[0].ui4_pitch;
                pui2_src = (UINT16*) ((UINT8*) pui2_src + t_lock_src.at_subplane[0].ui4_pitch);
            }
            
            x_gl_surface_unlock(h_dst_surf, NULL);
        }

        x_gl_surface_unlock(h_src_surf, NULL);
    }
    
    return ((GLR_OK == i4_ret) ? MHEG5R_OK : MHEG5R_INTERNAL_ERROR);
}



#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: mheg5_gui_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_gui_get_dbg_level(VOID)
{
    return _ui2_mheg5_gui_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: mheg5_gui_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_gui_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_gui_dbg_level = ui2_level;
    return TRUE;

}
#endif



