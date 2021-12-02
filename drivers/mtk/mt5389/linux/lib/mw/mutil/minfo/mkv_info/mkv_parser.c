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
 * $RCSfile: mkv_parser.c,v $
 * $Revision: #2 $ 1.0
 * $Date: 2013/01/04 $ 2004/11/26
 * $Author: pan.huang $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the mkv parser function
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

#include "inc/u_common.h"
#include "inc/x_mm_common.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "unicode/x_uc_str.h"
#include "mkv_parser.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"
#include "drm/divx_drm/x_divx_drm.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"
#include "../minfo.h"
#include "x_minfo.h"
#include "x_uc_str.h"
#include "mkv_parser.h"
#include "u_dbg.h"
#include "../../mm_util.h"
#include "x_divx_drm.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MKV_WLE(a,b)      ((((UINT16)b)<<8)|(a))
#define MKV_DWLE(a,b,c,d) ((((UINT32)d)<<24)|((c)<<16)|((b)<<8)|(a))

#define LITTLE_TO_BIG_DWORD(dwValue)   (((dwValue&0x000000FF)<<24)|     \
                                        ((dwValue&0x0000FF00)<< 8)|     \
                                        ((dwValue&0x00FF0000)>> 8)|     \
                                        ((dwValue&0xFF000000)>>24))

#define SUPPORT_MKV_SUBTITLE 1
#define MAX_CONTENT_ENCODING_LEN 1025
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    data definition
 ----------------------------------------------------------------------------*/
#if IMPROVE_MKV_PARSER_SPEED
static BOOL _g_parsed_segmentinfo;
static BOOL _g_parsed_tracks;
static BOOL _g_parsed_chapters;
static BOOL _g_parsed_tags;
static BOOL _g_parsed_cues;
static BOOL _g_parsed_cluster;
#endif
static UINT64 _g_seg_size;

const MINFO_MKV_ELEMENT_OPER_T g_at_mkv_element_op[] =
{
    { 
        MKV_ELEMENT_UNKNOWN,   
        0,    
        "Unknown element",             
        NULL
    },
    {
        MKV_ELEMENT_TOP_EBML_HEADER,
        0x1A45DFA3,    
        "Top lever : Ebml header",
        _mkv_parse_ebml_head
    },
    {
        MKV_ELEMENT_1ST_DOCTYPE,
        0x00004282,
        "1st lever : Doc Type",
        _mkv_parse_doctype
    },
    {
        MKV_ELEMENT_TOP_SEGMENT,
        0x18538067,    
        "Top lever: Segment",
        _mkv_parse_segment
    },
    {
        MKV_ELEMENT_1ST_SEGMENT_INFO,
        0x1549A966,    
        "1st lever : Segment Info",
        _mkv_parse_segment_info
    },
    {
        MKV_ELEMENT_2ND_SEG_UID,
        0x000073A4,
        "2nd lever : Seg uid",
        _mkv_parse_seg_uid
    },
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    {
        MKV_ELEMENT_2ND_PREV_SEG_UID,
        0x003CB923,
        "2nd lever : Prev seg uid",
        _mkv_parse_prev_seg_uid
    },
    {
        MKV_ELEMENT_2ND_NEXT_SEG_UID,
        0x003EB923,
        "2nd lever : Next seg uid",
        _mkv_parse_next_seg_uid
    },
#endif
    {
        MKV_ELEMENT_2ND_TITLE,
        0x00007BA9,
        "2nd lever : Title",
        _mkv_parse_seg_title
    },
    {
        MKV_ELEMENT_2ND_DURATION,
        0x00004489,
        "2nd lever : Duration",
        _mkv_parse_seg_duration
    },
    {
        MKV_ELEMENT_2ND_DATE,
        0x00004461,
        "2nd lever : Date",
        _mkv_parse_seg_date
    },
    {
        MKV_ELEMENT_2ND_TIMECODESCALE,
        0x002AD7B1,
        "2nd lever : TimeCodeScale",
        _mkv_parse_seg_timecode_scale
    },
    {
        MKV_ELEMENT_1ST_SEEKHEAD,
        0x114D9B74,
        "1st lever : Seek Head",
        _mkv_parse_seek_head
    },
#if IMPROVE_MKV_PARSER_SPEED
    {
        MKV_ELEMENT_2ND_SEEK_ENTRY,
        0x00004DBB,
        "2nd lever : Seek Entry",
        _mkv_parse_seek_entry
    },
    {
        MKV_ELEMENT_3RD_SEEK_ID,
        0x000053AB,
        "3rd lever : Seek ID",
        _mkv_parse_seek_id
    },
    {
        MKV_ELEMENT_3RD_SEEK_POSITION,
        0x000053AC,
        "3rd lever : Seek Position",
        _mkv_parse_seek_position
    },
#endif
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    {
        MKV_ELEMENT_1ST_EDIT,
        0x000045B9,
        "1st lever : Edition entry",
        _mkv_parse_edit_entry
    },
    {
        MKV_ELEMENT_2ND_EDIT_UID,
        0x000045BC,
        "2nd lever : Edition uid",
        _mkv_parse_edit_uid
    },
    {
        MKV_ELEMENT_2ND_EDIT_HIDDEN,
        0x000045BD,
        "2nd lever : Flag hidden",
        _mkv_parse_edit_hidden
    },
    {
        MKV_ELEMENT_2ND_EDIT_DEFAULT,
        0x000045DB,
        "2nd lever : Flag default",
        _mkv_parse_edit_default
    },
    {
        MKV_ELEMENT_2ND_EDIT_ORDERED,
        0x000045DD,
        "2nd lever : Flag ordered",
        _mkv_parse_edit_ordered
    },
    {
        MKV_ELEMENT_2ND_EDIT_CHAP_ATOM,
        0x000000B6,
        "2nd lever : Chapter Atom",
        _mkv_parse_chap_atom
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_UID,
        0x000073C4,
        "3rd lever : Chapter UID",
        _mkv_parse_chap_uid
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_TIME_START,
        0x00000091,
        "3rd lever : Chapter time start",
        _mkv_parse_chap_time_start
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_TIME_END,
        0x00000092,
        "3rd lever : Chapter time end",
        _mkv_parse_chap_time_end
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_HIDDEN,
        0x00000098,
        "3rd lever : Chapter hidden",
        _mkv_parse_chap_hidden
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_ENABLED,
        0x00004598,
        "3rd lever : Chapter enabled",
        _mkv_parse_chap_enabled
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_SEG_UID,
        0x00006E67,
        "3rd lever : Chapter segment uid",
        _mkv_parse_chap_seg_uid
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_SEG_EDIT_UID,
        0x00006EBC,
        "3rd lever : Chapter segment edition uid",
        _mkv_parse_chap_seg_edit_uid
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_TRACKS,
        0x0000008F,
        "3rd lever : Chapter tracks",
        _mkv_parse_chap_tracks
    },
    {
        MKV_ELEMENT_4TH_EDIT_CHAP_TRACK_NUM,
        0x00000089,
        "4th lever : Chapter track number",
        _mkv_parse_chap_track_num
    },
    {
        MKV_ELEMENT_3RD_EDIT_CHAP_DISP,
        0x00000080,
        "3rd lever : Chapter display",
        _mkv_parse_chap_disp
    },
    {
        MKV_ELEMENT_4TH_EDIT_CHAP_STR,
        0x00000085,
        "4th lever : Chapter string",
        _mkv_parse_chap_string
    },
    {
        MKV_ELEMENT_4TH_EDIT_CHAP_LANG,
        0x0000437C,
        "4th lever : Chapter language",
        _mkv_parse_chap_lang
    },
    {
        MKV_ELEMENT_4TH_EDIT_CHAP_COUNTRY,
        0x0000437E,
        "4th lever : Chapter country",
        _mkv_parse_chap_country
    },
#endif
    {
        MKV_ELEMENT_1ST_CLUSTERS,
        0x1F43B675,    
        "1st lever : Clusters",
        _mkv_parse_clusters_info
    },
    {
        MKV_ELEMENT_1ST_TRACK,
        0x1654AE6B,
        "1st lever : Track",
        _mkv_parse_track
    },
    {
        MKV_ELEMENT_2ND_TRACK_DATA,
        0x000000DA,    
        "2nd lever : Track Data",
        _mkv_parse_track_data
    },
    {
        MKV_ELEMENT_2ND_TRACK_ENTRY,
        0x000000AE,    
        "2nd lever : Track Entry",
        _mkv_parse_track_entry
    },
    {
        MKV_ELEMENT_3ND_TRACK_PAYLOAD_DATA,
        0x000000DE,    
        "3nd lever : Track Payload Data",
        _mkv_parse_track_payload_data
    },
    {
        MKV_ELEMENT_3RD_TRACK_NUMBER,
        0x000000D7,
        "3rd lever : Track Number",
        _mkv_parse_track_number
    },
    {
        MKV_ELEMENT_3RD_TRACK_UID,
        0x000073C5,
        "3rd lever : Track UID",
        _mkv_parse_track_uid
    },    
    {
        MKV_ELEMENT_3ND_TRACK_ENH_UID,
        0x000000C0,    
        "3nd lever : Enhanced Track UID",
        _mkv_parse_track_enh_uid
    },   
    {
        MKV_ELEMENT_3ND_TRACK_MASTER_UID,
        0x000000C7,    
        "3nd lever : Master Track UID",
        _mkv_parse_track_master_uid
    },   
    {
        MKV_ELEMENT_3ND_TRACK_ENH_SEG_UID,
        0x000000C1,    
        "3nd lever : Enhanced Track Segment UID",
        _mkv_parse_track_enh_seg_uid
    },   
    {
        MKV_ELEMENT_3ND_TRACK_MASTER_SEG_UID,
        0x000000C4,    
        "3nd lever : Master Track Segment UID",
        _mkv_parse_track_master_seg_uid
    },   
    {
        MKV_ELEMENT_3ND_TRACK_ENH_FLG,
        0x000000C6,    
        "3nd lever : Enhanced Track Flag",
        _mkv_parse_track_enh_flg
    },   
    {
        MKV_ELEMENT_3RD_TRACK_TYPE,
        0x00000083,
        "3rd lever : Track Type",
        _mkv_parse_track_type
    },
    {
        MKV_ELEMENT_3RD_FLAG_DEFAULT,
        0x00000088,
        "3rd lever : Flag Default",
        _mkv_parse_track_flag_default
    },
    {
        MKV_ELEMENT_3RD_DFT_DURATION,
        0x0023E383,
        "3rd lever : Default Duration",
        _mkv_parse_track_default_duration
    },
    {
        MKV_ELEMENT_3RD_TRACK_TIMECODE_SCALE,
        0x0023314F,
        "3rd lever : Track TimecodeScale",
        _mkv_parse_track_timecode_scale
    },
    {
        MKV_ELEMENT_3RD_LANGUAGE,
        0x0022B59C,
        "3rd lever : Language",
        _mkv_parse_track_language
    },
    {
        MKV_ELEMENT_3RD_CODEC_ID,
        0x00000086,
        "3rd lever : Codec ID",
        _mkv_parse_track_codec_id
    },
    {
        MKV_ELEMENT_3RD_CODEC_PRIVATE,
        0x000063A2,
        "3rd lever : Codec Private",
        _mkv_parse_track_codec_private
    },
    {
        MKV_ELEMENT_3RD_CODEC_NAME,
        0x00258688,
        "3rd lever : Codec Name",
        _mkv_parse_track_codec_name
    },
    {
        MKV_ELEMENT_3RD_VIDEO_TRACK,
        0x000000E0,
        "3rd lever : Video",
        _mkv_parse_video_track
    },
    {
        MKV_ELEMENT_4TH_VID_PIXEL_W,
        0x000000B0,
        "4th lever : Video Pixel Width",
        _mkv_parse_vid_pixel_w
    },
    {
        MKV_ELEMENT_4TH_VID_PIXEL_H,
        0x000000BA,
        "4th lever : Video Pixel Height",
        _mkv_parse_vid_pixel_h
    },
#if 0
    {
        MKV_ELEMENT_4TH_VID_PIXEL_CROP_T,
        0x000054BB,
        "4th lever : Video Pixel Crop Top",
        _mkv_parse_vid_pixel_crop_t
    },
    {
        MKV_ELEMENT_4TH_VID_PIXEL_CROP_B,
        0x000054AA,
        "4th lever : Video Pixel Crop Bottom",
        _mkv_parse_vid_pixel_crop_b
    },
    {
        MKV_ELEMENT_4TH_VID_PIXEL_CROP_L,
        0x000054CC,
        "4th lever : Video Pixel Crop Left",
        _mkv_parse_vid_pixel_crop_l
    },
    {
        MKV_ELEMENT_4TH_VID_PIXEL_CROP_R,
        0x000054DD,
        "4th lever : Video Pixel Crop Right",
        _mkv_parse_vid_pixel_crop_r
    },
#endif
    {
        MKV_ELEMENT_4TH_VID_DISP_W,
        0x000054B0,
        "4th lever : Video Display Width",
        _mkv_parse_vid_disp_w
    },
    {
        MKV_ELEMENT_4TH_VID_DISP_H,
        0x000054BA,
        "4th lever : Video Display Height",
        _mkv_parse_vid_disp_h
    },
    {
        MKV_ELEMENT_3RD_AUDIO_TRACK,
        0x000000E1,
        "3rd lever : Audio",
        _mkv_parse_audio_track
    },
    {
        MKV_ELEMENT_4TH_AUD_SAMP_FREQ,
        0x000000B5,
        "4th lever : Audio Sample Freq",
        _mkv_parse_aud_samp_freq
    },
#if 0
    {
        MKV_ELEMENT_4TH_AUD_OUTPUT_SAMP_FREQ,
        0x000078B5,
        "4th lever : Audio Output Sample Freq",
        _mkv_parse_aud_output_samp_freq
    },
#endif
    {
        MKV_ELEMENT_4TH_AUD_CHANNELS,
        0x0000009F,
        "4th lever : Audio Channels",
        _mkv_parse_aud_channels
    },
    {
        MKV_ELEMENT_4TH_AUD_BIT_DEPTH,
        0x00006264,
        "4th lever : Audio BitDepth",
        _mkv_parse_aud_bit_depth
    },
    {
        MKV_ELEMENT_3RD_CONTENT_ENCODINGS,
        0x00006D80,
        "3rd lever : Content Encodings",
        _mkv_parse_content_encodings
    },
    {
        MKV_ELEMENT_4RD_CONTENT_ENCODING,
        0x00006240,
        "4rd lever : Content Encoding",
        _mkv_parse_content_encoding
    },
    {
        MKV_ELEMENT_5RD_CONTENT_ENCODING_ORDER,
        0x00005031,
        "5rd lever : Content Order",
        _mkv_parse_content_encoding_order
    },
    {
        MKV_ELEMENT_5RD_CONTENT_ENCODING_SCOPE,
        0x00005032,
        "5rd lever : Content Scope",
        _mkv_parse_content_encoding_scope
    },
    {
        MKV_ELEMENT_5RD_CONTENT_ENCODING_TYPE,
        0x00005033,
        "5rd lever : Content Type",
        _mkv_parse_content_encoding_type
    },
    {
        MKV_ELEMENT_5RD_CONTENT_COMPRESSION,
        0x00005034,
        "5rd lever : Content Compression",
        _mkv_parse_content_compression
    },
    {
        MKV_ELEMENT_6RD_CONTENT_COMPALGO,
        0x00004254,
        "6rd lever : Content CompAlgo",
        _mkv_parse_content_compalgo
    },
    {
        MKV_ELEMENT_6RD_CONTENT_COMP_SETTINGS,
        0x00004255,
        "6rd lever : Content CompSettings",
        _mkv_parse_content_comp_settings
    },
    {
        MKV_ELEMENT_1ST_CUES,
        0x1C53BB6B,    
        "1st lever : Cues",
        _mkv_parse_cues_info
    },
    {
        MKV_ELEMENT_1ST_ATTACHMENT,
        0x1941A469,
        "1st lever : Attachment",
        _mkv_parse_attachment_info
    },
    {  /*add for font info*/
        MKV_ELEMENT_ATTACHMENT_FILE,
        0x000061A7,
        "2nd lever : Attachment file",
        _mkv_parse_attach_file_info
    }, 
    {  /*add for font info*/
        MKV_ELEMENT_ATTACH_FILE_NAME,
        0x0000466E,
        "3rd lever : Attachment file name",
        _mkv_parse_attach_file_ele_info
    }, 
    {  /*add for font info*/
        MKV_ELEMENT_ATTACH_FILE_MIME_TYPE,
        0x00004660,
        "3rd lever : Attach file mime type",
        _mkv_parse_attach_file_ele_info
    }, 
    {  /*add for font info*/
        MKV_ELEMENT_ATTACH_FILE_DATA,
        0x0000465C,
        "3rd lever : Attachment file data",
        _mkv_parse_attach_file_ele_info
    }, 
    {  /*add for font info*/
        MKV_ELEMENT_ATTACH_FILE_UID,
        0x000046AE,
        "3rd lever : Attachment file UID",
        _mkv_parse_attach_file_ele_info
    }, 
    {  /*add for font info*/
        MKV_ELEMENT_ATTACH_LINK_UID,
        0x00007446,
        "3rd lever : Attachment link UID",
        _mkv_parse_attach_link_ele_info
    }, 
    
#if 0
    {
        MKV_ELEMENT_2ND_ATTACH_FILE,
        0x000061A7,
        "2nd lever : Attach file",
        _mkv_parse_attach_file
    },
#endif
    {
        MKV_ELEMENT_1ST_CHAPTERS,
        0x1043A770,    
        "1st lever : Chapters",
        _mkv_parse_chapters_info
    },
    {
        MKV_ELEMENT_1ST_TAGS,
        0x1254C367,    
        "1st lever : Tags",
        _mkv_parse_tags
    },
    {
        MKV_ELEMENT_2ND_TAG_ENTRY,
        0x00007373,
        "2nd lever : Tag Entry",
        _mkv_parse_tag_entry
    },
#if 0
    {
        MKV_ELEMENT_3RD_TARGET,
        0x000063C0,
        "3rd lever : Target",
        _mkv_only_parse_mem
    },
    {
        MKV_ELEMENT_4TH_TARGET_TRACK_UID,
        0x000063C5,
        "4th lever : Target Track UID",
        _mkv_parse_target_track_uid
    },
#endif
    {
        MKV_ELEMENT_3RD_SIMPLE_TAG,
        0x000067C8,
        "3rd lever : Simple Tag",
        _mkv_parse_simple_tag
    },
    {
        MKV_ELEMENT_4TH_TAG_NAME,
        0x000045A3,
        "4th lever : Tag Name",
        _mkv_parse_tag_name
    },
    {
        MKV_ELEMENT_4TH_TAG_STRING,
        0x00004487,
        "4th lever : Tag String",
        _mkv_parse_tag_string
    }
};

/*add for font info*/
static FONT_INFO_T g_font_info;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  MKV_GETWLE
 *
 * Description: This function will get little endian word from buffer
 *
 * Inputs:  pui1_buf : specifies the data buffer
 *          sz_buf_len:specifies buffer length
 *          
 * Returns: UINT16 :  the word value
 ----------------------------------------------------------------------------*/
 UINT16 MKV_GETWLE(UINT8* pui1_buf, SIZE_T sz_buf_len)
{
    UINT16 ui2_return = 0;

    if (sz_buf_len < 2)
    {
        DBG_ERROR(("MKV_GETWLE: wrong buffer length!!"));
        return ui2_return;
    }
    ui2_return = MKV_WLE((*pui1_buf), (*(pui1_buf+1)));
    return ui2_return;
}

/*-----------------------------------------------------------------------------
 * Name:  MKV_GETDWLE
 *
 * Description: This function will get little endian dword from buffer
 *
 * Inputs:  pui1_buf : specifies the data buffer
 *          sz_buf_len:specifies buffer length
 *          
 * Returns: UINT32 :  the dword value
 ----------------------------------------------------------------------------*/
 UINT32 MKV_GETDWLE(UINT8* pui1_buf, SIZE_T sz_buf_len)
{
    UINT32 ui4_return = 0;

    if (sz_buf_len < 4)
    {
        DBG_ERROR(("MKV_GETDWLE: wrong buffer length!!"));
        return ui4_return;
    }
    ui4_return = MKV_DWLE((*pui1_buf), (*(pui1_buf+1)), (*(pui1_buf+2)), (*(pui1_buf+3)));
    return ui4_return;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_strm_atrbt_by_idx
 *
 * Description: This function will get steam info by count&stream type
 *
 * Inputs:  pt_all_strm_info : specifies the stream info poniter
 *          e_strm_rype      : stream type
 *          ui2_count        : the place in stream list, may be not identical 
 *                             to its stream idx
 *          
 * Outputs: pt_minfo_rec, return stream attributes 
 * 
 * Returns: MINFOR_OK :  Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_get_strm_atrbt_by_idx(MINFO_MKV_ALL_STRM_INFO_T* pt_all_strm_info,
                                 MKV_STREAM_TYPE_E          e_strm_type,
                                 UINT16                     ui2_count,
                                 MINFO_INFO_T*              pt_minfo_rec)
{ 
    MINFO_TYPE_STM_ATRBT_T*         pt_type_str_atrbt; 
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_video_strm;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_audio_strm;
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_sp_strm;
    
     /*Parameter check*/
    if ((NULL == pt_all_strm_info) || (NULL == pt_minfo_rec))
    {
        DBG_ERROR(("_mkv_get_strm_atrbt_by_idx:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*at least from the first place*/
    if (0 == ui2_count)
    {
        DBG_ERROR(("_mkv_get_strm_atrbt_by_idx: count is 0\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*get output pointer*/
    pt_type_str_atrbt = &(pt_minfo_rec->u.t_stm_attribute);
    
    switch (e_strm_type)
    {
    case MKV_STRM_VIDEO_TYPE:
        pt_video_strm = pt_all_strm_info->pt_video_strm_info_head;
        while (pt_video_strm != NULL)
        {
            if (pt_video_strm->ui1_stream_idx == (UINT8)ui2_count)
            {
                break;
            }
            pt_video_strm = pt_video_strm->pt_next_video_strm;
        }
        if (NULL != pt_video_strm)
        {
            pt_type_str_atrbt->t_stm_id.u.t_mkv_stm_id = pt_video_strm->ui4_stream_number;
            pt_type_str_atrbt->b_is_codec_support = TRUE;
            x_memcpy((VOID*)&(pt_type_str_atrbt->u.t_mkv_video_stm_attr),
                     (VOID*)&(pt_video_strm->t_video_info),
                      sizeof(MINFO_MKV_VIDEO_STM_ATRBT_T));
            if (pt_video_strm->t_video_info.e_enc == MINFO_INFO_VID_ENC_UNKNOWN)
            {
                pt_type_str_atrbt->b_is_codec_support = FALSE;
            }
            return MINFOR_OK;
        }
        break;
    case MKV_STRM_AUDIO_TYPE:
        pt_audio_strm = pt_all_strm_info->pt_audio_strm_info_head;
        while (pt_audio_strm != NULL)
        {
            if (pt_audio_strm->ui1_stream_idx == (UINT8)ui2_count)
            {
                break;
            }
            pt_audio_strm = pt_audio_strm->pt_next_audio_strm;
        }
        if (NULL != pt_audio_strm)
        {
            pt_type_str_atrbt->t_stm_id.u.t_mkv_stm_id = pt_audio_strm->ui4_stream_number;
            pt_type_str_atrbt->b_is_codec_support = TRUE;
            x_memcpy((VOID*)&(pt_type_str_atrbt->u.t_mkv_audio_stm_attr),
                     (VOID*)&(pt_audio_strm->t_audio_info),
                      sizeof(MINFO_MKV_AUDIO_STM_ATRBT_T));
            if (pt_audio_strm->t_audio_info.e_enc == MINFO_INFO_AUD_ENC_UNKNOWN)
            {
                pt_type_str_atrbt->b_is_codec_support = FALSE;
            }
            return MINFOR_OK;
        }
        break;
    case MKV_STRM_SUBTILE_TYPE:
        pt_sp_strm = pt_all_strm_info->pt_subtitle_strm_info_head;
        while (pt_sp_strm != NULL)
        {
            if (pt_sp_strm->ui1_stream_idx == (UINT8)ui2_count)
            {
                break;
            }
            pt_sp_strm = pt_sp_strm->pt_next_subtitle_strm;
        }
        if (NULL != pt_sp_strm)
        {
            pt_type_str_atrbt->t_stm_id.u.t_mkv_stm_id = pt_sp_strm->ui4_stream_number;
            pt_type_str_atrbt->b_is_codec_support = TRUE;
            x_memcpy((VOID*)&(pt_type_str_atrbt->u.t_mkv_sp_stm_attr),
                     (VOID*)&(pt_sp_strm->t_subtitle_info),
                      sizeof(MINFO_MKV_SUBTITLE_STM_ATRBT_T));
            if (pt_sp_strm->t_subtitle_info.e_subtitle_type == MINFO_INFO_SUBTITLE_TYPE_UNKNOWN)
            {
                pt_type_str_atrbt->b_is_codec_support = FALSE;
            }
            return MINFOR_OK;
        }
        break;
    default:
        break;
    }
    DBG_ERROR(("_mkv_get_strm_atrbt_by_idx:not found,"
               "stream type=%d, count=%u\r\n", 
               e_strm_type, 
               ui2_count));
    return MINFOR_INFO_NOT_FOUND;
}

/*add for font info*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_font_data
 *
 * Description: This function will get font data by count&stream type
 *
 * Inputs:  pt_all_strm_info : specifies the stream info poniter
 *              ui2_count        : the place in stream list, may be not identical 
 *                                      to its stream idx
 *          
 * Outputs: pt_minfo_rec, return stream attributes 
 * 
 * Returns: MINFOR_OK :  Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_get_font_data(MINFO_OBJ_T* pt_minfo_obj,
                             MINFO_MKV_ALL_STRM_INFO_T* pt_all_strm_info,
                             UINT16                     ui2_count,
                             MINFO_INFO_T*              pt_minfo_rec)
{
    INT32                  i4_ret;
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_sp_strm;
    FONT_INFO_T*                    pt_cur_font;
    getpos_fct              pf_get_pos;
    setpos_fct              pf_set_pos;
    UINT64                  ui8_old_pos;
    /*MINFO_MKV_INF_T*        pt_mkv_info_obj;*/
    copybytes_fct           pf_copybyte;
    UINT32                  ui4_read_size;
    UINT32                  ui4_tmp = 0;

     /*Parameter check*/
    if ((NULL == pt_minfo_obj)|| (NULL == pt_all_strm_info) || (NULL == pt_minfo_rec))
    {
        DBG_ERROR(("_mkv_get_font_data:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*pt_mkv_info_obj = (MINFO_MKV_INF_T*)pt_minfo_obj->pv_handler_obj;  */
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;    
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos(pt_minfo_obj, &ui8_old_pos);  /*backup old position*/    
    
    /*at least from the first place*/
    if (0 == ui2_count)
    {
        DBG_ERROR(("_mkv_get_font_data: count is 0\r\n"));
        return MINFOR_INV_ARG;
    }
    
    pt_sp_strm = pt_all_strm_info->pt_subtitle_strm_info_head;
    while (pt_sp_strm != NULL)
    {
        if (pt_sp_strm->ui1_stream_idx == (UINT8)ui2_count)
        {
            break;
        }
        pt_sp_strm = pt_sp_strm->pt_next_subtitle_strm;
    }
    
    if (NULL != pt_sp_strm)
    {
        pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
        while(pt_cur_font)
        {
            if(pt_cur_font->ui8_font_id == pt_sp_strm->t_subtitle_info.ui8_attach_link_id)
            {
                break;
            }
            pt_cur_font = pt_cur_font->pt_next_font;
        }
        if(pt_cur_font && !pt_cur_font->pv_file_data)
        {
            pt_cur_font->pv_file_data = _mm_util_mem_alloc(pt_cur_font->ui4_font_file_size + 4);
            if (NULL == pt_cur_font->pv_file_data)
            {
                DBG_ERROR(("_mkv_get_font_data: fail to allocate memory(%d)\r\n ", pt_cur_font->ui4_font_file_size));
                return MINFOR_OUT_OF_MEMORY;
            }
            x_memset(pt_cur_font->pv_file_data, 0, pt_cur_font->ui4_font_file_size);
            pf_set_pos(pt_minfo_obj, (INT64)pt_cur_font->ui8_file_offset, FM_SEEK_BGN);
            ui4_tmp = LITTLE_TO_BIG_DWORD(pt_cur_font->ui4_font_file_size);
            x_memcpy(pt_cur_font->pv_file_data, (VOID*)&ui4_tmp, 4);
            i4_ret = pf_copybyte(pt_minfo_obj, (VOID*)((UINT8*)pt_cur_font->pv_file_data + 4), 
                                 pt_cur_font->ui4_font_file_size, 
                                 pt_cur_font->ui4_font_file_size, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                DBG_INFO(("_mkv_get_font_data:copy data error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            if (ui4_read_size < pt_cur_font->ui4_font_file_size)
            {
                DBG_INFO(("_mkv_get_font_data:copy not complete!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
        }

        pf_set_pos(pt_minfo_obj, (INT64)ui8_old_pos, FM_SEEK_BGN);
        
        return MINFOR_OK;
    }

    return MINFOR_INFO_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_element_size
 *
 * Description: This function get element size.
 *
 * Inputs:  pt_ebml_element : specify the ebml element
 *          pt_mkv_info_obj : specify the mkv info object
 *          b_file          : specify the element data is stored in file or in buffer
 *
 * Returns: UINT64 :  the size of element.
 ----------------------------------------------------------------------------*/
UINT64 _mkv_get_element_size(EBML_ELE_BASIC_T* pt_ebml_element, 
                                  MINFO_OBJ_T*      pt_minfo_obj, 
                                  MINFO_MKV_INF_T*  pt_mkv_info_obj,
                                  BOOL              b_file)
{
    UINT64              ui8_ele_size;
    getpos_fct          pf_get_pos; 
    UINT64              ui8_cur_pos;
    CHAR*               psz_desc;
    
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    ui8_ele_size = _ebml_get_ele_size(pt_ebml_element);
    /*need check this size is correct or error value*/
    if (b_file)
    {
        /*ebml element in file, check if element size > file size*/
        pf_get_pos(pt_minfo_obj, &ui8_cur_pos);
        if ((ui8_ele_size + ui8_cur_pos) > (pt_mkv_info_obj->ui8_size))
        {
            psz_desc = _mkv_get_element_desc(pt_ebml_element);
            psz_desc = psz_desc;
            DBG_INFO(("_mkv_get_element_size: element size larger than file size!\r\n"));
            DBG_INFO(("current element is %s!\r\n", psz_desc));
            ui8_ele_size = pt_mkv_info_obj->ui8_size - ui8_cur_pos;
        }
    }
    else
    {
        /*ebml element in buffer, check if element size > buffer size*/
        ui8_cur_pos = pt_mkv_info_obj->ui8_cur_parser_offset;
        if (((UINT32)(ui8_ele_size + ui8_cur_pos)) > (pt_mkv_info_obj->ui4_buf_length))
        {
            psz_desc = _mkv_get_element_desc(pt_ebml_element);
            psz_desc = psz_desc;
            DBG_INFO(("_mkv_get_element_size: element size larger than buffer size!\r\n"));
            DBG_INFO(("current element is %s!\r\n", psz_desc));
            ui8_ele_size = (UINT64)(pt_mkv_info_obj->ui4_buf_length) - ui8_cur_pos;
        }
    }
    return ui8_ele_size;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_element_type
 *
 * Description: This function get element type.
 *
 * Inputs:  pt_ebml_element : specify the ebml element
 *
 * Returns: MKV_ELEMENT_TYPE_E :  the enum of element's type.
 ----------------------------------------------------------------------------*/
MKV_ELEMENT_TYPE_E _mkv_get_element_type(EBML_ELE_BASIC_T* pt_ebml_element)
{
    UINT32 ui4_id;
    UINT8  ui1_cnt;
    UINT8  ui1_num;
    
    ui1_num = (UINT8)(sizeof(g_at_mkv_element_op) /sizeof(MINFO_MKV_ELEMENT_OPER_T));
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    for (ui1_cnt = 0; ui1_cnt < ui1_num; ++ui1_cnt)
    {
        if (g_at_mkv_element_op[ui1_cnt].ui4_element_id == ui4_id)
        {
            DBG_INFO(("_mkv_get_element_type:%s\r\n", g_at_mkv_element_op[ui1_cnt].psz_description));
            return g_at_mkv_element_op[ui1_cnt].e_element_type;
        }
    }
    return MKV_ELEMENT_UNKNOWN;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_element_desc
 *
 * Description: This function get element's description.
 *
 * Inputs:  pt_ebml_element : specify the ebml element
 *
 * Returns: CHAR* : the element's description string.
 ----------------------------------------------------------------------------*/
CHAR* _mkv_get_element_desc(EBML_ELE_BASIC_T* pt_ebml_element)
{
    UINT32 ui4_id;
    UINT8  ui1_cnt;
    UINT8  ui1_num;
    
    ui1_num = (UINT8)(sizeof(g_at_mkv_element_op) /sizeof(MINFO_MKV_ELEMENT_OPER_T));
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    for (ui1_cnt = 0; ui1_cnt < ui1_num; ++ui1_cnt)
    {
        if (g_at_mkv_element_op[ui1_cnt].ui4_element_id == ui4_id)
        {
            return g_at_mkv_element_op[ui1_cnt].psz_description;
        }
    }
    return g_at_mkv_element_op[0].psz_description;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_element_parse_func
 *
 * Description: This function get element's parse function point.
 *
 * Inputs:  pt_ebml_element : specify the ebml element
 *
 * Returns: mkv_parse_element_func  : the element's parse function point.
 ----------------------------------------------------------------------------*/
mkv_parse_element_func _mkv_get_element_parse_func(EBML_ELE_BASIC_T* pt_ebml_element)
{
    UINT32                 ui4_id;
    UINT8                  ui1_cnt;
    UINT8                  ui1_num;
    mkv_parse_element_func pf_parse;
    
    ui1_num = (UINT8)(sizeof(g_at_mkv_element_op) /sizeof(MINFO_MKV_ELEMENT_OPER_T));
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    /*unkown element*/
    pf_parse = g_at_mkv_element_op[0].pf_parse_element;
    for (ui1_cnt = 1; ui1_cnt < ui1_num; ++ui1_cnt)
    {
        if (g_at_mkv_element_op[ui1_cnt].ui4_element_id == ui4_id)
        {
            /*DBG_INFO(("_mkv_get_element_parse_func:%s\r\n", g_at_mkv_element_op[ui1_cnt].psz_description));*/
            pf_parse = g_at_mkv_element_op[ui1_cnt].pf_parse_element;
            break; 
        }
    }
    return pf_parse;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_string
 *
 * Description: This function parse string data from buffer.
 *
 * Inputs:  pt_mkv_info_obj : specify the mkv info object
 *          ui8_size:         specify the string data length
 *          e_string_type     specify the string type
 *
 * Returns: void*  : the string pointer.
 ----------------------------------------------------------------------------*/
VOID* _mkv_parse_string(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, MKV_STRING_TYPE_E e_string_type)
{
    UINT32 ui4_size;
    VOID*  pv_string;
    UINT8* pui1_buf_pos;
    UINT32 ui4_buf_len;
    
    pv_string = NULL;
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_string: string size is 0, no need parse!\r\n"));
        return pv_string;
    }
    if ((ui8_size >> 32))
    {
        DBG_ERROR(("_mkv_parse_string:string size too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return pv_string;
    }
    ui4_size = (UINT32)(ui8_size);
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    if ((pt_mkv_info_obj->ui8_cur_parser_offset) >> 32)
    {
        DBG_ERROR(("_mkv_parse_string:buf offset too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return pv_string;
    }
    ui4_buf_len = pt_mkv_info_obj->ui4_buf_length - (UINT32)pt_mkv_info_obj->ui8_cur_parser_offset;
    if (e_string_type == MKV_STRING_CHAR)
    {
        pv_string = (VOID*)_ebml_buf_string((VOID*)pui1_buf_pos, ui4_buf_len, ui4_size);
    }
    else if (e_string_type == MKV_STRING_W_CHAR)
    {
        pv_string = (VOID*)_ebml_buf_utf8_string((VOID*)pui1_buf_pos, ui4_buf_len, ui4_size);
    }
    return pv_string;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_uint
 *
 * Description: This function parse uint data from buffer.
 *
 * Inputs:  pt_mkv_info_obj : sspecify the mkv info object
 *          ui8_size:         the uint data length
 *
 * Outputs: pui8_value :  the uint value
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_uint(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, UINT64* pui8_value)
{
    INT32  i4_ret;
    UINT32 ui4_size;
    UINT8* pui1_buf_pos;
    UINT32 ui4_buf_len;
    
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_uint: uint size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }
    if ((ui8_size >> 32))
    {
        DBG_ERROR(("_mkv_parse_uint:uint size too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_size = (UINT32)(ui8_size);
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    if ((pt_mkv_info_obj->ui8_cur_parser_offset) >> 32)
    {
        DBG_ERROR(("_mkv_parse_uint:buf offset too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_buf_len = pt_mkv_info_obj->ui4_buf_length - (UINT32)pt_mkv_info_obj->ui8_cur_parser_offset;
    i4_ret = _ebml_buf_uint((VOID*)pui1_buf_pos,ui4_buf_len, ui4_size, pui8_value);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_uint:parse data error [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_luint
 *
 * Description: This function parse uint data from buffer.
 *
 * Inputs:  pt_mkv_info_obj : sspecify the mkv info object
 *          ui8_size:         the uint data length
 *
 * Outputs: pui8_value :  the uint value
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_luint(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, UINT64* pui8_value1, UINT64* pui8_value2)
{
    INT32  i4_ret;
    UINT32 ui4_size;
    UINT8* pui1_buf_pos;    
    UINT32 ui4_buf_len;
    
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_uint: uint size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }
    if ((ui8_size >> 32))
    {
        DBG_ERROR(("_mkv_parse_uint:uint size too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_size = (UINT32)(ui8_size);
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    if ((pt_mkv_info_obj->ui8_cur_parser_offset) >> 32)
    {
        DBG_ERROR(("_mkv_parse_uint:buf offset too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_buf_len = pt_mkv_info_obj->ui4_buf_length - (UINT32)pt_mkv_info_obj->ui8_cur_parser_offset;
    i4_ret = _ebml_buf_luint((VOID*)pui1_buf_pos,ui4_buf_len, ui4_size, pui8_value1, pui8_value2);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_uint:parse data error [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_int
 *
 * Description: This function parse int data from buffer.
 *
 * Inputs:  pt_mkv_info_obj : specify the mkv info object
 *          ui8_size:         the int data length
 *
 * Outputs: pi8_value :  the int value
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_int(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, INT64* pi8_value)
{
    INT32  i4_ret;
    UINT32 ui4_size;
    UINT8* pui1_buf_pos;
    UINT32 ui4_buf_len;
    
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_int: int size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }
    if ((ui8_size >> 32))
    {
        DBG_ERROR(("_mkv_parse_int:int size too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_size = (UINT32)(ui8_size);
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    if ((pt_mkv_info_obj->ui8_cur_parser_offset) >> 32)
    {
        DBG_ERROR(("_mkv_parse_int:buf offset too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_buf_len = pt_mkv_info_obj->ui4_buf_length - (UINT32)pt_mkv_info_obj->ui8_cur_parser_offset;
    i4_ret = _ebml_buf_int((VOID*)pui1_buf_pos,ui4_buf_len, ui4_size, pi8_value);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_int:parse data error [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_float
 *
 * Description: This function parse float data from buffer.
 *
 * Inputs:  pt_mkv_info_obj : sspecify the mkv info object
 *          ui8_size:         the float data length
 *
 * Outputs: pt_value :  the float value
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_float(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT64 ui8_size, 
                       FLOAT_FRACTION_T* pt_value)
{
    INT32               i4_ret;
    UINT32              ui4_size;
    UINT8*              pui1_buf_pos;
    UINT32              ui4_buf_len;
    UINT64              ui8_value;
    
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_float: float size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }
    if ((ui8_size >> 32))
    {
        DBG_ERROR(("_mkv_parse_float:float size too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_size = (UINT32)(ui8_size);
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    if ((pt_mkv_info_obj->ui8_cur_parser_offset) >> 32)
    {
        DBG_ERROR(("_mkv_parse_float:buf offset too large [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_buf_len = pt_mkv_info_obj->ui4_buf_length - (UINT32)pt_mkv_info_obj->ui8_cur_parser_offset;
    i4_ret = _ebml_buf_float((VOID*)pui1_buf_pos, ui4_buf_len, ui4_size, &ui8_value);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_float:parse data error [%s]!\r\n", _mkv_get_element_desc(&(pt_mkv_info_obj->t_curr_element))));
    }
    i4_ret = _ebml_float_to_fraction(ui8_value, (UINT32)ui8_size, pt_value);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_element_head
 *
 * Description: This function parse element's head (id and size).
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          b_need_load       specify if need load this element's data from file to buffer
 *
 * Outputs: pt_element        specify the ebml element
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_element_head(MINFO_OBJ_T* pt_minfo_obj, 
                              EBML_ELE_BASIC_T* pt_element,
                              MINFO_MKV_INF_T*  pt_mkv_info_obj,
                              BOOL b_need_load)
{
    setpos_fct                  pf_set_pos;  
    getpos_fct                  pf_get_pos; 
    copybytes_fct               pf_copybyte;
    UINT64                      ui8_file_pos;
    UINT32                      ui4_read_size;
    UINT8                       ui1_element_data_offset;
    INT32                       i4_ret;
    UINT32                      ui4_copy_bytes;
    
    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (b_need_load)
    {
        pf_get_pos(pt_minfo_obj, &ui8_file_pos);
        ui4_copy_bytes = (UINT32)MKV_ELEMENT_MAX_HEAD_LEN;
        if ((ui8_file_pos + ui4_copy_bytes) > (pt_mkv_info_obj->ui8_size))
        {
            DBG_INFO(("_mkv_parse_element_head: element head size is smaller than 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_info_obj->ui8_size - ui8_file_pos);
        }
        i4_ret = pf_copybyte(pt_minfo_obj, pt_mkv_info_obj->pui1_header_buf, 
                             pt_mkv_info_obj->ui4_buf_length, 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_parse_element_head: copy head error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        pt_mkv_info_obj->ui8_cur_parser_offset = 0;
    }
    i4_ret = _ebml_buf_ele_base((VOID*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset), 
                                 MKV_ELEMENT_MAX_HEAD_LEN, 0, pt_element);
    if (i4_ret != EBMLR_OK)
    {
        if (i4_ret == EBMLR_ERR)
        {
            return MINFOR_FILE_ERR;
        }
        else
        {
            DBG_ERROR(("_mkv_parse_element_head: parse head error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
    }
    /*set parser pos to element data position*/
    ui1_element_data_offset = _ebml_get_ele_head_len(pt_element);
    if (b_need_load)
    {
        pf_set_pos(pt_minfo_obj, (INT64)(ui1_element_data_offset + ui8_file_pos), FM_SEEK_BGN); 
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui1_element_data_offset;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_feeder_parse
 *
 * Description: This function load element's data to buffer and parse it.
 *
 * Inputs:  pt_minfo_obj           specify the media info object
 *          ui8_broken_data_size   specify the broken data size
 *          b_file                 specify if skip data from file or from buffer
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_skip_data(MINFO_OBJ_T* pt_minfo_obj, UINT64 ui8_broken_data_size, MINFO_MKV_INF_T*  pt_mkv_info_obj, BOOL b_file)
{
    setpos_fct                  pf_set_pos;  
    getpos_fct                  pf_get_pos; 
    UINT64                      ui8_file_pos;
    
    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    if (b_file)
    {
        pf_get_pos(pt_minfo_obj, &ui8_file_pos);
        pf_set_pos(pt_minfo_obj , (INT64)(ui8_file_pos + ui8_broken_data_size), FM_SEEK_BGN); 
    }
    else
    {
        pt_mkv_info_obj->ui8_cur_parser_offset += ui8_broken_data_size;
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_jump_element_data
 *
 * Description: This function ignore the element's data. (for no need parse element)
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_element        specify the ebml element
 *          b_file            specify if jump data from file or from buffer
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_jump_element_data(MINFO_OBJ_T* pt_minfo_obj, 
                             EBML_ELE_BASIC_T* pt_element,
                             MINFO_MKV_INF_T*  pt_mkv_info_obj,
                             BOOL b_file)
{
    UINT64  ui8_data_size;
    
    ui8_data_size = _mkv_get_element_size(pt_element, pt_minfo_obj, pt_mkv_info_obj, b_file);
    _mkv_skip_data(pt_minfo_obj, ui8_data_size, pt_mkv_info_obj, b_file);
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_load_element_data
 *
 * Description: This function load element's data to buffer.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          ui8_data_size     specify the data size need to load
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_load_element_data(MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T*  pt_mkv_info_obj, UINT64 ui8_data_size)
{
    copybytes_fct               pf_copybyte;
    UINT32                      ui4_read_size;
    INT32                       i4_ret;
    UINT32                      ui4_ele_size;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if ((ui8_data_size >> 32))
    {
        DBG_INFO(("_mkv_load_element_data:load size too large!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui4_ele_size = (UINT32)(ui8_data_size);
    if (pt_mkv_info_obj->ui4_buf_length < ui4_ele_size)
    {
        DBG_INFO(("_mkv_load_element_data: buffer size < element data size!\r\n"));
         /*free old buffer*/
        _mm_util_mem_free(pt_mkv_info_obj->pui1_header_buf);
        pt_mkv_info_obj->pui1_header_buf = NULL;
        pt_mkv_info_obj->ui4_buf_length = 0;
        /*reallocate new buffer ,set new length*/
        pt_mkv_info_obj->pui1_header_buf = (UINT8*) _mm_util_mem_alloc(ui4_ele_size);
        if (NULL == pt_mkv_info_obj->pui1_header_buf)
        {
            DBG_INFO(("_mkv_load_element_data: fail to allocate memory(%d)\r\n ", ui4_ele_size));
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_mkv_info_obj->ui4_buf_length = ui4_ele_size;
    }
    x_memset(pt_mkv_info_obj->pui1_header_buf, 0, pt_mkv_info_obj->ui4_buf_length);
    i4_ret = pf_copybyte(pt_minfo_obj, pt_mkv_info_obj->pui1_header_buf, 
                         pt_mkv_info_obj->ui4_buf_length, 
                         ui4_ele_size, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_INFO(("_mkv_load_element_data:copy data error!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if (ui4_read_size < ui4_ele_size)
    {
        DBG_INFO(("_mkv_load_element_data:copy not complete!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset = 0;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_feeder_parse
 *
 * Description: This function load element's data to buffer and parse it.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_feeder_parse( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT64                  ui8_size;
    UINT64                  ui8_parse_pos;
    UINT64                  ui8_sub_element_size;
    INT32                   i4_ret;
    mkv_parse_element_func  pf_parse_data;
    CHAR*                   psz_desc;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    psz_desc = _mkv_get_element_desc(pt_ebml_element);
    psz_desc = psz_desc;
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    ui8_parse_pos = 0;
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_feeder_parse: element size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }
    i4_ret = _mkv_load_element_data(pt_minfo_obj, pt_mkv_info_obj, ui8_size);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_feeder_parse load data error : %s!\r\n", psz_desc));
        return MINFOR_INTERNAL_ERR;
    }
    while (ui8_parse_pos < ui8_size)
    {
        if(pt_minfo_obj->b_abort_flag == TRUE)
        {
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, FALSE);
        if (i4_ret == MINFOR_FILE_ERR)
        {
            /*file maybe broken*/
            DBG_INFO(("_mkv_feeder_parse: file maybe broken, prev element is %s!\r\n", psz_desc));
            _mkv_skip_data(pt_minfo_obj, (ui8_size - ui8_parse_pos), pt_mkv_info_obj, FALSE);
            break;
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_feeder_parse parse head error : prev element is %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        psz_desc = _mkv_get_element_desc(pt_ebml_element);
        ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
        ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
        if (pf_parse_data)
        {
            i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
        }
        else
        {
            i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, FALSE);
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_feeder_parse parse data error : %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        ui8_parse_pos += ui8_sub_element_size;
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_only_parse_file
 *
 * Description: This function only parse element from file(not load element's data to buffer).
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_only_parse_file( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT64                  ui8_size;
    UINT64                  ui8_parse_pos;
    UINT64                  ui8_sub_element_size;
    INT32                   i4_ret;
    mkv_parse_element_func  pf_parse_data;
    CHAR*                   psz_desc;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    psz_desc = _mkv_get_element_desc(pt_ebml_element);
    psz_desc = psz_desc;
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    ui8_parse_pos = 0;
    while (ui8_parse_pos < ui8_size)
    {
        if(pt_minfo_obj->b_abort_flag == TRUE)
        {
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        if (i4_ret == MINFOR_FILE_ERR)
        {
            /*file maybe broken*/
            DBG_INFO(("_mkv_only_parse_file: file maybe broken, prev element is %s!\r\n", psz_desc));
            _mkv_skip_data(pt_minfo_obj, (ui8_size - ui8_parse_pos), pt_mkv_info_obj, TRUE);
            break;
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_only_parse_file parse head error : prev element is %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        psz_desc = _mkv_get_element_desc(pt_ebml_element);
        ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
        ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
        pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
        if (pf_parse_data)
        {
            i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
        }
        else
        {
            i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, TRUE);
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_only_parse_file parse data error : %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        ui8_parse_pos += ui8_sub_element_size;
        #if IMPROVE_MKV_PARSER_SPEED
		if(_g_parsed_cluster && _g_parsed_tracks && _g_parsed_segmentinfo && _g_parsed_cues )
        {
            /*!!!_g_seg_size is not accurate*/
            pt_mkv_info_obj->t_idx_info.ui8_data_sz = _g_seg_size;
            break;
        }
        #endif
    }
    return MINFOR_OK;   
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_only_parse_mem
 *
 * Description: This function parse element from buffer.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_only_parse_mem( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT64                  ui8_size;
    UINT64                  ui8_parse_pos;
    UINT64                  ui8_sub_element_size;
    INT32                   i4_ret;
    mkv_parse_element_func  pf_parse_data;
    CHAR*                   psz_desc;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    psz_desc = _mkv_get_element_desc(pt_ebml_element);
    psz_desc = psz_desc;
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    ui8_parse_pos = 0;
    while (ui8_parse_pos < ui8_size)
    {
        if(pt_minfo_obj->b_abort_flag == TRUE)
        {
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, FALSE);
        if (i4_ret == MINFOR_FILE_ERR)
        {
            /*file maybe broken*/
            DBG_INFO(("_mkv_only_parse_mem: file maybe broken, prev element is %s!\r\n", psz_desc));
            _mkv_skip_data(pt_minfo_obj, (ui8_size - ui8_parse_pos), pt_mkv_info_obj, FALSE);
            break;
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_only_parse_mem parse head error : prev element is %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        psz_desc = _mkv_get_element_desc(pt_ebml_element);
        ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
        ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
        if (pf_parse_data)
        {
            i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
        }
        else
        {
            i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, FALSE);
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_only_parse_mem parse data error : %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        ui8_parse_pos += ui8_sub_element_size;
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_add_sp_node
 *
 * Description: This function add a sp stream node.
 *
 * Inputs:  pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_add_sp_node(MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_new_node; 
    
    if (MAX_NS_MKV_SP == pt_mkv_info_obj->t_all_strm_info.ui1_subtitle_strm_num)
    {
        DBG_INFO(("_mkv_add_sp_node: sp too many!\r\n"));
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = NULL;
        return MINFOR_OK;
    }
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_SUBTITLE_STRM_INFO_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_SUBTITLE_STRM_INFO_T));
    /*new subtitle stream*/
    pt_mkv_info_obj->t_all_strm_info.ui1_subtitle_strm_num += 1;
    pt_mkv_info_obj->ui1_cur_strm_num++;
    /*set current parse entry*/
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = pt_new_node;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type = MKV_STRM_SUBTILE_TYPE;
    return MINFOR_OK;
}
#if 0
void _mkv_parse_chk_tpt_segment(MINFO_OBJ_T* pt_minfo_obj)
{
    MINFO_MKV_INF_T*            pt_mkv_info_obj; 
    MINFO_MKV_INF_T*            pt_mkv_info_obj2; 
    MKV_STREAM_MT_INFO_T        t_mt_info;
    MKV_STREAM_TPT_INFO_T       t_tpt_info;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_video_strm_info;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_video_strm_info2;
    
    pt_mkv_info_obj = (MINFO_MKV_INF_T*)pt_minfo_obj->pv_handler_obj;
    if(pt_mkv_info_obj)
    {
        pt_video_strm_info = pt_mkv_info_obj->t_all_strm_info.pt_video_strm_info_head;
        t_mt_info = pt_video_strm_info->u.t_mt_info;

        pt_mkv_info_obj2 = pt_mkv_info_obj->pt_next;
        if(pt_mkv_info_obj2)
        {
            pt_video_strm_info2 = pt_mkv_info_obj2->t_all_strm_info.pt_video_strm_info_head;
            t_tpt_info = pt_video_strm_info2->u.t_tpt_info;
            if(t_mt_info.ui8_enh_seg_uid_high == pt_mkv_info_obj2->t_segment_info.ui8_seg_uid_high &&
               t_mt_info.ui8_enh_seg_uid_low == pt_mkv_info_obj2->t_segment_info.ui8_seg_uid_low &&
               t_mt_info.ui8_enh_track_uid == pt_video_strm_info2->ui8_track_uid &&
               
               t_tpt_info.ui8_master_seg_uid_high == pt_mkv_info_obj->t_segment_info.ui8_seg_uid_high &&
               t_tpt_info.ui8_master_seg_uid_low == pt_mkv_info_obj->t_segment_info.ui8_seg_uid_low &&
               t_tpt_info.ui8_master_track_uid == pt_video_strm_info->ui8_track_uid
               )
            {
                if(pt_video_strm_info2->fg_enh_track_flag)
                {
                    pt_mkv_info_obj->fg_trick_play_seg = FALSE;
                    pt_mkv_info_obj2->fg_trick_play_seg = TRUE;
                }
                else if(pt_video_strm_info->fg_enh_track_flag)
                {
                    pt_mkv_info_obj->fg_trick_play_seg = TRUE;
                    pt_mkv_info_obj2->fg_trick_play_seg = FALSE;
                    pt_minfo_obj->pv_handler_obj = pt_mkv_info_obj2;
                    pt_mkv_info_obj2->pt_next = pt_mkv_info_obj;
                    pt_mkv_info_obj->pt_next = NULL;
                }
            }
        }
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_elements
 *
 * Description: This function parse the mkv file.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_elements(MINFO_OBJ_T* pt_minfo_obj)
{
    setpos_fct                  pf_set_pos;  
    getpos_fct                  pf_get_pos;
    /*store mkv parsed related info*/
    MINFO_MKV_INF_T*            pt_mkv_info_obj; 
    /*read buffer pointer*/
    UINT8*                      pui1_head_buf ;  
    /*return code s*/
    INT32                       i4_ret; 
    EBML_ELE_BASIC_T*           pt_ebml_element;
    BOOL                        fg_srch_segment;
    BOOL                        fg_srch_ebml;
    mkv_parse_element_func      pf_parse;
    UINT64                      ui8_parse_pos;
    UINT64                      ui8_sub_element_size;
    
    DBG_INFO(("mkv_parse_elements:begin to parse mkv\r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (NULL == pt_minfo_obj)
    {
        DBG_ERROR(("mkv_parse_elements: pt_minfo_obj is NULL\r\n"));
        return  MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_get_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        DBG_ERROR(("mkv_parse_elements:input_fct_tbl has NULL function\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj = (MINFO_MKV_INF_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_mkv_info_obj)
    {
        DBG_ERROR(("_mkv_parse_elements:mkv_info_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pui1_head_buf = pt_mkv_info_obj->pui1_header_buf;
    if (NULL == pui1_head_buf)
    {
        DBG_ERROR(("_mkv_parse_elements:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    
    /*get file size*/
    pf_set_pos(pt_minfo_obj , 0, FM_SEEK_END);
    pf_get_pos(pt_minfo_obj, &(pt_mkv_info_obj->ui8_size));
    pf_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN); 

    fg_srch_segment = FALSE;
    fg_srch_ebml = FALSE;
    ui8_parse_pos = 0;
    i4_ret = MINFOR_OK;
    while (ui8_parse_pos < pt_mkv_info_obj->ui8_size)
    {
        /*exit at any time*/
        if(pt_minfo_obj->b_abort_flag == TRUE)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        pf_set_pos(pt_minfo_obj , (INT64)ui8_parse_pos, FM_SEEK_BGN); 
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
        ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
        
        /*parse ebml head*/
        if ((_mkv_get_element_type(pt_ebml_element) == MKV_ELEMENT_TOP_EBML_HEADER))
        {
            if(fg_srch_segment)
            {
                pt_mkv_info_obj->pt_next = (MINFO_MKV_INF_T*)_mm_util_mem_alloc(sizeof(MINFO_MKV_INF_T));
                if (pt_mkv_info_obj->pt_next == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mkv_info_obj->pt_next, 0, sizeof(MINFO_MKV_INF_T));

                pt_mkv_info_obj->pt_next->t_curr_element = pt_mkv_info_obj->t_curr_element;
                pt_mkv_info_obj->pt_next->pui1_header_buf = pt_mkv_info_obj->pui1_header_buf;
                pt_mkv_info_obj->pt_next->ui4_buf_length = pt_mkv_info_obj->ui4_buf_length;
                pt_mkv_info_obj->pt_next->ui8_cur_parser_offset = pt_mkv_info_obj->ui8_cur_parser_offset;
                pt_mkv_info_obj->pt_next->ui8_size = pt_mkv_info_obj->ui8_size;
                
                pt_mkv_info_obj = pt_mkv_info_obj->pt_next;
                pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
            }
            fg_srch_ebml = TRUE;
            pf_parse = g_at_mkv_element_op[MKV_ELEMENT_TOP_EBML_HEADER].pf_parse_element;
            i4_ret = pf_parse(pt_minfo_obj, pt_mkv_info_obj);
            if (i4_ret != MINFOR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            fg_srch_segment = FALSE;
        }
        /*parse segment*/    
        else if ((_mkv_get_element_type(pt_ebml_element) == MKV_ELEMENT_TOP_SEGMENT))
        {
            if(!fg_srch_ebml)
            {
                DBG_ERROR(("_mkv_parse_elements:file lack ebml head!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            fg_srch_segment = TRUE;
            pf_parse = g_at_mkv_element_op[MKV_ELEMENT_TOP_SEGMENT].pf_parse_element;
            i4_ret = pf_parse(pt_minfo_obj, pt_mkv_info_obj);
            if (i4_ret != MINFOR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            fg_srch_ebml = FALSE;
            if((pt_mkv_info_obj->t_segment_info.ui8_duration != 0) &&
               (_mm_div64((UINT64)(_g_seg_size * 90000),
                         (UINT64)(pt_mkv_info_obj->t_segment_info.ui8_duration),
                         NULL) > (UINT64)50*1024*1024))
            {
                MINFO_MKV_AUDIO_STRM_INFO_T*    pt_cur_aud_strm;
                MINFO_MKV_VIDEO_STRM_INFO_T*    pt_cur_vid_strm;
                pt_cur_aud_strm = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
                pt_cur_vid_strm = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
                pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
                pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
                pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_UNKNOWN;
            }
        }
        else
        {
            i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, TRUE);
            if (i4_ret != MINFOR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
        ui8_parse_pos += ui8_sub_element_size;
    }
    if (i4_ret != MINFOR_OK)
    {
        if(_g_parsed_cluster && _g_parsed_tracks && _g_parsed_segmentinfo)
        {
            i4_ret = MINFOR_OK;
        }
        else
        {
            return MINFOR_INTERNAL_ERR;
        }
    }
    /*_mkv_parse_chk_tpt_segment(pt_minfo_obj);*/
    if (fg_srch_segment == FALSE)
    {
        DBG_ERROR(("_mkv_parse_elements: not found segment element in file!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    return i4_ret;
}

/*ebml head*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_ebml_head
 *
 * Description: This function parse ebml head element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_ebml_head( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    return _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_doctype
 *
 * Description: This function parse doctype element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_doctype( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse ebml doc type*/
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    char*             psz_mkv_hdr = NULL;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_1ST_DOCTYPE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_doctype: current element isn't doctype!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if ((ui8_size != 8) && (ui8_size != 4))
    {
        DBG_ERROR(("_mkv_parse_doctype: file isn't matroska or webm, size not correct!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }    
    psz_mkv_hdr = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
    if (psz_mkv_hdr == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((x_memcmp((VOID*)"matroska", psz_mkv_hdr, 8) != 0) &&
        (x_memcmp((VOID*)"webm", psz_mkv_hdr, 4) != 0))
    {
        DBG_ERROR(("_mkv_parse_doctype: file isn't matroska!\r\n"));
        if (psz_mkv_hdr != NULL)
        {
            _ebml_string_free(&psz_mkv_hdr);
        }
        return MINFOR_INTERNAL_ERR;
    }
    /*free psz_mkv_hdr*/
    if (psz_mkv_hdr != NULL)
    {
        _ebml_string_free(&psz_mkv_hdr);
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_segment
 *
 * Description: This function parse segment element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_segment( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    getpos_fct pf_get_pos; 
    EBML_ELE_BASIC_T* pt_ebml_element;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    _g_seg_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);

    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_get_pos(pt_minfo_obj, &(pt_mkv_info_obj->ui8_seg_start_pos));
    pt_mkv_info_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;

#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_segmentinfo     = FALSE;
    _g_parsed_tracks         = FALSE;
    _g_parsed_chapters         = FALSE;
    _g_parsed_tags             = FALSE;
    _g_parsed_cues             = FALSE;
    _g_parsed_cluster         = FALSE;
#endif
    return _mkv_only_parse_file(pt_minfo_obj, pt_mkv_info_obj);
}

/*segment info*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_segment_info
 *
 * Description: This function parse segment info element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_segment_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32 i4_ret;

#if IMPROVE_MKV_PARSER_SPEED
    EBML_ELE_BASIC_T* pt_ebml_element;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    if (_g_parsed_segmentinfo)
    {
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        return MINFOR_OK;
    }
#endif
    
    /*set default value*/
    pt_mkv_info_obj->t_segment_info.ui8_timecode_scale = 1000000;/*ns*/
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    /*set information*/
    pt_mkv_info_obj->t_idx_info.ui8_seg_timecode_scale = pt_mkv_info_obj->t_segment_info.ui8_timecode_scale;
    pt_mkv_info_obj->t_segment_info.ui8_duration = (UINT64)(pt_mkv_info_obj->t_segment_info.ui8_org_duration * 
                                                   pt_mkv_info_obj->t_segment_info.ui8_timecode_scale);
    pt_mkv_info_obj->t_segment_info.ui8_duration = _mm_div64((UINT64)(pt_mkv_info_obj->t_segment_info.ui8_duration * 90),
                                                             (UINT64)1000000,
                                                              NULL); /*****s*90000*****/
                                              
    pt_mkv_info_obj->ui8_duration = pt_mkv_info_obj->t_segment_info.ui8_duration;
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_segmentinfo = TRUE;
#endif
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seg_uid
 *
 * Description: This function parse segment title element
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_SEG_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seg_title: current element isn't segment title!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &pt_mkv_info_obj->t_segment_info.ui8_seg_uid_high,
                              &pt_mkv_info_obj->t_segment_info.ui8_seg_uid_low);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seg_title
 *
 * Description: This function parse segment title element
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seg_title( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse segment title*/
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_TITLE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seg_title: current element isn't segment title!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_segment_info.pws_title = (UTF16_T*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_W_CHAR);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seg_duration
 *
 * Description: This function parse segment duration element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seg_duration( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32             i4_ret;
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element;
    FLOAT_FRACTION_T  t_duration;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_DURATION].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seg_duration: current element isn't segment duration!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_float(pt_mkv_info_obj, ui8_size, &t_duration);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if (t_duration.ui8_denominator != 0)
    {
        pt_mkv_info_obj->t_segment_info.ui8_org_duration = _mm_div64(t_duration.ui8_numerator, 
                                                             t_duration.ui8_denominator,
                                                             NULL);
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seg_date
 *
 * Description: This function parse segment date element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seg_date( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_DATE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seg_date: current element isn't segment date!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_int(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_segment_info.i8_date));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seg_timecode_scale
 *
 * Description: This function parse segment timecode scale element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seg_timecode_scale( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    UINT64              ui8_timecode_scale;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_TIMECODESCALE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seg_timecode_scale: current element isn't segment timecode scale!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_timecode_scale);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_segment_info.ui8_timecode_scale = ui8_timecode_scale; /*ns*/
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seek_head
 *
 * Description: This function parse seek head element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seek_head( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
#if !IMPROVE_MKV_PARSER_SPEED
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    /*current no need to parse seek info, so just skip*/
    _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
    return MINFOR_OK;
#else
    INT32 i4_ret;
    getpos_fct pf_get_pos; 
    setpos_fct pf_set_pos;  
    UINT32 ui4_id;
    UINT64 ui8_parser_offset;
    UINT64 ui8_seek_head_size;
    EBML_ELE_BASIC_T* pt_ebml_element;
    EBML_ELE_BASIC_T  t_ebml_element_bk;
    MINFO_MKV_SEEK_ENTRY_T* pt_seek_list;
    mkv_parse_element_func  pf_parse_data;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui8_seek_head_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    t_ebml_element_bk = pt_mkv_info_obj->t_curr_element;
    ui8_parser_offset = pt_mkv_info_obj->ui8_cur_parser_offset;
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_1ST_SEEKHEAD].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seek_head: current element isn't seek head!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_set_pos    = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pt_mkv_info_obj->t_all_seekhead_info.ui1_seek_num = 0;
    pf_get_pos(pt_minfo_obj, &(pt_mkv_info_obj->t_all_seekhead_info.ui8_seek_start_pos));
    pt_mkv_info_obj->t_all_seekhead_info.pt_curr_parse_entry = NULL;
    i4_ret = _mkv_only_parse_file(pt_minfo_obj, pt_mkv_info_obj);

    pt_seek_list = pt_mkv_info_obj->t_all_seekhead_info.pt_seek_entry_head;
    while (pt_seek_list)
    {
        if (pt_seek_list->ui8_seek_position + pt_mkv_info_obj->ui8_seg_start_pos < pt_mkv_info_obj->ui8_size)
        {
            pt_mkv_info_obj->ui8_cur_parser_offset = pt_seek_list->ui8_seek_position +
                pt_mkv_info_obj->ui8_seg_start_pos;
        }
        else 
        {
            if (pt_seek_list->ui4_seek_id == g_at_mkv_element_op[MKV_ELEMENT_1ST_CUES].ui4_element_id)
            {
                _g_parsed_cues = TRUE;
            }
            pt_seek_list = pt_seek_list->pt_next_seek_entry;
            continue;
        }

        if (pt_seek_list->ui4_seek_id == g_at_mkv_element_op[MKV_ELEMENT_1ST_SEEKHEAD].ui4_element_id ||
            pt_seek_list->ui4_seek_id == g_at_mkv_element_op[MKV_ELEMENT_1ST_CLUSTERS].ui4_element_id)
        {
            pt_seek_list = pt_seek_list->pt_next_seek_entry;
            continue;
        }
        pf_set_pos(pt_minfo_obj, (INT64)pt_mkv_info_obj->ui8_cur_parser_offset, FM_SEEK_BGN);
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        if (i4_ret != MINFOR_OK &&
            pt_seek_list->ui4_seek_id == g_at_mkv_element_op[MKV_ELEMENT_1ST_CUES].ui4_element_id)
        {
            _g_parsed_cues = TRUE;
            pt_seek_list = pt_seek_list->pt_next_seek_entry;
            i4_ret = MINFOR_OK;
            continue;
        }
        else if (i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui4_id = _ebml_get_ele_id(pt_ebml_element);        
        if(pt_seek_list->ui4_seek_id != ui4_id)        
        {            
            if(pt_seek_list->ui4_seek_id == g_at_mkv_element_op[MKV_ELEMENT_1ST_CUES].ui4_element_id)            
            {                
                _g_parsed_cues = TRUE;            
            }            
#ifndef __KERNEL__
            x_dbg_stmt("[Mkv_parse]Parse Seek Head Error,ui4_seek_id = 0X%x,ui4_id =OX%x\n",pt_seek_list->ui4_seek_id,ui4_id);
#endif
            pt_seek_list = pt_seek_list->pt_next_seek_entry;
            continue;        
        }
        pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
        if (pf_parse_data && pf_parse_data != _mkv_parse_seek_head)
        {
            i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
        }
        else
        {
             i4_ret = _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        }
        
        pt_seek_list = pt_seek_list->pt_next_seek_entry;
    }
    pf_set_pos(pt_minfo_obj, 
        (INT64)pt_mkv_info_obj->t_all_seekhead_info.ui8_seek_start_pos + ui8_seek_head_size, 
        FM_SEEK_BGN);
    pt_mkv_info_obj->t_curr_element = t_ebml_element_bk;
    pt_mkv_info_obj->ui8_cur_parser_offset = ui8_parser_offset + ui8_seek_head_size;
    return i4_ret;
#endif
}

#if IMPROVE_MKV_PARSER_SPEED
/*seek entry*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seek_entry
 *
 * Description: This function parse seek entry element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seek_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                   i4_ret;
    MINFO_MKV_SEEK_ENTRY_T* pt_seek_list;
    MINFO_MKV_SEEK_ENTRY_T* pt_new_node; 
    
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_SEEK_ENTRY_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_SEEK_ENTRY_T));
    /*find last node*/
    pt_seek_list = pt_mkv_info_obj->t_all_seekhead_info.pt_seek_entry_head;
    if (pt_seek_list == NULL)
    {
        pt_mkv_info_obj->t_all_seekhead_info.pt_seek_entry_head = pt_new_node;
    }
    else
    {
        while (pt_seek_list->pt_next_seek_entry != NULL)
        {
            pt_seek_list = pt_seek_list->pt_next_seek_entry;
        }
        pt_seek_list->pt_next_seek_entry = pt_new_node;
    }
    /*set current parse entry*/
    pt_mkv_info_obj->t_all_seekhead_info.ui1_seek_num += 1;
    pt_mkv_info_obj->t_all_seekhead_info.pt_curr_parse_entry = pt_new_node;
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seek_id
 *
 * Description: This function parse seek id element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seek_id( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek id*/
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    UINT64              ui8_seek_id;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_SEEK_ID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seek_id: current element isn't seek id!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_seek_id);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_seekhead_info.pt_curr_parse_entry->ui4_seek_id = (UINT32)ui8_seek_id;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_seek_position
 *
 * Description: This function parse seek position element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_seek_position( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_SEEK_POSITION].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_seek_position: current element isn't seek position!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/ 
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_seekhead_info.pt_curr_parse_entry->ui8_seek_position));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track
 *
 * Description: This function parse track element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32 i4_ret;
    
#if IMPROVE_MKV_PARSER_SPEED
    EBML_ELE_BASIC_T* pt_ebml_element;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    if (_g_parsed_tracks)
    {
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        return MINFOR_OK;
    }
#endif
    i4_ret = _mkv_only_parse_file(pt_minfo_obj, pt_mkv_info_obj);
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_tracks = TRUE;
#endif
    return i4_ret;
}

INT32 _mkv_parse_track_payload_data( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT8*              pui1_buf_pos;
    UINT8*              pui1_strd_chunk;
    UINT32              ui4_strd_size;
    
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element = NULL;
    MINFO_TYPE_DRM_INFO_T* pt_drm_info = NULL; 
    
    pt_drm_info = &pt_mkv_info_obj->t_minfo_drm_info;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_PAYLOAD_DATA].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_payload_data: current element isn't track payload data!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pui1_buf_pos = (UINT8*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset);
    /*to do parse*/
    DBG_INFO(("**********************DRM PARSE****************\n"));
    pt_drm_info->t_minfo_drm_type = DRM_TYPE_DIVX_DRM;
    ui4_strd_size = (SIZE_T)ui8_size;        
    pt_drm_info->u.t_minfo_divx_drm_obj.pui1_strd_chunk = 
       (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui4_strd_size);
    pui1_strd_chunk = pt_drm_info->u.t_minfo_divx_drm_obj.pui1_strd_chunk;
    if (pui1_strd_chunk == NULL)
    {
         return MINFOR_OUT_OF_MEMORY;
    }
    _mm_util_mem_copy(pui1_strd_chunk, pui1_buf_pos, ui4_strd_size);
    pt_mkv_info_obj->ui4_drm_size = ui4_strd_size;
    return MINFOR_OK;


}

INT32 _mkv_parse_track_data( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32  i4_ret;
    
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

/*parse track entry*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_entry
 *
 * Description: This function parse track entry element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    MINFO_MKV_VIDEO_STRM_INFO_T*      pt_cur_video_strm;
    MINFO_MKV_AUDIO_STRM_INFO_T*      pt_cur_audio_strm;
    #if SUPPORT_MKV_SUBTITLE
    MINFO_MKV_SUBTITLE_STRM_INFO_T*   pt_cur_sp_strm;
    #endif
    INT32                             i4_ret;
    UINT64                            ui8_def_duration;
    UINT32                            ui4_cnt = 0;
    
    /*reset generic information*/
    ui8_def_duration = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration;
    x_memset(&(pt_mkv_info_obj->t_all_strm_info.t_strm_properties), 0, sizeof(MKV_STREAM_PROPERTIES_T));

    /*set default value*/
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration = ui8_def_duration;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_enabled = 1;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_default = 1;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_forced = 0;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_numerator = 1;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_denominator = 1;
    x_memcpy(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ac_language, (VOID*)"eng", 3);
    /*to do parse*/
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_track_entry parse error!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*set stream information*/
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm != NULL)
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type == MKV_STRM_VIDEO_TYPE)
        {   
            pt_cur_video_strm = (MINFO_MKV_VIDEO_STRM_INFO_T*)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm);
            i4_ret = _mkv_set_video_info(pt_mkv_info_obj, pt_cur_video_strm);
        }
        else if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type == MKV_STRM_AUDIO_TYPE)
        {
            pt_cur_audio_strm = (MINFO_MKV_AUDIO_STRM_INFO_T*)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm);
            i4_ret = _mkv_set_audio_info(pt_mkv_info_obj, pt_cur_audio_strm);
        }
#if SUPPORT_MKV_SUBTITLE
        else if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type == MKV_STRM_SUBTILE_TYPE)
        {
            pt_cur_sp_strm = (MINFO_MKV_SUBTITLE_STRM_INFO_T*)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm);
            i4_ret = _mkv_set_sp_info(pt_mkv_info_obj, pt_cur_sp_strm);
        }
#endif
    }
    /*free resource*/
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.psz_codec_id != NULL)
    {
        _mm_util_mem_free(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.psz_codec_id);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.psz_codec_id = NULL; 
    }
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private != NULL)
    {
        _mm_util_mem_free(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private = NULL;
    }
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings != NULL)
    {
        _mm_util_mem_free(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings = NULL;
    }
    
    for (ui4_cnt = 0; ui4_cnt < pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_encoding_num; ui4_cnt ++)
    {
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding = 
                    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pt_next;
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pui1_comp_settings_data)
        {
            _mm_util_mem_free(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pui1_comp_settings_data);
            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pui1_comp_settings_data = NULL;
        }
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding = 
                    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_encoding_num = 0;
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_number
 *
 * Description: This function parse track number element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_number( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    UINT64              ui8_track_num;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_TRACK_NUMBER].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_number: current element isn't track number!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    if (ui8_size > 1)
    {
        DBG_ERROR(("_mkv_parse_track_number: track number too large!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_track_num);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_track_num = (UINT8)ui8_track_num;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_uid
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_TRACK_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_uid));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_enh_uid
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_enh_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_ENH_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_mt_info.ui8_enh_track_uid));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_master_uid
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_master_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_MASTER_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_tpt_info.ui8_master_track_uid));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_enh_seg_uid
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_enh_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_ENH_SEG_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_mt_info.ui8_enh_seg_uid_high),
                              &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_mt_info.ui8_enh_seg_uid_low));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_master_seg_uid
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_master_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_MASTER_SEG_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_tpt_info.ui8_master_seg_uid_high),
                              &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_tpt_info.ui8_master_seg_uid_low));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_enh_flg
 *
 * Description: This function parse track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_enh_flg( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    UINT64              ui8_tmp_val;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3ND_TRACK_ENH_FLG].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_uid: current element isn't track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, 
                              ui8_size, 
                              &ui8_tmp_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.fg_enh_track_flag = (ui8_tmp_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*add for font info*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_attach_link_ele_info
 *
 * Description: This function parse track attach link element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_attach_link_ele_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    UINT64              ui8_attach_link_id;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_ATTACH_LINK_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_attach_link_ele_info: current element isn't track attach link!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_attach_link_id);    
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_attach_link_id == 0)
    {
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_attach_link_id = ui8_attach_link_id;
    }

    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_type
 *
 * Description: This function parse track type element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_type( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
#if SUPPORT_MKV_SUBTITLE
    UINT8               ui1_track_type;
#endif
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_TRACK_TYPE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_type: current element isn't track type!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_type));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
#if SUPPORT_MKV_SUBTITLE
    ui1_track_type = (UINT8)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_type;
    if (ui1_track_type == MKV_SUBTITLE_TRACK_TYPE)
    {
        i4_ret = _mkv_add_sp_node(pt_minfo_obj, pt_mkv_info_obj);
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_parse_track_type add sp node error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
    }
#endif
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_flag_default
 *
 * Description: This function parse track default flag element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_flag_default( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_FLAG_DEFAULT].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_flag_default: current element isn't flag default!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_default));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_default_duration
 *
 * Description: This function parse track default duration.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_default_duration( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_DFT_DURATION].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_default_duration: current element isn't default duration!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_timecode_scale
 *
 * Description: This function parse track timecode scale.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_timecode_scale( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32               i4_ret;
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    FLOAT_FRACTION_T    t_tm_scale;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_TRACK_TIMECODE_SCALE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_timecode_scale: current element isn't track timecode scale!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_float(pt_mkv_info_obj, ui8_size, &t_tm_scale);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_numerator = (UINT32)t_tm_scale.ui8_int_part;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_denominator = (UINT32)t_tm_scale.ui8_scale_part;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_language
 *
 * Description: This function parse track language.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_language( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id = 0;
    UINT64              ui8_size = 0;
    EBML_ELE_BASIC_T*   pt_ebml_element = NULL;
    CHAR*               psz_language = NULL;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_LANGUAGE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_language: current element isn't track language!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    psz_language = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
    if (psz_language != NULL)
    {
        x_memcpy(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ac_language, psz_language, 3);
    }
    if (psz_language != NULL)
    {
        _ebml_string_free(&psz_language);
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_codec_id
 *
 * Description: This function parse track codec id element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_codec_id( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id = 0;
    UINT64              ui8_size = 0;
    EBML_ELE_BASIC_T*   pt_ebml_element = NULL;
    CHAR*               psz_codec_id = NULL;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_CODEC_ID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_codec_id: current element isn't codec id!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    psz_codec_id = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.psz_codec_id = psz_codec_id;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_codec_private
 *
 * Description: This function parse track codec private element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_codec_private( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    UINT8*              pui1_codec_private;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_CODEC_PRIVATE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_codec_private: current element isn't codec private!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    if (ui8_size)
    {
        pui1_codec_private = _mm_util_mem_alloc((UINT32)ui8_size);
        if (pui1_codec_private == NULL)
        {
            DBG_ERROR(("_mkv_parse_track_codec_private: memory alloc error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        x_memcpy((VOID*)(pui1_codec_private), 
                 (VOID*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset),
                 (UINT32)ui8_size);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private = pui1_codec_private;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_codec_private_len = ui8_size;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.b_has_codec_private = TRUE;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_track_codec_name
 *
 * Description: This function parse track codec name
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_track_codec_name( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_CODEC_NAME].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_track_codec_private: current element isn't codec private!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    if(x_strncmp((CHAR*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset), "DivXPlus", 8) == 0 ||
       x_strncmp((CHAR*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset), "DivX Plus", 9) == 0)
    {
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.fg_divx_plus = TRUE;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*parse track video*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_video_track
 *
 * Description: This function parse video track element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_video_track( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                        i4_ret;
    MINFO_MKV_VIDEO_STRM_INFO_T* pt_new_node; 
    EBML_ELE_BASIC_T*            pt_ebml_element;
    UINT64                       ui8_size;
    
    if (MAX_NS_MKV_VID == pt_mkv_info_obj->t_all_strm_info.ui1_video_strm_num)
    {
        DBG_INFO(("_mkv_parse_video_track: vid too many!\r\n"));
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = NULL;
        return MINFOR_OK;
    }
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_VIDEO_STRM_INFO_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_VIDEO_STRM_INFO_T));
    /*new video stream*/
    pt_mkv_info_obj->t_all_strm_info.ui1_video_strm_num += 1;
    pt_mkv_info_obj->ui1_cur_strm_num++;
    /*set current parse entry*/
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = pt_new_node;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type = MKV_STRM_VIDEO_TYPE;
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_w
 *
 * Description: This function parse video pixel width element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_w( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_cur_vid_strm;
    UINT64                          ui8_width; 
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_W].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_w: current element isn't video pixel width!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_vid_strm = (MINFO_MKV_VIDEO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_width);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_vid_strm->t_video_info.i4_width = (INT32)ui8_width;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_h
 *
 * Description: This function parse video pixel height element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_h( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_cur_vid_strm;
    UINT64                          ui8_height;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_H].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_h: current element isn't video pixel height!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_vid_strm = (MINFO_MKV_VIDEO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_height);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_vid_strm->t_video_info.i4_height = (INT32)ui8_height;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_crop_t
 *
 * Description: This function parse video pixel crop top element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_crop_t( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_CROP_T].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_crop_t: current element isn't video pixel crop top!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_crop_b
 *
 * Description: This function parse video pixel crop bottom element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_crop_b( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_CROP_B].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_crop_b: current element isn't video pixel crop bottom!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_crop_l
 *
 * Description: This function parse video pixel crop left element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_crop_l( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_CROP_L].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_crop_l: current element isn't video pixel crop left!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_pixel_crop_r
 *
 * Description: This function parse video pixel crop right element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_pixel_crop_r( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_PIXEL_CROP_R].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_pixel_crop_r: current element isn't video pixel crop right!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_disp_w
 *
 * Description: This function parse video display width element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_disp_w( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    UINT64                          ui8_width;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_cur_vid_strm;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_DISP_W].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_disp_w: current element isn't video display width!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_vid_strm = (MINFO_MKV_VIDEO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_width);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_vid_strm->t_video_info.i4_dsp_width = (INT32)ui8_width;
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_vid_disp_h
 *
 * Description: This function parse video display height element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_vid_disp_h( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    UINT64                          ui8_height;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_VIDEO_STRM_INFO_T*    pt_cur_vid_strm;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_VID_DISP_H].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_vid_disp_h: current element isn't video display height!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_vid_strm = (MINFO_MKV_VIDEO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_height);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_vid_strm->t_video_info.i4_dsp_height = (INT32)ui8_height;
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*parse track audio*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_audio_track
 *
 * Description: This function parse audio track element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_audio_track(MINFO_OBJ_T * pt_minfo_obj, MINFO_MKV_INF_T * pt_mkv_info_obj)
{
    INT32                        i4_ret;
    MINFO_MKV_AUDIO_STRM_INFO_T* pt_new_node; 
    EBML_ELE_BASIC_T*            pt_ebml_element;
    UINT64                       ui8_size;
    
    if (MAX_NS_MKV_AUD == pt_mkv_info_obj->t_all_strm_info.ui1_audio_strm_num)
    {
        DBG_INFO(("_mkv_parse_audio_track: aud too many!\r\n"));
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = NULL;
        return MINFOR_OK;
    }
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_AUDIO_STRM_INFO_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_AUDIO_STRM_INFO_T));
    /*new audio stream*/
    pt_mkv_info_obj->t_all_strm_info.ui1_audio_strm_num += 1;
    pt_mkv_info_obj->ui1_cur_strm_num++;
    /*set current parse entry*/
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = pt_new_node;
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_strm_type = MKV_STRM_AUDIO_TYPE;
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_aud_samp_freq
 *
 * Description: This function parse audio sample freq element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_aud_samp_freq( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    UINT64                          ui8_samp_freq = 0;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_cur_aud_strm;
    FLOAT_FRACTION_T                t_samp_freq;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_AUD_SAMP_FREQ].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_aud_samp_freq: current element isn't audio sample freq!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_aud_strm = (MINFO_MKV_AUDIO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_float(pt_mkv_info_obj, ui8_size, &t_samp_freq);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if (t_samp_freq.ui8_scale_part != 0)
    {
        ui8_samp_freq = _mm_div64(t_samp_freq.ui8_int_part, 
                              t_samp_freq.ui8_scale_part,
                              NULL);
    }
    pt_cur_aud_strm->t_audio_info.i4_samples_per_sec = (INT32)ui8_samp_freq;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_aud_output_samp_freq
 *
 * Description: This function parse audio output sample freq element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_aud_output_samp_freq( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_AUD_OUTPUT_SAMP_FREQ].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_aud_output_samp_freq: current element isn't audio output sample freq!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_aud_channels
 *
 * Description: This function parse audio channels element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_aud_channels( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_cur_aud_strm;
    INT64                           i8_channels;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_AUD_CHANNELS].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_aud_channels: current element isn't audio channels!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_aud_strm = (MINFO_MKV_AUDIO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_int(pt_mkv_info_obj, ui8_size, &i8_channels);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_aud_strm->t_audio_info.i2_channels = (INT16)i8_channels;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_aud_bit_depth
 *
 * Description: This function parse audio bit depth element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_aud_bit_depth( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_id;
    UINT64                          ui8_size;
    EBML_ELE_BASIC_T*               pt_ebml_element;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_cur_aud_strm;
    UINT64                          ui8_bit_depth;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_AUD_BIT_DEPTH].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_aud_bit_depth: current element isn't audio bit depth!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_cur_aud_strm = (MINFO_MKV_AUDIO_STRM_INFO_T*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_bit_depth);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_cur_aud_strm->t_audio_info.i2_bits_per_sample = (INT16)ui8_bit_depth;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*parse content encodings*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_encodings
 *
 * Description: This function parse content encodings, now just jump.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_encodings( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element;
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT8*            pui1_content_encodings;
    INT32             i4_ret = 0;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_CONTENT_ENCODINGS].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_encodings: current element isn't content encodings!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    /*to do parse*/
    if (ui8_size)
    {
        pui1_content_encodings = _mm_util_mem_alloc((UINT32)ui8_size);
        if (pui1_content_encodings == NULL)
        {
            DBG_ERROR(("_mkv_parse_content_encodings: memory alloc error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        x_memcpy((VOID*)(pui1_content_encodings), 
                 (VOID*)(pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset),
                 (UINT32)ui8_size);
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings = pui1_content_encodings;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_content_encodings_size = (UINT32)ui8_size;

        
        i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_parse_content_encodings parse error!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        
        return MINFOR_OK;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_encoding
 *
 * Description: This function parse content encoding.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_encoding( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                           i4_ret = 0;
    MKV_STREAM_PROPERTIES_T*        pt_strm_properties = NULL;

    pt_strm_properties = &pt_mkv_info_obj->t_all_strm_info.t_strm_properties;
    if(!pt_strm_properties->pt_content_encoding)
    {
        pt_strm_properties->pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
        if (pt_strm_properties->pt_content_encoding == NULL)
        {
            DBG_INFO(("\n-----------parse content encodings memory not enough-------------\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_strm_properties->pt_curr_content_encoding = pt_strm_properties->pt_content_encoding;
        pt_strm_properties->ui4_encoding_num = 1;
    }
    else if(pt_strm_properties->pt_curr_content_encoding)
    {
        pt_strm_properties->pt_curr_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
        if (pt_strm_properties->pt_curr_content_encoding->pt_next == NULL)
        {
            DBG_INFO(("\n-----------parse content encodings memory not enough-------------\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_strm_properties->pt_curr_content_encoding = pt_strm_properties->pt_curr_content_encoding->pt_next;
        pt_strm_properties->ui4_encoding_num++;
    }
    else
    {
        return MINFOR_INTERNAL_ERR;
    }
    x_memset(pt_strm_properties->pt_curr_content_encoding, 0, sizeof(MINFO_MKV_CONTENT_ENCODING_T));
    if (pt_strm_properties->pt_curr_content_encoding != NULL)
    {
    pt_strm_properties->pt_curr_content_encoding->ui8_scope = 1;
    /*to do parse*/
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_content_encoding parse error!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    }
    
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_encoding_order
 *
 * Description: This function parse content encoding order.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_encoding_order( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    UINT32            ui4_id = 0;
    UINT64            ui8_data_size = 0;
    UINT64            ui8_order = 0;
    INT32             i4_ret = 0;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_5RD_CONTENT_ENCODING_ORDER].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_encoding order: current element isn't content encoding order!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_data_size, &ui8_order);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->ui8_order = ui8_order;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_data_size;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_encoding_scope
 *
 * Description: This function parse content encoding scope.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_encoding_scope( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    UINT32            ui4_id = 0;
    UINT64            ui8_data_size = 0;
    UINT64            ui8_scope = 0;
    INT32             i4_ret = 0;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_5RD_CONTENT_ENCODING_SCOPE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_encoding scope: current element isn't content encoding scope!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_data_size, &ui8_scope);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->ui8_scope = ui8_scope;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_data_size;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_encoding_type
 *
 * Description: This function parse content encoding type.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_encoding_type( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    UINT32            ui4_id = 0;
    UINT64            ui8_data_size = 0;
    UINT64            ui8_type = 0;
    INT32             i4_ret = 0;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_5RD_CONTENT_ENCODING_TYPE].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_encoding type: current element isn't content encoding type!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_data_size, &ui8_type);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->ui8_type = ui8_type;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_data_size;
    
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_compression
 *
 * Description: This function parse content compression.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_compression( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32       i4_ret = 0;
    
    /*to do parse*/
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_content_encoding parse error!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_compalgo
 *
 * Description: This function parse content compalgo.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_compalgo( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    UINT32            ui4_id = 0;
    UINT64            ui8_data_size = 0;
    UINT64            ui8_compalog = 0;
    INT32             i4_ret = 0;

    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_6RD_CONTENT_COMPALGO].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_compalgo: current element isn't content compalgo!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_data_size, &ui8_compalog);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->ui8_compress_algo = ui8_compalog;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_data_size;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_content_comp_settings
 *
 * Description: This function parse content comp settings.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_content_comp_settings( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T* pt_ebml_element = NULL;
    UINT32            ui4_id = 0;
    UINT64            ui8_data_size = 0;

    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_6RD_CONTENT_COMP_SETTINGS].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_content_settings: current element isn't content settings!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    
    /*to do parse*/
    
    if (ui8_data_size < MAX_CONTENT_ENCODING_LEN)
    {
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.b_has_decode_setting = TRUE;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->ui8_comp_settings_length = ui8_data_size;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_curr_content_encoding->pui1_comp_settings_data = (UINT8*)_mkv_parse_string(pt_mkv_info_obj, ui8_data_size, MKV_STRING_CHAR);
    }
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_data_size;
    return MINFOR_OK;
}

/*parse clusters*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_clusters_info
 *
 * Description: This function parse clusters, now just jump.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_clusters_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    getpos_fct        pf_get_pos; 
    EBML_ELE_BASIC_T* pt_ebml_element;
    UINT32            ui4_id;
    UINT8             ui1_head_size;
    UINT64            ui8_data_size;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_1ST_CLUSTERS].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_clusters_info: current element isn't clusters!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui1_head_size = _ebml_get_ele_head_len(pt_ebml_element);
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    if (pt_mkv_info_obj->t_idx_info.ui8_data_offset == 0)
    {
        pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
        pf_get_pos(pt_minfo_obj, &(pt_mkv_info_obj->ui8_cluster_position));
        pt_mkv_info_obj->t_idx_info.ui8_data_offset = pt_mkv_info_obj->ui8_cluster_position - ui1_head_size;
        pt_mkv_info_obj->t_idx_info.ui8_data_sz = ui8_data_size + ui1_head_size;
    }
    else
    {
        pt_mkv_info_obj->t_idx_info.ui8_data_sz += ui8_data_size + ui1_head_size;
    }
    /*current no need to parse clusters info, so just skip*/
    _mkv_skip_data(pt_minfo_obj, ui8_data_size, pt_mkv_info_obj, TRUE);
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_cluster = TRUE;
#endif
    return MINFOR_OK;
}

/*parse cues*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_cues_info
 *
 * Description: This function parse cues, now just jump.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_cues_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    getpos_fct          pf_get_pos; 
    EBML_ELE_BASIC_T*   pt_ebml_element;
    UINT32              ui4_id;
    UINT8               ui1_head_size;
    UINT64              ui8_data_size;

    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
#if IMPROVE_MKV_PARSER_SPEED
    if (_g_parsed_cues)
    {
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        return MINFOR_OK;
    }
#endif
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_1ST_CUES].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_cues_info: current element isn't cues!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    ui1_head_size = _ebml_get_ele_head_len(pt_ebml_element);
    ui8_data_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_get_pos(pt_minfo_obj, &(pt_mkv_info_obj->ui8_cues_position));
    pt_mkv_info_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
    pt_mkv_info_obj->t_idx_info.ui8_idx_offset = pt_mkv_info_obj->ui8_cues_position - ui1_head_size;
    pt_mkv_info_obj->t_idx_info.ui8_idx_sz = ui8_data_size + ui1_head_size;
    pt_mkv_info_obj->t_idx_info.ui8_segment_data_offset = pt_mkv_info_obj->ui8_seg_start_pos;
    /*current no need to parse cues info, so just skip*/
    _mkv_skip_data(pt_minfo_obj, ui8_data_size, pt_mkv_info_obj, TRUE);
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_cues = TRUE;
#endif
    return MINFOR_OK;
}


/*add for font info*/ 
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_attach_file_ele_info
 *
 * Description: This function parse attachment file info.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_attach_file_ele_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT32                  ui4_id;
    UINT64                  ui8_size;
    UINT64                  ui8_font_id;
    INT32                   i4_ret;
    CHAR*                   psz_mkv_string = NULL;
    FONT_INFO_T*            pt_cur_font;
    FONT_INFO_T*            pt_next_font;
    FONT_INFO_T**           ppt_new_font;
    getpos_fct              pf_get_pos; 
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    DBG_ERROR(("ui4_id = %x, MKV_ELEMENT_ATTACH_FILE_DATA is %d, ui4_element_id is %x \r\n",
        ui4_id, 
        MKV_ELEMENT_ATTACH_FILE_DATA, 
        g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_DATA].ui4_element_id));
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_parse_attach_file_ele_info: element size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }

    if(ui4_id != g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_DATA].ui4_element_id)
    {   /*some font file is too large(maybe more than 10M), so do not load font obj.*/
        i4_ret = _mkv_load_element_data(pt_minfo_obj, pt_mkv_info_obj, ui8_size);
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_parse_attach_file_ele_info load data error : %s!\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
    }    

    if(ui4_id == g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_NAME].ui4_element_id)
    {
        psz_mkv_string = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
        x_memcpy((VOID*)g_font_info.ac_file_name, (VOID*)psz_mkv_string, (UINT32)ui8_size);
        *((CHAR*)(g_font_info.ac_file_name) + ui8_size) = 0;  /*'\0'*/
        *((CHAR*)(g_font_info.ac_file_name) + ui8_size+1) = 0;  /*'\0'*/
    }
    else if(ui4_id == g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_MIME_TYPE].ui4_element_id)
    {
        psz_mkv_string = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
        x_memcpy((VOID*)g_font_info.ac_file_mimetype, (VOID*)psz_mkv_string, (UINT32)ui8_size);
        g_font_info.ac_file_mimetype[ui8_size] = 0; /*'\0'*/

        /*font file attachement format sample*/
        /*
                |+ Attachments
                | + Attached
                |  + File name: Font.TTF
                |  + Mime type: application/x-truetype-font
                |  + File data, size: 2484512
                |  + File UID: 577931698
                |  + File description: CN-GB
                */
        if(x_strstr(g_font_info.ac_file_mimetype, "truetype-font")) /*it's ttf font*/
        {
            pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
            if(NULL == pt_cur_font) /*the first font file*/
            {
                ppt_new_font = (FONT_INFO_T**)&(pt_minfo_obj->pv_attach_file);
            }
            else
            {
                while(pt_cur_font->pt_next_font)
                {
                    pt_cur_font = pt_cur_font->pt_next_font;
                }
                ppt_new_font = &pt_cur_font->pt_next_font;
            }
            *ppt_new_font = _mm_util_mem_alloc(sizeof(FONT_INFO_T));
            if(NULL == *ppt_new_font)
            {
                /*free all font nodes */
                pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
                while(pt_cur_font)
                {
                    pt_next_font = pt_cur_font->pt_next_font;
                    if(pt_cur_font->pv_file_data)/*error*/
                    {
                        DBG_ERROR(("MKV : Free use data cause of alloc memory fail! \r\n"));
                    }
                    _mm_util_mem_free(pt_cur_font);
                    pt_cur_font = pt_next_font;
                }
                pt_minfo_obj->pv_attach_file = NULL;
                
                return MINFOR_INTERNAL_ERR;
            }
            x_memset(*ppt_new_font, 0, sizeof(FONT_INFO_T));
        }
    }
    else if(ui4_id == g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_DATA].ui4_element_id)
    {
        DBG_ERROR(("MKV : Enter get MKV_ELEMENT_ATTACH_FILE_DATA \r\n"));
        pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
        if(pt_cur_font)
        {
            while(pt_cur_font->pt_next_font)
            {
                pt_cur_font = pt_cur_font->pt_next_font;
            }

            if(0 == pt_cur_font->ui8_file_offset) /*font offset not used*/
            {        
                pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
                pf_get_pos(pt_minfo_obj, &(pt_cur_font->ui8_file_offset));                
                
                pt_cur_font->ui4_font_file_size = (UINT32)ui8_size;
                /*copy file name*/
                x_memcpy(pt_cur_font->ac_file_name, 
                         g_font_info.ac_file_name,
                         sizeof(g_font_info.ac_file_name));
                /*copy file mime type*/
                x_memcpy(pt_cur_font->ac_file_mimetype, 
                         g_font_info.ac_file_mimetype,
                         sizeof(g_font_info.ac_file_mimetype));
            }
        }   

        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, TRUE);
    }
    else if(ui4_id == g_at_mkv_element_op[MKV_ELEMENT_ATTACH_FILE_UID].ui4_element_id)
    {
        i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_font_id);        
        if (i4_ret != EBMLR_OK)
        {
            /*free all font nodes */
            pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
            while(pt_cur_font)
            {
                pt_next_font = pt_cur_font->pt_next_font;
                if(pt_cur_font->pv_file_data)/*error*/
                {
                    DBG_ERROR(("MKV : Free use data cause of alloc memory fail! \r\n"));
                }
                _mm_util_mem_free(pt_cur_font);
                pt_cur_font = pt_next_font;
            }
            pt_minfo_obj->pv_attach_file = NULL;
            
            return MINFOR_INTERNAL_ERR;
        }
        
        pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
        if(pt_cur_font)
        {
            while(pt_cur_font->pt_next_font)
            {
                pt_cur_font = pt_cur_font->pt_next_font;
            }
            if(0 == pt_cur_font->ui8_font_id) /*font id not used*/
            {
                pt_cur_font->ui8_font_id = ui8_font_id;
            }
        }
    }
    else 
    {
        /*free all font nodes */
        pt_cur_font = (FONT_INFO_T*)pt_minfo_obj->pv_attach_file;
        while(pt_cur_font)
        {
            pt_next_font = pt_cur_font->pt_next_font;
            if(pt_cur_font->pv_file_data)/*error*/
            {
                DBG_ERROR(("MKV : Free use data cause of alloc memory fail! \r\n"));
            }
            _mm_util_mem_free(pt_cur_font);
            pt_cur_font = pt_next_font;
        }
        pt_minfo_obj->pv_attach_file = NULL;
        
        DBG_ERROR(("_mkv_parse_attach_file_ele_info: current element not recognized!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*free psz_mkv_string*/
    if(psz_mkv_string)
    {
        _ebml_string_free(&psz_mkv_string);
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_attach_file_info
 *
 * Description: This function parse attachment file info.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_attach_file_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT64                  ui8_size;
    UINT64                  ui8_parse_pos;
    UINT64                  ui8_sub_element_size;
    INT32                   i4_ret;
    mkv_parse_element_func  pf_parse_data;
    CHAR*                   psz_desc;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    psz_desc = _mkv_get_element_desc(pt_ebml_element);
    psz_desc = psz_desc;
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
    ui8_parse_pos = 0;
    if (!ui8_size)
    {
        DBG_INFO(("_mkv_feeder_parse: element size is 0, no need parse!\r\n"));
        return MINFOR_OK;
    }    
        
    while (ui8_parse_pos < ui8_size)
    {
        if(pt_minfo_obj->b_abort_flag == TRUE)
        {
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        if (i4_ret == MINFOR_FILE_ERR)
        {
            /*file maybe broken*/
            DBG_INFO(("_mkv_feeder_parse: file maybe broken, prev element is %s!\r\n", psz_desc));
            _mkv_skip_data(pt_minfo_obj, (ui8_size - ui8_parse_pos), pt_mkv_info_obj, TRUE);
            break;
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_feeder_parse parse head error : prev element is %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        psz_desc = _mkv_get_element_desc(pt_ebml_element);
        ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
        ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
        pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
        if (pf_parse_data)
        {
            i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
        }
        else
        {
            i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, TRUE);
        }
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("_mkv_feeder_parse parse data error : %s!\r\n", psz_desc));
            return MINFOR_INTERNAL_ERR;
        }
        ui8_parse_pos += ui8_sub_element_size;
    }
    return MINFOR_OK;
}

/*parse attachments*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_attachment_info
 *
 * Description: This function parse attachment info, now just jump.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_attachment_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    EBML_ELE_BASIC_T*       pt_ebml_element;
    UINT64                  ui8_size;
    UINT64                  ui8_parse_pos;
    UINT64                  ui8_sub_element_size;
    INT32                   i4_ret;
    mkv_parse_element_func  pf_parse_data;
    CHAR*                   psz_desc; 
    
     /*add for font info*/ 
    if(pt_minfo_obj->pv_attach_file) /*already parsed*/
    {       
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        /*current no need to parse attachment info, so just skip*/
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
    }
    else
    {  
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        psz_desc = _mkv_get_element_desc(pt_ebml_element);
        psz_desc = psz_desc;
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
        ui8_parse_pos = 0;
        if (!ui8_size)
        {
            DBG_INFO(("_mkv_feeder_parse: element size is 0, no need parse!\r\n"));
            return MINFOR_OK;
        }
        
        while (ui8_parse_pos < ui8_size)
        {
            if(pt_minfo_obj->b_abort_flag == TRUE)
            {
                return MINFOR_INTERNAL_ERR;
            }
            x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
            i4_ret = _mkv_parse_element_head(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
            if (i4_ret == MINFOR_FILE_ERR)
            {
                /*file maybe broken*/
                DBG_INFO(("_mkv_feeder_parse: file maybe broken, prev element is %s!\r\n", psz_desc));
                _mkv_skip_data(pt_minfo_obj, (ui8_size - ui8_parse_pos), pt_mkv_info_obj, TRUE);
                break;
            }
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR(("_mkv_feeder_parse parse head error : prev element is %s!\r\n", psz_desc));
                return MINFOR_INTERNAL_ERR;
            }
            psz_desc = _mkv_get_element_desc(pt_ebml_element);
            ui8_parse_pos += _ebml_get_ele_head_len(pt_ebml_element);
            ui8_sub_element_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, TRUE);
            pf_parse_data = _mkv_get_element_parse_func(pt_ebml_element);
            if (pf_parse_data)
            {
                i4_ret = pf_parse_data(pt_minfo_obj, pt_mkv_info_obj);
            }
            else
            {
                i4_ret = _mkv_skip_data(pt_minfo_obj, ui8_sub_element_size, pt_mkv_info_obj, TRUE);
            }
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR(("_mkv_feeder_parse parse data error : %s!\r\n", psz_desc));
                return MINFOR_INTERNAL_ERR;
            }
            ui8_parse_pos += ui8_sub_element_size;
        }      
    }
    
    return MINFOR_OK;
}

/*parse chapters*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_chapters_info
 *
 * Description: This function parse chapters, now just jump.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_chapters_info( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
    INT32 i4_ret;
#if IMPROVE_MKV_PARSER_SPEED
    EBML_ELE_BASIC_T* pt_ebml_element;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    if (_g_parsed_chapters)
    {
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        return MINFOR_OK;
    }
#endif
    i4_ret = _mkv_only_parse_file(pt_minfo_obj, pt_mkv_info_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("_mkv_parse_chapters_info parse error!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_chapters = TRUE;
#endif
#else
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    /*current no need to parse attachment info, so just skip*/
    _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
#endif
    
    return MINFOR_OK;
}

/*parse tags*/
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_tags
 *
 * Description: This function parse tags element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_tags( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32 i4_ret;

#if IMPROVE_MKV_PARSER_SPEED
    EBML_ELE_BASIC_T* pt_ebml_element;
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    if (_g_parsed_tags)
    {
        _mkv_jump_element_data(pt_minfo_obj, pt_ebml_element, pt_mkv_info_obj, TRUE);
        return MINFOR_OK;
    }
#endif    
    i4_ret = _mkv_only_parse_file(pt_minfo_obj, pt_mkv_info_obj);
#if IMPROVE_MKV_PARSER_SPEED
    _g_parsed_tags = TRUE;
#endif
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_tag_entry
 *
 * Description: This function parse tag entry element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
 INT32 _mkv_parse_tag_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32 i4_ret;
    
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_target_track_uid
 *
 * Description: This function parse target track uid element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *             pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_target_track_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    /*parse seek position*/
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_TARGET_TRACK_UID].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_target_track_uid: current element isn't target track uid!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_simple_tag
 *
 * Description: This function parse simple tag element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_simple_tag( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32 i4_ret;
    CHAR* ps_tagname;
    BOOL  b_free = TRUE;
    
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    if (i4_ret == MINFOR_OK)
    {
        ps_tagname = pt_mkv_info_obj->t_simple_tag.ps_tagname;     
        if (ps_tagname != NULL)
        {
            if (x_memcmp((VOID*)"COPYRIGHT", ps_tagname, 9) == 0)
            {
                if (pt_mkv_info_obj->t_meta_info.pws_copyright == NULL)
                {
                    pt_mkv_info_obj->t_meta_info.pws_copyright = pt_mkv_info_obj->t_simple_tag.pws_tagstring;
                    b_free = FALSE;
                }
            }
            else if (x_memcmp((VOID*)"DIRECTOR", ps_tagname, 8) == 0)
            {
                if (pt_mkv_info_obj->t_meta_info.pws_director == NULL)
                {
                    pt_mkv_info_obj->t_meta_info.pws_director = pt_mkv_info_obj->t_simple_tag.pws_tagstring;
                    b_free = FALSE;
                }
            }
            else if (x_memcmp((VOID*)"GENRE", ps_tagname, 5) == 0)
            {
                if (pt_mkv_info_obj->t_meta_info.pws_genre == NULL)
                {
                    pt_mkv_info_obj->t_meta_info.pws_genre = pt_mkv_info_obj->t_simple_tag.pws_tagstring;
                    b_free = FALSE;
                }
            }
            else if (x_memcmp((VOID*)"LAW_RATING", ps_tagname, 10) == 0)            
            {                
                if (pt_mkv_info_obj->t_meta_info.pws_rating == NULL)                
                {                    
                    pt_mkv_info_obj->t_meta_info.pws_rating = pt_mkv_info_obj->t_simple_tag.pws_tagstring;                    
                    b_free = FALSE;                
                }            
            }
            _mm_util_mem_free(ps_tagname);
            pt_mkv_info_obj->t_simple_tag.ps_tagname = NULL;
        }
        if (pt_mkv_info_obj->t_simple_tag.pws_tagstring != NULL)
        {
            if (b_free)
            {
                _mm_util_mem_free(pt_mkv_info_obj->t_simple_tag.pws_tagstring);
            }
            pt_mkv_info_obj->t_simple_tag.pws_tagstring = NULL;
        }
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_target_tag_name
 *
 * Description: This function parse target tag name element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_tag_name( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_TAG_NAME].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_target_tag_name: current element isn't target tag name!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_simple_tag.ps_tagname = (CHAR*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_CHAR);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_parse_target_tag_string
 *
 * Description: This function parse target tag string element.
 *
 * Inputs:  pt_minfo_obj      specify the media info object
 *          pt_mkv_info_obj   specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_parse_tag_string( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32              ui4_id;
    UINT64              ui8_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_TAG_STRING].ui4_element_id)
    {
        DBG_ERROR(("_mkv_parse_target_tag_string: current element isn't target tag string!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_simple_tag.pws_tagstring = (UTF16_T*)_mkv_parse_string(pt_mkv_info_obj, ui8_size, MKV_STRING_W_CHAR);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_audio_info
 *
 * Description: This function set audio stream information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          pt_aud_strm          specify current parse audio stream
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_audio_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_AUDIO_STRM_INFO_T* pt_aud_strm)
{
    INT32                        i4_ret;
    MINFO_MKV_AUDIO_STRM_INFO_T* pt_exist_strm;
    MINFO_MKV_AUDIO_STRM_INFO_T* pt_aud_strm_list;
    BOOL                         b_exist = FALSE;
    UINT64                       ui8_remaind;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding = NULL;
    UINT32                          ui4_cnt = 0;

    i4_ret = MINFOR_OK;
    pt_exist_strm = _mkv_search_track_node(pt_mkv_info_obj, MKV_STRM_AUDIO_TYPE);
    if (pt_exist_strm == NULL)
    {
        /*find last node*/
        pt_aud_strm_list = pt_mkv_info_obj->t_all_strm_info.pt_audio_strm_info_head;
        if (pt_aud_strm_list == NULL)
        {
            pt_mkv_info_obj->t_all_strm_info.pt_audio_strm_info_head = pt_aud_strm;
        }
        else
        {
            while (pt_aud_strm_list->pt_next_audio_strm != NULL)
            {
                pt_aud_strm_list = pt_aud_strm_list->pt_next_audio_strm;
            }
            pt_aud_strm_list->pt_next_audio_strm = pt_aud_strm;
        }
    }
    else
    {
        /*this is an existed video track, so free the new allocated node*/
        b_exist = TRUE;
        if (pt_aud_strm->t_audio_info.i2_channels)
        {
            pt_exist_strm->t_audio_info.i2_channels = pt_aud_strm->t_audio_info.i2_channels;
        }
        if (pt_aud_strm->t_audio_info.i4_samples_per_sec)
        {
            pt_exist_strm->t_audio_info.i4_samples_per_sec= pt_aud_strm->t_audio_info.i4_samples_per_sec;
        }
        _mm_util_mem_free(pt_aud_strm);
        pt_aud_strm = pt_exist_strm;
        pt_mkv_info_obj->t_all_strm_info.ui1_audio_strm_num -= 1;
        pt_mkv_info_obj->ui1_cur_strm_num--;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = (VOID*)pt_exist_strm;
    }
    /*set generic information*/
    if (!b_exist)
    {
        pt_aud_strm->ui1_stream_idx = pt_mkv_info_obj->t_all_strm_info.ui1_audio_strm_num;
        pt_aud_strm->ui4_stream_number = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_track_num;
        pt_aud_strm->ui8_track_uid = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_uid;
    }
    if (!(pt_aud_strm->t_audio_info.b_default))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_default)
        {
            pt_aud_strm->t_audio_info.b_default = TRUE;
        }
    }
    if (!(pt_aud_strm->t_audio_info.s_lang[0]))
    {
        x_memcpy(pt_aud_strm->t_audio_info.s_lang, pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ac_language, 3);
    }
    pt_aud_strm->t_audio_info.b_divx_plus = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.fg_divx_plus;
     /*audio rate and scale*/
    if (!(pt_aud_strm->t_audio_info.ui4_rate))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration != 0)
        {
            pt_aud_strm->t_audio_info.ui4_rate = (UINT32)
                                    _mm_div64((UINT64)1000 * 1000000 * 1000, 
                                               pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                               &ui8_remaind);
            ui8_remaind = _mm_div64(ui8_remaind*10, 
                                               pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                               NULL);
            if (ui8_remaind >= 5)
            {
                pt_aud_strm->t_audio_info.ui4_rate += 1;
            }
        }
        pt_aud_strm->t_audio_info.ui4_scale = 1000;
    }
    /*audio timecode scale*/
    if (!(pt_aud_strm->t_audio_info.t_track_timecode_scale.ui4_float_numerator))
    {
        pt_aud_strm->t_audio_info.t_track_timecode_scale.ui4_float_numerator = 
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_numerator;
        pt_aud_strm->t_audio_info.t_track_timecode_scale.ui4_float_denominator =
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_denominator;
    }
    /*if this track has content encodings, just set unkown*/
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.b_has_decode_setting)
    {
        for(ui4_cnt = 0 ; ui4_cnt < pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_encoding_num ; ui4_cnt ++)
        {
            if (!pt_content_encoding)
            {
                pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                if (pt_content_encoding == NULL)
                {
                    DBG_INFO(("\n-----------set audio info memory1 not enough-------------\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }       
                pt_aud_strm->t_audio_info.pt_content_encoding = pt_content_encoding;
                pt_aud_strm->t_audio_info.ui4_encoding_num = 1;
            }
            else if (!pt_content_encoding->pt_next)
            {
                pt_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                if (pt_content_encoding->pt_next == NULL)
                {
                    DBG_INFO(("\n-----------set audio info memory2 not enough-------------\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }  
                pt_content_encoding = pt_content_encoding->pt_next;
                pt_aud_strm->t_audio_info.ui4_encoding_num ++;
            }
            else 
            {
                return MINFOR_INTERNAL_ERR;
            }
            if (pt_content_encoding != NULL)
            {
            x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
            pt_aud_strm->t_audio_info.b_has_decode_setting = TRUE;
            pt_content_encoding->ui8_order = 
                            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_order;
            pt_content_encoding->ui8_scope = 
                            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_scope;
            pt_content_encoding->ui8_type = 
                            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_type;
            pt_content_encoding->ui8_compress_algo = 
                            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_compress_algo;
            pt_content_encoding->ui8_comp_settings_length = 
                            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_comp_settings_length;
            pt_content_encoding->pui1_comp_settings_data = 
                            (UINT8*)x_mem_alloc((UINT32)pt_content_encoding->ui8_comp_settings_length);
                if (pt_content_encoding->pui1_comp_settings_data != NULL)
                {
            x_memcpy((VOID*)(pt_content_encoding->pui1_comp_settings_data),
                             (VOID*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pui1_comp_settings_data, 
                             (UINT32)pt_content_encoding->ui8_comp_settings_length);
        }
    }
        }
    }

    /* David Yeh Required this */
    if (pt_aud_strm->t_audio_info.b_has_decode_setting && pt_content_encoding != NULL)
    {
        if (pt_content_encoding->ui8_type == 1 ||
            pt_content_encoding->ui8_compress_algo != 3 ||
            pt_content_encoding->ui8_scope != 1)
        {
            pt_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
            pt_aud_strm->t_audio_info.b_has_decode_setting = FALSE;
            return MINFOR_OK;
        }
    }
    /*add for codec private*/
    if(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.b_has_codec_private)
    {
        pt_aud_strm->t_audio_info.ui8_codec_private_len = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_codec_private_len;
        if(pt_aud_strm->t_audio_info.ui8_codec_private_len != 0)
        {
            pt_aud_strm->t_audio_info.b_has_codec_private   = TRUE;
            pt_aud_strm->t_audio_info.pui1_codec_private    = (UINT8*)x_mem_alloc((UINT32)pt_aud_strm->t_audio_info.ui8_codec_private_len);
            if(pt_aud_strm->t_audio_info.pui1_codec_private != NULL)
            {
                x_memcpy((VOID*)(pt_aud_strm->t_audio_info.pui1_codec_private),
                                             (VOID*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private, 
                                             (UINT32)pt_aud_strm->t_audio_info.ui8_codec_private_len);
            }
        }
    }
    /*audio codec*/
    if (!(pt_aud_strm->t_audio_info.e_enc))
    {
        i4_ret = _mkv_set_audio_codec(pt_mkv_info_obj);
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_video_info
 *
 * Description: This function set video stream information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          pt_vid_strm          specify current parse video stream
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_video_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_VIDEO_STRM_INFO_T* pt_vid_strm)
{
    INT32                        i4_ret;
    MINFO_MKV_VIDEO_STRM_INFO_T* pt_exist_strm;
    MINFO_MKV_VIDEO_STRM_INFO_T* pt_vid_strm_list;
    BOOL                         b_exist = FALSE;
    UINT64                       ui8_remaind;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding = NULL;
    UINT32                          ui4_cnt = 0;

    i4_ret = MINFOR_OK;
    pt_exist_strm = _mkv_search_track_node(pt_mkv_info_obj, MKV_STRM_VIDEO_TYPE);
    if (pt_exist_strm == NULL)
    {
        /*this is new video track, so add to video strm list*/
        /*find last node*/
        pt_vid_strm_list = pt_mkv_info_obj->t_all_strm_info.pt_video_strm_info_head;
        if (pt_vid_strm_list == NULL)
        {
            pt_mkv_info_obj->t_all_strm_info.pt_video_strm_info_head = pt_vid_strm;
        }
        else
        {
            while (pt_vid_strm_list->pt_next_video_strm != NULL)
            {
                pt_vid_strm_list = pt_vid_strm_list->pt_next_video_strm;
            }
            pt_vid_strm_list->pt_next_video_strm = pt_vid_strm;
        }
    }
    else
    {
        /*this is an existed video track, so free the new allocated node*/
        b_exist = TRUE;
        if (pt_vid_strm->t_video_info.i4_width)
        {
            pt_exist_strm->t_video_info.i4_width = pt_vid_strm->t_video_info.i4_width;
        }
        if (pt_vid_strm->t_video_info.i4_height)
        {
            pt_exist_strm->t_video_info.i4_height = pt_vid_strm->t_video_info.i4_height;
        }
        _mm_util_mem_free(pt_vid_strm);
        pt_vid_strm = pt_exist_strm;
        pt_mkv_info_obj->t_all_strm_info.ui1_video_strm_num -= 1;
        pt_mkv_info_obj->ui1_cur_strm_num--;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = (VOID*)pt_exist_strm;
    }
    /*set general information*/
    if (!b_exist)
    {
        pt_vid_strm->ui1_stream_idx = pt_mkv_info_obj->t_all_strm_info.ui1_video_strm_num;
        pt_vid_strm->ui4_stream_number = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_track_num;
        pt_vid_strm->ui8_track_uid = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_uid;
        pt_vid_strm->fg_enh_track_flag = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.fg_enh_track_flag;
        pt_vid_strm->u.t_mt_info = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.u.t_mt_info;
    }
    if (!(pt_vid_strm->t_video_info.b_default))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_default)
        {
            pt_vid_strm->t_video_info.b_default = TRUE;
        }
    }
    pt_vid_strm->t_video_info.b_divx_plus = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.fg_divx_plus;
    /*video rate and scale*/
    if (!(pt_vid_strm->t_video_info.ui4_rate))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration != 0)
        {
            pt_vid_strm->t_video_info.ui4_rate = (UINT32)
                                    _mm_div64((UINT64)1000*1000000*1000, 
                                              pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                              &ui8_remaind);
            ui8_remaind = _mm_div64(ui8_remaind*10, 
                                              pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                              NULL);
            if (ui8_remaind >= 5)
            {
                pt_vid_strm->t_video_info.ui4_rate += 1;
            }
        }
        pt_vid_strm->t_video_info.ui4_scale = 1000;
    }
    /*video timecode scale*/
    if (!(pt_vid_strm->t_video_info.t_track_timecode_scale.ui4_float_numerator))
    {
        pt_vid_strm->t_video_info.t_track_timecode_scale.ui4_float_numerator = 
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_numerator;
        pt_vid_strm->t_video_info.t_track_timecode_scale.ui4_float_denominator = 
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_denominator;
    }
    /*if this track has content encodings, just set unkown*/
    
    if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.b_has_decode_setting)
    {
        for(ui4_cnt = 0 ; ui4_cnt < pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_encoding_num ; ui4_cnt ++)
        {
            if (!pt_content_encoding)
            {
                pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                if (pt_content_encoding == NULL)
                {
                    DBG_INFO(("\n-----------set video info memory1 not enough-------------\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }
                pt_vid_strm->t_video_info.pt_content_encoding = pt_content_encoding;
                pt_vid_strm->t_video_info.ui4_encoding_num = 1;
            }
            else if (!pt_content_encoding->pt_next)
            {
                pt_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                if (pt_content_encoding->pt_next == NULL)
                {
                    DBG_INFO(("\n-----------set video info memory2 not enough-------------\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }
                pt_content_encoding = pt_content_encoding->pt_next;
                pt_vid_strm->t_video_info.ui4_encoding_num ++;
            }
            else 
            {
                return MINFOR_INTERNAL_ERR;
            }
            if (pt_content_encoding != NULL)
            {
                x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                pt_vid_strm->t_video_info.b_has_decode_setting = TRUE;
                pt_content_encoding->ui8_order = 
                                pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_order;
                pt_content_encoding->ui8_scope = 
                                pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_scope;
                pt_content_encoding->ui8_type = 
                                pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_type;
                pt_content_encoding->ui8_compress_algo = 
                                pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_compress_algo;
                pt_content_encoding->ui8_comp_settings_length = 
                                pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->ui8_comp_settings_length;
                pt_content_encoding->pui1_comp_settings_data = 
                                (UINT8*)x_mem_alloc((UINT32)pt_content_encoding->ui8_comp_settings_length);
                if (pt_content_encoding->pui1_comp_settings_data != NULL)
                {
                    x_memcpy((VOID*)(pt_content_encoding->pui1_comp_settings_data),
                                     (VOID*)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pt_content_encoding->pui1_comp_settings_data, 
                                     (UINT32)pt_content_encoding->ui8_comp_settings_length);
                }
            }
        }
    }

    /* David Yeh Required this */
    if (pt_vid_strm->t_video_info.b_has_decode_setting && pt_content_encoding != NULL)
    {
        
        if (pt_content_encoding->ui8_type == 1 ||
            pt_content_encoding->ui8_compress_algo != 3 ||
            pt_content_encoding->ui8_scope != 1)
        { 
            pt_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
                    pt_vid_strm->t_video_info.b_has_decode_setting = FALSE;
            return MINFOR_OK;
        }
    }
    /*video codec*/
    if (!(pt_vid_strm->t_video_info.e_enc))
    {
        i4_ret = _mkv_set_video_codec(pt_mkv_info_obj);
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_sp_info
 *
 * Description: This function set subtitle stream information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          pt_sp_strm           specify current parse subtitle stream
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_sp_content_encoding_info(MINFO_MKV_SUBTITLE_STM_ATRBT_T* pt_mkv_sp_strm_attr, UINT64 ui8_offset, UINT64 ui8_end)
{
    INT32 i4_ret;
    UINT32 ui4_id;
    UINT64 ui8_sz;
    UINT8  ui1_head_len;
    EBML_ELE_BASIC_T t_element;  
    
    i4_ret = MINFOR_OK;
    while(ui8_offset < ui8_end)
    {
        ui1_head_len = MKV_ELEMENT_MAX_HEAD_LEN;
        if((UINT64)ui1_head_len > (UINT64)pt_mkv_sp_strm_attr->ui4_content_encodings_size - ui8_offset)
        {
            ui1_head_len = (UINT8)((UINT64)pt_mkv_sp_strm_attr->ui4_content_encodings_size - ui8_offset);
        }
        i4_ret = _ebml_buf_ele_base((VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset), 
                                     ui1_head_len, 0, &t_element);
        if(MINFOR_OK != i4_ret)
        {
            break;
        }
        
        ui4_id = _ebml_get_ele_id(&t_element);
        ui8_sz = _ebml_get_ele_size(&t_element);
        ui1_head_len = _ebml_get_ele_head_len(&t_element);
        ui8_offset += ui1_head_len;
        if(ui8_offset + ui8_sz <= ui8_end)
        {
            switch(ui4_id)
            {
                case 0x5031:
                    if(EBMLR_OK != (i4_ret = _ebml_buf_uint((VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset),
                                                            (UINT32)(ui8_end - ui8_offset), (UINT32)ui8_sz, 
                                                            &pt_mkv_sp_strm_attr->pt_curr_content_encoding->ui8_order)))
                    {
                        return i4_ret;
                    }
                    break;
                case 0x5032:
                    if(EBMLR_OK != (i4_ret = _ebml_buf_uint((VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset),
                                                            (UINT32)(ui8_end - ui8_offset), (UINT32)ui8_sz, 
                                                            &pt_mkv_sp_strm_attr->pt_curr_content_encoding->ui8_scope)))
                    {
                        return i4_ret;
                    }
                    break;
                case 0x5033:
                    if(EBMLR_OK != (i4_ret = _ebml_buf_uint((VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset),
                                                            (UINT32)(ui8_end - ui8_offset), (UINT32)ui8_sz, 
                                                            &pt_mkv_sp_strm_attr->pt_curr_content_encoding->ui8_type)))
                    {
                        return i4_ret;
                    }
                    break;
                case 0x4254:
                    if(EBMLR_OK != (i4_ret = _ebml_buf_uint((VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset),
                                                            (UINT32)(ui8_end - ui8_offset), (UINT32)ui8_sz, 
                                                            &pt_mkv_sp_strm_attr->pt_curr_content_encoding->ui8_compress_algo)))
                    {
                        return i4_ret;
                    }
                    break;
                case 0x4255:
                    pt_mkv_sp_strm_attr->pt_curr_content_encoding->ui8_comp_settings_length = ui8_sz;
                    pt_mkv_sp_strm_attr->pt_curr_content_encoding->pui1_comp_settings_data = (UINT8*)x_mem_alloc((UINT32)ui8_sz);
                    x_memcpy((VOID*)(pt_mkv_sp_strm_attr->pt_curr_content_encoding->pui1_comp_settings_data),
                             (VOID*)(pt_mkv_sp_strm_attr->pui1_content_encodings + ui8_offset), 
                             (UINT32)ui8_sz);
                    break;
                case 0x5034:
                    _mkv_set_sp_content_encoding_info(pt_mkv_sp_strm_attr, ui8_offset, ui8_offset + ui8_sz);
                    break;
                case 0x6240:
                    if(!pt_mkv_sp_strm_attr->pt_content_encoding)
                    {
                        pt_mkv_sp_strm_attr->pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                        if (pt_mkv_sp_strm_attr->pt_content_encoding == NULL)
                        {
                            i4_ret = MINFOR_OUT_OF_MEMORY;
                            break; 
                        }
                        pt_mkv_sp_strm_attr->pt_curr_content_encoding = pt_mkv_sp_strm_attr->pt_content_encoding;
                        pt_mkv_sp_strm_attr->ui4_encoding_num = 1;
                    }
                    else if(pt_mkv_sp_strm_attr->pt_curr_content_encoding)
                    {
                        pt_mkv_sp_strm_attr->pt_curr_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                        if (pt_mkv_sp_strm_attr->pt_curr_content_encoding->pt_next == NULL)
                        {
                            i4_ret = MINFOR_OUT_OF_MEMORY;
                            break;
                        }
                        pt_mkv_sp_strm_attr->pt_curr_content_encoding = pt_mkv_sp_strm_attr->pt_curr_content_encoding->pt_next;
                        pt_mkv_sp_strm_attr->ui4_encoding_num++;
                    }
                    else
                    {
                        return MINFOR_INTERNAL_ERR;
                    }
                    x_memset(pt_mkv_sp_strm_attr->pt_curr_content_encoding, 0, sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                    _mkv_set_sp_content_encoding_info(pt_mkv_sp_strm_attr, ui8_offset, ui8_offset + ui8_sz);
                    break;
                case 0x5035:
                    break;
                default:
                    break;
            }
        }

        ui8_offset += ui8_sz;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_sp_info
 *
 * Description: This function set subtitle stream information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          pt_sp_strm           specify current parse subtitle stream
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_sp_info(MINFO_MKV_INF_T* pt_mkv_info_obj, MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_sp_strm)
{
    INT32                           i4_ret;
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_exist_strm;
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_sp_strm_list;
    BOOL                            b_exist = FALSE;
    UINT64                          ui8_remaind;

    i4_ret = MINFOR_OK;
    pt_exist_strm = _mkv_search_track_node(pt_mkv_info_obj, MKV_STRM_SUBTILE_TYPE);
    if (pt_exist_strm == NULL)
    {
        /*this is new subtitle track, so add to subtitle strm list*/
        /*find last node*/
        pt_sp_strm_list = pt_mkv_info_obj->t_all_strm_info.pt_subtitle_strm_info_head;
        if (pt_sp_strm_list == NULL)
        {
            pt_mkv_info_obj->t_all_strm_info.pt_subtitle_strm_info_head = pt_sp_strm;
        }
        else
        {
            while (pt_sp_strm_list->pt_next_subtitle_strm != NULL)
            {
                pt_sp_strm_list = pt_sp_strm_list->pt_next_subtitle_strm;
            }
            pt_sp_strm_list->pt_next_subtitle_strm = pt_sp_strm;
        }
    }
    else
    {
        /*this is an existed subtitle track, so free the new allocated node*/
        b_exist = TRUE;
        _mm_util_mem_free(pt_sp_strm);
        pt_sp_strm = pt_exist_strm;
        pt_mkv_info_obj->t_all_strm_info.ui1_subtitle_strm_num -= 1;
        pt_mkv_info_obj->ui1_cur_strm_num--;
        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm = (VOID*)pt_exist_strm;
    }
    /*set general information*/
    if (!b_exist)
    {
        pt_sp_strm->ui1_stream_idx = pt_mkv_info_obj->t_all_strm_info.ui1_subtitle_strm_num;
        pt_sp_strm->ui4_stream_number = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui1_track_num;
        pt_sp_strm->ui8_track_uid = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_uid;        
    }
    if (!(pt_sp_strm->t_subtitle_info.b_default))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_default)
        {
            pt_sp_strm->t_subtitle_info.b_default = TRUE;
        }
    }
    if (!(pt_sp_strm->t_subtitle_info.s_lang[0]))
    {
        x_memcpy(pt_sp_strm->t_subtitle_info.s_lang, pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ac_language, 3);
    }
    /*subtitle rate and scale*/
    if (!(pt_sp_strm->t_subtitle_info.ui4_rate))
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration != 0)
        {
            pt_sp_strm->t_subtitle_info.ui4_rate = (UINT32)
                                    _mm_div64((UINT64)1000*1000000*1000, 
                                              pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                              &ui8_remaind);
            ui8_remaind = _mm_div64(ui8_remaind*10, 
                                              pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_def_duration,
                                              NULL);
            if (ui8_remaind >= 5)
            {
                pt_sp_strm->t_subtitle_info.ui4_rate += 1;
            }
        }
        pt_sp_strm->t_subtitle_info.ui4_scale = 1000;
    }
    /*subtitle timecode scale*/
    if (!(pt_sp_strm->t_subtitle_info.t_track_timecode_scale.ui4_float_numerator))
    {
        pt_sp_strm->t_subtitle_info.t_track_timecode_scale.ui4_float_numerator = 
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_numerator;
        pt_sp_strm->t_subtitle_info.t_track_timecode_scale.ui4_float_denominator = 
                        pt_mkv_info_obj->t_all_strm_info.t_strm_properties.t_track_tm_scale.ui4_float_denominator;
    }
    /*subtitle content encodings*/
    if (pt_sp_strm->t_subtitle_info.pui1_content_encodings == NULL)
    {
        if (pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings != NULL)
        {
            /*if this track has content encodings, set content encodings*/
            pt_sp_strm->t_subtitle_info.pui1_content_encodings = 
                _mm_util_mem_alloc(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_content_encodings_size);
            if (pt_sp_strm->t_subtitle_info.pui1_content_encodings == NULL)
            {
                DBG_ERROR(("_mkv_set_sp_info : malloc error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            x_memcpy((VOID *)pt_sp_strm->t_subtitle_info.pui1_content_encodings,
                     (VOID *)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_content_encodings,
                      pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_content_encodings_size);
            pt_sp_strm->t_subtitle_info.ui4_content_encodings_size = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui4_content_encodings_size;
            _mkv_set_sp_content_encoding_info(&pt_sp_strm->t_subtitle_info, 0, pt_sp_strm->t_subtitle_info.ui4_content_encodings_size);
        }
    }
    /*subtitle attachment link*/
    /*add for font info*/
    if(0 == pt_sp_strm->t_subtitle_info.ui8_attach_link_id)
    {
        pt_sp_strm->t_subtitle_info.ui8_attach_link_id = 
            pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_attach_link_id;
    }
    /*subtitle codec*/
    if (!(pt_sp_strm->t_subtitle_info.e_subtitle_type))
    {
        i4_ret = _mkv_set_sp_codec(pt_mkv_info_obj);
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_search_track_node
 *
 * Description: This function use the track uid to find out the track node from track list.
 *                   (for the situation that information about one track be spread over different track entrys)
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          e_strm_type          specify the stream type
 *
 * Returns: VOID*    The pointer points to the track node.
 ----------------------------------------------------------------------------*/
VOID* _mkv_search_track_node(MINFO_MKV_INF_T* pt_mkv_info_obj, MKV_STREAM_TYPE_E e_strm_type)
{
    VOID*                            pv_node;
    UINT64                           ui8_cur_track_uid;
    MINFO_MKV_AUDIO_STRM_INFO_T*     pt_aud_node;
    MINFO_MKV_VIDEO_STRM_INFO_T*     pt_vid_node;
    MINFO_MKV_SUBTITLE_STRM_INFO_T*  pt_sp_node;
    
    pv_node = NULL;
    ui8_cur_track_uid = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_track_uid;
    if (!ui8_cur_track_uid)
    {
        DBG_INFO(("_mkv_search_track_node: track uid hasn't been parsed!\r\n"));
        return NULL;
    }
    switch (e_strm_type)
    {
    case MKV_STRM_VIDEO_TYPE:
        pt_vid_node = pt_mkv_info_obj->t_all_strm_info.pt_video_strm_info_head;
        while (pt_vid_node)
        {
            if (pt_vid_node->ui8_track_uid == ui8_cur_track_uid)
            {
                pv_node = (VOID*)pt_vid_node;
                break;
            }
            pt_vid_node = pt_vid_node->pt_next_video_strm;
        }
        break;
    case MKV_STRM_AUDIO_TYPE:
        pt_aud_node = pt_mkv_info_obj->t_all_strm_info.pt_audio_strm_info_head;
        while (pt_aud_node)
        {
            if (pt_aud_node->ui8_track_uid == ui8_cur_track_uid)
            {
                pv_node = (VOID*)pt_aud_node;
                break;
            }
            pt_aud_node = pt_aud_node->pt_next_audio_strm;
        }
        break;
    case MKV_STRM_SUBTILE_TYPE:
        pt_sp_node = pt_mkv_info_obj->t_all_strm_info.pt_subtitle_strm_info_head;
        while (pt_sp_node)
        {
            if (pt_sp_node->ui8_track_uid == ui8_cur_track_uid)
            {
                pv_node = (VOID*)pt_sp_node;
                break;
            }
            pt_sp_node = pt_sp_node->pt_next_subtitle_strm;
        }
    default:
        break;
    }
    return pv_node;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_get_codec_type
 *
 * Description: This function get the codec type by codec id.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *          e_strm_type          specify the stream type
 *
 * Returns: UINT8    The codec type.
 ----------------------------------------------------------------------------*/
UINT8 _mkv_get_codec_type(MINFO_MKV_INF_T* pt_mkv_info_obj, UINT8 ui1_strm_type)
{
    CHAR* psz_codec_id;
    UINT8 ui1_codec_type;
    
    psz_codec_id = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.psz_codec_id;
    ui1_codec_type = MKV_UNKNOWN;
    if (ui1_strm_type == MKV_STRM_VIDEO_TYPE)
    {
        ui1_codec_type = MKV_V_UNKNOWN;
        if (psz_codec_id != NULL)
        {
            if (x_memcmp((VOID*)"V_MS/VFW/FOURCC", psz_codec_id, 15) == 0)
            {
                ui1_codec_type = MKV_V_MS_VFW_FOURCC;
            }
            else if (x_memcmp((VOID*)"V_MPEG4/ISO/AVC", psz_codec_id, 15) == 0)
            {
                ui1_codec_type = MKV_V_MPEG4_ISO_AVC;
            }     
            else if (x_memcmp((VOID*)"V_MPEG4/ISO/SP", psz_codec_id, 14) == 0)
            {
                ui1_codec_type = MKV_V_MPEG4_ISO_SP;
            }     
            else if (x_memcmp((VOID*)"V_MPEG4/ISO/ASP", psz_codec_id, 15) == 0)
            {
                ui1_codec_type = MKV_V_MPEG4_ISO_ASP;
            }     
            else if (x_memcmp((VOID*)"V_MPEG4/MS/V3", psz_codec_id, 13) == 0)
            {
                ui1_codec_type = MKV_V_MPEG4_MS_V3;
            }
            else if (x_memcmp((VOID*)"V_MPEG1", psz_codec_id, 7) == 0)
            {
                ui1_codec_type = MKV_V_MPEG1;
            }
            else if (x_memcmp((VOID*)"V_MPEG2", psz_codec_id, 7) == 0)
            {
                ui1_codec_type = MKV_V_MPEG2;
            }
            else if (x_memcmp((VOID*)"V_VP8", psz_codec_id, 5) == 0)
            {
                ui1_codec_type = MKV_V_VP8;
            }
        }
    }
    else if (ui1_strm_type == MKV_STRM_AUDIO_TYPE)
    {
        ui1_codec_type = MKV_A_UNKNOWN;
        if (psz_codec_id != NULL)
        {
            if (x_memcmp((VOID*)"A_MPEG/L3", psz_codec_id, 9) == 0)
            {
                ui1_codec_type = MKV_A_MPEG_L3;
            }
            else if (x_memcmp((VOID*)"A_MPEG/L2", psz_codec_id, 9) == 0)
            {
                ui1_codec_type = MKV_A_MPEG_L2;
            }     
            else if (x_memcmp((VOID*)"A_MPEG/L1", psz_codec_id, 9) == 0)
            {
                ui1_codec_type = MKV_A_MPEG_L1;
            }
            else if (x_memcmp((VOID*)"A_AC3", psz_codec_id, 5) == 0)
            {
                ui1_codec_type = MKV_A_AC3;
            }
            else if (x_memcmp((VOID*)"A_DTS", psz_codec_id, 5) == 0)
            {
                ui1_codec_type = MKV_A_DTS;
            }
            else if (x_memcmp((VOID*)"A_MS/ACM", psz_codec_id, 8) == 0)
            {
                MINFO_MKV_AUDIO_STRM_INFO_T* pt_audio_strm_info;
                pt_audio_strm_info = (MINFO_MKV_AUDIO_STRM_INFO_T*)
                    pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
                if (pt_audio_strm_info->t_audio_info.i2_bits_per_sample != 4)
                {
                    ui1_codec_type = MKV_A_MS_ACM;
                }
            }
            else if (x_memcmp((VOID*)"A_AAC", psz_codec_id, 5) == 0)
            {
                ui1_codec_type = MKV_A_AAC;
            }
            else if (x_memcmp((VOID*)"A_PCM/INT/BIG", psz_codec_id, 13) == 0)
            {
                ui1_codec_type = MKV_A_PCM_BE;
            }
            else if ((x_memcmp((VOID*)"A_PCM/INT/LIT", psz_codec_id, 13) == 0) ||
                     (x_memcmp((VOID*)"A_PCM/FLOAT/IEEE", psz_codec_id, 16) == 0))
            {
                ui1_codec_type = MKV_A_PCM_LE;
            }
            else if ((x_memcmp((VOID*)"A_VORBIS", psz_codec_id, 8) == 0))
            {
                ui1_codec_type = MKV_A_VORBIS;
            }

        }
    }
    else if (ui1_strm_type == MKV_STRM_SUBTILE_TYPE)
    {
        ui1_codec_type = MKV_S_UNKNOWN;
        if (x_memcmp((VOID*)"S_TEXT/UTF8", psz_codec_id, 11) == 0)
        {
            ui1_codec_type = MKV_S_TEXT_UTF8;
        }
        else if ((x_memcmp((VOID*)"S_TEXT/SSA", psz_codec_id, 10) == 0) ||
                 (x_memcmp((VOID*)"S_TEXT/ASS", psz_codec_id, 10) == 0) ||
                 (x_memcmp((VOID*)"S_SSA", psz_codec_id, 5) == 0) ||
                 (x_memcmp((VOID*)"S_ASS", psz_codec_id, 5) == 0))
        {
            ui1_codec_type = MKV_S_TEXT_SSA;
        }
        else if (x_memcmp((VOID*)"S_VOBSUB", psz_codec_id, 8) == 0)
        {
            ui1_codec_type = MKV_S_VOBSUB;
        }
        else if ((x_memcmp((VOID*)"S_TEXT/USF", psz_codec_id, 10) == 0) ||
                 (x_memcmp((VOID*)"S_USF", psz_codec_id, 5) == 0))
        {
            ui1_codec_type = MKV_S_TEXT_USF;
        }
        else if (x_memcmp((VOID*)"S_IMAGE/BMP", psz_codec_id, 11) == 0)
        {
            ui1_codec_type = MKV_S_IMG_BMP;
        }
        else if (x_memcmp((VOID*)"S_KATE", psz_codec_id, 6) == 0)
        {
            ui1_codec_type = MKV_S_KATE;
        }
    }
    return ui1_codec_type;
}

/*-----------------------------------------------------------------------------
 * Name:  _get_aud_codec
 *
 * Description: This function get the real audio codec by codec private value.
 *
 * Inputs:  ui2_format      specify the codec format
 *          pt_aud_strm     specify the current parsed audio stream
 *
 * Returns: UINT8    The audio real codec value.
 ----------------------------------------------------------------------------*/
UINT8  _get_aud_codec(UINT16 ui2_format, MINFO_MKV_AUDIO_STRM_INFO_T* pt_aud_strm)
{
    UINT8 ui1_a_codec = MINFO_INFO_AUD_ENC_UNKNOWN;
    
    switch (ui2_format)
    {
    case MKV_WAVE_FORMAT_PCM:
    case MKV_WAVE_FORMAT_ADPCM:
        ui1_a_codec = MINFO_INFO_AUD_ENC_PCM;
        pt_aud_strm->t_audio_info.e_align = MINFO_INFO_AUD_PCM_LITTLE_ENDIAN;
        break;
    case MKV_WAVE_FORMAT_DTS_MS:
    case MKV_WAVE_FORMAT_DTS:
        ui1_a_codec = MINFO_INFO_AUD_ENC_DTS;
        break;
    case MKV_WAVE_FORMAT_A52:
    case MKV_WAVE_FORMAT_DOLBY_AC3_SPDIF:
        ui1_a_codec = MINFO_INFO_AUD_ENC_AC3;
        break;
    case MKV_WAVE_FORMAT_AAC:
    case MKV_WAVE_FORMAT_FFMPEG_AAC:
    case MKV_WAVE_FORMAT_DIVIO_AAC:
        ui1_a_codec = MINFO_INFO_AUD_ENC_AAC;
        break;
    case MKV_WAVE_FORMAT_MPEGLAYER3:
        pt_aud_strm->t_audio_info.e_layer = 3;
    case MKV_WAVE_FORMAT_MPEG:
        ui1_a_codec = MINFO_INFO_AUD_ENC_MPEG_1;
        break;
    case MKV_WAVE_FORMAT_WMA1:
        ui1_a_codec = MINFO_INFO_AUD_ENC_WMA_V1;
        break;
    case MKV_WAVE_FORMAT_WMA2:
        ui1_a_codec = MINFO_INFO_AUD_ENC_WMA_V2;
        break;
    }
    return ui1_a_codec;
}

/*-----------------------------------------------------------------------------
 * Name:  _get_vid_codec
 *
 * Description: This function get the real video codec by codec private value.
 *
 * Inputs:  ui4_4cc      specify the codec fourcc value
 *
 * Returns: UINT8    The video real codec value.
 ----------------------------------------------------------------------------*/
UINT8 _get_vid_codec(UINT32 ui4_4cc)
{
  UINT8 ui1_v_codec = MINFO_INFO_VID_ENC_UNKNOWN;

  switch (ui4_4cc)
  {
    case MKV_DWLE('X', 'V', 'I', 'D'):   /*xvid*/
    case MKV_DWLE('x', 'v', 'i', 'd'):
	  ui1_v_codec = MINFO_INFO_VID_ENC_XVID;
      break;
    case MKV_DWLE('3', 'I', 'V', '1'):   /* 3ivx Delta 1.0~3.5*/
    case MKV_DWLE('3', 'i', 'v', '1'):
    case MKV_DWLE('3', 'I', 'V', '2'):   /* 3ivx Delta 4.0*/
    case MKV_DWLE('3', 'i', 'v', '2'):
    case MKV_DWLE('F', 'V', 'F', 'W'):  /*ffmpeg VFW*/
    case MKV_DWLE('f', 'v', 'f', 'w'):
    case MKV_DWLE('D', 'M', 'K', '2'):  /*a file from Gapha_Tong has this 4cc*/
    case MKV_DWLE('F', 'M', 'P', '4'):
      ui1_v_codec = MINFO_INFO_VID_ENC_MPEG_4;
      break;
    case MKV_DWLE('E', 'M', '2', 'V'):  /*tempt.*/
    case MKV_DWLE('M', 'P', 'G', '2'):
    case MKV_DWLE('M', 'P', 'E', 'G'):
    case MKV_DWLE('M', 'P', 'G', 'V'):           
    case MKV_DWLE('m', 'p', 'g', 'v'):                     
      ui1_v_codec = MINFO_INFO_VID_ENC_MPEG_2;
      break;
    case MKV_DWLE('H', '2', '6', '4'):
    case MKV_DWLE('h', '2', '6', '4'):
      ui1_v_codec = MINFO_INFO_VID_ENC_H264;
      break;
    case MKV_DWLE('R', 'M', 'P', '4'):  /*RMP4*/
    case MKV_DWLE('r', 'm', 'p', '4'):
      ui1_v_codec = MINFO_INFO_VID_ENC_UNKNOWN;
      break;
    case MKV_DWLE('D', 'I', 'V', '3'):  /*DivX 3.11*/
    case MKV_DWLE('d', 'i', 'v', '3'):
    case MKV_DWLE('D', 'I', 'V', '4'):
    case MKV_DWLE('d', 'i', 'v', '4'):
    case MKV_DWLE('D', 'I', 'V', '5'):
    case MKV_DWLE('d', 'i', 'v', '5'):
    case MKV_DWLE('D', 'I', 'V', '6'):
    case MKV_DWLE('d', 'i', 'v', '6'):
    case MKV_DWLE('M', 'P', '4', '3'):  /*Microsoft MPEG-4 v3*/
    case MKV_DWLE('m', 'p', '4', '3'):
    /* case MKV_DWLE('M', 'P', '4', 'S'): */
    case MKV_DWLE('C', 'O', 'L', '1'):  /* cool codec*/
    case MKV_DWLE('c', 'o', 'l', '1'):
    case MKV_DWLE('A', 'P', '4', '1'):  /*AngelPotion*/
    case MKV_DWLE('a', 'p', '4', '1'):
    case MKV_DWLE('n', 'A', 'V', 'I'):  /*nAVI*/
      ui1_v_codec = MINFO_INFO_VID_ENC_DIVX_311;
      break;
    case MKV_DWLE('D', 'I', 'V', 'X'):  /*DivX 4.x*/
    case MKV_DWLE('d', 'i', 'v', 'x'):
      ui1_v_codec = MINFO_INFO_VID_ENC_DIVX_4;
      break;
    case MKV_DWLE('D', 'X', '5', '0'):  /* DivX 5.x*/
    case MKV_DWLE('d', 'x', '5', '0'):
      ui1_v_codec = MINFO_INFO_VID_ENC_DIVX_5;
      break;
    case MKV_DWLE('m', 'j', 'p', 'g'):    
    case MKV_DWLE('M', 'J', 'P', 'G'):
      ui1_v_codec = MINFO_INFO_VID_ENC_MJPG;
      break;  
    case MKV_DWLE('w', 'v', 'c', '1'):
    case MKV_DWLE('W', 'V', 'C', '1'):
      ui1_v_codec = MINFO_INFO_VID_ENC_WVC1;
      break;
    case MKV_DWLE('w', 'm', 'v', '1'):
    case MKV_DWLE('W', 'M', 'V', '1'):
      ui1_v_codec = MINFO_INFO_VID_ENC_WMV1;
      break;
    case MKV_DWLE('w', 'm', 'v', '3'):
    case MKV_DWLE('W', 'M', 'V', '3'):
      ui1_v_codec = MINFO_INFO_VID_ENC_WMV3;
      break;
    default:
      break;
  }
  return(ui1_v_codec);
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_video_codec
 *
 * Description: This function set the video codec information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_video_codec(MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT8                        ui1_codec_type;
    UINT32                       ui4_size;
    MINFO_MKV_VIDEO_STRM_INFO_T* pt_cur_vid_strm;
    UINT8*                       pui1_codec_private;
    UINT32                       ui4_private_size;
    UINT32                       ui4_value;
    INT32                        i4_ret;
    
    pui1_codec_private = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private;
    pt_cur_vid_strm = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    ui4_size = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_codec_private_len;
    ui1_codec_type = _mkv_get_codec_type(pt_mkv_info_obj, MKV_STRM_VIDEO_TYPE);    
    i4_ret = MINFOR_OK;
    switch (ui1_codec_type)
    {
    case MKV_V_UNKNOWN:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_UNKNOWN;
        pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
        break;
    case MKV_V_MS_VFW_FOURCC:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MS_VFW_FOURCC;
        if (ui4_size < 40)
        {
            DBG_ERROR(("_mkv_set_video_codec: codec private size error %d!\r\n", ui4_size));
            i4_ret = MINFOR_INTERNAL_ERR;
        }
        else
        {
            /*BITMAP INFO HEADER*/
            ui4_private_size = MKV_GETDWLE((UINT8*)(pui1_codec_private), ui4_size);
            ui4_value = MKV_GETDWLE((UINT8*)(pui1_codec_private + 4), (ui4_size - 4));
            pt_cur_vid_strm->t_video_info.i4_width = (INT32)ui4_value;
            ui4_value = MKV_GETDWLE((UINT8*)(pui1_codec_private + 8), (ui4_size - 8));
            pt_cur_vid_strm->t_video_info.i4_height = (INT32)ui4_value;
            ui4_value = MKV_GETDWLE((UINT8*)(pui1_codec_private + 16), (ui4_size - 16));
            pt_cur_vid_strm->t_video_info.e_enc = (MINFO_INFO_VIDEO_ENC_T)_get_vid_codec(ui4_value);
            if (pt_cur_vid_strm->t_video_info.e_enc != MINFO_INFO_VID_ENC_UNKNOWN)
            {
                ui4_private_size = ui4_private_size - 40;
                if (ui4_private_size > 0)
                {
                    pt_cur_vid_strm->t_video_info.pui1_codec_info = _mm_util_mem_alloc(ui4_private_size);
                    if (pt_cur_vid_strm->t_video_info.pui1_codec_info == NULL)
                    {
                        DBG_ERROR(("_mkv_set_video_codec : MKV_V_MS_VFW_FOURCC malloc error!\r\n"));
                        return MINFOR_INTERNAL_ERR;
                    }
                    pt_cur_vid_strm->t_video_info.ui4_codec_info_size = ui4_private_size;
                    x_memcpy((VOID *)pt_cur_vid_strm->t_video_info.pui1_codec_info,
                             (VOID *)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private + 40),
                              ui4_private_size); 
                }
            }
        }
        break;
    case MKV_V_MPEG1:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG1;
        pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_MPEG_1;
        break;
    case MKV_V_MPEG2:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG2;
        pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_MPEG_2;
        break;
    case MKV_V_MPEG4_ISO_AVC:
    case MKV_V_MPEG4_ISO_ASP:
    case MKV_V_MPEG4_ISO_SP:
        if (ui1_codec_type == MKV_V_MPEG4_ISO_AVC)
        {
            pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_AVC;
            pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_H264;
        }
        else 
        {
            /*MKV_V_MPEG4_ISO_ASP/MKV_V_MPEG4_ISO_SP*/
            if (ui1_codec_type == MKV_V_MPEG4_ISO_ASP)
            {
                pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_ASP;
            }
            else
            {
                pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG4_ISO_SP;
            }
            pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_MPEG_4;
        }
        if (ui4_size > 0)
        {
            pt_cur_vid_strm->t_video_info.pui1_codec_info = _mm_util_mem_alloc(ui4_size);
            if (pt_cur_vid_strm->t_video_info.pui1_codec_info == NULL)
            {
                DBG_ERROR(("_mkv_set_video_codec : MKV_V_MPEG4_ISO_AVC malloc error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            pt_cur_vid_strm->t_video_info.ui4_codec_info_size = ui4_size;
            x_memcpy((VOID *)pt_cur_vid_strm->t_video_info.pui1_codec_info,
                     (VOID *)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private,
                      ui4_size);    
        }
        break;
    case MKV_V_MPEG4_MS_V3:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_MPEG4_MS_V3;
        pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_DIVX_311;
        break;
    case MKV_V_VP8:
        pt_cur_vid_strm->t_video_info.e_codec_type = MINFO_MKV_V_CODEC_TYPE_VP8;
        pt_cur_vid_strm->t_video_info.e_enc = MINFO_INFO_VID_ENC_VP8;
        break;
    default:
        DBG_ERROR(("_mkv_set_video_codec: codec type wrong %d !\r\n", ui1_codec_type));
        i4_ret = MINFOR_INTERNAL_ERR;
        break;
    }
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_sp_codec
 *
 * Description: This function set the subtitle codec information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_sp_codec(MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    MINFO_MKV_SUBTITLE_STRM_INFO_T* pt_cur_sp_strm;
    UINT8                           ui1_codec_type;
    UINT32                          ui4_size;
    INT32                           i4_ret;
    BOOL                            b_load_private = FALSE;
    
    pt_cur_sp_strm = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    ui4_size = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_codec_private_len;
    ui1_codec_type = _mkv_get_codec_type(pt_mkv_info_obj, MKV_STRM_SUBTILE_TYPE);
    i4_ret = MINFOR_OK;
    switch (ui1_codec_type)
    {
    case MKV_S_UNKNOWN:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_SUBTITLE_TYPE_UNKNOWN;
        break;
    case MKV_S_TEXT_UTF8:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8;
        break;
    case MKV_S_TEXT_SSA:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA;
        b_load_private = TRUE;
        break;
    case MKV_S_TEXT_USF:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_USF;
        break;
    case MKV_S_VOBSUB:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB;
        b_load_private = TRUE;
        break;
    case MKV_S_IMG_BMP:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP;
        b_load_private = TRUE;
        break;
    case MKV_S_KATE:
        pt_cur_sp_strm->t_subtitle_info.e_subtitle_type = MINFO_INFO_MKV_SUBTITLE_TYPE_KATE;
        break;
    }
    if (b_load_private)
    {
        if (ui4_size > 0)
        {
            pt_cur_sp_strm->t_subtitle_info.pui1_codec_info = _mm_util_mem_alloc(ui4_size);
            if (pt_cur_sp_strm->t_subtitle_info.pui1_codec_info == NULL)
            {
                DBG_ERROR(("_mkv_set_sp_codec : malloc error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            pt_cur_sp_strm->t_subtitle_info.ui4_codec_info_size = ui4_size;
            x_memcpy((VOID *)pt_cur_sp_strm->t_subtitle_info.pui1_codec_info,
                     (VOID *)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private,
                      ui4_size);    
        }
    }
    return i4_ret;  
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_set_audio_codec
 *
 * Description: This function set the audio codec information.
 *
 * Inputs:  pt_mkv_info_obj      specify the mkv info object
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 _mkv_set_audio_codec(MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT8                           ui1_codec_type;
    UINT32                          ui4_size;
    MINFO_MKV_AUDIO_STRM_INFO_T*    pt_cur_aud_strm;
    UINT8*                          pui1_codec_private;
    UINT16                          ui2_format;
    UINT16                          ui2_value;
    UINT32                          ui4_value;
    UINT16                          ui2_cb_size;
    INT32                           i4_ret;
    UINT8*                          pui1_tmp = NULL;
    UINT32                          ui4_tmp = 0;
    
    pui1_codec_private = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private;
    pt_cur_aud_strm = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pv_cur_parse_strm;
    ui4_size = (UINT32)pt_mkv_info_obj->t_all_strm_info.t_strm_properties.ui8_codec_private_len;
    ui1_codec_type = _mkv_get_codec_type(pt_mkv_info_obj, MKV_STRM_AUDIO_TYPE);    
    i4_ret = MINFOR_OK;
    switch (ui1_codec_type)
    {
    case MKV_A_UNKNOWN:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        break;
    case MKV_A_AAC:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_AAC;
        if (ui4_size > 0)
        {
            pt_cur_aud_strm->t_audio_info.pui1_codec_info = _mm_util_mem_alloc(ui4_size);
            if (pt_cur_aud_strm->t_audio_info.pui1_codec_info == NULL)
            {
                DBG_ERROR(("_mkv_set_audio_codec : MKV_A_AAC malloc error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            x_memcpy((VOID *)pt_cur_aud_strm->t_audio_info.pui1_codec_info,
                     (VOID *)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private),
                      ui4_size);   
            pt_cur_aud_strm->t_audio_info.ui4_codec_info_size = ui4_size;
            /* aac profile parse:  first 5 bits is profile, if profile value=31, then profile= 32 + follow 6bits  */
            pui1_tmp = pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private;
            ui4_tmp = (pui1_tmp[0]&0xF8)>>3;
            if (ui4_tmp == 0x1F)
            {
                ui4_tmp = ((pui1_tmp[0]&0x07) << 3 |
                    (pui1_tmp[1]&0xE0) >> 5);
                pt_cur_aud_strm->t_audio_info.ui4_aac_profile = 32 + ui4_tmp;
            }
            else
            {
                pt_cur_aud_strm->t_audio_info.ui4_aac_profile = ui4_tmp;
            }
        }
        break;
    case MKV_A_AC3:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_AC3;
        break;
    case MKV_A_DTS:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_DTS;
        break;
    case MKV_A_MPEG_L1:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        pt_cur_aud_strm->t_audio_info.e_layer = 1;
        break;
    case MKV_A_MPEG_L2:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        pt_cur_aud_strm->t_audio_info.e_layer = 2;
        break;
    case MKV_A_MPEG_L3:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        pt_cur_aud_strm->t_audio_info.e_layer = 3;
        break;
    case MKV_A_MS_ACM:
        /*need check private*/
        if (ui4_size < 18)
        {
            DBG_ERROR(("_mkv_set_audio_codec: codec private size error %d!\r\n", ui4_size));
            i4_ret = MINFOR_INTERNAL_ERR;
        }
        else
        {
            ui2_format = MKV_GETWLE(pui1_codec_private, ui4_size);            
            /*set format codec*/
            pt_cur_aud_strm->t_audio_info.e_enc = (MINFO_INFO_AUDIO_ENC_T)_get_aud_codec(ui2_format, pt_cur_aud_strm);
            if (pt_cur_aud_strm->t_audio_info.e_enc != MINFO_INFO_AUD_ENC_UNKNOWN)
            {
                /*WAVE FORMAT EX*/
                ui2_value = MKV_GETWLE((UINT8*)(pui1_codec_private + 2), (ui4_size - 2));
                pt_cur_aud_strm->t_audio_info.i2_channels = (INT16)ui2_value;
                ui4_value = MKV_GETDWLE((UINT8*)(pui1_codec_private + 4), (ui4_size - 4));
                pt_cur_aud_strm->t_audio_info.i4_samples_per_sec = (INT32)ui4_value;
                ui4_value = MKV_GETDWLE((UINT8*)(pui1_codec_private + 8), (ui4_size - 8));
                pt_cur_aud_strm->t_audio_info.ui4_avg_bytes_sec = ui4_value;
                ui2_value = MKV_GETWLE((UINT8*)(pui1_codec_private + 12), (ui4_size - 12));
                pt_cur_aud_strm->t_audio_info.i2_block_align = (INT16)ui2_value;
                ui2_value = MKV_GETWLE((UINT8*)(pui1_codec_private + 14), (ui4_size - 14));
                pt_cur_aud_strm->t_audio_info.i2_bits_per_sample = (INT16)ui2_value;
                ui2_cb_size = MKV_GETWLE((UINT8*)(pui1_codec_private + 16), (ui4_size - 16));
                if ((pt_cur_aud_strm->t_audio_info.e_enc == MINFO_INFO_AUD_ENC_WMA_V1) && (ui2_cb_size > 6))
                {
                    pt_cur_aud_strm->t_audio_info.ui2_encode_opts = MKV_GETWLE((UINT8*)(pui1_codec_private + 24), (ui4_size - 24));
                }
                else if ((pt_cur_aud_strm->t_audio_info.e_enc == MINFO_INFO_AUD_ENC_WMA_V2) && (ui2_cb_size > 4))
                {
                    pt_cur_aud_strm->t_audio_info.ui2_encode_opts = MKV_GETWLE((UINT8*)(pui1_codec_private + 22), (ui4_size - 22));
                }
                if (ui2_cb_size > 0)
                {
                    pt_cur_aud_strm->t_audio_info.pui1_codec_info = _mm_util_mem_alloc((UINT32)ui2_cb_size);
                    if (pt_cur_aud_strm->t_audio_info.pui1_codec_info == NULL)
                    {
                        DBG_ERROR(("_mkv_set_audio_codec : MKV_A_MS_ACM malloc error!\r\n"));
                        return MINFOR_INTERNAL_ERR;
                    }
                    x_memcpy((VOID *)pt_cur_aud_strm->t_audio_info.pui1_codec_info,
                             (VOID *)(pt_mkv_info_obj->t_all_strm_info.t_strm_properties.pui1_codec_private + 18),
                             (UINT32)ui2_cb_size);   
                    pt_cur_aud_strm->t_audio_info.ui4_codec_info_size = (UINT32)ui2_cb_size;
                }
            }
        }
        break;
    case MKV_A_PCM_BE:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_PCM;
        pt_cur_aud_strm->t_audio_info.e_align = MINFO_INFO_AUD_PCM_BIG_ENDIAN;
        break;
    case MKV_A_PCM_LE:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_PCM;
        pt_cur_aud_strm->t_audio_info.e_align = MINFO_INFO_AUD_PCM_LITTLE_ENDIAN;
        break;
    case MKV_A_VORBIS:
        pt_cur_aud_strm->t_audio_info.e_enc = MINFO_INFO_AUD_ENC_VORBIS;
        break;
    default:
        DBG_ERROR(("_mkv_set_audio_codec: codec type wrong %d !\r\n", ui1_codec_type));
        i4_ret = MINFOR_INTERNAL_ERR;
        break;
    }
    return i4_ret;    
}
#if MKV_DIVX_PLUS_SUPPORT /*MKV DivXPlus*/
INT32 _mkv_parse_prev_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_PREV_SEG_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &pt_mkv_info_obj->t_segment_info.ui8_prev_seg_uid_h,
                              &pt_mkv_info_obj->t_segment_info.ui8_prev_seg_uid_l);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_next_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;
    
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_NEXT_SEG_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &pt_mkv_info_obj->t_segment_info.ui8_next_seg_uid_h,
                              &pt_mkv_info_obj->t_segment_info.ui8_next_seg_uid_l);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_edit_entry( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                   i4_ret;
    MINFO_MKV_EDITION_T*    pt_list;
    MINFO_MKV_EDITION_T*    pt_new_node; 
    
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_EDITION_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_EDITION_T));
    /*find last node*/
    pt_list = pt_mkv_info_obj->t_chapters_info.pt_edition;
    if (pt_list == NULL)
    {
        pt_mkv_info_obj->t_chapters_info.pt_edition = pt_new_node;
    }
    else
    {
        while (pt_list->pt_next!= NULL)
        {
            pt_list = pt_list->pt_next;
        }
        pt_list->pt_next = pt_new_node;
    }
    /*set current parse entry*/
    pt_mkv_info_obj->t_chapters_info.ui4_edition_num += 1;
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry = pt_new_node;
    i4_ret = _mkv_feeder_parse(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

INT32 _mkv_parse_edit_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_EDIT_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->ui8_uid = ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_edit_hidden( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_EDIT_HIDDEN].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->fg_hidden = (ui8_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_edit_default( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_EDIT_DEFAULT].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->fg_default = (ui8_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_edit_ordered( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_2ND_EDIT_ORDERED].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->fg_ordered = (ui8_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_atom( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                     i4_ret;
    MINFO_MKV_CHAP_ATOM_T*    pt_list;
    MINFO_MKV_CHAP_ATOM_T*    pt_new_node; 
    EBML_ELE_BASIC_T*         pt_ebml_element;
    UINT64                    ui8_size;
    
    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_CHAP_ATOM_T));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_CHAP_ATOM_T));
    /*find last node*/
    pt_list = pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom;
    if (pt_list == NULL)
    {
        pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom = pt_new_node;
    }
    else
    {
        while (pt_list->pt_next!= NULL)
        {
            pt_list = pt_list->pt_next;
        }
        pt_list->pt_next = pt_new_node;
    }
    /*set current parse entry*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->ui4_chap_num += 1;
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap = pt_new_node;
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

INT32 _mkv_parse_chap_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui8_uid = ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_time_start( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_TIME_START].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*ui8_val = _mm_div64((UINT64)ui8_val, (UINT64)1000000000, NULL); */
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui8_start_time = ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_time_end( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_TIME_END].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*ui8_val = _mm_div64((UINT64)ui8_val, (UINT64)1000000000, NULL); */
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui8_end_time = ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_hidden( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_HIDDEN].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, ui8_size, &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->fg_hidden = (ui8_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_enabled( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_ENABLED].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, 
                             ui8_size, 
                             &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->fg_enabled = (ui8_val != 0);
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_seg_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_SEG_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_luint(pt_mkv_info_obj, 
                              ui8_size, 
                              &(pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui8_chap_seg_uid_h),
                              &(pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui8_chap_seg_uid_l));
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_seg_edit_uid( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_3RD_EDIT_CHAP_SEG_EDIT_UID].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, 
                             ui8_size, 
                             &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui4_edition_uid = (UINT32)ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_tracks( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                     i4_ret;
    MINFO_MKV_CHAP_TRACK*     pt_list;
    MINFO_MKV_CHAP_TRACK*     pt_new_node; 
    EBML_ELE_BASIC_T*       pt_ebml_element = NULL;
    UINT64                  ui8_size = 0;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    /*malloc a new seek entry to store info*/
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_CHAP_TRACK));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_CHAP_TRACK));
    /*find last node*/
    pt_list = pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom->pt_track;
    if (pt_list == NULL)
    {
        pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom->pt_track = pt_new_node;
    }
    else
    {
        while (pt_list->pt_next!= NULL)
        {
            pt_list = pt_list->pt_next;
        }
        pt_list->pt_next = pt_new_node;
    }
    /*set current parse entry*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui4_track_num += 1;
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_track = pt_new_node;
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}

INT32 _mkv_parse_chap_track_num( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    UINT64            ui8_val;
    INT32             i4_ret;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_EDIT_CHAP_TRACK_NUM].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    i4_ret = _mkv_parse_uint(pt_mkv_info_obj, 
                             ui8_size, 
                             &ui8_val);
    if (i4_ret != EBMLR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_track->ui4_number = (UINT32)ui8_val;
    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_disp( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    INT32                   i4_ret = 0;
    MINFO_MKV_DISPLAY*      pt_list = NULL;
    MINFO_MKV_DISPLAY*      pt_new_node = NULL; 
    EBML_ELE_BASIC_T*       pt_ebml_element = NULL;
    UINT64                  ui8_size = 0;
    
    /*malloc a new seek entry to store info*/
    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_new_node = _mm_util_mem_alloc(sizeof(MINFO_MKV_DISPLAY));
    if (pt_new_node == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_new_node, 0, sizeof(MINFO_MKV_DISPLAY));
    /*find last node*/
    pt_list = pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom->pt_display;
    if (pt_list == NULL)
    {
        pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_chap_atom->pt_display = pt_new_node;
    }
    else
    {
        while (pt_list->pt_next!= NULL)
        {
            pt_list = pt_list->pt_next;
        }
        pt_list->pt_next = pt_new_node;
    }
    /*set current parse entry*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->ui4_display_num += 1;
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display = pt_new_node;
    i4_ret = _mkv_only_parse_mem(pt_minfo_obj, pt_mkv_info_obj);
    return i4_ret;
}


INT32 _mkv_parse_chap_string( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_EDIT_CHAP_STR].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_string = 
            _mm_util_mem_alloc((SIZE_T)(sizeof(CHAR)*(ui8_size+1)));
    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_string == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memcpy(pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_string,
        pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset,
        (UINT32)ui8_size);

    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}

INT32 _mkv_parse_chap_lang( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_EDIT_CHAP_LANG].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_language = 
            _mm_util_mem_alloc((SIZE_T)(sizeof(CHAR)*(ui8_size+1)));
    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_language == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memcpy(pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_language,
        pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset,
        (SIZE_T)ui8_size);

    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}


INT32 _mkv_parse_chap_country( MINFO_OBJ_T* pt_minfo_obj, MINFO_MKV_INF_T* pt_mkv_info_obj)
{
    UINT32            ui4_id;
    UINT64            ui8_size;
    EBML_ELE_BASIC_T* pt_ebml_element;

    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry == NULL)
    {
        pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
        ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
        _mkv_skip_data(pt_minfo_obj, ui8_size, pt_mkv_info_obj, FALSE);
        return MINFOR_OK;
    }
    pt_ebml_element = &(pt_mkv_info_obj->t_curr_element);
    ui4_id = _ebml_get_ele_id(pt_ebml_element);
    ui8_size = _mkv_get_element_size(pt_ebml_element, pt_minfo_obj, pt_mkv_info_obj, FALSE);
    if (ui4_id != g_at_mkv_element_op[MKV_ELEMENT_4TH_EDIT_CHAP_COUNTRY].ui4_element_id)
    {
        return MINFOR_INTERNAL_ERR;
    }
    /*to do parse*/
    pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_country = 
            _mm_util_mem_alloc((SIZE_T)(sizeof(CHAR)*(ui8_size+1)));
    if (pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_country == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memcpy(pt_mkv_info_obj->t_chapters_info.pt_curr_parse_entry->pt_curr_chap->pt_cur_display->ps_country,
        pt_mkv_info_obj->pui1_header_buf + pt_mkv_info_obj->ui8_cur_parser_offset,
        (SIZE_T)ui8_size);

    pt_mkv_info_obj->ui8_cur_parser_offset += ui8_size;
    return MINFOR_OK;
}


#endif

