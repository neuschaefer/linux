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
 * mp4_parser.h,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the mp4 parser function
 *---------------------------------------------------------------------------*/

#ifndef _MP4_PARSER_H_
#define _MP4_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/minfo/mp4_info/mp4_common.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../../mfmtrecg/u_mfmtrecg.h"
#include "../../minfo/mp4_info/mp4_common.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/

#define MP4_BOX_BUF_LENGTH     1024

typedef struct _MP4_BOX_MVHD_T
{
    UINT8   ui1_version;
    UINT32 ui4_flags;

    UINT64 ui8_creation_time;
    UINT64 ui8_modification_time;
    UINT32 ui4_timescale;
    UINT64 ui8_duration;

    INT32   i4_rate;
    INT16   i2_volume;
    
    INT16   i2_reserved1;
    UINT32 ui4_reserved2[2];
    INT32   i4_matrix[9];
    UINT32 ui4_pre_defined[6];
    UINT32 ui4_next_track_id;
} MP4_BOX_MVHD_T;

typedef struct _MP4_BOX_TKHD_T
{
    UINT8   ui1_version;
    UINT32 ui4_flags;

    UINT64 ui8_creation_time;
    UINT64 ui8_modification_time;
    UINT32 ui4_track_id;
    UINT32 ui4_reserved1;
    UINT64 ui8_duration;

    UINT32 ui4_reserved2[2];
    INT16   i2_layer;
    INT16   i2_alternate_group;
    INT16   i2_volume;
    UINT16 ui2_reserved3;
    INT32   i4_matrix[9];
    UINT32 ui4_width;
    UINT32 ui4_height;
} MP4_BOX_TKHD_T;

typedef struct _MP4_BOX_MDHD_T
{
    UINT8   ui1_version;
    UINT32 ui4_flags;

    UINT64 ui8_creation_time;
    UINT64 ui8_modification_time;
    UINT32 ui4_timescale;
    UINT64 ui8_duration;

    INT8     i1_pad;                /* bit(1) */
    UINT8   ui1_language[3]; /* int(5) [3] */
    UINT16 ui2_pre_defined;
} MP4_BOX_MDHD_T;

typedef struct _MP4_BOX_HDLR_T
{
    UINT8   ui1_version;
    UINT32 ui4_flags;

    UINT32 ui4_pre_defined;
    UINT32 ui4_handler_type; /* 'vide' 'soud' 'hint' ... */

    UINT32 ui4_reserved[3];
    UCHAR* puc_name;
} MP4_BOX_HDLR_T;


#define MAX_NS_MP4_AUD  3
#define MAX_NS_MP4_VID  1
#define MAX_NS_MP4_SBTL 4
#define MAX_MARLIN_TRACK_NUM        7/* marlin max number of track : 1(video) + 2(audio) + 4(subtitle) */

typedef struct _MINFO_MP4_STRM_INF_T 
{
    MINFO_MP4_STM_ID_T t_video_strm_ID[MAX_NS_MP4_VID];
    MINFO_MP4_STM_ID_T t_audio_strm_ID[MAX_NS_MP4_AUD];
    MINFO_MP4_STM_ID_T t_sbtl_strm_ID[MAX_NS_MP4_SBTL];    
    MINFO_MP4_VIDEO_STM_ATRBT_T rVideStrmInf[MAX_NS_MP4_VID];
    MINFO_MP4_AUDIO_STM_ATRBT_T rAudStrmInf[MAX_NS_MP4_AUD];
    MINFO_MP4_SUBTITLE_STM_ATRBT_T rSbtlStrmInf[MAX_NS_MP4_SBTL];
    MINFO_MP4_MARLIN_TRACK_INFO_T       at_marlin_track_info[MAX_MARLIN_TRACK_NUM];/*VASSSS*/
    UINT8 ui1_audio_strm_num;
    UINT8 ui1_video_strm_num;
    UINT8 ui1_sbtl_strm_num;    
}MINFO_MP4_STRM_INF_T;


typedef struct _MINFO_MP4_MOOV_INF_T
{
    /* store current parsed box data */
    UINT8* pui1_box_buf;
    /* box buffer size */
    UINT32 ui4_buf_length;
    
    /* The pointer to the box list */
    MP4_BOX_T* pt_tree_box;
    UINT32 ui4_malloc_count;

    /* moov box offset */
    UINT64 ui8_moov_offset;
    /* moov box size */
    UINT32 ui4_moov_size;
    
    /* mdat box offset */
    UINT64 ui8_data_offset;
    /* mdat box size */
    UINT64 ui8_data_sz;

    /* movie time scale and duration */
    UINT32 ui4_movie_timescale;
    UINT64 ui8_movie_duration;
    
    /* Current parser offset, file base */
    UINT64 ui8_curr_parser_offset;

    /* generic infomation */
    MINFO_TYPE_GENERIC_T   t_generic_info;
    
    /* index table information */
    /*MINFO_TYPE_IDX_INFO_T  t_index_table_info;*/
    
    /* Stream information */
    MINFO_MP4_STRM_INF_T t_strm_Info;  
    UINT8 ui1_uCurStrmNum;
    
    /* The parser state, it is only useful in build riff tree */
    MP4_PARSER_STATE_ENUM_T eParserState;
    
    /* meta data info */
    MINFO_TYPE_META_DATA_T t_meta_data_info;

    /* to store cmov info if it is exist */
    MP4_BOX_CMOV_INFO_T t_mp4_box_cmov;
    PIFF_BOX_INET_INFO_T  t_piff_inet_info;
    MP4_MULTI_FILE_INFO_T   t_mp4_multi_file_info;
    BOOL b_fake_cmov;
}MINFO_MP4_MOOV_INF_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 mp4_minfo_box_parse (MINFO_OBJ_T*    pt_minfo_obj);

#endif /* _MP4_PARSER_H_ */

