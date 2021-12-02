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
 * ogg_minfo.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2010/06/10
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
#include "ogg_parser.h"
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
#include "ogg_parser.h"
#include "x_drm.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_OGG_BUFFER_LEN         1000
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
 * Name:  ogg_minfo_is_handler
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
 
INT32 ogg_minfo_is_handler(VOID*    pv_obj, MEDIA_FORMAT_T* pt_format)
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
    ( pt_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_fopen
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
INT32 ogg_minfo_fopen(CHAR*              ps_path_name, 
                     VOID*              pv_obj)
{

#ifndef __NO_FM__

    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_OGG_FILE_INF_T*    pt_ogg_file_inf;

    DBG_INFO(("API: ogg_minfo_fopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        DBG_ERROR(("ERROR: ogg_minfo_fopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for OGG file */
    pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_ogg_file_inf = (MINFO_OGG_FILE_INF_T*)_mm_util_mem_alloc(
        sizeof(MINFO_OGG_FILE_INF_T));
    if (pt_ogg_file_inf == NULL)
    {
        DBG_ERROR(("ERROR: ogg_minfo_fopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_ogg_file_inf, 0, sizeof(MINFO_OGG_FILE_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_ogg_file_inf;

    pt_ogg_file_inf->pui1_buffer = (UINT8*)_mm_util_mem_alloc(MAX_OGG_BUFFER_LEN);
    if (pt_ogg_file_inf->pui1_buffer == NULL)
    {
        DBG_ERROR(("ERROR: ogg_minfo_fopen alloc memory fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_ogg_file_inf->ui4_buf_len = MAX_OGG_BUFFER_LEN;
    
    DBG_INFO(("API: ogg_minfo_fopen OK \r\n"));    
    return MINFOR_OK;
    
#else
    return MINFOR_INITIAL_ERR ;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_mopen
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
INT32 ogg_minfo_mopen(VOID*              pv_mem_data, 
                      SIZE_T             z_mem_leng, 
                      VOID*              pv_obj)
{    
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_OGG_FILE_INF_T*    pt_ogg_file_inf;
    MEDIA_FEEDER_T*         pt_src_feeder; 

    DBG_INFO(("API: ogg_minfo_mopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pv_mem_data == NULL))
    {
        DBG_ERROR(("ERROR: ogg_minfo_mopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for OGG file */
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
	pt_minfo_obj->t_input_fct_tbl.pf_input2 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input2;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
    }
	
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_ogg_file_inf = (MINFO_OGG_FILE_INF_T*)_mm_util_mem_alloc(
        sizeof(MINFO_OGG_FILE_INF_T));
    if (pt_ogg_file_inf == NULL)
    {
        DBG_ERROR(("ERROR: ogg_minfo_mopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_ogg_file_inf, 0, sizeof(MINFO_OGG_FILE_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_ogg_file_inf;
    
    pt_ogg_file_inf->pui1_buffer = (UINT8*)_mm_util_mem_alloc(MAX_OGG_BUFFER_LEN);
    if (pt_ogg_file_inf->pui1_buffer == NULL)
    {
        DBG_ERROR(("ERROR: ogg_minfo_fopen alloc memory fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_ogg_file_inf->ui4_buf_len = MAX_OGG_BUFFER_LEN;
    DBG_INFO(("API: ogg_minfo_mopen OK \r\n"));
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_close
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
INT32 ogg_minfo_close( 
                        VOID*              pv_obj)
{
    MINFO_OBJ_T*            pt_minfo_obj;
	/*MINFO_OGG_FILE_INF_T*	pt_ogg_file_info;
   	 MINFO_RM_FILE_INF_T*    pt_rm_file_inf;*/

    DBG_INFO(("API: ogg_minfo_close begin \r\n"));
    
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
    else
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
    }

    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_parse
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
INT32 ogg_minfo_parse(VOID*              pv_obj)
{
    DBG_INFO(("API: ogg_minfo_parse begin \r\n"));
    
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
    /* parse ogg file boxes */
    return ogg_file_minfo_parse((MINFO_OBJ_T*)pv_obj);

} 

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_get_num_recs
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
INT32 ogg_minfo_get_num_recs(VOID*              pv_obj, 
                             MINFO_INFO_TYPE_T  e_info_type, 
                             UINT16*            pui2_num_recs)
{
    MINFO_OBJ_T*           pt_minfo_obj;
    MINFO_OGG_FILE_INF_T*   pt_ogg_file_inf;

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
    pt_ogg_file_inf = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;

    if (pt_ogg_file_inf)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: ogg_minfo_get_num_recs - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                *pui2_num_recs =1;
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                *pui2_num_recs = 
                    pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_video_cnt;
                break;
            case MINFO_INFO_TYPE_AUDIO_ATTR:
                *pui2_num_recs = 
                    pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_audio_cnt+
                    pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt;
                break;
            case MINFO_INFO_TYPE_SP_ATTR:
                *pui2_num_recs = 0;
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                *pui2_num_recs = 0;
                break;
            default:
                return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
}                       

/*-----------------------------------------------------------------------------
 * Name:  ogg_minfo_get_info
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
INT32 ogg_minfo_get_info(VOID*              pv_obj, 
                         MINFO_INFO_TYPE_T  e_info_type, 
                         MINFO_REC_IDX_T    ui2_index,                    
                         VOID*              pv_buf, 
                         SIZE_T*            pz_buf_leng)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_OGG_FILE_INF_T*   pt_ogg_file_inf;
    MINFO_INFO_T*           pt_minfo_rec;
    UINT8                   ui1_i = 0;
    UINT8                   ui1_cnt = 0;
    OGG_STREAM_INFO_T*  	pt_ogg_strm = NULL;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pz_buf_leng == NULL))
    {
        DBG_ERROR(("ERROR: ogg_minfo_get_info - invalid parameter \r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_ogg_file_inf = (MINFO_OGG_FILE_INF_T*)pt_minfo_obj->pv_handler_obj;
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
            DBG_ERROR(("ERROR: ogg_minfo_get_info - insufficient buffer \r\n"));  
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }

    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));

    if (pt_ogg_file_inf)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: ogg_minfo_get_info - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_general_info.ui8_pb_duration
                              =  pt_ogg_file_inf->t_generic_info.ui8_pb_duration;
                pt_minfo_rec->u.t_general_info.ui8_media_sz 
                              =  pt_ogg_file_inf->t_generic_info.ui8_media_sz;
                pt_minfo_rec->u.t_general_info.ui1_strm_ns = 
                                pt_ogg_file_inf->t_generic_info.ui1_strm_ns;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                {
                    if(ui2_index > 
                        pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_video_cnt)
                    {
                        DBG_ERROR
                          (("ERROR:ogg_minfo_get_info index>video stream num \r\n"));
                        return MINFOR_OUT_OF_RECORD;
                    }
                    for(ui1_i = 0; 
                        ui1_i < pt_ogg_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                        if (ui1_i >= OGG_STREAM_NUM_MAX)
                        {
                            return MINFOR_OUT_OF_RECORD;
                        }
                        if(pt_ogg_file_inf->t_file_layout.t_ogg_stream_info[ui1_i]
                                .t_ogg_strm_type == OGG_STRM_TYPE_VIDEO)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                if (ui1_cnt >= OGG_STREAM_NUM_MAX)
                                {
                                    return MINFOR_OUT_OF_RECORD;
                                }
                                pt_ogg_strm = &pt_ogg_file_inf->t_file_layout
                                                            .t_ogg_stream_info[ui1_i];
                                break;
                            }
                        }
                    }
                }
                if(pt_ogg_strm == NULL)
                {
                    DBG_ERROR(("ERROR: %d video stream not found \r\n", 
                        ui2_index));
                    return MINFOR_INTERNAL_ERR;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_ogg_stm_id 
                    = pt_ogg_strm->u.t_ogg_video_param.ui4_serial_num;
                pt_minfo_rec->u.t_stm_attribute.u.t_ogg_video_stm_attr 
                    = pt_ogg_strm->u.t_ogg_video_param;                
                if(pt_minfo_rec->u.t_stm_attribute.u.t_ogg_video_stm_attr.e_enc
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
                {
                    if(ui2_index > 
                        (pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_audio_cnt+
                        pt_ogg_file_inf->t_file_layout.t_ogg_stream_num.ui1_vorbis_cnt))
                    {
                        DBG_ERROR
                          (("ERROR:ogg_minfo_get_info index>audio stream num \r\n"));
                        return MINFOR_OUT_OF_RECORD;
                    }
                    for(ui1_i = 0; 
                        ui1_i < pt_ogg_file_inf->t_generic_info.ui1_strm_ns; 
                        ui1_i++)
                    {
                         if (ui1_i >= OGG_STREAM_NUM_MAX)
                        {
                            return MINFOR_OUT_OF_RECORD;
                        }
                        if(pt_ogg_file_inf->t_file_layout.t_ogg_stream_info[ui1_i]
                                .t_ogg_strm_type == OGG_STRM_TYPE_AUDIO)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                pt_ogg_strm = &pt_ogg_file_inf->t_file_layout
                                                            .t_ogg_stream_info[ui1_i];
                                if (pt_ogg_strm->u.t_ogg_audio_param.u.t_ogg_strm_audio_info.e_enc == MINFO_INFO_AUD_ENC_UNKNOWN)
                                {
                                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = FALSE;
                                }
                                else
                                {
                                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                                }
                                break;
                            }
                        }
                        else if(pt_ogg_file_inf->t_file_layout.t_ogg_stream_info[ui1_i]
                                .t_ogg_strm_type == OGG_STRM_TYPE_VORBIS)
                        {
                            ui1_cnt++;
                            if((UINT16)ui1_cnt == ui2_index)
                            {
                                pt_ogg_strm = &pt_ogg_file_inf->t_file_layout
                                                            .t_ogg_stream_info[ui1_i];
                                if (pt_ogg_strm->u.t_ogg_audio_param.u.t_ogg_strm_vorbis_info.e_enc == MINFO_INFO_AUD_ENC_UNKNOWN)
                                {
                                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = FALSE;
                                }
                                else
                                {
                                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                                }
                                break;
                            }
                        }
                    }
                }
                if(pt_ogg_strm == NULL)
                {
                    DBG_ERROR(("ERROR: %d audio stream not found \r\n", 
                        ui2_index));
                    return MINFOR_INFO_NOT_FOUND;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_ogg_stm_id 
                    = pt_ogg_strm->u.t_ogg_audio_param.ui4_serial_num;
                pt_minfo_rec->u.t_stm_attribute.u.t_ogg_audio_stm_attr
                    = pt_ogg_strm->u.t_ogg_audio_param;                           
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_idx_info.ui4_idx_type    = MM_MINFO_IDX_TYPE_NONE;
                pt_minfo_rec->u.t_idx_info.ui8_data_offset = 0;
                /*no idxbuild , but mm_hdlr.c range info gen will get range size from idx info*/
                pt_minfo_rec->u.t_idx_info.ui8_data_sz = pt_ogg_file_inf->t_generic_info.ui8_media_sz;

                if (pt_ogg_file_inf->b_data_offset_found)
                {
                    pt_minfo_rec->u.t_idx_info.ui8_data_offset = pt_ogg_file_inf->ui8_data_start_pts;
                }
                break;
            case MINFO_INFO_TYPE_DRM_INFO:
                pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
                break;
            case MINFO_INFO_TYPE_META_DATA:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_meta_data = pt_ogg_file_inf->t_meta_data_info;
                pt_minfo_rec->u.t_meta_data.ui8_pb_duration 
                    = pt_ogg_file_inf->t_generic_info.ui8_pb_duration;
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
