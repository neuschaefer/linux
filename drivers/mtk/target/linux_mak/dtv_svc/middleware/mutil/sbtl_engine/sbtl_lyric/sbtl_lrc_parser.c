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

#include "mutil/sbtl_engine/sbtl_lyric/sbtl_lyric_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_lyric/sbtl_lrc_parser.h"

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

#include "sbtl_lyric_page_inst_mngr.h"
#include "sbtl_lrc_parser.h"
#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _lrc_header_tag_fmt_init(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private);
static INT32 _lrc_creat_header_info(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _lrc_time_format_init(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private);

static INT32 _lrc_get_pts_time(
    UINT8                           ui1_fmt_idx,
    UINT8*                          pui1_buf,
    UINT8                           ui1_len,
    PTS_T*                          pt_pts,
    BOOL                            b_UTF16);

static INT32 _lrc_creat_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

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
static INT32 _lrc_header_tag_fmt_init(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private)
{
    SBTL_TAG_FORMAT_T*             pt_tag_inf;
    
    ASSERT(pt_private, ("pt_lrc_private=NULL\n"));

    /*[ti:*/
    pt_tag_inf = &pt_private->at_header_tag[LYRIC_HEADER_TAG_TITLE];
    pt_tag_inf->ui1_len = 4;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_tag_inf->ui1_fmt_len = 8;
    }
    else
    {
        pt_tag_inf->ui1_fmt_len = 4;
    }
    x_strcpy(pt_tag_inf->s_content, (const CHAR *) "[ti:");
    pt_tag_inf->ui1_content_pos[0] = 0;
    pt_tag_inf->ui1_content_pos[1] = 1;
    pt_tag_inf->ui1_content_pos[2] = 2;
    pt_tag_inf->ui1_content_pos[3] = 3;
    pt_tag_inf->b_check_num = FALSE;
    
    /*[ar:*/
    pt_tag_inf = &pt_private->at_header_tag[LYRIC_HEADER_TAG_ARTIST];
    pt_tag_inf->ui1_len = 4;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_tag_inf->ui1_fmt_len = 8;
    }
    else
    {
        pt_tag_inf->ui1_fmt_len = 4;
    }    
    x_strcpy(pt_tag_inf->s_content, (const CHAR *) "[ar:");
    pt_tag_inf->ui1_content_pos[0] = 0;
    pt_tag_inf->ui1_content_pos[1] = 1;
    pt_tag_inf->ui1_content_pos[2] = 2;
    pt_tag_inf->ui1_content_pos[3] = 3;
    pt_tag_inf->b_check_num = FALSE;

    /*[al:*/
    pt_tag_inf = &pt_private->at_header_tag[LYRIC_HEADER_TAG_ALBUM];
    pt_tag_inf->ui1_len = 4;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_tag_inf->ui1_fmt_len = 8;
    }
    else
    {
        pt_tag_inf->ui1_fmt_len = 4;
    }    
    x_strcpy(pt_tag_inf->s_content, (const CHAR *) "[al:");
    pt_tag_inf->ui1_content_pos[0] = 0;
    pt_tag_inf->ui1_content_pos[1] = 1;
    pt_tag_inf->ui1_content_pos[2] = 2;
    pt_tag_inf->ui1_content_pos[3] = 3;
    pt_tag_inf->b_check_num = FALSE;

    /*[by:*/
    pt_tag_inf = &pt_private->at_header_tag[LYRIC_HEADER_TAG_EDITOR];
    pt_tag_inf->ui1_len = 4;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_tag_inf->ui1_fmt_len = 8;
    }
    else
    {
        pt_tag_inf->ui1_fmt_len = 4;
    }    
    x_strcpy(pt_tag_inf->s_content, (const CHAR *) "[by:");
    pt_tag_inf->ui1_content_pos[0] = 0;
    pt_tag_inf->ui1_content_pos[1] = 1;
    pt_tag_inf->ui1_content_pos[2] = 2;
    pt_tag_inf->ui1_content_pos[3] = 3;
    pt_tag_inf->b_check_num = FALSE;

    /*]*/
    pt_tag_inf = &pt_private->t_end_tag;
    pt_tag_inf->ui1_len = 1;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_tag_inf->ui1_fmt_len = 2;
    }
    else
    {
        pt_tag_inf->ui1_fmt_len = 1;
    }    
    x_strcpy(pt_tag_inf->s_content, (const CHAR *) "]");
    pt_tag_inf->ui1_content_pos[0] = 0;
    pt_tag_inf->b_check_num = FALSE;
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
static INT32 _lrc_creat_header_info(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    INT32                               i4_ret;
    UINT8*                              pui1_buf = NULL; 
    UINT8                               ui1_i;
    UINT64                              ui8_len;
    UINT64                              ui8_offset = 0;
    UINT8                               ui1_fmt_len;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private->i8_pts_offset = 0;
    x_memset(pt_private->at_header_inf,0,LYRIC_HEADER_TAG_MAX_NUM * sizeof(SBTL_TAG_INFO_T));
    
    for(ui1_i = 0; ui1_i < LYRIC_HEADER_TAG_MAX_NUM; ui1_i ++)
    {
        i4_ret = sbtl_seek_tag_inf(pt_mngr->pt_engine,
                                   0,
                                   &pt_private->at_header_tag[ui1_i],
                                   &pt_private->t_end_tag,
                                   &pui1_buf,
                                   &ui8_offset,
                                   &ui8_len,
                                   MM_SBTL_FEEDER_SUBTITLE,
                                   pt_private->b_UTF16);
        if(i4_ret == MMSBTLR_OK)
        {
            pt_private->at_header_inf[ui1_i].ui4_offset = (UINT32)ui8_offset;
            pt_private->at_header_inf[ui1_i].ui1_length = (UINT8)ui8_len;
        }
    }

    ui8_offset = pt_private->at_header_inf[LYRIC_HEADER_TAG_OFFSET].ui4_offset;
    ui1_fmt_len = pt_private->at_header_inf[LYRIC_HEADER_TAG_OFFSET].ui1_length;
    if(ui1_fmt_len > 0)
    {
        UINT32     ui4_done = 0;
        PTS_T      t_pts;  
        BOOL       b_negative = TRUE;
        BOOL       b_fmt_error = FALSE;
        UINT16*    pui2_buf = NULL;
        UINT16     ui2_data;
        
        sbtl_feeder_inpnbyte_addr_p(pt_mngr->pt_engine,
                                    ui8_offset,
                                    ui1_fmt_len,
                                    &pui1_buf,
                                    &ui4_done,
                                    MM_SBTL_FEEDER_SUBTITLE);  
        if(ui4_done < ui1_fmt_len)
        {
            ui1_fmt_len = (UINT8)ui4_done;
        }

        if(pt_private->b_UTF16 == TRUE)
        {
            pui2_buf = (UINT16*)pui1_buf;
            ui2_data = *pui2_buf;
        }
        else
        {
            ui2_data =(UINT16)(*pui1_buf);
        }
        
        while( ((ui2_data < '0') || (ui2_data > '9')) && (ui1_fmt_len > 0))
        {
            if(ui2_data == '-')
            {
                b_negative = TRUE;
            }
            else if(ui2_data == '+')
            {
                b_negative = FALSE;
            } 
            else if(ui2_data != ' ')
            {
                b_fmt_error = TRUE;
                break;
            }
            
            if(pt_private->b_UTF16 == TRUE)
            {
                pui1_buf += 2;
                ui1_fmt_len -= 2;
                ui2_data = *pui2_buf;
            }
            else
            {
                pui1_buf ++;
                ui1_fmt_len --;
                ui2_data =(UINT16)(*pui1_buf);
            }                
        }

        if((ui1_fmt_len > 0) && (b_fmt_error == FALSE))
        {
            _lrc_get_pts_time(LRC_TAG_TIME_OFFSET, pui1_buf, ui1_fmt_len, &t_pts, pt_private->b_UTF16);
            if(b_negative == TRUE)
            {
                pt_private->i8_pts_offset = 0 - (INT64)t_pts;
            }
            else
            {
                pt_private->i8_pts_offset = (INT64)t_pts;
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
 /*lrc time tag: [mm:ss] ,[mm:ss.fff] ,[mm:ss.f]*/
static INT32 _lrc_time_format_init(SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private)
{
    SBTL_TAG_FORMAT_T*             pt_time_fmt;
    
    ASSERT(pt_private, ("pt_lrc_private=NULL\n"));

    /*ui1_time_fmt_cnt must <= SBTTL_LYRIC_TIME_FMT_MAX*/
    pt_private->ui1_time_fmt_cnt = LRC_TAG_TIME_LAST;

    /*[mm:ss]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT1];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    pt_time_fmt->ui1_len = 3;
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 14;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 7;
    }
    
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:]");
    
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 3;
    pt_time_fmt->ui1_content_pos[2] = 6;
    pt_time_fmt->b_check_num = TRUE;

    /*[mm:ss.ff]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT2];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 20;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 10;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 3;
    pt_time_fmt->ui1_content_pos[2] = 6;
    pt_time_fmt->ui1_content_pos[3] = 9;
    pt_time_fmt->b_check_num = TRUE;

    /*[mm:ss.fff]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT3];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 22;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 11;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 3;
    pt_time_fmt->ui1_content_pos[2] = 6;
    pt_time_fmt->ui1_content_pos[3] = 10;
    pt_time_fmt->b_check_num = TRUE;

    /*[mm:ss.f]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT4];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 18;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 9;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 3;
    pt_time_fmt->ui1_content_pos[2] = 6;
    pt_time_fmt->ui1_content_pos[3] = 8;
    pt_time_fmt->b_check_num = TRUE;

    /*[m:ss.f]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT5];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 16;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 8;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 2;
    pt_time_fmt->ui1_content_pos[2] = 5;
    pt_time_fmt->ui1_content_pos[3] = 7;
    pt_time_fmt->b_check_num = TRUE;

    /*[m:ss.ff]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT6];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 18;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 9;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 2;
    pt_time_fmt->ui1_content_pos[2] = 5;
    pt_time_fmt->ui1_content_pos[3] = 8;
    pt_time_fmt->b_check_num = TRUE;

    /*[m:ss.fff]*/
    pt_time_fmt = &pt_private->at_time_fmt[LRC_TAG_TIME_FMT7];
    ASSERT(pt_time_fmt, ("pt_time_fmt=NULL\n"));
    if(pt_private->b_UTF16 == TRUE)
    {
        pt_time_fmt->ui1_fmt_len = 20;
    }
    else
    {
        pt_time_fmt->ui1_fmt_len = 10;
    }    
    pt_time_fmt->ui1_len = 4;
    x_strcpy(pt_time_fmt->s_content, (const CHAR *) "[:.]");
    pt_time_fmt->ui1_content_pos[0] = 0;
    pt_time_fmt->ui1_content_pos[1] = 2;
    pt_time_fmt->ui1_content_pos[2] = 5;
    pt_time_fmt->ui1_content_pos[3] = 9;
    pt_time_fmt->b_check_num = TRUE;
    
    return MMSBTLR_OK;
}

static INT32 _lrc_get_pts_time(
    UINT8                           ui1_fmt_idx,
    UINT8*                          pui1_buf,
    UINT8                           ui1_len,
    PTS_T*                          pt_pts,
    BOOL                            b_UTF16)
{
    UINT32                          ui4_min = 0;
    UINT32                          ui4_sec = 0;
    UINT32                          ui4_ms = 0;
    UINT16*                         pui2_buf;

    if ((pui1_buf == NULL) || (pt_pts == NULL))
    {
        return MMSBTLR_INV_ARG;
    }

    pui2_buf =  (UINT16*)pui1_buf;

    switch(ui1_fmt_idx)
    {
    case LRC_TAG_TIME_FMT1:
        { 
            /*[mm:ss]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 2);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[4], 2);
                ui4_ms = 0;
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 2);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[4], 2);
                ui4_ms = 0;
            }
            break;
        }

    case LRC_TAG_TIME_FMT2:
        { 
            /*[mm:ss.ff]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 2);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[4], 2);
                ui4_ms = 10 * sbtl_wasci2decimal(&pui2_buf[7], 2);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 2);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[4], 2);
                ui4_ms = 10 * sbtl_asci2decimal(&pui1_buf[7], 2);
            }            
            break;
        }

    case LRC_TAG_TIME_FMT3:
        { 
            /*[mm:ss.fff]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 2);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[4], 2);
                ui4_ms =  sbtl_wasci2decimal(&pui2_buf[7], 3);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 2);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[4], 2);
                ui4_ms =  sbtl_asci2decimal(&pui1_buf[7], 3);
            }            
            break;
        } 
    
    case LRC_TAG_TIME_FMT4:
        { 
            /*[mm:ss.f]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 2);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[4], 2);
                ui4_ms =  100 * sbtl_wasci2decimal(&pui2_buf[7], 1);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 2);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[4], 2);
                ui4_ms =  100 * sbtl_asci2decimal(&pui1_buf[7], 1);
            }             
            break;
        } 

     case LRC_TAG_TIME_FMT5:
        { 
            /*[m:ss.f]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 1);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[3], 2);
                ui4_ms =  100 * sbtl_wasci2decimal(&pui2_buf[6], 1);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 1);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[3], 2);
                ui4_ms =  100 * sbtl_asci2decimal(&pui1_buf[6], 1);
            }             
            break;
        } 

      case LRC_TAG_TIME_FMT6:
        { 
            /*[m:ss.ff]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 1);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[3], 2);
                ui4_ms =  10 * sbtl_wasci2decimal(&pui2_buf[6], 2);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 1);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[3], 2);
                ui4_ms =  10 * sbtl_asci2decimal(&pui1_buf[6], 2);
            }             
            break;
        } 

       case LRC_TAG_TIME_FMT7:
        { 
            /*[m:ss.fff]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = sbtl_wasci2decimal(&pui2_buf[1], 1);
                ui4_sec = sbtl_wasci2decimal(&pui2_buf[3], 2);
                ui4_ms =  sbtl_wasci2decimal(&pui2_buf[6], 3);
            }
            else
            {
                ui4_min = sbtl_asci2decimal(&pui1_buf[1], 1);
                ui4_sec = sbtl_asci2decimal(&pui1_buf[3], 2);
                ui4_ms =  sbtl_asci2decimal(&pui1_buf[6], 3);
            }             
            break;
        } 
    
    case LRC_TAG_TIME_OFFSET:
        { 
            /*[mm:ss.f]*/
            if(b_UTF16 == TRUE)
            {
                ui4_min = 0;
                ui4_sec = 0;
                ui4_ms =  sbtl_wasci2decimal(&pui2_buf[0], ui1_len);
            }
            else
            {
                ui4_min = 0;
                ui4_sec = 0;
                ui4_ms =  sbtl_asci2decimal(&pui1_buf[0], ui1_len);
            }             
            break;
        }     
    
    default:
        break;
        
    }

    *pt_pts = (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                  + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                  + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60);
    
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
static INT32 _lrc_creat_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    UINT8*                              pui1_buf;
    UINT64                              ui8_pos = 0;
    UINT64                              ui8_offset = 0;
    UINT64                              ui8_seek_len;
    INT32                               i4_ret;
    UINT16                              ui2_found_cnt;
    BOOL                                b_found;
    UINT8                               ui1_fmt;        
    UINT8                               ui1_fmt_len;        
    PTS_T                               t_pts;
    UINT16                              ui2_time_cur = 0;
    UINT16                              ui2_continue_cnt;
    UINT16                              ui2_time_idx;
    UINT16                              ui2_content_len;
    
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    sbtl_feeder_setpos(pt_mngr->pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);

    i4_ret = sbtl_tag_inf_seek(pt_mngr->pt_engine,
           ui8_pos,
           pt_private->ui1_time_fmt_cnt,
           pt_private->at_time_fmt,
           MM_SBTL_FEEDER_SUBTITLE,
           FALSE,
           &pui1_buf,
           &ui8_seek_len,
           &ui2_found_cnt,
           &ui1_fmt,
           pt_private->b_UTF16);

    if ((i4_ret != MMSBTLR_OK) || (ui2_found_cnt == 0) || (ui1_fmt >= pt_private->ui1_time_fmt_cnt))
    {
        return MMSBTLR_NOT_FOUND;
    }
    
    while(1)
    {
        /*Find a time stamp information.Then check the next information is time stamp or not.*/
        if(ui2_time_cur >= pt_private->ui2_time_max_cnt)
        {
            i4_ret = MMSBTLR_INSUFFICIENT_MEMORY;
            break;
        }

        if(ui1_fmt >= SBTTL_LYRIC_TIME_FMT_MAX)
        {
            break;
        }
        
        ui1_fmt_len = pt_private->at_time_fmt[ui1_fmt].ui1_fmt_len;
        _lrc_get_pts_time(ui1_fmt, pui1_buf, ui1_fmt_len, &t_pts, pt_private->b_UTF16);

        pt_private->pt_time_stamp[ui2_time_cur].t_pts_s = t_pts;
        ui2_time_cur ++;
        ui2_continue_cnt = 1;
        ui8_pos += ui8_seek_len;

        do{
            b_found = sbtl_check_tag_inf_by_addr(pt_mngr->pt_engine,
                                           ui8_pos,
                                           pt_private->ui1_time_fmt_cnt,
                                           pt_private->at_time_fmt,
                                           MM_SBTL_FEEDER_SUBTITLE,
                                           &pui1_buf,
                                           &ui1_fmt,
                                           pt_private->b_UTF16);
            if((b_found == TRUE) && 
               (ui1_fmt <  pt_private->ui1_time_fmt_cnt) && 
               (ui1_fmt < SBTTL_LYRIC_TIME_FMT_MAX))
            {
                ui1_fmt_len = pt_private->at_time_fmt[ui1_fmt].ui1_fmt_len;
                _lrc_get_pts_time(ui1_fmt, pui1_buf, ui1_fmt_len, &t_pts, pt_private->b_UTF16);
                pt_private->pt_time_stamp[ui2_time_cur].t_pts_s = t_pts;
                ui8_pos += pt_private->at_time_fmt[ui1_fmt].ui1_fmt_len;  
                if(ui2_time_cur < pt_private->ui2_time_max_cnt)
                {
                    ui2_time_cur ++;
                    ui2_continue_cnt ++; 
                }
                else
                {
                    break;
                }
            }
            else
            {
                ui8_offset = ui8_pos;
                break;
            }
        }while(1);

        i4_ret = sbtl_tag_inf_seek(pt_mngr->pt_engine,
               ui8_pos,
               pt_private->ui1_time_fmt_cnt,
               pt_private->at_time_fmt,
               MM_SBTL_FEEDER_SUBTITLE,
               FALSE,
               &pui1_buf,
               &ui8_seek_len,
               &ui2_found_cnt,
               &ui1_fmt,
               pt_private->b_UTF16);

        if ((ui2_found_cnt > 0) && (ui1_fmt < SBTTL_LYRIC_TIME_FMT_MAX))
        {
            ui2_content_len = (UINT16)ui8_seek_len - (UINT16)pt_private->at_time_fmt[ui1_fmt].ui1_fmt_len;    
        }
        else if(i4_ret == MMSBTLR_EOT)
        {
            ui2_content_len = (UINT16)ui8_seek_len;
        }
        else
        {
            ui2_content_len = 0;
        }
        
        while(ui2_continue_cnt > 0)
        {
            ui2_time_idx = ui2_time_cur-ui2_continue_cnt;
            pt_private->pt_time_stamp[ui2_time_idx].ui4_offset = (UINT32)ui8_offset;
            pt_private->pt_time_stamp[ui2_time_idx].ui1_length = (UINT8)ui2_content_len;
            ui2_continue_cnt --;     
        }

         
        if ((ui2_found_cnt == 0) || (i4_ret == MMSBTLR_EOT))
        {
            /*There is no time stamp.*/
            break;
        }
    }

    pt_private->ui2_time_cnt = ui2_time_cur;
    pt_private->pt_time_stamp[ui2_time_cur].t_pts_s = 0xFFFFFFFFFFFFFFFF;
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
INT32 lrc_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                  BOOL                    b_pts_neg,
                                  PTS_T                   t_pts_current,
                                  UINT16                  ui2_time_cnt,
                                  UINT16*                 pui2_found_cnt)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    UINT16                           ui2_time_idx;
    UINT16                           ui2_idx;
    UINT16                           ui2_cur_idx;
    UINT16                           ui2_order_start;
    UINT16                           ui2_order_end;
    BOOL                             b_found = FALSE;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if((pt_private->pt_time_stamp == NULL) || 
       (pt_private->pui2_time_order == NULL) || 
       (pui2_found_cnt== NULL))
    {
        return MMSBTLR_INV_ARG;
    }

    *pui2_found_cnt = 0;

    /*If b_pts_neg = TRUE,it seek the header information*/
    if(b_pts_neg == TRUE)
    {
        for(ui2_idx = 0; ui2_idx < LYRIC_HEADER_TAG_MAX_NUM; ui2_idx ++)
        {
            if(pt_private->at_header_inf[ui2_idx].ui1_length > 0)
            {
                (*pui2_found_cnt) ++;
            }
        }

        if((*pui2_found_cnt) > 0)
        {
            pt_private->ui2_cur_idx = LYRIC_IDX_HEADER;
            pt_private->t_pts_s = 0;
            pt_private->t_pts_e = 0;
        }
        else
        {
            pt_private->ui2_cur_idx = LYRIC_IDX_NULL;
        }
        return MMSBTLR_OK;
    }
    
    /*If b_pts_neg = FALSE,it seek the time tag information*/
    if ((pt_private->ui2_cur_idx == LYRIC_IDX_HEADER) || 
        (pt_private->ui2_cur_idx == LYRIC_IDX_NULL))
    {
        ui2_cur_idx = 0;
    }
    else if(pt_private->ui2_cur_idx >= pt_private->ui2_time_cnt)
    {
        ui2_cur_idx = (UINT16)(pt_private->ui2_time_cnt - 1);
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
        ui2_order_end = (UINT16)pt_private->ui2_time_cnt ;
    }

    for(ui2_idx = ui2_order_start; ui2_idx <= ui2_order_end; ui2_idx ++)
    {
        ui2_time_idx = pt_private->pui2_time_order[ui2_idx];
        if(t_pts_current < pt_private->pt_time_stamp[ui2_time_idx].t_pts_s)
        {
            (*pui2_found_cnt)++;
            b_found = TRUE;
            break;
        }
    }

    if(b_found == TRUE)
    {
        if(ui2_idx > 0)
        {
            pt_private->ui2_cur_idx = (UINT16)(ui2_idx - 1);

            ui2_time_idx = pt_private->pui2_time_order[pt_private->ui2_cur_idx];
            pt_private->t_pts_s = pt_private->pt_time_stamp[ui2_time_idx].t_pts_s;
            
            ui2_time_idx = pt_private->pui2_time_order[ui2_idx];
            pt_private->t_pts_e = pt_private->pt_time_stamp[ui2_time_idx].t_pts_s;
        }
        else
        {
            DBG_INFO(("Lrc is before display!\n"));
        }
    }
    else
    {
        return MMSBTLR_EOT;
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
INT32 lrc_load_string(
    SBTL_PAGE_INST_MNGR_T*          pt_mngr,
    UINT16                          ui2_load_idx)
{
    UINT8                           au1MatchCode[4] = {0x0D, 0x0A, 0x0D, 0x0A};
    UINT16                          ui2_tag_idx = 0;
    UINT32                          ui4_length;
    UINT8*                          pui1_buf;
    UINT16*                         pui2_tmp;
    UINT32                          ui4_done_length;
    UINT8                           ui1_idx,ui1_idx2;
    UINT16                          ui2_tmp;
    UINT64                          ui8_offset;
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T* pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if ((pt_mngr->pt_engine == NULL) ||
        (ui2_load_idx == LYRIC_IDX_NULL) ||
        ((ui2_load_idx >= pt_private->ui2_time_cnt) &&
         (ui2_load_idx != LYRIC_IDX_HEADER)) )
    {
        return MMSBTLR_INV_ARG;
    }

    ui1_idx = 0;

    while(1)
    {
        if(ui2_load_idx == LYRIC_IDX_HEADER)
        {
            if(ui2_tag_idx >= LYRIC_HEADER_TAG_MAX_NUM)
            {
                break;
            }
            else
            {
                ui8_offset = (UINT64)pt_private->at_header_inf[ui2_tag_idx].ui4_offset;
                ui4_length = (UINT32)pt_private->at_header_inf[ui2_tag_idx].ui1_length;
            }
            
        }
        else
        {
            ui2_tag_idx = pt_private->pui2_time_order[ui2_load_idx];
            
            if(ui2_tag_idx >= pt_private->ui2_time_cnt)
            {
                return MMSBTLR_INV_ARG;
            }
            ui8_offset = (UINT64)pt_private->pt_time_stamp[ui2_tag_idx].ui4_offset;
            ui4_length = (UINT32)pt_private->pt_time_stamp[ui2_tag_idx].ui1_length;
        }

        sbtl_feeder_inpnbyte_addr_p(pt_mngr->pt_engine,
                                    ui8_offset,
                                    ui4_length,
                                    &pui1_buf,
                                    &ui4_done_length,
                                    MM_SBTL_FEEDER_SUBTITLE);
        
        if(ui4_done_length > 0)
        {
            if(ui4_done_length >= MM_SBTL_CACHE_MAX)
            {
                ui4_done_length = MM_SBTL_CACHE_MAX - 1;
            }
            
            x_memset(pt_private->aui1_text_buf,0,MM_SBTL_CACHE_MAX);
            x_memset(pt_private->w2s_text,0,sizeof(UTF16_T)*MM_SBTL_CACHE_MAX);
            x_memcpy(pt_private->aui1_text_buf,pui1_buf,ui4_done_length);
            pt_private->aui1_text_buf[MM_SBTL_CACHE_MAX - 1] = 0;
            sbtl_trans_to_w2s_ex(pt_mngr->pt_engine->t_cfg.e_encoding,
                                 pt_private->aui1_text_buf,
                                 ui4_done_length,
                                 pt_private->w2s_text);
            
            ui1_idx2 = 0;
            ui1_idx = 0;
            pui2_tmp = pt_private->w2s_text;
            while(*pui2_tmp != 0)
            {   
                ui2_tmp = *pui2_tmp;
                
                if (ui2_tmp == 0x0D)
                {
                    *pui2_tmp = 0;
                    if(pui2_tmp[1] == 0x0A)
                    {
                        pui2_tmp ++;
                        *pui2_tmp = 0;
                    }
                    ui1_idx -= 1;
                }
                else if(ui2_tmp == 0x0A)
                {
                    if(pui2_tmp[1] == 0x0A)
                    {
                        *pui2_tmp = 0;
                        break;
                    }
                    *pui2_tmp = 0;
                    ui1_idx -= 1;
                }
                else
                {
                    pt_private->w2s_text[ui1_idx] = ui2_tmp;

                }
                
                if (ui2_tmp == au1MatchCode[ui1_idx2])
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

                if (ui1_idx2 >= 4)
                {
                    ui1_idx -= 2;
                    pt_private->w2s_text[ui1_idx] = 0;
                    break;
                }  
                
                ui1_idx++;
                pui2_tmp ++;
                
            }
            
        }

        if(ui2_load_idx == LYRIC_IDX_HEADER)
        {
            pt_private->w2s_text[ui1_idx] = 0x0A; 
            ui2_tag_idx ++;
        }
        else
        {
            break;
        }
    }

    pt_private->w2s_text[ui1_idx] = 0;    
    
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
UINT32 lrc_timer_period(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    return 500;
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
INT32 lrc_init(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*  pt_private;
    UINT16                           ui2_found_cnt;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if(pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    _lrc_time_format_init(pt_private);
    
    sbtl_tag_inf_seek(pt_mngr->pt_engine,
                   0,
                   pt_private->ui1_time_fmt_cnt,
                   pt_private->at_time_fmt,
                   MM_SBTL_FEEDER_SUBTITLE,
                   TRUE,
                   NULL,
                   NULL,
                   &ui2_found_cnt,
                   NULL,
                   pt_private->b_UTF16);

    if(ui2_found_cnt == 0)
    {
        return MMSBTLR_INTERNAL_STATE_ERROR;
    }

    sbtl_lyric_alloc_time_stamp(pt_private,ui2_found_cnt);

    _lrc_creat_time_stamp(pt_mngr);

    sbtl_lyric_time_stamp_order(pt_mngr,0,ui2_found_cnt);

    _lrc_header_tag_fmt_init(pt_private);

    _lrc_creat_header_info(pt_mngr);
    
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
INT32 lrc_deinit(SBTL_PAGE_INST_MNGR_T* pt_mngr)
{
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
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
VOID lrc_start(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

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
VOID lrc_stop(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
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
BOOL lrc_parser_verify(SBTL_ENGINE_T*   pt_engine)
{

    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    return TRUE;
}

INT32 lrc_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                    VOID*                    pv_set_info,
                    UINT32                   ui4_set_type)
{
    SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*     pt_private;
    INT32                               i4_adjust;
    UINT64                              ui8_tmp;
    MM_SBTL_TM_OFST_INF*                pt_time_ofst;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_LYRIC_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));

    if(pv_set_info == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_ADJUST_TIME:
        pt_time_ofst = (MM_SBTL_TM_OFST_INF*)pv_set_info;
        pt_private->e_tm_ofst_mode = pt_time_ofst->e_ofst_mode;

        if(pt_time_ofst->e_ofst_mode == MM_SBTL_TM_OFST_MODE_USER)
        {
            i4_adjust = (INT32)pt_time_ofst->i8_time_ofst;
            if(i4_adjust >= 0)
            {
                ui8_tmp = (UINT64)(i4_adjust );
            }
            else
            {
                ui8_tmp = (UINT64)(0 - i4_adjust);
            }
                
            ui8_tmp = (UINT64)_mm_div64(PTS_FREQ*ui8_tmp, 1000, NULL);

            if(i4_adjust >= 0)
            {
                 pt_private->i8_usr_pts_ofst += (INT64)ui8_tmp;
            }
            else
            {
                 pt_private->i8_usr_pts_ofst -= (INT64)ui8_tmp;
            } 
            
        }
        
        
        break;
        
    default:
        break;
    }
    
    return MMSBTLR_OK;
}
