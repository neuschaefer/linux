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
 * $RCSfile: wgl_hts_cnt_def.c,v $
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
#include "os/inc/x_os.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/wgl_image.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/hts/wgl_hts.h"
#include "wgl/hts/wgl_hts_cnt_internal.h"
#include "wgl/hts/wgl_hts_cnt_def.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "HTSCNT"


#define _RANGE_IS_INTERSECT(_i4_start_r1, _i4_end_r1, _i4_start_r2, _i4_end_r2) \
         (!((_i4_end_r1)<=(_i4_start_r2) || (_i4_start_r1)>=(_i4_end_r2)))

#define _GET_ROW_EFF_H(_pt_row)                                                 \
            ((INT32)(_pt_row)->ui2_height)

#define _SET_BOOL_BY_OR(_b_target, _b_new)                                      \
    (_b_target) = (_b_new) ? TRUE : (_b_target)

#define _RECT_MOVE(_pt_rc_src, _i4_dx, _i4_dy, _pt_rc_out)                      \
    {                                                                           \
        (_pt_rc_out)->i4_left   = (_pt_rc_src)->i4_left + (_i4_dx);             \
        (_pt_rc_out)->i4_top    = (_pt_rc_src)->i4_top + (_i4_dy);              \
        (_pt_rc_out)->i4_right  = (_pt_rc_src)->i4_right + (_i4_dx);            \
        (_pt_rc_out)->i4_bottom = (_pt_rc_src)->i4_bottom + (_i4_dy);           \
    }


/*-----------------------------------------------------------------------------
                    defaults & constants
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _htscntdef_cmd_hdr_set_cnt_inset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_cnt_inset(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_set_page_indc_img(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_page_indc_img(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_set_hts_scrn_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_hts_scrn_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_set_bdr_inset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_update_cnt_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_purc_by_sec(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_scrl_up(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_scrl_down(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_page_up(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_page_down(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_page_num(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_set_vli_first(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _htscntdef_cmd_hdr_get_vli_first(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_get_vli_last(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _htscntdef_cmd_hdr_set_color(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_get_color(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_set_img(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_get_img(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _htscntdef_cmd_hdr_reset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx);
static INT32 _htscntdef_cmd_hdr_clear_all(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx);
static INT32 _htscntdef_tool_check_valid_img(
    IN      WGL_HIMG_TPL_T              h_img);
static INT32 _htscntdef_tool_get_color_set_std(
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs_dst, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info_src);
static VOID _htscntdef_tool_get_color_info_std(
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_std, 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out);
static INT32 _htscntdef_tool_get_img_set_std(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_out, 
    IN      WGL_IMG_INFO_T              *pt_img_info);
static VOID _htscntdef_tool_get_img_info_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
    OUT     WGL_IMG_INFO_T              *pt_img_info_out);
static INT32 _htscntdef_tool_paint(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_update);
static INT32 _htscntdef_tool_render_line(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      GL_RECT_T                   *pt_rc_g_update,
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      INT32                       i4_pu_left,  
    IN      INT32                       i4_pu_right, 
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_bound_w, 
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm);
static INT32 _htscntdef_tool_render_text_sec(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_text,
    IN      UINT16                      ui2_ci_sec_start,
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm,
    IN      INT32                       i4_line_base);
static INT32 _htscntdef_tool_get_text_sec_width( 
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_text,
    IN      UINT16                      ui2_ci_sec_start,
    IN      INT32                       i4_bound_w,
    IN      INT32                       i4_left,
    OUT     INT32                       *pi4_sec_w);
static INT32 _htscntdef_tool_render_img_sec(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      GL_RECT_T                   *pt_rc_g_update,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_img,
    IN      INT32                       i4_left,
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm,
    IN      INT32                       i4_line_base);
static INT32 _htscntdef_tool_draw_page_indicator_images(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_HTS_PAGE_INDC_INFO_T    *pt_page_imgs,
    IN      BOOL                        b_is_up_on,
    IN      BOOL                        b_is_down_on,
    IN      GL_RECT_T                   *pt_rc_update);
static BOOL _htscntdef_tool_is_rect_intersect(
    IN      GL_RECT_T                   *pt_rc_1,
    IN      GL_RECT_T                   *pt_rc_2);
static INT32 _htscntdef_tool_fill_bk(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint);
static INT32 _htscntdef_tool_fill_vp_bk(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint);
static VOID _htscntdef_tool_select_clr_std(
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_src,
    IN      UINT16                      ui2_state_paint,
    OUT     GL_COLOR_T                  **ppt_clr_out);
static VOID _htscntdef_tool_select_img_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
    IN      UINT16                      ui2_state_paint,
    OUT     WGL_HIMG_TPL_T              *ph_img_out);
static INT32 _htscntdef_tool_get_line_inset(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_line_inset_top,
    OUT     INT32                       *pi4_line_inset_bottom);
static INT32 _htscntdef_tool_get_parag_gap(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_parag_gap_top,
    OUT     INT32                       *pi4_parag_gap_bottom);
static INT32 _htscntdef_tool_get_line_gap(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_line_inset_top,
    OUT     INT32                       *pi4_line_inset_bottom);
static INT32 _htscntdef_tool_calc_text_sec_top(
    IN      WGL_HTMGR_V_ANCHOR_T        e_v_anchor,
    IN      INT32                       i4_line_top,
    IN      INT32                       i4_line_bottom,
    IN      INT32                       i4_line_base,
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_sec_inset_top,
    IN      INT32                       i4_sec_h,
    IN      INT32                       i4_sec_eff_h,
    OUT     INT32                       *pi4_sec_top);
static INT32 _htscntdef_tool_calc_img_sec_top(
    IN      WGL_HTMGR_V_ANCHOR_T        e_v_anchor,
    IN      INT32                       i4_line_top,
    IN      INT32                       i4_line_bottom,
    IN      INT32                       i4_line_base,
    IN      INT32                       i4_img_sec_eff_h,
    OUT     INT32                       *pi4_sec_top);
static INT32 _htscntdef_load_dft_color_set(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      UINT8                       ui1_target);


/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_hts_cnt_def_inst_create(
    WGL_CONTENT_CONTEXT_HANDLE_T        *ppv_ctx_cnt, 
    WGL_CONTENT_CREATE_INPUT_T          *pt_cnt_create )
{
    HTSCNTDEF_CTX_T     *pt_ctx = NULL;
    HTS_CNT_INIT_T      *pt_init = (HTS_CNT_INIT_T *)pt_cnt_create->pv_init;
    INT32               i4_ret, i4_ret_val;
    UINT16              ui2_vp_attrs;
    
    *ppv_ctx_cnt = NULL;
    CHECK_NULL(pt_init, WGLR_INV_ARG);

    pt_ctx = WGL_MEM_ALLOC(sizeof(HTSCNTDEF_CTX_T));    
    CHECK_NULL(pt_ctx, WGLR_OUT_OF_MEMORY);
#ifdef CLI_LVL_ALL
     pt_ctx ->ui4_mem_size = sizeof(HTSCNTDEF_CTX_T); 
#endif
___BLOCK_BEGIN___
    pt_ctx->ui4_styles                       = pt_init->ui4_styles;
    pt_ctx->pt_htmgr                         = pt_init->pt_htmgr;
    pt_ctx->t_page_indc.t_location_up.i4_x   = 0;
    pt_ctx->t_page_indc.t_location_up.i4_y   = 0;
    pt_ctx->t_page_indc.t_location_down.i4_x = 0;
    pt_ctx->t_page_indc.t_location_down.i4_y = 0;
    pt_ctx->t_page_indc.h_img_up_on          = NULL_HANDLE;
    pt_ctx->t_page_indc.h_img_up_off         = NULL_HANDLE;
    pt_ctx->t_page_indc.h_img_down_on        = NULL_HANDLE;
    pt_ctx->t_page_indc.h_img_down_off       = NULL_HANDLE;
    WGL_RECT_EMPTY(&pt_ctx->t_rc_update);
    
    /* init viewport */
    ui2_vp_attrs = 0;
    if(HAS_FLAG(pt_init->ui4_styles, WGL_STL_HTS_NEW_PAGE_KEEP_LAST_LINE))
    {
        WGL_SET_FLAG(ui2_vp_attrs, WGL_VP_ATTR_NEW_PAGE_KEEP_LAST_LINE);
    }
    if(HAS_FLAG(pt_init->ui4_styles, WGL_STL_HTS_FORCE_FILL_LAST_PAGE))
    {
        WGL_SET_FLAG(ui2_vp_attrs, WGL_VP_ATTR_FINAL_PAGE_DOWN_FILL_VP);
    }
    
    GOTO_EOB_AT_FAIL(wgl_hts_vp_init(&pt_ctx->t_vp, ui2_vp_attrs, (UINT16)RECT_H(&pt_cnt_create->t_rect_cnt), (VOID *)pt_ctx->pt_htmgr), i4_ret_val);
    pt_ctx->b_vp_moved = TRUE;
    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_cnt_create->pt_cvsst);
    
    WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_hts);
    _INSET_COPY(&pt_ctx->t_inset_bdr, &pt_init->t_inset_bdr);
    _INSET_EMPTY(&pt_ctx->t_inset_cnt);
    WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_cnt);
    WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_vp);
    
    GOTO_EOB_AT_FAIL(_htscntdef_load_dft_color_set(pt_ctx, WGL_CLR_BK), i4_ret_val);
    GOTO_EOB_AT_FAIL(_htscntdef_load_dft_color_set(pt_ctx, WGL_CLR_HTS_VP_BK), i4_ret_val);

    pt_ctx->t_imgs_bk.t_enable       = NULL_HANDLE;
    pt_ctx->t_imgs_bk.t_disable      = NULL_HANDLE;
    pt_ctx->t_imgs_bk.t_highlight    = NULL_HANDLE;
    pt_ctx->t_imgs_vp_bk.t_enable    = NULL_HANDLE;
    pt_ctx->t_imgs_vp_bk.t_disable   = NULL_HANDLE;
    pt_ctx->t_imgs_vp_bk.t_highlight = NULL_HANDLE;
    
___BLOCK_END___

    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret_val;
    }    
    
    *ppv_ctx_cnt = (WGL_CONTENT_CONTEXT_HANDLE_T)pt_ctx;
    return WGLR_OK;
}

            
INT32 wgl_hts_cnt_def_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt )
{
    HTSCNTDEF_CTX_T     *pt_ctx = (HTSCNTDEF_CTX_T *)pv_ctx_cnt;
    INT32               i4_ret;
    
    INFORM_ERR(wgl_hts_vp_destroy(&pt_ctx->t_vp));
    
    x_memset(pt_ctx, 0, sizeof(HTSCNTDEF_CTX_T));
    WGL_MEM_FREE(pt_ctx);
    pt_ctx = NULL;
    
    return WGLR_OK;    
}

        
INT32 wgl_hts_cnt_def_cmd_proc(  
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    UINT32                              ui4_cnt_cmd_id, 
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{
    INT32               i4_ret, i4_ret_val;
    HTSCNTDEF_CTX_T     *pt_ctx = (HTSCNTDEF_CTX_T *)pv_ctx_cnt;

    i4_ret_val = WGLR_OK;
    
    switch(ui4_cnt_cmd_id)
    {
        /* common content commands */
        case WGL_CMD_HTS_SET_CNT_INSET:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_set_cnt_inset(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_CNT_INSET:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_cnt_inset(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_SET_PAGE_INDC_IMG:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_set_page_indc_img(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_PAGE_INDC_IMG:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_page_indc_img(pt_ctx, pv_param1);
        break;
        
        /* internal content commands */
        case WGL_CNT_HTS_SET_HTS_SCRN_RECT:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_set_hts_scrn_rect(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_GET_HTS_SCRN_RECT:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_hts_scrn_rect(pt_ctx, pv_param1);
        break;

        case WGL_CNT_HTS_SET_BDR_INSET:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_set_bdr_inset(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_UPDATE_CNT_RECT:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_update_cnt_rect(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_GET_PURC_BY_SEC:
            i4_ret_val = _htscntdef_cmd_hdr_get_purc_by_sec(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_SCRL_UP:
            i4_ret_val = _htscntdef_cmd_hdr_scrl_up(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_SCRL_DOWN:
            i4_ret_val = _htscntdef_cmd_hdr_scrl_down(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_PAGE_UP:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_page_up(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_PAGE_DOWN:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_page_down(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_GET_PAGE_NUM:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_page_num(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_SET_VLI_FIRST:
            i4_ret_val = _htscntdef_cmd_hdr_set_vli_first(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_GET_VLI_FIRST:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_vli_first(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_GET_VLI_LAST:
            CHECK_VALID(pv_param2==NULL, WGLR_INV_ARG);
            i4_ret_val = _htscntdef_cmd_hdr_get_vli_last(pt_ctx, pv_param1);
        break;
        
        case WGL_CNT_HTS_GL_SET_COLOR:
            i4_ret_val = _htscntdef_cmd_hdr_set_color(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_GL_GET_COLOR:
            i4_ret_val = _htscntdef_cmd_hdr_get_color(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_GL_SET_IMG:
            i4_ret_val = _htscntdef_cmd_hdr_set_img(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_GL_GET_IMG:
            i4_ret_val = _htscntdef_cmd_hdr_get_img(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CNT_HTS_RESET:
            i4_ret_val = _htscntdef_cmd_hdr_reset(pt_ctx);
        break;
        
        case WGL_CNT_HTS_CLEAR_ALL:
            i4_ret_val = _htscntdef_cmd_hdr_clear_all(pt_ctx);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_CMD);
            i4_ret_val = WGLR_INV_CMD;
    }        
    
    return i4_ret_val;
}



INT32 wgl_hts_cnt_def_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state_paint)
{
    INT32               i4_ret;
    HTSCNTDEF_CTX_T     *pt_ctx = (HTSCNTDEF_CTX_T *)pv_ctx_cnt;
    
    /* check for quick return */
    if(   !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_hts) 
       || !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_cnt)
       || !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        return WGLR_OK;
    }
    
    /* copy t_rc_update */
    if(ui4_upt_rect==1 && RECT_IS_EFFECTIVE(pt_upt_rect[0]))
    {
        WGL_RECT_COPY(&pt_ctx->t_rc_update, &pt_upt_rect[0]);
    }
    else
    {
        SET_RECT_BY_SIZE(&pt_ctx->t_rc_update, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_hts), RECT_H(&pt_ctx->t_rc_scrn_hts));
    }
    
    if(   RECT_W(&pt_ctx->t_rc_update)==RECT_W(&pt_ctx->t_rc_scrn_hts)
       && RECT_H(&pt_ctx->t_rc_update)==RECT_H(&pt_ctx->t_rc_scrn_hts))
    {   /* paint all */
        pt_ctx->b_vp_moved = FALSE;
        INFORM_ERR(_htscntdef_tool_paint(pt_ctx, h_g, ui2_state_paint, NULL));
    }
    else
    {   /* paint partial */
        INFORM_ERR(_htscntdef_tool_paint(pt_ctx, h_g, ui2_state_paint, &pt_ctx->t_rc_update));
    }
    

    return WGLR_OK;
}
    

/*-----------------------------------------------------------------------------
                    command handling functions
 ----------------------------------------------------------------------------*/

static INT32 _htscntdef_cmd_hdr_set_cnt_inset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1)
{
    
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    x_memcpy(&pt_ctx->t_inset_cnt, pv_param1, sizeof(WGL_INSET_T));
 
    return WGLR_OK;   
}        

static INT32 _htscntdef_cmd_hdr_get_cnt_inset(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    x_memcpy(pv_param1, &pt_ctx->t_inset_cnt, sizeof(WGL_INSET_T));
    
    return WGLR_OK;    
}    

static INT32 _htscntdef_cmd_hdr_set_page_indc_img(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32                       i4_ret;
    WGL_HTS_PAGE_INDC_INFO_T    *pt_page_indc = (WGL_HTS_PAGE_INDC_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_page_indc, WGLR_INV_ARG);
    
    if(pt_page_indc->h_img_up_on!=NULL_HANDLE)
    {
        CATCH_FAIL(_htscntdef_tool_check_valid_img(pt_page_indc->h_img_up_on));
    }
    if(pt_page_indc->h_img_up_off!=NULL_HANDLE)
    {
        CATCH_FAIL(_htscntdef_tool_check_valid_img(pt_page_indc->h_img_up_off));
    }
    if(pt_page_indc->h_img_down_on!=NULL_HANDLE)
    {
        CATCH_FAIL(_htscntdef_tool_check_valid_img(pt_page_indc->h_img_down_on));
    }
    if(pt_page_indc->h_img_down_off!=NULL_HANDLE)
    {
        CATCH_FAIL(_htscntdef_tool_check_valid_img(pt_page_indc->h_img_down_off));
    }
    
    x_memcpy(&pt_ctx->t_page_indc, pt_page_indc, sizeof(WGL_HTS_PAGE_INDC_INFO_T));
    
    return WGLR_OK;   
}        

static INT32 _htscntdef_cmd_hdr_get_page_indc_img(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_HTS_PAGE_INDC_INFO_T    *pt_page_indc_out = (WGL_HTS_PAGE_INDC_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_page_indc_out, WGLR_INV_ARG);
    x_memcpy(pt_page_indc_out, &pt_ctx->t_page_indc, sizeof(WGL_HTS_PAGE_INDC_INFO_T));
    
    return WGLR_OK;    
}    


static INT32 _htscntdef_cmd_hdr_set_hts_scrn_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1)
{
    GL_RECT_T *pt_rc_scrn_hts = (GL_RECT_T *)pv_param1;
    
    CHECK_NULL(pt_rc_scrn_hts, WGLR_INV_ARG);
    WGL_RECT_COPY(&pt_ctx->t_rc_scrn_hts, pt_rc_scrn_hts);
 
    return WGLR_OK;   
}        

static INT32 _htscntdef_cmd_hdr_get_hts_scrn_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    GL_RECT_T *pt_rc_scrn_hts_out = (GL_RECT_T *)pv_param1;
    
    CHECK_NULL(pt_rc_scrn_hts_out, WGLR_INV_ARG);
    WGL_RECT_COPY(pt_rc_scrn_hts_out, &pt_ctx->t_rc_scrn_hts);
 
    return WGLR_OK;   
}        


static INT32 _htscntdef_cmd_hdr_set_bdr_inset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_INSET_T *pt_inset_bdr = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_bdr, WGLR_INV_ARG);
    _INSET_COPY(&pt_ctx->t_inset_bdr, pt_inset_bdr);
 
    return WGLR_OK;   
}        


static INT32 _htscntdef_cmd_hdr_update_cnt_rect(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    HTS_CNT_UPDATE_CNT_RECT_RET_T   *pt_ret_out = (HTS_CNT_UPDATE_CNT_RECT_RET_T *)pv_param1;
    INT32                           i4_size_vp_orig_w, i4_size_vp_orig_h;
    INT32                           i4_ret;
    
    CHECK_NULL(pt_ret_out, WGLR_INV_ARG);
    
    i4_size_vp_orig_w = RECT_W(&pt_ctx->t_rc_scrn_vp);
    i4_size_vp_orig_h = RECT_H(&pt_ctx->t_rc_scrn_vp);
    
    WGL_RECT_COPY(&pt_ctx->t_rc_scrn_cnt, &pt_ctx->t_rc_scrn_hts);
    _RECT_INSET(&pt_ctx->t_rc_scrn_cnt, &pt_ctx->t_inset_bdr);
    WGL_RECT_COPY(&pt_ctx->t_rc_scrn_vp, &pt_ctx->t_rc_scrn_cnt);
    _RECT_INSET(&pt_ctx->t_rc_scrn_vp, &pt_ctx->t_inset_cnt);
    
    pt_ret_out->b_vp_size_changed = (RECT_W(&pt_ctx->t_rc_scrn_vp)!=i4_size_vp_orig_w || RECT_H(&pt_ctx->t_rc_scrn_vp)!=i4_size_vp_orig_h) ? TRUE : FALSE;
    if(RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        pt_ret_out->t_size_vp.ui4_width  = (UINT32)RECT_W(&pt_ctx->t_rc_scrn_vp);
        pt_ret_out->t_size_vp.ui4_height = (UINT32)RECT_H(&pt_ctx->t_rc_scrn_vp);
    }
    else
    {
        pt_ret_out->t_size_vp.ui4_width  = 0;
        pt_ret_out->t_size_vp.ui4_height = 0;
    }        
    
    /* update viewport size */
    if(pt_ret_out->b_vp_size_changed)
    {
        BOOL    b_vp_moved = FALSE;
        CATCH_FAIL(wgl_hts_vp_set_vp_size(&pt_ctx->t_vp, (UINT16)pt_ret_out->t_size_vp.ui4_height, &b_vp_moved));
        _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
    }
    
    return WGLR_OK;   
}        

static INT32 _htscntdef_cmd_hdr_get_purc_by_sec(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    INT32                           i4_ret;
    UINT16                          ui2_sid_target  = WGL_POINTER_TO_UINT16(pv_param1);
    GL_RECT_T                       *pt_rc_purc_out = (GL_RECT_T *)pv_param2;
    GL_RECT_T                       t_rc_scrn_purc, t_rc_scrn_sec, t_rc_scrn_purc_temp;
    GL_RECT_T                       t_rc_scrn_row;
    UINT16                          ui2_first, ui2_last, ui2_cur_size, ui2_eff_last;
    UINT16                          ui2_num_row;
    WGL_HTMGR_ROW_CACHE_T           *pt_row, *pt_row_first, *pt_row_last;
    WGL_HTMGR_SEC_INFO_T            *pt_sec;
    INT32                           i4_scrn_prev_line_btm, i4_scrn_left;
    INT32                           i4_eff_line_h, i4_bound_w, i4_sec_w;
    UINT16                          ui2_ci_sec_start;
    UINT16                          ui2_sid, ui2_sid_first, ui2_sid_last;
    BOOL                            b_vp_moved;
    
    CHECK_NULL(pt_rc_purc_out, WGLR_INV_ARG);
    WGL_RECT_EMPTY(pt_rc_purc_out);
    
    /* check for quick return */
    if(   !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_hts) 
       || !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_cnt)
       || !RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        return WGLR_OK;
    }
    
    #define MAX_COORD  ((INT32) 0xFFFF)
    #define MIN_COORD  ((INT32)-0xFFFF)
    SET_RECT(&t_rc_scrn_purc, MAX_COORD, MAX_COORD, MIN_COORD, MIN_COORD);

    CATCH_FAIL(wgl_hts_vp_get_state(&pt_ctx->t_vp, &ui2_first, &ui2_last, &ui2_cur_size, &b_vp_moved));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
    
    if(pt_ctx->b_vp_moved)  
    {
        SET_RECT_BY_SIZE(pt_rc_purc_out, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_hts), RECT_H(&pt_ctx->t_rc_scrn_hts));
        return WGLR_OK;
    }
    
    /* check the last half-visible line if necessary*/    
    ui2_eff_last = ui2_last;
    ui2_num_row = HTMGR_GET_ROW_NUM(pt_ctx->pt_htmgr);
    if(   HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_HTS_SHOW_HALF_VISIBLE_LINE)
       && ((INT32)ui2_cur_size) < RECT_H(&pt_ctx->t_rc_scrn_vp)
       && ui2_last + (UINT16)1 < ui2_num_row)
    {
        ui2_eff_last++;
    }
    
    CATCH_FAIL(wgl_htmgr_get_row_info(pt_ctx->pt_htmgr, ui2_first, &pt_row_first));
    CATCH_FAIL(wgl_htmgr_get_row_info(pt_ctx->pt_htmgr, ui2_eff_last, &pt_row_last));
    
    /* check section id for quick return */
    CATCH_FAIL(wgl_htmgr_get_sec_id(pt_ctx->pt_htmgr, pt_row_first->pt_sec_start, &ui2_sid_first));
    CATCH_FAIL(wgl_htmgr_get_sec_id(pt_ctx->pt_htmgr, pt_row_last->pt_sec_end, &ui2_sid_last));
    if(!(ui2_sid_target >= ui2_sid_first && ui2_sid_target <= ui2_sid_last ))
    {
        return WGLR_OK;
    }
    
    i4_bound_w            = RECT_W(&pt_ctx->t_rc_scrn_vp);
    pt_row                = pt_row_first;
    ui2_sid               = ui2_sid_first;
    pt_sec                = pt_row->pt_sec_start;
    i4_scrn_prev_line_btm = pt_ctx->t_rc_scrn_vp.i4_top;
    while(pt_row)
    {
        /* init row contexts */
        i4_scrn_left  = pt_ctx->t_rc_scrn_vp.i4_left;
        i4_eff_line_h = wgl_hts_cnt_tool_get_line_height(pt_row);
        ui2_ci_sec_start = pt_row->ui2_ci_start;

        if(pt_row->b_is_dirty_1)
        {
            SET_RECT(
                &t_rc_scrn_row, 
                i4_scrn_left, 
                i4_scrn_prev_line_btm, 
                pt_ctx->t_rc_scrn_vp.i4_right,
                i4_scrn_prev_line_btm + i4_eff_line_h);
            
            if(RECT_IS_EFFECTIVE(t_rc_scrn_row))
            {
                WGL_RECT_COPY(&t_rc_scrn_purc_temp, &t_rc_scrn_purc);
                UNION_RECT(&t_rc_scrn_purc, &t_rc_scrn_purc_temp, &t_rc_scrn_row);
            }
        }
        else if(ui2_sid <= ui2_sid_target)
        {
            /* check each section of this row */
            if((pt_sec != NULL) && (pt_sec != pt_row->pt_sec_start))
            {
                CHECK_VALID(pt_sec->pt_next==pt_row->pt_sec_start, WGLR_INTERNAL_ERROR);
                pt_sec = pt_row->pt_sec_start;
                ui2_sid++;
            }
            i4_scrn_left += pt_row->i4_offset_x;
            while(pt_sec)
            {
                /* calculate section width */
                switch(pt_sec->e_sec_type)
                {
                    case HTMGR_SEC_TYPE_TEXT:
                        INFORM_ERR(_htscntdef_tool_get_text_sec_width(
                            pt_row,
                            pt_sec,
                            ui2_ci_sec_start,
                            i4_bound_w,
                            i4_scrn_left - pt_ctx->t_rc_scrn_vp.i4_left,
                            &i4_sec_w));
                    break;
                    
                    case HTMGR_SEC_TYPE_EOP:
                    case HTMGR_SEC_TYPE_EMPTY_TEXT:
                        i4_sec_w = 0;
                    break;
                    
                    case HTMGR_SEC_TYPE_TAB:
                        i4_sec_w = ((INT32)pt_sec->u_data.t_tab.ui2_offset);
                    break;
                    
                    case HTMGR_SEC_TYPE_IMG:
                        i4_sec_w = (INT32)pt_sec->u_data.t_img.ui2_width;
                    break;
                    
                    default:
                        CATCH_FAIL(WGLR_INTERNAL_ERROR);
                }
                
                if(ui2_sid==ui2_sid_target)
                {   /* merge section rect into the PU rect */
                    if(i4_sec_w > 0)
                    {
                        SET_RECT_BY_SIZE(
                            &t_rc_scrn_sec, 
                            i4_scrn_left, 
                            i4_scrn_prev_line_btm, 
                            i4_sec_w,
                            i4_eff_line_h);
                            
                        if(RECT_IS_EFFECTIVE(t_rc_scrn_sec))
                        {
                            WGL_RECT_COPY(&t_rc_scrn_purc_temp, &t_rc_scrn_purc);
                            UNION_RECT(&t_rc_scrn_purc, &t_rc_scrn_purc_temp, &t_rc_scrn_sec);
                        }
                    }
                }
                
                i4_scrn_left += i4_sec_w;
                
                if(   pt_sec==pt_row->pt_sec_end
                   || ui2_sid > ui2_sid_target)
                {
                    break;
                }
                
                pt_sec = pt_sec->pt_next;   
                ui2_sid++;
                ui2_ci_sec_start = 0;
            }
        }
        
        if( pt_row==pt_row_last )
        {
            if(pt_row->b_is_dirty_1)
            {
                /* add the area under the last row in the viewport */
                GL_RECT_T   t_rc_scrn_purc_residual;
                SET_RECT(&t_rc_scrn_purc_residual, pt_ctx->t_rc_scrn_vp.i4_left, i4_scrn_prev_line_btm+i4_eff_line_h, pt_ctx->t_rc_scrn_vp.i4_right, pt_ctx->t_rc_scrn_vp.i4_bottom);
                if(RECT_IS_EFFECTIVE(t_rc_scrn_purc_residual))
                {
                    WGL_RECT_COPY(&t_rc_scrn_purc_temp, &t_rc_scrn_purc);
                    UNION_RECT(&t_rc_scrn_purc, &t_rc_scrn_purc_temp, &t_rc_scrn_purc_residual);
                }
            }            
            break;
        }
        
        i4_scrn_prev_line_btm += i4_eff_line_h;
        pt_row                 = pt_row->pt_next;
    }        

    /* intersect *t_rc_scrn_purc with pt_ctx->t_rc_scrn_vp */
    INTERSECT_RECT(pt_rc_purc_out, &t_rc_scrn_purc, &pt_ctx->t_rc_scrn_vp);
    if(RECT_IS_EFFECTIVE(*pt_rc_purc_out))
    {
        MOVE_RECT(pt_rc_purc_out, -pt_ctx->t_rc_scrn_hts.i4_left, -pt_ctx->t_rc_scrn_hts.i4_top);
    }
    else
    {
        WGL_RECT_EMPTY(pt_rc_purc_out);
    }   
    
    return WGLR_OK;    
}    

static INT32 _htscntdef_cmd_hdr_scrl_up(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    INT32       i4_ret;
    UINT16      ui2_scrl_line_step = WGL_POINTER_TO_UINT16(pv_param1);
    BOOL        *pb_dirty          = (BOOL *)pv_param2;
    
    CHECK_NULL(pb_dirty, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_scrl_up(&pt_ctx->t_vp, ui2_scrl_line_step, pb_dirty));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, *pb_dirty);
    
    return WGLR_OK;    
}    

static INT32 _htscntdef_cmd_hdr_scrl_down(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    INT32       i4_ret;
    UINT16      ui2_scrl_line_step = WGL_POINTER_TO_UINT16(pv_param1);
    BOOL        *pb_dirty          = (BOOL *)pv_param2;
    
    CHECK_NULL(pb_dirty, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_scrl_down(&pt_ctx->t_vp, ui2_scrl_line_step, pb_dirty));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, *pb_dirty);
    
    return WGLR_OK;    
}    


static INT32 _htscntdef_cmd_hdr_page_up(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32       i4_ret;
    BOOL        *pb_dirty          = (BOOL *)pv_param1;
    
    CHECK_NULL(pb_dirty, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_page_up(&pt_ctx->t_vp, pb_dirty));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, *pb_dirty);
 
    return WGLR_OK;   
}    

static INT32 _htscntdef_cmd_hdr_page_down(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32       i4_ret;
    BOOL        *pb_dirty          = (BOOL *)pv_param1;
    
    CHECK_NULL(pb_dirty, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_page_down(&pt_ctx->t_vp, pb_dirty));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, *pb_dirty);
 
    return WGLR_OK;   
}    


static INT32 _htscntdef_cmd_hdr_get_page_num(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32       i4_ret;
    UINT16      *pui2_num_page = (UINT16 *)pv_param1;
    BOOL        b_vp_moved;

    CHECK_NULL(pui2_num_page, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_get_page_num(&pt_ctx->t_vp, pui2_num_page, &b_vp_moved));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
 
    return WGLR_OK;   
}    

static INT32 _htscntdef_cmd_hdr_set_vli_first(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_vli_first_new */
    OUT     VOID                        *pv_param2) /* BOOL   *pb_dirty */
{
    INT32       i4_ret;
    UINT16      ui2_vli_first_new = WGL_POINTER_TO_UINT16(pv_param1);
    BOOL        *pb_dirty         = (BOOL *)pv_param2;
    
    CHECK_NULL(pb_dirty, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_set_first(&pt_ctx->t_vp, ui2_vli_first_new, pb_dirty));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, *pb_dirty);

    return WGLR_OK;   
}        

static INT32 _htscntdef_cmd_hdr_get_vli_first(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32       i4_ret;
    UINT16      *pui2_vli_first_out = (UINT16 *)pv_param1;
    BOOL        b_vp_moved;
    
    CHECK_NULL(pui2_vli_first_out, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_get_first(&pt_ctx->t_vp, pui2_vli_first_out, &b_vp_moved));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
    
    return WGLR_OK;    
}   


static INT32 _htscntdef_cmd_hdr_get_vli_last(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    INT32       i4_ret;
    UINT16      *pui2_vli_last_out = (UINT16 *)pv_param1;
    BOOL        b_vp_moved;
    
    CHECK_NULL(pui2_vli_last_out, WGLR_INV_ARG);
    CATCH_FAIL(wgl_hts_vp_get_last(&pt_ctx->t_vp, pui2_vli_last_out, &b_vp_moved));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
    
    return WGLR_OK;    
}    


static INT32 _htscntdef_cmd_hdr_set_color(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1,/* UINT8            ui1_target */
    IN      VOID                        *pv_param2)/* WGL_COLOR_INFO_T *pt_clr_info */
{
    INT32               i4_ret, i4_ret_val;
    UINT8               ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_COLOR_INFO_T    *pt_clr_info = (WGL_COLOR_INFO_T *)pv_param2;
    
    i4_ret_val = WGLR_OK;
    if(pt_clr_info==NULL)
    { /* load default colors */
        i4_ret_val = _htscntdef_load_dft_color_set(pt_ctx, ui1_target);
    }
    else
    {
        switch(ui1_target)
        {
            case WGL_CLR_BK:
                i4_ret_val = _htscntdef_tool_get_color_set_std(&pt_ctx->t_clrs_bk, pt_clr_info);
            break;
            
            case WGL_CLR_HTS_VP_BK:
                i4_ret_val = _htscntdef_tool_get_color_set_std(&pt_ctx->t_clrs_vp_bk, pt_clr_info);
            break;
            
            default:
                INFORM_ERR(WGLR_INV_ARG);
                i4_ret_val = WGLR_INV_ARG;
        }
    }
 
    return i4_ret_val;   
}        

static INT32 _htscntdef_cmd_hdr_get_color(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT8            ui1_target */
    OUT     VOID                        *pv_param2) /* WGL_COLOR_INFO_T *pt_clr_info_out */
{
    INT32               i4_ret, i4_ret_val;
    UINT8               ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_COLOR_INFO_T    *pt_clr_info_out = (WGL_COLOR_INFO_T *)pv_param2;

    CHECK_NULL(pt_clr_info_out, WGLR_INV_ARG);
    i4_ret_val = WGLR_OK;
    switch(ui1_target)
    {
        case WGL_CLR_BK:
            _htscntdef_tool_get_color_info_std(&pt_ctx->t_clrs_bk, pt_clr_info_out);
        break;
        
        case WGL_CLR_HTS_VP_BK:
            _htscntdef_tool_get_color_info_std(&pt_ctx->t_clrs_vp_bk, pt_clr_info_out);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;
    }
    
    return i4_ret_val;    
}    

static INT32 _htscntdef_cmd_hdr_set_img(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT8           ui1_target */
    IN      VOID                        *pv_param2) /* WGL_IMG_INFO_T  *pt_img_info [can be NULL]*/
{
    INT32           i4_ret, i4_ret_val;
    UINT8           ui1_target   = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_IMG_INFO_T  *pt_img_info = (WGL_IMG_INFO_T *)pv_param2;
    
    i4_ret_val = WGLR_OK;
    
    if(pt_img_info==NULL)
    {
        switch(ui1_target)
        {
            case WGL_IMG_BK:
                pt_ctx->t_imgs_bk.t_enable       = NULL_HANDLE;
                pt_ctx->t_imgs_bk.t_disable      = NULL_HANDLE;
                pt_ctx->t_imgs_bk.t_highlight    = NULL_HANDLE;
            break;
            
            case WGL_IMG_HTS_VP_BK:
                pt_ctx->t_imgs_vp_bk.t_enable    = NULL_HANDLE;
                pt_ctx->t_imgs_vp_bk.t_disable   = NULL_HANDLE;
                pt_ctx->t_imgs_vp_bk.t_highlight = NULL_HANDLE;
            break;
            
            default:
                INFORM_ERR(WGLR_INV_ARG);
                i4_ret_val = WGLR_INV_ARG;
        }
    }
    else
    {
        switch(ui1_target)
        {
            case WGL_IMG_BK:
                i4_ret_val = _htscntdef_tool_get_img_set_std(&pt_ctx->t_imgs_bk, pt_img_info);
            break;
            
            case WGL_IMG_HTS_VP_BK:
                i4_ret_val = _htscntdef_tool_get_img_set_std(&pt_ctx->t_imgs_vp_bk, pt_img_info);
            break;
            
            default:
                INFORM_ERR(WGLR_INV_ARG);
                i4_ret_val = WGLR_INV_ARG;
        }
    }
    return i4_ret_val;   
}        

static INT32 _htscntdef_cmd_hdr_get_img(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT8           ui1_target */
    OUT     VOID                        *pv_param2) /* WGL_IMG_INFO_T  *pt_img_info_out */
{
    INT32           i4_ret, i4_ret_val;
    UINT8           ui1_target       = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_IMG_INFO_T  *pt_img_info_out = (WGL_IMG_INFO_T *)pv_param2;
    
    CHECK_NULL(pt_img_info_out, WGLR_INV_ARG);
    i4_ret_val = WGLR_OK;
    switch(ui1_target)
    {
        case WGL_IMG_BK:
            _htscntdef_tool_get_img_info_std(&pt_ctx->t_imgs_bk, pt_img_info_out);
        break;
        
        case WGL_IMG_HTS_VP_BK:
            _htscntdef_tool_get_img_info_std(&pt_ctx->t_imgs_vp_bk, pt_img_info_out);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;
    }
 
    return i4_ret_val;   
}    

static INT32 _htscntdef_cmd_hdr_reset(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx)
{
    INT32   i4_ret;
    BOOL    b_temp;
    
    CATCH_FAIL(wgl_hts_vp_sync(&pt_ctx->t_vp, &b_temp));
    return WGLR_OK;
}    

static INT32 _htscntdef_cmd_hdr_clear_all(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx)
{
    INT32   i4_ret;
    BOOL    b_temp;
    
    CATCH_FAIL(wgl_hts_vp_sync(&pt_ctx->t_vp, &b_temp));
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
                    toolkit functions
 ----------------------------------------------------------------------------*/

static INT32 _htscntdef_tool_check_valid_img(
    IN      WGL_HIMG_TPL_T              h_img)
{
    UINT32  ui4_tmp_w, ui4_tmp_h;
    return x_wgl_img_tpl_get_size(h_img, &ui4_tmp_w, &ui4_tmp_h);
} 

static INT32 _htscntdef_tool_get_color_set_std(
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs_dst, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info_src)
{
    INT32   i4_ret, i4_ret_val;

    i4_ret_val = WGLR_OK;
    switch(pt_clr_info_src->e_type) 
    {
        case WGL_COLOR_SET_BASIC:
            _COLOR_COPY( &pt_clrs_dst->t_enable, &pt_clr_info_src->u_color_data.t_basic.t_enable);
            _COLOR_COPY( &pt_clrs_dst->t_disable, &pt_clr_info_src->u_color_data.t_basic.t_disable);
            _COLOR_COPY( &pt_clrs_dst->t_highlight, &pt_clr_info_src->u_color_data.t_basic.t_enable);
            break;
        case WGL_COLOR_SET_STANDARD:
            _COLOR_COPY( &pt_clrs_dst->t_enable, &pt_clr_info_src->u_color_data.t_standard.t_enable);
            _COLOR_COPY( &pt_clrs_dst->t_disable, &pt_clr_info_src->u_color_data.t_standard.t_disable);
            _COLOR_COPY( &pt_clrs_dst->t_highlight, &pt_clr_info_src->u_color_data.t_standard.t_highlight);
            break;
        case WGL_COLOR_SET_EXTEND:
            _COLOR_COPY( &pt_clrs_dst->t_enable, &pt_clr_info_src->u_color_data.t_extend.t_enable);
            _COLOR_COPY( &pt_clrs_dst->t_disable, &pt_clr_info_src->u_color_data.t_extend.t_disable);
            _COLOR_COPY( &pt_clrs_dst->t_highlight, &pt_clr_info_src->u_color_data.t_extend.t_highlight);
            break;
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;
    };
    
    return i4_ret_val;
}

static VOID _htscntdef_tool_get_color_info_std(
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_std, 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out)
{
    pt_clr_info_out->e_type = WGL_COLOR_SET_STANDARD;
    _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_enable, &pt_clrs_std->t_enable);
    _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_disable, &pt_clrs_std->t_disable);
    _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_highlight, &pt_clrs_std->t_highlight);
    
}

static INT32 _htscntdef_tool_get_img_set_std(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_out, 
    IN      WGL_IMG_INFO_T              *pt_img_info)
{
    INT32 i4_ret, i4_ret_val;
    
    i4_ret_val = WGLR_OK;
    switch(pt_img_info->e_type) 
    {
        case WGL_IMG_SET_BASIC:
            pt_imgs_out->t_enable    = pt_img_info->u_img_data.t_basic.t_enable;
            pt_imgs_out->t_disable   = pt_img_info->u_img_data.t_basic.t_disable;
            pt_imgs_out->t_highlight = pt_img_info->u_img_data.t_basic.t_enable;
            break;
        case WGL_IMG_SET_STANDARD:
            pt_imgs_out->t_enable    = pt_img_info->u_img_data.t_standard.t_enable;
            pt_imgs_out->t_disable   = pt_img_info->u_img_data.t_standard.t_disable;
            pt_imgs_out->t_highlight = pt_img_info->u_img_data.t_standard.t_highlight;
            break;
        case WGL_IMG_SET_EXTEND:
            pt_imgs_out->t_enable    = pt_img_info->u_img_data.t_extend.t_enable;
            pt_imgs_out->t_disable   = pt_img_info->u_img_data.t_extend.t_disable;
            pt_imgs_out->t_highlight = pt_img_info->u_img_data.t_extend.t_highlight;
            break;
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;
    }    
    
    return i4_ret_val;
}

static VOID _htscntdef_tool_get_img_info_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
    OUT     WGL_IMG_INFO_T              *pt_img_info_out)
{
    pt_img_info_out->e_type                            = WGL_IMG_SET_STANDARD;
    pt_img_info_out->u_img_data.t_standard.t_enable    = pt_imgs_std->t_enable;
    pt_img_info_out->u_img_data.t_standard.t_disable   = pt_imgs_std->t_disable;
    pt_img_info_out->u_img_data.t_standard.t_highlight = pt_imgs_std->t_highlight;
}    
    

static INT32 _htscntdef_tool_paint(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_update) /* can be NULL */
{
    INT32                   i4_ret;
    UINT16                  ui2_first, ui2_last, ui2_cur_size;
    UINT16                  ui2_i, ui2_num_row, ui2_eff_last;
    WGL_HTMGR_ROW_CACHE_T   *pt_row;
    INT32                   i4_scrn_prev_line_btm, i4_scrn_g_top, i4_scrn_g_left;
    INT32                   i4_g_cnt_top, i4_g_cnt_btm;
    INT32                   i4_line_inset_top, i4_line_inset_btm;
    INT32                   i4_parag_gap_top, i4_parag_gap_btm;
    INT32                   i4_line_gap_top, i4_line_gap_btm;
    INT32                   i4_eff_line_h;
    GL_RECT_T               t_rc_scrn_clip, t_rc_scrn_clip_temp;
    GL_RECT_T               t_rc_scrn_update;
    GL_RECT_T               t_rc_g_update;
    BOOL                    b_vp_moved;
    
    /* fill content background */
    INFORM_ERR(_htscntdef_tool_fill_bk(pt_ctx, h_g, ui2_state_paint));
        
    /* fill vp background */
    INFORM_ERR(_htscntdef_tool_fill_vp_bk(pt_ctx, h_g, ui2_state_paint));
    
    /* render lines */
    if(pt_rc_update)
    {
        WGL_RECT_COPY(&t_rc_scrn_update, pt_rc_update);
        MOVE_RECT(&t_rc_scrn_update, pt_ctx->t_rc_scrn_hts.i4_left, pt_ctx->t_rc_scrn_hts.i4_top);
    }        
    else
    {
        WGL_RECT_COPY(&t_rc_scrn_update, &pt_ctx->t_rc_scrn_hts);
    }    
    i4_scrn_prev_line_btm = pt_ctx->t_rc_scrn_vp.i4_top;
    i4_scrn_g_left        = pt_ctx->t_rc_scrn_vp.i4_left;
    
    CATCH_FAIL(wgl_hts_vp_get_state(&pt_ctx->t_vp, &ui2_first, &ui2_last, &ui2_cur_size, &b_vp_moved));
    _SET_BOOL_BY_OR(pt_ctx->b_vp_moved, b_vp_moved);
    
    /* render the last half-visible line if necessary*/    
    ui2_eff_last = ui2_last;
    ui2_num_row  = HTMGR_GET_ROW_NUM(pt_ctx->pt_htmgr);
    if(   HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_HTS_SHOW_HALF_VISIBLE_LINE)
       && ((INT32)ui2_cur_size) < RECT_H(&pt_ctx->t_rc_scrn_vp)
       && ui2_last + (UINT16)1 < ui2_num_row)
    {
        ui2_eff_last++;
    }

    CATCH_FAIL(wgl_htmgr_get_row_info(pt_ctx->pt_htmgr, ui2_first, &pt_row));
    for(ui2_i=ui2_first; ui2_i<=ui2_eff_last && pt_row; ui2_i++)
    {
        if( i4_scrn_prev_line_btm > pt_ctx->t_rc_scrn_vp.i4_bottom)
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            break;
        }
        
    ___BLOCK_BEGIN___
        
        /* get row metrics */
        i4_ret = _htscntdef_tool_get_line_inset(pt_row, &i4_line_inset_top, &i4_line_inset_btm);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            i4_line_inset_top = i4_line_inset_btm = 0;
        }
        
        i4_ret = _htscntdef_tool_get_parag_gap(pt_row, &i4_parag_gap_top, &i4_parag_gap_btm);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            i4_parag_gap_top = i4_parag_gap_btm = 0;
        }
        
        i4_ret = _htscntdef_tool_get_line_gap(pt_row, &i4_line_gap_top, &i4_line_gap_btm);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            i4_line_gap_top = i4_line_gap_btm = 0;
        }
        
        /* quick skip for partial update */
        i4_eff_line_h = _GET_ROW_EFF_H(pt_row) + i4_line_gap_top + i4_line_gap_btm + i4_parag_gap_top + i4_parag_gap_btm;
        if(i4_scrn_prev_line_btm + i4_eff_line_h <= t_rc_scrn_update.i4_top )
        {
            GOTO_EOB;
        }
        
        /* setup row metrics */
        i4_scrn_g_top = i4_scrn_prev_line_btm - i4_line_inset_top + i4_parag_gap_top + i4_line_gap_top;
        i4_g_cnt_top  = i4_line_inset_top + i4_parag_gap_top + i4_line_gap_top;
        i4_g_cnt_btm  = i4_g_cnt_top + _GET_ROW_EFF_H(pt_row);
        
        /* setup clipping rect */
        SET_RECT(
            &t_rc_scrn_clip_temp,
            pt_ctx->t_rc_scrn_vp.i4_left, 
            i4_scrn_prev_line_btm,
            pt_ctx->t_rc_scrn_vp.i4_right,
            i4_scrn_prev_line_btm + i4_eff_line_h);
        if(!RECT_IS_EFFECTIVE(t_rc_scrn_clip_temp))
        {
            GOTO_EOB;
        }
        INTERSECT_RECT(&t_rc_scrn_clip, &t_rc_scrn_clip_temp, &pt_ctx->t_rc_scrn_vp);
        if(!RECT_IS_EFFECTIVE(t_rc_scrn_clip))
        {
            GOTO_EOB;
        }
        
        /* partial update */
        if(   pt_rc_update 
           && !_htscntdef_tool_is_rect_intersect(&t_rc_scrn_clip, &t_rc_scrn_update))
        {
            GOTO_EOB;
        }
        
        GOTO_EOB_AT_FAIL(x_gl_translate(h_g, i4_scrn_g_left, i4_scrn_g_top), i4_ret);
        GOTO_EOB_AT_FAIL(x_gl_set_clip(h_g, &t_rc_scrn_clip), i4_ret);
        _RECT_MOVE(&t_rc_scrn_update, -i4_scrn_g_left, -i4_scrn_g_top, &t_rc_g_update);
        
        INFORM_ERR(_htscntdef_tool_render_line(
            pt_ctx, 
            h_g, 
            &t_rc_g_update,
            pt_row, 
            t_rc_scrn_update.i4_left - i4_scrn_g_left,
            t_rc_scrn_update.i4_right - i4_scrn_g_left,
            0, 
            0, 
            RECT_W(&pt_ctx->t_rc_scrn_vp),
            i4_g_cnt_top,
            i4_g_cnt_btm));

    ___BLOCK_END___
        
        i4_scrn_prev_line_btm += i4_eff_line_h;
        pt_row->b_is_dirty_1   = FALSE;
        pt_row                 = pt_row->pt_next;
    }
    
    INFORM_ERR(x_gl_translate(h_g, pt_ctx->t_rc_scrn_hts.i4_left, pt_ctx->t_rc_scrn_hts.i4_top));
    INFORM_ERR(x_gl_set_clip(h_g, NULL));
    
    /* render page indicators */
    INFORM_ERR(_htscntdef_tool_draw_page_indicator_images(
        h_g,
        &pt_ctx->t_cvsst,
        &pt_ctx->t_page_indc,
        ui2_first > 0 ? TRUE : FALSE,
        ui2_last + (UINT16)1 < ui2_num_row ? TRUE : FALSE,
        &pt_ctx->t_rc_update));
    
    return WGLR_OK;
}        


static INT32 _htscntdef_tool_render_line(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      GL_RECT_T                   *pt_rc_g_update,
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      INT32                       i4_pu_left,  
    IN      INT32                       i4_pu_right, 
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_bound_w, 
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm)
{
    WGL_HTMGR_SEC_INFO_T    *pt_sec          = NULL;
    INT32                   i4_sec_w         = 0;
    UINT16                  ui2_ci_sec_start = 0;
    INT32                   i4_ret           = WGLR_OK;
    INT32                   i4_line_base;

    i4_left         += pt_row->i4_offset_x;
    ui2_ci_sec_start = pt_row->ui2_ci_start;
    pt_sec           = pt_row->pt_sec_start;
    i4_line_base     = ((INT32)pt_row->ui2_height) - ((INT32)pt_row->ui2_descend);
    i4_line_base     = MAX(i4_line_base, 0);
    
    while(pt_sec)
    {
        switch(pt_sec->e_sec_type)
        {
            case HTMGR_SEC_TYPE_TEXT:
            {
                INFORM_ERR(_htscntdef_tool_get_text_sec_width(
                    pt_row,
                    pt_sec,
                    ui2_ci_sec_start,
                    i4_bound_w,
                    i4_left,
                    &i4_sec_w));
                
                if(_RANGE_IS_INTERSECT(i4_left, i4_left+i4_sec_w, i4_pu_left, i4_pu_right))
                {
                    INFORM_ERR(_htscntdef_tool_render_text_sec(
                        pt_ctx,
                        h_g,
                        pt_row,
                        pt_sec,
                        ui2_ci_sec_start,
                        i4_left,
                        i4_top,
                        i4_cnt_top,
                        i4_cnt_btm,
                        i4_line_base));
                }
                i4_left += i4_sec_w;
            }
            break;
            
            case HTMGR_SEC_TYPE_EOP:
                 CHECK_VALID(pt_sec==pt_row->pt_sec_end, WGLR_INTERNAL_ERROR);
                 /* do nothing */
            break;
            
            case HTMGR_SEC_TYPE_TAB:
                i4_left += ((INT32)pt_sec->u_data.t_tab.ui2_offset);
            break;
            
            case HTMGR_SEC_TYPE_EMPTY_TEXT:
                /* do nothing */
            break;
            
            case HTMGR_SEC_TYPE_IMG:
            {   
                i4_sec_w = (INT32)pt_sec->u_data.t_img.ui2_width;
                if(_RANGE_IS_INTERSECT(i4_left, i4_left+i4_sec_w, i4_pu_left, i4_pu_right))
                {                
                    INFORM_ERR(_htscntdef_tool_render_img_sec(
                        h_g, 
                        &pt_ctx->t_cvsst,
                        pt_rc_g_update, 
                        pt_sec, 
                        i4_left, 
                        i4_cnt_top, 
                        i4_cnt_btm,
                        i4_line_base));
                }
                i4_left += i4_sec_w;
            }
            break;
            
            default:
                INFORM_ERR(WGLR_INTERNAL_ERROR);
        }
        
        if(pt_sec==pt_row->pt_sec_end
           || i4_left >= i4_bound_w)
        {
            break;
        }            
        ui2_ci_sec_start = 0;
        pt_sec           = pt_sec->pt_next;
    }        
    
    
    return WGLR_OK;
}

static INT32 _htscntdef_tool_render_text_sec(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_text,
    IN      UINT16                      ui2_ci_sec_start,
    IN      INT32                       i4_left,
    IN      INT32                       i4_top,
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm,
    IN      INT32                       i4_line_base) 
{
    UTF16_T                             *w2s_text;
    INT32                               i4_ret;
    UINT16                              ui2_text_len;
    INT32                               i4_sec_top, i4_sec_inset_top;
    WGL_MSFS_FONT_REG_INFO_T            *pt_font_reg;
    GL_COLOR_T                          t_clr_transp;
    GL_COLOR_T                          *pt_clr_text, *pt_clr_bk;
            
    /* init transparent color */
    wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_transp);
            
    /* prepare section text */
    w2s_text = wgl_htmgr_get_text(pt_ctx->pt_htmgr, pt_sec_text, ui2_ci_sec_start);
    if(pt_sec_text==pt_row->pt_sec_end)
    {
        ui2_text_len = pt_row->ui2_ci_end - ui2_ci_sec_start + (UINT16)1;
    }
    else
    {
        ui2_text_len = TBM_RANGE_GET_SIZE(pt_sec_text->t_range) - ui2_ci_sec_start;
    }
    
    /* calcuate section position */
    i4_sec_inset_top  = 0;
    CATCH_FAIL(wgl_msfs_get_font_reg_info(pt_sec_text->u_data.t_text.t_attr.h_font, &pt_font_reg));
    if(pt_font_reg)
    {
        i4_sec_inset_top = (INT32)(pt_font_reg->i2_row_inset_top);
    }
    
    INFORM_ERR(_htscntdef_tool_calc_text_sec_top(
        pt_sec_text->u_data.t_text.t_attr.e_v_anchor,
        i4_cnt_top,
        i4_cnt_btm,
        i4_line_base,
        pt_sec_text->u_data.t_text.t_attr.h_font,
        i4_sec_inset_top,
        (INT32)(pt_sec_text->u_data.t_text.ui2_height),
        (INT32)(pt_sec_text->u_data.t_text.ui2_eff_height),
        &i4_sec_top));
 
    pt_clr_bk   = &pt_sec_text->u_data.t_text.t_attr.t_clr_bk;
    pt_clr_text = &pt_sec_text->u_data.t_text.t_attr.t_clr_text;
    if( wgl_cvsst_check_palette_mode(&pt_ctx->t_cvsst)      &&
        wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_text) )
    {
        pt_clr_text = pt_clr_bk;
    }
    
    /* render text */
    INFORM_ERR(x_gl_draw_ustring_ex(
        h_g,
        i4_left,
        i4_sec_top,
        i4_cnt_top,
        w2s_text,
        ui2_text_len,
        NULL,
        0,
        pt_sec_text->u_data.t_text.t_attr.h_font,
        pt_clr_text,
        pt_clr_bk,
        &t_clr_transp,
        FALSE,
        0,
        0,
        (UINT16)(i4_cnt_btm - i4_cnt_top),
        0));

    return WGLR_OK;
    
}   

static INT32 _htscntdef_tool_get_text_sec_width( 
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_text,
    IN      UINT16                      ui2_ci_sec_start,
    IN      INT32                       i4_bound_w,
    IN      INT32                       i4_left,
    OUT     INT32                       *pi4_sec_w)
{
    INT32       i4_ret;
    
    if(pt_sec_text==pt_row->pt_sec_start)
    {
        *pi4_sec_w = (INT32)pt_row->ui2_width_start_sec;
    }
    else if(pt_sec_text==pt_row->pt_sec_end)
    {
        *pi4_sec_w = i4_bound_w - i4_left;
    }
    else
    {
        if(!(pt_sec_text->u_data.t_text.ui2_width>0))
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
        }
        *pi4_sec_w = (INT32)pt_sec_text->u_data.t_text.ui2_width;
    }
    
    return WGLR_OK;
}    

static INT32 _htscntdef_tool_render_img_sec(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      GL_RECT_T                   *pt_rc_g_update,
    IN      WGL_HTMGR_SEC_INFO_T        *pt_sec_img,
    IN      INT32                       i4_left,
    IN      INT32                       i4_cnt_top,
    IN      INT32                       i4_cnt_btm,
    IN      INT32                       i4_line_base)
{
    GL_RECT_T       t_rc_img;
    INT32           i4_img_w, i4_img_h, i4_sec_top;
    INT32           i4_ret;
    
    /* check for quick return */
    if( pt_sec_img->u_data.t_img.t_img.h_img == NULL_HANDLE 
        || pt_sec_img->u_data.t_img.ui2_width == 0 
        || pt_sec_img->u_data.t_img.ui2_height == 0)
    {
        return WGLR_OK;
    }
    
    i4_img_w = ((INT32)pt_sec_img->u_data.t_img.ui2_width) - ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_left) - ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_right);
    i4_img_h = ((INT32)pt_sec_img->u_data.t_img.ui2_height) - ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_top) - ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_bottom);
    
    /* check for quick return */
    if(i4_img_w <= 0 || i4_img_h <= 0 )
    {
        return WGLR_OK;
    } 
    
    INFORM_ERR(_htscntdef_tool_calc_img_sec_top(
        pt_sec_img->u_data.t_img.t_img.e_v_anchor,
        i4_cnt_top,
        i4_cnt_btm,
        i4_line_base,
        (INT32)pt_sec_img->u_data.t_img.ui2_height,
        &i4_sec_top
    ));
    
    SET_RECT_BY_SIZE(
        &t_rc_img, 
        i4_left + ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_left),
        i4_sec_top + ((INT32)pt_sec_img->u_data.t_img.t_img.ui2_gap_top),
        i4_img_w,
        i4_img_h);
    
    INFORM_ERR(wgl_img_tpl_paint(
        pt_sec_img->u_data.t_img.t_img.h_img, 
        &t_rc_img,
        pt_rc_g_update,
        h_g,
        pt_cvsst,
        WGL_TPL_PAINT_COMPOSE));
    
    return WGLR_OK;
}    

static INT32 _htscntdef_tool_draw_page_indicator_images(
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_HTS_PAGE_INDC_INFO_T    *pt_page_imgs,
    IN      BOOL                        b_is_up_on,
    IN      BOOL                        b_is_down_on,
    IN      GL_RECT_T                   *pt_rc_update /* could be NULL */)
{
    WGL_HIMG_TPL_T      h_imgs[2];
    GL_POINT_T          *pt_locs[2];
    GL_RECT_T           t_rc_img;
    UINT32              ui4_img_w, ui4_img_h;
    INT32               i4_ret;
    UINT8               ui1_i;

    h_imgs[0] = b_is_up_on ? 
            pt_page_imgs->h_img_up_on : pt_page_imgs->h_img_up_off;
    
    h_imgs[1] = b_is_down_on ? 
            pt_page_imgs->h_img_down_on : pt_page_imgs->h_img_down_off;

    pt_locs[0] = &pt_page_imgs->t_location_up;
    pt_locs[1] = &pt_page_imgs->t_location_down;
    
    for(ui1_i=0; ui1_i<2; ui1_i++)
    {
        if( h_imgs[ui1_i]!=NULL_HANDLE )
        {
            i4_ret = x_wgl_img_tpl_get_size(h_imgs[ui1_i], &ui4_img_w, &ui4_img_h);
            if(!WGL_IS_FAIL(i4_ret))
            {
                SET_RECT_BY_SIZE(   
                    &t_rc_img, 
                    pt_locs[ui1_i]->i4_x, 
                    pt_locs[ui1_i]->i4_y, 
                    (INT32)ui4_img_w,
                    (INT32)ui4_img_h);
                                
                if(   pt_rc_update
                   && !_htscntdef_tool_is_rect_intersect(&t_rc_img, pt_rc_update))
                {
                    continue;
                }

                INFORM_ERR(wgl_img_tpl_paint(h_imgs[ui1_i], &t_rc_img, pt_rc_update, h_g, pt_cvsst, WGL_TPL_PAINT_COMPOSE) );
            }
            else
            {
                INFORM_ERR(i4_ret);
            }
        }
    }

    return WGLR_OK;
}

static BOOL _htscntdef_tool_is_rect_intersect(
    IN      GL_RECT_T                   *pt_rc_1,
    IN      GL_RECT_T                   *pt_rc_2)
{
    INT32   i4_ret;
    BOOL    b_is_intersected;
    
    b_is_intersected = FALSE;
    INFORM_ERR(x_wgl_rect_is_cross(&b_is_intersected, pt_rc_1, pt_rc_2));
    
    return b_is_intersected;
}

static INT32 _htscntdef_tool_fill_bk(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint)
{
    INT32       i4_ret;
    GL_RECT_T   t_rc_cnt;
    
    SET_RECT_BY_SIZE(&t_rc_cnt, pt_ctx->t_inset_bdr.i4_left, pt_ctx->t_inset_bdr.i4_top, RECT_W(&pt_ctx->t_rc_scrn_cnt), RECT_H(&pt_ctx->t_rc_scrn_cnt));
    
    if(HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_GL_NO_BK))
    {
    }
    else if(HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_GL_NO_IMG_UI))
    {
        GL_COLOR_T  *pt_clr_bk;
        
        /* GRAPHIC_UI */
        _htscntdef_tool_select_clr_std(
            &pt_ctx->t_clrs_bk,
            ui2_state_paint,
            &pt_clr_bk);

        if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_bk))
        {
            return WGLR_OK;
        }
        
        INFORM_ERR(x_gl_set_color(h_g, pt_clr_bk));
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_cnt));
    }
    else
    {
        /* IMG_UI */
        HANDLE_T    h_img_bk;
        
        _htscntdef_tool_select_img_std(
            &pt_ctx->t_imgs_bk,
            ui2_state_paint,
            &h_img_bk);        
        
        if(h_img_bk != NULL_HANDLE)
        {
            INFORM_ERR(wgl_img_tpl_paint(
                h_img_bk, 
                &t_rc_cnt,
                &pt_ctx->t_rc_update,
                h_g,
                &pt_ctx->t_cvsst,
                HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_GL_BDR_FILL_CNT_BK) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT));
        }
    }
    
    return WGLR_OK;    
}         

static INT32 _htscntdef_tool_fill_vp_bk(
    IN      HTSCNTDEF_CTX_T             *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint)
{
    INT32               i4_ret;
    GL_COLOR_T          *pt_clr_vp_bk;
    GL_RECT_T           t_rc_vp;
    WGL_HIMG_TPL_T      h_img_vp_bk;
    INT32               i4_scrn_g_x, i4_scrn_g_y;
    
    i4_scrn_g_x = pt_ctx->t_rc_scrn_vp.i4_left;
    i4_scrn_g_y = pt_ctx->t_rc_scrn_vp.i4_top;
    CATCH_FAIL(x_gl_translate(h_g, i4_scrn_g_x, i4_scrn_g_y));
    
    if(HAS_FLAG(pt_ctx->ui4_styles, WGL_STL_HTS_VP_NO_BK))
    {
        return WGLR_OK;
    }
    
    SET_RECT(&t_rc_vp, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_vp), RECT_H(&pt_ctx->t_rc_scrn_vp));
    
    _htscntdef_tool_select_img_std(
        &pt_ctx->t_imgs_vp_bk,
        ui2_state_paint,
        &h_img_vp_bk);
        
    if(h_img_vp_bk != NULL_HANDLE)
    {
        GL_RECT_T       t_rc_g_update;
        
        _RECT_MOVE(&pt_ctx->t_rc_update, 
                   -(i4_scrn_g_x - pt_ctx->t_rc_scrn_hts.i4_left),
                   -(i4_scrn_g_y - pt_ctx->t_rc_scrn_hts.i4_top),
                   &t_rc_g_update);
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_vp_bk, 
            &t_rc_vp,
            &t_rc_g_update,
            h_g,
            &pt_ctx->t_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else
    {
        _htscntdef_tool_select_clr_std(
            &pt_ctx->t_clrs_vp_bk,
            ui2_state_paint,
            &pt_clr_vp_bk);

        if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_vp_bk))
        {
            return WGLR_OK;
        }
        
        if(GLR_OK == (i4_ret=x_gl_set_color(h_g, pt_clr_vp_bk)))
        {
            INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_vp));
        }
        else
        {
            INFORM_ERR(i4_ret);
        }            
    }
        
    return WGLR_OK;
}    

static VOID _htscntdef_tool_select_clr_std(
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_std,
    IN      UINT16                      ui2_state_paint,
    OUT     GL_COLOR_T                  **ppt_clr_out)
{
    if(!IS_STATE_ENABLED(ui2_state_paint))
    {
        *ppt_clr_out = &pt_clrs_std->t_disable;
    }
    else if(   IS_STATE_FOCUSED(ui2_state_paint) 
            && IS_STATE_ACTIVATED(ui2_state_paint))
    {
        *ppt_clr_out = &pt_clrs_std->t_highlight;
    }
    else
    {
        *ppt_clr_out = &pt_clrs_std->t_enable;
    }    
}
        
static VOID _htscntdef_tool_select_img_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
    IN      UINT16                      ui2_state_paint,
    OUT     WGL_HIMG_TPL_T              *ph_img_out)
{
    if(!IS_STATE_ENABLED(ui2_state_paint) )
    {
        *ph_img_out = pt_imgs_std->t_disable;
    }
    else if(   IS_STATE_FOCUSED(ui2_state_paint) 
            && IS_STATE_ACTIVATED(ui2_state_paint))
    {
        *ph_img_out = pt_imgs_std->t_highlight;
    }
    else
    {
        *ph_img_out = pt_imgs_std->t_enable;
    }    
}    

static INT32 _htscntdef_tool_get_line_inset(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_line_inset_top,
    OUT     INT32                       *pi4_line_inset_bottom)
{
    INT32           i4_ret;
    
    *pi4_line_inset_top    = 0;
    *pi4_line_inset_bottom = 0;
    switch(pt_row->e_highest_obj_type)
    {
        case HTMGR_ROW_HIGHEST_OBJ_TYPE_FONT:
        {
            WGL_MSFS_FONT_REG_INFO_T *pt_font_reg;
            
            CATCH_FAIL(wgl_msfs_get_font_reg_info(pt_row->u_highest.h_font_highest, &pt_font_reg));
            if(pt_font_reg)
            {
                *pi4_line_inset_top    = (INT32)pt_font_reg->i2_row_inset_top;
                *pi4_line_inset_bottom = (INT32)pt_font_reg->i2_row_inset_bottom;
            }
        }
        break;
        
        case HTMGR_ROW_HIGHEST_OBJ_TYPE_IMG:
        {
            /* do nothing, since the row content height is exactly 
               the height (ui2_height) of the heightest image section */
        }
        break;
        
        default:
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return WGLR_INTERNAL_ERROR;
    }    
    
    return WGLR_OK;
}        

static INT32 _htscntdef_tool_get_parag_gap(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_parag_gap_top,
    OUT     INT32                       *pi4_parag_gap_bottom)
{
    *pi4_parag_gap_top = WGL_HTMGR_IS_ROW_START_OF_PARAG(pt_row) ? (INT32)pt_row->pt_parag->t_gap.ui2_parag_gap_top : 0;
    *pi4_parag_gap_bottom = WGL_HTMGR_IS_ROW_END_OF_PARAG(pt_row) ? (INT32)pt_row->pt_parag->t_gap.ui2_parag_gap_bottom : 0;
    
    return WGLR_OK;
}        

static INT32 _htscntdef_tool_get_line_gap(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row,
    OUT     INT32                       *pi4_line_inset_top,
    OUT     INT32                       *pi4_line_inset_bottom)
{
    *pi4_line_inset_top    = pt_row->pt_parag->t_gap.i4_line_gap_top;
    *pi4_line_inset_bottom = pt_row->pt_parag->t_gap.i4_line_gap_bottom;

    return WGLR_OK;    
}        

static INT32 _htscntdef_tool_calc_text_sec_top(
    IN      WGL_HTMGR_V_ANCHOR_T        e_v_anchor,
    IN      INT32                       i4_line_top,
    IN      INT32                       i4_line_bottom,
    IN      INT32                       i4_line_base,
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_sec_inset_top,
    IN      INT32                       i4_sec_h,
    IN      INT32                       i4_sec_eff_h,
    OUT     INT32                       *pi4_sec_top)
{
    INT32   i4_ret, i4_ret_val;
    INT16   i2_descend;
    
    i4_ret_val = WGLR_OK;
    switch(e_v_anchor)
    {
        case HTMGR_V_ANCHOR_BOTTOM:
            /* calculate *pi4_sec_top later.. */
        break;
        
        case HTMGR_V_ANCHOR_CENTER:
            *pi4_sec_top = (i4_line_top + (((i4_line_bottom - i4_line_top)-i4_sec_eff_h)>>1)) - i4_sec_inset_top;
        return WGLR_OK;

        case HTMGR_V_ANCHOR_TOP:
            *pi4_sec_top = i4_line_top - i4_sec_inset_top;
        return WGLR_OK;
        
        default:
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            i4_ret_val = WGLR_INTERNAL_ERROR;
    }         
    
    /* e_v_anchor==HTMGR_V_ANCHOR_BOTTOM */
    i4_ret = x_fe_get_descender(h_font, &i2_descend);
    if(i4_ret != FER_OK)
    {
        INFORM_ERR(i4_ret);
        i2_descend = 0;
    }
    
    *pi4_sec_top = i4_line_base - (i4_sec_h + ((INT32)i2_descend) - 1);

    
    return i4_ret_val;   

}

static INT32 _htscntdef_tool_calc_img_sec_top(
    IN      WGL_HTMGR_V_ANCHOR_T        e_v_anchor,
    IN      INT32                       i4_line_top,
    IN      INT32                       i4_line_bottom,
    IN      INT32                       i4_line_base,
    IN      INT32                       i4_img_sec_eff_h,
    OUT     INT32                       *pi4_sec_top)
{
    INT32   i4_ret;
    
    switch(e_v_anchor)
    {
        case HTMGR_V_ANCHOR_BOTTOM:
            *pi4_sec_top = i4_line_base - i4_img_sec_eff_h;
        break;
        
        case HTMGR_V_ANCHOR_CENTER:
            *pi4_sec_top = i4_line_top + (((i4_line_bottom - i4_line_top) - i4_img_sec_eff_h)>>1);
        break;

        case HTMGR_V_ANCHOR_TOP:
            *pi4_sec_top = i4_line_top;
        break;
        
        default:
            *pi4_sec_top = i4_line_base - i4_img_sec_eff_h;
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return WGLR_INTERNAL_ERROR;
    }        
    
    return WGLR_OK;
}   

static INT32 _htscntdef_load_dft_color_set(
    IN OUT  HTSCNTDEF_CTX_T             *pt_ctx,
    IN      UINT8                       ui1_target)
{   
    INT32       i4_ret, i4_ret_val;     
    
    i4_ret_val = WGLR_OK;
    switch(ui1_target)
    {
        case WGL_CLR_BK:
            wgl_get_system_color(WGL_SYS_CLR_HTS_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_enable);
            wgl_get_system_color(WGL_SYS_CLR_HTS_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_disable);
            wgl_get_system_color(WGL_SYS_CLR_HTS_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_highlight);
        break;
        
        case WGL_CLR_HTS_VP_BK:
            wgl_get_system_color(WGL_SYS_CLR_HTS_VP, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_vp_bk.t_enable);
            wgl_get_system_color(WGL_SYS_CLR_HTS_VP_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_vp_bk.t_disable);
            wgl_get_system_color(WGL_SYS_CLR_HTS_VP_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_vp_bk.t_highlight);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;
    }
    
    return i4_ret_val;
}

