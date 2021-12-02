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
 * asf_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2013/01/04 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the asf parser function
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"  {
#endif


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
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/mm_util.h"
#include "mutil/mm_util_input.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "unicode/x_uc_str.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "asf_parser_common.h"
#include "asf_minfo_dump.h"
#include "asf_wmv8_intra.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "x_minfo.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"
#include "x_uc_str.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "asf_parser_common.h"
#include "asf_minfo_dump.h"
#include "asf_wmv8_intra.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data definition
 ----------------------------------------------------------------------------*/
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
#define MINFOR_ASF_BAD_INTERLEAVED_CHECK_PKG_NUM               (4000)
#define MINFOR_ASF_BAD_INTERLEAVED_MAX_VID_DIFFERENCE          (2000)
#define MINFOR_ASF_BAD_INTERLEAVED_MAX_AUD_DIFFERENCE          (10000)
#define ASF_MASK_0                            (0x01)
#define ASF_MASK_1                            (0x02)
#define ASF_MASK_2                            (0x04)
#define ASF_MASK_3                            (0x08)
#define ASF_MASK_4                            (0x10)
#define ASF_MASK_5                            (0x20)
#define ASF_MASK_6                            (0x40)
#define ASF_MASK_7                            (0x80)
#define ASF_PKG_SMALL                         (5001)
#define ASF_PKG_NORML                         (8001)
#define ASF_PKG_BIG                           (10001)
#define ASF_DIF_BIG                           (2000)
#define ASF_DIF_NORML                         (1700)
#define ASF_DIF_SMALL                         (1500)

const static MINFO_ASF_BAD_THRESHOLD_TBL s_bad_threshold_tbl[] =
{
    {ASF_PKG_SMALL,  ASF_DIF_BIG},
    {ASF_PKG_NORML,  ASF_DIF_NORML},
    {ASF_PKG_BIG,    ASF_DIF_SMALL}
};
#endif

/*define all objects operations*/
const MINFO_ASF_OBJ_OPER_T g_at_asf_obj_oper[] =
{
    { 
        ASF_OBJ_UNKNOWN,              
        &gt_asf_object_null_guid,                 
        "Unknown Object",             
        NULL, 
        NULL, 
        #ifdef CLI_LVL_ALL
        NULL
        #endif
    },
    {
        ASF_OBJ_HEADER,              
        &gt_asf_object_header_guid,               
        "Header Object",             
        _asf_load_header_object, 
        _asf_free_header_object, 
        #ifdef CLI_LVL_ALL
        _asf_dump_header_object
        #endif
    },
    {
        ASF_OBJ_DATA,                
        &gt_asf_object_data_guid,                 
        "Data Object",               
        _asf_load_data_object, 
        _asf_free_data_object, 
        #ifdef CLI_LVL_ALL
        _asf_dump_data_object
        #endif
    },
    { 
        ASF_OBJ_SIMPLE_INDEX,        
        &gt_asf_object_simple_index_guid,         
        "Simple Index Object",              
        NULL, 
        NULL,
        #ifdef CLI_LVL_ALL
        NULL
        #endif
    },
    {
        ASF_OBJ_INDEX,        
        &gt_asf_object_index_guid,         
        "Index Object",              
        NULL, 
        NULL,
        #ifdef CLI_LVL_ALL
        NULL
        #endif
    },
    {
        ASF_OBJ_FILE_PROPERTIES,     
        &gt_asf_object_file_properties_guid,      
        "File Properties Object",     
        _asf_load_file_properties, 
        _asf_free_file_object,
        #ifdef CLI_LVL_ALL
        _asf_dump_file_object
        #endif
    },
    { 
        ASF_OBJ_STREAM_PROPERTIES,    
        &gt_asf_object_stream_properties_guid,    
        "Stream Properties Object",   
        _asf_load_stream_properties, 
        _asf_free_stream_properties, 
        #ifdef CLI_LVL_ALL
        _asf_dump_stream_properties
        #endif
    },
    { 
        ASF_OBJ_HEADER_EXTENSION,     
        &gt_asf_object_header_extension_guid,     
        "Header Extention Object",    
        _asf_load_header_extention, 
        _asf_free_header_extention,
        #ifdef CLI_LVL_ALL
        _asf_dump_header_extention
        #endif
    },
    { 
        ASF_OBJ_EX_STREAM_PROPERTIES,         
        &gt_asf_object_extended_stream_properties,
        "Extended Stream Properties Object",  
        _asf_load_ex_stream_properties, 
        _asf_free_ex_stream_properties,    
        #ifdef CLI_LVL_ALL
        _asf_dump_ex_stream_properties
        #endif
    },
    { 
        ASF_OBJ_LANGUAGE_LIST,       
        &gt_asf_guid_language_list,          
        "Language List Object",         
        _asf_load_language_list, 
        _asf_free_language_list, 
        #ifdef CLI_LVL_ALL
        _asf_dump_language_list
        #endif
    },
    {
        ASF_OBJ_CODEC_LIST,          
        &gt_asf_object_codec_list_guid,           
        "Codec List Object",          
        _asf_load_codec_list, 
        _asf_free_codec_list, 
        #ifdef CLI_LVL_ALL
        _asf_dump_codec_list
        #endif
    },
    { 
        ASF_OBJ_CONTENT_DESCRIPTION, 
        &gt_asf_object_content_description_guid,  
        "Content Description Object", 
        _asf_load_content_description, 
        _asf_free_content_description,
        #ifdef CLI_LVL_ALL
        _asf_dump_content_description
        #endif
    },
    { 
        ASF_OBJ_EXT_CONTENT_DESCRIPTION, 
        &gt_asf_object_ext_content_description_guid,  
        "Extended Content Description Object", 
        _asf_load_ext_content_description, 
        _asf_free_ext_content_description,
        #ifdef CLI_LVL_ALL
        _asf_dump_ext_content_description
        #endif
    },
    
    { 
        ASF_OBJ_BITRATE_PROPERTIES, 
        &gt_asf_object_bitrate_properties_guid,  
        "Bitrate Properties Object", 
        _asf_load_bitrate_properties, 
        _asf_free_bitrate_properties,
        #ifdef CLI_LVL_ALL
        _asf_dump_bitrate_properties
        #endif
    },
    {
        ASF_OBJ_IDX_PARAS,           
        &gt_asf_object_index_paras_guid,          
        "Index Parameters Object",    
        _asf_load_index_parameters, 
        NULL, 
        #ifdef CLI_LVL_ALL
        NULL
        #endif
    },
    {
        ASF_OBJ_CONTENT_ENCRYPTION,           
        &gt_asf_object_content_encryption_guid,          
        "Content Encryption Object",    
        _asf_load_content_encryption, 
        _asf_free_content_encryption, 
        #ifdef CLI_LVL_ALL
        _asf_dump_content_encryption,
        #endif
    },

    { 
        ASF_OBJECT_BUTT,             
        &gt_asf_object_null_guid,                 
        "Unknow Object",              
        NULL, 
        NULL, 
        #ifdef CLI_LVL_ALL
        NULL
        #endif
    }
        
};

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  _asf_get_minfo_oper_tab_len
 *
 * Description: get minfo oper table length
 *
 * Inputs:   NULL 
 *          
 * Outputs:  NULL 
 * 
 * Returns:  minfo table length
 *
 * Note:     NULL
 *
 * Author :  lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
UINT32 _asf_get_minfo_oper_tab_len(VOID)
{
    return (sizeof(g_at_asf_obj_oper) / sizeof(MINFO_ASF_OBJ_OPER_T));
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_audio_codec
 *
 * Description: transfert audio FourCC to our private enumerate value
 *
 * Inputs:   ui4_4cc 
 *          
 * Outputs:  NULL 
 * 
 * Returns:  audio codec type enumerator
 *
 * Note:     NULL
 *
 * Author :  lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/

MINFO_INFO_AUDIO_ENC_T _asf_audio_codec( UINT32 ui4_4cc )
{

    switch (ui4_4cc)
    {
        case ASF_WAVE_FORMAT_MP3:
        case ASF_WAVE_FORMAT_MPEG:
             return MINFO_INFO_AUD_ENC_MPEG_1;
        case ASF_WAVE_FORMAT_WMA_1:
             return MINFO_INFO_AUD_ENC_WMA_V1;
        case ASF_WAVE_FORMAT_WMA_2:
             return MINFO_INFO_AUD_ENC_WMA_V2;
        case ASF_WAVE_FORMAT_WMA_PRO:
             return MINFO_INFO_AUD_ENC_WMA_PRO;
        /*     
        case ASF_WAVE_FORMAT_WMA_LOSSLESS:
             return MINFO_INFO_AUD_ENC_WMA_LOSSLESS;
             */
        case ASF_WAVE_FORMAT_PCM1:
            return MINFO_INFO_AUD_ENC_PCM;
        case ASF_WAVE_FORMAT_PCM2:
            return MINFO_INFO_AUD_ENC_ADPCM;
        case ASF_WAVE_FORMAT_PCMA:
            return MINFO_INFO_AUD_ENC_A_PCM;
        case ASF_WAVE_FORMAT_PCMU:
            return MINFO_INFO_AUD_ENC_U_PCM;    
        case ASF_WAVE_FORMAT_PCM11:
            return MINFO_INFO_AUD_ENC_DVI_ADPCM;
        case ASF_WAVE_FORMAT_AC3:
            return MINFO_INFO_AUD_ENC_AC3;
        case ASF_WAVE_FORMAT_AAC:
            return MINFO_INFO_AUD_ENC_AAC;
        default: 
             DBG_INFO(("_asf_audio_codec = %x\r\n", ui4_4cc));
             return MINFO_INFO_AUD_ENC_UNKNOWN;

    }
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_video_codec
 *
 * Description: transfert video FourCC to our private enumerate value
 *
 * Inputs:  ui4_4cc 
 *          
 * Outputs: NULL 
 * 
 * Returns: video codec type enumerator
 *
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/

MINFO_INFO_VIDEO_ENC_T _asf_video_codec( UINT32 ui4_4cc )
{

    switch (ui4_4cc)
    {
        /*case ASF_FOUR_CC('W','M','V','1'): 
             return MINFO_INFO_VID_ENC_WMV1;
        case ASF_FOUR_CC('W','M','V','2'):
             return MINFO_INFO_VID_ENC_WMV2;*/  /*mark for no license of MS*/
        case ASF_FOUR_CC('W','M','V','3'):
             return MINFO_INFO_VID_ENC_WMV3;
            
        case ASF_FOUR_CC('W','V','C','1'):
        case ASF_FOUR_CC('W','M','V','A'):
             return MINFO_INFO_VID_ENC_WVC1;
        case ASF_FOUR_CC('M', 'P', '4', '3'):  /*Microsoft MPEG-4 v3*/
        case ASF_FOUR_CC('m', 'p', '4', '3'):
             return MINFO_INFO_VID_ENC_DIVX_311;
             
        case ASF_FOUR_CC('m', 'j', 'p', 'g'):    
        case ASF_FOUR_CC('M', 'J', 'P', 'G'):
             return MINFO_INFO_VID_ENC_MJPG;
        case ASF_FOUR_CC('D', 'X', '5', '0'): 
             return MINFO_INFO_VID_ENC_DIVX_5;
        case ASF_FOUR_CC('D', 'I', 'V', 'X'): 
             return MINFO_INFO_VID_ENC_DIVX_4;
        case ASF_FOUR_CC('M', 'P', '4', 'S'):
			return MINFO_INFO_VID_ENC_MPEG_4;
        case ASF_FOUR_CC('X', 'V', 'I', 'D'):    
             return MINFO_INFO_VID_ENC_XVID;
        case ASF_FOUR_CC('H', '2', '6', '4'): 
             return MINFO_INFO_VID_ENC_H264;
        default: 
             DBG_INFO(("_asf_video_codec = %x\r\n", ui4_4cc));
             return MINFO_INFO_VID_ENC_UNKNOWN;
    }
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_stereoscopic_layout
 *
 * Description: transfert audio stereoscopic layout to our private enumerate value
 *
 * Inputs:   ui4_layout : stereoscopic layout
 *               ui4_len :    
 *
 * Outputs:  NULL 
 * 
 * Returns:  stereoscopic layouttype enumerator
 *
 * Note:     NULL
 *
 * Author :  lianming lin(mtk40234)
 * 
 * History:
 *   (1)2010-11-22 : initial

 ----------------------------------------------------------------------------*/
MM_SCOPIC_TYPE_T _asf_stereoscopic_layout( const UTF16_T* pui2_str, UINT32 ui4_size )
{
    UINT32      ui4_idx;
    UINT32      ui4_len;
    CHAR        ac_str[128];
    static const  CHAR* ac_StereoscopicLayout[MM_SCOPIC_MS + 1] = 
    {
        "Unknown",
        "SideBySideRF",
        "SideBySideLF",
        "OverUnderRT",
        "OverUnderLT",
        "DualStream",
        "MultiStream",
    };
    
    ui4_len = ui4_size / 2;
    
    /*get ext_dexcription name*/
    _asf_wide_char_2_str( ( UTF16_T* )pui2_str, ui4_len, ac_str, 128 );

    for ( ui4_idx = 0; ui4_idx <= MM_SCOPIC_MS; ++ui4_idx )
    {
        if ( x_strlen( ac_StereoscopicLayout[ui4_idx] ) == 
             x_strlen( ( const CHAR* )ac_str ) )
        {
            if ( !x_strncmp( ac_StereoscopicLayout[ui4_idx], ( const CHAR* )ac_str, ui4_len ) )
            {
                break;
            }
        }
        else
        {
            continue;
        }
    }

    if ( ui4_idx <= MM_SCOPIC_MS )
    {
        return ( MM_SCOPIC_TYPE_T )ui4_idx;
    }
    else
    {
        return MM_SCOPIC_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_strm_info_free
 *
 * Description: This function will free all resourece allocated for steam info
 *
 * Inputs:  pt_all_strm_info : specifies the stream info poniter
 *          
 * Outputs: pt_minfo_rec, return stream attributes 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_get_generic_info ( MINFO_OBJ_T*  pt_minfo_obj,
                              MINFO_INFO_T* pt_minfo_rec)
{
    MINFO_TYPE_GENERIC_T*             pt_generic;
    MINFO_ASF_INSTANCE_INFO_T*        pt_asf_instance_obj; 
    ASF_FILE_PROPERTIES_OBJECT_T*     pt_file_object;  /* File Object*/
    ASF_CONTENT_ENCRYPTION_OBJECT_T*  pt_content_encryption;
    ASF_EXT_CONTENT_DES_OBJECT_T*     pt_ext_content_des;

    if ((NULL == pt_minfo_obj) || (NULL == pt_minfo_rec))
    {
        DBG_INFO(("_asf_get_generic_info:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_get_generic_info:asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pt_file_object = &(pt_asf_instance_obj->t_file_object);

    /*get destination */
    pt_generic = &(pt_minfo_rec->u.t_general_info);

    /*file size,2008-10-17,get from file system ,
      originally it get from file object*/
    pt_generic->ui8_media_sz = pt_minfo_obj->z_buff_leng;
    /*media duration 100 ns -> ms*/
    pt_generic->ui8_pb_duration
               = _mm_div64(pt_file_object->ui8_play_duration * 9, 1000, NULL); 
    if(pt_generic->ui8_pb_duration > pt_file_object->ui8_preroll * 90)
    {
        pt_generic->ui8_pb_duration -= pt_file_object->ui8_preroll * 90;
    }
    else
    {
        pt_generic->ui8_pb_duration = 0;   
    }
    
    pt_generic->ui8_preroll = pt_file_object->ui8_preroll * 90;
    /*total stream count*/
    pt_generic->ui1_strm_ns = 
        pt_asf_instance_obj->t_all_strm_info.ui1_audio_strm_num
        + pt_asf_instance_obj->t_all_strm_info.ui1_video_strm_num;
    pt_generic->b_hd_content = pt_asf_instance_obj->b_hd_content;  
    /* dlna server ,default the False */
    pt_generic->b_non_seekable = pt_asf_instance_obj->b_non_seekable;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT  
    /*bad interleaved*/
    pt_generic->b_is_bad_interleaved = pt_asf_instance_obj->b_is_bad_interleaved;
#endif 
    /*packet min max size*/
    pt_generic->ui4_sz_min_acces_unt = pt_file_object->ui4_min_packet_size;
    pt_generic->ui4_sz_max_acces_unt = pt_file_object->ui4_max_packet_size;
    pt_generic->ui4_sz_max_bitrate   = pt_file_object->ui4_max_bitrate;

    pt_content_encryption = &(pt_asf_instance_obj->t_content_encryption);
    if (pt_content_encryption->pi1_protect_type != NULL)
    {
        if (0 == x_strcmp((const CHAR*)(pt_content_encryption->pi1_protect_type), "DRM"))
        {
            pt_generic->b_is_drm_exist = TRUE;
        }
        else
        {
            pt_generic->b_is_drm_exist = FALSE;
        }
    }
    else
    {
        pt_generic->b_is_drm_exist = FALSE;
    }
    
    pt_ext_content_des = &( pt_asf_instance_obj->t_ext_content_des );
    pt_generic->b_stereo_scopic = pt_ext_content_des->b_stereo_scopic;
    pt_generic->b_half_width    = pt_ext_content_des->b_half_width;
    pt_generic->b_half_height   = pt_ext_content_des->b_half_height;
    pt_generic->e_scopic_type   = pt_ext_content_des->e_scopic_type;
    pt_generic->ui8_skip_dur    = pt_ext_content_des->ui8_skip_dur;
    
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_set_strm_packet_size
 *
 * Description: This function will set stream packet size
 *               stream object
 *
 * Inputs: pt_all_strm_info :specifies the stream info poniter  
 *         ui4_strm_id: stream id    
 *         ui4_per_packe_size :specifies packet sizes   
 *          
 * Outputs: NULL
 * 
 * Returns: NULL
 * 
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_set_strm_packet_size( MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info,
                                UINT32                     ui4_strm_id,
                                UINT32                     ui4_per_packe_size )
{
    MINFO_ASF_PER_STRM_INFO_T*      pt_per_strm_info;
    ASF_STREAM_PROPERTIES_OBJECT_T* pt_stream_obj;
    MINFO_ASF_PER_AUD_STRM_INF_T*   pt_aud_strm_inf;
    MINFO_ASF_AUDIO_STM_ATRBT_T*    pt_aud_strm_atrbt;
    MINFO_ASF_PER_VID_STRM_INF_T*   pt_vid_strm_inf;
    MINFO_ASF_VIDEO_STM_ATRBT_T*    pt_vid_strm_atrbt;
    if (NULL == pt_all_strm_info)
    {
        DBG_ERROR(("_asf_set_strm_packet_size:NULL\r\n"));
        return;
    }

    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {
        pt_stream_obj = pt_per_strm_info->pt_stream_obj;
        if (NULL == pt_stream_obj)
        {  
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        /*all streams set same packet size*/
        if (ASF_STREAM_NUM_MAX == ui4_strm_id)
        {
            if (ASFR_GUID_EQUAL 
                == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                 &gt_asf_object_stream_type_audio))
            {
                pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T*)
                                   pt_per_strm_info->pv_type_info;
                if (NULL == pt_aud_strm_inf)
                {
                    pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                    continue;
                }
                pt_aud_strm_atrbt = &(pt_aud_strm_inf->t_aud_strm_atrbt);
                pt_aud_strm_atrbt->ui4_packet_sz = ui4_per_packe_size;
                
            }
            else if (ASFR_GUID_EQUAL 
                    == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_video))
            {
                pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)
                                   pt_per_strm_info->pv_type_info;
                if (NULL == pt_vid_strm_inf)
                {
                    pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                    continue;
                }
                pt_vid_strm_atrbt = &(pt_vid_strm_inf->t_vid_strm_atrbt);
                pt_vid_strm_atrbt->ui4_packet_sz = ui4_per_packe_size;
            }
            else
            {
                /**/
            }
            /*next stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;

        }
        else
        {
            /*set for one stream*/
            if (pt_per_strm_info->ui4_stream_number != ui4_strm_id)
            {
                pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                continue;
            }
            
            if (ASFR_GUID_EQUAL 
                == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                 &gt_asf_object_stream_type_audio))
            {
                pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T*)
                                   pt_per_strm_info->pv_type_info;
                if (NULL == pt_aud_strm_inf)
                {
                    DBG_ERROR(("_asf_set_strm_packet_size: "
                               "pt_aud_strm_inf NULL\r\n"));
                    return;
                }
                pt_aud_strm_atrbt = &(pt_aud_strm_inf->t_aud_strm_atrbt);
                pt_aud_strm_atrbt->ui4_packet_sz = ui4_per_packe_size;
                
            }
            else if (ASFR_GUID_EQUAL 
                    == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_video))
            {
                pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)
                                   pt_per_strm_info->pv_type_info;
                if (NULL == pt_vid_strm_inf)
                {
                    DBG_ERROR(("_asf_set_strm_packet_size: "
                               "pt_aud_strm_inf NULL\r\n"));
                    return;
                }
                pt_vid_strm_atrbt = &(pt_vid_strm_inf->t_vid_strm_atrbt);
                pt_vid_strm_atrbt->ui4_packet_sz = ui4_per_packe_size;
            }
            else
            {
                /**/
            }

        }
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_set_audio_strm_lang
 *
 * Description: This function will set audio steam info language by extended
 *               stream object
 *
 * Inputs:  pt_minfo_obj     :specifies minfo object            
 *          pt_all_strm_info : specifies the stream info poniter
 *          
 * Outputs: NULL
 * 
 * Returns: NULL
 * 
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_set_audio_strm_lang(MINFO_OBJ_T*                pt_minfo_obj,
                              MINFO_ASF_ALL_STRM_INFO_T*  pt_all_strm_info)
{
    MINFO_ASF_PER_STRM_INFO_T*         pt_per_strm_info;
    MINFO_ASF_INSTANCE_INFO_T*         pt_asf_instance_obj; 
    ASF_STREAM_PROPERTIES_OBJECT_T*    pt_stream_obj;
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_object;
    MINFO_ASF_PER_AUD_STRM_INF_T*      pt_aud_strm_inf;
    MINFO_ASF_AUDIO_STM_ATRBT_T*       pt_aud_strm_atrbt;
    UINT16                             ui2_language_idx;

    /*parameter check*/
    if ((NULL == pt_minfo_obj) || (NULL == pt_all_strm_info))
    {
        DBG_ERROR(("_asf_set_audio_strm_lang:NULL\r\n"));
        return;
    }

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_set_audio_strm_lang:pt_asf_instance_obj "
                   "is NULL\r\n"));
        return;
    }

    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {
        pt_stream_obj = pt_per_strm_info->pt_stream_obj;
        if (NULL == pt_stream_obj)
        {
            /*next stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        /*if not audio continue*/
        if (ASFR_GUID_EQUAL 
            != _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                             &gt_asf_object_stream_type_audio))
        {
            /*next stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        pt_ex_stream_object = pt_per_strm_info->pt_ex_stream_obj;
        if (NULL == pt_ex_stream_object)
        {
            /*next stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        ui2_language_idx = pt_ex_stream_object->ui2_language_idx;
        pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T*)
                           pt_per_strm_info->pv_type_info;
        if (NULL == pt_aud_strm_inf)
        {
            /*next stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        pt_aud_strm_atrbt = &(pt_aud_strm_inf->t_aud_strm_atrbt);
        if (ui2_language_idx < pt_asf_instance_obj->t_language.ui2_lang_count)
        {
            x_strncpy((CHAR *)pt_aud_strm_atrbt->s_lang, 
                      (CHAR*)pt_asf_instance_obj->t_language.pt_languge[ui2_language_idx],
                      ISO_639_LANG_LEN);
        }

        /*next stream*/
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_strm_info_free
 *
 * Description: This function will get steam info by count&stream type
 *
 * Inputs:  pt_all_strm_info : specifies the stream info poniter
 *                e_strm_rype : stream type
 *          ui4_count        : the place in stream list, may be not identical 
 *                             to its stream id
 *          
 * Outputs: pt_minfo_rec, return stream attributes 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_get_strm_atrbt_by_idx(MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info,
                                 ASF_STREAM_TYPE_E          e_strm_type,
                                 UINT32                     ui4_count,
                                 MINFO_INFO_T*              pt_minfo_rec)
{
    MINFO_ASF_PER_STRM_INFO_T*      pt_per_strm_info;
    ASF_STREAM_PROPERTIES_OBJECT_T* pt_stream_obj;
    MINFO_TYPE_STM_ATRBT_T*         pt_type_str_atrbt;    
    MINFO_ASF_PER_VID_STRM_INF_T*   pt_vid_strm_inf;
    MINFO_ASF_PER_AUD_STRM_INF_T*   pt_aud_strm_inf;
    UINT64                          ui8_average_time_per_frame;
    UINT64                          ui8_dividend;
   
    /*Parameter check*/
    if ((NULL == pt_all_strm_info) || (NULL == pt_minfo_rec))
    {
        DBG_ERROR(("_asf_get_strm_atrbt_by_idx:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*at least from the first place*/
    if (0 == ui4_count)
    {
        DBG_ERROR(("_asf_get_strm_atrbt_by_idx: count is 0\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*get output pointer*/
    pt_type_str_atrbt = &(pt_minfo_rec->u.t_stm_attribute);

    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {
        pt_stream_obj = pt_per_strm_info->pt_stream_obj;
        if (NULL != pt_stream_obj)
        {
            #ifdef __EMBEDED_STREAM_PROPER_OBJ__
            #else
            /*if hidden stream object  not count*/
            if (TRUE == pt_stream_obj->b_in_ex_stream)
            {
                DBG_INFO(("_asf_get_strm_atrbt_by_idx:true\r\n"));
                pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                continue;
            }
            #endif
            
            if (ASF_STRM_VIDEO_TYPE == e_strm_type)
            {   
                DBG_INFO(("_asf_get_strm_atrbt_by_idx:video type\r\n"));
                if (ASFR_GUID_EQUAL 
                    != _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_video))
                {
                    pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                    DBG_INFO(("_asf_get_strm_atrbt_by_idx:not video\r\n"));
                    continue;
                }

                
            }
            else if (ASF_STRM_AUDIO_TYPE == e_strm_type)
            {
                DBG_INFO(("_asf_get_strm_atrbt_by_idx:audio type\r\n"));
                if (ASFR_GUID_EQUAL 
                    != _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_audio))
                {
                    pt_per_strm_info = pt_per_strm_info->pt_next_strm;
                    DBG_INFO(("_asf_get_strm_atrbt_by_idx:not audio\r\n"));
                    continue;
                }
            } 
            else
            {
                DBG_INFO(("_asf_get_strm_atrbt_by_idx:other type\r\n"));
            }
            
            --ui4_count;   /*decrease count if zero then break*/
            if (0 == ui4_count)
            {   
                DBG_INFO(("_asf_get_strm_atrbt_by_idx:break\r\n"));
                break;
            }
            
        }
        DBG_INFO(("_asf_get_strm_atrbt_by_idx:stream null\r\n"));
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }

    /*if exsit*/
    if (NULL != pt_per_strm_info)
    {
        pt_type_str_atrbt->t_stm_id.u.t_asf_stm_id =
                                             pt_per_strm_info->ui4_stream_number;
        if (ASF_STRM_VIDEO_TYPE == e_strm_type)
        {
            pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)
                               pt_per_strm_info->pv_type_info;
            if (NULL != pt_vid_strm_inf)
            {
                x_memcpy((VOID*)&(pt_type_str_atrbt->u.t_asf_video_stm_attr),
                         (VOID*)&(pt_vid_strm_inf->t_vid_strm_atrbt),
                         sizeof(MINFO_ASF_VIDEO_STM_ATRBT_T));
            }

            if (pt_per_strm_info->pt_ex_stream_obj != NULL)
            {
                ui8_average_time_per_frame = 
                                pt_per_strm_info->pt_ex_stream_obj->ui8_average_time_per_frame;
            }
            else
            {
                ui8_average_time_per_frame = 0;
            }
            
            if (ui8_average_time_per_frame != 0)
            {
                /*0x2540BE400 = 1000*10000000*/
                ui8_dividend = 0x2;
                ui8_dividend <<= 32;
                ui8_dividend += 0x540BE400;
                pt_type_str_atrbt->u.t_asf_video_stm_attr.ui4_rate = (UINT32)
                    _mm_div64(ui8_dividend, 
                              ui8_average_time_per_frame,
                              NULL);
                pt_type_str_atrbt->u.t_asf_video_stm_attr.ui4_scale = 1000;
            }
            else
            {
                /*0 indicate unknow frame rate*/
                pt_type_str_atrbt->u.t_asf_video_stm_attr.ui4_rate  = 0;  
                pt_type_str_atrbt->u.t_asf_video_stm_attr.ui4_scale = 1000;
            }
                
        }
        
        if (ASF_STRM_AUDIO_TYPE == e_strm_type)
        {
            pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T*)
                               pt_per_strm_info->pv_type_info;
            if (NULL != pt_aud_strm_inf)
            {
                x_memcpy((VOID*)&(pt_type_str_atrbt->u.t_asf_audio_stm_attr),
                         (VOID*)&(pt_aud_strm_inf->t_aud_strm_atrbt),
                         sizeof(MINFO_ASF_AUDIO_STM_ATRBT_T));
            }
        }
        
        return MINFOR_OK;
    
    }
    else
    {   
        DBG_ERROR(("_asf_get_strm_atrbt_by_idx:not found,"
                   "stream type=%d, count=%u\r\n", 
                   e_strm_type, 
                   ui4_count));
        return MINFOR_INFO_NOT_FOUND;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_strm_info_free
 *
 * Description: This function will free all resourece allocated for steam info
 *
 * Inputs:  pt_all_strm_info : specifies the stream info poniter
 *          
 * Outputs: NULL 
 * 
 * Returns: NULL
 * 
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_strm_info_free(MINFO_ASF_ALL_STRM_INFO_T* pt_all_strm_info)
{
    MINFO_ASF_PER_STRM_INFO_T*         pt_per_strm_info;
    MINFO_ASF_PER_STRM_INFO_T*         pt_per_strm_info_temp;
    /*stream properties*/
    ASF_STREAM_PROPERTIES_OBJECT_T*    pt_stream_obj;
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_obj;
    ASF_STREAM_NAME_T*                 pt_stream_name;
    ASF_PAYLOAD_EX_SYS_T*              pt_payload_ex_system;   
    UTF16_T*                           pwsz_stream_name;
    UINT8*                             pui1_ex_sys_info;
    VOID*                              pv_type_info;
    MINFO_ASF_PER_VID_STRM_INF_T*      pt_vid_strm_inf;
    MINF_BITMAPINFOHEADER_T*           pt_bitmap_info_header;
    MINFO_ASF_PER_AUD_STRM_INF_T*      pt_aud_strm_inf;
    UINT32                             ui4_idx;
    
    /*Parameter check*/
    if (NULL == pt_all_strm_info)
    {
        DBG_ERROR(("_asf_strm_info_free:NULL\r\n"));
        return;
    }

    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;

    while(NULL != pt_per_strm_info)
    {
        /*1.free stream properties related resource */
        pt_stream_obj = pt_per_strm_info->pt_stream_obj;
        pv_type_info = pt_per_strm_info->pv_type_info;
        if (NULL != pt_stream_obj)
        {
            if (NULL != pv_type_info)
            {
                if (ASFR_GUID_EQUAL 
                    == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_video))
                {
                    pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)pv_type_info;
                    pt_bitmap_info_header = &(pt_vid_strm_inf->t_bitmap_info_header);
                    if (NULL != pt_bitmap_info_header->pui1_code_specific)
                    {
                        _mm_util_mem_free(pt_bitmap_info_header->pui1_code_specific);
                        pt_bitmap_info_header->pui1_code_specific = NULL;
                    }
                    
                }
                else if (ASFR_GUID_EQUAL 
                    == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_audio))
                {
                    pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T*)pv_type_info;
                    if (NULL != pt_aud_strm_inf->t_aud_strm_atrbt.pui1_codec_info)
                    {
                        _mm_util_mem_free
                           (pt_aud_strm_inf->t_aud_strm_atrbt.pui1_codec_info);
                        pt_aud_strm_inf->t_aud_strm_atrbt.pui1_codec_info = NULL;
                    }
                }
                else
                {
                    /*no resource*/
                }

                _mm_util_mem_free(pv_type_info);
                pv_type_info = NULL;
            }

            /*free itself*/
            _mm_util_mem_free(pt_stream_obj);
            pt_stream_obj = NULL;
        }
        else
        {
            /*no resource*/
        }
        
        /*2.free extended stream properties related resource***************/
        pt_ex_stream_obj = pt_per_strm_info->pt_ex_stream_obj;
        if (NULL != pt_ex_stream_obj)
        {
            /*free name info*/
            pt_stream_name = pt_ex_stream_obj->pt_stream_name;
            if ((NULL != pt_stream_name)
                && (0 < pt_ex_stream_obj->ui2_stream_name_count))
            {
                for (ui4_idx = 0; 
                     ui4_idx < pt_ex_stream_obj->ui2_stream_name_count; 
                     ++ui4_idx)
                {
                    pwsz_stream_name = pt_stream_name[ui4_idx].pwsz_stream_name;
                    if (NULL != pwsz_stream_name)
                    {
                        _mm_util_mem_free(pwsz_stream_name);
                        pwsz_stream_name = NULL;
                    }
                }
                _mm_util_mem_free(pt_stream_name);
                pt_stream_name = NULL;
            }
            
            /*free payload info*/
            pt_payload_ex_system = pt_ex_stream_obj->pt_payload_ex_system;
            if ((NULL != pt_payload_ex_system)
                 && (0 < pt_ex_stream_obj->ui2_payload_ex_sys_count))
            {
                for (ui4_idx = 0; 
                     ui4_idx < pt_ex_stream_obj->ui2_payload_ex_sys_count; 
                     ++ui4_idx)
                {
                    pui1_ex_sys_info = pt_payload_ex_system[ui4_idx].pui1_ex_sys_info;
                    if (NULL != pui1_ex_sys_info)
                    {
                        (VOID)_mm_util_mem_free(pui1_ex_sys_info);
                        pui1_ex_sys_info = NULL;
                    }
                }
                _mm_util_mem_free(pt_payload_ex_system);
                pt_payload_ex_system = NULL;
            }

            /*free itself*/
            _mm_util_mem_free(pt_ex_stream_obj);
            pt_ex_stream_obj = NULL;
        }
        /***********free extended stream properties related resource***********/

        
        /*3.free current node**************************************************/
        /*next stream*/
        pt_per_strm_info_temp = pt_per_strm_info;
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
        /*free current node*/
        _mm_util_mem_free(pt_per_strm_info_temp);
        pt_per_strm_info_temp = NULL;
        
    }

    /*set head is NULL*/
    pt_all_strm_info->p_strm_info_head = NULL;
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_search_strm_info_by_id
 *
 * Description: This function search a stream info by its number, if found 
 *               return its pointer, if not allocate a stream info node to
 *               store stream info and insert it in the stream info list which
 *               is the t_all_strm_info of MINFO_ASF_INSTANCE_INFO_T
 *
 * Inputs:  ui2_strm_id : stream id
 *          
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Note:  the dynamic resource will be freed in x_close ,so do not leak memory.
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
MINFO_ASF_PER_STRM_INFO_T* _asf_search_strm_info_by_id( MINFO_OBJ_T* pt_minfo_obj,
                                                        UINT16       ui2_strm_id )
{
    MINFO_ASF_ALL_STRM_INFO_T*  pt_all_strm_info;
    MINFO_ASF_PER_STRM_INFO_T*  pt_per_strm_info;
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj; 

    
    /*parameter check*/
    if (NULL == pt_minfo_obj)  
    {
        DBG_ERROR(("_asf_search_strm_info_by_id:parameter is NULL\r\n"));
        return NULL;
    }
    if ((ASF_STREAM_NUM_MAX < ui2_strm_id)
        || (0 == ui2_strm_id))
    {
        DBG_ERROR(("_asf_search_strm_info_by_id:parameter id is %u\r\n",
                   ui2_strm_id));
        return NULL;
    }
    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_search_strm_info_by_id:pt_asf_instance_obj "
                   "is NULL\r\n"));
        return NULL;
    }

    /*get all stream info pointer*/
    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {
        if (ui2_strm_id == (UINT16)pt_per_strm_info->ui4_stream_number)
        {   
            /*find out ,break up searching*/
            break;
        }
        
        /*next stream */
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }
    
    /*if not found*/
    if (NULL == pt_per_strm_info)
    {  
        /*allocate a node for this stream*/
        pt_per_strm_info = (MINFO_ASF_PER_STRM_INFO_T*)
                            _mm_util_mem_alloc(sizeof(MINFO_ASF_PER_STRM_INFO_T));
        if (NULL == pt_per_strm_info)
        {
            DBG_ERROR(("_asf_search_strm_info_by_id:failed to allocate "
                       "memory for pt_per_strm_info\r\n"));
            return NULL;
        }
        /*init memory zero*/
        x_memset(pt_per_strm_info, 0, sizeof(MINFO_ASF_PER_STRM_INFO_T));
        
        /*insert stream list*/
        if (NULL == pt_all_strm_info->p_strm_info_head)
        {  
            /*first stream node*/
            pt_all_strm_info->p_strm_info_head = pt_per_strm_info;
        }
        else
        {
            pt_per_strm_info->pt_next_strm = pt_all_strm_info->p_strm_info_head;
            pt_all_strm_info->p_strm_info_head = pt_per_strm_info;
        }
        pt_per_strm_info->ui4_stream_number = ui2_strm_id;
    }

    return pt_per_strm_info;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_get_stream_type
 *
 * Description: This function gets stream type by stream id
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          ui2_strm_id:  stream id
 *           
 * Outputs: NULL 
 * 
 * Returns: stream type
 *
 * Note:  called only after parsing
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-5-20 : initial

 ----------------------------------------------------------------------------*/

ASF_STREAM_TYPE_E _asf_get_stream_type( MINFO_OBJ_T* pt_minfo_obj,
                                        UINT16       ui2_strm_id )
{
    MINFO_ASF_ALL_STRM_INFO_T*       pt_all_strm_info;
    MINFO_ASF_PER_STRM_INFO_T*       pt_per_strm_info;
    MINFO_ASF_INSTANCE_INFO_T*       pt_asf_instance_obj; 
    ASF_STREAM_PROPERTIES_OBJECT_T*  pt_stream_obj;
    
    /*parameter check*/
    if (NULL == pt_minfo_obj)  
    {
        DBG_ERROR(("_asf_get_stream_type:parameter is NULL\r\n"));
        return ASF_STRM_UNKNOWN;
    }
    if ((ASF_STREAM_NUM_MAX < ui2_strm_id)
        || (0 == ui2_strm_id))
    {
        DBG_ERROR(("_asf_get_stream_type:parameter id is %u\r\n",
                   ui2_strm_id));
        return ASF_STRM_UNKNOWN;
    }
    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_get_stream_type:pt_asf_instance_obj "
                   "is NULL\r\n"));
        return ASF_STRM_UNKNOWN;
    }

    /*get all stream info pointer*/
    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    /*get stream list head*/
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {
        if (ui2_strm_id == (UINT16)pt_per_strm_info->ui4_stream_number)
        {   
            /*find out ,break up searching*/
            break;
        }
        
        /*next stream */
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }

    if (NULL != pt_per_strm_info)
    {
        pt_stream_obj = pt_per_strm_info->pt_stream_obj;
        if (ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                             &gt_asf_object_stream_type_video))
        {
            return ASF_STRM_VIDEO_TYPE;
        }
        else if (ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                                  &gt_asf_object_stream_type_audio))
        {
            return ASF_STRM_AUDIO_TYPE;
        }
        else
        {
            
        }
            
    }
    
    return ASF_STRM_UNKNOWN;
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_file_properties
 *
 * Description: This function gets the file object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the file object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:  
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_file_properties ( MINFO_OBJ_T*      pt_minfo_obj, 
                                  ASF_OBJ_COMMON_T* pt_this_obj )
{
    copybytes_fct                 pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*    pt_asf_instance_obj; 
    /*read buffer pointer*/
    UINT8*                        pui1_head;    
    /* File Object*/
    ASF_FILE_PROPERTIES_OBJECT_T* pt_file_object;
    UINT32                        ui4_read_size;
    INT32                         i4_ret;

    DBG_INFO(("_asf_load_file_properties:begin to load file "
              "properties object\r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_file_properties:para is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /************************************************************************
     Do                                                              
    ************************************************************************/
    /*get asf handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_file_properties:asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*get temporary buffer*/   
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_file_properties:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to file object memory*/
    pt_file_object = &(pt_asf_instance_obj->t_file_object);
    pt_this_obj->pv_obj_specific = (VOID *)pt_file_object;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_file_properties:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    ui4_read_size = 0;
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN),
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_file_properties:error object header (%d)!\r\n",
                  ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*get File guid and size*/
    _asf_get_guid(&(pt_file_object->t_file_id), pui1_head);
    pui1_head += 16;   /*GUID is 16 bytes long*/  
    
    /*Get file size, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_file_size), pui1_head);
    pui1_head += 8; 
    
    /*Get creation date, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_create_date), pui1_head);
    pui1_head += 8; 

    /*Get data packets count, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_packets_count), pui1_head);
    pui1_head += 8; 

    /*Get play duration, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_play_duration), pui1_head);
    pui1_head += 8; 

    /*Get send duration, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_send_duration), pui1_head);
    pui1_head += 8; 

    /*Get preroll, 8 bytes*/
    ASF_GET_QWORD((pt_file_object->ui8_preroll), pui1_head);
    pui1_head += 8; 


    /*Get flags, 4 bytes*/
    ASF_GET_DWORD((pt_file_object->ui4_flags), pui1_head);
    pui1_head += 4; 

    /*Get minimum data packet size, 4 bytes*/
    ASF_GET_DWORD((pt_file_object->ui4_min_packet_size), pui1_head);
    pui1_head += 4; 

    /*Get maximum data packet size, 4 bytes*/
    ASF_GET_DWORD((pt_file_object->ui4_max_packet_size), pui1_head);
    pui1_head += 4; 

    /*Get maximum bitrate, 4 bytes*/
    ASF_GET_DWORD((pt_file_object->ui4_max_bitrate), pui1_head);
    pui1_head += 4; 
    /***********************End to Get object info*****************************/


    /******************provided for index bulider ************************/
    /*pt_asf_instance_obj->t_idx_info.ui8_data_sz = pt_file_object->ui8_file_size;*/


    DBG_INFO(("_asf_load_file_properties:end loading file properties "
              "object\r\n"));
    
    return MINFOR_OK;
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_file_object
 *
 * Description: This function free the stream object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the file object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_free_file_object ( MINFO_OBJ_T*       pt_minfo_obj, 
                              ASF_OBJ_COMMON_T** ppt_this_obj)
{
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_stream_properties
 *
 * Description: This function gets the stream object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which store common info 
 *                        of the stream properties object            
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    if memory is leaked here ,in minfo_close will free it
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_stream_properties (MINFO_OBJ_T*      pt_minfo_obj,
                                   ASF_OBJ_COMMON_T* pt_this_obj)
{
    copybytes_fct                   pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*      pt_asf_instance_obj; 
    UINT8*                          pui1_head ;/*read buffer pointer*/       
    /*stream properties*/
    ASF_STREAM_PROPERTIES_OBJECT_T  t_strm_properties;
    ASF_STREAM_PROPERTIES_OBJECT_T* pt_strm_properties;
    MINFO_ASF_ALL_STRM_INFO_T*      pt_all_strm_info;
    MINFO_ASF_PER_STRM_INFO_T*      pt_per_strm_info;
    MINFO_ASF_PER_VID_STRM_INF_T*   pt_vid_strm_inf;
    MINFO_ASF_VIDEO_STM_ATRBT_T*    pt_vid_strm_atrbt;
    MINF_BITMAPINFOHEADER_T*        pt_bitmap_info_header;
    MINFO_ASF_PER_AUD_STRM_INF_T*   pt_aud_strm_inf;
    MINFO_ASF_AUDIO_STM_ATRBT_T*    pt_aud_strm_atrbt;
    /*error correction info*/
    MINO_SPREAD_AUDIO_T*            pt_error_correction;
    UINT32                          ui4_codec_specific_len;
    UINT16                          ui2_codec_id;
    UINT32                          ui4_read_size;
    INT32                           i4_ret;
   
    DBG_INFO(("_asf_load_stream_properties:begin to load stream "
              "properties\r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_stream_properties:para is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /************************************************************************
     Do                                                              
    ************************************************************************/
    /*init vaiables*/
    ui4_codec_specific_len = 0;
    ui2_codec_id = 0;
    ui4_read_size = 0;
    x_memset((VOID*)(&t_strm_properties), 0, sizeof(ASF_STREAM_PROPERTIES_OBJECT_T));

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_stream_properties:pt_asf_instance_obj "
                   "is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_stream_properties:pui1_head  is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_stream_properties:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*get stream properties object pointer to a stack variable*/
    pt_strm_properties = &t_strm_properties;

    /*whether is inside extended stream properties?*/
    if (ASF_OBJ_EX_STREAM_PROPERTIES == pt_this_obj->pt_parent->e_type)
    {
        pt_strm_properties->b_in_ex_stream = TRUE;
        #ifdef __EMBEDED_STREAM_PROPER_OBJ__
        /*continue to parse ...*/
        #else
        DBG_INFO(("_asf_load_stream_properties:Do not support\r\n"));
        return MINFOR_FMT_NOT_SUPPORT;
        #endif
    }
    else
    {
        pt_strm_properties->b_in_ex_stream =  FALSE;
    }

    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size );
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_stream_properties:error object header (%d)!\r\n",
                   ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*Get stream type GUID 16 bytes*/
    _asf_get_guid(&(pt_strm_properties->t_strm_type), pui1_head);
    pui1_head += 16; 

    /*Get stream error correction type GUID 16 bytes*/
    _asf_get_guid(&(pt_strm_properties->t_err_correct_type), pui1_head);
    pui1_head += 16;  
    
    /*get time offset, 8 bytes*/
    ASF_GET_QWORD(pt_strm_properties->ui8_time_offset, pui1_head);
    pui1_head += 8; 

    /*get type sepcific data length, 4 bytes*/
    ASF_GET_DWORD(pt_strm_properties->ui4_type_spec_len, pui1_head);
    pui1_head += 4; 
    
    /*get type correction data length, 4 bytes*/
    ASF_GET_DWORD(pt_strm_properties->ui4_err_correct_len, pui1_head);
    pui1_head += 4; 

    /*get stream flags, 2 bytes*/
    ASF_GET_WORD(pt_strm_properties->ui2_flags, pui1_head);
    pui1_head += 2; 
    /*stream number is the lowest 7 significant bits which is LSB
    valid:1~ASF_STREAM_NUM_MAX(127)*/
    pt_strm_properties->ui1_stream_number = 
        (UINT8) ((pt_strm_properties->ui2_flags) & (0x0000007f));
    pt_strm_properties->b_encrypt_flag = 
        (BOOL) ((pt_strm_properties->ui2_flags) & (0x80000000));

    /*get reserverd,4 bytes*/
    ASF_GET_DWORD(pt_strm_properties->ui4_reserved , pui1_head);
    pui1_head += 4; 

    /*Have known stream number so search in _MINFO_ASF_STRM_INFO_T for this 
      stream ,if not found allocate MINFO_ASF_PER_STRM_INFO_T for
      this stream
    */
    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    /*get stream info node*/
    pt_per_strm_info = _asf_search_strm_info_by_id(pt_minfo_obj,
                       pt_strm_properties->ui1_stream_number);
    if (NULL == pt_per_strm_info)
    {
        DBG_ERROR(("_asf_load_stream_properties:pt_per_strm_info NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*copy stack pt_strm_properties to pt_per_strm_info*/
    pt_strm_properties = pt_per_strm_info->pt_stream_obj;
    if (NULL == pt_strm_properties)
    {
        pt_strm_properties = (ASF_STREAM_PROPERTIES_OBJECT_T* )
                             _mm_util_mem_alloc(sizeof(ASF_STREAM_PROPERTIES_OBJECT_T));
        if (NULL == pt_strm_properties)
        {
            DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                       "memory for pt_stream_obj\r\n"));
            /*do not free memory here*/
            return MINFOR_OUT_OF_MEMORY;
        }  
        /* */
        pt_per_strm_info->pt_stream_obj = pt_strm_properties;
    }
    /*store stream properties info*/
    x_memcpy(pt_strm_properties, 
             &t_strm_properties, 
             sizeof(ASF_STREAM_PROPERTIES_OBJECT_T));
    /*set specific pointer for pt_this_obj*/
    pt_this_obj->pv_obj_specific = (VOID *)pt_strm_properties;


    /*get stream type specific data */
    /*if the stream is video*/
    if (ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                                         &gt_asf_object_stream_type_video))
    {
        /******************get video specific data***************************/
        if (NULL == pt_per_strm_info->pv_type_info)
        {
            pt_per_strm_info->pv_type_info = 
                             _mm_util_mem_alloc(sizeof(MINFO_ASF_PER_VID_STRM_INF_T));
            if (NULL == pt_per_strm_info->pv_type_info)
            {
                DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                           "memory for pv_type_info for video\r\n"));
                /*do not free memory here*/
                return MINFOR_OUT_OF_MEMORY;
            }
        }
        
        /*set video type specific info pointer*/
        pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T* )
                           pt_per_strm_info->pv_type_info;
        x_memset(pt_vid_strm_inf, 0, sizeof(MINFO_ASF_PER_VID_STRM_INF_T));
        
        /*set stream properties type specific info pointer*/
        pt_strm_properties->puil_type_spec_data = (UINT8 *)pt_vid_strm_inf;
        
        /*unhidden stream*/
        if (FALSE == pt_strm_properties->b_in_ex_stream)
        {
            /*count video stream*/
            ++pt_all_strm_info->ui1_video_strm_num;
            /*pt_per_strm_info*/
            /*add non-hidden video stream to index stream map to further build index table*/
            _asf_bit_map_set( ( UINT8* )&( pt_asf_instance_obj->t_idx_info.aui1_idx_strm_map[0] ),
                              16, pt_per_strm_info->ui4_stream_number );
        }
        else
        {
            /*count hidden video stream*/
            ++pt_all_strm_info->ui1_hidden_video_strm_num;
            /*pt_per_strm_info*/
            
        }
        /*video attribute pointer*/
        pt_vid_strm_atrbt = &(pt_vid_strm_inf->t_vid_strm_atrbt);
        /*get encoded image width, 4 bytes*/
        ASF_GET_DWORD(pt_vid_strm_atrbt->i4_width, pui1_head);
        pui1_head += 4; 
        
        /*get encoded image height, 4 bytes*/
        ASF_GET_DWORD(pt_vid_strm_atrbt->i4_height, pui1_head);
        pui1_head += 4; 
      
        /*get reserved flags, 1 bytes*/
        ASF_GET_BYTE(pt_vid_strm_atrbt->b_default, pui1_head);
        pui1_head += 1; 

        /*get format data size, 2 bytes*/
        ASF_GET_WORD(pt_vid_strm_inf->ui2_bitmap_header_size, pui1_head);
        pui1_head += 2; 
        
        /*get MINF_BITMAPINFOHEADER_T pointer*/
        pt_bitmap_info_header = &(pt_vid_strm_inf->t_bitmap_info_header);

        /*begin get bitmap info header data*/
        /*get format data size, 4 bytes*/
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_size, 
                      pui1_head);
        pui1_head += 4; 
        
        /*get image width, 4 bytes*/
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_width, 
                      pui1_head);
        pui1_head += 4; 
        
        /*get image height, 4 bytes*/
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_height, 
                      pui1_head);
        pui1_head += 4; 

        /*here add check if it is HD(high definition)*/
        if (((1280 == pt_bitmap_info_header->ui4_width) 
               && (720 == pt_bitmap_info_header->ui4_height))
            || ((1920 == pt_bitmap_info_header->ui4_width) 
               && (1080 == pt_bitmap_info_header->ui4_height)))
        {
            pt_asf_instance_obj->b_hd_content = TRUE;
        }

        /*get reserved, 2 bytes ,should be 1*/
        ASF_GET_WORD(pt_bitmap_info_header->ui2_reserved, 
                     pui1_head);
        pui1_head += 2; 
      
        /*get bits per pixel count, 2 bytes */
        ASF_GET_WORD(pt_bitmap_info_header->ui2_bit_count, 
                     pui1_head);
        pui1_head += 2; 

        /*get compression ID, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_compression, 
                      pui1_head);
        pui1_head += 4; 
        pt_vid_strm_atrbt->e_enc = _asf_video_codec(
                        pt_bitmap_info_header->ui4_compression);
        
        /*get image size, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_size_image, 
                      pui1_head);
        pui1_head += 4; 
        
        /*get horizontal pixels per meter, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_xpels_per_meter, 
                      pui1_head);
        pui1_head += 4; 

        /*get virtical  pixels per meter, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_ypels_per_meter,
                     pui1_head);
        pui1_head += 4; 
        
        /*get color used count, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_clr_used, 
                      pui1_head);
        pui1_head += 4; 
        
        /*get important color count, 4 bytes */
        ASF_GET_DWORD(pt_bitmap_info_header->ui4_clr_important, 
                      pui1_head);
        pui1_head += 4; 

        /*get codec specific data length = ui2_bitmap_header_size - 40
        (fixed fields length)*/
        ui4_codec_specific_len = (UINT32)(pt_vid_strm_inf->ui2_bitmap_header_size - 40);
        if (ui4_codec_specific_len > 0)
        {
            pt_bitmap_info_header->pui1_code_specific 
                  = (UINT8 *)_mm_util_mem_alloc(ui4_codec_specific_len);
            if (NULL != pt_bitmap_info_header->pui1_code_specific)
            {
                x_memcpy((VOID *)pt_bitmap_info_header->pui1_code_specific,
                         (VOID *)pui1_head, ui4_codec_specific_len);
            }
            else
            {
                DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                           "memory for pt_vid_strm_inf->t_bitmap_info_header"
                           " pui1_code_specific!\r\n"));
                /*do not free memory here*/
                return MINFOR_OUT_OF_MEMORY;
            }
            /*just for asf_minfo_get_info function*/
            pt_vid_strm_atrbt->ui4_codec_info_size = ui4_codec_specific_len;
            pt_vid_strm_atrbt->pui1_codec_info     = 
                                    pt_bitmap_info_header->pui1_code_specific;
        }
        else
        {
            pt_vid_strm_atrbt->ui4_codec_info_size = 0;
            pt_vid_strm_atrbt->pui1_codec_info     = NULL;
        }
        /*end get bitmap info header data*/


        /********************************************************************
        omit video error-specific data because generally vedio correction
        data length is 0
        *********************************************************************/
        pt_strm_properties->puil_err_correct_data = NULL;
        
    }
    /*if the stream is audio*/
    else if (ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                                              &gt_asf_object_stream_type_audio))
    {
        /***************get audio specific data*****************************/
        if (NULL == pt_per_strm_info->pv_type_info)
        {
            pt_per_strm_info->pv_type_info = 
                             _mm_util_mem_alloc(sizeof(MINFO_ASF_PER_AUD_STRM_INF_T));
            if (NULL == pt_per_strm_info->pv_type_info)
            {
                DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                           "memory for pv_type_info for audio\r\n"));
                /*do not free memory here*/
                return MINFOR_OUT_OF_MEMORY;
            }
        } 
        pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T* )
                           pt_per_strm_info->pv_type_info;
        x_memset(pt_aud_strm_inf, 0, sizeof(MINFO_ASF_PER_AUD_STRM_INF_T));
        pt_strm_properties->puil_type_spec_data = (UINT8 *)pt_aud_strm_inf;

        /*count audio stream*/
        /*++pt_all_strm_info->ui1_audio_strm_num;  */
        /*unhidden stream*/
        if (FALSE == pt_strm_properties->b_in_ex_stream)
        {
            /*count audio stream*/
            ++pt_all_strm_info->ui1_audio_strm_num;
            /*pt_per_strm_info*/
        }
        else
        {
            /*count hidden audio stream*/
            ++pt_all_strm_info->ui1_hidden_audio_strm_num;
            /*pt_per_strm_info*/
            
        }
       
        pt_aud_strm_atrbt = &(pt_aud_strm_inf->t_aud_strm_atrbt);
        /*get codec ID/format tag, 2 bytes */
        ASF_GET_WORD(ui2_codec_id, pui1_head);
        pui1_head += 2; 
        pt_aud_strm_atrbt->e_enc = _asf_audio_codec(ui2_codec_id);
        /*set layer value, if mp3 set layer = 3*/
        if (ASF_WAVE_FORMAT_MP3 != ui2_codec_id)
        {
            pt_aud_strm_atrbt->e_layer = 0;

        }
        else
        {
            pt_aud_strm_atrbt->e_layer = 3;
        }

        /*get number of channels ,2 bytes*/
        ASF_GET_WORD(pt_aud_strm_atrbt->i2_channels, pui1_head);
        pui1_head += 2; 
        
        /*get samples per second, 4 bytes*/
        ASF_GET_DWORD(pt_aud_strm_atrbt->i4_samples_per_sec, 
                      pui1_head);
        pui1_head += 4; 

        /*get average number of bytes per sencond*/
        ASF_GET_DWORD(pt_aud_strm_atrbt->ui4_avg_bytes_per_sec, pui1_head);
        pui1_head += 4; 
        
        /*get block alignment ,2 bytes*/
        ASF_GET_WORD(pt_aud_strm_atrbt->i2_block_align, pui1_head);
        pui1_head += 2; 
        
        /*get bit per sample ,2 bytes*/
        ASF_GET_WORD(pt_aud_strm_atrbt->i2_bits_per_sample, pui1_head);
        pui1_head += 2; 

        /*get code specific lenght ,2 bytes*/
        ASF_GET_WORD(pt_aud_strm_atrbt->ui4_codec_info_size , pui1_head);
        pui1_head += 2; 

        /*for extended format*/
        if(ui2_codec_id == ASF_WAVE_FORMAT_EXT)
        {
            ASF_GUID_T  t_ext_guid;
            pui1_head += 2;  /*skip bits per sample*/
            pui1_head += 4;  /*skip channel mask*/

            _asf_get_guid(&t_ext_guid, pui1_head);
            pui1_head += 16; 
            if(ASFR_GUID_EQUAL == _asf_cmp_guid(&t_ext_guid, 
                                              &gt_asf_codec_ext_ac3_guid))
            {
                pt_aud_strm_atrbt->e_enc = MINFO_INFO_AUD_ENC_AC3;
            }
        }

        if (pt_aud_strm_atrbt->ui4_codec_info_size  > 0)
        {
            pt_aud_strm_atrbt->pui1_codec_info = 
            (UINT8* )_mm_util_mem_alloc(pt_aud_strm_atrbt->ui4_codec_info_size);
            if (NULL != pt_aud_strm_atrbt->pui1_codec_info)
            {
                x_memcpy((VOID *)(pt_aud_strm_atrbt->pui1_codec_info),
                          pui1_head,  pt_aud_strm_atrbt->ui4_codec_info_size);
            }
            else
            {
                DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                           "memory for pt_aud_strm_inf->pui1_code_specific!\r\n"));
                /*do not free memory here*/
                return MINFOR_OUT_OF_MEMORY;
            }
        }
        
        pui1_head += pt_aud_strm_atrbt->ui4_codec_info_size; 
        
        /*set default field*/
        pt_aud_strm_atrbt->b_default = 0;

   
        /*****************begin audio error-specific data****************/
        pt_strm_properties->puil_err_correct_data = (UINT8*)
                            &(pt_aud_strm_inf->t_error_correction);
        if ((ASFR_GUID_EQUAL 
            == _asf_cmp_guid(&(pt_strm_properties->t_err_correct_type), 
                             &gt_asf_guid_audio_spread))
            && (pt_strm_properties->ui4_err_correct_len > 0))
        {
            pt_error_correction = &(pt_aud_strm_inf->t_error_correction);
            /*get span*/
            ASF_GET_BYTE(pt_error_correction->ui1_span, 
                         pui1_head);
            pui1_head += 1; 
            
            /*get virtual packet length ,2 bytes*/
            ASF_GET_WORD(pt_error_correction->ui2_vir_pack_len, 
                         pui1_head);
            pui1_head += 2; 
           
            /*get chunk length ,2 bytes*/
            ASF_GET_WORD(pt_error_correction->ui2_vir_chunk_len, 
                         pui1_head);
            pui1_head += 2; 

            /*get data length ,2 bytes, should be 1*/
            ASF_GET_WORD(pt_error_correction->ui2_silence_len,
                         pui1_head);
            pui1_head += 2; 

            /*the silence data should be 0*/
            pt_error_correction->pui1_silence = NULL;
            ++pui1_head;

        }
    }
    else
    {
        /*do not support streams other than video and audio*/
        DBG_ERROR(("_asf_load_stream_properties:nowaday do not support streams "
                   "other than video and audio"));
    }

   /***********************End to Get object info*****************************/
    DBG_INFO(("_asf_load_stream_properties:end loading stream properties\r\n"));
    return MINFOR_OK;
    
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_free_stream_properties
 *
 * Description: This function free the stream object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the stream properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_free_stream_properties( MINFO_OBJ_T*       pt_minfo_obj, 
                                   ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_content_description
 *
 * Description: This function gets the content description object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the content description object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_content_description( MINFO_OBJ_T*      pt_minfo_obj, 
                                     ASF_OBJ_COMMON_T* pt_this_obj )
{

    copybytes_fct                     pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*        pt_asf_instance_obj; 
    /*read buffer pointer*/     
    UINT8*                            pui1_head;     
    /*Language List Object*/
    ASF_CONTENT_DESCRIPTION_OBJECT_T* pt_content_des;

    UINT32                            ui4_read_size;
    UINT16                            ui2_title_len;
    UINT16                            ui2_author_len;
    UINT16                            ui2_copyright_len;
    UINT16                            ui2_des_len;
    UINT16                            ui2_rating_len;
  
    INT32                             i4_ret;

    DBG_INFO(("_asf_load_content_description:begin to load content description\r\n"));
    
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_content_description:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*init variables*/

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_content_description:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_content_description:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to content description object memory*/
    pt_content_des = &(pt_asf_instance_obj->t_content_des);
    pt_this_obj->pv_obj_specific = (VOID*)pt_content_des;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_content_description:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_content_description:error object (%d)!\r\n", 
                ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }

    /*get title length, 2 bytes*/
    ASF_GET_WORD(ui2_title_len, pui1_head);
    
    pui1_head += 2;   
    /*get author length, 2 bytes*/
    ASF_GET_WORD(ui2_author_len, pui1_head);
    pui1_head += 2;   
    
    /*get copyright length, 2 bytes*/
    ASF_GET_WORD(ui2_copyright_len, pui1_head);
    pui1_head += 2;   
    
    /*get description length, 2 bytes*/
    ASF_GET_WORD(ui2_des_len, pui1_head);
    pui1_head += 2;   
    
    /*get rating length, 2 bytes*/
    ASF_GET_WORD(ui2_rating_len, pui1_head);
    pui1_head += 2;   

    do
    {
        /*get title info,length include terminal NULL*/
        if (ui2_title_len > 0)
        {
            pt_content_des->pwsz_title = (UTF16_T*)_mm_util_mem_alloc(ui2_title_len);
            if (NULL == pt_content_des->pwsz_title)
            {
                DBG_ERROR(("_asf_load_content_description:fail to alloc mem for "
                           "pwsz_title"));
                break;;
            }
            x_memcpy(pt_content_des->pwsz_title, pui1_head, ui2_title_len);
            pui1_head += ui2_title_len;
            
        }

         /*get author info,length include terminal NULL*/
        if (ui2_author_len > 0)
        {
            pt_content_des->pwsz_author = (UTF16_T*)_mm_util_mem_alloc(ui2_author_len);
            if (NULL == pt_content_des->pwsz_author)
            {
                DBG_ERROR(("_asf_load_content_description:fail to alloc mem for "
                           "pwsz_author"));
                break;;
            }
            x_memcpy(pt_content_des->pwsz_author, pui1_head, ui2_author_len);
            pui1_head += ui2_author_len;
            
            
        }
        
        /*get copyright info,length include terminal NULL*/
        if (ui2_copyright_len > 0)
        {
            pt_content_des->pwsz_copyright = (UTF16_T*)_mm_util_mem_alloc(ui2_copyright_len);
            if (NULL == pt_content_des->pwsz_copyright)
            {
                DBG_ERROR(("_asf_load_content_description:fail to alloc mem for "
                           "pwsz_copyright"));
                break;;
            }
            x_memcpy(pt_content_des->pwsz_copyright, pui1_head, ui2_copyright_len);
            pui1_head += ui2_copyright_len;
            
        }
        /*get description info,length include terminal NULL*/
        if (ui2_des_len > 0)
        {
            pt_content_des->pwsz_description = (UTF16_T*)_mm_util_mem_alloc(ui2_des_len);
            if (NULL == pt_content_des->pwsz_description)
            {
                DBG_ERROR(("_asf_load_content_description:fail to alloc mem for "
                           "pwsz_description"));
                break;
            }
            x_memcpy(pt_content_des->pwsz_description, pui1_head, ui2_des_len);
            pui1_head += ui2_des_len;
            
        }
        /*get rating info,length include terminal NULL*/
        if (ui2_rating_len > 0)
        {
            pt_content_des->pwsz_rating = (UTF16_T*)_mm_util_mem_alloc(ui2_rating_len);
            if (NULL == pt_content_des->pwsz_rating)
            {
                DBG_ERROR(("_asf_load_content_description:fail to alloc mem for "
                           "pwsz_rating"));
                break;;
            }
            x_memcpy(pt_content_des->pwsz_rating, pui1_head, ui2_rating_len);
            pui1_head += ui2_rating_len;
            
        }
        
        DBG_INFO(("_asf_load_content_description:end loading content "
                  "description object\r\n"));
        return MINFOR_OK;
       
    }while(0);

    /* exception handle, free memory resource*/
    if (NULL != pt_content_des->pwsz_title)
    {
        _mm_util_mem_free(pt_content_des->pwsz_title);
        pt_content_des->pwsz_title = NULL;
    }
    if (NULL != pt_content_des->pwsz_author)
    {
        _mm_util_mem_free(pt_content_des->pwsz_author);
        pt_content_des->pwsz_author = NULL;
    }
    if (NULL != pt_content_des->pwsz_copyright)
    {
        _mm_util_mem_free(pt_content_des->pwsz_copyright);
        pt_content_des->pwsz_copyright = NULL;
    }
    if (NULL != pt_content_des->pwsz_description)
    {
        _mm_util_mem_free(pt_content_des->pwsz_description);
        pt_content_des->pwsz_description = NULL;
    }
    if (NULL != pt_content_des->pwsz_rating)
    {
        _mm_util_mem_free(pt_content_des->pwsz_rating);
        pt_content_des->pwsz_rating = NULL;
    }
    
    return MINFOR_OUT_OF_MEMORY;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_content_description
 *
 * Description: This function free the content description object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the content description object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-21 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_content_description ( MINFO_OBJ_T*       pt_minfo_obj, 
                                      ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_ext_content_description
 *
 * Description: This function gets the extended content description object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the extended content description object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:   if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : guanghui wang
 *               modified by Lianming   2010/11/22
 * 
 ----------------------------------------------------------------------------*/
INT32 _asf_load_ext_content_description( MINFO_OBJ_T*      pt_minfo_obj, 
                                     ASF_OBJ_COMMON_T* pt_this_obj )
{

    copybytes_fct                     pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*        pt_asf_instance_obj; 
    /*read buffer pointer*/     
    UINT8*                            pui1_head;     
    /*Language List Object*/
    ASF_EXT_CONTENT_DES_OBJECT_T*     pt_ext_content_des;
    UINT8*                            pui1_str    = NULL;
    UINT32                            ui4_str_len = 0;
    INT32                             i4_ret      = MINFOR_OK;
    UINT32                            ui4_read_size;
    UINT16                            ui2_entry_cnt;
    UINT16                            ui2_name_len;
    UINT16                            ui2_type;
    UINT16                            ui2_data_len;
    UINT32                            ui4_idx;
    ASF_EXT_CONTENT_DES_NAME_VALUE_T* pt_name_value;
    UINT32                            ui4_bool_value;
    const CHAR* ac_ext_des_name[EXT_CONT_NAME_MAX] = 
    {
        "WM/Year",
        "WM/AlbumTitle",
        "WM/AlbumArtist",
        "WM/Genre",
        
        "Stereoscopic",
        "StereoscopicLayout",
        "StereoscopicSkip",
        "StereoscopicHalfWidth",
        "StereoscopicHalfHeight",
        
    };

    DBG_INFO( ( "%s#Line%d:begin to load extended content description\r\n", 
              __FUNCTION__, __LINE__ ) );
    
    if ( ( NULL == pt_minfo_obj) || ( NULL == pt_this_obj ) )
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MINFOR_INTERNAL_ERR;
    }

    /*get minfo handler*/
    pt_asf_instance_obj = ( MINFO_ASF_INSTANCE_INFO_T* )( pt_minfo_obj->pv_handler_obj );
    if ( NULL == pt_asf_instance_obj )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_instance_obj is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if ( NULL == pui1_head )
    {
        DBG_ERROR( ( "%s#Line%d:pui1_head is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to content description object memory*/
    pt_ext_content_des           = &( pt_asf_instance_obj->t_ext_content_des );
    pt_this_obj->pv_obj_specific = ( VOID* )pt_ext_content_des;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if ( NULL == pf_copybyte )
    {
        DBG_ERROR( ( "%s#Line%d:pf_copybyte is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MINFOR_INTERNAL_ERR;
    }
    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte( pt_minfo_obj, pui1_head, 
                          pt_asf_instance_obj->ui4_buf_length, 
                          (SIZE_T)( pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN ), 
                          &ui4_read_size );
    if ( i4_ret != INPUTR_OK )
    {
        DBG_ERROR( ( "%s#Line%d:error object (%d)!\r\n", __FUNCTION__, __LINE__,
                     ui4_read_size ) );
        return MINFOR_INTERNAL_ERR;
    }

    /*get entry count, 2 bytes*/
    ASF_GET_WORD( ui2_entry_cnt, pui1_head );
    pui1_head += 2;
    while ( ui2_entry_cnt > 0 )
    {
        /*get name length, 2 bytes*/
        ASF_GET_WORD( ui2_name_len, pui1_head );
        pui1_head += 2;
        
        if ( ( !pui1_str ) || ( ui2_name_len > ui4_str_len ) ) 
        {
            if ( pui1_str )
            {
                _mm_util_mem_free( pui1_str );
            }
            ui4_str_len = ( ui2_name_len > 128 )? ui2_name_len : 128;
            pui1_str = ( UINT8* )_mm_util_mem_alloc( ui4_str_len );
            if ( NULL == pui1_str )
            {
                DBG_ERROR( ( "%s#Line%d:fail to alloc mem for pui1_str\r\n",
                             __FUNCTION__, __LINE__ ) );
                i4_ret = MINFOR_OUT_OF_MEMORY;
                break;
            }
        }
        /*get ext_dexcription name*/
        _asf_wide_char_2_str( ( UTF16_T* )pui1_head, ui2_name_len, ( CHAR* )pui1_str, ui4_str_len );
        pui1_head += ui2_name_len;

        ASF_GET_WORD( ui2_type, pui1_head );
        pui1_head += 2;

        ASF_GET_WORD( ui2_data_len, pui1_head );
        pui1_head += 2;

        i4_ret = MINFOR_OK;
        for ( ui4_idx = 0; ui4_idx < EXT_CONT_NAME_MAX; ++ui4_idx )
        {
            if ( !x_strncmp( ac_ext_des_name[ui4_idx], ( const CHAR* )pui1_str, ui2_name_len / 2 ) )
            {
                pt_name_value = & (pt_ext_content_des->at_name_value[ui4_idx] );
                pt_name_value->pc_name      = ( CHAR* )ac_ext_des_name[ui4_idx];
                pt_name_value->at_data_type = ( ASF_DATA_TYPE )ui2_type;
                pt_name_value->ui4_data_len = ( UINT32 )ui2_data_len;
                switch ( ui2_type )
                {
                    case DATA_TYPE_WC:
                    case DATA_TYPE_BYTE_ARR:
                        pt_name_value->u_data.pv_value = _mm_util_mem_alloc( ui2_data_len );
                        if ( NULL == pt_name_value->u_data.pv_value )
                        {
                            DBG_ERROR( ( "%s#Line%d:fail to alloc mem for %d\r\n",
                                       __FUNCTION__, __LINE__, ui2_type ) );
                            i4_ret = MINFOR_OUT_OF_MEMORY;
                            break;
                        }
                        x_memcpy( pt_name_value->u_data.pv_value, pui1_head, ui2_data_len ); 
                        break;
                    case DATA_TYPE_BOOL:
                        ASF_GET_DWORD( ui4_bool_value, pui1_head ); /*bool is 4  bytes long*/    
                        pt_name_value->u_data.b_value = ( ui4_bool_value != 0 )? TRUE : FALSE;
                        break;
                    case DATA_TYPEA_DWORD:
                        ASF_GET_DWORD( pt_name_value->u_data.ui4_value, pui1_head );
                        break;
                    case DATA_TYPEA_QWORD:
                        ASF_GET_QWORD( pt_name_value->u_data.ui8_value, pui1_head );
                        break;
                    case DATA_TYPE_WORD:
                        ASF_GET_WORD( pt_name_value->u_data.ui2_value, pui1_head );
                        break;
                    default:  
                        DBG_ERROR( ( "%s#Line%d:unknown data type %u\r\n", 
                                     pt_ext_content_des->at_name_value[ui4_idx].at_data_type ) );
                }

                if ( i4_ret != MINFOR_OK )
                {
                    break;
                }
            }
        }
        
        if ( i4_ret != MINFOR_OK )
        {
            break;
        }
        
        pui1_head += ui2_data_len; 

        --ui2_entry_cnt;
       
    }

    /*if error free resource*/
    if ( i4_ret != MINFOR_OK )
    {
        for ( ui4_idx = 0; ui4_idx < EXT_CONT_NAME_MAX; ++ui4_idx )
        {
            pt_name_value = &( pt_ext_content_des->at_name_value[ui4_idx] );
            switch ( pt_name_value->at_data_type )
            {
                case DATA_TYPE_WC:
                case DATA_TYPE_BYTE_ARR:
                    if ( pt_name_value->u_data.pv_value )
                    {
                        _mm_util_mem_free( pt_name_value->u_data.pv_value );
                        pt_name_value->u_data.pv_value = NULL;
                    }
                    break;
                case DATA_TYPE_BOOL:
                    break;
                case DATA_TYPEA_DWORD:
                    break;
                case DATA_TYPEA_QWORD:
                    break;
                case DATA_TYPE_WORD:
                    break;
                default:  
                    DBG_ERROR( ( "%s#Line%d:unknown data type %u ",
                                 __FUNCTION__, __LINE__,
                                 pt_ext_content_des->at_name_value[ui4_idx].at_data_type ) );
            }

        }
      
    }
    else /*set some shortcut for easy access*/
    {
        pt_ext_content_des->pwsz_year = ( UTF16_T* )( pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_YEAR].u_data.pv_value );
        pt_ext_content_des->pwsz_album_title = ( UTF16_T* )( pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_ALBUM_TITLE].u_data.pv_value );
        pt_ext_content_des->pwsz_album_artist = ( UTF16_T* )( pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_ALBUM_ARTIST].u_data.pv_value );
        pt_ext_content_des->pwsz_genre = ( UTF16_T* ) ( pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_GEBRE].u_data.pv_value );
        
        pt_ext_content_des->b_stereo_scopic = pt_ext_content_des->at_name_value
                                      [EXT_CONT_NAME_STEREOSCOPIC].u_data.b_value;
        pt_ext_content_des->b_half_width = pt_ext_content_des->at_name_value
                                      [EXT_CONT_NAME_STEREOSCOPIC_HALF_WIDTH].u_data.b_value;
        pt_ext_content_des->b_half_height = pt_ext_content_des->at_name_value
                                      [EXT_CONT_NAME_STEREOSCOPIC_HALF_HEIGHT].u_data.b_value;
        
        pt_ext_content_des->pwsz_stereoscopic_layout = ( UTF16_T* )( pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_STEREOSCOPIC_LAYOUT].u_data.pv_value );
        
        pt_ext_content_des->e_scopic_type = _asf_stereoscopic_layout( 
              pt_ext_content_des->pwsz_stereoscopic_layout,
              pt_ext_content_des->at_name_value[EXT_CONT_NAME_STEREOSCOPIC_LAYOUT].ui4_data_len );
        
        pt_ext_content_des->ui8_skip_dur = pt_ext_content_des->at_name_value
              [EXT_CONT_NAME_STEREOSCOPIC_SKIP].u_data.ui8_value;
        
    }
    
    if ( pui1_str )
    {
        _mm_util_mem_free( pui1_str );
    }
    
    if ( i4_ret == MINFOR_OK )
    {
        DBG_INFO( ( "%s#Line%d:end loading extended content description object\r\n",
                    __FUNCTION__, __LINE__ ) );
    }
    else
    {
        DBG_ERROR( ( "%s#Line%d:fail to load extended content description object\r\n",
                    __FUNCTION__, __LINE__ ) );
    }
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_content_description
 *
 * Description: This function free the extended content description object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the content description object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-21 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_ext_content_description ( MINFO_OBJ_T*       pt_minfo_obj, 
                                      ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_bitrate_properties
 *
 * Description: This function gets the bitrate properties object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the bitrate properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_bitrate_properties( MINFO_OBJ_T*      pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T* pt_this_obj )
{

    copybytes_fct                     pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*        pt_asf_instance_obj; 
    /*read buffer pointer*/     
    UINT8*                            pui1_head;     
    /*Biterate Properties Object*/
    ASF_BITRATE_PROPERTIES_OBJECT_T*  pt_bitrate_properties;
    ASF_BITRATE_REC_T*                pt_bitrate_record;

    UINT32                            ui4_read_size;
    UINT16                            ui2_flag;
    INT32                             i4_ret;
    UINT32                            ui4_idx;

    DBG_INFO(("_asf_load_bitrate_properties:begin to bitrate properties "
              "description\r\n"));
    
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_bitrate_properties:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*init variables*/

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_bitrate_properties:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_bitrate_properties:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to content description object memory*/
    pt_bitrate_properties = &(pt_asf_instance_obj->t_bitrate_properties);
    pt_this_obj->pv_obj_specific = (VOID*)pt_bitrate_properties;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_bitrate_properties:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_bitrate_properties:error object (%d)!\r\n", 
                   ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }

    /*get record count, 2 bytes*/
    ASF_GET_WORD(pt_bitrate_properties->ui2_rec_count, pui1_head);
    pui1_head += 2;  

    /*get record info*/
    if (pt_bitrate_properties->ui2_rec_count > 0)
    {
        pt_bitrate_properties->pt_record = (ASF_BITRATE_REC_T*)
                  _mm_util_mem_alloc(pt_bitrate_properties->ui2_rec_count * 
                                     sizeof(ASF_BITRATE_REC_T));
        if (NULL == pt_bitrate_properties->pt_record)
        {
            DBG_ERROR(("_asf_load_bitrate_properties:alloc error!\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_bitrate_record = pt_bitrate_properties->pt_record;
        
        for (ui4_idx = 0; ui4_idx < pt_bitrate_properties->ui2_rec_count; ++ui4_idx)
        {
            /*get flag. bit0~bit6 is stream number*/
            ASF_GET_WORD(ui2_flag, pui1_head);
            pui1_head += 2;  
            pt_bitrate_record[ui4_idx].ui1_stream_num = (UINT8)(ui2_flag & 0x007f);
            /*get stream bitrate*/
            ASF_GET_DWORD(pt_bitrate_record[ui4_idx].ui4_bitrate, pui1_head);
            pui1_head += 4; 
        }
    } 

    
    DBG_INFO(("_asf_load_bitrate_properties:end loading bitrate "
              "properties object\r\n"));
    return MINFOR_OK;

}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_bitrate_properties
 *
 * Description: This function free the bitrate properties object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the bitrate properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-21 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_bitrate_properties( MINFO_OBJ_T*       pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_content_encryption
 *
 * Description: This function gets the content encryption object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the bitrate properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_content_encryption( MINFO_OBJ_T*      pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T* pt_this_obj )
{

    copybytes_fct                     pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*        pt_asf_instance_obj; 
    /*read buffer pointer*/     
    UINT8*                            pui1_head;     
    /*Biterate Properties Object*/
    ASF_CONTENT_ENCRYPTION_OBJECT_T*  pt_content_encryption;

    UINT32                            ui4_read_size;
    INT32                             i4_ret = MINFOR_OK;

    DBG_INFO(("_asf_load_content_encryption:begin to load content encryption object\r\n"));
    
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_content_encryption:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*init variables*/

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_content_encryption:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_content_encryption:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to content description object memory*/
    pt_content_encryption        = &(pt_asf_instance_obj->t_content_encryption);
    pt_this_obj->pv_obj_specific = (VOID*)pt_content_encryption;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_content_encryption:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_content_encryption:error object (%d)!\r\n", 
                   ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }

    do 
    {
        /*get secret data length, 4 bytes*/
        ASF_GET_DWORD(pt_content_encryption->ui4_secret_data_len, pui1_head);
        pui1_head += 4;
        
        /*get secret data,bytes*/
        if (pt_content_encryption->ui4_secret_data_len != 0)
        {
            pt_content_encryption->pui1_secret_data = (UINT8*)
                      _mm_util_mem_alloc(pt_content_encryption->ui4_secret_data_len);
            if (NULL == pt_content_encryption->pui1_secret_data)
            {
                DBG_ERROR(("_asf_load_content_encryption:secret_data is null!\r\n"));
                i4_ret = MINFOR_OUT_OF_MEMORY;
                break;
            }
            x_memcpy((VOID*)(pt_content_encryption->pui1_secret_data), 
                    (VOID*)pui1_head, 
                     pt_content_encryption->ui4_secret_data_len);
            pui1_head += pt_content_encryption->ui4_secret_data_len;
        }
        
        /*get protect type length*/
        ASF_GET_DWORD(pt_content_encryption->ui4_protect_type_len, pui1_head);
        pui1_head += 4;
        
        /*get protect type ,null terminated ASCII string*/
        if (pt_content_encryption->ui4_protect_type_len != 0)
        {
            pt_content_encryption->pi1_protect_type = (CHAR*)
                      _mm_util_mem_alloc(pt_content_encryption->ui4_protect_type_len);
            if (NULL == pt_content_encryption->pi1_protect_type)
            {
                DBG_ERROR(("_asf_load_content_encryption:protect_type is null!\r\n"));
                i4_ret = MINFOR_OUT_OF_MEMORY;
                break;
            }
            x_memcpy((VOID*)(pt_content_encryption->pi1_protect_type), 
                    (VOID*)pui1_head, 
                     pt_content_encryption->ui4_protect_type_len);
            pui1_head += pt_content_encryption->ui4_protect_type_len;
        }
        
        /*get key ID length*/
        ASF_GET_DWORD(pt_content_encryption->ui4_key_id_len, pui1_head);
        pui1_head += 4;
        
        /*get key ID ,null terminated ASCII string*/
        if (pt_content_encryption->ui4_key_id_len != 0)
        {
            pt_content_encryption->pi1_key_id = (CHAR*)
                      _mm_util_mem_alloc(pt_content_encryption->ui4_key_id_len);
            if (NULL == pt_content_encryption->pi1_key_id)
            {
                DBG_ERROR(("_asf_load_content_encryption:key_id is null!\r\n"));
                i4_ret = MINFOR_OUT_OF_MEMORY;
                break;
            }
            x_memcpy((VOID*)(pt_content_encryption->pi1_key_id), 
                    (VOID*)pui1_head, 
                     pt_content_encryption->ui4_key_id_len);
            pui1_head += pt_content_encryption->ui4_key_id_len;
        }
        
        /*get URL length*/
        ASF_GET_DWORD(pt_content_encryption->ui4_lic_url_len, pui1_head);
        pui1_head += 4;
        
        /*get URL , null terminated ASCII string*/
        if (pt_content_encryption->ui4_lic_url_len != 0)
        {
            pt_content_encryption->pi1_lic_url = (CHAR*)
                      _mm_util_mem_alloc(pt_content_encryption->ui4_lic_url_len);
            if (NULL == pt_content_encryption->pi1_lic_url)
            {
                DBG_ERROR(("_asf_load_content_encryption:lic_url is null!\r\n"));
                i4_ret = MINFOR_OUT_OF_MEMORY;
                break;
            }
            x_memcpy((VOID*)(pt_content_encryption->pi1_lic_url), 
                    (VOID*)pui1_head, 
                     pt_content_encryption->ui4_lic_url_len);
            pui1_head += pt_content_encryption->ui4_lic_url_len;
        }
        
        DBG_INFO(("_asf_load_content_encryption:end loading content encryption object\r\n"));
        return MINFOR_OK;
    } while (0);

    if (NULL != pt_content_encryption->pui1_secret_data)
    {
        _mm_util_mem_free(pt_content_encryption->pui1_secret_data);
        pt_content_encryption->pui1_secret_data = NULL;
    }
    
    if (NULL != pt_content_encryption->pi1_protect_type)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_protect_type);
        pt_content_encryption->pi1_protect_type = NULL;
    }
    if (NULL != pt_content_encryption->pi1_key_id)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_key_id);
        pt_content_encryption->pi1_key_id = NULL;
    }
    if (NULL != pt_content_encryption->pi1_lic_url)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_lic_url);
        pt_content_encryption->pi1_lic_url = NULL;
    }
    
    return MINFOR_OUT_OF_MEMORY;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_free_content_encryption
 *
 * Description: This function free the content encryption object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the bitrate properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2009-06-18 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_content_encryption( MINFO_OBJ_T*       pt_minfo_obj, 
                                    ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_language_list
 *
 * Description: This function gets the language list object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the language list object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_language_list( MINFO_OBJ_T*      pt_minfo_obj, 
                               ASF_OBJ_COMMON_T* pt_this_obj )
{

    copybytes_fct               pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj; 
    /*read buffer pointer*/    
    UINT8*                      pui1_head;     
    /*Language List Object*/
    ASF_LANGUAGE_LIST_OBJECT_T* pt_language;
    ISO_639_LANG_T              t_lang_2char;

    UINT32                      ui4_read_size;
    UINT32                      ui4_lang_count;
    UINT8                       ui1_lang_length;
    INT32                       i4_ret;

    DBG_INFO(("_asf_load_language_list:begin to load language list\r\n"));
    
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_language_list:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*init variables*/
    ui4_read_size  = 0;
    ui4_lang_count = 0;
    ui1_lang_length = 0;

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_language_list:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_language_list:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to language list object memory*/
    pt_language = &(pt_asf_instance_obj->t_language);
    pt_this_obj->pv_obj_specific = (void *)pt_language;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_language_list:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_language_list:error object header (%d)!\r\n", 
                ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }

    /*get language count, 2 bytes*/
    ASF_GET_WORD((pt_language->ui2_lang_count), pui1_head);
    pui1_head += 2;   

    if(0 != pt_language->ui2_lang_count)
    {
        pt_language->pt_languge = (ISO_639_LANG_T *) 
                                  _mm_util_mem_alloc(sizeof(ISO_639_LANG_T) 
                                    * pt_language->ui2_lang_count);
        if (NULL != pt_language->pt_languge)
        {
            x_memset(pt_language->pt_languge, 0, sizeof(ISO_639_LANG_T) 
                     * pt_language->ui2_lang_count);

            /*get language list*/
            for (ui4_lang_count = 0; ui4_lang_count < pt_language->ui2_lang_count; 
                 ++ui4_lang_count)
            {
                /*Get language length, 1 bytes*/
                ASF_GET_BYTE(ui1_lang_length, pui1_head);
                pui1_head += 1; 

                /*get language*/
                _asf_lang_rfc1766_to_639(pui1_head, 
                                         (UINT32)ui1_lang_length, 
                                         &t_lang_2char);
                _mm_lang_639_2char_to_3char(&t_lang_2char,
                                            &(pt_language->pt_languge[ui4_lang_count]));

                pui1_head += ui1_lang_length;
            }
            
        }
        else
        {
            DBG_ERROR(("_asf_load_language_list:failed to allocate for "
                       "pt_languge!\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
    }
    
    DBG_INFO(("_asf_load_language_list:end loading language list\r\n"));
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_language_list
 *
 * Description: This function free the language list object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the language list object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_language_list ( MINFO_OBJ_T*       pt_minfo_obj, 
                                ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_codec_list
 *
 * Description: This function gets the codec list object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the codec list object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_codec_list(MINFO_OBJ_T*      pt_minfo_obj, 
                           ASF_OBJ_COMMON_T* pt_this_obj)
{
    INT32                       i4_ret;
    copybytes_fct               pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj;  
    UINT8*                      pui1_head;     /*read buffer pointer*/
    ASF_CODEC_LIST_OBJECT_T *   pt_codec_list;/*Codec List Object*/
    UINT32                      ui4_read_size;
    UINT32                      ui4_codec_count;
    UINT16                      ui2_code_length ;
    #ifdef CLI_LVL_ALL
    CHAR                        ac_temp[40];
    UINT32                      ui4_len;
    #endif

    DBG_INFO(("_asf_load_codec_list:begin to load codec list\r\n"));

    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_codec_list:para is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*init variables*/
    ui4_read_size = 0;
    ui4_codec_count = 0;
    ui2_code_length = 0;
    #ifdef CLI_LVL_ALL
    x_memset(ac_temp, 0, 40);
    ui4_len = 40;
    #endif
    
    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_codec_list:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_codec_list:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to codec list object memory*/
    pt_codec_list = &(pt_asf_instance_obj->t_codec_list);
    pt_this_obj->pv_obj_specific = (void *)pt_codec_list;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_codec_list:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_codec_list:error object (%d)!\r\n", 
                ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
    

    /*get reserved, GUID 16 bytes*/
    /*Get stream type GUID 16 bytes*/
    _asf_get_guid(&(pt_codec_list->t_reserved), pui1_head);
    pui1_head += 16; 
    if (ASFR_GUID_EQUAL != _asf_cmp_guid(&(pt_codec_list->t_reserved), 
                                         &gt_asf_guid_reserved_2))
    {
        #ifdef CLI_LVL_ALL
        _asf_guid_to_str(&(pt_codec_list->t_reserved), ac_temp, &ui4_len);
        DBG_ERROR(("_asf_load_codec_list:error reserved %s\r\n", ac_temp));
        #endif
        return MINFOR_FMT_NOT_SUPPORT;
    }

    /*get codec count*/
    ASF_GET_DWORD(pt_codec_list->ui4_codec_entry_count, pui1_head);
    pui1_head += 4;   

    if (pt_codec_list->ui4_codec_entry_count != 0)
    {
        pt_codec_list->pt_codec = (ASF_CODEC_ENTRY_T *)
                              _mm_util_mem_alloc(pt_codec_list->ui4_codec_entry_count
                              * sizeof(ASF_CODEC_ENTRY_T));
        if (NULL == pt_codec_list->pt_codec)
        {
            DBG_ERROR(("_asf_load_codec_list:failed to allocate memory "
                       "for pt_codec\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        
        x_memset(pt_codec_list->pt_codec, 0, pt_codec_list->ui4_codec_entry_count
                  * sizeof(ASF_CODEC_ENTRY_T));
        
        /*get codec list*/
        for (ui4_codec_count = 0; 
             ui4_codec_count < pt_codec_list->ui4_codec_entry_count; 
             ++ui4_codec_count)
        {
            /*Get codec type, 2 bytes*/
            ASF_GET_WORD(pt_codec_list->pt_codec[ui4_codec_count].ui2_type, 
                         pui1_head);
            pui1_head += 2; 
       
            /*Get codec name length, 2 bytes*/
            ASF_GET_WORD(ui2_code_length, pui1_head);
            pui1_head += 2; 
            ui2_code_length *= 2;

            if (0 != ui2_code_length)
            {
                /*get codec name*/
                pt_codec_list->pt_codec[ui4_codec_count].pwsz_name = 
                                    (UTF16_T *)_mm_util_mem_alloc((SIZE_T)(ui2_code_length + 2));
                if (NULL !=  pt_codec_list->pt_codec[ui4_codec_count].pwsz_name)
                {
                     x_memcpy((VOID *)(pt_codec_list->pt_codec
                              [ui4_codec_count].pwsz_name),
                             pui1_head, ui2_code_length);
                     *((UTF16_T *)((CHAR *)(pt_codec_list->pt_codec
                     [ui4_codec_count].pwsz_name) 
                     + ui2_code_length)) = 0;
                   
                }
                else
                {
                    DBG_ERROR(("_asf_load_codec_list:failed to allocate memory"
                               " for pwsz_name\r\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }
            }

            pui1_head += ui2_code_length;

            /*Get codec description length, 2 bytes*/
            ASF_GET_WORD(ui2_code_length, pui1_head);
            pui1_head += 2; 
            ui2_code_length *= 2;

            if (0 != ui2_code_length)
            {
                /*get codec description*/
                pt_codec_list->pt_codec[ui4_codec_count].pwsz_desc = 
                                    (UTF16_T *)_mm_util_mem_alloc((SIZE_T)(ui2_code_length + 2));
                if (NULL !=  pt_codec_list->pt_codec[ui4_codec_count].pwsz_desc)
                {
                     x_memcpy((VOID *)(pt_codec_list->pt_codec
                             [ui4_codec_count].pwsz_desc),
                             pui1_head, ui2_code_length);
                     *((UTF16_T *)((CHAR *)(pt_codec_list->pt_codec
                     [ui4_codec_count].pwsz_desc) 
                     + ui2_code_length))  = 0;
                   
                }
                else
                {
                    DBG_ERROR(("_asf_load_codec_list:failed to allocate memory "
                               "for pwsz_desc\r\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }
            }

            pui1_head += ui2_code_length;

            /*Get codec info length, 2 bytes*/
            ASF_GET_WORD(ui2_code_length, pui1_head);
            pui1_head += 2; 
            pt_codec_list->pt_codec[ui4_codec_count].ui2_info_len = ui2_code_length;

            if (0 != ui2_code_length)
            {
                /*get codec info*/
                pt_codec_list->pt_codec[ui4_codec_count].pui1_info = 
                            (UINT8*)_mm_util_mem_alloc((SIZE_T)(ui2_code_length));
                if (NULL !=  pt_codec_list->pt_codec[ui4_codec_count].pui1_info)
                {
                     x_memcpy((VOID *)(pt_codec_list->pt_codec
                              [ui4_codec_count].pui1_info),
                              pui1_head, ui2_code_length);
                }
                else
                {
                    DBG_ERROR(("_asf_load_codec_list:failed to allocate memory"
                               " for pui1_info\r\n"));
                    return MINFOR_OUT_OF_MEMORY;
                }
            }
            pui1_head += ui2_code_length;
        
        }
        
    }

    DBG_INFO(("_asf_load_codec_list:end loading codec list\r\n"));
  
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_free_codec_list
 *
 * Description: This function free the codec list object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the codec list object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial
 *
 ----------------------------------------------------------------------------*/
INT32 _asf_free_codec_list ( MINFO_OBJ_T*       pt_minfo_obj, 
                             ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_ex_stream_properties
 *
 * Description: This function gets the extended stream properties object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the extended stream properties object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:    NULL.
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_ex_stream_properties(MINFO_OBJ_T*      pt_minfo_obj, 
                                     ASF_OBJ_COMMON_T* pt_this_obj)
{
    copybytes_fct                      pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*         pt_asf_instance_obj;  
    UINT8*                             pui1_head; /*read buffer pointer*/   
    MINFO_ASF_PER_STRM_INFO_T*         pt_per_strm_info;
    /* Extended Stream Object*/
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_object;
    ASF_EX_STREAM_PROPERTIES_OBJECT_T  t_ex_stream_object;
    ASF_STREAM_NAME_T*                 pt_stream_name;
    ASF_PAYLOAD_EX_SYS_T*              pt_payload_ex_system;
    
    ASF_OBJ_COMMON_T*                  pt_sub_obj ;
    ASF_OBJ_COMMON_T*                  pt_last_sub_obj ;

    
    UINT32                             ui4_read_size;
    /*extended stream properties num*/
    UINT32                             ui4_idx;
    UINT16                             ui2_name_len;
    UINT32                             ui4_name_total_len;
    UINT32                             ui4_payload_system_total_len;
    INT32                              i4_ret;

    DBG_INFO(("_asf_load_ex_stream_properties:begin to load extended stream "
              "properties\r\n"));
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*init variables*/
    ui4_read_size = 0;
    ui4_idx = 0;
    ui2_name_len = 0;
    ui4_name_total_len = 0;
    ui4_payload_system_total_len = 0;
    x_memset((VOID*)(&t_ex_stream_object), 0, sizeof(ASF_EX_STREAM_PROPERTIES_OBJECT_T));

    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:pt_asf_instance_obj NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*use stack variable to store extended stream properties object*/
    pt_ex_stream_object = &(t_ex_stream_object);

    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:error object header(%u)!\r\n",
                  ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
   
    /*get start time, 8 bytes*/
    ASF_GET_QWORD(pt_ex_stream_object->ui8_start_time, pui1_head);
    pui1_head += 8;   

    /*get end time, 8 bytes*/
    ASF_GET_QWORD(pt_ex_stream_object->ui8_end_time, pui1_head);
    pui1_head += 8;   

    /*get data bitrate, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_bitrate, pui1_head);
    pui1_head += 4;   

    /*get buffer size, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_buf_size, pui1_head);
    pui1_head += 4;   
    
    /*get initial buffer size, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_init_buf_size, pui1_head);
    pui1_head += 4;   

    /*get alter data bitrate, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_alter_bitrate, pui1_head);
    pui1_head += 4;   

    /*get alter buffer size, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_alter_buf_size, pui1_head);
    pui1_head += 4;   

    /*get alter initial buffer size, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_alter_init_buf_size, pui1_head);
    pui1_head += 4;   

    /*get maximum object size, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_max_obj_size, pui1_head);
    pui1_head += 4;   

    /*get flags, 4 bytes*/
    ASF_GET_DWORD(pt_ex_stream_object->ui4_flags, pui1_head);
    pui1_head += 4;   

    /*get stream number, 2 bytes*/
    ASF_GET_WORD(pt_ex_stream_object->ui2_stream_num, pui1_head);
    pui1_head += 2;   

    /*get stream language ID index, 2 bytes*/
    ASF_GET_WORD(pt_ex_stream_object->ui2_language_idx, pui1_head);
    pui1_head += 2;  

    /*get average time per frame , 8 bytes*/
    ASF_GET_QWORD(pt_ex_stream_object->ui8_average_time_per_frame, 
                  pui1_head);
    pui1_head += 8;   

    /*get stream name counts, 2 bytes*/
    ASF_GET_WORD(pt_ex_stream_object->ui2_stream_name_count, pui1_head);
    pui1_head += 2;   

    /*get payload system counts, 2 bytes*/
    ASF_GET_WORD(pt_ex_stream_object->ui2_payload_ex_sys_count, pui1_head);
    pui1_head += 2;   

    /*Have known stream number so search in _MINFO_ASF_STRM_INFO_T for this 
      stream ,if not found allocate MINFO_ASF_PER_STRM_INFO_T for
      this stream
   */
    /*get stream info node*/
    pt_per_strm_info = _asf_search_strm_info_by_id(pt_minfo_obj,
                       pt_ex_stream_object->ui2_stream_num);

    if (NULL == pt_per_strm_info)
    {
        DBG_ERROR(("_asf_load_ex_stream_properties:pt_per_strm_info NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*copy stack pt_strm_properties to pt_per_strm_info*/
    pt_ex_stream_object = pt_per_strm_info->pt_ex_stream_obj;
    if (NULL == pt_ex_stream_object)
    {
        pt_ex_stream_object = (ASF_EX_STREAM_PROPERTIES_OBJECT_T* )
                          _mm_util_mem_alloc(sizeof(ASF_EX_STREAM_PROPERTIES_OBJECT_T));
        if (NULL == pt_ex_stream_object)
        {
            DBG_ERROR(("_asf_load_stream_properties:failed to allocate "
                       "memory for pt_ex_stream_object\r\n"));
            /*do not free memory here*/
            return MINFOR_OUT_OF_MEMORY;
        } 
        pt_per_strm_info->pt_ex_stream_obj = pt_ex_stream_object;
    }
    /*store stream properties info*/
    x_memcpy(pt_ex_stream_object, 
             &t_ex_stream_object, 
             sizeof(ASF_EX_STREAM_PROPERTIES_OBJECT_T));
    /*set specific pointer for pt_this_obj*/
    pt_this_obj->pv_obj_specific = (VOID *)pt_ex_stream_object;

    /*get names*/
    if (pt_ex_stream_object->ui2_stream_name_count != 0)
    {
    
        pt_ex_stream_object->pt_stream_name = (ASF_STREAM_NAME_T *) _mm_util_mem_alloc(
                            (SIZE_T)((pt_ex_stream_object->ui2_stream_name_count)
                            * sizeof(ASF_STREAM_NAME_T)));
        if (pt_ex_stream_object->pt_stream_name != NULL)
        {
            x_memset((VOID*)pt_ex_stream_object->pt_stream_name, 0, 
                     (pt_ex_stream_object->ui2_stream_name_count) 
                     * sizeof(ASF_STREAM_NAME_T));
            for (ui4_idx = 0; ui4_idx < pt_ex_stream_object->ui2_stream_name_count; 
                 ++ui4_idx)
            {
                pt_stream_name = pt_ex_stream_object->pt_stream_name + ui4_idx;
                /*Get name language ID*/
                ASF_GET_WORD(pt_stream_name->ui2_strm_name_lang_idx, 
                                   pui1_head);
                pui1_head += 2;   
                /*get name length , Note: the unit is byte or word? i use byte*/
                ASF_GET_WORD(ui2_name_len, pui1_head);
                pui1_head += 2;
                
                if (ui2_name_len != 0)
                {
                    /*get name*/
                    pt_stream_name->pwsz_stream_name 
                                     = (UTF16_T *)_mm_util_mem_alloc((SIZE_T)(ui2_name_len + 2));
                    if (pt_stream_name->pwsz_stream_name 
                        != NULL)
                    {
                        x_memcpy(pt_stream_name->pwsz_stream_name, 
                                 pui1_head, 
                                 ui2_name_len);
                        *((UTF16_T*)((CHAR*)(pt_stream_name->pwsz_stream_name)
                                   + ui2_name_len))= 0;
                    }
                    else
                    {
                        DBG_ERROR(("_asf_load_ex_stream_properties:fail to "
                                   "allocate memory for pwsz_stream_name\r\n"));
                        return MINFOR_OUT_OF_MEMORY;
                    }
                    
                }

                pui1_head += ui2_name_len;
                /*name language ID + name length + name*/
                ui4_name_total_len += (2 + 2 + ui2_name_len);
            }
        }
        else
        {
            DBG_ERROR(("_asf_load_ex_stream_properties:fail to allocate memory"
                       " for pt_stream_name\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
    }
    else
    {
        pt_ex_stream_object->pt_stream_name = NULL;
        ui4_name_total_len = 0;
    }

    /*get payload system*/
    if (pt_ex_stream_object->ui2_payload_ex_sys_count != 0)
    {
        pt_ex_stream_object->pt_payload_ex_system = (ASF_PAYLOAD_EX_SYS_T *) 
                 _mm_util_mem_alloc(
                             (SIZE_T)((pt_ex_stream_object->ui2_payload_ex_sys_count)
                             * sizeof(ASF_PAYLOAD_EX_SYS_T))
                             );

        if (pt_ex_stream_object->pt_payload_ex_system != NULL)
        {
            
            x_memset((VOID *) pt_ex_stream_object->pt_payload_ex_system, 0, 
                              (pt_ex_stream_object->ui2_payload_ex_sys_count)
                               * sizeof(ASF_PAYLOAD_EX_SYS_T));
        
            for (ui4_idx = 0; 
                 ui4_idx < pt_ex_stream_object->ui2_payload_ex_sys_count; 
                 ++ui4_idx)
            {
                pt_payload_ex_system = pt_ex_stream_object->pt_payload_ex_system
                                       + ui4_idx;
                /*get GUID , 16 bytes*/
                _asf_get_guid(&(pt_payload_ex_system->t_guid),
                              pui1_head);
                pui1_head += 16; 

                /*get object size*/
                 ASF_GET_WORD(pt_payload_ex_system->ui2_size,
                                    pui1_head);
                pui1_head += 2; 

                /*get info length*/
                 ASF_GET_DWORD(pt_payload_ex_system->ui4_ex_sys_info_len, 
                                     pui1_head);
                pui1_head += 4; 
                
                if (pt_payload_ex_system->ui4_ex_sys_info_len 
                    != 0)
                {
                    pt_payload_ex_system->pui1_ex_sys_info 
                          = (UINT8*) _mm_util_mem_alloc
                          (pt_payload_ex_system->ui4_ex_sys_info_len);
                    if (pt_payload_ex_system->pui1_ex_sys_info 
                        != NULL)
                    {
                        x_memcpy(pt_payload_ex_system->pui1_ex_sys_info,
                                 pui1_head, 
                                 pt_payload_ex_system->ui4_ex_sys_info_len);
                    }
                    else
                    {
                        DBG_ERROR(("_asf_load_ex_stream_properties:fail to "
                                   "allocate memory for pui1_ex_sys_info\r\n"));
                        return MINFOR_INTERNAL_ERR;
                    }
                }
                
                pui1_head += pt_payload_ex_system->ui4_ex_sys_info_len;
                /*GUID + object size + info length + info*/
                ui4_payload_system_total_len +=
                       (16 + 2 + 4 
                       + pt_payload_ex_system->ui4_ex_sys_info_len);
                
            }

        }
        else
        {
            DBG_ERROR(("_asf_load_ex_stream_properties:fail to allocate memory"
                       " for pt_payload_ex_system\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        
    }
    else
    {
        pt_ex_stream_object->pt_payload_ex_system = NULL ;
        ui4_payload_system_total_len = 0;
    }
    
    pt_this_obj->ui8_offset = ((MINFO_ASF_INSTANCE_INFO_T*)
                         (pt_minfo_obj->pv_handler_obj))->ui8_cur_parser_offset;
    /*88 is the fix lenght of this object*/
    ASF_SET_CUR_PARSE_POS(pt_minfo_obj, (88 + ui4_name_total_len 
                          + ui4_payload_system_total_len), i4_ret);
    if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
    {   
        DBG_ERROR(("_asf_load_ex_stream_properties:self\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    #ifdef CLI_LVL_ALL
    /*dispay the object */
    _asf_dump_object(pt_this_obj);
    #endif

    /*get stream properties*/
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    pt_ex_stream_object->pt_strm_properties = NULL;
    /*88 is the fix lenght of this object*/
    if (pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN > 
            (88 + ui4_name_total_len + ui4_payload_system_total_len))
    {

        pt_last_sub_obj = pt_this_obj->pt_first_child;
        /*begin get stream objects,  stream counts is not than 127*/
        while (TRUE)
        {

            pt_sub_obj = (ASF_OBJ_COMMON_T*)
                         _mm_util_mem_alloc(sizeof(ASF_OBJ_COMMON_T));
            if (NULL == pt_sub_obj)
            {
                DBG_ERROR(("_asf_load_ex_stream_properties:failed to "
                            "allocate memory for pt_sub_obj!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            x_memset(pt_sub_obj, 0, sizeof(ASF_OBJ_COMMON_T));


            if (NULL == pt_this_obj->pt_first_child)
            {
                pt_this_obj->pt_first_child = pt_sub_obj;
            }
            else
            {
                pt_last_sub_obj->pt_next_sibling = pt_sub_obj;
            }
            pt_last_sub_obj = pt_sub_obj;
            
            i4_ret = _asf_load_object(pt_minfo_obj, pt_sub_obj, pt_this_obj);
            if (MINFOR_OK != i4_ret)
            {
                if (MINFOR_FMT_NOT_SUPPORT == i4_ret)
                {
                    /*skip it ,continue*/
                    pt_sub_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
                    ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 
                                          pt_sub_obj->ui8_size,
                                          i4_ret);
                    if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
                    {   
                        DBG_ERROR(("_asf_load_ex_stream_properties:sub\r\n"));
                        return MINFOR_INTERNAL_ERR;
                    }
                    /*print object that is not supported*/
                    DBG_INFO(("not supports embodded stream properties "
                              "objects currently\r\n"));

                }
                else
                {
                    DBG_ERROR(("_asf_load_ex_stream_properties: "
                               "_asf_load_object return %d\r\n", 
                               i4_ret));
                    return MINFOR_INTERNAL_ERR;
                }
            
            }

            #ifdef CLI_LVL_ALL
            /*dispay the object */
            _asf_dump_object(pt_sub_obj);
            #endif

            /*if exceeds the extended stream object limit break;*/
            if (pt_sub_obj->ui8_offset + pt_sub_obj->ui8_size 
                == (pt_this_obj->ui8_offset +  pt_this_obj->ui8_size))
            {
                break;
            }
            else if (pt_sub_obj->ui8_offset + pt_sub_obj->ui8_size 
                > (pt_this_obj->ui8_offset +  pt_this_obj->ui8_size))
            {
                DBG_ERROR(("_asf_load_ex_stream_properties:"
                           "ex_stream[%u]:offset=%llu,size=%llu, "
                           "sub offset=%llu, sub size=%llu, sub object "
                           "exceeds boundary\r\n",
                           pt_ex_stream_object->ui2_stream_num,
                           pt_this_obj->ui8_offset,
                           pt_this_obj->ui8_size,
                           pt_sub_obj->ui8_offset,
                           pt_sub_obj->ui8_size));
                return MINFOR_INTERNAL_ERR;
            }
            else
            {   
                /*continue ... normal*/
                continue;
            }
        }

    }
    
    DBG_INFO(("_asf_load_ex_stream_properties:end loading extended stream "
              "properties\r\n"));
    return MINFOR_OK;
    
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_free_ex_stream_properties
 *
 * Description: This function free the extended stream object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the extended  stream_properties
 *                         object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_free_ex_stream_properties ( MINFO_OBJ_T*       pt_minfo_obj, 
                                       ASF_OBJ_COMMON_T** ppt_this_obj )
{
    INT32   i4_ret;
    
    
    /*parameters check ,ignore...*/
    if ((NULL == pt_minfo_obj) 
        || (NULL == ppt_this_obj)
        || (NULL == *ppt_this_obj))
    {
        DBG_ERROR(("_asf_free_ex_stream_properties:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*do extended  stream extention free oper*/
    DBG_INFO(("_asf_free_ex_stream_properties:begin to free "
              "Extented Stream Object\r\n"));

    /*call free function*/
    i4_ret = _asf_free_composite_object(pt_minfo_obj, ppt_this_obj);

    if (MINFOR_OK == i4_ret)
    {
        DBG_INFO(("_asf_free_ex_stream_properties:end freeing "
                  "Extented Stream Object\r\n"));
        return MINFOR_OK;

    }
    else
    {
        DBG_INFO(("_asf_free_ex_stream_properties:failed to free "
                  "Extented Stream Object\r\n"));
        return i4_ret;
    }}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_index_parameters
 *
 * Description: This function gets the index parameters object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the index parameters object
 *           
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:   NULL
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_index_parameters ( MINFO_OBJ_T*      pt_minfo_obj, 
                                   ASF_OBJ_COMMON_T* pt_this_obj )
{
    INT32                        i4_ret;
    copybytes_fct                pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*   pt_asf_instance_obj; 
    /*read buffer pointer*/
    UINT8*                       pui1_head;     
    /*Index Parameters Object*/
    ASF_IDX_PARAMETERS_OBJECT_T* pt_idx_paras;

    UINT32 ui4_read_size  = 0;
    UINT32 ui4_i = 0;

    DBG_INFO(("_asf_load_index_parameters:begin to load index parameters\r\n"));
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_index_parameters:NULL parameters\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_index_parameters:NULL pt_asf_instance_obj\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_index_parameters:NULL pui1_head\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*point to language list object memory*/
    pt_idx_paras = &(pt_asf_instance_obj->t_idx_paras);
    pt_this_obj->pv_obj_specific = (void *)pt_idx_paras;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_index_parameters:NULL pf_copybyte\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_index_parameters:error object header!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }


    /*get time interval, 4 bytes*/
    ASF_GET_DWORD((pt_idx_paras->ui4_time_interval), pui1_head);
    pui1_head += 4;   

    /*get specifers count, 2 bytes*/
    ASF_GET_WORD((pt_idx_paras->ui2_specifers_count), pui1_head);
    pui1_head += 2;   

    if (pt_idx_paras->ui2_specifers_count != 0)
    {
        pt_idx_paras->pt_specifers = (ASF_IDX_SPECIFERS_T *)_mm_util_mem_alloc(
                                    sizeof(ASF_IDX_SPECIFERS_T)
                                    * (pt_idx_paras->ui2_specifers_count));
        if (NULL == pt_idx_paras->pt_specifers)
        {
            DBG_ERROR(("_asf_load_index_parameters:failed to allocate memory "
                       "for pt_specifers!\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_paras->pt_specifers, 0, sizeof(ASF_IDX_SPECIFERS_T)
                 * (pt_idx_paras->ui2_specifers_count));

        /*get specifers */
        for (ui4_i = 0; ui4_i < pt_idx_paras->ui2_specifers_count; ++ui4_i)
        {
            /*Get stream number, 2 bytes*/
            ASF_GET_WORD(pt_idx_paras->pt_specifers[ui4_i].ui2_stream_num, pui1_head);
            pui1_head += 2; 

            /*get index type, 2 bytes*/
             ASF_GET_WORD(pt_idx_paras->pt_specifers[ui4_i].ui2_idx_type, pui1_head);
            pui1_head += 2; 
        }
        
    }
    else
    {
        /**/
    }
    

    DBG_INFO(("_asf_load_index_parameters:end loading index parameters\r\n"));
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_index_parameters
 *
 * Description: This function free the index parameters object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the index parameters object
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_free_index_parameters ( MINFO_OBJ_T*       pt_minfo_obj, 
                                   ASF_OBJ_COMMON_T** ppt_this_obj )
{
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_header_extention
 *
 * Description: This function gets the header extention object info
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the header extention object
 *          
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Note:  NULL
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_load_header_extention( MINFO_OBJ_T*      pt_minfo_obj, 
                                  ASF_OBJ_COMMON_T* pt_this_obj )
{
    copybytes_fct               pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj;  
    ASF_HEADER_EX_OBJECT_T*     pt_header_ex;
    /*read buffer pointer*/
    UINT8*                      pui1_head;     
    ASF_GUID_T                  t_guid;
    UINT16                      ui2_reserved;
    UINT32                      ui4_read_size;
    ASF_OBJ_COMMON_T *          pt_sub_obj;
    ASF_OBJ_COMMON_T *          pt_last_sub_obj;
    INT32                       i4_ret;

    DBG_INFO(("_asf_load_header_extention:begin to load header extention\r\n"));
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_header_extention:NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*init variables*/
    ui2_reserved = 0;
    ui4_read_size = 0;
    x_memset(&t_guid, 0, sizeof(ASF_GUID_T));
    
    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_header_extention:pt_asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_header_extention:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pt_header_ex = &(pt_asf_instance_obj->t_header_ex);
    pt_this_obj->pv_obj_specific = (void *)pt_header_ex;
    
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if (NULL == pf_copybyte)
    {
        DBG_ERROR(("_asf_load_header_extention:pf_copybyte is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*have read the the header of a object, so load attributes directly*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length, 
                         (SIZE_T)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_header_extention:error object header (%d)!\r\n", 
                    ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*get reservered1 guid, 16 bytes*/
    _asf_get_guid(&t_guid, pui1_head);
    pui1_head += 16;   /*GUID is 16 bytes long*/  
    if (ASFR_GUID_EQUAL != _asf_cmp_guid(&t_guid, &gt_asf_guid_reserved_1))
    {
        DBG_ERROR(("_asf_load_header_extention:reserved1 error\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*Get reservered2, 2 bytes, should be 6*/
    ASF_GET_WORD(ui2_reserved, pui1_head);
    pui1_head += 2; 
    if (6 != ui2_reserved)
    {
        DBG_ERROR(("_asf_load_header_extention:reserved2 =%d, is not 6\r\n", 
                ui2_reserved));
        return MINFOR_INTERNAL_ERR;
    }

    /*change current parsing position*/
    pt_this_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
    /*we got 30 byte info ,so offset 46 (only header extention object)*/
    ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 46, i4_ret);
    if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
    {   
        DBG_ERROR(("_asf_load_header_extention:self\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*dispaly header extention object*/
    #ifdef CLI_LVL_ALL
    /*dispay the object */
    _asf_dump_object(pt_this_obj);
    #endif
  
    /**************************************************************************
                    continue to load sub objects 
    **************************************************************************/
    /*get extention header data length*/
    ASF_GET_DWORD(pt_header_ex->ui4_size, pui1_head);
    pui1_head += 4; 

    if (0 == pt_header_ex->ui4_size)
    {
        DBG_INFO(("_asf_load_header_extention: ui4_ex_data_size is 0\r\n"));
        return MINFOR_OK;
    }

    pt_last_sub_obj = pt_this_obj->pt_first_child;
    /*prevent from deadly looping, sub objects are not than 150*/
    while ( TRUE)
    {
    
        pt_sub_obj = (ASF_OBJ_COMMON_T *)_mm_util_mem_alloc(sizeof(ASF_OBJ_COMMON_T));
        if (NULL == pt_sub_obj)
        {
            DBG_ERROR(("_asf_load_header_extention:fail to allocate memory\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_sub_obj, 0, sizeof(ASF_OBJ_COMMON_T));

        /**/
        if (NULL == pt_this_obj->pt_first_child)
        {
            pt_this_obj->pt_first_child = pt_sub_obj;
        }
        else
        {
            pt_last_sub_obj->pt_next_sibling = pt_sub_obj;
        }
        pt_last_sub_obj = pt_sub_obj;
        
        i4_ret = _asf_load_object(pt_minfo_obj, pt_sub_obj, pt_this_obj);
        if (MINFOR_OK != i4_ret)
        {
            if (MINFOR_FMT_NOT_SUPPORT == i4_ret)
            {
                /*skip it ,continue*/
                pt_sub_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
                ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 
                                      pt_sub_obj->ui8_size,
                                      i4_ret);
                if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
                {   
                    DBG_ERROR(("_asf_load_header_extention:sub\r\n"));
                    return MINFOR_INTERNAL_ERR;
                }
            }
            else
            {
                DBG_ERROR(("_asf_load_header_extention:_asf_load_object "
                           "return %d\r\n", 
                           i4_ret));
                return MINFOR_INTERNAL_ERR;
            }
        
        }
        #ifdef CLI_LVL_ALL
        /*dispay the object */
        if (ASF_OBJ_EX_STREAM_PROPERTIES != pt_sub_obj->e_type)
        {
            _asf_dump_object(pt_sub_obj);
        }
        #endif
       

        /*when sub object reach header extention object's boundary break out*/
        if ((pt_sub_obj->ui8_offset + pt_sub_obj->ui8_size) 
            == (pt_this_obj->ui8_offset + pt_this_obj->ui8_size))
        {
            break;
        }
        else if ((pt_sub_obj->ui8_offset + pt_sub_obj->ui8_size) 
            > (pt_this_obj->ui8_offset + pt_this_obj->ui8_size))
        {
            DBG_ERROR(("_asf_load_header_extention:exception path!, "
                "sub ui8_offset=%llu, ui8_size=%llu"
                "this ui8_offset=%llu, ui8_size=%llu\r\n",
                pt_sub_obj->ui8_offset,
                pt_sub_obj->ui8_size,
                pt_this_obj->ui8_offset,
                pt_this_obj->ui8_size));
            return MINFOR_INTERNAL_ERR;

        }
        else
        {
            /*...Normal...*/
        }
        
    }
    
    DBG_INFO(("_asf_load_header_extention:end loading header extention\r\n"));

    return MINFOR_OK;
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_header_extention
 *
 * Description: This function free the header extention object dynamic resource
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the header extention object
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
INT32 _asf_free_header_extention( MINFO_OBJ_T*       pt_minfo_obj, 
                                  ASF_OBJ_COMMON_T** ppt_this_obj )
{
    INT32   i4_ret;
    
    
    /*parameters check ,ignore...*/
    if ((NULL == pt_minfo_obj) 
        || (NULL == ppt_this_obj)
        || (NULL == *ppt_this_obj))
    {
        DBG_ERROR(("_asf_free_header_extention:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    DBG_INFO(("_asf_free_header_extention:begin to free "
              "Header Extension Object\r\n"));
    
    /*call free function*/
    i4_ret = _asf_free_composite_object(pt_minfo_obj, ppt_this_obj);

    if (MINFOR_OK == i4_ret)
    {
        DBG_INFO(("_asf_free_header_extention:end freeing "
                  "Header Extension Object\r\n"));
        return MINFOR_OK;

    }
    else
    {
        DBG_INFO(("_asf_free_header_extention:failed to free "
                  "Header Extension Object\r\n"));
        return i4_ret;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_load_object
 *
 * Description: This function parse a object and will call a object 
 *               specific callback parse function.
 *
 * Inputs:  pt_minfo_obj:  specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of a object
 *           pt_parent_obj: the parent of a object
 *           
 * Outputs: NULL 
 * 
 * Returns: NULL
 *
 * Note:  (1)if the object is not supported ,skip it.
 *
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/

 INT32 _asf_load_object( MINFO_OBJ_T*      pt_minfo_obj, 
                         ASF_OBJ_COMMON_T* pt_this_obj,
                         ASF_OBJ_COMMON_T* pt_parent_obj )
{
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T* pt_asf_instance_obj; 
    setpos_fct                 pf_set_pos;   
    copybytes_fct              pf_copybyte;
    UINT32                     ui4_read_size;
    UINT32                     ui4_Idx;
    INT32                      i4_ret;
    UINT8*                     pui1_head;
                               

    /*check parameters*/
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj) 
        || (NULL == pt_parent_obj))
    {
        DBG_INFO(("_asf_load_object:parameters NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*init variables*/
    ui4_read_size = 0;
    ui4_Idx = 0;
    i4_ret = 0;
    
    /*get asf handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_object:asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /*get temporary buffer*/   
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_object:pui1_head is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    if ((NULL == pf_set_pos) || (NULL == pf_copybyte))
    {
        DBG_INFO(("_asf_load_object:callback NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_this_obj->pt_parent = pt_parent_obj;
    pt_this_obj->pt_first_child = NULL;
    pt_this_obj->pt_next_sibling = NULL;

    /*read object header from file*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)ASF_OBJ_COMMON_LEN, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_object:error object header!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
 
    /*get object guid and size*/
    _asf_get_guid(&(pt_this_obj->t_guid), pui1_head);
    /*GUID is 16 bytes long*/  
    ASF_GET_QWORD(pt_this_obj->ui8_size, pui1_head + 16);  


    /*...used by sun object when asf_load_objects call this routine ...*/
    for (ui4_Idx = 0; 
         ui4_Idx < sizeof(g_at_asf_obj_oper) / sizeof(MINFO_ASF_OBJ_OPER_T); 
         ++ui4_Idx)
    {
        if (ASFR_GUID_EQUAL == _asf_cmp_guid((const ASF_GUID_T *)
                                              &(pt_this_obj->t_guid), 
                                             (const ASF_GUID_T *)
                                             (g_at_asf_obj_oper[ui4_Idx].pt_guid)))
        {
            break;
        }
    }
    
    /*whether is supported?*/
    if(ui4_Idx >= sizeof(g_at_asf_obj_oper) / sizeof(MINFO_ASF_OBJ_OPER_T))
    {
        /*not support, the supper routies should know*/
        DBG_INFO(("_asf_load_object: ui4_Idx=%d\r\n", ui4_Idx));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    
    /*get object enumerator*/
    pt_this_obj->e_type = g_at_asf_obj_oper[ui4_Idx].e_obj_type;
    if (ASF_OBJECT_BUTT == pt_this_obj->e_type)
    {
        /*not support, the supper routies should know*/
        DBG_INFO(("_asf_load_object: ASF_OBJECT_BUTT\r\n"));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    
    /*call its load function */
    if (NULL == g_at_asf_obj_oper[ui4_Idx].pf_load_obj)
    {
        DBG_INFO(("_asf_load_object: g_at_asf_obj_oper[%d]pf_load_obj "
                  "is NULL\r\n", 
                  ui4_Idx));
        return MINFOR_FMT_NOT_SUPPORT;
    }

    /*we should check whether the temporary buffer is enough?*/
    if (pt_asf_instance_obj->ui4_buf_length 
        < (UINT32)(pt_this_obj->ui8_size - ASF_OBJ_COMMON_LEN))
    {
        /*free old buffer*/
        _mm_util_mem_free(pt_asf_instance_obj->pui1_header_buf);
        pt_asf_instance_obj->pui1_header_buf = NULL;

        /*reallocate new buffer ,set new length*/
        pt_asf_instance_obj->pui1_header_buf = (UINT8*) _mm_util_mem_alloc
                                               ((UINT32)pt_this_obj->ui8_size);
        if (NULL == pt_asf_instance_obj->pui1_header_buf)
        {
            DBG_INFO(("_asf_load_object: fail to allocate memory(%d)\r\n ", 
                      pt_this_obj->ui8_size));
            return MINFOR_OUT_OF_MEMORY;
        }
        x_memset(pt_asf_instance_obj->pui1_header_buf, 0, 
                (UINT32)pt_this_obj->ui8_size);
        pt_asf_instance_obj->ui4_buf_length = (UINT32)pt_this_obj->ui8_size;
        pui1_head = pt_asf_instance_obj->pui1_header_buf;
    }
    
    /*call object callback function to get object content*/
    i4_ret = g_at_asf_obj_oper[ui4_Idx].pf_load_obj(pt_minfo_obj, 
                                                    pt_this_obj);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_load_object:asf_load_obj_func return %d!\r\n",
                   i4_ret));
        return MINFOR_FMT_NOT_SUPPORT;
    }
    
    /*set to the beginnig of the object*/
    if ((ASF_OBJ_HEADER_EXTENSION != pt_this_obj->e_type)  
        && (ASF_OBJ_EX_STREAM_PROPERTIES != pt_this_obj->e_type))
    {
        pt_this_obj->ui8_offset = ((MINFO_ASF_INSTANCE_INFO_T*)
                         (pt_minfo_obj->pv_handler_obj))->ui8_cur_parser_offset;
        ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 
                             pt_this_obj->ui8_size,
                             i4_ret);

        if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
        {   
            DBG_ERROR(("_asf_load_object:set pos error\r\n"));
            return MINFOR_INTERNAL_ERR;
        }

        
    }
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_composite_object
 *
 * Description: This function free composite resources
 *
 * Inputs:  pt_minfo_obj:  specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of a object
 *
 * Outputs:  pt_this_obj = NULL , related resources are free
 *
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-17 : initial
           
 ----------------------------------------------------------------------------*/

INT32  _asf_free_composite_object( MINFO_OBJ_T*       pt_minfo_obj,
                                   ASF_OBJ_COMMON_T** ppt_this_obj )
{
    ASF_OBJ_COMMON_T* pt_sub_obj;
    ASF_OBJ_COMMON_T* pt_temp_obj;
    UINT32            ui4_Idx;
    UINT32            ui4_tab_len;
    INT32             i4_ret;

    /*check parameter*/
    if((NULL == pt_minfo_obj)
      || (NULL == ppt_this_obj) 
      || (NULL == *ppt_this_obj))
    {
        DBG_ERROR(("_asf_free_composite_object:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*init varables*/
    ui4_tab_len = sizeof(g_at_asf_obj_oper) / sizeof(MINFO_ASF_OBJ_OPER_T);
    /*get first child*/
    pt_sub_obj = (*ppt_this_obj)->pt_first_child;

    /*free sub objects*/
    while (pt_sub_obj)
    {
        /*...used by sun object when asf_load_objects call this routine ...*/
        for (ui4_Idx = 0; ui4_Idx < ui4_tab_len; ++ui4_Idx)
        {
            if (pt_sub_obj->e_type == g_at_asf_obj_oper[ui4_Idx].e_obj_type)
            {
                break;
            }
        }
        
        /*whether is supported? should not enter this path*/
        if(ui4_Idx >= ui4_tab_len)
        {
            DBG_INFO(("_asf_free_composite_object:exception ui4_Idx=%d\r\n", 
                      ui4_Idx));
            
            /*do not break, other sub object should be freed*/
            pt_temp_obj = pt_sub_obj;
            pt_sub_obj = pt_sub_obj->pt_next_sibling;
            /*free node directly*/
            (VOID)_mm_util_mem_free(pt_temp_obj);
            pt_temp_obj = NULL;
            DBG_INFO(("_asf_free_composite_object:free Object:%s\r\n", 
                      g_at_asf_obj_oper[ASF_OBJ_UNKNOWN].psz_description));
            continue;
        }

        pt_temp_obj = pt_sub_obj->pt_next_sibling;

        /*call its free function*/
        if ( g_at_asf_obj_oper[ui4_Idx].pf_free_obj != NULL )
        {
            i4_ret = g_at_asf_obj_oper[ui4_Idx].pf_free_obj(pt_minfo_obj, 
                                                            &pt_sub_obj);
            if (MINFOR_OK != i4_ret)
            {
                DBG_ERROR(("_asf_free_composite_object: %s's free function "
                           "failed %d\r\n",
                           g_at_asf_obj_oper[ui4_Idx].psz_description,
                           i4_ret));
            }

        } 
        
        /*free node directly*/
        if (NULL != pt_sub_obj)
        {
            (VOID)_mm_util_mem_free(pt_sub_obj);
            pt_sub_obj = NULL;
            DBG_INFO(("_asf_free_composite_object:free Object:%s\r\n", 
                      g_at_asf_obj_oper[ui4_Idx].psz_description));
        }
        
        /*next sub object*/
        pt_sub_obj = pt_temp_obj;
      
    }
    
    /*free parent object*/
    _mm_util_mem_free((VOID *)(*ppt_this_obj));
    *ppt_this_obj = NULL;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_header_object
 *
 * Description: This function load header object,parse its attributes
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the header object
 *          
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 * NOTE:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-17 : initial
       
----------------------------------------------------------------------------*/
INT32  _asf_load_header_object( MINFO_OBJ_T*      pt_minfo_obj, 
                                ASF_OBJ_COMMON_T* pt_this_obj )
{
    setpos_fct                         pf_set_pos;    
    copybytes_fct                      pf_copybyte;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*         pt_asf_instance_obj;  
    /*Header Object*/
    ASF_HEADER_OBJECT_T*               pt_header_object;
    /*read buffer pointer*/
    UINT8*                             pui1_head;  
    ASF_OBJ_COMMON_T*                  pt_sub_obj;
    /*the last child of header object*/    
    ASF_OBJ_COMMON_T*                  pt_last_sub_obj ; 
    ASF_OBJ_COMMON_T*                  pt_asf_header_obj;
    INT32                              i4_ret;
    UINT32                             ui4_read_size;
    UINT8                              ui1_reserved1;
    UINT8                              ui1_reserved2;
    UINT32                             ui4_Idx;
    #ifdef CLI_LVL_ALL
    CHAR                               ac_temp[40];
    UINT32                             ui4_len;
    #endif
    UINT32                             ui4_header_fixed_len;
    
    DBG_INFO(("_asf_load_header_object:begin to parse asf header\r\n"));

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_header_object:NULL poniter\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    if ((pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        DBG_ERROR(("_asf_load_header_object:NULL copybytes or set_pos\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pt_asf_header_obj = pt_this_obj;

    /*Get func*/
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    
    /************************************************************************/
    /* do                                                     */
    /************************************************************************/
    /*init variables*/
    ui4_read_size = 0;
    ui1_reserved1 = 0;
    ui1_reserved2 = 0;
    #ifdef CLI_LVL_ALL
    x_memset(ac_temp, 0, 40);                                       
    ui4_len = 40;
     #endif   
    /*get asf instance handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_header_object:NULL pv_handler_obj in MINFO_OBJ_T\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_header_object:NULL pui1_header_buf in "
                   "MINFO_ASF_INSTANCE_INFO_T\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_header_object = (ASF_HEADER_OBJECT_T *)
                       &(pt_asf_instance_obj->t_header_object);
    pt_asf_header_obj->pv_obj_specific = pt_header_object;
    
    
    /*node linking relation*/
    pt_asf_header_obj->pt_parent        = NULL;
    pt_asf_header_obj->pt_first_child   = NULL;
    pt_asf_header_obj->pt_next_sibling  = NULL;

    /*load object header from file*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)ASF_OBJ_COMMON_LEN, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_header_object:error object header "
                   "ui4_read_size=%d!\r\n", 
                ui4_read_size));
        return MINFOR_INTERNAL_ERR;
    }
        
 
    /*get object guid and size*/
    _asf_get_guid(&(pt_asf_header_obj->t_guid), pui1_head);
    /*GUID is 16 bytes long*/  
    ASF_GET_QWORD(pt_asf_header_obj->ui8_size, pui1_head + 16);  

    if (ASFR_GUID_EQUAL != _asf_cmp_guid(&(pt_asf_header_obj->t_guid), 
                                         &gt_asf_object_header_guid))
    {
        #ifdef CLI_LVL_ALL
        _asf_guid_to_str(&(pt_asf_header_obj->t_guid), ac_temp, &ui4_len);
        DBG_ERROR(("_asf_load_header_object:guid is:%s\r\n", ac_temp));
        #endif
        return MINFOR_INTERNAL_ERR;
    }

    /*set object enumerator*/
    pt_asf_header_obj->e_type = ASF_OBJ_HEADER;    
    
    /*set to the beginnig of the object*/
    (VOID)pf_set_pos((VOID*) pt_minfo_obj,  
        (INT64)((MINFO_ASF_INSTANCE_INFO_T*)
        (pt_minfo_obj->pv_handler_obj))->ui8_cur_parser_offset,
        FM_SEEK_BGN);

    
    
    /*Header Object is at least 30 bytes*/
    if( pt_asf_header_obj->ui8_size < 30 )
    {
        DBG_ERROR(("_asf_load_header_object: less than smallest object header"
                   " ,len=%d\r\n",
                pt_asf_header_obj->ui8_size));
        return MINFOR_INTERNAL_ERR;
    }

    /*get object fields data other than guid and size,including:
     Object ID(16)+Object Size(8)+sub_object_num(4)+reserved1(1)+reserved2(1)
    */
    ui4_header_fixed_len = 30;
    if (pt_asf_instance_obj->ui4_buf_length < ui4_header_fixed_len)
    {
        _mm_util_mem_free(pt_asf_instance_obj->pui1_header_buf);
        pt_asf_instance_obj->pui1_header_buf = NULL;
        pt_asf_instance_obj->pui1_header_buf = (UINT8*)
                                              _mm_util_mem_alloc(ui4_header_fixed_len);
        if (NULL == pt_asf_instance_obj->pui1_header_buf)
        {
            DBG_ERROR(("_asf_load_header_object: failed to allocate memory for "
                       "pt_asf_instance_obj->pui1_header_buf\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        pui1_head = pt_asf_instance_obj->pui1_header_buf;
        x_memset(pui1_head, 0, ui4_header_fixed_len);
        pt_asf_instance_obj->ui4_buf_length = ui4_header_fixed_len;
        
    }
    
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                        (SIZE_T)ui4_header_fixed_len, 
                        &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_header_object: pf_copybyte return %d\r\n", i4_ret));
        return MINFOR_INTERNAL_ERR;
    }

    /*sub object number of header objects*/
    ASF_GET_DWORD(pt_asf_instance_obj->t_header_object.ui4_sub_object_count, 
                  pui1_head + ASF_OBJ_COMMON_LEN);
    
    ui1_reserved1 = pui1_head[28];  /*is reserved1*/
    ui1_reserved2 = pui1_head[29]; /*is reserved2*/
    
    /*should be 0x01 ,if not, can only ignore it*/
    if (0x01 != ui1_reserved1)
    {
        DBG_INFO(("_asf_load_header_object:ignore reserved1=%d\r\n", 
                  ui1_reserved1)); 
    }
    /*should be 0x02 ,if not, fail to source the content*/
    if (0x02 != ui1_reserved2)
    {  
        DBG_ERROR(("_asf_load_header_object:should be 0x02, error reserved2=%d\r\n", 
                   ui1_reserved2));
        return MINFOR_INTERNAL_ERR;
    }

    /*change current parsing position*/
    pt_asf_header_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
    /*we got 30 byte info ,so offset 30*/
    ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 30, i4_ret);
    if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
    {   
        DBG_ERROR(("_asf_load_header_object:self\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*display header object*/
    #ifdef CLI_LVL_ALL
    /*dispay the object */
    _asf_dump_object(pt_asf_header_obj);
    #endif
    
    /* continue to load sub objects */
    if (0 == pt_asf_instance_obj->t_header_object.ui4_sub_object_count)
    {   
        /*no sub objects return*/
        DBG_INFO(("_asf_load_header_object:end parsing asf header no "
                  "sub objects\r\n"));
        return MINFOR_OK;
    }

    pt_last_sub_obj = pt_asf_header_obj->pt_first_child;
    for(ui4_Idx = 0; 
        ui4_Idx< pt_asf_instance_obj->t_header_object.ui4_sub_object_count; 
        ++ui4_Idx)
    {
    
        pt_sub_obj = (ASF_OBJ_COMMON_T*)_mm_util_mem_alloc(sizeof(ASF_OBJ_COMMON_T));
        if (NULL == pt_sub_obj)
        {
            DBG_ERROR(("_asf_load_header_object:pt_sub_obj fail to "
                       "allocate memory\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        x_memset(pt_sub_obj, 0, sizeof(ASF_OBJ_COMMON_T));

        /**/
        if (NULL == pt_asf_header_obj->pt_first_child)
        {
            pt_asf_header_obj->pt_first_child = pt_sub_obj;
        }
        else
        {
            pt_last_sub_obj->pt_next_sibling = pt_sub_obj;
        }
        pt_last_sub_obj = pt_sub_obj;
        
        i4_ret = _asf_load_object(pt_minfo_obj, pt_sub_obj, pt_asf_header_obj);
        if (MINFOR_OK != i4_ret)
        {
            if (MINFOR_FMT_NOT_SUPPORT == i4_ret)
            {
                /*skip it ,continue*/
                pt_sub_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
                ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 
                                      pt_sub_obj->ui8_size, 
                                      i4_ret);
                if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
                {   
                    DBG_ERROR(("_asf_load_header_object:sub\r\n"));
                    return MINFOR_INTERNAL_ERR;
                }
                
                /*print object that is not supported*/
                DBG_INFO(("the followed object is not supported currently\r\n"));

            }
            else
            {
                DBG_ERROR(("_asf_load_header_object: _asf_load_object return %d\r\n", 
                           i4_ret));
                return MINFOR_INTERNAL_ERR;
            }
        
        }

        #ifdef CLI_LVL_ALL
        /*dispay the object */
        if (ASF_OBJ_HEADER_EXTENSION != pt_sub_obj->e_type)
        {
            _asf_dump_object(pt_sub_obj);
            
        }
        #endif
        
        /*if sub object is invald e.g. error object size*/
        if ((pt_sub_obj->ui8_offset + pt_sub_obj->ui8_size) 
            > (pt_asf_header_obj->ui8_offset + pt_asf_header_obj->ui8_size))
        {
            DBG_ERROR(("_asf_load_header_object:error size---"
                       "sub_obj offsize is   %d"
                       "ui8_size is %d; "
                       "header_obj offset is %d "
                       "ui8_size is %d\r\n",
                       pt_sub_obj->ui8_offset,
                       pt_sub_obj->ui8_size,
                       pt_asf_header_obj->ui8_offset,
                       pt_asf_header_obj->ui8_size));
            return MINFOR_INTERNAL_ERR;
        }

    }

    /*set stream properties*/
    _asf_set_audio_strm_lang(pt_minfo_obj, 
                             &(pt_asf_instance_obj->t_all_strm_info));
    
    DBG_INFO(("_asf_load_header_object:end parsing asf header\r\n"));
    return MINFOR_OK;
    
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_free_header_object
 *
 * Description: This function free header object resources
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the header object
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial
           
 ----------------------------------------------------------------------------*/

INT32 _asf_free_header_object( MINFO_OBJ_T*       pt_minfo_obj, 
                               ASF_OBJ_COMMON_T** ppt_this_obj )
{
    INT32   i4_ret;
    
    
    /*parameters check ,ignore...*/
    if ((NULL == pt_minfo_obj) 
        || (NULL == ppt_this_obj)
        || (NULL == *ppt_this_obj))
    {
        DBG_ERROR(("_asf_free_header_object:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /*do header free oper*/
    DBG_INFO(("_asf_free_header_object:begin to free "
              "Header Object\r\n"));
    /*call free function*/
    i4_ret = _asf_free_composite_object(pt_minfo_obj, ppt_this_obj);

    if (MINFOR_OK == i4_ret)
    {
        DBG_INFO(("_asf_free_header_object:end freeing "
                  "Header Object\r\n"));
        return MINFOR_OK;

    }
    else
    {
        DBG_INFO(("_asf_free_header_object:failed to free "
                  "Header Object\r\n"));
        return i4_ret;
    }
}
#if 0
/*-----------------------------------------------------------------------------
 * Name:  _asf_load_packet_object
 *
 * Description: This function load data packet
 *
 * Inputs:  pv_obj:        specifies the module object
 *          copybytes_fct:  
 *          setpos_fct:              
 *          getpos_fct:
 *          ui8_offset:   specify the packet position
 *          pt_packet:    specify packet length
 *
 * Outputs: pt_packet 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * NOTE:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-5-17 : initial
        
----------------------------------------------------------------------------*/
INT32  _asf_load_packet_object( VOID*                pv_obj, 
                                asf_copybytes_fct    pf_copy,
                                asf_setpos_fct       pf_setpos,
                                asf_getpos_fct       pf_getpos,
                                UINT64               ui8_offset,
                                ASF_PACKET_OBJECT_T* pt_packet)
{
    UINT64                      ui8_old_pos;
    /*temporary buffer*/
    UINT8*                      pui1_buffer;
    UINT32                      ui4_buf_len;
    INT32                       i4_ret;
    UINT8*                      pui1_head;
    
    UINT8                       ui1_len_type_flg;
    UINT8                       ui1_property_flg;  
    UINT8                       ui1_flag; 
    
    UINT8                       ui1_stream_id;
    UINT32                      ui4_med_obj_num;
    UINT32                      ui4_med_offset;
    UINT32                      ui4_replicated_len;
    UINT32                      ui4_payload_length;
    UINT32                      ui4_med_obj_size;

    ASF_PAYLOAD_PARSE_INFO_T*   pt_payload_parse_info;
    ASF_PAYLOAD_SINGLE_T*       pt_single_data;
    ASF_PAYLOAD_COM_SINGLE_T*   pt_com_single_data;
    ASF_PAYLOAD_MULTI_T*        pt_multi_data; 
    ASF_PAYLOAD_COM_MULTI_T*    pt_com_multi_data;
    UINT8                       ui1_payload_flags;
    UINT8                       ui1_payload_num;
    UINT8                       ui1_i;
    UINT32                      ui4_read_size;
 
    
    /*Parameters checking*/
    if (NULL == pv_obj)
    {
        DBG_ERROR(("_asf_load_packet_object:NULL pv_obj\r\n"));
        return MINFOR_INV_ARG;
    }

    if ((NULL == pf_copy) 
        || (NULL == pf_setpos) 
        || (NULL == pf_getpos))
    {
        DBG_ERROR(("_asf_load_packet_object:NULL callback\r\n"));
        return MINFOR_INV_ARG;
    }

    /*set position at the ui8_offset*/
    pf_getpos(pv_obj, &ui8_old_pos);
    i4_ret = pf_setpos(pv_obj, (INT64)ui8_offset, FM_SEEK_BGN);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_load_packet_object:set position error\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    /*allocate 512bytes initial buffer ,it is enough to parse packet*/
    ui4_buf_len = ASF_PACKET_INIT_LEN;
    if ((ui4_buf_len < pt_packet->t_payload_parse_info.ui4_packet_len)
        && (ASF_PACKET_LEN_MAX >= pt_packet->t_payload_parse_info.ui4_packet_len))
    {
        ui4_buf_len = pt_packet->t_payload_parse_info.ui4_packet_len;
    }
    
    pui1_buffer = (UINT8*)_mm_util_mem_alloc(ui4_buf_len);
    if (NULL == pui1_buffer)
    {
        DBG_ERROR(("_asf_load_packet_object:fail to allocate memory\r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }

    /*read...*/
    i4_ret = pf_copy(pv_obj, pui1_buffer, 
                      ui4_buf_len, 
                      ui4_buf_len, 
                      &ui4_read_size);
    if (ui4_buf_len != ui4_read_size)
    {
        DBG_ERROR(("_asf_load_packet_object:fail to read\r\n"));
        _mm_util_mem_free(pui1_buffer);
        return MINFOR_INTERNAL_ERR;
    }
    pui1_head = pui1_buffer;


    /******************************************************************
                   Begin to loading  error info ......
    *******************************************************************/
    /*judge whether error info exsit by the first byte:bit7*/
    if (0x80 == ((*pui1_head) & 0x80))
    {
        /*error info + parse + payload*/
        /*bi5&bit6 should be 00, bit4=0, bit0~bit3:0010*/
        if ((0x00 != ((*pui1_head) & 0x60)) 
            || (0x00 != ((*pui1_head) & 0x10))
            || (0x02 != ((*pui1_head) & 0x0f)))
        {
            DBG_ERROR(("_asf_load_packet_object:error file\r\n"));
            _mm_util_mem_free(pui1_buffer);
            return MINFOR_INTERNAL_ERR;
        }
        pt_packet->ui1_err_correction_flg = *pui1_head;
        /*skip error data info to parse info*/
        pt_packet->pui1_err_data = NULL;
        pui1_head += 3;

        
    }
    else
    {
        /*parse + payload*/
        DBG_INFO(("_asf_load_packet_object:no error correction info\r\n"));
    }


    
    /******************************************************************
                   Begin to loading  parse info ......
    *******************************************************************/
    pt_payload_parse_info = &(pt_packet->t_payload_parse_info);
    /*get lenght type*/
    pt_payload_parse_info->ui1_len_type_flg = *pui1_head++;
    /*get property type*/
    pt_payload_parse_info->ui1_property_flg = *pui1_head++;

    ui1_len_type_flg = pt_payload_parse_info->ui1_len_type_flg;

    /*get packet length: bit5&bit6*/
    ui1_flag = (ui1_len_type_flg & 0x60) >> 5;
    pt_payload_parse_info->ui4_packet_len =
               _asf_get_value_by_len_type(&pui1_head, ui1_flag);
    
    
    /*get sequence length: bit1&bit2*/
    ui1_flag = (ui1_len_type_flg & 0x06) >> 1;
    pt_payload_parse_info->ui4_sequence_len = 
               _asf_get_value_by_len_type(&pui1_head, ui1_flag);
    

    /*get padding length: bit3&bit4*/
    ui1_flag = (ui1_len_type_flg & 0x18) >> 3;
    pt_payload_parse_info->ui4_padding_len = 
               _asf_get_value_by_len_type(&pui1_head, ui1_flag);


    /*get send time*/
    ASF_GET_DWORD(pt_payload_parse_info->ui4_send_time, 
                  pui1_head);       
    pui1_head += 4;
    
    /*get duration time*/
    ASF_GET_WORD(pt_payload_parse_info->ui2_duration, 
                 pui1_head);       
    pui1_head += 2;

    
    /******************************************************************
                   Begin to retrieve payload data
    *******************************************************************/
    /*set initial value*/
    /*first video media object offset*/
    pt_packet->t_payload_data.ui8_first_video_obj_offset = (UINT64)0-1; /* 0xffffffffffffffff;  */

    /* containng the first video media object */
    pt_packet->t_payload_data.b_video_exist = FALSE;
    
    /* the ID of stream containng the first video media object */
    pt_packet->t_payload_data.ui1_stream_id = 0xff;
    /*whether the first fragment?*/
    pt_packet->t_payload_data.ui4_media_offset = 0xffffffff;

    ui1_property_flg = pt_packet->t_payload_parse_info.ui1_property_flg;
    /*judge single or multi payload type: bit0*/
    ui1_flag = ui1_len_type_flg & 0x01;
    if (0x00 == ui1_flag) 
    {
        /*******************If single payload*************************/
        /*get stream number*/
        ASF_GET_BYTE(ui1_stream_id, pui1_head);
        ++pui1_head;
        
        /*get media object number: bit4 & bit5*/
        ui1_flag = (ui1_property_flg & 0x30) >> 4;
        ui4_med_obj_num = _asf_get_value_by_len_type(&pui1_head, ui1_flag);
        
        /*get media offset or pts:bit2 & bit3*/
        ui1_flag = (ui1_property_flg & 0x0C) >> 2;
        ui4_med_offset = _asf_get_value_by_len_type(&pui1_head, ui1_flag);
        
        /*whether is compressed: bit0 & bit1*/
        ui1_flag = ui1_property_flg & 0x03;
        ui4_replicated_len = _asf_get_value_by_len_type(&pui1_head, ui1_flag);
        
        /*compressed payload*/
        if (1 == ui4_replicated_len)
        {
            pt_packet->t_payload_data.e_payload_type = ASF_PAYLOAD_COM_SINGLE;
            pt_com_single_data = &(pt_packet->t_payload_data.u.t_com_single_data);
            pt_com_single_data->ui1_stream_num = ui1_stream_id;
            pt_com_single_data->ui4_med_obj_num = ui4_med_obj_num;
            pt_com_single_data->ui4_pts = ui4_med_offset;
            
            /*get pts delta: 1 byte*/
            ++pui1_head;

            if (0x80 == (ui1_stream_id & 0x80))
            {
                /*sub-payload#0 data length 1 byte*/
                ASF_GET_BYTE(pt_packet->t_payload_data.ui1_first_video_obj_len, 
                             pui1_head);
                ++pui1_head;
                
                /*save media object file offset*/
                pt_packet->t_payload_data.ui8_first_video_obj_offset = 
                           (UINT64)(ui8_offset + pui1_head - pui1_buffer);
                pt_packet->t_payload_data.b_video_exist = TRUE;
                pt_packet->t_payload_data.ui1_stream_id = ui1_stream_id & 0x7f;
                /*uncompressed is 0*/
                pt_packet->t_payload_data.ui4_media_offset = 0;
                
            }
            else
            {
                
            }
        }
        /*unpressed payload*/
        else
        {
            pt_packet->t_payload_data.e_payload_type = ASF_PAYLOAD_SINGLE;
            pt_single_data = &(pt_packet->t_payload_data.u.t_single_data);
            pt_single_data->ui1_stream_num = ui1_stream_id;
            pt_single_data->ui4_med_obj_num = ui4_med_obj_num;
            pt_single_data->ui4_offset = ui4_med_offset;
            pt_single_data->ui4_relicated_len = ui4_replicated_len;

            /*do not retrieve replicated data other than medai object size
            and pts*/
            ASF_GET_DWORD(pt_single_data->ui4_med_obj_size, pui1_head);
            pui1_head += 4;
            
            ASF_GET_DWORD(pt_single_data->ui4_pts, pui1_head);
            pui1_head += 4;
            
            pt_single_data->pui1_replicated_other = NULL;
            
            pui1_head += pt_single_data->ui4_relicated_len - 8;

            if (0x80 == (ui1_stream_id & 0x80))
            {
                /*save media object file offset*/
                pt_packet->t_payload_data.ui8_first_video_obj_offset = 
                           (UINT64)(ui8_offset + pui1_head - pui1_buffer);
                pt_packet->t_payload_data.ui1_first_video_obj_len =
                           (UINT8)pt_single_data->ui4_med_obj_size;
                pt_packet->t_payload_data.b_video_exist = TRUE;
                pt_packet->t_payload_data.ui1_stream_id = ui1_stream_id & 0x7f;
                pt_packet->t_payload_data.ui4_media_offset = ui4_med_offset;
            }
            else
            {
                
            }
        }
    }
    else
    {
        /*******************If multi payloads*************************/
        /*get payload flags 1 byte: bit0~bit5  payload number*/
        ASF_GET_BYTE(ui1_payload_flags, pui1_head);
        ++pui1_head;
        /*payloads count*/
        ui1_payload_num = ui1_payload_flags & 0x3f;
        for (ui1_i = 0; ui1_i < ui1_payload_num; ++ui1_i)
        {
            /************we only parser the first video payload**************/
            /*get stream number*/
            ASF_GET_BYTE(ui1_stream_id, pui1_head);
            ++pui1_head;

            /*get media object number: bit4 & bit5*/
            ui1_flag = (ui1_property_flg & 0x30) >> 4;
            ui4_med_obj_num = _asf_get_value_by_len_type(&pui1_head, ui1_flag);

            /*get media offset or pts:bit2 & bit3*/
            ui1_flag = (ui1_property_flg & 0x0C) >> 2;
            ui4_med_offset = _asf_get_value_by_len_type(&pui1_head, ui1_flag);

            /*whether is compressed: bit0 & bit1*/
            ui1_flag = ui1_property_flg & 0x03;
            ui4_replicated_len = _asf_get_value_by_len_type(&pui1_head, ui1_flag);
            
            /*compressed payload*/
            if (1 == ui4_replicated_len)
            {
                pt_packet->t_payload_data.e_payload_type = ASF_PAYLOAD_COM_MULTI;
                pt_com_multi_data = &(pt_packet->t_payload_data.u.t_com_multi_data);
                pt_com_multi_data->ui1_payload_flg = ui1_payload_flags;

                /*get pts delta: 1 byte*/
                ++pui1_head;
                
                /*get this payload length bit6&bit7*/
                ui1_flag = (ui1_payload_flags & 0xC0) >> 6;
                ui4_payload_length = 
                    _asf_get_value_by_len_type(&pui1_head, ui1_flag);
                
                /*If key stream(must be video) get the first media object offset 
                and size,otherwise skip... ...*/
                if(0x80 == (ui1_stream_id & 0x80))
                {
                    /* sub-payload#0 data length 1 byte*/
                    ASF_GET_BYTE(pt_packet->t_payload_data.ui1_first_video_obj_len,
                                 pui1_head);
                    ++pui1_head;
                    /*first video media object offset*/
                    pt_packet->t_payload_data.ui8_first_video_obj_offset = 
                               (UINT64)(ui8_offset + pui1_head  - pui1_buffer);

                    /* containng the first video media object */
                    pt_packet->t_payload_data.b_video_exist = TRUE;
                    
                    /* the ID of stream containng the first video media object */
                    pt_packet->t_payload_data.ui1_stream_id = 
                               ui1_stream_id & 0x7f;
                    /*whether the first fragment? this is ture for compressed*/
                    pt_packet->t_payload_data.ui4_media_offset = 0;
                    break;
                    
                }
                else
                {
                    /*get to next payload*/
                    pui1_head += ui4_payload_length;
                    continue;
                }
            }
            /*uncompressed*/
            else
            {
                pt_packet->t_payload_data.e_payload_type = ASF_PAYLOAD_MULTI;
                pt_multi_data = &(pt_packet->t_payload_data.u.t_multi_data);
                pt_multi_data->ui1_payload_flg = ui1_payload_flags;

                /*get media object size */
                ASF_GET_DWORD(ui4_med_obj_size, pui1_head);
                pui1_head += 4;
                
                /*get media object pts*/
                pui1_head += 4;
                
                pui1_head += ui4_replicated_len - 8;
                
                /*get first payload length bit6&bit7*/
                ui1_flag = (ui1_payload_flags & 0xC0) >> 6;
                ui4_payload_length = 
                    _asf_get_value_by_len_type(&pui1_head, ui1_flag);

                /*If key stream(must be video) get the first media object offset 
                and size,otherwise skip... ...*/
                if(0x80 == (ui1_stream_id & 0x80))
                {
                    /*first video media object offset*/
                    pt_packet->t_payload_data.ui8_first_video_obj_offset = 
                               (UINT64)(ui8_offset + pui1_head  - pui1_buffer);
                    pt_packet->t_payload_data.ui1_first_video_obj_len = 
                               (UINT8)ui4_med_obj_size;

                    /* containng the first video media object */
                    pt_packet->t_payload_data.b_video_exist = TRUE;
                    
                    /* the ID of stream containng the first video media object */
                    pt_packet->t_payload_data.ui1_stream_id = 
                               ui1_stream_id & 0x7f;
                    /*whether the first fragment?*/
                    pt_packet->t_payload_data.ui4_media_offset = ui4_med_offset;
                    break;
                    
                }
                else
                {
                    /*get to next payload*/
                    pui1_head += ui4_payload_length;
                    continue;
                }
            }        
        }
    }

    /*free memory*/
    _mm_util_mem_free(pui1_buffer);
    pui1_buffer = NULL;
    
    /*restore old position*/
    i4_ret = pf_setpos(pv_obj, (INT64)ui8_old_pos, FM_SEEK_BGN);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_load_packet_object:restore position error\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_wmv_supported_test
 *
 * Description: This function test whether suppoet wmv 
 *
 * Inputs:  pt_minfo_obj:         specifies the module object
 *          ui8_data_packet_begin:  packet beginning offset
 *          ui4_per_packet_size:    packet size         
 *
 * Outputs: pb_supported: TRUE: if supported 
 *                          FLASE:if not supported
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * NOTE:    Use temporary buffer, so be careful of its content altered
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-5-28 : initial
        
----------------------------------------------------------------------------*/

INT32 _asf_wmv_supported_test( MINFO_OBJ_T*  pt_minfo_obj,
                               UINT64        ui8_data_packet_begin,
                               UINT32        ui4_per_packet_size,
                               BOOL*         pb_supported)
{  
    INT32                            i4_ret;
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*       pt_asf_instance_obj; 
    UINT8*                           pui1_head;  
    MINFO_ASF_ALL_STRM_INFO_T*       pt_all_strm_info;   
    MINFO_ASF_PER_STRM_INFO_T*       p_strm_info_head;
    MINFO_ASF_PER_VID_STRM_INF_T*    pt_vid_strm_inf;
    
    /*stream properties*/
    ASF_STREAM_PROPERTIES_OBJECT_T*  pt_stream_obj;
    setpos_fct                       pf_set_pos;    
    copybytes_fct                    pf_copybyte;
    getpos_fct                       pf_get_pos;

    UINT64                           ui8_packet_idx;
    UINT64                           ui8_packet_offset;
    
    ASF_PACKET_OBJECT_T              t_packet;
   
    SeqHeaderInfo                    t_header_info;
    UINT64                           ui8_old_pos;
    UINT32                           ui4_read_size;
 
    /*parameters check, 4 parameters*/
    *pb_supported = FALSE; /*not supported*/
    
    if (NULL == pt_minfo_obj)
    {
        DBG_ERROR(("_asf_wmv_supported_test:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /**********************Do********************************/

    /*get asf instance handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_wmv_supported_test:"
                   "NULL pv_handler_obj in MINFO_OBJ_T\r\n"));
        return MINFOR_INITIAL_ERR;
    }
    
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_wmv_supported_test:NULL pui1_header_buf in "
                   "MINFO_ASF_INSTANCE_INFO_T\r\n"));
        return MINFOR_INITIAL_ERR;
    }

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    if ((NULL == pf_set_pos) 
        || (NULL == pf_copybyte)
        || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_asf_wmv_supported_test:NULL input\r\n"));
        return MINFOR_INITIAL_ERR;
    }
    
    ui8_packet_idx = 0;
    while (TRUE)
    {

        ui8_packet_offset = ui8_data_packet_begin
                            + ui8_packet_idx * ui4_per_packet_size;
        ++ui8_packet_idx;

        x_memset(&t_packet, 0 ,sizeof(t_packet));
        t_packet.t_payload_parse_info.ui4_packet_len = ui4_per_packet_size;
        i4_ret = _asf_load_packet_object( pt_minfo_obj, 
                                          pf_copybyte,
                                          pf_set_pos,
                                          pf_get_pos,
                                          ui8_packet_offset,
                                          &t_packet);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("_asf_wmv_supported_test:load packet object error %d\r\n",
                       i4_ret));
            return MINFOR_INITIAL_ERR;
        }

        /*If video media object*/
        if (( ASF_STRM_VIDEO_TYPE == _asf_get_stream_type(pt_minfo_obj,
                                 t_packet.t_payload_data.ui1_stream_id))
           && (TRUE == t_packet.t_payload_data.b_video_exist)
           && (0 == t_packet.t_payload_data.ui4_media_offset))
        {
            break;
        }
    }

    /*should find the first video media object*/
    t_header_info.u4Len = t_packet.t_payload_data.ui1_first_video_obj_len;

    if (t_header_info.u4Len
        > pt_asf_instance_obj->ui4_buf_length)
    {
        if (NULL != pt_asf_instance_obj->pui1_header_buf)
        {
            _mm_util_mem_free(pt_asf_instance_obj->pui1_header_buf);
            pt_asf_instance_obj->pui1_header_buf = NULL;
        }
        pt_asf_instance_obj->pui1_header_buf = (UINT8*)
                            _mm_util_mem_alloc(t_header_info.u4Len);
        if (NULL == pt_asf_instance_obj->pui1_header_buf)
        {
            DBG_ERROR(("_asf_wmv_supported_test:allocate buffer error\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }

        pui1_head = pt_asf_instance_obj->pui1_header_buf;
        x_memset(pui1_head, 0, t_header_info.u4Len);
        pt_asf_instance_obj->ui4_buf_length = t_header_info.u4Len;
        
    }
    /*save old position*/
    pf_get_pos(pt_minfo_obj, &ui8_old_pos);
    pf_set_pos(pt_minfo_obj, 
               (INT64)t_packet.t_payload_data.ui8_first_video_obj_offset,
               FM_SEEK_BGN);
    i4_ret = pf_copybyte(pt_minfo_obj, 
                         pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)t_header_info.u4Len, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_wmv_supported_test: copy return %d\r\n", 
                   i4_ret));
        return MINFOR_INITIAL_ERR;
    }
    t_header_info.u1Addr = pui1_head;


    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    p_strm_info_head = pt_all_strm_info->p_strm_info_head;
    while (NULL != p_strm_info_head)
    {
        pt_stream_obj = p_strm_info_head->pt_stream_obj;
        if (NULL == pt_stream_obj)
        {
            DBG_ERROR(("_asf_wmv_supported_test:pt_stream_obj Exception!!!\r\n"));
            return MINFOR_INITIAL_ERR;
        }
        if (t_packet.t_payload_data.ui1_stream_id != pt_stream_obj->ui1_stream_number)
        {
            p_strm_info_head = p_strm_info_head->pt_next_strm;
            continue;
        }
        
        if (ASFR_GUID_EQUAL 
                    != _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                     &gt_asf_object_stream_type_video))
        {
            DBG_ERROR(("_asf_wmv_supported_test:type Exception!!!\r\n"));
            return MINFOR_INITIAL_ERR;
        }
        break;
        
    }
    
    if (NULL == p_strm_info_head)
    {
        return MINFOR_INITIAL_ERR;
    }

    pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)
                       p_strm_info_head->pv_type_info;
    if (NULL == pt_vid_strm_inf)
    {
        DBG_ERROR(("_asf_wmv_supported_test:pt_vid_strm_inf Exception!!!\r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /*show information*/
    if(FALSE == fgDecoderSupport(pt_vid_strm_inf->t_vid_strm_atrbt.e_enc,
                                 &t_header_info))
    {
        
        DBG_INFO(("_asf_wmv_supported_test:codec support wmv8 intra\r\n"));
        *pb_supported = TRUE;
    }
    else
    {
        DBG_INFO(("_asf_wmv_supported_test:codec doesn't support wmv8 intra\r\n"));
        
    }
    
    /*restore prior position*/
    pf_set_pos(pt_minfo_obj, 
               (INT64)ui8_old_pos,
               FM_SEEK_BGN);
    
    return MINFOR_OK;
    
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  _asf_load_data_object
 *
 * Description: This function load data object,parse its attributes
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *          pt_this_obj:  point to a structure which stores common info 
 *                        of the data object
 *          
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * NOTE:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-17 : initial
        
----------------------------------------------------------------------------*/
INT32  _asf_load_data_object( MINFO_OBJ_T*      pt_minfo_obj, 
                              ASF_OBJ_COMMON_T* pt_this_obj )
{
    setpos_fct                 pf_set_pos;    
    copybytes_fct              pf_copybyte;

    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T* pt_asf_instance_obj ; 
    ASF_DATA_OBJECT_T*         pt_data_object;
    /*read buffer pointer*/
    UINT8*                     pui1_head;  
    ASF_OBJ_COMMON_T*          pt_asf_data_obj;
    INT32                      i4_ret;
    UINT32                     ui4_read_size;
    UINT16                     ui2_reserved;
    #ifdef CLI_LVL_ALL
    CHAR                       ac_temp[40];
    UINT32                     ui4_len;
    #endif
    UINT32                     ui4_per_packe_size;/*per packe size*/
    UINT32                     ui4_data_head_len;


    DBG_INFO(("_asf_load_data_object:begin to load data object\r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_minfo_obj) || (NULL == pt_this_obj))
    {
        DBG_ERROR(("_asf_load_data_object:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    if ((NULL == pt_minfo_obj->t_input_fct_tbl.pf_copybytes)
        || (NULL == pt_minfo_obj->t_input_fct_tbl.pf_set_pos))
    {
        DBG_ERROR(("_asf_load_data_object:pf_copybytes or pf_set_pos is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    /************************************************************************/
    /* do                                                     */
    /************************************************************************/
    /*init variables*/
    ui2_reserved = 0;
    #ifdef CLI_LVL_ALL
    x_memset(ac_temp, 0 ,40);
    ui4_len = 40;
    #endif
    ui4_per_packe_size = 0;  /*per packe size*/
    
    /*get asf instance handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_load_data_object:NULL pv_handler_obj in MINFO_OBJ_T\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_load_data_object:NULL pui1_header_buf in "
                   "MINFO_ASF_INSTANCE_INFO_T\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_data_object = &(pt_asf_instance_obj->t_data_object);
    pt_this_obj->pv_obj_specific = pt_data_object;
    
    pt_asf_data_obj = pt_this_obj;
    
    /*node linking relation*/
    pt_asf_data_obj->pt_parent        = NULL;
    pt_asf_data_obj->pt_first_child   = NULL;
    pt_asf_data_obj->pt_next_sibling  = NULL;

    /*load object header from file*/
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)ASF_OBJ_COMMON_LEN, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_data_object:error header of data object!\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
        
 
    /*get object guid and size*/
    _asf_get_guid(&(pt_asf_data_obj->t_guid), pui1_head);
    /*GUID is 16 bytes long*/ 
    ASF_GET_QWORD(pt_asf_data_obj->ui8_size, pui1_head + 16);   

    if (ASFR_GUID_EQUAL != _asf_cmp_guid(&(pt_asf_data_obj->t_guid), 
                                         &gt_asf_object_data_guid))
    {
        #ifdef CLI_LVL_ALL
        _asf_guid_to_str(&(pt_asf_data_obj->t_guid), ac_temp, &ui4_len);
        DBG_ERROR(("_asf_load_data_object:guid is:%s\r\n", ac_temp));        
        #endif
        return MINFOR_INTERNAL_ERR;
    }

    /*set object enumerator*/
    pt_asf_data_obj->e_type = ASF_OBJ_DATA;    
    
    /*set to the beginnig of the object*/
    pf_set_pos((VOID*) pt_minfo_obj,  
               (INT64)((MINFO_ASF_INSTANCE_INFO_T*)
               (pt_minfo_obj->pv_handler_obj))->ui8_cur_parser_offset,
                FM_SEEK_BGN);
    
    /*Header Object is at least 50 bytes*/
    if( pt_asf_data_obj->ui8_size < 50 )
    {
        DBG_ERROR(("_asf_load_data_object: less than smallest object header, "
                   "len=%llu\r\n",
                pt_asf_data_obj->ui8_size));
        return MINFOR_INTERNAL_ERR;
    }
    
    /*26 :File ID (16bytes) + total Packets(8bytes) + Reserved(2 bytes)*/
    ui4_data_head_len = 26 + ASF_OBJ_COMMON_LEN;

    /*get object fields data other than guid and size, including:
     File ID (16bytes) + total Packets(8bytes) + Reserved(2 bytes)*/
    if (26 > pt_asf_instance_obj->ui4_buf_length)
    {
        if (NULL != pt_asf_instance_obj->pui1_header_buf)
        {
            _mm_util_mem_free(pt_asf_instance_obj->pui1_header_buf);
            pt_asf_instance_obj->pui1_header_buf = NULL;
        }
        pt_asf_instance_obj->pui1_header_buf = (UINT8*)
                                                _mm_util_mem_alloc(ui4_data_head_len);
        if (NULL == pt_asf_instance_obj->pui1_header_buf)
        {
            DBG_ERROR(("_asf_load_data_object:allocate error\r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }

        pui1_head = pt_asf_instance_obj->pui1_header_buf;
        x_memset(pui1_head, 0, ui4_data_head_len);
        pt_asf_instance_obj->ui4_buf_length = ui4_data_head_len;
        
    }
    
    i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)ui4_data_head_len, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_load_data_object: pf_copybyte return %d\r\n", 
                   i4_ret));
        return MINFOR_INTERNAL_ERR;
    }


    pui1_head += ASF_OBJ_COMMON_LEN;

    /*get File guid*/
    _asf_get_guid(&(pt_data_object->t_file_id), pui1_head);
    pui1_head += 16;   /*GUID is 16 bytes long*/  
    
    /*Get total packet counts, 8 bytes*/
    ASF_GET_QWORD((pt_data_object->ui8_total_packets), pui1_head);
    pui1_head += 8; 

    /*Get reserved, 2 bytes*/
    ASF_GET_WORD(ui2_reserved, pui1_head);
    pui1_head += 2; 
   
    /*should be 0x0101 ,if not, can only ignore it*/
    if (0x0101 != ui2_reserved)
    {
        DBG_INFO(("_asf_load_data_object:ignore reserved2=%u\r\n",
                  ui2_reserved)); 
    }
    pt_data_object->ui2_reserved = ui2_reserved;

    pt_asf_data_obj->ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
    
    pt_data_object->ui8_data_packet_begin = pt_asf_data_obj->ui8_offset 
                                    + ASF_OBJ_COMMON_LEN + 16 + 8 + 2;



    /******************provided for index bulider ************************/
    pt_asf_instance_obj->t_idx_info.ui8_data_offset = 
                                                  pt_asf_data_obj->ui8_offset;
    pt_asf_instance_obj->t_idx_info.ui8_data_sz = pt_asf_data_obj->ui8_size;

    /*calculate packet size ,the fixed fields of Data Object are 50 bytes long*/
    if (0 != pt_data_object->ui8_total_packets)
    {
        ui4_per_packe_size = (UINT32)_mm_div64((pt_asf_data_obj->ui8_size - 50),
                               (pt_data_object->ui8_total_packets), NULL);
    }
    else
    {
        DBG_INFO(("_asf_load_data_object:warning:ui8_total_packets is zero\r\n"));
        ui4_per_packe_size = 0;
    }

    /*set stream properties*/
    _asf_set_strm_packet_size( &(pt_asf_instance_obj->t_all_strm_info),
                               ASF_STREAM_NUM_MAX,
                               ui4_per_packe_size );

    /*change current parsing position
           we got 30 byte info ,so offset 30
        */
    ASF_SET_CUR_PARSE_POS_CHK_BSY(pt_minfo_obj, 
                          pt_asf_data_obj->ui8_size, 
                          i4_ret);
    if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
    {   
        DBG_ERROR(("_asf_load_data_object:set pos error\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    else if (ASF_MINFO_SET_FILE_POS_BSY == i4_ret)
    {
        pt_asf_instance_obj->b_non_seekable = TRUE;
        return MINFOR_OK;
    }

    if ((50 == pt_asf_data_obj->ui8_size)
        && ((pt_asf_instance_obj->t_all_strm_info.ui1_audio_strm_num != 0)
            || (pt_asf_instance_obj->t_all_strm_info.ui1_video_strm_num != 0)
            || (pt_asf_instance_obj->t_all_strm_info.ui1_hidden_video_strm_num != 0)
            || (pt_asf_instance_obj->t_all_strm_info.ui1_hidden_audio_strm_num != 0)))
    {
        /*Retrieved by FM API, not pt_asf_instance_obj->t_file_object->ui8_file_size*/
        pt_asf_instance_obj->t_idx_info.ui8_data_sz = 
                             pt_minfo_obj->z_buff_leng  
                              - pt_asf_data_obj->ui8_offset;
        /*set file position to the end*/
        ASF_SET_CUR_PARSE_POS(pt_minfo_obj, 
                              pt_asf_instance_obj->t_idx_info.ui8_data_sz 
                                - pt_asf_data_obj->ui8_size, 
                              i4_ret);
        if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
        {   
            DBG_ERROR(("_asf_load_data_object:set pos error for exception handling\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
    }


    /****************** provided for wmv8 x8intra detecting ******************/
    /*Get the first video media object offset and its length so that  Service
    Context module can tell whether play a wmv medai file*/

    
    /*display data object*/
    #ifdef CLI_LVL_ALL
    /*dispay the object */
    _asf_dump_object(pt_this_obj);
    #endif

    DBG_INFO(("_asf_load_data_object:end loading data object\r\n"));
    return MINFOR_OK;
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_data_object
 *
 * Description: This function free data object resources
 *
 * Inputs:  pt_minfo_obj: specifies Minfo object
 *          pt_this_obj:  two level pointer, finally point to a structure which 
 *                        stores common info of the data object
 * Outputs: NULL 
 * 
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial
           
 ----------------------------------------------------------------------------*/

INT32  _asf_free_data_object( MINFO_OBJ_T*       pt_minfo_obj, 
                              ASF_OBJ_COMMON_T** ppt_this_obj )
{

    /*parameters check ,ignore...*/
    if ((NULL == pt_minfo_obj) 
        || (NULL == ppt_this_obj)
        || (NULL == *ppt_this_obj))
    {
        DBG_ERROR(("_asf_free_data_object:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    DBG_INFO(("_asf_free_data_object:begin to free Data Object\r\n"));

    /*free Data object*/
    if (NULL != *ppt_this_obj)
    {
        _mm_util_mem_free((VOID *)(*ppt_this_obj));
        *ppt_this_obj = NULL;
        DBG_INFO(("_asf_free_data_object:free Data Object\r\n"));
    }
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_free_dynamic_resource
 *
 * Description: This function free all dynamic resources which are in 
 *              MINFO_ASF_INSTANCE_INFO_T
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *           
 *
 * Outputs:  NULL 
 *
 * Returns:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-23 : initial
 ----------------------------------------------------------------------------*/
VOID  _asf_free_dynamic_resource( MINFO_OBJ_T* pt_minfo_obj )
{

    MINFO_ASF_INSTANCE_INFO_T*         pt_asf_instance_obj;  
    ASF_CODEC_LIST_OBJECT_T*           pt_codec_list;
    ASF_CODEC_ENTRY_T*                 pt_codec;
    ASF_IDX_PARAMETERS_OBJECT_T*       pt_idx_paras;
    ASF_SIMPLE_INDEX_OBJECT_T*         pt_simple_index_object;
    /*Language List Object*/
    ASF_LANGUAGE_LIST_OBJECT_T*        pt_language;
    ASF_CONTENT_DESCRIPTION_OBJECT_T*  pt_content_des;
    ASF_BITRATE_PROPERTIES_OBJECT_T*   pt_bitrate_properties;
    ASF_CONTENT_ENCRYPTION_OBJECT_T*   pt_content_encryption;
    MINFO_ASF_ALL_STRM_INFO_T*         pt_all_strm_info;
    ASF_EXT_CONTENT_DES_OBJECT_T*      pt_ext_content_des;
    ASF_EXT_CONTENT_DES_NAME_VALUE_T*  pt_name_value;
    INT32                              i4_ret;
    UINT32                             ui4_idx;
    
    if (NULL == pt_minfo_obj)
    {
        DBG_ERROR(("_asf_free_dynamic_resource:NULL\r\n"));
        return ;
    }
    
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*) 
                           pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_free_dynamic_resource:asf_instance_obj=NULL\r\n"));
        return ;
    }
    /*init variables*/
   
    /*have got what we want ,so can free resource*/
    /*1.free header object tree*/
    i4_ret = _asf_free_header_object(pt_minfo_obj, 
                                     &(pt_asf_instance_obj->t_root.pt_header));
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_free_dynamic_resource:error occured during "
                   "Header Object\r\n"));
    }

    
    /*2.free data object*/
     i4_ret = _asf_free_data_object(pt_minfo_obj, 
                                    &(pt_asf_instance_obj->t_root.pt_data));
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_free_dynamic_resource:error occured during free "
                   "Data Object\r\n"));
    }
    

    /*All stream info:including Stream properties and Extended Stream*/
    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    _asf_strm_info_free(pt_all_strm_info);
    
    /*Codec List Object*/
    pt_codec_list = &(pt_asf_instance_obj->t_codec_list);
    if (pt_codec_list->ui4_codec_entry_count && pt_codec_list->pt_codec)
    {
        for (ui4_idx = 0; ui4_idx < pt_codec_list->ui4_codec_entry_count; 
             ++ui4_idx)
        {
            pt_codec = pt_codec_list->pt_codec + ui4_idx;
            if (pt_codec->pwsz_name)
            {
                (VOID)_mm_util_mem_free(pt_codec->pwsz_name);
                pt_codec->pwsz_name = NULL;
            }
            
            if (pt_codec->pwsz_desc)
            {
                (VOID)_mm_util_mem_free(pt_codec->pwsz_desc);
                pt_codec->pwsz_desc = NULL;
            }

            if (pt_codec->pui1_info)
            {
                (VOID)_mm_util_mem_free(pt_codec->pui1_info);
                pt_codec->pui1_info = NULL;
            }
            
        }
        
        (VOID)_mm_util_mem_free(pt_codec_list->pt_codec);
        pt_codec_list->pt_codec = NULL;
        
    }

    /*Language List Object*/
    pt_language = &(pt_asf_instance_obj->t_language);
    if (pt_language->pt_languge)
    {
        (VOID)_mm_util_mem_free(pt_language->pt_languge);
        pt_language->pt_languge = NULL;
        
    }

    /*Content Description Object*/
    pt_content_des = &(pt_asf_instance_obj->t_content_des);
    if (NULL != pt_content_des->pwsz_title)
    {
        _mm_util_mem_free(pt_content_des->pwsz_title);
        pt_content_des->pwsz_title = NULL;
    }
    if (NULL != pt_content_des->pwsz_author)
    {
        _mm_util_mem_free(pt_content_des->pwsz_author);
        pt_content_des->pwsz_author = NULL;
    }
    if (NULL != pt_content_des->pwsz_copyright)
    {
        _mm_util_mem_free(pt_content_des->pwsz_copyright);
        pt_content_des->pwsz_copyright = NULL;
    }
    if (NULL != pt_content_des->pwsz_description)
    {
        _mm_util_mem_free(pt_content_des->pwsz_description);
        pt_content_des->pwsz_description = NULL;
    }
    if (NULL != pt_content_des->pwsz_rating)
    {
        _mm_util_mem_free(pt_content_des->pwsz_rating);
        pt_content_des->pwsz_rating = NULL;
    }

    /*Extended Content Description Object*/
    pt_ext_content_des = &(pt_asf_instance_obj->t_ext_content_des);
    for ( ui4_idx = 0; ui4_idx < EXT_CONT_NAME_MAX; ++ui4_idx )
    {
        pt_name_value = &( pt_ext_content_des->at_name_value[ui4_idx] );
        switch ( pt_name_value->at_data_type )
        {
            case DATA_TYPE_WC:
            case DATA_TYPE_BYTE_ARR:
                if ( pt_name_value->u_data.pv_value )
                {
                    _mm_util_mem_free( pt_name_value->u_data.pv_value );
                    pt_name_value->u_data.pv_value = NULL;
                }
                break;
            case DATA_TYPE_BOOL:
                break;
            case DATA_TYPEA_DWORD:
                break;
            case DATA_TYPEA_QWORD:
                break;
            case DATA_TYPE_WORD:
                break;
            default:  
                DBG_ERROR( ( "%s#Line%d:unknown data type %u ",
                             __FUNCTION__, __LINE__,
                             pt_ext_content_des->at_name_value[ui4_idx].at_data_type ) );
        }
    
    }

    /*Bitrate Properties Object*/
    pt_bitrate_properties = &(pt_asf_instance_obj->t_bitrate_properties);
    if (NULL != pt_bitrate_properties->pt_record)
    {
        _mm_util_mem_free(pt_bitrate_properties->pt_record);
        pt_bitrate_properties->pt_record = NULL;
    }

    /*Content Encryption Object*/
    pt_content_encryption = &(pt_asf_instance_obj->t_content_encryption);
    if (NULL != pt_content_encryption->pui1_secret_data)
    {
        _mm_util_mem_free(pt_content_encryption->pui1_secret_data);
        pt_content_encryption->pui1_secret_data = NULL;
    }
    if (NULL != pt_content_encryption->pi1_protect_type)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_protect_type);
        pt_content_encryption->pi1_protect_type = NULL;
    }
    if (NULL != pt_content_encryption->pi1_key_id)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_key_id);
        pt_content_encryption->pi1_key_id = NULL;
    }
    if (NULL != pt_content_encryption->pi1_lic_url)
    {
        _mm_util_mem_free(pt_content_encryption->pi1_lic_url);
        pt_content_encryption->pi1_lic_url = NULL;
    }
    
    /*Index Parameters Object*/
    pt_idx_paras = &(pt_asf_instance_obj->t_idx_paras);
    if (pt_idx_paras->ui2_specifers_count && pt_idx_paras->pt_specifers)
    {
        (VOID)_mm_util_mem_free(pt_idx_paras->pt_specifers);
        pt_idx_paras->pt_specifers = NULL;
    }

    /*simple Index Object*/
    pt_simple_index_object = &(pt_asf_instance_obj->t_simple_index_object);
    if (pt_simple_index_object->ui4_index_entry_count 
        && pt_simple_index_object->pt_index_entry )
    {
        (VOID)_mm_util_mem_free(pt_simple_index_object->pt_index_entry);
        pt_simple_index_object->pt_index_entry = NULL;
    }

    return ;
}

#ifdef CLI_LVL_ALL
extern VOID _asf_cli_dump_object( MINFO_OBJ_T*   pt_minfo_obj, 
                                         ASF_OBJ_TYPE_E e_obj_type );
#endif

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
/*-----------------------------------------------------------------------------
 * Name:  _asf_get_bad_interleaved_threshold
 * Description: 
 * Inputs:  ui4_packet_len  
 * Returns: 
 * History: 
 ----------------------------------------------------------------------------*/
UINT32 _asf_bad_interleaved_threshold(UINT32 ui4_packet_len)
{
    UINT8   ui1_table_len =0;
    UINT8   ui1_idx;

    ui1_table_len = sizeof(s_bad_threshold_tbl)/sizeof(MINFO_ASF_BAD_THRESHOLD_TBL);

    for (ui1_idx=0; ui1_idx<ui1_table_len; ui1_idx++)
    {
        if (s_bad_threshold_tbl[ui1_idx].ui1_packet_len > ui4_packet_len)
        {
            return s_bad_threshold_tbl[ui1_idx].ui1_bad_threshold;
        }
    }
    
    return MINFOR_ASF_BAD_INTERLEAVED_MAX_VID_DIFFERENCE;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_analyse_paket_info
 *
 * Description: The function is charge to find the most difference among the streams 
 *                  throght the input parameter present time.
 *
 * Inputs:  pt_minfo_obj   : specifies the Minfo object
 *
 * Returns: MINFOR_OK :                if successful
 *             others non zero values:   if unsuccessful
 *          
 ----------------------------------------------------------------------------*/
static INT32 _asf_analyse_paket_info(MINFO_OBJ_T*    pt_minfo_obj)
{
    MINFO_ASF_ALL_STRM_INFO_T*  pt_all_strm_info;
    MINFO_ASF_PER_STRM_INFO_T*  pt_per_strm_info;
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj; 
    ASF_BD_PKT_T*               pt_bd_pkt;
    UINT32                      ui4_defferent_time      = 0;
    UINT32                      ui4_max                 = 0;
    BOOL                        b_audio                 = FALSE;
    UINT32                      ui4_vid_threshold       = 0;

    /* check the parameter*/
    if (NULL == pt_minfo_obj)  
    {
        DBG_ERROR(("_asf_analyse_paket_info:parameter is NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)
                          (pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("_asf_analyse_paket_info:pt_asf_instance_obj "
                   "is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    pt_bd_pkt = &pt_asf_instance_obj->t_bd_pkt;
    if ((ASF_STREAM_NUM_MAX < pt_bd_pkt->ui1_stream_id) || (0 == pt_bd_pkt->ui1_stream_id))
    {
        DBG_ERROR(("_asf_analyse_paket_info:parameter id is %us\r\n",
                   pt_bd_pkt->ui1_stream_id));
        return MINFOR_INV_ARG;
    }
    /* initial */
    pt_all_strm_info = &(pt_asf_instance_obj->t_all_strm_info);
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;

    /*Get the threshold for video first.just for first come out stream is video*/
    ui4_vid_threshold = _asf_bad_interleaved_threshold(pt_bd_pkt->ui4_packet_size);
    
    while (NULL != pt_per_strm_info)
    {   
        /* fill the present time */
        if (pt_bd_pkt->ui1_stream_id == (UINT8)pt_per_strm_info->ui4_stream_number)
        {   
            if(pt_per_strm_info->ui4_present_time == 0)
            {
                pt_bd_pkt->ui4_first_met_pkt = pt_asf_instance_obj->ui4_cur_packet_count;
                DBG_INFO(("_asf_analyse_paket_info:strm id %d met when parsing %d packet\r\n",
                   pt_bd_pkt->ui1_stream_id, pt_bd_pkt->ui4_first_met_pkt));
            }
            pt_per_strm_info->ui4_present_time = pt_bd_pkt->ui4_pts;
            /* check audio*/
            if(ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_per_strm_info->pt_stream_obj->t_strm_type), &gt_asf_object_stream_type_audio))
            {
                b_audio = TRUE;
            }
            else
            {
                b_audio = FALSE;
            }
            break;
        }
        /* find the next*/
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }
    /*Error Handle*/
    if(pt_per_strm_info == NULL)
    {
        DBG_ERROR(("_asf_bad_interleaved_pts_diff: pt_per_strm_info"
                   "is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    /* check the max difference */
    pt_per_strm_info = pt_all_strm_info->p_strm_info_head;
    while (NULL != pt_per_strm_info)
    {   
        if (pt_bd_pkt->ui1_stream_id == (UINT8)pt_per_strm_info->ui4_stream_number)
        {   
            /*ignore the same stream*/
            pt_per_strm_info = pt_per_strm_info->pt_next_strm;
            continue;
        }
        /* have the no present time stream*/
        if(pt_per_strm_info->ui4_present_time == 0)
        {
            ui4_max         = 0;
            pt_bd_pkt->b_met_all_strms = FALSE;
            break;
        }
        else
        {
            pt_bd_pkt->b_met_all_strms = TRUE;
        }
        /* check the different time*/
        if(pt_per_strm_info->ui4_present_time > pt_bd_pkt->ui4_pts)
        {
            ui4_defferent_time = pt_per_strm_info->ui4_present_time - pt_bd_pkt->ui4_pts;
        }
        else
        {
            ui4_defferent_time = pt_bd_pkt->ui4_pts - pt_per_strm_info->ui4_present_time;
        }
        /* save the max */
        if(ui4_max < ui4_defferent_time)
        {
            ui4_max = ui4_defferent_time;
        }
        /* find the next*/
        pt_per_strm_info = pt_per_strm_info->pt_next_strm;
    }

    if ((pt_asf_instance_obj->ui4_cur_packet_count == 499 && ui4_max == 0) ||
        (b_audio  && ui4_max > MINFOR_ASF_BAD_INTERLEAVED_MAX_AUD_DIFFERENCE) ||
        (!b_audio && ui4_max > ui4_vid_threshold))
    {
        pt_asf_instance_obj->b_is_bad_interleaved = TRUE;
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_load_packet
 *
 * Description: This function is to check interleave of audio and video.
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * History:
 *   (1)2010-12-16 : initial
           
 ----------------------------------------------------------------------------*/
static INT32 _asf_load_packet(MINFO_OBJ_T* pt_minfo_obj)
{
    INT32                               i4_ret = MINFOR_OK;
    MINFO_ASF_INSTANCE_INFO_T*          pt_asf_instance_obj;
    ASF_BD_PKT_T*                       pt_bd_pkt;
    getpos_fct                          pf_get_pos; 
    setpos_fct                          pf_set_pos; 
    input1bytes_fct                     pf_input1bytes;
    input2bytes_fct                     pf_input2bytes;
    input4bytes_fct                     pf_input4bytes;

    UINT8                               ui1_type_flag;
    UINT8                               ui1_property_flag;
    UINT8                               ui1_payload_flag;
    UINT8                               ui1_i;                        /*the loop variable*/
    UINT32                              ui4_packet_len          = 0;               /*count payload in the multiple payload*/
    UINT32                              ui4_seqence_len         = 0;              /*the seqence length*/
    UINT32                              ui4_padding_len         = 0;              /*the padding length*/
    UINT32                              ui4_replicated_len      = 0;           /*the replicated length*/
    UINT32                              ui4_payload_len         = 0;              /*the payload length*/
    UINT32                              ui4_offset_into_obj     = 0;          /*the offset into object*/
    UINT64                              ui8_pkg_position        = 0;             /*the initial position for per pkg*/
    
    /* check parameter */
    if(NULL == pt_minfo_obj)
    {
        return MINFOR_INV_ARG;
    }
    
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T* )pt_minfo_obj->pv_handler_obj;
    if(NULL == pt_asf_instance_obj)
    {
        return MINFOR_INTERNAL_ERR;
    }

    pt_bd_pkt = &pt_asf_instance_obj->t_bd_pkt;

    pf_get_pos     = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_set_pos     = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_input1bytes = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_input2bytes = pt_minfo_obj->t_input_fct_tbl.pf_input2;
    pf_input4bytes = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    if(NULL == pf_get_pos || NULL == pf_set_pos || NULL == pf_input1bytes || NULL == pf_input2bytes || NULL == pf_input4bytes)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    pf_get_pos(pt_minfo_obj, &ui8_pkg_position);
    /* get the error correct flag ,if not correct error object, will be the packet type flag*/
    pf_input1bytes(pt_minfo_obj, &ui1_type_flag);
    if ((ui1_type_flag & 0x80) == 0x80/* have the correct error infor*/)
    {
        if ((0x00 != (ui1_type_flag & 0x60)) 
        || (0x00 != (ui1_type_flag & 0x10))
        || (0x02 != (ui1_type_flag & 0x0f)))
        {
            DBG_ERROR(("_asf_check_bad_interleaved:error file\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        /*skip error correct object data*/
        pf_set_pos(pt_minfo_obj, 2, MEDIA_SEEK_CUR);
        /* the packet type flag*/
        pf_input1bytes(pt_minfo_obj, &ui1_type_flag);
    }
    /* read the priority flag */
    pf_input1bytes(pt_minfo_obj, &ui1_property_flag);
    /* read packet length */
    ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_type_flag, ASF_MASK_5, ASF_MASK_6, ui4_packet_len);
    if (ui4_packet_len == 0)
    {
        if(pt_asf_instance_obj->t_file_object.ui4_max_packet_size == pt_asf_instance_obj->t_file_object.ui4_min_packet_size)
        {
            ui4_packet_len = pt_asf_instance_obj->t_file_object.ui4_max_packet_size;
        }
    }
    pt_bd_pkt->ui4_packet_size = ui4_packet_len;
    
    /*read the sequence*/
    ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_type_flag, ASF_MASK_1, ASF_MASK_2, ui4_seqence_len);
    /*read the padding len*/
    ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_type_flag, ASF_MASK_3, ASF_MASK_4, ui4_padding_len);
    /* skip the send time & duration time */
    pf_set_pos(pt_minfo_obj, 6, MEDIA_SEEK_CUR);
    /* payload parsing
                just more read one byte
        */
    if((ui1_type_flag & 0x01) == 0x00/*simple payload*/)
    {
        /*
                 stream id [6-7]                                                     
                 media object number[4-5]
                 offset into media object  (present time)[2-3]
                 replicated data len         (replicated data len)[0-1]
                 replicated data              (present time delta = 1)
                 payload data                 (sub payload data)
              */
        DBG_INFO(("_asf_check_bad_interleaved:simple payload\r\n"));
        /* get stream id*/
        pf_input1bytes(pt_minfo_obj, &pt_bd_pkt->ui1_stream_id);
        pt_bd_pkt->ui1_stream_id = pt_bd_pkt->ui1_stream_id & (ASF_MASK_7 -1);
        /* skip the media object number length */
        ASF_SKIP_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_4, ASF_MASK_5);
        /*offset into media object*/
        ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_2, ASF_MASK_3, ui4_offset_into_obj);
        /*replicated data len*/
        ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_0, ASF_MASK_1, ui4_replicated_len);
        /*read present time in the object of replicated data object*/
        if(ui4_replicated_len != 1)
        {
            pf_set_pos(pt_minfo_obj, 4, MEDIA_SEEK_CUR);
            pf_input4bytes(pt_minfo_obj, &pt_bd_pkt->ui4_pts);
        }
        else
        {
            pt_bd_pkt->ui4_pts = ui4_offset_into_obj;
        }
        _asf_analyse_paket_info(pt_minfo_obj);
        i4_ret = pf_set_pos(pt_minfo_obj, ui8_pkg_position + (UINT64)ui4_packet_len /*+ (UINT64)ui4_padding_len*/ , MEDIA_SEEK_BGN);
    }
    else/*multiple payload*/
    {
        DBG_INFO(("_asf_check_bad_interleaved:multiple payload\r\n"));
        pf_input1bytes(pt_minfo_obj, &ui1_payload_flag);
        /*parsing the multiple payload*/
        for(ui1_i = 0;ui1_i < (ui1_payload_flag & 0x3F); ui1_i++)
        {  
            /*initial before loop using*/                   
            ui4_offset_into_obj     = 0;                    
            ui4_replicated_len      = 0;                    
            ui4_payload_len         = 0;        
            
            /*stream id*/
            pf_input1bytes(pt_minfo_obj, &pt_bd_pkt->ui1_stream_id);
            pt_bd_pkt->ui1_stream_id = pt_bd_pkt->ui1_stream_id & (ASF_MASK_7 -1);
            /* skip the media object number length*/
            ASF_SKIP_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_4, ASF_MASK_5);
            /*offset into media object*/
            ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_2, ASF_MASK_3, ui4_offset_into_obj);
            /*replicated data len*/
            ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_property_flag, ASF_MASK_0, ASF_MASK_1, ui4_replicated_len);
            /*read present time in the object of replicated data object*/
            if(ui4_replicated_len != 1)
            {
                pf_set_pos(pt_minfo_obj, 4, MEDIA_SEEK_CUR);
                pf_input4bytes(pt_minfo_obj, &pt_bd_pkt->ui4_pts);
                /* skip the replicated data*/
                pf_set_pos(pt_minfo_obj, ui4_replicated_len - 8, MEDIA_SEEK_CUR);
            }
            else
            {
                pt_bd_pkt->ui4_pts = ui4_offset_into_obj;
                /* skip the replicated data*/
                pf_set_pos(pt_minfo_obj, ui4_replicated_len, MEDIA_SEEK_CUR);
            }
            /*read the payload len*/
            ASF_GET_DATA_BY_MASK(pt_minfo_obj, ui1_payload_flag, ASF_MASK_6, ASF_MASK_7, ui4_payload_len);
            /*skip the payload data*/
            pf_set_pos(pt_minfo_obj, ui4_payload_len, MEDIA_SEEK_CUR);
            _asf_analyse_paket_info(pt_minfo_obj);
        }
        /*skip the padding data*/
        pf_set_pos(pt_minfo_obj, ui4_padding_len, MEDIA_SEEK_CUR);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_check_bad_interleaved
 *
 * Description: This function is to check interleave of audio and video.
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * History:
 *   (1)2010-12-16 : initial
           
 ----------------------------------------------------------------------------*/
static INT32 _asf_bad_interleaved_check(MINFO_OBJ_T* pt_minfo_obj)
{
    INT32                               i4_ret = MINFOR_OK;
    MINFO_ASF_INSTANCE_INFO_T*          pt_asf_instance_obj;
    getpos_fct                          pf_get_pos; 
    setpos_fct                          pf_set_pos; 

    UINT64                              ui8_old_position        = 0;
    UINT32                              ui4_total_pkg           = 0;
    UINT32                              ui4_pkg_count           = 0;
    BOOL                                b_index_obj_exsit       = FALSE;
    
    /* check parameter */
    if(NULL == pt_minfo_obj)
    {
        return MINFOR_INV_ARG;
    }
    
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T* )pt_minfo_obj->pv_handler_obj;
    if(NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("%s: pt_asf_instance_obj:NULL\r\n",__func__));
        return MINFOR_INTERNAL_ERR;
    }

    pf_get_pos     = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_set_pos     = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
   
    if(NULL == pf_get_pos || NULL == pf_set_pos)
    {
        return MINFOR_INTERNAL_ERR;
    }

    /*check the index table exist*/
    if ( _asf_bit_map_is_set( pt_asf_instance_obj->t_idx_info.aui1_idx_strm_map, 16, 128 ) )
    {
        DBG_INFO(("%s: Index table object is exist.\r\n",__func__));
        b_index_obj_exsit = TRUE;
    }
    
    /* get the old position*/
    i4_ret = pf_get_pos(pt_minfo_obj, &ui8_old_position);
    if(i4_ret != INPUTR_OK)
    {
        DBG_INFO(("%s: Get Pos fail,return is %d.\r\n",__func__, __LINE__));
        return MINFOR_FILE_ERR;   
    }
    /* set position to the first packet*/
    i4_ret = pf_set_pos(pt_minfo_obj, pt_asf_instance_obj->t_data_object.ui8_data_packet_begin, MEDIA_SEEK_BGN);
    /* total PKG number */
    if(pt_asf_instance_obj->t_data_object.ui8_total_packets < MINFOR_ASF_BAD_INTERLEAVED_CHECK_PKG_NUM)
    {
        ui4_total_pkg = pt_asf_instance_obj->t_data_object.ui8_total_packets;
    }
    else
    {
        ui4_total_pkg = MINFOR_ASF_BAD_INTERLEAVED_CHECK_PKG_NUM;
    }

    while(ui4_pkg_count < ui4_total_pkg/*PKG number*/)
    {
        pt_asf_instance_obj->ui4_cur_packet_count = ui4_pkg_count++;
        i4_ret = _asf_load_packet(pt_minfo_obj);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        if (!pt_asf_instance_obj->b_is_bad_interleaved && ui4_pkg_count == 500)
        {
            DBG_INFO(("%s: the file is well interleaved.\r\n",__func__));
            break;
        }
        

        if (pt_asf_instance_obj->b_is_bad_interleaved && 
            (pt_asf_instance_obj->t_bd_pkt.b_met_all_strms ||
             (!pt_asf_instance_obj->t_bd_pkt.b_met_all_strms && b_index_obj_exsit)))
        {
            DBG_INFO(("%s: the file is bad interleaved.\r\n",__func__));
            break;
        }
    }    
    
    /*Resume the old position*/
    i4_ret = pf_set_pos(pt_minfo_obj, ui8_old_position, MEDIA_SEEK_BGN);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_FILE_ERR;
    }
    else
    {
        return MINFOR_OK;
    }
}

#endif

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_object_parse
 *
 * Description: This function parse the asf container context,it is the 
 *              parser entry.
 *
 * Inputs:  pt_minfo_obj: specifies the Minfo object
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK :              if successful
 *          others non zero values:  if unsuccessful
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-12 : initial
           
 ----------------------------------------------------------------------------*/

INT32 asf_minfo_object_parse( MINFO_OBJ_T* pt_minfo_obj )
{
    /*setpos function*/
    setpos_fct                  pf_set_pos;  
    getpos_fct                  pf_get_pos; 
    /*copy bytes function*/
    copybytes_fct               pf_copybyte;
    /*store asf parsed related info*/
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj; 
    /*read buffer pointer*/
    UINT8*                      pui1_head ;  
    /*how many bytes are read from a file*/
    UINT32                      ui4_read_size;
    /*return code s*/
    INT32                       i4_ret; 
    /*object file position*/
    UINT64                      ui8_file_pos;
    static BOOL                 b_first_simple_idx_obj;
    static BOOL                 b_idx_obj;
    /*dump  a object  temporarily*/
    ASF_OBJ_COMMON_T            t_obj;
    #ifdef CLI_LVL_ALL
    CHAR                        ac_temp[40]; 
     /*GUID str temp buffer length*/
    UINT32                      ui4_len;  
    #endif
    BOOL                        b_header_obj_done;
    BOOL                        b_data_obj_done;
    UINT32                      ui4_idx_entry_count;    
    
     
    DBG_INFO(("asf_minfo_parse:begin to parse asf\r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

    if (NULL == pt_minfo_obj)
    {
        DBG_ERROR(("asf_minfo_object_parse: pt_minfo_obj is NULL\r\n"));
        return  MINFOR_INTERNAL_ERR;
    }

    /*asf will use the folloeing input functions*/
    if ((pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_get_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        DBG_ERROR(("asf_minfo_object:input_fct_tbl has NULL function\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
 
    /************************************************************************
     Do                                                              
    ************************************************************************/
    /*init variables*/
    x_memset(&t_obj, 0, sizeof(ASF_OBJ_COMMON_T));
    /*end of file indicator*/
    /*how many bytes are read from a file*/
    ui4_read_size = 0;
    /*return code s*/
    i4_ret = MINFOR_OK; 
    /*object file position*/
    ui8_file_pos = 0;
    b_first_simple_idx_obj = FALSE;
    b_idx_obj              = FALSE;

    b_header_obj_done = FALSE;
    b_data_obj_done = FALSE;
    
    /*get minfo handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*) 
                           pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("asf_minfo_object_parse:asf_instance_obj is NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;

    /*set at beginning of the file */
    pf_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN); 

    /*set current parsing position 0*/
    pt_asf_instance_obj->ui8_cur_parser_offset = 0;

    /*init variable*/
    pt_asf_instance_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
    
    /* end of file indicator ,If file corrupted,what can we do?*/
    while (pt_asf_instance_obj->ui8_cur_parser_offset 
           < pt_minfo_obj->z_buff_leng)
    {
        /*set to ui8_cur_parser_offset*/
        pf_set_pos(pt_minfo_obj, 
                   (INT64)pt_asf_instance_obj->ui8_cur_parser_offset, 
                   FM_SEEK_BGN); 
        
        /*get file position*/
        pf_get_pos(pt_minfo_obj, &ui8_file_pos);
        
        pui1_head = pt_asf_instance_obj->pui1_header_buf;
        if (NULL == pui1_head)
        {
            DBG_ERROR(("asf_minfo_object_parse:pui1_head is NULL\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        /*read object header from file*/
        i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                             pt_asf_instance_obj->ui4_buf_length , 
                             (SIZE_T)ASF_OBJ_COMMON_LEN, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("asf_minfo_object_parse:error object header! "
                       "position is %llu\r\n",
                       pt_asf_instance_obj->ui8_cur_parser_offset));
            /*If header and data are parsed other data may be junk do not read
            CQ ID is :DTV00124963 & DTV00125016*/
            if ((TRUE == b_header_obj_done) && (TRUE == b_data_obj_done))
            {
                break; /*consider ok*/
            }
            
            return MINFOR_INTERNAL_ERR;
        }
        
        /*restore file position*/
        pf_set_pos(pt_minfo_obj, (INT64)ui8_file_pos, FM_SEEK_BGN); 
 
        /*get object guid and size*/
        _asf_get_guid(&(t_obj.t_guid), pui1_head);
        ASF_GET_QWORD(t_obj.ui8_size, pui1_head + 16); 
        #ifdef CLI_LVL_ALL
        ui4_len = 40;
        x_memset(ac_temp, 0, 40);
        _asf_guid_to_str(&(t_obj.t_guid), ac_temp, &ui4_len);
        DBG_INFO(("asf_minfo_object_parse:OBJECT GUID=%s  SIZE=%llu\r\n", 
                  ac_temp,
                  t_obj.ui8_size));
        #endif
        /*If object size is valid in case file is corrupt*/
        if (0 == t_obj.ui8_size)
        {
            DBG_ERROR(("asf_minfo_object_parse:error object header! "
                       "object size is zero\r\n"));
            /* If the data and file object have parsed, conside ignoring the valid handle*/
            if ((TRUE == b_header_obj_done) && (TRUE == b_data_obj_done))
            {
                break; /*consider ok*/
            }
            return MINFOR_INTERNAL_ERR;
        }

        /*parse header info*/
        if (ASFR_GUID_EQUAL == _asf_cmp_guid((const ASF_GUID_T*)&(t_obj.t_guid), 
                                             &gt_asf_object_header_guid))
        {
        
            pt_asf_instance_obj->t_root.pt_header = (ASF_OBJ_COMMON_T *) 
                                    _mm_util_mem_alloc(sizeof(ASF_OBJ_COMMON_T));
            if (NULL == pt_asf_instance_obj->t_root.pt_header)
            {
                DBG_ERROR(("asf_minfo_object_parse: failed to allocate memory "
                           "for pt_asf_header_obj\r\n"));
                return MINFOR_OUT_OF_MEMORY;
            }
            x_memset(pt_asf_instance_obj->t_root.pt_header, 0, 
                     sizeof(ASF_OBJ_COMMON_T));
            
            i4_ret = _asf_load_header_object(pt_minfo_obj, 
                                             pt_asf_instance_obj
                                             ->t_root.pt_header);
            if (MINFOR_OK != i4_ret)
            {
                _asf_free_header_object(pt_minfo_obj, 
                                              &(pt_asf_instance_obj
                                              ->t_root.pt_header));
                DBG_ERROR(("asf_minfo_object_parse:failed to load header\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            b_header_obj_done = TRUE;
            
        }
        /*parse data info*/
        else if  (ASFR_GUID_EQUAL == _asf_cmp_guid((const ASF_GUID_T*)&(t_obj.t_guid), 
                                                   &gt_asf_object_data_guid))
        {
            pt_asf_instance_obj->t_root.pt_data = (ASF_OBJ_COMMON_T *)
                                          _mm_util_mem_alloc(sizeof(ASF_OBJ_COMMON_T));
            if (NULL == pt_asf_instance_obj->t_root.pt_data)
            {
                DBG_ERROR(("asf_minfo_object_parse:fail to allocate memory "
                           "for pt_data\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            x_memset(pt_asf_instance_obj->t_root.pt_data, 0, 
                     sizeof(ASF_OBJ_COMMON_T));
            
            i4_ret = _asf_load_data_object(pt_minfo_obj, 
                                           pt_asf_instance_obj->t_root.pt_data);
            if (MINFOR_OK != i4_ret)
            {
#ifdef ENABLE_MMS_SUPPORT
                if(pt_minfo_obj->b_only_has_hdr)
                {
                    return MINFOR_OK;
                }
#endif
                (VOID)_asf_free_data_object(pt_minfo_obj, 
                                            &(pt_asf_instance_obj->t_root.pt_data));
                DBG_ERROR(("asf_minfo_object_parse:failed to load header\r\n"));
                return MINFOR_INTERNAL_ERR;
            }   
            b_data_obj_done = TRUE;
            
            /* check the server if to support the operation of seek*/
            if (pt_asf_instance_obj->b_non_seekable)
            {
                return MINFOR_OK;
            }
        }
        /*parse simple object to get offset used by index builder module*/
        else if (ASFR_GUID_EQUAL == _asf_cmp_guid((const ASF_GUID_T*)&(t_obj.t_guid), 
                                                  &gt_asf_object_simple_index_guid))
        {
            if (!b_first_simple_idx_obj)
            {
                pt_asf_instance_obj->t_idx_info.ui8_idx_offset
                                 = pt_asf_instance_obj->ui8_cur_parser_offset;
                pt_asf_instance_obj->t_idx_info.ui8_idx_sz = t_obj.ui8_size;

                /*get simple index entry number offset: not more than ui4_buf_length
                                    (ASF_OBJ_COMMON_LEN + 16 + 8 + 4 +4 )*/
                /*get file position*/
                pf_get_pos(pt_minfo_obj, &ui8_file_pos);
                i4_ret = pf_copybyte(pt_minfo_obj, pui1_head, 
                                     pt_asf_instance_obj->ui4_buf_length , 
                                     (SIZE_T)(ASF_OBJ_COMMON_LEN + 16 + 8 + 4 +4 ), 
                                     &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    DBG_ERROR(("asf_minfo_object_parse:get index entry number "
                               "err!\r\n",
                               i4_ret));
                    return MINFOR_INTERNAL_ERR;
                }
                /*restore file position*/
                pf_set_pos(pt_minfo_obj, (INT64)ui8_file_pos, FM_SEEK_BGN); 
                /*get simple index entry number */
                pui1_head += (ASF_OBJ_COMMON_LEN + 16 + 8 + 4); 
                ASF_GET_DWORD(ui4_idx_entry_count, pui1_head);
                DBG_INFO(("asf_minfo_object_parse:index entry number= %d!!!\r\n",
                          ui4_idx_entry_count));
                if (ui4_idx_entry_count > 1) /*if only one consider no index*/
                {
                    pt_asf_instance_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
                }
                else
                {
                    pt_asf_instance_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
                }

                b_first_simple_idx_obj = TRUE;
            }
            t_obj.ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
            
            ASF_SET_CUR_PARSE_POS(pt_minfo_obj, t_obj.ui8_size, i4_ret);
            if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
            {   
                DBG_ERROR(("asf_minfo_object_parse:set pos error simp\r\n"));
                /*If header and data are parsed other data may be junk do not read
                CQ ID is :DTV00209727*/
                if ((TRUE == b_header_obj_done) && (TRUE == b_data_obj_done))
                {
                    break; /*consider ok*/
                }
                return MINFOR_INTERNAL_ERR;
            }
            /*display  object*/
            #ifdef CLI_LVL_ALL
            /*dispay the object */
            t_obj.e_type = ASF_OBJ_SIMPLE_INDEX;
            _asf_dump_object(&t_obj);
            #endif
        }
        else if (ASFR_GUID_EQUAL == _asf_cmp_guid((const ASF_GUID_T*)&(t_obj.t_guid), 
                                                  &gt_asf_object_index_guid))
        {
            if ( !b_idx_obj &&
                 pt_asf_instance_obj->t_ext_content_des.e_scopic_type == MM_SCOPIC_DS)
            {
                pt_asf_instance_obj->t_idx_info.ui8_idx_offset
                                 = pt_asf_instance_obj->ui8_cur_parser_offset;
                pt_asf_instance_obj->t_idx_info.ui8_idx_sz = t_obj.ui8_size;

                pt_asf_instance_obj->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
                _asf_bit_map_set( ( UINT8*)&( pt_asf_instance_obj->t_idx_info.aui1_idx_strm_map[0]),
                          16, 128 );

                b_idx_obj = TRUE;
            }
            
            t_obj.ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
            
            ASF_SET_CUR_PARSE_POS(pt_minfo_obj, t_obj.ui8_size, i4_ret);
            /*display  object*/
            #ifdef CLI_LVL_ALL
            /*dispay the object */
            t_obj.e_type = ASF_OBJ_INDEX;
            _asf_dump_object(&t_obj);
            #endif
        }
        else
        {
            /*just skip the top level unsupported object*/
            /*change current parsing position*/
            t_obj.ui8_offset = pt_asf_instance_obj->ui8_cur_parser_offset;
            
            ASF_SET_CUR_PARSE_POS(pt_minfo_obj, t_obj.ui8_size, i4_ret);
            if (ASF_MINFO_SET_FILE_POS_ERR == i4_ret)
            {   
                DBG_ERROR(("asf_minfo_object_parse:set pos error unknown\r\n"));
                /*If header and data are parsed other data may be junk do not read
                CQ ID is :DTV00209727*/
                if ((TRUE == b_header_obj_done) && (TRUE == b_data_obj_done))
                {
                    break; /*consider ok*/
                }
                return MINFOR_INTERNAL_ERR;
            }

            /*display  object*/
            #ifdef CLI_LVL_ALL
            /*dispay the object */
            t_obj.e_type = ASF_OBJ_UNKNOWN;
            _asf_dump_object(&t_obj);
            #endif
        }

        /*If header , data and index are parsed other data may be junk do not read
        CQ ID is :DTV00208705*/
        if ((TRUE == b_header_obj_done) 
             && (TRUE == b_data_obj_done)
             && (( TRUE == b_first_simple_idx_obj ) || ( b_idx_obj == TRUE )))
        {
            break; /*consider ok*/
        }
    }
    
    DBG_INFO(("asf_minfo_object_parse:end asf parsing including header and data\r\n"));
    
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /* check the bad interleaved */
    i4_ret = _asf_bad_interleaved_check(pt_minfo_obj); 
    if(i4_ret != MINFOR_OK)
    {
        pt_asf_instance_obj->b_is_bad_interleaved = FALSE;
    }
#endif
    
#ifdef CLI_LVL_ALL
    _asf_cli_dump_object( pt_minfo_obj, ASF_OBJECT_BUTT );
#endif
    return MINFOR_OK;
    
    /***********************************************************************
    *************************************************************************/
}

#ifdef __cplusplus
}
#endif
