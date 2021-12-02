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
 * $RCSfile: wgl_progress_cnt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: e85a0206805591aaa79fa1dc6b3f3326 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/def_dbg_level_mw.h"
#include "handle/x_handle.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
#include "unicode/x_uc_str.h"
#include "font/x_fe.h"
#include "wgl/progress/wgl_progress_cnt.h"
#include "wgl/progress/wgl_progress_cli.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_pg_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif


/*Extra Flag*/
#define PG_BK_CLR_NOT_INIT              ((UINT64)0x0000000000000001)
#define PG_BK_IMG_NOT_INIT              ((UINT64)0x0000000000000002)
#define PG_FONT_NOT_INIT                ((UINT64)0x0000000000000004)
#define PG_TEXT_CLR_NOT_INIT            ((UINT64)0x0000000000000008)
#define PG_BODY_CLR_NOT_INIT            ((UINT64)0x0000000000000010)
#define PG_LEVEL1_CLR_NOT_INIT          ((UINT64)0x0000000000000020)
#define PG_LEVEL2_CLR_NOT_INIT          ((UINT64)0x0000000000000040)
#define PG_LEVEL3_CLR_NOT_INIT          ((UINT64)0x0000000000000080)
#define PG_LEVEL4_CLR_NOT_INIT          ((UINT64)0x0000000000000100)
#define PG_LEVEL5_CLR_NOT_INIT          ((UINT64)0x0000000000000200)
#define PG_BODY_IMG_NOT_INIT            ((UINT64)0x0000000000000400)
#define PG_LEVLE1_IMG_NOT_INIT          ((UINT64)0x0000000000000800)
#define PG_LEVLE2_IMG_NOT_INIT          ((UINT64)0x0000000000001000)
#define PG_LEVLE3_IMG_NOT_INIT          ((UINT64)0x0000000000002000)
#define PG_LEVLE4_IMG_NOT_INIT          ((UINT64)0x0000000000004000)
#define PG_LEVLE5_IMG_NOT_INIT          ((UINT64)0x0000000000008000)
#define PG_IMG_UI                       ((UINT64)0x0000000000010000)
#define PG_LAYOUT_VERTICAL_UP           ((UINT64)0x0000000000020000)
#define PG_LAYOUT_VERTICAL_DOWN         ((UINT64)0x0000000000040000)
#define PG_LAYOUT_BLOCK                 ((UINT64)0x0000000000080000)
#define PG_LAYOUT_POINT                 ((UINT64)0x0000000000100000)
#define PG_DIRTY_RANGE                  ((UINT64)0x0000000000200000)
#define PG_DIRTY_POS                    ((UINT64)0x0000000000400000)
#define PG_BAR_CURSOR                   ((UINT64)0x0000000000800000)
#define PG_NO_BK                        ((UINT64)0x0000000001000000)
#define PG_USER_DEF_MAX_LOC             ((UINT64)0x0000000002000000)
#define PG_USER_DEF_MIN_LOC             ((UINT64)0x0000000004000000)
#define PG_USER_DEF_POS_LOC             ((UINT64)0x0000000008000000)
#define PG_USE_SYS_IMG                  ((UINT64)0x0000000010000000)
#define PG_NOT_CLEAN_BK                 ((UINT64)0x0000000020000000)
#define PG_BAR_IMAGE_W_H_NOT_INIT       ((UINT64)0x0000000040000000)
#define PG_BAR_CURSOR_CLR_NOT_INIT      ((UINT64)0x0000000080000000)     
#define PG_BAR_CURSOR_IMG_NOT_INIT      ((UINT64)0x0000000100000000)
#define PG_BAR_FRAME_NOT_INIT           ((UINT64)0x0000000200000000)
#define PG_DEFAULT_BLOCK_GAP            ((UINT8)0x03)
#define PG_IDR_MAX_CHAR_LENGTH          ((UINT8)15)

/*helper marco*/
#define _PG_IS_VERTICAL_UP(ui8_flag)    ((ui8_flag) & PG_LAYOUT_VERTICAL_UP)
#define _PG_IS_VERTICAL_DOWN(ui8_flag)  ((ui8_flag) & PG_LAYOUT_VERTICAL_DOWN)
#define _PG_IS_HORIZTONAL(ui8_flag)     (!_PG_IS_VERTICAL_UP(ui8_flag) && !_PG_IS_VERTICAL_DOWN(ui8_flag))
#define _PG_IS_BLOCK(ui8_flag)          ((ui8_flag) & PG_LAYOUT_BLOCK)
#define _PG_IS_POINT(ui8_flag)          ((ui8_flag) & PG_LAYOUT_POINT)
#define _PG_IS_SMOOTH(ui8_flag)         (!(_PG_IS_BLOCK(ui8_flag) && _PG_IS_POINT(ui8_flag)))
#define _PG_IS_RANGE_DIRTY(ui8_flag)    ((ui8_flag) & PG_DIRTY_RANGE)
#define _PG_IS_POS_DIRTY(ui8_flag)      ((ui8_flag) & PG_DIRTY_POS)
#define _PG_IS_SHOW_IDR(ui1_flag)       (!((ui1_flag) & WGL_PG_DM_HIDE_IDR))
#define _PG_IS_SHOW_MIN(ui1_flag)       ((ui1_flag) & WGL_PG_DM_SHOW_MIN)
#define _PG_IS_SHOW_MAX(ui1_flag)       ((ui1_flag) & WGL_PG_DM_SHOW_MAX)
#define _PG_IS_PERCENT(ui1_flag)        ((ui1_flag) & WGL_PG_DM_PERCENT)
#define _PG_IS_HIDE_BAR(ui1_flag)       ((ui1_flag) & WGL_PG_DM_HIDE_BAR)
#define _PG_IS_CURSOR(ui8_flag)         ((ui8_flag) & PG_BAR_CURSOR)
#define PROGRESS_CHECK_ID(id)        (((id) < WGL_PG_LEVEL_MAX)? (id): 0)


/*helper marco for dirty flag*/
#define _PG_TURN_ON_POS_DIRTY(pt_pg)                                \
    {                                                               \
        WGL_SET_FLAG((pt_pg->ui8_extra_flag), PG_DIRTY_POS);        \
    }

#define _PG_TURN_OFF_POS_DIRTY(pt_pg)                               \
    {                                                               \
        WGL_CLEAR_FLAG((pt_pg->ui8_extra_flag), PG_DIRTY_POS);      \
    }

#define _PG_TURN_ON_RANGE_DIRTY(pt_pg)                              \
    {                                                               \
        WGL_SET_FLAG((pt_pg->ui8_extra_flag), PG_DIRTY_RANGE);      \
    }

#define _PG_TURN_OFF_RANGE_DIRTY(pt_pg)                             \
    {                                                               \
        WGL_CLEAR_FLAG((pt_pg->ui8_extra_flag), PG_DIRTY_RANGE);    \
    }

typedef struct _WGL_PG_CONTENT_T
{
#ifdef CLI_LVL_ALL     
    UINT32                         ui4_mem_size;                    /*this field must be the first field in the structure*/
#endif  
    UINT16                          ui2_state;                      /*state of the progress bar widget*/
    WGL_COLOR_STD_SET_T             t_clr_bk;                       /*color set for background*/
    WGL_IMG_STD_SET_T               t_img_bk;                       /*template image for background*/
    WGL_COLOR_STD_SET_T             t_clr_body;                     /*color set for body*/
    WGL_IMG_STD_SET_T               t_img_body;                     /*template image for body*/
    WGL_COLOR_STD_SET_T             t_clr_text;                     /*text color set*/
    GL_SIZE_T                       t_size_cursor;
    WGL_COLOR_STD_SET_T             t_clr_cursor;
    WGL_IMG_STD_SET_T               t_img_cursor;
    WGL_INSET_T                     t_cursor_inset;                  /*inset for out boder*/
    WGL_COLOR_STD_SET_T             t_cursor_inset_clr;
    GL_RECT_T                       t_cnt_rect;                     /*content rect*/
    WGL_INSET_T                     t_body_inset;                   /*inset for progress body*/
    WGL_INSET_T                     t_bar_inset;                    /*inset for bar*/
    WGL_FONT_INFO_T                 t_font_info;                    /*font info for text*/
    HFONT_T                         t_text_font;                    /*font for text*/
    WGL_FONT_INFO_T                 t_focus_font_info;              /*font information for text*/
    HFONT_T                         t_text_focus_font;              /*font for text*/
    WGL_COLOR_STD_SET_T             a_bar_clr[WGL_PG_LEVEL_MAX];
    WGL_IMG_STD_SET_T               a_bar_img[WGL_PG_LEVEL_MAX];
    WGL_PG_IDR_LOC_T                t_max_loc;
    WGL_PG_IDR_LOC_T                t_min_loc;
    WGL_PG_IDR_LOC_T                t_pos_loc;
    INT32                           a_boundary[WGL_PG_LEVEL_MAX - 1];
    UTF16_T                         a_idr_buf[PG_IDR_MAX_CHAR_LENGTH];
    UTF16_T                         a_min_buf[PG_IDR_MAX_CHAR_LENGTH];
    UTF16_T                         a_max_buf[PG_IDR_MAX_CHAR_LENGTH];
    INT32                           i4_cur_pos;
    INT32                           i4_min;
    INT32                           i4_max;
    UINT64                          ui8_extra_flag;                 /*Flag*/
    /*UINT16                        ui2_font_height;*/              /*height of a line*/
    UINT16                          ui2_min_width;                  /*width for min indicator*/
    UINT16                          ui2_max_width;                  /*width for max indicator*/
    UINT16                          ui2_idr_width;                  /*width for idr indicator*/
    UINT16                          ui2_block_width;
    UINT16                          ui2_block_height;
    UINT16                          ui2_bar_min_size;               /*user specify the min bar size*/
    UINT16                          ui2_bar_img_width;              /*width for bar image*/
    UINT16                          ui2_bar_img_height;             /*height for bar image*/
    UINT8                           ui1_block_gap;                  /*gap between bar block for block style*/
    UINT8                           ui1_step_size;
    UINT8                           ui1_levels;
    UINT8                           ui1_min_length;                 /*character length for min indicator*/
    UINT8                           ui1_max_length;                 /*character length for max indicator*/
    UINT8                           ui1_idr_length;                 /*character length for idr indicator*/
    UINT8                           ui1_dis_mode;
    WGL_CANVAS_STATUS_T             t_cvsst;            
} WGL_PG_CONTENT_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_pg_cnt_paint_img_bk(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state);

static INT32 _wgl_pg_cnt_paint_clr_bk(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT16                          ui2_state);


static INT32 _wgl_pg_cnt_paint_fg(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state);

static INT32 _wgl_pg_cnt_paint_fg_body(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state,
    GL_RECT_T*                      pt_available_rect);

static INT32 _wgl_pg_cnt_paint_fg_bar(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state,
    GL_RECT_T*                      pt_available_rect);

static INT32 _wgl_pg_cnt_paint_fg_img_body(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state,
    const GL_RECT_T*                pt_body_rect);

static INT32 _wgl_pg_cnt_paint_fg_clr_body(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT16                          ui2_state,
    const GL_RECT_T*                pt_body_rect);

static INT32 _wgl_pg_cnt_paint_fg_bar_smooth(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX]);

static INT32 _wgl_pg_cnt_paint_fg_bar_block(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX]);

static INT32 _wgl_pg_cnt_paint_fg_bar_point(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX]);

static INT32 _wgl_pg_cnt_paint_fg_bar_cursor(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX],
    UINT16                          ui2_state);


static INT32 _wgl_pg_cnt_paint_indicator(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT16                          ui2_state,
    const GL_RECT_T*                pt_available_rect);

static INT32 _wgl_pg_cnt_set_img(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_IMG_INFO_T*           pt_img_info);

static INT32 _wgl_pg_cnt_get_img(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target,
    WGL_IMG_INFO_T*                 pt_img_info);

static INT32 _wgl_pg_cnt_set_color(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_COLOR_INFO_T*         pt_clr_info);

static INT32 _wgl_pg_cnt_get_color(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target,
    WGL_COLOR_INFO_T*               pt_clr_info);

static INT32 _wgl_pg_cnt_set_font(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const WGL_FONT_INFO_T*          pt_font_info);

static INT32 _wgl_pg_cnt_get_font(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_FONT_INFO_T*                pt_font_info);

static INT32 _wgl_pg_cnt_set_font_ex(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const WGL_FONT_INFO_EX_T*       pt_font_info);

static INT32 _wgl_pg_cnt_get_font_ex(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_FONT_INFO_EX_T*             pt_font_info);

static INT32 _wgl_pg_cnt_set_levels(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const WGL_PG_LEVELS_INFO_T*     pt_infos);

static INT32 _wgl_pg_cnt_get_levels(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_PG_LEVELS_INFO_T*           pt_infos);

static INT32 _wgl_pg_cnt_set_bar_inset(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const WGL_INSET_T*              pt_bar_inset);

static INT32 _wgl_pg_cnt_cal_cursor_pos(   
    WGL_PG_CONTENT_T*               pt_cnt_data,
    GL_RECT_T*                      pt_up_bar_rect,
    INT32*                          pi4_x_offset,
    INT32*                          pi4_y_offset);
#if 0

static INT32 _wgl_pg_cnt_get_cursor_w_h(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const GL_SIZE_T*                pt_cursor_size);
#endif
static INT32 _wgl_pg_cnt_calc_update_rect(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_PG_CNT_CALC_UPDATE_INFO_T*  pt_update_info);

static INT32 _wgl_pg_cnt_get_indicator_rect(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    GL_RECT_T*                      pt_indicator_rect,
    BOOL*                           pb_is_show);
    
static INT32 _wgl_pg_cnt_set_indicator_loc(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_PG_CNT_IDR_LOC_TYPE_T       e_idr_type,
    WGL_PG_IDR_LOC_T*               pt_idr_loc);
    
static INT32 _wgl_pg_cnt_get_indicator_loc(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_PG_CNT_IDR_LOC_TYPE_T       e_idr_type,
    WGL_PG_IDR_LOC_T*               pt_idr_loc);            

static VOID _wgl_pg_cnt_update_range(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    INT32                           i4_min,
    INT32                           i4_max);

static INT32 _wgl_pg_cnt_prepare_img(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target);

static INT32 _wgl_pg_cnt_use_sys_color(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target);

static INT32 _wgl_pg_cnt_calc_indicator(
    INT32                           i4_value,
    INT32                           i4_min,
    INT32                           i4_max,
    BOOL                            b_percent,
    HFONT_T                         h_font,
    UTF16_T                         a_utf16_buf[PG_IDR_MAX_CHAR_LENGTH],
    UINT8*                          pui1_length,
    UINT16*                         pui2_width,
    UINT16*                         pui2_height);

static INT32 _wgl_pg_cnt_ready_bar_img(
    WGL_PG_CONTENT_T*               pt_data,
    UINT16                          ui2_state,
    WGL_HIMG_TPL_T                  ah_img_levels[WGL_PG_LEVEL_MAX]);

static INT32 _wgl_pg_cnt_ready_bar_clr(
    WGL_PG_CONTENT_T*               pt_data,
    UINT16                          ui2_state,
    GL_COLOR_T*                     apt_clr_levels[WGL_PG_LEVEL_MAX]);

    
static VOID _wgl_pg_cnt_offset_idr_rect(
    const WGL_PG_IDR_LOC_T*         pt_idr_loc,
    INT32                           i4_idr_width,
    INT32                           i4_idr_height,
    GL_RECT_T*                      pt_idr_rect);

static INT32 _wgl_pg_cnt_select_text_font(
    WGL_PG_CONTENT_T*               pt_cnt_data, 
    UINT16                          ui2_state,
    HANDLE_T*                       pt_selected_font,
    WGL_FONT_INFO_T*                pt_selected_font_info);

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 *              content create function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 wgl_pg_cnt_inst_create( 
    WGL_CONTENT_CONTEXT_HANDLE_T*       ph_cnt_data, 
    WGL_CONTENT_CREATE_INPUT_T*         pt_param)
{
    /*local variable declaration*/
    WGL_PG_CONTENT_T*                   pt_data;
    UINT64                              ui8_init_flag;

    DBG_API(("{WGL(PG_CNT)} %s: ph_cnt_data = 0x%.8x \tpt_param = 0x%.8x\n", __FUNCTION__, ph_cnt_data, pt_param));
    
    /*check parameter*/
    if(NULL == ph_cnt_data || NULL == pt_param)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(PG_CNT)} pt_param->pv_init = 0x%.8x   ui4_type_border = 0x%.8x\n"
                             "\tt_rect_cnt(l, t, r, b) = (%d, %d, %d, %d)\n",
                             pt_param->pv_init,
                             pt_param->t_rect_cnt.i4_left,
                             pt_param->t_rect_cnt.i4_top,
                             pt_param->t_rect_cnt.i4_right,
                             pt_param->t_rect_cnt.i4_bottom));

    /*allocate memory for content data*/
    pt_data = (WGL_PG_CONTENT_T*) WGL_MEM_ALLOC(sizeof(WGL_PG_CONTENT_T));
    if(NULL == pt_data)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to alloc memory for content instance. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    /*initialize content data*/
    x_memset(pt_data, 0, sizeof(WGL_PG_CONTENT_T));
    
#ifdef CLI_LVL_ALL
    pt_data->ui4_mem_size = sizeof(WGL_PG_CONTENT_T);
#endif
    pt_data->ui8_extra_flag = PG_BK_CLR_NOT_INIT 
                            | PG_BK_IMG_NOT_INIT 
                            | PG_FONT_NOT_INIT 
                            | PG_TEXT_CLR_NOT_INIT
                            | PG_LEVEL1_CLR_NOT_INIT 
                            | PG_LEVEL2_CLR_NOT_INIT 
                            | PG_LEVEL3_CLR_NOT_INIT
                            | PG_LEVEL4_CLR_NOT_INIT 
                            | PG_LEVEL5_CLR_NOT_INIT 
                            | PG_LEVLE1_IMG_NOT_INIT
                            | PG_LEVLE2_IMG_NOT_INIT 
                            | PG_LEVLE3_IMG_NOT_INIT 
                            | PG_LEVLE4_IMG_NOT_INIT
                            | PG_LEVLE5_IMG_NOT_INIT 
                            | PG_BODY_CLR_NOT_INIT 
                            | PG_BODY_IMG_NOT_INIT
                            | PG_BAR_IMAGE_W_H_NOT_INIT
                            | PG_BAR_CURSOR_CLR_NOT_INIT
                            | PG_BAR_CURSOR_IMG_NOT_INIT;

    _PG_TURN_ON_RANGE_DIRTY(pt_data);
    _PG_TURN_ON_POS_DIRTY(pt_data);

    /*default setting*/
    pt_data->ui2_state = WGL_STATE_ENABLED;
    pt_data->ui1_levels = 1;
    pt_data->i4_min = 0;
    pt_data->i4_max = 100;
    pt_data->i4_cur_pos = 0;
    pt_data->ui1_dis_mode = WGL_PG_DM_PERCENT;
    pt_data->ui1_block_gap = PG_DEFAULT_BLOCK_GAP;
    pt_data->ui1_step_size = 0;
    wgl_cvsst_copy(&pt_data->t_cvsst, pt_param->pt_cvsst);
    
    /*store settings*/
    ui8_init_flag = (UINT32)pt_param->pv_init;
    WGL_RECT_COPY(&pt_data->t_cnt_rect, &pt_param->t_rect_cnt);
    pt_data->ui8_extra_flag |= (WGL_IS_IMAGE_UI(ui8_init_flag)) ? PG_IMG_UI : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_PG_VERTICAL_UP) ? PG_LAYOUT_VERTICAL_UP : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_PG_VERTICAL_DOWN) ? PG_LAYOUT_VERTICAL_DOWN : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_PG_BLOCK) ? PG_LAYOUT_BLOCK : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_PG_POINT) ? PG_LAYOUT_POINT : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_PG_CURSOR) ? PG_BAR_CURSOR : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_GL_NO_BK) ? PG_NO_BK : 0;
    pt_data->ui8_extra_flag |= (ui8_init_flag & WGL_STL_GL_USE_SYS_IMG) ? PG_USE_SYS_IMG : 0;
    pt_data->ui8_extra_flag |= (WGL_STL_GL_BDR_FILL_CNT_BK & ui8_init_flag) ? PG_NOT_CLEAN_BK : 0;
    
    /*prepare return value*/
    *ph_cnt_data = (WGL_CONTENT_CONTEXT_HANDLE_T)pt_data;
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content destroy function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_pg_cnt_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data)
{
    /*check parameter*/
    WGL_PG_CONTENT_T*                   pt_data;
    INT32                               i4_ret;
    
    DBG_API(("{WGL(PG_CNT)} %s: h_cnt_data = 0x%.8x\n", __FUNCTION__, h_cnt_data));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    pt_data = (WGL_PG_CONTENT_T*)h_cnt_data;    
    
    /*free font*/
    if(pt_data->t_text_font)
    {
        i4_ret = WGL_FONT_FREE(pt_data->t_text_font);
        pt_data->t_text_font = NULL_HANDLE;
        WGL_ASSERT(0 == i4_ret);
    }
    /*free focus font*/
    if(pt_data->t_text_focus_font)
    {
        i4_ret = WGL_FONT_FREE(pt_data->t_text_focus_font);
        pt_data->t_text_focus_font = NULL_HANDLE;
        WGL_ASSERT(0 == i4_ret);
    }
    /*free control logic object's resource*/
    WGL_MEM_FREE(pt_data);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content paint function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_pg_cnt_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    DBG_API(("{WGL(PG_CNT)} %s: h_cnt_data = 0x%.8x \th_graphics = 0x%.8x \tui2_state = %d\n", __FUNCTION__, h_cnt_data, h_graphics, ui2_state));

    /*check parameter*/
    if(NULL == h_cnt_data || NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*paint background*/
    if(PG_NO_BK & ((WGL_PG_CONTENT_T*)h_cnt_data)->ui8_extra_flag)
    {
        i4_ret = WGLR_OK;
    }
    else
    {
        /*image style*/
        if(PG_IMG_UI & ((WGL_PG_CONTENT_T*)h_cnt_data)->ui8_extra_flag)
        {
            i4_ret = _wgl_pg_cnt_paint_img_bk((WGL_PG_CONTENT_T*)h_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
        }
        else /*color style*/
        {
            i4_ret = _wgl_pg_cnt_paint_clr_bk((WGL_PG_CONTENT_T*)h_cnt_data, h_graphics, ui2_state);
        }
    }
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*paint foreground*/
    return _wgl_pg_cnt_paint_fg((WGL_PG_CONTENT_T*)h_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);  
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content command procedure type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                          
INT32 wgl_pg_cnt_command_proc(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data,
    UINT32                              ui4_cmd_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2)
{   
    /*local variable declaration*/
    WGL_PG_CONTENT_T*                   pt_pg;

    DBG_API(("{WGL(PG_CNT)} %s: h_cnt_data = 0x%.8x \tui4_cmd_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
                                __FUNCTION__, 
                                h_cnt_data, 
                                ui4_cmd_id,
                                pv_param1,
                                pv_param2));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if(!(IS_PROGRESS_CNT_CMD(ui4_cmd_id) || WGL_IS_CMD_GENERAL(ui4_cmd_id)))
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    pt_pg = (WGL_PG_CONTENT_T*)h_cnt_data;

    switch (ui4_cmd_id)
    {
    case WGL_CMD_GL_SET_IMAGE:
        return _wgl_pg_cnt_set_img(pt_pg, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_IMAGE:
        return _wgl_pg_cnt_get_img(pt_pg, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_COLOR:
        return _wgl_pg_cnt_set_color(pt_pg, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_COLOR:
        return _wgl_pg_cnt_get_color(pt_pg, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_FONT:
        return _wgl_pg_cnt_set_font(pt_pg, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT:
        return _wgl_pg_cnt_get_font(pt_pg, (WGL_FONT_INFO_T*)pv_param1);
        
    case WGL_CMD_GL_SET_FONT_EX:
        return _wgl_pg_cnt_set_font_ex(pt_pg, (WGL_FONT_INFO_EX_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT_EX:
        return _wgl_pg_cnt_get_font_ex(pt_pg, (WGL_FONT_INFO_EX_T*)pv_param1);

    case WGL_CMD_PG_SET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(PG_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((WGL_INSET_T*)pv_param1)->i4_left,
                        ((WGL_INSET_T*)pv_param1)->i4_top,
                        ((WGL_INSET_T*)pv_param1)->i4_right,
                        ((WGL_INSET_T*)pv_param1)->i4_bottom));

            if(FALSE == wgl_is_legal_inset(&pt_pg->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_pg->t_body_inset, (WGL_INSET_T*)pv_param1);
        }
        break;

    case WGL_CMD_PG_GET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_pg->t_body_inset);
        }
        break;
    case WGL_CMD_PG_SET_CURSOR_INSET:
         {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(PG_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((WGL_INSET_T*)pv_param1)->i4_left,
                        ((WGL_INSET_T*)pv_param1)->i4_top,
                        ((WGL_INSET_T*)pv_param1)->i4_right,
                        ((WGL_INSET_T*)pv_param1)->i4_bottom));

            if(FALSE == wgl_is_legal_inset(&pt_pg->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_pg->t_cursor_inset, (WGL_INSET_T*)pv_param1);

            pt_pg->ui8_extra_flag |= PG_BAR_FRAME_NOT_INIT;
         }
        break;
    case WGL_CMD_PG_GET_CURSOR_INSET:
         {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_pg->t_cursor_inset);
         }
        break;
        
    case WGL_CMD_PG_SET_DISPLAY_MODE:
        {
            if(pt_pg->ui1_dis_mode != WGL_POINTER_TO_UINT8(pv_param1))
            {
                _PG_TURN_ON_POS_DIRTY(pt_pg);
                _PG_TURN_ON_RANGE_DIRTY(pt_pg);
            }

            pt_pg->ui1_dis_mode = WGL_POINTER_TO_UINT8(pv_param1);
        }
        break;

    case WGL_CMD_PG_GET_DISPLAY_MODE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_DISPLAY_MODE, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT8*)pv_param1) = pt_pg->ui1_dis_mode;
        }
        break;

    case WGL_CMD_PG_SET_BLOCK_GAP:
        {
            pt_pg->ui1_block_gap = WGL_POINTER_TO_UINT8(pv_param1);
        }
        break;

    case WGL_CMD_PG_GET_BLOCK_GAP:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_BLOCK_GAP, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            *((UINT8*)pv_param1) = pt_pg->ui1_block_gap;
        }
        break;
        
    case WGL_CMD_PG_SET_STEP_SIZE:
        {
            pt_pg->ui1_step_size = WGL_POINTER_TO_UINT8(pv_param1);
        }
        break;

    case WGL_CMD_PG_GET_STEP_SIZE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_BLOCK_GAP, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            *((UINT8*)pv_param1) = pt_pg->ui1_step_size;
        }
        break;
    
    case WGL_CMD_PG_SET_BLK_PT_SIZE:
        {
            pt_pg->ui2_block_width = WGL_POINTER_TO_UINT16(pv_param1);
            pt_pg->ui2_block_height = WGL_POINTER_TO_UINT16(pv_param2);
        }
        break;

    case WGL_CMD_PG_GET_BLK_PT_SIZE:
        {
            if(NULL == pv_param1 || NULL == pv_param2)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_BLK_PT_SIZE, the both arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            *((UINT16*)pv_param1) = pt_pg->ui2_block_width;
            *((UINT16*)pv_param2) = pt_pg->ui2_block_height;
        }
        break;
    case WGL_CMD_PG_SET_CURSOR_W_H:
        {
            GL_SIZE_T* pt_cursor_size;
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_SET_CURSOR_W_H, the both arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            pt_cursor_size = (GL_SIZE_T*)pv_param1;
            
            pt_pg->t_size_cursor.ui4_width  = pt_cursor_size->ui4_width;
            pt_pg->t_size_cursor.ui4_height = pt_cursor_size->ui4_height;
        }
        break;
    case WGL_CMD_PG_GET_CURSOR_W_H:
        {
            GL_SIZE_T* pt_cursor_size;
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_CURSOR_W_H, the both arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            pt_cursor_size = (GL_SIZE_T*)pv_param1;
            pt_cursor_size->ui4_width  = pt_pg->t_size_cursor.ui4_width;
            pt_cursor_size->ui4_height = pt_pg->t_size_cursor.ui4_height;
        }
        break;
        
    case WGL_CMD_PG_SET_LEVELS:
        return _wgl_pg_cnt_set_levels(pt_pg, (WGL_PG_LEVELS_INFO_T*)pv_param1);

    case WGL_CMD_PG_GET_LEVELS:
        return _wgl_pg_cnt_get_levels(pt_pg, (WGL_PG_LEVELS_INFO_T*)pv_param1);

    case WGL_CMD_PG_SET_BAR_INSET:
        return _wgl_pg_cnt_set_bar_inset(pt_pg, (WGL_INSET_T*)pv_param1);

    case WGL_CMD_PG_GET_BAR_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_BAR_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_pg->t_bar_inset);
        }
        break;
        
    case WGL_CNT_PG_SET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_SET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(PG_CNT)} set_rect(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((GL_RECT_T*)pv_param1)->i4_left,
                        ((GL_RECT_T*)pv_param1)->i4_top,
                        ((GL_RECT_T*)pv_param1)->i4_right,
                        ((GL_RECT_T*)pv_param1)->i4_bottom));

            /*copy rect*/
            WGL_RECT_COPY(&pt_pg->t_cnt_rect, (GL_RECT_T*)pv_param1);
        }
        break;

    case WGL_CNT_PG_GET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_GET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*prepare return value*/
            WGL_RECT_COPY((GL_RECT_T*)pv_param1, &pt_pg->t_cnt_rect);
        }
        break;

    case WGL_CNT_PG_UPDATE_POS:
        {
            pt_pg->i4_cur_pos = (INT32)pv_param1;

            _PG_TURN_ON_POS_DIRTY(pt_pg);
        }
        break;

    case WGL_CNT_PG_UPDATE_RANGE:
        _wgl_pg_cnt_update_range(pt_pg, (INT32)pv_param1, (INT32)pv_param2);
        break;

    case WGL_CNT_PG_CALC_UPDATE_RECT:
        return _wgl_pg_cnt_calc_update_rect(pt_pg, (WGL_PG_CNT_CALC_UPDATE_INFO_T*)pv_param1);
        
    case WGL_CNT_PG_GET_INDICATOR_RECT:
        return _wgl_pg_cnt_get_indicator_rect(pt_pg, (GL_RECT_T*)pv_param1, (BOOL*)pv_param2);
        
    case WGL_CMD_PG_SET_IDR_LOC:
        return _wgl_pg_cnt_set_indicator_loc(pt_pg, (WGL_PG_CNT_IDR_LOC_TYPE_T)(INT32)pv_param1, (WGL_PG_IDR_LOC_T*)pv_param2);
        
    case WGL_CMD_PG_GET_IDR_LOC:
        return _wgl_pg_cnt_get_indicator_loc(pt_pg, (WGL_PG_CNT_IDR_LOC_TYPE_T)(INT32)pv_param1, (WGL_PG_IDR_LOC_T*)pv_param2);
        
    case WGL_CMD_PG_SET_BAR_MIN_SIZE:
        {
            pt_pg->ui2_bar_min_size = WGL_POINTER_TO_UINT16(pv_param1);
        }
        break;
        
    case WGL_CMD_PG_GET_BAR_MIN_SIZE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_PG_GET_BAR_MIN_SIZE, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            *((UINT16*)pv_param1) = pt_pg->ui2_bar_min_size;
        }
        break;
        
    case WGL_CNT_PG_UPDATE_CVSST:
        return wgl_update_cvvst((WGL_CVS_CLR_CFG_T*)pv_param1, &pt_pg->t_cvsst);
   /*Internal Content Commands*/
    case WGL_CNT_PG_SET_STATE:
    {        
        pt_pg->ui2_state = WGL_POINTER_TO_UINT16(pv_param1);
        /*update dirty flag*/
        _PG_TURN_ON_RANGE_DIRTY(pt_pg);
        _PG_TURN_ON_POS_DIRTY(pt_pg);
        break;
    }
    default:
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_img_bk(
    WGL_PG_CONTENT_T*                  pt_data, 
    GL_HGRAPHICS_T                     h_graphics,
    UINT32                             ui4_upt_rect,
    const GL_RECT_T*                   pt_upt_rect,
    UINT16                             ui2_state)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                     h_image;
    INT32                              i4_ret;

    /*check and cache images*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_BK_IMG_NOT_INIT))
    {
        i4_ret = _wgl_pg_cnt_prepare_img(pt_data, WGL_IMG_BK);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret; 
        }
        
        /*remove the flag for bk image*/
        WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, PG_BK_IMG_NOT_INIT);
    }


    /*select image*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        h_image = pt_data->t_img_bk.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_image = pt_data->t_img_bk.t_enable;
    }
    else
    {
        h_image = pt_data->t_img_bk.t_disable;
    }

    /*paint background*/
    if(NULL_HANDLE != h_image)
    {
        i4_ret = wgl_img_tpl_paint(h_image, 
                                   &pt_data->t_cnt_rect, 
                                   pt_upt_rect, 
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   (WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_NOT_CLEAN_BK)) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT);
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw image UI to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
    }
    else
    {
        /*init clean color*/
        i4_ret = WGLR_OK;
    }

    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_clr_bk(
    WGL_PG_CONTENT_T*                 pt_data, 
    GL_HGRAPHICS_T                    h_graphics,
    UINT16                            ui2_state)
{
    /*local variable declaration*/
    GL_COLOR_T*                       pt_bk_clr;
    INT32                             i4_ret;
    
    /*check and cache color*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_BK_CLR_NOT_INIT))
    {
        i4_ret = _wgl_pg_cnt_use_sys_color(pt_data, WGL_CLR_BK);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret; 
        }
        
        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, PG_BK_CLR_NOT_INIT);
    }
    
    /*select color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        pt_bk_clr = &pt_data->t_clr_bk.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        pt_bk_clr = &pt_data->t_clr_bk.t_enable;
    }
    else
    {
        pt_bk_clr = &pt_data->t_clr_bk.t_disable;
    }

    /*should check PG_NOT_CLEAN_BK style here*/
    
    /*paint background*/
    i4_ret = wgl_fill_widget_bk(h_graphics, pt_bk_clr, &pt_data->t_cnt_rect, &pt_data->t_cvsst);
    WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill color to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg(
    WGL_PG_CONTENT_T*             pt_data, 
    GL_HGRAPHICS_T                h_graphics,
    UINT32                        ui4_upt_rect,
    const GL_RECT_T*              pt_upt_rect,
    UINT16                        ui2_state)
{
    /*local variable declaration*/
    WGL_FONT_INFO_T               t_font_info;
    GL_RECT_T                     t_available_rect;
    INT32                         i4_ret;
    
    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_pg_cnt_set_font(pt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
   
    /*paint body*/
    WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
    i4_ret = _wgl_pg_cnt_paint_fg_body(pt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state, &t_available_rect);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*paint bar*/
    i4_ret = _wgl_pg_cnt_paint_fg_bar(pt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state, &t_available_rect);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*paint indicator, min, max*/
    i4_ret = _wgl_pg_cnt_paint_indicator(pt_data, h_graphics, ui2_state, &t_available_rect);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 *                  pt_available_rect = pt_available_rect - body inset
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_body(
    WGL_PG_CONTENT_T*                  pt_data, 
    GL_HGRAPHICS_T                     h_graphics,
    UINT32                             ui4_upt_rect,
    const GL_RECT_T*                   pt_upt_rect,
    UINT16                             ui2_state,
    GL_RECT_T*                         pt_available_rect)
{   
    /*calculate body rectangle*/
    WGL_RECT_INSET(pt_available_rect, 
                   pt_data->t_body_inset.i4_left, 
                   pt_data->t_body_inset.i4_top,
                   pt_data->t_body_inset.i4_right, 
                   pt_data->t_body_inset.i4_bottom);
    
    /*paint body*/
    if(PG_IMG_UI & pt_data->ui8_extra_flag)
    {
        return _wgl_pg_cnt_paint_fg_img_body(pt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state, pt_available_rect);
    }
    else
    {
        return _wgl_pg_cnt_paint_fg_clr_body(pt_data, h_graphics, ui2_state, pt_available_rect);
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 *              pt_available_rect = pt_available_rect - bar inset
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_bar(
    WGL_PG_CONTENT_T*                 pt_data, 
    GL_HGRAPHICS_T                    h_graphics,
    UINT32                            ui4_upt_rect,
    const GL_RECT_T*                  pt_upt_rect,
    UINT16                            ui2_state,
    GL_RECT_T*                        pt_available_rect)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                    ah_img_levels[WGL_PG_LEVEL_MAX] = {0};
    GL_COLOR_T*                       apt_clr_levels[WGL_PG_LEVEL_MAX] = {NULL};
    INT32                             a_level_up_bounds[WGL_PG_LEVEL_MAX] = {0};
    INT32                             i4_ret, i4_index, i4_count;
    BOOL                              b_is_img_ui;
    
    /*prepare image or color*/
    if(PG_IMG_UI & pt_data->ui8_extra_flag)
    {
        i4_ret = _wgl_pg_cnt_ready_bar_img(pt_data, ui2_state, ah_img_levels);
    }
    else
    {
        i4_ret = _wgl_pg_cnt_ready_bar_clr(pt_data, ui2_state, apt_clr_levels);
    }
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*prepare level boundaries*/
     if (pt_data->ui1_levels > 5)
    {
        return WGLR_INTERNAL_ERROR;
    }
    i4_count = pt_data->ui1_levels; /*i4_count is larger than 0*/
    if (i4_count <= 0)
    {
        return WGLR_INTERNAL_ERROR;
    }
    for(i4_index = 0; i4_index < (i4_count - 1); i4_index++)
    {
        a_level_up_bounds[i4_index] = pt_data->a_boundary[i4_index];
    }
    
    a_level_up_bounds[i4_count - 1] = pt_data->i4_max;
    
    /*calculate the bar available rectangle*/
    WGL_RECT_INSET(pt_available_rect, 
                   pt_data->t_bar_inset.i4_left, 
                   pt_data->t_bar_inset.i4_top,
                   pt_data->t_bar_inset.i4_right, 
                   pt_data->t_bar_inset.i4_bottom);

    /*paint bar*/
    if(_PG_IS_HIDE_BAR(pt_data->ui1_dis_mode))
    {
        return WGLR_OK;
    }
    else
    {
        b_is_img_ui = ((PG_IMG_UI & pt_data->ui8_extra_flag) == 0) ? FALSE : TRUE;
        
        if(_PG_IS_BLOCK(pt_data->ui8_extra_flag))
        {
            return _wgl_pg_cnt_paint_fg_bar_block(pt_data, 
                                                  h_graphics, 
                                                  ui4_upt_rect, 
                                                  pt_upt_rect, 
                                                  b_is_img_ui, 
                                                  pt_available_rect, 
                                                  a_level_up_bounds, 
                                                  ah_img_levels, 
                                                  (const GL_COLOR_T**)apt_clr_levels);
        }
        else if(_PG_IS_POINT(pt_data->ui8_extra_flag))
        {
            return _wgl_pg_cnt_paint_fg_bar_point(pt_data, 
                                                  h_graphics, 
                                                  ui4_upt_rect, 
                                                  pt_upt_rect, 
                                                  b_is_img_ui, 
                                                  pt_available_rect, 
                                                  a_level_up_bounds, 
                                                  ah_img_levels, 
                                                  (const GL_COLOR_T**)apt_clr_levels);
        }
        else if(_PG_IS_CURSOR(pt_data->ui8_extra_flag))
        {
            return _wgl_pg_cnt_paint_fg_bar_cursor(pt_data, 
                                                  h_graphics, 
                                                  ui4_upt_rect, 
                                                  pt_upt_rect, 
                                                  b_is_img_ui, 
                                                  pt_available_rect, 
                                                  a_level_up_bounds, 
                                                  ah_img_levels, 
                                                  (const GL_COLOR_T**)apt_clr_levels,
                                                  ui2_state);
        }
        else
        {
            return _wgl_pg_cnt_paint_fg_bar_smooth(pt_data, 
                                                   h_graphics, 
                                                   ui4_upt_rect, 
                                                   pt_upt_rect, 
                                                   b_is_img_ui, 
                                                   pt_available_rect, 
                                                   a_level_up_bounds, 
                                                   ah_img_levels, 
                                                   (const GL_COLOR_T**)apt_clr_levels);
        }
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_img_body(
    WGL_PG_CONTENT_T*                      pt_data, 
    GL_HGRAPHICS_T                         h_graphics,
    UINT32                                 ui4_upt_rect,
    const GL_RECT_T*                       pt_upt_rect,
    UINT16                                 ui2_state,
    const GL_RECT_T*                       pt_body_rect)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                         h_img_body;
    INT32                                  i4_ret;
    
    /*check and cache body image*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_BODY_IMG_NOT_INIT))
    {
        i4_ret = _wgl_pg_cnt_prepare_img(pt_data, WGL_IMG_PG_BODY);
        if(WGLR_OK != i4_ret)
        {
            return WGLR_OK; 
        }
        
        /*remove the flag for body image*/
        WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, PG_BODY_IMG_NOT_INIT);
    }
    
    /*select body image*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        h_img_body = pt_data->t_img_body.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_img_body = pt_data->t_img_body.t_enable;
    }
    else
    {
        h_img_body = pt_data->t_img_body.t_disable;
    }
    
#if 0 /*for CR 2584*/    
    /*check whether selected body image exists*/
    if(NULL_HANDLE == h_img_body)
    {
        h_img_body = WGL_STD_GET_NON_NULL_IMG(&pt_data->t_img_body);
    }
#endif
    
    if(NULL_HANDLE == h_img_body)
    {
        return WGLR_OK;
    }
    
    /*paint body*/
    if(NULL_HANDLE != h_img_body)
    {
        i4_ret = wgl_img_tpl_paint(h_img_body, 
                                   pt_body_rect, 
                                   pt_upt_rect, 
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw body image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_clr_body(
    WGL_PG_CONTENT_T*                      pt_data, 
    GL_HGRAPHICS_T                         h_graphics,
    UINT16                                 ui2_state,
    const GL_RECT_T*                       pt_body_rect)
{
    /*local variable declaration*/
    GL_COLOR_T*                            pt_color;
    INT32                                  i4_ret;
    GL_RECT_T                              t_cnt_rect= {0};
    /*check and cache body color*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_BODY_CLR_NOT_INIT))
    {
        i4_ret = _wgl_pg_cnt_use_sys_color(pt_data, WGL_CLR_PG_BODY);
        if(WGLR_OK != i4_ret)
        {
            return WGLR_OK; 
        }
        
        /*remove the flag for body color*/
        WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, PG_BODY_CLR_NOT_INIT);
    }
    if (_PG_IS_CURSOR(pt_data->ui8_extra_flag))
    {
        /*select color*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            pt_color = &pt_data->t_cursor_inset_clr.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            pt_color = &pt_data->t_cursor_inset_clr.t_enable;
        }
        else
        {
            pt_color = &pt_data->t_cursor_inset_clr.t_disable;
        }
        
        t_cnt_rect.i4_left = pt_body_rect->i4_left;
        t_cnt_rect.i4_top = pt_body_rect->i4_top;
        t_cnt_rect.i4_right = pt_body_rect->i4_right;
        t_cnt_rect.i4_bottom = pt_body_rect->i4_bottom;
            
        i4_ret = wgl_fill_widget_bk(h_graphics, pt_color, &t_cnt_rect, &pt_data->t_cvsst);
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill color to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
        if (i4_ret != WGLR_OK)
        {
            return i4_ret;
        }
        if (pt_data->ui8_extra_flag & PG_BAR_FRAME_NOT_INIT)
        {
            WGL_RECT_INSET( &t_cnt_rect,
                            pt_data->t_cursor_inset.i4_left,
                            pt_data->t_cursor_inset.i4_top,
                            pt_data->t_cursor_inset.i4_right,
                            pt_data->t_cursor_inset.i4_bottom);
        }
    }
    else
    {
        t_cnt_rect.i4_left = pt_body_rect->i4_left;
        t_cnt_rect.i4_top = pt_body_rect->i4_top;
        t_cnt_rect.i4_right = pt_body_rect->i4_right;
        t_cnt_rect.i4_bottom = pt_body_rect->i4_bottom;
    }
    /*select body color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        pt_color = &pt_data->t_clr_body.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        pt_color = &pt_data->t_clr_body.t_enable;
    }
    else
    {
        pt_color = &pt_data->t_clr_body.t_disable;
    }
    
    /*paint body*/
    i4_ret = wgl_fill_widget_bk(h_graphics, pt_color, &t_cnt_rect, &pt_data->t_cvsst);
    WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill body color. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
    
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_bar_smooth(
    WGL_PG_CONTENT_T*                  pt_data, 
    GL_HGRAPHICS_T                     h_graphics,
    UINT32                             ui4_upt_rect,
    const GL_RECT_T*                   pt_upt_rect,
    BOOL                               b_is_img_ui,
    const GL_RECT_T*                   pt_available_rect,
    const INT32                        a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T               a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*                  apt_clr_levels[WGL_PG_LEVEL_MAX])
{
    /*local varaible declaration*/
    GL_RECT_T                          a_level_rects[WGL_PG_LEVEL_MAX] = {{0}};
    WGL_TPL_PAINT_ALGSM_T              e_paint_algorism;
    INT32                              i4_last;
    INT32                              i4_avail_width;
    INT32                              i4_avail_height;
    INT32                              i4_min_size;
    INT32                              i4_range;
    INT32                              i4_value;
    INT32                              i4_ret;
    UINT8                              ui1_index;
    UINT8                              ui1_pos_level;
    /*calculate level rectangles*/
    ui1_pos_level = 0;
    i4_avail_width = WGL_RECT_GET_WIDTH(pt_available_rect);
    i4_avail_height = WGL_RECT_GET_HEIGHT(pt_available_rect);
    i4_min_size = (INT32)(pt_data->ui2_bar_min_size);

    i4_range = pt_data->i4_max - pt_data->i4_min;
    i4_value = pt_data->i4_cur_pos - pt_data->i4_min;

    
    /*calc scale size*/
    if(_PG_IS_HORIZTONAL(pt_data->ui8_extra_flag))
    {
        i4_last = pt_available_rect->i4_left;
        e_paint_algorism = WGL_TPL_PAINT_ALGSM_L2R_T2B;
        if(i4_avail_width > i4_min_size)
        {
            i4_avail_width -= i4_min_size;
        }
        else
        {
            i4_min_size = i4_avail_width;
            i4_avail_width = 0;
        }
    }
    else
    {
        if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
        {
            i4_last = pt_available_rect->i4_bottom;
            e_paint_algorism = WGL_TPL_PAINT_ALGSM_L2R_B2T;
        }
        else /*_PG_IS_VERTICAL_DOWN*/
        {
            i4_last = pt_available_rect->i4_top;
            e_paint_algorism = WGL_TPL_PAINT_ALGSM_L2R_T2B;
        }
        
        if(i4_avail_height > i4_min_size)
        {
            i4_avail_height -= i4_min_size;
        }
        else
        {
            i4_min_size = i4_avail_height;
            i4_avail_height = 0;
        }
    }
    

    if(pt_data->ui1_levels > 5)
    {
        return WGLR_INTERNAL_ERROR;
    }
    for(ui1_index = 0; ui1_index < pt_data->ui1_levels; ui1_index++)
    {
        ui1_pos_level++;

         if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
        {
            /*set bottom, left, right*/
            a_level_rects[ui1_index].i4_bottom = i4_last;
            a_level_rects[ui1_index].i4_left = pt_available_rect->i4_left;
            a_level_rects[ui1_index].i4_right = pt_available_rect->i4_right;

            /*calculate top*/
            if(pt_data->i4_cur_pos > a_level_up_bounds[ui1_index])
            {
                a_level_rects[ui1_index].i4_top = pt_available_rect->i4_bottom - i4_min_size - i4_avail_height * (a_level_up_bounds[ui1_index] - pt_data->i4_min) / i4_range;
                if (pt_data->ui1_step_size!= 0)
                {
                    if ((a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_top = a_level_rects[ui1_index].i4_bottom - 
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                        if( a_level_rects[ui1_index].i4_top <0)
                        {
                            a_level_rects[ui1_index].i4_bottom -= a_level_rects[ui1_index].i4_top;
                            a_level_rects[ui1_index].i4_top = 0;
                        }
                    }
                }
            }
            else
            {
                a_level_rects[ui1_index].i4_top = pt_available_rect->i4_bottom - i4_min_size - i4_avail_height * i4_value / i4_range;
                if (pt_data->ui1_step_size != 0)
                {
                    if (pt_data->i4_cur_pos == pt_data->i4_max)
                    {
                        a_level_rects[ui1_index].i4_top  = pt_available_rect->i4_top;
                    }
                    else if ((a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_top = a_level_rects[ui1_index].i4_bottom -
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                        if( a_level_rects[ui1_index].i4_top <0)
                        {
                            a_level_rects[ui1_index].i4_bottom -= a_level_rects[ui1_index].i4_top;
                            a_level_rects[ui1_index].i4_top = 0;
                        }
                    }
                }
                break;
            }
            
            /*keep the last top*/
            i4_last = a_level_rects[ui1_index].i4_top;
        }
        else if(_PG_IS_VERTICAL_DOWN(pt_data->ui8_extra_flag))
        {
            /*set top, left, right*/
            a_level_rects[ui1_index].i4_top = i4_last;
            a_level_rects[ui1_index].i4_left = pt_available_rect->i4_left;
            a_level_rects[ui1_index].i4_right = pt_available_rect->i4_right;

            /*calculate bottom*/
            if(pt_data->i4_cur_pos > a_level_up_bounds[ui1_index])
            {
                a_level_rects[ui1_index].i4_bottom = pt_available_rect->i4_top + i4_min_size + i4_avail_height * (a_level_up_bounds[ui1_index] - pt_data->i4_min) / i4_range;
                if (pt_data->ui1_step_size != 0)
                {
                    if ((a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_bottom =a_level_rects[ui1_index].i4_top + 
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                    }
                }
            }
            else
            {
                a_level_rects[ui1_index].i4_bottom = pt_available_rect->i4_top + i4_min_size + i4_avail_height * i4_value / i4_range;
                if (pt_data->ui1_step_size != 0)
                {
                    if (pt_data->i4_cur_pos == pt_data->i4_max)
                    {
                        a_level_rects[ui1_index].i4_bottom = pt_available_rect->i4_bottom;
                    }
                    else if ((a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_bottom =a_level_rects[ui1_index].i4_top + 
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_bottom - a_level_rects[ui1_index].i4_top))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                    }
                }
                break;
            }
            
            /*keep the last bottom*/
            i4_last = a_level_rects[ui1_index].i4_bottom;
        }
        else /*_PG_IS_HORIZTONAL(ui4_flag)*/
        {
            /*set top, bottom, left*/
            a_level_rects[ui1_index].i4_left = i4_last;
            a_level_rects[ui1_index].i4_top = pt_available_rect->i4_top;
            a_level_rects[ui1_index].i4_bottom = pt_available_rect->i4_bottom;

            /*calculate right*/
            if(pt_data->i4_cur_pos > a_level_up_bounds[ui1_index])
            {
                a_level_rects[ui1_index].i4_right = pt_available_rect->i4_left + i4_min_size + i4_avail_width * (a_level_up_bounds[ui1_index] - pt_data->i4_min) / i4_range;
                if (pt_data->ui1_step_size != 0)
                {
                    if ((a_level_rects[ui1_index].i4_right - a_level_rects[ui1_index].i4_left)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_right =a_level_rects[ui1_index].i4_left +
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_right - a_level_rects[ui1_index].i4_left))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                    }
                }
            }
            else
            {
                a_level_rects[ui1_index].i4_right = pt_available_rect->i4_left + i4_min_size + i4_avail_width * i4_value / i4_range;
                if (pt_data->ui1_step_size != 0)
                {
                    if (pt_data->i4_cur_pos == pt_data->i4_max)
                    {
                        a_level_rects[ui1_index].i4_right = pt_available_rect->i4_right;
                    }
                    else if ((a_level_rects[ui1_index].i4_right - a_level_rects[ui1_index].i4_left)%pt_data->ui1_step_size != 0)
                    {
                        a_level_rects[ui1_index].i4_right =a_level_rects[ui1_index].i4_left + 
                            ((INT32)(((FLOAT)(a_level_rects[ui1_index].i4_right - a_level_rects[ui1_index].i4_left))/pt_data->ui1_step_size + 0.5)*pt_data->ui1_step_size) ;
                    }
                }
                break;
            }
            
            /*keep the last right*/
            i4_last = a_level_rects[ui1_index].i4_right;
        }
    }
    
    /*paint levels*//*smooth style*/
    i4_ret = WGLR_OK;
    for(ui1_index = 0; ui1_index < ui1_pos_level; ui1_index++)
    {
        if(b_is_img_ui)
        {
            if(NULL_HANDLE != a_img_levels[ui1_index])
            {
                i4_ret = wgl_img_tpl_paint_ex(a_img_levels[ui1_index], 
                                              &a_level_rects[ui1_index], 
                                              pt_upt_rect, 
                                              h_graphics, 
                                              &pt_data->t_cvsst,
                                              WGL_TPL_PAINT_COMPOSE,
                                              e_paint_algorism);

                WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw bar image (smooth style). (%d) at L%d\n\n", i4_ret, __LINE__)));
            }
        }
        else
        {
           i4_ret = wgl_fill_widget_bk(h_graphics, 
                                       apt_clr_levels[ui1_index], 
                                       &a_level_rects[ui1_index],
                                       &pt_data->t_cvsst);
           
           WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (smooth style). (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_bar_block(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX])
{
    /*local variable declaration*/
    GL_RECT_T                       t_paint_rect;
    GL_RECT_T                       t_limit_rect;
    GL_RECT_T                       t_temp_rect;
    INT32                           i4_base = 0;
    INT32                           i4_start = 0, i4_end = 0, i4_range, i4_target;
    INT32                           i4_avail_width, i4_avail_height;
    INT32                           i4_blk_width = 0, i4_blk_height = 0;
    INT32                           i4_min_size, i4_current_blk = 0;
    UINT32                          ui4_img_width, ui4_img_height;
    INT32                           i4_ret;
    BOOL                            b_update = TRUE, b_calculate_offset = TRUE, b_not_use_asign, b_continue;
    UINT8                           ui1_index = 0;
    
    
    /*initialize varaible*/
    i4_avail_width = WGL_RECT_GET_WIDTH(pt_available_rect);
    i4_avail_height = WGL_RECT_GET_HEIGHT(pt_available_rect);
    i4_range = pt_data->i4_max - pt_data->i4_min;
    
    i4_min_size = (INT32)(pt_data->ui2_bar_min_size);
    
    /*calc scale size*/
    if(_PG_IS_HORIZTONAL(pt_data->ui8_extra_flag))
    {
        if(i4_avail_width > i4_min_size)
        {
            i4_avail_width -= i4_min_size;
        }
        else
        {
            i4_min_size = i4_avail_width;
            i4_avail_width = 0;
        }

        i4_base = pt_available_rect->i4_left;
        i4_target = pt_available_rect->i4_left + i4_avail_width * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range + i4_min_size;
    }
    else
    {
       if(i4_avail_height > i4_min_size)
        {
            i4_avail_height -= i4_min_size;
        }
        else
        {
            i4_min_size = i4_avail_height;
            i4_avail_height = 0;
        }

        if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
        {
            i4_base = pt_available_rect->i4_bottom;
            i4_target = pt_available_rect->i4_bottom - i4_avail_height * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range - i4_min_size;
        }
        else
        {
            i4_base = pt_available_rect->i4_top;
            i4_target = pt_available_rect->i4_top + i4_avail_height * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range + i4_min_size;
        }
    }
     
    if(0 == pt_data->ui2_block_width || 0 == pt_data->ui2_block_height)
    {
        /*use the width and height of level image*/
        b_not_use_asign = TRUE;
    }
    else
    {
        /*use user setting*/
        i4_blk_width = (INT32)(pt_data->ui2_block_width);
        i4_blk_height = (INT32)(pt_data->ui2_block_height);
        b_not_use_asign = FALSE;
    }

    /*init boundary rectangle*/
    WGL_RECT_COPY(&t_limit_rect, pt_available_rect);
    
    do
    {
        if(ui1_index == 5)
        {
            break;
        }
        if(b_not_use_asign && b_update)
        {
            /*get image size*/
            if(b_is_img_ui)
            {
                if(NULL_HANDLE != a_img_levels[PROGRESS_CHECK_ID(ui1_index)])
                {
                    i4_ret = x_wgl_img_tpl_get_size(a_img_levels[PROGRESS_CHECK_ID(ui1_index)], &ui4_img_width, &ui4_img_height);
                    if(WGLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get bar image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return i4_ret;
                    }
                    WGL_ASSERT(ui4_img_width <= 0x7FFFFFFF);
                    WGL_ASSERT(ui4_img_height <= 0x7FFFFFFF);
            
                    i4_blk_width = (INT32)ui4_img_width;
                    i4_blk_height = (INT32)ui4_img_height;
                }
                else
                {
                    if(_PG_IS_HORIZTONAL(pt_data->ui8_extra_flag))
                    {
                        i4_blk_width = i4_avail_width * (a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)] - pt_data->i4_min) / i4_range;
                        i4_blk_height = i4_avail_height;
                    }
                    else
                    {
                        i4_blk_width = i4_avail_width;
                        i4_blk_height = i4_avail_height * (a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)] - pt_data->i4_min) / i4_range;
                    }
                }
            }
            else /*use available width or height*/
            {
                i4_blk_width = (0 == pt_data->ui2_block_width) ? i4_avail_width : (INT32)(pt_data->ui2_block_width);
                i4_blk_height = (0 == pt_data->ui2_block_height) ? i4_avail_height : (INT32)(pt_data->ui2_block_height);
            }

            b_update = FALSE;
            b_calculate_offset = TRUE;
        }

        /*calculate start and offset for the level image*/
        if(b_calculate_offset)
        {
            if(_PG_IS_HORIZTONAL(pt_data->ui8_extra_flag))
            {
                i4_start = (i4_avail_height > i4_blk_height) ? ((i4_avail_height - i4_blk_height) >> 1) : 0;
                i4_start += pt_available_rect->i4_top;
                i4_end = (i4_avail_height > i4_blk_height) ? i4_blk_height : i4_avail_height;
                i4_end += i4_start;

                /*update boundary rectangle*/
                if(pt_data->i4_cur_pos >= a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)])
                {
                    t_limit_rect.i4_right = pt_available_rect->i4_left + i4_avail_width * (a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)] - pt_data->i4_min) / i4_range + i4_min_size;
                }
                else
                {
                    t_limit_rect.i4_right = pt_available_rect->i4_left + i4_avail_width * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range + i4_min_size;
                }
            }
            else
            {
                i4_start = (i4_avail_width > i4_blk_width) ? ((i4_avail_width - i4_blk_width) >> 1) : 0;
                i4_start += pt_available_rect->i4_left;
                i4_end = (i4_avail_width > i4_blk_width) ? i4_blk_width : i4_avail_width;
                i4_end += i4_start;

                if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
                {
                    /*update boundary rectangle*/
                    if(pt_data->i4_cur_pos >= a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)])
                    {
                        t_limit_rect.i4_top = pt_available_rect->i4_bottom - i4_avail_height * (a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)] - pt_data->i4_min) / i4_range - i4_min_size;

                    }
                    else
                    {
                        t_limit_rect.i4_top = pt_available_rect->i4_bottom - i4_avail_height * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range - i4_min_size;
                    }
                }
                else
                {
                    /*update boundary rectangle*/
                    if(pt_data->i4_cur_pos >= a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)])
                    {
                        t_limit_rect.i4_bottom = pt_available_rect->i4_top + i4_avail_height * (a_level_up_bounds[PROGRESS_CHECK_ID(ui1_index)] - pt_data->i4_min) / i4_range + i4_min_size;
                    }
                    else
                    {
                        t_limit_rect.i4_bottom = pt_available_rect->i4_top + i4_avail_height * (pt_data->i4_cur_pos - pt_data->i4_min) / i4_range + i4_min_size;
                    }
                }
            }

            b_calculate_offset = FALSE;
        }

        if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
        {
            t_paint_rect.i4_bottom = i4_base;
            t_paint_rect.i4_top = t_paint_rect.i4_bottom - ((i4_current_blk > 0) ? i4_current_blk : i4_blk_height);
            t_paint_rect.i4_left = i4_start;
            t_paint_rect.i4_right = i4_end;

            /*check top*/
            /*painted range is larger than requested range*/
            i4_current_blk = (t_paint_rect.i4_top < t_limit_rect.i4_top) ? (t_limit_rect.i4_top - t_paint_rect.i4_top) : 0;

            /*next bottom*/
            i4_base = (i4_current_blk > 0) ? (t_limit_rect.i4_top) : (t_paint_rect.i4_top - pt_data->ui1_block_gap);

            /*loop*/
            b_continue = (i4_base > i4_target) ? TRUE : FALSE;
        }
        else if(_PG_IS_VERTICAL_DOWN(pt_data->ui8_extra_flag))
        {
            t_paint_rect.i4_top = i4_base;
            t_paint_rect.i4_bottom = t_paint_rect.i4_top + ((i4_current_blk > 0) ? i4_current_blk : i4_blk_height);
            t_paint_rect.i4_left = i4_start;
            t_paint_rect.i4_right = i4_end;

            /*check bottom*/
            /*painted range is larger than requested range*/
            i4_current_blk = (t_paint_rect.i4_bottom > t_limit_rect.i4_bottom) ? (t_paint_rect.i4_bottom - t_limit_rect.i4_bottom) : 0;

            /*next top*/
            i4_base = (i4_current_blk > 0) ? (t_limit_rect.i4_bottom) : (t_paint_rect.i4_bottom + pt_data->ui1_block_gap);

            /*loop*/
            b_continue = (i4_base < i4_target) ? TRUE : FALSE;
        }
        else /*_PG_IS_HORIZTONAL(ui4_flag)*/
        {
            t_paint_rect.i4_left = i4_base;
            t_paint_rect.i4_right = t_paint_rect.i4_left + ((i4_current_blk > 0) ? i4_current_blk : i4_blk_width);
            t_paint_rect.i4_top = i4_start;
            t_paint_rect.i4_bottom = i4_end;

            /*check right*/
            /*painted range is larger than requested range*/
            i4_current_blk = (t_paint_rect.i4_right > t_limit_rect.i4_right) ? (t_paint_rect.i4_right - t_limit_rect.i4_right) : 0;

            /*next left*/
            i4_base = (i4_current_blk > 0) ? (t_limit_rect.i4_right) : (t_paint_rect.i4_right + pt_data->ui1_block_gap);

            /*loop*/
            b_continue = (i4_base < i4_target) ? TRUE : FALSE;
        }

        /*get the intersection rectangle*/
        if(i4_current_blk > 0)
        {
            i4_ret = x_wgl_rect_intersect(&t_temp_rect, &t_paint_rect, &t_limit_rect);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get intersection rect of paint rect and limit rect. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            
            WGL_RECT_COPY(&t_paint_rect, &t_temp_rect);
        }

        /*paint foreground*/
        if(b_is_img_ui)
        {
            if(NULL_HANDLE != a_img_levels[PROGRESS_CHECK_ID(ui1_index)])
            {
                i4_ret = wgl_img_tpl_paint(a_img_levels[PROGRESS_CHECK_ID(ui1_index)], 
                                           &t_paint_rect, 
                                           pt_upt_rect, 
                                           h_graphics, 
                                           &pt_data->t_cvsst,
                                           WGL_TPL_PAINT_COMPOSE);

                WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw bar image (block style). (%d) at L%d\n\n", i4_ret, __LINE__)));
            }
            else
            {
                i4_ret = WGLR_OK;
            }
        }
        else
        {
            i4_ret = wgl_fill_widget_bk(h_graphics, 
                                        apt_clr_levels[PROGRESS_CHECK_ID(ui1_index)], 
                                        &t_paint_rect,
                                        &pt_data->t_cvsst);

            WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (block style). (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*next level*/
        if(i4_current_blk > 0)
        {
            ui1_index++;
            b_update = (b_is_img_ui) ? TRUE : FALSE;
            b_calculate_offset = TRUE;
        }
        
    } while(b_continue);
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_bar_point(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX])
{
    /*local variable declaration*/
    UINT32                          ui4_img_width = 0;
    UINT32                          ui4_img_height = 0;
    GL_RECT_T                       t_paint_rect = {0};
    UINT8                           ui1_pos_level;
    INT32                           i4_offset_x;
    INT32                           i4_offset_y;
    INT32                           i4_available_width;
    INT32                           i4_available_height;
    INT32                           i4_ret;
    
    /*get the level for cur-pos*/ 
    if (pt_data->ui1_levels > 5)
    {
        return WGLR_INTERNAL_ERROR;
    }
    for(ui1_pos_level = 0; ui1_pos_level < pt_data->ui1_levels; ui1_pos_level++)
    {		
        if(pt_data->i4_cur_pos <= a_level_up_bounds[ui1_pos_level])
        {
            break;
        }
    }
    WGL_ASSERT(ui1_pos_level < pt_data->ui1_levels);

    if(ui1_pos_level > 4)
    {
          return WGLR_INTERNAL_ERROR;
    }
    /*check template handle*/
    if(b_is_img_ui && NULL_HANDLE == a_img_levels[ui1_pos_level])
    {
        return WGLR_OK;
    }
    
    /*calculate the paint rectangle*/
    if(0 == pt_data->ui2_block_width || 0 == pt_data->ui2_block_height)
    {
        if(b_is_img_ui)
        {
            i4_ret = x_wgl_img_tpl_get_size(a_img_levels[ui1_pos_level], &ui4_img_width, &ui4_img_height);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get bar image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            WGL_ASSERT(ui4_img_width <= 0x7FFFFFFF);
            WGL_ASSERT(ui4_img_height <= 0x7FFFFFFF);
        
            t_paint_rect.i4_right = (INT32)ui4_img_width;
            t_paint_rect.i4_bottom = (INT32)ui4_img_height;
        }
        else
        {
            t_paint_rect.i4_right = (0 == pt_data->ui2_block_width) ? (WGL_RECT_GET_WIDTH(pt_available_rect)): (INT32)(pt_data->ui2_block_width);
            t_paint_rect.i4_bottom = (0 == pt_data->ui2_block_height) ? (WGL_RECT_GET_HEIGHT(pt_available_rect)): (INT32)(pt_data->ui2_block_height);            
        }
    }
    else
    {
        t_paint_rect.i4_right = (INT32)(pt_data->ui2_block_width);
        t_paint_rect.i4_bottom = (INT32)(pt_data->ui2_block_height);
    }

    /*check width and height*/
    i4_available_width = WGL_RECT_GET_WIDTH(pt_available_rect);
    i4_available_height = WGL_RECT_GET_HEIGHT(pt_available_rect);
    if(t_paint_rect.i4_right > i4_available_width)
    {
        t_paint_rect.i4_right = i4_available_width;
    }
    if(t_paint_rect.i4_bottom > i4_available_height)
    {
        t_paint_rect.i4_bottom = i4_available_height;
    }

    /*calculate the offset*/
    if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
    {
        i4_offset_x = (i4_available_width - t_paint_rect.i4_right) >> 1;
        i4_offset_x += pt_available_rect->i4_left;
        i4_offset_y = pt_available_rect->i4_bottom - t_paint_rect.i4_bottom;
        i4_offset_y -= (pt_data->i4_cur_pos - pt_data->i4_min) * (i4_available_height - t_paint_rect.i4_bottom) / (pt_data->i4_max - pt_data->i4_min);
    }
    else if(_PG_IS_VERTICAL_DOWN(pt_data->ui8_extra_flag))
    {
        i4_offset_x = (i4_available_width - t_paint_rect.i4_right) >> 1;
        i4_offset_x += pt_available_rect->i4_left;
        i4_offset_y = (pt_data->i4_cur_pos - pt_data->i4_min) * (i4_available_height - t_paint_rect.i4_bottom) / (pt_data->i4_max - pt_data->i4_min);
        i4_offset_y += pt_available_rect->i4_top;
    }
    else
    {
        i4_offset_x = (pt_data->i4_cur_pos - pt_data->i4_min) * (i4_available_width - t_paint_rect.i4_right) / (pt_data->i4_max - pt_data->i4_min);
        i4_offset_x += pt_available_rect->i4_left;
        i4_offset_y = (i4_available_height - t_paint_rect.i4_bottom) >> 1;
        i4_offset_y += pt_available_rect->i4_top;
    }

    /*offset the paint rectangle*/
    WGL_RECT_OFFSET(&t_paint_rect, i4_offset_x, i4_offset_y);

    /*paint point*/
    if(b_is_img_ui)
    {
        i4_ret = wgl_img_tpl_paint(a_img_levels[ui1_pos_level], 
                                   &t_paint_rect, 
                                   pt_upt_rect,
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);

        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw bar image (point style). (%d) at L%d\n\n", i4_ret, __LINE__)));
    }
    else
    {
        i4_ret = wgl_fill_widget_bk(h_graphics, 
                                    apt_clr_levels[ui1_pos_level], 
                                    &t_paint_rect,
                                    &pt_data->t_cvsst);
        
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (point style). (%d) at L%d\n\n", i4_ret, __LINE__)));
    }

    return i4_ret;
}
/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_fg_bar_cursor(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    BOOL                            b_is_img_ui,
    const GL_RECT_T*                pt_available_rect,
    const INT32                     a_level_up_bounds[WGL_PG_LEVEL_MAX],
    const WGL_HIMG_TPL_T            a_img_levels[WGL_PG_LEVEL_MAX],
    const GL_COLOR_T*               apt_clr_levels[WGL_PG_LEVEL_MAX],
    UINT16                          ui2_state)
{
    /*local variable declaration*/
    UINT32                          ui4_img_width = 0;
    UINT32                          ui4_img_height = 0;
    GL_RECT_T                       t_update_bar_rect = {0};
    GL_RECT_T                       t_cursor_rect = {0};
    UINT8                           ui1_pos_level;
    INT32                           i4_offset_x;
    INT32                           i4_offset_y;
    INT32                           i4_ret;
    GL_COLOR_T*                     pt_frame_clr;
    GL_COLOR_T*                     pt_cursor_clr;
    WGL_HIMG_TPL_T                  h_img_tmp = NULL_HANDLE;

    /*get the level for cur-pos*/ 
    if (pt_data->ui1_levels > 5 || pt_data->ui1_levels < 1)
    {
        return WGLR_INTERNAL_ERROR;
    }
    for(ui1_pos_level = 0; ui1_pos_level < pt_data->ui1_levels; ui1_pos_level++)
    {		
        if(pt_data->i4_cur_pos <= a_level_up_bounds[ui1_pos_level])
        {
            break;
        }
    }
    WGL_ASSERT(ui1_pos_level < pt_data->ui1_levels);

    if(ui1_pos_level > 4)
    {
          return WGLR_INTERNAL_ERROR;
    }
    /*check template handle*/
    if(b_is_img_ui && NULL_HANDLE == a_img_levels[ui1_pos_level])
    {
        return WGLR_OK;
    }
    
    /*calculate the paint rectangle*/
   if(b_is_img_ui)
    {

        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_img_tmp = pt_data->t_img_cursor.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            h_img_tmp = pt_data->t_img_cursor.t_enable;
        }
        else
        {
            h_img_tmp = pt_data->t_img_cursor.t_disable;
        }
        
        i4_ret = x_wgl_img_tpl_get_size(a_img_levels[0], &ui4_img_width, &ui4_img_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get bar image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        WGL_ASSERT(ui4_img_width <= 0x7FFFFFFF);
        WGL_ASSERT(ui4_img_height <= 0x7FFFFFFF);
    
        t_cursor_rect.i4_right = (INT32)ui4_img_width;
        t_cursor_rect.i4_bottom = (INT32)ui4_img_height;

        
        i4_ret = x_wgl_img_tpl_get_size(h_img_tmp, &ui4_img_width, &ui4_img_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get bar image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        WGL_ASSERT(ui4_img_width <= 0x7FFFFFFF);
        WGL_ASSERT(ui4_img_height <= 0x7FFFFFFF);
    
        pt_data->t_size_cursor.ui4_width  = ui4_img_width;
        pt_data->t_size_cursor.ui4_height = ui4_img_height;
    }
    else
    {
        t_cursor_rect.i4_right = (0 == pt_data->t_size_cursor.ui4_width) ? (WGL_RECT_GET_WIDTH(pt_available_rect)): ((INT32)(pt_data->t_size_cursor.ui4_width));
        t_cursor_rect.i4_bottom = (0 == pt_data->t_size_cursor.ui4_height ) ? (WGL_RECT_GET_HEIGHT(pt_available_rect)): ((INT32)(pt_data->t_size_cursor.ui4_height));            
    }

    t_update_bar_rect.i4_left = pt_data->t_cnt_rect.i4_left;
    t_update_bar_rect.i4_top = pt_data->t_cnt_rect.i4_top;
    t_update_bar_rect.i4_right = pt_data->t_cnt_rect.i4_right;
    t_update_bar_rect.i4_bottom = pt_data->t_cnt_rect.i4_bottom;
    WGL_RECT_INSET( &t_update_bar_rect,
                    pt_data->t_body_inset.i4_left,
                    pt_data->t_body_inset.i4_top,
                    pt_data->t_body_inset.i4_right,
                    pt_data->t_body_inset.i4_bottom);
    if (pt_data->ui8_extra_flag & PG_BAR_FRAME_NOT_INIT)
    {
        WGL_RECT_INSET(&t_update_bar_rect,
                    pt_data->t_cursor_inset.i4_left,
                    pt_data->t_cursor_inset.i4_top,
                    pt_data->t_cursor_inset.i4_right,
                    pt_data->t_cursor_inset.i4_bottom);
    }

    _wgl_pg_cnt_cal_cursor_pos(pt_data,&t_update_bar_rect, &i4_offset_x, &i4_offset_y);
    /*offset the paint rectangle*/
    WGL_RECT_OFFSET(&t_cursor_rect, i4_offset_x, i4_offset_y);

    /*paint cursor*/
    if(b_is_img_ui)
    {
        i4_ret = wgl_img_tpl_paint(a_img_levels[0], 
                                   &t_update_bar_rect, 
                                   pt_upt_rect,
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);
        
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw bar image (point style). (%d) at L%d\n\n", i4_ret, __LINE__)));

        i4_ret = wgl_img_tpl_paint(h_img_tmp, 
                                   &t_cursor_rect, 
                                   pt_upt_rect,
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);
                
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw bar image (point style). (%d) at L%d\n\n", i4_ret, __LINE__)));
    }
    else
    {
        i4_ret = wgl_fill_widget_bk(h_graphics, 
                                    apt_clr_levels[0], 
                                    &t_update_bar_rect,
                                    &pt_data->t_cvsst);

        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (cursor style). (%d) at L%d\n\n", i4_ret, __LINE__)));
         if (pt_data->ui8_extra_flag & PG_BAR_FRAME_NOT_INIT)
        {
            /*select frame color*/
            if(IS_STATE_FOCUSED(ui2_state))
            {
                pt_frame_clr = &pt_data->t_cursor_inset_clr.t_highlight;
            }
            else if(IS_STATE_ENABLED(ui2_state))
            {
                pt_frame_clr = &pt_data->t_cursor_inset_clr.t_enable;
            }
            else
            {
                pt_frame_clr = &pt_data->t_cursor_inset_clr.t_disable;
            }
            i4_ret = wgl_fill_widget_bk(h_graphics, 
                                    pt_frame_clr, 
                                    &t_cursor_rect,
                                    &pt_data->t_cvsst);
            
            WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (cursor style). (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        else
        {
           pt_data->t_cursor_inset.i4_left = 0;
           pt_data->t_cursor_inset.i4_top = 0;
           pt_data->t_cursor_inset.i4_right = 0;
           pt_data->t_cursor_inset.i4_bottom = 0;
        }

        WGL_RECT_INSET(&t_cursor_rect,
               pt_data->t_cursor_inset.i4_left,
               pt_data->t_cursor_inset.i4_top,
               pt_data->t_cursor_inset.i4_right,
               pt_data->t_cursor_inset.i4_bottom);
        /*select cursor color*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            pt_cursor_clr = &pt_data->t_clr_cursor.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            pt_cursor_clr = &pt_data->t_clr_cursor.t_enable;
        }
        else
        {
            pt_cursor_clr = &pt_data->t_clr_cursor.t_disable;
        }
        i4_ret = wgl_fill_widget_bk(h_graphics, 
                                pt_cursor_clr, 
                                &t_cursor_rect,
                                &pt_data->t_cvsst);
        
        WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to fill bar color (cursor style). (%d) at L%d\n\n", i4_ret, __LINE__)));
        
    }

    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_paint_indicator(
    WGL_PG_CONTENT_T*               pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT16                          ui2_state,
    const GL_RECT_T*                pt_available_rect)
{
    /*local variable declaration*/
    GL_RECT_T                       t_paint_rect;
    GL_RECT_T                       t_temp_rect;
    GL_RECT_T                       t_target_rect;
    GL_COLOR_T                      t_empty_clr;
    GL_COLOR_T*                     pt_clr;
    INT32                           i4_ret;
    HANDLE_T                        t_text_font; 
    WGL_FONT_INFO_T                 t_font_info;
    INT16                           i2_font_height;
    UINT16                          ui2_font_height;
    /*init empty color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_empty_clr);
    
    /*get font according to widget state*/   
    i4_ret = _wgl_pg_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_font_height);
    if(FER_OK != i4_ret || i2_font_height < 0)
    {
        WGL_ASSERT(0);

        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get font height (handle = %d). (FER=%d) at L%d\n\n", t_text_font, i4_ret, __LINE__));
        return  i4_ret;
    }
    ui2_font_height = (UINT16)i2_font_height;
    
    /*check and cache text color*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_TEXT_CLR_NOT_INIT))
    {
        i4_ret = _wgl_pg_cnt_use_sys_color(pt_data, WGL_CLR_TEXT);
        if(WGLR_OK != i4_ret)
        {
            return WGLR_OK; 
        }
        
        /*remove the flag for body color*/
        WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, PG_TEXT_CLR_NOT_INIT);
    }

    /*select text color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        pt_clr = &pt_data->t_clr_text.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        pt_clr = &pt_data->t_clr_text.t_enable;
    }
    else
    {
        pt_clr = &pt_data->t_clr_text.t_disable;
    }
    
    /*get IDR width*/
    if(_PG_IS_POS_DIRTY(pt_data->ui8_extra_flag) && _PG_IS_SHOW_IDR(pt_data->ui1_dis_mode))
    {
        /*get IDR string, length, width, height*/
        i4_ret = _wgl_pg_cnt_calc_indicator(pt_data->i4_cur_pos, 
                                            pt_data->i4_min, 
                                            pt_data->i4_max,
                                            _PG_IS_PERCENT(pt_data->ui1_dis_mode), 
                                            t_text_font, 
                                            pt_data->a_idr_buf, 
                                            &pt_data->ui1_idr_length, 
                                            &pt_data->ui2_idr_width, 
                                            &ui2_font_height);
        
        if(WGLR_OK != i4_ret)
        {
            return WGLR_INTERNAL_ERROR;
        }

        /*update pos dirty flag*/
        _PG_TURN_OFF_POS_DIRTY(pt_data);
    }

    /*get Min width and Max width*/
    if(_PG_IS_RANGE_DIRTY(pt_data->ui8_extra_flag))
    {
        if(_PG_IS_SHOW_MIN(pt_data->ui1_dis_mode))
        {
            /*get MIN string, length, width, height*/
            i4_ret = _wgl_pg_cnt_calc_indicator(pt_data->i4_min, 
                                                pt_data->i4_min, 
                                                pt_data->i4_max, 
                                                _PG_IS_PERCENT(pt_data->ui1_dis_mode), 
                                                t_text_font, 
                                                pt_data->a_min_buf,
                                                &pt_data->ui1_min_length, 
                                                &pt_data->ui2_min_width, 
                                                &ui2_font_height);
            
            if(WGLR_OK != i4_ret)
            {
                return WGLR_INTERNAL_ERROR;
            }
        }
        
        if(_PG_IS_SHOW_MAX(pt_data->ui1_dis_mode))
        {
            /*get MAX string, length, width, height*/
            i4_ret = _wgl_pg_cnt_calc_indicator(pt_data->i4_max, 
                                                pt_data->i4_min, 
                                                pt_data->i4_max, 
                                                _PG_IS_PERCENT(pt_data->ui1_dis_mode), 
                                                t_text_font, 
                                                pt_data->a_max_buf, 
                                                &pt_data->ui1_max_length, 
                                                &pt_data->ui2_max_width, 
                                                &ui2_font_height);
            
            if(WGLR_OK != i4_ret)
            {
                return WGLR_INTERNAL_ERROR;
            }
        }

        /*update pos dirty flag*/
        _PG_TURN_OFF_RANGE_DIRTY(pt_data);
    }

    /*paint indicator*/
    if(_PG_IS_SHOW_IDR(pt_data->ui1_dis_mode))
    {
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_USER_DEF_POS_LOC))
        {            
            _wgl_pg_cnt_offset_idr_rect(&pt_data->t_pos_loc,
                                        (INT32)(pt_data->ui2_idr_width),
                                        (INT32)ui2_font_height,
                                        &t_paint_rect);
        }
        else
        {
            /*calc draw rectangle*/
            t_temp_rect.i4_left = ((WGL_RECT_GET_WIDTH(pt_available_rect) - pt_data->ui2_idr_width) >> 1) + pt_available_rect->i4_left;
            t_temp_rect.i4_right = t_temp_rect.i4_left + pt_data->ui2_idr_width;
            t_temp_rect.i4_top = ((WGL_RECT_GET_HEIGHT(pt_available_rect) - ui2_font_height) >> 1) + pt_available_rect->i4_top;
            t_temp_rect.i4_bottom = t_temp_rect.i4_top + ui2_font_height;
            
            WGL_RECT_COPY(&t_paint_rect, &t_temp_rect);
        }
    
        /*draw text*/
        if(!WGL_RECT_IS_EMPTY(&t_paint_rect) &&
           !wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_clr))
        {                       
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          t_paint_rect.i4_left,
                                          t_paint_rect.i4_top,
                                          0,
                                          pt_data->a_idr_buf,
                                          pt_data->ui1_idr_length,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_clr,
                                          &t_empty_clr,
                                          &t_empty_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)(t_font_info.i2_width),
                                          0,
                                          0,
                                          0);
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw indicator. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    }

    /*paint min*/
    if(_PG_IS_SHOW_MIN(pt_data->ui1_dis_mode))
    {
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_USER_DEF_MIN_LOC))
        {            
            _wgl_pg_cnt_offset_idr_rect(&pt_data->t_min_loc,
                                        (INT32)(pt_data->ui2_min_width),
                                        (INT32)ui2_font_height,
                                        &t_paint_rect);
        }        
        else
        {
            /*calc draw rectangle*/
            if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
            {
                t_target_rect.i4_left = pt_data->t_cnt_rect.i4_left;
                t_target_rect.i4_right = pt_data->t_cnt_rect.i4_right;
                t_target_rect.i4_bottom = pt_data->t_cnt_rect.i4_bottom;
                t_target_rect.i4_top = t_target_rect.i4_bottom - pt_data->t_body_inset.i4_bottom;
            }
            else if(_PG_IS_VERTICAL_DOWN(pt_data->ui8_extra_flag))
            {
                t_target_rect.i4_left = pt_data->t_cnt_rect.i4_left;
                t_target_rect.i4_right = pt_data->t_cnt_rect.i4_right;
                t_target_rect.i4_top = pt_data->t_cnt_rect.i4_top;
                t_target_rect.i4_bottom = t_target_rect.i4_top + pt_data->t_body_inset.i4_top;
            }
            else /*_PG_IS_HORIZTONAL(ui4_flag)*/
            {
                t_target_rect.i4_left = pt_data->t_cnt_rect.i4_left;
                t_target_rect.i4_top = pt_data->t_cnt_rect.i4_top;
                t_target_rect.i4_bottom = pt_data->t_cnt_rect.i4_bottom;
                t_target_rect.i4_right = t_target_rect.i4_left + pt_data->t_body_inset.i4_left;
            }
    
            t_temp_rect.i4_left = ((WGL_RECT_GET_WIDTH(&t_target_rect) - pt_data->ui2_min_width) >> 1) + t_target_rect.i4_left;
            t_temp_rect.i4_right = t_temp_rect.i4_left + pt_data->ui2_min_width;
            t_temp_rect.i4_top = ((WGL_RECT_GET_HEIGHT(&t_target_rect) - ui2_font_height) >> 1) + t_target_rect.i4_top;
            t_temp_rect.i4_bottom = t_temp_rect.i4_top + ui2_font_height;              
            
            WGL_RECT_COPY(&t_paint_rect, &t_temp_rect);
        }

        /*draw text*/
        if(!WGL_RECT_IS_EMPTY(&t_paint_rect) &&
           !wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_clr))
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          t_paint_rect.i4_left,
                                          t_paint_rect.i4_top,
                                          0,
                                          pt_data->a_min_buf,
                                          pt_data->ui1_min_length,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_clr,
                                          &t_empty_clr,
                                          &t_empty_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)(t_font_info.i2_width),
                                          0,
                                          0,
                                          0);                                 
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw min. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    }

    /*paint max*/
    if(_PG_IS_SHOW_MAX(pt_data->ui1_dis_mode))
    {
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, PG_USER_DEF_MAX_LOC))
        {
            _wgl_pg_cnt_offset_idr_rect(&pt_data->t_max_loc,
                                        (INT32)(pt_data->ui2_max_width),
                                        (INT32)ui2_font_height,
                                        &t_paint_rect);
        }
        else
        {
            /*calc draw rectangle*/
            if(_PG_IS_VERTICAL_UP(pt_data->ui8_extra_flag))
            {
                t_target_rect.i4_left = pt_data->t_cnt_rect.i4_left;
                t_target_rect.i4_right = pt_data->t_cnt_rect.i4_right;
                t_target_rect.i4_top = pt_data->t_cnt_rect.i4_top;
                t_target_rect.i4_bottom = t_target_rect.i4_top + pt_data->t_body_inset.i4_top;
            }
            else if(_PG_IS_VERTICAL_DOWN(pt_data->ui8_extra_flag))
            {
                t_target_rect.i4_left = pt_data->t_cnt_rect.i4_left;
                t_target_rect.i4_right = pt_data->t_cnt_rect.i4_right;
                t_target_rect.i4_bottom = pt_data->t_cnt_rect.i4_bottom;
                t_target_rect.i4_top = t_target_rect.i4_bottom - pt_data->t_body_inset.i4_bottom;
            }
            else /*_PG_IS_HORIZTONAL(ui4_flag)*/
            {
                t_target_rect.i4_right = pt_data->t_cnt_rect.i4_right;
                t_target_rect.i4_top = pt_data->t_cnt_rect.i4_top;
                t_target_rect.i4_bottom = pt_data->t_cnt_rect.i4_bottom;
                t_target_rect.i4_left = t_target_rect.i4_right - pt_data->t_body_inset.i4_right;
            }
            
            t_temp_rect.i4_left = ((WGL_RECT_GET_WIDTH(&t_target_rect) - pt_data->ui2_max_width) >> 1) + t_target_rect.i4_left;
            t_temp_rect.i4_right = t_temp_rect.i4_left + pt_data->ui2_max_width;
            t_temp_rect.i4_top = ((WGL_RECT_GET_HEIGHT(&t_target_rect) - ui2_font_height) >> 1) + t_target_rect.i4_top;
            t_temp_rect.i4_bottom = t_temp_rect.i4_top + ui2_font_height;              
            
            WGL_RECT_COPY(&t_paint_rect, &t_temp_rect);
        }
        
        /*draw text*/
        if(!WGL_RECT_IS_EMPTY(&t_paint_rect) && 
           !wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_clr))
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          t_paint_rect.i4_left,
                                          t_paint_rect.i4_top,
                                          0,
                                          pt_data->a_max_buf,
                                          pt_data->ui1_max_length,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_clr,
                                          &t_empty_clr,
                                          &t_empty_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)(t_font_info.i2_width),
                                          0,
                                          0,
                                          0);
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to draw max. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_img(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_IMG_INFO_T*           pt_img_info)
{   
    /*local varaible declaration*/
    WGL_IMG_STD_SET_T*              pt_action_img;
    UINT64                          ui8_action_flag;

    DBG_INFO(("{WGL(PG_CNT)} pt_img_info = 0x%.8x   e_type = 0x%.8x\n"
                             "\t(enable, disable) = (0x%.8x, 0x%.8x)\n"
                             "\t(highlight, highlight_inactive) = (0x%.8x, 0x%.8x)\n"
                             "\t(push, push_unhighlight) = (0x%.8x, 0x%.8x)\n",
                             pt_img_info,
                             ((NULL == pt_img_info) ? 0 : pt_img_info->e_type),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_enable),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_disable),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_highlight),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_highlight_inactive),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_push),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_push_unhighlight)));

    /*check pt_img_info*/
    if(pt_img_info && FALSE == wgl_is_legal_img_info(pt_img_info))
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*select dest*/
    switch (ui1_target)
    {
    case WGL_IMG_BK:
        {
            pt_action_img = &pt_cnt_data->t_img_bk;
            ui8_action_flag = PG_BK_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_FG:
    case WGL_IMG_PG_BODY:
        {
            pt_action_img = &pt_cnt_data->t_img_body;
            ui8_action_flag = PG_BODY_IMG_NOT_INIT;
        }
        break;
    
    case WGL_IMG_PG_LEVEL_1:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[0];
            ui8_action_flag = PG_LEVLE1_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_2:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[1];
            ui8_action_flag = PG_LEVLE2_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_3:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[2];
            ui8_action_flag = PG_LEVLE3_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_4:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[3];
            ui8_action_flag = PG_LEVLE4_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_5:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[4];
            ui8_action_flag = PG_LEVLE5_IMG_NOT_INIT;
        }
        break;
        
    case WGL_IMG_PG_CURSOR:
        {
            pt_action_img = &pt_cnt_data->t_img_cursor;
            ui8_action_flag = PG_LEVLE5_IMG_NOT_INIT;
        }
        break;
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown image target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }
    

    /*update background image*/
    if(pt_img_info)
    {
        COPY_IMG_INFO_TO_STD(pt_action_img, pt_img_info, NULL_HANDLE);

        /*remove the flag for action image*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }
    else
    {
        x_memset(pt_action_img, 0, sizeof(WGL_IMG_STD_SET_T));

        /*reset the flag for action image*/
        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }
    
    WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_BAR_IMAGE_W_H_NOT_INIT);
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_img(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    WGL_IMG_INFO_T*                     pt_img_info)
{
    /*local variable declaration*/
    INT32                               i4_ret;
    WGL_IMG_STD_SET_T*                  pt_action_img;
    UINT64                              ui8_action_flag;

    /*check parameter*/
    if(NULL == pt_img_info)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*select source*/
    switch (ui1_target)
    {
    case WGL_IMG_BK:
        {
            pt_action_img = &pt_cnt_data->t_img_bk;
            ui8_action_flag = PG_BK_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_FG:
    case WGL_IMG_PG_BODY:
        {
            pt_action_img = &pt_cnt_data->t_img_body;
            ui8_action_flag = PG_BODY_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_1:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[0];
            ui8_action_flag = PG_LEVLE1_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_2:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[1];
            ui8_action_flag = PG_LEVLE2_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_3:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[2];
            ui8_action_flag = PG_LEVLE3_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_4:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[3];
            ui8_action_flag = PG_LEVLE4_IMG_NOT_INIT;
        }
        break;

    case WGL_IMG_PG_LEVEL_5:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[4];
            ui8_action_flag = PG_LEVLE5_IMG_NOT_INIT;
        }
        break;

   case WGL_IMG_PG_CURSOR:
        {
            pt_action_img = &pt_cnt_data->t_img_cursor;
            ui8_action_flag = PG_LEVLE5_IMG_NOT_INIT;
        }
        break;
        
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown image target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }

    /*check and cache images*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, ui8_action_flag))
    {
        i4_ret = _wgl_pg_cnt_prepare_img(pt_cnt_data, ui1_target);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret; 
        }
    
        /*remove the flag for bk image*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }

    /*prepare return value*/
    SET_STANDARD_TO_IMG_INFO(pt_img_info, pt_action_img);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_color(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_COLOR_INFO_T*             pt_clr_info)
{
    /*local varaible declaration*/
    GL_COLOR_T                          t_default_color;
    WGL_COLOR_STD_SET_T*                pt_action_clr;
    UINT64                              ui8_action_flag;

    DBG_INFO(("{WGL(PG_CNT)} pt_clr_info = 0x%.8x   e_type = 0x%.8x\n"
                             "\t(enable, disable) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n"
                             "\t(highlight, highlight_inactive) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n"
                             "\t(t_push, push_unhighlight) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n",
                             pt_clr_info,
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->e_type),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u3.b)));

    /*init default color*/
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_default_color);
    
    /*select target*/
    switch (ui1_target)
    {
    case WGL_CLR_BK:
        {
            pt_action_clr = &pt_cnt_data->t_clr_bk;
            ui8_action_flag = PG_BK_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_TEXT:
        {
            pt_action_clr = &pt_cnt_data->t_clr_text;
            ui8_action_flag = PG_TEXT_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_BODY:
        {
            pt_action_clr = &pt_cnt_data->t_clr_body;
            ui8_action_flag = PG_BODY_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_1:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[0];
            ui8_action_flag = PG_LEVEL1_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_2:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[1];
            ui8_action_flag = PG_LEVEL2_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_3:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[2];
            ui8_action_flag = PG_LEVEL3_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_4:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[3];
            ui8_action_flag = PG_LEVEL4_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_5:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[4];
            ui8_action_flag = PG_LEVEL5_CLR_NOT_INIT;
        }
        break;
        
    case WGL_CLR_PG_CURSOR:
        {
            pt_action_clr = &pt_cnt_data->t_clr_cursor;
            ui8_action_flag = PG_BAR_CURSOR_CLR_NOT_INIT;
        }
        break;
    case WGL_CLR_PG_CURSOR_INSET:
        {
            pt_action_clr = &pt_cnt_data->t_cursor_inset_clr;
            ui8_action_flag = PG_BAR_CURSOR_CLR_NOT_INIT;
        }
        break;
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown color target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }
    

    /*update background image*/
    if(pt_clr_info)
    {
        COPY_CLR_INFO_TO_STD(pt_action_clr, pt_clr_info, t_default_color);

        /*remove the flag for action color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }
    else
    {
        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &pt_action_clr->t_enable);
        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &pt_action_clr->t_disable);
        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &pt_action_clr->t_highlight);

        /*reset the flag for action color*/
        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_color(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    WGL_COLOR_INFO_T*                   pt_clr_info)
{
    /*local variable declaration*/
    WGL_COLOR_STD_SET_T*                pt_action_clr;
    UINT64                              ui8_action_flag;
    INT32                               i4_ret;

    /*check parameter*/
    if(NULL == pt_clr_info)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid input parameter. For WGL_CMD_GL_GET_COLOR, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*select source*/
    switch (ui1_target)
    {
    case WGL_CLR_BK:
        {
            pt_action_clr = &pt_cnt_data->t_clr_bk;
            ui8_action_flag = PG_BK_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_TEXT:
        {
            pt_action_clr = &pt_cnt_data->t_clr_text;
            ui8_action_flag = PG_TEXT_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_BODY:
        {
            pt_action_clr = &pt_cnt_data->t_clr_body;
            ui8_action_flag = PG_BODY_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_1:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[0];
            ui8_action_flag = PG_LEVEL1_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_2:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[1];
            ui8_action_flag = PG_LEVEL2_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_3:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[2];
            ui8_action_flag = PG_LEVEL3_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_4:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[3];
            ui8_action_flag = PG_LEVEL4_CLR_NOT_INIT;
        }
        break;

    case WGL_CLR_PG_LEVEL_5:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[4];
            ui8_action_flag = PG_LEVEL5_CLR_NOT_INIT;
        }
        break;
   case WGL_CLR_PG_CURSOR:
        {
            pt_action_clr = &pt_cnt_data->t_clr_cursor;
            ui8_action_flag = PG_BAR_CURSOR_CLR_NOT_INIT;
        }
        break;
   case WGL_CLR_PG_CURSOR_INSET:
        {
            pt_action_clr = &pt_cnt_data->t_cursor_inset_clr;
            ui8_action_flag = PG_BAR_CURSOR_CLR_NOT_INIT;
        }
        break;
        
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown color target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, ui8_action_flag))
    {
        /*get system colors*/
        i4_ret = _wgl_pg_cnt_use_sys_color(pt_cnt_data, ui1_target);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, ui8_action_flag);
    }

    /*prepare return value*/
    SET_STANDARD_TO_COLOR_INFO(pt_clr_info, pt_action_clr);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_font(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const WGL_FONT_INFO_T*          pt_font_info)
{
    INT32                           i4_ret;

    DBG_INFO(("{WGL(PG_CNT)} pt_font_info = 0x%.8x   e_font_cmap =  0x%.8x\n"
                             "\te_font_size = 0x%.8x   e_font_style = 0x%.8x\n"
                             "\tpt_font_info->a_c_font_name = %s\n",
                             pt_font_info,
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_cmap),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_size),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_style),
                             ((NULL == pt_font_info) ? "\0" : pt_font_info->a_c_font_name)));

    /*update dirty flag*/
    _PG_TURN_ON_RANGE_DIRTY(pt_cnt_data);
    _PG_TURN_ON_POS_DIRTY(pt_cnt_data);

    if(pt_font_info)
    {
         /*update pt_cnt_data->t_text_font,pt_cnt_data->t_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, pt_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
       /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, pt_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT);
    }
    else
    {
        /*free pt_cnt_data->t_text_font and clear pt_cnt_data->t_font_info*/
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            i4_ret = WGL_FONT_FREE(pt_cnt_data->t_text_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
        /*free pt_cnt_data->t_text_focus_font and clear pt_cnt_data->t_focus_font_info*/
       x_memset(&pt_cnt_data->t_focus_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_focus_font)
        {
            i4_ret = WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }
        
        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT);
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *           command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_font(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_FONT_INFO_T*                pt_font_info)
{
    /*local variable declaration*/
    WGL_FONT_INFO_T                 t_font_info;
    INT32                           i4_ret;
    
    /*check parameter*/
    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system font info. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_pg_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    /*prepare return value*/
    x_memcpy(pt_font_info, &pt_cnt_data->t_font_info, sizeof(WGL_FONT_INFO_T));

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_font_ex(
    WGL_PG_CONTENT_T*             pt_cnt_data,
    const WGL_FONT_INFO_EX_T*     pt_font_info_ex)
{
    INT32                         i4_ret = WGLR_OK;
   
    /*check parameter*/
    if(NULL != pt_font_info_ex && pt_font_info_ex->e_type != WGL_FONT_SET_BASIC)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_SET_FONT_EX (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
      /*update dirty flag*/
    _PG_TURN_ON_RANGE_DIRTY(pt_cnt_data);
    _PG_TURN_ON_POS_DIRTY(pt_cnt_data);    

    if(pt_font_info_ex)
    {
        /*update pt_cnt_data->t_text_font,pt_cnt_data->t_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, &(pt_font_info_ex->u_font_data.t_basic.t_unfocused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
       /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, &(pt_font_info_ex->u_font_data.t_basic.t_focused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        /*remove the flag for font*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT);
    }
    else
    {
         /*free pt_cnt_data->t_text_font and clear pt_cnt_data->t_font_info*/
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            i4_ret = WGL_FONT_FREE(pt_cnt_data->t_text_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
        /*free pt_cnt_data->t_text_focus_font and clear pt_cnt_data->t_focus_font_info*/
       x_memset(&pt_cnt_data->t_focus_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_focus_font)
        {
            i4_ret = WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }

        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT);
    }
    
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_CMD_TEXT_GET_MIN_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_font_ex(
    WGL_PG_CONTENT_T*             pt_cnt_data,
    WGL_FONT_INFO_EX_T*           pt_font_info_ex)
{
    /*local variable declaration*/
    INT32                         i4_ret;
    WGL_FONT_INFO_T               t_font_info;
    
    /*check parameter*/
    if(NULL == pt_font_info_ex)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT_EX, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system font info. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_pg_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*prepare return value*/
    pt_font_info_ex->e_type = WGL_FONT_SET_BASIC;
    x_memcpy(&(pt_font_info_ex->u_font_data.t_basic.t_unfocused), &pt_cnt_data->t_font_info, sizeof(WGL_FONT_INFO_T));
    x_memcpy(&(pt_font_info_ex->u_font_data.t_basic.t_focused), &pt_cnt_data->t_focus_font_info, sizeof(WGL_FONT_INFO_T));

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_levels(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    const WGL_PG_LEVELS_INFO_T*         pt_infos)
{
    /*local variable declaration*/
    UINT8                               ui1_index;
    UINT64                              ui8_img_mask;
    UINT64                              ui8_clr_mask;
    UINT64                              a_img_masks[WGL_PG_LEVEL_MAX] = {PG_LEVLE5_IMG_NOT_INIT, 
                                                                         PG_LEVLE4_IMG_NOT_INIT,
                                                                         PG_LEVLE3_IMG_NOT_INIT,
                                                                         PG_LEVLE2_IMG_NOT_INIT,
                                                                         PG_LEVLE1_IMG_NOT_INIT};
    UINT64                              a_clr_masks[WGL_PG_LEVEL_MAX] = {PG_LEVEL5_CLR_NOT_INIT, 
                                                                         PG_LEVEL4_CLR_NOT_INIT,
                                                                         PG_LEVEL3_CLR_NOT_INIT,
                                                                         PG_LEVEL2_CLR_NOT_INIT,
                                                                         PG_LEVEL1_CLR_NOT_INIT};

    /*check parameter*/
    if(NULL == pt_infos)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(PG_CNT)} ui1_level_number = %d\n", pt_infos->ui1_level_number));

    /*check levels*/
    if(WGL_PG_LEVEL_MAX < pt_infos->ui1_level_number || 0 == pt_infos->ui1_level_number)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. 1 ~ WGL_PG_LEVEL_MAX are allowed. (%d) at L%d\n\n", WGLR_OVER_SUPPORT_LIMIT, __LINE__));
        return WGLR_OVER_SUPPORT_LIMIT;
    }

    /*check boundary*/
    for(ui1_index = 0; ui1_index < (pt_infos->ui1_level_number - (UINT8)2); ui1_index++)
    {
        DBG_INFO(("{WGL(PG_CNT)} level %d = %d\n", ui1_index + 1, pt_infos->a_boundary[ui1_index]));
        WGL_PG_REPORT(TRUE, (ui1_index != (UINT8)(pt_infos->ui1_level_number - 3)), DBG_INFO(("{WGL(PG_CNT)} level %d = %d\n", pt_infos->ui1_level_number, pt_infos->a_boundary[pt_infos->ui1_level_number - 1])));

        if(pt_infos->a_boundary[ui1_index] > pt_infos->a_boundary[ui1_index + 1])
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. Lower level should be less than higher. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
            return WGLR_INV_ARG;
        }
    }
    if(pt_infos->ui1_level_number > 1)
    {
        if(pt_infos->a_boundary[0] < pt_cnt_data->i4_min 
            || pt_infos->a_boundary[pt_infos->ui1_level_number - 2] > pt_cnt_data->i4_max)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. Levels should be lagrer than the min and less than the max. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
            return WGLR_INV_ARG;
        }
    }

    /*set clean mask*/
    ui8_img_mask = 0;
    ui8_clr_mask = 0;
    for(ui1_index = 0; ui1_index < (WGL_PG_LEVEL_MAX - pt_infos->ui1_level_number); ui1_index++)
    {
        ui8_img_mask |= a_img_masks[ui1_index];
        ui8_clr_mask |= a_clr_masks[ui1_index];
    }
    
    /*clean level image and level color*/
    if(WGL_PG_LEVEL_MAX > pt_infos->ui1_level_number)
    {
        x_memset(&pt_cnt_data->a_bar_clr[pt_infos->ui1_level_number], 
                 0, 
                 (WGL_PG_LEVEL_MAX - pt_infos->ui1_level_number) * sizeof(WGL_COLOR_STD_SET_T));
        x_memset(&pt_cnt_data->a_bar_img[pt_infos->ui1_level_number], 
                 0, 
                 (WGL_PG_LEVEL_MAX - pt_infos->ui1_level_number) * sizeof(WGL_IMG_STD_SET_T));

        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, ui8_img_mask);
        WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, ui8_clr_mask);
    }

    /*store setting*/
    pt_cnt_data->ui1_levels = pt_infos->ui1_level_number;
    x_memcpy(pt_cnt_data->a_boundary, pt_infos->a_boundary, (WGL_PG_LEVEL_MAX - 1) * sizeof(INT32));

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_levels(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    WGL_PG_LEVELS_INFO_T*           pt_infos)
{
    /*check parameter*/
    if(NULL == pt_infos)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*prepare return value*/
    pt_infos->ui1_level_number = pt_cnt_data->ui1_levels;

    x_memcpy(pt_infos->a_boundary, pt_cnt_data->a_boundary, (WGL_PG_LEVEL_MAX - 1) * sizeof(INT32));

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          _wgl_pg_cnt_calc_update_rect
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_calc_update_rect(
    WGL_PG_CONTENT_T*                     pt_cnt_data,
    WGL_PG_CNT_CALC_UPDATE_INFO_T*        pt_update_info)
{
    GL_RECT_T                             t_bar_rect;
    INT32                                 i4_old_end, i4_new_end, i4_bar_w, i4_bar_h, i4_min_size;
    INT32                                 i4_old_start, i4_new_start, i4_ret;
    UINT32                                ui4_img_w, ui4_img_h;

    if(NULL == pt_update_info || 
       (pt_cnt_data->i4_min > pt_update_info->i4_old_pos) ||
       (pt_cnt_data->i4_max < pt_update_info->i4_old_pos) || 
       (pt_cnt_data->i4_min > pt_update_info->i4_new_pos) ||
       (pt_cnt_data->i4_max < pt_update_info->i4_new_pos))
    {
        return WGLR_INV_ARG;
    }

    if(pt_update_info->i4_new_pos == pt_update_info->i4_old_pos)
    {
        WGL_RECT_EMPTY(&pt_update_info->t_update_rect);
        return WGLR_OK;
    }

    /*calculate the bar rectangle*/
    WGL_RECT_COPY(&t_bar_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_bar_rect, 
                   pt_cnt_data->t_body_inset.i4_left, 
                   pt_cnt_data->t_body_inset.i4_top,
                   pt_cnt_data->t_body_inset.i4_right, 
                   pt_cnt_data->t_body_inset.i4_bottom);
    WGL_RECT_INSET(&t_bar_rect, 
                   pt_cnt_data->t_bar_inset.i4_left, 
                   pt_cnt_data->t_bar_inset.i4_top,
                   pt_cnt_data->t_bar_inset.i4_right, 
                   pt_cnt_data->t_bar_inset.i4_bottom);
                   
    i4_min_size = (INT32)(pt_cnt_data->ui2_bar_min_size);
    
    /*check bar image size*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_BAR_IMAGE_W_H_NOT_INIT))
    {
        if(NULL_HANDLE != pt_cnt_data->a_bar_img[0].t_highlight)
        {
            i4_ret = x_wgl_img_tpl_get_size(pt_cnt_data->a_bar_img[0].t_highlight, &ui4_img_w, &ui4_img_h);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(PG_CNT)} ERR: x_wgl_img_tpl_get_size() failed. (i4_ret = %d) at L%d\n\n", i4_ret, __LINE__));
                
                pt_cnt_data->ui2_bar_img_width = 0;
                pt_cnt_data->ui2_bar_img_height = 0;
            }
            else
            {
                pt_cnt_data->ui2_bar_img_width = (UINT16)ui4_img_w;
                pt_cnt_data->ui2_bar_img_height = (UINT16)ui4_img_h;
            }
        }
        else
        {
            pt_cnt_data->ui2_bar_img_width = 0;
            pt_cnt_data->ui2_bar_img_height = 0;
        }
        
        WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_BAR_IMAGE_W_H_NOT_INIT);
    }

    if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
    {
        i4_bar_w = WGL_RECT_GET_WIDTH(&t_bar_rect);
        
        pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top;
        pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom;

        if(_PG_IS_POINT(pt_cnt_data->ui8_extra_flag))
        {
            i4_old_start = (pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * (i4_bar_w - pt_cnt_data->ui2_block_width) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            i4_new_start = (pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * (i4_bar_w - pt_cnt_data->ui2_block_width) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            
            pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left + ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
            pt_update_info->t_update_rect.i4_right = t_bar_rect.i4_left + ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) + pt_cnt_data->ui2_block_width;
        }
        else if (_PG_IS_CURSOR(pt_cnt_data->ui8_extra_flag))
        {
        #if 0
            i4_old_start = (pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * (i4_bar_w - pt_cnt_data->t_size_cursor.ui4_width) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            i4_new_start = (pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * (i4_bar_w - pt_cnt_data->t_size_cursor.ui4_width) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            
            pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left + ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
            pt_update_info->t_update_rect.i4_right = t_bar_rect.i4_left + ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) + pt_cnt_data->t_size_cursor.ui4_width;
        #endif
            pt_update_info->t_update_rect.i4_top = pt_cnt_data->t_cnt_rect.i4_top;
            pt_update_info->t_update_rect.i4_bottom = pt_cnt_data->t_cnt_rect.i4_bottom;

            pt_update_info->t_update_rect.i4_left  = pt_cnt_data->t_cnt_rect.i4_left;
            pt_update_info->t_update_rect.i4_right = pt_cnt_data->t_cnt_rect.i4_right;
        }
        else
        {
            if(i4_bar_w > i4_min_size)
            {
                i4_bar_w -= i4_min_size;
            }
            else
            {
                i4_min_size = i4_bar_w;
                i4_bar_w = 0;
            }
            
            i4_old_end = i4_min_size + ((pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * i4_bar_w) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);    
            i4_new_end = i4_min_size + ((pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * i4_bar_w) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            
            if(i4_new_end > i4_old_end) /*bar increases*/
            {
                i4_old_end -= pt_cnt_data->ui2_bar_img_width;
                
                pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left + i4_old_end;
                if(pt_update_info->t_update_rect.i4_left < t_bar_rect.i4_left)
                {
                    pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left;
                }
                
                pt_update_info->t_update_rect.i4_right = t_bar_rect.i4_left + i4_new_end;
            }
            else
            {
                i4_new_end -= pt_cnt_data->ui2_bar_img_width;
                if(i4_new_end < i4_min_size)
                {
                    i4_new_end = i4_min_size;
                }
                
                pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left + i4_new_end;
                pt_update_info->t_update_rect.i4_right = t_bar_rect.i4_left + i4_old_end;
            }
        }
        
    }
    else 
    {
        i4_bar_h = WGL_RECT_GET_HEIGHT(&t_bar_rect);
        
        pt_update_info->t_update_rect.i4_left = t_bar_rect.i4_left;
        pt_update_info->t_update_rect.i4_right = t_bar_rect.i4_right;
        
        if(_PG_IS_POINT(pt_cnt_data->ui8_extra_flag))
        {
            i4_old_start = (pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * (i4_bar_h - pt_cnt_data->ui2_block_height) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            i4_new_start = (pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * (i4_bar_h - pt_cnt_data->ui2_block_height) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            
            if(_PG_IS_VERTICAL_UP(pt_cnt_data->ui8_extra_flag))
            {
                pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_bottom - ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) - pt_cnt_data->ui2_block_height;
                pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom - ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
            }
            else
            {
                pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top + ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
                pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_top + ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) + pt_cnt_data->ui2_block_height;
            }
        }
        else if (_PG_IS_CURSOR(pt_cnt_data->ui8_extra_flag))
        {
        #if 0
            i4_old_start = (pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * (i4_bar_h - pt_cnt_data->t_size_cursor.ui4_height) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            i4_new_start = (pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * (i4_bar_h - pt_cnt_data->t_size_cursor.ui4_height) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            
            if(_PG_IS_VERTICAL_UP(pt_cnt_data->ui8_extra_flag))
            {
                pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_bottom - ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) - pt_cnt_data->t_size_cursor.ui4_height;
                pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom - ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
            }
            else
            {
                pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top + ((i4_new_start > i4_old_start) ? i4_old_start : i4_new_start);
                pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_top + ((i4_new_start > i4_old_start) ? i4_new_start : i4_old_start) + pt_cnt_data->t_size_cursor.ui4_height;
            }
        #endif
            pt_update_info->t_update_rect.i4_top = pt_cnt_data->t_cnt_rect.i4_top ;
            pt_update_info->t_update_rect.i4_bottom = pt_cnt_data->t_cnt_rect.i4_bottom;
            pt_update_info->t_update_rect.i4_left = pt_cnt_data->t_cnt_rect.i4_left;
            pt_update_info->t_update_rect.i4_right= pt_cnt_data->t_cnt_rect.i4_right;
        }
        else
        {
            if(i4_bar_h > i4_min_size)
            {
                i4_bar_h -= i4_min_size;
            }
            else
            {
                i4_min_size = i4_bar_h;
                i4_bar_h = 0;
            }
            
            i4_old_end = i4_min_size + ((pt_update_info->i4_old_pos - pt_cnt_data->i4_min) * i4_bar_h) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
            i4_new_end = i4_min_size + ((pt_update_info->i4_new_pos - pt_cnt_data->i4_min) * i4_bar_h) / (pt_cnt_data->i4_max - pt_cnt_data->i4_min);
    
            if(_PG_IS_VERTICAL_UP(pt_cnt_data->ui8_extra_flag))
            {
                if(i4_new_end > i4_old_end) /*bar increases*/
                {
                    i4_old_end -= pt_cnt_data->ui2_bar_img_height;
                    
                    pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_bottom - i4_new_end;
                    pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom - i4_old_end;
                    if(pt_update_info->t_update_rect.i4_bottom > t_bar_rect.i4_bottom)
                    {
                        pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom;
                    }
                }
                else
                {
                    i4_new_end -= pt_cnt_data->ui2_bar_img_height;
                    if(i4_new_end < i4_min_size)
                    {
                        i4_new_end = i4_min_size;
                    }
                    
                    pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_bottom - i4_old_end;
                    pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_bottom - i4_new_end;
                }
            }
            else
            {
                if(i4_new_end > i4_old_end) /*bar increases*/
                {
                    i4_old_end -= pt_cnt_data->ui2_bar_img_height;
                    
                    pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top + i4_old_end;
                    if(pt_update_info->t_update_rect.i4_top < t_bar_rect.i4_top)
                    {
                        pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top;
                    }
                    
                    pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_top + i4_new_end;
                }
                else
                {
                    i4_new_end -= pt_cnt_data->ui2_bar_img_height;
                    if(i4_new_end < i4_min_size)
                    {
                        i4_new_end = i4_min_size;
                    }
                    
                    pt_update_info->t_update_rect.i4_top = t_bar_rect.i4_top + i4_new_end;
                    pt_update_info->t_update_rect.i4_bottom = t_bar_rect.i4_top + i4_old_end;
                }
            }
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          _wgl_pg_cnt_get_indicator_rect
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_indicator_rect(
    WGL_PG_CONTENT_T*                       pt_cnt_data,
    GL_RECT_T*                              pt_indicator_rect,
    BOOL*                                   pb_is_show)
{
    GL_RECT_T                               t_bar_rect;
    INT32                                   i4_ret, i4_size;
    HANDLE_T                                t_text_font; 
    WGL_FONT_INFO_T                         t_font_info;
     INT16                                  i2_font_height;
    UINT16                                  ui2_font_height;
    
    if(NULL == pt_indicator_rect || NULL == pb_is_show)
    {
        return WGLR_INV_ARG;
    }
    /*get font according to widget state*/   
    i4_ret = _wgl_pg_cnt_select_text_font( pt_cnt_data, pt_cnt_data->ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_font_height);
    if(FER_OK != i4_ret || i2_font_height < 0)
    {
        WGL_ASSERT(0);

        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get font height (handle = %d). (FER=%d) at L%d\n\n", t_text_font, i4_ret, __LINE__));
        return  i4_ret;
    }
    ui2_font_height = (UINT16)i2_font_height;
    
    *pb_is_show = _PG_IS_SHOW_IDR(pt_cnt_data->ui1_dis_mode);


    if(_PG_IS_RANGE_DIRTY(pt_cnt_data->ui8_extra_flag) || FALSE == _PG_IS_SHOW_MIN(pt_cnt_data->ui1_dis_mode))
    {
        /*get MIN string, length, width, height*/
        i4_ret = _wgl_pg_cnt_calc_indicator(pt_cnt_data->i4_min, 
                                            pt_cnt_data->i4_min, 
                                            pt_cnt_data->i4_max, 
                                            _PG_IS_PERCENT(pt_cnt_data->ui1_dis_mode), 
                                            t_text_font, 
                                            pt_cnt_data->a_min_buf,
                                            &pt_cnt_data->ui1_min_length, 
                                            &pt_cnt_data->ui2_min_width, 
                                            &ui2_font_height);
        
        if(WGLR_OK != i4_ret)
        {
            return WGLR_INTERNAL_ERROR;
        }
    }
    
    if(_PG_IS_RANGE_DIRTY(pt_cnt_data->ui8_extra_flag) || FALSE == _PG_IS_SHOW_MAX(pt_cnt_data->ui1_dis_mode))
    {
        /*get MAX string, length, width, height*/
        i4_ret = _wgl_pg_cnt_calc_indicator(pt_cnt_data->i4_max, 
                                            pt_cnt_data->i4_min, 
                                            pt_cnt_data->i4_max, 
                                            _PG_IS_PERCENT(pt_cnt_data->ui1_dis_mode), 
                                            t_text_font, 
                                            pt_cnt_data->a_max_buf, 
                                            &pt_cnt_data->ui1_max_length, 
                                            &pt_cnt_data->ui2_max_width, 
                                            &ui2_font_height);
        
        if(WGLR_OK != i4_ret)
        {
            return WGLR_INTERNAL_ERROR;
        }
    }
        
    if(_PG_IS_RANGE_DIRTY(pt_cnt_data->ui8_extra_flag))
    {
        /*update pos dirty flag*/
        _PG_TURN_OFF_RANGE_DIRTY(pt_cnt_data);
    }

    /*calculate the bar rectangle*/
    WGL_RECT_COPY(&t_bar_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_bar_rect, 
                   pt_cnt_data->t_body_inset.i4_left, 
                   pt_cnt_data->t_body_inset.i4_top,
                   pt_cnt_data->t_body_inset.i4_right, 
                   pt_cnt_data->t_body_inset.i4_bottom);
    WGL_RECT_INSET(&t_bar_rect, 
                   pt_cnt_data->t_bar_inset.i4_left, 
                   pt_cnt_data->t_bar_inset.i4_top,
                   pt_cnt_data->t_bar_inset.i4_right, 
                   pt_cnt_data->t_bar_inset.i4_bottom);

    /*calc draw rectangle*/
    i4_size = (INT32)((pt_cnt_data->ui2_max_width > pt_cnt_data->ui2_min_width) ? pt_cnt_data->ui2_max_width : pt_cnt_data->ui2_min_width);
    
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_POS_LOC))
    {
        #if 1
        _wgl_pg_cnt_offset_idr_rect(&pt_cnt_data->t_pos_loc,
                                    i4_size,
                                    (INT32)ui2_font_height,
                                    pt_indicator_rect);
        #else                                        
        pt_indicator_rect->i4_left = pt_cnt_data->t_pos_loc.t_loc_rect.i4_left;
        pt_indicator_rect->i4_top = pt_cnt_data->t_pos_loc.t_loc_rect.i4_top;
        pt_indicator_rect->i4_right = pt_indicator_rect->i4_left + ((i4_size > WGL_RECT_GET_WIDTH(&pt_cnt_data->t_cnt_rect)) ? 
                                                                        WGL_RECT_GET_WIDTH(&pt_cnt_data->t_cnt_rect) : i4_size);
        pt_indicator_rect->i4_bottom = pt_indicator_rect->i4_top + pt_cnt_data->ui2_font_height;
        #endif
    }
    else
    {
        pt_indicator_rect->i4_left = ((WGL_RECT_GET_WIDTH(&t_bar_rect) - i4_size) >> 1) + t_bar_rect.i4_left;
        pt_indicator_rect->i4_right = pt_indicator_rect->i4_left + i4_size;
    
        pt_indicator_rect->i4_top = ((WGL_RECT_GET_HEIGHT(&t_bar_rect) - ui2_font_height) >> 1) + t_bar_rect.i4_top;
        pt_indicator_rect->i4_bottom = pt_indicator_rect->i4_top + ui2_font_height;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_indicator_loc(
    WGL_PG_CONTENT_T*                     pt_cnt_data,
    WGL_PG_CNT_IDR_LOC_TYPE_T             e_idr_type,
    WGL_PG_IDR_LOC_T*                     pt_idr_loc)
{
    WGL_PG_IDR_LOC_T                      t_empty = {{0}, 0};
    
    switch (e_idr_type)
    {
    case WGL_PG_CNT_ILT_MAX:
        {
            if(pt_idr_loc)
            {
                if(WGL_IS_VALID_ALIGN(pt_idr_loc->ui1_align))
                {
                    pt_cnt_data->t_max_loc = *pt_idr_loc;
                    WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MAX_LOC);
                    
                    WGL_RECT_NORMALIZE(&pt_cnt_data->t_max_loc.t_loc_rect);
                }
                else
                {
                    DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_SET_IDR_LOC command, 2st argument is invalid. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG;
                }
            }
            else
            {
                pt_cnt_data->t_max_loc = t_empty;
                WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MAX_LOC);
            }
        }       
        break;
        
    case WGL_PG_CNT_ILT_MIN:
        {
            if(pt_idr_loc)
            {
                if(WGL_IS_VALID_ALIGN(pt_idr_loc->ui1_align))
                {
                    pt_cnt_data->t_min_loc = *pt_idr_loc;
                    WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MIN_LOC);
                    
                    WGL_RECT_NORMALIZE(&pt_cnt_data->t_min_loc.t_loc_rect);
                }
                else
                {
                    DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_SET_IDR_LOC command, 2st argument is invalid. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG;
                }
            }
            else
            {
                pt_cnt_data->t_min_loc = t_empty;
                WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MIN_LOC);
            }
        }
        break;
        
    case WGL_PG_CNT_ILT_POS:
        {
            if(pt_idr_loc)
            {
                if(WGL_IS_VALID_ALIGN(pt_idr_loc->ui1_align))
                {
                    pt_cnt_data->t_pos_loc = *pt_idr_loc;
                    WGL_SET_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_POS_LOC);
                    
                    WGL_RECT_NORMALIZE(&pt_cnt_data->t_pos_loc.t_loc_rect);
                }
                else
                {
                    DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_SET_IDR_LOC command, 2st argument is invalid. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG;
                }    
            }
            else
            {
                pt_cnt_data->t_pos_loc = t_empty;
                WGL_CLEAR_FLAG(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_POS_LOC);
            }
        }
        break;
        
    default:
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_SET_IDR_LOC command, 1st argument is undefined. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_get_indicator_loc(
    WGL_PG_CONTENT_T*                     pt_cnt_data,
    WGL_PG_CNT_IDR_LOC_TYPE_T             e_idr_type,
    WGL_PG_IDR_LOC_T*                     pt_idr_loc)
{
    WGL_PG_IDR_LOC_T                      t_empty = {{0}, 0};
    
    if(NULL == pt_idr_loc)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_GET_IDR_LOC command, 2st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }        
    
    switch (e_idr_type)
    {
    case WGL_PG_CNT_ILT_MAX:
        {
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MAX_LOC))
            {
                *pt_idr_loc = pt_cnt_data->t_max_loc;
            }
            else
            {
                *pt_idr_loc = t_empty;
            }
        }       
        break;
        
    case WGL_PG_CNT_ILT_MIN:
        {
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_MIN_LOC))
            {
                *pt_idr_loc = pt_cnt_data->t_min_loc;
            }
            else
            {
                *pt_idr_loc = t_empty;
            }
        }
        break;
        
    case WGL_PG_CNT_ILT_POS:
        {
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USER_DEF_POS_LOC))
            {
                *pt_idr_loc = pt_cnt_data->t_pos_loc;
            }
            else
            {
                *pt_idr_loc = t_empty;
            }
        }
        break;
        
    default:
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. For WGL_CNT_PG_GET_IDR_LOC command, 1st argument is undefined. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}     

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static VOID _wgl_pg_cnt_update_range(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    INT32                               i4_min,
    INT32                               i4_max)
{
    /*local variable declaration*/
    WGL_PG_LEVELS_INFO_T                t_level_info = {0};
    INT32                               i4_index,i4_value;
    UINT8                               ui1_copy_index;
    INT32                               i4_ret;

    /*keep new min and max*/
    pt_cnt_data->i4_min = i4_min;
    pt_cnt_data->i4_max = i4_max;
    
    _PG_TURN_ON_RANGE_DIRTY(pt_cnt_data);

    if (pt_cnt_data->ui1_levels > 5)
    {
        return;
    }

    /*check level boundary*/
    for(i4_index = 1, ui1_copy_index = 0; i4_index < pt_cnt_data->ui1_levels; i4_index++)
    {		
        i4_value = pt_cnt_data->a_boundary[i4_index - 1];
        if( i4_value > i4_min && i4_value < i4_max )
        {
            t_level_info.a_boundary[ui1_copy_index++] = i4_value;
        }
    }

    t_level_info.ui1_level_number = ui1_copy_index + (UINT8)1;

    /*update levels info*/
    i4_ret = _wgl_pg_cnt_set_levels(pt_cnt_data, &t_level_info);
    WGL_ASSERT(WGLR_OK == i4_ret);
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_set_bar_inset(
    WGL_PG_CONTENT_T*                   pt_cnt_data,
    const WGL_INSET_T*                  pt_bar_inset)
{
    /*local variable declaration*/
    GL_RECT_T                           t_body_rect;

    if(NULL == pt_bar_inset)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(PG_CNT)} bar-inset(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
            pt_bar_inset,
            pt_bar_inset->i4_left,
            pt_bar_inset->i4_top,
            pt_bar_inset->i4_right,
            pt_bar_inset->i4_bottom));

    WGL_RECT_COPY(&t_body_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_body_rect, 
                   pt_cnt_data->t_body_inset.i4_left, 
                   pt_cnt_data->t_body_inset.i4_top,
                   pt_cnt_data->t_body_inset.i4_right, 
                   pt_cnt_data->t_body_inset.i4_bottom);

    if(FALSE == wgl_is_legal_inset(&t_body_rect, pt_bar_inset))
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The input inset is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
                
    WGL_COPY_INSET(&pt_cnt_data->t_bar_inset, pt_bar_inset);
    return WGLR_OK;
}

#if 0
static INT32 _wgl_pg_cnt_set_cursor_w_h(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const GL_SIZE_T*                pt_cursor_size)
{
    /*local variable declaration*/
    GL_RECT_T                           t_body_rect;
    if(NULL == pt_cursor_size)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    DBG_INFO(("{WGL(PG_CNT)} point_size(w, h) = 0x%.8x (%d, %d,)\n",
        pt_cursor_size,
        pt_cursor_size->ui4_width,
        pt_cursor_size->ui4_height));
    
    WGL_RECT_COPY(&t_body_rect, &pt_cnt_data->t_cnt_rect);
    if (   pt_cursor_size->ui4_height > t_body_rect.i4_bottom - t_body_rect.i4_top
        || pt_cursor_size->ui4_width  > t_body_rect.i4_right  - t_body_rect.i4_left
        || pt_cursor_size->ui4_height == 0
        || pt_cursor_size->ui4_width == 0)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The input size is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_cnt_data->t_size_cursor.ui4_height = pt_cursor_size->ui4_height;

    pt_cnt_data->t_size_cursor.ui4_width  = pt_cursor_size->ui4_width;

    return WGLR_OK;
}


static INT32 _wgl_pg_cnt_get_cursor_w_h(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    const GL_SIZE_T*                pt_cursor_size)
{
     if(NULL == pt_cursor_size)
     {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Invalid arguments. The input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
     }

     pt_cursor_size->ui4_height = pt_cnt_data->t_size_cursor.ui4_height;
     pt_cursor_size->ui4_width  = pt_cnt_data->t_size_cursor.ui4_width;
     return WGLR_OK;
}
#endif
/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_prepare_img(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target)
{
    /*local variable declaration*/
    WGL_IMG_STD_SET_T*              pt_action_img;
    UINT16                          ui2_enable_index;
    UINT16                          ui2_disable_index;
    UINT16                          ui2_highlight_index;
    INT32                           i4_ret;
    WGL_IMG_ST_T                    e_segment_type;
    UINT32                          ui4_width;
    UINT32                          ui4_height;
    WGL_HIMG_TPL_T                  h_tpl_img;

    /*select source*/
    switch (ui1_target)
    {
    case WGL_IMG_BK:
        {
            pt_action_img = &pt_cnt_data->t_img_bk;
            ui2_enable_index = WGL_SYS_IMG_PG_BK;
            ui2_disable_index = WGL_SYS_IMG_PG_BK_DIS;
            ui2_highlight_index = WGL_SYS_IMG_PG_BK_HT;
        }
        break;

    case WGL_IMG_FG:
    case WGL_IMG_PG_BODY:
        {
            pt_action_img = &pt_cnt_data->t_img_body;
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_BODY;
                ui2_disable_index = WGL_SYS_IMG_PG_BODY_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_BODY_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_BODY;
                ui2_disable_index = WGL_SYS_IMG_PG_V_BODY_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_BODY_HT;
            }
        }
        break;

    case WGL_IMG_PG_LEVEL_1:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[0];
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_LEVEL1;
                ui2_disable_index = WGL_SYS_IMG_PG_LEVEL1_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_LEVEL1_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_LEVEL1;
                ui2_disable_index = WGL_SYS_IMG_PG_V_LEVEL1_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_LEVEL1_HT;
            }
        }
        break;

    case WGL_IMG_PG_LEVEL_2:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[1];
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_LEVEL2;
                ui2_disable_index = WGL_SYS_IMG_PG_LEVEL2_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_LEVEL2_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_LEVEL2;
                ui2_disable_index = WGL_SYS_IMG_PG_V_LEVEL2_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_LEVEL2_HT;
            }
        }
        break;

    case WGL_IMG_PG_LEVEL_3:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[2];
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_LEVEL3;
                ui2_disable_index = WGL_SYS_IMG_PG_LEVEL3_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_LEVEL3_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_LEVEL3;
                ui2_disable_index = WGL_SYS_IMG_PG_V_LEVEL3_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_LEVEL3_HT;
            }
        }
        break;

    case WGL_IMG_PG_LEVEL_4:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[3];
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_LEVEL4;
                ui2_disable_index = WGL_SYS_IMG_PG_LEVEL4_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_LEVEL4_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_LEVEL4;
                ui2_disable_index = WGL_SYS_IMG_PG_V_LEVEL4_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_LEVEL4_HT;
            }
        }
        break;

    case WGL_IMG_PG_LEVEL_5:
        {
            pt_action_img = &pt_cnt_data->a_bar_img[4];
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_LEVEL5;
                ui2_disable_index = WGL_SYS_IMG_PG_LEVEL5_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_LEVEL5_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_LEVEL5;
                ui2_disable_index = WGL_SYS_IMG_PG_V_LEVEL5_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_LEVEL5_HT;
            }
        }
        break;
    case WGL_IMG_PG_CURSOR:
        {
            pt_action_img = &pt_cnt_data->t_img_cursor;
            if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
            {
                ui2_enable_index = WGL_SYS_IMG_PG_CURSOR;
                ui2_disable_index = WGL_SYS_IMG_PG_CURSOR_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_CURSOR_HT;
            }
            else
            {
                ui2_enable_index = WGL_SYS_IMG_PG_V_CURSOR;
                ui2_disable_index = WGL_SYS_IMG_PG_V_CURSOR_DIS;
                ui2_highlight_index = WGL_SYS_IMG_PG_V_CURSOR_HT;
            }
        }
        break;
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown image target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }

    /*get system enable image*/
    if((NULL_HANDLE == pt_action_img->t_enable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(ui2_enable_index, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system image (ID=%d). (%d) at L%d\n\n", ui2_enable_index, i4_ret, __LINE__));
            return i4_ret;
        }

        pt_action_img->t_enable = h_tpl_img;
    }

    /*get system disable image*/
    if((NULL_HANDLE == pt_action_img->t_disable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(ui2_disable_index, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system image (ID=%d). (%d) at L%d\n\n", ui2_disable_index, i4_ret, __LINE__));
            return i4_ret;
        }

        pt_action_img->t_disable = h_tpl_img;
    }

    /*get system highlight image*/
    if((NULL_HANDLE == pt_action_img->t_highlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(ui2_highlight_index, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system image (ID=%d). (%d) at L%d\n\n", ui2_highlight_index, i4_ret, __LINE__));
            return i4_ret;
        }

        pt_action_img->t_highlight = h_tpl_img;
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_use_sys_color(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_target)
{
    /*local variable declaration*/
    WGL_COLOR_STD_SET_T*            pt_action_clr;
    UINT16                          ui2_enable_index;
    UINT16                          ui2_disable_index;
    UINT16                          ui2_highlight_index;
    INT32                           i4_ret;
    
    /*select source*/
    switch (ui1_target)
    {
    case WGL_CLR_BK:
        {
            pt_action_clr = &pt_cnt_data->t_clr_bk;
            ui2_enable_index = WGL_SYS_CLR_PG_BK;
            ui2_disable_index = WGL_SYS_CLR_PG_BK_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_BK_HT;
        }
        break;

    case WGL_CLR_TEXT:
        {
            pt_action_clr = &pt_cnt_data->t_clr_text;
            ui2_enable_index = WGL_SYS_CLR_TEXT;
            ui2_disable_index = WGL_SYS_CLR_TEXT_DIS;
            ui2_highlight_index = WGL_SYS_CLR_TEXT_HT;
        }
        break;

    case WGL_CLR_PG_BODY:
        {
            pt_action_clr = &pt_cnt_data->t_clr_body;
            ui2_enable_index = WGL_SYS_CLR_PG_BODY;
            ui2_disable_index = WGL_SYS_CLR_PG_BODY_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_BODY_HT;
        }
        break;

    case WGL_CLR_PG_LEVEL_1:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[0];
            ui2_enable_index = WGL_SYS_CLR_PG_LEVEL1;
            ui2_disable_index = WGL_SYS_CLR_PG_LEVEL1_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_LEVEL1_HT;
        }
        break;

    case WGL_CLR_PG_LEVEL_2:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[1];
            ui2_enable_index = WGL_SYS_CLR_PG_LEVEL2;
            ui2_disable_index = WGL_SYS_CLR_PG_LEVEL2_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_LEVEL2_HT;
        }
        break;

    case WGL_CLR_PG_LEVEL_3:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[2];         
            ui2_enable_index = WGL_SYS_CLR_PG_LEVEL3;
            ui2_disable_index = WGL_SYS_CLR_PG_LEVEL3_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_LEVEL3_HT;
        }
        break;

    case WGL_CLR_PG_LEVEL_4:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[3];
            ui2_enable_index = WGL_SYS_CLR_PG_LEVEL4;
            ui2_disable_index = WGL_SYS_CLR_PG_LEVEL4_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_LEVEL4_HT;
        }
        break;

    case WGL_CLR_PG_LEVEL_5:
        {
            pt_action_clr = &pt_cnt_data->a_bar_clr[4];
            ui2_enable_index = WGL_SYS_CLR_PG_LEVEL5;
            ui2_disable_index = WGL_SYS_CLR_PG_LEVEL5_DIS;
            ui2_highlight_index = WGL_SYS_CLR_PG_LEVEL5_HT;
        }
        break;
        
    default:
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Unknown color target. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
            return WGLR_NOT_SUPPORT_TARGET;
        }
    }

    /*get system enable color*/
    i4_ret = wgl_get_system_color(ui2_enable_index, &pt_cnt_data->t_cvsst, &pt_action_clr->t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system color (ID=%d). (%d) at L%d\n\n", ui2_enable_index, i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color*/
    i4_ret = wgl_get_system_color(ui2_disable_index, &pt_cnt_data->t_cvsst, &pt_action_clr->t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system color (ID=%d). (%d) at L%d\n\n", ui2_disable_index, i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color*/
    i4_ret = wgl_get_system_color(ui2_highlight_index, &pt_cnt_data->t_cvsst, &pt_action_clr->t_highlight);
    WGL_PG_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system color (ID=%d). (%d) at L%d\n\n", ui2_highlight_index, i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_calc_indicator(
    INT32                               i4_value,
    INT32                               i4_min,
    INT32                               i4_max,
    BOOL                                b_percent,
    HFONT_T                             h_font,
    UTF16_T                             a_utf16_buf[PG_IDR_MAX_CHAR_LENGTH],
    UINT8*                              pui1_length,
    UINT16*                             pui2_width,
    UINT16*                             pui2_height)
{
    /*local variable declaration*/
    CHAR                        a_chr_buf[PG_IDR_MAX_CHAR_LENGTH] = {0};
    INT32                       i4_width;
    INT32                       i4_height;
    INT32                       i4_ret;
    UINT32                      i4_length;
    
    /*clean buffer*/
    x_memset(a_utf16_buf, 0, PG_IDR_MAX_CHAR_LENGTH * sizeof(UTF16_T));

    /*get string*/
    if(b_percent)
    {
        /*i4_ret = x_sprintf(a_chr_buf, "%d%", i4_value / i4_total);*/
        x_sprintf(a_chr_buf, "%d%%", ((i4_value - i4_min) * 100) / (i4_max - i4_min));
    }
    else
    {
        /*i4_ret = x_sprintf(a_chr_buf, "%d", i4_value);*/
        x_sprintf(a_chr_buf, "%d", i4_value);
    }

    /*
    if(OSR_OK != i4_ret)
    {
        return WGLR_INTERNAL_ERROR;
    }*/

    /*get string length*/
    i4_length = x_strlen (a_chr_buf);
    WGL_ASSERT(i4_length <= 0x000000FF);
    *pui1_length = (UINT8)i4_length;
    
    /*convert ot UTF16*/
    i4_ret = x_uc_ps_to_w2s(a_chr_buf, a_utf16_buf, PG_IDR_MAX_CHAR_LENGTH);
    if(UCR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to convert to UTF16. (UCR=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*get width*/
    i4_ret = x_fe_get_string_size(h_font, a_utf16_buf, -1,  &i4_width, &i4_height);
    if(FER_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get string size. (FER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
    
    WGL_ASSERT(i4_width <= 0x0000FFFF);
    WGL_ASSERT(i4_height <= 0x0000FFFF);
    
    *pui2_height = (UINT16)i4_height;
    *pui2_width = (UINT16)i4_width;

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_ready_bar_img(
    WGL_PG_CONTENT_T*                  pt_data,
    UINT16                             ui2_state,
    WGL_HIMG_TPL_T                     ah_img_levels[WGL_PG_LEVEL_MAX])
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_img_tmp;
    WGL_IMG_STD_SET_T*                  pt_cur_img;
    INT32                               i4_ret, i4_index, i4_count;
    UINT8                               a_ui1_id[] = {WGL_IMG_PG_LEVEL_1, 
                                                      WGL_IMG_PG_LEVEL_2, 
                                                      WGL_IMG_PG_LEVEL_3, 
                                                      WGL_IMG_PG_LEVEL_4, 
                                                      WGL_IMG_PG_LEVEL_5};
    UINT64                              a_ui8_flag[] = {PG_LEVLE1_IMG_NOT_INIT, 
                                                        PG_LEVLE2_IMG_NOT_INIT, 
                                                        PG_LEVLE3_IMG_NOT_INIT, 
                                                        PG_LEVLE4_IMG_NOT_INIT, 
                                                        PG_LEVLE5_IMG_NOT_INIT};
    
    /*check and cache images*/
    if (pt_data->ui1_levels > 5)
    {
        return WGLR_INTERNAL_ERROR;
    }
    i4_count = pt_data->ui1_levels;
    for(i4_index = 0; i4_index < i4_count; i4_index++)
    {
		if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, a_ui8_flag[i4_index]))
        {
            i4_ret = _wgl_pg_cnt_prepare_img(pt_data, a_ui1_id[i4_index]);
            if(WGLR_OK != i4_ret)
            {
                return WGLR_OK; 
            }
            
            /*remove the flag for level image*/
            WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, a_ui8_flag[i4_index]);
        }
    }

    /*prepare level images*/
    for(i4_index = 0; i4_index < i4_count; i4_index++)
    {		
        /*visit next level*/
        pt_cur_img = &pt_data->a_bar_img[i4_index];

        /*select level image*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_img_tmp = pt_cur_img->t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            h_img_tmp = pt_cur_img->t_enable;
        }
        else
        {
            h_img_tmp = pt_cur_img->t_disable;
        }

#if 0 /*for CR 2584*/            
        /*check whether selected level image exists*/
        if(NULL_HANDLE == h_img_tmp)
        {
            h_img_tmp = WGL_STD_GET_NON_NULL_IMG(pt_cur_img);
        }
        if(NULL_HANDLE == h_img_tmp)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Template image for the bar is NULL. (%d) at L%d\n\n", WGLR_NO_IMAGE, __LINE__));
            return WGLR_NO_IMAGE;
        }
#endif        

        /*keep level image*/
        ah_img_levels[i4_index] = h_img_tmp;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_ready_bar_clr(
    WGL_PG_CONTENT_T*                  pt_data,
    UINT16                             ui2_state,
    GL_COLOR_T*                        apt_clr_levels[WGL_PG_LEVEL_MAX])
{
    /*local variable declaration*/
    GL_COLOR_T*                         pt_clr;
    WGL_COLOR_STD_SET_T*                pt_cur_set;
    INT32                               i4_ret, i4_index, i4_count;
    UINT8                               a_ui1_id[] = {WGL_CLR_PG_LEVEL_1, 
                                                      WGL_CLR_PG_LEVEL_2, 
                                                      WGL_CLR_PG_LEVEL_3, 
                                                      WGL_CLR_PG_LEVEL_4, 
                                                      WGL_CLR_PG_LEVEL_5};
    UINT64                              a_ui8_flag[] = {PG_LEVEL1_CLR_NOT_INIT, 
                                                        PG_LEVEL2_CLR_NOT_INIT, 
                                                        PG_LEVEL3_CLR_NOT_INIT, 
                                                        PG_LEVEL4_CLR_NOT_INIT, 
                                                        PG_LEVEL5_CLR_NOT_INIT};

    /*check and cache color*/
    if (pt_data->ui1_levels > 5)
    {
        return WGLR_INTERNAL_ERROR;
    }
    i4_count = pt_data->ui1_levels;
    for(i4_index = 0; i4_index < i4_count; i4_index++)
    {
		
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui8_extra_flag, a_ui8_flag[i4_index]))
        {
            i4_ret = _wgl_pg_cnt_use_sys_color(pt_data, a_ui1_id[i4_index]);
            if(WGLR_OK != i4_ret)
            {
                return WGLR_OK; 
            }
            
            /*remove the flag for level color*/
            WGL_CLEAR_FLAG(pt_data->ui8_extra_flag, a_ui8_flag[i4_index]);
        }
	}
 

    /*prepare level colors*/
    for(i4_index = 0; i4_index < i4_count; i4_index++)
    {	

        /*visit next level*/
        pt_cur_set = &pt_data->a_bar_clr[i4_index];

        /*select level color*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            pt_clr = &pt_cur_set->t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            pt_clr = &pt_cur_set->t_enable;
        }
        else
        {
            pt_clr = &pt_cur_set->t_disable;
        }

        /*keep level color*/
        apt_clr_levels[i4_index] = pt_clr;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static VOID _wgl_pg_cnt_offset_idr_rect(
    const WGL_PG_IDR_LOC_T*              pt_idr_loc,
    INT32                                i4_idr_width,
    INT32                                i4_idr_height,
    GL_RECT_T*                           pt_idr_rect)
{
    if(WGL_RECT_IS_EMPTY(&pt_idr_loc->t_loc_rect) || 0 >= i4_idr_width || 0 >= i4_idr_height)
    {
        WGL_RECT_EMPTY(pt_idr_rect);
        return;
    }
    else
    {
        if(WGL_IS_H_RIGHT(pt_idr_loc->ui1_align))
        {
            pt_idr_rect->i4_right = pt_idr_loc->t_loc_rect.i4_right;
            pt_idr_rect->i4_left = pt_idr_rect->i4_right - i4_idr_width;
        }
        else if(WGL_IS_H_CENTER(pt_idr_loc->ui1_align))
        {
            pt_idr_rect->i4_left = pt_idr_loc->t_loc_rect.i4_left + ((WGL_RECT_GET_WIDTH(&pt_idr_loc->t_loc_rect) - i4_idr_width) >> 1);
            pt_idr_rect->i4_right = pt_idr_rect->i4_left + i4_idr_width;
        }
        else /*left*/
        {
            pt_idr_rect->i4_left = pt_idr_loc->t_loc_rect.i4_left;
            pt_idr_rect->i4_right = pt_idr_rect->i4_left + i4_idr_width;
        }
        
        if(WGL_IS_V_BOTTOM(pt_idr_loc->ui1_align))
        {
            pt_idr_rect->i4_bottom = pt_idr_loc->t_loc_rect.i4_bottom;
            pt_idr_rect->i4_top = pt_idr_rect->i4_bottom - i4_idr_height;
        }
        else if(WGL_IS_V_CENTER(pt_idr_loc->ui1_align))
        {
            pt_idr_rect->i4_top = pt_idr_loc->t_loc_rect.i4_top + ((WGL_RECT_GET_HEIGHT(&pt_idr_loc->t_loc_rect) - i4_idr_height) >> 1);
            pt_idr_rect->i4_bottom = pt_idr_rect->i4_top + i4_idr_height;
        }
        else /*top*/
        {
            pt_idr_rect->i4_top = pt_idr_loc->t_loc_rect.i4_top;
            pt_idr_rect->i4_bottom = pt_idr_rect->i4_top + i4_idr_width;
        }
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_pg_cnt_select_text_font(
    WGL_PG_CONTENT_T*                 pt_cnt_data, 
    UINT16                            ui2_state,
    HANDLE_T*                         pt_selected_font,
    WGL_FONT_INFO_T*                  pt_selected_font_info)
{
    /*local variable declaration*/
    INT32                             i4_ret;
    WGL_FONT_INFO_T                   t_font_info;
    
    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui8_extra_flag, PG_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(PG_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_pg_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    /*select color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        *pt_selected_font = pt_cnt_data->t_text_focus_font;
        *pt_selected_font_info = pt_cnt_data->t_focus_font_info;
    }
    else
    {
        *pt_selected_font = pt_cnt_data->t_text_font;
        *pt_selected_font_info = pt_cnt_data->t_font_info;
    }

    return WGLR_OK;
}
static INT32 _wgl_pg_cnt_cal_cursor_pos(
    WGL_PG_CONTENT_T*               pt_cnt_data,
    GL_RECT_T*                      pt_up_bar_rect,
    INT32*                          pi4_x_offset,
    INT32*                          pi4_y_offset)
{
    GL_RECT_T t_bar_rect = {0};
    GL_SIZE_T t_cursor_size;
    if (pt_cnt_data == NULL ||pi4_x_offset == NULL || pi4_y_offset == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
    *pi4_x_offset = 0;
    *pi4_y_offset = 0;
    
    t_bar_rect.i4_left = pt_cnt_data->t_cnt_rect.i4_left;
    t_bar_rect.i4_top  = pt_cnt_data->t_cnt_rect.i4_top;
    t_bar_rect.i4_right = pt_cnt_data->t_cnt_rect.i4_right;
    t_bar_rect.i4_bottom = pt_cnt_data->t_cnt_rect.i4_bottom;
    t_cursor_size = pt_cnt_data->t_size_cursor;
    
    WGL_RECT_INSET( &t_bar_rect, 
                    pt_cnt_data->t_body_inset.i4_left,
                    pt_cnt_data->t_body_inset.i4_top,
                    pt_cnt_data->t_body_inset.i4_right,
                    pt_cnt_data->t_body_inset.i4_bottom);

    if (pt_cnt_data->i4_cur_pos > pt_cnt_data->i4_max
        && pt_cnt_data->i4_cur_pos < pt_cnt_data->i4_min)
    {
        pt_cnt_data->i4_cur_pos = pt_cnt_data->i4_min;
    }
    if(_PG_IS_HORIZTONAL(pt_cnt_data->ui8_extra_flag))
    {
        *pi4_x_offset = t_bar_rect.i4_left +                                            \
            (INT32)((DOUBLE)(pt_cnt_data->i4_cur_pos - pt_cnt_data->i4_min)*                            \
            ((DOUBLE)(t_bar_rect.i4_right - t_bar_rect.i4_left - t_cursor_size.ui4_width)/(DOUBLE)(pt_cnt_data->i4_max - pt_cnt_data->i4_min)));


        if (pt_cnt_data->t_cnt_rect.i4_bottom - pt_cnt_data->t_cnt_rect.i4_top 
            < (INT32)pt_cnt_data->t_size_cursor.ui4_height)
        {
            pt_cnt_data->t_size_cursor.ui4_height = (UINT32)(t_bar_rect.i4_bottom - t_bar_rect.i4_top);
        }
        *pi4_y_offset = t_bar_rect.i4_top + 
            (INT32)((t_bar_rect.i4_bottom - t_bar_rect.i4_top - (INT32)pt_cnt_data->t_size_cursor.ui4_height )/2);
        if (pt_up_bar_rect != NULL)
        {
            pt_up_bar_rect->i4_right = *pi4_x_offset;
        }
    }
    else
    {
        if (_PG_IS_VERTICAL_UP(pt_cnt_data->ui8_extra_flag))
        {
            *pi4_y_offset = t_bar_rect.i4_top +                                            \
                (INT32)((DOUBLE)(pt_cnt_data->i4_max - pt_cnt_data->i4_cur_pos)*                            \
                ((DOUBLE)(t_bar_rect.i4_bottom- t_bar_rect.i4_top - t_cursor_size.ui4_height)/(DOUBLE)(pt_cnt_data->i4_max - pt_cnt_data->i4_min)));

            if (pt_cnt_data->t_cnt_rect.i4_right- pt_cnt_data->t_cnt_rect.i4_left
                < (INT32)pt_cnt_data->t_size_cursor.ui4_height)
            {
                pt_cnt_data->t_size_cursor.ui4_height = (UINT32)(pt_cnt_data->t_cnt_rect.i4_right- pt_cnt_data->t_cnt_rect.i4_left);
            }
            *pi4_x_offset = t_bar_rect.i4_left + 
                (INT32)((t_bar_rect.i4_right - t_bar_rect.i4_left - (INT32)pt_cnt_data->t_size_cursor.ui4_width )/2);

            if (pt_up_bar_rect != NULL)
            {
                pt_up_bar_rect->i4_top = *pi4_y_offset;
            }
        }
        else
        {
            *pi4_y_offset = t_bar_rect.i4_top +                                            \
                (INT32)((DOUBLE)(pt_cnt_data->i4_cur_pos - pt_cnt_data->i4_min)*                            \
                ((DOUBLE)(t_bar_rect.i4_bottom- t_bar_rect.i4_top - t_cursor_size.ui4_height)/(DOUBLE)(pt_cnt_data->i4_max - pt_cnt_data->i4_min)));

            if (pt_cnt_data->t_cnt_rect.i4_right- pt_cnt_data->t_cnt_rect.i4_left
                < (INT32)pt_cnt_data->t_size_cursor.ui4_height)
            {
                pt_cnt_data->t_size_cursor.ui4_height =(UINT32)( pt_cnt_data->t_cnt_rect.i4_right- pt_cnt_data->t_cnt_rect.i4_left);
            }
            *pi4_x_offset = t_bar_rect.i4_left + 
                (INT32)((t_bar_rect.i4_right - t_bar_rect.i4_left - (INT32)pt_cnt_data->t_size_cursor.ui4_width)/2);

            if (pt_up_bar_rect != NULL)
            {
                pt_up_bar_rect->i4_bottom = *pi4_y_offset;
            }
        }
    }
    return WGLR_OK;
}


