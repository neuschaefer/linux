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
 * $RCSfile: wgl_tktp_cnt_def.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 2c46162b759413f7bb637f26bd65e2f5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "graphic/x_gl.h"
#include "os/inc/x_os.h"
#include "unicode/x_uc_str.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_common.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/tktp/wgl_tktp_cnt_internal.h"
#include "wgl/tktp/wgl_tktp_cnt_def.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/tktp/u_wgl_tktp.h"

 /*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "TKTPCNT"

#define REPLACE_HORIZ_ALIGN(_ui1_align_orig, _ui1_align_horiz)  \
    (((_ui1_align_orig) & 0x0F) | ((_ui1_align_horiz) & 0xF0))

#define DEF_FONT_W              20
#define DEF_FONT_H              30

#define BKSURF_PAD_FACTOR       2

/*-----------------------------------------------------------------------------
                    constants
 ----------------------------------------------------------------------------*/
#define DFT_ALIGN_MSG           WGL_AS_LEFT_CENTER

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _tktpcntdef_cmd_hdr_reset_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_do_next_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_get_if_need_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_get_has_text_moved(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_update_screen_location(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_update_text(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_update_cnt_rect(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_update_cnt_inset(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_update_font(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_update_color(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_set_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_get_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_get_color(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);

static INT32 _tktpcntdef_cmd_hdr_get_expected_cnt_height(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_get_cnt_inset(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                         *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_set_align(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktpcntdef_cmd_hdr_get_align(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktpcntdef_tool_update_scrl_context(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_need_scrl);

static INT32 _tktpcntdef_tool_check_need_scrl(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_need_scrl,
    OUT     INT32                       *pi4_scrl_remain);

static INT32 _tktpcntdef_tool_render_text_in_bound(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UTF16_T                     *w2s_text,
    IN      UINT16                      ui2_len_text,
    IN      UINT16                      ui2_ci_start,
    IN      INT32                       i4_kern_left,
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_right_bound,
    IN      INT32                       i4_bg_h,
    IN      BOOL                        b_wrap_over,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T                  *pt_clr_fg,
    IN      GL_COLOR_T                  *pt_clr_bk,
    OUT     UINT16                      *pui2_ci_end, 
    OUT     INT32                       *pi4_right,
    OUT     INT32                       *pi4_kern_after_ci_end);

static INT32 _tktpcntdef_tool_render_msg(
    IN      GL_HGRAPHICS_T              h_g_dst,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      GL_RECT_T                   *pt_rc_bk,
    IN      GL_RECT_T                   *pt_rc_msg,
    IN      WGL_STRING_T                *pt_str_msg,
    IN      UINT8                       ui1_align_msg,
    IN      GL_COLOR_T                  *pt_clr_msg_fg,
    IN      GL_COLOR_T                  *pt_clr_msg_bk,
    OUT     UINT16                      *pui2_ci_end, 
    OUT     INT32                       *pi4_right,
    OUT     INT32                       *pi4_kern_after_ci_end);

static INT32 _tktpcntdef_tool_check_bksurf(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      BOOL                        b_need_scrl);

static VOID _tktpcntdef_tool_check_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx);

static INT32 _tktpcntdef_tool_calc_cpblk_size(
    IN      HFONT_T                     h_font_msg,
    OUT     INT32                       *pi4_cpblk_size);

static VOID _tktpcntdef_tool_calc_obj_pos(
    IN      GL_RECT_T                   *pt_rc_ref,
    IN      UINT8                       ui1_align_obj,
    IN      INT32                       i4_w_obj,
    IN      INT32                       i4_h_obj,
    OUT     INT32                       *pi4_left,
    OUT     INT32                       *pi4_top);
    
static VOID _tktpcntdef_tool_calc_obj_vertical_pos(
    IN      INT32                       i4_top_ref,
    IN      INT32                       i4_btm_ref,
    IN      UINT8                       ui1_align_obj,
    IN      INT32                       i4_h_obj,
    OUT     INT32                       *pi4_top);

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_tktp_cnt_def_inst_create(
    WGL_CONTENT_CONTEXT_HANDLE_T        *ppv_ctx_cnt, 
    WGL_CONTENT_CREATE_INPUT_T          *pt_cnt_create )
{
    TKTP_CNT_INIT_T     *pt_init = (TKTP_CNT_INIT_T *)pt_cnt_create->pv_init;
    TKTPCNTDEF_CTX_T    *pt_ctx = NULL;
    INT32               i4_ret;

    *ppv_ctx_cnt = NULL;

    pt_ctx = (TKTPCNTDEF_CTX_T *)WGL_MEM_ALLOC(sizeof(TKTPCNTDEF_CTX_T));
    if(pt_ctx==NULL)
    {
        INFORM_ERR(WGLR_OUT_OF_MEMORY);
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL
     pt_ctx->ui4_mem_size = sizeof(TKTPCNTDEF_CTX_T); 
#endif
    pt_ctx->ui4_style = pt_init->ui4_style;
    pt_ctx->ui2_attr  = 0;
    if(pt_init->b_force_scrl_short_text)
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, TTCD_ATTR_FORCE_SCRL_SHORT_TEXT);
    }
    pt_ctx->ui2_state = 0;
    WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY);
    WGL_SET_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED);
    WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL);

    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_cnt_create->pt_cvsst);
    pt_ctx->pt_str_msg = pt_init->pt_str_msg;
    pt_ctx->i4_scrn_wgt_left = 0;
    pt_ctx->i4_scrn_wgt_top = 0;
    _INSET_EMPTY(&pt_ctx->t_inset_cnt);
    WGL_RECT_COPY(&pt_ctx->t_rc_cnt, &pt_cnt_create->t_rect_cnt);
    WGL_RECT_COPY(&pt_ctx->t_rc_vp, &pt_ctx->t_rc_cnt);
    _RECT_INSET(&pt_ctx->t_rc_vp, &pt_ctx->t_inset_cnt);
    WGL_RECT_EMPTY(&pt_ctx->t_rc_bksurf);    
    wgl_get_system_color(WGL_SYS_CLR_TKTP_BK, pt_cnt_create->pt_cvsst, &pt_ctx->t_clr_bk);
    pt_ctx->ui1_align_msg = DFT_ALIGN_MSG;
    wgl_get_system_color(WGL_SYS_CLR_TKTP_MSG_FG, pt_cnt_create->pt_cvsst, &pt_ctx->t_clr_msg_fg);
    wgl_get_system_color(WGL_SYS_CLR_TKTP_MSG_BK, pt_cnt_create->pt_cvsst, &pt_ctx->t_clr_msg_bk);
    pt_ctx->i4_scrl_step = (INT32)pt_init->ui2_scrl_step_size;
    pt_ctx->h_surf_bk = NULL_HANDLE;
    pt_ctx->h_g_bk = NULL_HANDLE;
    
    /* init scrolling contexts */
    pt_ctx->b_scrl_once = FALSE;
    
    /* calculate the cpblk size */
    i4_ret = _tktpcntdef_tool_calc_cpblk_size(
        WGL_STR_GET_FONT(pt_ctx->pt_str_msg),
        &pt_ctx->i4_cpblk_size);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }
    
    pt_ctx->i4_scrl_remain = 0;
    pt_ctx->i4_orig_scrl_remain = 0;
    pt_ctx->i4_offset_bksurf = 0;
    pt_ctx->ui2_ci_last = 0;
    pt_ctx->i4_buf_right_ci_last = 0;
    pt_ctx->i4_kern_after_ci_last = 0;
    
    _tktpcntdef_tool_check_scrl_step_size(pt_ctx);
    *ppv_ctx_cnt = (WGL_CONTENT_CONTEXT_HANDLE_T)pt_ctx;

    return WGLR_OK;
}

INT32 wgl_tktp_cnt_def_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt )
{
    TKTPCNTDEF_CTX_T    *pt_ctx = (TKTPCNTDEF_CTX_T *)pv_ctx_cnt;
    INT32               i4_ret;

    if(pt_ctx->h_g_bk != NULL_HANDLE)
    {
        INFORM_ERR(x_gl_obj_free(pt_ctx->h_g_bk));
    }

    if(pt_ctx->h_surf_bk != NULL_HANDLE)
    {
        INFORM_ERR(x_gl_obj_free(pt_ctx->h_surf_bk));
    }

    WGL_MEM_FREE(pt_ctx);
    pt_ctx = NULL;


    return WGLR_OK;
}

INT32 wgl_tktp_cnt_def_cmd_proc(  
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    UINT32                              ui4_cnt_cmd_id, 
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{
    INT32               i4_ret_val;
    TKTPCNTDEF_CTX_T    *pt_ctx = (TKTPCNTDEF_CTX_T *)pv_ctx_cnt;

    i4_ret_val = WGLR_OK;

    switch(ui4_cnt_cmd_id) 
    {
        /* Internal Content Commands */
        case WGL_CNT_TKTP_UPDATE_SCRN_LOC:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_screen_location(pt_ctx, pv_param1, pv_param2);
        }
        break;
        
        case WGL_CNT_TKTP_UPDATE_TEXT:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_text(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_UPDATE_CNT_RECT:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_cnt_rect(pt_ctx, pv_param1, pv_param2);
        }
        break;
        
        case WGL_CNT_TKTP_UPDATE_CNT_INSET:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_cnt_inset(pt_ctx, pv_param1, pv_param2);
        }
        break;
        
        case WGL_CNT_TKTP_UPDATE_FONT:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_font(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_UPDATE_COLOR:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_update_color(pt_ctx, pv_param1, pv_param2);
        }
        break;

        case WGL_CNT_TKTP_RESET_SCRL:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_reset_scrl(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_DO_NEXT_SCRL:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_do_next_scrl(pt_ctx, pv_param1, pv_param2);
        }
        break;

        case WGL_CNT_TKTP_GET_IF_NEED_SCRL:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_if_need_scrl(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_GET_HAS_TEXT_MOVED:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_has_text_moved(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_SET_SCRL_STEP_SIZE:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_set_scrl_step_size(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_GET_SCRL_STEP_SIZE:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_scrl_step_size(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CNT_TKTP_GET_COLOR:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_color(pt_ctx, pv_param1, pv_param2);
        }
        break;
        
        case WGL_CNT_TKTP_GET_EXPECTED_CNT_HEIGHT:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_expected_cnt_height(pt_ctx, pv_param1);
        }
        break;
        
        /* common commands */
        case WGL_CMD_TKTP_GET_CNT_INSET:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_cnt_inset(pt_ctx, pv_param1);
       }
        break;
        
        case WGL_CMD_TKTP_SET_ALIGN:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_set_align(pt_ctx, pv_param1);
        }
        break;
        
        case WGL_CMD_TKTP_GET_ALIGN:
        {
            i4_ret_val = _tktpcntdef_cmd_hdr_get_align(pt_ctx, pv_param1);
        }
        break;

        default:
            i4_ret_val = WGLR_INV_CMD;
    }

    return i4_ret_val;
}


INT32 wgl_tktp_cnt_def_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state_paint)
{
    TKTPCNTDEF_CTX_T     *pt_ctx = (TKTPCNTDEF_CTX_T *)pv_ctx_cnt;
    GL_RECT_T            t_rc_scrn_vp;
    INT32                i4_ret;

    /* quick return if the content component is not with an effective rectangle*/
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_cnt))
    {
        return WGLR_OK;
    }
    
    /* fill bk */
    if(!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_GL_NO_BK))
    {
        if (!wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, &pt_ctx->t_clr_bk))
        { 
            INFORM_ERR(x_gl_set_color(h_g, &pt_ctx->t_clr_bk));
            INFORM_ERR(x_gl_fill_rect(h_g, &pt_ctx->t_rc_cnt));
        }
    }
    
    /* quick return if the viewport is not with an effective rectangle*/
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_vp))
    {
        return WGLR_OK;
    }

    /* set the clipping area to fit into the viewport */
    WGL_RECT_COPY(&t_rc_scrn_vp, &pt_ctx->t_rc_vp);
    MOVE_RECT(&t_rc_scrn_vp, pt_ctx->i4_scrn_wgt_left, pt_ctx->i4_scrn_wgt_top);
    INFORM_ERR(x_gl_set_clip(h_g, &t_rc_scrn_vp));
    
    /* draw the viewport */
    if(HAS_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL))
    {
        /* clear viewport */
        if (!wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, &pt_ctx->t_clr_msg_bk))
        {
            INFORM_ERR(x_gl_set_color(h_g, &pt_ctx->t_clr_msg_bk));
            INFORM_ERR(x_gl_fill_rect(h_g, &pt_ctx->t_rc_vp));
        }
        
        /* bitblt the back surface */
        if(pt_ctx->i4_offset_bksurf < RECT_W(&pt_ctx->t_rc_bksurf))
        {
            INT32       i4_g_top_msg, i4_h_msg;
            GL_COLOR_T  *pt_clr_key_min, *pt_clr_key_max;
            GL_COLOR_T  t_clr_key_dcm_min = {0, {0}, {0}, {0}};
            GL_COLOR_T  t_clr_key_dcm_max = {0, {255}, {255}, {255}};
            
            i4_ret = wgl_str_get_height(pt_ctx->pt_str_msg, &i4_h_msg);
            if(WGL_IS_FAIL(i4_ret))
            {
                INFORM_ERR(i4_ret);
                i4_h_msg = DEF_FONT_H;
            }
            _tktpcntdef_tool_calc_obj_vertical_pos(
                pt_ctx->t_rc_vp.i4_top,
                pt_ctx->t_rc_vp.i4_bottom,
                pt_ctx->ui1_align_msg,
                i4_h_msg,
                &i4_g_top_msg);
            
            if (wgl_cvsst_check_palette_mode (&pt_ctx->t_cvsst))
            {
                pt_clr_key_min = &pt_ctx->t_cvsst.t_colorkey;
                pt_clr_key_max = &pt_ctx->t_cvsst.t_colorkey;
            }
            else
            {
                pt_clr_key_min = &t_clr_key_dcm_min;
                pt_clr_key_max = &t_clr_key_dcm_max;
            }
            
            INFORM_ERR (x_gl_transp_blt (
                h_g, 
                pt_ctx->h_surf_bk, 
                pt_ctx->t_rc_vp.i4_left,
                i4_g_top_msg,
                pt_ctx->i4_offset_bksurf,
                0,
                (UINT32)(MIN(RECT_W(&pt_ctx->t_rc_vp),(RECT_W(&pt_ctx->t_rc_bksurf) - pt_ctx->i4_offset_bksurf))),
                (UINT32)i4_h_msg,
                255,
                TRUE,
                pt_clr_key_min,
                pt_clr_key_max ));
         }
    }
    else
    {
        /* render the viewport directly without back surface */
        UINT16  ui2_tmp;
        INT32   i4_tmp_1, i4_tmp_2;
        INFORM_ERR(_tktpcntdef_tool_render_msg(
            h_g,
            &pt_ctx->t_cvsst,
            &pt_ctx->t_rc_vp,
            &pt_ctx->t_rc_vp,
            pt_ctx->pt_str_msg,
            pt_ctx->ui1_align_msg,
            &pt_ctx->t_clr_msg_fg,
            &pt_ctx->t_clr_msg_bk,
            &ui2_tmp,
            &i4_tmp_1,
            &i4_tmp_2));
    }
    
    /* cancel the clipping */
    INFORM_ERR(x_gl_set_clip(h_g, NULL));
    
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
                    command handling functions
 ----------------------------------------------------------------------------*/
static INT32 _tktpcntdef_cmd_hdr_reset_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1)
{
    TKTP_CNT_RSTSCRL_PARAM_T *pt_param = (TKTP_CNT_RSTSCRL_PARAM_T *)pv_param1;
    
    CHECK_NULL(pt_param, WGLR_INV_ARG);
    CHECK_VALID(   HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY)
                && HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED), WGLR_INTERNAL_ERROR);
    
    pt_ctx->b_scrl_once = pt_param->b_scrl_once;

    WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED);

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_do_next_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    TKTP_CNT_DNS_PARAM_T    *pt_dns_param = (TKTP_CNT_DNS_PARAM_T *)pv_param1;
    TKTP_CNT_DNS_RET_T      *pt_dns_ret = (TKTP_CNT_DNS_RET_T *)pv_param2;
    GL_RECT_T               t_rc_temp;
    INT32                   i4_ret;
    
    CHECK_VALID(pt_dns_param!=NULL && pt_dns_ret!=NULL, WGLR_INV_ARG);
    CHECK_VALID(   HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY)
                && !HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED), WGLR_INTERNAL_ERROR);

    pt_dns_ret->b_end_scrl = pt_ctx->b_scrl_once ? TRUE : FALSE;
    pt_dns_ret->b_end_reached = TRUE;

    /* quick return if no need scroll */
    if(!HAS_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        pt_ctx->i4_offset_bksurf = 0;
        return WGLR_OK;
    }

    /* quick return and inform error if the viewport is not with an effective rectangle*/
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_vp))
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        pt_ctx->i4_offset_bksurf = 0;
        return WGLR_OK;
    }

    /* quick return if scroll-once and has-been-finished-scroll */
    if(pt_ctx->b_scrl_once && pt_ctx->i4_scrl_remain<=0)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_OK;
    }

    /* shift pt_ctx->i4_offset_bksurf */
    /* update pt_ctx->i4_scrl_remain */
    /* prepare pt_dns_ret->b_end_reached, pt_dns_ret->b_end_scrl */
    if(pt_ctx->b_scrl_once)
    {
        INT32   i4_eff_step_size  = MIN(pt_ctx->i4_scrl_step, pt_ctx->i4_scrl_remain);
        pt_ctx->i4_offset_bksurf += i4_eff_step_size;
        pt_ctx->i4_scrl_remain   -= i4_eff_step_size;
        if(pt_ctx->i4_scrl_remain <= 0)
        {
            INFORM_ERR(x_gl_surface_fill(pt_ctx->h_surf_bk, &pt_ctx->t_clr_msg_bk, GL_SYNC));
            pt_ctx->i4_scrl_remain    = 0;
            pt_ctx->i4_offset_bksurf  = 0;
            pt_dns_ret->b_end_reached = TRUE;
            pt_dns_ret->b_end_scrl    = TRUE;
            return WGLR_OK;
        }
        else
        {
            pt_dns_ret->b_end_reached = FALSE;
            pt_dns_ret->b_end_scrl    = FALSE;
        }
    }
    else
    {
        pt_ctx->i4_offset_bksurf += pt_ctx->i4_scrl_step;
        pt_ctx->i4_scrl_remain   -= pt_ctx->i4_scrl_step;
        if(pt_ctx->i4_scrl_remain <= 0)
        {
            SET_RECT(
                &t_rc_temp, 
                RECT_W(&pt_ctx->t_rc_vp), 
                0, 
                RECT_W(&pt_ctx->t_rc_bksurf), 
                RECT_H(&pt_ctx->t_rc_bksurf));
            INFORM_ERR(_tktpcntdef_tool_render_msg(
                pt_ctx->h_g_bk, 
                &pt_ctx->t_cvsst,
                &pt_ctx->t_rc_bksurf,
                &t_rc_temp,
                pt_ctx->pt_str_msg,
                WGL_AS_LEFT_TOP,
                &pt_ctx->t_clr_msg_fg,
                &pt_ctx->t_clr_msg_bk,
                &pt_ctx->ui2_ci_last,
                &pt_ctx->i4_buf_right_ci_last,
                &pt_ctx->i4_kern_after_ci_last));
            pt_ctx->i4_offset_bksurf  = 0;
            pt_ctx->i4_scrl_remain    = pt_ctx->i4_orig_scrl_remain;
            pt_dns_ret->b_end_reached = TRUE;
            pt_dns_ret->b_end_scrl    = FALSE;
        }
        else
        {
            pt_dns_ret->b_end_reached = FALSE;
            pt_dns_ret->b_end_scrl    = FALSE;
        }
    }

    /* move content of back surface if necessary */
    if( !( pt_ctx->b_scrl_once &&
           pt_ctx->ui2_ci_last+(UINT16)1 >= WGL_STR_GET_TEXT_LEN(pt_ctx->pt_str_msg)) )
    {
        INT32   i4_move_dis, i4_h_msg;

        /* calculate the distance need to be moved */
        i4_move_dis = 0;
        while(i4_move_dis + pt_ctx->i4_cpblk_size <= pt_ctx->i4_offset_bksurf)
        {
            i4_move_dis += pt_ctx->i4_cpblk_size;
        }
        pt_ctx->i4_offset_bksurf -= i4_move_dis;

        if(i4_move_dis > 0)
        {
            UINT16  ui2_ci_start;
            
            /* move the text and clear the residual background area */
            if(pt_ctx->i4_buf_right_ci_last >= i4_move_dis)
            {
                INFORM_ERR( x_gl_surface_blt(
                    pt_ctx->h_surf_bk, 
                    pt_ctx->h_surf_bk, 
                    0,
                    0,
                    i4_move_dis,
                    0,
                    (UINT32)(pt_ctx->i4_buf_right_ci_last - i4_move_dis),
                    (UINT32)RECT_H(&pt_ctx->t_rc_bksurf),
                    255,
                    GL_SYNC) );
        
                /* fill right empty area of the back surface with the message text background color */
                SET_RECT(
                    &t_rc_temp, 
                    pt_ctx->i4_buf_right_ci_last - i4_move_dis, 
                    0, 
                    pt_ctx->t_rc_bksurf.i4_right, 
                    RECT_H(&pt_ctx->t_rc_bksurf));
                if(RECT_IS_EFFECTIVE(t_rc_temp))
                {
                    INFORM_ERR(x_gl_set_color(pt_ctx->h_g_bk, &pt_ctx->t_clr_msg_bk));
                    INFORM_ERR(x_gl_fill_rect(pt_ctx->h_g_bk, &t_rc_temp));
                }        
                
                /* shift pt_ctx->i4_buf_right_ci_last */
                pt_ctx->i4_buf_right_ci_last -= i4_move_dis;
            }
            else if(pt_ctx->i4_buf_right_ci_last > 0)
            {
                pt_ctx->i4_buf_right_ci_last = 0;
                INFORM_ERR(x_gl_surface_fill(pt_ctx->h_surf_bk, &pt_ctx->t_clr_msg_bk, GL_SYNC));
            }
            
            /* render remain text and shift pt_ctx->ui2_ci_last */
            ui2_ci_start = pt_ctx->ui2_ci_last + (UINT16)1;
            if( ui2_ci_start < WGL_STR_GET_TEXT_LEN(pt_ctx->pt_str_msg) )
            {
                i4_ret = wgl_str_get_height(pt_ctx->pt_str_msg, &i4_h_msg);
                if(WGL_IS_FAIL(i4_ret))
                {
                    INFORM_ERR(i4_ret);
                    i4_h_msg = DEF_FONT_H;
                }
                INFORM_ERR(_tktpcntdef_tool_render_text_in_bound(
                    pt_ctx->h_g_bk,
                    &pt_ctx->t_cvsst,
                    WGL_STR_GET_TEXT(pt_ctx->pt_str_msg),
                    WGL_STR_GET_TEXT_LEN(pt_ctx->pt_str_msg),
                    ui2_ci_start,
                    pt_ctx->i4_kern_after_ci_last,
                    pt_ctx->i4_buf_right_ci_last,
                    0,
                    pt_ctx->t_rc_bksurf.i4_right,
                    i4_h_msg,
                    FALSE,
                    WGL_STR_GET_FONT(pt_ctx->pt_str_msg),
                    &pt_ctx->t_clr_msg_fg,
                    &pt_ctx->t_clr_msg_bk,
                    &pt_ctx->ui2_ci_last,
                    &pt_ctx->i4_buf_right_ci_last,
                    &pt_ctx->i4_kern_after_ci_last));            
            }
            
        }
        
                

    }


    return WGLR_OK;
}


static INT32 _tktpcntdef_cmd_hdr_get_if_need_scrl(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL        *pb_need_scrl = (BOOL *)pv_param1;

    CHECK_NULL(pb_need_scrl, WGLR_INV_ARG);

    *pb_need_scrl = (   HAS_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL) 
                     && !(   pt_ctx->b_scrl_once
                          && pt_ctx->i4_scrl_remain <= 0)
                     ) ? TRUE : FALSE;

    return WGLR_OK;
}


static INT32 _tktpcntdef_cmd_hdr_get_has_text_moved(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL    *pb_has_moved = (BOOL *)pv_param1;
    INT32   i4_ret;

    CHECK_NULL(pb_has_moved, WGLR_INV_ARG);

    *pb_has_moved = FALSE;
    
    if(    HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY)    
        && !HAS_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED)    
        && HAS_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL) )
    {
        INT32   i4_msg_w;
        CATCH_FAIL(wgl_str_get_width(pt_ctx->pt_str_msg, &i4_msg_w));
    
        if(pt_ctx->i4_scrl_remain < pt_ctx->i4_orig_scrl_remain)
        {
            *pb_has_moved = TRUE;
        }
    }

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_update_screen_location(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    pt_ctx->i4_scrn_wgt_left = (INT32)pv_param1;
    pt_ctx->i4_scrn_wgt_top  = (INT32)pv_param2;
    return WGLR_OK;
}        

static INT32 _tktpcntdef_cmd_hdr_update_text(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL        *pb_need_scrl = (BOOL *)pv_param1;
    INT32       i4_ret;

    CHECK_NULL(pb_need_scrl, WGLR_INV_ARG);
    *pb_need_scrl = FALSE;
    
    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx, 
        pb_need_scrl));

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_update_cnt_rect(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    GL_RECT_T       *pt_rc_cnt_new = (GL_RECT_T *)pv_param1;
    BOOL            *pb_need_scrl_out = (BOOL *)pv_param2;
    INT32           i4_ret;

    CHECK_VALID(pt_rc_cnt_new!=NULL && pb_need_scrl_out!=NULL, WGLR_INV_ARG);

    *pb_need_scrl_out = FALSE;

    /* update cnt & vp rect */
    WGL_RECT_COPY(&pt_ctx->t_rc_cnt, pt_rc_cnt_new);
    WGL_RECT_COPY(&pt_ctx->t_rc_vp, &pt_ctx->t_rc_cnt);
    _RECT_INSET(&pt_ctx->t_rc_vp, &pt_ctx->t_inset_cnt);
    
    /* check i4_scrl_step and truncate it if necessary */
    _tktpcntdef_tool_check_scrl_step_size(pt_ctx);

    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx,
        pb_need_scrl_out));
    
    return WGLR_OK;

}

static INT32 _tktpcntdef_cmd_hdr_update_cnt_inset(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2)
{
    WGL_INSET_T     *pt_inset_cnt     = (WGL_INSET_T *)pv_param1;
    BOOL            *pb_need_scrl_out = (BOOL *)pv_param2;
    INT32           i4_ret;
    
    CHECK_NULL(pb_need_scrl_out, WGLR_INV_ARG);
    
    *pb_need_scrl_out = FALSE;

    /* update pt_ctx->t_inset_cnt */    
    if(pt_inset_cnt)
    {
        _INSET_COPY(&pt_ctx->t_inset_cnt, pt_inset_cnt);
    }
    else
    {
        _INSET_EMPTY(&pt_ctx->t_inset_cnt);
    }
    
    /* update vp rect */
    WGL_RECT_COPY(&pt_ctx->t_rc_vp, &pt_ctx->t_rc_cnt);
    _RECT_INSET(&pt_ctx->t_rc_vp, &pt_ctx->t_inset_cnt);
    
    /* check i4_scrl_step and truncate it if necessary */
    _tktpcntdef_tool_check_scrl_step_size(pt_ctx);

    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx,
        pb_need_scrl_out));
    
    return WGLR_OK;   
}        


static INT32 _tktpcntdef_cmd_hdr_update_font(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL        *pb_need_scrl_out = (BOOL *)pv_param1;
    INT32       i4_ret;

    CHECK_NULL(pb_need_scrl_out, WGLR_INV_ARG);
    *pb_need_scrl_out = FALSE;
   
    /* update: i4_cpblk_size */
    CATCH_FAIL(_tktpcntdef_tool_calc_cpblk_size(
        WGL_STR_GET_FONT(pt_ctx->pt_str_msg),
        &pt_ctx->i4_cpblk_size));

    /* check i4_scrl_step and truncate it if necessary */
    _tktpcntdef_tool_check_scrl_step_size(pt_ctx);

    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx,
        pb_need_scrl_out));

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_update_color(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UINT8               ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    GL_COLOR_T          *pt_clr = (GL_COLOR_T *)pv_param2;
    INT32               i4_ret;
    BOOL                b_need_scrl;

    switch(ui1_target) 
    {
        case WGL_CLR_BK:
            if(pt_clr==NULL)
            {
                wgl_get_system_color(WGL_SYS_CLR_TKTP_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clr_bk);
            }
            else
            {
                _COLOR_COPY(&pt_ctx->t_clr_bk, pt_clr);
            }
            break;
        case WGL_CLR_TEXT:
            if(pt_clr==NULL)
            {
                wgl_get_system_color(WGL_SYS_CLR_TKTP_MSG_FG, &pt_ctx->t_cvsst, &pt_ctx->t_clr_msg_fg);
            }
            else
            {
                _COLOR_COPY(&pt_ctx->t_clr_msg_fg, pt_clr);
            }
            break;
        case WGL_CLR_TEXT_BK:
            if(pt_clr==NULL)
            {
                wgl_get_system_color(WGL_SYS_CLR_TKTP_MSG_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clr_msg_bk);
            }
            else
            {
                _COLOR_COPY(&pt_ctx->t_clr_msg_bk, pt_clr);
            }
            break;
        default:
            return WGLR_NOT_SUPPORT_TARGET;
    }

    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx,
        &b_need_scrl));

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_set_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT16      ui2_scrl_step_size = WGL_POINTER_TO_UINT16(pv_param1);
        
    pt_ctx->i4_scrl_step = (INT32)ui2_scrl_step_size;
    _tktpcntdef_tool_check_scrl_step_size(pt_ctx);

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_get_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16  *pui2_step = (UINT16 *)pv_param1;

    CHECK_NULL(pui2_step, WGLR_INV_ARG);
    *pui2_step = (UINT16)pt_ctx->i4_scrl_step;

    return WGLR_OK;
}

static INT32 _tktpcntdef_cmd_hdr_get_color(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    UINT8           ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    GL_COLOR_T      *pt_clr_out = (GL_COLOR_T *)pv_param2;

    CHECK_NULL(pt_clr_out, WGLR_INV_ARG);

    switch(ui1_target) 
    {
        case WGL_CLR_BK:
            _COLOR_COPY(pt_clr_out, &pt_ctx->t_clr_bk);
            break;
        case WGL_CLR_TEXT:
            _COLOR_COPY(pt_clr_out, &pt_ctx->t_clr_msg_fg);
            break;
        case WGL_CLR_TEXT_BK:
            _COLOR_COPY(pt_clr_out, &pt_ctx->t_clr_msg_bk);
            break;
        default:
            return WGLR_NOT_SUPPORT_TARGET;
    }
    
    return WGLR_OK;

}


static INT32 _tktpcntdef_cmd_hdr_get_expected_cnt_height(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32   i4_ret;
    INT32   *pi4_h_cnt = (INT32 *)pv_param1;
    INT32   i4_h_msg;
    
    CHECK_NULL(pi4_h_cnt, WGLR_INV_ARG);

    i4_ret = wgl_str_get_height(pt_ctx->pt_str_msg, &i4_h_msg);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        i4_h_msg = DEF_FONT_H;
    }
    
    *pi4_h_cnt = i4_h_msg + _INSET_GET_H(pt_ctx->t_inset_cnt);

    return WGLR_OK;
}


static INT32 _tktpcntdef_cmd_hdr_get_cnt_inset(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                         *pv_param1)
{
    WGL_INSET_T *pt_inset_cnt = (WGL_INSET_T *)pv_param1;
    CHECK_NULL(pt_inset_cnt, WGLR_INV_ARG);
    
    _INSET_COPY(pt_inset_cnt, &pt_ctx->t_inset_cnt);
    
    return WGLR_OK;   
}        

static INT32 _tktpcntdef_cmd_hdr_set_align(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT8   ui1_align = WGL_POINTER_TO_UINT8(pv_param1);
    BOOL    b_need_scrl;
    INT32   i4_ret;
    
    CHECK_VALID(WGL_IS_VALID_ALIGN(ui1_align), WGLR_INV_ARG);
    pt_ctx->ui1_align_msg = ui1_align;
    
    /* update scrl context */
    CATCH_FAIL(_tktpcntdef_tool_update_scrl_context(
        pt_ctx,
        &b_need_scrl));
    
    return WGLR_OK;
}    

static INT32 _tktpcntdef_cmd_hdr_get_align(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    *((UINT8 *)pv_param1) = pt_ctx->ui1_align_msg;
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
                    toolkit functions
 ----------------------------------------------------------------------------*/
static INT32 _tktpcntdef_tool_update_scrl_context(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_need_scrl)
{
    INT32           i4_ret;
    INT32           i4_scrl_remain_new;
    
    *pb_need_scrl = FALSE;
    
    /* mark the scrolling context as dirty */
    WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY);

    /* check if need scroll */
    CATCH_FAIL(_tktpcntdef_tool_check_need_scrl(pt_ctx, pb_need_scrl, &i4_scrl_remain_new));
    if(*pb_need_scrl)
    {
        GL_RECT_T   t_rc_temp;
        UINT16      ui2_ci_last_new;
        INT32       i4_buf_right_ci_last_new, i4_kern_after_ci_last_new;
        
        /* check the size of the back surface */
        CATCH_FAIL(_tktpcntdef_tool_check_bksurf(pt_ctx, TRUE));
    
        /* render the back surface */
        SET_RECT(
            &t_rc_temp, 
            RECT_W(&pt_ctx->t_rc_vp), 
            0, 
            RECT_W(&pt_ctx->t_rc_bksurf),
            RECT_H(&pt_ctx->t_rc_bksurf));
        CATCH_FAIL(_tktpcntdef_tool_render_msg(
            pt_ctx->h_g_bk, 
            &pt_ctx->t_cvsst,
            &pt_ctx->t_rc_bksurf,
            &t_rc_temp,
            pt_ctx->pt_str_msg,
            WGL_AS_LEFT_TOP,
            &pt_ctx->t_clr_msg_fg,
            &pt_ctx->t_clr_msg_bk,
            &ui2_ci_last_new,
            &i4_buf_right_ci_last_new,
            &i4_kern_after_ci_last_new));
            
        /* set the NEED_SCRL flag */
        WGL_SET_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL);
        /* mark as wait-for-reset */
        /*      mark the scrolling context as ready */
        WGL_SET_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY);
        /*      force to reset the scrolling context */
        WGL_SET_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED);

        /* setup the scrolling context */
        pt_ctx->i4_orig_scrl_remain   = i4_scrl_remain_new;
        pt_ctx->i4_scrl_remain        = i4_scrl_remain_new;
        pt_ctx->i4_offset_bksurf      = 0;
        pt_ctx->ui2_ci_last           = ui2_ci_last_new;
        pt_ctx->i4_buf_right_ci_last  = i4_buf_right_ci_last_new;
        pt_ctx->i4_kern_after_ci_last = i4_kern_after_ci_last_new;
            
    }
    else
    {
        /* clear the NEED_SCRL flag */
        WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_NEED_SCRL);
        
        /* mark as no-need-scroll */
        WGL_CLEAR_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_CTX_READY);
        WGL_SET_FLAG(pt_ctx->ui2_state, TTCDST_SCRL_NOT_RESETED);

        /* clear the scrolling context */
        pt_ctx->b_scrl_once           = FALSE;        
        pt_ctx->i4_orig_scrl_remain   = 0;
        pt_ctx->i4_scrl_remain        = 0;
        pt_ctx->i4_offset_bksurf      = 0;
        pt_ctx->ui2_ci_last           = 0;   
        pt_ctx->i4_buf_right_ci_last  = 0;     
        pt_ctx->i4_kern_after_ci_last = 0;
    }
    
    return WGLR_OK;
}        

static INT32 _tktpcntdef_tool_check_need_scrl(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    OUT     BOOL                        *pb_need_scrl,
    OUT     INT32                       *pi4_scrl_remain)
{
    INT32       i4_msg_w;
    INT32       i4_ret;
    
    *pb_need_scrl = FALSE;
    *pi4_scrl_remain = 0;
    
    /* quick return if the viewport is not with an effective rectangle*/
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_vp))
    {
        return WGLR_OK;
    }
    
    CATCH_FAIL(wgl_str_get_width(pt_ctx->pt_str_msg, &i4_msg_w));

    if((i4_msg_w > RECT_W(&pt_ctx->t_rc_vp)) || 
       (HAS_FLAG(pt_ctx->ui2_attr, TTCD_ATTR_FORCE_SCRL_SHORT_TEXT) && i4_msg_w > 0) )
    {
        *pb_need_scrl = TRUE;
        *pi4_scrl_remain = i4_msg_w + RECT_W(&pt_ctx->t_rc_vp);
    }

    return WGLR_OK;
}


static INT32 _tktpcntdef_tool_render_text_in_bound(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UTF16_T                     *w2s_text,
    IN      UINT16                      ui2_len_text,
    IN      UINT16                      ui2_ci_start,
    IN      INT32                       i4_kern_left,
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_right_bound,
    IN      INT32                       i4_bg_h,
    IN      BOOL                        b_wrap_over,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T                  *pt_clr_fg,
    IN      GL_COLOR_T                  *pt_clr_bk,
    OUT     UINT16                      *pui2_ci_end, 
    OUT     INT32                       *pi4_right,
    OUT     INT32                       *pi4_kern_after_ci_end)
{
    INT32       i4_ret, i4_temp;
    INT32       i4_adv_x, i4_kern_x, i4_right_prv;
    UINT16      ui2_ci;
    UTF16_T     w2_ch_prv, w2_ch;
    BOOL        b_has_kern, b_keep;
    UTF16_T     w2s_buf[2];
    GL_COLOR_T  t_clr_transp;
    
    /*init transparent color*/
    wgl_cvsst_get_transparent_color(pt_cvsst, &t_clr_transp);

    /* quick return if text length is zero */
    if(ui2_len_text==0)
    {
        *pui2_ci_end = ui2_ci_start;
        *pi4_right = i4_left;
        *pi4_kern_after_ci_end = 0;
        return WGLR_OK;
    }

    /* init w2_ch_prv, w2_ch, w2s_buf, i4_kern_x, i4_right_prv */
    ui2_ci = ui2_ci_start;
    b_has_kern = x_fe_has_kerning(h_font);
    w2_ch_prv = WGL_CHAR_NULL;
    w2_ch = x_uc_w2s_get(w2s_text, ui2_ci);
    w2s_buf[0] = w2_ch;
    w2s_buf[1] = WGL_CHAR_NULL;
    i4_kern_x = i4_kern_left;
    i4_right_prv = i4_left;
    b_keep = TRUE;
    while(b_keep)
    {
        /* get i4_adv_x */
        i4_ret = x_fe_get_character_size(h_font, w2_ch, &i4_adv_x, &i4_temp);
        if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
        {
            INFORM_ERR(i4_ret);
            i4_adv_x = DEF_FONT_W;
        }

        /* check if the character will go out of the bound */
        if(i4_right_prv + i4_kern_x + i4_adv_x > i4_right_bound )
        {
            if(ui2_ci==0)
            {
                *pui2_ci_end = ui2_len_text - (UINT16)1;
                *pi4_kern_after_ci_end = 0;
            }
            else
            {
                *pui2_ci_end = ui2_ci -(UINT16)1;
                *pi4_kern_after_ci_end = i4_kern_x;
            }
            b_keep = FALSE;
            break;
        }
        
        if( wgl_cvsst_check_palette_mode(pt_cvsst) &&
            wgl_cvsst_check_color_transparent(pt_cvsst, pt_clr_fg))
        {
            pt_clr_fg = pt_clr_bk;
        }
        
        /* draw w2_ch */
        INFORM_ERR(x_gl_draw_ustring_ex(
            h_g,
            i4_right_prv + i4_kern_x,
            i4_top,
            i4_top,
            w2s_buf,
            1,
            NULL,
            0,
            h_font,
            pt_clr_fg,
            pt_clr_bk,
            &t_clr_transp,
            FALSE,
            0,
            0,
            (UINT16)i4_bg_h,
            -i4_kern_x));

        /* increase i4_right_prv to be the left of the next character */
        i4_right_prv += (i4_kern_x + i4_adv_x);
        
        /* get the next character */
        if(ui2_ci+(UINT16)1 < ui2_len_text)
        {
            ui2_ci++;
            w2_ch_prv = w2_ch;
            w2_ch = x_uc_w2s_get(w2s_text, ui2_ci);
            w2s_buf[0] = w2_ch;
        
            /* get kerning */
            if(b_has_kern)
            {
                i4_ret = x_fe_get_kerning(h_font, w2_ch_prv, w2_ch, &i4_kern_x, &i4_temp);
                if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
                {
                    INFORM_ERR(i4_ret);
                    i4_kern_x = 0;
                }
            }
            else
            {
                i4_kern_x = 0;
            }
        }
        else
        {   /* the last character is encountered */
            if(b_wrap_over)
            {
                ui2_ci = 0;
                w2_ch_prv = WGL_CHAR_NULL;
                w2_ch = x_uc_w2s_get(w2s_text, 0);
                w2s_buf[0] = w2_ch;
                i4_kern_x = 0;
            }
            else
            {
                *pui2_ci_end = ui2_ci;  
                *pi4_kern_after_ci_end = 0;
                b_keep = FALSE;
            }
        }
    }

    *pi4_right = i4_right_prv;

    return WGLR_OK;
}

static INT32 _tktpcntdef_tool_render_msg(
    IN      GL_HGRAPHICS_T              h_g_dst,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      GL_RECT_T                   *pt_rc_bk,
    IN      GL_RECT_T                   *pt_rc_msg,
    IN      WGL_STRING_T                *pt_str_msg,
    IN      UINT8                       ui1_align_msg,
    IN      GL_COLOR_T                  *pt_clr_msg_fg,
    IN      GL_COLOR_T                  *pt_clr_msg_bk,
    OUT     UINT16                      *pui2_ci_end, 
    OUT     INT32                       *pi4_right,
    OUT     INT32                       *pi4_kern_after_ci_end)
{
    INT32       i4_ret, i4_h_msg, i4_w_msg;
    INT32       i4_left_msg, i4_top_msg;

    *pui2_ci_end           = 0;
    *pi4_right             = 0;
    *pi4_kern_after_ci_end = 0;

    /* quick return if the viewport is not with an effective rectangle*/
    if(!RECT_IS_EFFECTIVE(*pt_rc_msg))
    {
        return WGLR_OK;
    }

    /* clear the background */
    if (!wgl_cvsst_check_color_transparent(pt_cvsst, pt_clr_msg_bk))
    {
        INFORM_ERR(x_gl_set_color(h_g_dst, pt_clr_msg_bk));
        INFORM_ERR(x_gl_fill_rect(h_g_dst, pt_rc_bk));
    }
    
    /* get the message width */
    CATCH_FAIL(wgl_str_get_width(pt_str_msg, &i4_w_msg));
    
    /* get the message height */
    i4_ret = wgl_str_get_height(pt_str_msg, &i4_h_msg);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        i4_h_msg = DEF_FONT_H;
    }

    /* determine the text position */
    _tktpcntdef_tool_calc_obj_pos(
        pt_rc_msg,
        ui1_align_msg,
        i4_w_msg,
        i4_h_msg,
        &i4_left_msg,
        &i4_top_msg);
    
    /* render the message text */
    INFORM_ERR(_tktpcntdef_tool_render_text_in_bound(
        h_g_dst,
        pt_cvsst,
        WGL_STR_GET_TEXT(pt_str_msg),
        WGL_STR_GET_TEXT_LEN(pt_str_msg),
        0,                                  /* ui2_ci_start */
        0,                                  /* i4_kern_left */
        i4_left_msg,                        /* i4_left */
        i4_top_msg,                         /* i4_top */
        pt_rc_msg->i4_right,                /* i4_right_bound */
        i4_h_msg,                           /* i4_bg_h */
        FALSE,                              /* b_wrap_over */
        WGL_STR_GET_FONT(pt_str_msg),   
        pt_clr_msg_fg,                  
        pt_clr_msg_bk,
        pui2_ci_end,
        pi4_right,
        pi4_kern_after_ci_end));

    return WGLR_OK;

}

static INT32 _tktpcntdef_tool_check_bksurf(
    IN      TKTPCNTDEF_CTX_T            *pt_ctx,
    IN      BOOL                        b_need_scrl)
{
    INT32   i4_ret;
    UINT32  ui4_orig_w, ui4_orig_h, ui4_new_w, ui4_new_h;
    INT32   i4_h;
    INT16   i2_font_w, i2_temp;
    
    /* quick return for viewport without an effective size */
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_vp))
    {
        return WGLR_OK;
    }
    
    /* do not check surface if don't need scrl */
    if(!b_need_scrl)
    {
        return WGLR_OK;
    }
    
    /* calculate new width */
    i4_ret = x_fe_get_max_advance(
        WGL_STR_GET_FONT(pt_ctx->pt_str_msg), 
        &i2_font_w,
        &i2_temp);
    if(i4_ret != FER_OK)
    {
        INFORM_ERR(i4_ret);
        i2_font_w = DEF_FONT_W;
    }
    ui4_new_w = (UINT32)(RECT_W(&pt_ctx->t_rc_vp) + BKSURF_PAD_FACTOR * ((INT32)i2_font_w));
    
    /* calculate new height */
    i4_ret = wgl_str_get_height(pt_ctx->pt_str_msg, &i4_h);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        i4_h = DEF_FONT_H;
    }
    ui4_new_h = (UINT32)i4_h;

    /* get old size */    
    if(pt_ctx->h_surf_bk==NULL_HANDLE)
    {
        ui4_orig_w = ui4_orig_h = 0;
    }
    else
    {
        GL_SURF_INFO_T t_surf_info;
        CATCH_FAIL(x_gl_surface_get_info(pt_ctx->h_surf_bk, &t_surf_info));
        ui4_orig_w = t_surf_info.ui4_width;
        ui4_orig_h = t_surf_info.ui4_height;
    }

    /* create a larger one if necessary */
    if(ui4_new_w > ui4_orig_w || ui4_new_h > ui4_orig_h)
    {
        GL_SURFACE_DESC_T   t_surf_desc;
        GL_HSURFACE_T       h_surf_new;
        GL_HGRAPHICS_T      h_g_new;
        GL_COLOR_T          t_clr_transp;
        
        t_surf_desc.ui4_width = ui4_new_w;
        t_surf_desc.ui4_height = ui4_new_h;
        t_surf_desc.e_clrmode = pt_ctx->t_cvsst.e_clr_mode;
        t_surf_desc.h_clipper = NULL_HANDLE;
        t_surf_desc.h_palette = pt_ctx->t_cvsst.h_palette;

        CATCH_FAIL(x_gl_surface_create(&t_surf_desc, &h_surf_new ));
        i4_ret = x_gl_create_graphics(h_surf_new, NULL, NULL, &h_g_new);
        if( i4_ret != GLR_OK)
        {
            INFORM_ERR(i4_ret);
            INFORM_ERR(x_gl_obj_free(h_surf_new));
            return i4_ret;
        }

        if(pt_ctx->h_g_bk != NULL_HANDLE)
        {
            INFORM_ERR(x_gl_obj_free(pt_ctx->h_g_bk));
            pt_ctx->h_g_bk = NULL_HANDLE;
        }
        if(pt_ctx->h_surf_bk != NULL_HANDLE)
        {
            INFORM_ERR(x_gl_obj_free(pt_ctx->h_surf_bk));
            pt_ctx->h_surf_bk = NULL_HANDLE;
        }
        
        pt_ctx->h_surf_bk = h_surf_new;
        pt_ctx->h_g_bk = h_g_new;

        /* update rectangle: t_rc_bksurf */
        SET_RECT_BY_SIZE(&pt_ctx->t_rc_bksurf, 0, 0, (INT32)ui4_new_w, (INT32)ui4_new_h);

        /* clean surface */
        wgl_cvsst_get_transparent_color( &pt_ctx->t_cvsst, &t_clr_transp );
        INFORM_ERR( x_gl_surface_fill( pt_ctx->h_surf_bk, &t_clr_transp, GL_SYNC ) );
    }

    return WGLR_OK;
}

static VOID _tktpcntdef_tool_check_scrl_step_size(
    IN OUT  TKTPCNTDEF_CTX_T            *pt_ctx)
{
    
    if(RECT_IS_EFFECTIVE(pt_ctx->t_rc_vp))
    {
        /* truncate the scroll step size within the width of the font */
        if(pt_ctx->i4_scrl_step > pt_ctx->i4_cpblk_size)
        {
            pt_ctx->i4_scrl_step = pt_ctx->i4_cpblk_size;
        }
        
        /* truncate the scroll step size within the width of viewport */
        if(pt_ctx->i4_scrl_step > RECT_W(&pt_ctx->t_rc_vp) )
        {
            pt_ctx->i4_scrl_step = RECT_W(&pt_ctx->t_rc_vp);
        }
    }

}

static INT32 _tktpcntdef_tool_calc_cpblk_size(
    IN      HFONT_T                     h_font_msg,
    OUT     INT32                       *pi4_cpblk_size)
{
    INT16       i2_font_w, i2_temp;
    INT32       i4_ret;
    
    i4_ret = x_fe_get_max_advance(
        h_font_msg, 
        &i2_font_w,
        &i2_temp);
    if(i4_ret != FER_OK)
    {
        INFORM_ERR(i4_ret);
        *pi4_cpblk_size = DEF_FONT_W;
    }

    *pi4_cpblk_size = (INT32)i2_font_w;

    return WGLR_OK;
}

static VOID _tktpcntdef_tool_calc_obj_pos(
    IN      GL_RECT_T                   *pt_rc_ref,
    IN      UINT8                       ui1_align_obj,
    IN      INT32                       i4_w_obj,
    IN      INT32                       i4_h_obj,
    OUT     INT32                       *pi4_left,
    OUT     INT32                       *pi4_top)
{
    if( WGL_IS_H_LEFT(ui1_align_obj) )
    {
        *pi4_left = pt_rc_ref->i4_left;
    }
    else if( WGL_IS_H_CENTER(ui1_align_obj) )
    {
        *pi4_left = ((RECT_W(pt_rc_ref) - i4_w_obj) >> 1) + pt_rc_ref->i4_left;
    }
    else
    {
        *pi4_left = pt_rc_ref->i4_right - i4_w_obj;
    }

    if( WGL_IS_V_TOP(ui1_align_obj) )
    {
        *pi4_top = pt_rc_ref->i4_top;
    }
    else if( WGL_IS_V_CENTER(ui1_align_obj) )
    {
        *pi4_top = ((RECT_H(pt_rc_ref) - i4_h_obj) >> 1) + pt_rc_ref->i4_top;
    }
    else
    {
        *pi4_top = pt_rc_ref->i4_bottom - i4_h_obj;
    }
    
}    

static VOID _tktpcntdef_tool_calc_obj_vertical_pos(
    IN      INT32                       i4_top_ref,
    IN      INT32                       i4_btm_ref,
    IN      UINT8                       ui1_align_obj,
    IN      INT32                       i4_h_obj,
    OUT     INT32                       *pi4_top)
{

    if( WGL_IS_V_TOP(ui1_align_obj) )
    {
        *pi4_top = i4_top_ref;
    }
    else if( WGL_IS_V_CENTER(ui1_align_obj) )
    {
        *pi4_top = (((i4_btm_ref - i4_top_ref) - i4_h_obj) >> 1) + i4_top_ref;
    }
    else
    {
        *pi4_top = i4_btm_ref - i4_h_obj;
    }
    
}    




