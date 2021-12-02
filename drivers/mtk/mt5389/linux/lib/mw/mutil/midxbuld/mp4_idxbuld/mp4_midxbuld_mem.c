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
 * $RCSfile: mp4_midxbuld_mem.c,v $
 * $Revision: #4 $ 1.0
 * $Date: 2013/01/18 $ 2004/11/26
 * $Author: huimiao.liu $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder public APIs  
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
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"
#include "dbg/x_dbg.h"
#include "mutil/mm_util.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "mp4_midxbuld.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"
#include "x_dbg.h"

#include "../../mm_util.h"

#include "x_lnk_list.h"
#include "mp4_midxbuld.h"

#endif

#if (ENABLE_CMOV == 1)
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MP4_KEY_LEN_OF_SAMPLE_NUM    100

#define ENABLE_CTTS 1
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
 * Name:  mp4_midxbuld_init_cmov_ctts_tbl
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_init_cmov_ctts_tbl(
                            MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                            MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                            MP4_IDXBULD_TRACK_T*        t_cur_track)
{
    UINT32                  ui4_tmp = 0;
    UINT32                  ui4_value = 0;
    UINT32                  ui4_entry_count = 0;
    MP4_RAW_CTTS_TBL_T*     pt_ctts_item_buf = NULL;
    UINT8*                  pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

    if((NULL == pt_midxbuld_obj) |(NULL == pt_key_ext_tbl)||(NULL == t_cur_track))
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)
                                (pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    /*get ctts entry count*/
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                        + (t_cur_track->t_ctts.ui8_box_offset + 12);
    LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
    pt_ctts_item_buf = (MP4_RAW_CTTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                sizeof(MP4_RAW_CTTS_TBL_T) * (ui4_entry_count+1), 
                                MM_MEM_MIDX_TAG);
    if(pt_ctts_item_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_ctts_item_buf, 0, sizeof(MP4_RAW_CTTS_TBL_T) * (ui4_entry_count+1));
    
    pt_key_ext_tbl->t_ctts_buf.ui4_ctts_entry_num = ui4_entry_count;
    pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf = pt_ctts_item_buf;

    pui1_cmov_data += 4;
    LOADB_DWRD(pui1_cmov_data, pt_ctts_item_buf[0].ui4_sample_count);
    LOADB_DWRD(pui1_cmov_data + 4, pt_ctts_item_buf[0].ui4_sample_offset);
    pui1_cmov_data += 8;
    for(ui4_tmp = 1; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        LOADB_DWRD(pui1_cmov_data, ui4_value);
        pt_ctts_item_buf[ui4_tmp].ui4_sample_count
                    = ui4_value + pt_ctts_item_buf[ui4_tmp-1].ui4_sample_count;
        LOADB_DWRD(pui1_cmov_data + 4, pt_ctts_item_buf[ui4_tmp].ui4_sample_offset);
        pui1_cmov_data += 8;
    }
    return MIDXBULDR_OK;
}

INT32 mp4_cmov_pcm_audio_sample_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MP4_IDXBULD_TRACK_T*    pt_cur_track)
{
    INT32                       i4_ret              = 0;
    UINT32                      ui4_tmp             = 0;
    UINT32                      ui4_entry_count     = 0;
    UINT32                      ui4_sample_count    = 0;
    UINT32                      ui4_cur_sample_num  = 0;
    UINT32                      ui4_i               = 0;
    MP4_RAW_STSC_TBL_T*         pr_stsc_buf         = NULL;
    MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T* pt_tmp_size_dur = NULL;
    UINT8*                      pui1_cmov_data      = NULL;
    MP4_BOX_CMOV_INFO_T*        pt_cmov_info        = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if((pt_midxbuld_obj == NULL) || (pt_cur_track == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;
    
    /* Check PCM data attribute first, only support sample_size==1 && sample_dur==1 case*/
    /* seek to STTS  box to get sample duration */
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                    + (pt_cur_track->t_stts.ui8_box_offset + 20);
    LOADB_DWRD(pui1_cmov_data, ui4_tmp);
    if(ui4_tmp != 1)/* Not PCM ??? */
    {
        return MIDXBULDR_INTERNAL_ERR;
    }    

    /* seek to STSZ  box to get sample size */
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                    + (pt_cur_track->t_stsz.ui8_box_offset + 12);
    LOADB_DWRD(pui1_cmov_data, ui4_tmp);
    if(ui4_tmp != 1)/* Not PCM ??? */
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    LOADB_DWRD(pui1_cmov_data + 4, ui4_sample_count);

    do
    {
        /* seek to STSC  box to get entry count */
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                        + (pt_cur_track->t_stsc.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_entry_count);

        /* Allocate temp buffer and read data to buffer*/
        pui1_cmov_data += 4;
        pr_stsc_buf = (MP4_RAW_STSC_TBL_T*)pui1_cmov_data;
        

        /* Reorder buffer data */
        pt_cur_track->t_pcm_sam_inf.ui4_size_dur_num = ui4_entry_count;
        pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur = 
            (MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T*)_mm_util_mem_alloc_with_tag(
                                                sizeof(MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T) * ui4_entry_count, 
                                                MM_MEM_MIDX_TAG);
        if (pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur == NULL)
        {
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break;
        }

        pt_tmp_size_dur = pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur;
        pt_tmp_size_dur[0].ui4_sample_start_idx     
            = LITTLE_TO_BIG_DWORD(pr_stsc_buf[0].ui4_first_chunk);
        pt_tmp_size_dur[0].ui4_sample_size_duration 
            = LITTLE_TO_BIG_DWORD(pr_stsc_buf[0].ui4_samples_per_chunk);
        for(ui4_i=1; ui4_i<ui4_entry_count; ui4_i++)
        {
            pt_tmp_size_dur[ui4_i].ui4_sample_start_idx     
                = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i].ui4_first_chunk);
            pt_tmp_size_dur[ui4_i-1].ui4_sample_count       
                = pt_tmp_size_dur[ui4_i].ui4_sample_start_idx - LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i-1].ui4_first_chunk);
            pt_tmp_size_dur[ui4_i].ui4_sample_size_duration 
                = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i].ui4_samples_per_chunk);
            ui4_cur_sample_num += pt_tmp_size_dur[ui4_i-1].ui4_sample_count
                                    *pt_tmp_size_dur[ui4_i-1].ui4_sample_size_duration;
        }        
        pt_tmp_size_dur[ui4_entry_count-1].ui4_sample_count
                = (ui4_sample_count - ui4_cur_sample_num)/pt_tmp_size_dur[ui4_entry_count-1].ui4_sample_size_duration;

        
        /* Get audio sample offset from STCO */        
        /* seek to STCO  box to get entry count */
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                        + (pt_cur_track->t_stco.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_entry_count);

        pui1_cmov_data += 4;

        pt_cur_track->t_pcm_sam_inf.ui4_total_sample_num= ui4_entry_count;
        pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf      = (UINT32*)pui1_cmov_data;
        for(ui4_i=0; ui4_i<ui4_entry_count; ui4_i++)
        {
            pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf[ui4_i]
                = LITTLE_TO_BIG_DWORD(pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf[ui4_i]);
        }        
    }while(0);

    if(i4_ret != MIDXBULDR_OK)
    {
        if(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur != NULL)
        {
            _mm_util_mem_free_with_tag(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur, 
                                       MM_MEM_MIDX_TAG);
            pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur = NULL;
        }
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_midxbuld_indexing
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_cmov_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                            UINT32*         pui4_range_id)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    UINT8                       ui1_idx;
    UINT32                      ui4_i, ui4_j;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_pre_entry = NULL;
    
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_t = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track = {0};
    
    UINT32 ui4_range_start_pts, ui4_range_end_pts;
    UINT32 ui4_range_start_sample, ui4_range_end_sample;
    UINT32 ui4_key_num, ui4_start_key_idx;
    
    UINT32 ui4_count_init, ui4_curr_pts;
    UINT32 ui4_sample_count, ui4_sample_duration;
    UINT64 ui8_time, ui8_curr_duration_sum;
    
    UINT32 ui4_chunk_count, ui4_entry_count;
    UINT32 ui4_curr_entry, ui4_curr_sample, ui4_curr_chunk, ui4_curr_chunk_sample;
    UINT32 ui4_first_chunk, ui4_next_first_chunk;
    UINT32 ui4_samples_per_chunk;
  
#if (ENABLE_CTTS == 1)
    UINT32              ui4_cur_sample_idx = 0;
    MP4_RAW_CTTS_TBL_T* pt_ctts_item_buf = NULL;
#endif

    UINT32              ui4_cmov_stts_offset = 0;
    UINT32              ui4_temp_value = 0;
    UINT8*                  pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;

    /* initial local variable */
    pt_tbl_obj = NULL;
    ui4_start_key_idx = 0;
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_mp4_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_mp4_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_mp4_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_mp4_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }

    ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;

    /* key table indexing */
    if (pt_mp4_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_tbl_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
        /* get related key ext table for key table */
        pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
        while (pt_key_ext_t != NULL)
        {
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id
                    == pt_key_ext_t->ui4_track_id)
            {
                break;
            }
            pt_key_ext_t = pt_key_ext_t->pt_next;
        }
        if (pt_key_ext_t == NULL)
        {
            if(ui4_key_num == 0)/*no key table exist*/
            {
                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
                pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
                pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
                return MIDXBULDR_OK;
            }
            else
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = ui4_key_num;  
        DBG_INFO(("indexing key number = %d \r\n", ui4_key_num));
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = pt_key_ext_t->ui8_base_offset;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
#if 0
        x_memcpy(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                 pt_key_ext_t->pt_key_entry, 
                 sizeof(MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_key_num);
#else
        _mp4_reorder_key_tbl(pt_tbl_obj, pt_key_ext_t);
#endif
        /* todo: need memory copy the entry to current range object?? */           
        return MIDXBULDR_OK;
    }

    /* non-key table indexing */
    pt_tbl_obj = pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj;
    while (pt_tbl_obj != NULL)
    {
        UINT32  ui4_ctts_i       = 0;
        if (pt_midxbuld_obj->b_abort_flag == TRUE)
        {
            return MIDXBULDR_ABORT_INDEXING;
        }
        if(pt_tbl_obj->ui4_tbl_size == 0)
        {
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        /* get range start / end pts */
        ui4_range_start_pts = pt_tbl_obj->ui4_start_pts;
        ui4_range_end_pts  =  pt_tbl_obj->ui4_end_pts;
        /* get range start / end frame(sample) */
        ui4_range_start_sample = pt_tbl_obj->ui4_start_sample;
        ui4_range_end_sample  = pt_tbl_obj->ui4_end_sample;
        /* get curr track info by pt_describ stream id */
        for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
        {
            if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id == 
                pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
            {
                 t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
                 break;
            }
        }
        if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        if((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.b_is_pcm_aud)
        {
            mp4_midxbuld_pcm_audio_sample_indexing( pt_midxbuld_obj,
                                                    pt_tbl_obj,
                                                    &t_cur_track); 
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        /* get related key ext table */
        pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
        while (pt_key_ext_t != NULL)
        {
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id
                    == pt_key_ext_t->ui4_track_id)
            {
                break;
            }
            pt_key_ext_t = pt_key_ext_t->pt_next;
        }
        if (pt_key_ext_t == NULL)
        {
            if(ui4_key_num != 0)
            {
                break;
            }
            return MIDXBULDR_INTERNAL_ERR;
        }
        ui4_key_num = pt_key_ext_t->ui4_key_num;
           
        /* cal start key ext index by start pts */
        for(ui4_i = ui4_key_num; ui4_i > 0; ui4_i--)
        {
            if (pt_key_ext_t->pt_key_entry[ui4_i-1].ui4_pts <= ui4_range_start_pts)
            {
                ui4_start_key_idx = ui4_i - 1;
                break;
            }
        }

        /* cal pts */
        if(ui4_key_num == 0)/*no key table exist*/
        {
            ui4_count_init = 0;
            ui4_curr_pts = 0;
            ui8_curr_duration_sum = 0;
            /* seek to 'time to sample'  box to cal pts */
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_stts.ui8_box_offset + 16);
            ui4_cmov_stts_offset = 16 ;
        }
        else
        {
            ui4_count_init
                = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample
                    - pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                        .ui4_stts_entry_sample;
            ui4_curr_pts = pt_key_ext_t->pt_key_entry[ui4_start_key_idx].ui4_pts;
            ui8_curr_duration_sum = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                    .ui8_key_ext_duration_sum;
            /* seek to 'time to sample'  box to cal pts */
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_stts.ui8_box_offset + 16
                            + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                .ui4_stts_entry);
            ui4_cmov_stts_offset = 16 + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_stts_entry;
        }
               
        /* init current entry */
        pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        
#if (ENABLE_CTTS == 1)
        if(t_cur_track.t_ctts.b_is_box_exist)
        {
            pt_ctts_item_buf = pt_key_ext_t->t_ctts_buf.pt_ctts_item_buf;
            ui4_cur_sample_idx = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                            .ui4_key_ext_sample;          
        }
#endif

        /* check the dividend can't equal zero */
        if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale == 0)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        
        while (ui4_curr_pts <= ui4_range_end_pts)
        {        
            if(ui4_cmov_stts_offset >= t_cur_track.t_stts.ui4_box_size)
            {
                break;
            }
            LOADB_DWRD(pui1_cmov_data, ui4_sample_count);
            LOADB_DWRD(pui1_cmov_data + 4, ui4_sample_duration);
            pui1_cmov_data += 8;
            ui4_cmov_stts_offset += 8;
           
            for(ui4_i = ui4_count_init; ui4_i < ui4_sample_count; ui4_i++)
            {
                ui8_time = ui8_curr_duration_sum * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
#if (ENABLE_CTTS == 1)
                if(t_cur_track.t_ctts.b_is_box_exist)
                {                                    
                    INT64   i8_ctts_ofst    = 0;
                    INT64   i8_cur_dur_sum  = 0;
                    for(;ui4_ctts_i < pt_key_ext_t->t_ctts_buf.ui4_ctts_entry_num; ui4_ctts_i ++)
                    {
                        if( ui4_cur_sample_idx <= pt_ctts_item_buf[ui4_ctts_i].ui4_sample_count)
                        {                    
                            break;                    
                        }
                    }                    
                    ui4_cur_sample_idx++;
                    
                    i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_ctts_i].ui4_sample_offset); 
                    i8_cur_dur_sum  = (INT64)ui8_curr_duration_sum;

                    if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
                    {
                        ui8_time = (UINT64)i8_cur_dur_sum;
                    }
                    else
                    {
                        ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
                    }
                        
                    ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                }
#endif
                ui4_curr_pts = (UINT32)_mm_div64(
                            ui8_time,
                            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale, 
                            NULL);                            
             
#if (ENABLE_CTTS == 1)/*To avoid last-N entry pts > end pts issue*/
                if(t_cur_track.t_ctts.b_is_box_exist == FALSE)
#endif
                {
                    if (ui4_curr_pts >= ui4_range_start_pts)
                    {
                        /* get current entry pts */
                        pt_idx_entry->ui4_pts = ui4_curr_pts;
                        pt_idx_entry++;
                        if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                            pt_tbl_obj->ui4_tbl_size))
                        {
                            break;
                        }
                    }
                }
                else
                {
                    /* get current entry ptes */
                    pt_idx_entry->ui4_pts = ui4_curr_pts;
                    pt_idx_entry++;
                    if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                        pt_tbl_obj->ui4_tbl_size))
                    {
                        break;
                    }
                }
                ui8_curr_duration_sum += (UINT64)ui4_sample_duration;
            }
            if (ui4_i < ui4_sample_count)
            {
                break;
            }
            ui4_count_init = 0;
        }
        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt)--*/

        /*caculate sample size*/
        if (t_cur_track.ui4_default_sample_size == 0)
        {
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_stsz.ui8_box_offset + 20
                            + 4*(ui4_range_start_sample-1));
        }
        
        for(ui4_i = 0; 
            ui4_i < (ui4_range_end_sample - ui4_range_start_sample + 1); 
            ui4_i++)
        {
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_i;
            if(pt_idx_entry == NULL)/*no key table exist*/
            {
                break;
            }
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                LOADB_DWRD(pui1_cmov_data + 4*ui4_i, pt_idx_entry->ui4_size);
            }
            else
            {
                pt_idx_entry->ui4_size = t_cur_track.ui4_default_sample_size;
            }     
        }
        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt)--*/

        /* cal sample offset */ 
        if(t_cur_track.t_stco.b_is_box_exist)
        {
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_stco.ui8_box_offset + 12);
        }
        else if(t_cur_track.t_co64.b_is_box_exist)
        {
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_co64.ui8_box_offset + 12);
        }
        /* get chunk count */  
        LOADB_DWRD(pui1_cmov_data, ui4_chunk_count);
        
        /* get sample to chunk entry count */
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                            + (t_cur_track.t_stsc.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
        if(ui4_key_num == 0)/*no key table exist*/
        {
            ui4_curr_entry = 0;
            ui4_curr_sample = 0;
        }
        else
        {
            ui4_curr_entry = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui4_stsc_entry;
            ui4_curr_sample = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui4_stsc_entry_sample;
        }
        ui4_curr_chunk_sample = ui4_curr_sample;
    
        while (ui4_curr_sample <= ui4_range_end_sample)
        {
            pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                 + (t_cur_track.t_stsc.ui8_box_offset + 16+ 12 * ui4_curr_entry);
            LOADB_DWRD(pui1_cmov_data, ui4_first_chunk);
            LOADB_DWRD(pui1_cmov_data + 4, ui4_samples_per_chunk);

            if (ui4_curr_entry < ui4_entry_count - 1)
            {
                LOADB_DWRD(pui1_cmov_data + 12, ui4_next_first_chunk);
            }
            else
            {
                ui4_next_first_chunk = ui4_chunk_count + 1;
            }

            for(ui4_curr_chunk = ui4_first_chunk; 
                ui4_curr_chunk < ui4_next_first_chunk; 
                ui4_curr_chunk++)
            {
                /*navy*/
                for(ui4_i = 0; ui4_i < ui4_samples_per_chunk; ui4_i++)
                {
                    if (pt_midxbuld_obj->b_abort_flag == TRUE)
                    {
                        return MIDXBULDR_ABORT_INDEXING;
                    }
                    if (ui4_curr_sample >= ui4_range_start_sample)
                    {
                        pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                        if(pt_idx_entry == NULL)/*no key table exist*/
                        {
                            break;
                        }
                        pt_idx_entry += (ui4_curr_sample - ui4_range_start_sample);
                        
                        if ((ui4_i == 0) || (ui4_curr_sample == ui4_range_start_sample))
                        {
                            UINT32  ui4_high = 0;
                            UINT32  ui4_low  = 0;
                            UINT64  ui8_tmp  = 0;
                            
                            /* seek to chunk offset box */
                            if(t_cur_track.t_stco.b_is_box_exist)
                            {
                                pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + (t_cur_track.t_stco.ui8_box_offset + 16
                                                + 4*(ui4_curr_chunk - 1));
                                LOADB_DWRD(pui1_cmov_data, ui4_low);
                            }
                            else if(t_cur_track.t_co64.b_is_box_exist)
                            {
                                pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + (t_cur_track.t_co64.ui8_box_offset + 16
                                                + 8*(ui4_curr_chunk - 1) + 0);
                                LOADB_DWRD(pui1_cmov_data, ui4_high);
                                LOADB_DWRD(pui1_cmov_data+4, ui4_low);
                            }
                            ui8_tmp = (((UINT64)ui4_high)<<32) | ui4_low;
                            if(((ui8_tmp & MP4_SUPER_BIG_MASK_1G_H) != 0) && 
                               (ui4_curr_sample == ui4_range_start_sample))
                            {
                                pt_tbl_obj->t_elmt_tbl.ui8_base_offset  = ui8_tmp&MP4_SUPER_BIG_MASK_1G_H;
                            }
                            pt_idx_entry->ui4_relative_offset = (UINT32)(ui8_tmp - pt_tbl_obj->t_elmt_tbl.ui8_base_offset);
                        }
                
                        if (ui4_i == 0)
                        {
                        }
                        else if (ui4_curr_sample == ui4_range_start_sample)
                        {
                            if (t_cur_track.ui4_default_sample_size == 0)
                            {
                                pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + (t_cur_track.t_stsz.ui8_box_offset + 20
                                                + 4*(ui4_curr_chunk_sample - 1));
                            }
                            for(ui4_j = 0; ui4_j < ui4_i; ui4_j++)
                            {
                                if (t_cur_track.ui4_default_sample_size == 0)
                                {
                                    LOADB_DWRD(pui1_cmov_data + 4*ui4_j, 
                                                ui4_temp_value);
                                    pt_idx_entry->ui4_relative_offset
                                                            += ui4_temp_value; 
                                }
                                else
                                {
                                    pt_idx_entry->ui4_relative_offset += 
                                        t_cur_track.ui4_default_sample_size;
                                }
                            }
                        }
                        else
                        {
                            pt_idx_pre_entry = pt_idx_entry - 1;
                            pt_idx_entry->ui4_relative_offset = 
                                        pt_idx_pre_entry->ui4_relative_offset
                                        + pt_idx_pre_entry->ui4_size;
                        }
                    }
                    ui4_curr_sample++;
                    if (ui4_curr_sample > ui4_range_end_sample)
                    {
                        ui4_curr_chunk = ui4_next_first_chunk;
                        break;
                    }
                }
                ui4_curr_chunk_sample += ui4_samples_per_chunk;
            }

            ui4_curr_entry++;
            if (ui4_curr_entry >= ui4_entry_count)
            {
                break;
            }
        }
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;
        DBG_INFO(("indexing ui4_number_of_entry = %d \r\n", 
                  pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry));
        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt) --*/       
       
        //if(pt_mp4_handler->ui1_curr_file_idx > 1)
        {
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
            for(ui4_i=0; ui4_i<pt_tbl_obj->ui4_tbl_size; ui4_i++)
            {
                pt_idx_entry->ui4_pts += pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;
                pt_idx_entry->ui4_relative_offset += pt_mp4_handler->t_cur_file_info.ui8_file_start_pos;
                pt_idx_entry++;            
            }
        }
#ifndef __KERNEL__
        x_dbg_stmt("mem indexing ui4_number_of_entry = %d \r\n", 
                  pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("***************** range info start*********************\r\n");
        x_dbg_stmt("start    pts: 0x%8x\r\n", pt_tbl_obj->ui4_start_pts);
        x_dbg_stmt("end      pts: 0x%8x\r\n", pt_tbl_obj->ui4_end_pts);
        x_dbg_stmt("start tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_pts);
        x_dbg_stmt("end  tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_pts);
        x_dbg_stmt("start sample: 0x%8x\r\n", pt_tbl_obj->ui4_start_sample);
        x_dbg_stmt("end   sample: 0x%8x\r\n", pt_tbl_obj->ui4_end_sample);
        x_dbg_stmt("start offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_relative_offset);
        x_dbg_stmt("end   offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_relative_offset);
        x_dbg_stmt("***************** range info end*********************\r\n");
        x_dbg_stmt("=====pts offset: %llu    pos offset:%llu =======\r\n", 
                    pt_mp4_handler->t_cur_file_info.ui8_file_start_pts,
                    pt_mp4_handler->t_cur_file_info.ui8_file_start_pos);
#endif
        pt_tbl_obj = pt_tbl_obj->pt_next;
    }

    pt_mp4_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_key_ext_cal_size
 *
 * Description: This function calculate key ext frame size
 *
 * Inputs:  pt_midxbuld_obj,     specify the media index builder object  
 *          pt_key_ext_tbl,      specifies key ext table
 *          t_cur_track,         specifies current track
 *
 * Outputs: pt_key_ext_tbl,      specifies key ext table
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
static INT32 mp4_cmov_key_ext_cal_size(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                        MP4_IDXBULD_TRACK_T         t_cur_track)
{
    UINT32              ui4_key_num;
    UINT32              ui4_key_sample_idx;
    UINT32              ui4_tmp;  
    UINT8*              pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;
    
    ui4_key_num = pt_key_ext_tbl->ui4_key_num;
    pui1_cmov_data += t_cur_track.t_stsz.ui8_box_offset + 20;
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        if (t_cur_track.ui4_default_sample_size == 0)
        {
            ui4_key_sample_idx
                        = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
            LOADB_DWRD(pui1_cmov_data + 4*(ui4_key_sample_idx - 1),
                        pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_size);
        }
        else
        {
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_size
                                        = t_cur_track.ui4_default_sample_size;
        }
    }
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_key_ext_cal_offset
 *
 * Description: This function calculate key ext frame offset
 *
 * Inputs:  pt_midxbuld_obj,     specify the media index builder object  
 *          pt_key_ext_tbl,      specifies key ext table
 *          t_cur_track,         specifies current track
 *
 * Outputs: pt_key_ext_tbl,      specifies key ext table
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
static INT32 mp4_cmov_key_ext_cal_offset(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                        MP4_IDXBULD_TRACK_T         t_cur_track)
{
    UINT32 ui4_key_num;
    UINT32 ui4_key_sample_idx;
    UINT32 ui4_entry_count;
    UINT32 ui4_chunk_count;
    UINT32 ui4_key_ext_chunk;
    UINT32 ui4_sample_offset_in_chunk;
    UINT32 ui4_next_first_chunk;
    UINT32 ui4_tmp, ui4_k, ui4_t;
    UINT32 ui4_temp_value;
    
    UINT8*                  pui1_cmov_data = NULL;
    UINT8*                  paui1_stco_buf = NULL;
    MP4_STSC_TBL_T*         pt_stsc_tbl = NULL;
    MP4_STSC_TBL_T*         pt_stsc_curr = NULL;
    MP4_STSC_TBL_T*         pt_stsc_prev = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;
    
    /*navy 080610 modified for VideoLen>AudioLen*/
    ui4_key_num = pt_key_ext_tbl->ui4_key_num;

    /* seek to 'sample to chunk' and 'chunk offset'  box to cal offset */
    if(t_cur_track.t_stco.b_is_box_exist)
    {
        pui1_cmov_data += (t_cur_track.t_stco.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_chunk_count);
    }
    else if(t_cur_track.t_co64.b_is_box_exist)
    {
        pui1_cmov_data += (t_cur_track.t_co64.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_chunk_count);
    }
    else
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    paui1_stco_buf = pui1_cmov_data + 4;

    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + t_cur_track.t_stsc.ui8_box_offset + 12;
    /* get sample to chunk entry count */
    LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
    pt_stsc_tbl = (MP4_STSC_TBL_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_STSC_TBL_T) * ui4_entry_count, 
                                        MM_MEM_MIDX_TAG);
    if (pt_stsc_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_stsc_tbl, 0, sizeof(MP4_STSC_TBL_T) * ui4_entry_count);

    pui1_cmov_data += 4;
    pt_stsc_curr = pt_stsc_tbl;
    pt_stsc_prev = pt_stsc_curr;
    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        LOADB_DWRD(pui1_cmov_data + 12 * ui4_tmp, 
                    pt_stsc_curr->ui4_first_chunk);
        LOADB_DWRD(pui1_cmov_data + 12 * ui4_tmp + 4, 
                    pt_stsc_curr->ui4_samples_per_chunk);
        LOADB_DWRD(pui1_cmov_data + 12 * ui4_tmp + 8, 
                    pt_stsc_curr->ui4_sample_description_index);
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            LOADB_DWRD(pui1_cmov_data + 12 * (ui4_tmp + 1), ui4_next_first_chunk);
        }
        else
        {
            ui4_next_first_chunk = ui4_chunk_count + 1;
        }
        pt_stsc_curr->ui4_repeat_count
                        = ui4_next_first_chunk - pt_stsc_curr->ui4_first_chunk;
        if (ui4_tmp == 0)
        {
            pt_stsc_curr->ui4_first_sample = 1;
        }
        else
        {
            pt_stsc_curr->ui4_first_sample = pt_stsc_prev->ui4_first_sample + 
                pt_stsc_prev->ui4_samples_per_chunk * pt_stsc_prev->ui4_repeat_count;
        }
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            pt_stsc_prev = pt_stsc_curr;
            pt_stsc_curr = pt_stsc_tbl + ui4_tmp + 1;
            pt_stsc_curr->ui4_first_chunk = ui4_next_first_chunk;
        }
    }    
    
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + t_cur_track.t_stsz.ui8_box_offset + 20;
    ui4_k = 0;
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        ui4_key_sample_idx = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
        for(; ui4_k < ui4_entry_count; ui4_k++)
        {
            if ((ui4_key_sample_idx >= pt_stsc_tbl[ui4_k].ui4_first_sample) &&
                (ui4_key_sample_idx < (pt_stsc_tbl[ui4_k].ui4_first_sample + 
                        pt_stsc_tbl[ui4_k].ui4_repeat_count * 
                            pt_stsc_tbl[ui4_k].ui4_samples_per_chunk)))
            {
                break;
            }
        }
        if(ui4_k >= ui4_entry_count)
        {
            ui4_k = ui4_entry_count - 1;
        }
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry = ui4_k;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry_sample
                                        = pt_stsc_tbl[ui4_k].ui4_first_sample;
        /* check the dividend can't equal zero */
        if(pt_stsc_tbl[ui4_k].ui4_samples_per_chunk == 0)
        {
            _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
            return MIDXBULDR_INITIAL_ERR;
        }
        ui4_key_ext_chunk = pt_stsc_tbl[ui4_k].ui4_first_chunk + 
                  (ui4_key_sample_idx - pt_stsc_tbl[ui4_k].ui4_first_sample)
                        / pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        ui4_sample_offset_in_chunk
                    = (ui4_key_sample_idx -pt_stsc_tbl[ui4_k].ui4_first_sample)
                        % pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        /* seek to chunk offset box */
        if(t_cur_track.t_stco.b_is_box_exist)
        {
            LOADB_DWRD(paui1_stco_buf + 4 * (ui4_key_ext_chunk - 1), 
                        pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset);
        }
        else if(t_cur_track.t_co64.b_is_box_exist)
        {
            UINT32      ui4_H = 0;
            UINT32      ui4_L = 0;
            LOADB_DWRD(paui1_stco_buf + 8 * (ui4_key_ext_chunk - 1),  ui4_H);
            LOADB_DWRD(paui1_stco_buf + 8 * (ui4_key_ext_chunk - 1)+4,ui4_L);
            
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset = (((UINT64)ui4_H)<<32) | ui4_L;
        }
        /* seek to sample size box */
        if (ui4_sample_offset_in_chunk > 0)
        {
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                for(ui4_t = 0; ui4_t < ui4_sample_offset_in_chunk; ui4_t++)
                {
                    LOADB_DWRD(pui1_cmov_data + 4*(ui4_key_sample_idx
                                    - ui4_sample_offset_in_chunk + ui4_t - 1),
                                ui4_temp_value);
                    pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset
                            += ui4_temp_value;
                }
            }
            else
            {
                pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset 
                           += t_cur_track.ui4_default_sample_size
                                        * ui4_sample_offset_in_chunk;
            }
        }
    }
    _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_midxbuld_key_ext_table_build
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_cmov_midxbuld_key_ext_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track = {0};
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl2 = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry = NULL;
    MP4_STTS_TBL_T*             pt_stts_tbl = NULL;
    UINT32 ui4_key_num;
    UINT32 ui4_key_sample_idx;
    UINT32 ui4_key_sample_pts;
    UINT32 ui4_entry_count;
    UINT32  ui4_i       = 0;
    UINT64 ui8_time;
    UINT64 ui8_tmp_duration;
    UINT32 ui4_tmp, ui4_k;
    INT32  i4_ret;
    UINT8  ui1_idx;
    UINT8  ui1_j;
    UINT32 ui4_last_ctts_idx                        = 0;
#if (ENABLE_CTTS == 1)
    MP4_RAW_CTTS_TBL_T* pt_ctts_item_buf = NULL;
#endif
    UINT8*                  pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;
    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;

    /* key table first */    
    ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;    
    for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_info->ui4_strm_id
                    == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
        {
            t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
            break;
        }
    }
    if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if(ui4_key_num == 0)/*no key table exist*/
    {
        if((pt_midxbuld_obj->t_src_info.ui1_total_strm_num ==1) &&
            !pt_midxbuld_obj->t_src_info.t_es_map[0].b_is_cur_strm_codec_spt)
        {
            return MIDXBULDR_FMT_NOT_SUPPORT;
        }
        pui1_cmov_data += (t_cur_track.t_stsz.ui8_box_offset + 16);
        LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
        ui4_key_num = ui4_entry_count / MP4_KEY_LEN_OF_SAMPLE_NUM + 1;
        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size = ui4_key_num;

        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist = 0;
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num, 
                    MM_MEM_MIDX_TAG);
        if (pt_idx_entry == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num);
        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl
                                        .u_fmt.t_es.pt_idx_entry = pt_idx_entry;
    
    }
    
    if (ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if (pt_mp4_handler->pt_key_ext_tbl != NULL)
    {
        _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_ext_tbl, MM_MEM_MIDX_TAG);
    }
    pt_mp4_handler->pt_key_ext_tbl = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                            MM_MEM_MIDX_TAG);
    if (pt_mp4_handler->pt_key_ext_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    
    x_memset(pt_mp4_handler->pt_key_ext_tbl, 
             0, 
             sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
    pt_key_ext_tbl = pt_mp4_handler->pt_key_ext_tbl;
    pt_key_ext_tbl->ui4_track_id = pt_key_info->ui4_strm_id;
    pt_key_ext_tbl->ui8_base_offset = 0;
    pt_key_ext_tbl->pt_next = NULL;
    /*navy 080610 modified for VideoLen>AudioLen*/
    pt_key_ext_tbl->ui4_key_num = ui4_key_num;
    pt_key_ext_tbl->pt_key_entry = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)
                                            *ui4_key_num, 
                                        MM_MEM_MIDX_TAG);
    pt_key_ext_tbl->pt_key_ext = (MP4_IDXBULD_KEY_EXT_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T)
                                            *ui4_key_num, 
                                        MM_MEM_MIDX_TAG);
    if ((pt_key_ext_tbl->pt_key_entry == NULL) 
        || (pt_key_ext_tbl->pt_key_ext == NULL))
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_ext_tbl->pt_key_entry, 
             0, 
             sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_key_num);
    x_memset(pt_key_ext_tbl->pt_key_ext, 
             0, 
             sizeof(MP4_IDXBULD_KEY_EXT_T)*ui4_key_num);
    /* seek to stss box to get key sample */
    if (t_cur_track.t_stss.b_is_box_exist)
    {
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                + (t_cur_track.t_stss.ui8_box_offset + 16);
    }
    
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        if(pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist != 0)
        {
            LOADB_DWRD(pui1_cmov_data + 4*ui4_tmp, 
                       pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample);
        }
        else
        {
            pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                            = ui4_tmp * MP4_KEY_LEN_OF_SAMPLE_NUM + 1;
        }
    }

    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                + (t_cur_track.t_stts.ui8_box_offset + 12);
    LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
    pt_stts_tbl = (MP4_STTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                    sizeof(MP4_STTS_TBL_T) * ui4_entry_count, 
                                    MM_MEM_MIDX_TAG);
    if (pt_stts_tbl == NULL)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_stts_tbl, 0, sizeof(MP4_STTS_TBL_T) * ui4_entry_count);

    pui1_cmov_data += 4;

    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        LOADB_DWRD(pui1_cmov_data + 8*ui4_tmp, 
                   pt_stts_tbl[ui4_tmp].ui4_sample_count);
        LOADB_DWRD(pui1_cmov_data + 8*ui4_tmp + 4, 
                   pt_stts_tbl[ui4_tmp].ui4_sample_duration);
        if (ui4_tmp == 0)
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
        }
        else
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                    pt_stts_tbl[ui4_tmp-1].ui4_starting_sample
                                    + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                            pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                            (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                                * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
        }
    }
    
    for(ui1_idx = 0; 
        ui1_idx < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; 
        ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_ext_tbl->ui4_track_id
                    == pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
        {
            break; /* get related source index */
        } 
    }
        
    /*caculate ctts start*/   
    if(t_cur_track.t_ctts.b_is_box_exist)
    {
        if(mp4_midxbuld_init_cmov_ctts_tbl(pt_midxbuld_obj, 
                                      pt_key_ext_tbl, 
                                      &t_cur_track) == MIDXBULDR_OK)
        {
            pt_ctts_item_buf = pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf;
        }
        else
        {
            return MIDXBULDR_INITIAL_ERR;
        }
    }
    
    ui4_k = 0;
    /* check the dividend can't equal zero */
    if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
    {
        return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
    }
    if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale == 0)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_INITIAL_ERR;
    }
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        ui4_key_sample_idx = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
        for(; ui4_k < ui4_entry_count; ui4_k++)
        {
            if ((ui4_key_sample_idx >= pt_stts_tbl[ui4_k].ui4_starting_sample) &&
                (ui4_key_sample_idx < (pt_stts_tbl[ui4_k].ui4_starting_sample
                                        + pt_stts_tbl[ui4_k].ui4_sample_count)))
                {
                    break;
                }
        }
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stts_entry = ui4_k;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stts_entry_sample
                                = pt_stts_tbl[ui4_k].ui4_starting_sample;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum
                  = pt_stts_tbl[ui4_k].ui8_starting_total_duration + 
                    (UINT64)(ui4_key_sample_idx - pt_stts_tbl[ui4_k].ui4_starting_sample)
                        *pt_stts_tbl[ui4_k].ui4_sample_duration;
        ui8_time = (UINT64)MIDXBULD_SYSTEM_CLOCK_FREQUENCY
                    * pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
#if (ENABLE_CTTS == 1)      
        if(t_cur_track.t_ctts.b_is_box_exist)
        {            
            INT64   i8_ctts_ofst    = 0;
            INT64   i8_cur_dur_sum  = 0;
            for(ui4_i=ui4_last_ctts_idx; ui4_i < pt_key_ext_tbl->t_ctts_buf.ui4_ctts_entry_num; ui4_i ++)
            {
                if( ui4_key_sample_idx <= pt_ctts_item_buf[ui4_i].ui4_sample_count)
                {                    
                    ui4_last_ctts_idx = ui4_i;   
                    break;                    
                }
            } 
            i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_i].ui4_sample_offset); 
            i8_cur_dur_sum  = (INT64)pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
            
            if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
            {
                ui8_time = (UINT64)i8_cur_dur_sum;
            }
            else
            {
                ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
            }
                
            ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        }
#endif
        pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_pts = 
                        (UINT32)_mm_div64(ui8_time,
                                          pt_midxbuld_obj->t_src_info
                                                    .t_es_map[ui1_idx].ui4_scale, 
                                          NULL);  

    }
    _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
    
    /* cal sample size */
    mp4_cmov_key_ext_cal_size(pt_midxbuld_obj, pt_key_ext_tbl, t_cur_track);

    /* cal offset */
    i4_ret = mp4_cmov_key_ext_cal_offset( pt_midxbuld_obj, pt_key_ext_tbl, t_cur_track);
    if (i4_ret != MIDXBULDR_OK)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return i4_ret;
    }
    
    /* other key extention table */
    for(ui1_j = 0; ui1_j < pt_mp4_handler->ui1_valid_track_cnt; ui1_j++)
    {
        if(ui1_j >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_info->ui4_strm_id
                        == pt_mp4_handler->t_track_info_box[ui1_j].ui4_track_id)
        {
            continue;
        }
        else
        {
            t_cur_track = pt_mp4_handler->t_track_info_box[ui1_j];
        }
        
        for(ui4_k=0; ui4_k<pt_mp4_handler->ui1_valid_track_cnt; ui4_k++)
        {
            if(pt_midxbuld_obj->t_src_info.t_es_map[ui4_k].ui4_strm_id == t_cur_track.ui4_track_id)
            {
                break;
            }
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui4_k].b_is_pcm_aud)
        {
            i4_ret = mp4_cmov_pcm_audio_sample_table_build( pt_midxbuld_obj,
                                                            &pt_mp4_handler->t_track_info_box[ui1_j]);
            if(i4_ret != MIDXBULDR_OK)
            {
            }
            continue;
        }
        
        pt_key_ext_tbl2 = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                    _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                                MM_MEM_MIDX_TAG);
        if (pt_key_ext_tbl2 == NULL)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl2, 0, sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
        pt_key_ext_tbl2->ui4_track_id = t_cur_track.ui4_track_id;
        pt_key_ext_tbl2->ui8_base_offset = 0;
        /*navy 080610 modified for VideoLen>AudioLen*/
        pt_key_ext_tbl2->ui4_key_num = ui4_key_num;
        pt_key_ext_tbl2->pt_next = NULL;
        pt_key_ext_tbl2->pt_key_entry = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_key_num, 
                                        MM_MEM_MIDX_TAG);
        pt_key_ext_tbl2->pt_key_ext = (MP4_IDXBULD_KEY_EXT_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T)*ui4_key_num, 
                                        MM_MEM_MIDX_TAG);
        if ((pt_key_ext_tbl2->pt_key_entry == NULL)
            || (pt_key_ext_tbl2->pt_key_ext == NULL))
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl2->pt_key_entry, 
                 0, 
                 sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num);
        x_memset(pt_key_ext_tbl2->pt_key_ext, 
                 0, 
                 sizeof(MP4_IDXBULD_KEY_EXT_T) * ui4_key_num);
        
        if(t_cur_track.t_ctts.b_is_box_exist)
        {
            if(mp4_midxbuld_init_cmov_ctts_tbl(pt_midxbuld_obj, 
                                                pt_key_ext_tbl2, 
                                                &t_cur_track) == MIDXBULDR_OK)
            {
                pt_ctts_item_buf = pt_key_ext_tbl2->t_ctts_buf.pt_ctts_item_buf;
            }
            else
            {
                return MIDXBULDR_INITIAL_ERR;
            }
        }
        pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                + (t_cur_track.t_stts.ui8_box_offset + 12);
        LOADB_DWRD(pui1_cmov_data, ui4_entry_count);
        pt_stts_tbl = (MP4_STTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                    sizeof(MP4_STTS_TBL_T) * ui4_entry_count, 
                                    MM_MEM_MIDX_TAG);
        if (pt_stts_tbl == NULL)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_stts_tbl, 0, sizeof(MP4_STTS_TBL_T) * ui4_entry_count);

        pui1_cmov_data += 4;
        for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
        {
            LOADB_DWRD(pui1_cmov_data + 8*ui4_tmp, 
                    pt_stts_tbl[ui4_tmp].ui4_sample_count);
            LOADB_DWRD(pui1_cmov_data + 8*ui4_tmp + 4, 
                    pt_stts_tbl[ui4_tmp].ui4_sample_duration);
            if (ui4_tmp == 0)
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
            }
            else
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                pt_stts_tbl[ui4_tmp-1].ui4_starting_sample 
                                    + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                    pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                    (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                        * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
            }
        }

        for(ui1_idx = 0; 
            ui1_idx < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; 
            ui1_idx++)
        {
            if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
            if (pt_key_ext_tbl2->ui4_track_id
                    == pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
            {
                break; /* get related source index */
            } 
        }
        ui4_k = 0;
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        /* check the dividend can't equal zero */
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale == 0)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
            return MIDXBULDR_INITIAL_ERR;
        }
        for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
        {
            /* get key pts first */
            ui4_key_sample_pts
                = pt_mp4_handler->pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_pts;
            ui8_time = (UINT64)ui4_key_sample_pts
                        * pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale;
            ui8_tmp_duration = (UINT32)_mm_div64(ui8_time,
                                                MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                NULL);                            
            
            for(; ui4_k < ui4_entry_count; ui4_k++)
            {
                if ((ui8_tmp_duration >= pt_stts_tbl[ui4_k].ui8_starting_total_duration) 
                    && (ui8_tmp_duration < (pt_stts_tbl[ui4_k].ui8_starting_total_duration
                                            + (UINT64)pt_stts_tbl[ui4_k].ui4_sample_count
                                            * pt_stts_tbl[ui4_k].ui4_sample_duration)))
                    {
                        break;
                    }
            }
            if(ui4_k >= ui4_entry_count)
            {
                /*ui4_k = ui4_entry_count - 1;*/
                /*navy 080610 modified for VideoLen>AudioLen*/
                pt_key_ext_tbl2->ui4_key_num = ui4_tmp;
                break;
            }
            
            /* check the dividend can't equal zero */
            if(pt_stts_tbl[ui4_k].ui4_sample_duration == 0)
            {
                _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
                _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
                return MIDXBULDR_INITIAL_ERR;
            }
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                    = pt_stts_tbl[ui4_k].ui4_starting_sample
                        + (UINT32)_mm_div64(ui8_tmp_duration - pt_stts_tbl[ui4_k].ui8_starting_total_duration,
                                            pt_stts_tbl[ui4_k].ui4_sample_duration, 
                                            NULL);
            ui4_key_sample_idx
                        = pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_stts_entry = ui4_k;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_stts_entry_sample
                        = pt_stts_tbl[ui4_k].ui4_starting_sample;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum
                        = pt_stts_tbl[ui4_k].ui8_starting_total_duration + 
                            (UINT64)(ui4_key_sample_idx-pt_stts_tbl[ui4_k].ui4_starting_sample)
                                *pt_stts_tbl[ui4_k].ui4_sample_duration;
            ui8_time = (UINT64)MIDXBULD_SYSTEM_CLOCK_FREQUENCY
                    * pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
#if (ENABLE_CTTS == 1)      
            if(t_cur_track.t_ctts.b_is_box_exist)
            {  
                UINT32  ui4_i       = 0;
                INT64   i8_ctts_ofst    = 0;
                INT64   i8_cur_dur_sum  = 0;
                
                for(ui4_i = 0; ui4_i < pt_key_ext_tbl2->t_ctts_buf.ui4_ctts_entry_num; ui4_i ++)
                {
                    if( ui4_key_sample_idx <= pt_ctts_item_buf[ui4_i].ui4_sample_count)
                    {                    
                        break;                    
                    }
                } 
                i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_i].ui4_sample_offset); 
                i8_cur_dur_sum  = (INT64)pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
                if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
                {
                    ui8_time = (UINT64)i8_cur_dur_sum;
                }
                else
                {
                    ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
                }
                    
                ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
            }
#endif
            pt_key_ext_tbl2->pt_key_entry[ui4_tmp].ui4_pts = 
                (UINT32)_mm_div64(ui8_time,
                                  pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale, 
                                  NULL);                            

            
        }
        _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);

        /* cal sample size */
        mp4_cmov_key_ext_cal_size(pt_midxbuld_obj, pt_key_ext_tbl2, t_cur_track);
        /* cal sample offset */
        i4_ret = mp4_cmov_key_ext_cal_offset(pt_midxbuld_obj, 
                                        pt_key_ext_tbl2, 
                                        t_cur_track);
        if (i4_ret != MIDXBULDR_OK)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return i4_ret;
        }
        pt_key_ext_tbl->pt_next = pt_key_ext_tbl2;
        pt_key_ext_tbl = pt_key_ext_tbl->pt_next;
    }
    
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_midxbuld_key_tbl_size_estimate
 *
 * Description: This function estimate key table size
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies the index builder object
 *          ui4_key_id ,        specifies the key track id
 *
 * Outputs: NULL 
 *
 * Returns: 
 *          value,   key table size
 ----------------------------------------------------------------------------*/
static UINT32 mp4_cmov_midxbuld_key_tbl_size_estimate(
                              MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                              UINT32            ui4_key_id)
{
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;
    MP4_IDXBULD_TRACK_T     t_key_track = {0};
    UINT32                  ui4_size = 0;
    UINT8                   ui1_idx;

    UINT8*                  pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return 0;
    }    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return 0;
    }
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return 0;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;

    for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return 0;
        }
        if (ui4_key_id == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
        {
            t_key_track = pt_mp4_handler->t_track_info_box[ui1_idx];
            break;
        }
    }
    if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
    {
        return 0;
    }
    if (!t_key_track.t_stss.b_is_box_exist)
    {
        return 0;
    }
    /*4 byte box size + 4 byte type + 4 byte ver&flag = 12 */
    pui1_cmov_data += (t_key_track.t_stss.ui8_box_offset + 12);
    LOADB_DWRD(pui1_cmov_data, ui4_size);
    
    return ui4_size;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_cmov_midxbuld_elmt_tbl_size_estimate
 *
 * Description: This function estimate range table size
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies the index builder object
 *          pt_range_obj ,      specifies the range object
 *          pt_curr_tbl_obj ,   specifies current element table object
 *          pt_describ ,        specifies the element table description
 *
 * Outputs: NULL 
 *
 * Returns: 
 *          value,   key table size
 ----------------------------------------------------------------------------*/
UINT32 mp4_cmov_midxbuld_elmt_tbl_size_estimate(
                              MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                              MP4_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                              MP4_IDXBULD_ELMT_TBL_OBJ_T*  pt_curr_tbl_obj,
                              MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_t = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track = {0};
    UINT32 ui4_key_num = 0;
    UINT32 ui4_start_sample = (UINT32)0xFFFFFFFF;
    UINT32 ui4_end_sample = 0;
    UINT32 ui4_i = 0;
    UINT32 ui4_tbl_size = 0;
    UINT8  ui1_idx;   
    UINT8*                  pui1_cmov_data = NULL;
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    
    if (pt_range_obj->u_range.t_es.ui4_start_pts
        >= pt_range_obj->u_range.t_es.ui4_end_pts)
    {
        return 0;
    }
    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    if(!pt_cmov_info->b_is_cmov_box_exist)
    {
        return 0;
    }
    pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data;
    
   
    switch (pt_describ->u_info.t_es.e_es_type)
    {
    case MIDXBULD_ELEM_STRM_TYPE_AUDIO:
    case MIDXBULD_ELEM_STRM_TYPE_VIDEO:
    case MIDXBULD_ELEM_STRM_TYPE_SUBTITLE:
        {
            /* get curr track info by pt_describ stream id */
            for(ui1_idx = 0; 
                ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; 
                ui1_idx++)
            {
                if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
                {
                    return 0;
                }
                if (pt_describ->u_info.t_es.ui4_strm_id 
                    == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
                {
                     t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
                     break;
                }
            }
            if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
            {
                return 0;
            }
            /* get related key ext table */
            pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
            while (pt_key_ext_t != NULL)
            {
                if (pt_describ->u_info.t_es.ui4_strm_id
                                        == pt_key_ext_t->ui4_track_id)
                {
                    break;
                }
                pt_key_ext_t = pt_key_ext_t->pt_next;
            }
            if((pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_describ->u_info.t_es.b_is_pcm_aud)
            {
            }
            else
            {
                if (pt_key_ext_t == NULL)
                {
                    return 0;
                }

                /* cal start key ext index by range start pts */
                ui4_key_num = pt_key_ext_t->ui4_key_num;
            }
        
            if(pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO 
                || pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_SUBTITLE)
            {
                for(ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
                {
                    if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                > pt_range_obj->u_range.t_es.ui4_start_pts)
                    {
                        if(ui4_i > 0)
                        {
                            ui4_i -= 1;/* Get previous key idx, if first one, just get first*/
                        }
                    }
                    else if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                == pt_range_obj->u_range.t_es.ui4_start_pts)
                    {/*do nothing with the ui4_i*/
                    }
                    else
                    {
                        continue;
                    }
                    ui4_start_sample = pt_key_ext_t->pt_key_ext[ui4_i]
                                                        .ui4_key_ext_sample;
                    pt_range_obj->u_range.t_es.ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts;
                    pt_curr_tbl_obj->ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts;
                    break;
                }
                if(((UINT32)0xFFFFFFFF) == ui4_start_sample)
                {
                    ui4_start_sample
                        = pt_key_ext_t->pt_key_ext[ui4_key_num - 1].ui4_key_ext_sample;
                    pt_curr_tbl_obj->ui4_start_pts
                        = pt_key_ext_t->pt_key_entry[ui4_key_num - 1].ui4_pts;
                    pt_range_obj->u_range.t_es.ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_key_num - 1].ui4_pts;
                }

                /* Get end sample index&pts */
                for(; ui4_i < ui4_key_num; ui4_i++)
                {
                    if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                        >= pt_range_obj->u_range.t_es.ui4_end_pts)
                    {
                        ui4_end_sample = pt_key_ext_t->pt_key_ext[ui4_i]
                                                        .ui4_key_ext_sample - 1;
                        pt_range_obj->u_range.t_es.ui4_end_pts
                                    = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts - 1;
                        break;
                    }
                }
            }
            else if((pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_describ->u_info.t_es.b_is_pcm_aud)
            {
                UINT32  ui4_sample_duration = 0;
                UINT32  ui4_target_pts_idx  = 0;
                UINT64  ui8_time            = 0;

                /* Start sample index */
                ui8_time = pt_range_obj->u_range.t_es.ui4_start_pts;
                ui8_time *= pt_describ->u_info.t_es.ui4_scale;
                ui4_target_pts_idx = (UINT32)_mm_div64(ui8_time,
                                                       MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                       NULL);
                
                /* cal start key ext index by start pts */
                for(ui4_i = 0; ui4_i < t_cur_track.t_pcm_sam_inf.ui4_size_dur_num; ui4_i++)
                {
                    if(ui4_sample_duration + t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
                                 >= ui4_target_pts_idx)
                    {
                        ui4_start_sample = t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                            + (ui4_target_pts_idx - ui4_sample_duration)/t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
                        break;
                    }
                    else
                    {
                        ui4_sample_duration += t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count;
                    }
                }

                /* End sample index */
                if(pt_range_obj->u_range.t_es.ui4_end_pts == MIDXBULD_IDX_RANGE_FULL)
                {
                    ui8_time = pt_midxbuld_obj->t_src_info.ui4_total_duration;
                }
                else
                {
                    ui8_time = pt_range_obj->u_range.t_es.ui4_end_pts;
                }
                ui8_time *= pt_describ->u_info.t_es.ui4_scale;
                ui4_target_pts_idx = (UINT32)_mm_div64(ui8_time,
                                                       MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                       NULL);
                
                /* cal start key ext index by start pts */
                for(; ui4_i < t_cur_track.t_pcm_sam_inf.ui4_size_dur_num; ui4_i++)
                {
                    if(ui4_sample_duration + t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
								   >= ui4_target_pts_idx ||
                        ((t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i + 1].ui4_sample_size_duration != 0) &&
						((ui4_target_pts_idx - ui4_sample_duration - t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
								   t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count)/
								   t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i + 1].ui4_sample_size_duration == 0))) 
                    {
                        ui4_end_sample = t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                            + (ui4_target_pts_idx - ui4_sample_duration)/t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
                        if (0 == (ui4_target_pts_idx - ui4_sample_duration)%t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration)
                        {
                            ui4_end_sample -= 1;
                        }
                        break;
                    }
                    else
                    {
                        ui4_sample_duration += t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count;
                    }
                }
                if((ui4_end_sample == 0) && (ui4_end_sample >= t_cur_track.t_pcm_sam_inf.ui4_total_sample_num))
                {
                    ui4_end_sample = t_cur_track.t_pcm_sam_inf.ui4_total_sample_num;
                }
                pt_curr_tbl_obj->ui4_start_pts  = pt_range_obj->u_range.t_es.ui4_start_pts;
                pt_curr_tbl_obj->ui4_end_pts    = pt_range_obj->u_range.t_es.ui4_end_pts;
            }
            else if(pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
            {
                UINT32  ui4_start_key_idx   = (UINT32)~0;
                UINT32  ui4_count_init      = 0; 
                UINT32  ui4_curr_pts        = 0;
                UINT32  ui4_last_pts        = 0;
                UINT32  ui4_sample_count    = 0;
                UINT32  ui4_sample_duration = 0;
                UINT64  ui8_time            = 0;
                UINT64  ui8_curr_duration_sum = 0;
                UINT32  ui4_cmov_stts_offset = 0;
                UINT32  ui4_cur_sample_idx  = 0;
                
                /* cal start key ext index by start pts */
                for(ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
                {
                    if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                >= pt_range_obj->u_range.t_es.ui4_start_pts)
                    {
                        if(ui4_i == 0)
                        {
                            ui4_start_key_idx = 0;
                        }
                        else
                        {
                            ui4_start_key_idx = ui4_i - 1;
                        }
                        break;
                    }
                }

                if(ui4_start_key_idx   == (UINT32)~0)/*Vlen > ALen*/
                {
                    ui4_start_key_idx = ui4_key_num - 1;
                }

                /* cal pts */
                ui4_count_init
                    = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample
                        - pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                            .ui4_stts_entry_sample;
                ui4_curr_pts = pt_key_ext_t->pt_key_entry[ui4_start_key_idx].ui4_pts;
                ui8_curr_duration_sum = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui8_key_ext_duration_sum;
                /* seek to 'time to sample'  box to cal pts */
                pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + (INT64)(t_cur_track.t_stts.ui8_box_offset + 16
                                + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                    .ui4_stts_entry);    
                ui4_cmov_stts_offset = 16 + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_stts_entry;
                
                /* check the dividend can't equal zero */
                if(pt_describ->u_info.t_es.ui4_scale == 0)
                {
                    return 0;
                }                
                ui4_cur_sample_idx = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample;
                while (ui4_curr_pts <= pt_range_obj->u_range.t_es.ui4_end_pts)
                {
                    if(ui4_cmov_stts_offset >= t_cur_track.t_stts.ui4_box_size)
                    {
                        break;
                    }
                    LOADB_DWRD(pui1_cmov_data, ui4_sample_count);
                    LOADB_DWRD(pui1_cmov_data + 4, ui4_sample_duration);
                    pui1_cmov_data += 8;
                    ui4_cmov_stts_offset += 8;

                    for(ui4_i = ui4_count_init; ui4_i < ui4_sample_count; ui4_i++)
                    {
                        ui8_time = ui8_curr_duration_sum * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;

                        ui4_curr_pts = (UINT32)_mm_div64(ui8_time,
                                                         pt_describ->u_info.t_es.ui4_scale, 
                                                         NULL);                            

                        if (ui4_curr_pts > pt_range_obj->u_range.t_es.ui4_end_pts)
                        {
                            ui4_end_sample = ui4_cur_sample_idx - 1;
                            pt_curr_tbl_obj->ui4_end_pts = ui4_curr_pts;
                            break;
                        }
                        if((ui4_curr_pts >= pt_range_obj->u_range.t_es.ui4_start_pts)
                            && (ui4_start_sample == 0xFFFFFFFF))
                        {
                            ui4_start_sample = ui4_cur_sample_idx;
                            pt_curr_tbl_obj->ui4_start_pts = ui4_last_pts;
                        }      
                        ui4_cur_sample_idx++;                  
                        ui4_last_pts = ui4_curr_pts;
                        ui8_curr_duration_sum += (UINT64)ui4_sample_duration;
                    }
                    
                    if (ui4_i < ui4_sample_count)
                    {
                        break;
                    }
                    ui4_count_init = 0;
                }
            }

#if 1
            if(ui4_end_sample == 0)
            {   
                MP4_MULTI_FILE_INFO_T*  pt_file_info    = NULL;

                pt_file_info = pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info;
                
                pui1_cmov_data = pt_cmov_info->pui1_decomp_cmov_data
                                    + (t_cur_track.t_stsz.ui8_box_offset + 16);
                LOADB_DWRD(pui1_cmov_data, ui4_end_sample);

                if(pt_file_info->ui1_file_num == pt_mp4_handler->ui1_curr_file_idx+1)
                {
                    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
                }
                else
                {
                    pt_range_obj->u_range.t_es.ui4_end_pts
                        = /*pt_mp4_handler->t_cur_file_info.ui8_file_start_pts
                            + */pt_mp4_handler->t_cur_file_info.ui8_file_duration;
                }    
            }
#endif
            pt_curr_tbl_obj->ui4_end_pts = pt_range_obj->u_range.t_es.ui4_end_pts;

            /* cal table size */
            pt_curr_tbl_obj->ui4_start_sample = ui4_start_sample;
            pt_curr_tbl_obj->ui4_end_sample = ui4_end_sample;
            
            ui4_tbl_size = ui4_end_sample - ui4_start_sample + 1;
            #ifndef __KERNEL__
            if(ui4_start_sample > ui4_end_sample)
            {
                x_dbg_stmt("!!!! ui4_start_sample > ui4_end_sample !!!!\r\n");
                ui4_tbl_size = 0;
            }
            if(ui4_tbl_size > 2000)
            {
                x_dbg_stmt("!!!! NOTICE: ui4_tbl_size=%d >2000, maybe error !!!!\r\n", ui4_tbl_size);
            }
            #endif
        }
        break;

    case MIDXBULD_ELEM_STRM_TYPE_KEY:
        {
            ui4_tbl_size = mp4_cmov_midxbuld_key_tbl_size_estimate(pt_midxbuld_obj, 
                                            pt_describ->u_info.t_es.ui4_strm_id);
        }
        break;

    default:
        {
            ui4_tbl_size = 0;
        }
        break;
    }

    return ui4_tbl_size;
}
#endif

