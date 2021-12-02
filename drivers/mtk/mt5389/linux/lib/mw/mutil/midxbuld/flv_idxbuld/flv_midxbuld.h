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
 * $RCSfile: flv_midxbuld.h,v $
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

#ifndef _FLV_MIDXBULD_H_
#define _FLV_MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/midxbuld/midxbuld.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../midxbuld.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define FLV_MIDXBULD_IDX_STRM_NUM_MAX (99)
#define FLV_MIDXBULD_RANGE_MAX (5)
#define FLV_MIDXBULD_ELMT_TBL_MAX (17) /* 1VID+8AUD+8ST */

#define FLV_IDX_BUF_LENGTH     102400  /* 16*6400 per time */

/* for FLV2.0 index type define */
#define FLV_INDEX_OF_INDEXES    0x00
#define FLV_INDEX_OF_CHUNKS     0x01
#define FLV_INDEX_IS_DATA       0x80
#define FLV_INDEX_2FIELD        0x01

typedef struct _FLV_IDXBULD_ELMT_TBL_OBJ_T
{
    UINT32      ui4_tbl_size;            /* total number of entry in table */
    UINT64      ui8_last_chunk_num;      /* last chunk number for pts calculation */
    MIDXBULD_ELMT_TBL_OBJ_T             t_elmt_tbl;
    UINT32                              ui4_tbl_id;
    struct _FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_next;
} FLV_IDXBULD_ELMT_TBL_OBJ_T;

typedef struct _FLV_IDXBULD_RANGE_OBJ_T
{
    MIDXBULD_ELMT_TBL_TYPE_T e_tbl_type;
    union 
    {
        MIDXBULD_ELMT_STRM_RANGE_T t_es;
    } u_range;
    UINT32 ui4_range_id;
    BOOL b_is_key_frame;
    BOOL b_index_is_valid;
    UINT8 ui1_tbl_total;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
} FLV_IDXBULD_RANGE_OBJ_T;

/*
typedef struct _FLV_IDXBULD_RANGE_OBJ_LIST_T
{
    FLV_IDXBULD_RANGE_OBJ_T* pt_obj;
    FLV_IDXBULD_RANGE_OBJ_LIST_T* pt_next;
} FLV_IDXBULD_RANGE_OBJ_LIST_T;
*/

typedef struct _FLV_BASE_INDX_ENTRY_T
{
    UINT64  ui8_base_indx_offset;
    UINT32  ui4_base_indx_size;
    UINT32  ui4_base_indx_length;
}FLV_BASE_INDX_ENTRY_T;

typedef struct _FLV_BASE_INDX_INFO_T
{
    UINT16  ui2_longs_per_entry;
    UINT8   ui1_indx_sub_type;
    UINT8   ui1_indx_type;
    UINT32  ui4_entry_num;
    UINT32  ui4_chunk_id;
    FLV_BASE_INDX_ENTRY_T*  pt_base_indx_entry;
}FLV_BASE_INDX_INFO_T;

typedef struct _FLV_MULTI_RIFF_INFO_T
{
    UINT64  ui8_movi_offset;
    UINT64  ui8_indx_base_offset;
    UINT32  ui4_start_indx_id;
    UINT32  ui4_indx_num;
    UINT32  ui4_key_num;
    struct _FLV_MULTI_RIFF_INFO_T*  pt_next;
}FLV_MULTI_RIFF_INFO_T;

/* This structure holds the obj of flv idxbuild info. */
typedef struct _FLV_IDXBULD_OBJ_T
{
    FLV_IDXBULD_RANGE_OBJ_T* pt_curr_range_obj;
    FLV_IDXBULD_RANGE_OBJ_T* pt_key_range_obj;
    FLV_IDXBULD_RANGE_OBJ_T* apt_range_obj[FLV_MIDXBULD_RANGE_MAX];
    UINT8                    ui1_range_count;
    /*FLV_IDXBULD_RANGE_OBJ_LIST_T* pt_range_obj_list;*/

    /*buf to store index data */
    UINT8* pui1_idx_buf;

    /* for key extern list */
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_ext;
#if 0
    UINT8 aui1_idx_strm_num_map[FLV_MIDXBULD_IDX_STRM_NUM_MAX];
    FLV_IDXBULD_ELMT_TBL_OBJ_T* apt_tbl_obj_map[MIDXBULD_ELMT_TBL_MAX];

    /* FLV2.0 */
    FLV_BASE_INDX_INFO_T*       apt_base_indx_info[MIDXBULD_ELMT_TBL_MAX];
    FLV_MULTI_RIFF_INFO_T*      pt_multi_riff_info;

    UINT64* apui8_last_chunk_num_map[MIDXBULD_ELMT_TBL_MAX];
    UINT64* pui8_key_frame_pos_map;
#endif
    UINT64 ui8_resume_pos;
    UINT64 ui8_resume_size;

    UINT8  ui1_curr_buld_progress;
    
    BOOL b_is_abs_offset;
    UINT32 ui4_invalid_entry;
} FLV_IDXBULD_OBJ_T;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This function open a handler for a file */
extern INT32 flv_midxbuld_fopen (
                        CHAR*                    ps_path_name,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        VOID*                    pv_obj);

/* This function open a handler for a memory buffer */
extern INT32 flv_midxbuld_mopen (
                        VOID*                    pv_mem_data,
                        SIZE_T                   z_mem_leng,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        VOID*                    pv_obj);

/* This function close a handler */
extern INT32 flv_midxbuld_close (VOID* pv_obj); 

/* This function start indexing */
extern INT32 flv_midxbuld_start_indexing (VOID* pv_obj, UINT32* pui4_range_id); 

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 flv_midxbuld_get_info (
                        VOID*                    pv_obj,   
                        MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng); 

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 flv_midxbuld_set_info (
                        VOID*                    pv_obj,   
                        MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng); 

#ifdef __cplusplus
}
#endif

#endif /* _FLV_MIDXBULD_H_ */
