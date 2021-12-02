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
 * $RCSfile: mkv_midxbuld.h,v $
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

#ifndef _MKV_MIDXBULD_H_
#define _MKV_MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/mm_common/rfc_ebml/rfc_ebml.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../midxbuld.h"
#include "../../mm_common/rfc_ebml/rfc_ebml.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define MKV_MIDXBULD_RANGE_MAX         (5)
#define MKV_MIDXBULD_RANGE_ID_MASK     ((UINT32)0xff00)
#define MKV_MIDXBULD_TABLE_ID_MASK     ((UINT32)0xff)
#define MKV_MIDXBULD_STREAM_NUM_MAX     127

/*
    MKV block     format:
    vint          TrackNumber (variable bytes)
    svint16       Timecode    (2bytes)  
    int           Flags       (1 byte)
*/
typedef struct _MKV_IDXBULD_BLOCK_ELE_T
{
    /*EBML_ELE_BASIC_T       t_base;*/
    UINT16                           ui2_stream_id;
    INT64                            i8_timecode;
    UINT64                           ui8_position;
    UINT32                           ui4_block_size;
    
    struct _MKV_IDXBULD_BLOCK_ELE_T* pt_next;
    /*
    key frame: 0x80,  1000 0000
    lace type: 0x06   0000 0110
    invisible: 0x08   0000 1000
    discardable:0x01  0000 0001
    Note: It is not mandated to set key bit even though it is key frame
    */
    /*UINT8                   ui1_flag;*/
    
    /*array of each frame size: details each frame'size*/
     
    /*
      real data: for video generally include one frame while for audio if apply
      lace to the block it may include a number of frames
    */
} MKV_IDXBULD_BLOCK_ELE_T;

/*MKV Simple Block*/
typedef MKV_IDXBULD_BLOCK_ELE_T MKV_IDXBULD_SIMPLE_BLOCK_ELE_T;

/*MKV BlockGroup*/
typedef struct _MKV_IDXBULD_BLOCK_GROUP_ELE_T
{
    MKV_IDXBULD_BLOCK_ELE_T                t_block;
    UINT8                                  ui1_refer_block_num;
    EBML_INT*                              pui8_refer_block;
    EBML_INT                               ui8_duration;
    struct _MKV_IDXBULD_BLOCK_GROUP_ELE_T* pt_next;
} MKV_IDXBULD_BLOCK_GROUP_ELE_T;

/*MKV cluster */
typedef struct _MKV_IDXBULD_CLUSTER_ELE_T
{
    /*EBML_ELE_BASIC_T       t_base;*/
    EBML_UINT                       ui8_base_timecode;
    /*0..n BlockGroup(v1) or SimpleBlock(v2)*/
    EBML_UINT32                     ui4_simple_block_count;
    MKV_IDXBULD_SIMPLE_BLOCK_ELE_T* pt_block_head;
    EBML_UINT32                     ui4_block_group_count;
    MKV_IDXBULD_BLOCK_GROUP_ELE_T*  pt_block_group_head;
    
} MKV_IDXBULD_CLUSTER_ELE_T;

/*MKV CuetrackPosition element */
typedef struct _MKV_IDXBULD_CUE_TRACK_POS_ELE_T
{
    /*EBML_ELE_BASIC_T       t_base;*/
    EBML_UINT16                               ui2_stream_id;
    EBML_UINT64                               ui8_pos;     /*offset into the file*/
    EBML_UINT64                               ui8_cluster_pos;
    EBML_UINT32                               ui4_block_size;
    /*EBML_UINT            ui8_block_num;*/
    struct _MKV_IDXBULD_CUE_TRACK_POS_ELE_T*  pt_next;
} MKV_IDXBULD_CUE_TRACK_POS_ELE_T;

/*MKV Cue Point element */
typedef struct _MKV_IDXBULD_CUE_POINT_ELE_T
{
    /*EBML_ELE_BASIC_T       t_base;*/
    EBML_UINT                             ui8_timecode;
    EBML_UINT16                           ui2_pos_num;
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T*      pt_track_pos_head;
    struct _MKV_IDXBULD_CUE_POINT_ELE_T*  pt_next;
} MKV_IDXBULD_CUE_POINT_ELE_T;

/*MKV Cue element */
typedef struct _MKV_IDXBULD_CUE_ELE_T
{
    /*EBML_ELE_BASIC_T       t_base;*/
    EBML_UINT32                   ui4_index_count;
    MKV_IDXBULD_CUE_POINT_ELE_T*  pt_cue_point_head;
    MKV_IDXBULD_CUE_POINT_ELE_T*  pt_cue_point_tail;    
} MKV_IDXBULD_CUE_ELE_T;


typedef struct _MKV_IDXBULD_ELMT_TBL_OBJ_T
{
    MIDXBULD_ELMT_TBL_OBJ_T             t_elmt_tbl;
    UINT32                              ui4_tbl_size;/* total number of entry in table */
    UINT32                              ui4_tbl_id;

    struct _MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_next;
} MKV_IDXBULD_ELMT_TBL_OBJ_T;

typedef struct _MKV_IDXBULD_RANGE_OBJ_T
{
    MIDXBULD_ELMT_TBL_TYPE_T e_tbl_type;
    union 
    {
        MIDXBULD_ELMT_STRM_RANGE_T t_es;
    } u_range;
    
    UINT32                      ui4_range_id;
    BOOL                        b_is_key_frame;
    BOOL                        b_index_is_valid;
    UINT8                       ui1_tbl_total;
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
} MKV_IDXBULD_RANGE_OBJ_T;


/* This structure holds the obj of MKV idxbuild info. */
typedef struct _MKV_IDXBULD_OBJ_T
{
    MKV_IDXBULD_RANGE_OBJ_T*    pt_curr_range_obj;
    MKV_IDXBULD_RANGE_OBJ_T*    pt_key_range_obj;
    MKV_IDXBULD_RANGE_OBJ_T*    apt_range_obj[MKV_MIDXBULD_RANGE_MAX];
    UINT8                       ui1_range_count;
    UINT8                       ui1_curr_buld_progress;

    /*for current range, get table object by stream id*/
    MKV_IDXBULD_ELMT_TBL_OBJ_T* apt_tbl_obj_map[MKV_MIDXBULD_STREAM_NUM_MAX];
    MIDXBULD_KEY_TBL_INFO_T     t_key_table_obj;
    MIDXBULD_KEY_INFO_T         t_key_info;
    MKV_IDXBULD_CUE_ELE_T*      pt_cue_ele;

    BOOL                        b_key_tbl_ok;

    UINT8                       ui1_idx_range_id_last;
    /*segment info data offset */
    UINT64                      ui8_seg_data_offset;
    /*segment timecode scale*/
    UINT64                      ui8_seg_tm_scale;
    /*store current parsed object buffer*/
    UINT32                      ui4_buf_length;
    UINT32                      ui4_buf_write_pos; 
    INT64                       i8_buf_read_pos; /*may be < 0*/
    UINT64                      ui8_buf_file_pos;
    UINT8*                      pui1_header_buf;
    UINT8*                      pui1_temp_buf;
    
    UINT64                      ui8_file_size;
    struct _MKV_IDXBULD_OBJ_T*  pt_next;
    /*other extension*/
} MKV_IDXBULD_OBJ_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 _mkv_midxbuld_parse_cue_ele(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                         EBML_ELE_BASIC_T*       pt_ele_head,
                                         MKV_IDXBULD_CUE_ELE_T*  pt_cue_elel,
                                         MKV_IDXBULD_OBJ_T*      pt_mkv_handler);

extern INT32  _mkv_midxbuld_key_frame_table_alloc( 
                                        MIDXBULD_KEY_INFO_T*    pt_key_info, 
                                        MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                                        MKV_IDXBULD_OBJ_T*      pt_mkv_handler,
                                        MIDXBULD_SOURCE_INFO_T* pt_src_info);

extern INT32 _mkv_midxbuld_get_idx_elmt_tbl(
                           MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                           MKV_IDXBULD_OBJ_T*        pt_mkv_handler,
                           MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl);

/* This function open a handler for a file */
extern INT32 mkv_midxbuld_fopen (
                              CHAR*                ps_path_name,
                              MIDXBULD_KEY_INFO_T* pt_key_info,
                              VOID*                pv_obj);

/* This function open a handler for a memory buffer */
extern INT32 mkv_midxbuld_mopen (
                                 VOID*                pv_mem_data,
                                 SIZE_T               z_mem_leng,
                                 MIDXBULD_KEY_INFO_T* pt_key_info,
                                 VOID*                pv_obj);
/* This function start indexing */
extern INT32 mkv_midxbuld_start_indexing (VOID* pv_obj, UINT32* pui4_range_id);

/* This API retrieves the information of media idx builder handle.
It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 mkv_midxbuld_get_info ( VOID*                    pv_obj,   
                                     MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                                     VOID*                    pv_buf,     
                                     SIZE_T*                  pz_buf_leng); 

/* This function close a handler */
extern INT32 mkv_midxbuld_close (VOID* pv_obj); 

/* This API retrieves the information of media idx builder handle. 
It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 mkv_midxbuld_set_info (
                                      VOID*                    pv_obj,   
                                      MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                                      VOID*                    pv_buf,     
                                      SIZE_T*                  pz_buf_leng); 

#ifdef __cplusplus
}
#endif

#endif /* _MKV_MIDXBULD_H_ */
