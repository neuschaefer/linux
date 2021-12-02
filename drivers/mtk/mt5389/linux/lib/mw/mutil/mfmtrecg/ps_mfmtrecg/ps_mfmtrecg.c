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
 * $RCSfile: ps_mfmtrecg.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
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
#include "mutil/mm_util_memory.h"
#include "mutil/mm_util_input.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "../../mm_util_memory.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PS_USE_SYSTEM_HEADER            1
#define PS_USE_CACHE                    1 

#define PS_FILE_SEEK_SIZE               (1024)

#define PS_DAT_PACK_SIZE                2352
#define PS_VOB_PACK_SIZE                2048
#define PS_MPG_PACK_SIZE                2324

#define PS_LOOKUP_LENGTH                (1*1024*1024)
#define PS_LOOKUP_PACK_NUMBER           3
#define ES_LOOKUP_PICTURE_COUNT         3
#define ES_LOOKUP_LENGTH                (2*1024*1024)

#define PS_PACK_START_CODE              0x000001BA
#define PS_SYSTEM_HEADER_START_CODE     0x000001BB
#define PS_PACKET_START_CODE_PREFIX     0x000001

#define VIDEO_SEQUENCE_HEADER_CODE      0x000001B3
#define VIDEO_GROUP_PICTURE_CODE        0x000001B8
#define VIDEO_PICTURE_START_CODE        0x00000100
#define VIDEO_SLICE_START               0x00000101
#define VIDEO_SLICE_END                 0x000001AF




#define PES_END_CODE                    0x00
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

#define RIFF_HEADER                     0x52494646
#define MP4_HEADER                      0x66747970
#define RIFF_AVI                        0x41565920
#define RIFF_WAVE                       0x57415645

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID _ps_fill_media_type(MEDIA_FORMAT_T* pt_format, 
                                UINT32          ui4_pack_size, 
                                BOOL            b_const_pack);

static BOOL _ps_check_stream_id(UINT8 ui1_stream_id);
#ifndef __NO_FM__
static INT32 _ps_file_next_start_code(HANDLE_T  h_file, 
                                      UINT64*   pui8_cur_pos, 
                                      UINT32*   pui4_start_code);

static INT32 _ps_file_read(HANDLE_T     h_file, 
                           UINT8        ui1_length, 
                           UINT8*       pui1_data);

static INT32 _ps_file_seek_pack(HANDLE_T    h_file, 
                                UINT64*     pui8_cur_pos);

static INT32 _ps_file_check_format(HANDLE_T     h_file, 
                                   UINT64       ui8_cur_pos, 
                                   UINT32*      pui4_pack_size, 
                                   BOOL*        pb_const_pack, 
                                   BOOL*        pb_found);
#endif

static INT32 _ps_mem_read(UINT8*    pui1_buf, 
                          SIZE_T    z_mem_leng, 
                          UINT64    ui8_cur_pos, 
                          UINT32    ui4_length, 
                          UINT8*    pui1_data);

static INT32 _ps_mem_next_start_code(UINT8*     pui1_buf, 
                                     SIZE_T     z_mem_leng, 
                                     UINT64*    pui8_cur_pos, 
                                     UINT32*    pui4_start_code);


static INT32 _ps_mem_seek_pack(UINT8*   pui1_buf, 
                               UINT64*  pui8_cur_pos, 
                               SIZE_T   z_mem_leng);

static INT32 _ps_mem_check_format(UINT8*    pui1_buf, 
                                  UINT64    ui8_cur_pos, 
                                  SIZE_T    z_mem_leng, 
                                  UINT32*   pui4_pack_size, 
                                  BOOL*     pb_const_pack, 
                                  BOOL*     pb_found);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID _ps_fill_media_type(MEDIA_FORMAT_T*     pt_format,
                                UINT32              ui4_pack_size,
                                BOOL                b_const_pack)
{
    pt_format->e_media_type = MEDIA_TYPE_PS;
    if(b_const_pack)
    {
        switch(ui4_pack_size)
        {
        case PS_DAT_PACK_SIZE:
            pt_format->t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_DAT;
            break;
            
        case PS_VOB_PACK_SIZE:
            pt_format->t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_VOB;
            break;

        default:
            pt_format->t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_MPG;
            break;
        }
    }
    else
    {
        pt_format->t_media_subtype.u.e_ps_subtype = MEDIA_PS_SUBTYPE_MPG;
    }    
}


 
static BOOL _ps_check_stream_id(UINT8   ui1_stream_id)
{
    UINT16  ui2_stream_id;
    
    ui2_stream_id = (UINT16)ui1_stream_id;
    if((ui2_stream_id >= PES_STREAM_START) && (ui2_stream_id <= PES_STREAM_END))
    {
        return TRUE;
    }
    return FALSE;
}

#ifndef __NO_FM__
static INT32 _ps_file_next_start_code(HANDLE_T  h_file, 
                                      UINT64*   pui8_cur_pos, 
                                      UINT32*   pui4_start_code)
{
    UINT8   ui1_tmp;
    INT32   i4_ret;
    UINT32  ui4_start_code_tmp;
    UINT64  ui8_cur_pos;

    ui8_cur_pos = *pui8_cur_pos;
    
    i4_ret = _ps_file_read(h_file, 4, (UINT8 *)&ui4_start_code_tmp);
    if (i4_ret != MFMTRECGR_OK)
    {
        return i4_ret;
    }
    ui8_cur_pos += 4;

    do
    {
        if(ui8_cur_pos > PS_LOOKUP_LENGTH)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if(ui4_start_code_tmp >= 0x100)
        {
            *pui8_cur_pos = ui8_cur_pos;
            *pui4_start_code = ui4_start_code_tmp;
            return MFMTRECGR_OK;
        }
        else
        {
            i4_ret = _ps_file_read(h_file, 1, &ui1_tmp);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            ui8_cur_pos += 1;
            ui4_start_code_tmp = ((ui4_start_code_tmp<<8)|ui1_tmp);
        }
    }
    while(1);
    
    /*return MFMTRECGR_INTERNAL_ERR;*/
}

static INT32 _ps_file_read(HANDLE_T     h_file, 
                           UINT8        ui1_length, 
                           UINT8*       pui1_data)
{
    
#ifndef __NO_FM__

    UINT8   ui1_tmp;
    UINT8   ui1_idx;
    INT32   i4_ret;
    UINT32  ui4_read_count;

    i4_ret = MFMTRECGR_OK;
    for (ui1_idx=0; ui1_idx<ui1_length; ui1_idx++)
    {
        i4_ret = x_fm_read(h_file, &ui1_tmp, 1, &ui4_read_count);
        
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        if (ui4_read_count != 1)
        {
            i4_ret = MFMTRECGR_INTERNAL_ERR;
            break;
        }

        pui1_data[ui1_length-ui1_idx-1] = ui1_tmp;
    }

    return i4_ret;
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}

static INT32 _ps_file_seek_pack(HANDLE_T    h_file, 
                                UINT64*     pui8_cur_pos)
{
    
#ifndef __NO_FM__
    
#if (PS_USE_CACHE==1)    
    UINT64  ui8_file_pos;
    UINT32  ui4_read_count;
    UINT32  ui4_read_idx;
    UINT32  ui4_pack_start_code;
    INT32   i4_ret;
    UINT8   ui1_tmp;
    UINT8*  pui1_buf;


    pui1_buf = (UINT8 *)_mm_util_mem_alloc(PS_FILE_SEEK_SIZE);
    if (pui1_buf == NULL)
    {
        return MFMTRECGR_OUT_OF_MEMORY;
    }

    
    i4_ret = x_fm_lseek(h_file, (INT64)*pui8_cur_pos, FM_SEEK_BGN, &ui8_file_pos);
    if (i4_ret != FMR_OK)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_FILE_ERR;
    }

    ui4_read_idx = 0;
    i4_ret = x_fm_read(h_file, pui1_buf, PS_FILE_SEEK_SIZE, &ui4_read_count);
    if (i4_ret != FMR_OK)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_FILE_ERR;
    }
    if (ui4_read_count < 4)
    {
        _mm_util_mem_free(pui1_buf);
        return MFMTRECGR_INTERNAL_ERR;
    }
    ui4_pack_start_code = pui1_buf[ui4_read_idx++];
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_buf[ui4_read_idx++]);
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_buf[ui4_read_idx++]);
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_buf[ui4_read_idx++]);
    ui8_file_pos += 4;
    
    do
    {
        if (ui8_file_pos > PS_LOOKUP_LENGTH)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if (ui4_pack_start_code == PS_PACK_START_CODE)
        {
            _mm_util_mem_free(pui1_buf);
            i4_ret = x_fm_lseek(h_file, 
                                (INT64)(ui8_file_pos-4), 
                                FM_SEEK_BGN, 
                                pui8_cur_pos);
            
            if (i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
            return MFMTRECGR_OK;
        }
        else
        {
            if (ui4_read_idx >= ui4_read_count)
            {
                ui4_read_idx = 0;
                i4_ret = x_fm_read(h_file, 
                                   pui1_buf, 
                                   PS_FILE_SEEK_SIZE, 
                                   &ui4_read_count);
                
                if (i4_ret != FMR_OK)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_FILE_ERR;
                }

                if(ui4_read_count<1)
                {
                    _mm_util_mem_free(pui1_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }

            ui1_tmp = pui1_buf[ui4_read_idx++];
            ui8_file_pos += 1;

            ui4_pack_start_code = ((ui4_pack_start_code<<8)|ui1_tmp);
        }
        
    }while(1);

    /*_mm_util_mem_free(pui1_buf);
    return MFMTRECGR_INTERNAL_ERR;*/
#else
    UINT64  ui8_file_pos;
    UINT32  ui4_read_count;
    UINT32  ui4_pack_start_code;
    INT32   i4_ret;
    UINT8   ui1_tmp;

    i4_ret = x_fm_lseek(h_file, (INT64)*pui8_cur_pos, FM_SEEK_BGN, &ui8_file_pos);
    if (i4_ret != FMR_OK)
    {
        return MFMTRECGR_FILE_ERR;
    }


    i4_ret = _ps_file_read(h_file, 4, &ui4_pack_start_code);
    if (i4_ret != MFMTRECGR_OK)
    {
        *pui8_cur_pos = ui8_file_pos;
        return i4_ret;
    }
    ui8_file_pos += 4;

    do
    {
        if(ui8_file_pos > PS_LOOKUP_LENGTH)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if (ui4_pack_start_code == PS_PACK_START_CODE)
        {
            i4_ret = x_fm_lseek(h_file, -4, FM_SEEK_CUR, &ui8_file_pos);
            if (i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }

            *pui8_cur_pos = ui8_file_pos;
            return MFMTRECGR_OK;
        }
        else
        {
            i4_ret = _ps_file_read(h_file, 1, &ui1_tmp);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            ui8_file_pos += 1;

            ui4_pack_start_code = ((ui4_pack_start_code<<8)|ui1_tmp);
        }
        
    }while(1);

    return MFMTRECGR_INTERNAL_ERR;

#endif
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}


static INT32 _ps_file_check_format(HANDLE_T     h_file, 
                                   UINT64       ui8_cur_pos, 
                                   UINT32*      pui4_pack_size, 
                                   BOOL*        pb_const_pack, 
                                   BOOL*        pb_found)
{

#ifndef __NO_FM__

    UINT64  ui8_pre_file_pos;
    UINT64  ui8_file_pos;
    UINT64  ui8_file_pos_tmp;
    UINT32  ui4_pack_start_code;
    UINT32  ui4_system_header_start_code;
    UINT32  ui4_packet_start_code_prefix;
    INT32   i4_ret;
    INT32   i4_ret_tmp;
    UINT16  ui2_system_header_length;
    UINT16  ui2_pes_packet_length;
    UINT8   ui1_pack_count;
    UINT8   ui1_tmp;
    UINT8   ui1_stream_id;
    BOOL    b_system_header;
    BOOL    b_mpeg2 = FALSE;
    
    b_system_header = FALSE;
    *pb_found = FALSE;
    *pb_const_pack = TRUE;
    
    ui8_file_pos = ui8_cur_pos;

    ui1_pack_count = 0;
    do
    {
        if(ui8_file_pos > PS_LOOKUP_LENGTH)
        {
            i4_ret = MFMTRECGR_INTERNAL_ERR;
            break;
        }
#if (PS_USE_SYSTEM_HEADER == 1)
        if(b_system_header || (ui1_pack_count>PS_LOOKUP_PACK_NUMBER))
#else
        if(ui1_pack_count>PS_LOOKUP_PACK_NUMBER)
#endif
        {
            *pb_found = TRUE;
            i4_ret = MFMTRECGR_OK;
            break;
        }

        ui8_pre_file_pos = ui8_file_pos;
        
        /*pack header*/
        i4_ret = _ps_file_next_start_code(h_file, 
                                          &ui8_file_pos, 
                                          &ui4_pack_start_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        
        if (ui4_pack_start_code != PS_PACK_START_CODE)
        {
            *pb_found = FALSE;
            i4_ret = MFMTRECGR_OK;
            break;
        }

        ui1_pack_count++;
        
        /*check mpeg1 or mpeg2*/
        i4_ret = _ps_file_read(h_file, 1, &ui1_tmp);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        ui8_file_pos += 1;
        
        if ((ui1_tmp & 0xf0) == 0x20)
        {
            /*mpeg1*/
            b_mpeg2 = FALSE;
            i4_ret = x_fm_lseek(h_file, 7, FM_SEEK_CUR, &ui8_file_pos);
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_FILE_ERR;
                break;
            }

        }
        else if ((ui1_tmp & 0xC0) == 0x40)
        { 
            /*mpeg2*/
            b_mpeg2 = TRUE;
            i4_ret = x_fm_lseek(h_file, 8, FM_SEEK_CUR, &ui8_file_pos);
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_FILE_ERR;
                break;
            }

            i4_ret = _ps_file_read(h_file, 1, &ui1_tmp);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            ui8_file_pos += 1;

            /*stuffing_byte*/
            ui1_tmp &= 0x07;
            i4_ret = x_fm_lseek(h_file, (INT64)ui1_tmp, FM_SEEK_CUR, &ui8_file_pos);
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_FILE_ERR;
                break;
            }

        }
        else
        {
            *pb_found = FALSE;
            i4_ret = MFMTRECGR_OK;
            break;
        }
        
        
        /*check system header*/
        ui8_file_pos_tmp = ui8_file_pos;
        i4_ret = _ps_file_next_start_code(h_file, 
                                          &ui8_file_pos, 
                                          &ui4_system_header_start_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }

        if (PS_SYSTEM_HEADER_START_CODE != ui4_system_header_start_code)
        {
            i4_ret = x_fm_lseek(h_file, 
                                (INT64)ui8_file_pos_tmp, 
                                FM_SEEK_BGN, 
                                &ui8_file_pos);
            
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_FILE_ERR;
                break;
            }
        }
        else
        {
            /*skip system header*/
            b_system_header = TRUE;
            
            i4_ret = _ps_file_read(h_file, 
                                   2, 
                                   (UINT8 *)&ui2_system_header_length);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            ui8_file_pos += 2;
            
            i4_ret = x_fm_lseek(h_file, 
                                (INT64)ui2_system_header_length, 
                                FM_SEEK_CUR, 
                                &ui8_file_pos);
            
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_FILE_ERR;
                break;
            }
        }

        do
        {
            /*PES_packet*/
            ui8_file_pos_tmp = ui8_file_pos;
            i4_ret = _ps_file_next_start_code(h_file, 
                                              &ui8_file_pos, 
                                              &ui4_packet_start_code_prefix);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            ui1_stream_id = (UINT8)(ui4_packet_start_code_prefix & 0xFF);
            ui4_packet_start_code_prefix = (ui4_packet_start_code_prefix>>8);
                
            if ((PS_PACKET_START_CODE_PREFIX != ui4_packet_start_code_prefix)
                || !_ps_check_stream_id(ui1_stream_id))
            {
                i4_ret = x_fm_lseek(h_file, 
                                    (INT64)ui8_file_pos_tmp, 
                                    FM_SEEK_BGN, 
                                    &ui8_file_pos);
                if (i4_ret != FMR_OK)
                {
                    i4_ret = MFMTRECGR_FILE_ERR;
                    break;
                }
                break;
            }
            else
            {
                /*skip PES data*/
                i4_ret = _ps_file_read(h_file, 
                                       2, 
                                       (UINT8 *)&ui2_pes_packet_length);
                if (i4_ret != MFMTRECGR_OK)
                {
                    break;
                }
                ui8_file_pos += 2;
                
                i4_ret = x_fm_lseek(h_file, 
                                    (INT64)ui2_pes_packet_length, 
                                    FM_SEEK_CUR, 
                                    &ui8_file_pos);
                if (i4_ret != FMR_OK)
                {
                    i4_ret = MFMTRECGR_FILE_ERR;
                    break;
                }
            }

            i4_ret = _ps_file_seek_pack(h_file, &ui8_file_pos);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            
            if(1 == ui1_pack_count)
            {
                *pui4_pack_size = (UINT32)(ui8_file_pos - ui8_pre_file_pos);
            }
            else
            {
                if (*pui4_pack_size != (UINT32)(ui8_file_pos-ui8_pre_file_pos))
                {
                    *pb_const_pack = FALSE;
                }
            }
            
            if(((*pui4_pack_size) == PS_DAT_PACK_SIZE) && b_mpeg2)
            {
               *pb_const_pack = FALSE; /*mpeg2 can not coexit with DAT subtype*/
            }
        }while(1);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        
    }while(1);

    i4_ret_tmp = x_fm_lseek(h_file, (INT64)ui8_cur_pos, FM_SEEK_BGN, &ui8_file_pos);
    if (i4_ret_tmp != FMR_OK)
    {
        return MFMTRECGR_FILE_ERR;
    }

    return i4_ret;
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}
#endif

static INT32 _ps_mem_read(UINT8*    pui1_buf, 
                          SIZE_T    z_mem_leng, 
                          UINT64    ui8_cur_pos, 
                          UINT32    ui4_length, 
                          UINT8*    pui1_data)
{
    UINT32          ui4_idx;
    UINT8           ui1_read;
    MEDIA_FEEDER_T* pt_src_feeder;

    pt_src_feeder = (MEDIA_FEEDER_T *)pui1_buf;
    if (0 == z_mem_leng)
    {
        pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                      (INT64)ui8_cur_pos,
                                                      MEDIA_SEEK_BGN);

    }

    for (ui4_idx=0; ui4_idx<ui4_length; ui4_idx++)
    {
        if ((ui8_cur_pos+ui4_idx) > PS_LOOKUP_LENGTH)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        if (0 == z_mem_leng)
        {
            INT32 i4_ret;
            
            i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_input1(&pt_src_feeder->h_feeder, 
                                                                  &ui1_read);
            if (i4_ret != INPUTR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            pui1_data[ui4_length-ui4_idx-1] = ui1_read;
        }
        else
        {
            if (z_mem_leng)
            {
                if ((ui8_cur_pos+ui4_idx) > z_mem_leng)
                {
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            pui1_data[ui4_length-ui4_idx-1] = pui1_buf[ui8_cur_pos+ui4_idx];
        }
    }

    return MFMTRECGR_OK;
}

static INT32 _ps_mem_next_start_code(UINT8*     pui1_buf, 
                                     SIZE_T     z_mem_leng, 
                                     UINT64*    pui8_cur_pos, 
                                     UINT32*    pui4_start_code)
{
    UINT8   ui1_tmp;
    INT32   i4_ret;
    UINT32  ui4_start_code_tmp;
    UINT64  ui8_cur_pos;

    ui8_cur_pos = *pui8_cur_pos;

    i4_ret = _ps_mem_read(pui1_buf, 
                          z_mem_leng, 
                          ui8_cur_pos, 
                          4, 
                          (UINT8 *)&ui4_start_code_tmp);
    if (i4_ret != MFMTRECGR_OK)
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    ui8_cur_pos += 4;

    do
    {   
        if(ui4_start_code_tmp >= 0x100)
        {
            *pui8_cur_pos = ui8_cur_pos;
            *pui4_start_code = ui4_start_code_tmp;
            return MFMTRECGR_OK;
        }
        else
        {
            i4_ret = _ps_mem_read(pui1_buf, 
                                  z_mem_leng, 
                                  ui8_cur_pos, 
                                  1, 
                                  &ui1_tmp);
            if (i4_ret != MFMTRECGR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui8_cur_pos += 1;
            ui4_start_code_tmp = ((ui4_start_code_tmp<<8)|ui1_tmp);
        }
    }
    while(1);
    
    /*return MFMTRECGR_INTERNAL_ERR;*/
}

static INT32 _ps_mem_seek_pack(UINT8*   pui1_buf, 
                               UINT64*  pui8_cur_pos, 
                               SIZE_T   z_mem_leng)
{

    UINT64  ui8_file_pos;
    UINT32  ui4_read_count;
    UINT32  ui4_read_idx;
    UINT32  ui4_pack_start_code;
    INT32   i4_ret;
    UINT8   ui1_tmp;
    UINT8*  pui1_data;
    
    MEDIA_FEEDER_T* pt_src_feeder;

    pt_src_feeder = (MEDIA_FEEDER_T *)pui1_buf;

    pui1_data = (UINT8 *)_mm_util_mem_alloc(PS_FILE_SEEK_SIZE);
    if (pui1_data == NULL)
    {
        return MFMTRECGR_OUT_OF_MEMORY;
    }

    if (0 == z_mem_leng)
    {
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                               (INT64)*pui8_cur_pos,
                                                               MEDIA_SEEK_BGN);
        if (i4_ret != FMR_OK)
        {
            _mm_util_mem_free(pui1_data);
            return MFMTRECGR_INTERNAL_ERR;
        }
    }

    ui8_file_pos = *pui8_cur_pos;
    ui4_read_idx = 0;

    i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(&pt_src_feeder->h_feeder,
                                                             pui1_data,
                                                             PS_FILE_SEEK_SIZE,
                                                             PS_FILE_SEEK_SIZE,
                                                             &ui4_read_count);

    if (i4_ret != MFMTRECGR_OK)
    {
        _mm_util_mem_free(pui1_data);
        return MFMTRECGR_INTERNAL_ERR;
    }

    ui4_pack_start_code = pui1_data[ui4_read_idx++];
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_data[ui4_read_idx++]);
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_data[ui4_read_idx++]);
    ui4_pack_start_code = ((ui4_pack_start_code<<8)|pui1_data[ui4_read_idx++]);
    ui8_file_pos += 4;
    
    do
    {
        if (ui8_file_pos > PS_LOOKUP_LENGTH)
        {
            _mm_util_mem_free(pui1_data);
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if (ui4_pack_start_code == PS_PACK_START_CODE)
        {
            _mm_util_mem_free(pui1_data);

            if (0 == z_mem_leng)
            {
                i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                                       (INT64)(ui8_file_pos-4),
                                                                       MEDIA_SEEK_BGN);
                if (i4_ret != FMR_OK)
                {
                    _mm_util_mem_free(pui1_data);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }
            *pui8_cur_pos = ui8_file_pos-4;
            
            return MFMTRECGR_OK;
        }
        else
        {
            if (ui4_read_idx >= ui4_read_count)
            {
                if(i4_ret == INPUTR_EOF)
                {
                    _mm_util_mem_free(pui1_data);
                    return MFMTRECGR_INTERNAL_ERR;
                }
                ui4_read_idx = 0;

                i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(&pt_src_feeder->h_feeder,
                                                                         pui1_data,
                                                                         PS_FILE_SEEK_SIZE,
                                                                         PS_FILE_SEEK_SIZE,
                                                                         &ui4_read_count);
                if(i4_ret == INPUTR_EOF)
                {
                    //do nothing; waiting for current read data processed
                }
                else if (i4_ret != MFMTRECGR_OK)
                {
                    _mm_util_mem_free(pui1_data);
                    return MFMTRECGR_INTERNAL_ERR;
                }
            }

            ui1_tmp = pui1_data[ui4_read_idx++];
            ui8_file_pos += 1;

            ui4_pack_start_code = ((ui4_pack_start_code<<8)|ui1_tmp);
        }
        
    }while(1);
}


static INT32 _ps_mem_check_format(UINT8*    pui1_buf, 
                                  UINT64    ui8_cur_pos, 
                                  SIZE_T    z_mem_leng, 
                                  UINT32*   pui4_pack_size, 
                                  BOOL*     pb_const_pack, 
                                  BOOL*     pb_found)
{
    UINT64  ui8_pre_file_pos;
    UINT64  ui8_file_pos;
    UINT64  ui8_file_pos_tmp;
    UINT32  ui4_pack_start_code;
    UINT32  ui4_system_header_start_code;
    UINT32  ui4_packet_start_code_prefix;
    INT32   i4_ret;
    UINT16  ui2_system_header_length;
    UINT16  ui2_pes_packet_length;
    UINT8   ui1_pack_count;
    UINT8   ui1_tmp;
    UINT8   ui1_stream_id;
    BOOL    b_system_header = FALSE;

    *pb_found = FALSE;
    *pb_const_pack = TRUE;

    ui8_file_pos = ui8_cur_pos;

    ui1_pack_count = 0;
    do
    {
#if (PS_USE_SYSTEM_HEADER == 1)
        if(b_system_header || (ui1_pack_count>PS_LOOKUP_PACK_NUMBER))
#else
        if(ui1_pack_count>PS_LOOKUP_PACK_NUMBER)
#endif

        {
            *pb_found = TRUE;
            i4_ret = MFMTRECGR_OK;
            break;
        }

        ui8_pre_file_pos = ui8_file_pos;
        
        /*pack header*/
        i4_ret = _ps_mem_next_start_code(pui1_buf, 
                                         z_mem_leng, 
                                         &ui8_file_pos, 
                                         &ui4_pack_start_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        
        if (ui4_pack_start_code != PS_PACK_START_CODE)
        {
            *pb_found = FALSE;
            i4_ret = MFMTRECGR_OK;
            break;
        }

        ui1_pack_count++;
        
        /*check mpeg1 or mpeg2*/
        i4_ret = _ps_mem_read(pui1_buf, z_mem_leng, ui8_file_pos, 1, &ui1_tmp);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        ui8_file_pos += 1;
        
        if ((ui1_tmp & 0xf0) == 0x20)
        {
            /*mpeg1*/
            ui8_file_pos += 7;

        }
        else if ((ui1_tmp & 0xC0) == 0x40)
        { 
            /*mpeg2*/
            ui8_file_pos += 8;

            i4_ret = _ps_mem_read(pui1_buf, 
                                  z_mem_leng, 
                                  ui8_file_pos, 
                                  1, 
                                  &ui1_tmp);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            ui8_file_pos += 1;

            /*stuffing_byte*/
            ui1_tmp &= 0x07;
            ui8_file_pos += ui1_tmp;
        }
        else
        {
            *pb_found = FALSE;
            i4_ret = MFMTRECGR_OK;
            break;
        }
        
        
        /*check system header*/
        ui8_file_pos_tmp = ui8_file_pos;
        i4_ret = _ps_mem_next_start_code(pui1_buf, 
                                         z_mem_leng, 
                                         &ui8_file_pos, 
                                         &ui4_system_header_start_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }

        if (PS_SYSTEM_HEADER_START_CODE != ui4_system_header_start_code)
        {
            ui8_file_pos = ui8_file_pos_tmp;
        }
        else
        {
            /*skip system header*/
            b_system_header = TRUE;

            i4_ret = _ps_mem_read(pui1_buf, 
                                  z_mem_leng, 
                                  ui8_file_pos, 
                                  2, 
                                  (UINT8 *)&ui2_system_header_length);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            ui8_file_pos += 2;

            ui8_file_pos += ui2_system_header_length;
        }

        do
        {
            /*PES_packet*/
            ui8_file_pos_tmp = ui8_file_pos;
            i4_ret = _ps_mem_next_start_code(pui1_buf, 
                                             z_mem_leng, 
                                             &ui8_file_pos, 
                                             &ui4_packet_start_code_prefix);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            ui1_stream_id = (UINT8)(ui4_packet_start_code_prefix & 0xFF);
            ui4_packet_start_code_prefix = (ui4_packet_start_code_prefix>>8);
                
            if ((PS_PACKET_START_CODE_PREFIX != ui4_packet_start_code_prefix)
                || !_ps_check_stream_id(ui1_stream_id))
            {
                ui8_file_pos = ui8_file_pos_tmp;
                break;
            }
            else
            {
                /*skip PES data*/
                i4_ret = _ps_mem_read(pui1_buf, 
                                      z_mem_leng, 
                                      ui8_file_pos, 
                                      2, 
                                      (UINT8 *)&ui2_pes_packet_length);
                if (i4_ret != MFMTRECGR_OK)
                {
                    break;
                }
                ui8_file_pos += 2;

                ui8_file_pos += ui2_pes_packet_length;
            }
            
            i4_ret = _ps_mem_seek_pack((UINT8 *)pui1_buf, 
                                       &ui8_file_pos, 
                                       z_mem_leng);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }
            
            if(1 == ui1_pack_count)
            {
                *pui4_pack_size = (UINT32)(ui8_file_pos - ui8_pre_file_pos);
            }
            else
            {
                if (*pui4_pack_size != (UINT32)(ui8_file_pos-ui8_pre_file_pos))
                {
                    *pb_const_pack = FALSE;
                }
            }
        }while(1);
        if(i4_ret != MFMTRECGR_OK)
        {
            break;
        }
        
    }while(1);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  ps_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler 
 *              which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_handler_file_confirm_fct(HANDLE_T              h_file,        
                                  MEDIA_FORMAT_T*       pt_format)     
{

#ifndef __NO_FM__

    UINT64  ui8_cur_pos;
    INT32   i4_ret;
    UINT32  ui4_pack_size = 0;
    BOOL    b_found=FALSE;
    BOOL    b_const_pack;
    UINT32  ui4_file_head;
    BOOL    b_riff;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || (h_file == NULL_HANDLE))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*parse PS first*/
    do
    {
        i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }


        i4_ret = _ps_file_read(h_file, 4, (UINT8*)&ui4_file_head);
        if (i4_ret != MFMTRECGR_OK)
        {
            return i4_ret;
        }

        if (ui4_file_head == RIFF_HEADER)
        {
            b_riff = TRUE;
        }
        else
        {
            b_riff = FALSE;
        }
        

        i4_ret = _ps_file_read(h_file, 4, (UINT8*)&ui4_file_head);
        if (i4_ret != MFMTRECGR_OK)
        {
            return i4_ret;
        }

        if (ui4_file_head == MP4_HEADER)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        if (b_riff)
        {
            i4_ret = _ps_file_read(h_file, 4, (UINT8*)&ui4_file_head);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }

            if ((ui4_file_head == RIFF_AVI) || (ui4_file_head == RIFF_WAVE))
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        }
        
        i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        while (1)
        {
            i4_ret = _ps_file_seek_pack(h_file, &ui8_cur_pos);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            i4_ret = _ps_file_check_format(h_file, 
                                           ui8_cur_pos, 
                                           &ui4_pack_size, 
                                           &b_const_pack, 
                                           &b_found);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            if (b_found)
            {
                _ps_fill_media_type(pt_format, ui4_pack_size, b_const_pack);
                break;
            }

            i4_ret = x_fm_lseek(h_file, 1, FM_SEEK_CUR, &ui8_cur_pos);
            if (i4_ret != FMR_OK)
            {
                i4_ret = MFMTRECGR_INTERNAL_ERR;
                break;
            }

            if (ui8_cur_pos > PS_LOOKUP_LENGTH)
            {
                i4_ret = MFMTRECGR_INTERNAL_ERR;
                break;
            }
        }

    }while(0);
    
    return i4_ret;
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}

                            
/*-----------------------------------------------------------------------------
 * Name:  ps_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer 
 *                          which want to retrieve the format.
 *
 *          z_mem_leng      specify the length of memory buffer 
 *                          which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ps_handler_mem_confirm_fct(const VOID*        pv_mem_data, 
                                 SIZE_T             z_mem_leng,   
                                 MEDIA_FORMAT_T*    pt_format) 
{
    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    UINT32              ui4_pack_size = 0;
    BOOL                b_found=FALSE;
    BOOL                b_const_pack;
    MEDIA_FEEDER_T*     pt_src_feeder;
    UINT32              ui4_file_head;
    BOOL                b_riff;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || (pv_mem_data == NULL))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }

    pt_src_feeder = (MEDIA_FEEDER_T *)pv_mem_data;
    if (0 == z_mem_leng)
    {
        if (NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_input4
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_input3
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_input2
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_input1
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos
            || NULL == pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    do
    {
        ui8_cur_pos = 0;

        i4_ret = _ps_mem_read((UINT8 *)pv_mem_data, 
                              z_mem_leng, 
                              0, 
                              4, 
                              (UINT8 *)&ui4_file_head);
        if (i4_ret != MFMTRECGR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        if (ui4_file_head == RIFF_HEADER)
        {
            b_riff = TRUE;
        }
        else
        {
            b_riff = FALSE;
        }

        i4_ret = _ps_mem_read((UINT8 *)pv_mem_data, 
                              z_mem_leng, 
                              4, 
                              4, 
                              (UINT8 *)&ui4_file_head);
        if (i4_ret != MFMTRECGR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        if (ui4_file_head == MP4_HEADER)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }

        if (b_riff)
        {
            i4_ret = _ps_mem_read((UINT8 *)pv_mem_data, 
                                  z_mem_leng, 
                                  8, 
                                  4, 
                                  (UINT8 *)&ui4_file_head);
            if (i4_ret != MFMTRECGR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }

            if ((ui4_file_head == RIFF_AVI) || (ui4_file_head == RIFF_WAVE))
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        }

        /*parse PS first*/
        while (1)
        {
            i4_ret = _ps_mem_seek_pack((UINT8 *)pv_mem_data, 
                                       &ui8_cur_pos, 
                                       z_mem_leng);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            i4_ret = _ps_mem_check_format((UINT8 *)pv_mem_data, 
                                          ui8_cur_pos, 
                                          z_mem_leng, 
                                          &ui4_pack_size, 
                                          &b_const_pack, 
                                          &b_found);
            if (i4_ret != MFMTRECGR_OK)
            {
                break;
            }

            if (b_found)
            {
                _ps_fill_media_type(pt_format, ui4_pack_size, b_const_pack);
                break;
            }

            ui8_cur_pos += 1;
            
            if (ui8_cur_pos > PS_LOOKUP_LENGTH)
            {
                i4_ret = MFMTRECGR_INTERNAL_ERR;
                break;
            }

            if (ui8_cur_pos >= z_mem_leng)
            {
                i4_ret = MFMTRECGR_INTERNAL_ERR;
                break;
            }
        }
    }while(0);

    return i4_ret;
}

