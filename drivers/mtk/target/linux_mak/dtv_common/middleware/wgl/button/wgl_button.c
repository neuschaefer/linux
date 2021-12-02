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
 * $RCSfile: wgl_button.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 45f0ea61343b0bc887222a305df44a87 $
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
#include "wgl/button/x_wgl_button.h"
#include "wgl/button/wgl_button.h"
#include "wgl/button/wgl_button_cnt.h"
#include "wgl/button/wgl_button_cli.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_btn_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

typedef struct _WGL_BTN_CONTROL_LOGIC_T
{
 #ifdef CLI_LVL_ALL     
    UINT32                         ui4_mem_size;/*this field must be the first field in the structure*/
#endif  
    VOID*                           pv_attach;      /*attached data*/
    WGL_CONTENT_CTRL_INTF_T         t_cnt_intf;     /*content interface*/
    WGL_CONTENT_CONTEXT_HANDLE_T    h_cnt_data;     /*content object*/
    WGL_BORDER_CTRL_INTF_T          t_bdr_intf;     /*border interface*/
    WGL_BORDER_CONTEXT_HANDLE_T     h_bdr_data;     /*border object*/
    UINT32                          ui4_style;      /*widget style*/
    UINT16                          ui2_state;
} WGL_BTN_CONTROL_LOGIC_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*WGL_MSG_PAINT message handler*/
static INT32 _wgl_btn_msg_hdr_paint(
                HANDLE_T                        h_widget,
                WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
                GL_HGRAPHICS_T                  h_graphics,
                UINT32                          ui4_upt_rect,
                const GL_RECT_T*                pt_upt_rect,
                UINT16                          ui2_state);

/*WGL_MSG_KEY_DOWN & WGL_MSG_KEY_UP message handler*/
static INT32 _wgl_btn_msg_hdr_key(
                HANDLE_T                        h_widget,
                UINT32                          ui4_k_code,
                UINT32                          ui4_k_state);

/*WGL_MSG_SIZE message handler*/
static INT32 _wgl_btn_msg_hdr_size(
                HANDLE_T                        h_widget,
                WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
                UINT16                          ui2_width,
                UINT16                          ui2_height);

/*WGL_MSG_COMMAND message handler*/
static INT32 _wgl_btn_msg_hdr_command(
                HANDLE_T                        h_widget,
                WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
                UINT32                          ui4_command_id,
                VOID*                           pv_param1,
                VOID*                           pv_param2,
                VOID*                           pv_internal_cmd_data);

/*WGL_CMD_BTN_GET_MIN_SIZE command handler*/
static INT32 _wgl_btn_cmd_hdr_get_min_size(
                const WGL_BTN_CONTROL_LOGIC_T*  pt_btn,
                GL_SIZE_T*                      pt_size);

static INT32 _wgl_btn_change_cnt_rect(
                const WGL_BTN_CONTROL_LOGIC_T*  pt_btn,
                const GL_RECT_T*                pt_widget_rect,
                BOOL                            b_new_inset,
                const WGL_INSET_T*              pt_inset);

/*send push/unpush event to parent*/
static INT32 wgl_notify_parent_push(
                HANDLE_T                        h_widget,
                BOOL                            b_is_pushed);

static INT32 wgl_btn_msg_hdr_get_focus(
                HANDLE_T                     h_widget);
    
static INT32 wgl_btn_msg_hdr_lose_focus(
                HANDLE_T                        h_widget);

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
INT32 x_wgl_button_init(
    UINT32                      ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_BUTTON;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_btn_inst_create, 
                                               wgl_btn_inst_destroy, 
                                               wgl_btn_default_proc,
                                               WGL_ATTR_FOCUSABLE};
    UINT32                      ui4_cnt_type = WGL_CONTENT_BUTTON_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_btn_cnt_inst_create,
                                               wgl_btn_cnt_inst_destroy,
                                               wgl_btn_cnt_command_proc,
                                               wgl_btn_cnt_paint}; 
    INT32                       i4_ret;

    DBG_API(("{WGL(BTN)} %s: ui4_init_type = 0x%.8x\n", __FUNCTION__, ui4_init_type));

    if(WGL_BTN_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN)} ERR: Fail ot register Component Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    

    if(WGL_BTN_INIT_DEFAULT_CNT & ui4_init_type)
    {
        i4_ret = wgl_register_content_intf(ui4_cnt_type, &t_cnt_intf);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN)} ERR: Fail to register Content Interface. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }

    i4_ret = wgl_btn_cli_init();
    WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN)} ERR: Fail to init Button CLI. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
INT32 wgl_btn_default_proc(
    HANDLE_T                    h_widget,
    UINT32                      ui4_msg,
    VOID*                       pv_param1,
    VOID*                       pv_param2)
{
    /*local variable declaration*/
    GL_RECT_T                   t_update_rect;
    WGL_BTN_CONTROL_LOGIC_T*    pt_btn = NULL;
    GL_RECT_T                   t_widget_rect = {0};
    VOID*                       pv_internal_cmd_data = NULL;
    INT32                       i4_ret = WMPR_DONE;
    INT32                       i4_temp;
    UINT32                      ui4_update_rect;

    DBG_API(("{WGL(BTN)} %s: h_widget = 0x%.8x \tui4_msg = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
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
                DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's update region. (%d) at L%d\n\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        break;
        
    case WGL_MSG_GET_FOCUS:
        return wgl_btn_msg_hdr_get_focus(h_widget);
        
    case WGL_MSG_LOSE_FOCUS:
        return wgl_btn_msg_hdr_lose_focus(h_widget);

    case WGL_MSG_KEY_DOWN:
        return _wgl_btn_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_DOWN);

    case WGL_MSG_KEY_UP:
        return _wgl_btn_msg_hdr_key(h_widget, (UINT32)pv_param1, WGL_KEY_STATE_UP);

    case WGL_MSG_COMMAND:
        {
            if(WGL_CMD_GL_SET_INSET == (UINT32)pv_param1)   /*need to get widget's rectangle before lock it*/
            {
                i4_ret = x_wgl_get_coords(h_widget, WGL_COORDS_LOCAL, &t_widget_rect);
                if(WGLR_OK != i4_ret)
                {
                    DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's dimension. (%d) at L%d\n\n", i4_ret, __LINE__));
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
    i4_ret = wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_btn);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    if(NULL == pt_btn)
    {
        i4_temp = wgl_release_instance(h_widget, pt_btn);
        WGL_ASSERT(WGLR_OK == i4_temp);
        
        DBG_ERROR(("{WGL(BTN)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
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
            i4_temp = wgl_release_instance(h_widget, pt_btn);
            WGL_ASSERT(WGLR_OK == i4_temp);
            i4_ret = x_wgl_query_canvas_info_callback(h_widget,WGL_CVS_CB_TXT_DIR,(VOID*)(&ui1_text_dir),&z_size);
            if( wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_btn) != WGLR_OK)
            {
                DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
                return WGLR_INV_HANDLE;
            }
            if(i4_ret == WGLR_OK)
            {
                i4_ret = _wgl_btn_msg_hdr_command(h_widget, 
                                                  pt_btn, 
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
            i4_ret = _wgl_btn_msg_hdr_paint(h_widget, 
                                        pt_btn, 
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
        i4_ret = _wgl_btn_msg_hdr_size(h_widget, 
                                       pt_btn, 
                                       WGL_GET_HI_WORD((UINT32)pv_param1), 
                                       WGL_GET_LOW_WORD((UINT32)pv_param1));
        break;

    case WGL_MSG_ENABLE:
        {
            WGL_UPDATE_STATE(pt_btn->ui2_state, WGL_POINTER_TO_BOOL(pv_param1), WGL_STATE_ENABLED);
            i4_ret = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_SET_STATE, WGL_PACK(pt_btn->ui2_state), NULL);
        }
        break;
        
    case WGL_MSG_NOTIFY:
        {
            WGL_ASSERT(0);  /*btn static should not receive this message*/
            i4_ret = WGLR_INTERNAL_ERROR;
            DBG_ERROR(("{WGL(BTN)} ERR: Button should not receive any notification. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
        break;
        

    case WGL_MSG_COMMAND:
        {
            if(NULL == pv_param2)
            {
                i4_ret = WGLR_INV_ARG;
                DBG_ERROR(("{WGL(BTN)} ERR: The 2nd parameter of WGL_MSG_COMMAND should not be NULL. (%d) at L%d\n\n", i4_ret, __LINE__));
            }
            else
            {
                i4_ret = _wgl_btn_msg_hdr_command(h_widget, 
                                                  pt_btn, 
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
            i4_ret = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_UPDATE_CVSST, pv_param1, NULL);
        }
        break;

    default:
        i4_ret = WGLR_INV_MSG;
        DBG_ERROR(("{WGL(BTN)} ERR: Unknown message type. (%d) at L%d\n\n", i4_ret, __LINE__));
        break;
    }

    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_btn);
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
INT32 wgl_btn_inst_create( 
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
    WGL_BTN_CONTROL_LOGIC_T*        pt_ctrl_logic;
    WGL_INSET_T                     t_bdr_inset;
    WGL_CANVAS_STATUS_T             t_cvsst;
    INT32                           i4_ret;
    
    DBG_API(("{WGL(BTN)} %s: pt_ctx_wgt = 0x%.8x \tpt_wgt_create = 0x%.8x\n", __FUNCTION__, pt_ctx_wgt, pt_wgt_create));

    /*check parameter*/
    if(NULL == pt_ctx_wgt || NULL == pt_wgt_create)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(BTN)} pt_ctx_wgt = 0x%.8x   ui4_type_content = 0x%.8x   ui4_type_border = 0x%.8x\n"
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
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get content's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
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
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get border's interface. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_TYPE;
    }

    /*allocate memory for control logic object*/
    pt_ctrl_logic = (WGL_BTN_CONTROL_LOGIC_T*) WGL_MEM_ALLOC(sizeof(WGL_BTN_CONTROL_LOGIC_T));
    if(NULL == pt_ctrl_logic)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to alloc memory for control logic. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;      
    }

    /*initialize control logic object*/
    x_memset(pt_ctrl_logic, 0, sizeof(WGL_BTN_CONTROL_LOGIC_T));

#ifdef CLI_LVL_ALL
    pt_ctrl_logic->ui4_mem_size = sizeof(WGL_BTN_CONTROL_LOGIC_T);   
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
            DBG_ERROR(("{WGL(BTN)} ERR: Fail to create border object. (%d) at L%d\n\n", i4_ret, __LINE__));
            goto _err_out;
        }

        /*get default border inset*/
        i4_ret = t_bdr_intf.pf_border_cmd_proc(h_bdr_data, WGL_CMD_GL_GET_INSET, &t_bdr_inset, NULL);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN)} ERR: Fail to get border's inset. (%d) at L%d\n\n", i4_ret, __LINE__));
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
        DBG_ERROR(("{WGL(BTN)} ERR: NULL-Border cannot be with WGL_STL_GL_BDR_FILL_CNT_BK style. (%d) at L%d\n\n", i4_ret, __LINE__));
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
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to create content object. (%d) at L%d\n\n", i4_ret, __LINE__));
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
INT32 wgl_btn_inst_destroy( 
    WGL_WIDGET_CONTEXT_HANDLE_T     t_ctx_wgt)
{
    /*local variable parameter*/
    WGL_BTN_CONTROL_LOGIC_T*        pt_ctrl_logic;
    INT32                           i4_ret;

    DBG_API(("{WGL(BTN)} %s: t_ctx_wgt = 0x%.8x\n", __FUNCTION__, t_ctx_wgt));

    /*check parameter*/
    if(NULL == t_ctx_wgt)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_ctrl_logic = (WGL_BTN_CONTROL_LOGIC_T*)t_ctx_wgt;

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
static INT32 _wgl_btn_msg_hdr_paint(
    HANDLE_T                        h_widget,
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
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
        DBG_ERROR(("{WGL(BTN)} ERR: GRAPHICS object is NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check if pushed*/
    if(IS_STATE_PUSHED(pt_btn->ui2_state))
    {
        WGL_SET_FLAG(ui2_state, WGL_STATE_PUSHED);
    }
    
    if(pt_btn->t_bdr_intf.pf_paint_border)
    {
        /*check border*/
        WGL_CHECK_INTERNAL(pt_btn->h_bdr_data);

        /*paint border*/
        i4_ret = pt_btn->t_bdr_intf.pf_paint_border(pt_btn->h_bdr_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN)} ERR: Fail to paint border. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }

    /*paint content*/
    WGL_CHECK_INTERNAL(pt_btn->t_cnt_intf.pf_paint_content);
    WGL_CHECK_INTERNAL(pt_btn->h_cnt_data);

    return pt_btn->t_cnt_intf.pf_paint_content(pt_btn->h_cnt_data, h_graphics, ui4_upt_rect, pt_upt_rect, ui2_state);
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
static INT32 _wgl_btn_msg_hdr_key(
    HANDLE_T                        h_widget,
    UINT32                          ui4_k_code,
    UINT32                          ui4_k_state)
{
    /*local varaible declaration*/
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn = NULL;
    BOOL                            b_is_pushed = FALSE;
    INT32                           i4_ret, i4_temp;
    UINT16                          ui2_old_state;

    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_btn))
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_btn)
    {
        i4_temp = wgl_release_instance(h_widget, pt_btn);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(BTN)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*check state*/
    if(!IS_STATE_ENABLED(pt_btn->ui2_state))
    {
        /*unlock instance*/
        i4_temp = wgl_release_instance(h_widget, pt_btn);
        WGL_ASSERT(WGLR_OK == i4_temp);
        return WGLR_KEY_MSG_NOT_PROCESSED;
    }

    /*check if the key is 'Enter' key*/
    ui2_old_state = pt_btn->ui2_state;
    if(BTN_SELECT == ui4_k_code)
    {
        if(WGL_STL_BTN_CHECKBOX & pt_btn->ui4_style) /*simulate checkbox behavior*/
        {
            if(WGL_KEY_STATE_DOWN == ui4_k_state)
            {
                /*update state*/
                if(WGL_IS_FLAG_TURN_ON(pt_btn->ui2_state, WGL_STATE_PUSHED))
                {
                    WGL_CLEAR_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                    b_is_pushed = FALSE;
                }
                else
                {
                    WGL_SET_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                    b_is_pushed = TRUE;
                }
            }
        }
        else
        {
            if(WGL_KEY_STATE_DOWN == ui4_k_state)
            {
                /*update state*/
                WGL_SET_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                b_is_pushed = TRUE;
            }
            else if(WGL_KEY_STATE_UP == ui4_k_state)
            {
                WGL_CLEAR_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                b_is_pushed = FALSE;
            }
            else /*repeat*/
            {
                WGL_ASSERT(0);
            }
        }
    }
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_btn);
    WGL_ASSERT(WGLR_OK == i4_temp);

    /*notify parent*/
    if(BTN_SELECT == ui4_k_code)
    {
        if(ui2_old_state != pt_btn->ui2_state)
        {
           /*raise a repaint*/
           i4_ret = x_wgl_repaint(h_widget, NULL, TRUE);
            WGL_ASSERT(WGLR_OK == i4_ret);        
            return wgl_notify_parent_push(h_widget, b_is_pushed);
        }

        return WGLR_OK;
    }
    else
    {
        i4_ret = wgl_notify_parent_key(h_widget, ui4_k_code, ui4_k_state);
        WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN)} ERR: Fail to notify widget's parent of key state. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
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
static INT32 _wgl_btn_msg_hdr_size(
    HANDLE_T                        h_widget,
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
    UINT16                          ui2_width,
    UINT16                          ui2_height)
{
    /*local variable declaration*/
    GL_RECT_T                       t_widget_rect = {0};
    INT32                           i4_ret;
    
    /*initialize content rect*/
    t_widget_rect.i4_right = (INT32)ui2_width;
    t_widget_rect.i4_bottom = (INT32)ui2_height;

    if(pt_btn->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_ASSERT(pt_btn->h_bdr_data);
        
        /*notify border to update its rectangle*/
        i4_ret = pt_btn->t_bdr_intf.pf_border_cmd_proc(pt_btn->h_bdr_data, WGL_BDR_GL_SET_SIZE, (VOID*)(INT32)ui2_width, (VOID*)(INT32)ui2_height);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    return _wgl_btn_change_cnt_rect(pt_btn, &t_widget_rect, FALSE, NULL);
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
static INT32 _wgl_btn_msg_hdr_command(
    HANDLE_T                        h_widget,
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn,
    UINT32                          ui4_command_id,
    VOID*                           pv_param1,
    VOID*                           pv_param2,
    VOID*                           pv_internal_cmd_data)
{
    DBG_API(("{WGL(BTN)} %s: h_widget = 0x%.8x \tui4_command_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
            __FUNCTION__, 
            h_widget, 
            ui4_command_id,
            pv_param1,
            pv_param2));

    /*check command*/
    if(! (WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_BUTTON, ui4_command_id) 
            || WGL_IS_CMD_GENERAL(ui4_command_id)
            || WGL_IS_CONTENT_CMD(ui4_command_id) 
            || WGL_IS_BORDER_CMD(ui4_command_id)))
    {
        
        DBG_ERROR(("{WGL(BTN)} ERR: Unknown Command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    /*get command handler*/
    switch (ui4_command_id)
    {
    case WGL_CMD_GL_SET_ATTACH_DATA:
        {
            pt_btn->pv_attach = pv_param1;
            return WGLR_OK;
        }

    case WGL_CMD_GL_GET_ATTACH_DATA:
        {
            if(NULL == (VOID**)pv_param1)
            {
                DBG_ERROR(("{WGL(BTN)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ATTACH_DATA, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            else
            {
                *((VOID**)pv_param1) = pt_btn->pv_attach;
                return WGLR_OK;
            }
        }

    case WGL_CMD_BTN_SET_PUSHED:
        {
            if(TRUE == WGL_POINTER_TO_BOOL(pv_param1))
            {
                WGL_SET_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                return WGLR_OK;
            }
            else if(FALSE == WGL_POINTER_TO_BOOL(pv_param1))
            {
                WGL_CLEAR_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
                return WGLR_OK;
            }
            else
            {
                DBG_ERROR(("{WGL(BTN)} ERR: Invalid arguments. For WGL_CMD_BTN_SET_PUSHED, its 1st input argument should be TRUE or FALSE. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
        }

    case WGL_CMD_BTN_GET_PUSHED:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(BTN)} ERR: Invalid arguments. For WGL_CMD_BTN_GET_PUSHED, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((BOOL*)pv_param1) = (IS_STATE_PUSHED(pt_btn->ui2_state)) ? TRUE : FALSE;
        }
        break;

    case WGL_CMD_BTN_GET_MIN_SIZE:
        return _wgl_btn_cmd_hdr_get_min_size(pt_btn, (GL_SIZE_T*)pv_param1);

    case WGL_CMD_GL_SET_INSET:
        {
            DBG_INFO(("{WGL(BTN)} set-inset(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n", 
                        pv_param1,
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_left),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_top),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_right),
                        ((NULL == pv_param1) ? 0 : ((WGL_INSET_T*)pv_param1)->i4_bottom)));

            /*[DTV00000540] For frame, icon, text, editbox, progressbar, and button widgets, the content background show incorrectly if the inset size  5  with uniform border*/
            /*Comments: widget component has to update CONTENT rectangle when BORDER rectangle changes.*/
            return _wgl_btn_change_cnt_rect(pt_btn, (GL_RECT_T*)pv_internal_cmd_data, TRUE, (WGL_INSET_T*)pv_param1);
        }
    
    case WGL_CMD_GL_SET_TEXT_DIRECTION:
        {
            INT32 i4_ret;
            WGL_CHECK_INTERNAL(pt_btn->t_cnt_intf.pf_content_cmd_proc);
            WGL_CHECK_INTERNAL(pt_btn->h_cnt_data);
          
            i4_ret = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, ui4_command_id, pv_param1, pv_param2);
            if(i4_ret == WGLR_OK)
            {
                if(NULL != pt_btn->t_bdr_intf.pf_border_cmd_proc)
                {
                    /*check border*/
                    WGL_ASSERT(pt_btn->h_bdr_data);
                    return pt_btn->t_bdr_intf.pf_border_cmd_proc(pt_btn->h_bdr_data, WGL_CMD_GL_SET_BDR_TEXT_DIRECTION, pv_param1, pv_param2);
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
            UINT32 ui4_wdg_mem = pt_btn->ui4_mem_size;
            WGL_MTX_OBJ_T*  pt_wgt_mem;
            if(NULL == pv_param1)
            {
                    DBG_ERROR(("{WGL(BUTTON)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                    return WGLR_INV_ARG; 
            }
            
            pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_btn->h_cnt_data;
            ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;
            if(pt_btn->t_bdr_intf.pf_create_instance != NULL)
            {
                pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_btn->h_bdr_data;
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
                WGL_CHECK_INTERNAL(pt_btn->t_cnt_intf.pf_content_cmd_proc);
                WGL_CHECK_INTERNAL(pt_btn->h_cnt_data);
                
                return pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, ui4_command_id, pv_param1, pv_param2);
            }
            else if(WGL_IS_BORDER_CMD(ui4_command_id))/*border command*/
            {
                if(NULL == pt_btn->t_bdr_intf.pf_border_cmd_proc)
                {
                    DBG_ERROR(("{WGL(BTN)} ERR: Border's command procedure is NULL. (%d) at L%d\n\n", WGLR_NO_BORDER, __LINE__));
                    return WGLR_NO_BORDER;
                }

                /*check border*/
                WGL_ASSERT(pt_btn->h_bdr_data);
                return pt_btn->t_bdr_intf.pf_border_cmd_proc(pt_btn->h_bdr_data, ui4_command_id, pv_param1, pv_param2);
            }
            else
            {
                DBG_ERROR(("{WGL(BTN)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
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
 *          WGL_CMD_BTN_GET_MIN_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_cmd_hdr_get_min_size(
    const WGL_BTN_CONTROL_LOGIC_T*  pt_btn,
    GL_SIZE_T*                      pt_size)
{
    /*local variable declaration*/
    GL_SIZE_T                       t_bdr_min_size = {0};
    INT32                           i4_ret;

    /*check content*/
    WGL_CHECK_INTERNAL(pt_btn->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_btn->h_cnt_data);

    if(pt_btn->t_bdr_intf.pf_border_cmd_proc)
    {
        /*check border*/
        WGL_CHECK_INTERNAL(pt_btn->h_bdr_data);
     

        /*get border min size*/
        i4_ret = WGLR_FUNC_NOT_IMPL;
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(BTN)} ERR: This command isn't exported in this version. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    /*Get Content Size*/
    i4_ret = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_GET_MIN_SIZE, pt_size, NULL);
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
 *           WGL_MSG_LOSE_FOCUS message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_btn_change_cnt_rect(
    const WGL_BTN_CONTROL_LOGIC_T*         pt_btn,
    const GL_RECT_T*                       pt_widget_rect,
    BOOL                                   b_new_inset,
    const WGL_INSET_T*                     pt_inset)
{
    GL_RECT_T                           t_cnt_rect;
    INT32                               i4_ret;

    /*check data*/
    WGL_CHECK_INTERNAL(pt_btn->t_cnt_intf.pf_content_cmd_proc);
    WGL_CHECK_INTERNAL(pt_btn->h_cnt_data);

    /*get cnt rect*/
    i4_ret = wgl_calc_cnt_rect(pt_btn->t_bdr_intf.pf_border_cmd_proc, pt_btn->h_bdr_data, pt_widget_rect, b_new_inset, pt_inset, &t_cnt_rect);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to calc content rectangle. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*update content rect*/
    return pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_SET_RECT, &t_cnt_rect, NULL);
}

/*******************************************************************************
 * Name
 *      
 * Description
 *           WGL_MSG_GET_FOCUS message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 wgl_btn_msg_hdr_get_focus(
    HANDLE_T                     h_widget)
{
    /*local varaible declaration*/
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn = NULL;
    INT32                           i4_temp, i4_ret;

    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_btn))
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_btn)
    {
        i4_temp = wgl_release_instance(h_widget, pt_btn);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(BTN)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*set focus state*/
    WGL_UPDATE_STATE(pt_btn->ui2_state, TRUE, WGL_STATE_FOCUSED);
    i4_temp = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_SET_STATE, WGL_PACK(pt_btn->ui2_state), NULL);
    WGL_ASSERT(WGLR_OK == i4_temp);
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_btn);
    WGL_ASSERT(WGLR_OK == i4_temp);

    i4_ret = wgl_notify_parent_focus(h_widget, TRUE);
    WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN)} ERR: Fail to notify widget's parent of getting focus. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *           WGL_MSG_LOSE_FOCUS message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 wgl_btn_msg_hdr_lose_focus(
    HANDLE_T                     h_widget)
{
    /*local varaible declaration*/
    WGL_BTN_CONTROL_LOGIC_T*        pt_btn = NULL;
    INT32                           i4_temp, i4_ret;

    /*Get btn data and lock instance*/
    if(WGLR_OK != wgl_get_instance(h_widget, (WGL_WIDGET_CONTEXT_HANDLE_T*)&pt_btn))
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's instance. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_btn)
    {
        i4_temp = wgl_release_instance(h_widget, pt_btn);
        WGL_ASSERT(WGLR_OK == i4_temp);

        DBG_ERROR(("{WGL(BTN)} ERR: Widget's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }

    /*reset pushed state*/
    if(!(WGL_STL_BTN_CHECKBOX & pt_btn->ui4_style)) /*simulate checkbox behavior*/
    {
        WGL_CLEAR_FLAG(pt_btn->ui2_state, WGL_STATE_PUSHED);
    }
    /*reset focus state*/
    WGL_UPDATE_STATE(pt_btn->ui2_state, FALSE, WGL_STATE_FOCUSED);
    i4_temp = pt_btn->t_cnt_intf.pf_content_cmd_proc(pt_btn->h_cnt_data, WGL_CNT_BTN_SET_STATE, WGL_PACK(pt_btn->ui2_state), NULL);
    WGL_ASSERT(WGLR_OK == i4_temp);
    
    /*unlock instance*/
    i4_temp = wgl_release_instance(h_widget, pt_btn);
    WGL_ASSERT(WGLR_OK == i4_temp);

    i4_ret = wgl_notify_parent_focus(h_widget, FALSE);  
    WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN)} ERR: Fail to notify widget's parent of losing focus. (%d) at L%d\n\n", i4_ret, __LINE__)));
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
static INT32 wgl_notify_parent_push(
    HANDLE_T                        h_widget,
    BOOL                            b_is_pushed)
{
    /*local varaible declaration*/
    WGL_NOTIFY_DATA_T               t_nfy_data = {0};
    HANDLE_T                        h_parent = NULL_HANDLE;
    INT32                           i4_ret;

    /*get parent*/
    i4_ret = x_wgl_get_parent(h_widget, &h_parent);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(BTN)} ERR: Fail to get widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    if(h_parent)
    {
        /*prepare notify data*/
        t_nfy_data.ui4_nc_code = (b_is_pushed) ? WGL_NC_BTN_PUSHED : WGL_NC_BTN_UNPUSHED;

        /*send notify to its parent*/
        i4_ret = x_wgl_send_notify(h_parent, t_nfy_data.ui4_nc_code, h_widget, &t_nfy_data);
        WGL_BTN_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(BTN)} ERR: Fail to send notficiation to widget's parent. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
        
    }

    return WGLR_OK;
}
