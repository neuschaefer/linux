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
 * $RCSfile: wgl_custom.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: d9ebaf90e0d0516360fba1622d16baa7 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_obj_type.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_sets_cli.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/* compatible color modes of current system color */ 
#define WGL_SYS_COLOR_COMPATIBLE_DIRECT_CLR_MODES     ( GL_CMCAP_RGB_D565    |  \
                                                        GL_CMCAP_ARGB_D1555  |  \
                                                        GL_CMCAP_ARGB_D4444  |  \
                                                        GL_CMCAP_ARGB_D8888  )

#define WGL_SYS_COLOR_COMPATIBLE_PALETTE_CLR_MODES    ( GL_CMCAP_ARGB_CLUT2  |  \
                                                        GL_CMCAP_ARGB_CLUT4  |  \
                                                        GL_CMCAP_ARGB_CLUT8 )
                                                  
/* color mdoes directly supported by current system color */
#define WGL_SYS_COLOR_SUPPORT_CLR_MODES               WGL_SYS_COLOR_COMPATIBLE_DIRECT_CLR_MODES


/* compatible color modes of current system image */ 
#define WGL_SYS_IMG_COMPATIBLE_DIRECT_CLR_MODES       ( GL_CMCAP_ARGB_D4444  |  \
                                                        GL_CMCAP_ARGB_D8888 )
                                                        
#define WGL_SYS_IMG_COMPATIBLE_PALETTE_CLR_MODES      (0)

/* color mdoes directly supported by current system image */
#define WGL_SYS_IMG_SUPPORT_CLR_MODES                 WGL_SYS_IMG_COMPATIBLE_DIRECT_CLR_MODES

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
/*declaration for system resource*/
UINT32                          g_ui4_wgl_sys_tpl_img_support_clr_modes = WGL_SYS_IMG_SUPPORT_CLR_MODES;
UINT32                          g_ui4_wgl_sys_color_support_clr_modes   = WGL_SYS_COLOR_SUPPORT_CLR_MODES;
#if 1
    WGL_HIMG_TPL_T              g_a_wgl_sys_tpl_img[WGL_SYS_IMG_COUNT];
    GL_COLOR_T                  g_a_wgl_sys_color[WGL_SYS_CLR_COUNT];
#else
    WGL_HIMG_TPL_T              g_a_wgl_sys_tpl_img[1] = {0};
    GL_COLOR_T                  g_a_wgl_sys_color[1] = {{0}};
#endif
WGL_FONT_INFO_T                 g_t_wgl_sys_font_info = 
{
    FE_FNT_SIZE_LARGE,
    FE_FNT_STYLE_REGULAR,
    FE_CMAP_ENC_UNICODE,
    ""
};
 
/*declaration for default image*/
/* enable/disable/highlight images for frame bk, icon bk, pg bk, text bk*/
#if 0
extern WGL_IMG_T                t_normal_bk_img;
extern WGL_IMG_T                t_disable_bk_img;
extern WGL_IMG_T                t_highlight_bk_img;

/* progress bar*/
extern WGL_IMG_T                t_pg_body_img;
extern WGL_IMG_T                t_pg_level_1_img;
extern WGL_IMG_T                t_pg_level_2_img;
extern WGL_IMG_T                t_pg_level_3_img;
extern WGL_IMG_T                t_pg_v_body_img;
extern WGL_IMG_T                t_pg_v_level_1_img;
extern WGL_IMG_T                t_pg_v_level_2_img;
extern WGL_IMG_T                t_pg_v_level_3_img;

/* button*/
extern WGL_IMG_T                t_btn_normal_bk_img;
extern WGL_IMG_T                t_btn_highlight_bk_img;
extern WGL_IMG_T                t_btn_push_bk_img;
#endif
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_custom_default_color_init(VOID);

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*******************************************************************************
 * Name
 *      
 * Description
 *          This is a customizable plug-in function.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_custom_try_convert_sys_color(
    const GL_COLOR_T*           pt_orig_sys_color,
    const WGL_CANVAS_STATUS_T*  pt_cvsst,
    GL_COLOR_T*                 pt_color_out)
{
    
    wgl_cvsst_clear_color(pt_cvsst, pt_color_out);
    
    /* try to convert ARGB group direct color into ARGB group palette color */
    if( GL_GET_COLORMODE_MASK(pt_cvsst->e_clr_mode) | WGL_SYS_COLOR_COMPATIBLE_PALETTE_CLR_MODES )
    {
        INT32           i4_ret;
        GL_COLOR_T      t_clr_tmp;
        UINT16          ui2_idx_clr;
        
        i4_ret = x_gl_palette_match_color( pt_cvsst->h_palette, FALSE, pt_orig_sys_color, &t_clr_tmp, &ui2_idx_clr);
        if( GLR_OK == i4_ret )
        {
            pt_color_out->u3.index = (UINT8)ui2_idx_clr;
        }
        else
        {
            DBG_ERROR(("{WGL(SETS)} ERR: Color matching error. (%d) at L%d\n\n", i4_ret, __LINE__));
        }
    }
    
    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 *          This is a customizable plug-in function.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_custom_try_get_compatible_sys_img(
    UINT16                      ui2_image_id,
    const WGL_CANVAS_STATUS_T*  pt_cvsst,
    WGL_IMG_ST_T*               pe_segment_type,
    UINT32*                     pui4_img_width,
    UINT32*                     pui4_img_height,
    WGL_HIMG_TPL_T*             ph_image)
{
    /* no palette mode image currently */
    *pe_segment_type  = WGL_IMG_ST_NO_SEGMENT;
    *pui4_img_width   = 0;
    *pui4_img_height  = 0;
    *ph_image         = NULL_HANDLE;
    
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
#if 0
INT32 wgl_init_default_sys_resource(VOID)
{
    /*local variable declaration*/
    GL_COLOR_T                  t_clr_fm_enable = {255, {64}, {64}, {64}};
    GL_COLOR_T                  t_clr_fm_highlight = {255, {128}, {128}, {128}};
    GL_COLOR_T                  t_clr_disable = {255, {32}, {32}, {32}};
    GL_COLOR_T                  t_clr_bk_enable = {255, {255}, {255}, {0}};
    GL_COLOR_T                  t_clr_bk_highlight = {255, {0}, {255}, {255}};
    GL_COLOR_T                  t_clr_bk_push = {255, {255}, {0}, {255}};
    GL_COLOR_T                  t_clr_text_enable = {255, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_text_highlight = {255, {255}, {255}, {255}};
    GL_COLOR_T                  t_clr_text_push = {255, {0}, {255}, {255}};
    GL_COLOR_T                  t_clr_text_disable = {255, {192}, {192}, {192}};
    GL_COLOR_T                  t_clr_green = {255, {0}, {192}, {0}};
    GL_COLOR_T                  t_clr_yellow = {255, {192}, {192}, {0}};
    GL_COLOR_T                  t_clr_red = {255, {192}, {0}, {0}};
    GL_COLOR_T                  t_clr_white = {255, {192}, {192}, {192}};
    
    GL_COLOR_T                  t_clr_lb_fg = {255, {220}, {220}, {220}};
    GL_COLOR_T                  t_clr_lb_fg_dis = {255, {180}, {180}, {180}};
    GL_COLOR_T                  t_clr_lb_fg_ht = {255, {255}, {255}, {255}};
    GL_COLOR_T                  t_clr_lb_bk = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_bk_dis = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_bk_ht = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_vp = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_vp_dis = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_vp_ht = {0, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_lb_elem = {255, {128}, {128}, {255}};
    GL_COLOR_T                  t_clr_lb_elem_dis = {255, {64}, {64}, {128}};
    GL_COLOR_T                  t_clr_lb_elem_ht = {255, {230}, {230}, {128}};
    GL_COLOR_T                  t_clr_lb_elem_ht_unfocus = {255, {220}, {220}, {75}};
    GL_COLOR_T                  t_clr_lb_elem_pushed = {255, {64}, {64}, {255}};
    GL_COLOR_T                  t_clr_lb_elem_sel = {255, {200}, {200}, {255}};
    GL_COLOR_T                  t_clr_lb_elem_sel_dis = {255, {100}, {100}, {128}};
    
    GL_COLOR_T                  t_clr_hts_bk_en =  {255, {160}, {160}, {160}};
    GL_COLOR_T                  t_clr_hts_bk_dis = {255, {64}, {64}, {64}};
    GL_COLOR_T                  t_clr_hts_bk_hlt = {255, {255}, {255}, {0}};
    GL_COLOR_T                  t_clr_hts_vp_en = {255, {200}, {200}, {200}};
    GL_COLOR_T                  t_clr_hts_vp_dis = {255, {128}, {128}, {128}};
    GL_COLOR_T                  t_clr_hts_vp_hlt = {255, {200}, {200}, {200}};
    GL_COLOR_T                  t_clr_hts_text = {255, {0}, {0}, {0}};
    GL_COLOR_T                  t_clr_hts_text_bk = {255, {200}, {200}, {200}};

    GL_COLOR_T                  t_clr_tktp_bk = {255, {64}, {64}, {64}};
    GL_COLOR_T                  t_clr_tktp_msg_fg = {255, {255}, {255}, {255}};
    GL_COLOR_T                  t_clr_tktp_msg_bk = {255, {0}, {0}, {0}};

    GL_COLOR_T                  t_clr_bdr_fg_enable = {255, {255}, {255}, {255}};
    GL_COLOR_T                  t_clr_bdr_fg_disable = {255, {160}, {160}, {160}};
    GL_COLOR_T                  t_clr_bdr_fg_highlight = {255, {0}, {196}, {0}};
    GL_COLOR_T                  t_clr_bdr_fg_highlight_inactive = {255, {0}, {32}, {0}};
    GL_COLOR_T                  t_clr_bdr_fg_push = {255, {0}, {255}, {0}};
    
    GL_COLOR_T                  t_clr_bdr_bk_enable = {255, {128}, {128}, {128}};
    GL_COLOR_T                  t_clr_bdr_bk_disable = {255, {64}, {64}, {64}};
    GL_COLOR_T                  t_clr_bdr_bk_highlight = {255, {196}, {0}, {0}};
    GL_COLOR_T                  t_clr_bdr_bk_highlight_inactive = {255, {32}, {0}, {0}};
    GL_COLOR_T                  t_clr_bdr_bk_push = {255, {255}, {0}, {0}};

    WGL_IMG_T*                  a_pt_bk_img[10];
    GL_COLOR_T*                 a_pt_clr[10];
    UINT16                      a_ui2_id[10];
    INT32                       i4_index, i4_ret;

    /*init widget sets' cli command*/
    i4_ret = wgl_sets_cli_init();
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*register auto-free function*/
    i4_ret = wgl_register_autofree_fcts();
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*init image*/
    a_pt_bk_img[0] = &t_normal_bk_img;
    a_pt_bk_img[1] = &t_disable_bk_img;
    a_pt_bk_img[2] = &t_highlight_bk_img;

    /*set default bk for frame*/
    a_ui2_id[0] = WGL_SYS_IMG_FM_BK;
    a_ui2_id[1] = WGL_SYS_IMG_FM_BK_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_FM_BK_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set default bk for icon and text static*/
    a_ui2_id[0] = WGL_SYS_IMG_STATIC;
    a_ui2_id[1] = WGL_SYS_IMG_STATIC_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_STATIC_HT;    
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set default bk for progress bar*/
    a_ui2_id[0] = WGL_SYS_IMG_PG_BK;
    a_ui2_id[1] = WGL_SYS_IMG_PG_BK_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_BK_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set default bk for edit box */
    a_ui2_id[0] = WGL_SYS_IMG_EB;
    a_ui2_id[1] = WGL_SYS_IMG_EB_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_EB_HT;    
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set default bk for button */
    a_pt_bk_img[0] = &t_btn_normal_bk_img;
    a_pt_bk_img[1] = &t_disable_bk_img;
    a_pt_bk_img[2] = &t_btn_highlight_bk_img;
    a_pt_bk_img[3] = &t_btn_push_bk_img;
    a_ui2_id[0] = WGL_SYS_IMG_BTN;
    a_ui2_id[1] = WGL_SYS_IMG_BTN_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_BTN_HT;   
    a_ui2_id[3] = WGL_SYS_IMG_BTN_PUSHED;   
    for(i4_index = 0; i4_index < 4; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set body image for progress bar */
    a_pt_bk_img[0] = &t_pg_body_img;
    a_pt_bk_img[1] = &t_pg_body_img;
    a_pt_bk_img[2] = &t_pg_body_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_BODY;
    a_ui2_id[1] = WGL_SYS_IMG_PG_BODY_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_BODY_HT;   
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set level-1 image for progress bar */
    a_pt_bk_img[0] = &t_pg_level_1_img;
    a_pt_bk_img[1] = &t_pg_level_1_img;
    a_pt_bk_img[2] = &t_pg_level_1_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_LEVEL1;
    a_ui2_id[1] = WGL_SYS_IMG_PG_LEVEL1_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_LEVEL1_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set level-2 image for progress bar */
    a_pt_bk_img[0] = &t_pg_level_2_img;
    a_pt_bk_img[1] = &t_pg_level_2_img;
    a_pt_bk_img[2] = &t_pg_level_2_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_LEVEL2;
    a_ui2_id[1] = WGL_SYS_IMG_PG_LEVEL2_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_LEVEL2_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set level-3 image for progress bar */
    a_pt_bk_img[0] = &t_pg_level_3_img;
    a_pt_bk_img[1] = &t_pg_level_3_img;
    a_pt_bk_img[2] = &t_pg_level_3_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_LEVEL3;
    a_ui2_id[1] = WGL_SYS_IMG_PG_LEVEL3_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_LEVEL3_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set body image for v-progress bar */
    a_pt_bk_img[0] = &t_pg_v_body_img;
    a_pt_bk_img[1] = &t_pg_v_body_img;
    a_pt_bk_img[2] = &t_pg_v_body_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_V_BODY;
    a_ui2_id[1] = WGL_SYS_IMG_PG_V_BODY_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_V_BODY_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set level-1 image for v-progress bar */
    a_pt_bk_img[0] = &t_pg_v_level_1_img;
    a_pt_bk_img[1] = &t_pg_v_level_1_img;
    a_pt_bk_img[2] = &t_pg_v_level_1_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_V_LEVEL1;
    a_ui2_id[1] = WGL_SYS_IMG_PG_V_LEVEL1_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_V_LEVEL1_HT;   
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set level-2 image for v-progress bar */
    a_pt_bk_img[0] = &t_pg_v_level_2_img;
    a_pt_bk_img[1] = &t_pg_v_level_2_img;
    a_pt_bk_img[2] = &t_pg_v_level_2_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_V_LEVEL2;
    a_ui2_id[1] = WGL_SYS_IMG_PG_V_LEVEL2_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_V_LEVEL2_HT;   
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set level-3 image for v-progress bar */
    a_pt_bk_img[0] = &t_pg_v_level_3_img;
    a_pt_bk_img[1] = &t_pg_v_level_3_img;
    a_pt_bk_img[2] = &t_pg_v_level_3_img;
    a_ui2_id[0] = WGL_SYS_IMG_PG_V_LEVEL3;
    a_ui2_id[1] = WGL_SYS_IMG_PG_V_LEVEL3_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_PG_V_LEVEL3_HT;   
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set image for listbox*/
#if 0    
    a_pt_bk_img[0] = &t_normal_bk_img;
    a_pt_bk_img[1] = &t_disable_bk_img;
    a_pt_bk_img[2] = &t_highlight_bk_img;
    a_pt_bk_img[3] = &t_btn_normal_bk_img;
    a_pt_bk_img[4] = &t_disable_bk_img;
    a_pt_bk_img[5] = &t_btn_highlight_bk_img;
    a_pt_bk_img[6] = &t_btn_highlight_bk_img;
    a_pt_bk_img[7] = &t_btn_push_bk_img;
    a_pt_bk_img[8] = &t_btn_normal_bk_img;
    a_pt_bk_img[9] = &t_disable_bk_img;
    a_ui2_id[0] = WGL_SYS_IMG_LB;
    a_ui2_id[1] = WGL_SYS_IMG_LB_DIS;
    a_ui2_id[2] = WGL_SYS_IMG_LB_HT;
    a_ui2_id[3] = WGL_SYS_IMG_LB_ELEM;
    a_ui2_id[4] = WGL_SYS_IMG_LB_ELEM_DIS;
    a_ui2_id[5] = WGL_SYS_IMG_LB_ELEM_HT;
    a_ui2_id[6] = WGL_SYS_IMG_LB_ELEM_HT_UNFOCUS;
    a_ui2_id[7] = WGL_SYS_IMG_LB_ELEM_PUSHED;
    a_ui2_id[8] = WGL_SYS_IMG_LB_ELEM_SEL;
    a_ui2_id[9] = WGL_SYS_IMG_LB_ELEM_SEL_DIS;
    for(i4_index = 0; i4_index < 10; i4_index++)
    {
        i4_ret = x_wgl_set_system_image(a_ui2_id[i4_index], WGL_IMG_ST_HV, a_pt_bk_img[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
#endif

    /*set font info*/
    i4_ret = x_wgl_set_system_font(NULL, FE_FNT_SIZE_LARGE, FE_FNT_STYLE_REGULAR, FE_CMAP_ENC_UNICODE);
    if(WGLR_OK != i4_ret)
    {
        WGL_ASSERT(0);
        return i4_ret;
    }

    /*set text color */
    a_pt_clr[0] = &t_clr_text_enable;
    a_pt_clr[1] = &t_clr_text_disable;
    a_pt_clr[2] = &t_clr_text_highlight;
    a_pt_clr[3] = &t_clr_text_push;
    a_ui2_id[0] = WGL_SYS_CLR_TEXT;
    a_ui2_id[1] = WGL_SYS_CLR_TEXT_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_TEXT_HT;  
    a_ui2_id[3] = WGL_SYS_CLR_TEXT_PUSHED;
    for(i4_index = 0; i4_index < 4; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set frame color */
    a_pt_clr[0] = &t_clr_fm_enable;
    a_pt_clr[1] = &t_clr_disable;
    a_pt_clr[2] = &t_clr_fm_highlight;
    a_ui2_id[0] = WGL_SYS_CLR_FM_BK;
    a_ui2_id[1] = WGL_SYS_CLR_FM_BK_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_FM_BK_HT; 
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set enable color to static/button/editbox/pg's bk*/
    a_pt_clr[0] = &t_clr_bk_enable;
    a_ui2_id[0] = WGL_SYS_CLR_STATIC;
    a_ui2_id[1] = WGL_SYS_CLR_BTN;
    a_ui2_id[2] = WGL_SYS_CLR_EB;
    a_ui2_id[3] = WGL_SYS_CLR_PG_BK;
    for(i4_index = 0; i4_index < 4; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set disable color to static/button/editbox/pg's bk*/
    a_pt_clr[0] = &t_clr_disable;
    a_ui2_id[0] = WGL_SYS_CLR_STATIC_DIS;
    a_ui2_id[1] = WGL_SYS_CLR_BTN_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_EB_DIS;
    a_ui2_id[3] = WGL_SYS_CLR_PG_BK_DIS;
    for(i4_index = 0; i4_index < 4; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set highlight color to static/button/editbox/pg's bk*/
    a_pt_clr[0] = &t_clr_bk_highlight;
    a_ui2_id[0] = WGL_SYS_CLR_STATIC_HT;
    a_ui2_id[1] = WGL_SYS_CLR_BTN_HT;
    a_ui2_id[2] = WGL_SYS_CLR_EB_HT;
    a_ui2_id[3] = WGL_SYS_CLR_PG_BK_HT;
    for(i4_index = 0; i4_index < 4; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set pg's body color*/
    a_pt_clr[0] = &t_clr_white;
    a_ui2_id[0] = WGL_SYS_CLR_PG_BODY;
    a_ui2_id[1] = WGL_SYS_CLR_PG_BODY_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_PG_BODY_HT;
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set pg's level 1 color*/
    a_pt_clr[0] = &t_clr_green;
    a_ui2_id[0] = WGL_SYS_CLR_PG_LEVEL1;
    a_ui2_id[1] = WGL_SYS_CLR_PG_LEVEL1_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_PG_LEVEL1_HT;
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set pg's level 2 color*/
    a_pt_clr[0] = &t_clr_yellow;
    a_ui2_id[0] = WGL_SYS_CLR_PG_LEVEL2;
    a_ui2_id[1] = WGL_SYS_CLR_PG_LEVEL2_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_PG_LEVEL2_HT;
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set pg's level 3 color*/
    a_pt_clr[0] = &t_clr_red;
    a_ui2_id[0] = WGL_SYS_CLR_PG_LEVEL3;
    a_ui2_id[1] = WGL_SYS_CLR_PG_LEVEL3_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_PG_LEVEL3_HT;
    for(i4_index = 0; i4_index < 3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[0]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set push color to button*/
    i4_ret = x_wgl_set_system_color(WGL_SYS_CLR_BTN_PUSHED, &t_clr_bk_push);
    if(WGLR_OK != i4_ret)
    {
        WGL_ASSERT(0);
        return i4_ret;
    }

    /*set List Box default foreground colors*/
    a_pt_clr[0] = &t_clr_lb_fg;
    a_pt_clr[1] = &t_clr_lb_fg_dis;
    a_pt_clr[2] = &t_clr_lb_fg_ht;
    a_ui2_id[0] = WGL_SYS_CLR_LB_FG;
    a_ui2_id[1] = WGL_SYS_CLR_LB_FG_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_LB_FG_HT;
    for(i4_index=0; i4_index<3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    /*set List Box default background colors*/
    a_pt_clr[0] = &t_clr_lb_bk;
    a_pt_clr[1] = &t_clr_lb_bk_dis;
    a_pt_clr[2] = &t_clr_lb_bk_ht;
    a_ui2_id[0] = WGL_SYS_CLR_LB_BK;
    a_ui2_id[1] = WGL_SYS_CLR_LB_BK_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_LB_BK_HT;
    for(i4_index=0; i4_index<3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    /*set List Box default viewport background colors*/
    a_pt_clr[0] = &t_clr_lb_vp;
    a_pt_clr[1] = &t_clr_lb_vp_dis;
    a_pt_clr[2] = &t_clr_lb_vp_ht;
    a_ui2_id[0] = WGL_SYS_CLR_LB_VP;
    a_ui2_id[1] = WGL_SYS_CLR_LB_VP_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_LB_VP_HT;
    for(i4_index=0; i4_index<3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    /*set List Box default element background colors*/
    a_pt_clr[0] = &t_clr_lb_elem;
    a_pt_clr[1] = &t_clr_lb_elem_dis;
    a_pt_clr[2] = &t_clr_lb_elem_ht;
    a_pt_clr[3] = &t_clr_lb_elem_ht_unfocus;
    a_pt_clr[4] = &t_clr_lb_elem_pushed;
    a_pt_clr[5] = &t_clr_lb_elem_sel;
    a_pt_clr[6] = &t_clr_lb_elem_sel_dis;
    a_ui2_id[0] = WGL_SYS_CLR_LB_ELEM;
    a_ui2_id[1] = WGL_SYS_CLR_LB_ELEM_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_LB_ELEM_HT;
    a_ui2_id[3] = WGL_SYS_CLR_LB_ELEM_HT_UNFOCUS;
    a_ui2_id[4] = WGL_SYS_CLR_LB_ELEM_PUSHED;
    a_ui2_id[5] = WGL_SYS_CLR_LB_ELEM_SEL;
    a_ui2_id[6] = WGL_SYS_CLR_LB_ELEM_SEL_DIS;
    for(i4_index=0; i4_index<7; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    /*set List Box default element text colors*/
    a_ui2_id[0] = WGL_SYS_CLR_LB_ELEM_TEXT;
    a_ui2_id[1] = WGL_SYS_CLR_LB_ELEM_TEXT_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_LB_ELEM_TEXT_HT;
    a_ui2_id[3] = WGL_SYS_CLR_LB_ELEM_TEXT_HT_UNFOCUS;
    a_ui2_id[4] = WGL_SYS_CLR_LB_ELEM_TEXT_PUSHED;
    a_ui2_id[5] = WGL_SYS_CLR_LB_ELEM_TEXT_SEL;
    a_ui2_id[6] = WGL_SYS_CLR_LB_ELEM_TEXT_SEL_DIS;
    for(i4_index=0; i4_index<7; i4_index++)
    {
        GL_COLOR_T  t_clr_tmp;
        t_clr_tmp.a = a_pt_clr[i4_index]->a;
        t_clr_tmp.u1.r = ~(a_pt_clr[i4_index]->u1.r);
        t_clr_tmp.u2.g = ~(a_pt_clr[i4_index]->u2.g);
        t_clr_tmp.u3.b = ~(a_pt_clr[i4_index]->u3.b);
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], &t_clr_tmp);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set hyper text default background & viewport colors*/
    a_pt_clr[0] = &t_clr_hts_bk_en;
    a_pt_clr[1] = &t_clr_hts_bk_dis;
    a_pt_clr[2] = &t_clr_hts_bk_hlt;
    a_pt_clr[3] = &t_clr_hts_vp_en;
    a_pt_clr[4] = &t_clr_hts_vp_dis;
    a_pt_clr[5] = &t_clr_hts_vp_hlt;
    a_pt_clr[6] = &t_clr_hts_text;
    a_pt_clr[7] = &t_clr_hts_text_bk;
    a_ui2_id[0] = WGL_SYS_CLR_HTS_BK;
    a_ui2_id[1] = WGL_SYS_CLR_HTS_BK_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_HTS_BK_HT;
    a_ui2_id[3] = WGL_SYS_CLR_HTS_VP;
    a_ui2_id[4] = WGL_SYS_CLR_HTS_VP_DIS;
    a_ui2_id[5] = WGL_SYS_CLR_HTS_VP_HT;
    a_ui2_id[6] = WGL_SYS_CLR_HTS_TEXT;
    a_ui2_id[7] = WGL_SYS_CLR_HTS_TEXT_BK;
    for(i4_index=0; i4_index<7; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    
    /*set ticker tape default background & viewport colors*/
    a_pt_clr[0] = &t_clr_tktp_bk;
    a_pt_clr[1] = &t_clr_tktp_msg_fg;
    a_pt_clr[2] = &t_clr_tktp_msg_bk;
    a_ui2_id[0] = WGL_SYS_CLR_TKTP_BK;
    a_ui2_id[1] = WGL_SYS_CLR_TKTP_MSG_FG;
    a_ui2_id[2] = WGL_SYS_CLR_TKTP_MSG_BK;
    for(i4_index=0; i4_index<3; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set default border foreground colors*/
    a_pt_clr[0] = &t_clr_bdr_fg_enable;
    a_pt_clr[1] = &t_clr_bdr_fg_disable;
    a_pt_clr[2] = &t_clr_bdr_fg_highlight;
    a_pt_clr[3] = &t_clr_bdr_fg_highlight_inactive;
    a_pt_clr[4] = &t_clr_bdr_fg_push;
    a_ui2_id[0] = WGL_SYS_CLR_BDR_FG;
    a_ui2_id[1] = WGL_SYS_CLR_BDR_FG_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_BDR_FG_HT;
    a_ui2_id[3] = WGL_SYS_CLR_BDR_FG_HT_INACT;
    a_ui2_id[4] = WGL_SYS_CLR_BDR_FG_PUSHED;
    for(i4_index=0; i4_index<5; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }

    /*set default border background colors*/
    a_pt_clr[0] = &t_clr_bdr_bk_enable;
    a_pt_clr[1] = &t_clr_bdr_bk_disable;
    a_pt_clr[2] = &t_clr_bdr_bk_highlight;
    a_pt_clr[3] = &t_clr_bdr_bk_highlight_inactive;
    a_pt_clr[4] = &t_clr_bdr_bk_push;
    a_ui2_id[0] = WGL_SYS_CLR_BDR_BK;
    a_ui2_id[1] = WGL_SYS_CLR_BDR_BK_DIS;
    a_ui2_id[2] = WGL_SYS_CLR_BDR_BK_HT;
    a_ui2_id[3] = WGL_SYS_CLR_BDR_BK_HT_INACT;
    a_ui2_id[4] = WGL_SYS_CLR_BDR_BK_PUSHED;
    for(i4_index=0; i4_index<5; i4_index++)
    {
        i4_ret = x_wgl_set_system_color(a_ui2_id[i4_index], a_pt_clr[i4_index]);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }    

    return WGLR_OK;
}
#else
INT32 wgl_init_default_sys_resource(VOID)
{
    return wgl_init_default_sys_resource_ex(NULL);
}
#endif

/*******************************************************************************
 * Name
 *      wgl_init_default_sys_resource_ex
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_init_default_sys_resource_ex(
    const WGL_CUSTOM_INIT_INFO_T*      pt_custom_info)
{
    INT32                       i4_ret;
#ifdef WGL_ANIM_SUPPORT
    WGL_ANIM_CONFIG_T           t_anim_cfg;
#endif

    /*init widget sets' cli command*/
    i4_ret = wgl_sets_cli_init();
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*register auto-free function*/
    i4_ret = wgl_register_autofree_fcts();
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    /*init default color*/
    i4_ret = _wgl_custom_default_color_init();
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*init default animation resource*/
#ifdef WGL_ANIM_SUPPORT
    t_anim_cfg.ui4_tick_interval = 50;
    t_anim_cfg.e_tick_mode = WGL_TICK_CONTINUOUS;
    i4_ret = x_wgl_init_animation_resource(&t_anim_cfg);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }
#endif

    /*set font info*/
    i4_ret = x_wgl_set_system_font(NULL, FE_FNT_SIZE_LARGE, FE_FNT_STYLE_REGULAR, FE_CMAP_ENC_UNICODE);
    if(WGLR_OK != i4_ret)
    {
        WGL_ASSERT(0);
        return i4_ret;
    }

    /*init wgl image cache.*/
    i4_ret = wgl_image_init();
    if(WGLR_OK != i4_ret)
    {
        WGL_ASSERT(0);
        return i4_ret;
    }

    /*init font handle manager*/
    return wgl_font_init((NULL == pt_custom_info) ? 0 : pt_custom_info->ui2_font_handle_grow_num);
}
/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      _wgl_custom_default_color_init
 * Description
 *          internal use, don't check NULL parameter
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_custom_default_color_init(VOID)
{
    x_memset(g_a_wgl_sys_tpl_img, 0, WGL_SYS_IMG_COUNT * sizeof(WGL_HIMG_TPL_T));
    x_memset(g_a_wgl_sys_color, 0, WGL_SYS_CLR_COUNT * sizeof(GL_COLOR_T));
    return WGLR_OK;
} 
