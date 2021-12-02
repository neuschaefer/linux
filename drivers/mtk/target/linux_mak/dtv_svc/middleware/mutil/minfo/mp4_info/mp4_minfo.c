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
 * mp4_minfo.c,v
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
#include "mp4_parser.h"
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
#include "mp4_parser.h"
#include "x_drm.h"

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
 * Name:  mp4_minfo_is_handler
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
 
INT32 mp4_minfo_is_handler(VOID*    pv_obj, MEDIA_FORMAT_T* pt_format)
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
    ( pt_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_fopen
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
INT32 mp4_minfo_fopen(CHAR*              ps_path_name, 
                      VOID*              pv_obj)
{

#ifndef __NO_FM__

    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_moov;

    DBG_INFO(("API: mp4_minfo_fopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        DBG_ERROR(("ERROR: mp4_minfo_fopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for MP4 file */
    pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_mp4_moov = (MINFO_MP4_MOOV_INF_T*)_mm_util_mem_alloc(sizeof(MINFO_MP4_MOOV_INF_T));
    pt_minfo_obj->pv_handler_obj = pt_mp4_moov;
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        DBG_ERROR(("ERROR: mp4_minfo_fopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_moov, 0, sizeof(MINFO_MP4_MOOV_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_mp4_moov;
    
    pt_mp4_moov->pui1_box_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8)*MP4_BOX_BUF_LENGTH);
    if (pt_mp4_moov->pui1_box_buf  == NULL)
    {
        DBG_ERROR(("ERROR: mp4_minfo_fopen alloc memory02 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_moov->pui1_box_buf , 0, sizeof(UINT8)*MP4_BOX_BUF_LENGTH);
    pt_mp4_moov->ui4_buf_length = MP4_BOX_BUF_LENGTH;
    pt_mp4_moov->t_mp4_multi_file_info.ui1_file_num = 1;

    DBG_INFO(("API: mp4_minfo_fopen OK \r\n"));    
    return MINFOR_OK;
    
#else
    return MINFOR_INITIAL_ERR ;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_mopen
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
INT32 mp4_minfo_mopen(VOID*              pv_mem_data, 
                      SIZE_T             z_mem_leng, 
                      VOID*              pv_obj)
{    
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_moov;
    MEDIA_FEEDER_T*         pt_src_feeder; 

    DBG_INFO(("API: mp4_minfo_mopen begin \r\n"));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pv_mem_data == NULL))
    {
        DBG_ERROR(("ERROR: mp4_minfo_mopen Empty point variable\r\n"));
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /* Big Endian input for MP4 file */
    if(z_mem_leng != 0)
    {
        pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_minput2bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_b_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_b_fct;
    }
    else
    {
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
    pt_mp4_moov = (MINFO_MP4_MOOV_INF_T*)_mm_util_mem_alloc(sizeof(MINFO_MP4_MOOV_INF_T));
    pt_minfo_obj->pv_handler_obj = pt_mp4_moov;
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        DBG_ERROR(("ERROR: mp4_minfo_mopen alloc memory01 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_moov, 0, sizeof(MINFO_MP4_MOOV_INF_T));
    pt_minfo_obj->pv_handler_obj = (VOID*)pt_mp4_moov;
    
    pt_mp4_moov->pui1_box_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8)*MP4_BOX_BUF_LENGTH);
    if (pt_mp4_moov->pui1_box_buf  == NULL)
    {
        DBG_ERROR(("ERROR: mp4_minfo_mopen alloc memory02 fail \r\n"));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_moov->pui1_box_buf , 0, sizeof(UINT8)*MP4_BOX_BUF_LENGTH);
    pt_mp4_moov->ui4_buf_length = MP4_BOX_BUF_LENGTH;
    pt_mp4_moov->t_mp4_multi_file_info.ui1_file_num = 1;

    pt_mp4_moov->b_fake_cmov = TRUE;

    DBG_INFO(("API: mp4_minfo_mopen OK \r\n"));
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_close
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
INT32 mp4_minfo_close( 
                        VOID*              pv_obj)
{
    UINT8                   ui1_tmp;
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_moov_info;

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
    pt_mp4_moov_info = ((MINFO_MP4_MOOV_INF_T*)(pt_minfo_obj->pv_handler_obj));

    if(pt_mp4_moov_info == NULL)
    {
        return MINFOR_OK;
    }
    if(pt_mp4_moov_info->pui1_box_buf != NULL)
    {
        _mm_util_mem_free(((MINFO_MP4_MOOV_INF_T*)(pt_minfo_obj->pv_handler_obj))->pui1_box_buf);
        ((MINFO_MP4_MOOV_INF_T*)(pt_minfo_obj->pv_handler_obj))->pui1_box_buf  = NULL;
    }

    for(ui1_tmp = 0; ui1_tmp < pt_mp4_moov_info->t_strm_Info.ui1_video_strm_num; ui1_tmp++)
    {
        if(ui1_tmp >= MAX_NS_MP4_VID)
        {
            break;
        }
        if(pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_codec_info)
        {
            _mm_util_mem_free(pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_codec_info);
            pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_codec_info = NULL;
        }
        if(pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_mvcC_codec_info)
        {
            _mm_util_mem_free(pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_mvcC_codec_info);
            pt_mp4_moov_info->t_strm_Info.rVideStrmInf[ui1_tmp].pui1_mvcC_codec_info = NULL;
        }
    }
    
    for(ui1_tmp = 0; ui1_tmp < pt_mp4_moov_info->t_strm_Info.ui1_audio_strm_num; ui1_tmp++)
    {
        if(ui1_tmp >= MAX_NS_MP4_AUD)
        {
            break;
        }
        if(pt_mp4_moov_info->t_strm_Info.rAudStrmInf[ui1_tmp].pui1_codec_info)
        {
            _mm_util_mem_free(pt_mp4_moov_info->t_strm_Info.rAudStrmInf[ui1_tmp].pui1_codec_info);
            pt_mp4_moov_info->t_strm_Info.rAudStrmInf[ui1_tmp].pui1_codec_info = NULL;
        }
    }
    if (pt_mp4_moov_info->t_meta_data_info.pwc_str_title != NULL)
    {
        _mm_util_mem_free(pt_mp4_moov_info->t_meta_data_info.pwc_str_title);
        pt_mp4_moov_info->t_meta_data_info.pwc_str_title = NULL;
    }
    if (pt_mp4_moov_info->t_meta_data_info.pwc_str_director != NULL)
    {
        _mm_util_mem_free(pt_mp4_moov_info->t_meta_data_info.pwc_str_director);
        pt_mp4_moov_info->t_meta_data_info.pwc_str_director = NULL;
    }
    if (pt_mp4_moov_info->t_meta_data_info.pwc_str_copyright != NULL)
    {
        _mm_util_mem_free(pt_mp4_moov_info->t_meta_data_info.pwc_str_copyright);
        pt_mp4_moov_info->t_meta_data_info.pwc_str_copyright = NULL;
    }
    if (pt_mp4_moov_info->t_meta_data_info.pwc_str_genre != NULL)
    {
        _mm_util_mem_free(pt_mp4_moov_info->t_meta_data_info.pwc_str_genre);
        pt_mp4_moov_info->t_meta_data_info.pwc_str_genre = NULL;
    }
#if (ENABLE_CMOV == 1)
    if (pt_mp4_moov_info->t_mp4_box_cmov.pui1_decomp_cmov_data != NULL)
    {
        _mm_util_mem_free(pt_mp4_moov_info->t_mp4_box_cmov.pui1_decomp_cmov_data);
        pt_mp4_moov_info->t_mp4_box_cmov.pui1_decomp_cmov_data = NULL;
    }
#endif

    for(ui1_tmp = 0; ui1_tmp<MAX_MARLIN_TRACK_NUM; ui1_tmp++)
    {
        MINFO_MP4_MARLIN_TRACK_INFO_T*   pt_mln_trak_inf = NULL;
        pt_mln_trak_inf = &(pt_mp4_moov_info->t_strm_Info.at_marlin_track_info[ui1_tmp]);
        if(pt_mln_trak_inf->pui1_ct_id != NULL)
        {
            _mm_util_mem_free(pt_mln_trak_inf->pui1_ct_id);
            pt_mln_trak_inf->pui1_ct_id = NULL;
        }
        if(pt_mln_trak_inf->pui1_satr_ctm != NULL)
        {
            _mm_util_mem_free(pt_mln_trak_inf->pui1_satr_ctm);
            pt_mln_trak_inf->pui1_satr_ctm = NULL;
        }
        if(pt_mln_trak_inf->pui1_satr_hmac_val != NULL)
        {
            _mm_util_mem_free(pt_mln_trak_inf->pui1_satr_hmac_val);
            pt_mln_trak_inf->pui1_satr_hmac_val = NULL;
        }
    }
    if (pt_minfo_obj->pv_handler_obj != NULL)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_parse
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
INT32 mp4_minfo_parse(VOID*              pv_obj)
{
    DBG_INFO(("API: mp4_minfo_parse begin \r\n"));
    
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
    /* parse mp4 file boxes */
    return mp4_minfo_box_parse((MINFO_OBJ_T*)pv_obj);

} 

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_get_num_recs
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
INT32 mp4_minfo_get_num_recs(VOID*              pv_obj, 
                             MINFO_INFO_TYPE_T  e_info_type, 
                             UINT16*            pui2_num_recs)
{
    MINFO_OBJ_T*           pt_minfo_obj;
    MINFO_MP4_MOOV_INF_T*  pt_mp4_handler;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pui2_num_recs == NULL))
    {
        DBG_ERROR(("ERROR: mp4_minfo_get_num_recs - invalid parameter \r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_mp4_handler = pt_minfo_obj->pv_handler_obj;

    if (pt_mp4_handler)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: mp4_minfo_get_num_recs - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                *pui2_num_recs =1;
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                *pui2_num_recs = pt_mp4_handler->t_strm_Info.ui1_video_strm_num;
                break;
            case MINFO_INFO_TYPE_AUDIO_ATTR:
                *pui2_num_recs = pt_mp4_handler->t_strm_Info.ui1_audio_strm_num;
                break;
            case MINFO_INFO_TYPE_SP_ATTR:
                *pui2_num_recs = pt_mp4_handler->t_strm_Info.ui1_sbtl_strm_num;
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                *pui2_num_recs = 1;
                break;
            default:
                return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
}
                        

/*-----------------------------------------------------------------------------
 * Name:  mp4_minfo_get_info
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
INT32 mp4_minfo_get_info(VOID*              pv_obj, 
                         MINFO_INFO_TYPE_T  e_info_type, 
                         MINFO_REC_IDX_T    ui2_index,                    
                         VOID*              pv_buf, 
                         SIZE_T*            pz_buf_leng)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_MP4_MOOV_INF_T*   pt_mp4_handler;
    MINFO_INFO_T*           pt_minfo_rec;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pz_buf_leng == NULL))
    {
        DBG_ERROR(("ERROR: mp4_minfo_get_info - invalid parameter \r\n"));
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_mp4_handler = pt_minfo_obj->pv_handler_obj;
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
            DBG_ERROR(("ERROR: mp4_minfo_get_info - insufficient buffer \r\n"));  
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }
    
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    /*x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));*/

    if (pt_mp4_handler)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            DBG_ERROR(("ERROR: mp4_minfo_get_info - not ready \r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_general_info.ui8_pb_duration = 
                    pt_mp4_handler->t_generic_info.ui8_pb_duration;
                pt_minfo_rec->u.t_general_info.ui8_media_sz= 
                    pt_mp4_handler->t_generic_info.ui8_media_sz;
                pt_minfo_rec->u.t_general_info.ui1_strm_ns= 
                    pt_mp4_handler->t_generic_info.ui1_strm_ns;
                pt_minfo_rec->u.t_general_info.b_is_drm_exist = 
                    pt_mp4_handler->t_generic_info.b_is_drm_exist;
                pt_minfo_rec->u.t_general_info.b_non_seekable = 
                    pt_mp4_handler->t_generic_info.b_non_seekable;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                if((ui2_index > pt_mp4_handler->t_strm_Info.ui1_video_strm_num)
                  ||(ui2_index == 0)
                  ||(ui2_index > MAX_NS_MP4_VID))
                {
                    DBG_ERROR(("ERROR: mp4_minfo_get_info index > video stream num \r\n"));
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.u.t_mp4i_video_stm_attr = 
                    pt_mp4_handler->t_strm_Info.rVideStrmInf[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id = 
                    pt_mp4_handler->t_strm_Info.t_video_strm_ID[ui2_index-1]; 
                if(pt_minfo_rec->u.t_stm_attribute.u.t_mp4i_video_stm_attr.e_enc
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
                if((ui2_index > pt_mp4_handler->t_strm_Info.ui1_audio_strm_num)
                  ||(ui2_index == 0)
                  ||(ui2_index > MAX_NS_MP4_AUD))
                {
                    DBG_ERROR(("ERROR: mp4_minfo_get_info index > audio stream num \r\n"));
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.u.t_mp4i_audio_stm_attr = 
                    pt_mp4_handler->t_strm_Info.rAudStrmInf[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id = 
                    pt_mp4_handler->t_strm_Info.t_audio_strm_ID[ui2_index-1];
                if(pt_minfo_rec->u.t_stm_attribute.u.t_mp4i_audio_stm_attr.e_enc
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
            case MINFO_INFO_TYPE_VIDEO_ENCRYPTION_INFO:
                if((ui2_index > pt_mp4_handler->t_strm_Info.ui1_video_strm_num)
                  ||(ui2_index == 0)
                  ||(ui2_index > MAX_NS_MP4_VID))
                {
                    DBG_ERROR(("ERROR: mp4_minfo_get_info index > video stream encrytion num \r\n"));
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->e_info_type = e_info_type;                
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                x_memcpy(&(pt_minfo_rec->u.t_encrytion_info.u.t_minfo_mp4_encry_info.u.t_video_encrytion_info),
                         &(pt_mp4_handler->t_strm_Info.at_marlin_track_info[0]),
                         sizeof(MINFO_MP4_TRAK_ENCRYTION_INFO_T));   
                pt_minfo_rec->u.t_encrytion_info.u.t_minfo_mp4_encry_info.ui4_strm_id
                        = pt_mp4_handler->t_strm_Info.t_video_strm_ID[ui2_index-1];
                break;

            case MINFO_INFO_TYPE_AUDIO_ENCRYPTION_INFO:  
                if((ui2_index > pt_mp4_handler->t_strm_Info.ui1_audio_strm_num)
                  ||(ui2_index == 0)
                  ||(ui2_index > MAX_NS_MP4_AUD))
                {
                    DBG_ERROR(("ERROR: mp4_minfo_get_info index > audio stream encrytion num \r\n"));
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->e_info_type = e_info_type;                
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                x_memcpy(&(pt_minfo_rec->u.t_encrytion_info.u.t_minfo_mp4_encry_info.u.t_audio_encrytion_info),
                         &(pt_mp4_handler->t_strm_Info.at_marlin_track_info[1+ui2_index-1]),
                         sizeof(MINFO_MP4_TRAK_ENCRYTION_INFO_T));    
                pt_minfo_rec->u.t_encrytion_info.u.t_minfo_mp4_encry_info.ui4_strm_id
                        = pt_mp4_handler->t_strm_Info.t_audio_strm_ID[ui2_index-1];            
                break;
            case MINFO_INFO_TYPE_IDX_INFO:
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
                pt_minfo_rec->u.t_idx_info.ui8_idx_offset   = pt_mp4_handler->ui8_moov_offset;
                pt_minfo_rec->u.t_idx_info.ui8_idx_sz       = pt_mp4_handler->ui4_moov_size;
                pt_minfo_rec->u.t_idx_info.ui8_data_offset  = pt_mp4_handler->ui8_data_offset;
                pt_minfo_rec->u.t_idx_info.ui8_data_sz      = pt_mp4_handler->ui8_data_sz;
                pt_minfo_rec->u.t_idx_info.t_mp4_extra_info.pt_mp4_cmov_info = (VOID*)&(pt_mp4_handler->t_mp4_box_cmov);
                pt_minfo_rec->u.t_idx_info.t_mp4_extra_info.pt_piff_inet_info= (VOID*)&(pt_mp4_handler->t_piff_inet_info);
                pt_minfo_rec->u.t_idx_info.t_mp4_extra_info.pt_mp4_multi_file_info = (VOID*)&(pt_mp4_handler->t_mp4_multi_file_info);
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_DRM_INFO:
                pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
                break;
            case MINFO_INFO_TYPE_META_DATA:
                COPY_DATA(  pt_mp4_handler->t_meta_data_info.pwc_str_title, 
                            pt_mp4_handler->t_meta_data_info.z_title_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_title, 
                            &(pt_minfo_rec->u.t_meta_data.z_title_len));
                COPY_DATA(  pt_mp4_handler->t_meta_data_info.pwc_str_director, 
                            pt_mp4_handler->t_meta_data_info.z_director_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_director, 
                            &(pt_minfo_rec->u.t_meta_data.z_director_len));
                COPY_DATA(  pt_mp4_handler->t_meta_data_info.pwc_str_copyright, 
                            pt_mp4_handler->t_meta_data_info.z_copyright_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_copyright, 
                            &(pt_minfo_rec->u.t_meta_data.z_copyright_len));
                COPY_DATA(  pt_mp4_handler->t_meta_data_info.pwc_str_genre, 
                            pt_mp4_handler->t_meta_data_info.z_genre_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_genre, 
                            &(pt_minfo_rec->u.t_meta_data.z_genre_len));
                pt_minfo_rec->u.t_meta_data.t_date
                    = pt_mp4_handler->t_meta_data_info.t_date;
                pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_rate
                    = pt_mp4_handler->t_meta_data_info.t_frame_rate.ui4_rate;
                pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_scale
                    = pt_mp4_handler->t_meta_data_info.t_frame_rate.ui4_scale;
                pt_minfo_rec->u.t_meta_data.ui8_pb_duration
                    = pt_mp4_handler->t_generic_info.ui8_pb_duration;
                if(pt_mp4_handler->ui8_movie_duration != 0)
                {
                    pt_minfo_rec->u.t_meta_data.ui4_bitrate =
                    (UINT32)_mm_div64(pt_mp4_handler->t_generic_info.ui8_media_sz
                                            * pt_mp4_handler->ui4_movie_timescale, 
                                pt_mp4_handler->ui8_movie_duration,
                                NULL);
                }
                else
                {
                    pt_minfo_rec->u.t_meta_data.ui4_bitrate = 0;                
                }
                
                /*video es info*/
                pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_width = 
                    pt_mp4_handler->t_strm_Info.rVideStrmInf[0].i4_width;
                pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_height = 
                    pt_mp4_handler->t_strm_Info.rVideStrmInf[0].i4_height;
                
                break;
            case MINFO_INFO_TYPE_SP_ATTR:
                if((ui2_index > pt_mp4_handler->t_strm_Info.ui1_sbtl_strm_num)
                  ||(ui2_index == 0)
                  ||(ui2_index > MAX_NS_MP4_SBTL))
                {
                    DBG_ERROR(("ERROR: mp4_minfo_get_info index > sbtl stream num \r\n"));
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
                pt_minfo_rec->u.t_stm_attribute.u.t_mp4_sp_stm_attr = 
                    pt_mp4_handler->t_strm_Info.rSbtlStrmInf[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id = 
                    pt_mp4_handler->t_strm_Info.t_sbtl_strm_ID[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            default:
                return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
} 
