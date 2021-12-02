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
 * $RCSfile: wgl_animation_cnt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 8db651ed8bc3a4bc366ac1385fd1ebda $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
#include "os/inc/x_os.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/x_wgl_sets.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
#include "wgl_animation_cnt.h"
#include "wgl_animation_cli.h"
#include "u_wgl_animation.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_anim_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif



/*Extra Flag*/
#define ANIM_BK_CLR_NOT_INIT        ((UINT8)0x01)
#define ANIM_BK_IMG_NOT_INIT        ((UINT8)0x02)
#define ANIM_IMG_UI                 ((UINT8)0x04)
#define ANIM_NO_BK                  ((UINT8)0x08)
#define ANIM_USE_SYS_IMG            ((UINT8)0x10)
#define ANIM_NOT_CLEAN_BK           ((UINT8)0x20)


typedef struct _WGL_ANIM_CONTENT_T
{
    WGL_COLOR_STD_SET_T             t_clr_bk;       /*background color set*/
    WGL_IMG_STD_SET_T               t_img_bk;       /*background template image*/
    WGL_IMG_STD_SET_T               t_fg_img;       /*foreground image*/
    GL_RECT_T                       t_cnt_rect;     /*content rect*/
    WGL_INSET_T                     t_fg_inset;     /*inset for foreground icon*/
    UINT8                           ui1_align;      /*alignment*/
    UINT8                           ui1_extra_flag; /*Flag*/   
    WGL_CANVAS_STATUS_T             t_cvsst;        /*canvas status*/ 
    WGL_HIMG_TPL_T                  h_playing_image;
    
} WGL_ANIM_CONTENT_T;


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _wgl_anim_cnt_paint_bk(
    WGL_ANIM_CONTENT_T*             pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state);

static INT32 _wgl_anim_cnt_paint_fg(
    WGL_ANIM_CONTENT_T*             pt_data, 
    GL_HGRAPHICS_T                  h_graphics,
    UINT32                          ui4_upt_rect,
    const GL_RECT_T*                pt_upt_rect,
    UINT16                          ui2_state);

static INT32 _wgl_anim_cnt_set_img(
    WGL_ANIM_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_IMG_INFO_T*           pt_img_info);

static INT32 _wgl_anim_cnt_get_img(
    WGL_ANIM_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    WGL_IMG_INFO_T*                 pt_img_info);

static INT32 _wgl_anim_cnt_set_color(
    WGL_ANIM_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    const WGL_COLOR_INFO_T*         pt_clr_info);

static INT32 _wgl_anim_cnt_get_color(
    WGL_ANIM_CONTENT_T*             pt_cnt_data,
    UINT8                           ui1_target,
    WGL_COLOR_INFO_T*               pt_clr_info);

/*WGL_CMD_ICON_GET_MIN_SIZE command handler*/
static INT32 _wgl_anim_cnt_get_min_size(
    const WGL_ANIM_CONTENT_T*       pt_cnt_data,
    GL_SIZE_T*                      pt_size);

static INT32 _wgl_anim_cnt_prepare_img(
    WGL_ANIM_CONTENT_T*             pt_cnt_data);

static INT32 _wgl_anim_cnt_use_sys_color(
    WGL_ANIM_CONTENT_T*             pt_cnt_data);


/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 *              content create function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 wgl_anim_cnt_inst_create( 
    WGL_CONTENT_CONTEXT_HANDLE_T*       ph_cnt_data, 
    WGL_CONTENT_CREATE_INPUT_T*         pt_param)
{
    /*local variable declaration*/
    WGL_ANIM_CONTENT_T*                 pt_data;
    UINT32                              ui4_style = ((WGL_ANIM_INIT_T *)(pt_param->pv_init))->ui4_style;
   
    
    DBG_API(("{WGL(ANIM_CNT)} %s: ph_cnt_data = 0x%.8x \tpt_param = 0x%.8x\n", __FUNCTION__, ph_cnt_data, pt_param));

    /*check parameter*/
    if(NULL == ph_cnt_data || NULL == pt_param)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    DBG_INFO(("{WGL(ANIM_CNT)} pt_param->pv_init = 0x%.8x   ui4_type_border = 0x%.8x\n"
                             "\tt_rect_cnt(l, t, r, b) = (%d, %d, %d, %d)\n",
                             pt_param->pv_init,
                             pt_param->t_rect_cnt.i4_left,
                             pt_param->t_rect_cnt.i4_top,
                             pt_param->t_rect_cnt.i4_right,
                             pt_param->t_rect_cnt.i4_bottom));

    /*allocate memory for content data*/    
    pt_data = (WGL_ANIM_CONTENT_T*) WGL_MEM_ALLOC(sizeof(WGL_ANIM_CONTENT_T));
    if(NULL == pt_data)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to alloc memory for content instance. (%d) at L%d\n\n", WGLR_OUT_OF_MEMORY, __LINE__));
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize content data*/
    x_memset(pt_data, 0, sizeof(WGL_ANIM_CONTENT_T));
    pt_data->ui1_extra_flag = ANIM_BK_CLR_NOT_INIT | ANIM_BK_IMG_NOT_INIT;
    pt_data->ui1_align = WGL_AS_LEFT_TOP;

    /*set bk style*/
    pt_data->ui1_extra_flag |= (ui4_style & WGL_STL_GL_NO_BK) ? ANIM_NO_BK : 0;

    /*store settings*/
    WGL_RECT_COPY(&pt_data->t_cnt_rect, &pt_param->t_rect_cnt);
    pt_data->ui1_extra_flag |= (WGL_IS_IMAGE_UI(ui4_style)) ? ANIM_IMG_UI : 0;
    pt_data->ui1_extra_flag |= (WGL_STL_GL_USE_SYS_IMG & (ui4_style)) ? ANIM_USE_SYS_IMG : 0;    
    pt_data->ui1_extra_flag |= (WGL_STL_GL_BDR_FILL_CNT_BK & (ui4_style)) ? ANIM_NOT_CLEAN_BK : 0;

      
    /*init canvas status*/
    wgl_cvsst_copy(&pt_data->t_cvsst, pt_param->pt_cvsst);
    
    /*prepare return value*/
    *ph_cnt_data = (WGL_CONTENT_CONTEXT_HANDLE_T)pt_data;
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content destroy function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_anim_cnt_inst_destroy(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data)
{
    DBG_API(("{WGL(ANIM_CNT)} %s: h_cnt_data = 0x%.8x\n", __FUNCTION__, h_cnt_data));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*free control logic object's resource*/
    WGL_MEM_FREE((WGL_ANIM_CONTENT_T*)h_cnt_data);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content paint function type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_anim_cnt_paint(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    DBG_API(("{WGL(ANIM_CNT)} %s: h_cnt_data = 0x%.8x \th_graphics = 0x%.8x \tui2_state = %d\n", __FUNCTION__, h_cnt_data, h_graphics, ui2_state));

    /*check parameter*/
    if(NULL == h_cnt_data || NULL_HANDLE == h_graphics)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*paint background*/
    i4_ret = _wgl_anim_cnt_paint_bk((WGL_ANIM_CONTENT_T*)h_cnt_data, 
                                    h_graphics, 
                                    ui4_upt_rect, 
                                    pt_upt_rect, 
                                    ui2_state);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*paint foreground*/
    return _wgl_anim_cnt_paint_fg((WGL_ANIM_CONTENT_T*)h_cnt_data, 
                                  h_graphics, 
                                  ui4_upt_rect, 
                                  pt_upt_rect, 
                                  ui2_state);  
}

/*******************************************************************************
 * Name
 *      
 * Description
 *              content command procedure type
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                          
INT32 wgl_anim_cnt_command_proc(
    WGL_CONTENT_CONTEXT_HANDLE_T        h_cnt_data,
    UINT32                              ui4_cmd_id,
    VOID*                               pv_param1,
    VOID*                               pv_param2)
{   
    WGL_ANIM_CONTENT_T*                 pt_anim;
    
    DBG_API(("{WGL(ANIM_CNT)} %s: h_cnt_data = 0x%.8x \tui4_cmd_id = 0x%.8x \tpv_param1 = 0x%.8x \tpv_param2 = 0x%.8x\n", 
                                __FUNCTION__, 
                                h_cnt_data, 
                                ui4_cmd_id,
                                pv_param1,
                                pv_param2));

    /*check parameter*/
    if(NULL == h_cnt_data)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. The 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
    pt_anim = (WGL_ANIM_CONTENT_T*)h_cnt_data;
    if(!(IS_ANIM_CNT_CMD(ui4_cmd_id) || WGL_IS_CMD_GENERAL(ui4_cmd_id)))
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Unknown command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
    }

    switch (ui4_cmd_id)
    {
    case WGL_CMD_GL_SET_IMAGE:
        return _wgl_anim_cnt_set_img(pt_anim, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_IMAGE:
        return _wgl_anim_cnt_get_img(pt_anim, WGL_POINTER_TO_UINT8(pv_param1), (WGL_IMG_INFO_T*)pv_param2);

    case WGL_CMD_GL_SET_COLOR:
        return _wgl_anim_cnt_set_color(pt_anim, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_GL_GET_COLOR:
        return _wgl_anim_cnt_get_color(pt_anim, WGL_POINTER_TO_UINT8(pv_param1), (WGL_COLOR_INFO_T*)pv_param2);

    case WGL_CMD_ANIM_SET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CMD_ANIM_SET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(ANIM_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                        pv_param1,
                        ((WGL_INSET_T*)pv_param1)->i4_left,
                        ((WGL_INSET_T*)pv_param1)->i4_top,
                        ((WGL_INSET_T*)pv_param1)->i4_right,
                        ((WGL_INSET_T*)pv_param1)->i4_bottom));

            if(FALSE == wgl_is_legal_inset(&pt_anim->t_cnt_rect, (WGL_INSET_T*)pv_param1))
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CMD_ANIM_SET_CNT_INSET, the inset specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET(&pt_anim->t_fg_inset, (WGL_INSET_T*)pv_param1);
        }
        break;

    case WGL_CMD_ANIM_GET_CNT_INSET:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(ICON_CNT)} ERR: Invalid arguments. For WGL_CMD_ANIM_GET_CNT_INSET, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            WGL_COPY_INSET((WGL_INSET_T*)pv_param1, &((WGL_ANIM_CONTENT_T*)h_cnt_data)->t_fg_inset);
        }
        break;

    case WGL_CMD_ANIM_SET_ALIGN:
        {
            if(FALSE == wgl_is_legal_alignment(WGL_POINTER_TO_UINT8(pv_param1)))
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CMD_ANIM_SET_ALIGN, the alignment specified by 1st argument is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }
            
            ((WGL_ANIM_CONTENT_T*)h_cnt_data)->ui1_align = WGL_POINTER_TO_UINT8(pv_param1);
        }
        break;

    case WGL_CMD_ANIM_GET_ALIGN:
        {
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CMD_ANIM_GET_ALIGN, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            *((UINT8*)pv_param1) = ((WGL_ANIM_CONTENT_T*)h_cnt_data)->ui1_align;
        }
        break;

    case WGL_CNT_ANIM_SET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CNT_ANIM_SET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            DBG_INFO(("{WGL(ANIM_CNT)} pv_param1(l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n",
                    pv_param1,
                    ((GL_RECT_T*)pv_param1)->i4_left,
                    ((GL_RECT_T*)pv_param1)->i4_top,
                    ((GL_RECT_T*)pv_param1)->i4_right,
                    ((GL_RECT_T*)pv_param1)->i4_bottom));

            /*copy rect*/
            WGL_RECT_COPY(&pt_anim->t_cnt_rect, (GL_RECT_T*)pv_param1);           
        }
        break;

    case WGL_CNT_ANIM_GET_RECT:
        {
            /*check parameter*/
            if(NULL == pv_param1)
            {
                DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. For WGL_CNT_ANIM_GET_RECT, the 1st argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
                return WGLR_INV_ARG;
            }

            /*prepare return value*/
            WGL_RECT_COPY((GL_RECT_T*)pv_param1, &((WGL_ANIM_CONTENT_T*)h_cnt_data)->t_cnt_rect);
        }
        break;

    case WGL_CNT_ANIM_GET_MIN_SIZE:
        return _wgl_anim_cnt_get_min_size(pt_anim, (GL_SIZE_T*)pv_param1);
        
    case WGL_CNT_ANIM_UPDATE_CVSST:
        return wgl_update_cvvst((WGL_CVS_CLR_CFG_T*)pv_param1, &((WGL_ANIM_CONTENT_T*)h_cnt_data)->t_cvsst);
    case WGL_CMD_ANIM_SET_CNT_PLAY_IMG:
        ((WGL_ANIM_CONTENT_T*)h_cnt_data)->h_playing_image = (WGL_HIMG_TPL_T)pv_param1;; 
        break;
    case WGL_CMD_GL_SET_FONT:
    case WGL_CMD_GL_GET_FONT:
    default:
        DBG_ERROR(("{WGL(ICON_CNT)} ERR: Unknown or unsupported command. (%d) at L%d\n\n", WGLR_INV_CMD, __LINE__));
        return WGLR_INV_CMD;
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
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_anim_cnt_paint_bk(
    WGL_ANIM_CONTENT_T*                 pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    WGL_HIMG_TPL_T                      h_image;
    GL_COLOR_T                          t_clr;
    INT32                               i4_ret;

    /*update info*/
    if(ANIM_NO_BK & pt_data->ui1_extra_flag)
    {
        return WGLR_OK;
    }
    else if(!(ANIM_IMG_UI & pt_data->ui1_extra_flag))
      {
        /*check and cache color*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT))
        {
            i4_ret = _wgl_anim_cnt_use_sys_color(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT);
        }

        /*select color*/
        if(IS_STATE_ACTIVATED(ui2_state))
        {
            t_clr = pt_data->t_clr_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            t_clr = pt_data->t_clr_bk.t_enable;
        }
        else
        {
            t_clr = pt_data->t_clr_bk.t_disable;
        }

        /*should check ICON_NOT_CLEAN_BK style here*/

        /*paint background*/
        i4_ret = wgl_fill_widget_bk(h_graphics, &t_clr, &pt_data->t_cnt_rect, &pt_data->t_cvsst);
        WGL_ANIM_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to fill background color. (%d) at L%d\n\n", i4_ret, __LINE__)));
        return i4_ret;
    }
    else
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_anim_cnt_prepare_img(pt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
            
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT);
        }
        
        /*select image*/
        if(IS_STATE_FOCUSED(ui2_state))
        {
            h_image = pt_data->t_img_bk.t_highlight;
        }
        else if(IS_STATE_ENABLED(ui2_state))
        {
            h_image = pt_data->t_img_bk.t_enable;
        }
        else
        {
            h_image = pt_data->t_img_bk.t_disable;
        }

        /*paint background*/
        if(NULL_HANDLE != h_image)
        {
            i4_ret = wgl_img_tpl_paint(h_image, 
                                       &pt_data->t_cnt_rect, 
                                       pt_upt_rect,
                                       h_graphics, 
                                       &pt_data->t_cvsst,
                                       (WGL_IS_FLAG_TURN_ON(pt_data->ui1_extra_flag, ANIM_NOT_CLEAN_BK)) 
                                       ? WGL_TPL_PAINT_COMPOSE
                                       : WGL_TPL_PAINT_BITBLT);
        
            WGL_ANIM_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to draw image UI to background. (%d) at L%d\n\n", i4_ret, __LINE__)));
        }
        else
        {
            i4_ret = WGLR_OK;
        }
        
        return i4_ret;
    }
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
static INT32 _wgl_anim_cnt_paint_fg(
    WGL_ANIM_CONTENT_T*                 pt_data, 
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*local variable declaration*/
    UINT32                              ui4_img_width = 0;
    UINT32                              ui4_img_height = 0;
    UINT32                              ui4_idx;
    WGL_HIMG_TPL_T                      h_image;
    GL_RECT_T                           t_icon_rect, t_available_rect;
    INT32                               i4_ret;
    INT32                               i4_x_offset, i4_y_offset;
    UINT32                              ui4_avail_width, ui4_avail_height;
    
    /*select image*/
   
    if(IS_STATE_FOCUSED(ui2_state))
    {
        h_image = pt_data->t_fg_img.t_highlight;
    }
    else if(IS_STATE_ENABLED(ui2_state))
    {
        h_image = pt_data->t_fg_img.t_enable;
    }
    else
    {
        h_image = pt_data->t_fg_img.t_disable;
    }


    if(NULL_HANDLE == h_image && pt_data->h_playing_image == NULL_HANDLE)
    {
        return WGLR_OK;
    }
    for (ui4_idx = 0; ui4_idx < 2; ui4_idx++)
    {
        if (ui4_idx == 1)
        {
          h_image = pt_data->h_playing_image; 
        }
        if (h_image == NULL_HANDLE)
        {
            continue;
        }

        /*get image size*/
        i4_ret = x_wgl_img_tpl_get_size(h_image, &ui4_img_width, &ui4_img_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get image's size. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        /*calculate available rectangle*/
        WGL_RECT_COPY(&t_available_rect, &pt_data->t_cnt_rect);
        WGL_RECT_INSET(&t_available_rect, 
                       pt_data->t_fg_inset.i4_left, 
                       pt_data->t_fg_inset.i4_top,
                       pt_data->t_fg_inset.i4_right, 
                       pt_data->t_fg_inset.i4_bottom);

        ui4_avail_width = (UINT32)WGL_RECT_GET_WIDTH(&t_available_rect);
        ui4_avail_height = (UINT32)WGL_RECT_GET_HEIGHT(&t_available_rect);

        /*calculate start position in x-axis*/
        if(WGL_IS_H_LEFT(pt_data->ui1_align))
        {
            i4_x_offset = 0;
        }
        else if(WGL_IS_H_CENTER(pt_data->ui1_align))
        {
            i4_x_offset = (INT32)((ui4_avail_width > ui4_img_width) ? ((ui4_avail_width - ui4_img_width) >> 1) : 0);
        }
        else
        {
            i4_x_offset = (INT32)((ui4_avail_width > ui4_img_width) ? (ui4_avail_width - ui4_img_width) : 0);
        }

        /*calculate start position in y-axis*/
        if(WGL_IS_V_TOP(pt_data->ui1_align))
        {
            i4_y_offset = 0;
        }
        else if(WGL_IS_V_CENTER(pt_data->ui1_align))
        {
            i4_y_offset = (INT32)((ui4_avail_height > ui4_img_height) ? ((ui4_avail_height - ui4_img_height) >> 1) : 0);
        }
        else
        {
            i4_y_offset = (INT32)((ui4_avail_height > ui4_img_height) ? (ui4_avail_height - ui4_img_height) : 0);
        }

        /*offset the available rectangle*/
        /*WGL_RECT_OFFSET(&t_available_rect, i4_x_offset, i4_y_offset);*/
        t_icon_rect.i4_left = i4_x_offset + t_available_rect.i4_left;
        t_icon_rect.i4_top = i4_y_offset + t_available_rect.i4_top;
        t_icon_rect.i4_right = t_icon_rect.i4_left + ((ui4_img_width > ui4_avail_width) ? ui4_avail_width : ui4_img_width);
        t_icon_rect.i4_bottom = t_icon_rect.i4_top + ((ui4_img_height > ui4_avail_height) ? ui4_avail_height : ui4_img_height);

        /*paint foreground*/
        i4_ret = wgl_img_tpl_paint(h_image, 
                                   &t_icon_rect, 
                                   pt_upt_rect,
                                   h_graphics, 
                                   &pt_data->t_cvsst,
                                   WGL_TPL_PAINT_COMPOSE); 
        WGL_ANIM_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(ANIM_CNT)} ERR=%d Fail to draw image.File=%s,Line=%d\n\n",i4_ret, __FILE__, __LINE__)));       

    }

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
static INT32 _wgl_anim_cnt_set_img(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_IMG_INFO_T*               pt_img_info)
{   

    DBG_INFO(("{WGL(ANIM_CNT)} pt_img_info = 0x%.8x   e_type = 0x%.8x\n"
                             "\t(enable, disable) = (0x%.8x, 0x%.8x)\n"
                             "\t(highlight, highlight_inactive) = (0x%.8x, 0x%.8x)\n"
                             "\t(push, push_unhighlight) = (0x%.8x, 0x%.8x)\n",
                             pt_img_info,
                             ((NULL == pt_img_info) ? 0 : pt_img_info->e_type),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_enable),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_disable),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_highlight),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_highlight_inactive),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_push),
                             ((NULL == pt_img_info) ? 0 : pt_img_info->u_img_data.t_extend.t_push_unhighlight)));
    
    /*check pt_img_info*/
    if(pt_img_info && FALSE == wgl_is_legal_img_info(pt_img_info))
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: The image info is illegal. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK == ui1_target)
    {
        /*update background image*/
        if(pt_img_info)
        {
            /*store pt_img_info*/
            COPY_IMG_INFO_TO_STD(&pt_cnt_data->t_img_bk, pt_img_info, NULL_HANDLE);

            /*remove the flag for action image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT);
        }
        else
        {
            x_memset(&pt_cnt_data->t_img_bk, 0, sizeof(WGL_IMG_STD_SET_T));

            /*reset the flag for bk image*/
            WGL_SET_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT);
        }
    }
    else if(WGL_IMG_FG == ui1_target)
    {
        /*update icon image*/
        if(pt_img_info)
        {
            COPY_IMG_INFO_TO_STD(&pt_cnt_data->t_fg_img, pt_img_info, NULL_HANDLE);
        }
        else
        {
            x_memset(&pt_cnt_data->t_fg_img, 0, sizeof(WGL_IMG_STD_SET_T));
        }
    }
    else
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK or WGL_IMG_FG. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }
    

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
static INT32 _wgl_anim_cnt_get_img(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_IMG_INFO_T*                     pt_img_info)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    /*check parameter*/
    if(NULL == pt_img_info)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid arguments. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(WGL_IMG_BK == ui1_target)
    {
        /*check and cache images*/
        if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT))
        {
            i4_ret = _wgl_anim_cnt_prepare_img(pt_cnt_data);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret; 
            }
        
            /*remove the flag for bk image*/
            WGL_CLEAR_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_IMG_NOT_INIT);
        }

        /*prepare return value*/
        SET_STANDARD_TO_IMG_INFO(pt_img_info, &pt_cnt_data->t_img_bk);
    }
    else if(WGL_IMG_FG == ui1_target)
    {
        /*prepare return value*/
        SET_STANDARD_TO_IMG_INFO(pt_img_info, &pt_cnt_data->t_fg_img);
    }
    else
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Unknown image target. The legal value is WGL_IMG_BK or WGL_IMG_FG. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }
    
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
static INT32 _wgl_anim_cnt_set_color(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    const WGL_COLOR_INFO_T*             pt_clr_info)
{
    /*local variable variable*/
    GL_COLOR_T                          t_default_color;

    DBG_INFO(("{WGL(ANIM_CNT)} pt_clr_info = 0x%.8x   e_type = 0x%.8x\n"
                             "\t(enable, disable) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n"
                             "\t(highlight, highlight_inactive) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n"
                             "\t(t_push, push_unhighlight) = (0x%.2x%.2x%.2x%.2x, 0x%.2x%.2x%.2x%.2x)\n",
                             pt_clr_info,
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->e_type),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_enable.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_disable.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_highlight_inactive.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push.u3.b),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.a),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u1.r),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u2.g),
                             ((NULL == pt_clr_info) ? 0 : pt_clr_info->u_color_data.t_extend.t_push_unhighlight.u3.b)));

    /*check target*/
    if(WGL_CLR_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }
    
    /*init default color*/
    wgl_cvsst_get_transparent_color(&pt_cnt_data->t_cvsst, &t_default_color);

    if(pt_clr_info)
    {
        /*copy bk color*/
        COPY_CLR_INFO_TO_STD(&pt_cnt_data->t_clr_bk, pt_clr_info, t_default_color);

        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT);
    }
    else
    {
        /*reset the flag for bk color*/
        WGL_SET_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT);
    }
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
static INT32 _wgl_anim_cnt_get_color(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data,
    UINT8                               ui1_target,
    WGL_COLOR_INFO_T*                   pt_clr_info)
{
    /*local variable declaration*/
    INT32                               i4_ret;

    /*check parameter*/
    if(NULL == pt_clr_info)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Invalid input parameter. For WGL_CMD_GL_GET_COLOR, the output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    /*check target*/
    if(WGL_CLR_BK != ui1_target)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Unknown color target. The legal value is WGL_CLR_BK. (%d) at L%d\n\n", WGLR_NOT_SUPPORT_TARGET, __LINE__));
        return WGLR_NOT_SUPPORT_TARGET;
    }

    if(WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT))
    {
        /*get system colors*/
        i4_ret = _wgl_anim_cnt_use_sys_color(pt_cnt_data);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }
        /*remove the flag for bk color*/
        WGL_CLEAR_FLAG(pt_cnt_data->ui1_extra_flag, ANIM_BK_CLR_NOT_INIT);
    }

    /*prepare return value*/
    SET_STANDARD_TO_COLOR_INFO(pt_clr_info, &pt_cnt_data->t_clr_bk);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_CMD_ICON_GET_MIN_SIZE command handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_anim_cnt_get_min_size(
    const WGL_ANIM_CONTENT_T*           pt_cnt_data,
    GL_SIZE_T*                          pt_size)
{
    /*local variable declaration*/
    UINT32                              ui4_width = 0;
    UINT32                              ui4_height = 0;
    INT32                               i4_ret;
    WGL_HIMG_TPL_T                      h_img;
    
    /*inset size*/
    pt_size->ui4_width = (UINT32)(pt_cnt_data->t_fg_inset.i4_left + pt_cnt_data->t_fg_inset.i4_right);
    pt_size->ui4_height = (UINT32)(pt_cnt_data->t_fg_inset.i4_top + pt_cnt_data->t_fg_inset.i4_bottom);

    /*calculate the foreground icon size*/
    h_img = WGL_STD_GET_NON_NULL_IMG(&pt_cnt_data->t_fg_img);

    if(h_img)
    {
        i4_ret = x_wgl_img_tpl_get_size(h_img, &ui4_width, &ui4_height);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get image size(handle = %d). (%d) at L%d\n\n", h_img, i4_ret, __LINE__));
            return i4_ret;
        }

        pt_size->ui4_width += ui4_width;
        pt_size->ui4_height += ui4_height;
    }

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
static INT32 _wgl_anim_cnt_prepare_img(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data)
{
    /*local variable declaration*/
    WGL_IMG_ST_T                        e_segment_type;
    UINT32                              ui4_width;
    UINT32                              ui4_height;
    WGL_HIMG_TPL_T                      h_tpl_img;
    INT32                               i4_ret;

    /*get system enable image for icon*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_enable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui1_extra_flag, ANIM_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_enable = h_tpl_img;
    }

    /*get system disable image for icon*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_disable) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui1_extra_flag, ANIM_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC_DIS, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC_DIS image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_disable = h_tpl_img;
    }

    /*get system highlight image for icon*/
    if((NULL_HANDLE == pt_cnt_data->t_img_bk.t_highlight) && WGL_IS_FLAG_TURN_ON(pt_cnt_data->ui1_extra_flag, ANIM_USE_SYS_IMG))
    {
        i4_ret = wgl_get_system_image(WGL_SYS_IMG_STATIC_HT, &pt_cnt_data->t_cvsst, &e_segment_type, &ui4_width, &ui4_height, &h_tpl_img);
        if(WGLR_OK != i4_ret)
        {
            DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get system WGL_SYS_IMG_STATIC_HT image. (%d) at L%d\n\n", i4_ret, __LINE__));
            return i4_ret;
        }

        pt_cnt_data->t_img_bk.t_highlight = h_tpl_img;
    }
    
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
static INT32 _wgl_anim_cnt_use_sys_color(
    WGL_ANIM_CONTENT_T*                 pt_cnt_data)
{
    /*local varaible declaration*/
    INT32                               i4_ret;

    /*get system enable color for icon*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_enable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get system WGL_SYS_CLR_STATIC color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /*get system disable color for icon*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC_DIS, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_disable);
    if(WGLR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(ANIM_CNT)} ERR: Fail to get system WGL_SYS_CLR_STATIC_DIS color. (%d) at L%d\n\n", i4_ret, __LINE__));
        return i4_ret;
    }

    /*get system highlight color for icon*/
    i4_ret = wgl_get_system_color(WGL_SYS_CLR_STATIC_HT, &pt_cnt_data->t_cvsst, &pt_cnt_data->t_clr_bk.t_highlight);
    WGL_ANIM_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL_ANIM_CNT} ERR: Fail to get system WGL_SYS_CLR_STATIC_HT color. (%d) at L%d\n\n", i4_ret, __LINE__)));
    return i4_ret;
}

#endif

