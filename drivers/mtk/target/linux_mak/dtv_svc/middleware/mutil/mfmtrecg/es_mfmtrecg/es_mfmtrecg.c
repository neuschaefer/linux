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

#define PS_LOOKUP_LENGTH                (500*1024)
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

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
#ifndef __NO_FM__
static INT32 _es_file_seek_start_code(HANDLE_T      h_file, 
                                      UINT64*       pui8_cur_pos,
                                      UINT32*       pui4_code)
{
    UINT64  ui8_file_pos;
    UINT32  ui4_read_count;
    UINT32  ui4_read_idx;
    UINT32  ui4_tmp_code;
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
    ui4_tmp_code = pui1_buf[ui4_read_idx++];
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_buf[ui4_read_idx++]);
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_buf[ui4_read_idx++]);
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_buf[ui4_read_idx++]);
    ui8_file_pos += 4;
    
    do
    {
        if (ui8_file_pos > ES_LOOKUP_LENGTH)
        {
            _mm_util_mem_free(pui1_buf);
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if ((ui4_tmp_code>>8) == PS_PACKET_START_CODE_PREFIX)
        {
            *pui4_code = ui4_tmp_code;
            
            _mm_util_mem_free(pui1_buf);

            *pui8_cur_pos = ui8_file_pos;
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

            ui4_tmp_code = ((ui4_tmp_code<<8)|ui1_tmp);
        }
        
    }while(1);

    /*_mm_util_mem_free(pui1_buf);
    return MFMTRECGR_INTERNAL_ERR;*/

    return MFMTRECGR_INTERNAL_ERR;
}

static INT32 _es_file_check_format(HANDLE_T     h_file, 
                                   UINT64       ui8_cur_pos,
                                   BOOL*        pb_found)
{
    INT32   i4_ret;
    UINT32  ui4_code;
    UINT64  ui8_pre_file_pos;
    UINT64  ui8_file_pos;
    
    BOOL    b_gop;
    BOOL    b_picture;
    ui8_file_pos = ui8_cur_pos;

    do
    {
        i4_ret = _es_file_seek_start_code(h_file, &ui8_file_pos, &ui4_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            return i4_ret;
        }
    
        if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
        {
            break;
        }
    }while(1);
    
    do
    {
        b_gop = FALSE;
        b_picture = FALSE;
        
        ui8_pre_file_pos = ui8_file_pos;
        do
        {
            i4_ret = _es_file_seek_start_code(h_file, &ui8_file_pos, &ui4_code);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            if (VIDEO_GROUP_PICTURE_CODE==ui4_code)
            {
                b_gop = TRUE;
            }
            else if (VIDEO_PICTURE_START_CODE==ui4_code)
            {
                b_picture = TRUE;
            }
            else if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
            {
                break;
            }
            
            if (b_gop && b_picture)
            {
                *pb_found = TRUE;
                return MFMTRECGR_OK;
            }
            
            if (ES_LOOKUP_LENGTH < (ui8_file_pos-ui8_pre_file_pos))
            {
                return MFMTRECGR_INV_ARG;
            }
            
        }while(1);
    }while(1);
}
#endif

static INT32 _es_mem_seek_start_code(UINT8*   pui1_buf, 
                                     UINT64*  pui8_cur_pos, 
                                     SIZE_T   z_mem_leng,
                                     UINT32*  pui4_code)
{
    UINT64  ui8_file_pos;
    UINT32  ui4_read_count;
    UINT32  ui4_read_idx;
    UINT32  ui4_tmp_code;
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
    
    ui4_read_idx = 0;
    
    ui4_tmp_code = pui1_data[ui4_read_idx++];
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_data[ui4_read_idx++]);
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_data[ui4_read_idx++]);
    ui4_tmp_code = ((ui4_tmp_code<<8)|pui1_data[ui4_read_idx++]);
    ui8_file_pos += 4;
    
    do
    {
        if (ui8_file_pos > ES_LOOKUP_LENGTH)
        {
            _mm_util_mem_free(pui1_data);
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        if ((ui4_tmp_code>>8) == PS_PACKET_START_CODE_PREFIX)
        {
            *pui4_code = ui4_tmp_code;
            
            _mm_util_mem_free(pui1_data);

            *pui8_cur_pos = ui8_file_pos;
            return MFMTRECGR_OK;
        }
        else
        {
            if (ui4_read_idx >= ui4_read_count)
            {
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
                
            }

            ui1_tmp = pui1_data[ui4_read_idx++];
            ui8_file_pos += 1;

            ui4_tmp_code = ((ui4_tmp_code<<8)|ui1_tmp);
        }
        
    }while(1);
}


static INT32 _es_mem_check_format(UINT8*    pui1_buf, 
                                  UINT64    ui8_cur_pos, 
                                  SIZE_T    z_mem_leng,
                                  BOOL*     pb_found)
{
    INT32   i4_ret;
    UINT32  ui4_code;
    UINT64  ui8_pre_file_pos;
    UINT64  ui8_file_pos;
    
    BOOL    b_gop;
    BOOL    b_picture;
    ui8_file_pos = ui8_cur_pos;

    do
    {
        i4_ret = _es_mem_seek_start_code(pui1_buf, &ui8_file_pos, z_mem_leng, &ui4_code);
        if (i4_ret != MFMTRECGR_OK)
        {
            return i4_ret;
        }
    
        if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
        {
            break;
        }
    }while(1);
    
    do
    {
        b_gop = FALSE;
        b_picture = FALSE;
        
        ui8_pre_file_pos = ui8_file_pos;
        do
        {
            i4_ret = _es_mem_seek_start_code(pui1_buf, &ui8_file_pos, z_mem_leng, &ui4_code);
            if (i4_ret != MFMTRECGR_OK)
            {
                return i4_ret;
            }
            if (VIDEO_GROUP_PICTURE_CODE==ui4_code)
            {
                b_gop = TRUE;
            }
            else if (VIDEO_PICTURE_START_CODE==ui4_code)
            {
                b_picture = TRUE;
            }
            else if (VIDEO_SEQUENCE_HEADER_CODE == ui4_code)
            {
                break;
            }
            
            if (b_gop && b_picture)
            {
                *pb_found = TRUE;
                return MFMTRECGR_OK;
            }
            
            if (ES_LOOKUP_LENGTH < (ui8_file_pos-ui8_pre_file_pos))
            {
                return MFMTRECGR_INV_ARG;
            }
            
        }while(1);
    }while(1);
}


/*-----------------------------------------------------------------------------
 * Name:  es_handler_file_confirm_fct
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
INT32 es_handler_file_confirm_fct(HANDLE_T              h_file,        
                                  MEDIA_FORMAT_T*       pt_format)     
{

#ifndef __NO_FM__

    UINT64  ui8_cur_pos;
    INT32   i4_ret;
    BOOL    b_found=FALSE;

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
    do
    {
        i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        i4_ret = _es_file_check_format(h_file, 
                                       ui8_cur_pos,
                                       &b_found);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }

        if (b_found && (i4_ret == MFMTRECGR_OK))
        {
            pt_format->e_media_type = MEDIA_TYPE_VIDEO_ES;
            pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M2V;
        }
        else
        {
            i4_ret = MFMTRECGR_INTERNAL_ERR;
        }
    }while(0);
    
    return i4_ret;
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}

                            
/*-----------------------------------------------------------------------------
 * Name:  es_handler_mem_confirm_fct
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
INT32 es_handler_mem_confirm_fct(const VOID*        pv_mem_data, 
                                 SIZE_T             z_mem_leng,   
                                 MEDIA_FORMAT_T*    pt_format) 
{
    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    BOOL                b_found=FALSE;
    MEDIA_FEEDER_T*     pt_src_feeder;
    
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

        i4_ret = _es_mem_check_format((UINT8 *)pv_mem_data, 
                                      ui8_cur_pos, 
                                      z_mem_leng,
                                      &b_found);
        if (i4_ret != MFMTRECGR_OK)
        {
            break;
        }

        if (b_found && (i4_ret == MFMTRECGR_OK))
        {
            pt_format->e_media_type = MEDIA_TYPE_VIDEO_ES;
            pt_format->t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_M2V;
        }
        else
        {
            i4_ret = MFMTRECGR_INTERNAL_ERR;
        }
    }while(0);

    return i4_ret;
}

