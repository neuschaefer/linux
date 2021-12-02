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
 * mp4_parser.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/07/25 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the mp4 parser function
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
#include "inc/u_sys_name.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "dbg/u_dbg.h"
#include "mutil/minfo/mp4_info/mp4_common.h"
#include "mutil/mm_util.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "unicode/x_uc_str.h"
#include "mp4_parser.h"
#include "unicode/x_uc_str.h"


#else

#include "x_os.h"

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "../minfo.h"
#include "x_minfo.h"
#include "u_dbg.h"
#include "../mp4_info/mp4_common.h"

#include "../../mm_util.h"

#include "x_uc_str.h"
#include "mp4_parser.h"
#include "x_uc_str.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MINFO_MP4_ES_DESCRIPTOR_TAG                3
#define MINFO_MP4_DECODER_CONFIG_DESCR             4
#define MINFO_MP4_DEC_SPECIFIC_INFO_TAG            5

#define MINFO_MP4_PARSE_NEED_NEXT_LOOP        0x0000000F

#define dwBigFourCC(a,b,c,d)    ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))


#define MP4_BOX_FTYP       dwBigFourCC('f', 't', 'y', 'p')
#define MP4_BOX_MOOV       dwBigFourCC('m', 'o', 'o', 'v')
#define MP4_BOX_MP4V       dwBigFourCC('m', 'p', '4', 'v')
#define MP4_BOX_MP4A       dwBigFourCC('m', 'p', '4', 'a')
#define MP4_BOX_ESDS       dwBigFourCC('e', 's', 'd', 's')
#define MP4_BOX_MDAT       dwBigFourCC('m', 'd', 'a', 't')
#define MP4_BOX_UUID       dwBigFourCC('u', 'u', 'i', 'd')
#define MP4_BOX_AVCC       dwBigFourCC('a', 'v', 'c', 'C')
#define MP4_BOX_MVCC       dwBigFourCC('m', 'v', 'c', 'C')
#define MP4_BOX_CMOV       dwBigFourCC('c', 'm', 'o', 'v')
#define MP4_BOX_MOOF       dwBigFourCC('m', 'o', 'o', 'f')
#define MP4_BOX_MFRA       dwBigFourCC('m', 'f', 'r', 'a')
#define MP4_BOX_MFRO       dwBigFourCC('m', 'f', 'r', 'o')
#define MP4_BOX_FLSZ       dwBigFourCC('f', 'l', 's', 'z')

#define MP4_CODEC_MP4A     dwBigFourCC('m', 'p', '4', 'a')
#define MP4_CODEC_MP4V     dwBigFourCC('m', 'p', '4', 'v')
#define MP4_CODEC_AVC1     dwBigFourCC('a', 'v', 'c', '1')
#define MP4_CODEC_S263     dwBigFourCC('s', '2', '6', '3')
#define MP4_CODEC_H263     dwBigFourCC('H', '2', '6', '3')
#define MP4_CODEC_h263     dwBigFourCC('h', '2', '6', '3')
#define MP4_CODEC_AC3      dwBigFourCC('a', 'c', '-', '3')
#define MP4_CODEC_EC3      dwBigFourCC('e', 'c', '-', '3')
#define MP4_CODEC_MS       dwBigFourCC('m', 's', 0x00, 0x55)
#define MP4_CODEC_MP3      dwBigFourCC('.', 'm', 'p', '3')
#define MP4_CODEC_SAMR     dwBigFourCC('s', 'a', 'm', 'r')
#define MP4_CODEC_SAWB     dwBigFourCC('s', 'a', 'w', 'b')

/* Audio PCM fourCC*/
#define MP4_CODEC_QCLP     dwBigFourCC('Q', 'c', 'l', 'p')
#define MP4_CODEC_QDMC     dwBigFourCC('Q', 'D', 'M', 'C')
#define MP4_CODEC_TWOS     dwBigFourCC('t', 'w', 'o', 's')
#define MP4_CODEC_RAW      dwBigFourCC('r', 'a', 'w', 0x20)
#define MP4_CODEC_ima4     dwBigFourCC('i', 'm', 'a', '4')
#define MP4_CODEC_IMA4     dwBigFourCC('I', 'M', 'A', '4')
#define MP4_CODEC_SOWT     dwBigFourCC('s', 'o', 'w', 't')

#define MP4_CODEC_JPEG     dwBigFourCC('j', 'p', 'e', 'g')
#define MP4_CODEC_MJPG     dwBigFourCC('M', 'J', 'P', 'G')
#define MP4_SBTL_STGS      dwBigFourCC('S', 'T', 'G', 'S')
#define MP4_SBTL_MP4S      dwBigFourCC('m', 'p', '4', 's')

/* For Marlin Spec Use*/
#define MP4_BRAND_MGSV     dwBigFourCC('M', 'G', 'S', 'V')/*marlin spec*/
#define MP4_BRAND_MSNV     dwBigFourCC('M', 'S', 'N', 'V')/*marlin spec*/
#define MP4_BOX_SCHM       dwBigFourCC('s', 'c', 'h', 'm')
#define MP4_BOX_SCHI       dwBigFourCC('s', 'c', 'h', 'i')
#define MP4_BOX_KSID       dwBigFourCC('k', 's', 'i', 'd')
#define MP4_BOX_KYID       dwBigFourCC('k', 'y', 'i', 'd')
#define MP4_BOX_EALG       dwBigFourCC('e', 'a', 'l', 'g')
#define MP4_BOX_SINF       dwBigFourCC('s', 'i', 'n', 'g')
#define MP4_BOX_8ID        dwBigFourCC('8', 'i', 'd', ' ')
#define MP4_BOX_SATR       dwBigFourCC('s', 'a', 't', 'r')
#define MP4_BOX_STYP       dwBigFourCC('s', 't', 'y', 'p')
#define MP4_BOX_HMAC       dwBigFourCC('h', 'm', 'a', 'c')


#define MP4_BOX_NDRM       dwBigFourCC('n', 'd', 'r', 'm')
#define MP4_BOX_NAM        dwBigFourCC(0xA9, 'n', 'a', 'm')
#define MP4_BOX_AUT        dwBigFourCC(0xA9, 'a', 'u', 't')
#define MP4_BOX_CPY        dwBigFourCC(0xA9, 'c', 'p', 'y')
#define MP4_BOX_META       dwBigFourCC('m', 'e', 't', 'a')



#define MP4_TRAK_TYPE_VIDE       dwBigFourCC('v', 'i', 'd', 'e')
#define MP4_TRAK_TYPE_SOUN       dwBigFourCC('s', 'o', 'u', 'n')
#define MP4_TRAK_TYPE_GRAP       dwBigFourCC('G', 'R', 'A', 'P')
#define MP4_TRAK_TYPE_SUBP       dwBigFourCC('s', 'u', 'b', 'p')
#define MP4_TRAK_TYPE_AUDI       dwBigFourCC('a', 'u', 'd', 'i')

#define MP4_BOX_MEHD       dwBigFourCC('m', 'e', 'h', 'd')

       
#define MP4_CMOV_ZIP_CODE_ZLIB       dwBigFourCC('z', 'l', 'i', 'b')

#define NO_SKIP_UNSUPPORT_TRACK 1
                                      /*(ui4_FourCC == MP4_CODEC_QCLP) || \
                                        (ui4_FourCC == MP4_CODEC_SOWT) || \  
                                        (ui4_FourCC == MP4_CODEC_QDMC)*/
#define IS_AUDIO_CODEC_PCM(ui4_FourCC) ((ui4_FourCC == MP4_CODEC_ima4) || \
                                        (ui4_FourCC == MP4_CODEC_IMA4) || \
                                        (ui4_FourCC == MP4_CODEC_TWOS) || \
                                        (ui4_FourCC == MP4_CODEC_RAW))

#define IS_AUDIO_IMA_ADPCM(ui4_FourCC) ((ui4_FourCC == MP4_CODEC_ima4) || \
                                        (ui4_FourCC == MP4_CODEC_IMA4))

#define IS_AUDIO_TWO_ADPCM(ui4_FourCC) ((ui4_FourCC == MP4_CODEC_SOWT) || \
                                        (ui4_FourCC == MP4_CODEC_TWOS))

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  _mp4_check_audio_layer
 *
 * Description: 
 *
 * Inputs:  -ui4_4cc
 *          
 * Outputs: -None
 *
 * Returns: Minfo video stream encoding type
 ----------------------------------------------------------------------------*/
UINT32 _mp4_check_audio_layer(MINFO_OBJ_T*   pt_minfo_obj, 
                            MINFO_MP4_AUDIO_STM_ATRBT_T  * pt_audio_strm_info)
{

    UINT64                          ui8_first_sample_ofs_co64   = 0;
    UINT64                          ui8_curr_offset             = 0;
    UINT32                          ui4_first_sample_ofs        = 0;
    UINT32                          ui4_first_sample_ofs_tmp    = 0;
    UINT32                          ui4_mdat_hdr_4bytes         = 0;
    UINT32                          ui4_layer                   = 0;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    MP4_BOX_T *                     pt_stco_box                 = NULL;
    setpos_fct                      pf_set_pos;    
    input4bytes_fct                 pf_inp4byte;

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    ui8_curr_offset = pt_mp4_hdlr_obj->ui8_curr_parser_offset;
    
    _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STCO, &pt_stco_box);
    if (pt_stco_box == NULL)
    {
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_CO64, &pt_stco_box);
        if(pt_stco_box != NULL)
        {                  
            /*Check whether this box is valid*/                    
            pf_set_pos(pt_minfo_obj, (INT64)pt_stco_box->ui8_offset + 16, FM_SEEK_BGN);
            pf_inp4byte(pt_minfo_obj, &ui4_first_sample_ofs);  
            pf_inp4byte(pt_minfo_obj, &ui4_first_sample_ofs_tmp); 
            ui8_first_sample_ofs_co64 = ((UINT64)ui4_first_sample_ofs << 32) | ui4_first_sample_ofs_tmp;
            pf_set_pos(pt_minfo_obj, (INT64)ui8_first_sample_ofs_co64, FM_SEEK_BGN);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        pf_set_pos(pt_minfo_obj, (INT64)pt_stco_box->ui8_offset + 16, FM_SEEK_BGN);
        pf_inp4byte(pt_minfo_obj, &ui4_first_sample_ofs); 
        pf_set_pos(pt_minfo_obj, (INT64)ui4_first_sample_ofs, FM_SEEK_BGN);
    }

    pf_inp4byte(pt_minfo_obj, &ui4_mdat_hdr_4bytes);  
    if((ui4_mdat_hdr_4bytes & 0xFFF00000) == 0xFFF00000)
    {
        ui4_layer = (ui4_mdat_hdr_4bytes & 0x60000)>>17;
        pt_audio_strm_info->e_layer = 4 - ui4_layer;/* 11172-3 p15 ,22*/
    }   
    pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
    
    return 0;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_aud_codec
 *
 * Description: 
 *
 * Inputs:  -pt_audio_strm_info
 *          -ui1_codec
 * Outputs: -None
 *
 * Returns: Minfo audio stream encoding type
 ----------------------------------------------------------------------------*/
static void _mp4_get_aud_codec(MINFO_OBJ_T*   pt_minfo_obj, 
                               MINFO_MP4_AUDIO_STM_ATRBT_T  * pt_audio_strm_info, 
                               UINT8                        ui1_codec)
{
    switch(ui1_codec)
    {/*14496_1 p42*/
    case 0x40:/*AAC*/
    case 0x67:/*AAC profile = 2*/
#if 0
    case 0x66:/*AAC profile = 1*/
    case 0x68:/*AAC profile = 3*/
#endif
        pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AAC;
        break;
    case 0x69:/*Mpeg 2*/
    case 0x6B:/*MP3*/
        pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        pt_audio_strm_info->e_layer = 3;
        _mp4_check_audio_layer(pt_minfo_obj, pt_audio_strm_info);
        break;
    default:
        pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        break;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_video_codec
 *
 * Description: 
 *
 * Inputs:  -ui4_4cc
 *          
 * Outputs: -None
 *
 * Returns: Minfo video stream encoding type
 ----------------------------------------------------------------------------*/
static MINFO_INFO_VIDEO_ENC_T _mp4_video_codec(UINT32 ui4_4cc)
{
    MINFO_INFO_VIDEO_ENC_T e_video_enc_mode;

    switch(ui4_4cc)
    {
    case MP4_CODEC_MP4V: 
        e_video_enc_mode = MINFO_INFO_VID_ENC_MPEG_4;
        break;
    case MP4_CODEC_AVC1: 
        e_video_enc_mode = MINFO_INFO_VID_ENC_H264;
        break;
    case MP4_CODEC_S263: 
    case MP4_CODEC_H263: 
    case MP4_CODEC_h263: 
        e_video_enc_mode = MINFO_INFO_VID_ENC_H263;
        break;
    case MP4_CODEC_JPEG: 
    case MP4_CODEC_MJPG:
        e_video_enc_mode = MINFO_INFO_VID_ENC_MJPG;
        break;
    default:
        e_video_enc_mode = MINFO_INFO_VID_ENC_UNKNOWN;
        break;
    }
    return(e_video_enc_mode);
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_box_is_need_parser
 *
 * Description: 
 *
 * Inputs:  -e_id
 *          
 * Outputs: -none
 *
 * Returns: TRUE      The box need parser
 *          FALSE     The box don't need parser
 ----------------------------------------------------------------------------*/ 
static BOOL _mp4_box_is_need_parser(MP4_BOX_ID_ENUM_T e_id)
{
    return (e_id == MP4_BOX_ID_MVHD 
            || e_id == MP4_BOX_ID_TKHD
            || e_id == MP4_BOX_ID_MDHD
            || e_id == MP4_BOX_ID_HDLR
            || e_id == MP4_BOX_ID_STSD
            || e_id == MP4_BOX_ID_STSZ
            || e_id == MP4_BOX_ID_STCO
            || e_id == MP4_BOX_ID_CO64
            || e_id == MP4_BOX_ID_ESDS);
}



/*-----------------------------------------------------------------------------
 * Name:  _mp4_add_box_to_tree
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          -e_id
 *          -ui8_offset
 *          -ui4_size
 *          
 * Outputs: -none
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/
static INT32 _mp4_add_box_to_tree(MINFO_OBJ_T*              pt_minfo_obj, 
                                  MP4_BOX_ID_ENUM_T           e_id, 
                                  UINT64                    ui8_offset,
                                  UINT32                    ui4_size)
{
    MP4_BOX_T*               pt_box;
    MP4_BOX_T*               pt_par_box;
    MP4_BOX_T*               pt_last_Sub;
    MINFO_MP4_MOOV_INF_T*      pt_mp4_hdlr_obj;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("ERROR:_mp4_add_box_to_tree: invalid parameter \r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    pt_box         = NULL;
    pt_par_box     = NULL;
    pt_last_Sub     = NULL;
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;   

    _mp4_get_par_box_by_offset(pt_mp4_hdlr_obj->pt_tree_box, 
                               pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                               &pt_par_box);

    pt_box = (MP4_BOX_T *)(_mm_util_mem_alloc(sizeof(MP4_BOX_T)));

    if (pt_box == NULL)
    {
        DBG_ERROR(("_mp4_add_box_to_tree: Failed to malloc mp4 box. \r\n "));
        return MINFOR_OUT_OF_MEMORY;
    }
    
    pt_mp4_hdlr_obj->ui4_malloc_count += 1;

    if (pt_par_box!= NULL)
    {
        _mp4_get_last_sub_box(pt_par_box, &pt_last_Sub);
    } 
    else if (pt_mp4_hdlr_obj->pt_tree_box== NULL)
    {
        pt_mp4_hdlr_obj->pt_tree_box = pt_box;
    }
    if (pt_last_Sub == NULL)
    {
        if (pt_par_box != NULL)
        {
            pt_par_box->pt_first_sub_box = pt_box;
        }    
    }
    else
    {
        pt_last_Sub->pt_next_box= pt_box;
    }  
    pt_box->ui8_offset      = ui8_offset;
    pt_box->e_box_id= e_id;
    pt_box->ui4_size= ui4_size;
    pt_box->pt_parent_box= pt_par_box;
    pt_box->pt_first_sub_box= NULL;
    pt_box->pt_next_box= NULL;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_delete_track
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          -pt_cur_box
 *          
 * Outputs: -none
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/
static INT32 _mp4_delete_track(MINFO_OBJ_T*     pt_minfo_obj, 
                               MP4_BOX_T*         pt_cur_box)
{
    MP4_BOX_T*                pt_par_box;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_hdlr_obj;
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;   

    while (pt_cur_box != NULL)
    {
        if (pt_cur_box->pt_first_sub_box!= NULL)
        {
            pt_cur_box = pt_cur_box->pt_first_sub_box;
        }
        else 
        {
            pt_par_box = pt_cur_box->pt_parent_box;
            if (pt_par_box == NULL)
            {               
                _mm_util_mem_free(pt_cur_box);
                pt_mp4_hdlr_obj->ui4_malloc_count -= 1;
                break;
            }
            if ((pt_par_box != NULL) 
                && (pt_par_box->pt_first_sub_box == pt_cur_box))
            {
                pt_par_box->pt_first_sub_box = pt_cur_box ->pt_next_box;
            }   
            if (pt_cur_box->pt_first_sub_box == NULL)
            {                
                _mm_util_mem_free(pt_cur_box);
                pt_mp4_hdlr_obj->ui4_malloc_count -= 1;
            }
            pt_cur_box = pt_par_box;
        }
     }
    if(pt_mp4_hdlr_obj->ui4_malloc_count != 0)
    {
        DBG_ERROR(("_mp4_delete_track: after delete, tree count is %ld \r\n ", 
                      pt_mp4_hdlr_obj->ui4_malloc_count));
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_delete_track_tree
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          
 * Outputs: -none
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/
static INT32 _mp4_delete_track_tree(MINFO_OBJ_T*  pt_minfo_obj)
{
    MINFO_MP4_MOOV_INF_T* pt_mp4_hdlr_obj;
   
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;   
    _mp4_delete_track(pt_minfo_obj, pt_mp4_hdlr_obj->pt_tree_box);
    pt_mp4_hdlr_obj->pt_tree_box= NULL;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_box_id_and_size
 *
 * Description: 
 *
 * Inputs:  -pv_data
 *             -pui4_size
 *          
 * Outputs: -
 *
 * Returns: MINFO MP4 BOX ID
 ----------------------------------------------------------------------------*/         
static MP4_BOX_ID_ENUM_T _mp4_get_box_id_and_size(void*       pv_data, 
                                                  UINT32*     pui4_size)
{
    UINT8   ui1_idx;
    UINT32  ui4_sz;
    
    LOADB_DWRD((UINT8 *)pv_data, ui4_sz);
    *pui4_size = ui4_sz;
    
    for (ui1_idx = 0; ui1_idx < MP4_BOX_ID_MAX; ui1_idx++)
    {
        if (0 == x_memcmp((((UINT8 *)pv_data) + 4), 
                          _r_mp4_box_id_map[ui1_idx].sz_four_cc, 4))
        {
            return _r_mp4_box_id_map[ui1_idx].e_box_id;
        }
    }
    return MP4_BOX_ID_MAX;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_check_box_buf
 *
 * Description: check mp4 handler object box buffer enough or not
 *
 * Inputs:  pt_mp4_obj      specify mp4 handler object
 *          ui4_curr_len    specify current need buffer length
 *          
 * Outputs: 
 *
 * Returns: MINFOR_OK                The API is successful. 
 *          MINFOR_OUT_OF_MEMORY     The API alloc memory fail.
 ----------------------------------------------------------------------------*/
static INT32 _mp4_check_box_buf(MINFO_MP4_MOOV_INF_T*  pt_mp4_obj, 
                                UINT32                 ui4_curr_len)
{
    if (ui4_curr_len > pt_mp4_obj->ui4_buf_length)
    {
        _mm_util_mem_free(pt_mp4_obj->pui1_box_buf);
        pt_mp4_obj->pui1_box_buf = NULL;
        pt_mp4_obj->pui1_box_buf = 
            (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui4_curr_len);
        if (pt_mp4_obj->pui1_box_buf  == NULL)
        {
            DBG_ERROR(("ERROR: _mp4_check_box_buf alloc memory fail \r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        x_memset(pt_mp4_obj->pui1_box_buf , 0, sizeof(UINT8) * ui4_curr_len);
        pt_mp4_obj->ui4_buf_length = ui4_curr_len;
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_descr_length_and_digit
 *
 * Description: 
 *
 * Inputs:  -pui1_buf
 *          
 * Outputs: -pui1_digit_num
 *
 * Returns: Description content length
 ----------------------------------------------------------------------------*/
static UINT32 _mp4_get_descr_length_and_digit(UINT8* pui1_buf, 
                                              UINT8* pui1_digit_num)
{
    UINT32 ui4_tmp;
    UINT32 ui4_length;
    
    ui4_length = 0;
    *pui1_digit_num = 0;
    do 
    {
        ui4_tmp = *pui1_buf;
        pui1_buf++;
        (*pui1_digit_num)++;
        ui4_length = (ui4_length << 7) + (ui4_tmp&0x7F);
    } while (ui4_tmp&0x80);
    return ui4_length;
}

/*-----------------------------------------------------------------------------
 * Name:  _pui8_mp4_moov_get_esds_pos
 *
 * Description: get esds box position
 *
 * Inputs:  stsd box start address
 *          
 * Outputs: esds position 
 *
 * Returns: NULL                 esds not exist. 
 ----------------------------------------------------------------------------*/   
static UINT8* _pui8_mp4_moov_get_esds_pos(UINT8* pui1_box_buf)
{
    UINT32   ui4_idx = 4; /*maybe can skip more */
    UINT32   ui4_fourcc = 0;  
    UINT32   ui4_box_size = 0;  
    UINT8*   pui1_tmp_buf = pui1_box_buf;
    
    LOADB_DWRD(pui1_box_buf, ui4_box_size);
    for (; ui4_idx < ui4_box_size; ui4_idx += 4)
    {
        LOADB_DWRD(pui1_box_buf + ui4_idx, ui4_fourcc);
        if (ui4_fourcc == MP4_BOX_ESDS)
        {
            return (pui1_box_buf + ui4_idx - 4);
        }
    }
    /*check one more box for 88916*/
    pui1_tmp_buf += ui4_box_size;
    ui4_idx = 4;
    LOADB_DWRD(pui1_tmp_buf, ui4_box_size);
    for (; ui4_idx < ui4_box_size; ui4_idx += 4)
    {
        LOADB_DWRD(pui1_tmp_buf + ui4_idx, ui4_fourcc);
        if (ui4_fourcc == MP4_BOX_ESDS)
        {
            return (pui1_tmp_buf + ui4_idx - 4);
        }
    }
    return NULL;
}
/*-----------------------------------------------------------------------------
 * Name:  _mp4_proc_audio_codec_info
 *
 * Description: parse optional description box
 *
 * Inputs:  - pt_minfo_obj
 *          - pui1_audio_codec_info
 *          - ui1_codec_length
 *          
 * Outputs: - 
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
static INT32 _mp4_proc_audio_codec_info(MINFO_OBJ_T*   pt_minfo_obj, 
                                        UINT8*         pui1_audio_codec_info, 
                                        UINT8          ui1_codec_length)
{
    UINT8                           ui1_tmp = 0;
    UINT8                           ui1_strm_idx = 0;
    UINT8                           ui1_audio_type = 0;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj = NULL;
    MINFO_MP4_AUDIO_STM_ATRBT_T*    pt_audio_strm_info = NULL;

    INT32 ai4_sample_rate[16] = 
    {
        96000,  88200,  64000,  48000,  44100,  32000,  24000,  22050,
        16000,  12000,  11025,   8000,   7350,      0,      0,      0
    };
    
    if ((pt_minfo_obj == NULL)
        || (pui1_audio_codec_info == NULL)
        || (ui1_codec_length < 2))
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    if(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num < 1)
    {
        return MINFOR_INTERNAL_ERR;
    }
    ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num - 1);
    if(ui1_strm_idx >= MAX_NS_MP4_AUD)
    {
        return MINFOR_INTERNAL_ERR;
    }
    pt_audio_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rAudStrmInf[ui1_strm_idx]);

    /*5 bit audio_type(if ==31, ext 6bit ) + 4 bit sample rate idx + 4 bit channel number*/
    /* Get audio type*/
    ui1_tmp = ((pui1_audio_codec_info[0]&0xf8)>>3);
    if(ui1_tmp != 0x1F)
    {
        ui1_audio_type = ui1_tmp;
        pt_audio_strm_info->ui4_aac_profile = ui1_tmp;
        
        /*get sample rate index*/
        ui1_tmp = ((pui1_audio_codec_info[0]&0x07)<<1)
                 |((pui1_audio_codec_info[1]&0x80)>>7);
        pt_audio_strm_info->i4_sample_rate = ai4_sample_rate[ui1_tmp&0x0F];

        /*get frame length falg*/
        if((ui1_audio_type == 1) || (ui1_audio_type == 2) || (ui1_audio_type == 3) || (ui1_audio_type == 4)|| 
          (ui1_audio_type == 6) || (ui1_audio_type == 7) || (ui1_audio_type == 17) || (ui1_audio_type == 19) || 
          (ui1_audio_type == 20) || (ui1_audio_type == 21) || (ui1_audio_type == 22) || (ui1_audio_type == 23))
        {
            if(ui1_tmp == 0x0F)
            {
                pt_audio_strm_info->b_frame_length = ((pui1_audio_codec_info[4]&0x04) == 0x04);
            }
            else
            {
                pt_audio_strm_info->b_frame_length = ((pui1_audio_codec_info[1]&0x04) == 0x04);
            }        
        }
        /*get channel number*/
        ui1_tmp = ((pui1_audio_codec_info[1]&0x78)>>3);
        pt_audio_strm_info->i2_channels = (INT16)ui1_tmp;
    }
    else
    {
        ui1_tmp = ((pui1_audio_codec_info[0]&0x07)<<3)
                 |((pui1_audio_codec_info[1]&0xE0)>>5);
        pt_audio_strm_info->ui4_aac_profile = 32 + ui1_tmp;
        
        /*get sample rate index*/
        ui1_tmp = (pui1_audio_codec_info[1]&0x1E)>>1;
        pt_audio_strm_info->i4_sample_rate = ai4_sample_rate[ui1_tmp&0x0F];
        
        /*get channel number*/
        ui1_tmp = ((pui1_audio_codec_info[1]&0x01)<<3)
                 |((pui1_audio_codec_info[2]&0xE0)>>5);
        pt_audio_strm_info->i2_channels = (INT16)ui1_tmp;
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_optional_description_parse
 *
 * Description: parse optional description box
 *
 * Inputs:  -
 *          
 * Outputs: - 
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
static INT32 _mp4_optional_description_parse(MINFO_OBJ_T*   pt_minfo_obj, 
                                             UINT8*         pui1_optional_descr, 
                                             UINT32         ui4_buf_len,
                                             UINT32         ui4_data_format)
{
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    MINFO_MP4_VIDEO_STM_ATRBT_T*    pt_video_strm_info;
    MINFO_MP4_AUDIO_STM_ATRBT_T*    pt_audio_strm_info;
    UINT8                           ui1_tmp;
    UINT8                           ui1_es_descr_digit;
    UINT32                          ui4_es_descr_length;
    UINT32                          ui4_fourcc;
    UINT32                          ui4_size;
    UINT8                           ui1_strm_idx;
    
    /* check parameter */
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }

    pt_video_strm_info = NULL;
    pt_audio_strm_info = NULL;
    ui1_tmp = 0;
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF)
    {
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num - 1);
        if(ui1_strm_idx >= MAX_NS_MP4_VID)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_video_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rVideStrmInf[ui1_strm_idx]);
    }
    else
    {
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num - 1);
        if(ui1_strm_idx >= MAX_NS_MP4_AUD)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_audio_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rAudStrmInf[ui1_strm_idx]);
    }
     
    if ((ui4_data_format == MP4_BOX_MP4V) || (ui4_data_format == MP4_BOX_MP4A))
    {
        pui1_optional_descr = _pui8_mp4_moov_get_esds_pos(pui1_optional_descr);
        if(pui1_optional_descr == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        /* get "esds" box */
        LOADB_DWRD(pui1_optional_descr + 4, ui4_fourcc);
        if (ui4_fourcc != MP4_BOX_ESDS)
        {
            return MINFOR_INTERNAL_ERR;
        }
        /* seek to ES_DESCRIPTOR_TAG */
        pui1_optional_descr += 12;
        ui4_fourcc = *(pui1_optional_descr);
        if (ui4_fourcc != MINFO_MP4_ES_DESCRIPTOR_TAG)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pui1_optional_descr++;
        _mp4_get_descr_length_and_digit(pui1_optional_descr, &ui1_es_descr_digit);
        pui1_optional_descr += ui1_es_descr_digit;
        ui4_fourcc = *(pui1_optional_descr + 2);
        if ((ui4_fourcc & 0x80) == 0x80)
        {
            ui1_tmp += 2;
        }
        if ((ui4_fourcc & 0x40) == 0x40)
        {
            ui1_tmp += 2;
        }
        if ((ui4_fourcc & 0x20) == 0x20)
        {
            ui1_tmp += 2;
        }
        
        pui1_optional_descr +=  (3 + ui1_tmp);
        ui4_fourcc = *(pui1_optional_descr);
        if (ui4_fourcc != MINFO_MP4_DECODER_CONFIG_DESCR)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pui1_optional_descr++;
        _mp4_get_descr_length_and_digit(pui1_optional_descr, &ui1_es_descr_digit);
        pui1_optional_descr += ui1_es_descr_digit;

        if(ui4_data_format == MP4_BOX_MP4A)
        {
            ui1_tmp = *(pui1_optional_descr);
            _mp4_get_aud_codec(pt_minfo_obj, pt_audio_strm_info, ui1_tmp);
        }
        else if(ui4_data_format == MP4_BOX_MP4V)
        {
            ui1_tmp = *(pui1_optional_descr);
            if( (ui1_tmp != 0x20)/*14496_1 p42*/
              &&(ui1_tmp != 0x60)
              &&(ui1_tmp != 0x61)
              &&(ui1_tmp != 0x62)
              &&(ui1_tmp != 0x63)
              &&(ui1_tmp != 0x64)
              &&(ui1_tmp != 0x65)
              &&(ui1_tmp != 0x6A)
              &&(ui1_tmp != 0x6C))
            {
                pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
                pt_video_strm_info->ui4_codec_info_size = 0;
                pt_video_strm_info->pui1_codec_info = NULL;
                return MINFOR_OK;
            }
        }

        pui1_optional_descr += 13;
        ui4_fourcc = *(pui1_optional_descr);
        if (ui4_fourcc != MINFO_MP4_DEC_SPECIFIC_INFO_TAG)
        {
            if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF)
            {
                pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
                pt_video_strm_info->ui4_codec_info_size = 0;
                pt_video_strm_info->pui1_codec_info = NULL;
            }
            else
            {
                if(pt_audio_strm_info->e_enc == MINFO_INFO_AUD_ENC_AAC)
                {/*h264 must have codec info*/
                    pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
                }
                pt_audio_strm_info->ui4_codec_info_size = 0;
                pt_audio_strm_info->pui1_codec_info = NULL;
            }
            return MINFOR_OK;
        }
        pui1_optional_descr++;
        ui4_es_descr_length = _mp4_get_descr_length_and_digit(pui1_optional_descr, 
                                                              &ui1_es_descr_digit);
        pui1_optional_descr += ui1_es_descr_digit;
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF)
        {
            pt_video_strm_info->ui4_codec_info_size = ui4_es_descr_length;
            if (ui4_es_descr_length == 0)
            {
                pt_video_strm_info->pui1_codec_info = NULL;
                pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;/*skip it!!!*/
            }
            else
            {
                pt_video_strm_info->pui1_codec_info
                        = _mm_util_mem_alloc(sizeof(UINT8)*ui4_es_descr_length);
                if (pt_video_strm_info->pui1_codec_info == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memcpy(pt_video_strm_info->pui1_codec_info,
                         pui1_optional_descr, 
                         ui4_es_descr_length);
            }
        }
        else
        {
            /* check audio codec type first: 14496_3_2005 EN P52*/
            UINT8 ui1_audio_object_type = (*pui1_optional_descr)>>3;
            if(ui1_audio_object_type == 31)
            {
                ui1_audio_object_type = (UINT8)(32 + (((*pui1_optional_descr)&0x07)<<3)
                                           + ((*(pui1_optional_descr+1))>>5));
                if(ui1_audio_object_type == 36)
                {
                    pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
                    pt_audio_strm_info->ui4_codec_info_size = 0;
                    pt_audio_strm_info->pui1_codec_info = NULL;
                    return MINFOR_OK;
                }
            }
            pt_audio_strm_info->ui4_codec_info_size = ui4_es_descr_length;
            if (ui4_es_descr_length == 0)
            {
                pt_audio_strm_info->pui1_codec_info = NULL;
            }
            else
            {
                pt_audio_strm_info->pui1_codec_info
                        = _mm_util_mem_alloc(sizeof(UINT8)*ui4_es_descr_length);
                if (pt_audio_strm_info->pui1_codec_info == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memcpy(pt_audio_strm_info->pui1_codec_info,
                         pui1_optional_descr, 
                         ui4_es_descr_length);
                if(pt_audio_strm_info->e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    _mp4_proc_audio_codec_info(pt_minfo_obj, 
                                               pui1_optional_descr,
                                        (UINT8)ui4_es_descr_length);
                }
            }
        }
    }
    else if(ui4_data_format == MP4_CODEC_AVC1)
    {
        /* get "avcC" box */
        UINT32 ui4_offset = 0;
        if(pt_video_strm_info == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_video_strm_info->ui4_codec_info_size = 0;
        pt_video_strm_info->pui1_codec_info = NULL;
        pt_video_strm_info->ui4_mvcC_codec_info_size = 0;
        pt_video_strm_info->pui1_mvcC_codec_info = NULL;
        while(1)
        {
            LOADB_DWRD(pui1_optional_descr + ui4_offset+4, ui4_fourcc);
            LOADB_DWRD(pui1_optional_descr + ui4_offset,   ui4_size);
            if (ui4_fourcc == MP4_BOX_AVCC)
            {
                if(ui4_size - 8 == 0)
                {
                    ui4_offset += ui4_size;
                    pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
                    continue;
                }
                pt_video_strm_info->ui4_codec_info_size = ui4_size - 8;
                pt_video_strm_info->pui1_codec_info
                        = _mm_util_mem_alloc(sizeof(UINT8)*(ui4_size - 8));
                if (pt_video_strm_info->pui1_codec_info == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memcpy(pt_video_strm_info->pui1_codec_info,
                        pui1_optional_descr + ui4_offset + 8, 
                        ui4_size - 8);
            }
            else if(ui4_fourcc == MP4_BOX_MVCC)
            {
                if(ui4_size - 8 == 0)
                {
                    ui4_offset += ui4_size;
                    continue;
                }
                pt_video_strm_info->ui4_mvcC_codec_info_size = ui4_size - 8;
                pt_video_strm_info->pui1_mvcC_codec_info
                        = _mm_util_mem_alloc(sizeof(UINT8)*(ui4_size - 8));
                if (pt_video_strm_info->pui1_mvcC_codec_info == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memcpy(pt_video_strm_info->pui1_mvcC_codec_info,
                        pui1_optional_descr + ui4_offset + 8, 
                        ui4_size - 8);
            }
            ui4_offset += ui4_size;
            if((ui4_offset >= ui4_buf_len) || (ui4_size == 0))
            {
                break;
            }
        };
    }
    else if (ui4_data_format == MP4_CODEC_MS)
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
            pt_audio_strm_info->e_layer = 3;
            _mp4_check_audio_layer(pt_minfo_obj, pt_audio_strm_info);  
            pt_audio_strm_info->ui4_codec_info_size = 0;
            pt_audio_strm_info->pui1_codec_info = NULL;
        }

    }
    else if (ui4_data_format == MP4_CODEC_AC3)
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AC3; 
        }
    }
     else if (ui4_data_format == MP4_CODEC_EC3 )
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_EC3; 
        }
    }
    else if (ui4_data_format == MP4_CODEC_MP3 )
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
            pt_audio_strm_info->e_layer = 3;
        }
    }
    else if (ui4_data_format == MP4_CODEC_SAMR)
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AMR;
        }
    }
    else if (ui4_data_format == MP4_CODEC_SAWB)
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_AWB;
        }
    }
    else if IS_AUDIO_CODEC_PCM(ui4_data_format)
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF)
        {
            if(IS_AUDIO_IMA_ADPCM(ui4_data_format))
            {
                pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_DVI_ADPCM;
            }
            else
            {
                pt_audio_strm_info->e_enc = MINFO_INFO_AUD_ENC_PCM;
            }

            if(IS_AUDIO_TWO_ADPCM(ui4_data_format))
            {
                pt_audio_strm_info->i4_sample_size = 2;
            }
            else
            {
                pt_audio_strm_info->i4_sample_size = 1;
            }
        }
    }
    else
    {
        if (pt_mp4_hdlr_obj->eParserState == MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF)
        {
            pt_video_strm_info->ui4_codec_info_size = 0;
            pt_video_strm_info->pui1_codec_info = NULL;
        }
        else
        {
            pt_audio_strm_info->ui4_codec_info_size = 0;
            pt_audio_strm_info->pui1_codec_info = NULL;
        }
    }
    
    return MINFOR_OK;
}

static INT32 _mp4_get_dur_from_mvex(MINFO_OBJ_T*   pt_minfo_obj)
{
    setpos_fct                      pf_set_pos;    
    copybytes_fct                   pf_copybyte;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    INT32                           i4_ret;
    UINT32                          ui4_fourcc;
    UINT32                          ui4_read_size;
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;

    pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
    i4_ret = pf_copybyte(pt_minfo_obj, 
                         pt_mp4_hdlr_obj->pui1_box_buf, 
                         pt_mp4_hdlr_obj->ui4_buf_length, 
                         32,/* is enough ???*/ 
                         &ui4_read_size);
    LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 4, ui4_fourcc);
    if(ui4_fourcc == MP4_BOX_MEHD)
    {
        UINT8   ui1_ver = 0;
        UINT32  ui4_high= 0;
        UINT32  ui4_low = 0;
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_high);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 16, ui4_low);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_low);
        }
        
        pt_mp4_hdlr_obj->ui8_movie_duration = ((UINT64)ui4_high<<32) | ui4_low;
        pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
                    _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                              pt_mp4_hdlr_obj->ui4_movie_timescale, 
                              NULL);
    }
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _mp4_parse_box_udta
 *
 * Description: parse optional description box
 *
 * Inputs:  -
 *          
 * Outputs: - 
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
static INT32 _mp4_parse_box_udta(MINFO_OBJ_T*   pt_minfo_obj)
{
    setpos_fct                      pf_set_pos;    
    copybytes_fct                   pf_copybyte;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    INT32                           i4_ret;
    UINT32                          ui4_udta_box_size;
    UINT32                          ui4_total_sub_box_size = 0;
    UINT32                          ui4_sub_box_size;
    UINT32                          ui4_fourcc;
    UINT32                          ui4_read_size;
    UINT16                          ui2_str_len;
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;

    pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset-8, FM_SEEK_BGN);
    i4_ret = pf_copybyte(pt_minfo_obj, 
                         pt_mp4_hdlr_obj->pui1_box_buf, 
                         pt_mp4_hdlr_obj->ui4_buf_length, 
                         8,/*4b size + 4b 'UDTA'*/ 
                         &ui4_read_size);
    LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf, ui4_udta_box_size);

    while(1)
    {    
        i4_ret = pf_copybyte(pt_minfo_obj, 
                            pt_mp4_hdlr_obj->pui1_box_buf, 
                            pt_mp4_hdlr_obj->ui4_buf_length, 
                            12,/*4b size + 4b 'UDTA' + 2 len + 2 unknown*/ 
                            &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf, ui4_sub_box_size);
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 4, ui4_fourcc);
        switch(ui4_fourcc)
        {
            case MP4_BOX_NDRM:
            {
                UINT32 ui4_version;
                BOOL fgEnable = FALSE;
                BOOL fgCePlay = FALSE;
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                     pt_mp4_hdlr_obj->pui1_box_buf, 
                                     pt_mp4_hdlr_obj->ui4_buf_length, 
                                     7,
                                     &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf, ui4_version);
                fgEnable = (BOOL)((pt_mp4_hdlr_obj->pui1_box_buf[4] & 0x80) != 0);
                fgCePlay = (BOOL)((pt_mp4_hdlr_obj->pui1_box_buf[5] & 0x40) != 0);
                pt_mp4_hdlr_obj->t_generic_info.b_is_drm_exist
                            = (BOOL)((ui4_version == 1) && fgEnable && fgCePlay);
            }
                break;
            case MP4_BOX_NAM:
            {
                LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_title_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title, 0, ui2_str_len);
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                    pt_mp4_hdlr_obj->pui1_box_buf, 
                                    sizeof(UTF16_T)*ui2_str_len, 
                                    sizeof(UTF16_T)*ui2_str_len,
                                    &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)(pt_mp4_hdlr_obj->pui1_box_buf), 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_title_len = 0;
                }
            }
                break;
            case MP4_BOX_AUT:
            {
                LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_director_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director, 0, ui2_str_len);
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                    pt_mp4_hdlr_obj->pui1_box_buf, 
                                    sizeof(UTF16_T)*ui2_str_len, 
                                    sizeof(UTF16_T)*ui2_str_len,
                                    &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)(pt_mp4_hdlr_obj->pui1_box_buf), 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_director_len = 0;
                }
            }
                break;
            case MP4_BOX_CPY:
            {
                LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_copyright_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright, 0, ui2_str_len);
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                    pt_mp4_hdlr_obj->pui1_box_buf, 
                                    sizeof(UTF16_T)*ui2_str_len, 
                                    sizeof(UTF16_T)*ui2_str_len,
                                    &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)(pt_mp4_hdlr_obj->pui1_box_buf), 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_copyright_len = 0;
                }
            }
                break;
            /*case MP4_BOX_META:
                ui4_sub_box_size = 8;process the next
                break;*/
            default:
            if(ui4_sub_box_size == 0)
            {
                ui4_sub_box_size = 4;
            }
                break;
        }
        ui4_total_sub_box_size += ui4_sub_box_size;
        if(ui4_total_sub_box_size >= ui4_udta_box_size)
        {
            break;
        }
        pf_set_pos(pt_minfo_obj, 
                    (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset
                                                        + ui4_total_sub_box_size, 
                    FM_SEEK_BGN);
    }
    return MINFOR_OK;
}

#if (ENABLE_CMOV == 1)
/*-----------------------------------------------------------------------------
 * Name:  _mp4_parse_box_cmov_udta
 *
 * Description: parse optional description box
 *
 * Inputs:  -
 *          
 * Outputs: - 
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
static INT32 _mp4_parse_box_cmov_udta(MINFO_OBJ_T*   pt_minfo_obj)
{
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    UINT32                          ui4_udta_box_size;
    UINT32                          ui4_total_sub_box_size = 0;
    UINT32                          ui4_sub_box_size;
    UINT32                          ui4_fourcc;
    UINT16                          ui2_str_len;
    UINT8*                          pui1_cmov_data_cur_pos;
    
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    pui1_cmov_data_cur_pos = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                           + pt_mp4_hdlr_obj->ui8_curr_parser_offset;

    LOADB_DWRD(pui1_cmov_data_cur_pos, ui4_udta_box_size);

    while(1)
    {    
        LOADB_DWRD(pui1_cmov_data_cur_pos, ui4_sub_box_size);
        LOADB_DWRD(pui1_cmov_data_cur_pos + 4, ui4_fourcc);
        switch(ui4_fourcc)
        {
            case MP4_BOX_NDRM:
            {
                UINT32 ui4_version;
                BOOL fgEnable = FALSE;
                BOOL fgCePlay = FALSE;
                pui1_cmov_data_cur_pos += 12;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += 12;
                LOADB_DWRD(pui1_cmov_data_cur_pos, ui4_version);
                fgEnable = (BOOL)((pui1_cmov_data_cur_pos[4] & 0x80) != 0);
                fgCePlay = (BOOL)((pui1_cmov_data_cur_pos[5] & 0x40) != 0);
                pt_mp4_hdlr_obj->t_generic_info.b_is_drm_exist
                            = (BOOL)((ui4_version == 1) && fgEnable && fgCePlay);
            }
                break;
            case MP4_BOX_NAM:
            {
                LOADB_WORD(pui1_cmov_data_cur_pos + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_title_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title, 0, sizeof(UTF16_T)*ui2_str_len);
                pui1_cmov_data_cur_pos += 12;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += 12;
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)pui1_cmov_data_cur_pos, 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_title, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_title_len = 0;
                }
                pui1_cmov_data_cur_pos += ui2_str_len;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui2_str_len;
            }
                break;
            case MP4_BOX_AUT:
            {
                LOADB_WORD(pui1_cmov_data_cur_pos + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_director_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director, 0, sizeof(UTF16_T)*ui2_str_len);
                pui1_cmov_data_cur_pos += 12;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += 12;
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)pui1_cmov_data_cur_pos, 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_director, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_director_len = 0;
                }
                pui1_cmov_data_cur_pos += ui2_str_len;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui2_str_len;
            }
                break;
            case MP4_BOX_CPY:
            {
                LOADB_WORD(pui1_cmov_data_cur_pos + 8, ui2_str_len);
                pt_mp4_hdlr_obj->t_meta_data_info.z_copyright_len = (SIZE_T)ui2_str_len;
                if(ui2_str_len == 0)
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright = NULL;
                    break;
                }
                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright
                        = _mm_util_mem_alloc(sizeof(UTF16_T)*ui2_str_len);
                if (pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright == NULL)
                {
                    return MINFOR_OUT_OF_MEMORY;
                }
                x_memset(pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright, 0, sizeof(UTF16_T)*ui2_str_len);
                pui1_cmov_data_cur_pos += 12;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += 12;
                if(UCR_OK != x_uc_ps_to_w2s((const CHAR*)pui1_cmov_data_cur_pos, 
                                pt_mp4_hdlr_obj->t_meta_data_info.pwc_str_copyright, 
                                ui2_str_len))
                {
                    pt_mp4_hdlr_obj->t_meta_data_info.z_copyright_len = 0;
                }
                pui1_cmov_data_cur_pos += ui2_str_len;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui2_str_len;
            }
                break;
            /*case MP4_BOX_META:
                ui4_sub_box_size = 8;process the next
                break;*/
            default:
            if(ui4_sub_box_size == 0)
            {
                ui4_sub_box_size = 4;
            }
                break;
        }
        ui4_total_sub_box_size += ui4_sub_box_size;
        if(ui4_total_sub_box_size >= ui4_udta_box_size)
        {
            break;
        }
    }
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _mp4_minfo_moov_track_info
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                The API is successful. 
 *          MINFOR_INTERNAL_ERR      The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _mp4_minfo_moov_track_info(MINFO_OBJ_T*  pt_minfo_obj )
{
    setpos_fct                      pf_set_pos;    
    copybytes_fct                   pf_copybyte;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    INT32                           i4_ret;
    UINT32                          ui4_box_size;
    UINT32                          ui4_fourcc;
    UINT64                          ui8_curr_offset;
    MP4_BOX_ID_ENUM_T               e_id;
    UINT8                           ui1_ver;
    UINT8*                          pui1_tmp;
    
    MINFO_MP4_VIDEO_STM_ATRBT_T*    pt_video_strm_info;
    MINFO_MP4_AUDIO_STM_ATRBT_T*    pt_audio_strm_info;   
    MINFO_MP4_SUBTITLE_STM_ATRBT_T* pt_sbtl_strm_info;
    MP4_BOX_T*                      pt_hdlr_box;
    MP4_BOX_T*                      pt_tkhd_box;
    MP4_BOX_T*                      pt_stsd_box;
    MP4_BOX_T*                      pt_mdhd_box;
    MP4_BOX_T*                      pt_stsz_box;
    UINT8                           ui1_strm_idx;
    UINT32                          ui4_sample_des_entry_num;
    UINT32                          ui4_read_size;
    UINT32                          ui4_high_word;
    UINT32                          ui4_video_sample_cnt;
    UINT64                          ui8_create_time = 0;
    
    
    /* check parameter */
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    
    switch(pt_mp4_hdlr_obj->eParserState)
    {
    case MP4_PARSER_STATE_IDLE:
        e_id = _mp4_get_box_id_and_size(pt_mp4_hdlr_obj->pui1_box_buf, &ui4_box_size);
        if(e_id == MP4_BOX_ID_MAX)
        {
            DBG_INFO(("----------- encount new box, add: 0x%.8X\r\n", 
                        pt_mp4_hdlr_obj->ui8_curr_parser_offset));
        }
        if(ui4_box_size == 0)
        {
            return MINFOR_INTERNAL_ERR;
        }
        if (e_id == MP4_BOX_ID_MVHD)
        {
            i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size - 8);
            if(i4_ret != MINFOR_OK)
            { 
                return MINFOR_INTERNAL_ERR;
            }
            i4_ret = pf_copybyte(pt_minfo_obj, 
                        pt_mp4_hdlr_obj->pui1_box_buf, 
                        pt_mp4_hdlr_obj->ui4_buf_length, 
                        ui4_box_size - 8, 
                        &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size - 8);
            ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf);
            if (ui1_ver == 1)
            {
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, 
                           pt_mp4_hdlr_obj->ui4_movie_timescale);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 24, ui4_high_word);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);

                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 8, ui8_create_time);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_high_word);
                ui8_create_time |= ((UINT64)ui4_high_word<<32);
            }
            else
            {
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, 
                           pt_mp4_hdlr_obj->ui4_movie_timescale);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 16, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 8, ui8_create_time);
            }
            _mm_gmtime((MM_TIME_T)ui8_create_time, 
                       MM_GMT_MP4_EPOCH_YEAR_M, 
                       &pt_mp4_hdlr_obj->t_meta_data_info.t_date);
            /*pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
            _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration*MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                    pt_mp4_hdlr_obj->ui4_movie_timescale, NULL);*/
        }
        else
        {
            if (e_id == MP4_BOX_ID_TRAK)
            {
                pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_CHECK_TRACK;
                if (pt_mp4_hdlr_obj->pt_tree_box != NULL)
                {
                    _mp4_delete_track_tree(pt_minfo_obj);
                }
                i4_ret = _mp4_add_box_to_tree(pt_minfo_obj, 
                                              e_id, 
                                              pt_mp4_hdlr_obj->ui8_curr_parser_offset-8, 
                                              ui4_box_size);
                if(i4_ret != MINFOR_OK)
                { 
                    return MINFOR_INTERNAL_ERR;
                }
            }
            else if (e_id == MP4_BOX_ID_UDTA)/*Get DRM Info*/
            {
                _mp4_parse_box_udta(pt_minfo_obj);
            }
            else if (e_id == MP4_BOX_ID_UUID)
            {
                UINT32 ui4_drm_flag;
                pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                    pt_mp4_hdlr_obj->pui1_box_buf, 
                                    pt_mp4_hdlr_obj->ui4_buf_length, 
                                    4, 
                                    &ui4_read_size);
                LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf, ui4_drm_flag);
                pt_mp4_hdlr_obj->t_generic_info.b_is_drm_exist
                        = (BOOL)(ui4_drm_flag == 0x1474EE4F);
            }
            else if(e_id == MP4_BOX_ID_MVEX)
            {
                _mp4_get_dur_from_mvex(pt_minfo_obj);
            }
            if (!_mp4_box_is_need_check_sub(e_id))
            {
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size-8);
                pf_set_pos(pt_minfo_obj , (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
            }
        }
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        else
        {
            i4_ret = pf_copybyte(pt_minfo_obj, 
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            if (i4_ret != INPUTR_OK)
            { 
                return MINFOR_INTERNAL_ERR;
            }
        }
        break;
        
    case MP4_PARSER_STATE_CHECK_TRACK:
        e_id = _mp4_get_box_id_and_size(pt_mp4_hdlr_obj->pui1_box_buf, &ui4_box_size);
        if(e_id == MP4_BOX_ID_MAX)
        {
            DBG_INFO(("----------- encount new box, add: 0x%.8X\r\n", 
                        pt_mp4_hdlr_obj->ui8_curr_parser_offset));
        }
        if(ui4_box_size == 0)
        {
            return MINFOR_INTERNAL_ERR;
        }
        if (_mp4_box_is_need_parser(e_id) || _mp4_box_is_need_check_sub(e_id) )
        {
            i4_ret = _mp4_add_box_to_tree(pt_minfo_obj, 
                                          e_id, 
                                          pt_mp4_hdlr_obj->ui8_curr_parser_offset-8, 
                                          ui4_box_size);
            if(i4_ret != MINFOR_OK)
            { 
                _mp4_delete_track_tree(pt_minfo_obj);
                return MINFOR_INTERNAL_ERR;
            }
        }
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size-8);
            pf_set_pos(pt_minfo_obj , (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
        }
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->pt_tree_box->ui8_offset + pt_mp4_hdlr_obj->pt_tree_box->ui4_size))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_TRACK_INF;
        }
        else
        {
            i4_ret = pf_copybyte(pt_minfo_obj, 
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            if (i4_ret != INPUTR_OK)
            { 
                return MINFOR_INTERNAL_ERR;
            }
        }
        break;
        
    case MP4_PARSER_STATE_LOAD_TRACK_INF:
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_HDLR, &pt_hdlr_box);
        if (pt_hdlr_box == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui4_box_size = pt_hdlr_box->ui4_size;
        ui8_curr_offset = pt_hdlr_box->ui8_offset;
        pf_set_pos(pt_minfo_obj , (INT64)ui8_curr_offset, FM_SEEK_BGN);
        
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 16, ui4_fourcc);
        if ((ui4_fourcc == MP4_TRAK_TYPE_VIDE)/*Only parse one video track*/
            &&(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num == 0))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF;
        }
        else if (ui4_fourcc == MP4_TRAK_TYPE_SOUN)
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF;
        }
        else if ((ui4_fourcc == MP4_TRAK_TYPE_GRAP) || (ui4_fourcc == MP4_TRAK_TYPE_SUBP))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_SUBTITLE_TRACK_INF;
        }       
        else
        {
            if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
                (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
            {
                _mp4_delete_track_tree(pt_minfo_obj);
                return MINFOR_OK;
            }
            pf_set_pos(pt_minfo_obj , (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            _mp4_delete_track_tree(pt_minfo_obj);
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        }
        break;    
       
    case MP4_PARSER_STATE_LOAD_SUBTITLE_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num += 1; 
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num > MAX_NS_MP4_SBTL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num - 1);
        pt_sbtl_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rSbtlStrmInf[ui1_strm_idx]);
        
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui8_curr_offset = pt_tkhd_box->ui8_offset;
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf+8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 96, pt_sbtl_strm_info->i4_width);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 100, pt_sbtl_strm_info->i4_height);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 84, pt_sbtl_strm_info->i4_width);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 88, pt_sbtl_strm_info->i4_height);
        }
        DBG_INFO(("[mp4_parser]sbtl track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track head box offset = 0x%lx \r\n", ui8_curr_offset));
        DBG_INFO(("[mp4_parser]sbtl track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]));

        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui8_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]sbtl track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track mdhd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, pt_sbtl_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 32, ui4_high_word);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 36, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, pt_sbtl_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }
        if(pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration == 0)
        {
            pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
                        _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration
                                            * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                    pt_sbtl_strm_info->ui4_timescale, 
                                    NULL);
        }
        
        DBG_INFO(("[mp4_parser]sbtl timescale = 0x%lx \r\n", pt_sbtl_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]sbtl duration = 0x%llx \r\n", pt_mp4_hdlr_obj->ui8_movie_duration));
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui8_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]sbtl track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track stsd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] sound sample description entry num = 1. \r\n"));
        }
        /* get data format 'stgs' ... */
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, ui4_fourcc);         
        if (ui4_fourcc != MP4_SBTL_STGS && ui4_fourcc != MP4_SBTL_MP4S)
        {/*skip not support track*/
            DBG_INFO(("[mp4_parser] Caution: sbtl track codec not support, SKIP IT!!! \r\n"));
            pt_mp4_hdlr_obj->ui1_uCurStrmNum -= 1;
            pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num -= 1;                      
        }        
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        if (ui4_fourcc == MP4_SBTL_STGS)
        {            
            pt_sbtl_strm_info->e_subtitle_type = MINFO_INFO_MP4_SUBTITLE_TYPE_BLUERAY;
        }
        else if (ui4_fourcc == MP4_SBTL_MP4S)
        {
            pt_sbtl_strm_info->e_subtitle_type = MINFO_INFO_MP4_SUBTITLE_TYPE_VOBSBTL;             
        }        
        pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             8, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;        
        break;

        
    case MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num += 1; 
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num > MAX_NS_MP4_VID)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num - 1);
        pt_video_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rVideStrmInf[ui1_strm_idx]);
        
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui8_curr_offset = pt_tkhd_box->ui8_offset;
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf+8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 96, pt_video_strm_info->i4_width);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 100, pt_video_strm_info->i4_height);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 84, pt_video_strm_info->i4_width);
            LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 88, pt_video_strm_info->i4_height);
        }
        DBG_INFO(("[mp4_parser]video track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track head box offset = 0x%lx \r\n", ui8_curr_offset));
        DBG_INFO(("[mp4_parser]video track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]));
        
        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui8_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]video track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track mdhd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, pt_video_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 32, ui4_high_word);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 36, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, pt_video_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }        
        pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
            _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration*MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                      pt_video_strm_info->ui4_timescale, 
                      NULL);
 
        DBG_INFO(("[mp4_parser]video timescale = 0x%lx \r\n", pt_video_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]video duration = 0x%llx \r\n", pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration));

      do
      {
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            DBG_INFO(("[mp4_parser] Caution: stsd box not exist!!! \r\n"));
#if NO_SKIP_UNSUPPORT_TRACK
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
            break;
            
#else
            pt_mp4_hdlr_obj->ui1_uCurStrmNum -= 1;
            pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num -= 1;
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
            _mp4_delete_track_tree(pt_minfo_obj);
            pf_set_pos(pt_minfo_obj, 
                        (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                        FM_SEEK_BGN);
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            break;
#endif
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui8_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]video track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track stsd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] NOTICE: video sample description entry num != 1\r\n"));
        }
        /* get data format 'mp4v' ... */
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, ui4_fourcc);
        pt_video_strm_info->e_enc = _mp4_video_codec(ui4_fourcc);
#if !NO_SKIP_UNSUPPORT_TRACK
        if (pt_video_strm_info->e_enc == MINFO_INFO_VID_ENC_UNKNOWN)
        {/*skip not support track*/
            DBG_INFO(("[mp4_parser] Caution: video track codec not support, SKIP IT!!! \r\n"));
            pt_mp4_hdlr_obj->ui1_uCurStrmNum -= 1;
            pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num -= 1;
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
            _mp4_delete_track_tree(pt_minfo_obj);
            pf_set_pos(pt_minfo_obj, 
                        (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                        FM_SEEK_BGN);
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            break;
        }
#endif
        /* get width and height */
        LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 48, pt_video_strm_info->i4_width);
        LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 50, pt_video_strm_info->i4_height);
        pui1_tmp = pt_mp4_hdlr_obj->pui1_box_buf;
        pui1_tmp += 102;
        i4_ret = _mp4_optional_description_parse(pt_minfo_obj, 
                                                 pui1_tmp, 
                                                 ui4_box_size - 102,
                                                 ui4_fourcc);
        if (i4_ret != MINFOR_OK)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
      }while(0);
        
        /* sample size box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, 
                            MP4_BOX_ID_STSZ, 
                            &pt_stsz_box);
        if (pt_stsz_box != NULL)
        {
            pf_set_pos(pt_minfo_obj, 
                        (INT64)pt_stsz_box->ui8_offset + 16, 
                        FM_SEEK_BGN);
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 4, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf, ui4_video_sample_cnt);
            /*FrameRate = SampleNum/(Duration/Scale)* timescale/timescale*/
            pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_rate = 
                (UINT32)_mm_div64((UINT64)ui4_video_sample_cnt 
                                    * pt_video_strm_info->ui4_timescale
                                    * pt_video_strm_info->ui4_timescale, 
                                    pt_mp4_hdlr_obj->ui8_movie_duration,
                                    NULL);
            pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_scale
                    = pt_video_strm_info->ui4_timescale;
            pt_video_strm_info->ui4_rate
                    = pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_rate;
            pt_video_strm_info->ui4_scale = pt_video_strm_info->ui4_timescale;
        }
        
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             8, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        break;
        
    case MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num += 1;        
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num > MAX_NS_MP4_AUD)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num - 1);
        pt_audio_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rAudStrmInf[ui1_strm_idx]);

         /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui8_curr_offset = pt_tkhd_box->ui8_offset;
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf+8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]);
        }
        DBG_INFO(("[mp4_parser]audio track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track head box offset = 0x%lx \r\n", ui8_curr_offset));
        DBG_INFO(("[mp4_parser]audio track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]));

        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui8_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]audio track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track mdhd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 28, pt_audio_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 32, ui4_high_word);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 36, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, pt_audio_strm_info->ui4_timescale);
            LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }
        if(pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration == 0)
        {
            pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
                        _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration
                                            * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                    pt_audio_strm_info->ui4_timescale, 
                                    NULL);
        }
        
        DBG_INFO(("[mp4_parser]audio timescale = 0x%lx \r\n", pt_audio_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]audio duration = 0x%llx \r\n", pt_mp4_hdlr_obj->ui8_movie_duration));
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui8_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]audio track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track stsd box offset = 0x%lx \r\n", ui8_curr_offset));
        pf_set_pos(pt_minfo_obj, (INT64)ui8_curr_offset, FM_SEEK_BGN);
        
        i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_box_size);
        if(i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             ui4_box_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] sound sample description entry num = 1. \r\n"));
        }
        /* get data format 'mp4a' ... */
        LOADB_DWRD(pt_mp4_hdlr_obj->pui1_box_buf + 20, ui4_fourcc);
        /*pt_audio_strm_info->e_enc = _mp4_get_aud_codec(ui4_fourcc);*/
        /* get audio sample size and sample rate */
        LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 40, pt_audio_strm_info->i2_channels);
        LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 42, pt_audio_strm_info->i4_sample_size);
        LOADB_WORD(pt_mp4_hdlr_obj->pui1_box_buf + 48, pt_audio_strm_info->i4_sample_rate);
        pui1_tmp = pt_mp4_hdlr_obj->pui1_box_buf;
        /*pui1_tmp += 52;*/
        i4_ret = _mp4_optional_description_parse(pt_minfo_obj, 
                                                 pui1_tmp, 
                                                 ui4_box_size,
                                                 ui4_fourcc);
        if (i4_ret != MINFOR_OK)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
#if !NO_SKIP_UNSUPPORT_TRACK
        if (pt_audio_strm_info->e_enc == MINFO_INFO_AUD_ENC_UNKNOWN)
        {/*skip not support track*/
            DBG_INFO(("[mp4_parser] Caution: audio track codec not support, SKIP IT!!! \r\n"));
            pt_mp4_hdlr_obj->ui1_uCurStrmNum -= 1;
            pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num -= 1;
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
            _mp4_delete_track_tree(pt_minfo_obj);
            pf_set_pos(pt_minfo_obj, 
                        (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                        FM_SEEK_BGN);
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 8, 
                                 &ui4_read_size);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            break;
        }
#endif
        
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             8, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        break;
        
    default:
        break;
    }
    
    return MINFO_MP4_PARSE_NEED_NEXT_LOOP;
}

#if (ENABLE_CMOV == 1)
/*-----------------------------------------------------------------------------
 * Name:  _mp4_minfo_cmov_track_info
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                The API is successful. 
 *          MINFOR_INTERNAL_ERR      The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _mp4_minfo_cmov_track_info(MINFO_OBJ_T*  pt_minfo_obj )
{
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj;
    INT32                           i4_ret;
    UINT32                          ui4_box_size;
    UINT32                          ui4_fourcc;
    UINT32                          ui4_curr_offset;
    MP4_BOX_ID_ENUM_T               e_id;
    UINT8                           ui1_ver;
    UINT8*                          pui1_tmp;
    
    MINFO_MP4_VIDEO_STM_ATRBT_T*    pt_video_strm_info;
    MINFO_MP4_AUDIO_STM_ATRBT_T*    pt_audio_strm_info;
    MINFO_MP4_SUBTITLE_STM_ATRBT_T* pt_sbtl_strm_info;
    MP4_BOX_T*                      pt_hdlr_box;
    MP4_BOX_T*                      pt_tkhd_box;
    MP4_BOX_T*                      pt_stsd_box;
    MP4_BOX_T*                      pt_mdhd_box;
    MP4_BOX_T*                      pt_stsz_box;
    UINT8                           ui1_strm_idx;
    UINT32                          ui4_sample_des_entry_num;
    UINT32                          ui4_high_word;
    UINT32                          ui4_video_sample_cnt;
    UINT64                          ui8_create_time = 0;
    UINT8*                          pui1_cmov_data = NULL;
    
    
    /* check parameter */
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }

    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;
    pt_mp4_hdlr_obj->ui8_curr_parser_offset = 8;/* here to skip 8(size+'moov')*/
    pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data + 8;
    
  while(1)
  {
    switch(pt_mp4_hdlr_obj->eParserState)
    {
    case MP4_PARSER_STATE_IDLE:
        e_id = _mp4_get_box_id_and_size(pui1_cmov_data, &ui4_box_size);
        if(e_id == MP4_BOX_ID_MAX)
        {
            DBG_INFO(("----------- encount new box, add: 0x%.8X\r\n", 
                        pt_mp4_hdlr_obj->ui8_curr_parser_offset));
        }
        if(ui4_box_size == 0)
        {
            if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
            {
                return MINFOR_OK;
            }
            return MINFOR_INTERNAL_ERR;
        }
        if (e_id == MP4_BOX_ID_MVHD)
        {
            pui1_cmov_data += 8;
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            ui1_ver = *(pui1_cmov_data);
            if (ui1_ver == 1)
            {
                LOADB_DWRD(pui1_cmov_data + 8, ui8_create_time);
                LOADB_DWRD(pui1_cmov_data + 12, ui4_high_word);
                ui8_create_time |= ((UINT64)ui4_high_word<<32);

                LOADB_DWRD(pui1_cmov_data + 20, 
                           pt_mp4_hdlr_obj->ui4_movie_timescale);
                
                LOADB_DWRD(pui1_cmov_data + 24, ui4_high_word);
                LOADB_DWRD(pui1_cmov_data + 28, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
            }
            else
            {
                LOADB_DWRD(pui1_cmov_data + 8, ui8_create_time);
                LOADB_DWRD(pui1_cmov_data + 12, 
                           pt_mp4_hdlr_obj->ui4_movie_timescale);
                LOADB_DWRD(pui1_cmov_data + 16, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
            }
            pui1_cmov_data += (ui4_box_size - 8);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size - 8);
            _mm_gmtime((MM_TIME_T)ui8_create_time, 
                       MM_GMT_MP4_EPOCH_YEAR_M, 
                       &pt_mp4_hdlr_obj->t_meta_data_info.t_date);
        }
        else
        {
            if (e_id == MP4_BOX_ID_TRAK)
            {
                pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_CHECK_TRACK;
                if (pt_mp4_hdlr_obj->pt_tree_box != NULL)
                {
                    _mp4_delete_track_tree(pt_minfo_obj);
                }
                i4_ret = _mp4_add_box_to_tree(pt_minfo_obj, 
                                              e_id, 
                                              pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                                              ui4_box_size);
                if(i4_ret != MINFOR_OK)
                { 
                    return MINFOR_INTERNAL_ERR;
                }
                pui1_cmov_data += 8;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
            }
            else if (e_id == MP4_BOX_ID_UDTA)/*Get DRM Info*/
            {
                _mp4_parse_box_cmov_udta(pt_minfo_obj);
                pui1_cmov_data = 
                   (pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + pt_mp4_hdlr_obj->ui8_curr_parser_offset);
            }
            else if (e_id == MP4_BOX_ID_UUID)
            {
                UINT32 ui4_drm_flag;
                pui1_cmov_data += 8;
                LOADB_DWRD(pui1_cmov_data, ui4_drm_flag);
                pt_mp4_hdlr_obj->t_generic_info.b_is_drm_exist
                        = (BOOL)(ui4_drm_flag == 0x1474EE4F);
            }
            if (!_mp4_box_is_need_check_sub(e_id))
            {
                pui1_cmov_data += ui4_box_size;
                pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui4_box_size;
            }
        }
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
        {
            return MINFOR_OK;
        }
        break;
        
    case MP4_PARSER_STATE_CHECK_TRACK:
        e_id = _mp4_get_box_id_and_size(pui1_cmov_data, &ui4_box_size);
        if(e_id == MP4_BOX_ID_MAX)
        {
            DBG_INFO(("----------- encount new box, add: 0x%.8X\r\n", 
                        pt_mp4_hdlr_obj->ui8_curr_parser_offset));
        }
        if(ui4_box_size == 0)
        {
            if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
            {
                pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_TRACK_INF;
                continue;
            }
            return MINFOR_INTERNAL_ERR;
        }
        if (_mp4_box_is_need_parser(e_id) || _mp4_box_is_need_check_sub(e_id) )
        {
            i4_ret = _mp4_add_box_to_tree(pt_minfo_obj, 
                                          e_id, 
                                          pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                                          ui4_box_size);
            if(i4_ret != MINFOR_OK)
            { 
                _mp4_delete_track_tree(pt_minfo_obj);
                return MINFOR_INTERNAL_ERR;
            }
        }
        pui1_cmov_data += 8;
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        
        if (!_mp4_box_is_need_check_sub(e_id))
        {
            pui1_cmov_data += (ui4_box_size - 8);
            pt_mp4_hdlr_obj->ui8_curr_parser_offset += (ui4_box_size - 8);
        }
        if(pt_mp4_hdlr_obj->pt_tree_box == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->pt_tree_box->ui8_offset + pt_mp4_hdlr_obj->pt_tree_box->ui4_size))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_TRACK_INF;
        }
        
        break;
        
    case MP4_PARSER_STATE_LOAD_TRACK_INF:
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_HDLR, &pt_hdlr_box);
        if (pt_hdlr_box == NULL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui4_box_size = pt_hdlr_box->ui4_size;
        ui4_curr_offset = pt_hdlr_box->ui8_offset;
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        
        LOADB_DWRD(pui1_cmov_data + 16, ui4_fourcc);
        if ((ui4_fourcc == MP4_TRAK_TYPE_VIDE)/*Only parse one video track*/
            &&(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num == 0))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF;
        }
        else if (ui4_fourcc == MP4_TRAK_TYPE_SOUN)
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF;
        }
        else if ((ui4_fourcc == MP4_TRAK_TYPE_GRAP) || (ui4_fourcc == MP4_TRAK_TYPE_SUBP))
        {
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_LOAD_SUBTITLE_TRACK_INF;
        }  
        else
        {
            if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
            {
                _mp4_delete_track_tree(pt_minfo_obj);
                return MINFOR_OK;
            }

            pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + pt_mp4_hdlr_obj->ui8_curr_parser_offset;
            _mp4_delete_track_tree(pt_minfo_obj);
            pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        }
        break;
        
    case MP4_PARSER_STATE_LOAD_SUBTITLE_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num += 1; 
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num > MAX_NS_MP4_SBTL)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num - 1);
        pt_sbtl_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rSbtlStrmInf[ui1_strm_idx]);
        
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui4_curr_offset = pt_tkhd_box->ui8_offset;
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        ui1_ver = *(pui1_cmov_data + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pui1_cmov_data + 96, pt_sbtl_strm_info->i4_width);
            LOADB_WORD(pui1_cmov_data + 100, pt_sbtl_strm_info->i4_height);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pui1_cmov_data + 84, pt_sbtl_strm_info->i4_width);
            LOADB_WORD(pui1_cmov_data + 88, pt_sbtl_strm_info->i4_height);
        }
        DBG_INFO(("[mp4_parser]sbtl track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track head box offset = 0x%lx \r\n", ui4_curr_offset));
        DBG_INFO(("[mp4_parser]sbtl track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_sbtl_strm_ID[ui1_strm_idx]));

        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui4_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]sbtl track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track mdhd box offset = 0x%lx \r\n", ui4_curr_offset));
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        ui1_ver = *(pt_mp4_hdlr_obj->pui1_box_buf + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, pt_sbtl_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 32, ui4_high_word);
            LOADB_DWRD(pui1_cmov_data + 36, pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, pt_sbtl_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }
        if(pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration == 0)
        {
            pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
                        _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration
                                            * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                    pt_sbtl_strm_info->ui4_timescale, 
                                    NULL);
        }
        
        DBG_INFO(("[mp4_parser]sbtl timescale = 0x%lx \r\n", pt_sbtl_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]sbtl duration = 0x%llx \r\n", pt_mp4_hdlr_obj->ui8_movie_duration));
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui4_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]sbtl track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]sbtl track stsd box offset = 0x%lx \r\n", ui4_curr_offset));
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
                
        LOADB_DWRD(pui1_cmov_data + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] sound sample description entry num = 1. \r\n"));
        }
        /* get data format 'stgs' ... */
        LOADB_DWRD(pui1_cmov_data + 20, ui4_fourcc);         
        if (ui4_fourcc != MP4_SBTL_STGS && ui4_fourcc != MP4_SBTL_MP4S)
        {/*skip not support track*/
            DBG_INFO(("[mp4_parser] Caution: sbtl track codec not support, SKIP IT!!! \r\n"));
            pt_mp4_hdlr_obj->ui1_uCurStrmNum -= 1;
            pt_mp4_hdlr_obj->t_strm_Info.ui1_sbtl_strm_num -= 1;                      
        }        
        if (pt_mp4_hdlr_obj->ui8_curr_parser_offset >= 
            (pt_mp4_hdlr_obj->ui8_moov_offset + pt_mp4_hdlr_obj->ui4_moov_size))
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        if (ui4_fourcc == MP4_SBTL_STGS)
        {            
            pt_sbtl_strm_info->e_subtitle_type = MINFO_INFO_MP4_SUBTITLE_TYPE_BLUERAY;
        }
        else if (ui4_fourcc == MP4_SBTL_MP4S)
        {
            pt_sbtl_strm_info->e_subtitle_type = MINFO_INFO_MP4_SUBTITLE_TYPE_VOBSBTL;             
        }
        
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + pt_mp4_hdlr_obj->ui8_curr_parser_offset;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;        
        break;

        
    case MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num += 1;
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num > MAX_NS_MP4_VID)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_video_strm_num - 1);
        pt_video_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rVideStrmInf[ui1_strm_idx]);
        
        /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui4_curr_offset = pt_tkhd_box->ui8_offset;

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        ui1_ver = *(pui1_cmov_data + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pui1_cmov_data + 96, pt_video_strm_info->i4_width);
            LOADB_WORD(pui1_cmov_data + 100, pt_video_strm_info->i4_height);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]);
            LOADB_WORD(pui1_cmov_data + 84, pt_video_strm_info->i4_width);
            LOADB_WORD(pui1_cmov_data + 88, pt_video_strm_info->i4_height);
        }
        DBG_INFO(("[mp4_parser]video track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track head box offset = 0x%lx \r\n", ui4_curr_offset));
        DBG_INFO(("[mp4_parser]video track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_video_strm_ID[ui1_strm_idx]));
        
        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui4_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]video track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track mdhd box offset = 0x%lx \r\n", ui4_curr_offset));
        
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        ui1_ver = *(pui1_cmov_data + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, pt_video_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 32, ui4_high_word);
            LOADB_DWRD(pui1_cmov_data + 36, 
                           pt_mp4_hdlr_obj->ui8_movie_duration);
                pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, pt_video_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }        
        pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
            _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration*MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                      pt_video_strm_info->ui4_timescale, 
                      NULL);
 
        DBG_INFO(("[mp4_parser]video timescale = 0x%lx \r\n", pt_video_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]video duration = 0x%llx \r\n", pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration));

      do
      {
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            DBG_INFO(("[mp4_parser] Caution: stsd box not exist!!! \r\n"));
            pt_video_strm_info->e_enc = MINFO_INFO_VID_ENC_UNKNOWN;
            break;
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui4_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]video track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]video track stsd box offset = 0x%lx \r\n", ui4_curr_offset));

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                        + ui4_curr_offset;
        
        LOADB_DWRD(pui1_cmov_data + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] video sample description entry num = 1\r\n"));
        }
        /* get data format 'mp4v' ... */
        LOADB_DWRD(pui1_cmov_data + 20, ui4_fourcc);
        pt_video_strm_info->e_enc = _mp4_video_codec(ui4_fourcc);
        /* get width and height */
        LOADB_WORD(pui1_cmov_data + 48, pt_video_strm_info->i4_width);
        LOADB_WORD(pui1_cmov_data + 50, pt_video_strm_info->i4_height);
        pui1_tmp = pui1_cmov_data;
        pui1_tmp += 102;
        i4_ret = _mp4_optional_description_parse(pt_minfo_obj, 
                                                 pui1_tmp, 
                                                 ui4_box_size - 102,
                                                 ui4_fourcc);
        if (i4_ret != MINFOR_OK)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
      }while(0);
        
        /* sample size box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, 
                            MP4_BOX_ID_STSZ, 
                            &pt_stsz_box);
        if (pt_stsz_box != NULL)
        {
            pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + pt_stsz_box->ui8_offset + 16;
            LOADB_DWRD(pui1_cmov_data, ui4_video_sample_cnt);
            /*FrameRate = SampleNum/(Duration/Scale)* timescale/timescale*/
            pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_rate = 
                (UINT32)_mm_div64((UINT64)ui4_video_sample_cnt 
                                    * pt_video_strm_info->ui4_timescale
                                    * pt_video_strm_info->ui4_timescale, 
                                    pt_mp4_hdlr_obj->ui8_movie_duration,
                                    NULL);
            pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_scale
                    = pt_video_strm_info->ui4_timescale;
            pt_video_strm_info->ui4_rate
                    = pt_mp4_hdlr_obj->t_meta_data_info.t_frame_rate.ui4_rate;
            pt_video_strm_info->ui4_scale = pt_video_strm_info->ui4_timescale;
        }
        
        if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
        {
            return MINFOR_OK;
        }

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + pt_mp4_hdlr_obj->ui8_curr_parser_offset;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        break;
        
    case MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF:
        pt_mp4_hdlr_obj->ui1_uCurStrmNum += 1;
        pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num += 1;
        if(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num > MAX_NS_MP4_AUD)
        {
            return MINFOR_INTERNAL_ERR;
        }
        ui1_strm_idx = (UINT8)(pt_mp4_hdlr_obj->t_strm_Info.ui1_audio_strm_num - 1);
        pt_audio_strm_info = &(pt_mp4_hdlr_obj->t_strm_Info.rAudStrmInf[ui1_strm_idx]);

         /* track header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_TKHD, &pt_tkhd_box);
        if (pt_tkhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_tkhd_box->ui4_size;
        ui4_curr_offset = pt_tkhd_box->ui8_offset;

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + ui4_curr_offset;
        ui1_ver = *(pui1_cmov_data + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, 
                pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, 
                pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]);
        }
        DBG_INFO(("[mp4_parser]audio track head box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track head box offset = 0x%lx \r\n", ui4_curr_offset));
        DBG_INFO(("[mp4_parser]audio track stream ID = 0x%lx \r\n", 
            pt_mp4_hdlr_obj->t_strm_Info.t_audio_strm_ID[ui1_strm_idx]));

        /* Media header box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_MDHD, &pt_mdhd_box);
        if (pt_mdhd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_mdhd_box->ui4_size;
        ui4_curr_offset = pt_mdhd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]audio track mdhd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track mdhd box offset = 0x%lx \r\n", ui4_curr_offset));

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + ui4_curr_offset;
        ui1_ver = *(pui1_cmov_data + 8);
        if (ui1_ver == 1)
        {
            LOADB_DWRD(pui1_cmov_data + 28, pt_audio_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 32, ui4_high_word);
            LOADB_DWRD(pui1_cmov_data + 36, pt_mp4_hdlr_obj->ui8_movie_duration);
            pt_mp4_hdlr_obj->ui8_movie_duration |= ((UINT64)ui4_high_word<<32);
        }
        else
        {
            LOADB_DWRD(pui1_cmov_data + 20, pt_audio_strm_info->ui4_timescale);
            LOADB_DWRD(pui1_cmov_data + 24, pt_mp4_hdlr_obj->ui8_movie_duration);
        }
        if(pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration == 0)
        {
            pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = 
                        _mm_div64(pt_mp4_hdlr_obj->ui8_movie_duration
                                            * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                    pt_audio_strm_info->ui4_timescale, 
                                    NULL);
        }
        
        DBG_INFO(("[mp4_parser]audio timescale = 0x%lx \r\n", pt_audio_strm_info->ui4_timescale));
        DBG_INFO(("[mp4_parser]audio duration = 0x%llx \r\n", pt_mp4_hdlr_obj->ui8_movie_duration));
        /* sample description box */
        _mp4_get_box_by_id(pt_mp4_hdlr_obj->pt_tree_box, MP4_BOX_ID_STSD, &pt_stsd_box);
        if (pt_stsd_box == NULL)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        ui4_box_size = pt_stsd_box->ui4_size;
        ui4_curr_offset = pt_stsd_box->ui8_offset;
        DBG_INFO(("[mp4_parser]audio track stsd box size = 0x%lx \r\n", ui4_box_size));
        DBG_INFO(("[mp4_parser]audio track stsd box offset = 0x%lx \r\n", ui4_curr_offset));

        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + ui4_curr_offset;
        
        LOADB_DWRD(pui1_cmov_data + 12, ui4_sample_des_entry_num);
        if (ui4_sample_des_entry_num != 1)
        {
            DBG_ERROR(("[mp4_parser.c] sound sample description entry num = 1. \r\n"));
        }
        /* get data format 'mp4a' ... */
        LOADB_DWRD(pui1_cmov_data + 20, ui4_fourcc);
        /*pt_audio_strm_info->e_enc = _mp4_get_aud_codec(ui4_fourcc);*/
        /* get audio sample size and sample rate */
        LOADB_WORD(pui1_cmov_data + 40, pt_audio_strm_info->i2_channels);
        LOADB_WORD(pui1_cmov_data + 42, pt_audio_strm_info->i4_sample_size);
        LOADB_WORD(pui1_cmov_data + 48, pt_audio_strm_info->i4_sample_rate);
        pui1_tmp = pui1_cmov_data;
        i4_ret = _mp4_optional_description_parse(pt_minfo_obj, 
                                                 pui1_tmp, 
                                                 ui4_box_size,
                                                 ui4_fourcc);
        if (i4_ret != MINFOR_OK)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_INTERNAL_ERR;
        }
        
        if(pt_mp4_hdlr_obj->ui8_curr_parser_offset 
                >= pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size)
        {
            _mp4_delete_track_tree(pt_minfo_obj);
            return MINFOR_OK;
        }
        pui1_cmov_data = pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                            + pt_mp4_hdlr_obj->ui8_curr_parser_offset;
        _mp4_delete_track_tree(pt_minfo_obj);
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;
        break;
        
    default:
        break;
    }
  };
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_decompress_cmov_box
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
INT32 mp4_minfo_decompress_cmov_box (MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32                   i4_ret;
    UINT32                  ui4_read_size;
    UINT32                  ui4_zip_code, ui4_uncomp_size;
    HANDLE_T                h_zip_engine;
    copybytes_fct           pf_copybyte;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_hdlr_obj;
    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj; 

    pt_mp4_hdlr_obj->t_mp4_box_cmov.b_is_cmov_box_exist = TRUE;
    pt_mp4_hdlr_obj->t_mp4_box_cmov.ui8_cmov_box_pos
                                = pt_mp4_hdlr_obj->ui8_moov_offset + 16;                
    pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data
                 = _mm_util_mem_alloc(sizeof(UINT8)
                            *pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size);
    if(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    i4_ret = pf_copybyte(pt_minfo_obj,
                         pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data, 
                         pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size, 
                         pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size, 
                         &ui4_read_size);
    if((i4_ret != INPUTR_OK)
        || (ui4_read_size != pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size))
    {
        return MINFOR_INTERNAL_ERR;
    }
                
    LOADB_DWRD(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data + 8, 
                ui4_zip_code);
    if(ui4_zip_code != MP4_CMOV_ZIP_CODE_ZLIB)
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
    
    LOADB_DWRD(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data + 20, 
                ui4_uncomp_size);
    if(ui4_uncomp_size > 0x100000)/* temp support max 1M*/
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    
    if (x_init_zlib_eng(SN_CL_ZIP, NULL, NULL, &h_zip_engine) != CLR_OK)
    {
        DBG_ERROR(("mp4 minfo unzip engine initialization failed.\n"));
        return MINFOR_INTERNAL_ERR;
    }           
    
    pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size = ui4_uncomp_size;
    pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                           = _mm_util_mem_alloc(sizeof(UINT8)*ui4_uncomp_size);
    if(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data, 0, ui4_uncomp_size);
    /* decompressed start*/
    i4_ret = x_do_zlib_decompress(h_zip_engine,
                    (pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data+26),
                    (pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size-26),
                    0,
                    CL_NULL_PARAM_TYPE,
                    NULL,
                    pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data,
                    (SIZE_T*)&ui4_uncomp_size,
                    NULL);
    if(i4_ret != MINFOR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    x_close_zlib_eng(h_zip_engine);
    _mm_util_mem_free(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data);
    pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data = NULL;
#if 0
    {
        UINT32 ui4_cnt = 0;
        for(; ui4_cnt<ui4_uncomp_size; ui4_cnt++)
        {
            if((ui4_cnt&0x0F) == 0)
            {
                DBG_INFO(("0x%.8x ", ui4_cnt));
            }
            DBG_INFO((" %.2x", pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data[ui4_cnt]));
            if((ui4_cnt&0x0F) == 0x0F)
            {
                DBG_INFO(("\r\n"));
            }
        }
    }
#endif
    return MINFOR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  _mp4_minfo_marlin_mdat_info
 *
 * Description: 
 *
 * Inputs:  -pt_minfo_obj
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                The API is successful. 
 *          MINFOR_INTERNAL_ERR      The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _mp4_minfo_marlin_mdat_info(MINFO_OBJ_T*  pt_minfo_obj )
{
    BOOL                            fg_ipmp_id_got  = 0;
    INT32                           i4_ret          = 0;
    UINT16                          ui2_value       = 0;
    UINT32                          ui4_moov_size   = 0;
    UINT32                          ui4_box_size    = 0;
    UINT32                          ui4_box_type    = 0;
    UINT32                          ui4_read_size   = 0;
    UINT32                          ui4_curr_offset = 0;  
    UINT32                          ui4_trak_type   = 0;  
    UINT8*                          pui1_mdat_buf   = NULL;
    setpos_fct                      pf_set_pos      = NULL;  
    input4bytes_fct                 pf_inp4byte     = NULL;  
    copybytes_fct                   pf_copybyte     = NULL;
    MINFO_MP4_MOOV_INF_T*           pt_mp4_hdlr_obj = NULL;  
    MINFO_MP4_MARLIN_TRACK_INFO_T   t_mln_trak_info;
    
    /* check parameter */
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    x_memset(&t_mln_trak_info, 0, sizeof(MINFO_MP4_MARLIN_TRACK_INFO_T));
    
    pf_set_pos      = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte     = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_copybyte     = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;    
    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj;

    pf_set_pos(pt_minfo_obj, (INT64)pt_mp4_hdlr_obj->ui8_data_offset, FM_SEEK_BGN);
    i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_moov_size);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    i4_ret = _mp4_check_box_buf(pt_mp4_hdlr_obj, ui4_moov_size - 4);
    if(i4_ret != MINFOR_OK)
    { 
        return MINFOR_INTERNAL_ERR;
    }
    i4_ret = pf_copybyte(pt_minfo_obj, 
                         pt_mp4_hdlr_obj->pui1_box_buf, 
                         pt_mp4_hdlr_obj->ui4_buf_length, 
                         ui4_moov_size - 4, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
#ifndef __KERNEL__
        x_dbg_stmt("============= Begin Parse ============== \r\n");
#endif
    pui1_mdat_buf   = pt_mp4_hdlr_obj->pui1_box_buf + ui4_curr_offset;
    
    while(ui4_curr_offset < ui4_moov_size - 4)
    {
        if(!fg_ipmp_id_got)
        {
            /* Find 0xA551 first */
            while(ui4_curr_offset < ui4_moov_size - 4)
            {
                LOADB_WORD(pui1_mdat_buf, ui2_value);
                if(ui2_value == 0xA551)
                {
                    LOAD_BYTE(pui1_mdat_buf - 1, 
                              t_mln_trak_info.ui1_ipmp_id);
                    ui4_curr_offset += 2;
                    pui1_mdat_buf   += 2;
                    fg_ipmp_id_got   = TRUE;
                    break;
                }
                ui4_curr_offset++;
                pui1_mdat_buf++;
            }
        }
        
        LOADB_DWRD(pui1_mdat_buf + 0, ui4_box_size);
        LOADB_DWRD(pui1_mdat_buf + 4, ui4_box_type);

        switch(ui4_box_type)
        {
        case MP4_BOX_SCHM: /* can check "ACBC" here */
        case MP4_BOX_KSID:  
        case MP4_BOX_KYID:
        case MP4_BOX_EALG:         
            pui1_mdat_buf   = pui1_mdat_buf + ui4_box_size;
            ui4_curr_offset+= ui4_box_size;
            break;
        case MP4_BOX_8ID :
            t_mln_trak_info.pui1_ct_id = 
                    (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * (ui4_box_size - 8));
            if(t_mln_trak_info.pui1_ct_id == NULL)
            {
                return MINFOR_OUT_OF_MEMORY;
            }
            x_memset(t_mln_trak_info.pui1_ct_id, 0, ui4_box_size - 8);
            x_memcpy(t_mln_trak_info.pui1_ct_id, 
                     pui1_mdat_buf + 8, 
                     ui4_box_size - 9);
            t_mln_trak_info.ui4_ct_id_length = ui4_box_size - 9;
            
            pui1_mdat_buf   = pui1_mdat_buf + ui4_box_size;
            ui4_curr_offset+= ui4_box_size;
            break;

        case MP4_BOX_STYP:
            t_mln_trak_info.pui1_satr_ctm = 
                    (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * (ui4_box_size + 8));
            if(t_mln_trak_info.pui1_satr_ctm == NULL)
            {
                return MINFOR_OUT_OF_MEMORY;
            }
            x_memset(t_mln_trak_info.pui1_satr_ctm, 0, ui4_box_size - 8);
            x_memcpy(t_mln_trak_info.pui1_satr_ctm, 
                     pui1_mdat_buf - 8, 
                     ui4_box_size + 8);
            t_mln_trak_info.ui4_satr_ctm_length = ui4_box_size + 8;
            
            LOADB_DWRD(pui1_mdat_buf + ui4_box_size - 6, ui4_trak_type);/*check video/audio*/
            
            pui1_mdat_buf   = pui1_mdat_buf + ui4_box_size;
            ui4_curr_offset+= ui4_box_size;
            break;

        case MP4_BOX_HMAC:
            t_mln_trak_info.pui1_satr_hmac_val = 
                    (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * (ui4_box_size - 8));
            if(t_mln_trak_info.pui1_satr_hmac_val == NULL)
            {
                return MINFOR_OUT_OF_MEMORY;
            }
            x_memset(t_mln_trak_info.pui1_satr_hmac_val, 0, ui4_box_size - 8);
            x_memcpy(t_mln_trak_info.pui1_satr_hmac_val, 
                     pui1_mdat_buf + 8, 
                     ui4_box_size - 8);
            t_mln_trak_info.ui4_satr_algo_type = ui4_box_size - 8;
            
            pui1_mdat_buf   = pui1_mdat_buf + ui4_box_size;
            ui4_curr_offset+= ui4_box_size;

            /* Store current trak info and prepare to get next trak infor */
            if(ui4_trak_type == MP4_TRAK_TYPE_VIDE)
            {
                x_memcpy(&(pt_mp4_hdlr_obj->t_strm_Info.at_marlin_track_info[0]),
                         &t_mln_trak_info,
                         sizeof(MINFO_MP4_MARLIN_TRACK_INFO_T));
            }
            else if(ui4_trak_type == MP4_TRAK_TYPE_AUDI)
            {
                x_memcpy(&(pt_mp4_hdlr_obj->t_strm_Info.at_marlin_track_info[1]),
                         &t_mln_trak_info,
                         sizeof(MINFO_MP4_MARLIN_TRACK_INFO_T));
            }
            else
            {
            /* Not support yet */
            }
#ifndef __KERNEL__
{
        UINT8           ui1_i = 0;
        x_dbg_stmt("\r\n\r\n8id box content length:%d, Data:%s \r\n", 
                    t_mln_trak_info.ui4_ct_id_length,
                    t_mln_trak_info.pui1_ct_id);
        x_dbg_stmt("ui1_ipmp_id:%d, ui4_satr_algo_type:%d \r\n", 
                    t_mln_trak_info.ui1_ipmp_id,
                    t_mln_trak_info.ui4_satr_algo_type);
        x_dbg_stmt("satr box content length:%d, Data:%s \r\n", 
                    t_mln_trak_info.ui4_satr_ctm_length,
                    t_mln_trak_info.pui1_satr_ctm);
        x_dbg_stmt("hmac content:");
        for(ui1_i = 0; ui1_i < t_mln_trak_info.ui4_satr_algo_type; ui1_i++)
        {
            x_dbg_stmt("%2x ", *(t_mln_trak_info.pui1_satr_hmac_val + ui1_i));
        }
}
#endif
            x_memset(&t_mln_trak_info, 0, sizeof(MINFO_MP4_MARLIN_TRACK_INFO_T));
            fg_ipmp_id_got = FALSE;
            break;

        case MP4_BOX_SINF:
        case MP4_BOX_SCHI:
        case MP4_BOX_SATR:
        default :
            pui1_mdat_buf   = pui1_mdat_buf + 8;
            ui4_curr_offset+= 8;
            break;
        }
    }
#ifndef __KERNEL__
        x_dbg_stmt("============= Finished Parse ============== \r\n");
#endif
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_box_parse
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
INT32 mp4_minfo_box_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    setpos_fct              pf_set_pos;    
    getpos_fct              pf_get_pos;   
    copybytes_fct           pf_copybyte;
    input4bytes_fct         pf_inp4byte;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_hdlr_obj;
    UINT64                  ui8_box_size;
    UINT32                  ui4_fourcc_type;
    UINT32                  ui4_read_size;
    INT32                   i4_ret;
    UINT32                  ui4_size, ui4_size2;
    BOOL                    b_is_moov_found = FALSE;
    BOOL                    b_is_mdat_found = FALSE;
    BOOL                    b_is_mfra_found = FALSE;
    BOOL                    b_is_moof_found = FALSE;
    UINT8                   ui1_check_box_cnt = 10;
    UINT64                  ui8_total_file_size     = 0;
    UINT64                  ui8_total_file_duration = 0;
        

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        DBG_ERROR(("ERROR: mp4_minfo_box_parse fail.1 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        DBG_ERROR(("ERROR: mp4_minfo_box_parse fail.2 \r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pt_mp4_hdlr_obj = (MINFO_MP4_MOOV_INF_T*)pt_minfo_obj->pv_handler_obj; 
    i4_ret = MINFOR_INTERNAL_ERR;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*seek to beginning*/
    i4_ret = pf_set_pos(pt_minfo_obj , 0, FM_SEEK_BGN);
    pt_mp4_hdlr_obj->ui8_curr_parser_offset = 0;

    /* get moov box offset and size */
    /*while (i4_ret >= pt_mp4_hdlr_obj->ui8_curr_parser_offset) */
    while (i4_ret == INPUTR_OK)
    {
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_size);
        ui8_box_size = (UINT64)ui4_size;
        if (i4_ret != INPUTR_OK)
        {
            if(i4_ret == INPUTR_EOF)
            {
                break;
            }
            DBG_ERROR(("ERROR: mp4_minfo_box_parse fail.3 \r\n"));
            return MINFOR_INTERNAL_ERR;
        }        
        pf_inp4byte(pt_minfo_obj, &ui4_fourcc_type);
        
        if (ui8_box_size == 1)
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size);
            pf_inp4byte(pt_minfo_obj, &ui4_size2);
            ui8_box_size = ((UINT64)ui4_size << 32) + ui4_size2;
        }

        if (ui8_box_size == 0) /* to end of file */
        {
            DBG_ERROR(("ERROR: mp4_minfo_box_parse fail.4 \r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        ui1_check_box_cnt--;
        switch(ui4_fourcc_type)
        {
        case MP4_BOX_FLSZ:
        {
            UINT8   ui1_i               = 0;
            UINT8   ui1_file_num        = (ui4_size - 8)/8;
            UINT32  ui4_read_size       = 0;

            ui8_total_file_size = ui4_size;
            
            if(ui1_file_num > MAX_SUPPORT_FILE_NUM)
            {
                ui1_file_num = MAX_SUPPORT_FILE_NUM;
            }
            pt_mp4_hdlr_obj->t_mp4_multi_file_info.ui1_file_num = ui1_file_num;
            
            i4_ret = pf_copybyte(pt_minfo_obj, 
                                 pt_mp4_hdlr_obj->pui1_box_buf, 
                                 pt_mp4_hdlr_obj->ui4_buf_length, 
                                 ui4_size - 8, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }

            for(ui1_i=0; ui1_i<ui1_file_num; ui1_i++)
            {
                UINT32  ui4_file_size        = 0;
                UINT32  ui4_file_duration    = 0;
                
                pt_mp4_hdlr_obj->t_mp4_multi_file_info.at_file_info[ui1_i].ui8_file_start_pos 
                    = ui8_total_file_size;
                pt_mp4_hdlr_obj->t_mp4_multi_file_info.at_file_info[ui1_i].ui8_file_start_pts 
                    = ui8_total_file_duration;
                
                LOADB_DWRD( pt_mp4_hdlr_obj->pui1_box_buf + sizeof(UINT32)*ui1_i*2, 
                            ui4_file_size);
                LOADB_DWRD( pt_mp4_hdlr_obj->pui1_box_buf + sizeof(UINT32)*(ui1_i*2+1), 
                            ui4_file_duration);
                pt_mp4_hdlr_obj->t_mp4_multi_file_info.at_file_info[ui1_i].ui8_file_duration
                    = ui4_file_duration;
                
                ui8_total_file_size     += ui4_file_size;
                ui8_total_file_duration += ui4_file_duration;
            }
            pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = ui8_total_file_duration;
            break;
        }
        case MP4_BOX_FTYP:
            /* check if marlin file */
            pf_inp4byte(pt_minfo_obj, &ui4_fourcc_type);
            if ((ui4_fourcc_type == MP4_BRAND_MGSV) ||
                (ui4_fourcc_type == MP4_BRAND_MSNV))
            {
                pt_mp4_hdlr_obj->t_piff_inet_info.b_is_bivl_file = TRUE;
            }
            break;
        case MP4_BOX_MOOV:
            b_is_moov_found = TRUE;
            pt_mp4_hdlr_obj->ui8_moov_offset = pt_mp4_hdlr_obj->ui8_curr_parser_offset;
            pt_mp4_hdlr_obj->ui4_moov_size = (UINT32)ui8_box_size;
#if (ENABLE_CMOV == 1)
            /* check whether there is cmov box exist*/
            i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_size);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }        
            i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_fourcc_type);
            if (i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            if(ui4_fourcc_type == MP4_BOX_CMOV)
            {
                pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size = ui4_size;
                i4_ret = mp4_minfo_decompress_cmov_box(pt_minfo_obj);
            }
#endif
            break;
        case MP4_BOX_MOOF:
            pt_mp4_hdlr_obj->b_fake_cmov = FALSE;
            if( pt_mp4_hdlr_obj->b_fake_cmov || !b_is_moof_found)
            {
                pt_mp4_hdlr_obj->ui8_data_offset
                                    = pt_mp4_hdlr_obj->ui8_curr_parser_offset;
                pt_mp4_hdlr_obj->ui8_data_sz = ui8_box_size;
                pt_mp4_hdlr_obj->t_piff_inet_info.b_is_piff_file    = TRUE;
                //pt_mp4_hdlr_obj->t_piff_inet_info.b_key_build       = TRUE;
                //pt_mp4_hdlr_obj->t_piff_inet_info.b_is_inet_data    = TRUE;//pt_mp4_hdlr_obj->b_fake_cmov;                
                pt_mp4_hdlr_obj->t_piff_inet_info.ui8_moof_start_pos= pt_mp4_hdlr_obj->ui8_curr_parser_offset; // to moof head                   
                pt_mp4_hdlr_obj->t_piff_inet_info.pui1_moof_buff    = _mm_util_mem_alloc(sizeof(UINT8)*15*1024);  
                x_memset(pt_mp4_hdlr_obj->t_piff_inet_info.pui1_moof_buff, 
                         0, 
                         sizeof(UINT8)*15*1024);
                pt_mp4_hdlr_obj->t_piff_inet_info.pui4_moof_pos = (UINT32*)
                                _mm_util_mem_alloc_with_tag(sizeof(UINT32)*20, 
                                                            MM_MEM_MIDX_TAG);  // only deal with 20 moof box;
                x_memset(pt_mp4_hdlr_obj->t_piff_inet_info.pui4_moof_pos, 
                         0, 
                         sizeof(UINT32)*20);
            }
            else
            {
                pt_mp4_hdlr_obj->t_piff_inet_info.b_is_piff_file = TRUE;
            }
            b_is_moof_found = TRUE;
            break;
        case MP4_BOX_MDAT:
            b_is_mdat_found = TRUE;
            pt_mp4_hdlr_obj->ui8_data_offset
                                = pt_mp4_hdlr_obj->ui8_curr_parser_offset;
            pt_mp4_hdlr_obj->ui8_data_sz = ui8_box_size;
            break;
        case MP4_BOX_MFRA:
            b_is_mfra_found = TRUE;
            pt_mp4_hdlr_obj->t_piff_inet_info.b_is_mfra_key_exist   = TRUE;
            pt_mp4_hdlr_obj->t_piff_inet_info.ui8_mfra_box_start_pos= pt_mp4_hdlr_obj->ui8_curr_parser_offset + 8;
            pt_mp4_hdlr_obj->t_piff_inet_info.ui4_mfra_box_size     = (UINT32)ui8_box_size;
            break;
        default:
            break;
        }
        if((b_is_moov_found && b_is_mdat_found && !b_is_mfra_found && !pt_mp4_hdlr_obj->t_piff_inet_info.b_is_bivl_file) || 
           (b_is_moov_found && b_is_mdat_found && b_is_mfra_found && b_is_moof_found && pt_mp4_hdlr_obj->t_piff_inet_info.b_is_bivl_file) ||/*sony marlin*/
           ui1_check_box_cnt == 0)
        {
            if(!b_is_mfra_found && b_is_moof_found && !pt_mp4_hdlr_obj->b_fake_cmov)
            {
                i4_ret = pf_set_pos(pt_minfo_obj, -12, FM_SEEK_END);
                i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_fourcc_type);
                if(ui4_fourcc_type == MP4_BOX_MFRO)
                { 
                    i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_size);/*skip 4 byte*/
                    i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_size);/*mfra box size*/
                    
                    i4_ret = pf_set_pos(pt_minfo_obj, -(INT64)(ui4_size - 8), FM_SEEK_END);/*to mfra box*/
                    pf_get_pos(pt_minfo_obj, &(pt_mp4_hdlr_obj->t_piff_inet_info.ui8_mfra_box_start_pos));
                    
                    pt_mp4_hdlr_obj->t_piff_inet_info.ui4_mfra_box_size     = ui4_size; 
                    pt_mp4_hdlr_obj->t_piff_inet_info.b_is_mfra_key_exist   = TRUE;
                }
            }
            break;
        }
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += ui8_box_size;
        i4_ret = pf_set_pos(pt_minfo_obj , 
                            (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, 
                            FM_SEEK_BGN);
    }

    pt_mp4_hdlr_obj->ui8_curr_parser_offset = pt_mp4_hdlr_obj->ui8_moov_offset;

    /*get size first to avoid read out of the file range*/
    i4_ret = pf_set_pos(pt_minfo_obj , 0, FM_SEEK_END|FM_SEEK_CHK);
    if (i4_ret != MINFOR_OK)
    {
        if(i4_ret == INPUTR_BUSY)
        {       
            pt_mp4_hdlr_obj->t_generic_info.b_non_seekable = TRUE;
            DBG_ERROR(("ERROR: seek to file end return busy. \r\n"));
        }        
        else
        {
            DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
            return i4_ret;
        }
    }
    pf_get_pos(pt_minfo_obj, &(pt_mp4_hdlr_obj->t_generic_info.ui8_media_sz));
#if 0
    if((pt_mp4_hdlr_obj->ui8_data_offset + pt_mp4_hdlr_obj->ui8_data_sz)
        > pt_mp4_hdlr_obj->t_generic_info.ui8_media_sz)
    {/*TBD:here size not exactly, only consider moov before mdat case*/
        pt_mp4_hdlr_obj->ui8_data_sz = pt_mp4_hdlr_obj->t_generic_info.ui8_media_sz
                                        - pt_mp4_hdlr_obj->ui8_data_offset;
    }
#else
    pt_mp4_hdlr_obj->ui8_data_sz = pt_mp4_hdlr_obj->t_generic_info.ui8_media_sz;
#endif

#if (ENABLE_CMOV == 1)
    do
    {
        if(pt_mp4_hdlr_obj->ui4_moov_size > 0x100000)
        {
            pt_mp4_hdlr_obj->b_fake_cmov = FALSE;
            break;
        }
        if(pt_mp4_hdlr_obj->b_fake_cmov && !pt_mp4_hdlr_obj->t_mp4_box_cmov.b_is_cmov_box_exist)
        {
            pt_mp4_hdlr_obj->t_mp4_box_cmov.ui8_cmov_box_pos
                                        = pt_mp4_hdlr_obj->ui8_moov_offset;      
            pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size
                                        = pt_mp4_hdlr_obj->ui4_moov_size;    
            pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_raw_cmov_data = NULL;

            pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_decomp_size
                                        = pt_mp4_hdlr_obj->ui4_moov_size;
            pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data
                                        = _mm_util_mem_alloc(sizeof(UINT8)*pt_mp4_hdlr_obj->ui4_moov_size);

            if(pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data == NULL)
            {
                break;
            }
            pt_mp4_hdlr_obj->t_mp4_box_cmov.b_is_cmov_box_exist = TRUE;
            
            i4_ret = pf_set_pos(pt_minfo_obj , pt_mp4_hdlr_obj->ui8_moov_offset, FM_SEEK_BGN);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR(("ERROR: seek to moov start fail. \r\n"));
                return i4_ret;
            }
            
            i4_ret = pf_copybyte(pt_minfo_obj,
                                 pt_mp4_hdlr_obj->t_mp4_box_cmov.pui1_decomp_cmov_data, 
                                 pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size, 
                                 pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size, 
                                 &ui4_read_size);
            if((i4_ret != INPUTR_OK)
                || (ui4_read_size != pt_mp4_hdlr_obj->t_mp4_box_cmov.ui4_cmov_box_size))
            {
                return MINFOR_INTERNAL_ERR;
            }
        }
    }while(0);

    if((pt_mp4_hdlr_obj->t_mp4_box_cmov.b_is_cmov_box_exist) || 
       pt_mp4_hdlr_obj->b_fake_cmov)
    {
        i4_ret = _mp4_minfo_cmov_track_info(pt_minfo_obj);
    }
    else
#endif
    {
        /* seek to moov container first sub box */
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        pf_set_pos(pt_minfo_obj , (INT64)pt_mp4_hdlr_obj->ui8_curr_parser_offset, FM_SEEK_BGN);
     
        pt_mp4_hdlr_obj->eParserState = MP4_PARSER_STATE_IDLE;

        i4_ret = pf_copybyte(pt_minfo_obj, 
                             pt_mp4_hdlr_obj->pui1_box_buf, 
                             pt_mp4_hdlr_obj->ui4_buf_length, 
                             8, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_INFO(("i4_ret = %d \r\n",  i4_ret));
            DBG_INFO(("ui4_read_size = %d \r\n", ui4_read_size));
            DBG_ERROR(("ERROR: mp4_minfo_box_parse fail.5 \r\n"));
            return MINFOR_INTERNAL_ERR;
        }
        pt_mp4_hdlr_obj->ui8_curr_parser_offset += 8;
        
        do
        {
            i4_ret = _mp4_minfo_moov_track_info(pt_minfo_obj);
        }while (i4_ret == MINFO_MP4_PARSE_NEED_NEXT_LOOP);
    }

    if(pt_mp4_hdlr_obj->t_piff_inet_info.b_is_bivl_file)
    {
        _mp4_minfo_marlin_mdat_info(pt_minfo_obj);
    }
    
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: _mp4_minfo_moov_track_info parse fail. i4_ret = %ld \r\n", i4_ret));
        return i4_ret;
    }

    if(ui8_total_file_duration != 0)
    {
        pt_mp4_hdlr_obj->t_generic_info.ui8_pb_duration = ui8_total_file_duration;
    }
    
    pt_mp4_hdlr_obj->t_generic_info.ui1_strm_ns = pt_mp4_hdlr_obj->ui1_uCurStrmNum;
    DBG_INFO(("INFO: mp4 file size = %lld \r\n", 
              pt_mp4_hdlr_obj->t_generic_info.ui8_media_sz));

    DBG_INFO(("INFO: mp4_minfo_box_parse OK. \r\n"));
    return MINFOR_OK;  
}

