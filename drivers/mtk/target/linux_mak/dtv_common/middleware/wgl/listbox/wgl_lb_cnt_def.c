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
 * $RCSfile: wgl_lb_cnt_def.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/7 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: f615a228487dbf815141dc7f8fdacc09 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "dbg/def_dbg_level_mw.h"
#include "unicode/x_uc_str.h"
#include "font/x_fe.h"

#include "wgl/u_wgl_common.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_image.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/wgl_bdr_tools.h"

#include "wgl/listbox/wgl_lb_cnt_internal.h"
#include "wgl/listbox/u_wgl_lb_cnt_common.h"
#include "wgl/listbox/wgl_lb_cnt_def.h"
#include "wgl/listbox/wgl_lb_dbg.h"
#include "wgl/listbox/wgl_lb_sb.h"
#include "wgl/arabic.h"
#include "wgl/listbox/wgl_lb_util.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE    wgl_lb_cli_get_eff_dbg_level()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME     "LBCNT"

/* return values for internal use */
#define LBCNTDEFR_OK            ((INT32)0)
#define LBCNTDEFR_INV_INSET     ((INT32)-10)

/* item attributes index */
#define ITEM_ATTR_ALIGN                0
#define ITEM_ATTR_BK_COLOR             1
#define ITEM_ATTR_TEXT_COLOR           2
#define ITEM_ATTR_FONT                 3
#define ITEM_ATTR_BK_IMAGE             4
#define ITEM_ATTR_FONT_EX              5

/* ui1_xy_sel : _lbcntdef_cal_obj_pos() & _lbcntdef_cal_item_obj_pos() */
#define XY_SEL_X            MAKE_BIT_MASK_8(0)
#define XY_SEL_Y            MAKE_BIT_MASK_8(1)

#ifdef WGL_LB_SUPPORT_MULTI_LINE
typedef struct _LBCNTDEF_LINE_INF_T
{
    UTF16_T*                    pw2s_text;
    UINT32                      ui4_length;
    INT32                       i4_w_eff;
    INT32                       i4_h_eff;
    INT32                       i4_w_body;
    INT32                       i4_h_body;
    INT32                       i4_w_etc;
    INT32                       i4_h_etc;
    BOOL                        b_need_smart_cut;
    BOOL                        b_need_tail_soft_hyphen;
} LBCNTDEF_LINE_INF_T;
#endif

#define PSUEDO_PAINT_STATE_FOR_HLT_ELEM_AREA    \
        (WGL_STATE_ENABLED | WGL_STATE_ACTIVATED | WGL_STATE_FOCUSED)

/* macros */
#define _LBE_COLOR_SET_COPY(_dst, _src)                                             \
    x_memcpy((VOID *)(_dst), (VOID *)(_src), sizeof(WGL_LB_COLOR_ELEM_SET_T))

#define _LBE_COLOR_SET_CLEAR(_pt_clrs)                                              \
    x_memset((VOID *) (_pt_clrs), 0, sizeof(WGL_LB_COLOR_ELEM_SET_T) )

#define _LBE_IMG_SET_COPY(_dst, _src)                                               \
    x_memcpy((VOID *)(_dst), (VOID *)(_src), sizeof(WGL_LB_IMG_ELEM_SET_T))

#define _LBE_IMG_SET_CLEAR(_pt_imgs)                                                \
    x_memset((VOID *) (_pt_imgs), 0, sizeof(WGL_LB_IMG_ELEM_SET_T))

#define _SCR_IMG_INFO_CLEAR(_t)                                                     \
    (_t).t_location_up.i4_x = (_t).t_location_up.i4_y                               \
        = (_t).t_location_down.i4_x = (_t).t_location_down.i4_y = 0;                \
    (_t).h_img_scr_up_on                                                            \
        = (_t).h_img_scr_up_off                                                     \
        = (_t).h_img_scr_down_on                                                    \
        = (_t).h_img_scr_down_off = NULL_HANDLE;        

#define _SCR_IMG_INFO_COPY(_dst, _src)                                              \
    x_memcpy((VOID *)(_dst), (VOID *)(_src), sizeof(WGL_LB_SCROLL_IMG_INFO_T))

#define _RANGE_IS_INTERSECT(_i4_start_r1, _i4_end_r1, _i4_start_r2, _i4_end_r2)     \
    (!((_i4_end_r1)<(_i4_start_r2) || (_i4_start_r1)>(_i4_end_r2)))

#define _IS_PG_IMG_UP_ON(_pt_ctx, _ui2_first, _ui2_last)                            \
    (!HAS_FLAG((_pt_ctx)->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || ((_ui2_first) > 0)

#define _IS_PG_IMG_DOWN_ON(_pt_ctx, _ui2_first, _ui2_last)                          \
    (!HAS_FLAG((_pt_ctx)->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || ((_ui2_last)+1 < (_pt_ctx)->ui2_elem_num)

#define _RECT_MOVE(_pt_rc_src, _i4_dx, _i4_dy, _pt_rc_out)                          \
    {                                                                               \
        (_pt_rc_out)->i4_left   = (_pt_rc_src)->i4_left + (_i4_dx);                 \
        (_pt_rc_out)->i4_top    = (_pt_rc_src)->i4_top + (_i4_dy);                  \
        (_pt_rc_out)->i4_right  = (_pt_rc_src)->i4_right + (_i4_dx);                \
        (_pt_rc_out)->i4_bottom = (_pt_rc_src)->i4_bottom + (_i4_dy);               \
    }

#define _RECT_AREA(_pt_rc)                                                          \
    (RECT_W(_pt_rc) * RECT_H(_pt_rc))

#define _WIDGET_STATE_TO_PAINT_STATE(_ui4_state_wgt)                                \
    ((UINT16) (_ui4_state_wgt))

/* shrink the global rect to be the same location and size of its local rect */
#define _RECT_SHRINK_BY_RECT(_rb, _rl)                                              \
    {                                                                               \
        (_rb).i4_left += (_rl).i4_left;                                             \
        (_rb).i4_top += (_rl).i4_top;                                               \
        (_rb).i4_right -= (((_rb).i4_right-(_rb).i4_left)-(_rl).i4_right);          \
        (_rb).i4_bottom -= (((_rb).i4_bottom-(_rb).i4_top)-(_rl).i4_bottom );       \
    }

#define _LBCNTDEF_SET_CLIP_RECT(_h_g, _t_rc_scrn_orig, _t_rc_scrn_clip, _i4_ret)    \
    (_i4_ret) = x_gl_set_clip((_h_g), &(_t_rc_scrn_clip));                          \
    if( WGL_IS_FAIL(_i4_ret))                                                       \
    {                                                                               \
        INFORM_ERR(_i4_ret);                                                        \
        break;                                                                      \
    }                                                                               \
    (_i4_ret) = x_gl_translate((_h_g), (_t_rc_scrn_orig).i4_left, (_t_rc_scrn_orig).i4_top); \
    if( WGL_IS_FAIL(_i4_ret))                                                       \
    {                                                                               \
        INFORM_ERR(_i4_ret);                                                        \
        break;                                                                      \
    }

#define MAKE_HETSM_MARKER(_pt_cnt_elem)                                            \
    ((VOID *)(_pt_cnt_elem))

#define _LBCNTDEF_GET_ELEM_STATE(_pt_ctx,_ui2_idx)                                      \
    lb_get_elem_state((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num,_ui2_idx) 

#define _LBCNTDEF_GET_ELEM_STATE_P(_pt_ctx,_ui2_idx)                                      \
    lb_get_elem_state_addr((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num,_ui2_idx) 

#define _LBCNTDEF_SET_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_set_elem_state((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state,TRUE) 

#define _LBCNTDEF_CLR_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_set_elem_state((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state,FALSE)

#define _LBCNTDEF_CHK_ELEM_STATE(_pt_ctx,_ui2_idx,_ui1_state)                   \
    lb_chk_elem_state((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num,_ui2_idx,_ui1_state)

#define _LBCNTDEF_FIND_ELEM_ST_INF(_pt_ctx,_ui1_state,_ui2_start,_ui2_cnt,_b_fwd,_pui2_idx,_ppui1_state) \
    lb_find_elem_special_state((_pt_ctx)->pt_elem_inf_list,(_pt_ctx)->ui2_elem_num, \
                                 _ui1_state,_ui2_start,_ui2_cnt,_b_fwd,TRUE,_pui2_idx,_ppui1_state)

#define _LBCNTDEF_ELEMS_EMPTY(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num == 0)

#define _LBCNTDEF_SET_ELEMS_NUM(_pt_ctx, _ui2_total_num)                                      \
    ((_pt_ctx)->ui2_elem_num=  _ui2_total_num)

#define _LBCNTDEF_GET_ELEMS_NUM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num)

#define _LBCNTDEF_INC_ELEMS_NUM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num++)

#define _LBCNTDEF_DEC_ELEMS_NUM(_pt_ctx)                                      \
    ((_pt_ctx)->ui2_elem_num--)

#define _LBCNTDEF_SET_ELEM_CACHE_NUM(_pt_ctx, _ui2_num)                       \
    ((_pt_ctx)->ui2_cache_num =  _ui2_num)

#define _LBCNTDEF_GET_ELEM_CACHE_NUM(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_num)

#define _LBCNTDEF_DEC_ELEM_CACHE_NUM(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_num -- )

#define _LBCNTDEF_SET_CACHE_START(_pt_ctx, _ui2_start)                       \
    ((_pt_ctx)->ui2_cache_start =  _ui2_start)

#define _LBCNTDEF_GET_CACHE_START(_pt_ctx)                                 \
    ((_pt_ctx)->ui2_cache_start)

/*-----------------------------------------------------------------------------
                    constants
 ----------------------------------------------------------------------------*/
/* constants */
#define LBCNTDEF_ITEM_FMT_POOL_INIT_NUM     0
#define LBCNTDEF_ITEM_FMT_POOL_EXP_NUM      32
#define LBCNTDEF_SMART_CUT_CHARS            ((UTF16_T *)(L"..."))
#define LBCNTDEF_SMART_CUT_CHARS_LEN        3
#define MAX_VALUE                           ((INT32) 0x40000000)

/* default values */
static const WGL_INSET_T LBCNTDEF_DEF_CNT_INSET = 
{
    5, 5, 5, 5
};

static const WGL_INSET_T LBCNTDEF_DEF_ELEM_INSET = 
{
    2, 2, 2, 2
};

static const WGL_INSET_T LBCNTDEF_DEF_COL_INSET = 
{
    2, 2, 2, 2
};

/*-----------------------------------------------------------------------------
                        internal functions
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_paint_all( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint);
static INT32 _lbcntdef_paint_partial( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt);
static BOOL _lbcntdef_tool_check_if_hlt_elem_visible(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      BOOL                        b_check_elem_in_vp,
    IN      INT32                       i4_h_vp,
    IN      INT32                       i4_h_elem,
    IN      WGL_LB_EXTRA_MARGIN_T       *pt_ex_margin,
    IN      LB_ELEM_T                   *pt_elem_vp_first,
    IN      LB_ELEM_T                   *pt_elem_vp_last,
    IN      INT32                       i4_first_elem_top_vp,
    IN      INT32                       i4_last_elem_btm_vp,
    IN      UINT16                      ui2_first,
    IN      UINT16                      ui2_last,
    OUT     LB_ELEM_T                   **ppt_elem_hlt_out,
    OUT     INT32                       *pi4_hlt_elem_top_vp_out,
    OUT     UINT16                      *pui2_hlt);
static VOID _lbcntdef_get_vp_params_tool_1(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      INT32                       i4_h_elem,
    IN      UINT16                      ui2_first,
    IN      UINT16                      ui2_last,
    OUT     LB_ELEM_T                   **ppt_elem_first_out,
    OUT     LB_ELEM_T                   **ppt_elem_last_out,
    OUT     INT32                       *pi4_h_elem_total_out);
static INT32 _lbcntdef_render_elem( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      UINT32                      ui4_elem_flag,
    IN      INT32                       i4_elem_top_scrn,
    IN      GL_RECT_T                   *pt_rc_scrn_upt);
static BOOL _lbcntdef_draw_elem_bk_normal(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint, 
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_elem_bk, 
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem_bk,
    IN      GL_RECT_T                   *pt_rc_scrn_elem,
    IN      GL_RECT_T                   *pt_rc_scrn_upt);
static BOOL _lbcntdef_draw_elem_bk_hlt(
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint,
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_elem_bk,
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem_bk,
    IN      GL_RECT_T                   *pt_rc_scrn_lb,
    IN      GL_RECT_T                   *pt_rc_scrn_vp,
    IN      GL_RECT_T                   *pt_rc_scrn_elem,
    IN      GL_RECT_T                   *pt_rc_scrn_upt);
static BOOL _lbcntdef_get_hlt_elem_area(
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb,
    IN      GL_RECT_T                   *pt_rc_vp,
    IN      GL_RECT_T                   *pt_rc_hlt_elem,
    OUT     GL_RECT_T                   *pt_rc_hlt_elem_logical,
    OUT     GL_RECT_T                   *pt_rc_hlt_elem_paint);
static INT32 _lbcntdef_draw_item_bk(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      GL_RECT_T                   *pt_rc_scrn_col,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_upt);
static INT32 _lbcntdef_render_col_data(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_lb_upt);
static INT32 _lbcntdef_render_col_data_text(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt,
    IN      UTF16_T                     *pw2s_text);
static INT32 _lbcntdef_render_col_data_text_normal (
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T                  *pt_clr_text,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      ui4_len_line_1,
    IN      UINT32                      ui4_len_line_1_eff,
    IN      INT32                       i4_w,
    IN      INT32                       i4_h,
    IN      UTF16_T                     *pw2s_text);
static INT32 _lbcntdef_render_col_data_text_with_scrl(
    IN      LBCNTDEF_CTX_T*             pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint, 
    IN      GL_RECT_T*                  pt_rc_scrn_col_cnt,
    IN      LB_ELEM_T*                  pt_elem, 
    IN      LBCNTDEF_ELEM_T*            pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T*                 pt_clr_text,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT16                      ui2_len_line_1,
    IN      UINT16                      ui2_len_line_1_eff,
    IN      INT32                       i4_w,
    IN      INT32                       i4_h,
    IN      GL_RECT_T*                  pt_rc_scrn_col_cnt_upt,
    IN      UTF16_T                     *pw2s_text);
static INT32 _lbcntdef_render_col_data_img(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt);
static INT32 _lbcntdef_render_col_data_img_suit(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt);
static BOOL _lbcntdef_tool_check_hlt_elem_eff_enlarged(
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT16                      ui2_state_paint);
static VOID _lbcntdef_cal_item_obj_pos( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      BOOL                        b_is_txt,
    IN      UINT8                       ui1_c,       
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y);
static VOID _lbcntdef_cal_obj_pos(
    IN      UINT8                       ui1_ttl_dir,
    IN      BOOL                        b_is_txt,
    IN      BOOL                        b_opp_txt_align,
    IN      UINT8                       ui1_align,
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y);
static VOID _lbcntdef_get_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     LB_ELEM_T                   **ppt_elem,
    OUT     LBCNTDEF_ELEM_T             **ppt_cnt_elem);
static VOID _lbcntdef_get_item_attr(
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT8                       ui1_c, 
    IN      UINT16                      ui2_attr_id, 
    OUT     VOID                        *pv_attr_ret);
static GL_COLOR_T *_lbcntdef_select_elem_color_img( 
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_colors,
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs,
    IN      UINT16                      ui2_state_paint,
    IN      UINT8                       ui1_state_elem,
    OUT     WGL_HIMG_TPL_T              *ph_img_out);
static WGL_HIMG_TPL_T _lbcntdef_select_enhanced_hlt_elem_img(
    IN      WGL_LB_IMG_HLT_ELEM_SET_T   *pt_imgs_hlt,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint);
static VOID _lbcntdef_select_item_img_by_state( 
    IN      WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit,
    IN      UINT16                      ui2_state_paint,
    IN      UINT8                       ui1_state_elem,
    OUT     WGL_HIMG_TPL_T              *ph_img_out);
static VOID _lbcntdef_select_lb_color(
    IN      UINT16                      ui2_state_paint,
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_std, 
    OUT     GL_COLOR_T                  **ppt_clr_out);
static VOID _lbcntdef_select_lb_img( 
    IN      UINT16                      ui2_state_paint,
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
    OUT     WGL_HIMG_TPL_T              *ph_img_out);
static INT32 _lbcntdef_update_col_item_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT8                       ui1_c,
    IN OUT  LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      UINT16                      ui2_state_paint, 
    IN      UINT16                      ui1_state_elem);
static INT32 _lbcntdef_set_col_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_T             *pt_font_info_src, 
    OUT     HFONT_T                     *ph_font,
    IN      UINT8                       ui1_c);
static INT32 _lbcntdef_set_col_font_ex(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_LB_FONT_INFO_EX_T       *pt_ex_lb_font_info_src, 
    OUT     HFONT_T                     *ph_font,
    IN      UINT8                       ui1_c);
static INT32 _lbcntdef_get_col_font_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    OUT     WGL_LB_FONT_INFO_EX_T       *pt_ex_lb_font_info, 
    IN      UINT8                       ui1_c);
static VOID _lbcntdef_get_sys_lb_elem_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_def);
static VOID _lbcntdef_get_sys_lb_elem_text_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_text);
static VOID _lbcntdef_get_sys_lb_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs);
static VOID _lbcntdef_get_sys_lb_fg_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs);
static VOID _lbcntdef_get_sys_lb_vp_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs);
static VOID _lbcntdef_get_sys_lb_elem_bk_img_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem);
static VOID  _lbcntdef_get_sys_lb_bk_img_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_IMG_STD_SET_T           *pt_imgs);
static VOID  _lbcntdef_set_dft_vp_bk_img_set(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_bk_vp);
static VOID _lbcntdef_tool_set_lb_color_set(
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs_dst, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info_src);
static VOID _lbcntdef_tool_set_lb_bk_img_set(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_dst, 
    IN      WGL_IMG_INFO_T              *pt_img_info_src);
static INT32 _lbcntdef_set_item_attr(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id,
    IN      VOID                        *pv_value);
static INT32 _lbcntdef_set_item_use_def(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id);
static INT32 _lbcntdef_get_item_use_def(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id,
    OUT     BOOL*                       pb_use_def);
static INT32 _lbcntdef_set_title_text(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      UTF16_T                     *w2s_text,  
    IN      UINT16                      ui2_len_text);
static INT32 _lbcntdef_set_title_info(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_LB_TITLE_INFO_T         *pt_ttl_info);
static VOID _lbcntdef_set_default_title_info(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx);
static VOID _lbcntdef_update_title_text_position(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      INT32                       i4_w_text,
    IN      INT32                       i4_h_text);
static INT32 _lbcntdef_add_elem(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      BOOL                        b_add_create);
static INT32 _lbcntdef_del_elem(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      BOOL                        b_del_free);
static INT32 _lbcntdef_set_lb_screen_rect_or_bdr_inset(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      GL_RECT_T                   *pt_rc_scrn_lb, 
    IN      WGL_INSET_T                 *pt_inset_bdr);
static INT32 _lbcntdef_update_vp_rect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx);
static INT32 _lbcntdef_set_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_T             *pt_font_info_src, 
    OUT     HFONT_T                     *ph_font);
static INT32 _lbcntdef_set_font_ex(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_EX_T          *pt_ex_font_info_src, 
    OUT     HFONT_T                     *ph_font);
static INT32 _lbcntdef_get_font_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    OUT     WGL_FONT_INFO_EX_T          *pt_ex_font_info);
static INT32 _lbcntdef_update_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_EX_T          *pt_ex_font_info, 
    OUT     HFONT_T                     *ph_font);
static INT32 _lbcntdef_draw_lb_bk(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt);
static INT32 _lbcntdef_draw_lb_vp_bk(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt);
static INT32 _lbcntdef_draw_scrl_bar(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt );
static INT32 _lbcntdef_draw_title(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_update);
static INT32 _lbcntdef_draw_scroll_images(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      GL_RECT_T                   *pt_rc_update );
static INT32 _lbcntdef_draw_indicator_images(
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs,
    IN      BOOL                        b_is_up_on,
    IN      BOOL                        b_is_down_on,
    IN      GL_RECT_T                   *pt_rc_update);
static UINT16 _lbcntdef_get_hlt_idx(
    IN      LBCNTDEF_CTX_T              *pt_ctx);
static BOOL _lbcntdef_tool_check_valid_image_handle(
    IN      WGL_HIMG_TPL_T              h_img);
static BOOL _lbcntdef_tool_check_valid_img_set_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs);
static BOOL _lbcntdef_tool_check_valid_elem_image_set(
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem);
static BOOL _lbcntdef_tool_check_valid_scroll_image_info(
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs);
static BOOL _lbcntdef_tool_check_valid_hlt_elem_img_set(
    IN      WGL_LB_IMG_HLT_ELEM_SET_T   *pt_hlt_elem_img_set);
static UINT16 _lbcntdef_tool_get_max_text_len(
    IN      const LB_COL_T*             pat_cols,
    IN      UINT8                       ui1_col_num );
static VOID _lbcntdef_add_update_region_by_diff_scrl_imgs (
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info_orig,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info_new);
static VOID _lbcntdef_add_update_region_by_vp_indicators(
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out);
static INT32 _lbcntdef_add_update_region_by_hlt_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_hlt,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out);
static INT32 _lbcntdef_add_update_region_by_range_elems (
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LB_CNT_PU_ELEM_RANGE_T      *pt_elem_range,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out);
static VOID _lbcntdef_add_update_region_by_full_lb (
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out);
static INT32 _lbcntdef_get_update_region_by_item(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT32                      ui4_state_wgt,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    OUT     LB_CNT_PU_REGION_T          *pt_pur_out);
static INT32 _lbcntdef_get_item_update_rect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui4_state_wgt,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    OUT     GL_RECT_T*                  pt_rc_out);
static INT32 _lbcntdef_get_update_region_generic(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     LB_CNT_PU_REGION_T          *pt_pur_out);
static INT32 _lbcntdef_set_text_dir(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT8                       ui1_text_dir);
static INT32 _lbcntdef_get_text_dir(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     UINT32*                     pui4_text_dir);
static INT32 _lbcntdef_cmd_hdr_set_hlt_elem_effect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect);
static INT32 _lbcntdef_cmd_hdr_get_hlt_elem_effect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect_out);
static INT32 _lbcntdef_cmd_hdr_set_scrl_bar(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_SCRL_BAR_T           *pt_attr_sb);
static INT32 _lbcntdef_cmd_hdr_get_req_timer(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    OUT     LB_CNT_TIMER_REQ_T*         pt_cnt_tr_out);
static INT32 _lbcntdef_cmd_hdr_do_timer(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    IN      LB_CNT_DO_TIMER_PARAM_T*    pt_param);
static INT32 _lbcntdef_timer_hdr_sht(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    IN      UINT32                      ui4_state_wgt,
    IN      LB_TR_T*                    pt_tr);
static UINT32 _lbcntdef_tool_get_first_line_len (
    IN      UINT8                       ui1_txt_dir,
    IN      UTF16_T*                    w2s_text);
static INT32 _lbcntdef_tool_cut_line_by_soft_hyphen (
    IN      UINT8                       ui1_txt_dir,
    IN      HFONT_T                     h_font,
    IN      UTF16_T*                    w2s_line,
    IN      UINT32                      ui4_len_line,
    IN      INT32                       i4_w_bound,
    OUT     UINT32*                     pui4_len_line_cut,
    OUT     INT32*                      pi4_w_cut,
    OUT     INT32*                      pi4_h);
static INT32 _lbcntdef_tool_get_item_width (
    IN      const LB_COL_T              *at_cols,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c);
static VOID _lbcntdef_purm_reset(
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur);
static VOID _lbcntdef_purm_copy(
    OUT     LB_CNT_PU_REGION_T          *pt_pur_dst, 
    IN      LB_CNT_PU_REGION_T          *pt_pur_src);
static VOID _lbcntdef_purm_add_rect(
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out, 
    IN      GL_RECT_T                   *pt_rc);
static BOOL _lbcntdef_purm_tool_decide_mergence(
    IN      GL_RECT_T                   *pt_rc_1,
    IN      GL_RECT_T                   *pt_rc_2);

#ifdef WGL_ANIM_SUPPORT
static INT32 _lbcntdef_add_update_region(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  WGL_UPD_REGION_T            *pt_upd_rgn);

static BOOL _lbcntdef_draw_anim_elem(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      LBCNTDEF_CTX_T*             pt_ctx,
    IN      WGL_CANVAS_STATUS_T*        pt_cvsst,
    IN      UINT8                       ui1_state_elem,
    IN      WGL_LB_ANIM_ELEM_EFFECT_T*  pt_anim_elem, 
    IN      UINT16                      ui2_state_paint, 
    IN      GL_RECT_T                   *pt_rc_scrn_upt);

static INT32 _lbcntdef_init_anim_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx);

static INT32 _lbcntdef_render_anim_elem( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      GL_RECT_T                   *pt_upd_lb_rgn);

static INT32 _lbcntdef_cmd_hdr_get_anim_elem_effect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     WGL_LB_ANIM_ELEM_EFFECT_T   *pt_effect_out);

static INT32 _lbcntdef_cmd_hdr_set_anim_elem_effect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_ANIM_ELEM_EFFECT_T    *pt_anim);

static INT32 _lbcntdef_tool_check_anim_elem(
    LBCNTDEF_CTX_T                              *pt_ctx,
    LB_ELEM_T                                   **ppt_anim_elem);
#endif

static INT32 _lbcntdef_do_cb_fct(
    LBCNTDEF_CTX_T*                         pt_ctx,
    WGL_LB_NFY_TYPE_T                       e_nfy_type,
    VOID*                                   pv_param1,
    VOID*                                   pv_param2);

#ifdef WGL_LB_NO_STORE_TEXT
static BOOL _lbcntdef_check_text_type_col(
    IN      LBCNTDEF_CTX_T              *pt_ctx); 

static INT32 _lbcntdef_get_item_data_ex(
    IN      LBCNTDEF_CTX_T*             pt_ctx,    
    IN      WGL_LB_ELEM_DATA_INFO_T*    pt_elem_data,
    IN      UINT16                      ui2_elem,
    IN      UINT8                       ui1_col,
    OUT     WGL_LB_ITEM_DATA_T**        ppt_item_data);

static INT32 _lbcntdef_render_elem_ex( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      INT32                       i4_elem_top_scrn,
    IN      GL_RECT_T                   *pt_rc_scrn_upt,
    IN      WGL_LB_ELEM_DATA_INFO_T*    pt_elem_data,
    IN      UINT16                      ui2_elem);

static INT32 _lbcntdef_tool_get_item_width_ex (
    IN      const LB_COL_T              *at_cols,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN     WGL_LB_ITEM_DATA_T*          pt_item_data);

static INT32 _lbcntdef_render_col_data_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt,
    IN     WGL_LB_ITEM_DATA_T*          pt_item_data);



static INT32 _lbcntdef_init_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx,
    UINT16                          ui2_max_elem,
    UINT16                          ui2_text_max_len);

static INT32 _lbcntdef_deinit_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx);

static INT32 _lbcntdef_clear_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx);

static INT32 _lbcntdef_no_store_get_data(
    LBCNTDEF_CTX_T                  *pt_ctx,
    WGL_LB_ELEM_DATA_INFO_T         *pt_data_inf);
#endif

static INT32 _lbcntdef_free_elem_tsa(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c);

static VOID _lbcntdef_get_elem_flag(
    UINT16                           ui2_elem_idx,
    UINT16                           ui2_vp_last,
    UINT16                           ui2_elem_num,
    UINT32                           *pui4_flag);
#ifdef WGL_LB_SUPPORT_MULTI_LINE
static INT32 _lbcntdef_get_line_inf(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      *pui4_len_line,
    IN      UINT32                      *pui4_len_line_eff,
    IN      UINT16                      *pui2_line_num,
    IN      UINT16 						ui2_max_lines,
    IN      INT32                       i4_h,
    OUT		INT32						*pi4_w,
    IN      UINT8                       ui1_c,
    IN      UTF16_T                     **ppw2s_text,
    OUT     LBCNTDEF_LINE_INF_T         **ppt_line);

static INT32 _lbcntdef_get_tail_line_inf(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      ui4_len_line_1,
    IN      UINT32                      ui4_len_line_1_eff,
    IN      INT32                       i4_h,
    IN      INT32                       i4_w,
    IN      UINT8                       ui1_c,
    IN      UTF16_T                     *pw2s_text,
    OUT     LBCNTDEF_LINE_INF_T         *pt_line);

static INT32 _lbcntdef_draw_line(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      GL_COLOR_T                  *pt_clr_text,
    IN      GL_COLOR_T                  *pt_clr_transp,
    IN      LBCNTDEF_LINE_INF_T*        pt_line,
    IN      UINT16                      ui2_index,
    IN      UINT16                      ui2_line_num);

static VOID _lbcntdef_cal_item_obj_pos_multiline( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      BOOL                        b_is_txt,
    IN      UINT8                       ui1_c,       
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT16                      ui2_line_num,
    IN      UINT16                      ui2_index,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y);
#endif
/*-----------------------------------------------------------------------------
                        exported functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: wgl_lb_cnt_def_inst_create
 *
 * Description: This API creates an instance of listbox content.
 *
 * Inputs:  ppt_cnt_create  Specifies the parameters for initialization.
 *
 * Outputs: ppv_ctx_cnt     Contains the instance reference of the new instance context.
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_cnt_def_inst_create(
    WGL_CONTENT_CONTEXT_HANDLE_T        *ppv_ctx_cnt, 
    WGL_CONTENT_CREATE_INPUT_T          *pt_cnt_create )
{
    LBCNTDEF_CTX_T                      *pt_ctx = NULL;
    LB_CNT_INIT_T                       *pt_init_lb_cnt;
    WGL_FONT_INFO_T                     t_font_info;
    UINT8                               ui1_c;
    INT32                               i4_size_vp;
    INT32                               i4_ret, i4_ret_blk;
    BOOL                                b_init_frlm, b_init_npm_itm_fmt, b_init_sb, b_init_trq, b_init_twl, b_init_hetsm;
	UINT16								ui2_buf_len = 0;
    UINT32                              ui4_size;

    *ppv_ctx_cnt   = NULL;
    
    pt_init_lb_cnt = (LB_CNT_INIT_T *)pt_cnt_create->pv_init;
    
	for (ui1_c=0; ui1_c <pt_init_lb_cnt->ui1_col_num; ui1_c++)
    {       
        if (pt_init_lb_cnt->at_cols[ui1_c].e_col_type == LB_COL_TYPE_TEXT )
        {
            if (pt_init_lb_cnt->at_cols[ui1_c].ui2_max_text_len > ui2_buf_len)
         	{
				ui2_buf_len = pt_init_lb_cnt->at_cols[ui1_c].ui2_max_text_len ;
         	}
        }      
    }
    
    ui4_size =  WGL_ALIGN_4(sizeof(LBCNTDEF_CTX_T)) + 
                WGL_ALIGN_4(sizeof(LBCNTDEF_COL_T) * pt_init_lb_cnt->ui1_col_num) +
                (ui2_buf_len + 1) * sizeof(UTF16_T);
    pt_ctx  = (LBCNTDEF_CTX_T *)WGL_MEM_ALLOC( ui4_size ); 
    CHECK_VALID(pt_ctx != NULL, WGLR_OUT_OF_MEMORY);

    i4_ret_blk         = WGLR_OK;
    b_init_frlm        = FALSE;
    b_init_npm_itm_fmt = FALSE;
    b_init_sb          = FALSE;
    b_init_trq         = FALSE;
    b_init_twl         = FALSE;
    b_init_hetsm       = FALSE;
    
___BLOCK_BEGIN___    
    
    /*init the FRLM (Font Resource List Manager)*/
    wgl_frlm_init(&pt_ctx->t_frlm);
    b_init_frlm = TRUE;

    /* init item format resource */
    GOTO_EOB_AT_FAIL (wgl_npm_init (&pt_ctx->t_npm_itm_fmt, 
                                    sizeof (LBCNTDEF_ITEM_FORMAT_T), 
                                    LBCNTDEF_ITEM_FMT_POOL_INIT_NUM, 
                                    LBCNTDEF_ITEM_FMT_POOL_EXP_NUM), i4_ret_blk);
    b_init_npm_itm_fmt = TRUE;
	pt_ctx->w2s_buffer = (UTF16_T*)(((UINT32) pt_ctx) + ((UINT32)WGL_ALIGN_4(sizeof(LBCNTDEF_CTX_T))) + ((UINT32)WGL_ALIGN_4 (sizeof(LBCNTDEF_COL_T) * pt_init_lb_cnt->ui1_col_num)));

    pt_ctx->ui4_style = pt_init_lb_cnt->ui4_style;
    pt_ctx->ui4_cnt_style = 0;
    pt_ctx->ui1_state = pt_init_lb_cnt->ui1_state;
    pt_ctx->ui1_flags = 0;
#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size = 0 ;
#endif  
    if(pt_init_lb_cnt->b_is_bdr_draw_interior)
    {
        WGL_SET_FLAG(pt_ctx->ui1_flags, LBCNTDEF_FLAG_IS_BDR_DRAW_INTERIOR);
    }
    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_cnt_create->pt_cvsst);

    WGL_RECT_COPY(&pt_ctx->t_rc_lb_cnt, &pt_cnt_create->t_rect_cnt);
    if( !_RECT_IS_VALID(pt_ctx->t_rc_lb_cnt) )
    {
        WGL_RECT_EMPTY(&pt_ctx->t_rc_lb_cnt);
    }
    WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_lb);
    WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_vp);
    WGL_RECT_EMPTY(&pt_ctx->t_rc_lb_vp);
    _INSET_CLEAR(&pt_ctx->t_inset_bdr);
    _INSET_COPY(&pt_ctx->t_inset_cnt, &LBCNTDEF_DEF_CNT_INSET);
    _INSET_COPY(&pt_ctx->t_inset_elem, &LBCNTDEF_DEF_ELEM_INSET);
    pt_ctx->ui2_elem_height = pt_init_lb_cnt->ui2_elem_size;
    pt_ctx->ui2_elem_dist   = pt_init_lb_cnt->ui2_elem_dist;
    
    /*init the default List Box Font*/
    GOTO_EOB_AT_FAIL(_lbcntdef_set_font(pt_ctx, NULL, &pt_ctx->h_font), i4_ret_blk);

    GOTO_EOB_AT_FAIL(wgl_frlm_get_info(&pt_ctx->t_frlm, pt_ctx->h_font, &t_font_info), i4_ret_blk);
    t_font_info.i2_width = 0;
    
    /*init the scrolling indication image information*/
    _SCR_IMG_INFO_CLEAR(pt_ctx->t_scr_imgs);
    _SCR_IMG_INFO_CLEAR(pt_ctx->t_scr_imgs_paging);
    
    _lbcntdef_get_sys_lb_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk);
    _lbcntdef_get_sys_lb_vp_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk_vp);
    _lbcntdef_get_sys_lb_bk_img_set(&pt_ctx->t_cvsst, &pt_ctx->t_imgs_bk);
    _lbcntdef_set_dft_vp_bk_img_set(&pt_ctx->t_imgs_bk_vp);
    _lbcntdef_get_sys_lb_elem_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_elem_bk);
    _lbcntdef_get_sys_lb_elem_bk_img_set(&pt_ctx->t_cvsst, &pt_ctx->t_imgs_elem_bk);
    pt_ctx->t_effect_hlt_elem.ui4_style = 0;
    pt_ctx->ui1_col_num                 = pt_init_lb_cnt->ui1_col_num;
    pt_ctx->at_cols                     = pt_init_lb_cnt->at_cols;
    
#ifdef WGL_LB_NO_STORE_TEXT
    /*It is used for the no stor style*/
    pt_ctx->ui2_max_text_len            = ui2_buf_len;
    _lbcntdef_init_elem_buf(pt_ctx,pt_init_lb_cnt->ui2_elem_num,ui2_buf_len);
#endif
    
    /*init the title*/
    WGL_RECT_EMPTY(&pt_ctx->t_rc_lb_ttl_text);
    pt_ctx->w2s_text_ttl = NULL;
     pt_ctx->w2s_text_ttl_raw = NULL;
    pt_ctx->ui2_len_ttl  = pt_ctx->ui2_len_ttl_raw = pt_ctx->ui2_len_ttl_buf = 0;
    /*The default text direction is LEFT_TO_RIGHT*/
    pt_ctx->ui1_txt_dir = WGL_TEXT_DIR_LEFT_TO_RIGHT;
    pt_ctx->b_opp_txt_align = FALSE;
    
    /*init text attr*/
    pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
    pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
 
    _lbcntdef_set_default_title_info(pt_ctx);
    _lbcntdef_get_sys_lb_fg_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_fg);
    _lbcntdef_get_sys_lb_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk);

    /*init pt_ctx->at_cnt_cols */
    pt_ctx->at_cnt_cols = (LBCNTDEF_COL_T *)(((BYTE *)pt_ctx) + WGL_ALIGN_4(sizeof(LBCNTDEF_CTX_T)));
    
    for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
    {
        pt_ctx->at_cnt_cols[ui1_c].ui1_align     = pt_init_lb_cnt->at_init_cols[ui1_c].ui1_align;
        if(pt_ctx->ui4_style & WGL_STL_GL_FIX_TEXT_DIR)
        {
            pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir= TRUE;
            pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir_align= TRUE;
        }
        else
        {
            pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir= FALSE;
            pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir_align= FALSE;
        }
        pt_ctx->at_cnt_cols[ui1_c].ui2_col_width = pt_init_lb_cnt->at_init_cols[ui1_c].ui2_width;
        _INSET_COPY(&pt_ctx->at_cnt_cols[ui1_c].t_inset, &LBCNTDEF_DEF_COL_INSET);
        /* not to render item bk by default */
        _LBE_COLOR_SET_CLEAR(&pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk); 
        _lbcntdef_get_sys_lb_elem_text_color_set(&pt_ctx->t_cvsst, &pt_ctx->at_cnt_cols[ui1_c].t_clrs_text);
        pt_ctx->at_cnt_cols[ui1_c].h_font        = pt_ctx->h_font;
        pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_normal              = t_font_info;
        pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_highlight           = t_font_info;
        pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_highlight_unfocused = t_font_info;
        pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_selected            = t_font_info;
        _LBE_IMG_SET_CLEAR(&pt_ctx->at_cnt_cols[ui1_c].t_imgs_bk);
    }

    _LBCNTDEF_SET_ELEMS_NUM(pt_ctx, 0);
    _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, 0);
    pt_ctx->pt_elems = NULL;
    #ifdef LB_LEGACY_DRAW_TEXT
    {
        pt_ctx->h_surf_text_temp = NULL_HANDLE;
    }
    #endif
    pt_ctx->ui4_tk_last_full_update = 0;
    _lbcntdef_purm_reset(&pt_ctx->t_pur_wait_for_update);

    /*init viewport*/
    i4_size_vp = (_RECT_GET_H(pt_ctx->t_rc_lb_cnt) - (pt_ctx->t_inset_cnt.i4_top+pt_ctx->t_inset_cnt.i4_bottom));
    i4_size_vp = i4_size_vp<0 ? 0 : i4_size_vp;
    GOTO_EOB_AT_FAIL( wgl_vp_init( 
        (UINT16)i4_size_vp,
        pt_ctx->ui2_elem_height,
        pt_ctx->ui2_elem_dist,
        pt_ctx->ui2_elem_num,
        &pt_ctx->t_vp), i4_ret_blk );
    
    /* init scroll bar context */
    wgl_lb_sb_init(&pt_ctx->t_ctx_sb);
    b_init_sb = TRUE;
    
    /* init scroll bar attributes */
    pt_ctx->t_attr_sb.ui4_style = 0;
    WGL_RECT_EMPTY(&pt_ctx->t_attr_sb.t_rc_body);
    _INSET_EMPTY(&pt_ctx->t_attr_sb.t_inset_track);
    pt_ctx->t_attr_sb.ui4_sz_thumb_min = 0;
    x_memset(&pt_ctx->t_attr_sb.t_clrs_body, 0, sizeof(WGL_COLOR_STD_SET_T));
    x_memset(&pt_ctx->t_attr_sb.t_imgs_body, 0, sizeof(WGL_IMG_STD_SET_T));
    x_memset(&pt_ctx->t_attr_sb.t_clrs_thumb, 0, sizeof(WGL_COLOR_STD_SET_T));
    x_memset(&pt_ctx->t_attr_sb.t_imgs_thumb, 0, sizeof(WGL_IMG_STD_SET_T));
    
    /* init timer request queue */
    GOTO_EOB_AT_FAIL( anm_trq_init ( & pt_ctx->t_trq, sizeof(LB_TR_T) ), i4_ret_blk );
    b_init_trq   = TRUE;
    
    /* init timer wait list */
    GOTO_EOB_AT_FAIL( anm_twl_init ( & pt_ctx->t_twl, sizeof(LB_TR_T) ), i4_ret_blk );
    b_init_twl   = TRUE;
    
    /* init highlight element text scroll manager */
    GOTO_EOB_AT_FAIL( lb_hetsm_init ( 
        & pt_ctx->t_hetsm,  
        _lbcntdef_tool_get_max_text_len( pt_ctx->at_cols, pt_ctx->ui1_col_num ) ), i4_ret_blk );
    b_init_hetsm = TRUE;

#ifdef WGL_ANIM_SUPPORT
    /*Init the default animation information*/
    _lbcntdef_init_anim_elem(pt_ctx);
#endif
    
___BLOCK_END___    

    if ( WGL_IS_FAIL ( i4_ret_blk ) )
    {
        if ( b_init_hetsm )
        {
            INFORM_ERR( lb_hetsm_deinit ( & pt_ctx->t_hetsm ) );
            b_init_hetsm = FALSE;
        }
        if ( b_init_twl )
        {
            INFORM_ERR( anm_twl_deinit ( & pt_ctx->t_twl ) );
            b_init_twl = FALSE;
        }
        if ( b_init_trq )
        {
            INFORM_ERR( anm_trq_deinit ( & pt_ctx->t_trq ) );
            b_init_trq = FALSE;
        }
        if ( b_init_sb )
        {
            wgl_lb_sb_deinit ( & pt_ctx->t_ctx_sb );
            b_init_sb = FALSE;
        }
        if (b_init_npm_itm_fmt)
        {
            wgl_npm_deinit (&pt_ctx->t_npm_itm_fmt);
            b_init_npm_itm_fmt = FALSE;
        }
        if ( b_init_frlm )
        {
            wgl_frlm_destroy(&pt_ctx->t_frlm);
        }
        if ( pt_ctx )
        {
            WGL_MEM_FREE( pt_ctx );
            pt_ctx = NULL;
        }
        
        return i4_ret_blk;
    }


#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size += ui4_size;
#endif  
    *ppv_ctx_cnt = (WGL_CONTENT_CONTEXT_HANDLE_T)pt_ctx;

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_cnt_def_inst_destroy
 *
 * Description: This API destroies a content instance.
 *
 * Inputs:  pv_ctx_cnt  Specifies the instance context handle.
 *
 * Outputs: -
 *
 * Returns: WGLR_OK         Routine successful.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_cnt_def_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt)
{
    LBCNTDEF_CTX_T      *pt_ctx  = (LBCNTDEF_CTX_T *)pv_ctx_cnt;
    LB_ELEM_T           *pt_elem = NULL;
    UINT16              ui2_i;
    UINT16              ui2_cache_end;
    INT32               i4_ret;

    INFORM_ERR( lb_hetsm_deinit ( & pt_ctx->t_hetsm ) );

    INFORM_ERR( anm_twl_deinit ( & pt_ctx->t_twl ) );

    INFORM_ERR( anm_trq_deinit ( & pt_ctx->t_trq ) );

#ifdef WGL_LB_NO_STORE_TEXT
    _lbcntdef_deinit_elem_buf(pt_ctx);
#endif

    #ifdef LB_LEGACY_DRAW_TEXT
    {
        if(pt_ctx->h_surf_text_temp != NULL_HANDLE)
        {
            CATCH_FAIL(x_gl_obj_free(pt_ctx->h_surf_text_temp));
            pt_ctx->h_surf_text_temp = NULL_HANDLE;
        }
    }
    #endif

    ui2_cache_end = _LBCNTDEF_GET_CACHE_START(pt_ctx) + _LBCNTDEF_GET_ELEM_CACHE_NUM(pt_ctx);
    for(ui2_i=0; ui2_i<_LBCNTDEF_GET_ELEMS_NUM(pt_ctx); ui2_i++)
    {
        if(ui2_i == _LBCNTDEF_GET_CACHE_START(pt_ctx) )
        {
            pt_elem = (LB_ELEM_T *)pt_ctx->pt_elems;
        }
        else if( (ui2_i > _LBCNTDEF_GET_CACHE_START(pt_ctx)) && (ui2_i < ui2_cache_end))
        {
            if(pt_elem != NULL)
            {
                pt_elem = pt_elem->pt_next;
            }
        }
        else
        {
            pt_elem = NULL; 
        }
        CATCH_FAIL(_lbcntdef_del_elem(pt_ctx, pt_elem,TRUE ));
    }

    if(pt_ctx->w2s_text_ttl)
    {
        WGL_MEM_FREE(pt_ctx->w2s_text_ttl);
        pt_ctx->w2s_text_ttl = NULL;
    }
     if(pt_ctx->w2s_text_ttl_raw)
    {
        WGL_MEM_FREE(pt_ctx->w2s_text_ttl_raw);
        pt_ctx->w2s_text_ttl_raw = NULL;
    }
    wgl_npm_deinit (&pt_ctx->t_npm_itm_fmt);
    
    wgl_frlm_destroy(&pt_ctx->t_frlm);
    
    wgl_lb_sb_deinit(&pt_ctx->t_ctx_sb);

    WGL_MEM_FREE((VOID *)pt_ctx);
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_cnt_def_cmd_proc
 *
 * Description: This API handles a content commmand.
 *
 * Inputs:  pv_ctx_cnt      Contains the instance context handle.
 *          ui4_cnt_cmd_id  Specifies the content command id.
 *          pv_param1       Contains the first parameter if necessary.
 *          pv_param2       Contains the second parameter if necessary.
 *
 * Outputs: pv_ctx_cnt      Contains the instance context handle.
 *          pv_param1       Contains the first return value if necessary.
 *          pv_param2       Contains the second return value if necessary.
 *
 * Returns: WGLR_OK                 Routine successful.
 *          WGLR_INV_ARG            Invalid argument.
 *          WGLR_INV_CMD            Invalid command.
 *          WGLR_INTERNAL_ERROR     Internal error.
 *          WGLR_BUFFER_NOT_ENOUGH  The prepared buffer not enough.
 *          < 0                     Other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_cnt_def_cmd_proc(  
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    UINT32                              ui4_cnt_cmd_id,
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{
    INT32                   i4_ret;
    INT32                   i4_ret_val = WGLR_OK;
    LBCNTDEF_CTX_T          *pt_ctx    = (LBCNTDEF_CTX_T *)pv_ctx_cnt;
    WGL_FONT_ATTR_T         t_attr;

    if( !WGL_IS_CMD_BELONG_CONTENT(WGL_CONTENT_LIST_BOX_DEF, ui4_cnt_cmd_id) )
    {
        return WGLR_INV_CMD;
    }

    switch(ui4_cnt_cmd_id)
    {
        /*Optimized Internal Content Commands*/
        case WGL_CNT_LB_SET_SCREEN_RECT: /*pv_param1:(GL_RECT_T *) pv_param2:(WGL_INSET_T *)*/
        {        
            GL_RECT_T *pt_rc_scrn_lb  = (GL_RECT_T *)pv_param1;
            WGL_INSET_T *pt_inset_bdr; 
            
            if (pv_param2)
            {
                pt_inset_bdr = (WGL_INSET_T *) pv_param2;
            }
            else
            {
                pt_inset_bdr = NULL;
            }

            i4_ret_val = _lbcntdef_set_lb_screen_rect_or_bdr_inset(pt_ctx, pt_rc_scrn_lb, pt_inset_bdr);
        }
        break;
        
        case WGL_CNT_LB_GET_SCREEN_RECT:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            WGL_RECT_COPY((GL_RECT_T *)pv_param1, &pt_ctx->t_rc_scrn_lb);
        }
        break;        
        
        /*Exported Content Commands*/
        case WGL_CMD_LB_SET_CNT_INSET:
        {
            if(pv_param1==NULL)
            {
                _INSET_COPY( &pt_ctx->t_inset_cnt, &LBCNTDEF_DEF_CNT_INSET);
            }
            else
            {
                WGL_INSET_T *pt_inset = (WGL_INSET_T *)pv_param1;
                if(CHECK_VALID_INSET(pt_inset))
                {
                    _INSET_COPY(&pt_ctx->t_inset_cnt, pt_inset);
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                    break;
                }
            }
            i4_ret_val = _lbcntdef_update_vp_rect(pt_ctx);
        }
        break;

        case WGL_CMD_LB_GET_CNT_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _INSET_COPY(pv_param1, &pt_ctx->t_inset_cnt);
        }
        break;
        
        case WGL_CMD_LB_SET_TITLE_TEXT: /*1:[IN]     (UTF16_T *)w2s_text
                                          2:[IN]     (UINT16)ui2_len_text */
        {
            i4_ret_val = _lbcntdef_set_title_text(pt_ctx, (UTF16_T *)pv_param1, WGL_POINTER_TO_UINT16(pv_param2));
        }
        break;
        
        case WGL_CMD_LB_GET_TITLE_TEXT: /*1:[IN/OUT] (UTF16_T *)w2s_text
                                          2:[IN]     (UINT16)ui2_len_buf */
        {
            UTF16_T     *w2s_buff   = (UTF16_T *)pv_param1;
            UINT16      ui2_len_buf = WGL_POINTER_TO_UINT16(pv_param2);
            CHECK_NULL(w2s_buff, WGLR_INV_ARG);
            CHECK_VALID(ui2_len_buf > pt_ctx->ui2_len_ttl_raw, WGLR_BUFFER_NOT_ENOUGH);
            x_uc_w2s_strncpy(w2s_buff, pt_ctx->w2s_text_ttl_raw, (UINT32)pt_ctx->ui2_len_ttl_raw);
            x_uc_w2s_set(w2s_buff, (UINT32)pt_ctx->ui2_len_ttl_raw, (UTF16_T)0);/*end character /0*/
        }
        break;

        case WGL_CMD_LB_SET_TITLE_INFO: 
        {
            WGL_LB_TITLE_INFO_T *pt_ttl_info = (WGL_LB_TITLE_INFO_T *)pv_param1;
            if(pt_ttl_info)
            {
                CHECK_VALID( WGL_IS_VALID_ALIGN(pt_ttl_info->ui1_align) && RECT_IS_EFFECTIVE(pt_ttl_info->t_rect), WGLR_INV_ARG);
            }
            i4_ret_val = _lbcntdef_set_title_info(pt_ctx, pt_ttl_info);
        }
        break;

        case WGL_CMD_LB_GET_TITLE_INFO:
        {
            WGL_LB_TITLE_INFO_T *pt_ttl_info = (WGL_LB_TITLE_INFO_T *)pv_param1;
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            WGL_RECT_COPY(&pt_ttl_info->t_rect, &pt_ctx->t_rc_lb_ttl_bound);
            pt_ttl_info->ui1_align = pt_ctx->ui1_align_ttl;
        }
        break;
        
        case WGL_CMD_LB_GET_ELEM_SIZE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = pt_ctx->ui2_elem_height;
        }
        break;

        case WGL_CMD_LB_SET_ELEM_SIZE:
        {
            WGL_LB_ELEM_SIZE_T* pt_elem_size_inf = (WGL_LB_ELEM_SIZE_T*)pv_param1;

            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            wgl_vp_change_element_size(&pt_ctx->t_vp,
                                       pt_elem_size_inf->b_size_by_num,
                                       pt_elem_size_inf->ui2_elem_height,
                                       pt_elem_size_inf->ui2_elem_num);
            
            pt_ctx->ui2_elem_height = pt_ctx->t_vp.ui2_size_elem;
            pt_ctx->t_ctx_sb.ui4_sz_elem = pt_ctx->ui2_elem_height;
            
            WGL_SET_FLAG(pt_ctx->t_ctx_sb.ui2_state, SBST_DIRTY);
        }
        break;        

        case WGL_CMD_LB_GET_ELEM_DISTANCE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT16 *)pv_param1) = pt_ctx->ui2_elem_dist;
        }
        break;

        case WGL_CMD_LB_SET_ELEM_DISTANCE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            pt_ctx->ui2_elem_dist = WGL_POINTER_TO_UINT16(pv_param1);
            wgl_vp_change_element_size(&pt_ctx->t_vp,FALSE,pt_ctx->ui2_elem_height,0);
            pt_ctx->t_ctx_sb.ui4_ds_elem = pt_ctx->ui2_elem_dist;
            
            WGL_SET_FLAG(pt_ctx->t_ctx_sb.ui2_state, SBST_DIRTY);
        }
        break;
        
        case WGL_CMD_LB_SET_ELEM_INSET:
        {
            if(pv_param1==NULL)
            {
                _INSET_COPY(&pt_ctx->t_inset_elem, &LBCNTDEF_DEF_ELEM_INSET);
            }
            else
            {
                WGL_INSET_T *pt_inset = (WGL_INSET_T *)pv_param1;
                
                if(CHECK_VALID_INSET(pt_inset))
                {
                    _INSET_COPY(&pt_ctx->t_inset_elem, pt_inset);
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                    break;
                }
            }
        }
        break;

        case WGL_CMD_LB_GET_ELEM_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _INSET_COPY(pv_param1, &pt_ctx->t_inset_elem);
        }
        break;
            
        case WGL_CMD_LB_SET_ELEM_COLOR:
        {
            if( pv_param2==NULL)
            {
                switch(WGL_POINTER_TO_UINT8(pv_param1))
                {
                    case WGL_CLR_BK:
                        _lbcntdef_get_sys_lb_elem_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_elem_bk);
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }
            else
            {
                switch(WGL_POINTER_TO_UINT8(pv_param1))
                {
                    case WGL_CLR_BK:
                        _LBE_COLOR_SET_COPY(&pt_ctx->t_clrs_elem_bk, pv_param2);
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }
        }
        break;

        case WGL_CMD_LB_GET_ELEM_COLOR:
        {
            WGL_LB_COLOR_ELEM_SET_T *pt_clrs_dst;
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
                
            switch(WGL_POINTER_TO_UINT8(pv_param1))
            {
                case WGL_CLR_BK:
                    pt_clrs_dst = &pt_ctx->t_clrs_elem_bk;
                    break;
                default:
                    return WGLR_INV_ARG;
            }

            _LBE_COLOR_SET_COPY(pv_param2, pt_clrs_dst);
            
        }
        break;

        case WGL_CMD_LB_GET_VISIBLE_ELEM_NUM:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);

            if( _LBCNTDEF_ELEMS_EMPTY(pt_ctx) )
            {
                *((UINT16 *)pv_param1) = 0;
            }
            else
            {
                UINT16  ui2_first, ui2_last;
                CATCH_FAIL(wgl_vp_get_visible_range(&pt_ctx->t_vp, &ui2_first, &ui2_last));
                *((UINT16 *)pv_param1) = ui2_last - ui2_first + (UINT16)1;
            }

        }
        break;

        case WGL_CMD_LB_GET_FIRST_VISIBLE_INDEX:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);

            if( _LBCNTDEF_ELEMS_EMPTY(pt_ctx) )
            {
                *((UINT16 *)pv_param1) = WGL_LB_NULL_INDEX;
            }
            else
            {
                UINT16  ui2_first, ui2_last;
                CATCH_FAIL(wgl_vp_get_visible_range(&pt_ctx->t_vp, &ui2_first, &ui2_last));
                *((UINT16 *)pv_param1) = ui2_first;
            }
            
        }
        break;

        case WGL_CMD_LB_GET_LAST_VISIBLE_INDEX:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);

            if( _LBCNTDEF_ELEMS_EMPTY(pt_ctx) )
            {
                *((UINT16 *)pv_param1) = WGL_LB_NULL_INDEX;
            }
            else
            {
                UINT16  ui2_first, ui2_last;
                CATCH_FAIL(wgl_vp_get_visible_range(&pt_ctx->t_vp, &ui2_first, &ui2_last));
                *((UINT16 *)pv_param1) = ui2_last;
            }
        }
        break;

        case WGL_CMD_LB_SET_COL_WIDTH:
        {
            UINT8       ui1_idx       = WGL_POINTER_TO_UINT8(pv_param1);
            UINT16      ui2_col_width = WGL_POINTER_TO_UINT16(pv_param2);
            
            if( ui1_idx >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(!CHECK_VALID_COL_WIDTH (ui2_col_width))
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            pt_ctx->at_cnt_cols[ui1_idx].ui2_col_width = ui2_col_width;
        }
        break;

        case WGL_CMD_LB_GET_COL_WIDTH:
        {
            UINT8 ui1_idx = WGL_POINTER_TO_UINT8(pv_param1);
            
            if( ui1_idx >= pt_ctx->ui1_col_num || pv_param2==NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            *((UINT16 *)pv_param2) = pt_ctx->at_cnt_cols[ui1_idx].ui2_col_width;
        }
        break;

        case WGL_CMD_LB_SET_COL_INSET:
        {
            UINT8       ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            
            if(ui1_c >= pt_ctx->ui1_col_num)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(pv_param2==NULL)
            {
                _INSET_COPY( &pt_ctx->at_cnt_cols[ui1_c].t_inset, &LBCNTDEF_DEF_COL_INSET);
            }
            else
            {
                WGL_INSET_T *pt_inset = (WGL_INSET_T *)pv_param2;
                
                if(CHECK_VALID_INSET(pt_inset))
                {
                    _INSET_COPY( &pt_ctx->at_cnt_cols[ui1_c].t_inset, pt_inset);
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                    break;
                }
            }
        }
        break;

        case WGL_CMD_LB_GET_COL_INSET:
        {
            UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            if(ui1_c>=pt_ctx->ui1_col_num)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            _INSET_COPY(pv_param2, &pt_ctx->at_cnt_cols[ui1_c].t_inset);

        }
        break;

        case WGL_CMD_LB_SET_COL_ALIGN:
        {
            UINT8 ui1_c     = WGL_POINTER_TO_UINT8(pv_param1);
            UINT8 ui1_align = WGL_POINTER_TO_UINT8(pv_param2);
            if(ui1_c>=pt_ctx->ui1_col_num || !WGL_IS_VALID_ALIGN( ui1_align))
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            pt_ctx->at_cnt_cols[ui1_c].ui1_align = ui1_align;

        }
        break;

        case WGL_CMD_LB_GET_COL_ALIGN:
        {
            UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            if( ui1_c>=pt_ctx->ui1_col_num || pv_param2==NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            *((UINT8 *)pv_param2) = pt_ctx->at_cnt_cols[ui1_c].ui1_align;
        }
        break;

        case WGL_CMD_LB_SET_COL_COLOR:
        {
            UINT8 ui1_c      = WGL_GET_HI_LOW_BYTE(pv_param1);
            UINT8 ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            
            if( ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(ui1_target==WGL_CLR_TEXT && !IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }

            if( pv_param2==NULL )
            { /*use the color set shared by all elements: the element color set*/
                switch(ui1_target)
                {
                    case WGL_CLR_BK:
                        _LBE_COLOR_SET_COPY(&pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk, &pt_ctx->t_clrs_elem_bk);
                        break;
                    case WGL_CLR_TEXT:
                        _lbcntdef_get_sys_lb_elem_text_color_set(&pt_ctx->t_cvsst, &pt_ctx->at_cnt_cols[ui1_c].t_clrs_text);
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }
            else
            {
                switch(ui1_target)
                {
                    case WGL_CLR_BK:
                        _LBE_COLOR_SET_COPY(&pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk, pv_param2);
                        break;
                    case WGL_CLR_TEXT:
                        _LBE_COLOR_SET_COPY(&pt_ctx->at_cnt_cols[ui1_c].t_clrs_text, pv_param2);
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }
        }
        break;

        case WGL_CMD_LB_GET_COL_COLOR:
        {
            UINT8 ui1_c      = WGL_GET_HI_LOW_BYTE(pv_param1);
            UINT8 ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            
            if( ui1_c >= pt_ctx->ui1_col_num || pv_param2 == NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(ui1_target == WGL_CLR_TEXT && !IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            switch(ui1_target)
            {
                case WGL_CLR_BK:
                    _LBE_COLOR_SET_COPY(pv_param2, &pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk);
                    break;
                case WGL_CLR_TEXT:
                    _LBE_COLOR_SET_COPY(pv_param2, &pt_ctx->at_cnt_cols[ui1_c].t_clrs_text);
                    break;
                default:
                    return WGLR_INV_ARG;
            }
        }
        break;
        
        case WGL_CMD_LB_SET_COL_BK_IMAGE:
        {
            UINT8                   ui1_c      = WGL_POINTER_TO_UINT8 (pv_param1);
            WGL_LB_IMG_ELEM_SET_T*  pt_imgs_bk = (WGL_LB_IMG_ELEM_SET_T *) pv_param2;
            
            if( ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if (pt_imgs_bk)
            {
                if (_lbcntdef_tool_check_valid_elem_image_set (pt_imgs_bk))
                {
                    _LBE_IMG_SET_COPY(&pt_ctx->at_cnt_cols[ui1_c].t_imgs_bk, pt_imgs_bk);
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                    break;
                }
            }
            else
            {
                /* clear bk image of this column */
                x_memset (&pt_ctx->at_cnt_cols[ui1_c].t_imgs_bk, 0, sizeof (WGL_LB_IMG_ELEM_SET_T));
            }
        }
        break;
        
        case WGL_CMD_LB_GET_COL_BK_IMAGE:
        {
            UINT8                   ui1_c          = WGL_POINTER_TO_UINT8 (pv_param1);
            WGL_LB_IMG_ELEM_SET_T*  pt_imgs_bk_out = (WGL_LB_IMG_ELEM_SET_T *) pv_param2;
            
            if( ui1_c >= pt_ctx->ui1_col_num || 
                pt_imgs_bk_out == NULL )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            _LBE_IMG_SET_COPY (pt_imgs_bk_out, &pt_ctx->at_cnt_cols[ui1_c].t_imgs_bk);
        }   
        break;

        case WGL_CMD_LB_SET_COL_FONT:
        {
            UINT8               ui1_c         = WGL_POINTER_TO_UINT8(pv_param1);
            WGL_FONT_INFO_T     *pt_font_info = (WGL_FONT_INFO_T *)pv_param2;
            
            if( ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_set_col_font( pt_ctx, pt_font_info, &pt_ctx->at_cnt_cols[ui1_c].h_font, ui1_c);
        }
        break;

        case WGL_CMD_LB_GET_COL_FONT:
        {
            UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            if( ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = wgl_frlm_get_info(&pt_ctx->t_frlm, pt_ctx->at_cnt_cols[ui1_c].h_font, (WGL_FONT_INFO_T *)pv_param2);
        }
        break;

        case WGL_CMD_LB_SET_COL_FONT_EX:
        {
            UINT8   ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            WGL_LB_FONT_INFO_EX_T   *pt_ex_lb_font_info = (WGL_LB_FONT_INFO_EX_T *)pv_param2;
            if( ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_set_col_font_ex( pt_ctx, pt_ex_lb_font_info, &pt_ctx->at_cnt_cols[ui1_c].h_font, ui1_c);
        }
        break;
        
        case WGL_CMD_LB_GET_COL_FONT_EX:
        {
            UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            if( ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_get_col_font_ex( pt_ctx, (WGL_LB_FONT_INFO_EX_T *)pv_param2, ui1_c);
        }
        break;
        
        case WGL_CMD_LB_SET_COL_FIX_TEXT_DIR:
        {
           UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
           
           WS_ASSERT(pv_param2==NULL);
           
           if( ui1_c >= pt_ctx->ui1_col_num )
           {
               i4_ret_val = WGLR_INV_ARG;
               break;
           }
           if(!IS_COL_TEXT(pt_ctx, ui1_c))
           {
               i4_ret_val = WGLR_INV_CMD;
               break;
           }
           pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir = TRUE;
           
        }
        break;
        case WGL_CMD_LB_SET_COL_FIX_TEXT_DIR_ALIGN:
        {
           UINT8 ui1_c = WGL_POINTER_TO_UINT8(pv_param1);
           
           WS_ASSERT(pv_param2==NULL);
           
           if( ui1_c >= pt_ctx->ui1_col_num )
           {
               i4_ret_val = WGLR_INV_ARG;
               break;
           }
           if(!IS_COL_TEXT(pt_ctx, ui1_c))
           {
               i4_ret_val = WGLR_INV_CMD;
               break;
           }
           pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir_align= TRUE;
           
        }
        break;

        case WGL_CMD_LB_SET_IS_ITEM_USE_DEF:
        {
            UINT16              ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            UINT8               ui1_c   = WGL_POINTER_TO_UINT8(pv_param2);
            LB_ELEM_T           *pt_elem;
            LBCNTDEF_ELEM_T     *pt_cnt_elem;

            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {
                /*It is not in the cahced range, do nothing*/
                i4_ret_val = WGLR_OK;
            }
            
            if(pt_cnt_elem->at_items[ui1_c].pt_fmt)
            {
                i4_ret = wgl_npm_free (&pt_ctx->t_npm_itm_fmt, (VOID *) pt_cnt_elem->at_items[ui1_c].pt_fmt);
                
                if (i4_ret != WGLR_OK)
                {
                    INFORM_ERR (i4_ret);
                }
                
                pt_cnt_elem->at_items[ui1_c].pt_fmt = NULL;
            }

        }
        break;

        case WGL_CMD_LB_GET_IS_ITEM_USE_DEF:
        {
            UINT16              ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8               ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T           *pt_elem;
            LBCNTDEF_ELEM_T     *pt_cnt_elem;
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c >= pt_ctx->ui1_col_num || pv_param2 == NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {
                /*It is not in the cahced range, false now*/
                *((BOOL *)pv_param2) = FALSE;
            }
            else
            {
                *((BOOL *)pv_param2) = (pt_cnt_elem->at_items[ui1_c].pt_fmt==NULL) ? TRUE : FALSE;
            }

        }
        break;

        case WGL_CMD_LB_SET_ITEM_ALIGN:
        {
            UINT16      ui2_idx   = WGL_GET_HI_WORD(pv_param1);
            UINT8       ui1_c     = WGL_GET_LOW_LOW_BYTE(pv_param1);
            UINT8       ui1_align = WGL_POINTER_TO_UINT8(pv_param2);

            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || 
                ui1_c>=pt_ctx->ui1_col_num || 
                !WGL_IS_VALID_ALIGN( ui1_align))
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            CATCH_FAIL(_lbcntdef_set_item_attr(pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_ALIGN, (VOID *)(UINT32)ui1_align));
            
        }
        break;

        case WGL_CMD_LB_GET_ITEM_ALIGN:
        {
            UINT16              ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8               ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            LB_ELEM_T           *pt_elem;
            LBCNTDEF_ELEM_T     *pt_cnt_elem;

            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num || pv_param2==NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);

            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {/*The idx is out of the range*/
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_ALIGN, pv_param2 );

        }
        break;

        case WGL_CMD_LB_SET_ITEM_COLOR:
        {
            UINT16                      ui2_idx    = WGL_GET_HI_WORD(pv_param1);
            UINT8                       ui1_c      = WGL_GET_LOW_HI_BYTE(pv_param1);
            UINT8                       ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            WGL_LB_COLOR_ELEM_SET_T     *pt_clrs;
            UINT16                      ui2_attr_id;
            
            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(ui1_target==WGL_CLR_TEXT && !IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            if(pv_param2==NULL)
            {
                switch(ui1_target)
                {
                    case WGL_CLR_BK:
                        pt_clrs = &pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk;
                        ui2_attr_id = ITEM_ATTR_BK_COLOR;
                        break;
                    case WGL_CLR_TEXT:
                        pt_clrs = &pt_ctx->at_cnt_cols[ui1_c].t_clrs_text;
                        ui2_attr_id = ITEM_ATTR_TEXT_COLOR;
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }
            else
            {
                pt_clrs = (WGL_LB_COLOR_ELEM_SET_T *) pv_param2;
                switch(ui1_target)
                {
                    case WGL_CLR_BK:
                        ui2_attr_id = ITEM_ATTR_BK_COLOR;
                        break;
                    case WGL_CLR_TEXT:
                        ui2_attr_id = ITEM_ATTR_TEXT_COLOR;                    
                        break;
                    default:
                        return WGLR_INV_ARG;
                }
            }

            CATCH_FAIL(_lbcntdef_set_item_attr(pt_ctx, ui2_idx, ui1_c, ui2_attr_id, (VOID *)pt_clrs));
        }
        break;

        case WGL_CMD_LB_GET_ITEM_COLOR:
        {
            UINT16                      ui2_idx    = WGL_GET_HI_WORD(pv_param1);
            UINT8                       ui1_c      = WGL_GET_LOW_HI_BYTE(pv_param1);
            UINT8                       ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            UINT16                      ui2_attr_id;
            LB_ELEM_T                   *pt_elem;
            LBCNTDEF_ELEM_T             *pt_cnt_elem;
            WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_tmp;
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(ui1_target==WGL_CLR_TEXT && !IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            switch(ui1_target) 
            {
                case WGL_CLR_BK:
                    ui2_attr_id = ITEM_ATTR_BK_COLOR;
                    break;
                case WGL_CLR_TEXT:
                    ui2_attr_id = ITEM_ATTR_TEXT_COLOR;
                    break;
                default:
                    return WGLR_INV_ARG;
            }
            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {/*The idx is out of the range*/
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ui2_attr_id, (VOID *)&pt_clrs_tmp );
            _LBE_COLOR_SET_COPY(pv_param2, pt_clrs_tmp);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_BK_IMAGE:
        {
            UINT16                      ui2_idx     = WGL_GET_HI_WORD(pv_param1);
            UINT8                       ui1_c       = (UINT8) WGL_GET_LOW_WORD(pv_param1);
            WGL_LB_IMG_ELEM_SET_T       *pt_imgs_bk = (WGL_LB_IMG_ELEM_SET_T *) pv_param2;
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if (pt_imgs_bk)
            {
                if (_lbcntdef_tool_check_valid_elem_image_set (pt_imgs_bk))
                {
                    i4_ret_val = _lbcntdef_set_item_attr (
                            pt_ctx,
                            ui2_idx,
                            ui1_c,
                            ITEM_ATTR_BK_IMAGE,
                            (VOID *) pt_imgs_bk );
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                    break;
                }
            }
            else
            {
                /* clear the image bk image set */
                i4_ret_val = _lbcntdef_set_item_attr (
                        pt_ctx,
                        ui2_idx,
                        ui1_c,
                        ITEM_ATTR_BK_IMAGE,
                        (VOID *) NULL );
            }
            
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_BK_IMAGE:
        {
            UINT16                      ui2_idx         = WGL_GET_HI_WORD(pv_param1);
            UINT8                       ui1_c           = (UINT8) WGL_GET_LOW_WORD(pv_param1);
            WGL_LB_IMG_ELEM_SET_T       *pt_imgs_bk_out = (WGL_LB_IMG_ELEM_SET_T *) pv_param2;
            LB_ELEM_T                   *pt_elem;
            LBCNTDEF_ELEM_T             *pt_cnt_elem;
            WGL_LB_IMG_ELEM_SET_T       *pt_imgs_bk;
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || 
                ui1_c >= pt_ctx->ui1_col_num    ||
                pt_imgs_bk_out == NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {/*The idx is out of the range*/
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_BK_IMAGE, (VOID *) &pt_imgs_bk);
            _LBE_IMG_SET_COPY(pt_imgs_bk_out, pt_imgs_bk);
        }
        break;

        case WGL_CMD_LB_SET_ITEM_FONT:
        {
            UINT16              ui2_idx       = WGL_GET_HI_WORD(pv_param1);
            UINT8               ui1_c         = WGL_GET_LOW_LOW_BYTE(pv_param1);
            WGL_FONT_INFO_T     *pt_font_info = (WGL_FONT_INFO_T *)pv_param2;
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx)  || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }

            CATCH_FAIL(_lbcntdef_set_item_attr(pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT, (VOID *)pt_font_info));
        }
        break;

        case WGL_CMD_LB_GET_ITEM_FONT:
        {
            /*TODO: check if this item is a text-item*/
            UINT16              ui2_idx           = WGL_GET_HI_WORD(pv_param1);
            UINT8               ui1_c             = WGL_GET_LOW_LOW_BYTE(pv_param1);
            WGL_FONT_INFO_T     *pt_font_info_out = (WGL_FONT_INFO_T *)pv_param2;
            LB_ELEM_T           *pt_elem;
            LBCNTDEF_ELEM_T     *pt_cnt_elem;
            HFONT_T             h_font_out;
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {/*The idx is out of the range*/
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_FONT, (VOID *)&h_font_out );
            i4_ret_val = wgl_frlm_get_info(&pt_ctx->t_frlm, h_font_out, pt_font_info_out);
        }
        break;

        case WGL_CMD_LB_SET_ITEM_FONT_EX:
        {
            UINT16  ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8   ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            WGL_LB_FONT_INFO_EX_T   *pt_ex_lb_font_info = (WGL_LB_FONT_INFO_EX_T *)pv_param2;
            
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }

            CATCH_FAIL(_lbcntdef_set_item_attr(pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT_EX, (VOID *)pt_ex_lb_font_info));          
        }
        break;

        case WGL_CMD_LB_GET_ITEM_FONT_EX:
        {
            /*TODO: check if this item is a text-item*/
            LB_ELEM_T                 *pt_elem;
            LBCNTDEF_ELEM_T           *pt_cnt_elem;
            UINT16                    ui2_idx           = WGL_GET_HI_WORD(pv_param1);
            UINT8                     ui1_c             = WGL_GET_LOW_LOW_BYTE(pv_param1);
 
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            if( ui2_idx>=_LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c>=pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
            if(pt_elem == NULL || pt_cnt_elem == NULL)
            {/*The idx is out of the range*/
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_FONT_EX, pv_param2 );
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_ALIGN_USE_DEF:
        {
            UINT16   ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            
            if( ui2_idx >= _LBCNTDEF_GET_ELEMS_NUM(pt_ctx) || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            i4_ret_val = _lbcntdef_set_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_ALIGN);
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_ALIGN_USE_DEF:
        {
            UINT16   ui2_idx     = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c       = WGL_GET_LOW_LOW_BYTE(pv_param1);
            BOOL     *pb_use_def = (BOOL *) pv_param2;

            if ( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num || pb_use_def == NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            i4_ret_val = _lbcntdef_get_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_ALIGN, pb_use_def);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_COLOR_USE_DEF:
        {
            UINT16   ui2_idx    = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c      = WGL_GET_LOW_HI_BYTE(pv_param1);
            UINT8    ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            UINT16   ui2_attr_id;

            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            switch(ui1_target) 
            {
                case WGL_CLR_BK:
                    ui2_attr_id = ITEM_ATTR_BK_COLOR;
                    break;
                    
                case WGL_CLR_TEXT:
                    if (IS_COL_TEXT (pt_ctx, ui1_c))
                    {
                        ui2_attr_id = ITEM_ATTR_TEXT_COLOR;
                    }
                    else
                    {
                        i4_ret_val = WGLR_INV_CMD;
                    }
                    break;
                    
                default:
                    i4_ret_val = WGLR_INV_ARG;
            }
            
            if (i4_ret_val != WGLR_OK) 
            {
                break;
            }

            i4_ret_val = _lbcntdef_set_item_use_def (pt_ctx, ui2_idx, ui1_c, ui2_attr_id);
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_COLOR_USE_DEF:
        {
            UINT16   ui2_idx    = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c      = WGL_GET_LOW_HI_BYTE(pv_param1);
            UINT8    ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
            UINT16   ui2_attr_id;
            BOOL     *pb_use_def = (BOOL *) pv_param2;
            
            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num || pb_use_def == NULL)
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            switch(ui1_target) 
            {
                case WGL_CLR_BK:
                    ui2_attr_id = ITEM_ATTR_BK_COLOR;
                    break;
                    
                case WGL_CLR_TEXT:
                    if (IS_COL_TEXT (pt_ctx, ui1_c))
                    {
                        ui2_attr_id = ITEM_ATTR_TEXT_COLOR;
                    }
                    else
                    {
                        i4_ret_val = WGLR_INV_CMD;
                    }
                    break;
                    
                default:
                    i4_ret_val = WGLR_INV_ARG;
            }
            
            if (i4_ret_val != WGLR_OK) 
            {
                break;
            }
            
            i4_ret_val = _lbcntdef_get_item_use_def (pt_ctx, ui2_idx, ui1_c, ui2_attr_id, pb_use_def);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_BK_IMAGE_USE_DEF:
        {
            UINT16   ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);

            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            i4_ret_val = _lbcntdef_set_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_BK_IMAGE);
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_BK_IMAGE_USE_DEF:
        {
            UINT16   ui2_idx     = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c       = WGL_GET_LOW_LOW_BYTE(pv_param1);
            BOOL     *pb_use_def = (BOOL *) pv_param2;

            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num || pb_use_def == NULL )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            i4_ret_val = _lbcntdef_get_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_BK_IMAGE, pb_use_def);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_FONT_USE_DEF:
        {
            UINT16   ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            
            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_set_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT);
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_FONT_USE_DEF:
        {
            UINT16   ui2_idx     = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c       = WGL_GET_LOW_LOW_BYTE(pv_param1);
            BOOL     *pb_use_def = (BOOL *) pv_param2;
            
            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num || pb_use_def == NULL )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_get_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT, pb_use_def);
        }
        break;
        
        case WGL_CMD_LB_SET_ITEM_FONT_EX_USE_DEF:
        {
            UINT16   ui2_idx = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c   = WGL_GET_LOW_LOW_BYTE(pv_param1);
            
            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_set_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT_EX);
        }
        break;
        
        case WGL_CMD_LB_GET_ITEM_FONT_EX_USE_DEF:
        {
            UINT16   ui2_idx     = WGL_GET_HI_WORD(pv_param1);
            UINT8    ui1_c       = WGL_GET_LOW_LOW_BYTE(pv_param1);
            BOOL     *pb_use_def = (BOOL *) pv_param2;
            
            if( ui2_idx >= pt_ctx->ui2_elem_num || ui1_c >= pt_ctx->ui1_col_num || pb_use_def == NULL )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(!IS_COL_TEXT(pt_ctx, ui1_c))
            {
                i4_ret_val = WGLR_INV_CMD;
                break;
            }
            
            i4_ret_val = _lbcntdef_get_item_use_def (pt_ctx, ui2_idx, ui1_c, ITEM_ATTR_FONT_EX, pb_use_def);
        }
        break;

        /*List Box Specific Commands*/
        case WGL_CMD_LB_EXT_SET_CNT_STYLE:
        {
            UINT32      ui4_cnt_style = (UINT32)pv_param1;
            
            pt_ctx->ui4_cnt_style = ui4_cnt_style;
        }
        break;

        case WGL_CMD_LB_EXT_GET_CNT_STYLE:
        {
            UINT32      *pui4_cnt_style = (UINT32 *)pv_param1;
            CHECK_NULL(pui4_cnt_style, WGLR_INV_ARG);
            *pui4_cnt_style = pt_ctx->ui4_cnt_style;
        }
        break;
        
        case WGL_CMD_LB_EXT_SET_ELEM_BK_IMAGE:
        {
           WGL_LB_IMG_ELEM_SET_T *pt_imgs_elem = (WGL_LB_IMG_ELEM_SET_T *)pv_param1;
           if(pt_imgs_elem==NULL)
           {
                _lbcntdef_get_sys_lb_elem_bk_img_set(&pt_ctx->t_cvsst, &pt_ctx->t_imgs_elem_bk);
           }
           else
           {
                if(_lbcntdef_tool_check_valid_elem_image_set(pt_imgs_elem))
                {
                    _LBE_IMG_SET_COPY(&pt_ctx->t_imgs_elem_bk, pt_imgs_elem);
                }
                else
                {
                    i4_ret_val = WGLR_INV_ARG;
                }
           }
        }
        break;

        case WGL_CMD_LB_EXT_GET_ELEM_BK_IMAGE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _LBE_IMG_SET_COPY(pv_param1, &pt_ctx->t_imgs_elem_bk);
        }
        break;

        case WGL_CMD_LB_EXT_SET_SCROLL_IMAGE:
        {
            WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs   = (WGL_LB_SCROLL_IMG_INFO_T *)pv_param1;
            BOOL                        b_need_repaint = WGL_POINTER_TO_BOOL(pv_param2);
            WGL_LB_SCROLL_IMG_INFO_T    t_scr_imgs_orig;

            _SCR_IMG_INFO_COPY(&t_scr_imgs_orig, &pt_ctx->t_scr_imgs);
            if(pt_scr_imgs==NULL)
            {
                _SCR_IMG_INFO_CLEAR(pt_ctx->t_scr_imgs);
            }
            else
            {
                if(_lbcntdef_tool_check_valid_scroll_image_info(pt_scr_imgs))
                {
                    _SCR_IMG_INFO_COPY(&pt_ctx->t_scr_imgs, pt_scr_imgs);
                }
                else
                {
                    return WGLR_INV_ARG;
                }
            }
            if(b_need_repaint)
            {
                _lbcntdef_add_update_region_by_diff_scrl_imgs(
                    &pt_ctx->t_pur_wait_for_update, 
                    &t_scr_imgs_orig, 
                    &pt_ctx->t_scr_imgs);
                    
                i4_ret_val = WGL_LBR_NEED_REPAINT_PARTIAL;
            }
        }
        break;
        
        case WGL_CMD_LB_EXT_GET_SCROLL_IMAGE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _SCR_IMG_INFO_COPY(pv_param1, &pt_ctx->t_scr_imgs);
        }
        break;

        case WGL_CMD_LB_EXT_SET_PAGING_IMAGE:
        {
            WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs   = (WGL_LB_SCROLL_IMG_INFO_T *)pv_param1;
            BOOL                        b_need_repaint = WGL_POINTER_TO_BOOL(pv_param2);
            WGL_LB_SCROLL_IMG_INFO_T    t_scr_imgs_orig;

            _SCR_IMG_INFO_COPY(&t_scr_imgs_orig, &pt_ctx->t_scr_imgs_paging);
            if(pt_scr_imgs==NULL)
            {
                _SCR_IMG_INFO_CLEAR(pt_ctx->t_scr_imgs_paging);
            }
            else
            {
                if(_lbcntdef_tool_check_valid_scroll_image_info(pt_scr_imgs))
                {
                    _SCR_IMG_INFO_COPY(&pt_ctx->t_scr_imgs_paging, pt_scr_imgs);
                }
                else
                {
                    return WGLR_INV_ARG;
                }
            }
            
            _lbcntdef_add_update_region_by_diff_scrl_imgs(
                &pt_ctx->t_pur_wait_for_update, 
                &t_scr_imgs_orig, 
                &pt_ctx->t_scr_imgs_paging );
                
            i4_ret_val = b_need_repaint ? WGL_LBR_NEED_REPAINT_PARTIAL : WGLR_OK;
        }
        break;
        
        case WGL_CMD_LB_EXT_GET_PAGING_IMAGE:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _SCR_IMG_INFO_COPY(pv_param1, &pt_ctx->t_scr_imgs_paging);
        }
        break;
        
        case WGL_CMD_LB_EXT_SET_HLT_ELEM_EFFECT:  /* pv_param1: (WGL_LB_HLT_ELEM_EFFECT_T *),  pv_param2: NULL */
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_cmd_hdr_set_hlt_elem_effect(pt_ctx, (WGL_LB_HLT_ELEM_EFFECT_T *)pv_param1);
        }
        break;
        
        case WGL_CMD_LB_EXT_GET_HLT_ELEM_EFFECT:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_cmd_hdr_get_hlt_elem_effect(pt_ctx, (WGL_LB_HLT_ELEM_EFFECT_T *)pv_param1);
        }
        break;
        
        case WGL_CMD_LB_EXT_SET_SCRL_BAR:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_cmd_hdr_set_scrl_bar(pt_ctx, (WGL_LB_SCRL_BAR_T *)pv_param1);
        }
        break;
        
        case WGL_CMD_LB_EXT_GET_SCRL_BAR:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            x_memcpy(pv_param1, &pt_ctx->t_attr_sb, sizeof(WGL_LB_SCRL_BAR_T));
        }
        break;
        
        case WGL_CMD_LB_EXT_CHG_SCRL_BAR_STATUS:
        {
            WGL_LB_SCRL_BAR_STATUS_T*   pt_sb_status = (WGL_LB_SCRL_BAR_STATUS_T *) pv_param1;
            
            CHECK_NULL (pt_sb_status, WGLR_INV_ARG);
            
            i4_ret_val = wgl_lb_sb_set_status (&pt_ctx->t_ctx_sb, pt_sb_status->ui4_size_vp, pt_sb_status->ui4_size_total, pt_sb_status->ui4_pos_vp_top);
            
            /* [Note] don't have to prepare PUR because this command imply a viewport moved event, */
            /*        this event will usually followed by a full repaint to make sure the visual correctness */
        }
        break;

        /*Static Internal Content Commands*/
        case WGL_CNT_LB_GET_ELEM_CNT_CTX_SIZE: /* pv_param1: (UINT8)column-number   pv_param2:(UINT32 *)content-element-size */
        {
            UINT8   ui1_col_num = WGL_POINTER_TO_UINT8(pv_param1);
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            *((UINT32 *)pv_param2) = (UINT32) (WGL_ALIGN_4(sizeof(LBCNTDEF_ELEM_T)) + sizeof(LBCNTDEF_ITEM_T)*ui1_col_num ) ;
        }
        break;

        /*Internal Content Commands*/
        case WGL_CNT_LB_SET_TEXT_DIRECTION:
        {
            i4_ret_val = _lbcntdef_set_text_dir(pt_ctx, WGL_POINTER_TO_UINT8(pv_param1));
        }
        break;
        
        case WGL_CNT_LB_GET_TEXT_DIRECTION:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_get_text_dir(pt_ctx, (UINT32*)pv_param1);
        }
        break;
        
        case WGL_CNT_LB_SET_STATE:
        {        
            pt_ctx->ui1_state = WGL_POINTER_TO_UINT8(pv_param1);
            /*update font*/
            i4_ret_val = _lbcntdef_update_font(pt_ctx, &pt_ctx->t_font_info_ex, &pt_ctx->h_font);
        }
        break;
            
        case WGL_CNT_LB_ADD_ELEM: /*pv_param1: element-index, pv_param2: element-pointer*/
        { 
            LB_ELEM_T *pt_elem = (LB_ELEM_T *)pv_param2;
            LB_ELEM_INF_CONTENT*   pt_content = (LB_ELEM_INF_CONTENT*)pv_param1;
            
            i4_ret_val = _lbcntdef_add_elem(pt_ctx, pt_elem, TRUE);
            if( !WGL_IS_FAIL(i4_ret_val) && pt_content )
            {
                pt_ctx->pt_elems = pt_content->pt_elems;
                _LBCNTDEF_SET_CACHE_START(pt_ctx, pt_content->ui2_cache_start);   
                _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, pt_content->ui2_cache_num);
                pt_ctx->pt_elem_inf_list = pt_content->pt_elem_inf_list;
            }
        }
        break;

        case WGL_CNT_LB_DEL_ELEM: /*pv_param1: element-list-pointer, pv_param2: element-pointer*/
        {
            LB_ELEM_T *pt_elem = (LB_ELEM_T *)pv_param2;
            LB_ELEM_INF_CONTENT*   pt_content = (LB_ELEM_INF_CONTENT*)pv_param1;

            i4_ret_val = _lbcntdef_del_elem(pt_ctx, pt_elem, TRUE);
            if(!WGL_IS_FAIL(i4_ret_val))
            {
                pt_ctx->pt_elems = pt_content->pt_elems;
                _LBCNTDEF_SET_CACHE_START(pt_ctx, pt_content->ui2_cache_start);   
                _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, pt_content->ui2_cache_num);
                pt_ctx->pt_elem_inf_list = pt_content->pt_elem_inf_list;
            }
        }
        break;
        
        case WGL_CNT_LB_CREATE_ELEM: 
        { 
            LB_ELEM_T *pt_elem = (LB_ELEM_T *)pv_param2;
            LB_ELEM_INF_CONTENT*   pt_content = (LB_ELEM_INF_CONTENT*)pv_param1;
            
            i4_ret_val = _lbcntdef_add_elem(pt_ctx, pt_elem, FALSE);
            if(!WGL_IS_FAIL(i4_ret_val))
            {
                pt_ctx->pt_elems = pt_content->pt_elems;
                _LBCNTDEF_SET_CACHE_START(pt_ctx, pt_content->ui2_cache_start);   
                _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, pt_content->ui2_cache_num);
                pt_ctx->pt_elem_inf_list = pt_content->pt_elem_inf_list;
            }
        }
        break;

        case WGL_CNT_LB_FREE_ELEM: 
        {
            LB_ELEM_T *pt_elem = (LB_ELEM_T *)pv_param2;
            LB_ELEM_INF_CONTENT*   pt_content = (LB_ELEM_INF_CONTENT*)pv_param1;

            i4_ret_val = _lbcntdef_del_elem(pt_ctx, pt_elem, FALSE );
            if(!WGL_IS_FAIL(i4_ret_val))
            {
                pt_ctx->pt_elems = pt_content->pt_elems;
                _LBCNTDEF_SET_CACHE_START(pt_ctx, pt_content->ui2_cache_start);   
                _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, pt_content->ui2_cache_num);
                pt_ctx->pt_elem_inf_list = pt_content->pt_elem_inf_list;
            }
        }
        break;

        case WGL_CNT_LB_ENSURE_ELEM_VISIBLE_PREFER_CENTER:
        {
            CHECK_VALID(WGL_POINTER_TO_UINT16(pv_param1)<pt_ctx->ui2_elem_num, WGLR_INV_ARG);
            CATCH_FAIL(wgl_vp_ensure_visible_prefer_center(&pt_ctx->t_vp, WGL_POINTER_TO_UINT16(pv_param1) ));
        }
        break;

        case WGL_CNT_LB_ENSURE_ELEM_VISIBLE_MIN_MOVE: /*pv_param1:[IN] LB_CNT_EEVMM_PARAM_T *, pv_param2:[OUT] *pb_is_vp_scrolled*/
        {
            LB_CNT_EEVMM_PARAM_T *pt_param = (LB_CNT_EEVMM_PARAM_T *)pv_param1;
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            CHECK_VALID(pt_param->ui2_idx < pt_ctx->ui2_elem_num, WGLR_INV_ARG);
            CATCH_FAIL(wgl_vp_check_visibility_minimal_move(&pt_ctx->t_vp, 
                                                pt_param->ui2_idx, 
                                                pt_param->ui2_margin, 
                                                TRUE, 
                                                (BOOL *)pv_param2,
                                                NULL));
        }
        break;
        
        case WGL_CNT_LB_ENSURE_ELEM_VISIBLE_NO_SCROLL: /*pv_param1:[IN] ui2_idx, pv_param2:[OUT] *pb_is_vp_scrolled*/
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            CHECK_VALID(ui2_idx<pt_ctx->ui2_elem_num, WGLR_INV_ARG );
            CATCH_FAIL(wgl_vp_check_visibility_no_scroll(&pt_ctx->t_vp,ui2_idx,TRUE,(BOOL *)pv_param2,NULL));
        }
        break;
        
        case WGL_CNT_LB_CHECK_ELEM_VISIBILITY_MIN_MOVE: /*pv_param1:[IN] LB_CNT_EEVMM_PARAM_T *, pv_param2:[IN/OUT] LB_CNT_CEV_PARAM_T * */
        {
            LB_CNT_EEVMM_PARAM_T *pt_param = (LB_CNT_EEVMM_PARAM_T *)pv_param1;
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            CHECK_VALID(pt_param->ui2_idx < pt_ctx->ui2_elem_num, WGLR_INV_ARG);
            CATCH_FAIL(wgl_vp_check_visibility_minimal_move(
                &pt_ctx->t_vp, 
                pt_param->ui2_idx, 
                pt_param->ui2_margin, 
                ((LB_CNT_CEV_PARAM_T *)pv_param2)->b_ensure_elem_visible, 
                &((LB_CNT_CEV_PARAM_T *)pv_param2)->b_is_vp_scrolled,
                NULL));
        }
        break;
        
        case WGL_CNT_LB_CHECK_ELEM_VISIBILITY_NO_SCROLL: /*pv_param1:[IN] ui2_idx, pv_param2:[IN/OUT] LB_CNT_CEV_PARAM_T * */
        {
            UINT16 ui2_idx = WGL_POINTER_TO_UINT16(pv_param1);
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            CHECK_VALID(ui2_idx<pt_ctx->ui2_elem_num, WGLR_INV_ARG );
            CATCH_FAIL(wgl_vp_check_visibility_no_scroll(
                &pt_ctx->t_vp,
                ui2_idx, 
                ((LB_CNT_CEV_PARAM_T *)pv_param2)->b_ensure_elem_visible,
                &((LB_CNT_CEV_PARAM_T *)pv_param2)->b_is_vp_scrolled,
                NULL));
        }
        break;

        case WGL_CNT_LB_SHOW_LAST_PAGE: /*pv_param1:[OUT] *pb_is_vp_scrolled*/
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            CATCH_FAIL(wgl_vp_show_last_page(&pt_ctx->t_vp, (BOOL *)pv_param1));
        }
        break;

        case WGL_CNT_LB_MOVE_ELEM_VISIBLE:
        {
            UINT16   ui2_idx         = WGL_POINTER_TO_UINT16(pv_param1);
            UINT16   ui2_idx_new_pos = WGL_POINTER_TO_UINT16(pv_param2);
            if(!(ui2_idx<pt_ctx->ui2_elem_num && ui2_idx_new_pos<pt_ctx->ui2_elem_num))
            {
                INFORM_ERR(WGLR_INTERNAL_ERROR);
                return WGLR_OK;
            }
            CATCH_FAIL(wgl_vp_move_visible(&pt_ctx->t_vp, ui2_idx, ui2_idx_new_pos));
        }
        break;
        
        case WGL_CNT_LB_GET_VISIBLE_RANGE: /*[1:OUT]WGL_PACK_2(ui2_vi_first, ui2_vi_last)
                                             [2:OUT]ui2_num_vp_elem
                                            */
        {
            UINT16 ui2_vp_elem_num_base;
            
            ui2_vp_elem_num_base   = wgl_vp_get_elem_num_base(&pt_ctx->t_vp);
            if(pv_param2!=NULL)
            {
                *((UINT16 *)pv_param2) = ui2_vp_elem_num_base;
            }
            if(pv_param1!=NULL)
            {
                *((UINT32 *)pv_param1) = (UINT32)WGL_PACK_2(WGL_LB_NULL_INDEX, WGL_LB_NULL_INDEX);
                if(pt_ctx->ui2_elem_num>0)
                {
                    UINT16 ui2_vi_first, ui2_vi_last;
                    CATCH_FAIL(wgl_vp_get_visible_range(&pt_ctx->t_vp, &ui2_vi_first, &ui2_vi_last));
                    *((UINT32 *)pv_param1) = (UINT32)WGL_PACK_2(ui2_vi_first, ui2_vi_last);
                }
            }
        }
        break;
        
        case WGL_CNT_LB_SET_COLOR: /*pv_param1:target-index | pv_param2:WGL_COLOR_INFO_T*/
        {
            UINT8 ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            switch(ui1_target) 
            {
                case WGL_CLR_BK:
                    if( pv_param2==NULL )
                    {
                        _lbcntdef_get_sys_lb_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_color_set(&pt_ctx->t_clrs_bk, (WGL_COLOR_INFO_T *)pv_param2);
                    }
                    break;
                case WGL_CLR_LB_VP_BK:
                    if(pv_param2==NULL)
                    {
                        _lbcntdef_get_sys_lb_vp_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk_vp);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_color_set(&pt_ctx->t_clrs_bk_vp, (WGL_COLOR_INFO_T *)pv_param2);
                    }
                    break;
                case WGL_CLR_LB_TITLE_FG:
                    if(pv_param2==NULL)
                    {
                        _lbcntdef_get_sys_lb_fg_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_fg);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_color_set(&pt_ctx->t_clrs_ttl_fg, (WGL_COLOR_INFO_T *)pv_param2);
                    }
                    break;
                case WGL_CLR_LB_TITLE_BK:
                    if(pv_param2==NULL)
                    {
                        _lbcntdef_get_sys_lb_bk_color_set(&pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_color_set(&pt_ctx->t_clrs_ttl_bk, (WGL_COLOR_INFO_T *)pv_param2);
                    }
                    break;
                default:
                    i4_ret_val = WGLR_INV_ARG;
            }

        }
        break;
        
        case WGL_CNT_LB_GET_COLOR: /*pv_param1:target-index | pv_param2:WGL_COLOR_INFO_T*/
        {        
            WGL_COLOR_INFO_T    *pt_clr_info;
            UINT8               ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            pt_clr_info = (WGL_COLOR_INFO_T *)pv_param2;
            switch(ui1_target) 
            {
                case WGL_CLR_BK:
                    pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
                    x_memcpy((VOID *)&pt_clr_info->u_color_data.t_standard, (VOID *)&pt_ctx->t_clrs_bk, sizeof(WGL_COLOR_STD_SET_T));
                    break;
                case WGL_CLR_LB_VP_BK:
                    pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
                    x_memcpy((VOID *)&pt_clr_info->u_color_data.t_standard, (VOID *)&pt_ctx->t_clrs_bk_vp, sizeof(WGL_COLOR_STD_SET_T));
                    break;
                case WGL_CLR_LB_TITLE_FG:
                    pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
                    x_memcpy((VOID *)&pt_clr_info->u_color_data.t_standard, (VOID *)&pt_ctx->t_clrs_ttl_fg, sizeof(WGL_COLOR_STD_SET_T));
                    break;
                case WGL_CLR_LB_TITLE_BK:
                    pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
                    x_memcpy((VOID *)&pt_clr_info->u_color_data.t_standard, (VOID *)&pt_ctx->t_clrs_ttl_bk, sizeof(WGL_COLOR_STD_SET_T));
                    break;
                default:
                    i4_ret_val = WGLR_INV_ARG;
            }
        }
        break;
        
        case WGL_CNT_LB_SET_IMAGE: /*pv_param1:target-index | pv_param2:WGL_IMG_INFO_T*/
        {
            UINT8 ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            switch(ui1_target)
            {
                case WGL_IMG_BK:
                    if( pv_param2==NULL )
                    {
                        _lbcntdef_get_sys_lb_bk_img_set(&pt_ctx->t_cvsst, &pt_ctx->t_imgs_bk);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_bk_img_set(&pt_ctx->t_imgs_bk, (WGL_IMG_INFO_T *)pv_param2 );
                    }
                break;
                
                case WGL_IMG_LB_VP_BK:
                    if( pv_param2==NULL )
                    {
                        _lbcntdef_set_dft_vp_bk_img_set(&pt_ctx->t_imgs_bk_vp);
                    }
                    else
                    {
                        _lbcntdef_tool_set_lb_bk_img_set(&pt_ctx->t_imgs_bk_vp, (WGL_IMG_INFO_T *)pv_param2 );
                    }
                break;

                default:
                    i4_ret_val = WGLR_INV_ARG;
            }

        }
        break;
        
        case WGL_CNT_LB_GET_IMAGE: /*pv_param1:target-index | pv_param2:WGL_IMG_INFO_T*/
        {  
            UINT8               ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            WGL_IMG_INFO_T      *pt_img_info;
            
            CHECK_NULL( pv_param2, WGLR_INV_ARG);
            switch(ui1_target)
            {
                case WGL_IMG_BK:
                    pt_img_info = (WGL_IMG_INFO_T *)pv_param2;
                    pt_img_info->e_type = WGL_IMG_SET_STANDARD;
                    x_memcpy(&pt_img_info->u_img_data.t_standard, &pt_ctx->t_imgs_bk, sizeof(WGL_IMG_STD_SET_T));
                break;
                
                case WGL_IMG_LB_VP_BK:
                    pt_img_info = (WGL_IMG_INFO_T *)pv_param2;
                    pt_img_info->e_type = WGL_IMG_SET_STANDARD;
                    x_memcpy(&pt_img_info->u_img_data.t_standard, &pt_ctx->t_imgs_bk_vp, sizeof(WGL_IMG_STD_SET_T));
                break;

                default:
                    i4_ret_val = WGLR_INV_ARG;
            }
            
        }
        break;
            
        case WGL_CNT_LB_SET_FONT: /*pv_param1: (WGL_FONT_INFO_T *) pv_param2:NULL*/
        {    
            i4_ret_val = _lbcntdef_set_font(pt_ctx, (WGL_FONT_INFO_T *)pv_param1, &pt_ctx->h_font);
        }
        break;

        case WGL_CNT_LB_SET_FONT_ATTR:
        {    
            UINT8 ui1_target;
            
            /*check parameter*/
            if ((pv_param1 == NULL) || pv_param2 == NULL)
            {
                return WGLR_INV_ARG;
            }

            ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
            if(ui1_target == WGL_LB_FONT_ATTR_UNREC_CHAR)
            {
                t_attr.w2c_unrec_char = *((UTF16_T*)pv_param2);
                i4_ret_val = wgl_frlm_change_font_attr(&pt_ctx->h_font, &t_attr, WGL_FONT_ATTR_MAKS_UNREC_CHAR);
            }
        }            
        break;

        case WGL_CNT_LB_SET_COL_FONT_ATTR:
        {    
            UINT8 ui1_target;
            UINT8 ui1_c;
            
            /*check parameter*/
            if ((pv_param1 == NULL) || pv_param2 == NULL)
            {
                return WGLR_INV_ARG;
            }

            ui1_c = WGL_GET_LOW_LOW_BYTE(pv_param1);
            ui1_target = WGL_GET_LOW_HI_BYTE(pv_param1);
            if( ui1_c >= pt_ctx->ui1_col_num )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            
            if(ui1_target == WGL_LB_FONT_ATTR_UNREC_CHAR)
            {
                t_attr.w2c_unrec_char = *((UTF16_T*)pv_param2);
                i4_ret_val = wgl_frlm_change_font_attr(&pt_ctx->at_cnt_cols[ui1_c].h_font, &t_attr, WGL_FONT_ATTR_MAKS_UNREC_CHAR);
            }
        }            
        break;
        
        case WGL_CNT_LB_GET_FONT: /*pv_param1: (WGL_FONT_INFO_T *) pv_param2:NULL*/
        {        
            CHECK_VALID(pv_param1 != NULL, WGLR_INV_ARG);
            i4_ret_val = wgl_frlm_get_info(&pt_ctx->t_frlm, pt_ctx->h_font, (WGL_FONT_INFO_T *)pv_param1);
        }
        break;

        case WGL_CNT_LB_SET_FONT_EX: /*pv_param1: (WGL_FONT_INFO_EX_T *) pv_param2:NULL*/
        {    
            i4_ret_val = _lbcntdef_set_font_ex(pt_ctx, (WGL_FONT_INFO_EX_T *)pv_param1, &pt_ctx->h_font);
        }
        break;

        case WGL_CNT_LB_GET_FONT_EX: /*pv_param1: (WGL_FONT_INFO_EX_T *) pv_param2:NULL*/
        {        
            CHECK_VALID(pv_param1 != NULL, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_get_font_ex(pt_ctx, (WGL_FONT_INFO_EX_T *)pv_param1);
        }
        break;
        case WGL_CMD_GL_SET_TEXT_ATTR:
        {
            if(NULL == pv_param1 || ((WGL_TEXT_ATTR_T*)pv_param1)->t_font_attr_type != WGL_TEXT_ATTR_REORDER)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_SET_TEXT_ATTR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            else
            {
                if(((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data == NULL)
                {
                    /*default value*/
                    pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
                    pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
     
                }else
                {
                    pt_ctx->t_arab_reorder_attr.b_find_init_level = FALSE;
                    pt_ctx->t_arab_reorder_attr.ui1_embedding_level = *((UINT8*)((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data);
                }
            }
        }
        break;
        case WGL_CMD_GL_GET_TEXT_ATTR:
        {
            if(NULL == pv_param1 || 
              ((WGL_TEXT_ATTR_T*)pv_param1)->t_font_attr_type != WGL_TEXT_ATTR_REORDER ||
              ((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data == NULL)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_TEXT_ARAB_ATTR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

           *((UINT8*)((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data) = pt_ctx->t_arab_reorder_attr.ui1_embedding_level;
        }
        break;
        case WGL_CNT_LB_SET_BDR_INSET: /*pv_param1:(WGL_INSET_T *) pv_param2: NULL*/
        {        
            WGL_INSET_T *pt_inset_bdr = (WGL_INSET_T *)pv_param1;
            i4_ret_val = _lbcntdef_set_lb_screen_rect_or_bdr_inset(pt_ctx, &pt_ctx->t_rc_scrn_lb, pt_inset_bdr);
        }
        break;
        
        case WGL_CNT_LB_CLEAR:
        {
            LB_ELEM_T   *pt_elem = (LB_ELEM_T *)pt_ctx->pt_elems;
            UINT16      ui2_i;
            for(ui2_i=0; ui2_i<_LBCNTDEF_GET_ELEM_CACHE_NUM(pt_ctx); ui2_i++)
            {
                CATCH_FAIL( _lbcntdef_del_elem(pt_ctx, pt_elem, TRUE));
                pt_elem = pt_elem->pt_next;
            }
            _LBCNTDEF_SET_ELEMS_NUM(pt_ctx, 0);
            _LBCNTDEF_SET_ELEM_CACHE_NUM(pt_ctx, 0);
            _LBCNTDEF_SET_CACHE_START(pt_ctx, 0);
            i4_ret_val           = wgl_vp_clear(&pt_ctx->t_vp);
        }
        break;

        case WGL_CNT_LB_SET_IS_BDR_DRAW_INTERIOR:
        {
            BOOL b_is_bdr_draw_interior = WGL_POINTER_TO_BOOL(pv_param1);
            if(b_is_bdr_draw_interior)
            {
                WGL_SET_FLAG(pt_ctx->ui1_flags, LBCNTDEF_FLAG_IS_BDR_DRAW_INTERIOR);
            }
            else
            {
                WGL_CLEAR_FLAG(pt_ctx->ui1_flags, LBCNTDEF_FLAG_IS_BDR_DRAW_INTERIOR);
            }
        }
        break;

        case WGL_CNT_LB_REQ_UPDATE_VP_INDRS:
        {
            _lbcntdef_add_update_region_by_vp_indicators(&pt_ctx->t_scr_imgs, &pt_ctx->t_pur_wait_for_update);
        }
        break;
        
        case WGL_CNT_LB_REQ_UPDATE_HLT_ELEM:
        {
            UINT16  ui2_hlt = WGL_POINTER_TO_UINT16(pv_param1);
            
            i4_ret_val = _lbcntdef_add_update_region_by_hlt_elem (pt_ctx, ui2_hlt, &pt_ctx->t_pur_wait_for_update);
        }
        break;
        
        case WGL_CNT_LB_REQ_UPDATE_UNHLT_ELEM: 
        {
            UINT16  ui2_unhlt = WGL_POINTER_TO_UINT16(pv_param1);
            
            i4_ret_val = _lbcntdef_add_update_region_by_hlt_elem (pt_ctx, ui2_unhlt, &pt_ctx->t_pur_wait_for_update);
        }
        break;
        
        case WGL_CNT_LB_REQ_UPDATE_RANGE_ELEMS:
        {
            LB_CNT_PU_ELEM_RANGE_T*  pt_elem_range = (LB_CNT_PU_ELEM_RANGE_T *) pv_param1;
            
            CHECK_NULL (pt_elem_range, WGLR_INV_ARG);
            
            i4_ret_val = _lbcntdef_add_update_region_by_range_elems (pt_ctx, pt_elem_range, &pt_ctx->t_pur_wait_for_update);
        }
        break;
        
        case WGL_CNT_LB_REQ_UPDATE_FULL:
        {
            _lbcntdef_add_update_region_by_full_lb (pt_ctx, &pt_ctx->t_pur_wait_for_update);
        }    
        break;

        case WGL_CNT_LB_GET_HLT_ELEM_AREA:
        {
            UINT16                          ui2_vi_first, ui2_vi_last;
            INT32                           i4_vp_first_elem_top;
            GL_RECT_T                       t_rc_hlt_elem;
            GL_RECT_T                       t_rc_hlt_elem_logical;
            INT32                           i4_h_elem = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
            UINT16                          ui2_hlt = WGL_POINTER_TO_UINT16(pv_param1);

            if(ui2_hlt >= pt_ctx->ui2_elem_num)
            {
                i4_ret = WGLR_INV_ARG;
                break;
            }
            
            CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_vi_first, &ui2_vi_last, &i4_vp_first_elem_top));
            
            t_rc_hlt_elem.i4_left   = pt_ctx->t_rc_lb_vp.i4_left;
            t_rc_hlt_elem.i4_top    = pt_ctx->t_rc_lb_vp.i4_top + (i4_vp_first_elem_top + (((INT32)ui2_hlt) - ((INT32)ui2_vi_first)) * i4_h_elem);
            t_rc_hlt_elem.i4_right  = pt_ctx->t_rc_lb_vp.i4_right;
            t_rc_hlt_elem.i4_bottom = t_rc_hlt_elem.i4_top + ((INT32)pt_ctx->ui2_elem_height);
            
            if(_lbcntdef_tool_check_hlt_elem_eff_enlarged(&pt_ctx->t_effect_hlt_elem, PSUEDO_PAINT_STATE_FOR_HLT_ELEM_AREA))
            {
                SET_RECT(
                    &t_rc_hlt_elem_logical,
                    t_rc_hlt_elem.i4_left   - (INT32)((INT16)pt_ctx->t_effect_hlt_elem.t_ex_margin.ui2_left),
                    t_rc_hlt_elem.i4_top    - (INT32)((INT16)pt_ctx->t_effect_hlt_elem.t_ex_margin.ui2_top),
                    t_rc_hlt_elem.i4_right  + (INT32)((INT16)pt_ctx->t_effect_hlt_elem.t_ex_margin.ui2_right),
                    t_rc_hlt_elem.i4_bottom + (INT32)((INT16)pt_ctx->t_effect_hlt_elem.t_ex_margin.ui2_bottom));
            }
            else
            {
                WGL_RECT_COPY(&t_rc_hlt_elem_logical, &t_rc_hlt_elem);
            }

            WGL_RECT_COPY((GL_RECT_T *)pv_param2, &t_rc_hlt_elem_logical);
        }
        break;

        case WGL_CNT_LB_GET_UPDATE_REGION_BY_ITEM:
        {
            LB_CNT_GURBI_PARAM_T    *pt_param = (LB_CNT_GURBI_PARAM_T *) pv_param1;
            
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            
            i4_ret_val = _lbcntdef_get_update_region_by_item(
                pt_ctx, 
                pt_param->ui4_state_wgt,
                pt_param->ui2_idx_elem, 
                pt_param->ui1_idx_col,
                (LB_CNT_PU_REGION_T *) pv_param2 );
        }
        break;
        
        case WGL_CNT_LB_GET_UPDATE_REGION_GENERIC:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_get_update_region_generic(
                pt_ctx, 
                (LB_CNT_PU_REGION_T *)pv_param1 );
        }
        break;

        case WGL_CNT_LB_GET_LAST_FULL_UPDATE_TICK:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            *((UINT32 *)pv_param1) = pt_ctx->ui4_tk_last_full_update;
        }
        break;
        
        case WGL_CNT_LB_GET_REQ_TIMER:
        {
            i4_ret_val = _lbcntdef_cmd_hdr_get_req_timer(
                pt_ctx, 
                (LB_CNT_TIMER_REQ_T *) pv_param1);
        } 
        break;
        
        case WGL_CNT_LB_DO_TIMER:
        {
            i4_ret_val = _lbcntdef_cmd_hdr_do_timer(
                pt_ctx, 
                (LB_CNT_DO_TIMER_PARAM_T *) pv_param1 );
        }
        break;
        
        case WGL_CNT_LB_UPDATE_CVSST:
        {
            wgl_cvsst_update_clr_cfg (&pt_ctx->t_cvsst, (WGL_CVS_CLR_CFG_T *) pv_param1);
        }
        break;

		case WGL_CNT_LB_GET_BUFFER:
        {
           *((UTF16_T**)pv_param1) = pt_ctx->w2s_buffer;
		   i4_ret_val = WGLR_OK;
        }
        break;

#ifdef WGL_ANIM_SUPPORT
        case WGL_CNT_LB_GET_ANIM_ELEM_AREA:
        {
            GL_RECT_T                       t_anim_elem;
            GL_RECT_T                       t_anim_elem_logical;
            INT32                           i4_h_elem = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
            LB_VP_RANGE_INF_T               t_vp_range;
            BOOL                            b_scroll;
            LB_CNT_EEVMM_PARAM_T *pt_param = (LB_CNT_EEVMM_PARAM_T *)pv_param1;

            if(pv_param1 == NULL || pt_param->ui2_idx >= pt_ctx->ui2_elem_num)
            {
                i4_ret = WGLR_INV_ARG;
                break;
            }

            if(HAS_STYLE(pt_ctx, WGL_STL_LB_NO_SCROLL))
            {
                CATCH_FAIL(wgl_vp_check_visibility_no_scroll(
                                &pt_ctx->t_vp,
                                pt_param->ui2_idx, 
                                FALSE,
                                &b_scroll,
                                &t_vp_range));          
            }
            else
            {
                CATCH_FAIL(wgl_vp_check_visibility_minimal_move(&pt_ctx->t_vp, 
                                                    pt_param->ui2_idx, 
                                                    pt_param->ui2_margin, 
                                                    FALSE, 
                                                    &b_scroll,
                                                    &t_vp_range));
            }
            
            t_anim_elem.i4_left   = pt_ctx->t_rc_lb_vp.i4_left;
            t_anim_elem.i4_top    = pt_ctx->t_rc_lb_vp.i4_top + (t_vp_range.i4_offset + 
                                   (((INT32)pt_param->ui2_idx) - ((INT32)t_vp_range.ui2_idx_vp_first)) * i4_h_elem);
            t_anim_elem.i4_right  = pt_ctx->t_rc_lb_vp.i4_right;
            t_anim_elem.i4_bottom = t_anim_elem.i4_top + ((INT32)pt_ctx->ui2_elem_height);
            
            /*Get the margin informaiton*/
            if(HAS_FLAG(pt_ctx->t_effect_hlt_elem.ui4_style, WGL_LB_HEF_STL_ENLARGE_SIZE))
            {
                x_memcpy(&pt_ctx->t_anim_elem.t_ex_margin, 
                         &pt_ctx->t_effect_hlt_elem.t_ex_margin, 
                         sizeof(WGL_LB_EXTRA_MARGIN_T));
            }
            else
            {
                x_memset(&pt_ctx->t_anim_elem.t_ex_margin, 0, sizeof(WGL_LB_EXTRA_MARGIN_T));
            }
            
            SET_RECT(
                &t_anim_elem_logical,
                t_anim_elem.i4_left   - (INT32)((INT16)pt_ctx->t_anim_elem.t_ex_margin.ui2_left),
                t_anim_elem.i4_top    - (INT32)((INT16)pt_ctx->t_anim_elem.t_ex_margin.ui2_top),
                t_anim_elem.i4_right  + (INT32)((INT16)pt_ctx->t_anim_elem.t_ex_margin.ui2_right),
                t_anim_elem.i4_bottom + (INT32)((INT16)pt_ctx->t_anim_elem.t_ex_margin.ui2_bottom));

            if(pv_param2 != NULL)
            {
                WGL_RECT_COPY((GL_RECT_T *)pv_param2, &t_anim_elem_logical);
            }
            
        }
        break; 

        case WGL_CNT_LB_SET_ANIM_ALPHA_INF:
        {
            pt_ctx->t_anim_elem.ui1_alpha_s = WGL_POINTER_TO_UINT8(pv_param1);
            pt_ctx->t_anim_elem.ui1_alpha_e = WGL_POINTER_TO_UINT8(pv_param2);
        }
        break;

        case WGL_CNT_LB_GET_ANIM_ALPHA_INF:
        {
            *((UINT8*)pv_param1) = pt_ctx->t_anim_elem.ui1_alpha_s;
            *((UINT8*)pv_param2) = pt_ctx->t_anim_elem.ui1_alpha_e;            
        }
        break;
            
        case WGL_CNT_LB_SET_ANIM_TICK_INF:  
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);

            x_memcpy(&pt_ctx->t_tick_inf,(WGL_ANIM_TICK_DATA*)pv_param1,sizeof(WGL_ANIM_TICK_DATA));

        }
        break;

       case WGL_CNT_LB_REQ_UPDATE_ANIM:
       {
            i4_ret_val = _lbcntdef_add_update_region(pt_ctx,(WGL_UPD_REGION_T*)pv_param1);   
       }
       break;

        case WGL_CMD_LB_SET_ANIM_ELEM:  
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_cmd_hdr_set_anim_elem_effect(pt_ctx, (WGL_LB_ANIM_ELEM_EFFECT_T *)pv_param1);
        }
        break;                    

        case WGL_CMD_LB_GET_ANIM_ELEM:  
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _lbcntdef_cmd_hdr_get_anim_elem_effect(pt_ctx, (WGL_LB_ANIM_ELEM_EFFECT_T *)pv_param1);
        }
        break;  
#endif
        case WGL_CMD_LB_SET_NTF:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            pt_ctx->pf_lb_cb_fct = (wgl_lb_cb_fct)pv_param1;
        }
        break;

        case WGL_CMD_LB_DO_NTF:
        {
            WGL_LB_NFY_TYPE_T    e_nfy_type ;
            WGL_PACKED_PARAM_T*  pt_pparam ;
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            CHECK_NULL(pv_param2, WGLR_INV_ARG);

            e_nfy_type = *((WGL_LB_NFY_TYPE_T*)pv_param1);
            pt_pparam = (WGL_PACKED_PARAM_T*)pv_param2;
            i4_ret_val = _lbcntdef_do_cb_fct(pt_ctx,e_nfy_type,pt_pparam->pv_param1,pt_pparam->pv_param2);
        }
        break;

       case WGL_CMD_LB_SET_HANDLE_TO_CNT:
       {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            pt_ctx->h_lb = *((HANDLE_T*)pv_param1);
       }
       break;

       case WGL_CMD_LB_GET_RANGE:
       {
           UINT16        ui2_first =0;
           UINT16        ui2_last =0;
           INT32         i4_vp_top_first;

           CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_first, &ui2_last, &i4_vp_top_first));
           if(pv_param1 != NULL)
           {
               *((UINT32 *)pv_param1) = (UINT32)WGL_PACK_2(ui2_first, ui2_last);
           } 

           if(pv_param2 != NULL)
           {
               *((INT32 *)pv_param2) = (INT32)i4_vp_top_first;
           } 
       }
       break;
       
       case WGL_CNT_LB_GET_CNT_MEM_ALLOC:
       {
           UINT32      ui4_wdg_mem = 0; 

           if(pv_param1 == NULL)
           {
                return WGLR_INV_ARG;
           }
#ifdef CLI_LVL_ALL    
           ui4_wdg_mem += pt_ctx->ui4_mem_size;
           ui4_wdg_mem += wgl_npm_get_total_size(&pt_ctx->t_npm_itm_fmt);
#endif
           *((UINT32 *)pv_param1) = ui4_wdg_mem ;
       }
       break;

       default:
       {
           INFORM_ERR(WGLR_INV_CMD);
           i4_ret_val = WGLR_INV_CMD;
       }
    }
    return i4_ret_val;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_lb_cnt_def_paint
 *
 * Description: This API paints the content area.
 *
 * Inputs:  pv_ctx_cnt       Specifies the instance context of content.
 *          h_g              Contains the handle of graphic context.
 *          ui4_upt_rect     Specifies update rectangle number.
 *          at_upt_rect      Contains the update rectangles.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/
INT32 wgl_lb_cnt_def_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        pv_ctx_cnt,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    at_upt_rect,
    UINT16                              ui2_state_paint)
{
    LBCNTDEF_CTX_T                      *pt_ctx = (LBCNTDEF_CTX_T *)pv_ctx_cnt;
    INT32                               i4_ret, i4_ret_fnl;
    GL_RECT_T                           t_rc_lb_upt;
    
    /*check the validity of the dimensions of the widget*/
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_lb_vp) ||
       !RECT_IS_EFFECTIVE(pt_ctx->t_rc_lb_cnt) )
    {
        return WGLR_OK;
    }

    if(ui4_upt_rect==1 && RECT_IS_EFFECTIVE(at_upt_rect[0]))
    {    
        WGL_RECT_COPY(&t_rc_lb_upt, &at_upt_rect[0]);
    }
    else
    {
        SET_RECT_BY_SIZE(&t_rc_lb_upt, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_lb), RECT_H(&pt_ctx->t_rc_scrn_lb));
    }
    
    if(RECT_W(&t_rc_lb_upt)>=RECT_W(&pt_ctx->t_rc_scrn_lb) &&
       RECT_H(&t_rc_lb_upt)>=RECT_H(&pt_ctx->t_rc_scrn_lb) )
    {
        INFORM_ERR(_lbcntdef_paint_all(pt_ctx, h_g, ui2_state_paint));
        _lbcntdef_purm_reset(&pt_ctx->t_pur_wait_for_update);
        pt_ctx->ui4_tk_last_full_update = os_get_sys_tick();
    }
    else
    {
        INFORM_ERR(_lbcntdef_paint_partial(pt_ctx, h_g, ui2_state_paint, &t_rc_lb_upt));
    }
    
    /* check if need to request a timer */
    i4_ret_fnl = ( ! anm_trq_check_empty( &pt_ctx->t_trq ) ) ? 
        WGL_LBR_REQ_TIMER : WGLR_OK;

    return i4_ret_fnl;
}    
    
#ifdef WGL_LB_NO_STORE_TEXT
static BOOL _lbcntdef_check_text_type_col(
    IN      LBCNTDEF_CTX_T              *pt_ctx)    
{
    UINT8                               ui1_col;
    
    for(ui1_col = 0; ui1_col < pt_ctx->ui1_col_num; ui1_col ++)
    {
        if(pt_ctx->at_cols[ui1_col].e_col_type == LB_COL_TYPE_TEXT)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_paint_all
 *
 * Description: This API fully updates the content area.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of graphic context.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_paint_all( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint)
{
    LB_ELEM_T                   *pt_elem=NULL, *pt_elem_hlt=NULL, *pt_elem_first=NULL, *pt_elem_last=NULL;
    LBCNTDEF_ELEM_T             *pt_cnt_elem_tmp=NULL;
    UINT16                      ui2_first=0, ui2_last=0, ui2_i;
    INT32                       i4_first_elem_top_vp, i4_h_elem_total;
    INT32                       i4_elem_top_scrn, i4_hlt_elem_top_scrn;
    GL_RECT_T                   t_rc_lb;
    INT32                       i4_h_elem;
    INT32                       i4_ret;
#ifdef WGL_LB_NO_STORE_TEXT
    WGL_LB_ELEM_DATA_INFO_T     t_elem_data;
#endif
    UINT16                      ui2_hlt_elem;
    UINT32                      ui4_elem_flag;

    /*fill the background of the content*/
    INFORM_ERR(_lbcntdef_draw_lb_bk(pt_ctx, h_g, ui2_state_paint, &pt_ctx->t_rc_lb_cnt));
    
___BLOCK_BEGIN___
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        GOTO_EOB;
    }
    /*fill the background of the viewport*/
    INFORM_ERR(_lbcntdef_draw_lb_vp_bk(pt_ctx, h_g, ui2_state_paint, &pt_ctx->t_rc_lb_vp));

    if(pt_ctx->ui2_elem_num==0)
    {
        GOTO_EOB;
    }

    i4_ret = wgl_vp_get_range(&pt_ctx->t_vp, &ui2_first, &ui2_last, &i4_first_elem_top_vp);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        GOTO_EOB;
    }
    if(ui2_first>=pt_ctx->ui2_elem_num || ui2_last>=pt_ctx->ui2_elem_num)
    {
        GOTO_EOB;
    }

#ifdef WGL_LB_NO_STORE_TEXT
    /*If the listbox is no store text tyle and that has text type col,it shou get the text information first.*/
    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {  
        _lbcntdef_no_store_get_data(pt_ctx,&t_elem_data);
    }
#endif

    i4_h_elem            = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
    _lbcntdef_get_elem(pt_ctx, ui2_first, &pt_elem_first, &pt_cnt_elem_tmp);
    if(pt_elem_first == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    pt_elem              = pt_elem_first;
    pt_elem_last         = pt_elem;
    i4_h_elem_total      = 0;
    i4_elem_top_scrn     = pt_ctx->t_rc_scrn_vp.i4_top + i4_first_elem_top_vp;
    pt_elem_hlt          = NULL;
    i4_hlt_elem_top_scrn = 0;
    for(ui2_i=ui2_first; ui2_i<=ui2_last; ui2_i++)
    {
        if( !( HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_i), LBEST_HLT)  &&
               _lbcntdef_tool_check_hlt_elem_eff_enlarged(&pt_ctx->t_effect_hlt_elem, ui2_state_paint) ))
        {
            _lbcntdef_get_elem_flag(ui2_i, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);

            /* normally render unhighlighted elements */
#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                INFORM_ERR(_lbcntdef_render_elem_ex(
                    pt_ctx, 
                    h_g, 
                    ui2_state_paint, 
                    pt_elem, 
                    i4_elem_top_scrn, 
                    &pt_ctx->t_rc_scrn_vp,
                    &t_elem_data,
                    ui2_i));
            }
            else
#endif                
            {
            INFORM_ERR(_lbcntdef_render_elem(
                pt_ctx, 
                h_g, 
                ui2_state_paint, 
                pt_elem, 
                ui4_elem_flag,
                i4_elem_top_scrn, 
                &pt_ctx->t_rc_scrn_vp));
            }
        }
        else
        {
            /* enlarged highlighted element found, keep the context for rendering it */
            pt_elem_hlt          = pt_elem;
            i4_hlt_elem_top_scrn = i4_elem_top_scrn;
            ui2_hlt_elem = ui2_i;
        }
        
        pt_elem_last         = pt_elem;
        i4_h_elem_total     += i4_h_elem;
        
        pt_elem              = pt_elem->pt_next;
        i4_elem_top_scrn    += i4_h_elem;
    }
    
    /* draw the scroll bar */
    SET_RECT_BY_SIZE(&t_rc_lb, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_lb), RECT_H(&pt_ctx->t_rc_scrn_lb));
    INFORM_ERR(_lbcntdef_draw_scrl_bar(
        pt_ctx,
        h_g,
        ui2_state_paint,
        &t_rc_lb));

    /* change the z-order of the highlighted element if it is enlarged */
    if( _lbcntdef_tool_check_hlt_elem_eff_enlarged(&pt_ctx->t_effect_hlt_elem, ui2_state_paint) )
    {
        INT32   i4_hlt_elem_top_vp;
        
        /* check if the highlighted element is visible in the viewport */
        if(pt_elem_hlt)
        {
            /* render highlighted element above all other elements */
            _lbcntdef_get_elem_flag(ui2_hlt_elem, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);
#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                INFORM_ERR(_lbcntdef_render_elem_ex(
                    pt_ctx, 
                    h_g, 
                    ui2_state_paint, 
                    pt_elem_hlt, 
                    i4_hlt_elem_top_scrn, 
                    &pt_ctx->t_rc_scrn_lb,
                    &t_elem_data,
                    ui2_hlt_elem));
            }
            else
#endif                
            {
            INFORM_ERR(_lbcntdef_render_elem(
                pt_ctx, 
                h_g, 
                ui2_state_paint, 
                pt_elem_hlt, 
                ui4_elem_flag,
                i4_hlt_elem_top_scrn, 
                &pt_ctx->t_rc_scrn_lb));
        }
        }
        /* still have to check if the extra margin of the highlighted element is visible in the viewport */
        else if(_lbcntdef_tool_check_if_hlt_elem_visible( 
            pt_ctx,
            FALSE,  /* don't check the elements in vp */
            RECT_H(&pt_ctx->t_rc_scrn_vp),
            i4_h_elem,
            &pt_ctx->t_effect_hlt_elem.t_ex_margin,
            pt_elem_first,
            pt_elem_last,
            i4_first_elem_top_vp,
            i4_first_elem_top_vp + i4_h_elem_total,
            ui2_first,
            ui2_last,
            &pt_elem_hlt,
            &i4_hlt_elem_top_vp,
            &ui2_hlt_elem))
        {
            /* render the extra margin of highlighted element above all other elements */
            _lbcntdef_get_elem_flag(ui2_hlt_elem, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);
            i4_hlt_elem_top_scrn = i4_hlt_elem_top_vp + pt_ctx->t_rc_scrn_vp.i4_top;
#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                INFORM_ERR(_lbcntdef_render_elem_ex(
                    pt_ctx, 
                    h_g, 
                    ui2_state_paint, 
                    pt_elem_hlt, 
                    i4_hlt_elem_top_scrn, 
                    &pt_ctx->t_rc_scrn_lb,
                    &t_elem_data,
                    ui2_hlt_elem));
            }
            else
#endif                
            {
            INFORM_ERR(_lbcntdef_render_elem(
                pt_ctx, 
                h_g, 
                ui2_state_paint, 
                pt_elem_hlt, 
                ui4_elem_flag,
                i4_hlt_elem_top_scrn, 
                &pt_ctx->t_rc_scrn_lb));
        }
    }
    }
    
___BLOCK_END___

    INFORM_ERR(x_gl_set_clip(h_g, &pt_ctx->t_rc_scrn_lb));
    INFORM_ERR(x_gl_translate(h_g, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top));

    /* draw the title */
    INFORM_ERR(_lbcntdef_draw_title(
        pt_ctx,
        h_g,
        ui2_state_paint,
        NULL));
    
    if (pt_ctx->ui2_elem_num > 0            &&
        ui2_first < pt_ctx->ui2_elem_num    &&
        ui2_last  < pt_ctx->ui2_elem_num    )
    {
        /* draw the scrolling indicator images */
        if( !HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_SCRL_IMG) || 
            ( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) )
        {
            INFORM_ERR(_lbcntdef_draw_scroll_images(
                pt_ctx,
                h_g,
                NULL));
        }
        
        /* draw the page up/down indicator image */
        if( !HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_PAGI_IMG) || 
            ( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) )
        {
            INFORM_ERR(_lbcntdef_draw_indicator_images(
                h_g, 
                &pt_ctx->t_cvsst,
                &pt_ctx->t_scr_imgs_paging, 
                (BOOL)( (!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || (ui2_first > 0) ? TRUE : FALSE),
                (BOOL)( (!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || (ui2_last+(UINT16)1 < pt_ctx->ui2_elem_num) ? TRUE : FALSE),
                NULL));
        }
    }
    
#ifdef WGL_LB_NO_STORE_TEXT
    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        _lbcntdef_clear_elem_buf(pt_ctx);
    }
#endif    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_paint_partial
 *
 * Description: This API partially paints the dirty area.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_rc_lb_upt     Contains the invalid update rect in listbox's coordinates.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_paint_partial( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt)
{
    GL_RECT_T       t_rc_lb_update_vp;
    INT32           i4_ret;
    UINT16          ui2_first=0, ui2_last=0;
    LB_ELEM_T       *pt_elem_first=NULL, *pt_elem_last=NULL, *pt_elem_hlt=NULL;
    INT32           i4_hlt_elem_top_scrn;
    INT32           i4_first_elem_top_vp, i4_h_elem_total;
    INT32           i4_h_elem;
#ifdef WGL_ANIM_SUPPORT
    LB_ELEM_T       *pt_anim_elem = NULL;
#endif
#ifdef WGL_LB_NO_STORE_TEXT
    WGL_LB_ELEM_DATA_INFO_T     t_elem_data;
#endif
    UINT16                      ui2_hlt_elem;
    LB_ELEM_T           *pt_elem;
    LBCNTDEF_ELEM_T     *pt_cnt_elem_tmp;
    UINT32              ui4_elem_flag;
    
    /*fill the background of the content*/
    INFORM_ERR(_lbcntdef_draw_lb_bk(pt_ctx, h_g, ui2_state_paint, pt_rc_lb_upt));
    
___BLOCK_BEGIN___

    if(!RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        GOTO_EOB;
    }
    /*fill the background of the viewport*/
    INFORM_ERR(_lbcntdef_draw_lb_vp_bk(pt_ctx, h_g, ui2_state_paint, pt_rc_lb_upt));
    
    if(pt_ctx->ui2_elem_num==0)
    {
        GOTO_EOB;
    }

    i4_ret = wgl_vp_get_range(&pt_ctx->t_vp, &ui2_first, &ui2_last, &i4_first_elem_top_vp);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        GOTO_EOB;
    }
    if(ui2_first>=pt_ctx->ui2_elem_num || ui2_last>=pt_ctx->ui2_elem_num)
    {
        GOTO_EOB;
    }

#ifdef WGL_LB_NO_STORE_TEXT
    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {   
        _lbcntdef_no_store_get_data(pt_ctx,&t_elem_data);
    }
#endif
    /* init highlighted element drawing context */
    pt_elem_first        = NULL;
    pt_elem_last         = NULL;
    i4_h_elem_total      = 0;
    i4_h_elem            = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
    pt_elem_hlt          = NULL;
    i4_hlt_elem_top_scrn = 0;

    /*check the intersaction of the viewport & update-rectangle*/
    INTERSECT_RECT(&t_rc_lb_update_vp, &pt_ctx->t_rc_lb_vp, pt_rc_lb_upt);
    if(RECT_IS_EFFECTIVE(t_rc_lb_update_vp))
    {
        INT32               i4_elem_top_lb, i4_elem_btm_lb;
        UINT16              ui2_i;
        GL_RECT_T           t_rc_scrn_eff_paint_in_vp;

        _RECT_MOVE(&t_rc_lb_update_vp, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top, &t_rc_scrn_eff_paint_in_vp);
        
        _lbcntdef_get_elem(pt_ctx, ui2_first, &pt_elem_first, &pt_cnt_elem_tmp);

        pt_elem         = pt_elem_first;
        pt_elem_last    = pt_elem;
        i4_h_elem_total = 0;
        i4_elem_top_lb  = pt_ctx->t_rc_lb_vp.i4_top + i4_first_elem_top_vp;
        i4_elem_btm_lb  = i4_elem_top_lb + i4_h_elem;
        for(ui2_i=ui2_first; pt_elem && ui2_i<=ui2_last && i4_elem_top_lb<t_rc_lb_update_vp.i4_bottom; ui2_i++)
        {
            if(_RANGE_IS_INTERSECT(i4_elem_top_lb, i4_elem_btm_lb-1, t_rc_lb_update_vp.i4_top, t_rc_lb_update_vp.i4_bottom-1))
            {
                if( ! HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_i), LBEST_HLT) ||
                    ! _lbcntdef_tool_check_hlt_elem_eff_enlarged(&pt_ctx->t_effect_hlt_elem, ui2_state_paint) )
                {
                    /* normally render unhighlighted elements */
                    _lbcntdef_get_elem_flag(ui2_i, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);
#ifdef WGL_LB_NO_STORE_TEXT
                    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
                    {
                        INFORM_ERR(_lbcntdef_render_elem_ex(
                            pt_ctx, 
                            h_g, 
                            ui2_state_paint, 
                            pt_elem, 
                            i4_elem_top_lb + pt_ctx->t_rc_scrn_lb.i4_top, 
                            &t_rc_scrn_eff_paint_in_vp,
                            &t_elem_data,
                            ui2_i));
                    }
                    else
#endif                        
                    {
                        INFORM_ERR(_lbcntdef_render_elem(
                                pt_ctx, 
                                h_g, 
                                ui2_state_paint, 
                                pt_elem, 
                                ui4_elem_flag,
                                i4_elem_top_lb + pt_ctx->t_rc_scrn_lb.i4_top, 
                                &t_rc_scrn_eff_paint_in_vp));
                    }
                }
                else
                {
                    /* enlarged highlighted element found, keep the context for rendering it */
                    pt_elem_hlt          = pt_elem;
                    i4_hlt_elem_top_scrn = i4_elem_top_lb + pt_ctx->t_rc_scrn_lb.i4_top;
                    ui2_hlt_elem = ui2_i;
                }
                }
#ifdef WGL_ANIM_SUPPORT
            if(HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_i), LBEST_ANIM))
            {
                pt_anim_elem = pt_elem;
            }
#endif
        
            pt_elem_last     = pt_elem;
            i4_h_elem_total += i4_h_elem;
        
            i4_elem_top_lb   = i4_elem_btm_lb;
            i4_elem_btm_lb  += i4_h_elem;
            pt_elem          = pt_elem->pt_next;
        }
    }
    
    /* draw the scroll bar */
    INFORM_ERR(_lbcntdef_draw_scrl_bar(
        pt_ctx,
        h_g,
        ui2_state_paint,
        pt_rc_lb_upt));

    /* check if the highlighted element is enlarged */
    if(_lbcntdef_tool_check_hlt_elem_eff_enlarged(&pt_ctx->t_effect_hlt_elem, ui2_state_paint))
    {
        GL_RECT_T   t_rc_scrn_upt;
        
        _RECT_MOVE(pt_rc_lb_upt, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top, &t_rc_scrn_upt);
        
        /* render the enlarged highlighted element if need */
        if(pt_elem_hlt)
        {
            /* render the enlarged highlighted element */
            _lbcntdef_get_elem_flag(ui2_hlt_elem, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);
                        
#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                INFORM_ERR(_lbcntdef_render_elem_ex(
                    pt_ctx,
                    h_g,
                    ui2_state_paint,
                    pt_elem_hlt,
                    i4_hlt_elem_top_scrn,
                    &t_rc_scrn_upt,
                    &t_elem_data,
                    ui2_hlt_elem));
            }
            else
#endif                
            {
            INFORM_ERR(_lbcntdef_render_elem(
                pt_ctx,
                h_g,
                ui2_state_paint,
                pt_elem_hlt,
                ui4_elem_flag,
                i4_hlt_elem_top_scrn,
                &t_rc_scrn_upt));
        }
        }
        /* still have to check if the enlarged highlighted element is visible and have to update */
        else 
        {
            INT32   i4_hlt_elem_top_vp;
            
            if(!pt_elem_first)
            {
                _lbcntdef_get_vp_params_tool_1(
                    pt_ctx,
                    i4_h_elem,
                    ui2_first,
                    ui2_last,
                    &pt_elem_first,
                    &pt_elem_last,
                    &i4_h_elem_total);
            }
            
            if((pt_elem_first) && (pt_elem_last))
            {
                if(_lbcntdef_tool_check_if_hlt_elem_visible(
                    pt_ctx,            
                    TRUE, /* the elements in vp will be first checked */
                    RECT_H(&pt_ctx->t_rc_scrn_vp),
                    i4_h_elem,
                    &pt_ctx->t_effect_hlt_elem.t_ex_margin,
                    pt_elem_first,
                    pt_elem_last,
                    i4_first_elem_top_vp,
                    i4_first_elem_top_vp + i4_h_elem_total,
                    ui2_first,
                    ui2_last,
                    &pt_elem_hlt,
                    &i4_hlt_elem_top_vp,
                    &ui2_hlt_elem))
                {
                    /* render the enlarged highlighted element */
                    _lbcntdef_get_elem_flag(ui2_hlt_elem, ui2_last, _LBCNTDEF_GET_ELEMS_NUM(pt_ctx), &ui4_elem_flag);
#ifdef WGL_LB_NO_STORE_TEXT
                    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
                    {
                        INFORM_ERR(_lbcntdef_render_elem_ex(
                            pt_ctx,
                            h_g,
                            ui2_state_paint,
                            pt_elem_hlt,
                            i4_hlt_elem_top_vp + pt_ctx->t_rc_scrn_vp.i4_top,
                            &t_rc_scrn_upt,
                            &t_elem_data,
                            ui2_hlt_elem));
                    }
                    else
#endif
                    {
                    INFORM_ERR(_lbcntdef_render_elem(
                        pt_ctx,
                        h_g,
                        ui2_state_paint,
                        pt_elem_hlt,
                        ui4_elem_flag,
                        i4_hlt_elem_top_vp + pt_ctx->t_rc_scrn_vp.i4_top,
                        &t_rc_scrn_upt));
                }
            }
        }
    }
    }
    
#ifdef WGL_ANIM_SUPPORT
    /*Paint the animation element,if one element is doing animation*/
    if(pt_anim_elem == NULL)
    {
        _lbcntdef_tool_check_anim_elem(pt_ctx, &pt_anim_elem);
    }
    
    if(pt_anim_elem != NULL)
    {
        _lbcntdef_render_anim_elem(
        pt_ctx,
        h_g,
        ui2_state_paint,
        pt_anim_elem,
        &pt_ctx->t_tick_inf.t_cur_rect);
    }
#endif
    
___BLOCK_END___

    INFORM_ERR(x_gl_set_clip(h_g, &pt_ctx->t_rc_scrn_lb));
    INFORM_ERR(x_gl_translate(h_g, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top));
    
    /* draw the title */
    INFORM_ERR(_lbcntdef_draw_title(
        pt_ctx,
        h_g,
        ui2_state_paint,
        pt_rc_lb_upt));

    if (pt_ctx->ui2_elem_num > 0            &&
        ui2_first < pt_ctx->ui2_elem_num    &&
        ui2_last  < pt_ctx->ui2_elem_num    )
    {
        /* draw the scrolling indicator images */
        if( !HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_SCRL_IMG) || 
            ( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) )
        {
            INFORM_ERR(_lbcntdef_draw_scroll_images(
                pt_ctx,
                h_g,
                pt_rc_lb_upt));
        }
        
        /* draw the page up/down indicator images */
        if( !HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_PAGI_IMG) || 
            ( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) )
        {
            INFORM_ERR(_lbcntdef_draw_indicator_images(
                h_g, 
                &pt_ctx->t_cvsst,
                &pt_ctx->t_scr_imgs_paging, 
                (BOOL)( (!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || (ui2_first > 0) ? TRUE : FALSE),
                (BOOL)( (!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) || (ui2_last+(UINT16)1 < pt_ctx->ui2_elem_num) ? TRUE : FALSE),
                pt_rc_lb_upt));
        }
    }
    
#ifdef WGL_LB_NO_STORE_TEXT
    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        _lbcntdef_clear_elem_buf(pt_ctx);
    }
#endif

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_if_hlt_elem_visible
 *
 * Description: This API checks if any part of the enlarged highlight element 
 *              (including the extra-margin) is visible. Some parameters are also 
 *              derived in this API. Note that This API must be called only when 
 *              the enlarged effect for highlight element is turned on.
 *
 * Inputs:  b_check_elem_in_vp  Specifies whether to search highlight element in 
 *                              viewport range or not. If it's FALSE, we ensure 
 *                              the highlighted element is NOT in vp.
 *
 *          i4_h_vp               Specifies the height of the viewport.
 *          i4_h_elem             Specifies the height of an element.
 *          pt_ex_margin          Contains the extra-margin of the highlighted element.
 *          pt_elem_vp_first      Contains the context of the first visible element.
 *          pt_elem_vp_last       Contains the context of the last visible element.
 *          i4_first_elem_top_vp  Specifies the y location of the first visible 
 *                                element's top boundary in vp's coordinates.
 *          i4_last_elem_btm_vp   Specifies the y location of the last visible 
 *                                element's bottom boundary in vp's coordinates.
 *
 * Outputs: ppt_elem_hlt_out         Contains pointer of the highlighted element.
 *          pi4_hlt_elem_top_vp_out  Contains the y location of the top boundary of 
 *                                   the highlighted element's main body in vp's coordinates.
 *
 * Returns: TRUE   If any part of the enlarged highlighted element 
 *                 (including the extra-margin) is visible. 
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_if_hlt_elem_visible(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      BOOL                        b_check_elem_in_vp,
    IN      INT32                       i4_h_vp,
    IN      INT32                       i4_h_elem,
    IN      WGL_LB_EXTRA_MARGIN_T       *pt_ex_margin,
    IN      LB_ELEM_T                   *pt_elem_vp_first,
    IN      LB_ELEM_T                   *pt_elem_vp_last,
    IN      INT32                       i4_first_elem_top_vp,
    IN      INT32                       i4_last_elem_btm_vp,
    IN      UINT16                      ui2_first,
    IN      UINT16                      ui2_last,
    OUT     LB_ELEM_T                   **ppt_elem_hlt_out,
    OUT     INT32                       *pi4_hlt_elem_top_vp_out,
    OUT     UINT16                      *pui2_hlt)
{
    LB_ELEM_T               *pt_elem;
    INT32                   i4_elem_top_vp, i4_elem_btm_vp;
    UINT16                  ui2_elem;
    
    *ppt_elem_hlt_out        = NULL;
    *pi4_hlt_elem_top_vp_out = 0;
    
    /* check if the main body of the highlighted element is visible in the vp */
    if(b_check_elem_in_vp)
    {
        pt_elem        = pt_elem_vp_first;
        i4_elem_top_vp = i4_first_elem_top_vp;
        ui2_elem = ui2_first;
        while(pt_elem)
        {
            if(HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_elem), LBEST_HLT))
            {
                *ppt_elem_hlt_out        = pt_elem;
                *pi4_hlt_elem_top_vp_out = i4_elem_top_vp;
                *pui2_hlt = ui2_elem;
                return TRUE;
            }
            
            if(pt_elem == pt_elem_vp_last)
            {
                break;
            }
            
            pt_elem         = pt_elem->pt_next;
            i4_elem_top_vp += i4_h_elem;
            ui2_elem ++;
        }
    }
    
    /* check the highlighted element backward before the first visible element in vp */
    pt_elem        = pt_elem_vp_first->pt_prev;
    i4_elem_btm_vp = i4_first_elem_top_vp;
    ui2_elem = ui2_first;
    while( pt_elem && 
           ((i4_elem_btm_vp + ((INT32)((INT16)pt_ex_margin->ui2_bottom))) > 0) )
    {
        ui2_elem --;
        if(HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_elem), LBEST_HLT))
        {
            *ppt_elem_hlt_out        = pt_elem;
            *pi4_hlt_elem_top_vp_out = i4_elem_btm_vp - i4_h_elem;
            *pui2_hlt = ui2_elem;
            return TRUE;
        }
        pt_elem         = pt_elem->pt_prev;
        i4_elem_btm_vp -= i4_h_elem;
    }
    
    /* check the highlighted element forward after the last visible element in vp */
    pt_elem        = pt_elem_vp_last->pt_next;
    i4_elem_top_vp = i4_last_elem_btm_vp;
    ui2_elem = ui2_last;
    while( pt_elem &&
           ((i4_elem_top_vp - ((INT32)((INT16)pt_ex_margin->ui2_top))) < i4_h_vp) )
    {
        ui2_elem ++;
        if(HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_elem), LBEST_HLT))
        {
            *ppt_elem_hlt_out        = pt_elem;
            *pi4_hlt_elem_top_vp_out = i4_elem_top_vp;
            *pui2_hlt = ui2_elem;
            return TRUE;
        }
        
        pt_elem         = pt_elem->pt_next;
        i4_elem_top_vp += i4_h_elem;
    }
    
    return FALSE;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_vp_params_tool_1
 *
 * Description: This API calculates some viewport related parameters.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          i4_h_elem           Specifies the element height.
 *          ui2_first           Specifies the first visible element index.
 *          ui2_last            Specifies the last visible element index.
 *
 * Outputs: ppt_elem_first_out  Contains the pointer of the first visible element context.
 *          ppt_elem_last_out   Contains the pointer of the last visible element context.
 *          pi4_h_elem_total_out  Contains the total height of all visible elements.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_vp_params_tool_1(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      INT32                       i4_h_elem,
    IN      UINT16                      ui2_first,
    IN      UINT16                      ui2_last,
    OUT     LB_ELEM_T                   **ppt_elem_first_out,
    OUT     LB_ELEM_T                   **ppt_elem_last_out,
    OUT     INT32                       *pi4_h_elem_total_out)
{
    LB_ELEM_T           *pt_elem;
    UINT16              ui2_i;
    
    if( ppt_elem_first_out == NULL || ppt_elem_last_out == NULL )
    {
        return;
    }
    _lbcntdef_get_elem(pt_ctx, ui2_first, ppt_elem_first_out, NULL);
    
    *pi4_h_elem_total_out = 0;
    pt_elem               = *ppt_elem_first_out;
    if( pt_elem == NULL )
    {
        return ;
    }

    for(ui2_i = ui2_first; ui2_i <= ui2_last; ui2_i ++)
    {
        *ppt_elem_last_out     = pt_elem;
        *pi4_h_elem_total_out += i4_h_elem;
        
        pt_elem                = pt_elem->pt_next;
    }
    
}

#ifdef WGL_ANIM_SUPPORT
static BOOL _lbcntdef_draw_anim_elem(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      LBCNTDEF_CTX_T*             pt_ctx,
    IN      WGL_CANVAS_STATUS_T*        pt_cvsst,
    IN      UINT8                       ui1_state_elem,
    IN      WGL_LB_ANIM_ELEM_EFFECT_T*  pt_anim_elem, 
    IN      UINT16                      ui2_state_paint, 
    IN      GL_RECT_T                   *pt_rc_scrn_upt)
{
    INT32               i4_ret;
    WGL_HIMG_TPL_T      h_img_elem_bk = NULL_HANDLE;
    WGL_LB_HLT_ELEM_EFFECT_T*       pt_effect;    
    GL_COLOR_T          *pt_clr_elem_bk = NULL;
    
    if(RECT_EMPTY(pt_rc_scrn_upt))
    {
        return FALSE;
    }
    
    /*The animation element is also using highlight information to draw*/
    ui1_state_elem = ui1_state_elem  | LBEST_HLT;

    /* set origin of h_g to the left-top corner of the screen */
    i4_ret = x_gl_translate(h_g, 0, 0);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);
    
    /* set clipping area of h_g */
    i4_ret = x_gl_set_clip(h_g, pt_rc_scrn_upt);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);

    pt_effect = &pt_ctx->t_effect_hlt_elem;
    if(HAS_FLAG(pt_ctx->t_tick_inf.ui2_cur_rsn,WGL_ANIM_TYPE_IMG))
    {       
        WGL_ANIM_IMG_INF_T*  pt_img_inf;
        
        pt_img_inf = &pt_ctx->t_tick_inf.t_new_tick.t_tick_inf.t_img;
        if(pt_img_inf->pt_img)
        {
            h_img_elem_bk = *pt_img_inf->pt_img;
        }
    }
    else if(pt_anim_elem->t_imgs.h_anim_hlt != NULL_HANDLE)
    {
        h_img_elem_bk = pt_anim_elem->t_imgs.h_anim_hlt;
    }
    else if( ! HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENHANCE_ELEM_BK) &&
        ! _lbcntdef_tool_check_hlt_elem_eff_enlarged(pt_effect, ui2_state_paint) )
    {
        h_img_elem_bk = pt_ctx->t_imgs_elem_bk.h_highlight;
    }
    else
    {
        pt_clr_elem_bk = _lbcntdef_select_elem_color_img(
            &pt_ctx->t_clrs_elem_bk,
            &pt_ctx->t_imgs_elem_bk,
            ui2_state_paint,
            ui1_state_elem,
            &h_img_elem_bk);
        if(h_img_elem_bk == NULL_HANDLE)
        {
            h_img_elem_bk = pt_ctx->t_imgs_elem_bk.h_highlight;
        }
        
        if(HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENHANCE_ELEM_BK))
        {
            h_img_elem_bk = _lbcntdef_select_enhanced_hlt_elem_img(
                &pt_ctx->t_effect_hlt_elem.t_imgs_bk,
                ui1_state_elem,
                ui2_state_paint);
        }
    }
    
    
    /* draw the element background */
    if(h_img_elem_bk != NULL_HANDLE)
    {
        /* fill by image */
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_elem_bk,
            pt_rc_scrn_upt,
            pt_rc_scrn_upt,
            h_g,
            pt_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else if(pt_clr_elem_bk != NULL && !WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_elem_bk))
    {
        /* fill bk color */
        INFORM_ERR(x_gl_set_color(h_g, pt_clr_elem_bk));
        INFORM_ERR(x_gl_fill_rect(h_g, pt_rc_scrn_upt));
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

static INT32 _lbcntdef_init_anim_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx)
{
    x_memset(&pt_ctx->t_anim_elem, 0, sizeof(WGL_LB_ANIM_ELEM_EFFECT_T));
    pt_ctx->t_anim_elem.ui4_style = WGL_LB_EA_STL_ONLY_ANIM_DATA;
    pt_ctx->t_anim_elem.ui1_alpha_s = 0;
    pt_ctx->t_anim_elem.ui1_alpha_e = 100;
    
    return WGLR_OK;
}

static INT32 _lbcntdef_render_anim_elem( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      GL_RECT_T                   *pt_upd_lb_rgn)
{
    GL_RECT_T*                          pt_cur_rect;
    UINT32                              ui4_style;
    GL_RECT_T                           t_scrn_upd;

    if(pt_elem == NULL || pt_upd_lb_rgn == NULL || (RECT_EMPTY(pt_upd_lb_rgn)))
    {
        return WGLR_INV_ARG;
    }
    
    pt_cur_rect = &pt_ctx->t_tick_inf.t_cur_rect;
    if(!(pt_cur_rect))
    {
        return WGLR_INV_ARG;
    }

    /*The update region is not now animation region or it has no animation doing,so do nothing.*/
    if(!RECT_INCLUDED(pt_cur_rect,pt_upd_lb_rgn) || 
       HAS_FLAG(pt_ctx->t_tick_inf.ui2_begin_flag,WGL_ANIM_TYPE_NONE))
    {
        return WGLR_OK;
    }


    _RECT_MOVE(pt_cur_rect, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top,&t_scrn_upd);

    if(HAS_FLAG(LB_GET_ANI_STY_BY_ELEM(pt_elem),WGL_LB_EA_STL_CNT_BY_ELEM))
    {
        ui4_style = LB_GET_ANI_STY_BY_ELEM(pt_elem);
    }
    else
    {
        ui4_style = pt_ctx->t_anim_elem.ui4_style;
    }

    if(HAS_FLAG(ui4_style,WGL_LB_EA_STL_ONLY_ANIM_DATA))
    {
        _lbcntdef_draw_anim_elem(
                h_g,
                pt_ctx,
                &pt_ctx->t_cvsst,
                pt_ctx->ui1_state,
                &pt_ctx->t_anim_elem,
                ui2_state_paint,
                &t_scrn_upd);
    }
    else if(HAS_FLAG(ui4_style,WGL_LB_EA_STL_ONLY_ANIM_DATA))
    {
        ;
    }

    return WGLR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_elem
 *
 * Description: This API renders an element.
 *
 * Inputs:  pt_ctx                Contains the instance context.
 *          h_g                   Contains the handle of the graphic context.
 *          ui2_state_paint       Specifies the current visual state of the widget. 
 *          pt_elem               Contains the context of the element.
 *          i4_elem_top_scrn      Specifies the vertical position of the top of element 
 *                                in screen coordinates.
 *          pt_rc_scrn_upt        Contains the dirty rect in screen coordinates.
 *                                - If the element is un-highlighted, the rect would be clipped in 
 *                                  the bound of the viewport.
 *                                - If the element is highlighted, the rect is NOT clipped and may be 
 *                                  as big as the whole listbox.
 *
 * Outputs: h_g                   Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK               Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_elem( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      UINT32                      ui4_elem_flag,
    IN      INT32                       i4_elem_top_scrn,
    IN      GL_RECT_T                   *pt_rc_scrn_upt)
{
    INT32                   i4_ret;
    GL_RECT_T               t_rc_scrn_elem, t_rc_scrn_elem_cnt;
    LBCNTDEF_ELEM_T         *pt_cnt_elem = (LBCNTDEF_ELEM_T *)pt_elem->pv_cnt_elem;
    UINT8                   ui1_state = LB_GET_STATE_BY_ELEM(pt_elem);
    
    ___BLOCK_BEGIN___
    
        SET_RECT(
            &t_rc_scrn_elem, 
            pt_ctx->t_rc_scrn_vp.i4_left,
            i4_elem_top_scrn,
            pt_ctx->t_rc_scrn_vp.i4_right,
            i4_elem_top_scrn + ((INT32)pt_ctx->ui2_elem_height));
            
        /* draw elem bk */
        if( (HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_SHOW_HLT_ELEM_BK) ||
             (!HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_SHOW_HLT_ELEM_BK) && 
              !HAS_FLAG(ui1_state, LBEST_HLT))) &&
            !((HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_NOT_SHOW_LAST_VP_ELEM_BK) &&
               HAS_FLAG(ui4_elem_flag, WGL_LB_CNT_ELEM_IS_LAST_OF_VP)) ||
              (HAS_FLAG(pt_ctx->ui4_cnt_style, WGL_LB_CNT_STL_NOT_SHOW_LAST_ELEM_BK) &&
               HAS_FLAG(ui4_elem_flag, WGL_LB_CNT_ELEM_IS_LAST_ELEM) )) )
        {
            if(!_lbcntdef_draw_elem_bk_normal(
                h_g,
                &pt_ctx->t_cvsst,
                ui1_state,
                ui2_state_paint,
                &pt_ctx->t_clrs_elem_bk,
                &pt_ctx->t_imgs_elem_bk,
                &t_rc_scrn_elem,
                pt_rc_scrn_upt))
            {
                GOTO_EOB;
            }
        }

        if(HAS_FLAG(ui1_state, LBEST_HLT))
        {
            /* draw the highlighted element bk */
            if(!_lbcntdef_draw_elem_bk_hlt(
                h_g,
                &pt_ctx->t_cvsst,
                &pt_ctx->t_effect_hlt_elem,
                ui1_state,
                ui2_state_paint,
                &pt_ctx->t_clrs_elem_bk,
                &pt_ctx->t_imgs_elem_bk,
                &pt_ctx->t_rc_scrn_lb,
                &pt_ctx->t_rc_scrn_vp,
                &t_rc_scrn_elem,
                pt_rc_scrn_upt))
            {
                GOTO_EOB;
            }
        }
#if 0        
        else
        {
            /* draw the non-highlighted element bk */
            if(!_lbcntdef_draw_elem_bk_normal(
                h_g,
                &pt_ctx->t_cvsst,
                ui1_state,
                ui2_state_paint,
                &pt_ctx->t_clrs_elem_bk,
                &pt_ctx->t_imgs_elem_bk,
                &t_rc_scrn_elem,
                pt_rc_scrn_upt))
            {
                GOTO_EOB;
            }
        }
#endif            
        /* draw items of the element */
        _RECT_INSET_TO(t_rc_scrn_elem_cnt, t_rc_scrn_elem, pt_ctx->t_inset_elem);
        if(RECT_IS_EFFECTIVE(t_rc_scrn_elem_cnt))
        {
            GL_RECT_T   t_rc_scrn_elem_cnt_upt;
            GL_RECT_T   t_rc_scrn_col, t_rc_scrn_col_upt;
            GL_RECT_T   t_rc_scrn_col_cnt, t_rc_scrn_col_cnt_upt;
            UINT8       ui1_c;
            INT32       i4_w_col, i4_scrn_col_left;
            
            /* check for quick return */
            INTERSECT_RECT(&t_rc_scrn_elem_cnt_upt, &t_rc_scrn_elem_cnt, pt_rc_scrn_upt);
            if(!RECT_IS_EFFECTIVE(t_rc_scrn_elem_cnt_upt))
            {
                GOTO_EOB;
            }
            
            i4_scrn_col_left = t_rc_scrn_elem_cnt.i4_left;
            
            for(ui1_c = 0; ui1_c < pt_ctx->ui1_col_num; ui1_c ++)
            {
                ___BLOCK_BEGIN___
                    
                    /* calc the real width of the item */
                    i4_w_col = _lbcntdef_tool_get_item_width (
                            pt_ctx->at_cols,
                            pt_ctx->at_cnt_cols,
                            ui2_state_paint,
                            pt_elem,
                            pt_cnt_elem,
                            ui1_c);
                                                                
                    /* draw the item's background */
                    SET_RECT(
                        &t_rc_scrn_col,
                        i4_scrn_col_left,
                        t_rc_scrn_elem_cnt.i4_top,
                        i4_scrn_col_left + i4_w_col,
                        t_rc_scrn_elem_cnt.i4_bottom);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col))
                    {
                        GOTO_EOB;
                    }

                    INTERSECT_RECT(&t_rc_scrn_col_upt, &t_rc_scrn_col, &t_rc_scrn_elem_cnt_upt);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_upt))
                    {
                        GOTO_EOB;
                    }
                    
                    /* set the origin and clipping of h_g */
                    _LBCNTDEF_SET_CLIP_RECT(h_g, t_rc_scrn_col, t_rc_scrn_col_upt, i4_ret);
                    
                    /* render bk */
                    INFORM_ERR(_lbcntdef_draw_item_bk(
                        h_g,
                        &pt_ctx->t_cvsst,
                        pt_ctx->at_cnt_cols,
                        &t_rc_scrn_col,
                        ui2_state_paint,
                        pt_elem,
                        pt_cnt_elem,
                        ui1_c,
                        &t_rc_scrn_col_upt));
                    
                    /* render the item's content area */
                    _RECT_INSET_TO(
                        t_rc_scrn_col_cnt,
                        t_rc_scrn_col,
                        pt_ctx->at_cnt_cols[ui1_c].t_inset);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_cnt))
                    {
                        GOTO_EOB;
                    }
                    INTERSECT_RECT(
                        &t_rc_scrn_col_cnt_upt, 
                        &t_rc_scrn_col_cnt, 
                        &t_rc_scrn_elem_cnt_upt);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_cnt_upt))
                    {
                        GOTO_EOB;
                    }
                    
                    /* set the origin and clipping of h_g */
                    _LBCNTDEF_SET_CLIP_RECT(h_g, t_rc_scrn_col_cnt, t_rc_scrn_col_cnt_upt, i4_ret);
                    
                    /* render content */
                    INFORM_ERR(_lbcntdef_render_col_data(
                        pt_ctx,
                        h_g,
                        &t_rc_scrn_col_cnt,
                        ui2_state_paint,
                        pt_elem,
                        pt_cnt_elem,
                        ui1_c,
                        &t_rc_scrn_col_cnt_upt));
                    
                ___BLOCK_END___
                    
                i4_scrn_col_left += i4_w_col;
                
                if(i4_scrn_col_left >= t_rc_scrn_elem_cnt.i4_right)
                {
                    break;
                }
            }
        }
        
    ___BLOCK_END___
    
    /* check HETSM's marker */
    if ( HAS_FLAG(ui1_state, LBEST_HLT ) )
    {
        /* check marker and reset if necessary */
        if ( ! lb_hetsm_check_marker( & pt_ctx->t_hetsm, MAKE_HETSM_MARKER(pt_cnt_elem) ) )
        {
            INFORM_ERR( lb_hetsm_reset( 
                & pt_ctx->t_hetsm, 
                MAKE_HETSM_MARKER(pt_cnt_elem) ) );
        }
        
    }
    
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_elem_bk_normal
 *
 * Description: This API draws the element background.
 *
 * Inputs:  h_g              Contains the handle of the graphic context.
 *          pt_cvsst         Contains the status of the current hosted canvas.
 *          ui1_state_elem   Specifies the state of the element.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_clrs_elem_bk  Contains the color set for the element's background.
 *          pt_imgs_elem_bk  Contains the image set for the element's background.
 *          pt_rc_scrn_elem  Contains the element rect in screen coordinates.
 *          pt_rc_scrn_upt   Contains the update rect in screen coordinates.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: TRUE   If the element bk is successfully rendered.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_draw_elem_bk_normal(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint, 
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_elem_bk, 
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem_bk,
    IN      GL_RECT_T                   *pt_rc_scrn_elem,
    IN      GL_RECT_T                   *pt_rc_scrn_upt)
{
    INT32               i4_ret;
    GL_COLOR_T          *pt_clr_elem_bk = NULL;
    WGL_HIMG_TPL_T      h_img_elem_bk = NULL_HANDLE;
    GL_RECT_T           t_rc_scrn_elem_upt = {0};
    
    /* prepare update rect in element */
    INTERSECT_RECT(
        &t_rc_scrn_elem_upt,
        pt_rc_scrn_elem,
        pt_rc_scrn_upt);
    if(!RECT_IS_EFFECTIVE(t_rc_scrn_elem_upt))
    {
        return FALSE;
    }
    
    /* set origin of h_g to the left-top corner of the screen */
    i4_ret = x_gl_translate(h_g, 0, 0);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);
    /* set clipping area of h_g */
    i4_ret = x_gl_set_clip(h_g, &t_rc_scrn_elem_upt);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);
    
    /* select a bk color & image */
    pt_clr_elem_bk = _lbcntdef_select_elem_color_img(
        pt_clrs_elem_bk,
        pt_imgs_elem_bk,
        ui2_state_paint,
        ui1_state_elem,
        &h_img_elem_bk);
    
    /* draw the element background */
    if(h_img_elem_bk != NULL_HANDLE)
    {
        /* fill by image */
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_elem_bk,
            pt_rc_scrn_elem,
            &t_rc_scrn_elem_upt,
            h_g,
            pt_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else if(!WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_elem_bk))
    {
        /* fill by color */
        INFORM_ERR(x_gl_set_color(h_g, pt_clr_elem_bk));
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_scrn_elem_upt));
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_elem_bk_hlt
 *
 * Description: This API draws the highlighted element's background.
 *
 * Inputs:  h_g              Contains the handle of the graphic context.
 *          pt_cvsst         Contains the status of the current hosted canvas.
 *          pt_effect        Contains the effect parameters for highlighted element.
 *          ui1_state_elem   Specifies the state of the element.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_clrs_elem_bk  Contains the color set for the element's background.
 *          pt_imgs_elem_bk  Contains the image set for the element's background.
 *          pt_rc_scrn_lb    Contains the listbox rect in screen coordinates.
 *          pt_rc_scrn_vp    Contains the viewport rect in screen coordinates.
 *          pt_rc_scrn_elem  Contains the element rect in screen coordinates.
 *                           This argument may contain a rect that completely 
 *                           lays outside the viewport
 *          pt_rc_scrn_upt   Contains the update rect in screen coordinates.
 *                           Note that this argument was not clipped in the bound of 
 *                           viewport, and it may be as big as the whole listbox.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: TRUE   If the element bk is successfully rendered.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_draw_elem_bk_hlt(
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint,
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_elem_bk,
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem_bk,
    IN      GL_RECT_T                   *pt_rc_scrn_lb,
    IN      GL_RECT_T                   *pt_rc_scrn_vp,
    IN      GL_RECT_T                   *pt_rc_scrn_elem,
    IN      GL_RECT_T                   *pt_rc_scrn_upt)
{
    INT32               i4_ret;
    GL_RECT_T           t_rc_scrn_elem_elg, t_rc_scrn_elem_elg_upt;
    GL_RECT_T           t_rc_scrn_elem_elg_upt_eff;
    GL_COLOR_T          *pt_clr_elem_bk = NULL;
    WGL_HIMG_TPL_T      h_img_elem_bk = NULL_HANDLE;
    
    if( ! HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENHANCE_ELEM_BK) &&
        ! _lbcntdef_tool_check_hlt_elem_eff_enlarged(pt_effect, ui2_state_paint) )
    {
        return _lbcntdef_draw_elem_bk_normal(
            h_g,
            pt_cvsst,
            ui1_state_elem,
            ui2_state_paint,
            pt_clrs_elem_bk,
            pt_imgs_elem_bk,
            pt_rc_scrn_elem,
            pt_rc_scrn_upt);
    }
    
    if(!_lbcntdef_get_hlt_elem_area(
        pt_effect,
        ui2_state_paint,
        pt_rc_scrn_lb,
        pt_rc_scrn_vp,
        pt_rc_scrn_elem,
        &t_rc_scrn_elem_elg,
        &t_rc_scrn_elem_elg_upt))
    {
        return FALSE;
    }
    
    /* prepare the effetive rect which needs to update */
    INTERSECT_RECT(
        &t_rc_scrn_elem_elg_upt_eff, 
        &t_rc_scrn_elem_elg_upt,
        pt_rc_scrn_upt);
    if(!RECT_IS_EFFECTIVE(t_rc_scrn_elem_elg_upt_eff))
    {
        return FALSE;
    }
    
    /* set origin of h_g to the left-top corner of the screen */
    i4_ret = x_gl_translate(h_g, 0, 0);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);
    /* set clipping area of h_g */
    i4_ret = x_gl_set_clip(h_g, &t_rc_scrn_elem_elg_upt_eff);
    CHECK_VALID(i4_ret == GLR_OK, FALSE);
    
    /* select a bk color & image */
    pt_clr_elem_bk = _lbcntdef_select_elem_color_img(
        pt_clrs_elem_bk,
        pt_imgs_elem_bk,
        ui2_state_paint,
        ui1_state_elem,
        &h_img_elem_bk);
    if(HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENHANCE_ELEM_BK))
    {
        h_img_elem_bk = _lbcntdef_select_enhanced_hlt_elem_img(
            &pt_effect->t_imgs_bk,
            ui1_state_elem,
            ui2_state_paint);
    }
    
    /* draw the element background */
    if(h_img_elem_bk != NULL_HANDLE)
    {
        /* fill bk image */
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_elem_bk, 
            &t_rc_scrn_elem_elg,
            &t_rc_scrn_elem_elg_upt_eff,
            h_g,
            pt_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else if(!WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_elem_bk))
    {
        /* fill bk color */
        INFORM_ERR(x_gl_set_color(h_g, pt_clr_elem_bk));
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_scrn_elem_elg_upt_eff));
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_hlt_elem_area
 *
 * Description: This API determines the effective area for painting the highlighted element 
 *              (including the extra-margin area).
 *
 * Inputs:  pt_effect           Contains the effect parameters for highlighted element.
 *          ui2_state_paint     Contains the paint state.
 *          pt_rc_lb            Contains the rect of the listbox in caller-defined coordinates.
 *          pt_rc_vp            Contains the rect of the viewport in caller-defined coordinates.
 *          pt_rc_hlt_elem      Contains the rect of the highlighted element in caller-defined coordinates.
 *
 * Outputs: pt_rc_hlt_elem_logical  Contains the logical area for the highlighted element.
 *          pt_rc_hlt_elem_paint    Contains the effective paint area for the highlighted element.
 *
 * Returns: WGLR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_get_hlt_elem_area(
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb,
    IN      GL_RECT_T                   *pt_rc_vp,
    IN      GL_RECT_T                   *pt_rc_hlt_elem,
    OUT     GL_RECT_T                   *pt_rc_hlt_elem_logical,
    OUT     GL_RECT_T                   *pt_rc_hlt_elem_paint)
{
    if(_lbcntdef_tool_check_hlt_elem_eff_enlarged(pt_effect, ui2_state_paint))
    {
        SET_RECT(
            pt_rc_hlt_elem_logical,
            pt_rc_hlt_elem->i4_left   - (INT32)((INT16)pt_effect->t_ex_margin.ui2_left),
            pt_rc_hlt_elem->i4_top    - (INT32)((INT16)pt_effect->t_ex_margin.ui2_top),
            pt_rc_hlt_elem->i4_right  + (INT32)((INT16)pt_effect->t_ex_margin.ui2_right),
            pt_rc_hlt_elem->i4_bottom + (INT32)((INT16)pt_effect->t_ex_margin.ui2_bottom));
            
        /* do clipping */
        if( HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_PUT_ABOVE_VP) )
        {
            /* check if the highlighted element is completely visible in the viewport */
            if( pt_rc_hlt_elem->i4_top >= pt_rc_vp->i4_top && 
                pt_rc_hlt_elem->i4_bottom <= pt_rc_vp->i4_bottom )
            {
                /* case 1/2: the highlighted element is completely visible in the viewport, */
                /* thus the whole highlighted element including its extra margin can */
                /* be seen even outside the viewport. */
                INTERSECT_RECT(
                    pt_rc_hlt_elem_paint,
                    pt_rc_hlt_elem_logical,
                    pt_rc_lb);
            }
            else
            {
                /* case 2/2: the highlighted element is partially visible in the viewport */
                /* thus only a part of the element that is below the top vp boundary */
                /* or below the bottom vp boundary would be rendered. */
                SET_RECT(
                    pt_rc_hlt_elem_paint,
                    MAX(pt_rc_hlt_elem_logical->i4_left, pt_rc_lb->i4_left),
                    MAX(pt_rc_hlt_elem_logical->i4_top, pt_rc_vp->i4_top),
                    MIN(pt_rc_hlt_elem_logical->i4_right, pt_rc_lb->i4_right),
                    MIN(pt_rc_hlt_elem_logical->i4_bottom, pt_rc_vp->i4_bottom) );
            }
        }
        else
        {
            INTERSECT_RECT(
                pt_rc_hlt_elem_paint,
                pt_rc_hlt_elem_logical,
                pt_rc_vp);
        }
    }
    else
    {
        WGL_RECT_COPY(pt_rc_hlt_elem_logical, pt_rc_hlt_elem);
        
        /* do clipping */
        INTERSECT_RECT(
            pt_rc_hlt_elem_paint,
            pt_rc_hlt_elem_logical,
            pt_rc_vp);
    }
    
    if(!RECT_IS_EFFECTIVE(*pt_rc_hlt_elem_paint))
    {
        return FALSE;
    }
    
    return TRUE;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_item_bk
 *
 * Description: This API draws an item's background.
 *
 * Inputs:  h_g              Contains the handle of the graphic context. 
 *                           The origin of the graphic context is set as the 
 *                           left-top corner of the item.
 *          pt_cvsst         Contains the status of the current hosted canvas.
 *          at_cnt_cols      Contains the array of content columns.
 *          pt_rc_scrn_col   Contains the item's rect in screen coordinates.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_elem          Contains the context of the item's element.
 *          pt_cnt_elem      Contains the context of the item's content element.
 *          ui1_c            Specifies the column index of the item.
 *          pt_rc_scrn_col_upt  Contains the update rect in the item in screen coordinates.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_item_bk(
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      GL_RECT_T                   *pt_rc_scrn_col,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_upt)
{
    WGL_LB_COLOR_ELEM_SET_T             *pt_clrs_item_bk = NULL;
    WGL_LB_IMG_ELEM_SET_T               *pt_imgs_item_bk = NULL;
    GL_COLOR_T                          *pt_clr_item_bk = NULL;
    WGL_HIMG_TPL_T                      h_img_item_bk = NULL_HANDLE;
    GL_RECT_T                           t_rc_g_col = {0};
    INT32                               i4_ret;

    _lbcntdef_get_item_attr(
        pt_cnt_elem,
        at_cnt_cols,
        ui1_c,
        ITEM_ATTR_BK_COLOR,
        (VOID *) &pt_clrs_item_bk);
        
    _lbcntdef_get_item_attr(
        pt_cnt_elem,
        at_cnt_cols,
        ui1_c,
        ITEM_ATTR_BK_IMAGE,
        (VOID *) &pt_imgs_item_bk);
        
    pt_clr_item_bk = _lbcntdef_select_elem_color_img(
        pt_clrs_item_bk, 
        pt_imgs_item_bk, 
        ui2_state_paint, 
        LB_GET_STATE_BY_ELEM(pt_elem), 
        & h_img_item_bk);
    
    SET_RECT_BY_SIZE(
        &t_rc_g_col, 
        0, 0, 
        RECT_W(pt_rc_scrn_col),
        RECT_H(pt_rc_scrn_col));
        
    if(h_img_item_bk != NULL_HANDLE)
    {
        GL_RECT_T       t_rc_g_col_upt;
        
        _RECT_MOVE(
            pt_rc_scrn_col_upt,
            - pt_rc_scrn_col->i4_left,
            - pt_rc_scrn_col->i4_top,
            &t_rc_g_col_upt);
            
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_item_bk,
            &t_rc_g_col,
            &t_rc_g_col_upt,
            h_g,
            pt_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else
    {
        /* draw item bk by filling color */
        
        if(!WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_item_bk) )
        {
            INFORM_ERR(x_gl_set_color(h_g, pt_clr_item_bk));
            INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_g_col));
        }
    }
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data
 *
 * Description: This API rendres an item.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context. 
 *                              The origin is set as (pt_rc_scrn_col_cnt->i4_left, pt_rc_scrn_col_cnt->i4_top)
 *                              The clipping area is set as 'pt_rc_scrn_col_cnt_upt'.
 *          pt_rc_scrn_col_cnt  Contains the content area of the item in screen coordinates.
 *          ui2_state_paint     Specifies the current visual state of the widget. 
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          pt_rc_scrn_col_cnt_upt  Contains the invalid update rect of the column content area 
 *                                  in screen's coordinates.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK              Routine successful.
 *          WGLR_INTERNAL_ERROR  Internal error.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt)
{
    INT32       i4_ret;

    switch(pt_ctx->at_cols[ui1_c].e_col_type)
    {
        case LB_COL_TYPE_TEXT:
            INFORM_ERR(_lbcntdef_render_col_data_text(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                ui2_state_paint,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt,
                pt_elem->at_items[ui1_c].t_text.pw2s_text));
        break;    
        
        case LB_COL_TYPE_IMG:
            INFORM_ERR(_lbcntdef_render_col_data_img(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt));
        break;    

        case LB_COL_TYPE_IMG_SUIT:
            INFORM_ERR(_lbcntdef_render_col_data_img_suit(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                ui2_state_paint,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt));
        break;    
        
        default:
            CATCH_FAIL(WGLR_INTERNAL_ERROR);
    }
    
    return WGLR_OK;
}

static INT32 _lbcntdef_free_elem_tsa(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c)
{
    INT32                               i4_ret;
    ANM_TSA_CTX_T*                      pt_ctx_tsa = NULL;
    
    CHECK_VALID(((pt_ctx != NULL) && (pt_cnt_elem != NULL) ), WGLR_INV_ARG);

    if (lb_hetsm_check_marker( &pt_ctx->t_hetsm, MAKE_HETSM_MARKER(pt_cnt_elem) ) )
    {
        
        /* remove the tsa belong to this column */
        /* query TSA */
        i4_ret = lb_hetsm_query_tsa( 
            &pt_ctx->t_hetsm, 
            MAKE_HETSM_MARKER(pt_cnt_elem), 
            ui1_c, 
            &pt_ctx_tsa );
            
        if (i4_ret == WGLR_OK)
        {
            if (pt_ctx_tsa)
            {
                INFORM_ERR (lb_hetsm_free_tsa (
                        &pt_ctx->t_hetsm, 
                        ui1_c));
            }
        }
        else
        {
            INFORM_ERR (i4_ret);
        }
    }

    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data_text
 *
 * Description: This API rendres a text item.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context.
 *          pt_rc_scrn_col_cnt  Contains the content area of the item in screen coordinates.
 *          ui2_state_paint     Specifies the current visual state of the widget. 
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          pt_rc_scrn_col_cnt_upt  Contains the invalid update rect of the column content area 
 *                                  in screen's coordinates.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data_text(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt,
    IN      UTF16_T                     *pw2s_text)
{
    INT32                       i4_w, i4_h;
    HFONT_T                     h_font;
    WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_text;
    GL_COLOR_T                  *pt_clr_text;
    INT32                       i4_w_bound = RECT_W(pt_rc_scrn_col_cnt);
    INT32                       i4_h_bound = RECT_H(pt_rc_scrn_col_cnt);
    UINT32                      ui4_len_line_1, ui4_len_line_1_eff;
    BOOL                        b_rendered;
    INT32                       i4_ret;
    BOOL                        b_free_tsa = FALSE;
    
    /* check for quick return */
    if(WGL_CHAR_NULL == x_uc_w2s_get(pw2s_text, 0) )
    {
        return WGLR_OK;
    }

    /* get text color */
    _lbcntdef_get_item_attr(
        pt_cnt_elem, 
        pt_ctx->at_cnt_cols, 
        ui1_c, 
        ITEM_ATTR_TEXT_COLOR, 
        (VOID *)&pt_clrs_text );
        
    pt_clr_text = _lbcntdef_select_elem_color_img(
        pt_clrs_text, 
        NULL, 
        ui2_state_paint, 
        LB_GET_STATE_BY_ELEM(pt_elem), 
        NULL);

    /* quick return if text color is transparent */    
    if(wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_text))
    {
        return WGLR_OK;
    }        

    /* update text font */
    i4_ret = _lbcntdef_update_col_item_font(
                pt_ctx,
                ui1_c,
                pt_cnt_elem,
                ui2_state_paint, 
                LB_GET_STATE_BY_ELEM(pt_elem));
    
    if (i4_ret != WGLR_OK)
    {
        INFORM_ERR(i4_ret);
    }
    
    /* get text font */
    _lbcntdef_get_item_attr(
        pt_cnt_elem, 
        pt_ctx->at_cnt_cols, 
        ui1_c, 
        ITEM_ATTR_FONT, 
        (VOID *)&h_font);
    /* get length of the first line,that is char num unit. */
    ui4_len_line_1 = _lbcntdef_tool_get_first_line_len (pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir?WGL_TEXT_DIR_LEFT_TO_RIGHT:pt_ctx->ui1_txt_dir, pw2s_text);

    /* evaluate the text, filtering out Soft-Hyphen */
    /*       ->|       |<-                          */
    /* case 1: xxxx           (ui2_len_line_1_eff == ui2_len_line_1 && i4_w <= i4_w_bound)                                           */
    /* case 2: xxxxxxxx*      (ui2_len_line_1_eff == ui2_len_line_1 && i4_w <= i4_w_bound, Soft-Hyphen at the end) -> NOT draw the Soft-Hyphen */
    /* case 3: xxxxxxxx-      (ui2_len_line_1_eff <  ui2_len_line_1 && i4_w <= i4_w_bound -> break by Soft-Hyphen                    */
    /* case 4: xxxxxxxxxxxxxx (ui2_len_line_1_eff == ui2_len_line_1 && i4_w > i4_w_bound  -> Enable Smart Cut or Enable Text Scroll) */
    i4_ret = _lbcntdef_tool_cut_line_by_soft_hyphen (
                    pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir?WGL_TEXT_DIR_LEFT_TO_RIGHT:pt_ctx->ui1_txt_dir,
                    h_font, 
                    pw2s_text, 
                    ui4_len_line_1,
                    i4_w_bound,
                    & ui4_len_line_1_eff,
                    & i4_w,
                    & i4_h );
    
     UINT32 ui4_str_len;
     ui4_str_len = x_uc_w2s_strlen(pw2s_text);
     ui4_str_len++;
    if (i4_ret != WGLR_OK)
    {
        INFORM_ERR(i4_ret);
        return WGLR_OK;
    }
    
	
    b_rendered = FALSE;
    
    /* check if hlt text scroll effect should be active */
    if( HAS_FLAG( LB_GET_STATE_BY_ELEM(pt_elem), LBEST_HLT )   &&
        HAS_FLAG( pt_ctx->t_effect_hlt_elem.ui4_style, WGL_LB_HEF_STL_SCRL_LONG_TEXT ) &&
#ifdef WGL_LB_SUPPORT_MULTI_LINE
        !HAS_FLAG( pt_ctx->ui4_style, WGL_STL_LB_SUPPORT_MULTILINE) &&
#endif
        ((IS_STATE_FOCUSED( ui2_state_paint ) && IS_STATE_ACTIVATED( ui2_state_paint ))  ||      
         HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_FORCE_SCRL_UNFOCUS) ))
    {
        if (i4_w > i4_w_bound)
        {
            INFORM_ERR( _lbcntdef_render_col_data_text_with_scrl(
                pt_ctx,
                h_g,
                ui2_state_paint,
                pt_rc_scrn_col_cnt,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                h_font,
                pt_clr_text,
                i4_w_bound,
                i4_h_bound,
                (UINT16)ui4_len_line_1,
                (UINT16)ui4_len_line_1_eff,
                i4_w,
                i4_h,
                pt_rc_scrn_col_cnt_upt,
                pw2s_text) );
            
            b_rendered = TRUE;
        }
        else 
        {
            b_free_tsa= TRUE;                        
        }
    }
    else if((!IS_STATE_FOCUSED( ui2_state_paint )) && HAS_FLAG( LB_GET_STATE_BY_ELEM(pt_elem), LBEST_HLT )   &&
        HAS_FLAG( pt_ctx->t_effect_hlt_elem.ui4_style, WGL_LB_HEF_STL_SCRL_LONG_TEXT ) &&
    (!HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_RETAIN_INF_UNFOCUS)))
    {
        b_free_tsa= TRUE;            
    }

    if(b_free_tsa == TRUE)
    {
        _lbcntdef_free_elem_tsa(pt_ctx, pt_cnt_elem, ui1_c);
    }
    
    if (!b_rendered)
    {
        INFORM_ERR( _lbcntdef_render_col_data_text_normal(
            pt_ctx,
            h_g,
            ui2_state_paint,
            pt_elem,
            pt_cnt_elem,
            ui1_c,
            h_font,
            pt_clr_text,
            i4_w_bound,
            i4_h_bound,
            ui4_len_line_1,
            ui4_len_line_1_eff,
            i4_w,
            i4_h,
            pw2s_text) );
    }    
    
    return WGLR_OK;
}

#ifndef WGL_LB_SUPPORT_MULTI_LINE
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data_text_normal
 *
 * Description: This API rendres a text item in normal mode
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context.
 *          ui2_state_paint     Specifies the current visual state of the widget. 
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          h_font              Specifies the font handle for the text item.
 *          pt_clr_text         Contains the text color.
 *          i4_w_bound          Specifies the width of the item's bound box.
 *          i4_h_bound          Specifies the height of the item's bound box.
 *          ui2_len_line_1      Specifies the length of the first line break by '\n'
 *          ui2_len_line_1_eff  Specifies the effective length of the first line after considering soft-hyphen
 *          i4_w                Specifies the text width.
 *          i4_h                Specifies the text height.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data_text_normal (
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T                  *pt_clr_text,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      ui4_len_line_1,
    IN      UINT32                      ui4_len_line_1_eff,
    IN      INT32                       i4_w,
    IN      INT32                       i4_h,
    IN      UTF16_T                     *pw2s_text)
{
    UTF16_T                     w2s_soft_hyphen[2] = {WGL_UTF16_SOFT_HYPHEN, 0};
    INT32                       i4_ret;
    INT32                       i4_w_etc, i4_h_etc;
    GL_COLOR_T                  t_clr_transp;
    UTF16_T*                    w2s_line_1;
    BOOL                        b_need_smart_cut, b_need_tail_soft_hyphen, b_need_draw_tail;
    INT32                       i4_left, i4_top;
    INT32                       i4_w_eff, i4_w_body;
    UINT32                      ui4_len_body;
    INT32                       i4_h_temp;
    UTF16_T*                    w2s_tail;
    UINT16                      ui2_len_tail;

    /* check for quick return */
    if (ui4_len_line_1_eff == 0 || i4_w == 0 || pw2s_text == NULL)
    {
        return WGLR_OK;
    }

    /* init transparent color */
    wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_transp);

    w2s_line_1 = pw2s_text;

    /* prepare info for text body string */
    /* (b_need_smart_cut, b_need_tail_soft_hyphen, i4_w_eff, ui4_len_body, i4_w_body) */
    i4_w_etc = i4_h_etc = 0;
    if ( i4_w > i4_w_bound     && 
        !HAS_FLAG( pt_ctx->ui4_style, WGL_STL_LB_NO_SMART_CUT ) )
    {
        BOOL    b_too_narrow;
        
        /* need smart cut */
        b_need_smart_cut        = TRUE;
        b_need_tail_soft_hyphen = FALSE;
        
        /* calculate the width of "..." */
        i4_ret = x_fe_get_string_size(h_font, 
                                      LBCNTDEF_SMART_CUT_CHARS, 
                                      LBCNTDEF_SMART_CUT_CHARS_LEN, 
                                      &i4_w_etc, 
                                      &i4_h_etc);
        
        if (!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
        {
            INFORM_ERR(i4_ret);
            return WGLR_OK;
        }
        
        /* try to grab as many chars as possible */
        b_too_narrow = TRUE;
        if (i4_w_bound > i4_w_etc)
        {
            UTF16_T*    w2s_temp;

            if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
            {
                i4_ret = wgl_get_max_char_in_box_ex (h_font,
                                                     w2s_line_1,
                                                     (INT32)ui4_len_line_1_eff,
                                                     TRUE,
                                                     i4_w_bound - i4_w_etc,
                                                     NULL,
                                                     w2s_soft_hyphen,
                                                     1,
                                                     0,
                                                     TRUE,
                                                     & w2s_temp,
                                                     & ui4_len_body,
                                                     & i4_w_body);

                if (i4_ret == WGLR_OK)
                {
                    if (ui4_len_body >= 1)
                    {
                        b_too_narrow = FALSE;
                    }
                }
                else
                {
                    INFORM_ERR (i4_ret);
                }
            }
            else
            {
                UINT32 ui4_str_len;
                ui4_str_len = x_uc_w2s_strlen(w2s_line_1);
                i4_ret = wgl_get_max_char_in_box_for_arab (
                                                     (INT32)ui4_len_line_1_eff,
                                                     h_font,
                                                     &w2s_line_1[ui4_str_len - 1],
                                                     (INT32)ui4_len_line_1_eff,
                                                     TRUE,
                                                     i4_w_bound - i4_w_etc,
                                                     NULL,
                                                     w2s_soft_hyphen,
                                                     1,
                                                     0,
                                                     TRUE,
                                                     & w2s_temp,
                                                     & ui4_len_body,
                                                     & i4_w_body);

                if (i4_ret == WGLR_OK)
                {
                    if (ui4_len_body >= 1)
                    {
                        b_too_narrow = FALSE;
                    }
                }
                else
                {
                    INFORM_ERR (i4_ret);
                }
            }
        }
        
        if(b_too_narrow)
        {
            /* i4_w_bound is too narrow to put any one char along with "..." */
            /* force to put one char in this case */
            
            ui4_len_body = 1;
            
            i4_ret = x_fe_get_string_size(h_font, 
                                          w2s_line_1, 
                                          1, 
                                          &i4_w_body, 
                                          &i4_h_temp);
            
            if(!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
                return WGLR_OK;
            } 
            
            i4_w_etc = 0;
            b_need_smart_cut = FALSE;
        }
        
        i4_w_eff = i4_w_body + i4_w_etc;
    }
    else
    {
        b_need_smart_cut = FALSE;
        b_need_tail_soft_hyphen = (i4_w <= i4_w_bound                  &&
                                   ui4_len_line_1_eff < ui4_len_line_1 &&
                                   WGL_UTF16_SOFT_HYPHEN == x_uc_w2s_get (w2s_line_1, ui4_len_line_1_eff - 1)) ? TRUE : FALSE;
        
        i4_w_eff = i4_w;
        
        if (b_need_tail_soft_hyphen)
        {
            INT32  i4_w_soft_hyphen;
            
            ui4_len_body = ui4_len_line_1_eff - 1;
            
            i4_ret = x_fe_get_character_size (h_font, 
                                             WGL_UTF16_SOFT_HYPHEN,
                                             & i4_w_soft_hyphen,
                                             & i4_h_temp );
                                           
            if(!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR (i4_ret);
                i4_w_soft_hyphen = 0;
            } 
            
            i4_w_body = i4_w - i4_w_soft_hyphen;
        }
        else
        {
            ui4_len_body = ui4_len_line_1_eff;
            i4_w_body    = i4_w;
        }
    }

    /* calculate the text's position */
    _lbcntdef_cal_item_obj_pos (pt_ctx, 
                                pt_cnt_elem,
                                TRUE,
                                ui1_c, 
                                XY_SEL_X | XY_SEL_Y,
                                i4_w_eff,
                                i4_h,
                                i4_w_bound,
                                i4_h_bound,
                                &i4_left,
                                &i4_top);

    /* draw the text body string */
    if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_RIGHT_TO_LEFT && !pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
    {
        UINT32 ui4_str_len;
        i4_left += i4_w_etc;
        ui4_str_len = x_uc_w2s_strlen(w2s_line_1);
        
        i4_ret = x_gl_draw_ustring_ex (h_g,
                               i4_left,
                               i4_top,
                               i4_top,
                               &w2s_line_1[ui4_str_len - ui4_len_body],
                               (UINT16) ui4_len_body,
                               w2s_soft_hyphen,
                               1,
                               h_font,
                               pt_clr_text,
                               &t_clr_transp,
                               NULL,
                               FALSE,
                               0,
                               0,
                               0,
                               0);
    }
    else
    {
        i4_ret = x_gl_draw_ustring_ex (h_g,
                               i4_left,
                               i4_top,
                               i4_top,
                               w2s_line_1,
                               (UINT16) ui4_len_body,
                               w2s_soft_hyphen,
                               1,
                               h_font,
                               pt_clr_text,
                               &t_clr_transp,
                               NULL,
                               FALSE,
                               0,
                               0,
                               0,
                               0);
    }
    

    
    if (i4_ret != GLR_OK)
    {
        INFORM_ERR (i4_ret);
    }
    
    /* draw the Smart-Cut-Symbols or Soft-Hyphen (w2s_tail, ui2_len_tail) */
    w2s_tail         = NULL;
    ui2_len_tail     = 0;
    b_need_draw_tail = FALSE;
    
    if (b_need_smart_cut)
    {
        /* draw "..." */
        w2s_tail         = LBCNTDEF_SMART_CUT_CHARS;
        ui2_len_tail     = LBCNTDEF_SMART_CUT_CHARS_LEN;
        b_need_draw_tail = TRUE;
    }
    else if (b_need_tail_soft_hyphen)
    {
        /* draw "-" */
        w2s_tail         = w2s_soft_hyphen;
        ui2_len_tail     = 1;
        b_need_draw_tail = TRUE;
    }

    if (b_need_draw_tail)
    {
        /* draw the tail */
        if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
        {
            i4_ret = x_gl_draw_ustring_ex (h_g,
                                           i4_left + i4_w_body,
                                           i4_top,
                                           i4_top,
                                           w2s_tail,
                                           ui2_len_tail,
                                           NULL,
                                           0,
                                           h_font,
                                           pt_clr_text,
                                           &t_clr_transp,
                                           NULL,
                                           FALSE,
                                           0,
                                           0,
                                           0,
                                           0 );
            
            if (i4_ret != GLR_OK)
            {
                INFORM_ERR (i4_ret);
            }
        }
        else
        {
            INT32 i4_w_tail, i4_h_tail;
            /* calculate the width of tail */
            i4_ret = x_fe_get_string_size(h_font, 
                                          LBCNTDEF_SMART_CUT_CHARS, 
                                          LBCNTDEF_SMART_CUT_CHARS_LEN, 
                                          &i4_w_tail, 
                                          &i4_h_tail);
            
            if (!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
                return WGLR_OK;
            }
            i4_ret = x_gl_draw_ustring_ex (h_g,
                                           i4_left - i4_w_tail,
                                           i4_top,
                                           i4_top,
                                           w2s_tail,
                                           ui2_len_tail,
                                           NULL,
                                           0,
                                           h_font,
                                           pt_clr_text,
                                           &t_clr_transp,
                                           NULL,
                                           FALSE,
                                           0,
                                           0,
                                           0,
                                           0 );
            
            if (i4_ret != GLR_OK)
            {
                INFORM_ERR (i4_ret);
            }
        }
    }

    return WGLR_OK;
}
#else
static INT32 _lbcntdef_get_line_inf(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      *pui4_len_line,
    IN      UINT32                      *pui4_len_line_eff,
    IN      UINT16                      *pui2_line_num,
    IN      UINT16 						ui2_max_lines,
    IN      INT32                       i4_h,
    OUT     INT32						*pi4_w,
    IN      UINT8                       ui1_c,
    IN      UTF16_T                     **ppw2s_text,
    OUT     LBCNTDEF_LINE_INF_T         **ppt_line)
{
	if(pt_ctx == NULL || *ppw2s_text == NULL || *ppt_line == NULL)
    {
        return WGLR_INV_ARG;
    }
    INT32                       i4_ret;
    WGL_BREAK_LINE_INFO_T       t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T     t_break_line_result = {0};
    UTF16_T                     w2_last_char;
	UTF16_T                     a_w2s_break_chars[] = WGL_CAN_BREAK_CHARS;/*WGL_CAN_BREAK_CHARS*/
	UTF16_T                     a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
	UTF16_T                     a_w2s_skip_chars[] = WGL_SKIP_CHARS;
	UINT16 						ui2_first_token_pos;
	UINT16 						ui2_line_num = 0;
	UTF16_T                     *pw2s_temp_text = *ppw2s_text;
	LBCNTDEF_LINE_INF_T         *pt_temp_line = *ppt_line;

	
    ui2_first_token_pos = 0;  
	t_break_line_Info.h_font = h_font;
    t_break_line_Info.w2c_force_token = a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars; 
    t_break_line_Info.w2s_string = pw2s_temp_text;  
	t_break_line_Info.i4_string_length = (INT32)*pui4_len_line;
	t_break_line_Info.i4_box_width = i4_w_bound;
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;
	while(ui2_line_num < (ui2_max_lines - 1))
    {
		  /*update parameter*/
	     ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
	                           (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
	                            0;
	     t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
	     t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;  

	 	 if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir )
        {
         	i4_ret = wgl_get_break_line_pos(
                        &t_break_line_Info,
                        &ui2_first_token_pos,
                        &t_break_line_result);
    }
    else
    {
	        i4_ret = wgl_get_break_line_pos_for_arab(
	                        t_break_line_Info.w2s_string ,
	                        &t_break_line_Info,
	                        &ui2_first_token_pos,
	                        &t_break_line_result);
	     }
        if (i4_ret != WGLR_OK)
        {
	        return i4_ret;
        }
	    if(NULL == t_break_line_result.w2s_next_start || (UTF16_T)0 == *t_break_line_result.w2s_next_start )
        {
            break;
    } 
        w2_last_char = t_break_line_Info.w2s_string[t_break_line_result.ui2_char_number - 1];
        if(NULL == x_uc_w2s_chr(a_w2s_force_chars, w2_last_char)) /*the last character is '\n'*/
    {
			pt_temp_line->ui4_length = t_break_line_result.ui2_char_number;
    }
        else /*the last character is '\n'*/
        {
            /*don't count the '\n' to display*/
            pt_temp_line->ui4_length = t_break_line_result.ui2_char_number - (UINT16)1;
        }
	    pt_temp_line->i4_h_eff = i4_h;
	    pt_temp_line->i4_h_body = i4_h;
	    pt_temp_line->pw2s_text = t_break_line_Info.w2s_string;
	    pt_temp_line->i4_w_body = (INT32)t_break_line_result.ui2_width;
	    pt_temp_line->b_need_smart_cut = FALSE;
	    pt_temp_line->b_need_tail_soft_hyphen = FALSE;
	    pt_temp_line->i4_w_etc = 0;
	    pt_temp_line->i4_h_etc = 0;
		*pui4_len_line -= t_break_line_result.ui2_char_number;
	    *pui4_len_line_eff -= t_break_line_result.ui2_char_number;
	    *pi4_w -= pt_temp_line->i4_w_body;
	    pw2s_temp_text += t_break_line_result.ui2_char_number;
	    ui2_line_num ++;
	    pt_temp_line ++; 
	    }
	*ppw2s_text = pw2s_temp_text; 
	*pui2_line_num = ui2_line_num; 
	*ppt_line =pt_temp_line;
    return WGLR_OK;
}

static INT32 _lbcntdef_get_tail_line_inf(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      ui4_len_line_1,
    IN      UINT32                      ui4_len_line_1_eff,
    IN      INT32                       i4_h,
    IN		INT32						i4_w,
    IN      UINT8                       ui1_c,
    IN      UTF16_T                     *pw2s_text,
    OUT     LBCNTDEF_LINE_INF_T         *pt_line)
{
    UTF16_T                     w2s_soft_hyphen[2] = {WGL_UTF16_SOFT_HYPHEN, 0};
    INT32                       i4_ret;
    UTF16_T*                    w2s_line_1;
    UTF16_T*                    pw2s_next;
    BOOL                        b_too_narrow ;
    INT32                       i4_h_temp ;

    if(pt_ctx == NULL || pw2s_text == NULL || pt_line == NULL)
    {
        return WGLR_INV_ARG;
    }

    w2s_line_1 = pw2s_text;

     /* prepare info for text body string */
    /* (b_need_smart_cut, b_need_tail_soft_hyphen, i4_w_eff, ui4_len_body, i4_w_body) */
    if ( i4_w > i4_w_bound     && 
        !HAS_FLAG( pt_ctx->ui4_style, WGL_STL_LB_NO_SMART_CUT ) )
    {
        
        /* need smart cut */
        pt_line->b_need_smart_cut        = TRUE;
        pt_line->b_need_tail_soft_hyphen = FALSE;
        
        /* calculate the width of "..." */
        i4_ret = x_fe_get_string_size(h_font, 
                                      LBCNTDEF_SMART_CUT_CHARS, 
                                      LBCNTDEF_SMART_CUT_CHARS_LEN, 
                                      &pt_line->i4_w_etc, 
                                      &pt_line->i4_h_etc);
        
        if (!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
        {
            INFORM_ERR(i4_ret);
            return WGLR_OK;
        }
        
        /* try to grab as many chars as possible */
        b_too_narrow = TRUE;
        if (i4_w_bound > pt_line->i4_w_etc)
        {
            if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
            {
                i4_ret = wgl_get_max_char_in_box_ex (h_font,
                                                     w2s_line_1,
                                                     (INT32)ui4_len_line_1_eff,
                                                     TRUE,
                                                     i4_w_bound - pt_line->i4_w_etc,
                                                     NULL,
                                                     w2s_soft_hyphen,
                                                     1,
                                                     0,
                                                     TRUE,
                                                     & pw2s_next,
                                                     & pt_line->ui4_length,
                                                     & pt_line->i4_w_body);

                if (i4_ret == WGLR_OK)
                {
                    if (pt_line->ui4_length >= 1)
                    {
                        b_too_narrow = FALSE;
                    }
                }
                else
                {
                    INFORM_ERR (i4_ret);
                }
            }
            else
            {
                UINT32 ui4_str_len;

                ui4_str_len = x_uc_w2s_strlen(w2s_line_1);

                i4_ret = wgl_get_max_char_in_box_for_arab (
                                                     (INT32)ui4_len_line_1_eff,
                                                     h_font,
                                                     &w2s_line_1[ui4_str_len - 1],
                                                     (INT32)ui4_len_line_1_eff,
                                                     TRUE,
                                                     i4_w_bound - pt_line->i4_w_etc,
                                                     NULL,
                                                     w2s_soft_hyphen,
                                                     1,
                                                     0,
                                                     TRUE,
                                                     & pw2s_next,
                                                     & pt_line->ui4_length,
                                                     & pt_line->i4_w_body);

                if (i4_ret == WGLR_OK)
                {
                    if (pt_line->ui4_length >= 1)
                    {
                        b_too_narrow = FALSE;
                    }
                }
                else
                {
                    INFORM_ERR (i4_ret);
                }
            }
        }
        
        if(b_too_narrow)
        {
            /* i4_w_bound is too narrow to put any one char along with "..." */
            /* force to put one char in this case */
            
            pt_line->ui4_length = 1;
            
            i4_ret = x_fe_get_string_size(h_font, 
                                          w2s_line_1, 
                                          1, 
                                          &pt_line->i4_w_body, 
                                          &i4_h_temp);
            
            if(!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
                return WGLR_OK;
            } 
            
            pt_line->i4_w_etc = 0;
            pt_line->b_need_smart_cut = FALSE;
        }
        
        pt_line->i4_w_eff = pt_line->i4_w_body + pt_line->i4_w_etc;
    }
    else
    {
        pt_line->b_need_smart_cut = FALSE;
        pt_line->b_need_tail_soft_hyphen = (i4_w <= i4_w_bound                  &&
                   ui4_len_line_1_eff < ui4_len_line_1 &&
                   WGL_UTF16_SOFT_HYPHEN == x_uc_w2s_get (w2s_line_1, ui4_len_line_1_eff - 1)) ? TRUE : FALSE;
        
        pt_line->i4_w_eff = i4_w;
        
        if (pt_line->b_need_tail_soft_hyphen)
        {
            INT32  i4_w_soft_hyphen;
            
            pt_line->ui4_length = ui4_len_line_1_eff - 1;
            
            i4_ret = x_fe_get_character_size (h_font, 
                                             WGL_UTF16_SOFT_HYPHEN,
                                             & i4_w_soft_hyphen,
                                             & i4_h_temp );
                                           
            if(!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR (i4_ret);
                i4_w_soft_hyphen = 0;
            } 
            
            pt_line->i4_w_body = i4_w - i4_w_soft_hyphen;
        }
        else
        {
            pt_line->ui4_length = ui4_len_line_1_eff;
            pt_line->i4_w_body    = i4_w;
        }
    }
                                                    
    pt_line->i4_h_eff = i4_h;
    pt_line->i4_h_body = i4_h;
    pt_line->pw2s_text = pw2s_text;

    return WGLR_OK;
}


static INT32 _lbcntdef_draw_line(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      HFONT_T                     h_font,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      GL_COLOR_T                  *pt_clr_text,
    IN      GL_COLOR_T                  *pt_clr_transp,
    IN      LBCNTDEF_LINE_INF_T*        pt_line,
    IN      UINT16                      ui2_index,
    IN      UINT16                      ui2_line_num)
{
    UTF16_T         w2s_soft_hyphen[2] = {WGL_UTF16_SOFT_HYPHEN, 0};
    INT32           i4_w_eff, i4_h_eff;
    INT32           i4_left, i4_top;
    UINT32          ui4_len_body;
    UTF16_T*        w2s_tail;
    UINT16          ui2_len_tail;
    BOOL            b_need_draw_tail;
    INT32           i4_ret;

    if( pt_line == NULL || ui2_line_num == 0)
    {
        return WGLR_INV_ARG;
    }

    i4_w_eff = pt_line->i4_w_eff;
    i4_h_eff = pt_line->i4_h_eff;

     /* calculate the text's position */
    _lbcntdef_cal_item_obj_pos_multiline(pt_ctx, 
                                pt_cnt_elem,
                                TRUE,
                                ui1_c, 
                                XY_SEL_X | XY_SEL_Y,
                                i4_w_eff,
                                i4_h_eff,
                                i4_w_bound,
                                i4_h_bound,
                                ui2_line_num,
                                ui2_index,
                                &i4_left,
                                &i4_top);

    ui4_len_body = pt_line->ui4_length;
    /* draw the text body string */
    if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_RIGHT_TO_LEFT && !pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
    {
        UINT32 ui4_str_len;

        i4_left += pt_line->i4_w_etc;
        ui4_str_len = x_uc_w2s_strlen(pt_line->pw2s_text);
        
        i4_ret = x_gl_draw_ustring_ex (h_g,
                               i4_left,
                               i4_top,
                               i4_top,
                               &pt_line->pw2s_text[ui4_str_len - ui4_len_body],
                               (UINT16) ui4_len_body,
                               w2s_soft_hyphen,
                               1,
                               h_font,
                               pt_clr_text,
                               pt_clr_transp,
                               NULL,
                               FALSE,
                               0,
                               0,
                               0,
                               0);
    }
    else
    {
        i4_ret = x_gl_draw_ustring_ex (h_g,
                               i4_left,
                               i4_top,
                               i4_top,
                               pt_line->pw2s_text,
                               (UINT16) ui4_len_body,
                               w2s_soft_hyphen,
                               1,
                               h_font,
                               pt_clr_text,
                               pt_clr_transp,
                               NULL,
                               FALSE,
                               0,
                               0,
                               0,
                               0);
    }

    /* draw the Smart-Cut-Symbols or Soft-Hyphen (w2s_tail, ui2_len_tail) */
    w2s_tail         = NULL;
    ui2_len_tail     = 0;
    b_need_draw_tail = FALSE;
    
    if (pt_line->b_need_smart_cut)
    {
        /* draw "..." */
        w2s_tail         = LBCNTDEF_SMART_CUT_CHARS;
        ui2_len_tail     = LBCNTDEF_SMART_CUT_CHARS_LEN;
        b_need_draw_tail = TRUE;
    }
    else if (pt_line->b_need_tail_soft_hyphen)
    {
        /* draw "-" */
        w2s_tail         = w2s_soft_hyphen;
        ui2_len_tail     = 1;
        b_need_draw_tail = TRUE;
    }

    if (b_need_draw_tail)
    {
        /* draw the tail */
        if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir)
        {
            i4_ret = x_gl_draw_ustring_ex (h_g,
                                           i4_left + pt_line->i4_w_body,
                                           i4_top,
                                           i4_top,
                                           w2s_tail,
                                           ui2_len_tail,
                                           NULL,
                                           0,
                                           h_font,
                                           pt_clr_text,
                                           pt_clr_transp,
                                           NULL,
                                           FALSE,
                                           0,
                                           0,
                                           0,
                                           0 );
            
            if (i4_ret != GLR_OK)
            {
                INFORM_ERR (i4_ret);
            }
        }
        else
        {
            INT32 i4_w_tail, i4_h_tail;
            /* calculate the width of tail */
            i4_ret = x_fe_get_string_size(h_font, 
                                          LBCNTDEF_SMART_CUT_CHARS, 
                                          LBCNTDEF_SMART_CUT_CHARS_LEN, 
                                          &i4_w_tail, 
                                          &i4_h_tail);
            
            if (!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
                return WGLR_OK;
            }
            i4_ret = x_gl_draw_ustring_ex (h_g,
                                           i4_left - i4_w_tail,
                                           i4_top,
                                           i4_top,
                                           w2s_tail,
                                           ui2_len_tail,
                                           NULL,
                                           0,
                                           h_font,
                                           pt_clr_text,
                                           pt_clr_transp,
                                           NULL,
                                           FALSE,
                                           0,
                                           0,
                                           0,
                                           0 );
            
            if (i4_ret != GLR_OK)
            {
                INFORM_ERR (i4_ret);
            }
        }
    }

    return WGLR_OK;
}

static INT32 _lbcntdef_render_col_data_text_normal (
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T                  *pt_clr_text,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT32                      ui4_len_line_1,
    IN      UINT32                      ui4_len_line_1_eff,
    IN      INT32                       i4_w,
    IN      INT32                       i4_h,
    IN      UTF16_T                     *pw2s_text)
{
    INT32                       i4_ret;
    GL_COLOR_T                  t_clr_transp;
    LBCNTDEF_LINE_INF_T*        pt_line_inf = NULL;
    LBCNTDEF_LINE_INF_T*        pt_line = NULL;
    UINT16                      ui2_line_num = 0;
    UINT16                      ui2_max_lines;
    UINT16                      ui2_i;
    UTF16_T*                    pw2s_start;
    UINT32                      ui4_left_len;
    UINT32                      ui4_left_len_eff;
	UINT32					    ui4_str_len;
	INT32						i4_w_left = i4_w;

    ui4_str_len = x_uc_w2s_strlen(pw2s_text);
    /* check for quick return */
    
    if (ui4_len_line_1_eff == 0 || i4_w == 0 || i4_h == 0 || pw2s_text == NULL)
    {
        return WGLR_OK;
    }

    /* init transparent color */
    wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_transp);

    pw2s_start = pw2s_text;

    do
    {/* Initial the line information memory*/
    if( HAS_FLAG( pt_ctx->ui4_style, WGL_STL_LB_SUPPORT_MULTILINE) )
    {
        ui2_max_lines = (UINT16)(i4_h_bound / i4_h);
    }
    else
    {
        ui2_max_lines = 1;
	        pt_line_inf = WGL_MEM_ALLOC(sizeof(LBCNTDEF_LINE_INF_T)*ui2_max_lines);
		    if( pt_line_inf == NULL)
		    {
		        return WGLR_OUT_OF_MEMORY;
		    }
		    x_memset(pt_line_inf, 0, sizeof(LBCNTDEF_LINE_INF_T)*ui2_max_lines);
		    pt_line = pt_line_inf;
	    	ui4_left_len = ui4_str_len;
	    	ui4_left_len_eff = ui4_str_len;
			break;
    }

    if( ui2_max_lines == 0)
    {
        return WGLR_INTERNAL_ERROR;
    }

    pt_line_inf = WGL_MEM_ALLOC(sizeof(LBCNTDEF_LINE_INF_T)*ui2_max_lines);
    if( pt_line_inf == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    x_memset(pt_line_inf, 0, sizeof(LBCNTDEF_LINE_INF_T)*ui2_max_lines);
    pt_line = pt_line_inf;
	    ui4_left_len = ui4_str_len;
	    ui4_left_len_eff = ui4_str_len;

        i4_ret = _lbcntdef_get_line_inf(pt_ctx,
                               h_font,
                               i4_w_bound,
                               i4_h_bound,
	                               &ui4_left_len,
	                               &ui4_left_len_eff,
	                               &ui2_line_num,
	                               ui2_max_lines,
                               i4_h,
	                               &i4_w_left,
                               ui1_c,
	                               &pw2s_start,
	                               &pt_line);
   }while(0);
    
    i4_ret = _lbcntdef_get_tail_line_inf(pt_ctx,
                           h_font,
                           i4_w_bound,
                           i4_h_bound,
                           ui4_left_len,
                           ui4_left_len_eff,
                           i4_h,
                           i4_w_left,
                           ui1_c,
                           pw2s_start,
                           pt_line);

    if(i4_ret != WGLR_OK )
    {
        WGL_MEM_FREE(pt_line_inf);
        return i4_ret;
    }
    ui2_line_num ++;

    /*draw the lines information */
    pt_line = pt_line_inf;
    for( ui2_i = 0; ui2_i < ui2_line_num ; ui2_i ++ )
    {
        i4_ret = _lbcntdef_draw_line(pt_ctx, 
                            pt_cnt_elem, 
                            ui1_c,
                            h_g, 
                            h_font,
                            i4_w_bound,
                            i4_h_bound,
                            pt_clr_text,
                            &t_clr_transp,
                            pt_line,
                            ui2_i,
                            ui2_line_num);
        if(i4_ret != WGLR_OK)
        {
            
            INFORM_ERR (i4_ret);
            break;
        }
        pt_line ++;
    }
    
    WGL_MEM_FREE(pt_line_inf);
    return WGLR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data_text_with_scrl
 *
 * Description: This API rendres a text item that needs to scroll.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context.
 *          ui2_state_paint     Specifies the current visual state of the widget. 
 *          pt_rc_scrn_col_cnt  Contains the content area of the item in screen coordinates.
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          h_font              Specifies the font handle for the text item.
 *          pt_clr_text         Contains the text color.
 *          i4_w_bound          Specifies the width of the item's bound box.
 *          i4_h_bound          Specifies the height of the item's bound box.
 *          ui2_len_line_1      Specifies the length of the first line break by '\n'
 *          ui2_len_line_1_eff  Specifies the effective length of the first line after considering soft-hyphen
 *          i4_w                Specifies the text width.
 *          i4_h                Specifies the text height.
 *          pt_rc_scrn_col_cnt_upt  Contains the invalid update rect of the column content area 
 *                                  in screen's coordinates.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data_text_with_scrl(
    IN      LBCNTDEF_CTX_T*             pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint, 
    IN      GL_RECT_T*                  pt_rc_scrn_col_cnt,
    IN      LB_ELEM_T*                  pt_elem, 
    IN      LBCNTDEF_ELEM_T*            pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      HFONT_T                     h_font,
    IN      GL_COLOR_T*                 pt_clr_text,
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT16                      ui2_len_line_1,
    IN      UINT16                      ui2_len_line_1_eff,
    IN      INT32                       i4_w,
    IN      INT32                       i4_h,
    IN      GL_RECT_T*                  pt_rc_scrn_col_cnt_upt,
    IN      UTF16_T                     *pw2s_text)
{
    INT32                   i4_ret, i4_ret_blk;
    ANM_TSA_CTX_T*          pt_ctx_tsa = NULL;
    ANM_TSA_INFO_T          t_tsa_info;
    BOOL                    b_read_only;
    UINT16                  ui2_start;
    UINT16                  ui2_len_eff;
    INT32                   i4_left_offset;
    BOOL                    b_end_scrl;
    BOOL                    b_req_timer_by_get_text, b_req_timer;
    GL_COLOR_T              t_clr_transp;
    UINT32                  ui4_ms_tk;
    GL_RECT_T               t_rc_scrn_col_cnt_in_vp;
    BOOL                    b_normal_text = TRUE;
    UINT32                  ui4_ms_timer_get_text;
    UINT32                  ui4_ms_timer_req;
        
    /* init transparent color */
    wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_transp);
    
    /* prepare TSA info */
    t_tsa_info.ui4_style = 0;
    
    if ( HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_FROM_FIRST_CHAR) )
    {
        WGL_SET_FLAG( t_tsa_info.ui4_style, TSA_STL_FROM_FIRST_CHAR );
    }
    
    if ( HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_TILL_LAST_CHAR) )
    {
        WGL_SET_FLAG( t_tsa_info.ui4_style, TSA_STL_TILL_LAST_CHAR );
    }
    
    if ( HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_REPEAT) )
    {
        WGL_SET_FLAG( t_tsa_info.ui4_style, TSA_STL_REPEAT );
    }
    
    ui4_ms_tk                 = x_os_get_sys_tick_period();
    t_tsa_info.h_font         = h_font;
    t_tsa_info.pw2s_text      = (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT) ? 
                                (pw2s_text) : (pw2s_text + x_uc_w2s_strlen(pw2s_text) - ui2_len_line_1);
    t_tsa_info.ui2_len_text   = ui2_len_line_1;
    t_tsa_info.i4_w_bound     = i4_w_bound;
    t_tsa_info.ui1_txt_dir    = pt_ctx->ui1_txt_dir;
    t_tsa_info.ui4_tk_delay   = pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_ms_delay / ui4_ms_tk;
    t_tsa_info.ui4_scrl_count = pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_scrl_count;
    t_tsa_info.i4_step        = 0;
    t_tsa_info.ui4_tk_start_delay = HAS_FLAG(pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_DELAY_START) ? 
                                        (pt_ctx->t_effect_hlt_elem.t_text_scrl.ui4_ms_start_delay / ui4_ms_tk) : 0;
#ifdef WGL_LB_NO_STORE_TEXT
    if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        t_tsa_info.b_no_store_text = TRUE;
    }
    else
#endif
    {
        t_tsa_info.b_no_store_text = FALSE;
    }

    i4_ret_blk  = WGLR_OK;
    b_req_timer = FALSE;
    ui4_ms_timer_req = 0;
    
___BLOCK_BEGIN___
    
    /* check marker and reset if necessary */
    if ( ! lb_hetsm_check_marker( &pt_ctx->t_hetsm, MAKE_HETSM_MARKER(pt_cnt_elem) ) )
    {
        GOTO_EOB_AT_FAIL( lb_hetsm_reset( 
            &pt_ctx->t_hetsm, 
            MAKE_HETSM_MARKER(pt_cnt_elem) ), i4_ret_blk );
    }
    
    /* query TSA */
    GOTO_EOB_AT_FAIL( lb_hetsm_query_tsa( 
        &pt_ctx->t_hetsm, 
        MAKE_HETSM_MARKER(pt_cnt_elem), 
        ui1_c, 
        &pt_ctx_tsa ), i4_ret_blk );
    
    if ( pt_ctx_tsa == NULL )
    {
        BOOL    b_req_timer_by_crt_tsa;
        UINT32  ui4_ms_timer_crt_tsa;
        
        /* create a new TSA */
        GOTO_EOB_AT_FAIL( lb_hetsm_create_tsa( 
            & pt_ctx->t_hetsm,
            ui1_c,
            & t_tsa_info,
            & pt_ctx_tsa,
            & b_req_timer_by_crt_tsa,
            & ui4_ms_timer_crt_tsa), i4_ret_blk );
        
        if (b_req_timer_by_crt_tsa)
        {
            b_req_timer = TRUE;
            ui4_ms_timer_req = ui4_ms_timer_crt_tsa;
        }
    }       
    else
    {
#ifdef WGL_LB_NO_STORE_TEXT
        if(pt_ctx_tsa->b_no_store_text == TRUE)
        {
            x_memset(pt_ctx_tsa->pw2s_buf,0,pt_ctx_tsa->ui2_len_buf);
            lb_util_w2s_strncpy_wo_ch (pt_ctx_tsa->pw2s_buf, pw2s_text, x_uc_w2s_strlen(pw2s_text), WGL_UTF16_SOFT_HYPHEN);
        }
#endif

        /* check if text and attributes of the TSA is modified */
        if ( anm_tsa_check_info_changed( pt_ctx_tsa, &t_tsa_info ) )
        {
            BOOL b_req_timer_by_upt_info;
            UINT32  ui4_ms_timer_upt_info;
            
            GOTO_EOB_AT_FAIL( anm_tsa_update_info( 
                pt_ctx_tsa, 
                & t_tsa_info, 
                & b_req_timer_by_upt_info,
                & ui4_ms_timer_upt_info ), i4_ret_blk );
                
            if (b_req_timer_by_upt_info)
            {
                b_req_timer = TRUE;
                ui4_ms_timer_req = ui4_ms_timer_upt_info;
            }
        }
    } 
    
    /* check if partial update */
    INTERSECT_RECT (&t_rc_scrn_col_cnt_in_vp, pt_rc_scrn_col_cnt, &pt_ctx->t_rc_scrn_vp);
    
    b_read_only = (RECT_IS_EFFECTIVE (t_rc_scrn_col_cnt_in_vp) &&
                        WGL_RECT_IS_INCLUDED( pt_rc_scrn_col_cnt_upt, &t_rc_scrn_col_cnt_in_vp )) ? FALSE : TRUE ;
    if(pt_ctx_tsa != NULL)
    {
    GOTO_EOB_AT_FAIL( anm_tsa_get_text( 
        pt_ctx_tsa, 
        b_read_only,
        & b_normal_text,
        & ui2_start,
        & ui2_len_eff,
        & i4_left_offset,
        & b_end_scrl,
        & b_req_timer_by_get_text,
        & ui4_ms_timer_get_text ), i4_ret_blk );
    }
    else
    {
        b_end_scrl = TRUE;
        b_normal_text  = TRUE;
        b_req_timer_by_get_text = FALSE;
    }
    
    if (b_req_timer_by_get_text)
    {
        b_req_timer = TRUE;
        ui4_ms_timer_req = ui4_ms_timer_get_text;
    }
        
    if (b_end_scrl && ! b_req_timer && pt_ctx_tsa)
    {
        INFORM_ERR (lb_hetsm_free_tsa (
                &pt_ctx->t_hetsm, 
                ui1_c));
    }
    
    if ( b_normal_text || 
        (b_end_scrl && ! b_req_timer) )
    {
        INFORM_ERR( _lbcntdef_render_col_data_text_normal(
            pt_ctx,
            h_g,
            ui2_state_paint,
            pt_elem,
            pt_cnt_elem,
            ui1_c,
            h_font,
            pt_clr_text,
            i4_w_bound,
            i4_h_bound,
            ui2_len_line_1, 
            ui2_len_line_1_eff, 
            i4_w,
            i4_h,
            pw2s_text) );
    }
    else
    {
        INT32   i4_top;
        
        _lbcntdef_cal_item_obj_pos(
            pt_ctx,
            pt_cnt_elem,
            TRUE,
            ui1_c,
            XY_SEL_Y,
            i4_w,
            i4_h,
            i4_w_bound,
            i4_h_bound,
            NULL,
            &i4_top);
        
        /* render scrolled text */
        INFORM_ERR( x_gl_draw_ustring_ex(
            h_g, 
            i4_left_offset,
            i4_top,
            i4_top,
            pw2s_text + ui2_start,
            ui2_len_eff,
            NULL,
            0,
            h_font,
            pt_clr_text,
            &t_clr_transp,
            pt_clr_text,
            FALSE,
            0,
            0,
            0,
            0 ));
        
    }
    
    if( b_req_timer )
    {
        /* request a timer ! */
        LB_TR_T         t_tr;
        UINT16          ui2_id_temp;
        
        t_tr.e_mode          = pt_ctx->t_effect_hlt_elem.t_text_scrl.b_sync ? WGL_TIMER_SYNC : WGL_TIMER_ASYNC;
        t_tr.e_purpose       = LB_TIMER_PRPS_SHT;
        t_tr.ui4_ms_delay    = ui4_ms_timer_req;
        t_tr.ui4_tk_req      = x_os_get_sys_tick();
        t_tr.u.t_sht.ui1_col = ui1_c;
        
        INFORM_ERR( anm_trq_put( &pt_ctx->t_trq, &t_tr, sizeof(LB_TR_T), &ui2_id_temp ) );
    }
    
#ifdef WGL_LB_NO_STORE_TEXT
    if(t_tsa_info.b_no_store_text == TRUE && pt_ctx_tsa != NULL && pt_ctx_tsa->pw2s_buf != NULL)
    {
        x_memset(pt_ctx_tsa->pw2s_buf,0,pt_ctx_tsa->ui2_len_buf);
    }
#endif

___BLOCK_END___

    if ( WGL_IS_FAIL( i4_ret_blk ) )
    {
        INFORM_ERR( _lbcntdef_render_col_data_text_normal(
            pt_ctx,
            h_g,
            ui2_state_paint,
            pt_elem,
            pt_cnt_elem,
            ui1_c,
            h_font,
            pt_clr_text,
            i4_w_bound,
            i4_h_bound,
            ui2_len_line_1, 
            ui2_len_line_1_eff, 
            i4_w,
            i4_h,
            pw2s_text) );
            
        return WGLR_OK;
    }
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data_img
 *
 * Description: This API rendres an image item.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context.
 *          pt_rc_scrn_col_cnt  Contains the content area of the item in screen coordinates.
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          pt_rc_scrn_col_cnt_upt  Contains the invalid update rect of the column content area 
 *                                  in screen's coordinates.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data_img(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt)
{
    INT32           i4_x, i4_y;
    UINT32          ui4_img_width, ui4_img_height;
    GL_RECT_T       t_rc_g_img;
    INT32           i4_ret;
    
    if( pt_elem->at_items[ui1_c].h_img == NULL_HANDLE)
    {
        return WGLR_OK;
    }

    i4_ret = x_wgl_img_tpl_get_size(pt_elem->at_items[ui1_c].h_img, &ui4_img_width, &ui4_img_height);
    if(!WGL_IS_FAIL(i4_ret))
    {
        _lbcntdef_cal_item_obj_pos(
            pt_ctx, 
            pt_cnt_elem,
            FALSE,
            ui1_c, 
            XY_SEL_X | XY_SEL_Y,
            (INT32)ui4_img_width, 
            (INT32)ui4_img_height, 
            RECT_W(pt_rc_scrn_col_cnt),
            RECT_H(pt_rc_scrn_col_cnt),
            &i4_x, &i4_y);
        WGL_RECT_SET(
            &t_rc_g_img, 
            i4_x, 
            i4_y, 
            i4_x + ((INT32)ui4_img_width), 
            i4_y + ((INT32)ui4_img_height) );
        if(RECT_IS_EFFECTIVE(t_rc_g_img))
        {
            GL_RECT_T   t_rc_g_upt;
            _RECT_MOVE(
                pt_rc_scrn_col_cnt_upt,
                - pt_rc_scrn_col_cnt->i4_left,
                - pt_rc_scrn_col_cnt->i4_top,
                &t_rc_g_upt);
            INFORM_ERR(wgl_img_tpl_paint(
                pt_elem->at_items[ui1_c].h_img, 
                &t_rc_g_img, 
                &t_rc_g_upt, 
                h_g, 
                &pt_ctx->t_cvsst,
                WGL_TPL_PAINT_COMPOSE));
        }
    }
    else
    {
        INFORM_ERR(i4_ret);
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_render_col_data_img_suit
 *
 * Description: This API rendres an image suit item.
 *
 * Inputs:  pt_ctx              Contains the instance context.
 *          h_g                 Contains the handle of the graphic context.
 *          pt_rc_scrn_col_cnt  Contains the content area of the item in screen coordinates.
 *          ui2_state_paint     Specifies the current visual state of the widget. 
 *          pt_elem             Contains the element context.
 *          pt_cnt_elem         Contains the content element context.
 *          ui1_c               Specifies the column index of the item.
 *          pt_rc_scrn_col_cnt_upt  Contains the invalid update rect of the column content area 
 *                                  in screen's coordinates.
 *
 * Outputs: h_g                 Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_render_col_data_img_suit(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt)
{
    WGL_HIMG_TPL_T      h_img;
    INT32               i4_x, i4_y;
    UINT32              ui4_img_width, ui4_img_height;
    GL_RECT_T           t_rc_g_img;
    INT32               i4_ret;
    
    _lbcntdef_select_item_img_by_state(  
        pt_elem->at_items[ui1_c].pt_img_suit, 
        ui2_state_paint,
        LB_GET_STATE_BY_ELEM(pt_elem),
        &h_img);
    if(h_img == NULL_HANDLE)
    {
        return WGLR_OK;
    }
    
    i4_ret = x_wgl_img_tpl_get_size(h_img, &ui4_img_width, &ui4_img_height);
    if(!WGL_IS_FAIL(i4_ret))
    {
        _lbcntdef_cal_item_obj_pos(
            pt_ctx, 
            pt_cnt_elem, 
            FALSE,
            ui1_c, 
            XY_SEL_X | XY_SEL_Y,
            (INT32)ui4_img_width, 
            (INT32)ui4_img_height, 
            RECT_W(pt_rc_scrn_col_cnt), 
            RECT_H(pt_rc_scrn_col_cnt), 
            &i4_x, &i4_y);
        WGL_RECT_SET(
            &t_rc_g_img, 
            i4_x, 
            i4_y, 
            i4_x + ((INT32)ui4_img_width), 
            i4_y + ((INT32)ui4_img_height) );
        if(RECT_IS_EFFECTIVE(t_rc_g_img))
        {
            GL_RECT_T   t_rc_g_upt;
            _RECT_MOVE(
                pt_rc_scrn_col_cnt_upt,
                - pt_rc_scrn_col_cnt->i4_left,
                - pt_rc_scrn_col_cnt->i4_top,
                &t_rc_g_upt);
            INFORM_ERR(wgl_img_tpl_paint(
                h_img, 
                &t_rc_g_img, 
                &t_rc_g_upt, 
                h_g, 
                &pt_ctx->t_cvsst,
                WGL_TPL_PAINT_COMPOSE));
        }
    }
    else
    {
        INFORM_ERR(i4_ret);
    }
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_hlt_elem_eff_enlarged
 *
 * Description: This API checks if the highlight element is effectively enlarged.
 *
 * Inputs:  pt_effect           Contains the effect parameters for highlighted element.
 *          ui2_state_paint     Contains the paint state.
 *
 * Outputs: -
 *
 * Returns: TRUE   The highlight element is effectively enlarged.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_hlt_elem_eff_enlarged(
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect,
    IN      UINT16                      ui2_state_paint)
{
   return 
    ( HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENLARGE_SIZE)  &&
       ( HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_UNFOCUS_KEEP_ENLARGE) ||
         ( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) 
       )
    ) ? TRUE : FALSE;
}    

                      
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cal_item_obj_pos
 *
 * Description: This API calculates the object position in an item.
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          pt_cnt_elem  Contains the content element context.
 *          ui1_c        Specifies the column index of the item.
 *          ui1_xy_sel   Specifies if pi4_x and/or pi4_y would be calculated and returned.
 *          i4_obj_w     Specifies the object width.
 *          i4_obj_h     Specifies the object height.
 *          i4_w_bound   Specifies the width of the item content area.
 *          i4_h_bound   Specifies the height of the item content area.
 *
 * Outputs: pi4_x        Contains the object's left position. It can be NULL if XY_SEL_X not specified.
 *          pi4_y        Contains the object's top position. It can be NULL if XY_SEL_Y not specified.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_cal_item_obj_pos( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      BOOL                        b_is_txt,
    IN      UINT8                       ui1_c,       
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y)
{
    UINT8 ui1_align;
    BOOL b_opp_text_align = pt_ctx->b_opp_txt_align;
    _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_ALIGN, (VOID *)&ui1_align);
    if(pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir_align)
    {
        b_opp_text_align = FALSE;
    }
    _lbcntdef_cal_obj_pos(pt_ctx->ui1_txt_dir, b_is_txt, b_opp_text_align, ui1_align, ui1_xy_sel, i4_obj_w, i4_obj_h, i4_w_bound, i4_h_bound, pi4_x, pi4_y);
}

#ifdef WGL_LB_SUPPORT_MULTI_LINE
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cal_item_obj_pos_multiline
 *
 * Description: This API calculates the object position in an item.
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          pt_cnt_elem  Contains the content element context.
 *          ui1_c        Specifies the column index of the item.
 *          ui1_xy_sel   Specifies if pi4_x and/or pi4_y would be calculated and returned.
 *          i4_obj_w     Specifies the object width.
 *          i4_obj_h     Specifies the object height.
 *          i4_w_bound   Specifies the width of the item content area.
 *          i4_h_bound   Specifies the height of the item content area.
 *
 * Outputs: pi4_x        Contains the object's left position. It can be NULL if XY_SEL_X not specified.
 *          pi4_y        Contains the object's top position. It can be NULL if XY_SEL_Y not specified.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_cal_item_obj_pos_multiline( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      BOOL                        b_is_txt,
    IN      UINT8                       ui1_c,       
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    IN      UINT16                      ui2_line_num,
    IN      UINT16                      ui2_index,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y)
{
    UINT8 ui1_align;
    BOOL b_opp_text_align = pt_ctx->b_opp_txt_align;
    INT32   i4_line_height;
    INT32   i4_y = 0;

    if( ui2_line_num == 0 || ui2_index >= ui2_line_num)
    {
        return ;
    }

    _lbcntdef_get_item_attr(pt_cnt_elem, pt_ctx->at_cnt_cols, ui1_c, ITEM_ATTR_ALIGN, (VOID *)&ui1_align);
    if(pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir || pt_ctx->at_cnt_cols[ui1_c].b_fixed_text_dir_align)
    {
        b_opp_text_align = FALSE;
    }

    i4_line_height = i4_h_bound / ui2_line_num;
    _lbcntdef_cal_obj_pos(pt_ctx->ui1_txt_dir, b_is_txt, b_opp_text_align, ui1_align, ui1_xy_sel, i4_obj_w, i4_obj_h, i4_w_bound, i4_line_height, pi4_x, &i4_y);

   *pi4_y = i4_y + i4_line_height * ui2_index; 
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cal_obj_pos
 *
 * Description: This API calculates an object's position according to alignment mode
 *
 * Inputs:  ui1_ttl_dir Specifies the direction for the ttl object.
 *          b_is_txt    Indicates if the object is txt.
 *          ui1_align   Specifies the alignment mode for the object.
 *          ui1_xy_sel  Specifies if the pi4_x and/or pi4_y would be calculated and returned.
 *          i4_obj_w    Specifies the width of the object.
 *          i4_obj_h    Specifies the height of the object.
 *          i4_w_bound  Specifies the width of the object's bounding area.
 *          i4_h_bound  Specifies the height of the object's bounding area.
 * 
 * Outputs: pi4_x       Contains the object's left position. It can be NULL if no XY_SEL_X specified.
 *          pi4_y       Contains the object's top position. It can be NULL if no XY_SEL_Y specified.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_cal_obj_pos(
    IN      UINT8                       ui1_ttl_dir,
    IN      BOOL                        b_is_txt,
    IN      BOOL                        b_opp_txt_align,
    IN      UINT8                       ui1_align,
    IN      UINT8                       ui1_xy_sel,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      INT32                       i4_w_bound,
    IN      INT32                       i4_h_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y)
{

    if( HAS_FLAG(ui1_xy_sel, XY_SEL_X) )
    {
        if (ui1_ttl_dir == WGL_TEXT_DIR_RIGHT_TO_LEFT && b_is_txt)
        {
            if( WGL_IS_H_RIGHT (ui1_align) )
            {
                *pi4_x = (b_opp_txt_align) ? 0 : (i4_w_bound - i4_obj_w);
            }
            else if( WGL_IS_H_CENTER(ui1_align) )
            {
                *pi4_x = (i4_w_bound - i4_obj_w) >> 1;
            }
            else
            {
                *pi4_x = (b_opp_txt_align) ? (i4_w_bound - i4_obj_w) : 0;
            }
        }
        else
        {   
            if( WGL_IS_H_LEFT(ui1_align) )
            {
                *pi4_x = (!b_is_txt) ? 0 : (b_opp_txt_align ? (i4_w_bound - i4_obj_w) : 0);
            }
            else if( WGL_IS_H_CENTER(ui1_align) )
            {
                *pi4_x = (i4_w_bound - i4_obj_w) >> 1;
            }
            else
            {
                *pi4_x = (!b_is_txt) ? (i4_w_bound - i4_obj_w) : (b_opp_txt_align ? 0 : (i4_w_bound - i4_obj_w));
            }
        }
    }

    if( HAS_FLAG(ui1_xy_sel, XY_SEL_Y) )
    {
        if( WGL_IS_V_TOP(ui1_align) )
        {
            *pi4_y = 0;
        }
        else if( WGL_IS_V_CENTER(ui1_align) )
        {
            *pi4_y = (i4_h_bound - i4_obj_h) >> 1;
        }
        else
        {
            *pi4_y = i4_h_bound - i4_obj_h;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_elem
 *
 * Description: This API gets the pointer of an element.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          ui2_idx       Specifies the element index.
 *
 * Outputs: ppt_elem      Contains the pointer of the requested element.
 *          ppt_cnt_elem  Contains the pointer of the corresponding content element.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN      UINT16                      ui2_idx, 
    OUT     LB_ELEM_T                   **ppt_elem,
    OUT     LBCNTDEF_ELEM_T             **ppt_cnt_elem)
{
    LB_ELEM_T   *pt_elem = (LB_ELEM_T *)pt_ctx->pt_elems;
    UINT16      ui2_i;
    UINT16      ui2_cache_end;

    if(ppt_elem != NULL)
    {
        *ppt_elem = NULL;
    }

    if(ppt_cnt_elem != NULL)
    {
        *ppt_cnt_elem = NULL;
    }
    
    ui2_cache_end = _LBCNTDEF_GET_CACHE_START(pt_ctx) + _LBCNTDEF_GET_ELEM_CACHE_NUM(pt_ctx);
    if( (ui2_idx < _LBCNTDEF_GET_CACHE_START(pt_ctx)) ||
        (ui2_idx >= ui2_cache_end) )
    {
        return ;
    }

    for(ui2_i=_LBCNTDEF_GET_CACHE_START(pt_ctx); ui2_i<ui2_idx; ui2_i++)
    {
        pt_elem = pt_elem->pt_next;
    }
    
    if(pt_elem == NULL || pt_elem->pv_cnt_elem == NULL)
    {
        return ;
    }

    if(ppt_elem != NULL)
    {
        *ppt_elem     = pt_elem;
    }

    if(ppt_cnt_elem != NULL)
    {
        *ppt_cnt_elem = (LBCNTDEF_ELEM_T *)pt_elem->pv_cnt_elem;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_select_elem_color_img
 *
 * Description: This API selects a color and image according to current visaul state.
 *
 * Inputs:  pt_colors        Contains the element color set.
 *          pt_imgs          Contains the element image set.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *          ui1_state_elem   Specifies the element state.
 *
 * Outputs: ph_img_out       Contains the image handle.
 *
 * Returns: The corresponding color.
 ----------------------------------------------------------------------------*/
static GL_COLOR_T *_lbcntdef_select_elem_color_img( 
    IN      WGL_LB_COLOR_ELEM_SET_T     *pt_colors,
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs,
    IN      UINT16                      ui2_state_paint,
    IN      UINT8                       ui1_state_elem,
    OUT     WGL_HIMG_TPL_T              *ph_img_out)
{
    WGL_LB_IMG_ELEM_SET_T   t_elem_imgs_empty = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
    WGL_HIMG_TPL_T          h_img_temp;

    pt_imgs    = pt_imgs ? pt_imgs : &t_elem_imgs_empty;
    ph_img_out = ph_img_out ? ph_img_out : &h_img_temp;
    
    *ph_img_out = NULL_HANDLE;
    if( HAS_FLAG(ui1_state_elem, LBEST_PUSHED ))
    {
        *ph_img_out = pt_imgs->h_pushed;
        return &pt_colors->t_pushed;
    }
    
    if( !IS_STATE_ENABLED(ui2_state_paint) || HAS_FLAG(ui1_state_elem, LBEST_DISABLED))
    {   /*The List Box looks like being in the disabled state
          or the element is in disabled state */
        if(HAS_FLAG(ui1_state_elem, LBEST_SEL) )
        {
            *ph_img_out = pt_imgs->h_selected_disable;
            return &pt_colors->t_selected_disable;
        }
        else 
        {
            *ph_img_out = pt_imgs->h_disable;
            return &pt_colors->t_disable;
        }
    }
    else if( IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) ) 
    {  /*The List Box looks like being in the enabled && focused state*/
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT) )
        {
            *ph_img_out = pt_imgs->h_highlight;
            return &pt_colors->t_highlight;
        }
        else if(HAS_FLAG(ui1_state_elem, LBEST_SEL))
        {
            *ph_img_out = pt_imgs->h_selected;
            return &pt_colors->t_selected;
        }
    }
    else
    {   /*The List Box looks like being in the enabled && unfocused/inactivated state*/
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT) )
        {
            *ph_img_out = pt_imgs->h_highlight_unfocus;
            return &pt_colors->t_highlight_unfocus;
        }
        else if(HAS_FLAG(ui1_state_elem, LBEST_SEL))
        {
            *ph_img_out = pt_imgs->h_selected;
            return &pt_colors->t_selected;
        }
    }

    *ph_img_out = pt_imgs->h_normal;
    return &pt_colors->t_normal;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_select_enhanced_hlt_elem_img
 *
 * Description: This API selects an image from WGL_LB_IMG_HLT_ELEM_SET_T 
 *              according to current visaul state.
 *
 * Inputs:  pt_imgs_hlt      Contains the element color set.
 *          ui1_state_elem   Specifies the element state. 
 *                           The element must be highlighted.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *
 * Returns: The corresponding image handle.
 ----------------------------------------------------------------------------*/
static WGL_HIMG_TPL_T _lbcntdef_select_enhanced_hlt_elem_img(
    IN      WGL_LB_IMG_HLT_ELEM_SET_T   *pt_imgs_hlt,
    IN      UINT8                       ui1_state_elem,
    IN      UINT16                      ui2_state_paint)
{
    WGL_HIMG_TPL_T  h_img_out;
    
    INFORM_INVALID(
        HAS_FLAG(ui1_state_elem, LBEST_HLT), WGLR_INTERNAL_ERROR);
    
    if(HAS_FLAG(ui1_state_elem, LBEST_PUSHED))
    {
        h_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? 
            pt_imgs_hlt->h_selected_pushed : pt_imgs_hlt->h_pushed;
    }
    else if( IS_STATE_FOCUSED(ui2_state_paint) &&
             IS_STATE_ACTIVATED(ui2_state_paint) )
    {
        h_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? 
            pt_imgs_hlt->h_selected_highlight : pt_imgs_hlt->h_highlight;
    }
    else
    {
        h_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? 
            pt_imgs_hlt->h_selected_highlight_unfocus : pt_imgs_hlt->h_highlight_unfocus;
    }
        
    return h_img_out;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_select_item_img_by_state
 *
 * Description: This API selects an image from image suit according to visual states.
 *
 * Inputs:  pt_img_suit      Contains the image suit.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *          ui1_state_elem   Specifies the element state.
 *
 * Outputs: ph_img_out       Contains the image handle.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_select_item_img_by_state( 
    IN      WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit,
    IN      UINT16                      ui2_state_paint,
    IN      UINT8                       ui1_state_elem,
    OUT     WGL_HIMG_TPL_T              *ph_img_out)
{

    *ph_img_out = NULL_HANDLE;
    
    if(HAS_FLAG(ui1_state_elem, LBEST_PUSHED))
    {
        /* listbox is in the enabled && focused state and the element is pushed */
        *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected_pushed : pt_img_suit->h_pushed;
    }
    else if(!IS_STATE_ENABLED(ui2_state_paint) || HAS_FLAG(ui1_state_elem, LBEST_DISABLED))
    {
        /* listbox is in the disabled state or the element is in disabled state */
        *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected_disable : pt_img_suit->h_disable;
    }
    else if(IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) )
    {
        /* listbox is in the enabled && focused state */
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
        {
            *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected_highlight : pt_img_suit->h_highlight;
        }
        else
        {
            *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected : pt_img_suit->h_normal;
        }
    }
    else
    {
        /*listbox is in the enabled && unfocused state */
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
        {
            *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected_highlight_unfocus : pt_img_suit->h_highlight_unfocus;
        }
        else
        {
            *ph_img_out = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_img_suit->h_selected : pt_img_suit->h_normal;
        }
    }
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_update_col_item_font
 *
 * Description: This API updates the column font and the item font according to visual states.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          pt_cnt_elem      Contains the content element context.
 *          ui1_c            Specifies the column index.
 *          ui2_state_paint  Specifies the current visual state of the widget.
 *          ui1_state_elem   Specifies the element state.
 *
 * Outputs: pt_ctx           Contains the changed instance context.
 *          pt_cnt_elem      Contains the changed content element context.
 *
 * Returns: WGLR_OK          Routine successful.
 *          < 0              Some other errors.
 ----------------------------------------------------------------------------*/

static INT32 _lbcntdef_update_col_item_font(
    IN OUT  LBCNTDEF_CTX_T      *pt_ctx,
    IN      UINT8               ui1_c,
    IN OUT  LBCNTDEF_ELEM_T     *pt_cnt_elem,
    IN      UINT16              ui2_state_paint, 
    IN      UINT16              ui1_state_elem)
{
    LBCNTDEF_ITEM_FORMAT_T      *pt_itm_fmt;
    WGL_FONT_INFO_T             t_font_info;
    INT32                       i4_ret;

    /*update item font*/
    pt_itm_fmt = pt_cnt_elem->at_items[ui1_c].pt_fmt;

    if (pt_itm_fmt)
    {
        if(IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) )
        {
            /* listbox is in the enabled && focused state */
            if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
            {
                t_font_info = pt_itm_fmt->t_item_font_info_ex.t_highlight;
            }
            else
            {
                t_font_info = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_itm_fmt->t_item_font_info_ex.t_selected : pt_itm_fmt->t_item_font_info_ex.t_normal;
            }
        }
        else
        {
            /*listbox is in the enabled && unfocused state */
            if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
            {
                t_font_info = pt_itm_fmt->t_item_font_info_ex.t_highlight_unfocused;
            }
            else
            {
                t_font_info = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_itm_fmt->t_item_font_info_ex.t_selected : pt_itm_fmt->t_item_font_info_ex.t_normal;
            }
        }
        CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &t_font_info, &pt_itm_fmt->h_font));
    }
    
    /*update column font*/
    if(IS_STATE_ACTIVATED(ui2_state_paint) && IS_STATE_FOCUSED(ui2_state_paint) )
    {
        /* listbox is in the enabled && focused state */
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
        {
            t_font_info = pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_highlight;
        }
        else
        {
            t_font_info = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_selected : pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_normal;
        }
    }
    else
    {
        /*listbox is in the enabled && unfocused state */
        if(HAS_FLAG(ui1_state_elem, LBEST_HLT))
        {
            t_font_info = pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_highlight_unfocused;
        }
        else
        {
            t_font_info = HAS_FLAG(ui1_state_elem, LBEST_SEL) ? pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_selected : pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex.t_normal;
        }
    }
    CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &t_font_info, &pt_ctx->at_cnt_cols[ui1_c].h_font));
 
    return WGLR_OK;
}

 
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_select_lb_color
 *
 * Description: This API selects a color for listbox.
 *
 * Inputs:  ui2_state_paint  Specifies the current visual state of the widget.
 *          pt_clrs_std      Contains the color set.
 *
 * Outputs: ppt_clr_out      Contains the pointer of the color.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_select_lb_color(
    IN      UINT16                      ui2_state_paint,
    IN      WGL_COLOR_STD_SET_T         *pt_clrs_std, 
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

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_select_lb_img
 *
 * Description: This API selects the background image.
 *
 * Inputs:  ui2_state_paint  Specifies the current visual state of the widget.
 *          pt_imgs_std      Contains the image set.
 *
 * Outputs: ph_img_out       Contains the image handle.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_select_lb_img( 
    IN      UINT16                      ui2_state_paint,
    IN      WGL_IMG_STD_SET_T           *pt_imgs_std, 
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

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_elem_bk_color_set
 *
 * Description: This API gets the system default color set for element background.
 *
 * Inputs:  pt_cvsst        Contains the status of the current hosted canvas.
 *
 * Outputs: pt_clrs_def     Contains the default color set for element background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_elem_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_def)
{
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM, pt_cvsst, &pt_clrs_def->t_normal)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_normal);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_DIS, pt_cvsst, &pt_clrs_def->t_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_disable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_HT, pt_cvsst, &pt_clrs_def->t_highlight)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_highlight);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_HT_UNFOCUS, pt_cvsst, &pt_clrs_def->t_highlight_unfocus)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_highlight_unfocus);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_PUSHED, pt_cvsst, &pt_clrs_def->t_pushed)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_pushed);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_SEL, pt_cvsst, &pt_clrs_def->t_selected)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_selected);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_SEL_DIS, pt_cvsst, &pt_clrs_def->t_selected_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_def->t_selected_disable);
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_elem_text_color_set
 *
 * Description: This API gets the system default color set for element text.
 *
 * Inputs:  pt_cvsst      Contains the status of the current hosted canvas.
 *
 * Outputs: pt_clrs_text  Contains the default color set for element text.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_elem_text_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_COLOR_ELEM_SET_T     *pt_clrs_text)
{
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT, pt_cvsst, &pt_clrs_text->t_normal)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_normal);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_DIS, pt_cvsst, &pt_clrs_text->t_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_disable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_HT, pt_cvsst, &pt_clrs_text->t_highlight)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_highlight);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_HT_UNFOCUS, pt_cvsst, &pt_clrs_text->t_highlight_unfocus)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_highlight_unfocus);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_PUSHED, pt_cvsst, &pt_clrs_text->t_pushed)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_pushed);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_SEL, pt_cvsst, &pt_clrs_text->t_selected)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_selected);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_ELEM_TEXT_SEL_DIS, pt_cvsst, &pt_clrs_text->t_selected_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs_text->t_selected_disable);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_bk_color_set
 *
 * Description: This API gets the system default color set for listbox background.
 *
 * Inputs:  pt_cvsst    Contains the status of the current hosted canvas.
 *
 * Outputs: pt_clrs     Conatins the default color set for listbox background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs)
{
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_BK, pt_cvsst, &pt_clrs->t_enable) ) )
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_enable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_BK_DIS, pt_cvsst, &pt_clrs->t_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_disable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_BK_HT, pt_cvsst, &pt_clrs->t_highlight)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_highlight);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_fg_color_set
 *
 * Description: This API gets the system default color set for listbox foreground.
 *
 * Inputs:  pt_cvsst    Contains the status of the current hosted canvas.
 *
 * Outputs: pt_clrs     Contains the default color set for listbox foreground.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_fg_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs)
{
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_FG, pt_cvsst, &pt_clrs->t_enable) ) )
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_enable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_FG_DIS, pt_cvsst, &pt_clrs->t_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_disable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_FG_HT, pt_cvsst, &pt_clrs->t_highlight)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_highlight);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_vp_bk_color_set
 *
 * Description: This API gets the system default color set for viewport background.
 *
 * Inputs:  pt_cvsst    Contains the status of the current hosted canvas.
 *
 * Outputs: pt_clrs     Conatins the default color set for viewport background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_vp_bk_color_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs)
{
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_VP, pt_cvsst, &pt_clrs->t_enable) ) )
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_enable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_VP_DIS, pt_cvsst, &pt_clrs->t_disable)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_disable);
    }
    if(WGL_IS_FAIL(wgl_get_system_color(WGL_SYS_CLR_LB_VP_HT, pt_cvsst, &pt_clrs->t_highlight)))
    {
        wgl_cvsst_clear_color(pt_cvsst, &pt_clrs->t_highlight);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_elem_bk_img_set
 *
 * Description: This API gets the system default image set for element background.
 *
 * Inputs:  pt_cvsst      Contains the status of the current hosted canvas.
 *
 * Outputs: pt_imgs_elem  Contains the default image set for element background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_sys_lb_elem_bk_img_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem)
{
    UINT32 ui4_tmp_w, ui4_tmp_h;
    WGL_IMG_ST_T e_tmp_st;
    
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_normal))) 
    {
        pt_imgs_elem->h_normal = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_DIS, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_disable))) 
    {
        pt_imgs_elem->h_disable = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_HT, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_highlight))) 
    {
        pt_imgs_elem->h_highlight = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_HT_UNFOCUS, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_highlight_unfocus))) 
    {
        pt_imgs_elem->h_highlight_unfocus = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_PUSHED, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_pushed))) 
    {
        pt_imgs_elem->h_pushed = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_SEL, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_selected))) 
    {
        pt_imgs_elem->h_selected = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_ELEM_SEL_DIS, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs_elem->h_selected_disable))) 
    {
        pt_imgs_elem->h_selected_disable = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_sys_lb_bk_img_set
 *
 * Description: This API gets the system default image set for listbox background.
 *
 * Inputs:  pt_cvsst  Contains the status of the current hosted canvas.
 *
 * Outputs: pt_imgs   Contains the default image set for listbox background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID  _lbcntdef_get_sys_lb_bk_img_set(
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    OUT     WGL_IMG_STD_SET_T           *pt_imgs)
{
    UINT32 ui4_tmp_w, ui4_tmp_h;
    WGL_IMG_ST_T e_tmp_st;
    
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs->t_enable))) 
    {
        pt_imgs->t_enable = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_DIS, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs->t_disable)))
    {
        pt_imgs->t_disable = NULL_HANDLE;
    }
    if(WGL_IS_FAIL(wgl_get_system_image(WGL_SYS_IMG_LB_HT, pt_cvsst, &e_tmp_st, &ui4_tmp_w, &ui4_tmp_h, &pt_imgs->t_highlight)) )
    {
        pt_imgs->t_highlight = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_dft_vp_bk_img_set
 *
 * Description: This API gets the system default image set for viewport background.
 *
 * Inputs:  -
 *
 * Outputs: pt_imgs_bk_vp  Contains the default image set for listbox viewport background.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID  _lbcntdef_set_dft_vp_bk_img_set(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_bk_vp)
{
    pt_imgs_bk_vp->t_enable    = NULL_HANDLE;
    pt_imgs_bk_vp->t_disable   = NULL_HANDLE;
    pt_imgs_bk_vp->t_highlight = NULL_HANDLE;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_set_lb_color_set
 *
 * Description: This API setups the standard color set from color info.
 *
 * Inputs:  pt_clr_info_src  Contains the color info.
 *
 * Outputs: pt_clrs_dst      Contains the standard color set.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_tool_set_lb_color_set(
    OUT     WGL_COLOR_STD_SET_T         *pt_clrs_dst, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info_src)
{

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
            WS_ASSERT(FALSE);
    };
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_set_lb_bk_img_set
 *
 * Description: This API setups a standard image set from image info.
 *
 * Inputs:  pt_img_info_src  Contains the image info.
 *
 * Outputs: pt_imgs_dst      Contains the standard image set.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_tool_set_lb_bk_img_set(
    OUT     WGL_IMG_STD_SET_T           *pt_imgs_dst, 
    IN      WGL_IMG_INFO_T              *pt_img_info_src)
{
    
    switch(pt_img_info_src->e_type) 
    {
        case WGL_IMG_SET_BASIC:
            pt_imgs_dst->t_enable = pt_img_info_src->u_img_data.t_basic.t_enable;
            pt_imgs_dst->t_disable = pt_img_info_src->u_img_data.t_basic.t_disable;
            pt_imgs_dst->t_highlight = pt_img_info_src->u_img_data.t_basic.t_enable;
            break;
        case WGL_IMG_SET_STANDARD:
            pt_imgs_dst->t_enable = pt_img_info_src->u_img_data.t_standard.t_enable;
            pt_imgs_dst->t_disable = pt_img_info_src->u_img_data.t_standard.t_disable;
            pt_imgs_dst->t_highlight = pt_img_info_src->u_img_data.t_standard.t_highlight;
            break;
        case WGL_IMG_SET_EXTEND:
            pt_imgs_dst->t_enable = pt_img_info_src->u_img_data.t_extend.t_enable;
            pt_imgs_dst->t_disable = pt_img_info_src->u_img_data.t_extend.t_disable;
            pt_imgs_dst->t_highlight = pt_img_info_src->u_img_data.t_extend.t_highlight;
            break;
        default:
            WS_ASSERT(FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_item_attr
 *
 * Description: This API retrieves an attribute from an item.
 *
 * Inputs:  pt_cnt_elem  Contains the context of an content element.
 *          at_cnt_cols  Contains the columns info.
 *          ui1_c        Specifies the column index of the item.
 *          ui2_attr_id  Specifies the attribute id.
 *
 * Outputs: pv_attr_ret  Contains the value of the requested attribute.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_get_item_attr(
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT8                       ui1_c, 
    IN      UINT16                      ui2_attr_id, 
    OUT     VOID                        *pv_attr_ret)
{
    LBCNTDEF_ITEM_FORMAT_T              *pt_itm_fmt;
    
    pt_itm_fmt = pt_cnt_elem->at_items[ui1_c].pt_fmt;
    
    switch( ui2_attr_id ) 
    {
        case ITEM_ATTR_ALIGN:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_ALIGN))
            {
                *((UINT8 *) pv_attr_ret) = pt_itm_fmt->ui1_align;
            }
            else
            {
                *((UINT8 *) pv_attr_ret) = at_cnt_cols[ui1_c].ui1_align;
            }
            break;
        case ITEM_ATTR_BK_COLOR:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_BK))
            {
                *((WGL_LB_COLOR_ELEM_SET_T **) pv_attr_ret) = &pt_itm_fmt->t_clrs_bk;
            }
            else
            {
                *((WGL_LB_COLOR_ELEM_SET_T **) pv_attr_ret) = &at_cnt_cols[ui1_c].t_clrs_bk;
            }
            break;
        case ITEM_ATTR_TEXT_COLOR:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_TEXT))
            {
                *((WGL_LB_COLOR_ELEM_SET_T **) pv_attr_ret) = &pt_itm_fmt->t_clrs_text;
            }
            else
            {
                *((WGL_LB_COLOR_ELEM_SET_T **) pv_attr_ret) = &at_cnt_cols[ui1_c].t_clrs_text;
            }
            break;
        case ITEM_ATTR_FONT:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT))
            {
                *((HFONT_T *) pv_attr_ret) = pt_itm_fmt->h_font;
            }
            else
            {
                *((HFONT_T *) pv_attr_ret) = at_cnt_cols[ui1_c].h_font;
            }
            break;
        case ITEM_ATTR_FONT_EX:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT_EX))
            {
                *((WGL_LB_FONT_INFO_EX_T *) pv_attr_ret) = pt_itm_fmt->t_item_font_info_ex;
            }
            else
            {
                *((WGL_LB_FONT_INFO_EX_T *) pv_attr_ret) = at_cnt_cols[ui1_c].t_col_font_info_ex;
            }
            break;
        case ITEM_ATTR_BK_IMAGE:
            if (pt_itm_fmt && HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_IMGS_BK))
            {
                *((WGL_LB_IMG_ELEM_SET_T **) pv_attr_ret) = &pt_itm_fmt->t_imgs_bk;
            }
            else
            {
                *((WGL_LB_IMG_ELEM_SET_T **) pv_attr_ret) = &at_cnt_cols[ui1_c].t_imgs_bk;
            }
            break;
        default:
            WS_ASSERT(FALSE);
    }    
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_item_attr
 *
 * Description: This API sets the attribute of an item.
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          ui2_idx      Specifies the element index of the item.
 *          ui1_c        Specifies the column index of the item.
 *          ui2_attr_id  Specifies the attribute id.
 *          pv_value     Specifies the value of the attribute.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_item_attr(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id,
    IN      VOID                        *pv_value)
{
    LB_ELEM_T               *pt_elem;
    LBCNTDEF_ELEM_T         *pt_cnt_elem;
    LBCNTDEF_ITEM_FORMAT_T  *pt_itm_fmt;
    INT32                   i4_ret;

    _lbcntdef_get_elem(pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
    
    if(pt_elem == NULL || pt_cnt_elem == NULL)
    {/*The idx is out of the range*/
        return WGLR_INV_ARG;    
    }

    if(pt_cnt_elem->at_items[ui1_c].pt_fmt == NULL)
    {
        i4_ret = wgl_npm_alloc (&pt_ctx->t_npm_itm_fmt, 
                                (VOID **) &pt_cnt_elem->at_items[ui1_c].pt_fmt);
        
        if(i4_ret != WGLR_OK)
        {
            INFORM_ERR (i4_ret);
            return i4_ret;
        }

        /*initialize the item format attributes*/
        pt_itm_fmt            = pt_cnt_elem->at_items[ui1_c].pt_fmt;
        pt_itm_fmt->ui2_flag  = 0;
        pt_itm_fmt->ui1_align = pt_ctx->at_cnt_cols[ui1_c].ui1_align;
        pt_itm_fmt->h_font    = pt_ctx->at_cnt_cols[ui1_c].h_font;
        pt_itm_fmt->t_item_font_info_ex= pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex;
        _LBE_COLOR_SET_COPY(&pt_itm_fmt->t_clrs_bk,   &pt_ctx->at_cnt_cols[ui1_c].t_clrs_bk);
        _LBE_COLOR_SET_COPY(&pt_itm_fmt->t_clrs_text, &pt_ctx->at_cnt_cols[ui1_c].t_clrs_text);
        _LBE_IMG_SET_COPY  (&pt_itm_fmt->t_imgs_bk,   &pt_ctx->at_cnt_cols[ui1_c].t_imgs_bk);
    }
    else
    {
        pt_itm_fmt = pt_cnt_elem->at_items[ui1_c].pt_fmt;
    }
    
    switch(ui2_attr_id)
    {
        case ITEM_ATTR_ALIGN:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_ALIGN);
            pt_itm_fmt->ui1_align = WGL_POINTER_TO_UINT8(pv_value);
        break;
        
        case ITEM_ATTR_BK_COLOR:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_BK);
            _LBE_COLOR_SET_COPY(&pt_itm_fmt->t_clrs_bk, pv_value);
        break;

        case ITEM_ATTR_TEXT_COLOR:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_TEXT);
            _LBE_COLOR_SET_COPY(&pt_itm_fmt->t_clrs_text, pv_value);
        break;
        
        case ITEM_ATTR_FONT:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT);
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT_EX);
            if (pv_value)
            {
                CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, (WGL_FONT_INFO_T*) pv_value, &pt_itm_fmt->h_font));
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex.t_normal, (WGL_FONT_INFO_T*) pv_value, sizeof(WGL_FONT_INFO_T));
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex.t_selected, (WGL_FONT_INFO_T*) pv_value, sizeof(WGL_FONT_INFO_T));
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex.t_highlight, (WGL_FONT_INFO_T*) pv_value, sizeof(WGL_FONT_INFO_T));
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex.t_highlight_unfocused, (WGL_FONT_INFO_T*) pv_value, sizeof(WGL_FONT_INFO_T));
            }
            else
            {
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex, &pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex, sizeof(WGL_LB_FONT_INFO_EX_T));
                CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_itm_fmt->t_item_font_info_ex.t_normal, &pt_itm_fmt->h_font)); 
            }
        break;

        case ITEM_ATTR_BK_IMAGE:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_IMGS_BK);
            if (pv_value)
            {
                _LBE_IMG_SET_COPY (&pt_itm_fmt->t_imgs_bk, (WGL_LB_IMG_ELEM_SET_T *) pv_value);
            }
            else
            {
                _LBE_IMG_SET_CLEAR (&pt_itm_fmt->t_imgs_bk);
            }
        break;

        case ITEM_ATTR_FONT_EX:
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT_EX);
            WGL_SET_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT);
            if (pv_value)
            {
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex, (WGL_LB_FONT_INFO_EX_T*) pv_value, sizeof(WGL_LB_FONT_INFO_EX_T));
                CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_itm_fmt->t_item_font_info_ex.t_normal, &pt_itm_fmt->h_font));
            }
            else
            {
                x_memcpy(&pt_itm_fmt->t_item_font_info_ex, &pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex, sizeof(WGL_LB_FONT_INFO_EX_T));
                CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_itm_fmt->t_item_font_info_ex.t_normal, &pt_itm_fmt->h_font)); 
            }
        break;
        
        default:
            return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_item_use_def
 *
 * Description: This API set an item to use default attributes
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          ui2_idx      Specifies the element index of the item.
 *          ui1_c        Specifies the column index of the item.
 *          ui2_attr_id  Specifies the attribute id.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_item_use_def(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id)
{
    LBCNTDEF_ELEM_T     *pt_cnt_elem;
    
    _lbcntdef_get_elem(pt_ctx, ui2_idx, NULL, &pt_cnt_elem);
    if(pt_cnt_elem == NULL)
    {
        return WGLR_OK;
    }
    
    if (pt_cnt_elem->at_items[ui1_c].pt_fmt)
    {
        LBCNTDEF_ITEM_FORMAT_T  *pt_itm_fmt;
        
        pt_itm_fmt = pt_cnt_elem->at_items[ui1_c].pt_fmt;
        
        switch(ui2_attr_id)
        {
            case ITEM_ATTR_ALIGN:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_ALIGN);
            break;
            
            case ITEM_ATTR_BK_COLOR:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_BK);
            break;
    
            case ITEM_ATTR_TEXT_COLOR:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_TEXT);
            break;
            
            case ITEM_ATTR_FONT:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT);
            break;

            case ITEM_ATTR_FONT_EX:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT_EX);
            break;   
    
            case ITEM_ATTR_BK_IMAGE:
                WGL_CLEAR_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_IMGS_BK);
            break;
            
            default:
                return WGLR_INV_ARG;
        }
        
        if (pt_itm_fmt->ui2_flag == 0)
        {
            INT32   i4_ret;
            
            /* recycle the item format info object */
            i4_ret = wgl_npm_free (&pt_ctx->t_npm_itm_fmt, (VOID *) pt_cnt_elem->at_items[ui1_c].pt_fmt);
            
            if (i4_ret != WGLR_OK)
            {
                INFORM_ERR (i4_ret);
            }
            
            pt_cnt_elem->at_items[ui1_c].pt_fmt = NULL;
        }
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_item_use_def
 *
 * Description: This API get if an item is using default attributes or not
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          ui2_idx      Specifies the element index of the item.
 *          ui1_c        Specifies the column index of the item.
 *          ui2_attr_id  Specifies the attribute id.
 *
 * Outputs: pb_use_def   Contains the answer.
 *
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_get_item_use_def(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    IN      UINT16                      ui2_attr_id,
    OUT     BOOL*                       pb_use_def)
{
    LBCNTDEF_ELEM_T     *pt_cnt_elem;

    *pb_use_def = TRUE;
    
    _lbcntdef_get_elem(pt_ctx, ui2_idx, NULL, &pt_cnt_elem);
    if(pt_cnt_elem == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    if (pt_cnt_elem->at_items[ui1_c].pt_fmt)
    {
        LBCNTDEF_ITEM_FORMAT_T  *pt_itm_fmt;
        
        pt_itm_fmt = pt_cnt_elem->at_items[ui1_c].pt_fmt;
    
        switch(ui2_attr_id)
        {
            case ITEM_ATTR_ALIGN:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_ALIGN) ? FALSE : TRUE;
            break;
            
            case ITEM_ATTR_BK_COLOR:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_BK) ? FALSE : TRUE;
            break;
    
            case ITEM_ATTR_TEXT_COLOR:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_CLRS_TEXT) ? FALSE : TRUE;
            break;
            
            case ITEM_ATTR_FONT:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT) ? FALSE : TRUE;
            break;
            
            case ITEM_ATTR_FONT_EX:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_FONT_EX) ? FALSE : TRUE;
            break;

            case ITEM_ATTR_BK_IMAGE:
                *pb_use_def = HAS_FLAG (pt_itm_fmt->ui2_flag, LBCNTDEF_ITEM_FMT_FLAG_IMGS_BK) ? FALSE : TRUE;
            break;
            
            default:
                return WGLR_INV_ARG;
        }
    }
    
    return WGLR_OK;
}    

    
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_title_text
 *
 * Description: This API sets the text of title.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          w2s_text      Contains the new text. If w2s_text is NULL, 
 *                        the title text would be cleared as empty.
 *          ui2_len_text  Specifies the text length.
 *
 * Outputs: pt_ctx        Contains the changed instance context.
 *
 * Returns: WGLR_OK             Routine successful.
 *          WGLR_INV_ARG        Invalid argument.
 *          WGLR_OUT_OF_MEMORY  Out of memory.
 *          < 0                 Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_title_text(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      UTF16_T                     *w2s_text,  
    IN      UINT16                      ui2_len_text)
{
    if(w2s_text==NULL && ui2_len_text != 0)
    {
        return WGLR_INV_ARG;
    }

    if(w2s_text==NULL || ui2_len_text==0)
    {
        pt_ctx->ui2_len_ttl = 0;
    }
    else
    {
        INT32   i4_w, i4_h, i4_ret;
        
        i4_w = i4_h = 0;
        pt_ctx->ui2_len_ttl = 0;
        if(ui2_len_text > pt_ctx->ui2_len_ttl_buf)
        {   /* expand the title text buffer */
            if(pt_ctx->w2s_text_ttl != NULL)
            {
                WGL_MEM_FREE(pt_ctx->w2s_text_ttl);
                pt_ctx->w2s_text_ttl = NULL;
                WGL_MEM_FREE(pt_ctx->w2s_text_ttl_raw);
                pt_ctx->w2s_text_ttl_raw = NULL;
                pt_ctx->ui2_len_ttl_buf = 0;
            #ifdef CLI_LVL_ALL     
                pt_ctx->ui4_mem_size -= (UINT32)(pt_ctx->ui2_len_ttl_buf * UTF16_MAX_LEN * 2);
            #endif  
            }
            pt_ctx->ui2_len_ttl_buf = ui2_len_text * (UINT16)4;
            pt_ctx->w2s_text_ttl = (UTF16_T *)WGL_MEM_ALLOC( (SIZE_T)(pt_ctx->ui2_len_ttl_buf * UTF16_MAX_LEN) );
            pt_ctx->w2s_text_ttl_raw= (UTF16_T *)WGL_MEM_ALLOC( (SIZE_T)(pt_ctx->ui2_len_ttl_buf * UTF16_MAX_LEN) );
            if(pt_ctx->w2s_text_ttl==NULL || pt_ctx->w2s_text_ttl_raw==NULL )
            {
                pt_ctx->ui2_len_ttl = 0;
                pt_ctx->ui2_len_ttl_raw= 0;
                pt_ctx->ui2_len_ttl_buf = 0;
                return WGLR_OUT_OF_MEMORY;
            }
            #ifdef CLI_LVL_ALL     
                pt_ctx->ui4_mem_size += (UINT32)(pt_ctx->ui2_len_ttl_buf * UTF16_MAX_LEN * 2);
            #endif  
        }
        
        if(pt_ctx->w2s_text_ttl && pt_ctx->w2s_text_ttl_raw && ui2_len_text > 0)
        {
            UINT32  ui4_eff_len;
            UINT32 ui4_str_len;
            ui4_str_len = x_uc_w2s_strlen(w2s_text);
            
            x_uc_w2s_strncpy(pt_ctx->w2s_text_ttl_raw, w2s_text, ui4_str_len);
            pt_ctx->ui2_len_ttl_raw= (UINT16)ui4_str_len;
            if(check_exist_arabic(w2s_text,(UINT16)ui4_str_len))
            {
            
               UTF16_T* ps_arab_str; 
               UINT16 ui2_arab_str_len = 0;
               VOID* pv_reorder_attr = NULL;
               
               ps_arab_str = (UTF16_T *)WGL_MEM_ALLOC( (SIZE_T)((ui4_str_len+1) * UTF16_MAX_LEN) );
               if(ps_arab_str == NULL)
               {
                   return WGLR_OUT_OF_MEMORY;
               }
               x_memset(ps_arab_str,WGL_CHAR_NULL,UTF16_MAX_LEN * (ui4_str_len+1));                
               if(pt_ctx->t_arab_reorder_attr.b_find_init_level == FALSE)
               {
                   pv_reorder_attr = &pt_ctx->t_arab_reorder_attr;
               }

               i4_ret = reorder_arabic(w2s_text,(UINT16)ui4_str_len,pv_reorder_attr,ps_arab_str,&ui2_arab_str_len); 
               if (i4_ret != WGLR_OK)
               {
                   WGL_MEM_FREE(ps_arab_str);
                   return i4_ret;
               }
               /*reset reorder attr after used for current string content*/
               pt_ctx->t_arab_reorder_attr.b_find_init_level = TRUE;
               pt_ctx->t_arab_reorder_attr.ui1_embedding_level = 0;
               
               ui4_eff_len = _lbcntdef_tool_get_first_line_len (WGL_TEXT_DIR_RIGHT_TO_LEFT, ps_arab_str);
               ui2_len_text = (ui2_arab_str_len > (UINT16)ui4_eff_len) ? (UINT16)ui4_eff_len : ui2_arab_str_len;
               
               x_uc_w2s_strncpy(pt_ctx->w2s_text_ttl, ps_arab_str+ui2_arab_str_len-ui2_len_text, ui2_len_text);
               WGL_MEM_FREE(ps_arab_str);
            }
            else
            {
            /* get length of the first line */
            ui4_eff_len = _lbcntdef_tool_get_first_line_len (pt_ctx->ui1_txt_dir, w2s_text);
            ui2_len_text = (ui2_len_text > (UINT16)ui4_eff_len) ? (UINT16)ui4_eff_len : ui2_len_text;
            
            if (pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT)
            {
                x_uc_w2s_strncpy(pt_ctx->w2s_text_ttl, w2s_text, ui2_len_text);
            }
            else
            {
                x_uc_w2s_strncpy(pt_ctx->w2s_text_ttl, w2s_text + ui4_str_len - ui2_len_text, ui2_len_text);
       
                }
            }
            i4_ret = x_fe_get_string_size(pt_ctx->h_font, pt_ctx->w2s_text_ttl, (INT32)ui2_len_text, &i4_w, &i4_h);
            if(!( i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
            {
                pt_ctx->ui2_len_ttl = 0;
                INFORM_ERR(i4_ret);
                return i4_ret;
            }
        }
        pt_ctx->ui2_len_ttl = ui2_len_text;

        _lbcntdef_update_title_text_position(pt_ctx, i4_w, i4_h);
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_title_info
 *
 * Description: This API sets the title info.
 *
 * Inputs:  pt_ctx       Contains the instance context.
 *          pt_ttl_info  Contains the new title info. If pt_ttl_info is NULL,
 *                       title info would be cleared as default.
 *
 * Outputs: pt_ctx       Contains the changed instance context.
 *
 * Returns: WGLR_OK      Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_title_info(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_LB_TITLE_INFO_T         *pt_ttl_info)
{
    if(pt_ttl_info==NULL)
    {
        _lbcntdef_set_default_title_info(pt_ctx);
    }
    else
    {
        pt_ctx->ui1_align_ttl = pt_ttl_info->ui1_align;
        WGL_RECT_COPY(&pt_ctx->t_rc_lb_ttl_bound, &pt_ttl_info->t_rect);
        if(pt_ctx->ui2_len_ttl > 0)
        {
            _lbcntdef_update_title_text_position(pt_ctx, RECT_W(&pt_ctx->t_rc_lb_ttl_text), RECT_H(&pt_ctx->t_rc_lb_ttl_text));
        }
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_default_title_info
 *
 * Description: This API sets clears the title info as default.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_set_default_title_info(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx)
{
    pt_ctx->ui1_align_ttl = WGL_AS_CENTER_TOP;
    WGL_RECT_COPY(&pt_ctx->t_rc_lb_ttl_bound, &pt_ctx->t_rc_lb_cnt);
    if(pt_ctx->ui2_len_ttl > 0)
    {
        _lbcntdef_update_title_text_position(pt_ctx, RECT_W(&pt_ctx->t_rc_lb_ttl_text), RECT_H(&pt_ctx->t_rc_lb_ttl_text));
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_update_title_text_position
 *
 * Description: This API updates the title text position according to 
 *              new size of the text.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          i4_w_text  Specifies the new width of the text.
 *          i4_h_text  Specifies the new height of the text.
 *
 * Outputs: pt_ctx     Contains the changed instance context.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_update_title_text_position(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      INT32                       i4_w_text,
    IN      INT32                       i4_h_text)
{
    INT32   i4_x, i4_y;
    
    _lbcntdef_cal_obj_pos( 
        pt_ctx->ui1_txt_dir,
        TRUE,
        pt_ctx->b_opp_txt_align,
        pt_ctx->ui1_align_ttl, 
        XY_SEL_X | XY_SEL_Y,
        i4_w_text, 
        i4_h_text, 
        RECT_W(&pt_ctx->t_rc_lb_ttl_bound),
        RECT_H(&pt_ctx->t_rc_lb_ttl_bound),
        &i4_x,
        &i4_y);
    i4_x += pt_ctx->t_rc_lb_ttl_bound.i4_left;
    i4_y += pt_ctx->t_rc_lb_ttl_bound.i4_top;
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_lb_ttl_text, i4_x, i4_y, i4_w_text, i4_h_text);
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_elem
 *
 * Description: This API informs the content that a new element is added/created.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *          pt_elem  Contains the context of the new element.
 *          b_add_create  TRUE:add elem; FALSE:create element
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK  Routine successful.
 *          < 0      Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_add_elem(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      BOOL                        b_add_create)
{
    UINT8               ui1_c;
    INT32               i4_ret;
    LBCNTDEF_ELEM_T     *pt_cnt_elem = NULL;
    
    CHECK_NULL(pt_ctx, WGLR_INV_ARG);
    if(pt_elem)
    {
        pt_cnt_elem = (LBCNTDEF_ELEM_T *)pt_elem->pv_cnt_elem;
        if(pt_cnt_elem == NULL)
        {
            return WGLR_INTERNAL_ERROR;
        }

        pt_cnt_elem->at_items = (LBCNTDEF_ITEM_T *)(((BYTE *)(pt_elem->pv_cnt_elem)) + WGL_ALIGN_4(sizeof(LBCNTDEF_ELEM_T)));
        /*Init this Content element*/
        for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
        {
            pt_cnt_elem->at_items[ui1_c].pt_fmt = NULL;
            
            switch(pt_ctx->at_cols[ui1_c].e_col_type)
            { /*TODO: init the cached information for the item here*/
                case LB_COL_TYPE_TEXT:
                    break;
                case LB_COL_TYPE_IMG:
                    break;
                case LB_COL_TYPE_IMG_SUIT:
                    break;
            }
        }
    }

    if(b_add_create == TRUE)
    {  /*add element*/
        _LBCNTDEF_INC_ELEMS_NUM(pt_ctx);

        CATCH_FAIL(wgl_vp_add_elem(&pt_ctx->t_vp));
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_del_elem
 *
 * Description: This API informs the content that an element is removed.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *          pt_elem  Contains the context of the element.
 *          b_del_free TRUE:delete element; FALSE:free the element
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK  Routine successful.
 *          < 0      Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_del_elem(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      LB_ELEM_T                   *pt_elem,
    IN      BOOL                        b_del_free)
{
    LBCNTDEF_ELEM_T     *pt_cnt_elem = NULL;
    UINT8               ui1_c;
    INT32               i4_ret;

    CHECK_NULL(pt_ctx, WGLR_INV_ARG);

    if(pt_elem)
    {
        pt_cnt_elem = (LBCNTDEF_ELEM_T *)pt_elem->pv_cnt_elem;
        if(pt_cnt_elem == NULL)
        {
            return WGLR_INTERNAL_ERROR;
        }

        for(ui1_c=0; ui1_c<pt_ctx->ui1_col_num; ui1_c++)
        {
            if(pt_cnt_elem->at_items[ui1_c].pt_fmt)
            {
                i4_ret = wgl_npm_free (&pt_ctx->t_npm_itm_fmt, 
                                       (VOID *) pt_cnt_elem->at_items[ui1_c].pt_fmt);
                if (i4_ret != WGLR_OK)
                {
                    INFORM_ERR (i4_ret);
                }
                
                pt_cnt_elem->at_items[ui1_c].pt_fmt = NULL;
            }
        }
        _LBCNTDEF_DEC_ELEM_CACHE_NUM(pt_ctx);
    }

    if(b_del_free == TRUE)
    {
        _LBCNTDEF_DEC_ELEMS_NUM(pt_ctx);

        CATCH_FAIL(wgl_vp_del_elem(&pt_ctx->t_vp));
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_lb_screen_rect_or_bdr_inset
 *
 * Description: This API updates the listbox rect (in screen coordinates) 
 *              or the border insets.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          pt_rc_scrn_lb  Contains the listbox rect in screen coordinates.
 *          pt_inset_bdr   Contains the border insets. Ignore it if NULL.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_lb_screen_rect_or_bdr_inset(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      GL_RECT_T                   *pt_rc_scrn_lb, 
    IN      WGL_INSET_T                 *pt_inset_bdr)
{
    INT32           i4_ret;
    
    CHECK_VALID(_RECT_IS_VALID(*pt_rc_scrn_lb), WGLR_INV_ARG );

    if (pt_inset_bdr)
    {
        _INSET_COPY(&pt_ctx->t_inset_bdr, pt_inset_bdr);
    }
    
    /* check if the size of content could be changed */
    if (pt_inset_bdr                                           || 
        RECT_W(pt_rc_scrn_lb) != RECT_W(&pt_ctx->t_rc_scrn_lb) ||
        RECT_H(pt_rc_scrn_lb) != RECT_H(&pt_ctx->t_rc_scrn_lb) )
    {
        GL_RECT_T       t_rc_cnt;

        SET_RECT_BY_SIZE(&t_rc_cnt, 0, 0, RECT_W(pt_rc_scrn_lb), RECT_H(pt_rc_scrn_lb));
        
        _RECT_INSET(&t_rc_cnt, &pt_ctx->t_inset_bdr);
        
        if(!_RECT_IS_VALID(t_rc_cnt))
        {
            WGL_RECT_EMPTY(&t_rc_cnt);
        }
    
        WGL_RECT_COPY(&pt_ctx->t_rc_lb_cnt, &t_rc_cnt);
    }

    /* update screen rect if necessary */
    if (!WGL_RECT_IS_EQUAL (pt_rc_scrn_lb, &pt_ctx->t_rc_scrn_lb))    
    {
        WGL_RECT_COPY(&pt_ctx->t_rc_scrn_lb, pt_rc_scrn_lb);
    
        CATCH_FAIL(_lbcntdef_update_vp_rect(pt_ctx));
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_update_vp_rect
 *
 * Description: This API updates the viewport rect.
 *
 * Inputs:  pt_ctx   Contains the instance context.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK  Routine successful.
 *          < 0      Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_update_vp_rect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx)
{
    INT32   i4_ret;
    BOOL    b_eff_rc_scrn_vp;
    UINT16  ui2_scrn_vp = 0;

    /*setup pt_ctx->t_rc_scrn_vp & pt_ctx->t_rc_lb_vp */
    b_eff_rc_scrn_vp = FALSE;
    wgl_vp_get_vp_size(&pt_ctx->t_vp, &ui2_scrn_vp);

    WGL_RECT_COPY(&pt_ctx->t_rc_scrn_vp, &pt_ctx->t_rc_scrn_lb);
    _RECT_INSET(&pt_ctx->t_rc_scrn_vp, &pt_ctx->t_inset_bdr);
    
    if(RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
    {
        _RECT_INSET(&pt_ctx->t_rc_scrn_vp, &pt_ctx->t_inset_cnt);

        if(RECT_IS_EFFECTIVE(pt_ctx->t_rc_scrn_vp))
        {
            WGL_RECT_COPY(&pt_ctx->t_rc_lb_vp, &pt_ctx->t_rc_scrn_vp);
            MOVE_RECT(&pt_ctx->t_rc_lb_vp, -pt_ctx->t_rc_scrn_lb.i4_left, -pt_ctx->t_rc_scrn_lb.i4_top);
            
            b_eff_rc_scrn_vp = TRUE;
        }
    }
    
    if(!b_eff_rc_scrn_vp)
    {
        WGL_RECT_EMPTY(&pt_ctx->t_rc_scrn_vp);
        WGL_RECT_EMPTY(&pt_ctx->t_rc_lb_vp);
    }

    if( ui2_scrn_vp != (UINT16)_RECT_GET_H(pt_ctx->t_rc_scrn_vp))
    {
        CATCH_FAIL(wgl_vp_set_vp_size(&pt_ctx->t_vp, (UINT16)_RECT_GET_H(pt_ctx->t_rc_scrn_vp)));
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_font
 *
 * Description: This API updates the font.
 *
 * Inputs:  pt_ctx            Contains the instance context.
 *          pt_font_info_src  Contains the font info. 
 *                            If it's NULL, the system default font would be used.
 *
 * Outputs: pt_ctx            Contains the changed context of lb.
 *          ph_font           Contains the font handle
 *
 * Returns: WGLR_OK           Routine successful.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
/* pt_font_info_src can be NULL */
static INT32 _lbcntdef_set_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_T             *pt_font_info_src, 
    OUT     HFONT_T                     *ph_font)
{
    INT32               i4_ret;
    WGL_FONT_INFO_T     *pt_font_info = pt_font_info_src;
    WGL_FONT_INFO_T     t_font_info_tmp;

    if(pt_font_info == NULL)
    {
       CATCH_FAIL(x_wgl_get_system_font(&t_font_info_tmp));
       t_font_info_tmp.i2_width = 0;
       pt_font_info = &t_font_info_tmp;
    }

    /*update the extend font info*/
    pt_ctx->t_font_info_ex.e_type = WGL_FONT_SET_BASIC;
    x_memcpy(&pt_ctx->t_font_info_ex.u_font_data.t_basic.t_focused, pt_font_info, sizeof(WGL_FONT_INFO_T));
    x_memcpy(&pt_ctx->t_font_info_ex.u_font_data.t_basic.t_unfocused, pt_font_info, sizeof(WGL_FONT_INFO_T));
    
    /*update font*/
    CATCH_FAIL(_lbcntdef_update_font(pt_ctx, &pt_ctx->t_font_info_ex, ph_font));

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_font_ex
 *
 * Description: This API sets the extend font info.
 *
 * Inputs:  pt_ctx               Contains the instance context.
 *          pt_ex_font_info_src  Contains the extend font info. 
 *                               If it's NULL, the system default font would be used.
 *
 * Outputs: pt_ctx               Contains the changed context of lb.
 *          ph_font              Contains the font handle
 *
 * Returns: WGLR_OK              Routine successful.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
/* pt_font_info_src can be NULL */
static INT32 _lbcntdef_set_font_ex(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_EX_T          *pt_ex_font_info_src, 
    OUT     HFONT_T                     *ph_font)
{
    INT32               i4_ret;
    WGL_FONT_INFO_EX_T  t_ex_font_info;
    WGL_FONT_INFO_T     t_font_info_tmp;

    if(pt_ex_font_info_src == NULL)
    {
        CATCH_FAIL(x_wgl_get_system_font(&t_font_info_tmp));
        t_ex_font_info.e_type = WGL_FONT_SET_BASIC;
        x_memcpy(&t_ex_font_info.u_font_data.t_basic.t_focused, &t_font_info_tmp, sizeof(WGL_FONT_INFO_T));
        x_memcpy(&t_ex_font_info.u_font_data.t_basic.t_unfocused, &t_font_info_tmp, sizeof(WGL_FONT_INFO_T));
    }
    else if (pt_ex_font_info_src->e_type != WGL_FONT_SET_BASIC)
    {
        i4_ret = WGLR_INV_ARG;
        INFORM_ERR (i4_ret);
        return i4_ret;
    }
    else
    {
        x_memcpy(&t_ex_font_info, pt_ex_font_info_src, sizeof(WGL_FONT_INFO_EX_T));
    }

    /*update the extend font info*/
    x_memcpy(&pt_ctx->t_font_info_ex, &t_ex_font_info, sizeof(WGL_FONT_INFO_EX_T));

    /*update font*/
    CATCH_FAIL(_lbcntdef_update_font(pt_ctx, &t_ex_font_info, ph_font));
  
    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_update_font
 *
 * Description: This API updates the listbox general font.
 *
 * Inputs:  pt_ctx               Contains the instance context.
 *          pt_ex_font_info      Contains the extend font info. 
 *                               If it's NULL, the system default font would be used.
 *
 * Outputs: pt_ctx               Contains the changed context of lb.
 *          ph_font              Contains the font handle
 *
 * Returns: WGLR_OK              Routine successful.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/

static INT32 _lbcntdef_update_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_EX_T          *pt_ex_font_info, 
    OUT     HFONT_T                     *ph_font)
{
    INT32               i4_ret;
    /*check lb's state*/
    if (HAS_FLAG(pt_ctx->ui1_state, LBST_FOCUSED))
    {
        CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_ex_font_info->u_font_data.t_basic.t_focused, ph_font));
    }
    else
    {
        CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_ex_font_info->u_font_data.t_basic.t_unfocused, ph_font));
    }
    
    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_font_ex
 *
 * Description: This API get the listbox extend general font info.
 *
 * Inputs:  pt_ctx               Contains the instance context.
 *
 * Outputs: pt_ex_font_info      Contains the extend font info.
 *
 * Returns: WGLR_OK              Routine successful.
 ----------------------------------------------------------------------------*/

static INT32 _lbcntdef_get_font_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    OUT     WGL_FONT_INFO_EX_T          *pt_ex_font_info)
{
    x_memcpy(pt_ex_font_info, &pt_ctx->t_font_info_ex, sizeof(WGL_FONT_INFO_EX_T));
    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_col_font
 *
 * Description: This API sets the column font info.
 *
 * Inputs:  pt_ctx               Contains the instance context.
 *          pt_font_info_src     Contains the font info. 
 *                               If it's NULL, the general extend font would be used.
 *          ui1_c                Specifies the column index.
 *
 * Outputs: pt_ctx               Contains the changed context of lb.
 *          ph_font              Contains the font handle
 *
 * Returns: WGLR_OK              Routine successful.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
/* pt_font_info_src can be NULL */
static INT32 _lbcntdef_set_col_font(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_FONT_INFO_T             *pt_font_info_src, 
    OUT     HFONT_T                     *ph_font,
    IN      UINT8                       ui1_c)
{
    INT32                 i4_ret;
    WGL_FONT_INFO_EX_T    t_ex_font_info;
    WGL_LB_FONT_INFO_EX_T t_ex_lb_font_info;

    if(pt_font_info_src == NULL)
    {
        CATCH_FAIL(_lbcntdef_get_font_ex(pt_ctx, &t_ex_font_info));
        t_ex_lb_font_info.t_highlight = t_ex_font_info.u_font_data.t_basic.t_focused;
        t_ex_lb_font_info.t_highlight_unfocused = t_ex_font_info.u_font_data.t_basic.t_focused;
        t_ex_lb_font_info.t_normal = t_ex_font_info.u_font_data.t_basic.t_unfocused;
        t_ex_lb_font_info.t_selected = t_ex_font_info.u_font_data.t_basic.t_unfocused;
    }
    else
    {
        x_memcpy(&t_ex_lb_font_info.t_highlight, pt_font_info_src, sizeof(WGL_FONT_INFO_T));
        x_memcpy(&t_ex_lb_font_info.t_highlight_unfocused, pt_font_info_src, sizeof(WGL_FONT_INFO_T));
        x_memcpy(&t_ex_lb_font_info.t_normal, pt_font_info_src, sizeof(WGL_FONT_INFO_T));
        x_memcpy(&t_ex_lb_font_info.t_selected, pt_font_info_src, sizeof(WGL_FONT_INFO_T));
    }

    /*update the extend font info*/
    pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex = t_ex_lb_font_info;
    
    /*update column font simply, the exact font will be set when repaint*/
    CATCH_FAIL(wgl_frlm_set_handle_by_info(&pt_ctx->t_frlm, &t_ex_lb_font_info.t_normal, ph_font));

    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_col_font_ex
 *
 * Description: This API sets the column font info.
 *
 * Inputs:  pt_ctx                  Contains the instance context.
 *          pt_ex_lb_font_info_src  Contains the extend font info. 
 *                                  If it's NULL, the general extend font would be used.
 *          ui1_c                   Specifies the column index.
 *
 * Outputs: pt_ctx                  Contains the changed context of lb.
 *          ph_font                 Contains the font handle
 * 
 * Returns: WGLR_OK                 Routine successful.
 *          < 0                     Some other errors.
 ----------------------------------------------------------------------------*/
/* pt_ex_lb_font_info_src can be NULL */
static INT32 _lbcntdef_set_col_font_ex(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx, 
    IN      WGL_LB_FONT_INFO_EX_T       *pt_ex_lb_font_info_src, 
    OUT     HFONT_T                     *ph_font,
    IN      UINT8                       ui1_c)
{
    INT32                 i4_ret;
    WGL_FONT_INFO_EX_T    t_ex_font_info;
    WGL_LB_FONT_INFO_EX_T t_ex_lb_font_info;

    if(pt_ex_lb_font_info_src == NULL)
    {
        CATCH_FAIL(_lbcntdef_get_font_ex(pt_ctx, &t_ex_font_info));
        t_ex_lb_font_info.t_highlight = t_ex_font_info.u_font_data.t_basic.t_focused;
        t_ex_lb_font_info.t_highlight_unfocused = t_ex_font_info.u_font_data.t_basic.t_focused;
        t_ex_lb_font_info.t_normal = t_ex_font_info.u_font_data.t_basic.t_unfocused;
        t_ex_lb_font_info.t_selected = t_ex_font_info.u_font_data.t_basic.t_unfocused;
    }
    else
    {
        x_memcpy(&t_ex_lb_font_info, pt_ex_lb_font_info_src, sizeof(WGL_LB_FONT_INFO_EX_T));
    }

    /*update the extend font info*/
    pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex = t_ex_lb_font_info;
    
    /*update column font simply, the exact font will be set when repaint*/
    CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &t_ex_lb_font_info.t_normal, ph_font));

    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_font_ex
 *
 * Description: This API sets the extend font info.
 *
 * Inputs:  pt_ctx               Contains the instance context.
 *          ui1_c                Specifies the column index.
 *
 * Outputs: pt_ex_lb_font_info   Contains the extend font info. 
 *
 * Returns: WGLR_OK              Routine successful.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/

static INT32 _lbcntdef_get_col_font_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    OUT     WGL_LB_FONT_INFO_EX_T       *pt_ex_lb_font_info, 
    IN      UINT8                       ui1_c)
{
    x_memcpy(pt_ex_lb_font_info, &pt_ctx->at_cnt_cols[ui1_c].t_col_font_info_ex, sizeof(WGL_LB_FONT_INFO_EX_T));

    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_lb_bk
 *
 * Description: This API draws the listbox background.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context in listbox's coordinates.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_rc_lb_upt     Contains the invalid update rect in listbox's coordinates.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_lb_bk(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt)
{
    INT32        i4_ret;
    BOOL         b_fill_bk_in_color_mode;
    
    b_fill_bk_in_color_mode = FALSE;
    if(HAS_FLAG(pt_ctx->ui4_style, WGL_STL_GL_NO_BK))
    {
    }
    else if(HAS_FLAG(pt_ctx->ui4_style, WGL_STL_GL_NO_IMG_UI))
    {
        /* GRAPHIC UI */
        b_fill_bk_in_color_mode = TRUE;
    }
    else
    {
        /* IMG_UI */
        WGL_HIMG_TPL_T  h_img_bk;
        _lbcntdef_select_lb_img(ui2_state_paint, &pt_ctx->t_imgs_bk, &h_img_bk);
        if(h_img_bk != NULL_HANDLE)
        {
            INFORM_ERR(wgl_img_tpl_paint(
                h_img_bk, 
                &pt_ctx->t_rc_lb_cnt, 
                pt_rc_lb_upt, 
                h_g, 
                &pt_ctx->t_cvsst,
                HAS_FLAG(pt_ctx->ui4_style, WGL_STL_GL_BDR_FILL_CNT_BK) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT));
        }
        else if(!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_GL_BDR_FILL_CNT_BK))
        {
            b_fill_bk_in_color_mode = TRUE;
        }
    }
    
    if(b_fill_bk_in_color_mode)
    {
        /* fill bk by color */
        GL_COLOR_T   *pt_clr_bk;
        _lbcntdef_select_lb_color(ui2_state_paint, &pt_ctx->t_clrs_bk, &pt_clr_bk);

        if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_bk))
        {
            return WGLR_OK;
        }
        
        i4_ret = x_gl_set_color(h_g, pt_clr_bk);
        if(!WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR( x_gl_fill_rect(h_g, &pt_ctx->t_rc_lb_cnt) );
        }
        else
        {
            INFORM_ERR(i4_ret);
        }
    }
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_lb_vp_bk
 *
 * Description: This API draws the viewport background.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_rc_lb_upt     Contains the invalid update rect in listbox's coordinates.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_lb_vp_bk(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt)
{
    WGL_HIMG_TPL_T      h_img;
    GL_COLOR_T          *pt_clr;
    GL_RECT_T           t_rc_lb_vp_upt;
    INT32               i4_ret;
    
    INTERSECT_RECT(&t_rc_lb_vp_upt, &pt_ctx->t_rc_lb_vp, pt_rc_lb_upt);
    if(!RECT_IS_EFFECTIVE(t_rc_lb_vp_upt))
    {
        return WGLR_OK;
    }
    
    _lbcntdef_select_lb_img(ui2_state_paint, &pt_ctx->t_imgs_bk_vp, &h_img);
    
    if(h_img != NULL_HANDLE)
    {
        INFORM_ERR(wgl_img_tpl_paint(
            h_img,
            &pt_ctx->t_rc_lb_vp,
            pt_rc_lb_upt,
            h_g, 
            &pt_ctx->t_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else
    {
        _lbcntdef_select_lb_color(ui2_state_paint, &pt_ctx->t_clrs_bk_vp, &pt_clr);
        if(!WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr))
        {
            i4_ret = x_gl_set_color(h_g, pt_clr);
            if(!WGL_IS_FAIL(i4_ret))
            {
                INFORM_ERR(x_gl_fill_rect(h_g, &pt_ctx->t_rc_lb_vp));
            }
            else
            {
                INFORM_ERR(i4_ret);
            }
        }
    }
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_scrl_bar
 *
 * Description: This API draws the scroll bar.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_rc_update     Contains the update rect.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_scrl_bar(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_lb_upt )
{
    INT32               i4_ret;
    WGL_HIMG_TPL_T      h_img_body;
    GL_COLOR_T          *pt_clr_body = NULL;
    BOOL                b_disabled;
    GL_RECT_T           t_rc_lb_body_upt;
    UINT16              ui2_first, ui2_last;
    INT32               i4_vp_top_first; 
    UINT32              ui4_ofs_thumb, ui4_sz_thumb;

    /* check if the scroll bar is turned on */
    if(!RECT_IS_EFFECTIVE(pt_ctx->t_attr_sb.t_rc_body))
    {
        return WGLR_OK;
    }
    
    INTERSECT_RECT(&t_rc_lb_body_upt, &pt_ctx->t_attr_sb.t_rc_body, pt_rc_lb_upt);
    if(!RECT_IS_EFFECTIVE(t_rc_lb_body_upt))
    {
        return WGLR_OK;
    }

    /* get vp parameters */
    CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_first, &ui2_last, &i4_vp_top_first));
    
    if (!HAS_FLAG (pt_ctx->t_attr_sb.ui4_style, WGL_LB_SB_STL_MANUAL_MODE))
    {
        /* update vp parameters */
        CATCH_FAIL(wgl_lb_sb_update_vp(&pt_ctx->t_ctx_sb, (UINT32) RECT_H(&pt_ctx->t_rc_scrn_vp), pt_ctx->ui2_elem_num, ui2_first, i4_vp_top_first));
    }
    else
    {
        /* do nothing in MANUAL mode */
    }
    
    /* get thumb's size & offset */
    CATCH_FAIL(wgl_lb_sb_get_thumb(&pt_ctx->t_ctx_sb, &b_disabled, &ui4_ofs_thumb, &ui4_sz_thumb));

    if(b_disabled)
    {
        if(HAS_FLAG(pt_ctx->t_attr_sb.ui4_style, WGL_LB_SB_STL_NO_SCRL_KEEP_BODY))
        {
            /* prepare the body's image or color in disabled state */
            h_img_body = pt_ctx->t_attr_sb.t_imgs_body.t_disable;
            /* select color if image is NULL_HANDLE */
            pt_clr_body = (h_img_body == NULL_HANDLE) ? &pt_ctx->t_attr_sb.t_clrs_body.t_disable : NULL;
        }
        else
        {
            /* just hide the scroll bar */
            return WGLR_OK;
        }
    }
    else
    {
        _lbcntdef_select_lb_img(ui2_state_paint, &pt_ctx->t_attr_sb.t_imgs_body, &h_img_body);
        if(h_img_body == NULL_HANDLE)
        {
            /* select color if image is NULL_HANDLE */
            _lbcntdef_select_lb_color(ui2_state_paint, &pt_ctx->t_attr_sb.t_clrs_body, &pt_clr_body);
        }
    }
    
    CATCH_FAIL(x_gl_set_clip(h_g, &pt_ctx->t_rc_scrn_lb));
    CATCH_FAIL(x_gl_translate(h_g, pt_ctx->t_rc_scrn_lb.i4_left, pt_ctx->t_rc_scrn_lb.i4_top));
    
    /* draw the body */
    if(h_img_body != NULL_HANDLE)
    {
        /* draw the body by image */
        INFORM_ERR(wgl_img_tpl_paint(
            h_img_body, 
            &pt_ctx->t_attr_sb.t_rc_body, 
            &t_rc_lb_body_upt,
            h_g,
            &pt_ctx->t_cvsst,
            WGL_TPL_PAINT_COMPOSE));
    }
    else
    {
        /* draw the body by color */
        if (!wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_body))
        {
            INFORM_ERR(x_gl_set_color(h_g, pt_clr_body));
            INFORM_ERR(x_gl_fill_rect(h_g, &pt_ctx->t_attr_sb.t_rc_body));
        }
    }
    
    if(!b_disabled)
    {
        GL_RECT_T       t_rc_lb_track, t_rc_lb_thumb;
        WGL_HIMG_TPL_T  h_img_thumb;
        
        _RECT_INSET_TO(t_rc_lb_track, pt_ctx->t_attr_sb.t_rc_body, pt_ctx->t_attr_sb.t_inset_track);
        if(!RECT_IS_EFFECTIVE(t_rc_lb_track))
        {
            return WGLR_OK;
        }
        
        t_rc_lb_thumb.i4_left   = t_rc_lb_track.i4_left;
        t_rc_lb_thumb.i4_top    = t_rc_lb_track.i4_top + ((INT32)ui4_ofs_thumb);
        t_rc_lb_thumb.i4_right  = t_rc_lb_track.i4_right;
        t_rc_lb_thumb.i4_bottom = t_rc_lb_thumb.i4_top + ((INT32)ui4_sz_thumb);
        if(!RECT_IS_EFFECTIVE(t_rc_lb_thumb))
        {
            return WGLR_OK;
        }
        
        /* draw thumb - select image */
        _lbcntdef_select_lb_img(ui2_state_paint, &pt_ctx->t_attr_sb.t_imgs_thumb, &h_img_thumb);
        if(h_img_thumb != NULL_HANDLE)
        {
            /* draw the thumb by image */
            INFORM_ERR(wgl_img_tpl_paint(
                h_img_thumb, 
                &t_rc_lb_thumb, 
                &t_rc_lb_body_upt,
                h_g,
                &pt_ctx->t_cvsst,
                WGL_TPL_PAINT_COMPOSE));
        }
        else
        {
            /* draw thumb - select color if image is NULL_HANDLE */
            GL_COLOR_T      *pt_clr_thumb;
            _lbcntdef_select_lb_color(ui2_state_paint, &pt_ctx->t_attr_sb.t_clrs_thumb, &pt_clr_thumb);
            /* draw the body by color */
            if (!wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_thumb))
            {
                INFORM_ERR(x_gl_set_color(h_g, pt_clr_thumb));
                INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_lb_thumb));
            }
        }
    }
        
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_title
 *
 * Description: This API draws the title.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context.
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_rc_update     Contains the update rect.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_title(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      GL_RECT_T                   *pt_rc_update  )
{
    INT32           i4_ret;
    GL_COLOR_T      *pt_clr_fg, *pt_clr_bk;
    BOOL            b_draw_str;

    if(pt_ctx->ui2_len_ttl==0 || pt_ctx->w2s_text_ttl==NULL)
    {
        return WGLR_OK;
    }
    
    if(pt_rc_update)
    {
        BOOL    b_is_intersected = FALSE;
        INFORM_ERR(x_wgl_rect_is_cross(&b_is_intersected, &pt_ctx->t_rc_lb_ttl_text, pt_rc_update));
        if(!b_is_intersected)
        {
            return WGLR_OK;
        }
    }
    
    _lbcntdef_select_lb_color(  
        ui2_state_paint,
        &pt_ctx->t_clrs_ttl_fg,
        &pt_clr_fg );
    _lbcntdef_select_lb_color(  
        ui2_state_paint,
        &pt_ctx->t_clrs_ttl_bk,
        &pt_clr_bk );
        
    b_draw_str = TRUE;
    if(wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_fg))
    {
        if(!WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_bk))
        {
            /* case 1 : bk color is not transparent */
            if(wgl_cvsst_check_palette_mode(&pt_ctx->t_cvsst))
            {
                /* case 1.1 : in palette mode, text is rendered by bitblt */
                /* make the text color same as bk color, look like text is transparent */
                pt_clr_fg = pt_clr_bk;
            }
            else
            {
                /* case 1.2 : in direct color mode, text is rendered by composition */
                /* do nothing, keep the text color transparent */
            }
        }
        else
        {
            /* case 2 : bk color is transparent */
            /* both text & bk color are transparent, don't need draw string at all */
            b_draw_str = FALSE;
        }
    }
    if ((pt_ctx->t_rc_lb_ttl_text.i4_left + pt_ctx->t_rc_scrn_lb.i4_left) < 0)
    {
        INT32 i4_w_temp;
        i4_w_temp = WGL_RECT_GET_WIDTH(&pt_ctx->t_rc_lb_ttl_text);
        pt_ctx->t_rc_lb_ttl_text.i4_left = 0 - pt_ctx->t_rc_scrn_lb.i4_left;
        pt_ctx->t_rc_lb_ttl_text.i4_right = pt_ctx->t_rc_lb_ttl_text.i4_left + i4_w_temp;
    }

    if ((pt_ctx->t_rc_lb_ttl_text.i4_top + pt_ctx->t_rc_scrn_lb.i4_top) < 0)
    {
        INT32 i4_h_temp;
        i4_h_temp = WGL_RECT_GET_HEIGHT(&pt_ctx->t_rc_lb_ttl_text);
        pt_ctx->t_rc_lb_ttl_text.i4_top = 0 - pt_ctx->t_rc_scrn_lb.i4_top;
        pt_ctx->t_rc_lb_ttl_text.i4_bottom = pt_ctx->t_rc_lb_ttl_text.i4_top + i4_h_temp;
    }
      
    if( b_draw_str )
    {
        INFORM_ERR(x_gl_draw_ustring_ex(
            h_g,
            pt_ctx->t_rc_lb_ttl_text.i4_left,
            pt_ctx->t_rc_lb_ttl_text.i4_top,
            pt_ctx->t_rc_lb_ttl_text.i4_top,
            pt_ctx->w2s_text_ttl,
            pt_ctx->ui2_len_ttl,
            NULL,
            0,
            pt_ctx->h_font,
            pt_clr_fg,
            pt_clr_bk,
            NULL,
            FALSE,
            0,
            0,
            (WGL_LB_IS_LAYER_BK_TRANSPARENT(pt_clr_bk) || wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_bk)) ? (UINT16)0 : (UINT16)RECT_H(&pt_ctx->t_rc_lb_ttl_text),
            0));
    }
    
    return WGLR_OK;            
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_scroll_images
 *
 * Description: This API draws the scroll indicator images.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *          h_g              Contains the handle of the graphic context.
 *          pt_rc_update     Contains the update rect.
 *
 * Outputs: h_g              Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK          Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_scroll_images(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      GL_RECT_T                   *pt_rc_update )
{
    INT32                               i4_ret;
    UINT16                              ui2_idx_hlt;
    BOOL                                b_up_on, b_down_on;
    
    /* find highlight element index */
    ui2_idx_hlt = _lbcntdef_get_hlt_idx(pt_ctx);
    
    /* check scroll up/down image */
    if(pt_ctx->ui2_elem_num <= 1 || ui2_idx_hlt==WGL_LB_NULL_INDEX)
    {
        b_up_on = b_down_on = FALSE;
    }
    else
    {
        b_up_on = (ui2_idx_hlt == 0 && HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) ? FALSE : TRUE;
        b_down_on = (ui2_idx_hlt == (pt_ctx->ui2_elem_num - (UINT16)1) && HAS_FLAG(pt_ctx->ui4_style, WGL_STL_LB_NO_WRAP_OVER)) ? FALSE : TRUE;
    }
    
    /* draw scroll image */
    INFORM_ERR(_lbcntdef_draw_indicator_images(
        h_g,
        &pt_ctx->t_cvsst,
        &pt_ctx->t_scr_imgs,
        b_up_on,
        b_down_on,
        pt_rc_update));
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_draw_indicator_images
 *
 * Description: This API draws the scrolling indicator images.
 *
 * Inputs:  h_g           Contains the handle of the graphic context.
 *          pt_cvsst      Contains the status of the current hosted canvas.
 *          pt_scr_imgs   Contains scrolling indicator image info.
 *          b_is_up_on    Specifies if the up indicator image is on or off.
 *          b_is_down_on  Specifies if the down indicator image is on or off.
 *          pt_rc_update  Contains the update rect. If pt_rc_update is NULL, 
 *                        the whole listbox should be updated.
 *
 * Outputs: h_g           Contains the handle of the changed graphic context.
 *
 * Returns: WGLR_OK       Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_draw_indicator_images(
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs,
    IN      BOOL                        b_is_up_on,
    IN      BOOL                        b_is_down_on,
    IN      GL_RECT_T                   *pt_rc_update)
{
    WGL_HIMG_TPL_T      h_imgs[2];
    GL_POINT_T          *pt_locs[2];
    GL_RECT_T           t_rc_img;
    UINT32              ui4_img_w, ui4_img_h;
    INT32               i4_ret;
    UINT8               ui1_i;

    h_imgs[0] = b_is_up_on ? 
            pt_scr_imgs->h_img_scr_up_on : pt_scr_imgs->h_img_scr_up_off;
    
    h_imgs[1] = b_is_down_on ? 
            pt_scr_imgs->h_img_scr_down_on : pt_scr_imgs->h_img_scr_down_off;

    pt_locs[0] = &pt_scr_imgs->t_location_up;
    pt_locs[1] = &pt_scr_imgs->t_location_down;
    
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
                                
                if(pt_rc_update)
                {
                    BOOL    b_is_intersected;
                    INFORM_ERR(x_wgl_rect_is_cross(&b_is_intersected, &t_rc_img, pt_rc_update));
                    if(!b_is_intersected)
                    {
                        continue;
                    }
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

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_hlt_idx
 *
 * Description: This API gets the index of the highlight element.
 *
 * Inputs:  pt_ctx           Contains the instance context.
 *
 * Outputs: -
 *
 * Returns: WGL_LB_NULL_INDEX  If no element is highlighted.
 *          Otherwise, the index of the highlight element.
 ----------------------------------------------------------------------------*/
UINT16 _lbcntdef_get_hlt_idx(
    IN      LBCNTDEF_CTX_T              *pt_ctx)
{
    UINT16              ui2_idx = WGL_LB_NULL_INDEX;
    
    _LBCNTDEF_FIND_ELEM_ST_INF(pt_ctx, LBEST_HLT,0, 1,TRUE, &ui2_idx, NULL);
    
    return ui2_idx ;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_valid_image_handle
 *
 * Description: This API checks if an image handle is valid or not.
 *
 * Inputs:  h_img  Specifies the image handle.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_valid_image_handle(
    IN      WGL_HIMG_TPL_T              h_img)
{
    UINT32  ui4_tmp_w, ui4_tmp_h;
    if(WGLR_OK!=x_wgl_img_tpl_get_size(h_img, &ui4_tmp_w, &ui4_tmp_h))
    {
        return FALSE;
    }
    
    return TRUE;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_valid_img_set_std
 *
 * Description: This API checks if a standard image set is valid or not.
 *
 * Inputs:  pt_imgs  Specifies the standard image set.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image set is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_valid_img_set_std(
    IN      WGL_IMG_STD_SET_T           *pt_imgs)
{
    WGL_HIMG_TPL_T      ah_imgs[3];
    UINT16              ui2_i;
    
    ah_imgs[0] = pt_imgs->t_enable;
    ah_imgs[1] = pt_imgs->t_disable;
    ah_imgs[2] = pt_imgs->t_highlight;
    
    for(ui2_i=0; ui2_i<3; ui2_i++)
    {
        if( ah_imgs[ui2_i] != NULL_HANDLE &&
            !_lbcntdef_tool_check_valid_image_handle(ah_imgs[ui2_i]) )
        {
            return FALSE;
        }
    }

    return TRUE;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_valid_elem_image_set
 *
 * Description: This API checks if an element image set is valid or not.
 *
 * Inputs:  pt_imgs_elem  Specifies the element image set.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_valid_elem_image_set(
    IN      WGL_LB_IMG_ELEM_SET_T       *pt_imgs_elem)
{
    WGL_HIMG_TPL_T    ah_img[7];
    UINT16            ui2_i;

    ah_img[0] = pt_imgs_elem->h_disable;
    ah_img[1] = pt_imgs_elem->h_highlight;
    ah_img[2] = pt_imgs_elem->h_highlight_unfocus;
    ah_img[3] = pt_imgs_elem->h_normal;
    ah_img[4] = pt_imgs_elem->h_pushed;
    ah_img[5] = pt_imgs_elem->h_selected;
    ah_img[6] = pt_imgs_elem->h_selected_disable;

    for(ui2_i=0; ui2_i<7; ui2_i++)
    {
        if( ah_img[ui2_i]!=NULL_HANDLE &&
            !_lbcntdef_tool_check_valid_image_handle(ah_img[ui2_i]) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_valid_scroll_image_info
 *
 * Description: This API checks if a scrolling indicator image info object 
 *              is valid or not.
 *
 * Inputs:  pt_scr_imgs  Specifies the scrolling indicator image info.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the image is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_valid_scroll_image_info(
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_imgs)
{
    WGL_HIMG_TPL_T    ah_img[4];
    UINT16            ui2_i;
    
    ah_img[0] = pt_scr_imgs->h_img_scr_down_off;
    ah_img[1] = pt_scr_imgs->h_img_scr_down_on;
    ah_img[2] = pt_scr_imgs->h_img_scr_up_off;
    ah_img[3] = pt_scr_imgs->h_img_scr_up_on;
    
    for(ui2_i=0; ui2_i<4; ui2_i++)
    {
        if(ah_img[ui2_i]!=NULL_HANDLE  &&
           !_lbcntdef_tool_check_valid_image_handle(ah_img[ui2_i]))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_check_valid_hlt_elem_img_set
 *
 * Description: This API checks if a highlight element image set  
 *              is valid or not.
 *
 * Inputs:  pt_hlt_elem_img_set  Contains the WGL_LB_IMG_HLT_ELEM_SET_T info.
 *
 * Outputs: -
 *
 * Returns: TRUE   If the info is valid.
 *          FALSE  Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_tool_check_valid_hlt_elem_img_set(
    IN      WGL_LB_IMG_HLT_ELEM_SET_T   *pt_hlt_elem_img_set)
{
    WGL_HIMG_TPL_T  ah_imgs[6];
    UINT16          ui2_i;
    
    ah_imgs[0] = pt_hlt_elem_img_set->h_highlight;
    ah_imgs[1] = pt_hlt_elem_img_set->h_highlight_unfocus;
    ah_imgs[2] = pt_hlt_elem_img_set->h_selected_highlight;
    ah_imgs[3] = pt_hlt_elem_img_set->h_selected_highlight_unfocus;
    ah_imgs[4] = pt_hlt_elem_img_set->h_pushed;
    ah_imgs[5] = pt_hlt_elem_img_set->h_selected_pushed;
    
    for(ui2_i = 0; ui2_i < 6; ui2_i ++ )
    {
        if( ah_imgs[ui2_i] != NULL_HANDLE  &&
            !_lbcntdef_tool_check_valid_image_handle(ah_imgs[ui2_i]))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_get_max_text_len
 *
 * Description: This API gets the maximum text length.
 *
 * Inputs:  pat_cols     Contains the column info array.
 *          ui1_col_num  Specifies the column number.
 *
 * Outputs: -
 *
 * Returns: the maximum text length.
 ----------------------------------------------------------------------------*/
static UINT16 _lbcntdef_tool_get_max_text_len(
    IN      const LB_COL_T*             pat_cols,
    IN      UINT8                       ui1_col_num )
{
    UINT8       ui1_c;
    UINT16      ui2_max_len;
    
    ui2_max_len = 0;
    for ( ui1_c = 0; ui1_c < ui1_col_num ; ui1_c ++ )
    {
        if ( pat_cols[ui1_c].e_col_type == LB_COL_TYPE_TEXT &&
             pat_cols[ui1_c].ui2_max_text_len > ui2_max_len )
        {
            ui2_max_len = pat_cols[ui1_c].ui2_max_text_len;
        }
    }
    
    return ui2_max_len;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_update_region_by_diff_scrl_imgs
 *
 * Description: This API appends update regions according to original and new 
 *              scrolling indicator images.
 *
 * Inputs:  pt_pur_out            Contains the update regions.
 *          pt_scr_img_info_orig  Contains the original scrolling indicator images.
 *          pt_scr_img_info_new   Contains the new scrolling indicator images.
 *
 * Outputs: pt_pur_out       Contains the changed update regions.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_add_update_region_by_diff_scrl_imgs (
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info_orig,
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info_new)
{
    _lbcntdef_add_update_region_by_vp_indicators(pt_scr_img_info_orig, pt_pur_out);
    _lbcntdef_add_update_region_by_vp_indicators(pt_scr_img_info_new, pt_pur_out);
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_update_region_by_vp_indicators
 *
 * Description: This function appends update regions according to a scrolling indicator image info.
 *
 * Inputs:  pt_scr_img_info  Contains the scrolling indicator image info.
 *          pt_pur_out       Contains the update regions.
 *
 * Outputs: pt_pur_out       Contains the changed update regions.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_add_update_region_by_vp_indicators(
    IN      WGL_LB_SCROLL_IMG_INFO_T    *pt_scr_img_info,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out)
{
    INT32               i4_ret;
    UINT16              ui2_i, ui2_k;
    HANDLE_T            aah_img[2][2];
    GL_POINT_T          at_loc_img[2];
    GL_RECT_T           at_rc_tmp[2];
    BOOL                ab_is_rect_empty[2];
    GL_RECT_T           t_rc_union;
    UINT32              ui4_img_w, ui4_img_h;
    
    aah_img[0][0]      = pt_scr_img_info->h_img_scr_up_on;
    aah_img[0][1]      = pt_scr_img_info->h_img_scr_up_off;
    aah_img[1][0]      = pt_scr_img_info->h_img_scr_down_on;
    aah_img[1][1]      = pt_scr_img_info->h_img_scr_down_off;
    at_loc_img[0].i4_x = pt_scr_img_info->t_location_up.i4_x;
    at_loc_img[0].i4_y = pt_scr_img_info->t_location_up.i4_y;
    at_loc_img[1].i4_x = pt_scr_img_info->t_location_down.i4_x;
    at_loc_img[1].i4_y = pt_scr_img_info->t_location_down.i4_y;

    
    for(ui2_i=0; ui2_i<2; ui2_i++)
    {
        for(ui2_k=0; ui2_k<2; ui2_k++)
        {
            if(aah_img[ui2_i][ui2_k]==NULL_HANDLE)
            {
                ab_is_rect_empty[ui2_k] = TRUE;
            }
            else
            {
                if(WGL_IS_FAIL(i4_ret=x_wgl_img_tpl_get_size(aah_img[ui2_i][ui2_k], &ui4_img_w, &ui4_img_h)))
                {
                    INFORM_ERR(i4_ret);
                    ab_is_rect_empty[ui2_k] = TRUE;
                }
                else
                {   
                    SET_RECT_BY_SIZE(&at_rc_tmp[ui2_k], at_loc_img[ui2_i].i4_x, at_loc_img[ui2_i].i4_y, (INT32)ui4_img_w, (INT32)ui4_img_h);
                    ab_is_rect_empty[ui2_k] = FALSE;
                }
            }
        }

        if(ab_is_rect_empty[0] && ab_is_rect_empty[1])
        {
            WGL_RECT_EMPTY(&t_rc_union);
        }
        else if(ab_is_rect_empty[0] && !ab_is_rect_empty[1])
        {
            WGL_RECT_COPY(&t_rc_union, &at_rc_tmp[1]);
        }
        else if(!ab_is_rect_empty[0] && ab_is_rect_empty[1])
        {
            WGL_RECT_COPY(&t_rc_union, &at_rc_tmp[0]);
        }
        else
        {
            UNION_RECT(&t_rc_union, &at_rc_tmp[0], &at_rc_tmp[1]);
        }

        _lbcntdef_purm_add_rect(pt_pur_out, &t_rc_union);
    }

}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_update_region_by_hlt_elem
 *
 * Description: This API add update regions by specifying the highlight element index.
 *
 * Inputs:  pt_ctx            Contains the instance context.
 *          ui2_hlt           Specifies the highlight element index.
 *          pt_pur_out        Contains the input update regions.
 *
 * Outputs: pt_pur_out        Contains the output update regions.
 *
 * Returns: WGLR_OK              Routine successful.
 *          WGLR_INTERNAL_ERROR  Internal error.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_add_update_region_by_hlt_elem(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui2_hlt,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out)
{
    UINT16                          ui2_vi_first, ui2_vi_last;
    INT32                           i4_vp_first_elem_top;
    GL_RECT_T                       t_rc_scrn_elem;
    GL_RECT_T                       t_rc_scrn_elem_elg_logical, t_rc_scrn_elem_elg_paint;
    const INT32                     i4_h_elem = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
    INT32                           i4_ret;
    
    CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_vi_first, &ui2_vi_last, &i4_vp_first_elem_top));
    
    t_rc_scrn_elem.i4_left   = pt_ctx->t_rc_scrn_vp.i4_left;
    t_rc_scrn_elem.i4_top    = pt_ctx->t_rc_scrn_vp.i4_top + (i4_vp_first_elem_top + (((INT32)ui2_hlt) - ((INT32)ui2_vi_first)) * i4_h_elem);
    t_rc_scrn_elem.i4_right  = pt_ctx->t_rc_scrn_vp.i4_right;
    t_rc_scrn_elem.i4_bottom = t_rc_scrn_elem.i4_top + ((INT32)pt_ctx->ui2_elem_height);
    
    if(_lbcntdef_get_hlt_elem_area(
        &pt_ctx->t_effect_hlt_elem,
        PSUEDO_PAINT_STATE_FOR_HLT_ELEM_AREA,
        &pt_ctx->t_rc_scrn_lb,
        &pt_ctx->t_rc_scrn_vp,
        &t_rc_scrn_elem,
        &t_rc_scrn_elem_elg_logical,
        &t_rc_scrn_elem_elg_paint))
    {
        GL_RECT_T   t_rc_lb_elem_elg;
        
        _RECT_MOVE(
            &t_rc_scrn_elem_elg_paint,
            - pt_ctx->t_rc_scrn_lb.i4_left,
            - pt_ctx->t_rc_scrn_lb.i4_top,
            &t_rc_lb_elem_elg);
        _lbcntdef_purm_add_rect(pt_pur_out, &t_rc_lb_elem_elg);
    }
    else
    {
        INFORM_ERR (WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }
        
    return WGLR_OK;
} 

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_update_region_by_range_elems
 *
 * Description: This function appends update regions by a range of elements.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          pt_elem_range  Contains the element range.
 *          pt_pur_out     Contains the original update regions.
 *
 * Outputs: pt_pur_out     Contains the changed update regions.
 *
 * Returns: WGLR_OK        Routine successful.
 *          < 0            Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_add_update_region_by_range_elems (
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      LB_CNT_PU_ELEM_RANGE_T      *pt_elem_range,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out)
{
    UINT16      ui2_vi_first, ui2_vi_last;
    INT32       i4_vp_first_elem_top;
    GL_RECT_T   t_rc_eff_pu_elems;
    INT32       i4_ret;
    
    WGL_RECT_EMPTY(&t_rc_eff_pu_elems);
    CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_vi_first, &ui2_vi_last, &i4_vp_first_elem_top));
    
    /* calculate the element update rect */
    if( pt_ctx->ui2_elem_num > 0                             &&
        pt_elem_range->ui2_pui_first < WGL_LB_MAX_ELEM_NUM   &&
        pt_elem_range->ui2_pui_last < WGL_LB_MAX_ELEM_NUM    &&
        pt_elem_range->ui2_pui_first <= pt_elem_range->ui2_pui_last)
    {
        if(_RANGE_IS_INTERSECT(pt_elem_range->ui2_pui_first, pt_elem_range->ui2_pui_last, ui2_vi_first, ui2_vi_last) )
        {
            UINT16      ui2_pui_first, ui2_pui_last;
            INT32       i4_eff_elem_h, i4_total_update_h;
            GL_RECT_T   t_rc_pu_elems;

            i4_eff_elem_h = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
            ui2_pui_first = (pt_elem_range->ui2_pui_first < ui2_vi_first) ? ui2_vi_first : pt_elem_range->ui2_pui_first;
            ui2_pui_last  = (pt_elem_range->ui2_pui_last > ui2_vi_last) ? ui2_vi_last : pt_elem_range->ui2_pui_last;
            
            t_rc_pu_elems.i4_left   = pt_ctx->t_rc_lb_vp.i4_left;
            t_rc_pu_elems.i4_right  = pt_ctx->t_rc_lb_vp.i4_right;
            t_rc_pu_elems.i4_top    = i4_vp_first_elem_top + pt_ctx->t_rc_lb_vp.i4_top + ((INT32)(ui2_pui_first-ui2_vi_first)) * i4_eff_elem_h;
            i4_total_update_h       = (INT32)((ui2_pui_last+1)-ui2_pui_first) * i4_eff_elem_h;
            t_rc_pu_elems.i4_bottom = t_rc_pu_elems.i4_top + ((i4_total_update_h<=0) ? 0 : i4_total_update_h);

            INTERSECT_RECT(&t_rc_eff_pu_elems, &t_rc_pu_elems, &pt_ctx->t_rc_lb_vp);
            
            _lbcntdef_purm_add_rect(pt_pur_out, &t_rc_eff_pu_elems);
        }
    }
    
    /* post process for enlarged highlight element */
    if(HAS_FLAG(pt_ctx->t_effect_hlt_elem.ui4_style, WGL_LB_HEF_STL_ENLARGE_SIZE))
    {
        UINT16           ui2_i;
        BOOL             b_found_hlt;
        
        b_found_hlt = FALSE;

        for(ui2_i = pt_elem_range->ui2_pui_first; ui2_i <= pt_elem_range->ui2_pui_last; ui2_i ++)
        {
            if(HAS_FLAG(_LBCNTDEF_GET_ELEM_STATE(pt_ctx, ui2_i), LBEST_HLT))
            {
                b_found_hlt = TRUE;
                break;
            }
        }
        
        if(b_found_hlt)
        {
            CATCH_FAIL(_lbcntdef_add_update_region_by_hlt_elem(pt_ctx, ui2_i, pt_pur_out));
        }
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_add_update_region_by_full_lb
 *
 * Description: This function appends update regions by the full listbox area.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          pt_pur_out     Contains the original update regions.
 *
 * Outputs: pt_pur_out     Contains the changed update regions.
 *
 * Returns: WGLR_OK        Routine successful.
 *          < 0            Some other errors.
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_add_update_region_by_full_lb (
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out)
{
    GL_RECT_T   t_rc_lb;
    
    _lbcntdef_purm_reset (pt_pur_out);
    
    SET_RECT (&t_rc_lb, 0, 0, RECT_W(&pt_ctx->t_rc_scrn_lb), RECT_H(&pt_ctx->t_rc_scrn_lb));
    
    _lbcntdef_purm_add_rect (pt_pur_out, &t_rc_lb);
    
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_update_region_by_item
 *
 * Description: This API gets the current update regions by specifying an item.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          ui4_state_wgt  Contains the current state of widget.
 *          ui2_idx        Specifies the element index of the item.
 *          ui1_c          Specifies the column index of the item.
 *
 * Outputs: pt_pur_out  Contains the update regions.
 *
 * Returns: WGLR_OK     Routine successful.
 *          < 0         Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_get_update_region_by_item(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT32                      ui4_state_wgt,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    OUT     LB_CNT_PU_REGION_T          *pt_pur_out)
{
    INT32       i4_ret;
    GL_RECT_T   t_rc_eff_pu_item;
    
    _lbcntdef_purm_copy(pt_pur_out, &pt_ctx->t_pur_wait_for_update);
    
    _lbcntdef_purm_reset(&pt_ctx->t_pur_wait_for_update);

    CATCH_FAIL( _lbcntdef_get_item_update_rect( pt_ctx, (UINT16)ui4_state_wgt, ui2_idx, ui1_c, &t_rc_eff_pu_item ) );
    
    _lbcntdef_purm_add_rect(pt_pur_out, &t_rc_eff_pu_item);

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_item_update_rect
 *
 * Description: This API gets the update rect of an item.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          ui4_state_wgt  Contains the current state of widget.
 *          ui2_idx        Specifies the element index of the item.
 *          ui1_c          Specifies the column index of the item.
 *
 * Outputs: pt_rc_out  Contains the update rect.
 *
 * Returns: WGLR_OK     Routine successful.
 *          < 0         Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_get_item_update_rect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT16                      ui4_state_wgt,
    IN      UINT16                      ui2_idx,
    IN      UINT8                       ui1_c,
    OUT     GL_RECT_T*                  pt_rc_out)
{
    INT32       i4_ret;
    UINT16      ui2_vi_first, ui2_vi_last;
    INT32       i4_vp_first_elem_top;
    GL_RECT_T   t_rc_eff_pu_item;
#ifdef WGL_LB_NO_STORE_TEXT
    WGL_LB_ELEM_DATA_INFO_T     t_elem_data;
    WGL_LB_ITEM_DATA_T*         pt_item_data = NULL;
#endif

    WGL_RECT_EMPTY(pt_rc_out);    
    WGL_RECT_EMPTY(&t_rc_eff_pu_item);
    
    CATCH_FAIL(wgl_vp_get_range(&pt_ctx->t_vp, &ui2_vi_first, &ui2_vi_last, &i4_vp_first_elem_top));
    
    if( pt_ctx->ui2_elem_num > 0             &&
        ui2_vi_first < WGL_LB_MAX_ELEM_NUM   &&
        ui2_vi_last < WGL_LB_MAX_ELEM_NUM    &&
        ui2_idx >= ui2_vi_first              &&
        ui2_idx <= ui2_vi_last)
    {
        GL_RECT_T           t_rc_pu_item;
        UINT8               ui1_k;
        INT32               i4_eff_elem_h = (INT32)(pt_ctx->ui2_elem_height + pt_ctx->ui2_elem_dist);
        LB_ELEM_T*          pt_elem;
        LBCNTDEF_ELEM_T*    pt_cnt_elem;
#ifdef WGL_LB_NO_STORE_TEXT
        INT32               i4_w_len;
#endif        
        _lbcntdef_get_elem (pt_ctx, ui2_idx, &pt_elem, &pt_cnt_elem);
        
        t_rc_pu_item.i4_top    = pt_ctx->t_rc_lb_vp.i4_top + i4_vp_first_elem_top + i4_eff_elem_h * ((INT32)(ui2_idx-ui2_vi_first));
        t_rc_pu_item.i4_bottom = t_rc_pu_item.i4_top + i4_eff_elem_h;
        t_rc_pu_item.i4_left   = pt_ctx->t_rc_lb_vp.i4_left + pt_ctx->t_inset_elem.i4_left;
        
#ifdef WGL_LB_NO_STORE_TEXT
        if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
        {   
            _lbcntdef_no_store_get_data(pt_ctx,&t_elem_data);
        }
#endif   
        
        for(ui1_k=0; ui1_k < ui1_c; ui1_k++)
        {
#ifdef WGL_LB_NO_STORE_TEXT
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
            {
                pt_item_data = NULL;
                _lbcntdef_get_item_data_ex(pt_ctx, &t_elem_data, ui2_idx, ui1_k, &pt_item_data);

                i4_w_len = _lbcntdef_tool_get_item_width_ex (
                            pt_ctx->at_cols,
                            pt_ctx->at_cnt_cols,
                            _WIDGET_STATE_TO_PAINT_STATE (ui4_state_wgt),
                            pt_elem,
                            pt_cnt_elem,
                            ui1_k,
                            pt_item_data); 
                t_rc_pu_item.i4_left += i4_w_len;

            }
            else
#endif                
            {
            t_rc_pu_item.i4_left += _lbcntdef_tool_get_item_width (
                        pt_ctx->at_cols,
                        pt_ctx->at_cnt_cols,
                        _WIDGET_STATE_TO_PAINT_STATE (ui4_state_wgt),
                        pt_elem,
                        pt_cnt_elem,
                        ui1_k);
        }
        
        }

#ifdef WGL_LB_NO_STORE_TEXT
        if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
        {
            pt_item_data = NULL;
            _lbcntdef_get_item_data_ex(pt_ctx, &t_elem_data, ui2_idx, ui1_c, &pt_item_data);

            i4_w_len = _lbcntdef_tool_get_item_width_ex (
                                            pt_ctx->at_cols,
                                            pt_ctx->at_cnt_cols,
                                            _WIDGET_STATE_TO_PAINT_STATE (ui4_state_wgt),
                                            pt_elem,
                                            pt_cnt_elem,
                                            ui1_c,
                                            pt_item_data);

            t_rc_pu_item.i4_right = t_rc_pu_item.i4_left + i4_w_len;

        }
        else
#endif            
        {
        t_rc_pu_item.i4_right = t_rc_pu_item.i4_left + 
                                _lbcntdef_tool_get_item_width (
                                        pt_ctx->at_cols,
                                        pt_ctx->at_cnt_cols,
                                        _WIDGET_STATE_TO_PAINT_STATE (ui4_state_wgt),
                                        pt_elem,
                                        pt_cnt_elem,
                                        ui1_c);
        }
        
        if(RECT_IS_EFFECTIVE(t_rc_pu_item))
        {
            INTERSECT_RECT(&t_rc_eff_pu_item, &t_rc_pu_item, &pt_ctx->t_rc_lb_vp);
            WGL_RECT_COPY(pt_rc_out, &t_rc_eff_pu_item);
        }

#ifdef WGL_LB_NO_STORE_TEXT
        if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
        {
            _lbcntdef_clear_elem_buf(pt_ctx);
        }        
#endif        
    }
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_update_region_generic
 *
 * Description: This API gets the current update regions.
 *
 * Inputs:  pt_ctx            Contains the instance context.
 *
 * Outputs: pt_pur_out        Contains the update regions.
 *
 * Returns: WGLR_OK           Routine successful.
 *          < 0               Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_get_update_region_generic(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     LB_CNT_PU_REGION_T          *pt_pur_out)
{

    _lbcntdef_purm_copy(pt_pur_out, &pt_ctx->t_pur_wait_for_update);
    _lbcntdef_purm_reset(&pt_ctx->t_pur_wait_for_update);
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_set_text_dir
 *
 * Description: This API sets the text direction.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          ui1_text_dir  Indicates the text direction
 *
 * Outputs: pt_ctx        Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_set_text_dir(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      UINT8                       ui1_text_dir)
{
    INT32 i4_ret;
    #if 0
    UINT16              ui2_e;
    UINT8               ui1_c; 
    LB_ELEM_T           *pt_elems;
    INT32               i4_len;
    #endif
    
    if (ui1_text_dir != WGL_TEXT_DIR_LEFT_TO_RIGHT && ui1_text_dir != WGL_TEXT_DIR_RIGHT_TO_LEFT)
    {
        i4_ret = WGLR_INV_ARG;
        INFORM_ERR(i4_ret);
        return i4_ret;
    }
    else
    { 
    	if (!(pt_ctx->ui4_style & WGL_STL_GL_FIX_TEXT_DIR) &&
			  pt_ctx->ui1_txt_dir != ui1_text_dir)
    		{
                pt_ctx->ui1_txt_dir = ui1_text_dir;
                if(!(pt_ctx->ui4_style & WGL_STL_GL_FIX_TEXT_DIR_ALIGN))
                {
                    pt_ctx->b_opp_txt_align= (pt_ctx->b_opp_txt_align) ? FALSE : TRUE;
                }
                /*text dir will affect  pt_ctx->t_rc_lb_ttl_text value*/
                if(pt_ctx->ui2_len_ttl > 0)
                {
                    _lbcntdef_update_title_text_position(pt_ctx, RECT_W(&pt_ctx->t_rc_lb_ttl_text), RECT_H(&pt_ctx->t_rc_lb_ttl_text));
                }
    
            /*AP will reset the text with opposite dircetion*/
            /*if (WGL_TEXT_DIR_RIGHT_TO_LEFT == ui1_text_dir)
            {  
                for (ui1_c=0; ui1_c< pt_ctx->ui1_col_num; ui1_c++ )
                {  
				    if (pt_ctx->at_cols[ui1_c].e_col_type == LB_COL_TYPE_TEXT)
                    {
						pt_elems = (const)(pt_ctx->pt_elems);
	                    for (ui2_e=0; ui2_e< pt_ctx->ui2_elem_num; ui2_e++)
	                    {                               
                            i4_len = (INT32)(x_uc_w2s_strlen(pt_elems->at_items[ui1_c].pw2s_text)); 							
                            if (i4_len > 0)
                            {
                                x_memset(pt_ctx->w2s_buffer,WGL_CHAR_NULL,(SIZE_T)(UTF16_MAX_LEN * (pt_ctx->at_cols[ui1_c].ui2_max_text_len + 1)));
                                i4_ret = reorder_arabic(pt_elems->at_items[ui1_c].pw2s_text,pt_ctx->at_cols[ui1_c].ui2_max_text_len,pt_ctx->w2s_buffer,NULL);
 							    if (i4_ret != WGLR_OK)
 						 	    {
 								    return i4_ret;
 						 	    }								
                                pt_elems->at_items[ui1_c].pw2s_text = x_uc_w2s_strcpy(pt_elems->at_items[ui1_c].pw2s_text,pt_ctx->w2s_buffer);
                            }                      
                            if (pt_elems != NULL)
                            {
                                pt_elems= pt_elems->pt_next;    
                            }
                       }
				 }
                   
               }  
            }  */ 
        }
    }
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_get_text_dir
 *
 * Description: This API gets the text direction.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *
 * Outputs: pui4_text_dir Reference of current text dorection.
 *
 * Returns: WGLR_OK       Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_get_text_dir(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     UINT32*                     pui4_text_dir)
{
    *pui4_text_dir = (UINT32)pt_ctx->ui1_txt_dir;
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cmd_hdr_set_hlt_elem_effect
 *
 * Description: This API sets the effect for highlight element.
 *
 * Inputs:  pt_ctx        Contains the instance context.
 *          pt_effect     Contains the effect parameters for highlighted element.
 *
 * Outputs: pt_ctx        Contains the changed instance context.
 *
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  Invalid arguments.
 *          < 0           Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_cmd_hdr_set_hlt_elem_effect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect)
{
    const UINT8 ui1_last_stl_shift = 4;
    
    CHECK_VALID(
        !(pt_effect->ui4_style & (~(MAKE_BIT_MASK_32((ui1_last_stl_shift + 1)) - 1))), WGLR_INV_ARG);
    
    if(HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_ENHANCE_ELEM_BK))
    {
        CHECK_VALID(
            _lbcntdef_tool_check_valid_hlt_elem_img_set(&pt_effect->t_imgs_bk), WGLR_INV_ARG);
    }
    
    if(HAS_FLAG(pt_effect->ui4_style, WGL_LB_HEF_STL_SCRL_LONG_TEXT))
    {
        const UINT8 ui1_scrl_last_stl_shift = 4;
        
        CHECK_VALID(
            !(pt_effect->t_text_scrl.ui4_style & (~(MAKE_BIT_MASK_32((ui1_scrl_last_stl_shift + 1)) - 1))), WGLR_INV_ARG);
        
        CHECK_VALID(
            ( !HAS_FLAG( pt_effect->t_text_scrl.ui4_style, WGL_LB_TEXT_SCRL_STL_REPEAT ) || 
              pt_effect->t_text_scrl.ui4_scrl_count == 0 )                                  &&
              
            ( pt_effect->t_text_scrl.ui4_ms_delay >= WGL_LB_TEXT_SCRL_MIN_DELAY )           &&
            
            ( pt_effect->t_text_scrl.b_sync == TRUE || pt_effect->t_text_scrl.b_sync == FALSE ), WGLR_INV_ARG);
    }
    
    x_memcpy(&pt_ctx->t_effect_hlt_elem, pt_effect, sizeof(WGL_LB_HLT_ELEM_EFFECT_T));
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cmd_hdr_get_hlt_elem_effect
 *
 * Description: This API handles the command WGL_CMD_LB_EXT_GET_HLT_ELEM_EFFECT.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *
 * Outputs: pt_effect_out  Contains the effect parameters.
 *
 * Returns: WGLR_OK        Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_cmd_hdr_get_hlt_elem_effect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     WGL_LB_HLT_ELEM_EFFECT_T    *pt_effect_out)
{
    x_memcpy(pt_effect_out, &pt_ctx->t_effect_hlt_elem, sizeof(WGL_LB_HLT_ELEM_EFFECT_T));
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cmd_hdr_set_scrl_bar
 *
 * Description: This API handles the command WGL_CMD_LB_EXT_SET_SCRL_BAR.
 *
 * Inputs:  pt_ctx      Contains the instance context.
 *
 * Outputs: pt_attr_sb  Contains the effect parameters.
 *
 * Returns: WGLR_OK     Routine successful.
 *          < 0         Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_cmd_hdr_set_scrl_bar(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_SCRL_BAR_T           *pt_attr_sb)
{
    const UINT16    ui2_shift_sb_last_style = 2;
    LB_SB_ATTRS_T   t_param;
    GL_RECT_T       t_rc_track;
    INT32           i4_ret;
    
    CHECK_VALID(
        !(pt_attr_sb->ui4_style & (~(MAKE_BIT_MASK_32((ui2_shift_sb_last_style + 1)) - 1))) &&
        RECT_IS_EFFECTIVE(pt_attr_sb->t_rc_body)                         &&
        _INSET_IS_VALID(pt_attr_sb->t_inset_track)                       &&
        pt_attr_sb->ui4_sz_thumb_min > 0                                 &&
        _lbcntdef_tool_check_valid_img_set_std(&pt_attr_sb->t_imgs_body) &&
        _lbcntdef_tool_check_valid_img_set_std(&pt_attr_sb->t_imgs_thumb), WGLR_INV_ARG);

    _RECT_INSET_TO(t_rc_track, pt_attr_sb->t_rc_body, pt_attr_sb->t_inset_track);
    CHECK_VALID(
        RECT_IS_EFFECTIVE(t_rc_track), WGLR_INV_ARG);

    t_param.ui2_style = 
        (HAS_FLAG(pt_attr_sb->ui4_style, WGL_LB_SB_STL_MANUAL_MODE)      ? SB_STL_MANUAL_MODE : 0)   |
        (HAS_FLAG(pt_attr_sb->ui4_style, WGL_LB_SB_STL_FIXED_THUMB_SIZE) ? SB_STL_FIXED_THUMB_SIZE : 0);
    
    t_param.ui4_sz_track = (UINT32) RECT_H(&t_rc_track);
    t_param.ui4_sz_thumb_min = pt_attr_sb->ui4_sz_thumb_min;
    t_param.ui4_sz_elem  = (UINT32)pt_ctx->ui2_elem_height;
    t_param.ui4_ds_elem  = (UINT32)pt_ctx->ui2_elem_dist;
    
    CATCH_FAIL(wgl_lb_sb_set_attrs(&pt_ctx->t_ctx_sb, &t_param));
        
    x_memcpy(&pt_ctx->t_attr_sb, pt_attr_sb, sizeof(WGL_LB_SCRL_BAR_T));
    
    
    return WGLR_OK;
}
    
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cmd_hdr_get_req_timer
 *
 * Description: This API handles the WGL_CNT_LB_GET_REQ_TIMER command.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *
 * Outputs: pt_ctx         Contains the changed instance context.
 *          pt_cnt_tr_out  Contains the timer request information.
 *
 * Returns: WGLR_OK        Routine successful.
 *          < 0            Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_cmd_hdr_get_req_timer(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    OUT     LB_CNT_TIMER_REQ_T*         pt_cnt_tr_out)
{
    INT32           i4_ret;
    LB_TR_T         t_tr;
    UINT16          ui2_id_tr;
    
    x_memset( pt_cnt_tr_out, 0, sizeof(LB_CNT_TIMER_REQ_T) );
    
    /* check if queue empty */
    if( anm_trq_check_empty( &pt_ctx->t_trq) )
    {
        return WGL_LBR_NO_MORE_TIMER_REQ;
    }
    
    /* get TR */
    CATCH_FAIL( anm_trq_get( &pt_ctx->t_trq, sizeof(LB_TR_T), &t_tr, &ui2_id_tr ) );
    
    /* do garbage collection in TWL */
    /* policy: clean wait-too-long TR */
    INFORM_ERR( anm_twl_del_by_parse( 
        &pt_ctx->t_twl,  
        lb_tool_twl_gc_fct,
        NULL ));
    
    /* transfer the TR into TWL */
    CATCH_FAIL( anm_twl_append( &pt_ctx->t_twl, ui2_id_tr, &t_tr, sizeof(LB_TR_T) ) ) ;
    
    /* prepare content timer request info */
    pt_cnt_tr_out->e_mode       = t_tr.e_mode;
    pt_cnt_tr_out->ui2_id       = ui2_id_tr;
    pt_cnt_tr_out->ui4_ms_delay = t_tr.ui4_ms_delay;
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_cmd_hdr_do_timer
 *
 * Description: This API handles the WGL_CNT_LB_DO_TIMER command.
 *
 * Inputs:  pt_ctx     Contains the instance context.
 *          pt_param   Contains the parameters.
 *
 * Outputs: pt_ctx     Contains the changed instance context.
 *
 * Returns: WGLR_OK    Routine successful.
 *          < 0        Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_cmd_hdr_do_timer(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    IN      LB_CNT_DO_TIMER_PARAM_T*    pt_param)
{
    INT32       i4_ret, i4_ret_timer;
    LB_TR_T     t_tr;
    
    /* retrieve this timer request */
    CATCH_FAIL( anm_twl_get( &pt_ctx->t_twl, pt_param->ui2_id_tr, sizeof(LB_TR_T), &t_tr) );
    
    i4_ret_timer = WGLR_OK;
    switch( t_tr.e_purpose )
    {
        case LB_TIMER_PRPS_SHT:
            i4_ret_timer = _lbcntdef_timer_hdr_sht( pt_ctx, pt_param->ui4_state_wgt, &t_tr );
        break;
        
        default:
            INFORM_ERR( WGLR_INTERNAL_ERROR );
            i4_ret_timer = WGLR_INTERNAL_ERROR;
    }
    
    return i4_ret_timer;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_timer_hdr_sht
 *
 * Description: This API handles the LB_TIMER_PRPS_SHT timer request.
 *
 * Inputs:  pt_ctx         Contains the instance context.
 *          ui4_state_wgt  Contains the current state of widget.
 *          pt_tr          Contains the timer request info.
 *
 * Outputs: pt_ctx   Contains the changed instance context.
 *
 * Returns: WGLR_OK              Routine successful.
 *          WGLR_INTERNAL_ERROR  Internal error occurs.
 *          < 0                  Some other errors.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_timer_hdr_sht(
    IN OUT  LBCNTDEF_CTX_T*             pt_ctx,
    IN      UINT32                      ui4_state_wgt,
    IN      LB_TR_T*                    pt_tr)
{
    INT32       i4_ret, i4_ret_timer;
    GL_RECT_T   t_rc_eff_pu_item;
    UINT16      ui2_idx_hlt;
    
    /* check TR data */
    CHECK_VALID( pt_tr->u.t_sht.ui1_col < pt_ctx->ui1_col_num , WGLR_INTERNAL_ERROR );
    
    i4_ret_timer = WGLR_OK;
    
    /* prepare PUR */
    ui2_idx_hlt = _lbcntdef_get_hlt_idx(pt_ctx);
    
    if( ui2_idx_hlt < pt_ctx->ui2_elem_num )
    {
        #if 1   
        CATCH_FAIL(_lbcntdef_add_update_region_by_hlt_elem (
                        pt_ctx, 
                        ui2_idx_hlt, 
                        &pt_ctx->t_pur_wait_for_update));
        #else
        CATCH_FAIL( _lbcntdef_get_item_update_rect(
            pt_ctx, 
            (UINT16)ui4_state_wgt,
            ui2_idx_hlt,
            pt_tr->u.t_sht.ui1_col,
            &t_rc_eff_pu_item) );
        #endif   
        _lbcntdef_purm_add_rect( 
            &pt_ctx->t_pur_wait_for_update, 
            &t_rc_eff_pu_item);
            
        i4_ret_timer = WGL_LBR_NEED_REPAINT_PARTIAL;
    }

    return i4_ret_timer;
}        

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_get_first_line_len
 *
 * Description: This API gets the length of the first line, 
 *              which may break by the line break symbol.
 *
 * Inputs:  w2s_text    The text to be parse.
 *
 * Outputs: *
 *
 * Returns: The length
 *          
 ----------------------------------------------------------------------------*/
static UINT32 _lbcntdef_tool_get_first_line_len (
    IN      UINT8                       ui1_txt_dir,
    IN      UTF16_T*                    w2s_text)
{
    UTF16_T         w2s_force_break[] = WGL_FORCE_BREAK_CHARS;
    UINT32          ui4_len;
    UTF16_T         w2_ch;

    ui4_len = 0;
    if (ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT )
    {
        do 
        {
            w2_ch = x_uc_w2s_get (w2s_text, ui4_len);
            
            if (w2_ch == ((UTF16_T) 0) )
            {
                break;
            }
            
            if (x_uc_w2s_chr (w2s_force_break, w2_ch))
            {
                break;
            }
            
            ui4_len ++;
            
        } while (w2_ch != ((UTF16_T) 0));
    }
    else
    {
        UINT32 ui4_str_len;

        ui4_str_len = x_uc_w2s_strlen(w2s_text); 
        
        while (ui4_str_len > 0) 
        {         
            ui4_str_len --;
            
            w2_ch = x_uc_w2s_get (w2s_text, ui4_str_len);
            
            if (x_uc_w2s_chr (w2s_force_break, w2_ch))
            {
                break;
            }
            
            ui4_len ++;
        }
    }
    
    return ui4_len;
}    

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_cut_line_by_soft_hyphen
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_tool_cut_line_by_soft_hyphen (
    IN      UINT8                       ui1_txt_dir,
    IN      HFONT_T                     h_font,
    IN      UTF16_T*                    w2s_line,
    IN      UINT32                      ui4_len_line,
    IN      INT32                       i4_w_bound,
    OUT     UINT32*                     pui4_len_line_cut,
    OUT     INT32*                      pi4_w_cut,
    OUT     INT32*                      pi4_h)
{
    UTF16_T     w2s_soft_hyphen[2] = {WGL_UTF16_SOFT_HYPHEN, 0};
    UTF16_T*    w2s_temp;
    UINT32      ui4_len_line_cut;
    INT32       i4_w_line_cut;
    INT16       i2_h_font;
    INT32       i4_ret;
    
    *pui4_len_line_cut = 0;
    *pi4_w_cut         = 0;
    *pi4_h             = 0;
    
    if (ui4_len_line == 0)
    {
        return WGLR_OK;
    }

    if (ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT)
    {
        i4_ret = wgl_get_max_char_in_box_ex (h_font,
                                             w2s_line,
                                             (INT32) ui4_len_line,
                                             TRUE,
                                             i4_w_bound,
                                             w2s_soft_hyphen,
                                             w2s_soft_hyphen,
                                             1,
                                             0,
                                             FALSE,
                                             & w2s_temp,
                                             & ui4_len_line_cut,
                                             & i4_w_line_cut );

        if (i4_ret != WGLR_OK)
        {
            INFORM_ERR (i4_ret);
            return i4_ret;
        }

        if (ui4_len_line_cut < ui4_len_line)
        {
            /* case 4: calculate the whole string width without Soft-Hyphen */
            i4_ret = wgl_get_max_char_in_box_ex (h_font,
                                                 w2s_line,
                                                 (INT32) ui4_len_line,
                                                 TRUE,
                                                 MAX_VALUE,
                                                 NULL,
                                                 w2s_soft_hyphen,
                                                 1,
                                                 0,
                                                 TRUE,
                                                 &w2s_temp,
                                                 & ui4_len_line_cut,
                                                 & i4_w_line_cut );
                                                 
            if (i4_ret != WGLR_OK)
            {
                INFORM_ERR (i4_ret);
                return i4_ret;
            }
        }
        else if (ui4_len_line_cut == ui4_len_line  && 
                 WGL_UTF16_SOFT_HYPHEN == x_uc_w2s_get (w2s_line, ui4_len_line_cut - 1) )
        {
            /* case 2: substract the width of the tail Soft-Hyphen */
            i4_ret = wgl_get_max_char_in_box_ex (h_font,
                                                 w2s_line,
                                                 (INT32) ui4_len_line,
                                                 TRUE,
                                                 i4_w_bound,
                                                 w2s_soft_hyphen,
                                                 w2s_soft_hyphen,
                                                 1,
                                                 0,
                                                 TRUE,
                                                 & w2s_temp,
                                                 & ui4_len_line_cut,
                                                 & i4_w_line_cut );

            if (i4_ret != WGLR_OK)
            {
                INFORM_ERR (i4_ret);
                return i4_ret;
            }

            if (ui4_len_line_cut > 0)
            {
                ui4_len_line_cut --;
            }
        }
    }
    else
    {
        UINT32 ui4_str_len;
        ui4_str_len = x_uc_w2s_strlen(w2s_line);
        i4_ret = wgl_get_max_char_in_box_for_arab(
                                                 (INT32) ui4_len_line, 
                                                 h_font, 
                                                 &w2s_line[ui4_str_len - 1], 
                                                 (INT32) ui4_len_line, 
                                                 TRUE, 
                                                 i4_w_bound, 
                                                 w2s_soft_hyphen,
                                                 w2s_soft_hyphen,
                                                 1,
                                                 0,
                                                 FALSE,
                                                 & w2s_temp,
                                                 & ui4_len_line_cut,
                                                 & i4_w_line_cut );


        if (i4_ret != WGLR_OK)
        {
            INFORM_ERR (i4_ret);
            return i4_ret;
        }

        if (ui4_len_line_cut < ui4_len_line)
        {
            /* case 4: calculate the whole string width without Soft-Hyphen */
            i4_ret = wgl_get_max_char_in_box_for_arab(
                                                 (INT32) ui4_len_line, 
                                                 h_font, 
                                                 &w2s_line[ui4_str_len - 1], 
                                                 (INT32) ui4_len_line, 
                                                 TRUE, 
                                                 MAX_VALUE, 
                                                 NULL,
                                                 w2s_soft_hyphen,
                                                 1,
                                                 0,
                                                 TRUE,
                                                 & w2s_temp,
                                                 & ui4_len_line_cut,
                                                 & i4_w_line_cut ); 
                                                 
            if (i4_ret != WGLR_OK)
            {
                INFORM_ERR (i4_ret);
                return i4_ret;
            }
        }
        else if (ui4_len_line_cut == ui4_len_line  && 
                 WGL_UTF16_SOFT_HYPHEN == x_uc_w2s_get (w2s_line, ui4_len_line_cut - 1) )
        {
            /* case 2: substract the width of the tail Soft-Hyphen */
            i4_ret = wgl_get_max_char_in_box_for_arab(
                                                 (INT32) ui4_len_line, 
                                                 h_font, 
                                                 &w2s_line[ui4_str_len - 1], 
                                                 (INT32) ui4_len_line, 
                                                 TRUE, 
                                                 i4_w_bound, 
                                                 w2s_soft_hyphen,
                                                 w2s_soft_hyphen,
                                                 1,
                                                 0,
                                                 TRUE,
                                                 & w2s_temp,
                                                 & ui4_len_line_cut,
                                                 & i4_w_line_cut );

            if (i4_ret != WGLR_OK)
            {
                INFORM_ERR (i4_ret);
                return i4_ret;
            }

            if (ui4_len_line_cut > 0)
            {
                ui4_len_line_cut --;
            }
        } 
    }
    
    i4_ret = x_fe_get_height (h_font, &i2_h_font);
    
    if (i4_ret != FER_OK)
    {
        INFORM_ERR (i4_ret);
        return WGLR_INTERNAL_ERROR;
    }
    
    *pui4_len_line_cut = ui4_len_line_cut;
    *pi4_w_cut         = i4_w_line_cut;
    *pi4_h             = (INT32) i2_h_font;
    
    return WGLR_OK;
}
    
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_tool_get_item_width
 *
 * Description: This function calculate the real width of an item.
 *
 * Inputs:  
 *          at_cols          Contains the array of columns' formats
 *          at_cnt_cols      Contains the array of content columns' formats
 *          ui2_state_paint  Specifies the current visual state of the widget. 
 *          pt_elem          Contains the context of the item's element.
 *          pt_cnt_elem      Contains the context of the item's content element.
 *          ui1_c            Specifies the column index of the item.
 *
 * Outputs: -
 *
 * Returns: The real width of the item.
 ----------------------------------------------------------------------------*/
static INT32 _lbcntdef_tool_get_item_width (
    IN      const LB_COL_T              *at_cols,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c)
{
    INT32   i4_w_col_eff;
    UINT16  ui2_col_width = at_cnt_cols[ui1_c].ui2_col_width;
    INT32   i4_ret;
    
    i4_w_col_eff = 0;
    
    if (!HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
        !HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        i4_w_col_eff = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
    }
    else if ( HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
             !HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        INT32   i4_w_col_not_empty;
        
        i4_w_col_not_empty = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
        
        switch (at_cols[ui1_c].e_col_type)
        {
            case LB_COL_TYPE_TEXT:
                i4_w_col_eff = (pt_elem->at_items[ui1_c].t_text.pw2s_text[0] == ((UTF16_T) 0)) ? 0 : i4_w_col_not_empty;
            break;
            
            case LB_COL_TYPE_IMG:
                i4_w_col_eff = (pt_elem->at_items[ui1_c].h_img == NULL_HANDLE) ? 0 : i4_w_col_not_empty;
            break;
            
            case LB_COL_TYPE_IMG_SUIT:
            {
                WGL_HIMG_TPL_T      h_img;
                
                _lbcntdef_select_item_img_by_state (
                        pt_elem->at_items[ui1_c].pt_img_suit,
                        ui2_state_paint,
                        LB_GET_STATE_BY_ELEM(pt_elem),
                        & h_img);
                
                i4_w_col_eff = (h_img == NULL_HANDLE) ? 0 : i4_w_col_not_empty;
            }
            break;
        }
        
    }
    else if ( HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
              HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        INT32               i4_w_col_max, i4_w_data, i4_w_total;
        BOOL                b_add_inset;
        const WGL_INSET_T*  pt_inset_col = &at_cnt_cols[ui1_c].t_inset;
        
        b_add_inset  = FALSE;
        i4_w_col_max = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
        i4_w_data    = 0;
        
        switch (at_cols[ui1_c].e_col_type)
        {
            case LB_COL_TYPE_TEXT:
            {
                if (pt_elem->at_items[ui1_c].t_text.pw2s_text[0] == ((UTF16_T) 0))
                {
                    i4_w_data = 0;
                }
                else
                {
                    HFONT_T  h_font;
                    INT32    i4_w_text, i4_h_text;
                    
                    _lbcntdef_get_item_attr(pt_cnt_elem, at_cnt_cols, ui1_c, ITEM_ATTR_FONT, (VOID *)&h_font);
                    
                    i4_ret = x_fe_get_string_size (h_font, pt_elem->at_items[ui1_c].t_text.pw2s_text, -1, &i4_w_text, &i4_h_text);
                    
                    if (i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE)
                    {
                        i4_w_data   = i4_w_text;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR(i4_ret);
                        i4_w_data = i4_w_col_max;
                    }
                }
            }
            break;
            
            case LB_COL_TYPE_IMG:
            {
                if (pt_elem->at_items[ui1_c].h_img != NULL_HANDLE)
                {
                    UINT32  ui4_w_img, ui4_h_img;
                    
                    i4_ret = x_wgl_img_tpl_get_size (pt_elem->at_items[ui1_c].h_img,
                                                     &ui4_w_img,
                                                     &ui4_h_img);
                                                     
                    if (i4_ret == WGLR_OK)
                    {
                        i4_w_data = (INT32) ui4_w_img;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR (i4_ret);
                        i4_w_data = 0;
                    }
                }
                else
                {
                    i4_w_data = 0;
                }
            }
            break;
            
            case LB_COL_TYPE_IMG_SUIT:
            {
                WGL_HIMG_TPL_T      h_img;
                
                _lbcntdef_select_item_img_by_state (
                        pt_elem->at_items[ui1_c].pt_img_suit,
                        ui2_state_paint,
                        LB_GET_STATE_BY_ELEM(pt_elem),
                        & h_img);
                
                if (h_img != NULL_HANDLE)
                {
                    UINT32  ui4_w_img, ui4_h_img;
                    
                    i4_ret = x_wgl_img_tpl_get_size (h_img,
                                                     &ui4_w_img,
                                                     &ui4_h_img);
                                                     
                    if (i4_ret == WGLR_OK)
                    {
                        i4_w_data = (INT32) ui4_w_img;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR (i4_ret);
                        i4_w_data = 0;
                    }
                }
                else
                {
                    i4_w_data = 0;
                }
            }
            break;
        }
        
        i4_w_total = i4_w_data;
        
        if (b_add_inset)
        {
            i4_w_total += (pt_inset_col->i4_left + pt_inset_col->i4_right);
        }
        
        i4_w_col_eff = MIN (i4_w_total, i4_w_col_max);
    }
    else
    {
        INFORM_ERR (WGLR_INTERNAL_ERROR);
    }
    
    return i4_w_col_eff;
}
        
/*-----------------------------------------------------------------------------
                    Partial Update Region Manager (PURM)
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_purm_reset
 *
 * Description: This API clears all the partial update regions.
 *
 * Inputs:  pt_pur_out            Contains the update regions.
 *
 * Outputs: pt_pur_out            Contains the changed update regions.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_purm_reset(
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur)
{
    pt_pur->ui2_num_rect = 0;
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_purm_copy
 *
 * Description: This API copies the partial update regions.
 *
 * Inputs:  pt_pur_src  Contains the source update regions.
 *
 * Outputs: pt_pur_dst  Contains the destination update regions.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_purm_copy(
    OUT     LB_CNT_PU_REGION_T          *pt_pur_dst, 
    IN      LB_CNT_PU_REGION_T          *pt_pur_src)
{
    pt_pur_dst->ui2_num_rect = pt_pur_src->ui2_num_rect;
    if(pt_pur_src->ui2_num_rect > 0 && pt_pur_src->ui2_num_rect <= LB_CNT_PU_REGION_MAX_NUM_RECT)
    {
        x_memcpy(pt_pur_dst->at_rcs, pt_pur_src->at_rcs, sizeof(GL_RECT_T)*pt_pur_src->ui2_num_rect);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_purm_add_rect
 *
 * Description: This API adds an update region into the update region set.
 *
 * Inputs:  pt_pur_out            Contains the update regions.
 *          pt_rc                 Contains the update rect.
 *
 * Outputs: pt_pur_out            Contains the changed update regions.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _lbcntdef_purm_add_rect(
    IN OUT  LB_CNT_PU_REGION_T          *pt_pur_out, 
    IN      GL_RECT_T                   *pt_rc)
{
    UINT16              ui2_i, ui2_irc_merge=0, ui2_num_rect_left;
    BOOL                b_keep_merge, b_has_empty_rect;
    GL_RECT_T           t_rc_union;

    if(!RECT_IS_EFFECTIVE(*pt_rc))
    {
        return;
    }

    /* append the rectangle at the tail of the rectangle list */
    if(pt_pur_out->ui2_num_rect >= LB_CNT_PU_REGION_MAX_NUM_RECT)
    {   /*by default, merge with the first rectangle*/
        UNION_RECT(&t_rc_union, pt_rc, &pt_pur_out->at_rcs[0]);
        WGL_RECT_COPY(&pt_pur_out->at_rcs[0], &t_rc_union);
        ui2_irc_merge = 0;
    }
    else
    {
        WGL_RECT_COPY(&pt_pur_out->at_rcs[pt_pur_out->ui2_num_rect], pt_rc);
        ui2_irc_merge = pt_pur_out->ui2_num_rect;
        pt_pur_out->ui2_num_rect++;
    }

    /* merge phase */
    b_has_empty_rect  = FALSE;
    b_keep_merge      = TRUE;
    ui2_num_rect_left = pt_pur_out->ui2_num_rect;
            
    while(b_keep_merge && ui2_num_rect_left > 1)
    {
        b_keep_merge = FALSE;
        for(ui2_i=0; ui2_i<pt_pur_out->ui2_num_rect && ui2_i < LB_CNT_PU_REGION_MAX_NUM_RECT; ui2_i++)
        {
            if(ui2_i!=ui2_irc_merge && !WGL_RECT_IS_EMPTY(&pt_pur_out->at_rcs[ui2_i]))
            {
                if(_lbcntdef_purm_tool_decide_mergence(&pt_pur_out->at_rcs[ui2_irc_merge], &pt_pur_out->at_rcs[ui2_i]))
                {  /*merge rectangle ui2_i into rectangle ui2_irc_merge*/
                    UNION_RECT(&t_rc_union, &pt_pur_out->at_rcs[ui2_irc_merge], &pt_pur_out->at_rcs[ui2_i]);
                    WGL_RECT_COPY(&pt_pur_out->at_rcs[ui2_irc_merge], &t_rc_union);
                    WGL_RECT_EMPTY(&pt_pur_out->at_rcs[ui2_i]);
                    ui2_num_rect_left--;
                    b_has_empty_rect = TRUE;
                    b_keep_merge     = TRUE;
                    break;
                }
            }
        }
    }

    /* clear empty rectangles */
    if(b_has_empty_rect)
    {
        LB_CNT_PU_REGION_T  t_pur_tmp;
        t_pur_tmp.ui2_num_rect = 0;
        for(ui2_i=0; ui2_i<pt_pur_out->ui2_num_rect && ui2_i < LB_CNT_PU_REGION_MAX_NUM_RECT; ui2_i++)
        {
            if(!WGL_RECT_IS_EMPTY(&pt_pur_out->at_rcs[ui2_i]))
            {
                WGL_RECT_COPY(&t_pur_tmp.at_rcs[t_pur_tmp.ui2_num_rect], &pt_pur_out->at_rcs[ui2_i]);
                t_pur_tmp.ui2_num_rect++;
            }
        }
        _lbcntdef_purm_copy(pt_pur_out, &t_pur_tmp);
    }


#ifdef _CHECK_CONSISTENCY
    {
        UINT16 ui2_k;
        
        for(ui2_i=0; ui2_i<pt_pur_out->ui2_num_rect&& ui2_i < LB_CNT_PU_REGION_MAX_NUM_RECT; ui2_i++)
        {
            if(WGL_RECT_IS_EMPTY(&pt_pur_out->at_rcs[ui2_i]))
            {
                INFORM_ERR(WGLR_INTERNAL_ERROR);
            }
            for(ui2_k=0; ui2_k<pt_pur_out->ui2_num_rect && ui2_k < LB_CNT_PU_REGION_MAX_NUM_RECT; ui2_k++)
            {
                if(ui2_k!=ui2_i)
                {
                    INFORM_ERR(x_wgl_rect_is_cross(&b_is_cross, &pt_pur_out->at_rcs[ui2_k], &pt_pur_out->at_rcs[ui2_i]));
                    if(b_is_cross)
                    {
                        INFORM_ERR(WGLR_INTERNAL_ERROR);
                    }
                }
            }
        }

    }
#endif

}

/*-----------------------------------------------------------------------------
 * Name: _lbcntdef_purm_tool_decide_mergence
 *
 * Description: This API decides whether to merge the two rects or not.
 *
 * Inputs:  pt_rc_1            Contains the first rect.
 *          pt_rc_2            Contains the second rect.
 *
 * Outputs: -
 *
 * Returns: TRUE    If need to merge them.
 *          FALSE   Otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _lbcntdef_purm_tool_decide_mergence(
    IN      GL_RECT_T                   *pt_rc_1,
    IN      GL_RECT_T                   *pt_rc_2)
{
    GL_RECT_T   t_rc_intct;
    
    INTERSECT_RECT(&t_rc_intct, pt_rc_1, pt_rc_2);
    if(RECT_IS_EFFECTIVE(t_rc_intct))
    {
        return TRUE;
    }
    
    /* check if rc_1 & rc_2 are adjacent */
    if( t_rc_intct.i4_left == t_rc_intct.i4_right ||
        t_rc_intct.i4_top == t_rc_intct.i4_bottom )
    {
        /* check if AREA(UNION(A,B)) == (AREA(A) + AREA(B)) */
        GL_RECT_T   t_rc_un;
        
        UNION_RECT(&t_rc_un, pt_rc_1, pt_rc_2);
        if( _RECT_AREA(&t_rc_un) == 
            _RECT_AREA(pt_rc_1) + _RECT_AREA(pt_rc_2))
        {
            return TRUE;
        }        
    }
    
    return FALSE;
}    
    

#ifdef WGL_ANIM_SUPPORT
static INT32 _lbcntdef_add_update_region(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  WGL_UPD_REGION_T            *pt_upd_rgn)
{
    UINT32                              ui4_i;
    
    if(pt_ctx == NULL || pt_upd_rgn == NULL || pt_upd_rgn->ui4_update_rect_num == 0)
    {
        return WGLR_INV_ARG;
    }

    for(ui4_i = 0; ui4_i < pt_upd_rgn->ui4_update_rect_num; ui4_i ++)
    {
        if(ui4_i <= WGL_MAX_UPDATE_RECT_NUM)
        {
            _lbcntdef_purm_add_rect(&pt_ctx->t_pur_wait_for_update, &pt_upd_rgn->at_update_rect[ui4_i]);        
        }
    }
    
    return WGLR_OK;
}

static INT32 _lbcntdef_cmd_hdr_set_anim_elem_effect(
    IN OUT  LBCNTDEF_CTX_T              *pt_ctx,
    IN      WGL_LB_ANIM_ELEM_EFFECT_T    *pt_anim)
{

    x_memcpy(&pt_ctx->t_anim_elem, pt_anim, sizeof(WGL_LB_ANIM_ELEM_EFFECT_T));

    return WGLR_OK;
}

static INT32 _lbcntdef_cmd_hdr_get_anim_elem_effect(
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    OUT     WGL_LB_ANIM_ELEM_EFFECT_T   *pt_effect_out)
{
    x_memcpy(pt_effect_out, &pt_ctx->t_anim_elem, sizeof(WGL_LB_ANIM_ELEM_EFFECT_T));
    return WGLR_OK;
}

static INT32 _lbcntdef_tool_check_anim_elem(
    LBCNTDEF_CTX_T                              *pt_ctx,
    LB_ELEM_T                                   **ppt_anim_elem)
{
    LB_ELEM_T                                   *pt_elem;
    UINT16                                      ui2_i;
    LBCNTDEF_ELEM_T                             *pt_cnt_elem;

    if(ppt_anim_elem == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    *ppt_anim_elem = NULL;
    
     _lbcntdef_get_elem(pt_ctx, 0, &pt_elem, &pt_cnt_elem);

    for(ui2_i = 0; ui2_i < pt_ctx->ui2_elem_num && pt_elem; ui2_i ++)
    {
        if(HAS_FLAG(LB_GET_STATE_BY_ELEM(pt_elem), LBEST_ANIM))
        {
            *ppt_anim_elem = pt_elem;
            return WGLR_OK;
        }

        pt_elem          = pt_elem->pt_next;
    }
    
    return WGLR_OK;
}
#endif

static INT32 _lbcntdef_do_cb_fct(
    LBCNTDEF_CTX_T*                         pt_ctx,
    WGL_LB_NFY_TYPE_T                       e_nfy_type,
    VOID*                                   pv_param1,
    VOID*                                   pv_param2)
{
    BOOL                        b_is_frm_mw_thread = FALSE;
    
    b_is_frm_mw_thread = wgl_cur_thread_is_mw_thread();
    pt_ctx->pf_lb_cb_fct(pt_ctx->h_lb,
                         b_is_frm_mw_thread,
                         e_nfy_type,
                         pv_param1,
                         pv_param2);
    return WGLR_OK;
}

#ifdef WGL_LB_NO_STORE_TEXT
static INT32 _lbcntdef_get_item_data_ex(
    IN      LBCNTDEF_CTX_T*             pt_ctx,    
    IN      WGL_LB_ELEM_DATA_INFO_T*    pt_elem_data,
    IN      UINT16                      ui2_elem,
    IN      UINT8                       ui1_col,
    OUT     WGL_LB_ITEM_DATA_T**        ppt_item_data)
{
    if(ppt_item_data == NULL)
    {
        return WGLR_INV_ARG;
    }
    *ppt_item_data = NULL;
    
    if((pt_elem_data == NULL) || 
       (ui2_elem > pt_elem_data->ui2_elem_num) ||
       (ui1_col > pt_elem_data->ui1_col_num) || 
       (pt_elem_data->pt_elem_data == NULL) ||
       (pt_elem_data->pt_elem_data[ui2_elem].at_items == NULL))
    {
        return WGLR_INV_ARG;
    }

    *ppt_item_data = &pt_elem_data->pt_elem_data[ui2_elem].at_items[ui1_col];
    
    return WGLR_OK;
}

static INT32 _lbcntdef_render_elem_ex( 
    IN      LBCNTDEF_CTX_T              *pt_ctx,
    IN OUT  GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint,
    IN      LB_ELEM_T                   *pt_elem,
    IN      INT32                       i4_elem_top_scrn,
    IN      GL_RECT_T                   *pt_rc_scrn_upt,
    IN      WGL_LB_ELEM_DATA_INFO_T*    pt_elem_data,
    IN      UINT16                      ui2_elem)
{
    INT32                   i4_ret;
    GL_RECT_T               t_rc_scrn_elem, t_rc_scrn_elem_cnt;
    LBCNTDEF_ELEM_T         *pt_cnt_elem = (LBCNTDEF_ELEM_T *)pt_elem->pv_cnt_elem;
    WGL_LB_ITEM_DATA_T*     pt_item_data = NULL;
    
    ___BLOCK_BEGIN___
    
        SET_RECT(
            &t_rc_scrn_elem, 
            pt_ctx->t_rc_scrn_vp.i4_left,
            i4_elem_top_scrn,
            pt_ctx->t_rc_scrn_vp.i4_right,
            i4_elem_top_scrn + ((INT32)pt_ctx->ui2_elem_height));
            
        /* draw elem bk */
        if(HAS_FLAG(LB_GET_STATE_BY_ELEM(pt_elem), LBEST_HLT))
        {
            /* draw the highlighted element bk */
            if(!_lbcntdef_draw_elem_bk_hlt(
                h_g,
                &pt_ctx->t_cvsst,
                &pt_ctx->t_effect_hlt_elem,
                LB_GET_STATE_BY_ELEM(pt_elem),
                ui2_state_paint,
                &pt_ctx->t_clrs_elem_bk,
                &pt_ctx->t_imgs_elem_bk,
                &pt_ctx->t_rc_scrn_lb,
                &pt_ctx->t_rc_scrn_vp,
                &t_rc_scrn_elem,
                pt_rc_scrn_upt))
            {
                GOTO_EOB;
            }
        }
        else
        {
            /* draw the non-highlighted element bk */
            if(!_lbcntdef_draw_elem_bk_normal(
                h_g,
                &pt_ctx->t_cvsst,
                LB_GET_STATE_BY_ELEM(pt_elem),
                ui2_state_paint,
                &pt_ctx->t_clrs_elem_bk,
                &pt_ctx->t_imgs_elem_bk,
                &t_rc_scrn_elem,
                pt_rc_scrn_upt))
            {
                GOTO_EOB;
            }
        }
            
        /* draw items of the element */
        _RECT_INSET_TO(t_rc_scrn_elem_cnt, t_rc_scrn_elem, pt_ctx->t_inset_elem);
        if(RECT_IS_EFFECTIVE(t_rc_scrn_elem_cnt))
        {
            GL_RECT_T   t_rc_scrn_elem_cnt_upt;
            GL_RECT_T   t_rc_scrn_col, t_rc_scrn_col_upt;
            GL_RECT_T   t_rc_scrn_col_cnt, t_rc_scrn_col_cnt_upt;
            UINT8       ui1_c;
            INT32       i4_w_col, i4_scrn_col_left;
            
            /* check for quick return */
            INTERSECT_RECT(&t_rc_scrn_elem_cnt_upt, &t_rc_scrn_elem_cnt, pt_rc_scrn_upt);
            if(!RECT_IS_EFFECTIVE(t_rc_scrn_elem_cnt_upt))
            {
                GOTO_EOB;
            }

            i4_scrn_col_left = t_rc_scrn_elem_cnt.i4_left;
            
            for(ui1_c = 0; ui1_c < pt_ctx->ui1_col_num; ui1_c ++)
            {
                ___BLOCK_BEGIN___

                    pt_item_data = NULL;
                    _lbcntdef_get_item_data_ex(pt_ctx,
                        pt_elem_data,
                        ui2_elem,
                        ui1_c,
                        &pt_item_data);
                    
                    /* calc the real width of the item */
                    i4_w_col = _lbcntdef_tool_get_item_width_ex (
                            pt_ctx->at_cols,
                            pt_ctx->at_cnt_cols,
                            ui2_state_paint,
                            pt_elem,
                            pt_cnt_elem,
                            ui1_c,
                            pt_item_data);
                                                                
                    /* draw the item's background */
                    SET_RECT(
                        &t_rc_scrn_col,
                        i4_scrn_col_left,
                        t_rc_scrn_elem_cnt.i4_top,
                        i4_scrn_col_left + i4_w_col,
                        t_rc_scrn_elem_cnt.i4_bottom);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col))
                    {
                        GOTO_EOB;
                    }

                    INTERSECT_RECT(&t_rc_scrn_col_upt, &t_rc_scrn_col, &t_rc_scrn_elem_cnt_upt);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_upt))
                    {
                        GOTO_EOB;
                    }
                    
                    /* set the origin and clipping of h_g */
                    _LBCNTDEF_SET_CLIP_RECT(h_g, t_rc_scrn_col, t_rc_scrn_col_upt, i4_ret);
                    
                    /* render bk */
                    INFORM_ERR(_lbcntdef_draw_item_bk(
                        h_g,
                        &pt_ctx->t_cvsst,
                        pt_ctx->at_cnt_cols,
                        &t_rc_scrn_col,
                        ui2_state_paint,
                        pt_elem,
                        pt_cnt_elem,
                        ui1_c,
                        &t_rc_scrn_col_upt));
                    
                    /* render the item's content area */
                    _RECT_INSET_TO(
                        t_rc_scrn_col_cnt,
                        t_rc_scrn_col,
                        pt_ctx->at_cnt_cols[ui1_c].t_inset);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_cnt))
                    {
                        GOTO_EOB;
                    }
                    INTERSECT_RECT(
                        &t_rc_scrn_col_cnt_upt, 
                        &t_rc_scrn_col_cnt, 
                        &t_rc_scrn_elem_cnt_upt);
                    if(!RECT_IS_EFFECTIVE(t_rc_scrn_col_cnt_upt))
                    {
                        GOTO_EOB;
                    }
                    
                    /* set the origin and clipping of h_g */
                    _LBCNTDEF_SET_CLIP_RECT(h_g, t_rc_scrn_col_cnt, t_rc_scrn_col_cnt_upt, i4_ret);
                    
                    /* render content */
                    INFORM_ERR(_lbcntdef_render_col_data_ex(
                        pt_ctx,
                        h_g,
                        &t_rc_scrn_col_cnt,
                        ui2_state_paint,
                        pt_elem,
                        pt_cnt_elem,
                        ui1_c,
                        &t_rc_scrn_col_cnt_upt,
                        pt_item_data));
                    
                ___BLOCK_END___
                    
                i4_scrn_col_left += i4_w_col;
                
                if(i4_scrn_col_left >= t_rc_scrn_elem_cnt.i4_right)
                {
                    break;
                }
            }
        }
        
    ___BLOCK_END___
    
    /* check HETSM's marker */
    if ( HAS_FLAG(LB_GET_STATE_BY_ELEM(pt_elem), LBEST_HLT ) )
    {
        /* check marker and reset if necessary */
        if ( ! lb_hetsm_check_marker( & pt_ctx->t_hetsm, MAKE_HETSM_MARKER(pt_cnt_elem) ) )
        {
            INFORM_ERR( lb_hetsm_reset( 
                & pt_ctx->t_hetsm, 
                MAKE_HETSM_MARKER(pt_cnt_elem) ) );
        }
        
    }
    
    
    return WGLR_OK;
}

static INT32 _lbcntdef_tool_get_item_width_ex (
    IN      const LB_COL_T              *at_cols,
    IN      LBCNTDEF_COL_T              *at_cnt_cols,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN     WGL_LB_ITEM_DATA_T*          pt_item_data)
{
    INT32   i4_w_col_eff;
    UINT16  ui2_col_width = at_cnt_cols[ui1_c].ui2_col_width;
    INT32   i4_ret;
    UTF16_T *pw2s_text = NULL;             
    
    i4_w_col_eff = 0;

    if(pt_item_data != NULL)
    {
        pw2s_text = pt_item_data->pw2s_text;
    }
    
    if (!HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
        !HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        i4_w_col_eff = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
    }
    else if ( HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
             !HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        INT32   i4_w_col_not_empty;
        
        i4_w_col_not_empty = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
        
        switch (at_cols[ui1_c].e_col_type)
        {
            case LB_COL_TYPE_TEXT:
                i4_w_col_eff = ((pw2s_text == NULL) || (pw2s_text[0] == ((UTF16_T) 0))) ? 0 : i4_w_col_not_empty;
            break;
            
            case LB_COL_TYPE_IMG:
                i4_w_col_eff = (pt_elem->at_items[ui1_c].h_img == NULL_HANDLE) ? 0 : i4_w_col_not_empty;
            break;
            
            case LB_COL_TYPE_IMG_SUIT:
            {
                WGL_HIMG_TPL_T      h_img;
                
                _lbcntdef_select_item_img_by_state (
                        pt_elem->at_items[ui1_c].pt_img_suit,
                        ui2_state_paint,
                        LB_GET_STATE_BY_ELEM(pt_elem),
                        & h_img);
                
                i4_w_col_eff = (h_img == NULL_HANDLE) ? 0 : i4_w_col_not_empty;
            }
            break;
        }
        
    }
    else if ( HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_AUTO) && 
              HAS_FLAG (ui2_col_width, WGL_LB_COL_WIDTH_STL_COMPACT))
    {
        INT32               i4_w_col_max, i4_w_data, i4_w_total;
        BOOL                b_add_inset;
        const WGL_INSET_T*  pt_inset_col = &at_cnt_cols[ui1_c].t_inset;
        
        b_add_inset  = FALSE;
        i4_w_col_max = (INT32) WGL_LB_GET_COL_WIDTH (ui2_col_width);
        i4_w_data    = 0;
        
        switch (at_cols[ui1_c].e_col_type)
        {
            case LB_COL_TYPE_TEXT:
            {
                if ((pw2s_text == NULL) || (pw2s_text[0] == ((UTF16_T) 0)))
                {
                    i4_w_data = 0;
                }
                else
                {
                    HFONT_T  h_font;
                    INT32    i4_w_text, i4_h_text;
                    
                    _lbcntdef_get_item_attr(pt_cnt_elem, at_cnt_cols, ui1_c, ITEM_ATTR_FONT, (VOID *)&h_font);
                    
                    i4_ret = x_fe_get_string_size (h_font, pw2s_text, -1, &i4_w_text, &i4_h_text);
                    
                    if (i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE)
                    {
                        i4_w_data   = i4_w_text;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR(i4_ret);
                        i4_w_data = i4_w_col_max;
                    }
                }
            }
            break;
            
            case LB_COL_TYPE_IMG:
            {
                if (pt_elem->at_items[ui1_c].h_img != NULL_HANDLE)
                {
                    UINT32  ui4_w_img, ui4_h_img;
                    
                    i4_ret = x_wgl_img_tpl_get_size (pt_elem->at_items[ui1_c].h_img,
                                                     &ui4_w_img,
                                                     &ui4_h_img);
                                                     
                    if (i4_ret == WGLR_OK)
                    {
                        i4_w_data = (INT32) ui4_w_img;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR (i4_ret);
                        i4_w_data = 0;
                    }
                }
                else
                {
                    i4_w_data = 0;
                }
            }
            break;
            
            case LB_COL_TYPE_IMG_SUIT:
            {
                WGL_HIMG_TPL_T      h_img;
                
                _lbcntdef_select_item_img_by_state (
                        pt_elem->at_items[ui1_c].pt_img_suit,
                        ui2_state_paint,
                        LB_GET_STATE_BY_ELEM(pt_elem),
                        & h_img);
                
                if (h_img != NULL_HANDLE)
                {
                    UINT32  ui4_w_img, ui4_h_img;
                    
                    i4_ret = x_wgl_img_tpl_get_size (h_img,
                                                     &ui4_w_img,
                                                     &ui4_h_img);
                                                     
                    if (i4_ret == WGLR_OK)
                    {
                        i4_w_data = (INT32) ui4_w_img;
                        b_add_inset = TRUE;
                    }
                    else
                    {
                        INFORM_ERR (i4_ret);
                        i4_w_data = 0;
                    }
                }
                else
                {
                    i4_w_data = 0;
                }
            }
            break;
        }
        
        i4_w_total = i4_w_data;
        
        if (b_add_inset)
        {
            i4_w_total += (pt_inset_col->i4_left + pt_inset_col->i4_right);
        }
        
        i4_w_col_eff = MIN (i4_w_total, i4_w_col_max);
    }
    else
    {
        INFORM_ERR (WGLR_INTERNAL_ERROR);
    }
    
    return i4_w_col_eff;
}

static INT32 _lbcntdef_render_col_data_ex(
    IN      LBCNTDEF_CTX_T              *pt_ctx, 
    IN OUT  GL_HGRAPHICS_T              h_g, 
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt,
    IN      UINT16                      ui2_state_paint, 
    IN      LB_ELEM_T                   *pt_elem, 
    IN      LBCNTDEF_ELEM_T             *pt_cnt_elem, 
    IN      UINT8                       ui1_c,
    IN      GL_RECT_T                   *pt_rc_scrn_col_cnt_upt,
    IN     WGL_LB_ITEM_DATA_T*          pt_item_data)    
{
    INT32       i4_ret;

    switch(pt_ctx->at_cols[ui1_c].e_col_type)
    {
        case LB_COL_TYPE_TEXT:
            if(HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT) && 
               (pt_item_data == NULL || pt_item_data->pw2s_text == NULL))
            {
                return WGLR_OK;
            }
            INFORM_ERR(_lbcntdef_render_col_data_text(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                ui2_state_paint,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt,
                pt_item_data->pw2s_text));
        break;    
        
        case LB_COL_TYPE_IMG:
            INFORM_ERR(_lbcntdef_render_col_data_img(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt));
        break;    

        case LB_COL_TYPE_IMG_SUIT:
            INFORM_ERR(_lbcntdef_render_col_data_img_suit(
                pt_ctx,
                h_g,
                pt_rc_scrn_col_cnt,
                ui2_state_paint,
                pt_elem,
                pt_cnt_elem,
                ui1_c,
                pt_rc_scrn_col_cnt_upt));
        break;    
        
        default:
            CATCH_FAIL(WGLR_INTERNAL_ERROR);
    }
    
    return WGLR_OK;
}

static INT32 _lbcntdef_init_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx,
    UINT16                          ui2_max_elem,
    UINT16                          ui2_text_max_len)
{
    WGL_LB_ELEM_DATA_T*             pt_elem_buf = NULL;
    WGL_LB_ITEM_DATA_T*             pt_item_buf = NULL;
    UINT16                          ui2_elem;
    UINT8                           ui1_col;
    UINT32                          ui4_size;
    UINT32                          ui4_text_size;
    UTF16_T*                        p2w_buf = NULL;
    
    x_memset(&pt_ctx->t_elem_inf, 0, sizeof(WGL_LB_ELEM_DATA_INFO_T));
    pt_ctx->ui2_elem_alloc = 0;
    pt_ctx->ui1_col_alloc = 0;
    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        return WGLR_OK;
    }
    
    ui4_size = sizeof(WGL_LB_ELEM_DATA_T)*ui2_max_elem;
    pt_elem_buf = (WGL_LB_ELEM_DATA_T*)WGL_MEM_ALLOC(ui4_size);
    if(pt_elem_buf == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL     
    pt_ctx->ui4_mem_size += ui4_size;
#endif  
    x_memset(pt_elem_buf,0,ui4_size);
    pt_ctx->t_elem_inf.pt_elem_data = pt_elem_buf;
    pt_ctx->ui2_elem_alloc = ui2_max_elem;
    
    ui4_size = sizeof(WGL_LB_ITEM_DATA_T)*pt_ctx->ui1_col_num;
    ui4_text_size = WGL_ALIGN_4(sizeof(UTF16_T)*ui2_text_max_len);
    /*Alloc element data buffer*/
    for(ui2_elem = 0; ui2_elem < ui2_max_elem; ui2_elem ++)
    {
        pt_item_buf = (WGL_LB_ITEM_DATA_T*)WGL_MEM_ALLOC(ui4_size);
        #ifdef CLI_LVL_ALL     
        pt_ctx->ui4_mem_size += ui4_size;
        #endif  
        if(pt_item_buf == NULL)
        {
            return WGLR_OUT_OF_MEMORY;
        }
        x_memset(pt_item_buf, 0, ui4_size);
        pt_elem_buf->at_items = pt_item_buf;

        /*Alloc column buffer*/
        for(ui1_col = 0; ui1_col < pt_ctx->ui1_col_num; ui1_col ++)
        {
            if(pt_item_buf == NULL)
            {
                break;
            }

            if(IS_COL_TEXT(pt_ctx, ui1_col))
            {
                p2w_buf = (UTF16_T*)WGL_MEM_ALLOC(ui4_text_size);
                #ifdef CLI_LVL_ALL     
                pt_ctx->ui4_mem_size += ui4_text_size;
                #endif  
                if(p2w_buf == NULL)
                {
                    return WGLR_OUT_OF_MEMORY;   
                }
                x_memset(p2w_buf, 0, ui4_text_size);
                pt_item_buf->pw2s_text = p2w_buf;
            }

            pt_item_buf ++;
        }
        
        pt_elem_buf ++;
    }
    pt_ctx->ui1_col_alloc = pt_ctx->ui1_col_num;
    
    return WGLR_OK;
}

static INT32 _lbcntdef_deinit_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx)
{
    WGL_LB_ELEM_DATA_T*             pt_elem_buf = NULL;
    WGL_LB_ITEM_DATA_T*             pt_item_buf = NULL;
    UINT16                          ui2_elem;
    UINT8                           ui1_col;
#ifdef CLI_LVL_ALL    
    UINT32                          ui4_size;
    UINT32                          ui4_text_size;
#endif

    if(pt_ctx == NULL)
    {
        return WGLR_INV_ARG;
    }

    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        return WGLR_OK;
    }
    
    pt_elem_buf = pt_ctx->t_elem_inf.pt_elem_data;
    
#ifdef CLI_LVL_ALL    
    ui4_size = sizeof(WGL_LB_ITEM_DATA_T) * pt_ctx->ui1_col_num;
    ui4_text_size = WGL_ALIGN_4( sizeof(UTF16_T) * pt_ctx->ui2_max_text_len );
#endif
    for(ui2_elem = 0; ui2_elem < pt_ctx->ui2_elem_num; ui2_elem ++,pt_elem_buf++)
    {
        if(pt_elem_buf->at_items == NULL)
        {
            continue;
        }

        pt_item_buf = pt_elem_buf->at_items;
        for(ui1_col = 0; ui1_col < pt_ctx->ui1_col_num; ui1_col ++)
        {
            if(pt_item_buf->pw2s_text != NULL)   
            {
                WGL_MEM_FREE(pt_item_buf->pw2s_text);
                #ifdef CLI_LVL_ALL     
                pt_ctx->ui4_mem_size -= ui4_text_size ;
                #endif  
            }
            pt_item_buf ++;
        }
        WGL_MEM_FREE(pt_elem_buf->at_items);
        #ifdef CLI_LVL_ALL     
        pt_ctx->ui4_mem_size -= ui4_size;
        #endif  
    }
    WGL_MEM_FREE(pt_ctx->t_elem_inf.pt_elem_data);

#ifdef CLI_LVL_ALL    
    ui4_size = sizeof(WGL_LB_ELEM_DATA_T)*pt_ctx->ui2_elem_num;
    pt_ctx->ui4_mem_size -= ui4_size;
#endif

    pt_ctx->t_elem_inf.ui2_elem_num = 0;
    pt_ctx->t_elem_inf.ui1_col_num = 0;
    pt_ctx->t_elem_inf.pt_elem_data = 0;
    pt_ctx->ui2_elem_alloc = 0;
    pt_ctx->ui1_col_alloc = 0;

    return WGLR_OK;
}

static INT32 _lbcntdef_clear_elem_buf(
    LBCNTDEF_CTX_T                  *pt_ctx)
{
    WGL_LB_ELEM_DATA_T*             pt_elem_buf = NULL;
    WGL_LB_ITEM_DATA_T*             pt_item_buf = NULL;
    UINT16                          ui2_elem;
    UINT8                           ui1_col;

    if(pt_ctx == NULL)
    {
        return WGLR_INV_ARG;
    }
    if(!HAS_FLAG(pt_ctx->ui4_style,WGL_STL_LB_NO_STORE_TEXT))
    {
        return WGLR_OK;
    }
    
    pt_elem_buf = pt_ctx->t_elem_inf.pt_elem_data;
    
    for(ui2_elem = 0; ui2_elem < pt_ctx->ui2_elem_num; ui2_elem ++,pt_elem_buf++)
    {
        if(pt_elem_buf->at_items == NULL)
        {
            continue;
        }

        pt_item_buf = pt_elem_buf->at_items;
        for(ui1_col = 0; ui1_col < pt_ctx->ui1_col_num; ui1_col ++)
        {
            if(pt_item_buf->pw2s_text != NULL)   
            {
                x_memset(pt_item_buf->pw2s_text,0,sizeof(pt_ctx->ui2_max_text_len));
            }
            pt_item_buf ++;
        }
    }

    pt_ctx->t_elem_inf.ui2_elem_num = 0;
    pt_ctx->t_elem_inf.ui1_col_num = 0;
    return WGLR_OK;
}

static INT32 _lbcntdef_no_store_get_data(
    LBCNTDEF_CTX_T                  *pt_ctx,
    WGL_LB_ELEM_DATA_INFO_T         *pt_data_inf)
{
    BOOL                            b_mw_thread;
    
    if(pt_ctx == NULL || pt_data_inf == NULL)
    {
        return WGLR_INV_ARG;
    }
    
    x_memset(pt_data_inf,0,sizeof(WGL_LB_ELEM_DATA_INFO_T));
    b_mw_thread = wgl_cur_thread_is_mw_thread();
    if(b_mw_thread == TRUE)
    {
        pt_data_inf->ui2_elem_num = pt_ctx->ui2_elem_num;
        pt_data_inf->ui1_col_num = pt_ctx->ui1_col_num;
        pt_data_inf->pt_elem_data = pt_ctx->t_elem_inf.pt_elem_data;
    }

    if((_lbcntdef_check_text_type_col(pt_ctx) == TRUE) && (pt_ctx->pf_lb_cb_fct != NULL))
    {
        _lbcntdef_do_cb_fct(pt_ctx,WGL_LB_NFY_REQ_ELEM_TEXT_DATA,NULL,pt_data_inf);  
    }

    if(b_mw_thread == TRUE)
    {
        pt_ctx->t_elem_inf.ui2_elem_num = pt_data_inf->ui2_elem_num;
        pt_ctx->t_elem_inf.ui1_col_num = pt_data_inf->ui1_col_num;
    }
    return WGLR_OK;
}
#endif

static VOID _lbcntdef_get_elem_flag(
    UINT16                           ui2_elem_idx,
    UINT16                           ui2_vp_last,
    UINT16                           ui2_elem_num,
    UINT32                           *pui4_flag)
{
    UINT16                           ui2_last = 0;

    if(pui4_flag == NULL)
    {
        return ;
    }
    *pui4_flag  = 0;

    if( ui2_elem_idx == ui2_vp_last )
    {
        WGL_SET_FLAG(*pui4_flag, WGL_LB_CNT_ELEM_IS_LAST_OF_VP);
    }
    
    if(ui2_elem_num > 0)
    {
        ui2_last = (UINT16)(ui2_elem_num - 1);
    }
    if(ui2_elem_idx == ui2_last)
    {
        WGL_SET_FLAG(*pui4_flag, WGL_LB_CNT_ELEM_IS_LAST_ELEM);
    }
}




