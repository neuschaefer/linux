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

#ifndef _ASF_MINFO_PARSER_H_
#define _ASF_MINFO_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/minfo/minfo.h"
#include "asf_parser_common.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../../mfmtrecg/u_mfmtrecg.h"
#include "../minfo.h"
#include "asf_parser_common.h"

#endif

#ifndef __EMBEDED_STREAM_PROPER_OBJ__
#define __EMBEDED_STREAM_PROPER_OBJ__
#endif
/*define asf parsing buffer length*/
#define ASF_MINFO_INIT_BUFF_LEN       5000
#define ASF_MINFO_SET_FILE_POS_ERR    (-1)
#define ASF_MINFO_SET_FILE_POS_BSY    (-4)

#define ASF_SET_CUR_PARSE_POS(pt_minfo_obj, offset, i4_ret)   \
do                                                    \
{                                                     \
    ((MINFO_ASF_INSTANCE_INFO_T*)((pt_minfo_obj)->pv_handler_obj))->ui8_cur_parser_offset  \
    += (offset);                                      \
    i4_ret = ((pt_minfo_obj)->t_input_fct_tbl.pf_set_pos)((VOID*)(pt_minfo_obj),           \
    (INT64)((MINFO_ASF_INSTANCE_INFO_T*)((pt_minfo_obj)->pv_handler_obj))->ui8_cur_parser_offset, \
    FM_SEEK_BGN);                                     \
} while (0)

#define ASF_SET_CUR_PARSE_POS_CHK_BSY(pt_minfo_obj, offset, i4_ret)   \
do                                                    \
{                                                     \
    ((MINFO_ASF_INSTANCE_INFO_T*)((pt_minfo_obj)->pv_handler_obj))->ui8_cur_parser_offset  \
    += (offset);                                      \
    i4_ret = ((pt_minfo_obj)->t_input_fct_tbl.pf_set_pos)((VOID*)(pt_minfo_obj),           \
    (INT64)((MINFO_ASF_INSTANCE_INFO_T*)((pt_minfo_obj)->pv_handler_obj))->ui8_cur_parser_offset, \
    (MEDIA_SEEK_BGN | MEDIA_SEEK_CHK));                                     \
} while (0)


/*object operation callback*/
typedef INT32(*asf_load_obj_func)( MINFO_OBJ_T*      pt_minfo_obj, 
                                   ASF_OBJ_COMMON_T* pt_this_obj );
typedef  INT32(*asf_free_obj_func)( MINFO_OBJ_T*       pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T** ppt_this_obj );
#ifdef CLI_LVL_ALL
typedef VOID(*asf_dump_object)( VOID* pv_obj_specific );
#endif

/*Object operation e.g. load/free object*/
typedef struct _MINFO_ASF_OBJ_OPER_T
{
    ASF_OBJ_TYPE_E     e_obj_type;
    const ASF_GUID_T*  pt_guid;
    const CHAR*        psz_description;
    asf_load_obj_func  pf_load_obj;
    asf_free_obj_func  pf_free_obj;
    #ifdef CLI_LVL_ALL
    asf_dump_object    pf_dump_obj;
    #endif
} MINFO_ASF_OBJ_OPER_T;


/*Video stream info*/
#define ASF_FOUR_CC(a,b,c,d)  ((((UINT32)(d))<<24)  \
                               | ((UINT32)((c)<<16)) \
                               | ((UINT32)((b)<<8))  \
                               | ((UINT32)(a)))
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
#define ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_flag, ui1_lmask, ui1_hmask, ui4_data) \
do{\
    UINT8 ui1_mask = ui1_lmask + ui1_hmask; \
    if((ui1_flag & ui1_mask) == ui1_lmask/*byte*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_input1(pt_minfo_obj, (UINT8*)(&ui4_data));\
    }\
    else if((ui1_flag & ui1_mask) == ui1_hmask/*word*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_input2(pt_minfo_obj, (UINT16*)(&ui4_data));\
    }\
    else if((ui1_flag & ui1_mask) == ui1_mask/*dword*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_input4(pt_minfo_obj, &ui4_data);\
    }\
    else\
    {\
        ui4_data = 0;\
    }\
}while(FALSE)

#define ASF_SKIP_DATA_BY_MASK(pt_minfo_obj, ui1_flag, ui1_lmask, ui1_hmask) \
do{\
    UINT8 ui1_mask = ui1_lmask + ui1_hmask; \
    if((ui1_flag & ui1_mask) == ui1_lmask/*byte*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos(pt_minfo_obj, 1, MEDIA_SEEK_CUR);\
    }\
    else if((ui1_flag & ui1_mask) == ui1_hmask/*word*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos(pt_minfo_obj, 2, MEDIA_SEEK_CUR);\
    }\
    else if((ui1_flag & ui1_mask) == ui1_mask/*dword*/)\
    {\
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos(pt_minfo_obj, 4, MEDIA_SEEK_CUR);\
    }\
}while(FALSE)
#endif

typedef struct _MINF_BITMAPINFOHEADER_T
{
    UINT32  ui4_size; 
    UINT32  ui4_width; 
    UINT32  ui4_height; 
    UINT16  ui2_reserved; 
    UINT16  ui2_bit_count; 
    UINT32  ui4_compression; 
    UINT32  ui4_size_image; 
    UINT32  ui4_xpels_per_meter; 
    UINT32  ui4_ypels_per_meter; 
    UINT32  ui4_clr_used; 
    UINT32  ui4_clr_important;
    UINT8*  pui1_code_specific;
} MINF_BITMAPINFOHEADER_T; 

#define ASF_WAVE_FORMAT_MP3           0x0055
#define ASF_WAVE_FORMAT_MPEG          0x0050

#define ASF_WAVE_FORMAT_PCM1          0x0001
#define ASF_WAVE_FORMAT_PCM2          0x0002
#define ASF_WAVE_FORMAT_PCMA          0x0006
#define ASF_WAVE_FORMAT_PCMU          0x0007
#define ASF_WAVE_FORMAT_PCM11         0x0011

#define ASF_WAVE_FORMAT_WMA_1         0x0160 
#define ASF_WAVE_FORMAT_WMA_2         0x0161 
#define ASF_WAVE_FORMAT_WMA_PRO       0x0162 
#define ASF_WAVE_FORMAT_WMA_LOSSLESS  0x0163
#define ASF_WAVE_FORMAT_AC3           0x2000
#define ASF_WAVE_FORMAT_AAC           0x00FF

/*for extended format*/
#define ASF_WAVE_FORMAT_EXT           0xFFFE


/*Audio stream info*/
typedef struct _MINO_WAVEFORMATEX_T
{
    UINT16  ui2_format_tag;
    UINT16  ui2_channels;
    UINT32  ui4_samples_per_sec;
    UINT32  ui4_avg_bytes_per_sec;
    UINT16  ui2_block_align;
    UINT16  ui2_bits_per_sample;
    UINT16  ui2_size;
    UINT8*  pui1_code_specific;
} MINO_WAVEFORMATEX_T;

/*Audio stream correction info*/
typedef struct _MINO_SPREAD_AUDIO_T
{
    UINT8   ui1_span;
    UINT16  ui2_vir_pack_len;
    UINT16  ui2_vir_chunk_len;
    UINT16  ui2_silence_len;
    UINT8*  pui1_silence;
} MINO_SPREAD_AUDIO_T;

typedef struct _MINFO_ASF_PER_VID_STRM_INF_T 
{
    /*type spcific info*/
    MINFO_ASF_VIDEO_STM_ATRBT_T t_vid_strm_atrbt;
    /*only need pui1_code_specific of t_bitmap_info_header,
     but still store other info herer*/
    UINT16                      ui2_bitmap_header_size;
    MINF_BITMAPINFOHEADER_T     t_bitmap_info_header;
    
    /*video no error correction inof*/
}MINFO_ASF_PER_VID_STRM_INF_T;

typedef struct _MINFO_ASF_PER_AUD_STRM_INF_T 
{   
    /*type spcific info*/
    MINFO_ASF_AUDIO_STM_ATRBT_T t_aud_strm_atrbt;
    /*the rest fields are minfo don't retrieve but still sotre here*/
    /*
    UINT32                      ui4_bytes_per_second;
    UINT32                      ui4_code_specific_len;
    UINT8*                      pui1_code_specific;
    */
    /*error correction info*/
    MINO_SPREAD_AUDIO_T         t_error_correction;
}MINFO_ASF_PER_AUD_STRM_INF_T;


/*Stream info structure*/
typedef struct _MINFO_ASF_PER_STRM_INFO_T 
{
    /*stream properties*/
    ASF_STREAM_PROPERTIES_OBJECT_T*    pt_stream_obj;
    /*extended stream properties*/
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_obj;
    UINT32                             ui4_stream_number;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /* for check the bad interleaved*/
    UINT32                             ui4_present_time;
#endif
    /*
     including type specific and error correction info
     MINFO_ASF_PER_AUD_STRM_INF_T*   or 
     MINFO_ASF_PER_VID_STRM_INF_T* 
    */
    VOID* pv_type_info;
    
    struct _MINFO_ASF_PER_STRM_INFO_T*  pt_next_strm;
}MINFO_ASF_PER_STRM_INFO_T;

/*Stream info structure*/
typedef struct _MINFO_ASF_ALL_STRM_INFO_T 
{
    /*each stream info*/
    MINFO_ASF_PER_STRM_INFO_T* p_strm_info_head;
    UINT8                      ui1_audio_strm_num;
    UINT8                      ui1_video_strm_num;
    UINT8                      ui1_hidden_video_strm_num;
    UINT8                      ui1_hidden_audio_strm_num;
}MINFO_ASF_ALL_STRM_INFO_T;


/*store all parsered infomation */
typedef struct _MINFO_ASF_INSTANCE_INFO_T
{
    /* The header to the asf tree which describe the whole file*/
    MINFO_ASF_ROOT_T t_root;

    /*******************************************************
                      begin parser result:                
    *******************************************************/
    /*Header Object*/
    ASF_HEADER_OBJECT_T               t_header_object;

    /* File Object*/
    ASF_FILE_PROPERTIES_OBJECT_T      t_file_object;

    /*Header extention object*/
    ASF_HEADER_EX_OBJECT_T            t_header_ex;

    /*all Stream info:including STREAM PROPERTIES OBJECT  and 
     Extended STREAM PROPERTIES information*/
    MINFO_ASF_ALL_STRM_INFO_T         t_all_strm_info;   

    /*Codec List Object*/
    ASF_CODEC_LIST_OBJECT_T           t_codec_list;

    /*Language List Object*/
    ASF_LANGUAGE_LIST_OBJECT_T        t_language;
    /* Content Description Object*/
    ASF_CONTENT_DESCRIPTION_OBJECT_T  t_content_des;
    
    /* Extended Content Description Object*/
    ASF_EXT_CONTENT_DES_OBJECT_T      t_ext_content_des;
    
    ASF_BITRATE_PROPERTIES_OBJECT_T   t_bitrate_properties;

    /*Content encryption:DRM*/
    ASF_CONTENT_ENCRYPTION_OBJECT_T   t_content_encryption;
    
    /*Index Parameters Object*/
    ASF_IDX_PARAMETERS_OBJECT_T       t_idx_paras;
    
    /*Data object*/
    ASF_DATA_OBJECT_T                 t_data_object;
    /*simple Index Object*/
    ASF_SIMPLE_INDEX_OBJECT_T         t_simple_index_object;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    ASF_BD_PKT_T                      t_bd_pkt;
    BOOL                              b_is_bad_interleaved;
    /* Current parser packet count */
    UINT32                            ui4_cur_packet_count;
#endif     

    /*******************************************************
                      end parser result                
     ******************************************************/

    /*index info , provided for index builder*/
    MINFO_TYPE_IDX_INFO_T              t_idx_info;
    BOOL                               b_hd_content;
    /* Current parser offset, file base */
    UINT64                             ui8_cur_parser_offset;
    /*Header buffer size*/
    UINT32                             ui4_buf_length;
    /*store current parsed object buffer*/
    UINT8*                             pui1_header_buf;

    /* server not support seek*/
    BOOL                               b_non_seekable;


}MINFO_ASF_INSTANCE_INFO_T;

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
typedef struct _MINFO_ASF_BAD_THRESHOLD_TBL
{
    /* the packet size*/
    UINT32  ui1_packet_len;
    /* the threshold about pts*/
    UINT32  ui1_bad_threshold;
}MINFO_ASF_BAD_THRESHOLD_TBL;
#endif
/*-----------------------------------------------------------------------------
                    variable declarations
 ----------------------------------------------------------------------------*/

/*define all objects operations*/
extern const MINFO_ASF_OBJ_OPER_T g_at_asf_obj_oper[];

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern UINT32 _asf_get_minfo_oper_tab_len(VOID);

extern MINFO_INFO_AUDIO_ENC_T _asf_audio_codec( UINT32 ui4_4cc );
extern MINFO_INFO_VIDEO_ENC_T _asf_video_codec( UINT32 ui4_4cc );
extern MM_SCOPIC_TYPE_T _asf_stereoscopic_layout( 
                                                    const UTF16_T* pui2_str,
                                                    UINT32         ui4_size );
extern VOID _asf_strm_info_free(MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info);

extern MINFO_ASF_PER_STRM_INFO_T* _asf_search_strm_info_by_id
                                    ( MINFO_OBJ_T* pt_minfo_obj,
                                      UINT16       ui2_strm_id );
extern INT32 _asf_get_strm_atrbt_by_idx(
                                 MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info,
                                 ASF_STREAM_TYPE_E          e_strm_type,
                                 UINT32                     ui4_count,
                                 MINFO_INFO_T*              pt_minfo_rec);

extern INT32 _asf_get_generic_info ( MINFO_OBJ_T*  pt_minfo_obj,
                                     MINFO_INFO_T* pt_minfo_rec);

extern VOID _asf_set_audio_strm_lang(
                                   MINFO_OBJ_T*               pt_minfo_obj,
                                   MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info);

extern VOID _asf_set_strm_packet_size( 
                                MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info,
                                UINT32                     ui4_strm_id,
                                UINT32                     ui4_per_packe_size );

extern ASF_STREAM_TYPE_E _asf_get_stream_type( MINFO_OBJ_T* pt_minfo_obj,
                                               UINT16       ui2_strm_id );


extern INT32 _asf_load_file_properties ( MINFO_OBJ_T*      pt_minfo_obj, 
                                         ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_file_object ( MINFO_OBJ_T*       pt_minfo_obj, 
                                     ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_stream_properties ( MINFO_OBJ_T*      pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_stream_properties ( MINFO_OBJ_T*       pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_language_list ( MINFO_OBJ_T*      pt_minfo_obj,
                                       ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_language_list ( MINFO_OBJ_T*       pt_minfo_obj, 
                                       ASF_OBJ_COMMON_T** ppt_this_obj );

extern INT32 _asf_load_codec_list ( MINFO_OBJ_T*      pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_codec_list ( MINFO_OBJ_T*       pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_ex_stream_properties( MINFO_OBJ_T*      pt_minfo_obj, 
                                             ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_ex_stream_properties ( MINFO_OBJ_T*       pt_minfo_obj, 
                                              ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_index_parameters( MINFO_OBJ_T*      pt_minfo_obj, 
                                         ASF_OBJ_COMMON_T* pt_this_obj);
extern INT32 _asf_free_index_parameters( MINFO_OBJ_T*       pt_minfo_obj, 
                                         ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_header_extention( MINFO_OBJ_T*      pt_minfo_obj, 
                                         ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_header_extention( MINFO_OBJ_T*       pt_minfo_obj, 
                                         ASF_OBJ_COMMON_T** ppt_this_obj );

extern INT32 _asf_load_object( MINFO_OBJ_T*      pt_minfo_obj, 
                               ASF_OBJ_COMMON_T* pt_this_obj,
                               ASF_OBJ_COMMON_T* pt_parent_obj );
extern INT32  _asf_free_composite_object( MINFO_OBJ_T*       pt_minfo_obj, 
                                          ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32  _asf_load_header_object( MINFO_OBJ_T*       pt_minfo_obj, 
                                       ASF_OBJ_COMMON_T*  pt_this_obj );

extern INT32  _asf_free_header_object( MINFO_OBJ_T*       pt_minfo_obj, 
                                       ASF_OBJ_COMMON_T** ppt_this_obj );

#if 0
extern INT32  _asf_load_packet_object( VOID*                pv_obj, 
                                       asf_copybytes_fct    pf_copy,
                                       asf_setpos_fct       pf_setpos,
                                       asf_getpos_fct       pf_getpos,
                                       UINT64               ui8_offset,
                                       ASF_PACKET_OBJECT_T* pt_packet);
extern INT32 _asf_wmv_supported_test( MINFO_OBJ_T*  pt_minfo_obj,
                                      UINT64        ui8_data_packet_begin,
                                      UINT32        ui4_per_packet_size,
                                      BOOL*         pb_supported);
#endif

extern INT32  _asf_load_data_object( MINFO_OBJ_T*      pt_minfo_obj,
                                     ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32  _asf_free_data_object( MINFO_OBJ_T*       pt_minfo_obj,
                                     ASF_OBJ_COMMON_T** ppt_this_obj );


extern INT32 _asf_load_content_description( MINFO_OBJ_T*      pt_minfo_obj, 
                                            ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_content_description ( MINFO_OBJ_T*       pt_minfo_obj, 
                                             ASF_OBJ_COMMON_T** ppt_this_obj );
extern INT32 _asf_load_ext_content_description( MINFO_OBJ_T*      pt_minfo_obj, 
                                            ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_ext_content_description ( MINFO_OBJ_T*       pt_minfo_obj, 
                                             ASF_OBJ_COMMON_T** ppt_this_obj );

extern INT32 _asf_load_bitrate_properties( MINFO_OBJ_T*      pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_bitrate_properties( MINFO_OBJ_T*       pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T** ppt_this_obj );

extern INT32 _asf_load_content_encryption( MINFO_OBJ_T*      pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T* pt_this_obj );
extern INT32 _asf_free_content_encryption( MINFO_OBJ_T*       pt_minfo_obj, 
                                           ASF_OBJ_COMMON_T** ppt_this_obj );

extern VOID  _asf_free_dynamic_resource( MINFO_OBJ_T* pt_minfo_obj );

extern INT32 asf_minfo_object_parse ( MINFO_OBJ_T* pt_minfo_obj );


#endif /* _ASF_MINFO_PARSER_H_ */

