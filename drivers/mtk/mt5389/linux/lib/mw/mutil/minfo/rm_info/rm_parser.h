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
 * rm_parser.h,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the rm parser function
 *---------------------------------------------------------------------------*/

#ifndef _RM_PARSER_H_
#define _RM_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../../mfmtrecg/u_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/

#define RM_BOX_BUF_LENGTH     1024
#define RM_STREAM_MAX                       0x0100/*tmp*/

#define PROP_FLAG_PN_SAVE_ENABLED           0x0001
#define PROP_FLAG_PN_PERFECT_PLAY_ENABLED   0x0002
#define PROP_FLAG_PN_LIVE_BROADCAST         0x0004

#define DATA_PKT_PN_RELIABLE_FLAG           0x0001
#define DATA_PKT_PN_KEYFRAME_FLAG           0x0002

typedef enum
{
    RM_STRM_TYPE_UNKNOWN,
    RM_STRM_TYPE_VIDEO,
    RM_STRM_TYPE_AUDIO,
    RM_STRM_TYPE_SUBTL,
    RM_STRM_TYPE_LOGIC_VIDEO,
    RM_STRM_TYPE_LOGIC_AUDIO
}RM_STRM_TYPE_T;

typedef struct _RM_FILE_HEADER_T 
{
    UINT32      ui4_object_id;           /* ".RMF" */
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT32      ui4_file_version;
    UINT32      ui4_num_headers;
}RM_FILE_HEADER_T;

typedef struct _RM_PROP_HEADER_T
{
    UINT32      ui4_object_id;           /* "PROP" */
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT32      ui4_max_bit_rate;
    UINT32      ui4_avg_bit_rate;
    UINT32      ui4_max_packet_size;
    UINT32      ui4_avg_packet_size;
    UINT32      ui4_num_packets;
    UINT32      ui4_duration;
    UINT32      ui4_preroll;
    UINT32      ui4_index_offset;
    UINT32      ui4_data_offset;
    UINT16      ui2_num_streams;
    UINT16      ui2_flags;
}RM_PROP_HEADER_T;

#ifdef RM_LOGICAL_STREAM_SUPPORT
typedef struct _RM_NAME_VAL_PROPERTY_T
{
    UINT32      ui4_size;
    UINT16      ui2_version;
    UINT8       ui1_name_len;
    UINT8*      pui1_name;
    INT32       i4_type;
    UINT16      ui2_val_len;
    UINT8*      pui1_val_data;
}RM_NAME_VAL_PROPERTY_T;

typedef struct _MINFO_RM_LOGIC_STM_STRUC_T
{
    UINT32      ui4_size;
    UINT16      ui2_version;
    UINT16      ui2_num_strms;
    UINT16*     pui2_strm_nums;
    UINT32*     pui4_data_offs;
    UINT16      ui2_num_rules;
    UINT16*     pui2_rule_strm_num_map;
    UINT16      ui2_num_properties;
    RM_NAME_VAL_PROPERTY_T*     pt_name_val_properties;
}MINFO_RM_LOGIC_STM_STRUC_T;
#endif

typedef struct _RM_MEDIA_PROPERTIY_T
{
    UINT32      ui4_object_id;       /* "MDPR" */
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT16      ui2_stream_number;
    UINT32      ui4_max_bit_rate;
    UINT32      ui4_avg_bit_rate;
    UINT32      ui4_max_packet_size;
    UINT32      ui4_avg_packet_size;
    UINT32      ui4_start_time;
    UINT32      ui4_preroll;
    UINT32      ui4_duration;
    UINT8       ui1_stream_name_size;
    UINT8*      pui1_stream_name; /* UINT8[stream_name_size] */
    UINT8       ui1_mime_type_size;
    UINT8*      pui1_mime_type; /* UINT8[mime_type_size] */

    UINT32      ui4_type_specific_len;
    UINT8*      pui1_type_specific_data; /* UINT8[type_specific_len] */
    RM_STRM_TYPE_T e_rm_strm_type;
    union
    {
        MINFO_RM_VIDEO_STM_ATRBT_T    r_rm_video_param;
        MINFO_RM_AUDIO_STM_ATRBT_T    r_rm_audio_v5_param;
#ifdef RM_LOGICAL_STREAM_SUPPORT
        MINFO_RM_LOGIC_STM_STRUC_T    r_rm_logic_strm;
#endif
    }u;
}RM_MEDIA_PROPERTIY_T;

typedef struct _RM_MEDIA_CONT_DESCRIPTION_T
{
    UINT32      ui4_object_id; /* "CONT" */
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT16      ui2_title_len;
    UINT8*      pui1_title;/* UINT8[title_len] */
    UINT16      ui2_author_len;
    UINT8*      pui1_author;/* UINT8[author_len] */
    UINT16      ui2_copyright_len;
    UINT8*      pui1_copyright;/* UINT8[copyright_len] */
    UINT16      ui2_comment_len;
    UINT8*      pui1_comment;/* UINT8[comment_len] */
}RM_MEDIA_CONT_DESCRIPTION_T;

typedef struct _RM_DATA_CHUNK_HEADER_T
{
    UINT32      ui4_offset;
    UINT32      ui4_object_id; //"DATA"
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT32      ui4_num_packets;
    UINT32      ui4_next_data_header;
}RM_DATA_CHUNK_HEADER_T;

typedef struct _RM_DATA_PACKET_HEADER_T
{
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT16      ui2_length;
    UINT16      ui2_stream_number;
    UINT32      ui4_timestamp;
    UINT8       ui1_reserved;
    UINT8       ui1_flags;
    /*
    HeaderType:  2bit
    u4NumPackets:7bit
    u4PacketNum: 7bit

    fgBrokenUpByUs: 1bit
    14/30 flag:     1bit
    u4FrameSize:   14bit
    if(flag)
    {
    unknown:        1bit
    14/30 flag:     1bit
    u4PartialFrameOffset: 14bit

    u1SeqNum:        8bit
    }
    */
    UINT8*      pui1_packet_data;/* UINT8[length] == NULL */
}RM_DATA_PACKET_HEADER_T;

typedef struct _RM_DATA_BLOCK_T
{
    RM_DATA_CHUNK_HEADER_T      t_data_block_head;
    RM_DATA_PACKET_HEADER_T*     pt_data_block_records;
}RM_DATA_BLOCK_T;

typedef struct _RM_INDEX_CHUNK_HEADER_T
{
    UINT32      ui4_object_id;  /* "INDX" */
    UINT32      ui4_size;
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT32      ui4_num_indices;
    UINT16      ui2_stream_number;
    UINT32      ui4_next_index_header;
    UINT32      ui4_cur_index_header;
}RM_INDEX_CHUNK_HEADER_T;

typedef struct _RM_INDEX_RECORD_T
{
    UINT16      ui2_object_version;
    /* the following only exist when (object_version == 0) */
    UINT32      ui4_timestamp;
    UINT32      ui4_offset;
    UINT32      ui4_cur_packet_count;
}RM_INDEX_RECORD_T;

typedef struct _RM_INDEX_BLOCK_T
{
    RM_INDEX_CHUNK_HEADER_T     t_index_block_head;
    RM_INDEX_RECORD_T*          pt_index_block_records;
}RM_INDEX_BLOCK_T;

typedef struct _RM_STREAM_NUM_T
{
    UINT8      ui1_video_cnt;
    UINT8      ui1_audio_cnt;
    UINT8      ui1_idx_cnt;
    UINT8      ui1_data_cnt;
#ifdef RM_LOGICAL_STREAM_SUPPORT
    UINT8      ui1_logic_video_cnt;
    UINT8      ui1_logic_audio_cnt;
#endif
}RM_STREAM_NUM_T;

typedef struct _RM_FILE_LAYOUT_T
{
    RM_STREAM_NUM_T                 t_stream_num;
    RM_FILE_HEADER_T                t_file_head;                    /*'.RMF'*/
    RM_PROP_HEADER_T                t_file_property;                /*'PROP'*/
    RM_MEDIA_CONT_DESCRIPTION_T     t_file_cont_desp;               /*'CONT'*/
    RM_MEDIA_PROPERTIY_T            r_strm_property[RM_STREAM_MAX]; /*'MDPR'*/
    RM_DATA_BLOCK_T                 t_file_data[RM_STREAM_MAX];     /*'DATA'*/
    RM_INDEX_BLOCK_T                r_strm_indx_tbl[RM_STREAM_MAX]; /*'INDX'*/
}RM_FILE_LAYOUT_T;

typedef struct _MINFO_RM_FILE_INF_T
{
    /* Current parser offset, file base */
    UINT64 ui8_curr_parser_offset;
    UINT8* pui1_buffer;
    UINT32 ui4_buf_len;
    /* generic infomation */
    MINFO_TYPE_GENERIC_T   t_generic_info;

    /* RM file content */
    RM_FILE_LAYOUT_T        r_file_layout;

    /* The parser state, it is only useful in build riff tree */
    MINFO_TYPE_META_DATA_T t_meta_data_info;
}MINFO_RM_FILE_INF_T;

typedef struct _MINFO_RM_SKIP_INF_T
{
    UINT32      ui4_object_id;
    UINT32      ui4_size;
}MINFO_RM_SKIP_INF_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 rm_file_minfo_box_parse(MINFO_OBJ_T*    pt_minfo_obj);

#endif /* _MP4_PARSER_H_ */

