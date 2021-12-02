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
 * $RCSfile: sbtl_mkv_text_page_inst_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "os/inc/os.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"
#include "unicode/x_uc_str.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_widget.h"

#include "mutil/sbtl_engine/sbtl_mkv/sbtl_mkv_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_mkv/sbtl_mkv_srt_parser.h"
#include "mutil/sbtl_engine/sbtl_mkv/sbtl_mkv_ssa_parser.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "x_uc_str.h"


#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"
#include "../sbtl_widget.h"

#include "sbtl_mkv_text_page_inst_mngr.h"
#include "sbtl_mkv_srt_parser.h"
#include "sbtl_mkv_ssa_parser.h"
#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_mkv_text_timer_period(SBTL_PAGE_INST_MNGR_T*     pt_mngr);
static INT32 _sbtl_mkv_text_timer_start(SBTL_PAGE_INST_MNGR_T*       pt_mngr);
static VOID _sbtl_mkv_text_timer_stop(SBTL_PAGE_INST_MNGR_T*         pt_mngr);


static INT32 _sbtl_mkv_text_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                            PTS_T                   t_pts_current,
                                            PTS_T*                  pt_pts_start,
                                            PTS_T*                  pt_pts_end,
                                            BOOL*                   pb_found);

static INT32 _sbtl_mkv_text_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

static INT32 _sbtl_mkv_text_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_mkv_text_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

static INT32 _sbtl_mkv_text_need_dmx(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                     BOOL*                      pb_need);

static INT32 _sbtl_mkv_text_handle_timing(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                          PTS_T                     t_pts);

static INT32 _sbtl_mkv_text_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                        VOID*                    pv_get_info,
                                   UINT32                   ui4_set_type);

static VOID _sbtl_mkv_text_remove(SBTL_PAGE_INST_MNGR_T*        pt_mngr);
static INT32 _sbtl_mkv_text_new_page(SBTL_PAGE_INST_MNGR_T*     pt_mngr);


static SBTL_PAGE_INST_MNGR_T* _text_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine);
static VOID _text_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_mkv_get_subtitle_info(
    SBTL_ENGINE_T*                      pt_engine,
    UINT32                              ui4_canvas_w,
    UINT32                              ui4_canvas_h,
    GL_RECT_T*                          pt_sbtl_rect);

static INT32 _sbtl_mkv_divx_cal_region(
    SBTL_ENGINE_T*                      pt_engine,
    UINT32                              ui4_canvas_w,
    UINT32                              ui4_canvas_h,    
    GL_RECT_T*                          pt_subtitle_rect);


/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static SBTL_MNGR_FCT_TBL_T t_text_fnct_tbl = 
{
    FALSE,
    _text_page_mngr_constructor,
    _text_page_mngr_destructor,
    NULL
};

static SBTL_MKV_TEXT_FNC_TBL_T  at_fnc_tbl_list[] = 
{
    {
        MM_SBTL_TYPE_MKV_SRT,
        mkv_srt_seek_time_stamp,
        mkv_srt_load_string,
        mkv_srt_timer_period,
        mkv_srt_init,
        mkv_srt_deinit,
        mkv_srt_start,
        mkv_srt_stop,
        NULL
    },
    {
        MM_SBTL_TYPE_MKV_SSA,
        mkv_ssa_seek_time_stamp,
        mkv_ssa_load_string,
        mkv_ssa_timer_period,
        mkv_ssa_init,
        mkv_ssa_deinit,
        mkv_ssa_start,
        mkv_ssa_stop,
        NULL
    },
    {
        MM_SBTL_TYPE_END,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }
};


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static UINT8 _sbtl_mkv_get_text_line_num(UTF16_T* ps_text)
{
    UINT16      ui2_idx     = 0;
    UINT8       ui1_line_num= 1;
    UTF16_T*    ps_tmp      = ps_text;

    if(ps_text == NULL)
    {
        return 0;
    }
    
    for(ui2_idx=0; ui2_idx<TEXT_BUF_LEN_MAX; ui2_idx++)
    {
        if(*ps_tmp == 0xA)
        {
            ui1_line_num++;
        }
        else if(*ps_tmp == 0)
        {
            break;
        }
        ps_tmp++;
    }
    return ui1_line_num;
}

static INT32 _sbtl_mkv_text_update_font_attr(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT32                               i4_ret;
    FE_FNT_STYLE                        e_new_style;
    WGL_FONT_INFO_T                     t_font_inf;
    UINT8                               ui1_line_num = 0;
    static  BOOL                        fg_font_size_changed = FALSE;    
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private  = NULL;

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    e_new_style = SBTL_GET_BASE_FONT_STYLE(pt_mngr->pt_engine);
    if( SBTL_GET_FONT_STYLE_FLAG(pt_mngr->pt_engine) == TRUE)           
    {
        e_new_style |= SBTL_GET_FONT_STYLE(pt_mngr->pt_engine);
    }

#ifndef __KERNEL__
    x_dbg_stmt("[MM SBTL] new font style:%x,prev:%x\n",e_new_style,SBTL_GET_PREV_FONT_STYLE(pt_mngr->pt_engine));
#endif
    /*if(e_new_style != SBTL_GET_PREV_FONT_STYLE(pt_mngr->pt_engine))*/
    if( SBTL_GET_FONT_STYLE_FLAG(pt_mngr->pt_engine) == TRUE)           
    {
        x_memcpy(&t_font_inf, &pt_mngr->pt_engine->t_cfg.t_font_info, sizeof(WGL_FONT_INFO_T));
        if( e_new_style & (FE_FNT_STYLE)(~FE_FNT_STYLE_REGULAR) )
        {
            e_new_style &= (FE_FNT_STYLE)(~FE_FNT_STYLE_REGULAR); 
        }
        t_font_inf.e_font_style = e_new_style;
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_GL_SET_FONT,
                                  (VOID*)&t_font_inf,
                                  NULL);
        if (MMSBTLR_OK != i4_ret)
        {
            return (MMSBTLR_WGL_FAIL);
        }

#ifndef __KERNEL__
    x_dbg_stmt("[MM SBTL]set new style:%x \n",t_font_inf.e_font_style);
#else
#endif
        SBTL_SET_PREV_FONT_STYLE(pt_mngr->pt_engine,e_new_style);
    }
    
    ui1_line_num = _sbtl_mkv_get_text_line_num(pt_private->w2s_text);
    if(ui1_line_num > 3)
    {
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_TEXT_SET_DISPLAY_START,
                                  (VOID*)0,
                                  NULL);
    }
    if(ui1_line_num == 3)
    {
        if(!fg_font_size_changed)
        {
            x_memcpy(&t_font_inf, &pt_mngr->pt_engine->t_cfg.t_font_info, sizeof(WGL_FONT_INFO_T));
            t_font_inf.e_font_size = FE_FNT_SIZE_CUSTOM;
            t_font_inf.ui1_custom_size -= 4;
            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_GL_SET_FONT,
                                      (VOID*)&t_font_inf,
                                      NULL);
            if (MMSBTLR_OK != i4_ret)
            {
                return (MMSBTLR_WGL_FAIL);
            }
            fg_font_size_changed = TRUE;
        }
    }
    else if(fg_font_size_changed)
    {
        x_memcpy(&t_font_inf, &pt_mngr->pt_engine->t_cfg.t_font_info, sizeof(WGL_FONT_INFO_T));
        t_font_inf.e_font_size = FE_FNT_SIZE_CUSTOM;
        t_font_inf.ui1_custom_size += 4;
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_GL_SET_FONT,
                                  (VOID*)&t_font_inf,
                                  NULL);
        if (MMSBTLR_OK != i4_ret)
        {
            return (MMSBTLR_WGL_FAIL);
        }
        fg_font_size_changed = FALSE;
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                            PTS_T                   t_pts_current,
                                            PTS_T*                  pt_pts_start,
                                            PTS_T*                  pt_pts_end,
                                            BOOL*                   pb_found)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                               i4_ret;
    SBTL_MKV_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_seek_time, ("pt_fnc_table->pf_seek_time=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_seek_time))
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_fnc_table->pf_seek_time(pt_mngr,
                                        t_pts_current,
                                        pt_pts_start,
                                        pt_pts_end,
                                        pb_found);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                               i4_ret;
    SBTL_MKV_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_load_string, ("pt_fnc_table->pf_load_string=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_load_string))
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_fnc_table->pf_load_string(pt_mngr);
    
    if(i4_ret == MMSBTLR_OK)
    {
        i4_ret = _sbtl_mkv_text_update_font_attr(pt_mngr);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_mkv_text_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_MKV_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_timer_period, ("pt_fnc_table->pf_timer_period=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_timer_period))
    {
        return 500;
    }

    return pt_fnc_table->pf_timer_period(pt_mngr);
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_timer_cb_fct(HANDLE_T     pt_tm_handle,
                               VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*  pt_mngr;
    MM_SBTL_HDLR_MSG_T      t_msg;

    ASSERT(pv_tag, ("pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;
    mm_sbtl_hdlr_send_timer_msg(t_msg, pt_mngr);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                                i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }

    i4_ret = x_timer_create(&pt_private->h_timer);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    i4_ret = x_timer_start(pt_private->h_timer,
                           _sbtl_mkv_text_timer_period(pt_mngr),
                           X_TIMER_FLAG_REPEAT,
                           _sbtl_timer_cb_fct,
                           pt_mngr);
                    
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mkv_text_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }
}

static INT32 _sbtl_mkv_get_subtitle_info(
    SBTL_ENGINE_T*                      pt_engine,
    UINT32                              ui4_canvas_w,
    UINT32                              ui4_canvas_h,
    GL_RECT_T*                          pt_sbtl_rect)
{
    INT32               i4_ret ;
    UINT8                       ui1_font_min, ui1_font_max;
    SBTL_DISPLAY_INFO_CMD_T*    pt_disp;

    i4_ret = MMSBTLR_INV_ARG;
    pt_disp = &pt_engine->t_cfg.t_display_info;
    ui1_font_min = 0;
    ui1_font_max = 0;
    
    if ((pt_engine == NULL) || (pt_sbtl_rect == NULL) || (pt_disp == NULL))
    {
        return MMSBTLR_INV_ARG;
    }

    if(pt_disp->b_display_info == TRUE)
    {
        /*The subtitle need do the display information to create subtitle display*/
        i4_ret = _sbtl_mkv_divx_cal_region(pt_engine,ui4_canvas_w,ui4_canvas_h,pt_sbtl_rect);
        
        if(pt_disp->ui1_font_min_grade > 0)
        {
            ui1_font_min = (UINT8)((UINT32)SBTL_RECT_H(&pt_disp->t_rect_refer) / pt_disp->ui1_font_min_grade);
        }

        if(pt_disp->ui1_font_max_grade > 0)
        {
            ui1_font_max = (UINT8)((UINT32)SBTL_RECT_H(&pt_disp->t_rect_refer) / pt_disp->ui1_font_max_grade);
        }   

        if((ui1_font_min <= ui1_font_max) && (ui1_font_min != 0) && (ui1_font_max != 0))
        {
            pt_engine->t_cfg.t_font_info.e_font_size = FE_FNT_SIZE_CUSTOM;
            pt_engine->t_cfg.t_font_info.ui1_custom_size = (ui1_font_min + ui1_font_max) >> 1;
        }

    }

    if((i4_ret != MMSBTLR_OK) || (pt_disp->b_display_info == FALSE))
    {
        /*Using default to creat subtitle display  or create fail*/ 
        pt_sbtl_rect->i4_left = 0;
        pt_sbtl_rect->i4_top = 0;
        pt_sbtl_rect->i4_right = (INT32)ui4_canvas_w;
        pt_sbtl_rect->i4_bottom = (INT32)ui4_canvas_h;     
    }

    return MMSBTLR_OK;
}

static INT32 _sbtl_mkv_divx_cal_region(
    SBTL_ENGINE_T*                      pt_engine,
    UINT32                              ui4_canvas_w,
    UINT32                              ui4_canvas_h,    
    GL_RECT_T*                          pt_subtitle_rect)
{
    SBTL_DISPLAY_INFO_CMD_T*    pt_display_inf;
    GL_RECT_T                   t_disp_osd;
    GL_RECT_T*                  pt_rect;
    INT32                       i4_width,i4_height;
    INT32                       i4_tmp = 0;
    
    if((pt_engine == NULL) || (pt_subtitle_rect == NULL))
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_display_inf = &pt_engine->t_cfg.t_display_info;
    if((pt_display_inf == NULL) || (SBTL_RECT_EMPTY(&(pt_display_inf->t_rect_refer)) == TRUE))
    {
        return MMSBTLR_INV_ARG;
    }

    x_memset(pt_subtitle_rect,0,sizeof(GL_RECT_T));
    x_memset(&t_disp_osd,0,sizeof(GL_RECT_T));

    if(pt_display_inf->ui4_scrn_width <=0)
    {
        pt_display_inf->ui4_scrn_width = (UINT32)SBTL_RECT_W(&pt_display_inf->t_rect_refer);
    }

    if(pt_display_inf->ui4_scrn_height <=0)
    {
        pt_display_inf->ui4_scrn_height = (UINT32)SBTL_RECT_H(&pt_display_inf->t_rect_refer);
    }
    
    pt_rect = &pt_display_inf->t_rect_refer;
    t_disp_osd.i4_left = (((INT32)ui4_canvas_w)*pt_rect->i4_left)/((INT32)pt_display_inf->ui4_scrn_width);
    t_disp_osd.i4_right = (((INT32)ui4_canvas_w)*pt_rect->i4_right)/((INT32)pt_display_inf->ui4_scrn_width);
    t_disp_osd.i4_top = (((INT32)ui4_canvas_h)*pt_rect->i4_top)/((INT32)pt_display_inf->ui4_scrn_height);
    t_disp_osd.i4_bottom = (((INT32)ui4_canvas_h)*pt_rect->i4_bottom)/((INT32)pt_display_inf->ui4_scrn_height);

    i4_width = SBTL_RECT_W(&t_disp_osd); 
    i4_height = SBTL_RECT_H(&t_disp_osd);
    
    if((pt_display_inf->ui1_l_mgn_percent <= 100) && 
       (pt_display_inf->ui1_r_mgn_percent <= 100) && 
       (pt_display_inf->ui1_b_mgn_percent <= 100) && 
       (pt_display_inf->ui1_h_percent <= 100))
    {
        i4_tmp = (i4_width * pt_display_inf->ui1_l_mgn_percent) / 100;
        pt_subtitle_rect->i4_left = t_disp_osd.i4_left + i4_tmp;

        i4_tmp = (i4_width * pt_display_inf->ui1_r_mgn_percent) / 100;
        pt_subtitle_rect->i4_right = t_disp_osd.i4_right - i4_tmp;

        i4_tmp = (i4_height * pt_display_inf->ui1_b_mgn_percent) / 100;
        pt_subtitle_rect->i4_bottom = t_disp_osd.i4_bottom - i4_tmp;

        i4_tmp = (i4_height * pt_display_inf->ui1_h_percent) / 100;
        pt_subtitle_rect->i4_top = (i4_tmp == 0) ? 0 : (pt_subtitle_rect->i4_bottom - i4_tmp);
    }

    if(SBTL_RECT_EMPTY(pt_subtitle_rect) == TRUE)
    {
        return MMSBTLR_INV_ARG;
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    INT32                                   i4_ret;
    GL_RECT_T                               t_sbtl_rc_scrn;
    UINT32                                  ui4_canvas_w;
    UINT32                                  ui4_canvas_h = 560;
    HANDLE_T                                h_canvas;
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SBTL_MKV_TEXT_FNC_TBL_T*                pt_fnc_table;
    SBTL_DISPLAY_INFO_CMD_T*                pt_disp;
    UINT16                                  ui2_height_max = 0;  
    UINT16                                  ui2_height_min = 0;    
    UINT8                                   ui1_font_min = 0;
    UINT8                                   ui1_font_max = 0;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_fnc_table = pt_private->pt_fnc_table;
        pt_disp = &pt_mngr->pt_engine->t_cfg.t_display_info;

    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_start, ("pt_fnc_table->pf_start=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_start))
    {
        return MMSBTLR_INV_ARG;
    }

    if (pt_private->h_sbtl_wgl)
        return MMSBTLR_EXIST;
    
    /* Get canvas */
    h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
    
    i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    _sbtl_mkv_get_subtitle_info(pt_mngr->pt_engine,ui4_canvas_w,ui4_canvas_h,&t_sbtl_rc_scrn);
    
    i4_ret = sbtl_wgl_create_widget(h_canvas,
                                    SBTL_WGL_WIDGET_TEXT,
                                    SBTL_WGL_BORDER_NULL,
                                    &t_sbtl_rc_scrn,
                                    255,
                                    (VOID*) (SBTL_WGL_STL_TEXT_MAX_DIS_10_LINE
                                            |SBTL_WGL_STL_TEXT_MULTILINE 
                                            |SBTL_WGL_STL_GL_NO_IMG_UI
                                            |SBTL_WGL_STL_TEXT_MAX_128),
                                    NULL,
                                    &(pt_private->h_sbtl_wgl));
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_FONT,
                           (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_info,
                           NULL);

    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    if(pt_disp->b_display_info == TRUE)
    {
        ui2_height_max = (UINT16)(ui4_canvas_h/18);
        ui2_height_min =(UINT16)(ui4_canvas_h/18);
#if 0        
        if(pt_disp->ui1_font_min_grade > 0)
        {
            ui2_height_max = ((UINT32)SBTL_RECT_H(&pt_disp->t_rect_refer) / pt_disp->ui1_font_min_grade);
            if(ui2_height_max > ui4_canvas_h/18 || ui2_height_max < ui4_canvas_h/26)
            {
                ui2_height_max = ui4_canvas_h/18;
            }
        }

        if(pt_disp->ui1_font_max_grade > 0)
        {
            ui2_height_min = ((UINT16)SBTL_RECT_H(&pt_disp->t_rect_refer) / pt_disp->ui1_font_max_grade);
            if(ui2_height_min > ui4_canvas_h/18 || ui2_height_min < ui4_canvas_h/26)
            {
                ui2_height_min = ui4_canvas_h/pt_disp->ui1_font_min_grade;
            }
        }
#endif

        if((ui2_height_min <= ui2_height_max) && (ui2_height_min != 0) && (ui2_height_max != 0))
        {
            i4_ret = sbtl_wgl_do_cmd ( pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_TEXT_GET_FONT_SIZE,
                                       (VOID*)&ui2_height_max,
                                       &ui1_font_max);
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }
            i4_ret = sbtl_wgl_do_cmd ( pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_TEXT_GET_FONT_SIZE,
                                       (VOID*)&ui2_height_min,
                                       &ui1_font_min);
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }
            pt_mngr->pt_engine->t_cfg.t_font_info.e_font_size = FE_FNT_SIZE_CUSTOM;
            pt_mngr->pt_engine->t_cfg.t_font_info.ui1_custom_size = (UINT8)((ui1_font_min + ui1_font_max)/2);
            i4_ret = sbtl_wgl_do_cmd ( pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_GL_SET_FONT,
                                       (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_info,
                                       NULL);

            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }
        }

    }
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_TEXT_SET_ALIGN,
                           (VOID*)((UINT32)pt_mngr->pt_engine->t_cfg.ui1_text_alignment),
                           NULL);
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_COLOR,
                           (VOID*)SBTL_WGL_CLR_BK, /* background color */
                           (VOID*)&pt_mngr->pt_engine->t_cfg.t_bg_color_info);
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_COLOR,
                           (VOID*)SBTL_WGL_CLR_TEXT, /* text color */
                           (VOID*)&pt_mngr->pt_engine->t_cfg.t_fg_color_info);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);

    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    SBTL_SET_BASE_FONT_STYLE(pt_mngr->pt_engine,pt_mngr->pt_engine->t_cfg.t_font_info.e_font_style);

    pt_private->ui8_pos = 0;
    
    pt_private->ui4_seek_offset = 0;
    
    pt_fnc_table->pf_start(pt_mngr);

    _sbtl_mkv_text_timer_start(pt_mngr);

    pt_mngr->b_started = TRUE;
    
    _sbtl_mkv_text_new_page(pt_mngr);
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SBTL_MKV_TEXT_FNC_TBL_T*                pt_fnc_table;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;

    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_stop, ("pt_fnc_table->pf_stop=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_stop))
    {
        return MMSBTLR_INV_ARG;
    }
    
    _sbtl_mkv_text_timer_stop(pt_mngr);

    pt_fnc_table->pf_stop(pt_mngr);

    _sbtl_mkv_text_remove(pt_mngr);

    /* The widget had already been closed */
    if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }

    pt_mngr->b_started = FALSE;
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_need_dmx(SBTL_PAGE_INST_MNGR_T*      pt_mngr, 
                                     BOOL*                       pb_need)
{
    *pb_need = TRUE;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_handle_timing(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                          PTS_T                      t_pts)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    INT32                               i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->b_page_exist)
    {
        if (t_pts < pt_private->t_pts_s)
        {
            pt_private->e_state = MKV_TEXT_STATE_DATA_RDY;
            return MMSBTLR_OK;
        }
        else if (t_pts >= pt_private->t_pts_s && t_pts <= pt_private->t_pts_e)
        {
            if (pt_private->e_state != MKV_TEXT_STATE_DISPLAY)
            {
                pt_private->e_state = MKV_TEXT_STATE_DISPLAY;
                /*display*/
                if(pt_mngr->pt_engine->t_cfg.b_hide == FALSE)
                {
                i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                          SBTL_WGL_CMD_TEXT_SET_TEXT,
                                          (VOID*)pt_private->w2s_text,
                                          (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));
                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("set text error!!\n"));
                }
                
                i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);
                
                if (MMSBTLR_OK != i4_ret)
                {
                   ASSERT(0, ("show text error!!\n"));
                }
            }
            }
            return MMSBTLR_OK;
        }
        else
        {
            _sbtl_mkv_text_remove(pt_mngr);
            _sbtl_mkv_text_new_page(pt_mngr);
        }
    }
    else
    {
        _sbtl_mkv_text_new_page(pt_mngr);
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                        VOID*                    pv_get_info,
                                   UINT32                   ui4_set_type)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                                i4_ret = MMSBTLR_OK;
    BOOL                                 b_flag;
    PTS_T                                t_pts_current;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_SHOW_HIDE:
        
        b_flag = *(BOOL*)pv_get_info;
        
        i4_ret = sbtl_stc_pts(pt_mngr->pt_engine,
                               &t_pts_current);

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("Get STC Error!!\n"));
            break;
        }
                
        if(b_flag == TRUE)
        {
            pt_mngr->pt_engine->t_cfg.b_hide = FALSE;         
        }
        else
        {
            pt_mngr->pt_engine->t_cfg.b_hide = TRUE;
            if (pt_private->e_state == MKV_TEXT_STATE_DISPLAY)
            {        
                i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
            
                if (MMSBTLR_OK != i4_ret)
                {
                    return (MMSBTLR_WGL_FAIL);
                }            
            }               
        }
        
        break;

    default:
        break;
    }

   
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mkv_text_remove(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                                i4_ret;
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_private->b_page_exist)
    {
        return;
    }
    
    if (pt_private->e_state == MKV_TEXT_STATE_DISPLAY)
    {
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
            
        if (MMSBTLR_OK != i4_ret)
        {
            ASSERT(0, ("clear widget fail!!/n"));
        }
    }

    pt_private->e_state = MKV_TEXT_STATE_NONE;
    pt_private->b_page_exist = FALSE;
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mkv_text_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    BOOL                                b_found = FALSE;
    PTS_T                               t_pts_current = 0;
    INT32                               i4_ret;
    PTS_T                               t_pts_start;
    PTS_T                               t_pts_end;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    ASSERT(pt_mngr->pt_engine, ("pt_mngr->pt_engine=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private) || (NULL==pt_mngr->pt_engine))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }

    if (pt_private->b_page_exist)
    {
        return MMSBTLR_OK;
    }
    i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

    if (i4_ret != MMSBTLR_OK)
    {
        ASSERT(0, ("Get STC Error!!\n"));
    }

    _sbtl_mkv_text_seek_time_stamp(pt_mngr,
                                   t_pts_current,
                                   &t_pts_start,
                                   &t_pts_end,
                                   &b_found);

    if (b_found)
    {
        pt_private->t_pts_s = t_pts_start;
        pt_private->t_pts_e = t_pts_end;

        /*Clear the tmp font style information.*/
        SBTL_SET_FONT_STYLE_FLAG(pt_mngr->pt_engine, FALSE);
        SBTL_SET_FONT_STYLE(pt_mngr->pt_engine, 0);

        if (t_pts_current < t_pts_start)
        {
            /*load str string*/
            i4_ret = _sbtl_mkv_text_load_string(pt_mngr);
            if (i4_ret == MMSBTLR_OK)
            {
                if (x_uc_w2s_strlen (pt_private->w2s_text))
                {
                    pt_private->e_state = MKV_TEXT_STATE_DATA_RDY;
                    pt_private->b_page_exist = TRUE;
                }
            }
        }
        else if (t_pts_current >= t_pts_start && t_pts_current <= t_pts_end)
        {
            /*display*/
            if (MKV_TEXT_STATE_DATA_RDY == pt_private->e_state)
            {
            }
            else
            {
                i4_ret = _sbtl_mkv_text_load_string(pt_mngr);
                if (i4_ret != MMSBTLR_OK)
                {
                    return MMSBTLR_OK;
                }
            }

            if (0 == x_uc_w2s_strlen (pt_private->w2s_text))
            {
                pt_private->e_state = MKV_TEXT_STATE_NONE;
                pt_private->b_page_exist = FALSE;
            }

            pt_private->e_state = MKV_TEXT_STATE_DISPLAY;
            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_TEXT_SET_TEXT,
                                      (VOID*)pt_private->w2s_text,
                                      (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));

            if (i4_ret != MMSBTLR_OK)
            {
                ASSERT(0, ("set text error!!\n"));
            }
            
            if(pt_mngr->pt_engine->t_cfg.b_hide == FALSE)
            {
            i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);
            
            if (MMSBTLR_OK != i4_ret)
            {
               ASSERT(0, ("show text error!!\n"));
            }
            }

            pt_private->b_page_exist = TRUE;
        }
        else
        {
            pt_private->e_state = MKV_TEXT_STATE_NONE;
            return MMSBTLR_OK;
        }
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static SBTL_PAGE_INST_MNGR_T* _text_page_mngr_constructor (SBTL_ENGINE_T*    pt_engine)
{
    SBTL_PAGE_INST_MNGR_T*                  pt_mngr;
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    UINT8                                   ui1_idx;
    BOOL                                    b_found;
    INT32                                   i4_ret;

    ASSERT(pt_engine, ("_text_page_mngr_constructor pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return NULL;
    }

    do
    {
        pt_mngr = (SBTL_PAGE_INST_MNGR_T*)x_mem_alloc(sizeof(SBTL_PAGE_INST_MNGR_T));
        if (NULL == pt_mngr)
        {
            break;
        }
        x_memset(pt_mngr, 0, sizeof(SBTL_PAGE_INST_MNGR_T));

        pt_mngr->pf_start = _sbtl_mkv_text_start;
        pt_mngr->pf_stop = _sbtl_mkv_text_stop;
        pt_mngr->pf_cfg = _sbtl_mkv_text_config;
        pt_mngr->pf_handle_timing = _sbtl_mkv_text_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_mkv_text_need_dmx;
        pt_mngr->pf_new_page = _sbtl_mkv_text_new_page;
        
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }
        x_memset(pt_mngr->pv_private, 0, sizeof(SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T));

        pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        
        b_found = FALSE;
        for(ui1_idx=0; ; ui1_idx++)
        {
            if (ui1_idx >= (UINT8)(sizeof(at_fnc_tbl_list)/sizeof(at_fnc_tbl_list[0])))
            {
                break;
            }
            
            if (MM_SBTL_TYPE_END == at_fnc_tbl_list[ui1_idx].e_sbtl_text_type)
            {
                break;
            }

            if(at_fnc_tbl_list[ui1_idx].e_sbtl_text_type == pt_engine->t_cfg.e_sbtl_type)
            {
                pt_private->pt_fnc_table = &at_fnc_tbl_list[ui1_idx];
                b_found = TRUE;
            }
        }
        
        if (!b_found)
        {
            break;
        }
        pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_ASCI;
        pt_private->ui8_pos = 0;
        
        i4_ret = pt_private->pt_fnc_table->pf_init(pt_mngr);
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        return pt_mngr;
    }while(0);

    if (pt_mngr)
    {
        if (pt_mngr->pv_private)
        {
            x_mem_free(pt_mngr->pv_private);
        }
        x_mem_free(pt_mngr);
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _text_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_text_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_text_page_mngr_destructor pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return;
    }

    if (pt_private->pt_fnc_table)
    {
        if (pt_private->pt_fnc_table->pf_deinit)
        {
            pt_private->pt_fnc_table->pf_deinit(pt_mngr);
        }
    }
    
    x_mem_free(pt_private);
    pt_mngr->pt_engine->pt_page_mngr = NULL;
    x_mem_free(pt_mngr);
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_sbtl_mkv_text_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_MKV_SRT,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_MKV_SSA,
                  &t_text_fnct_tbl);
}

