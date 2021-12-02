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
 * $RCSfile: wgl_button_cnt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 0a7fa33b9361f19cdf2b965eb22b6e88 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "font/x_fe.h"
#include "unicode/x_uc_str.h"
#include "handle/x_handle.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
#include "wgl/button/wgl_button_cnt.h"
#include "wgl/button/wgl_button_cli.h"
#include "wgl/arabic.h"



/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_btn_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*Extra Flag*/
#define BTN_BK_CLR_NOT_INIT         ((UINT16)0x0001)
#define BTN_BK_IMG_NOT_INIT         ((UINT16)0x0002)
#define BTN_IMG_UI                  ((UINT16)0x0004)
#define BTN_FONT_NOT_INIT           ((UINT16)0x0008)
#define BTN_TEXT_CLR_NOT_INIT       ((UINT16)0x0010)
#define BTN_DIS_TEXT_FIRST          ((UINT16)0x0020)
#define BTN_NO_BK                   ((UINT16)0x0040)
#define BTN_USE_SYS_IMG             ((UINT16)0x0080)
#define BTN_NOT_CLEAN_BK            ((UINT16)0x0100)
#define BTN_FIX_TEXT_DIR            ((UINT16)0x0200)
#define BTN_FIX_TEXT_DIR_ALIGN      ((UINT16)0x0400)

/**/
#define _BTN_GET_MAX_LENGTH(flag)    (WGL_BTN_MAX_STRING)

typedef struct _WGL_BTN_CONTENT_T
{
#ifdef CLI_LVL_ALL     
    UINT32                         ui4_mem_size;        /*this field must be the first field in the structure*/
#endif  
    WGL_COLOR_EXT_SET_T             t_clr_bk;           /*background color set*/
    WGL_IMG_EXT_SET_T               t_img_bk;           /*background template image*/
    WGL_IMG_EXT_SET_T               t_fg_img;           /*foreground icon*/
    WGL_COLOR_EXT_SET_T             t_clr_text;         /*text color set*/
    GL_RECT_T                       t_cnt_rect;         /*content rect*/
    GL_RECT_T                       t_img_draw_rect;    /*rectangle for drawing image*/
    GL_RECT_T                       t_text_draw_rect;   /*rectangle for drawing string*/
    WGL_INSET_T                     t_fg_inset;         /*inset for foreground icon and text*/
    WGL_FONT_INFO_T                 t_font_info;        /*font info for text*/
    HFONT_T                         t_text_font;        /*font for text*/
    WGL_FONT_INFO_EX_T              t_font_info_ex;     /*the extend font info*/
    INT32                           i4_obj_gap;         /*gap between icon and text*/
    UINT16                          ui2_state;          /*the button widget state*/
    UINT16                          ui2_text_dis_width; /*string width in pixel to display*/
    UINT16                          ui2_text_dis_len;   /*string length in character to display*/
    UINT16                          ui2_text_length;    /*total length of user string*/
    UINT16                          ui2_line_height;    /*height of a line*/
    UINT16                          ui2_extra_flag;     /*Flag*/
    UINT8                           ui1_align;          /*alignment*/
    UINT8                           ui1_text_direction; /*text read direction, left-to-right or right-to-left*/
    BOOL                            b_opposite_align;   /*TRUE: when paint, use the opposite alignment*/
    BOOL                            b_is_data_dirty;    /*TRUE: re-layout display*/
    WGL_CANVAS_STATUS_T             t_cvsst;            /*canvas status*/
    UTF16_T*                        pt_w2_arab_dsp_text;
    UINT16                          ui2_arab_dsp_len;
    UINT8                           ui1_cur_font_id;
    UTF16_T*                        pt_w2_text;         /*the start pos of the string buffer*/
} WGL_BTN_CONTENT_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_btn_cnt_paint_bk(
    WGL_BTN_CONTENT_T*                  pt_data,
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state);

static INT32 _wgl_btn_cnt_paint_fg(
    WGL_BTN_CONTENT_T*                  pt_data,
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr);

static INT32 _wgl_btn_cnt_update_draw_rect(
    WGL_BTN_CONTENT_T*                  pt_data, 
    WGL_HIMG_TPL_T                      h_image);

static INT32 _wgl_btn_cnt_calc_necessary_width(
    WGL_BTN_CONTENT_T*                  pt_data, 
    UINT16                              ui2_state,
    UINT32*                             pui4_width);

static INT32 _wgl_btn_cnt_set_img(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_IMG_INFO_T*               pt_img_info);

static INT32 _wgl_btn_cnt_get_img(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_target,
    WGL_IMG_INFO_T*                     pt_img_info);

static INT32 _wgl_btn_cnt_set_text(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT16                              ui2_length,
    const UTF16_T*                      w2s_str);

static INT32 _wgl_btn_cnt_set_color(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_COLOR_INFO_T*             pt_clr_info);

static INT32 _wgl_btn_cnt_get_color(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_target,
    WGL_COLOR_INFO_T*                   pt_clr_info);

static INT32 _wgl_btn_cnt_set_text_dir(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_text_dir);

static INT32 _wgl_btn_cnt_get_text_dir(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT32*                             pui4_text_dir);

static INT32 _wgl_btn_cnt_get_render_cond(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT16                              ui2_state,
    UINT8*                             pui1_render_cond);

static INT32 _wgl_btn_cnt_set_font(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    const WGL_FONT_INFO_T*              pt_font_info);

static INT32 _wgl_btn_cnt_get_font(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    WGL_FONT_INFO_T*                    pt_font_info);

static INT32 _wgl_btn_cnt_set_font_ex(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_ex_font_info);

static INT32 _wgl_btn_cnt_get_font_ex(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    WGL_FONT_INFO_EX_T*                 pt_ex_font_info);

static INT32 _wgl_btn_cnt_update_font(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_ex_font_info);

/*WGL_CMD_BTN_GET_MIN_SIZE command handler*/
static INT32 _wgl_btn_cnt_get_min_size(
    const WGL_BTN_CONTENT_T*            pt_cnt_data,
    GL_SIZE_T*                          pt_size);

static INT32 _wgl_btn_cnt_prepare_img(
    WGL_BTN_CONTENT_T*                  pt_cnt_data);

static INT32 _wgl_btn_cnt_use_sys_bk_color(
    WGL_BTN_CONTENT_T*                  pt_cnt_data);

static INT32 _wgl_btn_cnt_use_sys_text_color(
    WGL_BTN_CONTENT_T*                  pt_cnt_data);

static INT32 _wgl_btn_cnt_select_text_clr(
    WGL_BTN_CONTENT_T*                  pt_cnt_data, 
    UINT16                              ui2_state,
    GL_COLOR_T*                         pt_selected_clr);

static void _wgl_btn_cnt_reset_dirty_flag(
    WGL_BTN_CONTENT_T*                  pt_cnt_data);

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
INT32 wgl_btn_cnt_inst_create( 
    WGL_CONTENT_CONTEXT_HANDLE_T*       ph_cnt_data, 
    WGL_CONTENT_CREATE_INPUT_T*         pt_param)
{
    /*local variable declaration*/
    WGL_BTN_CONTENT_T*                  pt_data;
    UTF16_T                             *pt_w2_text;
    UTF16_T                             *pt_w2_arab_dsp_text;
    
    DBG_API(("{WGL(BTN_CNT)} %s: ph_cnt_data = 0x%.8x \tpt_param = 0x%.8x\n", __FUNCTION__, ph_cnt_data, pt_param));

    /*check parameter*/
    if(NULL == ph_cnt_data || NULL == pt_param)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(BTN_CNT)} pt_param->pv_init = 0x%.8x   ui4_type_border = 0x%.8x\n"
                             "\tt_rect_cnt(l, t, r, b) = (%d, %d, %d, %d)\n",
                             pt_param->pv_init,
                             pt_param->t_rect_cnt.i4_left,
                             pt_param->t_rect_cnt.i4_top,
                             pt_param->t_rect_cnt.i4_right,
                             pt_param->t_rect_cnt.i4_bottom));

    /*allocate memory for content data*/
    pt_data = (WGL_BTN_CONTENT_T*) WGL_MEM_ALLOC( WGL_ALIGN_4(sizeof(WGL_BTN_CONTENT_T)) + _BTN_GET_MAX_LENGTH((UINT32)pt_param->pv_init) * sizeof(UTF16_T) * 2 + sizeof(UTF16_T)*2 );
    if(NULL == pt_data)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to alloc memory for content instance. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }
    
     /*initialize content data*/
    x_memset(pt_data, 0, WGL_ALIGN_4(sizeof(WGL_BTN_CONTENT_T)) + _BTN_GET_MAX_LENGTH((UINT32)pt_param->pv_init) * sizeof(UTF16_T) * 2 + sizeof(UTF16_T)*2 );   
  
#ifdef CLI_LVL_ALL
    pt_data->ui4_mem_size = WGL_ALIGN_4(sizeof(WGL_BTN_CONTENT_T)) + _BTN_GET_MAX_LENGTH((UINT32)pt_param->pv_init) * sizeof(UTF16_T) * 2 + sizeof(UTF16_T)*2;
#endif
    pt_w2_text = (UTF16_T*)((UINT32)(pt_data) + (UINT32)(WGL_ALIGN_4(sizeof(WGL_BTN_CONTENT_T))));
    pt_w2_arab_dsp_text = (UTF16_T*)((UINT32)(pt_data) + (UINT32)(WGL_ALIGN_4(sizeof(WGL_BTN_CONTENT_T))) + (UINT32)(_BTN_GET_MAX_LENGTH((UINT32)pt_param->pv_init) * sizeof(UTF16_T))+ (UINT32)sizeof(UTF16_T));
    
     pt_data->ui2_extra_flag = BTN_BK_CLR_NOT_INIT 
                            | BTN_BK_IMG_NOT_INIT 
                            | BTN_FONT_NOT_INIT 
                            | BTN_TEXT_CLR_NOT_INIT;
    pt_data->ui1_align = WGL_AS_LEFT_TOP;
    pt_data->b_is_data_dirty = TRUE;
    wgl_cvsst_copy(&pt_data->t_cvsst, pt_param->pt_cvsst);
    pt_data->pt_w2_text = pt_w2_text;
    pt_data->pt_w2_arab_dsp_text = pt_w2_arab_dsp_text;

    /*store settings*/
    WGL_RECT_COPY(&pt_data->t_cnt_rect, &pt_param->t_rect_cnt);
    pt_data->ui2_extra_flag |= (WGL_IS_IMAGE_UI((UINT32)(pt_param->pv_init))) ? BTN_IMG_UI : 0;
    pt_data->ui2_extra_flag |= (WGL_STL_GL_NO_BK & ((UINT32)pt_param->pv_init)) ? BTN_NO_BK : 0;
    pt_data->ui2_extra_flag |= (WGL_STL_GL_USE_SYS_IMG & ((UINT32)pt_param->pv_init)) ? BTN_USE_SYS_IMG : 0;
    pt_data->ui2_extra_flag |= (WGL_STL_GL_BDR_FILL_CNT_BK & ((UINT32)pt_param->pv_init)) ? BTN_NOT_CLEAN_BK : 0;
    pt_data->ui2_extra_flag |= (WGL_STL_GL_FIX_TEXT_DIR& ((UINT32)pt_param->pv_init)) ? BTN_FIX_TEXT_DIR: 0;
    pt_data->ui2_extra_flag |= (WGL_STL_GL_FIX_TEXT_DIR_ALIGN& ((UINT32)pt_param->pv_init)) ? BTN_FIX_TEXT_DIR_ALIGN: 0;

    /*The default text direction is LEFT_TO_RIGHT*/
    pt_data->ui1_text_direction = WGL_TEXT_DIR_LEFT_TO_RIGHT;
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
INT32 wgl_btn_cnt_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data)
{
    WGL_BTN_CONTENT_T*                  pt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(BTN_CNT)} %s: h_cnt_data = 0x%.8x\n", __FUNCTION__, h_cnt_data));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
        
    pt_data = (WGL_BTN_CONTENT_T*)h_cnt_data;   
    
    /*free font*/
    if(pt_data->t_text_font)
    {
        i4_ret = WGL_FONT_FREE(pt_data->t_text_font);
        pt_data->t_text_font = NULL_HANDLE;
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
INT32 wgl_btn_cnt_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    GL_COLOR_T                          t_text_clr;
    WGL_FONT_INFO_T                     t_font_info;
    WGL_BTN_CONTENT_T*                  pt_cnt_data;
    INT32                               i4_ret;

    DBG_API(("{WGL(BTN_CNT)} %s: h_cnt_data = 0x%.8x \th_graphics = 0x%.8x \tui2_state = %d\n", __FUNCTION__, h_cnt_data, h_graphics, ui2_state));

    /*check parameter*/
    if(NULL == h_cnt_data || NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_cnt_data = (WGL_BTN_CONTENT_T*)h_cnt_data;

    /*init text color*/
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_text_clr);
    
    /*paint background*/
    i4_ret = _wgl_btn_cnt_paint_bk(pt_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*select text color*/
    i4_ret = _wgl_btn_cnt_select_text_clr(pt_cnt_data, ui2_state, &t_text_clr);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*check font*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system font. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_btn_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*paint foreground*/
    return _wgl_btn_cnt_paint_fg(pt_cnt_data, 
                                 h_graphics,
                                 ui4_upt_rect,
                                 pt_upt_rect,
                                 ui2_state, 
                                 &t_text_clr);  
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
INT32 wgl_btn_cnt_command_proc(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data,
    UINT32                              ui4_cmd_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2)
{   
    /*local variable declaration*/
    WGL_BTN_CONTENT_T*                  pt_btn;

    DBG_API(("{WGL(BTN_CNT)} %s: h_cnt_data = 0x%.8x \tui4_cmd_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
            __FUNCTION__, 
            h_cnt_data, 
            ui4_cmd_id,
            pv_param1,
            pv_param2));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. The 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    if(!(IS_BTN_CNT_CMD(ui4_cmd_id) || WGL_IS_CMD_GENERAL(ui4_cmd_id)))
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    pt_btn = (WGL_BTN_CONTENT_T*)h_cnt_data;

    switch (ui4_cmd_id)
    {
    case WGL_CMD_GL_SET_IMAGE:
        return _wgl_btn_cnt_set_img(pt_btn, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_IMAGE:
        return _wgl_btn_cnt_get_img(pt_btn, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_COLOR:
        return _wgl_btn_cnt_set_color(pt_btn, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_COLOR:
        return _wgl_btn_cnt_get_color(pt_btn, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_FONT:
        return _wgl_btn_cnt_set_font(pt_btn, (WGL_FONT_INFO_T*)pv_param1);

    case WGL_CMD_GL_GET_FONT:
        return _wgl_btn_cnt_get_font(pt_btn, (WGL_FONT_INFO_T*)pv_param1);
        
    case WGL_CMD_GL_SET_FONT_EX:
        return _wgl_btn_cnt_set_font_ex(pt_btn, (WGL_FONT_INFO_EX_T*)pv_param1);
    
    case WGL_CMD_GL_GET_FONT_EX:
        return _wgl_btn_cnt_get_font_ex(pt_btn, (WGL_FONT_INFO_EX_T*)pv_param1);

    case WGL_CMD_GL_SET_TEXT_DIRECTION:
        return _wgl_btn_cnt_set_text_dir(pt_btn, WGL_POINTER_TO_UINT8(pv_param1));

    case WGL_CMD_GL_GET_TEXT_DIRECTION:
        return _wgl_btn_cnt_get_text_dir(pt_btn, (UINT32*)pv_param1);    

    case WGL_CMD_GL_GET_RENDER_COND:
        return _wgl_btn_cnt_get_render_cond(pt_btn, WGL_POINTER_TO_UINT16(pv_param1),(UINT8*)pv_param2);   

    case WGL_CMD_BTN_SET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(BTN_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                                    pv_param1,
                                    ((WGL_INSET_T*)pv_param1)->i4_left,
                                    ((WGL_INSET_T*)pv_param1)->i4_top,
                                    ((WGL_INSET_T*)pv_param1)->i4_right,
                                    ((WGL_INSET_T*)pv_param1)->i4_bottom));
            
            if(FALSE == wgl_is_legal_inset(&pt_btn->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_btn->t_fg_inset, (WGL_INSET_T*)pv_param1);

            /*turn on dirty-flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
        }
        break;

    case WGL_CMD_BTN_GET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &pt_btn->t_fg_inset);
        }
        break;

    case WGL_CMD_BTN_SET_ALIGN:
        {
            if(FALSE == wgl_is_legal_alignment(WGL_POINTER_TO_UINT8(pv_param1)))
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_SET_ALIGN, the alignment specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            pt_btn->ui1_align = WGL_POINTER_TO_UINT8(pv_param1);

            /*update dirty flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
        }
        break;

    case WGL_CMD_BTN_GET_ALIGN:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_ALIGN, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT8*)pv_param1) = pt_btn->ui1_align;
        }
        break;

    case WGL_CMD_BTN_SET_TEXT:
        return _wgl_btn_cnt_set_text(pt_btn, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1);

    case WGL_CMD_BTN_GET_TEXT:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_TEXT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            /*check length*/
            if(pt_btn->ui2_text_length > WGL_POINTER_TO_UINT16(pv_param2))
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: The input buffer isn't enough. %d-character is necessary. (%d) at L%d\n\n", pt_btn->ui2_text_length, WGLR_BUFFER_NOT_ENOUGH, __LINE__));
                return WGLR_BUFFER_NOT_ENOUGH;
            }
            
            /*copy string*/
            x_memcpy(pv_param1, pt_btn->pt_w2_text, pt_btn->ui2_text_length * sizeof(UTF16_T));
        }
        break;

    case WGL_CMD_BTN_SET_DISTANCE:
        {
            pt_btn->i4_obj_gap = (INT32)pv_param1;

            /*update dirty flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
        }
        break;

    case WGL_CMD_BTN_GET_DISTANCE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_DISTANCE, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((INT32*)pv_param1) = pt_btn->i4_obj_gap;
        }
        break;

    case WGL_CMD_BTN_SET_ORDER:
        {
            if(WGL_BTN_DO_IMG_FIRST == WGL_POINTER_TO_UINT8(pv_param1))
            {
                /*remove the text-first flag*/
                WGL_CLEAR_FLAG(pt_btn->ui2_extra_flag, BTN_DIS_TEXT_FIRST);
            }
            else if(WGL_BTN_DO_TEXT_FIRST == WGL_POINTER_TO_UINT8(pv_param1))
            {
                /*turn on the text-first flag*/
                WGL_SET_FLAG(pt_btn->ui2_extra_flag, BTN_DIS_TEXT_FIRST);
            }
            else
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_SET_ORDER, the order specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*update dirty flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
        }
        break;

    case WGL_CMD_BTN_GET_ORDER:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_ORDER, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*check flag*/
            if(WGL_IS_FLAG_TURN_ON(pt_btn->ui2_extra_flag, BTN_DIS_TEXT_FIRST))
            {
                *((UINT8*)pv_param1) = WGL_BTN_DO_TEXT_FIRST;
            }
            else
            {
                *((UINT8*)pv_param1) = WGL_BTN_DO_IMG_FIRST;
            }
        }
        break;

    case WGL_CNT_BTN_SET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CNT_BTN_SET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(BTN_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                                    pv_param1,
                                    ((GL_RECT_T*)pv_param1)->i4_left,
                                    ((GL_RECT_T*)pv_param1)->i4_top,
                                    ((GL_RECT_T*)pv_param1)->i4_right,
                                    ((GL_RECT_T*)pv_param1)->i4_bottom));

            /*copy rect*/
            WGL_RECT_COPY(&pt_btn->t_cnt_rect, (GL_RECT_T*)pv_param1);  
            
            /*update dirty flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
        }
        break;

    case WGL_CNT_BTN_GET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CNT_BTN_GET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*prepare return value*/
            WGL_RECT_COPY((GL_RECT_T*)pv_param1, &pt_btn->t_cnt_rect);
        }
        break;

    case WGL_CNT_BTN_GET_MIN_SIZE:
        return _wgl_btn_cnt_get_min_size(pt_btn, (GL_SIZE_T*)pv_param1);

    case WGL_CNT_BTN_GET_NECESSARY_SIZE:
        return _wgl_btn_cnt_calc_necessary_width(pt_btn, WGL_POINTER_TO_UINT16(pv_param1), (UINT32*)pv_param2);
        
    case WGL_CNT_BTN_UPDATE_CVSST:
        return wgl_update_cvvst((WGL_CVS_CLR_CFG_T*)pv_param1, &pt_btn->t_cvsst);

    case WGL_CNT_BTN_SET_STATE:
        {
            pt_btn->ui2_state = WGL_POINTER_TO_UINT16(pv_param1);
            /*update dirty flag*/
            _wgl_btn_cnt_reset_dirty_flag(pt_btn);
            /*update font*/
            return _wgl_btn_cnt_update_font(pt_btn, &pt_btn->t_font_info_ex);
        } 
        
    default:
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
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
static INT32 _wgl_btn_cnt_paint_bk(
    WGL_BTN_CONTENT_T*              pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    GL_COLOR_T                          t_clr;
    INT32                               i4_ret;

    /*check paint type*/
    if(BTN_NO_BK & pt_data->ui2_extra_flag) /*no background*/
    {
        return WGLR_OK;
    }
    else if(!(BTN_IMG_UI & pt_data->ui2_extra_flag)) /*color style*/
    {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_btn_cnt_use_sys_bk_color(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT);
        }
        
        /*select color*/
        if(IS_STATE_PUSHED(ui2_state))
        {
            if(IS_STATE_FOCUSED(ui2_state))
            {
                t_clr = pt_data->t_clr_bk.t_push;
            }
            else
            {
                t_clr = pt_data->t_clr_bk.t_push_unhighlight;
            }
        }
        else if(IS_STATE_FOCUSED(ui2_state))
        {
            if(IS_STATE_ACTIVATED(ui2_state))
            {
                t_clr = pt_data->t_clr_bk.t_highlight;
            }
            else
            {
                t_clr = pt_data->t_clr_bk.t_highlight_inactive;
            }
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            t_clr = pt_data->t_clr_bk.t_enable;
        }
        else
        {
            t_clr = pt_data->t_clr_bk.t_disable;
        }

        /*should check BTN_NOT_CLEAN_BK style here*/
        
        /*paint background*/
        i4_ret = wgl_fill_widget_bk(h_graphics, &t_clr, &pt_data->t_cnt_rect, &pt_data->t_cvsst);
        WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to fill background color. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    else /*image style*/
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_btn_cnt_prepare_img(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT);
        }
        
        /*select image*/
        if(IS_STATE_PUSHED(ui2_state))
        {
            if(IS_STATE_FOCUSED(ui2_state))
            {
                h_image = pt_data->t_img_bk.t_push;
            }
            else
            {
                h_image = pt_data->t_img_bk.t_push_unhighlight;
            }
        }
        else if(IS_STATE_FOCUSED(ui2_state))
        {
            if(IS_STATE_ACTIVATED(ui2_state))
            {
                h_image = pt_data->t_img_bk.t_highlight;
            }
            else
            {
                h_image = pt_data->t_img_bk.t_highlight_inactive;
            }
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
                                       (WGL_IS_FLAG_TURN_ON(pt_data->ui2_extra_flag, BTN_NOT_CLEAN_BK)) ? WGL_TPL_PAINT_COMPOSE : WGL_TPL_PAINT_BITBLT);
            WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to draw image UI to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
static INT32 _wgl_btn_cnt_paint_fg(
    WGL_BTN_CONTENT_T*                  pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state,
    const GL_COLOR_T*                   pt_text_clr)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    INT32                               i4_ret;
    GL_COLOR_T                          t_empty_clr;
    UINT8                               ui1_cur_font_id;
    
    
    /*init empty color*/
    wgl_cvsst_get_transparent_color(&pt_data->t_cvsst, &t_empty_clr);
    /*select image*/
    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_data->t_fg_img.t_push;
        }
        else
        {
            h_image = pt_data->t_fg_img.t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            h_image = pt_data->t_fg_img.t_highlight;
        }
        else
        {
            h_image = pt_data->t_fg_img.t_highlight_inactive;
        }
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_image = pt_data->t_fg_img.t_enable;
    }
    else
    {
        h_image = pt_data->t_fg_img.t_disable;
    }
    
#if 0 /*for CR 2584*/
    /*check whether selected image exists*/
    if(NULL_HANDLE == h_image)
    {
        h_image = WGL_STD_GET_NON_NULL_IMG(&pt_data->t_fg_img);
    }
#endif
    i4_ret = x_fe_get_cur_font_resource(pt_data->t_text_font,&ui1_cur_font_id);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
	if (ui1_cur_font_id != pt_data->ui1_cur_font_id)
    {
  	   pt_data->b_is_data_dirty = TRUE;
  	   pt_data->ui1_cur_font_id = ui1_cur_font_id;
    }
    

    /*update layout info*/
    if(pt_data->b_is_data_dirty)
    {
        /*update image and text draw rectangle*/
        i4_ret = _wgl_btn_cnt_update_draw_rect(pt_data, h_image);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        
        /*turn off dirty flag*/
        pt_data->b_is_data_dirty = FALSE;
    }

    /*paint icon*/
    if((!WGL_RECT_IS_EMPTY(&pt_data->t_img_draw_rect)) && (NULL_HANDLE != h_image))
    {
        i4_ret = wgl_img_tpl_paint(h_image, 
                                   &pt_data->t_img_draw_rect, 
                                   pt_upt_rect, 
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to draw icon. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }

    /*paint string*/
    if(!WGL_RECT_IS_EMPTY(&pt_data->t_text_draw_rect) &&
       !wgl_cvsst_check_color_transparent(&pt_data->t_cvsst, pt_text_clr))
    {
        i4_ret = x_gl_draw_ustring_ex(h_graphics,
                                      pt_data->t_text_draw_rect.i4_left,
                                      pt_data->t_text_draw_rect.i4_top,
                                      0,
                                      pt_data->pt_w2_text,
                                      pt_data->ui2_text_dis_len,
                                      NULL,
                                      0,
                                      pt_data->t_text_font,
                                      pt_text_clr,
                                      &t_empty_clr,
                                      &t_empty_clr,
                                      (0 == pt_data->t_font_info.i2_width) ? FALSE : TRUE,
                                      (UINT16)(pt_data->t_font_info.i2_width),
                                      0,
                                      0,
                                      0);
        if(GLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to draw string. (%d) at L%d\n\n", i4_ret, __LINE__));
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
static INT32 _wgl_btn_cnt_update_draw_rect(
    WGL_BTN_CONTENT_T*                  pt_data, 
    WGL_HIMG_TPL_T                      h_image)
{
    /*local variable declaration*/
#if 1
    UTF16_T                             a_w2s_break_chars[] = WGL_CAN_BREAK_CHARS;
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
#else
    UTF16_T                             a_w2s_break_chars[BREAK_LINE_TOKEN_UTF16_MAX_LENGTH] = {0};
    UTF16_T                             a_w2s_force_chars[BREAK_LINE_TOKEN_UTF16_MAX_LENGTH] = {0};
#endif
    GL_RECT_T                           t_img_draw_rect = {0};
    GL_RECT_T                           t_text_draw_rect = {0};
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    GL_RECT_T                           t_available_rect;
    INT32                               i4_available_width, i4_available_height;
    INT32                               i4_img_width, i4_img_height;
    UINT32                              ui4_temp_width, ui4_temp_height;
    INT32                               i4_ret;                              
    
#if 0
    /*get UTF16 foramt for break and force characters*/
    i4_ret = wgl_get_break_line_token(a_w2s_break_chars, a_w2s_force_chars);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get break-line token. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
#endif
    
    /*calculate available rectangle for diaplay*/
    WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
    WGL_RECT_INSET(&t_available_rect, 
                   pt_data->t_fg_inset.i4_left, 
                   pt_data->t_fg_inset.i4_top,
                   pt_data->t_fg_inset.i4_right, 
                   pt_data->t_fg_inset.i4_bottom);

    i4_available_width = WGL_RECT_GET_WIDTH(&t_available_rect);
    i4_available_height = WGL_RECT_GET_HEIGHT(&t_available_rect);

    /*get image size*/
    if(h_image)
    {
        i4_ret = x_wgl_img_tpl_get_size(h_image, &ui4_temp_width, &ui4_temp_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get icon's size. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    else
    {
        ui4_temp_width = 0;
        ui4_temp_height = 0;
    }

    WGL_ASSERT(ui4_temp_width <= 0x7FFFFFFF);
    WGL_ASSERT(ui4_temp_height <= 0x7FFFFFFF);
    i4_img_width = (INT32)ui4_temp_width;
    i4_img_height = (INT32)ui4_temp_height;
    
    if(pt_data->ui2_text_length != 0)
    {
        /*available width for text*/
        t_break_line_Info.i4_box_width = i4_available_width - i4_img_width - pt_data->i4_obj_gap;

        /*Get text's necessary width*/
        if(t_break_line_Info.i4_box_width > 0)
        {
            t_break_line_Info.h_font = pt_data->t_text_font;
            t_break_line_Info.w2c_force_token = a_w2s_force_chars[0];
            t_break_line_Info.w2s_can_break = a_w2s_break_chars;
            t_break_line_Info.i4_string_length = (INT32)(pt_data->ui2_text_length);
            
            if (pt_data->ui1_text_direction == WGL_TEXT_DIR_LEFT_TO_RIGHT)
            {
                t_break_line_Info.w2s_string = pt_data->pt_w2_text;
                i4_ret = wgl_get_break_line_pos(&t_break_line_Info, NULL, &t_break_line_result);
                    
                if(i4_ret != WGLR_OK)
                {
                    DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }
				if (t_break_line_result.ui2_char_number - 1 >= 0)
				{
	                if (*(pt_data->pt_w2_text + t_break_line_result.ui2_char_number - 1) == a_w2s_force_chars[0])
	                {
	                    t_break_line_result.ui2_char_number --;
	                }
                }
            }
            else
            {
                t_break_line_Info.w2s_string = pt_data->pt_w2_text + pt_data->ui2_text_length - 1;
                i4_ret = wgl_get_break_line_pos_for_arab(
                    t_break_line_Info.w2s_string, 
                    &t_break_line_Info, 
                    NULL, 
                    &t_break_line_result);

                if(i4_ret != WGLR_OK)
                {
                    DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }

                if (*(pt_data->pt_w2_text + pt_data->ui2_text_length - t_break_line_result.ui2_char_number) == a_w2s_force_chars[0])
                {
                    t_break_line_result.ui2_char_number --;
                }
            }
        }
    }

    /*store text layout info*/
    pt_data->ui2_text_dis_len = t_break_line_result.ui2_char_number;
    pt_data->ui2_text_dis_width = t_break_line_result.ui2_width;
 
    /*calculate start position in x-axis*/
    if(WGL_IS_FLAG_TURN_ON(pt_data->ui2_extra_flag, BTN_DIS_TEXT_FIRST))
    {          
            if(WGL_IS_H_LEFT(pt_data->ui1_align))
            {
                t_text_draw_rect.i4_left = (pt_data->b_opposite_align) ? 
                    (i4_available_width - (pt_data->ui2_text_dis_width + pt_data->i4_obj_gap + i4_img_width)) : 
                    (0);
            }
            else if(WGL_IS_H_CENTER(pt_data->ui1_align))
            {
                t_text_draw_rect.i4_left = 
                    (i4_available_width - (pt_data->ui2_text_dis_width + pt_data->i4_obj_gap + i4_img_width)) >> 1;
            }
            else
            {
                t_text_draw_rect.i4_left = (pt_data->b_opposite_align) ? 
                    (0) :
                    (i4_available_width - (pt_data->ui2_text_dis_width + pt_data->i4_obj_gap + i4_img_width));
            }
            t_img_draw_rect.i4_left = t_text_draw_rect.i4_left + pt_data->ui2_text_dis_width + pt_data->i4_obj_gap;        
    }
    else
    {
       
            if(WGL_IS_H_LEFT(pt_data->ui1_align))
            {
                t_img_draw_rect.i4_left = (pt_data->b_opposite_align) ? 
                    (i4_available_width - (i4_img_width + pt_data->i4_obj_gap + pt_data->ui2_text_dis_width)) :
                    (0);
            }
            else if(WGL_IS_H_CENTER(pt_data->ui1_align))
            {
                t_img_draw_rect.i4_left = 
                    (i4_available_width - (i4_img_width + pt_data->i4_obj_gap + pt_data->ui2_text_dis_width)) >> 1;
            }
            else
            {
                t_img_draw_rect.i4_left = (pt_data->b_opposite_align) ? 
                    (0) :
                    (i4_available_width - (i4_img_width + pt_data->i4_obj_gap + pt_data->ui2_text_dis_width));
            }
            t_text_draw_rect.i4_left = t_img_draw_rect.i4_left + i4_img_width + pt_data->i4_obj_gap;
       
    }

    {   /*[DTV00068091]
              the height will be changed even if the same font handle when language info is changed.
              so the font height info should be gotten again*/
        INT16 i2_height;
        i4_ret = x_fe_get_height(pt_data->t_text_font, &i2_height);
        if(FER_OK == i4_ret && i2_height >= 0)
        {
            pt_data->ui2_line_height = (UINT16)i2_height;
        }
    }

    /*calculate start position in y-axis*/
    if(WGL_IS_V_TOP(pt_data->ui1_align))
    {
        t_img_draw_rect.i4_top = 0;
        t_text_draw_rect.i4_top = 0;
    }
    else if(WGL_IS_V_CENTER(pt_data->ui1_align))
    {
        t_img_draw_rect.i4_top = (i4_available_height - i4_img_height) >> 1;
        t_text_draw_rect.i4_top = (i4_available_height - pt_data->ui2_line_height) >> 1;
    }
    else
    {
        t_img_draw_rect.i4_top = i4_available_height - i4_img_height;
        t_text_draw_rect.i4_top = i4_available_height - pt_data->ui2_line_height;
    }

    /*set draw width and height*/
    t_img_draw_rect.i4_right = t_img_draw_rect.i4_left + i4_img_width;
    t_img_draw_rect.i4_bottom = t_img_draw_rect.i4_top + i4_img_height;
    t_text_draw_rect.i4_right = t_text_draw_rect.i4_left + pt_data->ui2_text_dis_width;
    t_text_draw_rect.i4_bottom = t_text_draw_rect.i4_top + pt_data->ui2_line_height;
    
    /*calculate valid text rectange*/
    if(pt_data->ui2_text_dis_width && pt_data->ui2_line_height)
    {
        WGL_RECT_OFFSET(&t_text_draw_rect, t_available_rect.i4_left, t_available_rect.i4_top);
        WGL_RECT_COPY(&pt_data->t_text_draw_rect,&t_text_draw_rect);
        /*remove x_wgl_rect_intersect is for fix issue:
                 when font height > widget's height then in verticle center/bottom alignment, t_text_draw_rect.i4_top < 0, thus intersect will let pt_data->t_text_draw_rect.i4_top >=0 
                 so text rendering is toward down direction and ui effect is wrong
             */

#if 0
        i4_ret = x_wgl_rect_intersect(&pt_data->t_text_draw_rect, &t_available_rect, &t_text_draw_rect);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get the intersection of the available rect and text rect for drawing string. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
#endif
    }
    else
    {
        WGL_RECT_EMPTY(&pt_data->t_text_draw_rect);
    }

    if(i4_img_width && i4_img_height)
    {
        WGL_RECT_OFFSET(&t_img_draw_rect, t_available_rect.i4_left, t_available_rect.i4_top);

        i4_ret = x_wgl_rect_intersect(&pt_data->t_img_draw_rect, &t_available_rect, &t_img_draw_rect);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get the intersection of the available rect and text rect for drawing string. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    else
    {
        WGL_RECT_EMPTY(&pt_data->t_img_draw_rect);
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name     _wgl_btn_cnt_calc_necessary_width
 *          
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_cnt_calc_necessary_width(
    WGL_BTN_CONTENT_T*                  pt_data, 
    UINT16                              ui2_state,
    UINT32*                             pui4_width)
{
    /*local variable declaration*/
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UINT32                              ui4_img_width, ui4_img_height, ui4_text_width;
    INT32                               i4_ret, i4_adv_x, i4_adv_y;
    WGL_HIMG_TPL_T                      h_image;

    if(NULL == pui4_width)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CNT_BTN_GET_NECESSARY_SIZE, the 2st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
        
    /*select image*/
    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_data->t_fg_img.t_push;
        }
        else
        {
            h_image = pt_data->t_fg_img.t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            h_image = pt_data->t_fg_img.t_highlight;
        }
        else
        {
            h_image = pt_data->t_fg_img.t_highlight_inactive;
        }
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_image = pt_data->t_fg_img.t_enable;
    }
    else
    {
        h_image = pt_data->t_fg_img.t_disable;
    }

    /*get image size*/
    if(h_image)
    {
        i4_ret = x_wgl_img_tpl_get_size(h_image, &ui4_img_width, &ui4_img_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get icon's size. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    else
    {
        ui4_img_width = 0;
    }

    WGL_ASSERT(ui4_img_width <= 0x7FFFFFFF);
    
    if(pt_data->ui2_text_length != 0)
    {
        /*available width for text*/
        t_break_line_Info.i4_box_width = 0x7FFFFFFF;

        /*Get text's necessary width*/
        t_break_line_Info.h_font = pt_data->t_text_font;
        t_break_line_Info.w2c_force_token = a_w2s_force_chars[0];
        t_break_line_Info.w2s_can_break = NULL;  
        t_break_line_Info.i4_string_length = (INT32)(pt_data->ui2_text_length);
        
        if (pt_data->ui1_text_direction == WGL_TEXT_DIR_LEFT_TO_RIGHT)
        {
            t_break_line_Info.w2s_string = pt_data->pt_w2_text;
            i4_ret = wgl_get_break_line_pos(&t_break_line_Info, NULL, &t_break_line_result);
                    
            if(i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        else
        {
            t_break_line_Info.w2s_string = pt_data->pt_w2_text + pt_data->ui2_text_length - 1;
            i4_ret = wgl_get_break_line_pos_for_arab(
                t_break_line_Info.w2s_string, 
                &t_break_line_Info, 
                NULL, 
                &t_break_line_result);
        
            if(i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }  
        }
        
        ui4_text_width = t_break_line_result.ui2_width;
        if (pt_data->ui1_text_direction == WGL_TEXT_DIR_RIGHT_TO_LEFT)
        {
            if (*(pt_data->pt_w2_text + pt_data->ui2_text_length - pt_data->ui2_text_dis_len) == a_w2s_force_chars[0])
            {
                i4_ret = x_fe_get_character_size(pt_data->t_text_font, (CHAR_CODE)a_w2s_force_chars[0], &i4_adv_x, &i4_adv_y);
                ui4_text_width += i4_adv_x;  
            }
        }
    }
    else
    {
        ui4_text_width = 0;
    }
    
    *pui4_width = ui4_text_width + ui4_img_width + pt_data->i4_obj_gap + pt_data->t_fg_inset.i4_left + pt_data->t_fg_inset.i4_right;
    
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
static INT32 _wgl_btn_cnt_set_img(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    UINT8                       ui1_target,
    const WGL_IMG_INFO_T*       pt_img_info)
{
    
    DBG_INFO(("{WGL(BTN_CNT)} pt_img_info = 0x%.8x   e_type = 0x%.8x\n"
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
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK == ui1_target)
    {
        /*update background image*/
        if(pt_img_info)
        {
            COPY_IMG_INFO_TO_EXT(&pt_cnt_data->t_img_bk, pt_img_info, NULL_HANDLE);

            /*remove the flag for action image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT);
        }
        else
        {
            x_memset(&pt_cnt_data->t_img_bk, 0, sizeof(WGL_IMG_EXT_SET_T));

            /*reset the flag for bk image*/
            WGL_SET_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT);
        }
    }
    else if(WGL_IMG_FG == ui1_target)
    {
        /*update btn image*/
        if(pt_img_info)
        {
            COPY_IMG_INFO_TO_EXT(&pt_cnt_data->t_fg_img, pt_img_info, NULL_HANDLE);
            _wgl_btn_cnt_reset_dirty_flag(pt_cnt_data);
        }
        else
        {
            x_memset(&pt_cnt_data->t_fg_img, 0, sizeof(WGL_IMG_EXT_SET_T));
        }
    }
    else
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK or WGL_IMG_FG. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_btn_cnt_get_img(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    UINT8                       ui1_target,
    WGL_IMG_INFO_T*             pt_img_info)
{
    /*local variable declaration*/
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL == pt_img_info)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK == ui1_target)
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_btn_cnt_prepare_img(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
        
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT);
        }

        /*prepare return value*/
        SET_EXTEND_TO_IMG_INFO(pt_img_info, &pt_cnt_data->t_img_bk);
    }
    else if(WGL_IMG_FG == ui1_target)
    {
        /*prepare return value*/
        SET_EXTEND_TO_IMG_INFO(pt_img_info, &pt_cnt_data->t_fg_img);
    }
    else
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK or WGL_IMG_FG. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_btn_cnt_set_text(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    UINT16                      ui2_length,
    const UTF16_T*              w2s_str)
{
    /*local variable declaration*/
    INT32                       i4_size;
    UINT16                      ui2_max_len;
    UINT16                      ui2_overflow_len;

    if(NULL == w2s_str)
    {
        /*reset user string*/
        x_memset(pt_cnt_data->pt_w2_text, 0, pt_cnt_data->ui2_text_length * sizeof(UTF16_T));
        pt_cnt_data->ui2_text_length = 0;
    }
    else
    {
        /*get length*/
        ui2_max_len = _BTN_GET_MAX_LENGTH(pt_cnt_data->ui2_extra_flag);
        i4_size = (INT32)x_uc_w2s_strlen(w2s_str);
        ui2_length = (ui2_length > (UINT16)i4_size) ? (UINT16)i4_size : ui2_length;
        ui2_overflow_len = 0;
        /*check length*/
        if(ui2_length > ui2_max_len)
        {
            ui2_overflow_len = ui2_length - ui2_max_len;
            ui2_length = ui2_max_len;  /*reserved buffer is MAX_LENGTH + 1*/
            /*return WGLR_BUFFER_NOT_ENOUGH;*/
        }
        
        /*copy string*/
        if (pt_cnt_data->ui1_text_direction == WGL_TEXT_DIR_LEFT_TO_RIGHT)
        {
            x_memcpy(pt_cnt_data->pt_w2_text, w2s_str, ui2_length * sizeof(UTF16_T));
        }
        else
        {
            x_memcpy(pt_cnt_data->pt_w2_text, w2s_str + ui2_overflow_len, ui2_length * sizeof(UTF16_T));
        }
        /*clean old data*/
        if(ui2_length < pt_cnt_data->ui2_text_length)
        {
            x_memset(pt_cnt_data->pt_w2_text + ui2_length, 0, sizeof(UTF16_T) * (pt_cnt_data->ui2_text_length - ui2_length));
        }
        
        /*keep length*/
        pt_cnt_data->ui2_text_length = ui2_length;

		if(check_exist_arabic(pt_cnt_data->pt_w2_text,ui2_length))
	    {		
	        UTF16_T *w2s_dst;
            UTF16_T *w2s_src;
			x_memset(pt_cnt_data->pt_w2_arab_dsp_text,0,(WGL_BTN_MAX_STRING + 1) * sizeof(UTF16_T));
	        reorder_arabic(pt_cnt_data->pt_w2_text,ui2_length,NULL,pt_cnt_data->pt_w2_arab_dsp_text,NULL);   
			w2s_dst = pt_cnt_data->pt_w2_text; 
			w2s_src = pt_cnt_data->pt_w2_arab_dsp_text; 
		    while((*(w2s_dst++) = *(w2s_src++)) != 0) {     
		    }  
	    }
    }
    
    /*update dirty flag*/
    _wgl_btn_cnt_reset_dirty_flag(pt_cnt_data);

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
static INT32 _wgl_btn_cnt_set_color(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    UINT8                       ui1_target,
    const WGL_COLOR_INFO_T*     pt_clr_info)
{
    /*local variable variable*/
    GL_COLOR_T                  t_default_color;

    DBG_INFO(("{WGL(BTN_CNT)} pt_clr_info = 0x%.8x   e_type = 0x%.8x\n"
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
            COPY_CLR_INFO_TO_EXT(&pt_cnt_data->t_clr_bk, pt_clr_info, t_default_color);

            /*remove the flag for bk color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for bk color*/
            WGL_SET_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT);
        }
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(pt_clr_info)
        {
            /*copy text color*/
            COPY_CLR_INFO_TO_EXT(&pt_cnt_data->t_clr_text, pt_clr_info, t_default_color);

            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT);
        }
        else
        {
            /*reset the flag for text color*/
            WGL_SET_FLAG(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT);
        }
    }
    else
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_btn_cnt_get_color(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    UINT8                       ui1_target,
    WGL_COLOR_INFO_T*           pt_clr_info)
{
    /*local variable declaration*/
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL == pt_clr_info)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid input parameter. For WGL_CMD_GL_GET_COLOR, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check target*/
    if(WGL_CLR_BK == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_btn_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            /*remove the flag for bk color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT);
        }

        /*prepare return value*/
        SET_EXTEND_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_bk);
    }
    else if(WGL_CLR_TEXT == ui1_target)
    {
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT))
        {
            /*get system colors*/
            i4_ret = _wgl_btn_cnt_use_sys_text_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
            /*remove the flag for text color*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT);
        }

        /*prepare return value*/
        SET_EXTEND_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_text);
    }
    else
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK or WGL_CLR_TEXT. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
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
static INT32 _wgl_btn_cnt_set_text_dir(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT8                               ui1_text_dir)
{
    /*INT32 i4_ret;*/
    
    if (ui1_text_dir != WGL_TEXT_DIR_LEFT_TO_RIGHT && ui1_text_dir != WGL_TEXT_DIR_RIGHT_TO_LEFT)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to set text direction. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    else
    {
        if (!(pt_cnt_data->ui2_extra_flag & BTN_FIX_TEXT_DIR)
			&& (pt_cnt_data->ui1_text_direction != ui1_text_dir)
			)
        {
            pt_cnt_data->ui1_text_direction = ui1_text_dir;
            if(!(pt_cnt_data->ui2_extra_flag & BTN_FIX_TEXT_DIR_ALIGN))
            {
                pt_cnt_data->b_opposite_align = (pt_cnt_data->b_opposite_align) ? FALSE : TRUE;
            }
            /*pt_cnt_data->t_text_draw_rect need update*/
            pt_cnt_data->b_is_data_dirty = TRUE;
            /*AP will reset the text with the opposite direction*/
            /*
            i4_ret = x_uc_w2s_reverse(pt_cnt_data->w2_text);
            if (i4_ret != UCR_OK)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to set text direction. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            */
            
            /*update dirty flag*/
            /*
            _wgl_btn_cnt_reset_dirty_flag(pt_cnt_data);
            */
        }
        return WGLR_OK;
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
static INT32 _wgl_btn_cnt_get_text_dir(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT32*                             pui4_text_dir)
{
    if (pui4_text_dir == NULL)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get text direction. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    *pui4_text_dir = (UINT32)pt_cnt_data->ui1_text_direction;
    return WGLR_OK;
}

static INT32 _wgl_btn_cnt_get_render_cond(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    UINT16                              ui2_state,
    UINT8*                              pui1_render_cond)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    INT32                               i4_ret =WGLR_OK;
    GL_COLOR_T                          t_empty_clr,t_clr,t_text_clr;
    UINT8                               ui1_fg_render_cond;
    if (pui1_render_cond == NULL)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get render condition. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
     wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_empty_clr);
    /*select image*/
    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_cnt_data->t_fg_img.t_push;
        }
        else
        {
            h_image = pt_cnt_data->t_fg_img.t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            h_image = pt_cnt_data->t_fg_img.t_highlight;
        }
        else
        {
            h_image = pt_cnt_data->t_fg_img.t_highlight_inactive;
        }
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_image = pt_cnt_data->t_fg_img.t_enable;
    }
    else
    {
        h_image = pt_cnt_data->t_fg_img.t_disable;
    }
    _wgl_btn_cnt_select_text_clr(pt_cnt_data, ui2_state, &t_text_clr);

    if( NULL_HANDLE != h_image)
    {
        ui1_fg_render_cond = WGL_RENDER_TRANS_OPAQUE;
    }
    else if(!WGL_RECT_IS_EMPTY(&pt_cnt_data->t_text_draw_rect) &&
       !wgl_cvsst_check_color_transparent(&pt_cnt_data->t_cvsst, &t_text_clr))
    {
        ui1_fg_render_cond = WGL_RENDER_TRANS_OPAQUE;            
    }else
    {
        ui1_fg_render_cond =  WGL_RENDER_TRANS;
    }
    if(BTN_NO_BK & pt_cnt_data->ui2_extra_flag)//no background
    {
        *pui1_render_cond = ui1_fg_render_cond;       
    }
    else if(!(BTN_IMG_UI & pt_cnt_data->ui2_extra_flag)) /*color style*/
    {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_btn_cnt_use_sys_bk_color(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_CLR_NOT_INIT);
        }
        
        /*select color*/
        if(IS_STATE_PUSHED(ui2_state))
        {
            if(IS_STATE_FOCUSED(ui2_state))
            {
                t_clr = pt_cnt_data->t_clr_bk.t_push;
            }
            else
            {
                t_clr = pt_cnt_data->t_clr_bk.t_push_unhighlight;
            }
        }
        else if(IS_STATE_FOCUSED(ui2_state))
        {
            if(IS_STATE_ACTIVATED(ui2_state))
            {
                t_clr = pt_cnt_data->t_clr_bk.t_highlight;
            }
            else
            {
                t_clr = pt_cnt_data->t_clr_bk.t_highlight_inactive;
            }
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
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_btn_cnt_prepare_img(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_BK_IMG_NOT_INIT);
        }
        
        /*select image*/
        if(IS_STATE_PUSHED(ui2_state))
        {
            if(IS_STATE_FOCUSED(ui2_state))
            {
                h_image = pt_cnt_data->t_img_bk.t_push;
            }
            else
            {
                h_image = pt_cnt_data->t_img_bk.t_push_unhighlight;
            }
        }
        else if(IS_STATE_FOCUSED(ui2_state))
        {
            if(IS_STATE_ACTIVATED(ui2_state))
            {
                h_image = pt_cnt_data->t_img_bk.t_highlight;
            }
            else
            {
                h_image = pt_cnt_data->t_img_bk.t_highlight_inactive;
            }
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
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_cnt_set_font(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    const WGL_FONT_INFO_T*      pt_font_info)
{
    WGL_FONT_INFO_EX_T          t_ex_font_info;

    DBG_INFO(("{WGL(BTN_CNT)} pt_font_info = 0x%.8x   e_font_cmap =  0x%.8x\n"
                             "\te_font_size = 0x%.8x   e_font_style = 0x%.8x\n"
                             "\tpt_font_info->a_c_font_name = %s\n",
                             pt_font_info,
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_cmap),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_size),
                             ((NULL == pt_font_info) ? 0 : pt_font_info->e_font_style),
                             ((NULL == pt_font_info) ? '\0' : pt_font_info->a_c_font_name)));

    /*update the extend font info*/
    if (pt_font_info)
    {
        t_ex_font_info.e_type = WGL_FONT_SET_BASIC;
        x_memcpy(&t_ex_font_info.u_font_data.t_basic.t_focused, pt_font_info, sizeof(WGL_FONT_INFO_T));
        x_memcpy(&t_ex_font_info.u_font_data.t_basic.t_unfocused, pt_font_info, sizeof(WGL_FONT_INFO_T));

        return _wgl_btn_cnt_set_font_ex(pt_cnt_data, &t_ex_font_info);
    }
    else
    {
        return _wgl_btn_cnt_set_font_ex(pt_cnt_data, NULL);
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_CMD_GL_GET_FONT command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_cnt_get_font(
    WGL_BTN_CONTENT_T*          pt_cnt_data,
    WGL_FONT_INFO_T*            pt_font_info)
{
    /*local variable declaration*/
    WGL_FONT_INFO_T             t_font_info;
    INT32 i4_ret;
    
    /*check parameter*/
    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_FONT_NOT_INIT))
    {
        /*get system font*/
        i4_ret = x_wgl_get_system_font(&t_font_info);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system font info. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        i4_ret = _wgl_btn_cnt_set_font(pt_cnt_data, &t_font_info);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    /*prepare return value*/
    x_memcpy(pt_font_info, &pt_cnt_data->t_font_info, sizeof(WGL_FONT_INFO_T));

    return WGLR_OK;
}

static INT32 _wgl_btn_cnt_set_font_ex(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_ex_font_info)
{
    INT32                       i4_ret;
    BOOL                        b_update_ok = FALSE;


	if(pt_cnt_data == NULL || pt_ex_font_info == NULL)
	{
		return WGLR_INV_ARG;
	}
    /*update dirty flag*/
    _wgl_btn_cnt_reset_dirty_flag(pt_cnt_data);
    /*update the extend font info*/
    x_memcpy(&pt_cnt_data->t_font_info_ex, pt_ex_font_info, sizeof(WGL_FONT_INFO_EX_T));
    
    do
    {
        if(pt_ex_font_info && pt_ex_font_info->e_type == WGL_FONT_SET_BASIC)
        {
            /*update pt_cnt_data->t_text_font*/
            i4_ret = _wgl_btn_cnt_update_font(pt_cnt_data, pt_ex_font_info);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            /*remove the flag for font*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_FONT_NOT_INIT);
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

                DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to free old font handle(%d). (HR=%d) at L%d\n\n", pt_cnt_data->t_text_font, i4_ret, __LINE__));
                i4_ret = WGLR_SYS_RC_CANNOT_FREE;
            }

            pt_cnt_data->t_text_font = NULL_HANDLE;
        }
        
        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui2_extra_flag, BTN_FONT_NOT_INIT);
    }
    
    return i4_ret;
}

static INT32 _wgl_btn_cnt_get_font_ex(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    WGL_FONT_INFO_EX_T*                 pt_ex_font_info)
{
    /*check parameter*/
    if(NULL == pt_ex_font_info)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_FONT_EX, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    x_memcpy(pt_ex_font_info, &pt_cnt_data->t_font_info_ex, sizeof(WGL_FONT_INFO_EX_T));
    
    return WGLR_OK;
}

static INT32 _wgl_btn_cnt_update_font(
    WGL_BTN_CONTENT_T*                  pt_cnt_data,
    const WGL_FONT_INFO_EX_T*           pt_ex_font_info)
{
    INT32                      i4_ret;
    INT16                      i2_height;
    const WGL_FONT_INFO_T*     pt_font_info;

    if (pt_cnt_data->ui2_state & WGL_STATE_FOCUSED)
    {
        pt_font_info = &pt_ex_font_info->u_font_data.t_basic.t_focused;
    }
    else
    {
        pt_font_info = &pt_ex_font_info->u_font_data.t_basic.t_unfocused;
    }

    i4_ret = wgl_update_font_handle(&pt_cnt_data->t_font_info, &pt_cnt_data->t_text_font, pt_font_info);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to update font handle. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    /*get font height*/
    i4_ret = x_fe_get_height(pt_cnt_data->t_text_font, &i2_height);
    if(FER_OK != i4_ret || i2_height < 0)
    {
        WGL_ASSERT(0);

        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get font info (handle = %d). (FER=%d) at L%d\n\n", pt_cnt_data->t_text_font, i4_ret, __LINE__));
        i4_ret = WGLR_GET_FONT_INFO_FAIL;
        return i4_ret;
    }
    pt_cnt_data->ui2_line_height = (UINT16)i2_height;

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_CMD_BTN_GET_MIN_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_cnt_get_min_size(
    const WGL_BTN_CONTENT_T*    pt_cnt_data,
    GL_SIZE_T*                  pt_size)
{
    /*local variable declaration*/
    UINT32                      ui4_width = 0;
    UINT32                      ui4_height = 0;
    INT32                       i4_ret;
    WGL_HIMG_TPL_T              h_img;

    if(NULL == pt_size)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Invalid arguments. For WGL_CNT_BTN_GET_MIN_SIZE, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    /*inset size*/
    pt_size->ui4_width = (UINT32)(pt_cnt_data->t_fg_inset.i4_left + pt_cnt_data->t_fg_inset.i4_right);
    pt_size->ui4_height = (UINT32)(pt_cnt_data->t_fg_inset.i4_top + pt_cnt_data->t_fg_inset.i4_bottom);

    /*calculate the foreground btn size*/
    h_img = WGL_STD_GET_NON_NULL_IMG(&pt_cnt_data->t_fg_img);

    if(h_img)
    {
        i4_ret = x_wgl_img_tpl_get_size(h_img, &ui4_width, &ui4_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get image size(handle = %d). (%d) at L%d\n\n", h_img, i4_ret, __LINE__));
            return i4_ret;
        }

        pt_size->ui4_width += ui4_width;
        pt_size->ui4_height += ui4_height;
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
static INT32 _wgl_btn_cnt_prepare_img(
    WGL_BTN_CONTENT_T*      pt_cnt_data)
{
    /*local variable declaration*/
    WGL_IMG_ST_T            e_segment_type;
    UINT32                  ui4_width;
    UINT32                  ui4_height;
    WGL_HIMG_TPL_T          h_tpl_img;
    INT32                   i4_ret;

    /*get system enable image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_enable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_BTN, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_IMG_BTN image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_enable = h_tpl_img;
    }

    /*get system disable image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_disable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_BTN_DIS, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_IMG_BTN_DIS image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_disable = h_tpl_img;
    }

    /*get system highlight image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_highlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_BTN_HT, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_IMG_BTN_HT image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_highlight = h_tpl_img;
        pt_cnt_data->t_img_bk.t_highlight_inactive = h_tpl_img;
    }

    /*get system pushed image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_push) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_BTN_PUSHED, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_IMG_BTN_PUSHED image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_push = h_tpl_img;
    }

    /*get system pushed-unhighlighted image for btn*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_push_unhighlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_BTN_PUSHED_UNHT, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_IMG_BTN_PUSHED_UNHT image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        pt_cnt_data->t_img_bk.t_push_unhighlight = h_tpl_img;
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
static INT32 _wgl_btn_cnt_use_sys_bk_color(
    WGL_BTN_CONTENT_T*      pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                   i4_ret;

    /*get system enable color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_BTN, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_BTN color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_BTN_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_BTN_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_BTN_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_highlight);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_BTN_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    pt_cnt_data->t_clr_bk.t_highlight_inactive = pt_cnt_data->t_clr_bk.t_highlight;

    /*get system pushed color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_BTN_PUSHED, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_push);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_BTN_PUSHED color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system pushed-unhighlight color for btn*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_BTN_PUSHED_UNHT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_push_unhighlight);
    WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_BTN_PUSHED_UNHT color. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
static INT32 _wgl_btn_cnt_use_sys_text_color(
    WGL_BTN_CONTENT_T*      pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                   i4_ret;

    /*get system enable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_highlight);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_HT color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    pt_cnt_data->t_clr_text.t_highlight_inactive = pt_cnt_data->t_clr_text.t_highlight;

    /*get system pushed color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_PUSHED, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_push);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_PUSHED color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system pushed-unhighlight color for text*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_TEXT_PUSHED_UNHT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_text.t_push_unhighlight);
    WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN_CNT)} ERR: Fail to get system WGL_SYS_CLR_TEXT_PUSHED_UNHT color. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
static INT32 _wgl_btn_cnt_select_text_clr(
    WGL_BTN_CONTENT_T*      pt_cnt_data, 
    UINT16                  ui2_state,
    GL_COLOR_T*             pt_selected_clr)
{
    /*local variable declaration*/
    INT32                   i4_ret;

    /*check if color infos are ready*/
    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_btn_cnt_use_sys_text_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        /*remove the flag for text color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui2_extra_flag, BTN_TEXT_CLR_NOT_INIT);
    }

    /*select color*/
    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            *pt_selected_clr = pt_cnt_data->t_clr_text.t_push;
        }
        else
        {
            *pt_selected_clr = pt_cnt_data->t_clr_text.t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            *pt_selected_clr = pt_cnt_data->t_clr_text.t_highlight;
        }
        else
        {
            *pt_selected_clr = pt_cnt_data->t_clr_text.t_highlight_inactive;
        }
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
static void _wgl_btn_cnt_reset_dirty_flag(
    WGL_BTN_CONTENT_T*      pt_cnt_data)
{
    pt_cnt_data->b_is_data_dirty = TRUE;
}

