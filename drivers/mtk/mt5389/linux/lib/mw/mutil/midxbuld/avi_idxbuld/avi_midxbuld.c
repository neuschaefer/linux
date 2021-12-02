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
 * $RCSfile: avi_midxbuld.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
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
#include "mutil/midxbuld/avi_idxbuld/avi_midxbuld.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"

#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../../mm_util_input.h"
#include "avi_midxbuld.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_util.h"
#include "../../mm_util.h"

#endif

#define VISUAL_TBL_FRM_CNT         100
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
const static UINT32 cui4_idx1 = 0x31786469; /* "idx1" */
#if 0
const static UINT32 cui4_movi = 0x69766f6d; /* "movi" */
const static UINT16 cui2_uncompress_vdo_frame = 0x6264; /* "db"(Uncompressed video frame) */
const static UINT16 cui2_compress_vdo_frame = 0x6364; /* "dc"(Uncompressed video frame) */
const static UINT16 cui2_audio_data = 0x6277; /* "wb"(Audio data) */
const static UINT16 cui2_subtitle_frame = 0x6273; /* "sb"(Subtitle frame (bitmap mode)) */
const static UINT16 cui2_drm_key_info_for_vdo = 0x6464; /* "dd"(DRM key info for the video frame) */
#endif
const static UINT32 cui4_aviif_keyframe = 0x00000010; /* AVIIF_KEYFRAME */

/* AVI2.0 standard Index Chunk Keyframe flag */
/* bit31 set for NON-Key */
const static UINT32 cui4_aviif_std_index_keyframe = (UINT32)0x80000000; 

static UINT8        ui1_idx_range_id_last = 1;

#define AVI_MIDXBULD_RANGE_ID_MASK                          ((UINT32)0xff00)
#define AVI_MIDXBULD_TABLE_ID_MASK                          ((UINT32)0xff)

#define AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID( ui4_chunk_id )                          \
    (((((ui4_chunk_id) & 0xff) - 0x30) * 10) +                                           \
     ((((ui4_chunk_id) & 0xff00) >> 8) - 0x30))
    

#define AVI_MIDXBULD_ESTIMATE_KEY_SIZE_START                ((UINT8)5)
#define AVI_MIDXBULD_ESTIMATE_KEY_SIZE_MID                  ((UINT8)10)
#define AVI_MIDXBULD_ESTIMATE_KEY_SIZE_END                  ((UINT8)15)
#define AVI_MIDXBULD_INDEXING_START                         ((UINT8)20)
#define AVI_MIDXBULD_INDEXING_VIDEO_START                   ((UINT8)25)
#define AVI_MIDXBULD_INDEXING_VID_DUR                       ((UINT8)60)
#define AVI_MIDXBULD_INDEXING_VIDEO_END                     ((UINT8)85)
#define AVI_MIDXBULD_INDEXING_END                           ((UINT8)100)

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT32 avi_midxbuld_key_tbl_size_estimate(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_key_id);
                                   
static UINT32 avi_midxbuld_chunk_size_estimate(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_chunk_id);
                              
static UINT32 avi_midxbuld_elmt_tbl_size_estimate(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        AVI_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                        MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ);

static INT32 avi_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T*    pt_elmt_tbl);

static INT32 avi_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl);

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
static INT32 _avi_midxbuld_get_start_info(
                        MIDXBULD_OBJ_T*                  pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type,
                        VOID*                            pt_start_info,
                        SIZE_T                           ui4_info_size);
#endif
static INT32 avi_midxbuld_alloc_range(
                        MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                        MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc);
                              
static INT32 avi_midxbuld_free_range(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id);

/* For some file, video stream include xxdb chunk */
static BOOL _b_is_compatible_vid_id(UINT32 ui4_vid_id, UINT32 ui4_curr_id)
{
    UINT8   ui1_vid_num;
    UINT8   ui1_curr_num;
    UINT16  ui2_curr_type;
    
    ui1_vid_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_vid_id);
    ui1_curr_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_curr_id);
    ui2_curr_type = (UINT16)(ui4_curr_id >> 16);
    if((ui1_vid_num == ui1_curr_num) &&
       ((0 == x_memcmp(&ui2_curr_type , "db", 2)) ||
        (0 == x_memcmp(&ui2_curr_type , "dc", 2))))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
static BOOL _b_is_compatible_aud_id(UINT32 ui4_aud_id, UINT32 ui4_curr_id)
{
    UINT8   ui1_aud_num;
    UINT8   ui1_curr_num;
    UINT16  ui2_curr_type;
    
    ui1_aud_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_aud_id);
    ui1_curr_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_curr_id);
    ui2_curr_type = (UINT16)(ui4_curr_id >> 16);
    if((ui1_aud_num == ui1_curr_num) &&
       (0 == x_memcmp(&ui2_curr_type , "wb", 2)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

static BOOL _avi_midxbuld_alloc_table_id (
                        AVI_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                        UINT32*                  pui4_tbl_id )
{
    *pui4_tbl_id = MIDXBULD_NULL_ID;
    
    if (pt_range_obj->ui1_tbl_total == 0xff)
    {
        return FALSE;
    }
    
    *pui4_tbl_id = (pt_range_obj->ui4_range_id & 0xffffff00) +
                   (++pt_range_obj->ui1_tbl_total);

    return TRUE;
}

static BOOL _avi_midxbuld_alloc_range_id (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32*                  pui4_range_id )
{
    AVI_IDXBULD_OBJ_T* pt_avi_handler;
    UINT8              ui1_key_range_id = 0;
    UINT8              ui1_range_id;

    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;

    *pui4_range_id = MIDXBULD_NULL_ID;

    if (pt_avi_handler->pt_key_range_obj != NULL)
    {
        ui1_key_range_id = (UINT8)((pt_avi_handler->pt_key_range_obj->ui4_range_id & AVI_MIDXBULD_RANGE_ID_MASK) >> 8);
    }
    else if(pt_avi_handler->ui1_range_count > 0) /* key range should be allocated first */
    {
        return FALSE;
    }

    if (pt_avi_handler->ui1_range_count < 0xff)
    {
        UINT8 ui1_i;
        BOOL  b_occupied = TRUE;

        while (b_occupied)
        {
            b_occupied = FALSE;
            for (ui1_i = 0; ui1_i < pt_avi_handler->ui1_range_count; ui1_i++)
            {
                if (ui1_i >= AVI_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                ui1_range_id = (UINT8)((pt_avi_handler->apt_range_obj[ui1_i]->ui4_range_id & AVI_MIDXBULD_RANGE_ID_MASK) >> 8);
                if ((ui1_range_id == ui1_idx_range_id_last) ||
                    (ui1_key_range_id == ui1_idx_range_id_last))
                {
                    b_occupied = TRUE;
                    ui1_idx_range_id_last++;
                    break;
                }
            }
        }
    
        *pui4_range_id = (UINT32)ui1_idx_range_id_last;
    
        ui1_idx_range_id_last = (UINT8)((ui1_idx_range_id_last == 0xff) ? 1 : (ui1_idx_range_id_last + 1));

        if (ui1_key_range_id == ui1_idx_range_id_last)
        {
            ui1_idx_range_id_last = (UINT8)((ui1_idx_range_id_last == 0xff) ? 
                                        1 : (ui1_idx_range_id_last + 1));
        }
    
        *pui4_range_id = ((pt_midxbuld_obj->h_midxbuld & 0x0000ffff) << 16) +
                         (*pui4_range_id << 8);
    
        return TRUE;
    }

    return FALSE;
}

static BOOL _avi_midxbuld_free_table_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_tbl_id)
{
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_obj = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj = NULL;
    UINT8                       ui1_free_range_id, ui1_range_id, ui1_i;
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt;

    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;

    if (pt_avi_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id)
    {
        pt_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
        pt_avi_handler->pt_key_range_obj->ui1_tbl_total--;
        pt_avi_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {
        ui1_free_range_id = (UINT8)((ui4_tbl_id & AVI_MIDXBULD_RANGE_ID_MASK) >> 8);
        
        for (ui1_i = 0; ui1_i < pt_avi_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= AVI_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            ui1_range_id = (UINT8)((pt_avi_handler->apt_range_obj[ui1_i]
                            ->ui4_range_id & AVI_MIDXBULD_RANGE_ID_MASK) >> 8);
            if (ui1_range_id == ui1_free_range_id)
            {
                BOOL b_id_match = FALSE;
            
                pt_obj = pt_avi_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_avi_handler->apt_range_obj[ui1_i]->pt_tbl_obj = pt_obj->pt_next;
                    pt_avi_handler->apt_range_obj[ui1_i]->ui1_tbl_total--;
                }
                else
                {
                    pt_last_obj = pt_obj;
                    pt_obj = pt_obj->pt_next;
                }

                while (pt_obj != NULL && 
                       b_id_match == FALSE)
                {
                    if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                    {
                        b_id_match = TRUE;
                        pt_last_obj->pt_next = pt_obj->pt_next;
                        pt_avi_handler->apt_range_obj[ui1_i]->ui1_tbl_total--;
                    }
                    else
                    {
                        pt_last_obj = pt_obj;
                        pt_obj = pt_obj->pt_next;
                    }
                }
                
                break;
            }
        }
    }

    if (pt_obj == NULL)
    {
        return FALSE;
    }

    pt_elmt = &(pt_obj->t_elmt_tbl);
    while (pt_elmt != NULL)
    {
        MIDXBULD_ELMT_TBL_OBJ_T*    pt_del_elmt = pt_elmt;
        if (pt_elmt->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
        {
            if (pt_elmt->u_fmt.t_es.pt_idx_entry != NULL)
            {
                _mm_util_mem_free_with_tag(pt_elmt->u_fmt.t_es.pt_idx_entry,
                                       MM_MEM_MIDX_TAG);
            }
        }
        else /* (pt_elmt->e_tbl_type == MIDXBULD_TBL_TYPE_AUDIO_KEY) */
        {
            if (pt_elmt->u_fmt.t_as_key.pui4_pts != NULL)
            {
                _mm_util_mem_free_with_tag(pt_elmt->u_fmt.t_as_key.pui4_pts,
                                       MM_MEM_MIDX_TAG);
            }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
            if (pt_elmt->u_fmt.t_as_key.pt_idx_entry != NULL)
            {
                _mm_util_mem_free_with_tag(pt_elmt->u_fmt.t_as_key.pt_idx_entry,
                                       MM_MEM_MIDX_TAG);
            }
#endif
        }
        pt_elmt = pt_elmt->pt_next;
        if (pt_del_elmt != &(pt_obj->t_elmt_tbl))
        {
            _mm_util_mem_free_with_tag(pt_del_elmt, MM_MEM_MIDX_TAG);
            pt_del_elmt = NULL;
        } 
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
            
    return TRUE;
}

static BOOL _avi_midxbuld_free_range_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_RANGE_OBJ_T*    pt_range_obj = NULL;
    UINT8                       ui1_i = 0;
    BOOL                        b_id_match = FALSE;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_tbl_id = MIDXBULD_NULL_ID;

    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;

    if (pt_avi_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)
    {
        b_id_match = TRUE;
        pt_range_obj = pt_avi_handler->pt_key_range_obj;
    }
    else if (pt_avi_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_avi_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= AVI_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            if (pt_avi_handler->apt_range_obj[ui1_i]->ui4_range_id == ui4_range_id)
            {
                b_id_match = TRUE;
                pt_range_obj = pt_avi_handler->apt_range_obj[ui1_i];

                break;
            }
        }
    }
    
    if (FALSE == b_id_match)
    {
        return FALSE;
    }

    if (pt_range_obj != NULL)
    {
        while (pt_range_obj->pt_tbl_obj != NULL)
        {
            pt_tbl_obj = pt_range_obj->pt_tbl_obj;
            ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
            _avi_midxbuld_free_table_obj(pt_midxbuld_obj, ui4_tbl_id);
        }
        
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        if (pt_avi_handler->pt_key_range_obj->ui4_range_id != ui4_range_id)
        {
            while ((ui1_i + 1) < pt_avi_handler->ui1_range_count)
            {
                if ((ui1_i + 1) >= AVI_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                pt_avi_handler->apt_range_obj[ui1_i] = pt_avi_handler->apt_range_obj[ui1_i + 1];
                pt_avi_handler->apt_range_obj[++ui1_i] = NULL;
            }
            pt_avi_handler->ui1_range_count--;
        }

        return TRUE;
    }

    return FALSE;
}

INT32 avi_midxbuld_multi_riff_indexing(MIDXBULD_OBJ_T*  pt_midxbuld_obj, 
                                       UINT32*          pui4_range_id)
{
    INT32                       i4_ret;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    copybytes_fct               pf_copybyte;
    setpos_fct                  pf_set_pos;
    UINT8                       ui1_strm_num, ui1_i;
    UINT32                      ui4_i;
    UINT64                      ui8_pos, ui8_size, ui8_refer_offset;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_key_tbl_obj = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_idx_entry;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT32                      ui4_chunk_id, ui4_offset, ui4_size;
    BOOL                        b_idx_entry;
    BOOL                        b_cbr[MIDXBULD_ELMT_TBL_MAX] = {0};
    UINT64                      ui8_denominator, ui8_numerator;
    UINT32                      ui4_pts;
    UINT32                      ui4_read_size;
    UINT32                      ui4_curr_size;
    UINT32                      ui4_cnt;
    UINT32                      ui4_curr_entries, ui4_entries_in_use;
    UINT8*                      pui1_index_buf;
    UINT8                       ui1_estimate_buld_start_percent;

    MIDXBULD_ELMT_TBL_OBJ_T*    pt_key_elmt;
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt;
    AVI_BASE_INDX_INFO_T*       pt_base_indx_info;
    AVI_BASE_INDX_ENTRY_T       t_curr_base_indx_entry;
    AVI_MULTI_RIFF_INFO_T*      pt_multi_riff;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if(pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_avi_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_avi_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_avi_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_avi_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }
    if (pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;

    /* check if the stream is CBR audio */
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        ui4_chunk_id = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
        ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_chunk_id);
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        if((pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) &&
           (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz != 0))
        {
            b_cbr[ui1_strm_num] = TRUE;
        }
        else
        {
            b_cbr[ui1_strm_num] = FALSE;
        }
    }

    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_key_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
        ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        pt_tbl_obj = pt_key_tbl_obj;
        /* key num is 0, no need build index table */
        if (pt_tbl_obj->ui4_tbl_size == 0) 
        {
            return MIDXBULDR_OK;
        }
    }
    else
    {
        DBG_ERROR(("ERROR: now only build key table \r\n"));
        return MIDXBULDR_INTERNAL_ERR;
    }

    /* get key base index info */
    for (ui1_i = 0; ui1_i < MIDXBULD_ELMT_TBL_MAX; ui1_i++)
    {
        pt_base_indx_info = pt_avi_handler->apt_base_indx_info[ui1_i];
        if((pt_base_indx_info == NULL) || 
           (pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id == 
            pt_base_indx_info->ui4_chunk_id) || 
           _b_is_compatible_vid_id(pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id, 
                                   pt_base_indx_info->ui4_chunk_id))
        {
            break;
        }
    }
    if(pt_base_indx_info == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    /* check if the key stream entry num is equal to audio stream */
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        /* stream index entry num should equal, otherwise can't build index table */
        if (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == 
            MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            if(pt_base_indx_info->ui4_entry_num != 
               pt_avi_handler->apt_base_indx_info[ui1_i]->ui4_entry_num)
            {
                pt_avi_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size = 0;
                return MIDXBULDR_OK;
            }
        }
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_START;
    ui1_estimate_buld_start_percent = AVI_MIDXBULD_INDEXING_VIDEO_START;

    pt_elmt = &(pt_tbl_obj->t_elmt_tbl);
    pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
    while ((pt_elmt != NULL) && (pt_multi_riff != NULL))
    {
        for(ui4_i = pt_multi_riff->ui4_start_indx_id; 
            ui4_i < (pt_multi_riff->ui4_start_indx_id + pt_multi_riff->ui4_indx_num);
            ui4_i ++)
        {
            t_curr_base_indx_entry = 
                pt_base_indx_info->pt_base_indx_entry[ui4_i];
            /* set position to the source begin */
            pf_set_pos(pt_midxbuld_obj, 
                       (INT64)t_curr_base_indx_entry.ui8_base_indx_offset,
                       FM_SEEK_BGN);
            ui4_curr_size = 32;
            pf_copybyte(pt_midxbuld_obj, 
                        pt_avi_handler->pui1_idx_buf, 
                        AVI_IDX_BUF_LENGTH,
                        ui4_curr_size, 
                        &ui4_read_size);

            LOADL_DWRD(pt_avi_handler->pui1_idx_buf + 12, ui4_entries_in_use);

            ui8_pos = 32;
            ui8_size = t_curr_base_indx_entry.ui4_base_indx_size;
            if (FALSE == pt_avi_handler->b_is_abs_offset)
            {
                ui8_refer_offset = 0;
            }
            else
            {
                ui8_refer_offset = 0;
            }

            ui4_curr_entries = 0;

            /* while loop to build index table*/
            while (ui8_pos < ui8_size)
            {
                ui4_curr_size = (UINT32)(ui8_size - ui8_pos);
                if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
                {
                    ui4_curr_size = AVI_IDX_BUF_LENGTH;
                }
                pf_copybyte(pt_midxbuld_obj, 
                            pt_avi_handler->pui1_idx_buf, 
                            AVI_IDX_BUF_LENGTH,
                            ui4_curr_size, 
                            &ui4_read_size);
                ui4_cnt = 0;
                while(ui4_cnt < ui4_read_size)
                {
                    pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;
                    
                    if (pt_midxbuld_obj->b_abort_flag == TRUE)
                    {
                        return MIDXBULDR_ABORT_INDEXING;
                    }
                    if(ui4_curr_entries >= ui4_entries_in_use)
                    {
                        break;
                    }
                    /* get "dwOffset" */
                    LOADL_DWRD(pui1_index_buf, ui4_offset);
                                
                    /* get "dwSize" */
                    LOADL_DWRD(pui1_index_buf + 4, ui4_size);
                    
                    if ((cui4_aviif_std_index_keyframe&ui4_size) == 0) /* key frame */
                    {
                        b_idx_entry = TRUE;
                    }
                    else
                    {
                        b_idx_entry = FALSE;
                    }

                    ui4_size &= 0x7FFFFFFF; /* bit31 for key flag */

                    if (b_idx_entry) /* record the entry */
                    {                
                        if (b_cbr[ui1_strm_num]) /* is CBR */
                        {
                          ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                  MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                          ui8_denominator = 
                            (UINT64)pt_elmt->u_fmt.t_es.t_info.ui4_avg_bytes_per_sec;
                        }
                        else /* is VBR */
                        {
                            ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                    pt_elmt->u_fmt.t_es.t_info.ui4_scale * 
                                                    MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                            ui8_denominator = (UINT64)pt_elmt->u_fmt.t_es.t_info.ui4_rate;
                        }
                        ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);

                        if (ui4_pts >= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts &&
                            ui4_pts <= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                            
                        {
                            if (pt_elmt->ui4_number_of_entry < pt_tbl_obj->ui4_tbl_size) /* check if the pre-allocate memory is enough */
                            {
                                pt_idx_entry = pt_elmt->u_fmt.t_es.pt_idx_entry + 
                                    pt_elmt->ui4_number_of_entry;
                                pt_idx_entry->ui4_pts = ui4_pts;
                                pt_idx_entry->ui4_relative_offset = 
                                    (UINT32)(ui8_refer_offset + ui4_offset); /* this offset is include FOURCC */
                                pt_idx_entry->ui4_size = ui4_size;
                                pt_elmt->ui4_number_of_entry++;
                                pt_avi_handler->ui1_curr_buld_progress = ui1_estimate_buld_start_percent + 
                                    (UINT8)((UINT32)(pt_elmt->ui4_number_of_entry * AVI_MIDXBULD_INDEXING_VID_DUR) / pt_tbl_obj->ui4_tbl_size);
                            }
                            else
                            {
#ifndef __KERNEL__
                                x_dbg_stmt("pre-allocate memory is NOT enough for dwChunkID(0x%.8X)!!!\r\n", 
                                            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
#endif
                                break; /* entries that not in use */
                            }                        
                        }
                    }

                    if (b_cbr[ui1_strm_num]) /* is CBR */
                    {
                        pt_tbl_obj->ui8_last_chunk_num += ui4_size;
                    }
                    else /* is VBR */
                    {
                        pt_tbl_obj->ui8_last_chunk_num++;
                    }
                    
                    ui4_curr_entries ++;
                    ui4_cnt += 8;
                }
                if (ui4_read_size < ui4_curr_size)
                {
                    break;
                }
                if(ui4_curr_entries >= ui4_entries_in_use)
                {
                    break;
                }
                ui8_pos += ui4_curr_size;
            }
        }
        pt_multi_riff = pt_multi_riff->pt_next;
        pt_elmt = pt_elmt->pt_next;
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_VIDEO_END;
    
    /* for key extern audio pts */
    pt_tbl_obj = pt_key_tbl_obj->pt_next;
    while(pt_tbl_obj != NULL)
    {
        if(pt_tbl_obj->t_elmt_tbl.e_tbl_type != MIDXBULD_TBL_TYPE_AUDIO_KEY)
        {
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        /* get the audio stream base index info */
        for (ui1_i = 0; ui1_i < MIDXBULD_ELMT_TBL_MAX; ui1_i++)
        {
            pt_base_indx_info = pt_avi_handler->apt_base_indx_info[ui1_i];
            if((pt_base_indx_info == NULL) || 
               (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id == 
                pt_base_indx_info->ui4_chunk_id))
            {
                break;
            }
        }
        if(pt_base_indx_info == NULL)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id);
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        pt_key_elmt = &(pt_key_tbl_obj->t_elmt_tbl);
        pt_elmt = &(pt_tbl_obj->t_elmt_tbl);
        pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
        while ((pt_elmt != NULL) && (pt_multi_riff != NULL))
        {
            for(ui4_i = pt_multi_riff->ui4_start_indx_id; 
                ui4_i < (pt_multi_riff->ui4_start_indx_id + pt_multi_riff->ui4_indx_num);
                ui4_i ++)
            {
                t_curr_base_indx_entry = 
                    pt_base_indx_info->pt_base_indx_entry[ui4_i];
                /* set position to the source begin */
                i4_ret = pf_set_pos(pt_midxbuld_obj, 
                                    (INT64)t_curr_base_indx_entry.ui8_base_indx_offset,
                                    FM_SEEK_BGN);
                if(i4_ret != INPUTR_OK)
                {
                    return MIDXBULDR_INITIAL_ERR;
                }
                ui4_curr_size = 32;
                i4_ret = pf_copybyte(pt_midxbuld_obj, 
                                     pt_avi_handler->pui1_idx_buf, 
                                     AVI_IDX_BUF_LENGTH,
                                     ui4_curr_size, 
                                     &ui4_read_size);
                if(i4_ret != INPUTR_OK)
                {
                    return MIDXBULDR_INITIAL_ERR;
                }
                ui8_pos = 32;
                ui8_size = (UINT64)t_curr_base_indx_entry.ui4_base_indx_size;
                if (FALSE == pt_avi_handler->b_is_abs_offset)
                {
                    ui8_refer_offset = 0;
                }
                else
                {
                    ui8_refer_offset = 0;
                }

                /* while loop to build index table*/
                while (ui8_pos < ui8_size)
                {
                    ui4_curr_size = (UINT32)(ui8_size - ui8_pos);
                    if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
                    {
                        ui4_curr_size = AVI_IDX_BUF_LENGTH;
                    }
                    pf_copybyte(pt_midxbuld_obj, 
                                pt_avi_handler->pui1_idx_buf, 
                                AVI_IDX_BUF_LENGTH,
                                ui4_curr_size, 
                                &ui4_read_size);
                    ui4_cnt = 0;
                    while(ui4_cnt < ui4_read_size)
                    {
                        pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;
                        
                        if (pt_midxbuld_obj->b_abort_flag == TRUE)
                        {
                            return MIDXBULDR_ABORT_INDEXING;
                        }
                        /* get "dwOffset" */
                        LOADL_DWRD(pui1_index_buf, ui4_offset);
                                    
                        /* get "dwSize" */
                        LOADL_DWRD(pui1_index_buf + 4, ui4_size);
                        
                        ui4_size &= 0x7FFFFFFF; /* bit31 for key flag */

                        /* several video key have same one audio pts */
                        while(pt_elmt->ui4_number_of_entry < pt_key_elmt->ui4_number_of_entry)
                        {
                            pt_key_idx_entry = 
                                pt_key_elmt->u_fmt.t_es.pt_idx_entry + 
                                pt_elmt->ui4_number_of_entry;

                            if (ui4_offset >= pt_key_idx_entry->ui4_relative_offset) /* record the entry */
                            {                
                                if (b_cbr[ui1_strm_num]) /* is CBR */
                                {
                                  ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                          MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                                  ui8_denominator = 
                                    (UINT64)pt_elmt->u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec;
                                }
                                else /* is VBR */
                                {
                                  ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                          pt_elmt->u_fmt.t_as_key.t_info.ui4_scale * 
                                                          MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                                  ui8_denominator = (UINT64)pt_elmt->u_fmt.t_as_key.t_info.ui4_rate;
                                }
                                ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
                                /* pts + offset */
                                ui4_pts += pt_midxbuld_obj->t_src_info.ui4_cbr_sub_vbr;
  #ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                                if (pt_elmt->u_fmt.t_as_key.e_entry_type == MIDXBULD_AUD_ALL_ENTRY_TYPE)
                                {
                                    (pt_elmt->u_fmt.t_as_key.pt_idx_entry + pt_elmt->ui4_number_of_entry)->ui4_pts = ui4_pts;
                                    (pt_elmt->u_fmt.t_as_key.pt_idx_entry + pt_elmt->ui4_number_of_entry)->ui4_relative_offset  = (UINT32)(ui8_refer_offset + ui4_offset);
                                    (pt_elmt->u_fmt.t_as_key.pt_idx_entry + pt_elmt->ui4_number_of_entry)->ui4_size  = ui4_size;
                                }
                                else
#endif
                                {
                                    *(pt_elmt->u_fmt.t_as_key.pui4_pts + pt_elmt->ui4_number_of_entry) = ui4_pts;
                                }
                                pt_elmt->ui4_number_of_entry++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (b_cbr[ui1_strm_num]) /* is CBR */
                        {
                            pt_tbl_obj->ui8_last_chunk_num += ui4_size;
                        }
                        else /* is VBR */
                        {
                            pt_tbl_obj->ui8_last_chunk_num++;
                        }
                        
                        ui4_cnt += 8;
                    }
                    if (ui4_read_size < ui4_curr_size)
                    {
                        break;
                    }
                    ui8_pos += ui4_curr_size;
                }
            }
#ifdef  MM_BAD_INTERLEAVED_FILE_SUPPORT   
            if (pt_elmt->u_fmt.t_as_key.e_entry_type == MIDXBULD_AUD_PTS_ENTRY_TYPE)
            {
                while(pt_elmt->ui4_number_of_entry < pt_key_elmt->ui4_number_of_entry)
                {
                    *(pt_elmt->u_fmt.t_as_key.pui4_pts + pt_elmt->ui4_number_of_entry) = 
                       (pt_key_elmt->u_fmt.t_es.pt_idx_entry + pt_elmt->ui4_number_of_entry)->ui4_pts;
                    pt_elmt->ui4_number_of_entry++;
                }
            }
#endif
            pt_multi_riff = pt_multi_riff->pt_next;
            pt_elmt = pt_elmt->pt_next;
            pt_key_elmt = pt_key_elmt->pt_next;
        }
        pt_tbl_obj = pt_tbl_obj->pt_next;
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_END;

    pt_avi_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    return MIDXBULDR_OK;
}

INT32 avi_midxbuld_vt_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, UINT32* pui4_range_id)
{
    INT32                       i4_ret;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    copybytes_fct               pf_copybyte;
    input4bytes_fct             pf_input4;
    setpos_fct                  pf_set_pos;
    UINT8                       ui1_strm_num, ui1_i;
    UINT8                       ui1_curr_strm_num = 0;
    UINT32                      ui4_i, ui4_j;
    UINT64                      ui8_pos, ui8_size, ui8_refer_offset;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_key_tbl_obj = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT32                      ui4_chunk_id, ui4_flags, ui4_offset, ui4_size;
    BOOL                        b_idx_entry;
    BOOL                        b_cbr[MIDXBULD_ELMT_TBL_MAX] = {0};
    UINT64                      ui8_denominator, ui8_numerator;
    UINT64                      aui8_last_chunk_num[MIDXBULD_ELMT_TBL_MAX] = {0};
    UINT32                      ui4_pts;
    UINT64                      ui8_last_chunk_num;
    UINT32                      ui4_read_size = 0;
    UINT32                      ui4_curr_size;
    UINT32                      ui4_cnt = 0;
    UINT8*                      pui1_index_buf;
    UINT8                       ui1_estimate_buld_start_percent;
    UINT32                      ui4_chunk_num = 0;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if(pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_avi_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_avi_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_avi_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_avi_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }
    if (pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;

    /* set position to the source begin */
    if(((UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_offset) == ((UINT32)0xFFFFFFFF)) /* has no idx */
    {
        pt_midxbuld_obj->t_src_info.ui8_idx_offset = pt_midxbuld_obj->t_src_info.ui8_data_offset;
    }
    pf_set_pos(pt_midxbuld_obj, (INT64)pt_midxbuld_obj->t_src_info.ui8_idx_offset, FM_SEEK_BGN);

    /* check the source start code */
    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_flags); 
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if (0x69766f6d /* "movi" */ != ui4_flags)
    {
        return MIDXBULDR_FMT_NOT_SUPPORT;
    }

    ui8_refer_offset = pt_midxbuld_obj->t_src_info.ui8_data_offset + 4;

    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_flags); 
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if (0x5453494c /* "LIST" */ == ui4_flags)
    {
        ui8_refer_offset += 12;
        pf_set_pos(pt_midxbuld_obj, (INT64)8 /*skip chunk size and type*/, FM_SEEK_CUR);
    }
    else
    {
        pf_set_pos(pt_midxbuld_obj, (INT64)-4 /*it's chunk id, reverse file postion*/, FM_SEEK_CUR);
    }

    /* check the source size */
    if(((UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_size) == ((UINT32)0xFFFFFFFF))
    {
        pt_midxbuld_obj->t_src_info.ui8_idx_size = pt_midxbuld_obj->t_src_info.ui8_data_size;
    }
    if (pt_midxbuld_obj->t_src_info.ui8_idx_size == 0)
    {
        return MIDXBULDR_INV_ARG;
    }

    /* check if the stream is CBR audio */
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        ui4_chunk_id = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
        ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_chunk_id);
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if((pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == 
            MIDXBULD_ELEM_STRM_TYPE_AUDIO) &&
           (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz != 0))
        {
            b_cbr[ui1_strm_num] = TRUE;
        }
        else
        {
            b_cbr[ui1_strm_num] = FALSE;
        }
        /*
            if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == 
                MIDXBULD_ELEM_STRM_TYPE_VIDEO)
            {
                b_has_video = TRUE;
            }*/
    }

    if (pt_avi_handler->ui8_resume_pos != 0)
    {
        ui8_pos = pt_avi_handler->ui8_resume_pos;
        ui8_size = pt_avi_handler->ui8_resume_size;
        pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_CUR);
    }
    else
    {
        ui8_pos = 0;
        ui8_size = pt_midxbuld_obj->t_src_info.ui8_idx_size;
        /* consider the partial range case */
        if ((pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts != 0) ||
            (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts < 
             pt_midxbuld_obj->t_src_info.ui4_total_duration))
        {
            MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry;
    
            ui4_i = 0;
            pt_key_entry = 
                pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
            if (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts != 0)
            {
                while (pt_key_entry->ui4_pts <= 
                       pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts)
                {
                    ui4_i++;
                    pt_key_entry++;
                }
        
                /* jump to start position for partial range index building */
                if (ui4_i >= 2)
                {
                    ui8_pos = *(pt_avi_handler->pui8_key_frame_pos_map + (ui4_i - 2));
                    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_CUR);
        
                    for (ui4_j = 0; ui4_j < MIDXBULD_ELMT_TBL_MAX; ui4_j++)
                    {
                        if (pt_avi_handler->aui1_idx_strm_num_map[ui4_j] != 0xff)
                        {
                            pt_tbl_obj = 
                                pt_avi_handler->apt_tbl_obj_map[pt_avi_handler->aui1_idx_strm_num_map[ui4_j]];
                            pt_tbl_obj->ui8_last_chunk_num = 
                                *(pt_avi_handler->apui8_last_chunk_num_map[ui4_j] + (ui4_i - 2));
                        }
                    }
                }
            }
    
            if (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts < 
                pt_midxbuld_obj->t_src_info.ui4_total_duration)
            {
                while (ui4_i < pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry &&
                       pt_key_entry->ui4_pts < pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                {
                    ui4_i++;
                    pt_key_entry++;
                }
        
                /* end position for partial range index building */
                if (ui4_i < pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry)
                {
                    ui8_size = *(pt_avi_handler->pui8_key_frame_pos_map + ui4_i);
                }
            }
        }
    }

    
    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_START;
    ui1_estimate_buld_start_percent = AVI_MIDXBULD_INDEXING_VIDEO_START;

    ui4_offset = 0;
    
    /* while loop to build index table*/
    while (ui8_pos < ui8_size)
    {
        ui4_curr_size = (UINT32)(ui8_size - ui8_pos);
        if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
        {
            ui4_curr_size = AVI_IDX_BUF_LENGTH;
        }
        if((0 != ui8_pos)/*not the first time*/ && (ui4_cnt >= ui4_read_size))
        {
            ui4_cnt -= ui4_read_size;
            ui8_pos += ui4_cnt;
            if(ui8_pos >= ui8_size) /*reach the file end*/
            {
                break;
            }
            pf_set_pos(pt_midxbuld_obj, (INT64)ui4_cnt, FM_SEEK_CUR);
        }
        pf_copybyte(pt_midxbuld_obj, 
                    pt_avi_handler->pui1_idx_buf, 
                    AVI_IDX_BUF_LENGTH,
                    ui4_curr_size, 
                    &ui4_read_size);
        ui4_cnt = 0;
        while(ui4_cnt < ui4_read_size)
        {
            pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;
            
            if (pt_midxbuld_obj->b_abort_flag == TRUE)
            {
                pt_avi_handler->ui8_resume_pos = ui8_pos;
                pt_avi_handler->ui8_resume_size = ui8_size;

                return MIDXBULDR_ABORT_INDEXING;
            }

            /* get "dwChunkId" */
            LOADL_DWRD(pui1_index_buf, ui4_chunk_id);                
            /* get "dwSize" */
            LOADL_DWRD(pui1_index_buf + 4, ui4_size);
            if((ui4_size % 2) != 0)
            {
                ui4_size += 1;
            }
            /* get "dwOffset" */
            ui4_offset = (UINT32)ui8_pos + ui4_cnt;
            
            ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_chunk_id); /* get stream number from dwChunkId */
            if(ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX) 
            {
                ui4_cnt += (ui4_size + 8);
                continue;
            }
            
            if (0 == ui1_strm_num/*pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] != 0xff*/) /* check if the index stream number match client's request */
            {
                pt_tbl_obj = pt_avi_handler->apt_tbl_obj_map[pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num]]; /* get table object */
                /* check dwChunkId for double confirm */
                if (1)
                {
                    pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                                   pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;

                    b_idx_entry = TRUE;
                        
                    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame) /* key frame table? */
                    {
                        /* key num is 0, no need build index table */
                        if (pt_tbl_obj->ui4_tbl_size == 0)
                        {
                            return MIDXBULDR_OK;
                        }
                        if (ui4_chunk_num % VISUAL_TBL_FRM_CNT) /* not key frame */
                        {
                            b_idx_entry = FALSE;
                        }
                        else
                        {
                            /* record position for every key */
                            *(pt_avi_handler->pui8_key_frame_pos_map + pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry) = ui8_pos;
                        }
                    }
                    
                    if (b_idx_entry) /* record the entry */
                    {                
                        if (b_cbr[ui1_strm_num]) /* is CBR */
                        {
                          ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                  MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                          ui8_denominator = 
                            (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_avg_bytes_per_sec;
                        }
                        else /* is VBR */
                        {
                            ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * 
                                                    MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                            ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;
                        }
                        ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);

                        if (ui4_pts >= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts &&
                            ui4_pts <= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                            
                        {
                            if (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry < pt_tbl_obj->ui4_tbl_size) /* check if the pre-allocate memory is enough */
                            {
                                pt_idx_entry->ui4_pts = ui4_pts;
                                pt_idx_entry->ui4_relative_offset = (UINT32)(ui8_refer_offset + ui4_offset); /* this offset is include FOURCC */
                                pt_idx_entry->ui4_size = ui4_size;
                                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry++;
                                pt_avi_handler->ui1_curr_buld_progress = ui1_estimate_buld_start_percent + 
                                    (UINT8)((UINT32)(pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry * AVI_MIDXBULD_INDEXING_VID_DUR) / pt_tbl_obj->ui4_tbl_size);
                            }
                            else
                            {
#ifndef __KERNEL__
                                x_dbg_stmt("pre-allocate memory is NOT enough for dwChunkID(0x%.8X)!!!\r\n", ui4_chunk_id);
#endif
                                break; /* entries that not in use */
                            }                        
                        }
                    }

                    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame)
                    {
                        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
                        {
                            return MIDXBULDR_INITIAL_ERR;
                        }
                        aui8_last_chunk_num[ui1_strm_num] = pt_tbl_obj->ui8_last_chunk_num;
                        
                        /* record a group of aui8_last_chunk_num for a key entry */
                        if ((ui4_chunk_num % VISUAL_TBL_FRM_CNT) == 0)
                        {
                            for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
                            {
                                if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                                {
                                    break;
                                }
                                ui1_curr_strm_num = 
                                    (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id);
                                if ((ui1_curr_strm_num >= MIDXBULD_ELMT_TBL_MAX) ||
                                    (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry < 1))
                                {
                                    return MIDXBULDR_INITIAL_ERR;
                                }
                                *(pt_avi_handler->apui8_last_chunk_num_map[ui1_i] + 
                                    (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry - 1)) = 
                                    aui8_last_chunk_num[ui1_curr_strm_num];
                            }
                        }
                    }

                    if (b_cbr[ui1_strm_num]) /* is CBR */
                    {
                        pt_tbl_obj->ui8_last_chunk_num += ui4_size;
                    }
                    else /* is VBR */
                    {
                        pt_tbl_obj->ui8_last_chunk_num++;
                    }
                }
                ui4_chunk_num ++;
            }
            else /* stream number not match */
            {
                if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame) /* to update aui8_last_chunk_num[ui1_strm_num] */
                {
                    if (b_cbr[ui1_strm_num]) /* is CBR */
                    {                    
                        /* get dwSize */                            
                        aui8_last_chunk_num[ui1_strm_num] += ui4_size;
                    }
                    else /* is VBR */
                    {                    
                        aui8_last_chunk_num[ui1_strm_num]++;
                    }
                }            
            }            
            ui4_cnt += (ui4_size + 8);
            
        }
        if (ui4_read_size < ui4_curr_size)
        {
            break;
        }
        ui8_pos += ui4_curr_size;
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_VIDEO_END;

    /* for key extern audio pts */
    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_key_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
        pt_tbl_obj = pt_key_tbl_obj->pt_next;
        while(pt_tbl_obj != NULL)
        {
            if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
            {
                /* get stream number from dwChunkId */
                for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
                {
                    if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                    {
                        return MIDXBULDR_INITIAL_ERR;
                    }
                    if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id == 
                        pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id)
                    {
                        break;
                    }
                }
                ui1_strm_num = 
                    (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID
                        (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id); 
                if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
                {
                    break;
                }
                /*for (ui4_i = 0; ui4_i < pt_key_tbl_obj->ui4_tbl_size; ui4_i++)*/
                for (ui4_i = 0; ui4_i < pt_key_tbl_obj->t_elmt_tbl.ui4_number_of_entry; ui4_i++)
                {
                    if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                    {
                        break;
                    }
                    ui8_last_chunk_num = *(pt_avi_handler->apui8_last_chunk_num_map[ui1_i] + ui4_i);
                    if (b_cbr[ui1_strm_num]) /* is CBR */
                    {
                       ui8_numerator = (UINT64)ui8_last_chunk_num * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                       ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec;
                    }
                    else /* is VBR */
                    {
                        ui8_numerator = (UINT64)ui8_last_chunk_num * pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                        ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate;
                    }
                    ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);

                    *(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts + ui4_i) = ui4_pts;
                }
                /* set audio element table number of entry same as key table */
                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_key_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
            }
            pt_tbl_obj = pt_tbl_obj->pt_next;
        }
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_END;
    
    pt_avi_handler->pt_curr_range_obj->b_index_is_valid = TRUE;
    
    return MIDXBULDR_OK;
}

static INT32 _avi_midxbuld_check_double_key_table(MIDXBULD_OBJ_T* pt_midxbuld_obj)
{
    AVI_IDXBULD_OBJ_T*            pt_avi_handler;
    AVI_IDXBULD_ELMT_TBL_OBJ_T*   pt_first_tbl_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T*   pt_second_tbl_obj;
    MIDXBULD_ELMT_STRM_ENTRY_T*   pt_first_idx_entry;
    MIDXBULD_ELMT_STRM_ENTRY_T*   pt_second_idx_entry;
    UINT32                        ui4_count = 0;
    UINT32                        ui4_diff  = 0;
    
    if (pt_midxbuld_obj ==NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if (pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /* search the first key table*/
    pt_first_tbl_obj = pt_avi_handler->pt_curr_range_obj->pt_tbl_obj;
    while(pt_first_tbl_obj != NULL)
    {
        if(pt_first_tbl_obj->t_elmt_tbl.e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
        {
            break;
        }
        else
        {
            pt_first_tbl_obj = pt_first_tbl_obj->pt_next;
        }
    }
    if(pt_first_tbl_obj == NULL)
    {
        return MIDXBULDR_OK;
    }
    /* search the second key table*/
    pt_second_tbl_obj = pt_first_tbl_obj->pt_next;
    while(pt_second_tbl_obj != NULL)
    {
        if(pt_second_tbl_obj->t_elmt_tbl.e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
        {
            break;
        }
        else
        {
            pt_second_tbl_obj = pt_second_tbl_obj->pt_next;
        } 
    }
    if (pt_second_tbl_obj == NULL)
    {
        return MIDXBULDR_OK;
    }
    /* entry number must equal */
    if (pt_second_tbl_obj->ui4_tbl_size != pt_first_tbl_obj->ui4_tbl_size)
    {
        return MIDXBULDR_OK;
    }
    pt_first_idx_entry  = pt_first_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    pt_second_idx_entry = pt_second_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    if (pt_first_idx_entry == NULL || pt_second_idx_entry == NULL)
    {
        return MIDXBULDR_OK;
    }
    /* judge the every entry is different more 10*/
    while(ui4_count < pt_second_tbl_obj->ui4_tbl_size)
    {
        if(pt_first_idx_entry->ui4_pts > pt_second_idx_entry->ui4_pts)
        {
            ui4_diff = pt_first_idx_entry->ui4_pts - pt_second_idx_entry->ui4_pts;
        }
        else
        {
            ui4_diff = pt_second_idx_entry->ui4_pts - pt_first_idx_entry->ui4_pts;
        }
        if(ui4_diff > 10)
        {
            pt_avi_handler->pt_key_tbl_ext->b_support_3d_trick = FALSE;
            return MIDXBULDR_INV_ARG;
        }
        pt_first_idx_entry++;
        pt_second_idx_entry++;
        ui4_count++;
    }
    pt_avi_handler->pt_key_tbl_ext->b_support_3d_trick = TRUE;

    return MIDXBULDR_OK;
}

INT32 avi_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, UINT32* pui4_range_id)
{
    INT32                       i4_ret;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    copybytes_fct               pf_copybyte;
    input4bytes_fct             pf_input4;
    setpos_fct                  pf_set_pos;
    UINT8                       ui1_strm_num, ui1_i;
    UINT8                       ui1_curr_strm_num = 0;
    UINT32                      ui4_i, ui4_j;
    UINT64                      ui8_pos, ui8_size, ui8_refer_offset;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_key_tbl_obj = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT32                      ui4_chunk_id, ui4_flags, ui4_offset, ui4_size;
    BOOL                        b_idx_entry;
    BOOL                        b_cbr[MIDXBULD_ELMT_TBL_MAX] = {0};
    UINT64                      ui8_denominator, ui8_numerator;
    UINT64                      aui8_last_chunk_num[MIDXBULD_ELMT_TBL_MAX] = {0};
    UINT32                      ui4_pts;
    UINT64                      ui8_last_chunk_num;
    UINT32                      ui4_read_size;
    UINT32                      ui4_curr_size;
    UINT32                      ui4_cnt;
    UINT8*                      pui1_index_buf;
    UINT8                       ui1_estimate_buld_start_percent;
    UINT16                      ui2_curr_type = 0;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if(pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_avi_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_avi_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_avi_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_avi_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }
    if (pt_avi_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    if(((UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_offset) == ((UINT32)0xFFFFFFFF) && 
       ((UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_size)   == ((UINT32)0xFFFFFFFF))
    {
        return avi_midxbuld_vt_indexing(pt_midxbuld_obj, pui4_range_id);
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;

    /* set position to the source begin */
    pf_set_pos(pt_midxbuld_obj, (INT64)pt_midxbuld_obj->t_src_info.ui8_idx_offset, FM_SEEK_BGN);

    /* check the source start code */
    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_flags);
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if (cui4_idx1 != ui4_flags)
    {
        return MIDXBULDR_FMT_NOT_SUPPORT;
    }

    /* check the source size */
    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_size);
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if (pt_midxbuld_obj->t_src_info.ui8_idx_size != ui4_size)
    {
        return MIDXBULDR_INV_ARG;
    }

    /* check if the stream is CBR audio */
#if 1 /* bsjin update for t_src_info.t_es_map stream id not sync with file 20080923 */
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        ui4_chunk_id = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
        ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_chunk_id);
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if((pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == 
            MIDXBULD_ELEM_STRM_TYPE_AUDIO) &&
           (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz != 0))
        {
            b_cbr[ui1_strm_num] = TRUE;
        }
        else
        {
            b_cbr[ui1_strm_num] = FALSE;
        }
    }
#else
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        b_cbr[ui1_i] = (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) &&
                       (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz != 0);
    }
#endif

    if (pt_avi_handler->ui8_resume_pos != 0)
    {
        ui8_pos = pt_avi_handler->ui8_resume_pos;
        ui8_size = pt_avi_handler->ui8_resume_size;
        pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_CUR);
    }
    else
    {
        ui8_pos = 0;
        ui8_size = pt_midxbuld_obj->t_src_info.ui8_idx_size;
        /* consider the partial range case */
        if ((pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts != 0) ||
            (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts < 
             pt_midxbuld_obj->t_src_info.ui4_total_duration))
        {
            MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry;
    
            ui4_i = 0;
            pt_key_entry = 
                pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
            if (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts != 0)
            {
                while (pt_key_entry->ui4_pts <= 
                       pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts)
                {
                    ui4_i++;
                    pt_key_entry++;
                }
        
                /* jump to start position for partial range index building */
                if (ui4_i >= 2)
                {
                    ui8_pos = *(pt_avi_handler->pui8_key_frame_pos_map + (ui4_i - 2));
                    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_CUR);
        
                    for (ui4_j = 0; ui4_j < MIDXBULD_ELMT_TBL_MAX; ui4_j++)
                    {
                        if (pt_avi_handler->aui1_idx_strm_num_map[ui4_j] != 0xff)
                        {
                            pt_tbl_obj = 
                                pt_avi_handler->apt_tbl_obj_map[pt_avi_handler->aui1_idx_strm_num_map[ui4_j]];
                            pt_tbl_obj->ui8_last_chunk_num = 
                                *(pt_avi_handler->apui8_last_chunk_num_map[ui4_j] + (ui4_i - 2));
                        }
                    }
                }
            }
    
            if (pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts < 
                pt_midxbuld_obj->t_src_info.ui4_total_duration)
            {
                while (ui4_i < pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry &&
                       pt_key_entry->ui4_pts < pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                {
                    ui4_i++;
                    pt_key_entry++;
                }
        
                /* end position for partial range index building */
                if (ui4_i < pt_avi_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry)
                {
                    ui8_size = *(pt_avi_handler->pui8_key_frame_pos_map + ui4_i);
                }
            }
        }
    }

    if (FALSE == pt_avi_handler->b_is_abs_offset)
    {
        ui8_refer_offset = pt_midxbuld_obj->t_src_info.ui8_data_offset;
    }
    else
    {
        ui8_refer_offset = 0;
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_START;
    ui1_estimate_buld_start_percent = AVI_MIDXBULD_INDEXING_VIDEO_START;

    /* while loop to build index table*/
    while (ui8_pos < ui8_size)
    {
        ui4_curr_size = (UINT32)(ui8_size - ui8_pos);
        if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
        {
            ui4_curr_size = AVI_IDX_BUF_LENGTH;
        }
        pf_copybyte(pt_midxbuld_obj, 
                    pt_avi_handler->pui1_idx_buf, 
                    AVI_IDX_BUF_LENGTH,
                    ui4_curr_size, 
                    &ui4_read_size);
        ui4_cnt = 0;
        while(ui4_cnt < ui4_read_size)
        {
            pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;
            
            if (pt_midxbuld_obj->b_abort_flag == TRUE)
            {
                pt_avi_handler->ui8_resume_pos = ui8_pos;
                pt_avi_handler->ui8_resume_size = ui8_size;

                return MIDXBULDR_ABORT_INDEXING;
            }

            /* get "dwChunkId" */
            LOADL_DWRD(pui1_index_buf, ui4_chunk_id);
            
            ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(ui4_chunk_id); /* get stream number from dwChunkId */
            ui2_curr_type = (UINT16)(ui4_chunk_id >> 16);
            
            if((ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX) ||
               (0 == x_memcmp(&ui2_curr_type , "dd", 2))) /* CR: DTV00132644 */
            {
                ui4_cnt += 16;
                continue;
            }
            if (pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] != 0xff) /* check if the index stream number match client's request */
            {
                pt_tbl_obj = pt_avi_handler->apt_tbl_obj_map[pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num]]; /* get table object */
                /* check dwChunkId for double confirm */
                if ( _b_is_compatible_vid_id(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id, ui4_chunk_id))
                {
                    pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                                   pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;

                    b_idx_entry = TRUE;
                    /* get "dwFlags" */
                    LOADL_DWRD(pui1_index_buf + 4, ui4_flags);
                        
                    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame/* &&
                        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_KEY*/) /* key frame table? */
                    {
                        /* key num is 0, no need build index table */
                        if (pt_tbl_obj->ui4_tbl_size == 0)
                        {
                            return MIDXBULDR_OK;
                        }
                        if ((cui4_aviif_keyframe&ui4_flags) != cui4_aviif_keyframe) /* not key frame */
                        {
                            b_idx_entry = FALSE;
                        }
                        else
                        {
                            /* record position for every key */
                            *(pt_avi_handler->pui8_key_frame_pos_map + pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry) = ui8_pos;
                        }
                    }

                    /* get "dwOffset" */
                    LOADL_DWRD(pui1_index_buf + 8, ui4_offset);
                        
                    /* get "dwSize" */
                    LOADL_DWRD(pui1_index_buf + 12, ui4_size);

                    if (b_idx_entry) /* record the entry */
                    {                
                        if (b_cbr[ui1_strm_num]) /* is CBR */
                        {
                          ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                  MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                          ui8_denominator = 
                            (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_avg_bytes_per_sec;
                        }
                        else /* is VBR */
                        {
                            ui8_numerator = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                                                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * 
                                                    MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                            ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;
                        }
                        ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);

                        if (ui4_pts >= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts &&
                            ui4_pts <= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                            
                        {
                            if (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry < pt_tbl_obj->ui4_tbl_size) /* check if the pre-allocate memory is enough */
                            {
                                pt_idx_entry->ui4_pts = ui4_pts;
                                pt_idx_entry->ui4_relative_offset = (UINT32)(ui8_refer_offset + ui4_offset); /* this offset is include FOURCC */
                                pt_idx_entry->ui4_size = ui4_size;
                                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry++;
                                pt_avi_handler->ui1_curr_buld_progress = ui1_estimate_buld_start_percent + 
                                    (UINT8)((UINT32)(pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry * AVI_MIDXBULD_INDEXING_VID_DUR) / pt_tbl_obj->ui4_tbl_size);
                            }
                            else
                            {
#ifndef __KERNEL__
                                x_dbg_stmt("pre-allocate memory is NOT enough for dwChunkID(0x%.8X)!!!\r\n", ui4_chunk_id);
#endif
                                break; /* entries that not in use */
                            }                        
                        }
                    }

                    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame)
                    {
                        aui8_last_chunk_num[ui1_strm_num] = pt_tbl_obj->ui8_last_chunk_num;
                        
                        /* record a group of aui8_last_chunk_num for a key entry */
                        if ((cui4_aviif_keyframe&ui4_flags) == cui4_aviif_keyframe)
                        {
                            for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
                            {
                                if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                                {
                                    break;
                                }
                                ui1_curr_strm_num = 
                                    (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id);
                                if ((ui1_curr_strm_num >= MIDXBULD_ELMT_TBL_MAX) ||
                                    (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry < 1))
                                {
                                    return MIDXBULDR_INITIAL_ERR;
                                }
                                *(pt_avi_handler->apui8_last_chunk_num_map[ui1_i] + 
                                    (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry - 1)) = 
                                    aui8_last_chunk_num[ui1_curr_strm_num];
                            }
                        }
                    }
                } 
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                if ( _b_is_compatible_aud_id(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id, ui4_chunk_id) &&
                    (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.e_entry_type == MIDXBULD_AUD_ALL_ENTRY_TYPE))
                {
                    /* key num is 0, no need build index table */
                    if (pt_tbl_obj->ui4_tbl_size == 0)
                    {
                        return MIDXBULDR_OK;
                    }
                    pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry + 
                                   pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
                    /* get "dwFlags" */
                    LOADL_DWRD(pui1_index_buf + 4, ui4_flags);
                    /* get "dwOffset" */
                    LOADL_DWRD(pui1_index_buf + 8, ui4_offset);   
                    /* get "dwSize" */
                    LOADL_DWRD(pui1_index_buf + 12, ui4_size);

                    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame &&
                        ((cui4_aviif_keyframe&ui4_flags) == cui4_aviif_keyframe)&&
                        (pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry < pt_tbl_obj->ui4_tbl_size))
                    {
                        if(b_cbr[ui1_strm_num]) /* CBR */
                        {
                            ui8_numerator   = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 90000;
                        }
                        else
                        {                     
                            ui8_numerator   = (UINT64)pt_tbl_obj->ui8_last_chunk_num * 
                            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale * 90000;
                            
                        }
                        ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate;
                        ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);

                        if (ui4_pts >= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_start_pts &&
                            ui4_pts <= pt_avi_handler->pt_curr_range_obj->u_range.t_es.ui4_end_pts)
                        {
                            pt_idx_entry->ui4_pts = ui4_pts;
                            pt_idx_entry->ui4_relative_offset = (UINT32)(ui8_refer_offset + ui4_offset); /* this offset is include FOURCC */
                            pt_idx_entry->ui4_size = ui4_size;
                            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry++;
                        }
                    }
                }
#endif
                if (b_cbr[ui1_strm_num]) /* is CBR */
                {
                    pt_tbl_obj->ui8_last_chunk_num += ui4_size;
                }
                else /* is VBR */
                {
                    pt_tbl_obj->ui8_last_chunk_num++;
                }
            }            
            else /* stream number not match */
            {
                if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame) /* to update aui8_last_chunk_num[ui1_strm_num] */
                {
                    if (b_cbr[ui1_strm_num]) /* is CBR */
                    {                    
                        /* get dwSize */
                        LOADL_DWRD(pui1_index_buf + 12, ui4_size);
                            
                        aui8_last_chunk_num[ui1_strm_num] += ui4_size;
                    }
                    else /* is VBR */
                    {                    
                        aui8_last_chunk_num[ui1_strm_num]++;
                    }
                }            
            }
            ui4_cnt += 16;
        }
        if (ui4_read_size < ui4_curr_size)
        {
            break;
        }
        ui8_pos += ui4_curr_size;
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_VIDEO_END;

    _avi_midxbuld_check_double_key_table(pt_midxbuld_obj);

    /* for key extern audio pts */
    if (pt_avi_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_key_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
        pt_tbl_obj = pt_key_tbl_obj->pt_next;
        while(pt_tbl_obj != NULL)
        {
            if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
            {
                /* get stream number from dwChunkId */
                for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
                {
                    if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                    {
                        return MIDXBULDR_INITIAL_ERR;
                    }
                    if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id == 
                        pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id)
                    {
                        break;
                    }
                }
                ui1_strm_num = 
                    (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID
                        (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id); 
                if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
                {
                    break;
                }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.e_entry_type == MIDXBULD_AUD_PTS_ENTRY_TYPE)
#endif              
                {
                    for (ui4_i = 0; ui4_i < pt_key_tbl_obj->ui4_tbl_size; ui4_i++)
                    {
                        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
                        {
                            return MIDXBULDR_INITIAL_ERR;
                        }
                        ui8_last_chunk_num = *(pt_avi_handler->apui8_last_chunk_num_map[ui1_i] + ui4_i);
                        if (b_cbr[ui1_strm_num]) /* is CBR */
                        {
                           ui8_numerator = (UINT64)ui8_last_chunk_num * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                           ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec;
                        }
                        else /* is VBR */
                        {
                            ui8_numerator = (UINT64)ui8_last_chunk_num * 
                                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale * 
                                MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                            ui8_denominator = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate;
                        }
                        ui4_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
                        /* pts + offset */
                        ui4_pts += pt_midxbuld_obj->t_src_info.ui4_cbr_sub_vbr;

                        *(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts + ui4_i) = ui4_pts;
                    }
                    /* set audio element table number of entry same as key table */
                    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_key_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
                }
            }
            pt_tbl_obj = pt_tbl_obj->pt_next;
        }
    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_INDEXING_END;
    
    pt_avi_handler->pt_curr_range_obj->b_index_is_valid = TRUE;
    
    return MIDXBULDR_OK;
}

/* parser and set base index info for avi2.0 */
static INT32 _avi_midxbuld_set_base_indx_info(
                        MIDXBULD_OBJ_T* pt_midxbuld_obj)
{
    UINT8                   ui1_i;
    UINT32                  ui4_k;
    AVI_IDXBULD_OBJ_T*      pt_avi_handler;
    copybytes_fct           pf_copybyte;
    setpos_fct              pf_set_pos;
    getpos_fct              pf_get_pos;
    UINT64                  ui8_pos;
    UINT8*                  pui1_index_buf;
    AVI_BASE_INDX_INFO_T*   pt_base_indx;
    UINT32                  ui4_read_size;
    UINT32                  ui4_curr_size;
    
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_midxbuld_obj, &ui8_pos);

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        pf_set_pos(pt_midxbuld_obj, 
                   (INT64)(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_indx_offset + 8),
                   FM_SEEK_BGN);
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_indx_size <= 
           AVI_IDX_BUF_LENGTH)
        {
            pt_base_indx = 
                (AVI_BASE_INDX_INFO_T*) _mm_util_mem_alloc_with_tag(
                                            sizeof(AVI_BASE_INDX_INFO_T), 
                                            MM_MEM_MIDX_TAG);
            if (pt_base_indx == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_base_indx, 0, sizeof(AVI_BASE_INDX_INFO_T));
            
            ui4_curr_size = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_indx_size;
            pf_copybyte(pt_midxbuld_obj, 
                        pt_avi_handler->pui1_idx_buf, 
                        AVI_IDX_BUF_LENGTH,
                        ui4_curr_size, 
                        &ui4_read_size);
            LOADL_WORD(pt_avi_handler->pui1_idx_buf, 
                       pt_base_indx->ui2_longs_per_entry);
            LOAD_BYTE(pt_avi_handler->pui1_idx_buf + 2,
                      pt_base_indx->ui1_indx_sub_type);
            LOAD_BYTE(pt_avi_handler->pui1_idx_buf + 3,
                      pt_base_indx->ui1_indx_type);
            LOADL_DWRD(pt_avi_handler->pui1_idx_buf + 4,
                       pt_base_indx->ui4_entry_num);
            LOADL_DWRD(pt_avi_handler->pui1_idx_buf + 8,
                       pt_base_indx->ui4_chunk_id);
            /* check index type */
            if(pt_base_indx->ui2_longs_per_entry != 4)
            {
                DBG_ERROR(("ERROR: unsupport index entry \r\n"));
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            pt_base_indx->pt_base_indx_entry = 
                (AVI_BASE_INDX_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(AVI_BASE_INDX_ENTRY_T) * pt_base_indx->ui4_entry_num, 
                    MM_MEM_MIDX_TAG);
            if(pt_base_indx->pt_base_indx_entry == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_base_indx->pt_base_indx_entry, 
                     0, 
                     sizeof(AVI_BASE_INDX_ENTRY_T) * pt_base_indx->ui4_entry_num);
            pui1_index_buf = pt_avi_handler->pui1_idx_buf + 24;
            for(ui4_k = 0; ui4_k < pt_base_indx->ui4_entry_num; ui4_k ++)
            {
                LOADL_QWRD(
                    pui1_index_buf + sizeof(AVI_BASE_INDX_ENTRY_T) * ui4_k,
                    pt_base_indx->pt_base_indx_entry[ui4_k].ui8_base_indx_offset);
                LOADL_DWRD(
                    pui1_index_buf + sizeof(AVI_BASE_INDX_ENTRY_T) * ui4_k + 8,
                    pt_base_indx->pt_base_indx_entry[ui4_k].ui4_base_indx_size);
                LOADL_DWRD(
                    pui1_index_buf + sizeof(AVI_BASE_INDX_ENTRY_T) * ui4_k + 12,
                    pt_base_indx->pt_base_indx_entry[ui4_k].ui4_base_indx_length);
            }
            pt_avi_handler->apt_base_indx_info[ui1_i] = pt_base_indx;
        }
        else
        {
            DBG_ERROR(("ERROR: stream base 'indx' size overflow!!! \r\n"));
            return MIDXBULDR_OUT_OF_MEMORY;
        }
    }

    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
    return MIDXBULDR_OK;
}

static INT32 _avi_midxbuld_multi_riff_key_tbl_size_estimate(
                                MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                UINT32          ui4_key_id)
{
    INT32               i4_ret;
    AVI_IDXBULD_OBJ_T*  pt_avi_handler;
    copybytes_fct       pf_copybyte;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    UINT64              ui8_size;
    UINT32              ui4_read_size;
    UINT32              ui4_curr_size;
    UINT32              ui4_cnt;
    UINT8*              pui1_index_buf;
    UINT32              ui4_check = 1;
    UINT32              ui4_size_with_flag;
    UINT8               ui1_i;
    UINT32              ui4_num;

    AVI_BASE_INDX_INFO_T*   pt_key_base_indx_info;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    AVI_BASE_INDX_INFO_T*   pt_key_base_aud_info = NULL;
    AVI_BASE_INDX_ENTRY_T   t_curr_base_aud_entry;
    UINT64                  ui8_aud_base_offset = 0;
#endif
    AVI_BASE_INDX_ENTRY_T   t_curr_base_indx_entry;
    AVI_MULTI_RIFF_INFO_T*  pt_multi_riff;
    AVI_MULTI_RIFF_INFO_T*  pt_multi_riff_last;

    UINT16              ui2_longs_per_entry;
    UINT8               ui1_index_sub_type;
    UINT8               ui1_index_type;
    UINT32              ui4_entries_in_use;
    UINT32              ui4_curr_entries;
    UINT64              ui8_base_offset;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_midxbuld_obj, &ui8_pos);

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_START;
    
    for (ui1_i = 0; ui1_i < MIDXBULD_ELMT_TBL_MAX; ui1_i++)
    {
        pt_key_base_indx_info = pt_avi_handler->apt_base_indx_info[ui1_i];
        if((pt_key_base_indx_info == NULL) || 
           (ui4_key_id == pt_key_base_indx_info->ui4_chunk_id) || 
           _b_is_compatible_vid_id(ui4_key_id, pt_key_base_indx_info->ui4_chunk_id))
        {
            break;
        }
    }
    if(pt_key_base_indx_info == NULL)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    for (ui1_i = 0; ui1_i < MIDXBULD_ELMT_TBL_MAX; ui1_i++)
    {
        pt_key_base_aud_info = pt_avi_handler->apt_base_indx_info[ui1_i];
        if((pt_key_base_aud_info == NULL) || 
           _b_is_compatible_aud_id(pt_key_base_aud_info->ui4_chunk_id, pt_key_base_aud_info->ui4_chunk_id))
        {
            break;
        }
    }
#endif    

    pt_multi_riff = (AVI_MULTI_RIFF_INFO_T*) _mm_util_mem_alloc_with_tag(
                                sizeof(AVI_MULTI_RIFF_INFO_T), MM_MEM_MIDX_TAG);
    if(pt_multi_riff == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_multi_riff, 0, sizeof(AVI_MULTI_RIFF_INFO_T));
    pt_avi_handler->pt_multi_riff_info = pt_multi_riff;
    pt_multi_riff_last = pt_multi_riff;

    for(ui4_num = 0; ui4_num < pt_key_base_indx_info->ui4_entry_num; ui4_num++)
    {        
        t_curr_base_indx_entry = 
            pt_key_base_indx_info->pt_base_indx_entry[ui4_num];
        i4_ret = pf_set_pos(pt_midxbuld_obj, 
                   (INT64)(t_curr_base_indx_entry.ui8_base_indx_offset + 8), 
                   (MEDIA_SEEK_BGN | MEDIA_SEEK_CHK));
        if(i4_ret != INPUTR_OK)
        {
            while(pt_avi_handler->pt_multi_riff_info != NULL)
            {
                pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
                pt_avi_handler->pt_multi_riff_info = 
                    pt_avi_handler->pt_multi_riff_info->pt_next;
                _mm_util_mem_free_with_tag(pt_multi_riff, MM_MEM_MIDX_TAG);
            }
            return MIDXBULDR_INTERNAL_ERR;
        }
        ui4_curr_size = 24; /* AVI2.0 standard index chunk head length=8+24 */
        pf_copybyte(pt_midxbuld_obj, 
                    pt_avi_handler->pui1_idx_buf, 
                    AVI_IDX_BUF_LENGTH,
                    ui4_curr_size, 
                    &ui4_read_size);
        LOADL_WORD(pt_avi_handler->pui1_idx_buf, ui2_longs_per_entry);
        LOAD_BYTE(pt_avi_handler->pui1_idx_buf + 2, ui1_index_sub_type);
        LOAD_BYTE(pt_avi_handler->pui1_idx_buf + 3, ui1_index_type);
        LOADL_DWRD(pt_avi_handler->pui1_idx_buf + 4, ui4_entries_in_use);
        LOADL_QWRD(pt_avi_handler->pui1_idx_buf + 12, ui8_base_offset);

        if((ui2_longs_per_entry != 2) ||
           (ui1_index_sub_type != 0)  ||
           (ui1_index_type != AVI_INDEX_OF_CHUNKS))
        {
            DBG_ERROR(("ERROR: now only support standard index chunk \r\n"));
            /*H02_HD_1080p12_1920x1080x30_8aud_8sub_01.divx, just have 7G valid data, the left data is invalid.*/
            return MIDXBULDR_OK/*MIDXBULDR_INTERNAL_ERR*/;
        }   

        if(ui4_num == 0)
        {
            pt_multi_riff->ui8_indx_base_offset = ui8_base_offset;
            pt_multi_riff->ui4_start_indx_id = 0;
            pt_multi_riff->ui4_indx_num = 0;
            pt_multi_riff->ui4_key_num = 0;
            pt_multi_riff->pt_next = NULL;
        }
        else
        {
            if(ui8_base_offset > pt_multi_riff->ui8_indx_base_offset)
            {
                pt_multi_riff = (AVI_MULTI_RIFF_INFO_T*) _mm_util_mem_alloc_with_tag(
                                    sizeof(AVI_MULTI_RIFF_INFO_T), MM_MEM_MIDX_TAG);
                if(pt_multi_riff == NULL)
                {
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_multi_riff, 0, sizeof(AVI_MULTI_RIFF_INFO_T));
                pt_multi_riff_last->pt_next = pt_multi_riff;
                pt_multi_riff_last = pt_multi_riff;
                pt_multi_riff->ui8_indx_base_offset = ui8_base_offset;
                pt_multi_riff->ui4_start_indx_id = ui4_num;
                pt_multi_riff->ui4_indx_num = 0;
                pt_multi_riff->ui4_key_num = 0;
                pt_multi_riff->pt_next = NULL;
            }
        }
        pt_multi_riff->ui4_indx_num++;

        ui4_curr_entries = 0; /* init current entries num 0 */
        ui8_size = 32; /* standard index chunk head length */
        while (ui8_size < t_curr_base_indx_entry.ui4_base_indx_size)
        {
            if(ui4_curr_entries >= ui4_entries_in_use)
            {
                break;
            }
            ui4_curr_size = (UINT32)(t_curr_base_indx_entry.ui4_base_indx_size - ui8_size);
            if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
            {
                ui4_curr_size = AVI_IDX_BUF_LENGTH;
            }
            pf_copybyte(pt_midxbuld_obj, 
                        pt_avi_handler->pui1_idx_buf, 
                        AVI_IDX_BUF_LENGTH,
                        ui4_curr_size, 
                        &ui4_read_size);
            ui4_cnt = 0;
            while(ui4_cnt < ui4_read_size)
            {
                if(ui4_curr_entries >= ui4_entries_in_use)
                {
                    break;
                }
                pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;                
                LOADL_DWRD(pui1_index_buf + 4, ui4_size_with_flag);
                if ((cui4_aviif_std_index_keyframe&ui4_size_with_flag) == 0)
                {
                    pt_multi_riff->ui4_key_num++; 
                    /* check if abs offset by first key frame */
                    if ((ui4_num == 0) && 
                        (pt_multi_riff->ui4_key_num == ui4_check))
                    {
                        /* For standard index chunk the offset is relative base offset*/
                        pt_avi_handler->b_is_abs_offset = FALSE;                                   
                    }
                }     
                ui4_curr_entries ++;
                ui4_cnt += 8;
            }
            if (ui4_read_size < ui4_curr_size)
            {
                break;
            }
            ui8_size += ui4_curr_size;
        }
        if ((ui4_num + 1) >= (pt_key_base_indx_info->ui4_entry_num / 2))
        {
            pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_MID;
        }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
        do{
            if (pt_key_base_aud_info != NULL)
            {
                t_curr_base_aud_entry = 
                        pt_key_base_aud_info->pt_base_indx_entry[ui4_num];
                i4_ret = pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_curr_base_aud_entry.ui8_base_indx_offset + 8), 
                           (MEDIA_SEEK_BGN | MEDIA_SEEK_CHK));
                if(i4_ret != INPUTR_OK)
                {
                    DBG_ERROR(("ERROR: set base audio offset fail. \r\n"));
                    break;
                }
                ui4_curr_size = 24; /* AVI2.0 standard index chunk head length=8+24 */
                pf_copybyte(pt_midxbuld_obj, 
                            pt_avi_handler->pui1_idx_buf, 
                            AVI_IDX_BUF_LENGTH,
                            ui4_curr_size, 
                            &ui4_read_size);
                
                LOADL_QWRD(pt_avi_handler->pui1_idx_buf + 12, ui8_aud_base_offset);
            }
            pt_multi_riff->ui8_aud_base_offset = ui8_aud_base_offset;
        }while(FALSE);
#endif

    }

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_END;
    
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
    return MIDXBULDR_OK;
}

/* initialize key frame table obj for multi-riff indexing */
static INT32 _avi_midxbuld_multi_riff_key_frame_table_alloc(
                            MIDXBULD_KEY_INFO_T*     pt_key_info, 
                            VOID*                    pv_obj)
{
    INT32                       i4_ret;
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj_last;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt;    /* for multi riff list */
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt_last;
    AVI_MULTI_RIFF_INFO_T*      pt_multi_riff;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT8                       ui1_i;
    UINT32                      ui4_tbl_id;

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;

    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;

    /* allocate elmt tbl range obj memory */
    pt_range_obj = (AVI_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (TRUE != _avi_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_key_info->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_avi_handler->pt_key_range_obj = pt_range_obj;
    
    x_memset(pt_range_obj, 0, sizeof(AVI_IDXBULD_RANGE_OBJ_T));
    pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->ui4_range_id = pt_key_info->ui4_range_id;
    pt_range_obj->b_is_key_frame = TRUE;
    pt_range_obj->b_index_is_valid = FALSE;
    pt_range_obj->ui1_tbl_total = 0;

    t_describ.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
    t_describ.u_info.t_es.ui4_strm_id = pt_key_info->ui4_strm_id;
    t_describ.u_info.t_es.ui4_scale = pt_key_info->ui4_scale;
    t_describ.u_info.t_es.ui4_rate = pt_key_info->ui4_rate;
    t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
    t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_key_info->ui4_avg_bytes_per_sec;

    /* allocate elmt tbl obj memory */
    pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_tbl_obj == NULL)
    {
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &pt_key_info->ui4_tbl_id))
    {
        _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_range_obj->pt_tbl_obj = pt_tbl_obj;
    pt_tbl_obj_last = pt_tbl_obj;

    x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
    
    i4_ret = _avi_midxbuld_multi_riff_key_tbl_size_estimate(
                                pt_midxbuld_obj, 
                                t_describ.u_info.t_es.ui4_strm_id);
    if(i4_ret != MIDXBULDR_OK)
    {
        return i4_ret;
    }

    pt_elmt = &(pt_tbl_obj->t_elmt_tbl);
    pt_elmt_last = pt_elmt;
    pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
    /* key num is 0, no need alloc memory */
    if(pt_multi_riff->ui4_key_num == 0)
    {
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
        pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
        pt_avi_handler->pt_curr_range_obj = pt_range_obj;
        return MIDXBULDR_OK;
    }
    while (pt_multi_riff != NULL)
    {
        if (pt_multi_riff->ui4_start_indx_id > 0)
        {
            pt_elmt = (MIDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(
                                sizeof(MIDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_elmt != NULL)
            {
                x_memset(pt_elmt, 0, sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
                pt_elmt_last->pt_next = pt_elmt;
                pt_elmt_last = pt_elmt;
            }
        }
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
            sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_multi_riff->ui4_key_num, 
            MM_MEM_MIDX_TAG);
        if ((pt_elmt == NULL) || (pt_idx_entry == NULL))
        {
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
            _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_entry, 
                 0, 
                 sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_multi_riff->ui4_key_num);
        pt_elmt->ui8_base_offset = pt_multi_riff->ui8_indx_base_offset;
        pt_elmt->ui4_number_of_entry = 0;
        pt_elmt->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_elmt->u_fmt.t_es.t_info = t_describ.u_info.t_es;
        pt_elmt->u_fmt.t_es.pt_idx_entry = pt_idx_entry;
        pt_elmt->pt_next = NULL;
        pt_tbl_obj->ui4_tbl_size += pt_multi_riff->ui4_key_num;
        
        pt_multi_riff = pt_multi_riff->pt_next;
    }
    pt_tbl_obj->ui8_last_chunk_num = 0;
    pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
    pt_tbl_obj->pt_next = NULL;

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }

            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;
            x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));

            
            pt_elmt = &(pt_tbl_obj->t_elmt_tbl);
            pt_elmt_last = pt_elmt;
            pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
            while (pt_multi_riff != NULL)
            {
                if (pt_multi_riff->ui4_start_indx_id > 0)
                {
                    pt_elmt = (MIDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(
                                sizeof(MIDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
                    if (pt_elmt == NULL)
                    {
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    else
                    {
                        x_memset(pt_elmt, 0, sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
                        pt_elmt_last->pt_next = pt_elmt;
                        pt_elmt_last = pt_elmt;
                    }
                }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                if (pt_midxbuld_obj->t_src_info.b_is_bad_interleaved)
                {
                    pt_elmt->u_fmt.t_as_key.pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                        sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_multi_riff->ui4_key_num, MM_MEM_MIDX_TAG);
                    
                    if ((pt_elmt == NULL) || 
                        (pt_elmt->u_fmt.t_as_key.pt_idx_entry == NULL))
                    {
                        pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry = NULL;
                        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                        pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    x_memset(pt_elmt->u_fmt.t_as_key.pt_idx_entry, 
                             0, 
                             sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_multi_riff->ui4_key_num);
                    /* Enty flag*/
                    pt_elmt->u_fmt.t_as_key.e_entry_type = MIDXBULD_AUD_ALL_ENTRY_TYPE;
                }
                else 
#endif
                {
                    pt_elmt->u_fmt.t_as_key.pui4_pts = (UINT32*) _mm_util_mem_alloc_with_tag(
                        sizeof(UINT32) * pt_multi_riff->ui4_key_num, MM_MEM_MIDX_TAG);
                    
                    if ((pt_elmt == NULL) || 
                        (pt_elmt->u_fmt.t_as_key.pui4_pts == NULL))
                    {
                        pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = NULL;
                        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                        pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    x_memset(pt_elmt->u_fmt.t_as_key.pui4_pts, 
                             0, 
                             sizeof(UINT32) * pt_multi_riff->ui4_key_num);
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                    /* Enty flag*/
                    pt_elmt->u_fmt.t_as_key.e_entry_type = MIDXBULD_AUD_PTS_ENTRY_TYPE;
#endif
                }
                pt_elmt->ui8_base_offset = pt_multi_riff->ui8_aud_base_offset;
                pt_elmt->ui4_number_of_entry = 0;
                pt_elmt->e_tbl_type = MIDXBULD_TBL_TYPE_AUDIO_KEY;
                pt_tbl_obj->ui4_tbl_size += pt_multi_riff->ui4_key_num;
                pt_elmt->u_fmt.t_as_key.t_info 
                    = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i];
                
                pt_multi_riff = pt_multi_riff->pt_next;
            }
            
            pt_tbl_obj->ui8_last_chunk_num = 0;
            pt_tbl_obj->ui4_tbl_id = ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;
        }
    }
    
    pt_avi_handler->pt_curr_range_obj = pt_range_obj;

    pt_avi_handler->ui8_resume_pos = 0;
    pt_avi_handler->ui8_resume_size = 0;

    return MIDXBULDR_OK;
}

#ifndef __NO_FM__
static INT32 _avi_midxbuld_double_key_frame_table_alloc(
                        VOID*                    pv_obj,
                        MIDXBULD_KEY_INFO_T*     pt_key_info)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj_last = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT8                       ui1_i, ui1_j;
    UINT8                       ui1_strm_num;
    UINT32                      aui4_key_num[AVI_MIDXBULD_IDX_STRM_NUM_MAX];
    UINT32                      ui4_tbl_id;
    UINT8                       ui1_map_index = 0;                    

    if(pv_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;      
    }

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;

    /* allocate elmt tbl range obj memory */
    pt_range_obj = (AVI_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (TRUE != _avi_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_range_obj->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_avi_handler->pt_key_range_obj = pt_range_obj;
    
    x_memset(pt_range_obj, 0, sizeof(AVI_IDXBULD_RANGE_OBJ_T));
    pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->b_is_key_frame = TRUE;
    pt_range_obj->b_index_is_valid = FALSE;
    pt_range_obj->ui1_tbl_total = 0;
    pt_range_obj->pt_tbl_obj    = NULL; 

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {        
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)
        {
            t_describ.u_info.t_es.e_es_type             = MIDXBULD_ELEM_STRM_TYPE_KEY;
            t_describ.u_info.t_es.ui4_strm_id           = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
            t_describ.u_info.t_es.ui4_scale             = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_scale;
            t_describ.u_info.t_es.ui4_rate              = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_rate;
            t_describ.u_info.t_es.ui4_sample_sz         = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz;
            t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_avg_bytes_per_sec;

            /* allocate elmt tbl obj memory */
            pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            /* initital the table_obj*/
            x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
            /* allocate elmt table_id */
            if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &pt_tbl_obj->ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }
            /* the set key info*/
            if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id == pt_key_info->ui4_strm_id)
            {
                pt_key_info->ui4_range_id = pt_range_obj->ui4_range_id;
                pt_key_info->ui4_tbl_id   = pt_tbl_obj->ui4_tbl_id;
            }
            /* set the table_obj to range */
            if(pt_range_obj->pt_tbl_obj == NULL)
            {
                pt_range_obj->pt_tbl_obj = pt_tbl_obj;
            }
            else
            {
                pt_tbl_obj_last->pt_next = pt_tbl_obj;
            }
            pt_tbl_obj_last = pt_tbl_obj;
            /* set default value*/
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            aui4_key_num[ui1_map_index] = avi_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, &t_describ);
            pt_tbl_obj->ui4_tbl_size = aui4_key_num[ui1_map_index];
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = t_describ.u_info.t_es;
            if(aui4_key_num[ui1_map_index] == 0)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
                pt_avi_handler->pt_curr_range_obj = pt_range_obj;
                return MIDXBULDR_OK;
            }
            for(ui1_j=0; ui1_j<ui1_map_index; ui1_j++)
            {
                if(aui4_key_num[ui1_j] != aui4_key_num[ui1_map_index])
                {     
                    _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                    pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                    pt_tbl_obj->ui4_tbl_id     = MIDXBULD_NULL_ID;
                    return MIDXBULDR_ENTRY_NOT_EQUAL;
                }
            }
            pt_idx_entry = 
                (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, 
                    MM_MEM_MIDX_TAG);
            if (pt_idx_entry == NULL)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                pt_tbl_obj->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;
        
            pt_tbl_obj->ui8_last_chunk_num = 0;
            pt_tbl_obj->pt_next = NULL;
        
            ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(t_describ.u_info.t_es.ui4_strm_id);
            if (ui1_strm_num >= AVI_MIDXBULD_IDX_STRM_NUM_MAX)
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_map_index;

            if (ui1_map_index >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            pt_avi_handler->apt_tbl_obj_map[ui1_map_index++] = pt_tbl_obj;
        }
    }
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {        
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }  
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }
            if(pt_tbl_obj_last == NULL)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = NULL;
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                pt_tbl_obj->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OK;
            }
            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;
            x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_AUDIO_KEY;
            pt_tbl_obj->ui4_tbl_size = aui4_key_num[0];
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_scale;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_rate;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_sample_sz 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_avg_bytes_per_sec;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts 
                = (UINT32*) _mm_util_mem_alloc_with_tag(sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts == NULL)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = NULL;
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_range_obj->ui4_range_id);
                pt_range_obj->ui4_range_id = MIDXBULD_NULL_ID;
                pt_tbl_obj->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts, 
                     0, 
                     sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size);

            pt_tbl_obj->ui8_last_chunk_num = 0;
            pt_tbl_obj->ui4_tbl_id = ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;
        }
    }
    
    pt_avi_handler->pt_curr_range_obj = pt_range_obj;
    pt_avi_handler->ui8_resume_pos = 0;
    pt_avi_handler->ui8_resume_size = 0;
    
    
    return MIDXBULDR_OK;
}
#endif


/* initialize key frame table obj for indexing */
static INT32 avi_midxbuld_key_frame_table_alloc(
                        MIDXBULD_KEY_INFO_T*     pt_key_info, 
                        VOID*                    pv_obj)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj_last;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT8                       ui1_i;
    UINT8                       ui1_strm_num;
    UINT8                       ui1_index = 0;
    UINT32                      ui4_key_num;
    UINT32                      ui4_tbl_id;
    /*UINT64                      ui8_temp;*/

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;

    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;

    /* allocate elmt tbl range obj memory */
    pt_range_obj = (AVI_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (TRUE != _avi_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_key_info->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_avi_handler->pt_key_range_obj = pt_range_obj;
    
    x_memset(pt_range_obj, 0, sizeof(AVI_IDXBULD_RANGE_OBJ_T));
    pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->ui4_range_id = pt_key_info->ui4_range_id;
    pt_range_obj->b_is_key_frame = TRUE;
    pt_range_obj->b_index_is_valid = FALSE;
    pt_range_obj->ui1_tbl_total = 0;

    t_describ.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
    t_describ.u_info.t_es.ui4_strm_id = pt_key_info->ui4_strm_id;
    t_describ.u_info.t_es.ui4_scale = pt_key_info->ui4_scale;
    t_describ.u_info.t_es.ui4_rate = pt_key_info->ui4_rate;
    t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
    t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_key_info->ui4_avg_bytes_per_sec;

    /* allocate elmt tbl obj memory */
    pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_tbl_obj == NULL)
    {
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &pt_key_info->ui4_tbl_id))
    {
        _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_range_obj->pt_tbl_obj = pt_tbl_obj;
    pt_tbl_obj_last = pt_tbl_obj;

    x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
    pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
    pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    ui4_key_num = avi_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, &t_describ);
    pt_tbl_obj->ui4_tbl_size = ui4_key_num;
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = t_describ.u_info.t_es;
    /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
    /* key num is 0, no need alloc memory */
    if(ui4_key_num == 0)
    {
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
        pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
        pt_avi_handler->pt_curr_range_obj = pt_range_obj;
        return MIDXBULDR_OK;
    }
    pt_idx_entry = 
        (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
            sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, 
            MM_MEM_MIDX_TAG);
    if (pt_idx_entry == NULL)
    {
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;

    pt_tbl_obj->ui8_last_chunk_num = 0;
    /*ui8_temp = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
    pt_tbl_obj->ui4_pts_per_frame = ui8_temp / pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;*/
    pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
    pt_tbl_obj->pt_next = NULL;

    ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(t_describ.u_info.t_es.ui4_strm_id);
    if (ui1_strm_num >= AVI_MIDXBULD_IDX_STRM_NUM_MAX)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_index;
    pt_avi_handler->apt_tbl_obj_map[ui1_index++] = pt_tbl_obj;

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {        
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }

            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;
            x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_AUDIO_KEY;
            pt_tbl_obj->ui4_tbl_size = ui4_key_num;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_scale;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_rate;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_sample_sz 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_avg_bytes_per_sec;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
            /*bad interleaved. allocate base offset key table*/
            if (pt_midxbuld_obj->t_src_info.b_is_bad_interleaved)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry
                    = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
                if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry == NULL)
                {
                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry = NULL;
                    _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry, 
                         0, 
                         sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.e_entry_type = MIDXBULD_AUD_ALL_ENTRY_TYPE;
                
                ui1_strm_num = (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id);

                pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_index;
                pt_avi_handler->apt_tbl_obj_map[ui1_index++] = pt_tbl_obj;
            }
            else
#endif 
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts 
                    = (UINT32*) _mm_util_mem_alloc_with_tag(sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
                if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts == NULL)
                {
                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = NULL;
                    _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts, 
                         0, 
                         sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size);
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.e_entry_type = MIDXBULD_AUD_PTS_ENTRY_TYPE;
#endif
            }
            pt_tbl_obj->ui8_last_chunk_num = 0;
            pt_tbl_obj->ui4_tbl_id = ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;
        }
    }
    
    pt_avi_handler->pt_curr_range_obj = pt_range_obj;

    pt_avi_handler->ui8_resume_pos = 0;
    pt_avi_handler->ui8_resume_size = 0;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_fopen(
                        CHAR*                    ps_path_name, 
                        MIDXBULD_KEY_INFO_T*     pt_key_info, 
                        VOID*                    pv_obj)
{

#ifndef __NO_FM__
    UINT8                   ui1_i;
    INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*      pt_avi_handler;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext_last;
    BOOL                    b_two_key_table = FALSE;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (ps_path_name == NULL) || (pt_key_info == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    if (pt_key_info->ui4_scale == 0 ||
        pt_key_info->ui4_rate == 0 ||
        (pt_key_info->ui4_sample_sz == 1 && pt_key_info->ui4_avg_bytes_per_sec == 0))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    pt_avi_handler = (AVI_IDXBULD_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_handler, 0, sizeof(AVI_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_avi_handler;
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 0777, FALSE, &pt_midxbuld_obj->h_file);
    if (i4_ret != FMR_OK)
    {
        pt_midxbuld_obj->pv_handler_obj = NULL;
        _mm_util_mem_free_with_tag(pt_avi_handler, MM_MEM_MIDX_TAG);
        return MIDXBULDR_INITIAL_ERR;
    }

    pt_avi_handler->ui1_range_count = 0;
    pt_avi_handler->b_is_abs_offset = FALSE;
    pt_avi_handler->ui4_invalid_entry = 0;
    ui1_idx_range_id_last = 1;

    pt_avi_handler->pui1_idx_buf = 
        (UINT8*)_mm_util_mem_alloc_with_tag(sizeof(UINT8)*AVI_IDX_BUF_LENGTH, 
                                            MM_MEM_MIDX_TAG);
    if(pt_avi_handler->pui1_idx_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_handler->pui1_idx_buf, 0, sizeof(UINT8)*AVI_IDX_BUF_LENGTH);

    x_memset(pt_avi_handler->aui1_idx_strm_num_map, 
             0xff, 
             AVI_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_avi_handler->apt_tbl_obj_map, 
             0, 
             sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);
  
    /* key table extern aduio */
    pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                            (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
    if(pt_key_tbl_ext == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_tbl_ext, 
             0, 
             sizeof(MIDXBULD_KEY_TBL_EXT_T));
    pt_avi_handler->pt_key_tbl_ext = pt_key_tbl_ext;
    pt_key_tbl_ext_last = pt_key_tbl_ext;
    pt_key_tbl_ext->pt_next = NULL;
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        /* for two or more video stream */
        if((pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)   
           && (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id != pt_key_info->ui4_strm_id))
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
            b_two_key_table = TRUE;
        }
    }
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
        }
    }

    /* AVI2.0 multi-riff */
    x_memset(pt_avi_handler->apt_base_indx_info,
             0, 
             sizeof(AVI_BASE_INDX_INFO_T*) * MIDXBULD_ELMT_TBL_MAX);
    if((pt_midxbuld_obj->t_src_info.ui4_idx_type & MM_MINFO_IDX_TYPE_STRM) == 
       MM_MINFO_IDX_TYPE_STRM)
    {
        /* stream index type */
        i4_ret = _avi_midxbuld_set_base_indx_info(pt_midxbuld_obj);
        if(i4_ret != MIDXBULDR_OK)
        {
            return i4_ret;
        }
        return _avi_midxbuld_multi_riff_key_frame_table_alloc(pt_key_info, pv_obj);
    }
    else
    {
        /* common index type */
        if(b_two_key_table)
        {
            return _avi_midxbuld_double_key_frame_table_alloc(pv_obj, pt_key_info);
        }
        else
        {
            return avi_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
        }
    }

#else
    return MIDXBULDR_INITIAL_ERR;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_mopen(
                        VOID*                    pv_mem_data,
                        SIZE_T                   z_mem_leng,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        VOID*                    pv_obj)
{
    UINT8                   ui1_i;
    INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*      pt_avi_handler;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext_last;
    BOOL                    b_two_key_table = FALSE;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    if (pt_key_info->ui4_scale == 0 ||
        pt_key_info->ui4_rate == 0 ||
        (pt_key_info->ui4_sample_sz == 1 && pt_key_info->ui4_avg_bytes_per_sec == 0))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    if (z_mem_leng == 0)
    {
        MEDIA_FEEDER_T* pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }

    pt_midxbuld_obj->i8_buff_pos = 0;

    pt_avi_handler = (AVI_IDXBULD_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_handler, 0, sizeof(AVI_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_avi_handler;

    pt_avi_handler->ui1_range_count = 0;
    pt_avi_handler->b_is_abs_offset = FALSE;
    pt_avi_handler->ui4_invalid_entry = 0;
    ui1_idx_range_id_last = 1;

    pt_avi_handler->pui1_idx_buf = 
        (UINT8*)_mm_util_mem_alloc_with_tag(sizeof(UINT8)*AVI_IDX_BUF_LENGTH, 
                                            MM_MEM_MIDX_TAG);
    if(pt_avi_handler->pui1_idx_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_handler->pui1_idx_buf, 0, sizeof(UINT8)*AVI_IDX_BUF_LENGTH);


    x_memset(pt_avi_handler->aui1_idx_strm_num_map, 
             0xff, 
             AVI_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_avi_handler->apt_tbl_obj_map, 
             0, 
             sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);

    /* key table extern aduio */
    pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                            (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
    if(pt_key_tbl_ext == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_tbl_ext, 
             0, 
             sizeof(MIDXBULD_KEY_TBL_EXT_T));
    pt_avi_handler->pt_key_tbl_ext = pt_key_tbl_ext;
    pt_key_tbl_ext_last = pt_key_tbl_ext;
    pt_key_tbl_ext->pt_next = NULL;

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        /* for two or more video stream */
        if((pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)   
           && (pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id != pt_key_info->ui4_strm_id))
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
            b_two_key_table = TRUE;
        }
    }
    
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
        }
    }

    /* AVI2.0 multi-riff */
    x_memset(pt_avi_handler->apt_base_indx_info,
             0, 
             sizeof(AVI_BASE_INDX_INFO_T*) * MIDXBULD_ELMT_TBL_MAX);
    if((pt_midxbuld_obj->t_src_info.ui4_idx_type & MM_MINFO_IDX_TYPE_STRM) == 
       MM_MINFO_IDX_TYPE_STRM)
    {
        /* stream index type */
        i4_ret = _avi_midxbuld_set_base_indx_info(pt_midxbuld_obj);
        if(i4_ret != MIDXBULDR_OK)
        {
            return i4_ret;
        }
        return _avi_midxbuld_multi_riff_key_frame_table_alloc(pt_key_info, pv_obj);
    }
    else
    {
        if(b_two_key_table)
        {
            return _avi_midxbuld_double_key_frame_table_alloc(pv_obj, pt_key_info);
        }
        else
        {
            /* common index type */
            return avi_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
        }
    }    
}    

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_close(VOID* pv_obj)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_ext;
    AVI_MULTI_RIFF_INFO_T*      pt_multi_riff;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    if (pt_midxbuld_obj->h_file)
    {
#ifndef __NO_FM__    
        x_fm_close(pt_midxbuld_obj->h_file);
#endif
        pt_midxbuld_obj->h_file = NULL_HANDLE;
    }
    if (pt_midxbuld_obj->pv_handler_obj != NULL)
    {
        AVI_IDXBULD_OBJ_T*  pt_avi_handler;
        UINT8               ui1_i;

        /* free all range objects */
        pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        while (pt_avi_handler->ui1_range_count > 0)
        {
            _avi_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                         pt_avi_handler->apt_range_obj[0]->ui4_range_id);
        }
        _avi_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                     pt_avi_handler->pt_key_range_obj->ui4_range_id);

        for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
        {
            if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            _mm_util_mem_free_with_tag(pt_avi_handler->apui8_last_chunk_num_map[ui1_i], 
                                       MM_MEM_MIDX_TAG);
        }
        _mm_util_mem_free_with_tag(pt_avi_handler->pui8_key_frame_pos_map, 
                                   MM_MEM_MIDX_TAG);
        _mm_util_mem_free_with_tag(pt_avi_handler->pui1_idx_buf, 
                                   MM_MEM_MIDX_TAG);

        while(pt_avi_handler->pt_key_tbl_ext != NULL)
        {
            pt_key_tbl_ext = pt_avi_handler->pt_key_tbl_ext;
            pt_avi_handler->pt_key_tbl_ext = pt_avi_handler->pt_key_tbl_ext->pt_next;
            _mm_util_mem_free_with_tag(pt_key_tbl_ext,
                                       MM_MEM_MIDX_TAG);
        }

        /* AVI2.0 multi-riff */
        for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
        {
            if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            if(pt_avi_handler->apt_base_indx_info[ui1_i] != NULL)
            {
                _mm_util_mem_free_with_tag(
                    pt_avi_handler->apt_base_indx_info[ui1_i]->pt_base_indx_entry, 
                    MM_MEM_MIDX_TAG);
                _mm_util_mem_free_with_tag(
                    pt_avi_handler->apt_base_indx_info[ui1_i], 
                    MM_MEM_MIDX_TAG);
            }
        }
        while(pt_avi_handler->pt_multi_riff_info != NULL)
        {
            pt_multi_riff = pt_avi_handler->pt_multi_riff_info;
            pt_avi_handler->pt_multi_riff_info = 
                pt_avi_handler->pt_multi_riff_info->pt_next;
            _mm_util_mem_free_with_tag(pt_multi_riff, MM_MEM_MIDX_TAG);
        }
    
        _mm_util_mem_free_with_tag(pt_midxbuld_obj->pv_handler_obj, 
                                   MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
    }
    return MIDXBULDR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_start_indexing
 *
 * Description: This function start indexing
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    MIDXBULD_OBJ_T*    pt_midxbuld_obj;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*build index array*/
    if((pt_midxbuld_obj->t_src_info.ui4_idx_type & MM_MINFO_IDX_TYPE_STRM) == 
       MM_MINFO_IDX_TYPE_STRM)
    {
        /* stream index type */
        return avi_midxbuld_multi_riff_indexing(pt_midxbuld_obj, pui4_range_id);
    }
    else
    {
        /* common index type */
        return avi_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
    }
} 

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_get_info(
                        VOID*                    pv_obj,   
                        MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*  pt_avi_handler;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) 
        || (pz_buf_leng== NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    switch (e_info_type)
    {
    case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
        return avi_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj, (MIDXBULD_ELMT_TBL_INFO_T*) pv_buf);

    case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
        return avi_midxbuld_get_key_idx_tbl_list(pt_midxbuld_obj, (MIDXBULD_KEY_TBL_INFO_T*)pv_buf);
        
    case MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS:
        *(UINT8*)pv_buf = pt_avi_handler->ui1_curr_buld_progress;
        return MIDXBULDR_OK;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    case MIDXBULD_GET_INFO_TYPE_AUDIO_START_INFO:
        return _avi_midxbuld_get_start_info(pv_obj, 
                                            MIDXBULD_TBL_TYPE_AUDIO_KEY,
                                            pv_buf,
                                            *pz_buf_leng);
    case MIDXBULD_GET_INFO_TYPE_VIDEO_START_INFO:
        return _avi_midxbuld_get_start_info(pv_obj, 
                                            MIDXBULD_TBL_TYPE_ELMT_STRM,
                                            pv_buf,
                                            *pz_buf_leng);    
#endif
    default:
        return MIDXBULDR_INV_ARG;
    }
}

static INT32 avi_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl)
{
    
    AVI_IDXBULD_OBJ_T*          pt_avi_handler = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_obj = NULL;

    if (pt_midxbuld_obj == NULL || pt_key_tbl == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    pt_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
    pt_key_tbl_obj = pt_avi_handler->pt_key_tbl_ext;
    
    pt_key_tbl->pt_key_tbl_ext = pt_avi_handler->pt_key_tbl_ext;
    
    while(pt_tbl_obj != NULL && pt_key_tbl_obj != NULL)
    {
        pt_key_tbl_obj->t_elmt_tbl = pt_tbl_obj->t_elmt_tbl;
        if(pt_tbl_obj->t_elmt_tbl.e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        }
        else
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts;
        }
        pt_tbl_obj = pt_tbl_obj->pt_next;
        pt_key_tbl_obj = pt_key_tbl_obj->pt_next;
    }

    return MIDXBULDR_OK;
}

static INT32 avi_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl)
{
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    UINT8                       ui1_i;
    BOOL                        b_id_match = FALSE;

    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if (pt_avi_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == pt_elmt_tbl->ui4_tbl_id)
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;
    }
    else if (pt_avi_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_avi_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= AVI_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            pt_obj = pt_avi_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
            while (pt_obj != NULL)
            {
                if (pt_obj->ui4_tbl_id == pt_elmt_tbl->ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_tbl_obj = pt_obj;
                    pt_obj = NULL;
                }
                else
                {
                    pt_obj = pt_obj->pt_next;
                }
            }

            if (b_id_match)
            {
                break;
            }
        }
        
    }
    if ((FALSE == b_id_match) || (NULL == pt_tbl_obj))
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_elmt_tbl->t_elmt_tbl.ui8_base_offset = pt_tbl_obj->t_elmt_tbl.ui8_base_offset;
    pt_elmt_tbl->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
    return MIDXBULDR_OK;
}

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
static INT32 _avi_midxbuld_get_start_info(
                MIDXBULD_OBJ_T*                  pt_midxbuld_obj,
                MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type,
                VOID*                            pt_start_info,
                SIZE_T                           z_info_size)
{
    AVI_IDXBULD_OBJ_T*          pt_avi_handler;
    AVI_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    MIXBULD_START_INFO_T*       pt_strt_info        = (MIXBULD_START_INFO_T*)pt_start_info;
    
    if (pt_midxbuld_obj == NULL || pt_strt_info == NULL) 
    {
        return MIDXBULDR_INV_ARG;
    }
    
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

    pt_tbl_obj = pt_avi_handler->pt_key_range_obj->pt_tbl_obj;

    while(pt_tbl_obj != NULL)
    {
        if (pt_tbl_obj->t_elmt_tbl.e_tbl_type != e_tbl_type)
        {
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        switch (e_tbl_type)
        {
        case MIDXBULD_TBL_TYPE_ELMT_STRM:
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry != NULL)
            {
                pt_strt_info->ui4_offset = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry->ui4_relative_offset;
                pt_strt_info->ui4_1st_pkgnum = 0;
            }
            else
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            
            return MIDXBULDR_OK;
        case MIDXBULD_TBL_TYPE_AUDIO_KEY:
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.e_entry_type == MIDXBULD_AUD_ALL_ENTRY_TYPE &&
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry != NULL)
            {
                pt_strt_info->ui4_offset = (UINT32)pt_tbl_obj->t_elmt_tbl.ui8_base_offset +
                                           pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pt_idx_entry->ui4_relative_offset;
                pt_strt_info->ui4_1st_pkgnum = 0;
            }
            else
            {
                return MIDXBULDR_KEY_TBL_NOT_READY;
            }
            return MIDXBULDR_OK;
        default:
            return MIDXBULDR_INITIAL_ERR;
        }
    }
    
    if (pt_tbl_obj == NULL)
    {
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }
    else
    {
        return MIDXBULDR_OK;
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  avi_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_set_info(
                        VOID*                    pv_obj,   
                        MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    AVI_IDXBULD_OBJ_T*  pt_avi_handler;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) 
        || (pz_buf_leng== NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    switch (e_info_type)
    {
    case MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE:
        return avi_midxbuld_alloc_range(pt_midxbuld_obj, (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pv_buf);

    case MIDXBULD_SET_INFO_TYPE_FREE_RANGE:
        return avi_midxbuld_free_range(pt_midxbuld_obj, (UINT32)pv_buf);

    default:
        return MIDXBULDR_INV_ARG;
    }
}

static UINT32 avi_midxbuld_key_tbl_size_estimate(
                              MIDXBULD_OBJ_T* pt_midxbuld_obj,
                              UINT32          ui4_key_id)
{
    INT32               i4_ret;
    AVI_IDXBULD_OBJ_T*  pt_avi_handler;
    copybytes_fct       pf_copybyte;
    input4bytes_fct     pf_input4;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    UINT64              ui8_i = 0;
    UINT32              ui4_size = 0;
    UINT32              ui4_read_size;
    UINT32              ui4_curr_size;
    UINT32              ui4_cnt;
    UINT8*              pui1_index_buf;
    UINT32              ui4_check = 1;
    UINT32              ui4_chunk_id;
    UINT32              ui4_flags;
    UINT8               ui1_strm_num = 0;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return 0;
    }    
    pt_avi_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_avi_handler == NULL)
    {
        return 0;
    }

    if(((UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_offset) != ((UINT32)0xFFFFFFFF))
    {
        pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
        pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
        pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
        pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;

        pf_get_pos(pt_midxbuld_obj, &ui8_pos);
        
        pf_set_pos(pt_midxbuld_obj, (INT64)(pt_midxbuld_obj->t_src_info.ui8_idx_offset + 8), FM_SEEK_BGN);

        pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_START;
      
        while (ui8_i < pt_midxbuld_obj->t_src_info.ui8_idx_size)
        {
            ui4_curr_size = (UINT32)(pt_midxbuld_obj->t_src_info.ui8_idx_size - ui8_i);
            if(ui4_curr_size >= AVI_IDX_BUF_LENGTH)
            {
                ui4_curr_size = AVI_IDX_BUF_LENGTH;
            }
            pf_copybyte(pt_midxbuld_obj, 
                        pt_avi_handler->pui1_idx_buf, 
                        AVI_IDX_BUF_LENGTH,
                        ui4_curr_size, 
                        &ui4_read_size);
            ui4_cnt = 0;
            while(ui4_cnt < ui4_read_size)
            {
                pui1_index_buf = pt_avi_handler->pui1_idx_buf + ui4_cnt;
                LOADL_DWRD(pui1_index_buf, ui4_chunk_id);
                if ((ui4_key_id == ui4_chunk_id) || 
                    _b_is_compatible_vid_id(ui4_key_id, ui4_chunk_id))
                {
                    LOADL_DWRD(pui1_index_buf + 4, ui4_flags);
                    if ((cui4_aviif_keyframe&ui4_flags) == cui4_aviif_keyframe)
                    {
                        ui4_size++;
                        if (ui4_size == ui4_check)
                        {
                            UINT32 ui4_offset;
                            UINT64 ui8_tmp_pos;

                            pf_get_pos(pt_midxbuld_obj, &ui8_tmp_pos);
                            
                            LOADL_DWRD(pui1_index_buf + 8, ui4_offset);
                                
                            pf_set_pos(pt_midxbuld_obj, (INT64)ui4_offset, FM_SEEK_BGN);

                            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_chunk_id);
                            if(i4_ret != INPUTR_OK)
                            {
                                return 0;
                            }
                            if ((ui4_key_id == ui4_chunk_id) || 
                                _b_is_compatible_vid_id(ui4_key_id, ui4_chunk_id))
                            {
                                pt_avi_handler->b_is_abs_offset = TRUE;
                            }
                            else
                            {
                                ui4_offset += pt_midxbuld_obj->t_src_info.ui8_data_offset;
                                pf_set_pos(pt_midxbuld_obj, (INT64)ui4_offset, FM_SEEK_BGN);
                                
                                i4_ret = pf_input4(pt_midxbuld_obj, &ui4_chunk_id);
                                if(i4_ret != INPUTR_OK)
                                {
                                    return 0;
                                }
                                if ((ui4_key_id == ui4_chunk_id) || 
                                    _b_is_compatible_vid_id(ui4_key_id, ui4_chunk_id))
                                {
                                    pt_avi_handler->b_is_abs_offset = FALSE;
                                }
                                else /* impossible case */
                                {
                                    ui4_check++;
                                    pt_avi_handler->ui4_invalid_entry++;
                                }
                            }
                            pf_set_pos(pt_midxbuld_obj, (INT64)ui8_tmp_pos, FM_SEEK_BGN);
                        }
                    }            
                }

                ui4_cnt += 16;
            }
            if (ui4_read_size < ui4_curr_size)
            {
                break;
            }
            pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_MID;
            
            ui8_i += ui4_curr_size;
        }
        pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
    }
    else
    {
        pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
        pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
        pf_get_pos(pt_midxbuld_obj, &ui8_pos);
        
        pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_MID;
        pt_avi_handler->b_is_abs_offset = FALSE;
        ui4_size = 1000;
    }

    /* allocate last chunk number map memory for every stream */
    for (ui1_strm_num = 0; ui1_strm_num < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_strm_num++)
    {
        if (ui1_strm_num >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        pt_avi_handler->apui8_last_chunk_num_map[ui1_strm_num] = 
            (UINT64*) _mm_util_mem_alloc_with_tag(sizeof(UINT64) * ui4_size, MM_MEM_MIDX_TAG);
    }

    pt_avi_handler->pui8_key_frame_pos_map = 
        (UINT64*) _mm_util_mem_alloc_with_tag(sizeof(UINT64) * ui4_size, MM_MEM_MIDX_TAG);

    pt_avi_handler->ui1_curr_buld_progress = AVI_MIDXBULD_ESTIMATE_KEY_SIZE_END;
    
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
    
    return ui4_size;
}

static UINT32 avi_midxbuld_chunk_size_estimate(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_chunk_id)
{
    INT32               i4_ret;
    input4bytes_fct     pf_input4;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    UINT64              ui8_i = 0;
    UINT32              ui4_chunk_size = 0;
    UINT32              ui4_latest_chunk_size = 0;
    UINT32              ui4_curr_id;


    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_midxbuld_obj, &ui8_pos);
    
    pf_set_pos(pt_midxbuld_obj, (INT64)(pt_midxbuld_obj->t_src_info.ui8_idx_offset + 8), FM_SEEK_BGN);

    while (ui8_i < pt_midxbuld_obj->t_src_info.ui8_idx_size)
    {
        i4_ret = pf_input4(pt_midxbuld_obj, &ui4_curr_id);
        if(i4_ret != INPUTR_OK)
        {
            return 0;
        }
        if ((ui4_chunk_id == ui4_curr_id) || 
            _b_is_compatible_vid_id(ui4_chunk_id, ui4_curr_id))
        {
            pf_set_pos(pt_midxbuld_obj, (INT64)8, FM_SEEK_CUR);
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_chunk_size);
            if(i4_ret != INPUTR_OK)
            {
                return 0;
            }
            if (ui4_latest_chunk_size == ui4_chunk_size)
            {
                pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
                return ui4_latest_chunk_size;
            }
            ui4_latest_chunk_size = ui4_chunk_size;
        }
        else
        {
            pf_set_pos(pt_midxbuld_obj, 12, FM_SEEK_CUR);
        }
        ui8_i += 16;
    }

    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);
    return 0;
}

static UINT32 avi_midxbuld_elmt_tbl_size_estimate(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        AVI_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                        MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ)
{
    UINT32 ui4_range = 0;
    UINT32 ui4_chunk_size;
    UINT32 ui4_tbl_size = 0;
    UINT64 ui8_denominator, ui8_numerator;
    UINT64 ui8_remainder;

    if (pt_range_obj->u_range.t_es.ui4_start_pts >= pt_range_obj->u_range.t_es.ui4_end_pts)
    {
        return 0;
    }
    
    if (pt_describ->u_info.t_es.e_es_type != MIDXBULD_ELEM_STRM_TYPE_KEY)
    {
        if (pt_range_obj->u_range.t_es.ui4_end_pts < pt_midxbuld_obj->t_src_info.ui4_total_duration)
        {
            ui4_range = pt_range_obj->u_range.t_es.ui4_end_pts - pt_range_obj->u_range.t_es.ui4_start_pts;
        }
        else
        {
            ui4_range = pt_midxbuld_obj->t_src_info.ui4_total_duration - pt_range_obj->u_range.t_es.ui4_start_pts;
        }
    }

    switch (pt_describ->u_info.t_es.e_es_type)
    {
    case MIDXBULD_ELEM_STRM_TYPE_AUDIO:
        {
            if (pt_describ->u_info.t_es.ui4_sample_sz != 0) /* is CBR */
            {
                ui4_chunk_size = avi_midxbuld_chunk_size_estimate(pt_midxbuld_obj, pt_describ->u_info.t_es.ui4_strm_id);
                ui8_denominator = (UINT64)ui4_chunk_size * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                if (ui8_denominator)
                {
                    ui8_numerator = (UINT64)ui4_range * pt_describ->u_info.t_es.ui4_avg_bytes_per_sec;
                    ui4_tbl_size = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, &ui8_remainder);
                    if (ui8_remainder)                        
                    {
                        ui4_tbl_size++;
                    }
                    ui4_tbl_size++;
                }
                else
                {
                    ui4_tbl_size = 0;
                }
            }
            else /* is VBR */
            {
                ui8_denominator = (UINT64)pt_describ->u_info.t_es.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                if (ui8_denominator)
                {
                    ui8_numerator = (UINT64)ui4_range * pt_describ->u_info.t_es.ui4_rate;
                    ui4_tbl_size = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, &ui8_remainder);
                    if (ui8_remainder)                        
                    {
                        ui4_tbl_size++;
                    }
                    ui4_tbl_size++;
                }
                else
                {
                    ui4_tbl_size = 0;
                }
            }
        }
        break;
        
    case MIDXBULD_ELEM_STRM_TYPE_VIDEO:
    case MIDXBULD_ELEM_STRM_TYPE_SUBTITLE:
        {
            ui8_denominator = (UINT64)pt_describ->u_info.t_es.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
            if (ui8_denominator)
            {
                ui8_numerator = (UINT64)ui4_range * pt_describ->u_info.t_es.ui4_rate;
                ui4_tbl_size = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, &ui8_remainder);
                if (ui8_remainder)
                {
                    ui4_tbl_size++;
                }
                ui4_tbl_size++;
            }
            else
            {
                ui4_tbl_size = 0;
            }
        }
        break;

    case MIDXBULD_ELEM_STRM_TYPE_KEY:
        {
            ui4_tbl_size = avi_midxbuld_key_tbl_size_estimate(pt_midxbuld_obj, pt_describ->u_info.t_es.ui4_strm_id);
        }
        break;

    default:
        {
            ui4_tbl_size = 0;
        }
    }

    return ui4_tbl_size;
}

static INT32 avi_midxbuld_alloc_range(
                        MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                        MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc)
{
    AVI_IDXBULD_OBJ_T*           pt_avi_handler;
    AVI_IDXBULD_RANGE_OBJ_T*     pt_range_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj;
    AVI_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj_last;
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ = NULL;
    UINT8                        ui1_tbl_count = 0;
    UINT8                        ui1_strm_num;
    /*UINT64                       ui8_temp;*/

    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;

    if (pt_avi_handler->ui1_range_count >= AVI_MIDXBULD_RANGE_MAX)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >= pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts)
    {
        return MIDXBULDR_INV_ARG;
    }

    if (pt_avi_handler->pt_key_range_obj != NULL &&
        pt_avi_handler->pt_key_range_obj->b_index_is_valid == FALSE)
    {
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }

    x_memset(pt_avi_handler->aui1_idx_strm_num_map, 0xff, AVI_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_avi_handler->apt_tbl_obj_map, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);

    if (pt_elmt_tbl_alloc->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
    {
        /* allocate elmt tbl range obj memory */
        pt_range_obj = (AVI_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
        if (pt_range_obj == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }

        if (TRUE != _avi_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_elmt_tbl_alloc->ui4_range_id))
        {
            _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
            return MIDXBULDR_OUT_OF_HANDLE;
        }

        pt_avi_handler->apt_range_obj[pt_avi_handler->ui1_range_count++] = pt_range_obj;

        x_memset(pt_range_obj, 0, sizeof(AVI_IDXBULD_RANGE_OBJ_T));
        pt_range_obj->e_tbl_type = pt_elmt_tbl_alloc->e_tbl_type;
        pt_range_obj->u_range.t_es = pt_elmt_tbl_alloc->u_range.t_es;
        pt_range_obj->ui4_range_id = pt_elmt_tbl_alloc->ui4_range_id;
        pt_range_obj->b_is_key_frame = FALSE;
        pt_range_obj->b_index_is_valid = FALSE;

        pt_describ = pt_elmt_tbl_alloc->pt_tbl_describ;

        /* allocate elmt tbl obj memory */
        pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
        if (pt_tbl_obj == NULL)
        {
            _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &pt_describ->ui4_tbl_id))
        {
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_HANDLE;
        }

        pt_range_obj->pt_tbl_obj = pt_tbl_obj;
        pt_tbl_obj_last = pt_tbl_obj;

        x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_tbl_obj->ui4_tbl_size = avi_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, pt_describ);
        
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;
        /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = 
            (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
        if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
        {
            _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);

        pt_tbl_obj->ui8_last_chunk_num = 0;
        /*ui8_temp = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        pt_tbl_obj->ui4_pts_per_frame = ui8_temp / pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;*/
        pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
        pt_tbl_obj->pt_next = NULL;

        ui1_strm_num = 
            (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(pt_describ->u_info.t_es.ui4_strm_id);
        if (ui1_strm_num >= AVI_MIDXBULD_IDX_STRM_NUM_MAX)
        {
            return MIDXBULDR_INITIAL_ERR;
        }
        pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_tbl_count;
        pt_avi_handler->apt_tbl_obj_map[ui1_tbl_count++] = pt_tbl_obj;
            
        while (pt_describ->pt_next != NULL)
        {
            pt_describ = pt_describ->pt_next;

            /* allocate elmt tbl obj memory */
            pt_tbl_obj = (AVI_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _avi_midxbuld_alloc_table_id(pt_range_obj, &pt_describ->ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }
            
            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;

            x_memset(pt_tbl_obj, 0, sizeof(AVI_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_tbl_obj->ui4_tbl_size = avi_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, pt_describ);
            
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;
            /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
            {
                _avi_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);

            pt_tbl_obj->ui8_last_chunk_num = 0;
            /*ui8_temp = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
            pt_tbl_obj->ui4_pts_per_frame = ui8_temp / pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;*/
            pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;

            ui1_strm_num = 
                (UINT8)AVI_MIDXBULD_GET_STRM_NUM_FROM_CHUNK_ID(pt_describ->u_info.t_es.ui4_strm_id);
            if (ui1_strm_num >= AVI_MIDXBULD_IDX_STRM_NUM_MAX)
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            pt_avi_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_tbl_count;
            ui1_tbl_count++;
            if (ui1_tbl_count >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            pt_avi_handler->apt_tbl_obj_map[ui1_tbl_count] = pt_tbl_obj;
        }

        pt_avi_handler->pt_curr_range_obj = pt_range_obj;

        pt_avi_handler->ui8_resume_pos = 0;
        pt_avi_handler->ui8_resume_size = 0;
    }
    else
    {
        return MIDXBULDR_INV_ARG;
    }

    return MIDXBULDR_OK;
}


static INT32 avi_midxbuld_free_range(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    AVI_IDXBULD_OBJ_T*  pt_avi_handler;
    
    pt_avi_handler = (AVI_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if (pt_avi_handler->ui1_range_count == 0 ||
        pt_avi_handler->pt_key_range_obj->ui4_range_id == ui4_range_id) /* can NOT free key range */
    {
        return MIDXBULDR_INV_HANDLE;
    }

    if (TRUE == _avi_midxbuld_free_range_obj(pt_midxbuld_obj, ui4_range_id))
    {
        return MIDXBULDR_OK;
    }
    
    return MIDXBULDR_INV_HANDLE;
}

