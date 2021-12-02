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
 * $RCSfile: mm_hdlr_uop_avi.c,v $
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
#include "mutil/minfo/avi_info/avi_menu_parser.h"
#include "svctx/svctx_dbg.h"
#include "dbg/u_dbg.h"
#include "util/x_lnk_list.h"
#include "svctx/mm_hdlr/avi_uop/mm_hdlr_uop_avi.h"

#include "unicode/x_uc_str.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* big endian for FourCC type */
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))

#define DIVX_NONE     dwBigFourCC('n', 'o', 'n', 'e')
#define TWO_MIN_RANGE_DUR 2*10*90000

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

#define TITLE_OBJECT_NUM_MAX      100
/* structure for record user setting */
typedef struct _DIVX_USER_SETTING
{
    INT32 i4_titleID;
    CHAR  sz_video_track_fourCC[5];
    BOOL  b_set_audio_track;
    CHAR  sz_audio_track_fourCC[5];
    BOOL  b_set_subtitle_track;
    CHAR  sz_subtitle_track_fourCC[5];
}DIVX_USER_SETTING;

static DIVX_MENU_OBJECT_T*  pt_divx_menu_manager = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_media_menu = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_btn_menu = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_root_media_menu = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_title_media_menu = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_chapter_media_menu = NULL;
#if 0
static DIVX_MENU_OBJECT_T*  pt_curr_audio_media_menu = NULL;
static DIVX_MENU_OBJECT_T*  pt_curr_subtitle_media_menu = NULL;
#endif
static CHAR                 sz_user_lang_code[5];
static DIVX_USER_SETTING    t_divx_user_setting[TITLE_OBJECT_NUM_MAX];
static UINT8                ui1_user_setting_cnt = 0;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _avi_uop_strema_id_2_ele_tbl_desc(
                             UINT32 ui4_strm_id ,                            /*I*/ 
                             MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_ele_alc_info,/*I*/
                             MIDXBULD_ELMT_TBL_DESCRIB_T**   ppt_ele_tbl_desc)/*O*/
{

  MIDXBULD_ELMT_TBL_DESCRIB_T* p_head_tbl_desc;
  
  p_head_tbl_desc = pt_ele_alc_info->pt_tbl_describ;
  *ppt_ele_tbl_desc = NULL;
  
  while(p_head_tbl_desc!= NULL)
  {
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
    return SVCTXR_FAIL;
  }
  return SVCTXR_OK;
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
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 avi_uop_hdlr_open (MEDIA_DESCRIBT_T* pt_obj)
{
    #if 0
    INT32   i4_ret;
    SIZE_T  z_rec_length;
    i4_ret = x_minfo_get_info(pt_obj->h_media_info, 
                              MINFO_INFO_TYPE_IDX_INFO, 
                              1, 
                              pt_divx_menu_manager, 
                              &z_rec_length);
    if(pt_divx_menu_manager != NULL)
    {
        ui1_user_setting_cnt = 0;
        x_memset((VOID*)t_divx_user_setting, 
                 0, 
                 sizeof(DIVX_USER_SETTING) * TITLE_OBJECT_NUM_MAX);
        pt_curr_media_menu = NULL;
        pt_curr_btn_menu = NULL;
        pt_curr_root_media_menu = NULL;
        pt_curr_title_media_menu = NULL;
        pt_curr_chapter_media_menu = NULL;
        #if 0
        pt_curr_audio_media_menu = NULL;
        pt_curr_subtitle_media_menu = NULL;
        #endif
    }
    
    if (i4_ret != MINFOR_OK)
    {
       return SVCTXR_FAIL;
    }
    #endif
    return SVCTXR_OK;
}

INT32 avi_uop_hdlr_is_hdlr(MEDIA_DESCRIBT_T* pt_obj,
	                           MEDIA_FORMAT_T*    pt_favor_media_format)
{
    /*check paprameter*/
   if(pt_obj == NULL ||pt_favor_media_format ==NULL)
   {
     return SVCTXR_INV_ARG;
   }
   if(pt_favor_media_format->e_media_type == MEDIA_TYPE_CONTAINER &&
      pt_favor_media_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
   {
      return SVCTXR_OK;
   }
   else
   {
      return SVCTXR_FAIL;
   }
}

INT32 avi_uop_hdlr_close (MEDIA_DESCRIBT_T* pt_obj)
{
    return SVCTXR_OK;
}

INT32 avi_uop_hdlr_get_tm_code (
                    MEDIA_DESCRIBT_T*         pt_obj,
                    MM_SVC_TIME_CODE_INFO_T*  pt_time_info)
{
      
    
    MM_SVC_TM_CODE_TYPE_T   e_tm_code_type;  
   /*check paprameter*/
  if(pt_obj == NULL ||pt_time_info ==NULL)
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
        pt_time_info->ui8_time = 0;
      break;
     default:
      break;
  }
  return SVCTXR_OK;
}

INT32 avi_uop_hdlr_get_dur(
                    MEDIA_DESCRIBT_T*       pt_obj, 
                    MM_SVC_DUR_INFO_T*      pt_time_dur)
{
   HANDLE_T             h_minfo;
   UINT16               ui2_rec_nums;
   SIZE_T               z_rec_sz;
   MM_SVC_DUR_TYPE_T    e_dur_type; 
   MINFO_INFO_T         t_media_info;  
   INT32 i4_ret;

   /*check paprameter*/
   if(pt_obj == NULL ||pt_time_dur ==NULL)
   {
     return SVCTXR_INV_ARG;
   }
   e_dur_type = pt_time_dur->t_dur_type;
   h_minfo =  pt_obj->h_media_info;
   switch(e_dur_type)
   {
        case MM_SVC_DUR_TYPE_TITLE:
        return SVCTXR_NOT_SUPPORT;
        case MM_SVC_DUR_TYPE_CHAPTER:
        return SVCTXR_NOT_SUPPORT;
        case MM_SVC_DUR_TYPE_ALL:
        x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_GENERIC , &ui2_rec_nums);
        if(ui2_rec_nums == 0 ||ui2_rec_nums > 1 )
        {
            return SVCTXR_NOT_SUPPORT;
        }
             z_rec_sz = sizeof (MINFO_INFO_T);
        x_memset(&t_media_info, 0, z_rec_sz);
             i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_GENERIC, 1, &t_media_info, &z_rec_sz);
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

INT32 avi_uop_handler_search_fct (
                    MEDIA_DESCRIBT_T*   pt_obj, 
                    MM_SVC_POS_TYPE_T   e_srh_type, 
                    MM_SVC_POS_INFO_T*  pt_trick_info)
{
   return SVCTXR_OK;
}

static INT32 _divx_set_user_lang_code(DIVX_MENU_OBJECT_T* pt_curr_media_menu_obj)
{
    DIVX_MENU_OBJECT_T* pt_menu_par_obj;
    /*check paprameter*/
    if(pt_curr_media_menu_obj == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_menu_par_obj = pt_curr_media_menu_obj->pt_parent;
    if(pt_menu_par_obj == NULL)
    {
        return SVCTXR_FAIL;
    }
    
    if (0 == x_memcmp(pt_menu_par_obj->sz_fourCC, "LGMN" ,4))
    {
        x_memcpy(sz_user_lang_code, 
                 pt_menu_par_obj->Type.t_language_menus.pi2_language_code,
                 4);
        sz_user_lang_code[4] = '\0';
    }
    else if(0 == x_memcmp(pt_menu_par_obj->sz_fourCC, "MENU" ,4))
    {
        x_memcpy(sz_user_lang_code, 
                 pt_menu_par_obj->Type.t_divx_media_manager.pi2_default_language,
                 4);
        sz_user_lang_code[4] = '\0';
    }
    else
    {
        return SVCTXR_FAIL;
    }    
    return SVCTXR_OK;
}

static INT32 _divx_set_track_info(
                     DIVX_MENU_OBJECT_T*    pt_media_track_obj, 
                     STREAM_TYPE_T          e_stream_type,
                     MM_SVC_NAV_INFO_T*     pt_btn_info)
{
    UINT32                      ui4_idx;
    DIVX_MENU_OBJECT_LIST_T*    pt_menu_list;
    DIVX_MENU_OBJECT_T*         pt_translation_lookup;
    DIVX_MENU_OBJECT_T*         pt_translation;
    DIVX_MENU_OBJECT_T*         pt_media_source;
    
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].e_type = e_stream_type;
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].t_strm_id.e_type = STREAM_ID_TYPE_AVI;
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].t_strm_id.u.t_stm_avi_id = 
          dwBigFourCC(pt_media_track_obj->Type.t_media_track.sz_trackID[0],
                      pt_media_track_obj->Type.t_media_track.sz_trackID[1],
                      pt_media_track_obj->Type.t_media_track.sz_trackID[2],
                      pt_media_track_obj->Type.t_media_track.sz_trackID[3]);
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].ui4_start_time = 
          (UINT32)pt_media_track_obj->Type.t_media_track.i4_start_time;
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].ui4_end_time = 
          (UINT32)pt_media_track_obj->Type.t_media_track.i4_end_time;
    pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].pv_lang_code = 
        pt_media_track_obj->Type.t_media_track.pi2_language_code;

    /* media source object */
    pt_media_source = pt_media_track_obj->Type.t_media_track.pt_media_source_obj;
    if(pt_media_source != NULL)
    {
        pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].ui8_offset = 
            pt_media_source->Type.t_media_source.ui8_riff_offset;
    }

    /* translation lookup object */
    pt_translation = NULL;
    pt_translation_lookup = pt_media_track_obj->Type.t_media_track.pt_translation_lookup_obj;
    if(pt_translation_lookup != NULL)
    {
        /* todo. maybe need check translation language code */
        pt_menu_list = pt_translation_lookup->pt_child_list;
        for(ui4_idx = 0; ui4_idx < pt_translation_lookup->ui4_child_count; ui4_idx ++)
        {
            if(pt_menu_list == NULL)
            {
                /* menu parser error */
                break;
            }
            pt_translation = pt_menu_list->pt_menu;
            if (0 == x_memcmp(pt_translation->Type.t_translation.pi2_language_code, 
                              sz_user_lang_code,
                              (SIZE_T)pt_translation->Type.t_translation.i4_language_code_len))
            {
                break;
            }
            pt_menu_list = pt_menu_list->pt_next;
        }
        if(pt_translation != NULL) 
        {
            pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].pv_track_txt = 
                pt_translation->Type.t_translation.pi2_value;
        }
        else
        {
            pt_btn_info->pt_tracks[pt_btn_info->ui1_track_ns].pv_track_txt = NULL;
        }
    }
    /* track number ++ */
    pt_btn_info->ui1_track_ns ++;
    
    return SVCTXR_OK;
}

static INT32 _divx_set_tracks_by_media_menu(
                     MM_SVC_NAV_INFO_T*     pt_btn_info)
{
    DIVX_MENU_OBJECT_T*         pt_media_video_track;
    DIVX_MENU_OBJECT_T*         pt_media_audio_track;
    DIVX_MENU_OBJECT_T*         pt_media_subtitle_track;
    
    /* get current media menu background video track info */
    pt_media_video_track = pt_curr_media_menu->Type.t_divx_media_menu.pt_background_video_obj;
    if (pt_media_video_track != NULL)
    {
        _divx_set_track_info(pt_media_video_track, ST_VIDEO, pt_btn_info);
    }

    /* get current media menu background audio track info */
    pt_media_audio_track = pt_curr_media_menu->Type.t_divx_media_menu.pt_background_audio_obj;
    if (pt_media_audio_track != NULL)
    {
        _divx_set_track_info(pt_media_audio_track, ST_AUDIO, pt_btn_info);
    }

    /* get current media menu overlay (subtitle track) info */
    if(pt_curr_btn_menu != NULL)
    {
        pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
        if (pt_media_subtitle_track != NULL)
        {
            _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
        }
    }
    
    return SVCTXR_OK;
}

static INT32 _divx_set_curr_related_menu(VOID)
{
    INT32                       i4_ret;
    UINT32                      ui4_idx;
    DIVX_MENU_OBJECT_LIST_T*    pt_menu_list;
    DIVX_MENU_OBJECT_T*         pt_menu_par_obj;
    DIVX_MENU_OBJECT_T*         pt_menu_obj;
    CHAR                        sz_menu_type[64];
    
    if(pt_curr_media_menu == NULL)
    {
        return SVCTXR_FAIL;
    }
    
    pt_menu_par_obj = pt_curr_media_menu->pt_parent;
    if(pt_menu_par_obj == NULL)
    {
        return SVCTXR_FAIL;
    }

    if (0 == x_memcmp(pt_menu_par_obj->sz_fourCC, "LGMN" ,4))
    {
        pt_curr_root_media_menu = pt_menu_par_obj->Type.t_language_menus.pt_root_menu_obj;
        pt_menu_list = pt_menu_par_obj->pt_child_list;
        for(ui4_idx = 0; ui4_idx < pt_menu_par_obj->ui4_child_count; ui4_idx++)
        {
            if(pt_menu_list == NULL)
            {
                /* menu parser error */
                break;
            }
            pt_menu_obj = pt_menu_list->pt_menu;
            if(pt_menu_obj->Type.t_divx_media_menu.i4_menu_type_len > 0)
            {
                i4_ret = x_uc_w2s_to_ps((const UTF16_T*)pt_menu_obj->Type.t_divx_media_menu.pi2_menu_type, 
                                        sz_menu_type, 
                                        (UINT32)(pt_menu_obj->Type.t_divx_media_menu.i4_menu_type_len/2));
                if(i4_ret != UCR_OK)
                {
                    DBG_ERROR(("ERR: _divx_set_curr_related_menu:for menu type "
                               "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret));
                    return SVCTXR_FAIL;
                }
            }
            if (0 == x_memcmp(sz_menu_type, "title", 5))
            {
                pt_curr_title_media_menu = pt_menu_obj;
            }
            else if (0 == x_memcmp(sz_menu_type, "chapter", 7))
            {
                pt_curr_chapter_media_menu = pt_menu_obj;
            }
            #if 0
            else if (0 == x_memcmp(sz_menu_type, "audio", 5))
            {
                pt_curr_audio_media_menu = pt_menu_obj;
            }
            else if (0 == x_memcmp(sz_menu_type, "subtitle", 8))
            {
                pt_curr_subtitle_media_menu = pt_menu_obj;
            }
            #endif
            pt_menu_list = pt_menu_list->pt_next;
        }
    }
    else
    {
        pt_curr_root_media_menu = NULL;
    }
    
    return SVCTXR_OK;
}

static INT32 _divx_menu_transition(                        
                     MM_SVC_NAV_INFO_T*      pt_btn_info,
                     BOOL                    b_is_all_update)
{
    INT32                       i4_ret;
    UINT32                      ui4_idx;
    UINT8                       ui1_i;
    DIVX_MENU_OBJECT_LIST_T*    pt_menu_list;
    DIVX_MENU_OBJECT_T*         pt_menu_obj;
    DIVX_MENU_OBJECT_T*         pt_action;
    
    if(pt_curr_media_menu != NULL)
    {
        i4_ret = _divx_set_user_lang_code(pt_curr_media_menu);
        if(i4_ret != SVCTXR_OK)
        {
            return i4_ret;
        }
        
        if(b_is_all_update)
        {
            pt_curr_btn_menu = 
                pt_curr_media_menu->Type.t_divx_media_menu.pt_starting_menu_item_obj;
        }
        
        if(pt_curr_media_menu->Type.t_divx_media_menu.pt_exit_action_obj == NULL)
        {
            pt_btn_info->b_is_repeat = TRUE;
        }
        else
        {
            pt_btn_info->b_is_repeat = FALSE;
        }
        pt_btn_info->b_is_menu_domain = TRUE;

        /* check EnterAction */
        if(pt_curr_media_menu->Type.t_divx_media_menu.pt_enter_action_obj != NULL)
        {
            /* enter action may contain the following actions */
            /* 1. one or more AudioSelectAction */
            /* 2. one or more SubtitleSelectAction */
            pt_action = pt_curr_media_menu->Type.t_divx_media_menu.pt_enter_action_obj;
                
            pt_menu_list = pt_action->pt_child_list;
            for(ui4_idx = 0; ui4_idx < pt_action->ui4_child_count; ui4_idx++)
            {
                if(pt_menu_list == NULL)
                {
                    /* menu parser error */
                    break;
                }
                pt_menu_obj = pt_menu_list->pt_menu;
                if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "STAC" ,4))
                {
                    /* Subtitle Select Action */
                    for (ui1_i = 0; ui1_i < ui1_user_setting_cnt; ui1_i ++)
                    {
                        if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                        {
                            return SVCTXR_FAIL;
                        }
                        if(t_divx_user_setting[ui1_i].i4_titleID == 
                           pt_menu_obj->Type.t_subtitle_select_action.i4_titleID)
                        {
                            break;
                        }
                    }
                    if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                    {
                        return SVCTXR_FAIL;
                    }
                    t_divx_user_setting[ui1_i].i4_titleID =
                        pt_menu_obj->Type.t_subtitle_select_action.i4_titleID;
                    x_memcpy(t_divx_user_setting[ui1_i].sz_subtitle_track_fourCC, 
                             pt_menu_obj->Type.t_subtitle_select_action.sz_trackID, 
                             4);
                    t_divx_user_setting[ui1_i].b_set_subtitle_track = TRUE;
                    if(ui1_i == ui1_user_setting_cnt)
                    {
                        ui1_user_setting_cnt ++;
                    }
                }
                else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "ASAC" ,4))
                {
                    /* Audio Select Action */
                    for (ui1_i = 0; ui1_i < ui1_user_setting_cnt; ui1_i ++)
                    {
                        if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                        {
                            return SVCTXR_FAIL;
                        }
                        if(t_divx_user_setting[ui1_i].i4_titleID == 
                           pt_menu_obj->Type.t_audio_select_action.i4_titleID)
                        {
                            break;
                        }
                    }
                    if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                    {
                        return SVCTXR_FAIL;
                    }
                    t_divx_user_setting[ui1_i].i4_titleID =
                        pt_menu_obj->Type.t_audio_select_action.i4_titleID;
                    x_memcpy(t_divx_user_setting[ui1_i].sz_audio_track_fourCC, 
                             pt_menu_obj->Type.t_audio_select_action.sz_trackID, 
                             4);
                    t_divx_user_setting[ui1_i].b_set_audio_track = TRUE;
                    if(ui1_i == ui1_user_setting_cnt)
                    {
                        ui1_user_setting_cnt ++;
                    }
                }
                pt_menu_list = pt_menu_list->pt_next;
            }
        }

        /* set related menu object */
        _divx_set_curr_related_menu();

        /* set media tracks info */
        _divx_set_tracks_by_media_menu(pt_btn_info);
    }
    return SVCTXR_OK;
}

INT32 avi_uop_handler_btn_fct (
                     MEDIA_DESCRIBT_T*       pt_obj,                          
                     MM_SVC_NAV_INFO_T*      pt_btn_info)
{   
    INT32                       i4_ret;
    UINT8                       ui1_i;
    UINT32                      ui4_idx;
    MM_SVC_BTN_TYPE_T           t_curr_btn_type;
    DIVX_MENU_OBJECT_LIST_T*    pt_menu_list;
    DIVX_MENU_OBJECT_LIST_T*    pt_menu_list2;
    DIVX_MENU_OBJECT_T*         pt_menu_obj;
    DIVX_MENU_OBJECT_T*         pt_menu_obj2;
    DIVX_MENU_OBJECT_T*         pt_media_subtitle_track;
    DIVX_MENU_OBJECT_T*         pt_action;
    DIVX_MENU_OBJECT_T*         pt_title;
    DIVX_MENU_OBJECT_T*         pt_chapter_or_title;
    CHAR                        sz_audio_trackID[5];
    CHAR                        sz_subtitle_trackID[5];
    CHAR                        sz_type[64];
    /*check paprameter*/
    if(pt_obj == NULL ||pt_btn_info ==NULL)
    {
         return SVCTXR_INV_ARG;
    }
    if(pt_divx_menu_manager == NULL)
    {
        return SVCTXR_FAIL;
    }
    /* init output data */
    pt_btn_info->ui1_track_ns = 0;
    pt_btn_info->b_is_menu_domain = TRUE;
    pt_btn_info->b_is_repeat = TRUE;
    
    t_curr_btn_type = pt_btn_info->t_btn_type;

    switch(t_curr_btn_type)
    {
    case MM_SVC_BTN_TYPE_STARTING_MENU:
        /* first check LGMN by system language */
        pt_menu_list = pt_divx_menu_manager->pt_child_list;
        for (ui4_idx = 0; ui4_idx < pt_divx_menu_manager->ui4_child_count; ui4_idx ++)
        {
            if (pt_menu_list == NULL)
            {
                /* menu parser error */
                break;
            }
            pt_menu_obj = pt_menu_list->pt_menu;
            if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "LGMN" ,4))
            {
                i4_ret = x_memcmp(pt_btn_info->pv_sys_lang_code, 
                                  pt_menu_obj->Type.t_language_menus.pi2_language_code,
                                  4);
                if(0 == i4_ret)
                {
                    pt_curr_media_menu = pt_menu_obj->Type.t_language_menus.pt_starting_menu_obj;
                    break;
                }
            }
            pt_menu_list = pt_menu_list->pt_next;
        }
        /* second check menu manager starting menu */
        if(pt_curr_media_menu == NULL)
        {
            pt_curr_media_menu = pt_divx_menu_manager->Type.t_divx_media_manager.pt_starting_menu_obj;
        }

        /* third check LGMN by menu manager default language */
        if(pt_curr_media_menu == NULL)
        {
            pt_menu_list = pt_divx_menu_manager->pt_child_list;
            for(ui4_idx = 0; ui4_idx < pt_divx_menu_manager->ui4_child_count; ui4_idx ++)
            {
                if(pt_menu_list == NULL)
                {
                    /* menu parser error */
                    break;
                }
                pt_menu_obj = pt_menu_list->pt_menu;
                if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "LGMN" ,4))
                {
                    i4_ret = x_memcmp(pt_divx_menu_manager->Type.t_divx_media_manager.pi2_default_language, 
                                      pt_menu_obj->Type.t_language_menus.pi2_language_code,
                                      4);
                    if(0 == i4_ret)
                    {
                        pt_curr_media_menu = pt_menu_obj->Type.t_language_menus.pt_starting_menu_obj;
                        break;
                    }
                }
                pt_menu_list = pt_menu_list->pt_next;
            }
        }
        
        /* step4 check first LGMN starting menu */
        if(pt_curr_media_menu == NULL)
        {
            pt_menu_list = pt_divx_menu_manager->pt_child_list;
            for(ui4_idx = 0; ui4_idx < pt_divx_menu_manager->ui4_child_count; ui4_idx ++)
            {
                if(pt_menu_list == NULL)
                {
                    /* menu parser error */
                    break;
                }
                pt_menu_obj = pt_menu_list->pt_menu;
                if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "LGMN" ,4))
                {
                    pt_curr_media_menu = pt_menu_obj->Type.t_language_menus.pt_starting_menu_obj;
                    break;
                }
                pt_menu_list = pt_menu_list->pt_next;
            }
        }
        
        /* step5 skip menu, play title in first RIFF */
        if(pt_curr_media_menu == NULL)
        {
            /* skip menu */
            pt_btn_info->b_is_menu_domain = FALSE;
        }
        else
        {
            i4_ret = _divx_menu_transition(pt_btn_info, TRUE);
            if(i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
        }
        
        break;
        
    case MM_SVC_BTN_TYPE_ROOT_MENU:
        if(pt_curr_root_media_menu != NULL)
        {
            pt_curr_media_menu = pt_curr_root_media_menu;
            i4_ret = _divx_menu_transition(pt_btn_info, TRUE);
            if(i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_TITLE_MENU:
        if(pt_curr_title_media_menu != NULL)
        {
            pt_curr_media_menu = pt_curr_title_media_menu;
            i4_ret = _divx_menu_transition(pt_btn_info, TRUE);
            if(i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_CHAPTER_MENU:
        if(pt_curr_chapter_media_menu != NULL)
        {
            pt_curr_media_menu = pt_curr_chapter_media_menu;
            i4_ret = _divx_menu_transition(pt_btn_info, TRUE);
            if(i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_UP:
        if(pt_curr_btn_menu != NULL)
        {
            if(pt_curr_btn_menu->Type.t_button_menu.pt_up_action_obj != NULL)
            {
                pt_curr_btn_menu = pt_curr_btn_menu->Type.t_button_menu.pt_up_action_obj;
                pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
                if (pt_media_subtitle_track != NULL)
                {
                    _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
                }
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_DOWN:
        if(pt_curr_btn_menu != NULL)
        {
            if(pt_curr_btn_menu->Type.t_button_menu.pt_down_action_obj != NULL)
            {
                pt_curr_btn_menu = pt_curr_btn_menu->Type.t_button_menu.pt_down_action_obj;
                pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
                if (pt_media_subtitle_track != NULL)
                {
                    _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
                }
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_LEFT:
        if(pt_curr_btn_menu != NULL)
        {
            if(pt_curr_btn_menu->Type.t_button_menu.pt_left_action_obj != NULL)
            {
                pt_curr_btn_menu = pt_curr_btn_menu->Type.t_button_menu.pt_left_action_obj;
                pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
                if (pt_media_subtitle_track != NULL)
                {
                    _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
                }
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_RIGHT:
        if(pt_curr_btn_menu != NULL)
        {
            if(pt_curr_btn_menu->Type.t_button_menu.pt_right_action_obj != NULL)
            {
                pt_curr_btn_menu = pt_curr_btn_menu->Type.t_button_menu.pt_right_action_obj;
                pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
                if (pt_media_subtitle_track != NULL)
                {
                    _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
                }
            }
        }
        break;
        
    case MM_SVC_BTN_TYPE_ENTER:
        if(pt_curr_btn_menu != NULL)
        {
            if(pt_curr_btn_menu->Type.t_button_menu.pt_select_action_obj != NULL)
            {
                pt_action = pt_curr_btn_menu->Type.t_button_menu.pt_select_action_obj;
                
                pt_menu_list = pt_action->pt_child_list;
                for(ui4_idx = 0; ui4_idx < pt_action->ui4_child_count; ui4_idx++)
                {
                    if(pt_menu_list == NULL)
                    {
                        /* menu parser error */
                        break;
                    }
                    pt_menu_obj = pt_menu_list->pt_menu;
                    if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "STAC" ,4))
                    {
                        /* Subtitle Select Action */
                        for (ui1_i = 0; ui1_i < ui1_user_setting_cnt; ui1_i ++)
                        {
                            if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                            {
                                return SVCTXR_FAIL;
                            }
                            if(t_divx_user_setting[ui1_i].i4_titleID == 
                               pt_menu_obj->Type.t_subtitle_select_action.i4_titleID)
                            {
                                break;
                            }
                        }
                        if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                        {
                            return SVCTXR_FAIL;
                        }
                        t_divx_user_setting[ui1_i].i4_titleID =
                            pt_menu_obj->Type.t_subtitle_select_action.i4_titleID;
                        x_memcpy(t_divx_user_setting[ui1_i].sz_subtitle_track_fourCC, 
                                 pt_menu_obj->Type.t_subtitle_select_action.sz_trackID, 
                                 4);
                        t_divx_user_setting[ui1_i].b_set_subtitle_track = TRUE;
                        if(ui1_i == ui1_user_setting_cnt)
                        {
                            ui1_user_setting_cnt ++;
                        }
                    }
                    else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "ASAC" ,4))
                    {
                        /* Audio Select Action */
                        for (ui1_i = 0; ui1_i < ui1_user_setting_cnt; ui1_i ++)
                        {
                            if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                            {
                                return SVCTXR_FAIL;
                            }
                            if(t_divx_user_setting[ui1_i].i4_titleID == 
                               pt_menu_obj->Type.t_audio_select_action.i4_titleID)
                            {
                                break;
                            }
                        }
                        if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                        {
                            return SVCTXR_FAIL;
                        }
                        t_divx_user_setting[ui1_i].i4_titleID =
                            pt_menu_obj->Type.t_audio_select_action.i4_titleID;
                        x_memcpy(t_divx_user_setting[ui1_i].sz_audio_track_fourCC, 
                                 pt_menu_obj->Type.t_audio_select_action.sz_trackID, 
                                 4);
                        t_divx_user_setting[ui1_i].b_set_audio_track = TRUE;
                        if(ui1_i == ui1_user_setting_cnt)
                        {
                            ui1_user_setting_cnt ++;
                        }
                    }
                    else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "PLAY" ,4))
                    {
                        /* Play Action */
                        pt_title = pt_menu_obj->Type.t_play_action.pt_title_obj;
                        if (pt_title == NULL)
                        {
                            /* menu parser error */
                            break;
                        }
                        for (ui1_i = 0; ui1_i < ui1_user_setting_cnt; ui1_i ++)
                        {
                            if (ui1_i >= TITLE_OBJECT_NUM_MAX)
                            {
                                return SVCTXR_FAIL;
                            }
                            if(t_divx_user_setting[ui1_i].i4_titleID == 
                                   pt_menu_obj->Type.t_play_action.i4_titleID)
                            {
                                break;
                            }
                        }
                        /* get user setting audio/subtitle track fourCC ID */  
                        if(pt_title->Type.t_title.i4_default_audio_len > 0)
                        {
                            i4_ret = x_uc_w2s_to_ps((const UTF16_T*)pt_title->Type.t_title.pi2_default_audio_track, 
                                                    sz_audio_trackID, 
                                                    (UINT32)(pt_title->Type.t_title.i4_default_audio_len/2));
                            if(i4_ret != UCR_OK)
                            {
                                DBG_ERROR(("ERR: for title default audio track: "
                                           "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret));
                                return SVCTXR_FAIL;
                            }
                        }
                        if(pt_title->Type.t_title.i4_default_subtitle_len > 0)
                        {
                            i4_ret = x_uc_w2s_to_ps((const UTF16_T*)pt_title->Type.t_title.pi2_default_subtitle_track, 
                                                    sz_subtitle_trackID, 
                                                    (UINT32)(pt_title->Type.t_title.i4_default_subtitle_len/2));
                            if(i4_ret != UCR_OK)
                            {
                                DBG_ERROR(("ERR: for title default subtitle track: "
                                           "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret));
                                return SVCTXR_FAIL;
                            }
                        }
                        if(ui1_i < ui1_user_setting_cnt)
                        {
                            if(t_divx_user_setting[ui1_i].b_set_audio_track)
                            {
                                x_memcpy(sz_audio_trackID, 
                                         t_divx_user_setting[ui1_i].sz_audio_track_fourCC, 
                                         4);
                            }
                            if(t_divx_user_setting[ui1_i].b_set_subtitle_track)
                            {
                                x_memcpy(sz_subtitle_trackID, 
                                         t_divx_user_setting[ui1_i].sz_subtitle_track_fourCC, 
                                         4);
                            }
                        }

                        /* get the chapter point or title(no chapter points) to playback */
                        pt_chapter_or_title = pt_menu_obj->Type.t_play_action.pt_media_obj;

                        /* get the track object by user setting or default */
                        pt_menu_list2 = pt_chapter_or_title->pt_child_list;
                        while(pt_menu_list2 != NULL)
                        {
                            pt_menu_obj2 = pt_menu_list2->pt_menu;
                            if (0 == x_memcmp(pt_menu_obj2->sz_fourCC, "MEDT", 4))
                            {
                                /* track type info */
                                if(pt_menu_obj2->Type.t_media_track.i4_type_len > 0)
                                {
                                    i4_ret = x_uc_w2s_to_ps((const UTF16_T*)pt_menu_obj2->Type.t_media_track.pi2_type, 
                                                            sz_type, 
                                                            (UINT32)(pt_menu_obj2->Type.t_media_track.i4_type_len/2));
                                    if(i4_ret != UCR_OK)
                                    {
                                        DBG_ERROR(("ERR: for track type discribed: "
                                                   "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret));
                                        return SVCTXR_FAIL;
                                    }
                                }
                                if(0 == x_memcmp(pt_menu_obj2->Type.t_media_track.sz_trackID, 
                                                 sz_audio_trackID, 
                                                 4))
                                {
                                    _divx_set_track_info(pt_menu_obj2, 
                                                         ST_AUDIO, 
                                                         pt_btn_info);
                                }
                                else if(0 == x_memcmp(pt_menu_obj2->Type.t_media_track.sz_trackID, 
                                                      sz_subtitle_trackID, 
                                                      4))
                                {
                                    _divx_set_track_info(pt_menu_obj2, 
                                                         ST_SUBTITLE, 
                                                         pt_btn_info);
                                }
                                else if(0 == x_memcmp(sz_type, "video", 5))
                                {
                                    _divx_set_track_info(pt_menu_obj2, 
                                                         ST_VIDEO, 
                                                         pt_btn_info);
                                }
                            }
                            pt_menu_list2 = pt_menu_list2->pt_next;
                        }
                        pt_btn_info->b_is_menu_domain = FALSE;
                    }
                    else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "PLY0" ,4))
                    {
                        /* Play From Current Offset Action */
                    }
                    else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "MNTX" ,4))
                    {
                        /* Menu Translation Action */
                        if(pt_menu_obj->Type.t_menu_transition_action.pt_menu_obj != NULL)
                        {
                            pt_curr_media_menu = pt_menu_obj->Type.t_menu_transition_action.pt_menu_obj;
                            _divx_menu_transition(pt_btn_info, TRUE);
                        }
                    }
                    else if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "BNTX" ,4))
                    {
                        /* Button Translation Action */
                        if(pt_menu_obj->Type.t_button_transition_action.pt_button_menu_obj != NULL)
                        {
                            pt_curr_btn_menu = pt_menu_obj->Type.t_button_transition_action.pt_button_menu_obj;
                            pt_media_subtitle_track = pt_curr_btn_menu->Type.t_button_menu.pt_overlay_obj;
                            if (pt_media_subtitle_track != NULL)
                            {
                                _divx_set_track_info(pt_media_subtitle_track, ST_SUBTITLE, pt_btn_info);
                            }
                        }
                    }
                    else
                    {
                        /* menu parser error */
                        break;
                    }
                    pt_menu_list = pt_menu_list->pt_next;
                }
            }
        }
        break;

    case MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK:
        /* Exit Action to perform upon the menu ending playback */
        /* The Action object must be a child of this DivxMediaMenu object */
        pt_action = pt_curr_media_menu->Type.t_divx_media_menu.pt_exit_action_obj;
        pt_menu_list = pt_action->pt_child_list;
        for(ui4_idx = 0; ui4_idx < pt_action->ui4_child_count; ui4_idx++)
        {
            if(pt_menu_list == NULL)
            {
                /* menu parser error */
                break;
            }
            pt_menu_obj = pt_menu_list->pt_menu;
            if (0 == x_memcmp(pt_menu_obj->sz_fourCC, "MNTX" ,4))
            {
                /* Menu Translation Action */
                if(pt_menu_obj->Type.t_menu_transition_action.pt_menu_obj != NULL)
                {
                    pt_menu_obj2 = pt_menu_obj->Type.t_menu_transition_action.pt_menu_obj;
                    if(pt_curr_media_menu->i4_objectID == pt_menu_obj2->i4_objectID)
                    {
                        /* menu transition not include subtitle */
                        _divx_menu_transition(pt_btn_info, FALSE);
                    }
                    else
                    {
                        pt_curr_media_menu = pt_menu_obj->Type.t_menu_transition_action.pt_menu_obj;
                        _divx_menu_transition(pt_btn_info, TRUE);
                    }
                    break;
                }
            }
            else
            {
                /* menu parser error */
                break;
            }
            pt_menu_list = pt_menu_list->pt_next;
        }
        break;
            
    default:
        break;                
    }
    
    return SVCTXR_OK;
}

INT32 avi_uop_handler_trick_fct (
                    MEDIA_DESCRIBT_T*       pt_obj, 
                    MM_SVC_TRICK_INFO_T*    pt_trick_info)
{
   return SVCTXR_OK;
}

/*check the content in MIDXBULD_ELMT_TBL_ALLOC_INFO_T to 
determine which stream(i.e stream_id)should be caculated and the memory of 
pt_range_info is allocated by client.*/
INT32 avi_uop_handler_get_range_fct(
              MEDIA_DESCRIBT_T*                pt_obj ,
              UINT32                           ui4_start_pts, 
              MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info)
{
   
    INT32 i4_ret;
    UINT16 ui2_i = 0;
    HANDLE_T h_minfo;
    UINT16  ui2_rec_nums = 0;
    UINT32  ui4_rec_sz = sizeof(MINFO_INFO_T);
    MINFO_INFO_T   t_genric_info;
    MINFO_INFO_T   t_vid_stm_info;
    MINFO_INFO_T   t_aud_stm_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_tbl_desc;

    UINT32 ui4_total_dur;
    UINT32 ui4_scale;
    UINT32 ui4_rate;
    UINT16 ui2_1minut_tbl_sz = 0;
    BOOL   b_is_full_range;
    UINT16 ui2_key_tbl_sz = 0;
    UINT16 ui2_entry_head_sz = 0;
    UINT16 ui2_entry_sz = 0;

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
                              &t_genric_info, (SIZE_T*)&ui4_rec_sz);
    if(i4_ret != MINFOR_OK)
    {
       return SVCTXR_NOT_SUPPORT;
    }

    /*ui2_start_pts might be transfer to seconds*/
    ui4_total_dur = (UINT32)(t_genric_info.u.t_general_info.ui8_pb_duration - (ui4_start_pts));
    
    /*get Key table size at first....*/
    ui2_key_tbl_sz = ui2_key_tbl_sz;      

    /*get video stream indx tble sz in 1 minutes*/
    x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR , &ui2_rec_nums);   
    if(ui2_rec_nums == 0)
    {
        return SVCTXR_NOT_SUPPORT;
    }    
    for (ui2_i=1; ui2_i<=ui2_rec_nums; ui2_i++)
    {
#ifndef MW_MM_DUAL_VID_SUPPORT
        if ( ui2_i > 1)       
        {            
            break;         
        }        
#endif
        i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_VIDEO_ATTR, ui2_i,
                              &t_vid_stm_info, (SIZE_T*)&ui4_rec_sz);
    i4_ret = _avi_uop_strema_id_2_ele_tbl_desc(
                    t_vid_stm_info.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id,
                    pt_range_info,
                    &pt_tbl_desc);
    if (SVCTXR_OK == i4_ret)
    {
        ui4_scale = pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_vid_stm_info.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_scale;
        ui4_rate =  pt_tbl_desc->u_info.t_es.ui4_rate = 
                t_vid_stm_info.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_rate;   
        pt_tbl_desc->u_info.t_es.ui4_sample_sz = 
                t_vid_stm_info.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_sample_size;  
        pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 0;/*QQQQQQQQQQQQQQ*/
        /* AVI2.0 */
        pt_tbl_desc->u_info.t_es.ui4_indx_offset = 
                t_vid_stm_info.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_indx_offset;
        pt_tbl_desc->u_info.t_es.ui4_indx_size= 
                t_vid_stm_info.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_indx_size;
        pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        ui2_1minut_tbl_sz = (UINT16)((ui4_rate/ui4_scale)*(ui2_entry_sz + ui2_entry_head_sz)*(60)); 
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
                                    (SIZE_T*)&ui4_rec_sz);
        i4_ret = _avi_uop_strema_id_2_ele_tbl_desc(
                        t_aud_stm_info.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id,
                        pt_range_info,
                        &pt_tbl_desc);
        if (SVCTXR_OK == i4_ret)
        {
            pt_tbl_desc->u_info.t_es.ui4_scale = 
                t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_scale;
            pt_tbl_desc->u_info.t_es.ui4_rate = 
                t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_rate;   
            pt_tbl_desc->u_info.t_es.ui4_sample_sz = 
                t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_sample_size;  
            
            if(pt_tbl_desc->u_info.t_es.ui4_sample_sz != 0)
            {
            /*only valid in case of CBA*/
            if(t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_avg_bytes_sec != 0)
            {
                pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 
                    t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_avg_bytes_sec;
            }
            else
            {
                pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 
                (pt_tbl_desc->u_info.t_es.ui4_rate*pt_tbl_desc->u_info.t_es.ui4_sample_sz)
                /pt_tbl_desc->u_info.t_es.ui4_scale;
            }
            }
            else
            {
                /*don't care in case of VBR*/ 
                pt_tbl_desc->u_info.t_es.ui4_avg_bytes_per_sec = 
                    t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_avg_bytes_sec;                 
            }
            /* AVI2.0 */
            pt_tbl_desc->u_info.t_es.ui4_indx_offset = 
                t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_indx_offset;
            pt_tbl_desc->u_info.t_es.ui4_indx_size= 
                t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_indx_size;
            pt_tbl_desc->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;

            ui2_1minut_tbl_sz += (t_aud_stm_info.u.t_stm_attribute.u.t_avi_audio_stm_attr.i4_samples_per_sec)*
                                 (ui2_entry_sz+ui2_entry_head_sz)*(60); 
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
    b_is_full_range = TRUE;
    if(b_is_full_range)
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
    pt_range_info->ui4_range_id = 1;  
    return SVCTXR_OK;
}

