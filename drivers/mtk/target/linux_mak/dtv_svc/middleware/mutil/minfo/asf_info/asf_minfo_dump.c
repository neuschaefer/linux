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
 * $Date: 2012/04/27 $ 2004/11/26
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
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "asf_parser_common.h"
#include "asf_minfo_dump.h"
#include "cli/x_cli.h"
#include "cli/u_cli.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "../../mm_util.h"
#include "x_uc_str.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "asf_parser_common.h"
#include "asf_minfo_dump.h"
#include "x_cli.h"
#include "u_cli.h"

#endif

#ifdef CLI_LVL_ALL

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

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/
extern BOOL _minfo_cli_get_dump_flag(VOID);

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_file_object
 *
 * Description: This function dumps a file object info
 *
 * Inputs:  pv_specific: point to a file object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/
VOID  _asf_dump_file_object( VOID* pv_specific )
{
    /* File Object*/
    ASF_FILE_PROPERTIES_OBJECT_T* pt_file_object;  
    /*GUID str temp buffer*/
    CHAR                          ac_temp[40]; 
     /*GUID str temp buffer length*/
    UINT32                        ui4_len;  
    UINT64                        ui8_create_tm;
    MM_TM_T                       t_tm;
    
    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        x_dbg_stmt("Error: _asf_dump_file_object:NULL\r\n");
        return;
    }

    
    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    x_memset(ac_temp, 0, 40);
    ui4_len = 40;  
    
    /*get file object*/
    pt_file_object = (ASF_FILE_PROPERTIES_OBJECT_T*)pv_specific;
   
    _asf_guid_to_str(&(pt_file_object->t_file_id), ac_temp, &ui4_len);
    x_dbg_stmt("File ID:%s\r\n", ac_temp);
    
    x_dbg_stmt("\r\n");
    
    x_dbg_stmt("File Size:\t%llu\r\n", 
              pt_file_object->ui8_file_size);
    
    x_dbg_stmt("Packets Count:\t%llu\r\n", 
              pt_file_object->ui8_packets_count);

    
    ui8_create_tm = _mm_div64(pt_file_object->ui8_create_date,
                             10000000,
                             NULL);
    _mm_gmtime((MM_TIME_T)ui8_create_tm, MM_GMT_ASF_EPOCH_YEAR_M, &t_tm);
    x_dbg_stmt("Create Date:\t%u-%u-%u %u:%u:%u\r\n", 
              t_tm.ui2_year,
              t_tm.ui1_month,
              t_tm.ui1_day,
              t_tm.ui1_hour,
              t_tm.ui1_min,
              t_tm.ui1_sec);
    
    x_dbg_stmt("Send Duration:\t%llu\r\n", 
              pt_file_object->ui8_send_duration);

    x_dbg_stmt("Preroll:\t%llu\r\n", 
              pt_file_object->ui8_preroll);
    
    x_dbg_stmt("Flags:\t%u\r\n", 
              pt_file_object->ui4_flags);
    
    x_dbg_stmt("Min Packet Size:\t%u\r\n", 
              pt_file_object->ui4_min_packet_size);
    
    x_dbg_stmt("Max Packet Size:\t%u\r\n", 
              pt_file_object->ui4_max_packet_size);
    
    x_dbg_stmt("Max Bitrate:\t%u\r\n", 
              pt_file_object->ui4_max_bitrate);

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_stream_properties
 *
 * Description: This function dumps the stream properties object info
 *
 * Inputs:  pv_specific: point to a stream properties object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/

VOID  _asf_dump_stream_properties ( VOID* pv_specific )
{
    /*stream properties object*/
    ASF_STREAM_PROPERTIES_OBJECT_T*  pt_strm_properties;
    /*video specific  stream properties */
    MINFO_ASF_PER_VID_STRM_INF_T*    pt_vid_strm_inf;
    /*audio specific  stream properties */
    MINFO_ASF_PER_AUD_STRM_INF_T*    pt_aud_strm_inf;
    MINFO_ASF_AUDIO_STM_ATRBT_T*     pt_aud_strm_atrbt;
    MINO_SPREAD_AUDIO_T*             pt_error_correction;
    MINF_BITMAPINFOHEADER_T*         pt_bitmap_info_header;
    MINFO_ASF_VIDEO_STM_ATRBT_T*     pt_vid_strm_atrbt;

    CHAR                             ac_temp[40]; /*GUID str temp buffer*/
    UINT32                           ui4_len;     /*GUID str temp buffer length*/
    UINT32                           ui4_i;       /*loop variable*/

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_stream_properties:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_stream_properties:NULL\r\n"); 
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    x_memset(ac_temp, 0, 40);
    ui4_len = 40;  
    ui4_i = 0; 

    
    /*get stream properties object*/
    pt_strm_properties = (ASF_STREAM_PROPERTIES_OBJECT_T *)pv_specific;

    if (ASFR_GUID_EQUAL 
        == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                         &gt_asf_object_stream_type_audio))
    {
        x_dbg_stmt("Stream Type: Audio\r\n");
    }
    else if (ASFR_GUID_EQUAL 
             == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                              &gt_asf_object_stream_type_video))
    {
        x_dbg_stmt("Stream Type: Video\r\n");
    }
    else
    {
        _asf_guid_to_str(&(pt_strm_properties->t_strm_type), 
                         ac_temp, &ui4_len);
        x_dbg_stmt("Unknown Stream Type, GUID:%s\r\n", 
                  ac_temp);
    }

    if (ASFR_GUID_EQUAL 
        == _asf_cmp_guid(&(pt_strm_properties->t_err_correct_type), 
                         &gt_asf_guid_audio_spread))
    {
        x_dbg_stmt("Error Type: Audio Spread\r\n");
    }
    else
    {
        x_dbg_stmt("Error Type: NULL\r\n");
    }

    
    x_dbg_stmt("Time Offset: %llu\r\n", 
              pt_strm_properties->ui8_time_offset);

    x_dbg_stmt("Type Specific Data Length: %u\r\n", 
              pt_strm_properties->ui4_type_spec_len);
  
    x_dbg_stmt("Error Correction Data Length: %u\r\n", 
              pt_strm_properties->ui4_err_correct_len);

    x_dbg_stmt("Flags: %u\r\n", 
              pt_strm_properties->ui2_flags);

    x_dbg_stmt("Stream Number: %u\r\n", 
              pt_strm_properties->ui1_stream_number);

    x_dbg_stmt("Encrypt: %u\r\n", 
              pt_strm_properties->b_encrypt_flag);

    x_dbg_stmt("Reserved: %u\r\n", 
              pt_strm_properties->ui4_reserved);

    /*if audio show WAVEFORMATEX*/
    if (ASFR_GUID_EQUAL 
        == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                         &gt_asf_object_stream_type_audio))
    {
        pt_aud_strm_inf = (MINFO_ASF_PER_AUD_STRM_INF_T *)
                           pt_strm_properties->puil_type_spec_data;

        /***************get audio specific data*****************************/
        pt_aud_strm_atrbt = &(pt_aud_strm_inf->t_aud_strm_atrbt);
        x_dbg_stmt("\r\nThe Minfo Module want to obtain the following info:\r\n");
        
        /*get codec ID/format tag, 2 bytes */
        x_dbg_stmt("Codec ID/Format Tag: %u\r\n", 
                  pt_aud_strm_atrbt->e_enc);

        /*get layer value*/
        x_dbg_stmt("Layer: %u\r\n", 
                  pt_aud_strm_atrbt->e_layer);

        /*get number of channels ,2 bytes*/
        x_dbg_stmt("Number Of Channels: %u\r\n", 
                  pt_aud_strm_atrbt->i2_channels);
       
        /*get samples per second, 4 bytes*/
        x_dbg_stmt("Samples Per Second: %u\r\n", 
                  pt_aud_strm_atrbt->i4_samples_per_sec);
        

        /*get average number of bytes per second*/
        x_dbg_stmt("Average Number Of Bytes Per Second"
                  "(Minfo Module Don't Get): %u\r\n", 
                  pt_aud_strm_atrbt->ui4_avg_bytes_per_sec);
        
        /*get block alignment ,2 bytes*/
        x_dbg_stmt("Block Alignment: %u\r\n", 
                  pt_aud_strm_atrbt->i2_block_align);
        
        /*get bit per sample ,2 bytes*/
        x_dbg_stmt("Bit Per Sample: %u\r\n", 
                  pt_aud_strm_atrbt->i2_bits_per_sample);

        /*get stream language*/
        x_dbg_stmt("Stream Language: %s\r\n", 
                  pt_aud_strm_atrbt->s_lang);

        /*get default field*/
        x_dbg_stmt("Default: %u\r\n", 
                  pt_aud_strm_atrbt->b_default);

        x_dbg_stmt("\r\nEnd Of The Minfo Module Obtaining\r\n");

        /*get code specific lenght ,2 bytes*/
        x_dbg_stmt("Audio Code Specific Lenght: %u\r\n", 
                  pt_aud_strm_atrbt->ui4_codec_info_size);

        /*audio code specific*/
        if (NULL != pt_aud_strm_atrbt->pui1_codec_info)
        {
            x_dbg_stmt("Audioo Code Specific:");
            for (ui4_i = 0; ui4_i < pt_aud_strm_atrbt->ui4_codec_info_size; 
                 ++ui4_i)
            {
                x_dbg_stmt("%02x  ",pt_aud_strm_atrbt->pui1_codec_info[ui4_i]);
            }
            x_dbg_stmt("\r\n"); 
        }
        
        /*****************begin audio error-specific data****************/
        if ( (ASFR_GUID_EQUAL 
            == _asf_cmp_guid(&(pt_strm_properties->t_err_correct_type), 
                             &gt_asf_guid_audio_spread))
            && (pt_strm_properties->ui4_err_correct_len > 0))
        {
            x_dbg_stmt("Error Correction Length: %u\r\n", 
                      pt_strm_properties->ui4_err_correct_len);

            pt_error_correction = &(pt_aud_strm_inf->t_error_correction);
            /*get span*/
            x_dbg_stmt("Span: %u\r\n", 
                      pt_error_correction->ui1_span);
            
            /*get virtual packet length ,2 bytes*/
            x_dbg_stmt("Virtual Packet Length: %u\r\n", 
                      pt_error_correction->ui2_vir_pack_len);
            
            /*get chunk length ,2 bytes*/
            x_dbg_stmt("Chunk Length: %u\r\n", 
                      pt_error_correction->ui2_vir_chunk_len);

            /*get data length ,2 bytes, should be 1*/
            x_dbg_stmt("Silence Length: %u\r\n", 
                      pt_error_correction->ui2_silence_len);

            /*the silence data should be 0*/
            x_dbg_stmt("Silence: 0\r\n");
        
        }
    }
    /*if video show BITMAPINfOHEADER*/
    else if (ASFR_GUID_EQUAL == _asf_cmp_guid(&(pt_strm_properties->t_strm_type), 
                                              &gt_asf_object_stream_type_video))
    {
        pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T *)
                           pt_strm_properties->puil_type_spec_data;
        /***********get video specific data********************/

        x_dbg_stmt("The BITMAPINfOHEADER Info:\r\n");

        /*get format data size, 2 bytes*/
        x_dbg_stmt("BITMAPINfOHEADER Size:%u\r\n", 
                  pt_vid_strm_inf->ui2_bitmap_header_size);

        pt_bitmap_info_header = &(pt_vid_strm_inf->t_bitmap_info_header);
        /*begin get bitmap info header data*/
        /*get format data size, 4 bytes*/
        if (pt_bitmap_info_header->ui4_size 
            != pt_vid_strm_inf->ui2_bitmap_header_size)
        {
            x_dbg_stmt("The BITMAPINfOHEADER size is wrong!\r\n");
        }
        
        /*get image width, 4 bytes*/
        x_dbg_stmt("Image Width:%u\r\n", 
                  pt_bitmap_info_header->ui4_width);
        
        /*get image height, 4 bytes*/
        x_dbg_stmt("Image Height:%u\r\n", 
                  pt_bitmap_info_header->ui4_height);

        /*get reserved, 2 bytes ,should be 1*/
        x_dbg_stmt("Reserved:%u\r\n", 
                  pt_bitmap_info_header->ui2_reserved);
      
        /*get bits per pixel count, 2 bytes */
        x_dbg_stmt("Bits Per Pixel Count:%u\r\n", 
                  pt_bitmap_info_header->ui2_bit_count);


        /*get compression ID, 4 bytes */
        x_dbg_stmt("Compression ID:%u\r\n", 
                  pt_bitmap_info_header->ui4_compression);

        /*get image size, 4 bytes */
        x_dbg_stmt("Image Size:%u\r\n", 
                  pt_bitmap_info_header->ui4_size_image);
        
        /*get horizontal pixels per meter, 4 bytes */
        x_dbg_stmt("Horizontal Pixels Per Meter:%u\r\n", 
                  pt_bitmap_info_header->ui4_xpels_per_meter);


        /*get virtical  pixels per meter, 4 bytes */
        x_dbg_stmt("Virtical Pixels Per Meter:%u\r\n", 
                  pt_bitmap_info_header->ui4_ypels_per_meter);
        
        /*get color used count, 4 bytes */
        x_dbg_stmt("Color Used Count:%u\r\n", 
                  pt_bitmap_info_header->ui4_clr_used);
        
        /*get important color count, 4 bytes */
        x_dbg_stmt("Important Color Count:%u\r\n", 
                  pt_bitmap_info_header->ui4_clr_important);

        /**********The Minfo Module want to obtian the following info:*********/
        x_dbg_stmt("\r\nThe Minfo Module want to obtain the following info:\r\n");

        pt_vid_strm_atrbt = &(pt_vid_strm_inf->t_vid_strm_atrbt);
        /*get video encode type*/
        x_dbg_stmt("Video Encode Type:%u\r\n", 
                  pt_vid_strm_atrbt->e_enc);


        /*get encoded image width, 4 bytes*/
        x_dbg_stmt("Image Width:%u\r\n", 
                  pt_vid_strm_atrbt->i4_width);
        
        /*get encoded image height, 4 bytes*/
        x_dbg_stmt("Image Height:%u\r\n", 
                  pt_vid_strm_atrbt->i4_height);
      
        /*get reserved flags, 1 bytes*/
        x_dbg_stmt("Default:%u\r\n", 
                  pt_vid_strm_atrbt->b_default);


        x_dbg_stmt("Video Code Specific Size:%u\r\n", 
                  pt_vid_strm_atrbt->ui4_codec_info_size);
        if (NULL != pt_vid_strm_atrbt->pui1_codec_info)
        {

            x_dbg_stmt("Video Code Specific :");
            for (ui4_i = 0; 
                 ui4_i < pt_vid_strm_atrbt->ui4_codec_info_size; 
                  ++ui4_i)
            {
                x_dbg_stmt("%02x  ", 
                          pt_vid_strm_atrbt->pui1_codec_info[ui4_i]);
            }
            x_dbg_stmt("\r\n"); 
        }

        x_dbg_stmt("\r\nEnd of The Minfo Module obtaining\r\n");

    }
    else
    {
        /*No data*/
    }
    
    return;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_language_list
 *
 * Description: This function dumps  the language list object info
 *
 * Inputs:  pv_specific: point to a language list object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/
VOID  _asf_dump_language_list ( VOID* pv_specific )
{

    /*Language List Object*/
    ASF_LANGUAGE_LIST_OBJECT_T* pt_language;
    /*loop variable*/
    UINT32                      ui4_i; 

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        x_dbg_stmt("Error: _asf_dump_language_list:NULL\r\n");
        return;
    }

    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    ui4_i = 0; 
    
    /*get language list object*/
    pt_language = (ASF_LANGUAGE_LIST_OBJECT_T *)pv_specific;


    /*get language count, 2 bytes*/
    x_dbg_stmt("Language Count: %u\r\n", 
              pt_language->ui2_lang_count);
    /*get language list*/
    if(0 != pt_language->ui2_lang_count)
    {
        if (NULL != pt_language->pt_languge)
        {
            /*get language list*/
            for (ui4_i = 0; ui4_i < pt_language->ui2_lang_count; ++ui4_i)
            {
                /*get language*/
                x_dbg_stmt("language[%u]: %s\r\n", 
                          pt_language->ui2_lang_count,
                          pt_language->pt_languge[ui4_i]);
                
            }
            
        }
        else
        {
        }
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_content_description
 *
 * Description: This function dumps  the content language object info
 *
 * Inputs:  pv_specific: point to a content content description structure
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
 *  (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_dump_content_description ( VOID* pv_specific )
{

    /*Content Description Object*/
    ASF_CONTENT_DESCRIPTION_OBJECT_T* pt_content_des;
    UINT32                            ui4_buf_len; 
    UINT32                            ui4_utf8_len; 

    CHAR*                             psz_buf = NULL; /*temporary buffer*/   
    INT32                             i4_ret;

    
    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        x_dbg_stmt("Error: _asf_dump_content_description:NULL\r\n");
        return;
    }

    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    ui4_buf_len = 0;
    
    /*get content description object*/
    pt_content_des = (ASF_CONTENT_DESCRIPTION_OBJECT_T *)pv_specific;

    do 
    {
        /*title info */
        if (NULL != pt_content_des->pwsz_title)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          (pt_content_des->pwsz_title) + 1;
            if (ui4_utf8_len > ui4_buf_len)
            {
                if (NULL != psz_buf)
                {
                    _mm_util_mem_free(psz_buf);
                }
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_utf8_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_content_description: failed "
                               "to allocate memory for for pwsz_title\r\n");
                    return;
                }
                ui4_buf_len = ui4_utf8_len;
            }

            i4_ret = x_uc_w2s_to_ps(pt_content_des->pwsz_title, 
                                    psz_buf, 
                                    ui4_buf_len);
            if(i4_ret != UCR_OK)
            {
                x_dbg_stmt("Error: _asf_dump_content_description:for pwsz_name "
                           "x_uc_w2s_to_ps failed(%d)\r\n", 
                           i4_ret);
                break; 
            }
            x_dbg_stmt("Title: %s\r\n", psz_buf);
            
        } 

        /*author info */
        if (NULL != pt_content_des->pwsz_author)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          (pt_content_des->pwsz_author) + 1;
            if (ui4_utf8_len > ui4_buf_len)
            {
                if (NULL != psz_buf)
                {
                    _mm_util_mem_free(psz_buf);
                }
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_utf8_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_content_description: failed "
                               "to allocate memory for for pwsz_author\r\n");
                    return;
                }
                ui4_buf_len = ui4_utf8_len;
            }

            i4_ret = x_uc_w2s_to_ps(pt_content_des->pwsz_author, 
                                    psz_buf, 
                                    ui4_buf_len);
            if(i4_ret != UCR_OK)
            {
                x_dbg_stmt("Error: _asf_dump_content_description:for pwsz_author "
                           "x_uc_w2s_to_ps failed(%d)\r\n", 
                           i4_ret);
                break; 
            }
            x_dbg_stmt("Author: %s\r\n", psz_buf);
            
        }
        
        /*copyright info */
        if (NULL != pt_content_des->pwsz_copyright)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          (pt_content_des->pwsz_copyright) + 1;
            if (ui4_utf8_len > ui4_buf_len)
            {
                if (NULL != psz_buf)
                {
                    _mm_util_mem_free(psz_buf);
                }
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_utf8_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_content_description: failed "
                               "to allocate memory for for pwsz_copyright\r\n");
                    return;
                }
                ui4_buf_len = ui4_utf8_len;
            }

            i4_ret = x_uc_w2s_to_ps(pt_content_des->pwsz_copyright, 
                                    psz_buf, 
                                    ui4_buf_len);
            if(i4_ret != UCR_OK)
            {
                x_dbg_stmt("Error: _asf_dump_content_description:for pwsz_copyright "
                           "x_uc_w2s_to_ps failed(%d)\r\n", 
                           i4_ret);
                break; 
            }
            x_dbg_stmt("Copyright: %s\r\n", psz_buf);
            
        }
        /*description info */
        if (NULL != pt_content_des->pwsz_description)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          (pt_content_des->pwsz_description) + 1;
            if (ui4_utf8_len > ui4_buf_len)
            {
                if (NULL != psz_buf)
                {
                    _mm_util_mem_free(psz_buf);
                }
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_utf8_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_content_description: failed "
                               "to allocate memory for for pwsz_description\r\n");
                    return;
                }
                ui4_buf_len = ui4_utf8_len;
            }

            i4_ret = x_uc_w2s_to_ps(pt_content_des->pwsz_description, 
                                    psz_buf, 
                                    ui4_buf_len);
            if(i4_ret != UCR_OK)
            {
                x_dbg_stmt("Error: _asf_dump_content_description:for pwsz_description "
                           "x_uc_w2s_to_ps failed(%d)\r\n", 
                           i4_ret);
                break; 
            }
            x_dbg_stmt("Description: %s\r\n", psz_buf);
            
        }
        /*rating info */
        if (NULL != pt_content_des->pwsz_rating)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          (pt_content_des->pwsz_rating) + 1;
            if (ui4_utf8_len > ui4_buf_len)
            {
                if (NULL != psz_buf)
                {
                    _mm_util_mem_free(psz_buf);
                }
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_utf8_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_content_description: failed "
                               "to allocate memory for for pwsz_rating\r\n");
                    return;
                }
                ui4_buf_len = ui4_utf8_len;
            }

            i4_ret = x_uc_w2s_to_ps(pt_content_des->pwsz_rating, 
                                    psz_buf, 
                                    ui4_buf_len);
            if(i4_ret != UCR_OK)
            {
                x_dbg_stmt("Error: _asf_dump_content_description:for pwsz_rating "
                           "x_uc_w2s_to_ps failed(%d)\r\n", 
                           i4_ret);
                break; 
            }
            x_dbg_stmt("Rating: %s\r\n", psz_buf);
            
        }        
    }while(0);

    if (NULL != psz_buf)
    {
        _mm_util_mem_free(psz_buf);
    }
    /*local variable need not set NULL*/
    return;
}

VOID _asf_dump_ext_content_description ( VOID* pv_specific )
{

    /*Content Description Object*/
    ASF_EXT_CONTENT_DES_OBJECT_T*     pt_ext_content_des;
    UINT32                            ui4_buf_len; 
    UINT32                            ui4_utf8_len; 

    CHAR*                             psz_buf = NULL; /*temporary buffer*/   
    INT32                             i4_ret;
    
    /**************************************************************
                     parameters check
      *************************************************************/
    if ( NULL == pv_specific )
    {
        x_dbg_stmt( "Error:%s#Line%d:NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }

    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    ui4_buf_len = 0;
    i4_ret      = MINFOR_OK;
        
    /*get extended content description object*/
    pt_ext_content_des = ( ASF_EXT_CONTENT_DES_OBJECT_T* )pv_specific;

    do 
    {
        /*year info */
        if ( NULL != pt_ext_content_des->pwsz_year )
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          ( pt_ext_content_des->pwsz_year ) + 1;
            if ( ui4_utf8_len > ui4_buf_len )
            {
                if ( NULL != psz_buf )
                {
                    _mm_util_mem_free( psz_buf );
                    psz_buf = NULL;
                }
                ui4_buf_len = ( ui4_utf8_len > 128 )? ui4_utf8_len : 128;
                psz_buf = ( CHAR* )_mm_util_mem_alloc( ui4_buf_len );
                if ( NULL == psz_buf )
                {
                    x_dbg_stmt( "Error: %s#Line%d: failed to allocate memory "
                                "for for pwsz_year\r\n", __FUNCTION__, __LINE__ );
                    i4_ret = MINFOR_OUT_OF_MEMORY;
                    break;
                }
                x_memset( psz_buf, 0, ui4_buf_len );
                
            }

            i4_ret = x_uc_w2s_to_ps( pt_ext_content_des->pwsz_year, 
                                     psz_buf, 
                                     ui4_buf_len );
            if ( i4_ret != UCR_OK )
            {
                x_dbg_stmt( "Error:  %s#Line%d:x_uc_w2s_to_ps for pwsz_year failed(%d)\r\n",
                            __FUNCTION__, __LINE__,
                            i4_ret );
                break; 
            }
            x_dbg_stmt( "Year: %s\r\n", psz_buf );
            
        } 

        /*album title info */
        if ( NULL != pt_ext_content_des->pwsz_album_title )
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          ( pt_ext_content_des->pwsz_album_title ) + 1;
            if ( ui4_utf8_len > ui4_buf_len )
            {
                if ( NULL != psz_buf )
                {
                    _mm_util_mem_free(psz_buf);
                    psz_buf = NULL;
                }
                ui4_buf_len = ( ui4_utf8_len > 128 )? ui4_utf8_len : 128;
                psz_buf = ( CHAR* )_mm_util_mem_alloc( ui4_buf_len );
                if ( NULL == psz_buf )
                {
                    x_dbg_stmt( "Error: %s#Line%d: failed to allocate memory "
                                "for for pwsz_album_title\r\n", __FUNCTION__, __LINE__ );
                    i4_ret = MINFOR_OUT_OF_MEMORY;
                    break;
                }
                x_memset( psz_buf, 0, ui4_buf_len );
                
            }

            i4_ret = x_uc_w2s_to_ps( pt_ext_content_des->pwsz_album_title, 
                                     psz_buf, 
                                     ui4_buf_len );
            if (i4_ret != UCR_OK)
            {
                x_dbg_stmt( "Error:  %s#Line%d:x_uc_w2s_to_ps for pwsz_album_title failed(%d)\r\n",
                            __FUNCTION__, __LINE__,
                            i4_ret );
                break; 
            }
            x_dbg_stmt( "Album Title: %s\r\n", psz_buf );
            
        }
        
        /*album artist info */
        if (NULL != pt_ext_content_des->pwsz_album_artist)
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          ( pt_ext_content_des->pwsz_album_artist ) + 1;
            if ( ui4_utf8_len > ui4_buf_len )
            {
                if ( NULL != psz_buf )
                {
                    _mm_util_mem_free( psz_buf );
                    psz_buf = NULL;
                }
                ui4_buf_len = ( ui4_utf8_len > 128 )? ui4_utf8_len : 128;
                psz_buf = ( CHAR* )_mm_util_mem_alloc( ui4_buf_len );
                if ( NULL == psz_buf )
                {
                    x_dbg_stmt( "Error: %s#Line%d: failed to allocate memory "
                                "for for pwsz_album_artist\r\n", __FUNCTION__, __LINE__ );
                    i4_ret = MINFOR_OUT_OF_MEMORY;
                    break;
                    
                }
                x_memset( psz_buf, 0, ui4_buf_len );
            }

            i4_ret = x_uc_w2s_to_ps( pt_ext_content_des->pwsz_album_artist, 
                                     psz_buf, 
                                     ui4_buf_len);
            if ( i4_ret != UCR_OK )
            {
                x_dbg_stmt( "Error:  %s#Line%d:x_uc_w2s_to_ps for pwsz_album_artist failed(%d)\r\n",
                            __FUNCTION__, __LINE__,
                            i4_ret );
                break; 
            }
            x_dbg_stmt( "Album Artist: %s\r\n", psz_buf );
            
        }
        
        /*genre info */
        if ( NULL != pt_ext_content_des->pwsz_genre )
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          ( pt_ext_content_des->pwsz_genre ) + 1;
            if ( ui4_utf8_len > ui4_buf_len )
            {
                if ( NULL != psz_buf )
                {
                    _mm_util_mem_free( psz_buf );
                    psz_buf = NULL;
                }
                ui4_buf_len = ( ui4_utf8_len > 128 )? ui4_utf8_len : 128;
                psz_buf = (CHAR*)_mm_util_mem_alloc( ui4_buf_len );
                if ( NULL == psz_buf )
                {
                    x_dbg_stmt( "Error: %s#Line%d: failed to allocate memory "
                                "for for pwsz_genre\r\n", __FUNCTION__, __LINE__ );
                    i4_ret = MINFOR_OUT_OF_MEMORY;
                    break;
                    
                }
                x_memset( psz_buf, 0, ui4_buf_len );
            }

            i4_ret = x_uc_w2s_to_ps( pt_ext_content_des->pwsz_genre, 
                                     psz_buf, 
                                     ui4_buf_len );
            if ( i4_ret != UCR_OK )
            {
                x_dbg_stmt( "Error:  %s#Line%d:x_uc_w2s_to_ps for pwsz_genre failed(%d)\r\n",
                            __FUNCTION__, __LINE__,
                            i4_ret );
                break; 
            }
            x_dbg_stmt( "Genre: %s\r\n", psz_buf );
            
        }  

        /*3D*/
        x_dbg_stmt( "Stereoscopic : %s\r\n",
                    ( pt_ext_content_des->b_stereo_scopic )? "TRUE" : "FALSE" );
        x_dbg_stmt( "StereoscopicHalfWidth : %s\r\n", 
                    ( pt_ext_content_des->b_half_width )? "TRUE" : "FALSE" );
        x_dbg_stmt( "StereoscopicHalfHeight : %s\r\n", 
                    ( pt_ext_content_des->b_half_height )? "TRUE" : "FALSE" );
        x_dbg_stmt( "StereoscopicSkip: %llu\r\n", pt_ext_content_des->ui8_skip_dur );

        if ( pt_ext_content_des->pwsz_stereoscopic_layout )
        {
            ui4_utf8_len = x_uc_w2s_strlen
                          ( pt_ext_content_des->pwsz_stereoscopic_layout ) + 1;
            if ( ui4_utf8_len > ui4_buf_len )
            {
                if ( NULL != psz_buf )
                {
                    _mm_util_mem_free( psz_buf );
                    psz_buf = NULL;
                }
                ui4_buf_len = ( ui4_utf8_len > 128 )? ui4_utf8_len : 128;
                psz_buf = (CHAR*)_mm_util_mem_alloc( ui4_buf_len );
                if ( NULL == psz_buf )
                {
                    x_dbg_stmt( "Error: %s#Line%d: failed to allocate memory "
                                "for for pwsz_stereoscopic_layout \r\n",
                                __FUNCTION__, __LINE__ );
                    i4_ret = MINFOR_OUT_OF_MEMORY;
                    break;
                    
                }
                x_memset( psz_buf, 0, ui4_buf_len );
            }

            i4_ret = x_uc_w2s_to_ps( pt_ext_content_des->pwsz_stereoscopic_layout, 
                                     psz_buf, 
                                     ui4_buf_len );
            if ( i4_ret != UCR_OK )
            {
                x_dbg_stmt( "Error:  %s#Line%d:x_uc_w2s_to_ps for pwsz_stereoscopic_layout failed(%d)\r\n",
                            __FUNCTION__, __LINE__,
                            i4_ret );
                break; 
            }
            
            x_dbg_stmt( "StereoscopicLayout: %s\r\n", psz_buf );
            
        }
        
    }while(0);

    if ( NULL != psz_buf )
    {
        _mm_util_mem_free( psz_buf );
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_bitrate_properties
 *
 * Description: This function dump  the bitrate properties object info
 *
 * Inputs:  pv_specific: point to a bitrate properties object structure
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
 *  (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_dump_bitrate_properties ( VOID* pv_specific )
{
    /*Biterate Properties Object*/
    ASF_BITRATE_PROPERTIES_OBJECT_T*  pt_bitrate_properties;
    ASF_BITRATE_REC_T*                pt_bitrate_record;
    
    UINT32                            ui4_idx;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_bitrate_properties:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_bitrate_properties:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    
    /*get bitrate properties object*/
    pt_bitrate_properties = (ASF_BITRATE_PROPERTIES_OBJECT_T*)pv_specific;
    if ((pt_bitrate_properties->ui2_rec_count > 0) 
         && (NULL == pt_bitrate_properties->pt_record))
    {
        /*DBG_ERROR(("_asf_dump_bitrate_properties:Exception!!!\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_bitrate_properties:Exception!!!\r\n");
    }
    else
    {
        /*get record count */
        for (ui4_idx = 0; ui4_idx < pt_bitrate_properties->ui2_rec_count; ++ui4_idx)
        {
            
            pt_bitrate_record = pt_bitrate_properties->pt_record + ui4_idx;
            /*ASSERT(pt_bitrate_record != NULL);*/
            /*Stream number , Bitrate*/
            x_dbg_stmt("Stream Number: %d Bitrate is %d\r\n", 
                      pt_bitrate_record->ui1_stream_num,
                      pt_bitrate_record->ui4_bitrate);
        }
    }

    return;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_content_encryption
 *
 * Description: This function dump  the content encryption object info
 *
 * Inputs:  pv_specific: point to a content encryption object structure
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
 *  (1)2008-10-21 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_dump_content_encryption( VOID* pv_specific )
{
    /*Content Encryption Object*/
    ASF_CONTENT_ENCRYPTION_OBJECT_T*  pt_content_encryption;
    UINT32                            ui4_idx;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_bitrate_properties:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_content_encryption:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    
    /*get content encryption object*/
    pt_content_encryption = (ASF_CONTENT_ENCRYPTION_OBJECT_T*)pv_specific;

    /*secret data*/
    x_dbg_stmt("Secret Data:");
    for (ui4_idx = 0; ui4_idx < pt_content_encryption->ui4_secret_data_len; ++ui4_idx)
    {
        if (0 == ui4_idx % 16)
        {
           x_dbg_stmt("\r\n");
        }
        x_dbg_stmt("%02x ", pt_content_encryption->pui1_secret_data[ui4_idx]);
    }
    x_dbg_stmt("\r\n");

    /*Protect type*/
    x_dbg_stmt("Protect Type:%s\r\n", pt_content_encryption->pi1_protect_type);
    
    /*Protect type*/
    x_dbg_stmt("Key ID:%s\r\n", pt_content_encryption->pi1_key_id);
    
    /*Protect type*/
    x_dbg_stmt("License URL:%s\r\n", pt_content_encryption->pi1_lic_url);
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_codec_list
 *
 * Description: This function dump  the codec list object info
 *
 * Inputs:  pv_specific: point to a codec list object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/
VOID  _asf_dump_codec_list ( VOID* pv_specific )
{
    /*Codec List Object*/
    ASF_CODEC_LIST_OBJECT_T* pt_codec_list;
    UINT32                   ui4_i;    /*loop variable*/
    UINT32                   ui4_j;    /*loop variable*/
    UINT32                   ui4_type; /*codec type*/
    
    INT32                    i4_ret; /*return code*/
    CHAR*                    asz_codec_type[] = {
                                                   "Video Codec",
                                                   "Audio Codec"
                                                   "Unknown Codec"
                                                };
    CHAR*                    psz_buf;          /*temporary buffer*/          
    UINT32                   ui4_buf_len;    /*temporary buffer length*/ 

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_codec_list:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_codec_list:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    ui4_i = 0;          /*loop variable*/
    ui4_j = 0;          /*loop variable*/
    ui4_type = 0;       /*codec type*/
    i4_ret = 0;         /*return code*/
    psz_buf  = NULL;    /*temporary buffer*/          
    ui4_buf_len = 0;    /*temporary buffer length*/ 
    
    /*get codec list object*/
    pt_codec_list = (ASF_CODEC_LIST_OBJECT_T *)pv_specific;
    /*get codec count*/
    x_dbg_stmt("Codec Count: %u\r\n", 
              pt_codec_list->ui4_codec_entry_count);

    if (pt_codec_list->ui4_codec_entry_count != 0)
    {
        if (NULL != pt_codec_list->pt_codec)
        {

            /*get codec list*/
            for (ui4_i = 0; ui4_i < pt_codec_list->ui4_codec_entry_count; ++ui4_i)
            {
                /*Get codec type, 2 bytes*/
                ui4_type = pt_codec_list->pt_codec[ui4_i].ui2_type;
                if (ASF_CODEC_UNKNOW == ui4_type)
                {
                    ui4_type = 2 ;
                }
                if (ui4_type < 3)
                {
                    x_dbg_stmt("Codec Type: %s\r\n", 
                              asz_codec_type[ui4_type]);
                }

                /*Get codec name */
                if (NULL != pt_codec_list->pt_codec[ui4_i].pwsz_name)
                {
                
                    ui4_buf_len = x_uc_w2s_strlen
                                (pt_codec_list->pt_codec[ui4_i].pwsz_name) + 1;
                    psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_buf_len);
                    if(NULL == psz_buf)
                    {
                        x_dbg_stmt("Error: _asf_dump_codec_list:for pwsz_name failed "
                                   "to allocate memory for psz_buf\r\n");
                        return;
                    }

                    i4_ret = x_uc_w2s_to_ps(pt_codec_list->pt_codec[ui4_i].pwsz_name, 
                                            psz_buf, ui4_buf_len);
                    if(i4_ret != UCR_OK)
                    {
                        _mm_util_mem_free(psz_buf);
                        psz_buf = NULL;
                        x_dbg_stmt("Error: _asf_dump_codec_list:for pwsz_name "
                                   "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret);
                        return ;
                    }
                    x_dbg_stmt("Codec Name: %s\r\n", psz_buf);
                    
                    /*free memeory*/
                    _mm_util_mem_free(psz_buf);
                    psz_buf = NULL;
                }
                
                /*Get codec description 2 */
                if (NULL != pt_codec_list->pt_codec[ui4_i].pwsz_desc)
                {
                    ui4_buf_len = x_uc_w2s_strlen
                                (pt_codec_list->pt_codec[ui4_i].pwsz_desc) + 1;
                    psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_buf_len);
                    if(NULL == psz_buf)
                    {
                        x_dbg_stmt("Error: _asf_dump_codec_list:for pwsz_desc failed "
                                   "to allocate memory for psz_buf\r\n");
                        return;
                    }

                    i4_ret = x_uc_w2s_to_ps(pt_codec_list->pt_codec[ui4_i].pwsz_desc, 
                                            psz_buf, ui4_buf_len);
                    if(i4_ret != UCR_OK)
                    {
                        _mm_util_mem_free(psz_buf);
                        psz_buf = NULL;
                        x_dbg_stmt("Error: _asf_dump_codec_list:for pwsz_desc "
                                   "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret);
                        return ;
                    }
                    x_dbg_stmt("Codec Description: %s\r\n", psz_buf);
                    
                    /*free memeory*/
                    _mm_util_mem_free(psz_buf);
                    psz_buf = NULL;
                }

                /*Get codec info*/
                if (0 !=  pt_codec_list->pt_codec[ui4_i].ui2_info_len)
                {
                    for (ui4_j = 0; 
                         ui4_j < pt_codec_list->pt_codec[ui4_i].ui2_info_len; 
                         ++ui4_j)
                    {
                        x_dbg_stmt("Codec Info: %02u\r\n", 
                                  pt_codec_list->pt_codec[ui4_i].pui1_info[ui4_j]);
                    }
                }
            
            }        
        }

    }

    return;
}



/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_ex_stream_properties
 *
 * Description: This function dump  the extended stream object info
 *
 * Inputs:  pv_specific: point to a extended stream properties object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/

VOID  _asf_dump_ex_stream_properties ( VOID* pv_specific )
{
    /* Extended Stream Object*/
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_object;
    ASF_STREAM_NAME_T*                 pt_stream_name;
    ASF_PAYLOAD_EX_SYS_T*              pt_payload_ex_system;
    
    /*loop variable*/
    UINT32                              ui4_idx;
    /*temporary buffer*/
    CHAR*                               psz_buf;
    /*temporary buffer length*/
    UINT32                              ui4_buf_len;
    /*return code*/
    INT32                               i4_ret;
    /*GUID str temp buffer*/
    CHAR                                ac_temp[40];
    /*GUID str temp buffer length*/
    UINT32                              ui4_len;
    /*loop variable*/
    UINT32                              ui4_i;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        x_dbg_stmt("Error: _asf_dump_ex_stream_properties:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    ui4_idx = 0;              /*loop variable*/
    psz_buf = NULL;           /*temporary buffer*/
    ui4_buf_len = 0;          /*temporary buffer length*/
    i4_ret = 0;               /*return code*/
    x_memset(ac_temp, 0, 40); /*GUID str temp buffer*/
    ui4_len = 40;             /*GUID str temp buffer length*/
    ui4_i = 0;                /*loop variable*/

    /*get extended stream properties object*/
    pt_ex_stream_object = (ASF_EX_STREAM_PROPERTIES_OBJECT_T *)pv_specific;

    /*get start time, 8 bytes*/
    x_dbg_stmt("Start Time: %llu\r\n", 
              pt_ex_stream_object->ui8_start_time);

    /*get end time, 8 bytes*/
    x_dbg_stmt("End Time: %llu\r\n", 
              pt_ex_stream_object->ui8_end_time);

    /*get data bitrate, 4 bytes*/
    x_dbg_stmt("Bit Rate: %u\r\n", 
              pt_ex_stream_object->ui4_bitrate);

    /*get buffer size, 4 bytes*/
    x_dbg_stmt("Buffer Size: %u\r\n", 
              pt_ex_stream_object->ui4_buf_size);
    
    /*get initial buffer size, 4 bytes*/
    x_dbg_stmt("Initial Buffer Size: %u\r\n", 
              pt_ex_stream_object->ui4_init_buf_size);

    /*get alter data bitrate, 4 bytes*/
    x_dbg_stmt("Alter Bit Rate: %u\r\n", 
              pt_ex_stream_object->ui4_alter_bitrate);

    /*get alter buffer size, 4 bytes*/
    x_dbg_stmt("Alter Buffer Size: %u\r\n", 
              pt_ex_stream_object->ui4_alter_buf_size);

    /*get alter initial buffer size, 4 bytes*/
    x_dbg_stmt("Alter Initial Buffer Size: %u\r\n", 
              pt_ex_stream_object->ui4_alter_init_buf_size);

    /*get maximum object size, 4 bytes*/
    x_dbg_stmt("Maximum Object Size: %u\r\n", 
              pt_ex_stream_object->ui4_max_obj_size);

    /*get flags, 4 bytes*/
    x_dbg_stmt("Flags: %u\r\n", 
              pt_ex_stream_object->ui4_flags);

    /*get stream number, 2 bytes*/
    x_dbg_stmt("Stream Number: %u\r\n", 
              pt_ex_stream_object->ui2_stream_num);

    /*get stream language ID index, 2 bytes*/
    x_dbg_stmt("Language ID: %u\r\n", 
              pt_ex_stream_object->ui2_language_idx);

    /*get average time per frame , 8 bytes*/
    x_dbg_stmt("Average Time Per Frame: %llu\r\n", 
              pt_ex_stream_object->ui8_average_time_per_frame);

    /*get stream name counts, 2 bytes*/
    x_dbg_stmt("Stream Name Count: %u\r\n", 
              pt_ex_stream_object->ui2_stream_name_count);

    /*get names*/
    if (pt_ex_stream_object->pt_stream_name != NULL)
    {
        for (ui4_idx = 0; ui4_idx < pt_ex_stream_object->ui2_stream_name_count; 
             ++ui4_idx)
        {
            pt_stream_name = pt_ex_stream_object->pt_stream_name + ui4_idx;
            if (NULL == pt_stream_name)
            {
                x_dbg_stmt("Error: _asf_dump_ex_stream_properties: stream[%u], "
                           "stream_name[%u] is NULL\r\n",
                           pt_ex_stream_object->ui2_stream_num,
                           ui4_idx);
                continue;
            }
            
            /*Get name language ID*/
            x_dbg_stmt("Stream Name[%d] Language ID: %u\r\n", 
                      ui4_idx, 
                      (pt_stream_name->ui2_strm_name_lang_idx));

            if (pt_stream_name->pwsz_stream_name 
                != NULL)
            {
                ui4_buf_len = x_uc_w2s_strlen
                             (pt_stream_name->pwsz_stream_name) 
                              + 1;
                psz_buf = (CHAR*)_mm_util_mem_alloc(ui4_buf_len);
                if(NULL == psz_buf)
                {
                    x_dbg_stmt("Error: _asf_dump_ex_stream_properties: failed to "
                               "allocate memory for pwsz_stream_name\r\n");
                    return;
                }

                i4_ret = x_uc_w2s_to_ps(pt_stream_name->pwsz_stream_name, 
                                        psz_buf, ui4_buf_len);
                if(i4_ret != UCR_OK)
                {
                    _mm_util_mem_free(psz_buf);
                    psz_buf = NULL;
                    x_dbg_stmt("Error: _asf_dump_ex_stream_properties: x_uc_w2s_to_ps"
                               " failed(%d)\r\n", i4_ret);
                    return ;
                }
                
                x_dbg_stmt("Stream Name[%u]: %s\r\n", 
                                       ui4_idx, 
                                       psz_buf);
                
                /*free memeory*/
                _mm_util_mem_free(psz_buf);
                psz_buf = NULL;

            }
            else
            {
                x_dbg_stmt("Stream Name[%u]: NULL\r\n", ui4_idx);
            }
  
        }
    }

    /*get payload system counts, 2 bytes*/
    x_dbg_stmt("Payload Extentison System Count: %u\r\n", 
              pt_ex_stream_object->ui2_payload_ex_sys_count);

    /*get payload system*/
    if (pt_ex_stream_object->pt_payload_ex_system != NULL)
    {
        for (ui4_idx = 0; 
             ui4_idx < pt_ex_stream_object->ui2_payload_ex_sys_count; 
             ++ui4_idx)
        {

            pt_payload_ex_system = pt_ex_stream_object->pt_payload_ex_system 
                                   + ui4_idx;
            if (NULL == pt_payload_ex_system)
            {
                x_dbg_stmt("Error: _asf_dump_ex_stream_properties: stream[%u], "
                           "payload_ex_system[%u] is NULL\r\n",
                           pt_ex_stream_object->ui2_stream_num,
                           ui4_idx);
                continue;
            }
            /*get GUID , 16 bytes*/
            _asf_guid_to_str(&(pt_payload_ex_system->t_guid), 
                            ac_temp, &ui4_len);
            x_dbg_stmt("Extension System ID:%s\r\n", ac_temp);

            /*get object size*/
            x_dbg_stmt("Object Size:%d\r\n", 
                      pt_payload_ex_system->ui2_size);

            /*get info length*/
            x_dbg_stmt("Extension System Info Length:%u\r\n", 
                      pt_payload_ex_system->ui4_ex_sys_info_len);
            
            if (pt_payload_ex_system->ui4_ex_sys_info_len != 0)
            {
                if (pt_payload_ex_system->pui1_ex_sys_info 
                    != NULL)
                {
                    x_dbg_stmt("Extension System Info:"); 

                    for (ui4_i = 0; 
                         ui4_i < pt_payload_ex_system->ui4_ex_sys_info_len; 
                         ++ui4_i)
                    {
                        x_dbg_stmt("%02u  ", 
                                  pt_payload_ex_system->pui1_ex_sys_info[ui4_i]);
                    }
                    x_dbg_stmt("\r\n"); 
                    
                }
                else
                {
                    x_dbg_stmt("Extension System Info: NULL\r\n");               
                }
            }
            
        }
    }
    else
    {
        x_dbg_stmt("Payload Extentison Systems: NULL");                
    }

    return;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_index_parameters
 *
 * Description: This function dump  the index parameters object info
 *
 * Inputs:  pv_specific: point to a index parameters object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/

VOID  _asf_dump_index_parameters( VOID* pv_specific )
{
    /*Index Parameters Object*/
    ASF_IDX_PARAMETERS_OBJECT_T* pt_idx_paras ;
    /*loop variable*/
    UINT32                       ui4_i;
    /*index type description*/
    CHAR*                        asz_index_type[] = {
                                                       "NEAR PAST PACKET",
                                                       "NEAR PAST MEDIA OBJECT",
                                                       "NEAR PAST TIME CODE"
                                                    };

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_index_parameters:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_index_parameters:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*init variables*/
    ui4_i = 0; /*loop variable*/
    
    /*get Index Parameters object*/
    pt_idx_paras = (ASF_IDX_PARAMETERS_OBJECT_T *)pv_specific;

    /*get time interval, 4 bytes*/
    x_dbg_stmt("Time Interval: %u\r\n", 
              pt_idx_paras->ui4_time_interval);

    /*get specifers count, 2 bytes*/
    x_dbg_stmt("Specifers Count: %u\r\n", 
              pt_idx_paras->ui2_specifers_count);

    if (pt_idx_paras->ui2_specifers_count != 0)
    {
        /*get specifers */
        for (ui4_i = 0; ui4_i < pt_idx_paras->ui2_specifers_count; ++ui4_i)
        {
            /*Get stream number, 2 bytes*/
            x_dbg_stmt("Stream Number: %u\r\n", 
                      pt_idx_paras->pt_specifers[ui4_i].ui2_stream_num);

            /*get index type, 2 bytes*/
            if ((pt_idx_paras->pt_specifers[ui4_i].ui2_idx_type - 1) < 3)
            {
                x_dbg_stmt("Stream Type: %s\r\n", 
                          asz_index_type[pt_idx_paras->pt_specifers
                          [ui4_i].ui2_idx_type - 1]);
            }
        }
    }
    else
    {
        x_dbg_stmt("Specifers : NULL\r\n");
    }
    
    return;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_header_extention
 *
 * Description: This function dump  the header extention object info
 *
 * Inputs:  pv_specific: point to a header extention object structure
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
 *  (1)2008-4-1 : initial

 ----------------------------------------------------------------------------*/

VOID  _asf_dump_header_extention ( VOID* pv_specific )
{
    /*header extention object*/
    ASF_HEADER_EX_OBJECT_T* pt_header_ex;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_header_extention:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_header_extention:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*get header extention object*/
    pt_header_ex = (ASF_HEADER_EX_OBJECT_T *)pv_specific;
    /*get extention header data length*/
    x_dbg_stmt("Extension Header Length: %u\r\n", 
              pt_header_ex->ui4_size);

    return;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_header_object
 *
 * Description: This function free header object resources
 *
 * Inputs:  pv_specific: point to a header object structure
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
 *  (1)2008-4-1 : initial
           
 ----------------------------------------------------------------------------*/

VOID  _asf_dump_header_object( VOID* pv_specific )

{
    /*header object*/
    ASF_HEADER_OBJECT_T* pt_header_object;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_header_object:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_header_object:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*get header object*/
    pt_header_object = (ASF_HEADER_OBJECT_T *)pv_specific;
    /*get extention header data length*/
    x_dbg_stmt("Sub Object Count: %u\r\n", 
              pt_header_object->ui4_sub_object_count);
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_data_object
 *
 * Description: This function dump a  data object infosa
 *
 * Inputs:  pv_specific: point to a data object structure
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
 *  (1)2008-4-1 : initial
           
 ----------------------------------------------------------------------------*/

VOID  _asf_dump_data_object( VOID* pv_specific )

{
    
    ASF_DATA_OBJECT_T* pt_data_object;/*data object*/
    CHAR               ac_temp[40];   /*GUID str temp buffer*/
    UINT32             ui4_len;       /*GUID str temp buffer length*/

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pv_specific)
    {
        /*DBG_ERROR(("_asf_dump_data_object:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_data_object:NULL\r\n");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    /*init vaiables*/
    x_memset(ac_temp, 0, 40);
    /*GUID str temp buffer length*/
    ui4_len = 40;

    /*get data object*/
    pt_data_object = (ASF_DATA_OBJECT_T *)pv_specific;

    /*get File guid*/
    _asf_guid_to_str(&(pt_data_object->t_file_id), ac_temp, &ui4_len);
    x_dbg_stmt("File ID:%s\r\n", ac_temp);
    
    /*Get total packet counts, 8 bytes*/
    x_dbg_stmt("Total Packet Count:%llu\r\n", 
              pt_data_object->ui8_total_packets);

    /*Get reserved, 2 bytes*/
    x_dbg_stmt("Reserved:%u\r\n", 
              pt_data_object->ui2_reserved);
   
    /*Get data begin position, 2 bytes*/
    x_dbg_stmt("Data Packet Position:%llu\r\n", 
              pt_data_object->ui8_data_packet_begin);

    return;
}


/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_object
 *
 * Description: This function dispaly object info e.g. size/offset/guid/type
 *
 * Inputs:  pt_this_obj point to a structure which stores object common info
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * NOTE: 
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-17 : initial
        
----------------------------------------------------------------------------*/
VOID  _asf_dump_object( ASF_OBJ_COMMON_T* pt_this_obj)
{
    
    UINT32     ui4_idx;          /*loop variable*/
    UINT32     ui4_minfo_tab_len;/*temp variable*/
    CHAR       ac_temp[40];      /*GUID str temp buffer*/
    UINT32     ui4_len;          /*GUID str temp buffer length*/
    
    
    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pt_this_obj)
    {
        /*DBG_ERROR(("_asf_dump_object:NULL\r\n "));*/
        x_dbg_stmt("Error: _asf_dump_object:NULL\r\n ");
        return;
    }
    
    /**************************************************************
                            Do
    **************************************************************/

    if (FALSE == _minfo_cli_get_dump_flag())
    {
        return;
    }
    
    x_dbg_stmt(" _asf_dump_object:begin dump object\r\n ");

    /*init vaiables*/
    x_memset(ac_temp, 0, 40);
    /*GUID str temp buffer length*/
    ui4_len = 40;

    /*get minfo oper tabl length*/
    ui4_minfo_tab_len = _asf_get_minfo_oper_tab_len();
    
    for (ui4_idx = 0; ui4_idx < ui4_minfo_tab_len; ++ui4_idx)
    {
        if (pt_this_obj->e_type == g_at_asf_obj_oper[ui4_idx].e_obj_type)
        {
            break;
        }
    }
    if (ui4_idx >= ui4_minfo_tab_len)
    {
        ui4_idx = ASF_OBJ_UNKNOWN;
    }

    /*display common info: type/guid/offset/size*/
    if (ASF_OBJ_STREAM_PROPERTIES == pt_this_obj->e_type)
    {
        
        x_dbg_stmt("\r\nType:\t%s[%u]\r\n", 
                  g_at_asf_obj_oper[ui4_idx].psz_description,
                  ((ASF_STREAM_PROPERTIES_OBJECT_T *)
                   (pt_this_obj->pv_obj_specific))->ui1_stream_number);
    }
    else if(ASF_OBJ_EX_STREAM_PROPERTIES == pt_this_obj->e_type)
    {
        x_dbg_stmt("\r\nType:\t%s[%u]\r\n", 
                  g_at_asf_obj_oper[ui4_idx].psz_description,
                  ((ASF_EX_STREAM_PROPERTIES_OBJECT_T *)
                   (pt_this_obj->pv_obj_specific))->ui2_stream_num);
    }
    else
    {
        x_dbg_stmt("\r\nType:\t%s\r\n", 
                  g_at_asf_obj_oper[ui4_idx].psz_description);
    }


    _asf_guid_to_str(&(pt_this_obj->t_guid), ac_temp, &ui4_len);
    x_dbg_stmt("Object ID:%s\r\n", ac_temp);

    x_dbg_stmt("Object Size:\t%llu\r\n",  pt_this_obj->ui8_size);
    x_dbg_stmt("File Position:\t%llu\r\n",  pt_this_obj->ui8_offset);

    /*if specific analysed, show it*/
    if ((NULL != pt_this_obj->pv_obj_specific) 
         && (NULL != g_at_asf_obj_oper[ui4_idx].pf_dump_obj))
    {
        g_at_asf_obj_oper[ui4_idx].pf_dump_obj
            ((VOID *)(pt_this_obj->pv_obj_specific));
    }
   
    x_dbg_stmt("_asf_dump_object:end dumping object\r\n ");
 
    return;
}

/*-----------------------------------------------------------------------------
 * Name: _asf_current_supported_object
 *
 * Description: show the current supported objects
 *
 * Inputs:  NULL 
 *          
 * Outputs: NULL 
 * 
 * Returns: NULL
 *
 * Note:  
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
VOID _asf_current_supported_object(VOID)
{
    UINT32   ui4_i;
    UINT32   ui4_len;
    
    /**************************************************************
                     parameters check
    *************************************************************/

    /**************************************************************
                            Do
    **************************************************************/
    x_dbg_stmt("Supported Objects:\r\n");

    ui4_len = _asf_get_minfo_oper_tab_len();
    for (ui4_i = 0; ui4_i < ui4_len; ++ui4_i)
    {
        if ((ASF_OBJ_UNKNOWN != g_at_asf_obj_oper[ui4_i].e_obj_type)
            && (ASF_OBJECT_BUTT != g_at_asf_obj_oper[ui4_i].e_obj_type))
        {
            x_dbg_stmt(" %s ID : %d\r\n", 
                      g_at_asf_obj_oper[ui4_i].psz_description, 
                      g_at_asf_obj_oper[ui4_i].e_obj_type);
        }
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_dump_composite_object
 *
 * Description: This function dumps composite object,e.g. header object
                /header extension/extended stream properties object
 *
 * Inputs:   pt_this_obj point to a structure which stores object common info
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * NOTE: 
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
    (1)2008-3-17 : initial
        
----------------------------------------------------------------------------*/
VOID _asf_dump_composite_object( ASF_OBJ_COMMON_T* pt_this_obj)
{
    ASF_OBJ_COMMON_T* pt_sub_obj;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pt_this_obj)
    {
        /*DBG_ERROR(("_asf_dump_composite_object:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_dump_composite_object:NULL\r\n");
        return;
    }

    /**************************************************************
                            Do
    **************************************************************/
    /*first dump itself*/
    _asf_dump_object(pt_this_obj);

    /*dump sub objects*/
    pt_sub_obj = pt_this_obj->pt_first_child;
    while (NULL != pt_sub_obj)
    {
        if ((ASF_OBJ_HEADER == pt_sub_obj->e_type) 
        || (ASF_OBJ_HEADER_EXTENSION == pt_sub_obj->e_type)
        || (ASF_OBJ_EX_STREAM_PROPERTIES == pt_sub_obj->e_type))
        {
            _asf_dump_composite_object(pt_sub_obj);
        }
        else
        {
            _asf_dump_object(pt_sub_obj);
        }
        pt_sub_obj = pt_sub_obj->pt_next_sibling;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: _asf_find_node
 *
 * Description: search node in asf tree by the object ID(e_obj_type)
 *
 * Inputs:  pt_minfo_obj : specifies the Minfo object
            e_obj_type:    object ID
 *          
 * Outputs: NULL   
 * 
 * Returns: NULL
 *
 * Note:  
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/

VOID _asf_cli_dump_object( MINFO_OBJ_T*   pt_minfo_obj, 
                           ASF_OBJ_TYPE_E e_obj_type )
{
    /*store asf parser related info*/
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj; 
    ASF_OBJ_COMMON_T*           p_obj;

    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pt_minfo_obj)
    {
        /*DBG_ERROR(("_asf_cli_dump_object:NULL\r\n"));*/
        x_dbg_stmt("Error: _asf_cli_dump_object:NULL\r\n");
        return ;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    
    /*get asf instance handler*/
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*)pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        /*DBG_ERROR(("_asf_cli_dump_object:NULL pv_handler_obj in MINFO_OBJ_T\r\n"));*/
        x_dbg_stmt("Error: _asf_cli_dump_object:NULL pv_handler_obj in MINFO_OBJ_T\r\n");
        return ;
    }

    /*dump all objects*/
    if (ASF_OBJECT_BUTT == e_obj_type)
    {

        _asf_dump_composite_object(pt_asf_instance_obj->t_root.pt_header);

    
        _asf_dump_object(pt_asf_instance_obj->t_root.pt_data);

        x_dbg_stmt("Index info are not parsered\r\n");
        return;
    }
    
    /*dump data objects*/
    if (ASF_OBJ_DATA == e_obj_type)
    {
        _asf_dump_object(pt_asf_instance_obj->t_root.pt_data);
        return ;
    }
    
    /*dump simple index objects*/
    if (ASF_OBJ_SIMPLE_INDEX == e_obj_type)
    {
        x_dbg_stmt("Index info are not parsered\r\n");
        return ;
    }

    /*dump a header objects*/
    /*seach the first match*/
    p_obj = _asf_find_node(pt_asf_instance_obj->t_root.pt_header, 
                           e_obj_type);
    while (NULL != p_obj)
    {
        /*dump objects embody sub objects*/
        if ((ASF_OBJ_HEADER == e_obj_type) 
            || (ASF_OBJ_HEADER_EXTENSION == e_obj_type)
            || (ASF_OBJ_EX_STREAM_PROPERTIES == e_obj_type))
        {
           _asf_dump_composite_object(p_obj);
        }
        /*dump single object*/
        else
        {
            _asf_dump_object(p_obj);
        }

        /*To get other objects of the same type ID,next match*/
        p_obj = p_obj->pt_next_sibling;
        while (NULL != p_obj)
        {
            if (e_obj_type == p_obj->e_type)
            {
                break;
            }
        }
    }
    return;
}


/*-----------------------------------------------------------------------------
 * Name: _minfo_dump_minfo_rec
 *
 * Description: dump minfo rec info
 *
 * Inputs:  t_minfo_rec speifies minfo rec objet
 *
 * Outputs: NULL
 *
 * Returns: NULL
 ----------------------------------------------------------------------------*/
VOID  _minfo_dump_minfo_rec(MINFO_INFO_T* pt_minfo_rec)
{
    MINFO_INFO_TYPE_T            e_info_type;
    MINFO_TYPE_GENERIC_T*        pt_general_info;  
    MINFO_ASF_AUDIO_STM_ATRBT_T* pt_asf_audio_stm_attr;
    MINFO_ASF_VIDEO_STM_ATRBT_T* pt_asf_video_stm_attr;
    MINFO_TYPE_IDX_INFO_T*       pt_idx_info; 
    UINT32                       ui4_strm_id;
    UINT32                       ui4_i;
    
    if (NULL == pt_minfo_rec)
    {
        x_dbg_stmt("_minfo_dump_minfo_rec:NULL \r\n");
        return ;
    }
    
    e_info_type = pt_minfo_rec->e_info_type;
    switch (e_info_type)
    {
        case MINFO_INFO_TYPE_GENERIC:
            pt_general_info = &(pt_minfo_rec->u.t_general_info);
            x_dbg_stmt("Gneric:duration=%llu; "
                      " media_sz=%llu; strm_ns=%u\r\n",
                      pt_general_info->ui8_pb_duration,
                      pt_general_info->ui8_media_sz,
                      pt_general_info->ui1_strm_ns);
            break;
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            ui4_strm_id = pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_asf_stm_id;
            pt_asf_video_stm_attr = 
                &(pt_minfo_rec->u.t_stm_attribute.u.t_asf_video_stm_attr);
            x_dbg_stmt("Video:stream id=%u; enc=%d; height=%d; "
                      "width=%d; packet_sz=%u, codec_info_size=%u;"
                      "default=%d; wmv8_intra_supported=%s; code info:\r\n",
                      ui4_strm_id,
                      pt_asf_video_stm_attr->e_enc,
                      pt_asf_video_stm_attr->i4_height,
                      pt_asf_video_stm_attr->i4_width,
                      pt_asf_video_stm_attr->ui4_packet_sz,
                      pt_asf_video_stm_attr->ui4_codec_info_size,
                      pt_asf_video_stm_attr->b_default,
                      (pt_minfo_rec->u.t_stm_attribute.b_is_codec_support
                      ? "TRUE" : "FALSE"));
            
            for (ui4_i = 0; 
                 ui4_i < pt_asf_video_stm_attr->ui4_codec_info_size; 
                 ++ui4_i)
            {
                x_dbg_stmt("%d  ", pt_asf_video_stm_attr->pui1_codec_info[ui4_i]);
            }
            x_dbg_stmt("\r\n");
             
            break;
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            ui4_strm_id = pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_asf_stm_id;
            pt_asf_audio_stm_attr = 
                &(pt_minfo_rec->u.t_stm_attribute.u.t_asf_audio_stm_attr);
            x_dbg_stmt("Audio:stream id=%u; enc=%d; layer=%d; "
                      "bits_per_sample=%d; block_align=%d; "
                      "channels=%d; samples_per_sec=%d; "
                      "lang=%s; packet_sz=%u; default=%d\r\n",
                      ui4_strm_id,
                      pt_asf_audio_stm_attr->e_enc,
                      pt_asf_audio_stm_attr->e_layer,
                      pt_asf_audio_stm_attr->i2_bits_per_sample,
                      pt_asf_audio_stm_attr->i2_block_align,
                      pt_asf_audio_stm_attr->i2_channels,
                      pt_asf_audio_stm_attr->i4_samples_per_sec,
                      (CHAR*)&(pt_asf_audio_stm_attr->s_lang),
                      pt_asf_audio_stm_attr->ui4_packet_sz,
                      pt_asf_audio_stm_attr->b_default);
            
            break;
        case MINFO_INFO_TYPE_IDX_INFO:
            pt_idx_info = &(pt_minfo_rec->u.t_idx_info);
            x_dbg_stmt("Index Info:idx_type=%d; "
                      "data_offset=%llu; data_sz=%llu; "
                      "idx_offset=%llu; idx_sz=%llu\r\n",
                      pt_idx_info->ui4_idx_type,
                      pt_idx_info->ui8_data_offset,
                      pt_idx_info->ui8_data_sz,
                      pt_idx_info->ui8_idx_offset,
                      pt_idx_info->ui8_idx_sz);
            break;
        default:
             x_dbg_stmt("_minfo_dump_minfo_rec:unknown\r\n");
             break;
            
    }
    
    return ;
}

#endif   /*if define CLI_LVL_ALL*/

/*-----------------------------------------------------------------------------
 * Name: _asf_find_node
 *
 * Description: search node in asf tree by the object ID(e_obj_type)
 *
 * Inputs:  pt_obj : the root node 
 *          
 * Outputs: NULL   
 * 
 * Returns: the node pointer
 *
 * Note:  
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/

ASF_OBJ_COMMON_T* _asf_find_node( ASF_OBJ_COMMON_T* pt_obj, 
                                  ASF_OBJ_TYPE_E    e_obj_type )
{
    ASF_OBJ_COMMON_T* pt_temp_obj;

    
    /**************************************************************
                     parameters check
    *************************************************************/
    if (NULL == pt_obj)
    {
#ifdef CLI_LVL_ALL
        x_dbg_stmt("Error: _asf_find_node:NULL\r\n");
#endif
        return NULL;
    }
    
    /**************************************************************
                            Do
    **************************************************************/
    
    if (e_obj_type == pt_obj->e_type)
    {
        return pt_obj;
    }
    
    pt_temp_obj = _asf_find_node(pt_obj->pt_first_child, e_obj_type);
    if (NULL != pt_temp_obj)
    {
        return pt_temp_obj;
    }
    return _asf_find_node(pt_obj->pt_next_sibling, e_obj_type);
}



#ifdef __cplusplus
}
#endif
