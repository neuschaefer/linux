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
 * $RCSfile: mkv_mfmtrecg.c,v $
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
#include "../mfmtrecg.h"
#include "mkv_mfmtrecg.h"

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
#include "../mfmtrecg.h"
#include "mkv_mfmtrecg.h"


#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

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
 * Name:  mkv_confirm_check
 *
 * Description: this function will check file format
 *
 * Inputs:  pui1_data_buf         specify the data buffer
 *          ui4_buf_len           specify the buffer length
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mkv_confirm_check(UINT8* pui1_data_buf, UINT32 ui4_buf_len, MEDIA_FORMAT_T* pt_format)
{
    INT32               i4_ret;
    UINT8               ui1_offset;
    UINT8*              pui1_read_pos = pui1_data_buf;
    CHAR*               psz_mkv_hdr = NULL;
    BOOL                b_find_doctype = FALSE;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
    pt_ebml_element = (EBML_ELE_BASIC_T*)_mm_util_mem_alloc(sizeof(EBML_ELE_BASIC_T));
    if (pt_ebml_element == NULL)
    {
        return MFMTRECGR_OUT_OF_MEMORY;
    }
    while (pui1_read_pos < pui1_data_buf + ui4_buf_len)
    {
        ui1_offset = 0;
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _ebml_buf_ele_base((VOID*)pui1_read_pos, ui4_buf_len, 0, pt_ebml_element);
        if (i4_ret != EBMLR_OK)
        {
            _mm_util_mem_free(pt_ebml_element);
            return MFMTRECGR_INTERNAL_ERR;
        }
        ui1_offset = _ebml_get_ele_head_len(pt_ebml_element);
        pui1_read_pos += ui1_offset;
        if (_ebml_get_ele_id(pt_ebml_element) == MKV_ELE_EBML_DOC_TYPE_ID)
        {
            /*doctype*/
            b_find_doctype = TRUE;
            psz_mkv_hdr = _ebml_buf_string((VOID*)pui1_read_pos, 
                                            ui4_buf_len,
                                           (UINT32)_ebml_get_ele_size(pt_ebml_element));
            if (psz_mkv_hdr == NULL)
            {
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_INTERNAL_ERR;
            }
            if ((x_memcmp((VOID*)"matroska", psz_mkv_hdr, 8) == 0) ||
                (x_memcmp((VOID*)"webm", psz_mkv_hdr, 4) == 0))
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype=MEDIA_CONTNR_SUBTYPE_MATROSKA;
                _ebml_string_free(&psz_mkv_hdr);
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_OK;
            }
            _ebml_string_free(&psz_mkv_hdr);
            break;
        }
        pui1_read_pos += _ebml_get_ele_size(pt_ebml_element);
    }
    _mm_util_mem_free(pt_ebml_element);
    if (!b_find_doctype)
    {
        /*not find doc type element, so use default value*/
        pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
        pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;
        return MFMTRECGR_OK;
    }
    return MFMTRECGR_INTERNAL_ERR;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mkv_handler_file_confirm_fct(
                            HANDLE_T                    h_file,        
                            MEDIA_FORMAT_T*             pt_format)     
{

#ifndef __NO_FM__

    UINT64              ui8_cur_pos = 0;
    INT32               i4_ret;
    UINT32              ui4_read;
    UINT32              ui4_element_id;
    UINT64              ui8_get_size;
    UINT32              ui4_element_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
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
        UINT8* pui1_head_buf;
        pui1_head_buf = _mm_util_mem_alloc(sizeof(UINT8) * MRECG_MKV_ELE_HEAD_LEN);
        if (pui1_head_buf == NULL)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        i4_ret = x_fm_read(h_file, pui1_head_buf, MRECG_MKV_ELE_HEAD_LEN, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            _mm_util_mem_free(pui1_head_buf);
            return MFMTRECGR_FILE_ERR;
        }
        pt_ebml_element = _mm_util_mem_alloc(sizeof(EBML_ELE_BASIC_T));
        if (pt_ebml_element == NULL)
        {
            _mm_util_mem_free(pui1_head_buf);
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
        i4_ret = _ebml_buf_ele_base((VOID*)pui1_head_buf, MRECG_MKV_ELE_HEAD_LEN, 0, pt_ebml_element);
        if (i4_ret != EBMLR_OK)
        {
            _mm_util_mem_free(pui1_head_buf);
            _mm_util_mem_free(pt_ebml_element);
            return MFMTRECGR_INTERNAL_ERR;
        }
        /*free head buffer*/
        _mm_util_mem_free(pui1_head_buf);
        pui1_head_buf = NULL;
        ui4_element_id = _ebml_get_ele_id(pt_ebml_element);
        ui8_get_size = _ebml_get_ele_size(pt_ebml_element);
        if ((ui8_get_size >> 32))
        {
            DBG_INFO(("ebml head size too large!\r\n"));
            _mm_util_mem_free(pt_ebml_element);
            return MFMTRECGR_INTERNAL_ERR;
        }
        ui4_element_size = (UINT32)(ui8_get_size);
        if ((ui4_element_id == MKV_ELE_EBML_HEAD_ID) && (ui4_element_size))
        {
            UINT8* pui1_data_buf;           
            UINT8 ui1_offset; 
            pui1_data_buf = _mm_util_mem_alloc(sizeof(UINT8) * ui4_element_size);
            if (pui1_data_buf == NULL)
            {
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_OUT_OF_MEMORY;
            }
            /*can put below code to a single funtion*/
            ui1_offset = _ebml_get_ele_head_len(pt_ebml_element);
            i4_ret = x_fm_lseek(h_file, ui1_offset, FM_SEEK_BGN, &ui8_cur_pos);
            if (i4_ret != FMR_OK)
            {
                _mm_util_mem_free(pui1_data_buf);
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_FILE_ERR;
            }
            i4_ret = x_fm_read(h_file, pui1_data_buf, ui4_element_size, &ui4_read);
            if (i4_ret != FMR_OK)
            {
                _mm_util_mem_free(pui1_data_buf);
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_FILE_ERR;
            }
            
            i4_ret = mkv_confirm_check(pui1_data_buf, ui4_element_size, pt_format);
            _mm_util_mem_free(pui1_data_buf);
            if (i4_ret == MFMTRECGR_OK)
            {
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_OK;
            }
        }
        _mm_util_mem_free(pt_ebml_element);
    }
    else
    {
        return MFMTRECGR_FILE_ERR;
    }
#endif 
    return MFMTRECGR_INTERNAL_ERR;
}
                            
/*-----------------------------------------------------------------------------
 * Name:  mkv_handler_mem_confirm_fct
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
INT32 mkv_handler_mem_confirm_fct(
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format) 
{

    INT32               i4_ret;
    UINT32              ui4_read;
    UINT64              ui8_buff_pos;
    MEDIA_FEEDER_T*     pt_src_feeder;
    UINT8*              pui1_buff;
    UINT64              ui8_get_size;
    UINT32              ui4_ele_size;
    EBML_ELE_BASIC_T*   pt_ebml_element;
    
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
            UINT8* pui1_head_buf;
            
            pui1_head_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * MRECG_MKV_ELE_HEAD_LEN);
            if (pui1_head_buf == NULL)
            {
                return MFMTRECGR_OUT_OF_MEMORY;
            }
            i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                      &pt_src_feeder->h_feeder,
                                      pui1_head_buf,
                                      MRECG_MKV_ELE_HEAD_LEN,
                                      MRECG_MKV_ELE_HEAD_LEN,
                                      &ui4_read);
            if (MRECG_MKV_ELE_HEAD_LEN != ui4_read)
            {
                DBG_ERROR(("mkv_handler_mem_confirm_fct:error read (%d)\r\n",
                           ui4_read));
                _mm_util_mem_free(pui1_head_buf);
                return MFMTRECGR_INTERNAL_ERR;
            }
            pt_ebml_element = (EBML_ELE_BASIC_T*)_mm_util_mem_alloc(sizeof(EBML_ELE_BASIC_T));
            if (pt_ebml_element == NULL)
            {
                _mm_util_mem_free(pui1_head_buf);
                return MFMTRECGR_OUT_OF_MEMORY;
            }
            x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
            i4_ret = _ebml_buf_ele_base((VOID*)pui1_head_buf, MRECG_MKV_ELE_HEAD_LEN, 0, pt_ebml_element);
            if (i4_ret != EBMLR_OK)
            {
                _mm_util_mem_free(pui1_head_buf);
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_INTERNAL_ERR;
            }
            /*free head buffer*/
            _mm_util_mem_free(pui1_head_buf);
            pui1_head_buf = NULL;
            ui8_get_size = _ebml_get_ele_size(pt_ebml_element);
            if ((ui8_get_size >> 32))
            {
                DBG_INFO(("ebml head size too large!\r\n"));
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui4_ele_size = (UINT32)(ui8_get_size);
            if ((_ebml_get_ele_id(pt_ebml_element) == MKV_ELE_EBML_HEAD_ID) && (ui4_ele_size))
            {
                UINT8* pui1_data_buf;           
                UINT8 ui1_offset = 0; 
                
                pui1_data_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui4_ele_size);
                if (pui1_data_buf == NULL)
                {
                    _mm_util_mem_free(pt_ebml_element);
                    return MFMTRECGR_OUT_OF_MEMORY;
                }
                ui1_offset = _ebml_get_ele_head_len(pt_ebml_element);
                i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                ui1_offset,
                                MEDIA_SEEK_BGN);
                if (i4_ret != INPUTR_OK)
                {
                    _mm_util_mem_free(pt_ebml_element);
                    _mm_util_mem_free(pui1_data_buf);
                    return MFMTRECGR_OUT_OF_MEMORY;
                }
                i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                      &pt_src_feeder->h_feeder,
                                      pui1_data_buf,
                                      ui4_ele_size,
                                      ui4_ele_size,
                                      &ui4_read);
                if (ui4_ele_size != ui4_read)
                {
                    _mm_util_mem_free(pt_ebml_element);
                    _mm_util_mem_free(pui1_data_buf);
                    return MFMTRECGR_INTERNAL_ERR;
                }

                i4_ret = mkv_confirm_check(pui1_data_buf, ui4_ele_size, pt_format);
                _mm_util_mem_free(pui1_data_buf);
                if (i4_ret == MFMTRECGR_OK)
                {
                    _mm_util_mem_free(pt_ebml_element);
                    return MFMTRECGR_OK;
                }
            }
            _mm_util_mem_free(pt_ebml_element);
        }
    }
    else
    {
        ui8_buff_pos = 0;
        pui1_buff = (UINT8*)pv_mem_data;
        if (ui8_buff_pos < z_mem_leng)
        {
            pt_ebml_element = (EBML_ELE_BASIC_T*)_mm_util_mem_alloc(sizeof(EBML_ELE_BASIC_T));
            if (pt_ebml_element == NULL)
            {
                return MFMTRECGR_OUT_OF_MEMORY;
            }
            x_memset(pt_ebml_element, 0, sizeof(EBML_ELE_BASIC_T));
            i4_ret = _ebml_buf_ele_base((VOID*)pui1_buff, MRECG_MKV_ELE_HEAD_LEN, 0, pt_ebml_element);
            if (i4_ret != EBMLR_OK)
            {
                _mm_util_mem_free(pt_ebml_element);
                return MFMTRECGR_INTERNAL_ERR;
            }
            ui8_get_size = _ebml_get_ele_size(pt_ebml_element);
            if ((ui8_get_size >> 32))
            {
                DBG_INFO(("ebml head size too large!\r\n"));
                _mm_util_mem_free(pt_ebml_element);
                return MINFOR_INTERNAL_ERR;
            }
            ui4_ele_size = (UINT32)(ui8_get_size);
            if ((_ebml_get_ele_id(pt_ebml_element) == MKV_ELE_EBML_HEAD_ID) && (ui4_ele_size))
            {
                UINT8* pui1_data_buf = pui1_buff + _ebml_get_ele_head_len(pt_ebml_element);
                i4_ret = mkv_confirm_check(pui1_data_buf, ui4_ele_size, pt_format);
                if (i4_ret == MFMTRECGR_OK)
                {
                    _mm_util_mem_free(pt_ebml_element);
                    return MFMTRECGR_OK;
                }
            }
            _mm_util_mem_free(pt_ebml_element);
         }
    }
    return MFMTRECGR_INTERNAL_ERR;
}

