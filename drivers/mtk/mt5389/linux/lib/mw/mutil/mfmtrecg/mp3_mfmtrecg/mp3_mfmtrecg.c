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
 * $RCSfile: mp3_mfmtrecg.c,v $
 * $Revision: #4 $ 1.0
 * $Date: 2012/08/03 $ 2004/11/26
 * $Author: cong.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description:
 *         This file contains all the media format recognizer public APIs
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

#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"
#include "mutil/mm_util_input.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"
#include "x_mm_common.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MFMTRRECG_MP3_MAX_PARSING_LEN                       (1024*200)

#define IS_MP3_HEADER(ac_arry)         \
    ((ac_arry[0] == 0xFF)          &&  \
     ((ac_arry[1] & 0xE0) == 0xE0) &&  \
     ((ac_arry[2] >> 4) != 0)      &&  \
     ((ac_arry[2] >> 4) != 15)         \
    )

#define MP3_GET_BITRATE(ui4_bitrate, ui1_version, ui1_layer, ui1_rate_index)   \
{                                                                              \
    switch (ui1_version)                                                       \
    {                                                                          \
        case 3:  /* Version 1 */  /* kbps */                                      \
            ui4_bitrate = MPEG_V1_BITRATE_TAB[ui1_layer - 1][ui1_rate_index];  \
            break;                                                             \
        case 2:  /* Version 2 */                                                 \
        case 0:  /* Version 2.5 */  /* kbps */                                    \
            ui4_bitrate = MPEG_V2_BITRATE_TAB[(ui1_layer - 1) >> 1][ui1_rate_index];  \
            break;                                                             \
        default:                                                               \
            ui4_bitrate = 0;                                                   \
            break;                                                             \
    }                                                                          \
    ui4_bitrate *= 1000;    /* bps */                                           \
}

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*const static CHAR auc_TAG[] = "TAG";*/
const static CHAR auc_ID3[] = "ID3";

/* VERSION_1 */
static UINT16 const MPEG_V1_BITRATE_TAB[3][16] =
{
    {0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0}, /* Layer III */
    {0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0}, /* Layer II */
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0}  /* Layer I */
};

/* VERSION_2, VERSION_2_5 */
static UINT16 const MPEG_V2_BITRATE_TAB[2][16] =
{
    {0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160, 0},    /* Layer II & III */
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0}     /* Layer I */
};

static UINT16 const MPEG_SAMPLERATE_TAB[3] = {44100, 48000, 32000};
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  mp3_read_n_byte_to_buf
 *
 * Description: this function will get n byte from handle
 *
 * Inputs:  h_file         specify the handle of the file
 *          i8_offset      start position
 *          ui2_read_size  read size
 *          b_is_mem       false:file  true:mem
 *
 * Outputs: pv_buff        output data
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
static INT32 mp3_read_n_byte_to_buf(
                                    HANDLE_T        h_file,
                                    const VOID*     pv_mem_data,
                                    SIZE_T          z_mem_leng,
                                    INT64           i8_offset,
                                    VOID*           pv_output_buff,
                                    UINT32          ui2_read_size,
                                    UINT32*         pui4_read_succ_size,
                                    BOOL            b_is_mem)
{
    INT32           i4_ret;
    UINT64          ui8_pos = 0;
    MEDIA_FEEDER_T* pt_src_feeder = NULL;

    if(b_is_mem)
    {
        if (pv_mem_data == NULL)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        if (z_mem_leng == 0)
        {
            pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
            i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                    &pt_src_feeder->h_feeder,
                                    i8_offset,
                                    MEDIA_SEEK_BGN);
            if (i4_ret != INPUTR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                    &pt_src_feeder->h_feeder,
                                    pv_output_buff,
                                    ui2_read_size,
                                    ui2_read_size,
                                    pui4_read_succ_size);
        }
        else
        {
            x_memcpy(pv_output_buff, pv_mem_data, ui2_read_size);
        }
    }
#ifndef __NO_FM__
    else
    {
        i4_ret = x_fm_lseek(h_file, i8_offset, FM_SEEK_BGN, &ui8_pos);
        if (i4_ret != FMR_OK)
        {
             return MFMTRECGR_FILE_ERR;
        }
        i4_ret = x_fm_read( h_file,
                            pv_output_buff,
                            ui2_read_size,
                            pui4_read_succ_size);
        if (i4_ret != FMR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
    }
#else
    UNUSED(ui8_pos);
#endif
    return MFMTRECGR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp3_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
static INT32 mp3_check_format(HANDLE_T            h_file,
                              const VOID*         pv_mem_data,
                              SIZE_T              z_mem_leng,
                              MEDIA_FORMAT_T*     pt_format,
                              BOOL                b_is_mem)
{
    UCHAR     ac_tag[3];

    UCHAR     *ac_buff;
    UCHAR     ac_tmp[10];
    UINT32    ui4_actual_read;
    UINT32    ui4_idx;
    UINT32    ui4_frame_length;
    UINT8     ui1_rate_index;
    UINT32    ui4_bitrate = 0;
    UINT32    ui4_sample_rate = 0;
    UINT8     ui1_slot_bytes = 0;
    UINT32    ui4_frame_slots = 0;
    UINT8     ui1_padding = 0;

    UINT8     ui1_layer, ui1_version;
    UINT32    ui4_read;

    UINT32    ui4_id3v2_offset = 0;
    INT32     i4_ret = 0;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || ((h_file == NULL_HANDLE) && !b_is_mem)
        || ((pv_mem_data == NULL) && b_is_mem))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    /* check ID3v2 tag is exist or NOT */

    do
    {
        UINT32 ui4_id3_tag_offset;
        if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                                  pv_mem_data,
                                  z_mem_leng,
                                  ui4_id3v2_offset,
                                  ac_tmp,
                                  10,
                                  &ui4_read,
                                  b_is_mem)) != MFMTRECGR_OK)
        {
            return i4_ret;
        }
        if (x_memcmp((VOID*)auc_ID3, ac_tmp, 3) == 0)
        {
                 /* calculate ID3v2 tag offset */
            ui4_id3_tag_offset = (((UINT32)(ac_tmp[6])) << 21)
                                 | (((UINT32)(ac_tmp[7])) << 14)
                                 | (((UINT32)(ac_tmp[8])) << 7)
                                 | ((UINT32)(ac_tmp[9]));
                 
            ui4_id3v2_offset += ui4_id3_tag_offset;
            ui4_id3v2_offset += 10; /*header of id3 tag v2*/
        }
        else
        {
            break;
        }
    }while(1);

    /* check MP3 frame header */
    /*
     *  syncword             11 bits
     *  version              2 bits
     *  layer                2 bits
     *
     */

    ac_buff = (UCHAR*)x_mem_alloc(sizeof(UCHAR) * MFMTRRECG_MP3_MAX_PARSING_LEN);
    if (ac_buff == NULL)
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                              pv_mem_data,
                              z_mem_leng,
                              (INT64)ui4_id3v2_offset,
                              ac_buff,
                              MFMTRRECG_MP3_MAX_PARSING_LEN,
                              &ui4_read,
                              b_is_mem)) != MFMTRECGR_OK)
    {
        if (ac_buff != NULL)
        {
            x_mem_free(ac_buff);
        }
        return i4_ret;
    }

    ui4_actual_read = ui4_read;
    ui4_idx = 0;
    /* find a possible audio frame header */
    while (ui4_idx < ui4_actual_read)
    {
        if ((ac_buff[ui4_idx] != 0xFF)
            || ((ac_buff[ui4_idx + 1] & 0xE0) != 0xE0)
            || ((ac_buff[ui4_idx + 1] & 0x18) >> 3 == 1)      /* version description is set to reserved */
            || ((ac_buff[ui4_idx + 1] & 0x06) >> 1 == 0)      /* layer description is set to reserved */
            || ((ac_buff[ui4_idx + 2] >> 4) == 0)
            || ((ac_buff[ui4_idx + 2] >> 4) == 15))
        {
            ui4_idx++;
        }
        else
        {
            x_memcpy(ac_tag, ac_buff + ui4_idx, 3);
            ui1_version = ((ac_tag[1] & 0x18) >> 3);
            ui1_layer = ((ac_tag[1] & 0x6) >> 1);

            /* bitrate index */
            ui1_rate_index = ac_tag[2] >> 4;
            MP3_GET_BITRATE(ui4_bitrate, ui1_version, ui1_layer, ui1_rate_index);            

            /* sampling frequency index */
            ui1_rate_index = ((ac_tag[2] & 0xc) >> 2);
            ui4_sample_rate = 0;
            if (ui1_rate_index != 3)    /* not reserved */
            {
                ui4_sample_rate = MPEG_SAMPLERATE_TAB[ui1_rate_index];
            }
            switch (ui1_version)
            {
                case 2: /* Version 2 */
                    ui4_sample_rate >>= 1;
                    break;
                case 0: /* Version 2.5 */
                    ui4_sample_rate >>= 2;
                    break;
            }
            if (ui4_sample_rate == 0)
            {
                /* read next 1 byte */
                ui4_idx++;
                continue;
            }
            /* padding bit */
            ui1_padding = ((ac_tag[2] & 0x2)>>1);

            /* calculate frame length */
            ui4_frame_length = 0;
            if (ui4_bitrate != 0)
            {
                ui1_slot_bytes = 1;
                if (ui1_layer == 3)         /* Layer 1 */
                {
                    ui4_frame_slots = 12;
                    ui1_slot_bytes = 4;
                }
                else if ((ui1_layer == 1)   /* Layer 3 */
                    /* Version 2 or Version 2.5 */
                    && (ui1_version == 2 || ui1_version == 0))
                {
                    ui4_frame_slots = 72;
                }
                else
                {
                    ui4_frame_slots = 144;
                }
                if (ui4_sample_rate != 0)
                {
                    ui4_frame_length = (ui4_frame_slots * ui4_bitrate / ui4_sample_rate + ui1_padding) * ui1_slot_bytes;
                }
            }
            
            /* jump to check next frame header file is valid or invalid */
            if (ui4_idx + ui4_frame_length < ui4_actual_read)
            {
                x_memcpy(ac_tmp, (ac_buff + ui4_idx + ui4_frame_length), 3);
            }
            else
            {
                if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                                          pv_mem_data,
                                          z_mem_leng,
                                          (INT64)(ui4_id3v2_offset + ui4_idx + ui4_frame_length),
                                          ac_tmp,
                                          3,
                                          &ui4_read,
                                          b_is_mem)) != MFMTRECGR_OK)
                {
                    if (ac_buff != NULL)
                    {
                        x_mem_free(ac_buff);
                    }
                    return i4_ret;
                }
            }
            
            /* next frame header is also valid */
            if (!IS_MP3_HEADER(ac_tmp))
            {
                ui4_idx++;
                
                if (ui4_idx + ui4_frame_length < ui4_actual_read)
                {
                    x_memcpy(ac_tmp, (ac_buff + ui4_idx + ui4_frame_length), 3);
                }
                else
                {
                    if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                                              pv_mem_data,
                                              z_mem_leng,
                                              (INT64)(ui4_id3v2_offset + ui4_idx + ui4_frame_length),
                                              ac_tmp,
                                              3,
                                              &ui4_read,
                                              b_is_mem)) != MFMTRECGR_OK)
                    {
                        if (ac_buff != NULL)
                        {
                            x_mem_free(ac_buff);
                        }
                        return i4_ret;
                    }
                }
            }
            if (IS_MP3_HEADER(ac_tmp))
            {
                /*read one more frame to avoid error recgnize*/
                if (ui4_idx + ui4_frame_length < ui4_actual_read)
                {
                    ui4_idx += ui4_frame_length;
                }
                else /*already read for the 2nd frame*/
                {
                    ui4_id3v2_offset += ui4_idx;
                    ui4_idx = 0;
                    ui4_actual_read = ui4_read;
                }

                /* bitrate index */
                ui1_rate_index = ac_tmp[2] >> 4;

                MP3_GET_BITRATE(ui4_bitrate, ui1_version, ui1_layer, ui1_rate_index);              

                /* padding bit */
                ui1_padding = ((ac_tmp[2] & 0x2)>>1);

                if(0 != ui4_bitrate)
                {
                    ui4_frame_length = 
                        (ui4_frame_slots * ui4_bitrate / ui4_sample_rate + ui1_padding) * 
                          ui1_slot_bytes;
                }
                else
                {   /*error */
                    if (ac_buff != NULL)
                    {
                        x_mem_free(ac_buff);
                    }
                    return MFMTRECGR_INTERNAL_ERR;
                }
                /* jump to check next frame header file is valid or invalid */
                if (ui4_idx + ui4_frame_length < ui4_actual_read)
                {
                    x_memcpy(ac_tmp, (ac_buff + ui4_idx + ui4_frame_length), 3);
                }
                else
                {
                    if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                                              pv_mem_data,
                                              z_mem_leng,
                                              (INT64)(ui4_id3v2_offset + ui4_idx + ui4_frame_length),
                                              ac_tmp,
                                              3,
                                              &ui4_read,
                                              b_is_mem)) != MFMTRECGR_OK)
                    {
                        if (ac_buff != NULL)
                        {
                            x_mem_free(ac_buff);
                        }
                        return i4_ret;
                    }
                }

                /* next frame header is also valid */
                if (!IS_MP3_HEADER(ac_tmp))
                {
                    ui4_idx++;
                    
                    if (ui4_idx + ui4_frame_length < ui4_actual_read)
                    {
                        x_memcpy(ac_tmp, (ac_buff + ui4_idx + ui4_frame_length), 3);
                    }
                    else
                    {
                        if((i4_ret = mp3_read_n_byte_to_buf(h_file,
                                                  pv_mem_data,
                                                  z_mem_leng,
                                                  (INT64)(ui4_id3v2_offset + ui4_idx + ui4_frame_length),
                                                  ac_tmp,
                                                  3,
                                                  &ui4_read,
                                                  b_is_mem)) != MFMTRECGR_OK)
                        {
                            if (ac_buff != NULL)
                            {
                                x_mem_free(ac_buff);
                            }
                            return i4_ret;
                        }
                    }
                }
                if (IS_MP3_HEADER(ac_tmp))
                {
                    /* then the media type & subtype can be set correctly */
                    if ((ui1_version == 0)
                        || (ui1_version == 2)
                        || (ui1_version == 3))
                    {
                        if ((ui1_layer == 3)        /* Layer 1 */
                            || (ui1_layer == 2))    /* Layer 2 */
                        {
                            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP2;
                        }
                        else if (ui1_layer == 1)    /* Layer 3 */
                        {
                            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                        }
                        else
                        {
                            if (ac_buff)
                            {
                                x_mem_free(ac_buff);
                            }
                            return MFMTRECGR_INTERNAL_ERR;
                        }
                    }
                    if (ac_buff)
                    {
                        x_mem_free(ac_buff);
                    }    
                    return MFMTRECGR_OK;
                }
            }                

            /* read next 1 byte */
            ui4_idx++;
        }
    }
    if (ac_buff)
    {
        x_mem_free(ac_buff);
    }
    return MFMTRECGR_INTERNAL_ERR;
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_handler_file_confirm_fct(
                                    HANDLE_T           h_file,
                                    MEDIA_FORMAT_T*    pt_format)
{

#ifndef __NO_FM__
    return mp3_check_format(h_file, NULL, 0, pt_format, FALSE);
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mp3_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer which want to retrieve the format.
 *          z_mem_leng      specify the length of memory buffer which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp3_handler_mem_confirm_fct(
                        const VOID*       pv_mem_data,
                        SIZE_T            z_mem_leng,
                        MEDIA_FORMAT_T*   pt_format)
{
#if 0
    UCHAR           ac_tag[3];
    UINT8           ui1_layer, ui1_version;
    INT32           i4_ret;
    UINT32          ui4_read;
    UINT64          ui8_file_len;
    MEDIA_FEEDER_T* pt_src_feeder = NULL;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || (pv_mem_data == NULL))
        /* add feeder interface ...
        || (z_mem_leng == 0))
        */
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if (z_mem_leng == 0)  /* feeder case */
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
    }

    /* check ID3 v2 */
    if (z_mem_leng == 0)
    {
        /* seek to begin */
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                &pt_src_feeder->h_feeder,
                                (VOID*) ac_tag,
                                3,
                                3,
                                &ui4_read);
        if (ui4_read != 3)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
        x_memcpy(ac_tag, pv_mem_data, 3);
    }

    if (x_memcmp((VOID*)auc_ID3, ac_tag, 3) == 0)
    {
        pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
        pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
        return MFMTRECGR_OK;
    }
    /* check MP3 frame */
    /*
    *  syncword             11 bits
    *  version              2 bits
    *  layer                2 bits
    */
    if ((ac_tag[0] == 0xFF) && ((ac_tag[1]&0xE0) == 0xE0))
    {
        /* check version
        00 - MPEG Version 2.5 (unofficial)
        01 - reserved
        10 - MPEG Version 2 (ISO/IEC 13818-3)
        11 - MPEG Version 1 (ISO/IEC 11172-3)*/
        ui1_version = ((ac_tag[1]&0x18)>>3);
        if ((ui1_version == 0)
            || (ui1_version == 2)
            || (ui1_version == 3))
        {
            /* check layer
            00 - reserved
            01 - Layer III
            10 - Layer II
            11 - Layer I*/
            ui1_layer = ((ac_tag[1]&0x6)>>1);
            if ((ui1_layer == 1)
                || (ui1_layer == 2)
                || (ui1_layer == 3))
            {
                pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                return MFMTRECGR_OK;
            }
        }
    }


    /* check ID3 v1 */
    if (z_mem_leng == 0)
    {
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_END);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos(
                                &pt_src_feeder->h_feeder,
                                &ui8_file_len);

        /* seek to last 128 bytes */
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                (INT64)(ui8_file_len - 128),
                                MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                &pt_src_feeder->h_feeder,
                                (VOID*) ac_tag,
                                3,
                                3,
                                &ui4_read);
        if (ui4_read != 3)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
        x_memcpy((VOID*)ac_tag, (VOID*)&(((UCHAR*)pv_mem_data)[z_mem_leng - 128]), 3);
    }

    if (x_memcmp((VOID*)auc_TAG, (VOID*)ac_tag, 3) == 0)
    {
        pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
        pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
        return MFMTRECGR_OK;
    }
    return MFMTRECGR_INTERNAL_ERR;
#else
    return mp3_check_format(NULL_HANDLE, pv_mem_data, z_mem_leng, pt_format, TRUE);
#endif
}




