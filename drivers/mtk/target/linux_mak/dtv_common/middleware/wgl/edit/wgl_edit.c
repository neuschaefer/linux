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
 * $RCSfile: wgl_edit.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/7 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 9b7a5fcc6320941c0f8349fa163d4954 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "unicode/x_uc_str.h"
#include "wgl/edit/x_wgl_edit.h"
#include "wgl/edit/wgl_edit.h"
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

#define _WGL_EB_DEF_KEY_CURSOR_LEFT     BTN_CURSOR_LEFT
#define _WGL_EB_DEF_KEY_CURSOR_RIGHT    BTN_CURSOR_RIGHT
#define _WGL_EB_DEF_KEY_BACKSPACE       BTN_CURSOR_DOWN
#define _WGL_EB_DEF_KEY_SW_INSERT       BTN_CURSOR_UP
#define _WGL_EB_DEF_KEY_ENTER_EDIT      BTN_SELECT
#define _WGL_EB_DEF_KEY_LEAVE_EDIT      BTN_EXIT

/**/
#define _WGL_EB_UTF16_DIGIT_0           ((UTF16_T)0x0030)
#define _WGL_EB_UTF16_ALPHA_A           ((UTF16_T)0x0041)
#define _WGL_EB_UTF16_ALPHA_a           ((UTF16_T)0x0061)
#define _WGL_EB_UTF16_CHAR_DOT          ((UTF16_T)0x002E)
#define _WGL_EB_UTF16_CHAR_SPACE        ((UTF16_T)0x0020)

/*macro for extension attribute*/
#define _WGL_EB_EXT_ATTR_IS_AUTO_EDITING(_ui2_flag)      ((_ui2_flag) & WGL_EB_EA_AUTO_EDITING)
#define _WGL_EB_EXT_ATTR_IS_CHAR_SUPPORT(_ui2_flag)      ((_ui2_flag) & WGL_EB_CHAR_SUPPORT)
#define _WGL_EB_EXT_ATTR_IS_ALWAYS_HT(_ui2_flag)         ((_ui2_flag) & WGL_EB_ALWAYS_HT)
#define _WGL_EB_EXT_ATTR_IS_KEEP_CURSOR_POS(_ui2_flag)         ((_ui2_flag) & WGL_EB_KEEP_CURSOR_POS)

/**/
typedef enum _WGL_EB_NFY_TYPE_T
{
    _WGL_EB_NFY_TYPE_NONE = 0,
    _WGL_EB_NFY_TYPE_KEY,
    _WGL_EB_NFY_TYPE_DATA_CHANGE,
    _WGL_EB_NFY_TYPE_OVER_MAX
} WGL_EB_NFY_TYPE_T;

typedef struct _WGL_EB_CONTROL_LOGIC_T
{
#ifdef CLI_LVL_ALL     
    UINT32                              ui4_mem_size;/*this field must be the first field in the structure*/
#endif  
    VOID*                               pv_attach;          /*attached data*/
    WGL_CONTENT_CTRL_INTF_T             t_cnt_intf;         /*content interface*/
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data;         /*content object*/
    WGL_BORDER_CTRL_INTF_T              t_bdr_intf;         /*border interface*/
    WGL_BORDER_CONTEXT_HANDLE_T         h_bdr_data;         /*border object*/
    WGL_EB_KEY_MAP_T                    t_key_map;          /*key map for control edit-box*/
    WGL_EB_MODE_T                       e_mode;             /*insert / replace mode*/
    UINT32                              ui4_style;          /*widget style*/
    UINT16                              ui2_state;
    UINT16                              ui2_cur_pos;        /*current cursor position. Zero-based*/
    UINT16                              ui2_input_length;   /*current input length*/
    UINT16                              ui2_input_limit;    /*input capacity*/
    UINT16                              ui2_ext_attr;       /*extension attribute*/
    BOOL                                b_change_flag;
    BOOL                                b_editing;          /*TRUE for editing, and FALSE for navigation*/
} WGL_EB_CONTROL_LOGIC_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*WGL_MSG_PAINT message handler*/
static INT32 _wgl_eb_msg_hdr_paint(
    HANDLE_T                        h_widget,
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state);

/*WGL_MSG_KEY_DOWN & WGL_MSG_KEY_UP message handler*/
static INT32 _wgl_eb_msg_hdr_key(
    HANDLE_T                            h_widget,
    UINT32                              ui4_k_code,
    UINT32                              ui4_k_state);

/*WGL_MSG_GET_FOCUS message handler*/
static INT32 _wgl_eb_msg_hdr_get_focus(
    HANDLE_T                           h_widget);

/*WGL_MSG_LOSE_FOCUS message handler*/
static INT32 _wgl_eb_msg_hdr_lose_focus(
    HANDLE_T                            h_widget);

/*WGL_MSG_SIZE message handler*/
static INT32 _wgl_eb_msg_hdr_size(
    HANDLE_T                            h_widget,
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT16                              ui2_width,
    UINT16                              ui2_height);

/*WGL_MSG_COMMAND message handler*/
static INT32 _wgl_eb_msg_hdr_command(
    HANDLE_T                            h_widget,
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT32                              ui4_command_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2,
    VOID*                               pv_internal_cmd_data);

static INT32 _wgl_eb_cmd_hdr_set_input_limit(
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT16                              ui2_limit);

static INT32 _wgl_eb_cmd_hdr_set_cur_pos(
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT16                              ui2_new_pos);

static INT32 _wgl_eb_cmd_hdr_set_key_map(
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    const WGL_EB_KEY_MAP_T*             pt_key_map);

static INT32 _wgl_eb_cmd_hdr_set_text(
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT16                              ui2_string_length,
    const UTF16_T*                      w2s_string);

static INT32 _wgl_eb_cmd_hdr_get_text(
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT16                              ui2_string_length,
    UTF16_T*                            w2s_string);

static INT32 _wgl_eb_change_cnt_rect(
    const WGL_EB_CONTROL_LOGIC_T*     pt_edit,
    const GL_RECT_T*                  pt_widget_rect,
    BOOL                              b_new_inset,
    const WGL_INSET_T*                pt_inset);

static INT32 _wgl_eb_process_key_event(
    HANDLE_T                            h_widget,
    WGL_EB_CONTROL_LOGIC_T*             pt_edit,
    UINT32                              ui4_k_code,
    UINT32                              ui4_k_state,
    WGL_EB_NFY_TYPE_T*                  pe_notify_flag,
    BOOL                                b_disable_repaint);

static INT32 _wgl_eb_notify_parent(
    HANDLE_T                            h_widget,
    WGL_EB_NFY_TYPE_T                   e_type);

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_eb_init(
    UINT32                      ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_EDIT_BOX;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_eb_inst_create, 
                                               wgl_eb_inst_destroy, 
                                               wgl_eb_default_proc,
                                               WGL_ATTR_FOCUSABLE};
    UINT32                      ui4_cnt_type = WGL_CONTENT_EDIT_BOX_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_eb_cnt_inst_create,
                                               wgl_eb_cnt_inst_destroy,
                                               wgl_eb_cnt_command_proc,
                                               wgl_eb_cnt_paint}; 
    INT32                       i4_ret;

    DBG_API(("{WGL(EB)} %s: ui4_init_type = 0x%.8x\n", __FUNCTION__, ui4_init_type));

    if(WGL_EB_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB)} ERR: Fail to register Component Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    

    if(WGL_EB_INIT_DEFAULT_CNT & ui4_init_type)
    {
        i4_ret = wgl_register_content_intf(ui4_cnt_type, &t_cnt_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB)} ERR: Fail to register Content Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    i4_ret = wgl_eb_cli_init();
    WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to init EditBox CLI. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

 /*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 wgl_eb_default_proc(
    HANDLE_T                    h_widget,
    UINT32                      ui4_msg,
    VOID*                       pv_param1,
    VOID*                       pv_param2)
{
    /*local variable declaration*/
    GL_RECT_T                   t_update_rect;
    WGL_EB_CONTROL_LOGIC_T*     pt_edit = NULL;
    GL_RECT_T                   t_widget_rect = {0};
    VOID*                       pv_internal_cmd_data = NULL;
    INT32                       i4_ret = WMPR_DONE;
    INT32                       i4_temp;
    UINT32                      ui4_update_rect;

    DBG_API(("{WGL(EB)} %s: h_widget = 0x%.8x \tui4_msg = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
            __FUNCTION__, 
            h_widget, 
            ui4_msg,
            pv_param1,
            pv_param2));

    switch(ui4_msg)
    {
    case WGL_MSG_PAINT:
        {
            i4_ret = x_wgl_get_update_region(h_widget, &ui4_update_rect, &t_update_rect);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's update region. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        break;

    case WGL_MSG_GET_FOCUS:
        return _wgl_eb_msg_hdr_get_focus(h_widget);
        
    case WGL_MSG_LOSE_FOCUS:
        return _wgl_eb_msg_hdr_lose_focus(h_widget);

    case WGL_MSG_KEY_DOWN:
        return _wgl_eb_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_DOWN);

    case WGL_MSG_KEY_UP:
        return _wgl_eb_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_UP);

    case WGL_MSG_COMMAND:
        {
            if(WGL_CMD_GL_SET_INSET == (UINT32)pv_param1)   /*need to get widget's rectangle before lock it*/
            {
                i4_ret = x_wgl_get_coords(h_widget, WGL_COORDS_LOCAL, &t_widget_rect);
                if(WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's dimension. (%d) at L%d\n\n", i4_ret, __LINE__));
                    return i4_ret;
                }
                
                pv_internal_cmd_data = &t_widget_rect;
            }
            else
            {
                pv_internal_cmd_data = NULL;
            }
        }
        break;

    default:
        break;
    }
    
    /*Get btn data and lock instance*/
    i4_ret = wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_edit);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_edit)
    {
        i4_temp = wgl_release_instance(h_widget, pt_edit);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(EB)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    switch (ui4_msg)
    {
    case WGL_MSG_PAINT:
            {
        /*app mybe needs to decide text direction at run time,so the widget query text direction from app's callback
               and update widget's text direction according to return value*/
        UINT8 ui1_text_dir;
        SIZE_T z_size = sizeof(UINT8);
        /*because x_wgl_query_canvas_info_callback() maybe call app's callback function,so unlock first*/
        i4_temp = wgl_release_instance(h_widget, pt_edit);
        WGL_ASSERT(WGLR_OK == i4_temp);
        i4_ret = x_wgl_query_canvas_info_callback(h_widget,WGL_CVS_CB_TXT_DIR,(VOID*)(&ui1_text_dir),&z_size);
        if( wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_edit) != WGLR_OK)
        {
            DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
            return WGLR_INV_HANDLE;
        }
        if(i4_ret == WGLR_OK)
        {
            i4_ret = _wgl_eb_msg_hdr_command(h_widget, 
                                              pt_edit, 
                                              WGL_CMD_GL_SET_TEXT_DIRECTION, 
                                              WGL_PACK(ui1_text_dir),
                                              NULL,
                                              pv_internal_cmd_data);

            if(i4_ret != WGLR_OK)
            {
                break;
            }
        }
        else if(i4_ret != WGLR_FUNC_NOT_IMPL)
        {
            break;
        }
        i4_ret = _wgl_eb_msg_hdr_paint(h_widget, 
                                       pt_edit, 
                                       (GL_HGRAPHICS_T)pv_param1, 
                                       ui4_update_rect, 
                                       &t_update_rect, 
                                       WGL_POINTER_TO_UINT16(pv_param2));
        break;
        }
    case WGL_MSG_ACTIVATE:
        /*do nothing*/
        i4_ret = WMPR_DONE;
        break;

    case WGL_MSG_MOVE:
        /*do nothing*/
        i4_ret = WMPR_DONE;
        break;

    case WGL_MSG_SIZE:
        i4_ret = _wgl_eb_msg_hdr_size(h_widget, 
                                      pt_edit, 
                                      WGL_GET_HI_WORD((UINT32)pv_param1), 
                                      WGL_GET_LOW_WORD((UINT32)pv_param1));
        break;

    case WGL_MSG_ENABLE:
        {
            WGL_UPDATE_STATE(pt_edit->ui2_state, WGL_POINTER_TO_BOOL(pv_param1), WGL_STATE_ENABLED);
            i4_ret = WMPR_DONE;
        }
        break;

    case WGL_MSG_NOTIFY:
        {
            WGL_ASSERT(0);  /*btn static should not receive this message*/
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL(EB)} ERR: EditBox should not receive any notification. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        break;

    case WGL_MSG_COMMAND:
        {
            if(NULL == pv_param2)
            {
                i4_ret = WGLR_INV_ARG;
                DBG_ERROR(("{WGL(EB)} ERR: The 2nd parameter of WGL_MSG_COMMAND should not be NULL. (%d) at L%d\n\n", i4_ret, __LINE__));
            }
            else
            {
                i4_ret = _wgl_eb_msg_hdr_command(h_widget, 
                                                 pt_edit, 
                                                 (UINT32)pv_param1, 
                                                 ((WGL_PACKED_PARAM_T*)pv_param2)->pv_param1,
                                                 ((WGL_PACKED_PARAM_T*)pv_param2)->pv_param2,
                                                 pv_internal_cmd_data);
            }
        }
        break;

    case WGL_MSG_CREATE:
        /*do nothing*/
        i4_ret = WMPR_DONE;
        break;

    case WGL_MSG_DESTROY:
        /*do nothing*/
        i4_ret = WMPR_DONE;
        break;
        
    case WGL_MSG_CHANGE_COLOR_CFG:
        {
            i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, WGL_CNT_EB_UPDATE_CVSST, pv_param1, NULL);
        }
        break;

    default:
        i4_ret = WGLR_INV_MSG;
        DBG_ERROR(("{WGL(EB)} ERR: Unknown message type. (%d) at L%d\n\n", i4_ret, __LINE__));
        break;
    }

    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_edit);
    WGL_ASSERT(WGLR_OK == i4_temp);

    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_eb_inst_create( 
    WGL_WIDGET_CONTEXT_HANDLE_T     *pt_ctx_wgt,
    WGL_WIDGET_CREATE_INPUT_T       *pt_wgt_create)
{
    /*local variable declaration*/
    GL_RECT_T                       t_cnt_rect = {0};
    WGL_CONTENT_CTRL_INTF_T         t_cnt_intf = {0};
    WGL_BORDER_CTRL_INTF_T          t_bdr_intf = {0};
    WGL_CONTENT_CONTEXT_HANDLE_T    h_cnt_data = NULL;
    WGL_BORDER_CONTEXT_HANDLE_T     h_bdr_data = NULL;
    WGL_CONTENT_CREATE_INPUT_T      t_cnt_create_input;
    WGL_BORDER_CREATE_INPUT_T       t_bdr_create_input;
    WGL_EB_CONTROL_LOGIC_T*         pt_ctrl_logic;
    WGL_INSET_T                     t_bdr_inset;
    WGL_CANVAS_STATUS_T             t_cvsst;
    INT32                           i4_ret;
    
    DBG_API(("{WGL(EB)} %s: pt_ctx_wgt = 0x%.8x \tpt_wgt_create = 0x%.8x\n", __FUNCTION__, pt_ctx_wgt, pt_wgt_create));

    /*check parameter*/
    if(NULL == pt_ctx_wgt || NULL == pt_wgt_create)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(EB)} pt_ctx_wgt = 0x%.8x   ui4_type_content = 0x%.8x   ui4_type_border = 0x%.8x\n"
              "\tpt_rect(l, t, r, b) = (%d, %d, %d, %d)\n"
              "\tpv_param = 0x%.8x\n",
              pt_ctx_wgt, 
              pt_wgt_create->ui4_type_content, 
              pt_wgt_create->ui4_type_border,
              pt_wgt_create->pt_rect->i4_left, pt_wgt_create->pt_rect->i4_top, pt_wgt_create->pt_rect->i4_right, pt_wgt_create->pt_rect->i4_bottom,
              pt_wgt_create->pv_param));

    /*initialize canvas status*/
    wgl_cvsst_update(&t_cvsst, &pt_wgt_create->t_cvs_info, pt_wgt_create->h_palette);
    
    /*query content factory*/
    i4_ret = wgl_query_content_intf(pt_wgt_create->ui4_type_content, &t_cnt_intf);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get content's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_TYPE;
    }
    WGL_CHECK_INTERNAL(t_cnt_intf.pf_content_cmd_proc 
               && t_cnt_intf.pf_create_instance 
               && t_cnt_intf.pf_destroy_instance 
               && t_cnt_intf.pf_paint_content); /*content should not be NULL*/
    
    /*query border factory*/
    i4_ret = wgl_query_border_intf(pt_wgt_create->ui4_type_border, &t_bdr_intf);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get border's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_TYPE;
    }

    /*allocate memory for control logic object*/
    pt_ctrl_logic = (WGL_EB_CONTROL_LOGIC_T*) WGL_MEM_ALLOC(sizeof(WGL_EB_CONTROL_LOGIC_T));
    if(NULL == pt_ctrl_logic)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to alloc memory for control logic. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize control logic object*/
    x_memset(pt_ctrl_logic, 0, sizeof(WGL_EB_CONTROL_LOGIC_T));

#ifdef CLI_LVL_ALL
    pt_ctrl_logic->ui4_mem_size = sizeof(WGL_EB_CONTROL_LOGIC_T);   
#endif

    /*initialize content rect*/
    t_cnt_rect.i4_right = WGL_RECT_GET_WIDTH(pt_wgt_create->pt_rect);
    t_cnt_rect.i4_bottom = WGL_RECT_GET_HEIGHT(pt_wgt_create->pt_rect);

    /*not NULL-Border*/
    if(t_bdr_intf.pf_create_instance) 
    {
        /*create border object*/
        t_bdr_create_input.i4_width = WGL_RECT_GET_WIDTH(pt_wgt_create->pt_rect);
        t_bdr_create_input.i4_height = WGL_RECT_GET_HEIGHT(pt_wgt_create->pt_rect);
        t_bdr_create_input.ui4_style = (UINT32)pt_wgt_create->pv_param; /*widget style*/
        t_bdr_create_input.pt_cvsst = &t_cvsst;
        i4_ret = t_bdr_intf.pf_create_instance(&h_bdr_data, &t_bdr_create_input);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB)} ERR: Fail to create border object. (%d) at L%d\n\n", i4_ret, __LINE__));
            goto _err_out;
        }

        /*get default border inset*/
        i4_ret = t_bdr_intf.pf_border_cmd_proc(h_bdr_data, WGL_CMD_GL_GET_INSET, &t_bdr_inset, NULL);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(EB)} ERR: Fail to get border's inset. (%d) at L%d\n\n", i4_ret, __LINE__));
            goto _err_out;
        }

        /*subtract border inset*/
        WGL_RECT_INSET(&t_cnt_rect, 
                       t_bdr_inset.i4_left, 
                       t_bdr_inset.i4_top, 
                       t_bdr_inset.i4_right, 
                       t_bdr_inset.i4_bottom);
    }
    else if(WGL_STL_GL_BDR_FILL_CNT_BK & ((UINT32)pt_wgt_create->pv_param))   /*null border*/
    {
        DBG_ERROR(("{WGL(EB)} ERR: NULL-Border cannot be with WGL_STL_GL_BDR_FILL_CNT_BK style. (%d) at L%d\n\n", i4_ret, __LINE__));
        i4_ret = WGLR_NO_BORDER;
        goto _err_out;
    }

    /*create content object*/
    WGL_RECT_COPY(&t_cnt_create_input.t_rect_cnt, &t_cnt_rect);
    t_cnt_create_input.pt_cvsst = &t_cvsst;
    t_cnt_create_input.pv_init = pt_wgt_create->pv_param; /*widget style*/
    i4_ret = t_cnt_intf.pf_create_instance(&h_cnt_data, &t_cnt_create_input);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to create content object. (%d) at L%d\n\n", i4_ret, __LINE__));
        goto _err_out;
    }

    /*store border object in control logic object*/
    x_memcpy(&pt_ctrl_logic->t_bdr_intf, &t_bdr_intf, sizeof(WGL_BORDER_CTRL_INTF_T));
    pt_ctrl_logic->h_bdr_data = h_bdr_data;

    /*store cotent object in control logic object*/ 
    x_memcpy(&pt_ctrl_logic->t_cnt_intf, &t_cnt_intf, sizeof(WGL_CONTENT_CTRL_INTF_T));
    pt_ctrl_logic->h_cnt_data = h_cnt_data;

    /*initialize other info*/
    pt_ctrl_logic->ui4_style = (UINT32)pt_wgt_create->pv_param;
    /*pt_ctrl_logic->ui2_state = (pt_ctrl_logic->ui4_style & WGL_STL_GL_DISABLED) ? 0 : WGL_STATE_ENABLED;*/
    pt_ctrl_logic->ui2_state = WGL_STATE_ENABLED;
    pt_ctrl_logic->b_change_flag = FALSE;
    pt_ctrl_logic->b_editing = FALSE;
    pt_ctrl_logic->e_mode = WGL_EB_MODE_REPLACE;
    pt_ctrl_logic->ui2_cur_pos = 0;
    pt_ctrl_logic->ui2_input_length = 0;
    pt_ctrl_logic->ui2_input_limit = WGL_EB_GET_MAX_LENGTH(pt_ctrl_logic->ui4_style);

    /*init control key map*/
    pt_ctrl_logic->t_key_map.ui4_cursor_left = _WGL_EB_DEF_KEY_CURSOR_LEFT;
    pt_ctrl_logic->t_key_map.ui4_cursor_right = _WGL_EB_DEF_KEY_CURSOR_RIGHT;
    pt_ctrl_logic->t_key_map.ui4_backsapce = _WGL_EB_DEF_KEY_BACKSPACE;
    pt_ctrl_logic->t_key_map.ui4_switch_insert = _WGL_EB_DEF_KEY_SW_INSERT;
    pt_ctrl_logic->t_key_map.ui4_enter_editing = _WGL_EB_DEF_KEY_ENTER_EDIT;
    pt_ctrl_logic->t_key_map.ui4_leave_editing = _WGL_EB_DEF_KEY_LEAVE_EDIT;

    /*prepare return value*/
    *pt_ctx_wgt = (WGL_WIDGET_CONTEXT_HANDLE_T)pt_ctrl_logic;
    return WGLR_OK;

_err_out:
    if(h_cnt_data)
    {
        t_cnt_intf.pf_destroy_instance(h_cnt_data);
    }
    if(h_bdr_data)
    {
        t_bdr_intf.pf_destroy_instance(h_bdr_data);
    }

    WGL_MEM_FREE(pt_ctrl_logic);
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_eb_inst_destroy( 
    WGL_WIDGET_CONTEXT_HANDLE_T     t_ctx_wgt)
{
    /*local variable parameter*/
    WGL_EB_CONTROL_LOGIC_T*         pt_ctrl_logic;
    INT32                           i4_ret;

    DBG_API(("{WGL(EB)} %s: t_ctx_wgt = 0x%.8x\n", __FUNCTION__, t_ctx_wgt));

    /*check parameter*/
    if(NULL == t_ctx_wgt)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_ctrl_logic = (WGL_EB_CONTROL_LOGIC_T*)t_ctx_wgt;

    /*invoke content object to destroy itself*/
    if(pt_ctrl_logic->h_cnt_data)
    {
        WGL_CHECK_INTERNAL(pt_ctrl_logic->t_cnt_intf.pf_destroy_instance); /*content should not be NULL*/
        i4_ret = pt_ctrl_logic->t_cnt_intf.pf_destroy_instance(pt_ctrl_logic->h_cnt_data);
        WGL_ASSERT(WGLR_OK == i4_ret);
    }

    /*invoke border object to destroy itself*/
    if(pt_ctrl_logic->h_bdr_data)
    {
        /*if border-data exists, border-interface should not be NULL*/
        WGL_CHECK_INTERNAL(pt_ctrl_logic->t_bdr_intf.pf_destroy_instance); 

        i4_ret = pt_ctrl_logic->t_bdr_intf.pf_destroy_instance(pt_ctrl_logic->h_bdr_data);
        WGL_ASSERT(WGLR_OK == i4_ret);
    }

    /*free control logic object's resource*/
    WGL_MEM_FREE(pt_ctrl_logic);

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_PAINT message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_paint(
    HANDLE_T                        h_widget,
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(EB)} ERR: GRAPHICS object is NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check if pushed*/
    if(IS_STATE_PUSHED(pt_edit->ui2_state))
    {
        WGL_SET_FLAG(ui2_state, WGL_STATE_PUSHED);
    }
    
    if(pt_edit->t_bdr_intf.pf_paint_border)
    {
        /*check border*/
        WGL_ASSERT(pt_edit->h_bdr_data);

        /*paint border*/
        i4_ret = pt_edit->t_bdr_intf.pf_paint_border(pt_edit->h_bdr_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*paint content*/
    WGL_CHECK_INTERNAL(pt_edit->t_cnt_intf.pf_paint_content);
    WGL_CHECK_INTERNAL(pt_edit->h_cnt_data);
    return pt_edit->t_cnt_intf.pf_paint_content(pt_edit->h_cnt_data, 
                                                h_graphics, 
                                                ui4_upt_rect, 
                                                pt_upt_rect, 
                                                ui2_state);
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_KEY_DOWN & WGL_MSG_KEY_UP message handler. Not check h_widget and pt_btn
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_key(
    HANDLE_T                        h_widget,
    UINT32                          ui4_k_code,
    UINT32                          ui4_k_state)
{
    /*local varaible declaration*/
    WGL_EB_CONTROL_LOGIC_T*         pt_edit = NULL;
    INT32                           i4_ret, i4_temp;
    BOOL                            b_invoke = TRUE;
    WGL_EB_NFY_TYPE_T               e_notify_flag = _WGL_EB_NFY_TYPE_NONE;
    

    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_edit))
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_edit)
    {
        i4_temp = wgl_release_instance(h_widget, pt_edit);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(EB)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*check state*/
    if(!IS_STATE_ENABLED(pt_edit->ui2_state))
    {
        b_invoke = FALSE;
    }

    /*invoke key event processor*/
    i4_ret = (b_invoke) ? _wgl_eb_process_key_event(h_widget, pt_edit, ui4_k_code, ui4_k_state, &e_notify_flag, FALSE) : WGLR_OK;
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_edit);
    WGL_ASSERT(WGLR_OK == i4_temp);

    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
        
    /*notify parent*/
    switch (e_notify_flag)
    {
    case _WGL_EB_NFY_TYPE_KEY:
        {
            i4_ret = wgl_notify_parent_key(h_widget, ui4_k_code, ui4_k_state);
            WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to notify widget's parent of key event. (%d) at L%d\n\n", i4_ret, __LINE__)));
            return i4_ret;
        }

    case _WGL_EB_NFY_TYPE_DATA_CHANGE:
    case _WGL_EB_NFY_TYPE_OVER_MAX:
        return _wgl_eb_notify_parent(h_widget, e_notify_flag);

    case _WGL_EB_NFY_TYPE_NONE:
        return WGLR_KEY_MSG_NOT_PROCESSED;
    default:
        break;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_GET_FOCUS.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_get_focus(
    HANDLE_T                           h_widget)
{
    /*local varaible declaration*/
    WGL_EB_CONTROL_LOGIC_T*         pt_edit = NULL;
    INT32                           i4_ret, i4_temp;
    WGL_EB_NFY_TYPE_T               e_notify_flag = _WGL_EB_NFY_TYPE_NONE;
    
    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_edit))
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_edit)
    {
        i4_temp = wgl_release_instance(h_widget, pt_edit);
        WGL_ASSERT(WGLR_OK == i4_temp);
        
        DBG_ERROR(("{WGL(EB)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
    /*set focus state*/
    /*WGL_UPDATE_STATE(pt_edit->ui2_state, TRUE, WGL_STATE_FOCUSED);*/
    i4_temp = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, WGL_CNT_EB_STATE_CHANE, NULL, NULL);
    WGL_ASSERT(WGLR_OK == i4_temp);
    /*invoke key event processor*//*simulate a enter editing key is release*/
    if(_WGL_EB_EXT_ATTR_IS_AUTO_EDITING(pt_edit->ui2_ext_attr))
    {
        i4_ret = _wgl_eb_process_key_event(h_widget, pt_edit, pt_edit->t_key_map.ui4_enter_editing, WGL_KEY_STATE_UP, &e_notify_flag, TRUE);
    }
    else
    {
        i4_ret = WGLR_OK;
    }
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_edit);
    WGL_ASSERT(WGLR_OK == i4_temp);
    
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*notify parent*/
    i4_ret = wgl_notify_parent_focus(h_widget, TRUE);
    WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to notify widget's parent of getting focus. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_LOSE_FOCUS.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_lose_focus(
    HANDLE_T                        h_widget)
{
    /*local varaible declaration*/
    WGL_EB_CONTROL_LOGIC_T*         pt_edit = NULL;
    INT32                           i4_ret, i4_temp;
    WGL_EB_NFY_TYPE_T               e_notify_flag = _WGL_EB_NFY_TYPE_NONE;
    
    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_edit))
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_edit)
    {
        i4_temp = wgl_release_instance(h_widget, pt_edit);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(EB)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
    /*set focus state*/
    /*WGL_UPDATE_STATE(pt_edit->ui2_state, FALSE, WGL_STATE_FOCUSED);*/
    i4_temp = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, WGL_CNT_EB_STATE_CHANE, NULL, NULL);
    WGL_ASSERT(WGLR_OK == i4_temp);
    /*invoke key event processor*//*simulate a leave editing key is release*/
    if(pt_edit->b_editing)
    {
        i4_ret = _wgl_eb_process_key_event(h_widget, pt_edit, pt_edit->t_key_map.ui4_leave_editing, WGL_KEY_STATE_UP, &e_notify_flag, TRUE);
    }
    else
    {
        i4_ret = WGLR_OK;
    }
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_edit);
    WGL_ASSERT(WGLR_OK == i4_temp);
    
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*notify parent*/
    i4_ret = wgl_notify_parent_focus(h_widget, FALSE);
    WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to notify widget's parent of losing focus. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_process_key_event(
    HANDLE_T                        h_widget,
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT32                          ui4_k_code,
    UINT32                          ui4_k_state,
    WGL_EB_NFY_TYPE_T*              pe_notify_flag,
    BOOL                            b_disable_repaint)
{
    /*local variable declaration*/
    BOOL                            b_repaint = FALSE;
    BOOL                            b_mode_change = FALSE;
    UINT16                          ui2_index, ui2_target_index;
    INT32                           i4_ret = WGLR_OK;
    UTF16_T                         w2_char;
        
    /*check if in editing mode*/
    if(FALSE == pt_edit->b_editing && ui4_k_code != pt_edit->t_key_map.ui4_enter_editing)
    {
        *pe_notify_flag = _WGL_EB_NFY_TYPE_KEY;
        return WGLR_OK;
    }
    
    /*init notify flag*/
    *pe_notify_flag = _WGL_EB_NFY_TYPE_NONE;

    /*process key event*/
    ui2_index = pt_edit->ui2_cur_pos;

    /*enter or leave editing mode*/
    if(ui4_k_code == pt_edit->t_key_map.ui4_enter_editing || ui4_k_code == pt_edit->t_key_map.ui4_leave_editing)
    {
        /*keep old editing mode*/
        b_mode_change = pt_edit->b_editing;
        if(WGL_KEY_STATE_UP == ui4_k_state)
        {
            pt_edit->b_editing = (pt_edit->t_key_map.ui4_enter_editing == pt_edit->t_key_map.ui4_leave_editing) ? 
                                  !pt_edit->b_editing : 
                                  ((ui4_k_code == pt_edit->t_key_map.ui4_enter_editing) ? TRUE : FALSE);
        }
        
        b_mode_change = (b_mode_change != pt_edit->b_editing) ? TRUE : FALSE;
        if(!_WGL_EB_EXT_ATTR_IS_KEEP_CURSOR_POS(pt_edit->ui2_ext_attr))
        {
            pt_edit->ui2_cur_pos = b_mode_change ? 0 : pt_edit->ui2_cur_pos;
        }
    }
    else if(ui4_k_code == pt_edit->t_key_map.ui4_cursor_left) /*move cursor left*/
    {
        if(WGL_KEY_STATE_DOWN == ui4_k_state)
        {
            pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos == 0) ? 0 : (pt_edit->ui2_cur_pos - (UINT16)1);
        }
    }
    else if(ui4_k_code == pt_edit->t_key_map.ui4_cursor_right) /*move cursor right*/
    {
        if(WGL_KEY_STATE_DOWN == ui4_k_state)
        {
            pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos == pt_edit->ui2_input_length) ? 
                                   pt_edit->ui2_input_length : 
                                   (pt_edit->ui2_cur_pos + (UINT16)1);

            if(pt_edit->ui4_style & WGL_STL_EB_RECYCLE)
            {
                pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos > pt_edit->ui2_input_limit - (UINT16)1) ? 0 : pt_edit->ui2_cur_pos;
            }
            else
            {
                pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos >= pt_edit->ui2_input_limit - (UINT16)1) ? 
                                       (pt_edit->ui2_input_limit - (UINT16)1) : 
                                       pt_edit->ui2_cur_pos;
            }
        }
    }
    else if(ui4_k_code == pt_edit->t_key_map.ui4_backsapce) /*remove previous character*/
    {
        if(WGL_KEY_STATE_DOWN == ui4_k_state && pt_edit->ui2_input_length > 0)
        {
            /*remove previous character*/
            ui2_target_index = (0 != pt_edit->ui2_cur_pos) ? (pt_edit->ui2_cur_pos - (UINT16)1) : 0;
            i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                             WGL_CNT_EB_INDEX_REMOVE, 
                                                             (VOID*)(UINT32)ui2_target_index, 
                                                             (VOID*)1);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }

            /*update status*/
            pt_edit->ui2_cur_pos = (0 != pt_edit->ui2_cur_pos) ? (pt_edit->ui2_cur_pos - (UINT16)1) : 0;
            pt_edit->ui2_input_length--;
            pt_edit->b_change_flag = TRUE;
            *pe_notify_flag = _WGL_EB_NFY_TYPE_DATA_CHANGE;
            b_repaint = TRUE;
        }
    }
    else if(ui4_k_code == pt_edit->t_key_map.ui4_switch_insert) /*switch insert/replace mode*/
    {
        if(WGL_KEY_STATE_DOWN == ui4_k_state)
        {
            pt_edit->e_mode = (WGL_EB_MODE_INSERT == pt_edit->e_mode) ? WGL_EB_MODE_REPLACE : WGL_EB_MODE_INSERT;
        }
    }
    else if((ui4_k_code >= BTN_DIGIT_0 && ui4_k_code <= BTN_DIGIT_9) 
            || ((_WGL_EB_EXT_ATTR_IS_CHAR_SUPPORT(pt_edit->ui2_ext_attr)) 
               && ((BTN_DIGIT_DOT == ui4_k_code) 
#ifdef BTN_ALPHA_A
                   || (ui4_k_code >= BTN_ALPHA_A && ui4_k_code <= BTN_ALPHA_Z)
                   || (ui4_k_code >= BTN_ALPHA_a && ui4_k_code <= BTN_ALPHA_z)
#endif
				  )
               )
           )
    {
        if(WGL_KEY_STATE_DOWN == ui4_k_state)
        {
            /*check if it's full*/
            if(WGL_EB_MODE_INSERT == pt_edit->e_mode && pt_edit->ui2_input_length == pt_edit->ui2_input_limit)
            {
                *pe_notify_flag = _WGL_EB_NFY_TYPE_OVER_MAX;
                return WGLR_OK;
            }
            
            /*Convert to UTF16*/
            w2_char = 0;
            if(ui4_k_code >= BTN_DIGIT_0 && ui4_k_code <= BTN_DIGIT_9)
            {
                w2_char = (UTF16_T)ui4_k_code - (UTF16_T)BTN_DIGIT_0 + _WGL_EB_UTF16_DIGIT_0;
            }
            else if(BTN_DIGIT_DOT == ui4_k_code)
            {
                w2_char = _WGL_EB_UTF16_CHAR_DOT;
            }
                  
#ifdef BTN_ALPHA_A
            else if(ui4_k_code >= BTN_ALPHA_A && ui4_k_code <= BTN_ALPHA_Z)
            {
                w2_char = ui4_k_code - BTN_ALPHA_A + _WGL_EB_UTF16_ALPHA_A;
            }
            else
            {
                w2_char = ui4_k_code - BTN_ALPHA_a + _WGL_EB_UTF16_ALPHA_a;
            }
#endif

            /*update content string*/
            if(WGL_EB_MODE_INSERT == pt_edit->e_mode || pt_edit->ui2_cur_pos == pt_edit->ui2_input_length) /*append at the end*/
            {
                
                i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                                 WGL_CNT_EB_INDEX_INSERT, 
                                                                 (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                                 (VOID*)(UINT32)w2_char);
                if(WGLR_OK != i4_ret)
                {
                    return i4_ret;
                }

                pt_edit->ui2_input_length = pt_edit->ui2_input_length + (UINT16)1;
            
            }
            else
            {
                i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                                 WGL_CNT_EB_INDEX_REPLACE, 
                                                                 (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                                 (VOID*)(UINT32)w2_char);
                if(WGLR_OK != i4_ret)
                {
                    return i4_ret;
                }
            }

            /*update status*/
           
            if(WGL_EB_MODE_REPLACE == pt_edit->e_mode || pt_edit->ui2_cur_pos == pt_edit->ui2_input_length - (UINT16)1)
            {
                pt_edit->ui2_cur_pos++;

                if(pt_edit->ui4_style & WGL_STL_EB_RECYCLE)
                {
                    pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos >= pt_edit->ui2_input_limit) ? 0 : pt_edit->ui2_cur_pos;
                }
                else
                {
                    pt_edit->ui2_cur_pos = (pt_edit->ui2_cur_pos >= pt_edit->ui2_input_limit) ? 
                                           (pt_edit->ui2_input_limit - (UINT16)1) : 
                                           pt_edit->ui2_cur_pos;
                }
            }
            b_repaint = TRUE;
            pt_edit->b_change_flag = TRUE;
            *pe_notify_flag = _WGL_EB_NFY_TYPE_DATA_CHANGE;
        }
    }
    else /*unknown character*/
    {
        *pe_notify_flag = _WGL_EB_NFY_TYPE_KEY;
        return WGLR_OK;
    }
    
    /*change highlight*/
    if(ui2_index != pt_edit->ui2_cur_pos || b_mode_change)
    {
        i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                         WGL_CNT_EB_INDEX_HIGHLIGHT, 
                                                         (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                         (VOID*)(UINT32)((pt_edit->b_editing || _WGL_EB_EXT_ATTR_IS_ALWAYS_HT(pt_edit->ui2_ext_attr)) ? TRUE : FALSE));
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*repaint for highlight index change*/
        b_repaint = TRUE;
    }

    /*Not editing mode, show string from the head*/
    if(!pt_edit->b_editing)
    {
        i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                         WGL_CNT_EB_INDEX_START, 
                                                         (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                         NULL);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        
        b_repaint = TRUE;
    }

    /*repaint*/
    if(b_repaint && FALSE == b_disable_repaint)
    {
        i4_ret = x_wgl_repaint(h_widget, NULL, FALSE);
        WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to repaint widget. (%d) at L%d\n\n", i4_ret, __LINE__)));
    }
    
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_SIZE message handler. Not check h_widget and pt_btn
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_size(
    HANDLE_T                        h_widget,
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT16                          ui2_width,
    UINT16                          ui2_height)
{
    /*local variable declaration*/
    GL_RECT_T                       t_widget_rect = {0};
    INT32                           i4_ret;
    
    /*initialize content rect*/
    t_widget_rect.i4_right = (INT32)ui2_width;
    t_widget_rect.i4_bottom = (INT32)ui2_height;

    if(pt_edit->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_ASSERT(pt_edit->h_bdr_data);
        
        /*notify border to update its rectangle*/
        i4_ret = pt_edit->t_bdr_intf.pf_border_cmd_proc(pt_edit->h_bdr_data, WGL_BDR_GL_SET_SIZE, (VOID*)(INT32)ui2_width, (VOID*)(INT32)ui2_height);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    return _wgl_eb_change_cnt_rect(pt_edit, &t_widget_rect, FALSE, NULL);

}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_msg_hdr_command(
                HANDLE_T                        h_widget,
                WGL_EB_CONTROL_LOGIC_T*         pt_edit,
                UINT32                          ui4_command_id,
                VOID*                           pv_param1,
                VOID*                           pv_param2,
                VOID*                           pv_internal_cmd_data)
{

    DBG_API(("{WGL(EB)} %s: h_widget = 0x%.8x \tui4_command_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
            __FUNCTION__, 
            h_widget, 
            ui4_command_id,
            pv_param1,
            pv_param2));

    /*check command*/
    if(! (WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_EDIT_BOX, ui4_command_id) 
            || WGL_IS_CMD_GENERAL(ui4_command_id)
            || WGL_IS_CONTENT_CMD(ui4_command_id) 
            || WGL_IS_BORDER_CMD(ui4_command_id)))
    {
        DBG_ERROR(("{WGL(EB)} ERR: Unknown Command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    /*get command handler*/
    switch (ui4_command_id)
    {
    case WGL_CMD_GL_SET_ATTACH_DATA:
        {
            pt_edit->pv_attach = pv_param1;
        }
        break;

    case WGL_CMD_GL_GET_ATTACH_DATA:
        {
            if(NULL == (VOID**)pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ATTACH_DATA, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            *((VOID**)pv_param1) = pt_edit->pv_attach;
        }
        break;

    case WGL_CMD_EB_SET_INPUT_LIMIT:
        return _wgl_eb_cmd_hdr_set_input_limit(pt_edit, WGL_POINTER_TO_UINT16(pv_param1));

    case WGL_CMD_EB_GET_INPUT_LIMIT:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_INPUT_LIMIT, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }

            *((UINT16*)pv_param1) = pt_edit->ui2_input_limit;
        }
        break;

    case WGL_CMD_EB_SET_TEXT:
        return _wgl_eb_cmd_hdr_set_text(pt_edit, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1);

    case WGL_CMD_EB_GET_TEXT:
        return _wgl_eb_cmd_hdr_get_text(pt_edit, WGL_POINTER_TO_UINT16(pv_param2), (UTF16_T*)pv_param1);

    case WGL_CMD_EB_GET_TEXT_LENGTH:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_TEXT_LENGTH, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            *((UINT16*)pv_param1) = pt_edit->ui2_input_length;
        }
        break;

    case WGL_CMD_EB_SET_CURSOR_POS:
        return _wgl_eb_cmd_hdr_set_cur_pos(pt_edit, WGL_POINTER_TO_UINT16(pv_param1));

    case WGL_CMD_EB_GET_CURSOR_POS:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_CURSOR_POS, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            *((UINT16*)pv_param1) = pt_edit->ui2_cur_pos;
        }
        break;

    case WGL_CMD_EB_SET_CHANGE_FLAG:
        {
            if( TRUE != WGL_POINTER_TO_BOOL(pv_param1) && FALSE != WGL_POINTER_TO_BOOL(pv_param1))
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_SET_CHANGE_FLAG, the value specified by 1st input argument should be TRUE or FALSE. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            pt_edit->b_change_flag = WGL_POINTER_TO_BOOL(pv_param1);
        }
        break;

    case WGL_CMD_EB_GET_CHANGE_FLAG:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_CHANGE_FLAG, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            *((BOOL*)pv_param1) = pt_edit->b_change_flag;
        }
        break;

    case WGL_CMD_EB_SET_KEY_MAP:
        return _wgl_eb_cmd_hdr_set_key_map(pt_edit, (WGL_EB_KEY_MAP_T*)pv_param1);

    case WGL_CMD_EB_GET_KEY_MAP:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_KEY_MAP, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            x_memcpy((WGL_EB_KEY_MAP_T*)pv_param1, &pt_edit->t_key_map, sizeof(WGL_EB_KEY_MAP_T));
        }
        break;

    case WGL_CMD_EB_SET_INSERT_MODE:
        {
            if(WGL_EB_MODE_REPLACE != (WGL_EB_MODE_T)(INT32)pv_param1 && WGL_EB_MODE_INSERT != (WGL_EB_MODE_T)(INT32)pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_SET_INSERT_MODE, the value specified by 1st input argument should be WGL_EB_MODE_REPLACE or WGL_EB_MODE_INSERT. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            pt_edit->e_mode = (WGL_EB_MODE_T)(INT32)pv_param1;
        }
        break;

    case WGL_CMD_EB_GET_INSERT_MODE:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_INSERT_MODE, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            *((WGL_EB_MODE_T*)pv_param1) = pt_edit->e_mode;
        }
        break;

    case WGL_CMD_GL_SET_INSET:
        {
            DBG_INFO(("{WGL(EB)} set-inset(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n", 
                pv_param1,
                ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_left),
                ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_top),
                ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_right),
                ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_bottom)));

            /*[DTV00000540] For frame, icon, text, editbox, progressbar, and button widgets, the content background show incorrectly if the inset size  5  with uniform border*/
            /*Comments: widget component has to update CONTENT rectangle when BORDER rectangle changes.*/
            return _wgl_eb_change_cnt_rect(pt_edit, (GL_RECT_T*)pv_internal_cmd_data, TRUE, (WGL_INSET_T*)pv_param1);
        }

    case WGL_CMD_EB_SET_EXT_ATTR:
        {
            /*clean specific bits and keep other bits*/
            pt_edit->ui2_ext_attr &= ~(WGL_POINTER_TO_UINT16(pv_param1));

            /*set specific bits*/
            pt_edit->ui2_ext_attr |= (WGL_POINTER_TO_UINT16(pv_param1) & WGL_POINTER_TO_UINT16(pv_param2));
        }
        break;

    case WGL_CMD_EB_GET_EXT_ATTR:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. For WGL_CMD_EB_GET_EXT_ATTR, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }

            *((UINT16*)pv_param1) = pt_edit->ui2_ext_attr;
        }
        break;
    case WGL_CMD_GL_SET_TEXT_DIRECTION:
            {
                if(NULL != pt_edit->t_bdr_intf.pf_border_cmd_proc)
                {
                    /*check border*/
                    WGL_ASSERT(pt_edit->h_bdr_data);
                    return pt_edit->t_bdr_intf.pf_border_cmd_proc(pt_edit->h_bdr_data, WGL_CMD_GL_SET_BDR_TEXT_DIRECTION, pv_param1, pv_param2);
                }
              
            }
           break;
#ifdef CLI_LVL_ALL
    case WGL_CMD_GL_GET_ALLOC_MEM:
    {
        UINT32 ui4_wdg_mem = pt_edit->ui4_mem_size;
        WGL_MTX_OBJ_T*  pt_wgt_mem;
        if(NULL == pv_param1)
        {
                DBG_ERROR(("{WGL(EDIT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
        }
        
        pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_edit->h_cnt_data;
        ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;
        if(pt_edit->t_bdr_intf.pf_create_instance != NULL)
        {
            pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_edit->h_bdr_data;
            ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;   
        }
        
        *((UINT32*)pv_param1) = ui4_wdg_mem;
        break;
    }
#endif
    default:
        {
            if(WGL_IS_CONTENT_CMD(ui4_command_id) || WGL_IS_CMD_GENERAL(ui4_command_id))/*content command*/
            {
                /*check content*/
                WGL_CHECK_INTERNAL(pt_edit->t_cnt_intf.pf_content_cmd_proc);
                WGL_CHECK_INTERNAL(pt_edit->h_cnt_data);
                
                return pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                               ui4_command_id, 
                                                               pv_param1, 
                                                               pv_param2);
            }
            else if(WGL_IS_BORDER_CMD(ui4_command_id))/*border command*/
            {
                if(NULL == pt_edit->t_bdr_intf.pf_border_cmd_proc)
                {
                    DBG_ERROR(("{WGL(EB)} ERR: Border's command procedure is NULL. (%d) at L%d\n\n", WGLR_NO_BORDER, __LINE__));
                    return WGLR_NO_BORDER;
                }

                /*check border*/
                WGL_ASSERT(pt_edit->h_bdr_data);
                return pt_edit->t_bdr_intf.pf_border_cmd_proc(pt_edit->h_bdr_data,
                                                              ui4_command_id, 
                                                              pv_param1, 
                                                              pv_param2);
            }
            else
            {
                DBG_ERROR(("{WGL(EB)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
                return WGLR_INV_CMD;
            }
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cmd_hdr_set_input_limit(
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT16                          ui2_limit)
{
    /*local variable declaration*/
    INT32                           i4_ret;

    if( WGL_EB_GET_MAX_LENGTH(pt_edit->ui4_style) < ui2_limit )
    {
        DBG_ERROR(("{WGL(EB)} ERR: The input limit specified by 1st input argument cannot be larger than %d. (%d) at L%d\n\n", WGL_EB_GET_MAX_LENGTH(pt_edit->ui4_style), WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(pt_edit->ui2_input_length <= ui2_limit)
    {
        pt_edit->ui2_input_limit = ui2_limit;
        return WGLR_OK; 
    }

    /*remove character: from index(pt_edit->ui2_input_length), and count(pt_edit->ui2_input_length - ui2_limit)*/
    i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data,
                                                     WGL_CNT_EB_INDEX_REMOVE, 
                                                     (VOID*)(UINT32)ui2_limit, 
                                                     (VOID*)(pt_edit->ui2_input_length - ui2_limit));
    
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*update status*/
    pt_edit->ui2_input_limit = ui2_limit;
    pt_edit->ui2_input_length = ui2_limit;
    pt_edit->ui2_cur_pos = 0;

    /*update highlight index*/
    i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, 
                                                     WGL_CNT_EB_INDEX_HIGHLIGHT, 
                                                     (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                     (VOID*)(UINT32)((pt_edit->b_editing || _WGL_EB_EXT_ATTR_IS_ALWAYS_HT(pt_edit->ui2_ext_attr)) ? TRUE : FALSE));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cmd_hdr_set_cur_pos(
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT16                          ui2_new_pos)
{
    /*cursor is zero-based: 0 ~ ui2_input_limit - 1*/
    if(ui2_new_pos >= pt_edit->ui2_input_length)
    {
        pt_edit->ui2_cur_pos = (pt_edit->ui2_input_length == pt_edit->ui2_input_limit) ? 
                               ((pt_edit->ui2_input_length == 0) ? 0 : (pt_edit->ui2_input_length - (UINT16)1)) : 
                               pt_edit->ui2_input_length;
    }
    else
    {
        pt_edit->ui2_cur_pos = ui2_new_pos;
    }
    
    /*highlight pos*/
    return pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data,
                                                   WGL_CNT_EB_INDEX_HIGHLIGHT, 
                                                   (VOID*)(UINT32)pt_edit->ui2_cur_pos, 
                                                   (VOID*)(UINT32)((pt_edit->b_editing || _WGL_EB_EXT_ATTR_IS_ALWAYS_HT(pt_edit->ui2_ext_attr)) ? TRUE : FALSE));
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cmd_hdr_set_key_map(
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    const WGL_EB_KEY_MAP_T*         pt_key_map)
{

    DBG_INFO(("{WGL(EB)} pt_key_map = 0x%.8x\n"
            "(cl, cr, back) = (0x%.8x, 0x%.8x, 0x%.8x)\n"
            "(sw, enter, leave) = (0x%.8x, 0x%.8x, 0x%.8x)\n",
            pt_key_map,
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_cursor_left),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_cursor_right),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_backsapce),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_switch_insert),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_enter_editing),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_leave_editing)));

    if(NULL == pt_key_map)
    {
        pt_edit->t_key_map.ui4_cursor_left = _WGL_EB_DEF_KEY_CURSOR_LEFT;
        pt_edit->t_key_map.ui4_cursor_right = _WGL_EB_DEF_KEY_CURSOR_RIGHT;
        pt_edit->t_key_map.ui4_backsapce = _WGL_EB_DEF_KEY_BACKSPACE;
        pt_edit->t_key_map.ui4_switch_insert = _WGL_EB_DEF_KEY_SW_INSERT;
        pt_edit->t_key_map.ui4_enter_editing = _WGL_EB_DEF_KEY_ENTER_EDIT;
        pt_edit->t_key_map.ui4_leave_editing = _WGL_EB_DEF_KEY_LEAVE_EDIT;
    }
    else
    {
        x_memcpy(&pt_edit->t_key_map, pt_key_map, sizeof(WGL_EB_KEY_MAP_T));
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cmd_hdr_set_text(
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT16                          ui2_string_length,
    const UTF16_T*                  w2s_string)
{
    /*local variable declaration*/
    UINT16                          ui2_real_length;
    INT32                           i4_ret;

    
    if(NULL == w2s_string)
    {
        /*reset user string*/
        ui2_real_length = 0;
    }
    else
    {
        /*check length*/
        ui2_real_length = (UINT16)x_uc_w2s_strlen(w2s_string);
        ui2_real_length = (ui2_real_length > ui2_string_length) ? ui2_string_length : ui2_real_length;
        ui2_real_length = (ui2_real_length > pt_edit->ui2_input_limit) ? pt_edit->ui2_input_limit : ui2_real_length;
    }

    /*update Edit-Box's Content*/
    i4_ret = pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data,
                                                     WGL_CNT_EB_SET_TEXT, 
                                                     (VOID*)w2s_string, 
                                                     (VOID*)(UINT32)ui2_real_length);
    if(WGLR_OK != i4_ret)
    {
        WGL_ASSERT(0);
        return i4_ret;
    }

    /*update status*/
    pt_edit->ui2_input_length = ui2_real_length;
    if(pt_edit->ui2_cur_pos >= ui2_real_length)
    {
        return _wgl_eb_cmd_hdr_set_cur_pos(pt_edit, ui2_real_length);
    }
    else
    {
        return WGLR_OK;
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_cmd_hdr_get_text(
    WGL_EB_CONTROL_LOGIC_T*         pt_edit,
    UINT16                          ui2_string_length,
    UTF16_T*                        w2s_string)
{
    if(NULL == w2s_string)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Invalid arguments. The output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(pt_edit->ui2_input_length >= ui2_string_length)
    {
        DBG_ERROR(("{WGL(EB)} ERR: The buffer isn't enough. %d-characters is necessary. (%d) at L%d\n\n", pt_edit->ui2_input_length, WGLR_BUFFER_NOT_ENOUGH, __LINE__));
        return WGLR_BUFFER_NOT_ENOUGH;
    }
    
    /*get Edit-Box's Content*/
    return pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data,
                                                   WGL_CNT_EB_GET_TEXT, 
                                                   (VOID*)w2s_string, 
                                                   (VOID*)(UINT32)pt_edit->ui2_input_length);
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          change editbox CONTENT's rectangle
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_eb_change_cnt_rect(
    const WGL_EB_CONTROL_LOGIC_T*     pt_edit,
    const GL_RECT_T*                  pt_widget_rect,
    BOOL                              b_new_inset,
    const WGL_INSET_T*                pt_inset)
{
    GL_RECT_T                           t_cnt_rect;
    INT32                               i4_ret;
    
    /*check data*/
    WGL_CHECK_INTERNAL(pt_edit->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_edit->h_cnt_data);
    
    /*get cnt rect*/
    i4_ret = wgl_calc_cnt_rect(pt_edit->t_bdr_intf.pf_border_cmd_proc, pt_edit->h_bdr_data, pt_widget_rect, b_new_inset, pt_inset, &t_cnt_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to calc content rectangle. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*update content rect*/
    return pt_edit->t_cnt_intf.pf_content_cmd_proc(pt_edit->h_cnt_data, WGL_CNT_EB_SET_RECT, &t_cnt_rect, NULL);
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_COMMAND message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 _wgl_eb_notify_parent(
    HANDLE_T                        h_widget,
    WGL_EB_NFY_TYPE_T               e_type)
{
    /*local varaible declaration*/
    WGL_NOTIFY_DATA_T               t_nfy_data = {0};
    HANDLE_T                        h_parent = NULL_HANDLE;
    INT32                           i4_ret;
    
    /*check e_type*/
    WGL_ASSERT(_WGL_EB_NFY_TYPE_DATA_CHANGE == e_type || _WGL_EB_NFY_TYPE_OVER_MAX == e_type);

    /*get parent*/
    i4_ret = x_wgl_get_parent(h_widget, &h_parent);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(EB)} ERR: Fail to get widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    if(h_parent)
    {
        /*prepare notify data*/
        t_nfy_data.ui4_nc_code = (_WGL_EB_NFY_TYPE_DATA_CHANGE == e_type) ? WGL_NC_EB_CHANGED : WGL_NC_EB_OVER_MAX;
        
        /*send notify to its parent*/
        i4_ret = x_wgl_send_notify(h_parent, t_nfy_data.ui4_nc_code, h_widget, &t_nfy_data);
        WGL_EB_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(EB)} ERR: Fail to send notficiation to widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    
    return WMPR_DONE;
}
