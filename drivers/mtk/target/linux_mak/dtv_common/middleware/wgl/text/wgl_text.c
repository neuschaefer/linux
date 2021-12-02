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
 * $RCSfile: wgl_text.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: f9fca5f7fad27b92bdcb4fb60464106c $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/text/x_wgl_text.h"
#include "wgl/text/wgl_text.h"
#include "wgl/text/wgl_text_cnt.h"
#include "wgl/text/wgl_text_cli.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_text_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

#define _WGL_TEXT_DEF_KEY_LINE_UP     0
#define _WGL_TEXT_DEF_KEY_LINE_DOWN   0
#define _WGL_TEXT_DEF_KEY_PAGE_UP     BTN_CURSOR_UP
#define _WGL_TEXT_DEF_KEY_PAGE_DOWN   BTN_CURSOR_DOWN


typedef struct _WGL_TEXT_CONTROL_LOGIC_T
{
#ifdef CLI_LVL_ALL     
    UINT32                              ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    VOID*                               pv_attach;      /*attached data*/
    WGL_CONTENT_CTRL_INTF_T             t_cnt_intf;     /*content interface*/
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data;     /*content object*/
    WGL_BORDER_CTRL_INTF_T              t_bdr_intf;     /*border interface*/
    WGL_BORDER_CONTEXT_HANDLE_T         h_bdr_data;     /*border object*/
    WGL_TEXT_KEY_MAP_T                  t_key_map;      /*key map for scroll*/
    WGL_TEXT_PS_TYPE_T                  e_scroll_type;  /*method for scroll page*/
    UINT32                              ui4_style;      /*widget style*/
    UINT16                              ui2_scroll_lines; /*scroll line for user define method*/
    UINT16                              ui2_state;
    HANDLE_T                            h_timer;
    BOOL                                b_is_scrl_state;  /*single line scroll state*/
    
} WGL_TEXT_CONTROL_LOGIC_T;

/**/
typedef enum _WGL_TEXT_NFY_TYPE_T
{
    _WGL_TEXT_NFY_TYPE_NONE = 0,
    _WGL_TEXT_NFY_TYPE_KEY,
    _WGL_TEXT_NFY_TYPE_LINE_CHANGE
} WGL_TEXT_NFY_TYPE_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
 extern SIZE_T x_uc_w2s_strlen(
        const UTF16_T*  w2s_str);

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*WGL_MSG_PAINT message handler*/
static INT32 _wgl_text_msg_hdr_paint(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state);

/*WGL_MSG_KEY_DOWN & WGL_MSG_KEY_UP message handler*/
static INT32 _wgl_text_msg_hdr_key(
    HANDLE_T                            h_widget,
    UINT32                              ui4_k_code,
    UINT32                              ui4_k_state);

/*WGL_MSG_SIZE message handler*/
static INT32 _wgl_text_msg_hdr_size(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    UINT16                              ui2_width,
    UINT16                              ui2_height);

/*WGL_MSG_COMMAND message handler*/
static INT32 _wgl_text_msg_hdr_command(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    UINT32                              ui4_command_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2,
    VOID*                               pv_internal_cmd_data);
/*WGL_CMD_GL_GET_PREFERRED_SIZE command handler*/
static INT32 _wgl_text_cmd_hdr_get_preferred_size(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    GL_SIZE_T*                          pt_size);

/*WGL_CMD_TEXT_GET_MIN_SIZE command handler*/
static INT32 _wgl_text_cmd_hdr_get_min_size(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    GL_SIZE_T*                          pt_size);

static INT32 _wgl_text_cmd_hdr_set_key_map(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    const WGL_TEXT_KEY_MAP_T*           pt_key_map);

static INT32 _wgl_text_cmd_hdr_set_page_scrol(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    WGL_TEXT_PS_TYPE_T                  e_type,
    UINT16                              ui2_user_def_line);


static INT32 _wgl_text_cmd_hdr_get_page_scrol(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    WGL_TEXT_PS_TYPE_T*                 pe_type,
    UINT16*                             pui2_user_def_line);

static INT32 _wgl_text_process_key_event(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    UINT32                              ui4_k_code,
    UINT32                              ui4_k_state,
    WGL_TEXT_NFY_TYPE_T*                pe_notify_type);

static INT32 _wgl_text_scrol_page(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    BOOL                                b_a_page,
    BOOL                                b_up,
    BOOL*                               pb_start_change);
            
static INT32 _wgl_text_notify_start_line_change(
    HANDLE_T                            h_widget);

static INT32 _wgl_text_change_cnt_rect(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    const GL_RECT_T*                    pt_widget_rect,
    BOOL                                b_new_inset,
    const WGL_INSET_T*                  pt_inset);

static INT32 _wgl_text_begin_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    HANDLE_T                            h_widget,
    VOID*                               pv_param1);

static INT32 _wgl_text_do_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    VOID*                               pv_param1,
    HANDLE_T                            h_widget);

static INT32 _wgl_text_end_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    HANDLE_T                            h_widget);

static INT32 _wgl_text_query_and_update_text_dir(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    VOID*                               pv_internal_cmd_data);


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
INT32 x_wgl_text_init(
    UINT32                      ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_TEXT;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_text_inst_create, 
                                               wgl_text_inst_destroy, 
                                               wgl_text_default_proc,
                                               WGL_ATTR_FOCUSABLE};
    UINT32                      ui4_cnt_type = WGL_CONTENT_TEXT_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_text_cnt_inst_create,
                                               wgl_text_cnt_inst_destroy,
                                               wgl_text_cnt_command_proc,
                                               wgl_text_cnt_paint}; 
    INT32                       i4_ret;

    DBG_API(("{WGL(TEXT)} %s: ui4_init_type = 0x%.8x\n", __FUNCTION__, ui4_init_type));
    
    if(WGL_TEXT_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail ot register Component Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    if(WGL_TEXT_INIT_DEFAULT_CNT & ui4_init_type)
    {
        i4_ret = wgl_register_content_intf(ui4_cnt_type, &t_cnt_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail to register Content Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    i4_ret = wgl_text_cli_init();
    WGL_TEXT_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(TEXT)} ERR: Fail to init Text CLI. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
INT32 wgl_text_default_proc(
    HANDLE_T                    h_widget,
    UINT32                      ui4_msg,
    VOID*                       pv_param1,
    VOID*                       pv_param2)
{
    /*local variable declaration*/
    GL_RECT_T                   t_update_rect;
    WGL_TEXT_CONTROL_LOGIC_T*   pt_text = NULL;
    GL_RECT_T                   t_widget_rect = {0};
    VOID*                       pv_internal_cmd_data = NULL;
    INT32                       i4_ret = WMPR_DONE;
    INT32                       i4_temp;
    UINT32                      ui4_update_rect;

    DBG_API(("{WGL(TEXT)} %s: h_widget = 0x%.8x \tui4_msg = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
                __FUNCTION__, 
                h_widget, 
                ui4_msg,
                pv_param1,
                pv_param2));

    switch (ui4_msg)
    {
    case WGL_MSG_PAINT:
        {
            i4_ret = x_wgl_get_update_region(h_widget, &ui4_update_rect, &t_update_rect);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's update region. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        break;
        
    case WGL_MSG_GET_FOCUS:
        {   
            i4_ret = wgl_notify_parent_focus(h_widget, TRUE);
            if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Fail to notify widget's parent of getting focus. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
           break;
        }
        
    case WGL_MSG_LOSE_FOCUS:
        {   
            i4_ret = wgl_notify_parent_focus(h_widget, FALSE);
             if(WGLR_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Fail to notify widget's parent of losing focus. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
            break;
        }       
        
    case WGL_MSG_KEY_DOWN:
        return _wgl_text_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_DOWN);
        
    case WGL_MSG_KEY_UP:
        return _wgl_text_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_UP);

    case WGL_MSG_COMMAND:
        {
            if(WGL_CMD_GL_SET_INSET == (UINT32)pv_param1)   /*need to get widget's rectangle before lock it*/
            {
                i4_ret = x_wgl_get_coords(h_widget, WGL_COORDS_LOCAL, &t_widget_rect);
                if(WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's dimension. (%d) at L%d\n\n", i4_ret, __LINE__));
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

    /*Get text data*/
    i4_ret = wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_text);
    if(WGLR_OK != i4_ret)
    {
        if (WGL_MSG_DESTROY !=ui4_msg)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's instance. (%d) at L%d,ui4_msg=%d\n\n", i4_ret, __LINE__,ui4_msg));
        }        
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_text)
    {
        i4_temp = wgl_release_instance(h_widget, pt_text);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(TEXT)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    switch (ui4_msg)
    {
    case WGL_MSG_PAINT:
         {
            i4_ret = _wgl_text_query_and_update_text_dir(h_widget,
                                                      pt_text,
                                                      pv_internal_cmd_data);
            if(i4_ret == WGLR_OK)
            {
                i4_ret = _wgl_text_msg_hdr_paint(h_widget, 
                                         pt_text, 
                                         (GL_HGRAPHICS_T)pv_param1, 
                                         ui4_update_rect, 
                                         &t_update_rect, 
                                         WGL_POINTER_TO_UINT16(pv_param2));
            }
            else if (i4_ret == WGLR_INV_HANDLE)
            {
                /*in this case, wgl_get_instance() return failed so do not release instance before return*/
                return i4_ret;
            }
        break;
        }
    case WGL_MSG_GET_FOCUS:
        {
            WGL_SET_FLAG(pt_text->ui2_state, WGL_STATE_FOCUSED);
            i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                          pt_text->h_cnt_data, 
                          WGL_CNT_TEXT_SET_STATE, 
                          WGL_PACK(pt_text->ui2_state), 
                          NULL);
        }
    case WGL_MSG_LOSE_FOCUS:
        {
            WGL_CLEAR_FLAG(pt_text->ui2_state, WGL_STATE_FOCUSED);
            i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                          pt_text->h_cnt_data, 
                          WGL_CNT_TEXT_SET_STATE, 
                          WGL_PACK(pt_text->ui2_state), 
                          NULL);
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
        i4_ret = _wgl_text_msg_hdr_size(h_widget, 
                                        pt_text, 
                                        WGL_GET_HI_WORD((UINT32)pv_param1), 
                                        WGL_GET_LOW_WORD((UINT32)pv_param1));
        break;

    case WGL_MSG_ENABLE:
        {
            WGL_UPDATE_STATE(pt_text->ui2_state, WGL_POINTER_TO_BOOL(pv_param1), WGL_STATE_ENABLED);
            i4_ret = WMPR_DONE;
        }
        break;

    case WGL_MSG_NOTIFY:
        {
            WGL_ASSERT(0);  /*text static should not receive this message*/
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL(TEXT)} ERR: Text should not receive any notification. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        break;

    case WGL_MSG_COMMAND:
        {
            if(NULL == pv_param2)
            {
                i4_ret = WGLR_INV_ARG;
                DBG_ERROR(("{WGL(TEXT)} ERR: The 2nd parameter of WGL_MSG_COMMAND should not be NULL. (%d) at L%d\n\n", i4_ret, __LINE__));
            }
            else
            {
                /*these commands will use text direction in their implementation*/
                if(((UINT32)pv_param1) == WGL_CMD_TEXT_GET_TOTAL_LINES ||
                    ((UINT32)pv_param1) == WGL_CMD_TEXT_PAGE_SCROL ||
                    ((UINT32)pv_param1) == WGL_CMD_TEXT_BEGIN_SCRL ||
                    ((UINT32)pv_param1) == WGL_CMD_TEXT_END_SCRL)
                {
                    i4_ret = _wgl_text_query_and_update_text_dir(h_widget,
                                                      pt_text,
                                                      pv_internal_cmd_data);
                    if(i4_ret != WGLR_OK)
                    {
                        if (i4_ret == WGLR_INV_HANDLE)
                        {
                            /*in this case, wgl_get_instance() return failed so do not release instance before return*/
                            return i4_ret;
                        }
                        break;
                    }
                    
                }
                i4_ret = _wgl_text_msg_hdr_command(h_widget, 
                                                   pt_text, 
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
            i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_UPDATE_CVSST, pv_param1, NULL);
        }
        break;    
        
    case WGL_MSG_TIMER:
        {
            i4_ret = _wgl_text_query_and_update_text_dir(h_widget,
                                                      pt_text,
                                                      pv_internal_cmd_data);
            if(i4_ret == WGLR_OK)
            {
                /*this command's implementation will use text direction*/
                i4_ret = _wgl_text_do_scrl(pt_text,pv_param1,h_widget);       
            }else if (i4_ret == WGLR_INV_HANDLE)
            {
                /*in this case, wgl_get_instance() return failed so do not release instance before return*/
                return i4_ret;
            }
        break;
        }
    default:
        i4_ret = WGLR_INV_MSG;
        DBG_ERROR(("{WGL(TEXT)} ERR: Unknown message type. (%d) at L%d\n\n", i4_ret, __LINE__));
        break;
    }
    i4_temp = wgl_release_instance(h_widget, pt_text);
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
INT32 wgl_text_inst_create( 
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
    WGL_TEXT_CONTROL_LOGIC_T*       pt_ctrl_logic;
    WGL_INSET_T                     t_bdr_inset;
    WGL_CANVAS_STATUS_T             t_cvsst;
    INT32                           i4_ret;
    
    /*check parameter*/
    if(NULL == pt_ctx_wgt || NULL == pt_wgt_create)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*initialize canvas status*/
    wgl_cvsst_update(&t_cvsst, &pt_wgt_create->t_cvs_info, pt_wgt_create->h_palette);

    /*query content factory*/
    i4_ret = wgl_query_content_intf(pt_wgt_create->ui4_type_content, &t_cnt_intf);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get content's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_TYPE;
    }
    /*check content interface*/
    WGL_CHECK_INTERNAL(t_cnt_intf.pf_content_cmd_proc 
               && t_cnt_intf.pf_create_instance 
               && t_cnt_intf.pf_destroy_instance 
               && t_cnt_intf.pf_paint_content);
    
    /*query border factory*/
    i4_ret = wgl_query_border_intf(pt_wgt_create->ui4_type_border, &t_bdr_intf);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get border's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_TYPE;
    }

    /*allocate memory for control logic object*/
    pt_ctrl_logic = (WGL_TEXT_CONTROL_LOGIC_T*) WGL_MEM_ALLOC(sizeof(WGL_TEXT_CONTROL_LOGIC_T));
    if(NULL == pt_ctrl_logic)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to alloc memory for control logic. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize control logic object*/
    x_memset(pt_ctrl_logic, 0, sizeof(WGL_TEXT_CONTROL_LOGIC_T));
    
#ifdef CLI_LVL_ALL
         pt_ctrl_logic->ui4_mem_size = sizeof(WGL_TEXT_CONTROL_LOGIC_T);   
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
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail to create border object. (%d) at L%d\n\n", i4_ret, __LINE__));
            goto _err_out;
        }

        /*get default border inset*/
        i4_ret = t_bdr_intf.pf_border_cmd_proc(h_bdr_data, WGL_CMD_GL_GET_INSET, &t_bdr_inset, NULL);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get border's inset. (%d) at L%d\n\n", i4_ret, __LINE__));
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
        DBG_ERROR(("{WGL(TEXT)} ERR: NULL-Border cannot be with WGL_STL_GL_BDR_FILL_CNT_BK style. (%d) at L%d\n\n", i4_ret, __LINE__));
        i4_ret = WGLR_NO_BORDER;
        goto _err_out;
    }

    /*create content object*/
    WGL_RECT_COPY(&t_cnt_create_input.t_rect_cnt, &t_cnt_rect);
    t_cnt_create_input.pt_cvsst = &t_cvsst;
    t_cnt_create_input.pv_init = pt_wgt_create->pv_param; /*widget style*/;
    i4_ret = t_cnt_intf.pf_create_instance(&h_cnt_data, &t_cnt_create_input);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to create content object. (%d) at L%d\n\n", i4_ret, __LINE__));
        goto _err_out;
    }

    /*store border object in control logic object*/
    x_memcpy(&pt_ctrl_logic->t_bdr_intf, &t_bdr_intf, sizeof(WGL_BORDER_CTRL_INTF_T));
    pt_ctrl_logic->h_bdr_data = h_bdr_data;

    /*store cotent object in control logic object*/
    x_memcpy(&pt_ctrl_logic->t_cnt_intf, &t_cnt_intf, sizeof(WGL_CONTENT_CTRL_INTF_T));
    pt_ctrl_logic->h_cnt_data = h_cnt_data;

    /*init key map*/
    pt_ctrl_logic->t_key_map.ui4_line_up = _WGL_TEXT_DEF_KEY_LINE_UP;
    pt_ctrl_logic->t_key_map.ui4_line_down = _WGL_TEXT_DEF_KEY_LINE_DOWN;
    pt_ctrl_logic->t_key_map.ui4_page_up = _WGL_TEXT_DEF_KEY_PAGE_UP;
    pt_ctrl_logic->t_key_map.ui4_page_down = _WGL_TEXT_DEF_KEY_PAGE_DOWN;
    
    /*init scroll type*/
    pt_ctrl_logic->e_scroll_type = WGL_TEXT_PS_TYPE_KEEP_LAST;
    pt_ctrl_logic->ui2_scroll_lines = 1;

    /*initialize other info*/
    pt_ctrl_logic->ui4_style = (UINT32)pt_wgt_create->pv_param;
    /*pt_ctrl_logic->ui2_state = (pt_ctrl_logic->ui4_style & WGL_STL_GL_DISABLED) ? 0 : WGL_STATE_ENABLED;*/
    pt_ctrl_logic->ui2_state = WGL_STATE_ENABLED;

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
INT32 wgl_text_inst_destroy( 
    WGL_WIDGET_CONTEXT_HANDLE_T     t_ctx_wgt)
{
    /*local variable parameter*/
    WGL_TEXT_CONTROL_LOGIC_T*       pt_ctrl_logic;
    INT32                           i4_ret;

    /*check parameter*/
    if(NULL == t_ctx_wgt)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    pt_ctrl_logic = (WGL_TEXT_CONTROL_LOGIC_T*)t_ctx_wgt;
    if (pt_ctrl_logic->b_is_scrl_state)
    {
       i4_ret = x_wgl_stop_timer(pt_ctrl_logic->h_timer);
       if (WGLR_OK != i4_ret)
       {
          DBG_ERROR(("{WGL(TEXT)} ERR: Fail to delete timer. i4_ret=%d,File=%s,Line=%d\n\n", i4_ret, __FILE__, __LINE__));
          return i4_ret;
       }   
    }    

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
        WGL_CHECK_INTERNAL(pt_ctrl_logic->t_bdr_intf.pf_destroy_instance); /*if border-data exists, border-interface should not be NULL*/
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
static INT32 _wgl_text_msg_hdr_paint(
    HANDLE_T                        h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*       pt_text,
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
        DBG_ERROR(("{WGL(TEXT)} ERR: GRAPHICS object is NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check content*/
    WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_paint_content);
    WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
    
    if(pt_text->t_bdr_intf.pf_paint_border)
    {
        /*check border*/
        WGL_CHECK_INTERNAL(pt_text->h_bdr_data);

        /*paint border*/
        i4_ret = pt_text->t_bdr_intf.pf_paint_border(pt_text->h_bdr_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    /*paint content*/
    return pt_text->t_cnt_intf.pf_paint_content(pt_text->h_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
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
static INT32 _wgl_text_msg_hdr_key(
    HANDLE_T                        h_widget,
    UINT32                          ui4_k_code,
    UINT32                          ui4_k_state)
{
    /*local varaible declaration*/
    WGL_TEXT_CONTROL_LOGIC_T*       pt_text = NULL;
    INT32                           i4_ret, i4_temp;
    WGL_TEXT_NFY_TYPE_T             e_notify_type;    

    /*Get btn data and lock instance*/
    i4_ret = wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_text);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_text)
    {
        i4_temp = wgl_release_instance(h_widget, pt_text);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(TEXT)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*handle key event*/
    i4_ret = _wgl_text_process_key_event(h_widget, pt_text, ui4_k_code, ui4_k_state, &e_notify_type);
    if(WGLR_INV_HANDLE == i4_ret)
    {
        /*in this case, wgl_get_instance() return failed so do not release instance before return*/
        return i4_ret;
    }

    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_text);
    WGL_ASSERT(WGLR_OK == i4_temp);

    /*check return value*/
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
        
    /*notify parent*/
    switch (e_notify_type)
    {
    case _WGL_TEXT_NFY_TYPE_KEY:
        {
            i4_ret = wgl_notify_parent_key(h_widget, ui4_k_code, ui4_k_state);
            WGL_TEXT_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(TEXT)} ERR: Fail to notify widget's parent of key state. (%d) at L%d\n\n", i4_ret, __LINE__)));
            return i4_ret;
        }

    case _WGL_TEXT_NFY_TYPE_LINE_CHANGE:
        return _wgl_text_notify_start_line_change(h_widget);

    case _WGL_TEXT_NFY_TYPE_NONE:
        return WGLR_KEY_MSG_NOT_PROCESSED;
    default:
        return WGLR_OK;
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_SIZE message handler. Not check h_widget and pt_text
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_msg_hdr_size(
    HANDLE_T                        h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*       pt_text,
    UINT16                          ui2_width,
    UINT16                          ui2_height)
{
    /*local variable declaration*/
    GL_RECT_T                       t_widget_rect = {0};
    INT32                           i4_ret;
    
    /*initialize content rect*/
    t_widget_rect.i4_right = (INT32)ui2_width;
    t_widget_rect.i4_bottom = (INT32)ui2_height;

    if(pt_text->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_ASSERT(pt_text->h_bdr_data);
        
        /*notify border to update its rectangle*/
        i4_ret = pt_text->t_bdr_intf.pf_border_cmd_proc(pt_text->h_bdr_data, WGL_BDR_GL_SET_SIZE, (VOID*)(INT32)ui2_width, (VOID*)(INT32)ui2_height);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    
    return _wgl_text_change_cnt_rect(pt_text, &t_widget_rect, FALSE, NULL);
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
static INT32 _wgl_text_msg_hdr_command(
    HANDLE_T                        h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*       pt_text,
    UINT32                          ui4_command_id,
    VOID*                           pv_param1,
    VOID*                           pv_param2,
    VOID*                           pv_internal_cmd_data)
{
    INT32                           i4_ret;
    BOOL                            b_start_change;

    DBG_API(("{WGL(TEXT)} %s: h_widget = 0x%.8x \tui4_command_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
            __FUNCTION__, 
            h_widget, 
            ui4_command_id,
            pv_param1,
            pv_param2));

    /*check command*/
    if(! (WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_TEXT, ui4_command_id) 
            || WGL_IS_CMD_GENERAL(ui4_command_id)
            || WGL_IS_CONTENT_CMD(ui4_command_id) 
            || WGL_IS_BORDER_CMD(ui4_command_id)))
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Unknown Command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    /*get command handler*/
    switch (ui4_command_id)
    {
    case WGL_CMD_GL_SET_ATTACH_DATA:
        {
            pt_text->pv_attach = pv_param1;
        }
        break;

    case WGL_CMD_GL_GET_ATTACH_DATA:
        {
            if(NULL == (VOID**)pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ATTACH_DATA, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
                
            *((VOID**)pv_param1) = pt_text->pv_attach;
        }
        break;

    case WGL_CMD_TEXT_SET_KEY_MAP:
        return _wgl_text_cmd_hdr_set_key_map(pt_text, (WGL_TEXT_KEY_MAP_T*)pv_param1);

    case WGL_CMD_TEXT_GET_KEY_MAP:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. For WGL_CMD_TEXT_GET_KEY_MAP, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            x_memcpy((WGL_TEXT_KEY_MAP_T*)pv_param1, &pt_text->t_key_map, sizeof(WGL_TEXT_KEY_MAP_T));
        }
        break;

    case WGL_CMD_TEXT_SET_PAGE_SCROL:
        return _wgl_text_cmd_hdr_set_page_scrol(pt_text, (WGL_TEXT_PS_TYPE_T)(INT32)pv_param1, WGL_POINTER_TO_UINT16(pv_param2));

    case WGL_CMD_TEXT_GET_PAGE_SCROL:
        return _wgl_text_cmd_hdr_get_page_scrol(pt_text, (WGL_TEXT_PS_TYPE_T*)pv_param1, (UINT16*)pv_param2);

    case WGL_CMD_TEXT_PAGE_SCROL:
        {
            i4_ret = _wgl_text_scrol_page(pt_text, TRUE, WGL_POINTER_TO_BOOL(pv_param1), &b_start_change);
            if(WGLR_OK == i4_ret && WGL_POINTER_TO_BOOL(pv_param2) && b_start_change)
            {
                x_wgl_repaint(h_widget, NULL, FALSE);
            }

            return i4_ret;
        }
	case WGL_CMD_GL_GET_PREFERRED_SIZE:
			return _wgl_text_cmd_hdr_get_preferred_size(pt_text, (GL_SIZE_T*)pv_param1);

    case WGL_CMD_TEXT_GET_MIN_SIZE:
        return _wgl_text_cmd_hdr_get_min_size(pt_text, (GL_SIZE_T*)pv_param1);

    case WGL_CMD_TEXT_SET_DISPLAY_START:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_SET_DISPLAY_START, pv_param1, pv_param2);
        }
	case WGL_CMD_TEXT_SET_DISPLAY_START_EX:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_SET_DISPLAY_START_EX, pv_param1, pv_param2);
        }
        
    case WGL_CMD_TEXT_GET_DISPLAY_START:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_GET_DISPLAY_START, pv_param1, pv_param2);
        }
        
        
    case WGL_CMD_TEXT_GET_TOTAL_LINES:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_GET_TOTAL_LINES, pv_param1, WGL_PACK(pt_text->ui2_state));
        }
        
        
    case WGL_CMD_TEXT_GET_PAGE_LINES:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_GET_PAGE_LINES, pv_param1, WGL_PACK(pt_text->ui2_state));
        }
    case WGL_CMD_TEXT_GET_MAX_LINE_WIDTH:
        {
            /*pass to content directly*/
            /*check content*/
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
            
            return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_MAX_LINE_WIDTH, pv_param1, WGL_PACK(pt_text->ui2_state));
        }

    case WGL_CMD_GL_SET_INSET:
        {
            DBG_INFO(("{WGL(TEXT)} set-inset(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n", 
                        pv_param1,
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_left),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_top),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_right),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_bottom)));

            /*[DTV00000540] For frame, icon, text, editbox, progressbar, and button widgets, the content background show incorrectly if the inset size  5  with uniform border*/
            /*Comments: widget component has to update CONTENT rectangle when BORDER rectangle changes.*/
            return _wgl_text_change_cnt_rect(pt_text, (GL_RECT_T*)pv_internal_cmd_data, TRUE, (WGL_INSET_T*)pv_param1);
        }
    case WGL_CMD_TEXT_BEGIN_SCRL:
        return _wgl_text_begin_scrl(pt_text,h_widget,pv_param1);        
    case WGL_CMD_TEXT_END_SCRL:
        return _wgl_text_end_scrl(pt_text,h_widget); 
    case WGL_CMD_GL_SET_TEXT_DIRECTION:
        {
            INT32 i4_ret;
            WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
          
            i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, ui4_command_id, pv_param1, pv_param2);
            if(i4_ret == WGLR_OK)
            {
                if(NULL != pt_text->t_bdr_intf.pf_border_cmd_proc)
                {
                    /*check border*/
                    WGL_ASSERT(pt_text->h_bdr_data);
                    return pt_text->t_bdr_intf.pf_border_cmd_proc(pt_text->h_bdr_data, WGL_CMD_GL_SET_BDR_TEXT_DIRECTION, pv_param1, pv_param2);
                }
                else
                {
                    return WGLR_OK;
                }
            }
            else
            {
                return i4_ret;
            }
            
        }
#ifdef CLI_LVL_ALL
    case WGL_CMD_GL_GET_ALLOC_MEM:
    {
        UINT32 ui4_wdg_mem = pt_text->ui4_mem_size;
        WGL_MTX_OBJ_T*  pt_wgt_mem;
        if(NULL == pv_param1)
        {
                DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
        }
        
        pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_text->h_cnt_data;
        ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;
        if(pt_text->t_bdr_intf.pf_create_instance != NULL)
        {
            pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_text->h_bdr_data;
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
                WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
                WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
                
                return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, ui4_command_id, pv_param1, pv_param2);
            }
            else if(WGL_IS_BORDER_CMD(ui4_command_id))/*border command*/
            {
                if(NULL == pt_text->t_bdr_intf.pf_border_cmd_proc)
                {
                    DBG_ERROR(("{WGL(TEXT)} ERR: Border's command procedure is NULL. (%d) at L%d\n\n", WGLR_NO_BORDER, __LINE__));
                    return WGLR_NO_BORDER;
                }

                /*check border*/
                WGL_CHECK_INTERNAL(pt_text->h_bdr_data);
                return pt_text->t_bdr_intf.pf_border_cmd_proc(pt_text->h_bdr_data, ui4_command_id, pv_param1, pv_param2);
            }
            else
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
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
 *          WGL_CMD_GL_GET_PREFERRED_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_cmd_hdr_get_preferred_size(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    GL_SIZE_T*                          pt_size)
{
    /*local variable declaration*/
    GL_SIZE_T                           t_bdr_prefer_size = {0};
    INT32                               i4_ret;

    /*check content*/
    WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_text->h_cnt_data);

    if(pt_text->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_CHECK_INTERNAL(pt_text->h_bdr_data);

        WGL_ASSERT(0); 

        /*get border min size*/
        i4_ret = WGLR_FUNC_NOT_IMPL;
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: This command isn't exported in this version. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    /*Get Content Size*/
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_GET_PREFERRED_SIZE, pt_size, NULL);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*sum the min size*/
    pt_size->ui4_width += t_bdr_prefer_size.ui4_width;
    pt_size->ui4_height += t_bdr_prefer_size.ui4_height;

    /*update content rect*/
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
static INT32 _wgl_text_cmd_hdr_get_min_size(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    GL_SIZE_T*                          pt_size)
{
    /*local variable declaration*/
    GL_SIZE_T                           t_bdr_min_size = {0};
    INT32                               i4_ret;

    /*check content*/
    WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_text->h_cnt_data);

    if(pt_text->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_CHECK_INTERNAL(pt_text->h_bdr_data);

        WGL_ASSERT(0); 

        /*get border min size*/
        i4_ret = WGLR_FUNC_NOT_IMPL;
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: This command isn't exported in this version. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    /*Get Content Size*/
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_GET_MIN_SIZE, pt_size, NULL);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*sum the min size*/
    pt_size->ui4_width += t_bdr_min_size.ui4_width;
    pt_size->ui4_height += t_bdr_min_size.ui4_height;

    /*update content rect*/
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
static INT32 _wgl_text_cmd_hdr_set_key_map(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    const WGL_TEXT_KEY_MAP_T*           pt_key_map)
{
    DBG_INFO(("{WGL(TEXT)} pt_key_map = 0x%.8x\n"
            "(line_down, line_up) = (0x%.8x, 0x%.8x)\n"
            "(page_down, page_up) = (0x%.8x, 0x%.8x)\n",
            pt_key_map,
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_line_down),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_line_up),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_page_down),
            ((NULL == pt_key_map) ? 0 : pt_key_map->ui4_page_up)));

    if(NULL == pt_key_map)
    {
        pt_text->t_key_map.ui4_line_down = _WGL_TEXT_DEF_KEY_LINE_DOWN;
        pt_text->t_key_map.ui4_line_up = _WGL_TEXT_DEF_KEY_LINE_UP;
        pt_text->t_key_map.ui4_page_down = _WGL_TEXT_DEF_KEY_PAGE_DOWN;
        pt_text->t_key_map.ui4_page_up = _WGL_TEXT_DEF_KEY_PAGE_UP;
    }
    else
    {
        x_memcpy(&pt_text->t_key_map, pt_key_map, sizeof(WGL_TEXT_KEY_MAP_T));
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
static INT32 _wgl_text_cmd_hdr_set_page_scrol(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    WGL_TEXT_PS_TYPE_T                  e_type,
    UINT16                              ui2_user_def_line)
{
    if(WGL_TEXT_PS_TYPE_KEEP_LAST == e_type || WGL_TEXT_PS_TYPE_REPLACE_ALL == e_type)
    {
        pt_text->e_scroll_type = e_type;
    }
    else if(WGL_TEXT_PS_TYPE_USER_DEF == e_type)
    {
        pt_text->e_scroll_type = e_type;
        pt_text->ui2_scroll_lines = ui2_user_def_line;
    }
    else
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. Legal type is WGL_TEXT_PS_TYPE_KEEP_LAST, WGL_TEXT_PS_TYPE_REPLACE_ALL, or WGL_TEXT_PS_TYPE_USER_DEF. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
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
static INT32 _wgl_text_cmd_hdr_get_page_scrol(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    WGL_TEXT_PS_TYPE_T*                 pe_type,
    UINT16*                             pui2_user_def_line)
{
    if(NULL == pe_type || NULL == pui2_user_def_line)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. Output arguments cannot be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    *pe_type = pt_text->e_scroll_type;
    if(WGL_TEXT_PS_TYPE_KEEP_LAST == *pe_type || WGL_TEXT_PS_TYPE_REPLACE_ALL == *pe_type)
    {
        *pui2_user_def_line = 0;
    }
    else
    {
        *pui2_user_def_line = pt_text->ui2_scroll_lines;
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
static INT32 _wgl_text_scrol_page(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    BOOL                                b_a_page,
    BOOL                                b_up,
    BOOL*                               pb_start_change)
{
    /*local variable declaration*/
    UINT16                              ui2_total_lines = 0, ui2_page_lines = 0, ui2_cur_line = 0, ui2_new_line = 0, ui2_scroll_lines = 0,ui2_max_dsp_lines = 0;
    INT32                               i4_ret;

    /*init return value*/
    if(pb_start_change)
    {
        *pb_start_change = FALSE;
    }
    
    /*check content*/
    WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
    
    /*get current line*/
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                      pt_text->h_cnt_data, 
                      WGL_CNT_TEXT_GET_DISPLAY_START, 
                      &ui2_cur_line, 
                      NULL);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*get total lines*/
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                      pt_text->h_cnt_data, 
                      WGL_CNT_TEXT_GET_TOTAL_LINES, 
                      &ui2_total_lines, 
                      WGL_PACK(pt_text->ui2_state));
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*page down or page up*/
    if(b_a_page)
    {
        /*get page lines*/
        i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                          pt_text->h_cnt_data, 
                          WGL_CNT_TEXT_GET_PAGE_LINES, 
                          &ui2_page_lines, 
                          WGL_PACK(pt_text->ui2_state));
     
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                          pt_text->h_cnt_data, 
                          WGL_CNT_TEXT_GET_MAX_DIS_LINE, 
                          &ui2_max_dsp_lines, 
                          NULL);
     
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        if (ui2_page_lines > ui2_max_dsp_lines)
        {
            ui2_page_lines = ui2_max_dsp_lines; 
        }
        
        if(WGL_TEXT_PS_TYPE_KEEP_LAST == pt_text->e_scroll_type)
        {
            ui2_scroll_lines = (ui2_page_lines == 0) ? (UINT16)0 : ui2_page_lines - (UINT16)1;
        }
        else if(WGL_TEXT_PS_TYPE_REPLACE_ALL == pt_text->e_scroll_type)
        {
            ui2_scroll_lines = ui2_page_lines;
        }
        else
        {
            ui2_scroll_lines = pt_text->ui2_scroll_lines;
        }
        
        if(!b_up)
        {
            if(WGL_TEXT_PS_TYPE_KEEP_LAST == pt_text->e_scroll_type)
            {
                ui2_new_line = ((ui2_cur_line + ui2_scroll_lines) >= (ui2_total_lines - (UINT16)1)) 
                    ? ui2_cur_line : ui2_cur_line + ui2_scroll_lines;
            }
            else
            {
                ui2_new_line = ((ui2_cur_line + ui2_scroll_lines) > (ui2_total_lines - (UINT16)1)) 
                    ? ui2_cur_line : ui2_cur_line + ui2_scroll_lines;
            }
        }
        else /*page_up*/
        {
            ui2_new_line = (ui2_cur_line < ui2_scroll_lines) 
                ? 0 : ui2_cur_line - ui2_scroll_lines;
        }
    }
    else /*line down or line up*/
    {
        if(!b_up)
        {
            ui2_new_line = (ui2_cur_line == (ui2_total_lines - (UINT16)1)) 
                ? ui2_cur_line : ui2_cur_line + (UINT16)1;
        }
        else /*line_up*/
        {
            ui2_new_line = (ui2_cur_line == (UINT16)0) ?(UINT16)0 : ui2_cur_line - (UINT16)1;
        }
    }
    
    if(ui2_new_line != ui2_cur_line)
    {
        i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(
                          pt_text->h_cnt_data, 
                          WGL_CNT_TEXT_SET_DISPLAY_START, 
                          (VOID*)(UINT32)ui2_new_line, 
                          NULL);
        if(WGLR_OK == i4_ret && pb_start_change)
        {
            *pb_start_change = TRUE;
        }
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
static INT32 _wgl_text_process_key_event(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    UINT32                              ui4_k_code,
    UINT32                              ui4_k_state,
    WGL_TEXT_NFY_TYPE_T*                pe_notify_type)
{
    /*local variable declaration*/
    BOOL                                b_a_page, b_up, b_start_change;
    INT32                               i4_ret;

    /*initial return value*/
    *pe_notify_type = _WGL_TEXT_NFY_TYPE_NONE;

    /*check whether the key is supported by this handler*/
    if(ui4_k_code != pt_text->t_key_map.ui4_page_down 
         && ui4_k_code != pt_text->t_key_map.ui4_page_up
         && ui4_k_code != pt_text->t_key_map.ui4_line_down 
         && ui4_k_code != pt_text->t_key_map.ui4_line_up)
    {
        *pe_notify_type = _WGL_TEXT_NFY_TYPE_KEY;
        return WGLR_OK;
    }

    /*ignore key_up*/
    if(WGL_KEY_STATE_UP == ui4_k_state)
    {
        return WGLR_OK;
    }

    /*handle key event*/
    b_a_page = (ui4_k_code == pt_text->t_key_map.ui4_page_down || ui4_k_code == pt_text->t_key_map.ui4_page_up) ? TRUE : FALSE;
    b_up = (ui4_k_code == pt_text->t_key_map.ui4_page_up || ui4_k_code == pt_text->t_key_map.ui4_line_up) ? TRUE : FALSE;

    /*_wgl_text_scrol_page() will use text direction,so update text direction first*/
    i4_ret = _wgl_text_query_and_update_text_dir(h_widget,pt_text,NULL);
    if(WGLR_OK != i4_ret)
    {
       return i4_ret;
    }

    /*handle page/line up/down*/
    i4_ret = _wgl_text_scrol_page(pt_text, b_a_page, b_up, &b_start_change);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*notify parent?*/
    if(b_start_change)
    {
        /*async paint*/
        x_wgl_repaint(h_widget, NULL, FALSE);

        *pe_notify_type = _WGL_TEXT_NFY_TYPE_LINE_CHANGE;
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
 * Returns
 ******************************************************************************/
static INT32 _wgl_text_notify_start_line_change(
    HANDLE_T                            h_widget)
{
    /*local varaible declaration*/
    WGL_NOTIFY_DATA_T               t_nfy_data = {0};
    HANDLE_T                        h_parent = NULL_HANDLE;
    INT32                           i4_ret;
        
    /*get parent*/
    i4_ret = x_wgl_get_parent(h_widget, &h_parent);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    if(h_parent)
    {
        /*prepare notify data*/
        t_nfy_data.ui4_nc_code = WGL_NC_TEXT_START_LINE_CHANGE;
        
        /*send notify to its parent*/
        i4_ret = x_wgl_send_notify(h_parent, t_nfy_data.ui4_nc_code, h_widget, &t_nfy_data);
        WGL_TEXT_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(TEXT)} ERR: Fail to send notficiation to widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    
    return WMPR_DONE;
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
static INT32 _wgl_text_change_cnt_rect(
    const WGL_TEXT_CONTROL_LOGIC_T*     pt_text,
    const GL_RECT_T*                    pt_widget_rect,
    BOOL                                b_new_inset,
    const WGL_INSET_T*                  pt_inset)
{
    GL_RECT_T                           t_cnt_rect;
    INT32                               i4_ret;
    
    /*check data*/
    WGL_CHECK_INTERNAL(pt_text->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_text->h_cnt_data);
    
    /*get cnt rect*/
    i4_ret = wgl_calc_cnt_rect(pt_text->t_bdr_intf.pf_border_cmd_proc, pt_text->h_bdr_data, pt_widget_rect, b_new_inset, pt_inset, &t_cnt_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to calc content rectangle. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*update content rect*/
    return pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_SET_RECT, &t_cnt_rect, NULL);
}

static INT32 _wgl_text_begin_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    HANDLE_T                            h_widget,
    VOID*                               pv_param1)
{    
    INT32                               i4_ret;    
    
    /*null argument check*/
    if (pv_param1 == NULL)
    { 
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid null argument. pv_param1 File=%s,Line=%d\n\n",  __FILE__,__LINE__));
        return WGLR_INV_ARG;
    }
    /*delay time check*/
    if (((WGL_TEXT_SCRL_T*)pv_param1)->ui4_ms_delay < WGL_TEXT_SCRL_MIN_DELAY)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid argument ui4_ms_delay=%d, pv_param1 File=%s,Line=%d\n\n", ((WGL_TEXT_SCRL_T*)pv_param1)->ui4_ms_delay, __FILE__,__LINE__));
        return WGLR_INV_ARG;
    }
    /*scroll style check*/
    if (((WGL_TEXT_SCRL_T*)pv_param1)->ui2_style > 15)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Invalid argument. ui2_scrl_style =%d,File=%s,Line=%d\n\n", ((WGL_TEXT_SCRL_T*)pv_param1)->ui2_style, __FILE__,__LINE__));
        return WGLR_INV_ARG;
    }
    /*doesn't support scrolling for multiline style*/
    if (pt_text->ui4_style & WGL_STL_TEXT_MULTILINE)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Does not support scrolling in multi line mode. File=%s,Line=%d\n\n",  __FILE__,__LINE__));
        return WGLR_INV_STATE;
    }   
    /*report error if already scrolling*/
    if (pt_text->b_is_scrl_state)
    {
        DBG_ERROR(("{WGL(TEX)} ERR: This widget is already scrolling. File=%s,Line=%d\n\n",  __FILE__,__LINE__));
        return WGLR_INV_STATE;       
    }    
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_BEGIN_SCRL_CHK, pv_param1, WGL_PACK(pt_text->ui2_state));
    if (i4_ret != WGLR_OK)
    {
        return i4_ret;
    }
    if (((WGL_TEXT_SCRL_T*)pv_param1)->ui2_style & WGL_TEXT_SCRL_SMART)
    {       
        i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_SCRL_SMART_CHK, pv_param1, WGL_PACK(pt_text->ui2_state));      
        if (i4_ret != WGLR_OK)
        {
            /*if style is smart cut but text not extend widget width, just do nothing*/
            return i4_ret;
        }  

    }
        
    if (pt_text->h_timer == NULL_TIME)
    {
        if ( ((WGL_TEXT_SCRL_T*)pv_param1)->b_sync){
             i4_ret = x_wgl_create_timer(h_widget,WGL_TIMER_SYNC,&(pt_text->h_timer));
        }
        else
        {
             i4_ret = x_wgl_create_timer(h_widget,WGL_TIMER_ASYNC,&(pt_text->h_timer));
        }
       
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(TEXT)} ERR: Fail to create timer. i4_ret =%d,File=%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
            return i4_ret;
        }
    }
    i4_ret = x_wgl_start_timer(pt_text->h_timer,((WGL_TEXT_SCRL_T*)pv_param1)->ui4_ms_delay,X_TIMER_FLAG_REPEAT,NULL);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to start timer. i4_ret =%d,File=%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
        return i4_ret;
    }
    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_BEGIN_SCRL, pv_param1, NULL);
    pt_text->b_is_scrl_state = TRUE;  
    return WGLR_OK;
}

static INT32 _wgl_text_do_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    VOID*                               pv_param1,
    HANDLE_T                            h_widget)
{
    HANDLE_T                            h_parent;
    GL_RECT_T                           t_rect;
    INT32                               i4_ret;

    i4_ret = pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_DO_SCRL, pv_param1,WGL_PACK(pt_text->ui2_state));
    if (i4_ret == WGL_TEXT_SCRL_END)
    {
        return _wgl_text_end_scrl(pt_text,h_widget);
    }
    else if (i4_ret != WGLR_OK)
    {
         return i4_ret;
    }
    i4_ret = x_wgl_get_parent(h_widget,&h_parent); 
    if (i4_ret != WGLR_OK)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get parent ret=%d,File =%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
        return i4_ret;
    } 
    x_wgl_get_coords(h_widget,WGL_COORDS_PARENT,&t_rect);
    i4_ret = x_wgl_repaint(h_parent,&t_rect,FALSE);
    if (i4_ret != WGLR_OK)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to repaint. i4_ret=%d,File =%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
        return i4_ret;
    } 
    return WGLR_OK;
}



static INT32 _wgl_text_end_scrl(
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    HANDLE_T                            h_widget)
{
    INT32                               i4_ret;   
    HANDLE_T                            h_parent; 
    GL_RECT_T                           t_rect;
        
    /*stop timer*/
    if (!pt_text->b_is_scrl_state)
    {
         return WGLR_OK;
    }
    i4_ret = x_wgl_stop_timer(pt_text->h_timer);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(TEXT)} ERR: Fail to delete timer. i4_ret=%d,File=%s,Line=%d\n\n", i4_ret, __FILE__, __LINE__));
        return i4_ret;
    }
    pt_text->b_is_scrl_state = FALSE; 
    pt_text->t_cnt_intf.pf_content_cmd_proc(pt_text->h_cnt_data, WGL_CNT_TEXT_END_SCRL, NULL, NULL);     

     
    i4_ret = x_wgl_get_parent(h_widget,&h_parent); 
    if (i4_ret != WGLR_OK)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get parent ret=%d,File =%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
        return i4_ret;
    } 
    x_wgl_get_coords(h_widget,WGL_COORDS_PARENT,&t_rect);
    i4_ret = x_wgl_repaint(h_parent,&t_rect,FALSE);
    if (i4_ret != WGLR_OK)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to repaint. i4_ret=%d,File =%s,Line=%d\n\n", i4_ret, __FILE__,__LINE__));
        return i4_ret;
    } 
  
    return WGLR_OK;  
}

/***************************************************************************************
App mybe needs to decide text direction at run time,so we let the widget to query text direction from app's callback
   and update widget's text direction according to callback functions's return value.
   
This api will be called in the functions whose implementation will use text direction.

Now in the implementation,there are these messages,commands,functions will call this api:
WGL_MSG_PAINT
WGL_MSG_TIMER
WGL_CMD_TEXT_GET_TOTAL_LINES
WGL_CMD_TEST_PAGE_SCROLL
 WGL_CMD_TEXT_BEGIN_SCRL
 WGL_CMD_TEXT_END_SCRL
 _wgl_text_process_key_event()

 ************************************************************************************/
static INT32 _wgl_text_query_and_update_text_dir(
    HANDLE_T                            h_widget,
    WGL_TEXT_CONTROL_LOGIC_T*           pt_text,
    VOID*                               pv_internal_cmd_data)
{
   INT32 i4_ret = WGLR_OK;
   INT32 i4_temp = WGLR_OK;
   UINT8 ui1_text_dir;
   SIZE_T z_size = sizeof(UINT8);
   
   /*because x_wgl_query_canvas_info_callback() maybe call app's callback function,so unlock first*/
   i4_temp = wgl_release_instance(h_widget, pt_text);
   WGL_ASSERT(WGLR_OK == i4_temp);
   i4_ret = x_wgl_query_canvas_info_callback(h_widget,WGL_CVS_CB_TXT_DIR,(VOID*)(&ui1_text_dir),&z_size);
   if(wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_text) != WGLR_OK)
    {
       DBG_ERROR(("{WGL(TEXT)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", i4_ret, __LINE__));
       return WGLR_INV_HANDLE;
    }
   if(i4_ret == WGLR_OK)
   {
       i4_ret = _wgl_text_msg_hdr_command(h_widget, 
                                         pt_text, 
                                         WGL_CMD_GL_SET_TEXT_DIRECTION, 
                                         WGL_PACK(ui1_text_dir),
                                         NULL,
                                         pv_internal_cmd_data);

   } 
   else if(i4_ret == WGLR_FUNC_NOT_IMPL)
   {
       /*no callback, do nothing*/
       i4_ret = WGLR_OK;
   }
   return i4_ret;

}
    

INT32 wgl_text_get_pref_height(
    UTF16_T*                            w2s_break_string,
    WGL_FONT_INFO_T*                    pt_font_info,
    UINT32                              ui4_style,
    UINT8                                ui1_text_dir,
    WGL_INSET_T*                        pt_cnt_inset,
    UINT16                              ui2_line_gap,    
    UINT16                              ui2_width,
    UINT16*                             pt_ui2_height)
{
    /*local varaible declaration*/
    UTF16_T                             a_w2s_break_chars[] = WGL_CAN_BREAK_CHARS;
    UTF16_T                             a_w2s_force_chars[] = WGL_FORCE_BREAK_CHARS;
    UTF16_T                             a_w2s_skip_chars[] = WGL_SKIP_CHARS;
    WGL_BREAK_LINE_INFO_T               t_break_line_Info = {0};
    WGL_BREAK_LINE_RESULT_T             t_break_line_result = {0};
    UINT16                              ui2_first_token_pos;
    INT32                               i4_ret;
    UINT16                              ui2_index = 0;    
    HFONT_T                             h_font;
    UINT32                              ui4_text_max_len;
    UINT32                              ui4_text_len;
    INT16                               i2_height;  
    UINT32                              ui4_text_count = 0;

    
    /*check parameter*/
    if ((NULL == pt_ui2_height) 
        ||(NULL == w2s_break_string)
        ||(NULL == pt_font_info))
    {
         DBG_ERROR(("{WGL(TEXT_CNT)} ERR: null param. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
         return WGLR_INV_ARG;
    } 
    if (ui2_width == 0)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: ui2_width is 0. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
        return WGLR_INV_ARG;
    }
    
    if (WGLR_OK != WGL_FONT_CREATE(pt_font_info, &h_font))
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: invalid font info. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
        return WGLR_INV_ARG;
    }
    /*if style is single line mode just return one line height plus content inset*/
    if (!(WGL_STL_TEXT_MULTILINE & ui4_style))
    {
        i4_ret = x_fe_get_height(h_font, &i2_height);
        if (i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get font height. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
            return i4_ret;
        }
        if (NULL == pt_cnt_inset)
        {
          *pt_ui2_height = (UINT16)i2_height;
        }
        else
        {
           *pt_ui2_height = (UINT16)(i2_height + pt_cnt_inset->i4_top + pt_cnt_inset->i4_bottom);
  
        }        
        return WGLR_OK;
    }
    /*multi line mode */
    ui4_text_max_len = (UINT32)(WGL_STL_TEXT_MAX_4096 & ui4_style ?                                        \
                    4096 : (WGL_STL_TEXT_MAX_2048 & ui4_style ?                          
                                2048 : (WGL_STL_TEXT_MAX_512& ui4_style ?              
                                            512 : (WGL_STL_TEXT_MAX_128 & ui4_style ?   
                                                        128 : 32))));       
    ui4_text_len =(UINT32) x_uc_w2s_strlen(w2s_break_string);
    if (ui4_text_len < ui4_text_max_len)
    {
        ui4_text_max_len = ui4_text_len;
    }
    
    if (pt_cnt_inset != NULL)
    {
        ui2_width -= pt_cnt_inset->i4_left + pt_cnt_inset->i4_right;
    }  
    
    t_break_line_Info.h_font = h_font;
    t_break_line_Info.w2c_force_token = (WGL_STL_TEXT_IGNORE_NEWLINE & ui4_style) ? (UTF16_T)'\0' : a_w2s_force_chars[0];
    t_break_line_Info.w2s_can_break = a_w2s_break_chars;    
    if (ui1_text_dir == WGL_TEXT_DIR_RIGHT_TO_LEFT)
    {           
       
       t_break_line_Info.w2s_string = w2s_break_string + ui4_text_max_len-1;
    } 
    else
    {            
       t_break_line_Info.w2s_string = w2s_break_string;  
    }
    t_break_line_Info.i4_string_length = (INT32)ui4_text_max_len;
    t_break_line_Info.i4_box_width = (INT32)ui2_width;
    t_break_line_result.w2s_next_start = t_break_line_Info.w2s_string;

    /*for soft-hyphen*/
    t_break_line_Info.w2s_skip = a_w2s_skip_chars;
    t_break_line_Info.ui2_skip_count =(UINT16)(sizeof(a_w2s_skip_chars) / sizeof(UTF16_T) - 1);
    t_break_line_Info.ui2_skip_width = 0;
    t_break_line_Info.b_skip_as_break = FALSE;    
    ui2_first_token_pos = 0; 
    /*get the necessary line info*/  
    while (t_break_line_result.w2s_next_start != NULL && t_break_line_result.w2s_next_start[0] != 0)   
    {        
        /*update parameter*/
        ui2_first_token_pos = (ui2_first_token_pos > t_break_line_result.ui2_char_number) ? 
                              (ui2_first_token_pos - t_break_line_result.ui2_char_number) : 
                              0;
        t_break_line_Info.w2s_string = t_break_line_result.w2s_next_start;
        t_break_line_Info.i4_string_length -= t_break_line_result.ui2_char_number;           

        /*Get text's necessary width*/        
        if (ui1_text_dir == WGL_TEXT_DIR_RIGHT_TO_LEFT)
        {
            i4_ret = wgl_get_break_line_pos_for_arab(
                        t_break_line_Info.w2s_string ,
                        &t_break_line_Info,
                        &ui2_first_token_pos,
                        &t_break_line_result);
        }
        else
        {   
            i4_ret = wgl_get_break_line_pos(
                        &t_break_line_Info,
                        &ui2_first_token_pos,
                        &t_break_line_result);
        }
        if (t_break_line_result.ui2_char_number == 0)
        {
              DBG_ERROR(("{WGL(TEXT_CNT)} ERR: ui2_width is too small. (%d) File=%s,Line=%d\n\n", WGLR_INV_ARG,__FILE__, __LINE__));       
              return WGLR_INV_ARG;
        }        
        if(i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get a position to break a line. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }          
        ui2_index++;   
        ui4_text_count += t_break_line_result.ui2_char_number;       
        if (ui4_text_count >= ui4_text_max_len)
        {
            break;
        }
        
    }
     
    i4_ret = x_fe_get_height(h_font, &i2_height); 
    if (i4_ret != WGLR_OK)
    {
        DBG_ERROR(("{WGL(TEXT_CNT)} ERR: Fail to get font height. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    *pt_ui2_height = (UINT16)((ui2_index * i2_height) + (ui2_index -1)*ui2_line_gap);
    if (NULL != pt_cnt_inset)   
    {
       *pt_ui2_height +=pt_cnt_inset->i4_top + pt_cnt_inset->i4_bottom;  
    }
    WGL_FONT_FREE(h_font);
    return WGLR_OK;
}




