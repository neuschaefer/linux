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
 * $RCSfile: mm_hdlr_uop_mkv.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains the implementation of mkv uop handler 
 *         function for service context
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
#include "svctx/mm_hdlr/mkv_uop/mm_hdlr_uop_mkv.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "svctx/svctx_dbg.h"

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
 
#if 1/*MKV DivXPlus*/
static INT32 mkv_uop_hdlr_get_chapter_def_info(MEDIA_DESCRIBT_T* pt_obj);
#endif

static INT32 _mkv_uop_strema_id_2_ele_tbl_desc(
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
 * Name:  mkv_uop_hdlr_open
 *
 * Description:This function prepare data for later use when open mkv uop handler
 *
 * Inputs:  pv_obj       specify the media description object  
 *
 * Outputs: NULL 
 *
 * Returns: SVCTXR_OK    Success. The routine was successful and the 
 *                       operation has been completed.
 *          NON-Zero     If fail
 ----------------------------------------------------------------------------*/
INT32 mkv_uop_hdlr_open (MEDIA_DESCRIBT_T* pt_obj)
{
    MM_HDLR_UOP_MKV_OBJ*  pt_mkv_uop_hdlr_obj;
    INT32                 i4_ret;
    MINFO_INFO_T          t_mkv_minfo_obj; 
    UINT16                ui2_rec_nums;
    UINT32                ui4_rec_sz;

    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_mkv_uop_hdlr_obj = _mm_util_mem_alloc(sizeof(MM_HDLR_UOP_MKV_OBJ));
    if(NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_NO_RESOURCE;
    }
    x_memset(pt_mkv_uop_hdlr_obj, 0, sizeof(MM_HDLR_UOP_MKV_OBJ));
    pt_obj->pv_uop_hdr_private_obj = (VOID*)pt_mkv_uop_hdlr_obj;

    ui4_rec_sz = sizeof(MINFO_INFO_T);
    x_memset(&t_mkv_minfo_obj, 0, ui4_rec_sz);
    
    /*begin to retrieve data for later use*/
    i4_ret =  x_minfo_get_num_recs(pt_obj->h_media_info, 
                                   MINFO_INFO_TYPE_GENERIC , 
                                   &ui2_rec_nums);
    if(ui2_rec_nums == 0 ||ui2_rec_nums > 1 )
    {
        return SVCTXR_NOT_SUPPORT;
    }

    /*get generic info*/
    i4_ret = x_minfo_get_info (pt_obj->h_media_info,    
                               MINFO_INFO_TYPE_GENERIC,
                               0,          
                               (VOID*)&t_mkv_minfo_obj,       
                               (SIZE_T*)&ui4_rec_sz);
    if (MINFOR_OK != i4_ret)
    {
        _mm_util_mem_free(pt_mkv_uop_hdlr_obj);
        pt_obj->pv_uop_hdr_private_obj = NULL;
        return SVCTXR_FAIL;
    }
    pt_mkv_uop_hdlr_obj->t_minfo_generic = t_mkv_minfo_obj.u.t_general_info;                    
#if 1/*MKV DivXPlus*/
    pt_mkv_uop_hdlr_obj->t_cur_title.ui4_title_len = 128;
    if(SVCTXR_OK != mkv_uop_hdlr_get_chapter_def_info(pt_obj))
    {
        pt_mkv_uop_hdlr_obj->b_has_chapter = FALSE;
    }
    else
    {
        pt_mkv_uop_hdlr_obj->b_has_chapter = TRUE;
    }
#endif
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_hdlr_is_hdlr
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
INT32 mkv_uop_hdlr_is_hdlr(MEDIA_DESCRIBT_T* pt_obj,
                           MEDIA_FORMAT_T*   pt_media_format)
{
    /*check paprameter*/
   if((NULL == pt_obj) || ( NULL == pt_media_format))
   {
     return SVCTXR_INV_ARG;
   }
   
   if((pt_media_format->e_media_type == MEDIA_TYPE_CONTAINER)
       && (pt_media_format->t_media_subtype.u.e_contnr_subtype 
           == MEDIA_CONTNR_SUBTYPE_MATROSKA))
   {
      return SVCTXR_OK;
   }
   else
   {
      return SVCTXR_FAIL;
   }
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_hdlr_close
 *
 * Description:This function free used data  when close mkv uop handler
 *
 * Inputs:  pv_obj       specify the media description object  
 *
 * Outputs: NULL 
 *
 * Returns: SVCTXR_OK    Success. The routine was successful and the 
 *                       operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mkv_uop_hdlr_close (MEDIA_DESCRIBT_T* pt_obj)
{
    MM_HDLR_UOP_MKV_OBJ* pt_mkv_uop_hdlr_obj;
    
    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_mkv_uop_hdlr_obj = (MM_HDLR_UOP_MKV_OBJ*)pt_obj->pv_uop_hdr_private_obj;
    if(NULL != pt_mkv_uop_hdlr_obj)
    {
        _mm_util_mem_free(pt_mkv_uop_hdlr_obj);
        pt_obj->pv_uop_hdr_private_obj = NULL;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_hdlr_get_tm_code
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
INT32 mkv_uop_hdlr_get_tm_code (
                                MEDIA_DESCRIBT_T*         pt_obj,
                                MM_SVC_TIME_CODE_INFO_T*  pt_time_info)
{
    MM_HDLR_UOP_MKV_OBJ*    pt_mkv_uop_hdlr_obj = NULL;
    MM_SVC_TM_CODE_TYPE_T   e_tm_code_type; 
    INT32                   i4_ret = 0;
    UINT32                  i4_cnt = 0;
    UINT64                  ui8_time = 0;
    SIZE_T                  z_len;
    MINFO_CHAP_INFO_T       pt_chapter;
    
   /*check paprameter*/
    if ((NULL == pt_obj) || (NULL == pt_time_info))
    {
        return SVCTXR_INV_ARG;
    }

    pt_mkv_uop_hdlr_obj = (MM_HDLR_UOP_MKV_OBJ*)pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }
    e_tm_code_type = pt_time_info->t_tm_code_type;
    switch(e_tm_code_type)
    {
        case MM_SVC_TM_CODE_TYPE_TITLE:
            break;
        case MM_SVC_TM_CODE_TYPE_CHAPTER:
            for (i4_cnt = 0; i4_cnt < pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num ; i4_cnt ++)
            {
                z_len = sizeof(MINFO_CHAP_INFO_T);
                pt_chapter.ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_title_idx;
                pt_chapter.ui4_playlist_idx = pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx;
                pt_chapter.ui4_chap_idx = i4_cnt;
                i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
                        MINFO_INFO_TYPE_CHAP_INFO, 
                        0, 
                        &pt_chapter, 
                        &z_len);
                if(i4_ret != MINFOR_OK)
                {
                    return SVCTXR_NOT_SUPPORT;
                }
                
                if ((ui8_time + pt_chapter.ui8_end_time - pt_chapter.ui8_start_time+ 1) >= pt_time_info->ui8_time)
                {
                    pt_time_info->ui2_idx = pt_chapter.ui4_chap_idx;
                    pt_time_info->ui8_time = pt_time_info->ui8_time - ui8_time + pt_chapter.ui8_start_time;
                    return SVCTXR_OK;
                }
                ui8_time += (pt_chapter.ui8_end_time - pt_chapter.ui8_start_time + 1);
            }
            if (i4_cnt >= pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num)
            {
                return SVCTXR_FAIL;
            }
            break;
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
 * Name:  mkv_uop_hdlr_get_dur
 *
 * Description:This function get the time code 
 *
 * Inputs:  pv_obj,     specify the media description object  
 *          e_dur_type, specify the duration type
 *
 * Outputs: pt_time_dur,output duration time
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/
INT32 mkv_uop_hdlr_get_dur (MEDIA_DESCRIBT_T*       pt_obj , 
                            MM_SVC_DUR_INFO_T*      pt_time_dur)
{
/*
    MM_HDLR_UOP_MKV_OBJ*  pt_mkv_uop_hdlr_obj;*/
    MM_SVC_DUR_TYPE_T     e_dur_type; 
    HANDLE_T              h_minfo;
    MINFO_INFO_T          t_media_info;  
    UINT16                ui2_rec_nums;
    SIZE_T                ui4_rec_sz;
    INT32                 i4_ret;
    
    /*check paprameter*/
    if((NULL == pt_obj) ||(NULL == pt_time_dur))
    {
        return SVCTXR_INV_ARG;
    }
    x_memset(&t_media_info, 0, sizeof (MINFO_INFO_T));
    /*
    pt_mkv_uop_hdlr_obj = (MM_HDLR_UOP_MKV_OBJ*)pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }*/
    
    e_dur_type = pt_time_dur->t_dur_type;
    h_minfo =  pt_obj->h_media_info;
    
    switch(e_dur_type)
    {
        case MM_SVC_DUR_TYPE_TITLE:
        case MM_SVC_DUR_TYPE_CHAPTER:
            return SVCTXR_NOT_SUPPORT;
        case MM_SVC_DUR_TYPE_ALL:
            pt_time_dur->t_dur_type = e_dur_type;
            pt_time_dur->ui2_idx = 0;
            x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_GENERIC , &ui2_rec_nums);
            if(ui2_rec_nums == 0 || ui2_rec_nums > 1 )
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
            /*
            pt_time_dur->ui4_duration = (UINT32)
                         pt_mkv_uop_hdlr_obj->t_minfo_generic.ui8_pb_duration;*/
        break;
        case MM_SVC_DUR_TYPE_BUFFERED:
        default:
        break;
    }
    
    return SVCTXR_OK;
 }


/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_handler_search_fct
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
INT32 mkv_uop_handler_search_fct (MEDIA_DESCRIBT_T*  pt_obj , 
                                  MM_SVC_POS_INFO_T* pt_srh_info)
{
   return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_handler_search_fct
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
INT32 mkv_uop_handler_trick_fct (
                                 MEDIA_DESCRIBT_T*       pt_obj, 
                                 MM_SVC_TRICK_INFO_T*    pt_trick_info)
{
   return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_handler_btn_fct
 *
 * Description:This function is menu button function used by divx spec.
 *
 * Inputs:  pv_obj,       specify the media description object  
 *          pt_btn_info , output button info
 *
 * Outputs: NULL
 *
 * Returns: SVCTXR_OK,    Success. The routine was successful and the 
 *                        operation has been completed.
 *          Non-Zero ,    If fail
 ----------------------------------------------------------------------------*/

INT32 mkv_uop_handler_btn_fct ( MEDIA_DESCRIBT_T* pt_obj , 
                                MM_SVC_NAV_INFO_T* pt_btn_info)
{
    MM_SVC_BTN_ACTION_INFO_T*           pt_btn_action_info;
    MM_SVC_REQ_T*                       pt_running_svc_req;
    INT32                               i4_ret = SVCTXR_OK;
    MM_SVC_DUR_INFO_T                   t_dur_code;
    MM_RANGE_POS_INFO_T                 t_set_info;
    SIZE_T                              z_get_info_size;
    SVCTX_T*                            pt_svctx;
    MM_SVC_TIME_CODE_INFO_T             t_tm_code;
    UINT32                              ui4_value = 0;
    SIZE_T                              z_size = 0;
    MM_SVC_POS_INFO_T                   t_pos_info;
    MM_HDLR_UOP_MKV_OBJ*                pt_mkv_uop_hdlr_obj = NULL;
    MINFO_CHAP_INFO_T                   t_chap_info;
    
    /*check parameters*/
    if ((pt_obj == NULL) || (pt_btn_info == NULL))
    {
        return SVCTXR_INV_ARG;
    }

    if(pt_obj->b_is_seeking)
    {
       return  SVCTXR_WRONG_STATE;
    }
    
    pt_mkv_uop_hdlr_obj = pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }
    
    x_memset(&t_set_info,0,sizeof(MM_RANGE_POS_INFO_T));
    x_memset(&t_pos_info,0,sizeof(MM_SVC_POS_INFO_T));
    i4_ret = svctx_get_obj(pt_obj->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"_mm_hdlr_get_media_obj(pt_svctx) failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
}
    pt_btn_action_info = &pt_btn_info->t_btn_action_info;
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req->pt_player->e_sess_cond !=SM_SESS_COND_STARTED)
    {
        return SVCTXR_WRONG_STATE;
}
    t_dur_code.t_dur_type = MM_SVC_DUR_TYPE_ALL;
    t_dur_code.ui2_idx = (UINT16)0;
    
    i4_ret = mm_hdlr_get(
                    pt_svctx,
                    SVCTX_MM_GET_TYPE_DUR,
                    (VOID*)&t_dur_code,
                    &z_size
                    );
    if (i4_ret != SVCTXR_OK)
    {
       i4_ret =  SVCTXR_FAIL;     
    }

    switch (pt_btn_action_info->t_btn_type.t_btn_type)
    {
        case MM_SVC_BTN_TYPE_SELECT_DIGTS:              
            if (pt_btn_action_info->t_btn_type.u.ui4_value >= (UINT32)10)
            {
                return SVCTXR_INV_ARG;
            }
            
            t_set_info.ui8_pos_val = t_dur_code.ui8_duration*pt_btn_action_info->t_btn_type.u.ui4_value/10;
            t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
            i4_ret = SVCTXR_OK;

            break;
        case MM_SVC_BTN_TYPE_SELECT_NEXT:

            if (TRUE == pt_mkv_uop_hdlr_obj->b_has_auth_chap)
            {
                i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_obj, 1, &t_chap_info);
                if(i4_ret == SVCTXR_OK)
                {
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_obj, 0, &t_chap_info);
                    if (i4_ret == SVCTXR_OK)
                    {
                        t_set_info.ui8_pos_val = (UINT64)((((UINT64)t_chap_info.ui4_playlist_idx) << 32) | (UINT64)(t_chap_info.ui4_chap_idx + 1));
                        t_pos_info.t_pos_type = MM_SVC_POS_TYPE_CHAPTER;
                    }
                    else
                    {
                        i4_ret = SVCTXR_FAIL;
                    }
                }
                else
                {
                    i4_ret = SVCTXR_FAIL;
                }
                break;
            }

            x_memset(&t_tm_code,0,sizeof(MM_SVC_TIME_CODE_INFO_T));
            z_get_info_size = sizeof(MM_SVC_TIME_CODE_INFO_T);
            i4_ret = mm_hdlr_get(pt_svctx,
                            SVCTX_MM_GET_TYPE_TIME_CODE,
                            &t_tm_code,
                            &z_get_info_size);
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt("\n***********get timecode error**********\n");
                return SVCTXR_FAIL;
            }

            ui4_value = (t_tm_code.ui8_time*10)/t_dur_code.ui8_duration + 1;
            if (ui4_value <= 9)
            {
                t_set_info.ui8_pos_val = t_dur_code.ui8_duration*ui4_value/10;
                if ((t_dur_code.ui8_duration*ui4_value) % 10 != 0)
                {
                    t_set_info.ui8_pos_val ++;
                }
                t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
            }
            else
            {
                return SVCTXR_FAIL;
            }
            i4_ret = SVCTXR_OK;
            break;
        case MM_SVC_BTN_TYPE_SELECT_PREV:

            if (TRUE == pt_mkv_uop_hdlr_obj->b_has_auth_chap)
            {
                i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_obj, 2, &t_chap_info);
                if(i4_ret == SVCTXR_OK)
                {
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_obj, 0, &t_chap_info);
                    if (i4_ret == SVCTXR_OK)
                    {
                        t_set_info.ui8_pos_val = (UINT64)((((UINT64)t_chap_info.ui4_playlist_idx) << 32) | (UINT64)(t_chap_info.ui4_chap_idx - 1));
                        t_pos_info.t_pos_type = MM_SVC_POS_TYPE_CHAPTER;
                    }
                    else
                    {
                        i4_ret = SVCTXR_FAIL;
                    }
                }
                else
                {
                    i4_ret = SVCTXR_FAIL;
                }
                break;
            }
            
            x_memset(&t_tm_code,0,sizeof(MM_SVC_TIME_CODE_INFO_T));
            z_get_info_size = sizeof(MM_SVC_TIME_CODE_INFO_T);
            i4_ret = mm_hdlr_get(pt_svctx,
                            SVCTX_MM_GET_TYPE_TIME_CODE,
                            &t_tm_code,
                            &z_get_info_size);
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt("\n***********get timecode error**********\n");
                return SVCTXR_FAIL;
            }
            ui4_value = (t_tm_code.ui8_time*10)/t_dur_code.ui8_duration;
            if (ui4_value >=1)
            {
                ui4_value --;
                t_set_info.ui8_pos_val = t_dur_code.ui8_duration*ui4_value/10;
                if ((t_dur_code.ui8_duration*ui4_value) % 10 != 0)
                {
                    t_set_info.ui8_pos_val ++;
                }
                t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
            }
            else
            {
                return SVCTXR_FAIL;
            }
            i4_ret = SVCTXR_OK;
            break;
        default:
            i4_ret = SVCTXR_FAIL;
            break;
    }
    if (i4_ret != SVCTXR_OK)
    {
        return SVCTXR_FAIL;
    }

    t_pos_info.ui8_pos_val = t_set_info.ui8_pos_val;
    i4_ret = mm_hdlr_set(
                    pt_svctx,
                    SVCTX_MM_SET_TYPE_SEEK,
                    (VOID*)&t_pos_info,
                    sizeof(MM_SVC_POS_INFO_T)
                    );
    if(SVCTXR_OK != i4_ret)
    {
        return SVCTXR_FAIL;
    }
    pt_obj->b_is_seeking = TRUE;
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_handler_get_range_fct
 *
 * Description:This function get range
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
INT32 mkv_uop_handler_get_range_fct(
                                  MEDIA_DESCRIBT_T*               pt_obj ,
                                  UINT32                          ui4_start_pts, 
                                  MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_range_info)
{
    HANDLE_T                     h_minfo;
    UINT16                       ui2_rec_nums;
    UINT32                       ui4_rec_sz;
    MINFO_INFO_T                 t_vid_stm_info;
    MINFO_INFO_T                 t_aud_stm_info;
    MINFO_INFO_T                 t_sp_stm_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_tbl_desc;
    INT32                        i4_ret;
    UINT16                       ui2_i;

    if ((NULL == pt_obj) || (NULL == pt_range_info))
    {
        return SVCTXR_INV_ARG;
    }

    h_minfo = pt_obj->h_media_info;
    ui2_rec_nums = 0;
    ui4_rec_sz = sizeof(MINFO_INFO_T);
    x_memset(&t_vid_stm_info, 0, ui4_rec_sz);
    x_memset(&t_aud_stm_info, 0, ui4_rec_sz);
    x_memset(&t_sp_stm_info, 0, ui4_rec_sz);
    
    /*get video stream info*/
    i4_ret = x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR , 
                                  &ui2_rec_nums);  
    if(MINFOR_OK != i4_ret)/*maybe ui2_rec_nums == 0*/
    {
        return SVCTXR_NOT_SUPPORT;
    }    

    i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR, 1,
                              &t_vid_stm_info,
                              (SIZE_T*)&ui4_rec_sz);
    if (MINFOR_OK != i4_ret)
    {
        return SVCTXR_FAIL;
    }
    
    /*stm_id has been assigned in function _mm_hdlr_media_preprocess*/
    i4_ret = _mkv_uop_strema_id_2_ele_tbl_desc(
                    t_vid_stm_info.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id,
                    pt_range_info,
                    &pt_tbl_desc);   
    if (SVCTXR_OK == i4_ret)
    {
        pt_tbl_desc->u_info.t_es.ui4_scale = 
            t_vid_stm_info.u.t_stm_attribute.u.t_mkv_video_stm_attr.ui4_scale;
        pt_tbl_desc->u_info.t_es.ui4_rate = 
            t_vid_stm_info.u.t_stm_attribute.u.t_mkv_video_stm_attr.ui4_rate;
        pt_tbl_desc->u_info.t_es.ui4_sample_sz = 0;  
        pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 0;
        pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
    }
    else
    {
        /*do nothing*/
    }


    /*get audio stream info*/
    i4_ret = x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR , 
                                  &ui2_rec_nums);  
    if(MINFOR_OK != i4_ret)/*maybe ui2_rec_nums == 0*/
    {
        return SVCTXR_NOT_SUPPORT;
    }    
    for(ui2_i = 1 ; ui2_i <= ui2_rec_nums ; ui2_i++)
    {
        ui4_rec_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, 
                                    MINFO_INFO_TYPE_AUDIO_ATTR, 
                                    (MINFO_REC_IDX_T)ui2_i,
                                    &t_aud_stm_info, 
                                    (SIZE_T*)&ui4_rec_sz);
        if (MINFOR_OK != i4_ret)
        {
            return SVCTXR_FAIL;
        }
        i4_ret = _mkv_uop_strema_id_2_ele_tbl_desc(
                        t_aud_stm_info.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_aud_stm_info.u.t_stm_attribute.u.t_mkv_audio_stm_attr.ui4_scale;
            pt_tbl_desc->u_info.t_es.ui4_rate = 
                t_aud_stm_info.u.t_stm_attribute.u.t_mkv_audio_stm_attr.ui4_rate;   
            pt_tbl_desc->u_info.t_es.ui4_sample_sz = 
                (UINT32)t_aud_stm_info.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i4_samples_per_sec;  
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        }
        else
        {
            /*do nothing*/
        }
    }
    
    /*get sp stream info*/
    i4_ret = x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_SP_ATTR , 
                                  &ui2_rec_nums);  
    if(MINFOR_OK != i4_ret)/*maybe ui2_rec_nums == 0*/
    {
        return SVCTXR_NOT_SUPPORT;
    }    
    for(ui2_i = 1 ; ui2_i <= ui2_rec_nums ; ui2_i++)
    {
        ui4_rec_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, 
                                    MINFO_INFO_TYPE_SP_ATTR, 
                                    (MINFO_REC_IDX_T)ui2_i,
                                    &t_sp_stm_info, 
                                    (SIZE_T*)&ui4_rec_sz);
        if (MINFOR_OK != i4_ret)
        {
            return SVCTXR_FAIL;
        }
        i4_ret = _mkv_uop_strema_id_2_ele_tbl_desc(
                        t_sp_stm_info.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_SUBTITLE;
        }
        else
        {
            /*do nothing*/
        }
    }
  
     /*PTS for 1sed = 90K*1 */
     pt_range_info->u_range.t_es.ui4_start_pts = ui4_start_pts ;
     pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
     pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;     

     /*PTS for 1sed = 90K*1 */
     pt_range_info->u_range.t_es.ui4_start_pts = ui4_start_pts ;
     pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
     pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;     
     return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_uop_handler_get_decode_info_fct
 *
 * Description:This function get cotent encodings
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
INT32 mkv_uop_handler_get_decode_info_fct(
                        MEDIA_DESCRIBT_T*             pt_obj ,
                        MM_RANGE_INFO_T*              pt_range_info)
{
    HANDLE_T                        h_minfo;
    UINT16                          ui2_rec_nums;
    UINT32                          ui4_rec_sz;
    MINFO_INFO_T                    t_vid_stm_info;
    MINFO_MKV_VIDEO_STM_ATRBT_T*    pt_mkv_vid_info = NULL;
    MM_RANGE_DECODER_T*             pt_mm_range_decoder = NULL; 
    MM_RANGE_DECODER_T*             pt_mm_range_decoder_last = NULL; 
    MINFO_INFO_T                    t_aud_stm_info;
    MINFO_MKV_AUDIO_STM_ATRBT_T*    pt_mkv_aud_info = NULL;
    INT32                           i4_ret;
    UINT16                          ui2_i;
    UINT16                          ui2_cnt = 0;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding = NULL;

    if ((NULL == pt_obj) || (NULL == pt_range_info))
    {
        return SVCTXR_INV_ARG;
    }

    h_minfo = pt_obj->h_media_info;
    ui2_rec_nums = 0;
    ui4_rec_sz = sizeof(MINFO_INFO_T);
    x_memset(&t_vid_stm_info, 0, ui4_rec_sz);
    x_memset(&t_aud_stm_info, 0, ui4_rec_sz);
    
    /*get video stream info*/
    i4_ret = x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR , 
                                  &ui2_rec_nums);  
    if(MINFOR_OK != i4_ret)/*maybe ui2_rec_nums == 0*/
    {
        return SVCTXR_NOT_SUPPORT;
    }    

    i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR, 1,
                              &t_vid_stm_info,
                              (SIZE_T*)&ui4_rec_sz);
    if (MINFOR_OK != i4_ret)
    {
        return SVCTXR_FAIL;
    }
    pt_mkv_vid_info = &t_vid_stm_info.u.t_stm_attribute.u.t_mkv_video_stm_attr;
    pt_content_encoding = NULL;
    pt_mm_range_decoder = NULL;
    if (pt_mkv_vid_info->b_has_decode_setting)
    {
        pt_mm_range_decoder = (MM_RANGE_DECODER_T*)x_mem_alloc(sizeof(MM_RANGE_DECODER_T));
        if (pt_mm_range_decoder == NULL)
        {
            return SVCTXR_FAIL;
        }
        if (pt_mm_range_decoder != NULL)
        {
            x_memset(pt_mm_range_decoder,0,sizeof(MM_RANGE_DECODER_T));
            pt_mm_range_decoder->b_has_encoding = TRUE;
            pt_mm_range_decoder->ui4_encoding_num = pt_mkv_vid_info->ui4_encoding_num;
            pt_mm_range_decoder->ui4_strm_id = t_vid_stm_info.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
            for (ui2_i = 0; ui2_i < pt_mkv_vid_info->ui4_encoding_num; ui2_i ++)
            {
                if (!pt_content_encoding)
                {
                    pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                    if (pt_mm_range_decoder == NULL)
                    {
                        return SVCTXR_FAIL;
                    }
                    pt_mm_range_decoder->pv_encoding_string = (VOID*)pt_content_encoding;
                }
                else if (!pt_content_encoding->pt_next)
                {
                    pt_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                    if (pt_mm_range_decoder->pt_next == NULL)
                    {
                        break;
                    }
                    pt_content_encoding = pt_content_encoding->pt_next;
                }
                else
                {
                    break;
                }
                if (pt_content_encoding != NULL)
                {
                    x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                    pt_content_encoding->ui8_order = pt_mkv_vid_info->pt_content_encoding->ui8_order;
                    pt_content_encoding->ui8_scope = pt_mkv_vid_info->pt_content_encoding->ui8_scope;
                    pt_content_encoding->ui8_type = pt_mkv_vid_info->pt_content_encoding->ui8_type;
                    pt_content_encoding->ui8_compress_algo = pt_mkv_vid_info->pt_content_encoding->ui8_compress_algo;
                    pt_content_encoding->ui8_comp_settings_length = pt_mkv_vid_info->pt_content_encoding->ui8_comp_settings_length;
                    pt_content_encoding->pui1_comp_settings_data = (UINT8*)x_mem_alloc((UINT32)pt_content_encoding->ui8_comp_settings_length);
                    if (pt_content_encoding->pui1_comp_settings_data != NULL)
                    {
                        x_memcpy((VOID*)(pt_content_encoding->pui1_comp_settings_data),
                                         (VOID*)pt_mkv_vid_info->pt_content_encoding->pui1_comp_settings_data, 
                                         (UINT32)pt_content_encoding->ui8_comp_settings_length);
                    }
                }
            }
        }
    }
    pt_range_info->pt_vid_decoder_info = pt_mm_range_decoder;

    /*get audio stream info*/
    pt_mm_range_decoder_last = NULL;
    i4_ret = x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR , 
                                  &ui2_rec_nums);  
    if(MINFOR_OK != i4_ret)/*maybe ui2_rec_nums == 0*/
    {
        return SVCTXR_NOT_SUPPORT;
    }    
    pt_mm_range_decoder = NULL;
    pt_mm_range_decoder = (MM_RANGE_DECODER_T*)x_mem_alloc(sizeof(MM_RANGE_DECODER_T));
    if (pt_mm_range_decoder == NULL)
    {
        return SVCTXR_FAIL;
    }
    x_memset(pt_mm_range_decoder,0,sizeof(MM_RANGE_DECODER_T));
    pt_range_info->pt_aud_decoder_info = pt_mm_range_decoder;
    for(ui2_i = 1 ; ui2_i <= ui2_rec_nums ; ui2_i++)
    {
        ui4_rec_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, 
                                    MINFO_INFO_TYPE_AUDIO_ATTR, 
                                    (MINFO_REC_IDX_T)ui2_i,
                                    &t_aud_stm_info, 
                                    (SIZE_T*)&ui4_rec_sz);
        if (MINFOR_OK != i4_ret)
        {
            return SVCTXR_FAIL;
        }
        pt_mkv_aud_info = &t_aud_stm_info.u.t_stm_attribute.u.t_mkv_audio_stm_attr;
        
        if(pt_mkv_aud_info->b_has_decode_setting || pt_mkv_aud_info->b_has_codec_private)
        {
            if (pt_mm_range_decoder == NULL)
            {
                pt_mm_range_decoder = (MM_RANGE_DECODER_T*)x_mem_alloc(sizeof(MM_RANGE_DECODER_T));
                if (pt_mm_range_decoder == NULL)
                {
                    return SVCTXR_FAIL;
                }
                x_memset(pt_mm_range_decoder,0,sizeof(MM_RANGE_DECODER_T));
                pt_mm_range_decoder_last->pt_next = pt_mm_range_decoder;
            }
            pt_mm_range_decoder->ui4_strm_id = t_aud_stm_info.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
            if (pt_mkv_aud_info->b_has_decode_setting)
            {
                if (pt_mm_range_decoder != NULL)
                {
                    pt_mm_range_decoder->b_has_encoding = TRUE;
                    pt_mm_range_decoder->ui4_encoding_num = pt_mkv_aud_info->ui4_encoding_num;
                    pt_content_encoding = NULL;
                    for (ui2_cnt = 0; ui2_cnt < pt_mkv_aud_info->ui4_encoding_num; ui2_cnt ++)
                    {
                        if (!pt_content_encoding)
                        {
                            pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                            if (pt_content_encoding == NULL)
                            {
                                return SVCTXR_FAIL;
                            }
                            x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                            pt_mm_range_decoder->pv_encoding_string = (VOID*)pt_content_encoding;
                        }
                        else if (!pt_content_encoding->pt_next)
                        {
                            pt_content_encoding->pt_next = (MINFO_MKV_CONTENT_ENCODING_T*)x_mem_alloc(sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                            if (pt_content_encoding->pt_next == NULL)
                            {
                                return SVCTXR_FAIL;
                            }
                            x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                            pt_content_encoding = pt_content_encoding->pt_next;
                        }
                        else
                        {
                            return SVCTXR_FAIL;
                        }
                        if (pt_content_encoding != NULL)
                        {
                            x_memset(pt_content_encoding,0,sizeof(MINFO_MKV_CONTENT_ENCODING_T));
                            pt_content_encoding->ui8_order = pt_mkv_aud_info->pt_content_encoding->ui8_order;
                            pt_content_encoding->ui8_scope = pt_mkv_aud_info->pt_content_encoding->ui8_scope;
                            pt_content_encoding->ui8_type = pt_mkv_aud_info->pt_content_encoding->ui8_type;
                            pt_content_encoding->ui8_compress_algo = pt_mkv_aud_info->pt_content_encoding->ui8_compress_algo;
                            pt_content_encoding->ui8_comp_settings_length = pt_mkv_aud_info->pt_content_encoding->ui8_comp_settings_length;
                            pt_content_encoding->pui1_comp_settings_data = (UINT8*)x_mem_alloc((UINT32)pt_content_encoding->ui8_comp_settings_length);
                            if (pt_content_encoding->pui1_comp_settings_data != NULL)
                            {
                                x_memcpy((VOID*)(pt_content_encoding->pui1_comp_settings_data),
                                                 (VOID*)pt_mkv_aud_info->pt_content_encoding->pui1_comp_settings_data, 
                                                 (UINT32)pt_content_encoding->ui8_comp_settings_length);
                            }
                        }
                    }
                }
            }
            if(pt_mkv_aud_info->b_has_codec_private && (pt_mm_range_decoder != NULL))
            {
                pt_mm_range_decoder->ui8_codec_private_len  = pt_mkv_aud_info->ui8_codec_private_len;
                pt_mm_range_decoder->b_has_codec_private = TRUE;
                pt_mm_range_decoder->pui1_codec_private     = (UINT8*)x_mem_alloc((UINT32)pt_mm_range_decoder->ui8_codec_private_len);
                if(pt_mm_range_decoder->pui1_codec_private != NULL)
                {
                    x_memcpy((VOID*)(pt_mm_range_decoder->pui1_codec_private),
                                                 (VOID*)pt_mkv_aud_info->pui1_codec_private, 
                                                 (UINT32)pt_mm_range_decoder->ui8_codec_private_len);
                }
            }
            if (pt_mm_range_decoder != NULL)
            {
                pt_mm_range_decoder_last = pt_mm_range_decoder;
                pt_mm_range_decoder_last->pt_next = NULL;
                pt_mm_range_decoder = NULL;
            }
        }
    }  
    return SVCTXR_OK;
}

#if 1/*MKV DivXPlus*/
static INT32 mkv_uop_hdlr_get_chapter_def_info(MEDIA_DESCRIBT_T* pt_obj)
{
    INT32  i4_ret = 0;
    SIZE_T z_len = 0;
    UINT32 ui4_playlist_id = 0;
    UINT32 ui4_chap_id = 0;
    
    HANDLE_TYPE_T         e_handle_type;
    MM_HDLR_UOP_MKV_OBJ*  pt_mkv_uop_hdlr_obj = NULL;
    MINFO_OBJ_T*          pt_minfo_obj = NULL;
    void*                 pv_obj = NULL;
    
    /*get mkv uop object*/
    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    pt_mkv_uop_hdlr_obj = pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }

    /*get minfo object*/
    i4_ret = handle_get_type_obj(pt_obj->h_media_info, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK)
        || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MINFOR_INV_HANDLE;
    }
    pt_minfo_obj = pv_obj;

    /*get title info*/
    pt_mkv_uop_hdlr_obj->t_cur_title.ui4_title_idx = pt_minfo_obj->ui4_cur_title;
    z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_title);
    i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
        MINFO_INFO_TYPE_TITLE_INFO, 
        0, 
        &pt_mkv_uop_hdlr_obj->t_cur_title, 
        &z_len);
    if(i4_ret != MINFOR_OK || pt_mkv_uop_hdlr_obj->t_cur_title.ui4_playlist_num == 0)
    {
         return SVCTXR_NOT_SUPPORT;
    }

    pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_title.ui4_title_idx;
    for(ui4_playlist_id = 0; ui4_playlist_id < pt_mkv_uop_hdlr_obj->t_cur_title.ui4_playlist_num; ui4_playlist_id++)
    {
        /*get playlist info*/
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_playlist);
        pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx = ui4_playlist_id;
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_PLAYLIST_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_playlist, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        if(pt_mkv_uop_hdlr_obj->t_cur_playlist.fg_default)
        {
            break;
        }
    }
    if(ui4_playlist_id == pt_mkv_uop_hdlr_obj->t_cur_title.ui4_playlist_num)
    {
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_playlist);
        pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx = 0;
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_PLAYLIST_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_playlist, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        if (pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num > 1)
        {
            pt_obj->b_has_chapter = TRUE;
        }
        else
        {
            pt_obj->b_has_chapter = FALSE;
        }
    }

    pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_title_idx    = pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_title_idx;
    pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_playlist_idx = pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx;
    for(ui4_chap_id = 0; ui4_chap_id < pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num; ui4_chap_id++)
    {
        /*get chapter info*/
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_chap);
        pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx = ui4_chap_id;
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_CHAP_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_chap, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        if (pt_mkv_uop_hdlr_obj->t_cur_chap.fg_hidden == FALSE)
        {
            pt_mkv_uop_hdlr_obj->b_has_auth_chap = TRUE;
        }
        if(pt_mkv_uop_hdlr_obj->t_cur_chap.fg_enable)
        {
            break;
        }
    }
    if(ui4_chap_id == pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num)
    {
        return SVCTXR_NOT_SUPPORT;
    }
    return SVCTXR_OK;
}

INT32 mkv_uop_handler_get_cur_chap(MEDIA_DESCRIBT_T*  pt_obj, 
                                           UINT16 ui2_idx,
                                           MINFO_CHAP_INFO_T* pt_chapter)
{
    MM_HDLR_UOP_MKV_OBJ*  pt_mkv_uop_hdlr_obj = NULL;
    INT32  i4_ret;
    SIZE_T z_len;
    
    /*get mkv uop object*/
    if (NULL == pt_obj || NULL == pt_chapter)
    {
        return SVCTXR_INV_ARG;
    }
    pt_mkv_uop_hdlr_obj = pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }

    if(FALSE == pt_mkv_uop_hdlr_obj->b_has_chapter)
    {
        return SVCTXR_NOT_SUPPORT;
    }

    if(ui2_idx == 0)
    {
        *pt_chapter = pt_mkv_uop_hdlr_obj->t_cur_chap;
        /*get chapter info*/
        z_len = sizeof(*pt_chapter);
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_CHAP_INFO, 
            0, 
            pt_chapter, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
    }
    else if(ui2_idx == 1)/*get next chapter*/
    {
        if(pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx < 
           pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num - 1)
        {
    *pt_chapter = pt_mkv_uop_hdlr_obj->t_cur_chap;
            pt_chapter->ui4_chap_idx++;
            /*get chapter info*/
            z_len = sizeof(*pt_chapter);
            i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
                MINFO_INFO_TYPE_CHAP_INFO, 
                0, 
                pt_chapter, 
                &z_len);
            if(i4_ret != MINFOR_OK)
            {
                 return SVCTXR_NOT_SUPPORT;
            }
        }
        else
        {
            return SVCTXR_NOT_SUPPORT;
        }
    }
    else  if(ui2_idx == 2)/*get prev chapter*/
    {
        if(pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx > 0)
        {
    *pt_chapter = pt_mkv_uop_hdlr_obj->t_cur_chap;
            pt_chapter->ui4_chap_idx--;
            /*get chapter info*/
            z_len = sizeof(*pt_chapter);
            i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
                MINFO_INFO_TYPE_CHAP_INFO, 
                0, 
                pt_chapter, 
                &z_len);
            if(i4_ret != MINFOR_OK)
            {
                 return SVCTXR_NOT_SUPPORT;
            }
        }
        else
        {
            return SVCTXR_NOT_SUPPORT;
        }
    }
    else 
    {
        return SVCTXR_NOT_SUPPORT;
    }
    
    return SVCTXR_OK;
}
               
INT32 mkv_uop_handler_set_cur_chap(MEDIA_DESCRIBT_T* pt_obj, 
                                           BOOL fg_next, 
                                           MM_SVC_CUR_CHAP_ID_T* pt_chap_id)
{
    INT32  i4_ret;
    SIZE_T z_len;
    MM_HDLR_UOP_MKV_OBJ*  pt_mkv_uop_hdlr_obj = NULL;
    
    /*get mkv uop object*/
    if (NULL == pt_obj)
    {
        return SVCTXR_INV_ARG;
    }
    pt_mkv_uop_hdlr_obj = pt_obj->pv_uop_hdr_private_obj;
    if (NULL == pt_mkv_uop_hdlr_obj)
    {
        return SVCTXR_INV_ARG;
    }
    
    if(FALSE == pt_mkv_uop_hdlr_obj->b_has_chapter)
    {
        return SVCTXR_NOT_SUPPORT;
    }
    if(NULL != pt_chap_id)/*wqyin*/
    {
        if(pt_chap_id->ui4_playlist_id >= pt_mkv_uop_hdlr_obj->t_cur_title.ui4_playlist_num    ||
           pt_chap_id->ui4_chapter_id >= pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num   ||
           (pt_chap_id->ui4_playlist_id == pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx &&
           pt_chap_id->ui4_chapter_id == pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx))
        {
            return SVCTXR_NOT_SUPPORT;
        }

        pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_title.ui4_title_idx;
        pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_playlist_idx = pt_chap_id->ui4_playlist_id;
        
        /*get playlist info*/
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_playlist);
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_PLAYLIST_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_playlist, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        if (pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num > 1)
        {
            pt_obj->b_has_chapter = TRUE;
        }
        else
        {
            pt_obj->b_has_chapter = FALSE;
        }
        pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_title.ui4_title_idx;
        pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_playlist_idx = pt_chap_id->ui4_playlist_id;
        pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx = pt_chap_id->ui4_chapter_id;
        /*get chapter info*/
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_chap);
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_CHAP_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_chap, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }

        pt_obj->ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_title_idx;
        pt_obj->ui4_playlist_idx = pt_chap_id->ui4_playlist_id;
        pt_obj->ui4_chap_idx = pt_chap_id->ui4_chapter_id;
    }
    else if(fg_next)
    {
        if(pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx < 
           pt_mkv_uop_hdlr_obj->t_cur_playlist.ui4_chapter_num - 1)
        {
            pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx++;
            /*get chapter info*/
            z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_chap);
            i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
                MINFO_INFO_TYPE_CHAP_INFO, 
                0, 
                &pt_mkv_uop_hdlr_obj->t_cur_chap, 
                &z_len);
            if(i4_ret != MINFOR_OK)
            {
                 return SVCTXR_NOT_SUPPORT;
            }
            pt_obj->ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_title_idx;            
            pt_obj->ui4_playlist_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_playlist_idx;            
            pt_obj->ui4_chap_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx;        
        }
        else
        {
            return SVCTXR_NOT_SUPPORT;
        }
    }
    else/*wqyin*/
    {
        if(pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx > 0)
        {
            pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx--;
        /*get chapter info*/
        z_len = sizeof(pt_mkv_uop_hdlr_obj->t_cur_chap);
        i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
            MINFO_INFO_TYPE_CHAP_INFO, 
            0, 
            &pt_mkv_uop_hdlr_obj->t_cur_chap, 
            &z_len);
        if(i4_ret != MINFOR_OK)
        {
             return SVCTXR_NOT_SUPPORT;
        }
        pt_obj->ui4_title_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_title_idx;        
        pt_obj->ui4_playlist_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_playlist_idx;        
        pt_obj->ui4_chap_idx = pt_mkv_uop_hdlr_obj->t_cur_chap.ui4_chap_idx;    
    }
        else
        {
            return SVCTXR_NOT_SUPPORT;
        }
    }
    return SVCTXR_OK;
}
               
#endif


