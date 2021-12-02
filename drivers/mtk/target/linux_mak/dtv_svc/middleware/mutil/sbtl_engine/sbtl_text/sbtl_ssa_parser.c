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
 * $RCSfile: sbtl_ssa_parser.c,v $
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
#include "mutil/sbtl_engine/sbtl_text/sbtl_ssa_parser.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"



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
#include "sbtl_ssa_parser.h"
#include "u_mm_sbtl_engine.h"

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
static CHAR s_dialogue_header[] = "Dialogue";
static CHAR s_events_header[] = "[Events]";
static CHAR s_format_header[] = "Format";

static CHAR s_text_header[] = "Text";
static CHAR s_start_header[] = "Start";
static CHAR s_end_header[] = "End";

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
static INT32 _ssa_search_string (SBTL_ENGINE_T*     pt_engine, 
                                 BOOL               b_UTF16,
                                 CHAR*              s_string,
                                 BOOL*              pb_found,
                                 UINT8              ui1_whence,
                                 BOOL               b_back,
                                 UINT64*            pui8_offset)
{
    UINT64      ui8_offset;
    UINT64      ui8_found_offset;
    UINT8       ui1_len;
    UINT8       ui1_idx;
    UINT8       ui1_tmp;
    UINT16      ui2_tmp;
    INT32       i4_ret;
    UINT32      ui4_done_bytes;
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    
    ui1_len = (UINT8)x_strlen((const CHAR *)s_string);
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

    if (ui1_whence == SBTL_SEARCH_BEGIN)
    {
        for (ui1_idx=0; ui1_idx<ui1_len; ui1_idx++)
        {
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }
            if (ui2_tmp != (UINT8)s_string[ui1_idx])
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;

                return MMSBTLR_NOT_FOUND;
            }
        }
        
        if (pui8_offset)
        {
            sbtl_feeder_getpos(pt_engine, &ui8_found_offset, MM_SBTL_FEEDER_SUBTITLE);
            if (b_UTF16)
            {
                *pui8_offset = ui8_found_offset - 2*ui1_len;
            }
            else
            {
                *pui8_offset = ui8_found_offset - ui1_len;
            }
        }

        if (b_back)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        *pb_found = TRUE;
        return MMSBTLR_OK;

    }
    else if (ui1_whence == SBTL_SEARCH_MIDDLE)
    {
        do
        {
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }

            if (ui2_tmp == (UINT8)s_string[0])
            {
                i4_ret = _ssa_search_string(pt_engine, 
                                            b_UTF16,
                                            &s_string[1],
                                            pb_found,
                                            SBTL_SEARCH_BEGIN,
                                            FALSE,
                                            &ui8_found_offset);
                
                if (i4_ret!=MMSBTLR_OK && i4_ret!=MMSBTLR_NOT_FOUND)
                {
                    sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    *pb_found = FALSE;
                    return i4_ret;
                }

                if (*pb_found == TRUE)
                {   
                    if (pui8_offset)
                    {
                        if (b_UTF16)
                        {
                            *pui8_offset = ui8_found_offset - 2;
                        }
                        else
                        {
                            *pui8_offset = ui8_found_offset-1;
                        }
                    }
                    if (b_back)
                    {
                        sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    }
                    return MMSBTLR_OK;
                }
            }
        }while(1);
    }
    else
    {
        *pb_found = FALSE;
        return MMSBTLR_NOT_SUPPORT;
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
static INT32 _ssa_search_string_p (SBTL_ENGINE_T*     pt_engine, 
                                        BOOL               b_UTF16,
                                       CHAR*              s_string,
                                       BOOL*              pb_found,
                                       UINT8              ui1_whence,
                                       BOOL               b_back,
                                       UINT64*            pui8_offset,
                                       UINT64             ui8_parse_size)
{
    UINT64      ui8_offset;
    UINT64      ui8_offset1;
    UINT64      ui8_found_offset;
    UINT8       ui1_len;
    UINT8       ui1_idx;
    UINT8       ui1_tmp;
    UINT16      ui2_tmp;
    INT32       i4_ret;
    UINT32      ui4_done_bytes;
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    
    ui1_len = (UINT8)x_strlen((const CHAR *)s_string);
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
    if(ui8_offset > ui8_parse_size)
    {
        *pb_found = FALSE;
        return MMSBTLR_OK;
    }

    if (ui1_whence == SBTL_SEARCH_BEGIN)
    {
        for (ui1_idx=0; ui1_idx<ui1_len; ui1_idx++)
        {
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }
            if (ui2_tmp != (UINT8)s_string[ui1_idx])
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;

                return MMSBTLR_NOT_FOUND;
            }
        }
        
        if (pui8_offset)
        {
            sbtl_feeder_getpos(pt_engine, &ui8_found_offset, MM_SBTL_FEEDER_SUBTITLE);
            if (b_UTF16)
            {
                *pui8_offset = ui8_found_offset - 2*ui1_len;
            }
            else
            {
                *pui8_offset = ui8_found_offset - ui1_len;
            }
        }

        if (b_back)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        *pb_found = TRUE;
        return MMSBTLR_OK;

    }
    else if (ui1_whence == SBTL_SEARCH_MIDDLE)
    {
        do
        {
            sbtl_feeder_getpos(pt_engine, &ui8_offset1, MM_SBTL_FEEDER_SUBTITLE);
            if(ui8_offset1 > ui8_parse_size)
            {
                *pb_found = FALSE;
                return MMSBTLR_OK;
            }
            
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }

            if (ui2_tmp == (UINT8)s_string[0])
            {
                i4_ret = _ssa_search_string_p(pt_engine, 
                                            b_UTF16,
                                            &s_string[1],
                                            pb_found,
                                            SBTL_SEARCH_BEGIN,
                                            FALSE,
                                            &ui8_found_offset,
                                            ui8_parse_size);
                
                if (i4_ret!=MMSBTLR_OK && i4_ret!=MMSBTLR_NOT_FOUND)
                {
                    sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    *pb_found = FALSE;
                    return i4_ret;
                }

                if (*pb_found == TRUE)
                {   
                    if (pui8_offset)
                    {
                        if (b_UTF16)
                        {
                            *pui8_offset = ui8_found_offset - 2;
                        }
                        else
                        {
                            *pui8_offset = ui8_found_offset-1;
                        }
                    }
                    if (b_back)
                    {
                        sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    }
                    return MMSBTLR_OK;
                }
            }
        }while(1);
    }
    else
    {
        *pb_found = FALSE;
        return MMSBTLR_NOT_SUPPORT;
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
static VOID _ssa_escape_useless(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    UINT8                   ui1_tmp;
    UINT8                               ui2_tmp;
    INT32                   i4_ret;
    UINT32                              ui4_done_bytes;
    SBTL_ENGINE_T*          pt_engine;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    while(1)
    {
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if(!((ui2_tmp == CARRIAGE_RETURN) || (ui2_tmp == LINE_FEED) || (ui2_tmp == ' ') || (ui2_tmp == CHAR_TAB)))
        {
            if (pt_private->b_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
        {
            sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
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
static INT32 _ssa_get_number(SBTL_ENGINE_T*     pt_engine,
                             BOOL               b_UTF16,
                             UINT32*            pui4_number)
{
    UINT8               ui1_tmp;
    UINT16              ui2_tmp;
    UINT8               ui1_idx;
    UINT32              ui4_number;
    INT32               i4_ret;
    UINT32              ui4_done_bytes;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_idx = 0;
    ui4_number = 0;
    
    do
    {
        if (b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
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
            ui4_number = ui4_number * 10 + (ui2_tmp-'0');
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
static INT32 _ssa_get_stc(SBTL_ENGINE_T*     pt_engine,
                          BOOL               b_UTF16,
                          UINT64*            pui8_time)
{
    INT32   i4_ret;
    UINT32  ui4_num;
    UINT32  ui4_time;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui4_time = 0;
    
    i4_ret = _ssa_get_number(pt_engine, b_UTF16, &ui4_num);
    
    if (i4_ret!=MMSBTLR_OK)
    {
        *pui8_time = 0;
        return i4_ret;
    }

    ui4_time += ui4_num*3600*1000;

    i4_ret = _ssa_get_number(pt_engine, b_UTF16, &ui4_num);
    
    if (i4_ret!=MMSBTLR_OK)
    {
        *pui8_time = 0;
        return i4_ret;
    }

    ui4_time += ui4_num*60*1000;

    i4_ret = _ssa_get_number(pt_engine, b_UTF16, &ui4_num);
    
    if (i4_ret!=MMSBTLR_OK)
    {
        *pui8_time = 0;
        return i4_ret;
    }

    ui4_time += ui4_num*1000;

    i4_ret = _ssa_get_number(pt_engine, b_UTF16, &ui4_num);
    
    if (i4_ret!=MMSBTLR_OK)
    {
        *pui8_time = 0;
        return i4_ret;
    }

    ui4_time += ui4_num*10;

    *pui8_time = (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_time, 1000, NULL);
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
static INT32 _ssa_count_commas(SBTL_ENGINE_T*   pt_engine,
                               BOOL             b_UTF16,
                               UINT8            ui1_comma_num)
{
    UINT8   ui1_count;
    UINT8   ui1_tmp;
    UINT16  ui2_tmp;
    INT32   i4_ret;
    UINT32  ui4_done_bytes;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_count = 0;
    
    while(1)
    {
        if (b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
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

        if(ui2_tmp == ',')
        {
            ui1_count++;
            if(ui1_count >= ui1_comma_num)
            {
                return MMSBTLR_OK;
            }
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
static INT32 _ssa_look4comm(SBTL_ENGINE_T*      pt_engine,
                            BOOL                b_UTF16,
                            CHAR*               s_target,
                            UINT8               ui1_target_len,
                            UINT8               ui1_look,
                            UINT8*              pui1_count)
{
    UINT8   ui1_idx;
    UINT8   ui1_num;
    UINT8   ui1_tmp;
    UINT16  ui2_tmp;
    INT32   i4_ret;
    UINT32  ui4_done_bytes;
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_idx = 0;
    ui1_num = 0;
    
    while(1)
    {
        if (b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
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

        if (ui1_idx >= ui1_target_len)
        {
            return MMSBTLR_NOT_FOUND;
        }
        
        if (ui2_tmp == (UINT8)s_target[ui1_idx])
        {
            ui1_idx++;
        }
        else
        {
            ui1_idx = 0;
        }

        if (ui1_idx >= ui1_target_len)
        {
            *pui1_count = ui1_num;
            return MMSBTLR_OK;
        }

        if(ui2_tmp == ui1_look)
        {
            ui1_num++;
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
static VOID _ssa_format(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    BOOL                                b_found;
    INT32                               i4_ret;
    SSA_PRIVATE_T*                      pt_ssa_private;
    UINT64                              ui1_offset;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    pt_ssa_private = (SSA_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_ssa_private, ("pt_ssa_private=NULL\n"));

    sbtl_feeder_getpos(pt_mngr->pt_engine, &ui1_offset, MM_SBTL_FEEDER_SUBTITLE);
    
    i4_ret = _ssa_search_string(pt_mngr->pt_engine, 
                                pt_private->b_UTF16,
                                 s_format_header,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 FALSE,
                                 NULL);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
    }
    else
    {
        /*
        Use default value
        From Spec: The field names must be spelled correctly, and are as follows:
        Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
        */
        pt_ssa_private->ui1_start_offset = 1;
        pt_ssa_private->ui1_end_offset = 2;
        pt_ssa_private->ui1_text_offset = 9;

        sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)ui1_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        return;
    }

    /*looking for the Start and count the number of ','*/
    i4_ret = _ssa_look4comm(pt_mngr->pt_engine,
                            pt_private->b_UTF16,
                            s_start_header,
                            5,
                            (UINT8)',',
                            &pt_ssa_private->ui1_start_offset);

    if (i4_ret != MMSBTLR_OK)
    {
        return;
    }
    

    /*looking for the End and count the number of ','*/
    i4_ret = _ssa_look4comm(pt_mngr->pt_engine,
                            pt_private->b_UTF16,
                            s_end_header,
                            3,
                            (UINT8)',',
                            &pt_ssa_private->ui1_end_offset);

    if (i4_ret != MMSBTLR_OK)
    {
        return;
    }

    pt_ssa_private->ui1_end_offset += pt_ssa_private->ui1_start_offset;
    
    /*looking for the End and count the number of ','*/
    i4_ret = _ssa_look4comm(pt_mngr->pt_engine,
                            pt_private->b_UTF16,
                            s_text_header,
                            4,
                            (UINT8)',',
                            &pt_ssa_private->ui1_text_offset);

    if (i4_ret != MMSBTLR_OK)
    {
        return;
    }

    pt_ssa_private->ui1_text_offset += pt_ssa_private->ui1_end_offset;
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
INT32 ssa_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                          PTS_T                         t_pts_current,
                          PTS_T*                        pt_pts_start,
                          PTS_T*                        pt_pts_end,
                          BOOL*                         pb_found)
{
    INT32                               i4_ret;
    BOOL                                b_found;
    SSA_PRIVATE_T*                      pt_ssa_private;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    UINT64                              ui1_offset;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    pt_ssa_private = (SSA_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_ssa_private, ("pt_ssa_private=NULL\n"));

    while (1)
    {
        i4_ret = _ssa_search_string(pt_mngr->pt_engine, 
                                    pt_private->b_UTF16,
                                     s_dialogue_header,
                                     &b_found,
                                     SBTL_SEARCH_MIDDLE,
                                     FALSE,
                                     NULL);
        
        if (MMSBTLR_OK==i4_ret && b_found)
        {
        }
        else
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        sbtl_feeder_getpos(pt_mngr->pt_engine, &ui1_offset, MM_SBTL_FEEDER_SUBTITLE);
        
        i4_ret = _ssa_count_commas(pt_mngr->pt_engine, 
                                   pt_private->b_UTF16, 
                                   pt_ssa_private->ui1_start_offset);

        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        _ssa_escape_useless(pt_mngr);

        i4_ret = _ssa_get_stc(pt_mngr->pt_engine, pt_private->b_UTF16, pt_pts_start);

        if (i4_ret!=MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)ui1_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        
        i4_ret = _ssa_count_commas(pt_mngr->pt_engine, 
                                   pt_private->b_UTF16,
                                   pt_ssa_private->ui1_end_offset);

        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        _ssa_escape_useless(pt_mngr);

        i4_ret = _ssa_get_stc(pt_mngr->pt_engine, pt_private->b_UTF16, pt_pts_end);
        if (i4_ret!=MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)ui1_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        if (t_pts_current<*pt_pts_end)
        {
            *pb_found = TRUE;
            return MMSBTLR_OK;
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
INT32 ssa_load_string(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    UINT16                             ui2_idx;
    UINT8                               ui1_tmp = 0;
    UINT16                              ui2_tmp;
    BOOL                                b_found;
    BOOL                                b_in_brace;
    INT32                               i4_ret;
    UINT32                              ui4_done_bytes;
    SSA_PRIVATE_T*                      pt_ssa_private;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    MM_SBTL_ENCODING_T                  e_encoding;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    pt_ssa_private = (SSA_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_ssa_private, ("pt_ssa_private=NULL\n"));
    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;

    ui2_idx= 0;
    b_in_brace = FALSE;
    
    i4_ret = _ssa_count_commas(pt_mngr->pt_engine, 
                               pt_private->b_UTF16,
                               pt_ssa_private->ui1_text_offset);

    if (i4_ret != MMSBTLR_OK)
    {
        return i4_ret;
    }

    while (1)
    {
        if(ui2_idx >= (UINT16)(MM_SBTL_CACHE_MAX - 2))
        {
            ui2_idx = (UINT16)(MM_SBTL_CACHE_MAX -2);
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
            }
            //pt_private->w2s_text[ui2_idx] = 0;
            break;
        }
        
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
            }
            //pt_private->w2s_text[ui2_idx] = 0;
            break;
        }
        
        if (b_in_brace)
        {
            if (ui2_tmp == '}')
            {
                b_in_brace= FALSE;
            }
            /* skip everything inside braces.*/
        }
        else if (ui2_tmp == '{')
        {
            b_in_brace = TRUE;
        }
        else if (ui2_tmp == 'D')
        {
            if (pt_private->b_UTF16)
            {
                sbtl_feeder_setpos(pt_mngr->pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
        {
            sbtl_feeder_setpos(pt_mngr->pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }

            i4_ret = _ssa_search_string(pt_mngr->pt_engine, 
                                        pt_private->b_UTF16,
                                         s_dialogue_header,
                                         &b_found,
                                         SBTL_SEARCH_BEGIN,
                                         TRUE,
                                         NULL);
            
            if (MMSBTLR_OK==i4_ret && b_found)
            {
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = 0;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = 0;
                }
                //pt_private->w2s_text[ui2_idx] = 0;
                break;
            }
            else if(MMSBTLR_EOT == i4_ret)
            {
                //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui2_idx]);
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = ui2_tmp;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = ui1_tmp;
                }
                    
                ui2_idx++;
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = 0;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = 0;
                }
                //pt_private->w2s_text[ui2_idx] = 0;

                break;
            }
            else
            {
                if (pt_private->b_UTF16)
                {
                    sbtl_feeder_setpos(pt_mngr->pt_engine, 2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
                }
                else
                {
                sbtl_feeder_setpos(pt_mngr->pt_engine, 1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
                }

                //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui2_idx]);
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = ui2_tmp;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = ui1_tmp;
                }
                
                ui2_idx++;
            }
        }
        else if ((ui2_tmp == CARRIAGE_RETURN) || (ui2_tmp == LINE_FEED))
        {
            /*do nothing with this case*/
        }
        else if(ui2_tmp == '\\')
        {
            if (pt_private->b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&ui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                break;
            }

            if ((ui2_tmp == 'N') || (ui2_tmp == 'n'))
            {
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = LINE_FEED;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = LINE_FEED;
                }
                //pt_private->w2s_text[ui2_idx] = LINE_FEED;
                ui2_idx++;
            }
            else
            {
                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = (UINT8)'\\';
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = (UINT8)'\\';
                }
                //pt_private->w2s_text[ui2_idx] = (UINT8)'\\';
                ui2_idx++;

                if (e_encoding == MM_SBTL_ENCODE_UTF16)
                {
                    pt_private->w2s_text[ui2_idx] = ui2_tmp;
                }
                else
                {
                    pt_private->aui1_text_buf[ui2_idx] = ui1_tmp;
                }
                //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui2_idx]);
                
                ui2_idx++;
            }
            /* change this content into CARRIAGE RETURN + LINE_FEED*/
        }
        else
        {
            
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = ui2_tmp;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = ui1_tmp;
            }
            
            //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui2_idx]);

            ui2_idx++;
        }
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
UINT32 ssa_timer_period(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
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
INT32 ssa_init(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    BOOL                                b_found;
    INT32                               i4_ret;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    pt_private->pv_data = x_mem_alloc(sizeof(SSA_PRIVATE_T));
    
    if (NULL == pt_private->pv_data)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }

    sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);

    i4_ret = _ssa_search_string(pt_mngr->pt_engine, 
                                pt_private->b_UTF16,
                                 s_events_header,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 FALSE,
                                 NULL);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
    }
    else
    {
        return MMSBTLR_NOT_SUPPORT;
    }

    _ssa_format(pt_mngr);
    
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
INT32 ssa_deinit(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

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
VOID ssa_start(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
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
VOID ssa_stop(SBTL_PAGE_INST_MNGR_T*       pt_mngr)
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
Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, TertiaryColour, BackColour, Bold, Italic, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, AlphaLevel, Encoding
Style: Default,Arial,20,65535,65535,65535,-2147483640,-1,0,1,3,0,2,30,30,30,0,0
Style: Titre_episode,Akbar,140,15724527,65535,65535,986895,-1,0,1,1,0,3,30,30,30,0,0
Style: Wolf main,Wolf_Rain,56,15724527,15724527,15724527,4144959,0,0,1,1,2,2,5,5,30,0,0

[Events]
Format: Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
Dialogue: Marked=0,0:02:40.65,0:02:41.79,Wolf main,Cher,0000,0000,0000,,Et les enregistrements de ses ondes delta ?
Dialogue: Marked=0,0:02:42.42,0:02:44.15,Wolf main,autre,0000,0000,0000,,Toujours rien.
*/
BOOL ssa_parser_verify(SBTL_ENGINE_T*   pt_engine)
{
    BOOL    b_found;
    INT32   i4_ret;
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

    i4_ret = _ssa_search_string_p(pt_engine, 
                                b_UTF16,
                                 s_dialogue_header,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 TRUE,
                                 NULL,
                                 MM_SBTL_PARSE_MAX_LENGTH);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;

}


