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
 * $RCSfile: ps_minfo.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
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
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"
#include "mutil/mm_util.h"
#include "drm/x_drm.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "../../mm_util.h"
#include "x_drm.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PS_USE_CACHE                    1 
#define PS_LOOKUP_LENGTH                (2*1024*1024)
#define PS_DURATION_OFFSET              (500*1024)
#define PS_CACHE_SIZE                   (128)
#define PS_LOOKUP_PART_NUMBER           2


#define PS_MPEG1_SYSTEM_CLOCK_FREQUENCY 90000
#define PS_MPEG2_SYSTEM_CLOCK_FREQUENCY 27000000

#define PS_PACK_START_CODE              0x000001BA
#define PS_SYSTEM_HEADER_START_CODE     0x000001BB
#define PS_PACKET_START_CODE_PREFIX     0x000001

#define PES_STREAM_START                0xBC
#define PES_STREAM_END                  0xFF

#define PES_PROGRAM_STREAM_MAP          0xBC
#define PES_PRIVATE_STREAM_1            0xBD
#define PES_PADDING                     0xBE
#define PES_PRIVATE_STREAM_2            0xBF
#define PES_ECM                         0xB0
#define PES_EMM                         0xB1
#define PES_PROGRAM_STREAM_DIRECTORY    0xFF
#define PES_DSMCC_STREAM                0xF2
#define PES_ITU_T_H222_1_TYPE_A_STREAM  0xF4
#define PES_ITU_T_H222_1_TYPE_B_STREAM  0xF5
#define PES_ITU_T_H222_1_TYPE_C_STREAM  0xF6
#define PES_ITU_T_H222_1_TYPE_D_STREAM  0xF7
#define PES_ITU_T_H222_1_TYPE_E_STREAM  0xF8

#define PES_VIDEO_MASK                  0xF0
#define PES_VIDEO_VAL                   0xE0

#define PES_AUDIO_MASK                  0xE0
#define PES_AUDIO_VAL                   0xC0

#define VOB_AUDIO_AC3_MASK              0xF8
#define VOB_AUDIO_AC3_VAL               0x80

#define VOB_AUDIO_DTS_MASK              0xF8
#define VOB_AUDIO_DTS_VAL               0x88

#define VOB_AUDIO_SDDS_MASK             0xF8
#define VOB_AUDIO_SDDS_VAL              0x90

#define VOB_AUDIO_LPCM_MASK             0xF8
#define VOB_AUDIO_LPCM_VAL              0xA0

#define VOB_SUB_PICTURE_MASK            0xE0
#define VOB_SUB_PICTURE_VAL             0x20

#define PS_MAX_VIDEO_NS                 1
#define PS_MAX_AUDIO_NS                 8
#define PS_MAX_SUB_PICTURE_NS           8

#define VIDEO_SEQUENCE_HEADER_CODE      0x000001B3
#define VIDEO_EXTENSION_START_CODE      0x000001B5
#define VIDEO_GROUP_PICTURE_CODE        0x000001B8
#define VIDEO_PICTURE_START_CODE        0x00000100

#define PS_MPEG1_STUFFING_BYTE          0xFF
#define PS_LAYER_LOOK_LENGTH            2048
#define PS_LAYER_LOOK_COUNT             2

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

typedef enum _PS_MPEG_TYPE_T
{
    PS_MPEG_1,
    PS_MPEG_2,
    PS_MPEG_UNKNOWN
}PS_MPEG_TYPE_T;

typedef struct _MINFO_PS_INF_T
{
    MEDIA_FORMAT_T          t_media_format;

    MINFO_TYPE_GENERIC_T    t_general_info;

    UINT64                  ui8_file_size;
    UINT64                  ui8_lookup_length;
    
    UINT8                   ui1_frame_rate;
    
    UINT64                  ui8_pts_min;
    UINT64                  ui8_pts_max;
    

    UINT64                  ui8_pos;

    UINT64                  ui8_cache_pos;
    UINT32                  ui4_read_count;
    
    UINT32                  ui4_system_header_count;
    UINT32                  ui4_navigation_count;

    MINFO_INFO_VIDEO_ENC_T  e_enc;
    
    BOOL                    b_finish;
    UINT8                   aui1_cache_buf[PS_CACHE_SIZE];
    UINT8                   ui1_audio_strm_num;
    UINT8                   ui1_video_strm_num;
    UINT8                   ui1_sub_picture_strm_num;
    MINFO_INFO_T            at_video_list[PS_MAX_VIDEO_NS];
    MINFO_INFO_T            at_audio_list[PS_MAX_AUDIO_NS];
    MINFO_INFO_T            at_sub_picture_list[PS_MAX_SUB_PICTURE_NS];
}MINFO_PS_INF_T;
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _ps_inp1byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT8*                pui1_data);

static INT32 _ps_inp2byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT16*               pui2_data);

static INT32 _ps_inp3byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT32*               pui4_data);

static INT32 _ps_inp4byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT32*               pui4_data);

static INT32 _ps_setpos(MINFO_PS_INF_T*         pt_ps_info,
                        MINFO_OBJ_T*            pt_minfo_obj,
                        INT64                   i8_offset, 
                        UINT8                   ui1_whence);

static INT32 _ps_getpos(MINFO_PS_INF_T*         pt_ps_info,
                        MINFO_OBJ_T*            pt_minfo_obj,
                        UINT64*                 pui8_offset);

static BOOL _ps_check_stream_id(UINT8 ui1_stream_id);

static INT32 _ps_check_finish(MINFO_PS_INF_T*   pt_ps_info,
                              MINFO_OBJ_T*      pt_minfo_obj,
                              BOOL*             pb_finish);

static INT32 _ps_next_start_code(MINFO_PS_INF_T*    pt_ps_info,
                                 MINFO_OBJ_T*       pt_minfo_obj,
                                 UINT32*            pui4_start_code,
                                 BOOL*              pb_found);

static INT32 _ps_sync_pack(MINFO_PS_INF_T*      pt_ps_info,
                           MINFO_OBJ_T*         pt_minfo_obj);


static INT32 _mpeg_vob(MINFO_PS_INF_T*      pt_ps_info,
                       MINFO_OBJ_T*         pt_minfo_obj,
                       UINT8                ui1_stream_id);

static INT32 _mpeg_audio_data(MINFO_PS_INF_T*       pt_ps_info,
                              MINFO_OBJ_T*          pt_minfo_obj,
                              UINT8                 ui1_stream_id);

static INT32 _mpeg_video_sequence(MINFO_PS_INF_T*   pt_ps_info,
                                  MINFO_OBJ_T*      pt_minfo_obj,
                                  UINT8             ui1_stream_id);


static INT32 _ps_pes_packet(MINFO_PS_INF_T*     pt_ps_info,
                            MINFO_OBJ_T*        pt_minfo_obj);

static INT32 _ps_system_header(MINFO_PS_INF_T*  pt_ps_info,
                               MINFO_OBJ_T*     pt_minfo_obj);

static INT32 _ps_pack_header(MINFO_PS_INF_T*    pt_ps_info,
                             MINFO_OBJ_T*       pt_minfo_obj);

static INT32 _ps_pack(MINFO_PS_INF_T*           pt_ps_info,
                      MINFO_OBJ_T*              pt_minfo_obj);

static INT32 _ps_program_stream(MINFO_PS_INF_T* pt_ps_info,
                                MINFO_OBJ_T*    pt_minfo_obj);

static INT32 _ps_check_audio_layer(MINFO_PS_INF_T*              pt_ps_info,
                                          MINFO_OBJ_T*          pt_minfo_obj,
                                          UINT8*                pui1_layer,
                                          UINT8*                pui1_version,
                                          BOOL*                 pb_found);
/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static INT32 _ps_inp1byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT8*                pui1_data)
{
#if (PS_USE_CACHE == 1)
    INT32						i4_ret;
    UINT64          ui8_pos;
    copybytes_fct   pf_copybytes;
    setpos_fct      pf_set_pos;
    
    pf_copybytes = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    
    _ps_getpos(pt_ps_info, pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos+1 > pt_ps_info->ui8_file_size
        || ui8_pos+1 >= pt_ps_info->ui8_lookup_length)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }

    if (0 == pt_ps_info->ui4_read_count
        || pt_ps_info->ui8_pos < pt_ps_info->ui8_cache_pos
        || pt_ps_info->ui8_pos+1 > pt_ps_info->ui8_cache_pos+pt_ps_info->ui4_read_count)
    {
        i4_ret = pf_set_pos(pt_minfo_obj, (INT64)pt_ps_info->ui8_pos, MEDIA_SEEK_BGN);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        i4_ret = pf_copybytes(pt_minfo_obj, 
                              pt_ps_info->aui1_cache_buf, 
                              PS_CACHE_SIZE, 
                              PS_CACHE_SIZE, 
                              &(pt_ps_info->ui4_read_count));
        if (i4_ret != MINFOR_OK || pt_ps_info->ui4_read_count > PS_CACHE_SIZE)
        {
            pt_ps_info->ui4_read_count = 0;
            return ((i4_ret == MINFOR_OK) ? MINFOR_OUT_OF_MEMORY : i4_ret);
        }

        pt_ps_info->ui8_cache_pos = pt_ps_info->ui8_pos;
    }

    *pui1_data = pt_ps_info->aui1_cache_buf[pt_ps_info->ui8_pos-pt_ps_info->ui8_cache_pos];
    pt_ps_info->ui8_pos++;
    
    return MINFOR_OK;

#else
    UINT64          ui8_pos;  
    getpos_fct      pf_get_pos;
    input1bytes_fct pf_inp1byte;    

    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos+1 > pt_ps_info->ui8_file_size
        || ui8_pos+1 >= pt_ps_info->ui8_lookup_length)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }
    *pui1_data = pf_inp1byte(pt_minfo_obj);

    return MINFOR_OK;
#endif    
}

static INT32 _ps_inp2byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT16*               pui2_data)
{
#if (PS_USE_CACHE == 1)
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    for (ui1_idx=0; ui1_idx<2; ui1_idx++)
    {
        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        
        *pui2_data = (((*pui2_data)<<8)|ui1_tmp);
    }

    return i4_ret;

#else    
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    UINT64          ui8_pos;  
    getpos_fct      pf_get_pos;
    input1bytes_fct pf_inp1byte;

    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos+2 > pt_ps_info->ui8_file_size
        || ui8_pos+2 > pt_ps_info->ui8_lookup_length)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }

    *pui2_data = 0;
    for (ui1_idx=0; ui1_idx<2; ui1_idx++)
    {
        ui1_tmp = pf_inp1byte(pt_minfo_obj);
        *pui2_data = (((*pui2_data)<<8)|ui1_tmp);
    }

    return MINFOR_OK;
#endif
}


static INT32 _ps_inp3byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT32*               pui4_data)
{
#if (PS_USE_CACHE == 1)
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    *pui4_data = 0;
    for (ui1_idx=0; ui1_idx<3; ui1_idx++)
    {
        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }

    return i4_ret;

#else
    UINT8           ui1_idx;
    UINT8           ui1_tmp;    
    UINT64          ui8_pos;   
    getpos_fct      pf_get_pos;
    input1bytes_fct pf_inp1byte;

    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos+3 > pt_ps_info->ui8_file_size
        || ui8_pos+3 > pt_ps_info->ui8_lookup_length)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }

    *pui4_data = 0;
    for (ui1_idx=0; ui1_idx<3; ui1_idx++)
    {
        ui1_tmp = pf_inp1byte(pt_minfo_obj);
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }

    return MINFOR_OK;
#endif
}

static INT32 _ps_inp4byte(MINFO_PS_INF_T*       pt_ps_info,
                          MINFO_OBJ_T*          pt_minfo_obj,
                          UINT32*               pui4_data)
{
#if (PS_USE_CACHE == 1)
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    for (ui1_idx=0; ui1_idx<4; ui1_idx++)
    {
        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }

    return i4_ret;

#else    
    UINT8           ui1_idx;
    UINT8           ui1_tmp;     
    UINT64          ui8_pos;  
    getpos_fct      pf_get_pos;
    input1bytes_fct pf_inp1byte;

    pf_inp1byte = pt_minfo_obj->t_input_fct_tbl.pf_input1;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_minfo_obj, &ui8_pos);
    
    if (ui8_pos+4 > pt_ps_info->ui8_file_size
        || ui8_pos+4 > pt_ps_info->ui8_lookup_length)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }

    *pui4_data = 0;
    for (ui1_idx=0; ui1_idx<4; ui1_idx++)
    {
        ui1_tmp = pf_inp1byte(pt_minfo_obj);
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }
    
    return MINFOR_OK;
#endif
}

static INT32 _ps_setpos(MINFO_PS_INF_T*         pt_ps_info,
                        MINFO_OBJ_T*            pt_minfo_obj,
                        INT64                   i8_offset, 
                        UINT8                   ui1_whence)
{
INT32 i4_ret;
#if (PS_USE_CACHE == 1)    
    INT64           i8_pos;
    setpos_fct      pf_set_pos;
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;

    switch (ui1_whence)
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_ps_info->ui8_pos);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_ps_info->ui8_file_size);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return MINFOR_INV_ARG;
    }
    
    if (i8_pos < 0)
    {
        return MINFOR_INV_ARG;
    }
    
    if (i8_pos > (INT64)pt_ps_info->ui8_file_size)
    {
        pt_ps_info->b_finish = TRUE;
        return MINFOR_EOT;
    }

    pt_ps_info->ui8_pos = (UINT64)i8_pos;
    
    if (pt_ps_info->ui8_pos < pt_ps_info->ui8_cache_pos
        || pt_ps_info->ui8_pos+1 > pt_ps_info->ui8_cache_pos+pt_ps_info->ui4_read_count)
    {
        pt_ps_info->ui4_read_count = 0;
        i4_ret = pf_set_pos(pt_minfo_obj, i8_pos, MEDIA_SEEK_BGN|MEDIA_SEEK_CHK);
        if(i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
    }
    
    return MINFOR_OK;
#else
    setpos_fct      pf_set_pos;

    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    i4_ret = pf_set_pos(pt_minfo_obj, i8_offset, ui1_whence|MEDIA_SEEK_CHK);
    if(i4_ret != INPUTR_OK)
    {
        return MINFOR_INTERNAL_ERR;
    }
    return MINFOR_OK;

#endif
}

static INT32 _ps_getpos(MINFO_PS_INF_T*         pt_ps_info,
                        MINFO_OBJ_T*            pt_minfo_obj,
                        UINT64*                 pui8_offset)
{
#if (PS_USE_CACHE == 1)
    *pui8_offset = pt_ps_info->ui8_pos;
#else
    pt_minfo_obj->t_input_fct_tbl.pf_get_pos(pt_minfo_obj, pui8_offset);
#endif
    return MINFOR_OK;
}

static INT32 _ps_check_finish(MINFO_PS_INF_T*   pt_ps_info,
                              MINFO_OBJ_T*      pt_minfo_obj,
                              BOOL*             pb_finish)
{
    *pb_finish = pt_ps_info->b_finish;
    return MINFOR_OK;
}

static BOOL _ps_check_stream_id(UINT8 ui1_stream_id)
{
    UINT16  ui2_stream_id;
    
    ui2_stream_id = (UINT16)ui1_stream_id;
    if ((ui2_stream_id >= PES_STREAM_START) && (ui2_stream_id <= PES_STREAM_END))
    {
        return TRUE;
    }
    return FALSE;
}

static INT32 _ps_next_start_code(MINFO_PS_INF_T*    pt_ps_info,
                                 MINFO_OBJ_T*       pt_minfo_obj,
                                 UINT32*            pui4_start_code,
                                 BOOL*              pb_found)

{
    UINT8   ui1_tmp;
    UINT32  ui4_start_code;
    INT32   i4_ret;
    
    i4_ret = _ps_inp4byte(pt_ps_info, pt_minfo_obj, &ui4_start_code);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    do
    {
        if(ui4_start_code >= 0x100)
        {
            *pui4_start_code = ui4_start_code;
            return MINFOR_OK;
        }
        else
        {
            i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
            if(i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }
            ui4_start_code = ((ui4_start_code<<8)|ui1_tmp);
        }
    }while(1);

    /*return MINFOR_OK;*/
}


static INT32 _ps_sync_pack(MINFO_PS_INF_T*      pt_ps_info,
                           MINFO_OBJ_T*         pt_minfo_obj)
{
    UINT8   ui1_tmp;
    UINT32  ui4_pack_start_code;
    INT32   i4_ret;
    
    i4_ret = _ps_inp4byte(pt_ps_info, pt_minfo_obj, &ui4_pack_start_code);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    do
    {        
        if (ui4_pack_start_code == PS_PACK_START_CODE)
        {
            i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, -4, MEDIA_SEEK_CUR);
            return MINFOR_OK;
        }
        else
        {
            i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
            if(i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }

            ui4_pack_start_code = ((ui4_pack_start_code<<8)|ui1_tmp);
        }
    }while(1);
}

static INT32 _mpeg_vob(MINFO_PS_INF_T*      pt_ps_info,
                       MINFO_OBJ_T*         pt_minfo_obj,
                       UINT8                ui1_stream_id)
{
    INT32                           i4_ret;
    BOOL                            b_found;
    UINT8                           ui1_idx;    
    UINT8                           ui1_sub_stream_id;
    BOOL                            b_audio;
    BOOL                            b_sub_picture;
    MINFO_PS_AUDIO_STM_ATRBT_T      t_ps_audio_stm_attr;
    MINFO_PS_SUBTITLE_STM_ATRBT_T   t_ps_sp_stm_attr;

    t_ps_sp_stm_attr.ui1_strm_id= 0;
    t_ps_sp_stm_attr.ui1_sub_strm_id = 0;
    t_ps_audio_stm_attr.i4_sampling_frequency = 0;
    t_ps_audio_stm_attr.i4_samples_per_quant = 0;
    t_ps_audio_stm_attr.i2_channels = 0;
    t_ps_audio_stm_attr.e_layer = 0;
    b_audio = FALSE;
    b_sub_picture = FALSE;
    
    i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_sub_stream_id);
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    if (VOB_AUDIO_AC3_VAL ==(ui1_sub_stream_id&VOB_AUDIO_AC3_MASK))
    {
        t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AC3;
        b_audio= TRUE;
    }
    else if (VOB_AUDIO_DTS_VAL ==(ui1_sub_stream_id&VOB_AUDIO_DTS_MASK))
    {
        t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_DTS;
        b_audio= TRUE;
    }
    else if (VOB_AUDIO_LPCM_VAL ==(ui1_sub_stream_id&VOB_AUDIO_LPCM_MASK))
    {
        t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_LPCM;
        b_audio= TRUE;
    }
    else if (VOB_SUB_PICTURE_VAL ==(ui1_sub_stream_id&VOB_SUB_PICTURE_MASK))
    {
        b_sub_picture = TRUE;
    }
    
    if (b_audio)
    {
        t_ps_audio_stm_attr.ui1_sub_stream_id = ui1_sub_stream_id;
        b_found = FALSE;
        
        for(ui1_idx=0; ui1_idx<pt_ps_info->ui1_audio_strm_num; ui1_idx++)
        {
            if (ui1_idx>=PS_MAX_AUDIO_NS)
            {
                break;
            }
            
            if (ui1_stream_id == (UINT8)pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id
                && ui1_sub_stream_id == (UINT8)pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id)
            {                        
                b_found = TRUE;
                break;
            }
        }

        if ((ui1_idx<PS_MAX_AUDIO_NS) && (!b_found))
        {
            if (pt_ps_info->ui1_audio_strm_num < PS_MAX_AUDIO_NS)
            {
                pt_ps_info->at_audio_list[ui1_idx].t_format = pt_ps_info->t_media_format;
                pt_ps_info->at_audio_list[ui1_idx].e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
                pt_ps_info->at_audio_list[ui1_idx].ui1_rec_idx = ui1_idx;
                pt_ps_info->ui1_audio_strm_num++;
    
                pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id = ui1_stream_id;
                pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = ui1_sub_stream_id;
                
                
                if (0 == ui1_idx)
                {
                    t_ps_audio_stm_attr.b_default = TRUE;
                }
                else
                {
                    t_ps_audio_stm_attr.b_default = FALSE;
                }
        
                pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.u.t_ps_audio_stm_attr = t_ps_audio_stm_attr;
            }
        }
    }
    
    if (b_sub_picture)
    {
        b_found = FALSE;

        if (pt_ps_info->ui1_sub_picture_strm_num <= PS_MAX_SUB_PICTURE_NS)
        {
            for(ui1_idx=0; ui1_idx<pt_ps_info->ui1_sub_picture_strm_num; ui1_idx++)
            {
                if (ui1_stream_id == (UINT8)pt_ps_info->at_sub_picture_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id
					&& ui1_sub_stream_id == (UINT8)pt_ps_info->at_sub_picture_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id) 
                {                        
                    b_found = TRUE;
                    break;
                }
            }
            
            if (!b_found)
            {
                if (ui1_idx < PS_MAX_SUB_PICTURE_NS)
                {
                    t_ps_sp_stm_attr.ui1_strm_id= ui1_stream_id;
                    t_ps_sp_stm_attr.ui1_sub_strm_id = ui1_sub_stream_id;
                    pt_ps_info->at_sub_picture_list[ui1_idx].t_format = pt_ps_info->t_media_format;
                    pt_ps_info->at_sub_picture_list[ui1_idx].e_info_type = MINFO_INFO_TYPE_SP_ATTR;
                    pt_ps_info->at_sub_picture_list[ui1_idx].ui1_rec_idx = ui1_idx;
                    pt_ps_info->at_sub_picture_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id = ui1_stream_id;
                    pt_ps_info->at_sub_picture_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = ui1_sub_stream_id;
                    pt_ps_info->at_sub_picture_list[ui1_idx].u.t_stm_attribute.u.t_ps_sp_stm_attr = t_ps_sp_stm_attr;

                    pt_ps_info->ui1_sub_picture_strm_num++;
                }
            }
        }
    }
    return i4_ret;
}


static INT32 _mpeg_audio_data(MINFO_PS_INF_T*       pt_ps_info,
                              MINFO_OBJ_T*          pt_minfo_obj,
                              UINT8                 ui1_stream_id)
{
    BOOL                            b_found;
    BOOL                            b_need_update;
    UINT8                           ui1_idx;
    MINFO_PS_AUDIO_STM_ATRBT_T      t_ps_audio_stm_attr;
    INT32                           i4_ret;
    UINT8                           ui1_layer = 0;
    UINT8                           ui1_version = 0;

    x_memset(&t_ps_audio_stm_attr, 0, sizeof(MINFO_PS_AUDIO_STM_ATRBT_T));
    
    switch (pt_ps_info->t_media_format.t_media_subtype.u.e_ps_subtype)
    {
    case MEDIA_PS_SUBTYPE_VOB:
        {
            if (ui1_stream_id & 0x10)
            {
                t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
            }
            else
            {
                t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_MPEG_2;
    
            }
        }
        break;
        
    case MEDIA_PS_SUBTYPE_MPG:
    case MEDIA_PS_SUBTYPE_DAT:
        {
            t_ps_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        }
        break;
        
    default:
        break;
    }
    
    t_ps_audio_stm_attr.ui1_sub_stream_id = 0;
    
    b_found = FALSE;
    
    for(ui1_idx=0; ui1_idx<pt_ps_info->ui1_audio_strm_num; ui1_idx++)
    {
        if (ui1_idx >= PS_MAX_AUDIO_NS)
        {
            break;
        }
        
        if (ui1_stream_id == (UINT8)pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id)
        {                        
            b_found = TRUE;
            break;
        }
    }
    
    if (!b_found || pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.u.t_ps_audio_stm_attr.e_layer == 0)
    {
        if (ui1_idx >= PS_MAX_AUDIO_NS)
        {
            return MINFOR_OK;
        }
        i4_ret = _ps_check_audio_layer(pt_ps_info,pt_minfo_obj,&ui1_layer,&ui1_version,&b_need_update);
           
        if(i4_ret != MINFOR_OK)
        {
            /*return i4_ret;*/
        }
        if(b_need_update)
        {
            t_ps_audio_stm_attr.e_enc    = (4 - ui1_version == 1) ? MINFO_INFO_AUD_ENC_MPEG_1 : MINFO_INFO_AUD_ENC_MPEG_2;
            t_ps_audio_stm_attr.e_layer  = 4 - ui1_layer;
        }
        if (pt_ps_info->ui1_audio_strm_num < PS_MAX_AUDIO_NS)
        {
            pt_ps_info->at_audio_list[ui1_idx].t_format = pt_ps_info->t_media_format;
            pt_ps_info->at_audio_list[ui1_idx].e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
            pt_ps_info->at_audio_list[ui1_idx].ui1_rec_idx = ui1_idx;
            pt_ps_info->ui1_audio_strm_num += (ui1_idx == pt_ps_info->ui1_audio_strm_num ? 1 : 0);
    
            pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id = ui1_stream_id;
            pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;
                    
            
            if (0 == ui1_idx)
            {
                t_ps_audio_stm_attr.b_default = TRUE;
            }
            else
            {
                t_ps_audio_stm_attr.b_default = FALSE;
            }
    
            pt_ps_info->at_audio_list[ui1_idx].u.t_stm_attribute.u.t_ps_audio_stm_attr = t_ps_audio_stm_attr;
        }
    }

    return MINFOR_OK;
}


static INT32 _mpeg_video_sequence(MINFO_PS_INF_T*   pt_ps_info,
                                  MINFO_OBJ_T*      pt_minfo_obj,
                                  UINT8             ui1_stream_id)
{
    INT32                           i4_ret;
    UINT32                          ui4_sequence_header_code;
    UINT32                          ui4_height;
    UINT32                          ui4_width;
    UINT32                          ui4_tmp4;
    BOOL                            b_found;
    UINT8                           ui1_idx;
    UINT8                           ui1_tmp1;
    BOOL                            b_seq_header;
    BOOL                            b_picture_header;
    UINT8                           ui1_aspect_ratio;
    UINT8                           ui1_load_intra_quantiser_matrix;
    UINT8                           ui1_load_non_intra_quantiser_matrix;
    UINT8                           ui1_height_ext;
    UINT8                           ui1_width_ext;
    MINFO_PS_VIDEO_STM_ATRBT_T      t_ps_video_stm_attr;

    t_ps_video_stm_attr.i4_height = 0;
    t_ps_video_stm_attr.i4_width = 0;
        
    i4_ret = _ps_next_start_code(pt_ps_info,
                                 pt_minfo_obj,
                                 &ui4_sequence_header_code,
                                 &b_found);
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    b_seq_header = FALSE;
    b_picture_header = FALSE;
    
    if (VIDEO_SEQUENCE_HEADER_CODE == ui4_sequence_header_code)
    {
        /*video sequence header*/
        b_seq_header = TRUE;
        
        i4_ret = _ps_inp3byte(pt_ps_info, pt_minfo_obj, &ui4_tmp4);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        ui4_width = ((ui4_tmp4>>12)&0xFFF);
        ui4_height = (ui4_tmp4&0xFFF);
        

        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_aspect_ratio);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        ui1_aspect_ratio = (ui1_aspect_ratio>>4);
        
        pt_ps_info->ui1_frame_rate = (ui1_aspect_ratio&0x0F);
        
        
        i4_ret = _ps_inp4byte(pt_ps_info, pt_minfo_obj, &ui4_tmp4);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
        
        ui1_load_intra_quantiser_matrix = (UINT8)(ui4_tmp4 & 0x02);
        if (ui1_load_intra_quantiser_matrix)
        {
            for (ui1_idx=0; ui1_idx<64; ui1_idx++)
            {
                i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp1);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
            }
            ui1_load_non_intra_quantiser_matrix = ui1_tmp1 & 0x01;
        }
        else
        {
            ui1_load_non_intra_quantiser_matrix = (UINT8)(ui4_tmp4 & 0x01);
        }

        if (ui1_load_non_intra_quantiser_matrix)
        {
            i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 64, MEDIA_SEEK_CUR);
            if(i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }
        }

        /*video sequence extension*/
        i4_ret = _ps_next_start_code(pt_ps_info,
                                     pt_minfo_obj,
                                     &ui4_tmp4,
                                     &b_found);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        if (VIDEO_EXTENSION_START_CODE == ui4_tmp4)
        {
            i4_ret = _ps_inp4byte(pt_ps_info, pt_minfo_obj, &ui4_tmp4);
            if (i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }
            
            ui1_height_ext = (UINT8)((ui4_tmp4>>13)&0x03);
            ui1_width_ext = (UINT8)((ui4_tmp4>>15)&0x03);

            ui4_height = (((UINT32)(ui1_height_ext)<<12)|ui4_height);
            ui4_width = (((UINT32)(ui1_width_ext)<<12)|ui4_width);
        }
            
        t_ps_video_stm_attr.ui1_stream_id = ui1_stream_id;
        t_ps_video_stm_attr.e_enc = pt_ps_info->e_enc;
        t_ps_video_stm_attr.i4_height = (INT32)ui4_height;
        t_ps_video_stm_attr.i4_width = (INT32)ui4_width;
        
        switch (ui1_aspect_ratio)
        {
        case 2: /*4:3*/
            if(ui4_height<=576 && ui4_width<=720)
            {
                t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_SD_4_3;
            }
            else
            {
                t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_HD_4_3;
            }
            break;

        case 3: /*16:9*/
            if(ui4_height<=576 && ui4_width<=720)
            {
                t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_SD_16_9;
            }
            else
            {
                t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_HD_16_9;
            }

            break;
            
        case 1: /*1:1*/   
        case 4: /*2.21:1*/
        default:
            t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_UNKNOWN;
            break;
        }
    }
    else if ( (VIDEO_GROUP_PICTURE_CODE == ui4_sequence_header_code) ||
              (VIDEO_PICTURE_START_CODE == ui4_sequence_header_code) )
    {
        b_picture_header = TRUE;

        t_ps_video_stm_attr.ui1_stream_id = ui1_stream_id;
        t_ps_video_stm_attr.e_enc = pt_ps_info->e_enc;
        
        if (MINFO_INFO_VID_ENC_MPEG_1 == t_ps_video_stm_attr.e_enc)
        {
            t_ps_video_stm_attr.i4_height = 240;
            t_ps_video_stm_attr.i4_width = 320;
        }
        else
        {
            t_ps_video_stm_attr.i4_height = 480;
            t_ps_video_stm_attr.i4_width = 720;
        }
        
        t_ps_video_stm_attr.e_vid_fmt = MINFO_INFO_VID_FMT_SD_4_3;

    }
    
    b_found = FALSE;
    
    for(ui1_idx=0; ui1_idx<pt_ps_info->ui1_video_strm_num; ui1_idx++)
    {
        if (ui1_stream_id == (UINT8)pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id)
        {
            if (b_seq_header)
            {
                if (0 == ui1_idx)
                {
                    t_ps_video_stm_attr.b_default = TRUE;
                }
                else
                {
                    t_ps_video_stm_attr.b_default = FALSE;
                }
                pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr = t_ps_video_stm_attr;
            }
            else if (b_picture_header)
            {
                if (0 == ui1_idx)
                {
                    t_ps_video_stm_attr.b_default = TRUE;
                }
                else
                {
                    t_ps_video_stm_attr.b_default = FALSE;
                }
                
                if (pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_height == 0 ||
                    pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_width == 0)
                {
                }
                else
                {
                    t_ps_video_stm_attr.i4_height = pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_height;
                    t_ps_video_stm_attr.i4_width = pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_width;
                }
                pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr = t_ps_video_stm_attr;
            }
            
            b_found = TRUE;
            break;
        }
    }
    
    if (!b_found)
    {   
        if (b_seq_header || b_picture_header)
        {
            if (pt_ps_info->ui1_video_strm_num < PS_MAX_VIDEO_NS)
            {
                pt_ps_info->at_video_list[ui1_idx].t_format = pt_ps_info->t_media_format;
                pt_ps_info->at_video_list[ui1_idx].e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
                pt_ps_info->at_video_list[ui1_idx].ui1_rec_idx = ui1_idx;
                pt_ps_info->ui1_video_strm_num++;


                pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id = ui1_stream_id;
                pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id = 0;


                t_ps_video_stm_attr.ui1_stream_id = ui1_stream_id;
                t_ps_video_stm_attr.e_enc = pt_ps_info->e_enc;
                
                if (0 == ui1_idx)
                {
                    t_ps_video_stm_attr.b_default = TRUE;
                }
                else
                {
                    t_ps_video_stm_attr.b_default = FALSE;
                }

                pt_ps_info->at_video_list[ui1_idx].u.t_stm_attribute.u.t_ps_video_stm_attr = t_ps_video_stm_attr;
            }
        }
    }

    return i4_ret;
}

static INT32 _ps_pes_packet(MINFO_PS_INF_T*     pt_ps_info,
                            MINFO_OBJ_T*        pt_minfo_obj)
{
    BOOL                            b_found;
    UINT8                           ui1_pes_header_data_length;
    UINT8                           ui1_stream_id;
    UINT8                           ui1_pts_dts_flag;
    UINT8                           ui1_tmp1;
    UINT16                          ui2_pes_packet_length;
    INT32                           i4_ret;
    UINT32                          ui4_pes_start_code;
    UINT64                          ui8_pos;
    UINT16                          ui2_tmp2;
    UINT64                          ui8_pts;

    
    i4_ret = _ps_next_start_code(pt_ps_info,
                                 pt_minfo_obj,
                                 &ui4_pes_start_code,
                                 &b_found);
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    ui1_stream_id = (UINT8)(ui4_pes_start_code & 0xFF);
    ui4_pes_start_code = (ui4_pes_start_code>>8);
        
    if ((PS_PACKET_START_CODE_PREFIX != ui4_pes_start_code)
        || !_ps_check_stream_id(ui1_stream_id))
    {
        _ps_setpos(pt_ps_info, pt_minfo_obj, -4, MEDIA_SEEK_CUR);
        return MINFOR_FMT_NOT_SUPPORT;
    }
    else
    { 
        /*PES data*/
        i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_pes_packet_length);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        i4_ret = _ps_getpos(pt_ps_info, pt_minfo_obj, &ui8_pos);
        if (i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        if ( ui1_stream_id != PES_PROGRAM_STREAM_MAP
            && ui1_stream_id != PES_PADDING
            && ui1_stream_id != PES_PRIVATE_STREAM_2
            && ui1_stream_id != PES_ECM
            && ui1_stream_id != PES_EMM
            && ui1_stream_id != PES_PROGRAM_STREAM_DIRECTORY
            && ui1_stream_id != PES_DSMCC_STREAM
            && ui1_stream_id != PES_ITU_T_H222_1_TYPE_E_STREAM) 
        {
            if (pt_ps_info->e_enc == MINFO_INFO_VID_ENC_MPEG_1)
            {/*packet()*/
                do
                {
                    i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp1);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    if(ui1_tmp1 != PS_MPEG1_STUFFING_BYTE)
                    {
                        break;
                    }
                }while(1);

                if (0x01 == (ui1_tmp1>>6))
                {
                    i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 1, MEDIA_SEEK_CUR);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp1);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                }
                if (0x02 == (ui1_tmp1>>4))
                {
                    ui8_pts = (UINT64)((UINT64)((ui1_tmp1>>1)&0x07)<<30);

                    
                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = ui8_pts | (UINT64)((UINT64)(ui2_tmp2>>1)<<15);


                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = ui8_pts | (UINT64)(ui2_tmp2>>1);
                    
                    if (pt_ps_info->ui8_pts_min > ui8_pts)
                    {
                        pt_ps_info->ui8_pts_min = ui8_pts;
                    }
                    
                    if (pt_ps_info->ui8_pts_max < ui8_pts)
                    {
                        pt_ps_info->ui8_pts_max = ui8_pts;
                    }
                }
                else if (0x03 == (ui1_tmp1>>4))
                {
                    ui8_pts = (UINT64)((UINT64)((ui1_tmp1>>1)&0x07)<<30);

                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = ui8_pts | (UINT64)((UINT64)(ui2_tmp2>>1)<<15);

                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    
                    ui8_pts = ui8_pts | (UINT64)(ui2_tmp2>>1);
                    
                    if (pt_ps_info->ui8_pts_min > ui8_pts)
                    {
                        pt_ps_info->ui8_pts_min = ui8_pts;
                    }
                    
                    if (pt_ps_info->ui8_pts_max < ui8_pts)
                    {
                        pt_ps_info->ui8_pts_max = ui8_pts;
                    }
                    i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 5, MEDIA_SEEK_CUR);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                }
                else if(ui1_tmp1 == 0x0F)
                {
                }
                else
                {
                    return MINFOR_FMT_NOT_SUPPORT;
                }
            }
            else
            {
                i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 1, MEDIA_SEEK_CUR);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
                
                i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_pts_dts_flag);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
                ui1_pts_dts_flag = (ui1_pts_dts_flag>>6);
                

                i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_pes_header_data_length);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
                
                if (ui1_pts_dts_flag == 0x02 || ui1_pts_dts_flag == 0x03)
                {
                    i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp1);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = (UINT64)((UINT64)((ui1_tmp1>>1)&0x07)<<30);

                    
                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = ui8_pts | (UINT64)((UINT64)(ui2_tmp2>>1)<<15);


                    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_tmp2);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                    ui8_pts = ui8_pts | (UINT64)(ui2_tmp2>>1);
                    
                    if (pt_ps_info->ui8_pts_min > ui8_pts)
                    {
                        pt_ps_info->ui8_pts_min = ui8_pts;
                    }
                    
                    if (pt_ps_info->ui8_pts_max < ui8_pts)
                    {
                        pt_ps_info->ui8_pts_max = ui8_pts;
                    }
                
                    i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, ui1_pes_header_data_length-5, MEDIA_SEEK_CUR);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                }
                else
                {
                    i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, ui1_pes_header_data_length, MEDIA_SEEK_CUR);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                }
            }
            
            if (PES_VIDEO_VAL == (ui1_stream_id & PES_VIDEO_MASK))
            {
                i4_ret = _mpeg_video_sequence(pt_ps_info,
                                              pt_minfo_obj,
                                              ui1_stream_id);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }

            }
            else if (PES_AUDIO_VAL == (ui1_stream_id&PES_AUDIO_MASK))
            {
                i4_ret = _mpeg_audio_data(pt_ps_info,
                                          pt_minfo_obj,
                                          ui1_stream_id);
                if (i4_ret != MINFOR_OK)
                {
                    return i4_ret;
                }
            }
            else if (ui1_stream_id == PES_PRIVATE_STREAM_1)
            {
                if (MEDIA_PS_SUBTYPE_VOB == pt_ps_info->t_media_format.t_media_subtype.u.e_ps_subtype
                    || MEDIA_PS_SUBTYPE_MPG == pt_ps_info->t_media_format.t_media_subtype.u.e_ps_subtype)
                {
                    i4_ret = _mpeg_vob(pt_ps_info,
                                       pt_minfo_obj,
                                       ui1_stream_id);
                    if (i4_ret != MINFOR_OK)
                    {
                        return i4_ret;
                    }
                }
            }
        }
        else if ( ui1_stream_id == PES_PROGRAM_STREAM_MAP
            || ui1_stream_id == PES_PRIVATE_STREAM_2
            || ui1_stream_id == PES_ECM
            || ui1_stream_id == PES_EMM
            || ui1_stream_id == PES_PROGRAM_STREAM_DIRECTORY
            || ui1_stream_id == PES_DSMCC_STREAM
            || ui1_stream_id == PES_ITU_T_H222_1_TYPE_E_STREAM)
        {
            /*for ( i=0;i<PES_packet_length;i++) 
            {
                PES_packet_data_byte
            }*/
        }
        else
        {
            /*i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, ui2_pes_packet_length, MEDIA_SEEK_CUR);
            if(i4_ret != MINFOR_OK)
            {
                return i4_ret;
            }*/
        }
        
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, (INT64)(ui8_pos+ui2_pes_packet_length), MEDIA_SEEK_BGN);
        if(i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
    }

    return i4_ret;
}


static INT32 _ps_system_header(MINFO_PS_INF_T*  pt_ps_info,
                               MINFO_OBJ_T*     pt_minfo_obj)
{
    UINT16  ui2_header_length;
    BOOL    b_found;
    INT32   i4_ret;
    UINT32  ui4_system_header_start_code;

    i4_ret = _ps_next_start_code(pt_ps_info,
                                 pt_minfo_obj,
                                 &ui4_system_header_start_code,
                                 &b_found);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    if (ui4_system_header_start_code != PS_SYSTEM_HEADER_START_CODE)
    {
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, -4, MEDIA_SEEK_CUR);
        return MINFOR_FMT_NOT_SUPPORT;
    }
    
    /*skip system header*/
    pt_ps_info->ui4_system_header_count++;
    if(pt_ps_info->ui4_system_header_count>=2)
    {
        /*pt_ps_info->b_finish = TRUE;*/
    }
    
    i4_ret = _ps_inp2byte(pt_ps_info, pt_minfo_obj, &ui2_header_length);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, (INT64)ui2_header_length, MEDIA_SEEK_CUR);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    return MINFOR_OK;
}


static INT32 _ps_pack_header(MINFO_PS_INF_T*    pt_ps_info,
                             MINFO_OBJ_T*       pt_minfo_obj)
{
    UINT8   ui1_tmp;
    BOOL    b_found;
    INT32   i4_ret;
    UINT32  ui4_pack_start_code;

    i4_ret = _ps_next_start_code(pt_ps_info,
                                 pt_minfo_obj,
                                 &ui4_pack_start_code,
                                 &b_found);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    if (ui4_pack_start_code != PS_PACK_START_CODE)
    {
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, -4, MEDIA_SEEK_CUR);
        return MINFOR_FMT_NOT_SUPPORT;
    }

    i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
    if(i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    if ((ui1_tmp & 0xf0) == 0x20)
    {
        /*mpeg1*/
        pt_ps_info->e_enc = MINFO_INFO_VID_ENC_MPEG_1;
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 7, MEDIA_SEEK_CUR);
        if(i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
    }
    else if ((ui1_tmp & 0xC0) == 0x40)
    { 
        /*mpeg2*/
        pt_ps_info->e_enc = MINFO_INFO_VID_ENC_MPEG_2;
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, 8, MEDIA_SEEK_CUR);
        if(i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
        if(i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }

        /*stuffing_byte*/
        ui1_tmp &= 0x07;
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj, ui1_tmp, MEDIA_SEEK_CUR);
        if(i4_ret != MINFOR_OK)
        {
            return i4_ret;
        }
    }
    else
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }

    i4_ret = _ps_system_header(pt_ps_info, pt_minfo_obj);
    if (MINFOR_FMT_NOT_SUPPORT == i4_ret)/*no system header*/
    {
        i4_ret = MINFOR_OK;
    }
    return i4_ret;
}

static INT32 _ps_pack(MINFO_PS_INF_T*           pt_ps_info,
                      MINFO_OBJ_T*              pt_minfo_obj)
{
    INT32   i4_ret;
        
    i4_ret = _ps_pack_header(pt_ps_info, pt_minfo_obj);
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    while (1)
    {
        i4_ret = _ps_pes_packet(pt_ps_info,
                                pt_minfo_obj);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
    }

    return i4_ret;
}


static INT32 _ps_program_stream(MINFO_PS_INF_T* pt_ps_info,
                                MINFO_OBJ_T*    pt_minfo_obj)
{
    INT32   i4_ret;
    BOOL    b_finish;

    pt_ps_info->b_finish = FALSE;
    
    do
    {
        i4_ret = _ps_sync_pack(pt_ps_info,
                               pt_minfo_obj);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }

        i4_ret = _ps_pack(pt_ps_info, pt_minfo_obj);
        if (i4_ret != MINFOR_OK)
        {
        }

        _ps_check_finish(pt_ps_info, pt_minfo_obj, &b_finish);
        if(b_finish)
        {
            break;
        }
        
    }while(1);

    return MINFOR_OK;
}
static INT32 _ps_check_audio_layer(MINFO_PS_INF_T*       pt_ps_info,
                                          MINFO_OBJ_T*          pt_minfo_obj,
                                          UINT8*                pui1_layer,
                                          UINT8*                pui1_version,
                                          BOOL*                 pb_found)
{   
    UINT32       ui4_mp3_data    = 0;
    UINT32       ui4_mp3_tag     = 0;
    UINT8        ui1_tmp         = 0;
    UINT8        ui1_frame_count = 0;    
    UINT8        ui1_version     = 0;
    UINT8        ui1_rate_index  = 0;    
    UINT16       ui2_sample_rate = 0;
    UINT8        ui1_padding     = 0;
    UINT32       ui4_frame_length= 0;
    UINT32       ui4_bitrate     = 0;
    UINT8        ui1_slot_bytes  = 0;
    UINT32       ui4_frame_slots = 0;
    UINT64       ui8_file_pos    = 0;
    UINT64       ui8_form_pos    = 0;    
    BOOL         b_const         = TRUE;
    MINFO_PS_AUDIO_STM_ATRBT_T   t_ps_audio_stm_attr;
    INT32        i4_ret          = MINFOR_OK;
    
     i4_ret = _ps_getpos(pt_ps_info, pt_minfo_obj, &ui8_form_pos);
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
        
    *pb_found = FALSE;
    
    t_ps_audio_stm_attr.e_enc   = (MINFO_INFO_AUDIO_ENC_T)0;
    t_ps_audio_stm_attr.e_layer = (MINFO_INFO_AUD_LAYER_T)0;
    
    while(ui8_file_pos      <  ui8_form_pos+PS_LAYER_LOOK_LENGTH
         && ui8_file_pos    <= pt_ps_info->ui8_file_size
         && ui1_frame_count <= PS_LAYER_LOOK_COUNT)
    {
        i4_ret = _ps_inp1byte(pt_ps_info, pt_minfo_obj, &ui1_tmp);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        ui4_mp3_data = (ui4_mp3_data << 8) | ui1_tmp;
            
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        if ((((UINT8*)(&ui4_mp3_data))[3] != 0xFF)
            || ((((UINT8*)(&ui4_mp3_data))[2] & 0xE0) != 0xE0)
            || ((((UINT8*)(&ui4_mp3_data))[2] & 0x18) >> 3 == 1)      /* version description is set to reserved */
            || ((((UINT8*)(&ui4_mp3_data))[2] & 0x06) >> 1 == 0)      /* layer description is set to reserved */
            || ((((UINT8*)(&ui4_mp3_data))[1] >> 4) == 0)
            || ((((UINT8*)(&ui4_mp3_data))[1] >> 4) == 15))
        {
            continue;
        }
        else
        {
            //x_memcpy(ac_tag,(UINT8*)ui4_mp3_data, 3);
            ui4_mp3_tag = ui4_mp3_data;
            ui1_version = ((((UINT8*)(&ui4_mp3_tag))[2] & 0x18) >> 3);
            *pui1_version = ui1_version;
            *pui1_layer = ((((UINT8*)(&ui4_mp3_tag))[2] & 0x6) >> 1);
            
            /* bitrate index */
            ui1_rate_index = ((UINT8*)(&ui4_mp3_tag))[1] >> 4;
            MP3_GET_BITRATE(ui4_bitrate, ui1_version, *pui1_layer, ui1_rate_index);            

            /* sampling frequency index */
            ui1_rate_index = ((((UINT8*)(&ui4_mp3_tag))[1] & 0xc) >> 2);
            ui2_sample_rate = 0;
            if (ui1_rate_index != 3)    /* not reserved */
            {
                ui2_sample_rate = MPEG_SAMPLERATE_TAB[ui1_rate_index];
            }
            switch (ui1_version)
            {
                case 2: /* Version 2 */
                    ui2_sample_rate >>= 1;
                    break;
                case 0: /* Version 2.5 */
                    ui2_sample_rate >>= 2;
                    break;
            }
            if (ui2_sample_rate == 0)
            {
                /* read next 1 byte */
                continue;
            }
            if(t_ps_audio_stm_attr.e_enc)
            {
                if(!(t_ps_audio_stm_attr.e_enc == (MINFO_INFO_AUDIO_ENC_T)ui1_version
                   && t_ps_audio_stm_attr.e_layer == (MINFO_INFO_AUD_LAYER_T)*pui1_layer))
                {    
                    b_const = FALSE;
                }
            }
            else
            {
                t_ps_audio_stm_attr.e_enc   = (MINFO_INFO_AUDIO_ENC_T)ui1_version;
                t_ps_audio_stm_attr.e_layer = (MINFO_INFO_AUD_LAYER_T)*pui1_layer;
            }
                
            /* padding bit */
            ui1_padding = ((((UINT8*)(&ui4_mp3_tag))[1] & 0x2)>>1);

            /* calculate frame length */
            ui4_frame_length = 0;
            if (ui4_bitrate != 0)
            {
                ui1_slot_bytes = 1;
                if (*pui1_layer == 3)         /* Layer 1 */
                {
                    ui4_frame_slots = 12;
                    ui1_slot_bytes = 4;
                }
                else if ((*pui1_layer == 1)   /* Layer 3 */
                    /* Version 2 or Version 2.5 */
                    && (ui1_version == 2 || ui1_version == 0))
                {
                    ui4_frame_slots = 72;
                }
                else
                {
                    ui4_frame_slots = 144;
                }
                if (ui2_sample_rate != 0)
                {
                    ui4_frame_length = (ui4_frame_slots * ui4_bitrate / ui2_sample_rate + ui1_padding) * ui1_slot_bytes;
                }
            }
            i4_ret = _ps_setpos(pt_ps_info,
                                pt_minfo_obj,
                                ui4_frame_length - 4,
                                MEDIA_SEEK_CUR);
            if(i4_ret != MINFOR_OK)
            {
                break;
            }
            *pb_found = (b_const?TRUE:FALSE);
            ui1_frame_count++;
        }
        i4_ret = _ps_getpos(pt_ps_info, pt_minfo_obj, &ui8_file_pos);
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
    }
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_is_handler
 *
 * Description: This function confirm media format could be handled or not.
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_is_handler(VOID*             pv_obj, 
                          MEDIA_FORMAT_T*   pt_format)
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL || pt_format == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/  
    if (pt_format->e_media_type == MEDIA_TYPE_PS)
    {
        switch (pt_format->t_media_subtype.u.e_ps_subtype)
        {
        case MEDIA_PS_SUBTYPE_MPG:
        case MEDIA_PS_SUBTYPE_DAT:
        case MEDIA_PS_SUBTYPE_VOB:
            return MINFOR_OK;
            /*break;*/
            
        default:
            return MINFOR_FMT_NOT_SUPPORT;
        }
    }

    return MINFOR_FMT_NOT_SUPPORT;
}
/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_fopen(CHAR*      ps_path_name, 
                     VOID*      pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;
    MINFO_PS_INF_T* pt_ps_info;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    
    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc(sizeof(MINFO_PS_INF_T));
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_PS_INF_T));

    pt_ps_info = pt_minfo_obj->pv_handler_obj;

    pt_ps_info->t_media_format = pt_minfo_obj->t_media_format;
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_mopen(VOID*      pv_mem_data, 
                     SIZE_T     z_mem_leng, 
                     VOID*      pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;
    MINFO_PS_INF_T* pt_ps_info;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pv_mem_data == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    
    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc(sizeof(MINFO_PS_INF_T));
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_PS_INF_T));

    pt_ps_info = pt_minfo_obj->pv_handler_obj;

    pt_ps_info->t_media_format = pt_minfo_obj->t_media_format;
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_close(VOID*  pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_parse
 *
 * Description: This function start parsing the infomation
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_parse(VOID*  pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;
    MINFO_PS_INF_T* pt_ps_info;
    UINT64          ui8_pos_bgn;
    UINT64          ui8_pos_end;
    getpos_fct      pf_get_pos;
    setpos_fct      pf_set_pos;
    UINT8           ui1_idx;
    UINT32          ui4_part_number;
    INT32           i4_ret;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_ps_info = pt_minfo_obj->pv_handler_obj;
    if (pt_ps_info == NULL)
    {
        return MINFOR_INITIAL_ERR;
    }
    
    if (NULL == pt_minfo_obj->t_input_fct_tbl.pf_get_pos
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_set_pos
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_copybytes
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_input4
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_input3
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_input2
        || NULL == pt_minfo_obj->t_input_fct_tbl.pf_input1)
    {
        return MINFOR_INITIAL_ERR;
    }
    
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    

    pf_set_pos(pt_minfo_obj, 0, MEDIA_SEEK_END);
    pf_get_pos(pt_minfo_obj, &ui8_pos_end);


    pf_set_pos(pt_minfo_obj, 0, MEDIA_SEEK_BGN);
    pf_get_pos(pt_minfo_obj, &ui8_pos_bgn);

    if (ui8_pos_bgn > ui8_pos_end)
    {
        return MINFOR_INTERNAL_ERR;
    }
    
    pt_ps_info->ui8_file_size = ui8_pos_end-ui8_pos_bgn;
    pt_ps_info->ui8_pts_min = (UINT64)((INT64)(-1));
    pt_ps_info->ui8_pts_max = 0;

    ui4_part_number = (UINT32)(pt_ps_info->ui8_file_size/(UINT64)PS_LOOKUP_LENGTH);
    
    if (ui4_part_number >= PS_LOOKUP_PART_NUMBER)
    {
        ui4_part_number = PS_LOOKUP_PART_NUMBER;
    }
    else if (ui4_part_number == 0)
    {
        ui4_part_number = 1;
    }

    for (ui1_idx=0; ui1_idx<(UINT8)ui4_part_number; ui1_idx++)
    {
        i4_ret = _ps_setpos(pt_ps_info, 
                   pt_minfo_obj,
                   (INT64)(pt_ps_info->ui8_file_size*ui1_idx/PS_LOOKUP_PART_NUMBER), 
                   MEDIA_SEEK_BGN);
        
        pt_ps_info->ui8_lookup_length = 
            PS_LOOKUP_LENGTH+pt_ps_info->ui8_file_size*ui1_idx/PS_LOOKUP_PART_NUMBER;
        if(i4_ret == MINFOR_OK)
        {
            _ps_program_stream(pt_ps_info, pt_minfo_obj);
        }
    }
    
    if (pt_ps_info->ui8_file_size > PS_DURATION_OFFSET)
    {
        i4_ret = _ps_setpos(pt_ps_info, pt_minfo_obj,(-1)*PS_DURATION_OFFSET, MEDIA_SEEK_END);
        pt_ps_info->ui8_lookup_length = pt_ps_info->ui8_file_size;
        pt_ps_info->ui8_pts_max = 0;
        if(i4_ret == MINFOR_OK)
        {
            _ps_program_stream(pt_ps_info, pt_minfo_obj);
        }
    }
    
    pt_ps_info->t_general_info.ui8_media_sz = pt_ps_info->ui8_file_size;

/*use PTS value instead of seconds*/
#if 0
    pt_ps_info->t_general_info.ui8_pb_duration = 
        _mm_div64((pt_ps_info->ui8_pts_max-pt_ps_info->ui8_pts_min),
                    PS_MPEG1_SYSTEM_CLOCK_FREQUENCY,
                    NULL);
#else
    /*use file position instead of time*/
    if (pt_ps_info->ui8_pts_max > pt_ps_info->ui8_pts_min)
    {
        pt_ps_info->t_general_info.ui8_pb_duration = pt_ps_info->ui8_pts_max-pt_ps_info->ui8_pts_min;
    }
    else
    {
        pt_ps_info->t_general_info.ui8_pb_duration = 0;
    }
    /*pt_ps_info->t_general_info.ui8_pb_duration = 0;*/
#endif

    if (pt_ps_info->ui8_pts_min == (UINT64)((INT64)(-1)))
    {
        pt_ps_info->t_general_info.ui8_preroll = 0;
    }
    else
    {
        pt_ps_info->t_general_info.ui8_preroll = pt_ps_info->ui8_pts_min;
    }
    
    pt_ps_info->t_general_info.ui1_strm_ns = 
        pt_ps_info->ui1_audio_strm_num +
        pt_ps_info->ui1_sub_picture_strm_num +
        pt_ps_info->ui1_video_strm_num;
    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_get_num_recs
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_get_num_recs(VOID*               pv_obj, 
                            MINFO_INFO_TYPE_T   e_info_type, 
                            UINT16*             pui2_num_recs)
{
    MINFO_OBJ_T*           pt_minfo_obj;
    MINFO_PS_INF_T*        pt_ps_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pui2_num_recs == NULL))
    {
        return MINFOR_INITIAL_ERR;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_ps_info = pt_minfo_obj->pv_handler_obj;
    
    if (pt_ps_info)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            return MINFOR_INFO_NOT_READY;    
        }

        switch(e_info_type)
        {
        case MINFO_INFO_TYPE_GENERIC:
            *pui2_num_recs =1;
            break;
            
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            *pui2_num_recs = pt_ps_info->ui1_video_strm_num;
            break;
            
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            *pui2_num_recs = pt_ps_info->ui1_audio_strm_num;
            break;
            
        case MINFO_INFO_TYPE_SP_ATTR:
            *pui2_num_recs = pt_ps_info->ui1_sub_picture_strm_num;
            break;
            
        case MINFO_INFO_TYPE_DRM_INFO:
            *pui2_num_recs =1;
            break;
            
        case MINFO_INFO_TYPE_META_DATA:
            *pui2_num_recs =1;
            break;

        case MINFO_INFO_TYPE_IDX_INFO:
        default:
            return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
}
                        

/*-----------------------------------------------------------------------------
 * Name:  ps_minfo_get_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_minfo_get_info(VOID*               pv_obj, 
                        MINFO_INFO_TYPE_T   e_info_type, 
                        MINFO_REC_IDX_T     ui2_index,                    
                        VOID*               pv_buf, 
                        SIZE_T*             pz_buf_leng)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_PS_INF_T*         pt_ps_info;
    MINFO_INFO_T*           pt_minfo_rec;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pz_buf_leng == NULL))
    {
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_ps_info = pt_minfo_obj->pv_handler_obj;
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;

    if (pv_buf == NULL)
    {/*get length only*/
        *pz_buf_leng = sizeof (MINFO_INFO_T);
        return MINFOR_OK;
    }
    else
    {/* get data */
        if (*pz_buf_leng < sizeof (MINFO_INFO_T))
        {
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }

    if (pt_ps_info)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            return MINFOR_INFO_NOT_READY;    
        }
                
        pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
        pt_minfo_rec->e_info_type = e_info_type;
        pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
        
        switch (e_info_type)
        {
        case MINFO_INFO_TYPE_GENERIC:
            pt_minfo_rec->u.t_general_info = pt_ps_info->t_general_info;
            break;
            
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            if (ui2_index<=pt_ps_info->ui1_video_strm_num && ui2_index>0)
            {
                *pt_minfo_rec = pt_ps_info->at_video_list[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
            }
            else
            {
                return MINFOR_INFO_NOT_FOUND;
            }
            break;
            
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            if (ui2_index<=pt_ps_info->ui1_audio_strm_num && ui2_index>0)
            {
                if (PS_MAX_AUDIO_NS <= (ui2_index-1))
                {
                    return MINFOR_INFO_NOT_FOUND;
                }
                *pt_minfo_rec = pt_ps_info->at_audio_list[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
            }
            else
            {
                return MINFOR_INFO_NOT_FOUND;
            }
            break;
            
        case MINFO_INFO_TYPE_SP_ATTR:
            if (ui2_index<=pt_ps_info->ui1_sub_picture_strm_num && ui2_index>0)
            {
                if ((ui2_index-1) >= PS_MAX_SUB_PICTURE_NS)
                {
                    return MINFOR_INFO_NOT_FOUND;
                }
                *pt_minfo_rec = pt_ps_info->at_sub_picture_list[ui2_index-1];
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
            }
            else
            {
                return MINFOR_INFO_NOT_FOUND;
            }
            break;
            
        case MINFO_INFO_TYPE_IDX_INFO:
            pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
            pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 0;
            pt_minfo_rec->u.t_idx_info.ui8_idx_sz = 0;
            pt_minfo_rec->u.t_idx_info.ui8_data_offset = 0;
            pt_minfo_rec->u.t_idx_info.ui8_data_sz = pt_ps_info->t_general_info.ui8_media_sz;
            break;
            
        case MINFO_INFO_TYPE_DRM_INFO:
            pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
        break;
        
        case MINFO_INFO_TYPE_META_DATA:
            x_memset(&pt_minfo_rec->u.t_meta_data, 0, sizeof(MINFO_TYPE_META_DATA_T));
            
            pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_rate = (UINT32)pt_ps_info->ui1_frame_rate;
            pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_scale = 1;
            
            pt_minfo_rec->u.t_meta_data.ui8_pb_duration = pt_ps_info->ui8_pts_max-pt_ps_info->ui8_pts_min;
            
            if (pt_minfo_rec->u.t_meta_data.ui8_pb_duration)
            {
                pt_minfo_rec->u.t_meta_data.ui4_bitrate = (UINT32)_mm_div64(pt_ps_info->ui8_file_size*8,
                                                                            pt_minfo_rec->u.t_meta_data.ui8_pb_duration,
                                                                            NULL);
            }

            /*video es info*/                
            pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_width = 
                pt_ps_info->at_video_list[0].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_width;
            pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_height = 
                pt_ps_info->at_video_list[0].u.t_stm_attribute.u.t_ps_video_stm_attr.i4_height;
                
            break;
    
        default:
            return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
} 

