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
 * $RCSfile: wgl_tktp.c,v $
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
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "unicode/x_uc_str.h"

#include "dbg/u_dbg.h"
#include "wgl/x_wgl.h"
#include "wgl/x_wgl_common.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/u_wgl_common.h"

#include "wgl/u_wgl_bdr_common.h"
#include "wgl/tktp/x_wgl_tktp.h"
#include "wgl/tktp/wgl_tktp.h"
#include "wgl/tktp/wgl_tktp_eng.h"
#include "wgl/tktp/wgl_tktp_cnt_internal.h"
#include "wgl/tktp/wgl_tktp_cnt_def.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "TKTP"

/* defualt values */
#define DEF_SCRL_DELAY              10
#define DEF_SCRL_STEP_SIZE          1
#define DEF_MSG_TEXT_INIT_LEN       1000
#define DEF_MSG_TEXT_INC_SIZE       2000

/* Ticker Tape widget engine thread definitions */
#define TKTP_ENG_THREAD_NAME_PREFIX                    "tktp_"
#define TKTP_ENG_THREAD_DEFAULT_PRIORITY        ((UINT8)   128)
#define TKTP_ENG_THREAD_DEFAULT_STACK_SIZE      ((SIZE_T) 4096)

/* macros */
#define DO_TKTP_CNT_CMD(_pt_ctx, _cmd_id, _param1, _param2, _ret)           \
        (_ret) = (_pt_ctx)->t_intf_cnt.pf_content_cmd_proc((_pt_ctx)->pv_ctx_cnt, (_cmd_id), (VOID *)(UINT32)(_param1), (VOID *)(UINT32)(_param2));  

#define DO_BDR_CMD(_pt_ctx, _cmd_id, _param1, _param2, _ret)                \
        (_ret) = (_pt_ctx)->t_intf_bdr.pf_border_cmd_proc((_pt_ctx)->pv_ctx_bdr, (_cmd_id), (VOID *)(UINT32)(_param1), (VOID *)(UINT32)(_param2));    

#define IS_NULL_BORDER(_pt_ctx)                                             \
    ((_pt_ctx)->t_intf_bdr.pf_create_instance==NULL)

#define TKTP_STM_IS_VISIBLE(_pt_ctx)                                        \
            HAS_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE) 

#define TKTP_STM_IS_HIDDEN(_pt_ctx)                                         \
            (   !HAS_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE) )

#define TKTP_STM_IS_ENABLED(_pt_ctx)                                        \
            HAS_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE) 

#define TKTP_STM_IS_DISABLED(_pt_ctx)                                       \
            (   !HAS_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE) )

#define TKTP_STM_IS_STOPPED(_pt_ctx)                                        \
            (    HAS_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE)               \
              && !(    HAS_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE)          \
                    && HAS_FLAG((_pt_ctx)->ui2_state, TTST_NEED_SCRL))) 

#define TKTP_STM_IS_SCRLING(_pt_ctx)                                        \
            (    HAS_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE)               \
              && HAS_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE)                \
              && HAS_FLAG((_pt_ctx)->ui2_state, TTST_NEED_SCRL)) 

#define TKTP_STM_SHOW(_pt_ctx)                                              \
            WGL_SET_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE)

#define TKTP_STM_HIDE(_pt_ctx)                                              \
            WGL_CLEAR_FLAG((_pt_ctx)->ui2_state, TTST_VISIBLE)

#define TKTP_STM_ENABLE(_pt_ctx)                                            \
            WGL_SET_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE)

#define TKTP_STM_DISABLE(_pt_ctx)                                           \
            WGL_CLEAR_FLAG((_pt_ctx)->ui2_state, TTST_ENABLE)

#define TKTP_STM_SET_NEED_SCRL(_pt_ctx)                                     \
            WGL_SET_FLAG((_pt_ctx)->ui2_state, TTST_NEED_SCRL)

#define TKTP_STM_CLEAR_NEED_SCRL(_pt_ctx)                                   \
            WGL_CLEAR_FLAG((_pt_ctx)->ui2_state, TTST_NEED_SCRL)

#define TKTP_STM_IS_NEED_SCRL(_pt_ctx)                                      \
            HAS_FLAG((_pt_ctx)->ui2_state, TTST_NEED_SCRL)


#ifdef DEBUG

#define GET_LOCK(_h_wgt, _ppt_ctx, _ret_var)                                \
    {                                                                       \
        (_ret_var) = wgl_get_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if((_ret_var)==WGLR_INV_HANDLE)                                     \
        {   /*the widget has been destroyed*/                               \
            return (_ret_var);                                              \
        }                                                                   \
        else if(WGL_IS_FAIL(_ret_var))                                      \
        {                                                                   \
            DBG_ERROR(("[%s] ERR: GET_LOCK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, (_ret_var), __FILE__, __LINE__));\
            return (_ret_var);                                              \
        }                                                                   \
    }

#define RELEASE_LOCK(_h_wgt, _pt_ctx, _ret_var)                             \
    {                                                                       \
        (_ret_var) = wgl_release_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        if(WGL_IS_FAIL(_ret_var))                                           \
        {                                                                   \
            DBG_ERROR(("[%s] ERR: RELEASE_LOCK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, (_ret_var), __FILE__, __LINE__));\
            return (_ret_var);                                              \
        }                                                                   \
        (_pt_ctx) = NULL;                                                   \
    }

#define CATCH_FAIL_UNLOCK(_h_wgt, _pt_ctx, _x)                               \
    {                                                                       \
        if((i4_ret=(_x))<0)                                                 \
        {                                                                   \
            INT32   i4_ret_x = i4_ret;                                      \
            DBG_ERROR(("[WGL(%s)] ERR: CATCH_FAIL_UNLOCK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, i4_ret_x, __FILE__, __LINE__));\
            if(_pt_ctx)                                                     \
            {                                                               \
                CATCH_FAIL(wgl_release_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx) )); \
            }                                                               \
            return i4_ret_x;                                                \
        }                                                                   \
    }
    
#define CATCH_FAIL_TMPULK(_expr)                                            \
        {                                                                   \
            if(WGL_IS_FAIL(i4_ret=(_expr)))                                 \
            {                                                               \
                if(i4_ret!=WGLR_INV_HANDLE)                                 \
                {                                                           \
                    DBG_ERROR(("[WGL(%s)] ERR: CATCH_FAIL_TMPULK()=%d File:%s Line:%d\n", DBG_MODULE_NAME, i4_ret, __FILE__, __LINE__));\
                }                                                           \
                return i4_ret;                                              \
            }                                                               \
        }
    



#else


#define GET_LOCK(_h_wgt, _ppt_ctx, _ret_var)                                \
    {                                                                       \
        (_ret_var) = wgl_get_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T*)(_ppt_ctx));\
        if(WGL_IS_FAIL(_ret_var))                                           \
        {                                                                   \
            return (_ret_var);                                              \
        }                                                                   \
    }

#define RELEASE_LOCK(_h_wgt, _pt_ctx, _ret_var)                             \
    {                                                                       \
        (_ret_var) = wgl_release_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx));\
        if(WGL_IS_FAIL(_ret_var))                                           \
        {                                                                   \
            return (_ret_var);                                              \
        }                                                                   \
        (_pt_ctx) = NULL;                                                   \
    }

#define CATCH_FAIL_UNLOCK(_h_wgt, _pt_ctx, _x)                               \
    {                                                                       \
        if((i4_ret=(_x))<0)                                                 \
        {                                                                   \
            INT32   i4_ret_x = i4_ret;                                      \
            if(_pt_ctx)                                                     \
            {                                                               \
                CATCH_FAIL(wgl_release_instance((_h_wgt), (WGL_WIDGET_CONTEXT_HANDLE_T)(_pt_ctx) )); \
            }                                                               \
            return i4_ret_x;                                                \
        }                                                                   \
    }
    
#define CATCH_FAIL_TMPULK(_expr)                                            \
        CATCH_FAIL(_expr)
    


#endif


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_msg_proc(
    IN      HANDLE_T                    h_tktp,
    IN      UINT32                      ui4_msg,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2);

static INT32 _tktp_cmd_proc(
    IN      HANDLE_T                    h_tktp,
    IN      UINT32                      ui4_cmd_id,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2);

static INT32 _tktp_cmd_hdr_get_style(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_get_state(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_show(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_cmd_hdr_hide(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktp_cmd_hdr_destroy(
    IN      HANDLE_T                    h_tktp, 
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktp_cmd_hdr_set_lcsz_ctrl(
    IN      HANDLE_T                    h_tktp, 
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_get_lcsz_ctrl(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_text(
    IN      TKTP_CTX_T                  *pt_ctx, 
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktp_cmd_hdr_get_text(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _tktp_cmd_hdr_clear_text(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_cmd_hdr_enable_scrl(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_resume_scrl(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_cmd_hdr_pause_scrl(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_cmd_hdr_set_scrl_step_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_get_scrl_step_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_scrl_delay(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_get_scrl_delay(
    IN      TKTP_CTX_T                  *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_reg_nfy(
    IN      TKTP_CTX_T                  *pt_ctx, 
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_font(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_color(
    IN OUT  TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
    
static INT32 _tktp_cmd_hdr_get_font(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_inset(
    IN OUT  HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_cnt_inset(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1);

static INT32 _tktp_cmd_hdr_set_align(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktp_msg_hdr_show( 
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_msg_hdr_hide( 
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_msg_hdr_paint(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint);

static INT32 _tktp_msg_hdr_size(
    IN OUT  HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1);

static INT32 _tktp_msg_hdr_enable(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _tktp_msg_hdr_create(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_msg_hdr_destroy(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_msg_hdr_move(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_engine_init(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      BOOL                        b_run);

static INT32 _tktp_engine_destroy(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_engine_go_sleep(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_engine_wake_up(
    IN      TKTP_CTX_T                  *pt_ctx);

static BOOL _tktp_engine_is_inited(
    IN      TKTP_CTX_T                  *pt_ctx);

static BOOL _tktp_engine_is_sleeping(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_tool_reg_action_after_scrl_once(
    IN      TKTP_CTX_T                      *pt_ctx,
    IN      TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action,
    IN      VOID                            *pv_tag_aso_nfy);

static INT32 _tktp_tool_hide(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_tool_start_scrl(
    IN      TKTP_CTX_T                  *pt_ctx);

static INT32 _tktp_tool_update_lcsz(
    IN      HANDLE_T                    h_tktp);

static INT32 _tktp_tool_update_cnt_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      INT32                       i4_wgt_w,
    IN      INT32                       i4_wgt_h);

static INT32 _tktp_tool_update_cnt_inset(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      WGL_INSET_T                 *pt_inset, 
    IN      BOOL                        b_check_scrl);

static INT32 _tktp_engine_exec_aso_action(
    IN      HANDLE_T                        h_tktp,
    IN      TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action,
    IN      wgl_tktp_nfy_fct                pf_tktp_nfy,
    IN      VOID                            *pv_tag_nfy,
    IN OUT  TKTP_ENG_INFO_T                 *pt_info);

/*-----------------------------------------------------------------------------
                    exported functions
 ----------------------------------------------------------------------------*/
INT32 x_wgl_tktp_init(UINT32 ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_TKTP;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_tktp_inst_create, 
                                               wgl_tktp_inst_destroy, 
                                               wgl_tktp_default_proc,
                                               0};
    UINT32                      ui4_cnt_type = WGL_CONTENT_TKTP_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_tktp_cnt_def_inst_create,
                                               wgl_tktp_cnt_def_inst_destroy,
                                               wgl_tktp_cnt_def_cmd_proc,
                                               wgl_tktp_cnt_def_paint}; 
    INT32                       i4_ret;

    
    if(WGL_TKTP_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    

    if(WGL_TKTP_INIT_DEFAULT_CNT & ui4_init_type)
    {
        i4_ret = wgl_register_content_intf(ui4_cnt_type, &t_cnt_intf);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }

    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_tktp_inst_create( 
    WGL_WIDGET_CONTEXT_HANDLE_T         *ppv_ctx_wgt,
    WGL_WIDGET_CREATE_INPUT_T           *pt_wgt_create )
{
    TKTP_CTX_T                      *pt_ctx = NULL;
    WGL_TKTP_INIT_T                 *pt_init = (WGL_TKTP_INIT_T *)pt_wgt_create->pv_param;
    INT32                           i4_ret;
    WGL_INSET_T                     t_inset_bdr;
    WGL_CONTENT_CREATE_INPUT_T      t_create_cnt;
    TKTP_CNT_INIT_T                 t_init_cnt;
    WGL_CANVAS_STATUS_T             t_cvsst;

    *ppv_ctx_wgt = NULL;
    
    CHECK_VALID(pt_init != NULL, WGLR_INV_ARG);
    CHECK_VALID(IS_VALID_STYLE(pt_init->ui4_style), WGLR_INV_ARG);
    
    /* check validity of pt_init->t_lcsz_ctrl */
    if(HAS_FLAG(pt_init->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL))
    { 
        CHECK_VALID(
               RECT_IS_EFFECTIVE(pt_init->t_lcsz_ctrl.t_rc_ref)
            && WGL_TKTP_IS_VALID_ANCHOR_MODE(pt_init->t_lcsz_ctrl.e_anchor), WGLR_INV_ARG);
    }
    else
    {
        CHECK_VALID(RECT_IS_EFFECTIVE(*pt_wgt_create->pt_rect), WGLR_INV_ARG);
    }

    pt_ctx = (TKTP_CTX_T *)WGL_MEM_ALLOC(sizeof(TKTP_CTX_T));
    if (pt_ctx == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL
    pt_ctx->ui4_mem_size = sizeof(TKTP_CTX_T);   
#endif

    /*initialize canvas status*/
    wgl_cvsst_update(&t_cvsst, &pt_wgt_create->t_cvs_info, pt_wgt_create->h_palette);

    /* General Attributes */
    pt_ctx->ui4_style = pt_init->ui4_style;
    pt_ctx->ui2_state = TTST_VISIBLE | TTST_ENABLE;
    pt_ctx->ui4_delay = DEF_SCRL_DELAY;
    pt_ctx->h_tktp = NULL_HANDLE;
    pt_ctx->pf_tktp_nfy = NULL;
    
    i4_ret = x_wgl_get_system_font(&pt_ctx->t_finfo_msg);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }
    
    WGL_FONT_CREATE (&pt_ctx->t_finfo_msg, &pt_ctx->h_font_msg);
    
    if(pt_ctx->h_font_msg == NULL_HANDLE)
    {
        INFORM_ERR(WGLR_INV_ARG);
        WGL_MEM_FREE(pt_ctx);
        return WGLR_INV_ARG;
    }
    
    if (pt_ctx->t_finfo_msg.i2_width != 0)
    {
        INFORM_ERR (x_fe_set_fixed_advance (pt_ctx->h_font_msg, pt_ctx->t_finfo_msg.i2_width, 0));
    }

    if(HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL))
    {
        x_memcpy(&pt_ctx->t_lcsz, &pt_init->t_lcsz_ctrl, sizeof(WGL_TKTP_LCSZ_CTRL_T));
    }
    else
    {
        x_memset(&pt_ctx->t_lcsz, 0, sizeof(WGL_TKTP_LCSZ_CTRL_T));
    }
    pt_ctx->pv_attach = NULL;
    
    /* Scrolling Contexts */
    pt_ctx->b_scrling_first = TRUE;
    pt_ctx->e_aso_action    = TKTP_ASO_ACTION_NONE;
    pt_ctx->pv_tag_aso_nfy  = NULL;
    
    /* Engine Contexts */
    pt_ctx->e_state_eng = ENGST_NOT_INIT;
    pt_ctx->e_eareq_eng = TKTP_EAREQ_NONE;
    pt_ctx->h_th_eng = NULL_HANDLE;
    pt_ctx->h_sema_eng = NULL_HANDLE;

    /* Message Data */
    i4_ret = wgl_str_init(
        &pt_ctx->t_str_msg,
        DEF_MSG_TEXT_INIT_LEN,
        DEF_MSG_TEXT_INC_SIZE,
        pt_ctx->h_font_msg);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        WGL_FONT_FREE(pt_ctx->h_font_msg);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }   

    /* Control Interface and Context of Border */
    /* query Border interface */
    pt_ctx->t_intf_bdr.pf_border_cmd_proc = NULL;
    pt_ctx->t_intf_bdr.pf_create_instance = NULL;
    pt_ctx->t_intf_bdr.pf_destroy_instance = NULL;
    pt_ctx->t_intf_bdr.pf_paint_border = NULL;
    i4_ret = wgl_query_border_intf(
        pt_wgt_create->ui4_type_border,
        &pt_ctx->t_intf_bdr);
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        WGL_FONT_FREE(pt_ctx->h_font_msg);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }

    /* create border */
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        WGL_BORDER_CREATE_INPUT_T       t_create_bdr;
        t_create_bdr.i4_width = RECT_W(pt_wgt_create->pt_rect);
        t_create_bdr.i4_height = RECT_H(pt_wgt_create->pt_rect);
        t_create_bdr.ui4_style = pt_init->ui4_style;
        t_create_bdr.pt_cvsst = &t_cvsst;
        i4_ret = pt_ctx->t_intf_bdr.pf_create_instance(&pt_ctx->pv_ctx_bdr, &t_create_bdr);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            WGL_FONT_FREE(pt_ctx->h_font_msg);
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }
    }

    /* query insets and is_bdr_draw_interior of border */
    if( !IS_NULL_BORDER(pt_ctx) )
    {
    ___BLOCK_BEGIN___
        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret);
        GOTO_EOB_AT_FAIL(i4_ret, i4_ret);
    ___BLOCK_END___
        
        if( WGL_IS_FAIL(i4_ret) )
        {
            INFORM_ERR(i4_ret);
            pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
            WGL_FONT_FREE(pt_ctx->h_font_msg);
            WGL_MEM_FREE(pt_ctx);
            return i4_ret;
        }
    }
    else
    {
        _INSET_EMPTY(&t_inset_bdr);
    }
    
    /* init Content */
    /* query the control interface of the content */
    i4_ret = wgl_query_content_intf( pt_wgt_create->ui4_type_content, &pt_ctx->t_intf_cnt );
    if(WGL_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        if( !IS_NULL_BORDER(pt_ctx) )
        {
            pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
        }
        WGL_FONT_FREE(pt_ctx->h_font_msg);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;        
    }
    
    
    /* init TKTP_CNT_INIT_T */
    t_init_cnt.ui4_style = pt_init->ui4_style;
    t_init_cnt.pt_str_msg = &pt_ctx->t_str_msg;
    t_init_cnt.ui2_scrl_step_size = DEF_SCRL_STEP_SIZE;
    t_init_cnt.b_force_scrl_short_text = HAS_FLAG(pt_init->ui4_style, WGL_STL_TKTP_NO_SCRL_SHORT_TEXT) ? FALSE : TRUE;

    /* init WGL_CONTENT_CREATE_INPUT_T */
    SET_RECT_BY_SIZE(&t_create_cnt.t_rect_cnt, 0, 0, RECT_W(pt_wgt_create->pt_rect), RECT_H(pt_wgt_create->pt_rect));
    _RECT_INSET(&t_create_cnt.t_rect_cnt, &t_inset_bdr);
    t_create_cnt.pt_cvsst = &t_cvsst;
    t_create_cnt.pv_init = (VOID *)&t_init_cnt;

    /* create the instance of content */
    i4_ret = pt_ctx->t_intf_cnt.pf_create_instance(&pt_ctx->pv_ctx_cnt, &t_create_cnt);
    if( WGL_IS_FAIL(i4_ret) )
    {
        INFORM_ERR(i4_ret);
        if( !IS_NULL_BORDER(pt_ctx) )
        {
            pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr);
        }
        WGL_FONT_FREE(pt_ctx->h_font_msg);
        WGL_MEM_FREE(pt_ctx);
        return i4_ret;
    }


    *ppv_ctx_wgt = (WGL_WIDGET_CONTEXT_HANDLE_T)pt_ctx;
    return WGLR_OK;
}

INT32 wgl_tktp_inst_destroy(
    WGL_WIDGET_CONTEXT_HANDLE_T         pv_ctx_wgt )
{
    INT32       i4_ret;
    TKTP_CTX_T  *pt_ctx = (TKTP_CTX_T *)pv_ctx_wgt;

    if(_tktp_engine_is_inited(pt_ctx))
    {
        INFORM_ERR(_tktp_engine_destroy(pt_ctx));
    }

    if(pt_ctx->h_th_eng != NULL_HANDLE)
    {
        /* do nothing, since the thread shall terminate itself */
    }

    INFORM_ERR(wgl_str_destroy(&pt_ctx->t_str_msg));

    if(pt_ctx->h_font_msg != NULL_HANDLE)
    {
        WGL_FONT_FREE(pt_ctx->h_font_msg);
        pt_ctx->h_font_msg = NULL_HANDLE;
    }

    /* destroy border */
    if(!IS_NULL_BORDER(pt_ctx))
    {
        INFORM_ERR(pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr));
    }

    /* destroy content */
    INFORM_ERR(pt_ctx->t_intf_cnt.pf_destroy_instance(pt_ctx->pv_ctx_cnt));

    /* destroy myself */
    x_memset(pt_ctx, 0, sizeof(TKTP_CTX_T));
    WGL_MEM_FREE(pt_ctx);

    return WGLR_OK;
}

INT32 wgl_tktp_default_proc( 
    HANDLE_T                            h_tktp,
    UINT32                              ui4_msg,
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{

    switch(ui4_msg) 
    {
        case WGL_MSG_COMMAND:
        {
            WGL_PACKED_PARAM_T *pt_params = (WGL_PACKED_PARAM_T *)pv_param2;
            return _tktp_cmd_proc(h_tktp, (UINT32)pv_param1, pt_params->pv_param1, pt_params->pv_param2);
        }
        case WGL_MSG_KEY_DOWN:
        case WGL_MSG_KEY_UP:
             return WGLR_KEY_MSG_NOT_PROCESSED;

        case WGL_MSG_GET_FOCUS:
        case WGL_MSG_LOSE_FOCUS:
        case WGL_MSG_KEY_REPEAT:
        case WGL_MSG_ACTIVATE:
        case WGL_MSG_NOTIFY:
        {
            /* Do nothing */
            return WGLR_OK;
        }
    }

    return _tktp_msg_proc(h_tktp, ui4_msg, pv_param1, pv_param2);

}

INT32 wgl_tktp_engine_tick_proc( 
    HANDLE_T                            h_tktp,
    TKTP_ENG_INFO_T                     *pt_info)
{
    TKTP_CTX_T                          *pt_ctx = NULL;
    INT32                               i4_ret;
    TKTP_CNT_DNS_PARAM_T                t_dns_param;
    TKTP_CNT_DNS_RET_T                  t_dns_ret;
    wgl_tktp_nfy_fct                    pf_tktp_nfy;
    UINT32                              ui4_ms_delay, ui4_tk_start, ui4_ms_elapse;

    pt_info->e_msg = TKTP_ENG_MSG_NEXT_TICK;
    pt_info->ui4_next_delay = 0;
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    pt_info->ui4_next_delay = pt_ctx->ui4_delay;

    ui4_tk_start = x_os_get_sys_tick();
    ui4_ms_delay = pt_ctx->ui4_delay;

    /* copy pf_tktp_nfy */
    pf_tktp_nfy = pt_ctx->pf_tktp_nfy;
    
    /* check for engine action request first */
    switch(pt_ctx->e_eareq_eng) 
    {
        case TKTP_EAREQ_NONE:
        {
            /* do nothing */
        }
    	break;
        case TKTP_EAREQ_DESTROY:
        {
            pt_info->e_msg = TKTP_ENG_MSG_DESTROY;
            pt_info->ui4_next_delay = 0;
            RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        }
        return WGLR_OK;
        default:
        {
            RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
            INFORM_ERR(WGLR_INTERNAL_ERROR);
        }
        return WGLR_INTERNAL_ERROR;
    }

    /* check if need to make itself sleep */
    if( ! TKTP_STM_IS_SCRLING(pt_ctx) ) 
    {  
        TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action;
        VOID                            *pv_tag_aso_nfy;
        
        CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, _tktp_engine_go_sleep(pt_ctx));
        pt_info->h_sema_eng     = pt_ctx->h_sema_eng;
        e_aso_action            = pt_ctx->e_aso_action;
        pv_tag_aso_nfy          = pt_ctx->pv_tag_aso_nfy;
        pt_ctx->e_aso_action    = TKTP_ASO_ACTION_NONE;
        pt_ctx->pv_tag_aso_nfy  = NULL;
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        pt_info->e_msg          = TKTP_ENG_MSG_SLEEP;
        pt_info->ui4_next_delay = 0;
        
        return _tktp_engine_exec_aso_action(h_tktp, e_aso_action, pf_tktp_nfy, pv_tag_aso_nfy, pt_info);
    }

    /*do the next scroll*/
    t_dns_param.pv_tag = NULL;
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_DO_NEXT_SCRL, &t_dns_param, &t_dns_ret, i4_ret );
    if(WGL_IS_FAIL(i4_ret))
    {
        INT32                       i4_ret_x = i4_ret;
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        
        INFORM_ERR(i4_ret_x);
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret );
        INFORM_ERR(i4_ret);               
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        return i4_ret_x;
    }

    /*issue a repaint*/
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    CATCH_FAIL_TMPULK(x_wgl_repaint(h_tktp, NULL, TRUE));

    /*check if scroll finished*/
    if(t_dns_ret.b_end_reached)
    {
        /* notify: WGL_TKTP_NFY_COND_SCRL_END_REACHED */
        if(pf_tktp_nfy)
        {
            pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_SCRL_END_REACHED, NULL);
        }

        GET_LOCK(h_tktp, &pt_ctx, i4_ret);

        if(HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE))
        {   /*stop scrolling: state: SCRLING->STOPPED*/
            TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
        }
        if(pt_ctx->b_scrling_first)
        {
            TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action;
            VOID                            *pv_tag_aso_nfy;
            
            /*after scrolled once*/
            pt_ctx->b_scrling_first = FALSE;
            e_aso_action            = pt_ctx->e_aso_action;
            pv_tag_aso_nfy          = pt_ctx->pv_tag_aso_nfy;
            pt_ctx->e_aso_action    = TKTP_ASO_ACTION_NONE;
            pt_ctx->pv_tag_aso_nfy  = NULL;
            RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
            ui4_ms_elapse = (x_os_get_sys_tick() - ui4_tk_start) * x_os_get_sys_tick_period();
            pt_info->ui4_next_delay = ui4_ms_delay > ui4_ms_elapse ? ui4_ms_delay - ui4_ms_elapse : 0;
            return _tktp_engine_exec_aso_action(h_tktp, e_aso_action, pf_tktp_nfy, pv_tag_aso_nfy, pt_info);
        }
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    }

    ui4_ms_elapse = (x_os_get_sys_tick() - ui4_tk_start) * x_os_get_sys_tick_period();
    pt_info->ui4_next_delay = ui4_ms_delay > ui4_ms_elapse ? ui4_ms_delay - ui4_ms_elapse : 0;
    
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/

static INT32 _tktp_msg_proc(
    IN      HANDLE_T                    h_tktp,
    IN      UINT32                      ui4_msg,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2)
{
    TKTP_CTX_T          *pt_ctx = NULL;
    INT32               i4_ret, i4_ret_val;

    switch(ui4_msg) 
    {
        case WGL_MSG_CREATE:
        {
            return _tktp_msg_hdr_create(h_tktp);
        }

        case WGL_MSG_DESTROY:
        {
            return _tktp_msg_hdr_destroy(h_tktp);
        }
        
        case WGL_MSG_SIZE:
        {
            return _tktp_msg_hdr_size(h_tktp, pv_param1);
        }
        
        case WGL_MSG_MOVE:
        {
            return _tktp_msg_hdr_move(h_tktp);
        }

    }
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);

    i4_ret_val = WGLR_OK;
    switch(ui4_msg)    
    {
        /* Internal Messages */
        case WGL_MSG_PAINT:
        {
            i4_ret_val = _tktp_msg_hdr_paint(pt_ctx, (GL_HGRAPHICS_T)pv_param1, WGL_POINTER_TO_UINT16(pv_param2));
        }
    	break;
        
        case WGL_MSG_ENABLE:
        {
            i4_ret_val = _tktp_msg_hdr_enable(pt_ctx, pv_param1);
        }
    	break;
        
        default:
        {
            INFORM_ERR(WGLR_INV_MSG);
            i4_ret_val = WGLR_INV_MSG;
        }
    }

    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    return i4_ret_val;
}


static INT32 _tktp_cmd_proc(
    IN      HANDLE_T                    h_tktp,
    IN      UINT32                      ui4_cmd_id,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2)
{
    TKTP_CTX_T                          *pt_ctx = NULL;
    INT32                               i4_ret, i4_ret_val;
    BOOL                                b_need_repaint;

    i4_ret_val     = WGLR_OK;
    b_need_repaint = FALSE;
    
    /*deliver content or border command */
    if(WGL_IS_CONTENT_CMD(ui4_cmd_id))
    {
        switch(ui4_cmd_id)
        {
            case WGL_CMD_TKTP_SET_CNT_INSET:
            return _tktp_cmd_hdr_set_cnt_inset(h_tktp, pv_param1);
        }
        
        GET_LOCK(h_tktp, &pt_ctx, i4_ret);
        
        switch(ui4_cmd_id)
        {
            case WGL_CMD_TKTP_SET_ALIGN:
                i4_ret_val = _tktp_cmd_hdr_set_align(pt_ctx, pv_param1);
                b_need_repaint = TRUE;
            break;
            
            default:
                DO_TKTP_CNT_CMD(pt_ctx, ui4_cmd_id, pv_param1, pv_param2, i4_ret_val);
        }
        
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        
        if(!WGL_IS_FAIL(i4_ret_val) && b_need_repaint)
        {
            return x_wgl_repaint(h_tktp, NULL, TRUE);
        }
        
        return i4_ret_val;
    }
    else if(WGL_IS_BORDER_CMD(ui4_cmd_id))
    {
        switch(ui4_cmd_id) 
        {
            case WGL_CMD_GL_SET_INSET:
            return _tktp_cmd_hdr_set_inset(h_tktp, pv_param1);
        }
        
        GET_LOCK(h_tktp, &pt_ctx, i4_ret);
        if(!IS_NULL_BORDER(pt_ctx))
        {
            DO_BDR_CMD(pt_ctx, ui4_cmd_id, pv_param1, pv_param2, i4_ret_val);
        }
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        return i4_ret_val;
    }
    
    /*check the validity of this widget command */
    if(! (WGL_IS_WIDGET_CMD(ui4_cmd_id) && 
            ( WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_TKTP, ui4_cmd_id) 
               || WGL_IS_CMD_GENERAL(ui4_cmd_id)
            ) 
         )
      )
    {
        return WGLR_INV_CMD;
    }    

    /*intercept commands that may not need to get the instance lock*/
    switch(ui4_cmd_id) 
    {
        case WGL_CMD_TKTP_SET_LCSZ_CTRL:
        {
            CATCH_FAIL(_tktp_cmd_hdr_set_lcsz_ctrl(h_tktp, pv_param1));
            return x_wgl_repaint(h_tktp, NULL, TRUE);
        }

        case WGL_CMD_GL_SET_FONT:
        {
            CATCH_FAIL(_tktp_cmd_hdr_set_font(h_tktp, pv_param1));
            return x_wgl_repaint(h_tktp, NULL, TRUE);
        }

        case WGL_CMD_TKTP_SHOW:
        {
            return _tktp_cmd_hdr_show(h_tktp);
        }

        case WGL_CMD_TKTP_HIDE:
        {
            return _tktp_cmd_hdr_hide(h_tktp, pv_param1, pv_param2);
        }

        case WGL_CMD_TKTP_ENABLE_SCRL:
        {
            return _tktp_cmd_hdr_enable_scrl(h_tktp, pv_param1);
        }

        case WGL_CMD_TKTP_DESTROY:
        {
            return _tktp_cmd_hdr_destroy(h_tktp, pv_param1, pv_param2);
        }
#ifdef CLI_LVL_ALL
        case WGL_CMD_GL_GET_ALLOC_MEM:
        {
            UINT32 ui4_wdg_mem = pt_ctx->ui4_mem_size;
            WGL_MTX_OBJ_T*  pt_wgt_mem;
            if(NULL == pv_param1)
            {
            	DBG_ERROR(("{WGL(TKTP)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                i4_ret_val = WGLR_INV_CMD;
            }
            pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_ctx->pv_ctx_cnt;
            ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;
            if(pt_ctx->t_intf_bdr.pf_create_instance != NULL)
            {
                pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_ctx->pv_ctx_bdr;
                ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;   
            }
            *((UINT32*)pv_param1) = ui4_wdg_mem;
            return WGLR_OK;
        }       
#endif        
    }
    

    GET_LOCK(h_tktp, &pt_ctx, i4_ret);

    switch(ui4_cmd_id) 
    {
        /* Widget Specific Commands */
        case WGL_CMD_TKTP_GET_STYLE:
        {
            i4_ret_val = _tktp_cmd_hdr_get_style(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_GET_STATE:
        {
            i4_ret_val = _tktp_cmd_hdr_get_state(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_GET_LCSZ_CTRL:
        {
            i4_ret_val = _tktp_cmd_hdr_get_lcsz_ctrl(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_SET_TEXT:
        {
            i4_ret_val = _tktp_cmd_hdr_set_text(pt_ctx, pv_param1, pv_param2);
            b_need_repaint = TRUE;
        }
    	break;

        case WGL_CMD_TKTP_GET_TEXT:
        {
            i4_ret_val = _tktp_cmd_hdr_get_text(pt_ctx, pv_param1, pv_param2);
        }
    	break;

        case WGL_CMD_TKTP_CLEAR_TEXT:
        {
            i4_ret_val = _tktp_cmd_hdr_clear_text(pt_ctx);
            b_need_repaint = TRUE;
        }
    	break;

        case WGL_CMD_TKTP_RESTART_SCRL:
        {
            i4_ret_val = _tktp_cmd_hdr_set_text(
                pt_ctx, 
                WGL_PACK(WGL_STR_GET_TEXT(&pt_ctx->t_str_msg)),
                WGL_PACK(WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_msg)));
            b_need_repaint = TRUE;
        }
    	break;
        
        case WGL_CMD_TKTP_RESUME_SCRL:
        {
            i4_ret_val = _tktp_cmd_hdr_resume_scrl(pt_ctx);
        }
    	break;

        case WGL_CMD_TKTP_PAUSE_SCRL:
        {
            i4_ret_val = _tktp_cmd_hdr_pause_scrl(pt_ctx);
        }
    	break;

        case WGL_CMD_TKTP_SET_SCRL_STEP_SIZE:
        {
            i4_ret_val = _tktp_cmd_hdr_set_scrl_step_size(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_GET_SCRL_STEP_SIZE:
        {
            i4_ret_val = _tktp_cmd_hdr_get_scrl_step_size(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_SET_SCRL_DELAY:
        {
            i4_ret_val = _tktp_cmd_hdr_set_scrl_delay(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_GET_SCRL_DELAY:
        {
            i4_ret_val = _tktp_cmd_hdr_get_scrl_delay(pt_ctx, pv_param1);
        }
    	break;

        case WGL_CMD_TKTP_REG_NFY:
        {
            i4_ret_val = _tktp_cmd_hdr_reg_nfy(pt_ctx, pv_param1);
        }
    	break;


        /* General Commands */
        case WGL_CMD_GL_SET_ATTACH_DATA:
        {
            pt_ctx->pv_attach = pv_param1;
        }    
        break;

        case WGL_CMD_GL_GET_ATTACH_DATA:
        {
            *((VOID **)pv_param1) = pt_ctx->pv_attach;
        }
        break;

        case WGL_CMD_GL_SET_COLOR:
        {
            i4_ret_val = _tktp_cmd_hdr_set_color(pt_ctx, pv_param1, pv_param2);
            b_need_repaint = TRUE;
        }
        break;

        case WGL_CMD_GL_GET_COLOR:
        {
            DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_GET_COLOR, pv_param1, pv_param2, i4_ret_val);
        }
        break;

        case WGL_CMD_GL_GET_FONT:
        {
            i4_ret_val = _tktp_cmd_hdr_get_font(pt_ctx, pv_param1);
        }
        break;

        case WGL_CMD_GL_SET_IMAGE:
        case WGL_CMD_GL_GET_IMAGE:
        {
            /* DO NOTHING */
        }
        break;
        default:
        {
            i4_ret_val = WGLR_INV_CMD;
        }

    }

    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    if(!WGL_IS_FAIL(i4_ret_val) && b_need_repaint)
    {
        return x_wgl_repaint(h_tktp, NULL, TRUE);
    }

    return i4_ret_val;
}


/*-----------------------------------------------------------------------------
                    command handler functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_cmd_hdr_get_style(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1)
{
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    *((UINT32 *)pv_param1) = pt_ctx->ui4_style;

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_get_state(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1)
{
    WGL_TKTP_STATE_T *pe_state = (WGL_TKTP_STATE_T *)pv_param1;

    CHECK_NULL(pe_state, WGLR_INV_ARG);
    
    if(TKTP_STM_IS_HIDDEN(pt_ctx))
    {
        *pe_state = WGL_TKTP_STATE_HIDDEN;
    }
    else if(TKTP_STM_IS_DISABLED(pt_ctx))
    {
        *pe_state = WGL_TKTP_STATE_SCRL_DISABLED;
    }
    else if(TKTP_STM_IS_SCRLING(pt_ctx))
    {
        *pe_state = pt_ctx->b_scrling_first ? WGL_TKTP_STATE_SCRLING_FIRST_CYCLE : WGL_TKTP_STATE_SCRLING_MORE_CYCLE;
    }
    else
    {
        *pe_state = WGL_TKTP_STATE_SCRL_STOPPED;
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_show(
    IN      HANDLE_T                    h_tktp)
{
    TKTP_CTX_T      *pt_ctx = NULL;
    INT32           i4_ret;
    BOOL            b_visible;

    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    b_visible = TKTP_STM_IS_VISIBLE(pt_ctx) ? TRUE : FALSE;
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    if(!b_visible)
    {
        CATCH_FAIL(x_wgl_show(h_tktp, WGL_SW_NORMAL));
        /*[NOTE]simulate a dummy message: WGL_MSG_SHOW*/
        GET_LOCK(h_tktp, &pt_ctx, i4_ret);
        CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, _tktp_msg_hdr_show(pt_ctx));
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    }

    return WGLR_OK;
}
                                
static INT32 _tktp_cmd_hdr_hide(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    TKTP_CTX_T                          *pt_ctx;
    INT32                               i4_ret;
    BOOL                                b_need_hide, b_notify_hidden;
    wgl_tktp_nfy_fct                    pf_tktp_nfy = NULL;
    VOID                                *pv_tag_nfy = pv_param2;
    WGL_TKTP_HIDE_MODE_T                e_hide_mode = (WGL_TKTP_HIDE_MODE_T)((UINT32)pv_param1);
    
    CHECK_VALID(WGL_TKTP_IS_VALID_HIDE_MODE(e_hide_mode), WGLR_INV_ARG);

    b_need_hide = FALSE;
    b_notify_hidden = FALSE;
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);

    if(TKTP_STM_IS_VISIBLE(pt_ctx))
    { 
        switch(e_hide_mode) 
        {
            case WGL_TKTP_HIDE_IMMEDIATELY:
            {
                /* notify: WGL_TKTP_NFY_COND_HIDDEN */
                b_notify_hidden = TRUE;
                b_need_hide = TRUE;
                
                /* check if need to override the previously sent hiding command with WGL_TKTP_HIDE_AFTER_SCRL_ONCE mode */
                if(pt_ctx->e_aso_action==TKTP_ASO_ACTION_HIDE)
                {
                    pt_ctx->e_aso_action = TKTP_ASO_ACTION_NONE;
                }
            }
    	    break;
            case WGL_TKTP_HIDE_AFTER_SCRL_ONCE:
            {
                if(TKTP_STM_IS_SCRLING(pt_ctx) && pt_ctx->b_scrling_first)
                { 
                    CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, _tktp_tool_reg_action_after_scrl_once(pt_ctx, TKTP_ASO_ACTION_HIDE, pv_tag_nfy));
                }
                else                
                {  /*state: has-been-scrolled-once*/
                    /* notify: WGL_TKTP_NFY_COND_HIDDEN */
                    b_notify_hidden = TRUE;
                    b_need_hide = TRUE;
                }
            }
    	    break;
        }
    }
    
    /* copy pf_tktp_nfy */
    if(b_notify_hidden)
    {
        pf_tktp_nfy = pt_ctx->pf_tktp_nfy;
    }

    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    if(b_need_hide)
    {
        CATCH_FAIL(_tktp_tool_hide(h_tktp));
    }

    if(b_notify_hidden && pf_tktp_nfy)
    {
        /* notify: WGL_TKTP_NFY_COND_HIDDEN */
        pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_HIDDEN, pv_tag_nfy);
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_destroy(
    IN      HANDLE_T                    h_tktp, 
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    WGL_TKTP_DESTROY_MODE_T e_des_mode = (WGL_TKTP_DESTROY_MODE_T)((UINT32)pv_param1);
    INT32                   i4_ret;
    VOID                    *pv_tag_nfy = pv_param2;
    
    if(!WGL_TKTP_IS_VALID_DESTROY_MODE(e_des_mode))
    {
        return WGLR_INV_ARG;
    }

    switch(e_des_mode) 
    {
        case WGL_TKTP_DESTROY_IMMEDIATELY:
        {
            TKTP_CTX_T              *pt_ctx = NULL;
            wgl_tktp_nfy_fct        pf_tktp_nfy;

            /* notify: WGL_TKTP_NFY_COND_DESTROYED */
            GET_LOCK(h_tktp, &pt_ctx, i4_ret);
            
            /* copy pf_tktp_nfy */
            pf_tktp_nfy = pt_ctx->pf_tktp_nfy;
            
            /* overrides other previous sent commands */
            if(pt_ctx->e_aso_action != TKTP_ASO_ACTION_NONE)
            {
                pt_ctx->e_aso_action = TKTP_ASO_ACTION_NONE;
            }
            
            RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
            if(pf_tktp_nfy)
            {
                pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_DESTROYED, pv_tag_nfy);
            }

            return x_wgl_destroy_widget(h_tktp);
        }
        
        case WGL_TKTP_DESTROY_AFTER_SCRL_ONCE:
        {
            TKTP_CTX_T  *pt_ctx = NULL;
    
            GET_LOCK(h_tktp, &pt_ctx, i4_ret);
            if( !(TKTP_STM_IS_SCRLING(pt_ctx) && pt_ctx->b_scrling_first) )
            { /* state: HIDDEN or STOPPED or has_been_scrolled_once */
                wgl_tktp_nfy_fct  pf_tktp_nfy = pt_ctx->pf_tktp_nfy;
                
                RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
                /* notify: WGL_TKTP_NFY_COND_DESTROYED */
                if(pf_tktp_nfy)
                {
                    pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_DESTROYED, pv_tag_nfy);
                }
                return x_wgl_destroy_widget(h_tktp);
            }

            CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, _tktp_tool_reg_action_after_scrl_once(pt_ctx, TKTP_ASO_ACTION_DESTROY, pv_tag_nfy));
            RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
        }
    	break;
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_lcsz_ctrl(
    IN      HANDLE_T                    h_tktp, 
    IN      VOID                        *pv_param1)
{
    TKTP_CTX_T              *pt_ctx = NULL;
    INT32                   i4_ret;
    WGL_TKTP_LCSZ_CTRL_T    *pt_lcsz = (WGL_TKTP_LCSZ_CTRL_T *)pv_param1;

    CHECK_NULL(pt_lcsz, WGLR_INV_ARG );
    CHECK_VALID(    RECT_IS_EFFECTIVE(pt_lcsz->t_rc_ref)
                 && WGL_TKTP_IS_VALID_ANCHOR_MODE(pt_lcsz->e_anchor), WGLR_INV_ARG);
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    if (!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL))
    {
        RELEASE_LOCK(h_tktp, pt_ctx, i4_ret); 
        return WGLR_INV_CMD;
    }
    pt_ctx->b_scrling_first = TRUE;
    x_memcpy(&pt_ctx->t_lcsz, pt_lcsz, sizeof(WGL_TKTP_LCSZ_CTRL_T));
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);    

    return _tktp_tool_update_lcsz(h_tktp);
}


static INT32 _tktp_cmd_hdr_get_lcsz_ctrl(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1)
{
    WGL_TKTP_LCSZ_CTRL_T  *pt_lcsz_out = (WGL_TKTP_LCSZ_CTRL_T *)pv_param1;
    
    CHECK_VALID( HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL), WGLR_INV_CMD );
    CHECK_NULL( pt_lcsz_out, WGLR_INV_ARG );

    x_memcpy(pt_lcsz_out, &pt_ctx->t_lcsz, sizeof(WGL_TKTP_LCSZ_CTRL_T));

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_text(
    IN      TKTP_CTX_T                  *pt_ctx, 
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UTF16_T     *w2s_text = (UTF16_T *)pv_param1;
    UINT16      ui2_len_text = WGL_POINTER_TO_UINT16(pv_param2);
    BOOL        b_need_scrl;
    INT32       i4_ret;

    CHECK_NULL(w2s_text, WGLR_INV_ARG);
    
    pt_ctx->b_scrling_first = TRUE;

    CATCH_FAIL(wgl_str_set_text(&pt_ctx->t_str_msg, w2s_text, ui2_len_text));

    /*inform the content that the message text has been updated*/
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_TEXT, &b_need_scrl, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    if(b_need_scrl)
    {   
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        /* must reset scrolling context after UPDATE commands if need scroll */
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        CATCH_FAIL(i4_ret);
        
        if(!TKTP_STM_IS_SCRLING(pt_ctx))
        {
            TKTP_STM_SET_NEED_SCRL(pt_ctx);
            if(TKTP_STM_IS_SCRLING(pt_ctx))
            {
                /* state: STOPPED -> SCRLING */
                CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
            }
        }
    }
    else 
    {   
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_get_text(
    IN      TKTP_CTX_T                  *pt_ctx, 
    OUT     VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UTF16_T  *w2s_buf_out = (UTF16_T *)pv_param1;
    UINT16   ui2_size_buf = WGL_POINTER_TO_UINT16(pv_param2);

    CHECK_NULL(w2s_buf_out, WGLR_INV_ARG);
    CHECK_VALID(ui2_size_buf >= (UINT16)1+WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_msg), WGLR_BUFFER_NOT_ENOUGH);
    
    x_uc_w2s_strncpy(w2s_buf_out, WGL_STR_GET_TEXT(&pt_ctx->t_str_msg), (UINT16)1+WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_msg));    

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_clear_text(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    INT32       i4_ret;
    BOOL        b_need_scrl;

    CATCH_FAIL(wgl_str_clear_text(&pt_ctx->t_str_msg));
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_TEXT, &b_need_scrl, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    if(b_need_scrl)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);  
        return WGLR_INTERNAL_ERROR;
    }
    else 
    {   
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_enable_scrl(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1)
{
    INT32   i4_ret; 
    BOOL    b_enable = WGL_POINTER_TO_BOOL(pv_param1);
    
    CHECK_VALID(b_enable==TRUE || b_enable==FALSE, WGLR_INV_ARG);   
    CATCH_FAIL(x_wgl_enable(h_tktp, b_enable));

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_resume_scrl(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    BOOL    b_need_scrl;
    INT32   i4_ret;
    
    if(TKTP_STM_IS_SCRLING(pt_ctx))
    {
        return WGLR_OK;
    }

    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_GET_IF_NEED_SCRL, &b_need_scrl, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    if(b_need_scrl)
    {
        TKTP_STM_SET_NEED_SCRL(pt_ctx);
        if(TKTP_STM_IS_SCRLING(pt_ctx))
        {
            CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
        }
    }
    else
    {
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_pause_scrl(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_scrl_step_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret;
    UINT16      ui2_scrl_step_size = WGL_POINTER_TO_UINT16(pv_param1);

    CHECK_VALID(ui2_scrl_step_size >= WGL_TKTP_MIN_SCRL_STEP_SIZE, WGLR_INV_ARG);
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_SET_SCRL_STEP_SIZE, ui2_scrl_step_size, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_get_scrl_step_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16  *pui2_scrl_step_size_out = (UINT16 *)pv_param1;
    INT32   i4_ret;
    
    CHECK_NULL(pui2_scrl_step_size_out, WGLR_INV_ARG);

    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_GET_SCRL_STEP_SIZE, pui2_scrl_step_size_out, NULL, i4_ret);
    CATCH_FAIL(i4_ret);
    
    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_scrl_delay(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT32  ui4_delay = (UINT32)pv_param1;

    CHECK_VALID(ui4_delay >= WGL_TKTP_MIN_SCRL_DELAY, WGLR_INV_ARG);

    pt_ctx->ui4_delay = ui4_delay;
    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_get_scrl_delay(
    IN      TKTP_CTX_T                  *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16  *pui2_scrl_delay_out = (UINT16 *)pv_param1;

    CHECK_NULL(pui2_scrl_delay_out, WGLR_INV_ARG);
    
    *pui2_scrl_delay_out = (UINT16)pt_ctx->ui4_delay;
    
    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_reg_nfy(
    IN      TKTP_CTX_T                  *pt_ctx, 
    IN      VOID                        *pv_param1)
{
    wgl_tktp_nfy_fct pf_tktp_nfy = (wgl_tktp_nfy_fct)pv_param1;

    pt_ctx->pf_tktp_nfy = pf_tktp_nfy;

    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_font(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1)
{
    TKTP_CTX_T          *pt_ctx = NULL;
    WGL_FONT_INFO_T     *pt_finfo = (WGL_FONT_INFO_T *)pv_param1;
    WGL_FONT_INFO_T     t_finfo_temp;
    BOOL                b_need_scrl, b_auto_resize;
    INT32               i4_ret, i4_ret_val;

    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    pt_ctx->b_scrling_first = TRUE;

    b_auto_resize = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL) ? TRUE : FALSE;

    i4_ret_val = WGLR_OK;
    
___BLOCK_BEGIN___    
    
    if(pt_finfo==NULL)
    {
        GOTO_EOB_AT_FAIL(x_wgl_get_system_font(&t_finfo_temp), i4_ret_val);
        pt_finfo = &t_finfo_temp;
    }
    
    GOTO_EOB_AT_FAIL(wgl_update_font_handle(
        &pt_ctx->t_finfo_msg,
        &pt_ctx->h_font_msg,
        pt_finfo), i4_ret_val);
    
    GOTO_EOB_AT_FAIL(wgl_str_set_font(&pt_ctx->t_str_msg, pt_ctx->h_font_msg), i4_ret_val);

    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_FONT, &b_need_scrl, NULL, i4_ret);
    GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);

    if(b_need_scrl)
    {   
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        /* must reset scrolling context after UPDATE commands if need scroll */
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);

        if(!TKTP_STM_IS_SCRLING(pt_ctx))
        {
            TKTP_STM_SET_NEED_SCRL(pt_ctx);
            if(TKTP_STM_IS_SCRLING(pt_ctx))
            {   /* state: STOPPED -> SCRLING */
                GOTO_EOB_AT_FAIL(_tktp_tool_start_scrl(pt_ctx), i4_ret_val);
            }
        }
    }
    else 
    {   
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }

___BLOCK_END___    

    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    if(!WGL_IS_FAIL(i4_ret_val) && b_auto_resize)
    {
        return _tktp_tool_update_lcsz(h_tktp);
    }
    
    return i4_ret_val;
}

static INT32 _tktp_cmd_hdr_set_color(
    IN OUT  TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    INT32       i4_ret;
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_COLOR, pv_param1, pv_param2, i4_ret);
    CATCH_FAIL(i4_ret);
    
    if(TKTP_STM_IS_NEED_SCRL(pt_ctx))
    {
        /* reset scrolling context if need scroll */
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        CATCH_FAIL(i4_ret);
    }
    
    return WGLR_OK;
}        



static INT32 _tktp_cmd_hdr_get_font(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_FONT_INFO_T *pt_finfo = (WGL_FONT_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_finfo, WGLR_INV_ARG);

    x_memcpy(pt_finfo, &pt_ctx->t_finfo_msg, sizeof(WGL_FONT_INFO_T));
    
    return WGLR_OK;
}

static INT32 _tktp_cmd_hdr_set_inset(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1)
{
    INT32               i4_ret, i4_ret_val;
    TKTP_CTX_T          *pt_ctx = NULL;
    GL_RECT_T           t_rc_tktp;
    WGL_INSET_T         *pt_inset = (WGL_INSET_T *)pv_param1;
    BOOL                b_auto_lcsz, b_dirty;
    
    if(pt_inset)
    {
        CHECK_VALID(
            pt_inset->i4_left   >= 0 &&
            pt_inset->i4_top    >= 0 &&
            pt_inset->i4_right  >= 0 &&
            pt_inset->i4_bottom >= 0, WGLR_INV_ARG);
    }
        
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    b_dirty = FALSE;
    b_auto_lcsz = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL) ? TRUE : FALSE;
    
    i4_ret_val = WGLR_OK;
___BLOCK_BEGIN___    
    if(!IS_NULL_BORDER(pt_ctx))
    {
        WGL_INSET_T   t_inset_def;
        if(pt_inset==NULL)
        {
            DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_MIN_INSET, &t_inset_def, NULL, i4_ret);
            GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);
            pt_inset = &t_inset_def;
        }

        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_SET_INSET, pt_inset, NULL, i4_ret);
        GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);
        b_dirty = TRUE;
    }
    
    if(!b_auto_lcsz)
    {
        GOTO_EOB_AT_FAIL(x_wgl_get_coords(
            pt_ctx->h_tktp, 
            WGL_COORDS_LOCAL, 
            &t_rc_tktp), i4_ret_val);

        GOTO_EOB_AT_FAIL(_tktp_tool_update_cnt_size(
            pt_ctx, 
            RECT_W(&t_rc_tktp), 
            RECT_H(&t_rc_tktp)), i4_ret_val);
    }
___BLOCK_END___    
    
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    
    if(!WGL_IS_FAIL(i4_ret_val))
    {
        if(b_dirty)
        {
            if(b_auto_lcsz)
            {
                CATCH_FAIL(_tktp_tool_update_lcsz(h_tktp));
            }
            else
            {
                CATCH_FAIL(x_wgl_repaint(h_tktp, NULL, TRUE));
            }
        }
        i4_ret_val = WGLR_OK;
    }

    return i4_ret_val;
}

static INT32 _tktp_cmd_hdr_set_cnt_inset(
    IN      HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1)
{
    INT32               i4_ret, i4_ret_fnl;
    TKTP_CTX_T          *pt_ctx = NULL;
    WGL_INSET_T         *pt_inset = (WGL_INSET_T *)pv_param1;
    BOOL                b_auto_lcsz;
    
    if(pt_inset)
    {
        CHECK_VALID(
            pt_inset->i4_left   >= 0 &&
            pt_inset->i4_top    >= 0 &&
            pt_inset->i4_right  >= 0 &&
            pt_inset->i4_bottom >= 0, WGLR_INV_ARG);
    }
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    b_auto_lcsz = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL) ? TRUE : FALSE;
    
    i4_ret_fnl = WGLR_OK;

___BLOCK_BEGIN___    
    
    GOTO_EOB_AT_FAIL(_tktp_tool_update_cnt_inset(
        pt_ctx, 
        pt_inset,
        b_auto_lcsz ? FALSE : TRUE), i4_ret_fnl);
       
___BLOCK_END___    
    
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    
    if(!WGL_IS_FAIL(i4_ret_fnl))
    {
        if(b_auto_lcsz)
        {
            CATCH_FAIL(_tktp_tool_update_lcsz(h_tktp));
        }
        else
        {
            CATCH_FAIL(x_wgl_repaint(h_tktp, NULL, TRUE));
        }
        i4_ret_fnl = WGLR_OK;
    }
    
    return i4_ret_fnl;
}        

static INT32 _tktp_cmd_hdr_set_align(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret;
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CMD_TKTP_SET_ALIGN, pv_param1, NULL, i4_ret);
    CATCH_FAIL(i4_ret);
    
    if(TKTP_STM_IS_NEED_SCRL(pt_ctx))
    {
        /* reset scrolling context if need scroll */
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        CATCH_FAIL(i4_ret);
    }    
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
                    message handler functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_msg_hdr_show( 
    IN      TKTP_CTX_T                  *pt_ctx)
{
    INT32   i4_ret;

    if(TKTP_STM_IS_VISIBLE(pt_ctx))
    {   /* state: STOPPED | SCRLING */
        return WGLR_OK;
    }

    TKTP_STM_SHOW(pt_ctx);
    
    if(TKTP_STM_IS_SCRLING(pt_ctx))
    {   /* state: HIDDEN -> SCRLING */
        CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
    }
    else
    {   /* state: HIDDEN -> STOPPED */
        /* do nothing */
    }

    return WGLR_OK;

}

static INT32 _tktp_msg_hdr_hide( 
    IN      TKTP_CTX_T                  *pt_ctx)
{
    INT32      i4_ret;
    
    if(TKTP_STM_IS_HIDDEN(pt_ctx))
    { /* state: HIDDEN */
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_OK;
    }
    
    /* state: STOPPED -> HIDDEN */
    /* state: SCRLING -> HIDDEN */
    TKTP_STM_HIDE(pt_ctx);

    /* do nothing, because tktp thread will auto break itself */
    
    return WGLR_OK;    
}

static INT32 _tktp_msg_hdr_paint(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint)
{
    INT32                               i4_ret;
    UINT32                              ui4_num_rect;
    GL_RECT_T                           t_rc_update;
    
    
    if(TKTP_STM_IS_HIDDEN(pt_ctx))
    {
        /*simulate a dummy messgage: WGL_MSG_SHOW*/
        CATCH_FAIL(_tktp_msg_hdr_show(pt_ctx));
    }
    
    if(   WGLR_OK != (i4_ret=x_wgl_get_update_region(pt_ctx->h_tktp, &ui4_num_rect, &t_rc_update))
       || ui4_num_rect != 1)
    {
        INFORM_ERR(i4_ret);
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        WGL_RECT_EMPTY(&t_rc_update);
    }

    if( !IS_NULL_BORDER(pt_ctx) )
    {
        INFORM_ERR( pt_ctx->t_intf_bdr.pf_paint_border(pt_ctx->pv_ctx_bdr, h_g, 1, &t_rc_update, ui2_state_paint) );
    }
    
    return pt_ctx->t_intf_cnt.pf_paint_content(pt_ctx->pv_ctx_cnt, h_g, 1, &t_rc_update, ui2_state_paint);
}


static INT32 _tktp_msg_hdr_enable(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      VOID                        *pv_param1)
{
    BOOL    b_enable = WGL_POINTER_TO_BOOL(pv_param1);
    INT32   i4_ret;

    if(    (    b_enable
             && TKTP_STM_IS_ENABLED(pt_ctx))
        ||
           (    !b_enable
             && TKTP_STM_IS_DISABLED(pt_ctx)))
    {
        return WGLR_OK;
    }

    if(b_enable)
    {
        TKTP_STM_ENABLE(pt_ctx);
        if(TKTP_STM_IS_SCRLING(pt_ctx))
        {   /* state: STOPPED -> SCRLING */
            CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
        }
        else
        {   /* state: HIDDEN -> HIDDEN */
            /* state: STOPPED -> STOPPED */
            /* do nothing */
        }
    }
    else
    {
        /* state: HIDDEN -> HIDDEN */
        /* state: SCRLING -> STOPPED */
        /* state: STOPPED -> STOPPED */
        TKTP_STM_DISABLE(pt_ctx);
        /* do nothing, because tktp thread will auto break itself */
    }

    return WGLR_OK;
}


static INT32 _tktp_msg_hdr_create(
    IN      HANDLE_T                    h_tktp)
{
    TKTP_CTX_T                          *pt_ctx = NULL;
    BOOL                                b_hide_after_created, b_auto_lcsz;
    INT32                               i4_ret; 
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    pt_ctx->h_tktp = h_tktp;
    b_hide_after_created = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_HIDE_AFTER_CREATED) ? TRUE : FALSE;
    b_auto_lcsz = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL) ? TRUE : FALSE;
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    
    /* auto adjust location and size */
    if(b_auto_lcsz)
    {
        CATCH_FAIL(_tktp_tool_update_lcsz(h_tktp));
    }
    else
    {
        /* issue a fake WGL_MSG_MOVE message to update context */
        CATCH_FAIL(_tktp_msg_hdr_move(h_tktp));
    }
    
    /* auto hide */
    if(b_hide_after_created)
    {
        CATCH_FAIL(_tktp_tool_hide(h_tktp));
    }
    else
    {
        CATCH_FAIL(x_wgl_show(h_tktp, WGL_SW_NORMAL));
    }

    return WGLR_OK;
}

static INT32 _tktp_msg_hdr_destroy(
    IN      HANDLE_T                    h_tktp)
{
    /* do nothing */
    return WGLR_OK;
}

static INT32 _tktp_msg_hdr_size(
    IN OUT  HANDLE_T                    h_tktp,
    IN      VOID                        *pv_param1)
{
    TKTP_CTX_T      *pt_ctx = NULL;
    INT32           i4_w = (INT32)WGL_GET_HI_WORD(pv_param1);
    INT32           i4_h = (INT32)WGL_GET_LOW_WORD(pv_param1);
    INT32           i4_ret;

    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    
    if(!IS_NULL_BORDER(pt_ctx))
    {
        DO_BDR_CMD(pt_ctx, WGL_BDR_GL_SET_SIZE, i4_w, i4_h, i4_ret);
        CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, i4_ret);
    }
    INFORM_ERR(_tktp_tool_update_cnt_size(pt_ctx, i4_w, i4_h));
    
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    return WGLR_OK;
}

static INT32 _tktp_msg_hdr_move(
    IN      HANDLE_T                    h_tktp)
{
    INT32               i4_ret, i4_ret_fnl;
    GL_RECT_T           t_rc_scrn_tktp;
    TKTP_CTX_T          *pt_ctx = NULL;
    
    CATCH_FAIL(x_wgl_get_coords(h_tktp, WGL_COORDS_SCREEN, &t_rc_scrn_tktp));
    
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_SCRN_LOC, t_rc_scrn_tktp.i4_left, t_rc_scrn_tktp.i4_top, i4_ret_fnl);
    INFORM_ERR(i4_ret_fnl);
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);
    
    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
                    ticker tape engine control functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_engine_init(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      BOOL                        b_run)
{
    INT32               i4_ret, i4_ret_val;
    TKTP_ENG_PARAM_T    t_param;
    CHAR                s_th_name[64];

    if(pt_ctx->e_state_eng != ENGST_NOT_INIT)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }
    
    pt_ctx->e_eareq_eng = TKTP_EAREQ_NONE;
    pt_ctx->h_th_eng = NULL_HANDLE;
    pt_ctx->h_sema_eng = NULL_HANDLE;

    /* create semaphore */
    i4_ret = x_sema_create(
        &pt_ctx->h_sema_eng, 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK);
    if( i4_ret != OSR_OK )
    {
        INFORM_ERR(i4_ret);
        return i4_ret;
    }
    
    /* create thread */
    x_sprintf(s_th_name, "%s%x", TKTP_ENG_THREAD_NAME_PREFIX, pt_ctx->h_tktp);
    t_param.h_tktp = pt_ctx->h_tktp;
    t_param.b_sleep_at_start = b_run ? FALSE : TRUE;
    t_param.h_sema_eng = t_param.b_sleep_at_start ? pt_ctx->h_sema_eng : NULL_HANDLE;

    i4_ret_val = x_thread_create(
        &pt_ctx->h_th_eng, 
        s_th_name,
        TKTP_ENG_THREAD_DEFAULT_STACK_SIZE,
        TKTP_ENG_THREAD_DEFAULT_PRIORITY,
        wgl_tktp_eng_core_proc,
        sizeof(TKTP_ENG_PARAM_T),
        (VOID *)&t_param);

    if(i4_ret_val != OSR_OK)
    {
        INFORM_ERR(x_sema_delete(pt_ctx->h_sema_eng));
        pt_ctx->h_th_eng = NULL_HANDLE;
        pt_ctx->h_sema_eng = NULL_HANDLE;
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }

    pt_ctx->e_state_eng = b_run ? ENGST_RUN : ENGST_SLEEP;

    return WGLR_OK;
}

static INT32 _tktp_engine_destroy(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    INT32   i4_ret, i4_ret_val;
    
    if(pt_ctx->e_state_eng==ENGST_NOT_INIT)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }
    
    pt_ctx->e_state_eng = ENGST_NOT_INIT;
    pt_ctx->e_eareq_eng = TKTP_EAREQ_DESTROY;
    pt_ctx->h_th_eng = NULL_HANDLE;
    
    INFORM_ERR(x_sema_unlock(pt_ctx->h_sema_eng));
    i4_ret_val = x_sema_delete(pt_ctx->h_sema_eng);
    pt_ctx->h_sema_eng = NULL_HANDLE;

    return i4_ret_val;
}

static INT32 _tktp_engine_go_sleep(
    IN      TKTP_CTX_T                  *pt_ctx)
{ 
    INT32   i4_ret;
    
    /* make sure the engine has been inited and in running state */
    if(pt_ctx->e_state_eng!=ENGST_RUN)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }
    
    i4_ret = x_sema_lock(
        pt_ctx->h_sema_eng, 
        X_SEMA_OPTION_NOWAIT);        
    
    pt_ctx->e_state_eng = ENGST_SLEEP;

    if(i4_ret==OSR_WOULD_BLOCK)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }
    else if(i4_ret != OSR_OK)
    {
        INFORM_ERR(i4_ret);
        return i4_ret;
    }

    return WGLR_OK;
}

static INT32 _tktp_engine_wake_up(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    INT32   i4_ret;

    /* make sure the engine has been inited and in sleeping state */
    if(pt_ctx->e_state_eng!=ENGST_SLEEP)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        return WGLR_INTERNAL_ERROR;
    }

    INFORM_ERR( x_sema_unlock(pt_ctx->h_sema_eng) ); 
    INFORM_ERR( x_sema_unlock(pt_ctx->h_sema_eng) ); 
    
    pt_ctx->e_state_eng = ENGST_RUN;
    
    return WGLR_OK;
}

static BOOL _tktp_engine_is_inited(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    return pt_ctx->e_state_eng==ENGST_NOT_INIT ? FALSE : TRUE;
}

static BOOL _tktp_engine_is_sleeping(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    return pt_ctx->e_state_eng==ENGST_SLEEP ? TRUE : FALSE;
}

#if 0
static BOOL _tktp_engine_is_running(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    return pt_ctx->e_state_eng==ENGST_RUN ? TRUE : FALSE;
}
#endif


static INT32 _tktp_engine_exec_aso_action(
    IN      HANDLE_T                        h_tktp,
    IN      TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action,
    IN      wgl_tktp_nfy_fct                pf_tktp_nfy,
    IN      VOID                            *pv_tag_nfy,
    IN OUT  TKTP_ENG_INFO_T                 *pt_info)
{
    INT32   i4_ret, i4_ret_val;
    
    i4_ret_val = WGLR_OK;
    switch(e_aso_action)
    {
        case TKTP_ASO_ACTION_NONE:
        {
            /* do nothing */
        }
        break;

        case TKTP_ASO_ACTION_HIDE:
        {
            /* notify: WGL_TKTP_NFY_COND_HIDDEN */
            if(pf_tktp_nfy)
            {
                pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_HIDDEN, pv_tag_nfy);
            }
            return _tktp_tool_hide(h_tktp);
        }

        case TKTP_ASO_ACTION_DESTROY:
        {
            /* notify: WGL_TKTP_NFY_COND_DESTROYED */
            if(pf_tktp_nfy)
            {
                pf_tktp_nfy(h_tktp, WGL_TKTP_NFY_COND_DESTROYED, pv_tag_nfy);
            }
            /* overrides the pt_info */
            pt_info->e_msg          = TKTP_ENG_MSG_DESTROY;
            pt_info->h_sema_eng     = NULL_HANDLE;
            pt_info->ui4_next_delay = 0;
            return x_wgl_destroy_widget(h_tktp);
        }

        default:
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            i4_ret_val = WGLR_INTERNAL_ERROR;
           
    }
    
    return i4_ret_val;
       
}        


/*-----------------------------------------------------------------------------
                    tools functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_tool_reg_action_after_scrl_once(
    IN      TKTP_CTX_T                      *pt_ctx,
    IN      TKTP_AFTER_SCRL_ONCE_ACTION_T   e_aso_action,
    IN      VOID                            *pv_tag_aso_nfy)
{
    INT32       i4_ret;

    switch(pt_ctx->e_aso_action) 
    {
        case TKTP_ASO_ACTION_NONE:
        {
            pt_ctx->e_aso_action   = e_aso_action;
            pt_ctx->pv_tag_aso_nfy = pv_tag_aso_nfy;
        }
    	break;

        case TKTP_ASO_ACTION_HIDE:
        {
            if(e_aso_action==TKTP_ASO_ACTION_DESTROY)
            {   /* action overriding */
                pt_ctx->e_aso_action   = TKTP_ASO_ACTION_DESTROY;
            }
            pt_ctx->pv_tag_aso_nfy = pv_tag_aso_nfy;
        }
    	break;

        case TKTP_ASO_ACTION_DESTROY:
        {
            /* do nothing, because no action can override "destroy" action */
            pt_ctx->pv_tag_aso_nfy = pv_tag_aso_nfy;
        }
    	break;

        default: 
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            return WGLR_INTERNAL_ERROR;
    }

    return WGLR_OK;

}

static INT32 _tktp_tool_hide(
    IN      HANDLE_T                    h_tktp)
{
    INT32       i4_ret;
    TKTP_CTX_T  *pt_ctx = NULL;

    CATCH_FAIL(x_wgl_show(h_tktp, WGL_SW_HIDE));
    /*[NOTE]simulate a dummy message: WGL_MSG_HIDE*/
    GET_LOCK(h_tktp, &pt_ctx, i4_ret);
    CATCH_FAIL_UNLOCK(h_tktp, pt_ctx, _tktp_msg_hdr_hide(pt_ctx));
    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);

    return WGLR_OK;
}

static INT32 _tktp_tool_start_scrl(
    IN      TKTP_CTX_T                  *pt_ctx)
{
    BOOL    b_has_text_moved;
    INT32   i4_ret;

    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_GET_HAS_TEXT_MOVED, &b_has_text_moved, NULL, i4_ret);
    CATCH_FAIL(i4_ret);
    if(!b_has_text_moved)
    {
        pt_ctx->b_scrling_first = TRUE;
    }

    if(_tktp_engine_is_inited(pt_ctx))
    {
        if(_tktp_engine_is_sleeping(pt_ctx))
        {
            CATCH_FAIL(_tktp_engine_wake_up(pt_ctx));
        }
    }
    else
    {
        CATCH_FAIL(_tktp_engine_init(pt_ctx, TRUE));
    }

    return WGLR_OK;
}

static INT32 _tktp_tool_update_lcsz(
    IN      HANDLE_T                    h_tktp)
{
    INT32           i4_ret_val, i4_ret; 
    TKTP_CTX_T      *pt_ctx = NULL;
    GL_RECT_T       t_rc_tktp;
    WGL_INSET_T     t_inset_bdr;
    INT32           i4_cnt_h_new, i4_tktp_h;

    GET_LOCK(h_tktp, &pt_ctx, i4_ret);

    i4_ret_val = WGLR_OK;

___BLOCK_BEGIN___

    if(!HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_AUTO_LCSZ_CTRL))
    {
        i4_ret_val = WGLR_INV_CMD;
        INFORM_ERR(WGLR_INV_CMD);
        GOTO_EOB;
    }

    if(!IS_NULL_BORDER(pt_ctx))
    {
        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret);
        GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);
    }
    else
    {
        _INSET_EMPTY(&t_inset_bdr);
    }
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_GET_EXPECTED_CNT_HEIGHT, &i4_cnt_h_new, NULL, i4_ret);
    GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);

    t_rc_tktp.i4_left = pt_ctx->t_lcsz.t_rc_ref.i4_left;
    t_rc_tktp.i4_right = pt_ctx->t_lcsz.t_rc_ref.i4_right;
    i4_tktp_h = i4_cnt_h_new + _INSET_GET_H(t_inset_bdr);
    switch(pt_ctx->t_lcsz.e_anchor) 
    {
        case WGL_TKTP_ANCHOR_TOP:
            t_rc_tktp.i4_top = pt_ctx->t_lcsz.t_rc_ref.i4_top;
            t_rc_tktp.i4_bottom = pt_ctx->t_lcsz.t_rc_ref.i4_top + i4_tktp_h;
    	    break;
        case WGL_TKTP_ANCHOR_BOTTOM:
            t_rc_tktp.i4_top = pt_ctx->t_lcsz.t_rc_ref.i4_bottom - i4_tktp_h;
            t_rc_tktp.i4_bottom = pt_ctx->t_lcsz.t_rc_ref.i4_bottom;
    	    break;
        default:
            WS_ASSERT(FALSE);
    }
    
___BLOCK_END___

    RELEASE_LOCK(h_tktp, pt_ctx, i4_ret);    

    if(!WGL_IS_FAIL(i4_ret_val))
    {
        GL_RECT_T   t_rc_tktp_orig;
        
        CATCH_FAIL(x_wgl_get_coords(h_tktp, WGL_COORDS_SCREEN, &t_rc_tktp_orig));
        CATCH_FAIL(x_wgl_move(h_tktp, &t_rc_tktp, WGL_SYNC_AUTO_REPAINT));
        
        if(RECT_W(&t_rc_tktp)==RECT_W(&t_rc_tktp_orig) && 
           RECT_H(&t_rc_tktp)==RECT_H(&t_rc_tktp_orig))
        {
            /* issue a fake WGL_MSG_SIZE message */
            CATCH_FAIL(_tktp_msg_hdr_size(
                h_tktp, 
                (VOID *)( (RECT_W(&t_rc_tktp)<<16) | (RECT_H(&t_rc_tktp)&0x0000FFFF) ) ));
        }
        
        if(WGL_RECT_IS_EQUAL(&t_rc_tktp, &t_rc_tktp_orig))
        {
            /* issue a repaint request */
            CATCH_FAIL(x_wgl_repaint(h_tktp, NULL, TRUE));
        }
        
        i4_ret_val = WGLR_OK;
    }

    return i4_ret_val;
}

static INT32 _tktp_tool_update_cnt_size(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      INT32                       i4_wgt_w,
    IN      INT32                       i4_wgt_h)
{
    
    WGL_INSET_T     t_inset_bdr;
    GL_RECT_T       t_rc_cnt;
    BOOL            b_need_scrl;
    INT32           i4_ret;

    if(!IS_NULL_BORDER(pt_ctx))
    {
        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret);
        if(WGL_IS_FAIL(i4_ret))
        {
            INFORM_ERR(i4_ret);
            _INSET_EMPTY(&t_inset_bdr);
        }
    }
    else
    {
        _INSET_EMPTY(&t_inset_bdr);
    }
    
    SET_RECT_BY_SIZE(&t_rc_cnt, 0, 0, i4_wgt_w, i4_wgt_h);
    _RECT_INSET(&t_rc_cnt, &t_inset_bdr);

    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_CNT_RECT, &t_rc_cnt, &b_need_scrl, i4_ret);
    CATCH_FAIL(i4_ret);

    if(b_need_scrl)
    {
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        /* must reset scrolling context after UPDATE commands if need scroll */
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        CATCH_FAIL(i4_ret);

        if(!TKTP_STM_IS_SCRLING(pt_ctx))
        {
            TKTP_STM_SET_NEED_SCRL(pt_ctx);
            if(TKTP_STM_IS_SCRLING(pt_ctx))
            {
                CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
            }
        }
    }
    else
    {
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }

    return WGLR_OK;
}
    
static INT32 _tktp_tool_update_cnt_inset(
    IN      TKTP_CTX_T                  *pt_ctx,
    IN      WGL_INSET_T                 *pt_inset, 
    IN      BOOL                        b_check_scrl)
{
    INT32   i4_ret;
    BOOL    b_need_scrl;
    
    DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_UPDATE_CNT_INSET, pt_inset, &b_need_scrl, i4_ret);
    CATCH_FAIL(i4_ret);

    if(b_check_scrl && b_need_scrl)
    {
        TKTP_CNT_RSTSCRL_PARAM_T    t_param;
        t_param.b_scrl_once = HAS_FLAG(pt_ctx->ui4_style, WGL_STL_TKTP_SCRL_ONCE) ? TRUE : FALSE;
        /* must reset scrolling context after UPDATE commands if need scroll */
        DO_TKTP_CNT_CMD(pt_ctx, WGL_CNT_TKTP_RESET_SCRL, &t_param, NULL, i4_ret);
        CATCH_FAIL(i4_ret);

        if(!TKTP_STM_IS_SCRLING(pt_ctx))
        {
            TKTP_STM_SET_NEED_SCRL(pt_ctx);
            if(TKTP_STM_IS_SCRLING(pt_ctx))
            {
                CATCH_FAIL(_tktp_tool_start_scrl(pt_ctx));
            }
        }
    }
    else
    {
        TKTP_STM_CLEAR_NEED_SCRL(pt_ctx);
    }
    
    return WGLR_OK;
    
}    

