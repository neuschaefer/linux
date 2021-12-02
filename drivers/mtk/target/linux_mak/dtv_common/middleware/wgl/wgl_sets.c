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
 * $RCSfile: wgl_sets.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 52fc0580ea63823aea30dfb15b459f72 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "handle/handle.h"
#include "os/inc/x_os.h"
#include "wgl/c_wgl_sets.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_image.h"
#include "wgl/wgl_sets.h"
#include "font/x_fe.h"
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


#define _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT          10

#define _GET_SYS_WIDGET_INTF_TABLE()                                        \
(&t_wgl_widget_intf_table)

#define _GET_SYS_CNT_INTF_TABLE()                                           \
(&t_wgl_cnt_intf_table)

#define _GET_SYS_BDR_INTF_TABLE()                                           \
(&t_wgl_bdr_intf_table)

#define _GET_SYS_WIDGET_INTF_RECORD(ui4_index)                              \
( &t_wgl_widget_intf_table.at_intf_records[ui4_index] )

#define _GET_SYS_CNT_INTF_RECORD(ui4_index)                                 \
( &t_wgl_cnt_intf_table.at_intf_records[ui4_index] )

#define _GET_SYS_BDR_INTF_RECORD(ui4_index)                                 \
( &t_wgl_bdr_intf_table.at_intf_records[ui4_index] )

#define _GET_SYS_WIDGET_INTF_NUMBERS()                                      \
( t_wgl_widget_intf_table.ui4_intf_count )

#define _GET_SYS_CNT_INTF_NUMBERS()                                         \
( t_wgl_cnt_intf_table.ui4_intf_count )

#define _GET_SYS_BDR_INTF_NUMBERS()                                         \
( t_wgl_bdr_intf_table.ui4_intf_count )

#define _IS_SYS_COLOR_COMPATIBLE_WITH_CANVAS(_pt_cvsst)                     \
    ( g_ui4_wgl_sys_color_support_clr_modes | GL_GET_COLORMODE_MASK((_pt_cvsst)->e_clr_mode) )

#define _IS_SYS_IMG_COMPATIBLE_WITH_CANVAS(_pt_cvsst)                       \
    ( g_ui4_wgl_sys_tpl_img_support_clr_modes | GL_GET_COLORMODE_MASK((_pt_cvsst)->e_clr_mode) )
    
#if 0
    #define _IS_NULL_SYS_TPL_IMG(ui2_img_id)                                    \
        ( NULL_HANDLE == g_a_wgl_sys_tpl_img[ui2_img_id] )
    
    #define _GET_SYS_TPL_IMG(ui2_img_id)                                        \
        ( g_a_wgl_sys_tpl_img[ui2_img_id] )
    
    #define _GET_SYS_FONT()                                                     \
        ( g_t_wgl_sys_font_info )
    
    #define _GET_SYS_COLOR(ui2_color_id)                                        \
        ( g_a_wgl_sys_color[ui2_color_id] )
#else
    #define _IS_NULL_SYS_TPL_IMG(ui2_img_id)                                    \
        ( NULL_HANDLE == g_a_wgl_sys_tpl_img[0] )
    
    #define _GET_SYS_TPL_IMG(ui2_img_id)                                        \
        ( g_a_wgl_sys_tpl_img[0] )
    
    #define _GET_SYS_FONT()                                                     \
        ( g_t_wgl_sys_font_info )
    
    #define _GET_SYS_COLOR(ui2_color_id)                                        \
        ( g_a_wgl_sys_color[0] )
#endif

typedef struct _WGL_WIDGET_INTF_TABLE_T
{
    WIDGET_INTF_RECORD_T*             at_intf_records;
    UINT32                            ui4_alloc_count;
    UINT32                            ui4_intf_count;
} WGL_WIDGET_INTF_TABLE_T;

typedef struct _WGL_CONTENT_INTF_TABLE_T
{
    CONTENT_INTF_RECORD_T*            at_intf_records;
    UINT32                            ui4_alloc_count;
    UINT32                            ui4_intf_count;
} WGL_CONTENT_INTF_TABLE_T;

typedef struct _WGL_BORDER_INTF_TABLE_T
{
    BORDER_INTF_RECORD_T*             at_intf_records;
    UINT32                            ui4_alloc_count;
    UINT32                            ui4_intf_count;
} WGL_BORDER_INTF_TABLE_T;
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static WGL_WIDGET_INTF_TABLE_T          t_wgl_widget_intf_table = {0};
static WGL_CONTENT_INTF_TABLE_T         t_wgl_cnt_intf_table = {0};
static WGL_BORDER_INTF_TABLE_T          t_wgl_bdr_intf_table = {0};


extern UINT32                           g_ui4_wgl_sys_tpl_img_support_clr_modes;
extern UINT32                           g_ui4_wgl_sys_color_support_clr_modes;
extern WGL_HIMG_TPL_T                   g_a_wgl_sys_tpl_img[];
extern GL_COLOR_T                       g_a_wgl_sys_color[];
extern WGL_FONT_INFO_T                  g_t_wgl_sys_font_info;


/* plug-in function declaration */
extern INT32 wgl_custom_try_convert_sys_color(
    const GL_COLOR_T*                   pt_orig_sys_color,
    const WGL_CANVAS_STATUS_T*          pt_cvsst,
    GL_COLOR_T*                         pt_color_out);
    
/* plug-in function declaration */
extern INT32 wgl_custom_try_get_compatible_sys_img(
    UINT16                              ui2_image_id,
    const WGL_CANVAS_STATUS_T*          pt_cvsst,
    WGL_IMG_ST_T*                       pe_segment_type,
    UINT32*                             pui4_img_width,
    UINT32*                             pui4_img_height,
    WGL_HIMG_TPL_T*                     ph_image);

 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_handle_autofree (HANDLE_T                h_handle,
                                   HANDLE_TYPE_T           e_type);

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
INT32 x_wgl_set_system_color(
    UINT16                      ui2_color_id,
    const GL_COLOR_T*           pt_color)
{
    DBG_API(("{WGL(SETS)} %s: ui2_color_id = 0x%.8x \tpt_color = 0x%.8x\n", 
            __FUNCTION__, ui2_color_id, pt_color));

    /*check parameter*/
    if(NULL == pt_color || !IS_LEGAL_SYS_CLR_ID(ui2_color_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*copy color information*/

    _GET_SYS_COLOR(ui2_color_id) = *pt_color;
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
/*NOT_READY*/                   
INT32 x_wgl_set_system_font(
    const CHAR*                 ps_font, 
    FE_FNT_SIZE                 e_size,
    FE_FNT_STYLE                e_style,
    FE_CMAP_ENCODING            e_cmap)
{
    WGL_FONT_INFO_T*            pt_font_info = &_GET_SYS_FONT();

    DBG_API(("{WGL(SETS)} %s: ps_font = 0x%.8x   e_size = 0x%.8x   e_style = 0x%.8x   e_cmap = 0x%.8x\n", 
            __FUNCTION__, ps_font, e_size, e_style, e_cmap));

    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(SETS)} ERR: No space to store font info. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        return WGLR_INTERNAL_ERROR;
    }
    
    if(ps_font)
    {
        if(x_strlen(ps_font) >= WGL_MAX_FONT_NAME)
        {
            DBG_ERROR(("{WGL(SETS)} ERR: Font name specified by ps_font is over limit(%d is max). (%d) at L%d\n\n", WGL_MAX_FONT_NAME, WGLR_OVER_SUPPORT_LIMIT, __LINE__));
            return WGLR_OVER_SUPPORT_LIMIT;
        }
        
        x_strcpy(pt_font_info->a_c_font_name, ps_font);
    }
    else
    {
        x_memset(pt_font_info, 0, sizeof(WGL_FONT_INFO_T));
    }
    
    pt_font_info->e_font_size = e_size;
    pt_font_info->e_font_style = e_style;
    pt_font_info->e_font_cmap = e_cmap;
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_set_system_image(
    UINT16                      ui2_image_id,
    WGL_IMG_ST_T                e_segment_type,
    const WGL_IMG_T*            pt_image)
{
    /*local varaible declaration*/
    WGL_HIMG_TPL_T              h_tpl = NULL_HANDLE;
    INT32                       i4_ret;

    DBG_API(("{WGL(SETS)} %s: ui2_image_id = 0x%.8x   e_segment_type = 0x%.8x   pt_image = 0x%.8x\n", 
            __FUNCTION__, ui2_image_id, e_segment_type, pt_image));

    /*check parameter*/
    if(NULL == pt_image || !IS_LEGAL_SYS_IMG_ID(ui2_image_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(SETS)} pt_image (w, h, d)= 0x%.8x (%d, %d, %d)\n"
              "\ttype = 0x%.8x   size = %d   data = 0x%.8x\n"
              "\ttable_cols = %d   table_data = 0x%.8x\n", 
              pt_image, pt_image->ui2_width, pt_image->ui2_lines,   pt_image->ui1_bits_per_pixel,
              pt_image->ui4_image_type, pt_image->ui4_image_size, pt_image->pui1_image_data,
              pt_image->ui2_table_columns, pt_image->pt_table_data));

    /*check template image*/
    if(!_IS_NULL_SYS_TPL_IMG(ui2_image_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: This system image has been initialized. This version doesn't support change system image at runtime (%d) at L%d\n\n", WGLR_NOT_SUPPORT_RUNTIME_UPDATE, __LINE__));
        return WGLR_NOT_SUPPORT_RUNTIME_UPDATE;
    }

    /*create template image*/
    i4_ret = wgl_img_tpl_create(e_segment_type, 0, pt_image, &h_tpl);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Fail to create template image. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*store tempalte image*/
    _GET_SYS_TPL_IMG(ui2_image_id) = h_tpl;

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Color
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_get_system_color(
    UINT16                      ui2_color_id,
    HANDLE_T                    h_canvas,
    GL_COLOR_T*                 pt_color)
{
    WGL_CANVAS_INFO_T           t_cvs_info;
    WGL_CANVAS_STATUS_T         t_cvsst;
    GL_HPALETTE_T               h_plt_cvs;
    INT32                       i4_ret;
    
    DBG_API(("{WGL(SETS)} %s: ui2_color_id = 0x%.8x   pt_color = 0x%.8x\n", __FUNCTION__, ui2_color_id, pt_color));

    /*check parameter*/
    if(NULL == pt_color || !IS_LEGAL_SYS_CLR_ID(ui2_color_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    /* get info of the canvas */
    if(WGLR_OK != (i4_ret = x_wgl_get_canvas_info(h_canvas, &t_cvs_info)))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Fail to get the canvas's info. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /* get palette of the canvas */
    if(WGLR_OK != (i4_ret = x_wgl_get_canvas_palette(h_canvas, &h_plt_cvs)))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Fail to get the canvas's palette. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    wgl_cvsst_update(&t_cvsst, &t_cvs_info, h_plt_cvs);
    i4_ret = wgl_get_system_color(ui2_color_id, &t_cvsst, pt_color);
    x_gl_obj_free(h_plt_cvs);
    
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Font
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_get_system_font(
    WGL_FONT_INFO_T*            pt_font_info)
{
    DBG_API(("{WGL(SETS)} %s: pt_font_info = 0x%.8x\n", __FUNCTION__, pt_font_info));

    /*check parameter*/
    if(NULL == pt_font_info)
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. Input arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*prepare return value*/
    x_memcpy(pt_font_info, &_GET_SYS_FONT(), sizeof(WGL_FONT_INFO_T));
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Image
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_get_system_image(
    UINT16                      ui2_image_id,
    HANDLE_T                    h_canvas,
    WGL_IMG_ST_T*               pe_segment_type,
    UINT32*                     pui4_img_width,
    UINT32*                     pui4_img_height,
    WGL_HIMG_TPL_T*             ph_image)
{
    /*local varaible declaration*/
    WGL_CANVAS_INFO_T           t_cvs_info;
    WGL_CANVAS_STATUS_T         t_cvsst;
    GL_HPALETTE_T               h_plt_cvs;
    INT32                       i4_ret;

    DBG_API(("{WGL(SETS)} %s: ui2_image_id = 0x%.8x   pe_segment_type = 0x%.8x\n"
            "pui4_img_width = 0x%.8x   pui4_img_height = 0x%.8x   ph_image = 0x%.8x\n", 
            __FUNCTION__, ui2_image_id, pe_segment_type,
            pui4_img_width, pui4_img_height, ph_image));

    /*check parameter*/
    if(NULL == ph_image 
        || NULL == pe_segment_type 
        || NULL == pui4_img_width 
        || NULL == pui4_img_height 
        || !IS_LEGAL_SYS_IMG_ID(ui2_image_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /* get info of the canvas */
    if(WGLR_OK != (i4_ret = x_wgl_get_canvas_info(h_canvas, &t_cvs_info)))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Fail to get the canvas's info. (%d) at L%d\n\n", i4_ret, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(GL_IS_PALETTE_MODE(t_cvs_info.e_clr_mode))
    {
        /* get palette of the canvas */
        if(WGLR_OK != (i4_ret = x_wgl_get_canvas_palette(h_canvas, &h_plt_cvs)))
        {
            DBG_ERROR(("{WGL(SETS)} ERR: Fail to get the canvas's palette. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    else
    {
        h_plt_cvs = NULL_HANDLE;
    }
    
    wgl_cvsst_update(&t_cvsst, &t_cvs_info, h_plt_cvs);
    
    return wgl_get_system_image(ui2_image_id, &t_cvsst, pe_segment_type, pui4_img_width, pui4_img_height, ph_image);
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
INT32 x_wgl_get_max_char_in_box_ex(
        HFONT_T                         h_fnt,
        const UTF16_T*                  w2s_str,
        INT32                           i4_max_len,
        BOOL                            b_check_length,
        INT32                           i4_box_w,
        const UTF16_T*                  w2s_can_break,
        const UTF16_T*                  w2s_skip,
        UINT16                          ui2_num_skip,
        UINT16                          ui2_w_skip,
        BOOL                            b_skip_as_break,
        UTF16_T**                       pw2s_next,
        UINT32*                         pui4_num_ch,
        INT32*                          pi4_width)
{
    return wgl_get_max_char_in_box_ex (h_fnt,
                                       w2s_str,
                                       i4_max_len,
                                       b_check_length,
                                       i4_box_w,
                                       w2s_can_break,
                                       w2s_skip,
                                       ui2_num_skip,
                                       ui2_w_skip,
                                       b_skip_as_break,
                                       pw2s_next,
                                       pui4_num_ch,
                                       pi4_width);
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
INT32 wgl_get_system_image(
    UINT16                      ui2_image_id,
    const WGL_CANVAS_STATUS_T*  pt_cvsst,
    WGL_IMG_ST_T*               pe_segment_type,
    UINT32*                     pui4_img_width,
    UINT32*                     pui4_img_height,
    WGL_HIMG_TPL_T*             ph_image)
{    
    WGL_IMG_TPL_T*              pt_template = NULL;
    
    *pe_segment_type  = WGL_IMG_ST_NO_SEGMENT;
    *pui4_img_width   = 0;
    *pui4_img_height  = 0;
    *ph_image         = NULL_HANDLE;
    
    /* check arguments */
    if(!IS_LEGAL_SYS_IMG_ID(ui2_image_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    /* check the color mode */
    if(_IS_SYS_IMG_COMPATIBLE_WITH_CANVAS(pt_cvsst))
    {
        /*check template image*/
        if(_IS_NULL_SYS_TPL_IMG(ui2_image_id))
        {
            DBG_INFO(("{WGL(SETS)} INFO: This system image has not been initialized. (%d) at L%d\n\n", WGLR_SYS_RC_NOT_INIT, __LINE__));
            return WGLR_OK;
        }
    
        /*get template image information*/
        if(HR_OK != handle_get_obj((HANDLE_T)_GET_SYS_TPL_IMG(ui2_image_id), (void**)&pt_template))
        {
            DBG_ERROR(("{WGL(SETS)} ERR: Fail to get this system image instance. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
            return WGLR_INTERNAL_ERROR;
        }
        if(NULL == pt_template)
        {
            DBG_ERROR(("{WGL(SETS)} ERR: System image instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
            return WGLR_INTERNAL_ERROR;
        }
    
        /*prepare return value*/
        *pe_segment_type = pt_template->e_segment_type;
        *pui4_img_width  = pt_template->t_surface_info.ui4_width;
        *pui4_img_height = pt_template->t_surface_info.ui4_height;
        *ph_image        = _GET_SYS_TPL_IMG(ui2_image_id);
    }
    else
    {
        /* Note: WGL does not provide system image with palette mode in this version. */
        /*       Simply return NULL_HANDLE ! */
        wgl_custom_try_get_compatible_sys_img(ui2_image_id, pt_cvsst, pe_segment_type, pui4_img_width, pui4_img_height, ph_image);
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
INT32 wgl_get_system_color(
    UINT16                      ui2_color_id,
    WGL_CANVAS_STATUS_T*        pt_cvsst,
    GL_COLOR_T*                 pt_color)
{
    GL_COLOR_T                  t_orig_sys_color;
    
    if(!IS_LEGAL_SYS_CLR_ID(ui2_color_id))
    {
        DBG_ERROR(("{WGL(SETS)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    
    if(_IS_SYS_COLOR_COMPATIBLE_WITH_CANVAS(pt_cvsst))
    {
        *pt_color = _GET_SYS_COLOR(ui2_color_id);
    }
    else
    {
        t_orig_sys_color = _GET_SYS_COLOR(ui2_color_id);
        return wgl_custom_try_convert_sys_color(
            &t_orig_sys_color, 
            pt_cvsst,
            pt_color); 
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
INT32 wgl_register_comp_intf(
    UINT32                          ui4_wgl_type,
    const WGL_WIDGET_CTRL_INTF_T*   pt_wgl_intf)
{
    WGL_WIDGET_INTF_TABLE_T*        pt_table;
    WIDGET_INTF_RECORD_T*           pt_new_record;
    UINT32                          ui4_index;
    SIZE_T                          z_size;

    /*check parameter*/
    if(NULL == pt_wgl_intf)
    {
        return WGLR_INV_ARG;
    }

    /*check if the type has been registered*/
    for (ui4_index = 0; ui4_index < _GET_SYS_WIDGET_INTF_NUMBERS(); ui4_index++)
    {
        if (ui4_wgl_type == _GET_SYS_WIDGET_INTF_RECORD(ui4_index)->e_type)
        {
            return WGLR_INV_ARG;
        }
    }

    /*check if there's free record*/
    pt_table = _GET_SYS_WIDGET_INTF_TABLE();
    if(pt_table->ui4_alloc_count <= pt_table->ui4_intf_count)
    {
        /*realloc a new buffer*/
        z_size = (pt_table->ui4_alloc_count + _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT) * sizeof(WIDGET_INTF_RECORD_T);
        pt_table->at_intf_records = x_mem_realloc(pt_table->at_intf_records, z_size);
        if(NULL == pt_table->at_intf_records)
        {
            pt_table->ui4_intf_count = 0;
            pt_table->ui4_alloc_count = 0;
            return WGLR_OUT_OF_MEMORY;
        }
        
        /*clean the new buffer*/
        x_memset(pt_table->at_intf_records + pt_table->ui4_intf_count, 0, _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT * sizeof(WIDGET_INTF_RECORD_T));
        pt_table->ui4_alloc_count += _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT;

    }

    /*add a record*/
    pt_new_record = _GET_SYS_WIDGET_INTF_RECORD(pt_table->ui4_intf_count);
    pt_new_record->e_type = (HANDLE_TYPE_T)ui4_wgl_type;
    x_memcpy(&pt_new_record->t_itf, pt_wgl_intf, sizeof(WGL_WIDGET_CTRL_INTF_T));
    
    pt_table->ui4_intf_count++;

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
INT32 wgl_register_content_intf(
    UINT32                          ui4_cnt_type,
    const WGL_CONTENT_CTRL_INTF_T*  pt_cnt_intf)
{
    WGL_CONTENT_INTF_TABLE_T*       pt_table;
    CONTENT_INTF_RECORD_T*          pt_new_record;
    UINT32                          ui4_index;
    SIZE_T                          z_size;
    
    /*check parameter*/
    if(NULL == pt_cnt_intf)
    {
        return WGLR_INV_ARG;
    }
    
    /*check if the type has been registered*/
    for (ui4_index = 0; ui4_index < _GET_SYS_CNT_INTF_NUMBERS(); ui4_index++)
    {
        if (ui4_cnt_type == _GET_SYS_CNT_INTF_RECORD(ui4_index)->ui4_cnt_type)
        {
            return WGLR_INV_ARG;
        }
    }
    
    /*check if there's free record*/
    pt_table = _GET_SYS_CNT_INTF_TABLE();
    if(pt_table->ui4_alloc_count <= pt_table->ui4_intf_count)
    {
        /*realloc a new buffer*/
        z_size = (pt_table->ui4_alloc_count + _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT) * sizeof(CONTENT_INTF_RECORD_T);
        pt_table->at_intf_records = x_mem_realloc(pt_table->at_intf_records, z_size);
        if(NULL == pt_table->at_intf_records)
        {
            pt_table->ui4_intf_count = 0;
            pt_table->ui4_alloc_count = 0;
            return WGLR_OUT_OF_MEMORY;
        }
        
        /*clean the new buffer*/
        x_memset(pt_table->at_intf_records + pt_table->ui4_intf_count, 0, _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT * sizeof(CONTENT_INTF_RECORD_T));
        pt_table->ui4_alloc_count += _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT;
    }
    
    /*add a record*/
    pt_new_record = _GET_SYS_CNT_INTF_RECORD(pt_table->ui4_intf_count);
    pt_new_record->ui4_cnt_type = ui4_cnt_type;
    x_memcpy(&pt_new_record->t_cnt_itf, pt_cnt_intf, sizeof(WGL_CONTENT_CTRL_INTF_T));
    
    pt_table->ui4_intf_count++;
    
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
INT32 wgl_register_border_intf(
    UINT32                          ui4_bdr_type,
    const WGL_BORDER_CTRL_INTF_T*   pt_bdr_intf)
{
    WGL_BORDER_INTF_TABLE_T*        pt_table;
    BORDER_INTF_RECORD_T*           pt_new_record;
    UINT32                          ui4_index;
    SIZE_T                          z_size;
    
    /*check parameter*/
    if(NULL == pt_bdr_intf)
    {
        return WGLR_INV_ARG;
    }
    
    /*check if the type has been registered*/
    for (ui4_index = 0; ui4_index < _GET_SYS_BDR_INTF_NUMBERS(); ui4_index++)
    {
        if (ui4_bdr_type == _GET_SYS_BDR_INTF_RECORD(ui4_index)->ui4_bdr_type)
        {
            return WGLR_INV_ARG;
        }
    }
    
    /*check if there's free record*/
    pt_table = _GET_SYS_BDR_INTF_TABLE();
    if(pt_table->ui4_alloc_count <= pt_table->ui4_intf_count)
    {
        /*realloc a new buffer*/
        z_size = (pt_table->ui4_alloc_count + _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT) * sizeof(BORDER_INTF_RECORD_T);
        pt_table->at_intf_records = x_mem_realloc(pt_table->at_intf_records, z_size);
        if(NULL == pt_table->at_intf_records)
        {
            pt_table->ui4_intf_count = 0;
            pt_table->ui4_alloc_count = 0;
            return WGLR_OUT_OF_MEMORY;
        }
        
        /*clean the new buffer*/
        x_memset(pt_table->at_intf_records + pt_table->ui4_intf_count, 0, _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT * sizeof(BORDER_INTF_RECORD_T));
        pt_table->ui4_alloc_count += _WGL_SYS_DEFAULT_INTF_TABLE_GROW_COUNT;
    }
    
    /*add a record*/
    pt_new_record = _GET_SYS_BDR_INTF_RECORD(pt_table->ui4_intf_count);
    pt_new_record->ui4_bdr_type = ui4_bdr_type;
    x_memcpy(&pt_new_record->t_bdr_itf, pt_bdr_intf, sizeof(WGL_BORDER_CTRL_INTF_T));
    
    pt_table->ui4_intf_count++;
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Image
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 wgl_query_comp_intf (
    HANDLE_TYPE_T               e_type,
    VOID**                      ppv_intf_out)
{
    UINT32                      i;
    
    if (!ppv_intf_out)
    {
        return WGLR_INV_ARG;
    }
    
    for (i = 0; i < _GET_SYS_WIDGET_INTF_NUMBERS(); i++)
    {
        if (e_type == _GET_SYS_WIDGET_INTF_RECORD(i)->e_type)
        {
            *ppv_intf_out = (VOID*) &(_GET_SYS_WIDGET_INTF_RECORD(i)->t_itf);
            return WGLR_OK;
        }
    }
    
    return WGLR_INV_TYPE;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Image
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/            
INT32 wgl_query_content_intf(
    UINT32                      ui4_cnt_type,
    WGL_CONTENT_CTRL_INTF_T*    pt_cnt_intf)
{
    /*local variable declaration*/
    UINT32                      i;
    
    if (NULL == pt_cnt_intf)
    {
        return WGLR_INV_ARG;
    }
    
    for (i = 0; i < _GET_SYS_CNT_INTF_NUMBERS(); i++)
    {
        if (ui4_cnt_type == _GET_SYS_CNT_INTF_RECORD(i)->ui4_cnt_type)
        {
            *pt_cnt_intf = _GET_SYS_CNT_INTF_RECORD(i)->t_cnt_itf;
            return WGLR_OK;
        }
    }
    
    return WGLR_INV_TYPE;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Get System Image
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 wgl_query_border_intf(
    UINT32                      ui4_bdr_type,
    WGL_BORDER_CTRL_INTF_T*     pt_bdr_intf)
{
    /*local variable declaration*/
    UINT32                      i;

    if (NULL == pt_bdr_intf)
    {
        return WGLR_INV_ARG;
    }

    for (i = 0; i < _GET_SYS_BDR_INTF_NUMBERS(); i++)
    {
        if (ui4_bdr_type == _GET_SYS_BDR_INTF_RECORD(i)->ui4_bdr_type)
        {
            *pt_bdr_intf = _GET_SYS_BDR_INTF_RECORD(i)->t_bdr_itf;
            return WGLR_OK;
        }
    }

    return WGLR_INV_TYPE;
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
INT32 wgl_register_autofree_fcts(VOID)
{
    static handle_autofree_fct apf_autofree_fcts[HT_GROUP_SIZE] = {0};
    
    /*set auto free function*/
    apf_autofree_fcts[0] = _wgl_handle_autofree;
    
    /*register function-table to aee*/
    /*For CQ[DTV00000412] When calling a_amb_exit_app to exit an application, the system will abort at handle free.*/
    /*solution: replace HT_GROUP_WGL by HT_WGL_IMG_TPL*/
    if (handle_set_autofree_tbl(HT_WGL_IMG_TPL, apf_autofree_fcts) != HR_OK)
    {
        return WGLR_INIT_FAILED;
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
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_handle_autofree (
    HANDLE_T                       h_handle,
    HANDLE_TYPE_T                  e_type)
{
    INT32 i4_ret;
    
    if (e_type != HT_WGL_IMG_TPL)
    {
        return HR_INV_HANDLE_TYPE;
    }

    switch (e_type)
    {
        case HT_WGL_IMG_TPL:
            i4_ret = x_wgl_img_tpl_destroy(h_handle);
            break;

        default: /* widget handle types */
            i4_ret = WGLR_INV_HANDLE;
            break;
    }
    
    return (i4_ret == WGLR_OK ? HR_OK : HR_INV_HANDLE);
}
