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
 * $RCSfile: mp3_id3_mfmtrecg.c,v $
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

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*const static CHAR auc_TAG[] = "TAG";*/
const static CHAR auc_ID3[] = "ID3";

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  mp3_id3_read_n_byte_to_buf
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
static INT32 mp3_id3_read_n_byte_to_buf(
                                    HANDLE_T        h_file,
                                    const VOID*     pv_mem_data,
                                    SIZE_T          z_mem_leng,
                                    INT64           i8_offset,
                                    VOID*           pv_output_buff,
                                    UINT16          ui2_read_size,
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
static INT32 mp3_id3_check_format(HANDLE_T            h_file,
                              const VOID*         pv_mem_data,
                              SIZE_T              z_mem_leng,
                              MEDIA_FORMAT_T*     pt_format,
                              BOOL                b_is_mem)
{
    UCHAR     ac_tmp[15];
    UINT32    ui4_read;
    INT32     i4_ret = 0;
    UINT32    ui4_id3v2_offset = 0;
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
        if((i4_ret = mp3_id3_read_n_byte_to_buf(h_file,
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

    if((i4_ret = mp3_id3_read_n_byte_to_buf(h_file,
                              pv_mem_data,
                              z_mem_leng,
                              (INT64)ui4_id3v2_offset,
                              ac_tmp,
                              4,
                              &ui4_read,
                              b_is_mem)) != MFMTRECGR_OK)
    {
        return i4_ret;
    }


    if ((ac_tmp[0] != 0xFF)
        || ((ac_tmp[1] & 0xE0) != 0xE0)
        || ((ac_tmp[1] & 0x18) >> 3 == 1)      /* version description is set to reserved */
        || ((ac_tmp[1] & 0x06) >> 1 == 0)      /* layer description is set to reserved */
        || ((ac_tmp[2] >> 4) == 0)
        || ((ac_tmp[2] >> 4) == 15))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }    

     pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
     pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;

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
INT32 mp3_id3_handler_file_confirm_fct(
                                    HANDLE_T           h_file,
                                    MEDIA_FORMAT_T*    pt_format)
{

#ifndef __NO_FM__
    return mp3_id3_check_format(h_file, NULL, 0, pt_format, FALSE);
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
INT32 mp3_id3_handler_mem_confirm_fct(
                        const VOID*       pv_mem_data,
                        SIZE_T            z_mem_leng,
                        MEDIA_FORMAT_T*   pt_format)
{
    return mp3_id3_check_format(NULL_HANDLE, pv_mem_data, z_mem_leng, pt_format, TRUE);
}




