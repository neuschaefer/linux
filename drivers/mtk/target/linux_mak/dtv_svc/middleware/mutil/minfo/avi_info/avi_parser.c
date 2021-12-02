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
 * $RCSfile: avi_parser.c,v $
 * $Revision: #3 $ 1.0
 * $Date: 2013/01/04 $ 2004/11/26
 * $Author: pan.huang $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the avi parser function
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
#include "avi_parser.h"
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
#include "avi_parser.h"
#include "u_dbg.h"
#include "../../mm_util.h"
#include "x_divx_drm.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 /*for dts support*/
#define MAX_DTS_PARSE_DATA                    (1024*4)
#define MINFO_AVI_PARSE_NEED_NEXT_LOOP        0x0000000F

#define MINFO_AVI_KNOWN_CHUNK_NUM             (MINFO_AVI_CHUNK_ID_MAX - 1)

#define MAX_SEC_TO_RECOVER_DUR                2
#define MAX_SIZE_TO_RECOVER_DUR               (30*1024*1024)  /*30M bytes*/

#define FOURCC(a,b,c,d)  ((((UINT32)d)<<24)|((c)<<16)|((b)<<8)|(a))

#define AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID( ui4_chunk_id )                          \
    (((((ui4_chunk_id) & 0xff) - 0x30) * 10) +                                           \
     ((((ui4_chunk_id) & 0xff00) >> 8) - 0x30))

#define VID_STRM_ID_DC        ((UINT32)0x63640000)  /* xxdc */
#define VID_STRM_ID_DB        ((UINT32)0x62640000)  /* xxdb */
#define VID_STRM_ID_IV        ((UINT32)0x76690000)  /* xxiv */

typedef struct _WAVEFORMATEX_T
{
  UINT16 ui2_format_tag;             /* format type */
  UINT16 ui2_n_channels;             /* number of channels (i.e. mono, stereo...) */
  UINT32 ui4_samples_per_sec;        /* sample rate */
  UINT32 ui4_n_avg_bytes_per_sec;    /* for buffer estimation */
  UINT16 ui2_n_block_align;          /* block size of data */
  UINT16 ui2_bits_per_sample;          /* Number of bits per sample of mono data */
  UINT16 ui2_extra_size;             /* the size of extra information append the structure*/
} WAVEFORMATEX_T;

#define AVI_WAVEFORMATEX_SIZE 18     /* WAVEFORMATEX_T : no struct align*/

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static MINFO_AVI_CHUNK_ID_T _a_chunk_id_array[MINFO_AVI_KNOWN_CHUNK_NUM]=
{
    { "RIFF", MINFO_AVI_CHUNK_ID_RIFF },
    { "LIST", MINFO_AVI_CHUNK_ID_LIST },
    { "AVI ", MINFO_AVI_CHUNK_ID_AVI  },
    { "hdrl", MINFO_AVI_CHUNK_ID_HDRL },
    { "avih", MINFO_AVI_CHUNK_ID_AVIH },
    { "strl", MINFO_AVI_CHUNK_ID_STRL },
    { "strh", MINFO_AVI_CHUNK_ID_STRH },
    { "strf", MINFO_AVI_CHUNK_ID_STRF },
    { "strd", MINFO_AVI_CHUNK_ID_STRD },
    { "auds", MINFO_AVI_CHUNK_ID_AUDS },
    { "mids", MINFO_AVI_CHUNK_ID_MIDS },
    { "vids", MINFO_AVI_CHUNK_ID_VIDS },
    { "txts", MINFO_AVI_CHUNK_ID_TXTS },
    { "JUNK", MINFO_AVI_CHUNK_ID_JUNK },
    { "RES1", MINFO_AVI_CHUNK_ID_RES1 },
    { "PRMI", MINFO_AVI_CHUNK_ID_PRMI },
    { "MTDA", MINFO_AVI_CHUNK_ID_MTDA },
    { "movi", MINFO_AVI_CHUNK_ID_MOV  },
    { "idx1", MINFO_AVI_CHUNK_ID_IDX1 },
    { "indx", MINFO_AVI_CHUNK_ID_INDX },
    { "strn", MINFO_AVI_CHUNK_ID_STRN },
    { "INFO", MINFO_AVI_CHUNK_ID_INFO },
    { "IART", MINFO_AVI_CHUNK_ID_IART },
    { "ICOP", MINFO_AVI_CHUNK_ID_ICOP },
    { "IGNR", MINFO_AVI_CHUNK_ID_IGNR },
    { "INAM", MINFO_AVI_CHUNK_ID_INAM },/*,
    { "odml", MINFO_AVI_CHUNK_ID_ODML },
    { "dmlh", MINFO_AVI_CHUNK_ID_DMLH },        
    { "IDIT", MINFO_AVI_CHUNK_ID_IDIT }*/
    { "fmt ", MINFO_AVI_CHUNK_ID_FMT  },      /*add for wave support*/
    { "data", MINFO_AVI_CHUNK_ID_DATA },    /*add for wave support*/
    { "WAVE", MINFO_AVI_CHUNK_ID_WAVE }, /*add for wave support*/
    { "fact", MINFO_AVI_CHUNK_ID_FACT }      /*add for wave support*/
#ifdef MM_AVI_DIVX6
    ,
    { "MENU", MINFO_AVI_CHUNK_ID_MENU },
    { "MRIF", MINFO_AVI_CHUNK_ID_MRIF },
    { "1idx", MINFO_AVI_CHUNK_ID_1IDX }
#endif
};



/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/

#if 0
static UINT32 _ui4_word24cc(UINT16 ui2_wrd)
{
  int i1_i;
  UINT32 ui4_4cc;

  ui4_4cc = 0;
  for(i1_i = 0; i1_i < 4; i1_i++)
  {
    ui4_4cc |= (((ui2_wrd >> (i1_i * 4)) & 0xf) | 0x30) << ((3 - i1_i) * 8);
  }

  return(ui4_4cc);
}

static MINFO_INFO_AUDIO_ENC_T _e_aud_codec(UINT32 ui4_4cc)
{
  MINFO_INFO_AUDIO_ENC_T e_ast_mode = MINFO_INFO_AUD_ENC_UNKNOWN;

  switch(ui4_4cc)
  {
    case FOURCC('0', '0', '5', '0'):
      e_ast_mode = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MPEG;*/
      break;
    case FOURCC('0', '0', '5', '5'):
      e_ast_mode = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MP3;*/
      break;
    case FOURCC('2', '0', '0', '0'):
      e_ast_mode = MINFO_INFO_AUD_ENC_AC3;
      break;
    case FOURCC('2', '0', '0', '1'):
      e_ast_mode = MINFO_INFO_AUD_ENC_DTS;
      break;
    case FOURCC('0', '0', '0', '1'):
      e_ast_mode = MINFO_INFO_AUD_ENC_PCM;
      break;
    case FOURCC('0', '0', '0', '2'):
      e_ast_mode = MINFO_INFO_AUD_ENC_ADPCM;
      break;
    case FOURCC('0', '1', '6', '1'):
      e_ast_mode = MINFO_INFO_AUD_ENC_WMA_V1;
      break;
    case FOURCC('0', '0', 'F', 'F'):
      e_ast_mode = MINFO_INFO_AUD_ENC_AAC;
      break;
    default:
      e_ast_mode = MINFO_INFO_AUD_ENC_UNKNOWN;
      break;
  }
  return(e_ast_mode);
}
#endif

static MINFO_INFO_AUDIO_ENC_T _e_avi_aud_codec(UINT16 ui2_format_tag)
{
  MINFO_INFO_AUDIO_ENC_T e_ast_mode = MINFO_INFO_AUD_ENC_UNKNOWN;

  switch(ui2_format_tag)
  {
    case 0x0050:
      e_ast_mode = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MPEG;*/
      break;
    case 0x0055:
      e_ast_mode = MINFO_INFO_AUD_ENC_MPEG_1;/*MPC_AUD_DEC_TYPE_MP3;*/
      break;
    case 0x2000:
      e_ast_mode = MINFO_INFO_AUD_ENC_AC3;
      break;
    case 0x2001:
      e_ast_mode = MINFO_INFO_AUD_ENC_DTS;
      break;
    case 0x0001:
      e_ast_mode = MINFO_INFO_AUD_ENC_PCM;
      break;
    case 0x0002:
      e_ast_mode = MINFO_INFO_AUD_ENC_ADPCM;
      break;
    case 0x0006:
      e_ast_mode = MINFO_INFO_AUD_ENC_A_PCM;
      break;
    case 0x0007:
      e_ast_mode = MINFO_INFO_AUD_ENC_U_PCM;
      break;  
    case 0x0011:
      e_ast_mode = MINFO_INFO_AUD_ENC_DVI_ADPCM;
      break;
    case 0x0161:
      e_ast_mode = MINFO_INFO_AUD_ENC_WMA_V1;
      break;
    case 0x00FF:
      e_ast_mode = MINFO_INFO_AUD_ENC_AAC;
      break;
    default:
      e_ast_mode = MINFO_INFO_AUD_ENC_UNKNOWN;
      break;
  }
  return(e_ast_mode);
}

static BOOL _b_is_subtitle(UINT32 ui4_4cc)
{
  switch(ui4_4cc)
  {
    case FOURCC('D', 'X', 'S', 'B'):  /* subtitle */
    case FOURCC('d', 'x', 's', 'b'):
      return TRUE;
    default:
      break;
  }
  return FALSE;
}

static BOOL _b_is_subtitle_plus(UINT32 ui4_4cc)
{
  switch(ui4_4cc)
  {
    case FOURCC('D', 'X', 'S', 'A'):  /* subtitle plus */
    case FOURCC('d', 'x', 's', 'a'):
      return TRUE;
    default:
      break;
  }
  return FALSE;
}

static MINFO_INFO_VIDEO_ENC_T _e_avi_vid_codec(UINT32 ui4_4cc)
{
  MINFO_INFO_VIDEO_ENC_T e_v_codec = MINFO_INFO_VID_ENC_UNKNOWN;

  switch(ui4_4cc)
  {
    case FOURCC('X', 'V', 'I', 'D'):   /*xvid*/    
    case FOURCC('x', 'v', 'i', 'd'):
		e_v_codec = MINFO_INFO_VID_ENC_XVID;
      break;
    case FOURCC('3', 'I', 'V', '1'):   /* 3ivx Delta 1.0~3.5*/
    case FOURCC('3', 'i', 'v', '1'):
    case FOURCC('3', 'I', 'V', '2'):   /* 3ivx Delta 4.0*/
    case FOURCC('3', 'i', 'v', '2'):
    case FOURCC('F', 'V', 'F', 'W'):  /*ffmpeg VFW*/
    case FOURCC('f', 'v', 'f', 'w'):
    case FOURCC('D', 'M', 'K', '2'):  /*a file from Gapha_Tong has this 4cc*/
    case FOURCC('F', 'M', 'P', '4'):  /*some avi has "FMP4" codec 4cc*/
      e_v_codec = MINFO_INFO_VID_ENC_MPEG_4;
      break;
    case FOURCC('E', 'M', '2', 'V'):  /*tempt.*/
    case FOURCC('M', 'P', 'G', '2'):
    case FOURCC('m', 'p', 'g', '2'):
    case FOURCC('M', 'P', 'E', 'G'):
    case FOURCC('M', 'P', 'G', 'V'):           
    case FOURCC('m', 'p', 'g', 'v'):                     
      e_v_codec = MINFO_INFO_VID_ENC_MPEG_2;
      break;
    case FOURCC('H', '2', '6', '3'):
    case FOURCC('h', '2', '6', '3'):
      e_v_codec = MINFO_INFO_VID_ENC_H263;
      break;
    case FOURCC('H', '2', '6', '4'):
    case FOURCC('h', '2', '6', '4'):
	case FOURCC('a', 'v', 'c', '1'):
	case FOURCC('A', 'V', 'C', '1'):		
      e_v_codec = MINFO_INFO_VID_ENC_H264;
      break;
    case FOURCC('R', 'M', 'P', '4'):  /*RMP4*/
    case FOURCC('r', 'm', 'p', '4'):
      e_v_codec = MINFO_INFO_VID_ENC_UNKNOWN;
      break;
    case FOURCC('D', 'I', 'V', '3'):  /*DivX 3.11*/
    case FOURCC('d', 'i', 'v', '3'):
    case FOURCC('D', 'I', 'V', '4'):
    case FOURCC('d', 'i', 'v', '4'):
    case FOURCC('D', 'I', 'V', '5'):
    case FOURCC('d', 'i', 'v', '5'):
    case FOURCC('D', 'I', 'V', '6'):
    case FOURCC('d', 'i', 'v', '6'):
    case FOURCC('M', 'P', '4', '3'):  /*Microsoft MPEG-4 v3*/
    case FOURCC('m', 'p', '4', '3'):
    /* case FOURCC('M', 'P', '4', 'S'): */
    case FOURCC('C', 'O', 'L', '1'):  /* cool codec*/
    case FOURCC('c', 'o', 'l', '1'):
    case FOURCC('A', 'P', '4', '1'):  /*AngelPotion*/
    case FOURCC('a', 'p', '4', '1'):
    case FOURCC('n', 'A', 'V', 'I'):  /*nAVI*/
      e_v_codec = MINFO_INFO_VID_ENC_DIVX_311;
      break;
    case FOURCC('D', 'I', 'V', 'X'):  /*DivX 4.x*/
    case FOURCC('d', 'i', 'v', 'x'):
      e_v_codec = MINFO_INFO_VID_ENC_DIVX_4;
      break;
    case FOURCC('D', 'X', '5', '0'):  /* DivX 5.x*/
    case FOURCC('d', 'x', '5', '0'):
      e_v_codec = MINFO_INFO_VID_ENC_DIVX_5;
      break;
    case FOURCC('m', 'j', 'p', 'g'):    
    case FOURCC('M', 'J', 'P', 'G'):
      e_v_codec = MINFO_INFO_VID_ENC_MJPG;
      break;
    case FOURCC('w', 'm', 'v', '3'):    
    case FOURCC('W', 'M', 'V', '3'):
      e_v_codec = MINFO_INFO_VID_ENC_WMV3;
      break;
    case FOURCC('m', 'p', 'g', '1'):
    case FOURCC('M', 'P', 'G', '1'):
      e_v_codec = MINFO_INFO_VID_ENC_MPEG_1;
      break;  
    default:
      break;
  }
  return(e_v_codec);
}


/*-----------------------------------------------------------------------------
 * Name:  _ui4_swap_4bytes
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/         
static UINT32 _ui4_swap_4bytes(UINT32 ui4_in)
{
    /*UINT32 u4Ret;

    u4Ret = (u4In >> 24);
    u4Ret |= ((u4In >> 16) & 0x0ff) << 8;
    u4Ret |= ((u4In >>  8) & 0x0ff) << 16;
    u4Ret |= (u4In << 24);*/
    return ui4_in;
}

static BOOL _b_is_chunk_need_parser(MINFO_AVI_CHUNK_ID_ENUM_T e_id)
{
    return (e_id == MINFO_AVI_CHUNK_ID_VIDS 
                || e_id == MINFO_AVI_CHUNK_ID_AUDS
                || e_id == MINFO_AVI_CHUNK_ID_STRF
                || e_id == MINFO_AVI_CHUNK_ID_STRN
                || e_id == MINFO_AVI_CHUNK_ID_STRD
                || e_id == MINFO_AVI_CHUNK_ID_AVIH
                || e_id == MINFO_AVI_CHUNK_ID_IART
                || e_id == MINFO_AVI_CHUNK_ID_ICOP
                || e_id == MINFO_AVI_CHUNK_ID_IGNR
                || e_id == MINFO_AVI_CHUNK_ID_INAM
#ifdef MM_AVI_DIVX6
                || e_id == MINFO_AVI_CHUNK_ID_1IDX
#endif
                || e_id == MINFO_AVI_CHUNK_ID_INDX
                || e_id == MINFO_AVI_CHUNK_ID_FMT); /*add for wave support*/
}


static BOOL _b_is_chunk_need_check_sub(MINFO_AVI_CHUNK_ID_ENUM_T e_id)
{
    return (e_id== MINFO_AVI_CHUNK_ID_AVI 
                || e_id == MINFO_AVI_CHUNK_ID_STRL
                || e_id == MINFO_AVI_CHUNK_ID_HDRL
                || e_id == MINFO_AVI_CHUNK_ID_INFO
                || e_id == MINFO_AVI_CHUNK_ID_WAVE);  /*add for wave support*/
}


static BOOL _b_is_offset_in_this_chunk(UINT32             ui4_offset, 
                                       MINFO_AVI_CHUNK_T* pt_chunk)
{
    return ((ui4_offset >= pt_chunk->ui4_offset) && 
            (ui4_offset <= pt_chunk->ui4_offset+ pt_chunk->ui4_size));
}

static INT32 _i4_get_par_chunk_by_offset(MINFO_AVI_CHUNK_T*  pt_head_chunk, 
                                         UINT32              ui4_offset, 
                                         MINFO_AVI_CHUNK_T** ppt_par_chunk)
{
    MINFO_AVI_CHUNK_T *pt_cur_chunk = pt_head_chunk;

    *ppt_par_chunk = NULL;

    while (pt_cur_chunk != NULL)
    {
        if (_b_is_offset_in_this_chunk(ui4_offset, pt_cur_chunk))
        {
            if (pt_cur_chunk->pt_first_sub_chunk != NULL)
            {
                pt_cur_chunk = pt_cur_chunk->pt_first_sub_chunk;
            }
            else
            {
                *ppt_par_chunk = pt_cur_chunk;
                break;
            }
        }
        else if (pt_cur_chunk->pt_next_chunk != NULL)
        {
            pt_cur_chunk = pt_cur_chunk->pt_next_chunk;
        }
        else
        {
            *ppt_par_chunk = pt_cur_chunk ->pt_par_chunk;
            break;
        }
    }
    return MINFOR_OK;
}


static INT32 _i4_get_last_sub_chunk(MINFO_AVI_CHUNK_T*  pt_par_chunk, 
                                    MINFO_AVI_CHUNK_T** ppt_last_sub_chunk)
{
    MINFO_AVI_CHUNK_T *pt_cur_chunk;
    pt_cur_chunk = pt_par_chunk->pt_first_sub_chunk;
    *ppt_last_sub_chunk = NULL;
    while(pt_cur_chunk != NULL)
    {
        *ppt_last_sub_chunk = pt_cur_chunk;
        pt_cur_chunk=pt_cur_chunk->pt_next_chunk;
    }
    return MINFOR_OK;
}    

static INT32 _i4_add_chunk_to_tree(MINFO_OBJ_T*              pt_minfo_obj, 
                                   MINFO_AVI_CHUNK_ID_ENUM_T e_id, 
                                   UINT32                    ui4_offset,
                                   UINT32                    ui4_size)
{ 
    MINFO_AVI_CHUNK_T *pt_par_chunk       = NULL;
    MINFO_AVI_CHUNK_T *pt_last_sub        = NULL;
    MINFO_AVI_RIFF_INF_T* pt_avi_hdlr_obj = NULL;
    MINFO_AVI_CHUNK_T *pt_chunk           = NULL;

    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   
    pt_chunk = (MINFO_AVI_CHUNK_T *)(_mm_util_mem_alloc(sizeof(MINFO_AVI_CHUNK_T)));
    if (pt_chunk == NULL)
    {
        /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: Failed to malloc obj\n "));*/
        return MINFOR_OK;
    }
    _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                          pt_avi_hdlr_obj->ui4_cur_parser_offset, 
                          &pt_par_chunk);
    
    pt_avi_hdlr_obj->ui4_malloc_count += 1;

    if (pt_par_chunk != NULL)
    {
        _i4_get_last_sub_chunk(pt_par_chunk, &pt_last_sub);
    } 
    else if (pt_avi_hdlr_obj->pt_head_chunk == NULL)
    {
        pt_avi_hdlr_obj->pt_head_chunk = pt_chunk;
    }
    if (pt_last_sub == NULL)
    {
        if (pt_par_chunk != NULL)
        {
            pt_par_chunk->pt_first_sub_chunk = pt_chunk;
        }    
    }
    else
    {
        pt_last_sub->pt_next_chunk = pt_chunk;
    }  
    pt_chunk ->ui4_offset = ui4_offset;
    pt_chunk->e_chunk_id = e_id;
    pt_chunk->ui4_size = ui4_size;
    pt_chunk->pt_par_chunk = pt_par_chunk;
    pt_chunk->pt_first_sub_chunk = NULL;
    pt_chunk->pt_next_chunk = NULL;
    /*mrCheckAndSaveChunkInf(pt_chunk);*/
    
    return MINFOR_OK;
}



INT32 _i4_delete_chunk(MINFO_OBJ_T*         pt_minfo_obj,
                       MINFO_AVI_CHUNK_T*   pt_cur_chunk)
{
    MINFO_AVI_CHUNK_T *pt_par_chunk;
    MINFO_AVI_RIFF_INF_T* pt_avi_hdlr_obj;
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   

    while(pt_cur_chunk != NULL)
    {
        if (pt_cur_chunk->pt_first_sub_chunk != NULL)
        {
            pt_cur_chunk = pt_cur_chunk->pt_first_sub_chunk;
        }
#if 0
        else if (pt_cur_chunk ->pt_next_chunk != NULL)
        {
            PB_AVI_CHUNK_T *pPreChunk;
            pPreChunk = pt_cur_chunk;
            pt_cur_chunk = pt_cur_chunk->pt_next_chunk;
            pPreChunk->pt_next_chunk = NULL;
        }
 #endif       
        else 
        {
            pt_par_chunk = pt_cur_chunk->pt_par_chunk;
            if (pt_par_chunk == NULL)
            {               
                _mm_util_mem_free(pt_cur_chunk);
                pt_avi_hdlr_obj->ui4_malloc_count -= 1;
                /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: Free pt_chunk address is 0x%lx, the number is 0x%lx\n ",pt_cur_chunk,m_u4MallocCount));*/
                break;
            }
            if ((pt_par_chunk != NULL) && 
                (pt_par_chunk->pt_first_sub_chunk == pt_cur_chunk))
            {
                pt_par_chunk->pt_first_sub_chunk = pt_cur_chunk ->pt_next_chunk;
            }   
            if (pt_cur_chunk->pt_first_sub_chunk == NULL)
            {                
                _mm_util_mem_free(pt_cur_chunk);
                pt_avi_hdlr_obj->ui4_malloc_count -= 1;
                /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: Free pt_chunk address is 0x%lx, the number is 0x%lx\n ",pt_cur_chunk,m_u4MallocCount));*/
            }
            pt_cur_chunk = pt_par_chunk;
        }
     }
    return MINFOR_OK;
}

static INT32 _i4_delete_riff_tree(MINFO_OBJ_T*  pt_minfo_obj)
{
   MINFO_AVI_RIFF_INF_T* pt_avi_hdlr_obj;
   pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   
    _i4_delete_chunk(pt_minfo_obj,pt_avi_hdlr_obj->pt_head_chunk);
    pt_avi_hdlr_obj->pt_head_chunk = NULL;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _e_get_chunk_id_and_size
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/         
static MINFO_AVI_CHUNK_ID_ENUM_T _e_get_chunk_id_and_size(void*   pv_data, 
                                                          UINT32* pui4_size)
{
    UINT16 ui2_idx;
   
    CHAR ac_fourcc[5] = {0};

    if((pv_data == NULL) || (pui4_size == NULL))
    {
        return MINFO_AVI_CHUNK_ID_NONE;
    }
    
    x_memcpy(ac_fourcc, pv_data, 4);
   /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser:Current fourCC is %s\n", szFourCC));*/ 
    for (ui2_idx = 0; ui2_idx < (UINT16)MINFO_AVI_KNOWN_CHUNK_NUM; ui2_idx++)
    {
        if (0 == x_memcmp(pv_data, _a_chunk_id_array[ui2_idx].s_four_cc, 4))
        {
            UINT32 ui4_sz;
            LOADL_DWRD((((UINT8 *)pv_data) + 4), ui4_sz);
            *pui4_size = ui4_sz;
            return _a_chunk_id_array[ui2_idx].e_chunk_id;
        }
    }
    return MINFO_AVI_CHUNK_ID_NONE;
}

static MINFO_AVI_CHUNK_ID_ENUM_T _e_get_chunk_id(void* pv_data)
{
    UINT16 ui2_idx;

    CHAR ac_fourcc[5] = {0};
    
    if(pv_data == NULL)
    {
        return MINFO_AVI_CHUNK_ID_NONE;
    }
    
    x_memcpy(ac_fourcc, pv_data, 4);

    for (ui2_idx = 0; ui2_idx < (UINT16)MINFO_AVI_KNOWN_CHUNK_NUM; ui2_idx++)
    {
        if (0 == x_memcmp(pv_data, _a_chunk_id_array[ui2_idx].s_four_cc, 4))
        {
            return _a_chunk_id_array[ui2_idx].e_chunk_id;
        }
    }
    return MINFO_AVI_CHUNK_ID_NONE;
}

static UINT32 _ui4_get_chunk_size(void* pv_data)
{
    UINT32 ui4_sz = 0;
    
    if(pv_data == NULL)
    {
        return 0;
    }
    
    LOADL_DWRD((((UINT8 *)pv_data) + 4), ui4_sz);
    
    return ui4_sz;
}

/*-----------------------------------------------------------------------------
 * Name:  _avi_check_header_buf
 *
 * Description: check avi handler object header buffer enough or not
 *
 * Inputs:  pt_avi_obj      specify avi handler object
 *          ui4_curr_len    specify current need buffer length
 *          
 * Outputs: 
 *
 * Returns: MINFOR_OK                The API is successful. 
 *          MINFOR_OUT_OF_MEMORY     The API alloc memory fail.
 ----------------------------------------------------------------------------*/
static INT32 _avi_check_header_buf(MINFO_AVI_RIFF_INF_T*  pt_avi_obj, 
                                   UINT32                 ui4_curr_len)
{
    if (ui4_curr_len > pt_avi_obj->ui4_buf_length)
    {
        _mm_util_mem_free(pt_avi_obj->pui1_header_buf);
        pt_avi_obj->pui1_header_buf = NULL;
        pt_avi_obj->pui1_header_buf = 
            (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui4_curr_len);
        if (pt_avi_obj->pui1_header_buf  == NULL)
        {
            DBG_ERROR(("ERROR: _avi_check_header_buf alloc memory fail \r\n"));
            return MINFOR_OUT_OF_MEMORY;
        }
        x_memset(pt_avi_obj->pui1_header_buf , 0, sizeof(UINT8) * ui4_curr_len);
        pt_avi_obj->ui4_buf_length = ui4_curr_len;
    }
    return MINFOR_OK;
}

INT32 _i4_check_and_save_chunk_inf(MINFO_OBJ_T*       pt_minfo_obj, 
                                   MINFO_AVI_CHUNK_T* pt_chunk)
{

    MINFO_AVI_RIFF_INF_T*  pt_avi_hdlr_obj = NULL;
    UINT32 ui4_compression;
    UINT8* pui1_head_buf;
    UINT8  ui1_aud_stm_nums;
    UINT8  ui1_vid_stm_nums;
    UINT8  ui1_sp_stm_nums;
    UINT32 ui4_bytes_per_sec;
    UINT16 ui2_lang;
    WAVEFORMATEX_T* pt_wfx;
    MINFO_AVI_VIDEO_STM_ATRBT_T*            pt_vid_info;
    MINFO_AVI_AUDIO_STM_ATRBT_T*            pt_aud_info;
    MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T*    pt_sp_info;
    UINT8* pui1_strd_chunk;
    UINT32 ui4_strd_size;
    UINT32 ui4_info_size;

    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   
    pui1_head_buf = pt_avi_hdlr_obj->pui1_header_buf;
    pt_wfx = (WAVEFORMATEX_T *)pui1_head_buf;
    
    switch (pt_chunk->e_chunk_id)
    {
       case MINFO_AVI_CHUNK_ID_AVIH:
         LOADL_DWRD(pui1_head_buf, pt_avi_hdlr_obj->ui4_frm_period); /*micro second per frame*/
         break;
       case MINFO_AVI_CHUNK_ID_STRD:
         ui1_vid_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num;
         while (ui1_vid_stm_nums > 0)
         {
             ui1_vid_stm_nums --;
             if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
             {
                 break;
             }
             pt_vid_info = 
                    &(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums]);

             if(pt_vid_info->e_enc == MINFO_INFO_VID_ENC_DIVX_311 || 
                pt_vid_info->e_enc == MINFO_INFO_VID_ENC_DIVX_4   ||
                pt_vid_info->e_enc == MINFO_INFO_VID_ENC_DIVX_5
               )
             {
                 pt_minfo_obj->t_minfo_drm_info.t_minfo_drm_type = DRM_TYPE_DIVX_DRM;
                 ui4_strd_size = pt_chunk->ui4_size-8-8 /*remove strd(4bytes)--size(4bytes)--drm version(4bytes)--drm size(4bytes)*/;        
                 if(pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk == NULL)
                 {
                 pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk = 
                    (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui4_strd_size);
                 pui1_strd_chunk = pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk;
                 if (pui1_strd_chunk == NULL)
                 {
                      return MINFOR_OUT_OF_MEMORY;
                 }
                 _mm_util_mem_copy(pui1_strd_chunk, pui1_head_buf+8/*remove drm version(4bytes)--drm size(4bytes)*/, ui4_strd_size);
                 pt_minfo_obj->ui4_drm_size = ui4_strd_size;
             }
         }
         }
         break;
       case MINFO_AVI_CHUNK_ID_VIDS:  
         DBG_INFO(("[avi_parser]MINFO_AVI_CHUNK_ID_VIDS case  \r\n"));
         pt_avi_hdlr_obj->ui1_cur_strm_num++;
         ui1_vid_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num;
         if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
         {
             break;
         }
         LOADL_DWRD(pui1_head_buf + 16, 
            pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_scale);
         LOADL_DWRD(pui1_head_buf + 20, 
            pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_rate);
         LOADL_DWRD(pui1_head_buf + 28,
            pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_length);   
         LOADL_DWRD(pui1_head_buf + 20 + 20,
            pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_sample_size);
         /*caculate playback duration*/
         pt_vid_info =&pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums];
         if(pt_vid_info->ui4_rate > 0)
         {            
            pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui8_duration =        
                _mm_div64((UINT64)pt_vid_info->ui4_length * pt_vid_info->ui4_scale * 90000, 
                          pt_vid_info->ui4_rate,
                          NULL);
         }
         else
         {
              pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui8_duration = 0;
         }
         break;
       case MINFO_AVI_CHUNK_ID_AUDS:
         pt_avi_hdlr_obj->ui1_cur_strm_num++;
         ui1_aud_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num;
         if (ui1_aud_stm_nums >= MAX_NS_AVI_AUD)
         {
             break;
         }
         LOADL_DWRD(pui1_head_buf + 16, 
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_scale);
         LOADL_DWRD(pui1_head_buf + 20, 
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_rate);
         LOADL_DWRD(pui1_head_buf + 28, 
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_length);
         LOADL_DWRD(pui1_head_buf + 40, 
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_sample_size);   
         break;
       case MINFO_AVI_CHUNK_ID_STRF:   
         DBG_INFO(("[avi_parser]MINFO_AVI_CHUNK_ID_STRF case  \r\n"));
         ui1_vid_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num;
         if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_VIDS)
         {
           LOADL_DWRD(pui1_head_buf + 16, ui4_compression);
           if ((_b_is_subtitle(ui4_compression)) || 
               (_b_is_subtitle_plus(ui4_compression)))
           {
             /* subtitle case */
             pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type = 
                MINFO_AVI_STRM_TYPE_SP;
             ui1_sp_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_num;
             if (ui1_sp_stm_nums >= MAX_NS_AVI_SP)
             {
                 break;
             }
             pt_sp_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_sp_strm_inf[ui1_sp_stm_nums]);
             if(_b_is_subtitle(ui4_compression))
             {
                pt_sp_info->ui4_subtitle_type = 
                    MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP;
             }
             else
             {
                pt_sp_info->ui4_subtitle_type = 
                    MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA;
             }
             if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
             {
                 break;
             }
             /* scale info */
             pt_sp_info->ui4_scale = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_scale;
             /* rate info */
             pt_sp_info->ui4_rate = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_rate;
             /* sample size */
             pt_sp_info->ui4_sample_size = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_sample_size;
             /* width and height */
             LOADL_DWRD(pui1_head_buf + 4, pt_sp_info->i4_width);
             LOADL_DWRD(pui1_head_buf + 8, pt_sp_info->i4_height); 

             pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_num++;            
             pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_id_list[pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_num-1] 
                                      = (UINT8)(pt_avi_hdlr_obj->ui1_cur_strm_num-1); 
             break; 
           }
           else
           {
             /* video case */
             pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type = 
                MINFO_AVI_STRM_TYPE_VID;
             if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
             {
                 break;
             }
             pt_vid_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums]);
             pt_vid_info->ui4_scale = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_scale;
             pt_vid_info->ui4_rate = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_rate;
             pt_vid_info->ui4_length = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_length;
             pt_vid_info->ui4_sample_size = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui4_sample_size;
             pt_vid_info->ui8_duration = 
               pt_avi_hdlr_obj->t_strm_inf.t_vid_inf_strh[ui1_vid_stm_nums].ui8_duration;
             if(pt_vid_info->ui8_duration >
                pt_avi_hdlr_obj->ui8_duration)
             {
                 pt_avi_hdlr_obj->ui8_duration = pt_vid_info->ui8_duration;
             }
             LOADL_DWRD(pui1_head_buf + 4, pt_vid_info->i4_width);
             LOADL_DWRD(pui1_head_buf + 8, pt_vid_info->i4_height);
             /*LOADL_DWRD(pui1_head_buf + 16, ui4_compression);*/
             pt_vid_info->e_enc = _e_avi_vid_codec(ui4_compression);
             pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num++;            
             pt_avi_hdlr_obj->t_strm_inf.ui1_vid_strm_id_list[pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num-1] 
                                      = (UINT8)(pt_avi_hdlr_obj->ui1_cur_strm_num-1); 
             if(pt_chunk->ui4_size >= 44) /*for wmv3 codec*/
             {
                LOADL_DWRD(pui1_head_buf + 40, pt_vid_info->ui4_ext_data);
             }
             if(pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num > MAX_NS_AVI_VID)
             {
                DBG_ERROR(("ERROR: video stream number overflow!!! \r\n"));
             }
           }   
         }   
         else if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_AUDS)
         {
            /* audio stream header info case */
            pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type = 
                MINFO_AVI_STRM_TYPE_AUD;
            ui1_aud_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num;
            if (ui1_aud_stm_nums >= MAX_NS_AVI_AUD)
            {
                break;
            }
            pt_aud_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums]);
            pt_aud_info->e_enc = 
                _e_avi_aud_codec( pt_wfx->ui2_format_tag);
            /*MPEG-LAYER3*/ 
            if(pt_wfx->ui2_format_tag == 0x55)
            {
                 pt_aud_info->e_layer = 3;
            }           
   
            if (pt_aud_info->e_enc == MINFO_INFO_AUD_ENC_WMA_V1)
            {
               LOADL_WORD(pui1_head_buf + AVI_WAVEFORMATEX_SIZE + 4, 
                          pt_aud_info->ui2_encode_opts);
                     
            }
            pt_aud_info->i4_samples_per_sec = (INT32)pt_wfx->ui4_samples_per_sec;
            pt_aud_info->i2_bits_per_sample = (INT16)pt_wfx->ui2_bits_per_sample;
            pt_aud_info->i2_block_align = (INT16)pt_wfx->ui2_n_block_align;
            pt_aud_info->i2_channels = (INT16)pt_wfx->ui2_n_channels;
            pt_aud_info->ui4_avg_bytes_sec = pt_wfx->ui4_n_avg_bytes_per_sec;

            /* profile */
            if(pt_aud_info->e_enc == MINFO_INFO_AUD_ENC_AAC && pt_wfx->ui2_extra_size != 0)
            {
                UINT8 ui1_extra;
                LOAD_BYTE(pui1_head_buf + AVI_WAVEFORMATEX_SIZE, ui1_extra);
                pt_aud_info->ui4_aac_profile = (ui1_extra>>3) & 0x1F;
            }

            pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num++;
            pt_avi_hdlr_obj->t_strm_inf.ui1_aud_strm_id_list[pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num-1] 
                = (UINT8)(pt_avi_hdlr_obj->ui1_cur_strm_num-1); 
            
            /* for sample size value error case */
            if(pt_wfx->ui2_format_tag == 0x55)
            {
                if(pt_aud_info->ui4_sample_size == 0)
                {
                    /* LG "240.avi" with 24000/576 == 24000/576 it should be VBRA */
                    /* We use wBlockAlign as a divisor instead of 1152 */
                    if (pt_aud_info->i2_block_align == 0)
                    {
                        /* Some files encoded by Dr.DivX has 0 dwSampleSize value in strh, */
                        /* but the audio is actually CBR MP3. */
                        if((pt_aud_info->ui4_rate / pt_aud_info->ui4_scale) !=
                           (UINT32)(pt_aud_info->i4_samples_per_sec / 1152))
                        {
                            pt_aud_info->ui4_sample_size = (UINT32)_mm_div64(
                                (UINT64)(pt_aud_info->ui4_avg_bytes_sec * pt_aud_info->ui4_scale),
                                (UINT64)pt_aud_info->ui4_rate,
                                NULL);
                        }
                    }
                    else
                    {   /* c12_12fps_drdivx102p.AVI case */
                        if((pt_aud_info->ui4_rate / pt_aud_info->ui4_scale) !=
                           (UINT32)(pt_aud_info->i4_samples_per_sec / pt_aud_info->i2_block_align))
                        {
                            pt_aud_info->ui4_sample_size = (UINT32)_mm_div64(
                                (UINT64)(pt_aud_info->ui4_avg_bytes_sec * pt_aud_info->ui4_scale),
                                (UINT64)pt_aud_info->ui4_rate,
                                NULL);
                        }
                    }
                }
                else if (pt_aud_info->ui4_sample_size != 1)
                {
                    /* C24_HD_1080p11_mp3_surround_253vbr_48_01.divx */
                    if ((pt_aud_info->ui4_scale != 0) && 
                        (pt_aud_info->i2_block_align != 0))
                    {
                        if ((pt_aud_info->ui4_rate / pt_aud_info->ui4_scale) ==
                           (UINT32)(pt_aud_info->i4_samples_per_sec / pt_aud_info->i2_block_align))
                        {
                            pt_aud_info->ui4_sample_size = 0;
                        }
                    }
                }
            }
            
            if(pt_aud_info->ui4_sample_size == 0)
            {
                /*VBR*/  
                if(pt_aud_info->ui4_rate > 0)
                {                
                    pt_aud_info->ui8_duration =        
                        _mm_div64((UINT64)pt_aud_info->ui4_length * pt_aud_info->ui4_scale * 90000, 
                                  pt_aud_info->ui4_rate,
                                  NULL);        
                }
                else
                {
                    pt_aud_info->ui8_duration = 0;
                }              
            }
            else if( pt_aud_info->ui4_sample_size == 1 &&
                     pt_avi_hdlr_obj->ui8_duration )
            {
                /*use the video duration*/
                pt_aud_info->ui8_duration = 0;
            }
            else
            {
                /*CBR*/              
                if(pt_aud_info->ui4_avg_bytes_sec != 0)
                {
                    ui4_bytes_per_sec = pt_aud_info->ui4_avg_bytes_sec;    
                }
                else if(pt_aud_info->i2_bits_per_sample != 0)
                {
                    ui4_bytes_per_sec = 
                        (UINT32)((pt_aud_info->i2_bits_per_sample/8)*
                                 (pt_aud_info->i4_samples_per_sec));
                }
                else
                {
                    ui4_bytes_per_sec = 1;
                }               
                pt_aud_info->ui8_duration =        
                    _mm_div64((UINT64)pt_aud_info->ui4_length * pt_aud_info->ui4_sample_size * 90000, 
                              ui4_bytes_per_sec,
                              NULL);
            }
            if((pt_aud_info->ui8_duration > pt_avi_hdlr_obj->ui8_duration) &&
               (pt_aud_info->e_enc != MINFO_INFO_AUD_ENC_UNKNOWN))
            {
               pt_avi_hdlr_obj->ui8_duration = pt_aud_info->ui8_duration;
            }
          } 
          break;
        case MINFO_AVI_CHUNK_ID_STRN:
          if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_VIDS)
          {
            if((pt_chunk->ui4_size - 8)== 0x19)/* for 3D */
            {
                ui1_vid_stm_nums = pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num;
                ui1_vid_stm_nums--;
                if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
                {
                    break;
                }
                _mm_util_mem_copy(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums].ac_3d_identifier, 
                                  pui1_head_buf, 
                                  20);
                _mm_util_mem_copy(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums].ac_3d_version, 
                                  pui1_head_buf + 20, 
                                  4);
            }
            else if((pt_chunk->ui4_size - 8) >= 13) /* Subtitle - en-US*/
            {
              if(*pui1_head_buf == 'S') /* Video - en-US */
              {
                /* subtitle case */
                LOADB_WORD(pui1_head_buf + 11, ui2_lang);
                ui1_sp_stm_nums = (UINT8)(pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_num - 1);
                if (ui1_sp_stm_nums >= MAX_NS_AVI_SP)
                {
                    break;
                }
                _mm_lang_to_639_2(ui2_lang, 
                                  (ISO_639_LANG_T *)pt_avi_hdlr_obj->t_strm_inf.t_sp_strm_inf[ui1_sp_stm_nums].s_lang);
              }
            }
          }   
          else if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_AUDS)
          {
             if((pt_chunk->ui4_size - 8) >= 10)/* Audio - en-US */
             {
               LOADB_WORD(pui1_head_buf + 8, ui2_lang);
               ui1_aud_stm_nums = (UINT8)(pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num - 1);
                if (ui1_aud_stm_nums >= MAX_NS_AVI_AUD)
                {
                    break;
                }
               _mm_lang_to_639_2(ui2_lang, 
                                 (ISO_639_LANG_T *)pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].s_lang);
             }
          }
          break;
          
        /* meta data */
        case MINFO_AVI_CHUNK_ID_IART:        
          ui4_info_size = pt_chunk->ui4_size - 8;
          pt_avi_hdlr_obj->t_meta_data_info.pwc_str_director
                  = _mm_util_mem_alloc(sizeof(UTF16_T)*ui4_info_size);
          if (pt_avi_hdlr_obj->t_meta_data_info.pwc_str_director == NULL)
          {
               return MINFOR_OUT_OF_MEMORY;
          }
          pt_avi_hdlr_obj->t_meta_data_info.z_director_len = (SIZE_T)ui4_info_size;
          x_uc_ps_to_w2s((const CHAR*)pui1_head_buf, 
                         pt_avi_hdlr_obj->t_meta_data_info.pwc_str_director, 
                         ui4_info_size);
          break;
          
        case MINFO_AVI_CHUNK_ID_ICOP:       
          ui4_info_size = pt_chunk->ui4_size-8;        
          pt_avi_hdlr_obj->t_meta_data_info.pwc_str_copyright
                  = _mm_util_mem_alloc(sizeof(UTF16_T)*ui4_info_size);
          if (pt_avi_hdlr_obj->t_meta_data_info.pwc_str_copyright == NULL)
          {
               return MINFOR_OUT_OF_MEMORY;
          }
          pt_avi_hdlr_obj->t_meta_data_info.z_copyright_len = (SIZE_T)ui4_info_size;
          x_uc_ps_to_w2s((const CHAR*)pui1_head_buf, 
                         pt_avi_hdlr_obj->t_meta_data_info.pwc_str_copyright, 
                         ui4_info_size);
          break;
          
        case MINFO_AVI_CHUNK_ID_IGNR:       
          ui4_info_size = pt_chunk->ui4_size-8;        
          pt_avi_hdlr_obj->t_meta_data_info.pwc_str_genre
                  = _mm_util_mem_alloc(sizeof(UTF16_T)*ui4_info_size);
          if (pt_avi_hdlr_obj->t_meta_data_info.pwc_str_genre == NULL)
          {
               return MINFOR_OUT_OF_MEMORY;
          }
          pt_avi_hdlr_obj->t_meta_data_info.z_genre_len = (SIZE_T)ui4_info_size;
          x_uc_ps_to_w2s((const CHAR*)pui1_head_buf, 
                         pt_avi_hdlr_obj->t_meta_data_info.pwc_str_genre, 
                         ui4_info_size);
          break;
          
        case MINFO_AVI_CHUNK_ID_INAM:            
          ui4_info_size = pt_chunk->ui4_size-8;        
          pt_avi_hdlr_obj->t_meta_data_info.pwc_str_title
                  = _mm_util_mem_alloc(sizeof(UTF16_T)*ui4_info_size);
          if (pt_avi_hdlr_obj->t_meta_data_info.pwc_str_title == NULL)
          {
               return MINFOR_OUT_OF_MEMORY;
          }
          pt_avi_hdlr_obj->t_meta_data_info.z_title_len = (SIZE_T)ui4_info_size;
          x_uc_ps_to_w2s((const CHAR*)pui1_head_buf, 
                         pt_avi_hdlr_obj->t_meta_data_info.pwc_str_title, 
                         ui4_info_size);
          break;
          
        case MINFO_AVI_CHUNK_ID_INDX:
            if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == 
               MINFO_AVI_CHUNK_ID_VIDS)
            {
                if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type == 
                   MINFO_AVI_STRM_TYPE_VID)
                {
                    ui1_vid_stm_nums = (UINT8)(pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num - 1);
                    if (ui1_vid_stm_nums >= MAX_NS_AVI_VID)
                    {
                        break;
                    }
                    pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums].ui4_indx_offset = 
                        pt_chunk->ui4_offset;
                    pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[ui1_vid_stm_nums].ui4_indx_size = 
                        pt_chunk->ui4_size;
                }
                else if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type == 
                   MINFO_AVI_STRM_TYPE_SP)
                {
                     ui1_sp_stm_nums = (UINT8)(pt_avi_hdlr_obj->t_strm_inf.ui1_sp_strm_num - 1);
                     if (ui1_sp_stm_nums >= MAX_NS_AVI_SP)
                     {
                         break;
                     }
                     pt_avi_hdlr_obj->t_strm_inf.t_sp_strm_inf[ui1_sp_stm_nums].ui4_indx_offset = 
                        pt_chunk->ui4_offset;
                     pt_avi_hdlr_obj->t_strm_inf.t_sp_strm_inf[ui1_sp_stm_nums].ui4_indx_size = 
                        pt_chunk->ui4_size;
                }
            }
            else if(pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_chunk_id == 
                    MINFO_AVI_CHUNK_ID_AUDS)
            {
                ui1_aud_stm_nums = (UINT8)(pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num - 1);
                if (ui1_aud_stm_nums >= MAX_NS_AVI_AUD)
                {
                    break;
                }
                pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_indx_offset = 
                    pt_chunk->ui4_offset;
                pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_stm_nums].ui4_indx_size = 
                    pt_chunk->ui4_size;
            }
            break;  
        /*add for wave file format support*/
        case MINFO_AVI_CHUNK_ID_FMT:
            pt_avi_hdlr_obj->ui1_cur_strm_num = 1;            

            /*case MINFO_AVI_CHUNK_ID_STRF:   */
            DBG_INFO(("[avi_parser]MINFO_AVI_CHUNK_ID_STRF case  \r\n"));
         
            /* audio stream header info case */
            pt_chunk->pt_par_chunk->pt_first_sub_chunk->e_strm_type = 
                MINFO_AVI_STRM_TYPE_AUD;

            pt_aud_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
            pt_aud_info->e_enc = 
                _e_avi_aud_codec( pt_wfx->ui2_format_tag);

            pt_aud_info->i4_samples_per_sec = (INT32)pt_wfx->ui4_samples_per_sec;
            pt_aud_info->i2_bits_per_sample = (INT16)pt_wfx->ui2_bits_per_sample;
            pt_aud_info->i2_block_align = (INT16)pt_wfx->ui2_n_block_align;
            pt_aud_info->i2_channels = (INT16)pt_wfx->ui2_n_channels;
            pt_aud_info->ui4_avg_bytes_sec = pt_wfx->ui4_n_avg_bytes_per_sec;

            /*MPEG-LAYER3*/ 
            if(pt_wfx->ui2_format_tag == 0x55)
            {
                pt_aud_info->e_layer = 3;
            }

            pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num++;
            pt_avi_hdlr_obj->t_strm_inf.ui1_aud_strm_id_list[0] = 1;
            
            break;
        case MINFO_AVI_CHUNK_ID_DATA:
            pt_aud_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_scale = 1;
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_rate  = 
                (UINT32)pt_aud_info->i4_samples_per_sec;
            LOADB_WORD(pui1_head_buf + 4, pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_length);
            pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_sample_size = 1; 
            /*CBR*/              
            if(pt_aud_info->ui4_avg_bytes_sec != 0)
            {
                ui4_bytes_per_sec = pt_aud_info->ui4_avg_bytes_sec;    
            }
            else if(pt_aud_info->i2_bits_per_sample != 0)
            {
                ui4_bytes_per_sec = 
                    (UINT32)((pt_aud_info->i2_bits_per_sample/8)*
                             (pt_aud_info->i4_samples_per_sec)*
                             ( pt_aud_info->i2_channels));
            }
            else
            {
                ui4_bytes_per_sec = 1;
            }
            pt_aud_info->ui4_length = pt_chunk->ui4_size;            
            pt_aud_info->ui8_duration =  
                _mm_div64((UINT64)pt_aud_info->ui4_length * 
                           pt_aud_info->ui4_sample_size * 90000,  
                           ui4_bytes_per_sec,
                           NULL);

            if(pt_aud_info->ui8_duration > pt_avi_hdlr_obj->ui8_duration)
            {
               pt_avi_hdlr_obj->ui8_duration = pt_aud_info->ui8_duration;
            }
            break;
        default:
            break;
    }
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _i4_avi_build_rtff_tree
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/                            
static INT32 _i4_avi_build_rtff_tree(MINFO_OBJ_T*  pt_minfo_obj, 
                                     BOOL*         pb_is_wait)
{
    MINFO_AVI_RIFF_INF_T*  pt_avi_hdlr_obj;
    setpos_fct      pf_set_pos;
    getpos_fct      pf_get_pos; 
    copybytes_fct   pf_copybyte;
    /*UINT64          ui8_file_pos;*/
    UINT64          ui8_file_size;


    INT32 i4_ret = MINFOR_INTERNAL_ERR;
    UINT32 ui4_size = 0;
    UINT32 ui4_start_offset = 0;
    UINT32 ui4_read_sz = 0;
    UINT32 ui4_read;
    MINFO_AVI_CHUNK_ID_ENUM_T e_id;
    MINFO_AVI_CHUNK_T* pt_par_chunk = NULL;
    BOOL b_is_need_save;


    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input1 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input2 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;

    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   
    e_id = _e_get_chunk_id_and_size(pt_avi_hdlr_obj->pui1_header_buf, &ui4_size);
    ui4_size += 8;
                  
#if 1
    switch(pt_avi_hdlr_obj->e_parser_state)
     {
            case MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER:
                e_id = _e_get_chunk_id_and_size(pt_avi_hdlr_obj->pui1_header_buf, &ui4_size);
                ui4_size += 8;
                ui4_start_offset = pt_avi_hdlr_obj->ui4_cur_parser_offset - 8;
                if (MINFO_AVI_CHUNK_ID_NONE != e_id)
                {
                    
                    if (e_id == MINFO_AVI_CHUNK_ID_LIST || e_id == MINFO_AVI_CHUNK_ID_RIFF
#ifdef MM_AVI_DIVX6
                    ||e_id == MINFO_AVI_CHUNK_ID_MRIF
#endif
                    ||e_id == MINFO_AVI_CHUNK_ID_STRH)
                    {
                        if (e_id == MINFO_AVI_CHUNK_ID_LIST)
                        {
                            /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is LIST\n"));*/
                        }
                        else if (e_id == MINFO_AVI_CHUNK_ID_RIFF)
                        {
                         /*MINFO_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is RIFF\n"));*/
                        }
#ifdef MM_AVI_DIVX6
                        else if (e_id == MINFO_AVI_CHUNK_ID_MRIF)
                        {
                            /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is MRIF\n"));*/
                        }
#endif
                        else
                        {
                           /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is STRH\n"));*/
                        }
    
                        b_is_need_save = TRUE;
                        ui4_read_sz = 4;
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_TYPE;
                        if (e_id == MINFO_AVI_CHUNK_ID_RIFF)
                        {
                            pt_avi_hdlr_obj->ui8_riff_size = ui4_size;
                            pt_avi_hdlr_obj->ui8_base_offset = pt_avi_hdlr_obj->ui4_cur_parser_offset - 8;
                        }
#ifdef MM_AVI_DIVX6
    
                        if (e_id == MINFO_AVI_CHUNK_ID_MRIF)
                        {
                            pt_avi_hdlr_obj->ui8_riff_size = ui4_size;
                            pt_avi_hdlr_obj->ui8_base_offset = pt_avi_hdlr_obj->ui4_cur_parser_offset - 8;
                        }
#endif
                    }
                    else if (_b_is_chunk_need_parser(e_id))
                    {
                        b_is_need_save = TRUE;
                        ui4_read_sz = ui4_size - 8;
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_LOAD_INF;
                    }
                    else if (_b_is_chunk_need_check_sub(e_id))
                    {
                        b_is_need_save = TRUE;
                        ui4_read_sz = 8;
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                    }
                    else
                    {
                        b_is_need_save = FALSE;
                        /*skip data size : ui4_size - 8*/
                        pt_avi_hdlr_obj->ui4_cur_parser_offset += ui4_size - 8;
                        pf_set_pos(pt_minfo_obj , (INT64)pt_avi_hdlr_obj->ui4_cur_parser_offset, MEDIA_SEEK_BGN);
                        ui4_read_sz = 8;
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                    }
                    if (b_is_need_save)
                    {
                         i4_ret = _i4_add_chunk_to_tree(pt_minfo_obj,e_id, ui4_start_offset, ui4_size);
                    }
                #if 0
                    i4_ret = _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                                                    pt_avi_hdlr_obj->ui4_cur_parser_offset, 
                                                    &pt_par_chunk);
                    if (e_id == PB_AVI_CHUNK_ID_INDX)
                    {
                        mrSaveIndexOffsetInf(pt_par_chunk, u2StartOffset, u4Size);      
                        mrSkipData( u4Size - 8);
                        u4ReadSz = 8;
                    }
                    else 
                #endif
                    if ((e_id == MINFO_AVI_CHUNK_ID_IDX1) 
#ifdef MM_AVI_DIVX6
                         || (e_id == MINFO_AVI_CHUNK_ID_1IDX)
#endif
                         )
                    {
                        if (MINFO_AVI_CHUNK_ID_IDX1 == e_id)
                        {
                            pt_avi_hdlr_obj->b_is_menu_riff = FALSE;
                        }
                        else
                        {
                            pt_avi_hdlr_obj->b_is_menu_riff = TRUE;
                        }
                        /*Start offset include idxl 4cc code*/ 
                        pt_avi_hdlr_obj->ui4_indxtbl_offset = ui4_start_offset;
                        /*size doesn't include idxl 4cc code*/
                        pt_avi_hdlr_obj->ui4_indxtbl_sz = ui4_size - 8;                        
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF;                       
                        break;
                    }
                    
                    /*add for wave file format support*/
                    if (e_id == MINFO_AVI_CHUNK_ID_DATA)
                    {
                        UINT32 ui4_dts_flag;
                        UINT32 ui4_cnt;
                        
                        i4_ret = _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                                                         pt_avi_hdlr_obj->ui4_cur_parser_offset,  
                                                         &pt_par_chunk);
                        if(pt_par_chunk == NULL)
                        {
                            /*pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                            _i4_delete_riff_tree(pt_minfo_obj);
                            return MINFOR_OK;*/
                            pt_par_chunk = pt_avi_hdlr_obj->pt_head_chunk;
                        }
                        /*if chunk size if error, fix it with file size*/
                        /*pf_get_pos(pt_minfo_obj, &ui8_file_pos);*/
                        pf_set_pos(pt_minfo_obj, 0, MEDIA_SEEK_END);
                        pf_get_pos(pt_minfo_obj, &ui8_file_size);
                        if(pt_avi_hdlr_obj->ui4_cur_parser_offset > (UINT32)ui8_file_size)
                        {   /*wav is no more than 4GB, but the parameter of pf_get_pos is UINT64,
                                                      so force ui8_file_size to UINT32 here.*/
                            pt_avi_hdlr_obj->ui4_cur_parser_offset = (UINT32)ui8_file_size;
                            /*ui8_file_pos = ui8_file_size;*/
                            ui4_size = (UINT32)ui8_file_size - ui4_start_offset;
                        }
                        /*pf_set_pos(pt_minfo_obj, (INT64)ui8_file_pos, MEDIA_SEEK_BGN); */

                        pt_par_chunk->e_chunk_id = e_id;
                        pt_par_chunk->ui4_size = ui4_size;

                        _i4_check_and_save_chunk_inf(pt_minfo_obj,pt_par_chunk);

                        pt_avi_hdlr_obj->ui4_cur_parser_offset -= (ui4_size - 8);
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;

                        /*movie start not include 4cc code*/
                         pt_avi_hdlr_obj->ui4_mov_start_base = 
                              pt_avi_hdlr_obj->ui4_cur_parser_offset;    
                         /*movie size not inclde 4cc code*/   
                         pt_avi_hdlr_obj->ui8_mov_sz = (UINT64)(ui4_size - 8);
                        
                         /*for dts process*/
                         pf_set_pos(pt_minfo_obj, 
                                    (INT64)(pt_avi_hdlr_obj->ui4_mov_start_base), 
                                    MEDIA_SEEK_BGN);
                         i4_ret = pf_copybyte(pt_minfo_obj, 
                                              pt_avi_hdlr_obj->pui1_header_buf, 
                                              pt_avi_hdlr_obj->ui4_buf_length, 
                                              MAX_DTS_PARSE_DATA, 
                                              &ui4_read);
                         if (i4_ret == MINFOR_OK)
                         {
                             ui4_cnt = 0;
                             do
                             {
                                ui4_dts_flag = (UINT32)pt_avi_hdlr_obj->pui1_header_buf[ui4_cnt + 3] << 24 | 
                                               (UINT32)pt_avi_hdlr_obj->pui1_header_buf[ui4_cnt + 2] << 16 | 
                                               (UINT32)pt_avi_hdlr_obj->pui1_header_buf[ui4_cnt + 1] << 8  | 
                                               (UINT32)pt_avi_hdlr_obj->pui1_header_buf[ui4_cnt];
                                
                                 if (ui4_dts_flag == 0x80017FFE || ui4_dts_flag == 0xE8001FFF)
                                 {
                                    pt_avi_hdlr_obj->ui4_mov_start_base += ui4_cnt;  
                                    pt_avi_hdlr_obj->ui8_mov_sz         -= ui4_cnt;
                                    
                                    pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].e_enc = MINFO_INFO_AUD_ENC_DTS;
                                    break;
                                 }
                             }while(ui4_cnt ++ < ui4_read);
                         }

                        _i4_delete_riff_tree(pt_minfo_obj);

                         return MINFOR_OK;
                    }
                }
                else
                {
                    e_id = _e_get_chunk_id(pt_avi_hdlr_obj->pui1_header_buf + 1);
                    if (e_id != MINFO_AVI_CHUNK_ID_NONE)
                    {
                        pt_avi_hdlr_obj->ui4_cur_parser_offset -= 7;
                        ui4_read_sz = 8;
                        pf_set_pos(pt_minfo_obj , (INT64)pt_avi_hdlr_obj->ui4_cur_parser_offset, MEDIA_SEEK_BGN);   
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                        /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is none, 
                        Add one offset change to: %d\n", m_rRiffInf.ui4_cur_parser_offset));*/
                    }
                    else
                    {
                        i4_ret = _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                                                      pt_avi_hdlr_obj->ui4_cur_parser_offset, 
                                                      &pt_par_chunk);
                        if (pt_par_chunk != NULL)   
                        {
                            /* meta data info */
                            if(pt_par_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_INFO ||
                               pt_par_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_AVI)
                            {
                                ui4_size = _ui4_get_chunk_size(pt_avi_hdlr_obj->pui1_header_buf);
				/* the case : Having data between riff and the end*/
                                if (pt_par_chunk->e_chunk_id == MINFO_AVI_CHUNK_ID_AVI &&
				   ((ui4_size == 0) || (((UINT64)pt_avi_hdlr_obj->ui4_cur_parser_offset + (UINT64)ui4_size) > pt_avi_hdlr_obj->ui8_riff_size)))
                                {
                                    return MINFOR_OK;
                                }
                                if((ui4_size % 2) != 0)
                                {
                                    ui4_size += 1;
                                }
                                pt_avi_hdlr_obj->ui4_cur_parser_offset += ui4_size;
                                /* meta data size some error in the file: */
                                /* VBB0063_XVID_MPG4_640x272_603Kbps_MP3_Stereo_VBR.avi */
                                if(pt_avi_hdlr_obj->ui4_cur_parser_offset > 
                                   (pt_par_chunk->ui4_offset + pt_par_chunk->ui4_size))
                                {
                                    pt_avi_hdlr_obj->ui4_cur_parser_offset =  
                                        pt_par_chunk->ui4_offset + pt_par_chunk->ui4_size;
                                }
                            }
                            else
                            {
                                pt_avi_hdlr_obj->ui4_cur_parser_offset =  
                                    pt_par_chunk->ui4_offset + pt_par_chunk->ui4_size;
                            }
                            /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser chunk ID is none, current file offset change to: %d\n", m_rRiffInf.ui4_cur_parser_offset));*/
                            pf_set_pos(pt_minfo_obj , 
                                       (INT64)pt_avi_hdlr_obj->ui4_cur_parser_offset, 
                                       MEDIA_SEEK_BGN); 
                            ui4_read_sz = 8;
                            pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                        }
                        else
                        {
                          
                            pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                             /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: Chunk ID is none, Delete riff tree\n"));*/
                            _i4_delete_riff_tree(pt_minfo_obj);
                            return MINFOR_OK;
                        }
                    }
                }
                break;
            case MINFO_AVI_PARSER_STATE_CHECK_CHUNK_TYPE:
                i4_ret = _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                                                     pt_avi_hdlr_obj->ui4_cur_parser_offset,  
                                                     &pt_par_chunk);
                if(pt_par_chunk == NULL)
                {
                    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                    _i4_delete_riff_tree(pt_minfo_obj);
                    return MINFOR_OK;
                }                
                e_id = _e_get_chunk_id(pt_avi_hdlr_obj->pui1_header_buf);
                pt_par_chunk->e_chunk_id = e_id;
                if (e_id == MINFO_AVI_CHUNK_ID_MOV)
                {
                     /*movie start from 4cc code*/
                     pt_avi_hdlr_obj->ui4_mov_start_base = pt_avi_hdlr_obj->ui4_cur_parser_offset - 4;    
                     /*movie size inclde 4cc code*/                                    
                     pt_avi_hdlr_obj->ui8_mov_sz = (UINT64)(pt_par_chunk->ui4_size - 8);

                     /*if (m_rIndexParserInf.rSupperIndexInf.fgSuperIndexInfInited)
                     {
                        m_rRiffInf.e_parser_state = MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF;
                        u4ReadSz = 0;
                        break;
                     }*/
                }
                
                if (_b_is_chunk_need_parser(e_id))
                {
                    ui4_read_sz = pt_par_chunk->ui4_size - 12;
                    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_LOAD_INF;
                }
                else if (_b_is_chunk_need_check_sub(e_id))
                {
                        ui4_read_sz = 8;
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                }
                else
                {
                    pt_avi_hdlr_obj->ui4_cur_parser_offset = pt_par_chunk->ui4_offset + pt_par_chunk->ui4_size;
                    i4_ret = pf_set_pos(pt_minfo_obj , (INT64)pt_avi_hdlr_obj->ui4_cur_parser_offset, (MEDIA_SEEK_BGN | MEDIA_SEEK_CHK));
                    /* for cyberlink,driver will return busy if Wm seek further distance
                                     Mw will not skip the mov field to find idx1.
                                     CR: 320338 320211 317109 <<hwg>>
                                */
                    if((i4_ret != INPUTR_OK)&&(e_id == MINFO_AVI_CHUNK_ID_MOV))
                    {
                        pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                        pt_avi_hdlr_obj->b_non_seekable = TRUE;
                        _i4_delete_riff_tree(pt_minfo_obj);                      
                        return MINFOR_OK;
                    }
                    /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser Other chunk type, the current parser offset: %d\n", m_rRiffInf.ui4_cur_parser_offset));*/
                    ui4_read_sz = 8;
                    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                }
                break;
            case MINFO_AVI_PARSER_STATE_LOAD_INF:
                i4_ret = _i4_get_par_chunk_by_offset(pt_avi_hdlr_obj->pt_head_chunk, 
                                                     pt_avi_hdlr_obj->ui4_cur_parser_offset,  
                                                     &pt_par_chunk);
                if(pt_par_chunk == NULL)
                {
                    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                    _i4_delete_riff_tree(pt_minfo_obj);
                    return MINFOR_OK;
                }
                e_id = pt_par_chunk->e_chunk_id;
                i4_ret = _i4_check_and_save_chunk_inf(pt_minfo_obj,pt_par_chunk);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
                ui4_read_sz = 8;
                pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;
                break;
            case MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF:
                /*i4_ret = mrLoadIndexInf();
                if (i4_ret != PB_S_OK)
                {
                    return i4_ret;
                }*/
                pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                _i4_delete_riff_tree(pt_minfo_obj);
                /*PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: Delete riff tree\n"));*/
                return MINFOR_OK;
    
            default:
                break;
      }
#endif
       if (ui4_read_sz != 0)
       {
            i4_ret = _avi_check_header_buf(pt_avi_hdlr_obj, ui4_read_sz);
            if(i4_ret != MINFOR_OK)
            { 
                return MINFOR_INTERNAL_ERR;
            }
            i4_ret = pf_copybyte(pt_minfo_obj, 
                                 pt_avi_hdlr_obj->pui1_header_buf, 
                                 pt_avi_hdlr_obj->ui4_buf_length, 
                                 ui4_read_sz, 
                                 &ui4_read);
            pt_avi_hdlr_obj->ui4_cur_parser_offset += ui4_read_sz;
            /*i4_ret = mrReadData(u4ReadSz);*/
           /*copy failed*/
           if ((i4_ret != INPUTR_OK) || (ui4_read == 0))
           {
                
                /*For file with no index*/
                if (pt_avi_hdlr_obj->ui4_mov_start_base != 0)
                {
                    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_IDLE;
                    _i4_delete_riff_tree(pt_minfo_obj);
                    return MINFOR_OK;
                }
               /* PB_AVI_DBG_MSG_ARGU(("lp_avi_parser: When build tree, fail to read data\n"));*/
                return MINFOR_INTERNAL_ERR;
           }
           *pb_is_wait = FALSE;
           /*(ui4_read_sz > 2048 ) || 
           (pt_avi_hdlr_obj->e_parser_state == MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF);
           */
           #if 1
           if (pt_avi_hdlr_obj->e_parser_state != MINFO_AVI_PARSER_STATE_LOAD_INDEX_INF)
           {
                /*m_uParserProgress = 10;*/
           }
           #endif
       }
       
    return MINFO_AVI_PARSE_NEED_NEXT_LOOP; 
}

static INT32 _i4_set_video_codec_info(MINFO_OBJ_T*  pt_minfo_obj, UINT8 i1_index)
{
    UINT8                           ui1_1st_vid_strm_id;
    UINT8                           ui1_strm_num;
    UINT16                          ui2_strm_type;  
    INT32                           i4_ret;
    UINT32                          ui4_four_cc;
    UINT32                          ui4_size;
    UINT32                          ui4_idx, ui4_cnt;
    UINT32                          ui4_read_size;
    UINT32                          ui4_curr_size;
    UINT32                          ui4_cur_offset;
    UINT64                          ui8_tmp_pos;
    BOOL                            b_is_save_codec_info;
    BOOL                            b_is_set_codec_info_done;  
    setpos_fct                      pf_set_pos;   
    getpos_fct                      pf_get_pos;     
    copybytes_fct                   pf_copybyte;
    input4bytes_fct                 pf_inp4byte;  
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj;
    MINFO_AVI_VIDEO_STM_ATRBT_T*    pt_vid_strm_info;
    MINFO_AVI_VIDEO_CODEC_INFO_T*   pt_vid_codec_info;
    UINT8*                          pui1_index_buf;
        
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
   
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/ 
    if(pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num == 0)      
    {
        DBG_INFO(("INFO: no video stream. \r\n"));
        return MINFOR_OK;
    }
    i1_index--;
    if(i1_index >= MAX_NS_AVI_VID)
    {
        return MINFOR_INV_ARG;
    }
    b_is_save_codec_info = FALSE;
    b_is_set_codec_info_done = FALSE;
    /* For first video stream */
    pt_vid_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[i1_index]);
    pt_vid_strm_info->pui1_codec_info = NULL;
    pt_vid_strm_info->ui4_codec_info_size = 0; 
    ui1_1st_vid_strm_id = pt_avi_hdlr_obj->t_strm_inf.ui1_vid_strm_id_list[i1_index];

    /* check 'idx1' first for high efficiency */
    if(pt_avi_hdlr_obj->ui4_indxtbl_offset > 0)       
    {
        /* seek to chunk entry */
        pf_set_pos(pt_minfo_obj , 
                   (INT64)pt_avi_hdlr_obj->ui4_indxtbl_offset + 8, 
                   MEDIA_SEEK_BGN);
        ui4_idx = 0;
        while (ui4_idx < pt_avi_hdlr_obj->ui4_indxtbl_sz)
        {
            ui4_curr_size = pt_avi_hdlr_obj->ui4_indxtbl_sz - ui4_idx;
            if(ui4_curr_size >= AVI_HEADER_BUF_LENGTH)
            {
                ui4_curr_size = AVI_HEADER_BUF_LENGTH;
            }
            pf_copybyte(pt_minfo_obj, 
                        pt_avi_hdlr_obj->pui1_header_buf, 
                        pt_avi_hdlr_obj->ui4_buf_length,
                        ui4_curr_size, 
                        &ui4_read_size);
            ui4_cnt = 0;
            while(ui4_cnt < ui4_curr_size)
            {
                pui1_index_buf = pt_avi_hdlr_obj->pui1_header_buf + ui4_cnt;
                LOADL_DWRD(pui1_index_buf, ui4_four_cc);
                ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);
                if(ui1_strm_num == ui1_1st_vid_strm_id)
                {
                    ui2_strm_type = (UINT16)(ui4_four_cc >> 16);
                    if(0 == x_memcmp(&ui2_strm_type , "db", 2))
                    {
                        if(b_is_save_codec_info)
                        {
                            pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                        VID_STRM_ID_DB | 
                                        (((ui1_strm_num % 10) + 0x30) << 8) | 
                                        (((ui1_strm_num % 100) / 10) + 0x30);
                            b_is_set_codec_info_done = TRUE;
                            break;
                        }
                        pt_vid_codec_info = (MINFO_AVI_VIDEO_CODEC_INFO_T *)
                            (_mm_util_mem_alloc(sizeof(MINFO_AVI_VIDEO_CODEC_INFO_T)));
                        if(pt_vid_codec_info == NULL)
                        {
                            return MINFOR_OUT_OF_MEMORY;
                        }
                        pt_vid_codec_info->e_type = MINFO_AVI_VIDEO_CODEC_INFO_TYPE_XXDB;
                        LOADL_DWRD(pui1_index_buf + 4, ui4_four_cc); /* flag */
                        LOADL_DWRD(pui1_index_buf + 8, ui4_four_cc); /* offset */
                        pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_offset = ui4_four_cc;
                        LOADL_DWRD(pui1_index_buf + 12, ui4_four_cc); /* length */
                        pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_size = ui4_four_cc;
                        /* check is abs offset or not */
                        /* seek to movi data(not include 'movi') */
                        pf_get_pos(pt_minfo_obj, &ui8_tmp_pos);
                        pf_set_pos(pt_minfo_obj, 
                                   (INT64)pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_offset, 
                                   MEDIA_SEEK_BGN);
                        pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* get ckid 'xxdb' */
                        pf_inp4byte(pt_minfo_obj, &ui4_size); /* get 'xxdb' size */
                        ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);
                        ui2_strm_type = (UINT16)(ui4_four_cc >> 16);
                        if((ui1_strm_num != ui1_1st_vid_strm_id) ||
                           (0 != x_memcmp(&ui2_strm_type, "db", 2)) ||
                           (ui4_size != pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_size))
                        {
                            pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_offset += 
                                pt_avi_hdlr_obj->ui4_mov_start_base;
                        }
                           
                        pt_vid_strm_info->pui1_codec_info = (UINT8 *)pt_vid_codec_info;
                        pt_vid_strm_info->ui4_codec_info_size = sizeof(MINFO_AVI_VIDEO_CODEC_INFO_T);
                        pf_set_pos(pt_minfo_obj, (INT64)ui8_tmp_pos, MEDIA_SEEK_BGN);
                        b_is_save_codec_info = TRUE;
                    }
                    else if(0 == x_memcmp(&ui2_strm_type , "dc", 2))
                    {
                        pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                    VID_STRM_ID_DC | 
                                    (((ui1_strm_num % 10) + 0x30) << 8) | 
                                    (((ui1_strm_num % 100) / 10) + 0x30);
                        b_is_set_codec_info_done = TRUE;
                        break;
                    }
                    else if(0 == x_memcmp(&ui2_strm_type , "iv", 2))
                    {
                        pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                    VID_STRM_ID_IV | 
                                    (((ui1_strm_num % 10) + 0x30) << 8) | 
                                    (((ui1_strm_num % 100) / 10) + 0x30);
                        b_is_set_codec_info_done = TRUE;
                        break;
                    }
                }
                ui4_cnt += 16;
            }
            if(b_is_set_codec_info_done)
            {
                break;
            }
            ui4_idx += ui4_curr_size;
        }
    }
    else /* if no 'idx1' need check first RIFF movi chunk */
    {
        /* seek to first RIFF movi chunk */
        ui4_cur_offset = pt_avi_hdlr_obj->ui4_mov_start_base + 4;
        pf_set_pos(pt_minfo_obj , 
                   (INT64)ui4_cur_offset, 
                   MEDIA_SEEK_BGN);
        while(1)
        {
            i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
            if(i4_ret != INPUTR_OK && i4_ret != INPUTR_EOF)
            {
                return MINFOR_FILE_ERR;
            }
            if (i4_ret == INPUTR_EOF){
                break;
            }
            ui4_cur_offset += 4;
            if(0 == x_memcmp(&ui4_four_cc , "LIST" ,4))
            {
                pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
                pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* 'rec ' */
                ui4_cur_offset += 8;
            }
            else
            {
                pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
                if((ui4_size % 2) != 0)
                {
                    ui4_size += 1;
                }
                    
                ui4_cur_offset += 4;
                ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);
                if(ui1_strm_num == ui1_1st_vid_strm_id)
                {
                    ui2_strm_type = (UINT16)(ui4_four_cc >> 16);
                    if(0 == x_memcmp(&ui2_strm_type , "db", 2))
                    {
                        if(b_is_save_codec_info)
                        {
                            pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                        VID_STRM_ID_DB | 
                                        (((ui1_strm_num % 10) + 0x30) << 8) | 
                                        (((ui1_strm_num % 100) / 10) + 0x30);
                            break;
                        }
                        pt_vid_codec_info = (MINFO_AVI_VIDEO_CODEC_INFO_T *)
                            (_mm_util_mem_alloc(sizeof(MINFO_AVI_VIDEO_CODEC_INFO_T)));
                        if(pt_vid_codec_info == NULL)
                        {
                            return MINFOR_OUT_OF_MEMORY;
                        }
                        pt_vid_codec_info->e_type = MINFO_AVI_VIDEO_CODEC_INFO_TYPE_XXDB;
                        pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_offset = ui4_cur_offset;
                        pt_vid_codec_info->u.t_avi_vid_codec_xxdb.ui4_size = ui4_size;
                        pt_vid_strm_info->pui1_codec_info = (UINT8 *)pt_vid_codec_info;
                        pt_vid_strm_info->ui4_codec_info_size = sizeof(MINFO_AVI_VIDEO_CODEC_INFO_T);
                        b_is_save_codec_info = TRUE;
                    }
                    else if(0 == x_memcmp(&ui2_strm_type , "dc", 2))
                    {
                        pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                    VID_STRM_ID_DC | 
                                    (((ui1_strm_num % 10) + 0x30) << 8) | 
                                    (((ui1_strm_num % 100) / 10) + 0x30);
                        break;
                    }
                    else if(0 == x_memcmp(&ui2_strm_type , "iv", 2))
                    {
                        pt_avi_hdlr_obj->aui4_video_strm_id[i1_index] = 
                                    VID_STRM_ID_IV | 
                                    (((ui1_strm_num % 10) + 0x30) << 8) | 
                                    (((ui1_strm_num % 100) / 10) + 0x30);
                        break;
                    }
                }
                pf_set_pos(pt_minfo_obj, (INT64)ui4_size, MEDIA_SEEK_CUR);
                ui4_cur_offset += ui4_size;
            }
        }
    }
    return MINFOR_OK;
}

static INT32 _i4_set_audio_pts_offset(MINFO_OBJ_T*  pt_minfo_obj)
{
    UINT8                           ui1_strm_num;
    UINT16                          ui2_strm_type;  
    INT32                           i4_ret;
    UINT32                          ui4_four_cc;
    UINT32                          ui4_cur_offset;
    setpos_fct                      pf_set_pos;
    input4bytes_fct                 pf_inp4byte;
    input2bytes_fct                 pf_inp2byte;
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj;
    MINFO_AVI_AUDIO_STM_ATRBT_T*    pt_aud_strm_info;
    UINT32                          ui4_aud_cbr_count;
    UINT32                          ui4_aud_vbr_count;
    BOOL                            b_is_found;
    UINT8                           ui1_aud_strm_count;
    UINT8                           ui1_aud_found_count = 0xFF;
    UINT8                           ui1_cur_aud_count;
    UINT16                          ui2_aud_chunk_bytes;
    UINT32                          ui4_cbr_pts;
    UINT32                          ui4_vbr_pts;
    UINT64                          ui8_numerator;
    UINT64                          ui8_denominator;
    UINT32                          ui4_size;
    UINT8                           ui1_vbr_aud_strm_cnt = 0;
        
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_inp2byte = pt_minfo_obj->t_input_fct_tbl.pf_input2;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
   
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/ 
    if(pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num == 0)      
    {
        DBG_INFO(("INFO: no audio stream. \r\n"));
        return MINFOR_OK;
    }

    ui4_aud_cbr_count = 0;
    ui4_aud_vbr_count = 0;
    b_is_found        = FALSE;

    ui1_aud_strm_count = pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num;
    if(ui1_aud_strm_count > MAX_NS_AVI_AUD)
    {
        ui1_aud_strm_count = MAX_NS_AVI_AUD;
    }

    pt_aud_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
    pt_aud_strm_info->ui4_cbr_sub_vbr = 0;  
    if(0 == pt_aud_strm_info->ui4_sample_size)
    {
        ui1_vbr_aud_strm_cnt ++;
    }

    if(0 == ui1_vbr_aud_strm_cnt)
    {
        return MINFOR_OK;
    }
    
    /* seek to first RIFF movi chunk */
    ui4_cur_offset = pt_avi_hdlr_obj->ui4_mov_start_base + 4;
    pf_set_pos(pt_minfo_obj , 
               (INT64)ui4_cur_offset, 
               MEDIA_SEEK_BGN);
    while(1)
    {
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
        if(i4_ret != INPUTR_OK && i4_ret != INPUTR_EOF)
        {
            return MINFOR_FILE_ERR;
        }
        if (i4_ret == INPUTR_EOF)
        {
            break;
        }
        if(0 == x_memcmp(&ui4_four_cc , "LIST" ,4))
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* 'rec ' */
        }
        else if(0 == x_memcmp(&ui4_four_cc , "RIFF" ,4)) /*for multi-fiff*/
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* 'AVIX' */
        }
        else
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            if((ui4_size % 2) != 0)
            {
                ui4_size += 1;
            }
                
            ui2_strm_type = (UINT16)(ui4_four_cc >> 16);
            if(0 == x_memcmp(&ui2_strm_type , "wb", 2)) /*not audio stream*/
            {
                if(0xFF == ui1_aud_found_count)
                {
                    ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);
    
                    for(ui1_cur_aud_count = 0; ui1_cur_aud_count < ui1_aud_strm_count; ui1_cur_aud_count ++)
                    {
                        if(ui1_strm_num == pt_avi_hdlr_obj->t_strm_inf.ui1_aud_strm_id_list[ui1_cur_aud_count])
                        {
                            ui1_aud_found_count = ui1_cur_aud_count;
                            /*if(ui1_aud_strm_count > MAX_NS_AVI_AUD)
                          	{
                          		  ui1_aud_strm_count = MAX_NS_AVI_AUD;
                          	}*/
                            break;
                        }
                    }
                }
                if(ui1_aud_found_count < ui1_aud_strm_count)
                {
                    pt_aud_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_found_count]);
                    if(0 == pt_aud_strm_info->ui4_sample_size) /*it's vbr stream*/
                    {
                        pf_inp2byte(pt_minfo_obj, &ui2_aud_chunk_bytes); /* read first 2 bytes */

                        if(0 == ui2_aud_chunk_bytes) /*will continue searching*/
                        {
                            ui4_aud_vbr_count += 1;
                            ui4_aud_cbr_count += ui4_size;
                        }
                        else  /*find the matched audio chunk*/
                        {
                            b_is_found = TRUE;
                            break;
                        }
                        
                        ui4_size -= 2; /*substract input 2 bytes*/
                    }                
                }                
            }          
            pf_set_pos(pt_minfo_obj, (INT64)ui4_size, MEDIA_SEEK_CUR);
        }
    }

    if(0xFF != ui1_aud_found_count && b_is_found)
    {
        pt_aud_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[ui1_aud_found_count]);
        
        /*cbr pts*/
        ui8_numerator = (UINT64)ui4_aud_cbr_count * 
                              MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        ui8_denominator = pt_aud_strm_info->ui4_rate;
        ui4_cbr_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
        /*vbr pts*/
        ui8_numerator = (UINT64)ui4_aud_vbr_count * 
                                     pt_aud_strm_info->ui4_scale *
                                  MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        ui8_denominator = pt_aud_strm_info->ui4_rate;
        ui4_vbr_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
          
        if(ui4_cbr_pts > ui4_vbr_pts)
        {
            pt_aud_strm_info->ui4_cbr_sub_vbr = ui4_cbr_pts - ui4_vbr_pts;
        }
        else
        {
            pt_aud_strm_info->ui4_cbr_sub_vbr = 0;
        }        
    }    

    return MINFOR_OK;
}

static INT32 _i4_set_idx_info(MINFO_OBJ_T*  pt_minfo_obj)
{
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj = NULL;
    BOOL                            b_need_build_keytbl = FALSE;

    if(NULL == pt_minfo_obj)
    {
        return MINFOR_INV_ARG;
    }
    
    if(pt_minfo_obj->t_media_format.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {    /*it's wav format, return directly*/
        return MINFOR_OK;
    }
    
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
    
    if(pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num == 0)
    {
        DBG_INFO(("INFO: no audio stream. \r\n"));
        return MINFOR_OK;
    }    

    if((pt_avi_hdlr_obj->ui4_indxtbl_offset != 0) &&
       (pt_avi_hdlr_obj->ui4_indxtbl_sz != 0))
    {
        return MINFOR_OK;
    }

    if(pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num == 0)
    {   /*build audio key only if there has no video in avi file*/
        /*pt_vid_info = &(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[0]);*/
                 
        /*if(pt_vid_info->e_enc == MINFO_INFO_VID_ENC_UNKNOWN)*/
        {
            b_need_build_keytbl = TRUE;
        }           
    }
    else
    {
        b_need_build_keytbl = FALSE;
    }
    
    if(b_need_build_keytbl)
    {
        pt_avi_hdlr_obj->ui4_indxtbl_offset = 0xFFFFFFFF;
        pt_avi_hdlr_obj->ui4_indxtbl_sz     = 0xFFFFFFFF;
        DBG_INFO(("INFO: need build key table without index chunk. \r\n"));
    }

    return MINFOR_OK;
}

static INT32 _i4_parse_dts_chunck(MINFO_OBJ_T*  pt_minfo_obj, BOOL b_is_dtshd)
{
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj = NULL;
    setpos_fct                      pf_set_pos;     
    getpos_fct                      pf_get_pos;   
    copybytes_fct                   pf_copybyte;
    INT32                           i4_ret = MINFOR_INTERNAL_ERR;
    UINT32                          ui4_read_size;
    UINT64                          ui8_chunk_size;
    UINT64                          ui8_data_offset;
    UINT8                           aui1_dts_buffer[8];
    MINFO_AVI_AUDIO_STM_ATRBT_T*    pt_aud_info;

    if(NULL == pt_minfo_obj)
    {
        return MINFOR_INV_ARG;
    }

    if ((pt_minfo_obj->t_input_fct_tbl.pf_input1 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input2 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;    
    
   
    i4_ret = pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_BGN);
    while (i4_ret == MINFOR_OK)
    {
        i4_ret = pf_copybyte(pt_minfo_obj, 
                             aui1_dts_buffer, 
                             8, 
                             8, 
                             &ui4_read_size);                
        if (i4_ret != MINFOR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        if(b_is_dtshd)
        {
            if (x_memcmp((VOID*)"DTSHDHDR", aui1_dts_buffer, 8) == 0)
            {
                /* file size */
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                     aui1_dts_buffer, 
                                     8, 
                                     8, 
                                     &ui4_read_size);
                if (i4_ret != MINFOR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                ui8_chunk_size = ((UINT64)aui1_dts_buffer[0] << 56) | 
                                 ((UINT64)aui1_dts_buffer[1] << 48) |
                                 ((UINT64)aui1_dts_buffer[2] << 40) |
                                 ((UINT64)aui1_dts_buffer[3] << 32) |
                                 ((UINT64)aui1_dts_buffer[4] << 24) |
                                 ((UINT64)aui1_dts_buffer[5] << 16) |
                                 ((UINT64)aui1_dts_buffer[6] << 8)  |
                                 (UINT64)aui1_dts_buffer[7];
                i4_ret = pf_set_pos(pt_minfo_obj , (INT64)ui8_chunk_size, MEDIA_SEEK_CUR);
                if (i4_ret != MINFOR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
            }
            else if((x_memcmp((VOID*)"CORESSMD", aui1_dts_buffer, 8) == 0) || 
                    (x_memcmp((VOID*)"AUPR-HDR", aui1_dts_buffer, 8) == 0))
            {
                /* file size */
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                     aui1_dts_buffer, 
                                     8, 
                                     8, 
                                     &ui4_read_size);
                if (i4_ret != MINFOR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                ui8_chunk_size = ((UINT64)aui1_dts_buffer[0] << 56) | 
                                 ((UINT64)aui1_dts_buffer[1] << 48) |
                                 ((UINT64)aui1_dts_buffer[2] << 40) |
                                 ((UINT64)aui1_dts_buffer[3] << 32) |
                                 ((UINT64)aui1_dts_buffer[4] << 24) |
                                 ((UINT64)aui1_dts_buffer[5] << 16) |
                                 ((UINT64)aui1_dts_buffer[6] << 8)  |
                                 (UINT64)aui1_dts_buffer[7];
                i4_ret = pf_set_pos(pt_minfo_obj , (INT64)ui8_chunk_size, MEDIA_SEEK_CUR);
                if (i4_ret != MINFOR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
            }
            else if(x_memcmp((VOID*)"STRMDATA", aui1_dts_buffer, 8) == 0)
            {
                /* file size */
                i4_ret = pf_copybyte(pt_minfo_obj, 
                                     aui1_dts_buffer, 
                                     8, 
                                     8, 
                                     &ui4_read_size);
                if (i4_ret != MINFOR_OK)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                ui8_chunk_size = ((UINT64)aui1_dts_buffer[0] << 56) | 
                                 ((UINT64)aui1_dts_buffer[1] << 48) |
                                 ((UINT64)aui1_dts_buffer[2] << 40) |
                                 ((UINT64)aui1_dts_buffer[3] << 32) |
                                 ((UINT64)aui1_dts_buffer[4] << 24) |
                                 ((UINT64)aui1_dts_buffer[5] << 16) |
                                 ((UINT64)aui1_dts_buffer[6] << 8)  |
                                 (UINT64)aui1_dts_buffer[7];
                pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num = 1;

                pf_get_pos(pt_minfo_obj, &ui8_data_offset);

                /*movie start not include 4cc code*/
                pt_avi_hdlr_obj->ui4_mov_start_base = (UINT32)ui8_data_offset;    
                /*movie size not inclde 4cc code*/   
                pt_avi_hdlr_obj->ui8_mov_sz = ui8_chunk_size;
                pt_avi_hdlr_obj->ui8_riff_size = ui8_chunk_size + ui8_data_offset;

                pt_aud_info = 
                    &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
                pt_aud_info->e_enc = MINFO_INFO_AUD_ENC_DTS;         
                
                pt_aud_info->i4_samples_per_sec = 0xFF;
                pt_aud_info->i2_bits_per_sample = 0;
                pt_aud_info->i2_block_align = 1;
                pt_aud_info->i2_channels = 2;
                pt_aud_info->ui4_avg_bytes_sec = 0;
                pt_avi_hdlr_obj->ui8_duration = 0;  
             
                return MINFOR_OK;
            }
        }
        else  /*it's cpt file*/
        {
            i4_ret = pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_END);
            if (i4_ret != MINFOR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num = 1;

            pf_get_pos(pt_minfo_obj, &ui8_data_offset);
            if(ui8_data_offset < 4)
            {
                return MINFOR_INTERNAL_ERR;
            }

            /*movie start not include 4cc code*/
            pt_avi_hdlr_obj->ui4_mov_start_base = 4;
            /*movie size not inclde 4cc code*/   
            pt_avi_hdlr_obj->ui8_mov_sz = ui8_data_offset - 4;
            pt_avi_hdlr_obj->ui8_riff_size = ui8_data_offset;
            
            pt_aud_info = 
                &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
            pt_aud_info->e_enc = MINFO_INFO_AUD_ENC_DTS;         
            
            pt_aud_info->i4_samples_per_sec = 0xFF;
            pt_aud_info->i2_bits_per_sample = 0;
            pt_aud_info->i2_block_align = 1;
            pt_aud_info->i2_channels = 2;
            pt_aud_info->ui4_avg_bytes_sec = 0;
            pt_avi_hdlr_obj->ui8_duration = 0;

            return MINFOR_OK;
        }
    }

    return MINFOR_OK;
}
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
static INT32 _i4_check_aud_vid_interleave(MINFO_OBJ_T*  pt_minfo_obj)
{
    UINT8                           ui1_strm_num;
    UINT16                          ui2_strm_type;  
    INT32                           i4_ret;
    UINT32                          ui4_four_cc;
    UINT32                          ui4_cur_offset;
    setpos_fct                      pf_set_pos;
    input4bytes_fct                 pf_inp4byte;
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj;
    MINFO_AVI_AUDIO_STM_ATRBT_T*    pt_aud_strm_info;
    MINFO_AVI_VIDEO_STM_ATRBT_T*    pt_vid_strm_info;
    UINT32                          ui4_vid_chunk_count;
    UINT32                          ui4_aud_chunk_count;
    UINT32                          ui4_vid_pts = 0;
    UINT32                          ui4_aud_pts = 0;
    UINT64                          ui8_numerator;
    UINT64                          ui8_denominator;
    UINT32                          ui4_size;
    UINT32                          ui4_limit_dur = 0;
    UINT32                          ui4_sub_pts = 0;
        
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
   
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/ 
    if(pt_avi_hdlr_obj->t_strm_inf.ui1_audio_strm_num == 0 ||
       pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num == 0)      
    {
        DBG_INFO(("INFO: no audio or video stream. \r\n"));
        return MINFOR_OK;
    }

    if(pt_avi_hdlr_obj->ui8_duration > 3*90000)
    {
        ui4_limit_dur = 3*90000;
    }
    else
    {
        ui4_limit_dur = pt_avi_hdlr_obj->ui8_duration;
    }
    ui4_vid_chunk_count = 0;
    ui4_aud_chunk_count = 0;

    pt_aud_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0]);
    pt_vid_strm_info = &(pt_avi_hdlr_obj->t_strm_inf.t_vid_strm_inf[0]);

    /* seek to first RIFF movi chunk */
    ui4_cur_offset = pt_avi_hdlr_obj->ui4_mov_start_base + 4;
    pf_set_pos(pt_minfo_obj , 
               (INT64)ui4_cur_offset, 
               MEDIA_SEEK_BGN);
    while(1)
    {
        i4_ret = pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
        if(i4_ret != INPUTR_OK)
        {
            return MINFOR_FILE_ERR;
        }
        if(0 == x_memcmp(&ui4_four_cc , "LIST" ,4))
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* 'rec ' */
        }
        else if(0 == x_memcmp(&ui4_four_cc , "RIFF" ,4)) /*for multi-fiff*/
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            pf_inp4byte(pt_minfo_obj, &ui4_four_cc); /* 'AVIX' */
        }
        else
        {
            pf_inp4byte(pt_minfo_obj, &ui4_size); /* size */
            if((ui4_size % 2) != 0)
            {
                ui4_size += 1;
            }
                
            ui2_strm_type = (UINT16)(ui4_four_cc >> 16);
            if(0 == x_memcmp(&ui2_strm_type , "wb", 2)) /*not audio stream*/
            {
                ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);    

                if(ui1_strm_num == pt_avi_hdlr_obj->t_strm_inf.ui1_aud_strm_id_list[0])
                {                    
                    if(0 == pt_aud_strm_info->ui4_sample_size) /*it's vbr stream*/
                    {
                        ui4_aud_chunk_count ++;                      
                        ui8_numerator   = (UINT64)ui4_aud_chunk_count * pt_aud_strm_info->ui4_scale * 90000;
                    }
                    else
                    {
                        ui4_aud_chunk_count += ui4_size;
                        ui8_numerator   = (UINT64)ui4_aud_chunk_count * 90000;
                    }
                    ui8_denominator = (UINT64)pt_aud_strm_info->ui4_rate;

                    ui4_aud_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
                    if(ui4_aud_pts >= ui4_limit_dur && ui4_vid_pts >= ui4_limit_dur)
                    {
                        break;
                    }
                    if (ui4_aud_pts >= ui4_limit_dur && ui4_vid_pts == 0)
                    {
                        break;
                    }

                }
            }
            else if(0 == x_memcmp(&ui2_strm_type , "dc", 2)) /*video stream*/
            {               
                ui1_strm_num = (UINT8)AVI_PARSER_GET_STRM_NUM_FROM_CHUNK_ID(ui4_four_cc);

                if(ui1_strm_num == pt_avi_hdlr_obj->t_strm_inf.ui1_vid_strm_id_list[0])
                {
                    ui4_vid_chunk_count ++;

                    ui8_numerator   = (UINT64)ui4_vid_chunk_count * pt_vid_strm_info->ui4_scale * 90000;
                    ui8_denominator = (UINT64)pt_vid_strm_info->ui4_rate;

                    ui4_vid_pts = (UINT32)_mm_div64(ui8_numerator, ui8_denominator, NULL);
                    if(ui4_vid_pts >= ui4_limit_dur && ui4_aud_pts >= ui4_limit_dur)
                    {
                        break;
                    }
                    if (ui4_vid_pts >= ui4_limit_dur && ui4_aud_pts == 0)
                    {
                        break;
                    }
                }
            }
            pf_set_pos(pt_minfo_obj, (INT64)ui4_size, MEDIA_SEEK_CUR);
        }
    }

    if(ui4_vid_pts > ui4_aud_pts)
    {
        ui4_sub_pts = ui4_vid_pts - ui4_aud_pts;
    }
    else
    {
        ui4_sub_pts = ui4_aud_pts - ui4_vid_pts;
    }

    if(ui4_sub_pts > 45000)
    {
        return MINFOR_INTERNAL_ERR;
    }
       

    return MINFOR_OK;
}
#endif

static INT32 _i4_cal_video_dur(MINFO_OBJ_T*  pt_minfo_obj)
{
    INT32                           i4_ret;
    UINT32                          ui4_four_cc;
    UINT32                          ui4_cur_offset;
    setpos_fct                      pf_set_pos;
    copybytes_fct                   pf_copybyte;
    MINFO_AVI_RIFF_INF_T*           pt_avi_hdlr_obj;
    UINT32                          ui4_vid_frms;
    UINT64                          ui8_total_dur;
    UINT32                          ui4_size;
    UINT32                          ui4_read_size;
    UCHAR*                          pui1_index_buf;
        
    pf_set_pos  = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;    
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
   
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/ 
    if(pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num == 0)      
    {
        DBG_INFO(("INFO: no video stream. \r\n"));
        return MINFOR_OK;
    }

    /* seek to first RIFF movi chunk */
    ui4_vid_frms   = 0;
    ui4_cur_offset = pt_avi_hdlr_obj->ui4_mov_start_base + 4;
    while(ui4_cur_offset < (pt_avi_hdlr_obj->ui4_mov_start_base + pt_avi_hdlr_obj->ui8_mov_sz))
    {
        pf_set_pos(pt_minfo_obj , 
               (INT64)ui4_cur_offset, 
               MEDIA_SEEK_BGN);
        i4_ret = pf_copybyte(pt_minfo_obj, 
                            pt_avi_hdlr_obj->pui1_header_buf, 
                            pt_avi_hdlr_obj->ui4_buf_length,
                            AVI_HEADER_BUF_LENGTH, 
                            &ui4_read_size);
        if(i4_ret != INPUTR_OK && i4_ret != INPUTR_EOF)
        {
            return MINFOR_FILE_ERR;
        }
        if(ui4_read_size < 12)
        {
            break;
        }

        pui1_index_buf = pt_avi_hdlr_obj->pui1_header_buf;
        while(pui1_index_buf <= pt_avi_hdlr_obj->pui1_header_buf + ui4_read_size - 12)
        {
            LOADL_DWRD(pui1_index_buf, ui4_four_cc);        
            ui4_cur_offset += 4;
            pui1_index_buf += 4;
            
            if(0 == x_memcmp(&ui4_four_cc , "LIST" ,4))
            {
                LOADL_DWRD(pui1_index_buf, ui4_size); /* size */
                LOADL_DWRD(pui1_index_buf + 4, ui4_four_cc); /* 'rec ' */
                pui1_index_buf += 8;
                ui4_cur_offset += 8;
            }
            else if(0 == x_memcmp(&ui4_four_cc , "RIFF" ,4)) /*for multi-fiff*/
            {
                LOADL_DWRD(pui1_index_buf, ui4_size); /* size */
                LOADL_DWRD(pui1_index_buf + 4, ui4_four_cc); /* 'AVIX' */
                pui1_index_buf += 8;
                ui4_cur_offset += 8;
            }
            else
            {
                LOADL_DWRD(pui1_index_buf, ui4_size); /* size */
                ui4_cur_offset += 4;
                pui1_index_buf += 4;
                
                if((ui4_size % 2) != 0)
                {
                    ui4_size += 1;
                }
                    
                if(0 == x_memcmp(&ui4_four_cc , "00dc", 4) || 
                   0 == x_memcmp(&ui4_four_cc , "00db", 4)) 
                {
                    ui4_vid_frms ++;                          
                }          

                ui4_cur_offset += ui4_size;
                pui1_index_buf += ui4_size;
            }            
         }      
    }

    ui8_total_dur = (UINT64)ui4_vid_frms * pt_avi_hdlr_obj->ui4_frm_period * 90000;
    ui8_total_dur = _mm_div64(ui8_total_dur, 1000000, NULL);
    
    if(ui8_total_dur > MAX_SEC_TO_RECOVER_DUR*90000)
    {
        pt_avi_hdlr_obj->ui8_duration = ui8_total_dur + 45000 /*+ 0.5 sec*/;
    }

    return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_parse
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 i4_avi_minfo_chunk_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    BOOL            b_need_wait;
    setpos_fct      pf_set_pos;     
    getpos_fct      pf_get_pos;   
    copybytes_fct   pf_copybyte;
    input4bytes_fct pf_inp4byte;    
    UINT32          ui4_four_cc;
    UINT32          ui4_chk_sz;
    UINT32          ui4_read_size;
    INT32           i4_ret = MINFOR_INTERNAL_ERR;
    MINFO_AVI_RIFF_INF_T*  pt_avi_hdlr_obj;
    BOOL            b_no_std_multi_riff = FALSE;
    BOOL            b_last_riff_is_avix = TRUE;
    UINT32          ui4_last_riff_size = 0;
    UINT8           ui1_index = 0;
        
    DBG_INFO(("[avi_parser]i4_avi_minfo_chunk_parse \r\n"));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_input1 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input2 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input4 == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }

    b_need_wait = FALSE;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*seek to beginning*/
    pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_BGN);
    /*
     * check  -- "RIFF" & "AVI " 
     *
     */
    pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
    /*ui4_four_cc =_ui4_swap_4bytes(ui4_four_cc);*/
    if(0 != x_memcmp(&ui4_four_cc , "RIFF" ,4))
    {
        i4_ret = MINFOR_INTERNAL_ERR;
        if((x_memcmp((VOID*)(&ui4_four_cc), "DTSH", 4) == 0)/*for DTS support*/)
        {
            i4_ret = _i4_parse_dts_chunck(pt_minfo_obj, TRUE);
        }
        else if((ui4_four_cc == 0x0180FE7F) || 
                (ui4_four_cc == 0x00E8FF1F))  /*.cpt file*/
        {
            i4_ret = _i4_parse_dts_chunck(pt_minfo_obj, FALSE);
        }

        return i4_ret;
    }

    pf_inp4byte(pt_minfo_obj, &ui4_chk_sz);
    ui4_chk_sz = ui4_chk_sz;
    pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
    ui4_four_cc =_ui4_swap_4bytes(ui4_four_cc);
    if(0 != x_memcmp(&ui4_four_cc , "AVI " ,4) &&
        0 != x_memcmp(&ui4_four_cc , "WAVE" ,4)) /*add for wave file support */
    {
        /*not a valid AVI file*/ 
        return MINFOR_INTERNAL_ERR;    
    }
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_BGN);

    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;    
    pt_avi_hdlr_obj->e_parser_state = MINFO_AVI_PARSER_STATE_CHECK_CHUNK_HEADER;

    pf_copybyte(pt_minfo_obj, 
                pt_avi_hdlr_obj->pui1_header_buf, 
                pt_avi_hdlr_obj->ui4_buf_length, 
                8, 
                &ui4_read_size);
    pt_avi_hdlr_obj->ui4_cur_parser_offset+=8;
    do
    {
        i4_ret = _i4_avi_build_rtff_tree(pt_minfo_obj, &b_need_wait);
    }while(i4_ret == MINFO_AVI_PARSE_NEED_NEXT_LOOP && (!b_need_wait));

    /* parse video code info 'xxdb' */
    for (ui1_index=0; ui1_index<pt_avi_hdlr_obj->t_strm_inf.ui1_video_strm_num; ui1_index++)
    {
        i4_ret = _i4_set_video_codec_info(pt_minfo_obj, ui1_index+1);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: set video codec info fail. \r\n"));
        return i4_ret;
    }
    }

    /* parse audio info  */
    i4_ret = _i4_set_audio_pts_offset(pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: parse audio pts fail. \r\n"));
        return i4_ret;
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /* check bad interleaved */
    i4_ret = _i4_check_aud_vid_interleave(pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        pt_avi_hdlr_obj->b_is_bad_interleaved = TRUE;
    }  
    else
    {
        pt_avi_hdlr_obj->b_is_bad_interleaved = FALSE;
    }
#endif

    if (pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].e_enc == MINFO_INFO_AUD_ENC_AAC &&
        pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_sample_size == 1)	
    {		
        pt_avi_hdlr_obj->t_strm_inf.t_aud_strm_inf[0].ui4_sample_size = 0;
    }

    /* parse audio info  */
    i4_ret = _i4_set_idx_info(pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("ERROR: set index info fail. \r\n"));
        return i4_ret;
    }

    /*
         seek to next riff beginning 
         Mw doesn't get mutiriff info if the seek return value is busy 
      */
    i4_ret = pf_set_pos(pt_minfo_obj, (INT64)(ui4_chk_sz + 8), (MEDIA_SEEK_BGN | MEDIA_SEEK_CHK));
    if (i4_ret != INPUTR_OK)
    {
        pt_avi_hdlr_obj->b_non_seekable = TRUE;
    }
    /* check the other riff to cal movi size */
    while (i4_ret == INPUTR_OK)
    {
        pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
        if(0 != x_memcmp(&ui4_four_cc , "RIFF" ,4))
        {
            break;
        }
        pf_inp4byte(pt_minfo_obj, &ui4_chk_sz);
        pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
        if(0 == x_memcmp(&ui4_four_cc , "AVI " ,4))
        {
            b_no_std_multi_riff = TRUE;
            break;
        }
        
        if(0 == x_memcmp(&ui4_four_cc , "AVIX" ,4))
        {
            pt_avi_hdlr_obj->ui8_mov_sz += ui4_chk_sz;
            b_last_riff_is_avix = TRUE;
        }
        else
        {
            b_last_riff_is_avix = FALSE;
        }
        
        ui4_chk_sz += 8; /* add 8 for fourCC and size */
        ui4_chk_sz -= 12; /* minus 12 for read 3*4 byte */
        ui4_last_riff_size = ui4_chk_sz + 12;
        i4_ret = pf_set_pos(pt_minfo_obj, (INT64)ui4_chk_sz, MEDIA_SEEK_CUR);
    }
    /* seek to file end for get media size */
    if(!b_no_std_multi_riff)
    {
        i4_ret = pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_END);
        if (i4_ret != MINFOR_OK)
        {
            DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
            return i4_ret;
        }
        pf_get_pos(pt_minfo_obj, &(pt_avi_hdlr_obj->ui8_riff_size));
    }
    else
    {
        pf_get_pos(pt_minfo_obj, &(pt_avi_hdlr_obj->ui8_riff_size));
        if(!b_last_riff_is_avix)
        {
            if(pt_avi_hdlr_obj->ui8_riff_size > (UINT64)(ui4_last_riff_size + 12))
            {
                pt_avi_hdlr_obj->ui8_riff_size = 
                    (UINT64)(pt_avi_hdlr_obj->ui8_riff_size - 
                              (ui4_last_riff_size + 12));
            }
        }
        else
        {
            if(pt_avi_hdlr_obj->ui8_riff_size > 12)
            {
                pt_avi_hdlr_obj->ui8_riff_size -= 12;
            }
        }
    }
    
    /* error handling for file size invalid */
    if ((pt_avi_hdlr_obj->ui8_mov_sz >= pt_avi_hdlr_obj->ui8_riff_size) &&
        ((UINT64)pt_avi_hdlr_obj->ui4_mov_start_base <= pt_avi_hdlr_obj->ui8_riff_size))
    {
        pt_avi_hdlr_obj->ui8_mov_sz = 
            pt_avi_hdlr_obj->ui8_riff_size - pt_avi_hdlr_obj->ui4_mov_start_base;
    }

    /* error recover for movi size invalid */
    if (((pt_avi_hdlr_obj->ui8_mov_sz * 10) < pt_avi_hdlr_obj->ui8_riff_size) && 
        ((UINT64)pt_avi_hdlr_obj->ui4_mov_start_base <= pt_avi_hdlr_obj->ui8_riff_size))
    {
        pt_avi_hdlr_obj->ui8_mov_sz = 
            pt_avi_hdlr_obj->ui8_riff_size - pt_avi_hdlr_obj->ui4_mov_start_base;
    }

    /*error recover for total duration*/
    if(pt_avi_hdlr_obj->ui8_duration < MAX_SEC_TO_RECOVER_DUR*90000 /*duration is less than 2s*/ &&
       pt_avi_hdlr_obj->ui8_mov_sz < MAX_SIZE_TO_RECOVER_DUR /*movi size is less than 30M bytes*/)
    {
        _i4_cal_video_dur(pt_minfo_obj);
    }

    return MINFOR_OK;  
}
