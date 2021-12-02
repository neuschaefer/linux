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
 * $RCSfile: wgl_hts.c,v $
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
#include "res_mngr/drv/u_irrc_btn_def.h"

#include "dbg/u_dbg.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_obj_type.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/u_wgl_common.h"

#include "wgl/hts/x_wgl_hts.h"
#include "wgl/hts/wgl_hts_cnt_internal.h"
#include "wgl/hts/wgl_hts.h"
#include "wgl/hts/wgl_hts_cnt_def.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "HTS"


#define DO_HTS_CNT_CMD(_pt_ctx, _cmd_id, _param1, _param2, _ret)            \
        (_ret) = (_pt_ctx)->t_intf_cnt.pf_content_cmd_proc((_pt_ctx)->pv_ctx_cnt, (_cmd_id), (VOID *)(UINT32)(_param1), (VOID *)(UINT32)(_param2));  

#define DO_BDR_CMD(_pt_ctx, _cmd_id, _param1, _param2, _ret)                \
        (_ret) = (_pt_ctx)->t_intf_bdr.pf_border_cmd_proc((_pt_ctx)->pv_ctx_bdr, (_cmd_id), (VOID *)(UINT32)(_param1), (VOID *)(UINT32)(_param2));    

#define IS_NULL_BORDER(_pt_ctx)                                             \
    ((_pt_ctx)->t_intf_bdr.pf_create_instance==NULL)



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
    


#endif


/*-----------------------------------------------------------------------------
                    defaults & constants
 ----------------------------------------------------------------------------*/
#define HTS_DFT_SCRL_LINE_STEP          1

static WGL_HTS_KEY_MAP_T t_key_map_dft = {   
            BTN_CURSOR_UP, 
            BTN_CURSOR_DOWN, 
            BTN_YELLOW,
            BTN_BLUE  };

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_msg_proc(
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_msg,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2);
static INT32 _hts_msg_hdr_paint(
    IN      HANDLE_T                    h_hts,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint);
static INT32 _hts_msg_hdr_key_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      UINT32                      ui4_key);
static INT32 _hts_msg_hdr_size(
    IN OUT  HANDLE_T                    h_hts);
static INT32 _hts_msg_hdr_move(
    IN OUT  HANDLE_T                    h_hts);
static INT32 _hts_msg_hdr_create(
    IN OUT  HANDLE_T                    h_hts);
static INT32 _hts_cmd_proc( 
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_cmd_id,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_clear_all(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_sec_repaint(
    IN      HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_scrl_up(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_scrl_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_page_up(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_page_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_start_new_parag(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_set(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_get(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_set_justif(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_get_justif(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_set_indent(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_cur_parag_get_indent(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_reset(
    IN OUT  HTS_CTX_T                   *pt_ctx);
static INT32 _hts_cmd_hdr_append_text_sec(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_append_img_sec(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_append_tab(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_dft_sec_set_v_anchor(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_dft_sec_get_v_anchor(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_dft_sec_set_font(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_dft_sec_get_font(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_dft_sec_set_color(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_dft_sec_get_color(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_set_v_anchor(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_v_anchor(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_set_img(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_img(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_set_text(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_text(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_sec_set_font(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_font(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_set_color(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_color(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2); 
static INT32 _hts_cmd_hdr_sec_get_location(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, 
    OUT     VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_get_page_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_get_line_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_get_parag_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_set_first_visible_line(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1, 
    IN      VOID                        *pv_param2);
static INT32 _hts_cmd_hdr_get_first_visible_line(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_get_last_visible_line(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_set_scrl_line_step(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _hts_cmd_hdr_get_scrl_line_step(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_set_key_map( 
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1); 
static INT32 _hts_cmd_hdr_get_key_map(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1); 
static INT32 _hts_evt_hdr_on_change_vp_size(
    IN OUT  HANDLE_T                    h_hts,
    IN      BOOL                        b_on_create);
static INT32 _hts_tool_cur_parag_set(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      WGL_HTS_PARAG_ATTR_T        *pt_attr);
static INT32 _hts_tool_send_notify(
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_notify_code);
static INT32 _hts_tool_check_image_handle(
    IN      WGL_HIMG_TPL_T              h_img);

static INT32 _hts_tool_check_hts_scrn_location(
    IN OUT  HANDLE_T                    h_hts);


/*-----------------------------------------------------------------------------
                    exported functions
 ----------------------------------------------------------------------------*/
INT32 x_wgl_hts_init(UINT32 ui4_init_type)
{
    UINT32                      ui4_wgl_type = HT_WGL_WIDGET_HTS;
    WGL_WIDGET_CTRL_INTF_T      t_wgl_intf = { wgl_hts_inst_create, 
                                               wgl_hts_inst_destroy, 
                                               wgl_hts_default_proc,
                                               WGL_ATTR_FOCUSABLE};
    UINT32                      ui4_cnt_type = WGL_CONTENT_HTS_DEF;
    WGL_CONTENT_CTRL_INTF_T     t_cnt_intf = { wgl_hts_cnt_def_inst_create,
                                               wgl_hts_cnt_def_inst_destroy,
                                               wgl_hts_cnt_def_cmd_proc,
                                               wgl_hts_cnt_def_paint}; 
    INT32                       i4_ret;

    
    if(WGL_HTS_INIT_CTRL_LOGIC & ui4_init_type)
    {
        i4_ret = wgl_register_comp_intf(ui4_wgl_type, &t_wgl_intf);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
    }
    

    if(WGL_HTS_INIT_DEFAULT_CNT & ui4_init_type)
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
INT32 wgl_hts_inst_create( 
    WGL_WIDGET_CONTEXT_HANDLE_T         *ppv_ctx_wgt,
    WGL_WIDGET_CREATE_INPUT_T           *pt_wgt_create )
{
    HTS_CTX_T                           *pt_ctx = NULL;
    WGL_HTS_INIT_T                      *pt_init = (WGL_HTS_INIT_T *)pt_wgt_create->pv_param;
    INT32                               i4_ret, i4_ret_val;
    WGL_HTMGR_INIT_T                    t_htmgr_init;
    WGL_INSET_T                         t_inset_bdr;
    HTS_CNT_INIT_T                      t_cnt_init;
    WGL_CONTENT_CREATE_INPUT_T          t_create_cnt;
    BOOL                                b_init_htmgr, b_init_frlm;
    WGL_CANVAS_STATUS_T                 t_cvsst;
    
    *ppv_ctx_wgt = NULL;
    
    /* check init parameters */
    CHECK_VALID(((pt_init->ui4_style & (~((WGL_STL_HTS_LAST_STYLE<<1)-1))) == 0), WGLR_INV_ARG);
    CHECK_VALID(   pt_init->ui2_num_reg_font==0 
                || pt_init->at_font_reg_table!=NULL, WGLR_INV_ARG);
    
    /* check font registration table: 
       if(ui2_row_inset_top+ui2_row_inset_bottom >= font_height) return WGLR_INV_ARG  */
    /* do nothing */       
    
    /* allocate the context */
    pt_ctx = (HTS_CTX_T *)WGL_MEM_ALLOC(sizeof(HTS_CTX_T));
    CHECK_NULL(pt_ctx, WGLR_OUT_OF_MEMORY);
#ifdef CLI_LVL_ALL
    pt_ctx->ui4_mem_size = sizeof(HTS_CTX_T);   
#endif
    /*initialize canvas status*/
    wgl_cvsst_update(&t_cvsst, &pt_wgt_create->t_cvs_info, pt_wgt_create->h_palette);
    
    i4_ret_val = WGLR_OK;
    pt_ctx->at_font_reg_tbl   = NULL;
    pt_ctx->ui2_num_reg_fonts = 0;
    b_init_htmgr              = FALSE;
    b_init_frlm               = FALSE;
    pt_ctx->pv_ctx_cnt        = NULL;
    pt_ctx->pv_ctx_bdr        = NULL;

___BLOCK_BEGIN___
    
    /* init font info reg table */
    if(pt_init->ui2_num_reg_font>0)
    {
        UINT16  ui2_i;
        pt_ctx->at_font_reg_tbl = WGL_WIDGET_MEM_ALLOC(pt_ctx,sizeof(WGL_MSFS_FONT_REG_INFO_T) * pt_init->ui2_num_reg_font);
        if(pt_ctx->at_font_reg_tbl==NULL)
        {
            INFORM_ERR(WGLR_OUT_OF_MEMORY);
            i4_ret_val = WGLR_OUT_OF_MEMORY;
            GOTO_EOB;
        }
        for(ui2_i=0; ui2_i<pt_init->ui2_num_reg_font; ui2_i++)
        {
            x_strncpy(pt_ctx->at_font_reg_tbl[ui2_i].s_font_name, pt_init->at_font_reg_table[ui2_i].s_font_name, WGL_MAX_FONT_NAME);
            pt_ctx->at_font_reg_tbl[ui2_i].e_font_size          = pt_init->at_font_reg_table[ui2_i].e_font_size;
            pt_ctx->at_font_reg_tbl[ui2_i].i2_row_inset_top     = (INT16) pt_init->at_font_reg_table[ui2_i].ui2_line_inset_top;
            pt_ctx->at_font_reg_tbl[ui2_i].i2_row_inset_bottom  = (INT16) pt_init->at_font_reg_table[ui2_i].ui2_line_inset_bottom;
            pt_ctx->at_font_reg_tbl[ui2_i].ui2_mono_space_width = 0;
        }
        pt_ctx->ui2_num_reg_fonts = pt_init->ui2_num_reg_font;
    }
    else
    {
        pt_ctx->ui2_num_reg_fonts   = 0;
        pt_ctx->at_font_reg_tbl     = NULL;
    }
    
    /* init styles */
    pt_ctx->ui4_styles = pt_init->ui4_style;
    
    /* init frlm */
    wgl_frlm_init(&pt_ctx->t_frlm), i4_ret_val;
    b_init_frlm = TRUE;
    wgl_frlm_set_font_reg_table(&pt_ctx->t_frlm, pt_ctx->at_font_reg_tbl, pt_ctx->ui2_num_reg_fonts);

    /* init htmgr */
    t_htmgr_init.i4_vp_width     = 0;
    t_htmgr_init.ui2_tab_size    = 0;
    t_htmgr_init.e_justif_type   = HTMGR_JUSTIF_TYPE_LEFT;
    t_htmgr_init.ui2_indent_size = 0;
    t_htmgr_init.b_is_word_wrap  = TRUE;
    t_htmgr_init.b_no_merge_sec  = TRUE;
    t_htmgr_init.e_clr_mode      = pt_wgt_create->t_cvs_info.e_clr_mode;
    GOTO_EOB_AT_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_init->t_font_info_dft, &t_htmgr_init.t_pen_font.h_font), i4_ret_val);
    t_htmgr_init.t_pen_font.ui1_offset = WGL_HTMGR_TEXT_OFFSET_NORMAL;
    
    wgl_get_system_color(WGL_SYS_CLR_HTS_TEXT, &t_cvsst, &t_htmgr_init.t_pen_color.t_clr_text);
    wgl_get_system_color(WGL_SYS_CLR_HTS_TEXT_BK, &t_cvsst, &t_htmgr_init.t_pen_color.t_clr_bk);
    wgl_get_system_color(WGL_SYS_CLR_HTS_TEXT_BK, &t_cvsst, &t_htmgr_init.t_pen_color.t_clr_edge);
    
    t_htmgr_init.t_pen_color.ui1_opacity_fg = WGL_HTMGR_OPACITY_SOLID;
    t_htmgr_init.t_pen_color.ui1_opacity_bk = WGL_HTMGR_OPACITY_SOLID;
    GOTO_EOB_AT_FAIL(wgl_htmgr_init(&pt_ctx->t_htmgr, &pt_ctx->t_frlm, &t_htmgr_init), i4_ret_val);
    b_init_htmgr = TRUE;
    
    /* init ui2_scrl_line_step */
    pt_ctx->ui2_scrl_line_step = HTS_DFT_SCRL_LINE_STEP;
    
    /* init keymap */
    x_memcpy(&pt_ctx->t_keymap, &t_key_map_dft, sizeof(WGL_HTS_KEY_MAP_T));
    
    /* init pv_attach */
    pt_ctx->pv_attach = NULL;
    
    /* create border */
    pt_ctx->t_intf_bdr.pf_border_cmd_proc  = NULL;
    pt_ctx->t_intf_bdr.pf_create_instance  = NULL;
    pt_ctx->t_intf_bdr.pf_destroy_instance = NULL;
    pt_ctx->t_intf_bdr.pf_paint_border     = NULL;
    GOTO_EOB_AT_FAIL(wgl_query_border_intf( pt_wgt_create->ui4_type_border, &pt_ctx->t_intf_bdr ), i4_ret_val); 
    
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        WGL_BORDER_CREATE_INPUT_T       t_create_bdr;
        t_create_bdr.i4_width = RECT_W(pt_wgt_create->pt_rect); 
        t_create_bdr.i4_height = RECT_H(pt_wgt_create->pt_rect);
        t_create_bdr.ui4_style = pt_init->ui4_style;
        t_create_bdr.pt_cvsst = &t_cvsst;
        GOTO_EOB_AT_FAIL(pt_ctx->t_intf_bdr.pf_create_instance(&pt_ctx->pv_ctx_bdr, &t_create_bdr), i4_ret_val); 
    
        /*query insets of border*/
        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret);
        GOTO_EOB_AT_FAIL(i4_ret, i4_ret_val);
    }
    else
    {
        _INSET_EMPTY(&t_inset_bdr);
    } 
    
    /* create content */
    GOTO_EOB_AT_FAIL(wgl_query_content_intf( pt_wgt_create->ui4_type_content, &pt_ctx->t_intf_cnt), i4_ret_val);
    t_cnt_init.ui4_styles = pt_ctx->ui4_styles;
    t_cnt_init.pt_htmgr   = &pt_ctx->t_htmgr;
    _INSET_COPY(&t_cnt_init.t_inset_bdr, &t_inset_bdr);
    SET_RECT_BY_SIZE(&t_create_cnt.t_rect_cnt, 0, 0, RECT_W(pt_wgt_create->pt_rect), RECT_H(pt_wgt_create->pt_rect));
    _RECT_INSET(&t_create_cnt.t_rect_cnt, &t_inset_bdr);
    t_create_cnt.pt_cvsst = &t_cvsst;
    t_create_cnt.pv_init  = (VOID *)&t_cnt_init;
    GOTO_EOB_AT_FAIL(pt_ctx->t_intf_cnt.pf_create_instance(&pt_ctx->pv_ctx_cnt, &t_create_cnt), i4_ret_val);

___BLOCK_END___

    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);        

        /* destroy content */
        if(pt_ctx->pv_ctx_cnt)
        {
            INFORM_ERR(pt_ctx->t_intf_cnt.pf_destroy_instance(pt_ctx->pv_ctx_cnt));
        }
        
        /* destroy border */
        if(pt_ctx->pv_ctx_bdr)
        {
            INFORM_ERR(pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr));
        }

        /* destroy htmgr */
        if(b_init_htmgr)
        {
            wgl_htmgr_destroy(&pt_ctx->t_htmgr);
        }
        
        /* destroy frlm */
        if(b_init_frlm)
        {
            wgl_frlm_destroy(&pt_ctx->t_frlm);
        }
        
        /* free font reg table */
        if(pt_ctx->at_font_reg_tbl)
        {
            WGL_WIDGET_MEM_FREE(pt_ctx,sizeof(WGL_MSFS_FONT_REG_INFO_T) * pt_init->ui2_num_reg_font,pt_ctx->at_font_reg_tbl);
            pt_ctx->at_font_reg_tbl   = NULL;
            pt_ctx->ui2_num_reg_fonts = 0;
        }        
        WGL_MEM_FREE(pt_ctx->t_htmgr.pt_parag_cur);
        WGL_MEM_FREE(pt_ctx->t_htmgr.pt_row_cur);
        WGL_MEM_FREE(pt_ctx);
		pt_ctx = NULL;

        return i4_ret_val;
    }        
    
    
    *ppv_ctx_wgt = (WGL_WIDGET_CONTEXT_HANDLE_T)pt_ctx;
    
    return WGLR_OK;
}
    
    
INT32 wgl_hts_inst_destroy(
    WGL_WIDGET_CONTEXT_HANDLE_T         pv_ctx_wgt )
{
    HTS_CTX_T   *pt_ctx = (HTS_CTX_T *)pv_ctx_wgt;
    INT32       i4_ret;
    
    /* destroy content */
    INFORM_ERR(pt_ctx->t_intf_cnt.pf_destroy_instance(pt_ctx->pv_ctx_cnt));
    
    /* destroy border */
    if(!IS_NULL_BORDER(pt_ctx))
    {
        INFORM_ERR(pt_ctx->t_intf_bdr.pf_destroy_instance(pt_ctx->pv_ctx_bdr));
    }
    
    /* destroy htmgr */
    wgl_htmgr_destroy(&pt_ctx->t_htmgr);
    
    /* destroy frlm */
    wgl_frlm_destroy(&pt_ctx->t_frlm);
    
    /* free font reg table */
    if(pt_ctx->at_font_reg_tbl)
    {
        WGL_WIDGET_MEM_FREE(pt_ctx,sizeof(WGL_MSFS_FONT_REG_INFO_T) * pt_ctx->ui2_num_reg_fonts,pt_ctx->at_font_reg_tbl);
        pt_ctx->at_font_reg_tbl   = NULL;
        pt_ctx->ui2_num_reg_fonts = 0;
    }        

    x_memset(pt_ctx, 0, sizeof(HTS_CTX_T));
    WGL_MEM_FREE((VOID *)pt_ctx);
    pt_ctx = NULL;
    
    return WGLR_OK;   
}    


INT32 wgl_hts_default_proc( 
    HANDLE_T                            h_hts,
    UINT32                              ui4_msg,
    VOID                                *pv_param1,
    VOID                                *pv_param2)
{
    switch(ui4_msg) 
    {
        case WGL_MSG_COMMAND:
        {
            WGL_PACKED_PARAM_T *pt_params = (WGL_PACKED_PARAM_T *)pv_param2;
            return _hts_cmd_proc(h_hts, (UINT32)pv_param1, pt_params->pv_param1, pt_params->pv_param2);
        }
        
        case WGL_MSG_ACTIVATE:
        case WGL_MSG_ENABLE:
        case WGL_MSG_NOTIFY:
        case WGL_MSG_DESTROY:
        {
            /* do nothing */
            return WGLR_OK;
        }
    
    }    
    
    return _hts_msg_proc(h_hts, ui4_msg, pv_param1, pv_param2);
}    




/*-----------------------------------------------------------------------------
                    message handling functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_msg_proc(
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_msg,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2)
{
    INT32           i4_ret, i4_ret_val;
    
    i4_ret_val = WGLR_OK;
    
    switch(ui4_msg)
    {
        case WGL_MSG_PAINT:
            return _hts_msg_hdr_paint(h_hts, (GL_HGRAPHICS_T)pv_param1, WGL_POINTER_TO_UINT16(pv_param2));
        
        case WGL_MSG_KEY_DOWN:
            return _hts_msg_hdr_key_down(h_hts, (UINT32)pv_param1);
        
        case WGL_MSG_KEY_UP:
            return wgl_notify_parent_key(h_hts, (UINT32)pv_param1, WGL_KEY_STATE_DOWN);
        
        case WGL_MSG_KEY_REPEAT:
            return wgl_notify_parent_key(h_hts, (UINT32)pv_param1, WGL_KEY_STATE_REPEAT);
        
        case WGL_MSG_GET_FOCUS:
            return wgl_notify_parent_focus(h_hts, TRUE);
        
        case WGL_MSG_LOSE_FOCUS:
            return wgl_notify_parent_focus(h_hts, FALSE);
        
        case WGL_MSG_SIZE:
            return _hts_msg_hdr_size(h_hts);

        case WGL_MSG_MOVE:
            return _hts_msg_hdr_move(h_hts);       
        
        case WGL_MSG_CREATE:
            return _hts_msg_hdr_create(h_hts);

        default:
            INFORM_ERR(WGLR_INV_MSG);
            i4_ret_val = WGLR_INV_MSG;            
    }
 
    return i4_ret_val;
}      

static INT32 _hts_msg_hdr_paint(
    IN      HANDLE_T                    h_hts,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state_paint)
{
    INT32                               i4_ret;
    UINT32                              ui4_num_rect;
    GL_RECT_T                           t_rc_update;
    HTS_CTX_T                           *pt_ctx = NULL;
    
    INFORM_ERR(_hts_tool_check_hts_scrn_location(h_hts));
    
    if(  WGLR_OK!=(i4_ret=x_wgl_get_update_region(h_hts, &ui4_num_rect, &t_rc_update))
       ||ui4_num_rect != 1 )
    {
        INFORM_ERR(i4_ret);
        INFORM_ERR(WGLR_INTERNAL_ERROR);
        WGL_RECT_EMPTY(&t_rc_update);
    }
    
    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        INFORM_ERR( pt_ctx->t_intf_bdr.pf_paint_border(pt_ctx->pv_ctx_bdr, h_g, 1, &t_rc_update, ui2_state_paint) );
    }
    
    INFORM_ERR(pt_ctx->t_intf_cnt.pf_paint_content(pt_ctx->pv_ctx_cnt, h_g, 1, &t_rc_update, ui2_state_paint));
    
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);

    return WGLR_OK;
}
  
  
static INT32 _hts_msg_hdr_key_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      UINT32                      ui4_key)
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_dirty;
    UINT32      ui4_notify;
    
    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    
    i4_ret_val = WGLR_OK;
    b_dirty    = FALSE;
    ui4_notify = 0;

___BLOCK_BEGIN___    
    if(ui4_key==pt_ctx->t_keymap.ui4_scrl_up)
    {
        DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SCRL_UP, pt_ctx->ui2_scrl_line_step, &b_dirty, i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
        ui4_notify = b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_TOP;
    }
    else if(ui4_key==pt_ctx->t_keymap.ui4_scrl_down)
    {
        DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SCRL_DOWN, pt_ctx->ui2_scrl_line_step, &b_dirty, i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
        ui4_notify = b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_BOTTOM;
    }
    else if(ui4_key==pt_ctx->t_keymap.ui4_page_up)
    {
        DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_PAGE_UP, &b_dirty, NULL, i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
        ui4_notify = b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_TOP;
    }
    else if(ui4_key==pt_ctx->t_keymap.ui4_page_down)
    {
        DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_PAGE_DOWN, &b_dirty, NULL, i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
        ui4_notify = b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_BOTTOM;
    }
    else
    {
        RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
        return wgl_notify_parent_key(h_hts, ui4_key, WGL_KEY_STATE_DOWN);
    }        

___BLOCK_END___    
    
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if( !WGL_IS_FAIL(i4_ret_val) )
    {
        if(b_dirty)
        {
            CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
        }
        
        /* send notify */
        CATCH_FAIL(_hts_tool_send_notify(h_hts, ui4_notify));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_msg_hdr_size(
    IN OUT  HANDLE_T                    h_hts)
{
    return _hts_evt_hdr_on_change_vp_size(h_hts, FALSE);
}
    
static INT32 _hts_msg_hdr_move(
    IN OUT  HANDLE_T                    h_hts)
{
    return _hts_evt_hdr_on_change_vp_size(h_hts, FALSE);
}
  
static INT32 _hts_msg_hdr_create(
    IN OUT  HANDLE_T                    h_hts)
{
    return _hts_evt_hdr_on_change_vp_size(h_hts, TRUE);
}        
      

/*-----------------------------------------------------------------------------
                    command handling functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_cmd_proc( 
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_cmd_id,
    IN OUT  VOID                        *pv_param1,
    IN OUT  VOID                        *pv_param2)
{
    INT32           i4_ret, i4_ret_val;
    HTS_CTX_T       *pt_ctx = NULL;
    
    i4_ret_val = WGLR_OK;

    /* deliver content or border command */
    if(WGL_IS_CONTENT_CMD(ui4_cmd_id))
    {
        GET_LOCK(h_hts, &pt_ctx, i4_ret);
        
        /* intercept commands */
        switch(ui4_cmd_id)
        {
            case WGL_CMD_HTS_SET_CNT_INSET:
            {
                DO_HTS_CNT_CMD(pt_ctx, ui4_cmd_id, pv_param1, pv_param2, i4_ret_val);
                CATCH_FAIL_UNLOCK(h_hts, pt_ctx, i4_ret_val);
                RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
                return _hts_evt_hdr_on_change_vp_size(h_hts, FALSE);
            }
            
            default:
                DO_HTS_CNT_CMD(pt_ctx, ui4_cmd_id, pv_param1, pv_param2, i4_ret_val);
        }
        
        RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
        return i4_ret_val;
    }
    else if(WGL_IS_BORDER_CMD(ui4_cmd_id))
    {
        GET_LOCK(h_hts, &pt_ctx, i4_ret);
        if(!IS_NULL_BORDER(pt_ctx))
        {
            /* intercept commands */
            switch(ui4_cmd_id)
            {
                case WGL_CMD_GL_SET_INSET:
                {
                    DO_BDR_CMD(pt_ctx, WGL_CMD_GL_SET_INSET, pv_param1, pv_param2, i4_ret_val);
                    CATCH_FAIL_UNLOCK(h_hts, pt_ctx, i4_ret_val);
                    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
                    return _hts_evt_hdr_on_change_vp_size(h_hts, FALSE);
                }
                
                default:
                    DO_BDR_CMD(pt_ctx, ui4_cmd_id, pv_param1, pv_param2, i4_ret_val);
            }
            
        }
        RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
        return i4_ret_val;
    }
    
    /*check the validity of this widget command */
    if(! ( WGL_IS_WIDGET_CMD(ui4_cmd_id) 
           && 
           (   WGL_IS_CMD_BELONG_WIDGET(HT_WGL_WIDGET_HTS, ui4_cmd_id) 
            || WGL_IS_CMD_GENERAL(ui4_cmd_id))))
    {
        INFORM_ERR(WGLR_INV_CMD);
        return WGLR_INV_CMD;
    }    

    switch(ui4_cmd_id)
    {
        case WGL_CMD_HTS_CLEAR_ALL:
            return _hts_cmd_hdr_clear_all(h_hts, pv_param1);
        
        case WGL_CMD_HTS_SEC_REPAINT:
            return _hts_cmd_hdr_sec_repaint(h_hts, pv_param1);

        case WGL_CMD_HTS_SCRL_UP:
            return _hts_cmd_hdr_scrl_up(h_hts, pv_param1);

        case WGL_CMD_HTS_SCRL_DOWN:
            return _hts_cmd_hdr_scrl_down(h_hts, pv_param1);

        case WGL_CMD_HTS_PAGE_UP:
            return _hts_cmd_hdr_page_up(h_hts, pv_param1);

        case WGL_CMD_HTS_PAGE_DOWN:
            return _hts_cmd_hdr_page_down(h_hts, pv_param1);

        case WGL_CMD_HTS_SET_FIRST_VISIBLE_LINE:
            return _hts_cmd_hdr_set_first_visible_line(h_hts, pv_param1, pv_param2);
            
    }        

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    
    switch(ui4_cmd_id)
    {
        /* specific commands */    
        case WGL_CMD_HTS_START_NEW_PARAG:
            i4_ret_val = _hts_cmd_hdr_start_new_parag(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_HTS_CUR_PARAG_SET:
            i4_ret_val = _hts_cmd_hdr_cur_parag_set(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_CUR_PARAG_GET:
            i4_ret_val = _hts_cmd_hdr_cur_parag_get(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_CUR_PARAG_SET_JUSTIF:
            i4_ret_val = _hts_cmd_hdr_cur_parag_set_justif(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_CUR_PARAG_GET_JUSTIF:
            i4_ret_val = _hts_cmd_hdr_cur_parag_get_justif(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_CUR_PARAG_SET_INDENT:
            i4_ret_val = _hts_cmd_hdr_cur_parag_set_indent(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_CUR_PARAG_GET_INDENT:
            i4_ret_val = _hts_cmd_hdr_cur_parag_get_indent(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_RESET:
            i4_ret_val = _hts_cmd_hdr_reset(pt_ctx);
        break;
        
        case WGL_CMD_HTS_APPEND_TEXT_SEC:
            i4_ret_val = _hts_cmd_hdr_append_text_sec(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_APPEND_IMG_SEC:
            i4_ret_val = _hts_cmd_hdr_append_img_sec(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_APPEND_TAB:
            i4_ret_val = _hts_cmd_hdr_append_tab(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_HTS_DFT_SEC_SET_V_ANCHOR:
            i4_ret_val = _hts_cmd_hdr_dft_sec_set_v_anchor(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_HTS_DFT_SEC_GET_V_ANCHOR:
            i4_ret_val = _hts_cmd_hdr_dft_sec_get_v_anchor(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_DFT_SEC_SET_FONT:
            i4_ret_val = _hts_cmd_hdr_dft_sec_set_font(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_HTS_DFT_SEC_GET_FONT:
            i4_ret_val = _hts_cmd_hdr_dft_sec_get_font(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_HTS_DFT_SEC_SET_COLOR:
            i4_ret_val = _hts_cmd_hdr_dft_sec_set_color(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_HTS_DFT_SEC_GET_COLOR:
            i4_ret_val = _hts_cmd_hdr_dft_sec_get_color(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_HTS_SEC_SET_V_ANCHOR:
            i4_ret_val = _hts_cmd_hdr_sec_set_v_anchor(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_HTS_SEC_GET_V_ANCHOR:
            i4_ret_val = _hts_cmd_hdr_sec_get_v_anchor(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_HTS_SEC_SET_IMG:
            i4_ret_val = _hts_cmd_hdr_sec_set_img(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_GET_IMG:
            i4_ret_val = _hts_cmd_hdr_sec_get_img(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_SET_TEXT:
            i4_ret_val = _hts_cmd_hdr_sec_set_text(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_GET_TEXT:
            i4_ret_val = _hts_cmd_hdr_sec_get_text(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_SET_FONT:
            i4_ret_val = _hts_cmd_hdr_sec_set_font(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_GET_FONT:
            i4_ret_val = _hts_cmd_hdr_sec_get_font(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_SET_COLOR:
            i4_ret_val = _hts_cmd_hdr_sec_set_color(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_SEC_GET_COLOR:
            i4_ret_val = _hts_cmd_hdr_sec_get_color(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_HTS_SEC_GET_LOCATION:
            i4_ret_val = _hts_cmd_hdr_sec_get_location(pt_ctx, pv_param1, pv_param2);
        break;

        case WGL_CMD_HTS_GET_PAGE_NUM:
            i4_ret_val = _hts_cmd_hdr_get_page_num(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_LINE_NUM:
            i4_ret_val = _hts_cmd_hdr_get_line_num(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_PARAG_NUM:
            i4_ret_val = _hts_cmd_hdr_get_parag_num(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_FIRST_VISIBLE_LINE:
            i4_ret_val = _hts_cmd_hdr_get_first_visible_line(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_LAST_VISIBLE_LINE:
            i4_ret_val = _hts_cmd_hdr_get_last_visible_line(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_SET_SCRL_LINE_STEP:
            i4_ret_val = _hts_cmd_hdr_set_scrl_line_step(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_SCRL_LINE_STEP:
            i4_ret_val = _hts_cmd_hdr_get_scrl_line_step(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_SET_KEY_MAP:
            i4_ret_val = _hts_cmd_hdr_set_key_map(pt_ctx, pv_param1);
        break;

        case WGL_CMD_HTS_GET_KEY_MAP:
            i4_ret_val = _hts_cmd_hdr_get_key_map(pt_ctx, pv_param1);
        break;


        /* overrided general commands */    
        case WGL_CMD_GL_SET_ATTACH_DATA:
            pt_ctx->pv_attach = pv_param1;
        break;

        case WGL_CMD_GL_GET_ATTACH_DATA:
            *((VOID **)pv_param1) = pt_ctx->pv_attach;
        break;
        
        case WGL_CMD_GL_SET_COLOR:
            DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GL_SET_COLOR, pv_param1, pv_param2, i4_ret_val);
        break;
        
        case WGL_CMD_GL_GET_COLOR:
            DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GL_GET_COLOR, pv_param1, pv_param2, i4_ret_val);
        break;
        
        case WGL_CMD_GL_SET_IMAGE:
            DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GL_SET_IMG, pv_param1, pv_param2, i4_ret_val);
        break;

        case WGL_CMD_GL_GET_IMAGE:
            DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GL_GET_IMG, pv_param1, pv_param2, i4_ret_val);
        break;
#ifdef CLI_LVL_ALL
        case WGL_CMD_GL_GET_ALLOC_MEM:
        {
			UINT32 ui4_wdg_mem = pt_ctx->ui4_mem_size;
            WGL_MTX_OBJ_T*  pt_wgt_mem;
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(TEXT)} ERR: Invalid arguments. For WGL_CMD_GL_GET_ALLOC_MEM, its 1st input argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG; 
            }
            
            pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_ctx->pv_ctx_cnt;
            ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;
            if(pt_ctx->t_intf_bdr.pf_create_instance!= NULL)
            {
                pt_wgt_mem = (WGL_MTX_OBJ_T* )pt_ctx->pv_ctx_bdr;
                ui4_wdg_mem += pt_wgt_mem->ui4_mem_size;   
            }
        #ifndef MW_CC_SUPPORT_CUST_1
            UINT32 ui4_mem_size = 0;
			ui4_wdg_mem += pt_ctx->t_frlm.ui4_mem_size - sizeof(WGL_FRLM_CTX_T);
			if( wgl_htmgr_get_struct_size(&pt_ctx->t_htmgr, &ui4_mem_size) == HTMGRR_OK )
			{
				ui4_wdg_mem += ui4_mem_size;
			}
        #endif
            *((UINT32*)pv_param1) = ui4_wdg_mem;
            break;
        }
#endif 
        default:
        {
            INFORM_ERR(WGLR_INV_CMD);
            i4_ret_val = WGLR_INV_CMD;
        }
    }   
    
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);

    return i4_ret_val;        
}


static INT32 _hts_cmd_hdr_clear_all(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1) /* BOOL b_repaint */
{
    INT32       i4_ret;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param1);
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    CATCH_FAIL_UNLOCK(h_hts, pt_ctx, wgl_htmgr_del_all(&pt_ctx->t_htmgr));
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_CLEAR_ALL, NULL, NULL, i4_ret);
    CATCH_FAIL_UNLOCK(h_hts, pt_ctx, i4_ret);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(b_repaint)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_repaint(
    IN      HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1) /*  */
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    GL_RECT_T   t_rc_pu;
    
    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GET_PURC_BY_SEC, pv_param1, &t_rc_pu, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);

    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }
        
    if(RECT_IS_EFFECTIVE(t_rc_pu))
    {        
        CATCH_FAIL(x_wgl_repaint(h_hts, &t_rc_pu, TRUE));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_scrl_up(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param1);
    BOOL        b_dirty;
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SCRL_UP, pt_ctx->ui2_scrl_line_step, &b_dirty, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }
     
    if(b_repaint && b_dirty)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    /* send notify */
    CATCH_FAIL(_hts_tool_send_notify(h_hts, b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_TOP));
        
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_scrl_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param1);
    BOOL        b_dirty;
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SCRL_DOWN, pt_ctx->ui2_scrl_line_step, &b_dirty, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }

    if(b_repaint && b_dirty)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    /* send notify */
    CATCH_FAIL(_hts_tool_send_notify(h_hts, b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_BOTTOM));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_page_up(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param1);
    BOOL        b_dirty;
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_PAGE_UP, &b_dirty, NULL, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }
    
    if(b_repaint && b_dirty)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    /* send notify */
    CATCH_FAIL(_hts_tool_send_notify(h_hts, b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_TOP));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_page_down(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1)
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param1);
    BOOL        b_dirty;
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_PAGE_DOWN, &b_dirty, NULL, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }
    
    if(b_repaint && b_dirty)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    /* send notify */
    CATCH_FAIL(_hts_tool_send_notify(h_hts, b_dirty ? WGL_NC_HTS_VP_MOVED : WGL_NC_HTS_REACH_BOTTOM));
    
    return WGLR_OK;
}        


static INT32 _hts_cmd_hdr_start_new_parag(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32 i4_ret;
    
    /* check arguments */
    if(pv_param1 != NULL)
    {
        WGL_HTS_PARAG_ATTR_T  *pt_parag_attr = (WGL_HTS_PARAG_ATTR_T *)pv_param1;
        CHECK_VALID( HTS_IS_VALID_JUSTIF(pt_parag_attr->e_justif), WGLR_INV_ARG);
    }
    
    CATCH_FAIL(wgl_htmgr_insert_cr(&pt_ctx->t_htmgr));
    
    if(pv_param1 != NULL)
    {
        CATCH_FAIL(_hts_tool_cur_parag_set(pt_ctx, (WGL_HTS_PARAG_ATTR_T *)pv_param1));
    }        
    
    return WGLR_OK;
}        


static INT32 _hts_cmd_hdr_cur_parag_set(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_HTS_PARAG_ATTR_T    *pt_attr = (WGL_HTS_PARAG_ATTR_T *)pv_param1;
    INT32                   i4_ret;
    
    CHECK_NULL(pt_attr, WGLR_INV_ARG);
    CHECK_VALID( HTS_IS_VALID_JUSTIF(pt_attr->e_justif), WGLR_INV_ARG);
    
    CATCH_FAIL(_hts_tool_cur_parag_set(pt_ctx, pt_attr));
       
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_cur_parag_get(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_HTS_PARAG_ATTR_T    *pt_attr = (WGL_HTS_PARAG_ATTR_T *)pv_param1;
    WGL_HTMGR_PARAG_GAP_T   t_parag_gap;
    INT32                   i4_ret;

    CHECK_NULL(pt_attr, WGLR_INV_ARG);    

    x_memset(pt_attr, 0, sizeof(WGL_HTS_PARAG_ATTR_T));    
    pt_attr->e_justif               = HTS_CONVERT_JUSTIF_HTMGR_TO_HTS(wgl_htmgr_get_justif_type(&pt_ctx->t_htmgr));
    pt_attr->ui2_indent             = HTMGR_GET_INDENT_SIZE(&pt_ctx->t_htmgr);    
    CATCH_FAIL(wgl_htmgr_cur_parag_get_gap(&pt_ctx->t_htmgr, &t_parag_gap));
    pt_attr->ui2_parag_gap_top      = t_parag_gap.ui2_parag_gap_top;
    pt_attr->ui2_parag_gap_bottom   = t_parag_gap.ui2_parag_gap_bottom;
    pt_attr->i4_line_gap_top        = t_parag_gap.i4_line_gap_top;
    pt_attr->i4_line_gap_bottom     = t_parag_gap.i4_line_gap_bottom;
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_cur_parag_set_justif(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32                   i4_ret;
    WGL_HTS_JUSTIF_TYPE_T   e_justif = (WGL_HTS_JUSTIF_TYPE_T)((UINT32)pv_param1);
    
    CHECK_VALID(HTS_IS_VALID_JUSTIF(e_justif), WGLR_INV_ARG);
    CATCH_FAIL(wgl_htmgr_set_justif_type(&pt_ctx->t_htmgr, HTS_CONVERT_JUSTIF_HTS_TO_HTMGR(e_justif)));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_cur_parag_get_justif(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_HTS_JUSTIF_TYPE_T   *pe_justif = (WGL_HTS_JUSTIF_TYPE_T *)(pv_param1);
    
    CHECK_NULL(pe_justif, WGLR_INV_ARG);
    *pe_justif = HTS_CONVERT_JUSTIF_HTMGR_TO_HTS(wgl_htmgr_get_justif_type(&pt_ctx->t_htmgr));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_cur_parag_set_indent(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)
{
    INT32   i4_ret;
    
    CATCH_FAIL(wgl_htmgr_set_indent_size(&pt_ctx->t_htmgr, WGL_POINTER_TO_UINT16(pv_param1)));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_cur_parag_get_indent(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16 *pui2_indent = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_indent, WGLR_INV_ARG);
    *pui2_indent = HTMGR_GET_INDENT_SIZE(&pt_ctx->t_htmgr);
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_reset(
    IN OUT  HTS_CTX_T                   *pt_ctx)
{
    INT32 i4_ret;

    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_RESET, NULL, NULL, i4_ret);
    CATCH_FAIL(i4_ret);
    CATCH_FAIL(wgl_htmgr_reset(&pt_ctx->t_htmgr));

    return WGLR_OK;
}   

static INT32 _hts_cmd_hdr_append_text_sec(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    INT32                   i4_ret;
    WGL_HTS_TEXT_INFO_T     *pt_text = (WGL_HTS_TEXT_INFO_T *)pv_param1;
    UINT16                  *pui2_sec_id_new = (UINT16 *)pv_param2;

    CHECK_NULL(pui2_sec_id_new, WGLR_INV_ARG);    
    
    *pui2_sec_id_new = WGL_HTS_INV_SEC_ID;
    
    if(pt_text)
    {
        WGL_HTMGR_TEXT_ATTR_T    t_sec_attr;
        
        CHECK_VALID(   HTS_IS_VALID_STRING(&pt_text->t_string) 
                    && HTS_IS_VALID_V_ANCHOR(pt_text->e_v_anchor), WGLR_INV_ARG);
        
        t_sec_attr.e_v_anchor = HTS_CONVERT_V_ANCHOR_HTS_TO_HTMGR(pt_text->e_v_anchor);
        CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, &pt_text->t_font, &t_sec_attr.h_font));
        t_sec_attr.ui1_offset = WGL_HTMGR_TEXT_OFFSET_NORMAL;                        
        _COLOR_COPY(&t_sec_attr.t_clr_text, &pt_text->t_clr_text);
        _COLOR_COPY(&t_sec_attr.t_clr_bk, &pt_text->t_clr_text_bk);
        _COLOR_COPY(&t_sec_attr.t_clr_edge, &pt_text->t_clr_text_bk);
        t_sec_attr.ui1_opacity_fg = WGL_HTMGR_OPACITY_SOLID;
        t_sec_attr.ui1_opacity_bk = WGL_HTMGR_OPACITY_SOLID;
        
        CATCH_FAIL(wgl_htmgr_append_text_sec(
            &pt_ctx->t_htmgr, 
            pt_text->t_string.w2s_str, 
            pt_text->t_string.ui2_len_str, 
            &t_sec_attr,
            pui2_sec_id_new));
    }
    else
    {
        CATCH_FAIL(wgl_htmgr_append_text_sec(
            &pt_ctx->t_htmgr, 
            NULL, 
            0, 
            NULL,
            pui2_sec_id_new));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_append_img_sec(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    INT32               i4_ret;
    WGL_HTS_IMG_INFO_T  *pt_img = (WGL_HTS_IMG_INFO_T *)pv_param1;
    UINT16              *pui2_sec_id_new = (UINT16 *)pv_param2;

    CHECK_NULL(pui2_sec_id_new, WGLR_INV_ARG);    
    
    *pui2_sec_id_new = WGL_HTS_INV_SEC_ID;
    
    if(pt_img)
    {
        WGL_HTMGR_IMG_INFO_T    t_htm_img;
        
        CATCH_FAIL(_hts_tool_check_image_handle(pt_img->h_img));
        t_htm_img.h_img          = pt_img->h_img;
        t_htm_img.e_v_anchor     = HTS_CONVERT_V_ANCHOR_HTS_TO_HTMGR(pt_img->e_v_anchor);
        t_htm_img.ui2_gap_left   = pt_img->ui2_gap_left;
        t_htm_img.ui2_gap_top    = pt_img->ui2_gap_top;
        t_htm_img.ui2_gap_right  = pt_img->ui2_gap_right;
        t_htm_img.ui2_gap_bottom = pt_img->ui2_gap_bottom;
        CATCH_FAIL(wgl_htmgr_append_image(&pt_ctx->t_htmgr, &t_htm_img, pui2_sec_id_new));
    }
    else
    {
        CATCH_FAIL(wgl_htmgr_append_image(&pt_ctx->t_htmgr, NULL, pui2_sec_id_new));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_append_tab(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1) /* UINT16 ui2_tab_size [must > 0]*/
{
    INT32           i4_ret;
    UINT16          ui2_tab_size = WGL_POINTER_TO_UINT16(pv_param1);
    
    CHECK_VALID(ui2_tab_size>0, WGLR_INV_ARG);
    wgl_htmgr_set_tab_size(&pt_ctx->t_htmgr, ui2_tab_size);
    CATCH_FAIL(wgl_htmgr_write_tab(&pt_ctx->t_htmgr));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_dft_sec_set_v_anchor(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1) /* WGL_HTS_V_ANCHOR_T */
{
    INT32                   i4_ret;
    WGL_HTS_V_ANCHOR_T      e_v_anchor = (WGL_HTS_V_ANCHOR_T)((UINT32)pv_param1);
    
    CHECK_VALID( HTS_IS_VALID_V_ANCHOR(e_v_anchor), WGLR_INV_ARG);
    CATCH_FAIL(wgl_htmgr_set_cur_sec_v_anchor(&pt_ctx->t_htmgr, HTS_CONVERT_V_ANCHOR_HTS_TO_HTMGR(e_v_anchor)));
    
    return WGLR_OK;
}

static INT32 _hts_cmd_hdr_dft_sec_get_v_anchor(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* (WGL_HTS_V_ANCHOR_T *) */
{
    WGL_HTS_V_ANCHOR_T  *pe_v_anchor = (WGL_HTS_V_ANCHOR_T *)pv_param1;
    
    CHECK_NULL(pe_v_anchor, WGLR_INV_ARG);
    *pe_v_anchor = HTS_CONVERT_V_ANCHOR_HTMGR_TO_HTS(wgl_htmgr_get_cur_sec_v_anchor(&pt_ctx->t_htmgr));
    
    return WGLR_OK;
}            


static INT32 _hts_cmd_hdr_dft_sec_set_font(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)  /* (WGL_FONT_INFO_T *) [can be NULL] */
{
    INT32                   i4_ret;
    WGL_FONT_INFO_T         *pt_font_info = (WGL_FONT_INFO_T *)pv_param1;
    WGL_HTMGR_PEN_FONT_T    t_pen_font;
    WGL_FONT_INFO_T         t_font_sys;
    
    if(pt_font_info==NULL)
    {
        CATCH_FAIL(x_wgl_get_system_font(&t_font_sys));
        pt_font_info = &t_font_sys;
    }   
    CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, pt_font_info, &t_pen_font.h_font));
    t_pen_font.ui1_offset = WGL_HTMGR_TEXT_OFFSET_NORMAL;
    CATCH_FAIL(wgl_htmgr_set_pen_font(&pt_ctx->t_htmgr, &t_pen_font));
    
    return WGLR_OK;
}

static INT32 _hts_cmd_hdr_dft_sec_get_font(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* (WGL_FONT_INFO_T *) */
{
    INT32                   i4_ret;
    WGL_FONT_INFO_T         *pt_font_info = (WGL_FONT_INFO_T *)pv_param1;
    WGL_HTMGR_PEN_FONT_T    t_pen_font;
    
    CHECK_NULL(pt_font_info, WGLR_INV_ARG);
    wgl_htmgr_get_pen_font(&pt_ctx->t_htmgr, &t_pen_font);
    CATCH_FAIL(wgl_frlm_get_info(&pt_ctx->t_frlm, t_pen_font.h_font, pt_font_info));
    
    return WGLR_OK;
}      

static INT32 _hts_cmd_hdr_dft_sec_set_color(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT8 ui1_target*/
    IN      VOID                        *pv_param2) /* GL_COLOR_T *pt_clr [cannot be NULL] */
{
    UINT8                   ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    GL_COLOR_T              *pt_clr = (GL_COLOR_T *)pv_param2;
    WGL_HTMGR_PEN_COLOR_T   t_pen_clr;
    INT32                   i4_ret;
    
    CHECK_NULL(pt_clr, WGLR_INV_ARG);
    
    wgl_htmgr_get_pen_color(&pt_ctx->t_htmgr, &t_pen_clr);
    switch(ui1_target)
    {
        case WGL_CLR_HTS_SEC_TEXT:
            _COLOR_COPY(&t_pen_clr.t_clr_text, pt_clr);
        break;
        case WGL_CLR_HTS_SEC_TEXT_BK:
            _COLOR_COPY(&t_pen_clr.t_clr_bk, pt_clr);
            _COLOR_COPY(&t_pen_clr.t_clr_edge, pt_clr);
        break;
        default:
           INFORM_ERR(WGLR_NOT_SUPPORT_TARGET);
           return WGLR_NOT_SUPPORT_TARGET;
    }
    wgl_htmgr_set_pen_color(&pt_ctx->t_htmgr, &t_pen_clr);
    
    return WGLR_OK;
}

static INT32 _hts_cmd_hdr_dft_sec_get_color(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT8 ui1_target */
    OUT     VOID                        *pv_param2) /* GL_COLOR_T *pt_clr */
{
    UINT8                   ui1_target = WGL_POINTER_TO_UINT8(pv_param1);
    GL_COLOR_T              *pt_clr = (GL_COLOR_T *)pv_param2;
    WGL_HTMGR_PEN_COLOR_T   t_pen_clr;
    INT32                   i4_ret;
    
    CHECK_NULL(pt_clr, WGLR_INV_ARG);
    
    wgl_htmgr_get_pen_color(&pt_ctx->t_htmgr, &t_pen_clr);
    switch(ui1_target)
    {
        case WGL_CLR_HTS_SEC_TEXT:
            _COLOR_COPY(pt_clr, &t_pen_clr.t_clr_text);
        break;
        case WGL_CLR_HTS_SEC_TEXT_BK:
            _COLOR_COPY(pt_clr, &t_pen_clr.t_clr_bk);
        break;
        default:
            INFORM_ERR(WGLR_NOT_SUPPORT_TARGET);
            return WGLR_NOT_SUPPORT_TARGET;
    }
    
    return WGLR_OK;
}      

static INT32 _hts_cmd_hdr_sec_set_v_anchor(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    IN      VOID                        *pv_param2) /* WGL_HTS_V_ANCHOR_T e_v_anchor */
{
    INT32               i4_ret;
    UINT16              ui2_sec_id = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_V_ANCHOR_T  e_v_anchor = (WGL_HTS_V_ANCHOR_T)((UINT32)pv_param2);
    
    CHECK_VALID(HTS_IS_VALID_V_ANCHOR(e_v_anchor), WGLR_INV_ARG );
    
    i4_ret = wgl_htmgr_sec_set_v_anchor(&pt_ctx->t_htmgr, ui2_sec_id, HTS_CONVERT_V_ANCHOR_HTS_TO_HTMGR(e_v_anchor));
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}

static INT32 _hts_cmd_hdr_sec_get_v_anchor(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    OUT     VOID                        *pv_param2) /* WGL_HTS_V_ANCHOR_T *pe_v_anchor */
{
    INT32                   i4_ret;
    UINT16                  ui2_sec_id   = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_V_ANCHOR_T      *pe_v_anchor = (WGL_HTS_V_ANCHOR_T *)pv_param2;
    WGL_HTMGR_V_ANCHOR_T    e_v_anchor_htmgr;
    
    CHECK_NULL(pe_v_anchor, WGLR_INV_ARG);    
    *pe_v_anchor = WGL_HTS_V_ANCHOR_BOTTOM;
    
    i4_ret = wgl_htmgr_sec_get_v_anchor(&pt_ctx->t_htmgr, ui2_sec_id, &e_v_anchor_htmgr);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    *pe_v_anchor = HTS_CONVERT_V_ANCHOR_HTMGR_TO_HTS(e_v_anchor_htmgr);
    return WGLR_OK;
}            

static INT32 _hts_cmd_hdr_sec_set_img(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    IN      VOID                        *pv_param2) /* WGL_HTS_IMG_INFO_T *pt_img [cannot be NULL]*/
{
    INT32                   i4_ret;
    UINT16                  ui2_sec_id = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_IMG_INFO_T      *pt_img    = (WGL_HTS_IMG_INFO_T *)pv_param2;
    WGL_HTMGR_IMG_INFO_T    t_img_htmgr;
    
    CHECK_NULL(pt_img, WGLR_INV_ARG);
    CATCH_FAIL(_hts_tool_check_image_handle(pt_img->h_img));
    CHECK_VALID(HTS_IS_VALID_V_ANCHOR(pt_img->e_v_anchor), WGLR_INV_ARG );
    
    t_img_htmgr.h_img = pt_img->h_img;
    t_img_htmgr.e_v_anchor = HTS_CONVERT_V_ANCHOR_HTS_TO_HTMGR(pt_img->e_v_anchor);
    t_img_htmgr.ui2_gap_left = pt_img->ui2_gap_left;
    t_img_htmgr.ui2_gap_top = pt_img->ui2_gap_top;
    t_img_htmgr.ui2_gap_right = pt_img->ui2_gap_right;
    t_img_htmgr.ui2_gap_bottom = pt_img->ui2_gap_bottom;
    
    i4_ret = wgl_htmgr_sec_set_img_info(&pt_ctx->t_htmgr, ui2_sec_id, &t_img_htmgr);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_get_img(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    OUT     VOID                        *pv_param2) /* WGL_HTS_IMG_INFO_T *pt_img_out */
{
    INT32              i4_ret;
    UINT16             ui2_sec_id = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_IMG_INFO_T *pt_img_out = (WGL_HTS_IMG_INFO_T *)pv_param2;
    WGL_HTMGR_IMG_INFO_T    t_img_htmgr;
    
    CHECK_NULL(pt_img_out, WGLR_INV_ARG);
    
    i4_ret = wgl_htmgr_sec_get_img_info(&pt_ctx->t_htmgr, ui2_sec_id, &t_img_htmgr);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    pt_img_out->h_img = t_img_htmgr.h_img;
    pt_img_out->e_v_anchor = HTS_CONVERT_V_ANCHOR_HTMGR_TO_HTS(t_img_htmgr.e_v_anchor);
    pt_img_out->ui2_gap_left = t_img_htmgr.ui2_gap_left;
    pt_img_out->ui2_gap_top = t_img_htmgr.ui2_gap_top;
    pt_img_out->ui2_gap_right = t_img_htmgr.ui2_gap_right;
    pt_img_out->ui2_gap_bottom = t_img_htmgr.ui2_gap_bottom;
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_set_text(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    IN      VOID                        *pv_param2) /* WGL_HTS_STRING_T *pt_str [cannot be NULL]*/
{
    INT32                   i4_ret;
    UINT16                  ui2_sec_id = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_STRING_T        *pt_str    = (WGL_HTS_STRING_T *)pv_param2;
        
    CHECK_VALID(pt_str!=NULL && HTS_IS_VALID_STRING(pt_str), WGLR_INV_ARG);

    i4_ret = wgl_htmgr_sec_set_text(&pt_ctx->t_htmgr, ui2_sec_id, pt_str->w2s_str, pt_str->ui2_len_str);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_get_text(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    OUT     VOID                        *pv_param2) /* WGL_HTS_STRING_T *pt_str_out*/
{
    INT32              i4_ret;
    UINT16             ui2_sec_id  = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_HTS_STRING_T   *pt_str_out = (WGL_HTS_STRING_T *)pv_param2;
    
    CHECK_NULL(pt_str_out, WGLR_INV_ARG);
    CHECK_VALID(   pt_str_out->w2s_str!=NULL 
                && pt_str_out->ui2_size_buf!=0
                && pt_str_out->ui2_len_str==0, WGLR_INV_ARG);
    
    i4_ret = wgl_htmgr_sec_get_text(&pt_ctx->t_htmgr, ui2_sec_id, pt_str_out->w2s_str, pt_str_out->ui2_size_buf, &pt_str_out->ui2_len_str);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_set_font(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    IN      VOID                        *pv_param2) /* WGL_FONT_INFO_T *pt_font [can be NULL]*/
{
    INT32               i4_ret;
    UINT16              ui2_sec_id  = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_FONT_INFO_T     *pt_font    = (WGL_FONT_INFO_T *)pv_param2;
    WGL_FONT_INFO_T     t_font_sys;
    HFONT_T             h_font;
    
    if(pt_font==NULL)
    {
        CATCH_FAIL(x_wgl_get_system_font(&t_font_sys));
        pt_font = &t_font_sys;
    }   
    CATCH_FAIL(wgl_frlm_get_handle_by_info(&pt_ctx->t_frlm, pt_font, &h_font));

    i4_ret = wgl_htmgr_sec_set_font(&pt_ctx->t_htmgr, ui2_sec_id, h_font);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_get_font(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* UINT16 ui2_sec_id */
    OUT     VOID                        *pv_param2) /* WGL_FONT_INFO_T *pt_font_out*/
{
    INT32               i4_ret;
    UINT16              ui2_sec_id   = WGL_POINTER_TO_UINT16(pv_param1);
    WGL_FONT_INFO_T     *pt_font_out = (WGL_FONT_INFO_T *)pv_param2;
    HFONT_T             h_font;
    
    CHECK_NULL(pt_font_out, WGLR_INV_ARG);
    
    i4_ret = wgl_htmgr_sec_get_font(&pt_ctx->t_htmgr, ui2_sec_id, &h_font);
    if(HTMGR_IS_FAIL(i4_ret))
    {
        INFORM_ERR(i4_ret);
        return WGLR_INV_ARG;
    }
    
    CATCH_FAIL(wgl_frlm_get_info(&pt_ctx->t_frlm, h_font, pt_font_out));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_set_color(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* WGL_PACK_2(ui2_sec_id, ui1_target) */
    IN      VOID                        *pv_param2) /* GL_COLOR_T *pt_clr [cannot be NULL]*/
{
    INT32       i4_ret, i4_ret_val;
    UINT16      ui2_sec_id = WGL_GET_HI_WORD(pv_param1);
    UINT8       ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
    GL_COLOR_T  *pt_clr = (GL_COLOR_T *)pv_param2;
    
    CHECK_NULL(pt_clr, WGLR_INV_ARG);
    
    switch(ui1_target)
    {
        case WGL_CLR_HTS_SEC_TEXT:
            i4_ret_val = wgl_htmgr_sec_set_color_text(&pt_ctx->t_htmgr, ui2_sec_id, pt_clr);
        break;
        case WGL_CLR_HTS_SEC_TEXT_BK:
            i4_ret_val = wgl_htmgr_sec_set_color_text_bk(&pt_ctx->t_htmgr, ui2_sec_id, pt_clr);
        break;
        default:
           INFORM_ERR(WGLR_NOT_SUPPORT_TARGET);
           return WGLR_NOT_SUPPORT_TARGET;
    }
    
    if(HTMGR_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_get_color(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* WGL_PACK_2(ui2_sec_id, ui1_target) */
    OUT     VOID                        *pv_param2) /* GL_COLOR_T *pt_clr_out */
{
    
    INT32       i4_ret, i4_ret_val;
    UINT16      ui2_sec_id = WGL_GET_HI_WORD(pv_param1);
    UINT8       ui1_target = WGL_GET_LOW_LOW_BYTE(pv_param1);
    GL_COLOR_T  *pt_clr_out = (GL_COLOR_T *)pv_param2;
    
    CHECK_NULL(pt_clr_out, WGLR_INV_ARG);
    
    switch(ui1_target)
    {
        case WGL_CLR_HTS_SEC_TEXT:
            i4_ret_val = wgl_htmgr_sec_get_color_text(&pt_ctx->t_htmgr, ui2_sec_id, pt_clr_out);
        break;
        case WGL_CLR_HTS_SEC_TEXT_BK:
            i4_ret_val = wgl_htmgr_sec_get_color_text_bk(&pt_ctx->t_htmgr, ui2_sec_id, pt_clr_out);
        break;
        default:
           INFORM_ERR(WGLR_NOT_SUPPORT_TARGET);
           return WGLR_NOT_SUPPORT_TARGET;
    }
    
    if(HTMGR_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return WGLR_INV_ARG;
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_sec_get_location(
    IN      HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1, /* WGL_PACK_2(ui2_sec_id_start, ui2_sec_id_end) */
    OUT     VOID                        *pv_param2) /* WGL_HTS_LINE_RANGE_T *pt_range_out */
{
    UINT16                  ui2_sec_id_start = WGL_GET_HI_WORD(pv_param1);
    UINT16                  ui2_sec_id_end   = WGL_GET_LOW_WORD(pv_param1);
    WGL_HTS_LINE_RANGE_T    *pt_range_out    = (WGL_HTS_LINE_RANGE_T *)pv_param2; 
    
    CHECK_NULL(pt_range_out, WGLR_INV_ARG);
    return wgl_htmgr_get_line_range_by_sec_range(
        &pt_ctx->t_htmgr, 
        ui2_sec_id_start, 
        ui2_sec_id_end,
        &pt_range_out->ui2_line_start,
        &pt_range_out->ui2_line_end);
}    

static INT32 _hts_cmd_hdr_get_page_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_num_page */
{
    INT32   i4_ret;
    
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GET_PAGE_NUM, pv_param1, NULL, i4_ret);
    
    return i4_ret;
}        

static INT32 _hts_cmd_hdr_get_line_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_num_line */
{
    UINT16  *pui2_num_line = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_num_line, WGLR_INV_ARG);
    *pui2_num_line = HTMGR_GET_ROW_NUM(&pt_ctx->t_htmgr);
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_get_parag_num(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_num_parag */
{
    UINT16  *pui2_num_parag = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_num_parag, WGLR_INV_ARG);
    *pui2_num_parag = wgl_htmgr_get_paragraph_num(&pt_ctx->t_htmgr);
    
    return WGLR_OK;
}        


static INT32 _hts_cmd_hdr_set_first_visible_line(
    IN OUT  HANDLE_T                    h_hts,
    IN      VOID                        *pv_param1, /* UINT16 ui2_vli_first */
    IN      VOID                        *pv_param2) /* BOOL b_repaint */
{
    INT32       i4_ret, i4_ret_val;
    HTS_CTX_T   *pt_ctx = NULL;
    BOOL        b_repaint = WGL_POINTER_TO_BOOL(pv_param2);
    BOOL        b_dirty;
    
    CHECK_VALID(b_repaint==TRUE || b_repaint==FALSE, WGLR_INV_ARG);

    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SET_VLI_FIRST, pv_param1, &b_dirty, i4_ret_val);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(WGL_IS_FAIL(i4_ret_val))
    {
        INFORM_ERR(i4_ret_val);
        return i4_ret_val;
    }
    
    if(b_repaint && b_dirty)
    {
        CATCH_FAIL(x_wgl_repaint(h_hts, NULL, TRUE));
    }
    
    /* send notify */
    if(b_dirty)
    {
        CATCH_FAIL(_hts_tool_send_notify(h_hts, WGL_NC_HTS_VP_MOVED));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_get_first_visible_line(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_vli_first */
{
    INT32 i4_ret;
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GET_VLI_FIRST, pv_param1, NULL, i4_ret);
    CATCH_FAIL(i4_ret);
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_get_last_visible_line(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_vli_last */
{
    INT32 i4_ret;
    CHECK_NULL(pv_param1, WGLR_INV_ARG);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GET_VLI_LAST, pv_param1, NULL, i4_ret);
    CATCH_FAIL(i4_ret);

    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_set_scrl_line_step(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1)/* UINT16 ui2_scrl_line_step */
{
    pt_ctx->ui2_scrl_line_step = WGL_POINTER_TO_UINT16(pv_param1);
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_get_scrl_line_step(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* UINT16 *pui2_scrl_line_step */
{
    UINT16  *pui2_scrl_line_step = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_scrl_line_step, WGLR_INV_ARG);
    *pui2_scrl_line_step = pt_ctx->ui2_scrl_line_step;
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_set_key_map(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      VOID                        *pv_param1) /* WGL_HTS_KEY_MAP_T *pt_keymap [cannot be NULL] */
{
    WGL_HTS_KEY_MAP_T *pt_keymap = (WGL_HTS_KEY_MAP_T *)pv_param1;
    
    CHECK_NULL(pt_keymap, WGLR_INV_ARG);
    
    x_memcpy(&pt_ctx->t_keymap, pt_keymap, sizeof(WGL_HTS_KEY_MAP_T));
    
    return WGLR_OK;
}        

static INT32 _hts_cmd_hdr_get_key_map(
    IN      HTS_CTX_T                   *pt_ctx,
    OUT     VOID                        *pv_param1) /* WGL_HTS_KEY_MAP_T *pt_keymap_out */
{
    WGL_HTS_KEY_MAP_T *pt_keymap_out = (WGL_HTS_KEY_MAP_T *)pv_param1;
    
    CHECK_NULL(pt_keymap_out, WGLR_INV_ARG);
    
    x_memcpy(pt_keymap_out, &pt_ctx->t_keymap, sizeof(WGL_HTS_KEY_MAP_T));
    
    return WGLR_OK;
}        


/*-----------------------------------------------------------------------------
                    event handling functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_evt_hdr_on_change_vp_size(
    IN OUT  HANDLE_T                    h_hts,
    IN      BOOL                        b_on_create)
{
    INT32                           i4_ret, i4_ret_val;
    HTS_CTX_T                       *pt_ctx = NULL;
    WGL_INSET_T                     t_inset_bdr;
    GL_RECT_T                       t_rc_scrn_hts;
    HTS_CNT_UPDATE_CNT_RECT_RET_T   t_ret;
    
    i4_ret_val = WGLR_OK;
    
    CATCH_FAIL(x_wgl_get_coords(h_hts, WGL_COORDS_SCREEN, &t_rc_scrn_hts));

    GET_LOCK(h_hts, &pt_ctx, i4_ret);

___BLOCK_BEGIN___    
    
    if( !IS_NULL_BORDER(pt_ctx) )
    {
        DO_BDR_CMD(pt_ctx, WGL_BDR_GL_SET_SIZE, RECT_W(&t_rc_scrn_hts), RECT_H(&t_rc_scrn_hts), i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
        DO_BDR_CMD(pt_ctx, WGL_CMD_GL_GET_INSET, &t_inset_bdr, NULL, i4_ret_val);
        GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
    }
    else
    {
        _INSET_EMPTY(&t_inset_bdr);
    }
    
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SET_HTS_SCRN_RECT, &t_rc_scrn_hts, NULL, i4_ret_val);
    GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_SET_BDR_INSET, &t_inset_bdr, NULL, i4_ret_val);
    GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_UPDATE_CNT_RECT, &t_ret, NULL, i4_ret_val);
    GOTO_EOB_AT_FAIL(i4_ret_val, i4_ret_val);
    
    if(t_ret.b_vp_size_changed)
    {
        GOTO_EOB_AT_FAIL(wgl_htmgr_set_viewport_width(&pt_ctx->t_htmgr, (INT32)t_ret.t_size_vp.ui4_width), i4_ret_val);
    }
    
    if(b_on_create)
    {
        wgl_htmgr_set_def_vp_width(&pt_ctx->t_htmgr, (UINT16)t_ret.t_size_vp.ui4_width );
    }
    
___BLOCK_END___    
    
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);

    if(!WGL_IS_FAIL(i4_ret_val))
    {
        if(t_ret.b_vp_size_changed)
        {
            HANDLE_T    h_parent;
            
            /* send notify */
            CATCH_FAIL(x_wgl_get_parent(h_hts, &h_parent));
            if(h_parent)
            {
                WGL_NOTIFY_DATA_T       t_notify;
                x_memset(&t_notify, 0, sizeof(WGL_NOTIFY_DATA_T));
                t_notify.ui4_nc_code    = WGL_NC_HTS_VP_MOVED;
                INFORM_ERR(x_wgl_send_notify(h_parent, t_notify.ui4_nc_code, h_hts, &t_notify));
            }
        }
    }
    
    return WGLR_OK;
}        



/*-----------------------------------------------------------------------------
                    toolkit functions
 ----------------------------------------------------------------------------*/
static INT32 _hts_tool_cur_parag_set(
    IN OUT  HTS_CTX_T                   *pt_ctx,
    IN      WGL_HTS_PARAG_ATTR_T        *pt_attr)
{
    INT32                   i4_ret;
    WGL_HTMGR_PARAG_GAP_T   t_parag_gap;
    
    CATCH_FAIL(wgl_htmgr_set_indent_size(&pt_ctx->t_htmgr, pt_attr->ui2_indent));
    CATCH_FAIL(wgl_htmgr_set_justif_type(&pt_ctx->t_htmgr, HTS_CONVERT_JUSTIF_HTS_TO_HTMGR(pt_attr->e_justif)));
    t_parag_gap.ui2_parag_gap_top = pt_attr->ui2_parag_gap_top;
    t_parag_gap.ui2_parag_gap_bottom = pt_attr->ui2_parag_gap_bottom;
    t_parag_gap.i4_line_gap_top = pt_attr->i4_line_gap_top;
    t_parag_gap.i4_line_gap_bottom = pt_attr->i4_line_gap_bottom;
    CATCH_FAIL(wgl_htmgr_set_parag_gap(&pt_ctx->t_htmgr, &t_parag_gap));

    return WGLR_OK;    
}    

static INT32 _hts_tool_send_notify(
    IN      HANDLE_T                    h_hts,
    IN      UINT32                      ui4_notify_code)
{
    INT32           i4_ret;
    HANDLE_T        h_parent;
    
    CATCH_FAIL(x_wgl_get_parent(h_hts, &h_parent));
    if(h_parent)
    {
        WGL_NOTIFY_DATA_T       t_notify;
        x_memset(&t_notify, 0, sizeof(WGL_NOTIFY_DATA_T));
        t_notify.ui4_nc_code    = ui4_notify_code;
        INFORM_ERR(x_wgl_send_notify(h_parent, t_notify.ui4_nc_code, h_hts, &t_notify));
    }
    
    return WGLR_OK;
}        

static INT32 _hts_tool_check_image_handle(
    IN      WGL_HIMG_TPL_T              h_img)
{
    if(h_img != NULL_HANDLE)
    {
        UINT32  ui4_w_temp, ui4_h_temp;
        return x_wgl_img_tpl_get_size(h_img, &ui4_w_temp, &ui4_h_temp);
    }
    
    return WGLR_OK;
}        

static INT32 _hts_tool_check_hts_scrn_location(
    IN OUT  HANDLE_T                    h_hts)
{
    HTS_CTX_T                           *pt_ctx = NULL;
    GL_RECT_T                           t_rc_scrn_hts_new;
    GL_RECT_T                           t_rc_scrn_hts_orig;
    INT32                               i4_ret;
    
    CATCH_FAIL(x_wgl_get_coords(h_hts, WGL_COORDS_SCREEN, &t_rc_scrn_hts_new));
    
    GET_LOCK(h_hts, &pt_ctx, i4_ret);
    DO_HTS_CNT_CMD(pt_ctx, WGL_CNT_HTS_GET_HTS_SCRN_RECT, &t_rc_scrn_hts_orig, NULL, i4_ret);
    CATCH_FAIL_UNLOCK(h_hts, pt_ctx, i4_ret);
    RELEASE_LOCK(h_hts, pt_ctx, i4_ret);
    
    if(!WGL_RECT_IS_EQUAL(&t_rc_scrn_hts_orig, &t_rc_scrn_hts_new))
    {
        CATCH_FAIL(_hts_evt_hdr_on_change_vp_size(h_hts, FALSE));
    }
    
    return WGLR_OK;
}    

