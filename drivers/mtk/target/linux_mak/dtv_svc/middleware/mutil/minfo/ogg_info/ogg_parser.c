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
 * ogg_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2013/01/04 $ 2004/11/26
 * $Author: pan.huang $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the ogg parser function
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
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/mm_util.h"
#include "dbg/u_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "unicode/x_uc_str.h"
#include "ogg_parser.h"

#else

#include "x_os.h"

#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "../../mm_util.h"
#include "u_dbg.h"
#include "x_uc_str.h"
#include "ogg_parser.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*
#define dwBigFourCC(a,b,c,d)    ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
*/

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/

/*#define OGG_READ_HEAD_LEN                1000*/
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
#define HX_GET_MAJOR_VERSION(prodVer) ((prodVer >> 28) & 0xF)
#define HX_GET_MINOR_VERSION(prodVer) ((prodVer >> 20) & 0xFF)
#define OGG_PAGE_HEADER        dwBigFourCC('O','g','g','S') 
#define OGG_MAX_HEAD_LEN                300
#define OGG_FILE_PARSE_MAX_SIZE            (10*1024*1024)  /*below this value, ogg file will be parsed completely*/
#define BIG_FILE_PARSE_PAGE_MAX_NUM        120                /*big file will parse the first several pages*/
#define LAST_64K_DATA_LENGTH            64*1024            /*last 64k bytes*/        


/*
#define LITTLE_TO_BIG_WORD(ui2Value)    (((ui2Value&0x00FF)<< 8)|         \
                                        ((ui2Value&0xFF00)>>8))
#define LITTLE_TO_BIG_DWORD(ui4Value)   (((ui4Value&0x000000FF)<<24)|    \
                                        ((ui4Value&0x0000FF00)<< 8)|     \
                                        ((ui4Value&0x00FF0000)>> 8)|     \
                                        ((ui4Value&0xFF000000)>>24))
*/

static INT32 _ogg_info_set_pos(MINFO_OBJ_T *pt_minfo_obj, 
                                 UINT64 ui8_file_offset, 
                                 UINT8 ui1_whence)
{
    setpos_fct      pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos; 
    MEDIA_FEEDER_T  t_feeder;
    /*feeder*/
    if(pt_minfo_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_minfo_obj->h_src_feeder;
        return pf_set_pos(&t_feeder, (INT64)ui8_file_offset, ui1_whence);
    }
    /*file*/
    else
    {
        return pf_set_pos(pt_minfo_obj, (INT64)ui8_file_offset, ui1_whence);
    }
}

static INT32 _ogg_info_get_pos(MINFO_OBJ_T *pt_minfo_obj, 
                                 UINT64 *pui8_cur_pos)
{
    getpos_fct      pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos; 
    MEDIA_FEEDER_T  t_feeder;
    /*feeder*/
    if(pt_minfo_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_minfo_obj->h_src_feeder;
        return pf_get_pos(&t_feeder, pui8_cur_pos);
    }
    /*file*/
    else
    {
        return pf_get_pos(pt_minfo_obj, pui8_cur_pos);
    }
}

static INT32 _ogg_info_copy_bytes(MINFO_OBJ_T *pt_minfo_obj, 
                                    void* pv_buf, 
                                    SIZE_T z_buf_leng, 
                                    SIZE_T z_copy_size, 
                                    UINT32* pui1_size)
{
    copybytes_fct      pf_copybyte  = pt_minfo_obj->t_input_fct_tbl.pf_copybytes; 
    MEDIA_FEEDER_T     t_feeder;
    
    if (pt_minfo_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_minfo_obj->h_src_feeder;
        return pf_copybyte(&t_feeder, pv_buf, z_buf_leng, z_copy_size, pui1_size);
    }
   
    else
    {
        return pf_copybyte(pt_minfo_obj, pv_buf, z_buf_leng, z_copy_size, pui1_size);
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _ogg_get_aud_codec
 *
 * Description: 
 *
 * Inputs:  -pt_audio_strm_info
 *          -ui1_codec
 * Outputs: -None
 *
 * Returns: Minfo audio stream encoding type
 ----------------------------------------------------------------------------*/
static void _ogg_get_aud_codec(OGG_STREAM_AUDIO_INFO_T* 
                                                    pt_audio_strm_info)
{
    pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
    pt_audio_strm_info->e_layer = 0;
    switch(pt_audio_strm_info->ui4_sub_type)
    {
        case dwBigFourCC('0','0','5','0'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MPEG;*/
            break;
        case dwBigFourCC('0','0','5','5'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MP3;*/
        pt_audio_strm_info->e_layer = 3;
            break;
        case dwBigFourCC('2','0','0','0'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AC3;
            break;
        case dwBigFourCC('2','0','0','1'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_DTS;
            break; 
        case dwBigFourCC('0','0','0','1'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_PCM;
            break;
        case dwBigFourCC('0','0','0','2'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_ADPCM;
            break;
        case dwBigFourCC('0','1','6','1'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_WMA_V1;
            break;
        case dwBigFourCC('0','0','F','F'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AAC;
            break;
        case dwBigFourCC('0','0','1','1'):
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_DVI_ADPCM;
            break;
        default:
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _ogg_get_vid_codec
 *
 * Description: 
 *
 * Inputs:  -ui4_4cc
 *          
 * Outputs: -None
 *
 * Returns: Minfo video stream encoding type
 ----------------------------------------------------------------------------*/
static void _ogg_get_vid_codec(MINFO_OGG_VIDEO_STM_ATRBT_T* 
                                                    pt_video_strm_info)
{
    pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
    switch(pt_video_strm_info->ui4_sub_type)
    {
        case dwBigFourCC('X', 'V', 'I', 'D'):   /*xvid*/
        case dwBigFourCC('x', 'v', 'i', 'd'):
			pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_XVID;
            break;
        case dwBigFourCC('3', 'I', 'V', '1'):   /* 3ivx Delta 1.0~3.5*/
        case dwBigFourCC('3', 'i', 'v', '1'):
        case dwBigFourCC('3', 'I', 'V', '2'):   /* 3ivx Delta 4.0*/
        case dwBigFourCC('3', 'i', 'v', '2'):
        case dwBigFourCC('F', 'V', 'F', 'W'):  /*ffmpeg VFW*/
        case dwBigFourCC('f', 'v', 'f', 'w'):
        case dwBigFourCC('D', 'M', 'K', '2'):  /*a file from Gapha_Tong has this 4cc*/
        case dwBigFourCC('F', 'M', 'P', '4'):  /*some avi has "FMP4" codec 4cc*/
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_MPEG_4;
            break;
        case dwBigFourCC('E', 'M', '2', 'V'):  /*tempt.*/
        case dwBigFourCC('M', 'P', 'G', '2'):
        case dwBigFourCC('m', 'p', 'g', '2'):
        case dwBigFourCC('M', 'P', 'E', 'G'):
        case dwBigFourCC('M', 'P', 'G', 'V'):           
        case dwBigFourCC('m', 'p', 'g', 'v'):                     
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_MPEG_2;
            break;
        case dwBigFourCC('H', '2', '6', '4'):
        case dwBigFourCC('h', '2', '6', '4'):
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_H264;
            break;
        case dwBigFourCC('R', 'M', 'P', '4'):  /*RMP4*/
        case dwBigFourCC('r', 'm', 'p', '4'):
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
            break;
        case dwBigFourCC('D', 'I', 'V', '3'):  /*DivX 3.11*/
        case dwBigFourCC('d', 'i', 'v', '3'):
        case dwBigFourCC('D', 'I', 'V', '4'):
        case dwBigFourCC('d', 'i', 'v', '4'):
        case dwBigFourCC('D', 'I', 'V', '5'):
        case dwBigFourCC('d', 'i', 'v', '5'):
        case dwBigFourCC('D', 'I', 'V', '6'):
        case dwBigFourCC('d', 'i', 'v', '6'):
        case dwBigFourCC('M', 'P', '4', '3'):  /*Microsoft MPEG-4 v3*/
        case dwBigFourCC('m', 'p', '4', '3'):
        /* case FOURCC('M', 'P', '4', 'S'): */
        case dwBigFourCC('C', 'O', 'L', '1'):  /* cool codec*/
        case dwBigFourCC('c', 'o', 'l', '1'):
        case dwBigFourCC('A', 'P', '4', '1'):  /*AngelPotion*/
        case dwBigFourCC('a', 'p', '4', '1'):
        case dwBigFourCC('n', 'A', 'V', 'I'):  /*nAVI*/
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_DIVX_311;
            break;
        case dwBigFourCC('D', 'I', 'V', 'X'):  /*DivX 4.x*/
        case dwBigFourCC('d', 'i', 'v', 'x'):
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_DIVX_4;
            break;
        case dwBigFourCC('D', 'X', '5', '0'):  /* DivX 5.x*/
        case dwBigFourCC('d', 'x', '5', '0'):
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_DIVX_5;
            break;
        case dwBigFourCC('m', 'j', 'p', 'g'):    
        case dwBigFourCC('M', 'J', 'P', 'G'):
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_MJPG;
            break;
        default:
            break;
      }
}

/*-----------------------------------------------------------------------------
 * Name:  _ogg_minfo_parse_bos_page
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/  
INT32 _ogg_minfo_parse_bos_page(MINFO_OBJ_T*    pt_minfo_obj)
{
    UINT32                  ui4_cnt=0;
    INT32                   i4_ret=0;
    UINT32                  ui4_read_succ_data_num = 0;
    UINT8*                  pui1_header = NULL;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj = NULL;
    OGG_STREAM_INFO_T*      pt_ogg_strm_info = NULL;
    const CHAR              i1_vorbis_header[8] = {0x01,'v','o','r','b','i','s','\0'};
    const CHAR              i1_video_header[7] = {0x01,'v','i','d','e','o','\0'};
    const CHAR              i1_audio_header[7] = {0x01,'a','u','d','i','o','\0'};
    
    DBG_INFO(("OGG INFO: ogg_minfo_parse_bos_page\r\n"));
    pt_ogg_info_obj     = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    
    ui4_cnt = pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_audio_cnt+
            pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_video_cnt+
            pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt;
     if (ui4_cnt >= OGG_STREAM_NUM_MAX)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_ogg_strm_info=&pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt];

    i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset, 
                    FM_SEEK_BGN);
    
    i4_ret = _ogg_info_copy_bytes(pt_minfo_obj,
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    9, 
                    &ui4_read_succ_data_num);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }

    i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset, 
                    FM_SEEK_BGN);
    
    pui1_header=pt_ogg_info_obj->pui1_buffer;
    /*vorbis stream bos info*/
    if (!x_memcmp(pui1_header,i1_vorbis_header,7))
    {
        pt_ogg_strm_info->u.t_ogg_audio_param.t_ogg_vorbis_spec.vorbis_header_offset 
                    = pt_ogg_info_obj->ui8_curr_parser_offset;
        i4_ret = _ogg_info_copy_bytes(pt_minfo_obj, 
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    sizeof(OGG_STREAM_VORBIS_INFO_T), 
                    &ui4_read_succ_data_num);    
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        pt_ogg_strm_info->t_ogg_strm_type = OGG_STRM_TYPE_VORBIS;
        pt_ogg_strm_info->u.t_ogg_audio_param.t_ogg_audio_type = OGG_STRM_TYPE_VORBIS;
        pt_ogg_strm_info->ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        pt_ogg_strm_info->u.t_ogg_audio_param.ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        x_memcpy(pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui1_vorbis_id,pui1_header,7);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+7,    pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_vorbis_version);
        LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+11,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui1_audio_channel);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+12, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_audio_sample_rate);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+16, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_bitrate_maximun);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+20, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_bitrate_norminal);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+24, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_bitrate_minimun);
        LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+28,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.blocksize);
        LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+29,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.frameflag);
        pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.e_enc = MINFO_INFO_AUD_ENC_VORBIS;
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt++;
    }
    /*video stream bos info*/
    else if (!x_memcmp(pui1_header,i1_video_header,6))   
    {
        i4_ret = _ogg_info_copy_bytes(pt_minfo_obj, 
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    sizeof(MINFO_OGG_VIDEO_STM_ATRBT_T), 
                    &ui4_read_succ_data_num);    
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }

        pt_ogg_strm_info->t_ogg_strm_type = OGG_STRM_TYPE_VIDEO;
        pt_ogg_strm_info->ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        pt_ogg_strm_info->u.t_ogg_video_param.ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        x_memcpy(pt_ogg_strm_info->u.t_ogg_video_param.ui1_video_id,pui1_header,9);
        LOADB_DWRD(pt_ogg_info_obj->pui1_buffer+9,    pt_ogg_strm_info->u.t_ogg_video_param.ui4_sub_type);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+13,  pt_ogg_strm_info->u.t_ogg_video_param.ui4_size);
        LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+17, pt_ogg_strm_info->u.t_ogg_video_param.ui8_timeunit);
        LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+25, pt_ogg_strm_info->u.t_ogg_video_param.ui8_sample_per_unit);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+33, pt_ogg_strm_info->u.t_ogg_video_param.ui4_default_len);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+37, pt_ogg_strm_info->u.t_ogg_video_param.ui4_buffer_size);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+41,  pt_ogg_strm_info->u.t_ogg_video_param.ui2_bit_per_sample);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+43,  pt_ogg_strm_info->u.t_ogg_video_param.ui2_padding);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+45,  pt_ogg_strm_info->u.t_ogg_video_param.ui4_width);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+49,  pt_ogg_strm_info->u.t_ogg_video_param.ui4_height);
        _ogg_get_vid_codec(&pt_ogg_strm_info->u.t_ogg_video_param);
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_video_cnt++;
    }
    /*audio stream bos info*/
    else if (!x_memcmp(pui1_header,i1_audio_header,6))
    {
        i4_ret = _ogg_info_copy_bytes(pt_minfo_obj, 
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    sizeof(OGG_STREAM_AUDIO_INFO_T), 
                    &ui4_read_succ_data_num);    
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }

        pt_ogg_strm_info->t_ogg_strm_type = OGG_STRM_TYPE_AUDIO;
        pt_ogg_strm_info->u.t_ogg_audio_param.t_ogg_audio_type = OGG_STRM_TYPE_AUDIO;
        pt_ogg_strm_info->ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        pt_ogg_strm_info->u.t_ogg_audio_param.ui4_serial_num = pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui4_bitstream_serial_num;
        x_memcpy(pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui1_audio_id,pui1_header,9);
        LOADB_DWRD(pt_ogg_info_obj->pui1_buffer+9,     pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui4_sub_type);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+13,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui4_size);
        LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+17, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui8_timeunit);
        LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+25, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui8_sample_per_unit);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+33, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui4_default_len);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+37, pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui4_buffer_size);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+41,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui2_bit_per_sample);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+43,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui2_padding);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+45,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui2_channels);
        LOADL_WORD(pt_ogg_info_obj->pui1_buffer+47,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui2_blockalign);
        LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+49,  pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui4_avg_bytes_per_sec);
        _ogg_get_aud_codec(&pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info);
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_audio_cnt++;
    }
    else
    {
    }

    pt_ogg_info_obj->b_bos_page_found = TRUE;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _ogg_select_strm
 *
 * Description: find the same serial num strm and return the stream type and pts in array
 *
 * Inputs:  ui8_serial_num
 *          
 * Outputs: ui4_pos
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/  
INT32 _ogg_select_strm(MINFO_OBJ_T*         pt_minfo_obj,    
                            UINT32              ui8_serial_num,
                            UINT32*             pui4_pos,
                            OGG_STRM_TYPE_T*    pt_ogg_strm_type)
{

    UINT8                   ui1_cnt;        
    UINT8                   ui1_num;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj=NULL;
    OGG_STREAM_INFO_T*      pt_ogg_strm_info=NULL;

    pt_ogg_info_obj = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
    ui1_num = pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_audio_cnt+
            pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_video_cnt+
            pt_ogg_info_obj->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt;

    for (ui1_cnt=0;ui1_cnt<ui1_num;ui1_cnt++)
    {
        if (ui1_cnt >= OGG_STREAM_NUM_MAX)
        {
            return MINFOR_INFO_NOT_FOUND;
        }
        pt_ogg_strm_info = &pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui1_cnt];
        if(ui8_serial_num == pt_ogg_strm_info->ui4_serial_num)
            break;
    }

    if (ui1_cnt<ui1_num)
    {
        *pui4_pos = ui1_cnt;
        *pt_ogg_strm_type = pt_ogg_strm_info->t_ogg_strm_type;
    }
    else
    {
        return MINFOR_INFO_NOT_FOUND;
    }

    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _ogg_minfo_parse_eos_page
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/  
INT32 _ogg_minfo_parse_eos_page(MINFO_OBJ_T*    pt_minfo_obj)
{
    UINT32                    ui4_cnt=0;
    INT32                    i4_ret=0;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj = NULL;
    OGG_PAGE_HEADER_T*      pt_ogg_page_header = NULL;
    OGG_STRM_TYPE_T            pt_ogg_strm_type;

    DBG_INFO(("OGG INFO: ogg_minfo_parse_eos_page\r\n")); 
    pt_ogg_info_obj = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
    pt_ogg_page_header = &pt_ogg_info_obj->t_file_layout.t_ogg_page_header;

    i4_ret = _ogg_select_strm(pt_minfo_obj,pt_ogg_page_header->ui4_bitstream_serial_num,&ui4_cnt,&pt_ogg_strm_type);
    if (i4_ret != MINFOR_OK)
    {
        return MINFOR_INFO_NOT_FOUND;
    }
    /*get the last granule pos*/
    if (ui4_cnt >= OGG_STREAM_NUM_MAX)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_VORBIS)
        ||(pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_AUDIO))
    {
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.ui8_granule_pos 
                = pt_ogg_page_header->ui8_granule_pos;
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.ui4_audio_page_num
                = pt_ogg_page_header->ui4_page_sequence_num;
    }
    else if (pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_VIDEO)
    {
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_video_param.ui8_granule_pos 
                = pt_ogg_page_header->ui8_granule_pos;
        pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_video_param.ui4_video_page_num
                = pt_ogg_page_header->ui4_page_sequence_num;
    }

    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _ogg_minfo_parse_vorbis_comment_page
 *
 * Description: parse the vorbis comment info,like copyright,artist,etc.
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/  

INT32 _ogg_minfo_parse_vorbis_comment_page(MINFO_OBJ_T*    pt_minfo_obj)
{
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _ogg_minfo_parse_data_page
 *
 * Description: parse the normal page of stream
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/  
INT32 _ogg_minfo_parse_data_page(MINFO_OBJ_T*    pt_minfo_obj)
{

	MINFO_OGG_FILE_INF_T*	pt_ogg_info_obj = NULL;
	OGG_PAGE_HEADER_T*		pt_ogg_page_header = NULL;
	INT32					i4_ret = 0;
	UINT32					ui4_read_succ_data_num = 0;
	UINT8*					pui1_header;
	const CHAR				i1_vorbis_comment[8] = {0x03,'v','o','r','b','i','s','\0'};
	const CHAR				i1_vorbis_setup[8] = {0x05,'v','o','r','b','i','s','\0'};
	UINT32					ui4_cnt=0;
	OGG_STRM_TYPE_T 		pt_ogg_strm_type;
	UINT32					ui4_cur_offset = 0;
	UINT32					ui4_i = 0;


	pt_ogg_info_obj = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
	pt_ogg_page_header	   = &pt_ogg_info_obj->t_file_layout.t_ogg_page_header;

	i4_ret = _ogg_select_strm(pt_minfo_obj,pt_ogg_page_header->ui4_bitstream_serial_num,&ui4_cnt,&pt_ogg_strm_type);
	if (i4_ret != MINFOR_OK)
	{
		return MINFOR_INFO_NOT_FOUND;
	}
	if (ui4_cnt >= OGG_STREAM_NUM_MAX)
	{
		return MINFOR_INTERNAL_ERR;
	}
	if ((pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_VORBIS)
		||(pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_AUDIO))
	{
		pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.ui8_granule_pos 
				= pt_ogg_page_header->ui8_granule_pos;
		pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.ui4_audio_page_num
				= pt_ogg_page_header->ui4_page_sequence_num;
	}
	else if (pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].t_ogg_strm_type == OGG_STRM_TYPE_VIDEO)
	{
		pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_video_param.ui8_granule_pos 
				= pt_ogg_page_header->ui8_granule_pos;
		pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_video_param.ui4_video_page_num
				= pt_ogg_page_header->ui4_page_sequence_num;
	}
	
	ui4_cur_offset = 0;
	if (pt_ogg_page_header->ui8_granule_pos == 0)
	{
		for (ui4_i = 0; ui4_i< pt_ogg_info_obj->t_file_layout.t_ogg_page_header.ui1_page_segment; ui4_i ++)
		{
			i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
							pt_ogg_info_obj->ui8_curr_parser_offset + ui4_cur_offset, 
							FM_SEEK_BGN);

			i4_ret = _ogg_info_copy_bytes(pt_minfo_obj,
							pt_ogg_info_obj->pui1_buffer, 
							(SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
							9, 
							&ui4_read_succ_data_num);
			if (i4_ret != INPUTR_OK)
			{
				return MINFOR_INTERNAL_ERR;
			}
			
			pui1_header=pt_ogg_info_obj->pui1_buffer;
			if (!x_memcmp(pui1_header,i1_vorbis_comment,7))
			{
				pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.t_ogg_vorbis_spec.comment_offset
						= pt_ogg_info_obj->ui8_curr_parser_offset + ui4_cur_offset;
				i4_ret = _ogg_minfo_parse_vorbis_comment_page(pt_minfo_obj);
				if (i4_ret != INPUTR_OK)
				{
					return MINFOR_INTERNAL_ERR;
				}
				
			}
			else if (!x_memcmp(pui1_header,i1_vorbis_setup,7))
			{
				pt_ogg_info_obj->t_file_layout.t_ogg_stream_info[ui4_cnt].u.t_ogg_audio_param.t_ogg_vorbis_spec.setup_offset
						= pt_ogg_info_obj->ui8_curr_parser_offset + ui4_cur_offset;
			}
			else
			{
				if (!pt_ogg_info_obj->b_data_offset_found)
				{
					pt_ogg_info_obj->ui8_data_start_pts = pt_ogg_info_obj->ui8_curr_parser_offset - 27 
							- pt_ogg_page_header->ui1_page_segment;
					pt_ogg_info_obj->b_data_offset_found = TRUE;
				}
			}
			ui4_cur_offset += pt_ogg_info_obj->t_file_layout.t_ogg_page_header.segment_table[ui4_i];
		}
		
		return MINFOR_OK;
	}

	return MINFOR_OK;
}



/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_all_page_parse
 *
 * Description: parse page info, found bos, eos page and common page.
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
INT32 ogg_minfo_all_page_parse(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0; 
    UINT32                ui4_i = 0;
    UINT32                ui4_cnt = 0;
    UINT32            ui4_offset = 0;
    UINT32                ui4_read_succ_data_num;
    UINT32                ui4_header_id;
    input4bytes_fct         pf_inp4byte;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj=NULL;
    OGG_PAGE_HEADER_T*      pt_ogg_page_header=NULL;

    DBG_INFO(("OGG INFO: ogg_minfo_all_page_parse begin\r\n")); 
    pf_inp4byte         = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_ogg_info_obj     = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_ogg_page_header     = &pt_ogg_info_obj->t_file_layout.t_ogg_page_header;

    while (1)
    {
        i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset,
                    FM_SEEK_BGN);
        
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_header_id);
        if (i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF)
            {
                break;
            }
            return MINFOR_INTERNAL_ERR;
        }

        i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset,
                    FM_SEEK_BGN);
        if (ui4_header_id == OGG_PAGE_HEADER)
        {
            UINT32          ui4_read_num;

            x_memset(pt_ogg_page_header,0,sizeof(OGG_PAGE_HEADER_T));
            ui4_read_num = OGG_MAX_HEAD_LEN;
            if (OGG_MAX_HEAD_LEN+pt_ogg_info_obj->ui8_curr_parser_offset > pt_ogg_info_obj->t_generic_info.ui8_media_sz)
            {
                ui4_read_num = pt_ogg_info_obj->t_generic_info.ui8_media_sz-pt_ogg_info_obj->ui8_curr_parser_offset;
            }

            _ogg_info_set_pos(pt_minfo_obj, 
               pt_ogg_info_obj->ui8_curr_parser_offset, 
               FM_SEEK_BGN);

            i4_ret = _ogg_info_copy_bytes(pt_minfo_obj, 
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    ui4_read_num, 
                    &ui4_read_succ_data_num);    
            if (i4_ret != INPUTR_OK)
            {
                DBG_INFO(("OGG INFO: read buffer error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            LOADB_DWRD(pt_ogg_info_obj->pui1_buffer,        pt_ogg_page_header->ui4_object_id);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+4,       pt_ogg_page_header->ui1_version);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+5,       pt_ogg_page_header->ui1_header_type);
            LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+6,      pt_ogg_page_header->ui8_granule_pos);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+14,     pt_ogg_page_header->ui4_bitstream_serial_num);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+18,     pt_ogg_page_header->ui4_page_sequence_num);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+22,     pt_ogg_page_header->ui4_crc_check);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+26,      pt_ogg_page_header->ui1_page_segment);
            pt_ogg_page_header->page_size = 27;
            ui4_offset = 27;

            if(pt_ogg_page_header->ui1_page_segment>255)
            {
                DBG_INFO(("OGG INFO: wrong page segment num\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            
            if(pt_ogg_page_header->ui1_page_segment>0)
            {
                for(ui4_cnt=0;ui4_cnt<pt_ogg_page_header->ui1_page_segment;ui4_cnt++)
                {
                    LOAD_BYTE(pt_ogg_info_obj->pui1_buffer + 27 + ui4_cnt, pt_ogg_page_header->segment_table[ui4_cnt]);
                    pt_ogg_page_header->page_size += pt_ogg_page_header->segment_table[ui4_i]+1;
                    ui4_offset ++;
                }
            }
            
            pt_ogg_info_obj->ui8_curr_parser_offset += ui4_offset;

            if ((pt_ogg_page_header->ui1_header_type&0x02)>>1)
            {
                i4_ret = _ogg_minfo_parse_bos_page(pt_minfo_obj);
            }
            else if ((pt_ogg_page_header->ui1_header_type&0x04)>>2)
            {
                i4_ret = _ogg_minfo_parse_eos_page(pt_minfo_obj);
            }
            else
            {
                i4_ret = _ogg_minfo_parse_data_page(pt_minfo_obj);
            }

            if (i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }

            for (ui4_i=0;ui4_i<pt_ogg_page_header->ui1_page_segment;ui4_i++)
            {
                pt_ogg_info_obj->ui8_curr_parser_offset += pt_ogg_page_header->segment_table[ui4_i];
            }
            if (pt_ogg_info_obj->ui8_curr_parser_offset >= pt_ogg_info_obj->t_generic_info.ui8_media_sz)
            {
                break;
            }
            
        }
        else
        {
            DBG_INFO(("OGG INFO: wrong page head\r\n"));
            return MINFOR_INTERNAL_ERR;
        }
    }
    DBG_INFO(("OGG INFO: ogg_minfo_page_parse succeed!\r\n"));
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_all_page_parse
 *
 * Description: found eos page, parse eos page head.
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
INT32 _ogg_parse_last_64k_file(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj=NULL;
    input4bytes_fct         pf_inp4byte;
    UINT32                    ui4_header_id = 0;

    pf_inp4byte         = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_ogg_info_obj     = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
    pt_ogg_info_obj->ui8_curr_parser_offset = pt_ogg_info_obj->t_generic_info.ui8_media_sz - LAST_64K_DATA_LENGTH;

    i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                pt_ogg_info_obj->ui8_curr_parser_offset, 
                FM_SEEK_BGN);
    /*search ogg page header*/
    do
    {
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_header_id);
        if (i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF)
            {
                break;
            }
            return MINFOR_INTERNAL_ERR;
        }
        if (ui4_header_id == OGG_PAGE_HEADER)
        {
            break;
        }
        else
        {
            pt_ogg_info_obj->ui8_curr_parser_offset++;

            i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset, 
                    FM_SEEK_BGN);
        }
    } while(pt_ogg_info_obj->ui8_curr_parser_offset+27 < pt_ogg_info_obj->t_generic_info.ui8_media_sz);
    
    if (pt_ogg_info_obj->ui8_curr_parser_offset+27 >= pt_ogg_info_obj->t_generic_info.ui8_media_sz)
    {
        return MINFOR_INFO_NOT_FOUND;
    }
    /*parse last page*/
    i4_ret = ogg_minfo_all_page_parse(pt_minfo_obj);    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_page_parse
 *
 * Description: parse page info, found bos.
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
INT32 ogg_minfo_page_parse(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0; 
    UINT32                  ui4_i = 0;
    UINT32                  ui4_cnt = 0;
    UINT32                  ui4_page_num = 0;
    UINT32            ui4_offset = 0;
    UINT32                  ui4_read_succ_data_num;
    UINT32                  ui4_header_id;
    input4bytes_fct         pf_inp4byte;
    MINFO_OGG_FILE_INF_T*   pt_ogg_info_obj=NULL;
    OGG_PAGE_HEADER_T*      pt_ogg_page_header=NULL;

    DBG_INFO(("OGG INFO: ogg_minfo_page_parse begin\r\n")); 
    pf_inp4byte         = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_ogg_info_obj     = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_ogg_page_header     = &pt_ogg_info_obj->t_file_layout.t_ogg_page_header;
    ui4_page_num         = 0;

    while (1)
    {
        i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset,
                    FM_SEEK_BGN);
        
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_header_id);
        if (i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF)
            {
                break;
            }
            else if (pt_ogg_info_obj->b_bos_page_found)
            {
                return MINFOR_OK;
            }
            else
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
        i4_ret = _ogg_info_set_pos(pt_minfo_obj, 
                    pt_ogg_info_obj->ui8_curr_parser_offset,
                    FM_SEEK_BGN);
        if (ui4_header_id == OGG_PAGE_HEADER)
        {
            UINT32          ui4_read_num;

            x_memset(pt_ogg_page_header,0,sizeof(OGG_PAGE_HEADER_T));
            ui4_read_num = OGG_MAX_HEAD_LEN;
            if (OGG_MAX_HEAD_LEN+pt_ogg_info_obj->ui8_curr_parser_offset > pt_ogg_info_obj->t_generic_info.ui8_media_sz)
            {
                ui4_read_num = pt_ogg_info_obj->t_generic_info.ui8_media_sz-pt_ogg_info_obj->ui8_curr_parser_offset;
            }

            _ogg_info_set_pos(pt_minfo_obj, 
               pt_ogg_info_obj->ui8_curr_parser_offset, 
               FM_SEEK_BGN);

            i4_ret = _ogg_info_copy_bytes(pt_minfo_obj, 
                    pt_ogg_info_obj->pui1_buffer, 
                    (SIZE_T)pt_ogg_info_obj->ui4_buf_len, 
                    ui4_read_num, 
                    &ui4_read_succ_data_num);    
            if (i4_ret != INPUTR_OK)
            {
                DBG_INFO(("OGG INFO: read buffer error!\r\n"));
                return MINFOR_INTERNAL_ERR;
            }
            LOADB_DWRD(pt_ogg_info_obj->pui1_buffer,        pt_ogg_page_header->ui4_object_id);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+4,         pt_ogg_page_header->ui1_version);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+5,         pt_ogg_page_header->ui1_header_type);
            LOADL_QWRD(pt_ogg_info_obj->pui1_buffer+6,        pt_ogg_page_header->ui8_granule_pos);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+14,       pt_ogg_page_header->ui4_bitstream_serial_num);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+18,       pt_ogg_page_header->ui4_page_sequence_num);
            LOADL_DWRD(pt_ogg_info_obj->pui1_buffer+22,       pt_ogg_page_header->ui4_crc_check);
            LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+26,        pt_ogg_page_header->ui1_page_segment);
            pt_ogg_page_header->page_size = 27;
            ui4_offset = 27;

            if(pt_ogg_page_header->ui1_page_segment>255)
            {
                DBG_INFO(("OGG INFO: wrong page segment num\r\n"));
                return MINFOR_INTERNAL_ERR;
            }

            if(pt_ogg_page_header->ui1_page_segment>0)
            {
                for(ui4_cnt=0;ui4_cnt<pt_ogg_page_header->ui1_page_segment;ui4_cnt++)
                {
                    LOAD_BYTE(pt_ogg_info_obj->pui1_buffer+27+ui4_cnt,     pt_ogg_page_header->segment_table[ui4_cnt]);
                    pt_ogg_page_header->page_size += pt_ogg_page_header->segment_table[ui4_cnt]+1;
            ui4_offset++;
                }
            }

            pt_ogg_info_obj->ui8_curr_parser_offset += ui4_offset;

            if ((pt_ogg_page_header->ui1_header_type&0x02)>>1)
            {
                i4_ret = _ogg_minfo_parse_bos_page(pt_minfo_obj);
                ui4_page_num++;
            }
            else if ((pt_ogg_page_header->ui1_header_type&0x04)>>2)
            {
                i4_ret = _ogg_minfo_parse_eos_page(pt_minfo_obj);
                ui4_page_num++;
            }
            else
            {
                i4_ret = _ogg_minfo_parse_data_page(pt_minfo_obj);
                ui4_page_num++;
            }

            if (i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }

            for (ui4_i=0;ui4_i<pt_ogg_page_header->ui1_page_segment;ui4_i++)
            {
                pt_ogg_info_obj->ui8_curr_parser_offset += pt_ogg_page_header->segment_table[ui4_i];
            }
        }
        else
        {
            DBG_INFO(("OGG INFO: wrong page head\r\n"));
            if (pt_ogg_info_obj->b_bos_page_found)
            {
                return MINFOR_OK;
            }
            else
            {
                return MINFOR_INTERNAL_ERR;
            }
        }

        if (ui4_page_num > BIG_FILE_PARSE_PAGE_MAX_NUM)
        {
            if (!pt_minfo_obj->b_filesize_non_gettable)
            {
                i4_ret = _ogg_parse_last_64k_file(pt_minfo_obj);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
            }
            break;
        }
    }
    DBG_INFO(("OGG INFO: ogg_minfo_page_parse succeed!\r\n"));
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_file_minfo_parse
 *
 * Description: 
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 ogg_file_minfo_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    MINFO_OGG_FILE_INF_T*       pt_ogg_hdlr_obj = NULL;
    INT32                       i4_ret = 0;
    UINT64                      ui8_duration = 0;
    UINT8                       ui1_i = 0;
    OGG_STREAM_INFO_T*          pt_ogg_strm_info=NULL;
        

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("ERROR: ogg_file_minfo_parse fail.1 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        DBG_ERROR(("ERROR: ogg_file_minfo_parse fail.2 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pt_ogg_hdlr_obj = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;

    i4_ret = MINFOR_INTERNAL_ERR;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*store file size*/
    i4_ret = _ogg_info_set_pos(pt_minfo_obj , 0, FM_SEEK_END);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
        return i4_ret;
    }
    _ogg_info_get_pos(pt_minfo_obj, &(pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz));
    
    if (pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz == (UINT64)(-1))
    {
        pt_minfo_obj->b_filesize_non_gettable = TRUE;
    }
    
    /*seek to beginning*/
    i4_ret = _ogg_info_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN);
    pt_ogg_hdlr_obj->ui8_curr_parser_offset = 0;
    
    /*parse file*/
    if (pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz <= OGG_FILE_PARSE_MAX_SIZE)
    {
        i4_ret = ogg_minfo_all_page_parse(pt_minfo_obj);
    }
    else
    {
        i4_ret = ogg_minfo_page_parse(pt_minfo_obj);
    }
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR:ogg_file_minfo_parse parse fail.i4_ret = %ld \r\n",
            i4_ret));
        return i4_ret;
    }
    
    /* store generic info */
    pt_ogg_hdlr_obj->t_generic_info.ui1_strm_ns = (UINT8)
                (pt_ogg_hdlr_obj->t_file_layout.t_ogg_stream_num.ui1_video_cnt
                + pt_ogg_hdlr_obj->t_file_layout.t_ogg_stream_num.ui1_audio_cnt
                +pt_ogg_hdlr_obj->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt);
    
    for (ui1_i=0;ui1_i<pt_ogg_hdlr_obj->t_generic_info.ui1_strm_ns;ui1_i++)
    {
        if (ui1_i >= OGG_STREAM_NUM_MAX)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_ogg_strm_info = &pt_ogg_hdlr_obj->t_file_layout.t_ogg_stream_info[ui1_i];
        if (pt_ogg_strm_info->t_ogg_strm_type == OGG_STRM_TYPE_VORBIS)
        {
            ui8_duration = _mm_div64(pt_ogg_strm_info->u.t_ogg_audio_param.ui8_granule_pos*1000,
                                pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.ui4_audio_sample_rate,
                                NULL);
            if (pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num != 0)
            {
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_frame_num = _mm_div64(pt_ogg_strm_info->u.t_ogg_audio_param.ui8_granule_pos*100,
                                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num,
                                NULL);
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_packet_size = _mm_div64(pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz,
                                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num,
                                NULL);
            }
            else
            {
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_frame_num = 0;
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_packet_size = 0;
            }
        }
        else if (pt_ogg_strm_info->t_ogg_strm_type == OGG_STRM_TYPE_AUDIO)
        {
            ui8_duration = _mm_div64(pt_ogg_strm_info->u.t_ogg_audio_param.ui8_granule_pos*1000,
                                pt_ogg_strm_info->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.ui8_sample_per_unit,
                                NULL);
            if (pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num != 0)
            {
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_frame_num = _mm_div64(pt_ogg_strm_info->u.t_ogg_audio_param.ui8_granule_pos*100,
                                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num,
                                NULL);
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_packet_size = _mm_div64(pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz,
                                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_audio_page_num,
                                NULL);
            }
            else
            {
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_frame_num = 0;
                pt_ogg_strm_info->u.t_ogg_audio_param.ui4_avg_packet_size = 0;
            }
        }
        else if (pt_ogg_strm_info->t_ogg_strm_type == OGG_STRM_TYPE_VIDEO)
        {
            ui8_duration = _mm_div64(pt_ogg_strm_info->u.t_ogg_video_param.ui8_granule_pos*pt_ogg_strm_info->u.t_ogg_video_param.ui8_timeunit,
                                10000,
                                NULL);
            if (pt_ogg_strm_info->u.t_ogg_video_param.ui4_video_page_num != 0)
            {
                pt_ogg_strm_info->u.t_ogg_video_param.ui4_avg_frame_num = _mm_div64(pt_ogg_strm_info->u.t_ogg_video_param.ui8_granule_pos*100,
                                pt_ogg_strm_info->u.t_ogg_video_param.ui4_video_page_num,
                                NULL);
                pt_ogg_strm_info->u.t_ogg_video_param.ui4_avg_packet_size = _mm_div64(pt_ogg_hdlr_obj->t_generic_info.ui8_media_sz,
                                pt_ogg_strm_info->u.t_ogg_video_param.ui4_video_page_num,
                                NULL);
            }
            else
            {
                pt_ogg_strm_info->u.t_ogg_video_param.ui4_avg_frame_num = 0;
                pt_ogg_strm_info->u.t_ogg_video_param.ui4_avg_packet_size = 0;
            }
        }
        else
        {
            continue;
        }
        
        if (ui8_duration > pt_ogg_hdlr_obj->t_generic_info.ui8_pb_duration)
        {
            pt_ogg_hdlr_obj->t_generic_info.ui8_pb_duration = ui8_duration;
        }
    }
    pt_ogg_hdlr_obj->t_generic_info.ui8_pb_duration = pt_ogg_hdlr_obj->t_generic_info.ui8_pb_duration*90;
    DBG_INFO(("INFO: ogg_file_minfo_parse OK. \r\n"));
    
    return MINFOR_OK;
}
