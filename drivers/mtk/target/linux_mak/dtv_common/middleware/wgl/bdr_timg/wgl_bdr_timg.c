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
 * $RCSfile: wgl_bdr_timg.c,v $
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
#include "wgl/x_wgl_rect.h"
#include "wgl/wgl_image.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/bdr_timg/u_wgl_bdr_timg.h"
#include "wgl/bdr_timg/wgl_bdr_timg.h"
#include "wgl/wgl_string.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/u_wgl_bdr_common.h"

#include "wgl/arabic.h"


/*-----------------------------------------------------------------------------
                    constants
 ----------------------------------------------------------------------------*/
const static WGL_INSET_T BDR_TIMG_DFT_INSET = {0, 0, 0, 0};
const static WGL_INSET_T BDR_TIMG_MIN_INSET = {0, 0, 0, 0};

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "BDRTIMG"

#define IN
#define OUT

#define TTL_STR_INIT_BUF_SIZE       30
#define TTL_STR_BUF_INC_SIZE        20

#define BDR_SMART_CUT_CHARS            ((UTF16_T *)(L"..."))
#define BDR_SMART_CUT_CHARS_LEN        3
#define BDR_MAX_VALUE                  ((INT32) 0x40000000)

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
#define BDR_TIMG_ATTR_NO_BDR_BK     MAKE_BIT_MASK_16(0)
#define BDR_TIMG_ATTR_NO_IMG_UI     MAKE_BIT_MASK_16(1)
#define BDR_TIMG_ATTR_DRAW_INTR     MAKE_BIT_MASK_16(2)

typedef struct _BDR_TIMG_CTX_T
{
#ifdef CLI_LVL_ALL     
    UINT32                     ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    /* general attributes */
    UINT16                      ui2_attr;
    GL_RECT_T                   t_rc_widget;
    WGL_INSET_T                 t_inset_bdr;
    WGL_CANVAS_STATUS_T         t_cvsst;

    /* background color */
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_bk;
    WGL_COLOR_EXT_SET_T         t_clrs_bk;
    
    /* background image */
    WGL_IMG_SET_TYPE_T          e_imgs_type_src_bk;
    WGL_IMG_EXT_SET_T           t_imgs_bk;

    /* title info */
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_ttl;
    WGL_COLOR_EXT_SET_T         t_clrs_ttl;
    WGL_COLOR_SET_TYPE_T        e_clrs_type_src_ttl_bk;
    WGL_COLOR_EXT_SET_T         t_clrs_ttl_bk;
    WGL_BDR_TIMG_TITLE_INFO_T   t_info_ttl;
    WGL_STRING_T                t_str_ttl;
    WGL_BDR_TIMG_TITLE_INFO_T   t_info_ttl_ex;
    HFONT_T                     h_ttl_fnt_ex;
    UINT8                       ui1_text_direction;
    
    /* icon info */
    WGL_BDR_TIMG_ICON_INFO_T    t_info_icon;
    WGL_IMG_SET_TYPE_T          e_imgs_type_src_icon;
    WGL_IMG_EXT_SET_T           t_imgs_icon;
    
} BDR_TIMG_CTX_T;


/*-----------------------------------------------------------------------------
                    internal functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_size(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _bdr_timg_cmd_hdr_set_inset(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_inset(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_min_inset(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_timg_cmd_hdr_set_is_bdr_draw_interior(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);
    
static INT32 _bdr_timg_cmd_hdr_get_is_bdr_draw_interior(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_set_bdr_color(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _bdr_timg_cmd_hdr_set_style(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_style(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_bdr_color(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
static INT32 _bdr_timg_cmd_hdr_set_text_direction(
    IN OUT  BDR_TIMG_CTX_T               *pt_ctx, 
    IN      UINT8                       ui1_text_dir);
static INT32 _bdr_timg_cmd_hdr_get_text_direction(
    IN OUT  BDR_TIMG_CTX_T               *pt_ctx, 
    IN      UINT8                       *pui1_text_dir);
static INT32 _bdr_timg_cmd_hdr_set_bk_img(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_bk_img(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_timg_cmd_hdr_set_title_info(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_title_info(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_set_title_text(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2);

static INT32 _bdr_timg_cmd_hdr_get_title_text(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2);
        
static INT32 _bdr_timg_cmd_hdr_set_icon_info(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);

static INT32 _bdr_timg_cmd_hdr_get_icon_info(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);
    
static INT32 _bdr_timg_cmd_hdr_set_icon(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);
    
static INT32 _bdr_timg_cmd_hdr_get_icon(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1);

static INT32 _bdr_timg_tool_bdr_area_fill_color(
    IN      GL_RECT_T                   *pt_rc_wgt,
    IN      WGL_INSET_T                 *pt_inset_bdr,
    IN      GL_HGRAPHICS_T              h_g, 
    IN      GL_COLOR_T                  *pt_clr);

static INT32 _bdr_timg_tool_bdr_area_fill_img(
    IN      GL_RECT_T                   *pt_rc_wgt,
    IN      WGL_INSET_T                 *pt_inset_bdr,
    IN      GL_RECT_T                   *pt_rc_upt,
    IN      GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_HIMG_TPL_T              h_img);
    
static VOID _bdr_timg_tool_copy_from_color_info_to_extend_color_set(
    OUT     WGL_COLOR_EXT_SET_T         *pt_clrs_ex,
    IN      WGL_COLOR_INFO_T            *pt_clr_info_from);

static VOID _bdr_timg_tool_copy_from_extend_color_set_to_color_info( 
    OUT     WGL_COLOR_INFO_T            *pt_clr_info_out, 
    IN      WGL_COLOR_SET_TYPE_T        e_clrs_type,
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_ex_from);

static VOID _bdr_timg_tool_copy_from_img_info_to_extend_img_set(
    OUT     WGL_IMG_EXT_SET_T           *pt_imgs_ex,
    IN      WGL_IMG_INFO_T              *pt_img_info_from);

static VOID _bdr_timg_tool_copy_from_extend_img_set_to_img_info( 
    OUT     WGL_IMG_INFO_T              *pt_img_info_out, 
    IN      WGL_IMG_SET_TYPE_T          e_imgs_type,
    IN      WGL_IMG_EXT_SET_T           *pt_imgs_ex_from);

static VOID _bdr_timg_tool_load_sys_def_clrs_bk(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx);
    
static VOID _bdr_timg_tool_load_sys_def_clrs_ttl_text(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx);

static VOID _bdr_timg_tool_load_sys_def_clrs_ttl_text_bk(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx);

static VOID _bdr_timg_tool_select_color(
    IN      WGL_COLOR_EXT_SET_T         *pt_clrs_src,
    IN      UINT16                      ui2_state, 
    OUT     GL_COLOR_T                  **ppt_clr_out);
    
static VOID _bdr_timg_tool_select_img(
    IN      WGL_IMG_EXT_SET_T           *pt_imgs_src,
    IN      UINT16                      ui2_state, 
    OUT     WGL_HIMG_TPL_T              *ph_img_out);

static INT32 _bdr_timg_tool_draw_title(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt);
    
static INT32 _bdr_timg_tool_draw_icon(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt);
    
static VOID _bdr_timg_tool_cal_obj_pos( 
    IN      UINT8                      ui1_text_dir,
    IN      UINT8                       ui1_align,
    IN      INT32                       i4_obj_w,    
    IN      INT32                       i4_obj_h,    
    IN      GL_RECT_T                   *pt_rc_bound,
    OUT     INT32                       *pi4_x,
    OUT     INT32                       *pi4_y);

static INT32 _bdr_timg_cmd_hdr_set_title_info_ex(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1);
    

/*-----------------------------------------------------------------------------
                    exported functions 
 ----------------------------------------------------------------------------*/
INT32 x_wgl_bdr_timg_init(VOID)
{
    WGL_BORDER_CTRL_INTF_T      t_bdr_intf = { wgl_bdr_timg_inst_create,
                                               wgl_bdr_timg_inst_destroy,
                                               wgl_bdr_timg_cmd_proc,
                                               wgl_bdr_timg_paint
                                             };
    
    return wgl_register_border_intf(WGL_BORDER_TIMG, &t_bdr_intf);
}

/*-----------------------------------------------------------------------------
                    public functions 
 ----------------------------------------------------------------------------*/
INT32 wgl_bdr_timg_inst_create(
    WGL_BORDER_CONTEXT_HANDLE_T         *ppv_ctx_bdr, 
    WGL_BORDER_CREATE_INPUT_T           *pt_bdr_create)
{
    BDR_TIMG_CTX_T  *pt_ctx = NULL;
    
    *ppv_ctx_bdr = NULL;

    pt_ctx = (BDR_TIMG_CTX_T *)WGL_MEM_ALLOC(sizeof(BDR_TIMG_CTX_T));
    CHECK_VALID(pt_ctx != NULL, WGLR_OUT_OF_MEMORY);
#ifdef CLI_LVL_ALL   
    pt_ctx->ui4_mem_size = sizeof(BDR_TIMG_CTX_T);
#endif  
    pt_ctx->ui2_attr = 0;
    if(HAS_FLAG(pt_bdr_create->ui4_style, WGL_STL_GL_BDR_FILL_CNT_BK))
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR);
    }
    
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, pt_bdr_create->i4_width, pt_bdr_create->i4_height);
    WGL_COPY_INSET(&pt_ctx->t_inset_bdr, &BDR_TIMG_DFT_INSET);
    wgl_cvsst_copy(&pt_ctx->t_cvsst, pt_bdr_create->pt_cvsst);
    _bdr_timg_tool_load_sys_def_clrs_bk(pt_ctx);
    
    pt_ctx->e_imgs_type_src_bk = WGL_IMG_SET_BASIC;
    x_memset(&pt_ctx->t_imgs_bk, 0, sizeof(WGL_IMG_EXT_SET_T));
    
    /* init title info */ 
    _bdr_timg_tool_load_sys_def_clrs_ttl_text(pt_ctx);
    _bdr_timg_tool_load_sys_def_clrs_ttl_text_bk(pt_ctx);
    pt_ctx->t_info_ttl.ui2_style = 0;
    x_wgl_get_system_font(&pt_ctx->t_info_ttl.t_font);
    WGL_RECT_EMPTY(&pt_ctx->t_info_ttl.t_rc_ref);
    pt_ctx->t_info_ttl.ui1_align = WGL_AS_CENTER_CENTER;
    x_memset(&pt_ctx->t_info_ttl_ex, 0, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));
    pt_ctx->h_ttl_fnt_ex = NULL_HANDLE;
    pt_ctx->ui1_text_direction = WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT;
    x_memset(&pt_ctx->t_str_ttl, 0, sizeof(WGL_STRING_T));

    
    /* init icon info */
    WGL_RECT_EMPTY(&pt_ctx->t_info_icon.t_rc_ref);
    pt_ctx->t_info_icon.ui1_align = WGL_AS_LEFT_TOP;
    pt_ctx->e_imgs_type_src_icon = WGL_IMG_SET_BASIC;
    x_memset(&pt_ctx->t_imgs_icon, 0, sizeof(WGL_IMG_EXT_SET_T));
    
    *ppv_ctx_bdr = (WGL_BORDER_CONTEXT_HANDLE_T)pt_ctx;
    return WGLR_OK;
}

INT32 wgl_bdr_timg_inst_destroy(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr)
{
    HFONT_T          h_font;
    BDR_TIMG_CTX_T   *pt_ctx = (BDR_TIMG_CTX_T *)pv_ctx_bdr;
    INT32            i4_ret;
    
    h_font = WGL_STR_GET_FONT(&pt_ctx->t_str_ttl);
    if(h_font!=NULL_HANDLE)
    {
        WGL_FONT_FREE(h_font);
        INFORM_ERR(wgl_str_destroy(&pt_ctx->t_str_ttl));
    }

    if(pt_ctx->h_ttl_fnt_ex != NULL_HANDLE)
    {
        WGL_FONT_FREE(pt_ctx->h_ttl_fnt_ex);
    }

    WGL_MEM_FREE(pt_ctx);

    return WGLR_OK;
}

INT32 wgl_bdr_timg_paint(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    GL_HGRAPHICS_T                      h_g,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    at_upt_rect,
    UINT16                              ui2_state)
{
    BDR_TIMG_CTX_T  *pt_ctx = (BDR_TIMG_CTX_T *)pv_ctx_bdr;
    GL_RECT_T       t_rc_cnt, t_rc_upt;
    INT32           i4_ret;
    
    /* check for quick return */
    if(!IS_STATE_VISIBLE(ui2_state) ||
       !RECT_IS_EFFECTIVE(pt_ctx->t_rc_widget))    
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
    
    /* prepare content rect */
    WGL_RECT_COPY(&t_rc_cnt, &pt_ctx->t_rc_widget);
    _RECT_INSET(&t_rc_cnt, &pt_ctx->t_inset_bdr);

    /* draw background */
    if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_BDR_BK))
    {
    }   
    else
    {
        if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_IMG_UI))
        {
            /* fill background color */
            GL_COLOR_T  *pt_clr_bk;
            
            _bdr_timg_tool_select_color(&pt_ctx->t_clrs_bk, ui2_state, &pt_clr_bk);

            if (!wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_bk))
            {
                if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR))
                {
                    INFORM_ERR(x_gl_set_color(h_g, pt_clr_bk));
                    INFORM_ERR(x_gl_fill_rect(h_g, &pt_ctx->t_rc_widget));
                }
                else if(RECT_IS_EFFECTIVE(t_rc_cnt) && !WGL_RECT_IS_INCLUDED(&t_rc_cnt, &t_rc_upt))
                {
                    INFORM_ERR(_bdr_timg_tool_bdr_area_fill_color(&pt_ctx->t_rc_widget, &pt_ctx->t_inset_bdr, h_g, pt_clr_bk));
                }
            }
        }        
        else
        {
            WGL_HIMG_TPL_T      h_img_bk;
            
            /* draw background image */
            _bdr_timg_tool_select_img(&pt_ctx->t_imgs_bk, ui2_state, &h_img_bk);
            if(h_img_bk!=NULL_HANDLE)
            {
                if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR))
                {
                    INFORM_ERR(wgl_img_tpl_paint(h_img_bk, &pt_ctx->t_rc_widget, &t_rc_upt, h_g, &pt_ctx->t_cvsst, WGL_TPL_PAINT_COMPOSE));
                }
                else if(RECT_IS_EFFECTIVE(t_rc_cnt) && !WGL_RECT_IS_INCLUDED(&t_rc_cnt, &t_rc_upt))
                {
                    INFORM_ERR(_bdr_timg_tool_bdr_area_fill_img(&pt_ctx->t_rc_widget, &pt_ctx->t_inset_bdr, &t_rc_upt, h_g, &pt_ctx->t_cvsst, h_img_bk));
                }
            }
        }
    }     
    
    /* draw icon */
    INFORM_ERR(_bdr_timg_tool_draw_icon(pt_ctx, h_g, &pt_ctx->t_cvsst, ui2_state, &t_rc_upt));
    
    /* draw title */
    if(WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl) > 0)
    {
        CHECK_VALID(WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)!=NULL_HANDLE, WGLR_INTERNAL_ERROR);
        INFORM_ERR(_bdr_timg_tool_draw_title(pt_ctx, h_g, ui2_state, &t_rc_upt));
    }

    return WGLR_OK;    
}

INT32 wgl_bdr_timg_cmd_proc(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    UINT32                              ui4_cmd_id, 
    VOID                                *pv_param1, 
    VOID                                *pv_param2)
{
    BDR_TIMG_CTX_T      *pt_ctx = (BDR_TIMG_CTX_T *)pv_ctx_bdr;
    INT32               i4_ret, i4_ret_val;
    
    if(!(WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_BASE, ui4_cmd_id) ||
         WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_TIMG, ui4_cmd_id)) )
    {
        INFORM_ERR(WGLR_INV_CMD);
        return WGLR_INV_CMD;
    }
    
    i4_ret_val = WGLR_OK;

    switch(ui4_cmd_id) 
    {
        /*Internal Border Commands*/
        case WGL_BDR_GL_SET_SIZE:
            i4_ret_val = _bdr_timg_cmd_hdr_set_size(pt_ctx, pv_param1, pv_param2);
            break;
        
        /*Common Border Commands*/
        case WGL_CMD_GL_SET_INSET:
            i4_ret_val = _bdr_timg_cmd_hdr_set_inset(pt_ctx, pv_param1);
            break;
        
        case WGL_CMD_GL_GET_INSET:
            i4_ret_val = _bdr_timg_cmd_hdr_get_inset(pt_ctx, pv_param1);
            break;
            
        case WGL_CMD_GL_GET_MIN_INSET:
            i4_ret_val = _bdr_timg_cmd_hdr_get_min_inset(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR:
            i4_ret_val = _bdr_timg_cmd_hdr_set_is_bdr_draw_interior(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR:
            i4_ret_val = _bdr_timg_cmd_hdr_get_is_bdr_draw_interior(pt_ctx, pv_param1);
            break;

        case WGL_CMD_GL_SET_BDR_COLOR: /*pv_param1: (UINT8)target, pv_param2:(WGL_COLOR_INFO_T *)*pt_color_info*/
            i4_ret_val = _bdr_timg_cmd_hdr_set_bdr_color(pt_ctx, pv_param1, pv_param2);
            break;

        case WGL_CMD_GL_GET_BDR_COLOR:
            i4_ret_val = _bdr_timg_cmd_hdr_get_bdr_color(pt_ctx, pv_param1, pv_param2);
            break;
        case WGL_CMD_GL_SET_BDR_TEXT_DIRECTION:
        {
            i4_ret_val = _bdr_timg_cmd_hdr_set_text_direction(pt_ctx, WGL_POINTER_TO_UINT8(pv_param1));
            break;
        }
        case WGL_CMD_GL_GET_BDR_TEXT_DIRECTION:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            i4_ret_val = _bdr_timg_cmd_hdr_get_text_direction(pt_ctx, (UINT8*)pv_param1);
            break;
        }
        /* Specific Border Commands */
        case WGL_CMD_BDR_TIMG_SET_STYLE:
            i4_ret_val = _bdr_timg_cmd_hdr_set_style(pt_ctx, pv_param1);
            break;
            
        case WGL_CMD_BDR_TIMG_GET_STYLE:
            i4_ret_val = _bdr_timg_cmd_hdr_get_style(pt_ctx, pv_param1);
            break;
            
        case WGL_CMD_BDR_TIMG_SET_BK_IMG:
            i4_ret_val = _bdr_timg_cmd_hdr_set_bk_img(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_GET_BK_IMG:
            i4_ret_val = _bdr_timg_cmd_hdr_get_bk_img(pt_ctx, pv_param1);
            break;        
            
        case WGL_CMD_BDR_TIMG_SET_TITLE_INFO:
            i4_ret_val = _bdr_timg_cmd_hdr_set_title_info(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_GET_TITLE_INFO:
            i4_ret_val = _bdr_timg_cmd_hdr_get_title_info(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_SET_TITLE_TEXT:
            i4_ret_val = _bdr_timg_cmd_hdr_set_title_text(pt_ctx, pv_param1, pv_param2);
            break;        

        case WGL_CMD_BDR_TIMG_GET_TITLE_TEXT:
            i4_ret_val = _bdr_timg_cmd_hdr_get_title_text(pt_ctx, pv_param1, pv_param2);
            break;        

        case WGL_CMD_BDR_TIMG_SET_ICON_INFO:
            i4_ret_val = _bdr_timg_cmd_hdr_set_icon_info(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_GET_ICON_INFO:
            i4_ret_val = _bdr_timg_cmd_hdr_get_icon_info(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_SET_ICON:
            i4_ret_val = _bdr_timg_cmd_hdr_set_icon(pt_ctx, pv_param1);
            break;        

        case WGL_CMD_BDR_TIMG_GET_ICON:
            i4_ret_val = _bdr_timg_cmd_hdr_get_icon(pt_ctx, pv_param1);
            break; 

        case WGL_CMD_BDR_TIMG_SET_TITLE_INFO_EX:
            i4_ret_val = _bdr_timg_cmd_hdr_set_title_info_ex(pt_ctx, pv_param1);            
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
 * Name: _bdr_timg_cmd_hdr_set_size
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
static INT32 _bdr_timg_cmd_hdr_set_size(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    IN      VOID                        *pv_param2)
{
    INT32       i4_size_w = (INT32)pv_param1;
    INT32       i4_size_h = (INT32)pv_param2;
    SET_RECT_BY_SIZE(&pt_ctx->t_rc_widget, 0, 0, i4_size_w, i4_size_h);
    
    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_inset
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
static INT32 _bdr_timg_cmd_hdr_set_inset(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_INSET_T     *pt_inset = (WGL_INSET_T *)pv_param1;
    
    if(pt_inset==NULL)
    {
        WGL_COPY_INSET(&pt_ctx->t_inset_bdr, &BDR_TIMG_DFT_INSET);
    }
    else
    {
        CHECK_VALID(_INSET_IS_VALID(*pt_inset), WGLR_INV_ARG);
        WGL_COPY_INSET(&pt_ctx->t_inset_bdr, pt_inset);
    }    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_inset
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
static INT32 _bdr_timg_cmd_hdr_get_inset(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_INSET_T *pt_inset_out = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_out, WGLR_INV_ARG);
    _INSET_COPY(pt_inset_out, &pt_ctx->t_inset_bdr);
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_min_inset
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
static INT32 _bdr_timg_cmd_hdr_get_min_inset(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    
    WGL_INSET_T *pt_inset_out = (WGL_INSET_T *)pv_param1;
    
    CHECK_NULL(pt_inset_out, WGLR_INV_ARG);
    _INSET_COPY(pt_inset_out, &BDR_TIMG_MIN_INSET);
        
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_is_bdr_draw_interior
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
static INT32 _bdr_timg_cmd_hdr_set_is_bdr_draw_interior(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    BOOL    b_draw = WGL_POINTER_TO_BOOL(pv_param1);
    
    CHECK_VALID(b_draw==TRUE || b_draw==FALSE, WGLR_INV_ARG);
    
    if(b_draw)
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR);
    }
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_is_bdr_draw_interior
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
static INT32 _bdr_timg_cmd_hdr_get_is_bdr_draw_interior(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    BOOL    *pb_draw = (BOOL *)pv_param1;
    
    CHECK_NULL(pb_draw, WGLR_INV_ARG);
    *pb_draw = HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_DRAW_INTR) ? TRUE : FALSE;
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_bdr_color
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
static INT32 _bdr_timg_cmd_hdr_set_bdr_color(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
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
            case WGL_CLR_BDR_BK:
                _bdr_timg_tool_load_sys_def_clrs_bk(pt_ctx);
            break;
            
            case WGL_CLR_BDR_TIMG_TITLE_TEXT:
                _bdr_timg_tool_load_sys_def_clrs_ttl_text(pt_ctx);
            break;
            
            case WGL_CLR_BDR_TIMG_TITLE_TEXT_BK:
                _bdr_timg_tool_load_sys_def_clrs_ttl_text_bk(pt_ctx);
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
            case WGL_CLR_BDR_BK:
                pt_ctx->e_clrs_type_src_bk = pt_clr_info->e_type;
                _bdr_timg_tool_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_bk, pt_clr_info);
            break;
            
            case WGL_CLR_BDR_TIMG_TITLE_TEXT:
                pt_ctx->e_clrs_type_src_ttl = pt_clr_info->e_type;
                _bdr_timg_tool_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_ttl, pt_clr_info);
            break;
            
            case WGL_CLR_BDR_TIMG_TITLE_TEXT_BK:
                pt_ctx->e_clrs_type_src_ttl_bk = pt_clr_info->e_type;
                _bdr_timg_tool_copy_from_color_info_to_extend_color_set(&pt_ctx->t_clrs_ttl_bk, pt_clr_info);
            break;
             
            default:
                INFORM_ERR(WGLR_INV_ARG);
                return WGLR_INV_ARG;
        }        
    }
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_bdr_color
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
static INT32 _bdr_timg_cmd_hdr_get_bdr_color(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    UINT8            ui1_target       = WGL_POINTER_TO_UINT8(pv_param1);
    WGL_COLOR_INFO_T *pt_clr_info_out = (WGL_COLOR_INFO_T *)pv_param2;
    INT32            i4_ret; 
    
    switch(ui1_target)
    {
        case WGL_CLR_BDR_BK:
            _bdr_timg_tool_copy_from_extend_color_set_to_color_info(pt_clr_info_out, pt_ctx->e_clrs_type_src_bk, &pt_ctx->t_clrs_bk);
        break;
        
        case WGL_CLR_BDR_TIMG_TITLE_TEXT:
            _bdr_timg_tool_copy_from_extend_color_set_to_color_info(pt_clr_info_out, pt_ctx->e_clrs_type_src_ttl, &pt_ctx->t_clrs_ttl);
        break;
        
        case WGL_CLR_BDR_TIMG_TITLE_TEXT_BK:
            _bdr_timg_tool_copy_from_extend_color_set_to_color_info(pt_clr_info_out, pt_ctx->e_clrs_type_src_ttl_bk, &pt_ctx->t_clrs_ttl_bk);
        break;
        
        default:
            INFORM_ERR(WGLR_INV_ARG);
            return WGLR_INV_ARG;
    }            
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_text_direction
 *
 * Description: This command handler sets the target text direction  
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          ui1_text_dir   Contains the text direction
 * Outputs: -
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG     The text direction is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_text_direction(
    IN OUT  BDR_TIMG_CTX_T               *pt_ctx, 
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

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_text_direction
 *
 * Description: This command handler gets the target text direction info 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pui1_text_dir    Contains the text direction info, 
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_text_direction(
    IN OUT  BDR_TIMG_CTX_T               *pt_ctx, 
    OUT      UINT8*                       pui1_text_dir 
    )
{
    (*pui1_text_dir) = pt_ctx->ui1_text_direction;
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_style
 *
 * Description: This command handler sets the styles 
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UINT32) Contains the styles of the border
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK        Routine successful.
 *          WGLR_INV_ARG   The styles combination is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_style(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    UINT32 ui4_style = (UINT32)pv_param1;
    
    CHECK_VALID((ui4_style & (~((WGL_STL_BDR_TIMG_LAST_STYLE<<1)-1))) == 0, WGLR_INV_ARG );
    
    if(HAS_FLAG(ui4_style, WGL_STL_BDR_TIMG_NO_BK))
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_BDR_BK);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_BDR_BK);
    }
    
    if(HAS_FLAG(ui4_style, WGL_STL_BDR_TIMG_NO_IMG_UI))
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_IMG_UI);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_IMG_UI);
    }

    if(HAS_FLAG(ui4_style, WGL_STL_BDR_TIMG_NO_SMART_CUT))
    {
        WGL_SET_FLAG(pt_ctx->ui2_attr, WGL_STL_BDR_TIMG_NO_SMART_CUT);
    }
    else
    {
        WGL_CLEAR_FLAG(pt_ctx->ui2_attr, WGL_STL_BDR_TIMG_NO_SMART_CUT);
    }
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_style
 *
 * Description: This command handler gets the styles
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (UINT32 *) Returns the styles
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_style(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    UINT32 *pui4_style_out = (UINT32 *)pv_param1;
    CHECK_NULL(pui4_style_out, WGLR_INV_ARG);
    
    *pui4_style_out = 0;
    if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_IMG_UI))
    {
        WGL_SET_FLAG(*pui4_style_out, WGL_STL_BDR_TIMG_NO_IMG_UI);
    }
    if(HAS_FLAG(pt_ctx->ui2_attr, BDR_TIMG_ATTR_NO_BDR_BK))
    {
        WGL_SET_FLAG(*pui4_style_out, WGL_STL_BDR_TIMG_NO_BK);
    }
    
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_bk_img
 *
 * Description: This command handler sets the background image set
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (WGL_IMG_INFO_T *) Contains the background image set
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK        Routine successful.
 *          WGLR_INV_ARG   The image set info is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_bk_img(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_IMG_INFO_T *pt_img_info = (WGL_IMG_INFO_T *)pv_param1;
    
    if(pt_img_info==NULL)
    {
        pt_ctx->e_imgs_type_src_bk = WGL_IMG_SET_BASIC;
        x_memset(&pt_ctx->t_imgs_bk, 0, sizeof(WGL_IMG_EXT_SET_T));
    }    
    else
    {
        CHECK_VALID(IS_LEGAL_IMG_SET_TYPE(pt_img_info->e_type), WGLR_INV_ARG);
        pt_ctx->e_imgs_type_src_bk = pt_img_info->e_type;
        _bdr_timg_tool_copy_from_img_info_to_extend_img_set(&pt_ctx->t_imgs_bk, pt_img_info);
    }        
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_bk_img
 *
 * Description: This command handler gets the background image set
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_IMG_INFO_T *) Returns the background image set
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_bk_img(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_IMG_INFO_T  *pt_img_info = (WGL_IMG_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_img_info, WGLR_INV_ARG);
    _bdr_timg_tool_copy_from_extend_img_set_to_img_info(pt_img_info, pt_ctx->e_imgs_type_src_bk, &pt_ctx->t_imgs_bk);

    return WGLR_OK;
}        

static INT32 _bdr_timg_cmd_hdr_set_title_info_ex(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_BDR_TIMG_TITLE_INFO_T   *pt_info = (WGL_BDR_TIMG_TITLE_INFO_T *)pv_param1;
    WGL_BDR_TIMG_TITLE_INFO_T   t_info;
    HFONT_T                     h_font;
    INT32                       i4_ret;
    
    CHECK_NULL(pt_info, WGLR_INV_ARG);
    
    x_memcpy(&t_info, &pt_ctx->t_info_ttl, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));
    x_memcpy(&t_info.t_font, &pt_info->t_font, sizeof(WGL_FONT_INFO_T));

    if(x_strlen(t_info.t_font.a_c_font_name)==0)
    {
        x_strcpy(t_info.t_font.a_c_font_name, SN_FONT_DEFAULT);
    } 
    
    WGL_FONT_CREATE (&t_info.t_font, &h_font);
    CHECK_VALID(h_font!=NULL_HANDLE, WGLR_INV_ARG);

    if (t_info.t_font.i2_width != 0)
    {
        INFORM_ERR (x_fe_set_fixed_advance (h_font, t_info.t_font.i2_width, 0));
    }
    
    x_memcpy(&pt_ctx->t_info_ttl_ex, &t_info, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));
    pt_ctx->h_ttl_fnt_ex = h_font;

    return WGLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_title_info
 *
 * Description: This command handler sets the title information
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (WGL_BDR_TIMG_TITLE_INFO_T *) Contains the title info
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  (1) pv_param1 is NULL
 *                        (2) The title info is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_title_info(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_BDR_TIMG_TITLE_INFO_T   *pt_info = (WGL_BDR_TIMG_TITLE_INFO_T *)pv_param1;
    WGL_BDR_TIMG_TITLE_INFO_T   t_info;
    HFONT_T                     h_font;
    INT32                       i4_ret;
    
    CHECK_NULL(pt_info, WGLR_INV_ARG);
    CHECK_VALID(
        !(pt_info->ui2_style & (~((WGL_BDR_TIMG_TITLE_STL_LAST_STYLE<<1)-1))) &&
        _RECT_IS_VALID(pt_info->t_rc_ref)                                     &&
        WGL_IS_VALID_ALIGN(pt_info->ui1_align), 
        WGLR_INV_ARG);
    
    x_memcpy(&t_info, pt_info, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));
    if(x_strlen(t_info.t_font.a_c_font_name)==0)
    {
        x_strcpy(t_info.t_font.a_c_font_name, SN_FONT_DEFAULT);
    }        
    
    if( WGL_STR_GET_FONT(&pt_ctx->t_str_ttl)==NULL_HANDLE ||
       !wgl_str_check_font(&pt_ctx->t_str_ttl, &t_info.t_font))
    {
        /* the different font */
        WGL_FONT_CREATE (&t_info.t_font, &h_font);
        CHECK_VALID(h_font!=NULL_HANDLE, WGLR_INV_ARG);

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
    
    x_memcpy(&pt_ctx->t_info_ttl, &t_info, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));

    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_title_info
 *
 * Description: This command handler gets the title information
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_BDR_TIMG_TITLE_INFO_T *) Returns the title info
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_title_info(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_BDR_TIMG_TITLE_INFO_T   *pt_info_ttl_out = (WGL_BDR_TIMG_TITLE_INFO_T *)pv_param1;
    CHECK_NULL(pt_info_ttl_out, WGLR_INV_ARG);
    
    x_memcpy(pt_info_ttl_out, &pt_ctx->t_info_ttl, sizeof(WGL_BDR_TIMG_TITLE_INFO_T));
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_title_text
 *
 * Description: This command handler sets the title text
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UTF16_T *) Contains the UTF16_T string of the title text, 
 *                      if NULL, the title text will be cleared.
 *          pv_param2   (UINT16) Contains the string length. 
 *                      if 0, the title text will be cleared.
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  pv_param1 is NULL but pv_param2 is not 0
 *          <0            Other errors
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_title_text(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
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
        CHECK_VALID(h_font != NULL_HANDLE, WGLR_INV_ARG);
        
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
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_title_text
 *
 * Description: This command handler gets the title text
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (UTF16_T *) Contains the pointer of a UTF16_T buffer 
 *                      to receive the title text
 *          pv_param2   (UINT16) Contains the size of the buffer
 *
 * Outputs: pv_param1   (UTF16_T *) Returns the title string
 *                    
 * Returns: WGLR_OK                 Routine successful.
 *          WGLR_INV_ARG            The pv_param1 is NULL
 *          WGLR_BUFFER_NOT_ENOUGH  The buffer is not big enough
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_title_text(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1,
    OUT     VOID                        *pv_param2)
{
    UTF16_T     *w2s_ttl_text_out = (UTF16_T *)pv_param1;
    UINT16      ui2_size_buf      = WGL_POINTER_TO_UINT16(pv_param2);
    
    CHECK_NULL(w2s_ttl_text_out, WGLR_INV_ARG);
    CHECK_VALID(ui2_size_buf > WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl), WGLR_BUFFER_NOT_ENOUGH);
    
    x_uc_w2s_strncpy(w2s_ttl_text_out, WGL_STR_GET_TEXT(&pt_ctx->t_str_ttl), WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl));
    x_uc_w2s_set(w2s_ttl_text_out, WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl), (UTF16_T)0);
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_icon_info
 *
 * Description: This command handler sets the icon information
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (WGL_BDR_TIMG_ICON_INFO_T *) Contains the icon info
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK       Routine successful.
 *          WGLR_INV_ARG  (1) pv_param1 is NULL
 *                        (2) the icon info is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_icon_info(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_BDR_TIMG_ICON_INFO_T    *pt_info = (WGL_BDR_TIMG_ICON_INFO_T *)pv_param1;
    
    CHECK_NULL(
        pt_info, WGLR_INV_ARG);
    CHECK_VALID( 
        WGL_IS_VALID_ALIGN(pt_info->ui1_align)  &&
        _RECT_IS_VALID(pt_info->t_rc_ref), WGLR_INV_ARG);
    
    x_memcpy(&pt_ctx->t_info_icon, pt_info, sizeof(WGL_BDR_TIMG_ICON_INFO_T));
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_icon_info
 *
 * Description: This command handler gets the icon information
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_BDR_TIMG_ICON_INFO_T *) Contains the icon info
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_icon_info(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_BDR_TIMG_ICON_INFO_T    *pt_info_out = (WGL_BDR_TIMG_ICON_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_info_out, WGLR_INV_ARG);
    x_memcpy(pt_info_out, &pt_ctx->t_info_icon, sizeof(WGL_BDR_TIMG_ICON_INFO_T));
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_set_icon
 *
 * Description: This command handler sets the image info of the icon
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *          pv_param1   (WGL_IMG_INFO_T *) Contains the image info of the icon
 *                      if NULL, the image info of the icon will be cleared
 * Outputs: -
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The image info is invalid
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_set_icon(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx,
    IN      VOID                        *pv_param1)
{
    WGL_IMG_INFO_T *pt_img_info = (WGL_IMG_INFO_T *)pv_param1;
    
    if(pt_img_info==NULL)
    {
        pt_ctx->e_imgs_type_src_icon = WGL_IMG_SET_BASIC;
        x_memset(&pt_ctx->t_imgs_icon, 0, sizeof(WGL_IMG_EXT_SET_T));
    }
    else
    {   
        CHECK_VALID(IS_LEGAL_IMG_SET_TYPE(pt_img_info->e_type), WGLR_INV_ARG);
        pt_ctx->e_imgs_type_src_icon = pt_img_info->e_type;
        _bdr_timg_tool_copy_from_img_info_to_extend_img_set(&pt_ctx->t_imgs_icon, pt_img_info);
    }
    
    return WGLR_OK;
}        
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_cmd_hdr_get_icon
 *
 * Description: This command handler gets the image info of the icon
 *
 * Inputs:  pt_ctx      Contains the context of the border
 *
 * Outputs: pv_param1   (WGL_IMG_INFO_T *) Returns the image info of the icon
 *                    
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_ARG    The pv_param1 is NULL
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_cmd_hdr_get_icon(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    OUT     VOID                        *pv_param1)
{
    WGL_IMG_INFO_T *pt_img_info_out = (WGL_IMG_INFO_T *)pv_param1;
    
    CHECK_NULL(pt_img_info_out, WGLR_INV_ARG);
    _bdr_timg_tool_copy_from_extend_img_set_to_img_info(pt_img_info_out, pt_ctx->e_imgs_type_src_icon, &pt_ctx->t_imgs_icon);
    
    return WGLR_OK;
}        


/*-----------------------------------------------------------------------------
                    internal functions definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_bdr_area_fill_color
 *
 * Description: This function fill the border area with some specified color
 *
 * Inputs:  pt_rc_wgt     Contains the widget rect
 *          pt_inset_bdr  Contains the border inset
 *          h_g           Contains the handle of graphics object
 *          pt_clr        Contains the filling color 
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_tool_bdr_area_fill_color(
    IN      GL_RECT_T                   *pt_rc_wgt,
    IN      WGL_INSET_T                 *pt_inset_bdr,
    IN      GL_HGRAPHICS_T              h_g, 
    IN      GL_COLOR_T                  *pt_clr)
{
    INT32       i4_ret;
    GL_RECT_T   t_rc_temp;
    
    INFORM_ERR(x_gl_set_color(h_g, pt_clr));
    
    /* clear left */
    SET_RECT(&t_rc_temp, 0, 0, pt_inset_bdr->i4_left, pt_rc_wgt->i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* clear top */
    SET_RECT(&t_rc_temp, 0, 0, pt_rc_wgt->i4_right, pt_inset_bdr->i4_top);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* clear right */
    SET_RECT(&t_rc_temp, pt_rc_wgt->i4_right - pt_inset_bdr->i4_right, 0, pt_rc_wgt->i4_right, pt_rc_wgt->i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    /* clear bottom */
    SET_RECT(&t_rc_temp, 0, pt_rc_wgt->i4_bottom - pt_inset_bdr->i4_bottom, pt_rc_wgt->i4_right, pt_rc_wgt->i4_bottom);
    if(RECT_IS_EFFECTIVE(t_rc_temp))
    {
        INFORM_ERR(x_gl_fill_rect(h_g, &t_rc_temp));
    }
    
    return WGLR_OK;
}   
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_bdr_area_fill_img
 *
 * Description: This function fill the border area with some specified template image
 *
 * Inputs:  pt_rc_wgt     Contains the widget rect
 *          pt_inset_bdr  Contains the border inset
 *          pt_rc_upt     Contains the update rect
 *          h_g           Contains the handle of graphics object
 *          pt_cvsst      Contains the status of the current hosted canvas.
 *          h_img         Contains the template image
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK             Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_tool_bdr_area_fill_img(
    IN      GL_RECT_T                   *pt_rc_wgt,
    IN      WGL_INSET_T                 *pt_inset_bdr,
    IN      GL_RECT_T                   *pt_rc_upt,
    IN      GL_HGRAPHICS_T              h_g, 
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      WGL_HIMG_TPL_T              h_img)
{
    INT32       i4_ret;
    GL_RECT_T   at_rc_area[4], t_rc_render;
    UINT16      ui2_i;

    /* left border area */
    SET_RECT(&at_rc_area[0], 0, 0, pt_inset_bdr->i4_left, pt_rc_wgt->i4_bottom);
    /* top border area */
    SET_RECT(&at_rc_area[1], pt_inset_bdr->i4_left, 0, pt_rc_wgt->i4_right - pt_inset_bdr->i4_right, pt_inset_bdr->i4_top);
    /* right border area */
    SET_RECT(&at_rc_area[2], pt_rc_wgt->i4_right - pt_inset_bdr->i4_right, 0, pt_rc_wgt->i4_right, pt_rc_wgt->i4_bottom);
    /* bottom border area */
    SET_RECT(&at_rc_area[3], pt_inset_bdr->i4_left, pt_rc_wgt->i4_bottom - pt_inset_bdr->i4_bottom, pt_rc_wgt->i4_right - pt_inset_bdr->i4_right, pt_rc_wgt->i4_bottom);

    for(ui2_i=0; ui2_i<4; ui2_i++)
    {
        if(RECT_IS_EFFECTIVE(at_rc_area[ui2_i]))
        {
            INTERSECT_RECT(&t_rc_render, &at_rc_area[ui2_i], pt_rc_upt);
            if(RECT_IS_EFFECTIVE(t_rc_render))
            {
                INFORM_ERR(wgl_img_tpl_paint(h_img, pt_rc_wgt, &t_rc_render, h_g, pt_cvsst, WGL_TPL_PAINT_COMPOSE));
            }
        }
    }

    return WGLR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_copy_from_color_info_to_extend_color_set
 *
 * Description: This tool function copy extended color set from an color info
 *
 * Inputs:  pt_clr_info_from  Contains the source color info
 *
 * Outputs: pt_clrs_ex        Contains the result extended color set
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_copy_from_color_info_to_extend_color_set(
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
 * Name: _bdr_timg_tool_copy_from_extend_color_set_to_color_info
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
static VOID _bdr_timg_tool_copy_from_extend_color_set_to_color_info( 
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
 * Name: _bdr_timg_tool_copy_from_img_info_to_extend_img_set
 *
 * Description: This tool function copy extended image set from an image info object
 *
 * Inputs:  pt_img_info_from  Contains the source image info
 *
 * Outputs: pt_imgs_ex        Contains the result extended image set
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_copy_from_img_info_to_extend_img_set(
    OUT     WGL_IMG_EXT_SET_T           *pt_imgs_ex,
    IN      WGL_IMG_INFO_T              *pt_img_info_from)
{
    switch(pt_img_info_from->e_type)
    {
        case WGL_IMG_SET_BASIC:
            pt_imgs_ex->t_enable             = pt_img_info_from->u_img_data.t_basic.t_enable;
            pt_imgs_ex->t_disable            = pt_img_info_from->u_img_data.t_basic.t_disable;
            pt_imgs_ex->t_highlight          = pt_img_info_from->u_img_data.t_basic.t_enable;
            pt_imgs_ex->t_highlight_inactive = pt_img_info_from->u_img_data.t_basic.t_enable;
            pt_imgs_ex->t_push               = pt_img_info_from->u_img_data.t_basic.t_enable;
            pt_imgs_ex->t_push_unhighlight   = pt_img_info_from->u_img_data.t_basic.t_enable;
            break;
        case WGL_IMG_SET_STANDARD:
            pt_imgs_ex->t_enable             = pt_img_info_from->u_img_data.t_standard.t_enable;
            pt_imgs_ex->t_disable            = pt_img_info_from->u_img_data.t_standard.t_disable;
            pt_imgs_ex->t_highlight          = pt_img_info_from->u_img_data.t_standard.t_highlight;
            pt_imgs_ex->t_highlight_inactive = pt_img_info_from->u_img_data.t_standard.t_enable;
            pt_imgs_ex->t_push               = pt_img_info_from->u_img_data.t_standard.t_highlight;
            pt_imgs_ex->t_push_unhighlight   = pt_img_info_from->u_img_data.t_standard.t_enable;
            break;
        case WGL_IMG_SET_EXTEND:
            x_memcpy(pt_imgs_ex, &pt_img_info_from->u_img_data.t_extend, sizeof(WGL_IMG_EXT_SET_T));
            break;
    }
}    

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_copy_from_extend_img_set_to_img_info
 *
 * Description: This tool function copy image info from an extended image set
 *
 * Inputs:  pt_imgs_ex_from   Contains the source extended image set
 *          e_imgs_type       Contains the target image set type 
 *
 * Outputs: pt_img_info_out   Contains the result image info
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_copy_from_extend_img_set_to_img_info( 
    OUT     WGL_IMG_INFO_T              *pt_img_info_out, 
    IN      WGL_IMG_SET_TYPE_T          e_imgs_type,
    IN      WGL_IMG_EXT_SET_T           *pt_imgs_ex_from)
{
    pt_img_info_out->e_type = e_imgs_type;
    switch(e_imgs_type)
    {
        case WGL_IMG_SET_BASIC:
            pt_img_info_out->u_img_data.t_basic.t_enable       = pt_imgs_ex_from->t_enable;
            pt_img_info_out->u_img_data.t_basic.t_disable      = pt_imgs_ex_from->t_disable;
        break;
        
        case WGL_IMG_SET_STANDARD:
            pt_img_info_out->u_img_data.t_standard.t_enable    = pt_imgs_ex_from->t_enable;
            pt_img_info_out->u_img_data.t_standard.t_disable   = pt_imgs_ex_from->t_disable;
            pt_img_info_out->u_img_data.t_standard.t_highlight = pt_imgs_ex_from->t_highlight;
        break;
        
        case WGL_IMG_SET_EXTEND:
            x_memcpy(&pt_img_info_out->u_img_data.t_extend, pt_imgs_ex_from, sizeof(WGL_IMG_EXT_SET_T));
        break;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_load_sys_def_clrs_bk
 *
 * Description: This tool function loads the system default background colors
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_load_sys_def_clrs_bk(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx)
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
 * Name: _bdr_timg_tool_load_sys_def_clrs_ttl_text
 *
 * Description: This tool function loads the default title text colors
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_load_sys_def_clrs_ttl_text(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx)
{
    pt_ctx->e_clrs_type_src_ttl = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_FG_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl.t_push_unhighlight);
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_load_sys_def_clrs_ttl_text_bk
 *
 * Description: This tool function loads the default title text background colors
 *
 * Inputs:  pt_ctx      Contains the border context
 *
 * Outputs: pt_ctx      Contains the result border context
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_load_sys_def_clrs_ttl_text_bk(
    IN OUT  BDR_TIMG_CTX_T              *pt_ctx)
{
    pt_ctx->e_clrs_type_src_ttl_bk = WGL_COLOR_SET_EXTEND;
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_enable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_DIS, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_disable);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_highlight);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_HT_INACT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_highlight_inactive);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_push);
    wgl_get_system_color(WGL_SYS_CLR_BDR_BK_PUSHED_UNHT, &pt_ctx->t_cvsst, &pt_ctx->t_clrs_ttl_bk.t_push_unhighlight);
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_select_color
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
static VOID _bdr_timg_tool_select_color(
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
 * Name: _bdr_timg_tool_select_img
 *
 * Description: This tool function select an image according to a certain state
 *
 * Inputs:  pt_imgs_src  Contains the source extended image set
 *          ui2_state    Specifies the state
 *
 * Outputs: pph_img_out  Contains the image corresponding to the specified state
 *                    
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _bdr_timg_tool_select_img(
    IN      WGL_IMG_EXT_SET_T           *pt_imgs_src,
    IN      UINT16                      ui2_state, 
    OUT     WGL_HIMG_TPL_T              *ph_img_out)
{
    if(IS_STATE_PUSHED(ui2_state))
    {
        if(IS_STATE_FOCUSED(ui2_state))
        {
            *ph_img_out = pt_imgs_src->t_push;
        }
        else
        {
            *ph_img_out = pt_imgs_src->t_push_unhighlight;
        }
    }
    else if(IS_STATE_FOCUSED(ui2_state))
    {
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            *ph_img_out = pt_imgs_src->t_highlight;
        }
        else
        {
            *ph_img_out = pt_imgs_src->t_highlight_inactive;
        }
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        *ph_img_out = pt_imgs_src->t_enable;
    }
    else
    {
        *ph_img_out = pt_imgs_src->t_disable;
    }
    
}    
    
/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_draw_title
 *
 * Description: This tool function draw the title
 *
 * Inputs:  pt_ctx      Contains the border context
 *          h_g         Contains the GL_HGRAPHICS_T handle
 *          ui2_state   Specifies the widget state
 *          pt_rc_upt   Contains the update rect
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK     Routine successful.
 *          < 0         Other errors
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_tool_draw_title(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
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
    HFONT_T         h_font;
    UTF16_T         w2s_soft_hyphen[2] = {WGL_UTF16_SOFT_HYPHEN, 0};
	BOOL            b_smart_cut = FALSE;
	#ifdef ARAB_EXIST_SUPPORT
	BOOL            b_exist_arab = FALSE;
	UTF16_T*        w2s_arabic_text = NULL;
	UINT16          ui2_arabic_len  = 0;
	#endif
	
	WGL_RECT_COPY(&t_rc_ttl, &pt_ctx->t_rc_widget);
	WGL_RECT_INSET(&t_rc_ttl, pt_ctx->t_inset_bdr.i4_left, pt_ctx->t_inset_bdr.i4_top,
					pt_ctx->t_inset_bdr.i4_right, pt_ctx->t_inset_bdr.i4_bottom);
	
	
	#ifdef ARAB_EXIST_SUPPORT
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
	
    if(IS_STATE_FOCUSED(ui2_state) && pt_ctx->h_ttl_fnt_ex != NULL_HANDLE)
    {
        h_font = pt_ctx->h_ttl_fnt_ex;
    }
    else
    {
        h_font = WGL_STR_GET_FONT(&pt_ctx->t_str_ttl);
    }

	CATCH_FAIL(x_wgl_get_string_size(h_font,pt_ctx->t_str_ttl.w2s_text,pt_ctx->t_str_ttl.ui2_len_text,&i4_w,&i4_h));
	
	if (i4_w > RECT_W(&t_rc_ttl))
	{
		i4_w = RECT_W(&t_rc_ttl);
		b_smart_cut = TRUE;
	}
	
    _bdr_timg_tool_cal_obj_pos(
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
    	#ifdef ARAB_EXIST_SUPPORT
		if (w2s_arabic_text)
		{
			WGL_MEM_FREE(w2s_arabic_text);
		}
		#endif
        return WGLR_OK;
    }
    
    if(HAS_FLAG(pt_ctx->t_info_ttl.ui2_style, WGL_BDR_TIMG_TITLE_STL_TRANSP_BK))
    {
        wgl_cvsst_get_transparent_color(&pt_ctx->t_cvsst, &t_clr_clean);
        pt_clr_ttl_bk = &t_clr_clean;
        i4_bg_h       = 0;
    }
    else
    {
        _bdr_timg_tool_select_color(&pt_ctx->t_clrs_ttl_bk, ui2_state, &pt_clr_ttl_bk);   
        i4_bg_h = i4_h;
    }

    _bdr_timg_tool_select_color(&pt_ctx->t_clrs_ttl, ui2_state, &pt_clr_ttl);
    
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
        BOOL        b_draw_tail = FALSE;
        UINT16      ui2_bg_h;
        UINT32      ui4_len_body;
        INT32       i4_w_body;
        UTF16_T*    w2s_temp;
        UINT32      ui4_len_str_cut = 0;
        INT32       i4_width_cut = 0;
        INT32       i4_w_etc, i4_h_etc;
		
		i4_w_body = i4_w;
		
        ui4_len_body = (UINT32)WGL_STR_GET_TEXT_LEN(&pt_ctx->t_str_ttl);

		if(b_smart_cut )
        {
            /*The string width is larger than the box, get the string length
             *that in the box.*/

            /* calculate the width of "..." */
		    
            i4_ret = x_fe_get_string_size(h_font, 
                                          BDR_SMART_CUT_CHARS, 
                                          BDR_SMART_CUT_CHARS_LEN, 
                                          &i4_w_etc, 
                                          &i4_h_etc);

            if (!(i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
				#ifdef ARAB_EXIST_SUPPORT
				if (w2s_arabic_text)
				{
					WGL_MEM_FREE(w2s_arabic_text);
				}
				#endif
                return WGLR_OK;
            }

			b_draw_tail = TRUE;
		}
		
		#ifdef ARAB_EXIST_SUPPORT
		if (!b_exist_arab)
		#endif	
		{
	        i4_ret = wgl_get_max_char_in_box_ex (h_font,
	                                             WGL_STR_GET_TEXT(&pt_ctx->t_str_ttl),
	                                             (INT32) ui4_len_body,
	                                             TRUE,
	                                             i4_w_body,
	                                             NULL,
	                                             w2s_soft_hyphen,
	                                             1,
	                                             0,
	                                             TRUE,
	                                             & w2s_temp,
	                                             & ui4_len_str_cut,
	                                             & i4_width_cut);
		}
		#ifdef ARAB_EXIST_SUPPORT
		else
		{
			i4_ret = wgl_get_max_char_in_box_for_arab(pt_ctx->t_str_ttl.ui2_len_text,
													  h_font,
							    					  &w2s_arabic_text[pt_ctx->t_str_ttl.ui2_len_text - 1],
							    					  (INT32)ui2_arabic_len,
												      TRUE,
												      i4_w_body,
												      NULL,
												      w2s_soft_hyphen,
												      1,
												      0,
					    							  TRUE,
					    							  &w2s_temp,
					    							  &ui4_len_str_cut,
					    							  &i4_width_cut);
		}
		#endif
        if (i4_ret != WGLR_OK)
        {
        	#ifdef ARAB_EXIST_SUPPORT
			if (w2s_arabic_text)
			{
				WGL_MEM_FREE(w2s_arabic_text);
			}
			#endif
            INFORM_ERR (i4_ret);
			return i4_ret;
        }

		
        ui2_bg_h = wgl_cvsst_check_color_transparent(&pt_ctx->t_cvsst, pt_clr_ttl_bk) ? (UINT16)0 : (UINT16)i4_bg_h; 
#ifdef ARAB_EXIST_SUPPORT
		if (!b_exist_arab)
#endif	
		{

        INFORM_ERR(x_gl_draw_ustring_ex(h_g, 
								        i4_x,
								        i4_y,
								        i4_y,
								        WGL_STR_GET_TEXT(&pt_ctx->t_str_ttl),
								        (UINT16)ui4_len_str_cut,
								        NULL,
								        0,
								        h_font,
								        pt_clr_ttl,
								        pt_clr_ttl_bk,
								        NULL,
								        FALSE,
								        0,
								        0,
								        ui2_bg_h,
								        0));
		}
#ifdef ARAB_EXIST_SUPPORT
		else
		{
			INFORM_ERR(x_gl_draw_ustring_ex(h_g, 
								        i4_x,
								        i4_y,
								        i4_y,
								        w2s_arabic_text + pt_ctx->t_str_ttl.ui2_len_text - ui4_len_str_cut,
								        (UINT16)ui4_len_str_cut,
								        NULL,
								        0,
								        h_font,
								        pt_clr_ttl,
								        pt_clr_ttl_bk,
								        NULL,
								        FALSE,
								        0,
								        0,
								        ui2_bg_h,
								        0));
		}
#endif


        if( b_draw_tail == TRUE )
        {
            INFORM_ERR(x_gl_draw_ustring_ex(h_g, 
                                            i4_x + i4_w_body,
                                            i4_y,
                                            i4_y,
                                            BDR_SMART_CUT_CHARS,
                                            BDR_SMART_CUT_CHARS_LEN,
                                            NULL,
                                            0,
                                            h_font,
                                            pt_clr_ttl,
                                            pt_clr_ttl_bk,
                                            NULL,
                                            FALSE,
                                            0,
                                            0,
                                            ui2_bg_h,
                                            0));
        }
    }
	#ifdef ARAB_EXIST_SUPPORT
	if (w2s_arabic_text)
	{
		WGL_MEM_FREE(w2s_arabic_text);
	}
	#endif
    return WGLR_OK;   
}

/*-----------------------------------------------------------------------------
 * Name: _bdr_timg_tool_draw_icon
 *
 * Description: This tool function draw the icon
 *
 * Inputs:  pt_ctx      Contains the border context
 *          h_g         Contains the GL_HGRAPHICS_T handle
 *          pt_cvsst    Contains the status of the current hosted canvas.
 *          ui2_state   Specifies the widget state
 *          pt_rc_upt   Contains the update rect
 *
 * Outputs: -
 *                    
 * Returns: WGLR_OK     Routine successful.
 *          < 0         Other errors
 ----------------------------------------------------------------------------*/
static INT32 _bdr_timg_tool_draw_icon(
    IN      BDR_TIMG_CTX_T              *pt_ctx,
    IN      GL_HGRAPHICS_T              h_g,
    IN      WGL_CANVAS_STATUS_T         *pt_cvsst,
    IN      UINT16                      ui2_state, 
    IN      GL_RECT_T                   *pt_rc_upt)
{
    INT32               i4_ret;
    WGL_HIMG_TPL_T      h_icon;
    
    _bdr_timg_tool_select_img(&pt_ctx->t_imgs_icon, ui2_state, &h_icon);
    
    if(h_icon != NULL_HANDLE)
    {
        UINT32      ui4_w, ui4_h;
        INT32       i4_x, i4_y;
        GL_RECT_T   t_rc_icon;
        BOOL        b_need_update;
        
        CATCH_FAIL(x_wgl_img_tpl_get_size(h_icon, &ui4_w, &ui4_h));
        _bdr_timg_tool_cal_obj_pos(
            pt_ctx->ui1_text_direction,
            pt_ctx->t_info_icon.ui1_align, 
            (INT32)ui4_w, 
            (INT32)ui4_h, 
            &pt_ctx->t_info_icon.t_rc_ref,
            &i4_x,
            &i4_y);
        SET_RECT_BY_SIZE(&t_rc_icon, i4_x, i4_y, (INT32)ui4_w, (INT32)ui4_h);
        CATCH_FAIL(x_wgl_rect_is_cross(&b_need_update, &t_rc_icon, pt_rc_upt));
        if(b_need_update)
        {
            INFORM_ERR(wgl_img_tpl_paint(h_icon, &t_rc_icon, pt_rc_upt, h_g, pt_cvsst, WGL_TPL_PAINT_COMPOSE));
        }            
    }        
    
    return WGLR_OK;
}    

static VOID _bdr_timg_tool_cal_obj_pos( 
    IN      UINT8                      ui1_text_dir,
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

