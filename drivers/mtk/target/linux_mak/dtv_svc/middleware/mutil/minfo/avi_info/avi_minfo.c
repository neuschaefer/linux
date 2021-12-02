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
 * $RCSfile: avi_minfo.c,v $
 * $Revision: #2 $ 1.0
 * $Date: 2013/01/04 $ 2004/11/26
 * $Author: pan.huang $
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
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include"avi_parser.h"
#include "mutil/mm_util.h"
#include "drm/divx_drm/x_divx_drm.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include"avi_parser.h"
#include "x_util.h"
#include "../../mm_util.h"
#include "../../../drm/divx_drm/x_divx_drm.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define VID_STRM_ID_0        ((UINT32)0x63640000)
#define AUD_STRM_ID_0        ((UINT32)0x62770000)
#define SP_STRM_ID_0         ((UINT32)0x62730000)
const static CHAR AVI_3D_IDENTIFER[] = "FUJIFILM AVI STREAM ";
const static CHAR AVI_3D_VERSION[]   = "0200";


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
 * Name:  avi_minfo_is_handler
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
INT32 avi_minfo_is_handler(VOID*    pv_obj, MEDIA_FORMAT_T* pt_format)
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL || 
    pt_format == NULL)
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/    
    if(((pt_format->e_media_type == MEDIA_TYPE_CONTAINER) && 
       (pt_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)) ||
      ((pt_format->e_media_type == MEDIA_TYPE_AUDIO_ES) && 
       (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE)))
    {   /*add for wave support*/
        return MINFOR_OK;
    }
    else
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
}
/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_fopen
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
INT32 avi_minfo_fopen(
                        CHAR*              ps_path_name, 
                        VOID*              pv_obj)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_AVI_RIFF_INF_T*   pt_avi_riff;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc(sizeof(MINFO_AVI_RIFF_INF_T));
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_AVI_RIFF_INF_T));
    pt_avi_riff = (MINFO_AVI_RIFF_INF_T*)(pt_minfo_obj->pv_handler_obj);
    /* Willy : temp use frame buffer dram to store parse data. */
    pt_avi_riff->pui1_header_buf = 
        _mm_util_mem_alloc(sizeof(UINT8) * AVI_HEADER_BUF_LENGTH);
    pt_avi_riff->ui4_buf_length = AVI_HEADER_BUF_LENGTH;
    if (pt_avi_riff->pui1_header_buf  == NULL)
    {
         return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_riff->pui1_header_buf, 
             0, 
             sizeof(UINT8) * AVI_HEADER_BUF_LENGTH);

    
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_mopen
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
INT32 avi_minfo_mopen(
                        VOID*              pv_mem_data, 
                        SIZE_T             z_mem_leng, 
                        VOID*              pv_obj)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_AVI_RIFF_INF_T*   pt_avi_riff;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pv_mem_data == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc(sizeof(MINFO_AVI_RIFF_INF_T));
    if (pt_minfo_obj->pv_handler_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_AVI_RIFF_INF_T));
    pt_avi_riff = (MINFO_AVI_RIFF_INF_T*)(pt_minfo_obj->pv_handler_obj);
    /* Willy : temp use frame buffer dram to store parse data. */
    pt_avi_riff->pui1_header_buf = 
        _mm_util_mem_alloc(sizeof(UINT8) * AVI_HEADER_BUF_LENGTH);
    pt_avi_riff->ui4_buf_length = AVI_HEADER_BUF_LENGTH;
    if (pt_avi_riff->pui1_header_buf  == NULL)
    {
         return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_avi_riff->pui1_header_buf, 
             0, 
             sizeof(UINT8) * AVI_HEADER_BUF_LENGTH);    
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_close
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
INT32 avi_minfo_close( 
                        VOID*              pv_obj)
{
    UINT8                   ui1_tmp;
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_AVI_RIFF_INF_T*   pt_avi_riff_info;
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
    pt_avi_riff_info = ((MINFO_AVI_RIFF_INF_T*)(pt_minfo_obj->pv_handler_obj));
    
    if(pt_avi_riff_info->pui1_header_buf != NULL)
    {
        _mm_util_mem_free(pt_avi_riff_info->pui1_header_buf);
        pt_avi_riff_info->pui1_header_buf  = NULL;
    }

    for(ui1_tmp = 0; ui1_tmp < pt_avi_riff_info->t_strm_inf.ui1_video_strm_num; ui1_tmp++)
    {
        if (ui1_tmp >= MAX_NS_AVI_VID)
        {
            break;
        }
        if(pt_avi_riff_info->t_strm_inf.t_vid_strm_inf[ui1_tmp].pui1_codec_info)
        {
            _mm_util_mem_free(pt_avi_riff_info->t_strm_inf.t_vid_strm_inf[ui1_tmp].pui1_codec_info);
            pt_avi_riff_info->t_strm_inf.t_vid_strm_inf[ui1_tmp].pui1_codec_info = NULL;
        }
    }
    
    for(ui1_tmp = 0; ui1_tmp < pt_avi_riff_info->t_strm_inf.ui1_audio_strm_num; ui1_tmp++)
    {
        if (ui1_tmp >= MAX_NS_AVI_AUD)
        {
            break;
        }
        if(pt_avi_riff_info->t_strm_inf.t_aud_strm_inf[ui1_tmp].pui1_codec_info)
        {
            _mm_util_mem_free(pt_avi_riff_info->t_strm_inf.t_aud_strm_inf[ui1_tmp].pui1_codec_info);
            pt_avi_riff_info->t_strm_inf.t_aud_strm_inf[ui1_tmp].pui1_codec_info = NULL;
        }
    }

    pt_minfo_obj->t_minfo_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
    if (pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk != NULL)
    {
        _mm_util_mem_free(pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk);
        pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk = NULL;
    }

    /* free meta data memory */    
    if (pt_avi_riff_info->t_meta_data_info.pwc_str_title != NULL)
    {
        _mm_util_mem_free(pt_avi_riff_info->t_meta_data_info.pwc_str_title);
        pt_avi_riff_info->t_meta_data_info.pwc_str_title = NULL;
    }
    if (pt_avi_riff_info->t_meta_data_info.pwc_str_director != NULL)
    {
        _mm_util_mem_free(pt_avi_riff_info->t_meta_data_info.pwc_str_director);
        pt_avi_riff_info->t_meta_data_info.pwc_str_director = NULL;
    }
    if (pt_avi_riff_info->t_meta_data_info.pwc_str_copyright != NULL)
    {
        _mm_util_mem_free(pt_avi_riff_info->t_meta_data_info.pwc_str_copyright);
        pt_avi_riff_info->t_meta_data_info.pwc_str_copyright = NULL;
    }
    if (pt_avi_riff_info->t_meta_data_info.pwc_str_genre != NULL)
    {
        _mm_util_mem_free(pt_avi_riff_info->t_meta_data_info.pwc_str_genre);
        pt_avi_riff_info->t_meta_data_info.pwc_str_genre = NULL;
    }

    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }
    if (pt_minfo_obj->pui1_drm_info != NULL)
    {
        x_mem_free(pt_minfo_obj->pui1_drm_info);
        pt_minfo_obj->pui1_drm_info = NULL;
    }
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_parse
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
INT32 avi_minfo_parse( 
                        VOID*              pv_obj)
{
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
    /* parse ID3 */
    return i4_avi_minfo_chunk_parse((MINFO_OBJ_T*)pv_obj);

} 

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_get_num_recs
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
INT32 avi_minfo_get_num_recs(
                        VOID*                pv_obj, 
                        MINFO_INFO_TYPE_T    e_info_type, 
                        UINT16*              pui2_num_recs)
{
     MINFO_OBJ_T*        pt_minfo_obj;
     MINFO_AVI_RIFF_INF_T* pt_avi_handler;
    
     /************************************************************************/
     /* Check parameter                                                      */
     /************************************************************************/
     if ((pv_obj == NULL)
         || (pui2_num_recs == NULL))
     {
         return MINFOR_INITIAL_ERR;
     }
     /************************************************************************/
     /* do                                                                   */
     /************************************************************************/
     pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
     pt_avi_handler = pt_minfo_obj->pv_handler_obj;
    
     if (pt_avi_handler)
     {
         if (pt_minfo_obj->b_parsing_successful == FALSE)
         {
             return MINFOR_INFO_NOT_READY;    
         }
         
         switch(e_info_type)
         {
             case MINFO_INFO_TYPE_GENERIC:
                 *pui2_num_recs = (UINT16)1;
                 break;                
             case MINFO_INFO_TYPE_VIDEO_ATTR:
                 /*QQQQQ*/
                 *pui2_num_recs = (UINT16)pt_avi_handler->t_strm_inf.ui1_video_strm_num;;
                 break;
             case MINFO_INFO_TYPE_AUDIO_ATTR:
                 *pui2_num_recs = (UINT16)pt_avi_handler->t_strm_inf.ui1_audio_strm_num;
                 break;
             case MINFO_INFO_TYPE_SP_ATTR:
                 *pui2_num_recs = (UINT16)pt_avi_handler->t_strm_inf.ui1_sp_strm_num;
                 break;
             case MINFO_INFO_TYPE_IDX_INFO:
                 *pui2_num_recs = (UINT16)1;
                 break;
             default:
              return MINFOR_INFO_NOT_FOUND;
         }
         return MINFOR_OK;
     }
     return MINFOR_FMT_NOT_SUPPORT;
}
                        

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_get_info
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
INT32 avi_minfo_get_info( 
                        VOID*                pv_obj, 
                        MINFO_INFO_TYPE_T    e_info_type, 
                        MINFO_REC_IDX_T      ui2_index,                    
                        VOID*                pv_buf, 
                        SIZE_T*              pz_buf_leng)
{
    MINFO_OBJ_T*            pt_minfo_obj;
    MINFO_AVI_RIFF_INF_T*   pt_avi_handler;
    MINFO_INFO_T*           pt_minfo_rec;
    INT32                   i4_width, i4_height;
    UINT8                   ui1_strm_id;   
    UINT64                  ui8_mov_sz_bit;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (pz_buf_leng == NULL))
    {
        return MINFOR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_avi_handler = pt_minfo_obj->pv_handler_obj;
    if(pv_buf == NULL)
    {
        /*get length only*/
        *pz_buf_leng = (SIZE_T)sizeof (MINFO_INFO_T);
        return MINFOR_OK;
    }
    else
    {
        /* get data */
        if ((SIZE_T)*pz_buf_leng < sizeof (MINFO_INFO_T))
        {
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }
    
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    /*x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));*/

    if (pt_avi_handler != NULL)
    {
        if (pt_minfo_obj->b_parsing_successful == FALSE)
        {
            return MINFOR_INFO_NOT_READY;    
        }
                
        pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
        pt_minfo_rec->e_info_type = e_info_type;
        pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
        switch(e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                if(ui2_index > 1)
                {
                   return MINFOR_OUT_OF_RECORD;
                }
                /* get the first video stream W/H */
                if(pt_avi_handler->t_strm_inf.ui1_video_strm_num == 0)
                {
                    pt_minfo_rec->u.t_general_info.b_hd_content = FALSE;
                }
                else
                {
                i4_width = pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].i4_width;
                i4_height = pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].i4_height;
                if(((i4_width == 1280) && (i4_height == 720)) ||
                   ((i4_width == 1920) && (i4_height == 1080)))
                {
                    pt_minfo_rec->u.t_general_info.b_hd_content = TRUE;
                }
                else
                {
                    pt_minfo_rec->u.t_general_info.b_hd_content = FALSE;
                }
                }
                pt_minfo_rec->u.t_general_info.ui8_pb_duration  = 
                                            pt_avi_handler->ui8_duration; 
                pt_minfo_rec->u.t_general_info.ui8_media_sz     =
                                            pt_avi_handler->ui8_riff_size;
                pt_minfo_rec->u.t_general_info.ui1_strm_ns      = 
                                            pt_avi_handler->ui1_cur_strm_num; 
                pt_minfo_rec->u.t_general_info.b_non_seekable   = 
                                            pt_avi_handler->b_non_seekable;
                pt_minfo_rec->u.t_general_info.b_is_drm_exist   =
                    (pt_minfo_obj->t_minfo_drm_info.t_minfo_drm_type != DRM_TYPE_NONE);
                /*for 3D */
                if((pt_avi_handler->t_strm_inf.ui1_video_strm_num == 2) && 
                   (x_strcmp(pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ac_3d_identifier, AVI_3D_IDENTIFER) == 0)&&
                   (x_strcmp(pt_avi_handler->t_strm_inf.t_vid_strm_inf[1].ac_3d_identifier, AVI_3D_IDENTIFER) == 0) &&
                   (x_strcmp(pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ac_3d_version,    AVI_3D_VERSION)   == 0)&&
                   (x_strcmp(pt_avi_handler->t_strm_inf.t_vid_strm_inf[1].ac_3d_version,    AVI_3D_VERSION)   == 0))
                {
                    pt_minfo_rec->u.t_general_info.b_stereo_scopic = TRUE;
                    pt_minfo_rec->u.t_general_info.e_scopic_type   = MM_SCOPIC_DS;
                }
                else
                {
                    pt_minfo_rec->u.t_general_info.b_stereo_scopic = FALSE;
                    pt_minfo_rec->u.t_general_info.e_scopic_type   = MM_SCOPIC_UNKNOWN;
                }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                /* Bad interleaved Flag */
                pt_minfo_rec->u.t_general_info.b_is_bad_interleaved = pt_avi_handler->b_is_bad_interleaved;
#endif               
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break; 

            case MINFO_INFO_TYPE_META_DATA:
                COPY_DATA(  pt_avi_handler->t_meta_data_info.pwc_str_title, 
                            pt_avi_handler->t_meta_data_info.z_title_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_title, 
                            &(pt_minfo_rec->u.t_meta_data.z_title_len));
                COPY_DATA(  pt_avi_handler->t_meta_data_info.pwc_str_director, 
                            pt_avi_handler->t_meta_data_info.z_director_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_director, 
                            &(pt_minfo_rec->u.t_meta_data.z_director_len));
                COPY_DATA(  pt_avi_handler->t_meta_data_info.pwc_str_copyright, 
                            pt_avi_handler->t_meta_data_info.z_copyright_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_copyright, 
                            &(pt_minfo_rec->u.t_meta_data.z_copyright_len));
                COPY_DATA(  pt_avi_handler->t_meta_data_info.pwc_str_genre, 
                            pt_avi_handler->t_meta_data_info.z_genre_len, 
                            pt_minfo_rec->u.t_meta_data.pwc_str_genre, 
                            &(pt_minfo_rec->u.t_meta_data.z_genre_len));                
                
                pt_minfo_rec->u.t_meta_data.ui8_pb_duration = 
                                            pt_avi_handler->ui8_duration; 
                if(pt_avi_handler->t_strm_inf.ui1_video_strm_num == 0)
                {
                    pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_rate = 1;
                    pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_scale = 1;
                }
                else
                {
                    pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_rate = 
                        pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ui4_rate;
                    pt_minfo_rec->u.t_meta_data.t_frame_rate.ui4_scale = 
                        pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ui4_scale;
                }
                ui8_mov_sz_bit = (UINT64)pt_avi_handler->ui8_mov_sz * 8;
                pt_minfo_rec->u.t_meta_data.ui4_bitrate = (UINT32)_mm_div64(
                            ui8_mov_sz_bit * MIDXBULD_SYSTEM_CLOCK_FREQUENCY,
                            pt_avi_handler->ui8_duration, 
                            NULL);

                /*video es info*/                
                pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_width = 
                    pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].i4_width;
                pt_minfo_rec->u.t_meta_data.t_video_es_info.i4_video_inf_height = 
                    pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].i4_height;

                break;
                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                if ((ui2_index > pt_avi_handler->t_strm_inf.ui1_video_strm_num) || 
                    (ui2_index == 0) || 
                    (ui2_index > MAX_NS_AVI_VID))
                {
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->u.t_stm_attribute.u.t_avi_video_stm_attr = 
                             pt_avi_handler->t_strm_inf.t_vid_strm_inf[ui2_index-1];
                ui1_strm_id = pt_avi_handler->t_strm_inf.ui1_vid_strm_id_list[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_avi_stm_id = 
                                            pt_avi_handler->aui4_video_strm_id[ui2_index-1];
                if(pt_minfo_rec->u.t_stm_attribute.u.t_avi_video_stm_attr.e_enc
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
                if ((ui2_index > pt_avi_handler->t_strm_inf.ui1_audio_strm_num) || 
                    (ui2_index == 0) || 
                    (ui2_index > MAX_NS_AVI_AUD))
                {
                    return MINFOR_OUT_OF_RECORD;
                }
                pt_minfo_rec->u.t_stm_attribute.u.t_avi_audio_stm_attr = 
                            pt_avi_handler->t_strm_inf.t_aud_strm_inf[ui2_index-1];
                ui1_strm_id = pt_avi_handler->t_strm_inf.ui1_aud_strm_id_list[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_avi_stm_id = 
                                AUD_STRM_ID_0 | 
                                (((ui1_strm_id % 10) + 0x30) << 8) | 
                                (((ui1_strm_id % 100) / 10) + 0x30);          
                if(pt_minfo_rec->u.t_stm_attribute.u.t_avi_audio_stm_attr.e_enc
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
            case MINFO_INFO_TYPE_SP_ATTR:
                if ((ui2_index > pt_avi_handler->t_strm_inf.ui1_sp_strm_num) || 
                    (ui2_index == 0) || 
                    (ui2_index > MAX_NS_AVI_SP))
                {
                    return MINFOR_OUT_OF_RECORD;
                }                
                pt_minfo_rec->u.t_stm_attribute.u.t_avi_sp_stm_attr = 
                            pt_avi_handler->t_strm_inf.t_sp_strm_inf[ui2_index-1];
                ui1_strm_id = pt_avi_handler->t_strm_inf.ui1_sp_strm_id_list[ui2_index-1];
                pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_avi_stm_id = 
                                SP_STRM_ID_0 | 
                                (((ui1_strm_id % 10) + 0x30) << 8) | 
                                (((ui1_strm_id % 100) / 10) + 0x30);          
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
                
            case MINFO_INFO_TYPE_IDX_INFO:
                pt_minfo_rec->u.t_idx_info.ui4_idx_type = 0;
                pt_minfo_rec->t_format = pt_minfo_obj->t_media_format;
                pt_minfo_rec->e_info_type = e_info_type;
                pt_minfo_rec->ui1_rec_idx = 1;
                pt_minfo_rec->u.t_idx_info.ui8_idx_offset = 
                    pt_avi_handler->ui4_indxtbl_offset;    
                pt_minfo_rec->u.t_idx_info.ui8_idx_sz     = 
                    pt_avi_handler->ui4_indxtbl_sz;
                pt_minfo_rec->u.t_idx_info.ui8_data_offset = 
                    pt_avi_handler->ui4_mov_start_base;    
                pt_minfo_rec->u.t_idx_info.ui8_data_sz     = 
                    pt_avi_handler->ui8_mov_sz;
                if(pt_minfo_rec->u.t_idx_info.ui8_idx_offset > 0 )
                {
                    pt_minfo_rec->u.t_idx_info.ui4_idx_type |= MM_MINFO_IDX_TYPE_COMMON;
                }
                
                if(pt_avi_handler->t_strm_inf.ui1_video_strm_num == 0)
                {
                    if(pt_minfo_rec->u.t_idx_info.ui8_idx_offset == 0)
                    {
                        pt_minfo_rec->u.t_idx_info.ui4_idx_type |= MM_MINFO_IDX_TYPE_NONE;
                    }
                }
                else
                {
                if(pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ui4_indx_offset > 0)
                {
                    pt_minfo_rec->u.t_idx_info.ui4_idx_type |= MM_MINFO_IDX_TYPE_STRM;
                }
                if((pt_minfo_rec->u.t_idx_info.ui8_idx_offset == 0 ) && 
                   (pt_avi_handler->t_strm_inf.t_vid_strm_inf[0].ui4_indx_offset == 0))
                {
                    pt_minfo_rec->u.t_idx_info.ui4_idx_type |= MM_MINFO_IDX_TYPE_NONE;
                }
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_DRM_INFO:
                if (pt_minfo_obj->pui1_drm_info != NULL)
                {
                    x_mem_free(pt_minfo_obj->pui1_drm_info);
                    pt_minfo_obj->pui1_drm_info = NULL;
                }
                pt_minfo_obj->pui1_drm_info = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * pt_minfo_obj->ui4_drm_size);
                if (pt_minfo_obj->pui1_drm_info == NULL)
                {
                    return MINFOR_INTERNAL_ERR;
                }
                
                x_memset(pt_minfo_obj->pui1_drm_info, 0, pt_minfo_obj->ui4_drm_size);
                x_memcpy(pt_minfo_obj->pui1_drm_info, pt_minfo_obj->t_minfo_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk, pt_minfo_obj->ui4_drm_size);

                pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = pt_minfo_obj->t_minfo_drm_info.t_minfo_drm_type;
                pt_minfo_rec->u.t_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk = pt_minfo_obj->pui1_drm_info;
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
            case MINFO_INFO_TYPE_CODEC:
            {
                MINFO_AVI_VIDEO_STM_ATRBT_T  t_avi_strm_attr;
                if ((ui2_index > pt_avi_handler->t_strm_inf.ui1_video_strm_num) || 
                    (ui2_index == 0) || 
                    (ui2_index > MAX_NS_AVI_VID))
                {
                    return MINFOR_OUT_OF_RECORD;
                }

                t_avi_strm_attr = 
                             pt_avi_handler->t_strm_inf.t_vid_strm_inf[ui2_index-1];                
                if ((t_avi_strm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_311) ||
                    (t_avi_strm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_4) ||
                    (t_avi_strm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_5) )
                    /*||
                    (t_avi_strm_attr.e_enc == MINFO_INFO_VID_ENC_XVID))*/
                {
                    pt_minfo_rec->u.t_divx_info.t_minfo_info_codec_enc = MINFO_INFO_CODEC_VID_ENC_DIVX;
                }
                else
                {
                    pt_minfo_rec->u.t_divx_info.t_minfo_info_codec_enc = MINFO_INFO_CODEC_VID_ENC_UNKONWN;
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
            }
                break;
            default:
             return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    return MINFOR_FMT_NOT_SUPPORT;
} 
