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
 * $RCSfile: flv_parser.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the flv parser function
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
#include "flv_parser.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"
#include "svctx/svctx_dbg.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"
#include "../minfo.h"
#include "x_minfo.h"
#include "x_uc_str.h"
#include "flv_parser.h"
#include "u_dbg.h"
#include "../../mm_util.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define VID_STRM_ID        ((UINT8)9)   /* video */
#define AUD_STRM_ID        ((UINT8)8)   /* audio */
#define SCRIPT_STRM_ID     ((UINT8)18)  /* script */

#define SCRIPT_TYPE_IDX    ((UINT8)10)  /* this is index array */

#define VIDEO_BITMASK       0x01 /*FLV is big endian*/
#define AUDIO_BITMASK       0x04

#define FLV_HEADER_SIZE      9
#define FLV_TAG_HDR_SIZE    11

#define FLV_PARSE_TAG_CNT   5  /*parse frames of first 5 seconds*/
#define FLV_PARSE_IDX_CNT   20 /*parse last 20 frames to find index table*/

#define FLV_KEY_TBL_FOUND              (0xFFFFFFFF)

#define FLV_DATAOBJ_ENDCODE 0x000009
#define FLV_SAVE_DATA_BUF_LEN 1024

#define FLV_TIMES_STR       "times"
#define FLV_FILEPOS_STR     "filepositions"
#define FLV_VID_FRM_RATE    "framerate"
#define FLV_TOTAL_DURATION  "duration"
#define FLV_WIDTH           "width"
#define FLV_HEIGHT          "height"

static BOOL  g_b_has_times    = FALSE;
static BOOL  g_b_has_filepos  = FALSE;
static BOOL  g_b_has_frmrate  = FALSE;
static BOOL  g_b_has_duration = FALSE;
static BOOL  g_b_has_width    = FALSE;
static BOOL  g_b_has_height   = FALSE;

static CHAR* g_pac_save_buf   = NULL;
static UINT8 g_ui1_frame_rate = 0;
static UINT64 g_ui8_duration   = 0;
static UINT16 g_ui2_width     = 0;
static UINT16 g_ui2_height    = 0;


#define GET_UINT64_VALUE(pui1_buf, ui8_value)  \
    ui8_value = (UINT64)pui1_buf[0] << 56 |    \
                (UINT64)pui1_buf[1] << 48 |    \
                (UINT64)pui1_buf[2] << 40 |    \
                (UINT64)pui1_buf[3] << 32 |    \
                (UINT64)pui1_buf[4] << 24 |    \
                (UINT64)pui1_buf[5] << 16 |    \
                (UINT64)pui1_buf[6] << 8  |    \
                (UINT64)pui1_buf[7];           \
    pui1_buf += 8;                             \
    ui4_cur_pos += 8;

#define GET_UINT32_VALUE(pui1_buf, ui4_value)  \
    ui4_value = (UINT32)pui1_buf[0] << 24 |    \
                (UINT32)pui1_buf[1] << 16 |    \
                (UINT32)pui1_buf[2] << 8  |    \
                (UINT32)pui1_buf[3];           \
    pui1_buf += 4;                             \
    ui4_cur_pos += 4;

#define GET_UINT24_VALUE(pui1_buf, ui4_value)  \
    ui4_value = (UINT32)pui1_buf[0] << 16 |    \
                (UINT32)pui1_buf[1] << 8  |    \
                (UINT32)pui1_buf[2];           \
    pui1_buf += 3;                             \
    ui4_cur_pos += 3;

#define GET_UINT16_VALUE(pui1_buf, ui2_value) \
    ui2_value = (UINT16)pui1_buf[0] << 8 |    \
                (UINT16)pui1_buf[1];          \
    pui1_buf += 2;                            \
    ui4_cur_pos += 2;

#define GET_STR_VALUE(pui1_buf, psz_str)      \
{                                             \
   UINT16 ui2_strlen;                         \
   GET_UINT16_VALUE(pui1_buf, ui2_strlen);    \
   if(ui2_strlen < FLV_SAVE_DATA_BUF_LEN)     \
   {                                          \
      x_memcpy(psz_str, pui1_buf, ui2_strlen);   \
      psz_str[ui2_strlen]=0;                  \
   }                                          \
   else                                       \
   {                                          \
      x_memcpy(psz_str, pui1_buf, FLV_SAVE_DATA_BUF_LEN);   \
      psz_str[FLV_SAVE_DATA_BUF_LEN]=0;       \
   }                                          \
   pui1_buf += (ui2_strlen);                  \
   ui4_cur_pos += (ui2_strlen);               \
}

#define GET_LSTR_VALUE(pui1_buf, psz_lstr)    \
{                                             \
   UINT32 ui4_strlen;                         \
   GET_UINT32_VALUE(pui1_buf, ui4_strlen);    \
   if(ui4_strlen < FLV_SAVE_DATA_BUF_LEN)     \
   {                                          \
      x_memcpy(psz_lstr, pui1_buf, ui4_strlen);  \
      psz_lstr[ui4_strlen]=0;                 \
   }                                          \
   else                                       \
   {                                          \
      x_memcpy(psz_lstr, pui1_buf, FLV_SAVE_DATA_BUF_LEN);   \
      psz_lstr[FLV_SAVE_DATA_BUF_LEN]=0;      \
   }                                          \
   pui1_buf += (ui4_strlen);                  \
   ui4_cur_pos += (ui4_strlen);               \
}

#define FLV_ALLOC_BUF()                 \
    if(g_pac_save_buf == NULL)          \
    {                                   \
        g_pac_save_buf = (CHAR*)x_mem_alloc(FLV_SAVE_DATA_BUF_LEN+1); \
        if(g_pac_save_buf == NULL)      \
        {                               \
            return MINFOR_INTERNAL_ERR; \
        }                               \
    }

#define FLV_FREE_BUF()                  \
    if(g_pac_save_buf != NULL)          \
    {                                   \
        x_mem_free(g_pac_save_buf);     \
        g_pac_save_buf = NULL;          \
    }


typedef enum
{
   FRAME_TYPE_KEY_FRAME=1,
   FRAME_TYPE_INTER_FRAME,
   FRAME_TYPE_DISINTER_FRAME,
   FRAME_TYPE_GENER_KEY_FRAME,
   FRAME_TYPE_CMD_FRAME
}E_FLV_VIDEO_FRAME_TYPE;

typedef enum
{
  VIDEO_FMT_JPG=1,
  VIDEO_FMT_H263,
  VIDEO_FMT_SCREEN,
  VIDEO_FMT_VP6,
  VIDEO_FMT_VP6A,
  VIDEO_FMT_SCREEN2,
  VIDEO_FMT_AVC
}E_FLV_VIDEO_FMT;


typedef enum
{
    VALUE_NUM,
    VALUE_BOOL,
    VALUE_STR,
    VALUE_OBJ,
    VALUE_CLIP,
    VALUE_NULL,
    VALUE_UNDEF,
    VALUE_REFER,
    VALUE_ECMA,
    VALUE_STRICT=10,
    VALUE_DATE,
    VALUE_LSTR
}E_VALUE_TYPE;

const UINT32 aui4_bitrate[] =
{
    5512,
    11025,
    22050,
    44100
};


/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/


static MINFO_INFO_AUDIO_ENC_T _e_flv_aud_codec(UINT8 ui1_aud_fmt)
{
  MINFO_INFO_AUDIO_ENC_T e_ast_mode = MINFO_INFO_AUD_ENC_UNKNOWN;

  switch(ui1_aud_fmt)
  {
    case 0:
    case 3:
    case 8:
      e_ast_mode = MINFO_INFO_AUD_ENC_PCM;
      break;
    case 1:
      e_ast_mode = MINFO_INFO_AUD_ENC_ADPCM;
      break;
    case 2:
    case 14:
      e_ast_mode = MINFO_INFO_AUD_ENC_MPEG_1;
      break;
    case 17:
      e_ast_mode = MINFO_INFO_AUD_ENC_DVI_ADPCM;
      break;
    case 4:
    case 5:
    case 6:
    case 10:
      e_ast_mode = MINFO_INFO_AUD_ENC_AAC;
      break;    
    default:      
      break;
  }
  
  return(e_ast_mode);
}


static MINFO_INFO_VIDEO_ENC_T _e_flv_vid_codec(UINT8 ui1_vid_fmt)
{
  MINFO_INFO_VIDEO_ENC_T e_v_codec = MINFO_INFO_VID_ENC_UNKNOWN;

  switch(ui1_vid_fmt)
  {
    case 1:                     
      e_v_codec = MINFO_INFO_VID_ENC_MJPG;
      break;   
    case 2:
      /*e_v_codec = MINFO_INFO_VID_ENC_SORENSON;*/
      e_v_codec = MINFO_INFO_VID_ENC_MPEG_4; /*use mpeg4 codec*/
      break;    
    case 4:   /*support for VP6*/
      e_v_codec = MINFO_INFO_VID_ENC_VP6;
      break;
    case 7:
      e_v_codec = MINFO_INFO_VID_ENC_H264;
      break;
    default:
      break;
  }
  
  return(e_v_codec);
}

static INT32 _i4_flv_script_dataobjects(UINT8* pui1_buffer, 
                   BOOL b_has_endcode, UINT32 ui4_obj_count,
                   UINT32 ui4_data_size);
static INT32 _i4_flv_script_objectvalue(UINT8* pui1_buffer,
                                      UINT32 ui4_data_size);

#define DOUBLE_PRECISION                29 /*the count of get number after "." of double*/
#define DBL_SIG_BITS                    (52 - DOUBLE_PRECISION)
static INT32 _i4_flv_cvt_double_2_int64(UINT8* aui1_db_value, INT64* pi8_out_val, UINT32 ui4_scale)
{
    UINT64 ui8_tail;
    INT32 i4_e_val;
    BOOL  b_sign = 0;

    if(NULL == aui1_db_value || NULL == pi8_out_val)
    {
        return MINFOR_INTERNAL_ERR;
    }

    if((UINT64)ui4_scale >= ((UINT64)1 << (64 - DOUBLE_PRECISION)))
    {   /*will over flow*/
        *pi8_out_val = 0;
        return MINFOR_INTERNAL_ERR;
    }
    
    b_sign = (BOOL)(aui1_db_value[7] >> 7);

    i4_e_val = (INT32)(((((INT32)(aui1_db_value[7] & 0x7F)) << 4) & 0x7FF) | 
                        ((aui1_db_value[6] >> 4) & 0x0F));
    if(i4_e_val != 0x7FF)
    {
        i4_e_val -= (INT32)0x3FF;
        if((i4_e_val > 32) || (i4_e_val < (-5)))
        { /*will over flow*/
            *pi8_out_val = 0;
            return MINFOR_INTERNAL_ERR;
        }
    }
    else
    {
        if(b_sign)
        {   /* - INF*/
            *pi8_out_val = (INT64)0xFFFFFFFFFFFFFFFF;
            return MINFOR_OK;
        }
        else
        {
            /* + INF*/
            *pi8_out_val = (INT64)0x7FFFFFFFFFFFFFFF;
            return MINFOR_OK;
        }
    }

    ui8_tail = (UINT64)1 << ((52 - DBL_SIG_BITS));
    if(DBL_SIG_BITS <= 48)
    {
        ui8_tail |= (((UINT64)aui1_db_value[6] & 0x0F) << ((48 - DBL_SIG_BITS) & 0x3F));
        if((48 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[5] >> 
                ((8 - (48 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 40)
    {
        ui8_tail |= (((UINT64)aui1_db_value[5]) << ((40 - DBL_SIG_BITS) & 0x3F));
        if((40 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[4] >> 
                ((8 - (40 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 32)
    {
        ui8_tail |= ((UINT64)aui1_db_value[4] << ((32 - DBL_SIG_BITS) & 0x1F));
        if((32 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[3] >> 
                ((8 - (32 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 24)
    {
        ui8_tail |= ((UINT64)aui1_db_value[3] << ((24 - DBL_SIG_BITS) & 0x1F));
        if((24 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[2] >> 
                ((8 - (24 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 16)
    {
        ui8_tail |= ((UINT64)aui1_db_value[2] << ((16 - DBL_SIG_BITS) & 0x0F));
        if((16 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[1] >> 
                ((8 - (16 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 8)
    {
        ui8_tail |= ((UINT64)aui1_db_value[1] << ((8 - DBL_SIG_BITS) & 0x07));
        if((8 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[0] >> 
                ((8 - (8 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 0)
    {
        ui8_tail |= (UINT64)(aui1_db_value[0] & 0xFF);
    }

    if(i4_e_val >= 0)
    {
        ui8_tail <<= i4_e_val;
    }
    else
    {
        ui8_tail >>= (-i4_e_val);
    }
    ui8_tail *= ui4_scale;    
    ui8_tail += ((UINT64)1 << (DOUBLE_PRECISION - 1));
    ui8_tail >>= DOUBLE_PRECISION;
    if(b_sign)
    {
        *pi8_out_val = - (INT64)ui8_tail;
    }
    else
    {
        *pi8_out_val = (INT64)ui8_tail;
    }

    return MINFOR_OK;

}

static INT32 _i4_flv_script_objectvalue(UINT8* pui1_buffer, UINT32 ui4_data_size)
{
    UCHAR*  pui1_data = pui1_buffer;
    UINT8   ui1_val_type;
    UINT64  ui8_fd_value;
    UINT32  ui4_array_cnt=0;
    INT32   i4_process_size=0;
    UINT32  ui4_cur_pos = 0;

    ui1_val_type=*pui1_data++;
    ui4_cur_pos ++;

    if(ui1_val_type==VALUE_NUM)
    {
        
        GET_UINT64_VALUE(pui1_data,ui8_fd_value);
        _i4_flv_cvt_double_2_int64((UINT8*)&ui8_fd_value, (INT64*)&ui8_fd_value, 1);
        if(g_b_has_frmrate)
        {
            g_ui1_frame_rate = (UINT8)ui8_fd_value;
        }
        else if(g_b_has_duration)
        {
            g_ui8_duration = ui8_fd_value;
        }
        else if(g_b_has_width)
        {
            g_ui2_width = (UINT16)ui8_fd_value;
        }
        else if(g_b_has_height)
        {
            g_ui2_height = (UINT16)ui8_fd_value;
        }
    }
    else if(ui1_val_type==VALUE_BOOL)
    {
        pui1_data ++;
        ui4_cur_pos ++;
    }
    else if(ui1_val_type==VALUE_STR || ui1_val_type==VALUE_CLIP)
    {
        GET_STR_VALUE(pui1_data, g_pac_save_buf);
    }
    else if(ui1_val_type==VALUE_OBJ || ui1_val_type==VALUE_ECMA || ui1_val_type==VALUE_STRICT)
    {
         BOOL bHasEndCode=TRUE;
       
         if(ui1_val_type==VALUE_ECMA ||ui1_val_type==VALUE_STRICT )
         {
             GET_UINT32_VALUE(pui1_data, ui4_array_cnt);
         }

         if(ui1_val_type==VALUE_STRICT)
         {
             bHasEndCode=FALSE;
         }
   
         i4_process_size=_i4_flv_script_dataobjects(pui1_data, bHasEndCode,
                            ui4_array_cnt, ui4_data_size - ui4_cur_pos);
         if(i4_process_size == (INT32)FLV_KEY_TBL_FOUND)
         {
             return (INT32)FLV_KEY_TBL_FOUND;
         }
         
         if(i4_process_size > 0)
         {
            pui1_data += i4_process_size;
            ui4_cur_pos += i4_process_size;
         }
    }
    else if(ui1_val_type==VALUE_NULL)
    {
        pui1_data ++;
        ui4_cur_pos ++;
    }
    else if(ui1_val_type==VALUE_UNDEF)
    {
        /*ui4_cur_pos = (UINT32)-1;*/
    }
    else if(ui1_val_type==VALUE_REFER)
    {
        /*GET_UINT16_VALUE(pui1_data, ui2_value);*/
        pui1_data += sizeof(UINT16);
        ui4_cur_pos += sizeof(UINT16);
    }
    else if(ui1_val_type==VALUE_DATE)
    {
        /*GET_UINT64_VALUE(pui1_data,ui8_fd_value);
            pd8_value = (DOUBLE*)(&ui8_fd_value);

            d8_value = *pd8_value;
            GET_UINT16_VALUE(pui1_data, ui2_value1);
            ui2_value1 = ui2_value1;*/

        pui1_data += (sizeof(UINT64) + sizeof(UINT16));
        ui4_cur_pos += (sizeof(UINT64) + sizeof(UINT16));
    }
    else if(ui1_val_type==VALUE_LSTR)
    {
        GET_LSTR_VALUE(pui1_data, g_pac_save_buf);
    }    

    return (INT32)ui4_cur_pos;
}

static INT32 _i4_flv_script_dataobjects(UINT8*  pui1_buffer,BOOL b_has_endcode,
                                      UINT32 ui4_obj_cnt, UINT32 ui4_data_size )
{
   UINT32  ui4_obj_end=0;
   INT32   i4_process_size=0;
   UCHAR*  pui1_data = pui1_buffer;
   UINT32  ui4_cur_pos = 0;
   
   if(b_has_endcode)
   {
       GET_UINT24_VALUE(pui1_data, ui4_obj_end);
       pui1_data -= 3; /*restore pointer*/
       ui4_cur_pos -= 3;
   }
   if(!b_has_endcode && !ui4_obj_cnt)
   {
       return 0;
   }
   while((i4_process_size >= 0) && 
         (ui4_obj_end != FLV_DATAOBJ_ENDCODE) && 
         (ui4_cur_pos < ui4_data_size))
   {
       if(b_has_endcode)
       {
           GET_STR_VALUE(pui1_data, g_pac_save_buf);
           if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_TIMES_STR))
           {
                g_b_has_times = TRUE;
                if((g_ui1_frame_rate != 0) && (g_ui8_duration != 0))
                {
                    return (INT32)FLV_KEY_TBL_FOUND;
                }
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_FILEPOS_STR))
           {
                g_b_has_filepos= TRUE;
                if((g_ui1_frame_rate != 0) && (g_ui8_duration != 0))
                {
                    return (INT32)FLV_KEY_TBL_FOUND;
                }
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_VID_FRM_RATE))
           {
                g_b_has_frmrate = TRUE;                
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_TOTAL_DURATION))
           {
                g_b_has_duration = TRUE;                
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_WIDTH))
           {
                g_b_has_width = TRUE;
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_HEIGHT))
           {
                g_b_has_height = TRUE;
           }
       }
       i4_process_size=_i4_flv_script_objectvalue(pui1_data, ui4_data_size - ui4_cur_pos);
       g_b_has_frmrate = FALSE;
       g_b_has_duration = FALSE;
       g_b_has_width    = FALSE;
       g_b_has_height   = FALSE;
       if((i4_process_size == (INT32)FLV_KEY_TBL_FOUND) ||
          ((g_b_has_filepos || g_b_has_times) && 
           (g_ui8_duration   != 0) && 
           (g_ui1_frame_rate != 0)
          )
         )                
       {
           return (INT32)FLV_KEY_TBL_FOUND;
       }
       
       if(i4_process_size > 0)
       {
           pui1_data += i4_process_size;
           ui4_cur_pos += i4_process_size;
       }
       
       if(b_has_endcode)
       {
           GET_UINT24_VALUE(pui1_data, ui4_obj_end);
           pui1_data -= 3; /*restore pointer*/
           ui4_cur_pos -= 3;
       }
       else 
       {
          ui4_obj_cnt--;
          if(ui4_obj_cnt==0)
          {
             break;
          }
       }
   }   
   
   if(b_has_endcode)
   {
       GET_UINT24_VALUE(pui1_data, ui4_obj_end);
   }
   return (INT32)ui4_cur_pos;
}


static INT32 _i4_flv_script_tag_parse(UINT8* pui1_buffer, UINT32 ui4_len)
{
    UINT32  ui4_readpos  = 0;
    UINT32  ui4_cur_parse_size = 0;
    
    while(ui4_readpos < ui4_len)
    {
       ui4_cur_parse_size = (UINT32)_i4_flv_script_objectvalue(pui1_buffer, ui4_len - ui4_readpos);
       if(ui4_cur_parse_size == (UINT32)(FLV_KEY_TBL_FOUND))
       {
           break;
       }
       ui4_readpos += ui4_cur_parse_size;
       pui1_buffer += ui4_cur_parse_size;       
    }

    if(g_b_has_times || g_b_has_filepos)
    {
        g_b_has_times = FALSE;
        g_b_has_filepos = FALSE;
        
        return MINFOR_OK;
    }

    return MINFOR_INTERNAL_ERR;
}

static INT32 _i4_flv_audio_tag_parse(UINT8*  pui1_buffer, MINFO_FLV_INF_T*  pt_flv_hdlr_obj, UINT32 ui4_len)
{
   UINT8 ui1_data=0;

   pt_flv_hdlr_obj->t_strm_inf.ui1_audio_strm_num = 1;

   ui1_data = pui1_buffer[0];
   pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.e_enc = _e_flv_aud_codec(ui1_data >> 4);
   if(MINFO_INFO_AUD_ENC_MPEG_1 == pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.e_enc)
   {
       pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.e_layer = 3; /*FLV only has mp3 audio*/
   }

   pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.i4_samples_per_sec = (INT32)aui4_bitrate[(ui1_data & 0x0c) >> 2];
   pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.i2_bits_per_sample = ((ui1_data & 0x02) >> 1) ? 16 : 8;
   pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.i2_channels = (ui1_data & 0x01) ? 2 : 1;
   
   return MINFOR_OK;
}

static INT32 _i4_flv_video_tag_parse(UINT8 *  pui1_buffer, MINFO_FLV_INF_T* pt_flv_hdlr_obj, UINT32 ui4_len)
{
    E_FLV_VIDEO_FRAME_TYPE e_frame_type;
    E_FLV_VIDEO_FMT e_vid_fmt;

    e_frame_type = (E_FLV_VIDEO_FRAME_TYPE)(pui1_buffer[0] >> 4);
    e_vid_fmt = (E_FLV_VIDEO_FMT)(pui1_buffer[0] & 0x0F);
    if(e_frame_type<FRAME_TYPE_KEY_FRAME || e_frame_type>FRAME_TYPE_CMD_FRAME)
    {
       DBG_ERROR(("Video frame type error:%d \r\n",e_frame_type));
        return MINFOR_INTERNAL_ERR;
    }

    if(e_vid_fmt<VIDEO_FMT_JPG || e_vid_fmt> VIDEO_FMT_AVC)
    {
       DBG_ERROR(("Video Codec  type error:%d \r\n",e_vid_fmt));
        return MINFOR_INTERNAL_ERR;
    }

    DBG_INFO(("Video:FrameType=%d,CodecType=%d \r\n",e_frame_type,e_vid_fmt));

    pt_flv_hdlr_obj->t_strm_inf.ui1_video_strm_num = 1;
    pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.e_enc = _e_flv_vid_codec(e_vid_fmt);
    
   return MINFOR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  flv_minfo_parse
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
INT32 i4_flv_minfo_tag_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    setpos_fct      pf_set_pos;     
    getpos_fct      pf_get_pos;   
    copybytes_fct   pf_copybyte;
    /*input4bytes_fct pf_inp4byte;    */
    UINT32          ui4_read_size;
    INT32           i4_ret = MINFOR_OK;
    MINFO_FLV_INF_T*  pt_flv_hdlr_obj;
    /*UINT8           ui1_version;
    UINT8           ui1_vid_aud_flag;*/
    UINT32          ui4_data_offset;
    UINT32          ui4_previous_len;
    UINT32          ui4_data_len = 0;
    UINT32          ui4_cur_pos = 0;

    UINT8           ui1_tag_type;
    UINT32          ui4_time_stamp;
    UINT8           ui1_flag;
    /*UINT32          ui4_stream_id;*/

    BOOL             b_vid_tag_parsed = FALSE;
    BOOL             b_aud_tag_parsed = FALSE;
    BOOL             b_script_tag_parsed = FALSE;
    UINT32           ui4_file_size;

    UINT32           ui4_aud_frm_cnt = 0;
    UINT32           ui4_vid_frm_cnt = 0;
    UINT32           ui4_aud_bytes   = 0;

    UINT32           ui4_cnt = 0;
        
    DBG_INFO(("[flv_parser]i4_flv_minfo_tag_parse \r\n"));
    
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

    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    /*pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;*/
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*seek to beginning*/
    pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_BGN);
    /*
         * check  -- "FLV " 
         *
         */
    pt_flv_hdlr_obj = (MINFO_FLV_INF_T*)pt_minfo_obj->pv_handler_obj;
    pf_copybyte(pt_minfo_obj, 
                pt_flv_hdlr_obj->pui1_header_buf, 
                pt_flv_hdlr_obj->ui4_buf_length, 
                FLV_HEADER_SIZE, 
                &ui4_read_size);
    
    if(0 != x_memcmp(pt_flv_hdlr_obj->pui1_header_buf , "FLV" ,3))
    {
        /*not a valid FLV file*/
        return MINFOR_INTERNAL_ERR;    
    }

    FLV_ALLOC_BUF();
    
    /* get file size */
    pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_END);
    pf_get_pos(pt_minfo_obj, (UINT64*)&ui4_file_size);
    pf_set_pos(pt_minfo_obj , 0, MEDIA_SEEK_BGN);
    
    #if 0
    ui1_version = *(pt_flv_hdlr_obj->pui1_header_buf + 3);
    ui1_vid_aud_flag = *(pt_flv_hdlr_obj->pui1_header_buf + 4);
    
    if(ui1_vid_aud_flag & VIDEO_BITMASK)
    {
        pt_flv_hdlr_obj->b_has_video = TRUE;
    }
    else
    {
        b_vid_tag_parsed = TRUE;
    }

    if(ui1_vid_aud_flag & AUDIO_BITMASK)
    {
        pt_flv_hdlr_obj->b_has_audio= TRUE;
    }
    else
    {
        b_aud_tag_parsed = TRUE;
    }
    #else
    pt_flv_hdlr_obj->b_has_video = FALSE;
    pt_flv_hdlr_obj->b_has_audio = FALSE;
    #endif
    
    ui4_data_offset = ((UINT32)pt_flv_hdlr_obj->pui1_header_buf[5] << 24) |
                      ((UINT32)pt_flv_hdlr_obj->pui1_header_buf[6] << 16) | 
                      ((UINT32)pt_flv_hdlr_obj->pui1_header_buf[7] << 8)  |
                       (UINT32)pt_flv_hdlr_obj->pui1_header_buf[8];

    pt_flv_hdlr_obj->ui4_mov_start_base = ui4_data_offset;
    pt_flv_hdlr_obj->ui8_mov_sz = ui4_file_size - ui4_data_offset;
    
    pf_set_pos(pt_minfo_obj , (INT64)ui4_data_offset, MEDIA_SEEK_BGN);
    pt_flv_hdlr_obj->ui4_cur_parser_offset = ui4_data_offset;
    ui4_cur_pos = 0;

    pf_copybyte(pt_minfo_obj, 
                pt_flv_hdlr_obj->pui1_header_buf, 
                pt_flv_hdlr_obj->ui4_buf_length, 
                FLV_BUF_LENGTH, 
                &ui4_read_size);
    
    while((ui4_cur_pos < ui4_read_size))
    {
        ui4_previous_len = (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 24 |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos+1] << 16 |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos+2] << 8 |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos+3];
        ui4_cur_pos += 4;
        if(ui4_previous_len && ui4_previous_len != ui4_data_len + FLV_TAG_HDR_SIZE)
        {
           DBG_ERROR(("Warning,Previous Tag len is not correct \r\n"));
        }        
           
       if(ui4_cur_pos + FLV_TAG_HDR_SIZE > ui4_read_size)
       {
            pt_flv_hdlr_obj->ui4_cur_parser_offset += ui4_cur_pos;
            if(pt_flv_hdlr_obj->ui4_cur_parser_offset > ui4_file_size)
            {
                FLV_FREE_BUF();
                return MINFOR_INTERNAL_ERR;
            }
            else if(pt_flv_hdlr_obj->ui4_cur_parser_offset == ui4_file_size)
            {
                break; /*meet the end of file*/
            }
            
            pf_set_pos(pt_minfo_obj , (INT64)(pt_flv_hdlr_obj->ui4_cur_parser_offset), MEDIA_SEEK_BGN);
            pf_copybyte(pt_minfo_obj, 
                        pt_flv_hdlr_obj->pui1_header_buf, 
                        pt_flv_hdlr_obj->ui4_buf_length, 
                        FLV_BUF_LENGTH, 
                        &ui4_read_size);
            ui4_cur_pos = 0;
       }

       ui1_tag_type=pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos];
       ui4_cur_pos += 1;
       ui4_data_len = (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                      (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                      (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;
       ui4_cur_pos += 3;
       ui4_time_stamp=(UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                      (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                      (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;
       ui4_cur_pos += 3;
       ui1_flag=pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos];       
       ui4_time_stamp=(UINT32)ui1_flag<<24 |ui4_time_stamp;       
       ui4_cur_pos += 1;
#if 0
       ui4_stream_id=(UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                     (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                     (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;
#endif
       ui4_cur_pos += 3;

       if(ui4_cur_pos + ui4_data_len > ui4_read_size)
       {
            pt_flv_hdlr_obj->ui4_cur_parser_offset += ui4_cur_pos;
            if(pt_flv_hdlr_obj->ui4_cur_parser_offset >= ui4_file_size)
            {
                FLV_FREE_BUF();
                return MINFOR_INTERNAL_ERR;
            }
            pf_set_pos(pt_minfo_obj , (INT64)(pt_flv_hdlr_obj->ui4_cur_parser_offset), MEDIA_SEEK_BGN);
            pf_copybyte(pt_minfo_obj, 
                        pt_flv_hdlr_obj->pui1_header_buf, 
                        pt_flv_hdlr_obj->ui4_buf_length, 
                        FLV_BUF_LENGTH, 
                        &ui4_read_size);
            ui4_cur_pos = 0;
       }
#if 0
       DBG_INFO(("NewTag:Type=%s,Stream=%d,Lenght=%d,Time=%d \r\n",
               ui1_tag_type == 8 ? "Audio" : 
               (ui1_tag_type == 9 ? "Video" : "ScriptData"), 
                ui4_stream_id, ui4_data_len, ui4_time_stamp));
#endif
       if(ui1_tag_type == AUD_STRM_ID)
       {
          if(!b_aud_tag_parsed)
          {   /*skip this tag*/
              i4_ret = _i4_flv_audio_tag_parse(&pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] , 
                                               pt_flv_hdlr_obj, ui4_data_len);
              if(MINFOR_OK == i4_ret)
              {
                  b_aud_tag_parsed = TRUE;
                  pt_flv_hdlr_obj->b_has_audio = TRUE;
                  pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui4_start_pts = ui4_time_stamp * 90;
              }  
          }          
          ui4_aud_frm_cnt ++;
          ui4_aud_bytes += ui4_data_len;
       }
       else if(ui1_tag_type == VID_STRM_ID)
       {          
          if(!b_vid_tag_parsed)
          {
            i4_ret = _i4_flv_video_tag_parse(&pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] , 
                                               pt_flv_hdlr_obj, ui4_data_len);
            if(MINFOR_OK == i4_ret)
            {
                b_vid_tag_parsed = TRUE;
                pt_flv_hdlr_obj->b_has_video = TRUE;
                pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui4_start_pts = ui4_time_stamp * 90;
            }
          }          
          ui4_vid_frm_cnt ++;
       }
       else if(ui1_tag_type == SCRIPT_STRM_ID)
       {
          ui4_time_stamp = 0; /*the time stamp of script should be ignore*/
          if(!b_script_tag_parsed)
          {
        #if 0
               ASSERT((ui4_data_len + ui4_cur_pos) <= FLV_BUF_LENGTH, 
                ("%s(), FLV Buffer overflow!!! \r\n",__func__));
        #endif
              if((ui4_data_len + ui4_cur_pos) > FLV_BUF_LENGTH)
              {
                  FLV_FREE_BUF();
                  return MINFOR_INTERNAL_ERR;
              }

              i4_ret = _i4_flv_script_tag_parse(&pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos], ui4_data_len);
              if(MINFOR_OK == i4_ret)
              {
                  pt_flv_hdlr_obj->ui4_indxtbl_offset = 
                    pt_flv_hdlr_obj->ui4_cur_parser_offset + ui4_cur_pos;                      
                  pt_flv_hdlr_obj->ui4_indxtbl_sz = ui4_data_len;                  
              }
              b_script_tag_parsed = TRUE; /*key table must be the first script tag*/
          }
       }
       else 
       {            
            DBG_ERROR(("Unknown FLV tag format, not support \r\n"));
       }
       ui4_cur_pos += ui4_data_len;
       
       /*all needed tag have been parsed*/
       /*if(b_vid_tag_parsed && b_aud_tag_parsed)*/
       {
           if(ui4_time_stamp >= FLV_PARSE_TAG_CNT * 1000)
           {
               if((ui4_vid_frm_cnt > FLV_PARSE_TAG_CNT) || (ui4_aud_frm_cnt > FLV_PARSE_TAG_CNT))
               {   /*for correct error time code*/
                   break;
               }
           }
       }
    }

    pt_flv_hdlr_obj->ui1_cur_strm_num = pt_flv_hdlr_obj->t_strm_inf.ui1_audio_strm_num +
                                        pt_flv_hdlr_obj->t_strm_inf.ui1_video_strm_num;

    if(g_ui1_frame_rate != 0)
    {
        pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui4_rate = g_ui1_frame_rate;
        g_ui1_frame_rate = 0;
    }
    else
    {
        pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui4_rate = 
                                     ui4_vid_frm_cnt / FLV_PARSE_TAG_CNT;
    }
    pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui4_scale = 1;
    pt_flv_hdlr_obj->ui4_video_strm_id = 1; 
    
    /*pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.i4_samples_per_sec = 
                                     ui4_aud_frm_cnt / FLV_PARSE_TAG_CNT;*/
    pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui4_avg_bytes_per_sec = 
                                     ui4_aud_bytes / FLV_PARSE_TAG_CNT;

    /*get duration and index table*/
    if(0 != g_ui8_duration)
    {
        pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui8_duration = g_ui8_duration * 90000;
        pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui8_duration = g_ui8_duration * 90000;
        g_ui8_duration = 0;
    }
    else
    {
        b_aud_tag_parsed = FALSE;
        b_vid_tag_parsed = FALSE;
        pt_flv_hdlr_obj->ui4_cur_parser_offset = ui4_file_size - 4;
        pf_set_pos(pt_minfo_obj , (INT64)pt_flv_hdlr_obj->ui4_cur_parser_offset, MEDIA_SEEK_BGN);
        pf_copybyte(pt_minfo_obj, 
                    pt_flv_hdlr_obj->pui1_header_buf, 
                    pt_flv_hdlr_obj->ui4_buf_length, 
                    4, 
                    &ui4_read_size); 

        ui4_previous_len = (UINT32)pt_flv_hdlr_obj->pui1_header_buf[0] << 24 |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[1] << 16 |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[2] << 8  |
                           (UINT32)pt_flv_hdlr_obj->pui1_header_buf[3];

        if(ui4_previous_len >= ui4_file_size) /*file error*/
        {
            pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui8_duration = 0;              
            pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui8_duration = 0;
        }
        else
        {
            ui4_cnt = FLV_PARSE_IDX_CNT;
            while(ui4_cnt -- > 0)
            {
                pt_flv_hdlr_obj->ui4_cur_parser_offset -= (ui4_previous_len + 4);/*get prev tag size*/
                pf_set_pos(pt_minfo_obj , (INT64)(pt_flv_hdlr_obj->ui4_cur_parser_offset), MEDIA_SEEK_BGN);
                pf_copybyte(pt_minfo_obj, 
                        pt_flv_hdlr_obj->pui1_header_buf, 
                        pt_flv_hdlr_obj->ui4_buf_length, 
                        FLV_TAG_HDR_SIZE + 4, 
                        &ui4_read_size);


               ui4_previous_len = (UINT32)pt_flv_hdlr_obj->pui1_header_buf[0] << 24 |
                                   (UINT32)pt_flv_hdlr_obj->pui1_header_buf[1] << 16 |
                                   (UINT32)pt_flv_hdlr_obj->pui1_header_buf[2] << 8  |
                                   (UINT32)pt_flv_hdlr_obj->pui1_header_buf[3];

               ui4_cur_pos = 4;
               ui1_tag_type=pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos];
               ui4_cur_pos += 1;
               ui4_data_len = (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                              (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                              (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;
               ui4_cur_pos += 3;
               ui4_time_stamp=(UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                              (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                              (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;
               ui4_cur_pos += 3;
               ui1_flag=pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos];       
               ui4_time_stamp=(UINT32)ui1_flag<<24 |ui4_time_stamp;       
               ui4_cur_pos += 1;
               
               /*ui4_stream_id=(UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos] << 16 |
                             (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 1] << 8 |
                             (UINT32)pt_flv_hdlr_obj->pui1_header_buf[ui4_cur_pos + 2] ;*/
               ui4_cur_pos += 3;       
                       
               if(ui1_tag_type == AUD_STRM_ID)
               {
                  if(!b_aud_tag_parsed)
                  { 
                      b_aud_tag_parsed = TRUE;
                      pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui8_duration = (UINT64)ui4_time_stamp * 90;
                  }
               }
               else if(ui1_tag_type == VID_STRM_ID)
               {          
                  if(!b_vid_tag_parsed)
                  {
                     b_vid_tag_parsed = TRUE;
                     pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui8_duration = (UINT64)ui4_time_stamp * 90;
                  }
               }

               /*all needed tag have been parsed*/
               if(b_vid_tag_parsed && b_aud_tag_parsed)
               {
                   break;
               }
            }   
        }
    }
	
    if(pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui8_duration > 
         pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui8_duration)
    {
        pt_flv_hdlr_obj->ui8_duration = 
            pt_flv_hdlr_obj->t_strm_inf.t_aud_strm_inf.ui8_duration;
    }
    else
    {
        pt_flv_hdlr_obj->ui8_duration = 
            pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.ui8_duration;
    }

    pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.i4_width  = (INT32)g_ui2_width;
    pt_flv_hdlr_obj->t_strm_inf.t_vid_strm_inf.i4_height = (INT32)g_ui2_height;

    g_ui2_width  = 0;
    g_ui2_height = 0;

    FLV_FREE_BUF();
 
    return MINFOR_OK;  
}
