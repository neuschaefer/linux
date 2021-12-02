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
 * $RCSfile: wgl_text_cnt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 34b66dc8d906a6e8eec22c80161c4825 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_sets.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_image.h"
#include "font/x_fe.h"
#include "unicode/x_uc_str.h"
#include "wgl/text/wgl_text_cnt.h"
#include "wgl/text/wgl_text_cli.h"
#include "wgl/arabic.h"
#include "wgl/arabic.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_text_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_LEVEL_ERROR
#endif

/*Extra Flag*/
#define TEXT_BK_CLR_NOT_INIT            ((UINT32)0x00000001)
#define TEXT_BK_IMG_NOT_INIT            ((UINT32)0x00000002)
#define TEXT_FONT_NOT_INIT              ((UINT32)0x00000004)
#define TEXT_TEXT_CLR_NOT_INIT          ((UINT32)0x00000008)
#define TEXT_IMG_UI                     ((UINT32)0x00000010)
#define TEXT_MULTILINE                  ((UINT32)0x00000020)
#define TEXT_SMART_CUT                  ((UINT32)0x00000040)
#define TEXT_MAX_32                     ((UINT32)0x00000080)
#define TEXT_MAX_128                    ((UINT32)0x00000100)
#define TEXT_MAX_512                    ((UINT32)0x00000200)
#define TEXT_MAX_2048                   ((UINT32)0x00000400)
#define TEXT_MAX_4096                   ((UINT32)0x00000800)
#define TEXT_NO_BK                      ((UINT32)0x00001000)
#define TEXT_TOTAL_LINES_NOT_INIT       ((UINT32)0x00002000)
#define TEXT_SMART_CUT_OVER_MAX         ((UINT32)0x00004000)
#define TEXT_USE_SYS_IMG                ((UINT32)0x00008000)
#define TEXT_NOT_CLEAN_BK               ((UINT32)0x00010000)
#define TEXT_EDGE_CLR_NOT_INIT          ((UINT32)0x00020000)
#define TEXT_END_OF_NEWLINE             ((UINT32)0x00040000)
#define TEXT_USER_DEF_MAX_LINE          ((UINT32)0x00080000)
#define TEXT_IGNORE_NEWLINE             ((UINT32)0x00100000)
#define TEXT_SCROLLING                  ((UINT32)0x00200000)
#define TEXT_MAX_32768                  ((UINT32)0x00400000)
#define TEXT_FIX_DIR                    ((UINT32)0x00800000)
#define TEXT_FIX_DIR_ALIGN              ((UINT32)0x01000000)
#define TEXT_MULTISTRING                ((UINT32)0x02000000)
#define TEXT_INTERLACE                  ((UINT32)0x04000000)
#define TEXT_STRING_POSITION_BY_IDX     ((UINT32)0x08000000)
#define TEXT_MS_NO_SMARTCUT             ((UINT32)0x10000000)
#define TEXT_TEXT_BG_CLR_NOT_INIT       ((UINT32)0x20000000)
    
/*overflow state*/
#define TEXT_OVERFLOW_RIGHT             ((UINT16)0x0001)
#define TEXT_OVERFLOW_LEFT              ((UINT16)0x0002)
/*max line for display*/
#define TEXT_MAX_TOTAL_LINE             ((UINT8)255)
/*max line capability for processing v-center alignment*/
#define TEXT_MAX_DIS_LINE               (UINT8)40)

#define TEXT_SMART_CUT_MAX_LENGHT       ((UINT16)5)
#define TEXT_SMART_CUT_CHARS            "..."

#define TEXT_BREAK_CHARS_MAX_LENGHT       ((UINT8)4)

#define _TEXT_CONVERT_MAX_FLAG(flag)                                                                                    \
    ((UINT32)((WGL_STL_TEXT_MAX_32768 & (flag)) ?                                                                        \
                 TEXT_MAX_32768 : ((WGL_STL_TEXT_MAX_4096 & (flag)) ?                                                     \
                TEXT_MAX_4096 : ((WGL_STL_TEXT_MAX_2048 & (flag)) ?                                                     \
                                        TEXT_MAX_2048 : ((WGL_STL_TEXT_MAX_512 & (flag)) ?                              \
                                                                TEXT_MAX_512 : ((WGL_STL_TEXT_MAX_128 & (flag)) ?       \
                                                                                 TEXT_MAX_128 : TEXT_MAX_32))))))     \

#define _TEXT_GET_MAX_LENGTH(flag)                                              \
    ((UINT32)((TEXT_MAX_32768 & (flag)) ?                                        \
                    32768 : ((TEXT_MAX_4096 & (flag)) ?                          \
                    4096 : ((TEXT_MAX_2048 & (flag)) ?                          \
                                2048 : ((TEXT_MAX_512 & (flag)) ?               \
                                            512 : ((TEXT_MAX_128 & (flag)) ?    \
                                                       128 : 32))))))           \

typedef struct _WGL_TEXT_LINE_INFO
{
    UTF16_T*                        w2s_string;
    UINT16                          ui2_width;
    UINT16                          ui2_str_len;
    UINT16                          ui2_str_idx;
} WGL_TEXT_LINE_INFO;

typedef struct _WGL_TEXT_MULTI_FOCUS_STYLE
{
    WGL_FONT_INFO_T                     t_font_info;            /*font information for text*/
    HFONT_T                             t_text_font;            /*font for text*/
}WGL_TEXT_MULTI_FOCUS_STYLE;

typedef struct _WGL_TEXT_MULTI_STRING
{
    UTF16_T*                            ps_string;
    UINT16                              ui2_size;
    UINT16                              ui2_lines;
	UTF16_T*							ps_arab_dsp_text;
	UINT16								ui2_arab_dsp_text_len;
    struct _WGL_TEXT_MULTI_STRING*             pt_next;
}WGL_TEXT_MULTI_STRING;

typedef struct _WGL_TEXT_MULTI_STRING_A
{
    WGL_TEXT_MULTI_STRING*              pt_string_element;
    UINT16                              ui2_lines;
}WGL_TEXT_MULTI_STRING_A;

typedef struct _WGL_TEXT_CONTENT_T
{
#ifdef CLI_LVL_ALL     
    UINT32                              ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    UINT8                               ui1_read_direction;     /*text read direction, left-to-right or right-to-left*/
    BOOL                                b_opposite_align;       /*TRUE: when paint, use the opposite alignment*/
    WGL_COLOR_STD_SET_T                 t_clr_bk;               /*background color set*/
    WGL_IMG_STD_SET_T                   t_img_bk;               /*background template image*/
    GL_RECT_T                           t_cnt_rect;             /*content rect*/
    WGL_COLOR_STD_SET_T                 t_clr_text;             /*text color set*/
    WGL_COLOR_STD_SET_T                 t_clr_edge;             /*text color set*/
    WGL_COLOR_STD_SET_T                 t_clr_text_bg;          /*text background color set*/
    WGL_INSET_T                         t_fg_inset;             /*inset for foreground text*/
    WGL_FONT_INFO_T                     t_font_info;            /*font information for text*/
    HFONT_T                             t_text_font;            /*font for text*/
    WGL_FONT_INFO_T                     t_focus_font_info;      /*font information for text*/
    HFONT_T                             t_text_focus_font;      /*font for text*/
    WGL_TEXT_LINE_INFO*                 pt_line_infos;          /*start pointer of the line infos*/
    UINT32                              ui4_extra_flag;         /*Flag*/
    INT32                               i4_line_gap;            /*gap between lines*/
    UINT16                              ui2_start_line;         /*start line to display*/
    BOOL                                b_start_idx_valid;
    UINT16                              ui2_text_length;        /*total length of user string*/
    UINT16                              ui2_total_lines;        /*total line number*/
    UINT16                              ui2_line_capacity;      /*line number for display*/
    UINT16                              ui2_lines_in_display;   /*available lines in display rectangle*/
    UINT16                              ui2_scrl_style;         /*scroll style */    
    UINT16                              ui2_scrl_count;         /*scroll repeat count , o indicates infinite times*/
    UINT16                              ui2_scrl_step;          /*repeat step of scrolling state*/
    UINT16                              ui2_scrl_begin_idx;     /*begin char index of scrolling state*/
    UINT16                              ui2_scrl_len;           /*scroll text length*/
    UINT16                              ui2_scrl_width;         /*scroll text width*/
    UINT16                              ui2_scrl_start_x;       /*scroll text start x*/
    UINT16                              ui2_overflow_flag;      /*overflow state*/ 
    BOOL                                b_is_data_dirty;        /*TRUE: re-layout display*/
    BOOL                                b_more_data;            /*TRUE: there're more data out of the display range*/
    UINT8                               ui1_align;              /*alignment*/    
    UINT8                               ui1_align_custom;       /*alignment*/
    WGL_CANVAS_STATUS_T                 t_cvsst;                /*canvas status*/      
    UTF16_T                             *pt_w2_arab_dsp_text;
    UINT16                              ui2_arab_dsp_text_len;        /*total length of user string*/
    UINT8                               ui1_cur_font_id;
    REORDER_ARRTRIBUTE_T                t_arab_reorder_attr;
    UTF16_T                             *pt_w2_text;           /*the start pos of the string bufer*/
    WGL_TEXT_MULTI_STRING               *pt_multi_string;
    UINT16                              ui2_multi_string_num;
    UINT16                              ui2_interlace;
    UINT16                              ui2_highlight_idx;     /*index of string*/
    UINT16                              ui2_highlight_postion;
    UTF16_T                             as_break_chars[TEXT_BREAK_CHARS_MAX_LENGHT];            
} WGL_TEXT_CONTENT_T;

typedef struct _WGL_REVERSE_LINE_RESULT_T
{
    UTF16_T*        w2s_line_start;
    UINT16          ui2_char_number;
    UINT16          ui2_width;
} WGL_REVERSE__LINE_RESULT_T;


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_text_cnt_paint_bk(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_select_text_clr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_selected_clr,
    GL_COLOR_T*                         pt_edge_clr);

static INT32 _wgl_text_cnt_select_text_bg_clr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_bg_clr);

static INT32 _wgl_text_cnt_select_text_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    HANDLE_T*                           pt_selected_font,
    WGL_FONT_INFO_T*                    pt_selected_font_info);

static INT32 _wgl_text_cnt_paint_fg_multiple_string(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_paint_fg_multiple_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_paint_fg_multiple_line_custom(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_paint_fg_single_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_paint_fg_single_line_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state);


static INT32 _wgl_text_cnt_set_img(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_IMG_INFO_T*               pt_img_info);

static INT32 _wgl_text_cnt_get_img(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_IMG_INFO_T*                     pt_img_info);

static INT32 _wgl_text_cnt_set_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_COLOR_INFO_T*             pt_clr_info);

static INT32 _wgl_text_cnt_get_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_COLOR_INFO_T*                   pt_clr_info);

static INT32 _wgl_text_cnt_set_text(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_length,
    const UTF16_T*                      w2s_str,
    BOOL                                b_append);

static INT32 _wgl_text_cnt_set_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const WGL_FONT_INFO_T*              pt_font_info);

static INT32 _wgl_text_cnt_get_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    WGL_FONT_INFO_T*                    pt_font_info);

static INT32 _wgl_text_cnt_set_font_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_font_info_ex);

static INT32 _wgl_text_cnt_get_font_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    WGL_FONT_INFO_EX_T*                 pt_font_info_ex);

static INT32 _wgl_text_cnt_set_text_dir(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_text_dir);

static INT32 _wgl_text_cnt_get_text_dir(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               *pui1_text_dir);
    
static INT32 _wgl_text_cnt_get_render_cond(
    WGL_TEXT_CONTENT_T*                  pt_cnt_data,
    UINT16                              ui2_state,
    UINT8*                              pui1_render_cond);    
static INT32 _wgl_text_cnt_set_font_attr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    WGL_TEXT_FONT_ATTR_TYPE_T           e_type,
    VOID*                               pv_param);

static INT32 _wgl_text_cnt_set_font_attr_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_TEXT_FONT_ATTR_T*               pt_font_attr);

/*WGL_CMD_TEXT_GET_PREFERRED_SIZE command handler*/
static INT32 _wgl_text_cnt_get_preferred_size(
    WGL_TEXT_CONTENT_T*           pt_cnt_data,
    GL_SIZE_T*                          pt_size);
static INT32 _wgl_text_cnt_get_max_line_width(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state,
    UINT16                               *pui2_text_max_line_width);

/*WGL_CMD_TEXT_GET_MIN_SIZE command handler*/
static INT32 _wgl_text_cnt_get_min_size(
    const WGL_TEXT_CONTENT_T*           pt_cnt_data,
    GL_SIZE_T*                          pt_size);

static INT32 _wgl_text_cnt_prepare_img(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_use_sys_bk_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_use_sys_text_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_use_sys_edge_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_use_sys_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);
static INT32 _wgl_text_cnt_calc_string_lines(
    WGL_TEXT_CONTENT_T*                         pt_cnt_data,
    UINT16                                      ui2_state,
    UTF16_T*                                    ps_string,
    UINT16                                      ui2_string_size,
    UTF16_T*									ps_arab_dsp_text,
    UINT16*										pui2_arab_dsp_text_len,
    UINT16*                                     pui2_total_lines,
    const GL_RECT_T*                            pt_available_rect);

static INT32 _wgl_text_cnt_layout_v_top_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state,
    BOOL                                b_smart_cut);

static INT32 _wgl_text_cnt_layout_v_top(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state);

#if 0
static INT32 _wgl_text_cnt_layout_v_center(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_layout_v_bottom(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);
#endif

static INT32 _wgl_text_cnt_calc_total_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const GL_RECT_T*                    pt_available_rect,
    UINT16                              ui2_state,
    UINT16*                             pui2_total_lines);

static INT32 _wgl_text_cnt_calc_total_line_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const GL_RECT_T*                    pt_available_rect,
    UINT16                              ui2_state,
    UINT16*                             pui2_total_lines);


static INT32 _wgl_text_cnt_calc_page_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const GL_RECT_T*                    pt_available_rect,
    UINT16                              ui2_state,
    UINT16*                             pui2_page_lines);

static void _wgl_text_cnt_reset_dirty_flag(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT32                              ui4_extra_flag);
    
static INT32 _wgl_text_cnt_get_smart_cut_char(
    UTF16_T                             a_w2s_smart_cut_char[TEXT_SMART_CUT_MAX_LENGHT],
    UINT16*                             pui2_smart_cut_len);

static INT32 _wgl_text_cnt_get_smart_cut_info(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    HFONT_T                             h_font,
    const UTF16_T*                      w2s_line_string,
    INT32                               i4_line_width,
    UINT16                              ui2_line_char_len,
    UTF16_T                             a_w2s_smart_cut_char[TEXT_SMART_CUT_MAX_LENGHT],
    UINT16*                             pui2_smart_cut_len,
    INT32*                              pi4_smart_cut_width,
    UINT16*                             pui2_keep_len,
    INT32*                              pi4_keep_width);

static INT32 _wgl_text_cnt_get_max_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16*                             pt_ui2_max_line);


static INT32 _wgl_text_cnt_set_max_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_max_line);

static INT32 _wgl_text_cnt_begin_scrl_check(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_scrl_smart_check(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state);


static VOID _wgl_text_cnt_begin_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    VOID*                               pv_param1);

static VOID _wgl_text_cnt_reset_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);


static VOID _wgl_text_cnt_end_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_do_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state);

static INT32 _wgl_text_cnt_update_arab_text(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data);

static INT32 _wgl_text_cnt_set_string(
    WGL_TEXT_CONTENT_T *                pt_cnt_data,
    UTF16_T *                           pt_string,
    UINT16                              ui2_string_size);

static INT32 _wgl_text_cnt_set_string_prev(
    WGL_TEXT_CONTENT_T *                pt_cnt_data,
    UTF16_T *                           pt_string,
    UINT16                              ui2_string_size);

static INT32 _wgl_text_cnt_remove_string_end(
    WGL_TEXT_CONTENT_T *                pt_cnt_data);

static INT32 _wgl_text_cnt_remove_string_prev(
    WGL_TEXT_CONTENT_T *                pt_cnt_data);

static INT32 _wgl_text_cnt_clear_string(
    WGL_TEXT_CONTENT_T *                pt_cnt_data);

static INT32 _wgl_text_cnt_set_break_chars(
    WGL_TEXT_CONTENT_T * pt_cnt_data,
    UTF16_T *            pt_break_chars,
    UINT8                ui1_break_chars_len);

static INT32 _wgl_text_cnt_get_point_size_from_height(WGL_TEXT_CONTENT_T* pt_cnt_data,
																UINT16	ui2_height,																
																UINT8*  pui1_point_size);



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
INT32 wgl_text_cnt_inst_create( 
    WGL_CONTENT_CONTEXT_HANDLE_T*       ph_cnt_data, 
    WGL_CONTENT_CREATE_INPUT_T*         pt_param)
{
    /*local variable declaration*/
    WGL_TEXT_CONTENT_T*                 pt_data;
    WGL_TEXT_LINE_INFO*                 pt_line_info;
    UTF16_T                             *pt_w2_text;
    UTF16_T                             *pt_w2_arab_dsp_text;
    UINT32                              ui4_flag;
    UINT32                              ui4_byte_num;
    UINT8                               ui1_lines;
    
    DBG_API(("{WGL(TEXT_CNT)} %s: ph_cnt_data = 0x%.8x \tpt_param = 0x%.8x\n", __FUNCTION__, ph_cnt_data, pt_param));

    /*check parameter*/
    if(NULL == ph_cnt_data || NULL == pt_param)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(TEXT_CNT)} pt_param->pv_init = 0x%.8x   ui4_type_border = 0x%.8x\n"
                             "\tt_rect_cnt(l, t, r, b) = (%d, %d, %d, %d)\n",
                             pt_param->pv_init,
                             pt_param->t_rect_cnt.i4_left,
                             pt_param->t_rect_cnt.i4_top,
                             pt_param->t_rect_cnt.i4_right,
                             pt_param->t_rect_cnt.i4_bottom));
    

    /*set length*/
    ui4_flag = _TEXT_CONVERT_MAX_FLAG((UINT32)(pt_param->pv_init));
    ui4_byte_num = (_TEXT_GET_MAX_LENGTH(ui4_flag)+1) * sizeof(UTF16_T);
    
   
    /*get the count of line infos*/
    if((WGL_STL_TEXT_MULTILINE & ((UINT32)(pt_param->pv_init)))
        || (WGL_STL_TEXT_MULTISTRING& ((UINT32)(pt_param->pv_init))))
    {
        if(WGL_CMD_TEXT_SET_MAX_DIS_LINE & ((UINT32)(pt_param->pv_init)))
        {
            ui1_lines = 5;
        }
        if(WGL_STL_TEXT_MAX_DIS_40_LINE & ((UINT32)(pt_param->pv_init)))
        {
            ui1_lines = 40;
        }
        else if(WGL_STL_TEXT_MAX_DIS_20_LINE & ((UINT32)(pt_param->pv_init)))
        {
            ui1_lines = 20;
        }
        else if(WGL_STL_TEXT_MAX_DIS_10_LINE & ((UINT32)(pt_param->pv_init)))
        {
            ui1_lines = 10;
        }
        else
        {
            ui1_lines = 5;
        }
    }
    else
    {
        ui1_lines = 1; /*single line*/
    }

    /*allocate memory for content data*/   
    pt_data = (WGL_TEXT_CONTENT_T*) WGL_MEM_ALLOC(WGL_ALIGN_4(sizeof(WGL_TEXT_CONTENT_T)) +(ui4_byte_num * 2));
    
    if(NULL == pt_data)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to alloc memory for content instance. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    
    /*initialize content data*/
    x_memset(pt_data, 0, WGL_ALIGN_4(sizeof(WGL_TEXT_CONTENT_T)) +(ui4_byte_num * 2));
 
#ifdef CLI_LVL_ALL
     pt_data->ui4_mem_size = WGL_ALIGN_4(sizeof(WGL_TEXT_CONTENT_T)) +(ui4_byte_num * 2);   
#endif
    pt_w2_text = (UTF16_T*)((UINT32)(pt_data) + (UINT32)(WGL_ALIGN_4(sizeof(WGL_TEXT_CONTENT_T))));
    pt_w2_arab_dsp_text = (UTF16_T*)((UINT32)(pt_data) + (UINT32)(WGL_ALIGN_4(sizeof(WGL_TEXT_CONTENT_T))) + ui4_byte_num);
    /*allocate memory for line info*/
    pt_line_info = (WGL_TEXT_LINE_INFO*)WGL_WIDGET_MEM_ALLOC((VOID*)pt_data,sizeof(WGL_TEXT_LINE_INFO) * ui1_lines);
    if(NULL == pt_line_info)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to alloc memory for line info. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        WGL_MEM_FREE(pt_data);
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_line_info, 0, sizeof(WGL_TEXT_LINE_INFO) * ui1_lines);
    ui4_flag |= TEXT_BK_CLR_NOT_INIT 
             | TEXT_BK_IMG_NOT_INIT 
             | TEXT_FONT_NOT_INIT 
             | TEXT_TEXT_CLR_NOT_INIT 
             | TEXT_TOTAL_LINES_NOT_INIT
             | TEXT_STRING_POSITION_BY_IDX
             | TEXT_TEXT_BG_CLR_NOT_INIT;
    
    pt_data->ui1_align = WGL_AS_LEFT_TOP;   
    pt_data->ui1_align_custom = WGL_TEXT_NONE_ALIGN_CUSTOM;   
    pt_data->ui4_extra_flag = ui4_flag;
    pt_data->b_is_data_dirty = TRUE;
    pt_data->ui2_line_capacity = ui1_lines;
    pt_data->pt_line_infos = pt_line_info;
    pt_data->b_more_data = FALSE;
    pt_data->ui2_total_lines = 0;
    wgl_cvsst_copy(&pt_data->t_cvsst, pt_param->pt_cvsst);
    
    pt_data->t_arab_reorder_attr.b_find_init_level = TRUE;
    pt_data->t_arab_reorder_attr.ui1_embedding_level = 0;
    
    pt_data->pt_w2_text = pt_w2_text;
    pt_data->pt_w2_arab_dsp_text = pt_w2_arab_dsp_text;

    /*these break char defined in wgl_tools.h
          #define WGL_CAN_BREAK_CHARS                 {0x002D, 0x0020, WGL_UTF16_SOFT_HYPHEN, 0}
      */
    pt_data->as_break_chars[0]=0x002D;
    pt_data->as_break_chars[1]=0x0020;
    pt_data->as_break_chars[2]=WGL_UTF16_SOFT_HYPHEN;
    pt_data->as_break_chars[3]=0;

    /*set bk style*/
    pt_data->ui4_extra_flag |= (WGL_STL_GL_NO_BK & ((UINT32)(pt_param->pv_init)))? TEXT_NO_BK : 0;

    /*store settings*/
    WGL_RECT_COPY(&pt_data->t_cnt_rect, &pt_param->t_rect_cnt);
    pt_data->ui4_extra_flag |= (WGL_IS_IMAGE_UI((UINT32)(pt_param->pv_init))) ? TEXT_IMG_UI : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_MULTILINE & ((UINT32)(pt_param->pv_init))) ? TEXT_MULTILINE : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_SMART_CUT & ((UINT32)(pt_param->pv_init))) ? TEXT_SMART_CUT : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_SMART_CUT_OVER_MAX & ((UINT32)(pt_param->pv_init))) ? TEXT_SMART_CUT_OVER_MAX : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_GL_USE_SYS_IMG & ((UINT32)pt_param->pv_init)) ? TEXT_USE_SYS_IMG : 0;    
    pt_data->ui4_extra_flag |= (WGL_STL_GL_BDR_FILL_CNT_BK & ((UINT32)pt_param->pv_init)) ? TEXT_NOT_CLEAN_BK : 0;    
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_MAX_DIS_USER_DEF & ((UINT32)pt_param->pv_init)) ? TEXT_USER_DEF_MAX_LINE : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_IGNORE_NEWLINE & ((UINT32)pt_param->pv_init)) ? TEXT_IGNORE_NEWLINE : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_GL_FIX_TEXT_DIR & ((UINT32)pt_param->pv_init)) ? TEXT_FIX_DIR : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_GL_FIX_TEXT_DIR_ALIGN & ((UINT32)pt_param->pv_init)) ? TEXT_FIX_DIR_ALIGN : 0;
	pt_data->ui4_extra_flag |= (WGL_STL_TEXT_MULTISTRING & ((UINT32)pt_param->pv_init)) ? TEXT_MULTISTRING : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_TEXT_MS_NO_SMARTCUT & ((UINT32)(pt_param->pv_init))) ? TEXT_MS_NO_SMARTCUT : 0;

    if(0 == (TEXT_IGNORE_NEWLINE & pt_data->ui4_extra_flag))
    {
        pt_data->ui4_extra_flag |= (WGL_STL_TEXT_END_AS_NEWLINE & ((UINT32)pt_param->pv_init)) ? TEXT_END_OF_NEWLINE : 0;
    }    
    
    pt_data->ui2_text_length = 0;
    pt_data->ui1_read_direction = WGL_TEXT_DIR_LEFT_TO_RIGHT;    
    pt_data->b_opposite_align = FALSE;
    
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
INT32 wgl_text_cnt_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data)
{
    /*local variable*/
    WGL_TEXT_CONTENT_T*                 pt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(TEXT_CNT)} %s: h_cnt_data = 0x%.8x\n", __FUNCTION__, h_cnt_data));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_data = (WGL_TEXT_CONTENT_T*)h_cnt_data;

    /*free font*/
    if(pt_data->t_text_font)
    {
        i4_ret = WGL_FONT_FREE(pt_data->t_text_font);
        pt_data->t_text_font = NULL_HANDLE;
        WGL_ASSERT(0 == i4_ret);
    }
    if(pt_data->t_text_focus_font)
    {
        i4_ret = WGL_FONT_FREE(pt_data->t_text_focus_font);
        pt_data->t_text_focus_font = NULL_HANDLE;
        WGL_ASSERT(0 == i4_ret);
    }
    /*free buffer for line info*/
    WGL_ASSERT(pt_data->pt_line_infos);
    WGL_WIDGET_MEM_FREE(pt_data,sizeof(WGL_TEXT_LINE_INFO) * pt_data->ui2_line_capacity,pt_data->pt_line_infos);
    pt_data->pt_line_infos = NULL;

    _wgl_text_cnt_clear_string((WGL_TEXT_CONTENT_T*)h_cnt_data);

    /*free control logic object's resource*/
    WGL_MEM_FREE((WGL_TEXT_CONTENT_T*)h_cnt_data);

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
INT32 wgl_text_cnt_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    GL_COLOR_T                          t_selected_clr;
    GL_COLOR_T                          t_edge_clr;
    WGL_TEXT_CONTENT_T*                 pt_cnt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(TEXT_CNT)} %s: h_cnt_data = 0x%.8x \th_graphics = 0x%.8x \tui2_state = %d\n", __FUNCTION__, h_cnt_data, h_graphics, ui2_state));

    /*check parameter*/
    if(NULL == h_cnt_data || NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_cnt_data = (WGL_TEXT_CONTENT_T*)h_cnt_data;

    /*init selected color*/
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_selected_clr);

    /*paint background*/
    i4_ret = _wgl_text_cnt_paint_bk(pt_cnt_data, 
                                    h_graphics,
                                    ui4_upt_rect,
                                    pt_upt_rect,
                                    ui2_state);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    if (pt_cnt_data->ui2_text_length == 0 && !(((WGL_TEXT_CONTENT_T*)h_cnt_data)->ui4_extra_flag & TEXT_MULTISTRING))
    {
        /*x_dbg_stmt("pt_cnt_data->ui2_text_length is 0!!!\n");*/
        return WGLR_OK;
    }

    /*select text color*/
    i4_ret = _wgl_text_cnt_select_text_clr(pt_cnt_data, ui2_state, &t_selected_clr, &t_edge_clr);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*paint foreground*/
    if (((WGL_TEXT_CONTENT_T*)h_cnt_data)->ui4_extra_flag & TEXT_SCROLLING)
    {
        return  _wgl_text_cnt_paint_fg_single_line_scrl((WGL_TEXT_CONTENT_T*)h_cnt_data, h_graphics, &t_selected_clr, &t_edge_clr,ui2_state);    
    }
    if(((WGL_TEXT_CONTENT_T*)h_cnt_data)->ui4_extra_flag & TEXT_MULTILINE)
    {
        if(((WGL_TEXT_CONTENT_T*)h_cnt_data)->ui1_align_custom == WGL_TEXT_H_ALIGN_CUSTOM )
        {
            return _wgl_text_cnt_paint_fg_multiple_line_custom((WGL_TEXT_CONTENT_T*)h_cnt_data, h_graphics, &t_selected_clr, &t_edge_clr,ui2_state);
        }
        else
        {
        return _wgl_text_cnt_paint_fg_multiple_line((WGL_TEXT_CONTENT_T*)h_cnt_data, h_graphics, &t_selected_clr, &t_edge_clr,ui2_state);
    }
    }
    if (((WGL_TEXT_CONTENT_T*)h_cnt_data)->ui4_extra_flag & TEXT_MULTISTRING)
    {
        return _wgl_text_cnt_paint_fg_multiple_string((WGL_TEXT_CONTENT_T*)h_cnt_data, h_graphics, pt_upt_rect,ui2_state);
    }
    else
    {
        return _wgl_text_cnt_paint_fg_single_line((WGL_TEXT_CONTENT_T*)h_cnt_data, h_graphics, &t_selected_clr, &t_edge_clr,ui2_state);
    }
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
INT32 wgl_text_cnt_command_proc(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data,
    UINT32                              ui4_cmd_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2)
{
    /*local variable declaration*/
    WGL_TEXT_CONTENT_T*                 pt_text;

    DBG_API(("{WGL(TEXT_CNT)} %s: h_cnt_data = 0x%.8x \tui4_cmd_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
                                __FUNCTION__, 
                                h_cnt_data, 
                                ui4_cmd_id,
                                pv_param1,
                                pv_param2));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. The 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if(!(IS_TEXT_CNT_CMD(ui4_cmd_id) || WGL_IS_CMD_GENERAL(ui4_cmd_id)))
    {        
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    pt_text = (WGL_TEXT_CONTENT_T*)h_cnt_data;

    switch (ui4_cmd_id)
    {
    case WGL_CMD_GL_SET_IMAGE:
        return _wgl_text_cnt_set_img(pt_text, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_IMAGE:
        return _wgl_text_cnt_get_img(pt_text, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_COLOR:
        return _wgl_text_cnt_set_color(pt_text, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_COLOR:
        return _wgl_text_cnt_get_color(pt_text, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_FONT:
        return _wgl_text_cnt_set_font(pt_text, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT:
        return _wgl_text_cnt_get_font(pt_text, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_SET_FONT_EX:
        return _wgl_text_cnt_set_font_ex(pt_text, (WGL_FONT_INFO_EX_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT_EX:
        return _wgl_text_cnt_get_font_ex(pt_text, (WGL_FONT_INFO_EX_T*)pv_param1);

   case WGL_CMD_GL_SET_TEXT_DIRECTION:
        return _wgl_text_cnt_set_text_dir(pt_text, WGL_POINTER_TO_UINT8(pv_param1));

    case WGL_CMD_GL_GET_TEXT_DIRECTION:
        return _wgl_text_cnt_get_text_dir(pt_text, (UINT8*)pv_param1);    
    case WGL_CMD_GL_GET_RENDER_COND:
        return _wgl_text_cnt_get_render_cond(pt_text, WGL_POINTER_TO_UINT16(pv_param1),(UINT8*)pv_param2);   

    case WGL_CMD_TEXT_SET_FONT_ATTR:
        return _wgl_text_cnt_set_font_attr(pt_text, (WGL_TEXT_FONT_ATTR_TYPE_T)pv_param1, pv_param2);
    case WGL_CMD_TEXT_SET_FONT_ATTR_EX:
        return _wgl_text_cnt_set_font_attr_ex(pt_text, WGL_POINTER_TO_UINT8(pv_param1), (WGL_TEXT_FONT_ATTR_T*)pv_param2);
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
                    pt_text->t_arab_reorder_attr.b_find_init_level = TRUE;
                    pt_text->t_arab_reorder_attr.ui1_embedding_level = 0;
                
                }else
                {
                    pt_text->t_arab_reorder_attr.b_find_init_level = FALSE;
                    pt_text->t_arab_reorder_attr.ui1_embedding_level = (*(UINT8*)(((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data));
                }
             }
            /*update dirty flag*/
           _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
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

           *((UINT8*)(((WGL_TEXT_ATTR_T*)pv_param1)->pv_attr_data)) = pt_text->t_arab_reorder_attr.ui1_embedding_level;
        }
        break;
	case WGL_CMD_TEXT_SET_IGNORE_NEWLINE:
		{
			if (WGL_POINTER_TO_BOOL(pv_param1))
			{
	    		pt_text->ui4_extra_flag |= TEXT_IGNORE_NEWLINE;
			}
			else
			{
				pt_text->ui4_extra_flag &= ~TEXT_IGNORE_NEWLINE;
			}
		}
		break;
    case WGL_CMD_TEXT_SET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(TEXT_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((WGL_INSET_T*)pv_param1)->i4_left,
                        ((WGL_INSET_T*)pv_param1)->i4_top,
                        ((WGL_INSET_T*)pv_param1)->i4_right,
                        ((WGL_INSET_T*)pv_param1)->i4_bottom));

            if(FALSE == wgl_is_legal_inset(&pt_text->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_text->t_fg_inset, (WGL_INSET_T*)pv_param1);
        
            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, TEXT_TOTAL_LINES_NOT_INIT);
        }
        break;

    case WGL_CMD_TEXT_GET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_text->t_fg_inset);
        }
        break;

    case WGL_CMD_TEXT_SET_ALIGN:
        {
            if(FALSE == wgl_is_legal_alignment(WGL_POINTER_TO_UINT8(pv_param1)))
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_ALIGN, the alignment specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            pt_text->ui1_align = WGL_POINTER_TO_UINT8(pv_param1);

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
        }
        break;

    case WGL_CMD_TEXT_SET_CUSTOM_ALIGN:
            {
                if(WGL_TEXT_H_ALIGN_CUSTOM != WGL_POINTER_TO_UINT8(pv_param1))
                {
                    DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_CUSTOM_ALIGN, the alignment specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG;
                }
    
                pt_text->ui1_align_custom = WGL_POINTER_TO_UINT8(pv_param1);
            }
            break;

    case WGL_CMD_TEXT_GET_ALIGN:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_ALIGN, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT8*)pv_param1) = pt_text->ui1_align;
        }
        break;

    case WGL_CMD_TEXT_SET_TEXT:
        return _wgl_text_cnt_set_text(pt_text, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1, FALSE);

    case WGL_CMD_TEXT_APPEND_TEXT:
        return _wgl_text_cnt_set_text(pt_text, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1, TRUE);

    case WGL_CMD_TEXT_GET_TEXT:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_TEXT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            /*check length*/
            if(pt_text->ui2_text_length >= WGL_POINTER_TO_UINT16(pv_param2) )
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: The input buffer isn't enough. %d-character is necessary. (%d) at L%d\n\n", pt_text->ui2_text_length, WGLR_BUFFER_NOT_ENOUGH, __LINE__));
                return WGLR_BUFFER_NOT_ENOUGH;
            }
            
            /*copy string*/
            x_memcpy(pv_param1, pt_text->pt_w2_text, (pt_text->ui2_text_length + 1) * sizeof(UTF16_T));
        }
        break;

    case WGL_CNT_TEXT_SET_DISPLAY_START:
        {
            pt_text->ui2_start_line = WGL_POINTER_TO_UINT16(pv_param1);

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
        }
        break;

	case WGL_CNT_TEXT_SET_DISPLAY_START_EX:
        {
            pt_text->ui2_start_line = WGL_POINTER_TO_UINT16(pv_param1);

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);

            pt_text->b_start_idx_valid = TRUE;
        }
        break;

    case WGL_CNT_TEXT_GET_DISPLAY_START:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CNT_TEXT_GET_DISPLAY_START, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT16*)pv_param1) = pt_text->ui2_start_line;
        }
        break;
    case WGL_CMD_TEXT_GET_FONT_HANDLE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CNT_TEXT_GET_DISPLAY_START, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((HANDLE_T*)pv_param1) = pt_text->t_text_font;
        }
        break;

    case WGL_CNT_TEXT_GET_TOTAL_LINES:
        if (WGL_STL_TEXT_MULTISTRING & pt_text->ui4_extra_flag)
        {
            GL_RECT_T                   t_available_rect;
            
            /*calculate available rectangle*/
            WGL_RECT_COPY(&t_available_rect, &pt_text->t_cnt_rect);
            WGL_RECT_INSET(&t_available_rect, 
                           pt_text->t_fg_inset.i4_left, 
                           pt_text->t_fg_inset.i4_top,
                           pt_text->t_fg_inset.i4_right, 
                           pt_text->t_fg_inset.i4_bottom);
            return _wgl_text_cnt_calc_total_line_ex(pt_text, &t_available_rect,WGL_POINTER_TO_UINT16(pv_param2) ,(UINT16*)pv_param1);
        }
        else
        {
            GL_RECT_T                   t_available_rect;
            
            /*calculate available rectangle*/
            WGL_RECT_COPY(&t_available_rect, &pt_text->t_cnt_rect);
            WGL_RECT_INSET(&t_available_rect, 
                           pt_text->t_fg_inset.i4_left, 
                           pt_text->t_fg_inset.i4_top,
                           pt_text->t_fg_inset.i4_right, 
                           pt_text->t_fg_inset.i4_bottom);
            return _wgl_text_cnt_calc_total_line(pt_text, &t_available_rect,WGL_POINTER_TO_UINT16(pv_param2) ,(UINT16*)pv_param1);
        }

    case WGL_CNT_TEXT_GET_PAGE_LINES:
        {
            GL_RECT_T                   t_available_rect;
            
            /*calculate available rectangle*/
            WGL_RECT_COPY(&t_available_rect, &pt_text->t_cnt_rect);
            WGL_RECT_INSET(&t_available_rect, 
                pt_text->t_fg_inset.i4_left, 
                pt_text->t_fg_inset.i4_top,
                pt_text->t_fg_inset.i4_right, 
                pt_text->t_fg_inset.i4_bottom);
            return _wgl_text_cnt_calc_page_line(pt_text, &t_available_rect, WGL_POINTER_TO_UINT16(pv_param2),(UINT16*)pv_param1);
        }
    case WGL_CNT_TEXT_MAX_LINE_WIDTH:
        
         return _wgl_text_cnt_get_max_line_width(pt_text,WGL_POINTER_TO_UINT16(pv_param2),(UINT16*)pv_param1);
    

    case WGL_CMD_TEXT_SET_LINE_GAP:
        {
            pt_text->i4_line_gap = (INT32)pv_param1;

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
        }
        break;

    case WGL_CMD_TEXT_GET_LINE_GAP:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_LINE_GAP, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((INT32*)pv_param1) = pt_text->i4_line_gap;
        }
        break;

    case WGL_CNT_TEXT_SET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CNT_TEXT_SET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(TEXT_CNT)} set_rect(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((GL_RECT_T*)pv_param1)->i4_left,
                        ((GL_RECT_T*)pv_param1)->i4_top,
                        ((GL_RECT_T*)pv_param1)->i4_right,
                        ((GL_RECT_T*)pv_param1)->i4_bottom));

            /*copy rect*/
            WGL_RECT_COPY(&pt_text->t_cnt_rect, (GL_RECT_T*)pv_param1); 
            
            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, TEXT_TOTAL_LINES_NOT_INIT);
        }
        break;

    case WGL_CNT_TEXT_GET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CNT_TEXT_GET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*prepare return value*/
            WGL_RECT_COPY((GL_RECT_T*)pv_param1, &((WGL_TEXT_CONTENT_T*)h_cnt_data)->t_cnt_rect);
        }
        break;
	case WGL_CNT_TEXT_GET_PREFERRED_SIZE:
		return _wgl_text_cnt_get_preferred_size((WGL_TEXT_CONTENT_T*)h_cnt_data, (GL_SIZE_T*)pv_param1);

    case WGL_CNT_TEXT_GET_MIN_SIZE:
        return _wgl_text_cnt_get_min_size((WGL_TEXT_CONTENT_T*)h_cnt_data, (GL_SIZE_T*)pv_param1);

    case WGL_CNT_TEXT_UPDATE_CVSST:
        return wgl_update_cvvst((WGL_CVS_CLR_CFG_T*)pv_param1, &((WGL_TEXT_CONTENT_T*)h_cnt_data)->t_cvsst);            

    case WGL_CNT_TEXT_GET_MAX_DIS_LINE:
        return _wgl_text_cnt_get_max_line((WGL_TEXT_CONTENT_T*)h_cnt_data, pv_param1);

    case WGL_CMD_TEXT_SET_MAX_DIS_LINE:
        return _wgl_text_cnt_set_max_line((WGL_TEXT_CONTENT_T*)h_cnt_data, WGL_POINTER_TO_UINT16(pv_param1));

    case WGL_CNT_TEXT_BEGIN_SCRL_CHK:
        return _wgl_text_cnt_begin_scrl_check((WGL_TEXT_CONTENT_T*)h_cnt_data,WGL_POINTER_TO_UINT16(pv_param2));
    case WGL_CNT_TEXT_SCRL_SMART_CHK:
        return _wgl_text_cnt_scrl_smart_check((WGL_TEXT_CONTENT_T*)h_cnt_data,WGL_POINTER_TO_UINT16(pv_param2));
    case WGL_CNT_TEXT_BEGIN_SCRL:
         _wgl_text_cnt_begin_scrl((WGL_TEXT_CONTENT_T*)h_cnt_data,pv_param1); 
         break;

    case WGL_CNT_TEXT_END_SCRL:
         _wgl_text_cnt_end_scrl((WGL_TEXT_CONTENT_T*)h_cnt_data); 
          break;

    case WGL_CNT_TEXT_DO_SCRL:
        return _wgl_text_cnt_do_scrl((WGL_TEXT_CONTENT_T*)h_cnt_data,WGL_POINTER_TO_UINT16(pv_param2));        

    case WGL_CNT_TEXT_SET_STATE:/*in order to get correct update region in paint method*/
           /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
           break;
    case WGL_CMD_TEXT_SET_STRING:
          return _wgl_text_cnt_set_string((WGL_TEXT_CONTENT_T*)h_cnt_data,pv_param1,WGL_POINTER_TO_UINT16(pv_param2));

    case WGL_CMD_TEXT_SET_STRING_PREV:
          return _wgl_text_cnt_set_string_prev((WGL_TEXT_CONTENT_T*)h_cnt_data,pv_param1,WGL_POINTER_TO_UINT16(pv_param2));

    case WGL_CMD_TEXT_PLAY_NEXT_STRING:
          return _wgl_text_cnt_remove_string_prev((WGL_TEXT_CONTENT_T*)h_cnt_data);

    case WGL_CMD_TEXT_PLAY_PREV_STRING:
          return _wgl_text_cnt_remove_string_end((WGL_TEXT_CONTENT_T*)h_cnt_data);

    case WGL_CMD_TEXT_STRING_CLEAR:
          return _wgl_text_cnt_clear_string((WGL_TEXT_CONTENT_T*)h_cnt_data);

    case WGL_CMD_TEXT_SET_HIGHLIGHT_POSTION:
        {
            WGL_TEXT_POSITION_TYPE_T e_type = (WGL_TEXT_POSITION_TYPE_T)(pv_param2);
            switch(e_type)
            {
                case WGL_TEXT_POSITION_BY_STRING_INDEX: 
                    {
                        pt_text->ui2_highlight_postion = WGL_POINTER_TO_UINT16(pv_param1);
                        pt_text->ui4_extra_flag |= TEXT_STRING_POSITION_BY_IDX;
                        /*update dirty flag*/
                        _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
                    }
                break;
                
                default:
                    {
                        pt_text->ui2_highlight_postion = WGL_POINTER_TO_UINT16(pv_param1);
                        pt_text->ui4_extra_flag &= ~TEXT_STRING_POSITION_BY_IDX;
                        /*update dirty flag*/
                        _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
                    }
                break;
            }
        }
        break;
    case WGL_CMD_TEXT_SET_HIGHLIGHT_STRING:
        {
            pt_text->ui2_highlight_idx = WGL_POINTER_TO_UINT16(pv_param1);

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
        }
        break;
        
    case WGL_CMD_TEXT_SET_INTERLACED:
        {
            pt_text->ui2_interlace= WGL_POINTER_TO_UINT16(pv_param1);

            /*update dirty flag*/
            _wgl_text_cnt_reset_dirty_flag(pt_text, 0);
        }
        break;
    case WGL_CMD_TEXT_GET_INTERLACED:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_LINE_GAP, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT16*)pv_param1) = pt_text->ui2_interlace;
        }
        break;
    case WGL_CMD_TEXT_GET_CNT_WIDTH:
        {
            GL_RECT_T                   t_available_rect;
            UINT16*                     pui2_width = (UINT16*)pv_param1;
            
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_LINE_GAP, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            /*calculate available rectangle*/
            WGL_RECT_COPY(&t_available_rect, &pt_text->t_cnt_rect);
            WGL_RECT_INSET(&t_available_rect, 
                pt_text->t_fg_inset.i4_left, 
                pt_text->t_fg_inset.i4_top,
                pt_text->t_fg_inset.i4_right, 
                pt_text->t_fg_inset.i4_bottom);

            *pui2_width = (UINT16)(WGL_RECT_GET_WIDTH(&t_available_rect));            
        }
        break;
    case WGL_CMD_TEXT_SET_BREAK_CHARS:
        if(NULL == pv_param1)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_BREAK_CHARS, the 1st argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
            return WGLR_INV_ARG;
        }
        return _wgl_text_cnt_set_break_chars((WGL_TEXT_CONTENT_T*)h_cnt_data,(UTF16_T*)pv_param1,WGL_POINTER_TO_UINT8(pv_param2));

	case WGL_CMD_TEXT_GET_FONT_POINT_SIZE_FROM_HEIGHT:
		if (NULL == pv_param1 || NULL == pv_param2)
		{
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_SIZE_FROM_HEIGHT, the 1st and 2nd argument cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
            return WGLR_INV_ARG;
        }	
		return _wgl_text_cnt_get_point_size_from_height((WGL_TEXT_CONTENT_T*)h_cnt_data, (UINT16)*((UINT16*)pv_param1),(UINT8*)pv_param2);
    default:
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
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
static INT32 _wgl_text_cnt_paint_bk(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    GL_COLOR_T                          t_clr;
    INT32                               i4_ret;

    /*check paint type*/
    if(TEXT_NO_BK & pt_cnt_data->ui4_extra_flag)
    {
        return WGLR_OK;
    }
    else if(!(TEXT_IMG_UI & pt_cnt_data->ui4_extra_flag))
    {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_text_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
        }
        
        /*select image*/
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            t_clr = pt_cnt_data->t_clr_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            t_clr = pt_cnt_data->t_clr_bk.t_enable;
        }
        else
        {
            t_clr = pt_cnt_data->t_clr_bk.t_disable;
        }

        /*should check TEXT_NOT_CLEAN_BK style here*/
        
        /*paint background*/
        i4_ret = wgl_fill_widget_bk(h_graphics, &t_clr, &pt_cnt_data->t_cnt_rect, &pt_cnt_data->t_cvsst);
        WGL_TEXT_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to fill background color. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    else
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_text_cnt_prepare_img(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }        
        }

        /*select image*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_cnt_data->t_img_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            h_image = pt_cnt_data->t_img_bk.t_enable;
        }
        else
        {
            h_image = pt_cnt_data->t_img_bk.t_disable;
        }

        /*paint background*/
        if(NULL_HANDLE != h_image)
        {
            i4_ret = wgl_img_tpl_paint(h_image, 
                                       &pt_cnt_data->t_cnt_rect, 
                                       pt_upt_rect, 
                                       h_graphics, 
                                       &pt_cnt_data->t_cvsst,
                                       (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_NOT_CLEAN_BK)) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT);
            WGL_TEXT_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw image UI to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        else
        {
            i4_ret = WGLR_OK;
        }
        
        return i4_ret;
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
static INT32 _wgl_text_cnt_select_text_clr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_selected_clr,
    GL_COLOR_T*                         pt_edge_clr)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_text_cnt_use_sys_text_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check if edge colors are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_text_cnt_use_sys_edge_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*select color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_highlight;
        *pt_edge_clr = pt_cnt_data->t_clr_edge.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_enable;
        *pt_edge_clr = pt_cnt_data->t_clr_edge.t_enable;
    }
    else
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_disable;
        *pt_edge_clr = pt_cnt_data->t_clr_edge.t_disable;
    }

    return WGLR_OK;
}

static INT32 _wgl_text_cnt_select_text_bg_clr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_bg_clr)
{
	if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT))
    {
        /*get system colors*/
        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, pt_bg_clr);
    }
	else
	{
		/*select color*/
	    if(IS_STATE_FOCUSED(ui2_state))
	    {
	        *pt_bg_clr = pt_cnt_data->t_clr_text_bg.t_highlight;
	    }
	    else if(IS_STATE_ENABLED(ui2_state))
	    {
	        *pt_bg_clr = pt_cnt_data->t_clr_text_bg.t_enable;
	    }
	    else
	    {
	        *pt_bg_clr = pt_cnt_data->t_clr_text_bg.t_disable;
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
static INT32 _wgl_text_cnt_select_text_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    UINT16                              ui2_state,
    HANDLE_T*                           pt_selected_font,
    WGL_FONT_INFO_T*                    pt_selected_font_info)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
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

static INT32 _wgl_text_cnt_update_arab_text_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UTF16_T *                           ps_string,
    UTF16_T *							ps_out_string)
{
    SIZE_T    t_size;
    UINT16    ui2_arab_dsp_len;
    UINT16    ui2_len_capacity;
    INT32     i4_ret = WGLR_OK;
    VOID*     pv_reorder_attr = NULL;
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);

    t_size = (ui2_len_capacity + 1) * sizeof(UTF16_T);
    x_memset(pt_cnt_data->pt_w2_arab_dsp_text,0,t_size);
    if(pt_cnt_data->t_arab_reorder_attr.b_find_init_level == FALSE)
    {
        pv_reorder_attr = &pt_cnt_data->t_arab_reorder_attr;
    }
	
    i4_ret = reorder_arabic(ps_string,ui2_len_capacity, pv_reorder_attr,
                            ps_out_string,&ui2_arab_dsp_len); 
    if (WGLR_OK != i4_ret)
    {
        return i4_ret;
    }   
	#if 0
    if (ui2_arab_dsp_len > 0)
    {
        w2s_src_end = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_arab_dsp_len -1]);    
        w2s_dst_end = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity - 1]);

        while(w2s_src_end >= pt_cnt_data->pt_w2_arab_dsp_text)
        {
           *w2s_dst_end = *w2s_src_end;
           w2s_src_end--;
           w2s_dst_end--;
        }
        if (ui2_len_capacity - ui2_arab_dsp_len > 0)
        {
           x_memset(pt_cnt_data->pt_w2_arab_dsp_text,0,(ui2_len_capacity - ui2_arab_dsp_len) * sizeof(UTF16_T)); 
        }
    }
	#endif

    pt_cnt_data->ui2_arab_dsp_text_len = ui2_arab_dsp_len;
    return i4_ret; 

}

static INT32 _wgl_text_cnt_update_arab_text(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data)
{
    UTF16_T*  w2s_src_end;
    UTF16_T*  w2s_dst_end;
    SIZE_T    t_size;
    UINT16    ui2_arab_dsp_len;
    UINT16    ui2_len_capacity;
    INT32     i4_ret = WGLR_OK;
    VOID*     pv_reorder_attr = NULL;
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);

    t_size = (ui2_len_capacity + 1) * sizeof(UTF16_T);
    x_memset(pt_cnt_data->pt_w2_arab_dsp_text,0,t_size);
    if(pt_cnt_data->t_arab_reorder_attr.b_find_init_level == FALSE)
    {
        pv_reorder_attr = &pt_cnt_data->t_arab_reorder_attr;
    }
    i4_ret = reorder_arabic(pt_cnt_data->pt_w2_text,ui2_len_capacity, pv_reorder_attr,pt_cnt_data->pt_w2_arab_dsp_text,&ui2_arab_dsp_len); 
    if (WGLR_OK != i4_ret)
    {
        return i4_ret;
    }   
    if (ui2_arab_dsp_len > 0)
    {
        w2s_src_end = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_arab_dsp_len -1]);    
        w2s_dst_end = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity - 1]);

        while(w2s_src_end >= pt_cnt_data->pt_w2_arab_dsp_text)
        {
           *w2s_dst_end = *w2s_src_end;
           w2s_src_end--;
           w2s_dst_end--;
        }
        if (ui2_len_capacity - ui2_arab_dsp_len > 0)
        {
           x_memset(pt_cnt_data->pt_w2_arab_dsp_text,0,(ui2_len_capacity - ui2_arab_dsp_len) * sizeof(UTF16_T)); 
        }
    }
    
    pt_cnt_data->ui2_arab_dsp_text_len = ui2_arab_dsp_len;
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
static INT32 _wgl_text_cnt_paint_fg_multiple_string(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    INT32                               i4_ret;
    INT32                               i4_x_start;
    INT32                               i4_y_start;
    INT32                               i4_widget_height;
    INT32                               i4_total_line_height;
    INT32                               i4_display_height;
    UINT16                              ui2_draw_width;
    UINT16                              ui2_draw_len;
    UINT16                              ui2_lines;
    UINT16                              ui2_cur_line;
    WGL_TEXT_LINE_INFO*                 pt_cur_line_info;
    GL_RECT_T                           t_available_rect;
    GL_COLOR_T                          t_text_bg_clr;
    GL_COLOR_T*                         pt_highlight_text_clr;
    GL_COLOR_T*                         pt_enable_text_clr;
    GL_COLOR_T*                         pt_highlight_edge_clr;
    GL_COLOR_T*                         pt_enable_edge_clr;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    UTF16_T                             a_w2s_force_break_skip_chars[] = WGL_FORCE_BREAK_SKIP_CHARS;
    INT32                               i, i4_adv_x, i4_adv_y;
    INT32                               i4_keep_line_width=0;
    UTF16_T                             t_last_char;
    INT32                               i4_skip_x_start=0;
    INT32                               i4_skip_x_offset;
    UINT8                               ui1_adjusted_align;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT16                               i2_line_height;
 #ifdef ARAB_EXIST_SUPPORT
    BOOL								b_exist_arab = FALSE;
 #endif
    UINT8								ui1_cur_font_id;
    UINT16 ui2_muti_line_idx = 0;
    INT16 i2_highlight_display_end_line = 0;
    WGL_TEXT_MULTI_STRING *pt_multi_string ;
    GL_RECT_T                           t_work_rect = {0};
    GL_RECT_T                           t_line_rect = {0};
  	UINT16                              ui2_max_line_width = 0;

    if (pt_cnt_data->pt_multi_string == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }
     /*init background color*/
	_wgl_text_cnt_select_text_bg_clr(pt_cnt_data, ui2_state, &t_text_bg_clr);
    
    /*check if text colors are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_text_cnt_use_sys_text_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check if edge colors are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_text_cnt_use_sys_edge_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    pt_highlight_text_clr = &(pt_cnt_data->t_clr_text.t_highlight);
    pt_enable_text_clr    = &(pt_cnt_data->t_clr_text.t_enable);
    pt_highlight_edge_clr = &(pt_cnt_data->t_clr_edge.t_highlight);
    pt_enable_edge_clr    = &(pt_cnt_data->t_clr_edge.t_enable);

    if (pt_cnt_data->ui2_line_capacity < pt_cnt_data->ui2_interlace + 1)
    {
        pt_cnt_data->ui2_interlace = pt_cnt_data->ui2_line_capacity - 1;
    }
    ui1_adjusted_align = 0;
    if(!pt_cnt_data->b_opposite_align)
    {
        ui1_adjusted_align = pt_cnt_data->ui1_align;
    }
    else
    {
       /*reverse text horizontal alignment for right to left text direction*/
        ui1_adjusted_align |= (WGL_IS_H_LEFT(pt_cnt_data->ui1_align)<<2);
        ui1_adjusted_align |= WGL_IS_H_CENTER(pt_cnt_data->ui1_align);
        ui1_adjusted_align |= (WGL_IS_H_RIGHT(pt_cnt_data->ui1_align)>>2);
        
        /*text direction dose not affect vertical alignment*/
        ui1_adjusted_align |= (pt_cnt_data->ui1_align&0x0F);
    }

    if ((pt_cnt_data->ui4_extra_flag & TEXT_MS_NO_SMARTCUT) && !(WGL_IS_H_CENTER(ui1_adjusted_align)))
    {
        i4_ret = _wgl_text_cnt_layout_v_top_ex(pt_cnt_data,ui2_state, FALSE);
    }
    else
    {
        i4_ret = _wgl_text_cnt_layout_v_top_ex(pt_cnt_data,ui2_state, TRUE);
    }
    
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    i4_ret = x_fe_get_cur_font_resource(t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
  	  return i4_ret;
    }
    if (ui1_cur_font_id != pt_cnt_data->ui1_cur_font_id)
    {
  	   pt_cnt_data->b_is_data_dirty = TRUE;
  	   pt_cnt_data->ui1_cur_font_id = ui1_cur_font_id;
    }

    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    WGL_INTERSECT_RECT(&t_work_rect, pt_upt_rect, &t_available_rect);
    
    /*get widget height*/
    i4_widget_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    WGL_ASSERT(i4_widget_height);
    
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate the line number to paint*/
    i4_total_line_height = pt_cnt_data->i4_line_gap + i2_line_height;

    ui2_lines =  (UINT16)(i4_widget_height / i4_total_line_height);
    ui2_lines += ((i4_widget_height - i4_total_line_height * ui2_lines) >= i2_line_height) ? 1 : 0;
           
    /*check if anything to display anything*/
    if(ui2_lines == 0) 
    {
        ui2_lines = 1;/*in order to keep consistent with the behavior of single line style,
                                         that is to say UI  will display one line text at least 
                                         even if the widget can not contain the whole line text*/
        //return WGLR_OK;
    }   
    
    /*check if line number is over display line*/
    if(ui2_lines >= pt_cnt_data->ui2_lines_in_display)
    {
        ui2_lines = pt_cnt_data->ui2_lines_in_display;
    }
    
    if (ui2_lines < pt_cnt_data->ui2_highlight_postion)
    {
        pt_cnt_data->ui2_highlight_postion = 0;
    }
    /*calculate the display height*/
    i4_display_height = (ui2_lines - 1) * i4_total_line_height + i2_line_height;  
    
    pt_multi_string = pt_cnt_data->pt_multi_string;

    for(;ui2_muti_line_idx<=pt_cnt_data->ui2_highlight_idx;ui2_muti_line_idx++)
    {
        i2_highlight_display_end_line += pt_multi_string->ui2_lines;
        pt_multi_string = pt_multi_string->pt_next;
    }
    if(i2_highlight_display_end_line>ui2_lines)
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos+i2_highlight_display_end_line-ui2_lines;
    }
    else
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos;
    }

	for (ui2_cur_line = pt_cnt_data->ui2_start_line; ui2_cur_line < ui2_lines; ui2_cur_line++)
	{
		ui2_max_line_width = MAX(ui2_max_line_width,pt_cur_line_info->ui2_width);
		pt_cur_line_info ++;
	}

    if(i2_highlight_display_end_line>ui2_lines)
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos+i2_highlight_display_end_line-ui2_lines;
    }
    else
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos;
    }
    /*start line*/
    if(WGL_IS_V_TOP(ui1_adjusted_align))
    {
       // pt_cur_line_info = pt_cnt_data->pt_line_infos;
        i4_y_start = t_available_rect.i4_top + (INT32)(pt_cnt_data->ui2_start_line * i4_total_line_height);
    }
    else if(WGL_IS_V_CENTER(ui1_adjusted_align))
    {
        //pt_cur_line_info = pt_cnt_data->pt_line_infos + ((pt_cnt_data->ui2_lines_in_display - ui2_lines) >> 1);
        i4_y_start = t_available_rect.i4_top + ((i4_widget_height - i4_display_height) >> 1);
    }
    else /*V_BOTTOM*/
    {
       // pt_cur_line_info = pt_cnt_data->pt_line_infos + pt_cnt_data->ui2_lines_in_display - ui2_lines;
        i4_y_start = t_available_rect.i4_bottom - i4_display_height;
    }

     /*paint lines*/
    x_memcpy(&t_line_rect, &t_work_rect, sizeof(GL_RECT_T));
    for(ui2_cur_line = pt_cnt_data->ui2_start_line; ui2_cur_line < ui2_lines; ui2_cur_line++)
    {
        if (pt_cur_line_info->w2s_string == NULL)
        {
            i4_y_start += i4_total_line_height;
            pt_cur_line_info++;
            continue;
        }
        
        t_line_rect.i4_top    = i4_y_start;
        t_line_rect.i4_bottom = i4_y_start + i2_line_height;  
        if (!WGL_RECT_VT_IS_OVERLAP(&t_line_rect,&t_work_rect))
        {
            i4_y_start += i4_total_line_height;
            pt_cur_line_info++;
            continue;
        }
		
			ui2_draw_len      = pt_cur_line_info->ui2_str_len;
			ui2_draw_width = pt_cur_line_info->ui2_width;

		if ((pt_cnt_data->ui4_extra_flag & TEXT_MS_NO_SMARTCUT) && (WGL_IS_H_CENTER(ui1_adjusted_align)))
		{
			i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - ui2_max_line_width) >> 1);
		}
		else
		{
			if(WGL_IS_H_RIGHT(ui1_adjusted_align))
			{
				i4_x_start = t_available_rect.i4_right - pt_cur_line_info->ui2_width;
			}
			else if(WGL_IS_H_CENTER(ui1_adjusted_align))
			{
				i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - pt_cur_line_info->ui2_width) >> 1);
			}
			else /*H_LEFT*/
			{
				i4_x_start = t_available_rect.i4_left;
			}
		}

        if(ui2_draw_len > 0 && ui2_draw_width > 0 && 
           (!wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_highlight_text_clr) ||
           !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_enable_text_clr)))
        {                        
          

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif        
            {
                t_last_char = pt_cur_line_info->w2s_string[ui2_draw_len - 1];
                if ((pt_cur_line_info->ui2_str_idx == pt_cnt_data->ui2_highlight_idx)
                    && !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_highlight_text_clr))
                {
                    i4_ret = x_gl_draw_ustring_ex(
                                  h_graphics,
                                  i4_x_start,
                                  i4_y_start,
                                  i4_y_start,
                                  pt_cur_line_info->w2s_string,
                                  ui2_draw_len,
                                  (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                 (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                  t_text_font,
                                  pt_highlight_text_clr,
                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                  pt_highlight_edge_clr,
                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                  (UINT16)t_font_info.i2_width,
                                  0,
                                  (UINT16)i2_line_height,
                                  0);
                }
                else
                {
                    i4_ret = x_gl_draw_ustring_ex(
                                  h_graphics,
                                  i4_x_start,
                                  i4_y_start,
                                  i4_y_start,
                                  pt_cur_line_info->w2s_string,
                                  ui2_draw_len,
                                  (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                 (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                  t_text_font,
                                  pt_enable_text_clr,
                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                  pt_enable_edge_clr,
                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                  (UINT16)t_font_info.i2_width,
                                  0,
                                  (UINT16)i2_line_height,
                                  0);
                }
            }
            else
            {
                t_last_char = *(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len);
                i4_skip_x_offset = 0;
                if((ui2_cur_line == ui2_lines - (UINT16)1) )
                {
                    i4_x_start += pt_cur_line_info->ui2_width -i4_keep_line_width;
                }
                if(a_w2s_skip_chars[0] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char, 
                                                    &i4_skip_x_offset, 
                                                    &i4_adv_y);                  
                }
                if ((pt_cur_line_info->ui2_str_idx == pt_cnt_data->ui2_highlight_idx)
                    && !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_highlight_text_clr))
                {
                    i4_ret = x_gl_draw_ustring_ex(
                                  h_graphics,
                                  i4_x_start + i4_skip_x_offset,
                                  i4_y_start,
                                  i4_y_start,
                                  (UTF16_T*)(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len) ,
                                  ui2_draw_len,
                                  (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                  (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                  t_text_font,
                                  pt_highlight_text_clr,
                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                  pt_highlight_edge_clr,
                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                  (UINT16)t_font_info.i2_width,
                                  0,
                                  (UINT16)i2_line_height,
                                  0);
                }
                else
                {
                    i4_ret = x_gl_draw_ustring_ex(
                                  h_graphics,
                                  i4_x_start + i4_skip_x_offset,
                                  i4_y_start,
                                  i4_y_start,
                                  (UTF16_T*)(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len) ,
                                  ui2_draw_len,
                                  (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                  (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                  t_text_font,
                                  pt_enable_text_clr,
                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                  pt_enable_edge_clr,
                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                  (UINT16)t_font_info.i2_width,
                                  0,
                                  (UINT16)i2_line_height,
                                  0);
                }
            }
            
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }

            
            /*draw '-' when softhypen is the last char of a line */
            for(i = (INT32)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1); i > 0; i--)
            {
                if(a_w2s_skip_chars[i - 1] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char, 
                                                    &i4_adv_x, 
                                                    &i4_adv_y);

                    if(i4_ret != FER_OK && FER_NO_CHAR_CODE != i4_ret)
                    {
                        i4_adv_x = 0;
                    }

 #ifdef ARAB_EXIST_SUPPORT
                    if (!b_exist_arab)
#else
                    if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif              
                    {
                        i4_skip_x_start = i4_x_start + pt_cur_line_info->ui2_width - i4_adv_x;
                    }
                    else
                    {
                        i4_skip_x_start = i4_x_start;
                    }
                    if ((pt_cnt_data->ui2_highlight_idx == pt_cur_line_info->ui2_str_idx)
                    && !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_highlight_text_clr))
                    
                    {
                        i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                                      i4_skip_x_start,
                                                      i4_y_start,
                                                      i4_y_start,
                                                      &t_last_char,
                                                      1,
                                                      NULL,
                                                      0,
                                                      t_text_font,
                                                      pt_highlight_text_clr,
                                                      (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                                      pt_highlight_edge_clr,
                                                      (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                                      (UINT16)t_font_info.i2_width,
                                                      0,
                                                      (UINT16)i2_line_height,
                                                      0);
                        
                    }
                    else
                    {
                        i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                                      i4_skip_x_start,
                                                      i4_y_start,
                                                      i4_y_start,
                                                      &t_last_char,
                                                      1,
                                                      NULL,
                                                      0,
                                                      t_text_font,
                                                      pt_enable_text_clr,
                                                      (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                                      pt_enable_edge_clr,
                                                      (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                                      (UINT16)t_font_info.i2_width,
                                                      0,
                                                      (UINT16)i2_line_height,
                                                      0);
                    }
                    if(GLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return WGLR_TEXT_DRAW_FAIL;
                    }
                }
            }
        }
        
        i4_y_start += i4_total_line_height;
        pt_cur_line_info++;
        
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
static INT32 _wgl_text_cnt_paint_fg_multiple_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    INT32                               i4_ret;
    INT32                               i4_x_start;
    INT32                               i4_y_start;
    INT32                               i4_widget_height;
    INT32                               i4_total_line_height;
    INT32                               i4_display_height;
    UINT16                              ui2_draw_width = 0;
    UINT16                              ui2_draw_len;
    UINT16                              ui2_lines;
    UINT16                              ui2_cur_line;
    WGL_TEXT_LINE_INFO*                 pt_cur_line_info, *pt_line_info_temp;
    GL_RECT_T                           t_available_rect;
    GL_COLOR_T                          t_text_bg_clr;
    UTF16_T                             a_w2s_smart_cut_chars[TEXT_SMART_CUT_MAX_LENGHT] = {0};
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    UTF16_T                             a_w2s_force_break_skip_chars[] = WGL_FORCE_BREAK_SKIP_CHARS;
    UINT16                              ui2_smart_cut_len = 0;
    INT32                               i4_smart_cut_width = 0, i, i4_adv_x, i4_adv_y;
    UINT16                              ui2_keep_line_len;
    INT32                               i4_keep_line_width=0;
    BOOL                                b_replace_smart_cut;
    UTF16_T                             t_last_char;
    INT32                               i4_skip_x_start=0;
    INT32                               i4_skip_x_offset;
    UINT8                               ui1_adjusted_align;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT16                               i2_line_height;
 #ifdef ARAB_EXIST_SUPPORT
    BOOL								b_exist_arab = FALSE;
 #endif
    UINT8								ui1_cur_font_id;
	
    /*init background color*/
	_wgl_text_cnt_select_text_bg_clr(pt_cnt_data, ui2_state, &t_text_bg_clr);

    /*check if line infos are out of date*/
    if(pt_cnt_data->b_is_data_dirty)
    {
        if(0 != pt_cnt_data->ui2_text_length)
        {
            i4_ret = _wgl_text_cnt_layout_v_top(pt_cnt_data,ui2_state);

            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        
        pt_cnt_data->b_is_data_dirty = FALSE;
    }           
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
   if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    i4_ret = x_fe_get_cur_font_resource(t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
  	  return i4_ret;
    }
    if (ui1_cur_font_id != pt_cnt_data->ui1_cur_font_id)
    {
  	   pt_cnt_data->b_is_data_dirty = TRUE;
  	   pt_cnt_data->ui1_cur_font_id = ui1_cur_font_id;
    }
    /*initialize smart_cut flag*/
    b_replace_smart_cut = (pt_cnt_data->b_more_data) ? TRUE : FALSE;

    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    if (t_available_rect.i4_right <= t_available_rect.i4_left ||
        t_available_rect.i4_bottom <= t_available_rect.i4_top)
    {
        return WGLR_OK;
    }
    
    /*get widget height*/
    i4_widget_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    WGL_ASSERT(i4_widget_height);
    
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate the line number to paint*/
    i4_total_line_height = pt_cnt_data->i4_line_gap + i2_line_height;
    i4_ret = _wgl_text_cnt_calc_page_line(pt_cnt_data, &t_available_rect, ui2_state,&ui2_lines);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*check if anything to display anything*/
    if(ui2_lines == 0) 
    {
        ui2_lines = 1;/*in order to keep consistent with the behavior of single line style,
                                         that is to say UI  will display one line text at least 
                                         even if the widget can not contain the whole line text*/
        //return WGLR_OK;
    }   
    
    /*check if line number is over display line*/
    if(ui2_lines >= pt_cnt_data->ui2_lines_in_display)
    {
        ui2_lines = pt_cnt_data->ui2_lines_in_display;
    }
    else
    {
        /*available rectange cannot contain all lines*/
        b_replace_smart_cut = TRUE;
    }

    /*calculate the display height*/
    i4_display_height = (ui2_lines - 1) * i4_total_line_height + i2_line_height;  
    
    ui1_adjusted_align = 0;
    if(!pt_cnt_data->b_opposite_align)
    {
        ui1_adjusted_align = pt_cnt_data->ui1_align;
    }else
    {
       /*reverse text horizontal alignment for right to left text direction*/
        ui1_adjusted_align |= (WGL_IS_H_LEFT(pt_cnt_data->ui1_align)<<2);
        ui1_adjusted_align |= WGL_IS_H_CENTER(pt_cnt_data->ui1_align);
        ui1_adjusted_align |= (WGL_IS_H_RIGHT(pt_cnt_data->ui1_align)>>2);
        
        /*text direction dose not affect vertical alignment*/
        ui1_adjusted_align |= (pt_cnt_data->ui1_align&0x0F);
    }
    /*start line*/
    if(WGL_IS_V_TOP(ui1_adjusted_align))
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos;
        i4_y_start = t_available_rect.i4_top;
    }
    else if(WGL_IS_V_CENTER(ui1_adjusted_align))
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos + ((pt_cnt_data->ui2_lines_in_display - ui2_lines) >> 1);
        i4_y_start = t_available_rect.i4_top + ((i4_widget_height - i4_display_height) >> 1);
    }
    else /*V_BOTTOM*/
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos + pt_cnt_data->ui2_lines_in_display - ui2_lines;
        i4_y_start = t_available_rect.i4_bottom - i4_display_height;
    }

    if (pt_cnt_data->b_start_idx_valid && pt_cnt_data->ui2_start_line < pt_cnt_data->ui2_lines_in_display)
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos + pt_cnt_data->ui2_start_line;
    }

    /*get smart-cut info*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT))
    {
        pt_line_info_temp = pt_cur_line_info + ui2_lines - 1;  
        i4_ret = _wgl_text_cnt_get_smart_cut_info(pt_cnt_data,
                                                  t_text_font, 
                                                  pt_line_info_temp->w2s_string, 
                                                  (INT32)pt_line_info_temp->ui2_width, 
                                                  pt_line_info_temp->ui2_str_len, 
                                                  a_w2s_smart_cut_chars, 
                                                  &ui2_smart_cut_len, 
                                                  &i4_smart_cut_width, 
                                                  &ui2_keep_line_len,
                                                  &i4_keep_line_width);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        
        /*don't have to show smart-cut chars if the width is zero*/
        b_replace_smart_cut = (ui2_smart_cut_len > 0) ? b_replace_smart_cut : FALSE;
        if (b_replace_smart_cut)
        {
            pt_line_info_temp->ui2_str_len = ui2_keep_line_len;
            pt_line_info_temp->ui2_width   = (UINT16)i4_keep_line_width;
            
            if (ui2_smart_cut_len < x_uc_w2s_strlen(a_w2s_smart_cut_chars))
            {
                pt_line_info_temp->w2s_string = a_w2s_smart_cut_chars;
            }
        }
    }
    else
    {
        /*user turns off this function*/
        b_replace_smart_cut = FALSE;
    }
    
    /*paint lines*/
    for(ui2_cur_line = 0; ui2_cur_line < ui2_lines; ui2_cur_line++)
    {
        if(WGL_IS_H_RIGHT(ui1_adjusted_align))
        {
            if (b_replace_smart_cut && ui2_cur_line == ui2_lines - 1)
            {
                i4_x_start = t_available_rect.i4_right - pt_cur_line_info->ui2_width - i4_smart_cut_width;
            }
            else
            {
                i4_x_start = t_available_rect.i4_right - pt_cur_line_info->ui2_width;
            }
        }
        else if(WGL_IS_H_CENTER(ui1_adjusted_align))
        {
            if (b_replace_smart_cut && ui2_cur_line == ui2_lines - 1)
            {
                i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - pt_cur_line_info->ui2_width - i4_smart_cut_width) >> 1);
            }
            else
            {
                i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - pt_cur_line_info->ui2_width) >> 1);
            }
        }
        else /*H_LEFT*/
        {
            i4_x_start = t_available_rect.i4_left;
        }

        /*paint text*/
        if(b_replace_smart_cut && (ui2_cur_line == ui2_lines - (UINT16)1))
        {
            /*draw the last line*/          
            /*draw the smart-cut characters*/        
            if(!wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
            {

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
            {
                    INT32 i4_smart_cut_start = 0;
                    if (i4_smart_cut_width >= WGL_RECT_GET_WIDTH(&t_available_rect))
                    {
                        i4_smart_cut_start = i4_x_start;
                        ui2_draw_width = 0;
                    }
                    else if (pt_cur_line_info->ui2_width + i4_smart_cut_width > WGL_RECT_GET_WIDTH(&t_available_rect))
                    {
                        i4_smart_cut_start = i4_x_start + WGL_RECT_GET_WIDTH(&t_available_rect) - i4_smart_cut_width;
                        ui2_draw_width = pt_cur_line_info->ui2_width;
                    }
                    else
                    {
                        i4_smart_cut_start = i4_x_start + pt_cur_line_info->ui2_width;
                        ui2_draw_width = pt_cur_line_info->ui2_width;
                    }
                    
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                              i4_smart_cut_start,
                                              i4_y_start,
                                              i4_y_start,
                                              a_w2s_smart_cut_chars,
                                              ui2_smart_cut_len,
                                              NULL,
                                              0,
                                              t_text_font, 
                                              pt_selected_clr,
                                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                              pt_edge_clr,
                                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                              (UINT16)t_font_info.i2_width,
                                              0,
                                              (UINT16)i2_line_height,
                                              0);
                }
                else
                {
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          i4_x_start,
                                          i4_y_start,
                                          i4_y_start,
                                          a_w2s_smart_cut_chars,
                                          ui2_smart_cut_len,
                                          NULL,
                                          0,
                                          t_text_font, 
                                          pt_selected_clr,
                                          (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                          pt_edge_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          (UINT16)i2_line_height,
                                          0);  
                    ui2_draw_width = (pt_cur_line_info->ui2_width >(UINT16)i4_smart_cut_width) ? (pt_cur_line_info->ui2_width -(UINT16)i4_smart_cut_width) : (UINT16)0;
                }
                if(GLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return WGLR_TEXT_DRAW_FAIL;
                }
                
            }

            ui2_draw_len = ui2_keep_line_len;
        }
        else
        {
            ui2_draw_len = pt_cur_line_info->ui2_str_len;
            ui2_draw_width = pt_cur_line_info->ui2_width;
        }

        if(ui2_draw_len > 0 && ui2_draw_width > 0 && 
           !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
        {                        
          

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif        
            {
                t_last_char = pt_cur_line_info->w2s_string[ui2_draw_len - 1];
                i4_ret = x_gl_draw_ustring_ex(
                              h_graphics,
                              i4_x_start,
                              i4_y_start,
                              i4_y_start,
                              pt_cur_line_info->w2s_string,
                              ui2_draw_len,
                              (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                             (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                              t_text_font,
                              pt_selected_clr,
                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                              pt_edge_clr,
                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                              (UINT16)t_font_info.i2_width,
                              0,
                              (UINT16)i2_line_height,
                              0);
            }
            else
            {
                t_last_char = *(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len);
                i4_skip_x_offset = 0;
                if((ui2_cur_line == ui2_lines - (UINT16)1) && b_replace_smart_cut )
                {
                    i4_x_start += pt_cur_line_info->ui2_width -i4_keep_line_width;
                }
                if(a_w2s_skip_chars[0] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char, 
                                                    &i4_skip_x_offset, 
                                                    &i4_adv_y);                  
                }

                i4_ret = x_gl_draw_ustring_ex(
                              h_graphics,
                              i4_x_start + i4_skip_x_offset,
                              i4_y_start,
                              i4_y_start,
                              (UTF16_T*)(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len) ,
                              ui2_draw_len,
                              (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                              (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                              t_text_font,
                              pt_selected_clr,
                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                              pt_edge_clr,
                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                              (UINT16)t_font_info.i2_width,
                              0,
                              (UINT16)i2_line_height,
                              0);
            }
            
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }

            
            /*draw '-' when softhypen is the last char of a line */
            if(ui2_cur_line != (ui2_lines-(UINT16)1))
           	{
            for(i = (INT32)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1); i > 0; i--)
            {
                if(a_w2s_skip_chars[i - 1] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char, 
                                                    &i4_adv_x, 
                                                    &i4_adv_y);

                    if(i4_ret != FER_OK && FER_NO_CHAR_CODE != i4_ret)
                    {
                        i4_adv_x = 0;
                    }

 #ifdef ARAB_EXIST_SUPPORT
                    if (!b_exist_arab)
#else
                    if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif              
                    {
                        i4_skip_x_start = i4_x_start + pt_cur_line_info->ui2_width - i4_adv_x;
                    }
                    else
                    {
                        i4_skip_x_start = i4_x_start;
                    }
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                                  i4_skip_x_start,
                                                  i4_y_start,
                                                  i4_y_start,
                                                  &t_last_char,
                                                  1,
                                                  NULL,
                                                  0,
                                                  t_text_font,
                                                  pt_selected_clr,
                                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                                  pt_edge_clr,
                                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                                  (UINT16)t_font_info.i2_width,
                                                  0,
                                                  (UINT16)i2_line_height,
                                                  0);
                    if(GLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return WGLR_TEXT_DRAW_FAIL;
                    }
                }
            }
          }
        }
        
        i4_y_start += i4_total_line_height;
        pt_cur_line_info++;
        
    }
    
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_paint_fg_multiple_line_custom(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    INT32                               i4_ret;
    INT32                               i4_x_start;
    INT32                               i4_y_start;
    INT32                               i4_widget_height;
    INT32                               i4_total_line_height;
    INT32                               i4_display_height;
    UINT16                              ui2_draw_width = 0;
    UINT16                              ui2_draw_len;
    UINT16                              ui2_lines;
    UINT16                              ui2_cur_line;
    WGL_TEXT_LINE_INFO*                 pt_cur_line_info, *pt_line_info_temp;
    GL_RECT_T                           t_available_rect;
    GL_COLOR_T                          t_text_bg_clr;
    UTF16_T                             a_w2s_smart_cut_chars[TEXT_SMART_CUT_MAX_LENGHT] = {0};
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    UTF16_T                             a_w2s_force_break_skip_chars[] = WGL_FORCE_BREAK_SKIP_CHARS;
    UINT16                              ui2_smart_cut_len = 0;
    INT32                               i4_smart_cut_width = 0, i, i4_adv_x, i4_adv_y;
    UINT16                              ui2_keep_line_len;
    INT32                               i4_keep_line_width=0;
    BOOL                                b_replace_smart_cut;
    UTF16_T                             t_last_char;
    INT32                               i4_skip_x_start=0;
    INT32                               i4_skip_x_offset;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT16                               i2_line_height;
 #ifdef ARAB_EXIST_SUPPORT
    BOOL								b_exist_arab = FALSE;
 #endif
    UINT8								ui1_cur_font_id;
	  UINT32 ui4_max_line_len = 0;
    WGL_TEXT_LINE_INFO*                 pt_cur_line_info_tmp ;
    
    /*init background color*/
	_wgl_text_cnt_select_text_bg_clr(pt_cnt_data, ui2_state, &t_text_bg_clr);
    /*check if line infos are out of date*/
    if(pt_cnt_data->b_is_data_dirty)
    {
        if(0 != pt_cnt_data->ui2_text_length)
        {
            i4_ret = _wgl_text_cnt_layout_v_top(pt_cnt_data,ui2_state);

            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        
        pt_cnt_data->b_is_data_dirty = FALSE;
    }           
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
   if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    i4_ret = x_fe_get_cur_font_resource(t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
  	  return i4_ret;
    }
    if (ui1_cur_font_id != pt_cnt_data->ui1_cur_font_id)
    {
  	   pt_cnt_data->b_is_data_dirty = TRUE;
  	   pt_cnt_data->ui1_cur_font_id = ui1_cur_font_id;
    }
    /*initialize smart_cut flag*/
    b_replace_smart_cut = (pt_cnt_data->b_more_data) ? TRUE : FALSE;

    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    if (t_available_rect.i4_right <= t_available_rect.i4_left ||
        t_available_rect.i4_bottom <= t_available_rect.i4_top)
    {
        return WGLR_OK;
    }
    
    /*get widget height*/
    i4_widget_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    WGL_ASSERT(i4_widget_height);
    
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate the line number to paint*/
    i4_total_line_height = pt_cnt_data->i4_line_gap + i2_line_height;
    i4_ret = _wgl_text_cnt_calc_page_line(pt_cnt_data, &t_available_rect, ui2_state,&ui2_lines);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*check if anything to display anything*/
    if(ui2_lines == 0) 
    {
        ui2_lines = 1;/*in order to keep consistent with the behavior of single line style,
                                         that is to say UI  will display one line text at least 
                                         even if the widget can not contain the whole line text*/
        //return WGLR_OK;
    }   
    
    /*check if line number is over display line*/
    if(ui2_lines >= pt_cnt_data->ui2_lines_in_display)
    {
        ui2_lines = pt_cnt_data->ui2_lines_in_display;
    }
    else
    {
        /*available rectange cannot contain all lines*/
        b_replace_smart_cut = TRUE;
    }

    /*calculate the display height*/
    i4_display_height = (ui2_lines - 1) * i4_total_line_height + i2_line_height;  
   
    /*start line*/
    if(WGL_IS_V_TOP(pt_cnt_data->ui1_align))
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos;
        i4_y_start = t_available_rect.i4_top;
    }
    else if(WGL_IS_V_CENTER(pt_cnt_data->ui1_align))
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos + ((pt_cnt_data->ui2_lines_in_display - ui2_lines) >> 1);
        i4_y_start = t_available_rect.i4_top + ((i4_widget_height - i4_display_height) >> 1);
    }
    else /*V_BOTTOM*/
    {
        pt_cur_line_info = pt_cnt_data->pt_line_infos + pt_cnt_data->ui2_lines_in_display - ui2_lines;
        i4_y_start = t_available_rect.i4_bottom - i4_display_height;
    }

    /*get smart-cut info*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT))
    {
        pt_line_info_temp = pt_cur_line_info + ui2_lines - 1;  
        i4_ret = _wgl_text_cnt_get_smart_cut_info(pt_cnt_data,
                                                  t_text_font, 
                                                  pt_line_info_temp->w2s_string, 
                                                  (INT32)pt_line_info_temp->ui2_width, 
                                                  pt_line_info_temp->ui2_str_len, 
                                                  a_w2s_smart_cut_chars, 
                                                  &ui2_smart_cut_len, 
                                                  &i4_smart_cut_width, 
                                                  &ui2_keep_line_len,
                                                  &i4_keep_line_width);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        
        /*don't have to show smart-cut chars if the width is zero*/
        b_replace_smart_cut = (ui2_smart_cut_len > 0) ? b_replace_smart_cut : FALSE;
        if (b_replace_smart_cut)
        {
            pt_line_info_temp->ui2_str_len = ui2_keep_line_len;
            pt_line_info_temp->ui2_width   = (UINT16)i4_keep_line_width;
            
            if (ui2_smart_cut_len < x_uc_w2s_strlen(a_w2s_smart_cut_chars))
            {
                pt_line_info_temp->w2s_string = a_w2s_smart_cut_chars;
            }
        }
    }
    else
    {
        /*user turns off this function*/
        b_replace_smart_cut = FALSE;
    }
    pt_cur_line_info_tmp = pt_cur_line_info;
    for(ui2_cur_line = 0; ui2_cur_line < ui2_lines; ui2_cur_line++)
    {
        if(ui4_max_line_len < pt_cur_line_info_tmp->ui2_width)
        {
            ui4_max_line_len = pt_cur_line_info_tmp->ui2_width;
        }
        pt_cur_line_info_tmp++;
    }
    /*paint lines*/
    for(ui2_cur_line = 0; ui2_cur_line < ui2_lines; ui2_cur_line++)
    {
        i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - ui4_max_line_len) >> 1);

       /*paint text*/
        if(b_replace_smart_cut && (ui2_cur_line == ui2_lines - (UINT16)1))
        {
            /*draw the last line*/          
            /*draw the smart-cut characters*/        
            if(!wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
            {

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
            {
                    INT32 i4_smart_cut_start = 0;
                    if (i4_smart_cut_width >= WGL_RECT_GET_WIDTH(&t_available_rect))
                    {
                        i4_smart_cut_start = i4_x_start;
                        ui2_draw_width = 0;
                    }
                    else if (pt_cur_line_info->ui2_width + i4_smart_cut_width > WGL_RECT_GET_WIDTH(&t_available_rect))
                    {
                        i4_smart_cut_start = i4_x_start + WGL_RECT_GET_WIDTH(&t_available_rect) - i4_smart_cut_width;
                        ui2_draw_width = pt_cur_line_info->ui2_width;
                    }
                    else
                    {
                        i4_smart_cut_start = i4_x_start + pt_cur_line_info->ui2_width;
                        ui2_draw_width = pt_cur_line_info->ui2_width;
                    }
                    
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                              i4_smart_cut_start,
                                              i4_y_start,
                                              i4_y_start,
                                              a_w2s_smart_cut_chars,
                                              ui2_smart_cut_len,
                                              NULL,
                                              0,
                                              t_text_font, 
                                              pt_selected_clr,
                                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                              pt_edge_clr,
                                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                              (UINT16)t_font_info.i2_width,
                                              0,
                                              (UINT16)i2_line_height,
                                              0);
                }
                else
                {
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          i4_x_start,
                                          i4_y_start,
                                          i4_y_start,
                                          a_w2s_smart_cut_chars,
                                          ui2_smart_cut_len,
                                          NULL,
                                          0,
                                          t_text_font, 
                                          pt_selected_clr,
                                          (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                          pt_edge_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          (UINT16)i2_line_height,
                                          0);  
                    ui2_draw_width = (pt_cur_line_info->ui2_width >(UINT16)i4_smart_cut_width) ? (pt_cur_line_info->ui2_width -(UINT16)i4_smart_cut_width) : (UINT16)0;
                }
                if(GLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return WGLR_TEXT_DRAW_FAIL;
                }
                
            }

            ui2_draw_len = ui2_keep_line_len;
        }
        else
        {
            ui2_draw_len = pt_cur_line_info->ui2_str_len;
            ui2_draw_width = pt_cur_line_info->ui2_width;
        }

        if(ui2_draw_len > 0 && ui2_draw_width > 0 && 
           !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
        {                        
          

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif        
            {
                t_last_char = pt_cur_line_info->w2s_string[ui2_draw_len - 1];
                i4_ret = x_gl_draw_ustring_ex(
                              h_graphics,
                              i4_x_start,
                              i4_y_start,
                              i4_y_start,
                              pt_cur_line_info->w2s_string,
                              ui2_draw_len,
                              (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                             (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                              t_text_font,
                              pt_selected_clr,
                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                              pt_edge_clr,
                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                              (UINT16)t_font_info.i2_width,
                              0,
                              (UINT16)i2_line_height,
                              0);
            }
            else
            {
                t_last_char = *(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len);
                i4_skip_x_offset = 0;
                if((ui2_cur_line == ui2_lines - (UINT16)1) && b_replace_smart_cut )
                {
                    i4_x_start += pt_cur_line_info->ui2_width -i4_keep_line_width;
                }
                if(a_w2s_skip_chars[0] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char,
                                                    &i4_skip_x_offset, 
                                                    &i4_adv_y);                  
                }

                i4_ret = x_gl_draw_ustring_ex(
                              h_graphics,
                              i4_x_start + i4_skip_x_offset,
                              i4_y_start,
                              i4_y_start,
                              (UTF16_T*)(pt_cur_line_info->w2s_string + pt_cur_line_info->ui2_str_len - ui2_draw_len) ,
                              ui2_draw_len,
                              (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                              (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                              t_text_font,
                              pt_selected_clr,
                              (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                              pt_edge_clr,
                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                              (UINT16)t_font_info.i2_width,
                              0,
                              (UINT16)i2_line_height,
                              0);
            }
            
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }

            
            /*draw '-' when softhypen is the last char of a line */
            if(ui2_cur_line != (ui2_lines-(UINT16)1))
         {
            for(i = (INT32)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1); i > 0; i--)
            {
                if(a_w2s_skip_chars[i - 1] == t_last_char)
                {
                    /*get advance */
                    i4_ret = x_fe_get_character_size(t_text_font, 
                                                    (CHAR_CODE)t_last_char,
                                                    &i4_adv_x, 
                                                    &i4_adv_y);

                    if(i4_ret != FER_OK && FER_NO_CHAR_CODE != i4_ret)
                    {
                        i4_adv_x = 0;
                    }

 #ifdef ARAB_EXIST_SUPPORT
                    if (!b_exist_arab)
#else
                    if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif              
                    {
                        i4_skip_x_start = i4_x_start + pt_cur_line_info->ui2_width - i4_adv_x;
                    }
                    else
                    {
                        i4_skip_x_start = i4_x_start;
                    }
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                                  i4_skip_x_start,
                                                  i4_y_start,
                                                  i4_y_start,
                                                  &t_last_char,
                                                  1,
                                                  NULL,
                                                  0,
                                                  t_text_font,
                                                  pt_selected_clr,
                                                  (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                                  pt_edge_clr,
                                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                                  (UINT16)t_font_info.i2_width,
                                                  0,
                                                  (UINT16)i2_line_height,
                                                  0);
                    if(GLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                        return WGLR_TEXT_DRAW_FAIL;
                    }
                }
            }
        }
        }
        i4_y_start += i4_total_line_height;
        pt_cur_line_info++;
        
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
static INT32 _wgl_text_cnt_paint_fg_single_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    UTF16_T                             a_w2s_force_break_skip_chars[] = WGL_FORCE_BREAK_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};    
    INT32                               i4_ret;
    INT32                               i4_x_start;
    INT32                               i4_y_start;
    GL_RECT_T                           t_available_rect;
    GL_COLOR_T                          t_text_bg_clr;
    UTF16_T                             a_w2s_smart_cut_chars[TEXT_SMART_CUT_MAX_LENGHT] = {0};
    UINT16                              ui2_smart_cut_len = 0;
    INT32                               i4_smart_cut_width = 0;
    UINT16                              ui2_keep_line_len;
    UINT16                              ui2_draw_len = 0;
    UINT16                              ui2_len_capacity;
    BOOL                                b_replace_smart_cut=FALSE;  
    INT32                               i4_keep_line_width=0;
    UTF16_T                             t_last_char=(UTF16_T)0;
    UINT8                               ui1_adjusted_align;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT16                               i2_line_height;
    BOOL                                b_exist_arab = FALSE;
    UINT8								ui1_cur_font_id;
    

    
     /*init background color*/
	_wgl_text_cnt_select_text_bg_clr(pt_cnt_data, ui2_state, &t_text_bg_clr);
	 
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    if (t_available_rect.i4_right <= t_available_rect.i4_left ||
        t_available_rect.i4_bottom <= t_available_rect.i4_top)
    {
        return WGLR_OK;
    }

    /*get line width*/
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);

    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);

    b_exist_arab = check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length);

    i4_ret = x_fe_get_cur_font_resource(t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    if (ui1_cur_font_id != pt_cnt_data->ui1_cur_font_id)
    {
		 pt_cnt_data->b_is_data_dirty = TRUE;
		 pt_cnt_data->ui1_cur_font_id = ui1_cur_font_id;
    }

    /*check if line infos are out of date*/
    if(pt_cnt_data->b_is_data_dirty)
    {
        /*Get text's necessary width*/
        if(t_break_line_Info.i4_box_width > 0)
        {
            t_break_line_Info.h_font = t_text_font;
            t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
            /*CR2637, in single line style, print characters as many as possible.*/
            t_break_line_Info.w2s_can_break = NULL; /*a_w2s_break_chars;*/
           
            if (!b_exist_arab)              
            {
                t_break_line_Info.w2s_string = pt_cnt_data->pt_w2_text;
                t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_text_length;
            }
            else
            {
                 i4_ret = _wgl_text_cnt_update_arab_text(pt_cnt_data);
				 if (WGLR_OK != i4_ret)
		  	     {
		  	         return i4_ret;
		  	     }   
                 /*reset reorder attr after used for current string content*/
                 pt_cnt_data->t_arab_reorder_attr.b_find_init_level = TRUE;
                 pt_cnt_data->t_arab_reorder_attr.ui1_embedding_level = 0;
                 
		         t_break_line_Info.w2s_string = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity - 1]);
		         t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_arab_dsp_text_len;                    
            }  

            /*for soft-hyphen*/
            t_break_line_Info.w2s_skip = a_w2s_skip_chars;
            t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
            t_break_line_Info.ui2_skip_width = 0;
            t_break_line_Info.b_skip_as_break = FALSE;
#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
            {
                i4_ret = wgl_get_break_line_pos(&t_break_line_Info, NULL, &t_break_line_result);
            }
            else
            {
                i4_ret = wgl_get_break_line_pos_for_arab(
                            t_break_line_Info.w2s_string ,
                            &t_break_line_Info, 
                            NULL,
                            &t_break_line_result);
            }
                
            
            if(i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }

        /*data more then one line*/
        pt_cnt_data->b_more_data = (NULL == t_break_line_result.w2s_next_start) ? FALSE : TRUE;
        pt_cnt_data->pt_line_infos->ui2_width = t_break_line_result.ui2_width;
        pt_cnt_data->pt_line_infos->ui2_str_len = t_break_line_result.ui2_char_number;
        pt_cnt_data->ui2_lines_in_display = 1;
        
        /*store line info*/      

#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
        
       {
            pt_cnt_data->pt_line_infos->w2s_string = pt_cnt_data->pt_w2_text;
            if (t_break_line_result.ui2_char_number - 1 >= 0)
            {
           		 t_last_char = pt_cnt_data->pt_w2_text[t_break_line_result.ui2_char_number - 1];  
            }
        }
        else
        {
            pt_cnt_data->pt_line_infos->w2s_string = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity -t_break_line_result.ui2_char_number]);
            t_last_char = *(pt_cnt_data->pt_line_infos->w2s_string);           
        }      

        if(TEXT_END_OF_NEWLINE & pt_cnt_data->ui4_extra_flag && 0 == (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag))
        {
            if(t_break_line_result.ui2_char_number > 0 && (a_w2s_force_chars[0] == t_last_char))  /*break because of force-break character*/              
            {   
                pt_cnt_data->b_more_data = FALSE;
            }
        }
        if (a_w2s_force_chars[0] == t_last_char)
        {              
           pt_cnt_data->pt_line_infos->ui2_str_len -= 1;
        }
#ifdef ARAB_EXIST_SUPPORT
        if (b_exist_arab)
#else
        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_RIGHT_TO_LEFT )
#endif
       {   
            if (a_w2s_force_chars[0] == t_last_char)
            {
               pt_cnt_data->pt_line_infos->w2s_string += 1;               
            }
        }      
        
        /*reset dirty flag*/
        pt_cnt_data->b_is_data_dirty = FALSE;
    }   
    ui1_adjusted_align = 0;

    if(!pt_cnt_data->b_opposite_align)
    {
        ui1_adjusted_align = pt_cnt_data->ui1_align;
    }
    else
    {
       /*reverse text horizontal alignment for right to left text direction*/
        ui1_adjusted_align |= (WGL_IS_H_LEFT(pt_cnt_data->ui1_align)<<2);
        ui1_adjusted_align |= WGL_IS_H_CENTER(pt_cnt_data->ui1_align);
        ui1_adjusted_align |= (WGL_IS_H_RIGHT(pt_cnt_data->ui1_align)>>2);
        
        /*text direction dose not affect vertical alignment*/
        ui1_adjusted_align |= (pt_cnt_data->ui1_align&0x0F);
    }

    if (pt_cnt_data->b_more_data && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT))
    {
        i4_ret = _wgl_text_cnt_get_smart_cut_info(
                     pt_cnt_data,
                     t_text_font, 
                     pt_cnt_data->pt_line_infos->w2s_string, 
                     (INT32)(pt_cnt_data->pt_line_infos->ui2_width), 
                     pt_cnt_data->pt_line_infos->ui2_str_len, 
                     a_w2s_smart_cut_chars, 
                     &ui2_smart_cut_len, 
                     &i4_smart_cut_width, 
                     &ui2_keep_line_len,
                     &i4_keep_line_width);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
       b_replace_smart_cut = (ui2_smart_cut_len > 0) ? TRUE : FALSE;

       if (b_replace_smart_cut)
       {
           if (ui2_smart_cut_len < x_uc_w2s_strlen(a_w2s_smart_cut_chars))
           {
               pt_cnt_data->pt_line_infos->w2s_string = a_w2s_smart_cut_chars;
           }
           pt_cnt_data->pt_line_infos->ui2_str_len = ui2_keep_line_len;
           pt_cnt_data->pt_line_infos->ui2_width = (UINT16)i4_keep_line_width;
       }
       
    }
    
    /*calculate y offset*/
    if(WGL_IS_V_TOP(ui1_adjusted_align))
    {
        i4_y_start = t_available_rect.i4_top;
    }
    else if(WGL_IS_V_CENTER(ui1_adjusted_align))
    {
        i4_y_start = t_available_rect.i4_top + ((WGL_RECT_GET_HEIGHT(&t_available_rect) - i2_line_height) >> 1);
    }
    else
    {
        i4_y_start = t_available_rect.i4_bottom - i2_line_height;
    }

    /*calculate x offset*/
    if(WGL_IS_H_RIGHT(ui1_adjusted_align))
    {
        if (b_replace_smart_cut)
        {
            i4_x_start = t_available_rect.i4_right - i4_keep_line_width - i4_smart_cut_width;
        }
        else
        {
            i4_x_start = t_available_rect.i4_right - pt_cnt_data->pt_line_infos->ui2_width;
        }
    }
    else if(WGL_IS_H_CENTER(ui1_adjusted_align))
    {
        if (b_replace_smart_cut)
        {
            i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - i4_keep_line_width - i4_smart_cut_width) >> 1);
        }
        else
        {
            i4_x_start = t_available_rect.i4_left + ((WGL_RECT_GET_WIDTH(&t_available_rect) - pt_cnt_data->pt_line_infos->ui2_width) >> 1);
        }
    }
    else
    {
        i4_x_start = t_available_rect.i4_left;
    }

    /*paint lines*/
    if(b_replace_smart_cut)
    {
        /*draw the last line*/
        /*draw the smart-cut characters*/    
        if(!wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
        {
#ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
             {
                INT32 i4_smart_cut_start = 0;
                if (i4_smart_cut_width >= WGL_RECT_GET_WIDTH(&t_available_rect))
                {
                    i4_smart_cut_start = i4_x_start;
                }
                else if (pt_cnt_data->pt_line_infos->ui2_width + i4_smart_cut_width > WGL_RECT_GET_WIDTH(&t_available_rect))
                {
                    i4_smart_cut_start = i4_x_start + WGL_RECT_GET_WIDTH(&t_available_rect) - i4_smart_cut_width;
                }
                else
                {
                    i4_smart_cut_start = i4_x_start + pt_cnt_data->pt_line_infos->ui2_width;
                }
                
                i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          i4_smart_cut_start,
                                          i4_y_start,
                                          i4_y_start,
                                          a_w2s_smart_cut_chars,
                                          ui2_smart_cut_len,
                                          (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                          (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                          t_text_font,
                                          pt_selected_clr,
                                          (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                          pt_edge_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)(t_font_info.i2_width),
                                          0,
                                          (UINT16)i2_line_height,
                                          0);      
            }
            else{
                 i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          i4_x_start,
                                          i4_y_start,
                                          i4_y_start,
                                          a_w2s_smart_cut_chars,
                                          ui2_smart_cut_len,
                                          (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                          (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                          t_text_font,
                                          pt_selected_clr,
                                          (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                          pt_edge_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          (UINT16)i2_line_height,
                                          0);  
            }
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }

        ui2_draw_len = ui2_keep_line_len;
    }/*if(pt_cnt_data->b_more_data && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT))*/
    else
    {
        ui2_draw_len = pt_cnt_data->pt_line_infos->ui2_str_len;
    }

    if( ui2_draw_len > 0        &&
        !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
    {
 #ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif     
            {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                      i4_x_start,
                                      i4_y_start,
                                      i4_y_start,
                                      pt_cnt_data->pt_line_infos->w2s_string,
                                      ui2_draw_len,
                                      (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                      (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                      t_text_font,
                                      pt_selected_clr,
                                      (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                      pt_edge_clr,
                                      (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                      (UINT16)t_font_info.i2_width,
                                      0,
                                      (UINT16)i2_line_height,
                                      0);
        }
        else
        {
            
             if(b_replace_smart_cut )
             {
                i4_x_start += pt_cnt_data->pt_line_infos->ui2_width -i4_keep_line_width;
             }            
             i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                      i4_x_start,
                                      i4_y_start,
                                      i4_y_start,
                                      pt_cnt_data->pt_line_infos->w2s_string + pt_cnt_data->pt_line_infos->ui2_str_len - ui2_draw_len,
                                      ui2_draw_len,
                                      (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? a_w2s_force_break_skip_chars : a_w2s_skip_chars,
                                      (UINT16)((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (sizeof(a_w2s_force_break_skip_chars)/sizeof(UTF16_T) - 1) : (sizeof(a_w2s_skip_chars)/sizeof(UTF16_T) - 1)),
                                      t_text_font,
                                      pt_selected_clr,
                                      (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                      pt_edge_clr,
                                      (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                      (UINT16)(t_font_info.i2_width),
                                      0,
                                      (UINT16)i2_line_height,
                                      0);
        }
        if(GLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
            return WGLR_TEXT_DRAW_FAIL;
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
static INT32 _wgl_text_cnt_paint_fg_single_line_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    const GL_COLOR_T*                   pt_selected_clr,
    const GL_COLOR_T*                   pt_edge_clr,
    UINT16                              ui2_state)
{
    
    INT32                               i4_ret;
    INT32                               i4_y_start;
    GL_RECT_T                           t_available_rect;
    GL_COLOR_T                          t_text_bg_clr;
    UTF16_T*                            w2s_string;
    UINT16                              ui2_len_capacity;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT16                               i2_line_height;
     i4_ret = WGLR_OK;
     if (pt_cnt_data->ui2_scrl_len == 0)
     {
         return WGLR_OK;
     }
   
    /*init background color*/
	_wgl_text_cnt_select_text_bg_clr(pt_cnt_data, ui2_state, &t_text_bg_clr);
	 
     /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);  
       
    /*calculate y offset*/
    if(WGL_IS_V_TOP(pt_cnt_data->ui1_align))
    {
        i4_y_start = t_available_rect.i4_top;
    }
    else if(WGL_IS_V_CENTER(pt_cnt_data->ui1_align))
    {
        i4_y_start = t_available_rect.i4_top + ((WGL_RECT_GET_HEIGHT(&t_available_rect) - i2_line_height) >> 1);
    }
    else
    {
        i4_y_start = t_available_rect.i4_bottom - i2_line_height;
    }    

    if(!wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, pt_selected_clr))
    {
        if (WGL_TEXT_DIR_LEFT_TO_RIGHT == pt_cnt_data->ui1_read_direction)
        {
            w2s_string = &(pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx]);            
        }
        else
        {
            ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
            w2s_string = &(pt_cnt_data->pt_w2_text[ui2_len_capacity -  pt_cnt_data->ui2_scrl_begin_idx - pt_cnt_data->ui2_scrl_len]);   
        }
        i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                     (INT32) pt_cnt_data->ui2_scrl_start_x,
                                      i4_y_start,
                                      i4_y_start,
                                      w2s_string,
                                      pt_cnt_data->ui2_scrl_len,
                                      NULL,
                                      0,
                                      t_text_font,
                                      pt_selected_clr,
                                      (t_text_bg_clr.a == 0 )?  NULL : (&t_text_bg_clr),
                                      pt_edge_clr,
                                      (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                      (UINT16)t_font_info.i2_width,
                                      0,
                                      (UINT16)i2_line_height,
                                      0);        
        if(GLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to draw string. (%d) File=%d,Line=%d\n\n", i4_ret,__FILE__, __LINE__));
            return WGLR_TEXT_DRAW_FAIL;
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
static INT32 _wgl_text_cnt_set_img(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_IMG_INFO_T*           pt_img_info)
{   

    DBG_INFO(("{WGL(TEXT_CNT)} pt_img_info = 0x%.8x   e_type = 0x%.8x\n"
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

    /*check target*/
    if(WGL_IMG_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    /*check pt_img_info*/
    if(pt_img_info && FALSE == wgl_is_legal_img_info(pt_img_info))
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*update background image*/
    if(pt_img_info)
    {
        COPY_IMG_INFO_TO_STD(&pt_cnt_data->t_img_bk, pt_img_info, NULL_HANDLE);

        /*remove the flag for action image*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT);
    }
    else
    {
        x_memset(&pt_cnt_data->t_img_bk, 0, sizeof(WGL_IMG_STD_SET_T));

        /*reset the flag for bk image*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT);

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
static INT32 _wgl_text_cnt_get_img(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    WGL_IMG_INFO_T*                 pt_img_info)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL == pt_img_info)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    /*check and cache images*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT))
    {
        i4_ret = _wgl_text_cnt_prepare_img(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret; 
        }    
    }

    /*prepare return value*/
    SET_STANDARD_TO_IMG_INFO(pt_img_info, &pt_cnt_data->t_img_bk);
    
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
static INT32 _wgl_text_cnt_set_color(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_COLOR_INFO_T*         pt_clr_info)
{
    /*local variable variable*/
    GL_COLOR_T                      t_default_color;

    DBG_INFO(("{WGL(TEXT_CNT)} pt_clr_info = 0x%.8x   e_type = 0x%.8x\n"
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
    
    /*check target*/
    if(WGL_CLR_BK == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy bk color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_bk, pt_clr_info, t_default_color);

            /*remove the flag for bk color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for bk color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT);
        }
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy text color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_text, pt_clr_info, t_default_color);

            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for text color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT);
        }
    }
    else if(WGL_CLR_EDGE == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy text color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_edge, pt_clr_info, t_default_color);
            
            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for text color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT);
        }
    }
	else if (WGL_CLR_TEXT_BG == ui1_target)
	{
		if(pt_clr_info)
        {
            /*copy text color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_text_bg, pt_clr_info, t_default_color);
            
            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_BG_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for text color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_BG_CLR_NOT_INIT);
        }
	}
    else
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
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
static INT32 _wgl_text_cnt_get_color(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    WGL_COLOR_INFO_T*               pt_clr_info)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL == pt_clr_info)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid input parameter. For WGL_CMD_GL_GET_COLOR, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check target*/
    if(WGL_CLR_BK == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_text_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }

        /*prepare return value*/
        SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_bk);
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_text_cnt_use_sys_text_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }

        /*prepare return value*/
        SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_text);
    }
    else if(WGL_CLR_EDGE == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_text_cnt_use_sys_edge_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        
        /*prepare return value*/
        SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_edge);
    }
	else if (WGL_CLR_TEXT_BG == ui1_target)
	{		
		if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT))
	    {
	    	WGL_COLOR_STD_SET_T  t_text_bg_color;
	        /*get system colors*/
	        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &(t_text_bg_color.t_disable));
	        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &(t_text_bg_color.t_enable));
	        wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &(t_text_bg_color.t_highlight));
			SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &t_text_bg_color);
	    }
		else
		{
			/*prepare return value*/
        	SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_text_bg);
		}
        
	}
    else
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
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
static INT32 _wgl_text_cnt_set_text(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT16                          ui2_length,
    const UTF16_T*                  w2s_str,
    BOOL                            b_append)
{
    /*local variable declaration*/
    UTF16_T                         a_w2s_smart_cut_char[TEXT_SMART_CUT_MAX_LENGHT] = {0};
    UTF16_T*                        w2s_copy_start;
    UINT16                          ui2_smart_cut_len;
    BOOL                            b_small_cut = FALSE;
    UINT16                          ui2_len, ui2_len_capacity, ui2_copy_len;
    INT32                           i4_ret;    
    UINT32                          ui4_text_byte;
   // UINT16                          ui2_overflow_len = 0;

    if (NULL == w2s_str && b_append)
    {
        return WGLR_OK;
    }

    
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);

    if (NULL == w2s_str) /*not append*/
    {
        /*reset user string*/ 
        ui4_text_byte =  (UINT32)(pt_cnt_data->ui2_text_length * sizeof(UTF16_T));
            x_memset(pt_cnt_data->pt_w2_text, 0, (SIZE_T)ui4_text_byte);    
        pt_cnt_data->ui2_text_length = 0;
    }
    else
    {
        
        /*get text length and compare it with input param ui2_length */
        ui2_copy_len = (UINT16)x_uc_w2s_strlen(w2s_str);
        ui2_copy_len = (ui2_length > ui2_copy_len) ? ui2_copy_len : ui2_length;

        if (b_append)
        {
            ui2_len = ui2_copy_len + pt_cnt_data->ui2_text_length;                   
        }
        else
        {
            ui2_len = ui2_copy_len;            
        }

        /*check if i4_len > i4_len_capacity*/
        if (ui2_len > ui2_len_capacity)
        {
            //ui2_overflow_len = ui2_len - ui2_len_capacity;
            b_small_cut = (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT_OVER_MAX)) ? TRUE : FALSE;

            /*truncate string when the input is over maximum*/
            if (b_append)
            {
                ui2_copy_len = ui2_len_capacity - pt_cnt_data->ui2_text_length;
            }
            else
            {
                ui2_copy_len = ui2_len_capacity;
            }

            ui2_len = ui2_len_capacity;
        }  

        /*calculate copy start position of a_w2_text*/
        if (b_append)
        {
           
                w2s_copy_start = &pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_text_length];   
                    
        }
        else
        {
                w2s_copy_start = &pt_cnt_data->pt_w2_text[0];  
        }
        
        if (ui2_copy_len > 0)
        {
            /*copy string*/
                x_memcpy(w2s_copy_start, w2s_str, (SIZE_T)(ui2_copy_len * sizeof(UTF16_T))); 
            
        }
        
        /*replace with smart-cut character*/
        if (b_small_cut)
        {
            i4_ret = _wgl_text_cnt_get_smart_cut_char(a_w2s_smart_cut_char, &ui2_smart_cut_len);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }

            ui2_smart_cut_len = (ui2_smart_cut_len < ui2_len) ? ui2_smart_cut_len : ui2_len;
            if (ui2_smart_cut_len < 5)
            {
                x_memcpy(&pt_cnt_data->pt_w2_text[ui2_len - ui2_smart_cut_len], a_w2s_smart_cut_char, ui2_smart_cut_len * sizeof(UTF16_T));
            }
        }
    
        /*clean old data*/
        if (ui2_len < pt_cnt_data->ui2_text_length)
        {            
           
            x_memset(&pt_cnt_data->pt_w2_text[ui2_len], 0, sizeof(UTF16_T) * (pt_cnt_data->ui2_text_length - ui2_len));
        }
        
        /*keep length*/
        pt_cnt_data->ui2_text_length = ui2_len;
    }
    
    /*update dirty flag*/
    pt_cnt_data->ui2_start_line = 0;
    _wgl_text_cnt_reset_dirty_flag(pt_cnt_data, TEXT_TOTAL_LINES_NOT_INIT);

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
static INT32 _wgl_text_cnt_set_font(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    const WGL_FONT_INFO_T*          pt_font_info)
{
    INT32                           i4_ret = WGLR_OK;

    DBG_INFO(("{WGL(TEXT_CNT)} pt_font_info = 0x%.8x   e_font_cmap =  0x%.8x\n"
                             "\te_font_size = 0x%.8x   e_font_style = 0x%.8x\n"
                             "\tpt_font_info->a_c_font_name = %s\n",
                             pt_font_info,
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_cmap),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_size),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_style),
                             ((NULL == pt_font_info) ? "\0" : pt_font_info->a_c_font_name)));

    /*reset dirty flag*/
    _wgl_text_cnt_reset_dirty_flag(pt_cnt_data, TEXT_TOTAL_LINES_NOT_INIT);
    

    if(pt_font_info)
    {
        /*update pt_cnt_data->t_text_font,pt_cnt_data->t_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, pt_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
       /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, pt_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        /*remove the flag for font*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT);
    }
    else
    {
         /*free pt_cnt_data->t_text_font and clear pt_cnt_data->t_font_info*/
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_font);
            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
        /*free pt_cnt_data->t_text_focus_font and clear pt_cnt_data->t_focus_font_info*/
       x_memset(&pt_cnt_data->t_focus_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_focus_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }

        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT);
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
static INT32 _wgl_text_cnt_get_font(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    WGL_FONT_INFO_T*                pt_font_info)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
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
static INT32 _wgl_text_cnt_set_font_ex(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    const WGL_FONT_INFO_EX_T*       pt_font_info_ex)
{
    INT32                           i4_ret = WGLR_OK;
   
    /*check parameter*/
    if(NULL != pt_font_info_ex && pt_font_info_ex->e_type != WGL_FONT_SET_BASIC)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_SET_FONT_EX (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    /*reset dirty flag*/
    _wgl_text_cnt_reset_dirty_flag(pt_cnt_data, TEXT_TOTAL_LINES_NOT_INIT);
    

    if(pt_font_info_ex)
    {
        /*update pt_cnt_data->t_text_font,pt_cnt_data->t_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, &(pt_font_info_ex->u_font_data.t_basic.t_unfocused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
       /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, &(pt_font_info_ex->u_font_data.t_basic.t_focused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        /*remove the flag for font*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT);
    }
    else
    {
         /*free pt_cnt_data->t_text_font and clear pt_cnt_data->t_font_info*/
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_font);
            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
        /*free pt_cnt_data->t_text_focus_font and clear pt_cnt_data->t_focus_font_info*/
       x_memset(&pt_cnt_data->t_focus_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_focus_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }

        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT);
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
static INT32 _wgl_text_cnt_get_font_ex(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    WGL_FONT_INFO_EX_T*             pt_font_info_ex)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL == pt_font_info_ex)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT_EX, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
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
static INT32 _wgl_text_cnt_set_text_dir(
    WGL_TEXT_CONTENT_T*                  pt_cnt_data,
    UINT8                                ui1_text_dir)
{
   // UINT16                      ui2_len_capacity;
   // UINT16                      ui2_idx;
    
    if (ui1_text_dir != WGL_TEXT_DIR_LEFT_TO_RIGHT && ui1_text_dir != WGL_TEXT_DIR_RIGHT_TO_LEFT)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to set text direction. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    else
    {
        if (!(pt_cnt_data->ui4_extra_flag & TEXT_FIX_DIR) &&
			pt_cnt_data->ui1_read_direction != ui1_text_dir
			)
        {
            /*re-store text content according to new text direction*/
            #if 0
            if(pt_cnt_data->ui2_text_length>0)
            {
                 ui2_len_capacity = _TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
                 if(ui1_text_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT)
                {
                    for(ui2_idx = 0;ui2_idx < pt_cnt_data->ui2_text_length; ui2_idx++)
                    {
                        pt_cnt_data->a_w2_text[ui2_idx] = pt_cnt_data->a_w2_text[ui2_len_capacity - pt_cnt_data->ui2_text_length + ui2_idx];
                    }
                    /*clean old data*/
                    x_memset(&pt_cnt_data->a_w2_text[pt_cnt_data->ui2_text_length],0,sizeof(UTF16_T)*(ui2_len_capacity - pt_cnt_data->ui2_text_length));
                }else
                {
					if (check_exist_arabic( pt_cnt_data->a_w2_text, pt_cnt_data->ui2_text_length))
					{
					
                    for(ui2_idx = 0;ui2_idx < pt_cnt_data->ui2_text_length; ui2_idx++)
                    {
                        pt_cnt_data->a_w2_text[ui2_len_capacity - ui2_idx - 1] = pt_cnt_data->a_w2_text[pt_cnt_data->ui2_text_length - 1 - ui2_idx];
                    }
                    /*clean old data*/
                    x_memset(pt_cnt_data->a_w2_text,0,sizeof(UTF16_T)*(ui2_len_capacity - pt_cnt_data->ui2_text_length));
                    }

                }
                 /*update dirty flag*/
                pt_cnt_data->ui2_start_line = 0;
                _wgl_text_cnt_reset_dirty_flag(pt_cnt_data, TEXT_TOTAL_LINES_NOT_INIT);
            }
            #endif
            /*update text read direction*/
            pt_cnt_data->ui1_read_direction = ui1_text_dir;
            if(!(pt_cnt_data->ui4_extra_flag & TEXT_FIX_DIR_ALIGN))
            {
                pt_cnt_data->b_opposite_align = (pt_cnt_data->b_opposite_align) ? FALSE : TRUE;
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
static INT32 _wgl_text_cnt_get_text_dir(
    WGL_TEXT_CONTENT_T*                  pt_cnt_data,
    UINT8                               *pui1_text_dir)
{
    if (pui1_text_dir == NULL)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get text direction. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    *pui1_text_dir = pt_cnt_data->ui1_read_direction;
    return WGLR_OK;
}
static INT32 _wgl_text_cnt_get_render_cond(
    WGL_TEXT_CONTENT_T*                  pt_cnt_data,
    UINT16                              ui2_state,
    UINT8*                              pui1_render_cond)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    INT32                               i4_ret =WGLR_OK;
    GL_COLOR_T                          t_clr,t_text_clr,t_edge_clr;
    UINT8                               ui1_fg_render_cond;
    if (pui1_render_cond == NULL)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get render condition. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    _wgl_text_cnt_select_text_clr(pt_cnt_data, ui2_state, &t_text_clr,&t_edge_clr);
  
    if( pt_cnt_data->ui2_text_length == 0 ||
       wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, &t_text_clr))
    {
        ui1_fg_render_cond = WGL_RENDER_TRANS;            
    }else
    {
        ui1_fg_render_cond =  WGL_RENDER_TRANS_OPAQUE;
    }
    if(TEXT_NO_BK & pt_cnt_data->ui4_extra_flag)//no background
    {
        *pui1_render_cond = ui1_fg_render_cond;       
    }
    else if(!(TEXT_IMG_UI & pt_cnt_data->ui4_extra_flag)) /*color style*/
    {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_text_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT);
        }
        
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            t_clr = pt_cnt_data->t_clr_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            t_clr = pt_cnt_data->t_clr_bk.t_enable;
        }
        else
        {
            t_clr = pt_cnt_data->t_clr_bk.t_disable;
        }
       
       if(t_clr.a == 255)
        {
            *pui1_render_cond = WGL_RENDER_OPAQUE;
        }else if(t_clr.a == 0)
        {            
            *pui1_render_cond = ui1_fg_render_cond;          
        }
        else
        {
            *pui1_render_cond = WGL_RENDER_SEMI_TRANS;
        }
    }
    else /*image style*/
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_text_cnt_prepare_img(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT);
        }
        
          /*select image*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_cnt_data->t_img_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            h_image = pt_cnt_data->t_img_bk.t_enable;
        }
        else
        {
            h_image = pt_cnt_data->t_img_bk.t_disable;
        }
        
        if(NULL_HANDLE != h_image)
        {
             //todo need judge image's data if is opaque
            //*pui1_render_cond = WGL_RENDER_OPAQUE;
            *pui1_render_cond = WGL_RENDER_SEMI_TRANS;    
        }
        else
        {        
            *pui1_render_cond = ui1_fg_render_cond;          
        }
    }        
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_cnt_set_font_attr(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    WGL_TEXT_FONT_ATTR_TYPE_T           e_type,
    VOID*                               pv_param)
{
    INT32                               i4_ret;
    WGL_FONT_ATTR_T                     t_attr;
    
    switch(e_type)
    {
    case WGL_TEXT_FONT_ATTR_UNIFORM_WIDTH:
        {
            /*use system font if it isn't initialized.*/
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
            {
                /*use system font*/
                i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
                if(WGLR_OK != i4_ret)
                {
                    return i4_ret;
                }
            }

            /**/
            t_attr.t_uniform.ui2_width = WGL_POINTER_TO_UINT16(pv_param);
            i4_ret =  wgl_font_change_attr(&pt_cnt_data->t_text_font, &t_attr, WGL_FONT_ATTR_MASK_UNIFORM);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            return wgl_font_change_attr(&pt_cnt_data->t_text_focus_font, &t_attr, WGL_FONT_ATTR_MASK_UNIFORM);

        }
     case WGL_TEXT_FONT_ATTR_UNREC_CHAR:
        {
            /*check parameter*/
            if(NULL == pv_param)
            {
                return WGLR_INV_ARG;
            }

            /*use system font if it isn't initialized.*/
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
            {
                /*use system font*/
                i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
                if(WGLR_OK != i4_ret)
                {
                    return i4_ret;
                }
            }
            
            /**/
            t_attr.w2c_unrec_char = *((UTF16_T*)pv_param);
            return wgl_font_change_attr(&pt_cnt_data->t_text_font, &t_attr, WGL_FONT_ATTR_MAKS_UNREC_CHAR);
        }
    
    default:
        return WGLR_INV_ARG;
    }
}


/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_cnt_set_font_attr_ex(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_TEXT_FONT_ATTR_T*               pt_font_attr)
{
    INT32                               i4_ret;

    if(pt_font_attr == NULL ||
        (ui1_target != WGL_TEXT_FONT_ATTR_FOCUS_FONT && 
        ui1_target!= WGL_TEXT_FONT_ATTR_UNFOCUS_FONT)
       )
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_SET_FONT_ATTR_EX. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    switch(pt_font_attr->t_font_attr_type)
    {
    case WGL_TEXT_FONT_ATTR_UNIFORM_WIDTH:
        {
            WGL_FONT_ATTR_T     t_attr;

            /*use system font if it isn't initialized.*/
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
            {
                /*use system font*/
                i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
                if(WGLR_OK != i4_ret)
                {
                    return i4_ret;
                }
            }

            t_attr.t_uniform.ui2_width = WGL_POINTER_TO_UINT16(pt_font_attr->pv_attr_data);
            if(ui1_target == WGL_TEXT_FONT_ATTR_UNFOCUS_FONT)
            {
            return wgl_font_change_attr(&pt_cnt_data->t_text_font, &t_attr, WGL_FONT_ATTR_MASK_UNIFORM);
            }/*ui1_target == WGL_TEXT_FONT_ATTR_FOCUS_FONT*/
            else
            {
                return wgl_font_change_attr(&pt_cnt_data->t_text_focus_font, &t_attr, WGL_FONT_ATTR_MASK_UNIFORM);
            }
        }
    
    default:
        return WGLR_INV_ARG;
    }
}

static BOOL _is_skip_char(
    UTF16_T             w2_ch, 
    const UTF16_T*      w2s_skip, 
    UINT16              ui2_skiplen)
{
    UINT16      ui2_i;

    for (ui2_i = 0; ui2_i < ui2_skiplen; ui2_i++)
    {
        if (x_uc_w2s_get (w2s_skip, ui2_i) == w2_ch)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

static INT32 _wgl_text_cnt_cal_line_width(
	HFONT_T					    h_fnt,
	const UTF16_T*				w2s_str,	
	UINT32                     ui4_str_len,
	const UTF16_T*              w2s_skip,
	UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
	UINT32*						pui4_width)
{
		INT32						i4_ret;
		UTF16_T 					w2_ch_prv=(UTF16_T)0, w2_ch=(UTF16_T)0;
		INT32						i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
		BOOL						b_has_kern;
		INT32						i4_w_add_synth;
        UINT32                      ui4_i,ui4_len;
	   i4_ret = x_fe_get_add_width_by_synthesis (h_fnt, &i4_w_add_synth);
		   
	   if (i4_ret != FER_OK)
	   {
		   i4_w_add_synth = 0;
	   }
       
	   *pui4_width = 0;
	   ui4_len = ui4_str_len;
	   i4_ret = FER_OK;
	   b_has_kern = x_fe_has_kerning(h_fnt);
	   for(ui4_i=0; ui4_i<ui4_len; ui4_i++)
	   {
		   w2_ch = x_uc_w2s_get(w2s_str, ui4_i);
	
		   if(w2s_skip && _is_skip_char(w2_ch, w2s_skip, ui2_num_skip))
		   {
			   *pui4_width		   += (UINT32)ui2_w_skip;
               w2_ch_prv = 0;
		   }
		   else
		   {
			   if(b_has_kern)
			   {
				   i4_ret = x_fe_get_kerning( h_fnt,
											  (CHAR_CODE)w2_ch_prv,
											  (CHAR_CODE)w2_ch,
											  &i4_kern_x,
											  &i4_kern_y);
				   if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
				   {
					   *pui4_width += (UINT32)i4_kern_x;
				   }
			   }
	   
			   /* count advance */
			   i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
			   if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
			   {
				   *pui4_width += (UINT32)i4_adv_x;
			   }
			   
	           w2_ch_prv = w2_ch;
		   }
		
	   }
	
	   *pui4_width += (UINT32)i4_w_add_synth;
       return WGLR_OK;
}

static INT32 _wgl_text_cnt_cal_line_width_for_arab(
	HFONT_T					    h_fnt,
	const UTF16_T*				w2s_str,
	UINT32                      ui4_str_len,
	const UTF16_T*              w2s_skip,
	UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
	UINT32*						pui4_width)
{
		INT32						i4_ret;
		UTF16_T 					w2_ch_prv=(UTF16_T)0, w2_ch=(UTF16_T)0;
		INT32						i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
		BOOL						b_has_kern;
		INT32						i4_w_add_synth;
        UINT32                      ui4_i,ui4_len;

	   i4_ret = x_fe_get_add_width_by_synthesis (h_fnt, &i4_w_add_synth);
		   
	   if (i4_ret != FER_OK)
	   {
		   i4_w_add_synth = 0;
	   }
       
	   *pui4_width = 0;
	   ui4_len = ui4_str_len;
	   i4_ret = FER_OK;
	   b_has_kern = x_fe_has_kerning(h_fnt);
	   for(ui4_i=0; ui4_i<ui4_len; ui4_i++)
	   {
		   w2_ch = *(w2s_str - ui4_i);
	
		   if(w2s_skip && _is_skip_char(w2_ch, w2s_skip, ui2_num_skip))
		   {
			   *pui4_width		   += (UINT32)ui2_w_skip;
               w2_ch_prv = 0;
		   }
		   else
		   {
			   if(b_has_kern)
			   {
				   i4_ret = x_fe_get_kerning( h_fnt,
											  (CHAR_CODE)w2_ch,
											  (CHAR_CODE)w2_ch_prv,
											  &i4_kern_x,
											  &i4_kern_y);
				   if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
				   {
					   *pui4_width += (UINT32)i4_kern_x;
				   }
			   }
	   
			   /* count advance */
			   i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
			   if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
			   {
				   *pui4_width += (UINT32)i4_adv_x;
			   }
			   
	           w2_ch_prv = w2_ch;
		   }
		
	   }
	
	   *pui4_width += (UINT32)i4_w_add_synth;
       return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_CNT_TEXT_GET_PERFERRED_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_cnt_get_preferred_size(
    WGL_TEXT_CONTENT_T*       pt_cnt_data,
    GL_SIZE_T*                      pt_size)
{   
    /*calculate the foreground text size*/
	UTF16_T*							w2s_break_string = pt_cnt_data->pt_w2_text;
	UINT32								ui4_style = pt_cnt_data->ui4_extra_flag;
	WGL_INSET_T						    t_cnt_inset = pt_cnt_data->t_fg_inset;
	/*local varaible declaration*/
	UTF16_T 							a_w2s_skip_chars[] = WGL_SKIP_CHARS;
	UINT16                              ui2_skip_chars_num = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
	INT32								i4_ret;
	HFONT_T 							h_font = pt_cnt_data->t_text_font;
	INT16								i2_height;	
    UINT32                              ui4_line_width = 0;
	UINT16                              ui2_len_capacity = 0; 
    INT32                               i4_rendering_offset = 0 ;
    BOOL b_exist_arab;
    
	/*check parameter*/
	if (NULL == pt_size) 
	{
		 DBG_ERROR(("{WGL(TEXT_CNT)} ERR: null param. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__)); 	  
		 return WGLR_INV_ARG;
	} 
	
	pt_size->ui4_width = (UINT32)(t_cnt_inset.i4_left + t_cnt_inset.i4_right);
	pt_size->ui4_height = (UINT32)(t_cnt_inset.i4_top + t_cnt_inset.i4_bottom);
	if((NULL == w2s_break_string))
	{
		return WGLR_OK;
	}
    
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
	 /*get font according to widget state*/
    //todo,there is not state info in WGL_TEXT_CONTENT_T ?
    
     b_exist_arab = check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length);

    /*Style is single line mode */
	if (!(TEXT_MULTILINE & ui4_style))
	{
		i4_ret = x_fe_get_height(h_font, &i2_height);
		if (i4_ret != WGLR_OK)
		{
			DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get font height. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__)); 	  
			return i4_ret;
		}
		pt_size->ui4_height += (UINT32)i2_height;
        
        i4_ret = x_fe_get_rendering_offset(h_font,&i4_rendering_offset);
        if (i4_ret != WGLR_OK)
		{
			DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get font rendering offset. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__)); 	  
			return i4_ret;
		}
        if(i4_rendering_offset <0)
        {
		    pt_size->ui4_height += (UINT32)(-i4_rendering_offset);
        }
        else
        {
            pt_size->ui4_height += (UINT32)i4_rendering_offset;
        }
        
		if (!b_exist_arab)
		{
			_wgl_text_cnt_cal_line_width(
				                 h_font,
				                 w2s_break_string,
				                 (UINT32)pt_cnt_data->ui2_text_length,
				                 a_w2s_skip_chars,
				                 ui2_skip_chars_num,
				                 0,
				                 &ui4_line_width);
		}
		else
		{

            i4_ret = _wgl_text_cnt_update_arab_text(pt_cnt_data);
             if (WGLR_OK != i4_ret)
             {
                 return i4_ret;
             }   
            				 
            w2s_break_string = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity - 1]);
            _wgl_text_cnt_cal_line_width_for_arab(
            	                 h_font,
            	                 w2s_break_string,
            	                 (UINT32)pt_cnt_data->ui2_arab_dsp_text_len,
            	                 a_w2s_skip_chars,
            	                 ui2_skip_chars_num,
            	                 0,
            	                 &ui4_line_width);

		}
        
        pt_size->ui4_width += ui4_line_width;
	}
    return WGLR_OK;
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
static INT32 _wgl_text_cnt_get_min_size(
    const WGL_TEXT_CONTENT_T*       pt_cnt_data,
    GL_SIZE_T*                      pt_size)
{   
    /*inset size*/
    pt_size->ui4_width = (UINT32)(pt_cnt_data->t_fg_inset.i4_left + pt_cnt_data->t_fg_inset.i4_right);
    pt_size->ui4_height = (UINT32)(pt_cnt_data->t_fg_inset.i4_top + pt_cnt_data->t_fg_inset.i4_bottom);

    /*calculate the foreground text size*/

    return WGLR_OK;
}

static INT32 _wgl_text_cnt_get_max_line_width(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state,
    UINT16                               *pui2_text_max_line_width)
{

     /*local variable declaration*/
    INT32                               i4_ret;
    UINT16                              ui2_lines;
    UINT16                              ui2_cur_line;
    WGL_TEXT_LINE_INFO*                 pt_cur_line_info;
    UINT16                              ui2_line_width = 0;
    /*check parameter*/
    if (NULL == pui2_text_max_line_width) 
    {
         DBG_ERROR(("{WGL(TEXT_CNT)} ERR: null param. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
         return WGLR_INV_ARG;
    } 
    
    /*check if line infos are out of date*/
    if(pt_cnt_data->b_is_data_dirty)
    {
        if(0 != pt_cnt_data->ui2_text_length)
        {
            i4_ret = _wgl_text_cnt_layout_v_top(pt_cnt_data,ui2_state);

            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        
        pt_cnt_data->b_is_data_dirty = FALSE;
    }           
    
    ui2_lines = pt_cnt_data->ui2_lines_in_display;
  
    pt_cur_line_info = pt_cnt_data->pt_line_infos;
   
    /*paint lines*/
    for(ui2_cur_line = 0; ui2_cur_line < ui2_lines; ui2_cur_line++)
    {
        if(pt_cur_line_info->ui2_width > ui2_line_width)
        {
            ui2_line_width = pt_cur_line_info->ui2_width;
        }
        pt_cur_line_info++;
        
    }
    *pui2_text_max_line_width = ui2_line_width;
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
static INT32 _wgl_text_cnt_prepare_img(
    WGL_TEXT_CONTENT_T*             pt_cnt_data)
{
    /*local variable declaration*/
    WGL_IMG_ST_T                    e_segment_type;
    UINT32                          ui4_width;
    UINT32                          ui4_height;
    WGL_HIMG_TPL_T                  h_tpl_img;
    INT32                           i4_ret;

    /*get system enable image for text*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_enable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_enable = h_tpl_img;
    }

    /*get system disable image for text*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_disable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC_DIS, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC_DIS image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_disable = h_tpl_img;
    }

    /*get system highlight image for text*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_highlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC_HT, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC_HT image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_highlight = h_tpl_img;
    }
    
    /*remove the flag for bk image*/
    WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_IMG_NOT_INIT);
    
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
static INT32 _wgl_text_cnt_use_sys_bk_color(
    WGL_TEXT_CONTENT_T*             pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                           i4_ret;

    /*get system enable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_STATIC color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_STATIC_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_highlight);

    /*remove the flag for bk image*/
    if(WGLR_OK == i4_ret)
    {
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_BK_CLR_NOT_INIT);
    }
    else
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_STATIC_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_text_cnt_use_sys_text_color(
    WGL_TEXT_CONTENT_T*             pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                           i4_ret;

    /*get system enable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_highlight);

    if(WGLR_OK == i4_ret)
    {
        /*remove the flag for text color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TEXT_CLR_NOT_INIT);
    }
    else
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_text_cnt_use_sys_edge_color(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                           i4_ret;
    
    /*get system enable color for edge*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_edge.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_edge.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system highlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_edge.t_highlight);
    
    if(WGLR_OK == i4_ret)
    {
        /*remove the flag for text color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_EDGE_CLR_NOT_INIT);
    }
    else
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_text_cnt_use_sys_font(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data)
{
    WGL_FONT_INFO_T                     t_font_info;
    INT32                               i4_ret;

    /*get system font*/
    i4_ret = x_wgl_get_system_font(&t_font_info);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    return _wgl_text_cnt_set_font(pt_cnt_data, &t_font_info);
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
static INT32 _wgl_text_cnt_layout_v_top_ex(
    WGL_TEXT_CONTENT_T*                         pt_cnt_data,
    UINT16                                      ui2_state,
    BOOL                                        b_smart_cut)
{
    /*local varaible declaration*/
    UTF16_T*                            a_w2s_break_chars = pt_cnt_data->as_break_chars;/*WGL_CAN_BREAK_CHARS*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UTF16_T                             w2_last_char;
    UINT16                              ui2_first_token_pos;
    INT32                               i4_ret;
    UINT16                              ui2_index;
    WGL_TEXT_LINE_INFO*                 pt_line_info;
    GL_RECT_T                           t_available_rect;
    BOOL                                b_start_record = FALSE;
//    UINT16                              ui2_text_max_len;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
	BOOL 								b_exist_arab;
	WGL_TEXT_MULTI_STRING*              pt_string_element = NULL; 
    UINT16                              ui2_string_idx= 0;
    WGL_TEXT_MULTI_STRING_A             *pt_string_point = NULL;
    UINT16                              ui2_highlight_lines = 0;
    INT16                              i2_highlight_display_postion = 0;
    UINT16                              ui2_prev_highlight_lines = 0;
    UINT16                              ui2_i;
        
    pt_string_element = pt_cnt_data->pt_multi_string;

    if (pt_string_element == NULL)
    {
        return WGLR_INTERNAL_ERROR;
    }

    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);
    

    if (!b_smart_cut)
    {
        ui2_index = 0;
        x_memset(pt_cnt_data->pt_line_infos, 0, pt_cnt_data->ui2_line_capacity * sizeof(WGL_TEXT_LINE_INFO));
        pt_line_info = pt_cnt_data->pt_line_infos;
        pt_cnt_data->ui2_lines_in_display = pt_cnt_data->ui2_line_capacity;
        while(pt_string_element != NULL && ui2_index < pt_cnt_data->ui2_line_capacity)
        {
            if (pt_string_element->ps_string == NULL)
            {
                pt_line_info->ui2_str_idx = ui2_index;
                pt_line_info++;
                ui2_index ++;
            }
            else
            {
                pt_line_info->w2s_string = pt_string_element->ps_string;
                pt_line_info->ui2_width = WGL_RECT_GET_WIDTH(&t_available_rect);;
                pt_line_info->ui2_str_len   = pt_string_element->ui2_size;
                pt_line_info->ui2_str_idx = ui2_index;
                pt_line_info++;
                ui2_index ++;
            }
            
            pt_string_element = pt_string_element->pt_next;
        }

        return WGLR_OK;
    }
    pt_string_point = (WGL_TEXT_MULTI_STRING_A *)WGL_WIDGET_MEM_ALLOC(pt_cnt_data,pt_cnt_data->ui2_multi_string_num * sizeof(WGL_TEXT_MULTI_STRING_A));
    if (pt_string_point == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_string_point, 0, pt_cnt_data->ui2_multi_string_num * sizeof(WGL_TEXT_MULTI_STRING_A));
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,pt_cnt_data->ui2_multi_string_num * sizeof(WGL_TEXT_MULTI_STRING_A),pt_string_point);
        return i4_ret;
    }
 //   ui2_text_max_len = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);

    while(pt_string_element != NULL)
    {
        if (pt_string_element->ui2_lines == 0)
        {
            i4_ret = _wgl_text_cnt_calc_string_lines(pt_cnt_data, 
                                                 ui2_state, 
                                                 pt_string_element->ps_string,
                                                 pt_string_element->ui2_size,
                                                 pt_string_element->ps_arab_dsp_text,
                                                 &pt_string_element->ui2_arab_dsp_text_len,
                                                 &ui2_highlight_lines,
                                                 &t_available_rect);
            if (i4_ret != WGLR_OK || ui2_highlight_lines == 0)
            {
                WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,pt_cnt_data->ui2_multi_string_num * sizeof(WGL_TEXT_MULTI_STRING_A),pt_string_point);
                return i4_ret;
            }
            pt_string_element->ui2_lines = ui2_highlight_lines;
        }
        pt_string_point[ui2_string_idx].pt_string_element = pt_string_element;
        pt_string_point[ui2_string_idx].ui2_lines = pt_string_element->ui2_lines;
        
        ui2_string_idx ++;
        
        pt_string_element = pt_string_element->pt_next;
    }
    
    /*calc highlight string lines*/
    ui2_highlight_lines = pt_string_point[pt_cnt_data->ui2_highlight_idx].ui2_lines;
    
    if (pt_cnt_data->ui2_interlace != 0)
    {
        ui2_highlight_lines = ui2_highlight_lines * (1 + pt_cnt_data->ui2_interlace);
    }
    
    if (pt_cnt_data->ui4_extra_flag & TEXT_STRING_POSITION_BY_IDX)
    {
        UINT16 ui2_muti_line_idx = 0;
        i2_highlight_display_postion = 0;
        for(;ui2_muti_line_idx<pt_cnt_data->ui2_highlight_idx;ui2_muti_line_idx++)
        {
            i2_highlight_display_postion += pt_string_point[ui2_muti_line_idx].ui2_lines;
        }
        //i2_highlight_display_postion = ui2_highlight_lines * pt_cnt_data->ui2_highlight_idx;
    }
    else
    {
        i2_highlight_display_postion = (INT16)pt_cnt_data->ui2_highlight_postion;
    }
    
    if (ui2_highlight_lines + pt_cnt_data->ui2_highlight_postion > pt_cnt_data->ui2_line_capacity)
    {
        i2_highlight_display_postion = pt_cnt_data->ui2_line_capacity - ui2_highlight_lines;
    }
    
    if (i2_highlight_display_postion < 0)
    {
        i2_highlight_display_postion = 0;
    }

    ui2_prev_highlight_lines = (UINT16)i2_highlight_display_postion;
    
    pt_string_element = pt_string_point[pt_cnt_data->ui2_highlight_idx].pt_string_element;

    x_memset(pt_cnt_data->pt_line_infos, 0, pt_cnt_data->ui2_line_capacity * sizeof(WGL_TEXT_LINE_INFO));

    pt_line_info = &pt_cnt_data->pt_line_infos[i2_highlight_display_postion];
    ui2_string_idx = pt_cnt_data->ui2_highlight_idx;
    /*initialize variable*/
    t_break_line_Info.h_font = t_text_font;
    t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    
    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;
    ui2_index = (UINT16)i2_highlight_display_postion;
    
    /*fill line info after highlight postion*/
    while(pt_string_element != NULL && ui2_index < pt_cnt_data->ui2_line_capacity)
    {
        if (pt_string_element->ps_string == NULL)
        {
            pt_line_info->w2s_string = t_break_line_Info.w2s_string;
            pt_line_info->ui2_width = 0;
            pt_line_info->ui2_str_len = 0;
            pt_line_info->ui2_str_idx = ui2_string_idx;
            pt_line_info++;
            ui2_index ++;
            
            if (pt_cnt_data->ui2_interlace != 0 )
            {
                for (ui2_i = 0; ui2_i < pt_cnt_data->ui2_interlace && ui2_index < pt_cnt_data->ui2_line_capacity; ui2_i ++)
                {
                    pt_line_info->w2s_string = NULL;
                    pt_line_info->ui2_width = 0;
                    pt_line_info->ui2_str_len   = 0;
                    pt_line_info->ui2_str_idx = ui2_string_idx;
                    pt_line_info++;
                    ui2_index ++;
                }
            }
        }
        else
        {
            b_exist_arab = check_exist_arabic(pt_string_element->ps_string,pt_string_element->ui2_size);
            if (!b_exist_arab)
            {           
               t_break_line_Info.w2s_string = pt_string_element->ps_string;  
        	   t_break_line_Info.i4_string_length =(INT32) (pt_string_element->ui2_size);
            } 
            else
            {         
           		if (pt_string_element->ps_arab_dsp_text == NULL)
           		{

					pt_string_element->ps_arab_dsp_text = (UTF16_T *)WGL_MEM_ALLOC((pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
					if (pt_string_element->ps_arab_dsp_text == NULL)
					{
						WGL_MEM_FREE(pt_string_point);
						return WGLR_OUT_OF_MEMORY;
					}
					x_memset(pt_string_element->ps_arab_dsp_text, 0, (pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
					
					i4_ret = _wgl_text_cnt_update_arab_text_ex(pt_cnt_data, pt_string_element->ps_string, pt_string_element->ps_arab_dsp_text);
					if (WGLR_OK != i4_ret)
					 {
					     WGL_MEM_FREE(pt_string_point);
					     return i4_ret;
					}
					/*reset reorder attr after used for current string content*/
					pt_cnt_data->t_arab_reorder_attr.b_find_init_level = TRUE;
					pt_cnt_data->t_arab_reorder_attr.ui1_embedding_level = 0;

					pt_string_element->ui2_arab_dsp_text_len = pt_cnt_data->ui2_arab_dsp_text_len;
					if (pt_string_element->ui2_arab_dsp_text_len > pt_string_element->ui2_size)
					{
						x_dbg_stmt("[Text]memory error.\n");
					}
           		}
                 
                t_break_line_Info.w2s_string = pt_string_element->ps_arab_dsp_text;
                t_break_line_Info.i4_string_length = (INT32)pt_string_element->ui2_arab_dsp_text_len;                    
           	}
			
			t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
			t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;
			t_break_line_result.ui2_char_number = 0;   
			ui2_first_token_pos = 0;   

			b_exist_arab = FALSE;

            /*get the necessary line info*/
            pt_cnt_data->b_more_data = TRUE;

            for (; ui2_index < pt_cnt_data->ui2_line_capacity; )
            {
                if(!b_start_record)
                {
                    b_start_record = TRUE;
                }

                /*update parameter*/
                ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                                      (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                                      0;
                t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
                t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;        

                /*Because if smart-cut is enabled, we want to print as many characters as possible.
                     So, we ignore can-break character if smart-cut is turned on.*/
                if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT) && 
                   (ui2_index + (UINT16)1) == pt_cnt_data->ui2_line_capacity)
                {
                    t_break_line_Info.w2s_can_break = NULL;
                }

                /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
               if (!b_exist_arab)
#else
               if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
                if(i4_ret != WGLR_OK)
                {
                    DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                    WGL_MEM_FREE(pt_string_point);
                    return i4_ret;
                }

                if(b_start_record)
                {
                    /*keep the start and necessary width*/
#ifdef ARAB_EXIST_SUPPORT
                    if (!b_exist_arab)
#else
                    if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
                    {           
                        pt_line_info->w2s_string = t_break_line_Info.w2s_string;
                    }  
                    else
                    {            
                        pt_line_info->w2s_string = (UTF16_T*)(t_break_line_Info.w2s_string - t_break_line_result.ui2_char_number + 1);                
                    }
                    
                    pt_line_info->ui2_width = t_break_line_result.ui2_width;
                    pt_line_info->ui2_str_idx = ui2_string_idx;
                    if(t_break_line_result.ui2_char_number != 0)
                    {
                        
#ifdef ARAB_EXIST_SUPPORT
                        if (!b_exist_arab)
#else
                        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
                        {           
                            w2_last_char = t_break_line_Info.w2s_string[t_break_line_result.ui2_char_number - 1];
                        } 
                        else
                        {            
                            w2_last_char = *(pt_line_info->w2s_string);
                        }
                        if((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag)
                            || NULL == x_uc_w2s_chr(a_w2s_force_chars, w2_last_char)) /*the last character isn't '\n'*/
                        {
                            pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number;
                        }
                        else /*the last character is '\n'*/
                        {
                            /*don't count the '\n' to display*/
                            pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number - (UINT16)1;
#ifdef ARAB_EXIST_SUPPORT
                           if (b_exist_arab)
#else
                           if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_RIGHT_TO_LEFT)
#endif
                           {            
                               pt_line_info->w2s_string = pt_line_info->w2s_string + 1;
                           } 
                        }
                    }
                    else
                    {
                        pt_line_info->ui2_str_len = 0;
                    }

                    /*the start for next scan*/
                    pt_line_info++;
                    ui2_index++;

                    if (pt_cnt_data->ui2_interlace != 0)
                    {
                        for (ui2_i = 0; ui2_i < pt_cnt_data->ui2_interlace && ui2_index < pt_cnt_data->ui2_line_capacity; ui2_i ++)
                        {
                            pt_line_info->w2s_string = NULL;
                            pt_line_info->ui2_width = 0;
                            pt_line_info->ui2_str_len   = 0;
                            pt_line_info->ui2_str_idx = ui2_string_idx;
                            pt_line_info++;
                            ui2_index ++;
                        }
                    }
                } /*if(b_start_record)*/

                /*check if the current position is the end*/
                if(NULL == t_break_line_result.w2s_next_start|| (UTF16_T)0 == *t_break_line_result.w2s_next_start )
                {
                    /*turn off the more-data flag*/
                    pt_cnt_data->b_more_data = FALSE;
                    break;
                }
            }
        }
        
        pt_string_element = pt_string_element->pt_next;
        ui2_string_idx ++;
    }
    
    pt_cnt_data->ui2_lines_in_display = ui2_index;
    
    if (pt_cnt_data->ui2_highlight_idx == 0)
    {
        pt_cnt_data->ui2_start_line = i2_highlight_display_postion;
    }
    else
    {
        pt_cnt_data->ui2_start_line = 0;
    }
    /*fill line info before highlight postion*/
    if (ui2_prev_highlight_lines > 0 && pt_cnt_data->ui2_highlight_idx > 0)
    {
        INT16  i2_prev_lines = ui2_prev_highlight_lines;
        INT16  i2_prev_string_idx = pt_cnt_data->ui2_highlight_idx - 1;
        
        for (;i2_prev_string_idx >= 0 && i2_prev_lines > 0; i2_prev_string_idx --)
        {
            i2_prev_lines -= pt_string_point[i2_prev_string_idx].ui2_lines * (1 + pt_cnt_data->ui2_interlace);
        }
        if (i2_prev_lines > 0)
        {
            pt_cnt_data->ui2_start_line = (UINT16)i2_prev_lines;
        }
        else
        {
            pt_cnt_data->ui2_start_line = 0;
        }
        
        if (i2_prev_string_idx < 0)
        {
            i2_prev_string_idx = 0;
            pt_string_element = pt_string_point[0].pt_string_element;
        }
        else
        {
            pt_string_element = pt_string_point[i2_prev_string_idx].pt_string_element;
        }
        
        b_start_record = FALSE;
        
        pt_line_info = pt_cnt_data->pt_line_infos;
        while (pt_string_element != NULL && i2_prev_lines < ui2_prev_highlight_lines)
        {
            if (pt_string_element->ps_string == NULL)
            {
                if (i2_prev_lines >= 0)
                {
                    pt_line_info->w2s_string = NULL;
                    pt_line_info->ui2_width = 0;
                    pt_line_info->ui2_str_len   = 0;
                    pt_line_info->ui2_str_idx = (UINT16)i2_prev_string_idx;
                    pt_line_info++;
                }
                i2_prev_lines ++;
                if (pt_cnt_data->ui2_interlace != 0)
                {
                    for (ui2_i = 0; ui2_i < pt_cnt_data->ui2_interlace; ui2_i ++)
                    {
                        if (i2_prev_lines >= 0)
                        {
                            pt_line_info->w2s_string = NULL;
                            pt_line_info->ui2_width = 0;
                            pt_line_info->ui2_str_len   = 0;
                            pt_line_info->ui2_str_idx = (UINT16)i2_prev_string_idx;
                            pt_line_info++;
                        }
                        i2_prev_lines ++;
                    }
                }
            }
            else
            {
                b_exist_arab = check_exist_arabic(pt_string_element->ps_string,pt_string_element->ui2_size);
                if (!b_exist_arab)
                {           
                   t_break_line_Info.w2s_string = pt_string_element->ps_string;  
            	   t_break_line_Info.i4_string_length =(INT32) (pt_string_element->ui2_size);
                } 
                else
                {            
            		
                     if (pt_string_element->ps_arab_dsp_text == NULL)
	           		{

						pt_string_element->ps_arab_dsp_text = (UTF16_T *)WGL_MEM_ALLOC((pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
						if (pt_string_element->ps_arab_dsp_text == NULL)
						{
							WGL_MEM_FREE(pt_string_point);
							return WGLR_OUT_OF_MEMORY;
						}
						x_memset(pt_string_element->ps_arab_dsp_text, 0, (pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
						
						i4_ret = _wgl_text_cnt_update_arab_text_ex(pt_cnt_data, pt_string_element->ps_string, pt_string_element->ps_arab_dsp_text);
						if (WGLR_OK != i4_ret)
						 {
						     WGL_MEM_FREE(pt_string_point);
						     return i4_ret;
						}
						/*reset reorder attr after used for current string content*/
						pt_cnt_data->t_arab_reorder_attr.b_find_init_level = TRUE;
						pt_cnt_data->t_arab_reorder_attr.ui1_embedding_level = 0;

						pt_string_element->ui2_arab_dsp_text_len = pt_cnt_data->ui2_arab_dsp_text_len;
						if (pt_string_element->ui2_arab_dsp_text_len > pt_string_element->ui2_size)
						{
							x_dbg_stmt("[Text]memory error.\n");
						}
	           		}
                     
                     t_break_line_Info.w2s_string = pt_string_element->ps_arab_dsp_text;
                     t_break_line_Info.i4_string_length = (INT32)pt_string_element->ui2_arab_dsp_text_len;                    

               }
               
                t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
                t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;
                t_break_line_result.ui2_char_number = 0;        
                ui2_first_token_pos = 0;   

                /*get the necessary line info*/
                pt_cnt_data->b_more_data = TRUE;

                for (; i2_prev_lines < (INT16)(pt_cnt_data->ui2_line_capacity) && i2_prev_lines < (INT16)ui2_prev_highlight_lines; )
                {
                    if(!b_start_record && i2_prev_lines >= 0)
                    {
                        b_start_record = TRUE;
                    }

                    /*update parameter*/
                    ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                                          (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                                          0;
                    t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
                    t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;        

                    /*Because if smart-cut is enabled, we want to print as many characters as possible.
                         So, we ignore can-break character if smart-cut is turned on.*/
                    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT) && 
                       (i2_prev_lines + (INT16)1) == (INT16)pt_cnt_data->ui2_line_capacity)
                    {
                        t_break_line_Info.w2s_can_break = NULL;
                    }

                    /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
                   if (!b_exist_arab)
#else
                   if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
                    if(i4_ret != WGLR_OK)
                    {
                        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                        WGL_MEM_FREE(pt_string_point);
                        return i4_ret;
                    }

                    if(b_start_record)
                    {
                        /*keep the start and necessary width*/
#ifdef ARAB_EXIST_SUPPORT
                        if (!b_exist_arab)
#else
                        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
                        {           
                            pt_line_info->w2s_string = t_break_line_Info.w2s_string;
                        }  
                        else
                        {  
                            if ((t_break_line_Info.w2s_string - t_break_line_result.ui2_char_number) > 0)
                        	{
                                pt_line_info->w2s_string = (UTF16_T*)(t_break_line_Info.w2s_string - t_break_line_result.ui2_char_number + 1);
                        	}	
                        }
                        
                        pt_line_info->ui2_width = t_break_line_result.ui2_width;
                        pt_line_info->ui2_str_idx = (UINT16)i2_prev_string_idx;
                        if(t_break_line_result.ui2_char_number != 0)
                        {
                            
#ifdef ARAB_EXIST_SUPPORT
                            if (!b_exist_arab)
#else
                            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
                            {           
                                w2_last_char = t_break_line_Info.w2s_string[t_break_line_result.ui2_char_number - 1];
                            } 
                            else
                            {            
                                w2_last_char = *(pt_line_info->w2s_string);
                            }
                            if((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag)
                                || NULL == x_uc_w2s_chr(a_w2s_force_chars, w2_last_char)) /*the last character isn't '\n'*/
                            {
                                pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number;
                            }
                            else /*the last character is '\n'*/
                            {
                                /*don't count the '\n' to display*/
                                pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number - (UINT16)1;
#ifdef ARAB_EXIST_SUPPORT
                               if (b_exist_arab)
#else
                               if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_RIGHT_TO_LEFT)
#endif
                               {           
                                   pt_line_info->w2s_string = pt_line_info->w2s_string + 1;
                                } 
                            }
                        }
                        else
                        {
                            pt_line_info->ui2_str_len = 0;
                        }

                        /*the start for next scan*/
                        pt_line_info++;
                        
                        i2_prev_lines++;

                        if (pt_cnt_data->ui2_interlace != 0)
                        {
                            for (ui2_i = 0; ui2_i < pt_cnt_data->ui2_interlace; ui2_i ++)
                            {
                                if (i2_prev_lines >= 0)
                                {
                                    pt_line_info->w2s_string = NULL;
                                    pt_line_info->ui2_width = 0;
                                    pt_line_info->ui2_str_len   = 0;
                                    pt_line_info->ui2_str_idx = (UINT16)i2_prev_string_idx;
                                    pt_line_info++;
                                }
                                i2_prev_lines ++;
                            }
                        }
                    } /*if(b_start_record)*/

                    /*check if the current position is the end*/
                    if(NULL == t_break_line_result.w2s_next_start|| (UTF16_T)0 == *t_break_line_result.w2s_next_start )
                    {
                        /*turn off the more-data flag*/
                        pt_cnt_data->b_more_data = FALSE;
                        break;
                    }
                }
            }
            pt_string_element = pt_string_element->pt_next;
            i2_prev_string_idx ++;
        }
        
    }
    WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,pt_cnt_data->ui2_multi_string_num * sizeof(WGL_TEXT_MULTI_STRING_A),pt_string_point);
    return WGLR_OK;
}


static INT32 _wgl_text_cnt_calc_string_lines(
    WGL_TEXT_CONTENT_T*                         pt_cnt_data,
    UINT16                                      ui2_state,
    UTF16_T*                                    ps_string,
    UINT16                                      ui2_string_size,
    UTF16_T*									ps_arab_dsp_text,
    UINT16*										pui2_arab_dsp_text_len,
    UINT16*                                     pui2_total_lines,
    const GL_RECT_T*                            pt_available_rect)
{
    /*local varaible declaration*/
    UTF16_T*                            a_w2s_break_chars = pt_cnt_data->as_break_chars;/*WGL_CAN_BREAK_CHARS*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UINT16                              ui2_first_token_pos;
//    UINT16                              ui2_len_capacity;
    UINT16                              ui2_line_index = 0;
    INT32                               i4_ret;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
#ifdef ARAB_EXIST_SUPPORT
	BOOL								b_exist_arab;
#endif    
    if(NULL == pui2_total_lines)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if (ps_string == NULL || *ps_string == 0x0000)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        *pui2_total_lines = 1;
        return WGLR_OK;
    }
    *pui2_total_lines = 0;

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check cache*/
    if(!WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TOTAL_LINES_NOT_INIT))
    {
        *pui2_total_lines = pt_cnt_data->ui2_total_lines;
        return WGLR_OK;
    }
    else
    {
        /*reset ui2_total_lines*/
        pt_cnt_data->ui2_total_lines = 0;
    }    
     /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    /*initialize variable*/
    t_break_line_Info.h_font = t_text_font;
    t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(pt_available_rect); 
    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;
    
 //   ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
    

#ifdef ARAB_EXIST_SUPPORT
    b_exist_arab = check_exist_arabic(ps_string, ui2_string_size);
#endif	
    t_break_line_Info.i4_string_length = (INT32)ui2_string_size;
    
#ifdef ARAB_EXIST_SUPPORT
     if (!b_exist_arab)
#else
     if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif 
    {
        t_break_line_Info.w2s_string = ps_string;
        
    }
    else
    {
    	if (ps_arab_dsp_text == NULL)
    	{
    		ps_arab_dsp_text = (UTF16_T *)WGL_MEM_ALLOC((ui2_string_size + 1) * sizeof(UTF16_T));
			if (ps_arab_dsp_text == NULL)
			{
				return WGLR_OUT_OF_MEMORY;
			}
			x_memset(ps_arab_dsp_text, 0, (ui2_string_size + 1) * sizeof(UTF16_T));
			i4_ret = _wgl_text_cnt_update_arab_text_ex(pt_cnt_data, ps_string, ps_arab_dsp_text);
			if (WGLR_OK != i4_ret)
			{
				 x_mem_free(ps_arab_dsp_text);
			     return i4_ret;
			}

			*pui2_arab_dsp_text_len = pt_cnt_data->ui2_arab_dsp_text_len;
			if (*pui2_arab_dsp_text_len > ui2_string_size)
			{
				x_dbg_stmt("[Text]memory error.\n");
			}
    	}
        
        t_break_line_Info.w2s_string = ps_arab_dsp_text;
        t_break_line_Info.i4_string_length = (INT32)*pui2_arab_dsp_text_len;                    
    }
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    ui2_first_token_pos = 0;
    b_exist_arab = FALSE;
    
    /*get the necessary line info*/
    while(t_break_line_result.w2s_next_start != NULL && t_break_line_result.w2s_next_start[0] != 0)
    {
        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                              (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                              0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;
        
        /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
        if (!b_exist_arab)
#else
        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
        if(i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        ui2_line_index++;
    }

    *pui2_total_lines = ui2_line_index;
	x_mem_free(ps_arab_dsp_text);
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
static INT32 _wgl_text_cnt_layout_v_top(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT16                                      ui2_state)
{
    /*local varaible declaration*/
    UTF16_T*                            a_w2s_break_chars = pt_cnt_data->as_break_chars;/*WGL_CAN_BREAK_CHARS*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UTF16_T                             w2_last_char;
    UINT16                              ui2_first_token_pos, ui2_cur_line;
    INT32                               i4_ret;
    UINT16                              ui2_index;
    WGL_TEXT_LINE_INFO*                 pt_line_info;
    GL_RECT_T                           t_available_rect;
    BOOL                                b_start_record = FALSE;
    UINT16                              ui2_text_max_len;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
	BOOL 								b_exist_arab;
	

    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    ui2_text_max_len = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);
    
    /*initialize variable*/
    t_break_line_Info.h_font = t_text_font;
    t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    

	b_exist_arab = check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length);
    if (!b_exist_arab)
    {           
       t_break_line_Info.w2s_string = pt_cnt_data->pt_w2_text;  
	   t_break_line_Info.i4_string_length =(INT32) (pt_cnt_data->ui2_text_length);
    } 
    else
    {            
		
         i4_ret = _wgl_text_cnt_update_arab_text(pt_cnt_data);
		 if (WGLR_OK != i4_ret)
  	     {
  	         return i4_ret;
         }
         /*reset reorder attr after used for current string content*/
         pt_cnt_data->t_arab_reorder_attr.b_find_init_level = TRUE;
         pt_cnt_data->t_arab_reorder_attr.ui1_embedding_level = 0;
         
         t_break_line_Info.w2s_string = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_text_max_len - 1]);
         t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_arab_dsp_text_len;                    

   }
   
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;

    pt_line_info = pt_cnt_data->pt_line_infos;
    ui2_first_token_pos = 0;   

    /*get the necessary line info*/
    pt_cnt_data->b_more_data = TRUE;
    for (ui2_index = 0, ui2_cur_line = 0; ui2_index < pt_cnt_data->ui2_line_capacity; ui2_cur_line++)
    {
        if((!b_start_record) && (ui2_cur_line >= pt_cnt_data->ui2_start_line))
        {
            b_start_record = TRUE;
        }

        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                              (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                              0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;        

        /*Because if smart-cut is enabled, we want to print as many characters as possible.
             So, we ignore can-break character if smart-cut is turned on.*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_SMART_CUT) && 
           (ui2_index + (UINT16)1) == pt_cnt_data->ui2_line_capacity)
        {
            t_break_line_Info.w2s_can_break = NULL;
        }

        /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
       if (!b_exist_arab)
#else
       if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
        if(i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        if(b_start_record)
        {
            /*keep the start and necessary width*/
  #ifdef ARAB_EXIST_SUPPORT
            if (!b_exist_arab)
#else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
            {           
                pt_line_info->w2s_string = t_break_line_Info.w2s_string;
            }  
            else
            {            
                pt_line_info->w2s_string = (UTF16_T*)(t_break_line_Info.w2s_string - t_break_line_result.ui2_char_number + 1);                
            }
            
            pt_line_info->ui2_width = t_break_line_result.ui2_width;
            if(t_break_line_result.ui2_char_number != 0)
            {
                
 #ifdef ARAB_EXIST_SUPPORT
                if (!b_exist_arab)
#else
                if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
                {           
                    w2_last_char = t_break_line_Info.w2s_string[t_break_line_result.ui2_char_number - 1];
                } 
                else
                {            
                    w2_last_char = *(pt_line_info->w2s_string);
                }
                if((TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag)
                    || NULL == x_uc_w2s_chr(a_w2s_force_chars, w2_last_char)) /*the last character isn't '\n'*/
                {
                    pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number;
                }
                else /*the last character is '\n'*/
                {
                    /*don't count the '\n' to display*/
                    pt_line_info->ui2_str_len = t_break_line_result.ui2_char_number - (UINT16)1;
#ifdef ARAB_EXIST_SUPPORT
                   if (b_exist_arab)
#else
                   if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_RIGHT_TO_LEFT)
#endif
                   {           
                       pt_line_info->w2s_string = pt_line_info->w2s_string + 1;
                    } 
                }
            }
            else
            {
                pt_line_info->ui2_str_len = 0;
            }

            /*the start for next scan*/
            pt_line_info++;
            ui2_index++;
        } /*if(b_start_record)*/

        /*check if the current position is the end*/
        if(NULL == t_break_line_result.w2s_next_start || (UTF16_T)0 == *t_break_line_result.w2s_next_start )
        {
            /*turn off the more-data flag*/
            pt_cnt_data->b_more_data = FALSE;
            break;
        }
    }

    pt_cnt_data->ui2_lines_in_display = ui2_index;
  
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
#if 0
static INT32 _wgl_text_cnt_layout_v_center(
    WGL_TEXT_CONTENT_T* pt_cnt_data)
{
    /*local varaible declaration*/
    UTF16_T                             a_w2s_break_chars[] = WGL_CAN_BREAK_CHARS;
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_TEXT_LINE_INFO      a_line_infos[TEXT_MAX_TOTAL_LINE] = {0}; /*index: 0 ~ 254*/
    WGL_BREAK_LINE_INFO_T   t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T t_break_line_result = {0};
    UINT16                  ui2_first_token_pos;
    INT32                   i4_ret;
    UINT16                  ui2_cur_line = 0, ui2_copy_start;
    GL_RECT_T               t_available_rect;
    

    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, pt_cnt_data->t_fg_inset.i4_left, pt_cnt_data->t_fg_inset.i4_top,
                        pt_cnt_data->t_fg_inset.i4_right, pt_cnt_data->t_fg_inset.i4_bottom);
    
    /*initialize variable*/
    t_break_line_Info.h_font = pt_cnt_data->t_text_font;
    t_break_line_Info.w2c_force_token = a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;
    t_break_line_Info.w2s_string = pt_cnt_data->a_w2_text;
    t_break_line_Info.i4_string_length = pt_cnt_data->ui2_text_length;
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1;
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;

    ui2_first_token_pos = 0;


    /*get the necessary line info*/
    while(NULL != t_break_line_result.w2s_next_start)
    {
        /*keep the next start*/
        a_line_infos[ui2_cur_line].w2s_string = t_break_line_result.w2s_next_start;
        
        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                                    (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;
        
        /*Get text's necessary width*/
        i4_ret = wgl_get_break_line_pos(&t_break_line_Info, &ui2_first_token_pos, &t_break_line_result);
        if(i4_ret != WGLR_OK)
        {
            return i4_ret;
        }
        
        /*keep the necessary width*/
        a_line_infos[ui2_cur_line].ui2_width = t_break_line_result.ui2_width;
        a_line_infos[ui2_cur_line].ui2_str_len = t_break_line_result.ui2_char_number;

        /*update index for line info*/
        ui2_cur_line++;
        if(ui2_cur_line >= TEXT_MAX_TOTAL_LINE)
        {
            return WGLR_OVER_SUPPORT_LIMIT;
        }
    }

    /*calculate the center lines*/
    if(ui2_cur_line > pt_cnt_data->ui2_line_capacity)
    {
        ui2_copy_start = ((ui2_cur_line - pt_cnt_data->ui2_line_capacity) >> 1);
        ui2_cur_line = pt_cnt_data->ui2_line_capacity; /*ui2_cur_line keeps the lines to copy*/

        /*turn on the more-data flag*/
        pt_cnt_data->b_more_data = TRUE;
    }
    else
    {
        ui2_copy_start = 0; /*ui2_cur_line keeps the lines to copy*/

        /*turn off the more-data flag*/
        pt_cnt_data->b_more_data = FALSE;
    }

    /*copy line infos to content*/
    x_memcpy(pt_cnt_data->pt_line_infos, &a_line_infos[ui2_copy_start], ui2_cur_line * sizeof(WGL_TEXT_LINE_INFO));
    pt_cnt_data->ui2_lines_in_display = ui2_cur_line;

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
#if 0
static INT32 _wgl_text_cnt_layout_v_bottom(
    WGL_TEXT_CONTENT_T* pt_cnt_data)
{
    /*local varaible declaration*/
    UTF16_T                             a_w2s_break_chars[] = WGL_CAN_BREAK_CHARS;
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_TEXT_LINE_INFO      a_line_infos[TEXT_MAX_DIS_LINE] = {0};
    UINT16                  ui2_cur_line = 0;
    BOOL                    b_loop = FALSE;
    WGL_BREAK_LINE_INFO_T   t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T t_break_line_result = {0};
    UINT16                  ui2_first_token_pos;
    INT32                   i4_ret;
    GL_RECT_T               t_available_rect;


    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, pt_cnt_data->t_fg_inset.i4_left, pt_cnt_data->t_fg_inset.i4_top,
                        pt_cnt_data->t_fg_inset.i4_right, pt_cnt_data->t_fg_inset.i4_bottom);

    /*initialize variable*/
    t_break_line_Info.h_font = pt_cnt_data->t_text_font;
    t_break_line_Info.w2c_force_token = a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;
    t_break_line_Info.w2s_string = pt_cnt_data->a_w2_text;
    t_break_line_Info.i4_string_length = pt_cnt_data->ui2_text_length;
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1;
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;
    
    ui2_first_token_pos = 0;

    /*get the necessary line info*/
    while(NULL != t_break_line_result.w2s_next_start)
    {
        /*keep the next start*/
        a_line_infos[ui2_cur_line].w2s_string = t_break_line_result.w2s_next_start;
        
        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
            (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;
        
        /*Get text's necessary width*/
        i4_ret = wgl_get_break_line_pos(&t_break_line_Info, &ui2_first_token_pos, &t_break_line_result);
        if(i4_ret != WGLR_OK)
        {
            return i4_ret;
        }
        
        /*keep the necessary width*/
        a_line_infos[ui2_cur_line].ui2_width = t_break_line_result.ui2_width;
        a_line_infos[ui2_cur_line].ui2_str_len = t_break_line_result.ui2_char_number;
        
        /*update index for line info*/
        ui2_cur_line++;
        if(ui2_cur_line >= pt_cnt_data->ui2_line_capacity)
        {
            ui2_cur_line = 0;
            b_loop = TRUE;
        }
    }

    /*copy line infos to content*/
    if(b_loop)
    {
        x_memcpy(pt_cnt_data->pt_line_infos, &a_line_infos[ui2_cur_line], 
                    (pt_cnt_data->ui2_line_capacity - ui2_cur_line) * sizeof(WGL_TEXT_LINE_INFO));
        if(0 != ui2_cur_line)
        {
            x_memcpy(pt_cnt_data->pt_line_infos + pt_cnt_data->ui2_line_capacity - ui2_cur_line, 
                    a_line_infos, ui2_cur_line * sizeof(WGL_TEXT_LINE_INFO));
        }

        /*turn on the more-data flag*/
        pt_cnt_data->b_more_data = TRUE;
        pt_cnt_data->ui2_lines_in_display = pt_cnt_data->ui2_line_capacity;
    }
    else
    {
        x_memcpy(pt_cnt_data->pt_line_infos, a_line_infos, ui2_cur_line * sizeof(WGL_TEXT_LINE_INFO));

        /*turn off the more-data flag*/
        pt_cnt_data->b_more_data = FALSE;
        pt_cnt_data->ui2_lines_in_display = ui2_cur_line;
    }

    return WGLR_OK;
}
#endif

static INT32 _wgl_text_cnt_calc_total_line_ex(
    WGL_TEXT_CONTENT_T*         pt_cnt_data,
    const GL_RECT_T*            pt_available_rect,
    UINT16                      ui2_state,
    UINT16*                     pui2_total_lines)
{
    /*local varaible declaration*/
    UTF16_T*                            a_w2s_break_chars = pt_cnt_data->as_break_chars;/*WGL_CAN_BREAK_CHARS*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UINT16                              ui2_first_token_pos;
//    UINT16                              ui2_len_capacity;
    INT32                               i4_ret;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    WGL_TEXT_MULTI_STRING *             pt_string_element = NULL;
#ifdef ARAB_EXIST_SUPPORT
	BOOL								b_exist_arab;
#endif    
    if(NULL == pui2_total_lines)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if (pt_cnt_data->pt_multi_string == NULL)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        *pui2_total_lines = 0;
        return WGLR_OK;
    }

    pt_string_element = pt_cnt_data->pt_multi_string;
    *pui2_total_lines = 0;

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check cache*/
    if(!WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TOTAL_LINES_NOT_INIT))
    {
        *pui2_total_lines = pt_cnt_data->ui2_total_lines;
        return WGLR_OK;
    }
    else
    {
        /*reset ui2_total_lines*/
        pt_cnt_data->ui2_total_lines = 0;
    }    
     /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    /*initialize variable*/
    t_break_line_Info.h_font = t_text_font;
    t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(pt_available_rect); 
    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;
    
//    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);


    while (pt_string_element != NULL)
    {
        if (pt_string_element->ps_string == NULL)
        {
            (pt_cnt_data->ui2_total_lines)++;
        }
        else
        {
#ifdef ARAB_EXIST_SUPPORT
            b_exist_arab = check_exist_arabic(pt_string_element->ps_string,pt_string_element->ui2_size);
#endif	
            t_break_line_Info.i4_string_length = (INT32)pt_string_element->ui2_size;
            
#ifdef ARAB_EXIST_SUPPORT
             if (!b_exist_arab)
#else
             if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif 
            {
                t_break_line_Info.w2s_string = pt_string_element->ps_string;
                
            }
            else
            {
            	if (pt_string_element->ps_arab_dsp_text == NULL)
           		{

					pt_string_element->ps_arab_dsp_text = (UTF16_T *)WGL_MEM_ALLOC((pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
					if (pt_string_element->ps_arab_dsp_text == NULL)
					{
						return WGLR_OUT_OF_MEMORY;
					}
					x_memset(pt_string_element->ps_arab_dsp_text, 0, (pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
					
					i4_ret = _wgl_text_cnt_update_arab_text_ex(pt_cnt_data, pt_string_element->ps_string, pt_string_element->ps_arab_dsp_text);
					if (WGLR_OK != i4_ret)
					 {
					     return i4_ret;
					}


					pt_string_element->ui2_arab_dsp_text_len = pt_cnt_data->ui2_arab_dsp_text_len;
					if (pt_string_element->ui2_arab_dsp_text_len > pt_string_element->ui2_size)
					{
						x_dbg_stmt("[Text]memory error.\n");
					}
           		}
                
                t_break_line_Info.w2s_string = pt_string_element->ps_arab_dsp_text;
                t_break_line_Info.i4_string_length = (INT32)pt_string_element->ui2_arab_dsp_text_len;                    
            }
            t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

            ui2_first_token_pos = 0;
            
            
            /*get the necessary line info*/
            while(t_break_line_result.w2s_next_start != NULL && t_break_line_result.w2s_next_start[0] != 0)
            {
                /*update parameter*/
                ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                                      (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                                      0;
                t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
                t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;
                
                /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
                if (!b_exist_arab)
#else
                if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
                if(i4_ret != WGLR_OK)
                {
                    DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }
                
                (pt_cnt_data->ui2_total_lines)++;
                pt_cnt_data->ui2_total_lines += pt_cnt_data->ui2_interlace;
            }
        }

        pt_string_element = pt_string_element->pt_next;
            
    }
    *pui2_total_lines = pt_cnt_data->ui2_total_lines;
    WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TOTAL_LINES_NOT_INIT);
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
static INT32 _wgl_text_cnt_calc_total_line(
    WGL_TEXT_CONTENT_T*         pt_cnt_data,
    const GL_RECT_T*            pt_available_rect,
    UINT16                      ui2_state,
    UINT16*                     pui2_total_lines)
{
    /*local varaible declaration*/
    UTF16_T*                            a_w2s_break_chars = pt_cnt_data->as_break_chars;/*WGL_CAN_BREAK_CHARS*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UINT16                              ui2_first_token_pos;
    UINT16                              ui2_len_capacity;
    INT32                               i4_ret;
    HANDLE_T                        t_text_font;
    WGL_FONT_INFO_T           t_font_info;
#ifdef ARAB_EXIST_SUPPORT
	BOOL								b_exist_arab;
#endif    
    if(NULL == pui2_total_lines)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    *pui2_total_lines = 0;

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_FONT_NOT_INIT))
    {
        /*use system font*/
        i4_ret = _wgl_text_cnt_use_sys_font(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check cache*/
    if(!WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, TEXT_TOTAL_LINES_NOT_INIT))
    {
        *pui2_total_lines = pt_cnt_data->ui2_total_lines;
        return WGLR_OK;
    }
    else
    {
        /*reset ui2_total_lines*/
        pt_cnt_data->ui2_total_lines = 0;
    }    
     /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    /*initialize variable*/
    t_break_line_Info.h_font = t_text_font;
    t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    
    t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_text_length;
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(pt_available_rect); 
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
#ifdef ARAB_EXIST_SUPPORT
	b_exist_arab = check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length);
#endif	

#ifdef ARAB_EXIST_SUPPORT
     if (!b_exist_arab)
#else
     if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif 
    {
        t_break_line_Info.w2s_string = pt_cnt_data->pt_w2_text;
    }
    else
    {
    
        i4_ret = _wgl_text_cnt_update_arab_text(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        t_break_line_Info.w2s_string = &(pt_cnt_data->pt_w2_arab_dsp_text[ui2_len_capacity - 1]);
        t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_arab_dsp_text_len;                    
    }
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;

    ui2_first_token_pos = 0;
    
    
    /*get the necessary line info*/
    while(t_break_line_result.w2s_next_start != NULL && t_break_line_result.w2s_next_start[0] != 0)
    {
        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                              (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                              0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;
        
        /*Get text's necessary width*/
#ifdef ARAB_EXIST_SUPPORT
        if (!b_exist_arab)
#else
        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
#endif
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
        if(i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        (pt_cnt_data->ui2_total_lines)++;
    }
    
    *pui2_total_lines = pt_cnt_data->ui2_total_lines;
    WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, TEXT_TOTAL_LINES_NOT_INIT);
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
static INT32 _wgl_text_cnt_calc_page_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    const GL_RECT_T*                    pt_available_rect,
    UINT16                              ui2_state,
    UINT16*                             pui2_page_lines)
{
    INT32                               i4_widget_height, i4_total_line_height,i4_ret;
    UINT16                              ui2_lines;
    INT16                               i2_line_height;
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;

    if(NULL == pui2_page_lines)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    *pui2_page_lines = 0;
        
    /*get widget height*/
    i4_widget_height = WGL_RECT_GET_HEIGHT(pt_available_rect);
    WGL_ASSERT(i4_widget_height >= 0);
    
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
   /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    WGL_ASSERT(FER_OK != i4_ret && i2_line_height >= 0);
    
    /*calculate the line number to paint*/
    i4_total_line_height = pt_cnt_data->i4_line_gap + i2_line_height;
    if(i4_total_line_height <= 0)
    {
        return WGLR_OK;
    }
    
    ui2_lines =  (UINT16)(i4_widget_height / i4_total_line_height);
    ui2_lines += ((i4_widget_height - i4_total_line_height * ui2_lines) >= i2_line_height) ? 1 : 0;

    *pui2_page_lines = ui2_lines;
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
static void _wgl_text_cnt_reset_dirty_flag(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    UINT32                          ui4_extra_flag)
{
    pt_cnt_data->b_is_data_dirty = TRUE;
    pt_cnt_data->b_more_data = FALSE;

    WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, ui4_extra_flag);
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
static INT32 _wgl_text_cnt_get_smart_cut_char(
    UTF16_T                         a_w2s_smart_cut_char[TEXT_SMART_CUT_MAX_LENGHT],
    UINT16*                         pui2_smart_cut_len)
{
    CHAR*                           s_smart_cut_chars = TEXT_SMART_CUT_CHARS;
    INT32                           i4_temp;
    INT32                           i4_ret;
    
    /*get UTF16 foramt for smart-cut characters*/
    i4_ret = x_uc_ps_to_w2s(s_smart_cut_chars, a_w2s_smart_cut_char, TEXT_SMART_CUT_MAX_LENGHT);
    if(UCR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to convert to UTF16. (UCR=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*get smart-cut length*/
    i4_temp = (INT32)x_uc_w2s_strlen(a_w2s_smart_cut_char);
    WGL_ASSERT(i4_temp >= 0);
    *pui2_smart_cut_len = (UINT16)i4_temp;

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
static INT32 _wgl_text_cnt_get_smart_cut_info(
    WGL_TEXT_CONTENT_T*             pt_cnt_data,
    HFONT_T                         h_font,
    const UTF16_T*                  w2s_line_string,
    INT32                           i4_line_width,
    UINT16                          ui2_line_char_len,
    UTF16_T                         a_w2s_smart_cut_char[TEXT_SMART_CUT_MAX_LENGHT],
    UINT16*                         pui2_smart_cut_len,
    INT32*                          pi4_smart_cut_width,
    UINT16*                         pui2_keep_len,
    INT32*                          pi4_keep_width)
{
    INT32                           i4_temp_width;
    INT32                           i4_ret;
    UTF16_T*                        pw2s_temp;
    UINT32                          ui4_temp_num_ch;

    /*get smart-cut character*/
    i4_ret = _wgl_text_cnt_get_smart_cut_char(a_w2s_smart_cut_char, pui2_smart_cut_len);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*get smart-cut width*/
    if(FER_OK != x_fe_get_string_size(h_font, a_w2s_smart_cut_char, -1, pi4_smart_cut_width, &i4_temp_width))
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get string size. (FER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
#if 0
    if(ui2_line_char_len <= *pui2_smart_cut_len)
    {
        *pui2_keep_len = 0;
         if(pi4_keep_width != NULL)
        {
         *pi4_keep_width = 0;
        }
    }
    else
#endif
    {
        INT32 i4_draw_len = 0;
        GL_RECT_T                           t_available_rect;
        /*calculate available rectangle*/
        WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
        WGL_RECT_INSET(&t_available_rect, 
                       pt_cnt_data->t_fg_inset.i4_left, 
                       pt_cnt_data->t_fg_inset.i4_top,
                       pt_cnt_data->t_fg_inset.i4_right, 
                       pt_cnt_data->t_fg_inset.i4_bottom);
        
        
        if (*pi4_smart_cut_width >= WGL_RECT_GET_WIDTH(&t_available_rect))
        {
            *pui2_keep_len = 0;
            *pi4_keep_width = 0;
            
        #ifdef ARAB_EXIST_SUPPORT
            if (!check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length))
        #else
            if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
        #endif
            {
            
            /*get the display number for the last line*/
            i4_ret = wgl_get_max_char_in_box(
                         h_font, 
                         a_w2s_smart_cut_char, 
                         (INT32)pui2_smart_cut_len,
                         TRUE,
                         WGL_RECT_GET_WIDTH(&t_available_rect),   
                         NULL, 
                         &pw2s_temp, 
                         &ui4_temp_num_ch, 
                         &i4_temp_width);
            }
            else
            {
                 /*get the display number for the last line*/
                i4_ret = wgl_get_max_char_in_box_for_arab(
                    (INT32)pui2_smart_cut_len,
                    h_font,
                    &a_w2s_smart_cut_char[(INT32)pui2_smart_cut_len - 1],
                    (INT32)pui2_smart_cut_len,
                    TRUE,
                    WGL_RECT_GET_WIDTH(&t_available_rect),
                    NULL, 
                    NULL,
                    0,
                    0,
                    TRUE,
                    &pw2s_temp, 
                    &ui4_temp_num_ch, 
                    &i4_temp_width);
            }
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            
            WGL_ASSERT(ui4_temp_num_ch < 0x0000FFFF);
            *pui2_smart_cut_len = (UINT16)ui4_temp_num_ch;
            if(pi4_smart_cut_width != NULL)
            {
             *pi4_smart_cut_width = i4_temp_width;
            }
            return WGLR_OK;
        }
        else if (i4_line_width + *pi4_smart_cut_width > WGL_RECT_GET_WIDTH(&t_available_rect))
        {
            i4_draw_len = WGL_RECT_GET_WIDTH(&t_available_rect) - *pi4_smart_cut_width;
        }
        else
        {
            i4_draw_len = i4_line_width;
        }
        
    #ifdef ARAB_EXIST_SUPPORT
        if (!check_exist_arabic(pt_cnt_data->pt_w2_text,pt_cnt_data->ui2_text_length))
    #else
        if (pt_cnt_data->ui1_read_direction== WGL_TEXT_DIR_LEFT_TO_RIGHT )
    #endif
        {
        
        /*get the display number for the last line*/
        i4_ret = wgl_get_max_char_in_box(
                     h_font, 
                     w2s_line_string, 
                     (INT32)ui2_line_char_len,
                     TRUE,
                     i4_draw_len,   
                     NULL, 
                     &pw2s_temp, 
                     &ui4_temp_num_ch, 
                     &i4_temp_width);
        }
        else
        {
             /*get the display number for the last line*/
            i4_ret = wgl_get_max_char_in_box_for_arab(
                (INT32)ui2_line_char_len,
                h_font,
                w2s_line_string + ui2_line_char_len -1,
                (INT32)ui2_line_char_len,
                TRUE,
                i4_draw_len,
                NULL, 
                NULL,
                0,
                0,
                TRUE,
                &pw2s_temp, 
                &ui4_temp_num_ch, 
                &i4_temp_width);
        }
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        WGL_ASSERT(ui4_temp_num_ch < 0x0000FFFF);
        *pui2_keep_len = (UINT16)ui4_temp_num_ch;
        if(pi4_keep_width != NULL)
        {
         *pi4_keep_width = i4_temp_width;
        }
    }

   
    return WGLR_OK;
}


static INT32 _wgl_text_cnt_get_max_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16*                             pt_ui2_max_line)
{
    if (pt_ui2_max_line == NULL)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid null arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    *pt_ui2_max_line = pt_cnt_data->ui2_line_capacity;
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
static INT32 _wgl_text_cnt_set_max_line(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_max_line)
{
    WGL_TEXT_LINE_INFO*                 pt_line_info;

    /*check parameter*/
    if(4096 < ui2_max_line)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Invalid arguments. For WGL_CMD_TEXT_SET_MAX_DIS_LINE, the max line number cannot be over 4096. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check setting*/
    if(0 == (TEXT_MULTILINE & pt_cnt_data->ui4_extra_flag) || 0 == (TEXT_USER_DEF_MAX_LINE & pt_cnt_data->ui4_extra_flag))
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: This command is only for WGL_STL_TEXT_MULTILINE with WGL_STL_TEXT_MAX_DIS_USER_DEF widget. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    if(ui2_max_line == pt_cnt_data->ui2_line_capacity)
    {
        return WGLR_OK;
    }

    /*free buffer for line info*/
    WGL_CHECK_INTERNAL(pt_cnt_data->pt_line_infos);

    /**/
    pt_line_info = (WGL_TEXT_LINE_INFO*)WGL_WIDGET_MEM_ALLOC((VOID*)pt_cnt_data,sizeof(WGL_TEXT_LINE_INFO) * ui2_max_line);
    if(NULL == pt_line_info)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to alloc memory for WGL_CMD_TEXT_SET_MAX_DIS_LINE (lines = %d). (%d) at L%d\n\n", ui2_max_line, WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }

    /*init buffer*/
    x_memset(pt_line_info, 0, sizeof(WGL_TEXT_LINE_INFO) * ui2_max_line);

    /*dup current layout*/
    if(ui2_max_line < pt_cnt_data->ui2_line_capacity)
    {
        x_memcpy(pt_line_info, pt_cnt_data->pt_line_infos, sizeof(WGL_TEXT_LINE_INFO) * ui2_max_line);
    }
    else
    {
        _wgl_text_cnt_reset_dirty_flag(pt_cnt_data, TEXT_TOTAL_LINES_NOT_INIT);
    }

    /*free buffer for line info*/
    WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_LINE_INFO) * pt_cnt_data->ui2_line_capacity,pt_cnt_data->pt_line_infos);
    pt_cnt_data->pt_line_infos = pt_line_info;
    pt_cnt_data->ui2_line_capacity = ui2_max_line;
    
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_begin_scrl_check(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state)
{   
    HANDLE_T                        t_text_font;
    WGL_FONT_INFO_T           t_font_info;
    INT32                               i4_ret;
     /*null text check*/
    if (pt_cnt_data->ui2_text_length == 0)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: No text to scroll. File=%s,Line=%d\n\n",  __FILE__,__LINE__));
        return WGLR_INV_STATE;
    }
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
     /*null font check*/
    if (t_text_font == NULL_HANDLE)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: No font to scroll. File=%s,Line=%d\n\n",  __FILE__,__LINE__));
        return WGLR_INV_STATE;
    }    
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_scrl_smart_check(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state)
{   
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    INT32                               i4_ret;
    GL_RECT_T                           t_available_rect;
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};   
    UINT16                              ui2_len_capacity;     
    
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
     /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    /*get line width*/
    t_break_line_Info.i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);

    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);   
   
    /*Get text's necessary width*/
    if(t_break_line_Info.i4_box_width > 0)
    {
        t_break_line_Info.h_font = t_text_font;
        t_break_line_Info.w2c_force_token = (TEXT_IGNORE_NEWLINE & pt_cnt_data->ui4_extra_flag) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
        /*CR2637, in single line style, print characters as many as possible.*/
        t_break_line_Info.w2s_can_break = NULL; /*a_w2s_break_chars;*/
        if (WGL_TEXT_DIR_LEFT_TO_RIGHT == pt_cnt_data->ui1_read_direction)
        {
            t_break_line_Info.w2s_string = pt_cnt_data->pt_w2_text;
        }
        else
        {
            t_break_line_Info.w2s_string = &(pt_cnt_data->pt_w2_text[ui2_len_capacity - 1]);
        }
        
        t_break_line_Info.i4_string_length = (INT32)pt_cnt_data->ui2_text_length;

        /*for soft-hyphen*/
        t_break_line_Info.w2s_skip = a_w2s_skip_chars;
        t_break_line_Info.ui2_skip_count = (UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
        t_break_line_Info.ui2_skip_width = 0;
        t_break_line_Info.b_skip_as_break = FALSE;
        if (WGL_TEXT_DIR_LEFT_TO_RIGHT == pt_cnt_data->ui1_read_direction)
        {
            i4_ret = wgl_get_break_line_pos(&t_break_line_Info, NULL, &t_break_line_result);
        }
        else
        {
            i4_ret = wgl_get_break_line_pos_for_arab(
                        t_break_line_Info.w2s_string ,
                        &t_break_line_Info, 
                        NULL,
                        &t_break_line_result);
        }            
        
        if(i4_ret != WGLR_OK)
        {           
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    

        /*data more then one line*/
        if (NULL == t_break_line_result.w2s_next_start)
        {          
            return WGLR_INV_CMD;
        }
    } 
    return WGLR_OK;
}



static VOID _wgl_text_cnt_begin_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    VOID*                               pv_param1)
{   
    pt_cnt_data->ui2_scrl_style = ((WGL_TEXT_SCRL_T*)pv_param1)->ui2_style;   
    pt_cnt_data->ui2_scrl_count = ((WGL_TEXT_SCRL_T*)pv_param1)->ui2_scrl_count; 
    if (WGL_TEXT_DIR_LEFT_TO_RIGHT == pt_cnt_data->ui1_read_direction)
    {   
        pt_cnt_data->ui2_overflow_flag |= TEXT_OVERFLOW_RIGHT;
    }
    else
    {
        pt_cnt_data->ui2_overflow_flag |= TEXT_OVERFLOW_LEFT;
    } 
    WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag,TEXT_SCROLLING);     
   
}



static INT32 _wgl_get_calculate_char_width(
    HFONT_T                             h_font,
    UTF16_T                             w2_left_char,
    UTF16_T                             w2_right_char,
    BOOL                                b_flag_left)
{
   INT32                                i4_adv_x=0,i4_adv_y=0,i4_kern_x=0, i4_kern_y=0;
   INT32                                i4_ret;
   INT32                                i4_remove_width = 0;

    if (b_flag_left)
    {
        i4_ret = x_fe_get_character_size(h_font,
                                (CHAR_CODE)w2_left_char,
                                &i4_adv_x,
                                &i4_adv_y);
    }
    else
    {
        i4_ret = x_fe_get_character_size(h_font,
                                (CHAR_CODE)w2_right_char,
                                &i4_adv_x,
                                &i4_adv_y);
    }
    
    if (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
    {
        i4_remove_width += i4_adv_x;                  
    }
    if (w2_right_char != (UTF16_T)0)
    {
        i4_ret = x_fe_get_kerning( 
                     h_font,
                     (CHAR_CODE)w2_left_char,
                     (CHAR_CODE)w2_right_char,
                     &i4_kern_x,
                     &i4_kern_y);
        if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
        {
            i4_remove_width += i4_kern_x;
        }
    }
    return i4_remove_width;

}

static INT32 _wgl_text_cnt_do_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data,
    UINT16                              ui2_state)
{    
    INT32                               i4_ret;
    UTF16_T*                            w2s_string = NULL;
    UINT16                              ui2_len_capacity;
    INT32                               i4_box_width = 0;
    GL_RECT_T                           t_available_rect;
    UTF16_T*                            w2s_next_start = NULL;
    UTF16_T                             w2_left_char=(UTF16_T)0,w2_right_char=(UTF16_T)0,w2_first_char=(UTF16_T)0,w2_second_char=(UTF16_T)0;                           
    UINT32                              ui4_chars_in_line;
    INT32                               i4_adv_x=0,i4_adv_y=0;
    INT32                               i4_break_width, i4_shift_width;
    INT32                               i4_remove_width=0,i4_tmp_width=0;
    UINT16                              ui2_append_idx=0;
    INT32                               i4_append_width=0,i4_append_count=0;      
    HANDLE_T                            t_text_font;
    WGL_FONT_INFO_T                     t_font_info;
    UINT8								ui1_cur_font_id;
    
    i4_ret = WGLR_OK;
    /*get font according to widget state*/
    i4_ret = _wgl_text_cnt_select_text_font( pt_cnt_data, ui2_state, &t_text_font, &t_font_info);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
	
    i4_ret = x_fe_get_cur_font_resource(t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
	   return i4_ret;
    }
    if (ui1_cur_font_id != pt_cnt_data->ui1_cur_font_id)
    {
		pt_cnt_data->b_is_data_dirty = TRUE;
		pt_cnt_data->ui1_cur_font_id = ui1_cur_font_id;
    }
    
    /*bounds ,text or font has been changed reset scrolling*/
    if (pt_cnt_data->b_is_data_dirty)
    {
         if (t_text_font == NULL_HANDLE ||
             pt_cnt_data->ui2_text_length == 0)
         {            
             return WGL_TEXT_SCRL_END;             
         }
         _wgl_text_cnt_reset_scrl(pt_cnt_data);
         pt_cnt_data->ui2_scrl_step = 0;
         /*reset dirty flag*/
         pt_cnt_data->b_is_data_dirty = FALSE;
    }        
    if (((WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_scrl_style , WGL_TEXT_SCRL_TILL_LAST_CHAR))
        && (pt_cnt_data->ui2_scrl_begin_idx == pt_cnt_data->ui2_text_length))
        || ((WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui2_scrl_style , WGL_TEXT_SCRL_TILL_LAST_CHAR))
        && (pt_cnt_data->ui2_scrl_begin_idx + pt_cnt_data->ui2_scrl_len == pt_cnt_data->ui2_text_length)))
    {
                   
        if (WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui2_scrl_style , WGL_TEXT_SCRL_REPEAT))
        {
            return WGL_TEXT_SCRL_END;                   
        }
        /*is not infinite repeat times*/
        if (pt_cnt_data->ui2_scrl_count != 0)
        {
             pt_cnt_data->ui2_scrl_step++;
             if( pt_cnt_data->ui2_scrl_step == pt_cnt_data->ui2_scrl_count)
             {
                  return WGL_TEXT_SCRL_END;  
             }
         }            
        _wgl_text_cnt_reset_scrl(pt_cnt_data);                  
       
    }
          
    /*calculate available rectangle*/
    WGL_RECT_COPY(&t_available_rect, &pt_cnt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_cnt_data->t_fg_inset.i4_left, 
                   pt_cnt_data->t_fg_inset.i4_top,
                   pt_cnt_data->t_fg_inset.i4_right, 
                   pt_cnt_data->t_fg_inset.i4_bottom);

    /*get line width*/
    i4_box_width = WGL_RECT_GET_WIDTH(&t_available_rect);
    ui2_len_capacity = (UINT16)_TEXT_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag);
    
    if (i4_box_width > 0)
    {

        if (WGL_TEXT_DIR_LEFT_TO_RIGHT  == pt_cnt_data->ui1_read_direction)
        {
            w2s_string = pt_cnt_data->pt_w2_text + pt_cnt_data->ui2_scrl_begin_idx;
        }
        else
        {
            w2s_string = &(pt_cnt_data->pt_w2_text[ui2_len_capacity - 1]);
        }            
          
        if (pt_cnt_data->ui2_scrl_len == 0 && pt_cnt_data->ui2_scrl_begin_idx == 0)
        {            
            if (WGL_TEXT_DIR_LEFT_TO_RIGHT  == pt_cnt_data->ui1_read_direction)
            {
                if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_scrl_style,WGL_TEXT_SCRL_FROM_FIRST_CHAR))
                {
                    w2_left_char = *w2s_string;
                    i4_ret = x_fe_get_character_size(t_text_font,
                                (CHAR_CODE)w2_left_char,
                                &i4_adv_x,
                                &i4_adv_y);
                    if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
                    {
                        i4_remove_width += i4_adv_x;  
                        i4_break_width = i4_adv_x;
                        ui4_chars_in_line = 1;
                    }
                    if (pt_cnt_data->ui2_text_length > 1)
                    {
                        w2s_next_start = &(pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx + 1]);                        
                    }
                    else
                    {
                        w2s_next_start = NULL;
                    }
                    
                }
                else
                {
                    i4_ret = wgl_get_max_char_in_box_ex(t_text_font,
                                                w2s_string,
                                                (INT32)(pt_cnt_data->ui2_text_length),
                                                TRUE,
                                                i4_box_width,
                                                NULL,
                                                NULL,
                                                0,
                                                0,
                                                TRUE,
                                                &w2s_next_start,
                                                &ui4_chars_in_line,
                                                &i4_break_width); 
                    /*max char count is 0, no char can be displayed*/
                    if(ui4_chars_in_line == 0)
                    {
                        ui4_chars_in_line = 1;
                        w2_first_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx];
                        if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                        {
                            w2_second_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx + 1] ;
                        }
                        i4_break_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE);  
                    }
                }
                          
            }
            else
            {
                if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_scrl_style,WGL_TEXT_SCRL_FROM_FIRST_CHAR))
                {
                    w2_right_char = *w2s_string;;
                    i4_ret = x_fe_get_character_size(t_text_font,
                                (CHAR_CODE)w2_right_char,
                                &i4_adv_x,
                                &i4_adv_y);
                    if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
                    {
                        i4_remove_width += i4_adv_x;  
                        i4_break_width = i4_adv_x;
                        ui4_chars_in_line = 1;
                    }
                    if (pt_cnt_data->ui2_text_length > 1)
                    {
                        w2s_next_start = w2s_string - 1;                        
                    }
                    else
                    {
                        w2s_next_start = NULL;
                    }
                    
                }
                else
                {
                    i4_ret = wgl_get_max_char_in_box_for_arab(
                                                (INT32)(pt_cnt_data->ui2_text_length),
                                                t_text_font,
                                                w2s_string,
                                                (INT32)(pt_cnt_data->ui2_text_length),
                                                TRUE,
                                                i4_box_width,
                                                NULL,
                                                NULL,
                                                0,
                                                0,
                                                TRUE,
                                                &w2s_next_start,
                                                &ui4_chars_in_line,
                                                &i4_break_width); 
                    /*max char count is 0, no char can be displayed*/
                    if(ui4_chars_in_line == 0)
                    {
                        ui4_chars_in_line = 1;
                        w2_first_char = *w2s_string;
                        if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                        {
                            w2_second_char = *(w2s_string - 1);
                        }
                        i4_break_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE); 
                    }
                }
               
            }   
            
            if(i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to wgl_get_max_char_in_box_ex. i4_ret=%d,File =%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
                return i4_ret;
            } 
            pt_cnt_data->ui2_scrl_begin_idx += pt_cnt_data->ui2_scrl_len;
            pt_cnt_data->ui2_scrl_len = (UINT16)ui4_chars_in_line;
            pt_cnt_data->ui2_scrl_width = (UINT16)i4_break_width;  
            if (WGL_TEXT_DIR_LEFT_TO_RIGHT  == pt_cnt_data->ui1_read_direction)
            {
                /*quite small that can't display one char*/
                if (t_available_rect.i4_right < (INT32)(pt_cnt_data->ui2_scrl_width))
                {
                     pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left);
                }
                else
                {
                     pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_right -  pt_cnt_data->ui2_scrl_width) ;
                }            
                if (pt_cnt_data->ui2_scrl_begin_idx + pt_cnt_data->ui2_scrl_len == pt_cnt_data->ui2_text_length)
                {
                     WGL_CLEAR_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT);
                }
            }
            else
            {                
                pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left); 
                if (pt_cnt_data->ui2_scrl_begin_idx + pt_cnt_data->ui2_scrl_len == pt_cnt_data->ui2_text_length)
                {
                     WGL_CLEAR_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT);
                }
            }
            
        } 
        else /*not first frame of text*/
        {
            if (WGL_TEXT_DIR_LEFT_TO_RIGHT  == pt_cnt_data->ui1_read_direction)
            {               
               
                if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT))
                {
                    ui2_append_idx = pt_cnt_data->ui2_scrl_begin_idx + pt_cnt_data->ui2_scrl_len;
                    if (ui2_append_idx == pt_cnt_data->ui2_text_length)
                    {
                        WGL_CLEAR_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT);
                    }
                }
                else /*right not overflow*/
                {
                    ui2_append_idx = 0;  
                    /* right not overflow , but  left overflow*/
                    if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT))
                    {
                        pt_cnt_data->ui2_scrl_len -= 1;
                        pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left);                       
                        w2_left_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx]; 
                        pt_cnt_data->ui2_scrl_begin_idx++;
                        if (pt_cnt_data->ui2_scrl_begin_idx <  pt_cnt_data->ui2_text_length)/*till last char*/
                        {
                            w2_right_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx];                         
                            i4_tmp_width = _wgl_get_calculate_char_width(
                                               t_text_font,
                                               w2_left_char,
                                               w2_right_char,
                                               TRUE);   
                            pt_cnt_data->ui2_scrl_width -= i4_tmp_width;
                        }
                        else
                        {
                            pt_cnt_data->ui2_scrl_width = 0;

                        }
                        
                    }
                    else /*both right and  left are not overflow*/
                    {
                        w2_first_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx];
                       
                        if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                        {
                            w2_second_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx + 1] ;
                        }
                        i4_shift_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE);                      
                        if (pt_cnt_data->ui2_scrl_start_x - i4_shift_width < t_available_rect.i4_left)
                        {
                            WGL_SET_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT);
                            if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                            {
                               pt_cnt_data->ui2_scrl_begin_idx++;
                               pt_cnt_data->ui2_scrl_len -= 1;
                               pt_cnt_data->ui2_scrl_width -= i4_shift_width;
                               pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left);   
                            }
                        }
                        else
                        {
                            pt_cnt_data->ui2_scrl_start_x -= i4_shift_width;

                        }
                    }/*both right and  left are not overflow*/
                    
                }/*right not overflow*/
                   
               if (ui2_append_idx <  pt_cnt_data->ui2_text_length && ui2_append_idx > 0)
               {
                   w2_left_char = pt_cnt_data->pt_w2_text[ui2_append_idx -1];
                   if (WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT))
                   {
                       w2_right_char = pt_cnt_data->pt_w2_text[ui2_append_idx];  
                       i4_append_width = _wgl_get_calculate_char_width(
                                          t_text_font,
                                          w2_left_char,
                                          w2_right_char,
                                          FALSE);                       
                       if (i4_append_width > i4_box_width -  pt_cnt_data->ui2_scrl_width)
                       {
                           w2_first_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx];
                    
                           if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                           {
                              w2_second_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx + 1] ;
                           }
                           i4_remove_width = _wgl_get_calculate_char_width(
                                                 t_text_font,
                                                 w2_first_char,
                                                 w2_second_char,
                                                 TRUE);         
                           pt_cnt_data->ui2_scrl_begin_idx++;  
                           WGL_SET_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT);
                       }                           
                       else
                       {
                           i4_remove_width = 0;
                           pt_cnt_data->ui2_scrl_len += 1;   
                           /*pt_cnt_data->ui2_scrl_begin_idx not change*/
                       }
                       pt_cnt_data->ui2_scrl_width += i4_append_width - i4_remove_width;
                       if ((UINT16)(t_available_rect.i4_right) < pt_cnt_data->ui2_scrl_width)
                       {
                           pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left);
                       }
                       else
                       {
                           pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_right -  pt_cnt_data->ui2_scrl_width);
                       }   
                      

                   }
                   else /*text overflow left*/
                   { 
                       w2_first_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx];
                       
                       if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                       {
                           w2_second_char = pt_cnt_data->pt_w2_text[pt_cnt_data->ui2_scrl_begin_idx + 1] ;
                       }
                       i4_remove_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE);
                       pt_cnt_data->ui2_scrl_width -= i4_remove_width;
                       for (;ui2_append_idx < pt_cnt_data->ui2_text_length; ui2_append_idx++)
                       {
                           w2_right_char = pt_cnt_data->pt_w2_text[ui2_append_idx];  
                           i4_tmp_width = _wgl_get_calculate_char_width(
                                              t_text_font,
                                              w2_left_char,
                                              w2_right_char,                                              
                                              FALSE);
                            /*quite small that can't display one char*/
                           if (pt_cnt_data->ui2_scrl_width == 0)
                           {
                               i4_append_width += i4_tmp_width; 
                               i4_append_count++;
                               break;
                           }
                          
                           if (i4_append_width + i4_tmp_width > i4_box_width - pt_cnt_data->ui2_scrl_width)
                           {
                               break;  
                           }
                           else if (i4_append_width + i4_tmp_width == i4_box_width - pt_cnt_data->ui2_scrl_width)
                           {
                               i4_append_width += i4_tmp_width; 
                               i4_append_count++;
                               break;
                           }
                           else
                           {
                               i4_append_width += i4_tmp_width;  
                                i4_append_count++;
                           }
                          
                           w2_left_char = w2_right_char;                           
                       }
                       pt_cnt_data->ui2_scrl_begin_idx++;
                       pt_cnt_data->ui2_scrl_len += i4_append_count -1;
                       pt_cnt_data->ui2_scrl_width += i4_append_width;
                       pt_cnt_data->ui2_scrl_start_x = (UINT16)t_available_rect.i4_left;
                      
                   }  /*text overflow left*/                     
               }                     
             
            }
            else /*text read from right*/
            { 
                if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT))
                {
                    ui2_append_idx = pt_cnt_data->ui2_scrl_begin_idx + pt_cnt_data->ui2_scrl_len;
                    if (ui2_append_idx == pt_cnt_data->ui2_text_length)
                    {
                        WGL_CLEAR_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_LEFT);
                    }
                }
                else /*left not overflow*/
                {
                    ui2_append_idx = 0;  
                    /* left not overflow , but  right overflow*/
                    if (WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT))
                    {
                        pt_cnt_data->ui2_scrl_len -= 1;
                        /*pt_cnt_data->ui2_scrl_start_x = t_available_rect.i4_left;*/
                        w2_right_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx); 
                        pt_cnt_data->ui2_scrl_begin_idx++;
                        if (pt_cnt_data->ui2_scrl_begin_idx <  pt_cnt_data->ui2_text_length)/*till last char*/
                        {
                            w2_left_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx); 
                            i4_tmp_width = _wgl_get_calculate_char_width(
                                               t_text_font,
                                               w2_left_char,
                                               w2_right_char,
                                               FALSE);   
                            pt_cnt_data->ui2_scrl_width -= i4_tmp_width;
                        }
                        else
                        {
                            pt_cnt_data->ui2_scrl_width = 0;

                        }
                        pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_right -  pt_cnt_data->ui2_scrl_width); 
                        
                    }
                    else /*both right and  left are not overflow*/
                    {
                        w2_first_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx);
                       
                        if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                        {
                            w2_second_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx - 1);
                        }
                        i4_shift_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE);                      
                        if (pt_cnt_data->ui2_scrl_start_x + i4_shift_width +  pt_cnt_data->ui2_scrl_width > t_available_rect.i4_right)
                        {
                            WGL_SET_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT);
                            if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                            {
                               pt_cnt_data->ui2_scrl_begin_idx++;
                               pt_cnt_data->ui2_scrl_len -= 1;
                               pt_cnt_data->ui2_scrl_width -= i4_shift_width;
                               pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_right - pt_cnt_data->ui2_scrl_width);   
                            }
                        }
                        else
                        {
                            pt_cnt_data->ui2_scrl_start_x += i4_shift_width;

                        }
                    }/*both right and  left are not overflow*/
                    
                }/*left not overflow*/
                   
               if (ui2_append_idx <  pt_cnt_data->ui2_text_length && ui2_append_idx > 0)
               {
                   w2_right_char = *(w2s_string - ui2_append_idx + 1);
                   if (WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT))
                   {
                       w2_left_char = *(w2s_string - ui2_append_idx);  
                       i4_append_width = _wgl_get_calculate_char_width(
                                          t_text_font,
                                          w2_left_char,
                                          w2_right_char,
                                          TRUE);                       
                       if (i4_append_width > i4_box_width -  pt_cnt_data->ui2_scrl_width)
                       {
                           w2_first_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx);
                    
                           if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                           {
                               w2_second_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx - 1) ;
                           }
                           i4_remove_width = _wgl_get_calculate_char_width(
                                                 t_text_font,
                                                 w2_first_char,
                                                 w2_second_char,
                                                 TRUE);         
                           pt_cnt_data->ui2_scrl_begin_idx++;  
                           WGL_SET_FLAG(pt_cnt_data->ui2_overflow_flag,TEXT_OVERFLOW_RIGHT);
                       }                           
                       else
                       {
                           i4_remove_width = 0;
                           pt_cnt_data->ui2_scrl_len += 1;   
                           /*pt_cnt_data->ui2_scrl_begin_idx not change*/
                       }
                       pt_cnt_data->ui2_scrl_width += i4_append_width - i4_remove_width;
                       pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_left);
                       /*
                                   if (t_available_rect.i4_right < pt_cnt_data->ui2_scrl_width)
                                   {
                                       pt_cnt_data->ui2_scrl_start_x = t_available_rect.i4_left;
                                   }
                                   else
                                   {
                                       pt_cnt_data->ui2_scrl_start_x = t_available_rect.i4_right -  pt_cnt_data->ui2_scrl_width ;
                                   }   
                                  */

                   }
                   else /*text overflow right*/
                   { 
                       w2_first_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx);
                       
                       if (pt_cnt_data->ui2_scrl_begin_idx < pt_cnt_data->ui2_text_length - (UINT16)1)
                       {
                          w2_second_char = *(w2s_string - pt_cnt_data->ui2_scrl_begin_idx - 1) ;
                       }
                       i4_remove_width = _wgl_get_calculate_char_width(
                                             t_text_font,
                                             w2_first_char,
                                             w2_second_char,
                                             TRUE);
                       pt_cnt_data->ui2_scrl_width -= i4_remove_width;
                       for (;ui2_append_idx < pt_cnt_data->ui2_text_length; ui2_append_idx++)
                       {
                           w2_left_char = *(w2s_string - ui2_append_idx);  
                           i4_tmp_width = _wgl_get_calculate_char_width(
                                              t_text_font,
                                              w2_left_char,
                                              w2_right_char,                                              
                                              TRUE);
                            /*quite small that can't display one char*/
                           if (pt_cnt_data->ui2_scrl_width == 0)
                           {
                               i4_append_width += i4_tmp_width; 
                               i4_append_count++;
                               break;
                           }
                          
                           if (i4_append_width + i4_tmp_width > i4_box_width - pt_cnt_data->ui2_scrl_width)
                           {
                               break;  
                           }
                           else if (i4_append_width + i4_tmp_width == i4_box_width - pt_cnt_data->ui2_scrl_width)
                           {
                               i4_append_width += i4_tmp_width; 
                               i4_append_count++;
                               break;
                           }
                           else
                           {
                               i4_append_width += i4_tmp_width;  
                                i4_append_count++;
                           }
                          
                           w2_right_char = w2_left_char;                           
                       }
                       pt_cnt_data->ui2_scrl_begin_idx++;
                       pt_cnt_data->ui2_scrl_len += i4_append_count -1;
                       pt_cnt_data->ui2_scrl_width += i4_append_width;
                       pt_cnt_data->ui2_scrl_start_x = (UINT16)(t_available_rect.i4_right -  pt_cnt_data->ui2_scrl_width);
                      
                   }  /*text overflow left*/                     
               }                     
             
            }/*text read from right*/
           
        }/*not first frame of scrolling*/
    }

   
    return WGLR_OK;
}


static VOID _wgl_text_cnt_reset_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data)
{
     pt_cnt_data->ui2_scrl_begin_idx = 0;
     pt_cnt_data->ui2_scrl_len = 0;
     pt_cnt_data->ui2_scrl_width = 0;
     pt_cnt_data->ui2_scrl_start_x = 0;
     pt_cnt_data->ui2_overflow_flag = 0;
     if (WGL_TEXT_DIR_LEFT_TO_RIGHT == pt_cnt_data->ui1_read_direction)
     {   
         pt_cnt_data->ui2_overflow_flag |= TEXT_OVERFLOW_RIGHT;
                
     }
     else
     {
         pt_cnt_data->ui2_overflow_flag |= TEXT_OVERFLOW_LEFT;
     }    
}


static VOID _wgl_text_cnt_end_scrl(
    WGL_TEXT_CONTENT_T*                 pt_cnt_data)
{
  
     pt_cnt_data->ui2_scrl_style = 0;  
     pt_cnt_data->ui2_scrl_count = 0;
     pt_cnt_data->ui2_scrl_step = 0;
     _wgl_text_cnt_reset_scrl(pt_cnt_data);
     WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag,TEXT_SCROLLING); 
     pt_cnt_data->b_is_data_dirty = TRUE;
}

static INT32 _wgl_text_cnt_set_string(
    WGL_TEXT_CONTENT_T * pt_cnt_data,
    UTF16_T *            pt_string,
    UINT16               ui2_string_size)
{
    WGL_TEXT_MULTI_STRING* pt_string_element = NULL;
    WGL_TEXT_MULTI_STRING* pt_string_point   = NULL;

    pt_string_element = (WGL_TEXT_MULTI_STRING*)WGL_WIDGET_MEM_ALLOC((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING));
    if (pt_string_element == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_string_element, 0, sizeof(WGL_TEXT_MULTI_STRING));
    
    pt_string_element->ui2_size= ui2_string_size;
    
    if (pt_string_element->ui2_size == (UINT16)0)
    {
        pt_string_element->ps_string = NULL;
        pt_string_element->ui2_lines = 1;
    }
    else
    {
        pt_string_element->ps_string = (UTF16_T *)WGL_WIDGET_MEM_ALLOC((VOID*)pt_cnt_data,(pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
        if (pt_string_element->ps_string == NULL)
        {
            WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING),pt_string_element);
            return WGLR_OUT_OF_MEMORY;
        }
        x_memset(pt_string_element->ps_string, 0, sizeof(UTF16_T)*(pt_string_element->ui2_size + 1));
        x_memcpy(pt_string_element->ps_string, pt_string, pt_string_element->ui2_size * sizeof(UTF16_T));
    }
    if (pt_cnt_data->pt_multi_string == NULL)
    {
        pt_cnt_data->pt_multi_string = pt_string_element;
    }
    else
    {
        pt_string_point  =   pt_cnt_data->pt_multi_string;
        
        while(pt_string_point->pt_next != NULL)
        {
            pt_string_point = pt_string_point->pt_next;
        }

        pt_string_point->pt_next = pt_string_element;
    }
    pt_cnt_data->ui2_multi_string_num ++;
    
    return WGLR_OK;
}
static INT32 _wgl_text_cnt_set_string_prev(
    WGL_TEXT_CONTENT_T * pt_cnt_data,
    UTF16_T *            pt_string,
    UINT16               ui2_string_size)
{
    WGL_TEXT_MULTI_STRING* pt_string_element = NULL;
    WGL_TEXT_MULTI_STRING* pt_string_point   = NULL;

    pt_string_element = (WGL_TEXT_MULTI_STRING*)WGL_WIDGET_MEM_ALLOC((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING));
    if (pt_string_element == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_string_element, 0, sizeof(WGL_TEXT_MULTI_STRING));
    
    pt_string_element->ui2_size = ui2_string_size;
    
    if (pt_string_element->ui2_size == 0)
    {
        pt_string_element->ps_string = NULL;
        pt_string_element->ui2_lines = 1;
    }
    else
    {
        pt_string_element->ps_string = (UTF16_T *)WGL_WIDGET_MEM_ALLOC((VOID*)pt_cnt_data,(pt_string_element->ui2_size + 1) * sizeof(UTF16_T));
        if (pt_string_element->ps_string == NULL)
        {
            WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING),pt_string_element);
            return WGLR_OUT_OF_MEMORY;
        }
        x_memset(pt_string_element->ps_string, 0, sizeof(UTF16_T)*(pt_string_element->ui2_size + 1));
        x_memcpy(pt_string_element->ps_string, pt_string, pt_string_element->ui2_size * sizeof(UTF16_T));
    }
    
    pt_string_point  =   pt_cnt_data->pt_multi_string;
    pt_cnt_data->pt_multi_string = pt_string_element;
    pt_string_element->pt_next = pt_string_point;
    pt_cnt_data->ui2_multi_string_num ++;
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_remove_string_end(WGL_TEXT_CONTENT_T * pt_cnt_data)
{
    WGL_TEXT_MULTI_STRING* pt_string_point = NULL;
    WGL_TEXT_MULTI_STRING* pt_string_element = NULL;
    
    if (pt_cnt_data->pt_multi_string == NULL)
        
        return WGLR_OK;
    
    pt_string_point = pt_cnt_data->pt_multi_string;

    if (pt_string_point->pt_next == NULL)
    {
        pt_cnt_data->pt_multi_string = NULL;
        if (pt_string_point->ps_string != NULL)
        {
            WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,(pt_string_point->ui2_size + 1) * sizeof(UTF16_T),pt_string_point->ps_string);
            
        }
        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING),pt_string_point);
        return WGLR_OK;
    }
    else
    {
        pt_string_element = pt_string_point;
        while(pt_string_point->pt_next != NULL)
        {
            pt_string_element = pt_string_point;
            
            pt_string_point = pt_string_point->pt_next;
        }
        
        pt_string_element->pt_next = NULL;
        
        if (pt_string_point->ps_string != NULL)
        {
            WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,(pt_string_point->ui2_size + 1) * sizeof(UTF16_T),pt_string_point->ps_string);
        }
		if (pt_string_point->ps_arab_dsp_text != NULL)
		{
	        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,(pt_string_point->ui2_size + 1) * sizeof(UTF16_T),pt_string_point->ps_arab_dsp_text);
		}
		
        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING),pt_string_point);
		
    }
    pt_cnt_data->ui2_multi_string_num --;
    
    return WGLR_OK;
}
static INT32 _wgl_text_cnt_remove_string_prev(WGL_TEXT_CONTENT_T * pt_cnt_data)
{
    WGL_TEXT_MULTI_STRING* pt_string_point = NULL;
    
    if (pt_cnt_data->pt_multi_string == NULL)
        
        return WGLR_OK;
    
    pt_string_point = pt_cnt_data->pt_multi_string;

    if (pt_string_point->pt_next == NULL)
    {
        pt_cnt_data->pt_multi_string = NULL;     
    }
    else
    {
        pt_cnt_data->pt_multi_string = pt_string_point->pt_next;
        
    }
    
    if (pt_string_point->ps_string != NULL)
    {
        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,(pt_string_point->ui2_size + 1) * sizeof(UTF16_T),pt_string_point->ps_string);
    }
	if (pt_string_point->ps_arab_dsp_text != NULL)
	{
        WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,(pt_string_point->ui2_size + 1) * sizeof(UTF16_T),pt_string_point->ps_arab_dsp_text);
	}
    
    WGL_WIDGET_MEM_FREE((VOID*)pt_cnt_data,sizeof(WGL_TEXT_MULTI_STRING),pt_string_point);

     pt_cnt_data->ui2_multi_string_num --;
     
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_clear_string(WGL_TEXT_CONTENT_T * pt_cnt_data)
{  
    if (pt_cnt_data->pt_multi_string == NULL)
    {
        pt_cnt_data->ui2_multi_string_num = 0;
        return WGLR_OK;
    }
    while(pt_cnt_data->pt_multi_string != NULL)
    {
        _wgl_text_cnt_remove_string_prev( pt_cnt_data);
    }

    pt_cnt_data->ui2_multi_string_num = 0;
     
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_set_break_chars(
    WGL_TEXT_CONTENT_T * pt_cnt_data,
    UTF16_T *            pt_break_chars,
    UINT8                ui1_break_chars_len)
{
    if(ui1_break_chars_len > (TEXT_BREAK_CHARS_MAX_LENGHT-1))/*the last char is reserved to 0,the end of string*/
    {
        ui1_break_chars_len = TEXT_BREAK_CHARS_MAX_LENGHT-1;
    }
    x_memset(&pt_cnt_data->as_break_chars,(UINT8)0,TEXT_BREAK_CHARS_MAX_LENGHT*sizeof(UTF16_T));
    x_memcpy(&pt_cnt_data->as_break_chars,pt_break_chars,ui1_break_chars_len*sizeof(UTF16_T));
    return WGLR_OK;
}

static INT32 _wgl_text_cnt_get_point_size_from_height(WGL_TEXT_CONTENT_T* pt_cnt_data,
																UINT16	ui2_height,																
																UINT8*  pui1_point_size)
{
	return x_fe_get_size_from_height(pt_cnt_data->t_text_font, 
									ui2_height,
									pui1_point_size);
}

