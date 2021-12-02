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
 * $RCSfile: wgl_bdr_drop_shadow_right.c,v $
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
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/bdr_drop_shadow_right/wgl_bdr_drop_shadow_right.h"
#include "wgl/bdr_drop_shadow_right/u_wgl_bdr_drop_shadow_right.h"

/*-----------------------------------------------------------------------------
                    constants
 ----------------------------------------------------------------------------*/
#define BDR_DPSDWRHT_DFT_HEIGHT                 6
#define BDR_DPSDWRHT_DFT_SHADOW_ALPHA_FACTOR    128

static const WGL_INSET_T BDR_DPSDWRHT_DFT_INSET =
    {
        0,
        BDR_DPSDWRHT_DFT_HEIGHT, 
        0,
        BDR_DPSDWRHT_DFT_HEIGHT
    };

static const WGL_INSET_T BDR_DPSDWRHT_MIN_INSET =
    {
        0,
        0,
        0,
        0
    };


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "BDRDSR"

#define IN
#define OUT


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _BDR_DPSDWRHT_CTX_T
{
#ifdef CLI_LVL_ALL     
    UINT32                  ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    BOOL                    b_is_draw_intr;
    GL_RECT_T               t_rc_widget;
    WGL_INSET_T             t_inset_bdr;
    UINT16                  ui2_height;
    UINT8                   ui1_shadow_alpha_factor;
    UINT8                   aui1_fg_alpha[6];
    WGL_CANVAS_STATUS_T     t_cvsst;
    
    WGL_COLOR_SET_TYPE_T    e_clrs_type_src_fg;
    WGL_COLOR_EXT_SET_T     t_clrs_fg;
    WGL_COLOR_SET_TYPE_T    e_clrs_type_src_bk;
    WGL_COLOR_EXT_SET_T     t_clrs_bk;

} BDR_DPSDWRHT_CTX_T;



/*-----------------------------------------------------------------------------
                    internal functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_size(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_set_inset(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_dpsdwrht_cmd_hdr_get_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_get_min_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);
 
static INT32 _bdr_dpsdwrht_cmd_hdr_set_is_bdr_draw_interior(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_dpsdwrht_cmd_hdr_get_is_bdr_draw_interior(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_dpsdwrht_cmd_hdr_set_bdr_color(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _bdr_dpsdwrht_cmd_hdr_get_bdr_color(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);

static INT32 _bdr_dpsdwrht_cmd_hdr_get_preferred_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_set_height(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_get_height(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_set_shadow_alpha_factor(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1);
    
static INT32 _bdr_dpsdwrht_cmd_hdr_get_shadow_alpha_factor(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static VOID _bdr_dpsdwrht_tool_copy_from_color_info_to_extend_color_set(
    OUT     WGL_COLOR_EXT_SET_T         *pt_clrs_ex,
    IN      WGL_COLOR_INFO_T            *pt_clr_info_from);
    
static VOID _bdr_dpsdwrht_tool_copy_from_extend_color_set_to_color_info( 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out, 
    IN      WGL_COLOR_SET_TYPE_T        e_clrs_type,
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_ex_from);
    
static VOID _bdr_dpsdwrht_tool_load_sys_def_clrs_bk(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx);
    
static VOID _bdr_dpsdwrht_tool_load_sys_def_clrs_fg(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx);
    
static VOID _bdr_dpsdwrht_tool_select_color(
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_src,
    IN      UINT16                      ui2_state, 
    OUT     GL_COLOR_T                  **ppt_clr_out);

static VOID _bdr_dpsdwrht_tool_apply_shadow_alpha_factor(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx);
    
    
/*-----------------------------------------------------------------------------
                    exported functions 
 ----------------------------------------------------------------------------*/
INT32 x_wgl_bdr_drop_shadow_right_init(VOID)
{
    WGL_BORDER_CTRL_INTF_T      t_bdr_intf = 
        {
            wgl_bdr_drop_shadow_right_inst_create,
            wgl_bdr_drop_shadow_right_inst_destroy,
            wgl_bdr_drop_shadow_right_cmd_proc,
            wgl_bdr_drop_shadow_right_paint
        };
    
    return wgl_register_border_intf(WGL_BORDER_DROP_SHADOW_RIGHT, &t_bdr_intf);
}


/*-----------------------------------------------------------------------------
                    public functions 
 ----------------------------------------------------------------------------*/
INT32 wgl_bdr_drop_shadow_right_inst_create(
    WGL_BORDER_CONTEXT_HANDLE_T         *ppv_ctx_bdr, 
    WGL_BORDER_CREATE_INPUT_T           *pt_bdr_create)
{
    BDR_DPSDWRHT_CTX_T  *pt_ctx = NULL;
    INT32               i4_ret;

    *ppv_ctx_bdr    = NULL;
    
    pt_ctx = (BDR_DPSDWRHT_CTX_T *)WGL_MEM_ALLOC(sizeof(BDR_DPSDWRHT_CTX_T));
    if(pt_ctx==NULL)
    {
        INFORM_ERR(WGLR_OUT_OF_MEMORY);
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL   
    pt_ctx->ui4_mem_size = sizeof(BDR_DPSDWRHT_CTX_T);
#endif    
    pt_ctx->b_is_draw_intr          = HAS_FLAG(pt_bdr_create->ui4_style, WGL_STL_GL_BDR_FILL_CNT_BK) ? TRUE : FALSE;
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, pt_bdr_create->i4_width, pt_bdr_create->i4_height);
    WGL_COPY_INSET(&pt_ctx->t_inset_bdr, &BDR_DPSDWRHT_DFT_INSET);
    pt_ctx->ui2_height              = BDR_DPSDWRHT_DFT_HEIGHT;
    pt_ctx->ui1_shadow_alpha_factor = BDR_DPSDWRHT_DFT_SHADOW_ALPHA_FACTOR;
    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_bdr_create->pt_cvsst);
    _bdr_dpsdwrht_tool_load_sys_def_clrs_bk(pt_ctx);
    _bdr_dpsdwrht_tool_load_sys_def_clrs_fg(pt_ctx);
    
    *ppv_ctx_bdr = (WGL_BORDER_CONTEXT_HANDLE_T)pt_ctx; 
    return WGLR_OK;    
}

INT32 wgl_bdr_drop_shadow_right_inst_destroy(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr)
{

    WGL_MEM_FREE(pv_ctx_bdr);

    return WGLR_OK;
}


INT32 wgl_bdr_drop_shadow_right_paint(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    at_upt_rect,
    UINT16                              ui2_state)
{
    BDR_DPSDWRHT_CTX_T  *pt_ctx = (BDR_DPSDWRHT_CTX_T *)pv_ctx_bdr;
    INT32               i4_ret;
    GL_RECT_T           t_rc_cnt;
    GL_RECT_T           t_rc_upt;
    GL_RECT_T           t_rc_shadow;
    GL_COLOR_T          *pt_clr_shadow;

    /* check state for quick return */
    if(!IS_STATE_VISIBLE(ui2_state))    
    {
        return WGLR_OK;
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
    
    /* draw the content */
    if(pt_ctx->b_is_draw_intr)
    {
        GL_COLOR_T      *pt_clr_bk;
        _bdr_dpsdwrht_tool_select_color(&pt_ctx->t_clrs_bk, ui2_state, &pt_clr_bk);
        INFORM_ERR(wgl_fill_widget_bk(h_g, pt_clr_bk, &t_rc_cnt, &pt_ctx->t_cvsst));
    }    
    
    /* check for quick return */
    if(WGL_RECT_IS_INCLUDED(&t_rc_cnt, &t_rc_upt))
    {
        return WGLR_OK;
    }
    
    /* draw the shadow */
    if(pt_ctx->ui2_height > 0)
    {
        INT32 i4_shadow_h = (INT32)pt_ctx->ui2_height;    
        _bdr_dpsdwrht_tool_select_color(&pt_ctx->t_clrs_fg, ui2_state, &pt_clr_shadow);

        if (wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_shadow))
        {
            return WGLR_OK;
        }
        
        i4_ret = x_gl_set_color(h_g, pt_clr_shadow);        
        if(i4_ret==GLR_OK)
        {
            /* draw the right shadow */
            SET_RECT(
                &t_rc_shadow,
                t_rc_cnt.i4_right,
                t_rc_cnt.i4_top + i4_shadow_h,
                t_rc_cnt.i4_right + i4_shadow_h,
                t_rc_cnt.i4_bottom + i4_shadow_h);
            INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_shadow));
            
            /* draw the bottom shadow */
            SET_RECT(
                &t_rc_shadow,
                t_rc_cnt.i4_left + i4_shadow_h,
                t_rc_cnt.i4_bottom,
                t_rc_cnt.i4_right,
                t_rc_cnt.i4_bottom + i4_shadow_h);
            if(RECT_IS_EFFECTIVE(t_rc_shadow))
            {
                INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_shadow));
            }
        }             
        else
        {
            INFORM_ERR(i4_ret);
        }
    }
    
    return WGLR_OK;
}

INT32 wgl_bdr_drop_shadow_right_cmd_proc(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    UINT32                              ui4_cmd_id, 
    VOID                                *pv_param1, 
    VOID                                *pv_param2)
{
    BDR_DPSDWRHT_CTX_T  *pt_ctx = (BDR_DPSDWRHT_CTX_T *)pv_ctx_bdr;
    INT32           i4_ret, i4_ret_val;

    if(!(WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_BASE, ui4_cmd_id) ||
         WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_DROP_SHADOW_RIGHT, ui4_cmd_id)) )
    {
        INFORM_ERR(WGLR_INV_CMD);
        return WGLR_INV_CMD;
    }
    
    i4_ret_val = WGLR_OK;

    switch(ui4_cmd_id) 
    {
        /*Internal Border Commands*/
        case WGL_BDR_GL_SET_SIZE:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_size(pt_ctx, pv_param1, pv_param2);
            break;

        /*Common Border Commands*/
        case WGL_CMD_GL_SET_INSET:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_inset(pt_ctx, pv_param1);
            break;
        
        case WGL_CMD_GL_GET_INSET:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_inset(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_GET_MIN_INSET:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_min_inset(pt_ctx, pv_param1);
            break;
    
        case WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_is_bdr_draw_interior(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_is_bdr_draw_interior(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_SET_BDR_COLOR: /*pv_param1: (UINT8)target, pv_param2:(WGL_COLOR_INFO_T *)*pt_color_info*/
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_bdr_color(pt_ctx, pv_param1, pv_param2);
            break;

        case WGL_CMD_GL_GET_BDR_COLOR:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_bdr_color(pt_ctx, pv_param1, pv_param2);
            break;

        /* Specific Border Commands */
        case WGL_CMD_BDR_DPSDWRHT_GET_PREFERRED_INSET:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_preferred_inset(pt_ctx, pv_param1);
            break;
        
        case WGL_CMD_BDR_DPSDWRHT_SET_HEIGHT:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_height(pt_ctx, pv_param1);
            break;

        case WGL_CMD_BDR_DPSDWRHT_GET_HEIGHT:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_height(pt_ctx, pv_param1);
            break;
        
        case WGL_CMD_BDR_DPSDWRHT_SET_SHADOW_ALPHA_FACTOR:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_set_shadow_alpha_factor(pt_ctx, pv_param1);
            break;
        
        case WGL_CMD_BDR_DPSDWRHT_GET_SHADOW_ALPHA_FACTOR:
            i4_ret_val = _bdr_dpsdwrht_cmd_hdr_get_shadow_alpha_factor(pt_ctx, pv_param1);
            break;
        case WGL_CMD_GL_SET_BDR_TEXT_DIRECTION:
            break;

        default:
            i4_ret_val = WGLR_INV_CMD;
    }

    INFORM_ERR(i4_ret_val);
    return i4_ret_val;
}



/*-----------------------------------------------------------------------------
                    command handler definitions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_size
 *
 * Description: This command handler sets the new size of the host widget
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (INT32)Contains the new width of the widget
 *          pv_param2   (INT32)Contains the new height of the widget
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK     Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_size(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    INT32       i4_size_w = (INT32)pv_param1;
    INT32       i4_size_h = (INT32)pv_param2;
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, i4_size_w, i4_size_h);
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_inset
 *
 * Description: This command handler sets the insets of the border
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (WGL_INSET_T *)Contains the insets, could be NULL
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK     Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_inset(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_INSET_T     *pt_inset = (WGL_INSET_T *)pv_param1;
    
    if(pt_inset==NULL)
    {
        WGL_COPY_INSET(&pt_ctx->t_inset_bdr, &BDR_DPSDWRHT_DFT_INSET);
    }
    else
    {
        CHECK_VALID(_INSET_IS_VALID(*pt_inset), WGLR_INV_ARG);
        WGL_COPY_INSET(&pt_ctx->t_inset_bdr, pt_inset);
    }    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_inset
 *
 * Description: This command handler gets the insets of the border
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_INSET_T *)Contains the insets
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_INSET_T *pt_inset_out = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_out, WGLR_INV_ARG);
    _INSET_COPY(pt_inset_out, &pt_ctx->t_inset_bdr);
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_min_inset
 *
 * Description: This command handler gets the minimal insets of the border
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_INSET_T *)Contains the insets
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_min_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    
    WGL_INSET_T *pt_inset_out = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_out, WGLR_INV_ARG);
    _INSET_COPY(pt_inset_out, &BDR_DPSDWRHT_MIN_INSET);
        
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_is_bdr_draw_interior
 *
 * Description: This command handler sets the flag indicating the border should 
 *              draw the content part of the widget or not 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (BOOL) Contains the flag
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK     Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_is_bdr_draw_interior(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1)
{
    BOOL    b_draw = WGL_POINTER_TO_BOOL(pv_param1);
    
    CHECK_VALID(b_draw==TRUE || b_draw==FALSE, WGLR_INV_ARG);
    
    pt_ctx->b_is_draw_intr = b_draw;    
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_is_bdr_draw_interior
 *
 * Description: This command handler gets the flag indicating the border should 
 *              draw the content part of the widget or not 
 
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (BOOL *) Contains the flag
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_is_bdr_draw_interior(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL    *pb_draw = (BOOL *)pv_param1;
    
    CHECK_NULL(pb_draw, WGLR_INV_ARG);
    *pb_draw = pt_ctx->b_is_draw_intr;    
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_bdr_color
 *
 * Description: This command handler sets the target color info 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UINT8) Contains the target to be set color info
 *          pv_param2   (WGL_COLOR_INFO_T *) Contains the color info, 
 *                      NULL indicates the system default color info
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    (1) The color target indicated in pv_param1 is invalid
 *                          (2) The color info is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_bdr_color(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    UINT8            ui1_target   = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_COLOR_INFO_T *pt_clr_info = (WGL_COLOR_INFO_T *)pv_param2;
    INT32            i4_ret;
    
    if(pt_clr_info==NULL)
    {
        switch(ui1_target)
        {
            case WGL_CLR_BK:
            case WGL_CLR_BDR_BK:
                _bdr_dpsdwrht_tool_load_sys_def_clrs_bk(pt_ctx);
            break;
            
            case WGL_CLR_BDR_FG:
                _bdr_dpsdwrht_tool_load_sys_def_clrs_fg(pt_ctx);
            break;
            
            default:
                INFORM_ERR(WGLR_INV_ARG);
                return WGLR_INV_ARG;
        }
    }
    else
    {
        CHECK_VALID(IS_LEGAL_CLR_SET_TYPE(pt_clr_info->e_type), WGLR_INV_ARG);
        switch(ui1_target)
        {
            case WGL_CLR_BK:
            case WGL_CLR_BDR_BK:
                pt_ctx->e_clrs_type_src_bk = pt_clr_info->e_type;
                _bdr_dpsdwrht_tool_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_bk, pt_clr_info);
            break;
            
            case WGL_CLR_BDR_FG:
                pt_ctx->e_clrs_type_src_fg = pt_clr_info->e_type;
                _bdr_dpsdwrht_tool_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_fg, pt_clr_info);
				pt_ctx->aui1_fg_alpha[0] = pt_ctx->t_clrs_fg.t_enable.a;   
				pt_ctx->aui1_fg_alpha[1] = pt_ctx->t_clrs_fg.t_disable.a;         
				pt_ctx->aui1_fg_alpha[2] = pt_ctx->t_clrs_fg.t_highlight.a;       
				pt_ctx->aui1_fg_alpha[3] = pt_ctx->t_clrs_fg.t_highlight_inactive.a; 
				pt_ctx->aui1_fg_alpha[4] = pt_ctx->t_clrs_fg.t_push.a;             
				pt_ctx->aui1_fg_alpha[5] = pt_ctx->t_clrs_fg.t_push_unhighlight.a; 
                _bdr_dpsdwrht_tool_apply_shadow_alpha_factor(pt_ctx);
            break;
            
            default:
                INFORM_ERR(WGLR_INV_ARG);
                return WGLR_INV_ARG;
        }        
    }
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_bdr_color
 *
 * Description: This command handler gets the target color info 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UINT8) Contains the target to be got color info
 *
 * Outputs: pv_param2   (WGL_COLOR_INFO_T *) Contains the color info, 
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    (1) The color target indicated in pv_param1 is invalid
 *                          (2) The argument pv_param2 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_bdr_color(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    UINT8            ui1_target       = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_COLOR_INFO_T *pt_clr_info_out = (WGL_COLOR_INFO_T *)pv_param2;
    INT32            i4_ret; 
    
    switch(ui1_target)
    {
        case WGL_CLR_BK:
        case WGL_CLR_BDR_BK:
            _bdr_dpsdwrht_tool_copy_from_extend_color_set_to_color_info(pt_clr_info_out, pt_ctx->e_clrs_type_src_bk, &pt_ctx->t_clrs_bk);
        break;
        
        case WGL_CLR_BDR_FG:
            _bdr_dpsdwrht_tool_copy_from_extend_color_set_to_color_info(pt_clr_info_out, pt_ctx->e_clrs_type_src_fg, &pt_ctx->t_clrs_fg);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_ARG);
            return WGLR_INV_ARG;
    }            
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_preferred_inset
 *
 * Description: This command handler gets the preferred inset according to current border context
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_INSET_T *)Returns the preferred inset
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The argument pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_preferred_inset(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_INSET_T *pt_inset_out = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_out, WGLR_INV_ARG);
    pt_inset_out->i4_left   = 0;
    pt_inset_out->i4_top    = 0;
    pt_inset_out->i4_right  = (INT32)pt_ctx->ui2_height;
    pt_inset_out->i4_bottom = (INT32)pt_ctx->ui2_height;
    
    return WGLR_OK;   
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_height
 *
 * Description: This command handler sets the shadow height 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UINT16) Contains the shadow height
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_height(
    IN OUT  BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT16 ui2_height = WGL_POINTER_TO_UINT16(pv_param1);
    
    pt_ctx->ui2_height = ui2_height;
 
    return WGLR_OK;   
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_height
 *
 * Description: This command handler gets the shadow height
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (UINT16 *) Returns the shadow height
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The argument pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_height(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT16  *pui2_height_out = (UINT16 *)pv_param1;
    
    CHECK_NULL(pui2_height_out, WGLR_INV_ARG);
    *pui2_height_out = pt_ctx->ui2_height;
 
    return WGLR_OK;   
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_set_shadow_alpha_factor
 *
 * Description: This command handler sets the alpha factor for shadow color
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UINT8) Contains the shadow alpha factor
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_set_shadow_alpha_factor(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT8   ui1_saf = WGL_POINTER_TO_UINT8(pv_param1);
    
    pt_ctx->ui1_shadow_alpha_factor = ui1_saf;
    _bdr_dpsdwrht_tool_apply_shadow_alpha_factor (pt_ctx);
 
    return WGLR_OK;   
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_cmd_hdr_get_shadow_alpha_factor
 *
 * Description: This command handler gets the alpha factor for shadow color
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (UINT8 *) Returns the shadow alpha factor
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The argument pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_dpsdwrht_cmd_hdr_get_shadow_alpha_factor(
    IN      BDR_DPSDWRHT_CTX_T          *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT8   *pui1_saf_out = (UINT8 *)pv_param1;
    
    *pui1_saf_out = pt_ctx->ui1_shadow_alpha_factor;
 
    return WGLR_OK;   
}        



/*-----------------------------------------------------------------------------
                    internal functions definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_copy_from_color_info_to_extend_color_set
 *
 * Description: This tool function copy extended color set from an color info
 *
 * Inputs:  pt_clr_info_from  Contains the source color info
 *
 * Outputs: pt_clrs_ex        Contains the result extended color set
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_copy_from_color_info_to_extend_color_set(
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

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_copy_from_extend_color_set_to_color_info
 *
 * Description: This tool function copy color info from an extended color set
 *
 * Inputs:  pt_clrs_ex_from   Contains the source extended color set
 *          e_clrs_type       Contains the target color set type 
 *
 * Outputs: pt_clr_info_out   Contains the result color info
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_copy_from_extend_color_set_to_color_info( 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out, 
    IN      WGL_COLOR_SET_TYPE_T        e_clrs_type,
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_ex_from)
{
    pt_clr_info_out->e_type = e_clrs_type;
    switch(e_clrs_type) 
    {
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

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_load_sys_def_clrs_bk
 *
 * Description: This tool function loads the system default background colors
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_load_sys_def_clrs_bk(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx)
{
    pt_ctx->e_clrs_type_src_bk = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_bk.t_push_unhighlight);
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_load_sys_def_clrs_bk
 *
 * Description: This tool function loads the system default foreground colors
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_load_sys_def_clrs_fg(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx)
{
    pt_ctx->e_clrs_type_src_fg = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_fg.t_push_unhighlight);
	pt_ctx->aui1_fg_alpha[0] = pt_ctx->t_clrs_fg.t_enable.a;   
	pt_ctx->aui1_fg_alpha[1] = pt_ctx->t_clrs_fg.t_disable.a;         
	pt_ctx->aui1_fg_alpha[2] = pt_ctx->t_clrs_fg.t_highlight.a;       
	pt_ctx->aui1_fg_alpha[3] = pt_ctx->t_clrs_fg.t_highlight_inactive.a; 
	pt_ctx->aui1_fg_alpha[4] = pt_ctx->t_clrs_fg.t_push.a;             
	pt_ctx->aui1_fg_alpha[5] = pt_ctx->t_clrs_fg.t_push_unhighlight.a; 
    _bdr_dpsdwrht_tool_apply_shadow_alpha_factor(pt_ctx);
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_select_color
 *
 * Description: This tool function select a color according to a certain state
 *
 * Inputs:  pt_clrs_src  Contains the source extended color set
 *          ui2_state    Specifies the state
 *
 * Outputs: ppt_clr_out  Contains the color corresponding to the specified state
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_select_color(
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

/*-----------------------------------------------------------------------------
 * Name: _bdr_dpsdwrht_tool_apply_shadow_alpha_factor
 *
 * Description: This tool function applies the shadow alpha factor to the shadow color
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_dpsdwrht_tool_apply_shadow_alpha_factor(
    IN OUT  BDR_DPSDWRHT_CTX_T              *pt_ctx)
{
    if(!wgl_cvsst_check_palette_mode(&pt_ctx->t_cvsst))
    {
    	pt_ctx->t_clrs_fg.t_enable.a             = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[0]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
        pt_ctx->t_clrs_fg.t_disable.a            = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[1]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
        pt_ctx->t_clrs_fg.t_highlight.a          = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[2]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
        pt_ctx->t_clrs_fg.t_highlight_inactive.a = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[3]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
        pt_ctx->t_clrs_fg.t_push.a               = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[4]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
        pt_ctx->t_clrs_fg.t_push_unhighlight.a   = (UINT8)((((UINT32)pt_ctx->aui1_fg_alpha[5]) * ((UINT32)pt_ctx->ui1_shadow_alpha_factor)) / ((UINT32)255));
    }
}    


