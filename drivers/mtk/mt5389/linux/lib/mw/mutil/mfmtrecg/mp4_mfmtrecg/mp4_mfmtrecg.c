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
 * mp4_mfmtrecg.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
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

#include "mutil/mm_util.h"

#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "mutil/mm_util_input.h"
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "u_dbg.h"

#include "../../mm_util.h"
#include "../../mm_util_input.h"

#include "x_lnk_list.h"
#include "x_mm_common.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))

#define MP4_BOX_FTYP        dwBigFourCC('f', 't', 'y', 'p')
#define MP4_BOX_PNOT        dwBigFourCC('p', 'n', 'o', 't')
#define MP4_BOX_PICT        dwBigFourCC('P', 'I', 'C', 'T')
#define MP4_BOX_MOOV        dwBigFourCC('m', 'o', 'o', 'v')
#define MP4_BOX_MDAT        dwBigFourCC('m', 'd', 'a', 't')
#define MP4_BOX_UUID        dwBigFourCC('u', 'u', 'i', 'd')            
#define MP4_BOX_PDIN        dwBigFourCC('p', 'd', 'i', 'n')
#define MP4_BOX_MOOF        dwBigFourCC('m', 'o', 'o', 'f')
#define MP4_BOX_MFRA        dwBigFourCC('m', 'f', 'r', 'a')
#define MP4_BOX_FREE        dwBigFourCC('f', 'r', 'e', 'e')
#define MP4_BOX_SKIP        dwBigFourCC('s', 'k', 'i', 'p')
#define MP4_BOX_META        dwBigFourCC('m', 'e', 't', 'a')
#define MP4_BOX_WIDE        dwBigFourCC('w', 'i', 'd', 'e')/*mov*/
#define MP4_BOX_UDTA        dwBigFourCC('u', 'd', 't', 'a')
#define MP4_BOX_TRAK        dwBigFourCC('t', 'r', 'a', 'k')
#define MP4_BOX_MDIA        dwBigFourCC('m', 'd', 'i', 'a')
#define MP4_BOX_HDLR        dwBigFourCC('h', 'd', 'l', 'r')
#define MP4_BOX_YQOO        dwBigFourCC('y', 'q', 'o', 'o')
#define MP4_BOX_SIDX        dwBigFourCC('s', 'i', 'd', 'x')
#define MP4_BOX_FLSZ        dwBigFourCC('f', 'l', 's', 'z')
#define MP4_BOX_TYPE_SOUN   dwBigFourCC('s', 'o', 'u', 'n')
#define MP4_BOX_TYPE_VIDE   dwBigFourCC('v', 'i', 'd', 'e')


#define MP4_BOX_HEADER_SIZE( p_box ) \
  ( 8 + ( p_box->ui4_mp4_box_size == 1 ? 8 : 0 ) \
      + ( p_box->ui4_mp4_box_type == MP4_BOX_UUID ? 16 : 0 ) )

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#ifndef __NO_FM__
/*-----------------------------------------------------------------------------
* Name:  mp4_finput4bytes_fct
*
* Description: 
*
* Inputs:  -
*          
* Outputs: -
*
* Returns: 
----------------------------------------------------------------------------*/                            
static UINT32 mp4_finput4bytes_fct(HANDLE_T h_file)
{
    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    
    i4_ret = x_fm_read(h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui4_data&0xff000000)>>24) 
                | ((ui4_data&0xff0000)>>8) 
                | ((ui4_data&0xff00)<<8) 
                | ((ui4_data&0xff)<<24);
    }
    else
    {
        return (UINT32)-1;
    }    
}
#endif

/*-----------------------------------------------------------------------------
* Name:  mp4_minput4bytes_fct
*
* Description: 
*
* Inputs:  -
*          
* Outputs: -
*
* Returns: 
----------------------------------------------------------------------------*/ 
UINT32 mp4_minput4bytes_fct (const VOID* pv_mem_data, UINT64* pui8_buff_pos)
{
    UINT32          ui4_ret = 0;
    UCHAR*          puc_buff = (UCHAR*)pv_mem_data;

    ui4_ret = (UINT32)(puc_buff[*pui8_buff_pos] << 24) 
            | (UINT32)(puc_buff[*pui8_buff_pos + 1] << 16) 
            | (UINT32)(puc_buff[*pui8_buff_pos + 2] << 8) 
            | (UINT32)(puc_buff[*pui8_buff_pos + 3]);
    *pui8_buff_pos = *pui8_buff_pos + 4;
    
    return ui4_ret;
}

#ifndef __NO_FM__
/*-----------------------------------------------------------------------------
 * Name:  check_is_m4a_file
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
static INT32 check_is_m4a_file(
                                HANDLE_T            h_file,        
                                MEDIA_FORMAT_T*     pt_format)     
{
    UINT64  ui8_cur_pos;
    INT32   i4_ret;
    UINT32  ui4_size;
    UINT64  ui8_box_size;
    UINT32  ui4_box_type;
    UINT32  ui4_trak_type;
    UINT64  ui8_moov_range = (UINT64)-1;
    BOOL    b_inc_aud = FALSE;
    BOOL    b_need_enter_box = FALSE;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL) || (h_file == NULL_HANDLE))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if(FMR_OK != i4_ret)
    {
        return MFMTRECGR_FILE_ERR;
    }
    while (i4_ret == FMR_OK)
    {
        if(ui8_cur_pos >= ui8_moov_range)
        {
            break;
        }
        ui8_box_size = (UINT64)mp4_finput4bytes_fct(h_file);
        if (ui8_box_size == (UINT32)-1)
        {
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_box_type = mp4_finput4bytes_fct(h_file);
        if (ui4_box_type == (UINT32)-1)
        {
            return MFMTRECGR_FILE_ERR;
        }
        switch(ui4_box_type)
        {
            case MP4_BOX_MOOV:
                ui8_moov_range = ui8_cur_pos + ui8_box_size;
                /*intend without break*/
            case MP4_BOX_TRAK:
            case MP4_BOX_MDIA:
                b_need_enter_box = TRUE;
                break;
            case MP4_BOX_HDLR:
                ui4_trak_type = mp4_finput4bytes_fct(h_file);
                if (ui4_trak_type == (UINT32)-1)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                
                ui4_trak_type = mp4_finput4bytes_fct(h_file);
                if (ui4_trak_type == (UINT32)-1)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                /*skip the former 8 bytes*/
                ui4_trak_type = mp4_finput4bytes_fct(h_file);
                if(ui4_trak_type == MP4_BOX_TYPE_SOUN)
                {
                    b_inc_aud = TRUE;
                }
                else if(ui4_trak_type == MP4_BOX_TYPE_VIDE)
                {
                    return MFMTRECGR_OK;
                }
                else if (ui4_trak_type == (UINT32)-1)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                else
                {
                    /*Do Nothing*/
                }
                ui8_box_size -= 12;
                break;
            default:
                b_need_enter_box = FALSE;
                break;
        }
        
        if (ui8_box_size == 1) /* 64-bit big size */
        {
            ui4_size = mp4_finput4bytes_fct(h_file);
            if (ui4_size == (UINT32)-1)
            {
                return MFMTRECGR_FILE_ERR;
            }
            ui8_box_size = ((UINT64)ui4_size<<32) + mp4_finput4bytes_fct(h_file);
            i4_ret = x_fm_lseek(h_file, (INT64)ui8_box_size - 16, FM_SEEK_CUR, &ui8_cur_pos);
            if(FMR_OK != i4_ret)
            {
                return MFMTRECGR_FILE_ERR;
            }
        }
        else if(ui8_box_size == 0) /* box extends to end of file */
        {
            i4_ret = FMR_EOF;
        }
        else if(!b_need_enter_box)
        {
            i4_ret = x_fm_lseek(h_file, (INT64)ui8_box_size - 8, FM_SEEK_CUR, &ui8_cur_pos);
            if(FMR_OK != i4_ret)
            {
                return MFMTRECGR_FILE_ERR;
            }
        }
    }
    /*m4a file need mp4 only include audio trak*/
    if (b_inc_aud) 
    {
        pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
        pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
        pt_format->e_mm_svc_type = MEDIA_SVC_TYPE_AUDIO_ONLY;
        /*DBG_INFO(("INFO: File recognizer is MP4 file \r\n")); */
        return MFMTRECGR_OK;
    }
    
    return MFMTRECGR_INTERNAL_ERR;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  check_is_m4a_mem
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
static INT32 check_is_m4a_mem(  const VOID*       pv_mem_data, 
                                SIZE_T            z_mem_leng,   
                                MEDIA_FORMAT_T*   pt_format)    
{
    INT32           i4_ret;
    UINT32          ui4_size, ui4_size1;
    MEDIA_FEEDER_T* pt_src_feeder;
    UINT64          ui8_buff_pos = 0;
    UINT64          ui8_box_size;
    UINT32          ui4_box_type;
    UINT32          ui4_trak_type;
    UINT64          ui8_moov_range = (UINT64)-1;
    BOOL            b_inc_aud = FALSE;
    BOOL            b_need_enter_box = FALSE;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if(z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        while (i4_ret == FMR_OK)
        {
            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos(
                                    &pt_src_feeder->h_feeder,
                                    &ui8_buff_pos);
            if(ui8_buff_pos >= ui8_moov_range)
            {
                break;
            }
            
            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_size);
            ui8_box_size = ui4_size;
            if (i4_ret != INPUTR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_box_type);
            if (ui4_box_type == 0)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            
            switch(ui4_box_type)
            {
                case MP4_BOX_MOOV:
                    ui8_moov_range = ui8_buff_pos + ui8_box_size;
                    /*intend without break*/
                case MP4_BOX_TRAK:
                case MP4_BOX_MDIA:
                    b_need_enter_box = TRUE;
                    break;
                case MP4_BOX_HDLR:
                    pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_trak_type);
                    if (ui4_trak_type == 0)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    
                    pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_trak_type);
                    if (ui4_trak_type == 0)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    
                    /*skip the former 8 bytes*/                    
                    pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_trak_type);
                    if(ui4_trak_type == MP4_BOX_TYPE_SOUN)
                    {
                        b_inc_aud = TRUE;
                    }
                    else if(ui4_trak_type == MP4_BOX_TYPE_VIDE)
                    {
                        return MFMTRECGR_OK;
                    }
                    else
                    {
                        /*Do Nothing*/
                    }
                    ui8_box_size -= 12;
                    break;
                default:
                    b_need_enter_box = FALSE;
                    break;
            }
            
            if (ui8_box_size == 1) /* 64-bit big size */
            {
                pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                         &pt_src_feeder->h_feeder, 
                                         &ui4_size);
                pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                         &pt_src_feeder->h_feeder, 
                                         &ui4_size1);
                ui8_box_size = ((UINT64)ui4_size<<32) + ui4_size1;
                              
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                    &pt_src_feeder->h_feeder,
                                    (INT64)ui8_box_size - 16,
                                    FM_SEEK_CUR);
            }
            else if(ui8_box_size == 0) /* box extends to end of file */
            {
                i4_ret = FMR_EOF;
            }
            else if(!b_need_enter_box)
            {
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                    &pt_src_feeder->h_feeder,
                                    (INT64)ui8_box_size - 8,
                                    FM_SEEK_CUR);
            }
        }
        /*m4a file need mp4 only include audio trak*/
        if (b_inc_aud) 
        {
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
            pt_format->e_mm_svc_type = MEDIA_SVC_TYPE_AUDIO_ONLY;
            /*DBG_INFO(("INFO: File recognizer is MP4 file \r\n")); */
            return MFMTRECGR_OK;
        }
    }
    else
    {
        ui8_buff_pos = 0;
        while (ui8_buff_pos < z_mem_leng)
        {
            ui8_box_size = (UINT64)mp4_minput4bytes_fct(pv_mem_data,  &ui8_buff_pos);
            if (ui8_box_size == (UINT32)-1)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        
            ui4_box_type = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
            if (ui4_box_type == (UINT32)-1)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            switch(ui4_box_type)
            {
                case MP4_BOX_MOOV:
                    ui8_moov_range = ui8_buff_pos + ui8_box_size;
                    /*intend without break*/
                case MP4_BOX_TRAK:
                case MP4_BOX_MDIA:
                    b_need_enter_box = TRUE;
                    break;
                case MP4_BOX_HDLR:
                    ui4_trak_type = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                    if (ui4_box_type == (UINT32)-1)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    ui4_trak_type = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                    if (ui4_box_type == (UINT32)-1)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    
                    /*skip the former 8 bytes*/            
                    ui4_trak_type = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos); 
                    if(ui4_trak_type == MP4_BOX_TYPE_SOUN)
                    {
                        b_inc_aud = TRUE;
                    }
                    else if(ui4_trak_type == MP4_BOX_TYPE_VIDE)
                    {
                        return MFMTRECGR_OK;
                    }
                    else
                    {
                        /*Do Nothing*/
                    }
                    ui8_box_size -= 12;
                    break;
                default:
                    b_need_enter_box = FALSE;
                    break;
            }
            /*m4a file need mp4 only include audio trak*/
            if (b_inc_aud) 
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
                pt_format->e_mm_svc_type = MEDIA_SVC_TYPE_AUDIO_ONLY;
                /*DBG_INFO(("INFO: File recognizer is MP4 file \r\n")); */
                return MFMTRECGR_OK;
            }
    
            if (ui8_box_size == 1) /* 64-bit big size */
            {
                ui4_size = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                ui8_box_size = ((UINT64)ui4_size<<32) + 
                               mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                
                ui8_buff_pos = ui8_buff_pos + ui8_box_size - 16;
            }
            else if(ui8_box_size == 0) /* box extends to end of file */
            {
                i4_ret = FMR_EOF;
            }
            else if(!b_need_enter_box)
            {
                ui8_buff_pos = ui8_buff_pos + ui8_box_size - 8;
            }
        }
    }
    return MFMTRECGR_INTERNAL_ERR;
}


/*-----------------------------------------------------------------------------
 * Name:  mp4_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_handler_file_confirm_fct(
                            HANDLE_T            h_file,        
                            MEDIA_FORMAT_T*     pt_format)     
{

#ifndef __NO_FM__

    UINT64  ui8_cur_pos;
    INT32   i4_ret;
    UINT32  ui4_size;
    UINT64  ui8_box_size;
    UINT32  ui4_box_type;
    BOOL    b_inc_moov = FALSE;
    BOOL    b_inc_mdat = FALSE;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL) || (h_file == NULL_HANDLE))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if(FMR_OK != i4_ret)
    {
        return MFMTRECGR_FILE_ERR;
    }
    while (i4_ret == FMR_OK)
    {
        ui8_box_size = (UINT64)mp4_finput4bytes_fct(h_file);
        if (ui8_box_size == (UINT32)-1)
        {
            return MFMTRECGR_FILE_ERR;
        }
        
        ui4_box_type = mp4_finput4bytes_fct(h_file);
        if (ui4_box_type == (UINT32)-1)
        {
            return MFMTRECGR_FILE_ERR;
        }
        switch(ui4_box_type)
        {
            case MP4_BOX_FTYP:
            case MP4_BOX_UUID:
            case MP4_BOX_PNOT:
            case MP4_BOX_PICT:
            case MP4_BOX_FLSZ:
                break;
            case MP4_BOX_MOOV:
     #if (ENABLE_CMOV != 1)
            {/*skip cmov file at temp*/
                UINT32 ui4_tmp_box_size = 0;
                UINT32 ui4_tmp_box_type = 0;
                ui4_tmp_box_size = mp4_finput4bytes_fct(h_file);
                if (ui4_tmp_box_size == (UINT32)-1)
                {
                    return MFMTRECGR_FILE_ERR;
                }
        
                ui4_tmp_box_type = mp4_finput4bytes_fct(h_file);
                if (ui4_tmp_box_type == (UINT32)-1)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                if(ui4_tmp_box_type == dwBigFourCC('c', 'm', 'o', 'v'))
                {
                    return MFMTRECGR_INTERNAL_ERR;
                }
                i4_ret = x_fm_lseek(h_file, (INT64)-8, FM_SEEK_CUR, &ui8_cur_pos);
                if(FMR_OK != i4_ret)
                {
                    return MFMTRECGR_FILE_ERR;
                }
            }
     #endif
                b_inc_moov = TRUE;
                break;
            case MP4_BOX_MDAT:
                b_inc_mdat = TRUE;
                break;
            case MP4_BOX_PDIN:
            case MP4_BOX_MOOF:
            case MP4_BOX_MFRA:
            case MP4_BOX_FREE:
            case MP4_BOX_SKIP:
            case MP4_BOX_META:
            case MP4_BOX_WIDE:/*mov*/
            case MP4_BOX_UDTA:
            case MP4_BOX_YQOO:
            case MP4_BOX_SIDX:
                break;
            default:
                return MFMTRECGR_INTERNAL_ERR;
        }
        
        /*mp4 file need include box "moov" and "mdat"*/
        if (b_inc_moov && b_inc_mdat) 
        {
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
            /*DBG_INFO(("INFO: File recognizer is MP4 file \r\n")); */
            if(MFMTRECGR_FILE_ERR == check_is_m4a_file(h_file, pt_format))
            {
                return MFMTRECGR_FILE_ERR;
            }
            return MFMTRECGR_OK;
        }
    
        if (ui8_box_size == 1) /* 64-bit big size */
        {
            ui4_size = mp4_finput4bytes_fct(h_file);
            if(ui4_size == (UINT32)-1)
            {
                return MFMTRECGR_FILE_ERR;
            }
            ui8_box_size = ((UINT64)ui4_size<<32) + mp4_finput4bytes_fct(h_file);
            i4_ret = x_fm_lseek(h_file, (INT64)ui8_box_size - 16, FM_SEEK_CUR, &ui8_cur_pos);
            if(i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
        }
        else if(ui8_box_size == 0) /* box extends to end of file */
        {
            i4_ret = FMR_EOF;
        }
        else
        {
            i4_ret = x_fm_lseek(h_file, (INT64)ui8_box_size - 8, FM_SEEK_CUR, &ui8_cur_pos);
            if(i4_ret != FMR_OK)
            {
                return MFMTRECGR_FILE_ERR;
            }
        }
    }
    return MFMTRECGR_INTERNAL_ERR;

#else

    return MFMTRECGR_INTERNAL_ERR;

#endif
}
                            
/*-----------------------------------------------------------------------------
 * Name:  mp4_handler_mem_confirm_fct
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
INT32 mp4_handler_mem_confirm_fct(const VOID*       pv_mem_data, 
                                  SIZE_T            z_mem_leng,   
                                  MEDIA_FORMAT_T*   pt_format) 
{
    INT32           i4_ret;
    UINT32          ui4_size, ui4_size1;
    UINT64          ui8_box_size;
    UINT32          ui4_box_type;
    MEDIA_FEEDER_T* pt_src_feeder;
    UINT64          ui8_buff_pos;
    BOOL            b_inc_moov = FALSE;
    BOOL            b_inc_mdat = FALSE;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if(z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        while (i4_ret == INPUTR_OK)
        {
            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_size);
            ui8_box_size = ui4_size;
            if (i4_ret != INPUTR_OK)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_box_type);
            if (ui4_box_type == 0)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            switch(ui4_box_type)
            {
                case MP4_BOX_FTYP:
                case MP4_BOX_UUID:
                case MP4_BOX_PNOT:
                case MP4_BOX_PICT:
                case MP4_BOX_FLSZ:
                    break;
                case MP4_BOX_MOOV:
        #if (ENABLE_CMOV != 1)
                {/*skip cmov file at temp*/
                    UINT32 ui4_tmp_box_size = 0;
                    UINT32 ui4_tmp_box_type = 0;
                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                            &pt_src_feeder->h_feeder, 
                                            &ui4_tmp_box_size);
                    if (i4_ret != INPUTR_OK)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                            &pt_src_feeder->h_feeder, 
                                            &ui4_tmp_box_type);
                    if (i4_ret != INPUTR_OK)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    if ((ui4_tmp_box_type == (UINT32)-1)
                        ||(ui4_tmp_box_type == dwBigFourCC('c', 'm', 'o', 'v')))
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                        &pt_src_feeder->h_feeder,
                                        (INT64)-8,
                                        FM_SEEK_CUR);
            }
     #endif
                    b_inc_moov = TRUE;
                    break;
                case MP4_BOX_MDAT:
                    b_inc_mdat = TRUE;
                    break;
                case MP4_BOX_PDIN:
                case MP4_BOX_MOOF:
                case MP4_BOX_MFRA:
                case MP4_BOX_FREE:
                case MP4_BOX_SKIP:
                case MP4_BOX_META:
                case MP4_BOX_WIDE:/*mov*/
                case MP4_BOX_UDTA:
                case MP4_BOX_YQOO:
                case MP4_BOX_SIDX:
                break;
                default:
                return MFMTRECGR_INTERNAL_ERR;
            }
        
            /*mp4 file need include box "moov" and "mdat"*/
            if (b_inc_moov && b_inc_mdat) 
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype = 
                                          MEDIA_CONTNR_SUBTYPE_MP4;
                /*DBG_INFO(("INFO: Memory feeder recognizer is MP4 file \r\n")); */
                check_is_m4a_mem(pv_mem_data, z_mem_leng, pt_format);
                return MFMTRECGR_OK;
            }
    
            if (ui8_box_size == 1) /* 64-bit big size */
            {
                pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                         &pt_src_feeder->h_feeder, 
                                         &ui4_size);
                pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                         &pt_src_feeder->h_feeder, 
                                         &ui4_size1);
                ui8_box_size = ((UINT64)ui4_size<<32) + ui4_size1;
                              
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                    &pt_src_feeder->h_feeder,
                                    (INT64)ui8_box_size - 16,
                                    FM_SEEK_CUR);
            }
            else if(ui8_box_size == 0) /* box extends to end of file */
            {
                i4_ret = FMR_EOF;
            }
            else
            {
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                    &pt_src_feeder->h_feeder,
                                    (INT64)ui8_box_size - 8,
                                    FM_SEEK_CUR);
            }
        }
    }
    else
    {
        ui8_buff_pos = 0;
        while (ui8_buff_pos < z_mem_leng)
        {
            ui8_box_size = (UINT64)mp4_minput4bytes_fct(pv_mem_data,  &ui8_buff_pos);
            if (ui8_box_size == (UINT32)-1)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        
            ui4_box_type = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
            if (ui4_box_type == (UINT32)-1)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
            switch(ui4_box_type)
            {
                case MP4_BOX_FTYP:
                case MP4_BOX_UUID:
                case MP4_BOX_PNOT:
                case MP4_BOX_PICT:
                case MP4_BOX_FLSZ:
                    break;
                case MP4_BOX_MOOV:
        #if (ENABLE_CMOV != 1)
                {/*skip cmov file at temp*/
                    UINT32 ui4_tmp_box_size = 0;
                    UINT32 ui4_tmp_box_type = 0;
                    ui4_tmp_box_size = (UINT64)mp4_minput4bytes_fct(pv_mem_data,  &ui8_buff_pos);
                    if (ui8_box_size == (UINT32)-1)
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    
                    ui4_tmp_box_type = (UINT64)mp4_minput4bytes_fct(pv_mem_data,  &ui8_buff_pos);
                    if ((ui4_tmp_box_type == (UINT32)-1)
                        ||(ui4_tmp_box_type == dwBigFourCC('c', 'm', 'o', 'v')))
                    {
                        return MFMTRECGR_INTERNAL_ERR;
                    }
                    ui8_buff_pos = ui8_buff_pos - 8;
                }
        #endif
                    b_inc_moov = TRUE;
                    break;
                case MP4_BOX_MDAT:
                    b_inc_mdat = TRUE;
                    break;
                case MP4_BOX_PDIN:
                case MP4_BOX_MOOF:
                case MP4_BOX_MFRA:
                case MP4_BOX_FREE:
                case MP4_BOX_SKIP:
                case MP4_BOX_META:
                case MP4_BOX_WIDE:/*mov*/
                case MP4_BOX_UDTA:
                case MP4_BOX_YQOO:
                case MP4_BOX_SIDX:
                    break;
                default:
                    return MFMTRECGR_INTERNAL_ERR;
            }
        
            /*mp4 file need include box "moov" and "mdat"*/
            if (b_inc_moov && b_inc_mdat) 
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype = 
                                          MEDIA_CONTNR_SUBTYPE_MP4;
                /*DBG_INFO(("INFO: Memory recognizer is MP4 file \r\n")); */                
                check_is_m4a_mem(pv_mem_data, z_mem_leng, pt_format);
                return MFMTRECGR_OK;
            }
    
            if (ui8_box_size == 1) /* 64-bit big size */
            {
                ui4_size = mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                ui8_box_size = ((UINT64)ui4_size<<32) + 
                               mp4_minput4bytes_fct(pv_mem_data, &ui8_buff_pos);
                
                ui8_buff_pos = ui8_buff_pos + ui8_box_size - 16;
            }
            else if(ui8_box_size == 0) /* box extends to end of file */
            {
                i4_ret = FMR_EOF;
            }
            else
            {
                ui8_buff_pos = ui8_buff_pos + ui8_box_size - 8;
            }
        }
    }
    return MFMTRECGR_INTERNAL_ERR;
}


