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
 * rm_minfo.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
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
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/mm_util_input.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "rm_parser.h"
#include "drm/x_drm.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "../../mm_util_input.h"
#include "x_dbg.h"
#include "u_dbg.h"

#include "../../mm_util.h"

#include "x_lnk_list.h"
#include "rm_parser.h"
#include "x_drm.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_RM_BUFFER_LEN         10000
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
 * Name:  rm_minfo_is_handler
 *
 * Description: This function confirm media format could be handled or not.
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
 
INT32 rm_minfo_is_handler(VOID*    pv_obj, MEDIA_FORMAT_T* pt_format)
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
    if((pt_format->e_media_type == MEDIA_TYPE_CONTAINER) && 
    ( pt_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_fopen(CHAR*              ps_path_name, 
                     VOID*              pv_obj)
{

#ifndef __NO_FM__

    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_RM_FILE_INF_T*    pt_rm_file_inf;

    DBG_INFO(("API: rm_minfo_fopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        DBG_ERROR(("ERROR: rm_minfo_fopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for RM file */
    pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_rm_file_inf = (MINFO_RM_FILE_INF_T*)_mm_util_mem_alloc(
        sizeof(MINFO_RM_FILE_INF_T));
    if (pt_rm_file_inf == NULL)
    {
        DBG_ERROR(("ERROR: rm_minfo_fopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_rm_file_inf, 0, sizeof(MINFO_RM_FILE_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_rm_file_inf;

    pt_rm_file_inf->pui1_buffer = (UINT8*)_mm_util_mem_alloc(MAX_RM_BUFFER_LEN);
    if (pt_rm_file_inf->pui1_buffer == NULL)
    {
        DBG_ERROR(("ERROR: rm_minfo_fopen alloc memory fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_rm_file_inf->ui4_buf_len = MAX_RM_BUFFER_LEN;
    
    DBG_INFO(("API: rm_minfo_fopen OK \r\n"));    
    return MINFOR_OK;
    
#else
    return MINFOR_INITIAL_ERR ;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_mopen(VOID*              pv_mem_data, 
                      SIZE_T             z_mem_leng, 
                      VOID*              pv_obj)
{    
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_RM_FILE_INF_T*    pt_rm_file_inf;
    MEDIA_FEEDER_T*         pt_src_feeder; 

    DBG_INFO(("API: rm_minfo_mopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pv_mem_data == NULL))
    {
        DBG_ERROR(("ERROR: rm_minfo_mopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for RM file */
    if(z_mem_leng != 0)
    {
        pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_minput2bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_b_fct;
    }
    else
    {
        /*INT32  i4_ret;*/
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_minfo_obj->h_src_feeder = pt_src_feeder->h_feeder;
        #if  0
        pt_minfo_obj->t_input_fct_tbl.pf_input2 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input2;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_copybytes;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos;
        if(pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos &&
           pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos )
        {
            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos
                (pt_src_feeder, 0, FM_SEEK_END);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
                return i4_ret;
            }
            pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos
                (pt_src_feeder, &pt_minfo_obj->z_buff_leng);     
        }
        #endif
    }
    
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_rm_file_inf = (MINFO_RM_FILE_INF_T*)_mm_util_mem_alloc(
        sizeof(MINFO_RM_FILE_INF_T));
    if (pt_rm_file_inf == NULL)
    {
        DBG_ERROR(("ERROR: rm_minfo_mopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_rm_file_inf, 0, sizeof(MINFO_RM_FILE_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_rm_file_inf;
    
    pt_rm_file_inf->pui1_buffer = (UINT8*)_mm_util_mem_alloc(MAX_RM_BUFFER_LEN);
    if (pt_rm_file_inf->pui1_buffer == NULL)
    {
        DBG_ERROR(("ERROR: rm_minfo_fopen alloc memory fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_rm_file_inf->ui4_buf_len = MAX_RM_BUFFER_LEN;
    DBG_INFO(("API: rm_minfo_mopen OK \r\n"));
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_close( 
                        VOID*              pv_obj)
{
    UINT8 ui1_i = 0;
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_RM_FILE_INF_T*    pt_rm_file_inf;

    DBG_INFO(("API: mp4_minfo_close begin \r\n"));
    
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
    if(pt_minfo_obj->pv_handler_obj  == NULL)
    {
        return MINFOR_INV_ARG;
    }
    pt_rm_file_inf = (MINFO_RM_FILE_INF_T*)(pt_minfo_obj->pv_handler_obj);
    
    /* free CONT mem */
    if(pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_title != NULL)
    {
        _mm_util_mem_free(
            pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_title);
    }
    if(pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_author != NULL)
    {
        _mm_util_mem_free(
            pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_author);
    }
    if(pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_copyright != NULL)
    {
        _mm_util_mem_free(
            pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_copyright);
    }
    if(pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_comment != NULL)
    {
        _mm_util_mem_free(
            pt_rm_file_inf->r_file_layout.t_file_cont_desp.pui1_comment);
    }
    /* free MDPR mem */
    for(; ui1_i<RM_STREAM_MAX-1; ui1_i++)
    {
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                                    .pui1_stream_name != NULL)
        {
            _mm_util_mem_free(pt_rm_file_inf->r_file_layout
                                    .r_strm_property[ui1_i].pui1_stream_name);
        }
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                                    .pui1_mime_type != NULL)
        {
            _mm_util_mem_free(pt_rm_file_inf->r_file_layout
                                    .r_strm_property[ui1_i].pui1_mime_type);
        }
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                            .pui1_type_specific_data != NULL)
        {
            _mm_util_mem_free(
                pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].
                pui1_type_specific_data);
        }
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                      .e_rm_strm_type == RM_STRM_TYPE_AUDIO)
        {
            if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                               .u.r_rm_audio_v5_param.pv_intlv_ptrn != NULL)
            {
                _mm_util_mem_free(pt_rm_file_inf->r_file_layout.
                    r_strm_property[ui1_i].u.r_rm_audio_v5_param.pv_intlv_ptrn);
            }
            if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].u
                .r_rm_audio_v5_param.pv_codec_opaque_data != NULL)
            {
                _mm_util_mem_free(pt_rm_file_inf->r_file_layout
                    .r_strm_property[ui1_i].u.r_rm_audio_v5_param
                    .pv_codec_opaque_data);
            }
        }
        if((pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type 
            == RM_STRM_TYPE_VIDEO) 
            &&
            (pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
            .u.r_rm_video_param.ui4_specific > 0)
            )
        {
            _mm_util_mem_free((VOID*)(pt_rm_file_inf->r_file_layout.
                r_strm_property[ui1_i].u.r_rm_video_param.ui4_specific));
        }
#ifdef RM_LOGICAL_STREAM_SUPPORT
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == RM_STRM_TYPE_LOGIC_AUDIO ||
           pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == RM_STRM_TYPE_LOGIC_VIDEO)
        {
            MINFO_RM_LOGIC_STM_STRUC_T* pr_rm_logic;
            RM_NAME_VAL_PROPERTY_T*     pr_val_prop;
            UINT16  ui2_idx;
            pr_rm_logic = &pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].u.r_rm_logic_strm;
            pr_val_prop = (RM_NAME_VAL_PROPERTY_T*)pr_rm_logic->pt_name_val_properties;
            
            if(pr_val_prop)
            {
                for(ui2_idx = 0; ui2_idx < pr_rm_logic->ui2_num_properties; ui2_idx++)
                {
                    if(pr_val_prop->pui1_name)
                    {
                        _mm_util_mem_free((VOID*)pr_val_prop->pui1_name);
                    }
                    if(pr_val_prop->pui1_val_data)
                    {
                        _mm_util_mem_free((VOID*)pr_val_prop->pui1_val_data);
                    }
                    pr_val_prop = (RM_NAME_VAL_PROPERTY_T*)((UINT8*)pr_val_prop + sizeof(RM_NAME_VAL_PROPERTY_T));
                }
                _mm_util_mem_free((VOID*)pr_rm_logic->pt_name_val_properties);
            }
            
            if(pr_rm_logic->pui2_rule_strm_num_map)
            {
                _mm_util_mem_free((VOID*)pr_rm_logic->pui2_rule_strm_num_map);
            }
            if(pr_rm_logic->pui2_strm_nums)
            {
                _mm_util_mem_free((VOID*)pr_rm_logic->pui2_strm_nums);
            }
            if(pr_rm_logic->pui4_data_offs)
            {
                _mm_util_mem_free((VOID*)pr_rm_logic->pui4_data_offs);
            }
        }
#endif
    }
    /* free DATA mem */
    for(ui1_i = 0; ui1_i < RM_STREAM_MAX-1; ui1_i++)
    {
        if(pt_rm_file_inf->r_file_layout.t_file_data[ui1_i].pt_data_block_records != NULL)
        {
            if(pt_rm_file_inf->r_file_layout.t_file_data[ui1_i]
                .pt_data_block_records->pui1_packet_data != NULL)
            {
                _mm_util_mem_free(pt_rm_file_inf->r_file_layout.t_file_data[ui1_i]
                                    .pt_data_block_records->pui1_packet_data);
            }
            _mm_util_mem_free(
                pt_rm_file_inf->r_file_layout.t_file_data[ui1_i].pt_data_block_records);
        }
    }
    /* free IDX mem */
    for(ui1_i = 0; ui1_i < RM_STREAM_MAX-1; ui1_i++)
    {
        if(pt_rm_file_inf->r_file_layout.r_strm_indx_tbl[ui1_i]
                           .pt_index_block_records != NULL)
        {
            _mm_util_mem_free(pt_rm_file_inf->r_file_layout
                                .r_strm_indx_tbl[ui1_i].pt_index_block_records);
        }
    }
    /*free temp buffer*/
    if(pt_rm_file_inf->pui1_buffer != NULL)
    {
        _mm_util_mem_free(pt_rm_file_inf->pui1_buffer);
    }
    _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
    pt_minfo_obj->pv_handler_obj = NULL;
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_parse
 *
 * Description: This function start parsing the infomation
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_parse(VOID*              pv_obj)
{
    DBG_INFO(("API: rm_minfo_parse begin \r\n"));
    
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
    /* parse rm file boxes */
    return rm_file_minfo_box_parse((MINFO_OBJ_T*)pv_obj);

} 

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_get_num_recs
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_get_num_recs(VOID*              pv_obj, 
                             MINFO_INFO_TYPE_T  e_info_type, 
                             UINT16*            pui2_num_recs)
{
    MINFO_OBJ_T*           pt_minfo_obj;
    MINFO_RM_FILE_INF_T*   pt_rm_file_inf;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pui2_num_recs == NULL))
    {
        DBG_ERROR(("ERROR: rm_minfo_get_num_recs - invalid parameter \r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_rm_file_inf = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;

    if (pt_rm_file_inf)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: rm_minfo_get_num_recs - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                *pui2_num_recs =1;
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                *pui2_num_recs = 
                    pt_rm_file_inf->r_file_layout.t_stream_num.ui1_video_cnt;
                break;
            case MINFO_INFO_TYPE_AUDIO_ATTR:
                *pui2_num_recs = 
                    pt_rm_file_inf->r_file_layout.t_stream_num.ui1_audio_cnt;
                break;
            case MINFO_INFO_TYPE_SP_ATTR:
                *pui2_num_recs = 0;
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                *pui2_num_recs = 
                    pt_rm_file_inf->r_file_layout.t_stream_num.ui1_idx_cnt;
                break;
            default:
                return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
}
                        

#ifdef RM_LOGICAL_STREAM_SUPPORT
INT32 _rm_minfo_get_data_offset(MINFO_RM_FILE_INF_T*    pt_rm_file_inf,
                                     RM_MEDIA_PROPERTIY_T*   pr_rm_strm,
                                     UINT32*                 pui4_data_offset,
                                     UINT32*                 pui4_data_size)
{
    UINT8                   ui1_i               = 0;
    UINT8                   ui1_i2              = 0;
    UINT8                   ui1_i3              = 0;
    UINT8                   ui1_strm_ns         = 0;
    RM_STRM_TYPE_T          e_rm_strm_type      = RM_STRM_TYPE_LOGIC_VIDEO;  
    
    if(pr_rm_strm == NULL ||
       pui4_data_offset == NULL)
    {
        return MINFOR_INV_ARG;
    }

    *pui4_data_offset = 0;
    *pui4_data_size = 0;
    ui1_strm_ns = pt_rm_file_inf->r_file_layout.t_stream_num.ui1_video_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_audio_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt ;
    
    if(pr_rm_strm->e_rm_strm_type == RM_STRM_TYPE_VIDEO)
    {
        e_rm_strm_type = RM_STRM_TYPE_LOGIC_VIDEO;
    }
    else if(pr_rm_strm->e_rm_strm_type == RM_STRM_TYPE_AUDIO)
    {
        e_rm_strm_type = RM_STRM_TYPE_LOGIC_AUDIO;
    }
    else 
    {
        return MINFOR_FCT_NOT_SUPPORT;
    }
    
    for(ui1_i = 0; ui1_i < ui1_strm_ns; ui1_i++)
    {
        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == e_rm_strm_type)
        {
            for(ui1_i2 = 0; 
                ui1_i2 < pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].u.r_rm_logic_strm.ui2_num_strms; 
                ui1_i2++)
            {
                if(pr_rm_strm->ui2_stream_number == 
                   pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].u.r_rm_logic_strm.pui2_strm_nums[ui1_i2])
                {
                    *pui4_data_offset = 
                        pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].u.r_rm_logic_strm.pui4_data_offs[ui1_i2];
                    for(ui1_i3 = 0; ui1_i3 < pt_rm_file_inf->r_file_layout.t_stream_num.ui1_data_cnt; ui1_i3++)
                    {
                        if(pt_rm_file_inf->r_file_layout.t_file_data[ui1_i3].t_data_block_head.ui4_offset == *pui4_data_offset)
                        {
                            *pui4_data_size = 
                                pt_rm_file_inf->r_file_layout.t_file_data[ui1_i3].t_data_block_head.ui4_size;
                            return MINFOR_OK;
                        }
                    }
                }
            }
            
        }
    }
    
    return MINFOR_FCT_NOT_SUPPORT;
}

INT32 _rm_minfo_select(RM_STRM_TYPE_T          e_strm_type,
                          MINFO_RM_FILE_INF_T*    pt_rm_file_inf,
                          RM_MEDIA_PROPERTIY_T**  ppr_rm_strm,
                          UINT32*                 pui4_data_offset,
                          UINT32*                 pui4_data_size)
{
    RM_MEDIA_PROPERTIY_T*   pr_rm_strm          = NULL;
    RM_MEDIA_PROPERTIY_T*   pr_rm_tmp_strm      = NULL;
    UINT8                   ui1_i               = 0;
    UINT8                   ui1_strm_ns         = 0;

    if(pt_rm_file_inf == NULL ||
       ppr_rm_strm == NULL)
    {
        return MINFOR_INV_ARG;
    }

    ui1_strm_ns = pt_rm_file_inf->r_file_layout.t_stream_num.ui1_video_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_audio_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                  pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt ;
    switch(e_strm_type)
    {
    case RM_STRM_TYPE_VIDEO:
        for(ui1_i = 0; ui1_i < ui1_strm_ns; ui1_i++)
        {
            if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == RM_STRM_TYPE_VIDEO)
            {
                pr_rm_tmp_strm = &pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i];
                if((pr_rm_strm == NULL) ||
                       (pr_rm_tmp_strm->u.r_rm_video_param.e_enc != MINFO_INFO_VID_ENC_UNKNOWN) &&
                       (pr_rm_strm->u.r_rm_video_param.e_enc == MINFO_INFO_VID_ENC_UNKNOWN ||
                            pr_rm_strm->u.r_rm_video_param.e_enc != MINFO_INFO_VID_ENC_UNKNOWN &&
                           (pr_rm_strm->u.r_rm_video_param.ui2_frm_width * pr_rm_strm->u.r_rm_video_param.ui2_frm_height <
                            pr_rm_tmp_strm->u.r_rm_video_param.ui2_frm_width * pr_rm_tmp_strm->u.r_rm_video_param.ui2_frm_height ||
                            pr_rm_strm->u.r_rm_video_param.ui2_frm_width * pr_rm_strm->u.r_rm_video_param.ui2_frm_height ==
                            pr_rm_tmp_strm->u.r_rm_video_param.ui2_frm_width * pr_rm_tmp_strm->u.r_rm_video_param.ui2_frm_height &&
                            pr_rm_strm->u.r_rm_video_param.ui4_avg_bit_rate < pr_rm_tmp_strm->u.r_rm_video_param.ui4_avg_bit_rate)))
                {
                    if(_rm_minfo_get_data_offset(pt_rm_file_inf, pr_rm_tmp_strm, pui4_data_offset,pui4_data_size) == MINFOR_OK)
                    {
                        pr_rm_strm = pr_rm_tmp_strm;
                    }
                }
            }
        }
        break;

    case RM_STRM_TYPE_AUDIO:
        for(ui1_i = 0; ui1_i < ui1_strm_ns; ui1_i++)
        {
            if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == RM_STRM_TYPE_AUDIO)
            {
                pr_rm_tmp_strm = &pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i];
                if(pr_rm_strm == NULL ||
                    (pr_rm_tmp_strm->u.r_rm_audio_v5_param.e_enc != MINFO_INFO_AUD_ENC_UNKNOWN) && 
                    (pr_rm_strm->u.r_rm_audio_v5_param.e_enc == MINFO_INFO_AUD_ENC_UNKNOWN ||
                           pr_rm_strm->u.r_rm_audio_v5_param.e_enc != MINFO_INFO_AUD_ENC_UNKNOWN &&
                           (pr_rm_strm->u.r_rm_audio_v5_param.ui2_num_channel < 
                            pr_rm_tmp_strm->u.r_rm_audio_v5_param.ui2_num_channel ||
                            pr_rm_strm->u.r_rm_audio_v5_param.ui2_num_channel == 
                            pr_rm_tmp_strm->u.r_rm_audio_v5_param.ui2_num_channel &&
                            pr_rm_strm->u.r_rm_audio_v5_param.ui4_sample_rate < 
                            pr_rm_tmp_strm->u.r_rm_audio_v5_param.ui4_sample_rate)))
                {
                    if(_rm_minfo_get_data_offset(pt_rm_file_inf, pr_rm_tmp_strm, pui4_data_offset,pui4_data_size) == MINFOR_OK)
                    {
                        pr_rm_strm = pr_rm_tmp_strm;
                    }
                }
            }
        }
        break;

    default:
        return MINFOR_FCT_NOT_SUPPORT;
    }

    *ppr_rm_strm = pr_rm_strm;
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  rm_minfo_get_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 rm_minfo_get_info(VOID*              pv_obj, 
                         MINFO_INFO_TYPE_T  e_info_type, 
                         MINFO_REC_IDX_T    ui2_index,                    
                         VOID*              pv_buf, 
                         SIZE_T*            pz_buf_leng)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_RM_FILE_INF_T*    pt_rm_file_inf;
    MINFO_INFO_T*           pt_minfo_rec;
    UINT8                   ui1_i = 0;
    UINT8                   ui1_cnt = 0;
#ifdef RM_LOGICAL_STREAM_SUPPORT
    UINT32                  ui4_data_offset = 0;
    UINT32                  ui4_data_size = 0;
#endif
    RM_MEDIA_PROPERTIY_T*   pr_rm_strm = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pz_buf_leng == NULL))
    {
        DBG_ERROR(("ERROR: rm_minfo_get_info - invalid parameter \r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_rm_file_inf = (MINFO_RM_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
    if (pv_buf == NULL)
    {   
        /*get length only*/
        *pz_buf_leng = sizeof (MINFO_INFO_T);
        return MINFOR_OK;
    }
    else
    {
        /* get data */
        if (*pz_buf_leng < sizeof (MINFO_INFO_T))
        {
            DBG_ERROR(("ERROR: rm_minfo_get_info - insufficient buffer \r\n"));  
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }

    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));

    if (pt_rm_file_inf)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: rm_minfo_get_info - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_general_info.ui8_pb_duration = 
                                pt_rm_file_inf->t_generic_info.ui8_pb_duration;
                pt_minfo_rec->u.t_general_info.ui8_media_sz= 
                                pt_rm_file_inf->t_generic_info.ui8_media_sz;
                pt_minfo_rec->u.t_general_info.ui1_strm_ns= 
                                pt_rm_file_inf->t_generic_info.ui1_strm_ns;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
#ifdef RM_LOGICAL_STREAM_SUPPORT
                if(pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                   pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt > 0)
                {
                    _rm_minfo_select(RM_STRM_TYPE_VIDEO, 
                        pt_rm_file_inf, 
                        &pr_rm_strm, 
                        &ui4_data_offset,
                        &ui4_data_size);
                }
                if(pr_rm_strm == NULL)
#endif
                {
                    if(ui2_index > 
                        pt_rm_file_inf->r_file_layout.t_stream_num.ui1_video_cnt)
                    {
                        DBG_ERROR
                          (("ERROR:rm_minfo_get_info index>video stream num \r\n"));
                        return MINFOR_OUT_OF_RECORD;
                    }
                    for(ui1_i = 0; 
                        ui1_i < pt_rm_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                .e_rm_strm_type == RM_STRM_TYPE_VIDEO)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                pr_rm_strm = &pt_rm_file_inf->r_file_layout
                                                            .r_strm_property[ui1_i];
                                break;
                            }
                        }
                    }
                }
                if(pr_rm_strm == NULL)
                {
                    DBG_ERROR(("ERROR: %d video stream not found \r\n", 
                        ui2_index));
                    return MINFOR_INTERNAL_ERR;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_rm_stm_id 
                    = pr_rm_strm->ui2_stream_number;
                pt_minfo_rec->u.t_stm_attribute.u.t_rm_video_stm_attr 
                    = pr_rm_strm->u.r_rm_video_param;                
                if(pt_minfo_rec->u.t_stm_attribute.u.t_rm_video_stm_attr.e_enc
                    == MINFO_INFO_VID_ENC_UNKNOWN)
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = FALSE;
                }
                else
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_AUDIO_ATTR:
#ifdef RM_LOGICAL_STREAM_SUPPORT
                if(pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                   pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt > 0)
                {
                    _rm_minfo_select(RM_STRM_TYPE_AUDIO, 
                        pt_rm_file_inf, 
                        &pr_rm_strm, 
                        &ui4_data_offset,
                        &ui4_data_size);
                }
                if(pr_rm_strm == NULL)
#endif
                {
                    if(ui2_index > 
                        pt_rm_file_inf->r_file_layout.t_stream_num.ui1_audio_cnt)
                    {
                        DBG_ERROR
                          (("ERROR:rm_minfo_get_info index>audio stream num \r\n"));
                        return MINFOR_OUT_OF_RECORD;
                    }
                    for(ui1_i = 0; 
                        ui1_i < pt_rm_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                .e_rm_strm_type == RM_STRM_TYPE_AUDIO)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                pr_rm_strm = &pt_rm_file_inf->r_file_layout
                                                            .r_strm_property[ui1_i];
                                break;
                            }
                        }
                    }
                }
                if(pr_rm_strm == NULL)
                {
                    DBG_ERROR(("ERROR: %d audio stream not found \r\n", 
                        ui2_index));
                    return MINFOR_INTERNAL_ERR;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_rm_stm_id 
                    = pr_rm_strm->ui2_stream_number;
                pt_minfo_rec->u.t_stm_attribute.u.t_rm_audio_stm_attr
                    = pr_rm_strm->u.r_rm_audio_v5_param;                           
                if(pt_minfo_rec->u.t_stm_attribute.u.t_rm_audio_stm_attr.e_enc
                    == MINFO_INFO_AUD_ENC_UNKNOWN)
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = FALSE;
                }
                else
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
                pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 
                       pt_rm_file_inf->r_file_layout.t_file_property.ui4_index_offset;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
#ifdef RM_LOGICAL_STREAM_SUPPORT
                if(pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                   pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt > 0)
                {
                    /*get audio data offset and size*/
                    _rm_minfo_select(RM_STRM_TYPE_AUDIO, 
                        pt_rm_file_inf, 
                        &pr_rm_strm, 
                        &ui4_data_offset,
                        &ui4_data_size);
                    pt_minfo_rec->u.t_idx_info.ui8_data_offset2 = ui4_data_offset;
                    pt_minfo_rec->u.t_idx_info.ui8_data_sz2 = ui4_data_size;
                    
                    _rm_minfo_select(RM_STRM_TYPE_VIDEO, 
                        pt_rm_file_inf, 
                        &pr_rm_strm, 
                        &ui4_data_offset,
                        &ui4_data_size);
                }
                if(pr_rm_strm)
                {
                    pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 
                        pt_rm_file_inf->r_file_layout.r_strm_indx_tbl[pr_rm_strm->ui2_stream_number]
                        .t_index_block_head.ui4_cur_index_header;
                    pt_minfo_rec->u.t_idx_info.ui8_idx_sz = 
                        pt_rm_file_inf->r_file_layout.r_strm_indx_tbl[pr_rm_strm->ui2_stream_number]
                        .t_index_block_head.ui4_size;
                    pt_minfo_rec->u.t_idx_info.ui8_data_offset = ui4_data_offset;
                    pt_minfo_rec->u.t_idx_info.ui8_data_sz = ui4_data_size;
                }
                else
#endif
                {
                    for(ui1_i = 0; 
                        ui1_i < pt_rm_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].e_rm_strm_type == 
                           RM_STRM_TYPE_VIDEO)
                        {
                            break;
                        }
                    }
                    pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 
                        pt_rm_file_inf->r_file_layout.r_strm_indx_tbl
                        [pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i].ui2_stream_number]
                        .t_index_block_head.ui4_cur_index_header;
                    pt_minfo_rec->u.t_idx_info.ui8_idx_sz = 
                        pt_rm_file_inf->r_file_layout.r_strm_indx_tbl[pt_minfo_rec->ui1_rec_idx]
                        .t_index_block_head.ui4_size;
                    pt_minfo_rec->u.t_idx_info.ui8_data_offset = 
                        pt_rm_file_inf->r_file_layout.t_file_property.ui4_data_offset;
                    pt_minfo_rec->u.t_idx_info.ui8_data_sz = 
                        pt_rm_file_inf->r_file_layout.t_file_data[0].t_data_block_head.ui4_size;
                }
                if(pt_minfo_rec->u.t_idx_info.ui8_idx_offset == 0)
                {
                    pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
                }
                break;
            case MINFO_INFO_TYPE_DRM_INFO:
                pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
                break;
            case MINFO_INFO_TYPE_META_DATA:
                pt_minfo_rec->u.t_meta_data = pt_rm_file_inf->t_meta_data_info;
                pt_minfo_rec->u.t_meta_data.ui8_pb_duration 
                    = pt_rm_file_inf->t_generic_info.ui8_pb_duration;
				
#ifdef RM_LOGICAL_STREAM_SUPPORT
                if(pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_video_cnt +
                   pt_rm_file_inf->r_file_layout.t_stream_num.ui1_logic_audio_cnt > 0)
                {
                    _rm_minfo_select(RM_STRM_TYPE_VIDEO, 
                        pt_rm_file_inf, 
                        &pr_rm_strm, 
                        &ui4_data_offset,
                        &ui4_data_size);
					pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_width = pr_rm_strm->u.r_rm_video_param.ui2_frm_width;
					pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_height = pr_rm_strm->u.r_rm_video_param.ui2_frm_height;
                }
                if(pr_rm_strm == NULL)
#endif
                {
                    if(ui2_index > 
                        pt_rm_file_inf->r_file_layout.t_stream_num.ui1_video_cnt)
                    {
                        DBG_ERROR
                          (("ERROR:rm_minfo_get_info index>video stream num \r\n"));
                        return MINFOR_OUT_OF_RECORD;
                    }
                    for(ui1_i = 0; 
                        ui1_i < pt_rm_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                        if(pt_rm_file_inf->r_file_layout.r_strm_property[ui1_i]
                                .e_rm_strm_type == RM_STRM_TYPE_VIDEO)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_width = pt_rm_file_inf->r_file_layout
                                                            .r_strm_property[ui1_i].u.r_rm_video_param.ui2_frm_width;

								pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_height = pt_rm_file_inf->r_file_layout
                                                            .r_strm_property[ui1_i].u.r_rm_video_param.ui2_frm_height;
                                break;
                            }
                        }
                    }
                }
                break;
            /*case MINFO_INFO_TYPE_SP_ATTR:
                            break;*/
            default:
                return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
} 
