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
 * $RCSfile: mkv_parser.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the mkv parser function
 *---------------------------------------------------------------------------*/

#ifndef _MKV_PARSER_H_
#define _MKV_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "inc/u_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "../../mm_common/rfc_ebml/rfc_ebml.h"

#else

#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../../mfmtrecg/u_mfmtrecg.h"
#include "../../mm_common/rfc_ebml/rfc_ebml.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/
#define MAX_NS_MKV_AUD 8
#define MAX_NS_MKV_VID 1
#define MAX_NS_MKV_SP  8

#define MKV_HEADER_BUF_LENGTH               20480
#define MKV_ELEMENT_MAX_HEAD_LEN            12
#define MKV_SUBTITLE_TRACK_TYPE             0x11

#define MKV_WAVE_FORMAT_PCM                 0x0001
#define MKV_WAVE_FORMAT_ADPCM               0x0002
#define MKV_WAVE_FORMAT_DTS_MS              0x0008
#define MKV_WAVE_FORMAT_DTS                 0x2001
#define MKV_WAVE_FORMAT_MPEG                0x0050
#define MKV_WAVE_FORMAT_MPEGLAYER3          0x0055
#define MKV_WAVE_FORMAT_DOLBY_AC3_SPDIF     0x0092
#define MKV_WAVE_FORMAT_WMA1                0x0160
#define MKV_WAVE_FORMAT_WMA2                0x0161
#define MKV_WAVE_FORMAT_WMAS                0x000a
#define MKV_WAVE_FORMAT_WMAP                0x0162 
#define MKV_WAVE_FORMAT_WMAL                0x0163
#define MKV_WAVE_FORMAT_DIVIO_AAC           0x4143
#define MKV_WAVE_FORMAT_AAC                 0x00FF
#define MKV_WAVE_FORMAT_FFMPEG_AAC          0x706D
#define MKV_WAVE_FORMAT_A52                 0x2000
#define IMPROVE_MKV_PARSER_SPEED 1

#define MKV_DIVX_PLUS_SUPPORT               1

typedef enum _MKV_STREAM_TYPE_E
{
    MKV_STRM_UNKNOWN,
    MKV_STRM_VIDEO_TYPE,
    MKV_STRM_AUDIO_TYPE,
    MKV_STRM_SUBTILE_TYPE    
}MKV_STREAM_TYPE_E;

typedef enum _MKV_STRM_CODEC_TYPE_E
{
    MKV_UNKNOWN,
    MKV_V_UNKNOWN,
    MKV_V_MS_VFW_FOURCC,
    MKV_V_MPEG4_ISO_AVC,
    MKV_V_MPEG4_ISO_SP,
    MKV_V_MPEG4_ISO_ASP,
    MKV_V_MPEG4_MS_V3,
    MKV_V_MPEG1,
    MKV_V_MPEG2,
    MKV_V_VP8,
    MKV_A_UNKNOWN,
    MKV_A_MPEG_L3,
    MKV_A_MPEG_L2,
    MKV_A_MPEG_L1,
    MKV_A_PCM_LE,
    MKV_A_PCM_BE,
    MKV_A_AC3, 
    MKV_A_DTS,
    MKV_A_MS_ACM,
    MKV_A_AAC,
    MKV_A_VORBIS,
    MKV_S_UNKNOWN,
    MKV_S_TEXT_UTF8,
    MKV_S_TEXT_SSA,
    MKV_S_TEXT_USF,
    MKV_S_VOBSUB,
    MKV_S_IMG_BMP,
    MKV_S_KATE
}MKV_STRM_CODEC_TYPE_E;

typedef enum _MKV_STRING_TYPE_E
{
    MKV_STRING_CHAR,
    MKV_STRING_W_CHAR
}MKV_STRING_TYPE_E;

typedef enum _MKV_ELEMENT_TYPE_E
{
    MKV_ELEMENT_UNKNOWN,
    MKV_ELEMENT_TOP_EBML_HEADER,
    MKV_ELEMENT_1ST_DOCTYPE,
    MKV_ELEMENT_TOP_SEGMENT,
    MKV_ELEMENT_1ST_SEGMENT_INFO,
    MKV_ELEMENT_2ND_SEG_UID,
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    MKV_ELEMENT_2ND_PREV_SEG_UID,
    MKV_ELEMENT_2ND_NEXT_SEG_UID,
#endif
    MKV_ELEMENT_2ND_TITLE,
    MKV_ELEMENT_2ND_DURATION,
    MKV_ELEMENT_2ND_DATE,
    MKV_ELEMENT_2ND_TIMECODESCALE,
    MKV_ELEMENT_1ST_SEEKHEAD,
#if IMPROVE_MKV_PARSER_SPEED
    MKV_ELEMENT_2ND_SEEK_ENTRY,
    MKV_ELEMENT_3RD_SEEK_ID,
    MKV_ELEMENT_3RD_SEEK_POSITION,
#endif
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    MKV_ELEMENT_1ST_EDIT,
    MKV_ELEMENT_2ND_EDIT_UID,
    MKV_ELEMENT_2ND_EDIT_HIDDEN,
    MKV_ELEMENT_2ND_EDIT_DEFAULT,
    MKV_ELEMENT_2ND_EDIT_ORDERED,
    MKV_ELEMENT_2ND_EDIT_CHAP_ATOM,
    MKV_ELEMENT_3RD_EDIT_CHAP_UID,
    MKV_ELEMENT_3RD_EDIT_CHAP_TIME_START,
    MKV_ELEMENT_3RD_EDIT_CHAP_TIME_END,
    MKV_ELEMENT_3RD_EDIT_CHAP_HIDDEN,
    MKV_ELEMENT_3RD_EDIT_CHAP_ENABLED,
    MKV_ELEMENT_3RD_EDIT_CHAP_SEG_UID,
    MKV_ELEMENT_3RD_EDIT_CHAP_SEG_EDIT_UID,
    MKV_ELEMENT_3RD_EDIT_CHAP_TRACKS,
    MKV_ELEMENT_4TH_EDIT_CHAP_TRACK_NUM,
    MKV_ELEMENT_3RD_EDIT_CHAP_DISP,
    MKV_ELEMENT_4TH_EDIT_CHAP_STR,
    MKV_ELEMENT_4TH_EDIT_CHAP_LANG,
    MKV_ELEMENT_4TH_EDIT_CHAP_COUNTRY,
#endif
    MKV_ELEMENT_1ST_CLUSTERS,
    MKV_ELEMENT_1ST_TRACK,
    MKV_ELEMENT_2ND_TRACK_DATA,
    MKV_ELEMENT_2ND_TRACK_ENTRY,
    MKV_ELEMENT_3ND_TRACK_PAYLOAD_DATA,
    MKV_ELEMENT_3RD_TRACK_NUMBER,
    MKV_ELEMENT_3RD_TRACK_UID,
    MKV_ELEMENT_3ND_TRACK_ENH_UID,
    MKV_ELEMENT_3ND_TRACK_MASTER_UID,
    MKV_ELEMENT_3ND_TRACK_ENH_SEG_UID,
    MKV_ELEMENT_3ND_TRACK_MASTER_SEG_UID,
    MKV_ELEMENT_3ND_TRACK_ENH_FLG,
    MKV_ELEMENT_3RD_TRACK_TYPE,
    MKV_ELEMENT_3RD_FLAG_DEFAULT,
    MKV_ELEMENT_3RD_DFT_DURATION,
    MKV_ELEMENT_3RD_TRACK_TIMECODE_SCALE,
    MKV_ELEMENT_3RD_LANGUAGE,
    MKV_ELEMENT_3RD_CODEC_ID,
    MKV_ELEMENT_3RD_CODEC_PRIVATE,
    MKV_ELEMENT_3RD_CODEC_NAME,
    MKV_ELEMENT_3RD_VIDEO_TRACK,
    MKV_ELEMENT_4TH_VID_PIXEL_W,
    MKV_ELEMENT_4TH_VID_PIXEL_H,
#if 0
    MKV_ELEMENT_4TH_VID_PIXEL_CROP_T,
    MKV_ELEMENT_4TH_VID_PIXEL_CROP_B,
    MKV_ELEMENT_4TH_VID_PIXEL_CROP_L,
    MKV_ELEMENT_4TH_VID_PIXEL_CROP_R,
#endif
    MKV_ELEMENT_4TH_VID_DISP_W,
    MKV_ELEMENT_4TH_VID_DISP_H,
    MKV_ELEMENT_3RD_AUDIO_TRACK,
    MKV_ELEMENT_4TH_AUD_SAMP_FREQ,
#if 0
    MKV_ELEMENT_4TH_AUD_OUTPUT_SAMP_FREQ,
#endif
    MKV_ELEMENT_4TH_AUD_CHANNELS,
    MKV_ELEMENT_4TH_AUD_BIT_DEPTH,
    MKV_ELEMENT_3RD_CONTENT_ENCODINGS,
    MKV_ELEMENT_4RD_CONTENT_ENCODING,
    MKV_ELEMENT_5RD_CONTENT_ENCODING_ORDER,
    MKV_ELEMENT_5RD_CONTENT_ENCODING_SCOPE,
    MKV_ELEMENT_5RD_CONTENT_ENCODING_TYPE,
    MKV_ELEMENT_5RD_CONTENT_COMPRESSION,
    MKV_ELEMENT_6RD_CONTENT_COMPALGO,
    MKV_ELEMENT_6RD_CONTENT_COMP_SETTINGS,
    MKV_ELEMENT_1ST_CUES,
    /*MKV_ELEMENT_2ND_CUE_POINT,*/
    MKV_ELEMENT_1ST_ATTACHMENT,
    MKV_ELEMENT_ATTACHMENT_FILE, /*add for font info*/
    MKV_ELEMENT_ATTACH_FILE_NAME,
    MKV_ELEMENT_ATTACH_FILE_MIME_TYPE,
    MKV_ELEMENT_ATTACH_FILE_DATA,
    MKV_ELEMENT_ATTACH_FILE_UID,
    MKV_ELEMENT_ATTACH_LINK_UID,
    /*MKV_ELEMENT_2ND_ATTACH_FILE,*/
    MKV_ELEMENT_1ST_CHAPTERS,
    MKV_ELEMENT_1ST_TAGS,
    MKV_ELEMENT_2ND_TAG_ENTRY,
#if 0
    MKV_ELEMENT_3RD_TARGET,
    MKV_ELEMENT_4TH_TARGET_TRACK_UID,
#endif
    MKV_ELEMENT_3RD_SIMPLE_TAG,
    MKV_ELEMENT_4TH_TAG_NAME,
    MKV_ELEMENT_4TH_TAG_STRING,
    MKV_ELEMENT_MAX_TYPE
}MKV_ELEMENT_TYPE_E;

#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
typedef struct _MINFO_MKV_DISPLAY
{
    CHAR*    ps_string;
    CHAR*    ps_language;
    CHAR*    ps_country;
    struct _MINFO_MKV_DISPLAY* pt_next;
}MINFO_MKV_DISPLAY;
typedef struct _MINFO_MKV_CHAP_TRACK
{
    UINT32   ui4_number;
    struct _MINFO_MKV_CHAP_TRACK* pt_next;
}MINFO_MKV_CHAP_TRACK;
typedef struct _MINFO_MKV_CHAP_ATOM_T
{
    UINT64   ui8_uid;
    UINT64   ui8_start_time;
    UINT64   ui8_end_time;
    BOOL     fg_hidden;
    BOOL     fg_enabled;
    UINT64   ui8_chap_seg_uid_h;
    UINT64   ui8_chap_seg_uid_l;
    UINT32   ui4_edition_uid;
    UINT32   ui4_display_num;
    UINT32   ui4_track_num;
    MINFO_MKV_CHAP_TRACK*  pt_track;
    MINFO_MKV_CHAP_TRACK*  pt_cur_track;
    MINFO_MKV_DISPLAY*     pt_display;
    MINFO_MKV_DISPLAY*     pt_cur_display;
    struct _MINFO_MKV_CHAP_ATOM_T*  pt_next;
}MINFO_MKV_CHAP_ATOM_T;
typedef struct _MINFO_MKV_EDITION_T
{
    UINT64   ui8_uid;
    UINT32   ui4_chap_num;
    BOOL     fg_default;
    BOOL     fg_ordered;
    BOOL     fg_hidden;
    MINFO_MKV_CHAP_ATOM_T*  pt_chap_atom;
    MINFO_MKV_CHAP_ATOM_T*  pt_curr_chap;
    struct _MINFO_MKV_EDITION_T*  pt_next;
}MINFO_MKV_EDITION_T;
typedef struct _MINFO_MKV_CHAPTERS_T
{
    UINT32   ui4_edition_num;
    MINFO_MKV_EDITION_T*  pt_edition;
    MINFO_MKV_EDITION_T*  pt_curr_parse_entry;
}MINFO_MKV_CHAPTERS_T;
#endif

/*Segment info structure*/
typedef struct _MINFO_MKV_SEGMENT_INFO_T
{
    UTF16_T* pws_segment_file_name;
    UINT64   ui8_seg_uid_high;
    UINT64   ui8_seg_uid_low;
    UINT64   ui8_timecode_scale;
    UINT64   ui8_duration;
    UINT64   ui8_org_duration;
    INT64    i8_date;
    UTF16_T* pws_title;
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    UINT64   ui8_prev_seg_uid_h;
    UINT64   ui8_prev_seg_uid_l;
    UINT64   ui8_next_seg_uid_h;
    UINT64   ui8_next_seg_uid_l;
#endif
}MINFO_MKV_SEGMENT_INFO_T;

/*SeekHead info structure*/
typedef struct _MINFO_MKV_SEEK_ENTRY_T
{
    UINT32 ui4_seek_id;
    UINT64 ui8_seek_position;
    struct _MINFO_MKV_SEEK_ENTRY_T* pt_next_seek_entry;
}MINFO_MKV_SEEK_ENTRY_T;

typedef struct _MINFO_MKV_SEEK_HEAD_INFO_T
{
    UINT8                   ui1_seek_num;
    UINT64                  ui8_seek_start_pos; /*seek head pos*/
    MINFO_MKV_SEEK_ENTRY_T* pt_curr_parse_entry;
    MINFO_MKV_SEEK_ENTRY_T* pt_seek_entry_head; 
}MINFO_MKV_SEEK_HEAD_INFO_T;

#if 0
/*Attachment info structure*/
typedef struct _MINFO_MKV_ATTACH_FILE_T
{
    CHAR* psz_file_description;
    CHAR* psz_file_name;
    CHAR* psz_file_mimetype;
    struct _MINFO_MKV_ATTACH_FILE_T* pt_next_attach_file;
    /*void* pv_file_data;*/
}MINFO_MKV_ATTACH_FILE_T;

typedef struct _MINFO_MKV_ALL_ATTACH_INFO_T
{
    UINT8 ui1_attach_num;
    MINFO_MKV_ATTACH_FILE_T* pt_attach_file_head;
}MINFO_MKV_ALL_ATTACH_INFO_T;
#endif

/*float timecode scale struct*/
typedef struct _MKV_TIMECODE_SCALE_FLOAT_T
{
    UINT32 ui4_float_numerator;
    UINT32 ui4_float_denominator;
}MKV_TIMECODE_SCALE_FLOAT_T;

/*master track info structure*/
typedef struct _MKV_STREAM_MT_INFO_T
{
    UINT64 ui8_enh_track_uid;
    UINT64 ui8_enh_seg_uid_high;
    UINT64 ui8_enh_seg_uid_low;
}MKV_STREAM_MT_INFO_T;

/*trick play track info structure*/
typedef struct _MKV_STREAM_TPT_INFO_T
{
    UINT64 ui8_master_track_uid;
    UINT64 ui8_master_seg_uid_high;
    UINT64 ui8_master_seg_uid_low;
}MKV_STREAM_TPT_INFO_T;

/*Stream gerneric info structure*/
typedef struct _MKV_STREAM_PROPERTIES_T
{
    UINT8  ui1_track_num;
    UINT64 ui8_enabled;
    UINT64 ui8_default;
    UINT64 ui8_forced;
    UINT64 ui8_lacing;
    UINT64 ui8_track_uid;
    UINT64 ui8_track_type;
    /*add for font info*/
    UINT64 ui8_attach_link_id;
    UINT64 ui8_def_duration;
    union{
        MKV_STREAM_MT_INFO_T  t_mt_info;
        MKV_STREAM_TPT_INFO_T t_tpt_info;
        } u;
    BOOL   fg_enh_track_flag;
    BOOL   fg_divx_plus;
    MKV_TIMECODE_SCALE_FLOAT_T  t_track_tm_scale;
    CHAR   ac_language[3];
    CHAR*  psz_codec_id;
    /*add for codec private*/
    BOOL    b_has_codec_private;
    UINT64  ui8_codec_private_len;
    UINT8*  pui1_codec_private;
    void*  pv_cur_parse_strm;
    UINT8  ui1_strm_type;
    UINT32 ui4_content_encodings_size;
    UINT8* pui1_content_encodings;
    
    /* add for content encodings */
    BOOL                            b_has_decode_setting;
    UINT32                          ui4_encoding_num;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_curr_content_encoding;
}MKV_STREAM_PROPERTIES_T;

typedef struct _MINFO_MKV_VIDEO_STRM_INFO_T
{
    /*stream properties*/
    UINT8                                 ui1_stream_idx;
    UINT32                                ui4_stream_number;
    UINT64                                ui8_track_uid;
    MINFO_MKV_VIDEO_STM_ATRBT_T           t_video_info;    
    union{
        MKV_STREAM_MT_INFO_T  t_mt_info;
        MKV_STREAM_TPT_INFO_T t_tpt_info;
        } u;
    BOOL   fg_enh_track_flag;
    struct _MINFO_MKV_VIDEO_STRM_INFO_T*  pt_next_video_strm;
}MINFO_MKV_VIDEO_STRM_INFO_T;

typedef struct _MINFO_MKV_AUDIO_STRM_INFO_T
{
    /*stream properties*/
    UINT8                                 ui1_stream_idx;
    UINT32                                ui4_stream_number;
    UINT64                                ui8_track_uid;
    MINFO_MKV_AUDIO_STM_ATRBT_T           t_audio_info;    
    union{
        MKV_STREAM_MT_INFO_T  t_mt_info;
        MKV_STREAM_TPT_INFO_T t_tpt_info;
        } u;
    BOOL   fg_enh_track_flag;
    struct _MINFO_MKV_AUDIO_STRM_INFO_T*  pt_next_audio_strm;
}MINFO_MKV_AUDIO_STRM_INFO_T;

typedef struct _MINFO_MKV_SUBTITLE_STRM_INFO_T
{
    /*stream properties*/
    UINT8                                    ui1_stream_idx;
    UINT32                                   ui4_stream_number;
    UINT64                                   ui8_track_uid;    
    MINFO_MKV_SUBTITLE_STM_ATRBT_T           t_subtitle_info;    
    struct _MINFO_MKV_SUBTITLE_STRM_INFO_T*  pt_next_subtitle_strm;
}MINFO_MKV_SUBTITLE_STRM_INFO_T;

typedef struct _MINFO_MKV_ALL_STRM_INFO_T 
{    
    MKV_STREAM_PROPERTIES_T         t_strm_properties;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_video_strm_info_head;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_audio_strm_info_head;
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_subtitle_strm_info_head;
    UINT8                           ui1_audio_strm_num;
    UINT8                           ui1_video_strm_num;
    UINT8                           ui1_subtitle_strm_num;
}MINFO_MKV_ALL_STRM_INFO_T;

typedef struct _MINFO_META_INFO_T
{
    UTF16_T* pws_director;
    UTF16_T* pws_copyright;
    UTF16_T* pws_genre;
    UTF16_T* pws_rating;
}MINFO_META_INFO_T;

typedef struct _MINFO_TAG_INFO_T
{
    CHAR*    ps_tagname;
    UTF16_T* pws_tagstring;
}MINFO_TAG_INFO_T;

typedef struct _MINFO_MKV_INF_T
{
    /* File length base on frame */
    UINT64 ui8_duration;
    /*file size*/
    UINT64 ui8_size;
    /*segment info*/
    MINFO_MKV_SEGMENT_INFO_T  t_segment_info;
    /*seek head info*/
    MINFO_MKV_SEEK_HEAD_INFO_T t_all_seekhead_info;

    /*attachment info*/
    /*MINFO_MKV_ALL_ATTACH_INFO_T t_all_attach;*/

    /*segment data start position, file base*/
    UINT64 ui8_seg_start_pos;
    /*cues position, file base*/
    UINT64 ui8_cues_position;
    /*cluster position, file base*/
    UINT64 ui8_cluster_position;

    UINT8 ui1_cur_strm_num;

    BOOL fg_trick_play_seg;

    /* Stream information */
    MINFO_MKV_ALL_STRM_INFO_T t_all_strm_info;

#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    MINFO_MKV_CHAPTERS_T  t_chapters_info;
#endif    

    /*index info , provided for index builder*/
    MINFO_TYPE_IDX_INFO_T t_idx_info;

    /*meta data info*/
    MINFO_META_INFO_T t_meta_info;
    
    /* File length base on PTS */
    /*UINT32 ui4_total_pts;*/   

    /*tag info*/
    MINFO_TAG_INFO_T t_simple_tag;
    
    /*current parsed element*/
    EBML_ELE_BASIC_T t_curr_element; 

    /*store current parsed chunk header*/
    UINT8* pui1_header_buf;
    /*header buffer size*/
    UINT32 ui4_buf_length;
    /*store current parsed pos*/
    UINT64 ui8_cur_parser_offset;

    /* store drm information */
    MINFO_TYPE_DRM_INFO_T           t_minfo_drm_info;

    UINT32                          ui4_drm_size;
    struct _MINFO_MKV_INF_T* pt_next;
}MINFO_MKV_INF_T;

/*object operation callback*/
typedef INT32(*mkv_parse_element_func)( MINFO_OBJ_T* pt_minfo_obj, 
                                        MINFO_MKV_INF_T* pt_mkv_info_obj);

typedef struct _MINFO_MKV_ELEMENT_OPER_T
{
    MKV_ELEMENT_TYPE_E     e_element_type;
    UINT32                 ui4_element_id;
    CHAR*                  psz_description;
    mkv_parse_element_func pf_parse_element;
} MINFO_MKV_ELEMENT_OPER_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern UINT16 MKV_GETWLE(UINT8* pui1_buf, SIZE_T sz_buf_len);
extern UINT32 MKV_GETDWLE(UINT8* pui1_buf, SIZE_T sz_buf_len);

extern INT32 _mkv_get_strm_atrbt_by_idx(MINFO_MKV_ALL_STRM_INFO_T* pt_all_strm_info,
                                        MKV_STREAM_TYPE_E          e_strm_type,
                                        UINT16                     ui2_count,
                                        MINFO_INFO_T*              pt_minfo_rec);

/*add for font info*/
extern INT32 _mkv_get_font_data(MINFO_OBJ_T* pt_minfo_obj,
                             MINFO_MKV_ALL_STRM_INFO_T* pt_all_strm_info,
                             UINT16                     ui2_count,
                             MINFO_INFO_T*              pt_minfo_rec);
/*mkv parse function*/
extern INT32 _mkv_parse_elements(MINFO_OBJ_T* pt_minfo_obj);

/*mkv common function*/
extern UINT64 _mkv_get_element_size(EBML_ELE_BASIC_T* pt_ebml_element, MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T*  pt_mkv_info_obj, BOOL b_file);
extern MKV_ELEMENT_TYPE_E _mkv_get_element_type(EBML_ELE_BASIC_T* pt_ebml_element);
extern CHAR* _mkv_get_element_desc(EBML_ELE_BASIC_T* pt_ebml_element);
extern mkv_parse_element_func _mkv_get_element_parse_func(EBML_ELE_BASIC_T* pt_ebml_element);
extern VOID* _mkv_parse_string(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, MKV_STRING_TYPE_E e_string_type);
extern INT32 _mkv_parse_uint(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, UINT64* pui8_value);
extern INT32 _mkv_parse_luint(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, UINT64* pui8_value1, UINT64* pui8_value2);
extern INT32 _mkv_parse_int(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, INT64* pi8_value);
extern INT32 _mkv_parse_float(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, FLOAT_FRACTION_T* pt_value);
extern INT32 _mkv_parse_element_head(MINFO_OBJ_T* pt_minfo_obj, 
                                     EBML_ELE_BASIC_T* pt_element,
                                     MINFO_MKV_INF_T*  pt_mkv_info_obj,
                                     BOOL b_need_load);
extern INT32 _mkv_skip_data(MINFO_OBJ_T* pt_minfo_obj, UINT64 ui8_broken_data_size, MINFO_MKV_INF_T*  pt_mkv_info_obj, BOOL b_file);
extern INT32 _mkv_jump_element_data(MINFO_OBJ_T* pt_minfo_obj, 
                                    EBML_ELE_BASIC_T* pt_element,
                                    MINFO_MKV_INF_T*  pt_mkv_info_obj,
                                    BOOL b_file);
extern INT32 _mkv_load_element_data(MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T*  pt_mkv_info_obj, UINT64 ui8_data_size);
extern INT32 _mkv_feeder_parse( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_only_parse_file( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_only_parse_mem( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_add_sp_node(MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);

/*element parse function*/
extern INT32 _mkv_parse_ebml_head( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_doctype( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_segment( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_segment_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seg_title( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seg_duration( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seg_date( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seg_timecode_scale( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seek_head( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#if IMPROVE_MKV_PARSER_SPEED
extern INT32 _mkv_parse_seek_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seek_id( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_seek_position( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#endif
extern INT32 _mkv_parse_track( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_payload_data( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_data( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_number( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_enh_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_master_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_enh_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_master_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_enh_flg( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
/*add for font info*/
extern INT32 _mkv_parse_attach_link_ele_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_type( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_flag_default( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_default_duration( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_timecode_scale( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_language( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_codec_id( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_codec_private( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_track_codec_name( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_video_track( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_pixel_w( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_pixel_h( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#if 0
extern INT32 _mkv_parse_vid_pixel_crop_t( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_pixel_crop_b( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_pixel_crop_l( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_pixel_crop_r( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#endif
extern INT32 _mkv_parse_vid_disp_w( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_vid_disp_h( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_audio_track(MINFO_OBJ_T * pt_minfo_obj, MINFO_MKV_INF_T * pt_mkv_info_obj);
extern INT32 _mkv_parse_aud_samp_freq( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#if 0
extern INT32 _mkv_parse_aud_output_samp_freq( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#endif
extern INT32 _mkv_parse_aud_channels( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_aud_bit_depth( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_encodings( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_encoding( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_encoding_order( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_encoding_scope( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_encoding_type( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_compression( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_compalgo( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_content_comp_settings( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_clusters_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_cues_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_attachment_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
/*add for font info*/
extern INT32 _mkv_parse_attach_file_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_attach_file_ele_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chapters_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_tags( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_tag_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#if 0
extern INT32 _mkv_parse_target_track_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#endif
extern INT32 _mkv_parse_simple_tag( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_tag_name( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_tag_string( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);

extern INT32 _mkv_set_audio_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_AUDIO_STRM_INFO_T* pt_aud_strm);
extern INT32 _mkv_set_video_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_VIDEO_STRM_INFO_T* pt_vid_strm);
extern INT32 _mkv_set_sp_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_sp_strm);
extern VOID* _mkv_search_track_node(MINFO_MKV_INF_T* pt_mkv_info_obj, MKV_STREAM_TYPE_E e_strm_type);
extern UINT8 _mkv_get_codec_type(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT8 ui1_strm_type);
extern UINT8  _get_aud_codec(UINT16 ui2_format, MINFO_MKV_AUDIO_STRM_INFO_T* pt_aud_strm);
extern UINT8 _get_vid_codec(UINT32 ui4_4cc);
extern INT32 _mkv_set_video_codec(MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_set_audio_codec(MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_set_sp_codec(MINFO_MKV_INF_T* pt_mkv_info_obj);
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
extern INT32 _mkv_parse_prev_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_next_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_edit_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_edit_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_edit_hidden( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_edit_default( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_edit_ordered( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_atom( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_time_start( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_time_end( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_hidden( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_enabled( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_seg_edit_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_tracks( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_track_num( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_disp( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_string( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_lang( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
extern INT32 _mkv_parse_chap_country( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj);
#endif
#endif /* _MKV_PARSER_H_ */

