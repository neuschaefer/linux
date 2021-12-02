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
 * $RCSfile: mp4_midxbuld.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all media index builder API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _MP4_MIDXBULD_H_
#define _MP4_MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mp4_midx_parser.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../midxbuld.h"
#include "mp4_midx_parser.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define MP4_MIDXBULD_RANGE_MAX      (5)
#define MP4_MIDXBULD_ELMT_TBL_MAX   (17) /* 1VID+8AUD+8ST */

#define OPTIMAZE_CODE    1

#define MP4_SUPER_BIG_MASK_1G_H         (~((UINT64)0x3FFFFFFF))
#define MP4_SUPER_BIG_MASK_1G_L         (  (UINT64)0x3FFFFFFF)

#define LITTLE_TO_BIG_DWORD(dwValue)   (((dwValue&0x000000FF)<<24)|     \
                                        ((dwValue&0x0000FF00)<< 8)|     \
                                        ((dwValue&0x00FF0000)>> 8)|     \
                                        ((dwValue&0xFF000000)>>24))
#define MP4_MIDXBULD_BUF_SIZE           (100*1024)      


/*piff structure*/
typedef struct _PIFF_TFHD_TBL_T
{
    UINT32 track_id;
    UINT32 default_sample_count;
    /* add for tfhd box,to store default values;*/
    UINT64 base_data_ofs;
    UINT32 default_sample_duration;
    UINT32 default_sample_size;
}PIFF_TFHD_TBL_T;

typedef struct _TRUN_QUICK_SEEK_TBL
{
    /*UINT32 moof_num; 1 moof to 1 trun, or just deal with the first trun in a moof*/
    UINT32 trun_ofs; /*one moof to n trun*/
    UINT32 trun_elem_ofs; 
    UINT32 cur_trun_cnt;
    UINT32 moof_seq_num;
    UINT32 mdat_elem_ofs;
    UINT32 key_sample_size;    
    BOOL  sample_duration_prest;
    BOOL  sample_size_prest;          
    BOOL  sample_flages_prest;                               
    BOOL  sample_compt_ofs_prest; 
    
    struct TRUN_QUICK_SEEK_TBL* pt_next;
} TRUN_QUICK_SEEK_TBL;

/*****************************************************/

/* PCM audio sample infor*/
typedef struct _MP4_PCM_AUDIO_SAMPLE_SIZE_DURATION_INFO_T
{/*Info from STSC box*/
    UINT32 ui4_sample_start_idx;
    UINT32 ui4_sample_count;
    UINT32 ui4_sample_size_duration;
}MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T;

typedef struct _MP4_PCM_AUDIO_SAMPLE_INFO_T
{
    UINT32                                  ui4_total_sample_num;
    UINT32*                                 paui4_ofst_buf;
    
    UINT32                                  ui4_size_dur_num;
    MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T*   pat_aud_pcm_size_dur;
}MP4_PCM_AUDIO_SAMPLE_INFO_T;

typedef struct _MP4_BOX_ATTR_T
{
    BOOL   b_is_box_exist;
    UINT64  ui8_box_offset;
    UINT32 ui4_box_size;
}MP4_BOX_ATTR_T;

typedef struct _MP4_IDXBULD_TRACK_T
{
    UINT32 ui4_track_id;
    UINT32 ui4_default_sample_size;/* to reduce read time */
    MP4_BOX_ATTR_T t_stts; /*time to sample */
    MP4_BOX_ATTR_T t_stsc; /* sample to chunk */
    MP4_BOX_ATTR_T t_stsz; /* sample size*/
    MP4_BOX_ATTR_T t_stco; /* chunk offset */
    MP4_BOX_ATTR_T t_co64; /* 64bit chunk offset */
    MP4_BOX_ATTR_T t_ctts; /* composition time to sample */
    MP4_BOX_ATTR_T t_stss; /* sync sample table */
    MP4_PCM_AUDIO_SAMPLE_INFO_T t_pcm_sam_inf;
} MP4_IDXBULD_TRACK_T;

typedef struct _MP4_STTS_TBL_T
{
    UINT32 ui4_sample_count;
    UINT32 ui4_sample_duration;
    UINT32 ui4_starting_sample;
    UINT64 ui8_starting_total_duration;
}MP4_STTS_TBL_T;

typedef struct _MP4_RAW_CTTS_TBL_T
{
    UINT32 ui4_sample_count;
    UINT32 ui4_sample_offset;
}MP4_RAW_CTTS_TBL_T;

typedef struct _MP4_CTTS_BUF_T
{
    UINT32              ui4_ctts_entry_num;
    MP4_RAW_CTTS_TBL_T* pt_ctts_item_buf;
}MP4_CTTS_BUF_T;

#if (OPTIMAZE_CODE == 1)
typedef struct _MP4_RAW_STTS_TBL_T
{
    UINT32 ui4_sample_count;
    UINT32 ui4_sample_duration;
}MP4_RAW_STTS_TBL_T;

typedef struct _MP4_RAW_STSC_TBL_T
{
    UINT32 ui4_first_chunk;
    UINT32 ui4_samples_per_chunk;
    UINT32 ui4_sample_description_index;
}MP4_RAW_STSC_TBL_T;

typedef struct _MP4_MEM_BUF_T
{
    UINT32  ui4_mem_buf_size;
    VOID*   pv_mem_buffer;
}MP4_MEM_BUF_T;
#endif

typedef struct _MP4_STSC_TBL_T
{
    UINT32 ui4_first_chunk;
    UINT32 ui4_samples_per_chunk;
    UINT32 ui4_sample_description_index;
    UINT32 ui4_first_sample;
    UINT32 ui4_repeat_count;
}MP4_STSC_TBL_T;

typedef struct _MP4_IDXBULD_KEY_EXT_T
{
    UINT32 ui4_key_ext_sample;
    UINT64 ui8_key_ext_duration_sum;
    UINT32 ui4_key_trun_entry_ofs;
    UINT32 ui4_key_ctts;
    UINT32 ui4_key_trun_sample_cnt;
    UINT32 ui4_stts_entry;
    UINT32 ui4_stts_entry_sample;
    UINT32 ui4_stsc_entry;
    UINT32 ui4_stsc_entry_sample;
}MP4_IDXBULD_KEY_EXT_T;


typedef struct 
{
    UINT32  ui4_pts;             /* might be invalid */
    UINT64  ui8_relative_offset; /* ui8_relative_offset */
    UINT32  ui4_size;            /* pure data size, not include header */
} MP4_MIDXBULD_ELMT_STRM_ENTRY_T;

typedef struct _MP4_IDXBULD_KEY_EXT_TBL_T
{
    UINT32 ui4_track_id;
    UINT64 ui8_base_offset;  /* in case of file size > 4G */
    UINT32 ui4_key_num;      /* valid number of key entry */
    MP4_MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry;
    MP4_IDXBULD_KEY_EXT_T*      pt_key_ext;
    MP4_CTTS_BUF_T              t_ctts_buf; /*used to store ctts table*/
    struct _MP4_IDXBULD_KEY_EXT_TBL_T* pt_next;
}MP4_IDXBULD_KEY_EXT_TBL_T;

typedef struct _MP4_IDXBULD_ELMT_TBL_OBJ_T
{
    UINT32 ui4_tbl_size; /* total number of entry in table */
    UINT32 ui4_start_sample; /* start sample for indexing */
    UINT32 ui4_end_sample;   /* end sample for indexing */
    UINT32 ui4_start_pts;    /* start pts for indexing */
    UINT32 ui4_end_pts;      /* end pts for indexing */
    UINT32 ui4_tbl_id;
    UINT8 ui1_is_key_exist;  /* whether key table is exist */
    MIDXBULD_ELMT_TBL_OBJ_T     t_elmt_tbl;
    /*for piff inet*/
    TRUN_QUICK_SEEK_TBL*             trun_qseek_tbl; 
    PIFF_TFHD_TBL_T*                 tfhd_box_info;
    struct _MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_next;
} MP4_IDXBULD_ELMT_TBL_OBJ_T;

typedef struct _MP4_IDXBULD_RANGE_OBJ_T
{
    MIDXBULD_ELMT_TBL_TYPE_T        e_tbl_type;
    union 
    {
        MIDXBULD_ELMT_STRM_RANGE_T  t_es;
    } u_range;
    UINT32                          ui4_range_id;
    BOOL                            b_is_key_frame;
    BOOL                            b_index_is_valid;
    UINT8                           ui1_tbl_total;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj;
} MP4_IDXBULD_RANGE_OBJ_T;

typedef struct 
{
    BOOL                            b_is_moov_empty;
    UINT32                          ui4_mln_moov_sample_cnt;
    UINT32                          ui4_mln_moov_key_cnt;
    UINT32                          ui4_mln_moov_dur;
} MP4_IDXBULD_MARLIN_MOOV_INF_T;

/* This structure holds the obj of mp4 idxbuild info. */
typedef struct _MP4_IDXBULD_OBJ_T
{
    MP4_IDXBULD_RANGE_OBJ_T*        pt_curr_range_obj;
    MP4_IDXBULD_RANGE_OBJ_T*        pt_key_range_obj;
    MP4_IDXBULD_RANGE_OBJ_T*        apt_range_obj[MP4_MIDXBULD_RANGE_MAX];
    UINT8                           ui1_range_count;

    /* The pointer to the box list */
    MP4_BOX_T*                      pt_tree_box;
    UINT32                          ui4_malloc_count;

    UINT64                          ui8_curr_parser_offset;
    MP4_PARSER_STATE_ENUM_T         e_midx_parser_state;

    /* for key extern list */
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext;
#if (OPTIMAZE_CODE == 1)   
    MP4_MEM_BUF_T                   t_mem_buf;/*mem buffer*/
#endif
    
    UINT8                           ui1_valid_track_cnt;
    MP4_IDXBULD_TRACK_T             t_track_info_box[MIDXBULD_ELMT_TBL_MAX];
    
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc;

    /* for piff & inet piff*/
    UINT32                          piff_inet_buf_moof_cnt;    
    PIFF_BOX_INET_INFO_T*           piff_inet_moof_info;    
    UINT64                          piff_inet_last_duration[MIDXBULD_ELMT_TBL_MAX];
    UINT32                          piff_inet_last_sample[MIDXBULD_ELMT_TBL_MAX];
    
    MP4_IDXBULD_MARLIN_MOOV_INF_T   t_mln_moov_info;

    MP4_IDXBULD_KEY_EXT_TBL_T*      pt_key_ext_tbl;    
    UINT8                           ui1_curr_buld_progress;
    
    UINT8                       ui1_curr_file_idx;
    MIDXBULD_KEY_INFO_T*        pt_key_info;
    MP4_SINGLE_FILE_INFO_T      t_cur_file_info;
} MP4_IDXBULD_OBJ_T;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This function open a handler for a file */
extern INT32 mp4_midxbuld_fopen (
                              CHAR*                ps_path_name,
                              MIDXBULD_KEY_INFO_T* pt_key_info,
                              VOID*                pv_obj);

/* This function open a handler for a memory buffer */
extern INT32 mp4_midxbuld_mopen (
                              VOID*                pv_mem_data,
                              SIZE_T               z_mem_leng,
                              MIDXBULD_KEY_INFO_T* pt_key_info,
                              VOID*                pv_obj);

/* This function close a handler */
extern INT32 mp4_midxbuld_close (VOID* pv_obj); 

/* This function start indexing */
extern INT32 mp4_midxbuld_start_indexing (VOID* pv_obj, UINT32* pui4_range_id); 

/* This API retrieves the information of media idx builder handle. * 
 *It will return MINFO_NOT_READY, before parse media complete.     */
extern INT32 mp4_midxbuld_get_info (
                              VOID*                    pv_obj,   
                              MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                              VOID*                    pv_buf,     
                              SIZE_T*                  pz_buf_leng); 

/* This API retrieves the information of media idx builder handle. *
 * It will return MINFO_NOT_READY, before parse media complete.    */
extern INT32 mp4_midxbuld_set_info (
                              VOID*                    pv_obj,   
                              MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                              VOID*                    pv_buf,     
                              SIZE_T*                  pz_buf_leng); 

extern VOID _mp4_free_key_ext_tbl(MP4_IDXBULD_KEY_EXT_TBL_T* pt_key_ext_head);

extern BOOL _mp4_midxbuld_alloc_range_id (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32*                  pui4_range_id );
extern BOOL _mp4_midxbuld_free_range_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id);

extern BOOL _mp4_midxbuld_alloc_table_id (
                        MP4_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                        UINT32*                  pui4_tbl_id );
extern UINT32 mp4_midxbuld_key_tbl_size_estimate(
                              MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                              UINT32          ui4_key_id);
                              
extern UINT32 mp4_midxbuld_elmt_tbl_size_estimate(
                              MIDXBULD_OBJ_T*               pt_midxbuld_obj,
                              MP4_IDXBULD_RANGE_OBJ_T*      pt_range_obj,
                              MP4_IDXBULD_ELMT_TBL_OBJ_T*   pt_curr_tbl_obj,
                              MIDXBULD_ELMT_TBL_DESCRIB_T*  pt_describ);

extern INT32 mp4_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T*    pt_elmt_tbl);

extern INT32 mp4_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl);

extern INT32 mp4_midxbuld_alloc_range(
                        MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                        MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc);
                              
extern INT32 mp4_midxbuld_free_range(
                              MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                              UINT32            ui4_range_id);
extern INT32 _mp4_reorder_key_tbl(MP4_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj, 
                                  MP4_IDXBULD_KEY_EXT_TBL_T*   pt_key_ext_t);

extern INT32 _mp4_midxbuld_mem_alloc(MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                                    VOID**              pv_buf,
                                    UINT32             ui4_size);

extern INT32 _mp4_midxbuld_mem_free(MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                                    VOID*             pv_buf);

extern INT32 piff_inet_get_data(MIDXBULD_OBJ_T*  pt_midx_obj);

extern INT32 piff_get_key_from_mfra(MIDXBULD_OBJ_T*  pt_midxbuld_obj, MIDXBULD_KEY_INFO_T* pt_key_info);

extern INT32 piff_get_trun_info(MIDXBULD_OBJ_T*         pt_midx_obj, 
                                MIDXBULD_KEY_INFO_T*    pt_key_info,
                                UINT64                  ui4_ofs_len, 
                                UINT64                  ui4_time_len,
                                UINT32                  ui4_key_num_max);

extern INT32 piff_midxbuld_key_frame_table_alloc(MIDXBULD_KEY_INFO_T* pt_key_info, 
                                                 VOID*                pv_obj);

extern INT32 piff_midxbuld_key_ext_table_build(
                            MIDXBULD_OBJ_T* pt_midxbuld_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info);

extern INT32 piff_midxbuld_elmt_tbl_size_estimate(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                     MP4_IDXBULD_RANGE_OBJ_T*       pt_range_obj, 
                                     MP4_IDXBULD_ELMT_TBL_OBJ_T*    pt_tbl_obj, 
                                     MIDXBULD_ELMT_TBL_DESCRIB_T*   pt_describ);

extern INT32 piff_midxbuld_inet_elmt_tbl_size_estimate(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                     MP4_IDXBULD_RANGE_OBJ_T*       pt_range_obj, 
                                     MP4_IDXBULD_ELMT_TBL_OBJ_T*    pt_tbl_obj, 
                                     MIDXBULD_ELMT_TBL_DESCRIB_T*   pt_describ);


extern INT32 piff_inet_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                            UINT32* pui4_range_id); 

extern INT32 piff_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                            UINT32* pui4_range_id);

extern INT32 piff_get_key_info( MIDXBULD_OBJ_T* pt_midx_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info);

extern INT32 mp4_midxbuld_pcm_audio_sample_indexing(
                            MIDXBULD_OBJ_T*                 pt_midxbuld_obj,
                            MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj,
                            MP4_IDXBULD_TRACK_T*            pt_cur_track);


#if (ENABLE_CMOV == 1)
extern INT32 mp4_cmov_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                        UINT32*         pui4_range_id);

extern INT32 mp4_cmov_midxbuld_key_ext_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info);
    
extern UINT32 mp4_cmov_midxbuld_elmt_tbl_size_estimate(
                              MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                              MP4_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                              MP4_IDXBULD_ELMT_TBL_OBJ_T*  pt_curr_tbl_obj,
                              MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _MP4_MIDXBULD_H_ */
