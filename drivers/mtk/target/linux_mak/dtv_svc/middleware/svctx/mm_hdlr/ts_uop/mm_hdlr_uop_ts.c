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
 * $RCSfile: mm_hdlr_uop_ts.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "svctx/mm_hdlr/ts_uop/mm_hdlr_uop_ts.h"
#include "mutil/mm_util.h"

#include "file_mngr/x_fm.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#define CHK_MAX_TAG_NUM    3
#define CHK_MAX_DATA_NUM   10/* 5M = 10*0.5M */
#define INVALID_VALUE      0x12345678
#define TS_BUF_SIZE        (512*1024)/*Only process first 512k and last 512k data*/
#define TS_DUR_SIZE_RATIO  (30*1024)/*Expierence Date the ratio between duration and size 2*1024*1024 needs 21 bits*/
#define TS_DUR_SIZE_MAX_RATIO  (10*1024*1024)/*Expierence Date the ratio between duration and size 2*1024*1024 needs 21 bits*/
#define TS_PCR_FREQ        90000

static UINT32 ui4_ts_packet_len = 188;
static UINT64 ui8_get_pcr_value(UINT8* pui1_buf, UINT32 ui4_buf_len, BOOL fg_is_first)
{
    UINT32    ui4_i               = 0;
    UINT32    ui4_cur_pos         = 0;
    UINT32    ui4_tag_self_adap   = 0;/* Adaption field */
    UINT32    ui4_tag_adap_len    = 0;/* Adaption field length*/
    UINT32    ui4_tag_pcr_flag    = 0;/* Adaption field flag */
    UINT64    ui8_pcr_value       = INVALID_VALUE;
AGAIN:
    while(1)
    {
        if(ui4_cur_pos >= ui4_buf_len)
        {
            return ui8_pcr_value;
        }
        if(pui1_buf[ui4_cur_pos] == 0x47)
        {
            for(ui4_i = 0; ui4_i < CHK_MAX_TAG_NUM; ui4_i ++)
            {
                if(pui1_buf[ui4_cur_pos + ui4_i * ui4_ts_packet_len] != 0x47)
                {
                    break;
                }
            }
            if(ui4_i == CHK_MAX_TAG_NUM)
            {
                break;
            }
        }
        ui4_cur_pos ++;
    }

    while(ui4_cur_pos < ui4_buf_len)
    {
        /*Adaption field flag in ts packet forth byte third bit*/
        if(pui1_buf[ui4_cur_pos] != 0x47)
        {
            goto AGAIN;
        }
        ui4_tag_self_adap = pui1_buf[ui4_cur_pos + 3] >> 5 & 0x01; 
		ui4_tag_adap_len  = pui1_buf[ui4_cur_pos + 4];
        if((1 == ui4_tag_self_adap) && (ui4_tag_adap_len != 0))  /*check adaption field flag */
        {
            /*PCR flag is in ts packet sixth byte forth bit*/
            ui4_tag_pcr_flag = pui1_buf[ui4_cur_pos + 5] >> 4 & 0x01; 
            if(1==ui4_tag_pcr_flag)      /*Check PCR flag*/
            {
                /* Calculator pcr value*/
                ui8_pcr_value = (pui1_buf[ui4_cur_pos + 6] << 25) | 
                                (pui1_buf[ui4_cur_pos + 7] << 17) | 
                                (pui1_buf[ui4_cur_pos + 8] << 9)  |
                                (pui1_buf[ui4_cur_pos + 9] << 1)  |
                                ((pui1_buf[ui4_cur_pos + 10] >> 7) & 0x01);
                ui8_pcr_value &= 0x1ffffffff; 
                if(fg_is_first)
                {
                    break;
                }
            }
        }
        ui4_cur_pos = ui4_cur_pos + ui4_ts_packet_len;
        if(ui4_cur_pos >= ui4_buf_len)
        {
            break;
        }
    }

    return ui8_pcr_value;
}

static UINT64 ui8_get_ts_duration_with_feeder(MEDIA_DESCRIBT_T*   pt_media_desc,
                                                      UINT64* pt_start_pcr)
{
    INT32           i4_ret              = 0;
    UINT32          ui4_read            = 0;
    UINT32          ui4_buff_size       = 0;
    UINT64          ui8_cur_pos         = 0;
    UINT64          ui8_file_size       = 0;
    UINT64          ui8_pcr_value_first = INVALID_VALUE;
    UINT64          ui8_pcr_value_end   = 0;
    UINT8*          pui1_buffer         = NULL;
    UINT32          ui4_half_N_Mega     = 0;
    UINT64          ui8_calc_dur        = 0;
    UINT32          ui4_byte_per_sec    = 0;

    x_dbg_stmt("Enter ts duration retrieve \n");

    MEDIA_FEEDER_T* pt_src_feeder = NULL;    
        
    pt_src_feeder = (MEDIA_FEEDER_T*)pt_media_desc->pt_media_feeder;
    i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                                            &pt_src_feeder->h_feeder,
                                                            0,
                                                            MEDIA_SEEK_END|MEDIA_SEEK_CHK);
    if (i4_ret != INPUTR_OK)
    {
        x_dbg_stmt("Ts_duration:pf_set_pos(end) faile, ret %d\n", i4_ret);
        return 0;
    }

    i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos(
                            &pt_src_feeder->h_feeder,
                            &ui8_cur_pos);
    x_dbg_stmt("Ts_duration: pf_get_pos(=%lld) ret = %d \n", ui8_cur_pos, i4_ret);
    if (i4_ret != INPUTR_OK)
    {        
        return 0;
    }


    pt_src_feeder = (MEDIA_FEEDER_T*)pt_media_desc->pt_media_feeder;
    i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                            &pt_src_feeder->h_feeder,
                            0,
                            MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        x_dbg_stmt("Ts_duration: pf_set_pos(0) failed %d\n",i4_ret);
        return 0;
    }
    
    ui8_file_size = ui8_cur_pos;
    
    /* get first pcr value*/
    if(ui8_file_size > TS_BUF_SIZE)
    {
        ui4_buff_size = TS_BUF_SIZE;
    }
    else
    {
        ui4_buff_size = (UINT32)ui8_file_size;
    }
    
    pui1_buffer = (UINT8*)x_mem_alloc(ui4_buff_size);
    if(pui1_buffer == NULL)
    {
        x_dbg_stmt("Ts_duration: Buffer alloc failed\n"); 
        return 0;
    }

    /* get first pcr value*/
    ui4_half_N_Mega = 0;
    while(1)
    {
        x_memset(pui1_buffer, 0, ui4_buff_size);
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                            &pt_src_feeder->h_feeder,
                            ui4_buff_size * ui4_half_N_Mega,
                            MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            x_dbg_stmt("Ts_duration:  pf_set_pos(%d) failed\n", ui4_buff_size * ui4_half_N_Mega);
            x_mem_free(pui1_buffer);
            return 0;
        }

        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                            &pt_src_feeder->h_feeder,
                            pui1_buffer,
                            ui4_buff_size,
                            ui4_buff_size,
                            &ui4_read);
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("Ts_duration:  read fail ret = %d \n", i4_ret);
            break;
        }
        
        ui8_pcr_value_first = ui8_get_pcr_value(pui1_buffer, ui4_buff_size, TRUE);
        if(ui8_pcr_value_first != INVALID_VALUE)
        {
            break;
        }
        ui4_half_N_Mega++;
        if(ui4_half_N_Mega > CHK_MAX_DATA_NUM)
        {
            if(ui8_pcr_value_first == INVALID_VALUE)
            {
                ui8_pcr_value_first = 0;
            }
            break;
        }
    }
    if(pt_start_pcr != NULL)
    {
        if(ui8_pcr_value_first != INVALID_VALUE && ui8_pcr_value_first != 0)
        {
            *pt_start_pcr = ui8_pcr_value_first;
        }       
        x_mem_free(pui1_buffer);
        x_dbg_stmt("Ts_duration:  pt_start_pcr != NULL \n");
        return 0;
    }

    if(ui8_pcr_value_first == INVALID_VALUE)
    {
        x_mem_free(pui1_buffer);
        x_dbg_stmt("Ts_duration: The head of file has no valid time info\n");
        return 0;
    }

    ui4_half_N_Mega = 1;
    /* get last pcr value*/
    while(1)
    {
        x_memset(pui1_buffer, 0, ui4_buff_size);
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                            &pt_src_feeder->h_feeder,
                            (INT64)(-1)*ui4_buff_size*ui4_half_N_Mega,
                            MEDIA_SEEK_END);
        if (i4_ret != INPUTR_OK)
        {
            x_dbg_stmt("Ts_duration: pf_set_pos(%lld to end) fail\n", (INT64)(-1)*ui4_buff_size * ui4_half_N_Mega);
            break;
        }

        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                            &pt_src_feeder->h_feeder,
                            pui1_buffer,
                            ui4_buff_size,
                            ui4_buff_size,
                            &ui4_read);
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("Ts_duration: read from end fail ret = %d \n", i4_ret);
            break;
        }

        ui8_pcr_value_end = ui8_get_pcr_value(pui1_buffer, ui4_buff_size, FALSE);        
        if(ui8_pcr_value_end != INVALID_VALUE)
        {
            break;
        }
        ui4_half_N_Mega++;
        if(ui4_half_N_Mega > CHK_MAX_DATA_NUM)
        {
            break;
        }
    }

    x_mem_free(pui1_buffer);
    
    if(ui8_pcr_value_end == INVALID_VALUE)
    {
        x_dbg_stmt("Ts_duration: The rear of file has no valid time info\n");
        return 0;
    }
    ui8_calc_dur = (ui8_pcr_value_end - ui8_pcr_value_first) % 0x200000000;
    if(ui8_calc_dur != 0)
    {
        ui4_byte_per_sec = (UINT32)(_mm_div64(ui8_file_size*TS_PCR_FREQ, 
                                 ui8_calc_dur, 
                                 NULL));
    }
    
    if(ui4_byte_per_sec < TS_DUR_SIZE_RATIO || ui4_byte_per_sec > TS_DUR_SIZE_MAX_RATIO)
    {
        x_dbg_stmt("Ts duration:The duration is too bigger %lld for such file\n",ui8_calc_dur);
        return 0;
    }    
    else if(ui8_pcr_value_end >= ui8_pcr_value_first)
    {
        x_dbg_stmt("Ts duration: ui8_pcr_value_end = %lld, ui8_pcr_value_first = %lld \n",
                    ui8_pcr_value_end,
                    ui8_pcr_value_first);
        
        return (ui8_pcr_value_end - ui8_pcr_value_first);
    }
    else
    {
        x_dbg_stmt("Ts duration: ui8_pcr_value_end = %lld, ui8_pcr_value_first = %lld \n",
                    ui8_pcr_value_end ,
                    ui8_pcr_value_first);
        
        return (ui8_pcr_value_end + 0x200000000 - ui8_pcr_value_first);/*13818-1-2000 p30+ 0x1ffffffff */
    }
}


static UINT64 ui8_get_ts_duration(char * ps_filename,UINT64* pt_start_pcr)
{
    INT32           i4_ret              = 0;
    UINT32          ui4_read            = 0;
    UINT32          ui4_buff_size       = 0;
    UINT64          ui8_cur_pos         = 0;
    UINT64          ui8_file_size       = 0;
    UINT64          ui8_pcr_value_first = INVALID_VALUE;
    UINT64          ui8_pcr_value_end   = 0;
    UINT8*          pui1_buffer         = NULL;
    HANDLE_T        h_file              = NULL_HANDLE;
    UINT32          ui4_half_N_Mega     = 0;
    UINT64          ui8_calc_dur        = 0;
    UINT32          ui4_byte_per_sec    = 0;

    if(ps_filename == NULL)
    {
        return 0;
    }
    
    i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                        ps_filename,
                        FM_READ_ONLY, 
                        0777,
                        FALSE, 
                        &h_file);
    if(i4_ret != FMR_OK)
    {
        x_fm_close(h_file);
        return 0;
    }
                
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_END|FM_SEEK_CHK, &ui8_cur_pos);
    if(i4_ret != FMR_OK)
    {
   //x_dbg_stmt("[%s][%d][%d]\r\n", __func__, __LINE__,i4_ret);
        x_fm_close(h_file);    
        return 0;
    }
    ui8_file_size = ui8_cur_pos;
    
    /* get first pcr value*/
    if(ui8_file_size > TS_BUF_SIZE)
    {
        ui4_buff_size = TS_BUF_SIZE;
    }
    else
    {
        ui4_buff_size = (UINT32)ui8_file_size;
    }
    
    pui1_buffer = (UINT8*)x_mem_alloc(ui4_buff_size);
    if(pui1_buffer == NULL)
    {
        x_fm_close(h_file);
        x_mem_free(pui1_buffer);
        return 0;
    }

    /* get first pcr value*/
    ui4_half_N_Mega = 0;
    while(1)
    {
        x_memset(pui1_buffer, 0, ui4_buff_size);
        i4_ret = x_fm_lseek(h_file,  ui4_buff_size * ui4_half_N_Mega, FM_SEEK_BGN|FM_SEEK_CHK, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        {
         //x_dbg_stmt("[%s][%d][%d]\r\n", __func__, __LINE__,i4_ret);
            x_fm_close(h_file);
            x_mem_free(pui1_buffer);
            return 0;
        }
        i4_ret = x_fm_read(h_file, pui1_buffer, ui4_buff_size, &ui4_read);
        if(i4_ret != FMR_OK)
        {
            break;
        }
        ui8_pcr_value_first = ui8_get_pcr_value(pui1_buffer, ui4_buff_size, TRUE);
        if(ui8_pcr_value_first != INVALID_VALUE)
        {
            break;
        }
        ui4_half_N_Mega++;
        if(ui4_half_N_Mega > CHK_MAX_DATA_NUM)
        {
            if(ui8_pcr_value_first == INVALID_VALUE)
            {
                ui8_pcr_value_first = 0;
            }
            break;
        }
    }
    if(pt_start_pcr != NULL)
    {
        if(ui8_pcr_value_first != INVALID_VALUE && ui8_pcr_value_first != 0)
        {
            *pt_start_pcr = ui8_pcr_value_first;
        }       
        x_fm_close(h_file);
        x_mem_free(pui1_buffer);
        return 0;
    }

    if(ui8_pcr_value_first == INVALID_VALUE)
    {
        x_fm_close(h_file);
        x_mem_free(pui1_buffer);
        return 0;
    }

    ui4_half_N_Mega = 1;
    /* get last pcr value*/
    while(1)
    {
        x_memset(pui1_buffer, 0, ui4_buff_size);
        i4_ret = x_fm_lseek(h_file,  (INT64)(-1)*ui4_buff_size*ui4_half_N_Mega, FM_SEEK_END|FM_SEEK_CHK, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        {
        // x_dbg_stmt("[%s][%d][%d]\r\n", __func__, __LINE__,i4_ret);
            break;
        }
        i4_ret = x_fm_read(h_file, pui1_buffer, ui4_buff_size, &ui4_read);
        if(i4_ret != FMR_OK)
        {
            break;
        }
        ui8_pcr_value_end = ui8_get_pcr_value(pui1_buffer, ui4_buff_size, FALSE);        
        if(ui8_pcr_value_end != INVALID_VALUE)
        {
            break;
        }
        ui4_half_N_Mega++;
        if(ui4_half_N_Mega > CHK_MAX_DATA_NUM)
        {
            break;
        }
    }

    x_fm_close(h_file);
    x_mem_free(pui1_buffer);
    
    if(ui8_pcr_value_end == INVALID_VALUE)
    {
        return 0;
    }
    ui8_calc_dur = (ui8_pcr_value_end - ui8_pcr_value_first) % 0x200000000;
    if(ui8_calc_dur != 0)
    {
        ui4_byte_per_sec = (UINT32)(_mm_div64(ui8_file_size*TS_PCR_FREQ, 
                                 ui8_calc_dur, 
                                 NULL));
    }
    
    if(ui4_byte_per_sec < TS_DUR_SIZE_RATIO || ui4_byte_per_sec > TS_DUR_SIZE_MAX_RATIO)
    {
        x_dbg_stmt("Ts duration:The duration is too big or smal (%lld) for such file\n",ui8_calc_dur);
        return 0;
    }    
    else if(ui8_pcr_value_end >= ui8_pcr_value_first)
    {
        x_dbg_stmt("Ts duration: ui8_pcr_value_end = %lld, ui8_pcr_value_first = %lld \n",ui8_pcr_value_end,ui8_pcr_value_first);
        return (ui8_pcr_value_end - ui8_pcr_value_first);
    }
    else
    {
        x_dbg_stmt("Ts duration: ui8_pcr_value_end = %lld, ui8_pcr_value_first = %lld \n",ui8_pcr_value_end,ui8_pcr_value_first);
        return (ui8_pcr_value_end + 0x200000000 - ui8_pcr_value_first);/*13818-1-2000 p30+ 0x1ffffffff */
    }
}

INT32 ts_uop_hdlr_open (MEDIA_DESCRIBT_T*   pt_obj)
{
    return SVCTXR_OK;
}

INT32 ts_uop_hdlr_is_hdlr(MEDIA_DESCRIBT_T*     pt_obj,
                          MEDIA_FORMAT_T*       pt_favor_media_format)
{
    /*check paprameter*/
    if(pt_obj == NULL ||pt_favor_media_format ==NULL)
    {
        return SVCTXR_INV_ARG;
    }

    if (pt_favor_media_format->e_media_type == MEDIA_TYPE_TS
    && (pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_188
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_192
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_192_ZERO
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT_CMPB
    ||  pt_favor_media_format->t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_192_SYSTEM_B))
    {
        return SVCTXR_OK;
    }
    else
    {
        return SVCTXR_FAIL;
    }
}

INT32 ts_uop_hdlr_close (MEDIA_DESCRIBT_T*  pt_obj)
{
    return SVCTXR_OK;
}

INT32 ts_uop_hdlr_get_tm_code (MEDIA_DESCRIBT_T*            pt_obj,
                               MM_SVC_TIME_CODE_INFO_T*     pt_time_info)
{
    MM_SVC_TM_CODE_TYPE_T   e_tm_code_type;
    INT32                   i4_ret;
    
    /*check paprameter*/
    if (pt_obj == NULL ||pt_time_info ==NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    i4_ret = SVCTXR_OK;
    e_tm_code_type = pt_time_info->t_tm_code_type;
    switch(e_tm_code_type)
    {
    case MM_SVC_TM_CODE_TYPE_ALL:
        pt_time_info->ui8_time = 0;
        break;
        
    case MM_SVC_TM_CODE_TYPE_TITLE:
    case MM_SVC_TM_CODE_TYPE_CHAPTER:
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }
    return i4_ret;
}

INT32 ts_uop_hdlr_get_dur(MEDIA_DESCRIBT_T*         pt_obj, 
                          MM_SVC_DUR_INFO_T*        pt_time_dur)
{
    MM_SVC_DUR_TYPE_T   e_dur_type; 
    INT32               i4_ret;

    /*check paprameter*/
    if(pt_obj == NULL ||pt_time_dur ==NULL)
    {
        return SVCTXR_INV_ARG;
    }
    if(pt_obj->t_media_fmt.t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_188)
    {
        ui4_ts_packet_len = 188;
    }
    else
    {
        ui4_ts_packet_len = 192;
    }
    i4_ret = SVCTXR_OK;
    e_dur_type = pt_time_dur->t_dur_type;

    switch(e_dur_type)
    {
    case MM_SVC_DUR_TYPE_ALL:
        if(pt_obj->ui8_media_dur != 0 || pt_obj->b_have_duration)
        {
            pt_time_dur->ui8_duration = pt_obj->ui8_media_dur;
        }
        else if(pt_obj->pt_media_feeder == NULL)
        {
            pt_time_dur->ui8_duration = ui8_get_ts_duration(pt_obj->ps_media_path,NULL);
            pt_obj->ui8_media_dur = pt_time_dur->ui8_duration;
        }
        else
        {
            pt_time_dur->ui8_duration = ui8_get_ts_duration_with_feeder(pt_obj, NULL);
            pt_obj->ui8_media_dur = pt_time_dur->ui8_duration;
        }
        pt_obj->b_have_duration = TRUE;
        break;
    case MM_SVC_DUR_TYPE_TITLE:
    case MM_SVC_DUR_TYPE_CHAPTER:
    case MM_SVC_DUR_TYPE_BUFFERED:
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }
    return i4_ret;
}

INT32 ts_uop_handler_search_fct (MEDIA_DESCRIBT_T*      pt_obj,
                                 MM_SVC_POS_INFO_T*     pt_trick_info)
{
    return SVCTXR_OK;
}

INT32 ts_uop_handler_btn_fct (MEDIA_DESCRIBT_T*     pt_obj,                          
                              MM_SVC_NAV_INFO_T*    pt_btn_info)
{   
    if(pt_obj == NULL ||pt_btn_info ==NULL)
    {
        return SVCTXR_INV_ARG;
    }
    return SVCTXR_OK;
}

INT32 ts_uop_handler_trick_fct (MEDIA_DESCRIBT_T*       pt_obj, 
                                MM_SVC_TRICK_INFO_T*    pt_trick_info)
{
    return SVCTXR_OK;
}

INT32 ts_uop_handler_get_range_fct(MEDIA_DESCRIBT_T*                pt_obj ,
                                   UINT32                           ui4_start_pts, 
                                   MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info)
{
    UINT64    ui8_start_pcr = (UINT64)(-1);
    if(pt_obj->t_media_fmt.t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_188)
    {
        ui4_ts_packet_len = 188;
    }
    else
    {
        ui4_ts_packet_len = 192;
    }

    ui8_get_ts_duration(pt_obj->ps_media_path,&ui8_start_pcr);
        
    pt_range_info->u_range.t_es.ui4_start_pts = ui4_start_pts;
    pt_range_info->u_range.t_es.ui8_start_pcr = ui8_start_pcr;
    pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;

    return SVCTXR_OK;
}

