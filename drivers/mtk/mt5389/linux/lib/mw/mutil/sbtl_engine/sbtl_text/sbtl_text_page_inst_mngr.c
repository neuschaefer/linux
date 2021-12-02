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
 * $RCSfile: sbtl_text_page_inst_mngr.c,v $
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
#include "mutil/sbtl_engine/sbtl_text/sbtl_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_srt_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_sami_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_subviewer_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_mplayer2_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_ssa_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_mdvdsub_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_txt_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_powerdivx_parser.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_dvdss_parser.h"
#include "mutil/mm_util.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "x_uc_str.h"
#include "../../mm_util.h"


#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"
#include "../sbtl_widget.h"

#include "sbtl_text_page_inst_mngr.h"
#include "sbtl_srt_parser.h"
#include "sbtl_sami_parser.h"
#include "sbtl_subviewer_parser.h"
#include "sbtl_mplayer2_parser.h"
#include "sbtl_ssa_parser.h"
#include "sbtl_mdvdsub_parser.h"
#include "sbtl_txt_parser.h"
#include "sbtl_powerdivx_parser.h"
#include "sbtl_dvdss_parser.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 #define PTS_FREQ        ((UINT64)(90000))
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_text_timer_period(SBTL_PAGE_INST_MNGR_T*     pt_mngr);
static INT32 _sbtl_text_timer_start(SBTL_PAGE_INST_MNGR_T*       pt_mngr);
static VOID _sbtl_text_timer_stop(SBTL_PAGE_INST_MNGR_T*         pt_mngr);


static INT32 _sbtl_text_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts_current,
                                        PTS_T*                  pt_pts_start,
                                        PTS_T*                  pt_pts_end,
                                        BOOL*                   pb_found);

static INT32 _sbtl_text_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

static INT32 _sbtl_text_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_text_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

static INT32 _sbtl_text_need_dmx(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                 BOOL*                      pb_need);

static INT32 _sbtl_text_handle_timing(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                      PTS_T                     t_pts);

static INT32 _sbtl_text_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  VOID*                    pv_cfg_info,
                               UINT32                   ui4_set_type);

static VOID _sbtl_text_remove(SBTL_PAGE_INST_MNGR_T*        pt_mngr);
static INT32 _sbtl_text_new_page(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

static INT32 _sbtl_text_adjust_pts(PTS_T* pt_pts, SBTL_PAGE_INST_MNGR_T* pt_mngr, BOOL* pb_negative);

static SBTL_PAGE_INST_MNGR_T* _text_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine);
static VOID _text_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static BOOL _text_page_mngr_parser_verify(SBTL_ENGINE_T*    pt_engine,
                                          UINT8*            pt_type);
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static SBTL_MNGR_FCT_TBL_T t_text_fnct_tbl = 
{
    TRUE,
    _text_page_mngr_constructor,
    _text_page_mngr_destructor,
    _text_page_mngr_parser_verify
};

static SBTL_TEXT_FNC_TBL_T  at_fnc_tbl_list[] = 
{
    {
        MM_SBTL_TYPE_TEXT_SRT,
        srt_seek_time_stamp,
        srt_load_string,
        srt_timer_period,
        srt_init,
        srt_deinit,
        srt_start,
        srt_stop,
        srt_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_SAMI,
        sami_seek_time_stamp,
        sami_load_string,
        sami_timer_period,
        sami_init,
        sami_deinit,
        sami_start,
        sami_stop,
        sami_parser_verify,
        sami_config,
        sami_get_info
    },
    {
        MM_SBTL_TYPE_TEXT_SVSUB,
        svsub_seek_time_stamp,
        svsub_load_string,
        svsub_timer_period,
        svsub_init,
        svsub_deinit,
        svsub_start,
        svsub_stop,
        svsub_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_MPLAYER2,
        mplayer2_seek_time_stamp,
        mplayer2_load_string,
        mplayer2_timer_period,
        mplayer2_init,
        mplayer2_deinit,
        mplayer2_start,
        mplayer2_stop,
        mplayer2_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_SSA,
        ssa_seek_time_stamp,
        ssa_load_string,
        ssa_timer_period,
        ssa_init,
        ssa_deinit,
        ssa_start,
        ssa_stop,
        ssa_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_MDVDSUB,
        mdvdsub_seek_time_stamp,
        mdvdsub_load_string,
        mdvdsub_timer_period,
        mdvdsub_init,
        mdvdsub_deinit,
        mdvdsub_start,
        mdvdsub_stop,
        mdvdsub_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_TXT,
        txt_seek_time_stamp,
        txt_load_string,
        txt_timer_period,
        txt_init,
        txt_deinit,
        txt_start,
        txt_stop,
        txt_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_POWERDIVX,
        psb_seek_time_stamp,
        psb_load_string,
        psb_timer_period,
        psb_init,
        psb_deinit,
        psb_start,
        psb_stop,
        psb_parser_verify,
        NULL,
        NULL
    },
    {
        MM_SBTL_TYPE_TEXT_DVDSS,
        dvdss_seek_time_stamp,
        dvdss_load_string,
        dvdss_timer_period,
        dvdss_init,
        dvdss_deinit,
        dvdss_start,
        dvdss_stop,
        dvdss_parser_verify,
        NULL,
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
        NULL,
        NULL,
        NULL
    }
};


/*-----------------------------------------------------------------------------
                    Internal functions implementations
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
static INT32 _sbtl_text_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts_current,
                                        PTS_T*                  pt_pts_start,
                                        PTS_T*                  pt_pts_end,
                                        BOOL*                   pb_found)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;
    SBTL_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
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
static INT32 _sbtl_text_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;
    SBTL_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_load_string, ("pt_fnc_table->pf_load_string=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_load_string))
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_fnc_table->pf_load_string(pt_mngr);
    
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
static UINT32 _sbtl_text_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
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
    mm_sbtl_hdlr_send_timer_msg(t_msg, (VOID*)pt_mngr);
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
static INT32 _sbtl_text_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                            i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

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
                           _sbtl_text_timer_period(pt_mngr),
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
static VOID _sbtl_text_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }
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
static INT32 _sbtl_text_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    INT32                               i4_ret;
    GL_RECT_T                           t_sbtl_rc_scrn;
    UINT32                              ui4_canvas_w = 0;
    UINT32                              ui4_canvas_h = 0;
    HANDLE_T                            h_canvas;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SBTL_TEXT_FNC_TBL_T*                pt_fnc_table;
    GL_POINT_T                          t_osd_ofst;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_fnc_table = pt_private->pt_fnc_table;

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
    
    if (pt_mngr->pt_engine->t_cfg.b_rect_osd)
    {
        t_sbtl_rc_scrn = pt_mngr->pt_engine->t_cfg.t_rect_osd;
    }
    else
    {
        i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
        
        t_sbtl_rc_scrn.i4_left = (INT32)(ui4_canvas_w/6);
        t_sbtl_rc_scrn.i4_top = (INT32)(ui4_canvas_h/10);
        t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w-(INT32)(ui4_canvas_w/6);
        t_sbtl_rc_scrn.i4_bottom = (INT32)(ui4_canvas_h)-(INT32)(ui4_canvas_h/10);
    }    
    
    t_osd_ofst = pt_mngr->pt_engine->t_cfg.t_pos_adjust;
    
    if(t_osd_ofst.i4_x !=0 || t_osd_ofst.i4_y !=0)
    {            
        t_sbtl_rc_scrn.i4_left += t_osd_ofst.i4_x;
        t_sbtl_rc_scrn.i4_top += t_osd_ofst.i4_y;
        t_sbtl_rc_scrn.i4_right += t_osd_ofst.i4_x;
        t_sbtl_rc_scrn.i4_bottom += t_osd_ofst.i4_y;

        if(t_sbtl_rc_scrn.i4_left < 0)
        {
            t_sbtl_rc_scrn.i4_right -= t_sbtl_rc_scrn.i4_left;
            t_sbtl_rc_scrn.i4_left = 0;
        }

        if(t_sbtl_rc_scrn.i4_top < 0)
        {
            t_sbtl_rc_scrn.i4_bottom -= t_sbtl_rc_scrn.i4_top;
            t_sbtl_rc_scrn.i4_top = 0;
        }
        
        if(t_sbtl_rc_scrn.i4_right > (INT32)ui4_canvas_w)
        {
            t_sbtl_rc_scrn.i4_left -= (t_sbtl_rc_scrn.i4_right - (INT32)ui4_canvas_w);
            t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w;
        }

        if(t_sbtl_rc_scrn.i4_bottom > (INT32)ui4_canvas_h)
        {
            t_sbtl_rc_scrn.i4_top -= (t_sbtl_rc_scrn.i4_bottom - (INT32)ui4_canvas_h);
            t_sbtl_rc_scrn.i4_bottom = (INT32)ui4_canvas_h;
        }
    }
    i4_ret = sbtl_wgl_create_widget(h_canvas,
                                    SBTL_WGL_WIDGET_TEXT,
                                    SBTL_WGL_BORDER_NULL,
                                    &t_sbtl_rc_scrn,
                                    255,
                                    (VOID*) (SBTL_WGL_STL_TEXT_MAX_DIS_40_LINE
                                            |SBTL_WGL_STL_TEXT_SMART_CUT
                                            |SBTL_WGL_STL_TEXT_SMART_CUT_OVER_MAX
                                            |SBTL_WGL_STL_TEXT_MULTILINE 
                                            |SBTL_WGL_STL_GL_NO_IMG_UI
                                            |SBTL_WGL_STL_TEXT_MAX_2048),
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

i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_COLOR,
                           (VOID*)SBTL_WGL_CLR_TEXT_BG, /* text color */
                           (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_bg_color);    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    if(pt_mngr->pt_engine->t_cfg.ui4_cmd_mask | MM_SBTL_CMD_MSK_EDGE_COLOR)
    {
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_COLOR,
                                       (VOID*)SBTL_WGL_CLR_EDGE, /* font edge color */
                                       (VOID*)&(pt_mngr->pt_engine->t_cfg.t_edge_color_info));
                
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }
    }

    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);

    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    SBTL_SET_BASE_FONT_STYLE(pt_mngr->pt_engine,pt_mngr->pt_engine->t_cfg.t_font_info.e_font_style);

    sbtl_feeder_init(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE);
    
    pt_private->ui4_seek_offset = 0;
    //pt_private->ui2_time_max_cnt = 0;
    pt_private->ui2_cur_idx = SUBTITLE_IDX_NULL;
    
    pt_fnc_table->pf_start(pt_mngr);

    _sbtl_text_timer_start(pt_mngr);

    pt_mngr->b_started = TRUE;
    
    //_sbtl_text_new_page(pt_mngr);
    
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
static INT32 _sbtl_text_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SBTL_TEXT_FNC_TBL_T*                pt_fnc_table;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;

    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_stop, ("pt_fnc_table->pf_stop=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_stop))
    {
        return MMSBTLR_INV_ARG;
    }
    
    _sbtl_text_timer_stop(pt_mngr);

    pt_fnc_table->pf_stop(pt_mngr);
    
    _sbtl_text_remove(pt_mngr);
    /* The widget had already been closed */
    if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }

    pt_mngr->b_started = FALSE;
    pt_private->e_state = TEXT_STATE_NONE; 
    pt_private->b_page_exist = FALSE;    
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
static INT32 _sbtl_text_need_dmx(SBTL_PAGE_INST_MNGR_T*      pt_mngr, 
                                 BOOL*                       pb_need)
{
    *pb_need = FALSE;
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
static INT32 _sbtl_text_handle_timing(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                      PTS_T                      t_pts)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    INT32                               i4_ret;
    BOOL                                b_negative;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    _sbtl_text_adjust_pts(&t_pts,pt_mngr,&b_negative);

    if (pt_private->b_page_exist)
    {
        if (t_pts < pt_private->t_pts_s)
        {
            pt_private->e_state = TEXT_STATE_DATA_RDY;
            return MMSBTLR_OK;
        }
        else if (t_pts >= pt_private->t_pts_s && t_pts <= pt_private->t_pts_e)
        {
            if (pt_private->e_state != TEXT_STATE_DISPLAY)
            {
                pt_private->e_state = TEXT_STATE_DISPLAY;
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
            _sbtl_text_remove(pt_mngr);
            _sbtl_text_new_page(pt_mngr);
        }
    }
    else
    {
        _sbtl_text_new_page(pt_mngr);
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
static INT32 _sbtl_text_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  VOID*                    pv_cfg_info,
                               UINT32                   ui4_set_type)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_TEXT_FNC_TBL_T*            pt_fnc_table;
    INT32                           i4_ret = MMSBTLR_OK;
    MM_SBTL_ENCODING_T              e_encoding;
    WGL_FONT_INFO_T*                pt_font_info;
    UINT8                           ui1_tmp;
    BOOL                            b_flag;
    PTS_T                           t_pts_current;
    MM_SBTL_TM_OFST_INF*            pt_time_ofst;
    INT64                           i8_adjust;
    GL_RECT_T                       t_sbtl_rc_scrn;
    UINT32                          ui4_canvas_w;
    UINT32                          ui4_canvas_h;
    HANDLE_T                        h_canvas;
    GL_POINT_T*                     pt_osd_ofst;
WGL_COLOR_INFO_T*  pt_color_info = (WGL_COLOR_INFO_T*)pv_cfg_info;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    pt_font_info = &pt_mngr->pt_engine->t_cfg.t_font_info;
    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_LANGUAGE:
        pt_private->e_language = *(MM_SBTL_LANG_T*)pv_cfg_info;
        break;
    
    case EVN_MM_SBTL_SET_OSD_FONT_INFO:
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_GL_SET_FONT,
                                  (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_info,
                                  NULL);
        if (MMSBTLR_OK != i4_ret)
    {
           return (MMSBTLR_WGL_FAIL);
        }            
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

            if (MMSBTLR_OK != i4_ret)
            {
               ASSERT(0, ("show text error!!\n"));
        }            

        sbtl_set_font_inf_record(pt_mngr->pt_engine,&pt_mngr->pt_engine->t_cfg.t_font_info);
        break;

    case EVN_MM_SBTL_SET_ENCODING:
        e_encoding = *((MM_SBTL_ENCODING_T*)pv_cfg_info);
        
        sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);        
        if(e_encoding == MM_SBTL_ENCODE_AUTO)
        {
            x_strcpy(pt_font_info->a_c_font_name, SN_FONT_DEFAULT);
            pt_font_info->e_font_size  = FE_FNT_SIZE_MEDIUM;
            pt_font_info->e_font_style = FE_FNT_STYLE_REGULAR;
            pt_font_info->e_font_cmap  = FE_CMAP_ENC_UNICODE;
            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_GL_SET_FONT,
                                      (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_info,
                                      NULL);
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }             
            
            pt_mngr->pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_ASCI;
            pt_private->b_UTF16 = FALSE;  
            
            sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            if (0xFF == ui1_tmp)
            {
                sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                if (0xFE == ui1_tmp)
                {
                    pt_private->b_UTF16 = TRUE;
                    pt_mngr->pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF16;
                }
            }else if (0xEF == ui1_tmp)
            {
                sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                if (0xBB == ui1_tmp)
                {
                    sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                    if(0xBF == ui1_tmp)
                    {
                        pt_mngr->pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF8;
                    }
                 }
            }

            sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);        
            
        }
        else if(e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            pt_private->b_UTF16 = TRUE;
        }
        else
        {
            pt_private->b_UTF16 = FALSE;
    }

        sbtl_set_encode_record(pt_mngr->pt_engine,e_encoding);
        pt_private->b_page_exist = FALSE;
        pt_private->t_pts_s = 0;
        pt_private->t_pts_e = 0;
        break;
               
    case EVN_MM_SBTL_SET_SHOW_HIDE:
        
        b_flag = *(BOOL*)pv_cfg_info;
        
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
            if (pt_private->e_state == TEXT_STATE_DISPLAY)
            {        
                i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
            
                if (MMSBTLR_OK != i4_ret)
                {
                    return (MMSBTLR_WGL_FAIL);
                }            
            }               
        }
        
        break;
case EVN_MM_SBTL_SET_ADJUST_TIME:
        pt_time_ofst = (MM_SBTL_TM_OFST_INF*)pv_cfg_info;

        i8_adjust = pt_time_ofst->i8_time_ofst;

        
        if(i8_adjust <= 0)
        {
            sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        _sbtl_text_new_page(pt_mngr);
        //sbtl_set_time_ofst_record(pt_mngr->pt_engine , pt_time_ofst);
        break;

    case EVN_MM_SBTL_SET_OSD_FG_COLOR:
        if(pt_color_info->e_type != WGL_COLOR_SET_STANDARD)
        {
            return (MMSBTLR_WGL_FAIL);
        }
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_GL_SET_COLOR,
                                  (VOID*)SBTL_WGL_CLR_TEXT, 
                                  (VOID*)(pt_color_info));
            
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        } 
            
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

        if (MMSBTLR_OK != i4_ret)
        {
           ASSERT(0, ("show text error!!\n"));
        } 

    break;
case EVN_MM_SBTL_SET_OSD_FONT_EDGE_COLOR:

        if(pt_color_info->e_type != WGL_COLOR_SET_STANDARD)
        {
            return (MMSBTLR_WGL_FAIL);
        }
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                   SBTL_WGL_CMD_GL_SET_COLOR,
                                   (VOID*)SBTL_WGL_CLR_EDGE, 
                                   (VOID*)&(pt_mngr->pt_engine->t_cfg.t_edge_color_info));
            
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        } 
            
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

        if (MMSBTLR_OK != i4_ret)
        {
           ASSERT(0, ("show text error!!\n"));
        } 

        //sbtl_set_edge_color_record(pt_mngr->pt_engine, (WGL_COLOR_INFO_T*)pv_cfg_info);
        break;


    case EVN_MM_SBTL_SET_OSD_OFFSET:
        pt_osd_ofst = (GL_POINT_T*)pv_cfg_info;
        h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;

        i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);

        if (MMSBTLR_OK != i4_ret)
        {
            return (MMSBTLR_WGL_FAIL);
        } 
    
        if (pt_mngr->pt_engine->t_cfg.b_rect_osd)
        {
            t_sbtl_rc_scrn = pt_mngr->pt_engine->t_cfg.t_rect_osd;
        }
        else
        {                   
            t_sbtl_rc_scrn.i4_left = (INT32)(ui4_canvas_w/6);
            t_sbtl_rc_scrn.i4_top = (INT32)(ui4_canvas_h/10);
            t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w-(INT32)(ui4_canvas_w/6);
            t_sbtl_rc_scrn.i4_bottom = (INT32)(ui4_canvas_h)-(INT32)(ui4_canvas_h/10);
        }
        
        t_sbtl_rc_scrn.i4_left += pt_osd_ofst->i4_x;
        t_sbtl_rc_scrn.i4_top += pt_osd_ofst->i4_y;
        t_sbtl_rc_scrn.i4_right += pt_osd_ofst->i4_x;
        t_sbtl_rc_scrn.i4_bottom += pt_osd_ofst->i4_y;

        if(t_sbtl_rc_scrn.i4_left < 0)
        {
            t_sbtl_rc_scrn.i4_right -= t_sbtl_rc_scrn.i4_left;
            t_sbtl_rc_scrn.i4_left = 0;
        }

        if(t_sbtl_rc_scrn.i4_top < 0)
        {
            t_sbtl_rc_scrn.i4_bottom -= t_sbtl_rc_scrn.i4_top;
            t_sbtl_rc_scrn.i4_top = 0;
        }
        
        if(t_sbtl_rc_scrn.i4_right > (INT32)ui4_canvas_w)
        {
            t_sbtl_rc_scrn.i4_left -= (t_sbtl_rc_scrn.i4_right - (INT32)ui4_canvas_w);
            t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w;
        }

        if(t_sbtl_rc_scrn.i4_bottom > (INT32)ui4_canvas_h)
        {
            t_sbtl_rc_scrn.i4_top -= (t_sbtl_rc_scrn.i4_bottom - (INT32)ui4_canvas_h);
            t_sbtl_rc_scrn.i4_bottom = (INT32)ui4_canvas_h;
        }
        i4_ret = sbtl_wgl_move(pt_private->h_sbtl_wgl,&t_sbtl_rc_scrn,SBTL_WGL_SYNC_AUTO_REPAINT);
        
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        } 
         
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }

        //sbtl_set_osd_ofst_record(pt_mngr->pt_engine, &pt_mngr->pt_engine->t_cfg.t_pos_adjust);
        break;

    case EVN_MM_SBTL_SET_OSD_RECT:
    
        break;
               
    default:
        break;
    }

    if (NULL==pt_fnc_table)
    {
        return MMSBTLR_OK;
    }

    if (pt_fnc_table->pf_config)
    {
        pt_fnc_table->pf_config(pt_mngr, ui4_set_type);
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
static INT32 _sbtl_text_get(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                            UINT32                   ui4_get_type,
                            VOID*                    pv_get_info,
                            SIZE_T                   z_get_info_len)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_TEXT_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    if (NULL==pt_fnc_table)
    {
        return MMSBTLR_OK;
    }

    if (pt_fnc_table->pf_get_info)
    {
        pt_fnc_table->pf_get_info(pt_mngr, ui4_get_type, pv_get_info, z_get_info_len);
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
static VOID _sbtl_text_remove(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                           i4_ret;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_private->b_page_exist)
    {
        return;
    }
    
    if (pt_private->e_state == TEXT_STATE_DISPLAY)
    {                
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
   
        if (MMSBTLR_OK != i4_ret)
        {
            ASSERT(0, ("clear widget fail!!/n"));
        }
    }

    pt_private->e_state = TEXT_STATE_NONE;
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
static INT32 _sbtl_text_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    BOOL                            b_found = FALSE;
    PTS_T                           t_pts_current;
    INT32                           i4_ret;
    PTS_T                           t_pts_start;
    PTS_T                           t_pts_end;
    STC_CTRL_TYPE_T                 t_stc_type;
    FE_FNT_STYLE                        e_new_style;
    WGL_FONT_INFO_T                     t_font_inf;
BOOL                            b_negative=FALSE;
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    ASSERT(pt_mngr->pt_engine, ("pt_mngr->pt_engine=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private) || (NULL==pt_mngr->pt_engine))
    {
        return MMSBTLR_INV_ARG;
    }

    t_stc_type = STC_CTRL_START;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }

    if (pt_private->b_page_exist)
    {
        return MMSBTLR_OK;
    }

    i4_ret = sbtl_stc_type(pt_mngr->pt_engine, &t_stc_type);
    DBG_INFO(("[MM SBTL]t_stc_type:%d \n",t_stc_type));
    /*if((t_stc_type == STC_CTRL_START) && (i4_ret == MMSBTLR_OK))*/
    {
        i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("Get STC Error!!\n"));
        }

        _sbtl_text_adjust_pts(&t_pts_current,pt_mngr,&b_negative);

        _sbtl_text_seek_time_stamp(pt_mngr,
                                   t_pts_current,
                                   &t_pts_start,
                                   &t_pts_end,
                                   &b_found);
    }

    if (b_found)
    {
        pt_private->t_pts_s = t_pts_start;
        pt_private->t_pts_e = t_pts_end;

        /*Clear the tmp font style information.*/
        SBTL_SET_FONT_STYLE_FLAG(pt_mngr->pt_engine, FALSE);
        SBTL_SET_FONT_STYLE(pt_mngr->pt_engine, 0);

        if (t_pts_current < t_pts_start)
        {
            _sbtl_text_remove(pt_mngr);
            /*load str string*/
            i4_ret = _sbtl_text_load_string(pt_mngr);
            if (i4_ret == MMSBTLR_OK)
            {
                if (x_uc_w2s_strlen (pt_private->w2s_text))
                {
                    pt_private->e_state = TEXT_STATE_DATA_RDY;
                    pt_private->b_page_exist = TRUE;
                }
            }
        }
        else if (t_pts_current >= t_pts_start && t_pts_current <= t_pts_end)
        {
            /*display*/
            if (TEXT_STATE_DATA_RDY == pt_private->e_state)
            {
            }
            else
            {
                i4_ret = _sbtl_text_load_string(pt_mngr);
                if (i4_ret != MMSBTLR_OK)
                {
                    return MMSBTLR_OK;
                }
            }

            if (0 == x_uc_w2s_strlen (pt_private->w2s_text))
            {
                pt_private->e_state = TEXT_STATE_NONE;
                pt_private->b_page_exist = FALSE;
            }

            e_new_style = SBTL_GET_BASE_FONT_STYLE(pt_mngr->pt_engine);
            if( SBTL_GET_FONT_STYLE_FLAG(pt_mngr->pt_engine) == TRUE)           
            {
                e_new_style |= SBTL_GET_FONT_STYLE(pt_mngr->pt_engine);
            }

#ifndef __KERNEL__
            x_dbg_stmt("[MM SBTL] new font style:%x,prev:%x\n",e_new_style,SBTL_GET_PREV_FONT_STYLE(pt_mngr->pt_engine));
#else
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
            x_dbg_stmt("[MM SBTL]set new style:%x \n");
#else
#endif
                SBTL_SET_PREV_FONT_STYLE(pt_mngr->pt_engine,e_new_style);
            }

            pt_private->e_state = TEXT_STATE_DISPLAY;
            
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
            pt_private->e_state = TEXT_STATE_NONE;
            return MMSBTLR_OK;
        }
    }

    return MMSBTLR_OK;
}

static INT32 _sbtl_text_adjust_pts(PTS_T* pt_pts, SBTL_PAGE_INST_MNGR_T* pt_mngr, BOOL* pb_negative)
{
    INT64                           i8_pts_tmp;
    UINT64                          ui8_tmp;
    INT64                           i8_adjust;
    
    if((pt_pts == NULL) || (pb_negative == NULL) || (pt_mngr == NULL))
    {
        return MMSBTLR_INV_ARG;
    }

    i8_pts_tmp = (INT64)(*pt_pts);
    i8_adjust = pt_mngr->pt_engine->t_cfg.t_time_ofst.i8_time_ofst;

    if(i8_adjust >= 0)
    {
        ui8_tmp = (UINT64)(i8_adjust);
        ui8_tmp     = (UINT64)_mm_div64(PTS_FREQ*ui8_tmp, 1000, NULL);
        i8_pts_tmp += ui8_tmp;
    }
    else
    {
        ui8_tmp = (UINT64)(0 - i8_adjust);
        ui8_tmp     = (UINT64)_mm_div64(PTS_FREQ*ui8_tmp, 1000, NULL);
        i8_pts_tmp -= ui8_tmp;
    }          

    if(i8_pts_tmp < 0)
    {
        *pt_pts = 0;
        *pb_negative = TRUE;
    }
    else
    {
        *pt_pts = (PTS_T)i8_pts_tmp;
        *pb_negative = FALSE;
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
    SBTL_PAGE_INST_MNGR_T*              pt_mngr;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    UINT8                               ui1_idx;
    INT32                               i4_ret;

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

        pt_mngr->pf_start = _sbtl_text_start;
        pt_mngr->pf_stop = _sbtl_text_stop;
        pt_mngr->pf_cfg = _sbtl_text_config;
        pt_mngr->pf_handle_timing = _sbtl_text_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_text_need_dmx;
        pt_mngr->pf_new_page = _sbtl_text_new_page;
        pt_mngr->pf_get = _sbtl_text_get;
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_TEXT_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }
        x_memset(pt_mngr->pv_private, 0, sizeof(SBTL_TEXT_PAGE_MNGR_PRIVATE_T));

        pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        
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

                sbtl_feeder_init(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE);

                {
                    UINT8 ui1_tmp;
                    
                    pt_private->b_UTF16 = FALSE;
                    pt_private->e_language = MM_SBTL_LANG_ASCI;
                    
                    sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                    
                    if (0xFF == ui1_tmp)
                    {
                        sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                        if (0xFE == ui1_tmp)
                        {
                            pt_private->b_UTF16 = TRUE;
                            pt_private->e_language = MM_SBTL_LANG_UNICODE;
                            pt_mngr->pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF16;
                        }
                    }else if (0xEF == ui1_tmp)
                    {
                        sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                        if (0xBB == ui1_tmp)
                        {
                            sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                            if(0xBF == ui1_tmp)
                            {
                                pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF8;
                            }
                        }
                    }

                    if(pt_engine->t_cfg.e_encoding == MM_SBTL_ENCODE_AUTO)
                    {
                        pt_engine->t_cfg.e_encoding = MM_SBTL_DFT_ENCODING;
                    }

                    if (pt_private->b_UTF16 == TRUE)
                    {
                        pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF16;
                    }
                    else if(pt_mngr->pt_engine->t_cfg.t_font_info.e_font_cmap == FE_CMAP_ENC_BIG5) 
                    {
                        pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_BIG5;
                        pt_mngr->pt_engine->t_cfg.t_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
                    }
                    else if(pt_mngr->pt_engine->t_cfg.t_font_info.e_font_cmap == FE_CMAP_ENC_GB2312) 
                    {
                        pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_GB2312;
                        pt_mngr->pt_engine->t_cfg.t_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
                    }
#if 0                    
                    else
                    {
                        pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_ASCI;
                    }
#endif                    
                    sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                }

                i4_ret = pt_private->pt_fnc_table->pf_init(pt_mngr);
                if (i4_ret != MMSBTLR_OK)
                {
                      break;
                }
        
                return pt_mngr;
            }
        }

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
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_text_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
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
    if(pt_private->pt_time_stamp != NULL)
    {
        x_mem_free(pt_private->pt_time_stamp);
        pt_private->pt_time_stamp = NULL;
    }

    if(pt_private->pui2_time_order != NULL)
    {
        x_mem_free(pt_private->pui2_time_order);
        pt_private->pui2_time_order = NULL;
    }
    x_mem_free(pt_private);
    pt_mngr->pt_engine->pt_page_mngr = NULL;
    x_mem_free(pt_mngr);
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
static BOOL _text_page_mngr_parser_verify(SBTL_ENGINE_T*    pt_engine,
                                          UINT8*            pt_type)

{
    UINT8   ui1_idx;

    ASSERT(pt_engine, ("_text_page_mngr_parser_verify pt_engine=NULL\n"));
    ASSERT(pt_type, ("_vobsub_page_mngr_verify pt_type=NULL\n"));
    if ((NULL==pt_engine) || (NULL==pt_type))
    {
        return FALSE;
    }

    
    sbtl_feeder_init(pt_engine, MM_SBTL_FEEDER_SUBTITLE);
    
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
        
        if (*pt_type == MM_SBTL_TYPE_TEXT)
        {
            if(at_fnc_tbl_list[ui1_idx].pf_verify(pt_engine))
            {
                *pt_type = at_fnc_tbl_list[ui1_idx].e_sbtl_text_type;
                return TRUE;
            }
        }
        else
        {
            if (*pt_type == at_fnc_tbl_list[ui1_idx].e_sbtl_text_type)
            {
                if(at_fnc_tbl_list[ui1_idx].pf_verify(pt_engine))
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    
    return FALSE;
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
VOID x_sbtl_text_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_TEXT,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_SRT,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_SAMI,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_SVSUB,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_MPLAYER2,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_SSA,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_MDVDSUB,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_TXT,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_POWERDIVX,
                  &t_text_fnct_tbl);

    sbtl_register(MM_SBTL_TYPE_TEXT_DVDSS,
                  &t_text_fnct_tbl);
}

