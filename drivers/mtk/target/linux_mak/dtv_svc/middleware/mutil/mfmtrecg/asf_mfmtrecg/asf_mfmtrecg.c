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
 * asf_mfmtrecg.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media format recognizer public APIs  
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"  {
#endif
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
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "mutil/mm_util_input.h"
#include "util/x_lnk_list.h"
#include "asf_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"
#include "mutil/minfo/asf_info/asf_parser_common.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"

#include "../../mm_util.h"
#include "../../mm_util_input.h"

#include "x_lnk_list.h"
#include "asf_mfmtrecg.h"
#include "../mfmtrecg.h"
#include "../../minfo/asf_info/asf_parser_common.h"

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
 * Name:  asf_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match 
 *               extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format       retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 *          other non-zero value:  unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial


 ----------------------------------------------------------------------------*/
INT32 asf_handler_file_confirm_fct( HANDLE_T           h_file,        
                                    MEDIA_FORMAT_T*    pt_format)     
{

#ifndef __NO_FM__

    #if 0
    /*the current position of the file related to h_file*/
    UINT64       ui8_cur_pos;    
    INT32        i4_ret;  /*return code*/         
    /*indicates how many bytes are read*/  
    UINT32       ui4_read;      
    ASF_GUID_T   t_headerguid;
    CHAR         ac_asf_hdr[ASF_HEADER_TEST_LENGTH];
    #endif

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format)
        || (NULL_HANDLE == h_file))
    {
        DBG_ERROR(("asf_handler_file_confirm_fct: Null error\r\n"));
        return MFMTRECGR_INV_ARG;
    }
    
    /*If WMA?*/
    return asf_wma_handler_file_confirm_fct(h_file, pt_format);
    
    #if 0
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init variables*/
    x_memset(&t_headerguid, 0, sizeof(ASF_GUID_T));
    x_memset(ac_asf_hdr, 0, sizeof(CHAR) * ASF_HEADER_TEST_LENGTH);
    
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if (FMR_OK == i4_ret)
    {
        i4_ret = x_fm_read(h_file, ac_asf_hdr, ASF_HEADER_TEST_LENGTH, 
                          &ui4_read);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("asf_handler_file_confirm_fct: x_fm_read "
                       "failed(%d)\r\n", 
                       i4_ret));
            return MFMTRECGR_INTERNAL_ERR;
        }
        
        /*try to get the first guid of a asf file */
        _asf_get_guid(&t_headerguid, (UINT8*)ac_asf_hdr);
        /*whether is the asf standard header guid?*/
        if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_headerguid, 
                                             &gt_asf_object_header_guid))
        {
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype
                                    = MEDIA_CONTNR_SUBTYPE_ASF;
            return MFMTRECGR_OK;
        }
        else
        {
            pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
            pt_format->t_media_subtype.u.e_contnr_subtype 
                                    = MEDIA_CONTNR_SUBTYPE_UNKNOWN;

            DBG_ERROR(("asf_handler_file_confirm_fct:unknown\r\n"));
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
        DBG_ERROR(("asf_handler_file_confirm_fct:x_fm_lseek "
                   "failed(%d)\r\n", i4_ret));
        return MFMTRECGR_INTERNAL_ERR;
    }
    #endif
#else

    return MFMTRECGR_INTERNAL_ERR;

#endif

}

                            
/*-----------------------------------------------------------------------------
 * Name:  asf_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer which want to 
 *                          retrieve the format.
 *          z_mem_leng      specify the length of memory buffer which want to 
 *                          retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the 
 *                          operation has been completed.
 *
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_handler_mem_confirm_fct( const VOID*      pv_mem_data, 
                                   SIZE_T           z_mem_leng,   
                                   MEDIA_FORMAT_T*  pt_format ) 
{
    #if 0
    INT32           i4_ret;  /*return code*/         
    /*indicates how many bytes are read*/  
    UINT32          ui4_read;      
    ASF_GUID_T      t_headerguid;
    CHAR            ac_asf_hdr[ASF_HEADER_TEST_LENGTH];
    MEDIA_FEEDER_T* pt_src_feeder;
    #endif
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pt_format == NULL))
    {
        DBG_ERROR(("asf_handler_mem_confirm_fct: Null error\r\n"));
        return MFMTRECGR_INV_ARG;
    }
    
    return asf_wma_handler_mem_confirm_fct(pv_mem_data, z_mem_leng, pt_format);
    
    #if 0
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init variables*/
    x_memset(&t_headerguid, 0, sizeof(ASF_GUID_T));
    x_memset(ac_asf_hdr, 0, sizeof(CHAR) * ASF_HEADER_TEST_LENGTH);


    /*if 0 != z_mem_leng read buffer directly*/
    if (0 != z_mem_leng)
    {
        x_memcpy(ac_asf_hdr, pv_mem_data, ASF_HEADER_TEST_LENGTH);
    }
    else
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

        /*set file position at the beginning*/
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (INPUTR_OK == i4_ret)
        {
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                      &pt_src_feeder->h_feeder,
                                      ac_asf_hdr,
                                      ASF_HEADER_TEST_LENGTH,
                                      ASF_HEADER_TEST_LENGTH, 
                                      &ui4_read);
            if (ASF_HEADER_TEST_LENGTH != ui4_read)
            {
                DBG_ERROR(("asf_handler_mem_confirm_fct:error read (%d)\r\n",
                           ui4_read));
                return MFMTRECGR_INTERNAL_ERR;
            }
            else
            {
                /**/
            }
            
        }
        else
        {
            DBG_ERROR(("asf_handler_mem_confirm_fct:set position error(%d)\r\n",
                       i4_ret));
            return MFMTRECGR_INTERNAL_ERR;
        }
    }


    /*try to get the first guid of a asf file */
    _asf_get_guid(&t_headerguid, (UINT8*)ac_asf_hdr);

    /*whether is the asf standard header guid?*/
    if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_headerguid, 
                                         &gt_asf_object_header_guid))
    {
        pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
        pt_format->t_media_subtype.u.e_contnr_subtype
                                = MEDIA_CONTNR_SUBTYPE_ASF;
        return MFMTRECGR_OK;
    }
    else
    {
        DBG_ERROR(("asf_handler_mem_confirm_fct:unknown\r\n"));
            /*set unknown file format*/
        pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
        pt_format->t_media_subtype.u.e_contnr_subtype 
                                = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
        return MFMTRECGR_INTERNAL_ERR;
    }
    #endif
 
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_mfmt_is_wma
 *
 * Description: _asf_mfmt_is_wma
 *
 * Inputs:   pt_input  : specify input function 
 *           pv_obj :    object input related obj 
 *          
 * Outputs:  NULL 
 * 
 * Returns:  whether is a wma file 
 *
 * Note:     NULL
 *
 * Author :  lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-20 : initial

 ----------------------------------------------------------------------------*/
BOOL _asf_mfmt_is_wma(MFMTRECG_SRC_INPUT_FCT_TBL_T* pt_input, VOID* pv_obj)
{
    copybytes_fct       pf_copybytes;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    INT32               i4_ret;
    UINT64              ui8_pos;
    UINT64              ui8_file_len;
    UINT16              ui2_video_count;
    UINT16              ui2_audio_count;
    UINT8               aui1_buffer[ASF_OBJ_COMMON_LEN];
    UINT32              ui4_read_count;
    ASF_GUID_T          t_guid;   
    UINT64              ui8_size;
    UINT64              ui8_header_size;
    UINT64              ui8_skip_len;
    
    if ((NULL == pt_input) || (NULL == pv_obj))
    {
        DBG_ERROR(("_asf_mfmt_is_wma:NULL\r\n"));
        return FALSE;
    }
    
    /*get input function*/
    pf_copybytes = pt_input->pf_copybytes;
    pf_set_pos = pt_input->pf_set_pos;
    pf_get_pos = pt_input->pf_get_pos;
    if ((NULL == pf_copybytes) 
        || (NULL == pf_set_pos) 
        || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_asf_mfmt_is_wma:input NULL\r\n"));
        return FALSE;
    }
    
    /*set file position at the end */
    i4_ret = pf_set_pos(pv_obj, 0, FM_SEEK_END);     
    if (INPUTR_OK != i4_ret)
    {
        return FALSE;
    }
    /*get the position*/
    i4_ret = pf_get_pos(pv_obj, &ui8_pos);
    if (INPUTR_OK != i4_ret)
    {
        return FALSE;
    }
    
    /*calculate the length*/
    ui8_file_len = ui8_pos;
    
    /*set file position at the beginning */
    i4_ret = pf_set_pos(pv_obj, 0, FM_SEEK_BGN);     
    if (INPUTR_OK != i4_ret)
    {
        return FALSE;
    }
    
    /* only parse stream properties object */
    ui2_video_count = 0;
    ui2_audio_count = 0;
    while (1)
    {
        /*read object GUID and size*/
        i4_ret = pf_copybytes(pv_obj, 
                              aui1_buffer, 
                              ASF_OBJ_COMMON_LEN,
                              ASF_OBJ_COMMON_LEN,
                              &ui4_read_count);
        if ((INPUTR_OK != i4_ret) || (ASF_OBJ_COMMON_LEN != ui4_read_count))
        {
            DBG_ERROR(("_asf_mfmt_is_wma:pf_copybytes error(%d)\r\n", 
                       i4_ret));
            return FALSE;
        }
        
        /*get object guid and size*/
        _asf_get_guid(&t_guid, aui1_buffer);
        /*GUID is 16 bytes long*/  
        ASF_GET_QWORD(ui8_size, aui1_buffer + 16);  

        /*if is heade object begin to pasre*/
        if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_guid, 
                                             &gt_asf_object_header_guid))
        {
            /*set at the beginnig of child objects, header object at least 30bytes*/
            i4_ret = pf_set_pos(pv_obj, 6, FM_SEEK_CUR);     
            if (INPUTR_OK != i4_ret)
            {
                return FALSE;
            }
            ui8_header_size = ui8_size - 30; /*30=ASF_OBJ_COMMON_LEN+6*/

            while (1)
            {
                /*read object GUID and size*/
                i4_ret = pf_copybytes(pv_obj, 
                                      aui1_buffer, 
                                      ASF_OBJ_COMMON_LEN,
                                      ASF_OBJ_COMMON_LEN,
                                      &ui4_read_count);
                if ((INPUTR_OK != i4_ret) || (ASF_OBJ_COMMON_LEN != ui4_read_count))
                {
                    DBG_ERROR(("_asf_mfmt_is_wma:pf_copybytes error(%d)\r\n", 
                               i4_ret));
                    return FALSE;
                }
                
                /*get object guid and size*/
                _asf_get_guid(&t_guid, aui1_buffer);
                /*GUID is 16 bytes long*/  
                ASF_GET_QWORD(ui8_size, aui1_buffer + 16);              
                
                /*if  stream properties object */
                if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_guid, 
                                       &gt_asf_object_stream_properties_guid))
                {
                    /*get stream type*/
                    /*read object GUID and size*/
                    i4_ret = pf_copybytes(pv_obj, 
                                          aui1_buffer, 
                                          ASF_OBJ_COMMON_LEN,
                                          16,
                                          &ui4_read_count);
                    if ((INPUTR_OK != i4_ret) || (16 != ui4_read_count))
                    {
                        DBG_ERROR(("_asf_mfmt_is_wma:pf_copybytes stream type "
                                   "error(%d)\r\n", 
                                   i4_ret));
                        return FALSE;
                    }
                    _asf_get_guid(&t_guid, aui1_buffer);
                    /*if video stream properties object set flag founf*/
                    if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_guid, 
                                       &gt_asf_object_stream_type_video))
                    {
                        ++ui2_video_count;
                    }
                    /*audio set flag found*/
                    else if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_guid, 
                                       &gt_asf_object_stream_type_audio))
                    {
                        ++ui2_audio_count;
                    }
                    else
                    {
                    }
                    /*16 is the stream type long*/
                    ui8_skip_len = ui8_size - ASF_OBJ_COMMON_LEN - 16;
                }
                /*just skip the object*/
                else
                {
                    ui8_skip_len = ui8_size - ASF_OBJ_COMMON_LEN;
                }
                
                i4_ret = pf_set_pos(pv_obj, 
                                    (INT64)ui8_skip_len, 
                                    FM_SEEK_CUR);     
                if (INPUTR_OK != i4_ret)
                {
                    return FALSE;
                }
               
                /*If reach to the end?*/
                ui8_header_size -= ui8_size;
                if (ui8_header_size <= 0)
                {
                    break;
                }
            }

            break;
        }
        /*skip the object*/
        else
        {
            i4_ret = pf_set_pos(pv_obj, 
                                (INT64)(ui8_size - ASF_OBJ_COMMON_LEN), 
                                FM_SEEK_CUR);     
            if (INPUTR_OK != i4_ret)
            {
                return FALSE;
            }
            i4_ret = pf_get_pos(pv_obj, &ui8_pos);
            if (INPUTR_OK != i4_ret)
            {
                return FALSE;
            }
           
            /*If reach to the end?*/
            if (ui8_pos >= ui8_file_len)
            {
                break;
            }
        }
    }
         
    /*if only audio found flag we consider the file is wma otherwise not*/
    if ((ui2_audio_count > 0) && (0 == ui2_video_count))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  asf_wma_handler_file_confirm_fct
 *
 * Description: this function will handle wma as same as mp3
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format       retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful 
 *                          and the operation has been completed.
 *          other non-zero value:  unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-6-19 : initial


 ----------------------------------------------------------------------------*/
INT32 asf_wma_handler_file_confirm_fct( HANDLE_T           h_file,        
                                        MEDIA_FORMAT_T*    pt_format)     
{
#ifndef __NO_FM__
    /*the current position of the file related to h_file*/
    UINT64                         ui8_cur_pos;    
    INT32                          i4_ret;  /*return code*/         
    /*indicates how many bytes are read*/  
    UINT32                         ui4_read;      
    ASF_GUID_T                     t_headerguid;
    CHAR                           ac_asf_hdr[ASF_HEADER_TEST_LENGTH];
    BOOL                           b_is_wma;
    MFMTRECG_SRC_INPUT_FCT_TBL_T   t_input_func;
    MM_UTIL_INPUT_T                t_input;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_format)
        || (NULL_HANDLE == h_file))
    {
        DBG_ERROR(("asf_wma_handler_file_confirm_fct: Null error\r\n"));
        return MFMTRECGR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init variables*/
    x_memset(&t_headerguid, 0, sizeof(ASF_GUID_T));
    x_memset(ac_asf_hdr, 0, sizeof(CHAR) * ASF_HEADER_TEST_LENGTH);

    pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
    pt_format->t_media_subtype.u.e_aud_subtype 
                            = MEDIA_AUD_SUBTYPE_UNKNOWN;

    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if (FMR_OK == i4_ret)
    {
        i4_ret = x_fm_read(h_file, ac_asf_hdr, ASF_HEADER_TEST_LENGTH, 
                          &ui4_read);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("asf_wma_handler_file_confirm_fct: x_fm_read "
                       "failed(%d)\r\n", 
                       i4_ret));
            return MFMTRECGR_FILE_ERR;
        }
        
        /*try to get the first guid of a asf file */
        _asf_get_guid(&t_headerguid, (UINT8*)ac_asf_hdr);
        /*whether is the asf standard header guid?*/
        if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_headerguid, 
                                             &gt_asf_object_header_guid))
        { 
            /* whether video stream is contained in stream properties object? */
            t_input_func.pf_copybytes = mm_util_fcopybytes_fct;
            t_input_func.pf_input1 = mm_util_finput1bytes_fct;
            t_input_func.pf_input2 = mm_util_finput2bytes_l_fct;
            t_input_func.pf_input3 = mm_util_finput3bytes_l_fct;
            t_input_func.pf_input4 = mm_util_finput4bytes_l_fct;
            t_input_func.pf_set_pos = mm_util_fsetpos_fct;
            t_input_func.pf_get_pos = mm_util_fgetpos_fct;

            t_input.h_file = h_file;
            
            b_is_wma = _asf_mfmt_is_wma(&t_input_func, (VOID*)(&t_input));
            if (TRUE == b_is_wma)
            {
                pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
                pt_format->t_media_subtype.u.e_aud_subtype
                                        = MEDIA_AUD_SUBTYPE_WMA;
                /*return MFMTRECGR_OK;*/
            }
            else
            {
                pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
                pt_format->t_media_subtype.u.e_contnr_subtype
                                        = MEDIA_CONTNR_SUBTYPE_ASF;
                /*return MFMTRECGR_INTERNAL_ERR;*/
            }
            return MFMTRECGR_OK;
        }

        DBG_ERROR(("asf_wma_handler_file_confirm_fct:unknown\r\n"));
        return MFMTRECGR_INTERNAL_ERR;
    }
    else
    {
        DBG_ERROR(("asf_wma_handler_file_confirm_fct:x_fm_lseek "
                   "failed(%d)\r\n", i4_ret));
        return MFMTRECGR_FILE_ERR;
    }
#else

    return MFMTRECGR_INTERNAL_ERR;

#endif

}

/*-----------------------------------------------------------------------------
 * Name:  asf_wma_handler_mem_confirm_fct
 *
 * Description: this function will handle wma as same as mp3
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer which want to 
 *                          retrieve the format.
 *          z_mem_leng      specify the length of memory buffer which want to 
 *                          retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the 
 *                          operation has been completed.
 *
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-6-19 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_wma_handler_mem_confirm_fct( const VOID*      pv_mem_data, 
                                       SIZE_T           z_mem_leng,   
                                       MEDIA_FORMAT_T*  pt_format ) 
{
    INT32                          i4_ret;  /*return code*/         
    /*indicates how many bytes are read*/  
    UINT32                         ui4_read;      
    ASF_GUID_T                     t_headerguid;
    CHAR                           ac_asf_hdr[ASF_HEADER_TEST_LENGTH];
    MEDIA_FEEDER_T*                pt_src_feeder;

    BOOL                           b_is_wma;
    MFMTRECG_SRC_INPUT_FCT_TBL_T   t_input_func;
    MM_UTIL_INPUT_T                t_input;

    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pt_format == NULL))
    {
        DBG_ERROR(("asf_handler_mem_confirm_fct: Null error\r\n"));
        return MFMTRECGR_INV_ARG;
    }
    

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init variables*/
    x_memset(&t_headerguid, 0, sizeof(ASF_GUID_T));
    x_memset(ac_asf_hdr, 0, sizeof(CHAR) * ASF_HEADER_TEST_LENGTH);


    /*if 0 != z_mem_leng read buffer directly*/
    if (0 != z_mem_leng)
    {
        x_memcpy(ac_asf_hdr, pv_mem_data, ASF_HEADER_TEST_LENGTH);
        t_input_func.pf_copybytes = mm_util_mcopybytes_fct;
        t_input_func.pf_input1 = mm_util_minput1bytes_fct;
        t_input_func.pf_input2 = mm_util_minput2bytes_l_fct;
        t_input_func.pf_input3 = mm_util_minput3bytes_l_fct;
        t_input_func.pf_input4 = mm_util_minput4bytes_l_fct;
        t_input_func.pf_set_pos = mm_util_msetpos_fct;
        t_input_func.pf_get_pos = mm_util_mgetpos_fct;

        t_input.pv_buff_or_file_path_name = (VOID*)pv_mem_data;
        t_input.z_buff_leng = z_mem_leng;
        t_input.i8_buff_pos = 0;

        
    }
    else
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

        

        /*set file position at the beginning*/
        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (MFDRR_OK == i4_ret)
        {
            i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes(
                                      &pt_src_feeder->h_feeder,
                                      ac_asf_hdr,
                                      ASF_HEADER_TEST_LENGTH,
                                      ASF_HEADER_TEST_LENGTH,
                                      &ui4_read);
            if (ASF_HEADER_TEST_LENGTH != ui4_read)
            {
                DBG_ERROR(("asf_handler_mem_confirm_fct:error read (%d)\r\n",
                           ui4_read));
                return MFMTRECGR_INTERNAL_ERR;
            }
            else
            {
                /**/
            }
            
        }
        else
        {
            DBG_ERROR(("asf_handler_mem_confirm_fct:set position error(%d)\r\n",
                       i4_ret));
            return MFMTRECGR_INTERNAL_ERR;
        }

        t_input_func.pf_copybytes = pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        t_input_func.pf_input1 = pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        t_input_func.pf_input2 = pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        t_input_func.pf_input3 = pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        t_input_func.pf_input4 = pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        t_input_func.pf_set_pos = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        t_input_func.pf_get_pos = pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;

        t_input.h_src_feeder = pt_src_feeder->h_feeder;

        i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (MFDRR_OK == i4_ret)
        {
            
        }
        else
        {
            DBG_ERROR(("asf_handler_mem_confirm_fct:set beginning error(%d)\r\n",
                       i4_ret));
            return MFMTRECGR_INTERNAL_ERR;
        }
        
    }


    /*try to get the first guid of a asf file */
    _asf_get_guid(&t_headerguid, (UINT8*)ac_asf_hdr);

    /*whether is the asf standard header guid?*/
    if (ASFR_GUID_EQUAL == _asf_cmp_guid(&t_headerguid, 
                                         &gt_asf_object_header_guid))
    {
        b_is_wma = _asf_mfmt_is_wma(&t_input_func, (VOID*)(&t_input));
        if (TRUE == b_is_wma)
        {
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            pt_format->t_media_subtype.u.e_aud_subtype
                                    = MEDIA_AUD_SUBTYPE_WMA;
            /*return MFMTRECGR_OK;*/
        }
        else
        {
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype
                                    = MEDIA_CONTNR_SUBTYPE_ASF;
            /*return MFMTRECGR_INTERNAL_ERR;*/
        }
        return MFMTRECGR_OK;
    }
    else
    {
        DBG_ERROR(("asf_handler_mem_confirm_fct:unknown\r\n"));
        /*set unknown file format*/
        pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
        pt_format->t_media_subtype.u.e_contnr_subtype
                                = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
        return MFMTRECGR_INTERNAL_ERR;
    }
 
}

#ifdef __cplusplus
    }
#endif

