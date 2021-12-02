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
 * $RCSfile: wgl_bdr_uniform.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 71baf27228f4384b375d6d38f5aa67b3 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "unicode/x_uc_str.h"
#include "wgl/wgl_tools.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/x_wgl_common.h"
#include "wgl/wgl_string.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/bdr_uniform/u_wgl_bdr_uniform.h"
#include "wgl/bdr_uniform/wgl_bdr_uniform.h"
#include "wgl/arabic.h"

/*-----------------------------------------------------------------------------
                    constants
 ----------------------------------------------------------------------------*/
static const WGL_INSET_T BDRUNFM_DEF_INSET = 
{
    5, 5, 5, 5
};

static const WGL_INSET_T BDRUNFM_MIN_INSET = 
{
    0, 0, 0, 0  
};

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "BDRUFM"

#define IN
#define OUT

#define TTL_STR_INIT_BUF_SIZE       30
#define TTL_STR_BUF_INC_SIZE        20

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _BDR_UFM_CTX_T
{
#ifdef CLI_LVL_ALL     
    UINT32                     ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    BOOL                        b_is_draw_intr;
    GL_RECT_T                   t_rc_widget;
    WGL_INSET_T                 t_inset_bdr;
    WGL_BDR_UNFM_MARGIN_T       t_margin;
    INT32                       i4_thickness;
    WGL_CANVAS_STATUS_T         t_cvsst;
    
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_fg;
    WGL_COLOR_EXT_SET_T         t_clrs_fg;
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_bk;
    WGL_COLOR_EXT_SET_T         t_clrs_bk;
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_inner_bk;
    WGL_COLOR_EXT_SET_T         t_clrs_inner_bk;
    
    /* title info */
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_ttl;
    WGL_COLOR_EXT_SET_T         t_clrs_ttl;
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_ttl_bk;
    WGL_COLOR_EXT_SET_T         t_clrs_ttl_bk;
    WGL_BDR_UNFM_TITLE_INFO_T   t_info_ttl;
    WGL_STRING_T                t_str_ttl;
    UINT8                       ui1_text_direction;
} BDR_UFM_CTX_T;



/*-----------------------------------------------------------------------------
                    internal functions declarations
 ----------------------------------------------------------------------------*/
static VOID _bdrunfm_tool_select_color(
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_src,
    IN      UINT16                      ui2_state, 
    OUT     GL_COLOR_T                  **ppt_clr_out);
static INT32 _bdrunfm_tool_set_inset(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      WGL_INSET_T                 *pt_inset);
static INT32 _bdrunfm_tool_set_color(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_target, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info);
static VOID _bdrunfm_copy_from_color_info_to_extend_color_set(
    OUT     WGL_COLOR_EXT_SET_T         *pt_clrs_ex,
    IN      WGL_COLOR_INFO_T            *pt_clr_info_from);
static INT32 _bdrunfm_tool_get_color(
    IN      BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_target, 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out);
static INT32 _bdrunfm_tool_set_text_direction(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_text_dir);
static INT32 _bdrunfm_tool_get_text_direction(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    OUT      UINT8                      *pui1_text_dir);
static INT32 _bdrunfm_cmd_hdr_set_bdr_title_info(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _bdrunfm_cmd_hdr_get_bdr_title_info(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _bdrunfm_cmd_hdr_set_bdr_title_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
static INT32 _bdrunfm_cmd_hdr_get_bdr_title_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
static INT32 _bdrunfm_cmd_hdr_set_thickness(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _bdrunfm_cmd_hdr_get_thickness(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1);
static INT32 _bdrunfm_cmd_hdr_set_margin(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1);
static INT32 _bdrunfm_cmd_hdr_get_margin(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1);
static VOID _bdrunfm_copy_from_extend_color_set_to_color_info(
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out, 
    IN      WGL_COLOR_SET_TYPE_T        e_clrs_type,
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_ex_from);
static VOID _bdrunfm_load_sys_def_clrs_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx);
static VOID _bdrunfm_load_sys_def_clrs_fg(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx);
static VOID _bdrunfm_load_def_clrs_inner_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx);
static VOID _bdrunfm_load_def_clrs_ttl_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx);
static VOID _bdrunfm_load_def_clrs_ttl_text_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx);
static INT32 _bdrunfm_tool_fill_border_area(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt);
static INT32 _bdrunfm_tool_draw_border(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_cnt,
    IN      GL_RECT_T                   *pt_rc_upt);
static INT32 _bdrunfm_tool_fill_inner_area(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_cnt,
    IN      GL_RECT_T                   *pt_rc_upt);
static INT32 _bdrunfm_tool_draw_title(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt);
static VOID _bdrunfm_tool_cal_obj_pos( 
    IN      UINT8                       ui1_text_dir,
    IN      UINT8                       ui1_align,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      GL_RECT_T                   *pt_rc_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y);



/*-----------------------------------------------------------------------------
                    exported functions 
 ----------------------------------------------------------------------------*/
INT32 x_wgl_bdr_uniform_init(VOID)
{
    WGL_BORDER_CTRL_INTF_T      t_bdr_intf = { wgl_bdr_uniform_inst_create,
                                               wgl_bdr_uniform_inst_destroy,
                                               wgl_bdr_uniform_cmd_proc,
                                               wgl_bdr_uniform_paint
                                             };
    
    return wgl_register_border_intf(WGL_BORDER_UNIFORM, &t_bdr_intf);
}


/*-----------------------------------------------------------------------------
                    public functions 
 ----------------------------------------------------------------------------*/
INT32 wgl_bdr_uniform_inst_create(
    WGL_BORDER_CONTEXT_HANDLE_T         *ppv_ctx_bdr, 
    WGL_BORDER_CREATE_INPUT_T           *pt_bdr_create)
{
    BDR_UFM_CTX_T *pt_ctx = NULL;
    
    *ppv_ctx_bdr = NULL;

    pt_ctx = (BDR_UFM_CTX_T *)WGL_MEM_ALLOC(sizeof(BDR_UFM_CTX_T));
    if(pt_ctx==NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL   
    pt_ctx->ui4_mem_size = sizeof(BDR_UFM_CTX_T);
#endif   

    pt_ctx->b_is_draw_intr = HAS_FLAG(pt_bdr_create->ui4_style, WGL_STL_GL_BDR_FILL_CNT_BK) ? TRUE : FALSE;
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, pt_bdr_create->i4_width, pt_bdr_create->i4_height);
    WGL_COPY_INSET(&pt_ctx->t_inset_bdr, &BDRUNFM_DEF_INSET);
    x_memset(&pt_ctx->t_margin, 0, sizeof(WGL_BDR_UNFM_MARGIN_T));
    pt_ctx->i4_thickness = 0;
    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_bdr_create->pt_cvsst);
    _bdrunfm_load_sys_def_clrs_bk(pt_ctx);
    _bdrunfm_load_sys_def_clrs_fg(pt_ctx);
    _bdrunfm_load_def_clrs_inner_bk(pt_ctx);
    
    /*init title info*/ 
    _bdrunfm_load_def_clrs_ttl_text(pt_ctx);
    _bdrunfm_load_def_clrs_ttl_text_bk(pt_ctx);
    pt_ctx->t_info_ttl.ui2_style = 0;
    x_wgl_get_system_font(&pt_ctx->t_info_ttl.t_font);
    WGL_RECT_EMPTY(&pt_ctx->t_info_ttl.t_rc_ref);
    pt_ctx->t_info_ttl.ui1_align = WGL_AS_CENTER_CENTER;
    x_memset(&pt_ctx->t_str_ttl, 0, sizeof(WGL_STRING_T));
    pt_ctx->ui1_text_direction = WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT;
    
    *ppv_ctx_bdr = (WGL_BORDER_CONTEXT_HANDLE_T)pt_ctx;
    return WGLR_OK;
}

INT32 wgl_bdr_uniform_inst_destroy(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr)
{
    HFONT_T         h_font;
    BDR_UFM_CTX_T   *pt_ctx = (BDR_UFM_CTX_T *)pv_ctx_bdr;
    INT32           i4_ret;
    
    h_font = WGL_STR_GET_FONT(&pt_ctx->t_str_ttl);
    if(h_font!=NULL_HANDLE)
    {
        WGL_FONT_FREE(h_font);
        INFORM_ERR(wgl_str_destroy(&pt_ctx->t_str_ttl));
    }

    WGL_MEM_FREE(pt_ctx);

    return WGLR_OK;
}



INT32 wgl_bdr_uniform_paint(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    at_upt_rect,
    UINT16                              ui2_state)
{
    BDR_UFM_CTX_T   *pt_ctx = (BDR_UFM_CTX_T *)pv_ctx_bdr;
    INT32           i4_ret;
    GL_RECT_T       t_rc_cnt, t_rc_upt;
    
    if(!IS_STATE_VISIBLE(ui2_state) ||
       !RECT_IS_EFFECTIVE(pt_ctx->t_rc_widget))    
    {
        return WGLR_OK;
    }
    
    /* fill the background of uniform border */
    if(pt_ctx->b_is_draw_intr)
    {
        GL_COLOR_T      *pt_clr_bk;
        _bdrunfm_tool_select_color(&pt_ctx->t_clrs_bk, ui2_state, &pt_clr_bk);
        INFORM_ERR(wgl_fill_widget_bk(h_g, pt_clr_bk, &pt_ctx->t_rc_widget, &pt_ctx->t_cvsst));
    }
    
    /* check inset for quick return */
    WGL_RECT_COPY(&t_rc_cnt, &pt_ctx->t_rc_widget);
    _RECT_INSET(&t_rc_cnt, &pt_ctx->t_inset_bdr);
    if(!RECT_IS_EFFECTIVE(t_rc_cnt))
    {
        return WGLR_OK;
    }
    
    /* prepare the update rect */
    if(RECT_IS_EFFECTIVE(at_upt_rect[0]))        
    {
        WGL_RECT_COPY(&t_rc_upt, &at_upt_rect[0]);
    }
    else
    {
        WGL_RECT_COPY(&t_rc_upt, &pt_ctx->t_rc_widget);
    }    
    
    /* check for quick return */
    if(WGL_RECT_IS_INCLUDED(&t_rc_cnt, &t_rc_upt))
    {
        return WGLR_OK;
    }
    
    /* fill the border area with background color */
    if(!pt_ctx->b_is_draw_intr)
    {
        INFORM_ERR(_bdrunfm_tool_fill_border_area(pt_ctx, h_g, ui2_state, &t_rc_upt));
    }
    
    /* draw the border */
    if(pt_ctx->i4_thickness > 0)
    {
        INFORM_ERR(_bdrunfm_tool_draw_border(pt_ctx, h_g, ui2_state, &t_rc_cnt, &t_rc_upt));
    }
    
    /* fill the inner area within the border */
    if(!( pt_ctx->t_margin.i4_left==0     &&
          pt_ctx->t_margin.i4_top==0      &&
          pt_ctx->t_margin.i4_right==0    &&
          pt_ctx->t_margin.i4_bottom==0 ))
    {
        INFORM_ERR(_bdrunfm_tool_fill_inner_area(pt_ctx, h_g, ui2_state, &t_rc_cnt, &t_rc_upt));
    }
    
    /* draw the title */
    if(WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl) > 0)
    {
        CHECK_VALID(WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)!=NULL_HANDLE, WGLR_INTERNAL_ERROR);
        INFORM_ERR(_bdrunfm_tool_draw_title(pt_ctx, h_g, ui2_state, &t_rc_upt));
    }
    
    return WGLR_OK;
}    

INT32 wgl_bdr_uniform_cmd_proc(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    UINT32                              ui4_cmd_id, 
    VOID                                *pv_param1, 
    VOID                                *pv_param2)
{
    BDR_UFM_CTX_T   *pt_ctx = (BDR_UFM_CTX_T *)pv_ctx_bdr;
    INT32           i4_ret, i4_ret_val;

    if(!(WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_BASE, ui4_cmd_id) ||
         WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_UNIFORM, ui4_cmd_id)) )
    {
        INFORM_ERR(WGLR_INV_CMD);
        return WGLR_INV_CMD;
    }
    
    i4_ret_val = WGLR_OK;

    switch(ui4_cmd_id) 
    {
        /*Internal Border Commands*/
        case WGL_BDR_GL_SET_SIZE:
        {
            INT32       i4_size_w = (INT32)pv_param1;
            INT32       i4_size_h = (INT32)pv_param2;
            SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, i4_size_w, i4_size_h);
            break;
        }

        /*Common Border Commands*/
        case WGL_CMD_GL_SET_INSET:
        {
            i4_ret_val = _bdrunfm_tool_set_inset(pt_ctx, (WGL_INSET_T *)pv_param1);
            break;
        }
        
        case WGL_CMD_GL_GET_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _INSET_COPY(pv_param1, &pt_ctx->t_inset_bdr);
            break;
        }

        case WGL_CMD_GL_GET_MIN_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            _INSET_COPY(pv_param1, &BDRUNFM_MIN_INSET);
            break;
        }
    
        case WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR:
        {
            BOOL b_is_draw_intr = WGL_POINTER_TO_BOOL(pv_param1);
            if(!_IS_VALID_BOOL(b_is_draw_intr))
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
            pt_ctx->b_is_draw_intr = b_is_draw_intr;
            break;
        }

        case WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR:
        {
            *((BOOL *)pv_param1) = pt_ctx->b_is_draw_intr;
            break;
        }

        case WGL_CMD_GL_SET_BDR_COLOR: /*pv_param1: (UINT8)target, pv_param2:(WGL_COLOR_INFO_T *)*pt_color_info*/
        {
            i4_ret_val = _bdrunfm_tool_set_color(pt_ctx, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T *)pv_param2);
            break;
        }

        case WGL_CMD_GL_GET_BDR_COLOR:
        {
            CHECK_NULL(pv_param2, WGLR_INV_ARG);
            i4_ret_val = _bdrunfm_tool_get_color(pt_ctx, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T *)pv_param2);
            break;
        }
        case WGL_CMD_GL_SET_BDR_TEXT_DIRECTION:
        {
            i4_ret_val = _bdrunfm_tool_set_text_direction(pt_ctx, WGL_POINTER_TO_UINT8(pv_param1));
            break;
        }
        case WGL_CMD_GL_GET_BDR_TEXT_DIRECTION:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _bdrunfm_tool_get_text_direction(pt_ctx, (UINT8*)pv_param1);
            break;
        }
        case WGL_CMD_BDR_UNFM_SET_TITLE_INFO:
            i4_ret_val = _bdrunfm_cmd_hdr_set_bdr_title_info(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_BDR_UNFM_GET_TITLE_INFO:
            i4_ret_val = _bdrunfm_cmd_hdr_get_bdr_title_info(pt_ctx, pv_param1);
        break;
        
        case WGL_CMD_BDR_UNFM_SET_TITLE_TEXT:
            i4_ret_val = _bdrunfm_cmd_hdr_set_bdr_title_text(pt_ctx, pv_param1, pv_param2);
        break;
        
        case WGL_CMD_BDR_UNFM_GET_TITLE_TEXT:
            i4_ret_val = _bdrunfm_cmd_hdr_get_bdr_title_text(pt_ctx, pv_param1, pv_param2);
        break;
        
        /*Specific Border Commands*/
        case WGL_CMD_BDR_UNFM_SET_THICKNESS:
            i4_ret_val = _bdrunfm_cmd_hdr_set_thickness(pt_ctx, pv_param1);
        break;        
        
        case WGL_CMD_BDR_UNFM_GET_THICKNESS:
            i4_ret_val = _bdrunfm_cmd_hdr_get_thickness(pt_ctx, pv_param1);
        break;        

        case WGL_CMD_BDR_UNFM_SET_MARGIN:
            i4_ret_val = _bdrunfm_cmd_hdr_set_margin(pt_ctx, pv_param1);
        break;        
        
        case WGL_CMD_BDR_UNFM_GET_MARGIN:
            i4_ret_val = _bdrunfm_cmd_hdr_get_margin(pt_ctx, pv_param1);
        break;        

        default:
            INFORM_ERR(WGLR_INV_CMD);
            return WGLR_INV_CMD;
    }


    return i4_ret_val;
}




/*-----------------------------------------------------------------------------
                    Internal functions definitions
 ----------------------------------------------------------------------------*/

static VOID _bdrunfm_tool_select_color(
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_src,
    IN      UINT16                      ui2_state, 
    OUT     GL_COLOR_T                  **ppt_clr_out)
{

    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            *ppt_clr_out = &pt_clrs_src->t_push;
        }
        else
        {
            *ppt_clr_out = &pt_clrs_src->t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            *ppt_clr_out = &pt_clrs_src->t_highlight;
        }
        else
        {
            *ppt_clr_out = &pt_clrs_src->t_highlight_inactive;
        }
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        *ppt_clr_out = &pt_clrs_src->t_enable;
    }
    else
    {
        *ppt_clr_out = &pt_clrs_src->t_disable;
    }

}



static INT32 _bdrunfm_tool_set_inset(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      WGL_INSET_T                 *pt_inset)
{

    if(pt_inset==NULL)
    {
        _INSET_COPY(&pt_ctx->t_inset_bdr, &BDRUNFM_DEF_INSET);
    }
    else
    {
        CHECK_VALID(_INSET_IS_VALID(*pt_inset), WGLR_INV_ARG);
        _INSET_COPY(&pt_ctx->t_inset_bdr, pt_inset);
    }

    return WGLR_OK;
}



static INT32 _bdrunfm_tool_set_color(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_target, 
    IN      WGL_COLOR_INFO_T            *pt_clr_info)
{
    INT32   i4_ret_val, i4_ret;
    
    CHECK_VALID(
        pt_clr_info==NULL   ||
        IS_LEGAL_CLR_SET_TYPE(pt_clr_info->e_type), WGLR_INV_ARG);
    
    i4_ret_val = WGLR_OK;
    switch(ui1_target)
    {
        case WGL_CLR_BK:
        case WGL_CLR_BDR_BK:
        {
            if(pt_clr_info==NULL)
            { /*load system defaults*/
                _bdrunfm_load_sys_def_clrs_bk(pt_ctx);
            }
            else
            {
                pt_ctx->e_clrs_type_src_bk = pt_clr_info->e_type;
                _bdrunfm_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_bk, pt_clr_info);
            }
        }
        break;
        
        case WGL_CLR_BDR_FG:
        {
            if(pt_clr_info==NULL)
            {
                _bdrunfm_load_sys_def_clrs_fg(pt_ctx);
            }
            else
            {
                pt_ctx->e_clrs_type_src_fg = pt_clr_info->e_type;
                _bdrunfm_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_fg, pt_clr_info);
            }
        }
        break;
        
        case WGL_CLR_BDR_UNFM_INNER_BK:
        {
            if(pt_clr_info==NULL)
            {
                _bdrunfm_load_def_clrs_inner_bk(pt_ctx);                
            }
            else
            {
                pt_ctx->e_clrs_type_src_inner_bk = pt_clr_info->e_type;
                _bdrunfm_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_inner_bk, pt_clr_info);
            }
        }
        break;
        
        case WGL_CLR_BDR_UNFM_TITLE_TEXT:
        {
            if(pt_clr_info==NULL)
            {
                _bdrunfm_load_def_clrs_ttl_text(pt_ctx);
            }
            else
            {
                pt_ctx->e_clrs_type_src_ttl = pt_clr_info->e_type;
                _bdrunfm_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_ttl, pt_clr_info);
            }
        }
        break;
        
        case WGL_CLR_BDR_UNFM_TITLE_TEXT_BK:
        {
            if(pt_clr_info==NULL)
            {
                _bdrunfm_load_def_clrs_ttl_text_bk(pt_ctx);
            }
            else
            {
                pt_ctx->e_clrs_type_src_ttl_bk = pt_clr_info->e_type;
                _bdrunfm_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_ttl_bk, pt_clr_info);
            }
        }
        break;
    
        default:
            INFORM_ERR(WGLR_INV_ARG);
            i4_ret_val = WGLR_INV_ARG;        
        
    }
        

    return i4_ret_val;

}


static VOID _bdrunfm_copy_from_color_info_to_extend_color_set(
    OUT     WGL_COLOR_EXT_SET_T         *pt_clrs_ex,
    IN      WGL_COLOR_INFO_T            *pt_clr_info_from)
{
    
    switch(pt_clr_info_from->e_type) 
    {
        case WGL_COLOR_SET_BASIC:
            _COLOR_COPY(&pt_clrs_ex->t_disable, &pt_clr_info_from->u_color_data.t_basic.t_disable);
            _COLOR_COPY(&pt_clrs_ex->t_enable, &pt_clr_info_from->u_color_data.t_basic.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_highlight, &pt_clr_info_from->u_color_data.t_basic.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_highlight_inactive, &pt_clr_info_from->u_color_data.t_basic.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_push, &pt_clr_info_from->u_color_data.t_basic.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_push_unhighlight, &pt_clr_info_from->u_color_data.t_basic.t_enable);
            break;
        case WGL_COLOR_SET_STANDARD:
            _COLOR_COPY(&pt_clrs_ex->t_disable, &pt_clr_info_from->u_color_data.t_standard.t_disable);
            _COLOR_COPY(&pt_clrs_ex->t_enable, &pt_clr_info_from->u_color_data.t_standard.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_highlight, &pt_clr_info_from->u_color_data.t_standard.t_highlight);
            _COLOR_COPY(&pt_clrs_ex->t_highlight_inactive, &pt_clr_info_from->u_color_data.t_standard.t_enable);
            _COLOR_COPY(&pt_clrs_ex->t_push, &pt_clr_info_from->u_color_data.t_standard.t_highlight);
            _COLOR_COPY(&pt_clrs_ex->t_push_unhighlight, &pt_clr_info_from->u_color_data.t_standard.t_enable);
            break;
        case WGL_COLOR_SET_EXTEND:
            x_memcpy(pt_clrs_ex, &pt_clr_info_from->u_color_data.t_extend, sizeof(WGL_COLOR_EXT_SET_T));
            break;
    }
    
}


static INT32 _bdrunfm_tool_get_color(
    IN      BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_target, 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out)
{
    WGL_COLOR_SET_TYPE_T e_clrs_type;
    WGL_COLOR_EXT_SET_T *pt_clrs_ex_from;

    if(ui1_target==WGL_CLR_BK || ui1_target==WGL_CLR_BDR_BK)
    {
        e_clrs_type = pt_ctx->e_clrs_type_src_bk;
        pt_clrs_ex_from = &pt_ctx->t_clrs_bk;
    }
    else if(ui1_target==WGL_CLR_BDR_FG)
    {
        e_clrs_type = pt_ctx->e_clrs_type_src_fg;
        pt_clrs_ex_from = &pt_ctx->t_clrs_fg;
    }
    else
    {
        return WGLR_INV_ARG;
    }

    _bdrunfm_copy_from_extend_color_set_to_color_info(pt_clr_info_out, e_clrs_type, pt_clrs_ex_from);

    return WGLR_OK;

}


static INT32 _bdrunfm_tool_set_text_direction(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_text_dir 
    )
{
    if(ui1_text_dir != WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT &&  ui1_text_dir != WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT )
    {
        return WGLR_INV_ARG;
    }
    pt_ctx->ui1_text_direction = ui1_text_dir;

    return WGLR_OK;
}

static INT32 _bdrunfm_tool_get_text_direction(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx, 
    OUT      UINT8*                       pui1_text_dir 
    )
{
    (*pui1_text_dir) = pt_ctx->ui1_text_direction;
    return WGLR_OK;
}

static INT32 _bdrunfm_cmd_hdr_set_bdr_title_info(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1)
{    
    WGL_BDR_UNFM_TITLE_INFO_T   *pt_info = (WGL_BDR_UNFM_TITLE_INFO_T *)pv_param1;
    WGL_BDR_UNFM_TITLE_INFO_T   t_info;
    HFONT_T                     h_font;
    INT32                       i4_ret;
    
    CHECK_NULL(pt_info, WGLR_INV_ARG);
    CHECK_VALID(
        !(pt_info->ui2_style & (~((WGL_BDR_UNFM_TITLE_STL_LAST_STYLE<<1)-1))) &&
        pt_info->t_rc_ref.i4_left <= pt_info->t_rc_ref.i4_right               &&
        pt_info->t_rc_ref.i4_top  <= pt_info->t_rc_ref.i4_bottom              && 
        WGL_IS_VALID_ALIGN(pt_info->ui1_align), 
        WGLR_INV_ARG);
        
    x_memcpy(&t_info, pt_info, sizeof(WGL_BDR_UNFM_TITLE_INFO_T));
    if(x_strlen(t_info.t_font.a_c_font_name)==0)
    {
        x_strcpy(t_info.t_font.a_c_font_name, SN_FONT_DEFAULT);
    }        
    
    if( WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)==NULL_HANDLE ||
       !wgl_str_check_font(&pt_ctx->t_str_ttl, &t_info.t_font))
    {
        /* the different font */
        WGL_FONT_CREATE(&t_info.t_font, &h_font);
        CHECK_VALID(h_font != NULL_HANDLE, WGLR_INV_ARG);
            
        if (t_info.t_font.i2_width != 0)
        {
            INFORM_ERR (x_fe_set_fixed_advance (h_font, t_info.t_font.i2_width, 0));
        }
            
        if(WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)==NULL_HANDLE)
        {
            /* not init yet */
            CATCH_FAIL(wgl_str_init(&pt_ctx->t_str_ttl, TTL_STR_INIT_BUF_SIZE, TTL_STR_BUF_INC_SIZE, h_font));
        }
        else
        {
            /* replace original font */
            WGL_FONT_FREE(WGL_STR_GET_FONT(&pt_ctx->t_str_ttl));
            CATCH_FAIL(wgl_str_set_font(&pt_ctx->t_str_ttl, h_font));
        }
    }
    
    x_memcpy(&pt_ctx->t_info_ttl, &t_info, sizeof(WGL_BDR_UNFM_TITLE_INFO_T));
    
    return WGLR_OK;
}

static INT32 _bdrunfm_cmd_hdr_get_bdr_title_info(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_BDR_UNFM_TITLE_INFO_T    *pt_info_out = (WGL_BDR_UNFM_TITLE_INFO_T *)pv_param1;
    CHECK_NULL(pt_info_out, WGLR_INV_ARG);
    
    x_memcpy(pt_info_out, &pt_ctx->t_info_ttl, sizeof(WGL_BDR_UNFM_TITLE_INFO_T));
    
    return WGLR_OK;
}    

static INT32 _bdrunfm_cmd_hdr_set_bdr_title_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UTF16_T     *w2s_ttl_text = (UTF16_T *)pv_param1;
    UINT16      ui2_len_text  = WGL_POINTER_TO_UINT16(pv_param2);
    INT32       i4_ret;
    
    CHECK_VALID(
        w2s_ttl_text!=NULL ||
        (w2s_ttl_text==NULL && ui2_len_text==0), WGLR_INV_ARG);
        
    if(WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)==NULL_HANDLE)
    {
        HFONT_T             h_font;
        
        /* not init yet */
        WGL_FONT_CREATE(&pt_ctx->t_info_ttl.t_font, &h_font);
        CHECK_VALID(h_font != NULL_HANDLE, WGLR_INTERNAL_ERROR);
        
        if (pt_ctx->t_info_ttl.t_font.i2_width != 0)
        {
            INFORM_ERR (x_fe_set_fixed_advance (h_font, pt_ctx->t_info_ttl.t_font.i2_width, 0));
        }
        
        CATCH_FAIL(wgl_str_init(&pt_ctx->t_str_ttl, TTL_STR_INIT_BUF_SIZE, TTL_STR_BUF_INC_SIZE, h_font));
    }
        
    if(w2s_ttl_text==NULL)
    {
        UTF16_T     w2s_temp[5];
        CATCH_FAIL(wgl_str_set_text(&pt_ctx->t_str_ttl, w2s_temp, 0));
    }
    else
    {
        CATCH_FAIL(wgl_str_set_text(&pt_ctx->t_str_ttl, w2s_ttl_text, ui2_len_text));
    }
    
    return WGLR_OK;
}        

static INT32 _bdrunfm_cmd_hdr_get_bdr_title_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UTF16_T     *w2s_ttl_text_out = (UTF16_T *)pv_param1;
    UINT16      ui2_size_buf      = WGL_POINTER_TO_UINT16(pv_param2);
    
    CHECK_NULL(w2s_ttl_text_out, WGLR_INV_ARG);
    CHECK_VALID(ui2_size_buf > WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl), WGLR_BUFFER_NOT_ENOUGH);
    
    x_uc_w2s_strncpy(w2s_ttl_text_out, WGL_STR_GET_TEXT(&pt_ctx->t_str_ttl), WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl));
    x_uc_w2s_set(w2s_ttl_text_out, WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl), (UTF16_T)0);
    
    return WGLR_OK;
}        

static INT32 _bdrunfm_cmd_hdr_set_thickness(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT16  ui2_thickness;
 
    CHECK_VALID((INT32)pv_param1>=0, WGLR_INV_ARG);
    ui2_thickness = WGL_POINTER_TO_UINT16(pv_param1);
    pt_ctx->i4_thickness = (INT32)ui2_thickness;
    return WGLR_OK;   
}    

static INT32 _bdrunfm_cmd_hdr_get_thickness(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16  *pui2_thickness_out = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_thickness_out, WGLR_INV_ARG);
    *pui2_thickness_out = (UINT16)pt_ctx->i4_thickness;
 
    return WGLR_OK;   
}    

static INT32 _bdrunfm_cmd_hdr_set_margin(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_BDR_UNFM_MARGIN_T *pt_margin = (WGL_BDR_UNFM_MARGIN_T *)pv_param1;
    
    CHECK_NULL(pt_margin, WGLR_INV_ARG);
    x_memcpy(&pt_ctx->t_margin, pt_margin, sizeof(WGL_BDR_UNFM_MARGIN_T));
    return WGLR_OK;   
}    

static INT32 _bdrunfm_cmd_hdr_get_margin(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_BDR_UNFM_MARGIN_T *pt_margin_out = (WGL_BDR_UNFM_MARGIN_T *)pv_param1;
    
    CHECK_NULL(pt_margin_out, WGLR_INV_ARG);
    x_memcpy(pt_margin_out, &pt_ctx->t_margin, sizeof(WGL_BDR_UNFM_MARGIN_T));
    return WGLR_OK;   
}    

static VOID _bdrunfm_copy_from_extend_color_set_to_color_info(
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out, 
    IN      WGL_COLOR_SET_TYPE_T        e_clrs_type,
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_ex_from)
{
    pt_clr_info_out->e_type = e_clrs_type;
    switch(e_clrs_type) {
        case WGL_COLOR_SET_BASIC:
            _COLOR_COPY(&pt_clr_info_out->u_color_data.t_basic.t_disable, &pt_clrs_ex_from->t_disable);
            _COLOR_COPY(&pt_clr_info_out->u_color_data.t_basic.t_enable, &pt_clrs_ex_from->t_enable);
            break;
        case WGL_COLOR_SET_STANDARD:
            _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_disable, &pt_clrs_ex_from->t_disable);
            _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_enable, &pt_clrs_ex_from->t_enable);
            _COLOR_COPY(&pt_clr_info_out->u_color_data.t_standard.t_highlight, &pt_clrs_ex_from->t_highlight);
            break;
        case WGL_COLOR_SET_EXTEND:
            x_memcpy((VOID *)&pt_clr_info_out->u_color_data.t_extend, (VOID *)pt_clrs_ex_from, sizeof(WGL_COLOR_EXT_SET_T));
            break;
    }
    
}

static VOID _bdrunfm_load_sys_def_clrs_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx)
{
    pt_ctx->e_clrs_type_src_bk = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_push_unhighlight);
}

static VOID _bdrunfm_load_sys_def_clrs_fg(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx)
{
    pt_ctx->e_clrs_type_src_fg = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_push_unhighlight);
}
static VOID _bdrunfm_load_def_clrs_inner_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx)
{
    pt_ctx->e_clrs_type_src_inner_bk = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_inner_bk.t_push_unhighlight);
}

static VOID _bdrunfm_load_def_clrs_ttl_text(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx)
{
    pt_ctx->e_clrs_type_src_ttl = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_push_unhighlight);
}    

static VOID _bdrunfm_load_def_clrs_ttl_text_bk(
    IN OUT  BDR_UFM_CTX_T               *pt_ctx)
{
    pt_ctx->e_clrs_type_src_ttl_bk = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_push_unhighlight);
}

static INT32 _bdrunfm_tool_fill_border_area(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt)
{
    GL_COLOR_T      *pt_clr_bk;
    INT32           i4_ret;
    GL_RECT_T       t_rc_temp;
    
    _bdrunfm_tool_select_color(&pt_ctx->t_clrs_bk, ui2_state, &pt_clr_bk);

    if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_bk))
    {
        return WGLR_OK;
    }
    
    i4_ret = x_gl_set_color(h_g, pt_clr_bk);
    if(i4_ret!=GLR_OK)
    {
        INFORM_ERR(i4_ret);
        return WGLR_OK;
    }
    
    /* left */
    SET_RECT(&t_rc_temp, 0, 0, pt_ctx->t_inset_bdr.i4_left, pt_ctx->t_rc_widget.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* top */
    SET_RECT(&t_rc_temp, 0, 0, pt_ctx->t_rc_widget.i4_right, pt_ctx->t_inset_bdr.i4_top);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* right */
    SET_RECT(&t_rc_temp, pt_ctx->t_rc_widget.i4_right-pt_ctx->t_inset_bdr.i4_right, 0, pt_ctx->t_rc_widget.i4_right, pt_ctx->t_rc_widget.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* bottom */
    SET_RECT(&t_rc_temp, 0, pt_ctx->t_rc_widget.i4_bottom - pt_ctx->t_inset_bdr.i4_bottom, pt_ctx->t_rc_widget.i4_right, pt_ctx->t_rc_widget.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
 
    return WGLR_OK;   
}        

static INT32 _bdrunfm_tool_draw_border(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_cnt,
    IN      GL_RECT_T                   *pt_rc_upt)
{
    INT32       i4_ret;
    GL_RECT_T   t_rc_temp;
    GL_COLOR_T  *pt_clr_fg;
    
    _bdrunfm_tool_select_color(&pt_ctx->t_clrs_fg, ui2_state, &pt_clr_fg);

    if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_fg))
    {
        return WGLR_OK;
    }
    
    i4_ret = x_gl_set_color(h_g, pt_clr_fg);
    if(i4_ret!=GLR_OK)
    {
        INFORM_ERR(i4_ret);
        return WGLR_OK;
    }
    
    /* left */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom + pt_ctx->i4_thickness);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* top */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right + pt_ctx->i4_thickness,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* right */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right + pt_ctx->i4_thickness,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom + pt_ctx->i4_thickness);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* bottom */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left - pt_ctx->i4_thickness,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right + pt_ctx->i4_thickness,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom + pt_ctx->i4_thickness);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    return WGLR_OK;   
}

static INT32 _bdrunfm_tool_fill_inner_area(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_cnt,
    IN      GL_RECT_T                   *pt_rc_upt)
{
    INT32       i4_ret;
    GL_RECT_T   t_rc_temp;
    GL_COLOR_T  *pt_clr_inner;
    
    _bdrunfm_tool_select_color(&pt_ctx->t_clrs_inner_bk, ui2_state, &pt_clr_inner);

    if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_inner))
    {
        return WGLR_OK;
    }
    
    i4_ret = x_gl_set_color(h_g, pt_clr_inner);
    if(i4_ret!=GLR_OK)
    {
        INFORM_ERR(i4_ret);
        return WGLR_OK;
    }
    
    /* left */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top,
        pt_rc_cnt->i4_left,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* top */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right,
        pt_rc_cnt->i4_top);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* right */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_right,
        pt_rc_cnt->i4_top - pt_ctx->t_margin.i4_top,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }

    /* bottom */
    SET_RECT(
        &t_rc_temp, 
        pt_rc_cnt->i4_left - pt_ctx->t_margin.i4_left,
        pt_rc_cnt->i4_bottom,
        pt_rc_cnt->i4_right + pt_ctx->t_margin.i4_right,
        pt_rc_cnt->i4_bottom + pt_ctx->t_margin.i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
   
    return WGLR_OK;   
}

static INT32 _bdrunfm_tool_draw_title(
    IN      BDR_UFM_CTX_T               *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt)
{
    INT32           i4_ret;
    GL_COLOR_T      *pt_clr_ttl, *pt_clr_ttl_bk;
    GL_COLOR_T      t_clr_clean;
    INT32           i4_x, i4_y, i4_w, i4_h, i4_bg_h;
    BOOL            b_need_update, b_draw_str;
    GL_RECT_T       t_rc_ttl;
	HANDLE_T        h_font = NULL_HANDLE;
    #ifdef ARAB_EXIST_SUPPORT
	BOOL            b_exist_arab = FALSE;
	UTF16_T*        w2s_arabic_text = NULL;
	UINT16          ui2_arabic_len  = 0;
	
	b_exist_arab = check_exist_arabic(pt_ctx->t_str_ttl.w2s_text,pt_ctx->t_str_ttl.ui2_len_text);
	if (b_exist_arab)
	{
		w2s_arabic_text = (UTF16_T*)WGL_MEM_ALLOC((pt_ctx->t_str_ttl.ui2_len_text + 1)*sizeof(UTF16_T));
		if (w2s_arabic_text  == NULL)
		{
		    return WGLR_OUT_OF_MEMORY;
		}
		x_memset(w2s_arabic_text,0,(pt_ctx->t_str_ttl.ui2_len_text + 1)*sizeof(UTF16_T));
		reorder_arabic(pt_ctx->t_str_ttl.w2s_text, pt_ctx->t_str_ttl.ui2_len_text,NULL, w2s_arabic_text, &ui2_arabic_len);
	}
	
	#endif

    h_font = WGL_STR_GET_FONT(&pt_ctx->t_str_ttl);

	CATCH_FAIL(x_wgl_get_string_size(h_font,pt_ctx->t_str_ttl.w2s_text,pt_ctx->t_str_ttl.ui2_len_text,&i4_w,&i4_h));
	
    _bdrunfm_tool_cal_obj_pos(
        pt_ctx->ui1_text_direction,
        pt_ctx->t_info_ttl.ui1_align, 
        i4_w,
        i4_h,
        &pt_ctx->t_info_ttl.t_rc_ref,
        &i4_x, 
        &i4_y);
        
    SET_RECT_BY_SIZE(&t_rc_ttl, i4_x, i4_y, i4_w, i4_h);        
    CATCH_FAIL(x_wgl_rect_is_cross(&b_need_update, &t_rc_ttl, pt_rc_upt));
    if(!b_need_update)
    {
        return WGLR_OK;
    }
    
    if(HAS_FLAG(pt_ctx->t_info_ttl.ui2_style, WGL_BDR_UNFM_TITLE_STL_TRANSP_BK))
    {
        wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_clean);
        pt_clr_ttl_bk = &t_clr_clean;
        i4_bg_h       = 0;
    }
    else
    {
        _bdrunfm_tool_select_color(&pt_ctx->t_clrs_ttl_bk, ui2_state, &pt_clr_ttl_bk);   
        i4_bg_h = i4_h;
    }
    
    _bdrunfm_tool_select_color(&pt_ctx->t_clrs_ttl, ui2_state, &pt_clr_ttl);
    
    /* string rendering control */
    b_draw_str = TRUE;
    if(wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_ttl))
    {
        if( i4_bg_h != 0 )
        {
            /* case 1 : bk color is not transparent */
            if(wgl_cvsst_check_palette_mode(&pt_ctx->t_cvsst))
            {
                /* case 1.1 : in palette mode, text is rendered by bitblt */
                /* make the text color same as bk color, look like text is transparent */
                pt_clr_ttl = pt_clr_ttl_bk;
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
    
    if( b_draw_str )
    {
#ifdef ARAB_EXIST_SUPPORT
		if (!b_exist_arab)
#endif	
		{
	        INFORM_ERR(x_gl_draw_ustring_ex(
	            h_g, 
	            i4_x,
	            i4_y,
	            i4_y,
	            WGL_STR_GET_TEXT(&pt_ctx->t_str_ttl),
	            WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl),
	            NULL,
	            0,
	            WGL_STR_GET_FONT(&pt_ctx->t_str_ttl),
	            pt_clr_ttl,
	            pt_clr_ttl_bk,
	            NULL,
	            FALSE,
	            0,
	            0,
	            wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_ttl_bk) ? (UINT16)0 : (UINT16)i4_bg_h,
	            0));
		}
#ifdef ARAB_EXIST_SUPPORT
		else
		{
			INFORM_ERR(x_gl_draw_ustring_ex(
	            h_g, 
	            i4_x,
	            i4_y,
	            i4_y,
	            w2s_arabic_text + WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl) - ui2_arabic_len,
	            ui2_arabic_len,
	            NULL,
	            0,
	            WGL_STR_GET_FONT(&pt_ctx->t_str_ttl),
	            pt_clr_ttl,
	            pt_clr_ttl_bk,
	            NULL,
	            FALSE,
	            0,
	            0,
	            wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_ttl_bk) ? (UINT16)0 : (UINT16)i4_bg_h,
	            0));
		}
#endif	

    }
    
#ifdef ARAB_EXIST_SUPPORT
	if (w2s_arabic_text)
	{
		WGL_MEM_FREE(w2s_arabic_text);
	}
#endif
    return WGLR_OK;   
}


static VOID _bdrunfm_tool_cal_obj_pos( 
    IN      UINT8                       ui1_text_dir,
    IN      UINT8                       ui1_align,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      GL_RECT_T                   *pt_rc_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y)
{
    UINT8 ui1_adjusted_align;

    ui1_adjusted_align = 0;
    if(ui1_text_dir == WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT)
    {
        ui1_adjusted_align = ui1_align;
    }else
    {
       /*reverse text horizontal alignment for right to left text direction*/
        ui1_adjusted_align |= (WGL_IS_H_LEFT(ui1_align)<<2);
        ui1_adjusted_align |= WGL_IS_H_CENTER(ui1_align);
        ui1_adjusted_align |= (WGL_IS_H_RIGHT(ui1_align)>>2);
        
        /*text direction dose not affect vertical alignment*/
        ui1_adjusted_align |= (ui1_align&0x0F);
    }
    if( WGL_IS_H_LEFT(ui1_adjusted_align) )
    {
        *pi4_x = pt_rc_bound->i4_left;
    }
    else if( WGL_IS_H_CENTER(ui1_adjusted_align) )
    {
        *pi4_x = pt_rc_bound->i4_left + ((RECT_W(pt_rc_bound) - i4_obj_w) >> 1);
    }
    else
    {
        *pi4_x = pt_rc_bound->i4_right - i4_obj_w;
    }

    if( WGL_IS_V_TOP(ui1_adjusted_align) )
    {
        *pi4_y = pt_rc_bound->i4_top;
    }
    else if( WGL_IS_V_CENTER(ui1_adjusted_align) )
    {
        *pi4_y = pt_rc_bound->i4_top + ((RECT_H(pt_rc_bound) - i4_obj_h) >> 1);
    }
    else
    {
        *pi4_y = pt_rc_bound->i4_bottom - i4_obj_h;
    }

}

