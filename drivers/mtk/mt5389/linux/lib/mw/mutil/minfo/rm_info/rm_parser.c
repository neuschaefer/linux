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
 * rm_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the rm parser function
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
#include "rm_parser.h"

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
#include "rm_parser.h"

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


#define ENABLE_LOG 0
#define SAVE_FILE_NAVE "navy.txt"


#define RM_INDEX_RECORD_SIZE           14
#define RM_DATA_HEADER_SIZE            18
#define RM_INDEX_HEADER_SIZE           20

#define REALAUDIO_MIME_TYPE            "audio/x-pn-realaudio"
#define REALAUDIO_ENCRYPTED_MIME_TYPE  "audio/x-pn-realaudio-encrypted"
#define REALVIDEO_MIME_TYPE            "video/x-pn-realvideo"
#define REALVIDEO_ENCRYPTED_MIME_TYPE  "video/x-pn-realvideo-encrypted"

#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
#define HX_GET_MAJOR_VERSION(prodVer) ((prodVer >> 28) & 0xF)
#define HX_GET_MINOR_VERSION(prodVer) ((prodVer >> 20) & 0xFF)

#define RM_BOX_RMF       dwBigFourCC('.', 'R', 'M', 'F')
#define RM_BOX_PROP       dwBigFourCC('P', 'R', 'O', 'P')
#define RM_BOX_CONT       dwBigFourCC('C', 'O', 'N', 'T')
#define RM_BOX_MDPR       dwBigFourCC('M', 'D', 'P', 'R')
#define RM_BOX_DATA       dwBigFourCC('D', 'A', 'T', 'A')
#define RM_BOX_INDX       dwBigFourCC('I', 'N', 'D', 'X')
#define AAC_INVALID_CH  0xFF
/*
#define LITTLE_TO_BIG_WORD(ui2Value)    (((ui2Value&0x00FF)<< 8)|         \
                                        ((ui2Value&0xFF00)>>8))
#define LITTLE_TO_BIG_DWORD(ui4Value)   (((ui4Value&0x000000FF)<<24)|    \
                                        ((ui4Value&0x0000FF00)<< 8)|     \
                                        ((ui4Value&0x00FF0000)>> 8)|     \
                                        ((ui4Value&0xFF000000)>>24))
*/
#if 1
UINT32 *_au4RprSizeArray;
#endif
static UINT32 _u4BitPos;
static UINT32 _u4BitNum;
enum AudioObjectType
{
    AACMAIN = 1,
    AACLC   = 2,
    AACSSR  = 3,
    AACLTP  = 4,
    AACSBR  = 5,
    AACSCALABLE = 6,
    TWINVQ  = 7
};
const UINT32 aSampleRate[13] =
{
    96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000,
    11025, 8000, 7350
};
const UINT32 channelMapping[][4] =
{
    {0,0,0,0},
    {1,0,0,0}, /* center */
    {2,0,0,0}, /* left, right */
    {3,0,0,0}, /* center, left, right */
    {3,0,1,0}, /* center, left, right, rear surround */
    {3,0,2,0}, /* center, left, right, left surround, right surround */
    {3,0,2,1}, /* center, left, right, left surround, right surround, lfe */
    {5,0,2,1}, /* center, left, right, left outside, right outside, left surround, right surround, lfe */
};
static UINT16 bAacChannelCfg(UINT32 dwChannelNs)
{
  if((dwChannelNs >= 1) && (dwChannelNs <= 6))
  {
    return((UINT16)dwChannelNs);
  }
  if(dwChannelNs == 8)
  {
    return(7);
  }
  return(AAC_INVALID_CH);
}
static void byteAlign(UINT8 *bs)
{
  if (_u4BitPos % 8)
    _u4BitPos += (8 - _u4BitPos % 8);
}
static void initBits(UINT8 *bs, UINT32 u4BitNum)
{
  _u4BitPos = 0;
  _u4BitNum = u4BitNum;
}
static UINT32 readBits(UINT8 *bs, UINT32 u4BitCnt)
{
  UINT32 u4Ret = 0;
  
  if (_u4BitNum < u4BitCnt)
    return 0;

  while (u4BitCnt > 0)
  {
    u4Ret = ((u4Ret << 1) | ((*(bs + (_u4BitPos >> 3)) >> (7 - _u4BitPos % 8)) & 0x01));
    _u4BitPos++;
    _u4BitNum--;
    u4BitCnt--;
  }
  
  return u4Ret;
}

static INT32 _rm_info_set_pos(MINFO_OBJ_T *pt_minfo_obj, 
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

static INT32 _rm_info_get_pos(MINFO_OBJ_T *pt_minfo_obj, 
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

static INT32 _rm_info_copy_bytes(MINFO_OBJ_T *pt_minfo_obj, 
                                    void* pv_buf, 
                                    SIZE_T z_buf_leng, 
                                    SIZE_T z_copy_size, 
                                    UINT32* pui4_size)
{
    copybytes_fct      pf_copybyte  = pt_minfo_obj->t_input_fct_tbl.pf_copybytes; 
    MEDIA_FEEDER_T     t_feeder;
    /*feeder*/
    if(pt_minfo_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_minfo_obj->h_src_feeder;
        return pf_copybyte(&t_feeder, pv_buf, z_buf_leng, z_copy_size, pui4_size);
    }
    /*file*/
    else
    {
        return pf_copybyte(pt_minfo_obj, pv_buf, z_buf_leng, z_copy_size, pui4_size);
    }
}

static BOOL ga_config_get_data(MINFO_RM_AUDIO_STM_ATRBT_T *ptSamInf, 
                                    UINT8 *bs, UINT32 u4BitNum)
{
    UINT32 i;
    UINT32 index = 0;
    UINT32 flag = 0;
    UINT32 skip = 0;
    UINT32 channelConfig = 0;
    UINT32 audioObjectType;
    UINT32 numFrontChannels = 0;
    UINT32 numSideChannels = 0;
    UINT32 numBackChannels = 0;
    UINT32 numFrontElements;
    UINT32 numSideElements;
    UINT32 numBackElements;
    UINT32 numLfeElements;
    UINT32 numAssocElements;
    UINT32 numValidCCElements;
    BOOL bSBR;
    
    initBits(bs, u4BitNum);
    
    /* audio object type */
    audioObjectType = readBits(bs, 5);

    /* sampling frequency */
    index = readBits(bs, 4);
    if (index == 0xF)
        ptSamInf->ui4_sample_rate= (readBits(bs, 24));
    else
        ptSamInf->ui4_sample_rate = (aSampleRate[index]);

    /* channel configuration */
    channelConfig = readBits(bs, 4);

    /* sbr parameters */
    if (audioObjectType == AACSBR)
    {
        bSBR = TRUE;
        index = readBits(bs, 4);
        readBits(bs, 24);
        audioObjectType = readBits(bs, 5);
    }
    else
    {
        bSBR = FALSE;
    }

    /* make sure format is supported */
    if (audioObjectType == AACMAIN ||
        audioObjectType == AACLC ||
        audioObjectType == AACLTP)
    {
        flag = readBits(bs, 1);
        flag = readBits(bs, 1);
        if (flag)
            readBits(bs, 14);
        flag = readBits(bs, 1); /* extension flag - not defined */

        if (channelConfig == 0)
        {
            skip = readBits(bs, 4); /* ignore element instance tag */
            skip = readBits(bs, 2); /* ignore object type */
            skip = readBits(bs, 4); /* ignore sampling frequency index */

            numFrontElements = readBits(bs, 4); 
            numSideElements = readBits(bs, 4);
            numBackElements = readBits(bs, 4);
            numLfeElements = readBits(bs, 2);
            numAssocElements = readBits(bs, 3);
            numValidCCElements = readBits(bs, 4);

            if(readBits(bs, 1)) /* mono mixdown present */
                skip = readBits(bs, 4); /* ignore mixdown element */

            if(readBits(bs, 1)) /* stereo mixdown present */
                skip = readBits(bs, 4); /* ignore mixdown element */

            if(readBits(bs, 1)) /* matrix mixdown present */
            {
                skip = readBits(bs, 2); /* ignore mixdown index */
                skip = readBits(bs, 1); /* ignore surround enable */
            }

            for ( i = 0; i < numFrontElements; i++)
            {
                numFrontChannels += (1+readBits(bs, 1));
                skip = readBits(bs, 4); /* ignore tag select */
            }
            for ( i = 0; i < numSideElements; i++)
            {
                numSideChannels += (1+readBits(bs, 1));
                skip = readBits(bs, 4); /* ignore tag select */
            }
            for ( i = 0; i < numBackElements; i++)
            {
                numBackChannels += (1+readBits(bs, 1));
                skip = readBits(bs, 4); /* ignore tag select */
            }
            for ( i = 0; i < numLfeElements; i++)
                skip = readBits(bs, 4); /* ignore tag select */
            for ( i = 0; i < numAssocElements; i++)
                skip = readBits(bs, 4); /* ignore tag select */
            for ( i = 0; i < numValidCCElements; i++)
            {
                skip = readBits(bs, 1); /* ignore 'is_ind_sw' */
                skip = readBits(bs, 4); /* ignore tag select */
            }

            byteAlign(bs);

            /* ignore comment field data */
            index = readBits(bs, 8);
            for (i = 0; i < index; i++)
            {
                skip = readBits(bs, 8);
            }
        }
        else
        {
            if (channelConfig >= 8)
                return FALSE;

            numFrontChannels = channelMapping[channelConfig][0];
            numSideChannels = channelMapping[channelConfig][1];
            numBackChannels  = channelMapping[channelConfig][2];
            numLfeElements = channelMapping[channelConfig][3];

            numFrontElements = (numFrontChannels + 1)>>1;
            numSideElements  = (numSideChannels + 1)>>1;
            numBackElements  = (numBackChannels + 1)>>1;
        }

        ptSamInf->ui2_num_channel= bAacChannelCfg(numFrontChannels +
                                             numSideChannels +
                                             numBackChannels +
                                             numLfeElements);

    }
    else /* format not supported */
    {
        return FALSE;
    }

    /* check for SBR info if there is enough data left in the bitstream */
    if (!bSBR && _u4BitNum >= 16)
    {
        if (readBits(bs, 11) == 0x2b7)
        {
            if (readBits(bs, 5) == 5)
            {
                bSBR = readBits(bs, 1);
            }
        }
    }
    skip = skip;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_get_aud_codec
 *
 * Description: 
 *
 * Inputs:  -pt_audio_strm_info
 *          -ui1_codec
 * Outputs: -None
 *
 * Returns: Minfo audio stream encoding type
 ----------------------------------------------------------------------------*/
static void _rm_get_aud_codec(MINFO_RM_AUDIO_STM_ATRBT_T* 
                                                    pt_audio_strm_info)
{
    switch(pt_audio_strm_info->ui4_codec_id)
    {
    case dwBigFourCC('r', 'a', 'a', 'c'):
    case dwBigFourCC('r', 'a', 'c', 'p'):
        pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AAC;
        break;
    case dwBigFourCC('c', 'o', 'o', 'k'):
        if(pt_audio_strm_info->ui2_flavor_idx > 29)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        }
        else
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_COOK;
        }
        break;
    case dwBigFourCC('a', 't', 'r', 'c'):
    case dwBigFourCC('s', 'i', 'p', 'r'):
    default:
        pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        break;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_video_codec
 *
 * Description: 
 *
 * Inputs:  -ui4_4cc
 *          
 * Outputs: -None
 *
 * Returns: Minfo video stream encoding type
 ----------------------------------------------------------------------------*/
static void _rm_get_vid_codec(MINFO_RM_VIDEO_STM_ATRBT_T* 
                                                    pt_video_strm_info)
{
    switch(pt_video_strm_info->ui4_codec_id)
    {
    case dwBigFourCC('R', 'V', '3', '0'):
        pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_RV8;
        break;
    case dwBigFourCC('R', 'V', '4', '0'):
        pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_RV9;
        break;
    case dwBigFourCC('R', 'V', '1', '0'):
    case dwBigFourCC('R', 'V', '2', '0'):
    default:
        pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
        break;
    }
#if 0    
    if(pt_video_strm_info->ui2_frm_height < 144 || pt_video_strm_info->ui2_frm_width < 176)
    {
        pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
    }
#endif
}

static INT32 _rm_get_src_data(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0;
    UINT32                  ui4_size = 0;
    UINT32                  ui4_read_succ_data_num = 0;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj = NULL;
    
    pt_rm_info_obj     = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    x_memset(pt_rm_info_obj->pui1_buffer, 0, pt_rm_info_obj->ui4_buf_len);
    _rm_info_set_pos(pt_minfo_obj, 
               pt_rm_info_obj->ui8_curr_parser_offset, 
               FM_SEEK_BGN);
    i4_ret = _rm_info_copy_bytes(pt_minfo_obj, 
                         pt_rm_info_obj->pui1_buffer, 
                         (SIZE_T)pt_minfo_obj->z_buff_leng, 
                         8, 
                         &ui4_read_succ_data_num);
    if ((i4_ret != INPUTR_OK) || (ui4_read_succ_data_num < 8))
    {
        DBG_ERROR(("RM ERR: _rm_get_src_data pf_copybyte err, size=8!\r\n")); 
        return MINFOR_INTERNAL_ERR;
    }
    
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4, ui4_size);
    if(ui4_size < 8)
    {
        DBG_ERROR(("RM ERR: _rm_get_src_data ui4_size is wrong!\r\n")); 
        return MINFOR_INTERNAL_ERR;
    }
    if(ui4_size > pt_rm_info_obj->ui4_buf_len)
    {
        DBG_ERROR(("RM ERR: _rm_get_src_data buffer doesn't enough!\r\n")); 
        return MINFOR_INTERNAL_ERR;
    }
    ui4_size -= 8;
    i4_ret = _rm_info_copy_bytes(pt_minfo_obj, 
                         pt_rm_info_obj->pui1_buffer + 8, 
                         (SIZE_T)pt_minfo_obj->z_buff_leng, 
                         ui4_size, 
                         &ui4_read_succ_data_num);
    if ((i4_ret != INPUTR_OK) || (ui4_read_succ_data_num < ui4_size))
    {
        DBG_ERROR(("RM ERR: _rm_get_src_data pf_copybyte err, size=%d!\r\n", ui4_size)); 
        return MINFOR_INTERNAL_ERR;
    }
    return MINFOR_OK;
}

static INT32 _get_rmf_cont(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj = NULL;
    RM_FILE_HEADER_T*       pt_file_head = NULL;
    
    DBG_INFO(("RM INFO: _get_rmf_cont \r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_file_head = &pt_rm_info_obj->r_file_layout.t_file_head;

    i4_ret = _rm_get_src_data(pt_minfo_obj);
    if(MINFOR_OK != i4_ret)
    {
        return i4_ret;
    }
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer,      pt_file_head->ui4_object_id);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,  pt_file_head->ui4_size);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 8,  pt_file_head->ui2_object_version);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 10, pt_file_head->ui4_file_version);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 14, pt_file_head->ui4_num_headers);

    pt_rm_info_obj->ui8_curr_parser_offset += pt_file_head->ui4_size;
    DBG_INFO(("RM INFO: _get_rmf_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_prop_cont(MINFO_OBJ_T*    pt_minfo_obj)
{ 
    INT32                   i4_ret = 0;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj = NULL;
    RM_PROP_HEADER_T*       pt_file_property = NULL;
    
    DBG_INFO(("RM INFO: _get_prop_cont \r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_file_property = &pt_rm_info_obj->r_file_layout.t_file_property;

    i4_ret = _rm_get_src_data(pt_minfo_obj);
    if(MINFOR_OK != i4_ret)
    {
        return i4_ret;
    }

    LOADB_DWRD(pt_rm_info_obj->pui1_buffer,        pt_file_property->ui4_object_id);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,    pt_file_property->ui4_size);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 8,    pt_file_property->ui2_object_version);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 10,   pt_file_property->ui4_max_bit_rate);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 14,   pt_file_property->ui4_avg_bit_rate);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 18,   pt_file_property->ui4_max_packet_size);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 22,   pt_file_property->ui4_avg_packet_size);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 26,   pt_file_property->ui4_num_packets);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 30,   pt_file_property->ui4_duration);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 34,   pt_file_property->ui4_preroll);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 38,   pt_file_property->ui4_index_offset);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 42,   pt_file_property->ui4_data_offset);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 46,   pt_file_property->ui2_num_streams);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 48,   pt_file_property->ui2_flags);

    pt_rm_info_obj->ui8_curr_parser_offset += pt_file_property->ui4_size;
    DBG_INFO(("RM INFO: _get_prop_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_cont_cont(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                        i4_ret = 0;
    UINT32                          ui4_offset = 0;
    UINT8*                       pui1_tmp_buf = NULL;
    MINFO_RM_FILE_INF_T*         pt_rm_info_obj = NULL;
    RM_MEDIA_CONT_DESCRIPTION_T* pt_file_cont_desp = NULL;
    
    DBG_INFO(("RM INFO: _get_cont_cont \r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_file_cont_desp = &pt_rm_info_obj->r_file_layout.t_file_cont_desp;

    i4_ret = _rm_get_src_data(pt_minfo_obj);
    if(MINFOR_OK != i4_ret)
    {
        return i4_ret;
    }

    LOADB_DWRD(pt_rm_info_obj->pui1_buffer,        pt_file_cont_desp->ui4_object_id);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,    pt_file_cont_desp->ui4_size);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 8,    pt_file_cont_desp->ui2_object_version);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 10,   pt_file_cont_desp->ui2_title_len);

    if(pt_file_cont_desp->ui2_object_version != 0)
    {
        DBG_INFO(("RM INFO: _get_cont_cont succeed\r\n")); 
        return MINFOR_OK;
    }
    ui4_offset += 12;
    /*get title*/
    if(pt_file_cont_desp->ui2_title_len != 0)
    {
        if(ui4_offset + pt_file_cont_desp->ui2_title_len > 
            pt_file_cont_desp->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont ui2_title_len err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_file_cont_desp->ui2_title_len);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_file_cont_desp->pui1_title = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_file_cont_desp->ui2_title_len);
    }
    ui4_offset += pt_file_cont_desp->ui2_title_len;

    /*get author length and string*/
    pui1_tmp_buf = NULL;
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + ui4_offset,    
               pt_file_cont_desp->ui2_author_len);  
    ui4_offset += 2;
    if(pt_file_cont_desp->ui2_author_len != 0)
    {
        if(ui4_offset + pt_file_cont_desp->ui2_author_len > 
            pt_file_cont_desp->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont ui2_author_len err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_file_cont_desp->ui2_author_len);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_file_cont_desp->pui1_author = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_file_cont_desp->ui2_author_len);
    }
    ui4_offset += pt_file_cont_desp->ui2_author_len;

    /* get copyright length and string */
    pui1_tmp_buf = NULL;
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + ui4_offset,    
               pt_file_cont_desp->ui2_copyright_len);  
    ui4_offset += 2;
    if(pt_file_cont_desp->ui2_copyright_len != 0)
    {
        if(ui4_offset + pt_file_cont_desp->ui2_copyright_len > 
            pt_file_cont_desp->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont ui2_copyright_len err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_file_cont_desp->ui2_copyright_len);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_file_cont_desp->pui1_copyright = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_file_cont_desp->ui2_copyright_len);
    }
    ui4_offset += pt_file_cont_desp->ui2_copyright_len;

    /*get comment length and string*/
    pui1_tmp_buf = NULL;
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + ui4_offset,    
               pt_file_cont_desp->ui2_comment_len);  
    ui4_offset += 2;
    if(pt_file_cont_desp->ui2_comment_len != 0)
    {
        if(ui4_offset + pt_file_cont_desp->ui2_comment_len > 
            pt_file_cont_desp->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont ui2_comment_len err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_file_cont_desp->ui2_comment_len);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_cont_cont malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_file_cont_desp->pui1_comment = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_file_cont_desp->ui2_comment_len);
    }
    
    pt_rm_info_obj->ui8_curr_parser_offset += pt_file_cont_desp->ui4_size;
    DBG_INFO(("RM INFO: _get_cont_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_mdpr_cont(MINFO_OBJ_T*    pt_minfo_obj)
{   
    INT32                   i4_ret = 0;
    UINT32                  ui4_offset = 0;
    UINT8*                  pui1_tmp_buf = NULL;
    UINT8                   ui1_strm_id = 0;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj = NULL;
    RM_MEDIA_PROPERTIY_T*   pt_strm_property = NULL;
    
    DBG_INFO(("RM INFO: _get_mdpr_cont\r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    i4_ret = _rm_get_src_data(pt_minfo_obj);
    if(MINFOR_OK != i4_ret)
    {
        return i4_ret;
    }

    ui1_strm_id = pt_rm_info_obj->r_file_layout.t_stream_num.ui1_video_cnt +
                  pt_rm_info_obj->r_file_layout.t_stream_num.ui1_audio_cnt
#ifdef RM_LOGICAL_STREAM_SUPPORT
                   + pt_rm_info_obj->r_file_layout.t_stream_num.ui1_logic_video_cnt
                   + pt_rm_info_obj->r_file_layout.t_stream_num.ui1_logic_audio_cnt
#endif
                  ;
    pt_strm_property = 
        &pt_rm_info_obj->r_file_layout.r_strm_property[ui1_strm_id];
    pt_strm_property->u.r_rm_video_param.ui4_specific = 0;
    
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer,         pt_strm_property->ui4_object_id);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,     pt_strm_property->ui4_size);  
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 8,     pt_strm_property->ui2_object_version);  
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 10,    pt_strm_property->ui2_stream_number);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 12,    pt_strm_property->ui4_max_bit_rate);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 16,    pt_strm_property->ui4_avg_bit_rate);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 20,    pt_strm_property->ui4_max_packet_size);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 24,    pt_strm_property->ui4_avg_packet_size);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 28,    pt_strm_property->ui4_start_time);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 32,    pt_strm_property->ui4_preroll);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 36,    pt_strm_property->ui4_duration);  
    LOAD_BYTE (pt_rm_info_obj->pui1_buffer + 40,    pt_strm_property->ui1_stream_name_size);  
    if(pt_rm_info_obj->r_file_layout.t_file_property.ui4_duration == 0 &&
        pt_strm_property->ui4_duration != 0)
    {
        pt_rm_info_obj->r_file_layout.t_file_property.ui4_duration
            = pt_strm_property->ui4_duration;
    }

    ui4_offset += 41;

    /*get stream name length and string */
    if(pt_strm_property->ui1_stream_name_size != 0)
    {
        if(ui4_offset + pt_strm_property->ui1_stream_name_size > 
            pt_strm_property->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui1_stream_name_size err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_strm_property->ui1_stream_name_size);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui1_stream_name_size malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_strm_property->pui1_stream_name = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_strm_property->ui1_stream_name_size);
    }
    ui4_offset += pt_strm_property->ui1_stream_name_size;

    /*get mime type_size length and string*/
    pui1_tmp_buf = NULL;
    LOAD_BYTE(pt_rm_info_obj->pui1_buffer + ui4_offset,    
              pt_strm_property->ui1_mime_type_size);  
    ui4_offset ++;
    if(pt_strm_property->ui1_mime_type_size != 0)
    {
        if(ui4_offset + pt_strm_property->ui1_mime_type_size > 
            pt_strm_property->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui1_mime_type_size err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_strm_property->ui1_mime_type_size);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui1_mime_type_size malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_strm_property->pui1_mime_type = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_strm_property->ui1_mime_type_size);
        /* "video/x-pn-realvideo" */
        if(x_strncmp((CHAR*)pui1_tmp_buf, "logical-fileinfo", 16) == 0)
        {
            /*Do nothing*/
        }
#ifdef RM_LOGICAL_STREAM_SUPPORT
        else if(x_strncmp((CHAR*)pui1_tmp_buf, "logical-audio", 13) == 0)
        {
            pt_strm_property->e_rm_strm_type = RM_STRM_TYPE_LOGIC_AUDIO;
            pt_rm_info_obj->r_file_layout.t_stream_num.ui1_logic_audio_cnt++;
        }
        else if(x_strncmp((CHAR*)pui1_tmp_buf, "logical-video", 13) == 0)
        {
            pt_strm_property->e_rm_strm_type = RM_STRM_TYPE_LOGIC_VIDEO;
            pt_rm_info_obj->r_file_layout.t_stream_num.ui1_logic_video_cnt++;
        }
#else
        else if(x_strncmp((CHAR*)pui1_tmp_buf, "logical", 7) == 0)
        {   
            /*as codec not support*/
            UINT16 ui2_temp;
            for(ui2_temp = 0; ui2_temp < RM_STREAM_MAX; ui2_temp++)
            {
                pt_rm_info_obj->r_file_layout.r_strm_property[(UINT8)ui2_temp].
                    u.r_rm_video_param.e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
            }
            pt_strm_property->e_rm_strm_type = RM_STRM_TYPE_LOGIC_VIDEO;
            return MINFOR_INTERNAL_ERR;
        }
#endif
        else if(x_strncmp((CHAR*)pui1_tmp_buf, "video", 5) == 0)
        {
            pt_strm_property->e_rm_strm_type = RM_STRM_TYPE_VIDEO;
            pt_rm_info_obj->r_file_layout.t_stream_num.ui1_video_cnt++;
        }
        /* "audio/x-pn-realaudio" */
        else if(x_strncmp((CHAR*)pui1_tmp_buf, "audio", 5) == 0)
        {
            pt_strm_property->e_rm_strm_type = RM_STRM_TYPE_AUDIO;
            pt_rm_info_obj->r_file_layout.t_stream_num.ui1_audio_cnt++;
        }
    }
    ui4_offset += pt_strm_property->ui1_mime_type_size;
    if(pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_VIDEO)
    {
        LOADB_WORD(pt_rm_info_obj->pui1_buffer + 10, 
            pt_strm_property->u.r_rm_video_param.ui2_strm_num);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 12, 
            pt_strm_property->u.r_rm_video_param.ui4_max_bit_rate);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 16, 
            pt_strm_property->u.r_rm_video_param.ui4_avg_bit_rate);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 20, 
            pt_strm_property->u.r_rm_video_param.ui4_max_packet_size);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 24, 
            pt_strm_property->u.r_rm_video_param.ui4_avg_packet_size);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 28, 
            pt_strm_property->u.r_rm_video_param.ui4_start_time);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 32, 
            pt_strm_property->u.r_rm_video_param.ui4_preroll);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 36, 
            pt_strm_property->u.r_rm_video_param.ui4_duration);  
    }
    else if(pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_AUDIO)
    {
        LOADB_WORD(pt_rm_info_obj->pui1_buffer + 10, 
            pt_strm_property->u.r_rm_audio_v5_param.ui2_strm_num);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 12, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_max_bit_rate);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 16, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_avg_bit_rate);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 20, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_max_packet_size);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 24, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_avg_packet_size);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 28, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_start_time);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 32, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_preroll);  
        LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 36, 
            pt_strm_property->u.r_rm_audio_v5_param.ui4_duration);  
    }

    /*get type specific length and string*/
    pui1_tmp_buf = NULL;
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + ui4_offset,    
              pt_strm_property->ui4_type_specific_len);  
    ui4_offset += 4;
    if(pt_strm_property->ui4_type_specific_len != 0)
    {
        if(ui4_offset + pt_strm_property->ui4_type_specific_len > 
            pt_strm_property->ui4_size)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui4_type_specific_len err!\r\n")); 
            return MINFOR_INTERNAL_ERR;
        }
        pui1_tmp_buf = (UINT8*)_mm_util_mem_alloc(
            pt_strm_property->ui4_type_specific_len);
        if(pui1_tmp_buf == NULL)
        {
            DBG_ERROR(("RM ERR: _get_mdpr_cont ui4_type_specific_len malloc err!\r\n")); 
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_strm_property->pui1_type_specific_data = pui1_tmp_buf;
        x_memcpy(pui1_tmp_buf, 
                 pt_rm_info_obj->pui1_buffer + ui4_offset, 
                 pt_strm_property->ui4_type_specific_len);
        if(pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_VIDEO)
        {
            MINFO_RM_VIDEO_STM_ATRBT_T* pr_rm_video_parm;
            UINT32 ui4_major_ver;
            UINT32 ui4_minor_ver;
            
            pr_rm_video_parm = &pt_strm_property->u.r_rm_video_param;
            if(pt_strm_property->ui4_type_specific_len < 34)
            {
                DBG_ERROR(("RM ERR: _get_mdpr_cont video spec info err!\r\n")); 
                return MINFOR_INTERNAL_ERR;
            }
            LOADB_DWRD(pui1_tmp_buf + 8,  pr_rm_video_parm->ui4_codec_id);  
            LOADB_WORD(pui1_tmp_buf + 12, pr_rm_video_parm->ui2_frm_width);  
            LOADB_WORD(pui1_tmp_buf + 14, pr_rm_video_parm->ui2_frm_height);   
            LOADB_WORD(pui1_tmp_buf + 16, pr_rm_video_parm->ui2_bit_count);   
            LOADB_WORD(pui1_tmp_buf + 18, pr_rm_video_parm->ui2_pad_width);  
            LOADB_WORD(pui1_tmp_buf + 20, pr_rm_video_parm->ui2_pad_height);   
            LOADB_DWRD(pui1_tmp_buf + 22, pr_rm_video_parm->ui4_frm_rate);   
            LOADB_DWRD(pui1_tmp_buf + 26, pr_rm_video_parm->ui4_spo_extra_flags);  
            LOADB_DWRD(pui1_tmp_buf + 30, pr_rm_video_parm->ui4_version);  
            if (pr_rm_video_parm->ui4_codec_id == 0x52565452)  /*fix values from too-old encoders*/
            {
                pr_rm_video_parm->ui4_codec_id = 0x52563230;
            }
            else if (pr_rm_video_parm->ui4_codec_id == 0x52565432)
            {
                pr_rm_video_parm->ui4_codec_id = 0x52563330;
            }
            _rm_get_vid_codec(pr_rm_video_parm);
            ui4_major_ver = HX_GET_MAJOR_VERSION(pr_rm_video_parm->ui4_version);
            ui4_minor_ver = HX_GET_MINOR_VERSION(pr_rm_video_parm->ui4_version);
            if (!(ui4_minor_ver & 0x80))
            {
                if (ui4_major_ver == 3 && ui4_minor_ver == 2)
                {
                #if 1
                    UINT32 ui4_num_rpr_size;
                    UINT32 i;
                    UINT8  ui1_val;
                    UINT8* pbBasePtr;
                    pbBasePtr = pui1_tmp_buf + 34;
                    ui4_num_rpr_size 
                        = (pr_rm_video_parm->ui4_spo_extra_flags & 0x70000) >> 16;
                    if(ui4_num_rpr_size <= 8)
                    {
                        _au4RprSizeArray = (UINT32*)x_mem_alloc
                            (2*(ui4_num_rpr_size+1)*sizeof(UINT32));
                        if(!_au4RprSizeArray)
                        {
                            return MINFOR_INTERNAL_ERR;
                        }
                        if(pt_strm_property->ui4_type_specific_len >= ui4_num_rpr_size + 34)
                        {
                            for (i = 0; i < ui4_num_rpr_size; i++)
                            {
                                LOAD_BYTE(pbBasePtr, ui1_val);
                                _au4RprSizeArray[2 * i + 2] = (UINT32)(ui1_val << 2); 
                                pbBasePtr++;
                                LOAD_BYTE(pbBasePtr, ui1_val);
                                _au4RprSizeArray[2 * i + 3] = (UINT32)(ui1_val << 2); 
                                pbBasePtr++;
                            }
                        }
                        pr_rm_video_parm->ui4_specific = (UINT32)_au4RprSizeArray;
                        _au4RprSizeArray[0] = pr_rm_video_parm->ui2_frm_width;
                        _au4RprSizeArray[1] = pr_rm_video_parm->ui2_frm_height;
                    }
                #endif
                    pr_rm_video_parm->ui1_ecc_mask = 0x20;
                }
                else if (ui4_major_ver == 4 && ui4_minor_ver == 0)
                {
                    if(pt_strm_property->ui4_type_specific_len >= 38)
                    {
                        LOADB_DWRD(pui1_tmp_buf + 34, pr_rm_video_parm->ui4_specific); 
                    }
                    pr_rm_video_parm->ui1_ecc_mask = 0x80;
                }
            }
        }
        else if(pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_AUDIO)
        {
            MINFO_RM_AUDIO_STM_ATRBT_T* pr_rm_audio_v5_parm;
            UINT32 ui4_num_frm_in_super_blk = 0;
            UINT16 ui2_val;
            
            LOADB_WORD(pui1_tmp_buf + 4, ui2_val); 
            if(pt_strm_property->ui4_type_specific_len < 73)
            {
                DBG_ERROR(("RM ERR: _get_mdpr_cont audio spec info err!\r\n")); 
                return MINFOR_INTERNAL_ERR;
            }
            pr_rm_audio_v5_parm = &pt_strm_property->u.r_rm_audio_v5_param;
            LOADB_WORD(pui1_tmp_buf + 16, pr_rm_audio_v5_parm->ui2_version);  
            LOADB_WORD(pui1_tmp_buf + 18, pr_rm_audio_v5_parm->ui2_revision);  
            LOADB_WORD(pui1_tmp_buf + 20, pr_rm_audio_v5_parm->ui2_hdr_bytes);  
            LOADB_WORD(pui1_tmp_buf + 22, pr_rm_audio_v5_parm->ui2_flavor_idx);  
            LOADB_DWRD(pui1_tmp_buf + 24, pr_rm_audio_v5_parm->ui4_granularity);  
            LOADB_DWRD(pui1_tmp_buf + 28, pr_rm_audio_v5_parm->ui4_total_bytes);  
            LOADB_DWRD(pui1_tmp_buf + 32, 
                pr_rm_audio_v5_parm->ui4_bytes_per_min);  
            LOADB_DWRD(pui1_tmp_buf + 36, 
                pr_rm_audio_v5_parm->ui4_bytes_per_min2);  
            LOADB_WORD(pui1_tmp_buf + 40, 
                pr_rm_audio_v5_parm->ui2_intlv_factor);  
            LOADB_WORD(pui1_tmp_buf + 42, 
                pr_rm_audio_v5_parm->ui2_intlv_blk_sz);  
            LOADB_WORD(pui1_tmp_buf + 44, 
                pr_rm_audio_v5_parm->ui2_codec_frm_sz);  
            LOADB_DWRD(pui1_tmp_buf + 46, pr_rm_audio_v5_parm->ui4_user_data);  
            LOADB_DWRD(pui1_tmp_buf + 50, pr_rm_audio_v5_parm->ui4_sample_rate);  
            LOADB_DWRD(pui1_tmp_buf + 54, 
                pr_rm_audio_v5_parm->ui4_act_sample_rate);  
            LOADB_WORD(pui1_tmp_buf + 58, pr_rm_audio_v5_parm->ui2_sample_sz);  
            LOADB_WORD(pui1_tmp_buf + 60, pr_rm_audio_v5_parm->ui2_num_channel);  
            LOADB_DWRD(pui1_tmp_buf + 62, pr_rm_audio_v5_parm->ui4_intlvr_id);  
            LOADB_DWRD(pui1_tmp_buf + 66, pr_rm_audio_v5_parm->ui4_codec_id);  
            LOAD_BYTE (pui1_tmp_buf + 70, pr_rm_audio_v5_parm->ui1_intlv_flag);  
            LOAD_BYTE (pui1_tmp_buf + 71, 
                pr_rm_audio_v5_parm->ui1_can_copy_flag);  
            LOAD_BYTE (pui1_tmp_buf + 72, pr_rm_audio_v5_parm->ui1_strm_type);            
            _rm_get_aud_codec(pr_rm_audio_v5_parm);
            if(ui2_val == 5) /*version 5 format*/
            {
                if(pt_strm_property->ui4_type_specific_len < 78)
                {
                    DBG_ERROR(("RM ERR: _get_mdpr_cont audio spec info err!\r\n")); 
                    return MINFOR_INTERNAL_ERR;
                }
                LOAD_BYTE (pui1_tmp_buf + 73, 
                    pr_rm_audio_v5_parm->ui1_has_intlv_ptrn);  
                if(pr_rm_audio_v5_parm->ui1_has_intlv_ptrn)
                {
                    if(pr_rm_audio_v5_parm->ui2_codec_frm_sz == 0)
                    {
                        return MINFOR_INTERNAL_ERR;
                    }
                    ui4_num_frm_in_super_blk = 
                        pr_rm_audio_v5_parm->ui2_intlv_factor
                        * pr_rm_audio_v5_parm->ui2_intlv_blk_sz 
                        / pr_rm_audio_v5_parm->ui2_codec_frm_sz;
                    pr_rm_audio_v5_parm->pv_intlv_ptrn = (UINT8*)
                        _mm_util_mem_alloc(ui4_num_frm_in_super_blk * 2);
                    if(pr_rm_audio_v5_parm->pv_intlv_ptrn == NULL)
                    {
                        DBG_ERROR(("RM ERR: _get_mdpr_cont audio spec info malloc err!\r\n")); 
                        return MINFOR_OUT_OF_MEMORY;
                    }
                    x_memcpy((UINT8*)pr_rm_audio_v5_parm->pv_intlv_ptrn, 
                        pui1_tmp_buf + 74, 
                        ui4_num_frm_in_super_blk * 2);
                }
                else
                {
                    pr_rm_audio_v5_parm->pv_intlv_ptrn = NULL;
                }
                LOADB_DWRD(pui1_tmp_buf + 74 + ui4_num_frm_in_super_blk * 2, 
                    pr_rm_audio_v5_parm->ui4_opaque_data_sz); 
                if(pr_rm_audio_v5_parm->ui4_opaque_data_sz)
                {
                    UINT8* pui1_opaque_data = NULL;
                    pr_rm_audio_v5_parm->pv_codec_opaque_data = (UINT8*)
                      _mm_util_mem_alloc(
                      pr_rm_audio_v5_parm->ui4_opaque_data_sz);
                    if(pr_rm_audio_v5_parm->pv_codec_opaque_data == NULL)
                    {
                        DBG_ERROR(("RM ERR: _get_mdpr_cont audio spec info malloc err!\r\n")); 
                        return MINFOR_OUT_OF_MEMORY;
                    }
                    x_memcpy((UINT8*)pr_rm_audio_v5_parm->pv_codec_opaque_data, 
                            pui1_tmp_buf + 78 + ui4_num_frm_in_super_blk * 2, 
                            pr_rm_audio_v5_parm->ui4_opaque_data_sz);
                    pui1_opaque_data = 
                        (UINT8*)pr_rm_audio_v5_parm->pv_codec_opaque_data;
                    if(*pui1_opaque_data == 0x02)
                    {
                        ga_config_get_data(pr_rm_audio_v5_parm, 
                            (pui1_opaque_data + 1), 
                            (pr_rm_audio_v5_parm->ui4_opaque_data_sz - 1) * 8);
                    }
                }
                else
                {
                    pr_rm_audio_v5_parm->pv_codec_opaque_data = NULL;
                }
            }
        }
#ifdef RM_LOGICAL_STREAM_SUPPORT
        else if(pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_LOGIC_VIDEO ||
                pt_strm_property->e_rm_strm_type == RM_STRM_TYPE_LOGIC_AUDIO)
        {
            MINFO_RM_LOGIC_STM_STRUC_T* pr_rm_logic;
            RM_NAME_VAL_PROPERTY_T*     pr_val_prop;
            UINT16  ui2_idx;
            ui4_offset = 0;
            
            pr_rm_logic = &pt_strm_property->u.r_rm_logic_strm;

            LOADB_DWRD(pui1_tmp_buf,        pr_rm_logic->ui4_size);  
            LOADB_WORD(pui1_tmp_buf + 4,    pr_rm_logic->ui2_version);  
            ui4_offset += 6;
            if(pr_rm_logic->ui2_version == 0)
            {
                LOADB_WORD(pui1_tmp_buf + ui4_offset, pr_rm_logic->ui2_num_strms);  
                ui4_offset += 2;

                /*stream numbers*/
                pr_rm_logic->pui2_strm_nums = (UINT16*)_mm_util_mem_alloc(2*pr_rm_logic->ui2_num_strms);
                if(pr_rm_logic->pui2_strm_nums == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                for(ui2_idx = 0; ui2_idx < pr_rm_logic->ui2_num_strms; ui2_idx++, ui4_offset += 2)
                {
                    LOADB_WORD(pui1_tmp_buf + ui4_offset,    pr_rm_logic->pui2_strm_nums[ui2_idx]);  
                }
                
                /*stream data offsets*/
                pr_rm_logic->pui4_data_offs = (UINT32*)_mm_util_mem_alloc(4*pr_rm_logic->ui2_num_strms);
                if(pr_rm_logic->pui4_data_offs == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                for(ui2_idx = 0; ui2_idx < pr_rm_logic->ui2_num_strms; ui2_idx++, ui4_offset += 4)
                {
                    LOADB_DWRD(pui1_tmp_buf + ui4_offset,    pr_rm_logic->pui4_data_offs[ui2_idx]);  
                }

                LOADB_WORD(pui1_tmp_buf + ui4_offset,pr_rm_logic->ui2_num_rules);  
                ui4_offset += 2;
                
                /*rule to steam number map*/
                pr_rm_logic->pui2_rule_strm_num_map = (UINT16*)_mm_util_mem_alloc(2*pr_rm_logic->ui2_num_rules);
                if(pr_rm_logic->pui2_rule_strm_num_map == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                for(ui2_idx = 0; ui2_idx < pr_rm_logic->ui2_num_rules; ui2_idx++, ui4_offset += 2)
                {
                    LOADB_WORD(pui1_tmp_buf + ui4_offset,    pr_rm_logic->pui2_rule_strm_num_map[ui2_idx]);  
                }
                
                LOADB_WORD(pui1_tmp_buf + ui4_offset,pr_rm_logic->ui2_num_properties);  
                ui4_offset += 2;
                
                /*name value properties*/
                pr_rm_logic->pt_name_val_properties = (RM_NAME_VAL_PROPERTY_T*)_mm_util_mem_alloc(
                    sizeof(RM_NAME_VAL_PROPERTY_T)*pr_rm_logic->ui2_num_properties);
                if(pr_rm_logic->pt_name_val_properties == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                pr_val_prop = (RM_NAME_VAL_PROPERTY_T*)pr_rm_logic->pt_name_val_properties;
                for(ui2_idx = 0; ui2_idx < pr_rm_logic->ui2_num_properties; ui2_idx++)
                {
                    LOADB_DWRD(pui1_tmp_buf + ui4_offset,     pr_val_prop->ui4_size);  
                    LOADB_WORD(pui1_tmp_buf + ui4_offset + 4, pr_val_prop->ui2_version);  
                    ui4_offset += 6;
                    
                    if(pr_val_prop->ui2_version == 0)
                    {
                        LOAD_BYTE(pui1_tmp_buf + ui4_offset, pr_val_prop->ui1_name_len);  
                        ui4_offset ++;
                        
                        /*name*/
                        pr_val_prop->pui1_name = (UINT8*)_mm_util_mem_alloc(pr_val_prop->ui1_name_len);
                        if(pr_val_prop->pui1_name == NULL)
                        {
                            return MINFOR_OUT_OF_MEMORY;
                        }
                        x_memcpy(pr_val_prop->pui1_name, 
                                pui1_tmp_buf + ui4_offset, 
                                pr_val_prop->ui1_name_len);
                        ui4_offset += pr_val_prop->ui1_name_len;

                        LOADB_DWRD(pui1_tmp_buf + ui4_offset,     pr_val_prop->i4_type);  
                        LOADB_WORD(pui1_tmp_buf + ui4_offset + 4, pr_val_prop->ui2_val_len);  
                        ui4_offset += 6;
                        
                        /*value*/
                        pr_val_prop->pui1_val_data = (UINT8*)_mm_util_mem_alloc(pr_val_prop->ui2_val_len);
                        if(pr_val_prop->pui1_val_data == NULL)
                        {
                            return MINFOR_OUT_OF_MEMORY;
                        }
                        x_memcpy(pr_val_prop->pui1_val_data, 
                                pui1_tmp_buf + ui4_offset, 
                                pr_val_prop->ui2_val_len);
                        ui4_offset += pr_val_prop->ui2_val_len;
                    }
                    
                    pr_val_prop = (RM_NAME_VAL_PROPERTY_T*)((UINT8*)pr_val_prop + sizeof(RM_NAME_VAL_PROPERTY_T));
                }
            }
        }
#endif
    }
    
    pt_rm_info_obj->ui8_curr_parser_offset += pt_strm_property->ui4_size;
    DBG_INFO(("RM INFO: _get_mdpr_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_data_cont(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret = 0;
    UINT32                  ui4_read_succ_data_num = 0;
    RM_DATA_CHUNK_HEADER_T* pt_file_data_head = NULL;
    MINFO_RM_FILE_INF_T*      pt_rm_info_obj;
    
    DBG_INFO(("RM INFO: _get_data_cont\r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    pt_file_data_head = 
        &(pt_rm_info_obj->r_file_layout.t_file_data[pt_rm_info_obj->r_file_layout.t_stream_num.ui1_data_cnt].t_data_block_head);
    pt_rm_info_obj->r_file_layout.t_stream_num.ui1_data_cnt++;

    _rm_info_set_pos(pt_minfo_obj, 
                pt_rm_info_obj->ui8_curr_parser_offset, 
                FM_SEEK_BGN);
    i4_ret = _rm_info_copy_bytes(pt_minfo_obj, 
                         pt_rm_info_obj->pui1_buffer, 
                         (SIZE_T)pt_minfo_obj->z_buff_leng, 
                         RM_DATA_HEADER_SIZE, 
                         &ui4_read_succ_data_num);
    if ((i4_ret != INPUTR_OK)
        || (ui4_read_succ_data_num < RM_DATA_HEADER_SIZE))
    {
        return MINFOR_INTERNAL_ERR;
    }

    pt_file_data_head->ui4_offset = (UINT32)pt_rm_info_obj->ui8_curr_parser_offset;
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 0,   pt_file_data_head->ui4_object_id);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,   pt_file_data_head->ui4_size);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 8,   pt_file_data_head->ui2_object_version);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 10,  pt_file_data_head->ui4_num_packets);  
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 14,  pt_file_data_head->ui4_next_data_header);  

    pt_rm_info_obj->ui8_curr_parser_offset += pt_file_data_head->ui4_size;
    DBG_INFO(("RM INFO: _get_data_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_indx_cont(MINFO_OBJ_T*    pt_minfo_obj)
{           
    INT32                       i4_ret = 0;
    UINT32                      ui4_read_succ_data_num = 0;
    UINT32                      ui4_num_records = 0;
    UINT16                      ui2_strm_index = 0;
    RM_INDEX_CHUNK_HEADER_T*    pt_file_indx_head;
    RM_INDEX_RECORD_T*          pt_index_records;
    MINFO_RM_FILE_INF_T*        pt_rm_info_obj;
    
    DBG_INFO(("RM INFO: _get_indx_cont\r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    _rm_info_set_pos(pt_minfo_obj, 
                pt_rm_info_obj->ui8_curr_parser_offset, 
                FM_SEEK_BGN);

    /*read INDX head  */
    i4_ret = _rm_info_copy_bytes(pt_minfo_obj, 
                         pt_rm_info_obj->pui1_buffer, 
                         (SIZE_T)pt_minfo_obj->z_buff_leng, 
                         RM_INDEX_HEADER_SIZE,
                         &ui4_read_succ_data_num);
    if ((i4_ret != INPUTR_OK) || (ui4_read_succ_data_num < RM_INDEX_HEADER_SIZE))
    {
        return MINFOR_INTERNAL_ERR;
    }
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 14,   ui2_strm_index);
    pt_file_indx_head = &pt_rm_info_obj->r_file_layout
                        .r_strm_indx_tbl[(UINT8)ui2_strm_index].t_index_block_head;
    pt_file_indx_head->ui4_cur_index_header = (UINT32)pt_rm_info_obj->ui8_curr_parser_offset;
    
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 0,    pt_file_indx_head->ui4_object_id);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,    pt_file_indx_head->ui4_size);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 8,    pt_file_indx_head->ui2_object_version);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 10,   pt_file_indx_head->ui4_num_indices);
    LOADB_WORD(pt_rm_info_obj->pui1_buffer + 14,   pt_file_indx_head->ui2_stream_number);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 16,   pt_file_indx_head->ui4_next_index_header);

    pt_rm_info_obj->r_file_layout.t_stream_num.ui1_idx_cnt++;
    ui4_num_records = pt_file_indx_head->ui4_num_indices;
    /*read INDX records*/
    if(pt_file_indx_head->ui4_num_indices != 0)
    {
        UINT8* pui1_temp = NULL;
        /*Here +1 to avoid overlap of the last two records*/
        pt_index_records = (RM_INDEX_RECORD_T*)_mm_util_mem_alloc(
                            sizeof(RM_INDEX_RECORD_T) * (ui4_num_records+1));
        if(pt_index_records == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_rm_info_obj->r_file_layout.r_strm_indx_tbl[(UINT8)ui2_strm_index]
                                    .pt_index_block_records = pt_index_records;
        
        pui1_temp = (UINT8*)pt_index_records;
        
        i4_ret = _rm_info_copy_bytes(pt_minfo_obj, 
                             pui1_temp + (sizeof(RM_INDEX_RECORD_T)
                             - RM_INDEX_RECORD_SIZE) * ui4_num_records
                             + RM_INDEX_RECORD_SIZE, 
                             (SIZE_T)pt_minfo_obj->z_buff_leng, 
                             RM_INDEX_RECORD_SIZE * ui4_num_records, 
                             &ui4_read_succ_data_num);
        if ((i4_ret != INPUTR_OK)
            || (ui4_read_succ_data_num < RM_INDEX_RECORD_SIZE*ui4_num_records))
        {
            return MINFOR_INTERNAL_ERR;
        }
        /*this is a trick to save mem*/
        pui1_temp = (UINT8*)pt_index_records;
        pui1_temp += (sizeof(RM_INDEX_RECORD_T)- RM_INDEX_RECORD_SIZE)
                    * ui4_num_records + RM_INDEX_RECORD_SIZE;
        for(ui4_num_records = pt_file_indx_head->ui4_num_indices; 
            ui4_num_records > 0; 
            ui4_num_records--)
        {
            if(ui4_num_records == 2)
            {
                i4_ret = 0;
            }
            LOADB_WORD(pui1_temp, pt_index_records->ui2_object_version);
            LOADB_DWRD(pui1_temp + 2, pt_index_records->ui4_timestamp);
            LOADB_DWRD(pui1_temp + 6, pt_index_records->ui4_offset);
            LOADB_DWRD(pui1_temp + 10,pt_index_records->ui4_cur_packet_count);
            pui1_temp += RM_INDEX_RECORD_SIZE;
            pt_index_records++;
        }
    }
    
    pt_rm_info_obj->ui8_curr_parser_offset += pt_file_indx_head->ui4_size;
    DBG_INFO(("RM INFO: _get_indx_cont succeed!\r\n")); 
    return MINFOR_OK;
}

static INT32 _get_skip_cont(MINFO_OBJ_T*    pt_minfo_obj)
{           
    INT32                   i4_ret = 0;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj = NULL;
    MINFO_RM_SKIP_INF_T     t_skip_data = {0};
    
    DBG_INFO(("RM INFO: _get_skip_cont \r\n")); 
    pt_rm_info_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    i4_ret = _rm_get_src_data(pt_minfo_obj);
    if(MINFOR_OK != i4_ret)
    {
        return i4_ret;
    }
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer,      t_skip_data.ui4_object_id);
    LOADB_DWRD(pt_rm_info_obj->pui1_buffer + 4,  t_skip_data.ui4_size);
    
    pt_rm_info_obj->ui8_curr_parser_offset += t_skip_data.ui4_size;
    DBG_INFO(("RM INFO: _get_skip_cont succeed!\r\n")); 
    return MINFOR_OK;
}

INT32 rm_minfo_box_parse(MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                     i4_ret = 0;
    INT32                      i4_state = MINFOR_OK;
    UINT32                     ui4_box_id = 0;
    input4bytes_fct         pf_inp4byte;
    MINFO_RM_FILE_INF_T*    pt_rm_info_obj;
    MEDIA_FEEDER_T          t_feeder;
    
    DBG_INFO(("RM INFO: rm_minfo_box_parse\r\n")); 
    pf_inp4byte     = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_rm_info_obj     = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 
    
    while(1)
    {
    	if (pt_rm_info_obj->ui8_curr_parser_offset >= pt_rm_info_obj->t_generic_info.ui8_media_sz)
		{
			DBG_INFO(("\n----------parse file end! offset = %llu, filesize = %llu----------\n", 
				pt_rm_info_obj->ui8_curr_parser_offset, pt_rm_info_obj->t_generic_info.ui8_media_sz));
			break;
		}
        i4_ret = _rm_info_set_pos(pt_minfo_obj, 
                    pt_rm_info_obj->ui8_curr_parser_offset, 
                    FM_SEEK_BGN);
        if(pt_minfo_obj->h_src_feeder != NULL_HANDLE)
        {
            UINT8 *aui1_buf;
            UINT8  ui1_tmp;
            t_feeder.h_feeder = pt_minfo_obj->h_src_feeder;
            i4_ret = pf_inp4byte(&t_feeder, &ui4_box_id);
            aui1_buf = (UINT8*)&ui4_box_id;
            ui1_tmp = aui1_buf[0];
            aui1_buf[0] = aui1_buf[3];
            aui1_buf[3] = ui1_tmp;
            ui1_tmp = aui1_buf[1];
            aui1_buf[1] = aui1_buf[2];
            aui1_buf[2] = ui1_tmp;
        }
        else
        {
            i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_box_id);
        }
        if(i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF || i4_ret == FMR_EOF)
            {
                break;
            }
            return MINFOR_INTERNAL_ERR;
        }
        switch(ui4_box_id)
        { 
        case RM_BOX_RMF:
            i4_state = _get_rmf_cont(pt_minfo_obj);
            break;
        case RM_BOX_PROP:
            i4_state = _get_prop_cont(pt_minfo_obj);
            break;
        case RM_BOX_CONT:
            i4_state = _get_cont_cont(pt_minfo_obj);
            break;
        case RM_BOX_MDPR:
            i4_state = _get_mdpr_cont(pt_minfo_obj);
            break;
        case RM_BOX_DATA:
            i4_state = _get_data_cont(pt_minfo_obj);
            break;
        case RM_BOX_INDX:
            i4_state = _get_indx_cont(pt_minfo_obj);
            break;
        default:
            i4_state = _get_skip_cont(pt_minfo_obj);
            break;
        }/* end switch */
        if(i4_state != MINFOR_OK)
        {
            break;
        }
    }/* end while(1) */
    DBG_INFO(("RM INFO: rm_minfo_box_parse succeed!\r\n")); 
    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_box_parse
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
INT32 rm_file_minfo_box_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    MINFO_RM_FILE_INF_T*    pt_rm_hdlr_obj;
    INT32                   i4_ret;
        

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("ERROR: rm_file_minfo_box_parse fail.1 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        DBG_ERROR(("ERROR: rm_file_minfo_box_parse fail.2 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pt_rm_hdlr_obj = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj; 

    i4_ret = MINFOR_INTERNAL_ERR;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*seek to beginning*/
    i4_ret = _rm_info_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN);
    pt_rm_hdlr_obj->ui8_curr_parser_offset = 0;

    i4_ret = _rm_info_set_pos(pt_minfo_obj , 0, FM_SEEK_END);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
        return i4_ret;
    }
    _rm_info_get_pos(pt_minfo_obj, &(pt_rm_hdlr_obj->t_generic_info.ui8_media_sz));    
	i4_ret = _rm_info_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN);
	
    i4_ret = rm_minfo_box_parse(pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR:rm_file_minfo_box_parse parse fail.i4_ret = %ld \r\n",
            i4_ret));
        return i4_ret;
    }

    /* store generic info */
    pt_rm_hdlr_obj->t_generic_info.ui8_pb_duration = (UINT64)
                pt_rm_hdlr_obj->r_file_layout.t_file_property.ui4_duration 
                * 90;
    pt_rm_hdlr_obj->t_generic_info.ui1_strm_ns = (UINT8)
                (pt_rm_hdlr_obj->r_file_layout.t_stream_num.ui1_video_cnt
                + pt_rm_hdlr_obj->r_file_layout.t_stream_num.ui1_audio_cnt
                /*+ pt_rm_hdlr_obj->r_file_layout.t_stream_num.ui1_subtitle_cnt*/);
    pt_rm_hdlr_obj->t_generic_info.ui8_preroll = (UINT64)
                pt_rm_hdlr_obj->r_file_layout.t_file_property.ui4_preroll 
                * 90;
    
    DBG_INFO(("INFO: rm_file_minfo_box_parse OK. \r\n"));
    return MINFOR_OK;  
}
