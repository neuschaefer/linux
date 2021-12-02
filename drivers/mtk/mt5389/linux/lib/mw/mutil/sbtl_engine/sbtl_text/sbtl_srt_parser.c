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
 * $RCSfile: sbtl_srt_parser.c,v $
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
#include "mutil/sbtl_engine/sbtl_text/sbtl_srt_parser.h"



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
#include "sbtl_srt_parser.h"
#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
INT32 _srt_check_time_format(SBTL_ENGINE_T*   pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_found_total,
    UINT16*                         pui2_count,
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private,
    BOOL                            b_UTF16);

static INT32 _srt_creat_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

INT32 _srt_alloc_time_stamp(
        SBTL_TEXT_PAGE_MNGR_PRIVATE_T*             pt_private,
        UINT16                                    ui2_time_stamp_cnt);

INT32 _srt_time_stamp_sort(
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*         pt_private,
    UINT16                              ui2_order_start,
    UINT16                              ui2_order_end);



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
INT32 srt_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                          PTS_T                     t_pts_current,
                          PTS_T*                    pt_pts_start,
                          PTS_T*                    pt_pts_end,
                          BOOL*                     pb_found)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    UINT16                           ui2_time_idx;
    UINT16                           ui2_idx;
    UINT16                           ui2_cur_idx;
    UINT16                           ui2_order_start;
    UINT16                           ui2_order_end;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if((pt_private->pt_time_stamp == NULL) || 
       (pt_private->pui2_time_order == NULL))
    {
        return MMSBTLR_INV_ARG;
    }


    if (pt_private->ui2_cur_idx == SUBTITLE_IDX_NULL)
    {
        ui2_cur_idx = 0;
    }
    else if(pt_private->ui2_cur_idx >= pt_private->ui2_time_max_cnt)
    {
        ui2_cur_idx = (UINT16)(pt_private->ui2_time_max_cnt - 1);
    }
    else
    {
        ui2_cur_idx = pt_private->ui2_cur_idx;
    }
    
    ui2_time_idx = pt_private->pui2_time_order[ui2_cur_idx];

    if(t_pts_current < pt_private->pt_time_stamp[ui2_time_idx].t_pts_s)
    {
        ui2_order_start = 0;
        ui2_order_end = ui2_cur_idx;
    }
    else
    {
        ui2_order_start = ui2_cur_idx;
        ui2_order_end = (UINT16)(pt_private->ui2_time_max_cnt -1) ;
    }

    for(ui2_idx = ui2_order_start; ui2_idx <= ui2_order_end; ui2_idx ++)
    {
        ui2_time_idx = pt_private->pui2_time_order[ui2_idx];
        if(t_pts_current < pt_private->pt_time_stamp[ui2_time_idx].t_pts_e)
        {
            *pb_found = TRUE;
            break;
        }
    }

    if(*pb_found == TRUE)
    {
         pt_private->ui2_cur_idx = ui2_idx;

         ui2_time_idx = pt_private->pui2_time_order[pt_private->ui2_cur_idx];
         *pt_pts_start = pt_private->pt_time_stamp[ui2_time_idx].t_pts_s;
         *pt_pts_end= pt_private->pt_time_stamp[ui2_time_idx].t_pts_e;

         return MMSBTLR_OK;
        
    }
    else
    {
        return MMSBTLR_EOT;
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
INT32 srt_load_string(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    UINT8                           au1MatchCode_dos[4] = {0x0D, 0x0A, 0x0D, 0x0A};
    UINT8                           au1MatchCode_linux[2] = {0x0A, 0x0A};
    UINT16                          ui2_idx = 0;
    UINT8                           ui1_idx2 = 0;
    UINT8                           ui1_idx3 = 0;
    UINT16                          ui2_tmp;
    INT32                           i4_ret;
    BOOL                            b_in_agl_brkt = FALSE;
    UINT32                          ui4_done_bytes;
    MM_SBTL_ENCODING_T              e_encoding;
    UINT16                           ui2_time_idx;
    UINT16                           ui2_cur_idx;
    
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;
    x_memset(pt_private->aui1_text_buf,0,sizeof(UINT8)*MM_SBTL_CACHE_MAX);
    x_memset(pt_private->w2s_text,0,sizeof(UINT16)*MM_SBTL_CACHE_MAX);

    ui2_cur_idx = pt_private->ui2_cur_idx;
    ui2_time_idx = pt_private->pui2_time_order[ui2_cur_idx];

    sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)pt_private->pt_time_stamp[ui2_time_idx].ui4_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    
    while (1)
    {
        if(ui2_idx >= MM_SBTL_CACHE_MAX)
        {
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx-1] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx-1] = 0;
            }
            break;
        }
        if (e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&pt_private->w2s_text[ui2_idx], &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &pt_private->aui1_text_buf[ui2_idx], MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = pt_private->aui1_text_buf[ui2_idx];
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;;
        }
        
        if (e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            sbtl_trans_to_w2s(MM_SBTL_LANG_UNICODE, pt_private->w2s_text[ui2_idx], &ui2_tmp);
        }
        else
        {
            sbtl_trans_to_w2s(MM_SBTL_LANG_ASCI, pt_private->aui1_text_buf[ui2_idx], &ui2_tmp);
        }
        
        ui2_idx++;
        
        if (ui2_tmp == 0x0D)
        {
            ui2_idx -= 1;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
            }
        }
        
        if (ui2_tmp == 0x0A && ui2_idx == 1)
        {
            ui2_idx -= 1;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
            }
        }

        if (ui2_tmp == au1MatchCode_dos[ui1_idx2])
        {
            ui1_idx2++;
        }
        else if (ui2_tmp == ' ')
        {
        }
        else
        {
            ui1_idx2 = 0;
        }
        
        if(ui2_tmp == au1MatchCode_linux[ui1_idx3])
        {
            ui1_idx3++;
        }
        else if (ui2_tmp == ' ')
        {
        }
        else
        {
            ui1_idx3 = 0;
        }

        if ((ui1_idx2 >= 4) && (ui2_idx >= 2))
        {
            ui2_idx -= 2;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
                pt_private->w2s_text[ui2_idx+1] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
                pt_private->aui1_text_buf[ui2_idx+1] = 0;
            }
            break;
        }

        if ((ui1_idx3 >= 2) && (ui2_idx >= 2))
        {
            ui2_idx -= 2;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui2_idx] = 0;
                pt_private->w2s_text[ui2_idx+1] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;
                pt_private->aui1_text_buf[ui2_idx+1] = 0;
            }
            break;
        }
        
        if (ui2_tmp == '<')
        {
            b_in_agl_brkt = TRUE;
        }
        
        if (b_in_agl_brkt)
        {
            ui2_idx -= 1;
            switch (ui2_tmp)
            {
            case 'u':
                SBTL_SET_FONT_STYLE(pt_mngr->pt_engine,FE_FNT_STYLE_UNDERLINE);
                SBTL_SET_FONT_STYLE_FLAG(pt_mngr->pt_engine, TRUE);
                break;

            case 'i':
                SBTL_SET_FONT_STYLE(pt_mngr->pt_engine,FE_FNT_STYLE_ITALIC);
                SBTL_SET_FONT_STYLE_FLAG(pt_mngr->pt_engine, TRUE);
                break;

            default:
                break;
            }

            if (ui2_tmp == '>')
            {
                b_in_agl_brkt = FALSE;
            }
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
UINT32 srt_timer_period(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
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
INT32 srt_init(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    UINT16                            ui2_found_cnt;
       
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    _srt_check_time_format(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE, TRUE, &ui2_found_cnt, NULL, pt_private->b_UTF16);
    _srt_alloc_time_stamp(pt_private, ui2_found_cnt);
    _srt_creat_time_stamp(pt_mngr);
    _srt_time_stamp_sort(pt_private, 0, ui2_found_cnt);
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
INT32 srt_deinit(SBTL_PAGE_INST_MNGR_T* pt_mngr)
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
VOID srt_start(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
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
VOID srt_stop(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
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
00:00:05,892 --> 00:00:10,730
*/
BOOL srt_parser_verify(SBTL_ENGINE_T*   pt_engine)
{
    INT32   i4_ret;
    UINT16  ui1_idx;
    UINT16  ui2_found_cnt = 0;
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
    
    ui1_idx = 0;
    
    for(ui1_idx=0; ui1_idx<256; ui1_idx++)
    {
        if(b_UTF16)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui1_idx*2, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui1_idx, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
      i4_ret = _srt_check_time_format(pt_engine, MM_SBTL_FEEDER_SUBTITLE, FALSE, &ui2_found_cnt, NULL, b_UTF16);  

      if(i4_ret == MMSBTLR_OK && ui2_found_cnt == 1)
      {
          return TRUE;
      }
    }
    
    return FALSE;
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

INT32 _srt_check_time_format(SBTL_ENGINE_T*   pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_found_total,
    UINT16*                         pui2_count,
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private,
    BOOL                            b_UTF16)
{
    UINT8                           ui1_idx = 0;
    UINT8                           ui1_idx1 = 0;
    UINT8                           aui1_tmp[12];
    UINT16                          aui2_tmp[12];
    INT32                           i4_ret;
    UINT32                          ui4_hour;
    UINT32                          ui4_min;
    UINT32                          ui4_sec;
    UINT32                          ui4_ms;
    UINT16                          ui2_num = 0;
    UINT32                          ui4_done_bytes;
    
    if(NULL == pui2_count)
    {
        return MMSBTLR_INV_ARG;
    }
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    *pui2_count = ui2_num;

    while(1)
    {
        if (ui1_idx >= 3)
        {
            ui1_idx = 0;
        }
        
        if (b_UTF16)
        {//read 00:00:00,000
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2*12, (UINT8*)&aui2_tmp[0], &ui4_done_bytes, e_type);
        }
        else
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 12, &aui1_tmp[0], &ui4_done_bytes, e_type);
            //ui2_tmp = ui1_tmp;
            for(ui1_idx1 = 0; ui1_idx1 < 12; ui1_idx1++)
            {
                aui2_tmp[ui1_idx1] = aui1_tmp[ui1_idx1];
            }
        }
        

        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if((aui2_tmp[0] >='0' && aui2_tmp[0] <='9') && (aui2_tmp[1] >='0' && aui2_tmp[1] <='9')
            && (aui2_tmp[3] >='0' && aui2_tmp[3] <='9') && (aui2_tmp[4] >='0' && aui2_tmp[4] <='9')
            && (aui2_tmp[6] >='0' && aui2_tmp[6] <='9') && (aui2_tmp[7] >='0' && aui2_tmp[7] <='9')
            && (aui2_tmp[9] >='0' && aui2_tmp[9] <='9') && (aui2_tmp[10] >='0' && aui2_tmp[10] <='9') && (aui2_tmp[11] >='0' && aui2_tmp[11] <='9')
            && (aui2_tmp[2] ==':' && aui2_tmp[5] ==':' && aui2_tmp[8] ==',') )
        {
            ui1_idx++;
            if(b_found_total == TRUE && pt_private != NULL && ui2_num < pt_private->ui2_time_max_cnt)
            {
                ui4_hour = sbtl_wasci2decimal(&aui2_tmp[0], 2);
                ui4_min = sbtl_wasci2decimal(&aui2_tmp[3], 2);
                ui4_sec = sbtl_wasci2decimal(&aui2_tmp[6], 2);
                ui4_ms = sbtl_wasci2decimal(&aui2_tmp[9], 3);
                pt_private->pt_time_stamp[ui2_num].t_pts_s= (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*3600);
                
            }
                
        }
        else 
        {
            if(b_found_total)
            {
                ui1_idx = 0;
                for(ui1_idx1 = 1; ui1_idx1 < 12; ui1_idx1++)
                {
                    if(aui2_tmp[ui1_idx1] >='0' && aui2_tmp[ui1_idx1] <='9')
                    {
                        
                        break;
                    }
                }
                if (b_UTF16)
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                         2*(ui1_idx1-12), 
                                                         MEDIA_SEEK_CUR, 
                                                         e_type);
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                         ui1_idx1-12, 
                                                         MEDIA_SEEK_CUR, 
                                                         e_type);
                }
                continue;
            }
            else
            {
                return MMSBTLR_OK;
            }
        }
        

        while(1)// ignore space
        {
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&aui2_tmp[0], &ui4_done_bytes, e_type);
                if(aui2_tmp[0] ==' ')
                {
                    
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                             -2, 
                                             MEDIA_SEEK_CUR, 
                                             e_type);
                    break;
                }
            }
            else
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 1, &aui1_tmp[0], &ui4_done_bytes, e_type);
               
                if(aui1_tmp[0] ==' ')
                {
                    
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                             -1, 
                                             MEDIA_SEEK_CUR, 
                                             e_type);
                    break;
                }
            }
        }

        if (b_UTF16)
        {//read -->
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2*3, (UINT8*)&aui2_tmp[0], &ui4_done_bytes, e_type);
            if (i4_ret != MMSBTLR_OK)
            {
                break;
            }
            if(aui2_tmp[0]=='-' && aui2_tmp[1]=='-' && aui2_tmp[2]=='>' )
            {
                ui1_idx++;
            }
            else 
            {
                if(b_found_total)
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                 -6, 
                                                 MEDIA_SEEK_CUR, 
                                                 e_type);
                    ui1_idx = 0;
                    continue;
                }
                else
                {
                    return MMSBTLR_OK;
                }
            }
        }
        else
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 3, &aui1_tmp[0], &ui4_done_bytes, e_type);
            if (i4_ret != MMSBTLR_OK)
            {
                break;
            }
            if(aui1_tmp[0]=='-' && aui1_tmp[1]=='-' && aui1_tmp[2]=='>' )
            {
                ui1_idx++;
            }
            else
            {
                
                if(b_found_total)
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                 -3, 
                                                 MEDIA_SEEK_CUR, 
                                                 e_type);
                    ui1_idx = 0;
                    continue;
                }
                else
                {
                    return MMSBTLR_OK;
                }
            }
        }

        while(1)// ignore space
        {
            if (b_UTF16)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&aui2_tmp[0], &ui4_done_bytes, e_type);
                if (i4_ret != MMSBTLR_OK)
                {
                    break;
                }
                if(aui2_tmp[0] ==' ')
                {
                    
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                             -2, 
                                             MEDIA_SEEK_CUR, 
                                             e_type);
                    break;
                }
            }
            else
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 1, &aui1_tmp[0], &ui4_done_bytes, e_type);
                   if (i4_ret != MMSBTLR_OK)
                {
                    break;
                }
                if(aui1_tmp[0] ==' ')
                {
                    
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                             -1, 
                                             MEDIA_SEEK_CUR, 
                                             e_type);
                    break;
                }
            }
        }
        
        if (b_UTF16)
        {//read 00:00:00,000
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2*12, (UINT8*)&aui2_tmp[0], &ui4_done_bytes, e_type);
        }
        else
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 12, &aui1_tmp[0], &ui4_done_bytes, e_type);
            
            for(ui1_idx1 = 0; ui1_idx1 < 12; ui1_idx1++)
            {
                aui2_tmp[ui1_idx1] = aui1_tmp[ui1_idx1];
            }
        }
        

        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if((aui2_tmp[0] >='0' && aui2_tmp[0] <='9') && (aui2_tmp[1] >='0' && aui2_tmp[1] <='9')
            && (aui2_tmp[3] >='0' && aui2_tmp[3] <='9') && (aui2_tmp[4] >='0' && aui2_tmp[4] <='9')
            && (aui2_tmp[6] >='0' && aui2_tmp[6] <='9') && (aui2_tmp[7] >='0' && aui2_tmp[7] <='9')
            && (aui2_tmp[9] >='0' && aui2_tmp[9] <='9') && (aui2_tmp[10] >='0' && aui2_tmp[10] <='9') && (aui2_tmp[11] >='0' && aui2_tmp[11] <='9')
            && (aui2_tmp[2] ==':' && aui2_tmp[5] ==':' && aui2_tmp[8] ==',') )
        {
            ui1_idx++;
            if(b_found_total == TRUE && pt_private != NULL && ui2_num < pt_private->ui2_time_max_cnt)
            {
                ui4_hour = sbtl_wasci2decimal(&aui2_tmp[0], 2);
                ui4_min = sbtl_wasci2decimal(&aui2_tmp[3], 2);
                ui4_sec = sbtl_wasci2decimal(&aui2_tmp[6], 2);
                ui4_ms = sbtl_wasci2decimal(&aui2_tmp[9], 3);
                pt_private->pt_time_stamp[ui2_num].t_pts_e= (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                                + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*3600);
                
                sbtl_feeder_getpos(pt_engine, (UINT64*)&pt_private->pt_time_stamp[ui2_num].ui4_offset, e_type);
            }
        }
        else
        {
            if(b_found_total)
            {
                ui1_idx = 0;
                for(ui1_idx1 = 1; ui1_idx1 < 12; ui1_idx1++)
                {
                    if(aui2_tmp[ui1_idx1] >='0' && aui2_tmp[ui1_idx1] <='9')
                    {
                        
                        break;
                    }
                }
                if (b_UTF16)
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                         2*(ui1_idx1-12), 
                                                         MEDIA_SEEK_CUR, 
                                                         e_type);
                }
                else
                {
                    i4_ret = sbtl_feeder_setpos(pt_engine,
                                                         ui1_idx1-12, 
                                                         MEDIA_SEEK_CUR, 
                                                         e_type);
                }
                continue;
            }
            else
            {
                return MMSBTLR_OK;
            }
        }
        

        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        
        if (ui1_idx >= 3)
        {
            ui1_idx = 0;

               ui2_num++;

            if((b_found_total == FALSE) && (ui2_num > 0))
            {
                *pui2_count = ui2_num;
                return MMSBTLR_OK;
            }
        }
    }
    
    *pui2_count = ui2_num;
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
static INT32 _srt_creat_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    UINT16                          ui2_count;
    INT32                           i4_ret;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);

    i4_ret = _srt_check_time_format(pt_mngr->pt_engine, MM_SBTL_FEEDER_SUBTITLE, TRUE, &ui2_count, pt_private, pt_private->b_UTF16);
    
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
INT32 _srt_alloc_time_stamp(
        SBTL_TEXT_PAGE_MNGR_PRIVATE_T*             pt_private,
        UINT16                                    ui2_time_stamp_cnt)
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

    t_size = sizeof(SBTL_TAG_INFO_T) * ui2_time_stamp_cnt;
    pt_private->pt_time_stamp = (SBTL_TAG_INFO_T*)x_mem_alloc(t_size);
    if(pt_private->pt_time_stamp == NULL)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }
    x_memset(pt_private->pt_time_stamp, 0, t_size);

    t_size = sizeof(UINT16) * ui2_time_stamp_cnt;
    pui2_order = (UINT16*)x_mem_alloc(t_size);
    if(pui2_order == NULL)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }

    pt_private->pui2_time_order = pui2_order;
    for(ui1_i = 0; ui1_i < ui2_time_stamp_cnt; ui1_i++)
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
INT32 _srt_time_stamp_sort(
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*         pt_private,
    UINT16                              ui2_order_start,
    UINT16                              ui2_order_end)
{
    UINT16                              ui2_start;
    UINT16                              ui2_end;
    UINT16                              ui2_i,ui2_j;
    UINT16                              ui2_cur,ui2_next;
    UINT16                              ui2_tmp_end;

    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    if ((ui2_order_end <= ui2_order_start) || (ui2_order_start > pt_private->ui2_time_max_cnt))
    {
        return MMSBTLR_INV_ARG;
    }

    ui2_start = ui2_order_start;
    if(ui2_order_end > pt_private->ui2_time_max_cnt)
    {
        ui2_end = pt_private->ui2_time_max_cnt;
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
INT32 _srt_free_time_stamp(SBTL_TEXT_PAGE_MNGR_PRIVATE_T* pt_private)
{
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

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


