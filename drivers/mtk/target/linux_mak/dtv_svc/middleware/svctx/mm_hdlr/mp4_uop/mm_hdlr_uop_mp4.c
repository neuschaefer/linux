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
 * $RCSfile: mm_hdlr_uop_mp4.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
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
#include "mutil/minfo/x_minfo.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "svctx/mm_hdlr/mp4_uop/mm_hdlr_uop_mp4.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define TWO_MIN_RANGE_DUR 2*10*90000
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _mp4_uop_strema_id_2_ele_tbl_desc(
                             UINT32 ui4_strm_id ,                            /*I*/ 
                             MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_ele_alc_info,/*I*/
                             MIDXBULD_ELMT_TBL_DESCRIB_T**   ppt_ele_tbl_desc)/*O*/
{

    MIDXBULD_ELMT_TBL_DESCRIB_T* p_head_tbl_desc;
    
    if ((NULL == pt_ele_alc_info) || (NULL == ppt_ele_tbl_desc))
    {
        return SVCTXR_INV_ARG;
    }
    
    p_head_tbl_desc = pt_ele_alc_info->pt_tbl_describ;
    *ppt_ele_tbl_desc = NULL;
    while(p_head_tbl_desc!= NULL)
    {
        /*if find out break*/
        if(p_head_tbl_desc->u_info.t_es.ui4_strm_id == ui4_strm_id)
        {
            *ppt_ele_tbl_desc = p_head_tbl_desc;          
            break;
        }
        else
        {
            p_head_tbl_desc = p_head_tbl_desc->pt_next;
        }
    }
    
    if(NULL == *ppt_ele_tbl_desc)
    {
        return SVCTXR_NOT_EXIST;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_hdlr_open
 *
 * Description:This function prepare data for later use when open mp4 uop handler
 *
 * Inputs:  pv_obj       specify the media description object  
 *
 * Outputs: NULL 
 *
 * Returns: SVCTXR_OK    Success. The routine was successful and the 
 *                       operation has been completed.
 *          NON-Zero     If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_hdlr_open (MEDIA_DESCRIBT_T* pt_obj)
{
    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_hdlr_is_hdlr
 *
 * Description:This function is to check if support the input format
 *
 * Inputs:  pv_obj,          specify the media description object  
 *          pt_media_format, specify the media format
 * Outputs: NULL 
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          SVCTXR_FAIL , If do not support the format
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_hdlr_is_hdlr(MEDIA_DESCRIBT_T* pt_obj,
                               MEDIA_FORMAT_T*    pt_media_format)
{
    /*check paprameter*/
   if((NULL == pt_obj) || ( NULL == pt_media_format))
   {
     return SVCTXR_INV_ARG;
   }
   
   if((pt_media_format->e_media_type == MEDIA_TYPE_CONTAINER)
       && (pt_media_format->t_media_subtype.u.e_contnr_subtype 
           == MEDIA_CONTNR_SUBTYPE_MP4))
   {
      return SVCTXR_OK;
   }
   else
   {
      return SVCTXR_FAIL;
   }
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_hdlr_close
 *
 * Description:This function free used data  when close mp4 uop handler
 *
 * Inputs:  pv_obj       specify the media description object  
 *
 * Outputs: NULL 
 *
 * Returns: SVCTXR_OK    Success. The routine was successful and the 
 *                       operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_hdlr_close (MEDIA_DESCRIBT_T* pt_obj)
{
    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_hdlr_get_tm_code
 *
 * Description:This function get the time code information
 *
 * Inputs:  pv_obj,          specify the media description object  
 *          e_timecode_type, specify the time code type
 *
 * Outputs: pt_time_info ,output time code information
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_hdlr_get_tm_code (
                    MEDIA_DESCRIBT_T*         pt_obj,
                    MM_SVC_TIME_CODE_INFO_T*  pt_time_info)
{
    MM_SVC_TM_CODE_TYPE_T   e_tm_code_type;  
   /*check paprameter*/
    if ((NULL == pt_obj) || (NULL == pt_time_info))
    {
        return SVCTXR_INV_ARG;
    }
    e_tm_code_type = pt_time_info->t_tm_code_type;
    switch(e_tm_code_type)
    {
    case MM_SVC_TM_CODE_TYPE_TITLE:
        return SVCTXR_NOT_SUPPORT;
    case MM_SVC_TM_CODE_TYPE_CHAPTER:
        return SVCTXR_NOT_SUPPORT;
    case MM_SVC_TM_CODE_TYPE_ALL:
            pt_time_info->t_tm_code_type = e_tm_code_type;
            pt_time_info->ui2_idx = 0;
        pt_time_info->ui8_time = 0;
        break;
     default:
      break;
  }
  return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_hdlr_get_dur
 *
 * Description:This function get the time duration
 *
 * Inputs:  pv_obj,          specify the media description object  
 *
 * Outputs: pt_time_dur ,   output time duration information
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_hdlr_get_dur(
                    MEDIA_DESCRIBT_T*       pt_obj, 
                    MM_SVC_DUR_INFO_T*      pt_time_dur)
{
    HANDLE_T h_minfo;
    UINT16 ui4_rec_nums;
    SIZE_T ui4_rec_sz;
    MM_SVC_DUR_TYPE_T    e_dur_type; 
    MINFO_INFO_T         t_media_info;  
    INT32 i4_ret;

    /*check paprameter*/
    if(NULL == pt_obj || NULL ==pt_time_dur)
    {
        return SVCTXR_INV_ARG;
    }
    x_memset(&t_media_info, 0, sizeof (MINFO_INFO_T));
    e_dur_type = pt_time_dur->t_dur_type;
    h_minfo =  pt_obj->h_media_info;
    switch(e_dur_type)
    {
    case MM_SVC_DUR_TYPE_TITLE:
        return SVCTXR_NOT_SUPPORT;
    case MM_SVC_DUR_TYPE_CHAPTER:
        return SVCTXR_NOT_SUPPORT;
    case MM_SVC_DUR_TYPE_ALL:
        x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_GENERIC , &ui4_rec_nums);
        if(ui4_rec_nums == 0 ||ui4_rec_nums > 1 )
        {
            return SVCTXR_NOT_SUPPORT;
        }
        ui4_rec_sz = sizeof (MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_GENERIC, 1, &t_media_info, &ui4_rec_sz);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        pt_time_dur->ui8_duration = t_media_info.u.t_general_info.ui8_pb_duration;
        break;         
    case MM_SVC_DUR_TYPE_BUFFERED:
        break;
    default:
        break;
    }
    return SVCTXR_OK;
 }


/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_handler_search_fct
 *
 * Description:This function serach position
 *
 * Inputs:  pv_obj,       specify the media description object  
 *          e_srh_type,   specify the position type
 *
 * Outputs: pt_time_dur , output position time
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_handler_search_fct (
                    MEDIA_DESCRIBT_T*   pt_obj, 
                    MM_SVC_POS_TYPE_T   e_srh_type, 
                    MM_SVC_POS_INFO_T*  pt_trick_info)
{
   return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_handler_btn_fct
 *
 * Description:This function do tricking
 *
 * Inputs:  pv_obj,       specify the media description object  
 *          pt_trick_info , output trick time
 *
 * Outputs: NULL
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_handler_btn_fct (
                     MEDIA_DESCRIBT_T*       pt_obj,                          
                     MM_SVC_NAV_INFO_T*      pt_btn_info)
{   
    /*check paprameter*/
    if(pt_obj == NULL ||pt_btn_info ==NULL)
    {
         return SVCTXR_INV_ARG;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_handler_trick_fct
 *
 * Description:This function do tricking
 *
 * Inputs:  pv_obj,       specify the media description object  
 *          pt_trick_info , output trick time
 *
 * Outputs: NULL
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_handler_trick_fct (
                    MEDIA_DESCRIBT_T*       pt_obj, 
                    MM_SVC_TRICK_INFO_T*    pt_trick_info)
{
   return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_uop_handler_get_range_fct
 *
 * Description: check the content in MIDXBULD_ELMT_TBL_ALLOC_INFO_T to 
 *              determine which stream(i.e stream_id)should be caculated 
 *              and the memory of pt_range_info is allocated by client.
 *
 * Inputs:  pv_obj,         specify the media description object  
 *          ui4_start_pts , start pts 
 *
 * Outputs: pt_range_info, output range info
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mp4_uop_handler_get_range_fct(
              MEDIA_DESCRIBT_T*                pt_obj ,
              UINT32                           ui4_start_pts, 
              MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info)
{
    INT32    i4_ret;
    UINT16   ui2_i = 0;
    HANDLE_T h_minfo;
    UINT16   ui2_rec_nums = 0;
    BOOL     b_is_full_range;       
    SIZE_T   ui4_rec_sz = sizeof(MINFO_INFO_T);
    UINT32   ui4_total_dur;
    MINFO_INFO_T   t_genric_info;
    MINFO_INFO_T   t_vid_stm_info;
    MINFO_INFO_T   t_aud_stm_info;
    MINFO_INFO_T   t_sp_stm_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_tbl_desc;
    
    x_memset(&t_genric_info, 0, ui4_rec_sz);
    x_memset(&t_vid_stm_info, 0, ui4_rec_sz);
    x_memset(&t_aud_stm_info, 0, ui4_rec_sz);
    h_minfo = pt_obj->h_media_info;
    /*caculate how much size needed to store 1-minute indxex tbl for video stream*/
    /*get media total duration*/
    x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_GENERIC , &ui2_rec_nums);
    if(ui2_rec_nums == 0 ||ui2_rec_nums > 1 )
    {
       return SVCTXR_NOT_SUPPORT;
    }
    i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_GENERIC, 1, 
                              &t_genric_info, &ui4_rec_sz);
    if(i4_ret != MINFOR_OK)
    {
       return SVCTXR_NOT_SUPPORT;
    }

    /*ui2_start_pts might be transfer to seconds*90000*/
    ui4_total_dur = (UINT32)(t_genric_info.u.t_general_info.ui8_pb_duration - (ui4_start_pts));
    
    /*get Key table size at first....
    ui2_key_tbl_sz = ui2_key_tbl_sz;*/      

    /*get video stream indx tble sz in 1 minutes*/
    x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR , &ui2_rec_nums);   
    if(ui2_rec_nums != 0)
    {       
        i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR, 1,
                                  &t_vid_stm_info,&ui4_rec_sz);
        i4_ret = _mp4_uop_strema_id_2_ele_tbl_desc(
                        t_vid_stm_info.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_vid_stm_info.u.t_stm_attribute.u.t_mp4i_video_stm_attr.ui4_scale;
            pt_tbl_desc->u_info.t_es.ui4_rate = 
                t_vid_stm_info.u.t_stm_attribute.u.t_mp4i_video_stm_attr.ui4_rate;
            pt_tbl_desc->u_info.t_es.ui4_sample_sz = 0;  
            pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 0;/*QQQQQQQQQQQQQQ*/
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
            pt_tbl_desc->u_info.t_es.b_is_cur_strm_codec_spt = 
                t_vid_stm_info.u.t_stm_attribute.b_is_codec_support;
        }
    }    


    /*get audio stream indx tble sz in 1 minutes*/
    x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR , &ui2_rec_nums);   
    for(ui2_i =1 ; ui2_i <= ui2_rec_nums ; ui2_i++)
    {
        ui4_rec_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, 
                                    MINFO_INFO_TYPE_AUDIO_ATTR, 
                                    (MINFO_REC_IDX_T)ui2_i,
                                    &t_aud_stm_info, 
                                    &ui4_rec_sz);
        i4_ret = _mp4_uop_strema_id_2_ele_tbl_desc(
                        t_aud_stm_info.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_aud_stm_info.u.t_stm_attribute.u.t_mp4i_audio_stm_attr.ui4_timescale;
            pt_tbl_desc->u_info.t_es.ui4_rate = 
                (UINT32)t_aud_stm_info.u.t_stm_attribute.u
                                            .t_mp4i_audio_stm_attr.i4_sample_rate;   
            pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 0;
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
            pt_tbl_desc->u_info.t_es.b_is_cur_strm_codec_spt = 
                t_aud_stm_info.u.t_stm_attribute.b_is_codec_support;
            pt_tbl_desc->u_info.t_es.b_is_pcm_aud
                = IS_AUDIO_PCM_ENC(t_aud_stm_info.u.t_stm_attribute.u.t_mp4i_audio_stm_attr.e_enc);
            if (pt_tbl_desc->u_info.t_es.b_is_pcm_aud)
            {
                pt_tbl_desc->u_info.t_es.ui4_sample_sz = t_aud_stm_info.u.t_stm_attribute.u.t_mp4i_audio_stm_attr.i4_sample_size;
            }
            else  
            {
                pt_tbl_desc->u_info.t_es.ui4_sample_sz = 0;  
            }
        }
    }
     
    /*get sp stream indx tble sz in 1 minutes*/
    x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_SP_ATTR , &ui2_rec_nums);   
    for(ui2_i =1 ; ui2_i <= ui2_rec_nums ; ui2_i++)
    {
        ui4_rec_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, 
                                    MINFO_INFO_TYPE_SP_ATTR, 
                                    (MINFO_REC_IDX_T)ui2_i,
                                    &t_sp_stm_info, 
                                    &ui4_rec_sz);
        i4_ret = _mp4_uop_strema_id_2_ele_tbl_desc(
                        t_sp_stm_info.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_sp_stm_info.u.t_stm_attribute.u.t_mp4_sp_stm_attr.ui4_timescale;             
            pt_tbl_desc->u_info.t_es.ui4_sample_sz = 0;  
            pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 0;
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_SUBTITLE;
            pt_tbl_desc->u_info.t_es.b_is_cur_strm_codec_spt = 
                t_sp_stm_info.u.t_stm_attribute.b_is_codec_support;
        }
    }   
    /*TBD*/
    /*add full or partial range judgement, the MM_IDXTBL_POOL_MAX_SZ should be consider*/
    /* TWO_MIN_RANGE_DUR is predfinde here , but this value should caculated through */
    /*size of 1 video entry + 1 entry of all audio entry and current remained free size in idx */
    /*builder. Here TWO_MIN_RANGE_DUR secs is assumed*/
#if 1
    /*if(ui2_1minut_tbl_sz*ui4_total_dur <  MM_IDXTBL_POOL_MAX_SZ - ui2_key_tbl_sz)*/
    if(TWO_MIN_RANGE_DUR > ui4_total_dur)
    {
        /*set full range to media end*/ 
        b_is_full_range = TRUE;
           
    }
    else
    {
       /*set partail range 1/4*/
       b_is_full_range = FALSE;
    }
#endif

      /* added for DLNA test*/                                                   
    if((pt_obj->e_src_type == SRC_TYPE_MEDIA_NETWORK)
        && (pt_obj->e_svc_protocol == PROTOCL_MODE_DLNA))
    {
        pt_range_info->u_range.t_es.ui4_start_pts = 0;
        pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
        pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    }
    else if(b_is_full_range)
    {
         /*PTS for 1sec = 90K*1 */
        /*QQQQQQ assume full range for pre testing*/
        pt_range_info->u_range.t_es.ui4_start_pts = ui4_start_pts;
        pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
        pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    }
    else
    {
        /*NOTE : currently ,assume index builder could afford to build TWO_MIN_RANGE_DUR range */
        pt_range_info->u_range.t_es.ui4_start_pts = ui4_start_pts  ;
        pt_range_info->u_range.t_es.ui4_end_pts = ui4_start_pts + TWO_MIN_RANGE_DUR;
        pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    }
     
    return SVCTXR_OK;
}


