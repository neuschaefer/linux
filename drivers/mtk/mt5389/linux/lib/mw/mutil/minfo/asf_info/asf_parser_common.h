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
 * asf_parser.h,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the asf parser function
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

#ifndef _ASF_PARSER_COMMON_H_
#define _ASF_PARSER_COMMON_H_

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
#include "handle/u_handle.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "x_uc_str.h"
#include "../../mm_util.h"

#endif
/*-----------------------------------------------------------------------------

                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/
#define ASFR_GUID_NOT_EQUAL    ( 1 )
#define ASFR_OK                ( 0 )
#define ASFR_GUID_EQUAL        ( ASFR_OK )

#define ASFR_INV_ARG            ( -1 )

#define ASF_STREAM_NUM_MAX     127
#define ASF_STRM_VID_MAX       ASF_STREAM_NUM_MAX
#define ASF_STRM_AUD_MAX       ASF_STREAM_NUM_MAX
#define ASF_WMA_KEY_TBL_ENTRY_MAX_CNT          1000

/*asf : litter endian ,define macro to get WORD/DWORD/QWORD/ 
 from a byte sequence*/
#if 0
#define ASF_GET_BYTE(ui1_data, p)  ( (ui1_data) = (UINT8)(p)[0] )
#define ASF_GET_WORD(ui2_data, p)  ( (ui2_data) = ((UINT16)(p)[1] << 8)   \
                                                  | (UINT16)(p)[0] )

#define ASF_GET_DWORD(ui4_data, p) ( (ui4_data) = ((UINT32)(p)[3] << 24)   \
                                              | ((UINT32)(p)[2] << 16)     \
                                              | ((UINT32)(p)[1] << 8)      \
                                              | (UINT32)(p)[0] )

#define ASF_GET_QWORD(ui8_data, p) ( (ui8_data) = ((UINT64)(p)[7] << 56)   \
                                                | ((UINT64)(p)[6] << 48)   \
                                                | ((UINT64)(p)[5] << 40)   \
                                                | ((UINT64)(p)[4] << 32)   \
                                                | ((UINT64)(p)[3] << 24)   \
                                                | ((UINT64)(p)[2] << 16)   \
                                                | ((UINT64)(p)[1] << 8)    \
                                                | (UINT64)(p)[0] )
#else
#define ASF_GET_BYTE(ui1_data, p)  LOAD_BYTE(p, ui1_data)
#define ASF_GET_WORD(ui2_data, p)  LOADL_WORD(p, ui2_data)
#define ASF_GET_DWORD(ui4_data, p) LOADL_DWRD(p, ui4_data)
#define ASF_GET_QWORD(ui8_data, p) LOADL_QWRD(p, ui8_data)
#endif

typedef INT32 (*asf_copybytes_fct)( VOID*   pv_obj, 
                                     VOID*   pv_buf, 
                                     SIZE_T  z_buf_leng, 
                                     SIZE_T  z_copy_size, 
                                     UINT32* pui4_size);
typedef INT32 (*asf_setpos_fct)(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence);
typedef INT32 (*asf_getpos_fct)(VOID* pv_obj, UINT64* pui8_cur_pos);



/*every object has some common fields*/                                                
#define ASF_OBJ_COMMON                \
    ASF_GUID_T      t_guid;     \
    UINT64          ui8_size;    
#define ASF_OBJ_COMMON_LEN    24

/*enumerate all supported object */
typedef enum _ASF_OBJ_TYPE_E
{
    ASF_OBJ_UNKNOWN,
    ASF_OBJ_HEADER,
    ASF_OBJ_DATA,
    ASF_OBJ_SIMPLE_INDEX,
    ASF_OBJ_FILE_PROPERTIES,
    ASF_OBJ_STREAM_PROPERTIES,
    ASF_OBJ_HEADER_EXTENSION,
    ASF_OBJ_EX_STREAM_PROPERTIES,
    ASF_OBJ_LANGUAGE_LIST,
    ASF_OBJ_CODEC_LIST,
    ASF_OBJ_CONTENT_DESCRIPTION,
    ASF_OBJ_EXT_CONTENT_DESCRIPTION,
    ASF_OBJ_METADATA,
    ASF_OBJ_IDX_PARAS,
    ASF_OBJ_BITRATE_PROPERTIES,
    ASF_OBJ_CONTENT_ENCRYPTION,
    ASF_OBJ_INDEX,
    ASF_OBJECT_BUTT
}ASF_OBJ_TYPE_E;

/*GUID identifying a object uniquely*/
typedef struct _ASF_GUID_T
{
    UINT32 ui4_v1;      /*  */
    UINT16 ui2_v2;      /*  */
    UINT16 ui2_v3;      /*  */
    UINT8  auc_v4[8];
} ASF_GUID_T;

/*ASF tree structure node definition*/
typedef struct _ASF_OBJ_COMMON_T
{
    ASF_OBJ_TYPE_E            e_type;     
    ASF_GUID_T                t_guid;     
    UINT64                    ui8_size;    
    UINT64                    ui8_offset; 
    /*point to a structure defined in ASF spec.*/
    const VOID*               pv_obj_specific;  
    
    struct _ASF_OBJ_COMMON_T* pt_first_child;
    struct _ASF_OBJ_COMMON_T* pt_parent;
    struct _ASF_OBJ_COMMON_T* pt_next_sibling;
    
}ASF_OBJ_COMMON_T;    

/*ASF tree root*/
typedef struct _MINFO_ASF_ROOT_T
{
    ASF_OBJ_COMMON_T* pt_header;
    ASF_OBJ_COMMON_T* pt_data;
    ASF_OBJ_COMMON_T* pt_simple_index;
}MINFO_ASF_ROOT_T;    


/****************************************************************************
 * 1. Begin Top-level asf objects definition
 ****************************************************************************/

/*Header : This is the first header found in a asf file
 * It's the only object that have subobject */
typedef struct _ASF_HEADER_OBJECT_T
{
    /*how many subobject are containted in this one? 
      not including internal subojects*/
    UINT32 ui4_sub_object_count;  
    /* 0x01, but could be safely ignored */
    UINT8  ui1_reserved1; 
    /* 0x02, if is different should fail to source the content */
    UINT8  ui1_reserved2; 

} ASF_HEADER_OBJECT_T;


/*Length type long*/
typedef enum _ASF_LENGTH_TYPE_E
{
    ASF_LEN_NULL  = 0,
    ASF_LEN_BYTE  = 1,
    ASF_LEN_WORD  = 2,
    ASF_LEN_DWORD = 3,
    AS_LEN_BUTT
    
}ASF_LENGTH_TYPE_E;

typedef struct _ASF_PAYLOAD_PARSE_INFO_T
{
    UINT8             ui1_len_type_flg;
    UINT8             ui1_property_flg;  
    
    UINT32            ui4_packet_len;
    UINT32            ui4_sequence_len;
    UINT32            ui4_padding_len;
    
    UINT32            ui4_send_time;
    UINT16            ui2_duration;
    
}ASF_PAYLOAD_PARSE_INFO_T;
/*Length type long*/
typedef enum _ASF_PAYLOAD_DATA_TYPE_E
{
    ASF_PAYLOAD_SINGLE,      /*single payload*/
    ASF_PAYLOAD_COM_SINGLE,  /*single payload,compressed*/
    ASF_PAYLOAD_MULTI,       /*multiple payloads*/
    ASF_PAYLOAD_COM_MULTI,   /*multiple payloads,compressed*/
    ASF_PAYLOAD_BUTT
    
}ASF_PAYLOAD_DATA_TYPE_E;

/*it is enough to parse a packet*/
#define ASF_PACKET_INIT_LEN             (512)
/*packet maximum length*/
#define ASF_PACKET_LEN_MAX              (64*1024)

/*uncompressed payload common info*/
/*strream number:bit7 is key marker*/ 
/*media object number*/
/*offset into media object*/
/*replicated data length */
/*media object number*/

#define ASF_PAYLOAD_COMMON_FIELDS  \
    UINT8  ui1_stream_num;        \
    UINT32 ui4_med_obj_num;       \
    UINT32 ui4_offset;            \
    UINT32 ui4_relicated_len;     \
    UINT32 ui4_med_obj_size;      \
    UINT32 ui4_pts;               \
    UINT8* pui1_replicated_other;  
    

/*compressed payload common info*/
/*replicated data length:should be 1 so bo not save
  UINT32 ui4_relicated_len;
*/
/*strream number:bit7 is key marker*/
/*media object number of the first sub-payload*/
/*PTS of the first sub-payload*/
 /*the pts interval between two sub-payloads*/
#define ASF_COM_PAYLOAD_COMMON_FIELDS  \
    UINT8  ui1_stream_num;             \
    UINT32 ui4_med_obj_num;            \
    UINT32 ui4_pts;                    \
    UINT8  ui1_pts_delta;               

/*single payload*/
typedef struct _ASF_PAYLOAD_SINGLE_T
{
    ASF_PAYLOAD_COMMON_FIELDS

}ASF_PAYLOAD_SINGLE_T;

/*single payload,compressed*/
typedef struct _ASF_PAYLOAD_COM_SINGLE_T
{
    ASF_COM_PAYLOAD_COMMON_FIELDS
    
}ASF_PAYLOAD_COM_SINGLE_T;

typedef struct _ASF_INFO_PER_PAYLOAD_T
{
    ASF_PAYLOAD_COMMON_FIELDS
    
}ASF_INFO_PER_PAYLOAD_T;

typedef struct _ASF_INFO_PER_COM_PAYLOAD_T
{
    ASF_COM_PAYLOAD_COMMON_FIELDS
    
}ASF_INFO_PER_COM_PAYLOAD_T;

/*multiple payloads*/
typedef struct _ASF_PAYLOAD_MULTI_T
{
    /*payloader number:bit0~bit5, payload length type: bit6,bit7*/
    UINT8                   ui1_payload_flg;
    

}ASF_PAYLOAD_MULTI_T;

/*multiple payloads,compressed*/
typedef struct _ASF_PAYLOAD_COM_MULTI_T
{
    /*payloader number:bit0~bit5, payload length type: bit6,bit7*/
    UINT8                   ui1_payload_flg;
}ASF_PAYLOAD_COM_MULTI_T;

/*ASF Payload structure*/
typedef struct _ASF_PAYLOAD_DATA_T
{
    ASF_PAYLOAD_DATA_TYPE_E       e_payload_type;    
    union _ASF_PAYLOAD_TYPE_U
    {
        ASF_PAYLOAD_SINGLE_T      t_single_data;
        ASF_PAYLOAD_COM_SINGLE_T  t_com_single_data;
        ASF_PAYLOAD_MULTI_T       t_multi_data; 
        ASF_PAYLOAD_COM_MULTI_T   t_com_multi_data;
    }u;
    
    /*file offset of the first media object in the payload*/
    UINT64                        ui8_first_video_obj_offset;
    UINT8                         ui1_first_video_obj_len;
    BOOL                          b_video_exist;
    UINT8                         ui1_stream_id;
    UINT32                        ui4_media_offset;
}ASF_PAYLOAD_DATA_T;

/*ASF Packet structure*/
typedef struct _ASF_PACKET_OBJECT_T
{
    /*Error correction Flag*/
    UINT8                    ui1_err_correction_flg;
    /*Error data*/
    UINT8*                   pui1_err_data;
    
    /*Payload parsing info*/
    ASF_PAYLOAD_PARSE_INFO_T t_payload_parse_info;
    /*Payload data*/
    ASF_PAYLOAD_DATA_T       t_payload_data;
    
}ASF_PACKET_OBJECT_T;

/*Data: which store the actual movies*/
typedef struct _ASF_DATA_OBJECT_T
{
    UINT64      ui8_size;    
    ASF_GUID_T  t_file_id;
    UINT64      ui8_total_packets;
    UINT16      ui2_reserved;
    UINT64      ui8_data_packet_begin; /*first data packet file position*/

    BOOL        b_is_codec_support; /*for wmv8*/
} ASF_DATA_OBJECT_T;


/*Simple Index:this structure will be removed to Index Builder*/
typedef struct _ASF_SIMP_IDX_ENTRY_T
{
    UINT32 ui4_packet_number;
    UINT16 ui2_packet_count;

} ASF_SIMP_IDX_ENTRY_T;

typedef struct _ASF_SIMPLE_INDEX_OBJECT_T
{
    ASF_OBJ_COMMON
    ASF_GUID_T            t_file_id;
    UINT64                ui8_time_interval;     /*ns*/
    UINT32                ui4_max_packet_count;
    UINT32                ui4_index_entry_count;

    ASF_SIMP_IDX_ENTRY_T* pt_index_entry;

} ASF_SIMPLE_INDEX_OBJECT_T;

typedef struct _ASF_IDX_EACH_STRM_BLOCK_T
{
    UINT32                              ui4_entry_cnt;
    UINT64                              ui8_blk_pos;
    UINT32*                             pui4_entry_offset;

    /*struct _ASF_IDX_EACH_STRM_BLOCK_T*  pt_next;*/
} ASF_IDX_EACH_STRM_BLOCK_T;


typedef struct _ASF_IDX_BLOCK_T
{
    UINT32                      ui4_strm_blk_cnt;
    ASF_IDX_EACH_STRM_BLOCK_T*  pt_strm_blk; 
    /*struct _ASF_IDX_BLOCK_T    pt_next;*/
} ASF_IDX_BLOCK_T;

typedef struct _ASF_INDEX_OBJECT_T
{
    ASF_OBJ_COMMON
    UINT32                ui4_time_interval;   /*ms*/
    UINT16                ui2_specifier_cnt;
    UINT32                ui4_blk_cnt;

    VOID*                 pv_specifier_obj;

    ASF_IDX_BLOCK_T*      pt_blk_entry;

} ASF_INDEX_OBJECT_T;


/****************************************************************************
 * End Top-level asf objects definition
 ****************************************************************************/

/****************************************************************************
 * 2.Begin Header asf objects definition
 ****************************************************************************/

#define ASF_FILE_BROADCAST   0x01
#define ASF_FILE_SEEKABLE    0x02

/*File Properties Object (mandatory, only one)*/
typedef struct _ASF_FILE_PROPERTIES_OBJECT_T
{
    ASF_GUID_T t_file_id;
    UINT64     ui8_file_size;
    UINT64     ui8_create_date;
    UINT64     ui8_packets_count;
    UINT64     ui8_play_duration;
    UINT64     ui8_send_duration;
    UINT64     ui8_preroll;
    UINT32     ui4_flags;
    UINT32     ui4_min_packet_size;
    UINT32     ui4_max_packet_size;
    UINT32     ui4_max_bitrate;

} ASF_FILE_PROPERTIES_OBJECT_T;

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
typedef struct _ASF_BD_PKT_T
{
    UINT8                       ui1_stream_id;       /*In*/
    UINT32                      ui4_packet_size;   /*In*/
    UINT32                      ui4_pts;           /*In*/
    UINT32                      ui4_first_met_pkt; /*Out*/
    BOOL                        b_met_all_strms;   /*Out*/
} ASF_BD_PKT_T;
#endif 

typedef enum _ASF_STREAM_TYPE_E
{
    ASF_STRM_UNKNOWN,
    ASF_STRM_VIDEO_TYPE,
    ASF_STRM_AUDIO_TYPE,
    ASF_STRM_SUBTILE_TYPE,
    ASF_STRM_BUTT
    
}ASF_STREAM_TYPE_E;
/*Stream Property Object (mandatory one per stream )*/
typedef struct _ASF_STREAM_PROPERTIES_OBJECT_T
{
    BOOL       b_in_ex_stream; /*if si inside extended stream properties object*/
    ASF_GUID_T t_strm_type;
    ASF_GUID_T t_err_correct_type;
    UINT64     ui8_time_offset;
    UINT32     ui4_type_spec_len;
    UINT32     ui4_err_correct_len;
    UINT16     ui2_flags;
    /*begin get from i2_flags via bit operation*/
    UINT8      ui1_stream_number;
    BOOL       b_encrypt_flag;
    /*end get from i2_flags via bit operation*/
  
    UINT32     ui4_reserved;
    UINT8*     puil_type_spec_data;
    UINT8*     puil_err_correct_data;
} ASF_STREAM_PROPERTIES_OBJECT_T;

/*Header Extention Object (mandatory, one only)*/
typedef struct _ASF_HEADER_EX_OBJECT_T
{

    ASF_GUID_T   t_reserved1;
    UINT16       ui2_reserved2;
    UINT32       ui4_size;

} ASF_HEADER_EX_OBJECT_T;


#define ASF_CODEC_VIDEO    0x0001
#define ASF_CODEC_AUDIO    0x0002
#define ASF_CODEC_UNKNOW   0xffff

typedef struct _ASF_CODEC_ENTRY_T
{
    UINT16     ui2_type;
    /*asf spec is unicode so need transform when loading from a asf file*/
    UTF16_T*   pwsz_name;   
    /*asf spec is unicode so need transform when loading from a asf file*/
    UTF16_T*   pwsz_desc;   

    UINT16     ui2_info_len;
    UINT8*     pui1_info;
} ASF_CODEC_ENTRY_T;

/*Codec List Object (optional one only)*/
typedef struct _ASF_CODEC_LIST_OBJECT_T
{
    ASF_GUID_T          t_reserved;
    UINT32              ui4_codec_entry_count;
    ASF_CODEC_ENTRY_T*  pt_codec;

} ASF_CODEC_LIST_OBJECT_T;

/*Content Description Object (optional one only)*/
typedef struct _ASF_CONTENT_DESCRIPTION_OBJECT_T
{
    /*Note:the following fields are specified in unicodes  so need transfer when 
    loading from a asf file*/
    UTF16_T* pwsz_title;
    UTF16_T* pwsz_author;
    UTF16_T* pwsz_copyright;
    UTF16_T* pwsz_description;
    UTF16_T* pwsz_rating;
     
} ASF_CONTENT_DESCRIPTION_OBJECT_T;
/*Content Description Object (optional one only)*/
typedef enum _ASF_EXT_CONTENT_DES_NAME_T
{
    EXT_CONT_NAME_YEAR,
    EXT_CONT_NAME_ALBUM_TITLE,
    EXT_CONT_NAME_ALBUM_ARTIST,
    EXT_CONT_NAME_GEBRE,
        
    EXT_CONT_NAME_STEREOSCOPIC,
    EXT_CONT_NAME_STEREOSCOPIC_LAYOUT,
    EXT_CONT_NAME_STEREOSCOPIC_SKIP,
    EXT_CONT_NAME_STEREOSCOPIC_HALF_WIDTH,
    EXT_CONT_NAME_STEREOSCOPIC_HALF_HEIGHT,
    
    EXT_CONT_NAME_MAX,
} ASF_EXT_CONTENT_DES_NAME_T;   /*extended name list*/

typedef enum _ASF_DATA_TYPE
{
    DATA_TYPE_WC,             /*Unicode string*/
    DATA_TYPE_BYTE_ARR,       
    DATA_TYPE_BOOL,
    DATA_TYPEA_DWORD,
    DATA_TYPEA_QWORD,
    DATA_TYPE_WORD,
    
    DATA_TYPE_MAX,
} ASF_DATA_TYPE;

typedef struct _ASF_EXT_CONTENT_DES_NAME_VALUE_T
{
    CHAR*            pc_name; 
    ASF_DATA_TYPE    at_data_type; 
    UINT32           ui4_data_len;
    
    /*for extended content description*/
    union 
    {   
        VOID*                        pv_value;
        BOOL                         b_value;
        UINT32                       ui4_value;
        UINT64                       ui8_value;
        UINT16                       ui2_value;
    } u_data;
    
} ASF_EXT_CONTENT_DES_NAME_VALUE_T;

typedef struct _ASF_EXT_CONTENT_DES_OBJECT_T
{
    /*Note:the following fields are specified in unicodes  so need transfer when 
           loading from a asf file*/
    ASF_EXT_CONTENT_DES_NAME_VALUE_T    at_name_value[EXT_CONT_NAME_MAX];

    /*shortcut */
    UTF16_T* pwsz_year;
    UTF16_T* pwsz_album_title;
    UTF16_T* pwsz_album_artist;
    UTF16_T* pwsz_genre;
    
    UTF16_T*                            pwsz_stereoscopic_layout;
    BOOL                                b_stereo_scopic;
    BOOL b_half_width;
    BOOL b_half_height;
    MM_SCOPIC_TYPE_T e_scopic_type;
    
    UINT64  ui8_skip_dur;    
    
} ASF_EXT_CONTENT_DES_OBJECT_T;

/*Bitrate Properties Object (optional one only, recommended)*/
typedef struct _ASF_BITRATE_REC_T
{
    UINT8  ui1_stream_num;
    UINT32 ui4_bitrate;
    
} ASF_BITRATE_REC_T;

typedef struct _ASF_BITRATE_PROPERTIES_OBJECT_T
{
    UINT16             ui2_rec_count;
    ASF_BITRATE_REC_T* pt_record;
} ASF_BITRATE_PROPERTIES_OBJECT_T;

/*   
Note:the following fields are specified in unicodes  
   so need transfer when loading from a asf file
*/
typedef struct _ASF_CONTENT_DES_ENTRY_T
{
    CHAR* psz_name;
    INT8* pi1_value;
} ASF_CONTENT_DES_ENTRY_T;
 
/*Content Description Extention Object*/
typedef struct _ASF_CONTENT_DESCRIPTION_EX_OBJECT_T
{
    UINT16                   ui2_count;
    ASF_CONTENT_DES_ENTRY_T* pt_content_des; 
} ASF_CONTENT_DESCRIPTION_EX_OBJECT_T;

#if 0
/*Stream Bitrate property Object (optional but recommended, one only)*/
typedef struct _ASF_BITRATE_PROPERTIES_OBJECT_T
{
    UINT16 i2_rec_count;
    struct
    {
        UINT8      ui1_stream_number;
        UINT32     ui4_avg_bitrate;
    } at_bitrate[ASF_STREAM_NUM_MAX];
} ASF_BITRATE_PROPERTIES_OBJECT_T;
#endif
/*Content Encryption Object (optional one only)*/
typedef struct _ASF_CONTENT_ENCRYPTION_OBJECT_T
{
    /*Note:the following fields are specified in unicodes  so need transfer when 
    loading from a asf file*/
    UINT32     ui4_secret_data_len;
    UINT8*     pui1_secret_data;
    
    UINT32     ui4_protect_type_len;
    CHAR*      pi1_protect_type;

    UINT32     ui4_key_id_len;
    CHAR*      pi1_key_id;

    UINT32     ui4_lic_url_len;
    CHAR*      pi1_lic_url;
} ASF_CONTENT_ENCRYPTION_OBJECT_T;

/****************************************************************************
 * End Header asf objects definition
 ****************************************************************************/


/****************************************************************************
 * 3. Begin  Header Extention asf objects definition
 ****************************************************************************/

typedef struct _ASF_STRAMM_NAME_T
{
    UINT16     ui2_strm_name_lang_idx;
    /*spec. is WCHAR so need be transformed to show info*/
    UTF16_T*   pwsz_stream_name;
} ASF_STREAM_NAME_T;

typedef struct _ASF_PAYLOAD_EX_SYS_T
{
    ASF_GUID_T t_guid;     
    UINT16     ui2_size;    
    UINT32     ui4_ex_sys_info_len;
    UINT8*     pui1_ex_sys_info;
} ASF_PAYLOAD_EX_SYS_T;

/*extended stream object */
typedef struct _ASF_EX_STREAM_PROPERTIES_OBJECT_T
{
    UINT64               ui8_start_time;
    UINT64               ui8_end_time;

    /*Bukey model R,B,F,*/
    UINT32               ui4_bitrate;
    UINT32               ui4_buf_size;
    UINT32               ui4_init_buf_size;
    UINT32               ui4_alter_bitrate;
    UINT32               ui4_alter_buf_size;
    UINT32               ui4_alter_init_buf_size;
    
    UINT32               ui4_max_obj_size;
    /*be composed of reliable flag, seekable flag,
     resend live cleanflag, reserved flag*/
    UINT32                ui4_flags; 
    
    UINT16                ui2_stream_num;
    UINT16                ui2_language_idx;
    UINT64                ui8_average_time_per_frame;

    UINT16                ui2_stream_name_count;
    UINT16                ui2_payload_ex_sys_count;
    
    ASF_STREAM_NAME_T*    pt_stream_name;
    ASF_PAYLOAD_EX_SYS_T* pt_payload_ex_system;
    ASF_OBJ_COMMON_T*     pt_strm_properties;
    
} ASF_EX_STREAM_PROPERTIES_OBJECT_T;

/*languages list object*/
typedef struct _ASF_LANGUAGE_LIST_OBJECT_T
{
    UINT16           ui2_lang_count;
    ISO_639_LANG_T*  pt_languge;
}  ASF_LANGUAGE_LIST_OBJECT_T;


/*data type definition*/
#define ASF_DATA_TYPE_UNICODE_STR 0x0000
#define ASF_DATA_TYPE_BYTE_ARRAY  0x0001
#define ASF_DATA_TYPE_BOOL        0x0002
#define ASF_DATA_TYPE_DWORD       0x0003
#define ASF_DATA_TYPE_QWORD       0x0004
#define ASF_DATA_TYPE_WORD        0x0005

typedef struct _ASF_METADATA_REC_T
{
    
    UINT16     ui2_stream_num;
    UINT16     ui2_data_type; 
    /*ASCII-Unicode*/
    CHAR*      psz_name;
    UINT32     i_data_len;
    UINT8*     pui1_data;

} ASF_METADATA_REC_T;

/*metadata object*/
typedef struct _ASF_METADATA_OBJECT_T
{
    UINT32              ui4_record_count;
    ASF_METADATA_REC_T* pt_record;

} ASF_METADATA_OBJECT_T;

/*Compatiblity Object*/
typedef struct _ASF_COMPATIBILITY_OBJECT_T
{
    UINT8 ui1_profile;
    UINT8 ui1_mode;

} ASF_COMPATIBILITY_OBJECT_T;

#define ASF_INDEX_TYPE_NEAR_PAST_PACKET        1
#define ASF_INDEX_TYPE_NEAR_PAST_MEDIA_OBJ     2
#define ASF_INDEX_TYPE_NEAR_PAST_TIME_CODE     3

typedef struct _ASF_IDX_SPECIFERS_T
{
    UINT16 ui2_stream_num;
    UINT16 ui2_idx_type;
} ASF_IDX_SPECIFERS_T;
/*Index parameters Object*/
typedef struct _ASF_IDX_PARAMETERS_OBJECT_T
{

    UINT32               ui4_time_interval;
    UINT16               ui2_specifers_count;
    ASF_IDX_SPECIFERS_T* pt_specifers;
} ASF_IDX_PARAMETERS_OBJECT_T;

/****************************************************************************
 * End Header Extention asf objects definition
 ****************************************************************************/

/* simulate clip buffer */
#ifdef CLI_LVL_ALL
typedef struct _ASF_CLIP_OBJ
{
    MEDIA_FEEDER_T  t_src_feeder;   
    VOID*           pv_clip_buf;  /* point to file content*/
    INT64           i8_clip_len;  /*file length*/
    INT64           i8_buff_pos;  /*current position of the buffer*/
}ASF_CLIP_OBJ;
/*define error code*/
#define ASFR_CLIP_OK                 0
#define ASFR_CLIP_ERROR              1
/*feeder handle*/
#define ASF_CLIP_HANDLE_FOR_CLI      1
#endif





/***********************************************************************************
                       data declarations
***********************************************************************************/
extern const ASF_GUID_T gt_asf_object_null_guid;
extern const ASF_GUID_T gt_asf_object_header_guid;
extern const ASF_GUID_T gt_asf_object_data_guid;
extern const ASF_GUID_T gt_asf_object_file_properties_guid;
extern const ASF_GUID_T gt_asf_object_stream_properties_guid;
extern const ASF_GUID_T gt_asf_object_header_extension_guid;
extern const ASF_GUID_T gt_asf_object_stream_type_audio;
extern const ASF_GUID_T gt_asf_object_stream_type_video;
extern const ASF_GUID_T gt_asf_guid_no_error_correction;
extern const ASF_GUID_T gt_asf_guid_audio_spread;
extern const ASF_GUID_T gt_asf_guid_reserved_1;
extern const ASF_GUID_T gt_asf_guid_reserved_2;

extern const ASF_GUID_T gt_asf_guid_language_list;
extern const ASF_GUID_T gt_asf_object_codec_list_guid;
extern const ASF_GUID_T gt_asf_object_codec_list_reserved_guid;
extern const ASF_GUID_T gt_asf_object_content_description_guid;
extern const ASF_GUID_T gt_asf_object_ext_content_description_guid;
extern const ASF_GUID_T gt_asf_object_extended_stream_properties;

extern const ASF_GUID_T gt_asf_object_simple_index_guid;
extern const ASF_GUID_T gt_asf_object_metadata_guid;
extern const ASF_GUID_T gt_asf_object_index_paras_guid;
extern const ASF_GUID_T gt_asf_object_bitrate_properties_guid;
extern const ASF_GUID_T gt_asf_object_content_encryption_guid;
extern const ASF_GUID_T gt_asf_object_index_guid;

/*for extended AC3 codec*/
extern const ASF_GUID_T gt_asf_codec_ext_ac3_guid;

extern VOID _asf_get_guid( ASF_GUID_T* pt_guid, UINT8* puc_data );
extern INT32 _asf_cmp_guid( const ASF_GUID_T*     pt_guid1, 
                            const ASF_GUID_T*     pt_guid2 );
extern VOID _asf_guid_to_str( ASF_GUID_T* pt_guid, 
                              CHAR*       psz_str, 
                              UINT32*     pui4_len );

extern  VOID _asf_lang_rfc1766_to_639(UINT8*          psz_lang,
                                      UINT32          ui4_len,
                                      ISO_639_LANG_T* pt_iso_639_lang);
extern UINT32 _asf_get_value_by_len_type(UINT8** ppui1_head,
                                         UINT8   ui1_len_type);
extern UINT32 _asf_wide_char_2_str( UTF16_T* pui2_wide_char, 
                                    UINT32   ui4_len,
                                    CHAR*    pc_str, 
                                    UINT32   ui4_buf_len );
extern INT32 _asf_bit_map_set( UINT8* pui1_map, UINT32 ui4_len, UINT32 ui4_idx );
extern BOOL _asf_bit_map_is_set( UINT8* pui1_map, UINT32 ui4_len, UINT32 ui4_idx );

/********************simulate clip buffer *************************************/
#ifdef CLI_LVL_ALL
extern UINT32 _cli_asf_mcopybytes_fct( VOID*  pv_feeder_obj, 
                                       VOID*  pv_buf, 
                                       SIZE_T z_buf_leng, 
                                       SIZE_T z_copy_size,
                                       UINT32* pui4_size);
extern INT32 _cli_asf_msetpos_fct( VOID* pv_feeder_obj, 
                                    INT64 i8_offset, 
                                    UINT8 ui1_whence);
extern INT32 _cli_asf_mgetpos_fct( VOID* pv_feeder_obj, UINT64* pui8_cur_pos );
extern INT32 _cli_asf_get_obj( CHAR*          psz_path_name , 
                                ASF_CLIP_OBJ** ppt_feeder_obj );
extern VOID _cli_asf_free_obj(ASF_CLIP_OBJ** ppt_clip_obj);
#endif



#endif /*_ASF_PARSER_COMMON_H_*/

