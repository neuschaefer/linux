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
 * $RCSfile: avi_mfmtrecg.c,v $
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
#include "x_mm_common.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* const static UCHAR auc_AVI_HDR[] = "RIFF"; */
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
 * Name:  avi_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_handler_file_confirm_fct(
                            HANDLE_T                    h_file,        
                            MEDIA_FORMAT_T*             pt_format)     
{

#ifndef __NO_FM__

    UINT64 ui8_cur_pos;
    INT32  i4_ret;
    CHAR   ac_avi_hdr[8];
    UINT32 ui4_read;
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
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if (i4_ret == FMR_OK)
    {
        i4_ret = x_fm_read(h_file, ac_avi_hdr, 4, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            return MFMTRECGR_FILE_ERR;
        }
        if (x_memcmp((VOID*)"RIFF", ac_avi_hdr, 4) == 0)
        {
            /* file size */
            i4_ret = x_fm_read(h_file, ac_avi_hdr, 4, &ui4_read);
            if (i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
            /* file format "AVI " */
            i4_ret = x_fm_read(h_file, ac_avi_hdr, 4, &ui4_read);
            if (i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
            if (x_memcmp((VOID*)"AVI ", ac_avi_hdr, 4) == 0)
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype=MEDIA_CONTNR_SUBTYPE_AVI;
                return MFMTRECGR_OK;
            }
            else if (x_memcmp((VOID*)"WAVE", ac_avi_hdr, 4) == 0)
            {   /*add for wave support*/
                i4_ret = x_fm_lseek(h_file, 20, FM_SEEK_BGN, &ui8_cur_pos);
                if (i4_ret == FMR_OK)
                {
                    i4_ret = x_fm_read(h_file, ac_avi_hdr, 4, &ui4_read);
                    if((i4_ret == FMR_OK) && (ac_avi_hdr[0] != 85)) 
                    {   /*it's not mp3 format, if wave has mp3 format, use mp3 recognizer process it*/
                        pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                        pt_format->t_media_subtype.u.e_aud_subtype=MEDIA_AUD_SUBTYPE_WAVE;
                        return MFMTRECGR_OK;
                    }
                    else if(i4_ret != FMR_OK)
                    {
                        return MFMTRECGR_FILE_ERR;
                    }
                }
                else
                {
                    return MFMTRECGR_FILE_ERR;
                }
            }
        }
        else if((x_memcmp((VOID*)"DTSH", ac_avi_hdr, 4) == 0)/*for DTS support*/)
        {
            i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            if (i4_ret == FMR_OK)
            {
                i4_ret = x_fm_read(h_file, ac_avi_hdr, 8, &ui4_read);
                if (i4_ret != FMR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                if (x_memcmp((VOID*)"DTSHDHDR", ac_avi_hdr, 8) == 0)
                {
                    pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                    pt_format->t_media_subtype.u.e_aud_subtype=MEDIA_AUD_SUBTYPE_WAVE;
                    return MFMTRECGR_OK;
                }                
            }
        }
        else
        {
            UINT32 ui4_dts_tag;

            ui4_dts_tag = ((UINT32)((UINT8)ac_avi_hdr[3])) << 24 |
                          ((UINT32)((UINT8)ac_avi_hdr[2])) << 16 |
                          ((UINT32)((UINT8)ac_avi_hdr[1])) << 8  |
                          ((UINT32)((UINT8)ac_avi_hdr[0]));

            if((ui4_dts_tag == 0x0180FE7F) || 
                (ui4_dts_tag == 0x00E8FF1F))  /*.cpt file*/
            {
                pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype=MEDIA_AUD_SUBTYPE_WAVE;
                return MFMTRECGR_OK;
            }
        }
    }
    else
    {
        return MFMTRECGR_FILE_ERR;
    }
#endif 
    return MFMTRECGR_INTERNAL_ERR;
}
                            
/*-----------------------------------------------------------------------------
 * Name:  avi_handler_mem_confirm_fct
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
INT32 avi_handler_mem_confirm_fct(
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format) 
{

    INT32           i4_ret;
    UINT32          ui4_read;
    UINT64          ui8_buff_pos;
    MEDIA_FEEDER_T* pt_src_feeder;
    UCHAR*          puc_buff;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if(z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (i4_ret == INPUTR_OK)
        {
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4(
                                         &pt_src_feeder->h_feeder, 
                                         &ui4_read);
          
            if (x_memcmp((VOID*)"RIFF", &ui4_read, 4) == 0)
            {
                /* file size */
                pt_src_feeder->t_feeder_le_fct_tbl.pf_input4(
                                             &pt_src_feeder->h_feeder, 
                                             &ui4_read);
                /* file format "AVI " */
                pt_src_feeder->t_feeder_le_fct_tbl.pf_input4(
                                             &pt_src_feeder->h_feeder, 
                                             &ui4_read);
                if (x_memcmp((VOID*)"AVI ", &ui4_read, 4) == 0)
                {
                    pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                    pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;
                    return MFMTRECGR_OK;
                }
                else if(x_memcmp((VOID*)"WAVE", &ui4_read, 4) == 0)
                {  /* add for wave support */
                    i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                20,
                                MEDIA_SEEK_BGN);
                    if (i4_ret == INPUTR_OK)
                    {
                        pt_src_feeder->t_feeder_le_fct_tbl.pf_input4(
                                                     &pt_src_feeder->h_feeder, 
                                                     &ui4_read);
                        if((ui4_read & 0xFF) != 85)
                        {   /*it's not mp3 format*/
                            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                            pt_format->t_media_subtype.u.e_aud_subtype=MEDIA_AUD_SUBTYPE_WAVE;
                            return MFMTRECGR_OK;
                        }
                    }
                }
            }
        }
    }
    else
    {
        ui8_buff_pos = 0;
        puc_buff = (UCHAR*)pv_mem_data;
        if (ui8_buff_pos < z_mem_leng)
        {
            ui4_read = (UINT32)(puc_buff[ui8_buff_pos]) 
                     | (UINT32)(puc_buff[ui8_buff_pos + 1] << 8) 
                     | (UINT32)(puc_buff[ui8_buff_pos + 2] << 16) 
                     | (UINT32)(puc_buff[ui8_buff_pos + 3] << 24);
            ui8_buff_pos += 4;
            if (ui8_buff_pos > z_mem_leng)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            if (x_memcmp((VOID*)"RIFF", &ui4_read, 4) == 0)
            {
                /* skip file size */
                ui8_buff_pos += 4;
                /* file format "AVI " */
                ui4_read = (UINT32)(puc_buff[ui8_buff_pos]) 
                         | (UINT32)(puc_buff[ui8_buff_pos + 1] << 8) 
                         | (UINT32)(puc_buff[ui8_buff_pos + 2] << 16) 
                         | (UINT32)(puc_buff[ui8_buff_pos + 3] << 24);
                ui8_buff_pos += 4;
                if (ui8_buff_pos > z_mem_leng)
                {
                    return MFMTRECGR_INTERNAL_ERR;
                }
                if (x_memcmp((VOID*)"AVI ", &ui4_read, 4) == 0)
                {
                    pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                    pt_format->t_media_subtype.u.e_contnr_subtype=MEDIA_CONTNR_SUBTYPE_AVI;
                    return MFMTRECGR_OK;
                }
                else if(x_memcmp((VOID*)"WAVE", &ui4_read, 4) == 0)
                {   /* add for wave support */
                    if ((((UINT32)z_mem_leng) > 20) && (puc_buff[20] != 85))
                    {
                        /*it's not mp3 format*/
                        pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                        pt_format->t_media_subtype.u.e_aud_subtype=MEDIA_AUD_SUBTYPE_WAVE;
                        return MFMTRECGR_OK;
                    }
                }
            }
        }
    }
    return MFMTRECGR_INTERNAL_ERR;
}

