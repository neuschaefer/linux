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
 * $RCSfile: sbtl_mplayer2_parser.c,v $
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
#include "mutil/sbtl_engine/sbtl_text/sbtl_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_mplayer2_parser.h"



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

#include "sbtl_text_page_inst_mngr.h"
#include "sbtl_mplayer2_parser.h"
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

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/

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
static VOID _mplayer2_escape_useless(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    UINT8                   ui1_tmp;
    INT32                   i4_ret;
    SBTL_ENGINE_T*          pt_engine;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT64                           ui8_offset;
    UINT16                          ui2_tmp;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    while(1)
    {
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            ui8_offset = -2;
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
            ui8_offset = -1;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if(!((ui2_tmp == CARRIAGE_RETURN) || (ui2_tmp == LINE_FEED) || (ui2_tmp == ' ') || (ui2_tmp == CHAR_TAB)))
        {
            sbtl_feeder_setpos(pt_engine, ui8_offset, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            break;
        }
    }

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
static INT32 _mplayer2_get_number(SBTL_ENGINE_T*    pt_engine,
                                  BOOL              b_UTF16,
                                  UINT32*           pui4_number)
{
    UINT8                           ui1_tmp;
    UINT8                           ui1_idx;
    UINT32                          ui4_number;
    INT32                           i4_ret;
    UINT16                          ui2_tmp;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_idx = 0;
    ui4_number = 0;
    
    do
    {
        if (b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
    
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }

        if (ui2_tmp>='0' && ui2_tmp<= '9')
        {
            ui4_number = ui4_number * 10 + (ui2_tmp -'0');
        }
        else if (0 == ui1_idx)
        {
            return MMSBTLR_NOT_FOUND;
        }
        else
        {
            break;
        }
        
        ui1_idx++;
    }while(1);
    
    *pui4_number = ui4_number;

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
static INT32 _mplayer2_format(SBTL_ENGINE_T*        pt_engine,
                              BOOL                  b_UTF16,
                              BOOL*                 pb_found,
                              UINT32*               pui4_time)
{
    UINT8               ui1_tmp;
    UINT32              ui4_time;
    INT32               i4_ret;
    BOOL                b_asci;
    UINT16                          ui2_tmp;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    b_asci = TRUE;
    ui4_time = 0;
    
    if (b_UTF16)
    {
        i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
    }
    else
    {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
        ui2_tmp = ui1_tmp;
    }
    
    if (i4_ret != MMSBTLR_OK)
    {
        *pb_found = FALSE;
        return i4_ret;
    }

    if (ui2_tmp == '[')
    {
        /* hit the first '[', continue search the */

        i4_ret = _mplayer2_get_number(pt_engine, b_UTF16,&ui4_time);

        if ((i4_ret!=MMSBTLR_OK) && (i4_ret!=MMSBTLR_NOT_FOUND))
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        if (MMSBTLR_NOT_FOUND == i4_ret)
        {
            b_asci = FALSE;
        }

        if (b_UTF16)
        {
            sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        i4_ret = sbtl_feeder_inpnbyte(pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if((ui2_tmp == ']'))
        {
            if (!b_asci) /* LG: case [timeA][] subtitle, no ascii hit */
            {
                *pui4_time += 20; /* 20 from "Subtitle workshop" */
            }
            else
            {
                *pui4_time = ui4_time;
            }

            *pb_found = TRUE;
            return MMSBTLR_OK;
        }
    }

    *pb_found = FALSE;
    return MMSBTLR_OK;
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
INT32 mplayer2_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*       pt_mngr,
                               PTS_T                        t_pts_current,
                               PTS_T*                       pt_pts_start,
                               PTS_T*                       pt_pts_end,
                               BOOL*                        pb_found)
{
    INT32                               i4_ret;
    BOOL                                b_found;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    UINT32*                             pui4_time;
    UINT64                              ui8_offset;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pui4_time = (UINT32*)&pt_private->pv_data;
    
    while(1)
    {

        sbtl_feeder_getpos(pt_mngr->pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
        
        i4_ret = _mplayer2_format(pt_mngr->pt_engine,
                                  pt_private->b_UTF16,
                                  &b_found,
                                  pui4_time);

        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found  = FALSE;
            return i4_ret;
        }

        if (b_found)
        {
            *pt_pts_start = (UINT64)_mm_div64(PTS_FREQ*(UINT64)(*pui4_time), 10, NULL);
            _mplayer2_escape_useless(pt_mngr);

            i4_ret = _mplayer2_format(pt_mngr->pt_engine,
                                      pt_private->b_UTF16,
                                      &b_found,
                                      pui4_time);
    
            if (i4_ret != MMSBTLR_OK)
            {
                *pb_found  = FALSE;
                return i4_ret;
            }

            if (b_found)
            {
                *pt_pts_end = (UINT64)_mm_div64(PTS_FREQ*(UINT64)(*pui4_time), 10, NULL);

                if (t_pts_current<*pt_pts_end)
                {
                    *pb_found = TRUE;
                    return MMSBTLR_OK;
                }
            }
        }

        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)(ui8_offset+2), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)(ui8_offset+1), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }            

        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found  = FALSE;
            return i4_ret;
        }
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
INT32 mplayer2_load_string(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    INT32                               i4_ret;
    UINT8                               ui1_tmp;
    UINT8                               ui1_idx;
    BOOL                                b_in_brace;
    UINT16                              ui2_tmp;
    MM_SBTL_ENCODING_T                  e_encoding;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    _mplayer2_escape_useless(pt_mngr);

    ui1_idx = 0;
    b_in_brace = FALSE;
    
    while (1)
    {
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }

        if (i4_ret != MMSBTLR_OK)
        {
            /*return i4_ret;*/
            if (pt_private->b_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = 0;
            }
            break;
        }

#ifdef MORE_CHANGE_LINE_SYMBOL
        if ((ui2_tmp == CARRIAGE_RETURN) || (ui2_tmp == LINE_FEED))
#else
        if (ui2_tmp == CARRIAGE_RETURN)
#endif
        {
            if (pt_private->b_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = 0;
            }
            break;
        }



        if(b_in_brace)
        {
            if((ui2_tmp == ']'))
            {
                b_in_brace= FALSE;
            }
            /* skip everything inside braces. */
        }
        else if((ui2_tmp == '['))
        {
            b_in_brace = TRUE;
        }
        else if (ui2_tmp == '|')
        {
            if (pt_private->b_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = LINE_FEED;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = LINE_FEED;
            }
            /* change this content into LINE_FEED*/
        }
        else
        {
            /* fill the byte into share memory directly */
            if (pt_private->b_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = ui2_tmp;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = ui1_tmp;
            }
            //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui1_idx]);
        }

        ui1_idx++;
    }
    
    if(e_encoding != MM_SBTL_ENCODE_UTF16)
    {
        sbtl_trans_to_w2s_ex(e_encoding,pt_private->aui1_text_buf,MM_SBTL_CACHE_MAX,pt_private->w2s_text);
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
UINT32 mplayer2_timer_period(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
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
INT32 mplayer2_init(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
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
INT32 mplayer2_deinit(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
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
VOID mplayer2_start(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_private->pv_data = 0;
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
VOID mplayer2_stop(SBTL_PAGE_INST_MNGR_T*       pt_mngr)
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
/*
[810][821]My dear...
*/
BOOL mplayer2_parser_verify(SBTL_ENGINE_T*     pt_engine)
{
    UINT8   ui1_count = 0;
    BOOL    b_found;
    UINT32  ui4_time;
    INT32   i4_ret;
    UINT64  ui8_offset;
    BOOL    b_UTF16;
    UINT8   ui1_tmp;
        
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    b_UTF16 = FALSE;
    
    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    sbtl_feeder_inpnbyte(pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
    
    if (0xFF == ui1_tmp)
    {
        sbtl_feeder_inpnbyte(pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        if (0xFE == ui1_tmp)
        {
            b_UTF16 = TRUE;
        }
    }
    
    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    
    while(1)
    {
        i4_ret = _mplayer2_format(pt_engine, b_UTF16, &b_found, &ui4_time);
        if (i4_ret != MMSBTLR_OK)
        {
            return FALSE;
        }

        if (b_found)
        {
            ui1_count++;
            if(ui1_count == 2)
            {
                return TRUE;
            }
        }
        else
        {
            sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
            
            if(ui8_offset > 256)
            {
                return FALSE;
            }
        }
    }
}


