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

#include "mutil/sbtl_engine/sbtl_lyric/sbtl_lyric_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_lyric/sbtl_lrc_parser.h"

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

#include "sbtl_lyric_page_inst_mngr.h"
#include "sbtl_lrc_parser.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_lyric_timer_period(SBTL_PAGE_INST_MNGR_T*     pt_mngr);
static INT32 _sbtl_lyric_timer_start(SBTL_PAGE_INST_MNGR_T*       pt_mngr);
static VOID _sbtl_lyric_timer_stop(SBTL_PAGE_INST_MNGR_T*         pt_mngr);


static INT32 _sbtl_lyric_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        BOOL                    b_pts_neg,
                                        PTS_T                   t_pts_current,
                                        UINT16                  ui2_time_cnt,
                                        UINT16*                 pt_found_cnt);

static INT32 _sbtl_lyric_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                           UINT16                  ui2_load_idx);

static INT32 _sbtl_lyric_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_lyric_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

static INT32 _sbtl_lyric_need_dmx(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                 BOOL*                      pb_need);

static INT32 _sbtl_lyric_handle_timing(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                      PTS_T                     t_pts);

static INT32 _sbtl_lyric_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  VOID*                    pv_cfg_info,
                               UINT32                   ui4_set_type);

static VOID _sbtl_lyric_remove(SBTL_PAGE_INST_MNGR_T*        pt_mngr);
static INT32 _sbtl_lyric_new_page(SBTL_PAGE_INST_MNGR_T*     pt_mngr);


static SBTL_PAGE_INST_MNGR_T* _lyric_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine);
static VOID _lyric_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static BOOL _lyric_page_mngr_parser_verify(SBTL_ENGINE_T*    pt_engine,
                                          UINT8*            pt_type);

static INT32 _sbtl_lyric_adjust_pts(PTS_T* pt_pts, INT64 i8_offset, BOOL* pb_negative);

static INT32 _sbtl_lyric_set_init_page(
    HANDLE_T                            h_sbtl_wgl,
    UINT16                              ui2_page_num);

static INT32 _sbtl_lyric_clear_page(
    HANDLE_T                            h_sbtl_wgl);
static INT32 _sbtl_lyric_get_boundary(SBTL_PAGE_INST_MNGR_T*  pt_mngr);
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static SBTL_MNGR_FCT_TBL_T t_lyric_fnct_tbl = 
{
    TRUE,
    _lyric_page_mngr_constructor,
    _lyric_page_mngr_destructor,
    _lyric_page_mngr_parser_verify
};

static SBTL_LYRIC_FNC_TBL_T  at_lyric_fnc_tbl_list[] = 
{
    {
        MM_SBTL_TYPE_LYRIC_LRC,
        lrc_seek_time_stamp,
        lrc_load_string,
        lrc_timer_period,
        lrc_init,
        lrc_deinit,
        lrc_start,
        lrc_stop,
        lrc_parser_verify,
        lrc_config,
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
INT32 sbtl_lyric_alloc_time_stamp(
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*         pt_private,
    UINT16                                  ui2_time_stamp_cnt)
{
    SIZE_T                                  t_size;
    UINT16                                  ui1_i;
    UINT16*                                 pui2_order;
    
    ASSERT(pt_private, ("pt_private=NULL\n"));
    if(ui2_time_stamp_cnt == 0)
    {
        return MMSBTLR_INV_ARG;
    }
    pt_private->ui2_time_max_cnt = ui2_time_stamp_cnt;

    t_size = sizeof(SBTL_TAG_INFO_T) * (ui2_time_stamp_cnt + 1);
    pt_private->pt_time_stamp = (SBTL_TAG_INFO_T*)x_mem_alloc(t_size);
    if(pt_private->pt_time_stamp == NULL)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }
    x_memset(pt_private->pt_time_stamp, 0, t_size);

    t_size = sizeof(UINT16) * (ui2_time_stamp_cnt + 1);
    pui2_order = (UINT16*)x_mem_alloc(t_size);
    if(pui2_order == NULL)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }

    pt_private->pui2_time_order = pui2_order;
    for(ui1_i = 0; ui1_i < (UINT16)(ui2_time_stamp_cnt + 1); ui1_i ++)
    {
        *pui2_order = ui1_i;    
        pui2_order ++;
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
INT32 sbtl_lyric_free_time_stamp(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private)
{
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private->ui2_time_cnt = 0;
    pt_private->ui2_time_max_cnt = 0;
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
INT32 sbtl_lyric_time_stamp_order(
    SBTL_PAGE_INST_MNGR_T*              pt_mngr,
    UINT16                              ui2_order_start,
    UINT16                              ui2_order_end)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    UINT16                              ui2_start;
    UINT16                              ui2_end;
    UINT16                              ui2_i,ui2_j;
    UINT16                              ui2_cur,ui2_next;
    UINT16                              ui2_tmp_end;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    if ((ui2_order_end <= ui2_order_start) || (ui2_order_start > pt_private->ui2_time_cnt))
    {
        return MMSBTLR_INV_ARG;
    }

    ui2_start = ui2_order_start;
    if(ui2_order_end > pt_private->ui2_time_cnt)
    {
        ui2_end = pt_private->ui2_time_cnt;
    }
    else
    {
        ui2_end = ui2_order_end;
    }
    
    for(ui2_j = 0; ui2_j < (ui2_end - ui2_start); ui2_j ++)
    {
        ui2_tmp_end = ui2_end -ui2_j;
        ui2_tmp_end --;

        for(ui2_i = ui2_start; ui2_i < ui2_tmp_end; ui2_i ++)
        {
            ui2_cur = pt_private->pui2_time_order[ui2_i];
            ui2_next = pt_private->pui2_time_order[ui2_i+1];
            if(pt_private->pt_time_stamp[ui2_cur].t_pts_s > pt_private->pt_time_stamp[ui2_next].t_pts_s)
            {
                pt_private->pui2_time_order[ui2_i] = ui2_next;
                pt_private->pui2_time_order[ui2_i+1] = ui2_cur;
            }
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
static INT32 _sbtl_lyric_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        BOOL                    b_pts_neg,
                                        PTS_T                   t_pts_current,
                                        UINT16                  ui2_time_cnt,
                                        UINT16*                 pt_found_cnt)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                            i4_ret;
    SBTL_LYRIC_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_seek_time, ("pt_fnc_table->pf_seek_time=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_seek_time))
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_fnc_table->pf_seek_time(pt_mngr,
                                        b_pts_neg,
                                        t_pts_current,
                                        ui2_time_cnt,
                                        pt_found_cnt);
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
static INT32 _sbtl_lyric_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                           UINT16                  ui2_load_idx)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;
    SBTL_LYRIC_FNC_TBL_T*            pt_fnc_table;

    if(ui2_load_idx == LYRIC_IDX_NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_load_string, ("pt_fnc_table->pf_load_string=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_load_string))
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_fnc_table->pf_load_string(pt_mngr,ui2_load_idx);
    
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
static UINT32 _sbtl_lyric_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_LYRIC_FNC_TBL_T*            pt_fnc_table;
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
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
static VOID _sbtl_lyric_timer_cb_fct(HANDLE_T     pt_tm_handle,
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
static INT32 _sbtl_lyric_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*   pt_private;
    INT32                             i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

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
                           _sbtl_lyric_timer_period(pt_mngr),
                           X_TIMER_FLAG_REPEAT,
                           _sbtl_lyric_timer_cb_fct,
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
static VOID _sbtl_lyric_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*   pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }
}

static INT32 _sbtl_lyric_clear_page(
    HANDLE_T                            h_sbtl_wgl)
{
    INT32                                   i4_ret;

    i4_ret = sbtl_wgl_do_cmd (h_sbtl_wgl,
                           SBTL_WGL_CMD_TEXT_CLEAR_STR,
                           NULL,
                           NULL);
    if (i4_ret != MMSBTLR_OK)
    {
        ASSERT(0, ("clear text page error!!\n"));
    }    

    return i4_ret;
}

static INT32 _sbtl_lyric_set_init_page(
    HANDLE_T                            h_sbtl_wgl,
    UINT16                              ui2_page_num)
{
    INT32                                   i4_ret;
    UINT16                                  ui2_cnt;
    UTF16_T                                 w2s_text[2];

    w2s_text[0] = 0x20;
    w2s_text[1] = 0;
    
    for(ui2_cnt = 0; ui2_cnt < ui2_page_num; ui2_cnt ++)
    {
        i4_ret = sbtl_wgl_do_cmd (h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_SET_STR,
                               (VOID*)w2s_text,
                               (VOID*)x_uc_w2s_strlen (w2s_text));

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("set text error!!\n"));
        }
    }
    
    return MMSBTLR_OK;
}

static INT32 _sbtl_lyric_init_hlt_inf(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*         pt_private;
    MM_SBTL_DISP_PAGE_T*                    pt_disp_page = &pt_mngr->pt_engine->t_cfg.t_disp_page;
    UINT16                                  ui2_tmp;
    INT32                                   i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_disp_page->e_page_mode == MM_SBTL_PAGE_HLT_FIXED)
    {
        ui2_tmp = (UINT16)(pt_private->ui2_hlt_page_idx * (1 + pt_disp_page->ui2_interval_line));
        if(ui2_tmp > pt_disp_page->ui2_page_line_num)
        {
            ui2_tmp = pt_disp_page->ui2_page_line_num >> 1;
        }
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_HLT_POS,
                               SBTL_WGL_PACK(ui2_tmp), 
                               SBTL_WGL_PACK(SBTL_WGL_TEXT_POSITION_BY_INDICATE));
        
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }  

        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_HLT_STR,
                               SBTL_WGL_PACK(pt_private->ui2_hlt_page_idx), 
                               NULL);

        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }         
    }
    else
    {
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_HLT_POS,
                               SBTL_WGL_PACK(0), 
                               SBTL_WGL_PACK(SBTL_WGL_TEXT_POSITION_BY_STR_IDX));
        
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }

        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_HLT_STR,
                               SBTL_WGL_PACK(pt_private->ui2_hlt_page_idx), 
                               NULL);

        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
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
static INT32 _sbtl_lyric_start(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    INT32                                   i4_ret;
    HANDLE_TYPE_T                           e_type;
    VOID*                                   pt_widget;
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*         pt_private;
    SBTL_LYRIC_FNC_TBL_T*                   pt_fnc_table;
    MM_SBTL_DISP_PAGE_T*                    pt_disp_page = &pt_mngr->pt_engine->t_cfg.t_disp_page;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_fnc_table = pt_private->pt_fnc_table;

    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_start, ("pt_fnc_table->pf_start=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_start))
    {
        return MMSBTLR_INV_ARG;
    }
    
    if (pt_private->h_sbtl_wgl)
    {
        return MMSBTLR_EXIST;
    }
    if(pt_mngr->pt_engine->t_cfg.h_gl_plane == NULL_HANDLE)
    {
        return (MMSBTLR_WGL_FAIL);
    }
    
    /*change log: ap directly set widget to lyric, so that ap can control the layer lyric show*/

    if (HR_OK == handle_get_type_obj(pt_mngr->pt_engine->t_cfg.h_gl_plane, 
                                     &e_type, 
                                     &pt_widget)                       &&
        IS_WIDGET_TYPE(e_type)/*                                       &&
        !ST_DESTROYED(pt_widget)*/)
    {
        pt_private->h_sbtl_wgl = pt_mngr->pt_engine->t_cfg.h_gl_plane;
    }
    else
    {
        /* Get canvas */
        GL_RECT_T       t_sbtl_rc_scrn;
        GL_RECT_T       t_canvas_rect;
        UINT32          ui4_canvas_w = 0;
        UINT32          ui4_canvas_h = 0;
        HANDLE_T        h_canvas     = NULL_HANDLE;
        
        h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
        
        i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }    

        SBTL_SET_RECT_BY_SIZE(&t_canvas_rect,0,0,(INT32)ui4_canvas_w,(INT32)ui4_canvas_h);
        
        if (pt_mngr->pt_engine->t_cfg.b_rect_osd)
        {
            SBTL_INTERSECT_RECT(&t_sbtl_rc_scrn,&pt_mngr->pt_engine->t_cfg.t_rect_osd,&t_canvas_rect);
        }
        else
        {
            t_sbtl_rc_scrn.i4_left = (INT32)(ui4_canvas_w/6);
            t_sbtl_rc_scrn.i4_top = (INT32)(ui4_canvas_h/10);
            t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w-(INT32)(ui4_canvas_w/6);
            t_sbtl_rc_scrn.i4_bottom = (INT32)(ui4_canvas_h)-(INT32)(ui4_canvas_h/10);
        } 

        i4_ret = sbtl_wgl_create_widget(h_canvas,
                                        SBTL_WGL_WIDGET_TEXT,
                                        SBTL_WGL_BORDER_NULL,
                                        &t_sbtl_rc_scrn,
                                        255,
                                        (VOID*) (SBTL_WGL_STL_TEXT_MAX_DIS_LINE
                                                |SBTL_WGL_STL_GL_NO_IMG_UI
                                                |SBTL_WGL_STL_TEXT_MAX_128 
                                                |SBTL_WGL_STL_TEXT_MULTISTR),
                                        NULL,
                                        &(pt_private->h_sbtl_wgl));
        
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }

        i4_ret = sbtl_wgl_float(pt_private->h_sbtl_wgl,TRUE,FALSE);
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }
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

    /*Set page information*/
    _sbtl_lyric_init_hlt_inf(pt_mngr);
    
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_TEXT_SET_INTERLACED,
                           SBTL_WGL_PACK(pt_disp_page->ui2_interval_line), 
                           NULL);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }  
    
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_TEXT_SET_LINE_GAP,
                           SBTL_WGL_PACK(pt_disp_page->i4_line_gap), 
                           NULL);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }  

    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);

    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    sbtl_feeder_init(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE);

    pt_private->ui4_seek_offset = 0;
    pt_private->ui2_cur_idx = LYRIC_IDX_NULL;
    pt_private->t_pts_e = 0;
    pt_private->t_pts_e = 0;
    pt_private->ui2_page_s = 0;
    pt_private->ui2_page_e = 0;
    pt_private->ui2_disp_num = 0;
    pt_private->i8_usr_pts_ofst = 0;
    pt_private->e_tm_ofst_mode = MM_SBTL_TM_OFST_MODE_AUTO;
    if(pt_disp_page->e_page_mode == MM_SBTL_PAGE_SCROLL_LINE)
    {
        pt_disp_page->ui2_hlt_fix_line = 0;
    }

    if(pt_mngr->pt_engine->t_cfg.ui4_cmd_mask & MM_SBTL_CMD_TIME_OFST)
    {
        i4_ret = pt_fnc_table->pf_config(pt_mngr, 
                                         (VOID*)&pt_mngr->pt_engine->t_cfg.t_time_ofst, 
                                         EVN_MM_SBTL_SET_ADJUST_TIME);        
        if (MMSBTLR_OK != i4_ret)
        {
           return (MMSBTLR_WGL_FAIL);
        }
    }
    
    pt_fnc_table->pf_start(pt_mngr);

    _sbtl_lyric_timer_start(pt_mngr);

    pt_mngr->b_started = TRUE;

    /*First send the null string for the page ,that is text widget requirment.*/
    _sbtl_lyric_set_init_page(pt_private->h_sbtl_wgl,pt_private->ui2_page_str_num);
    
    _sbtl_lyric_new_page(pt_mngr);
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
static INT32 _sbtl_lyric_stop(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*      pt_private;
    SBTL_LYRIC_FNC_TBL_T*                pt_fnc_table;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;

    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    ASSERT(pt_fnc_table->pf_stop, ("pt_fnc_table->pf_stop=NULL\n"));
    if ((NULL==pt_fnc_table) || (NULL==pt_fnc_table->pf_stop))
    {
        return MMSBTLR_INV_ARG;
    }
    
    _sbtl_lyric_timer_stop(pt_mngr);

    pt_fnc_table->pf_stop(pt_mngr);
    
    _sbtl_lyric_remove(pt_mngr);
    /* The widget had already been closed */
    /*if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }*/

    pt_mngr->b_started = FALSE;
    pt_private->e_state = LYRIC_STATE_NONE; 
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
static INT32 _sbtl_lyric_need_dmx(SBTL_PAGE_INST_MNGR_T*      pt_mngr, 
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
static INT32 _sbtl_lyric_handle_timing(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                               PTS_T                      t_pts)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    PTS_T                               t_pts_current = t_pts;
    BOOL                                b_negative;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));

    if(pt_private->e_tm_ofst_mode == MM_SBTL_TM_OFST_MODE_AUTO)
    {
        _sbtl_lyric_adjust_pts(&t_pts_current,pt_private->i8_pts_offset,&b_negative);
    }
    else if(pt_private->e_tm_ofst_mode == MM_SBTL_TM_OFST_MODE_USER)
    {
        _sbtl_lyric_adjust_pts(&t_pts_current,(pt_private->i8_pts_offset + pt_private->i8_usr_pts_ofst),&b_negative);
    } 
    
   _sbtl_lyric_new_page(pt_mngr);

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
static INT32 _sbtl_lyric_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                    VOID*                    pv_cfg_info,
                                    UINT32                   ui4_set_type)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_LYRIC_FNC_TBL_T*            pt_fnc_table;
    INT32                            i4_ret = MMSBTLR_OK;
    MM_SBTL_ENCODING_T               e_encoding;
    SBTL_CFG_T*                      pt_cfg;
    MM_SBTL_DISP_PAGE_T*             pt_disp_page; 
    GL_COLOR_T*                      pt_color;
    UINT16                           ui2_tmp;
    INT32                            i4_data;
    WGL_FONT_INFO_T*                 pt_font_info;
    UINT8                            ui1_tmp;
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    pt_fnc_table = pt_private->pt_fnc_table;
    ASSERT(pt_fnc_table, ("pt_fnc_table=NULL\n"));
    
    if (NULL==pt_fnc_table)
    {
        return MMSBTLR_OK;
    }

    if (pt_fnc_table->pf_config)
    {
        i4_ret = pt_fnc_table->pf_config(pt_mngr, pv_cfg_info, ui4_set_type);
    }

    pt_cfg = &(pt_mngr->pt_engine->t_cfg);
    ASSERT(pt_cfg, ("lyric config argment=NULL\n"));
    
    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_ADJUST_TIME:
        if(i4_ret == MMSBTLR_OK) 
        {
            _sbtl_lyric_new_page(pt_mngr);
        }
        break;

    case EVN_MM_SBTL_SET_OSD_FONT_INFO:
        if(i4_ret == MMSBTLR_OK) 
        {
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
            
        }
        break;

    case EVN_MM_SBTL_SET_ENCODING:
        e_encoding = *((MM_SBTL_ENCODING_T*)pv_cfg_info);
        
        pt_font_info = &(pt_mngr->pt_engine->t_cfg.t_font_info);
        sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);        
        if(e_encoding == MM_SBTL_ENCODE_AUTO)
        {
            x_strcpy(pt_font_info->a_c_font_name, MM_SBTL_DFT_FONT_NAME);
            pt_font_info->e_font_size  = MM_SBTL_DFT_FONT_SIZE;
            pt_font_info->e_font_style = MM_SBTL_DFT_FONT_STYLE;
            pt_font_info->e_font_cmap  = MM_SBTL_DFT_FONT_CMAP;
            pt_font_info->ui1_custom_size= MM_SBTL_DFT_FONT_CSTM_SIZE;
    
            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_GL_SET_FONT,
                                      (VOID*)&pt_mngr->pt_engine->t_cfg.t_font_info,
                                      NULL);
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }             
            
            pt_mngr->pt_engine->t_cfg.e_encoding = e_encoding = MM_SBTL_DFT_ENCODING;
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
            }else if(0xEF == ui1_tmp)
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
        pt_private->ui2_disp_num = 0;
        _sbtl_lyric_clear_page(pt_private->h_sbtl_wgl);
        _sbtl_lyric_set_init_page(pt_private->h_sbtl_wgl,pt_private->ui2_page_str_num);
        break;

        
    case EVN_MM_SBTL_SET_HLT_FG_COLOR:
        if(i4_ret == MMSBTLR_OK) 
        {   
            if(pt_cfg->t_fg_color_info.e_type != WGL_COLOR_SET_STANDARD)
            {
                return (MMSBTLR_WGL_FAIL);
            }
            
            pt_color = (GL_COLOR_T*)pv_cfg_info;
            x_memcpy ((VOID *)&pt_cfg->t_fg_color_info.u_color_data.t_standard.t_highlight, 
                      pt_color, 
                      sizeof(GL_COLOR_T));            

            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                   SBTL_WGL_CMD_GL_SET_COLOR,
                                   (VOID*)SBTL_WGL_CLR_TEXT, 
                                   (VOID*)&(pt_cfg->t_fg_color_info));
            
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            } 
            
            i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

            if (MMSBTLR_OK != i4_ret)
            {
               ASSERT(0, ("show text error!!\n"));
            }            
            
        }
        break;

    case EVN_MM_SBTL_SET_HLT_BG_COLOR:
        if(i4_ret == MMSBTLR_OK) 
        {   
            if(pt_cfg->t_bg_color_info.e_type != WGL_COLOR_SET_STANDARD)
            {
                return (MMSBTLR_WGL_FAIL);
            }
            
            pt_color = (GL_COLOR_T*)pv_cfg_info;
            x_memcpy ((VOID *)&pt_cfg->t_bg_color_info.u_color_data.t_standard.t_highlight, 
                      pt_color, 
                      sizeof(GL_COLOR_T));  

            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                   SBTL_WGL_CMD_GL_SET_COLOR,
                                   (VOID*)SBTL_WGL_CLR_BK, 
                                   (VOID*)&pt_cfg->t_bg_color_info);
            
            if (MMSBTLR_OK != i4_ret)
            {
               return (MMSBTLR_WGL_FAIL);
            }    
            
            i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

            if (MMSBTLR_OK != i4_ret)
            {
               ASSERT(0, ("show text error!!\n"));
            }            
            
        }
        break;

    case EVN_MM_SBTL_SET_PAGE_INF:
        if(i4_ret == MMSBTLR_OK) 
        {   
            pt_disp_page = (MM_SBTL_DISP_PAGE_T*)pv_cfg_info;
            if(pt_disp_page->ui2_page_line_num < 1)  
            {
                if (pt_private->e_state == LYRIC_STATE_DISPLAY)
                {                
                    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
                
                    if (MMSBTLR_OK != i4_ret)
                    {
                        ASSERT(0, ("clear widget fail!!/n"));
                    }
                }
                pt_private->e_state = LYRIC_STATE_NONE;
                pt_private->b_page_exist = FALSE;
                pt_private->ui2_page_str_num  = 0;
                pt_cfg->t_disp_page.ui2_page_line_num = 0;
                
               return (MMSBTLR_OK);
            }

            if(pt_disp_page->ui2_interval_line >= pt_disp_page->ui2_page_line_num)
            {
               return (MMSBTLR_WGL_FAIL);
            }                
            
            ui2_tmp = (UINT16) (pt_disp_page->ui2_page_line_num / (UINT16)(1 + pt_disp_page->ui2_interval_line)); 

            
            pt_private->ui2_page_str_num = ui2_tmp;
            
            if (pt_disp_page->e_page_mode == MM_SBTL_PAGE_HLT_FIXED) 
            {
                if(pt_disp_page->ui2_hlt_fix_line >= ui2_tmp)
                {
                    pt_disp_page->ui2_hlt_fix_line = (UINT16)(ui2_tmp >> 1);
                }

                pt_private->ui2_hlt_page_idx = pt_disp_page->ui2_hlt_fix_line ;
            }
            else
            {
                pt_private->ui2_hlt_page_idx = 0;
            }
            
            if(pt_disp_page->ui2_interval_line != pt_cfg->t_disp_page.ui2_interval_line)
            {
                i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_TEXT_SET_INTERLACED,
                                       SBTL_WGL_PACK(pt_disp_page->ui2_interval_line), 
                                       NULL);
                
                if (MMSBTLR_OK != i4_ret)
                {
                   return (MMSBTLR_WGL_FAIL);
                }  
            }

            if(pt_disp_page->i4_line_gap != pt_cfg->t_disp_page.i4_line_gap)
            {
                i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_TEXT_SET_LINE_GAP,
                                       SBTL_WGL_PACK(pt_disp_page->i4_line_gap), 
                                       NULL);
                
                if (MMSBTLR_OK != i4_ret)
                {
                   return (MMSBTLR_WGL_FAIL);
                }   
            }

            if((pt_disp_page->ui2_page_line_num != pt_cfg->t_disp_page.ui2_page_line_num) ||
                (pt_disp_page->ui2_interval_line != pt_cfg->t_disp_page.ui2_interval_line))
            {
                i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                       SBTL_WGL_CMD_TEXT_CLEAR_STR,
                                       NULL,
                                       NULL);

                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("set text error!!\n"));
                }             
                pt_private->ui2_page_s = 0;
                pt_private->ui2_page_e = 0;
                pt_private->ui2_disp_num = 0;
                _sbtl_lyric_clear_page(pt_private->h_sbtl_wgl);
                _sbtl_lyric_set_init_page(pt_private->h_sbtl_wgl,pt_private->ui2_page_str_num);
            }

            x_memcpy(&pt_cfg->t_disp_page,pt_disp_page,sizeof(MM_SBTL_DISP_PAGE_T));

            _sbtl_lyric_init_hlt_inf(pt_mngr);

            _sbtl_lyric_new_page(pt_mngr);

        }
        break;
        
    case EVN_MM_SBTL_SET_LINE_GAP:
        i4_data = *((INT32 *)pv_cfg_info);
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_SET_LINE_GAP,
                               SBTL_WGL_PACK(i4_data), 
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
static INT32 _sbtl_lyric_get(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                            UINT32                   ui4_get_type,
                            VOID*                    pv_get_info,
                            SIZE_T                   z_get_info_len)
{

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
static VOID _sbtl_lyric_remove(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                            i4_ret;
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_private->b_page_exist)
    {
        return;
    }
    
    if (pt_private->e_state == LYRIC_STATE_DISPLAY)
    {                
        i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
   
        if (MMSBTLR_OK != i4_ret)
        {
            ASSERT(0, ("clear widget fail!!/n"));
        }
    }

    pt_private->e_state = LYRIC_STATE_NONE;
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
static INT32 _sbtl_lyric_adjust_pts(PTS_T* pt_pts, INT64 i8_offset, BOOL* pb_negative)
{
    INT64                           i8_pts_tmp;

    if((pt_pts == NULL) || (pb_negative == NULL))
    {
        return MMSBTLR_INV_ARG;
    }

    i8_pts_tmp = (INT64)(*pt_pts);
    i8_pts_tmp += i8_offset;

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
static INT32 _sbtl_lyric_set_next_page(
    SBTL_PAGE_INST_MNGR_T*                  pt_mngr,
    INT16                                   i2_start_idx,
    UINT16                                  ui2_count,
    BOOL                                    b_flag)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    INT16                               i2_tmp;
    UINT16                              ui2_idx;
    INT32                               i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));

    i2_tmp = i2_start_idx;
    for(ui2_idx = 0; ui2_idx < ui2_count; ui2_idx ++)
    {
        if ((i2_tmp >= 0) && (i2_tmp < (INT16)pt_private->ui2_time_cnt))
        {
            i4_ret = _sbtl_lyric_load_string(pt_mngr,(UINT16)i2_tmp);
            if (i4_ret != MMSBTLR_OK)
            {
                pt_private->w2s_text[0] = 0x20;
                pt_private->w2s_text[1] = 0;
            }
        }
        else
        {
            pt_private->w2s_text[0] = 0x20;
            pt_private->w2s_text[1] = 0;
        }

        /*Set the string to widget.*/
        if(x_uc_w2s_strlen (pt_private->w2s_text) == 0)
        {
            pt_private->w2s_text[0] = 0x20;
            pt_private->w2s_text[1] = 0;
        }
        
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_SET_STR,
                               (VOID*)pt_private->w2s_text,
                               (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("set text error!!\n"));
        }

        if(b_flag == TRUE)
        {
            i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                   SBTL_WGL_CMD_TEXT_PLAY_NEXT_STR,
                                   (VOID*)pt_private->w2s_text,
                                   (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));

            if (i4_ret != MMSBTLR_OK)
            {
                ASSERT(0, ("set text error!!\n"));
            }
        }

        i2_tmp ++ ;
    }
   
    return MMSBTLR_OK;
}


static INT32 _sbtl_lyric_set_prev_page(
    SBTL_PAGE_INST_MNGR_T*                  pt_mngr,
    INT16                                   i2_start_idx,
    UINT16                                  ui2_count)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    INT16                               i2_data;
    UINT16                              ui2_idx;
    INT32                               i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));


    i2_data = (INT16)i2_start_idx;
        
    for(ui2_idx = 0; ui2_idx < ui2_count; ui2_idx ++)
    {
        if(i2_data >= 0 && i2_data <= (INT16)pt_private->ui2_time_cnt)
        {
            i4_ret = _sbtl_lyric_load_string(pt_mngr,(UINT16)i2_data);
            if (i4_ret != MMSBTLR_OK)
            {
                pt_private->w2s_text[0] = 0x20;
                pt_private->w2s_text[1] = 0;
            }
        }
        else
        {
            pt_private->w2s_text[0] = 0x20;
            pt_private->w2s_text[1] = 0;
        }
        
        /*Set the string to widget.*/
        if(x_uc_w2s_strlen (pt_private->w2s_text) == 0)
        {
            pt_private->w2s_text[0] = 0x20;
            pt_private->w2s_text[1] = 0;
        }
        
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_SET_PREV_STR,
                               (VOID*)pt_private->w2s_text,
                               (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("set text error!!\n"));
        }
        
        i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                               SBTL_WGL_CMD_TEXT_PLAY_PREV_STR,
                               (VOID*)pt_private->w2s_text,
                               (VOID*)x_uc_w2s_strlen (pt_private->w2s_text));

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("set text error!!\n"));
        } 

        i2_data -- ;

    } 
    
    return MMSBTLR_OK;
}
static INT32 _sbtl_lyric_get_range_lines(
    UINT16                                  ui2_old_s,
    UINT16                                  ui2_old_e,
    UINT16                                  ui2_new_s,
    UINT16                                  ui2_new_e,
    BOOL                                    b_next,
    INT16*                                  pi2_start,
    INT16*                                  pi2_count)
{
    if(pi2_start == NULL || pi2_count == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    if(ui2_new_e < ui2_new_s || ui2_old_e < ui2_old_s)
    {
        *pi2_start = 0;
        *pi2_count = 0;
        return MMSBTLR_INV_ARG;
    }

    if ((b_next == TRUE) && (ui2_new_e > ui2_old_e))
    {
        if(ui2_new_s > ui2_old_e)
        {
            *pi2_start = (INT16)ui2_new_s;
            *pi2_count = ui2_new_e - ui2_new_s + 1;
        }
        else
        {
            *pi2_start = ui2_old_e + 1;
            *pi2_count = ui2_new_e - (*pi2_start) + 1;
        }
    }
    else if((b_next == FALSE) && (ui2_new_s < ui2_old_s))
    {
        if(ui2_new_e < ui2_old_s)
        {
            *pi2_start = (INT16)ui2_new_e;
            *pi2_count = ui2_new_e - ui2_new_s + 1;
        }
        else
        {
            *pi2_start = ui2_old_s - 1;
            *pi2_count = ui2_old_s - ui2_new_s;
        }        
    }
    else
    {
        *pi2_start = 0;
        *pi2_count = 0;
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
static INT32 _sbtl_lyric_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    PTS_T                           t_pts_current;
    UINT16                          ui2_found = 0;
    BOOL                            b_negative = FALSE;
    INT32                           i4_ret;
    MM_SBTL_DISP_PAGE_T*            pt_disp_page = NULL;
     INT16                          i2_cnt;
    INT16                           i2_page_s;
    INT16                           i2_page_e;
    INT16                           i2_page_start;

    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }
            
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));


    if(pt_private->ui2_page_str_num < 1)
    {
        if (pt_private->e_state == LYRIC_STATE_DISPLAY)
        {                
            i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, FALSE);
        
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("clear widget fail!!/n"));
            }
        }
        pt_private->e_state = LYRIC_STATE_NONE;
        pt_private->b_page_exist = FALSE;
        return MMSBTLR_OK;
    }    

    i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

    if (i4_ret != MMSBTLR_OK)
    {
        ASSERT(0, ("Get STC Error!!\n"));
    }

    if(pt_private->e_tm_ofst_mode == MM_SBTL_TM_OFST_MODE_AUTO)
    {
        _sbtl_lyric_adjust_pts(&t_pts_current,pt_private->i8_pts_offset,&b_negative);
    }
    else if(pt_private->e_tm_ofst_mode == MM_SBTL_TM_OFST_MODE_USER)
    {
        _sbtl_lyric_adjust_pts(&t_pts_current,(pt_private->i8_pts_offset + pt_private->i8_usr_pts_ofst),&b_negative);
    }        
    
    
    i4_ret = _sbtl_lyric_seek_time_stamp(pt_mngr,
                                     b_negative,
                                     t_pts_current,
                                     pt_private->ui2_page_str_num,
                                     &ui2_found);
    if(ui2_found > 0)
    {
        /*Find lyric.*/
        if(t_pts_current < pt_private->t_pts_s)
        {
            //DBG_INFO(("[Lyric]Display prev one that in the page!\n"));
            //i4_ret = _sbtl_lyric_load_string(pt_mngr,pt_private->ui2_cur_idx);
            //pt_private->e_state = LYRIC_STATE_DATA_RDY;
            _sbtl_lyric_remove(pt_mngr);
        }
        else if((t_pts_current >= pt_private->t_pts_s) && (t_pts_current < pt_private->t_pts_e))
        {
            pt_private->e_state = LYRIC_STATE_DISPLAY;
            pt_disp_page = &(pt_mngr->pt_engine->t_cfg.t_disp_page);
            if(pt_disp_page->e_page_mode == MM_SBTL_PAGE_HLT_FIXED)
            {
                i2_page_s = (INT16)(pt_private->ui2_cur_idx - pt_private->ui2_hlt_page_idx);
                i2_page_e = (INT16)(i2_page_s + pt_private->ui2_page_str_num - 1);
                /*if(i2_page_s < 0)
                {
                    i2_page_s = 0;
                }
                
                if(i2_page_e > (INT16)pt_private->ui2_time_cnt)
                {
                    i2_page_e = (INT16)(pt_private->ui2_time_cnt - 1);
                }*/

                if(pt_private->ui2_disp_num == 0)
                {
                    i2_page_start = i2_page_s;
                    i2_cnt = i2_page_e - i2_page_s + 1;
                    if(i2_cnt > 0)
                    {
                        _sbtl_lyric_set_next_page(pt_mngr,i2_page_start,(UINT16)i2_cnt,TRUE);
                    }
                }
                else
                {
                    _sbtl_lyric_get_range_lines(pt_private->ui2_page_s,
                                                pt_private->ui2_page_e,
                                                (UINT16)i2_page_s,
                                                (UINT16)i2_page_e,
                                                TRUE,
                                                &i2_page_start,
                                                &i2_cnt);
                    if(i2_cnt > 0)
                    {
                        _sbtl_lyric_set_next_page(pt_mngr,i2_page_start,(UINT16)i2_cnt,TRUE);
                    }

                    _sbtl_lyric_get_range_lines(pt_private->ui2_page_s,
                                                pt_private->ui2_page_e,
                                                (UINT16)i2_page_s,
                                                (UINT16)i2_page_e,
                                                FALSE,
                                                &i2_page_start,
                                                &i2_cnt);
                    if(i2_cnt > 0)
                    {
                        _sbtl_lyric_set_prev_page(pt_mngr,i2_page_start,(UINT16)i2_cnt);
                    }
                    
                }


                pt_private->ui2_page_s = (UINT16)i2_page_s;
                pt_private->ui2_page_e = (UINT16)i2_page_e;

                if(pt_private->ui2_page_e >= pt_private->ui2_page_s)
                {
                    pt_private->ui2_disp_num = pt_private->ui2_page_e - pt_private->ui2_page_s;
                }
                else
                {
                    pt_private->ui2_disp_num = 0;
                }

            }
            else if(pt_disp_page->e_page_mode == MM_SBTL_PAGE_SCROLL_LINE)
            {
                if((pt_private->ui2_cur_idx > pt_private->ui2_page_e) ||
                   (pt_private->ui2_cur_idx < pt_private->ui2_page_s) ||
                   (pt_private->ui2_disp_num == 0))
                {
                    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                           SBTL_WGL_CMD_TEXT_CLEAR_STR,
                                           NULL,
                                           NULL);

                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("set text error!!\n"));
                    } 
                    
                    i4_ret = _sbtl_lyric_get_boundary(pt_mngr);

                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("get boundary error!!\n"));
                    } 

                    i2_cnt = (INT16)pt_private->ui2_page_str_num;
                    _sbtl_lyric_set_next_page(pt_mngr,(INT16)pt_private->ui2_page_s,(UINT16)i2_cnt,FALSE);
                    //pt_private->ui2_page_s = pt_private->ui2_cur_idx;
                    //pt_private->ui2_page_e = (UINT16)(pt_private->ui2_cur_idx + pt_private->ui2_page_str_num - 1);
                    pt_private->ui2_hlt_page_idx = pt_private->ui2_cur_idx - pt_private->ui2_page_s;
                }
                else
                {
                    pt_private->ui2_hlt_page_idx = pt_private->ui2_cur_idx - pt_private->ui2_page_s;
                }

                if(pt_private->ui2_page_e >= pt_private->ui2_page_s)
                {
                    pt_private->ui2_disp_num = pt_private->ui2_page_e - pt_private->ui2_page_s;
                }
                else
                {
                    pt_private->ui2_disp_num = 0;
                }
                
                if(pt_private->ui2_hlt_page_idx < pt_private->ui2_page_str_num)
                {
                    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                           SBTL_WGL_CMD_TEXT_HLT_STR,
                                           SBTL_WGL_PACK(pt_private->ui2_hlt_page_idx),
                                           NULL);

                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("set text error!!\n"));
                    }
                }
 

            }

            i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);

            if (MMSBTLR_OK != i4_ret)
            {
               ASSERT(0, ("show text error!!\n"));
            }

            pt_private->b_page_exist = TRUE;                 
            
            
        }
        else
        {
            /*Display the next lyric that in the page range.*/
            DBG_INFO(("[Lyric]Display next one that in the page!\n"));
        }
        
        
    }
    else if(i4_ret == MMSBTLR_EOT)
    {
        /*Lyric is at the end*/
        pt_private->e_state = LYRIC_STATE_END;
        DBG_INFO(("Lyric is at the end!\n"));
    }
    else
    {
        pt_private->e_state = LYRIC_STATE_NONE;
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
static SBTL_PAGE_INST_MNGR_T* _lyric_page_mngr_constructor (SBTL_ENGINE_T*    pt_engine)
{
    SBTL_PAGE_INST_MNGR_T*              pt_mngr;
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    UINT8                               ui1_idx;
    INT32                               i4_ret;
    UINT16                              ui2_tmp;
    MM_SBTL_DISP_PAGE_T*                pt_disp_page;

    ASSERT(pt_engine, ("_lyric_page_mngr_constructor pt_engine=NULL\n"));
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

        pt_mngr->pf_start = _sbtl_lyric_start;
        pt_mngr->pf_stop = _sbtl_lyric_stop;
        pt_mngr->pf_cfg = _sbtl_lyric_config;
        pt_mngr->pf_handle_timing = _sbtl_lyric_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_lyric_need_dmx;
        pt_mngr->pf_new_page = _sbtl_lyric_new_page;
        pt_mngr->pf_get = _sbtl_lyric_get;
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }
        x_memset(pt_mngr->pv_private, 0, sizeof(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T));

        pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        
        for(ui1_idx=0; ; ui1_idx++)
        {
            if (ui1_idx >= (UINT8)(sizeof(at_lyric_fnc_tbl_list)/sizeof(at_lyric_fnc_tbl_list[0])))
            {
                break;
            }
            
            if (MM_SBTL_TYPE_END == at_lyric_fnc_tbl_list[ui1_idx].e_sbtl_text_type)
            {
                break;
            }

            if(at_lyric_fnc_tbl_list[ui1_idx].e_sbtl_text_type == pt_engine->t_cfg.e_sbtl_type)
            {
                pt_private->pt_fnc_table = &at_lyric_fnc_tbl_list[ui1_idx];

                sbtl_feeder_init(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE);

                {
                    UINT8 ui1_tmp;

                    pt_private->b_UTF16 = FALSE;

                    sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                    
                    if (0xFF == ui1_tmp)
                    {
                        sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                        if (0xFE == ui1_tmp)
                        {
                            pt_private->b_UTF16 = TRUE;
                            pt_private->e_language = MM_SBTL_LANG_UNICODE;
                            pt_engine->t_cfg.e_encoding = MM_SBTL_ENCODE_UTF16;
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
                    
                    //sbtl_feeder_inpnbyte_p(pt_mngr->pt_engine, 2, &pui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
                    //sbtl_check_encoding_info(pt_engine, pui1_tmp[0], pui1_tmp[1], &pt_private->b_UTF16);
                    sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                }

                pt_disp_page = &(pt_engine->t_cfg.t_disp_page);
                ui2_tmp = (UINT16) (pt_disp_page->ui2_page_line_num / (UINT16)(1 + pt_disp_page->ui2_interval_line)); 
                if(pt_disp_page->e_page_mode == MM_SBTL_PAGE_HLT_FIXED)
                {
                    if(pt_disp_page->ui2_hlt_fix_line >= ui2_tmp)
                    {
                        pt_disp_page->ui2_hlt_fix_line = (UINT16)(ui2_tmp >> 1);
                    }
                    pt_private->ui2_hlt_page_idx = pt_disp_page->ui2_hlt_fix_line;
                }
                else
                {
                    pt_private->ui2_hlt_page_idx = 0;
                }
                pt_private->ui2_page_str_num = ui2_tmp;
            
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
static VOID _lyric_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_lyric_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_lyric_page_mngr_destructor pt_private=NULL\n"));
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
static BOOL _lyric_page_mngr_parser_verify(
    SBTL_ENGINE_T*                              pt_engine,
    UINT8*                                      pt_type)

{

    ASSERT(pt_engine, ("_lyric_page_mngr_parser_verify pt_engine=NULL\n"));
    ASSERT(pt_type, ("_lyric_page_mngr_parser_verify pt_type=NULL\n"));
    if ((NULL==pt_engine) || (NULL==pt_type))
    {
        return FALSE;
    }

    if (*pt_type == MM_SBTL_TYPE_LYRIC_LRC)
    {
        return TRUE;
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
VOID x_sbtl_lyric_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_LYRIC_LRC,
                  &t_lyric_fnct_tbl);

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
static INT32 _sbtl_lyric_get_boundary(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT16                           i2_cur_idx;
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T  *pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));

    i2_cur_idx = pt_private->ui2_cur_idx;
    pt_private->ui2_page_s = (i2_cur_idx/pt_private->ui2_page_str_num)*pt_private->ui2_page_str_num;
    pt_private->ui2_page_e = pt_private->ui2_page_s + pt_private->ui2_page_str_num - 1;

    if(pt_private->ui2_page_e >= pt_private->ui2_time_cnt)
    {
        pt_private->ui2_page_e = pt_private->ui2_time_cnt - 1;
    }

    //pt_private->ui2_disp_num = pt_private->ui2_page_e - pt_private->ui2_page_s;

    return MMSBTLR_OK;

    
}


