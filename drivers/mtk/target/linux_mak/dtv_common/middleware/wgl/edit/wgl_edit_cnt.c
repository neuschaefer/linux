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
 * $RCSfile: wgl_edit_cnt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/8 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 14c5da33b8f768ed089ed9166b1585fa $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
#include "font/x_fe.h"
#include "unicode/x_uc_str.h"
#include "wgl/edit/wgl_edit_cnt.h"
#include "wgl/edit/wgl_edit_cli.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_eb_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*relayout type*/
#define WGL_CNT_EB_RLT_NO_HT            ((UINT8)0x01)   /*Not to show highlight index*/
#define WGL_CNT_EB_RLT_CHG_HT_OFFSET    ((UINT8)0x02)   /*the offset between start index and highligh index is changed*/
#define WGL_CNT_EB_RLT_CHG_AVAIL_RECT   ((UINT8)0x04)   /*the available rectange for display string is changed*/
#define WGL_CNT_EB_RLT_CHG_DIS_CNT      ((UINT8)0x08)   /*the content in display area is changed*/
#define WGL_CNT_EB_RLT_CHG_ALIGN        ((UINT8)0x10)   /*the alignment is changed*/

/*Extra Flag*/
#define EB_BK_CLR_NOT_INIT              ((UINT32)0x00000001)
#define EB_BK_IMG_NOT_INIT              ((UINT32)0x00000002)
#define EB_IMG_UI                       ((UINT32)0x00000004)
#define EB_FONT_NOT_INIT                ((UINT32)0x00000008)
#define EB_TEXT_CLR_NOT_INIT            ((UINT32)0x00000010)
#define EB_MAX_16                       ((UINT32)0x00000020)
#define EB_MAX_64                       ((UINT32)0x00000040)
#define EB_MAX_256                      ((UINT32)0x00000080)
#define EB_PWD_CHAR                     ((UINT32)0x00000100)
#define EB_IDR_WIDTH_NOT_INIT           ((UINT32)0x00000200)
#define EB_PWD_WIDTH_NOT_INIT           ((UINT32)0x00000400)
#define EB_NO_BK                        ((UINT32)0x00000800)
#define EB_CURSOR_HT                    ((UINT32)0x00001000)
#define EB_USE_SYS_IMG                  ((UINT32)0x00002000)
#define EB_NOT_CLEAN_BK                 ((UINT32)0x00004000)
#define EB_USER_CURSOR_CLR              ((UINT32)0x00008000)
#define EB_CHAR_IMG                     ((UINT32)0x00010000)
#define EB_PWD_BK_WIDTH_NOT_INIT        ((UINT32)0x00020000)
#define EB_PWD_CHAR_ALL                 ((UINT32)0x00040000)

#define _EB_GET_MAX_LENGTH(flag)                                                \
    ((UINT16)((EB_MAX_256 & (flag)) ? 256 : ((EB_MAX_64 & (flag)) ? 64 : 16 )))

#define _EB_UTF16_DEF_PWD_CHAR          ((UTF16_T)0x002A)
#define _EB_UTF16_DEF_INPUT_IDR_CHAR    ((UTF16_T)0x005F)
#define _EB_UTF16_NULL_CHAR             ((UTF16_T)0x0000)
                    

typedef struct _WGL_EB_CONTENT_T
{
#ifdef CLI_LVL_ALL     
    UINT32                              ui4_mem_size;                    /*this field must be the first field in the structure*/
#endif  
    WGL_COLOR_STD_SET_T                 t_clr_bk;               /*background color set*/
    WGL_IMG_STD_SET_T                   t_img_bk;               /*background template image*/
    WGL_COLOR_STD_SET_T                 t_clr_text;             /*text color set*/
    WGL_COLOR_STD_SET_T                 t_clr_cursor;           /*text color set*/
    GL_RECT_T                           t_cnt_rect;             /*content rect*/
    GL_RECT_T                           t_text_draw_rect;       /*rectangle to display string*/
    GL_RECT_T                           t_char_imgs_rect;       /*rectangle to display char imgaes*/
    GL_RECT_T                           t_password_bk_draw_rect; /*rectangle to display password background string*/
    WGL_INSET_T                         t_fg_inset;             /*inset for foreground icon and text*/
    WGL_FONT_INFO_T                     t_font_info;            /*font info for text*/
    HFONT_T                             t_text_font;            /*font for text*/
    WGL_FONT_INFO_T                     t_focus_font_info;      /*font information for text*/
    HFONT_T                             t_text_focus_font;      /*font for text*/
    INT32                               i4_text_y_offset;       /*offset to draw string to the surface*/
    UTF16_T                             a_w2_passwd_char[2];    /*password character*/
    UTF16_T                             a_w2_input_indicator[2];/*character for indicating the user to input*/
    UINT16                              ui2_text_dis_width;     /**/
    UINT16                              ui2_text_dis_start;     /*the first character to display. Zero-based*/
    UINT16                              ui2_text_dis_len;       /**/
    UTF16_T                             w2_passwd_bk_char;
    /*UINT16                            ui2_line_height;*/      /*height of a line*/
    UINT16                              ui2_text_length;        /*total length of user string*/
    UINT16                              ui2_highlight_index;    /*index to highlight*/
    UINT16                              ui2_highlight_offset;
    UINT16                              ui2_passwd_width;
    UINT16                              ui2_passwd_bk_width;
    UINT16                              ui2_indicator_width;
    UINT32                              ui4_extra_flag;         /*Flag*/
    UINT8                               ui1_align;              /*alignment*/
    UINT8                               ui1_relayout_type;
    WGL_EB_CHAR_IMG_T                   t_char_img;
    BOOL                                b_is_relayout;          /*TRUE: if the string needs to be relayout.*/
    WGL_CANVAS_STATUS_T                 t_cvsst;                /*canvas status*/
    UTF16_T*                            pt_w2_text;           /*the start pos of the string buffer*/
    UINT16                              ui2_length_limit;
} WGL_EB_CONTENT_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
* extern methods declarations
*---------------------------------------------------------------------------*/
extern INT32 x_gl_get_translation(
    GL_HGRAPHICS_T      h_graphics, 
    INT32*              pi4_trans_x, 
    INT32*              pi4_trans_y);
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_eb_cnt_paint_bk(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state);

static INT32 _wgl_eb_cnt_paint_fg(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr,
    const GL_COLOR_T*                   pt_cursor_clr);

static INT32 _wgl_eb_cnt_paint_char_bk_image(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect);

static INT32 _wgl_eb_cnt_paint_hlt_char_bk_image(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    GL_RECT_T*                          pt_char_rect);

static INT32 _wgl_eb_cnt_draw_password_bk(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr);

static INT32 _wgl_eb_cnt_draw_password(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr);

static INT32 _wgl_eb_cnt_draw_highlight(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_cursor_clr,
    GL_RECT_T*                          pt_draw_rect);

static INT32 _wgl_eb_cnt_get_indicator_width(
    WGL_EB_CONTENT_T*                   pt_data, 
    UINT16                              ui2_state,
    INT32*                              pi4_width);

static INT32 _wgl_eb_cnt_update_pwd_draw_rect(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_state);

static INT32 _wgl_eb_cnt_update_draw_rect(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_state);

static INT32 _wgl_eb_cnt_update_text_info(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_state,
    const GL_RECT_T*                    pt_avail_rect);

static INT32 _wgl_eb_cnt_update_display_start(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_state,
    INT32                               i4_available_width);

static INT32 _wgl_eb_cnt_update_highlight_offset(
    WGL_EB_CONTENT_T*                   pt_data,
    INT32                               i4_available_width);

static INT32 _wgl_eb_cnt_make_highlight_visible(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_state,
    INT32                               i4_available_width);

static INT32 _wgl_eb_cnt_set_img(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_IMG_INFO_T*               pt_img_info);

static INT32 _wgl_eb_cnt_get_img(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    WGL_IMG_INFO_T*                     pt_img_info);

static INT32 _wgl_eb_cnt_set_text(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_length,
    const UTF16_T*                      w2s_str);

static INT32 _wgl_eb_cnt_set_color(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_COLOR_INFO_T*             pt_clr_info);

static INT32 _wgl_eb_cnt_get_color(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_target,
    WGL_COLOR_INFO_T*                   pt_clr_info);

static INT32 _wgl_eb_cnt_set_font(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    const WGL_FONT_INFO_T*              pt_font_info);

static INT32 _wgl_eb_cnt_get_font(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    WGL_FONT_INFO_T*                    pt_font_info);

static INT32 _wgl_eb_cnt_set_font_ex(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_font_info);

static INT32 _wgl_eb_cnt_get_font_ex(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    WGL_FONT_INFO_EX_T*                 pt_font_info);


static INT32 _wgl_eb_cnt_prepare_img(
    WGL_EB_CONTENT_T*                   pt_cnt_data);

static INT32 _wgl_eb_cnt_use_sys_bk_color(
    WGL_EB_CONTENT_T*                   pt_cnt_data);

static INT32 _wgl_eb_cnt_use_sys_text_color(
    WGL_EB_CONTENT_T*                   pt_cnt_data);

static INT32 _wgl_eb_cnt_select_text_clr(
    WGL_EB_CONTENT_T*                   pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_selected_clr);

static INT32 _wgl_eb_cnt_select_text_font(
    WGL_EB_CONTENT_T*                   pt_cnt_data, 
    UINT16                              ui2_state,
    HANDLE_T*                           pt_selected_font,
    WGL_FONT_INFO_T*                    pt_selected_font_info);

static VOID _wgl_eb_cnt_make_cursor_clr(
    WGL_EB_CONTENT_T*                   pt_cnt_data);

static VOID _wgl_eb_cnt_get_invert_color(
    const WGL_CANVAS_STATUS_T*          pt_cvsst,
    const GL_COLOR_T*                   pt_clr_src,
    GL_COLOR_T*                         pt_clr_inverted);

static INT32 _wgl_eb_cnt_index_start(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index);

static INT32 _wgl_eb_cnt_index_highlight(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    BOOL                                b_highlight);

static INT32 _wgl_eb_cnt_index_replace(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    UTF16_T                             w2_new_char);

static INT32 _wgl_eb_cnt_index_remove(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_index,
    UINT16                              ui2_length);

static INT32 _wgl_eb_cnt_index_insert(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    UTF16_T                             w2_new_char);

static INT32 _wgl_eb_cnt_set_char_img(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    WGL_EB_CHAR_IMG_T*                  pt_char_img);

static INT32 _wgl_eb_cnt_get_char_img(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    WGL_EB_CHAR_IMG_T*                  pt_char_img);

static VOID _wgl_eb_cnt_update_relayout_flag(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT8                               ui1_relayout_type);

static VOID _wgl_eb_cnt_switch_highlight(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    BOOL                                b_tunr_on);

static INT32 _wgl_eb_cnt_get_max_passwd_in_box(
    INT32                               i4_passwd_width,
    INT32                               i4_max_length,
    INT32                               i4_box_width,
    INT32*                              pi4_passwd_numbers,
    INT32*                              pi4_occupy_width);

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
INT32 wgl_eb_cnt_inst_create( 
    WGL_CONTENT_CONTEXT_HANDLE_T*       ph_cnt_data, 
    WGL_CONTENT_CREATE_INPUT_T*         pt_param)
{
    /*local variable declaration*/
    WGL_EB_CONTENT_T*                   pt_data;
    UINT32                              ui4_flag;
    UINT16                              ui2_byte_num;

    DBG_API(("{WGL(EB_CNT)} %s: ph_cnt_data = 0x%.8x \tpt_param = 0x%.8x\n", __FUNCTION__, ph_cnt_data, pt_param));
    
    /*check parameter*/
    if(NULL == ph_cnt_data || NULL == pt_param)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(EB_CNT)} pt_param->pv_init = 0x%.8x   ui4_type_border = 0x%.8x\n"
                             "\tt_rect_cnt(l, t, r, b) = (%d, %d, %d, %d)\n",
                             pt_param->pv_init,
                             pt_param->t_rect_cnt.i4_left,
                             pt_param->t_rect_cnt.i4_top,
                             pt_param->t_rect_cnt.i4_right,
                             pt_param->t_rect_cnt.i4_bottom));

    /*set length*/
    ui2_byte_num = (UINT16)sizeof(UTF16_T);
    if(WGL_STL_EB_MAX_CHAR_256 & ((UINT32)(pt_param->pv_init)))
    {
        ui2_byte_num *= _EB_GET_MAX_LENGTH(EB_MAX_256);
        ui4_flag = EB_MAX_256;
    }
    else if(WGL_STL_EB_MAX_CHAR_64 & ((UINT32)(pt_param->pv_init)))
    {
        ui2_byte_num *= _EB_GET_MAX_LENGTH(EB_MAX_64);
        ui4_flag = EB_MAX_64;
    }
    else
    {
        ui2_byte_num = _EB_GET_MAX_LENGTH(EB_MAX_16);
        ui4_flag = EB_MAX_16;
    }

    /*allocate memory for content data*/
    pt_data = (WGL_EB_CONTENT_T*) WGL_MEM_ALLOC( sizeof(WGL_EB_CONTENT_T) + ui2_byte_num * sizeof(UTF16_T) );
    if(NULL == pt_data)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to alloc memory for content instance. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    /*initialize content data*/
    x_memset(pt_data, 0, sizeof(WGL_EB_CONTENT_T) + ui2_byte_num * sizeof(UTF16_T));
#ifdef CLI_LVL_ALL
     pt_data->ui4_mem_size = sizeof(WGL_EB_CONTENT_T) + ui2_byte_num * sizeof(UTF16_T); 
#endif
    pt_data->pt_w2_text = (UTF16_T*)((UINT32)(pt_data) + (UINT32)(WGL_ALIGN_4(sizeof(WGL_EB_CONTENT_T))));
    pt_data->ui4_extra_flag = ui4_flag;
    pt_data->ui4_extra_flag |= EB_BK_CLR_NOT_INIT 
                            | EB_BK_IMG_NOT_INIT 
                            | EB_FONT_NOT_INIT 
                            | EB_TEXT_CLR_NOT_INIT 
                            | EB_IDR_WIDTH_NOT_INIT 
                            | EB_PWD_WIDTH_NOT_INIT;
    pt_data->ui1_align = WGL_AS_LEFT_TOP;
    pt_data->ui1_relayout_type = WGL_CNT_EB_RLT_NO_HT;
    pt_data->b_is_relayout = TRUE;
    wgl_cvsst_copy(&pt_data->t_cvsst, pt_param->pt_cvsst);
    pt_data->a_w2_passwd_char[0] = _EB_UTF16_DEF_PWD_CHAR;
    pt_data->a_w2_input_indicator[0] = _EB_UTF16_DEF_INPUT_IDR_CHAR;
    pt_data->w2_passwd_bk_char= _EB_UTF16_NULL_CHAR;
    pt_data->ui2_passwd_bk_width = 0;    
    pt_data->ui2_length_limit = ui2_byte_num;
    /*store settings*/
    WGL_RECT_COPY(&pt_data->t_cnt_rect, &pt_param->t_rect_cnt);
    pt_data->ui4_extra_flag |= (WGL_IS_IMAGE_UI((UINT32)(pt_param->pv_init))) ? EB_IMG_UI : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_EB_PASSWORD & ((UINT32)pt_param->pv_init)) ? EB_PWD_CHAR_ALL : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_EB_PASSWORD_ESC_LAST & ((UINT32)pt_param->pv_init)) ? EB_PWD_CHAR : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_GL_NO_BK & ((UINT32)pt_param->pv_init)) ? EB_NO_BK : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_GL_USE_SYS_IMG & ((UINT32)pt_param->pv_init)) ? EB_USE_SYS_IMG : 0;    
    pt_data->ui4_extra_flag |= (WGL_STL_GL_BDR_FILL_CNT_BK & ((UINT32)pt_param->pv_init)) ? EB_NOT_CLEAN_BK : 0;
    pt_data->ui4_extra_flag |= (WGL_STL_EB_CHAR_WITH_IMG & ((UINT32)pt_param->pv_init)) ? EB_CHAR_IMG : 0;
    
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
INT32 wgl_eb_cnt_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data)
{
    WGL_EB_CONTENT_T*                   pt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(EB_CNT)} %s: h_cnt_data = 0x%.8x\n", __FUNCTION__, h_cnt_data));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    pt_data = (WGL_EB_CONTENT_T*)h_cnt_data;    

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
INT32 wgl_eb_cnt_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    GL_COLOR_T                          t_text_clr, t_cursor_clr;
    WGL_FONT_INFO_T                     t_font_info;
    WGL_EB_CONTENT_T*                   pt_cnt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(EB_CNT)} %s: h_cnt_data = 0x%.8x \th_graphics = 0x%.8x \tui2_state = %d\n", __FUNCTION__, h_cnt_data, h_graphics, ui2_state));

    /*check parameter*/
    if(NULL == h_cnt_data || NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_cnt_data = (WGL_EB_CONTENT_T*)h_cnt_data;

    /*init text & cursor color*/
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_text_clr);
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_cursor_clr);

    /*paint background*/
    i4_ret = _wgl_eb_cnt_paint_bk(pt_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*select text color*/
    i4_ret = _wgl_eb_cnt_select_text_clr(pt_cnt_data, ui2_state, &t_text_clr);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*get highlight color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        t_cursor_clr = pt_cnt_data->t_clr_cursor.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        t_cursor_clr = pt_cnt_data->t_clr_cursor.t_enable;
    }
    else
    {
        t_cursor_clr = pt_cnt_data->t_clr_cursor.t_disable;
    }

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_eb_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*paint foreground*/
    return _wgl_eb_cnt_paint_fg(pt_cnt_data, 
                                h_graphics, 
                                ui4_upt_rect, 
                                pt_upt_rect, 
                                ui2_state, 
                                &t_text_clr,
                                &t_cursor_clr);   
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
INT32 wgl_eb_cnt_command_proc(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data,
    UINT32                              ui4_cmd_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2)
{   
    /*local variable declaration*/
    WGL_EB_CONTENT_T*                   pt_edit;

    DBG_API(("{WGL(EB_CNT)} %s: h_cnt_data = 0x%.8x \tui4_cmd_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
                                __FUNCTION__, 
                                h_cnt_data, 
                                ui4_cmd_id,
                                pv_param1,
                                pv_param2));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. The 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if(!(IS_EB_CNT_CMD(ui4_cmd_id) || WGL_IS_CMD_GENERAL(ui4_cmd_id)))
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    pt_edit = (WGL_EB_CONTENT_T*)h_cnt_data;

    switch (ui4_cmd_id)
    {
    case WGL_CMD_EB_SET_INPUT_LIMIT:
        {
            UINT16 length = WGL_POINTER_TO_UINT16(pv_param1);
            if (length != 0)
            {
                pt_edit->ui2_length_limit = MIN(length, _EB_GET_MAX_LENGTH(pt_edit->ui4_extra_flag));
            }
        }
        return WGLR_OK;
    case WGL_CMD_EB_GET_INPUT_LIMIT:
        {
            if (pv_param1 == NULL)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_INPUT_LIMIT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            *((UINT16*)pv_param1) = pt_edit->ui2_length_limit;
        }
        return WGLR_OK;
    case WGL_CMD_GL_SET_IMAGE:
        return _wgl_eb_cnt_set_img(pt_edit, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_IMAGE:
        return _wgl_eb_cnt_get_img(pt_edit, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_COLOR:
        return _wgl_eb_cnt_set_color(pt_edit, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_COLOR:
        return _wgl_eb_cnt_get_color(pt_edit, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_FONT:
        return _wgl_eb_cnt_set_font(pt_edit, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT:
        return _wgl_eb_cnt_get_font(pt_edit, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_SET_FONT_EX:
        return _wgl_eb_cnt_set_font_ex(pt_edit, (WGL_FONT_INFO_EX_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT_EX:
        return _wgl_eb_cnt_get_font_ex(pt_edit, (WGL_FONT_INFO_EX_T*)pv_param1);


    case WGL_CMD_EB_SET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(EB_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                    pv_param1,
                    ((WGL_INSET_T*)pv_param1)->i4_left,
                    ((WGL_INSET_T*)pv_param1)->i4_top,
                    ((WGL_INSET_T*)pv_param1)->i4_right,
                    ((WGL_INSET_T*)pv_param1)->i4_bottom));
            
            if(FALSE == wgl_is_legal_inset(&pt_edit->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_edit->t_fg_inset, (WGL_INSET_T*)pv_param1);

            /*turn on dirty-flag*/
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_AVAIL_RECT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);
        }
        break;

    case WGL_CMD_EB_GET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_edit->t_fg_inset);
        }
        break;

    case WGL_CMD_EB_SET_ALIGN:
        {
            if(FALSE == wgl_is_legal_alignment(WGL_POINTER_TO_UINT8(pv_param1)))
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_SET_ALIGN, the alignment specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            pt_edit->ui1_align = WGL_POINTER_TO_UINT8(pv_param1);

            /*update dirty flag*/
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_ALIGN);
        }
        break;

    case WGL_CMD_EB_GET_ALIGN:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_ALIGN, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT8*)pv_param1) = pt_edit->ui1_align;
        }
        break;

    case WGL_CMD_EB_SET_PWD_CHAR:
        {
            pt_edit->a_w2_passwd_char[0] = WGL_POINTER_TO_UTF16_T(pv_param1);

            /*update dirty flag*/
            WGL_SET_FLAG(pt_edit->ui4_extra_flag, EB_PWD_WIDTH_NOT_INIT);
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);
        }
        break;

    case WGL_CMD_EB_GET_PWD_CHAR:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_PWD_CHAR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            *((UTF16_T*)pv_param1) = pt_edit->a_w2_passwd_char[0];
        }
        break;
    case WGL_CMD_EB_SET_PWD_BK_CHAR:
       {
           pt_edit->w2_passwd_bk_char = WGL_POINTER_TO_UTF16_T(pv_param1);
           /*update dirty flag*/
            WGL_SET_FLAG(pt_edit->ui4_extra_flag, EB_PWD_BK_WIDTH_NOT_INIT);
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);

       }
       break;
    
   case WGL_CMD_EB_GET_PWD_BK_CHAR:
       {
           if(NULL == pv_param1)
           {
               DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_PWD_CHAR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
               return WGLR_INV_ARG;
           }
           
           *((UTF16_T*)pv_param1) = pt_edit->w2_passwd_bk_char;
       }
       break;

    case WGL_CNT_EB_SET_TEXT:
        return _wgl_eb_cnt_set_text(pt_edit, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1);

    case WGL_CNT_EB_GET_TEXT:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CNT_EB_GET_TEXT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            /*check length*/
            if(pt_edit->ui2_text_length > WGL_POINTER_TO_UINT16(pv_param2) )
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: The input buffer isn't enough. %d-character is necessary. (%d) at L%d\n\n", pt_edit->ui2_text_length, WGLR_BUFFER_NOT_ENOUGH, __LINE__));
                return WGLR_BUFFER_NOT_ENOUGH;
            }
            
            /*copy string*/
            x_memcpy(pv_param1, pt_edit->pt_w2_text, pt_edit->ui2_text_length * sizeof(UTF16_T));
        }
        break;

    case WGL_CNT_EB_INDEX_START:
        return _wgl_eb_cnt_index_start(pt_edit, WGL_POINTER_TO_UINT16(pv_param1));

    case WGL_CNT_EB_INDEX_HIGHLIGHT:
        return _wgl_eb_cnt_index_highlight(pt_edit, WGL_POINTER_TO_UINT16(pv_param1), WGL_POINTER_TO_BOOL(pv_param2));

    case WGL_CNT_EB_INDEX_REPLACE:
        return _wgl_eb_cnt_index_replace(pt_edit, WGL_POINTER_TO_UINT16(pv_param1), WGL_POINTER_TO_UTF16_T(pv_param2));

    case WGL_CNT_EB_INDEX_REMOVE:
        return _wgl_eb_cnt_index_remove(pt_edit, WGL_POINTER_TO_UINT16(pv_param1), WGL_POINTER_TO_UINT16(pv_param2));

    case WGL_CNT_EB_INDEX_INSERT:
        return _wgl_eb_cnt_index_insert(pt_edit, WGL_POINTER_TO_UINT16(pv_param1), WGL_POINTER_TO_UTF16_T(pv_param2));
            
    case WGL_CNT_EB_SET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CNT_EB_SET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(EB_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((GL_RECT_T*)pv_param1)->i4_left,
                        ((GL_RECT_T*)pv_param1)->i4_top,
                        ((GL_RECT_T*)pv_param1)->i4_right,
                        ((GL_RECT_T*)pv_param1)->i4_bottom));

            /*copy rect*/
            WGL_RECT_COPY(&pt_edit->t_cnt_rect, (GL_RECT_T*)pv_param1);
            
            /*update dirty flag*/
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_AVAIL_RECT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);
        }
        break;

    case WGL_CNT_EB_GET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CNT_EB_GET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*prepare return value*/
            WGL_RECT_COPY((GL_RECT_T*)pv_param1, &pt_edit->t_cnt_rect);
        }
        break;
        
    case WGL_CMD_EB_ENABLE_CURSOR_HT:
        {
            /**/
            if(WGL_POINTER_TO_BOOL(pv_param1))
            {
                WGL_SET_FLAG(pt_edit->ui4_extra_flag, EB_CURSOR_HT);
            }
            else
            {
                WGL_CLEAR_FLAG(pt_edit->ui4_extra_flag, EB_CURSOR_HT);
            }       
        }
        break;

    case WGL_CMD_EB_SET_CURSOR_CHAR:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_SET_CURSOR_CHAR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            pt_edit->a_w2_input_indicator[0] = *((UTF16_T*)pv_param1);
            WGL_SET_FLAG(pt_edit->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT);
        }
        break;

    case WGL_CMD_EB_GET_CURSOR_CHAR:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_EB_GET_CURSOR_CHAR, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UTF16_T*)pv_param1) = pt_edit->a_w2_input_indicator[0];
        }
        break;

    case WGL_CMD_EB_SET_CHAR_IMG:
        {
            return _wgl_eb_cnt_set_char_img(pt_edit, (WGL_EB_CHAR_IMG_T*)pv_param1);
        }

    case WGL_CMD_EB_GET_CHAR_IMG:
        {
            return _wgl_eb_cnt_get_char_img(pt_edit, (WGL_EB_CHAR_IMG_T*)pv_param1);
        }
    
    case WGL_CNT_EB_UPDATE_CVSST:
        return wgl_update_cvvst((WGL_CVS_CLR_CFG_T*)pv_param1, &pt_edit->t_cvsst);

    case WGL_CNT_EB_STATE_CHANE:
        {
            /*turn on dirty-flag*/
            _wgl_eb_cnt_update_relayout_flag(pt_edit, WGL_CNT_EB_RLT_CHG_AVAIL_RECT | WGL_CNT_EB_RLT_CHG_DIS_CNT);
        }
        break;
    default:
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
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
static INT32 _wgl_eb_cnt_paint_bk(
    WGL_EB_CONTENT_T*                   pt_data, 
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
    if(EB_NO_BK & pt_data->ui4_extra_flag)
    {
        return WGLR_OK;
    }
    if(!(EB_IMG_UI & pt_data->ui4_extra_flag))
    {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_eb_cnt_use_sys_bk_color(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT);
        }
        
        /*select image*/
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            t_clr = pt_data->t_clr_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            t_clr = pt_data->t_clr_bk.t_enable;
        }
        else
        {
            t_clr = pt_data->t_clr_bk.t_disable;
        }

        /*should check EB_NOT_CLEAN_BK style here*/
        
        /*paint background*/
        i4_ret = wgl_fill_widget_bk(h_graphics, &t_clr, &pt_data->t_cnt_rect, &pt_data->t_cvsst);
        WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to fill background color. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    else
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_eb_cnt_prepare_img(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT);
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
        
        if(NULL_HANDLE != h_image)
        {
            /*paint background*/
            i4_ret = wgl_img_tpl_paint(h_image, 
                                       &pt_data->t_cnt_rect, 
                                       pt_upt_rect, 
                                       h_graphics, 
                                       &pt_data->t_cvsst,
                                       (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_NOT_CLEAN_BK)) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT);
            WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw image UI to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
static INT32 _wgl_eb_cnt_paint_fg(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr,
    const GL_COLOR_T*                   pt_cursor_clr)
{
    /*local variable declaration*/
    GL_COLOR_T                          t_clean_clr;
    GL_RECT_T                           t_draw_rect;
    UTF16_T*                            w2s_start;
    INT32                               i4_width, i4_height;
    INT32                               i4_ret;
    HANDLE_T                            t_text_font; 
    WGL_FONT_INFO_T                     t_font_info;

    /*paint chars' background images*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG))
    {
        i4_ret = _wgl_eb_cnt_paint_char_bk_image(
                                                pt_data, 
                                                h_graphics, 
                                                ui4_upt_rect,
                                                pt_upt_rect);
        if (WGLR_OK != i4_ret)  
        {     
            return i4_ret;   
        }
    }

    /*init clean color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_clean_clr);

    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*update password character width*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_PWD_WIDTH_NOT_INIT))
    {   
        /*get indicator width*/
        i4_ret = x_fe_get_string_size(t_text_font, pt_data->a_w2_passwd_char, -1, &i4_width, &i4_height);
        if(FER_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get password character's size. (FER=%d) at L%d\n\n", i4_ret, __LINE__));
            return WGLR_GET_FONT_INFO_FAIL;
        }

        WGL_ASSERT(i4_width <= 0x0000FFFF);
        pt_data->ui2_passwd_width = (UINT16)i4_width;

        /*turn off password width dirty flag*/
        WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_PWD_WIDTH_NOT_INIT);
    }
    /*update password bk character width,this valus is affected by password bk character,font info change*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_PWD_BK_WIDTH_NOT_INIT))
    {   
       /*get indicator width*/
       i4_ret = x_fe_get_character_size(t_text_font, pt_data->w2_passwd_bk_char, &i4_width, &i4_height);
       if(FER_OK != i4_ret)
       {
           DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get password bk character's size. (FER=%d) at L%d\n\n", i4_ret, __LINE__));
           return WGLR_GET_FONT_INFO_FAIL;
       }

       WGL_ASSERT(i4_width <= 0x0000FFFF);
       pt_data->ui2_passwd_bk_width = (UINT16)i4_width;

       /*turn off password width dirty flag*/
      WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_PWD_BK_WIDTH_NOT_INIT);
    }

    
    /*update layout info*/
    if(pt_data->b_is_relayout)
    {
        /*update rectangle: pt_data->t_text_draw_rect and pt_data->t_password_bk_draw_rect*/
        if((EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag) && pt_data->w2_passwd_bk_char != _EB_UTF16_NULL_CHAR )
        {
            _wgl_eb_cnt_update_pwd_draw_rect(pt_data,ui2_state);
        }
        else
        {
            /*update text draw rectangle: pt_data->t_text_draw_rect*/
            i4_ret = _wgl_eb_cnt_update_draw_rect(pt_data,ui2_state);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        /*turn off dirty flag*/
        pt_data->b_is_relayout = FALSE;
    }
    
    /*paint string*/
    if(!WGL_RECT_IS_EMPTY(&pt_data->t_text_draw_rect))
    {
        /*draw string*/
        if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
        {
            /*draw password string*/
            i4_ret = _wgl_eb_cnt_draw_password(pt_data, h_graphics, ui2_state, pt_text_clr);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        else 
        {
            /*draw input string*/
            w2s_start = x_uc_w2s_get_ptr(pt_data->pt_w2_text, pt_data->ui2_text_dis_start);                                             
            
            if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
            {
                i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                              pt_data->t_text_draw_rect.i4_left,
                                              pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                              0,
                                              w2s_start,
                                              pt_data->ui2_text_dis_len,
                                              NULL,
                                              0,
                                              t_text_font,
                                              pt_text_clr,
                                              &t_clean_clr,
                                              &t_clean_clr,
                                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                              (UINT16)t_font_info.i2_width,
                                              0,
                                              0,
                                              0);                                                
                if(GLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw string to text surface. (GLR= %d) at L%d\n\n", i4_ret, __LINE__));
                    return WGLR_TEXT_DRAW_FAIL;
                }
            }
        }
    }
    else if(EB_PWD_CHAR & pt_data->ui4_extra_flag && pt_data->w2_passwd_bk_char != _EB_UTF16_NULL_CHAR )
    {
        /*draw password background display string,pt_data->t_password_bk_draw_rect is not empty*/
        i4_ret = _wgl_eb_cnt_draw_password(pt_data, h_graphics, ui2_state, pt_text_clr);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*init draw rectangle*/
    WGL_RECT_COPY(&t_draw_rect, &pt_data->t_text_draw_rect);    

    /*draw highlight*/
    if( !(WGL_CNT_EB_RLT_NO_HT & pt_data->ui1_relayout_type) )
    {
        i4_ret = _wgl_eb_cnt_draw_highlight(pt_data, h_graphics, ui2_state, pt_cursor_clr, &t_draw_rect);
        if(WGLR_OK != i4_ret)
        {
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
static INT32 _wgl_eb_cnt_paint_char_bk_image(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect)
{
    INT32                               i4_ret;
    UINT32                              ui4_img_num = 0;
    UINT32                              ui4_img_width = 0;
    UINT32                              ui4_img_height = 0;
    UINT32                              ui4_count, ui4_img_total_width, ui4_gap;
    WGL_HIMG_TPL_T                      h_image = NULL_HANDLE;
    GL_RECT_T                           t_char_img_rect, t_available_rect;
    INT32                               i4_available_width;
    INT32                               i4_available_height;
    INT32                               i4_x, i4_y;
    
    /*check for quick return*/
    ui4_gap = pt_data->t_char_img.ui4_gap;
    h_image = pt_data->t_char_img.h_normal;
    ui4_img_num = pt_data->t_char_img.ui4_img_num;
    if ((h_image == NULL_HANDLE) || (ui4_img_num == 0))
    {
        return WGLR_OK;
    }
    
    /*get image size*/
    i4_ret = x_wgl_img_tpl_get_size(h_image, &ui4_img_width, &ui4_img_height);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    ui4_img_total_width = ui4_img_width * ui4_img_num + (ui4_img_num - 1) * ui4_gap;;

    /*initialize available rectangle for diaplay*/
    WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
    WGL_RECT_INSET_W_CHECK(&t_available_rect, 
                   pt_data->t_fg_inset.i4_left, 
                   pt_data->t_fg_inset.i4_top,
                   pt_data->t_fg_inset.i4_right, 
                   pt_data->t_fg_inset.i4_bottom);

    i4_available_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    i4_available_width  = WGL_RECT_GET_WIDTH(&t_available_rect);

    /*calculate start position in x-axis*/
    if(WGL_IS_H_LEFT(pt_data->ui1_align))
    {
        t_char_img_rect.i4_left = (INT32)ui4_gap >> 1;
    }
    else if(WGL_IS_H_CENTER(pt_data->ui1_align))
    {
        t_char_img_rect.i4_left = (i4_available_width - (INT32)ui4_img_total_width) >> 1;
    }
    else
    {
        t_char_img_rect.i4_left = i4_available_width - (INT32)ui4_img_total_width - (INT32)(ui4_gap >> 1);
    }
    
    /*calculate start position in y-axis*/
    if(WGL_IS_V_TOP(pt_data->ui1_align))
    {
        t_char_img_rect.i4_top = 0;
    }
    else if(WGL_IS_V_CENTER(pt_data->ui1_align))
    {
        t_char_img_rect.i4_top = (i4_available_height - (INT32)ui4_img_height) >> 1;
    }
    else
    {
        t_char_img_rect.i4_top = i4_available_height - ui4_img_height;
    }
    t_char_img_rect.i4_left += t_available_rect.i4_left;
    t_char_img_rect.i4_top  += t_available_rect.i4_top;
    /*maybe out of screen*/
    i4_ret = x_gl_get_translation(h_graphics, &i4_x, &i4_y);
    if (i4_ret != GLR_OK)
    {
        return i4_ret;
    }
    if ((t_char_img_rect.i4_left + i4_x) < 0)
    {
        t_char_img_rect.i4_left = 0 - i4_x + (INT32)(ui4_gap >> 1);
    }
    if ((t_char_img_rect.i4_top + i4_y) < 0)
    {
        t_char_img_rect.i4_top = 0 - i4_y + (INT32)(ui4_gap >> 1);
    }
    
    t_char_img_rect.i4_bottom = t_char_img_rect.i4_top + ui4_img_height;

    pt_data->t_char_imgs_rect.i4_left   = t_char_img_rect.i4_left;
    pt_data->t_char_imgs_rect.i4_top    = t_char_img_rect.i4_top;
    pt_data->t_char_imgs_rect.i4_right  = pt_data->t_char_imgs_rect.i4_left + ui4_img_total_width;
    pt_data->t_char_imgs_rect.i4_bottom = t_char_img_rect.i4_bottom;
    
    for (ui4_count = 0; ui4_count < ui4_img_num; ui4_count++)
    {
        /*set draw width and height*/
        t_char_img_rect.i4_right = t_char_img_rect.i4_left + ui4_img_width;
        if (t_char_img_rect.i4_right <= 0)
        {
            t_char_img_rect.i4_left = t_char_img_rect.i4_right + ui4_gap;
            continue;
        }
        /*paint image*/
        i4_ret = wgl_img_tpl_paint(h_image, 
                                   &t_char_img_rect, 
                                   pt_upt_rect,
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);

        if (WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        t_char_img_rect.i4_left = t_char_img_rect.i4_right + ui4_gap;

        if (t_char_img_rect.i4_left >= t_available_rect.i4_right)
        {
            break;
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
static INT32 _wgl_eb_cnt_draw_password_bk(
    WGL_EB_CONTENT_T*                   pt_data,
    GL_HGRAPHICS_T                      h_graphics, 
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr)
{
    GL_COLOR_T  t_clean_clr;
    INT32       i4_start = 0;
    UINT16      ui2_i = 0;
    UTF16_T    w2s_start[1];
    INT32       i4_ret;
    HANDLE_T                  t_text_font; 
    WGL_FONT_INFO_T           t_font_info;
    
    /*init clean color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_clean_clr);
    /*init variable*/
    w2s_start[0] = pt_data->w2_passwd_bk_char;
    
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    
    /*draw password bk character*/
    if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
    {
        for(ui2_i=0;ui2_i<(pt_data->ui2_length_limit - pt_data->ui2_text_dis_len);ui2_i++)
        {
               /*displayed character must be within t_draw_rect region*/
               if(((pt_data->t_password_bk_draw_rect.i4_left + i4_start+pt_data->ui2_passwd_bk_width)<=pt_data->t_password_bk_draw_rect.i4_right &&
               (pt_data->t_password_bk_draw_rect.i4_left + i4_start) >= pt_data->t_password_bk_draw_rect.i4_left))
               {
                    i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                                  pt_data->t_password_bk_draw_rect.i4_left + i4_start,
                                                  pt_data->t_password_bk_draw_rect.i4_top+pt_data->i4_text_y_offset,
                                                  0,
                                                  w2s_start,
                                                  1,
                                                  NULL,
                                                  0,
                                                  t_text_font,
                                                  pt_text_clr,
                                                  &t_clean_clr,
                                                  &t_clean_clr,
                                                  (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                                  (UINT16)t_font_info.i2_width,
                                                  0,
                                                  0,
                                                  0); 
                    if(GLR_OK != i4_ret)
                    {
                        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw password character. (GLR=%d) at L%d\n\n", i4_ret, __LINE__));
                        return WGLR_TEXT_DRAW_FAIL;
                    }    
                }
            i4_start += pt_data->ui2_passwd_bk_width;
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
static INT32 _wgl_eb_cnt_draw_password(
    WGL_EB_CONTENT_T*                   pt_data,
    GL_HGRAPHICS_T                      h_graphics, 
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr)
{
    GL_COLOR_T  t_clean_clr;
    INT32       i4_start = 0;
    UINT16      ui2_i;
    UTF16_T*    w2s_start;
    INT32       i4_ret;
    HANDLE_T                  t_text_font; 
    WGL_FONT_INFO_T           t_font_info;
    
    /*init clean color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_clean_clr);
    
    if(pt_data->w2_passwd_bk_char != _EB_UTF16_NULL_CHAR)
    {
        _wgl_eb_cnt_draw_password_bk(pt_data,h_graphics,ui2_state,pt_text_clr);
    }
    
    /*init variable*/
    w2s_start = pt_data->a_w2_passwd_char;
    
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    
    /*draw password character*/
    if (IS_STATE_FOCUSED(ui2_state) && !(EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag))
    {
        for(ui2_i = 0; ui2_i < pt_data->ui2_text_dis_len - (UINT16)1; ui2_i++)
        {
            if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
            {
                i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                              pt_data->t_text_draw_rect.i4_left + i4_start,
                                              pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                              0,
                                              w2s_start,
                                              1,
                                              NULL,
                                              0,
                                              t_text_font,
                                              pt_text_clr,
                                              &t_clean_clr,
                                              &t_clean_clr,
                                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                              (UINT16)t_font_info.i2_width,
                                              0,
                                              0,
                                              0);                                           
                if(GLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw password character. (GLR=%d) at L%d\n\n", i4_ret, __LINE__));
                    return WGLR_TEXT_DRAW_FAIL;
                }
            }
            
            i4_start += pt_data->ui2_passwd_width;
        }
        if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          pt_data->t_text_draw_rect.i4_left + i4_start,
                                          pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                          0,
                                          &(pt_data->pt_w2_text[pt_data->ui2_text_dis_len - (UINT16)1]),
                                          1,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_text_clr,
                                          &t_clean_clr,
                                          &t_clean_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          0,
                                          0);                                           
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw password character. (GLR=%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    }
    else
    {
    for(ui2_i = 0; ui2_i < pt_data->ui2_text_dis_len; ui2_i++)
    {
        if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          pt_data->t_text_draw_rect.i4_left + i4_start,
                                          pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                          0,
                                          w2s_start,
                                          1,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_text_clr,
                                          &t_clean_clr,
                                          &t_clean_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          0,
                                          0);                                           
            if(GLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw password character. (GLR=%d) at L%d\n\n", i4_ret, __LINE__));
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
        
        i4_start += pt_data->ui2_passwd_width;
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
static INT32 _wgl_eb_cnt_draw_highlight(
    WGL_EB_CONTENT_T*                   pt_data,
    GL_HGRAPHICS_T                      h_graphics, 
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_cursor_clr,
    GL_RECT_T*                          pt_draw_rect)
{
    /*local variable declaration*/
    GL_COLOR_T                          t_clean_clr;
    UTF16_T*                            w2s_start;
    INT32                               i4_x;
    INT32                               i4_ret;
    UINT16                              ui2_ht_index;
    HANDLE_T                            t_text_font; 
    WGL_FONT_INFO_T                     t_font_info;
    INT32                               i4_char_x, i4_char_y;
    INT16                               i2_font_height;
    GL_RECT_T                           t_char_rect;
    
    if(FALSE == WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CURSOR_HT))
    {
        return WGLR_OK;
    }

    /*init clean color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_clean_clr);
    
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*calculate highlight rectangle*/
    if(pt_data->ui2_highlight_index < pt_data->ui2_text_length)
    {
        if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
        {
            w2s_start = pt_data->a_w2_passwd_char;
        }
        else
        {
            w2s_start = NULL;
        }
        /*set highlight offset*/
        i4_x = (INT32)pt_data->ui2_highlight_offset;
    }
    else
    {
        w2s_start = pt_data->a_w2_input_indicator;
        
        /*set highlight offset*/
        i4_x = (INT32)pt_data->ui2_text_dis_width;
        pt_draw_rect->i4_right += pt_data->ui2_indicator_width;
    }
    
    /*paint hlt char's background image*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG) && (pt_data->t_char_img.ui4_img_num != 0))
    {
        i4_ret = x_fe_get_height(t_text_font, &i2_font_height);
        if (FER_OK != i4_ret)
        {
            return i4_ret;
        }
        if (pt_data->ui2_text_dis_len == 0)
        {
            ui2_ht_index = 0;
        }
        else
        {
            ui2_ht_index = pt_data->ui2_highlight_index - pt_data->ui2_text_dis_start;
        }
        
        i4_char_x = pt_data->t_text_draw_rect.i4_left + ui2_ht_index * (t_font_info.i2_width);
        i4_char_y = pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset;
        t_char_rect.i4_left   = i4_char_x;
        t_char_rect.i4_top    = i4_char_y;
        t_char_rect.i4_right  = t_char_rect.i4_left + t_font_info.i2_width;
        t_char_rect.i4_bottom = t_char_rect.i4_top + (INT32)i2_font_height;
        
        i4_ret = _wgl_eb_cnt_paint_hlt_char_bk_image(
                                                pt_data, 
                                                h_graphics, 
                                                &t_char_rect);
        if (WGLR_OK != i4_ret)  
        {     
            return i4_ret;   
        }
    }
    
    /*draw highlight index*/
    if(!wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_cursor_clr))
    {
        if(w2s_start && !(pt_data->ui4_extra_flag & EB_PWD_CHAR_ALL ) && pt_data->ui2_highlight_index == (pt_data->ui2_text_dis_len - 1) && (IS_STATE_FOCUSED(ui2_state)))
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          pt_data->t_text_draw_rect.i4_left + i4_x,
                                          pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                          0,
                                          &pt_data->pt_w2_text[pt_data->ui2_highlight_index],
                                          1,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_cursor_clr,
                                          &t_clean_clr,
                                          &t_clean_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          0,
                                          0);
        }
        else if (w2s_start)
        {
            i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                          pt_data->t_text_draw_rect.i4_left + i4_x,
                                          pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                          0,
                                          w2s_start,
                                          1,
                                          NULL,
                                          0,
                                          t_text_font,
                                          pt_cursor_clr,
                                          &t_clean_clr,
                                          &t_clean_clr,
                                          (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                          (UINT16)t_font_info.i2_width,
                                          0,
                                          0,
                                          0);
        }
        else
        {
            w2s_start = x_uc_w2s_get_ptr(pt_data->pt_w2_text, pt_data->ui2_text_dis_start);                
            ui2_ht_index = pt_data->ui2_highlight_index - pt_data->ui2_text_dis_start;

            i4_ret = x_gl_draw_subset_ustring(h_graphics,
                                              pt_data->t_text_draw_rect.i4_left,
                                              pt_data->t_text_draw_rect.i4_top + pt_data->i4_text_y_offset,
                                              w2s_start,
                                              pt_data->ui2_text_dis_len,
                                              NULL,
                                              0,
                                              t_text_font,
                                              pt_cursor_clr,
                                              (0 == t_font_info.i2_width) ? FALSE : TRUE,
                                              (UINT16)t_font_info.i2_width,
                                              0,
                                              1,
                                              &ui2_ht_index);
        }

        if(GLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw highlight. (GLR=%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_eb_cnt_paint_hlt_char_bk_image(
    WGL_EB_CONTENT_T*                   pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    GL_RECT_T*                          pt_char_rect)
{
    INT32                               i4_ret;
    UINT32                              ui4_img_width = 0;
    UINT32                              ui4_img_height = 0;
    WGL_HIMG_TPL_T                      h_image = NULL_HANDLE;
    GL_RECT_T                           t_char_img_rect;
    INT32                               i4_char_rect_width, i4_char_rect_height;
    INT32                               i4_x_offset, i4_y_offset;
    
    /*check for quick return*/
    h_image = pt_data->t_char_img.h_highlight;
    if (h_image == NULL_HANDLE)
    {
        return WGLR_OK;
    }
    /*get image size*/
    i4_ret = x_wgl_img_tpl_get_size(h_image, &ui4_img_width, &ui4_img_height);
    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    i4_char_rect_width = WGL_RECT_GET_WIDTH(pt_char_rect);
    i4_char_rect_height = WGL_RECT_GET_HEIGHT(pt_char_rect);

    i4_x_offset = ((INT32)ui4_img_width - i4_char_rect_width) >> 1;
    i4_y_offset = ((INT32)ui4_img_height- i4_char_rect_height) >> 1;

    t_char_img_rect.i4_left   = pt_char_rect->i4_left - i4_x_offset;
    t_char_img_rect.i4_top    = pt_char_rect->i4_top  - i4_y_offset;    
    t_char_img_rect.i4_right  = t_char_img_rect.i4_left + ui4_img_width;
    t_char_img_rect.i4_bottom = t_char_img_rect.i4_top + ui4_img_height;

    /*paint image*/
    i4_ret = wgl_img_tpl_paint(h_image, 
                               &t_char_img_rect, 
                               &t_char_img_rect,
                               h_graphics, 
                               &pt_data->t_cvsst,
                               WGL_TPL_PAINT_COMPOSE);

    if (WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to draw image. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
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
static INT32 _wgl_eb_cnt_get_indicator_width(
    WGL_EB_CONTENT_T*           pt_data, 
    UINT16                      ui2_state,
    INT32*                      pi4_width)
{
    INT32                       i4_width, i4_hight, i4_ret;
    HANDLE_T                    t_text_font; 
    WGL_FONT_INFO_T             t_font_info;
    
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*get indicator width*/
    i4_ret = x_fe_get_string_size(t_text_font, pt_data->a_w2_input_indicator, -1, &i4_width, &i4_hight);
    if(FER_OK != i4_ret)
    {
        *pi4_width = 0;

        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get indicator width. (FER=%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_GET_FONT_INFO_FAIL;
    }

    *pi4_width = i4_width;
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
static INT32 _wgl_eb_cnt_update_pwd_draw_rect(
    WGL_EB_CONTENT_T*           pt_data,
    UINT16                      ui2_state)
{
    /*local variable declaration*/
    GL_RECT_T                   t_text_draw_rect = {0};
    GL_RECT_T                   t_available_rect;
    INT32                       i4_available_width;
    INT32                       i4_available_height;
    UINT16                      ui2_real_highlight,ui2_passwd_dis_width;
    INT32                       i4_ret, i4_temp;
    HANDLE_T                    t_text_font; 
    WGL_FONT_INFO_T             t_font_info;
    INT16                       i2_line_height;
    
    /*initialize available rectangle for diaplay*/
    if (WGL_IS_FLAG_TURN_OFF(pt_data->ui4_extra_flag, EB_CHAR_IMG) || 
        (pt_data->t_char_img.ui4_img_num == 0))
    {
        WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
        WGL_RECT_INSET_W_CHECK(&t_available_rect, 
                       pt_data->t_fg_inset.i4_left, 
                       pt_data->t_fg_inset.i4_top,
                       pt_data->t_fg_inset.i4_right, 
                       pt_data->t_fg_inset.i4_bottom);
    }
    else
    {
        WGL_RECT_COPY(&t_available_rect, &pt_data->t_char_imgs_rect);
        t_available_rect.i4_left  -= (pt_data->t_char_img.ui4_gap >> 1);
        t_available_rect.i4_right += (pt_data->t_char_img.ui4_gap >> 1);
        /* For avioding the deviation during the division*/
        if (pt_data->t_char_img.ui4_gap % 2 != 0)
        {
            t_available_rect.i4_right += 1;
        }
    }
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    if(FER_OK != i4_ret || i2_line_height < 0)
    {
        WGL_ASSERT(0);

        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get font height (handle = %d). (FER=%d) at L%d\n\n", t_text_font, i4_ret, __LINE__));
        return  i4_ret;
    }
    /*calculate available rectangle for display*/
    i4_available_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    if(pt_data->ui2_highlight_index >= pt_data->ui2_text_length) /*need to draw indicator*/
    {
        /*if the highlight index is next to the tail, reserve indicator space*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT))
        {
            i4_temp = 0;
            i4_ret = _wgl_eb_cnt_get_indicator_width(pt_data, ui2_state,&i4_temp);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            
            WGL_ASSERT(i4_temp <= 0x0000FFFF);
            pt_data->ui2_indicator_width = (UINT16)i4_temp;
            
            /*turn off indicator width dirty flag*/
            WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT);
        }
        
        i4_available_width = WGL_RECT_GET_WIDTH(&t_available_rect);

        /*show the tail character*/
        ui2_real_highlight = pt_data->ui2_highlight_index;
        pt_data->ui2_highlight_index = (pt_data->ui2_text_length > 0 ) ? (pt_data->ui2_text_length - (UINT16)1) : 0;
        
        /*get text layout info*/
        i4_ret = _wgl_eb_cnt_update_text_info(pt_data, ui2_state,&t_available_rect);

        /*restore the real highlight index*/
        pt_data->ui2_highlight_index = ui2_real_highlight;
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    else
    {
        i4_available_width = WGL_RECT_GET_WIDTH(&t_available_rect);

        /*get text layout info*/
        i4_ret = _wgl_eb_cnt_update_text_info(pt_data, ui2_state,&t_available_rect);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    /*calculate password character display region*/
    ui2_passwd_dis_width = (pt_data->ui2_length_limit-pt_data->ui2_text_dis_len)*pt_data->ui2_passwd_bk_width+pt_data->ui2_text_dis_width;
  
    /*calculate start position in x-axis*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG) && (pt_data->t_char_img.ui4_img_num != 0))
    {
        t_text_draw_rect.i4_left = 0;
    }
    else if(WGL_IS_H_LEFT(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_left = 0;
    }
    else if(WGL_IS_H_CENTER(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_left = (i4_available_width - ui2_passwd_dis_width) >> 1;
    }
    else
    {
        t_text_draw_rect.i4_left = i4_available_width - ui2_passwd_dis_width;
    }
    
    /*calculate start position in y-axis*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG) && (pt_data->t_char_img.ui4_img_num != 0))
    {
        t_text_draw_rect.i4_top = (i4_available_height - i2_line_height) >> 1;
    }
    else if(WGL_IS_V_TOP(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_top = 0;
    }
    else if(WGL_IS_V_CENTER(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_top = (i4_available_height - i2_line_height) >> 1;
    }
    else
    {
        t_text_draw_rect.i4_top = i4_available_height - i2_line_height;
    }

    /*set draw width and height*/
    t_text_draw_rect.i4_right = t_text_draw_rect.i4_left + ui2_passwd_dis_width;
    t_text_draw_rect.i4_bottom = t_text_draw_rect.i4_top + i2_line_height;
    
    /*keep offset in y-axix to draw string to surface*/
    WGL_RECT_OFFSET(&t_text_draw_rect, t_available_rect.i4_left, t_available_rect.i4_top);
    pt_data->i4_text_y_offset = (t_text_draw_rect.i4_top < 0) ? t_text_draw_rect.i4_top : 0;

    /*divide t_text_draw_rect into pt_data->t_text_draw_rec and pt_data->t_password_bk_draw_rect*/
    pt_data->t_text_draw_rect.i4_left = t_text_draw_rect.i4_left;
    pt_data->t_text_draw_rect.i4_right = t_text_draw_rect.i4_left+pt_data->ui2_text_dis_width;
    pt_data->t_text_draw_rect.i4_top = t_text_draw_rect.i4_top;
    pt_data->t_text_draw_rect.i4_bottom = t_text_draw_rect.i4_bottom;
    /*calculate valid text rectange*/
    i4_ret = x_wgl_rect_intersect(&pt_data->t_text_draw_rect, &t_available_rect, &pt_data->t_text_draw_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get the intersection rect of the available rect and text rect. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    pt_data->t_password_bk_draw_rect.i4_left = pt_data->t_text_draw_rect.i4_left+pt_data->ui2_text_dis_width;
    pt_data->t_password_bk_draw_rect.i4_right = pt_data->t_password_bk_draw_rect.i4_left + ui2_passwd_dis_width-pt_data->ui2_text_dis_width;
    pt_data->t_password_bk_draw_rect.i4_top = t_text_draw_rect.i4_top;
    pt_data->t_password_bk_draw_rect.i4_bottom= t_text_draw_rect.i4_bottom;

   
    /*calculate valid password bk display rectange*/
    i4_ret = x_wgl_rect_intersect(&pt_data->t_password_bk_draw_rect, &t_available_rect, &pt_data->t_password_bk_draw_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get the intersection rect of the available rect and text rect. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
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
static INT32 _wgl_eb_cnt_update_draw_rect(
    WGL_EB_CONTENT_T*           pt_data,
    UINT16                      ui2_state)
{
    /*local variable declaration*/
    GL_RECT_T                   t_text_draw_rect = {0};
    GL_RECT_T                   t_available_rect;
    INT32                       i4_available_width;
    INT32                       i4_available_height;
    UINT16                      ui2_real_highlight;
    INT32                       i4_ret, i4_temp;
    HANDLE_T                    t_text_font; 
    WGL_FONT_INFO_T             t_font_info;
    INT16                       i2_line_height;
    
    /*initialize available rectangle for diaplay*/
    if (WGL_IS_FLAG_TURN_OFF(pt_data->ui4_extra_flag, EB_CHAR_IMG) || 
        (pt_data->t_char_img.ui4_img_num == 0))
    {
        WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
        WGL_RECT_INSET_W_CHECK(&t_available_rect, 
                       pt_data->t_fg_inset.i4_left, 
                       pt_data->t_fg_inset.i4_top,
                       pt_data->t_fg_inset.i4_right, 
                       pt_data->t_fg_inset.i4_bottom);
    }
    else
    {
        WGL_RECT_COPY(&t_available_rect, &pt_data->t_char_imgs_rect);
        t_available_rect.i4_left  -= (pt_data->t_char_img.ui4_gap >> 1);
        t_available_rect.i4_right += (pt_data->t_char_img.ui4_gap >> 1);
        /* For avioding the deviation during the division*/
        if (pt_data->t_char_img.ui4_gap % 2 != 0)
        {
            t_available_rect.i4_right += 1;
        }
    }
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*get font height*/
    i4_ret = x_fe_get_height(t_text_font, &i2_line_height);
    if(FER_OK != i4_ret || i2_line_height < 0)
    {
        WGL_ASSERT(0);

        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get font height (handle = %d). (FER=%d) at L%d\n\n", t_text_font, i4_ret, __LINE__));
        return  i4_ret;
    }
    /*calculate available rectangle for display*/
    i4_available_height = WGL_RECT_GET_HEIGHT(&t_available_rect);
    if(pt_data->ui2_highlight_index >= pt_data->ui2_text_length) /*need to draw indicator*/
    {
        /*if the highlight index is next to the tail, reserve indicator space*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT))
        {
            i4_temp = 0;
            i4_ret = _wgl_eb_cnt_get_indicator_width(pt_data, ui2_state,&i4_temp);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            
            WGL_ASSERT(i4_temp <= 0x0000FFFF);
            pt_data->ui2_indicator_width = (UINT16)i4_temp;
            
            /*turn off indicator width dirty flag*/
            WGL_CLEAR_FLAG(pt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT);
        }
        
        i4_available_width = WGL_RECT_GET_WIDTH(&t_available_rect) - pt_data->ui2_indicator_width;
        i4_available_width = (i4_available_width < 0) ? 0 : i4_available_width;
        t_available_rect.i4_right = t_available_rect.i4_left + i4_available_width;

        /*show the tail character*/
        ui2_real_highlight = pt_data->ui2_highlight_index;
        pt_data->ui2_highlight_index = (pt_data->ui2_text_length > 0 ) ? (pt_data->ui2_text_length - (UINT16)1) : 0;
        
        /*get text layout info*/
        i4_ret = _wgl_eb_cnt_update_text_info(pt_data, ui2_state,&t_available_rect);

        /*restore the real highlight index*/
        pt_data->ui2_highlight_index = ui2_real_highlight;
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    else
    {
        i4_available_width = WGL_RECT_GET_WIDTH(&t_available_rect);

        /*get text layout info*/
        i4_ret = _wgl_eb_cnt_update_text_info(pt_data, ui2_state,&t_available_rect);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*calculate start position in x-axis*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG) && (pt_data->t_char_img.ui4_img_num != 0))
    {
        t_text_draw_rect.i4_left = 0;
    }
    else if(WGL_IS_H_LEFT(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_left = 0;
    }
    else if(WGL_IS_H_CENTER(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_left = (i4_available_width - pt_data->ui2_text_dis_width) >> 1;
    }
    else
    {
        t_text_draw_rect.i4_left = i4_available_width - pt_data->ui2_text_dis_width;
    }
    
    /*calculate start position in y-axis*/
    if (WGL_IS_FLAG_TURN_ON(pt_data->ui4_extra_flag, EB_CHAR_IMG) && (pt_data->t_char_img.ui4_img_num != 0))
    {
        t_text_draw_rect.i4_top = (i4_available_height - i2_line_height) >> 1;
    }
    else if(WGL_IS_V_TOP(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_top = 0;
    }
    else if(WGL_IS_V_CENTER(pt_data->ui1_align))
    {
        t_text_draw_rect.i4_top = (i4_available_height - i2_line_height) >> 1;
    }
    else
    {
        t_text_draw_rect.i4_top = i4_available_height - i2_line_height;
    }

    /*set draw width and height*/
    t_text_draw_rect.i4_right = t_text_draw_rect.i4_left + pt_data->ui2_text_dis_width;
    t_text_draw_rect.i4_bottom = t_text_draw_rect.i4_top + i2_line_height;
    
    /*keep offset in y-axix to draw string to surface*/
    WGL_RECT_OFFSET(&t_text_draw_rect, t_available_rect.i4_left, t_available_rect.i4_top);
    pt_data->i4_text_y_offset = (t_text_draw_rect.i4_top < 0) ? t_text_draw_rect.i4_top : 0;

    /*calculate valid text rectange*/
    i4_ret = x_wgl_rect_intersect(&pt_data->t_text_draw_rect, &t_available_rect, &t_text_draw_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get the intersection rect of the available rect and text rect. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
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
static INT32 _wgl_eb_cnt_update_text_info(
    WGL_EB_CONTENT_T*       pt_data,
    UINT16                  ui2_state,
    const GL_RECT_T*        pt_avail_rect)
{
    /*local variable declaration*/
    INT32                   i4_cal_len = 0;
    INT32                   i4_cal_offset = 0;
    INT32                   i4_reset_if_invisible = 0;
    INT32                   i4_reset = 0;
    INT32                   i4_available_width, i4_available_height;
    INT32                   i4_ret;
    
    /*available width for text*/
    i4_available_width = WGL_RECT_GET_WIDTH(pt_avail_rect);
    i4_available_height = WGL_RECT_GET_HEIGHT(pt_avail_rect);

    /*no string stored*/
    if(0 == pt_data->ui2_text_length || i4_available_width <= 0 || i4_available_height <= 0)
    {
        pt_data->ui2_highlight_index = 0;
        pt_data->ui2_text_dis_len = 0;
        pt_data->ui2_text_dis_start = 0;
        pt_data->ui2_text_dis_width = 0;
        pt_data->ui2_highlight_offset = 0;
        return WGLR_OK;
    }
    
    /*handle available rectange change*/
    if(WGL_CNT_EB_RLT_CHG_AVAIL_RECT & pt_data->ui1_relayout_type)
    {
        i4_cal_len++;

        if(!(WGL_CNT_EB_RLT_NO_HT & pt_data->ui1_relayout_type))
        {
            i4_reset_if_invisible++;
        }

        /*clean available rectangle change flag*/
        pt_data->ui1_relayout_type &= ~WGL_CNT_EB_RLT_CHG_AVAIL_RECT;
    }

    /*handle highlight offset change*/
    if(WGL_CNT_EB_RLT_CHG_HT_OFFSET & pt_data->ui1_relayout_type)
    {
        if(pt_data->ui2_highlight_index >= pt_data->ui2_text_dis_start 
            && pt_data->ui2_highlight_index < (pt_data->ui2_text_dis_start + pt_data->ui2_text_dis_len))
        {
            i4_cal_offset++;
        }
        else
        {
            i4_reset++;
        }

        /*clean available rectangle change flag*/
        pt_data->ui1_relayout_type &= ~WGL_CNT_EB_RLT_CHG_HT_OFFSET;
    }

    /*handle content change*/
    if(WGL_CNT_EB_RLT_CHG_DIS_CNT & pt_data->ui1_relayout_type)
    {
        i4_cal_len++;
        i4_reset_if_invisible++;

        /*clean available rectangle change flag*/
        pt_data->ui1_relayout_type &= ~WGL_CNT_EB_RLT_CHG_DIS_CNT;
    }

    /*update display_start, display_length, and display_width*/
    if(i4_cal_len > 0)
    {
        i4_ret = _wgl_eb_cnt_update_display_start(pt_data, ui2_state,i4_available_width);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    /*update highlight_offset*/
    if(i4_cal_offset > 0)
    {
        i4_ret = _wgl_eb_cnt_update_highlight_offset(pt_data, i4_available_width);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*check if highlight is in display area*/
    if(i4_reset_if_invisible > 0)
    {
        if(pt_data->ui2_highlight_index < pt_data->ui2_text_dis_start 
            || pt_data->ui2_highlight_index >= (pt_data->ui2_text_dis_start + pt_data->ui2_text_dis_len))
        {
            i4_reset++;
        }
    }

    /*reset the display_start to show the highlight index at the end of display area*/
    if(i4_reset > 0)
    {
        i4_ret = _wgl_eb_cnt_make_highlight_visible(pt_data, ui2_state,i4_available_width);
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
static INT32 _wgl_eb_cnt_update_display_start(
    WGL_EB_CONTENT_T*           pt_data,
    UINT16                      ui2_state,
    INT32                       i4_available_width)
{
    INT32                       i4_char_numbers = 0, i4_str_width = 0;
    UINT32                      ui4_temp;
    UTF16_T*                    w2s_start;
    UTF16_T*                    w2s_next;
    INT32                       i4_ret;
    HANDLE_T                    t_text_font; 
    WGL_FONT_INFO_T             t_font_info;

    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    if(WGL_IS_H_LEFT(pt_data->ui1_align))
    {
        if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
        {
            i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                       (INT32)pt_data->ui2_text_length - (INT32)pt_data->ui2_text_dis_start /*- 1 + 1*/,
                                                       i4_available_width, 
                                                       &i4_char_numbers, 
                                                       &i4_str_width);
        }
        else
        {
            w2s_start = x_uc_w2s_get_ptr(pt_data->pt_w2_text, pt_data->ui2_text_dis_start);
            i4_ret = wgl_get_max_char_in_box(t_text_font, 
                                             w2s_start, 
                                             (INT32)pt_data->ui2_text_length - (INT32)pt_data->ui2_text_dis_start /*- 1 + 1*/,
                                             TRUE,
                                             i4_available_width, 
                                             NULL, 
                                             &w2s_next, 
                                             &ui4_temp, 
                                             &i4_str_width);
            i4_char_numbers = (INT32)ui4_temp;
            WGL_ASSERT(ui4_temp <= 0x7FFFFFFF);

            WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__)));
        }

        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    else
    {
        if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
        {
            i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                       (INT32)pt_data->ui2_text_dis_start + (INT32)pt_data->ui2_text_dis_len /*- 1 + 1*/,
                                                       i4_available_width, 
                                                       &i4_char_numbers, 
                                                       &i4_str_width);
        }
        else
        {
            w2s_start = pt_data->pt_w2_text;
            i4_ret = wgl_rget_max_char_in_box(t_text_font, 
                                              w2s_start, 
                                              (INT32)pt_data->ui2_text_dis_start + (INT32)pt_data->ui2_text_dis_len /*- 1 + 1*/,
                                              TRUE,
                                              i4_available_width, 
                                              NULL, 
                                              &w2s_next, 
                                              &ui4_temp, 
                                              &i4_str_width);

            i4_char_numbers = (INT32)ui4_temp;
            WGL_ASSERT(ui4_temp <= 0x7FFFFFFF);

            WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__)));
        }

        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        pt_data->ui2_text_dis_start = pt_data->ui2_text_dis_start + pt_data->ui2_text_dis_len /*- 1*/ - (UINT16)i4_char_numbers;
    }
    
    /*check if overflow*/
    WGL_ASSERT(i4_char_numbers < 0x0000FFFF);
    WGL_ASSERT(i4_str_width < 0x0000FFFF);
    
    pt_data->ui2_text_dis_len = (UINT16)i4_char_numbers;
    pt_data->ui2_text_dis_width = (UINT16)i4_str_width;

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
static INT32 _wgl_eb_cnt_update_highlight_offset(
    WGL_EB_CONTENT_T*                   pt_data,
    INT32                               i4_available_width)
{
    INT32       i4_char_numbers = 0, i4_str_width = 0;
    INT32       i4_ret;
    
    /*calculate highlight offset*/
    if(0 != (EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag) && 
       pt_data->ui2_highlight_index > pt_data->ui2_text_dis_start)
    {
        i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                   (INT32)pt_data->ui2_highlight_index - (INT32)pt_data->ui2_text_dis_start,
                                                   i4_available_width, 
                                                   &i4_char_numbers, 
                                                   &i4_str_width);

        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        WGL_ASSERT(i4_char_numbers < 0x0000FFFF);
        WGL_ASSERT(i4_str_width < 0x0000FFFF);
        WGL_ASSERT(i4_char_numbers != (INT32)(pt_data->ui2_highlight_index - pt_data->ui2_text_dis_start));
    }

    pt_data->ui2_highlight_offset = (UINT16)i4_str_width;
    
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
static INT32 _wgl_eb_cnt_make_highlight_visible(
    WGL_EB_CONTENT_T*           pt_data,
    UINT16                      ui2_state,
    INT32                       i4_available_width)
{
    INT32                       i4_char_numbers = 0, i4_str_width = 0;
    UINT32                      ui4_temp;
    UTF16_T*                    w2s_next;
    INT32                       i4_ret;
    HANDLE_T                    t_text_font; 
    WGL_FONT_INFO_T             t_font_info;
    
    /*get font according to widget state*/   
    i4_ret = _wgl_eb_cnt_select_text_font( pt_data, ui2_state, &t_text_font, &t_font_info);  
    if(WGLR_OK != i4_ret)  
    {     
        return i4_ret;   
    }
    /*calculate display start and display length*/
    if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
    {
        i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                   (INT32)(pt_data->ui2_highlight_index + 1), 
                                                   i4_available_width, 
                                                   &i4_char_numbers, 
                                                   &i4_str_width);
    }
    else
    {
        i4_ret = wgl_rget_max_char_in_box(t_text_font, 
                                          pt_data->pt_w2_text, 
                                          pt_data->ui2_highlight_index + 1,
                                          TRUE,
                                          i4_available_width, 
                                          NULL, 
                                          &w2s_next, 
                                          &ui4_temp, 
                                          &i4_str_width);
        i4_char_numbers = (INT32)ui4_temp;
        WGL_ASSERT(ui4_temp <= 0x7FFFFFFF);

        WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__)));
    }
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    WGL_ASSERT(i4_char_numbers <= pt_data->ui2_highlight_index + 1);
    WGL_ASSERT(i4_str_width < 0x0000FFFF);
    
    pt_data->ui2_text_dis_width = (UINT16)i4_str_width;
    pt_data->ui2_text_dis_start = pt_data->ui2_highlight_index - (UINT16)i4_char_numbers + (UINT16)1;
    pt_data->ui2_text_dis_len = (UINT16)i4_char_numbers;
    
    /*if highlight index isn't at the end of display area (at this time start-index is 0), should show the following charachters.*/
    if(0 == pt_data->ui2_text_dis_start && pt_data->ui2_text_dis_len < pt_data->ui2_text_length)
    {
        i4_char_numbers = 0;
        i4_str_width = 0;
        
        if(EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag)
        {
            i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                       (INT32)pt_data->ui2_text_length, 
                                                       i4_available_width, 
                                                       &i4_char_numbers, 
                                                       &i4_str_width);
        }
        else
        {
            i4_ret = wgl_get_max_char_in_box(t_text_font, 
                                             pt_data->pt_w2_text, 
                                             (INT32)pt_data->ui2_text_length,
                                             TRUE,
                                             i4_available_width, 
                                             NULL, 
                                             &w2s_next, 
                                             &ui4_temp, 
                                             &i4_str_width);
            i4_char_numbers = (INT32)ui4_temp;
            WGL_ASSERT(ui4_temp <= 0x7FFFFFFF);

            WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get character number in a line. (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        WGL_ASSERT(i4_char_numbers < 0x0000FFFF);
        WGL_ASSERT(i4_str_width < 0x0000FFFF);
        
        pt_data->ui2_text_dis_len = (UINT16)i4_char_numbers;
        pt_data->ui2_text_dis_width = (UINT16)i4_str_width;     
    }
    
    if(0 != (EB_PWD_CHAR & pt_data->ui4_extra_flag || EB_PWD_CHAR_ALL & pt_data->ui4_extra_flag) &&
       pt_data->ui2_highlight_index > pt_data->ui2_text_dis_start)
    {
        i4_ret = _wgl_eb_cnt_get_max_passwd_in_box((INT32)pt_data->ui2_passwd_width, 
                                                   (INT32)pt_data->ui2_highlight_index - pt_data->ui2_text_dis_start,
                                                   i4_available_width, 
                                                   &i4_char_numbers, 
                                                   &i4_str_width);

        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        WGL_ASSERT(i4_char_numbers == 1);
        WGL_ASSERT(pt_data->ui2_text_dis_width >= (UINT16)i4_str_width);
        pt_data->ui2_highlight_offset = (UINT16)i4_str_width;
    }
    else
    {
        pt_data->ui2_highlight_offset = 0;
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
static INT32 _wgl_eb_cnt_set_img(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    UINT8                       ui1_target,
    const WGL_IMG_INFO_T*       pt_img_info)
{   

    DBG_INFO(("{WGL(EB_CNT)} pt_img_info = 0x%.8x   e_type = 0x%.8x\n"
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
        DBG_ERROR(("{WGL(EB_CNT)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    /*update background image*/
    if(pt_img_info)
    {
        COPY_IMG_INFO_TO_STD(&pt_cnt_data->t_img_bk, pt_img_info, NULL_HANDLE);

        /*remove the flag for action image*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT);
    }
    else
    {
        x_memset(&pt_cnt_data->t_img_bk, 0, sizeof(WGL_IMG_EXT_SET_T));

        /*reset the flag for bk image*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT);
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
static INT32 _wgl_eb_cnt_get_img(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    UINT8                       ui1_target,
    WGL_IMG_INFO_T*             pt_img_info)
{
    /*local variable declaration*/
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL == pt_img_info)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    /*check and cache images*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT))
    {
        i4_ret = _wgl_eb_cnt_prepare_img(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret; 
        }
    
        /*remove the flag for bk image*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_IMG_NOT_INIT);
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
static INT32 _wgl_eb_cnt_set_text(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    UINT16                      ui2_length,
    const UTF16_T*              w2s_str)
{
    /*local variable declaration*/
    INT32                       i4_str_length;

    /*clean the old string first*/
    x_memset(pt_cnt_data->pt_w2_text, 0, pt_cnt_data->ui2_text_length * sizeof(UTF16_T));
    pt_cnt_data->ui2_highlight_index = 0;

    if(NULL == w2s_str)
    {
        /*reset user string*/
        pt_cnt_data->ui2_text_length = 0;
    }
    else
    {
        /*get length*/
        i4_str_length = (INT32)x_uc_w2s_strlen(w2s_str);
        ui2_length = (ui2_length > (UINT16)i4_str_length) ? (UINT16)i4_str_length : ui2_length;
        ui2_length = (ui2_length > pt_cnt_data->ui2_length_limit) ? 
                     pt_cnt_data->ui2_length_limit : 
                     ui2_length;
        
        /*copy string*/
        x_memset(pt_cnt_data->pt_w2_text, 0, _EB_GET_MAX_LENGTH(pt_cnt_data->ui4_extra_flag)* sizeof(UTF16_T));
        x_memcpy(pt_cnt_data->pt_w2_text, w2s_str, ui2_length * sizeof(UTF16_T));
        
        /*keep length*/
        pt_cnt_data->ui2_text_length = ui2_length;

        /*reset status*/
        pt_cnt_data->ui2_text_dis_start = 0;
        pt_cnt_data->ui2_text_dis_width = 0;
        pt_cnt_data->ui2_text_dis_len = 0;
        pt_cnt_data->ui2_highlight_index = 0;
    }
                    
    /*update dirty flag*/
    _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);

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
static INT32 _wgl_eb_cnt_set_color(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    UINT8                       ui1_target,
    const WGL_COLOR_INFO_T*     pt_clr_info)
{
    /*local variable variable*/
    GL_COLOR_T                  t_default_color;

    DBG_INFO(("{WGL(EB_CNT)} pt_clr_info = 0x%.8x   e_type = 0x%.8x\n"
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
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for bk color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT);
        }
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy text color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_text, pt_clr_info, t_default_color);

            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT);

            /*cursor color*/
            if(WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui4_extra_flag, EB_USER_CURSOR_CLR))
            {
                _wgl_eb_cnt_make_cursor_clr(pt_cnt_data);
            }
        }
        else
        {
            /*reset the flag for text color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT);
        }
    }
    else if(WGL_CLR_EB_CURSOR == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy cursor color*/
            COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_cursor, pt_clr_info, t_default_color);

            /*turn on the flag for cursor color*/
            WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_USER_CURSOR_CLR);
        }
        else
        {
            /*turn off the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_USER_CURSOR_CLR);

            /*text color is already initialized*/
            if(WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT))
            {
                _wgl_eb_cnt_make_cursor_clr(pt_cnt_data);
            }
        }
    }
    else
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_eb_cnt_get_color(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    UINT8                       ui1_target,
    WGL_COLOR_INFO_T*           pt_clr_info)
{
    /*local variable declaration*/
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL == pt_clr_info)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid input parameter. For WGL_CMD_GL_GET_COLOR, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check target*/
    if(WGL_CLR_BK == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_eb_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            /*remove the flag for bk color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_BK_CLR_NOT_INIT);
        }

        /*prepare return value*/
        SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_bk);
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_eb_cnt_use_sys_text_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT);
        }

        /*prepare return value*/
        SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_text);
    }
    else if(WGL_CLR_EB_CURSOR == ui1_target)
    {
        
    }
    else
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_eb_cnt_set_font(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    const WGL_FONT_INFO_T*      pt_font_info)
{
    INT32                       i4_ret;
    BOOL                        b_update_ok = FALSE;

    DBG_INFO(("{WGL(EB_CNT)} pt_font_info = 0x%.8x   e_font_cmap =  0x%.8x\n"
                             "\te_font_size = 0x%.8x   e_font_style = 0x%.8x\n"
                             "\tpt_font_info->a_c_font_name = %s\n",
                             pt_font_info,
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_cmap),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_size),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_style),
                             ((NULL == pt_font_info) ? "\0" : pt_font_info->a_c_font_name)));

    /*update dirty flag*/
    WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT);
    _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);   

    do
    {
        if(pt_font_info)
        {
            /*Font should has fixed width under WGL_STL_EB_CHAR_WITH_IMG*/
            if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_CHAR_IMG))
            {
                if (pt_font_info->i2_width == 0)
                {
                    DBG_ERROR(("{WGL(EB_CNT)} ERR: Font should has fixed width under WGL_STL_EB_CHAR_WITH_IMG. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG;
                }
            }
            
            /*update pt_cnt_data->t_text_font*/
            i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, pt_font_info);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
            i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, pt_font_info);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }

            /*remove the flag for bk color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT);
            b_update_ok = TRUE;
        }
        else
        {
            i4_ret = WGLR_OK;
        }
    } while(0);

    if(!b_update_ok)
    {
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));
        
        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            i4_ret = WGL_FONT_FREE(pt_cnt_data->t_text_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }

            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
       /*free pt_cnt_data->t_text_focus_font and clear pt_cnt_data->t_focus_font_info*/
       x_memset(&pt_cnt_data->t_focus_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_focus_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }

        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT);
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
static INT32 _wgl_eb_cnt_get_font(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    WGL_FONT_INFO_T*            pt_font_info)
{
    /*local variable declaration*/
    WGL_FONT_INFO_T             t_font_info;
    INT32                       i4_ret;
    
    /*check parameter*/
    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system font info. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_eb_cnt_set_font(pt_cnt_data, &t_font_info);
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
static INT32 _wgl_eb_cnt_set_font_ex(
    WGL_EB_CONTENT_T*             pt_cnt_data,
    const WGL_FONT_INFO_EX_T*     pt_font_info_ex)
{
    INT32   i4_ret = WGLR_OK;
   
    /*check parameter*/
    if(NULL != pt_font_info_ex && pt_font_info_ex->e_type != WGL_FONT_SET_BASIC)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_SET_FONT_EX (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
      /*update dirty flag*/
    WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_IDR_WIDTH_NOT_INIT);
    _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);   
    

    if(pt_font_info_ex)
    {
        /*Font should has fixed width under WGL_STL_EB_CHAR_WITH_IMG*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_CHAR_IMG))
        {
            if ((pt_font_info_ex->u_font_data.t_basic.t_unfocused.i2_width == 0) ||
                (pt_font_info_ex->u_font_data.t_basic.t_focused.i2_width == 0))
            {
                DBG_ERROR(("{WGL(EB_CNT)} ERR: Font should has fixed width under WGL_STL_EB_CHAR_WITH_IMG. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
        }
        /*update pt_cnt_data->t_text_font,pt_cnt_data->t_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, &(pt_font_info_ex->u_font_data.t_basic.t_unfocused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
       /*update pt_cnt_data->t_text_focus_font,pt_cnt_data->t_focus_font_info*/
        i4_ret = wgl_update_font_handle(&pt_cnt_data->t_focus_font_info, &pt_cnt_data->t_text_focus_font, &(pt_font_info_ex->u_font_data.t_basic.t_focused));
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to update focus font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        /*remove the flag for font*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT);
    }
    else
    {
         /*free pt_cnt_data->t_text_font and clear pt_cnt_data->t_font_info*/
        x_memset(&pt_cnt_data->t_font_info, 0, sizeof(WGL_FONT_INFO_T));

        /*free old handle*/
        if(pt_cnt_data->t_text_font)
        {
            WGL_FONT_FREE(pt_cnt_data->t_text_font);
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
            WGL_FONT_FREE(pt_cnt_data->t_text_focus_font);
            if(0 != i4_ret)
            {
                WGL_ASSERT(0);

                DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to free old focus font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_focus_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }
            pt_cnt_data->t_text_focus_font = NULL_HANDLE;
        }

        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT);
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
static INT32 _wgl_eb_cnt_get_font_ex(
    WGL_EB_CONTENT_T*             pt_cnt_data,
    WGL_FONT_INFO_EX_T*           pt_font_info_ex)
{
    /*local variable declaration*/
    INT32                 i4_ret;
    WGL_FONT_INFO_T       t_font_info;
    
    /*check parameter*/
    if(NULL == pt_font_info_ex)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT_EX, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system font info. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_eb_cnt_set_font(pt_cnt_data, &t_font_info);
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
static INT32 _wgl_eb_cnt_set_char_img(
    WGL_EB_CONTENT_T*          pt_cnt_data,
    WGL_EB_CHAR_IMG_T*         pt_char_img)
{
    HANDLE_TYPE_T              e_handle_type;
    
    if (WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui4_extra_flag, EB_CHAR_IMG))
    {
        return WGLR_INV_CMD;
    }
    else if (NULL == pt_char_img)
    {
        pt_cnt_data->t_char_img.h_highlight = NULL_HANDLE;
        pt_cnt_data->t_char_img.h_normal    = NULL_HANDLE;
        pt_cnt_data->t_char_img.ui4_img_num = 0;
        pt_cnt_data->t_char_img.ui4_gap = 0;
    }
    else if ((FALSE == x_handle_valid(pt_char_img->h_highlight)) ||
             (FALSE == x_handle_valid(pt_char_img->h_normal)))
    {
        return WGLR_INV_ARG;
    }
    else if (HR_OK != x_handle_get_type(pt_char_img->h_highlight, &e_handle_type))   
    {
        return WGLR_INV_ARG;
    }
    else if (HT_WGL_IMG_TPL != e_handle_type)
    {
        return WGLR_INV_ARG;
    }
    else if (HR_OK != x_handle_get_type(pt_char_img->h_normal, &e_handle_type))   
    {
        return WGLR_INV_ARG;
    }
    else if (HT_WGL_IMG_TPL != e_handle_type)
    {
        return WGLR_INV_ARG;
    }
    else
    {
        x_memcpy(&pt_cnt_data->t_char_img, pt_char_img, sizeof(WGL_EB_CHAR_IMG_T));
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
static INT32 _wgl_eb_cnt_get_char_img(
    WGL_EB_CONTENT_T*           pt_cnt_data,
    WGL_EB_CHAR_IMG_T*          pt_char_img)
{
    if (WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui4_extra_flag, EB_CHAR_IMG))
    {
        return WGLR_INV_CMD;
    }
    else if (NULL == pt_char_img)
    {
        return WGLR_INV_ARG;
    }
    else
    {
        x_memcpy(pt_char_img, &pt_cnt_data->t_char_img, sizeof(WGL_EB_CHAR_IMG_T));
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
static INT32 _wgl_eb_cnt_prepare_img(
    WGL_EB_CONTENT_T*           pt_cnt_data)
{
    /*local variable declaration*/
    WGL_IMG_ST_T                e_segment_type;
    UINT32                      ui4_width;
    UINT32                      ui4_height;
    WGL_HIMG_TPL_T              h_tpl_img;
    INT32                       i4_ret;

    /*get system enable image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_enable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_EB, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_IMG_EB image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_enable = h_tpl_img;
    }

    /*get system disable image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_disable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_EB_DIS, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_IMG_EB_DIS image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_disable = h_tpl_img;
    }

    /*get system highlight image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_highlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_EB_HT, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_IMG_EB_HT image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_highlight = h_tpl_img;
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
static INT32 _wgl_eb_cnt_use_sys_bk_color(
    WGL_EB_CONTENT_T*           pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                       i4_ret;

    /*get system enable color for edit-box*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_EB, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_EB color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_EB_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_EB_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_EB_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_highlight);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_EB_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
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
static INT32 _wgl_eb_cnt_use_sys_text_color(
    WGL_EB_CONTENT_T*           pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                       i4_ret;

    /*get system enable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_highlight);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*not user defined cursor color*/
    if(WGL_IS_FLAG_TURN_OFF(pt_cnt_data->ui4_extra_flag, EB_USER_CURSOR_CLR))
    {
        _wgl_eb_cnt_make_cursor_clr(pt_cnt_data);
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
static INT32 _wgl_eb_cnt_select_text_clr(
    WGL_EB_CONTENT_T*           pt_cnt_data, 
    UINT16                      ui2_state,
    GL_COLOR_T*                 pt_selected_clr)
{
    /*local variable declaration*/
    INT32                       i4_ret;

    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_eb_cnt_use_sys_text_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        /*remove the flag for text color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui4_extra_flag, EB_TEXT_CLR_NOT_INIT);
    }

    /*select color*/
    if(IS_STATE_FOCUSED(ui2_state))
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_enable;
    }
    else
    {
        *pt_selected_clr = pt_cnt_data->t_clr_text.t_disable;
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
static INT32 _wgl_eb_cnt_select_text_font(
    WGL_EB_CONTENT_T*                 pt_cnt_data, 
    UINT16                            ui2_state,
    HANDLE_T*                         pt_selected_font,
    WGL_FONT_INFO_T*                  pt_selected_font_info)
{
    /*local variable declaration*/
    INT32                               i4_ret;
    WGL_FONT_INFO_T           t_font_info;
    
    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui4_extra_flag, EB_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_eb_cnt_set_font(pt_cnt_data, &t_font_info);
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


/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static VOID _wgl_eb_cnt_make_cursor_clr(
    WGL_EB_CONTENT_T*                   pt_cnt_data)
{
    _wgl_eb_cnt_get_invert_color(
        &pt_cnt_data->t_cvsst, 
        &pt_cnt_data->t_clr_text.t_highlight, 
        &pt_cnt_data->t_clr_cursor.t_highlight);
        
    _wgl_eb_cnt_get_invert_color(
        &pt_cnt_data->t_cvsst, 
        &pt_cnt_data->t_clr_text.t_enable, 
        &pt_cnt_data->t_clr_cursor.t_enable);

    _wgl_eb_cnt_get_invert_color(
        &pt_cnt_data->t_cvsst, 
        &pt_cnt_data->t_clr_text.t_disable, 
        &pt_cnt_data->t_clr_cursor.t_disable);
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
static VOID _wgl_eb_cnt_get_invert_color(
    const WGL_CANVAS_STATUS_T*       pt_cvsst,
    const GL_COLOR_T*                pt_clr_src,
    GL_COLOR_T*                      pt_clr_inverted)
{
    GL_COLOR_T                       t_dclr_src, t_dclr_inv;
    INT32                            i4_ret;
    
    wgl_cvsst_get_direct_color(pt_cvsst, pt_clr_src, &t_dclr_src);
    t_dclr_inv.a = t_dclr_src.a;
    t_dclr_inv.u1.r = ~t_dclr_src.u1.r;
    t_dclr_inv.u2.g = ~t_dclr_src.u2.g;
    t_dclr_inv.u3.b = ~t_dclr_src.u3.b;
    
    i4_ret = wgl_cvsst_match_color(pt_cvsst, FALSE, &t_dclr_inv, pt_clr_inverted);
    if( WGLR_OK != i4_ret )
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Match invert color error. (%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_eb_cnt_index_start(
    WGL_EB_CONTENT_T*               pt_cnt_data,
    UINT16                          ui2_index)
{
    UINT16                          ui2_old_start;

    if(ui2_index > pt_cnt_data->ui2_length_limit - (UINT16)1)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. Start index is over %d. (%d) at L%d\n\n", pt_cnt_data->ui2_length_limit - 1, WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    ui2_old_start = pt_cnt_data->ui2_text_dis_start;
    pt_cnt_data->ui2_text_dis_start = ui2_index;
        
    if(ui2_old_start != ui2_index)
    {
        _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET);
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
static INT32 _wgl_eb_cnt_index_highlight(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    BOOL                                b_highlight)
{
    UINT32                              ui4_old_index;
    BOOL                                b_old_highlight;
    UINT8                               ui1_flag;
    
    if(ui2_index > pt_cnt_data->ui2_length_limit - (UINT16)1)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. Highlight index is over %d. (%d) at L%d\n\n", pt_cnt_data->ui2_length_limit - 1, WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    /*keep current mode*/
    b_old_highlight = !(WGL_CNT_EB_RLT_NO_HT & pt_cnt_data->ui1_relayout_type);

    /*turn on relayout flag*/
    _wgl_eb_cnt_switch_highlight(pt_cnt_data, b_highlight);
    
    /*keep old status*/
    ui4_old_index = pt_cnt_data->ui2_highlight_index;
    
    /*keep new status*/
    pt_cnt_data->ui2_highlight_index = ui2_index;
    
    /*update highlight offset*/ 
    if( (ui4_old_index != ui2_index || b_old_highlight != b_highlight))
    {
        /*because the reserve space for indicator may affect the display area*/
        if(b_highlight)
        {
            ui1_flag = WGL_CNT_EB_RLT_CHG_HT_OFFSET | WGL_CNT_EB_RLT_CHG_AVAIL_RECT;
        }
        else
        {
            ui1_flag = WGL_CNT_EB_RLT_CHG_AVAIL_RECT;
        }
        
        _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, ui1_flag);
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
static INT32 _wgl_eb_cnt_index_replace(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    UTF16_T                             w2_new_char)
{
    INT32                               i4_temp;
    UINT8                               ui1_update;

    if(ui2_index > pt_cnt_data->ui2_length_limit - (UINT16)1)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. Replace index is over %d. (%d) at L%d\n\n", pt_cnt_data->ui2_length_limit - 1, WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*replace character*/
    i4_temp = x_uc_w2s_set(pt_cnt_data->pt_w2_text, ui2_index, w2_new_char);
    WGL_ASSERT(UCR_OK == i4_temp);

    /*update relayout flag*/
    if( ui2_index >= pt_cnt_data->ui2_text_dis_start 
        && ui2_index < (pt_cnt_data->ui2_text_dis_start + pt_cnt_data->ui2_text_dis_len) )
    {
        ui1_update = WGL_CNT_EB_RLT_CHG_DIS_CNT;

        /*check if the highlight_offset should be recalculated*/
        if(ui2_index <= pt_cnt_data->ui2_highlight_index)
        {
            ui1_update |= WGL_CNT_EB_RLT_CHG_HT_OFFSET;
        }

        _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, ui1_update);
    }
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cnt_index_remove(
    WGL_EB_CONTENT_T*                   pt_data,
    UINT16                              ui2_index,
    UINT16                              ui2_length)
{
    /*local variable declaration*/
    INT32                               i4_reset_len;
    UTF16_T*                            w2s_old_str;
    UTF16_T*                            w2s_new_str;
    UTF16_T*                            w2s_ret;
    UINT16                              ui2_old_start, ui2_old_len;
    BOOL                                b_relayout = FALSE;
    UINT8                               ui1_update;

    /*check index*/
    if(ui2_index > pt_data->ui2_length_limit - (UINT16)1)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. Remove index is over %d. (%d) at L%d\n\n", pt_data->ui2_length_limit - 1, WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(ui2_index >= pt_data->ui2_text_length || 0 == ui2_length)
    {
        return WGLR_OK;
    }

    /*get invalid length*/
    ui2_length = ((ui2_index + ui2_length) > pt_data->ui2_text_length) ? (pt_data->ui2_text_length - ui2_index) : ui2_length;
    
    /*get move string*/
    w2s_old_str = x_uc_w2s_get_ptr(pt_data->pt_w2_text, ui2_index);
    w2s_new_str = x_uc_w2s_get_ptr(w2s_old_str, ui2_length);

    /*move*/
    w2s_ret = x_uc_w2s_strcpy(w2s_old_str, w2s_new_str);
    WGL_ASSERT(NULL != w2s_ret);

    /*reset tail characters*/
    i4_reset_len = (INT32)ui2_length;
    while(i4_reset_len)
    {
        *(pt_data->pt_w2_text + pt_data->ui2_text_length - i4_reset_len) = _EB_UTF16_NULL_CHAR;
        i4_reset_len--;
    }

    /*update text length*/
    pt_data->ui2_text_length -= ui2_length;


    ui2_old_start = pt_data->ui2_text_dis_start;
    ui2_old_len = pt_data->ui2_text_dis_len;

    /*update display start and len*/
    pt_data->ui2_text_dis_len = (pt_data->ui2_text_dis_len > pt_data->ui2_text_length) ? pt_data->ui2_text_length : pt_data->ui2_text_dis_len;

    pt_data->ui2_text_dis_start = (pt_data->ui2_text_dis_start >= pt_data->ui2_text_length) ? 
                                  (pt_data->ui2_text_length - pt_data->ui2_text_dis_len) : 
                                  pt_data->ui2_text_dis_start;
    pt_data->ui2_text_dis_len = ((pt_data->ui2_text_dis_start + pt_data->ui2_text_dis_len) > pt_data->ui2_text_length) ? 
                                (pt_data->ui2_text_length - pt_data->ui2_text_dis_start) : 
                                pt_data->ui2_text_dis_len;

    /*remove data is in visible area*/
    if( ui2_index >= ui2_old_start && ui2_index < (ui2_old_start + ui2_old_len) )
    {
        b_relayout = TRUE;
    }
    if(ui2_old_start != pt_data->ui2_text_dis_start || ui2_old_len != pt_data->ui2_text_dis_len)
    {
        b_relayout = TRUE;
    }

    /*update relayout flag*/
    if(b_relayout)
    {
        ui1_update = WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET;
        _wgl_eb_cnt_update_relayout_flag(pt_data, ui1_update);
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
static INT32 _wgl_eb_cnt_index_insert(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    UINT16                              ui2_index,
    UTF16_T                             w2_new_char)
{
    /*local variable declaration*/
    UTF16_T*                            pw2_char;
    INT32                               i4_length;
    UINT8                               ui1_update;
    BOOL                                b_relayout = FALSE;

    /*check length*/
    if(pt_cnt_data->ui2_text_length >= pt_cnt_data->ui2_length_limit)
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Text buffer is full. (%d) at L%d\n\n", WGLR_BUFFER_NOT_ENOUGH, __LINE__));
        return WGLR_BUFFER_NOT_ENOUGH;
    }

    /*check index*/
    if(ui2_index > pt_cnt_data->ui2_text_length) /*over the tail*/
    {
        DBG_ERROR(("{WGL(EB_CNT)} ERR: Invalid arguments. Insert index is over %d. (%d) at L%d\n\n", pt_cnt_data->ui2_text_length, WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*shift string*/
    if(pt_cnt_data->ui2_text_length)
    {
        i4_length = (INT32)(pt_cnt_data->ui2_text_length - ui2_index);
        pw2_char = pt_cnt_data->pt_w2_text + pt_cnt_data->ui2_text_length;

        while(i4_length)
        {
            *pw2_char = *(pw2_char - 1);
            pw2_char--;
            i4_length--;
        }

        WGL_ASSERT(pw2_char == pt_cnt_data->pt_w2_text + ui2_index);
    }
    
    /*insert character*/
    *(pt_cnt_data->pt_w2_text + ui2_index) = w2_new_char;
    
    /*update text length*/
    pt_cnt_data->ui2_text_length++;
    pt_cnt_data->ui2_text_dis_len++;

    /*insert in visible area*/
    if(ui2_index >= pt_cnt_data->ui2_text_dis_start 
        && ui2_index < (pt_cnt_data->ui2_text_dis_start + pt_cnt_data->ui2_text_dis_len))
    {
        b_relayout = TRUE;
    }
    else if(0 == pt_cnt_data->ui2_text_dis_len) /*no string in visible area, insertion may affect the layout*/
    {
        b_relayout = TRUE;
    }

    /*update relayout flag*/
    if(b_relayout)
    {
        ui1_update = WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_HT_OFFSET;
        _wgl_eb_cnt_update_relayout_flag(pt_cnt_data, ui1_update);
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
static VOID _wgl_eb_cnt_update_relayout_flag(
    WGL_EB_CONTENT_T*               pt_cnt_data,
    UINT8                           ui1_relayout_type)
{
    /*local variable declaration*/
    UINT8  ui1_old_type = pt_cnt_data->ui1_relayout_type;

    if(pt_cnt_data->ui1_relayout_type & WGL_CNT_EB_RLT_NO_HT)
    {
        pt_cnt_data->ui1_relayout_type |= (ui1_relayout_type & (~WGL_CNT_EB_RLT_CHG_HT_OFFSET));
    }
    else
    {
        pt_cnt_data->ui1_relayout_type |= (ui1_relayout_type & (~WGL_CNT_EB_RLT_NO_HT));
    }
    
    if( (pt_cnt_data->ui1_relayout_type & (WGL_CNT_EB_RLT_CHG_AVAIL_RECT | WGL_CNT_EB_RLT_CHG_DIS_CNT | WGL_CNT_EB_RLT_CHG_ALIGN))
        || (ui1_old_type != pt_cnt_data->ui1_relayout_type))
    {
        pt_cnt_data->b_is_relayout = TRUE;

        /*clean align dirty flag*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui1_relayout_type, WGL_CNT_EB_RLT_CHG_ALIGN);
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
static VOID _wgl_eb_cnt_switch_highlight(
    WGL_EB_CONTENT_T*                   pt_cnt_data,
    BOOL                                b_tunr_on)
{
    if(b_tunr_on) /*turn on highlight*/
    {
        pt_cnt_data->ui1_relayout_type &= ~WGL_CNT_EB_RLT_NO_HT;
    }
    else /*turn off highlight*/
    {
        pt_cnt_data->ui1_relayout_type &= ~WGL_CNT_EB_RLT_CHG_HT_OFFSET;
        pt_cnt_data->ui1_relayout_type |= WGL_CNT_EB_RLT_NO_HT;
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
static INT32 _wgl_eb_cnt_get_max_passwd_in_box(
    INT32                               i4_passwd_width,
    INT32                               i4_max_length,
    INT32                               i4_box_width,
    INT32*                              pi4_passwd_numbers,
    INT32*                              pi4_occupy_width)
{
    WGL_CHECK_INTERNAL(i4_passwd_width > 0 && i4_max_length >= 0 && i4_box_width > 0);
    WGL_CHECK_INTERNAL(pi4_passwd_numbers && pi4_occupy_width);

    *pi4_passwd_numbers = i4_box_width / i4_passwd_width;
    *pi4_passwd_numbers = (*pi4_passwd_numbers > i4_max_length) ? i4_max_length : *pi4_passwd_numbers;
    *pi4_occupy_width = *pi4_passwd_numbers * i4_passwd_width;

    return WGLR_OK;
}
