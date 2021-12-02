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
 * $RCSfile: sbtl_mkv_ssa_parser.c,v $
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
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "inc/x_mm_common.h"
#include "unicode/x_uc_str.h"

#include "mutil/mm_util.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_mkv/sbtl_mkv_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_mkv/sbtl_mkv_ssa_parser.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_mm_common.h"
#include "x_uc_str.h"

#include "../../mm_util.h"

#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"

#include "sbtl_mkv_text_page_inst_mngr.h"
#include "sbtl_mkv_ssa_parser.h"
#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

#define CARRIAGE_RETURN             0x0d
#define LINE_FEED                   0x0a
#define CHAR_TAB                    0x09
#define CHAR_SPACE                  0x20

typedef struct _SSA_PRIVATE_T
{
    UINT8   ui1_start_offset;
    UINT8   ui1_end_offset;
    UINT8   ui1_text_offset;
}SSA_PRIVATE_T;

typedef enum
{
    SBTL_SEARCH_BEGIN,
    SBTL_SEARCH_MIDDLE
} SBTL_SEARCH_TYPE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/

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
INT32 mkv_ssa_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                              PTS_T                         t_pts_current,
                              PTS_T*                        pt_pts_start,
                              PTS_T*                        pt_pts_end,
                              BOOL*                         pb_found)
{
    BOOL            b_found;
    PTS_T           at_pts[2];
    UINT32          ui4_data_1;
    UINT32          ui4_data_2;
    UINT32          ui4_pts;
    UINT32          ui4_duration;
    INT32           i4_ret;

    *pb_found = FALSE;
    
    while (1)
    {
        b_found = sbtl_dmx_data_que_query(pt_mngr->pt_engine,
                                          &ui4_data_1,
                                          &ui4_data_2,
                                          &ui4_pts,
                                          &ui4_duration);
        if (!b_found)
        {
            return MMSBTLR_OK;
        }

        at_pts[0] = ui4_pts;
        at_pts[1] = ui4_pts+ui4_duration;

        if ((at_pts[0] > at_pts[1]) ||
            (t_pts_current >= at_pts[1]))
        {
            PTS_T       t_stc;
                
            *pt_pts_start = 0;
            *pt_pts_end = 0;

            sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                       &ui4_data_1,
                                       &ui4_data_2,
                                       &ui4_pts,
                                       &ui4_duration,
                                       &t_stc);

            
            i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                           ui4_data_1,
                                           ui4_data_2);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
            }

            continue;
        }

        break;
    }

    *pt_pts_start = ui4_pts;
    *pt_pts_end = ui4_pts+ui4_duration;

    *pb_found = TRUE;
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
INT32 mkv_ssa_load_string(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    UINT8                                   ui1_idx;
    UINT8                                   ui1_count;
    UINT8                                   ui1_tmp;
    BOOL                                    b_found;
    BOOL                                    b_in_brace;
    INT32                                   i4_ret;
    UINT32                                  ui4_data_1;
    UINT32                                  ui4_data_2;
    UINT32                                  ui4_pts;
    UINT32                                  ui4_duration;
    UINT32                                  ui4_pos;
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    
    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    ui1_idx = 0;
    ui4_pos = 0;
    b_in_brace = FALSE;


    b_found = sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                         &ui4_data_1,
                                         &ui4_data_2,
                                         &ui4_pts,
                                         &ui4_duration,
                                         &pt_private->t_append_stc);
    if (!b_found)
    {
        ASSERT(0, ("mkv_srt_load_string que retrival fail\n"));
        return MMSBTLR_NOT_FOUND;
    }

    i4_ret = sbtl_dmx_retrive_data(pt_mngr->pt_engine,
                                   ui4_data_1,
                                   ui4_data_2,
                                   pt_private->aui1_block_buf);
    if (MMSBTLR_OK != i4_ret)
    {
        ASSERT(0, ("mkv_srt_load_string data retrival fail\n"));
        return MMSBTLR_NOT_FOUND;
    }

    i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                           ui4_data_1,
                                           ui4_data_2);
    if (MMSBTLR_OK != i4_ret)
    {
        x_dbg_stmt("DEMUX_SET_TYPE_UNLOCK_BUFFER fail=%d\n", i4_ret);
        //ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
        return MMSBTLR_DMX_FAIL;
    }
    
    /*Events are stored in the Block in the following order: 
    ReadOrder, Layer, Style, Name, MarginL, MarginR, MarginV, Effect, Text (Layer comes from ASS specs ... it's empty for SSA.)*/
    ui1_count = 0;
    while (1)
    {
        if (ui4_pos >= ui4_data_2)
        {
            return MMSBTLR_EOT;
        }
        
        ui1_tmp = pt_private->aui1_block_buf[ui4_pos++];

        if (ui1_tmp == ',')
        {
            ui1_count++;
        }

        if (ui1_count >= 8)
        {
            break;
        }
    }

    while (1)
    {
        if (ui4_pos >= ui4_data_2)
        {
            return MMSBTLR_EOT;
        }

        ui1_tmp = pt_private->aui1_block_buf[ui4_pos++];

        if (ui1_tmp != ' ')
        {
            ui4_pos--;
            break;
        }
    }

    while (1)
    {
        if (ui4_pos >= ui4_data_2)
        {
            pt_private->aui1_text_buf[ui1_idx] = 0;
            x_uc_ps_to_w2s((const CHAR *)pt_private->aui1_text_buf,
                           pt_private->w2s_text,
                           256);
            return MMSBTLR_OK;
        }

        ui1_tmp = pt_private->aui1_block_buf[ui4_pos++];
        
        if (b_in_brace)
        {
            if (ui1_tmp == '}')
            {
                b_in_brace= FALSE;
            }
            /* skip everything inside braces.*/
        }
        else if (ui1_tmp == '{')
        {
            b_in_brace = TRUE;
        }
        else if ((ui1_tmp == CARRIAGE_RETURN) || (ui1_tmp == LINE_FEED))
        {
            /*do nothing with this case*/
        }
        else if(ui1_tmp == '\\')
        {
            ui1_tmp = pt_private->aui1_block_buf[ui4_pos++];
            
            if ((ui1_tmp == 'N') || (ui1_tmp == 'n'))
            {
                pt_private->aui1_text_buf[ui1_idx] = LINE_FEED;
                ui1_idx++;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = (UINT8)'\\';
                ui1_idx++;
                pt_private->aui1_text_buf[ui1_idx] = ui1_tmp;
                ui1_idx++;
            }
            /* change this content into CARRIAGE RETURN + LINE_FEED*/
        }
        else
        {
            pt_private->aui1_text_buf[ui1_idx] = ui1_tmp;
            ui1_idx++;
        }
    }
    
    /*return MMSBTLR_OK;*/
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
UINT32 mkv_ssa_timer_period(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    ASSERT(pt_mngr->pt_engine->t_cfg.i4_speed > 0, ("pt_mngr->pt_engine.t_cfg.i4_speed=0\n"));

    return (UINT32)((TIMER_PERIOD * 100)/pt_mngr->pt_engine->t_cfg.i4_speed);
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
INT32 mkv_ssa_init(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
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
INT32 mkv_ssa_deinit(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    pt_private = (SBTL_MKV_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->pv_data)
    {
        x_mem_free(pt_private->pv_data);
        pt_private->pv_data = NULL;
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
VOID mkv_ssa_start(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
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
VOID mkv_ssa_stop(SBTL_PAGE_INST_MNGR_T*       pt_mngr)
{
}

