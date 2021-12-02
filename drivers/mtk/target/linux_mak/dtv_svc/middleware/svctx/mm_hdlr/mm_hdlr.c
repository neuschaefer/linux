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
 * $RCSfile: mm_hdlr.c,v $
 * $Revision: #14 $
 * $Date: 2013/01/04 $
 * $Author: pan.huang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/13 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 7244f8264301adfae5367f1061228312 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef TIME_SHIFT_SUPPORT
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#endif
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "time_msrt/x_time_msrt.h"
#include "svctx/mm_hdlr/mm_hdlr.h"
#include "svctx/svctx_cli.h"
#include "svctx/util/player.h"
#include "mutil/mfmtrecg/x_mfmtrecg.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/mm_util.h"
#include "conn_mngr/cm_playback/x_playback_handler.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "strm_mngr/mm_sbtl_hdlr/u_sm_mm_sbtl_hdlr.h"
#include "scdb/u_scdb.h"
#ifdef ENABLE_DIVX_DRM
#include "drm/divx_drm/x_divx_drm.h"
#endif
#include "file_mngr/x_fm_mfw.h"
#include "font/x_fe.h"  /*add for font info*/
#include "drm/marlin_drm/x_marlin_drm.h"
#ifdef LINUX_TURNKEY_SOLUTION
#include "u_drv_cust.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "\n{SVCTX:MM_HDLR}Info: "
#define _API_HEADER         "\n{SVCTX:MM_HDLR}API:  "
#define _ERROR_HEADER       "\n{SVCTX:MM_HDLR}Err:  "
#define MW_HDLR_CHK_STRM_DELAY   8000 /*ms*/

typedef enum
{
    RANGE_STRM_SEL_MODE_UNKNOWN = -1,
    RANGE_STRM_SEL_MODE_ACTIVE,  /*openned stream only*/
    RANGE_STRM_SEL_MODE_ALL,     /*all stream in media*/
    RANGE_STRM_SEL_MODE_ID_LST
}   RANGE_STRM_SEL_MODE_T;

typedef struct _VSH_RM_SPEC_INFO_T
{
    UINT16          ui2_frm_width;
    UINT16          ui2_frm_height;
    UINT32          ui4_spo_extra_flags;
    UINT32          aui4_specific[25];
    BOOL            b_is_rm8;
} VSH_RM_SPEC_INFO_T;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_select_req (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_strm_req,
                    BOOL                        b_need_to_free_req
                    );
static INT32 _mm_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    );
static INT32 _mm_hdlr_check_audio_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    );
static INT32 _mm_hdlr_add_range_func( 
                    MM_SVC_REQ_T*               pt_running_svc_req, 
                    RANGE_STRM_SEL_MODE_T       e_mode
                    );
static  void _mm_hdlr_chk_strm_timeout_nfy(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    );
ASH_AUDIO_FMT_T _minfo_audio_enc_to_ash_enc(MINFO_INFO_AUDIO_ENC_T e_minfo_enc);

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
const STREAM_TYPE_T e_g_mm_strm_types[STRM_TYPE_COUNT] =
{
    ST_AUDIO,
    ST_VIDEO,
    ST_CLOSED_CAPTION,
    ST_ISDB_CAPTION,
    ST_ISDB_TEXT,
    ST_TELETEXT,
    ST_SUBTITLE
};

static SLIST_T(_MM_UOP_HANDLER_REG_T)  gt_uop_handler_reg_list = {NULL};
#ifndef DRV_NO_PRE_PARSING
HANDLE_T gh_pre_parsing_time_out = NULL_HANDLE;
UINT32   gui4_pre_parsing_time = 1000;/*ms*/
BOOL     gb_pre_parsing = FALSE;
#endif

static MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T* gpt_ts_idx_tbl_entry = NULL;

/*-----------------------------------------------------------------------------
 * SVCTX private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_media_obj
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_get_media_obj(HANDLE_T                    h_media_desc,
                             MEDIA_DESCRIBT_T**          ppt_media_desc)
{
   HANDLE_TYPE_T                    e_type;
   INT32                            i4_ret;

   i4_ret = handle_get_type_obj(h_media_desc, &e_type, (VOID**)ppt_media_desc);
   if (i4_ret != HR_OK || e_type != HT_MEDIA_STORGE_ELM_TYPE ||
       *ppt_media_desc == NULL)
   {
       x_dbg_stmt(_ERROR_HEADER"handle_get_type_obj(h_media_desc=%d) failed!\n"
                  "i4_ret=%d, e_type=%d, pv_obj=%d line = %d",
                  h_media_desc, i4_ret, e_type, *ppt_media_desc,__LINE__);
       if (i4_ret == HR_INV_HANDLE)
       {
           i4_ret = SVCTXR_INV_HANDLE;
       }
       else
       {
           i4_ret = SVCTXR_FAIL;
       }
       return i4_ret;
   }
   else
   {
       return SVCTXR_OK;
   }
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_send_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_send_msg(
                            HANDLE_T        h_svctx,
                            HANDLE_T        h_obj,
                            UINT8           ui1_msg_id,
                            VOID*           pv_tag,
                            UINT32          ui4_cond,
                            UINT32          ui4_data1,
                            UINT32          ui4_data2)
{
     SVCTX_MSG_T    t_msg              = {0};
     INT32          i4_ret =           SVCTXR_OK;
     /* not define message structure yet*/
     /*apply SVCTX_GENERIC_MSG_T*/
     t_msg.h_svctx = h_svctx;
     t_msg.u.t_generic.ui4_cond = ui4_cond;
     t_msg.u.t_generic.h_obj = h_obj;
     t_msg.u.t_generic.ui4_data1 = (UINT32)ui4_data1;
     t_msg.u.t_generic.ui4_data2 = (UINT32)ui4_data2;
     t_msg.ui1_msg_id            = ui1_msg_id;
     t_msg.pv_tag                = pv_tag;
     t_msg.pf_invoke_func        = NULL;
     /*QQQQQ h_svctx should be changed to sys_svctx*/
     i4_ret = svctx_send_msg(h_svctx, &t_msg);
     if (i4_ret != SVCTXR_OK)
     {
       /*dump error message*/
     }
     return i4_ret;

}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_range_free
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_range_free(MM_PRESENT_RANGE_LIST_T* pt_rage_node)
{
    /*clean range describtion*/
    MM_PRESENT_RANGE_LIST_T*      pt_range_lst = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*  pt_range_desc = NULL;
    MM_RANGE_ELMT_IDXTBL_T*       pt_range_table_info = NULL;
    MM_RANGE_ELMT_IDXTBL_T*       pt_range_elem_tbl   = NULL;
    MM_RANGE_DECODER_T*           pt_mm_range_decoder = NULL; 
    MM_RANGE_DECODER_T*           pt_mm_range_decoder_next = NULL; 
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding = NULL;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_content_encoding_next = NULL;
    
    pt_range_lst = pt_rage_node;
    while (pt_range_lst->t_range.pt_tbl_describ != NULL)
    {
       pt_range_desc = pt_range_lst->t_range.pt_tbl_describ->pt_next;
       x_mem_free(pt_range_lst->t_range.pt_tbl_describ);
       pt_range_lst->t_range.pt_tbl_describ = pt_range_desc;
    }

    if( pt_range_lst->pt_range_idxtbl_info != NULL)
    {
         while (pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst != NULL)
         {
             pt_range_table_info = pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pv_next_tbl;
             while (pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pt_next_partial)
             {
                pt_range_elem_tbl = pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pt_next_partial->pt_next_partial;
                x_mem_free (pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pt_next_partial);
                pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pt_next_partial = pt_range_elem_tbl;
             }
             x_mem_free(pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst);
             pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst = pt_range_table_info;
         }

         if (pt_range_lst->pt_range_idxtbl_info->pt_track_tm_scale_lst != NULL)
         {
            x_mem_free(pt_range_lst->pt_range_idxtbl_info->pt_track_tm_scale_lst);
            pt_range_lst->pt_range_idxtbl_info->pt_track_tm_scale_lst = NULL;
         }
         pt_mm_range_decoder = pt_range_lst->pt_range_idxtbl_info->pt_vid_decoder_info;
         while (pt_mm_range_decoder != NULL)
         {
             pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)pt_mm_range_decoder->pv_encoding_string;
             while(pt_content_encoding != NULL)
             {
                 if(pt_content_encoding->pui1_comp_settings_data != NULL)
                 {
                     x_mem_free(pt_content_encoding->pui1_comp_settings_data);
                     pt_content_encoding->pui1_comp_settings_data = NULL;
                 }
                 pt_content_encoding_next = pt_content_encoding->pt_next;
                 x_mem_free(pt_content_encoding);
                 pt_content_encoding = pt_content_encoding_next;
             }
             pt_mm_range_decoder_next = pt_mm_range_decoder->pt_next;
             x_mem_free(pt_mm_range_decoder);
             pt_mm_range_decoder = pt_mm_range_decoder_next;
         }

         pt_mm_range_decoder = pt_range_lst->pt_range_idxtbl_info->pt_aud_decoder_info;
         while (pt_mm_range_decoder != NULL)
         {
             pt_content_encoding = (MINFO_MKV_CONTENT_ENCODING_T*)pt_mm_range_decoder->pv_encoding_string;
             while(pt_content_encoding != NULL)
             {
                 if(pt_content_encoding->pui1_comp_settings_data != NULL)
                 {
                     x_mem_free(pt_content_encoding->pui1_comp_settings_data);
                     pt_content_encoding->pui1_comp_settings_data = NULL;
                 }
                 pt_content_encoding_next = pt_content_encoding->pt_next;
                 x_mem_free(pt_content_encoding);
                 pt_content_encoding = pt_content_encoding_next;
             }
             if(pt_mm_range_decoder->pui1_codec_private != NULL)
             {
                 x_mem_free(pt_mm_range_decoder->pui1_codec_private);
                 pt_mm_range_decoder->pui1_codec_private = NULL;
             }
             pt_mm_range_decoder_next = pt_mm_range_decoder->pt_next;
             x_mem_free(pt_mm_range_decoder);
             pt_mm_range_decoder = pt_mm_range_decoder_next;
         }   
         x_mem_free(pt_range_lst->pt_range_idxtbl_info);
    }
    x_mem_free(pt_rage_node);
    return SVCTXR_OK;
}

#ifdef TIME_SHIFT_SUPPORT
static INT32 _mm_hdlr_set_super_freeze_mode_off()
{
    INT32   i4_ret;
    HANDLE_T h_scc_vid;
    SM_COMMAND_T at_sm_cmds[3];
    SCC_VID_SUPER_FREEZE_T  t_freeze;

    /* create the scc handles */
    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = x_scc_comp_open(at_sm_cmds, &h_scc_vid);
    if(i4_ret != SMR_OK)
    {
        return SVCTXR_FAIL;
    }

    t_freeze.e_type = SCC_VID_SUPER_FREEZE_TYPE_FORCE;
    t_freeze.b_enable = FALSE;
    i4_ret = x_scc_vid_set_super_freeze(h_scc_vid, &t_freeze);

    x_scc_comp_close(h_scc_vid);

    if(i4_ret != SMR_OK)
    {
        return SVCTXR_FAIL;
    }
    return SVCTXR_OK;
}

static INT32 _mm_hdlr_handle_rec_info_updated (
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_lba_begin
                    )
{
    INT32               i4_ret;
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T            h_media ;
    MEDIA_DESCRIBT_T*   pt_media_desc;
    HANDLE_TYPE_T       e_type;
    BOOL                b_backward = FALSE;
    UINT32              ui4_mask_first = 0;
    UINT32              ui4_lba_first = 0;

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = handle_get_type_obj(h_media, &e_type, (VOID**)(&pt_media_desc));
    if (i4_ret != HR_OK || e_type != HT_MEDIA_STORGE_ELM_TYPE || pt_media_desc == NULL)
    {
        DBG_ERROR((_ERROR_HEADER
                   "handle_get_type_obj(h_media_desc=%d) failed!\r\n"
                   "i4_ret=%d, e_type=%d, pv_obj=%d\r\n",
                   h_media, i4_ret, e_type, pt_media_desc));
        /*ASSERT(FALSE, ("%s(), pt_media_desc is NULL\r\n", __func__));*/
        return SVCTXR_FAIL;
    }

    b_backward = IS_BACKWARD_DIR(pt_media_desc->e_speed);
    i4_ret = rec_util_query_info_update(pt_media_desc->h_rec_util,
                                        ui4_lba_begin,
                                        b_backward,
                                        &ui4_mask_first,
                                        &ui4_lba_first);
    if (i4_ret != REC_UTILR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"rec_util_query_info_update(lba=%u) failed, ret=%d\r\n", ui4_lba_begin, i4_ret);
        /*ASSERT(FALSE, ("%s(), rec_util_query_info_update() failed!\r\n", __func__));*/
        return SVCTXR_FAIL;
    }
    else if (ui4_mask_first != 0)
    {
        if ((!b_backward && ui4_lba_first < ui4_lba_begin) ||
            (b_backward && ui4_lba_first > ui4_lba_begin))
        {
            x_dbg_stmt((_ERROR_HEADER"invalid ui4_lba_first from rec_util_query_info_update()!\r\n"));
            /*ASSERT(FALSE, ("%s(), SM_SESS_GNRC_SET_TYPE_MONITOR_LBA failed!\r\n", __func__));*/
            return SVCTXR_FAIL;
        }

        pt_media_desc->ui4_rec_mntr_info_mask = ui4_mask_first;
        pt_media_desc->ui4_rec_mntr_lba = ui4_lba_first;

        i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_MONITOR_LBA,
                          (void*)ui4_lba_first,
                          sizeof(UINT32));
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"set monitor lba failed, ret=%d\r\n", i4_ret));
            /*ASSERT(FALSE, ("%s(), SM_SESS_GNRC_SET_TYPE_MONITOR_LBA failed!\r\n", __func__));*/
            return SVCTXR_FAIL;
        }
    }
    else
    {
        pt_media_desc->ui4_rec_mntr_info_mask = 0;
        pt_media_desc->ui4_rec_mntr_lba = 0;
    }

    DBG_INFO((_INFO_HEADER"%s(), ui4_mask_first=0x%.8X, ui4_lba_first=0x%.8X, ui4_lba_begin=0x%.8X(%d)\r\n",
              __func__, ui4_mask_first, ui4_lba_first, ui4_lba_begin, (UINT32)b_backward));

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_rec_util_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_rec_util_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_REC_UTIL_MSG_T*       pt_rec_msg
                    )
{
    INT32               i4_ret;
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    /*HANDLE_T            h_rec_util = pt_rec_msg->h_obj;*/
    MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
    UINT32              ui4_curr_lba = 0;
    UINT32              ui4_curr_tick = 0;
    MM_SVC_TRICK_INFO_T t_trick_info;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"%s(), pt_media_desc is NULL!\r\n", __func__);
        return i4_ret;
    }

    if (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        return SVCTXR_OK;
    }

    switch(pt_rec_msg->e_code) {
    case REC_UTIL_NTFY_CODE_COND_CHANGED:
        if (pt_rec_msg->uinfo.t_cond_chg.e_cond_new == REC_UTIL_COND_READY)
        {
            REC_UTIL_SHARE_DATA_T t_rec_share_data;

            DBG_INFO((_INFO_HEADER"%s(), REC_UTIL_COND_READY\r\n", __func__));
            i4_ret = rec_util_get_shared_data(pt_media_desc->h_rec_util,
                                              &t_rec_share_data);
            if (i4_ret != REC_UTILR_OK)
            {
                ASSERT(FALSE, ("%s(), rec_util_get_shared_data() failed!\r\n", __func__));
                return SVCTXR_FAIL;
            }
            pt_media_desc->t_rec_tick_ctrl_blk = t_rec_share_data.t_tick_cb;
            pt_media_desc->ps_media_path = (CHAR*)x_mem_alloc(x_strlen(t_rec_share_data.ps_strg_path) + 1);
            if (pt_media_desc->ps_media_path == NULL)
            {
                x_mem_free((VOID*)pt_media_desc->ps_media_path);
                return SVCTXR_NO_RESOURCE;
            }
            x_strcpy(pt_media_desc->ps_media_path, t_rec_share_data.ps_strg_path);

            pt_media_desc->pf_nfy(pt_media_desc->h_svctx,
                                  SVCTX_COND_PENDING,
                                  SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,
                                  ST_UNKNOWN,
                                  pt_media_desc->pv_tag,
                                  (VOID*)pt_media_desc->h_media_desc);
        }
        else if (pt_rec_msg->uinfo.t_cond_chg.e_cond_new == REC_UTIL_COND_TRANSITION)
        {
            DBG_INFO((_INFO_HEADER"%s(), REC_UTIL_COND_TRANSITION\r\n", __func__));
            if (!pt_svctx->b_in_stopping)
            {
                mm_hdlr_stop_svc(pt_svctx,TRUE);
            }
        }
        break;
    case REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED:
        {
            MM_REC_BUF_RANGE_INFO_T t_range_info;
            UINT32 ui4_range_size = 0;
            PVR_TICK_INDEX_T t_tick_idx_valid_start = {0};
            PVR_TICK_INDEX_T t_tick_idx_valid_end = {0};
            PVR_TICK_INDEX_T t_tick_idx_valid_curr = {0};
            UINT32 ui4_tick_num_valid_start = 0;
            UINT32 ui4_tick_num_valid_end = 0;
            UINT32 ui4_tick_num_safe_start = 0;
            UINT32 ui4_tick_num_safe_end = 0;

            if (pt_running_svc_req->pt_player->e_sess_cond != SM_SESS_COND_STARTED)
            {
                return SVCTXR_WRONG_STATE;
            }

            t_range_info.t_valid = pt_rec_msg->uinfo.t_range.t_valid_range;

            i4_ret = rec_util_get_valid_range(pt_media_desc->h_rec_util,
                                              &t_range_info.t_valid);
            if (i4_ret != REC_UTILR_OK)
            {
                ASSERT(FALSE, ("%s(L%d), rec_util_get_valid_range() failed, ret=%d", __func__, __LINE__, i4_ret));
            }

            i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                             t_range_info.t_valid.ui4_start,
                                             &t_tick_idx_valid_start);
            if (i4_ret != REC_UTILR_OK)
            {
                ASSERT(FALSE, ("%s(L%d), rec_util_get_tick_index() failed, ret=%d", __func__, __LINE__, i4_ret));
            }
            ui4_tick_num_valid_start = t_tick_idx_valid_start.ui4_tick_num;

            i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                             t_range_info.t_valid.ui4_end,
                                             &t_tick_idx_valid_end);
            if (i4_ret != REC_UTILR_OK)
            {
                ASSERT(FALSE, ("%s(L%d), rec_util_get_tick_index() failed, ret=%d", __func__, __LINE__, i4_ret));
            }
            ui4_tick_num_valid_end = t_tick_idx_valid_end.ui4_tick_num;

            if ((ui4_tick_num_valid_end - ui4_tick_num_valid_start + 1) < (MM_REC_BUF_SAFE_RANGE_START_THRESHOLD - 1))
            {
                return SVCTXR_OK;
            }

            /*ASSERT(t_range_info.t_valid.ui4_start ==
                   PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk, t_tick_idx_valid_start.ui4_tick_num),
                   ("%s(), invalid tick number!\r\n", __func__));
            ASSERT(t_range_info.t_valid.ui4_end ==
                   PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk, t_tick_idx_valid_end.ui4_tick_num),
                   ("%s(), invalid tick number!\r\n", __func__));*/
            ASSERT(t_tick_idx_valid_start.ui4_tick_num <= t_tick_idx_valid_end.ui4_tick_num, ("%s(), invalid tick number!\r\n", __func__));
            ui4_range_size = ui4_tick_num_valid_end - ui4_tick_num_valid_start + 1;

            if (ui4_range_size <=
                (MM_REC_BUF_SAFE_RANGE_START_THRESHOLD + MM_REC_BUF_SAFE_RANGE_END_THRESHOLD + 2))
            {
                ui4_tick_num_safe_start = t_tick_idx_valid_start.ui4_tick_num;
                ui4_tick_num_safe_end = t_tick_idx_valid_end.ui4_tick_num;
                t_range_info.t_safe = t_range_info.t_valid;
            }
            else
            {
                ui4_tick_num_safe_start = t_tick_idx_valid_start.ui4_tick_num + MM_REC_BUF_SAFE_RANGE_START_THRESHOLD;
                ui4_tick_num_safe_end = t_tick_idx_valid_end.ui4_tick_num - MM_REC_BUF_SAFE_RANGE_END_THRESHOLD;

                t_range_info.t_safe.ui4_start = PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk,
                                                                             ui4_tick_num_safe_start);
                t_range_info.t_safe.ui4_end = PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk,
                                                                           ui4_tick_num_safe_end);
            }

            i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_REC_BUF_RANGE,
                              (void*)(UINT32)&t_range_info,
                              sizeof(MM_REC_BUF_RANGE_INFO_T));
            if (i4_ret!= SMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"update valid range failed, ret=%d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
            pt_media_desc->t_valid_range = t_range_info.t_valid;
            pt_media_desc->t_safe_range = t_range_info.t_safe;

            DBG_INFO((_INFO_HEADER"tshift range (%u/%u/%u/%u) updated!\r\n",
                      pt_media_desc->t_valid_range.ui4_start,
                      pt_media_desc->t_safe_range.ui4_start,
                      pt_media_desc->t_safe_range.ui4_end,
                      pt_media_desc->t_valid_range.ui4_end));

            pt_media_desc->ui4_tick_num_valid_start = ui4_tick_num_valid_start;
            pt_media_desc->ui4_tick_num_valid_end = ui4_tick_num_valid_end;
            pt_media_desc->ui4_tick_num_safe_start = ui4_tick_num_safe_start;
            pt_media_desc->ui4_tick_num_safe_end = ui4_tick_num_safe_end;

            i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_curr_lba, &ui4_curr_tick);
            if (i4_ret!= SVCTXR_OK)
            {
                return SVCTXR_FAIL;
            }
            i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                             ui4_curr_tick,
                                             &t_tick_idx_valid_curr);
            if (i4_ret != REC_UTILR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s(), rec_util_get_tick_index() failed, ret=%d\r\n", __func__, i4_ret));
                return SVCTXR_FAIL;
            }

            if (t_tick_idx_valid_curr.ui4_tick_num <= ui4_tick_num_safe_start &&
                ui4_range_size >= (pt_media_desc->t_rec_tick_ctrl_blk.ui4_max_vld_entry_num - MM_REC_BUF_SAFE_RANGE_START_THRESHOLD))
            {
                if (pt_media_desc->e_speed < MM_SPEED_TYPE_FORWARD_1X)
                {
                    t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                    i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                    if (i4_ret != SVCTXR_OK)
                    {
                        DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK failed, ret=%d\r\n", i4_ret));
                    }
                }
            }

            if (pt_rec_msg->uinfo.t_range.b_info_updated &&
                pt_media_desc->ui4_rec_mntr_info_mask == 0)
            {

                i4_ret = _mm_hdlr_handle_rec_info_updated(pt_svctx, ui4_curr_lba);
                if (i4_ret!= SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"update rec info failed, ret=%d\r\n", i4_ret));
                    return SVCTXR_FAIL;
                }
            }
        }
        break;
    case REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED:
        if (pt_running_svc_req->pt_player->e_sess_cond != SM_SESS_COND_STARTED)
        {
            return SVCTXR_WRONG_STATE;
        }

        if (pt_media_desc->ui4_rec_mntr_info_mask == 0)
        {
            i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_curr_lba, &ui4_curr_tick);
            if (i4_ret!= SVCTXR_OK)
            {
                return SVCTXR_FAIL;
            }

            i4_ret = _mm_hdlr_handle_rec_info_updated(pt_svctx, ui4_curr_lba);
            if (i4_ret!= SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"update rec info failed, ret=%d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
        }
        break;
    case REC_UTIL_NTFY_CODE_RESET_REQ:
        x_dbg_stmt(_INFO_HEADER"%s(), REC_UTIL_NTFY_CODE_RESET_REQ\r\n", __func__);
        pt_media_desc->ui4_rec_util_reset_req_id = pt_rec_msg->uinfo.ui4_data;
        if (!pt_svctx->b_in_stopping)
        {
            mm_hdlr_stop_svc(pt_svctx,TRUE);
        }
        break;
    default:
        break;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_crnt_evn_retrieve_rating
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _mm_hdlr_crnt_evn_retrieve_rating(
                    MEDIA_DESCRIBT_T*           pt_media_desc,
                    CRNT_EVENT_T*               pt_this
                    )
{
    SIZE_T       z_len  = 0;
    INT32        i4_ret;

    pt_this->pv_crnt_evn_rating     = NULL;
    pt_this->z_crnt_evn_rating_size = 0;

    /*x_dbg_stmt(_INFO_HEADER"%s(), ui4_rec_mntr_lba_last=0x%.8X\r\n",
               __func__, pt_media_desc->ui4_rec_mntr_lba_last);*/

    i4_ret = rec_util_get_event_info_len(pt_media_desc->h_rec_util,
                                         pt_media_desc->ui4_rec_mntr_lba_last,
#ifdef MW_EDB_SUPPORT
                                         EDB_KEY_TYPE_RATING_LIST_BY_ID,
#else                    
                                         EVCTX_KEY_TYPE_RATING,
#endif  
                                         &z_len);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"rec_util_get_event_info_len(EVCTX_KEY_TYPE_RATING) failed, ret=%d\r\n",
                   i4_ret
                   ));
        return;
    }

    pt_this->z_crnt_evn_rating_size = z_len;

    DBG_INFO((_INFO_HEADER"crnt rating need buf size=%d, crnt buf size=%d\r\n",
              z_len,
              pt_this->z_evn_rating_buf_size
              ));
    /* if needed event rating buffer size > current event rating
     * buffer size or current buffer is empty, free old one if not empty,
     * create a new one and store the buffer size.
     */
    if (z_len > pt_this->z_evn_rating_buf_size || pt_this->pv_evn_rating_buf == NULL) {
        if (pt_this->pv_evn_rating_buf != NULL) {
            x_mem_free(pt_this->pv_evn_rating_buf);
            pt_this->pv_evn_rating_buf = NULL;
            DBG_INFO((_INFO_HEADER"free memory(size=%d) for event rating\r\n",
                      pt_this->z_evn_rating_buf_size
                      ));
        }

        pt_this->pv_evn_rating_buf = (VOID*) x_mem_alloc(z_len);
        if (pt_this->pv_evn_rating_buf == NULL) {
            pt_this->z_evn_rating_buf_size = 0;
            DBG_ERROR((_ERROR_HEADER"x_mem_alloc() failed\r\n"));
            return;
        }
        pt_this->z_evn_rating_buf_size = z_len;
        DBG_INFO((_INFO_HEADER"alloc memory(size=%d) for event rating\r\n", z_len));
    }

    i4_ret = rec_util_get_event_info(pt_media_desc->h_rec_util,
                                     pt_media_desc->ui4_rec_mntr_lba_last,
#ifndef MW_EDB_SUPPORT
                                     EVCTX_KEY_TYPE_RATING,
#else
                                     EDB_KEY_TYPE_RATING_LIST_BY_ID,        
#endif
                                     &z_len,
                                     pt_this->pv_evn_rating_buf);
    if (i4_ret != REC_UTILR_OK) {
        pt_this->z_crnt_evn_rating_size = 0;
        pt_this->pv_crnt_evn_rating     = NULL;
        DBG_ERROR((_ERROR_HEADER"rec_util_get_event_info() failed, ret=%d\r\n", i4_ret));
        return;
    } else {
        pt_this->pv_crnt_evn_rating     = pt_this->pv_evn_rating_buf;
    }
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_same_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/

static BOOL _mm_hdlr_same_strm_req(
    STRM_REQ_T*         pt_strm_req_1,
    STRM_REQ_T*         pt_strm_req_2
)
{
    BOOL    b_ret = FALSE;

    if ((STRM_REQ_SCDB == pt_strm_req_1->e_req_type) && (STRM_REQ_SCDB == pt_strm_req_2->e_req_type))
    {
        SCDB_REC_T*    pt_rec_1 = &(pt_strm_req_1->u.t_scdb.t_rec);
        SCDB_REC_T*    pt_rec_2 = &(pt_strm_req_2->u.t_scdb.t_rec);

        if (pt_rec_1->e_rec_type == pt_rec_2->e_rec_type)
        {
            switch (pt_rec_1->e_rec_type)
            {
                case SCDB_REC_TYPE_AUDIO_MPEG:
                    if ((pt_rec_1->u.t_audio_mpeg.ui2_pid   == pt_rec_2->u.t_audio_mpeg.ui2_pid) &&
                        (pt_rec_1->u.t_audio_mpeg.ui1_index == pt_rec_2->u.t_audio_mpeg.ui1_index))
                    {
                        b_ret = TRUE;
                    }
                    break;

                case SCDB_REC_TYPE_VIDEO_MPEG:
                    if (pt_rec_1->u.t_video_mpeg.ui2_pid == pt_rec_2->u.t_video_mpeg.ui2_pid)
                    {
                        b_ret = TRUE;
                    }
                    break;

                default:
                    b_ret = TRUE;
                    break;
            }
        }
    }
    else if ((STRM_REQ_PID == pt_strm_req_1->e_req_type) && (STRM_REQ_PID == pt_strm_req_2->e_req_type))
    {
        if (pt_strm_req_1->u.t_pid.t_mpeg_2_pid == pt_strm_req_2->u.t_pid.t_mpeg_2_pid)
        {
            b_ret = TRUE;
        }
    }
    else if (((STRM_REQ_SCDB == pt_strm_req_1->e_req_type) && (STRM_REQ_PID == pt_strm_req_2->e_req_type)) ||
             ((STRM_REQ_PID == pt_strm_req_1->e_req_type) && (STRM_REQ_SCDB == pt_strm_req_2->e_req_type)))
    {
        SCDB_STRM_REQ_T*    pt_scdb = NULL;
        PID_STRM_REQ_T*     pt_pid = NULL;

        if (STRM_REQ_SCDB == pt_strm_req_1->e_req_type)
        {
            pt_scdb = &pt_strm_req_1->u.t_scdb;
            pt_pid = &pt_strm_req_2->u.t_pid;
        }
        else
        {
            pt_pid = &pt_strm_req_1->u.t_pid;
            pt_scdb = &pt_strm_req_2->u.t_scdb;
        }

        if (pt_scdb->t_comp_id.e_type == pt_pid->e_strm_type)
        {
            switch (pt_scdb->t_comp_id.e_type)
            {
                case ST_AUDIO:
                    if ((pt_scdb->t_rec.u.t_audio_mpeg.ui2_pid == pt_pid->t_mpeg_2_pid) &&
                        (pt_scdb->t_rec.u.t_audio_mpeg.e_enc == pt_pid->u.t_aud_stream.e_aud_enc))
                    {
                        b_ret = TRUE;
                    }
                    break;

                case ST_VIDEO:
                    if ((pt_scdb->t_rec.u.t_video_mpeg.ui2_pid == pt_pid->t_mpeg_2_pid) &&
                        (pt_scdb->t_rec.u.t_video_mpeg.e_enc == pt_pid->u.t_vid_stream.e_vid_enc))
                    {
                        b_ret = TRUE;
                    }
                    break;

                default:
                    b_ret = TRUE;
                    break;
            }
        }
    }

    return (b_ret);
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_replay_audio
 * Description: Just to restart audio stream to do AV sync
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_replay_audio (
                    SVCTX_T*                    pt_svctx,
                    MEDIA_DESCRIBT_T*           pt_media_desc
                    )
{
    INT32               i4_ret = SVCTXR_OK;
    MM_SVC_REQ_T*       pt_running_svc_req = NULL;
    
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
    {
        i4_ret = x_sm_stop_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"x_sm_stop_stream(%d) failed, ret = %d)\r\n", ST_AUDIO, i4_ret));
            return SVCTXR_FAIL;
        }
        pt_media_desc->b_set_audio_stop = TRUE;
        
        i4_ret = x_sm_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_sm_play_stream(%d) failed, ret = %d)\r\n", ST_AUDIO, i4_ret);
            return SVCTXR_FAIL;
        }
        pt_media_desc->b_set_audio_stop = FALSE;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_pause_audio
 * Description: Just to pause audio stream to do AV sync
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_pause_audio (
                    SVCTX_T*                    pt_svctx,
                    MEDIA_DESCRIBT_T*           pt_media_desc
                    )
{
    INT32               i4_ret = SVCTXR_OK;
    MM_SVC_REQ_T*       pt_running_svc_req = NULL;
    
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
    {
        i4_ret = mm_hdlr_strm_trick_req(pt_svctx, ST_AUDIO, MM_SPEED_TYPE_PAUSE, FALSE);       
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_swdmx_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_swdmx_msg (
                    SVCTX_T*                    pt_svctx,
                    MEDIA_DESCRIBT_T*           pt_media_desc,
                    SVCTX_GENERIC_MSG_T*        pt_generic
                    )
{
    MM_RANGE_COND_T     e_nfy_cond = (MM_RANGE_COND_T)pt_generic->ui4_cond;
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32               i4_ret = SVCTXR_OK;
    MM_SVC_TRICK_INFO_T t_trick_info;
#if 0
    MM_RANGE_POS_INFO_T t_position_info;
#endif
    UINT32              ui4_rec_mntr_lba = 0;
    MM_PLAY_MODE_PL_T  t_mm_play_mode;

    if (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
		!MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
    {
        return SVCTXR_OK;
    }

    if(pt_svctx->b_in_stopping)
    {
        return SVCTXR_OK;
    }

    if (e_nfy_cond == MM_MONITOR_LBA_ARRIVAL)
    {
        DBG_INFO((_INFO_HEADER"%s(MONITOR_LBA_ARRIVAL), lba=0x%.8X\r\n",
                  __func__, pt_media_desc->ui4_rec_mntr_lba));

        pt_media_desc->ui4_rec_mntr_lba_last = pt_media_desc->ui4_rec_mntr_lba;

        if (pt_media_desc->ui4_rec_mntr_info_mask & REC_UTIL_INFO_MASK_RATING)
        {
            _mm_hdlr_crnt_evn_retrieve_rating(pt_media_desc, pt_running_svc_req->pt_crnt_event);
            i4_ret = mm_hdlr_check_svc_block(pt_svctx);
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"mm_hdlr_check_svc_block() failed, ret=%d\r\n", i4_ret));
            }
        }
        if (pt_media_desc->ui4_rec_mntr_info_mask & REC_UTIL_INFO_MASK_EVENT_INFO)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CRNT_EVN_READY,
                        ST_UNKNOWN
                        );
        }
        if (pt_media_desc->ui4_rec_mntr_info_mask & REC_UTIL_INFO_MASK_SCDB_UPDATE)
        {
            UINT16         ui2_scdb_rec_num = 10;
            UINT16         ui2_scdb_rec_num_tmp = 0;/*to fix warning*/
            SCDB_REC_T     at_scdb_recs_const[10];
            SCDB_REC_T*    pt_scdb_rec = NULL;
            UINT8          ui1_idx;
            STRM_REQ_T*    pt_new_strm_req = NULL;
            STRM_REQ_T*    pt_old_strm_req = NULL;

            x_memset(at_scdb_recs_const, 0, 10*sizeof(SCDB_REC_T));
            
            if ((pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X)
                 || (pt_media_desc->e_speed >= MM_SPEED_TYPE_FORWARD_2X))
            {
                /* get SCDB info */
                SCDB_REC_T* at_scdb_recs = NULL;
                i4_ret = rec_util_get_scdb_info_rec_num(
                                                pt_media_desc->h_rec_util,
                                                pt_media_desc->ui4_rec_mntr_lba,
                                                &ui2_scdb_rec_num);
                if(ui2_scdb_rec_num > 10)
                {
                    at_scdb_recs = (SCDB_REC_T*)x_mem_alloc(ui2_scdb_rec_num * sizeof(SCDB_REC_T));
                }
                else
                {
                    at_scdb_recs = at_scdb_recs_const;
                }
                i4_ret = rec_util_get_scdb_info(pt_media_desc->h_rec_util,
                                                pt_media_desc->ui4_rec_mntr_lba,
                                                &ui2_scdb_rec_num_tmp,
                                                at_scdb_recs);

                if (i4_ret != REC_UTILR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"get scdb info fail!\r\n"));
                    if(at_scdb_recs != at_scdb_recs_const)
                    {
                        x_mem_free(at_scdb_recs);
                    }
                    return SVCTXR_FAIL;
                }

                for (ui1_idx = 0 ; ui1_idx < (UINT8)(ui2_scdb_rec_num) ; ui1_idx++)
                {
                    pt_scdb_rec = &(at_scdb_recs[ui1_idx]);

                    switch(pt_scdb_rec->e_rec_type)
                    {
                        case SCDB_REC_TYPE_VIDEO_MPEG:
                            DBG_INFO((_INFO_HEADER"SCDB_REC_TYPE_VIDEO_MPEG [PID %d] [LBA %d]\r\n", pt_scdb_rec->u.t_video_mpeg.ui2_pid, pt_media_desc->ui4_rec_mntr_lba));
                            break;

                        case SCDB_REC_TYPE_AUDIO_MPEG:
                            DBG_INFO((_INFO_HEADER"SCDB_REC_TYPE_AUDIO_MPEG [PID %d] [LBA %d]\r\n", pt_scdb_rec->u.t_audio_mpeg.ui2_pid, pt_media_desc->ui4_rec_mntr_lba));
                            break;

                        default:
                            DBG_INFO((_INFO_HEADER"SCDB_REC_TYPE_OTHER [LBA %d]\r\n", pt_media_desc->ui4_rec_mntr_lba));
                            ;
                    }

                    if (pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG)
                    {
                        pt_new_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
                        if (pt_new_strm_req == NULL)
                        {
                            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                            if(at_scdb_recs != at_scdb_recs_const)
                            {
                                x_mem_free(at_scdb_recs);
                            }
                            return SVCTXR_NO_RESOURCE;
                        }

                        pt_old_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

                        pt_new_strm_req->h_svctx              = pt_media_desc->h_svctx;
                        pt_new_strm_req->pf_strm_nfy          = NULL;
                        pt_new_strm_req->pv_strm_nfy_tag      = NULL;
                        pt_new_strm_req->e_req_type           = STRM_REQ_PID;
                        pt_new_strm_req->u.t_pid.e_strm_type  = ST_VIDEO;
                        pt_new_strm_req->u.t_pid.t_mpeg_2_pid = pt_scdb_rec->u.t_video_mpeg.ui2_pid;

                        if (_mm_hdlr_same_strm_req(pt_old_strm_req, pt_new_strm_req) == FALSE)
                        {
                            DBG_INFO((_INFO_HEADER"%s(), Select stream using force PID = %d\r\n", __func__, pt_scdb_rec->u.t_video_mpeg.ui2_pid));

                            _mm_hdlr_handle_strm_select_req(pt_svctx, pt_running_svc_req, pt_new_strm_req, TRUE);
                        }
                        else
                        {
                            svctx_free_strm_req(pt_new_strm_req);
                        }
                        /* exit the loop */
                        break;
                    }
                }
                if(at_scdb_recs != at_scdb_recs_const)
                {
                    x_mem_free(at_scdb_recs);
                }
            }
        }
        /* set next monitor LBA */
        ui4_rec_mntr_lba = IS_BACKWARD_DIR(pt_media_desc->e_speed) ?
                           (pt_media_desc->ui4_rec_mntr_lba_last - 1) :
                           (pt_media_desc->ui4_rec_mntr_lba_last + 1);
        i4_ret = _mm_hdlr_handle_rec_info_updated(pt_svctx, ui4_rec_mntr_lba);
        if (i4_ret!= SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"update rec info failed, ret=%d\r\n", i4_ret));
        }
    }
    else if (e_nfy_cond == MM_OUT_OF_SAFE_RANGE_START)
    {
#if 0
        if (pt_media_desc->e_speed <= MM_SPEED_TYPE_FORWARD_1X)
        {
            if (pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X)
            {
                /* seek to the safe range start position */
                t_mm_play_mode.eSpeed=MM_SPEED_TYPE_PAUSE;
                t_mm_play_mode.ePlayMode=MM_PLAY_MODE_TYPE_NORMAL;
                
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_SPEED,
                                  (VOID*)&t_mm_play_mode,
                                  sizeof(MM_PLAY_MODE_PL_T));
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"set speed fail!\r\n"));
                    i4_ret =  SVCTXR_FAIL;
                }

                t_position_info.e_pos_type = MM_RANGE_POS_TYPE_TIME_MEDIA;
                t_position_info.ui4_idx = pt_running_svc_req->t_init_pos_info.ui2_idx;
                t_position_info.ui8_pos_val = (UINT64)pt_media_desc->t_safe_range.ui4_start;
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_SEEK,
                                  (VOID*)(((MM_RANGE_POS_INFO_T*)&t_position_info)),
                                  sizeof(MM_RANGE_POS_INFO_T));
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"set seek fail!\r\n"));
                    i4_ret =  SVCTXR_FAIL;
                }

                VSH_SRC_RESOLUTION_INFO_T    t_orig_video_info;

                t_orig_video_info = pt_svctx->t_video_info; /* backup */

                /* set trick mode forcely */
                pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_VIDEO;
                pt_svctx->t_video_info.ui4_width = 0xFFFFFFFF;
                pt_svctx->t_video_info.ui4_height = 0xFFFFFFFF;

                t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                i4_ret = mm_hdlr_set(pt_svctx,
                                     SVCTX_MM_SET_TYPE_TRICK,
                                     (VOID*)&t_trick_info,
                                     sizeof(MM_SVC_TRICK_INFO_T));
                if (i4_ret!= SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK fail!\r\n"));
                }

                pt_svctx->t_video_info = t_orig_video_info; /* restore */
            }
            else
            {
                t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                if (i4_ret != SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK failed, ret=%d\r\n", i4_ret));
                }
            }
        }
#endif
        mm_hdlr_invoke_client_svc_nfy_fct(
                       pt_svctx,
                       SVCTX_NTFY_CODE_REACH_VALID_RANGE_START,
                       ST_UNKNOWN
                       );
    }
    else if (e_nfy_cond == MM_OUT_OF_SAFE_RANGE_END)
    {
#if 0    
        if (pt_media_desc->e_speed > MM_SPEED_TYPE_FORWARD_1X)
        {
            if (pt_media_desc->e_speed >= MM_SPEED_TYPE_FORWARD_2X)
            {
                /* seek to the safe range start position */
                t_mm_play_mode.eSpeed=MM_SPEED_TYPE_PAUSE;
                t_mm_play_mode.ePlayMode=MM_PLAY_MODE_TYPE_NORMAL;
                
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_SPEED,
                                  (VOID*)&t_mm_play_mode,
                                  sizeof(MM_PLAY_MODE_PL_T));
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"set speed fail!\r\n"));
                    i4_ret =  SVCTXR_FAIL;
                }

                t_position_info.e_pos_type = MM_RANGE_POS_TYPE_TIME_MEDIA;
                t_position_info.ui4_idx = pt_running_svc_req->t_init_pos_info.ui2_idx;
                t_position_info.ui8_pos_val = (UINT64)pt_media_desc->t_safe_range.ui4_end;
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_SEEK,
                                  (VOID*)(((MM_RANGE_POS_INFO_T*)&t_position_info)),
                                  sizeof(MM_RANGE_POS_INFO_T));
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"set seek fail!\r\n"));
                    i4_ret =  SVCTXR_FAIL;
                }

                VSH_SRC_RESOLUTION_INFO_T    t_orig_video_info;

                t_orig_video_info = pt_svctx->t_video_info; /* backup */

                /* set trick mode forcely */
                pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_VIDEO;
                pt_svctx->t_video_info.ui4_width = 0xFFFFFFFF;
                pt_svctx->t_video_info.ui4_height = 0xFFFFFFFF;

                t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                i4_ret = mm_hdlr_set(pt_svctx,
                                     SVCTX_MM_SET_TYPE_TRICK,
                                     (VOID*)&t_trick_info,
                                     sizeof(MM_SVC_TRICK_INFO_T));
                if (i4_ret!= SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK fail!\r\n"));
                }

                pt_svctx->t_video_info = t_orig_video_info; /* restore */
            }
            else
            {
                t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                if (i4_ret != SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK failed, ret=%d\r\n", i4_ret));
                }
            }
        }
#endif
        mm_hdlr_invoke_client_svc_nfy_fct(
                       pt_svctx,
                       SVCTX_NTFY_CODE_REACH_VALID_RANGE_END,
                       ST_UNKNOWN
                       );
    }
    else if (e_nfy_cond == MM_REACH_VALID_RANGE_START)
    {
        /*Empty for: if Cur<VS, it will send MM_OUT_OF_SAFE_RANGE_START & MM_REACH_VALID_RANGE_START*/
    }
    else if (e_nfy_cond == MM_REACH_VALID_RANGE_END)
    {
        if (pt_media_desc->e_speed > MM_SPEED_TYPE_FORWARD_1X)
        {
            t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
            i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK failed, ret=%d\r\n", i4_ret));
            }
        }
    }
    else if (e_nfy_cond == MM_SIGNAL_Y_TO_N)
    {
        if(pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            _mm_hdlr_pause_audio(pt_svctx, pt_media_desc);
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream) &&
                pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].e_strm_cond == SM_COND_STARTED)
            {
                i4_ret = x_sm_stop_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                {
                    return SVCTXR_FAIL;
                }
            }
        }
        /*For h264.otherwise there will be garbage DTV00353348*/
        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream,
                              SM_VSH_SET_TYPE_VDEC_REPLAY,
                              NULL,
                              0);
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"Set replay video decode fail %d\n",i4_ret);
            }

        }
        _mm_hdlr_set_super_freeze_mode_off();/*turn off super freeze when no signal*/
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                          SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS,
                                          ST_UNKNOWN);
        mm_hdlr_check_signal_loss_priority(
                    pt_svctx,
                    pt_running_svc_req,
                    TRUE
                    );
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    }
    else if (e_nfy_cond == MM_SIGNAL_N_TO_Y)
    {
        if(pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            _mm_hdlr_replay_audio(pt_svctx, pt_media_desc);
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream) &&
                pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].e_strm_cond != SM_COND_STARTED)
            {
                i4_ret = x_sm_play_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_PLAYED)
                {
                    return SVCTXR_FAIL;
                }
            }
        }
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED, ST_UNKNOWN);
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY, ST_UNKNOWN);
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
        _mm_hdlr_check_audio_video_fmt(pt_svctx, pt_running_svc_req);
    }
    else if (e_nfy_cond == MM_BAD_TICK_Y_TO_N)
    {
        /* AP get bad tick information by polling */
        if(pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            _mm_hdlr_replay_audio(pt_svctx, pt_media_desc);
        }
        pt_media_desc->b_bad_tick = FALSE;
    }
    else if (e_nfy_cond == MM_BAD_TICK_N_TO_Y)
    {
        /* AP get bad tick information by polling */
        pt_media_desc->b_bad_tick = TRUE;
    }
    else if (e_nfy_cond == MM_RANGE_SEEK_DONE)
    {
    }
    if( (e_nfy_cond == MM_MONITOR_LBA_ARRIVAL)
        ||(e_nfy_cond == MM_SIGNAL_N_TO_Y)
        ||(e_nfy_cond == MM_BAD_TICK_N_TO_Y)
        ||(e_nfy_cond == MM_RANGE_SEEK_DONE))
    {
        i4_ret = mm_hdlr_get_current_event(pt_svctx);
    }

    return i4_ret;
}
#endif /* TIME_SHIFT_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_range_playback_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_range_playback_nfy_fct (
                                       VOID*            pv_nfy_tag,
                                       MM_RANGE_COND_T  e_nfy_cond,
                                       UINT32           ui4_data_1,
                                       UINT32           ui4_data_2)
{
    INT32                           i4_ret;
    MEDIA_DESCRIBT_T*               pt_media_desc;
    SVCTX_T*                        pt_svctx;
    HANDLE_T                        h_media_desc;

    h_media_desc = (HANDLE_T)pv_nfy_tag;
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_INFO(("mm_hdlr : range_nfy return fail pre1!\r\n"));
        return i4_ret;
    }

    i4_ret = svctx_get_obj( pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_INFO(("mm_hdlr : range_nfy return fail pre2!\r\n"));
        return i4_ret;
    }
    i4_ret = _mm_hdlr_send_msg(pt_media_desc->h_svctx,
                               h_media_desc,
                               _SVCTX_MSG_RANGE_CB_NTFY,
                               NULL,
                               (UINT32)e_nfy_cond,
                               ui4_data_1,
                               ui4_data_2);
    return (i4_ret==SVCTXR_OK);
}

#ifndef DRV_NO_PRE_PARSING
static VOID _mm_hdlr_pre_parse_timeout_callback(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    INT32                         i4_ret;
    MEDIA_DESCRIBT_T*             pt_media_desc;
    MM_SVC_UOP_HANDLER_FCT_TBL_T  t_fct_tbl;
    SVCTX_T*                      pt_svctx = (SVCTX_T*)pv_tag;
    MM_SVC_REQ_T*                 pt_running_svc_req = NULL;

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return ;
    }
    if (SVCTXR_OK != mm_hdlr_find_handler(&pt_media_desc->t_media_fmt, &t_fct_tbl))
    {
        return ;
    }
    pt_running_svc_req->t_uop_fct_tbl = t_fct_tbl;
    gb_pre_parsing = FALSE;
}
#endif

static INT32 _mm_hdlr_do_buf_stat_ctrl(MEDIA_DESCRIBT_T*   pt_media_desc,
                                            BOOL                b_start)
{
    INT32                           i4_ret;
    SVCTX_T*                        pt_svctx;
    MM_SVC_REQ_T*                   pt_running_svc_req = NULL;

    if(!pt_media_desc)
    {
        return SVCTXR_INV_ARG;
    }

    i4_ret = svctx_get_obj( pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_do_buf_stat_ctrl return fail pre2! line = %d\n", __LINE__);
        return i4_ret;
    }

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    
    if(!pt_running_svc_req ||
       !pt_running_svc_req->pt_player ||
       !x_handle_valid(pt_running_svc_req->pt_player->h_sess) ||
       (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) &&
        !x_handle_valid(pt_running_svc_req->pt_player->h_sess)))
    {
        return SVCTXR_INV_ARG;
    }

    if(b_start)
    {
        /*start stc*/
        i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_STC_CTRL,
                          (VOID*)(UINT32)1,
                          sizeof(UINT32));
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"set stc ctrl fail! line = %d\n", __LINE__);
            i4_ret =  SVCTXR_FAIL;
        }
        /*start video stream*/
        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                           ST_VIDEO,
                                           pt_media_desc->e_speed, 
                                           FALSE);
            
            x_dbg_stmt(_INFO_HEADER"x_sm_play_stream(ST_VIDEO) ret=%d\n",i4_ret);
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_play_stream(%d) failed, ret = %d)\r\n", ST_VIDEO, i4_ret);
                return SVCTXR_FAIL;
            }
            
        }
        /*start audio stream*/
        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) &&
           pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                           ST_AUDIO,
                                           pt_media_desc->e_speed, 
                                           FALSE);
            x_dbg_stmt(_INFO_HEADER"x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret);
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_play_stream(%d) failed, ret = %d)\r\n", ST_AUDIO, i4_ret);
                return SVCTXR_FAIL;
            }
        }
    }
    else
    {
        /*stop stc*/
        i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_STC_CTRL,
                          (VOID*)(UINT32)0,
                          sizeof(UINT32));
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"set stc ctrl fail!\r\n");
            i4_ret =  SVCTXR_FAIL;
        }
        /*stop video stream*/
        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                           ST_VIDEO,
                                           MM_SPEED_TYPE_PAUSE, 
                                           FALSE);
            DBG_INFO(("\n[MM_HDLR_API]x_sm_stop_stream(ST_VIDEO) ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"x_sm_stop_stream(%d) failed, ret = %d)\r\n", ST_VIDEO, i4_ret));
                return SVCTXR_FAIL;
            }
        }
        /*stop audio stream*/
        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) &&
           pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                           ST_AUDIO,
                                           MM_SPEED_TYPE_PAUSE, 
                                           FALSE);
            DBG_INFO(("\n[MM_HDLR_API]x_sm_stop_stream(ST_AUDIO) ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"x_sm_stop_stream(%d) failed, ret = %d)\r\n", ST_AUDIO, i4_ret));
                return SVCTXR_FAIL;
            }
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_support_strm_num
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_get_support_strm_num(
                                    MEDIA_DESCRIBT_T*   pt_media_desc,
                                    STREAM_TYPE_T        e_type,
                                    UINT16*                pui2_num
                                    )
{
    HANDLE_T                            h_minfo;
    UINT16                              ui2_video_track_cnt = 0;
    UINT16                              ui2_audio_track_cnt = 0;
    MINFO_INFO_T                        t_minfo_rec;
    UINT16                              ui4_vid_rec_nums  = 0;
    UINT16                              ui4_aud_rec_nums  = 0;
    UINT16                              ui4_sp_rec_nums   = 0;
    INT32                               i4_ret = 0;
    UINT32                              ui4_sz = 0;
    UINT16                              ui2_i  = 0;
    SVCTX_T*                            pt_svctx = NULL;
    SIZE_T                              z_rec_length = 0;
    MINFO_INFO_T                        t_minfo_gen_rec;
    
    /*check parameters*/
    if (pt_media_desc == NULL || pui2_num == NULL)
    {
        return SVCTXR_INV_ARG; 
    }
    *pui2_num = 0;
    
    h_minfo = pt_media_desc->h_media_info;
    i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    
    do
    {
    i4_ret = x_minfo_get_num_recs(h_minfo,
                                  MINFO_INFO_TYPE_VIDEO_ATTR,
                                  &ui4_vid_rec_nums);
        if (i4_ret == MINFOR_INFO_NOT_READY)
    {
            x_thread_delay(100);
    }
    }while(i4_ret == MINFOR_INFO_NOT_READY);
    
    if (e_type == ST_VIDEO)
    {    
        i4_ret = x_minfo_get_num_recs(h_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      &ui4_vid_rec_nums);
        ui4_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo,
                                  MINFO_INFO_TYPE_GENERIC,
                                  1,
                                  &t_minfo_gen_rec,
                                  (SIZE_T*)&ui4_sz);
        if (i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        
        ui2_video_track_cnt = ui4_vid_rec_nums;
        for (ui2_i = 0; ui2_i < (UINT16)ui4_vid_rec_nums; ui2_i++)
        {
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      (UINT16)(ui2_i + 1),
                                      &t_minfo_rec,
                                      &z_rec_length );
            if ( i4_ret != MINFOR_OK)
            {
                return  SVCTXR_NO_RECORD;
            }
            if (!(t_minfo_rec.u.t_stm_attribute.b_is_codec_support))
            {
                if (ui2_video_track_cnt > 0)
                {
                    ui2_video_track_cnt--;
                }
            }
#ifdef DIVX_HT_2_2_CER
            /*HD Support checking*/
            else if (t_minfo_gen_rec.u.t_general_info.b_hd_content && ui2_video_track_cnt!=0)
            {
                
                if (ui2_video_track_cnt > 0)
                {
                    ui2_video_track_cnt--;
                }
            }
#endif
        }
        *pui2_num = ui2_video_track_cnt;
    }
    else if (e_type == ST_AUDIO)
    {
        x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR, &ui4_aud_rec_nums);
        ui2_audio_track_cnt = ui4_aud_rec_nums;
        for (ui2_i = 0; ui2_i < (UINT16)ui4_aud_rec_nums; ui2_i++)
        {
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_AUDIO_ATTR,
                                      (UINT16)(ui2_i + 1),
                                      &t_minfo_rec,
                                      &z_rec_length);
            if (i4_ret != MINFOR_OK)
            {
                return    SVCTXR_NO_RECORD;
            }
            if (!(t_minfo_rec.u.t_stm_attribute.b_is_codec_support))
            {
                /*remove this record*/
                ui2_audio_track_cnt--;
            }
        }
        *pui2_num = ui2_audio_track_cnt;
    }
    else if (e_type == ST_SUBTITLE || e_type == ST_MM_SUBTITLE)
    {
        x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_SP_ATTR, &ui4_sp_rec_nums);
        for (ui2_i = 0; ui2_i < ui4_sp_rec_nums; ui2_i++)
        {
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_SP_ATTR,(UINT16)(ui2_i+1),
                                      &t_minfo_rec,
                                      &z_rec_length);
            if ( i4_ret != MINFOR_OK)
            {
                return  SVCTXR_NO_RECORD;
            }
        }
        if((ui4_sp_rec_nums == 0) &&pt_media_desc->b_is_lyric)
        {
            ui4_sp_rec_nums = 1;
        }
        *pui2_num = ui4_sp_rec_nums;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_do_range_callback
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_do_range_callback (
                                       VOID*            pv_nfy_tag,
                                       MM_RANGE_COND_T  e_nfy_cond,
                                       UINT32           ui4_data_1,
                                       UINT32           ui4_data_2)
{
    UINT32                          ui4_rang_id;
    INT32                           i4_ret = 0;
    SIZE_T                          z_size = 0;
    MEDIA_DESCRIBT_T*               pt_media_desc = NULL;
    SVCTX_T*                        pt_svctx = NULL;
    HANDLE_T                        h_media_desc;
    MM_PRESENT_RANGE_LIST_T*        pt_range_lst     = NULL;
    MM_PRESENT_RANGE_LIST_T*        pt_pre_range_lst = NULL;
    MM_PRESENT_RANGE_LIST_T*        pt_range_lst_tail= NULL;
    MM_SVC_REQ_T*                   pt_running_svc_req = NULL;
    INT8                            i1_idx = 0;

    if (pv_nfy_tag == NULL)
    {
        return FALSE;
    }

    ui4_rang_id = ui4_data_1;

    h_media_desc = (HANDLE_T)pv_nfy_tag;
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_INFO_HEADER "_mm_hdlr_get_media_obj() fail! driver callback error line = %d\n",__LINE__);
        return i4_ret;
    }

    i4_ret = svctx_get_obj( pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_INFO_HEADER "svctx_get_obj() fail! callback error line = %d\n",__LINE__);
        return i4_ret;
    }

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    pt_range_lst = pt_media_desc->pv_presnt_range_lst;
    do
    {
         if (pt_range_lst == NULL)
         {
           /*empty list*/
           if (e_nfy_cond == MM_RANGE_SEEK_DONE)
           {
               /*seek ready*/
               mm_hdlr_invoke_client_svc_nfy_fct(
                                        pt_svctx,
                                        SVCTX_NTFY_CODE_MEDIA_SEEK_READY,
                                        ST_UNKNOWN);

               pt_media_desc->b_is_seeking = FALSE;
            }
           else if(e_nfy_cond == MM_RANGE_SEEK_ERROR || e_nfy_cond == MM_RANGE_SEEK_NO_DATA)
           {
               /*seek error*/
               mm_hdlr_invoke_client_svc_nfy_fct(
                                        pt_svctx,
                                        SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR,
                                        ST_UNKNOWN);
           }
           x_dbg_stmt(_ERROR_HEADER" range_nfy return fail ! line = %d\n",__LINE__);
           return FALSE;
         }
         if (pt_range_lst->t_range.ui4_range_id == ui4_rang_id)
         {
           break;
         }
         pt_pre_range_lst = pt_range_lst;
         pt_range_lst = pt_range_lst->pt_next_range;
    }while(pt_range_lst!= NULL);
    if (pt_range_lst == NULL)
    {
        x_dbg_stmt(_ERROR_HEADER" range_nfy return fail ! line = %d\n",__LINE__);
        return FALSE;
    }

    x_dbg_stmt(" _mm_hdlr_do_range_callback() received msg = %d \n",e_nfy_cond);
    
    if (e_nfy_cond == MM_RANGE_DONE)
    {
      /*assume target range must at the head of range list*/
      /**shift the head to next range of target range.*/
      if (((MM_PRESENT_RANGE_LIST_T*)pt_media_desc->pv_presnt_range_lst)
           ->t_range.u_range.t_es.ui4_end_pts == (UINT32)MIDXBULD_IDX_RANGE_FULL &&
           !pt_media_desc->b_is_dmx_by_idx_tbl)
      {
            /*in case of single full range*/
            pt_media_desc->pv_presnt_range_lst = pt_range_lst->pt_next_range;
            /*free pt_range_lst???? who free it?????*/
            pt_media_desc->ui1_presnt_range_cnts--;
            _mm_hdlr_range_free(pt_range_lst);
            if ((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) &&
                (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) &&
                !((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) && !pt_media_desc->b_is_scdb_updated))
            {
                if (!pt_svctx->b_in_stopping)
                {
                    mm_hdlr_stop_svc(pt_svctx, TRUE);
                    if (pt_media_desc->b_is_thumbnail_playback)
                    {
                        pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;
                        pt_media_desc->b_video_thumbnail_done = TRUE;
                        svctx_timer_stop(pt_svctx);
                    }

                    mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                                      SVCTX_NTFY_CODE_MEDIA_EOS,
                                                      ST_UNKNOWN);
                }
            }

            x_dbg_stmt(_ERROR_HEADER" range_nfy return fail ! line = %d\n",__LINE__);
            return TRUE;
      }

      /*remove range in index builder*/
      /*in case of MP4*/
      if (pt_media_desc->b_is_dmx_by_idx_tbl)
      {
          z_size = sizeof(UINT32);
          i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                       MIDXBULD_SET_INFO_TYPE_FREE_RANGE,
                                       (VOID*)ui4_rang_id,
                                       &z_size);
          if (MIDXBULDR_INV_HANDLE == i4_ret)
          {
             x_dbg_stmt(_ERROR_HEADER"Drvier error , free same range id twice!!!! line = %d\n",__LINE__);
             /*ignore this error*/
             i4_ret = MIDXBULDR_OK;
          }

          if (i4_ret != MIDXBULDR_OK)
          {
              x_dbg_stmt(_ERROR_HEADER"midxbuld free range ERROR! line = %d\n",__LINE__);
              return FALSE;
          }
      }
      /*only remained one range*/
      if (pt_media_desc->ui1_presnt_range_cnts - 1 == 0)
      {
          pt_range_lst_tail = pt_media_desc->pv_presnt_range_lst;
      }
      else
      {
          pt_range_lst_tail = (MM_PRESENT_RANGE_LIST_T*)pt_media_desc->pv_presnt_range_lst;
      }

      for (i1_idx = 0; i1_idx < pt_media_desc->ui1_presnt_range_cnts -1 ;i1_idx++)
      {
          pt_range_lst_tail = pt_range_lst_tail->pt_next_range;
      }

      
          if (((pt_range_lst_tail->t_range.u_range.t_es.ui4_end_pts
                    == (UINT32)pt_media_desc->ui8_media_dur)
                || (pt_range_lst_tail->t_range.u_range.t_es.ui4_end_pts
                   == (UINT32)MIDXBULD_IDX_RANGE_FULL))
               &&(ui4_data_2 == (UINT32)MM_RANGE_SEAMLESS))
          {
              x_dbg_stmt(_ERROR_HEADER"midxbuld free range ERROR! line = %d\n",__LINE__);
              pt_media_desc->pv_presnt_range_lst = pt_range_lst->pt_next_range;
              pt_media_desc->ui1_presnt_range_cnts--;
              _mm_hdlr_range_free(pt_range_lst);
              return TRUE;
          }
          if (ui4_data_2 != (UINT32)MM_RANGE_SEAMLESS)
          {
              pt_range_lst->t_range.u_range.t_es.ui4_start_pts = ui4_data_2;
          }
          else
          {
              if (pt_media_desc->ui1_presnt_range_cnts > 2)
              {
                  x_dbg_stmt(_ERROR_HEADER"midxbuld free range ERROR! line = %d\n",__LINE__);
                  return TRUE;
              }
              pt_range_lst->t_range.u_range.t_es.ui4_start_pts =
                    pt_range_lst_tail->t_range.u_range.t_es.ui4_end_pts + 1;

          }
          pt_range_lst->t_range.u_range.t_es.ui4_end_pts =
          pt_range_lst->t_range.u_range.t_es.ui4_start_pts + 20*90000;

          if (pt_range_lst->t_range.u_range.t_es.ui4_end_pts > (UINT32)pt_media_desc->ui8_media_dur)
          {
              pt_range_lst->t_range.u_range.t_es.ui4_end_pts = (UINT32)pt_media_desc->ui8_media_dur;
          }
          DBG_INFO((_INFO_HEADER"mm_hdlr range_gen = start %u\n\r" ,pt_range_lst->t_range.u_range.t_es.ui4_start_pts));
          DBG_INFO((_INFO_HEADER"mm_hdlr range_gen = end %u\n\r" ,pt_range_lst->t_range.u_range.t_es.ui4_end_pts));

          i4_ret = _mm_hdlr_send_msg(pt_media_desc->h_svctx,
                                     (UINT32)NULL,
                                     _SVCTX_MSG_MRANGE_CTRL_NTFY,
                                     NULL,
                                     MM_RANGE_CTRL_BUILD,
                                     (UINT32)(pt_range_lst),             /*send current node in range list*/
                                     (UINT32)TRUE);                      /*crtl type sepcific data*/
    
      pt_media_desc->pv_presnt_range_lst = pt_range_lst->pt_next_range;
      pt_media_desc->ui1_presnt_range_cnts--;
    }
    else if (e_nfy_cond == MM_RANGE_ABORT)
    {
        pt_media_desc->ui1_presnt_range_cnts--;
        if ( pt_pre_range_lst == NULL)
        {
            pt_media_desc->pv_presnt_range_lst = pt_range_lst->pt_next_range;
        }
        else
        {
            pt_pre_range_lst->pt_next_range = pt_range_lst->pt_next_range;
        }
        _mm_hdlr_range_free(pt_range_lst);
        if (pt_media_desc->b_is_dmx_by_idx_tbl)
        {
            z_size = sizeof(UINT32);

            i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                         MIDXBULD_SET_INFO_TYPE_FREE_RANGE,
                                         (VOID*)ui4_rang_id,
                                         &z_size);

            if (MIDXBULDR_INV_HANDLE == i4_ret)
            {
                 x_dbg_stmt(_ERROR_HEADER"Drvier error , free same range id twice!!!! line = %d\n", __LINE__);
                 /*ignore this error*/
                 i4_ret = MIDXBULDR_OK;
            }

            if (i4_ret != MIDXBULDR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"range ntfy range error! line = %d\n",__LINE__);
                return FALSE;
            }
        }
    }
    else if (e_nfy_cond == MM_RANGE_SEEK_DONE)
    {
        /*seek ready*/
        x_dbg_stmt(_INFO_HEADER"DRIVER SEND MM_RANGE_SEEK_DONE\n");
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_SEEK_READY,
                        ST_UNKNOWN
                        );
        pt_media_desc->b_is_seeking = FALSE;
        if(pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X &&
           pt_media_desc->t_repeat_ab_info.b_seeking)
        {
            MM_SVC_TRICK_INFO_T             t_trick_info;
            PLAYER_T*                       pt_player = NULL;
            MM_PLAY_MODE_PL_T               t_mm_play_mode;

            pt_player          = pt_running_svc_req->pt_player;
            t_mm_play_mode.eSpeed=MM_SPEED_TYPE_PAUSE;
            t_mm_play_mode.ePlayMode=MM_PLAY_MODE_TYPE_NORMAL;
            i4_ret = x_sm_set(pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_SPEED,
                              (VOID*)&t_mm_play_mode,
                              sizeof(MM_PLAY_MODE_PL_T));
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_set_speed(%u) failed, ret = %d line = %d\n", SM_SESS_GNRC_SET_TYPE_SPEED, i4_ret , __LINE__);
                i4_ret =  SVCTXR_FAIL;
            }
            /*trick to 1x*/
            t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
            i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_set_speed(MM_SPEED_TYPE_FORWARD_1X) failed, ret = %d line = %d\n", i4_ret, __LINE__);
                i4_ret =  SVCTXR_FAIL;
            }
            mm_hdlr_invoke_client_svc_nfy_fct(
                 pt_svctx,
                 SVCTX_NTFY_CODE_MEDIA_AB_REPEAT_DONE,
                 ST_UNKNOWN
                 );
        }
        else if(pt_media_desc->b_is_parse_pmt)
        {
            MM_PLAY_MODE_PL_T  t_mm_play_mode;
            t_mm_play_mode.eSpeed=MM_SPEED_TYPE_FORWARD_1X;
            t_mm_play_mode.ePlayMode=MM_PLAY_MODE_TYPE_NORMAL;
            pt_media_desc->b_is_parse_pmt = FALSE;
            i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_SPEED,
                              (VOID*)&t_mm_play_mode,
                              sizeof(MM_PLAY_MODE_PL_T));
        }
        pt_media_desc->t_repeat_ab_info.b_seeking = FALSE;
        if (pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
        {
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream) &&
                x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
            {
                BOOL                b_set_sbtl_info;
                SIZE_T              z_set_info_size;
                
                i4_ret = x_sm_play_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"x_sm_play_stream(ST_SUBTITLE) failed, ret = %d line = %d\n", i4_ret,__LINE__);
                    i4_ret = SVCTXR_FAIL;
                }
                
                b_set_sbtl_info = TRUE;
                z_set_info_size = sizeof(BOOL);

                i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                 SM_MM_SBTL_SET_TYPE_SHOW_HIDE,
                                 &b_set_sbtl_info,
                                 z_set_info_size);
                if (i4_ret!= SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"show subtitle failed, ret = %d line = %d\n", i4_ret,__LINE__);
                    i4_ret = SVCTXR_FAIL;
                }
            }
        }
        if(pt_media_desc->b_is_buff_underflow)
        {
            i4_ret = _mm_hdlr_do_buf_stat_ctrl(pt_media_desc, FALSE);
        }
    }
    else if (e_nfy_cond == MM_RANGE_SEEK_ERROR)
    {
        x_dbg_stmt(_ERROR_HEADER"DRIVER SEND MM_RANGE_SEEK_ERROR\n");
        /*seek error*/
         mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR,
                        ST_UNKNOWN
                        );
         pt_media_desc->t_repeat_ab_info.b_seeking = FALSE;
    }
    else if(e_nfy_cond == MM_RANGE_SEEK_NO_DATA)
    {
        x_dbg_stmt(_ERROR_HEADER"DRIVER SEND MM_RANGE_SEEK_NO_DATA\n");
        svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
        /*seek ready*/
         mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_SEEK_READY,
                        ST_UNKNOWN
                        );
        pt_media_desc->b_is_seeking = FALSE;
        if(pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X &&
           pt_media_desc->t_repeat_ab_info.b_seeking)
        {
            MM_SVC_TRICK_INFO_T             t_trick_info;
            /*trick to 1x*/
            t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
            i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK failed, ret=%d line = %d\n", i4_ret, __LINE__);
                i4_ret =  SVCTXR_FAIL;
            }
            pt_media_desc->t_repeat_ab_info.b_seeking = FALSE;
            mm_hdlr_invoke_client_svc_nfy_fct(
                 pt_svctx,
                 SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
                 ST_UNKNOWN
                 );
        }
    }
    else if(e_nfy_cond == MM_RANGE_BUFF_READY)
    {
        x_dbg_stmt(_ERROR_HEADER"DRIVER SEND MM_RANGE_BUFF_READY\n");
        /*if(pt_media_desc->b_is_buff_underflow)*/
        {
            pt_media_desc->b_is_buff_underflow = FALSE;
            if(!pt_media_desc->b_is_seeking)
            {
                i4_ret = _mm_hdlr_do_buf_stat_ctrl(pt_media_desc, TRUE);
            }            
        }
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_BUF_READY,
                        ST_UNKNOWN
                        );
    }
    else if(e_nfy_cond == MM_RANGE_BUFF_UNDERFLOW)
    {    
        x_dbg_stmt(_ERROR_HEADER"DRIVER SEND MM_RANGE_BUFF_UNDERFLOW\n");
        /*if(!pt_media_desc->b_is_buff_underflow)*/
        {
            pt_media_desc->b_is_buff_underflow = TRUE;
            if(!pt_media_desc->b_is_seeking)
            {
                i4_ret = _mm_hdlr_do_buf_stat_ctrl(pt_media_desc, FALSE);
            }          
        }
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_UNDERFLOW,
                        ST_UNKNOWN
                        );
    }
#ifndef DRV_NO_PRE_PARSING
    else if(e_nfy_cond == MM_RANGE_PARSING_MODE)
    {
        if(!pt_svctx->b_in_stopping &&
            x_handle_valid(gh_pre_parsing_time_out) == TRUE)
        {
            gb_pre_parsing = TRUE;
            x_memset(&pt_running_svc_req->t_uop_fct_tbl, 0, sizeof(MM_SVC_UOP_HANDLER_FCT_TBL_T));
            x_timer_start(
                        gh_pre_parsing_time_out,
                        gui4_pre_parsing_time,
                        X_TIMER_FLAG_ONCE,
                        _mm_hdlr_pre_parse_timeout_callback,
                        (VOID*)pt_svctx
                        );
        }
    }
    else if(e_nfy_cond == MM_RANGE_PARSING_MODE_DONE)
    {
        if (x_handle_valid(gh_pre_parsing_time_out) == TRUE) {
            x_timer_stop(gh_pre_parsing_time_out);
            _mm_hdlr_pre_parse_timeout_callback(gh_pre_parsing_time_out, pt_svctx);
        }
    }
#endif
#ifndef TIME_SHIFT_SUPPORT
    else
    {
        x_dbg_stmt(_ERROR_HEADER"range_nfy return fail i! line = %d\n",__LINE__);
        return FALSE;
    }
#else

    i4_ret = _mm_hdlr_send_msg(pt_media_desc->h_svctx,
                               (UINT32)NULL,
                               _SVCTX_MSG_SWDMX_NTFY,
                               NULL,
                               (UINT32)e_nfy_cond,
                               (UINT32)ui4_data_1,
                               (UINT32)ui4_data_2);
    if (i4_ret != SVCTXR_OK)
    {
        return FALSE;
    }
#endif /* TIME_SHIFT_SUPPORT */

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_idxtbl_type_convt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static MM_ELEMT_TYPE_T _mm_hdlr_idxtbl_type_convt(
                                    MIDXBULD_ELMT_STRM_TYPE_T e_es_type)
{
    if (e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
    {
        return MM_ELEMT_TYPE_AUDIO;
    }
    else if (e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)
    {
        return MM_ELEMT_TYPE_VIDEO;
    }
    else if (e_es_type == MIDXBULD_ELEM_STRM_TYPE_SUBTITLE)
    {
        return MM_ELEMT_TYPE_SP;
    }
    else if (e_es_type == MIDXBULD_ELEM_STRM_TYPE_KEY)
    {
        return MM_ELEMT_TYPE_KEY;
    }
    else
    {
        return MM_ELEMT_TYPE_UNKNOWN;
    }
}


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_stream_id_config
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_stream_id_config(
                                       MEDIA_FORMAT_T t_media_fmt,
                                       UINT32         ui4_stream_id,
                                       STREAM_ID_T*   pt_strm_id
                                       )
{
    if (t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        if (t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_AVI;
            pt_strm_id->u.t_stm_avi_id = ui4_stream_id;
        }
        if (t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_ASF;
            pt_strm_id->u.t_stm_asf_id = ui4_stream_id;
        }
        if (t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_MP4;
            pt_strm_id->u.t_stm_mp4_id = ui4_stream_id;
        }
        if(t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_MKV;
            pt_strm_id->u.t_stm_mkv_id = ui4_stream_id;
        }
        if(t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_RM;
            pt_strm_id->u.t_stm_rm_id = ui4_stream_id;
        }
        /*add for flv*/
        if(t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_FLV;
            pt_strm_id->u.t_stm_flv_id = ui4_stream_id;
        }
        /*add for ogg*/
        if(t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
        {
            pt_strm_id->e_type = STREAM_ID_TYPE_OGG;
            pt_strm_id->u.t_stm_flv_id = ui4_stream_id;
        }
    }
    else if (t_media_fmt.e_media_type == MEDIA_TYPE_PS)
    {
        pt_strm_id->e_type = STREAM_ID_TYPE_PS;
        pt_strm_id->u.t_stm_ps_id.ui2_stm_id = (UINT16)ui4_stream_id;
    }
    else
    {
        return SVCTXR_FAIL;
    }
    return SVCTXR_OK ;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_es_info_cpy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _mm_hdlr_es_info_cpy( MEDIA_FORMAT_T t_media_fmt,
                                  MM_RANGE_ELMT_IDXTBL_T* pt_es_in_range,
                                  MIDXBULD_ELMT_STRM_TBL_INFO_T* pt_es_in_idx_build)
{


    pt_es_in_range->e_elem_type = _mm_hdlr_idxtbl_type_convt(
                                   pt_es_in_idx_build->e_es_type);

    pt_es_in_range->e_tbl_type = MM_RANGE_ELEMT_IDXTBL_TYPE_PLAYBACK;
    pt_es_in_range->ui4_avg_bytes_per_sec = pt_es_in_idx_build->ui4_avg_bytes_per_sec;
    pt_es_in_range->ui4_rate = pt_es_in_idx_build->ui4_rate;
    pt_es_in_range->ui4_scale =pt_es_in_idx_build->ui4_scale;
    if (pt_es_in_idx_build->ui4_sample_sz == 0)
    {
        pt_es_in_range->b_is_VBR = TRUE;
    }
    else
    {
        pt_es_in_range->b_is_VBR = FALSE;
    }
     _mm_hdlr_stream_id_config(
                          t_media_fmt,
                          pt_es_in_idx_build->ui4_strm_id,
                          &pt_es_in_range->t_strm_id);
    /*reset here , these stuff should be assignned out of this function*/
    if (pt_es_in_range->ui4_rate == 0)
    {
        pt_es_in_range->ui4_rate  = 1;
    }
    if (pt_es_in_range->ui4_scale == 0)
    {
        pt_es_in_range->ui4_scale = 1;
    }
    /*pt_es_in_range->ui4_start_frame_num = 0;
    pt_es_in_range->ui8_base_offset = 0;
    pt_es_in_range->ui4_end_frame_num = 0;
    pt_es_in_range->ui4_number_of_entry = 0;*/

}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_set_hdlr_tm_scale_lst
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_tm_scale_lst(HANDLE_T               h_minfo,
                                       MM_RANGE_INFO_T*       pt_mm_rang_info)
{
    INT32                           i4_ret = SVCTXR_OK;
    SIZE_T                          z_size;
    MM_RANGE_TRACK_TM_SCALE_T*      pt_mm_track_tm_scale_lst;
    UINT16                          ui2_strm_num;
    UINT16                          ui2_idx;
    UINT16                          ui2_total_strm_num;
    MINFO_INFO_T                    t_strm_attr;

    pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
    pt_mm_track_tm_scale_lst = (MM_RANGE_TRACK_TM_SCALE_T*)
        x_mem_alloc(sizeof(MM_RANGE_TRACK_TM_SCALE_T) * (pt_mm_rang_info->ui4_track_tm_scale_num));
    if (NULL == pt_mm_track_tm_scale_lst)
    {
        x_dbg_stmt("memory for tm code scale fail! line = %d\n", __LINE__);
        return SVCTXR_FAIL;
    }
    /*set allocated memoty 0*/
    x_memset(pt_mm_track_tm_scale_lst, 0,
             sizeof(MM_RANGE_TRACK_TM_SCALE_T)
             * pt_mm_rang_info->ui4_track_tm_scale_num);
    pt_mm_rang_info->pt_track_tm_scale_lst = pt_mm_track_tm_scale_lst;
    ui2_total_strm_num = (UINT16)pt_mm_rang_info->ui4_track_tm_scale_num;
    /*get video track tm scale*/
    i4_ret = x_minfo_get_num_recs(h_minfo,
                                  MINFO_INFO_TYPE_VIDEO_ATTR,
                                  &ui2_strm_num);
    if (MINFOR_OK != i4_ret)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    if (ui2_strm_num > ui2_total_strm_num)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    for (ui2_idx = 1 ; ui2_idx <= ui2_strm_num; ++ui2_idx)
    {
        z_size = sizeof(MINFO_INFO_T);
        x_memset(&t_strm_attr, 0, z_size);
        i4_ret = x_minfo_get_info( h_minfo,
                                   MINFO_INFO_TYPE_VIDEO_ATTR,
                                   (MINFO_REC_IDX_T)ui2_idx,
                                   (VOID*)&t_strm_attr,
                                   &z_size);
        if (MINFOR_OK != i4_ret)
        {
            x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
            pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
            return SVCTXR_FAIL;
        }
        pt_mm_track_tm_scale_lst->ui4_strm_id =
            t_strm_attr.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_numerator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_video_stm_attr.t_track_timecode_scale.ui4_float_numerator;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_denominator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_video_stm_attr.t_track_timecode_scale.ui4_float_denominator;
        pt_mm_track_tm_scale_lst++;
    }
    ui2_total_strm_num -= ui2_strm_num;
    /*get audio track tm scale*/
    i4_ret = x_minfo_get_num_recs(h_minfo,
                                  MINFO_INFO_TYPE_AUDIO_ATTR,
                                  &ui2_strm_num);
    if (MINFOR_OK != i4_ret)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    if (ui2_strm_num > ui2_total_strm_num)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    for (ui2_idx = 1 ; ui2_idx <= ui2_strm_num; ++ui2_idx)
    {
        z_size = sizeof(MINFO_INFO_T);
        x_memset(&t_strm_attr, 0, z_size);
        i4_ret = x_minfo_get_info( h_minfo,
                                   MINFO_INFO_TYPE_AUDIO_ATTR,
                                   (MINFO_REC_IDX_T)ui2_idx,
                                   (VOID*)&t_strm_attr,
                                   &z_size);
        if (MINFOR_OK != i4_ret)
        {
            x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
            pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
            return SVCTXR_FAIL;
        }
        pt_mm_track_tm_scale_lst->ui4_strm_id =
            t_strm_attr.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_numerator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_audio_stm_attr.t_track_timecode_scale.ui4_float_numerator;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_denominator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_audio_stm_attr.t_track_timecode_scale.ui4_float_denominator;
        pt_mm_track_tm_scale_lst++;
    }
    ui2_total_strm_num -= ui2_strm_num;
    /*get subtitle track tm scale*/
    i4_ret = x_minfo_get_num_recs(h_minfo,
                                  MINFO_INFO_TYPE_SP_ATTR,
                                  &ui2_strm_num);
    if (MINFOR_OK != i4_ret)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    if (ui2_strm_num > ui2_total_strm_num)
    {
        x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
        pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
        return SVCTXR_FAIL;
    }
    for (ui2_idx = 1 ; ui2_idx <= ui2_strm_num; ++ui2_idx)
    {
        z_size = sizeof(MINFO_INFO_T);
        x_memset(&t_strm_attr, 0, z_size);
        i4_ret = x_minfo_get_info( h_minfo,
                                   MINFO_INFO_TYPE_SP_ATTR,
                                   (MINFO_REC_IDX_T)ui2_idx,
                                   (VOID*)&t_strm_attr,
                                   &z_size);
        if (MINFOR_OK != i4_ret)
        {
            x_mem_free(pt_mm_rang_info->pt_track_tm_scale_lst);
            pt_mm_rang_info->pt_track_tm_scale_lst = NULL;
            return SVCTXR_FAIL;
        }
        pt_mm_track_tm_scale_lst->ui4_strm_id =
            t_strm_attr.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_numerator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_video_stm_attr.t_track_timecode_scale.ui4_float_numerator;
        pt_mm_track_tm_scale_lst->ui4_tm_scale_denominator =
            t_strm_attr.u.t_stm_attribute.u.t_mkv_video_stm_attr.t_track_timecode_scale.ui4_float_denominator;
        pt_mm_track_tm_scale_lst++;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_audio_desc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_add_audio_desc(HANDLE_T               h_minfo,
                                           UINT8                   ui1_aud_num,
                                           MM_RANGE_ELMT_IDXTBL_T* pt_es_in_range)
{
    MINFO_INFO_T                    t_minfo_data;
    ASH_AUDIO_CODEC_SPEC_INFO_T     t_ash_codec_info;
    SIZE_T                          z_size;
    INT32                           i4_ret;

    if(STREAM_ID_TYPE_MKV == pt_es_in_range->t_strm_id.e_type)
    {
        z_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_data, 0, z_size);
        i4_ret = x_minfo_get_info(h_minfo,
                                  MINFO_INFO_TYPE_AUDIO_ATTR,
                                  ui1_aud_num,
                                  &t_minfo_data,
                                  &z_size);
        if(MINFOR_OK != i4_ret)
        {
            return i4_ret;
        }
        x_memset(&t_ash_codec_info, 0, sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));
        if (t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V1 ||
            t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V2)
        {
            t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
            t_ash_codec_info.u.t_wma_info.ui2_encoder_opt =
                t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.ui2_encode_opts;
            t_ash_codec_info.u.t_wma_info.ui2_num_ch =
                (UINT16)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i2_channels;
            t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i2_block_align;
            t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec =
                t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.ui4_avg_bytes_sec;
            t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i4_samples_per_sec;

        }
        else if (t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_PCM)
        {
            t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i2_channels;
            t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i4_samples_per_sec;
            t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
            if (t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_align == MINFO_INFO_AUD_PCM_BIG_ENDIAN)
            {
                t_ash_codec_info.u.t_pcm_info.b_big_endian = TRUE;
            }
            else
            {
                t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
            }
        }
        else if (t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
        {
            t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i2_channels;
            t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                (UINT32)t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.i4_samples_per_sec;
        }
        t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
            t_minfo_data.u.t_stm_attribute.u.t_mkv_audio_stm_attr.e_enc);
        pt_es_in_range->t_aud_spec_info = t_ash_codec_info;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_range_info_gen
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/

static INT32 _mm_hdlr_range_info_gen(
                                    HANDLE_T                            h_media_desc,
                                    MM_RANGE_INFO_T*                    pt_mm_rang_info,
                                    MIDXBULD_ELMT_TBL_ALLOC_INFO_T*     pt_range_info,
                                    BOOL                                b_direction,
                                    BOOL                                b_is_eos,
                                    BOOL                                b_is_use_idx_tbl)

{
    MM_SVC_REQ_T*                   pt_running_svc_req = NULL;
    INT32                           i4_ret = 0;
    SIZE_T                          z_size = 0;
    UINT16                          ui2_rec_nums = 0;
    MIDXBULD_ELMT_TBL_OBJ_T*        pt_ele_tbl_obj = NULL;
    MIDXBULD_ELMT_TBL_INFO_T        t_ele_idx_tbl;
    MIDXBULD_KEY_TBL_INFO_T         t_key_tbl_info;
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext = NULL;
    MM_RANGE_ELMT_IDXTBL_T**        ppt_next = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*    pt_ele_tbl_desc = NULL;
    MEDIA_DESCRIBT_T*               pt_media_desc = NULL;
    MINFO_INFO_T                    t_strm_attr;
    MINFO_INFO_T                    t_minfo_idx;
    MINFO_INFO_T                    t_minfo_genric;
    HANDLE_T                        h_minfo;
    SVCTX_T*                        pt_svctx = NULL;
    HANDLE_T                        h_midxbuld;
#ifdef ENABLE_DIVX_DRM
    BOOL                            b_rtn_frm_rev = FALSE;
    DIVX_DRM_UNION_INFO_T           t_divx_drm_info;
#endif
    MM_RANGE_ELMT_IDXTBL_T*         pt_mm_ele_idx_tbl_v = NULL;
    MM_RANGE_ELMT_IDXTBL_T*         pt_mm_ele_idx_tbl_a = NULL;
    MM_RANGE_ELMT_IDXTBL_T*         pt_mm_ele_idx_tbl   = NULL;
    MM_RANGE_ELMT_IDXTBL_T*         pt_mm_ele_idx_tbl_last;
    UINT16                          ui2_aud_rec_nums = 0;
    MINFO_INFO_T                    t_minfo_aud;
    UINT32                          ui4_avi_start_pts_offset = 0;
    MM_RANGE_ELMT_IDXTBL_T*         pt_mm_ele_idx_tbl_ts = NULL;
#if SUPPORT_MULTI_INFO
    MINFO_INFO_T                    t_scale_info;
#endif


    if (pt_range_info == NULL || pt_mm_rang_info == NULL )
    {
        return SVCTXR_INV_ARG;
    }
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
    pt_running_svc_req = (MM_SVC_REQ_T*)(pt_svctx->pt_running_svc_req);
    h_minfo = pt_media_desc->h_media_info;
    h_midxbuld = pt_media_desc->h_index_info;

    z_size  = sizeof(MINFO_INFO_T);
    x_memset(&t_strm_attr,      0, z_size);
    x_memset(&t_minfo_idx,      0, z_size);
    x_memset(&t_minfo_genric,   0, z_size);
    x_memset(&t_minfo_genric,   0, z_size);
    x_memset(&t_minfo_aud,      0, z_size);
    x_memset(&t_key_tbl_info,   0, sizeof(MIDXBULD_KEY_TBL_INFO_T));
#if SUPPORT_MULTI_INFO
    x_memset(&t_scale_info,     0, z_size);
#endif

    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_IDX_INFO,
                              1,
                              &t_minfo_idx, &z_size);
    if (MM_HDLR_WITH_MINFO(pt_media_desc) && i4_ret != MINFOR_OK)
    {
        return SVCTXR_FAIL;
    }

    i4_ret = x_minfo_get_info(h_minfo,
                             MINFO_INFO_TYPE_GENERIC,
                             1,
                             &t_minfo_genric, &z_size);
    if (MM_HDLR_WITH_MINFO(pt_media_desc) && i4_ret != MINFOR_OK)
    {
        return SVCTXR_FAIL;
    }
    
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT    
    pt_mm_rang_info->b_is_bad_intlevd = pt_media_desc->b_is_bad_intlevd;
#endif

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI &&
        pt_media_desc->pt_strm_info == NULL /*it's not cmpb set mode*/)
    {
        i4_ret = x_minfo_get_num_recs(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      &ui2_rec_nums);
        if (i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        if (ui2_rec_nums == 0)
        {
            pt_mm_rang_info->ui8_vid_duration = 0;
        }
        else
        {
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      1,
                                      &t_strm_attr,
                                      &z_size);
            if (i4_ret != MINFOR_OK)
            {
                return SVCTXR_FAIL;
            }
            pt_mm_rang_info->ui8_vid_duration =
                t_strm_attr.u.t_stm_attribute.u.t_avi_video_stm_attr.ui8_duration;
        }
        /* get avi audio start pts offset */
        x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_AUDIO_ATTR, &ui2_aud_rec_nums);
        if (ui2_aud_rec_nums >= 1)
        {
            i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR, 1,
                         &t_minfo_aud,
                         &z_size);
            if ( i4_ret != MINFOR_OK)
            {
                return  SVCTXR_NO_RECORD;
            }
            ui4_avi_start_pts_offset =
                t_minfo_aud.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_cbr_sub_vbr;
        }
    }
    else
    {
        pt_mm_rang_info->ui8_vid_duration = t_minfo_genric.u.t_general_info.ui8_pb_duration;
    }

    if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL &&
       pt_media_desc->pt_strm_info != NULL) /*it's pull mode app master*/
    {
        pt_mm_rang_info->t_pts_offset         = (PTS_T)pt_media_desc->t_playback_ctrl_pull_info.ui8_preroll;
        pt_mm_rang_info->ui4_sz_max_acces_unt = pt_media_desc->t_playback_ctrl_pull_info.ui4_max_acces_unt;
        pt_mm_rang_info->ui4_sz_min_acces_unt = pt_media_desc->t_playback_ctrl_pull_info.ui4_min_acces_unt;
        pt_mm_rang_info->ui8_fileoffset       = pt_media_desc->t_playback_ctrl_pull_info.ui8_media_data_ofst;
        pt_mm_rang_info->ui8_pb_duration      = pt_media_desc->t_playback_ctrl_pull_info.ui8_media_duration;
        pt_mm_rang_info->z_range_sz           = pt_media_desc->t_playback_ctrl_pull_info.ui8_media_data_size;
    }
    else
    {
        pt_mm_rang_info->t_pts_offset    = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                       t_minfo_genric.u.t_general_info.ui8_preroll :
                                       0;
        pt_mm_rang_info->ui4_sz_max_acces_unt = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.ui4_sz_max_acces_unt :
                                            0;
        pt_mm_rang_info->ui4_sz_min_acces_unt = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.ui4_sz_min_acces_unt :
                                            0;
        pt_mm_rang_info->ui8_fileoffset  = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                       t_minfo_idx.u.t_idx_info.ui8_data_offset :
                                       0;

#ifdef ENABLE_MMS_SUPPORT
        if(pt_media_desc->pc_mms_header_buf)
        {
            pt_media_desc->ui8_media_sz = (UINT64)(-1);
            pt_mm_rang_info->ui8_pb_duration = (UINT64)(-1); /*for mms*/
            pt_mm_rang_info->z_range_sz      = (UINT64)(-1);
        }
        else
#endif 
        {
            pt_mm_rang_info->ui8_pb_duration = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                               t_minfo_genric.u.t_general_info.ui8_pb_duration :
                                               pt_media_desc->ui8_media_dur; /*for dlna lpcm format*/
    
            pt_mm_rang_info->z_range_sz      = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                               t_minfo_idx.u.t_idx_info.ui8_data_sz :
                                               pt_media_desc->ui8_media_sz;
        }

        /* for DLNA server */
        pt_mm_rang_info->b_non_seekable  = MM_HDLR_WITH_MINFO(pt_media_desc) ? 
                                           t_minfo_genric.u.t_general_info.b_non_seekable : FALSE;
#ifdef MW_MM_DUAL_VID_SUPPORT
        if (!pt_media_desc->b_is_thumbnail_playback)
        {
        pt_mm_rang_info->b_stereo_scopic = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.b_stereo_scopic :
                                            FALSE;
        pt_mm_rang_info->b_half_width = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.b_half_width :
                                            FALSE;
        pt_mm_rang_info->b_half_height = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.b_half_height :
                                            FALSE;
        pt_mm_rang_info->e_scopic_type = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.e_scopic_type :
                                            MM_SCOPIC_UNKNOWN;
        pt_mm_rang_info->ui8_skip_dur = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                            t_minfo_genric.u.t_general_info.ui8_skip_dur :
                                            0;
        }
        
#endif
    }

    if (pt_media_desc->e_scopic_type != MM_SCOPIC_UNKNOWN && 
        pt_mm_rang_info->e_scopic_type == MM_SCOPIC_UNKNOWN &&
        !pt_media_desc->b_is_thumbnail_playback)
    {
        pt_mm_rang_info->b_stereo_scopic = TRUE;
        pt_mm_rang_info->b_half_width = TRUE;
        pt_mm_rang_info->b_half_height = TRUE;
        pt_mm_rang_info->e_scopic_type = pt_media_desc->e_scopic_type;
        pt_mm_rang_info->ui8_skip_dur = 0;
    }

    pt_mm_rang_info->t_vid_start_pts =  pt_mm_rang_info->t_aud_start_pts = 0;
                                        //pt_range_info->u_range.t_es.ui4_start_pts;
    pt_mm_rang_info->ui8_start_pcr   =  pt_range_info->u_range.t_es.ui8_start_pcr;

    pt_media_desc->ui8_data_ofst = pt_mm_rang_info->ui8_fileoffset;

#ifdef RM_LOGICAL_STREAM_SUPPORT
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
    {
        pt_mm_rang_info->ui8_fileoffset2[0] = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                           t_minfo_idx.u.t_idx_info.ui8_data_offset :
                                           0;
        pt_mm_rang_info->z_range_sz2[0]    = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                           t_minfo_idx.u.t_idx_info.ui8_data_sz :
                                           pt_media_desc->ui8_media_sz;
        pt_mm_rang_info->ui8_fileoffset2[1] = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                           t_minfo_idx.u.t_idx_info.ui8_data_offset2 :
                                           0;
        pt_mm_rang_info->z_range_sz2[1]    = MM_HDLR_WITH_MINFO(pt_media_desc) ?
                                           t_minfo_idx.u.t_idx_info.ui8_data_sz2 :
                                           pt_media_desc->ui8_media_sz;
    }
#endif
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
    {
        pt_mm_rang_info->t_aud_start_pts += ui4_avi_start_pts_offset;
    }

    pt_mm_rang_info->t_init_stc      =  pt_mm_rang_info->t_vid_start_pts;
    pt_mm_rang_info->ui4_vid_render_counts = 0;
    /*tmp, to be modified*/
    if((pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)
        && ((pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA)
            || (pt_media_desc->e_svc_protocol == PROTOCL_MODE_UNKNOWN)))
    {
        if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA)
        {            
            pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_DLNA;
        }
        else if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_UNKNOWN)
        {
            pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_CMPB;
        }
    }
    else
    {
        if ((pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH))
        {
            if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_NETWORK_NETFLIX)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_NETFLIX;
            }
            else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_PUSH_MHP)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_MHP;
            }
            else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_NETWORK_SKYPE)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_SKYPE;
            }
            else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_HIGH_SPEED_STORAGE)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
            }
            else
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_PUSH;
            }
        }
        else if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
            pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
            && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
        {
            if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_NETWORK_NETFLIX)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_NETFLIX;
            }
            else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_HIGH_SPEED_STORAGE)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
            }
            else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_VESTEL_TS)
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
            }
            else
            {
                pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_NETWORK_CMPB;
            }
        }
        else
        {
            pt_mm_rang_info->e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
        }
    }

    /*mkv track timecode scale*/
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
    {
        /*mkv segment timecode scale*/
        pt_mm_rang_info->ui8_timecode_scale = t_minfo_genric.u.t_general_info.ui8_timecode_scale;
        pt_mm_rang_info->ui4_track_tm_scale_num = (UINT32)t_minfo_genric.u.t_general_info.ui1_strm_ns;
        i4_ret = _mm_hdlr_set_tm_scale_lst(h_minfo, pt_mm_rang_info);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_range_info_gen: set timecode scale lst fail!! line = %d\n", __LINE__);
            return SVCTXR_FAIL;
        }
    }

    /* video thumbnail support */
    if (pt_media_desc->b_is_thumbnail_playback)
    {
        pt_mm_rang_info->ui4_vid_render_counts = (UINT32)pt_media_desc->t_video_thumbnail_info.ui8_pic_idx;
        pt_mm_rang_info->b_thumbnail_pb = TRUE;
    }

    pt_mm_rang_info->ui4_aud_render_counts = 0;
    pt_mm_rang_info->ui4_sp_render_counts  = 0;
    if (pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
        pt_mm_rang_info->t_vid_start_render_pts = pt_mm_rang_info->t_aud_start_render_pts
                                                = pt_mm_rang_info->t_sp_start_render_pts
                                                = pt_running_svc_req->t_init_pos_info.ui8_pos_val;
    }
    else
    {
        pt_mm_rang_info->t_vid_start_render_pts = pt_mm_rang_info->t_aud_start_render_pts
                                                = pt_mm_rang_info->t_sp_start_render_pts
                                                = 0;
    }
    pt_mm_rang_info->pt_range_nfy = _mm_hdlr_range_playback_nfy_fct;
    pt_mm_rang_info->pv_tag = (VOID*)h_media_desc;
    pt_mm_rang_info->b_endofstream = b_is_eos;
    pt_mm_rang_info->b_direction   = b_direction;
    pt_mm_rang_info->ui4_range_id  = pt_range_info->ui4_range_id;

    /* for mkv cotent encodings */
    if (pt_running_svc_req->t_uop_fct_tbl.pf_get_decode != NULL)
    {
        pt_running_svc_req->t_uop_fct_tbl.pf_get_decode(pt_media_desc,pt_mm_rang_info);
    }
    
    /*no idexing table means that no key table could be allocated*/
    if (!pt_media_desc->b_is_no_idexing)
    {
        t_key_tbl_info.ui4_key_tbl_id = pt_media_desc->h_key_tbl;
        i4_ret = x_midxbuld_get_info(
                                      h_midxbuld,
                                      MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
                                      (VOID*)&t_key_tbl_info,
                                      &z_size);

        if (t_key_tbl_info.pt_key_tbl_ext == NULL ||
            i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"mm_hdlr :get key table ERROR! line = %d\n", __LINE__);
            return SVCTXR_FAIL;
        }
        pt_mm_rang_info->b_key_tbl_exist = TRUE;
        /* 
              for 3D
              check whether to support the 3D trick
         */
        pt_mm_rang_info->b_support_3d_trick = t_key_tbl_info.pt_key_tbl_ext->b_support_3d_trick;
        pt_ele_tbl_obj = &(t_key_tbl_info.pt_key_tbl_ext->t_elmt_tbl);
        if (pt_ele_tbl_obj->ui4_number_of_entry == 0)
        {
            pt_mm_rang_info->b_key_tbl_exist = FALSE;
            pt_media_desc->b_is_no_idexing = TRUE;
            ppt_next = (MM_RANGE_ELMT_IDXTBL_T**)&(pt_mm_rang_info->pt_idxtbl_lst);
        }
        else
        {
            pt_mm_ele_idx_tbl_last = NULL;
            while(pt_ele_tbl_obj != NULL)
            {
                /*allocate video key table*/
                pt_mm_ele_idx_tbl_v = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
                if (pt_mm_ele_idx_tbl_v == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_memset(pt_mm_ele_idx_tbl_v, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));

                _mm_hdlr_es_info_cpy(pt_media_desc->t_media_fmt,
                                     pt_mm_ele_idx_tbl_v,
                                     &pt_ele_tbl_obj->u_fmt.t_es.t_info);

                pt_mm_ele_idx_tbl_v->ui4_number_of_entry = pt_ele_tbl_obj->ui4_number_of_entry;

                /* For the case of MP3, we need to get the total duration from its uop handler */
                if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
                    && ((pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP2)
                        || (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)))
                {
                    pt_mm_rang_info->ui8_pb_duration = (pt_ele_tbl_obj->ui4_number_of_entry) * 90000;
                }

                pt_mm_ele_idx_tbl_v->ui8_base_offset = pt_ele_tbl_obj->ui8_base_offset;

                pt_mm_ele_idx_tbl_v->u.pt_pb_idx_tbl_entry =
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry;
                if (pt_mm_ele_idx_tbl_last == NULL)
                {
                    pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                    pt_mm_rang_info->pt_idxtbl_lst = pt_mm_ele_idx_tbl_v;
                }
                else
                {
                    pt_mm_ele_idx_tbl_last->pt_next_partial = pt_mm_ele_idx_tbl_v;
                    pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                }
                pt_ele_tbl_obj = pt_ele_tbl_obj->pt_next;
            }
            ppt_next = (MM_RANGE_ELMT_IDXTBL_T**)&(pt_mm_rang_info->pt_idxtbl_lst->pv_next_tbl);
        }

        /*add 3D second video */
        /*In order avoid side effect add 3D dual video key table here. In face this function is terrible,
                 however, should clean up later*/
         #if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID)))  
         if ( ( pt_media_desc->b_multi_track_3d )
               && (!pt_media_desc->b_is_thumbnail_playback) )
         {
             pt_key_tbl_ext = t_key_tbl_info.pt_key_tbl_ext->pt_next;

             while ( pt_key_tbl_ext != NULL )
             {
                 if ( ( pt_key_tbl_ext->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type ==
                        MIDXBULD_ELEM_STRM_TYPE_VIDEO )
                     || ( pt_key_tbl_ext->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type ==
                        MIDXBULD_ELEM_STRM_TYPE_KEY ) )
                 {
                     pt_ele_tbl_obj         = & ( pt_key_tbl_ext->t_elmt_tbl );
                     pt_mm_ele_idx_tbl_last = NULL;
                     while ( pt_ele_tbl_obj != NULL )
                     {
                         pt_mm_ele_idx_tbl_v = x_mem_alloc( sizeof( MM_RANGE_ELMT_IDXTBL_T ) );
                         if ( pt_mm_ele_idx_tbl_v == NULL )
                         {
                             return SVCTXR_NO_RESOURCE;
                         }
                         x_memset( pt_mm_ele_idx_tbl_v, 0, sizeof( MM_RANGE_ELMT_IDXTBL_T ) );
                         _mm_hdlr_es_info_cpy( pt_media_desc->t_media_fmt,
                                                pt_mm_ele_idx_tbl_v,
                                                &pt_ele_tbl_obj->u_fmt.t_es.t_info );
             
                         pt_mm_ele_idx_tbl_v->e_elem_type = MM_ELEMT_TYPE_KEY;
                         pt_mm_ele_idx_tbl_v->ui4_number_of_entry = pt_ele_tbl_obj->ui4_number_of_entry;
                         pt_mm_ele_idx_tbl_v->ui8_base_offset = pt_ele_tbl_obj->ui8_base_offset;
                         pt_mm_ele_idx_tbl_v->u.pt_pb_idx_tbl_entry =
                             ( MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* )pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry;
                         if ( pt_mm_ele_idx_tbl_last == NULL )
                         {
                             pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                             *ppt_next = pt_mm_ele_idx_tbl_v;
                             ppt_next = ( MM_RANGE_ELMT_IDXTBL_T** )&( pt_mm_ele_idx_tbl_v->pv_next_tbl );
                         }
                         else
                         {
                             pt_mm_ele_idx_tbl_last->pt_next_partial = pt_mm_ele_idx_tbl_v;
                             pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                         }
                         pt_ele_tbl_obj = pt_ele_tbl_obj->pt_next;
                     }
                 }
                 
                 pt_key_tbl_ext =  pt_key_tbl_ext->pt_next;
             }
         } 
         #endif 
        
        /*allocate audio key table*/
        /*Get audio key table from key table list*/
        if (pt_mm_rang_info->b_key_tbl_exist)
        {
            pt_key_tbl_ext = t_key_tbl_info.pt_key_tbl_ext;
            while (pt_key_tbl_ext != NULL)
            {
                if (pt_key_tbl_ext->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type ==
                    MIDXBULD_ELEM_STRM_TYPE_AUDIO)
                {
                    pt_ele_tbl_obj = &(pt_key_tbl_ext->t_elmt_tbl);
                    pt_mm_ele_idx_tbl_last = NULL;
                    while(pt_ele_tbl_obj != NULL)
                    {
                        pt_mm_ele_idx_tbl_a = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
                        if (pt_mm_ele_idx_tbl_a == NULL)
                        {
                            x_mem_free(pt_mm_ele_idx_tbl_v);
                            return SVCTXR_NO_RESOURCE;
                        }
                        x_memset(pt_mm_ele_idx_tbl_a, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
                        _mm_hdlr_es_info_cpy(pt_media_desc->t_media_fmt,
                                              pt_mm_ele_idx_tbl_a,
                                              &pt_ele_tbl_obj->u_fmt.t_as_key.t_info);

                        pt_mm_ele_idx_tbl_a->e_elem_type = MM_ELEMT_TYPE_KEY;
                        pt_mm_ele_idx_tbl_a->ui4_number_of_entry = pt_ele_tbl_obj->ui4_number_of_entry;
                        pt_mm_ele_idx_tbl_a->ui8_base_offset = pt_ele_tbl_obj->ui8_base_offset;
                        pt_mm_ele_idx_tbl_a->u.pt_pb_idx_tbl_entry =
                            (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)pt_ele_tbl_obj->u_fmt.t_as_key.pui4_pts;
                        if (pt_mm_ele_idx_tbl_last == NULL)
                        {
                            pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_a;
                            *ppt_next = pt_mm_ele_idx_tbl_a;
                            ppt_next = (MM_RANGE_ELMT_IDXTBL_T**)&(pt_mm_ele_idx_tbl_a->pv_next_tbl);
                        }
                        else
                        {
                            pt_mm_ele_idx_tbl_last->pt_next_partial = pt_mm_ele_idx_tbl_a;
                            pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_a;
                        }
                        pt_ele_tbl_obj = pt_ele_tbl_obj->pt_next;
                    }
                    pt_key_tbl_ext = pt_key_tbl_ext->pt_next;
                }
                else
                {
                    pt_key_tbl_ext =  pt_key_tbl_ext->pt_next;
                }
            }
        }
    }
    else
    {
#ifdef TIME_SHIFT_SUPPORT
        if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
        {
            pt_mm_ele_idx_tbl_ts = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
            if (pt_mm_ele_idx_tbl_ts == NULL)
            {
                ASSERT(FALSE, ("%s(), pt_mm_ele_idx_tbl_ts is NULL\r\n", __func__));
                return SVCTXR_NO_RESOURCE;
            }
            x_memset(pt_mm_ele_idx_tbl_ts, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
            pt_mm_ele_idx_tbl_ts->e_tbl_type = MM_RANGE_ELEMT_IDXTBL_TYPE_TIMESHIFT;
            pt_mm_ele_idx_tbl_ts->e_elem_type = MM_ELEMT_TYPE_UNKNOWN;
            pt_mm_ele_idx_tbl_ts->t_strm_id.e_type =  STREAM_ID_TYPE_TS;
            pt_mm_ele_idx_tbl_ts->t_strm_id.u.t_stm_ts_id = MPEG_2_NULL_PID;
            if (gpt_ts_idx_tbl_entry == NULL)
            {
                gpt_ts_idx_tbl_entry = x_mem_alloc(sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));
            }
            ASSERT(gpt_ts_idx_tbl_entry != NULL, ("%s(), gpt_ts_idx_tbl_entry is NULL\r\n", __func__));
            x_memset(gpt_ts_idx_tbl_entry, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));
            pt_mm_ele_idx_tbl_ts->u.pt_ts_idx_tbl_entry = gpt_ts_idx_tbl_entry;
            pt_mm_ele_idx_tbl_ts->u.pt_ts_idx_tbl_entry->t_tick_ctrl_blk = pt_media_desc->t_rec_tick_ctrl_blk;
            pt_mm_ele_idx_tbl_ts->pv_next_tbl = NULL;
            pt_mm_rang_info->pt_idxtbl_lst = pt_mm_ele_idx_tbl_ts;
        }
        else
#endif /* TIME_SHIFT_SUPPORT */
        if (MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
        {
            pt_mm_ele_idx_tbl_ts = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
            if (pt_mm_ele_idx_tbl_ts == NULL)
            {
                ASSERT(FALSE, ("%s(), pt_mm_ele_idx_tbl_ts is NULL\r\n", __func__));
                return SVCTXR_NO_RESOURCE;
            }
            x_memset(pt_mm_ele_idx_tbl_ts, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
            pt_mm_ele_idx_tbl_ts->e_tbl_type = MM_RANGE_ELEMT_IDXTBL_TYPE_TIMESHIFT;
            pt_mm_ele_idx_tbl_ts->e_elem_type = MM_ELEMT_TYPE_UNKNOWN;
            pt_mm_ele_idx_tbl_ts->t_strm_id.e_type =  STREAM_ID_TYPE_TS;
            pt_mm_ele_idx_tbl_ts->t_strm_id.u.t_stm_ts_id = MPEG_2_NULL_PID;
            if (gpt_ts_idx_tbl_entry == NULL)
            {
                gpt_ts_idx_tbl_entry = x_mem_alloc(sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));
            }
            ASSERT(gpt_ts_idx_tbl_entry != NULL, ("%s(), gpt_ts_idx_tbl_entry is NULL\r\n", __func__));
            x_memset(gpt_ts_idx_tbl_entry, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));
            pt_mm_ele_idx_tbl_ts->u.pt_ts_idx_tbl_entry = gpt_ts_idx_tbl_entry;
            pt_mm_ele_idx_tbl_ts->u.pt_ts_idx_tbl_entry->t_tick_ctrl_blk = pt_media_desc->t_rec_tick_ctrl_blk;
            pt_mm_ele_idx_tbl_ts->pv_next_tbl = NULL;
            pt_mm_rang_info->pt_idxtbl_lst = pt_mm_ele_idx_tbl_ts;
            
            return SVCTXR_OK;
        }
        pt_mm_rang_info->b_key_tbl_exist = FALSE;
        ppt_next = (MM_RANGE_ELMT_IDXTBL_T**)&(pt_mm_rang_info->pt_idxtbl_lst);
    }

    /*allocate element index tables*/
    pt_ele_tbl_desc = pt_range_info->pt_tbl_describ;
    if (pt_ele_tbl_desc != NULL)
    {
        UINT8 ui1_aud_num = 1;
        UINT8 ui1_video_num = 1;
        t_ele_idx_tbl.ui4_tbl_id = pt_ele_tbl_desc->ui4_tbl_id;
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        while(pt_ele_tbl_desc != NULL)
        {
            pt_mm_ele_idx_tbl = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
            if (pt_mm_ele_idx_tbl == NULL)
            {
                x_mem_free(pt_mm_ele_idx_tbl_v);
                x_mem_free(pt_mm_ele_idx_tbl_a);
                /* TBD:  memeory free
                for sub items allocated in this while loop */
                return SVCTXR_NO_RESOURCE;
            }
            /*should initialize it zero, herein contains pointer fields !!!!!*/
            x_memset(pt_mm_ele_idx_tbl, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
            _mm_hdlr_es_info_cpy(pt_media_desc->t_media_fmt,
                                 pt_mm_ele_idx_tbl,
                                 &pt_ele_tbl_desc->u_info.t_es);

            if (b_is_use_idx_tbl)
            {
                i4_ret = x_midxbuld_get_info(
                                             h_midxbuld,
                                             MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL,
                                             (VOID*)&t_ele_idx_tbl,
                                             &z_size);
                if (i4_ret != MIDXBULDR_OK)
                {
                    x_mem_free(pt_mm_ele_idx_tbl_v);
                    x_mem_free(pt_mm_ele_idx_tbl_a);
                    /* TBD:  memeory free
                    for sub items allocated in this while loop */
                    x_dbg_stmt(_ERROR_HEADER"get elem index tbl ERROR! line%d\n", __LINE__);
                    return SVCTXR_NO_RESOURCE;
                }
                pt_mm_ele_idx_tbl->ui4_number_of_entry = t_ele_idx_tbl.t_elmt_tbl.ui4_number_of_entry;
                pt_mm_ele_idx_tbl->ui8_base_offset = t_ele_idx_tbl.t_elmt_tbl.ui8_base_offset;
                pt_mm_ele_idx_tbl->u.pt_pb_idx_tbl_entry = (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)t_ele_idx_tbl.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;

            }
            else
            {
                if (pt_ele_tbl_desc->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
                {
                    z_size = sizeof(MINFO_INFO_T);
                    i4_ret = x_minfo_get_info(h_minfo,
                                             MINFO_INFO_TYPE_AUDIO_ATTR,
                                             ui1_aud_num,
                                             &t_strm_attr,
                                             &z_size);
                    pt_mm_ele_idx_tbl->ui4_number_of_entry =
                    t_strm_attr.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_length;
                    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                           pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
                    {
                        pt_mm_ele_idx_tbl->ui4_avg_frame_num = t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.ui4_avg_frame_num;
                        pt_mm_ele_idx_tbl->ui4_avg_packet_size = t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.ui4_avg_packet_size;
                        pt_mm_ele_idx_tbl->ui8_timeunit = t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_timeunit;
                        if (t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_AUDIO)
                        {
                            pt_mm_ele_idx_tbl->ui4_rate = t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;
                        }
                        else if (t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_VORBIS)
                        {
                            pt_mm_ele_idx_tbl->ui4_rate = t_strm_attr.u.t_stm_attribute.u.t_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_audio_sample_rate;
                        }
                    }
                   /*get audio description*/
                   _mm_hdlr_add_audio_desc(h_minfo, ui1_aud_num, pt_mm_ele_idx_tbl);
                   ui1_aud_num++;
                }
                else if (pt_ele_tbl_desc->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)
                {
                    z_size = sizeof(MINFO_INFO_T);
                   i4_ret = x_minfo_get_info(h_minfo,
                                             MINFO_INFO_TYPE_VIDEO_ATTR,
                                             ui1_video_num,
                                             &t_strm_attr,
                                             &z_size);
                   
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
                   if ( ( pt_media_desc->b_multi_track_3d )
                       && (!pt_media_desc->b_is_thumbnail_playback) )
                   {
                       ++ui1_video_num;
                   }
#else
#endif
                   pt_mm_ele_idx_tbl->ui4_number_of_entry =
                   t_strm_attr.u.t_stm_attribute.u.t_avi_video_stm_attr.ui4_length;
                   if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                           pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
                   {
                       pt_mm_ele_idx_tbl->ui4_avg_frame_num = t_strm_attr.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui4_avg_frame_num;
                       pt_mm_ele_idx_tbl->ui4_avg_packet_size = t_strm_attr.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui4_avg_packet_size;
                       pt_mm_ele_idx_tbl->ui8_timeunit = t_strm_attr.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui8_timeunit;
                       pt_mm_ele_idx_tbl->ui4_rate = t_strm_attr.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui8_sample_per_unit;
                   }
                }
                else if (pt_ele_tbl_desc->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_SUBTITLE)                
                {                   
                    z_size = sizeof(MINFO_INFO_T);                   
                    i4_ret = x_minfo_get_info(h_minfo,                                             
                            MINFO_INFO_TYPE_SP_ATTR,                                             
                            1,                                             
                            &t_strm_attr,                                             
                            &z_size);                   
                    pt_mm_ele_idx_tbl->ui4_number_of_entry = 0;                
                }   
                pt_mm_ele_idx_tbl->ui8_base_offset = 0;
                pt_mm_ele_idx_tbl->u.pt_pb_idx_tbl_entry = NULL;
            }
            pt_mm_ele_idx_tbl->ui4_start_frame_num = 0;
            pt_mm_ele_idx_tbl->ui4_end_frame_num = pt_mm_ele_idx_tbl->ui4_number_of_entry;
            *ppt_next = pt_mm_ele_idx_tbl;
            ppt_next = (MM_RANGE_ELMT_IDXTBL_T**)&(pt_mm_ele_idx_tbl->pv_next_tbl);
            pt_ele_tbl_desc = pt_ele_tbl_desc->pt_next;
            if (pt_ele_tbl_desc != NULL)
            {
                t_ele_idx_tbl.ui4_tbl_id = pt_ele_tbl_desc->ui4_tbl_id;
            }
        } /* while(pt_ele_tbl_desc != NULL) */
        pt_mm_ele_idx_tbl->pv_next_tbl = NULL;
    }

     if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES &&
        (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF || 
         pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE ||
         pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3))
    {
        z_size = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo,
                                  MINFO_INFO_TYPE_AUDIO_ATTR,
                                  1,
                                  &t_minfo_aud,
                                  &z_size);
        if (i4_ret != MINFOR_OK)
        {
            return SVCTXR_FAIL;
        }
        if(!pt_mm_rang_info->pt_idxtbl_lst)
        {
            pt_mm_rang_info->pt_idxtbl_lst = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
            x_memset(pt_mm_rang_info->pt_idxtbl_lst, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
        }
        else
        {
            if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF)
            {
                pt_mm_rang_info->pt_idxtbl_lst->ui4_align = 
                    (UINT32)((t_minfo_aud.u.t_stm_attribute.u.t_es_audio_stm_attr.ui2_bits_per_sample
                    * t_minfo_aud.u.t_stm_attribute.u.t_es_audio_stm_attr.ui2_channels)>>3);
            }
            if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE)
            {
                pt_mm_rang_info->pt_idxtbl_lst->ui4_align =
                    (UINT32)(t_minfo_aud.u.t_stm_attribute.u.t_avi_audio_stm_attr.i2_block_align);
            }
            if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3 &&
                t_minfo_aud.u.t_stm_attribute.u.t_es_audio_stm_attr.b_big_endian)
            {
                pt_mm_rang_info->pt_idxtbl_lst->ui4_align = (UINT32)16;
            }
        }
    }
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM) 
    {
        if(!pt_mm_rang_info->pt_idxtbl_lst)
        {
            pt_mm_rang_info->pt_idxtbl_lst = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
            x_memset(pt_mm_rang_info->pt_idxtbl_lst, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
        }
        else
        {
            pt_mm_rang_info->pt_idxtbl_lst->ui4_align = (UINT32)16;
        }
    }

#ifdef MM_LAST_MEMORY_SUPPORT
if(pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS)
{
    pt_mm_rang_info->t_start_pos.ui8_stc                    = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_stc;
    pt_mm_rang_info->t_start_pos.ui8_aud_pts_info           = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_aud_pts_info;
    pt_mm_rang_info->t_start_pos.ui8_aud_frame_position     = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_aud_frame_position;
    pt_mm_rang_info->t_start_pos.ui8_i_pts_info             = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_i_pts_info;
    pt_mm_rang_info->t_start_pos.ui8_pts_info               = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_pts_info;
    pt_mm_rang_info->t_start_pos.ui8_i_frame_position       = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_i_frame_position;
    pt_mm_rang_info->t_start_pos.ui8_frame_position         = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_frame_position;
    pt_mm_rang_info->t_start_pos.ui8_frame_position_disp    = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_frame_position_disp;
    pt_mm_rang_info->t_start_pos.i4_temporal_reference      = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.i4_temporal_reference;
    pt_mm_rang_info->t_start_pos.ui2_decoding_order         = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui2_decoding_order;
    pt_mm_rang_info->t_start_pos.ui4_timestap               = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui4_timestap;
}
#endif

#ifdef ENABLE_DIVX_DRM
    pt_mm_rang_info->b_drm = (pt_media_desc->t_drm_type == DRM_TYPE_NONE) ? FALSE : TRUE;
    if (pt_media_desc->b_rtn_frm_rev)    
    {       
        b_rtn_frm_rev = TRUE;       
        pt_media_desc->b_rtn_frm_rev = FALSE;    
    }
    if (pt_media_desc->t_drm_type == DRM_TYPE_DIVX_DRM)
    {
        if(!b_rtn_frm_rev)        
        {
        i4_ret = x_drm_set_info(pt_media_desc->h_drm, DRM_SET_TYPE_DIVX_DRM_PLAYBACK, NULL, 0);
        if (i4_ret != DRMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_drm_set_info(DRM_SET_TYPE_DIVX_DRM_PLAYBACK): DRM error code = %d line%d\n", i4_ret, __LINE__);
            return SVCTXR_FAIL;
        }
        }
        i4_ret = x_drm_get_info(pt_media_desc->h_drm, DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
        if (i4_ret != DRMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_drm_fet_info(DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO): DRM error code = %d line%d\n", i4_ret, __LINE__);
            return SVCTXR_FAIL;
        }

        pt_mm_rang_info->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info = t_divx_drm_info.pt_divx_drm_crypto_info;
        pt_mm_rang_info->t_drm_info_drv.t_drm_type = pt_media_desc->t_drm_type;
        x_dbg_stmt(_INFO_HEADER"mw set drm info by range to driver !!\n");
    }
#endif
#if SUPPORT_MULTI_INFO
    z_size = sizeof(MINFO_INFO_T);
    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_SCALE_INFO,
                              1,
                              &t_scale_info,
                              &z_size);
    if (i4_ret == MINFOR_OK)
    {
        pt_mm_rang_info->pt_next_scale = (MM_RANGE_INFO_T*)
            x_mem_alloc(sizeof(MM_RANGE_INFO_T));
        if(!pt_mm_rang_info->pt_next_scale)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        x_memset(pt_mm_rang_info->pt_next_scale, 0, sizeof(MM_RANGE_INFO_T));
        pt_mm_rang_info = pt_mm_rang_info->pt_next_scale;

        /*fill in scale range info*/
        pt_mm_rang_info->ui8_fileoffset = t_scale_info.u.t_scale_info.ui8_data_offset;
        pt_mm_rang_info->z_range_sz = t_scale_info.u.t_scale_info.ui8_data_sz;
        pt_mm_rang_info->ui8_timecode_scale = t_scale_info.u.t_scale_info.ui8_timecode_scale;
        /*scale key table*/
        t_key_tbl_info.ui4_key_tbl_id = pt_media_desc->h_key_tbl_ext[0];
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        i4_ret = x_midxbuld_get_info(
                                      h_midxbuld,
                                      MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
                                      (VOID*)&t_key_tbl_info,
                                      &z_size);

        if (t_key_tbl_info.pt_key_tbl_ext == NULL ||
            i4_ret != MIDXBULDR_OK)
        {
            x_mem_free(pt_mm_rang_info);
            return SVCTXR_FAIL;
        }
        pt_mm_rang_info->b_key_tbl_exist = TRUE;

        pt_ele_tbl_obj = &(t_key_tbl_info.pt_key_tbl_ext->t_elmt_tbl);
        if (pt_ele_tbl_obj->ui4_number_of_entry == 0)
        {
            pt_mm_rang_info->b_key_tbl_exist = FALSE;
            pt_media_desc->b_is_no_idexing = TRUE;
        }
        else
        {
            pt_mm_ele_idx_tbl_last = NULL;
            while(pt_ele_tbl_obj != NULL)
            {
                /*allocate video key table*/
                pt_mm_ele_idx_tbl_v = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
                if (pt_mm_ele_idx_tbl_v == NULL)
                {
                    x_mem_free(pt_mm_rang_info);
                    return SVCTXR_NO_RESOURCE;
                }
                x_memset(pt_mm_ele_idx_tbl_v, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));

                _mm_hdlr_es_info_cpy(pt_media_desc->t_media_fmt,
                                     pt_mm_ele_idx_tbl_v,
                                     &pt_ele_tbl_obj->u_fmt.t_es.t_info);

                pt_mm_ele_idx_tbl_v->ui4_number_of_entry = pt_ele_tbl_obj->ui4_number_of_entry;
                pt_mm_ele_idx_tbl_v->ui8_base_offset = pt_ele_tbl_obj->ui8_base_offset;
                pt_mm_ele_idx_tbl_v->u.pt_pb_idx_tbl_entry =
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry;
                if (pt_mm_ele_idx_tbl_last == NULL)
                {
                    pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                    pt_mm_rang_info->pt_idxtbl_lst = pt_mm_ele_idx_tbl_v;
                }
                else
                {
                    pt_mm_ele_idx_tbl_last->pt_next_partial = pt_mm_ele_idx_tbl_v;
                    pt_mm_ele_idx_tbl_last = pt_mm_ele_idx_tbl_v;
                }
                pt_ele_tbl_obj = pt_ele_tbl_obj->pt_next;
            }
        }
    }
#endif
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_scdb_rec_get_stream_id
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static UINT32  _mm_hdlr_scdb_rec_get_stream_id(SCDB_REC_T   t_scdb_rec)
{
    UINT32 ui4_stream_id = 0;

    switch(t_scdb_rec.e_rec_type)
    {
    case SCDB_REC_TYPE_AUDIO_AVI:
        ui4_stream_id = t_scdb_rec.u.t_audio_avi.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_AVI:
        ui4_stream_id = t_scdb_rec.u.t_video_avi.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_SUBTITLE_DIVX:
        ui4_stream_id = t_scdb_rec.u.t_subtitle_avi.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_MP4:
        ui4_stream_id = t_scdb_rec.u.t_audio_mp4.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_MP4:
        ui4_stream_id = t_scdb_rec.u.t_video_mp4.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_SUBTITLE_MP4:
        ui4_stream_id = t_scdb_rec.u.t_sp_mp4.ui4_stream_id;
    break;
    case SCDB_REC_TYPE_AUDIO_ASF:
        ui4_stream_id = t_scdb_rec.u.t_audio_asf.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_ASF:
        ui4_stream_id = t_scdb_rec.u.t_video_asf.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_PS:
        ui4_stream_id = t_scdb_rec.u.t_audio_ps.ui1_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_PS:
        ui4_stream_id = t_scdb_rec.u.t_video_ps.ui1_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_MKV:
        ui4_stream_id = t_scdb_rec.u.t_audio_mkv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_MKV:
        ui4_stream_id = t_scdb_rec.u.t_video_mkv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_SUBTITLE_MKV:
        ui4_stream_id = t_scdb_rec.u.t_subtitle_mkv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_RM:
        ui4_stream_id = t_scdb_rec.u.t_audio_mkv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_RM:
        ui4_stream_id = t_scdb_rec.u.t_video_mkv.ui4_stream_id;
        break;
    /*add for flv*/
    case SCDB_REC_TYPE_VIDEO_FLV:
        ui4_stream_id = t_scdb_rec.u.t_video_flv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_FLV:
        ui4_stream_id = t_scdb_rec.u.t_audio_flv.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_OGG:
        ui4_stream_id = t_scdb_rec.u.t_video_ogg.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_OGG:
        ui4_stream_id = t_scdb_rec.u.t_audio_ogg.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_VIDEO_ES:
        ui4_stream_id = t_scdb_rec.u.t_audio_es.ui4_stream_id;
        break;
    case SCDB_REC_TYPE_AUDIO_ES:
        ui4_stream_id = t_scdb_rec.u.t_audio_es.ui4_stream_id;
        break;
    }

    return ui4_stream_id;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_minfo_get_stream_id
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static UINT32  _mm_hdlr_minfo_get_stream_id(MINFO_INFO_T   t_minfo)
{

    UINT32 ui4_stream_id = 0;
    if (t_minfo.t_format.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        switch (t_minfo.t_format.t_media_subtype.u.e_contnr_subtype)
        {
            case MEDIA_CONTNR_SUBTYPE_AVI:
                 ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
                 break;
            case MEDIA_CONTNR_SUBTYPE_ASF:
                 ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_asf_stm_id;
                 break;
            case MEDIA_CONTNR_SUBTYPE_MP4:
                 ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;
                 break;
            case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                 ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
                 break;
            case MEDIA_CONTNR_SUBTYPE_RM:
                ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_rm_stm_id;
                break;
                /*add for flv*/
            case MEDIA_CONTNR_SUBTYPE_FLV:
                ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_flv_stm_id;
                break;
            case MEDIA_CONTNR_SUBTYPE_OGG:
                ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_ogg_stm_id;
                break;
            default:
                  break;
        }
    }
    else if (t_minfo.t_format.e_media_type == MEDIA_TYPE_PS)
    {
        ui4_stream_id = t_minfo.u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
    }
    return ui4_stream_id;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_media_fmt_2_scdb_rec_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static SCDB_REC_TYPE_T _mm_hdlr_media_fmt_2_scdb_rec_type(
                                              MEDIA_FORMAT_T       t_media_fmt,
                                              STREAM_TYPE_T        e_stm_type)
{

    SCDB_REC_TYPE_T t_return_val = SCDB_REC_TYPE_UNKNOWN;

    if ((t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
        && (t_media_fmt.t_media_subtype.u.e_contnr_subtype != MEDIA_CONTNR_SUBTYPE_CMPB))
    {
        switch(t_media_fmt.t_media_subtype.u.e_contnr_subtype)
        {
            case MEDIA_CONTNR_SUBTYPE_UNKNOWN:
                t_return_val =  SCDB_REC_TYPE_UNKNOWN;
                break;
            case MEDIA_CONTNR_SUBTYPE_AVI:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_AVI;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val =  SCDB_REC_TYPE_AUDIO_AVI;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE) || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val =  SCDB_REC_TYPE_SUBTITLE_DIVX;
                }
                else
                {
                    t_return_val =  SCDB_REC_TYPE_UNKNOWN;
                }
                break;
            case MEDIA_CONTNR_SUBTYPE_ASF:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val =  SCDB_REC_TYPE_VIDEO_ASF;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_ASF;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                }
                break;
            case MEDIA_CONTNR_SUBTYPE_MP4:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_MP4;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_MP4;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_MP4;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                }
                break;
            case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_MKV;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_MKV;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_MKV;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                }
                break;
            case MEDIA_CONTNR_SUBTYPE_RM:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_RM;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_RM;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                }
                break;
                /*add for flv*/
            case MEDIA_CONTNR_SUBTYPE_FLV:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_FLV;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_FLV;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                } 
                break;
            case MEDIA_CONTNR_SUBTYPE_OGG:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_OGG;
                }
                else if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_OGG;
                }
                else
                {
                    t_return_val = SCDB_REC_TYPE_UNKNOWN;
                }
                break;
            default :
            return SCDB_REC_TYPE_UNKNOWN;
     } /* switch end */
    }  /* if */
    else if (t_media_fmt.e_media_type == MEDIA_TYPE_PS)
    {
        if (e_stm_type == ST_VIDEO)
        {
            t_return_val = SCDB_REC_TYPE_VIDEO_PS;
        }
        else if (e_stm_type == ST_AUDIO)
        {
            t_return_val = SCDB_REC_TYPE_AUDIO_PS;
        }
        else if ((e_stm_type == ST_MM_SUBTITLE)
              || (e_stm_type == ST_SUBTITLE))
        {
            t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
        }
        else
        {
            t_return_val = SCDB_REC_TYPE_UNKNOWN;
        }
    }
    else if (t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        switch(t_media_fmt.t_media_subtype.u.e_aud_subtype)
        {
            case MEDIA_AUD_SUBTYPE_MP3:
            case MEDIA_AUD_SUBTYPE_MP2:
                if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_MP3;
                }
                else if (e_stm_type == ST_SUBTITLE)
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                break;
            case MEDIA_AUD_SUBTYPE_WMA:
                if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_ASF;
                }
                else if (e_stm_type == ST_SUBTITLE)
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                break;
            case MEDIA_AUD_SUBTYPE_LPCM:
            case MEDIA_AUD_SUBTYPE_AAC:
            case MEDIA_AUD_SUBTYPE_WAVE:  /*add for wave support*/
            case MEDIA_AUD_SUBTYPE_AIF:
            case MEDIA_AUD_SUBTYPE_AC3:
            case MEDIA_AUD_SUBTYPE_DTS:
            case MEDIA_AUD_SUBTYPE_AMR:
            case MEDIA_AUD_SUBTYPE_AWB:
                if (e_stm_type == ST_AUDIO)
                {
                    t_return_val = SCDB_REC_TYPE_AUDIO_ES;
                }
                break;
            default :
                return SCDB_REC_TYPE_UNKNOWN;
        }
    }
    else if ((t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
            ||((t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
                && (t_media_fmt.t_media_subtype.u.e_contnr_subtype
                        == MEDIA_CONTNR_SUBTYPE_CMPB)))
    {
        switch(t_media_fmt.t_media_subtype.u.e_vid_subtype)
        {
            case MEDIA_VID_SUBTYPE_M1V:
            case MEDIA_VID_SUBTYPE_M2V:
            case MEDIA_VID_SUBTYPE_M4V:
            case MEDIA_VID_SUBTYPE_H264:
            case MEDIA_VID_SUBTYPE_VC1:
            case MEDIA_VID_SUBTYPE_AVS:
                if (e_stm_type == ST_VIDEO)
                {
                    t_return_val = SCDB_REC_TYPE_VIDEO_ES;
                }
                else if ((e_stm_type == ST_MM_SUBTITLE)
                      || (e_stm_type == ST_SUBTITLE))
                {
                    t_return_val = SCDB_REC_TYPE_SUBTITLE_EXT;
                }
                break;
            default :
                return SCDB_REC_TYPE_UNKNOWN;
        }
    }
    else
    {
    }

    return t_return_val ;
}

VID_ENC_T _minfo_video_enc_to_scdb_enc(MINFO_INFO_VIDEO_ENC_T e_minfo_enc)
{
    VID_ENC_T   e_scdb_enc;
    
    switch (e_minfo_enc)
    {
    case MINFO_INFO_VID_ENC_MPEG_1:
        e_scdb_enc = VID_ENC_MPEG_1;
        break;
    case MINFO_INFO_VID_ENC_MPEG_2:
        e_scdb_enc = VID_ENC_MPEG_2;
        break;                            
    case MINFO_INFO_VID_ENC_MPEG_4:
        e_scdb_enc = VID_ENC_MPEG_4;
        break;
    case MINFO_INFO_VID_ENC_DIVX_311:
        e_scdb_enc = VID_ENC_DIVX_311;
        break;
    case MINFO_INFO_VID_ENC_DIVX_4:
        e_scdb_enc = VID_ENC_DIVX_4;
        break;
    case MINFO_INFO_VID_ENC_DIVX_5:
        e_scdb_enc = VID_ENC_DIVX_5;
        break;
    case MINFO_INFO_VID_ENC_XVID:
        e_scdb_enc = VID_ENC_MPEG_4;
        break;
    case MINFO_INFO_VID_ENC_WMV1:
        e_scdb_enc = VID_ENC_WMV1;
        break;
    case MINFO_INFO_VID_ENC_WMV2:
        e_scdb_enc = VID_ENC_WMV2;
        break;
    case MINFO_INFO_VID_ENC_WMV3:
        e_scdb_enc = VID_ENC_WMV3;
        break;
    case MINFO_INFO_VID_ENC_WVC1:
        e_scdb_enc = VID_ENC_WVC1;
        break;
    case MINFO_INFO_VID_ENC_H264:
        e_scdb_enc = VID_ENC_H264;
        break;
    case MINFO_INFO_VID_ENC_H263:
        e_scdb_enc = VID_ENC_H263;
        break;
    case MINFO_INFO_VID_ENC_MJPG:
        e_scdb_enc = VID_ENC_MJPEG;
        break;
    case MINFO_INFO_VID_ENC_RV8:
        e_scdb_enc = VID_ENC_RV8;
        break;
    case MINFO_INFO_VID_ENC_RV9:
        e_scdb_enc = VID_ENC_RV9;
        break;
    case MINFO_INFO_VID_ENC_SORENSON:
        e_scdb_enc = VID_ENC_SORENSON;
        break;
    case MINFO_INFO_VID_ENC_NV12:
        e_scdb_enc = VID_ENC_NV12;
        break;
    case MINFO_INFO_VID_ENC_VP6:
        e_scdb_enc = VID_ENC_VP6;
        break;
    case MINFO_INFO_VID_ENC_VP8:
        e_scdb_enc = VID_ENC_VP8;
        break;
    default:
        e_scdb_enc = VID_ENC_UNKNOWN;
        break;
    }

    return e_scdb_enc;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_audio_enc_to_scdb_enc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
AUD_ENC_T _minfo_audio_enc_to_scdb_enc(MINFO_INFO_AUDIO_ENC_T e_minfo_enc)
{
    AUD_ENC_T   e_scdb_enc;

    switch (e_minfo_enc)
    {
    case MINFO_INFO_AUD_ENC_AC3:
    case MINFO_INFO_AUD_ENC_EC3:
        e_scdb_enc = AUD_ENC_AC3;
        break;

    case MINFO_INFO_AUD_ENC_MPEG_1:
        e_scdb_enc = AUD_ENC_MPEG_1;
        break;

    case MINFO_INFO_AUD_ENC_MPEG_2:
        e_scdb_enc = AUD_ENC_MPEG_2;
        break;

    case MINFO_INFO_AUD_ENC_PCM:
    case MINFO_INFO_AUD_ENC_ADPCM:
    case MINFO_INFO_AUD_ENC_DVI_ADPCM:
    case MINFO_INFO_AUD_ENC_A_PCM:
    case MINFO_INFO_AUD_ENC_U_PCM:    
        e_scdb_enc = AUD_ENC_PCM;
        break;

    case MINFO_INFO_AUD_ENC_TV_SYS:
        e_scdb_enc = AUD_ENC_TV_SYS;
        break;

    case MINFO_INFO_AUD_ENC_DTS:
        e_scdb_enc = AUD_ENC_DTS;
        break;

    case MINFO_INFO_AUD_ENC_AAC:
        e_scdb_enc = AUD_ENC_AAC;
        break;

    case MINFO_INFO_AUD_ENC_EU_CANAL_PLUS:
        e_scdb_enc = AUD_ENC_EU_CANAL_PLUS;
        break;

    case MINFO_INFO_AUD_ENC_WMA_V1:
        e_scdb_enc = AUD_ENC_WMA_V1;
        break;

    case MINFO_INFO_AUD_ENC_WMA_V2:
        e_scdb_enc = AUD_ENC_WMA_V2;
        break;

    case MINFO_INFO_AUD_ENC_WMA_V3:
        e_scdb_enc = AUD_ENC_WMA_V3;
        break;

    case MINFO_INFO_AUD_ENC_WMA_PRO:
        e_scdb_enc = AUD_ENC_WMA_PRO;
        break;  
            
    case MINFO_INFO_AUD_ENC_WMA_LOSSLESS:
        e_scdb_enc = AUD_ENC_WMA_LOSSLESS;
        break; 

    case MINFO_INFO_AUD_ENC_LPCM:
        e_scdb_enc = AUD_ENC_LPCM;
        break;

    case MINFO_INFO_AUD_ENC_COOK:
        e_scdb_enc = AUD_ENC_COOK;
        break;

    case MINFO_INFO_AUD_ENC_VORBIS:
        e_scdb_enc = AUD_ENC_VORBIS;
        break;
        
    case MINFO_INFO_AUD_ENC_AMR:
        e_scdb_enc = AUD_ENC_AMR;
        break;

    case MINFO_INFO_AUD_ENC_AWB:
        e_scdb_enc = AUD_ENC_AWB;
        break;
        
    default:
        e_scdb_enc = AUD_ENC_UNKNOWN;
        break;
    }

    return e_scdb_enc;
}


MINFO_INFO_VIDEO_ENC_T mm_hdlr_vid_enc_convert_scdb_to_minfo(VID_ENC_T e_scdb_enc)
{
    MINFO_INFO_VIDEO_ENC_T   e_minfo_enc;
    
    switch (e_scdb_enc)
    {
    case VID_ENC_MPEG_1:
        e_minfo_enc = MINFO_INFO_VID_ENC_MPEG_1;
        break;
    case VID_ENC_MPEG_2:
        e_minfo_enc = MINFO_INFO_VID_ENC_MPEG_2;
        break;                            
    case VID_ENC_MPEG_4:
        e_minfo_enc = MINFO_INFO_VID_ENC_MPEG_4;
        break;
    case VID_ENC_DIVX_311:
        e_minfo_enc = MINFO_INFO_VID_ENC_DIVX_311;
        break;
    case VID_ENC_DIVX_4:
        e_minfo_enc = MINFO_INFO_VID_ENC_DIVX_4;
        break;
    case VID_ENC_DIVX_5:
        e_minfo_enc = MINFO_INFO_VID_ENC_DIVX_5;
        break;
    case VID_ENC_XVID:
        e_minfo_enc = MINFO_INFO_VID_ENC_XVID;
        break;
    case VID_ENC_WMV1:
        e_minfo_enc = MINFO_INFO_VID_ENC_WMV1;
        break;
    case VID_ENC_WMV2:
        e_minfo_enc = MINFO_INFO_VID_ENC_WMV2;
        break;
    case VID_ENC_WMV3:
        e_minfo_enc = MINFO_INFO_VID_ENC_WMV3;
        break;
    case VID_ENC_WVC1:
        e_minfo_enc = MINFO_INFO_VID_ENC_WVC1;
        break;
    case VID_ENC_H264:
        e_minfo_enc = MINFO_INFO_VID_ENC_H264;
        break;
    case VID_ENC_H263:
        e_minfo_enc = MINFO_INFO_VID_ENC_H263;
        break;
    case VID_ENC_MJPEG:
        e_minfo_enc = MINFO_INFO_VID_ENC_MJPG;
        break;
    case VID_ENC_RV8:
        e_minfo_enc = MINFO_INFO_VID_ENC_RV8;
        break;
    case VID_ENC_RV9:
        e_minfo_enc = MINFO_INFO_VID_ENC_RV9;
        break;
    case VID_ENC_SORENSON:
        e_minfo_enc = MINFO_INFO_VID_ENC_SORENSON;
        break;
    case VID_ENC_NV12:
        e_minfo_enc = MINFO_INFO_VID_ENC_NV12;
        break;
    case VID_ENC_VP6:
        e_minfo_enc = MINFO_INFO_VID_ENC_VP6;
        break;
    case VID_ENC_VP8:
        e_minfo_enc = MINFO_INFO_VID_ENC_VP8;
        break;
    default:
        e_minfo_enc = MINFO_INFO_VID_ENC_UNKNOWN;
        break;
    }

    return e_minfo_enc;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_aud_enc_convert_scdb_to_minfo
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
MINFO_INFO_AUDIO_ENC_T mm_hdlr_aud_enc_convert_scdb_to_minfo(AUD_ENC_T e_scdb_enc)
{
    MINFO_INFO_AUDIO_ENC_T e_minfo_enc;

    switch (e_scdb_enc)
    {
    case AUD_ENC_AC3:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AC3;
        break;

    case AUD_ENC_MPEG_1:
        e_minfo_enc = MINFO_INFO_AUD_ENC_MPEG_1;
        break;

    case AUD_ENC_MPEG_2:
        e_minfo_enc = MINFO_INFO_AUD_ENC_MPEG_2;
        break;

    case AUD_ENC_PCM:
        e_minfo_enc = MINFO_INFO_AUD_ENC_PCM;
        break;

    case AUD_ENC_TV_SYS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_TV_SYS;
        break;

    case AUD_ENC_DTS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_DTS;
        break;

    case AUD_ENC_AAC:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AAC;
        break;

    case AUD_ENC_EU_CANAL_PLUS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_EU_CANAL_PLUS;
        break;

    case AUD_ENC_WMA_V1:
        e_minfo_enc = MINFO_INFO_AUD_ENC_WMA_V1;
        break;

    case AUD_ENC_WMA_V2:
        e_minfo_enc = MINFO_INFO_AUD_ENC_WMA_V2;
        break;

    case AUD_ENC_WMA_V3:
        e_minfo_enc = MINFO_INFO_AUD_ENC_WMA_V3;
        break;

    case AUD_ENC_LPCM:
        e_minfo_enc = MINFO_INFO_AUD_ENC_LPCM;
        break;

    case AUD_ENC_COOK:
        e_minfo_enc = MINFO_INFO_AUD_ENC_COOK;
        break;

    case AUD_ENC_VORBIS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_VORBIS;
        break;
        
    case AUD_ENC_AMR:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AMR;

    case AUD_ENC_AWB:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AWB;
        
    default:
        e_minfo_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        break;
    }

    return e_minfo_enc;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_audio_enc_to_ash_enc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
ASH_AUDIO_FMT_T _minfo_audio_enc_to_ash_enc(MINFO_INFO_AUDIO_ENC_T e_minfo_enc)
{
    ASH_AUDIO_FMT_T   e_ash_enc;

    switch (e_minfo_enc)
    {
    case MINFO_INFO_AUD_ENC_AC3:
    case MINFO_INFO_AUD_ENC_EC3:
        e_ash_enc = ASH_AUDIO_FMT_AC3;
        break;

    case MINFO_INFO_AUD_ENC_MPEG_1:
        e_ash_enc = ASH_AUDIO_FMT_MPEG;
        break;

    case MINFO_INFO_AUD_ENC_MPEG_2:
        e_ash_enc = ASH_AUDIO_FMT_MPEG;
        break;

    case MINFO_INFO_AUD_ENC_PCM:
    case MINFO_INFO_AUD_ENC_ADPCM:
    case MINFO_INFO_AUD_ENC_DVI_ADPCM:
    case MINFO_INFO_AUD_ENC_A_PCM:
    case MINFO_INFO_AUD_ENC_U_PCM:    
        e_ash_enc = ASH_AUDIO_FMT_PCM;
        break;

    case MINFO_INFO_AUD_ENC_TV_SYS:
        e_ash_enc = ASH_AUDIO_FMT_TV_SYS;
        break;

    case MINFO_INFO_AUD_ENC_DTS:
        e_ash_enc = ASH_AUDIO_FMT_DTS;
        break;

    case MINFO_INFO_AUD_ENC_AAC:
        e_ash_enc = ASH_AUDIO_FMT_AAC;
        break;

    case MINFO_INFO_AUD_ENC_EU_CANAL_PLUS:
        e_ash_enc = ASH_AUDIO_FMT_EU_CANAL_PLUS;
        break;

    case MINFO_INFO_AUD_ENC_WMA_V1:
    case MINFO_INFO_AUD_ENC_WMA_V2:
    case MINFO_INFO_AUD_ENC_WMA_V3:
        e_ash_enc = ASH_AUDIO_FMT_WMA;
        break;

    case MINFO_INFO_AUD_ENC_WMA_PRO:  
        e_ash_enc = ASH_AUDIO_FMT_WMAPRO;
        break;

    case MINFO_INFO_AUD_ENC_WMA_LOSSLESS:  
        e_ash_enc = ASH_AUDIO_FMT_WMALOSSLESS;
        break;        

    case MINFO_INFO_AUD_ENC_LPCM:
        e_ash_enc = ASH_AUDIO_FMT_PCM;
        break;

    case MINFO_INFO_AUD_ENC_COOK:
        e_ash_enc = ASH_AUDIO_FMT_COOK;
        break;

    case MINFO_INFO_AUD_ENC_VORBIS:
        e_ash_enc = ASH_AUDIO_FMT_VORBIS;
        break;
        
    case MINFO_INFO_AUD_ENC_AMR:
        e_ash_enc = ASH_AUDIO_FMT_AMR;
        break;

    case MINFO_INFO_AUD_ENC_AWB:
        e_ash_enc = ASH_AUDIO_FMT_AWB;
        break;
        
    default:
        e_ash_enc = ASH_AUDIO_FMT_UNKNOWN;
        break;
    }
    return e_ash_enc;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_audio_enc_to_ash_enc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
MINFO_INFO_AUDIO_ENC_T mm_hdlr_aud_enc_convert_ash_to_minfo(ASH_AUDIO_FMT_T   e_ash_enc)
{
    MINFO_INFO_AUDIO_ENC_T e_minfo_enc;

    switch (e_ash_enc)
    {
    case ASH_AUDIO_FMT_AC3:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AC3;
        break;

    case ASH_AUDIO_FMT_MPEG:
        e_minfo_enc = MINFO_INFO_AUD_ENC_MPEG_2;
        break;

    case ASH_AUDIO_FMT_PCM:
        e_minfo_enc = MINFO_INFO_AUD_ENC_PCM;
        break;

    case ASH_AUDIO_FMT_TV_SYS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_TV_SYS;
        break;

    case ASH_AUDIO_FMT_DTS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_DTS;
        break;

    case ASH_AUDIO_FMT_AAC:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AAC;
        break;

    case ASH_AUDIO_FMT_EU_CANAL_PLUS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_EU_CANAL_PLUS;
        break;

    case ASH_AUDIO_FMT_WMA:
        e_minfo_enc =MINFO_INFO_AUD_ENC_WMA_V3 ;
        break;

    case ASH_AUDIO_FMT_COOK:
        e_minfo_enc = MINFO_INFO_AUD_ENC_COOK;
        break;

    case ASH_AUDIO_FMT_VORBIS:
        e_minfo_enc = MINFO_INFO_AUD_ENC_VORBIS;
        break;
        
    case ASH_AUDIO_FMT_AMR:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AMR;
        break;

    case ASH_AUDIO_FMT_AWB:
        e_minfo_enc = MINFO_INFO_AUD_ENC_AWB;
        break;
        
    default:
        e_minfo_enc = MINFO_INFO_AUD_ENC_UNKNOWN;
        break;
    }
    return e_minfo_enc;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_mkv_subtitle_type_to_scdb_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
MKV_SBTL_TYPE_T _minfo_mkv_subtitle_type_to_scdb_type(MINFO_INFO_SUBTITLE_TYPE_T e_minfo_type)
{
    MKV_SBTL_TYPE_T   e_scdb_sp_type;

    switch (e_minfo_type)
    {
    case MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8:
        e_scdb_sp_type = MKV_SBTL_SRT;
        break;

    case MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA:
        e_scdb_sp_type = MKV_SBTL_SSA;
        break;

    case MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB:
        e_scdb_sp_type = MKV_SBTL_IMAGE;
        break;

    case MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP:
        e_scdb_sp_type = MKV_SBTL_IMAGE;
        break;

    default:
        e_scdb_sp_type = (MKV_SBTL_TYPE_T)0;
        break;
    }

    return e_scdb_sp_type;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_audio_enc_to_pcm_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
ASH_AUDIO_PCM_TYPE_T _minfo_audio_enc_to_pcm_type(MINFO_INFO_AUDIO_ENC_T  e_enc)
{
    ASH_AUDIO_PCM_TYPE_T  pcm_type = ASH_AUDIO_PCM_TYPE_NORMAL;

    switch(e_enc)
    {
    case MINFO_INFO_AUD_ENC_ADPCM:
        pcm_type = ASH_AUDIO_PCM_TYPE_MS_ADPCM;
        break;
    case MINFO_INFO_AUD_ENC_DVI_ADPCM:
        pcm_type = ASH_AUDIO_PCM_TYPE_IMA_ADPCM;
        break;
    case MINFO_INFO_AUD_ENC_A_PCM:
        pcm_type = ASH_AUDIO_PCM_TYPE_A_PCM;
        break;
    case MINFO_INFO_AUD_ENC_U_PCM:
        pcm_type = ASH_AUDIO_PCM_TYPE_U_PCM;
        break;
    case MINFO_INFO_AUD_ENC_PCM:
    default:
        pcm_type = ASH_AUDIO_PCM_TYPE_NORMAL;
        break;
    }

    return pcm_type;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_avi_strm_rec_to_scdb_avi_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_avi_strm_rec_to_scdb_avi_strm_rec(
                                                  MINFO_INFO_T      t_minfo_rec,
                                                  SCDB_REC_T*       pt_scdb_rec)

{
    MINFO_AVI_AUDIO_STM_ATRBT_T t_minfo_avi_audio =
    t_minfo_rec.u.t_stm_attribute.u.t_avi_audio_stm_attr;
    MINFO_AVI_VIDEO_STM_ATRBT_T t_minfo_avi_video =
    t_minfo_rec.u.t_stm_attribute.u.t_avi_video_stm_attr;

    MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T t_minfo_avi_sp =
      t_minfo_rec.u.t_stm_attribute.u.t_avi_sp_stm_attr;

    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_avi.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
        pt_scdb_rec->u.t_audio_avi.ui4_scale = t_minfo_avi_audio.ui4_scale;
        pt_scdb_rec->u.t_audio_avi.ui4_rate = t_minfo_avi_audio.ui4_rate;
        pt_scdb_rec->u.t_audio_avi.e_enc =
            _minfo_audio_enc_to_scdb_enc(t_minfo_avi_audio.e_enc);
        pt_scdb_rec->u.t_audio_avi.e_layer = t_minfo_avi_audio.e_layer;
        pt_scdb_rec->u.t_audio_avi.ui4_scale = t_minfo_avi_audio.ui4_scale;
        pt_scdb_rec->u.t_audio_avi.ui4_sample_size =
            t_minfo_avi_audio.ui4_sample_size;
        pt_scdb_rec->u.t_audio_avi.i2_channels = t_minfo_avi_audio.i2_channels;
        pt_scdb_rec->u.t_audio_avi.i4_samples_per_sec =
            t_minfo_avi_audio.i4_samples_per_sec;
        pt_scdb_rec->u.t_audio_avi.i2_block_align = t_minfo_avi_audio.i2_block_align;
        pt_scdb_rec->u.t_audio_avi.i2_bits_per_sample =
            t_minfo_avi_audio.i2_bits_per_sample;
        pt_scdb_rec->u.t_audio_avi.b_default =
            t_minfo_avi_audio.b_default;

        x_memcpy(pt_scdb_rec->u.t_audio_avi.s_lang, t_minfo_avi_audio.s_lang,ISO_639_LANG_LEN);
    }
    /*video data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_avi.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
        pt_scdb_rec->u.t_video_avi.ui4_scale = t_minfo_avi_video.ui4_scale;
        pt_scdb_rec->u.t_video_avi.ui4_rate = t_minfo_avi_video.ui4_rate;
        //pt_scdb_rec->u.t_video_avi.e_enc = t_minfo_avi_video.e_enc;
        pt_scdb_rec->u.t_video_avi.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_avi_video.e_enc);
        if( t_minfo_avi_video.e_enc == MINFO_INFO_VID_ENC_MJPG)
        {
            pt_scdb_rec->u.t_video_avi.e_enc = VID_ENC_MJPEG;
        }
        pt_scdb_rec->u.t_video_avi.ui4_sample_size =
            t_minfo_avi_video.ui4_sample_size;
        pt_scdb_rec->u.t_video_avi.i4_width = t_minfo_avi_video.i4_width;
        pt_scdb_rec->u.t_video_avi.i4_height = t_minfo_avi_video.i4_height;
        pt_scdb_rec->u.t_video_avi.b_default = t_minfo_avi_video.b_default;
    }
    /*subpicture data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        pt_scdb_rec->u.t_subtitle_avi.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
        pt_scdb_rec->u.t_subtitle_avi.ui4_scale =
            t_minfo_avi_sp.ui4_scale;
        pt_scdb_rec->u.t_subtitle_avi.ui4_rate =
            t_minfo_avi_sp.ui4_rate;
        pt_scdb_rec->u.t_subtitle_avi.ui4_subtitle_type =
            t_minfo_avi_sp.ui4_subtitle_type;
        pt_scdb_rec->u.t_subtitle_avi.ui4_sample_size =
            t_minfo_avi_sp.ui4_sample_size;
        pt_scdb_rec->u.t_subtitle_avi.i4_width =
            t_minfo_avi_sp.i4_width;
        pt_scdb_rec->u.t_subtitle_avi.i4_height =
            t_minfo_avi_sp.i4_height;
        pt_scdb_rec->u.t_subtitle_avi.b_default =
            t_minfo_avi_sp.b_default;
        x_memcpy(pt_scdb_rec->u.t_subtitle_avi.s_lang, t_minfo_avi_sp.s_lang, ISO_639_LANG_LEN);
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_wave_strm_rec_to_scdb_wave_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
 /*add for wave support*/
static INT32 _minfo_wave_strm_rec_to_scdb_wave_strm_rec(
                                            MINFO_INFO_T        t_minfo_rec,
                                            SCDB_REC_T*         pt_scdb_rec)
{
    MINFO_AVI_AUDIO_STM_ATRBT_T t_minfo_avi_audio =
    t_minfo_rec.u.t_stm_attribute.u.t_avi_audio_stm_attr;

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc =
            _minfo_audio_enc_to_scdb_enc(t_minfo_avi_audio.e_enc);
        pt_scdb_rec->ui4_private = 0;
        pt_scdb_rec->e_rec_type  = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _minfo_aac_strm_rec_to_scdb_aac_strm_rec
 * Description:
 * Input arguments:
 * Output arguments:
 *        None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_aac_strm_rec_to_scdb_aac_strm_rec(MINFO_INFO_T    t_minfo_rec,
                                                      SCDB_REC_T*     pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_AAC;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _minfo_amr_strm_rec_to_scdb_amr_strm_rec
 * Description:
 * Input arguments:
 * Output arguments:
 *        None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_amr_strm_rec_to_scdb_amr_strm_rec(MINFO_INFO_T    t_minfo_rec,
                                                      SCDB_REC_T*     pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_AMR;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _minfo_awb_strm_rec_to_scdb_awb_strm_rec
 * Description:
 * Input arguments:
 * Output arguments:
 *        None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_awb_strm_rec_to_scdb_awb_strm_rec(MINFO_INFO_T    t_minfo_rec,
                                                      SCDB_REC_T*     pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_AWB;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}

static INT32 _minfo_pcm_strm_rec_to_scdb_pcm_strm_rec(MINFO_INFO_T   t_minfo_rec,
                                                      SCDB_REC_T*    pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_PCM;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_ac3_strm_rec_to_scdb_ac3_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_ac3_strm_rec_to_scdb_ac3_strm_rec(MINFO_INFO_T   t_minfo_rec,
                                                      SCDB_REC_T*    pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_AC3;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_dts_strm_rec_to_scdb_dts_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_dts_strm_rec_to_scdb_dts_strm_rec(MINFO_INFO_T   t_minfo_rec,
                                                      SCDB_REC_T*    pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc         = AUD_ENC_DTS;
        pt_scdb_rec->ui4_private                = 0;
        pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_aif_strm_rec_to_scdb_aif_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
 /*add for wave support*/
static INT32 _minfo_aif_strm_rec_to_scdb_aif_strm_rec(
                                            MINFO_INFO_T        t_minfo_rec,
                                            SCDB_REC_T*         pt_scdb_rec)
{
    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
        pt_scdb_rec->u.t_audio_es.e_enc = AUD_ENC_LPCM;
        pt_scdb_rec->ui4_private = 0;
        pt_scdb_rec->e_rec_type  = SCDB_REC_TYPE_AUDIO_ES;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_asf_strm_rec_to_scdb_asf_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_asf_strm_rec_to_scdb_asf_strm_rec(
                                            MINFO_INFO_T        t_minfo_rec,
                                            SCDB_REC_T*         pt_scdb_rec)
{
    MINFO_ASF_AUDIO_STM_ATRBT_T t_minfo_asf_audio =
    t_minfo_rec.u.t_stm_attribute.u.t_asf_audio_stm_attr;
    MINFO_ASF_VIDEO_STM_ATRBT_T t_minfo_asf_video =
    t_minfo_rec.u.t_stm_attribute.u.t_asf_video_stm_attr;

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_asf.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_asf_stm_id;
        pt_scdb_rec->u.t_audio_asf.e_enc =
            _minfo_audio_enc_to_scdb_enc(t_minfo_asf_audio.e_enc);
        pt_scdb_rec->u.t_audio_asf.e_layer =
            t_minfo_asf_audio.e_layer;
        pt_scdb_rec->u.t_audio_asf.i2_channels =
            t_minfo_asf_audio.i2_channels;
        pt_scdb_rec->u.t_audio_asf.i4_samples_per_sec =
            t_minfo_asf_audio.i4_samples_per_sec;
        pt_scdb_rec->u.t_audio_asf.i2_block_align =
            t_minfo_asf_audio.i2_block_align;
        pt_scdb_rec->u.t_audio_asf.i2_bits_per_sample =
            t_minfo_asf_audio.i2_bits_per_sample;
        /*pt_scdb_rec->u.t_audio_asf.pv_codec_info =
            (VOID*)t_minfo_asf_audio.pui1_codec_info;
        pt_scdb_rec->u.t_audio_asf.ui4_codec_info_sz =
            t_minfo_asf_audio.ui4_codec_info_size;*/
        pt_scdb_rec->u.t_audio_asf.b_default =
            t_minfo_asf_audio.b_default;
        x_memcpy(pt_scdb_rec->u.t_audio_asf.s_lang, t_minfo_asf_audio.s_lang, ISO_639_LANG_LEN);
    }
    /*video data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_asf.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_asf_stm_id;
        //pt_scdb_rec->u.t_video_asf.e_enc = t_minfo_asf_video.e_enc;
        pt_scdb_rec->u.t_video_asf.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_asf_video.e_enc);
        pt_scdb_rec->u.t_video_asf.i4_width =
            t_minfo_asf_video.i4_width;
        pt_scdb_rec->u.t_video_asf.i4_height =
            t_minfo_asf_video.i4_height;
        pt_scdb_rec->u.t_video_asf.b_default =
            t_minfo_asf_video.b_default;
    }
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        return SVCTXR_OK;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_mp4_strm_rec_to_scdb_mp4_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_mp4_strm_rec_to_scdb_mp4_strm_rec(
                                           MINFO_INFO_T t_minfo_rec,
                                           SCDB_REC_T*  pt_scdb_rec)
{
    MINFO_MP4_AUDIO_STM_ATRBT_T t_minfo_mp4_audio =
    t_minfo_rec.u.t_stm_attribute.u.t_mp4i_audio_stm_attr;
    MINFO_MP4_VIDEO_STM_ATRBT_T t_minfo_mp4_video =
    t_minfo_rec.u.t_stm_attribute.u.t_mp4i_video_stm_attr;
    MINFO_MP4_SUBTITLE_STM_ATRBT_T t_minfo_mp4_sp = 
    t_minfo_rec.u.t_stm_attribute.u.t_mp4_sp_stm_attr;     

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }
    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_mp4.ui4_stream_id =
         t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;
        pt_scdb_rec->u.t_audio_mp4.e_enc =
         _minfo_audio_enc_to_scdb_enc(t_minfo_mp4_audio.e_enc);
        pt_scdb_rec->u.t_audio_mp4.e_layer =
         t_minfo_mp4_audio.e_layer;
        pt_scdb_rec->u.t_audio_mp4.i2_channels =
         t_minfo_mp4_audio.i2_channels;
        pt_scdb_rec->u.t_audio_mp4.i4_sample_rate =
         t_minfo_mp4_audio.i4_sample_rate ;
        pt_scdb_rec->u.t_audio_mp4.i4_sample_size =
         t_minfo_mp4_audio.i4_sample_size;
        pt_scdb_rec->u.t_audio_asf.b_default =
         t_minfo_mp4_audio.b_default;
    }
    /*video data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_mp4.ui4_stream_id =
         t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;
        //pt_scdb_rec->u.t_video_mp4.e_enc = t_minfo_mp4_video.e_enc;
        pt_scdb_rec->u.t_video_mp4.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_mp4_video.e_enc);
        /*It might not need e_vid_fmt info*/
        /*pt_scdb_rec->u.t_video_mp4.e_vid_fmt*/
        pt_scdb_rec->u.t_video_mp4.i4_width =
         t_minfo_mp4_video.i4_width;
        pt_scdb_rec->u.t_video_mp4.i4_height =
         t_minfo_mp4_video.i4_height;
        pt_scdb_rec->u.t_video_mp4.b_default =
         t_minfo_mp4_video.b_default;
    }
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        pt_scdb_rec->u.t_sp_mp4.ui4_stream_id =
         t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;        
        pt_scdb_rec->u.t_sp_mp4.i4_width =
         t_minfo_mp4_sp.i4_width;
        pt_scdb_rec->u.t_sp_mp4.i4_height =
         t_minfo_mp4_sp.i4_height;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_mkv_strm_rec_to_scdb_mkv_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _minfo_mkv_strm_rec_to_scdb_mkv_strm_rec(
                                                  MINFO_INFO_T      t_minfo_rec,
                                                  SCDB_REC_T*       pt_scdb_rec)

{
    MINFO_MKV_AUDIO_STM_ATRBT_T       t_minfo_mkv_audio;
    MINFO_MKV_VIDEO_STM_ATRBT_T       t_minfo_mkv_video;
    MINFO_MKV_SUBTITLE_STM_ATRBT_T    t_minfo_mkv_sp;

    t_minfo_mkv_audio = t_minfo_rec.u.t_stm_attribute.u.t_mkv_audio_stm_attr;
    t_minfo_mkv_video = t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr;
    t_minfo_mkv_sp = t_minfo_rec.u.t_stm_attribute.u.t_mkv_sp_stm_attr;

    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_mkv.ui4_stream_id
            = t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_scdb_rec->u.t_audio_mkv.ui4_scale = t_minfo_mkv_audio.ui4_scale;
        pt_scdb_rec->u.t_audio_mkv.ui4_rate = t_minfo_mkv_audio.ui4_rate;
        pt_scdb_rec->u.t_audio_mkv.e_enc
            = _minfo_audio_enc_to_scdb_enc(t_minfo_mkv_audio.e_enc);
        pt_scdb_rec->u.t_audio_mkv.e_layer = t_minfo_mkv_audio.e_layer;
        pt_scdb_rec->u.t_audio_mkv.e_pcm_align = t_minfo_mkv_audio.e_align;
        pt_scdb_rec->u.t_audio_mkv.i2_channels = t_minfo_mkv_audio.i2_channels;
        pt_scdb_rec->u.t_audio_mkv.i4_samples_per_sec
            = t_minfo_mkv_audio.i4_samples_per_sec;
        pt_scdb_rec->u.t_audio_mkv.i2_block_align = t_minfo_mkv_audio.i2_block_align;
        pt_scdb_rec->u.t_audio_mkv.i2_bits_per_sample
            = t_minfo_mkv_audio.i2_bits_per_sample;
        pt_scdb_rec->u.t_audio_mkv.b_default = t_minfo_mkv_audio.b_default;
        x_memcpy(pt_scdb_rec->u.t_audio_mkv.s_lang,
                t_minfo_mkv_audio.s_lang,
                ISO_639_LANG_LEN);
    }
    /*video data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_mkv.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_scdb_rec->u.t_video_mkv.ui4_scale = t_minfo_mkv_video.ui4_scale;
        pt_scdb_rec->u.t_video_mkv.ui4_rate = t_minfo_mkv_video.ui4_rate;
        //pt_scdb_rec->u.t_video_mkv.e_enc = t_minfo_mkv_video.e_enc;
        pt_scdb_rec->u.t_video_mkv.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_mkv_video.e_enc);
        pt_scdb_rec->u.t_video_mkv.i4_width = t_minfo_mkv_video.i4_width;
        pt_scdb_rec->u.t_video_mkv.i4_height = t_minfo_mkv_video.i4_height;
        pt_scdb_rec->u.t_video_mkv.b_default = t_minfo_mkv_video.b_default;
    }
    /*subpicture data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        pt_scdb_rec->u.t_subtitle_mkv.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
        pt_scdb_rec->u.t_subtitle_mkv.ui4_scale = t_minfo_mkv_sp.ui4_scale;
        pt_scdb_rec->u.t_subtitle_mkv.ui4_rate = t_minfo_mkv_sp.ui4_rate;
        pt_scdb_rec->u.t_subtitle_mkv.e_sbtl_type
            = _minfo_mkv_subtitle_type_to_scdb_type(t_minfo_mkv_sp.e_subtitle_type);
        pt_scdb_rec->u.t_subtitle_mkv.b_default = t_minfo_mkv_sp.b_default;
        x_memcpy(pt_scdb_rec->u.t_subtitle_mkv.s_lang,
                t_minfo_mkv_sp.s_lang,
                ISO_639_LANG_LEN);
    }
    return SVCTXR_OK;
}

/*add for flv*/

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_flv_strm_rec_to_scdb_flv_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _minfo_flv_strm_rec_to_scdb_flv_strm_rec(
                                                  MINFO_INFO_T      t_minfo_rec,
                                                  SCDB_REC_T*       pt_scdb_rec)
{
    MINFO_FLV_AUDIO_STM_ATRBT_T       t_minfo_flv_audio;
    MINFO_FLV_VIDEO_STM_ATRBT_T       t_minfo_flv_video;

    t_minfo_flv_audio = t_minfo_rec.u.t_stm_attribute.u.t_flv_audio_stm_attr;
    t_minfo_flv_video = t_minfo_rec.u.t_stm_attribute.u.t_flv_video_stm_attr;

    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }


    /*audio data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_flv.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
        pt_scdb_rec->u.t_audio_flv.e_enc
            = _minfo_audio_enc_to_scdb_enc(t_minfo_flv_audio.e_enc);
        pt_scdb_rec->u.t_audio_flv.e_layer = t_minfo_flv_audio.e_layer;
        pt_scdb_rec->u.t_audio_flv.ui4_scale =  1;
        pt_scdb_rec->u.t_audio_flv.ui4_duration = (UINT32)t_minfo_flv_audio.ui8_duration;
        pt_scdb_rec->u.t_audio_flv.ui4_rate = t_minfo_flv_audio.ui4_avg_bytes_per_sec;
        pt_scdb_rec->u.t_audio_flv.i2_sample_size =  t_minfo_flv_audio.i2_bits_per_sample;
        pt_scdb_rec->u.t_audio_flv.i2_channels =  t_minfo_flv_audio.i2_channels;
        pt_scdb_rec->u.t_audio_flv.ui4_start_pts =  t_minfo_flv_audio.ui4_start_pts;
        pt_scdb_rec->u.t_audio_flv.i4_samples_per_sec =  t_minfo_flv_audio.i4_samples_per_sec;
    }
    /*video data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_flv.ui4_stream_id =
            t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_avi_stm_id;
        pt_scdb_rec->u.t_video_flv.e_enc
            = _minfo_video_enc_to_scdb_enc(t_minfo_flv_video.e_enc);
        pt_scdb_rec->u.t_video_flv.ui4_scale =  t_minfo_flv_video.ui4_scale;
        pt_scdb_rec->u.t_video_flv.ui4_duration = (UINT32)t_minfo_flv_video.ui8_duration;
        pt_scdb_rec->u.t_video_flv.ui4_rate =  t_minfo_flv_video.ui4_rate;
        pt_scdb_rec->u.t_video_flv.ui4_start_pts = t_minfo_flv_video.ui4_start_pts;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_rm_strm_rec_to_scdb_rm_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _minfo_rm_strm_rec_to_scdb_rm_strm_rec(
                                                  MINFO_INFO_T      t_minfo_rec,
                                                  SCDB_REC_T*       pt_scdb_rec)
{
    MINFO_RM_AUDIO_STM_ATRBT_T       t_minfo_rm_audio;
    MINFO_RM_VIDEO_STM_ATRBT_T       t_minfo_rm_video;

    t_minfo_rm_audio = t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr;
    t_minfo_rm_video = t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr;

    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_rm.ui4_stream_id = t_minfo_rm_audio.ui2_strm_num;
        pt_scdb_rec->u.t_audio_rm.e_enc
            = _minfo_audio_enc_to_scdb_enc(t_minfo_rm_audio.e_enc);
        pt_scdb_rec->u.t_audio_rm.e_layer = 0;
        pt_scdb_rec->u.t_audio_rm.ui4_preroll =  t_minfo_rm_audio.ui4_preroll;
        pt_scdb_rec->u.t_audio_rm.ui4_duration =  t_minfo_rm_audio.ui4_duration;
        pt_scdb_rec->u.t_audio_rm.ui4_max_bit_rate =  t_minfo_rm_audio.ui4_max_bit_rate;
        pt_scdb_rec->u.t_audio_rm.ui4_avg_bit_rate =  t_minfo_rm_audio.ui4_avg_bit_rate;
        pt_scdb_rec->u.t_audio_rm.ui4_max_packet_size =  t_minfo_rm_audio.ui4_max_packet_size;
        pt_scdb_rec->u.t_audio_rm.ui4_avg_packet_size =  t_minfo_rm_audio.ui4_avg_packet_size;
        pt_scdb_rec->u.t_audio_rm.ui4_start_time =  t_minfo_rm_audio.ui4_start_time;
        pt_scdb_rec->u.t_audio_rm.ui2_version =  t_minfo_rm_audio.ui2_version;
        pt_scdb_rec->u.t_audio_rm.ui2_revision =  t_minfo_rm_audio.ui2_revision;
        pt_scdb_rec->u.t_audio_rm.ui2_hdr_bytes =  t_minfo_rm_audio.ui2_hdr_bytes;
        pt_scdb_rec->u.t_audio_rm.ui2_flavor_idx =  t_minfo_rm_audio.ui2_flavor_idx;
        pt_scdb_rec->u.t_audio_rm.ui4_granularity =  t_minfo_rm_audio.ui4_granularity;
        pt_scdb_rec->u.t_audio_rm.ui4_total_bytes =  t_minfo_rm_audio.ui4_total_bytes;
        pt_scdb_rec->u.t_audio_rm.ui4_bytes_per_min =  t_minfo_rm_audio.ui4_bytes_per_min;
        pt_scdb_rec->u.t_audio_rm.ui4_bytes_per_min2 =  t_minfo_rm_audio.ui4_bytes_per_min2;
        pt_scdb_rec->u.t_audio_rm.ui2_intlv_factor =  t_minfo_rm_audio.ui2_intlv_factor;
        pt_scdb_rec->u.t_audio_rm.ui2_intlv_blk_sz =  t_minfo_rm_audio.ui2_intlv_blk_sz;
        pt_scdb_rec->u.t_audio_rm.ui2_codec_frm_sz =  t_minfo_rm_audio.ui2_codec_frm_sz;
        pt_scdb_rec->u.t_audio_rm.ui4_user_data =  t_minfo_rm_audio.ui4_user_data;
        pt_scdb_rec->u.t_audio_rm.ui4_sample_rate =  t_minfo_rm_audio.ui4_sample_rate;
        pt_scdb_rec->u.t_audio_rm.ui4_act_sample_rate =  t_minfo_rm_audio.ui4_act_sample_rate;
        pt_scdb_rec->u.t_audio_rm.ui2_sample_sz =  t_minfo_rm_audio.ui2_sample_sz;
        pt_scdb_rec->u.t_audio_rm.ui2_num_channel =  t_minfo_rm_audio.ui2_sample_sz;
        pt_scdb_rec->u.t_audio_rm.ui4_intlvr_id =  t_minfo_rm_audio.ui4_intlvr_id;
        pt_scdb_rec->u.t_audio_rm.ui4_codec_id =  t_minfo_rm_audio.ui4_codec_id;
        pt_scdb_rec->u.t_audio_rm.ui1_intlv_flag =  t_minfo_rm_audio.ui1_intlv_flag;
        pt_scdb_rec->u.t_audio_rm.ui1_can_copy_flag =  t_minfo_rm_audio.ui1_can_copy_flag;
        pt_scdb_rec->u.t_audio_rm.ui1_strm_type =  t_minfo_rm_audio.ui1_strm_type;
        pt_scdb_rec->u.t_audio_rm.ui1_has_intlv_ptrn =  t_minfo_rm_audio.ui1_has_intlv_ptrn;
        pt_scdb_rec->u.t_audio_rm.pv_intlv_ptrn =  t_minfo_rm_audio.pv_intlv_ptrn;
        pt_scdb_rec->u.t_audio_rm.ui4_opaque_data_sz =  t_minfo_rm_audio.ui4_opaque_data_sz;
        pt_scdb_rec->u.t_audio_rm.pv_codec_opaque_data =  t_minfo_rm_audio.pv_codec_opaque_data;
    }
    /*video data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_rm.ui4_stream_id = t_minfo_rm_video.ui2_strm_num;
        //pt_scdb_rec->u.t_video_rm.e_enc = t_minfo_rm_video.e_enc;
        pt_scdb_rec->u.t_video_rm.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_rm_video.e_enc);
        pt_scdb_rec->u.t_video_rm.ui4_preroll =  t_minfo_rm_video.ui4_preroll;
        pt_scdb_rec->u.t_video_rm.ui4_duration =  t_minfo_rm_video.ui4_duration;
        pt_scdb_rec->u.t_video_rm.ui4_max_bit_rate =  t_minfo_rm_video.ui4_max_bit_rate;
        pt_scdb_rec->u.t_video_rm.ui4_avg_bit_rate =  t_minfo_rm_video.ui4_avg_bit_rate;
        pt_scdb_rec->u.t_video_rm.ui4_max_packet_size =  t_minfo_rm_video.ui4_max_packet_size;
        pt_scdb_rec->u.t_video_rm.ui4_avg_packet_size =  t_minfo_rm_video.ui4_avg_packet_size;
        pt_scdb_rec->u.t_video_rm.ui4_start_time =  t_minfo_rm_video.ui4_start_time;
        pt_scdb_rec->u.t_video_rm.ui4_codec_id =  t_minfo_rm_video.ui4_codec_id;
        pt_scdb_rec->u.t_video_rm.ui2_frm_width =  t_minfo_rm_video.ui2_frm_width;
        pt_scdb_rec->u.t_video_rm.ui2_frm_height =  t_minfo_rm_video.ui2_frm_height;
        pt_scdb_rec->u.t_video_rm.ui2_bit_count =  t_minfo_rm_video.ui2_bit_count;
        pt_scdb_rec->u.t_video_rm.ui2_pad_width =  t_minfo_rm_video.ui2_pad_width;
        pt_scdb_rec->u.t_video_rm.ui2_pad_height =  t_minfo_rm_video.ui2_pad_height;
        pt_scdb_rec->u.t_video_rm.ui4_frm_rate =  t_minfo_rm_video.ui4_frm_rate;
        pt_scdb_rec->u.t_video_rm.ui4_spo_extra_flags =  t_minfo_rm_video.ui4_spo_extra_flags;
        pt_scdb_rec->u.t_video_rm.ui4_specific =  t_minfo_rm_video.ui4_specific;
        pt_scdb_rec->u.t_video_rm.ui1_ecc_mask =  t_minfo_rm_video.ui1_ecc_mask;
        pt_scdb_rec->u.t_video_rm.ui4_version =  t_minfo_rm_video.ui4_version;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
* Name
 *      _minfo_ps_strm_rec_to_scdb_ps_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_ps_strm_rec_to_scdb_ps_strm_rec(MINFO_INFO_T    t_minfo_rec,
                                                    SCDB_REC_T*     pt_scdb_rec)
{
    MINFO_PS_AUDIO_STM_ATRBT_T t_minfo_ps_audio;
    MINFO_PS_VIDEO_STM_ATRBT_T t_minfo_ps_video;
    MINFO_PS_SUBTITLE_STM_ATRBT_T   t_minfo_ps_subtitle;
    if (pt_scdb_rec ==NULL)
    {
       return SVCTXR_INV_ARG;
    }

    t_minfo_ps_audio = t_minfo_rec.u.t_stm_attribute.u.t_ps_audio_stm_attr;
    t_minfo_ps_video = t_minfo_rec.u.t_stm_attribute.u.t_ps_video_stm_attr;
    t_minfo_ps_subtitle = t_minfo_rec.u.t_stm_attribute.u.t_ps_sp_stm_attr;
    /*audio data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        /*QQQ*/
        pt_scdb_rec->u.t_audio_ps.ui1_stream_id =
            (UINT8)t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id;

        pt_scdb_rec->u.t_audio_ps.ui1_sub_stream_id =
            (UINT8)t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id;

        pt_scdb_rec->u.t_audio_ps.e_enc =
            _minfo_audio_enc_to_scdb_enc(t_minfo_ps_audio.e_enc);

        pt_scdb_rec->u.t_audio_ps.e_layer =
            t_minfo_ps_audio.e_layer;

        pt_scdb_rec->u.t_audio_ps.i2_channels =
            t_minfo_ps_audio.i2_channels;

        pt_scdb_rec->u.t_audio_ps.i4_samples_per_quant =
            t_minfo_ps_audio.i4_samples_per_quant;

        pt_scdb_rec->u.t_audio_ps.i4_sampling_frequency =
            t_minfo_ps_audio.i4_sampling_frequency;

        pt_scdb_rec->u.t_audio_ps.b_default =
            t_minfo_ps_audio.b_default;
    }
    /*video data*/
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_ps.ui1_stream_id =
            (UINT8)t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_ps_stm_id.ui1_stm_id;

        //pt_scdb_rec->u.t_video_ps.e_enc = t_minfo_ps_video.e_enc;    
        pt_scdb_rec->u.t_video_ps.e_enc = _minfo_video_enc_to_scdb_enc(t_minfo_ps_video.e_enc);

        pt_scdb_rec->u.t_video_ps.e_vid_fmt =
            t_minfo_ps_video.e_vid_fmt;

        pt_scdb_rec->u.t_video_ps.i4_width =
            t_minfo_ps_video.i4_width;

        pt_scdb_rec->u.t_video_ps.i4_height =
            t_minfo_ps_video.i4_height;

        pt_scdb_rec->u.t_video_ps.b_default =
            t_minfo_ps_video.b_default;
    }
    if (t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        pt_scdb_rec->u.t_subtitle_ps.ui2_strm_id = 
            ((UINT16)(t_minfo_ps_subtitle.ui1_strm_id<<8)) | ((UINT16)t_minfo_ps_subtitle.ui1_sub_strm_id);
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_mp3_strm_rec_to_scdb_mp3_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_mp3_strm_rec_to_scdb_mp3_strm_rec(
    MINFO_INFO_T                    t_minfo_rec,
    SCDB_REC_T*                     pt_scdb_rec
)
{
    MINFO_MP3_AUDIO_STM_ATRBT_T*    pt_mp3_audio_stm_attr;

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    pt_mp3_audio_stm_attr = &(t_minfo_rec.u.t_stm_attribute.u.t_mp3_audio_stm_attr);

    /* audio data */
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        /* encoding type */
        pt_scdb_rec->u.t_audio_mp3.e_enc = _minfo_audio_enc_to_scdb_enc(pt_mp3_audio_stm_attr->e_enc);
        /* layer */
        pt_scdb_rec->u.t_audio_mp3.e_layer = pt_mp3_audio_stm_attr->e_layer;
        /* bitrate */
        pt_scdb_rec->u.t_audio_mp3.ui4_bitrate = pt_mp3_audio_stm_attr->ui4_bitrate;
        /* sample rate */
        pt_scdb_rec->u.t_audio_mp3.ui4_sample_rate = pt_mp3_audio_stm_attr->ui4_sample_rate;
        /* version */
        pt_scdb_rec->u.t_audio_mp3.ui1_version = pt_mp3_audio_stm_attr->ui1_version;
        /* frame length */
        pt_scdb_rec->u.t_audio_mp3.ui4_frame_length = pt_mp3_audio_stm_attr->ui4_frame_length;
    }

    /* others */
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR
       || t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
       return SVCTXR_OK;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_mp3_strm_rec_to_scdb_mp3_strm_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_ogg_strm_rec_to_scdb_ogg_strm_rec(
    MINFO_INFO_T                    t_minfo_rec,
    SCDB_REC_T*                     pt_scdb_rec
)
{
    MINFO_OGG_VIDEO_STM_ATRBT_T     pt_ogg_video_stm_attr;
    MINFO_OGG_AUDIO_STM_ATRBT_T     pt_ogg_audio_stm_attr;

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    pt_ogg_audio_stm_attr = t_minfo_rec.u.t_stm_attribute.u.t_ogg_audio_stm_attr;
    pt_ogg_video_stm_attr = t_minfo_rec.u.t_stm_attribute.u.t_ogg_video_stm_attr;

    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    /*audio data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
    {
        pt_scdb_rec->u.t_audio_ogg.ui4_stream_id
            = t_minfo_rec.u.t_stm_attribute.u.t_ogg_audio_stm_attr.ui4_serial_num;
        pt_scdb_rec->u.t_audio_ogg.ui4_serial_num = pt_ogg_audio_stm_attr.ui4_serial_num;
        pt_scdb_rec->u.t_audio_ogg.ui8_granule_pos = pt_ogg_audio_stm_attr.ui8_granule_pos;
        pt_scdb_rec->u.t_audio_ogg.ui4_audio_page_num = pt_ogg_audio_stm_attr.ui4_audio_page_num;
        pt_scdb_rec->u.t_audio_ogg.ui4_avg_frame_num = pt_ogg_audio_stm_attr.ui4_avg_frame_num;
        pt_scdb_rec->u.t_audio_ogg.ui4_avg_packet_size = pt_ogg_audio_stm_attr.ui4_avg_packet_size;
        if (pt_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_VORBIS)
        {
            pt_scdb_rec->u.t_audio_ogg.e_enc = _minfo_audio_enc_to_scdb_enc(pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.e_enc);
            pt_scdb_rec->u.t_audio_ogg.t_ogg_audio_type = OGG_STRM_TYPE_VORBIS;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui4_vorbis_version = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_vorbis_version;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui1_audio_channel = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui1_audio_channel;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui4_audio_sample_rate = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_audio_sample_rate;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui4_bitrate_maximun = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_bitrate_maximun;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui4_bitrate_norminal = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_bitrate_norminal;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.ui4_bitrate_minimun = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.ui4_bitrate_minimun;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.blocksize = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.blocksize;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_vorbis_ogg.frameflag = pt_ogg_audio_stm_attr.u.t_ogg_strm_vorbis_info.frameflag;
        }
        else if (pt_ogg_audio_stm_attr.t_ogg_audio_type == OGG_STRM_TYPE_AUDIO)
        {
            pt_scdb_rec->u.t_audio_ogg.e_enc = _minfo_audio_enc_to_scdb_enc(pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc);
            pt_scdb_rec->u.t_audio_ogg.e_layer = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_layer;
            pt_scdb_rec->u.t_audio_ogg.t_ogg_audio_type = OGG_STRM_TYPE_AUDIO;
            pt_scdb_rec->u.t_audio_ogg.ui4_serial_num = pt_ogg_audio_stm_attr.ui4_serial_num;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui4_sub_type = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_sub_type;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui4_size = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_size;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui8_timeunit = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_timeunit;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui4_default_len = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_default_len;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui4_buffer_size = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_buffer_size;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui2_bit_per_sample = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui2_padding = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_padding;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui2_channels = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_channels;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui2_blockalign = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_blockalign;
            pt_scdb_rec->u.t_audio_ogg.u.pt_scdb_normal_ogg.ui4_avg_bytes_per_sec = pt_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_avg_bytes_per_sec;
        }
    }
    /*video data*/
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
    {
        pt_scdb_rec->u.t_video_ogg.ui4_stream_id
            = t_minfo_rec.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui4_serial_num;
        //pt_scdb_rec->u.t_video_ogg.e_enc = pt_ogg_video_stm_attr.e_enc;
        pt_scdb_rec->u.t_video_ogg.e_enc = _minfo_video_enc_to_scdb_enc(pt_ogg_video_stm_attr.e_enc);
        pt_scdb_rec->u.t_video_ogg.ui4_serial_num = pt_ogg_video_stm_attr.ui4_serial_num;
        pt_scdb_rec->u.t_video_ogg.ui8_granule_pos = pt_ogg_video_stm_attr.ui8_granule_pos;
        pt_scdb_rec->u.t_video_ogg.ui4_video_page_num = pt_ogg_video_stm_attr.ui4_video_page_num;
        pt_scdb_rec->u.t_video_ogg.ui4_avg_packet_size = pt_ogg_video_stm_attr.ui4_avg_packet_size;
        pt_scdb_rec->u.t_video_ogg.ui4_avg_frame_num = pt_ogg_video_stm_attr.ui4_avg_frame_num;
        pt_scdb_rec->u.t_video_ogg.ui4_sub_type =  pt_ogg_video_stm_attr.ui4_sub_type;
        pt_scdb_rec->u.t_video_ogg.ui4_size =  pt_ogg_video_stm_attr.ui4_size;
        pt_scdb_rec->u.t_video_ogg.ui8_timeunit =  pt_ogg_video_stm_attr.ui8_timeunit;
        pt_scdb_rec->u.t_video_ogg.ui8_sample_per_unit =  pt_ogg_video_stm_attr.ui8_sample_per_unit;
        pt_scdb_rec->u.t_video_ogg.ui4_default_len =  pt_ogg_video_stm_attr.ui4_default_len;
        pt_scdb_rec->u.t_video_ogg.ui4_buffer_size =  pt_ogg_video_stm_attr.ui4_buffer_size;
        pt_scdb_rec->u.t_video_ogg.ui2_bit_per_sample =  pt_ogg_video_stm_attr.ui2_bit_per_sample;
        pt_scdb_rec->u.t_video_ogg.ui2_padding =  pt_ogg_video_stm_attr.ui2_padding;
        pt_scdb_rec->u.t_video_ogg.ui4_width =  pt_ogg_video_stm_attr.ui4_width;
        pt_scdb_rec->u.t_video_ogg.ui4_height =  pt_ogg_video_stm_attr.ui4_height;
    }
    return SVCTXR_OK;
}

static INT32 _ext_subtitle_rec_2_scdb_rec(
    MINFO_INFO_T                    t_minfo_rec,
    SCDB_REC_T*                     pt_scdb_rec
)
{
    MINFO_EXT_SUBTITLE_STM_ATRBT_T*    pt_ext_subtitle_stm_attr;

    if (pt_scdb_rec == NULL)
    {
       return SVCTXR_INV_ARG;
    }

    pt_ext_subtitle_stm_attr = &(t_minfo_rec.u.t_stm_attribute.u.t_ex_subtt_stm_attr);

    /* audio data */
    if(t_minfo_rec.e_info_type == MINFO_INFO_TYPE_SP_ATTR)
    {
        pt_scdb_rec->e_rec_type = SCDB_REC_TYPE_SUBTITLE_EXT;
        pt_scdb_rec->u.t_ext_subtitle.h_ext_subtitle = pt_ext_subtitle_stm_attr->h_ext_subtitle;
        x_memcpy(pt_scdb_rec->u.t_ext_subtitle.c_lang_list , pt_ext_subtitle_stm_attr->c_lang_list, 3);
        pt_scdb_rec->u.t_ext_subtitle.h_related_subtitle = pt_ext_subtitle_stm_attr->h_related_subtitle;
        pt_scdb_rec->u.t_ext_subtitle.b_is_related = pt_ext_subtitle_stm_attr->b_is_related;
        pt_scdb_rec->u.t_ext_subtitle.ui2_idx = pt_ext_subtitle_stm_attr->ui2_idx;
    }
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _minfo_rec_2_scdb_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _minfo_rec_2_scdb_rec(MINFO_INFO_T t_minfo_rec,
                                   SCDB_REC_T*  pt_scdb_rec)
{
    INT32 i4_ret = SVCTXR_OK;
    if (pt_scdb_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    switch (t_minfo_rec.t_format.e_media_type)
    {
      case MEDIA_TYPE_CONTAINER:
          switch(t_minfo_rec.t_format.t_media_subtype.u.e_contnr_subtype)
          {
          case MEDIA_CONTNR_SUBTYPE_AVI:
              i4_ret =_minfo_avi_strm_rec_to_scdb_avi_strm_rec(t_minfo_rec,pt_scdb_rec);
              break;
          case MEDIA_CONTNR_SUBTYPE_ASF:
              i4_ret =_minfo_asf_strm_rec_to_scdb_asf_strm_rec(t_minfo_rec,pt_scdb_rec);
              break;
          case MEDIA_CONTNR_SUBTYPE_MP4:
              i4_ret =_minfo_mp4_strm_rec_to_scdb_mp4_strm_rec(t_minfo_rec,pt_scdb_rec);
              break;
          case MEDIA_CONTNR_SUBTYPE_MATROSKA:
              i4_ret = _minfo_mkv_strm_rec_to_scdb_mkv_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_CONTNR_SUBTYPE_RM:
              i4_ret = _minfo_rm_strm_rec_to_scdb_rm_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          /*add for flv*/
          case MEDIA_CONTNR_SUBTYPE_FLV:
              i4_ret =_minfo_flv_strm_rec_to_scdb_flv_strm_rec(t_minfo_rec,pt_scdb_rec);
              break;
          case MEDIA_CONTNR_SUBTYPE_OGG:
              i4_ret = _minfo_ogg_strm_rec_to_scdb_ogg_strm_rec(t_minfo_rec,pt_scdb_rec);
              break;
          default:
                    return SVCTXR_NOT_SUPPORT;
          }
          break;
      case MEDIA_TYPE_PS:
          i4_ret =_minfo_ps_strm_rec_to_scdb_ps_strm_rec(t_minfo_rec,pt_scdb_rec);
          break;
      case MEDIA_TYPE_AUDIO_ES:
          switch(t_minfo_rec.t_format.t_media_subtype.u.e_aud_subtype)
          {
          case MEDIA_AUD_SUBTYPE_MP3:
          case MEDIA_AUD_SUBTYPE_MP2:
              i4_ret = _minfo_mp3_strm_rec_to_scdb_mp3_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_WMA:
              i4_ret = _minfo_asf_strm_rec_to_scdb_asf_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_WAVE: /*add for wave support*/
              i4_ret = _minfo_wave_strm_rec_to_scdb_wave_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_AIF:
              i4_ret = _minfo_aif_strm_rec_to_scdb_aif_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_AAC:
              i4_ret = _minfo_aac_strm_rec_to_scdb_aac_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_AMR:
              i4_ret = _minfo_amr_strm_rec_to_scdb_amr_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_AWB:
              i4_ret = _minfo_awb_strm_rec_to_scdb_awb_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_AC3:
              i4_ret = _minfo_ac3_strm_rec_to_scdb_ac3_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_DTS:
              i4_ret = _minfo_dts_strm_rec_to_scdb_dts_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          case MEDIA_AUD_SUBTYPE_LPCM:
              i4_ret = _minfo_pcm_strm_rec_to_scdb_pcm_strm_rec(t_minfo_rec, pt_scdb_rec);
              break;
          default:
                return SVCTXR_NOT_SUPPORT;
          }
          break;
        #if 0
        case MEDIA_TYPE_VIDEO_ES:
            pt_scdb_rec->u.t_audio_es.ui4_stream_id = 0;
            pt_scdb_rec->ui4_private                = 0;
            pt_scdb_rec->e_rec_type                 = SCDB_REC_TYPE_VIDEO_ES;
            switch(t_minfo_rec.t_format.t_media_subtype.u.e_aud_subtype)
            {
                case MEDIA_VID_SUBTYPE_M1V:
                pt_scdb_rec->u.t_audio_es.e_enc = VID_ENC_MPEG_1;
                case MEDIA_VID_SUBTYPE_M2V:
                pt_scdb_rec->u.t_audio_es.e_enc = VID_ENC_MPEG_2;
                break;
                case MEDIA_VID_SUBTYPE_M4V:
                pt_scdb_rec->u.t_audio_es.e_enc = VID_ENC_MPEG_4;
                break;
                case MEDIA_VID_SUBTYPE_H264:
                pt_scdb_rec->u.t_audio_es.e_enc = VID_ENC_H264;
                case MEDIA_VID_SUBTYPE_VC1:
                pt_scdb_rec->u.t_audio_es.e_enc = VID_ENC_WVC1;
                break;
                default:
                return SVCTXR_NOT_SUPPORT;
            }
            break;
        #endif
      default:
          return SVCTXR_NO_RECORD;
      }
      return i4_ret;
}

/*add for font info*/
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_add_subtitle_font
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_add_subtitle_font(
                                        STREAM_COMP_ID_T*   pt_strm_comp_id,
                                        MEDIA_DESCRIBT_T*   pt_media_desc,
                                        MINFO_INFO_T*       pt_minfo_rec
                                        )
{
#if 1//ndef ANDROID
    INT32           i4_ret;
    UINT32          ui4_scdb_idx;
    UINT16          ui2_rec_nums;
    SIZE_T          z_rec_length;
    UINT32          ui4_scdb_tag;
    UINT64          ui8_cur_sbtl_link_id;
    FONT_INFO_T*    pt_cur_font;
    FONT_INFO_T*    pt_pre_font = NULL;
    FONT_INFO_T*    pt_use_font = NULL;
    MINFO_INFO_T    t_minfo_rec;

    if (pt_media_desc == NULL ||
        pt_minfo_rec == NULL ||
        pt_strm_comp_id == NULL)
    {
         return SVCTXR_INV_ARG;
    }

    ui4_scdb_tag = (UINT32)pt_strm_comp_id->pv_stream_tag;
    ui4_scdb_idx = (ui4_scdb_tag & 0x000000ff);
    z_rec_length = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, z_rec_length);
    i4_ret = x_minfo_get_num_recs(pt_media_desc->h_media_info,
                                MINFO_INFO_TYPE_VIDEO_ATTR,
                                &ui2_rec_nums);
    if (i4_ret != MINFOR_OK)
    {
        return  SVCTXR_NO_RECORD;
    }

    if (pt_strm_comp_id->e_type == ST_SUBTITLE)
    {
        if (ui4_scdb_idx > 0)
        {
            ui4_scdb_idx -= ui2_rec_nums;
        }
        i4_ret = x_minfo_get_num_recs(pt_media_desc->h_media_info,
                                    MINFO_INFO_TYPE_AUDIO_ATTR,
                                    &ui2_rec_nums);
        if(i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        if (ui4_scdb_idx > 0)
        {
            ui4_scdb_idx -= ui2_rec_nums;
        }

        ui8_cur_sbtl_link_id = pt_minfo_rec->u.t_stm_attribute.u.t_mkv_sp_stm_attr.ui8_attach_link_id;

        i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                MINFO_INFO_TYPE_FONT_DATA,
                                (UINT16)(ui4_scdb_idx+1),
                                &t_minfo_rec,
                                &z_rec_length
                                );
        if(i4_ret == MINFOR_OK)
        {
            pt_cur_font = pt_media_desc->pt_font_info;
            while(pt_cur_font)
            {
                if(ui8_cur_sbtl_link_id == pt_cur_font->ui8_font_id)
                {
                    if(pt_cur_font->pv_file_data)
                    {/*add font to system*/
                        pt_use_font = pt_cur_font;
                    }
                    else
                    {
                        x_dbg_stmt(_ERROR_HEADER"The font loaded failed!! line%d\n", __LINE__);
                    }
                }
                else if(pt_cur_font->pv_file_data)
                {
                    pt_pre_font = pt_cur_font;
                }

                if(pt_use_font && pt_pre_font)
                {
                    if(!x_strcmp((CHAR*)pt_use_font->ac_file_name, (CHAR*)pt_pre_font->ac_file_name))
                    {
                        break;
                    }
                }
                pt_cur_font = pt_cur_font->pt_next_font;
            }

            if(pt_use_font)
            {
                i4_ret = x_fe_add_mem_font((CHAR*)(pt_use_font->ac_file_name),
                                               (VOID*)((UINT8*)pt_use_font->pv_file_data + 4),
                                               pt_use_font->ui4_font_file_size, 24,  36,  48);
                if(FER_FONT_EXIST == i4_ret)
                {
                    if(pt_pre_font && pt_pre_font->pv_file_data)
                    {
                        _mm_util_mem_free(pt_use_font->pv_file_data);
                        pt_use_font->pv_file_data = pt_pre_font->pv_file_data;
                        pt_pre_font->pv_file_data = NULL;
                    }

                    x_dbg_stmt(_INFO_HEADER"The font added alredy exist!!\n");
                }
                else if(FER_OK != i4_ret)
                {
                    x_dbg_stmt(_ERROR_HEADER"The font added to system failed!!\n");
                }
                return  SVCTXR_OK;
            }
        }
        else
        {
            return  SVCTXR_NO_RECORD;
        }
    }
    #endif // #ifndef ANDROID
    return  SVCTXR_NO_RECORD;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_remove_subtitle_font
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_remove_subtitle_font(
                                        STREAM_COMP_ID_T*   pt_strm_comp_id,
                                        MEDIA_DESCRIBT_T*   pt_media_desc,
                                        MINFO_INFO_T*       pt_minfo_rec
                                        )
{
#if 1//ndef ANDROID
    INT32           i4_ret;
    UINT64          ui8_cur_sbtl_link_id;
    FONT_INFO_T*    pt_cur_font;

    if (pt_media_desc == NULL ||
        pt_minfo_rec == NULL ||
        pt_strm_comp_id == NULL)
    {
         return SVCTXR_INV_ARG;
    }

    if (pt_strm_comp_id->e_type == ST_SUBTITLE)
    {
        ui8_cur_sbtl_link_id = pt_minfo_rec->u.t_stm_attribute.u.t_mkv_sp_stm_attr.ui8_attach_link_id;

        pt_cur_font = pt_media_desc->pt_font_info;
        while(pt_cur_font)
        {
            if(ui8_cur_sbtl_link_id != pt_cur_font->ui8_font_id)
            {
                if(pt_cur_font->pv_file_data)
                {/*add font to system*/
                    i4_ret = x_fe_remove_font((CHAR*)(pt_cur_font->ac_file_name),  X_FE_OPTION_FORCE);
                    if(FER_OK != i4_ret)
                    {
                        x_dbg_stmt((_INFO_HEADER"The font remove from system failed!!\n"));
                    }
                    else
                    {
                        _mm_util_mem_free(pt_cur_font->pv_file_data);
                        pt_cur_font->pv_file_data = NULL;
                    }
                }
            }
            pt_cur_font = pt_cur_font->pt_next_font;
        }

        return  SVCTXR_OK;
    }
  #endif // #ifndef ANDROID
    return  SVCTXR_NO_RECORD;
}



/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_minfo_by_scdb_tag
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_get_minfo_by_scdb_tag(
                                        STREAM_COMP_ID_T*   pt_strm_comp_id,
                                        MEDIA_DESCRIBT_T*   pt_media_desc,
                                        MINFO_INFO_T*       pt_minfo_rec
                                        )
{
    INT32           i4_ret;
    UINT32          ui4_scdb_idx;
    UINT16          ui2_rec_nums;
    SIZE_T          z_rec_length;
    UINT32          ui4_scdb_tag;

    if (pt_media_desc == NULL ||
        pt_minfo_rec == NULL ||
        pt_strm_comp_id == NULL)
    {
         return SVCTXR_INV_ARG;
    }

    ui4_scdb_tag = (UINT32)pt_strm_comp_id->pv_stream_tag;
    ui4_scdb_idx = (ui4_scdb_tag & 0x000000ff);
    z_rec_length = sizeof(MINFO_INFO_T);
    i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_VIDEO,&ui2_rec_nums);
    if (i4_ret != MINFOR_OK)
    {
        return  SVCTXR_NO_RECORD;
    }


    if (pt_strm_comp_id->e_type == ST_VIDEO)
    {
        if (ui4_scdb_idx < ui2_rec_nums)
        {

            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                    MINFO_INFO_TYPE_VIDEO_ATTR,
                                    (UINT16)(ui4_scdb_idx+1),
                                    pt_minfo_rec,
                                    &z_rec_length
                                    );
            if (i4_ret == MINFOR_OK)
            {
                return  SVCTXR_OK;
            }
            else
            {
                return  SVCTXR_NO_RECORD;
            }
        }
    }
    else if (pt_strm_comp_id->e_type == ST_AUDIO)
    {
        if (ui4_scdb_idx > 0 && ui4_scdb_idx >= ui2_rec_nums)
        {
            ui4_scdb_idx -= ui2_rec_nums;
        }

        i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                MINFO_INFO_TYPE_AUDIO_ATTR,
                                (UINT16)(ui4_scdb_idx+1),
                                pt_minfo_rec,
                                &z_rec_length
                                );
        if (i4_ret == MINFOR_OK)
        {
            return  SVCTXR_OK;
        }
        else
        {
            return  SVCTXR_NO_RECORD;
        }
    }
    else if (pt_strm_comp_id->e_type == ST_SUBTITLE)
    {
        if (ui4_scdb_idx > 0)
        {
            ui4_scdb_idx -= ui2_rec_nums;
        }
        
        i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_AUDIO,&ui2_rec_nums);
        if(i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        if (ui4_scdb_idx > 0 && ui4_scdb_idx >= ui2_rec_nums)
        {
            ui4_scdb_idx -= ui2_rec_nums;
        }

        i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_SUBTITLE,&ui2_rec_nums);
        if(i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        if (ui4_scdb_idx >= ui2_rec_nums)
        {
            pt_media_desc->b_is_cur_ext_first = TRUE;
            return SVCTXR_OK;
        }
        else
        {
            pt_media_desc->b_is_cur_ext_first = FALSE;
        }
        
        i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                MINFO_INFO_TYPE_SP_ATTR,
                                (UINT16)(ui4_scdb_idx+1),
                                pt_minfo_rec,
                                &z_rec_length
                                );
        if(i4_ret == MINFOR_OK)
        {
            return  SVCTXR_OK;
        }
        else
        {
            return  SVCTXR_NO_RECORD;
        }
    }
    return  SVCTXR_NO_RECORD;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _frame_rate_cnvrt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
VSH_SRC_FRAME_RATE_T _frame_rate_cnvrt(UINT32 ui4_scale ,
                                       UINT32 ui4_rate)
{
     UINT64 u8Fps = 0 ;
     if (ui4_rate == 0 || ui4_scale == 0)
     {
         return VSH_SRC_FRAME_RATE_UNKNOWN;
     }
     else
     {
        u8Fps = _mm_div64((UINT64)((UINT64)ui4_rate*1000),ui4_scale,NULL);        
        switch (u8Fps)
        {
            case 1000:                
                return VSH_SRC_FRAME_RATE_1;            
            case 2000:                
                return VSH_SRC_FRAME_RATE_2;            
            case 5000:                
                return VSH_SRC_FRAME_RATE_5;            
            case 6000:                
                return VSH_SRC_FRAME_RATE_6;            
            case 8000:                
                return VSH_SRC_FRAME_RATE_8;            
            case 10000:                
                return VSH_SRC_FRAME_RATE_10;            
            case 12000:                
                return VSH_SRC_FRAME_RATE_12;            
            case 15000:                
                return VSH_SRC_FRAME_RATE_15;            
            case 20000:                
                return VSH_SRC_FRAME_RATE_20;            
            case 23970:/*Not in spec*/
            case 23976:
            case 23982:/*Not in spec*/
                return VSH_SRC_FRAME_RATE_23_976;
            case 24000:
                return VSH_SRC_FRAME_RATE_24;
            case 25000:
                return VSH_SRC_FRAME_RATE_25;
            case 29970:
                return VSH_SRC_FRAME_RATE_29_97;
            case 29998:
            case 29999:            
            case 30000:
                return VSH_SRC_FRAME_RATE_30;
            case 48000:                
                return VSH_SRC_FRAME_RATE_48;            
            case 50000:
                return VSH_SRC_FRAME_RATE_50;
            case 59940:
                return VSH_SRC_FRAME_RATE_59_94;
            case 60000:
                return VSH_SRC_FRAME_RATE_60;
            case 70000:                
                return VSH_SRC_FRAME_RATE_70;            
            case 120000:                
                return VSH_SRC_FRAME_RATE_120;
            default:
                return VSH_SRC_FRAME_RATE_UNKNOWN;
        }
     }
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_set_strm_codec_spec_info
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns : if set audio fail and it is critical,
 *           it needs return FAIL and the caller will close it
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_strm_codec_spec_info( HANDLE_T          h_media_desc,
                                                HANDLE_T          h_stream,
                                                STREAM_COMP_ID_T  t_strm_comp_id)
{

    INT32                           i4_ret;
    /*UINT32                        ui4_stream_tag;*/
    MINFO_INFO_T                    t_minfo_data;
    MINFO_TYPE_STM_ATRBT_T*         pt_stm_attribute = NULL;
    INT8                            i1_info_sz = 0;
    UINT32                          ui4HeaderData1;
    MEDIA_DESCRIBT_T*               pt_media_desc;
    VSH_SEQUENCE_INFO_T             t_vsh_seq_info;
#if SUPPORT_MULTI_INFO
    MINFO_INFO_T                    t_scale_info;
    SIZE_T                          z_rec_length;
#endif
    ASH_AUDIO_CODEC_SPEC_INFO_T     t_ash_codec_info;
    UINT16*                         pui2_codec_optn;

    i4_ret = _mm_hdlr_get_media_obj(h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    x_memset(&t_minfo_data,     0, sizeof(MINFO_INFO_T));
    x_memset(&t_vsh_seq_info, 0, sizeof(VSH_SEQUENCE_INFO_T));
    x_memset(&t_ash_codec_info, 0, sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));
    /* get scdb index from tag */
    if(MM_HDLR_WITH_MINFO(pt_media_desc))
    {
        /*ui4_stream_tag = (UINT32)(t_strm_comp_id.pv_stream_tag);*/
        _mm_hdlr_get_minfo_by_scdb_tag(&t_strm_comp_id, pt_media_desc, &t_minfo_data);
            pt_stm_attribute = &(t_minfo_data.u.t_stm_attribute);
    }
    else
    {
        UINT16              ui2_i;
        MM_STRM_ATRBT_T*    pt_stm_atrbt = NULL;
        STREAM_TYPE_T       e_type;

        if (pt_media_desc->pt_strm_info == NULL)
        {
            return SVCTXR_OK;
        }

        pt_stm_atrbt = pt_media_desc->pt_strm_info->at_stm_atrbt;
        for (ui2_i = 0 ; ui2_i < pt_media_desc->pt_strm_info->ui2_num_stm ; ui2_i++)
        {
            e_type = pt_stm_atrbt[ui2_i].e_type;

            if (t_strm_comp_id.e_type == e_type)
            {
                pt_stm_attribute = &(pt_stm_atrbt[ui2_i].t_stm_atrbt);
                break;
            }
        }
    }

    if (pt_stm_attribute == NULL)
    {
        return SVCTXR_FAIL;
    }

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_AUDIO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                i1_info_sz =(INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc
                    == MINFO_INFO_AUD_ENC_WMA_V1)
                {
                    t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                    t_ash_codec_info.u.t_wma_info.ui2_encoder_opt =
                        pt_stm_attribute->u.t_avi_audio_stm_attr.ui2_encode_opts;
                    t_ash_codec_info.u.t_wma_info.ui2_num_ch =
                        (UINT16)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                        (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_block_align;
                    t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec =
                        pt_stm_attribute->u.t_avi_audio_stm_attr.ui4_avg_bytes_sec;
                    t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                        (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i4_samples_per_sec;

                }
                else if (IS_AUDIO_PCM_ENC(pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc))
                {
                    i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                         (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                         (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                    t_ash_codec_info.u.t_pcm_info.ui2_block_align =
                         (UINT16)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_block_align;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                         (UINT16)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_bits_per_sample;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    t_ash_codec_info.u.t_pcm_info.e_pcm_type = 
                         _minfo_audio_enc_to_pcm_type(pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc);
                }
                else if (pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_aac_info.ui4_aac_profile =
                        (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.ui4_aac_profile;
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);
                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                else
                {
                    return SVCTXR_OK;
                }
           }
           break;
           case ST_VIDEO:
           i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
           if (x_handle_valid(h_stream) == TRUE)
           {
              /* avi h264 need width and height info also */              
              if ((pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_311) ||
                  (pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_H264) ||
                  (pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WMV3))
              {
                  t_vsh_seq_info.b_is_raw = FALSE;
                  t_vsh_seq_info.u.t_seq_data.ui2_width =
                      (UINT16)pt_stm_attribute->u.t_avi_video_stm_attr.i4_width;
                  t_vsh_seq_info.u.t_seq_data.ui2_height =
                      (UINT16)pt_stm_attribute->u.t_avi_video_stm_attr.i4_height;
                  if(pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WMV3 &&
                     pt_stm_attribute->u.t_avi_video_stm_attr.ui4_ext_data != 0)
                  {                    
                    ui4HeaderData1 = pt_stm_attribute->u.t_avi_video_stm_attr.ui4_ext_data;
                    ui4HeaderData1 = ((ui4HeaderData1&0x000000FF)<<24) |
                                     ((ui4HeaderData1&0x0000FF00)<<8) |
                                     ((ui4HeaderData1&0x00FF0000)>>8) |
                                     ((ui4HeaderData1&0xFF000000)>>24);
                    t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = ui4HeaderData1;
                    t_vsh_seq_info.u.t_seq_data.ui2_seq_hdr_sz = 4;
                  }
                  else
                  {
                    t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = 0;
                  }
                  t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                              _frame_rate_cnvrt(pt_stm_attribute->u.t_avi_video_stm_attr.ui4_scale,
                                                pt_stm_attribute->u.t_avi_video_stm_attr.ui4_rate);

              }
              else if (pt_stm_attribute->u.t_avi_video_stm_attr.ui4_codec_info_size != 0)
              {
                  t_vsh_seq_info.b_is_raw = TRUE;
                  t_vsh_seq_info.u.t_seq_raw.pc_buff = pt_stm_attribute->u.t_avi_video_stm_attr.pui1_codec_info;
                  t_vsh_seq_info.u.t_seq_raw.z_buff_size = pt_stm_attribute->u.t_avi_video_stm_attr.ui4_codec_info_size;
              }
              else /* default case */
              {
                  t_vsh_seq_info.b_is_raw = FALSE;
                  t_vsh_seq_info.u.t_seq_data.ui2_width =
                      (UINT16)pt_stm_attribute->u.t_avi_video_stm_attr.i4_width;
                  t_vsh_seq_info.u.t_seq_data.ui2_height =
                      (UINT16)pt_stm_attribute->u.t_avi_video_stm_attr.i4_height;
                  t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = 0;
                  t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                              _frame_rate_cnvrt(pt_stm_attribute->u.t_avi_video_stm_attr.ui4_scale,
                                                pt_stm_attribute->u.t_avi_video_stm_attr.ui4_rate);
              }
              i4_ret = x_sm_set(h_stream,
                                  SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                  (VOID*)&t_vsh_seq_info,
                                  (SIZE_T)i1_info_sz);

              if (i4_ret != SMR_OK)
              {
                  return SVCTXR_OK;
              }
            }
            break;

            default:
            break;
       }
    }
    
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
    {
        switch (t_strm_comp_id.e_type)
        {
           case ST_AUDIO:
        i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (IS_AUDIO_PCM_ENC(pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc))
                {
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
            (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_bit_per_sample;
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;
                    t_ash_codec_info.u.t_pcm_info.ui2_block_align =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_blockalign;
                    t_ash_codec_info.u.t_pcm_info.e_pcm_type =
                        _minfo_audio_enc_to_pcm_type(pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc);
                    t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                }
                else if (pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_channels;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;
                }
                else if (pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc == MINFO_INFO_AUD_ENC_WMA_V1)
                {
                    t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                    t_ash_codec_info.u.t_wma_info.ui2_num_ch =
                        (UINT16)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_channels;
                    t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui2_blockalign;
                    t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                        (UINT32)pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;

                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                pt_stm_attribute->u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);

                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
                   break;
           case ST_VIDEO:
                   i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
                if (x_handle_valid(h_stream) == TRUE)
                {
                      /* ogg h264 need width and height info also */
                      if ((pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_311) ||
                          (pt_stm_attribute->u.t_avi_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_H264))
                      {
                          t_vsh_seq_info.b_is_raw = FALSE;
                          t_vsh_seq_info.u.t_seq_data.ui2_width =
                              (UINT16)pt_stm_attribute->u.t_ogg_video_stm_attr.ui4_width;
                          t_vsh_seq_info.u.t_seq_data.ui2_height =
                              (UINT16)pt_stm_attribute->u.t_ogg_video_stm_attr.ui4_height;
                          t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = 0;
                      }            
                      i4_ret = x_sm_set(h_stream,
                                           SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                        (VOID*)&t_vsh_seq_info,
                                        (SIZE_T)i1_info_sz);

                    if (i4_ret != SMR_OK)
                      {
                         return SVCTXR_OK;
                      }
                }
                break;

            default:
                break;
       }
    }
    
    /*case ASF or WMA */
    if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER
        && pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
        || (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES
        && pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA))
    {
       switch (t_strm_comp_id.e_type)
       {
           case ST_VIDEO:
           i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
           if (x_handle_valid(h_stream) == TRUE)
           {

               t_vsh_seq_info.b_is_raw  = FALSE;/*in case of WMV789*/
               t_vsh_seq_info.u.t_seq_data.ui2_width =
                   (UINT16)pt_stm_attribute->u.t_asf_video_stm_attr.i4_width;
               t_vsh_seq_info.u.t_seq_data.ui2_height =
                   (UINT16)pt_stm_attribute->u.t_asf_video_stm_attr.i4_height;

               t_vsh_seq_info.u.t_seq_data.ui2_seq_hdr_sz =
                   (UINT16)pt_stm_attribute->u.t_asf_video_stm_attr.ui4_codec_info_size;
               t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                    _frame_rate_cnvrt(pt_stm_attribute->u.t_asf_video_stm_attr.ui4_scale,
                                      pt_stm_attribute->u.t_asf_video_stm_attr.ui4_rate);
               t_vsh_seq_info.u.t_seq_data.ui1_sample_id = (UINT8)pt_stm_attribute->u.t_asf_video_stm_attr.ui1_sample_id;

               /*without start code case*/
               if ((pt_stm_attribute->u.t_avi_video_stm_attr.e_enc != MINFO_INFO_VID_ENC_WVC1 ) 
               &&  (pt_stm_attribute->u.t_avi_video_stm_attr.e_enc != MINFO_INFO_VID_ENC_MPEG_4))
               {
                   if (pt_stm_attribute->u.t_asf_video_stm_attr.ui4_codec_info_size != 0)
                   {
                       x_memcpy(&ui4HeaderData1,
                           pt_stm_attribute->u.t_asf_video_stm_attr.pui1_codec_info,
                           4);
                       ui4HeaderData1 = ((ui4HeaderData1&0x000000FF)<<24) |
                                         ((ui4HeaderData1&0x0000FF00)<<8) |
                                         ((ui4HeaderData1&0x00FF0000)>>8) |
                                         ((ui4HeaderData1&0xFF000000)>>24);
                   }
                   else
                   {
                       ui4HeaderData1 = 0;
                   }
                   t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = ui4HeaderData1;
               }
               else /*start code case*/
               {
                   t_vsh_seq_info.b_is_raw = TRUE;
                   t_vsh_seq_info.u.t_seq_raw.pc_buff
                        = pt_stm_attribute->u.t_asf_video_stm_attr.pui1_codec_info;
                   t_vsh_seq_info.u.t_seq_raw.z_buff_size
                        = pt_stm_attribute->u.t_asf_video_stm_attr.ui4_codec_info_size;
                   t_vsh_seq_info.u.t_seq_raw.e_frame_rate
                        = _frame_rate_cnvrt(pt_stm_attribute->u.t_asf_video_stm_attr.ui4_scale,
                                            pt_stm_attribute->u.t_asf_video_stm_attr.ui4_rate);
                   t_vsh_seq_info.u.t_seq_raw.ui1_sample_id
                        = (UINT8)pt_stm_attribute->u.t_asf_video_stm_attr.ui1_sample_id;
               }
               i4_ret = x_sm_set(h_stream,
                                  SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                  (VOID*)&t_vsh_seq_info,
                                  (SIZE_T)i1_info_sz);

               if (i4_ret != SMR_OK)
               {
                   return SVCTXR_OK;
               }
            }
            break;
            case ST_AUDIO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                /* pt_audio_stream = (SM_AUDIO_STREAM_T*)sm_get_strm_obj(h_stream); */
                {
                    i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);

                    if (pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V1 ||
                        pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V2)
                    {
                        x_memset(&t_ash_codec_info.u.t_wma_info, 0, sizeof(ASH_AUDIO_WMA_INFO_T));
                        pui2_codec_optn = (UINT16*)(pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info);
                        t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                        t_ash_codec_info.u.t_wma_info.ui2_encoder_opt = pui2_codec_optn [2];
                        t_ash_codec_info.u.t_wma_info.ui2_num_ch =
                          (UINT16)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_channels;
                        t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                           (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_block_align;
                         t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec =
                        pt_stm_attribute->u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec;
                            t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                       (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i4_samples_per_sec;
                    }
                    else if ((pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_PRO) ||
                             (pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_LOSSLESS))
                    {
                        t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                        t_ash_codec_info.u.t_wma_info.ui2_num_ch  =
                            (UINT16)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_channels;
                        t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                            (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_block_align;
                        t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec =
                            pt_stm_attribute->u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec;
                        t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                            (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i4_samples_per_sec;
                        if (pt_stm_attribute->u.t_asf_audio_stm_attr.ui4_codec_info_size >= 18)
                        {
                            x_memcpy(&t_ash_codec_info.u.t_wma_info.ui2_valid_bits, 
                                     pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info, 2);
                            x_memcpy(&t_ash_codec_info.u.t_wma_info.ui4_channel_mask, 
                                      pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info + 2, 4);
                            x_memcpy(&t_ash_codec_info.u.t_wma_info.ui4_adv_option2, 
                                      pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info + 10, 4);
                            x_memcpy(&t_ash_codec_info.u.t_wma_info.ui2_encoder_opt,
                                      pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info + 14, 2);
                            x_memcpy(&t_ash_codec_info.u.t_wma_info.ui2_adv_option, 
                                      pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info + 16, 2);
                        }
                        else
                        {
                            x_dbg_stmt("\n[mm_hdlr][error] WMAPRO extra info is wrong line %d\n", __LINE__);
                        }
                    }
                    else if (IS_AUDIO_PCM_ENC(pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc))
                    {
                         i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                         t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                             (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_channels;
                         t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                             (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i4_samples_per_sec;
                         t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                         t_ash_codec_info.u.t_pcm_info.ui2_block_align =
                             (UINT16)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_block_align;
                         t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                             (UINT16)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_bits_per_sample;
                         t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                         t_ash_codec_info.u.t_pcm_info.e_pcm_type =  
                              _minfo_audio_enc_to_pcm_type(pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc);
                    }
                    else if(pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
                    {
                        t_ash_codec_info.u.t_aac_info.ui4_num_ch      = (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i2_channels;
                        t_ash_codec_info.u.t_aac_info.ui4_sample_rate = (UINT32)pt_stm_attribute->u.t_asf_audio_stm_attr.i4_samples_per_sec;
                        if (pt_stm_attribute->u.t_asf_audio_stm_attr.ui4_codec_info_size != 0 &&
                            pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info     != NULL)
                        {
                             UINT8 ui1_info = *(pt_stm_attribute->u.t_asf_audio_stm_attr.pui1_codec_info);
                             t_ash_codec_info.u.t_aac_info.ui4_aac_profile = (UINT32)((ui1_info>>3) & 0x1F);
                        }   
                    }
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);

                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
            }
            break;
            default:
            break;
       }
    }

    /*case MP4*/
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_VIDEO:
            i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
            if (x_handle_valid(h_stream) == TRUE)
            {/* mp4 h264 need width and height*/
                t_vsh_seq_info.b_is_raw  = FALSE;
                t_vsh_seq_info.u.t_seq_data.ui2_width =
                      (UINT16)pt_stm_attribute->u.t_mp4i_video_stm_attr.i4_width;
                t_vsh_seq_info.u.t_seq_data.ui2_height =
                      (UINT16)pt_stm_attribute->u.t_mp4i_video_stm_attr.i4_height;
                t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                      _frame_rate_cnvrt(pt_stm_attribute->u.t_mp4i_video_stm_attr.ui4_scale,
                                        pt_stm_attribute->u.t_mp4i_video_stm_attr.ui4_rate);
                i4_ret = x_sm_set(h_stream,
                                   SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                   (VOID*)&t_vsh_seq_info,
                                   (SIZE_T)i1_info_sz);

                t_vsh_seq_info.b_is_raw  = TRUE;
                t_vsh_seq_info.u.t_seq_raw.z_buff_size =
                pt_stm_attribute->u.t_mp4i_video_stm_attr.ui4_codec_info_size;
                t_vsh_seq_info.u.t_seq_raw.pc_buff =
                pt_stm_attribute->u.t_mp4i_video_stm_attr.pui1_codec_info;
                i4_ret = x_sm_set(h_stream,
                                   SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                   (VOID*)&t_vsh_seq_info,
                                   (SIZE_T)i1_info_sz);

                if(pt_stm_attribute->u.t_mp4i_video_stm_attr.pui1_mvcC_codec_info != NULL)
                {
                    t_vsh_seq_info.b_is_raw                 = TRUE;
                    t_vsh_seq_info.b_is_second_raw          = TRUE;
                    t_vsh_seq_info.u.t_seq_raw.z_buff_size  =
                        pt_stm_attribute->u.t_mp4i_video_stm_attr.ui4_mvcC_codec_info_size;
                    t_vsh_seq_info.u.t_seq_raw.pc_buff      =
                        pt_stm_attribute->u.t_mp4i_video_stm_attr.pui1_mvcC_codec_info;
                    i4_ret = x_sm_set(h_stream,
                                       SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                       (VOID*)&t_vsh_seq_info,
                                       (SIZE_T)i1_info_sz);
                }
                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
            }
            break;
            case ST_AUDIO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (pt_stm_attribute->u.t_mp4i_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                       (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                       (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.i4_sample_rate;
                    t_ash_codec_info.u.t_aac_info.ui4_aac_profile =
                       (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.ui4_aac_profile;
                    t_ash_codec_info.u.t_aac_info.b_frame_length =
                       (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.b_frame_length;
                }
                else if (IS_AUDIO_PCM_ENC(pt_stm_attribute->u.t_mp4i_audio_stm_attr.e_enc))
                {
                    i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                            (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                            (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.i4_sample_rate;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample = 
                        (UINT32)pt_stm_attribute->u.t_mp4i_audio_stm_attr.i4_sample_size;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian          = TRUE;
                    t_ash_codec_info.u.t_pcm_info.ui2_block_align       = 0;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific   = FALSE;
                    t_ash_codec_info.u.t_pcm_info.e_pcm_type = 
                         _minfo_audio_enc_to_pcm_type(pt_stm_attribute->u.t_mp4i_audio_stm_attr.e_enc);
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                pt_stm_attribute->u.t_mp4i_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);

                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;/*not allowed to play*/
                }
            }
            break;
            default:
            break;
        }
    }
    /*case PS*/
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_PS)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_VIDEO:
             break;
            case ST_AUDIO:
              i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (pt_stm_attribute->u.t_ps_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_PCM ||
                    pt_stm_attribute->u.t_ps_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_LPCM)
                {
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_ps_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_ps_audio_stm_attr.i4_sampling_frequency;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian = TRUE;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = TRUE;
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                pt_stm_attribute->u.t_ps_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);

                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
             break;
             default:
                break;
        }
    }

    /*case mkv*/
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_AUDIO:
            if(x_handle_valid(h_stream) == TRUE)
            {
                x_memset(&t_ash_codec_info, 0, sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));
                i1_info_sz =(INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (pt_stm_attribute->u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V1 ||
                    pt_stm_attribute->u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V2)
                {
                    t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                    t_ash_codec_info.u.t_wma_info.ui2_encoder_opt =
                        pt_stm_attribute->u.t_mkv_audio_stm_attr.ui2_encode_opts;
                    t_ash_codec_info.u.t_wma_info.ui2_num_ch =
                        (UINT16)pt_stm_attribute->u.t_mkv_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_wma_info.ui4_block_sz =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i2_block_align;
                    t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec =
                        pt_stm_attribute->u.t_mkv_audio_stm_attr.ui4_avg_bytes_sec;
                    t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i4_samples_per_sec;

                }
                else if (pt_stm_attribute->u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_PCM)
                {
                    i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    if (pt_stm_attribute->u.t_mkv_audio_stm_attr.e_align == MINFO_INFO_AUD_PCM_BIG_ENDIAN)
                    {
                        t_ash_codec_info.u.t_pcm_info.b_big_endian = TRUE;
                    }
                    else
                    {
                        t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                    }
                }
                else if (pt_stm_attribute->u.t_mkv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_aac_info.ui4_aac_profile =
                        (UINT32)pt_stm_attribute->u.t_mkv_audio_stm_attr.ui4_aac_profile;
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                    pt_stm_attribute->u.t_mkv_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);
                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
            }
            break;
            case ST_VIDEO:
            i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
            if(x_handle_valid(h_stream) == TRUE)
            {
                x_memset(&t_vsh_seq_info, 0, sizeof(VSH_SEQUENCE_INFO_T));
                t_vsh_seq_info.b_is_raw = FALSE;
                t_vsh_seq_info.u.t_seq_data.ui2_width =
                    (UINT16)pt_stm_attribute->u.t_mkv_video_stm_attr.i4_width;
                t_vsh_seq_info.u.t_seq_data.ui2_height =
                    (UINT16)pt_stm_attribute->u.t_mkv_video_stm_attr.i4_height;
                t_vsh_seq_info.u.t_seq_data.e_src_asp = VSH_SRC_ASPECT_RATIO_USR;                
                t_vsh_seq_info.u.t_seq_data.e_asp_usr_type = VSH_ASP_USR_TYPE_DISP;                        
                t_vsh_seq_info.u.t_seq_data.ui2_asp_usr_h =                                 
                    (UINT16)pt_stm_attribute->u.t_mkv_video_stm_attr.i4_dsp_height;                     
                t_vsh_seq_info.u.t_seq_data.ui2_asp_usr_w=                              
                    (UINT16)pt_stm_attribute->u.t_mkv_video_stm_attr.i4_dsp_width;                
                t_vsh_seq_info.u.t_seq_data.b_chk_framerate = pt_stm_attribute->u.t_mkv_video_stm_attr.b_divx_plus;
                t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                          _frame_rate_cnvrt(pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_scale,
                                            pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_rate);
#if SUPPORT_MULTI_INFO
                    x_memset(&t_scale_info, 0, sizeof(MINFO_INFO_T));
                    z_rec_length = sizeof(MINFO_INFO_T);
                    i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                              MINFO_INFO_TYPE_SCALE_INFO,
                                              1,
                                              &t_scale_info,
                                              &z_rec_length);
                    if (i4_ret == MINFOR_OK)
                    {
                        t_vsh_seq_info.pt_next_scale = (VSH_SEQUENCE_INFO_T*)x_mem_alloc(sizeof(VSH_SEQUENCE_INFO_T));
                        if(!t_vsh_seq_info.pt_next_scale)
                        {
                            return SVCTXR_FAIL;
                        }
                        x_memset(t_vsh_seq_info.pt_next_scale, 0, sizeof(VSH_SEQUENCE_INFO_T));
                        t_vsh_seq_info.pt_next_scale->b_is_raw = FALSE;
                        t_vsh_seq_info.pt_next_scale->u.t_seq_data.ui2_width =
                            (UINT16)t_scale_info.u.t_scale_info.i4_width;
                        t_vsh_seq_info.pt_next_scale->u.t_seq_data.ui2_height =
                            (UINT16)t_scale_info.u.t_scale_info.i4_height;
                        t_vsh_seq_info.pt_next_scale->u.t_seq_data.e_frame_rate =
                                  _frame_rate_cnvrt(t_scale_info.u.t_scale_info.ui4_scale,
                                                    t_scale_info.u.t_scale_info.ui4_rate);
                    }
#endif
                if (pt_stm_attribute->u.t_mkv_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_DIVX_311)
                {
                    t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = 0;
                }
                else if((pt_stm_attribute->u.t_mkv_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WMV1)
                      ||(pt_stm_attribute->u.t_mkv_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WMV2)
                      ||(pt_stm_attribute->u.t_mkv_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WMV3))
                {
                    if (pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_codec_info_size != 0)
                    {
                        x_memcpy(&ui4HeaderData1,
                            pt_stm_attribute->u.t_mkv_video_stm_attr.pui1_codec_info,
                            4);
                        ui4HeaderData1 = ((ui4HeaderData1&0x000000FF)<<24) |
                                         ((ui4HeaderData1&0x0000FF00)<<8) |
                                         ((ui4HeaderData1&0x00FF0000)>>8) |
                                         ((ui4HeaderData1&0xFF000000)>>24);
                    }
                    else
                    {
                       ui4HeaderData1 = 0;
                    }
                    t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = ui4HeaderData1;
                }
                /*else if(pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_codec_info_size != 0)*/
                else
                {
                    i4_ret = x_sm_set(h_stream,
                                       SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                       (VOID*)&t_vsh_seq_info,
                                       (SIZE_T)i1_info_sz);

                    t_vsh_seq_info.b_is_raw  = TRUE;
                    t_vsh_seq_info.u.t_seq_raw.z_buff_size =
                        pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_codec_info_size;
                    t_vsh_seq_info.u.t_seq_raw.pc_buff =
                        pt_stm_attribute->u.t_mkv_video_stm_attr.pui1_codec_info;
                    t_vsh_seq_info.u.t_seq_raw.e_frame_rate =
                          _frame_rate_cnvrt(pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_scale,
                                            pt_stm_attribute->u.t_mkv_video_stm_attr.ui4_rate);
                }
#if SUPPORT_MULTI_INFO
                    if(t_vsh_seq_info.pt_next_scale)
                    {
                        if (t_scale_info.u.t_scale_info.e_enc == MINFO_INFO_VID_ENC_DIVX_311)
                        {
                            t_vsh_seq_info.pt_next_scale->u.t_seq_data.ui4_seq_hdr = 0;
                        }
                        else if((t_scale_info.u.t_scale_info.e_enc == MINFO_INFO_VID_ENC_WMV1)
                              ||(t_scale_info.u.t_scale_info.e_enc == MINFO_INFO_VID_ENC_WMV2)
                              ||(t_scale_info.u.t_scale_info.e_enc == MINFO_INFO_VID_ENC_WMV3))
                        {
                            if (t_scale_info.u.t_scale_info.ui4_codec_info_size != 0)
                            {
                                x_memcpy(&ui4HeaderData1,
                                    t_scale_info.u.t_scale_info.pui1_codec_info,
                                    4);
                                ui4HeaderData1 = ((ui4HeaderData1&0x000000FF)<<24) |
                                                 ((ui4HeaderData1&0x0000FF00)<<8) |
                                                 ((ui4HeaderData1&0x00FF0000)>>8) |
                                                 ((ui4HeaderData1&0xFF000000)>>24);
                            }
                            else
                            {
                               ui4HeaderData1 = 0;
                            }
                            t_vsh_seq_info.pt_next_scale->u.t_seq_data.ui4_seq_hdr = ui4HeaderData1;
                        }
                        else
                        {
                            t_vsh_seq_info.pt_next_scale->b_is_raw  = TRUE;
                            t_vsh_seq_info.pt_next_scale->u.t_seq_raw.z_buff_size =
                                t_scale_info.u.t_scale_info.ui4_codec_info_size;
                            t_vsh_seq_info.pt_next_scale->u.t_seq_raw.pc_buff =
                                t_scale_info.u.t_scale_info.pui1_codec_info;
                            t_vsh_seq_info.pt_next_scale->u.t_seq_raw.e_frame_rate =
                                  _frame_rate_cnvrt(t_scale_info.u.t_scale_info.ui4_scale,
                                                    t_scale_info.u.t_scale_info.ui4_rate);
                        }
                    }
#endif
                i4_ret = x_sm_set(h_stream,
                                  SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                  (VOID*)&t_vsh_seq_info,
                                  (SIZE_T)i1_info_sz);
#if SUPPORT_MULTI_INFO
                if(t_vsh_seq_info.pt_next_scale)
                {
                    x_mem_free(t_vsh_seq_info.pt_next_scale);
                    t_vsh_seq_info.pt_next_scale = NULL;
                }
#endif

                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
            }
            break;
            default:
            break;
       }
    }

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_VIDEO:
                {
                    VSH_RM_SPEC_INFO_T* pt_rm_vid_spec_info = (VSH_RM_SPEC_INFO_T*)x_mem_alloc(sizeof(VSH_RM_SPEC_INFO_T));
                    UINT32 ui4_specific_size = 0;
                    if(pt_rm_vid_spec_info == NULL)
                    {
                        return SVCTXR_FAIL;
                    }
        
                    x_memset(&t_vsh_seq_info, 0, sizeof(VSH_SEQUENCE_INFO_T));
                    x_memset(pt_rm_vid_spec_info, 0, sizeof(VSH_RM_SPEC_INFO_T));
                    if(MINFO_INFO_VID_ENC_RV8 == pt_stm_attribute->u.t_rm_video_stm_attr.e_enc)
                    {
                            pt_rm_vid_spec_info->b_is_rm8 = TRUE;
                    }
                    else
                    {
                            pt_rm_vid_spec_info->b_is_rm8 = FALSE;
                    }
                    pt_rm_vid_spec_info->ui2_frm_height =
                        pt_stm_attribute->u.t_rm_video_stm_attr.ui2_frm_height;
                    pt_rm_vid_spec_info->ui2_frm_width =
                        pt_stm_attribute->u.t_rm_video_stm_attr.ui2_frm_width;
                    pt_rm_vid_spec_info->ui4_spo_extra_flags =
                        pt_stm_attribute->u.t_rm_video_stm_attr.ui4_spo_extra_flags;

                    ui4_specific_size = (((pt_rm_vid_spec_info->ui4_spo_extra_flags & 0x70000) >> 16) + 1) * 2;
                    
                    ui4_specific_size *= sizeof(UINT32); 
                    
                    if (ui4_specific_size > sizeof(pt_rm_vid_spec_info->aui4_specific))
                    {
                        ui4_specific_size = sizeof(pt_rm_vid_spec_info->aui4_specific);
                    }
                    
                    if(pt_stm_attribute->u.t_rm_video_stm_attr.ui4_specific != 0 &&
                        ui4_specific_size > 0)
                    {
                        x_memcpy(
                                (void*)pt_rm_vid_spec_info->aui4_specific, 
                                (void*)pt_stm_attribute->u.t_rm_video_stm_attr.ui4_specific,
                                ui4_specific_size);
                    }
                    
                    t_vsh_seq_info.b_is_raw = TRUE;
                    t_vsh_seq_info.u.t_seq_raw.pc_buff = (UCHAR*)pt_rm_vid_spec_info;
                    t_vsh_seq_info.u.t_seq_raw.z_buff_size = sizeof(VSH_RM_SPEC_INFO_T);
                    i4_ret = x_sm_set(h_stream,
                                      SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                      (VOID*)&t_vsh_seq_info,
                                      (SIZE_T)sizeof(VSH_SEQUENCE_INFO_T));

                    if(i4_ret != SMR_OK)
                    {
                        if(pt_rm_vid_spec_info)
                        {
                            x_mem_free(pt_rm_vid_spec_info);
                        }
                        return SVCTXR_OK;
                    }
                    if(pt_rm_vid_spec_info)
                    {
                        x_mem_free(pt_rm_vid_spec_info);
                    }
                }
                break;
            case ST_AUDIO:
                x_memset(&t_ash_codec_info, 0, sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));

                if(MINFO_INFO_AUD_ENC_COOK == pt_stm_attribute->u.t_rm_audio_stm_attr.e_enc)
                {
                    t_ash_codec_info.u.t_cook_info.ui2_codec_frm_sz =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_codec_frm_sz;
                    t_ash_codec_info.u.t_cook_info.ui2_flavor_idx =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_flavor_idx;
                    t_ash_codec_info.u.t_cook_info.ui2_intlv_blk_sz =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_intlv_blk_sz;
                    t_ash_codec_info.u.t_cook_info.ui2_intlv_factor =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_intlv_factor;
                    t_ash_codec_info.u.t_cook_info.ui2_num_channel =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_num_channel;
                    t_ash_codec_info.u.t_cook_info.ui4_sample_rate =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui4_sample_rate;
                    t_ash_codec_info.u.t_cook_info.pv_codec_opaque_data =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.pv_codec_opaque_data;
                    t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                        pt_stm_attribute->u.t_rm_audio_stm_attr.e_enc);
                    i4_ret = x_sm_set(h_stream,
                                      SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                      (VOID*)&t_ash_codec_info,
                                      (SIZE_T)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));

                }
                else if(MINFO_INFO_AUD_ENC_AAC == pt_stm_attribute->u.t_rm_audio_stm_attr.e_enc)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui2_num_channel;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                        pt_stm_attribute->u.t_rm_audio_stm_attr.ui4_sample_rate;
                    t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                        pt_stm_attribute->u.t_rm_audio_stm_attr.e_enc);
                    i4_ret = x_sm_set(h_stream,
                                      SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                      (VOID*)&t_ash_codec_info,
                                      (SIZE_T)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));
                }
                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                break;
             default:
                break;
        }
    }

    /*add for flv*/
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_VIDEO:
            #if 0
            i1_info_sz = (INT8)sizeof(VSH_SEQUENCE_INFO_T);
            if (x_handle_valid(h_stream) == TRUE)
            {   /* mp4 h264 need width and height*/
                x_memset(&t_vsh_seq_info, 0, sizeof(t_vsh_seq_info));
                t_vsh_seq_info.b_is_raw  = FALSE;
                t_vsh_seq_info.u.t_seq_data.ui2_width =
                      (UINT16)t_minfo_data.u.t_stm_attribute.u.t_flv_video_stm_attr.i4_width;
                t_vsh_seq_info.u.t_seq_data.ui2_height =
                      (UINT16)t_minfo_data.u.t_stm_attribute.u.t_flv_video_stm_attr.i4_height;
                t_vsh_seq_info.u.t_seq_raw.e_frame_rate =
                                _frame_rate_cnvrt(1, t_minfo_data.u.t_stm_attribute.u.t_flv_video_stm_attr.ui4_rate);
                i4_ret = x_sm_set(h_stream,
                                   SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                   (VOID*)&t_vsh_seq_info,
                                   (SIZE_T)i1_info_sz);

                /*t_vsh_seq_info.b_is_raw  = TRUE;
                                t_vsh_seq_info.u.t_seq_raw.z_buff_size =
                                t_minfo_data.u.t_stm_attribute.u.t_mp4i_video_stm_attr.ui4_codec_info_size;
                                t_vsh_seq_info.u.t_seq_raw.pc_buff =
                                t_minfo_data.u.t_stm_attribute.u.t_mp4i_video_stm_attr.pui1_codec_info;
                                i4_ret = x_sm_set(h_stream,
                                   SM_VSH_SET_TYPE_SEQUENCE_INFO,
                                   (VOID*)&t_vsh_seq_info,
                                   (SIZE_T)i1_info_sz);*/

                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
            }
            #endif
            break;
            case ST_AUDIO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                x_memset(&t_ash_codec_info, 0, sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T));
                i1_info_sz =(INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (IS_AUDIO_PCM_ENC(t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.e_enc))
                {
                    i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (UINT32)t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        (UINT32)t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                        (UINT16)t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.i2_bits_per_sample;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                    t_ash_codec_info.u.t_pcm_info.e_pcm_type =  
                         _minfo_audio_enc_to_pcm_type(t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.e_enc);
                }
                else if (t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_AAC)
                {
                    t_ash_codec_info.u.t_aac_info.ui4_num_ch =
                        (UINT32)t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_aac_info.ui4_sample_rate =
                        (UINT32)t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.i4_samples_per_sec;
                }
                t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                    t_minfo_data.u.t_stm_attribute.u.t_flv_audio_stm_attr.e_enc);
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);
                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
            }
            break;
            default:
            break;
        }
    }

    /* case AUDIO ES */
    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        switch (t_strm_comp_id.e_type)
        {
            case ST_VIDEO:
                break;
            case ST_AUDIO:
                i1_info_sz = (INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM)
                {
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (pt_stm_attribute->u.t_es_audio_stm_attr.ui2_channels != 0)
                        ? pt_stm_attribute->u.t_es_audio_stm_attr.ui2_channels
                        : 2;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        pt_stm_attribute->u.t_es_audio_stm_attr.ui4_sampling_frequency;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian =
                        pt_stm_attribute->u.t_es_audio_stm_attr.b_big_endian;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                        (pt_stm_attribute->u.t_es_audio_stm_attr.ui2_bits_per_sample != 0)
                        ? pt_stm_attribute->u.t_es_audio_stm_attr.ui2_bits_per_sample
                        : 16;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    t_ash_codec_info.e_fmt = ASH_AUDIO_FMT_PCM;
                }
                else if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE)
                {  /*add for wave support*/
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                         (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                         (UINT32)pt_stm_attribute->u.t_avi_audio_stm_attr.i4_samples_per_sec;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian = FALSE;
                    t_ash_codec_info.u.t_pcm_info.ui2_block_align =
                         (UINT16)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_block_align;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                         (UINT16)pt_stm_attribute->u.t_avi_audio_stm_attr.i2_bits_per_sample;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;

                    t_ash_codec_info.u.t_pcm_info.e_pcm_type =
                          _minfo_audio_enc_to_pcm_type(pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc);

                    t_ash_codec_info.e_fmt = _minfo_audio_enc_to_ash_enc(
                      pt_stm_attribute->u.t_avi_audio_stm_attr.e_enc);
                    /*pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_LPCM;*/
                }
                else if(pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF)
                {
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch =
                        (pt_stm_attribute->u.t_es_audio_stm_attr.ui2_channels != 0)
                        ? pt_stm_attribute->u.t_es_audio_stm_attr.ui2_channels
                        : 2;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate =
                        pt_stm_attribute->u.t_es_audio_stm_attr.ui4_sampling_frequency;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian =
                        pt_stm_attribute->u.t_es_audio_stm_attr.b_big_endian;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample =
                        (pt_stm_attribute->u.t_es_audio_stm_attr.ui2_bits_per_sample != 0)
                        ? pt_stm_attribute->u.t_es_audio_stm_attr.ui2_bits_per_sample
                        : 16;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    t_ash_codec_info.e_fmt = ASH_AUDIO_FMT_PCM;
                    t_ash_codec_info.u.t_pcm_info.b_signed = pt_stm_attribute->u.t_es_audio_stm_attr.b_signed;
                }
                else if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3)
                {
                    t_ash_codec_info.u.t_ac3_info.ui1_endian = pt_stm_attribute->u.t_es_audio_stm_attr.b_big_endian;
                    t_ash_codec_info.e_fmt = ASH_AUDIO_FMT_AC3;
                }
                else
                {
                    break;
                }
                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);

                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
             break;
             default:
                break;
        }
    }

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_CMPB)
    {
        switch (t_strm_comp_id.e_type)
        {
        case ST_VIDEO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                i1_info_sz = sizeof(VSH_SEQUENCE_INFO_T);
            if ((pt_stm_attribute->u.t_cmpb_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_MJPG) ||
                (pt_stm_attribute->u.t_cmpb_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_NV12))
            {
                t_vsh_seq_info.b_is_raw = FALSE;
                t_vsh_seq_info.u.t_seq_data.ui2_width =
                (UINT16)pt_stm_attribute->u.t_cmpb_video_stm_attr.i4_width;
                t_vsh_seq_info.u.t_seq_data.ui2_height =
                (UINT16)pt_stm_attribute->u.t_cmpb_video_stm_attr.i4_height;
                t_vsh_seq_info.u.t_seq_data.ui4_seq_hdr = 0;
                t_vsh_seq_info.u.t_seq_data.e_frame_rate =
                _frame_rate_cnvrt(pt_stm_attribute->u.t_cmpb_video_stm_attr.ui4_scale,
                                pt_stm_attribute->u.t_cmpb_video_stm_attr.ui4_rate);
                }
                else if(pt_stm_attribute->u.t_cmpb_video_stm_attr.e_enc == MINFO_INFO_VID_ENC_WVC1)
                {
                    t_vsh_seq_info.b_is_raw = TRUE;
                    t_vsh_seq_info.u.t_seq_raw.pc_buff
                        = pt_stm_attribute->u.t_cmpb_video_stm_attr.pu1_buff;
                    t_vsh_seq_info.u.t_seq_raw.z_buff_size
                        = pt_stm_attribute->u.t_cmpb_video_stm_attr.ui4_buff_size;
                    t_vsh_seq_info.u.t_seq_raw.e_frame_rate
                        =  _frame_rate_cnvrt(pt_stm_attribute->u.t_cmpb_video_stm_attr.ui4_scale,
                                    pt_stm_attribute->u.t_cmpb_video_stm_attr.ui4_rate);
                    t_vsh_seq_info.u.t_seq_raw.ui1_sample_id
                        = (UINT8)pt_stm_attribute->u.t_cmpb_video_stm_attr.ui1_sample_id;    
                }
                
                x_sm_set(h_stream,
                         SM_VSH_SET_TYPE_SEQUENCE_INFO,
                         (VOID*)&t_vsh_seq_info,
                         (SIZE_T)i1_info_sz);
            }
            break;
        case ST_AUDIO:
            if (x_handle_valid(h_stream) == TRUE)
            {
                MINFO_CMPB_AUDIO_STM_ATRBT_T* pt_cmpb_aud_stm_attr = NULL;
    
                pt_cmpb_aud_stm_attr = &(pt_stm_attribute->u.t_cmpb_audio_stm_attr);
                i1_info_sz =(INT8)sizeof(ASH_AUDIO_CODEC_SPEC_INFO_T);
                
                if (pt_stm_attribute->u.t_cmpb_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_PCM ||
                    pt_stm_attribute->u.t_cmpb_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_ADPCM)
                {
                    
                    t_ash_codec_info.u.t_pcm_info.ui4_num_ch
                                    = (UINT32)pt_cmpb_aud_stm_attr->ui2_channels;
                    t_ash_codec_info.u.t_pcm_info.ui4_sample_rate
                                    = pt_cmpb_aud_stm_attr->ui4_sample_rate;
                    t_ash_codec_info.u.t_pcm_info.b_big_endian 
                                    = pt_cmpb_aud_stm_attr->fg_big_endian;
                    t_ash_codec_info.u.t_pcm_info.ui2_block_align 
                                    = pt_cmpb_aud_stm_attr->ui2_block_align;
                    t_ash_codec_info.u.t_pcm_info.ui2_bits_per_sample 
                                    = pt_cmpb_aud_stm_attr->ui2_bits_per_sample;
                    t_ash_codec_info.u.t_pcm_info.b_info_not_specific = FALSE;
                    t_ash_codec_info.e_fmt = ASH_AUDIO_FMT_PCM;
                    if (pt_stm_attribute->u.t_cmpb_audio_stm_attr.e_pcm_type == MM_AUD_PCM_TYPE_NORMAL)
                    {
                        t_ash_codec_info.u.t_pcm_info.e_pcm_type = ASH_AUDIO_PCM_TYPE_NORMAL;
                    }
                    else if (pt_stm_attribute->u.t_cmpb_audio_stm_attr.e_pcm_type == MM_AUD_PCM_TYPE_MS_ADPCM)
                    {
                        t_ash_codec_info.u.t_pcm_info.e_pcm_type = ASH_AUDIO_PCM_TYPE_MS_ADPCM;
                    }
                    else
                    {
                        t_ash_codec_info.u.t_pcm_info.e_pcm_type = ASH_AUDIO_PCM_TYPE_IMA_ADPCM;
                    }
                    
                }
                else if(pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V1 ||
                        pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V2 ||
                        pt_stm_attribute->u.t_asf_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_WMA_V3)
                {
                    t_ash_codec_info.u.t_wma_info.ui2_hdr_src = 1;
                    t_ash_codec_info.u.t_wma_info.ui2_encoder_opt    = (UINT16)pt_cmpb_aud_stm_attr->ui2_encoder_opt;
                    t_ash_codec_info.u.t_wma_info.ui2_num_ch         = (UINT16)pt_cmpb_aud_stm_attr->ui2_channels;
                    t_ash_codec_info.u.t_wma_info.ui4_block_sz       = (UINT32)pt_cmpb_aud_stm_attr->ui4_block_size;
                    t_ash_codec_info.u.t_wma_info.ui4_bits_per_sec   = (UINT32)pt_cmpb_aud_stm_attr->ui4_bits_per_sec;
                    t_ash_codec_info.u.t_wma_info.ui4_sample_per_sec = (UINT32)pt_cmpb_aud_stm_attr->ui4_sample_per_sec;
                    t_ash_codec_info.e_fmt = ASH_AUDIO_FMT_WMA;
                }

                i4_ret = x_sm_set(h_stream,
                                  SM_ASH_SET_TYPE_CODEC_SPEC_INFO,
                                  (VOID*)&t_ash_codec_info,
                                  (SIZE_T)i1_info_sz);
                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_OK;
                }
            }
            break;

            default:
            break;
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_sel_strm_in_range_alloc_info
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_sel_strm_in_range_alloc_info(
                            MM_SVC_REQ_T*                     pt_running_svc_req,
                            MIDXBULD_ELMT_TBL_ALLOC_INFO_T*   pt_range_info,
                            RANGE_STRM_SEL_MODE_T             e_mode,
                            VOID*                             pv_id_list)
{
    UINT8                            ui1_idx;
    INT32                            i4_ret;
    STRM_REQ_T*                      pt_strm_req;
    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_ele_idxtbl_des = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_pre = NULL;
    MEDIA_DESCRIBT_T*                pt_media_desc;
    UINT16                           ui2_aud_rec_nums= 0;
    UINT16                           ui2_vid_rec_nums= 0;
    UINT16                           ui2_sp_rec_nums= 0;
    UINT16                           ui2_idx = 0;
    UINT16                           ui2_sp_idx = 0;
    UINT16                           ui2_counts = 0;
    UINT32                           ui4_rec_sz= sizeof(MINFO_INFO_T);
    MINFO_INFO_T                     t_vid_stm_info;
    MINFO_INFO_T                     t_aud_stm_info;
    MINFO_INFO_T                     t_sp_stm_info;

    if ((NULL == pt_running_svc_req ) || (NULL == pt_range_info))
    {
        return SVCTXR_INV_ARG;
    }
    if ((e_mode == RANGE_STRM_SEL_MODE_ID_LST)&&
        pv_id_list == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    /* initialize first */
    x_memset((VOID*)&t_vid_stm_info, 0, sizeof(MINFO_INFO_T));
    x_memset((VOID*)&t_aud_stm_info, 0, sizeof(MINFO_INFO_T));
    x_memset((VOID*)&t_sp_stm_info,  0, sizeof(MINFO_INFO_T));

    if (MM_HDLR_WITH_MINFO(pt_media_desc))
    {
        x_minfo_get_num_recs(pt_media_desc->h_media_info,
                             MINFO_INFO_TYPE_VIDEO_ATTR ,
                             &ui2_vid_rec_nums);
        x_minfo_get_num_recs(pt_media_desc->h_media_info,
                             MINFO_INFO_TYPE_AUDIO_ATTR ,
                             &ui2_aud_rec_nums);
        x_minfo_get_num_recs(pt_media_desc->h_media_info,                             
                            MINFO_INFO_TYPE_SP_ATTR ,                             
                            &ui2_sp_rec_nums);
    }
    else
    {
        UINT16              ui2_i;
        MM_STRM_ATRBT_T*    pt_stm_atrbt = NULL;

        if (pt_media_desc->pt_strm_info == NULL)
        {
            return SVCTXR_OK;
        }

        pt_stm_atrbt = pt_media_desc->pt_strm_info->at_stm_atrbt;
        for (ui2_i = 0 ; ui2_i < pt_media_desc->pt_strm_info->ui2_num_stm ; ui2_i++)
        {
            if (pt_stm_atrbt[ui2_i].e_type == ST_VIDEO)
            {
                ui2_vid_rec_nums = 1;
                t_vid_stm_info.u.t_stm_attribute = (pt_stm_atrbt[ui2_i]).t_stm_atrbt;
                t_vid_stm_info.t_format = pt_media_desc->t_media_fmt;
            }
            else if (pt_stm_atrbt[ui2_i].e_type == ST_AUDIO)
            {
                ui2_aud_rec_nums = 1;
                t_aud_stm_info.u.t_stm_attribute = (pt_stm_atrbt[ui2_i]).t_stm_atrbt;
                t_aud_stm_info.t_format = pt_media_desc->t_media_fmt;
            }
            else if (pt_stm_atrbt[ui2_i].e_type == ST_SUBTITLE)            
            {                
                ui2_sp_rec_nums = 1;                
                t_sp_stm_info.u.t_stm_attribute = (pt_stm_atrbt[ui2_i]).t_stm_atrbt;                
                t_sp_stm_info.t_format = pt_media_desc->t_media_fmt;            
            }
        }
    }

    /*initailize the pt_range_info*/
    x_memset(pt_range_info, 0, sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T));

    ui2_counts = ui2_vid_rec_nums + ui2_aud_rec_nums + ui2_sp_rec_nums;
    /*stream selection*/
    for (ui1_idx = 0; ui1_idx < (UINT8)ui2_counts; ui1_idx++)
    {
        if (e_mode == RANGE_STRM_SEL_MODE_ACTIVE)
        {
            /*e_strm_type = e_g_mm_strm_types[ui1_idx];*/
            if (ui2_aud_rec_nums == 0)/*pure video*/
            {
                if(ui1_idx >= 2)
                {
                    break;
                }
                else if(ui1_idx == 0)
                {
                    ui1_idx = 1;
                }
            }
            if (ui1_idx > 2) /*only 1 video & 1 audio*/
            {
                break;
            }
            pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ui1_idx]);

            if (x_handle_valid(pt_strm_req->h_stream) == TRUE)
            {
                pt_ele_idxtbl_des = x_mem_alloc(sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T));
                if (pt_ele_idxtbl_des == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_memset(pt_ele_idxtbl_des, 0, (sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T)));
                pt_ele_idxtbl_des->pt_next = NULL;
                pt_ele_idxtbl_des->u_info.t_es.ui4_strm_id =
                _mm_hdlr_scdb_rec_get_stream_id(pt_strm_req->u.t_scdb.t_rec);
            }
        }
        else if (e_mode == RANGE_STRM_SEL_MODE_ALL)
        {
                if (ui2_vid_rec_nums != 0 || ui2_aud_rec_nums != 0 || ui2_sp_rec_nums != 0)
                {
                     pt_ele_idxtbl_des = x_mem_alloc(sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T));
                }
                if (pt_ele_idxtbl_des == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_memset(pt_ele_idxtbl_des, 0, (sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T)));
                pt_ele_idxtbl_des->pt_next = NULL;
                if (ui2_vid_rec_nums > 0)
                {
                    if (MM_HDLR_WITH_MINFO(pt_media_desc))
                    {
                         i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                                   MINFO_INFO_TYPE_VIDEO_ATTR,
                                                   1,
                                                   &t_vid_stm_info,
                                                   (SIZE_T*)&ui4_rec_sz);
                    }

                    pt_ele_idxtbl_des->u_info.t_es.ui4_strm_id =
                        _mm_hdlr_minfo_get_stream_id(t_vid_stm_info);
                    ui2_vid_rec_nums--;
                    /*This code is bad, but I have no better choice, later will clean it up!!!*/
                    #if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
                    while ( ui2_vid_rec_nums )
                    {
                        --ui2_vid_rec_nums;
                        ++ui1_idx;
                    }
                    #endif
                }
                else if (ui2_aud_rec_nums > 0)
                {
                     if (MM_HDLR_WITH_MINFO(pt_media_desc))
                     {
                         ui2_idx++;

                         i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                                   MINFO_INFO_TYPE_AUDIO_ATTR,
                                                   ui2_idx,
                                                   &t_aud_stm_info,
                                                   (SIZE_T*)&ui4_rec_sz);
                      }

                      pt_ele_idxtbl_des->u_info.t_es.ui4_strm_id =
                          _mm_hdlr_minfo_get_stream_id(t_aud_stm_info);
                      ui2_aud_rec_nums--;
                }
                else if (ui2_sp_rec_nums > 0)                
                {                     
                    if (MM_HDLR_WITH_MINFO(pt_media_desc))                     
                    {                         
                         ui2_sp_idx++;

                        i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,                                                   
                                    MINFO_INFO_TYPE_SP_ATTR,                                                   
                                                   ui2_sp_idx,
                                    &t_sp_stm_info,                                                   
                                    (SIZE_T*)&ui4_rec_sz);                      
                    }                      

                    pt_ele_idxtbl_des->u_info.t_es.ui4_strm_id =                          
                        _mm_hdlr_minfo_get_stream_id(t_sp_stm_info);                      
                    ui2_sp_rec_nums--;                
                }
            }
            else if (e_mode == RANGE_STRM_SEL_MODE_ID_LST)
            {
                /*TBD*/
            }
            else
            {
                return SVCTXR_FAIL;
            }
            if (pt_pre != NULL)
            {
                pt_pre ->pt_next = pt_ele_idxtbl_des;
                pt_pre = pt_ele_idxtbl_des;
            }
            else
            {
                /*first node*/
                pt_pre = pt_ele_idxtbl_des;
                pt_range_info->pt_tbl_describ = pt_ele_idxtbl_des;
            }
    }
    
   /*add 3D second video */
    #if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
    if ( ( pt_media_desc->b_multi_track_3d )
       && (!pt_media_desc->b_is_thumbnail_playback) )
    {
        pt_strm_req = &( pt_running_svc_req->t_strm_reqs_3d_vid );
        if ((( e_mode == RANGE_STRM_SEL_MODE_ACTIVE ) 
                 && ( x_handle_valid( pt_strm_req->h_stream ) == TRUE ))
              || ( e_mode == RANGE_STRM_SEL_MODE_ALL ))
        {
            pt_ele_idxtbl_des = x_mem_alloc( sizeof( MIDXBULD_ELMT_TBL_DESCRIB_T ));
            if ( pt_ele_idxtbl_des == NULL )
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memset( pt_ele_idxtbl_des, 0, sizeof( MIDXBULD_ELMT_TBL_DESCRIB_T ));
            pt_ele_idxtbl_des->u_info.t_es.ui4_strm_id =
            _mm_hdlr_scdb_rec_get_stream_id( pt_strm_req->u.t_scdb.t_rec );
        }
    }
    if ( pt_pre != NULL )
    {
        pt_pre ->pt_next = pt_ele_idxtbl_des;
        pt_pre           = pt_ele_idxtbl_des;
    }
    else
    {
        /*first node*/
        pt_pre                        = pt_ele_idxtbl_des;
        pt_range_info->pt_tbl_describ = pt_ele_idxtbl_des;
    }
    #endif 

    if (pt_pre == NULL)
    {
        /*ASSERT(pt_pre != NULL, ("%s(), pt_pre is NULL",__func__));*/
        x_dbg_stmt(_ERROR_HEADER"mm_hdlr :Caution! no streams available to play in range_alloc\n");
        return  SVCTXR_FAIL;
    }
    else
    {
        pt_pre->pt_next = NULL;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_unsel_strm_in_range_alloc_info
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_unsel_strm_in_range_alloc_info(
                           MIDXBULD_ELMT_TBL_ALLOC_INFO_T*   pt_range_info)
{

    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_free_node = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_elmt_tbl_desc = NULL;
    if (pt_range_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    /*stream selection*/
    pt_elmt_tbl_desc = pt_range_info->pt_tbl_describ;

    while (pt_elmt_tbl_desc != NULL)
    {
        pt_free_node = pt_elmt_tbl_desc;
        pt_elmt_tbl_desc = pt_elmt_tbl_desc->pt_next;
        x_mem_free(pt_free_node);
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_insert_range_to_list
 * Description
 * Input arguments
 * Output arguments
*      - ppt_range_lst
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_insert_range_to_list(
                                            MEDIA_DESCRIBT_T*                 pt_media_desc,
                                            MIDXBULD_ELMT_TBL_ALLOC_INFO_T*   pt_range_info,
                                            MM_PRESENT_RANGE_LIST_T**         ppt_range_lst
                                           )
{


    MIDXBULD_ELMT_TBL_DESCRIB_T**    ppt_ele_tbl_descr;
    MM_PRESENT_RANGE_LIST_T*         pt_range_lst_tail = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_ptr = NULL;
    MM_PRESENT_RANGE_LIST_T*         pt_range_lst = NULL;
    INT8                             ui1_idx;

    /*append a ranget to range list*/
    pt_range_lst =(MM_PRESENT_RANGE_LIST_T*)x_mem_alloc(
                  sizeof(MM_PRESENT_RANGE_LIST_T));
    if ( pt_range_lst == NULL )
    {
       return SVCTXR_NO_RESOURCE;
    }
    /*should be initialized because it contain pointer field*/
    x_memset(pt_range_lst, 0, sizeof(MM_PRESENT_RANGE_LIST_T));

    /* store the range handle */
    pt_range_lst->t_range.ui4_range_id = pt_range_info->ui4_range_id;
    pt_range_lst->t_range.u_range.t_es.ui4_start_pts =
                                pt_range_info->u_range.t_es.ui4_start_pts;
    pt_range_lst->t_range.u_range.t_es.ui4_end_pts =
                                 pt_range_info->u_range.t_es.ui4_end_pts;
    pt_range_lst->t_range.u_range.t_es.ui8_start_pcr = 
                                pt_range_info->u_range.t_es.ui8_start_pcr;
    /* store elemt tbl handle */
    ppt_ele_tbl_descr = &pt_range_lst->t_range.pt_tbl_describ;
    pt_ptr = pt_range_info->pt_tbl_describ;
    while ( pt_ptr != NULL )
    {
       /*If fail to allocate memory ???*/
       *ppt_ele_tbl_descr =
       (MIDXBULD_ELMT_TBL_DESCRIB_T*)x_mem_alloc(sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T));
       if(*ppt_ele_tbl_descr == NULL)
       {
            return SVCTXR_NO_RESOURCE;
       }
       (*ppt_ele_tbl_descr)->ui4_tbl_id = pt_ptr->ui4_tbl_id;
       (*ppt_ele_tbl_descr)->u_info.t_es = pt_ptr->u_info.t_es;

       pt_ptr = pt_ptr->pt_next;
       ppt_ele_tbl_descr = &((*ppt_ele_tbl_descr)->pt_next);
       /* TBD:  memeory free */
    }
    (*ppt_ele_tbl_descr) = NULL;

    /*pt_media_desc->pv_presnt_range_lst = (VOID*)pt_range_lst;*/
    if (!pt_media_desc->pv_presnt_range_lst)
    {
        pt_media_desc->pv_presnt_range_lst = pt_range_lst;
    }
    else
    {
        pt_range_lst_tail = (MM_PRESENT_RANGE_LIST_T*)pt_media_desc->pv_presnt_range_lst;
        for( ui1_idx = 0 ; ui1_idx <  pt_media_desc->ui1_presnt_range_cnts-1 ; ui1_idx++)
         {
            pt_range_lst_tail = pt_range_lst_tail->pt_next_range;
         }
         pt_range_lst_tail->pt_next_range = pt_range_lst;
    }
    pt_range_lst->pt_next_range = NULL;
    *ppt_range_lst = pt_range_lst;
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_midxbuld_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_midxbuld_nty(
                        HANDLE_T                    h_midx_buld,
                        MIDXBULD_NFY_EVENT_TYPE_T   e_event,
                        const VOID*                 pv_data1, /*range handle*/
                        const VOID*                 pv_data2, /*reserved*/
                        const VOID*                 pv_tag)
{
   INT32 i4_ret                      = SVCTXR_OK;
   SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
   MM_SVC_REQ_T*  pt_running_svc_req = NULL;
   HANDLE_T       h_svctx            = NULL_HANDLE;

   pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

   h_svctx = pt_running_svc_req->t_parent.h_svctx;
   i4_ret = _mm_hdlr_send_msg(h_svctx,
                              h_midx_buld,
                              _SVCTX_MSG_MIDXBULD_NTFY,
                              (VOID*)pv_tag,
                              e_event,
                              (UINT32)pv_data1,
                              (UINT32)pv_data2);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_mnav_info_update_process
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_mnav_info_update_process(
                           SVCTX_T*             pt_svctx,
                           MM_NAVI_INFO_TYPE    t_info_type,
                           VOID*                pv_update_info)
{

    if (pt_svctx == NULL)
    {
        return SVCTXR_FAIL;
    }
    switch(t_info_type)
    {
    case MM_NAVI_INFO_DURATION:
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                          SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
                                          ST_UNKNOWN
                                          );
        break;
    default:
        break;
    }

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_mrange_ctrl_event_process
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_mrange_ctrl_event_process(
                           MM_PRESENT_RANGE_LIST_T*          pt_node_range_lst,
                           MM_SVC_REQ_T*                     pt_running_svc_req,
                           MM_RANGE_CTRL_EVENT_T             t_event,
                           UINT32                            ui4_data1)


{
    HANDLE_T                        h_media_desc = NULL_HANDLE;
    MM_RANGE_INFO_T*                pt_mm_range_info = NULL;
    PLAYER_T*                       pt_player = NULL;
    BOOL                            b_is_eos = FALSE;
    BOOL                            b_is_use_idx_tbl = FALSE;
    INT32                           i4_ret = SVCTXR_OK;
    SIZE_T                          z_size = 0;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_range_info = NULL;
    MEDIA_DESCRIBT_T*               pt_media_desc = NULL;
    MM_PRESENT_RANGE_LIST_T*        pt_range_lst     = NULL;
    MM_PRESENT_RANGE_LIST_T*        pt_pre_range_lst = NULL;
    UINT32                          ui4_rang_id = 0;
    SVCTX_T*                        pt_svctx = NULL;

    if (pt_node_range_lst == NULL || pt_running_svc_req == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_player    = pt_running_svc_req->pt_player;
    h_media_desc = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc,
                                   &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    i4_ret = svctx_get_obj( pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK) 
    {
        x_dbg_stmt(_INFO_HEADER"add range failed! line%d\n",__LINE__);
        return i4_ret;
    }

    pt_range_info = (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)(pt_media_desc->pv_curr_index_range_info);
    switch(t_event)
    {
    case MM_RANGE_CTRL_ADD:
        if ((ui4_data1&MM_END_RANGE) != 0)
        {
            b_is_eos = TRUE;
        }

        if (pt_svctx->b_in_stopping == TRUE)
        {
            return i4_ret;
        }
        pt_mm_range_info = x_mem_alloc(sizeof(MM_RANGE_INFO_T));
        if (pt_mm_range_info == NULL)
        {
            return SVCTXR_FAIL;
        }
        /*should initialize it*/
        x_memset(pt_mm_range_info, 0, sizeof(MM_RANGE_INFO_T));
        b_is_use_idx_tbl = (BOOL)(ui4_data1&0xff); /*only mp4*/
        _mm_hdlr_range_info_gen(h_media_desc,
                               pt_mm_range_info,
                               &(pt_node_range_lst->t_range),
                               MM_RANGE_DIR_FORWARD,
                               b_is_eos,
                               b_is_use_idx_tbl);

        /*queue range to demuxer*/
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
       if (pt_media_desc->b_is_bad_intlevd)
       {   
            MIXBULD_START_INFO_T            t_start_info;
            SIZE_T                          z_size = sizeof(MIXBULD_START_INFO_T);
            
            x_memset(&t_start_info, 0, sizeof(MIXBULD_START_INFO_T));

            i4_ret = x_midxbuld_get_info(
                                      pt_media_desc->h_index_info,
                                      MIDXBULD_GET_INFO_TYPE_VIDEO_START_INFO,
                                      (VOID*)&t_start_info,
                                      &z_size);
            if (i4_ret == MINFOR_OK)
            {
                if(!(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                   pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF))
                {
                    pt_mm_range_info->ui8_fileoffset = t_start_info.ui4_offset;
                    pt_mm_range_info->z_range_sz = pt_media_desc->ui8_media_sz - t_start_info.ui4_offset;
                }
            }
            
            if (pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE)
            {
                i4_ret = x_sm_set(pt_running_svc_req->pt_player_act->h_sess,
                         SM_SESS_GNRC_SET_TYPE_ADD_RANGE,
                         pt_mm_range_info,
                         sizeof(MM_RANGE_INFO_T));
            }
            else
            {   
                i4_ret = x_sm_set(pt_media_desc->pt_player_vid->h_sess,
                         SM_SESS_GNRC_SET_TYPE_ADD_RANGE,
                         pt_mm_range_info,
                         sizeof(MM_RANGE_INFO_T));
            }
            if (i4_ret!= SMR_OK)
            {
                DBG_INFO(("mm_hdlr : range adding ERROR!\r\n"));
                _mm_hdlr_send_msg(
                         pt_running_svc_req->t_parent.h_svctx,
                         (UINT32)NULL,
                         _SVCTX_MSG_MRANGE_CTRL_NTFY,
                         NULL,
                         MM_RANGE_CTRL_DEL,
                         (UINT32)NULL,
                         pt_mm_range_info->ui4_range_id);
                i4_ret = SVCTXR_FAIL;
            }
       }
       if (pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
       {
            MIXBULD_START_INFO_T            t_start_info;
            SIZE_T                          z_size = sizeof(MIXBULD_START_INFO_T);
            
            x_memset(&t_start_info, 0, sizeof(MIXBULD_START_INFO_T));

            i4_ret = x_midxbuld_get_info(
                                      pt_media_desc->h_index_info,
                                      MIDXBULD_GET_INFO_TYPE_AUDIO_START_INFO,
                                      (VOID*)&t_start_info,
                                      &z_size);
            if (i4_ret == MINFOR_OK)
            {
                if(!(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                   pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF))
                {
                    pt_mm_range_info->ui8_fileoffset = t_start_info.ui4_offset;
                    pt_mm_range_info->z_range_sz = pt_media_desc->ui8_media_sz - t_start_info.ui4_offset;
                }
                pt_mm_range_info->ui4_1st_pkgnum = t_start_info.ui4_1st_pkgnum;
            }

            i4_ret = x_sm_set(pt_media_desc->pt_player_aud->h_sess,
                         SM_SESS_GNRC_SET_TYPE_ADD_RANGE,
                         pt_mm_range_info,
                         sizeof(MM_RANGE_INFO_T));

            if (i4_ret!= SMR_OK)
            {
                DBG_INFO(("mm_hdlr : range adding ERROR!\r\n"));
                _mm_hdlr_send_msg(
                         pt_running_svc_req->t_parent.h_svctx,
                         (UINT32)NULL,
                         _SVCTX_MSG_MRANGE_CTRL_NTFY,
                         NULL,
                         MM_RANGE_CTRL_DEL,
                         (UINT32)NULL,
                         pt_mm_range_info->ui4_range_id);
                i4_ret = SVCTXR_FAIL;
            }
            pt_media_desc->b_is_set_sec_range = TRUE;
       }    
       else
       {
            i4_ret = x_sm_set(pt_player->h_sess,
                         SM_SESS_GNRC_SET_TYPE_ADD_RANGE,
                         pt_mm_range_info,
                         sizeof(MM_RANGE_INFO_T));
            if (i4_ret!= SMR_OK)
            {
                x_dbg_stmt(_INFO_HEADER"add range failed! line%d\n",__LINE__);
                _mm_hdlr_send_msg(
                         pt_running_svc_req->t_parent.h_svctx,
                         (UINT32)NULL,
                         _SVCTX_MSG_MRANGE_CTRL_NTFY,
                         NULL,
                         MM_RANGE_CTRL_DEL,
                         (UINT32)NULL,
                         pt_mm_range_info->ui4_range_id);
                i4_ret = SVCTXR_FAIL;
            }
            pt_media_desc->b_is_set_sec_range = TRUE;
       }
#else
        i4_ret = x_sm_set(pt_player->h_sess,
                         SM_SESS_GNRC_SET_TYPE_ADD_RANGE,
                         pt_mm_range_info,
                         sizeof(MM_RANGE_INFO_T));
        if (i4_ret!= SMR_OK)
        {
            x_dbg_stmt(_INFO_HEADER"add range failed! line%d\n",__LINE__);
            _mm_hdlr_send_msg(
                     pt_running_svc_req->t_parent.h_svctx,
                     (UINT32)NULL,
                     _SVCTX_MSG_MRANGE_CTRL_NTFY,
                     NULL,
                     MM_RANGE_CTRL_DEL,
                     (UINT32)NULL,
                     pt_mm_range_info->ui4_range_id);
            i4_ret = SVCTXR_FAIL;
        }
#endif        
        /*store back to range list node.*/
        pt_media_desc->ui1_presnt_range_cnts++;
        pt_node_range_lst->pt_range_idxtbl_info = pt_mm_range_info;
        pt_media_desc->b_is_range_indexing = FALSE;
        break;
    case MM_RANGE_CTRL_DEL:
        ui4_rang_id = ui4_data1;
        pt_range_lst = pt_media_desc->pv_presnt_range_lst;
        do
        {
             if(pt_range_lst->t_range.ui4_range_id == ui4_rang_id)
             {
               break;
             }
             pt_pre_range_lst = pt_range_lst;
             pt_range_lst = pt_range_lst->pt_next_range;
        }
        while(pt_range_lst!= NULL);
        if(pt_range_lst == NULL)
        {
            x_dbg_stmt(_INFO_HEADER"add range failed! line%d\n",__LINE__);
            return SVCTXR_FAIL;
        }
        pt_media_desc->ui1_presnt_range_cnts--;
        if ( pt_pre_range_lst == NULL)
        {
            pt_media_desc->pv_presnt_range_lst = pt_range_lst->pt_next_range;
        }
        else
        {
            pt_pre_range_lst->pt_next_range = pt_range_lst->pt_next_range;
        }
        if (pt_media_desc->b_is_dmx_by_idx_tbl)
        {
            z_size = sizeof(UINT32);

            i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                         MIDXBULD_SET_INFO_TYPE_FREE_RANGE,
                                         (VOID*)ui4_rang_id,
                                         &z_size);

            if (i4_ret != MIDXBULDR_OK)
            {
                x_dbg_stmt(_INFO_HEADER"midxbuld free range failed! line%d\n",__LINE__);
                return SVCTXR_FAIL;
            }
        }
        break;
    case MM_RANGE_CTRL_FLUSH:
        break;
    case MM_RANGE_CTRL_BUILD:
        if(!pt_running_svc_req->t_uop_fct_tbl.pf_get_range)
        {
            return SVCTXR_FAIL;
        }
        i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_range(pt_media_desc,
                                                              pt_node_range_lst->t_range.u_range.t_es.ui4_start_pts ,
                                                              pt_range_info);

        z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
        i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                    MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE,
                                    (VOID*)pt_range_info,
                                    &z_size);
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt(_INFO_HEADER"midxbuld alloc range failed! line%d\n",__LINE__);
            _mm_hdlr_range_free(pt_node_range_lst);
            return SVCTXR_FAIL;
        }

        i4_ret = x_midxbuld_start_indexing(
                           pt_media_desc->h_index_info);

        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt(_INFO_HEADER"midxbuld start indexing failed! line%d\n",__LINE__);
            return SVCTXR_FAIL;
        }
        pt_media_desc->b_is_range_indexing = TRUE;

        DBG_INFO((_INFO_HEADER"range start pts %d:!\n",pt_range_info->u_range.t_es.ui4_start_pts));
        DBG_INFO((_INFO_HEADER"range end pts %d:!\n",pt_range_info->u_range.t_es.ui4_end_pts));
        pt_media_desc->h_curr_indexing_range = pt_range_info->ui4_range_id;
        _mm_hdlr_range_free(pt_node_range_lst);

        break;
    default:
        break;
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_midxbuld_event_process
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_midxbuld_event_process(
                           HANDLE_T                    h_midx_buld,
                           HANDLE_T                    h_range,
                           MIDXBULD_NFY_EVENT_TYPE_T   e_event,
                           MM_SVC_REQ_T*               pt_running_svc_req)
{
   UINT32                           ui4_flag = 0;
   SIZE_T                           z_size = 0;
   MM_PRESENT_RANGE_LIST_T*         pt_range_lst = NULL;
   MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info = NULL;
   MEDIA_DESCRIBT_T*                pt_media_desc = NULL;
   RANGE_STRM_SEL_MODE_T            e_sel_mode;
   INT32                            i4_ret = 0;


    if (pt_running_svc_req == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,
                                   &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    
    if (pt_media_desc->pv_curr_index_range_info == NULL)
    {
        pt_media_desc->pv_curr_index_range_info = x_mem_alloc(sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T));
        if(pt_media_desc->pv_curr_index_range_info == NULL)
        {
            ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));
            return SVCTXR_NO_RESOURCE;
        }
        x_memset(pt_media_desc->pv_curr_index_range_info, 0, sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T));
     }
     pt_range_info = (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pt_media_desc->pv_curr_index_range_info;

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
       pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
    {
        pt_media_desc->b_is_dmx_by_idx_tbl = TRUE;
    }

    switch(e_event)
    {
    case MIDXBULD_NFY_EVENT_INDEXING_DONE:
        {
             pt_media_desc->b_has_key_table = TRUE;
            if ( !pt_media_desc->b_is_dmx_by_idx_tbl)
            {
                x_dbg_stmt(_INFO_HEADER"this file create index table success !!\n");
                pt_media_desc->b_is_keytbl_indexing = FALSE;
                e_sel_mode = RANGE_STRM_SEL_MODE_ACTIVE;

                if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                   pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                {
                    e_sel_mode = RANGE_STRM_SEL_MODE_ALL;
                }

                i4_ret = _mm_hdlr_add_range_func(pt_running_svc_req, e_sel_mode);
                if (i4_ret != SVCTXR_OK)
                {
                    return i4_ret;
                }
            }
            else if(pt_media_desc->ui1_presnt_range_cnts < 2)
            {
                if (pt_media_desc->b_is_keytbl_indexing)
                {
                    x_dbg_stmt(_INFO_HEADER"this file create index table success !!\n");
                    pt_media_desc->b_is_keytbl_indexing = FALSE;                    
                    e_sel_mode = RANGE_STRM_SEL_MODE_ALL;
                    i4_ret = _mm_hdlr_sel_strm_in_range_alloc_info(pt_running_svc_req,
                                                               pt_range_info,
                                                               e_sel_mode,
                                                               NULL);
                    if(!pt_running_svc_req->t_uop_fct_tbl.pf_get_range)
                    {
                        return SVCTXR_FAIL;
                    }
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_range(pt_media_desc,
                                                                        (UINT32)pt_running_svc_req->t_init_pos_info.ui8_pos_val,
                                                                        pt_range_info);
                    z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
                    i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                                 MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE,
                                              (VOID*)pt_range_info,
                                                 &z_size);
                    if (i4_ret != MIDXBULDR_OK)
                    {
                        x_dbg_stmt(_INFO_HEADER"midxbuld alloc range failed! line%d\n",__LINE__);
                        return SVCTXR_FAIL;
                    }
                    
                    i4_ret = x_midxbuld_start_indexing(pt_media_desc->h_index_info);
                    if (i4_ret != MIDXBULDR_OK)
                    {
                        x_dbg_stmt(_INFO_HEADER"midxbuld start indexing failed! line%d\n",__LINE__);
                        return SVCTXR_FAIL;
                    }
                    pt_media_desc->b_is_range_indexing = TRUE;
                    pt_media_desc->h_curr_indexing_range = pt_range_info->ui4_range_id;
                }
                else if (pt_media_desc->b_is_range_indexing)
                {
                    pt_media_desc->b_is_range_indexing = FALSE;
                    
                    if (pt_media_desc->h_curr_indexing_range == h_range)
                    {
                        i4_ret = _mm_hdlr_insert_range_to_list(pt_media_desc,
                                                                pt_range_info,
                                                                &pt_range_lst);
                        if (pt_range_info->u_range.t_es.ui4_end_pts
                                 == (UINT32)MIDXBULD_IDX_RANGE_FULL)
                        {
                                    ui4_flag = MM_END_RANGE;
                        }
                        else
                        {
                                    ui4_flag = MM_NOT_END_RANGE;
                        }
                        ui4_flag |= pt_media_desc->b_is_dmx_by_idx_tbl;
                        i4_ret = _mm_hdlr_send_msg(pt_running_svc_req->t_parent.h_svctx,
                                                   (UINT32)NULL,
                                                   _SVCTX_MSG_MRANGE_CTRL_NTFY,
                                                   NULL,
                                                   MM_RANGE_CTRL_ADD,
                                                  (UINT32)(pt_range_lst), /*range info*/
                                                   (UINT32)ui4_flag); /*crtl type sepcific data*/
                        return SVCTXR_OK;
                    }
                    if ((pt_media_desc->ui1_presnt_range_cnts + 1 < 2) &&
                          (pt_range_info->u_range.t_es.ui4_end_pts != (UINT32)MIDXBULD_IDX_RANGE_FULL) &&
                          !pt_media_desc->b_is_range_indexing)
                    {
                        /*build next range*/
                        if(!pt_running_svc_req->t_uop_fct_tbl.pf_get_range)
                        {
                            return SVCTXR_FAIL;
                        }
                        i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_range(
                                     pt_media_desc,
                                pt_range_info->u_range.t_es.ui4_end_pts,
                                pt_range_info);

                        z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
                        i4_ret = x_midxbuld_set_info(pt_media_desc->h_index_info,
                                                     MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE,
                                                  (VOID*)pt_range_info,
                                                     &z_size);
                        if (i4_ret != MIDXBULDR_OK)
                        {
                            x_dbg_stmt(_INFO_HEADER"midxbuld alloc range failed! line%d, ret %d\n",__LINE__, i4_ret);
                            return SVCTXR_FAIL;
                        }

                        i4_ret = x_midxbuld_start_indexing(pt_media_desc->h_index_info);

                        if (i4_ret != MIDXBULDR_OK)
                        {
                            x_dbg_stmt(_INFO_HEADER"midxbuld alloc start indexing failed! line%d\n",__LINE__);
                            return SVCTXR_FAIL;
                        }
                        pt_media_desc->b_is_range_indexing = TRUE;
                      pt_media_desc->h_curr_indexing_range = pt_range_info->ui4_range_id;
                    }
                }
                else
                {
                            /* ASSERT*/
                }
            }
            return SVCTXR_OK;
        }
    case MIDXBULD_NFY_EVENT_INDEXING_ABORT:
        pt_media_desc->b_is_range_indexing = FALSE;
        break;
    case MIDXBULD_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MIDXBULD_NFY_EVENT_ERROR:
        {
            x_dbg_stmt(_ERROR_HEADER"this file create index table fail !!\n");
            if ( !pt_media_desc->b_is_dmx_by_idx_tbl)
            {
                pt_media_desc->b_is_no_idexing = TRUE;
                pt_media_desc->b_is_keytbl_indexing = FALSE;
                e_sel_mode = RANGE_STRM_SEL_MODE_ACTIVE;

                if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                   pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                {
                    e_sel_mode = RANGE_STRM_SEL_MODE_ALL;
                }
                
                i4_ret = _mm_hdlr_add_range_func(pt_running_svc_req, e_sel_mode);
                if (i4_ret != SVCTXR_OK)
                {
                    return i4_ret;
                }
            }
            else
            {
              pt_media_desc->b_is_range_indexing = FALSE;
            }
        }
        /* remove the range in range list */
        /* TBD */
        break;
    }
    
    return SVCTXR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_add_range_func
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_add_range_func( 
                    MM_SVC_REQ_T*               pt_running_svc_req, 
                    RANGE_STRM_SEL_MODE_T       e_mode)
{
    MEDIA_DESCRIBT_T*               pt_media_desc = NULL;
    INT32                           i4_ret = 0;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T  t_range_info;
    MM_PRESENT_RANGE_LIST_T*        pt_range_lst = NULL;
    
    if (pt_running_svc_req == NULL)
    {
        ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r \n",__func__));   
        return SVCTXR_INV_ARG; 
    }

    x_memset(&t_range_info, 0, sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T));
    
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }
    do 
    {
        i4_ret = _mm_hdlr_sel_strm_in_range_alloc_info(pt_running_svc_req,
                                                               &t_range_info,
                                                               e_mode,
                                                               NULL);

        if (pt_media_desc->pt_strm_info == NULL)
        { 
            if(!pt_running_svc_req->t_uop_fct_tbl.pf_get_range)
            {
                i4_ret = SVCTXR_FAIL;
                break;
            }
            i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_range(
                                pt_media_desc,
                                (UINT32)pt_running_svc_req->t_init_pos_info.ui8_pos_val,
                                &t_range_info);
            if (i4_ret != SVCTXR_OK)
            {
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }
        /*append a range to range list*/
        i4_ret = _mm_hdlr_insert_range_to_list(
                     pt_media_desc,
                     &t_range_info,
                     &pt_range_lst);
        if (i4_ret != SVCTXR_OK)
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        i4_ret = _mm_hdlr_send_msg(
                         pt_running_svc_req->t_parent.h_svctx,
                         (UINT32)NULL,
                         _SVCTX_MSG_MRANGE_CTRL_NTFY,
                         NULL,
                         MM_RANGE_CTRL_ADD,
                         (UINT32)(pt_range_lst),
                         (UINT32)FALSE);
        if (i4_ret != SVCTXR_OK)
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }
    }while(0);
    
    _mm_hdlr_unsel_strm_in_range_alloc_info(&t_range_info); 

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_media_preprocess
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_media_preprocess( MM_SVC_REQ_T*  pt_running_svc_req )
{    
    SIZE_T                           z_rec_length = 0;
    HANDLE_T                         h_minfo;
    HANDLE_T                         h_svctx;
    SVCTX_T*                         pt_svctx;
    INT32                            i4_ret = SVCTXR_OK;
    UINT16                           ui4_vid_rec_nums = 0;
    UINT16                           ui4_aud_rec_nums = 0;
    UINT8                            ui1_vid_elem_cnt = 0;
    UINT16                           ui4_sp_rec_nums = 0;
    UINT8                            ui1_i = 0;

    MIDXBULD_SOURCE_INFO_T           t_src_info;
    MIDXBULD_KEY_INFO_T              t_key_info;
    MINFO_INFO_T                     t_minfo_gen;
    MINFO_INFO_T                     t_minfo_idx;
    MINFO_INFO_T                     t_minfo_rec;
    MINFO_INFO_T                     t_minfo_aud;
#if SUPPORT_MULTI_INFO
    MINFO_INFO_T                     t_scale_info;
    MIDXBULD_KEY_INFO_T*             pt_key_info;
#endif
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T   t_range_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T*     pt_eletbl_dec;
    MEDIA_DESCRIBT_T*                pt_media_desc;
    MINFO_TYPE_STM_ATRBT_T*          pt_vid_attr;
    
    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r \n",__func__));    
    if ( pt_running_svc_req == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    h_svctx = pt_running_svc_req->t_parent.h_svctx;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }
    h_minfo = pt_media_desc->h_media_info; 
    z_rec_length = sizeof(MINFO_INFO_T);

    x_memset(&t_range_info, 0, sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T));
    x_memset(&t_minfo_gen, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_minfo_idx, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_minfo_aud, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_src_info, 0, sizeof(MIDXBULD_SOURCE_INFO_T));
#if SUPPORT_MULTI_INFO
    x_memset(&t_scale_info, 0, sizeof(MINFO_INFO_T));
#endif

    /* mmp no minfo */
    if (!MM_HDLR_WITH_MINFO(pt_media_desc))
    {
        pt_media_desc->b_is_no_idexing = TRUE;
        
        i4_ret = _mm_hdlr_add_range_func(pt_running_svc_req, RANGE_STRM_SEL_MODE_ACTIVE);
        if (i4_ret != SVCTXR_OK)
        {
           return i4_ret;
        }

        if (((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)&&(pt_media_desc->e_src_sub_type != SRC_SUB_TYPE_VESTEL_TS))|| 
            (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES &&
             (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3 ||
              pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC ||
              pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM)))
        {
            MM_SVC_DUR_INFO_T   t_dur_code;
            t_dur_code.t_dur_type = MM_SVC_DUR_TYPE_ALL;
            t_dur_code.ui2_idx = 1;
            t_dur_code.ui8_duration = (UINT64)-1;
            pt_running_svc_req->t_uop_fct_tbl.pf_get_dur(pt_media_desc,&t_dur_code);
            pt_media_desc->ui8_media_dur = t_dur_code.ui8_duration;            
        }
        return SVCTXR_OK;
    }

    /* get total duration */
    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_GENERIC,
                              1,
                              &t_minfo_gen ,
                              &z_rec_length );
    if (i4_ret != MINFOR_OK)
    {
       x_dbg_stmt(_ERROR_HEADER"x_minfo_get_info(MINFO_INFO_TYPE_GENERIC) i4_ret = %d line=%d\n", i4_ret, __LINE__);
       return i4_ret;
    }

    pt_media_desc->ui8_media_dur = t_minfo_gen.u.t_general_info.ui8_pb_duration;
    
    /* mmp with minfo but no indexbuld*/
    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
   
    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_IDX_INFO,
                              1,
                              &t_minfo_idx,
                              &z_rec_length );
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"x_minfo_get_info(MINFO_INFO_TYPE_IDX_INFO) i4_ret = %d line=%d\n", i4_ret, __LINE__);
        return SVCTXR_FAIL;
    }
    
    /* file no indexing information , ignore indexing build.ex PS file*/
    if((t_minfo_idx.u.t_idx_info.ui4_idx_type & MM_MINFO_IDX_TYPE_NONE) == MM_MINFO_IDX_TYPE_NONE)
    {
        x_dbg_stmt(_INFO_HEADER" this file is no index table! \n");
        pt_media_desc->b_is_no_idexing = TRUE;
        i4_ret = _mm_hdlr_add_range_func(pt_running_svc_req, RANGE_STRM_SEL_MODE_ALL);
        if (i4_ret != SVCTXR_OK)
        {
            return i4_ret;
        }
        return SVCTXR_OK;
    }

    /*file with idx info*/    
    /*mp4 dlna moov box size check*/
    if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
        && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
            == MEDIA_CONTNR_SUBTYPE_MP4))
    {
        if((pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)
            &&(pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA))
        {
           
            if(t_minfo_idx.u.t_idx_info.ui8_idx_sz > DLNA_SUPPORTED_MP4_MOOV_SIZE_MAX)
            {
                mm_hdlr_invoke_client_svc_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT,
                                    ST_UNKNOWN);
                return SVCTXR_NOT_SUPPORT;
            }
        }
        if(pt_media_desc->ui8_media_dur > (UINT64)13*3600*90000)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT,
                                ST_UNKNOWN);
            return SVCTXR_NOT_SUPPORT;
        }
    }

    /************* Open Index Builder handle ********************/
    i4_ret = x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_VIDEO_ATTR,&ui4_vid_rec_nums);   
    if (MINFOR_OK != i4_ret)
    {
        x_dbg_stmt(_ERROR_HEADER"x_minfo_get_num_recs(MINFO_INFO_TYPE_VIDEO_ATTR) i4_ret = %d line=%d\n", i4_ret, __LINE__);
        return i4_ret;
    }
    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_VIDEO_ATTR,
                              1,
                              &t_minfo_rec,
                              &z_rec_length);
    if (MINFOR_OK != i4_ret)
    {
        x_dbg_stmt(_ERROR_HEADER"x_minfo_get_num_recs(MINFO_INFO_TYPE_VIDEO_ATTR) i4_ret = %d line=%d\n", i4_ret, __LINE__);
    }
    pt_vid_attr = &(t_minfo_rec.u.t_stm_attribute);
    
    
    if(!MM_HDLR_AUDIO_ES_WITH_MINFO(pt_media_desc))
    {
        pt_media_desc->b_non_seekable = t_minfo_gen.u.t_general_info.b_non_seekable;
    }
    
    i4_ret = x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_AUDIO_ATTR, &ui4_aud_rec_nums);
    if (MINFOR_OK != i4_ret)
    {
        x_dbg_stmt(_ERROR_HEADER"x_minfo_get_num_recs(MINFO_INFO_TYPE_AUDIO_ATTR) i4_ret = %d line=%d\n",i4_ret, __LINE__);
        return i4_ret;
    }
    if (ui4_aud_rec_nums >= 1)
    {
        z_rec_length = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR, 1,
                     &t_minfo_aud,
                     &z_rec_length);
        if ( i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
        t_src_info.ui4_cbr_sub_vbr =
            t_minfo_aud.u.t_stm_attribute.u.t_avi_audio_stm_attr.ui4_cbr_sub_vbr;
    }
    else
    {
        t_src_info.ui4_cbr_sub_vbr = 0;
    }

    i4_ret = x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_SP_ATTR, &ui4_sp_rec_nums);
    if (MINFOR_OK != i4_ret)
    {
        x_dbg_stmt(_ERROR_HEADER"x_minfo_get_num_recs(MINFO_INFO_TYPE_SP_ATTR) i4_ret = %d line=%d\n",i4_ret, __LINE__);
        return i4_ret;
    }


    i4_ret = _mm_hdlr_sel_strm_in_range_alloc_info(pt_running_svc_req,
                                                  &t_range_info,
                                                  RANGE_STRM_SEL_MODE_ALL,
                                                  NULL);
    if (i4_ret != SVCTXR_OK)
    {
       return SVCTXR_FAIL;
    }

    if(!pt_running_svc_req->t_uop_fct_tbl.pf_get_range)
    {
        return SVCTXR_FAIL;
    }
    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_range(
                                                            pt_media_desc,
                                                            (UINT32)pt_running_svc_req->t_init_pos_info.ui8_pos_val,
                                                            &t_range_info);
    /*ui4_vid_rec_nums+ui4_aud_rec_nums;*/
    /*1 audio + 1 video by default , for multi-audio case it should be modified later*/
    t_src_info.ui1_total_strm_num = (UINT8)ui4_vid_rec_nums + ui4_aud_rec_nums + ui4_sp_rec_nums;
    t_src_info.ui4_idx_type = t_minfo_idx.u.t_idx_info.ui4_idx_type;
    t_src_info.ui8_idx_offset = t_minfo_idx.u.t_idx_info.ui8_idx_offset;
    t_src_info.ui8_idx_size   = t_minfo_idx.u.t_idx_info.ui8_idx_sz;
    t_src_info.ui8_data_offset = t_minfo_idx.u.t_idx_info.ui8_data_offset;
    t_src_info.ui8_data_size   = t_minfo_idx.u.t_idx_info.ui8_data_sz;
    t_src_info.ui4_total_duration = (UINT32)t_minfo_gen.u.t_general_info.ui8_pb_duration;
    t_src_info.ui4_packet_sz = pt_vid_attr->u.t_asf_video_stm_attr.ui4_packet_sz;
    x_memcpy(&t_src_info.t_mp4_extra_info, &t_minfo_idx.u.t_idx_info.t_mp4_extra_info, sizeof(MP4_MINFO_EXTRA_INFO_T));
    /*for mkv idxbuld*/
    t_src_info.ui8_seg_data_offset = t_minfo_idx.u.t_idx_info.ui8_segment_data_offset;
    t_src_info.ui8_seg_tm_scale = t_minfo_idx.u.t_idx_info.ui8_seg_timecode_scale;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /*for bad interleaved*/
    t_src_info.b_is_bad_interleaved = t_minfo_gen.u.t_general_info.b_is_bad_interleaved;
    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
       pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
    {    
         t_src_info.ui4_1st_pkgnum   = t_minfo_idx.u.t_idx_info.ui4_1st_pkgnum;
    }
    else
    {
         t_src_info.ui4_1st_pkgnum   = 0;
    }
#endif    
#if SUPPORT_MULTI_INFO
    /*get scale info*/
    z_rec_length = sizeof(MINFO_INFO_T);
    t_src_info.pt_next = NULL;
    t_key_info.pt_next = NULL;
    i4_ret = x_minfo_get_info(h_minfo,
                              MINFO_INFO_TYPE_SCALE_INFO,
                              1,
                              &t_scale_info,
                              &z_rec_length);
    if (i4_ret == MINFOR_OK)
    {
        x_dbg_stmt(_INFO_HEADER"this mkv file has scale info !\n");
        MIDXBULD_SOURCE_INFO_T* pt_src_info;
        /*allocdate 2th src info*/
        t_src_info.pt_next = (MIDXBULD_SOURCE_INFO_T*)
            x_mem_alloc(sizeof(MIDXBULD_SOURCE_INFO_T));
        if(!t_src_info.pt_next)
        {
            return SVCTXR_NO_RESOURCE;
        }
        x_memset(t_src_info.pt_next, 0, sizeof(MIDXBULD_SOURCE_INFO_T));
        /*allocate 2th key info*/
        t_key_info.pt_next = (MIDXBULD_KEY_INFO_T*)
            x_mem_alloc(sizeof(MIDXBULD_KEY_INFO_T));
        if(!t_key_info.pt_next)
        {
            return SVCTXR_NO_RESOURCE;
        }
        x_memset(t_key_info.pt_next, 0, sizeof(MIDXBULD_KEY_INFO_T));

        t_key_info.pt_next->ui4_scale = t_scale_info.u.t_scale_info.ui4_scale;
        t_key_info.pt_next->ui4_rate= t_scale_info.u.t_scale_info.ui4_rate;
        t_key_info.pt_next->ui4_strm_id = t_scale_info.u.t_scale_info.ui4_strm_id;

        pt_src_info = t_src_info.pt_next;
        /*get scale source info*/
        pt_src_info->ui4_cbr_sub_vbr = 0;
        pt_src_info->ui4_packet_sz = 0;
        pt_src_info->ui1_total_strm_num = 1;
        pt_src_info->ui4_idx_type = t_scale_info.u.t_scale_info.ui4_idx_type;
        pt_src_info->ui8_idx_offset = t_scale_info.u.t_scale_info.ui8_idx_offset;
        pt_src_info->ui8_idx_size   = t_scale_info.u.t_scale_info.ui8_idx_sz;
        pt_src_info->ui8_data_offset = t_scale_info.u.t_scale_info.ui8_data_offset;
        pt_src_info->ui8_data_size   = t_scale_info.u.t_scale_info.ui8_data_sz;
        pt_src_info->ui4_total_duration = (UINT32)t_scale_info.u.t_scale_info.ui8_pb_duration;
        pt_src_info->ui8_seg_data_offset = t_scale_info.u.t_scale_info.ui8_segment_data_offset;
        pt_src_info->ui8_seg_tm_scale = t_scale_info.u.t_scale_info.ui8_timecode_scale;
    }
#endif

    /*assume video stream should be fist entry of t_range_info*/
    /*assume build 1video + 1audio key table, not consider subtitle case*/
    pt_eletbl_dec = t_range_info.pt_tbl_describ;
    for (ui1_i = 0 ; ui1_i < (UINT8)(ui4_vid_rec_nums + ui4_aud_rec_nums + ui4_sp_rec_nums) ; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if (pt_eletbl_dec == NULL)
        {
            break;
        }
        t_src_info.t_es_map[ui1_i] = pt_eletbl_dec->u_info.t_es;
        pt_eletbl_dec = pt_eletbl_dec->pt_next;
        if (t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO)
        {
            ui1_vid_elem_cnt = ui1_i;
        }
    }

    t_key_info.ui4_strm_id = t_src_info.t_es_map[ui1_vid_elem_cnt].ui4_strm_id; 
    /*now only asf use this fileds*/
    x_memcpy( &( t_key_info.aui1_idx_strm_map[0] ), 
              &( t_minfo_idx.u.t_idx_info.aui1_idx_strm_map[0] ), 16 );
    t_key_info.ui4_scale = t_src_info.t_es_map[ui1_vid_elem_cnt].ui4_scale;
    t_key_info.ui4_rate = t_src_info.t_es_map[ui1_vid_elem_cnt].ui4_rate;
    t_key_info.ui4_sample_sz = t_src_info.t_es_map[ui1_vid_elem_cnt].ui4_sample_sz;
    t_key_info.ui4_avg_bytes_per_sec = t_src_info.t_es_map[ui1_vid_elem_cnt].ui4_avg_bytes_per_sec;

    /*now only asf use this fileds*/        
    x_memcpy( &( t_key_info.aui1_idx_strm_map[0] ), &( t_minfo_idx.u.t_idx_info.aui1_idx_strm_map[0] ), 16 );
    
    do
    {
        if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
            pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
            && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
        {
            i4_ret = x_midxbuld_mopen(pt_media_desc->pt_media_feeder,
                                      0,
                                      (x_midxbuld_nfy_fct)_mm_hdlr_midxbuld_nty,
                                      pt_svctx,
                                      &pt_media_desc->t_media_fmt,
                                      &t_src_info,
                                      &t_key_info,
                                      &pt_media_desc->h_index_info);
        }
        else
        {
            i4_ret = x_midxbuld_fopen(
                                    pt_media_desc->ps_media_path,
                                  (x_midxbuld_nfy_fct)_mm_hdlr_midxbuld_nty,
                                  pt_svctx,&pt_media_desc->t_media_fmt,
                                    &t_src_info,
                                    &t_key_info,
                                  &pt_media_desc->h_index_info);
        }
        if (i4_ret != MIDXBULDR_OK)
        {
            /*do not return fail when key table build failed, so that media file can be played normally*/
           x_dbg_stmt(_ERROR_HEADER"Open midxbuld info error! i4_ret = %d, line%d\n", i4_ret, __LINE__);
           if ((pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_CONTAINER) ||
                 (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                 != MEDIA_CONTNR_SUBTYPE_MP4))
           {
                pt_media_desc->b_is_no_idexing = TRUE; 
                i4_ret = _mm_hdlr_add_range_func(pt_running_svc_req, RANGE_STRM_SEL_MODE_ALL);
               if (i4_ret != SVCTXR_OK)
               {
                   break;
               }
           }
           break;
        }
        pt_media_desc->b_is_keytbl_indexing = TRUE;
        pt_media_desc->h_key_tbl = t_key_info.ui4_tbl_id;
#if SUPPORT_MULTI_INFO
        if(t_key_info.pt_next)
        {
            pt_media_desc->h_key_tbl_ext[0] = t_key_info.pt_next->ui4_tbl_id;
        }
#endif
    }while(0);

    _mm_hdlr_unsel_strm_in_range_alloc_info(&t_range_info);
    
#if SUPPORT_MULTI_INFO
    pt_key_info = t_key_info.pt_next;
    while(pt_key_info)
    {
        t_key_info.pt_next = pt_key_info->pt_next;
        x_mem_free(pt_key_info);
        pt_key_info = t_key_info.pt_next;
    }
    t_key_info.pt_next = NULL;
#endif
                   
    return i4_ret;
}

static INT32 _mm_hdlr_media_close(HANDLE_T h_media_desc)
{
    INT32                         i4_ret;
    MEDIA_DESCRIBT_T*             pt_media_desc;
    MM_PRESENT_RANGE_LIST_T*      pt_range_lst;
    MM_PRESENT_RANGE_LIST_T*      pt_range;
    MIDXBULD_ELMT_TBL_DESCRIB_T*  pt_range_desc;

    MM_RANGE_ELMT_IDXTBL_T*          pt_range_table_info;

    i4_ret = _mm_hdlr_get_media_obj(h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    /*clean the range list*/
    pt_range_lst =(MM_PRESENT_RANGE_LIST_T*)pt_media_desc->pv_presnt_range_lst;
    while (pt_range_lst!=NULL)
    {
       /*clean range describtion*/
       while (pt_range_lst->t_range.pt_tbl_describ != NULL)
       {
          pt_range_desc = pt_range_lst->t_range.pt_tbl_describ->pt_next;
          x_mem_free(pt_range_lst->t_range.pt_tbl_describ);
          pt_range_lst->t_range.pt_tbl_describ = pt_range_desc;
       }

       while ( pt_range_lst->pt_range_idxtbl_info != NULL)
       {
            MM_RANGE_INFO_T* pt_range_idxtbl_info = pt_range_lst->pt_range_idxtbl_info;
            MM_RANGE_DECODER_T* pt_range_decoder_info = NULL;
            MM_RANGE_DECODER_T* pt_range_decoder_info_next = NULL;
            /* free content encodings for mkv */
            pt_range_decoder_info = pt_range_idxtbl_info->pt_vid_decoder_info;
            while (pt_range_decoder_info)
            {
                if (pt_range_decoder_info->pv_encoding_string != NULL)
                {
                    x_mem_free(pt_range_decoder_info->pv_encoding_string);
                    pt_range_decoder_info->pv_encoding_string = NULL;
                }
                pt_range_decoder_info_next = pt_range_decoder_info->pt_next;
                x_mem_free(pt_range_decoder_info);
                pt_range_decoder_info = pt_range_decoder_info_next;
            }
            
            pt_range_decoder_info = pt_range_idxtbl_info->pt_aud_decoder_info;
            while (pt_range_decoder_info)
            {
                if (pt_range_decoder_info->pv_encoding_string != NULL)
                {
                    x_mem_free(pt_range_decoder_info->pv_encoding_string);
                    pt_range_decoder_info->pv_encoding_string = NULL;
                }
                if(pt_range_decoder_info->pui1_codec_private != NULL)
                {
                     x_mem_free(pt_range_decoder_info->pui1_codec_private);
                     pt_range_decoder_info->pui1_codec_private = NULL;
                }
                pt_range_decoder_info_next = pt_range_decoder_info->pt_next;
                x_mem_free(pt_range_decoder_info);
                pt_range_decoder_info = pt_range_decoder_info_next;
            }
            
            while (pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst != NULL)
            {
                pt_range_table_info = pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst->pv_next_tbl;
                x_mem_free(pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst);
                pt_range_lst->pt_range_idxtbl_info->pt_idxtbl_lst = pt_range_table_info;
            }
            /*for mkv track timecode scale*/
            if (pt_range_lst->pt_range_idxtbl_info->pt_track_tm_scale_lst != NULL)
            {
                x_mem_free(pt_range_lst->pt_range_idxtbl_info->pt_track_tm_scale_lst);
            }
#if SUPPORT_MULTI_INFO
            pt_range_lst->pt_range_idxtbl_info = pt_range_idxtbl_info->pt_next_scale;
#else
            pt_range_lst->pt_range_idxtbl_info = NULL;
#endif
            x_mem_free(pt_range_idxtbl_info);
       }
       pt_range = pt_range_lst->pt_next_range;
       x_mem_free(pt_range_lst);
       pt_range_lst = pt_range;

    }

    if (pt_media_desc->pv_scdb_recs!=NULL)
    {
        x_mem_free(pt_media_desc->pv_scdb_recs);
        pt_media_desc->pv_scdb_recs =NULL;
    }


    if (pt_media_desc->pv_sess_data!=NULL)
    {
        x_mem_free(pt_media_desc->pv_sess_data);
        pt_media_desc->pv_sess_data =NULL;
    }

    /*Closed in x_svctx_media_close()*/
    if (x_handle_valid(pt_media_desc->h_index_info))
    {
        x_midxbuld_close(pt_media_desc->h_index_info);
    }
    /*add unselect stream here.*/
    if(pt_media_desc->pv_curr_index_range_info != NULL)
    {
        _mm_hdlr_unsel_strm_in_range_alloc_info((MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)
                                                 pt_media_desc->pv_curr_index_range_info);
        x_mem_free(pt_media_desc->pv_curr_index_range_info);
        pt_media_desc->pv_curr_index_range_info = NULL;
    }
    pt_media_desc->pv_presnt_range_lst = NULL;
    pt_media_desc->ui1_presnt_range_cnts = 0;
    pt_media_desc->ui1_stream_eos_status = 0;
    pt_media_desc->b_is_range_indexing = FALSE;
#ifdef ENABLE_NAUTILUS
    if(pt_media_desc->t_drm_ctrl.h_timer != NULL_HANDLE)
    {        
        x_timer_stop (pt_media_desc->t_drm_ctrl.h_timer);
        x_timer_delete(pt_media_desc->t_drm_ctrl.h_timer);
        pt_media_desc->t_drm_ctrl.h_timer = NULL_HANDLE;
    }
#endif
    /*free range info....*/
    /*TBD*/
    /*TBD*/
    /*should invokd uop handler close */

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_is_blocked_service
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _mm_hdlr_is_blocked_service(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    BOOL                        b_auto_notify_client
                    )
{
    SVC_BLOCK_COND_T e_old_block_cond = SVC_BLOCK_COND_PASSED;
    SVC_BLOCK_COND_T e_new_block_cond = SVC_BLOCK_COND_PASSED;

    if (pt_running_svc_req == NULL) 
    {
        x_dbg_stmt(_ERROR_HEADER"%s(), pt_running_svc_req is NULL\n", __func__);
        return FALSE;
    }

    e_old_block_cond = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);

    if (pt_running_svc_req->t_parent.b_force_unblock == TRUE) {
        e_new_block_cond = SVC_BLOCK_COND_PASSED;
        rating_chker_set_block_cond(
                            pt_running_svc_req->pt_rating_chker,
                            e_new_block_cond
                            );
    } else {
        e_new_block_cond = rating_chker_check(
                            pt_running_svc_req->pt_rating_chker,
                            pt_running_svc_req->t_src_info.s_src_grp_name,
                            pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id,
                            e_check_guide,
                            crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event)
                            );
    }

    if (b_auto_notify_client == FALSE) {
        return TRUE;
    }

    switch (e_new_block_cond) {
    case SVC_BLOCK_COND_PASSED:
    case SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET:
        if (e_old_block_cond == SVC_BLOCK_COND_PASSED
            || e_old_block_cond == SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET) {
            return FALSE;
        }
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_UNBLOCKED,
                    ST_UNKNOWN
                    );
       /* ch_hdlr_resend_crnt_evn_nfy(
                    pt_svctx,
                    pt_running_svc_req
                    );*/
        return FALSE;
    case SVC_BLOCK_COND_USER_BLOCK_CH:
    case SVC_BLOCK_COND_USER_BLOCK_INP:
    case SVC_BLOCK_COND_EXCEED_RATING:
    default:
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_BLOCKED,
                    ST_UNKNOWN
                    );
        return TRUE;
    }
}

/*add for dts support*/
typedef enum {
    LITTLE_EN = 0,
    BIG_EN
} DTS_EN_T;

#ifndef LINUX_TURNKEY_SOLUTION
extern void AUD_SetDTSInfo (DTS_EN_T endian);
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_update_scdb_recs
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_update_scdb_recs(HANDLE_T          h_media ,
                                       HANDLE_T          h_conn)
{
    UINT16                              ui2_i               = 0;
    UINT16                              ui2_video_track_cnt = 0;
    UINT16                              ui2_audio_track_cnt = 0;
    INT32                               i4_ret;
    UINT32                              ui4_sz;
    UINT16                              ui4_vid_rec_nums  = 0;
    UINT16                              ui4_aud_rec_nums  = 0;
    UINT16                              ui4_sp_rec_nums   = 0;
    UINT16                              ui2_scdb_rec_nums =0;
    SIZE_T                              z_rec_length;
    PLAYBACK_HANDLER_QUEUE_FILE_T       t_file_item;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_rec_buf   = NULL;
    MINFO_INFO_T                        t_minfo_rec[MIDXBULD_ELMT_TBL_MAX];
    MINFO_INFO_T*                       pt_minfo_rec_buf;
    MEDIA_DESCRIBT_T*                   pt_media_desc;
    SVCTX_T*                            pt_svctx;
    MINFO_INFO_T                        t_minfo_gen_rec;
    /*add for font info*/
    MINFO_INFO_T                        t_minfo_font_rec;
    HANDLE_T                            h_minfo;
    UINT32                              ui4_scdb_tag;
    DATA_FMT_T                          e_format          = DATA_FMT_UNKNOWN;
    DTS_EN_T                            e_dts_edn;

    i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    x_memset(t_minfo_rec,       0, MIDXBULD_ELMT_TBL_MAX * sizeof(MINFO_INFO_T));
    x_memset(&t_minfo_gen_rec,  0, sizeof(MINFO_INFO_T));
    x_memset(&t_minfo_font_rec, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_file_item,      0, sizeof(PLAYBACK_HANDLER_QUEUE_FILE_T));
    
    h_minfo          = pt_media_desc->h_media_info;
    pt_minfo_rec_buf = t_minfo_rec;

    i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    if (MM_HDLR_WITH_MINFO(pt_media_desc))
    {
        /****** Query all stream information than updata to scdb ***********/
        i4_ret = x_minfo_get_num_recs(h_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      &ui4_vid_rec_nums);
        while(i4_ret == MINFOR_INFO_NOT_READY)
        {
            x_thread_delay(100);
            i4_ret = x_minfo_get_num_recs(h_minfo,
                                          MINFO_INFO_TYPE_VIDEO_ATTR,
                                          &ui4_vid_rec_nums);
        }
        ui4_sz = sizeof(MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo,
                                  MINFO_INFO_TYPE_GENERIC,
                                  1,
                                  &t_minfo_gen_rec,
                                  (SIZE_T*)&ui4_sz);
        if (i4_ret != MINFOR_OK)
        {
            return  SVCTXR_NO_RECORD;
        }
#ifdef MW_MM_DUAL_VID_SUPPORT
        if ( t_minfo_gen_rec.u.t_general_info.e_scopic_type == MM_SCOPIC_DS &&
              t_minfo_gen_rec.u.t_general_info.b_stereo_scopic)
        {
             pt_media_desc->b_multi_track_3d = TRUE;
        }
#endif

#if 0
#ifdef ENABLE_DIVX_DRM
        /*check DRM support*/
        if (t_minfo_gen_rec.u.t_general_info.b_is_drm_exist)
        {
            if (!(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                  pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI))
                        {
                 mm_hdlr_invoke_client_svc_nfy_fct(
                           pt_svctx,
                           SVCTX_NTFY_CODE_MEDIA_DRM_NOT_SUPPORT,
                           ST_UNKNOWN
                           );
                return  SVCTXR_NO_RECORD;
            }
        }
#endif
#endif
        /*add for font info*/
        if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER &&
             pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            ui4_sz = sizeof(MINFO_INFO_T);
            x_minfo_get_info(h_minfo,MINFO_INFO_TYPE_FONT_INFO, 1,&t_minfo_font_rec,
                                      (SIZE_T*)&ui4_sz);

            pt_media_desc->pt_font_info = t_minfo_font_rec.u.t_font_info.pt_font_info;
        }

        ui2_video_track_cnt = (UINT16)0;
        for (ui2_i = 0; ui2_i < (UINT16)ui4_vid_rec_nums; ui2_i++)
        {
            if ((ui2_i >= MIDXBULD_ELMT_TBL_MAX)
                 || (ui2_video_track_cnt >= MIDXBULD_ELMT_TBL_MAX))
            {
                break;
            }
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      (UINT16)(ui2_i + 1),
                                      &t_minfo_rec[ui2_video_track_cnt++],
                                      &z_rec_length );
            if ( i4_ret != MINFOR_OK)
            {
                return  SVCTXR_NO_RECORD;
            }
            if (!(t_minfo_rec[ui2_video_track_cnt - 1].u.t_stm_attribute.b_is_codec_support))
            {
                /*check codec support flag to check
                if this stream should be insert to scdb*/
                /*remove this record*/
                if (ui2_video_track_cnt > 0)
                {
                    ui2_video_track_cnt--;
                }
            }
#ifdef DIVX_HT_2_2_CER
            /*HD Support checking*/
            if (t_minfo_gen_rec.u.t_general_info.b_hd_content && ui4_vid_rec_nums !=0)
            {
                mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT,
                    ST_VIDEO
                    );
                if (ui2_video_track_cnt > 0)
                {
                    ui2_video_track_cnt--;
                }
            }
#endif
        }

        x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_AUDIO_ATTR, &ui4_aud_rec_nums);
        ui2_audio_track_cnt = 0;
        for (ui2_i = 0; ui2_i < (UINT16)ui4_aud_rec_nums; ui2_i++)
        {

            if ((ui2_i + ui2_video_track_cnt >= MIDXBULD_ELMT_TBL_MAX)
                || (ui2_video_track_cnt + ui2_audio_track_cnt >= MIDXBULD_ELMT_TBL_MAX))
            {
                break;
            }
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(h_minfo,
                                      MINFO_INFO_TYPE_AUDIO_ATTR,
                                      (UINT16)(ui2_i + 1),
                                      &t_minfo_rec[ui2_video_track_cnt + ui2_audio_track_cnt++],
                                      &z_rec_length);
            if (i4_ret != MINFOR_OK)
            {
                return  SVCTXR_NO_RECORD;
            }
            if (!(t_minfo_rec[ui2_video_track_cnt + ui2_audio_track_cnt - 1].u.t_stm_attribute.b_is_codec_support))
            {
                /*remove this record*/
                ui2_audio_track_cnt--;
            }
        }
#ifdef SUPPORT_SET_SBTL_ENCODING
        if ( !pt_media_desc->b_is_ext_first )
        {
#endif
            x_minfo_get_num_recs(h_minfo,MINFO_INFO_TYPE_SP_ATTR, &ui4_sp_rec_nums);
            for (ui2_i = 0; ui2_i < ui4_sp_rec_nums; ui2_i++)
            {
                if (ui2_i + ui2_video_track_cnt + ui2_audio_track_cnt >= MIDXBULD_ELMT_TBL_MAX)
                {
                    break;
                }
                z_rec_length = sizeof(MINFO_INFO_T);
                i4_ret = x_minfo_get_info(h_minfo,
                                          MINFO_INFO_TYPE_SP_ATTR,(UINT16)(ui2_i+1),
                                          &t_minfo_rec[ui2_i + ui2_video_track_cnt + ui2_audio_track_cnt],
                                          &z_rec_length);
                if ( i4_ret != MINFOR_OK)
                {
                    return  SVCTXR_NO_RECORD;
                }
            }
#ifdef SUPPORT_SET_SBTL_ENCODING
        }
#endif
        if(pt_media_desc->b_is_lyric&&
            (ui2_video_track_cnt + ui2_audio_track_cnt < MIDXBULD_ELMT_TBL_MAX))
        {
            MINFO_INFO_T* pt_minfo_rec;

            ui4_sp_rec_nums = 1;

            pt_minfo_rec = &t_minfo_rec[ui2_video_track_cnt + ui2_audio_track_cnt];
            x_memcpy(pt_minfo_rec, &t_minfo_rec[0], sizeof(*pt_minfo_rec));
            pt_minfo_rec->e_info_type = MINFO_INFO_TYPE_SP_ATTR;
        }
        else if (pt_media_desc->b_is_ext_first &&
            (ui2_video_track_cnt + ui2_audio_track_cnt < MIDXBULD_ELMT_TBL_MAX))
        {
            MINFO_INFO_T* pt_minfo_rec;
            
            if (ui4_sp_rec_nums + pt_media_desc->ui2_total_ext_subtitle_num > 8)
            {
                pt_media_desc->ui2_total_ext_subtitle_num = 8 - ui4_sp_rec_nums;
            }
            for(ui2_i = 0; ui2_i < pt_media_desc->ui2_total_ext_subtitle_num; ui2_i ++)
            {
                if(ui2_video_track_cnt + ui2_audio_track_cnt + ui4_sp_rec_nums + ui2_i >= MIDXBULD_ELMT_TBL_MAX)
                {
                    break;
                }
                if(ui2_i >= EXT_SUBTITLE_MAX_NUM)
                {
                    break;
                }
                pt_minfo_rec = &t_minfo_rec[ui2_video_track_cnt + ui2_audio_track_cnt + ui4_sp_rec_nums + ui2_i];
                pt_minfo_rec->e_info_type = MINFO_INFO_TYPE_SP_ATTR;
                pt_minfo_rec->u.t_stm_attribute.u.t_ex_subtt_stm_attr.h_ext_subtitle = pt_media_desc->t_mm_svctx_sbtl_desc[ui2_i].h_ext_subtitle;
                x_memcpy(&pt_minfo_rec->u.t_stm_attribute.u.t_ex_subtt_stm_attr.c_lang_list,pt_media_desc->t_mm_svctx_sbtl_desc[ui2_i].c_lang_list,3);
                pt_minfo_rec->u.t_stm_attribute.u.t_ex_subtt_stm_attr.h_related_subtitle = pt_media_desc->t_mm_svctx_sbtl_desc[ui2_i].h_related_subtitle;
                pt_minfo_rec->u.t_stm_attribute.u.t_ex_subtt_stm_attr.ui2_idx = pt_media_desc->t_mm_svctx_sbtl_desc[ui2_i].ui2_idx;
                pt_minfo_rec->u.t_stm_attribute.u.t_ex_subtt_stm_attr.b_is_related = pt_media_desc->t_mm_svctx_sbtl_desc[ui2_i].b_is_related;
            }
        }

        if((ui4_vid_rec_nums != 0) && (ui2_video_track_cnt == 0))
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT,
                ST_VIDEO
                );
            strm_fmt_chker_set_format_status(
                     ((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->pt_strm_fmt_chker,
                     ST_VIDEO
                     );
        }

        if((ui4_aud_rec_nums != 0) && (ui2_audio_track_cnt == 0))
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
                ST_AUDIO
                );
            strm_fmt_chker_set_format_status(
                     ((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->pt_strm_fmt_chker,
                     ST_AUDIO
                     );
        }

        if(ui2_video_track_cnt + ui2_audio_track_cnt == 0)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC,
                ST_UNKNOWN);
            mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT,
                ST_UNKNOWN);
        }

        ui2_scdb_rec_nums = ui2_video_track_cnt + ui2_audio_track_cnt + ui4_sp_rec_nums + pt_media_desc->ui2_total_ext_subtitle_num;
        pt_scdb_rec_buf = x_mem_alloc(
                              sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)*ui2_scdb_rec_nums);

        if (pt_scdb_rec_buf == NULL)
        {
            return SVCTXR_NO_RESOURCE;
        }

        for (ui2_i = 0 ; ui2_i< ui2_scdb_rec_nums ; ui2_i++)
        {
            MINFO_INFO_T* pt_minfo_rec;
            STREAM_TYPE_T e_stream_type;
            
            if(ui2_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            pt_minfo_rec = &pt_minfo_rec_buf[ui2_i];
            /*assign stream id*/
            if (pt_minfo_rec_buf[ui2_i].e_info_type == MINFO_INFO_TYPE_VIDEO_ATTR)
            {
                e_stream_type = ST_VIDEO;
            }
            else if (pt_minfo_rec_buf[ui2_i].e_info_type == MINFO_INFO_TYPE_AUDIO_ATTR)
            {
                e_stream_type = ST_AUDIO;
            }
            else if (pt_minfo_rec_buf[ui2_i].e_info_type == MINFO_INFO_TYPE_SP_ATTR)
            {
                e_stream_type = ST_SUBTITLE;
            }
            else
            {
                e_stream_type = ST_UNKNOWN;
            }
            pt_scdb_rec_buf[ui2_i].t_stream_id.e_type = e_stream_type;
            /*
            pt_scdb_rec_buf[ui1_i].t_stream_id.pv_stream_tag = (VOID*)_mm_hdlr_gen_stream_tag();
            */
            ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(ui2_i);
            pt_scdb_rec_buf[ui2_i].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;

            /*assign scdb record type*/
            pt_scdb_rec_buf[ui2_i].t_scdb_rec.e_rec_type =
                _mm_hdlr_media_fmt_2_scdb_rec_type(pt_minfo_rec->t_format,
                                                   e_stream_type);
            /*assign private data*/
            pt_scdb_rec_buf[ui2_i].t_scdb_rec.ui4_private = 0;
            /*copy main data*/
            if(pt_minfo_rec->t_format.e_media_type == MEDIA_TYPE_AUDIO_ES &&
               pt_minfo_rec->t_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE &&
               pt_minfo_rec->u.t_stm_attribute.u.t_avi_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_DTS )
            {
                if(pt_minfo_rec->u.t_stm_attribute.u.t_avi_audio_stm_attr.i4_samples_per_sec == 0xFF)
                {                
                    pt_minfo_rec->u.t_stm_attribute.u.t_avi_audio_stm_attr.i4_samples_per_sec = 0;
                    e_dts_edn = BIG_EN;
                }
                else
                {
                    e_dts_edn = LITTLE_EN;
                }
#ifdef LINUX_TURNKEY_SOLUTION
                HANDLE_T h_scc_aud  = NULL_HANDLE;
                SM_COMMAND_T at_sm_cmds[3];

                /* create the scc handles */
                at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
                at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
                at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
                at_sm_cmds[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
                at_sm_cmds[2].e_code     = SM_CMD_CODE_END;
                at_sm_cmds[2].u.ui4_data = 0;
                i4_ret = x_scc_comp_open(at_sm_cmds, &h_scc_aud);
                if (i4_ret == SMR_OK)
                {
                    x_scc_aud_set_endian_mode(h_scc_aud, (SCC_AUD_ENDIAN_MODE_T)e_dts_edn);
                    x_scc_comp_close(h_scc_aud);
                }
/*
                i4_ret= d_custom_aud_set(
                                DRV_CUSTOM_AUD_TYPE_DTS_ENDIAN,
                                (VOID*)e_dts_edn,
                                 sizeof(e_dts_edn),
                                 TRUE
                              );
                if (i4_ret != SVCTXR_OK)
                {
                    DBG_INFO(("\n d_custom_aud_set failed, i4_ret = %d \n", i4_ret));
                }
*/
#else
                AUD_SetDTSInfo (e_dts_edn);
#endif
            }
            
            i4_ret = _minfo_rec_2_scdb_rec(pt_minfo_rec_buf[ui2_i],
                                           &pt_scdb_rec_buf[ui2_i].t_scdb_rec);
            if (i4_ret != SVCTXR_OK)
            {
                //do nothing
            }
        }
        /*store extra subtitle*/
        for (ui2_i = ui2_scdb_rec_nums - pt_media_desc->ui2_total_ext_subtitle_num; ui2_i < ui2_scdb_rec_nums; ui2_i ++)
        {
            if (ui2_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            i4_ret = _ext_subtitle_rec_2_scdb_rec(pt_minfo_rec_buf[ui2_i],
                                           &pt_scdb_rec_buf[ui2_i].t_scdb_rec);
            if (i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
        }
        pt_media_desc->ui8_media_sz = t_minfo_gen_rec.u.t_general_info.ui8_media_sz;
    }
    else /*if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)*/
    {
        HANDLE_T        h_file;

        if((pt_media_desc->pt_strm_info != NULL) &&
             (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS))
        {
            UINT8   ui1_idx;

            ui2_scdb_rec_nums = pt_media_desc->pt_strm_info->ui2_num_stm;
            pt_scdb_rec_buf = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)
                                           * ui2_scdb_rec_nums);
            if ( pt_scdb_rec_buf == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }

            x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);
            x_dbg_stmt("[MM_HDLR]:parsing the ts scdb_rec_nums=%d.\r\n",ui2_scdb_rec_nums);
            for (ui1_idx=0; (UINT16)ui1_idx<ui2_scdb_rec_nums; ui1_idx++)
            {
                ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(ui1_idx);
                pt_scdb_rec_buf[ui1_idx].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;
                pt_scdb_rec_buf[ui1_idx].t_scdb_rec.ui4_private = 0;

                if(ui1_idx >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type
                    = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].e_type;
                if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_VIDEO)
                {
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.ui2_pid =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.e_enc =
                        _minfo_video_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.e_enc);
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.e_vid_fmt =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.e_vid_fmt;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.b_default =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.b_default;
                    

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_MPEG;
                    x_dbg_stmt("[MM_HDLR]:{ST_VIDEO} pid=%d,fmt=%d,encode=%d.\r\n",pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.ui2_pid,
                                                                                   pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.e_vid_fmt,
                                                                                   pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_mpeg.e_enc);
                }
                else if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_AUDIO)
                {
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_MPEG;

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.ui2_pid
                        = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.e_enc =
                        _minfo_audio_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc);
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.e_layer = 
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.e_layer;

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.e_aud_fmt = AUD_FMT_UNKNOWN;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.ui1_index = 0;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.b_default = TRUE;
                    x_dbg_stmt("[MM_HDLR]:{ST_AUDIO} pid=%d,fmt=%d,encode=%d.\r\n",pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.ui2_pid,
                                                                                   pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.e_aud_fmt,
                                                                                   pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_mpeg.e_enc);
                    
                }
                else if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_SUBTITLE)
                {
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_SUBTITLE_DVB;

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_dvb_sbtl.ui2_pid
                        = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
                    x_memcpy(&(pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_dvb_sbtl), 
                             &(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_cmpb_subtl_stm_attr), 
                             sizeof(SCDB_SUBTITLE_DVB_T));
                }
            }
        }
        else if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_PS) && (pt_media_desc->pt_strm_info != NULL))
        {
            UINT8   ui1_idx;

            ui2_scdb_rec_nums = pt_media_desc->pt_strm_info->ui2_num_stm;
            pt_scdb_rec_buf = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)
                                           * ui2_scdb_rec_nums);
            if ( pt_scdb_rec_buf == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }

            x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);

            for (ui1_idx=0; (UINT16)ui1_idx<ui2_scdb_rec_nums; ui1_idx++)
            {
                ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(ui1_idx);
                pt_scdb_rec_buf[ui1_idx].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;
                pt_scdb_rec_buf[ui1_idx].t_scdb_rec.ui4_private = 0;

                if(ui1_idx >= SUPPORTED_STRM_NUM_MAX)
                {
                    break;
                }
                pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type
                    = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].e_type;
                if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_VIDEO)
                {
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.ui1_stream_id =
                        (UINT8)pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.e_enc =
                        _minfo_video_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.e_enc);
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.e_vid_fmt =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.e_vid_fmt;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.i4_width =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.i4_width;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.i4_height =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.i4_height;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_ps.b_default =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_video_stm_attr.b_default;
                    

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_PS;
                }
                else if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_AUDIO)
                {
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_PS;

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.ui1_stream_id
                        = (UINT8)(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id);
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.ui1_sub_stream_id
                        = (UINT8)(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_sub_stm_id);

                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.e_enc =
                        _minfo_audio_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc);
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.e_layer =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.e_layer;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.i2_channels =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.i2_channels;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.i4_samples_per_quant =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.i4_samples_per_quant;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.i4_sampling_frequency =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.i4_sampling_frequency;
                    
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_ps.b_default =
                        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_ps_audio_stm_attr.b_default;

                }
            }
        }
        else if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
            || (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            || ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
                && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                        == MEDIA_CONTNR_SUBTYPE_CMPB)))
        {
            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                e_format = DATA_FMT_MPEG_2;
            }
            t_file_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svl_id = pt_media_desc->ui2_svl_id;
            t_file_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svl_rec_id = pt_media_desc->ui2_svl_rec_id;
            t_file_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svc_id = pt_media_desc->ui2_svc_id;
            t_file_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svc_pid = pt_media_desc->ui2_svc_pid;
            t_file_item.t_extra.t_mpeg2_info.t_svc_info.ui2_index = pt_media_desc->ui2_pmt_idx;
            t_file_item.t_extra.t_mpeg2_info.ui4_flags = 0;
            if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                /*t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_DUAL_MONO_FAKE_STEREO; NO USE NOW*/

                /* Just for the case that the differece PMT have same pid and differ section length or differ version */
                if (pt_media_desc->e_fill_scdb_mode == MM_SVC_FILL_SCDB_APPEND)
                {
                    t_file_item.t_extra.t_mpeg2_info.ui4_flags |=  CM_FLAG_APPEND_SCDB_SUPPORT;
                }

#ifdef TIME_SHIFT_SUPPORT
                MM_SVC_REQ_T*   pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

                if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
                {
                    if ((pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_DVB) ||
                        (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ISDB) ||
                        (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_DTMB))
                    {
                        t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_DVB;
                    }
                    else if(pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ATSC)
                    {
                        t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_ATSC;
                    }
                    else
                    {
#ifdef MM_TS_DVBT_SUPPORT
                        t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_DVB;
#else
                        t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_ATSC;
#endif
                    }

                }
                else
#endif /* TIME_SHIFT_SUPPORT */
                {
#ifdef MM_TS_DVBT_SUPPORT
                    t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_DVB;
#else
                    t_file_item.t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_ATSC;
#endif
                }
            }

            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
            {
                ui2_scdb_rec_nums = 1;
                pt_scdb_rec_buf = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)
                                               * ui2_scdb_rec_nums);
                if ( pt_scdb_rec_buf == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                
                x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);

                ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(0);
                pt_scdb_rec_buf[0].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;
                pt_scdb_rec_buf[0].t_scdb_rec.ui4_private = 0;

                pt_scdb_rec_buf[0].t_stream_id.e_type = ST_VIDEO;
                pt_scdb_rec_buf[0].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_ES;
                switch(pt_media_desc->t_media_fmt.t_media_subtype.u.e_vid_subtype)
                {
                    case MEDIA_VID_SUBTYPE_M1V:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_MPEG_1;
                        break;
                    case MEDIA_VID_SUBTYPE_M2V:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_MPEG_2;
                        break;
                    case MEDIA_VID_SUBTYPE_M4V:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_MPEG_4;
                        break;
                    case MEDIA_VID_SUBTYPE_H264:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_H264;
                        break;
                    case MEDIA_VID_SUBTYPE_VC1:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_WVC1;
                        break;
                    case MEDIA_VID_SUBTYPE_AVS:
                        pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.e_enc = VID_ENC_AVS;
                        break;
                    default:
                        break;
                }
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_video_es.ui4_stream_id = 1;
            }
            else if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
                && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                        == MEDIA_CONTNR_SUBTYPE_CMPB)
                && (pt_media_desc->pt_strm_info != NULL))
            {
                UINT8   ui1_idx;
                
                ui2_scdb_rec_nums = pt_media_desc->pt_strm_info->ui2_num_stm;
                pt_scdb_rec_buf = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)
                                               * ui2_scdb_rec_nums);
                if ( pt_scdb_rec_buf == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }

                x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);

                for (ui1_idx=0; (UINT16)ui1_idx<ui2_scdb_rec_nums; ui1_idx++)
                {
                    ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(ui1_idx);
                    pt_scdb_rec_buf[ui1_idx].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;
                    pt_scdb_rec_buf[ui1_idx].t_scdb_rec.ui4_private = 0;

                    if(ui1_idx >= SUPPORTED_STRM_NUM_MAX)
                    {
                        break;
                    }
                    pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type
                        = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].e_type;
                    if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_VIDEO)
                    {
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_ES;
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_es.e_enc =
                         _minfo_video_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_cmpb_video_stm_attr.e_enc);

                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_video_es.ui4_stream_id = 1;
                    }
                    else if(pt_scdb_rec_buf[ui1_idx].t_stream_id.e_type == ST_AUDIO)
                    {
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_ES;
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_es.e_enc =
                            _minfo_audio_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_cmpb_audio_stm_attr.e_enc);
                        
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_es.e_layer
                            = (AUD_LAYER_T)(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_cmpb_audio_stm_attr.ui1_mpg_layer);
                        pt_scdb_rec_buf[ui1_idx].t_scdb_rec.u.t_audio_es.ui4_stream_id = 
                            pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_idx].t_stm_atrbt.u.t_cmpb_audio_stm_attr.ui4_stream_id;
                    }
                }
            }
        }
        else if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES) && 
                 (pt_media_desc->pt_meta_info == NULL)/*it's not set mode*/)
        {
            ui2_scdb_rec_nums = 1;
            pt_scdb_rec_buf = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)
                                          * ui2_scdb_rec_nums);
            if ( pt_scdb_rec_buf == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }

            x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);

            ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(0);
            pt_scdb_rec_buf[0].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;
            pt_scdb_rec_buf[0].t_scdb_rec.ui4_private    = 0;

            pt_scdb_rec_buf[0].t_stream_id.e_type        = ST_AUDIO;
            pt_scdb_rec_buf[0].t_scdb_rec.e_rec_type     =
                _mm_hdlr_media_fmt_2_scdb_rec_type(pt_media_desc->t_media_fmt, ST_AUDIO);

            if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_LPCM;
            }
            else if(pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_AAC;
            }
            else if(pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AMR)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_AMR;
            }
            else if(pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AWB)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_AWB;
            }
            else if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_LPCM;
            }
            else if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.e_enc = AUD_ENC_AC3;
            }
            else if (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
            {
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_mp3.e_enc = AUD_ENC_MPEG_1;
                pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_mp3.e_layer = AUD_LAYER_3;
            }
            else
            {
            }

            pt_scdb_rec_buf[0].t_scdb_rec.u.t_audio_es.ui4_stream_id = 0;
        }
        else
        {
            if (pt_media_desc->pt_strm_info != NULL)
            {
                STREAM_TYPE_T    e_stream_type;

                /* allocate scdb buffer */
                ui2_scdb_rec_nums = pt_media_desc->pt_strm_info->ui2_num_stm;

                if (ui2_scdb_rec_nums > MIDXBULD_ELMT_TBL_MAX)
                {
                    return SVCTXR_FAIL;
                }

                pt_scdb_rec_buf = x_mem_alloc(
                                      sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T) * ui2_scdb_rec_nums);

                if (pt_scdb_rec_buf == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }

                x_memset (pt_scdb_rec_buf, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T)* ui2_scdb_rec_nums);

                /* initialize first */
                x_memset((VOID*)&t_minfo_rec, 0, sizeof(MINFO_INFO_T) * MIDXBULD_ELMT_TBL_MAX);

                for (ui2_i = 0 ; ui2_i < ui2_scdb_rec_nums ; ui2_i++)
                {
                    
                    if(ui2_i >= SUPPORTED_STRM_NUM_MAX)
                    {
                        break;
                    }
                    e_stream_type = pt_media_desc->pt_strm_info->at_stm_atrbt[ui2_i].e_type;

                    pt_scdb_rec_buf[ui2_i].t_stream_id.e_type = e_stream_type;
                    ui4_scdb_tag = (UINT32)0x76543200 | (UINT32)(ui2_i);
                    pt_scdb_rec_buf[ui2_i].t_stream_id.pv_stream_tag = (VOID*)ui4_scdb_tag;

                    /* assign scdb record type */
                    pt_scdb_rec_buf[ui2_i].t_scdb_rec.e_rec_type =
                        _mm_hdlr_media_fmt_2_scdb_rec_type(pt_media_desc->t_media_fmt,
                                                           e_stream_type);
                    /* assign private data */
                    pt_scdb_rec_buf[ui2_i].t_scdb_rec.ui4_private = 0;

                    switch(e_stream_type)
                    {
                        case ST_VIDEO:
                            t_minfo_rec[ui2_i].e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
                            break;
                        case ST_AUDIO:
                            t_minfo_rec[ui2_i].e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
                            break;
                        case ST_SUBTITLE:
                            t_minfo_rec[ui2_i].e_info_type = MINFO_INFO_TYPE_SP_ATTR;
                            break;
                        default:
                            t_minfo_rec[ui2_i].e_info_type = MINFO_INFO_TYPE_UNKNOWN;
                    }

                    t_minfo_rec[ui2_i].t_format = pt_media_desc->t_media_fmt;
                    t_minfo_rec[ui2_i].u.t_stm_attribute = pt_media_desc->pt_strm_info->at_stm_atrbt[ui2_i].t_stm_atrbt;

                    /* copy main data */
                    i4_ret = _minfo_rec_2_scdb_rec(t_minfo_rec[ui2_i],
                                                   &pt_scdb_rec_buf[ui2_i].t_scdb_rec);
                    if (i4_ret != SVCTXR_OK)
                    {
                        return i4_ret;
                    }
                }
            }
            else
            {
                return SVCTXR_FAIL;
            }
        }
        if((pt_media_desc->ps_media_path != NULL) && 
           (pt_media_desc->e_svc_protocol != PROTOCL_MODE_PULL))
        {
            if((x_strncasecmp(pt_media_desc->ps_media_path, "http:", 5) == 0) ||
               (x_strncasecmp(pt_media_desc->ps_media_path, "https:", 6) == 0))
            {
                x_dbg_stmt(_INFO_HEADER"svctx media size = %d\n", (UINT32)pt_media_desc->ui8_media_sz);
            }
            else
            {
                i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE,
                                       pt_media_desc->ps_media_path,
                                       FM_READ_ONLY,
                                       FM_MODE_USR_READ,
                                       TRUE, /* no cache */
                                       &h_file);
                if (i4_ret == FMR_OK)
                {
                    UINT64        ui8_current_pos;
    
                    i4_ret = x_fm_mfw_lseek(h_file,
                                           (INT64) 0,
                                            FM_SEEK_END,
                                            &ui8_current_pos);
                    if (i4_ret != FMR_OK && i4_ret != FMR_BUSY)
                    {
                        i4_ret = x_fm_mfw_close(h_file);
                        if (i4_ret != FMR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"Could not close file handle %d line%d\n", h_file, __LINE__);
                        }
    
                        return SVCTXR_FAIL;
                    }
                    else
                    {
                        pt_media_desc->ui8_media_sz = ui8_current_pos;
                    }
                }
                else
                {
                    return SVCTXR_FAIL;
                }
    
                i4_ret = x_fm_mfw_close(h_file);
                if (i4_ret != FMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"Could not close file handle %d line%d\n", h_file, __LINE__);
                }
            }
        }

    }


    if(((ui2_audio_track_cnt != 0) && (ui2_video_track_cnt == 0))
        || (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES))
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
            pt_svctx,
            SVCTX_NTFY_CODE_AUDIO_ONLY_STRM,
            ST_UNKNOWN
            );            
    }

    if(((ui2_audio_track_cnt == 0) && (ui2_video_track_cnt != 0))
        || (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES))
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
            pt_svctx,
            SVCTX_NTFY_CODE_VIDEO_ONLY_STRM,
            ST_UNKNOWN
            );           
    }
        
    if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI)
    {
        PLAYBACK_HANDLER_QUEUE_URI_T    t_uri_item;

        x_memset(&t_uri_item, 0, sizeof(PLAYBACK_HANDLER_QUEUE_URI_T));
        
        /*pt_scdb_rec_buf would be free in close media*/
        pt_media_desc->pv_scdb_recs  = pt_scdb_rec_buf;
        t_uri_item.e_format         = e_format;
        t_uri_item.ui4_flags        = CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED;
        t_uri_item.ps_path          = (const CHAR*)pt_media_desc->ps_media_path;
        t_uri_item.h_uri            = pt_media_desc->h_uri_cm;
        /*move start and end*/
#ifdef ENABLE_MMS_SUPPORT
        if(pt_media_desc->pc_mms_header_buf)
        {
            t_uri_item.ui8_end_offset   = (UINT64)(-1);
        }
        else
#endif
        {            
            t_uri_item.ui8_end_offset   = pt_media_desc->ui8_media_sz;
        }
        t_uri_item.ui8_start_offset = 0;
        t_uri_item.pt_scdb_info     = pt_scdb_rec_buf;
        t_uri_item.ui4_nb_records   = ui2_scdb_rec_nums;  
        t_uri_item.ps_proxy_url     = (const CHAR*)pt_media_desc->ps_proxy_url;      
        t_uri_item.ps_agent_url     = (const CHAR*)pt_media_desc->ps_agent_url;  
        t_uri_item.ps_cookie_url    = (const CHAR*)pt_media_desc->ps_cookie_url;  
        t_uri_item.t_extra.t_mpeg2_info.ui4_flags = t_file_item.t_extra.t_mpeg2_info.ui4_flags;
        i4_ret = x_cm_set(h_conn,
                          PLAYBACK_CTRL_SET_QUEUE_URI,
                          (VOID*) & t_uri_item);
    }
    else if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
        && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
    {
        PLAYBACK_HANDLER_QUEUE_PULL_T   t_pull_item;
        
        x_memset(&t_pull_item, 0, sizeof(PLAYBACK_HANDLER_QUEUE_PULL_T));

        pt_media_desc->pv_scdb_recs  = pt_scdb_rec_buf;
        t_pull_item.e_format         = e_format;
        t_pull_item.ui4_flags        = CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED;
        
        if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
        {
            t_pull_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svl_id = pt_media_desc->ui2_svl_id;
            t_pull_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svl_rec_id = pt_media_desc->ui2_svl_rec_id;
            t_pull_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svc_id = pt_media_desc->ui2_svc_id;
            t_pull_item.t_extra.t_mpeg2_info.t_svc_info.ui2_svc_pid = pt_media_desc->ui2_svc_pid;
            t_pull_item.t_extra.t_mpeg2_info.t_svc_info.ui2_index = pt_media_desc->ui2_pmt_idx;
            t_pull_item.t_extra.t_mpeg2_info.ui4_flags = 0;
        }

        t_pull_item.t_pull_info.h_pb
                                = pt_media_desc->t_playback_ctrl_pull_info.h_pb;
        t_pull_item.t_pull_info.pv_app_tag
                                = pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag;
        t_pull_item.t_pull_info.pf_open
                                = (x_playback_pull_open_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_open;
        t_pull_item.t_pull_info.pf_associate
                                = (x_playback_pull_associate_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_associate;
        t_pull_item.t_pull_info.pf_dissociate
                                = (x_playback_pull_dissociate_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_dissociate;
        t_pull_item.t_pull_info.pf_close
                                = (x_playback_pull_close_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_close;
        t_pull_item.t_pull_info.pf_read
                                = (x_playback_pull_read_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_read;
        t_pull_item.t_pull_info.pf_read_async
                                = (x_playback_pull_read_async_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_read_async;
        t_pull_item.t_pull_info.pf_byteseek
                                = (x_playback_pull_byteseek_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_byteseek;
        t_pull_item.t_pull_info.pf_abort_read_async
                                = (x_playback_pull_abort_read_async_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_abort_read_async;
        t_pull_item.t_pull_info.pf_get_input_len
                                = (x_playback_pull_get_input_length_fct)pt_media_desc->t_playback_ctrl_pull_info.pf_get_input_len;

        
        /*move start and end*/
        t_pull_item.ui8_end_offset   = pt_media_desc->ui8_media_sz;
        t_pull_item.ui8_start_offset = 0;
        t_pull_item.pt_scdb_info     = pt_scdb_rec_buf;
        t_pull_item.ui4_nb_records   = ui2_scdb_rec_nums;
        t_pull_item.t_extra.t_mpeg2_info.ui4_flags = t_file_item.t_extra.t_mpeg2_info.ui4_flags;

        i4_ret = x_cm_set(h_conn,
                          PLAYBACK_CTRL_SET_QUEUE_PULL,
                          (VOID*) & t_pull_item);
    }
    else if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH)
        && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
    {
        PLAYBACK_HANDLER_QUEUE_BUFFER_T   t_queue_buf_item;
        
        x_memset(&t_queue_buf_item, 0, sizeof(PLAYBACK_HANDLER_QUEUE_BUFFER_T));

        pt_media_desc->pv_scdb_recs  = pt_scdb_rec_buf;
        t_queue_buf_item.ui4_id           = 0;
        t_queue_buf_item.ui4_flags        = CM_PLAYBACK_FLAG_STICKY_SCDB | CM_PLAYBACK_FLAG_CANNOT_SET_OFFSET;   
        t_queue_buf_item.e_format         = DATA_FMT_UNKNOWN; 
        t_queue_buf_item.ui8_start_offset = 0;
        t_queue_buf_item.ui8_end_offset   = 0;
        t_queue_buf_item.ui8_size         = 0;
        t_queue_buf_item.pv_data          = (VOID*)((UINT32)-1);
        t_queue_buf_item.pt_scdb_info     = pt_scdb_rec_buf;
        t_queue_buf_item.ui4_nb_records   = ui2_scdb_rec_nums;
        t_queue_buf_item.ui4_buf_agt_index= 0;

        i4_ret = x_cm_set(h_conn,
                          PLAYBACK_CTRL_SET_QUEUE_BUFFER,
                          (VOID*) & t_queue_buf_item);
    }
    else
    {
        /*pt_scdb_rec_buf would be free in close media*/
        pt_media_desc->pv_scdb_recs  = pt_scdb_rec_buf;
        t_file_item.e_format         = e_format;
        t_file_item.ui4_flags        = CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED;
        t_file_item.ps_path          = (const CHAR*)pt_media_desc->ps_media_path;
        /*move start and end*/
        t_file_item.ui8_end_offset   = pt_media_desc->ui8_media_sz;
        t_file_item.ui8_start_offset = 0;
        t_file_item.pt_scdb_info     = pt_scdb_rec_buf;
        t_file_item.ui4_nb_records   = ui2_scdb_rec_nums;
        
        i4_ret = x_cm_set(h_conn,
                          PLAYBACK_CTRL_SET_QUEUE_FILE,
                          (VOID*) & t_file_item);
    }
    if ((i4_ret != CMR_OK) && (i4_ret != CMR_ASYNC_NFY))
    {
         return SVCTXR_FAIL;
    }
    else
    {
         return SVCTXR_OK;
    }
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_close_text_strms
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_close_text_strms(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    UINT32                      ui4_text_strm_maks
                    )
{

    INT32 i4_ret;

    if (ui4_text_strm_maks & ST_MASK_CLOSED_CAPTION) {
        STRM_REQ_T* pt_cc_strm_req  = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
        /* this video stream has related closed cpation in using*/
        if (x_handle_valid(pt_cc_strm_req->h_stream) == TRUE) {
            pt_cc_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_cc_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_cc_strm_req->e_strm_cond = SM_COND_CLOSING;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }

    if (ui4_text_strm_maks & ST_MASK_SUBTITLE) {
        STRM_REQ_T* pt_sbtl_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE]);
        /* this video stream has related subtitle in using*/
        if (x_handle_valid(pt_sbtl_strm_req->h_stream) == TRUE) {
            pt_sbtl_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_sbtl_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_sbtl_strm_req->e_strm_cond = SM_COND_CLOSING;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }

    if (ui4_text_strm_maks & ST_MASK_TELETEXT) {
        STRM_REQ_T* pt_tt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_TELETEXT]);
        /* this video stream has related teletext in using*/
        if (x_handle_valid(pt_tt_strm_req->h_stream) == TRUE) {
            pt_tt_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_tt_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_tt_strm_req->e_strm_cond = SM_COND_CLOSING;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_check_strm_scdb_record
 * Description
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_check_strm_scdb_record(
                    HANDLE_T                     h_scdb,
                    STREAM_TYPE_T                e_strm_type,
                    STRM_REQ_T*                  pt_strm_req
                    )
{
    SCDB_REC_T t_scdb_rec;
    INT32      i4_ret = SCDBR_REC_NOT_FOUND;

    if (x_handle_valid(pt_strm_req->h_stream) == FALSE
            || pt_strm_req->e_req_type != STRM_REQ_SCDB
            || pt_strm_req->u.t_scdb.t_comp_id.e_type != e_strm_type) {
        return SVCTXR_NO_RECORD;
    }

    i4_ret = x_scdb_get_rec (
                    h_scdb,
                    &pt_strm_req->u.t_scdb.t_comp_id,
                    &t_scdb_rec
                    );
    if (i4_ret == SCDBR_REC_NOT_FOUND) 
    {
        return SVCTXR_NO_RECORD;
    }

    return (i4_ret == SCDBR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_reload_video_stream_as_blocked
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_reload_video_stream_as_blocked (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
#if 1
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
        svctx_ctrl_block(pt_svctx, FALSE);
        mm_hdlr_reload_stream(pt_svctx, pt_running_svc_req, ST_VIDEO);
        svctx_ctrl_block(pt_svctx, TRUE);
    }
#endif
    return SVCTXR_OK;
}

#if 1/*porting from ch_hdlr*/
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_open_cc_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_open_cc_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    INT32            i4_ret      = SVCTXR_OK;
    STREAM_COMP_ID_T t_comp_id;

    /* if there's a closed stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || pt_strm_req->e_req_type != STRM_REQ_SCDB
            || pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_UNKNOWN
            || x_handle_valid(pt_strm_req->h_stream) == TRUE
            || pt_running_svc_req->t_cc_filter.e_cc_type == SM_CC_TYPE_IGNORE)
    {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) > 0)
    {
        if (!SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter))
        {
            return SVCTXR_OK;
        }
    }

    i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    svctx_strm_comp_cc_filter,
                    (VOID*)(&pt_running_svc_req->t_cc_filter),
                    &t_comp_id,
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    if (i4_ret == SVCTXR_OK)
    {
        pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
    }
    else
    {
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
    }

    i4_ret = _mm_hdlr_handle_strm_select_req (
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req,
                    FALSE
                    );
    if (i4_ret == SVCTXR_OK)
    {
        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
    }
    else
    {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_open_sbtl_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_open_sbtl_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T*                  pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE]);
    INT32                        i4_ret      = SVCTXR_OK;
    STREAM_TYPE_T                e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);
    STREAM_COMP_ID_T             t_comp_id;
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct;
    VOID*                        pv_strm_fltr_tag;

    /* if there's a subtitle stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || e_strm_type == ST_UNKNOWN
            || x_handle_valid(pt_strm_req->h_stream) == TRUE)
    {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) > 0)
    {
        return SVCTXR_OK;
    }

    if (pt_strm_req->e_req_type == STRM_REQ_SCDB)
    {
        SCDB_REC_T t_scdb_rec;

        i4_ret = x_scdb_get_rec (
                        player_get_scdb_handle(pt_running_svc_req->pt_player),
                        &pt_strm_req->u.t_scdb.t_comp_id,
                        &t_scdb_rec
                        );
        if (i4_ret != SCDBR_OK)
        {
            mm_hdlr_choose_strm_filter(
                        pt_svctx,
                        ST_SUBTITLE,
                        &pt_strm_fltr_fct,
                        &pv_strm_fltr_tag
                        );
            i4_ret = svctx_get_strm_comp_id(
                        pt_svctx,
                        ST_SUBTITLE,
                        TRUE,
                        pt_strm_fltr_fct,
                        pv_strm_fltr_tag,
                        &t_comp_id,
                        &(pt_strm_req->u.t_scdb.t_rec)
                        );
            if (i4_ret == SVCTXR_OK)
            {
                pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
            }
            else
            {
                return SVCTXR_OK;
            }
        }

        i4_ret = mm_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        TRUE
                        );
        if (i4_ret == SVCTXR_OK)
        {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        }
        else
        {
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        }
    }
    else if (pt_strm_req->e_req_type == STRM_REQ_PID)
    {
        i4_ret = mm_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        TRUE
                        );
        if (i4_ret == SVCTXR_OK)
        {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        }
        else
        {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        }
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_video_strm_post_start_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_video_strm_post_start_handler(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req  = NULL;
    INT32       i4_ret             = SVCTXR_OK;

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    do {
        if (pt_video_strm_req == NULL)
        {
            break;
        }

        if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE)
        {
            break;
        }

        i4_ret = svctx_set_video_stream_attrs(pt_svctx, pt_video_strm_req->h_stream);
        if (i4_ret != SVCTXR_OK)
        {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE)
        {
            break;
        }

        if (pt_video_strm_req->t_ca.e_event == SM_EVN_CA_COND_NOT_AUTHORIZED)
        {
            break;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE)
        {
            break;
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_CLOSED_CAPTION)) > 0)
        {
            _mm_hdlr_open_cc_stream_after_video_started(pt_svctx, pt_running_svc_req);
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_SUBTITLE)) > 0)
        {
            _mm_hdlr_open_sbtl_stream_after_video_started(pt_svctx, pt_running_svc_req);
        }
    } while (0);

    return SVCTXR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_strm_started_nty_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_started_nty_as_presenting(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    INT32         i4_ret               = SVCTXR_OK;
    MM_SVC_REQ_T* pt_running_svc_req   = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STREAM_TYPE_T e_strm_type          = svctx_get_strm_req_strm_type(pt_strm_req);
    STRM_REQ_T**  ppt_pending_strm_req = NULL;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
    {
        ASSERT(0, ("%s(), over the array bound\r\n", __func__));
    }
    ppt_pending_strm_req = &((pt_running_svc_req)->pt_pending_strm_reqs[e_strm_type]);

    switch(pt_strm_req->e_penging_status) 
    {
    case STRM_PENDING_COND_SELECT_STRM:
        if (e_strm_type != ST_UNKNOWN && (*ppt_pending_strm_req) != NULL) 
        {
            i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        } 
        else 
        {
            mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            if (e_strm_type == ST_VIDEO) 
            {
                _mm_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
            }
        }
        break;
    case STRM_PENDING_COND_STOP_STRM:
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        break;
    case STRM_PENDING_COND_NONE:
        mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
        if (e_strm_type == ST_VIDEO) 
        {
            i4_ret = _mm_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
        }
        break;
    default:
        break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_strm_closed_nty_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_closed_nty_as_presenting(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req   = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STREAM_TYPE_T e_strm_type          = svctx_get_strm_req_strm_type(pt_strm_req);
    MEDIA_DESCRIBT_T* pt_media_desc    = NULL;
    STRM_REQ_T**  ppt_pending_strm_req = NULL;
    INT32         i4_ret               = SVCTXR_OK;
    BOOL          b_auto_play = TRUE;
    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,
                                   &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    switch(pt_strm_req->e_penging_status) {
    case STRM_PENDING_COND_SELECT_STRM:
           if (e_strm_type == ST_UNKNOWN) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            break;
        }
        if ((pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X) &&
            ((e_strm_type == ST_AUDIO) || (e_strm_type == ST_SUBTITLE)))
        {
            b_auto_play = FALSE;
        }
        if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            ASSERT(0, ("%s(), over the array bound\r\n", __func__));
        }
        ppt_pending_strm_req =
        &((pt_running_svc_req)->pt_pending_strm_reqs[e_strm_type]);
        if (*ppt_pending_strm_req != NULL) 
        {
            *pt_strm_req = *(*ppt_pending_strm_req);
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
            i4_ret = mm_hdlr_open_strm(pt_svctx, pt_strm_req, b_auto_play);
            if (i4_ret == SVCTXR_OK) 
            {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
                if(e_strm_type == ST_AUDIO && pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X)
                {
                    pt_media_desc->b_set_audio_stop = TRUE;
                }
            } 
            else 
            {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            }
        } 
        else 
        {
            mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
            pt_strm_req->h_stream         = NULL_HANDLE;
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        }
        break;
    case STRM_PENDING_COND_STOP_STRM:
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        pt_strm_req->h_stream         = NULL_HANDLE;
        break;
    default:
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        pt_strm_req->h_stream         = NULL_HANDLE;
        break;
    }

    if (pt_strm_req->e_penging_status == STRM_PENDING_COND_NONE) {
        SRC_OPER_TYPE_T e_src_oper_type;
        SNK_OPER_TYPE_T e_snk_oper_type;

        e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);
        if (e_strm_type == ST_UNKNOWN) {
            return i4_ret;
        }
        if (pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(e_strm_type)) {
            return i4_ret;
        }

        if (player_get_oper_types(
                        pt_running_svc_req->pt_player,
                        &e_src_oper_type,
                        &e_snk_oper_type
                        ) == SVCTXR_OK
                && e_src_oper_type == SRC_OPER_TYPE_CONNECT
                && e_snk_oper_type == SNK_OPER_TYPE_PLAY) {
            /* It is not trigger by client of svctx, so retain the original record
             * to reselect the same stream as far as it can
             */
            return i4_ret;
        }

        pt_strm_req->e_req_type                = STRM_REQ_SCDB;
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_strm_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*         pt_strm_req        = NULL;
    HANDLE_T            h_media ;
    MEDIA_DESCRIBT_T*   pt_media_desc;
    STREAM_TYPE_T       e_strm_type;
    UINT8               ui1_idx;
    INT32               i4_ret = SVCTXR_OK;
    HANDLE_T            h_stream;
    /*SVCTX_MSG_T         t_msg;*/
    MM_SVC_TRICK_INFO_T t_trick_info;

    h_stream = pt_strm_msg->h_obj;
    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++)
    {
        e_strm_type = e_g_mm_strm_types[ui1_idx];
        if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            break;
        }
        pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        if (pt_strm_req == NULL || pt_strm_req->h_stream != h_stream) {
            continue;
        }
        else
        {
          break;
        }
    }
    if((ui1_idx == (UINT8)STRM_TYPE_COUNT) || (pt_strm_req == NULL))
    {
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID)))  
      /*check 3D additional track */
      if(pt_running_svc_req->t_strm_reqs_3d_vid.h_stream == h_stream)
      {
          pt_strm_req = &(pt_running_svc_req->t_strm_reqs_3d_vid);
          e_strm_type = ST_VIDEO;
      }
      else
#endif
      {
           return i4_ret;
      }
    }

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    switch(pt_strm_msg->e_event) {
    case SM_EVN_AS_REQ_RESTART:
        /*this modify is to handle  the scdb update case ,the solution now is temprory ,if we have bypass,
        we should modify the code like ch_hdlr handle_strm_msg*/
        i4_ret = x_sm_stop_stream(pt_strm_req->h_stream);
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"Stop audio failed ret(%d) line(%d)\n",i4_ret,__LINE__);
            break;
        }
        i4_ret = x_sm_play_stream(pt_strm_req->h_stream);
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"Play audio failed ret(%d) line(%d)\n",i4_ret,__LINE__);
            break;
        }
        
        break;
    case SM_EVN_VS_VIDEO_PLAY_DONE:
        x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_VIDEO_PLAY_DONE\n");
        mm_hdlr_invoke_client_strm_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_VIDEO_PLAY_DONE,
                pt_strm_req
                );       
        break;
    case SM_EVN_CCS_EXISTED_CC_IND_CHG:
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG,
                    ST_CLOSED_CAPTION
                    );
        break;
    case SM_EVN_VS_VIDEO_RESOLUTION_CHG:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE\n");
            TMS_END("SELECT_SVC_2_VIDEO_FMT_NTY");
            svctx_set_video_info(pt_svctx, &pt_strm_msg->uinfo.t_video);
            mm_hdlr_invoke_client_strm_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                pt_strm_req
                );

#ifdef TIME_SHIFT_SUPPORT
            if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
            {
                if (pt_media_desc->b_trick_mode_revive)
                {
                    pt_media_desc->b_trick_mode_revive = FALSE;
                    if (pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE)
                    {
                        t_trick_info.e_speed_type = pt_media_desc->e_speed;
                        i4_ret = mm_hdlr_set(pt_svctx,
                                             SVCTX_MM_SET_TYPE_TRICK,
                                             (VOID*)&t_trick_info,
                                             sizeof(MM_SVC_TRICK_INFO_T));
                        if (i4_ret!= SVCTXR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK_PAUSE fail!\n");
                        }
                    }
                }
                mm_hdlr_get_current_event(pt_svctx);
            }
#endif /* TIME_SHIFT_SUPPORT */
            break;
    case SM_EVN_AS_AUD_INFO_CHG:
            DBG_INFO((_INFO_HEADER" DRIVER SEND SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE\n"));
            svctx_set_audio_info(pt_svctx, &pt_strm_msg->uinfo.t_audio);
            mm_hdlr_invoke_client_strm_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                pt_strm_req
                );
#ifdef TIME_SHIFT_SUPPORT
                if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
                {
                    mm_hdlr_get_current_event(pt_svctx);
                }
#endif /* TIME_SHIFT_SUPPORT */

            break;
    case SM_EVN_RANGE_AB_DONE:
        {
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_RANGE_AB_DONE\n");
            if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE &&
               !pt_media_desc->b_is_seeking)
            {
                MM_SVC_TRICK_INFO_T             t_trick_info;
                if(pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X &&
                   pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE)
                {
                    /*pause at first to avoid play to begin or end before seek ready*/
                    t_trick_info.e_speed_type = MM_SPEED_TYPE_PAUSE;
                    i4_ret = mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                    if (i4_ret != SMR_OK)
                    {
                        x_dbg_stmt(_ERROR_HEADER"SM_EVN_RANGE_AB_DONE:trick MM_SPEED_TYPE_PAUSE fail! line(%d)\n",__LINE__);
                        i4_ret =  SVCTXR_FAIL;
                    }
                }
                /* seek to A position */
                pt_media_desc->t_repeat_ab_info.t_repeat_info_a.t_pos_type = MM_SVC_POS_TYPE_RANGE_AB_START;
                i4_ret = mm_hdlr_set(pt_svctx,
                                  SVCTX_MM_SET_TYPE_SEEK,
                                  (VOID*)&pt_media_desc->t_repeat_ab_info.t_repeat_info_a,
                                  sizeof(MM_SVC_POS_INFO_T));

                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"SM_EVN_RANGE_AB_DONE: seek MM_SVC_POS_TYPE_RANGE_AB_START fail! line(%d)\n",__LINE__);
                    i4_ret =  SVCTXR_FAIL;
                }
                /*after seek ready will trick to 1x*/
                pt_media_desc->b_is_seeking = TRUE;
                pt_media_desc->t_repeat_ab_info.b_seeking = TRUE;
            }
        }
        break;
#ifdef DIVX_PLUS_CER 
    case SM_EVN_VS_CHAP_DONE:/*wqyin*/            
        x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_CHAP_DONE\n");            
        if(!pt_media_desc->b_is_seeking)            
        {                
            MINFO_CHAP_INFO_T t_chap_info;                
            MINFO_CHAP_INFO_T t_chap_info_next;                
            i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info);                
            if(i4_ret == SVCTXR_OK)                
            {                    
                if (pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE)/*it's done of FF*/                    
                {                        
                    /*set next chapter*/                        
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap(pt_media_desc, TRUE, NULL);                    
                }                    
                else                    
                {                        
                    /*set prev chapter*/                        
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap(pt_media_desc, FALSE, NULL);                    
                }                    
                if(i4_ret != SVCTXR_OK)                    
                {                       
                    /*no prev/next chapter then stop */                        
                    if (!pt_svctx->b_in_stopping)                        
                    {                            
                        mm_hdlr_invoke_client_strm_nfy_fct(                                 
                            pt_svctx,                                 
                            SVCTX_NTFY_CODE_MEDIA_EOS,                                 
                            pt_strm_req                                 
                            );                            
                        player_stop(pt_running_svc_req->pt_player);                        
                    }                    
                }                    
                else                    
                {                        
                    VSH_CHAP_INFO_T t_set_info = {0};                        
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info_next);                        
                    if(i4_ret == SVCTXR_OK)                        
                    {                             
                        /*seek to next chapter*/                                
                        MM_SVC_POS_INFO_T t_pos_info;                                                                
                        t_set_info.b_seek = FALSE;                                
                        t_set_info.b_first = FALSE;
                        t_set_info.ui8_start_time = 0;                                
                        t_set_info.ui8_end_time = 0xFFFFFFFFFFFFFFFF;                                
                        i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));                                                                
                        t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;                                
                        if (pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE)/*it's done of FF*/                                
                        {                                    
                            t_pos_info.ui8_pos_val = t_chap_info_next.ui8_start_time;                                
                        }                                
                        else                                
                        {                                    
                            t_pos_info.ui8_pos_val = t_chap_info_next.ui8_end_time;                                
                        }                                
                        pt_media_desc->b_divxplus_seek = TRUE;                                
                        i4_ret = mm_hdlr_set(                                                
                            pt_svctx,                                                
                            SVCTX_MM_SET_TYPE_SEEK,                                                
                            (VOID*)&t_pos_info,                                                
                            sizeof(MM_SVC_POS_INFO_T)                                                
                            );                                
                        pt_media_desc->b_divxplus_seek = FALSE;                                
                        if(SVCTXR_OK != i4_ret)                                
                        {                                    
                            return SVCTXR_FAIL;                                
                        }                                
                        pt_media_desc->b_is_seeking = TRUE;                                
                        /*notify to ap*/                               
                        mm_hdlr_invoke_client_svc_nfy_fct(                                            
                                pt_svctx,                                            
                                SVCTX_NTFY_CODE_CHAPTER_CHANGE,                                            
                                ST_VIDEO                                            
                                );   
                        if((pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE &&                               
                            (t_chap_info_next.ui8_start_time > t_chap_info.ui8_end_time + 1 ||                               
                            t_chap_info_next.ui8_start_time < t_chap_info.ui8_start_time)) ||                               
                            (pt_media_desc->e_speed <= MM_SPEED_TYPE_STEP_REVERSE &&                               
                            (t_chap_info.ui8_start_time > t_chap_info_next.ui8_end_time + 1 ||                               
                            t_chap_info.ui8_start_time < t_chap_info_next.ui8_start_time)))                            
                        {
                            t_set_info.b_seek = TRUE;                            
                        }                            
                        else                            
                        {                                
                            t_set_info.b_seek = FALSE;                            
                        }                            
                        t_set_info.b_first = FALSE;
                        t_set_info.ui8_start_time = t_chap_info_next.ui8_start_time;                            
                        t_set_info.ui8_end_time = t_chap_info_next.ui8_end_time;                            
                        i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));                        
                    }                    
                }                    
                i4_ret = SVCTXR_OK;                
            }            
        }            
        break;
#endif    
    case SM_EVN_VS_FRAMERATE_NOT_SUPPORT: 
        x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_FRAMERATE_NOT_SUPPORT\n");      
        mm_hdlr_invoke_client_svc_nfy_fct(                 
                pt_svctx,                 
                SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT,                 
                ST_VIDEO                 
                );    
        if (!pt_svctx->b_in_stopping)
        {
            mm_hdlr_stop_svc(pt_svctx,TRUE);
            pt_svctx->b_in_stopping = TRUE;
            pt_media_desc->b_video_thumbnail_done = TRUE;
            pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;
            svctx_timer_stop(pt_svctx);
        }     
        break;
        
    case SM_EVN_VS_EOS:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_EOS\n");
#ifdef DIVX_PLUS_CER
            if(pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap && 
               pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap &&
               (!pt_media_desc->b_is_thumbnail_playback) &&
               !pt_media_desc->b_is_seeking)
            {
                VSH_CHAP_INFO_T t_set_info = {0};
                MINFO_CHAP_INFO_T t_chap_info = {0};
                MINFO_CHAP_INFO_T t_chap_info_next;
                
                if(IS_FORWARD_DIR(pt_media_desc->e_speed)&&
                   pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info) == SVCTXR_OK)
                {
                    /*set next chapter*/
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap(pt_media_desc, TRUE, NULL);
                    if(i4_ret != SVCTXR_OK)
                    {
                        /*no next chapter then stop */
                        if (!pt_svctx->b_in_stopping)
                        {
                            mm_hdlr_invoke_client_strm_nfy_fct(
                                 pt_svctx,
                                 SVCTX_NTFY_CODE_MEDIA_EOS,
                                 pt_strm_req
                                 );
                            player_stop(pt_running_svc_req->pt_player);
                        }
                        break;
                    }
                    else
                    {
                        i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info_next);
                    }
                }
                else if (IS_BACKWARD_DIR(pt_media_desc->e_speed)&&
                    pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info) == SVCTXR_OK)
                {
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 2, &t_chap_info_next);
                    if(i4_ret != SVCTXR_OK)
                    {
                        /*no next chapter then stop */
                        if (!pt_svctx->b_in_stopping)
                        {
                            mm_hdlr_invoke_client_strm_nfy_fct(
                                 pt_svctx,
                                 SVCTX_NTFY_CODE_MEDIA_EOS,
                                 pt_strm_req
                                 );
                            player_stop(pt_running_svc_req->pt_player);
                        }    
                    }
                    break;
                }
                else
                {
                    i4_ret = SVCTXR_FAIL;
                }
                if(i4_ret == SVCTXR_OK)
                {
                    /*seek to next chapter*/
                    MM_SVC_POS_INFO_T t_pos_info;
                    
                    t_set_info.b_seek = FALSE;
                    t_set_info.b_first = FALSE;
                    t_set_info.ui8_start_time = 0;
                    t_set_info.ui8_end_time = 0xFFFFFFFFFFFFFFFF;
                    i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                    
                    t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
                    if (pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE)/*it's done of FF*/
                    {
                        t_pos_info.ui8_pos_val = t_chap_info_next.ui8_start_time;
                    }
                    else
                    {
                        t_pos_info.ui8_pos_val = t_chap_info_next.ui8_end_time;
                    }
                    pt_media_desc->b_divxplus_seek = TRUE;
                    i4_ret = mm_hdlr_set(
                                    pt_svctx,
                                    SVCTX_MM_SET_TYPE_SEEK,
                                    (VOID*)&t_pos_info,
                                    sizeof(MM_SVC_POS_INFO_T)
                                    );
                    pt_media_desc->b_divxplus_seek = FALSE;
                    if(SVCTXR_OK != i4_ret)
                    {
                        return SVCTXR_FAIL;
                    }
                    pt_media_desc->b_is_seeking = TRUE;
                    /*notify to ap*/
                    mm_hdlr_invoke_client_svc_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_CHAPTER_CHANGE,
                                ST_VIDEO
                                );
                    pt_media_desc->b_is_seeking = TRUE;
                    if((pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE &&
                       (t_chap_info_next.ui8_start_time > t_chap_info.ui8_end_time + 1 ||
                       t_chap_info_next.ui8_start_time < t_chap_info.ui8_start_time)) ||
                       (pt_media_desc->e_speed <= MM_SPEED_TYPE_STEP_REVERSE &&
                       (t_chap_info.ui8_start_time > t_chap_info_next.ui8_end_time + 1 ||
                       t_chap_info.ui8_start_time < t_chap_info_next.ui8_start_time)))
                    {
                        t_set_info.b_seek = TRUE;
                    }
                    else
                    {
                        t_set_info.b_seek = FALSE;
                    }
                    t_set_info.b_first = FALSE;
                    t_set_info.ui8_start_time = t_chap_info_next.ui8_start_time;
                    t_set_info.ui8_end_time = t_chap_info_next.ui8_end_time;
                    i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                    break;
                }
            }
                   
#endif
            if (IS_BACKWARD_DIR(pt_media_desc->e_speed))
            {
                if (TRUE == pt_strm_msg->uinfo.ui4_data)/*it's video eos of FF*/
                {
                    x_dbg_stmt(_ERROR_HEADER"video eos, state mismatch, ignore line(%d)\n",__LINE__);
                    break;
                }
                
                /* replay the media again.*/
        		mm_hdlr_invoke_client_strm_nfy_fct(
				 pt_svctx,
				 SVCTX_NTFY_CODE_MEDIA_BOS,
				 pt_strm_req
				 );
			break;

            }
            else if (!pt_svctx->b_in_stopping)
            {
                if (FALSE == pt_strm_msg->uinfo.ui4_data)/*it's video eos of FR*/
                {
                    x_dbg_stmt(_ERROR_HEADER"video eos, state mismatch, ignore line(%d)\n",__LINE__);
                    break;
                }
                //if (pt_media_desc->b_still_at_last)
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                         pt_svctx,
                         SVCTX_NTFY_CODE_MEDIA_EOS,
                         pt_strm_req
                         );
                    break;
                }
                pt_media_desc->ui1_stream_eos_status |= (ST_VIDEO+1);
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
                {
                    if(!(pt_media_desc->ui1_stream_eos_status & (ST_AUDIO+1))
                            && pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
                    {
                        return SVCTXR_OK;
                    }
                }
                if (!pt_media_desc->b_is_thumbnail_playback)
                {
                     mm_hdlr_stop_svc(pt_svctx,TRUE);
                }
                else
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                         pt_svctx,
                         SVCTX_NTFY_CODE_MEDIA_EOS,
                         pt_strm_req
                         );
                }
            }
            break;
    case SM_EVN_AS_EOS:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_AS_EOS\n");
            if (!pt_svctx->b_in_stopping)
            {
                if (IS_BACKWARD_DIR(pt_media_desc->e_speed))
                {
                    if (TRUE == pt_strm_msg->uinfo.ui4_data)/*it's video eos of FF*/
                    {
                        x_dbg_stmt(_ERROR_HEADER"audio eos, state mismatch, ignore line(%d)\n",__LINE__);
                        //break;
                    }
                    	mm_hdlr_invoke_client_strm_nfy_fct(
                         pt_svctx,
                         SVCTX_NTFY_CODE_MEDIA_BOS,
                         pt_strm_req
                         );
                }
                else
                {
                    if (FALSE == pt_strm_msg->uinfo.ui4_data)/*it's video eos of FR*/
                    {
                        x_dbg_stmt(_ERROR_HEADER"audio eos, state mismatch, ignore line(%d)\n",__LINE__);
                        //break;
                    }
                    
                    pt_media_desc->ui1_stream_eos_status |= (ST_AUDIO+1);
                    if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
                    {
                        if(!(pt_media_desc->ui1_stream_eos_status & (ST_VIDEO+1)))
                        {
                            return SVCTXR_OK;
                        }
                    }
                   	else
					{
						mm_hdlr_invoke_client_strm_nfy_fct(
                         pt_svctx,
                         SVCTX_NTFY_CODE_MEDIA_EOS,
                         pt_strm_req
                         );
					}
                   //mm_hdlr_stop_svc(pt_svctx,TRUE);
                }
            }
            else
            {/*skip the latter audio eos*/
                return SVCTXR_OK;
            }
            break;
    case SM_EVN_AS_CODEC_NOT_SUPPORT:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_AS_CODEC_NOT_SUPPORT\n");
            mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
                    pt_strm_req
                    );
            break;
    case SM_EVN_VS_CODEC_NOT_SUPPORT:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_CODEC_NOT_SUPPORT\n");
            mm_hdlr_invoke_client_strm_nfy_fct(
               pt_svctx,
               SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT,
               pt_strm_req
               );
           if (!pt_svctx->b_in_stopping && pt_media_desc->b_is_thumbnail_playback)
            {
                mm_hdlr_stop_svc(pt_svctx,TRUE);
                pt_svctx->b_in_stopping = TRUE;
                pt_media_desc->b_video_thumbnail_done = TRUE;
                pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;
                svctx_timer_stop(pt_svctx);
            }
            break;
    case SM_EVN_AS_DECODE_STATUS_CHG:
        break;
    case SM_EVN_VS_DISP_FRAME:
                mm_hdlr_invoke_client_strm_nfy_fct(
               pt_svctx,
               SVCTX_NTFY_CODE_MEDIA_VIDEO_PRESENT,
               pt_strm_req
           );
          break;
    case SM_EVN_VS_BIT_RATE:
        pt_media_desc->ui4_bit_rate = pt_strm_msg->uinfo.ui4_data;
        mm_hdlr_invoke_client_strm_nfy_fct(
                       pt_svctx,
                       SVCTX_NTFY_CODE_MEDIA_VS_BIT_RATE,
                       pt_strm_req
                       );
        break;
    case SM_EVN_VS_DIG_DECODE_STATUS_CHG:
        switch(pt_strm_msg->uinfo.e_video_decode_status) 
        {
        case VSH_DIG_DECODE_CODEC_NOT_SUPPORT:
        case VSH_DIG_DECODE_HD_NOT_SUPPORT:
        case VSH_DIG_DECODE_RES_NOT_SUPPORT:    
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            if(pt_media_desc->b_is_seeking)
            {
                /*seek ready*/
                 mm_hdlr_invoke_client_svc_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_SEEK_READY,
                                ST_UNKNOWN
                                );
               pt_media_desc->b_is_seeking = FALSE;
            }
            if(VSH_DIG_DECODE_CODEC_NOT_SUPPORT == pt_strm_msg->uinfo.e_video_decode_status)
            {
                x_dbg_stmt(_INFO_HEADER" DRIVER SEND VSH_DIG_DECODE_CODEC_NOT_SUPPORT\n");
                mm_hdlr_invoke_client_strm_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT,
                                pt_strm_req
                                );
            }
            else if (VSH_DIG_DECODE_HD_NOT_SUPPORT == pt_strm_msg->uinfo.e_video_decode_status)
            {
                x_dbg_stmt(_INFO_HEADER" DRIVER SEND VSH_DIG_DECODE_HD_NOT_SUPPORT\n");
                mm_hdlr_invoke_client_strm_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT,
                                pt_strm_req
                                );
            }
            else
            {
                x_dbg_stmt(_INFO_HEADER" DRIVER SEND VSH_DIG_DECODE_RES_NOT_SUPPORT\n");
                mm_hdlr_invoke_client_strm_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT,
                                pt_strm_req
                                );
            }
            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
            if (i4_ret != SMR_OK)
            {
                i4_ret = SVCTXR_FAIL;
            }
            break;
        case VSH_DIG_DECODE_NO_DATA:
            break;
               /*intention miss break*/
        case VSH_DIG_DECODE_ERROR:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND VSH_DIG_DECODE_ERROR\n");
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            svctx_reset_video_info(pt_svctx);
            if (pt_strm_msg->uinfo.e_video_decode_status == VSH_DIG_DECODE_ERROR)
            {
                strm_fmt_chker_set_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);
            }
            mm_hdlr_check_strms_status(pt_svctx);
            break;
        case VSH_DIG_DECODE_NORMAL:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND VSH_DIG_DECODE_NORMAL\n");
            if ((pt_svctx->b_is_blocked == FALSE)
                 &&(pt_svctx->e_video_decode_status == VSH_DIG_DECODE_NO_DATA
                    || pt_svctx->e_video_decode_status == VSH_DIG_DECODE_ERROR))
            {

                svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

                if (svctx_is_valid_video_fmt(pt_svctx) == FALSE)
                {
                    svctx_update_video_info_from_sm(pt_svctx, pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
                    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE)
                    {
                        mm_hdlr_invoke_client_strm_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                                    pt_strm_req
                                    );
                    }
                    _mm_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
                }
            }
            break;
        case VSH_DIG_DECODE_UNKNOWN:
        default:
            break;
        }
        pt_svctx->e_video_decode_status = pt_strm_msg->uinfo.e_video_decode_status;
        break;
    case SM_EVN_VS_THUMBNAIL_DONE:
            x_dbg_stmt(_INFO_HEADER" DRIVER SEND SM_EVN_VS_THUMBNAIL_DONE\n");
            if (!pt_svctx->b_in_stopping)
            {
                mm_hdlr_stop_svc(pt_svctx,TRUE);
                pt_media_desc->b_video_thumbnail_done = TRUE;
                pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;
                svctx_timer_stop(pt_svctx);
            }
            break;

    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        switch(svctx_get_strm_req_strm_type(pt_strm_req))
        {
        case ST_VIDEO:
            /* it could be from clear to scrambled,
             * and need to set video plane to blank(background) mode
             * and reset video format information.
             */
            if (pt_svctx->ui4_frz_frozen_strm_mask != 0)
            {
                pt_svctx->ui4_frz_frozen_strm_mask = 0;
                mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_UNFREEZE,
                        ST_UNKNOWN
                        );
            }

            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            svctx_reset_video_info(pt_svctx);
            break;
        default:
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_handle_copy_protection(
                        pt_svctx,
                        pt_strm_req,
                        pt_strm_msg);
#endif
        pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
        pt_svctx->b_delay_video_mute_check = TRUE;/*No need to delay check*/
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_strm_req
                    );
        pt_svctx->b_delay_video_mute_check = FALSE;
        break;
    case SM_EVN_CA_COND_UNKNOWN:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_handle_copy_protection(
                        pt_svctx,
                        pt_strm_req,
                        pt_strm_msg);
#endif
        pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
        break;/*return SVCTXR_OK;*/
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED:
    case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
        switch(svctx_get_strm_req_strm_type(pt_strm_req))
        {
        case ST_VIDEO:
            if ((svctx_is_scrambled_strm(&pt_strm_req->t_ca) == TRUE)
                    && (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED)
                    && (pt_svctx->b_is_blocked == FALSE))
            {
                /* if it is from scrambled to clear and current content is not
                 * blocked, it need to set video plane to normal/dinit mode
                 * and re-get and re-send video format information again
                 */
                svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

                pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
                if (svctx_is_valid_video_fmt(pt_svctx) == FALSE)
                {
                    svctx_update_video_info_from_sm(pt_svctx, pt_strm_req->h_stream);
                    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE)
                    {
                        mm_hdlr_invoke_client_strm_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                                    pt_strm_req
                                    );
                    }
                    _mm_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
                }
            }
            else
            {
                pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
            }
            break;
        default:
            pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_handle_copy_protection(
                        pt_svctx,
                        pt_strm_req,
                        pt_strm_msg);
#endif
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_strm_req
                    );
        break;
    case SM_EVN_CCI_INFORMATION:
        pt_strm_req->t_cp = pt_strm_msg->uinfo.t_cp;
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CP,
                    pt_strm_req
                    );
        break;
    }

    if (pt_strm_req == NULL)
    {
        return SVCTXR_FAIL;
    }
    if (pt_strm_req->e_strm_cond == pt_strm_msg->e_cond) 
    {
        return SVCTXR_OK;
    }

    pt_strm_req->e_strm_cond = pt_strm_msg->e_cond;
    switch(pt_strm_req->e_strm_cond) 
    {
    case SM_COND_OPENED:
        if (pt_svctx->b_in_stopping == TRUE)
        {
            if ((pt_media_desc->ui1_stream_eos_status & (ST_AUDIO+1)) ||
                (pt_media_desc->ui1_stream_eos_status & (ST_VIDEO+1)))
            {
                if (!pt_media_desc->b_is_thumbnail_playback)
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_EOS,
                        pt_strm_req
                        );
                }
            }
        }
        else if(pt_strm_msg->e_event != SM_EVN_AS_DRV_STOPPED)
        {
            DBG_INFO((_INFO_HEADER" open stream ok. "));
            if (e_strm_type == ST_AUDIO)
            {
                 DBG_INFO((_INFO_HEADER"stream type is: audio.\n"));
            }
            else if (e_strm_type == ST_VIDEO)
            {
                DBG_INFO((_INFO_HEADER"stream type is: video.\n"));
            }
            else
            {
                DBG_INFO((_INFO_HEADER"stream type is: %d.\n", e_strm_type));
            }

            /* check the audio only
               set the value to audio decoder
            */
            if (e_strm_type == ST_AUDIO)
            {
                UINT16 ui2_vid_num = 0;
                BOOL   b_aud_only  = FALSE;
                i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc, ST_VIDEO, &ui2_vid_num);
                if ((i4_ret == SVCTXR_OK) && (ui2_vid_num == 0))
                {
                    b_aud_only = TRUE;
                }
                x_sm_set(h_stream,
                         SM_ASH_SET_TYPE_AUDIO_ONLY_INFO,
                         (VOID*)(UINT32)b_aud_only,
                         sizeof(BOOL));
            }
            
            i4_ret = _mm_hdlr_set_strm_codec_spec_info(pt_running_svc_req->h_media_desc,
                                                       h_stream,
                                                       pt_strm_req->u.t_scdb.t_comp_id);
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt(_INFO_HEADER"set codec special info fail! line(%d)\n",__LINE__);
                if (e_strm_type == ST_AUDIO)
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
                       pt_strm_req
                       );
                    i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
                    if (i4_ret != SMR_OK)
                    {
                        i4_ret = SVCTXR_FAIL;
                    }
                }
            }

#ifdef TIME_SHIFT_SUPPORT
            if ((MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))  &&
                (pt_media_desc->b_is_scdb_updated == FALSE)     &&
                (e_strm_type == ST_AUDIO)                       &&
                (pt_media_desc->e_speed == MM_SPEED_TYPE_PAUSE))
            {
                if (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
                {
                    mm_hdlr_invoke_client_svc_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_AUDIO_ONLY_SVC,
                                    ST_UNKNOWN
                                    );
                }
            }
            if(MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt)    &&
               (e_strm_type == ST_AUDIO)                       &&
               (pt_media_desc->e_speed == MM_SPEED_TYPE_PAUSE) &&
               (pt_strm_req->e_penging_status == STRM_PENDING_COND_SELECT_STRM))
            {                 
                mm_hdlr_invoke_client_strm_nfy_fct(pt_svctx,
                                                   SVCTX_NTFY_CODE_STREAM_OPENED,
                                                   pt_strm_req);
            }
#endif
        }
        break;
    case SM_COND_STARTED:
        if(pt_svctx->b_in_stopping)
        {
            break;
        }
        if (e_strm_type == ST_AUDIO)
        {
            DBG_INFO((_INFO_HEADER"stream started type is: audio.\n"));
        }
        else if (e_strm_type == ST_VIDEO)
        {
            DBG_INFO((_INFO_HEADER"stream started type is: video.\n"));
        }
        else if (e_strm_type == ST_SUBTITLE)
        {
            DBG_INFO((_INFO_HEADER"stream started type is: subtitle.\n"));
        }
        else
        {
            DBG_INFO((_INFO_HEADER"stream started type is: %d.\n",e_strm_type));
        }

        if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
            (pt_media_desc->b_is_scdb_updated == FALSE) &&
            (e_strm_type == ST_AUDIO || e_strm_type == ST_VIDEO))
        {
            do {
                VSH_SRC_RESOLUTION_INFO_T t_orig_video_info;
#ifdef TIME_SHIFT_SUPPORT
                MM_SPEED_TYPE_T           e_orig_speed_type = MM_SPEED_TYPE_FORWARD_1X;
#endif

                if (e_strm_type == ST_VIDEO)
                {
                    if((x_handle_valid(pt_running_svc_req->t_strm_reqs_3d_vid.h_stream)     && 
                            (pt_running_svc_req->t_strm_reqs_3d_vid.e_strm_cond != SM_COND_STARTED))
                     ||((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))&&
                            (pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond != SM_COND_STARTED)))
                    {
                        break;
                    }
                    else if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
                    {
                        if(pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X &&
                           pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond != SM_COND_STARTED)
                        {
                            break;
                        }
                    }
                    else
                    {
                        /* video only case */
                    }

                    /*2 videos has started*/
                    if (pt_media_desc->e_fill_scdb_mode == MM_SVC_FILL_SCDB_APPEND)
                    {
                        x_dbg_stmt("[mm_hdlr][info]set release table cmd to playback.\n");
                        x_cm_set(pt_running_svc_req->pt_player_act->h_conn,
                                 PLAYBACK_CTRL_SET_RELEASE_TABLES,
                                 NULL);
                    }
                }
                if (e_strm_type == ST_AUDIO)
                {
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
                    {
                        if((pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond != SM_COND_STARTED) &&
                            /*and !(video codec not support or video format update)*/
                           !(pt_running_svc_req->pt_strm_fmt_chker->ui4_strm_format_cond & MAKE_BIT_MASK_32(ST_VIDEO)))
                        {
                            break;
                        }
                    }
                    else
                    {
                        /* audio only case */
                    }

                }

                pt_media_desc->b_is_scdb_updated = TRUE;
                pt_media_desc->b_is_parse_pmt    = FALSE;

                /* disable BLOCK_PSI */
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_BLOCK_PSI,
                                  (void*)(UINT32)FALSE,
                                  sizeof(BOOL));

                if (i4_ret!= SMR_OK)
                {
                    ASSERT(FALSE, ("%s(), SM_SESS_GNRC_SET_TYPE_BLOCK_PSI fail!\r\n", __func__));
                }

                t_trick_info.e_speed_type = pt_media_desc->e_speed;
#ifdef TIME_SHIFT_SUPPORT
                if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
                    t_trick_info.e_speed_type != MM_SPEED_TYPE_FORWARD_1X &&
                    t_trick_info.e_speed_type != MM_SPEED_TYPE_STEP)
                {
                    pt_media_desc->b_trick_mode_revive = TRUE;
                    e_orig_speed_type = pt_media_desc->e_speed;
                    t_trick_info.e_speed_type = MM_SPEED_TYPE_STEP;
                }
#endif /* TIME_SHIFT_SUPPORT */
                t_orig_video_info = pt_svctx->t_video_info; /* backup */
                /* set trick mode forcedly */
                pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_VIDEO;
                pt_svctx->t_video_info.ui4_width = 0xFFFFFFFF;
                pt_svctx->t_video_info.ui4_height = 0xFFFFFFFF;
                i4_ret = mm_hdlr_set(pt_svctx,
                                     SVCTX_MM_SET_TYPE_TRICK,
                                     (VOID*)&t_trick_info,
                                     sizeof(MM_SVC_TRICK_INFO_T));
#ifdef TIME_SHIFT_SUPPORT
                if (pt_media_desc->b_trick_mode_revive)
                {
                    pt_media_desc->e_speed = e_orig_speed_type;
                }
#endif /* TIME_SHIFT_SUPPORT */
                pt_svctx->t_video_info = t_orig_video_info; /* restore */
                if (i4_ret!= SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK fail!\n"));
                }

                mm_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
                            ST_UNKNOWN);

                mm_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_NORMAL,
                            ST_UNKNOWN
                            );

                mm_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
                            ST_UNKNOWN
                            );

            } while (0);
        }
#ifdef TIME_SHIFT_SUPPORT
        else if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt)
                 && (e_strm_type == ST_VIDEO)
                 && (pt_media_desc->b_trick_mode_revive))
        {
            if (pt_media_desc->e_speed == MM_SPEED_TYPE_STEP
                || pt_media_desc->e_speed == MM_SPEED_TYPE_PAUSE)
            {
                VSH_SRC_RESOLUTION_INFO_T    t_orig_video_info;
                MM_SPEED_TYPE_T              e_orig_speed_type = pt_media_desc->e_speed;

                pt_media_desc->b_trick_mode_revive = FALSE;
                t_trick_info.e_speed_type = MM_SPEED_TYPE_STEP;

                t_orig_video_info = pt_svctx->t_video_info; /* backup */

                /* set trick mode forcely */
                pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_VIDEO;
                pt_svctx->t_video_info.ui4_width = 0xFFFFFFFF;
                pt_svctx->t_video_info.ui4_height = 0xFFFFFFFF;

                i4_ret = mm_hdlr_set(pt_svctx,
                                     SVCTX_MM_SET_TYPE_TRICK,
                                     (VOID*)&t_trick_info,
                                     sizeof(MM_SVC_TRICK_INFO_T));
                if (i4_ret!= SVCTXR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"SVCTX_MM_SET_TYPE_TRICK fail!\n"));
                }
                pt_media_desc->e_speed = e_orig_speed_type;

                pt_svctx->t_video_info = t_orig_video_info; /* restore */
            }
        }
#endif /* TIME_SHIFT_SUPPORT */
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (ST_VIDEO == e_strm_type)
        {
            svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_ENABLE);
        }
        else if (ST_AUDIO == e_strm_type)
        {
            svctx_ctrl_main_audio(pt_svctx, FALSE);
        }
#endif

        if (e_strm_type == ST_MM_SUBTITLE ||
            e_strm_type == ST_SUBTITLE)
        {
             mm_hdlr_invoke_client_strm_nfy_fct(
                 pt_svctx,
                 SVCTX_NTFY_CODE_MEDIA_SUBTITLE_FMT_UPDATE,
                 pt_strm_req
                 );
        }
        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            i4_ret = _mm_hdlr_handle_strm_started_nty_as_presenting(pt_svctx, pt_strm_req);
        } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
           /* i4_ret = _mm_hdlr_handle_strm_started_nty_as_pending(pt_svctx, pt_strm_req);*/
        }
        break;
    case SM_COND_CLOSED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (ST_VIDEO == e_strm_type)
        {
            svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_DISABLE);
        }
        else if (ST_AUDIO == e_strm_type)
        {
            svctx_ctrl_main_audio(pt_svctx, FALSE);
        }
#endif
        if (pt_strm_req->t_stop_cb.pf_nfy != NULL) {
            pt_strm_req->t_stop_cb.pf_nfy(pt_strm_req->t_stop_cb.pv_tag);
        }

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            i4_ret = _mm_hdlr_handle_strm_closed_nty_as_presenting(pt_svctx, pt_strm_req);
        } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
          /*  i4_ret = _mm_hdlr_handle_strm_closed_nty_as_pending(pt_svctx, pt_strm_req);*/
        }
        break;
    case SM_COND_ERROR:
        break;
    }
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    return SVCTXR_OK;
}

#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_update_tuner_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_update_tuner_type(
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    INT32     i4_ret;
    TSL_REC_T t_tsl_rec;

    i4_ret = mm_hdlr_get_crnt_tsl_rec(
                    pt_running_svc_req,
                    &t_tsl_rec
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    switch(t_tsl_rec.uheader.t_desc.e_bcst_medium) {
    case BRDCST_MEDIUM_DIG_TERRESTRIAL:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_TER_DIG;
        break;
    case BRDCST_MEDIUM_DIG_CABLE:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_CAB_DIG;
        break;
    case BRDCST_MEDIUM_DIG_SATELLITE:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_SAT_DIG;
        break;
    case BRDCST_MEDIUM_ANA_TERRESTRIAL:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_TER_ANA;
        break;
    case BRDCST_MEDIUM_ANA_CABLE:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_CAB_ANA;
        break;
    case BRDCST_MEDIUM_ANA_SATELLITE:
        pt_running_svc_req->e_crnt_tuner_type = DEV_TUNER_SAT_ANA;
        break;
    case BRDCST_MEDIUM_1394:
        pt_running_svc_req->e_crnt_tuner_type = DEV_1394;
        break;
    default:
        pt_running_svc_req->e_crnt_tuner_type = DEV_UNKNOWN;
        break;
    }

    return SVCTXR_OK;
}
#endif /* TIME_SHIFT_SUPPORT */
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_cm_cmds
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static  INT32 _mm_hdlr_get_cm_cmds(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    CM_COMMAND_T*               pt_cm_cmds
                    )
{
    INT32   i4_ret = SVCTXR_OK;
    UINT8   ui1_i  = 0;

    pt_cm_cmds[0].e_code = CONN_END;

    switch (pt_running_svc_req->t_parent.e_src_type) {
#ifdef TIME_SHIFT_SUPPORT
       case SRC_TYPE_MEDIA_TIME_SHIFT:
       {
           SVL_REC_T t_svl_rec;

           i4_ret = mm_hdlr_get_crnt_svl_rec(
                       pt_running_svc_req,
                       &t_svl_rec
                       );
           if (i4_ret != SVCTXR_OK) {
               break;
           }

           _mm_hdlr_update_tuner_type(pt_running_svc_req, &t_svl_rec);

           pt_running_svc_req->e_brdcst_type = t_svl_rec.uheader.t_rec_hdr.e_brdcst_type;
       }
#endif /* TIME_SHIFT_SUPPORT */
       case SRC_TYPE_MEDIA_STORGE:
       case SRC_TYPE_MEDIA_PVR:
       {
          pt_cm_cmds[ui1_i].e_code          = CONN_SRC_TYPE;
          pt_cm_cmds[ui1_i++].u.ps_name     = PLAYBACK_HANDLER_DEFAULT_NAME;
          pt_cm_cmds[ui1_i].e_code          = PLAYBACK_CONN_DISC_IF_FILE_ERROR;
          pt_cm_cmds[ui1_i++].u.b_boolean   = FALSE;
          pt_cm_cmds[ui1_i].e_code          = PLAYBACK_CONN_DUAL_MONO_FAKE_STEREO;
          pt_cm_cmds[ui1_i++].u.b_boolean   = TRUE;
          pt_cm_cmds[ui1_i].e_code          = CONN_END;
          break;
       }
       case SRC_TYPE_MEDIA_MEM_CLIP:
       {
          break;
       }
       case SRC_TYPE_MEDIA_NETWORK:
       {
            HANDLE_T h_media = NULL_HANDLE;
            MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
            h_media = pt_running_svc_req->h_media_desc;
            i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
            
            pt_cm_cmds[ui1_i].e_code        = CONN_SRC_TYPE;
            pt_cm_cmds[ui1_i++].u.ps_name   = PLAYBACK_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[ui1_i].e_code        = PLAYBACK_CONN_DUAL_MONO_FAKE_STEREO;
            pt_cm_cmds[ui1_i++].u.b_boolean = TRUE;
          
            if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI)
            {                
                if (MM_SVC_BUFFCTRL_TYPE_BYTE == pt_media_desc->t_bufctrl_info.e_ctrl_type)
                {
                    pt_media_desc->t_cm_mode_ctrl.e_op_mode = PLAYBACK_CONN_OPERATION_MODE_CIRCULAR;
                    pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_buffer_len
                        = pt_media_desc->t_bufctrl_info.ui4_buf_size.ui4_bytes;
                    pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_keep_buffer_threshold
                        = pt_media_desc->t_bufctrl_info.ui4_keepbuf_threshold;
                    pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_re_buferf_threshold
                        = pt_media_desc->t_bufctrl_info.ui4_rebuf_threshold;
                }
                else
                {
                    /*Todo: need to transfer from duration to bytes*/
                }
                pt_cm_cmds[ui1_i].e_code        = PLAYBACK_CONN_OPERATION_MODE;
                pt_cm_cmds[ui1_i++].u.pv_ptr    = &pt_media_desc->t_cm_mode_ctrl;
                
                pt_cm_cmds[ui1_i].e_code        = CONN_END;
            }
            else if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH)
            {
                pt_media_desc->t_cm_mode_ctrl.e_op_mode = PLAYBACK_CONN_OPERATION_MODE_PUSH_SHARE_MEM;
                pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_buffer_len
                            = pt_media_desc->t_bufctrl_info.ui4_buf_size.ui4_bytes;
                pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_keep_buffer_threshold
                            = pt_media_desc->t_bufctrl_info.ui4_keepbuf_threshold;
                pt_media_desc->t_cm_mode_ctrl.u.t_flow_ctrl.ui4_re_buferf_threshold
                            = pt_media_desc->t_bufctrl_info.ui4_rebuf_threshold;
                pt_media_desc->t_cm_mode_ctrl.ui4_pid = 0;
                
                pt_cm_cmds[ui1_i].e_code        = PLAYBACK_CONN_OPERATION_MODE;
                pt_cm_cmds[ui1_i++].u.pv_ptr    = &pt_media_desc->t_cm_mode_ctrl;
                
                pt_cm_cmds[ui1_i].e_code        = PLAYBACK_CONN_SHARED_MEMORY;
                pt_cm_cmds[ui1_i++].u.b_boolean = TRUE;
                
                if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_NETWORK_SKYPE)
                {
                    pt_cm_cmds[ui1_i].e_code         = PLAYBACK_CONN_SOURCE_TYPE;
                    pt_cm_cmds[ui1_i++].u.ui4_number   = PLAYBACK_CONN_SOURCE_TYPE_SKYPE;
                    pt_cm_cmds[ui1_i].e_code         = CONN_END;                                                                  
                }
                else
                {
                    pt_cm_cmds[ui1_i].e_code         = CONN_END;
                }  
            }
            else if (pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
            {
                pt_media_desc->t_cm_mode_ctrl.e_op_mode = PLAYBACK_CONN_OPERATION_MODE_PULL;
                
                pt_cm_cmds[ui1_i].e_code        = PLAYBACK_CONN_OPERATION_MODE;
                pt_cm_cmds[ui1_i++].u.pv_ptr    = &pt_media_desc->t_cm_mode_ctrl;

                pt_cm_cmds[ui1_i].e_code        = CONN_END;
            }
          break;
       }
    }

    return i4_ret;
}

#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_svl
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 *      pt_msg          the pointer points to an SVCTX_MSG_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_svl(
                    MM_SVC_REQ_T*               pt_svc_req,
                    HANDLE_T*                   ph_svl
                    )
{
    INT32 i4_ret;

    ASSERT(pt_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
    ASSERT(pt_svc_req->t_parent.e_src_type == SRC_TYPE_MEDIA_TIME_SHIFT, ("%s(), not SRC_TYPE_MEDIA_TIME_SHIFT\r\n", __func__));

    if (pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id == SVL_NULL_ID) {
        return SVCTXR_FAIL;
    }

    i4_ret = rating_chker_open_svl(
                    pt_svc_req->pt_rating_chker,
                    pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id
                    );
    if (i4_ret == SVCTXR_OK) {
        *ph_svl = rating_chker_get_svl_handle(pt_svc_req->pt_rating_chker);
    }

    return i4_ret;
}
#endif /* TIME_SHIFT_SUPPORT */

static INT32 _mm_hdlr_handle_reset_running_svc_req(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT8            ui1_idx;
    STREAM_TYPE_T    e_strm_type;
    STRM_REQ_T*      pt_strm_req;

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) 
    {
        e_strm_type = e_g_mm_strm_types[ui1_idx];
        if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            break;
        }

        pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

        if(e_strm_type == ST_SUBTITLE) /*only reset subtitle request flag, to avoid affecting next movie playing*/
        {
            pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
        }
    }
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_cm_disconnected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_cm_disconnected(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_signal_loss
                    )
{
    pt_svctx->e_state = SVCTX_COND_PENDING;
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_player_stopped
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_player_stopped(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret;
    MM_SVC_UOP_HANDLER_FCT_TBL_T t_uop_tbl;
    MEDIA_DESCRIBT_T*            pt_media_desc;

    SM_SESS_GNRC_DATA_T*         pt_sess_data;
    HANDLE_T                     h_media ;
    HANDLE_TYPE_T                e_type;

#ifndef DRV_NO_PRE_PARSING
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }
#endif
    x_dbg_stmt(_INFO_HEADER"%s \n", __func__);
    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = handle_get_type_obj(h_media, &e_type, (VOID**)(&pt_media_desc));
    if (i4_ret != HR_OK || e_type != HT_MEDIA_STORGE_ELM_TYPE || pt_media_desc == NULL)
    {
        ASSERT(FALSE, ("%s(), pt_media_desc is NULL\r\n", __func__));
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    pt_media_desc->b_is_set_sec_range = FALSE;
#endif
    /*In case of pt_svctx->b_in_stopping is true , that means the stop is triggle by client
    in stead of mm_hdlr itself for replay*/
    if ( IS_BACKWARD_DIR(pt_media_desc->e_speed)&& !pt_svctx->b_in_stopping)
    {
         /*Open UOP Hander */
       i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
       if (i4_ret != SVCTXR_OK)
       {
             return i4_ret;
       }
       /*close uop handler*/
       t_uop_tbl = pt_running_svc_req->t_uop_fct_tbl;
       if(t_uop_tbl.pf_close)
       {
           i4_ret = t_uop_tbl.pf_close(pt_media_desc);
           if (i4_ret != SVCTXR_OK)
           {
            /*ignore this failed*/
           }
       }
       _mm_hdlr_media_close(pt_running_svc_req->h_media_desc);
       if (pt_media_desc->ui1_presnt_range_cnts!=0)
       {
            ASSERT(0, ("%s(), Error", __func__));
            return SVCTXR_FAIL;
       }
       pt_sess_data = (SM_SESS_GNRC_DATA_T*) x_mem_alloc(sizeof(SM_SESS_GNRC_DATA_T));
       if (pt_sess_data == NULL)
       {
           return SVCTXR_FAIL;
       }
       x_memset(pt_sess_data, 0, sizeof(SM_SESS_GNRC_DATA_T));
       pt_sess_data->t_media_fmt = pt_media_desc->t_media_fmt;
       pt_sess_data->e_mm_sync_mode = SM_SESS_GNRC_MM_SYNC_UNKNOWN;
       pt_running_svc_req->pt_player->pv_sess_data =(VOID*)pt_sess_data;
       pt_running_svc_req->pt_player->z_sess_data_size = sizeof(SM_SESS_GNRC_DATA_T);
       pt_running_svc_req->ui4_title_idx = pt_media_desc->ui4_title_idx;
       pt_running_svc_req->ui4_playlist_idx = pt_media_desc->ui4_playlist_idx;
       pt_running_svc_req->ui4_chap_idx = 0;
       x_memset(&pt_media_desc->t_mm_last_info, 0, sizeof(MM_SVC_STOP_INFO));
       pt_media_desc->t_mm_last_info.ui4_title_idx = pt_media_desc->ui4_title_idx;
       pt_media_desc->t_mm_last_info.ui4_playlist_idx = pt_media_desc->ui4_playlist_idx;
       pt_media_desc->t_mm_last_info.ui4_chap_idx = 0;
#ifdef MM_LAST_MEMORY_SUPPORT       
       x_memset(&pt_running_svc_req->t_init_pos_info, 0, sizeof(MM_SVC_POS_INFO_T));
#endif
       pt_media_desc->b_rtn_frm_rev = TRUE;
       pt_media_desc->pv_sess_data =(VOID*)pt_sess_data;
       pt_media_desc->b_is_scdb_updated = FALSE;
       pt_media_desc->b_is_add_range = FALSE;
       pt_media_desc->b_has_key_table = FALSE;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
       pt_media_desc->b_is_sec_stop = FALSE;
       pt_media_desc->b_is_set_sec_range = FALSE;
#endif
       player_play(pt_running_svc_req->pt_player,100);
       pt_media_desc->e_speed = MM_SPEED_TYPE_FORWARD_1X;
       pt_media_desc->b_set_audio_stop = FALSE;
       mm_hdlr_invoke_client_svc_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
                                ST_UNKNOWN
                                );
       mm_hdlr_invoke_client_svc_nfy_fct(
                                pt_svctx,
                                SVCTX_NTFY_CODE_MEDIA_REPLAY,
                                ST_UNKNOWN
                                );
       return  SVCTXR_OK;
    }

    i4_ret = strm_fmt_chker_close(pt_running_svc_req->pt_strm_fmt_chker);
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
    pt_running_svc_req->pt_strm_fmt_chker = NULL;

    _mm_hdlr_handle_reset_running_svc_req(pt_svctx, pt_running_svc_req);

#ifdef TIME_SHIFT_SUPPORT
#if 0
    i4_ret = rating_chker_close(pt_running_svc_req->pt_rating_chker);
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
    pt_running_svc_req->pt_rating_chker = NULL;

    i4_ret = crnt_evn_close(pt_running_svc_req->pt_crnt_event);
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
    pt_running_svc_req->pt_crnt_event = NULL;
#endif
#endif /* TIME_SHIFT_SUPPORT */

    i4_ret = player_close(pt_running_svc_req->pt_player);
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    if (pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
        i4_ret = player_close(pt_running_svc_req->pt_player_enhance);
        if (i4_ret != SVCTXR_OK) 
        {
            return i4_ret;
        }
    }
#endif
    /*Open UOP Hander */
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK) 
    {
        ASSERT(FALSE, ("%s(), pt_media_desc is NULL!\r\n", __func__));
        return i4_ret;
    }
    /*close uop handler*/
    t_uop_tbl = pt_running_svc_req->t_uop_fct_tbl;
    if(t_uop_tbl.pf_close)
    {
        i4_ret = t_uop_tbl.pf_close(pt_media_desc);
        if (i4_ret != SVCTXR_OK)
        {
          /*ignore this failed*/
        }
    }

    if(pt_media_desc->b_video_thumbnail_done)
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
            pt_svctx,
            SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE,
            ST_VIDEO);
    }
    _mm_hdlr_media_close(pt_running_svc_req->h_media_desc);
    pt_running_svc_req->pt_player = NULL;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    pt_running_svc_req->pt_player_enhance = NULL;
#endif
    pt_running_svc_req->pt_player_act = NULL;

#ifdef TIME_SHIFT_SUPPORT
    if (pt_media_desc->ui4_rec_util_reset_req_id > 0)
    {
        i4_ret = rec_util_permit_reset_req(pt_media_desc->h_rec_util,
                                           pt_media_desc->ui4_rec_util_reset_req_id);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_permit_reset_req() failed!\n"));
            return SVCTXR_FAIL;
        }
        pt_media_desc->ui4_rec_util_reset_req_id = 0;
    }
#endif /* TIME_SHIFT_SUPPORT */

    if(x_handle_valid(pt_media_desc->h_minfo_wait_sema))
    {
        x_sema_unlock(pt_media_desc->h_minfo_wait_sema);
    }
    svctx_ntfy_svc_req_stopped(pt_svctx);
    return SVCTXR_OK;
}

#ifdef ENABLE_NAUTILUS
static VOID _mm_hdlr_drm_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
   SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
   MM_SVC_REQ_T*  pt_running_svc_req = NULL;
   HANDLE_T       h_svctx            = NULL_HANDLE;

   pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
   h_svctx = pt_running_svc_req->t_parent.h_svctx;
   
   _mm_hdlr_send_msg(h_svctx,
                     NULL_HANDLE,
                     _SVCTX_MSG_DRM_TIMEOUT_FORCE_EXIT_NTFY,
                     pv_tag,
                     0,
                     0,
                     0);
}

static BOOL _mm_hdlr_mln_drm_status_cb (VOID*                   pv_tag, 
                                        MARLIN_RTN_DRM_STATUS_E e_drm_status)
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
   
    mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                  (SVCTX_NTFY_CODE_T)(SVCTX_NTFY_CODE_DRM_STATUS_START + e_drm_status),
                                      ST_UNKNOWN);
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_check_mln_crypt_info
 * Description: if current is marlin mp4 file, it need to init and get key
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_check_mln_crypt_info(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT8                   ui1_i           = 0;
    UINT8                   ui1_start_idx   = 0;
    SIZE_T                  z_info_len      = sizeof(MINFO_INFO_T);
    INT32                   i4_ret          = SVCTXR_FAIL;
    BOOL                    fg_is_success   = FALSE;
    MEDIA_DESCRIBT_T*       pt_media_desc   = NULL;
    MINFO_INFO_T            t_minfo_rec;
    MARLIN_CMD_DRM_UNINIT_PARAM_T   t_drm_unit;
    VOID*                   pv_ml_nau_h_drm = NULL;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER)
        && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
        && (pt_media_desc->t_drm_info.e_drm_type == MM_DRM_TYPE_MLN))
    {
        MARLIN_INIT_MEDIA_PARAM_T       t_ml_at_idr_param;
        MARLIN_CMD_DRM_INIT_PARAM_T     t_ml_init_inf;

        x_memset(&t_ml_at_idr_param,0, sizeof(MARLIN_INIT_MEDIA_PARAM_T));
        x_memset(&t_ml_init_inf,    0, sizeof(MARLIN_CMD_DRM_INIT_PARAM_T));

        t_ml_at_idr_param.pui1_act_tkn_url      = pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url;
        t_ml_at_idr_param.ui2_act_tkn_url_length= pt_media_desc->t_drm_info.u.t_mln_drm_inf.ui2_act_tkn_url_length;
        t_ml_at_idr_param.pui1_afl_tkn          = pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn;
        t_ml_at_idr_param.ui2_afl_tkn_length    = pt_media_desc->t_drm_info.u.t_mln_drm_inf.ui2_afl_tkn_length;
        t_ml_at_idr_param.pf_drm_status_dyn_nfy = _mm_hdlr_mln_drm_status_cb;
        t_ml_at_idr_param.pv_cb_tag             = (VOID*)pt_svctx;
        
        /* Should fill notify function here */
        t_ml_init_inf.pt_ml_at_idr_params = &t_ml_at_idr_param;

        fg_is_success = x_marlin_cmd_drm_init(&t_ml_init_inf);
        if(fg_is_success)
        {
            /* Store expire time first */
            if(t_ml_init_inf.ui1_ml_exp_tm_chk_num == 0)
            {
                pt_media_desc->t_drm_ctrl.ui4_max_view_time = (UINT32)-1;/* Not limit*/
            }
            else
            {
                pt_media_desc->t_drm_ctrl.ui4_max_view_time = (UINT32)-1;
                for(ui1_i = 0; 
                    (ui1_i < t_ml_init_inf.ui1_ml_exp_tm_chk_num) && (ui1_i < MARLIN_NAU_TM_NUM); 
                    ui1_i++)
                {
                    if(pt_media_desc->t_drm_ctrl.ui4_max_view_time > t_ml_init_inf.aui4_ml_exp_tm[ui1_i])
                    {
                        pt_media_desc->t_drm_ctrl.ui4_max_view_time = t_ml_init_inf.aui4_ml_exp_tm[ui1_i];
                    }
                }
            }
        }
        else
        {
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                          (SVCTX_NTFY_CODE_T)(SVCTX_NTFY_CODE_DRM_STATUS_START + t_ml_at_idr_param.t_drm_status),
                                              ST_UNKNOWN);
            return SVCTXR_FAIL;
        }

        pv_ml_nau_h_drm = t_ml_init_inf.pv_ml_nau_h_drm;
    }
    if(fg_is_success)
    {
        UINT16  ui2_num = 0;
        MARLIN_RTN_BASIC_CCI_T          t_basic_cci;
        MARLIN_START_TITLE_PARAM_T      t_ml_start_title;
        MARLIN_CMD_DRM_PROC_PARAM_T     t_ml_proc_param;

        x_memset(&t_basic_cci,      0, sizeof(MARLIN_RTN_BASIC_CCI_T));
        x_memset(&t_ml_start_title, 0, sizeof(MARLIN_START_TITLE_PARAM_T));
        x_memset(&t_ml_proc_param,  0, sizeof(MARLIN_CMD_DRM_PROC_PARAM_T));

        ui1_start_idx = 0;
        _mm_hdlr_get_support_strm_num(pt_media_desc,
                                      ST_VIDEO,
                                      &ui2_num);
        if(ui2_num == 1)/*Only support one video*/
        {
            x_memset(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, 
                                      MINFO_INFO_TYPE_VIDEO_ENCRYPTION_INFO,
                                      1,
                                      &t_minfo_rec,
                                      &z_info_len); 
            /* always return OK*/
            x_memcpy(&(t_ml_start_title.at_track_info[ui1_start_idx]),
                     &(t_minfo_rec.u.t_encrytion_info.u.t_minfo_mp4_encry_info.u.t_video_encrytion_info),
                     sizeof(MARLIN_TRACK_INFO_T));
            t_ml_start_title.ui4_track_num = 1;
            pt_media_desc->t_crypt_info.u.t_ml_crypt.t_track_drm[ui1_start_idx].e_strm_type  = ST_VIDEO;
            pt_media_desc->t_crypt_info.u.t_ml_crypt.t_track_drm[ui1_start_idx].ui4_track_id
                = t_minfo_rec.u.t_encrytion_info.u.t_minfo_mp4_encry_info.ui4_strm_id;
        }
        
        ui1_start_idx = ui2_num;
        _mm_hdlr_get_support_strm_num(pt_media_desc,
                                      ST_AUDIO,
                                      &ui2_num);
        for(ui1_i = 0; (ui1_i < 2) && (ui1_i < ui2_num); ui1_i++)/*Only support two audio*/
        {
            x_memset(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, 
                                      MINFO_INFO_TYPE_AUDIO_ENCRYPTION_INFO,
                                      1 + ui1_i,
                                      &t_minfo_rec,
                                      &z_info_len); 
            /* always return OK*/
            x_memcpy(&(t_ml_start_title.at_track_info[ui1_start_idx + ui1_i]),
                     &(t_minfo_rec.u.t_encrytion_info.u.t_minfo_mp4_encry_info.u.t_audio_encrytion_info),
                     sizeof(MARLIN_TRACK_INFO_T));
            pt_media_desc->t_crypt_info.u.t_ml_crypt.t_track_drm[ui1_start_idx + ui1_i].e_strm_type  = ST_AUDIO;
            pt_media_desc->t_crypt_info.u.t_ml_crypt.t_track_drm[ui1_start_idx + ui1_i].ui4_track_id
                = t_minfo_rec.u.t_encrytion_info.u.t_minfo_mp4_encry_info.ui4_strm_id;
        }
        t_ml_start_title.ui4_track_num += ui2_num;
        t_ml_start_title.pt_basic_cci = &t_basic_cci;
        t_ml_proc_param.pt_ml_track_idr_params = &t_ml_start_title;
        
        t_ml_proc_param.pv_ml_nau_h_drm = pv_ml_nau_h_drm;

        fg_is_success = x_marlin_cmd_drm_process(&t_ml_proc_param);
        if(fg_is_success)
        {
            pt_media_desc->t_crypt_info.e_mm_crypt_type = MM_CRYPT_TYPE_MLN;
            pt_media_desc->t_crypt_info.u.t_ml_crypt.ui1_valid_cnt = t_ml_start_title.ui4_track_num;
            for(ui1_i = 0; ui1_i < t_ml_start_title.ui4_track_num; ui1_i++)
            {
                pt_media_desc->t_crypt_info.u.t_ml_crypt.t_track_drm[ui1_i].ui2_key_idx 
                                                = t_ml_proc_param.aui2_ml_track_sec_slots[ui1_i];
            }
        }
        else
        {
            t_drm_unit.pv_ml_nau_h_drm = pv_ml_nau_h_drm;
            x_marlin_cmd_drm_uninit (&t_drm_unit);
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                          (SVCTX_NTFY_CODE_T)(SVCTX_NTFY_CODE_DRM_STATUS_START + t_ml_start_title.e_drm_status),
                                              ST_UNKNOWN);
            return SVCTXR_FAIL;
        }
    }

    t_drm_unit.pv_ml_nau_h_drm = pv_ml_nau_h_drm;
    x_marlin_cmd_drm_uninit (&t_drm_unit);

    return i4_ret;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_cm_connected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_cm_connected(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    MEDIA_DESCRIBT_T*   pt_media_desc;
    MM_SVC_UOP_HANDLER_FCT_TBL_T t_uop_tbl;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    SIZE_T z_info_len = sizeof(UINT32);
    x_minfo_set_info(pt_media_desc->h_media_info, 
        MINFO_INFO_TYPE_SET_TITLE,
        0,
        &pt_running_svc_req->ui4_title_idx,
        &z_info_len);
    pt_media_desc->ui4_title_idx = pt_running_svc_req->ui4_title_idx;
    
    if(pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS ||
       pt_media_desc->pt_strm_info != NULL)
    {                
        i4_ret = _mm_hdlr_update_scdb_recs( 
                                            pt_running_svc_req->h_media_desc,
                                            pt_running_svc_req->pt_player_act->h_conn);   
        if (i4_ret != SVCTXR_OK)
        {
            if (i4_ret == SVCTXR_NO_RESOURCE)
            {  
                ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));   
            }
            x_dbg_stmt(_ERROR_HEADER" update scdb records failed! i4_ret = %d\n", i4_ret);  
            return i4_ret;
        }        
        else
        {            
            /* simulate SIGNAL_STATUS_LOCKED if PLAYBACK_CTRL_SET_QUEUE_FILE successfully */ 
            pt_svctx->b_signal_loss_notified = FALSE;
            pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;
        }   
    }
    
    /*Open UOP Hander */
    t_uop_tbl = pt_running_svc_req->t_uop_fct_tbl;
    if(t_uop_tbl.pf_open)
    {
        i4_ret = t_uop_tbl.pf_open(pt_media_desc);
        if (i4_ret != SVCTXR_OK)
        {
          /*ignore this failed*/
        }
        else 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CRNT_EVN_READY,
                        ST_UNKNOWN
                        );
        }
    }

    /*notify duration update*/
    if (pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS)
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
                    ST_UNKNOWN
                    );
    }
#ifdef ENABLE_NAUTILUS
    i4_ret = _mm_hdlr_check_mln_crypt_info(pt_svctx,
                                           pt_running_svc_req);
    if(i4_ret != SVCTXR_OK)
    {/* Force Exit*/
        _mm_hdlr_drm_timeout_cb(pt_media_desc->t_drm_ctrl.h_timer,/* No use, null is OK*/
                                (VOID*)pt_svctx);
    }
 #endif

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_sess_in_closing
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_sess_in_closing(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT8            ui1_idx;
    STREAM_TYPE_T    e_strm_type;
    STRM_REQ_T*      pt_strm_req;

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) {

        e_strm_type = e_g_mm_strm_types[ui1_idx];
        if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            break;
        }
        if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL) {
            svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
            pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
        }
        pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;


        if (e_strm_type == ST_AUDIO) {
           svctx_reset_audio_info(pt_svctx);
        }

        if (e_strm_type == ST_VIDEO) {
           svctx_reset_video_info(pt_svctx);
        }
    }
    strm_fmt_chker_reset(pt_running_svc_req->pt_strm_fmt_chker);
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_is_valid_for_strm_select
 * Description
 * Input arguments
 *      pt_svctx         the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_is_valid_for_strm_select(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{

    if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0)
    {
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_block_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = NULL;
    INT32       i4_ret            = SVCTXR_OK;

    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
    strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    if (pt_audio_strm_req != NULL) {
        if (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE) {
            i4_ret = x_sm_close_stream(pt_audio_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return SVCTXR_FAIL;
            }

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSING;
        }

        if (pt_audio_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) {
              pt_audio_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid = MPEG_2_NULL_PID;
        }
    }

    /*pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);
    if (pt_svctx->b_ad_enabled && pt_audio_strm_req != NULL) {
        if (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE) {
            i4_ret = x_sm_close_stream(pt_audio_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return SVCTXR_FAIL;
            }

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSING;
        }

        if (pt_audio_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) {
              pt_audio_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid = MPEG_2_NULL_PID;
        }
    }*/

#if 1
    _mm_hdlr_reload_video_stream_as_blocked(pt_svctx, pt_running_svc_req);

    rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
#else
    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
        rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
        i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    } else {
        SM_SESS_COND_T e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
        if (e_sess_cond == SM_SESS_COND_OPENED
                || e_sess_cond == SM_SESS_COND_STARTING
                || e_sess_cond == SM_SESS_COND_STARTED) {
            i4_ret = player_pause(pt_running_svc_req->pt_player);
            if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
                return i4_ret;
            }
        }
    }
#endif
    pt_svctx->e_state = SVCTX_COND_PENDING;
    if (SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter)) {
        i4_ret = _mm_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_SUBTITLE
                    );
    } else {
        i4_ret = _mm_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
                    );
    }

    /*x_dbg_stmt(_INFO_HEADER"%s(i4_ret=%d)\r\n", __func__, i4_ret);*/

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_check_audio_video_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_check_audio_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    STRM_REQ_T* pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) 
    {
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    pt_video_strm_req
                    );
    }
    
    if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) 
    {
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_audio_strm_req
                    );
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_check_video_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_check_video_stream (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);


    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_VIDEO;
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_video_strm_req
                    );
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        return SVCTXR_OK;
    }

    if (pt_video_strm_req->e_strm_cond == SM_COND_STARTED) {
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_video_strm_req
                    );
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
            if (pt_video_strm_req->t_ca.e_event != SM_EVN_CA_COND_UNKNOWN) {
                mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_video_strm_req
                    );
            }
            mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CP,
                    pt_video_strm_req
                    );
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_unblock_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_unblock_service (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    INT32       i4_ret = SVCTXR_OK;

    /*if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        pt_svctx->b_signal_loss_notified = FALSE;
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS, ST_UNKNOWN);
        return SVCTXR_OK;
    }*/

    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
    strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

    /* Reload video stream */
    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) 
    {
        mm_hdlr_reload_stream(pt_svctx, pt_running_svc_req, ST_VIDEO);
    } 
    else 
    {
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);

        if (svctx_is_valid_video_fmt(pt_svctx) == FALSE) 
        {
            svctx_update_video_info_from_sm(pt_svctx, pt_video_strm_req->h_stream);
        }

        _mm_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
        _mm_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
    }

    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) 
    {
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_VIDEO;
        mm_hdlr_invoke_client_strm_nfy_fct(
                                            pt_svctx,
                                            SVCTX_NTFY_CODE_NO_STREAM,
                                            pt_video_strm_req
                                            );
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    /* Reload audio stream */
    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE
                || pt_audio_strm_req->e_strm_cond != SM_COND_STARTED) 
    {
        mm_hdlr_reload_stream(pt_svctx, pt_running_svc_req, ST_AUDIO);
    }

    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE
                && pt_audio_strm_req->e_strm_cond == SM_COND_CLOSED) 
    {
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_audio_strm_req
                    );
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, 
                                      SVCTX_NTFY_CODE_NORMAL, 
                                      ST_UNKNOWN);
    mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                      SVCTX_NTFY_CODE_CRNT_EVN_READY,
                                      ST_UNKNOWN);
    _mm_hdlr_check_audio_video_fmt(pt_svctx, pt_running_svc_req);

    rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );

    /*x_dbg_stmt(_INFO_HEADER"%s(i4_ret=%d)\r\n", __func__, i4_ret);*/

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_sess_opened
 * Description
 *      The API is used to choose default stream components for each stream type
 *      for a specific service to start. If this API is failed,
 *      the attached application will receive
 *      SVCTX_MSG_NO_AUDIO_STRM, if no audio stream in this channel,
 *      SVCTX_MSG_OPEN_AUDIO_STRM_FAILED, if open audio stream failed,
 *      SVCTX_MSG_NO_VIDEO_STRM, if no video stream in this channel,
 *      SVCTX_MSG_OPEN_VIDEO_STRM_FAILED, if open video stream failed.
 *      SVCTX_MSG_PLAY_SESS_FAILED, if play this session failed
 *      event.
 * Input arguments
 *      pt_svctx         the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_sess_opened(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32                        i4_ret;
    UINT8                        ui1_idx;
    STREAM_TYPE_T                e_strm_type;
    STRM_REQ_T*                  pt_strm_req;
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct;
    VOID*                        pv_strm_fltr_tag;
    BOOL                         b_with_any_strm = FALSE;
    /* SVC_BLOCK_COND_T          e_block_cond; */
    MEDIA_DESCRIBT_T*            pt_media_desc;
    PLAYER_T*                    pt_player = pt_running_svc_req->pt_player_act;


    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if(i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
 
    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)    
    {                
        i4_ret = _mm_hdlr_update_scdb_recs( 
                                            pt_running_svc_req->h_media_desc,
                                            pt_running_svc_req->pt_player->h_conn);   
        if (i4_ret != SVCTXR_OK)
        {
            if (i4_ret == SVCTXR_NO_RESOURCE)
            {  
                ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));   
            }
            x_dbg_stmt(_ERROR_HEADER" update scdb records failed! i4_ret = %d\n", i4_ret);  
            return i4_ret;
        }        
        else
        {            
            /* simulate SIGNAL_STATUS_LOCKED if PLAYBACK_CTRL_SET_QUEUE_FILE successfully */ 
            pt_svctx->b_signal_loss_notified = FALSE;
            pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;
        }   
    }
#ifdef MW_MM_FAKE_DAUL_VID
    if (pt_media_desc->b_multi_track_3d && !pt_media_desc->b_is_thumbnail_playback)
    {
        MM_3D_PROPERTY_T e_3d_property = MM_3D_PROPERTY_DAUL;
        i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_3D_PROPERTY,
                      (VOID*)&e_3d_property,
                      sizeof(MM_3D_PROPERTY_T));
        if (i4_ret != SMR_OK)
        {
           DBG_ERROR((_ERROR_HEADER"SM_SESS_GNRC_SET_TYPE_AB_POSITION fail!\r\n"));
           i4_ret =  SVCTXR_FAIL;
        }
    }
#endif

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++)
    {
        e_strm_type = e_g_mm_strm_types[ui1_idx];
        i4_ret = _mm_hdlr_is_valid_for_strm_select(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req
                    );
        if (i4_ret == SVCTXR_FAIL) 
        {
            continue;
        }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
         /* WILLY_BAD_INTLVE*/ 
        if (pt_running_svc_req->pt_player_enhance != NULL)
        {
             if (pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream == NULL_HANDLE && e_strm_type == ST_VIDEO)
             {
                  ui1_idx = STRM_TYPE_COUNT;
             }
             else if (pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream == NULL_HANDLE && e_strm_type == ST_AUDIO)
             {
                  ui1_idx = STRM_TYPE_COUNT;
             }
        }
#endif
        /*boundary checking*/
        if(e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            break;
        }
        pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        if (x_handle_valid(pt_strm_req->h_stream) == TRUE) 
        {
            pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
            pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
            b_with_any_strm = TRUE;
            continue;
        }

        pt_strm_req->h_svctx         =  pt_running_svc_req->t_parent.h_svctx;
        pt_strm_req->h_stream        = NULL_HANDLE;
        pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;

        i4_ret = SVCTXR_NOT_EXIST;

        if (pt_strm_req->e_req_type == STRM_REQ_SCDB)
        {
          
                mm_hdlr_choose_strm_filter(
                                pt_svctx,
                                e_strm_type,
                                &pt_strm_fltr_fct,
                                &pv_strm_fltr_tag
                                );
                i4_ret = svctx_get_strm_comp_id(
                                pt_svctx,
                                e_strm_type,
                                FALSE,
                                pt_strm_fltr_fct,
                                pv_strm_fltr_tag,
                                &(pt_strm_req->u.t_scdb.t_comp_id),
                                &(pt_strm_req->u.t_scdb.t_rec)
                                );

            if (i4_ret == SVCTXR_NOT_EXIST)
            {
                if(e_strm_type == ST_SUBTITLE && (
                   pt_media_desc->b_is_lyric /* ||
                   pt_media_desc->b_is_ext_first*/ /*look external subtt as internal*/ ||
                   /*for show subtitle when replay the same file after FB to head*/
                   pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag != NULL))
                {
                    pt_strm_req->e_req_type = STRM_REQ_SCDB;
                    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_SUBTITLE;
                    i4_ret = mm_hdlr_open_strm(pt_svctx, pt_strm_req, FALSE);
                    if (i4_ret != SVCTXR_OK)
                    {
                        return i4_ret;
                    }
                }
                else if (e_strm_type == ST_CLOSED_CAPTION) 
                {
                    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
                    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
                    i4_ret = SVCTXR_OK;
                }
                continue;
            }
            if (i4_ret == SVCTXR_DB_MODIFIED) 
            {
                /* scdb modified, search again! */
                ui1_idx = 0;
                continue;
            }

            if (i4_ret != SVCTXR_OK) 
            {
                mm_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_INTERNAL_ERROR,
                            e_strm_type
                            );
                return SVCTXR_FAIL;
            }
        } 
        else if (pt_strm_req->e_req_type == STRM_REQ_PID) 
        {
            /* by-pass to direct invoke ch_hdlr_open_strm */
        } 
        else 
        {
            continue;
        }

        i4_ret = mm_hdlr_open_strm(pt_svctx, pt_strm_req, FALSE);
        if (i4_ret != SVCTXR_OK) 
        {
            x_dbg_stmt(_ERROR_HEADER"mm_hdlr_open_strm() fail! i4_ret = %d line = %d\n", i4_ret, __LINE__);
            return i4_ret;
        }
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID)))  
       /*Check 3D Second Track*/
        if (pt_media_desc->b_multi_track_3d && e_strm_type == ST_VIDEO &&
             !pt_media_desc->b_is_thumbnail_playback)
        {
               pt_strm_fltr_fct  = _mm_hdlr_3d_multi_video_track_filter;
               pv_strm_fltr_tag   = (VOID*)pt_svctx;
               i4_ret = svctx_get_strm_comp_id(
                                pt_svctx,
                                e_strm_type,
                                FALSE,
                                pt_strm_fltr_fct,
                                pv_strm_fltr_tag,
                                &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_comp_id),
                                &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_rec)
                                );
/*               _scc_comp_vid_get_vid_dec_cap( pt_vid_comp );*/
              i4_ret = mm_hdlr_open_strm(pt_svctx, &(pt_running_svc_req->t_strm_reqs_3d_vid), FALSE);    
        }
#endif        
        b_with_any_strm = TRUE;
    }

    if (b_with_any_strm == FALSE) 
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_NO_ANY_STREAM,
                        ST_UNKNOWN
                        );
    }

    if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
    {
        if(NULL == pt_media_desc->pt_strm_info)
        {
            /*enable BLOCK_PSI mode in demux*/
            i4_ret = x_sm_set(pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_BLOCK_PSI,
                          (void*)(UINT32)TRUE,
                          sizeof(BOOL));

             if (i4_ret!= SMR_OK)
             {
                 DBG_ERROR((_ERROR_HEADER"block PSI fail!\n"));
                 return SVCTXR_FAIL;
             }
        }
    }
    /*play until index table buildied*/
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_sess_started
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_sess_started(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32               i4_ret;
    MEDIA_DESCRIBT_T*   pt_media_desc;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    /* notify AP here for non-TS file */
    if (pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS &&
        (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream)||
        x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream)))
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_NORMAL,
                ST_UNKNOWN
                );
    }
    else if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)&&
            (pt_media_desc->pt_strm_info))
    {
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NORMAL,
                    ST_UNKNOWN
                    );
    }
#ifdef TIME_SHIFT_SUPPORT
    else if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        pt_svctx->e_state = SVCTX_COND_PRESENTING;
    }
#endif

    if (pt_media_desc->e_fill_scdb_mode != MM_SVC_FILL_SCDB_APPEND)
    {
        x_dbg_stmt("_mm_hdlr_handle_sess_started() check stream status \n");

        if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
           (pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI)  && 
           (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
        {
            if(x_handle_valid(pt_running_svc_req->h_chk_strm_timer))
            {
                x_dbg_stmt("[MM_hdlr]:Start h_chk_strm_timer for network uri ts strm \r\n");
                pt_media_desc->b_uri_ts_strm_chk = TRUE;
                i4_ret = x_timer_start(pt_running_svc_req->h_chk_strm_timer,                                            
                                           MW_HDLR_CHK_STRM_DELAY*4, /*delay 32s for network uri ts*/
                                           X_TIMER_FLAG_ONCE,
                                           _mm_hdlr_chk_strm_timeout_nfy,
                                           (VOID*)pt_svctx);
                if ( i4_ret != OSR_OK)
                {
                    x_dbg_stmt("[MM_hdlr]:Start h_chk_strm_timer failed! ret=%d\r\n", i4_ret);
                    //return  SVCTXR_FAIL;
                }
            }
        }
        else
        {
            mm_hdlr_check_strms_status(pt_svctx);
        }
    }

#ifdef DIVX_PLUS_CER
    if(pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap != NULL &&
       pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap != NULL)
    {
        MM_SVC_CUR_CHAP_ID_T t_chap_id;
        MINFO_CHAP_INFO_T t_chap;
        MINFO_CHAP_INFO_T t_chap_next;
        
#ifdef MM_LAST_MEMORY_SUPPORT
        t_chap_id.ui4_chapter_id = pt_media_desc->t_mm_last_info.ui4_chap_idx;
        t_chap_id.ui4_playlist_id = pt_media_desc->t_mm_last_info.ui4_playlist_idx;
#else
        t_chap_id.ui4_chapter_id = pt_running_svc_req->ui4_chap_idx;
        t_chap_id.ui4_playlist_id = pt_running_svc_req->ui4_playlist_idx;
#endif     
        i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap);
        if(i4_ret == SVCTXR_OK)
        {
            VSH_CHAP_INFO_T t_set_info = {0};/*wqyin*/
            HANDLE_T        h_stream = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
            
            if(t_chap.ui4_playlist_idx != t_chap_id.ui4_playlist_id ||
               t_chap.ui4_chap_idx != t_chap_id.ui4_chapter_id)
            {
                i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_set_cur_chap(pt_media_desc, FALSE, &t_chap_id);
                if(i4_ret == SVCTXR_OK)
                {
                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap);
                    if(i4_ret == SVCTXR_OK)
                    {
                        MM_SVC_POS_INFO_T t_pos_info;

                        if (pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.ui8_pos_val == 0 &&
                            pt_media_desc->t_mm_last_info.ui4_playlist_idx != 0)
                        {
                            if(x_handle_valid(h_stream))
                            {
                                t_set_info.b_seek = FALSE;
                                t_set_info.b_first = TRUE;
                                t_set_info.ui8_start_time = 0;
                                t_set_info.ui8_end_time = (UINT64)(-1);
                                i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                            }
                            t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
                            t_pos_info.ui8_pos_val = t_chap.ui8_start_time;
                            i4_ret = mm_hdlr_set(
                                            pt_svctx,
                                            SVCTX_MM_SET_TYPE_SEEK,
                                            (VOID*)&t_pos_info,
                                            sizeof(MM_SVC_POS_INFO_T)
                                            );
                            if(SVCTXR_OK == i4_ret)
                            {
                                pt_media_desc->b_is_seeking = TRUE;
                            }
                        }
                        if(SVCTXR_OK == i4_ret)
                        {
                            if(x_handle_valid(h_stream))
                            {
                                if (pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE)/*it's done of FF*/
                                {
                                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 1, &t_chap_next);
                                }
                                else
                                {
                                    i4_ret = pt_running_svc_req->t_uop_fct_tbl.pf_get_cur_chap(pt_media_desc, 2, &t_chap_next);
                                }
                                 if(i4_ret == SVCTXR_OK && 
                                     ((pt_media_desc->e_speed > MM_SPEED_TYPE_STEP_REVERSE &&
                                (t_chap_next.ui8_start_time > t_chap.ui8_end_time + 1 ||
                                t_chap_next.ui8_start_time < t_chap.ui8_start_time)) ||
                                (pt_media_desc->e_speed <= MM_SPEED_TYPE_STEP_REVERSE &&
                                (t_chap.ui8_start_time > t_chap_next.ui8_end_time + 1 ||
                                t_chap.ui8_start_time < t_chap_next.ui8_start_time))))
                                {
                                    t_set_info.b_seek = TRUE;
                                }
                                else
                                {
                                    t_set_info.b_seek = FALSE;
                                }
                                t_set_info.b_first = TRUE;
                                t_set_info.ui8_start_time = t_chap.ui8_start_time;
                                t_set_info.ui8_end_time = t_chap.ui8_end_time;
                                i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                            }
                            mm_hdlr_invoke_client_svc_nfy_fct(
                                        pt_svctx,
                                        SVCTX_NTFY_CODE_CHAPTER_CHANGE,
                                        ST_VIDEO
                                        );
                            mm_hdlr_invoke_client_svc_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
                                    ST_UNKNOWN
                                    );
                        }
                    }
                }
            }
            else
            {
                if(x_handle_valid(h_stream))
                {
                    t_set_info.b_first = TRUE;
                    t_set_info.ui8_start_time = t_chap.ui8_start_time;
                    t_set_info.ui8_end_time = t_chap.ui8_end_time;
                    i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_set_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                }
            }
        }
    }
#endif
#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
        pt_media_desc->h_rec_util != NULL_HANDLE)
    {
        MM_BUF_RANGE_INFO_T t_valid_range = {0};
        SVCTX_REC_UTIL_MSG_T t_rec_util_msg = {0};

        i4_ret = rec_util_get_valid_range(pt_media_desc->h_rec_util,
                                          &t_valid_range);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_valid_range() failed, ret=%d\n", i4_ret));
        }
        else if (t_valid_range.ui4_start != t_valid_range.ui4_end)
        {
            t_rec_util_msg.h_obj = pt_media_desc->h_rec_util;
            t_rec_util_msg.e_code = REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED;
            t_rec_util_msg.uinfo.t_range.b_info_updated = FALSE;
            t_rec_util_msg.uinfo.t_range.t_valid_range = t_valid_range;
            _mm_hdlr_handle_rec_util_msg(pt_svctx, &t_rec_util_msg);
            x_dbg_stmt(_INFO_HEADER"%s(),tshift 1st valid range (%u/%u) updated.\n",
                       __func__, t_valid_range.ui4_start, t_valid_range.ui4_end);
        }
    }
#endif /* TIME_SHIFT_SUPPORT */

#ifdef ENABLE_NAUTILUS
    if(pt_media_desc->t_drm_ctrl.ui4_max_view_time != 0)
    {
        x_timer_create(&(pt_media_desc->t_drm_ctrl.h_timer));
        x_timer_start(pt_media_desc->t_drm_ctrl.h_timer,
                      pt_media_desc->t_drm_ctrl.ui4_max_view_time,
                      X_TIMER_FLAG_ONCE,
                      _mm_hdlr_drm_timeout_cb,
                      (VOID*)pt_svctx);
    }
#endif
    TMS_END("SELECT_SVC_2_SESS_STARTED");

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_strm_select_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_select_req (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_strm_req,
                    BOOL                        b_need_to_free_req
                    )
{
    INT32          i4_ret      = SVCTXR_OK;
    STRM_REQ_T*    pt_strm_req = NULL;
    STRM_REQ_T**   ppt_pending_strm_req = NULL;
    SM_SESS_COND_T e_sess_cond = SM_SESS_COND_CLOSED;
    STREAM_TYPE_T  e_strm_type = svctx_get_strm_req_strm_type(pt_new_strm_req);
    MEDIA_DESCRIBT_T* pt_media_desc = NULL;
    BOOL              b_auto_play;

    if (e_strm_type >= STRM_TYPE_COUNT)
    {
        return SVCTXR_INV_ARG;
    }

    /* should set this ui4_stream_mode before any check to retain
     * the stream mode setting.
     */
    pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);

    if (!(e_strm_type == ST_CLOSED_CAPTION
        && SVCTX_IS_INVISIBLE_CC_FILTER(pt_new_strm_req->t_cc_filter)))
    {
        /* Signal unlock or channel block but require to open a new non-video stream
         * will skip the request */
        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED ||
            ((pt_svctx->b_is_blocked == TRUE) &&
             (e_strm_type!=ST_VIDEO)) )
        {
            if (b_need_to_free_req == TRUE)
            {
                svctx_free_strm_req(pt_new_strm_req);
            }
            return SVCTXR_WRONG_STATE;
        }
    }

    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
    if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) 
    {
        if (b_need_to_free_req == TRUE) 
        {
            svctx_free_strm_req(pt_new_strm_req);
        }
        return SVCTXR_WRONG_STATE;
    }

    if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
    {
        ASSERT(0, ("%s(), over the array bound\r\n", __func__));
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);

    switch(e_strm_type){
    case ST_CLOSED_CAPTION:
        if(pt_new_strm_req->h_gl_plane != NULL_HANDLE)
        {
            pt_running_svc_req->h_gl_plane  = pt_new_strm_req->h_gl_plane;
            pt_running_svc_req->t_cc_filter = pt_new_strm_req->t_cc_filter;
        }
        break;
    case ST_SUBTITLE:
        /*work round to fix conflic between MM Subtitle and TS Subtitle*/
        if(pt_new_strm_req->h_gl_plane != NULL_HANDLE)
        {
            pt_running_svc_req->h_gl_plane  = pt_new_strm_req->h_gl_plane;
        }
        break;
    case ST_AUDIO:
        if (!pt_new_strm_req->b_aux_audio)
        {
            mm_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
        }
        break;
    case ST_VIDEO:
        mm_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
        _mm_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
                    );
        break;
    default:
        break;
    }

    if (!(e_strm_type == ST_CLOSED_CAPTION
            && SVCTX_IS_INVISIBLE_CC_FILTER(pt_new_strm_req->t_cc_filter))) 
    {
        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(e_strm_type)) > 0) 
        {
            if (b_need_to_free_req == TRUE) 
            {
                svctx_free_strm_req(pt_new_strm_req);
            }
            return SVCTXR_FROZEN;
        }
    }

    pt_new_strm_req->pt_strm_fltr_fct = pt_strm_req->pt_strm_fltr_fct;
    pt_new_strm_req->pv_strm_fltr_tag = pt_strm_req->pv_strm_fltr_tag;

    switch(pt_strm_req->e_strm_cond) {
    case SM_COND_CLOSED:
        b_auto_play = e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE;
        if (e_strm_type == ST_AUDIO || e_strm_type == ST_SUBTITLE)
        {
            if (pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X)
            {
                b_auto_play = FALSE;
            }
        }
        *pt_strm_req = *pt_new_strm_req;
        i4_ret = mm_hdlr_open_strm(
                                    pt_svctx,
                                    pt_strm_req,
                                    b_auto_play
                                    );
        if (i4_ret == SVCTXR_OK) 
        {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            if(!b_auto_play && e_strm_type == ST_AUDIO)
            {
                /*it equal to set audio stop*/
                pt_media_desc->b_set_audio_stop = TRUE;
            }

        } 
        else 
        {
            pt_strm_req->e_req_type                = STRM_REQ_SCDB;
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        }
        if (b_need_to_free_req == TRUE) 
        {
            svctx_free_strm_req(pt_new_strm_req);
        }
        break;
    case SM_COND_OPENED:
    case SM_COND_STARTED:
#ifdef TIME_SHIFT_SUPPORT
        if ((MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
            && (e_strm_type == ST_VIDEO))
        {
            if (pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X)
            {
                pt_media_desc->b_trick_mode_revive = TRUE;
            }
        }
#endif

        if (*ppt_pending_strm_req!= NULL)
        {
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
        }

        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING)
        {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            pt_strm_req->e_strm_cond      = SM_COND_CLOSING;
            *ppt_pending_strm_req = pt_new_strm_req;
            i4_ret = SVCTXR_OK;
        }
        else if (i4_ret == SMR_INV_HANDLE)
        {
            *pt_strm_req             = *pt_new_strm_req;
            pt_strm_req->e_strm_cond = SM_COND_CLOSED;
            i4_ret = mm_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE
                        );
            if (i4_ret == SVCTXR_OK)
            {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
                if(e_sess_cond == SM_SESS_COND_OPENED && e_strm_type == ST_AUDIO)
                {
                    /*it equal to set audio stop*/
                    pt_media_desc->b_set_audio_stop = TRUE;
                }

            }
            else
            {
                pt_strm_req->e_req_type                = STRM_REQ_SCDB;
                pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
                pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
            }
            if (b_need_to_free_req == TRUE)
            {
                svctx_free_strm_req(pt_new_strm_req);
            }
        }
        else
        {
            if (b_need_to_free_req == TRUE)
            {
                svctx_free_strm_req(pt_new_strm_req);
            }
            i4_ret = SVCTXR_FAIL;
        }
        break;
    default:
        if (*ppt_pending_strm_req != NULL)
        {
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
        }

        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        *ppt_pending_strm_req = pt_new_strm_req;
        break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_new_strm_req_as_reselecting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_create_new_strm_req_as_reselecting(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    MM_SVC_REQ_T*                pt_running_svc_req,
                    STRM_REQ_T*                  pt_old_strm_req,
                    STRM_REQ_T**                 ppt_new_strm_req
                    )
{
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct = NULL;
    VOID*                        pv_strm_fltr_tag = NULL;
    INT32                        i4_ret           = SVCTXR_NOT_EXIST;
    STRM_REQ_T*                  pt_new_strm_req  = NULL;
    SVCTX_MSG_T                  t_stop_msg       = {0};

    pt_new_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
    if (pt_new_strm_req == NULL) {
        return SVCTXR_NO_RESOURCE;
    }
    /*Add for playing ts&pvr subtitle*/
    if((e_strm_type == ST_SUBTITLE) && 
        ((pt_old_strm_req->pt_strm_fltr_fct == NULL) || (pt_old_strm_req->pv_strm_fltr_tag == NULL)))
    {
        pt_old_strm_req->pt_strm_fltr_fct = _mm_hdlr_default_strm_comp_filter;
        pt_old_strm_req->pv_strm_fltr_tag = (VOID*)pt_svctx;;
    }

    pt_new_strm_req->e_req_type = STRM_REQ_SCDB;
    pt_new_strm_req->pt_strm_fltr_fct = pt_old_strm_req->pt_strm_fltr_fct;
    pt_new_strm_req->pv_strm_fltr_tag = pt_old_strm_req->pv_strm_fltr_tag;
#if 0
    if (pt_old_strm_req->e_req_type == STRM_REQ_SCDB
            && svctx_get_strm_req_strm_type(pt_old_strm_req) == e_strm_type
            && e_strm_type != ST_CLOSED_CAPTION)
    {
        i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    e_strm_type,
                    FALSE,
                    _mm_hdlr_strm_comp_pid_filter,
                    &(pt_old_strm_req->u.t_scdb.t_rec),
                    &(pt_new_strm_req->u.t_scdb.t_comp_id),
                    &(pt_new_strm_req->u.t_scdb.t_rec)
                    );
    }

    if (i4_ret != SVCTXR_OK)
#endif
    {
#ifdef MW_MM_DUAL_VID_SUPPORT 
        if(e_strm_type == ST_VIDEO)
        {/*For TS case, AP filter only filter out the first one, but 3D TS need filter the non-mvc one*/
            pt_strm_fltr_fct  = _mm_hdlr_default_strm_comp_filter;
            pv_strm_fltr_tag   = (VOID*)pt_svctx;
        }
        else
#endif 
        {
            /* get new matched SCDB record */
            mm_hdlr_choose_strm_filter(
                            pt_svctx,
                            e_strm_type,
                            &pt_strm_fltr_fct,
                            &pv_strm_fltr_tag
                            );
        }

        i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    e_strm_type,
                    TRUE,
                    pt_strm_fltr_fct,
                    pv_strm_fltr_tag,
                    &pt_new_strm_req->u.t_scdb.t_comp_id,
                    &pt_new_strm_req->u.t_scdb.t_rec
                    );
    }

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_new_strm_req);
        pt_new_strm_req = NULL;

        switch (e_strm_type){
        case ST_CLOSED_CAPTION:
            /* if no related SCDB record, it is forced to select the
             * specific cc stream by client, so don't stop it.
             */
            break;
        default:
            t_stop_msg.h_svctx                       = pt_old_strm_req->h_svctx;
            t_stop_msg.ui1_msg_id                    = _SVCTX_MSG_STOP_STRM_REQ;
            t_stop_msg.u.t_stop_strm_req.e_type      = e_strm_type;
            t_stop_msg.u.t_stop_strm_req.b_aux_audio = FALSE;

            mm_hdlr_stop_strm(pt_svctx, &t_stop_msg);
            pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);
            break;
        }
    }

    *ppt_new_strm_req = pt_new_strm_req;
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_reselect_strms
 * Description
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_reselect_strms(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32            i4_ret;
    UINT8            ui1_idx;
    STREAM_TYPE_T    e_strm_type;
    STRM_REQ_T*      pt_old_strm_req  = NULL;
    STRM_REQ_T*      pt_new_strm_req  = NULL;
    MM_SVC_REQ_T*    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T         h_scdb;
    SM_SESS_COND_T   e_sess_cond;
    SVCTX_MSG_T      t_msg;
    MEDIA_DESCRIBT_T*   pt_media_desc;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

#ifndef DRV_NO_PRE_PARSING
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }
#endif
    h_scdb       = player_get_scdb_handle(pt_running_svc_req->pt_player);
    e_sess_cond  = player_get_sess_cond(pt_running_svc_req->pt_player);

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }
    
    /*svctx_scdb_chk_timer_stop(pt_svctx);*/
    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++)
    {
        e_strm_type = e_g_mm_strm_types[ui1_idx];
        i4_ret = _mm_hdlr_is_valid_for_strm_select(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req
                    );
        if (i4_ret == SVCTXR_FAIL)
        {
            continue;
        }
        if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
        {
            ASSERT(0, ("%s(), over the array bound\r\n", __func__));
        }
        pt_old_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

        if ((pt_old_strm_req->e_req_type != STRM_REQ_SCDB)
            && (pt_old_strm_req->e_req_type != STRM_REQ_PID))
        {
            continue;
        }
        if (pt_old_strm_req->u.t_scdb.t_comp_id.e_type == ST_CLOSED_CAPTION)
        {
            if (pt_old_strm_req->t_cc_filter.e_cc_type == SM_CC_TYPE_AUTO_SEL)
            {
                continue;
            }
        }
        /*if (x_handle_valid(pt_old_strm_req->h_stream) == TRUE)//removed base ch_hdlr
        {
            continue;
        }*/

        i4_ret = _mm_hdlr_check_strm_scdb_record(
                    h_scdb,
                    e_strm_type,
                    pt_old_strm_req
                    );
        if (i4_ret != SVCTXR_NO_RECORD)
        {
            continue;
        }

        /* only process the following logic when SCDB record is absent */
        i4_ret =_mm_hdlr_create_new_strm_req_as_reselecting(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req,
                    pt_old_strm_req,
                    &pt_new_strm_req
                    );
        if (i4_ret != SVCTXR_OK)
        {
            continue;
        }

        pt_new_strm_req->h_svctx  = pt_running_svc_req->t_parent.h_svctx;
        pt_new_strm_req->h_stream = NULL_HANDLE;
        pt_new_strm_req->h_gl_plane = pt_running_svc_req->h_gl_plane;

        pt_new_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_new_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;

        switch (e_sess_cond)
        {
        case SM_SESS_COND_OPENED:
            if (e_strm_type == ST_VIDEO)
            {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            *pt_old_strm_req = *pt_new_strm_req;
            svctx_free_strm_req(pt_new_strm_req);
            /* reslect stream when SM_SESS_COND_OPENED */
            i4_ret = mm_hdlr_open_strm(
                            pt_svctx,
                            pt_old_strm_req,
                            FALSE
                            );
            break;
        case SM_SESS_COND_STARTING:
        case SM_SESS_COND_STARTED:
            if (e_strm_type == ST_VIDEO)
            {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            pt_new_strm_req->h_svctx         = pt_old_strm_req->h_svctx;
            pt_new_strm_req->pf_strm_nfy     = pt_old_strm_req->pf_strm_nfy;
            pt_new_strm_req->pv_strm_nfy_tag = pt_old_strm_req->pv_strm_nfy_tag;

            t_msg.h_svctx                    = pt_old_strm_req->h_svctx;
            t_msg.ui1_msg_id                 = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req              = pt_new_strm_req;

            i4_ret = mm_hdlr_select_strm(pt_svctx, &t_msg);
            break;
        default:
            svctx_free_strm_req(pt_new_strm_req);
            break;
        }
    }

    if ((e_sess_cond != SM_SESS_COND_OPENED) || (pt_media_desc->ui1_presnt_range_cnts == 0))
    {/*If current hasn't set range, no need to play*/
        return SVCTXR_OK;
    }

    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    pt_media_desc->b_set_audio_stop = FALSE;
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_seek_to_ts_file_start
 * Description
 *      The API is used to open a pipe.
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *---------------------------------------------------------------------------*/
 static INT32 _mm_hdlr_seek_to_ts_file_start(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req)
{
    INT32               i4_ret = SVCTXR_OK;
    MEDIA_DESCRIBT_T*   pt_media_desc;
    MM_RANGE_POS_INFO_T t_position_info;
    MM_PLAY_MODE_PL_T  t_mm_play_mode;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }
    
    /* seek to the start position */
    t_mm_play_mode.eSpeed=MM_SPEED_TYPE_PAUSE;
    t_mm_play_mode.ePlayMode=MM_PLAY_MODE_TYPE_NORMAL;
    i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_SPEED,
                      (VOID*)&t_mm_play_mode,
                      sizeof(MM_PLAY_MODE_PL_T));
    if (i4_ret != SMR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"set speed pause fail!\n");
        i4_ret =  SVCTXR_FAIL;
    }

#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        t_position_info.e_pos_type = MM_RANGE_POS_TYPE_TIME_MEDIA;
        t_position_info.ui4_idx = pt_running_svc_req->t_init_pos_info.ui2_idx;

        if (pt_running_svc_req->t_init_pos_info.ui8_pos_val <= (UINT64)pt_media_desc->ui4_tick_num_safe_start)
        {
            t_position_info.ui8_pos_val = (UINT64)pt_media_desc->ui4_tick_num_safe_start;
        }
        else if (pt_running_svc_req->t_init_pos_info.ui8_pos_val >= (UINT64)pt_media_desc->ui4_tick_num_safe_end)
        {
            t_position_info.ui8_pos_val = (UINT64)pt_media_desc->ui4_tick_num_safe_end;
        }
        else
        {
            t_position_info.ui8_pos_val = pt_running_svc_req->t_init_pos_info.ui8_pos_val;
        }

        t_position_info.ui8_pos_val = (UINT64)PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk,
                                                                           (UINT32)t_position_info.ui8_pos_val);

        x_dbg_stmt(_INFO_HEADER"tshift 1st seek(#%u)\r\n", (UINT32)t_position_info.ui8_pos_val);
    }
    else
#endif
    {
#ifdef MM_LAST_MEMORY_SUPPORT
    if(!pt_media_desc->b_is_parse_pmt && pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_frame_position != 0)
    {
        t_position_info.e_pos_type = MM_RANGE_POS_TYPE_OFFSET_RANGE_START;
        t_position_info.t_start_pos.ui8_stc                    = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_stc;
        t_position_info.t_start_pos.ui8_aud_pts_info           = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_aud_pts_info;
        t_position_info.t_start_pos.ui8_aud_frame_position     = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_aud_frame_position;
        t_position_info.t_start_pos.ui8_i_pts_info             = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_i_pts_info;
        t_position_info.t_start_pos.ui8_pts_info               = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_pts_info;
        t_position_info.t_start_pos.ui8_i_frame_position       = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_i_frame_position;
        t_position_info.t_start_pos.ui8_frame_position         = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_frame_position;
        t_position_info.t_start_pos.ui8_frame_position_disp    = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui8_frame_position_disp;
        t_position_info.t_start_pos.i4_temporal_reference      = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.i4_temporal_reference;
        t_position_info.t_start_pos.ui2_decoding_order         = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui2_decoding_order;
        t_position_info.t_start_pos.ui4_timestap               = pt_media_desc->t_mm_last_info.t_mm_svc_pos_info.u.render_info.ui4_timestap;
    }
    else
    {
        t_position_info.e_pos_type = MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR;
    }
#else
        t_position_info.e_pos_type = MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR;
#endif
        t_position_info.ui4_idx = 0;
        t_position_info.ui8_pos_val = 0;
    }
    i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_SEEK,
                      (VOID*)(((MM_RANGE_POS_INFO_T*)&t_position_info)),
                      sizeof(MM_RANGE_POS_INFO_T));
    if (i4_ret != SMR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"set seek fail!\n");
        i4_ret =  SVCTXR_FAIL;
    }

#ifndef MM_LAST_MEMORY_SUPPORT
    i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_STC,
                      (VOID*)(UINT32)0,
                      sizeof(STC_T));
    if (i4_ret != SMR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"set stc fail!\n");
        i4_ret =  SVCTXR_FAIL;
    }
#endif
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_scdb_updated
 * Description
 *      The API is used to open a pipe.
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *---------------------------------------------------------------------------*/
 static INT32 _mm_hdlr_handle_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    UINT32                      ui4_reason
                    )
{
    INT32               i4_ret = SVCTXR_OK;
    SM_SESS_COND_T      e_sess_cond;
    MEDIA_DESCRIBT_T*   pt_media_desc;
    UINT16              ui2_vid_aud_nums = 0;
    UINT16              ui2_vid_nums = 0;
#ifdef MW_MM_DUAL_VID_SUPPORT
    x_svctx_strm_comp_filter_fct             pt_strm_fltr_fct;
    VOID*                                    pv_strm_fltr_tag;
#endif
    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);

    if (pt_svctx->b_is_blocked == TRUE)
    {
        _mm_hdlr_reload_video_stream_as_blocked(pt_svctx, pt_running_svc_req);
        return SVCTXR_OK;
    }

    if (ui4_reason == SCDB_REASON_UNK)
    {
        return i4_ret;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    if (e_sess_cond != SM_SESS_COND_OPENED
            && e_sess_cond != SM_SESS_COND_STARTING
            && e_sess_cond != SM_SESS_COND_STARTED)
    {
        return SVCTXR_OK;
    }

    if ((ui4_reason & SCDB_RECORD_DEL) > 0)
    {
        x_dbg_stmt(_INFO_HEADER"%s(), SCDB_RECORD_DEL\r\n", __func__);
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_DEL,
                        ST_UNKNOWN
                        );
    }

    if ((ui4_reason & SCDB_RECORD_ADD) > 0)
    {
        x_dbg_stmt(_INFO_HEADER"%s(), SCDB_RECORD_ADD\r\n", __func__);
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_ADD,
                        ST_UNKNOWN
                        );

        if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)&&
            (NULL == pt_media_desc->pt_strm_info))
        {
            if (!pt_media_desc->b_is_scdb_updated)
            {
#if 1
                /*
                   Gray [2009/08/14]:

                   We need to further check any scdb record of selected stream
                   mode exists or NOT.
                   - If YES, we could seek to the start postion.
                   - If NO, we need to continue the parsing.
                */

                HANDLE_T               h_scdb;
                STREAM_TYPE_T          e_strm_type;
                UINT8                  ui1_i;
                UINT16                 ui2_num_recs = 0;
                UINT32                 ui4_db_version = SCDB_NULL_VER_ID;

                h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);

                for (ui1_i = 0; ui1_i < (UINT8)STRM_TYPE_COUNT ; ui1_i++)
                {
                    e_strm_type = e_g_mm_strm_types[ui1_i];

                    if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0)
                    {
                        continue;
                    }
                    i4_ret = x_scdb_get_num_recs(
                                 h_scdb,
                                 e_strm_type,
                                 &ui2_num_recs,
                                 &ui4_db_version
                                 );
                    if (i4_ret != SCDBR_OK)
                    {
                        x_dbg_stmt(_ERROR_HEADER"get number of scdb records fail! line = %d\n",__LINE__);
                        i4_ret =  SVCTXR_FAIL;
                    }
                    if (e_strm_type == ST_VIDEO)
                    {
                        ui2_vid_nums = ui2_num_recs;
                    }

                    if(e_strm_type == ST_AUDIO || e_strm_type == ST_VIDEO)
                    {
                        ui2_vid_aud_nums += ui2_num_recs;
                    }
                    
                    /* find scdb records of selected stream mode, escape this loop */
                    if (ui2_vid_nums > 0)
                    {
                        break;
                    }
                }
#endif
                /* only seek to the start postion when scdb record of selected stream
                   mode exists */
                if (ui2_vid_aud_nums > 0 && (pt_media_desc->e_fill_scdb_mode != MM_SVC_FILL_SCDB_APPEND))
                {
                    i4_ret = _mm_hdlr_seek_to_ts_file_start(pt_svctx,
                                                            pt_running_svc_req);
                }
            }
        }
    }

    if ((ui4_reason & SCDB_RECORD_MOD) > 0)
    {
        x_dbg_stmt(_INFO_HEADER"%s(), SCDB_RECORD_MOD\r\n", __func__);
        mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_MODIFY,
                        ST_UNKNOWN
                        );
    }

#ifdef TIME_SHIFT_SUPPORT
    /* when in TRICK mode => use force PID instead of reselect stream here */
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt)
        && ((pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X)
             || (pt_media_desc->e_speed >= MM_SPEED_TYPE_FORWARD_2X)))
    {
        /* do nothing */
        ;
    }
    else
    {
#endif
        /*general format file can also receive scdb updated*/
        if ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            && (NULL == pt_media_desc->pt_strm_info))
        {
            if ((ui4_reason & SCDB_RECORD_ADD) > 0) 
            {   
                /* Just for the case that the differece PMT distance is far.
                               1. open stream when time out.
                               2. open stream if 2 or more video updated.
                            */
                if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
                   (pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI)  && 
                   (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
                {
                    x_dbg_stmt("[MM_hdlr]:Stop uri network ts strm chk timer\r\n");
                    if(x_handle_valid(pt_running_svc_req->h_chk_strm_timer))
                    {
                        x_timer_stop(pt_running_svc_req->h_chk_strm_timer);
                    }
                    pt_media_desc->b_uri_ts_strm_chk = FALSE;
                }
                
                if (pt_media_desc->e_fill_scdb_mode == MM_SVC_FILL_SCDB_APPEND)
                {
                    UINT32    ui4_is_hd_mvc = 1;
                    if (!pt_media_desc->b_chk_strm_timer_started && ui2_vid_nums > 0)
                    {
                        i4_ret = x_timer_start(pt_running_svc_req->h_chk_strm_timer,                                            
                                           MW_HDLR_CHK_STRM_DELAY,
                                           X_TIMER_FLAG_ONCE,
                                           _mm_hdlr_chk_strm_timeout_nfy,
                                           (VOID*)pt_svctx);
                        if ( i4_ret != OSR_OK)
                        {
                            DBG_ERROR(("[MM_hdlr]:Start h_chk_strm_timer failed! ret=%d\r\n", i4_ret));
                            i4_ret = _mm_hdlr_reselect_strms(pt_svctx);
                            return  SVCTXR_FAIL;
                        }
                        pt_media_desc->b_chk_strm_timer_started = TRUE;
                    }
                    if(!pt_media_desc->b_chk_strm_timer_out && ui2_vid_nums > 1)
                    {
                        x_timer_stop(pt_running_svc_req->h_chk_strm_timer);
                        _mm_hdlr_chk_strm_timeout_nfy(pt_running_svc_req->h_chk_strm_timer, (VOID*)pt_svctx);
                    }
                    i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                      SM_SESS_GNRC_SET_TYPE_HDMVC_INFO,
                                      (VOID*)(ui4_is_hd_mvc),
                                      sizeof(UINT32));
                    if (i4_ret != SMR_OK)
                    {
                        x_dbg_stmt(_ERROR_HEADER"x_sm_set(%u) failed, ret = %d)\r\n", SM_SESS_GNRC_SET_TYPE_HDMVC_INFO, i4_ret);
                    }
                }
                else
                {
#ifdef MW_MM_DUAL_VID_SUPPORT 
                    /*check 3D propery*/
                    pt_strm_fltr_fct  = _mm_hdlr_3d_multi_video_track_filter;
                    pv_strm_fltr_tag   = (VOID*)pt_svctx;
                    i4_ret = svctx_get_strm_comp_id(
                                    pt_svctx,
                                    ST_VIDEO,
                                    FALSE,
                                    pt_strm_fltr_fct,
                                    pv_strm_fltr_tag,
                                    &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_comp_id),
                                    &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_rec)
                                    );
#endif 
#ifdef MW_MM_FAKE_DAUL_VID
                    if (pt_media_desc->b_multi_track_3d  &&
                        !pt_media_desc->b_is_thumbnail_playback)
                    {
                        MM_3D_PROPERTY_T    e_3d_property = MM_3D_PROPERTY_DAUL;
                        UINT32              ui4_tmp = (UINT32)e_3d_property;
                        i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                          SM_SESS_GNRC_SET_TYPE_3D_PROPERTY,
                                          (VOID*)&ui4_tmp,
                                          sizeof(UINT32));
                        if (i4_ret != SMR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"SM_SESS_GNRC_SET_TYPE_3D_PROPERTY fail!\n");
                            i4_ret =  SVCTXR_FAIL;
                        }
                    }
#endif
                    /*maybe cause assert in driver(timing issue) if do not add this condition*/
                    i4_ret = _mm_hdlr_reselect_strms(pt_svctx);
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID)))       
                    /*open 3D second track*/
                    if (pt_media_desc->b_multi_track_3d &&
                       !pt_media_desc->b_is_thumbnail_playback)
                    {
                         
                         pt_running_svc_req->t_strm_reqs_3d_vid.e_req_type                = STRM_REQ_SCDB;   
                         i4_ret = mm_hdlr_open_strm(pt_svctx, &(pt_running_svc_req->t_strm_reqs_3d_vid), TRUE);
                        if (i4_ret != SVCTXR_OK)
                        {
                            return i4_ret;
                        }
                    }
#endif
                }
            }
        }
#ifdef TIME_SHIFT_SUPPORT
    }
#endif

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_check_cc_avail_cond
 * Description
 *      The API is used to check currrent service's non-fake cc availability
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *---------------------------------------------------------------------------*/
 static INT32 _mm_hdlr_check_cc_avail_cond(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT16 ui2_num_recs   = 0;
    INT32  i4_ret;

    /* get the amount of non-fake CC SCDB records */
    i4_ret = mm_hdlr_get_non_fake_cc_scdb_num_recs(
                    pt_svctx,
                    &ui2_num_recs
                    );
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    if ((ui2_num_recs  > 0 && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES) ||
        (ui2_num_recs == 0 && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_NO))
    {
            /* nothing change */
        return SVCTXR_OK;
    }

    if (ui2_num_recs > 0)
    {
        pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_YES;
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
    }
    else
    {
        pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_NO;
        mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_player_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_player_nty(
                    PLAYER_T*                   pt_player,
                    PLAYER_NTFY_CODE_T          e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    MM_SVC_REQ_T*  pt_running_svc_req = NULL;
    SVCTX_NTFY_CODE_T   e_nfy_code;
    INT32          i4_ret             = SVCTXR_OK;
    HANDLE_T       h_media;
    MEDIA_DESCRIBT_T*   pt_media_desc;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL, line = %d",__func__, __LINE__));

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL, line=%d\n", __func__, __LINE__));
    if(pt_running_svc_req == NULL)
    {
        return SVCTXR_FAIL;
    }

    /* WILLY_BAD_INTLVE*/
    pt_running_svc_req->pt_player_act = pt_player;

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    switch(e_code)
    {
    case PLAYER_NTFY_CODE_PIPE_OPENED_GET_CMDS:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_svctx->b_fake_disc = FALSE;/*MM no need care this, and when exit it will reselect_svc*/
#endif
        i4_ret = _mm_hdlr_get_cm_cmds(pt_svctx, pt_running_svc_req, (CM_COMMAND_T*)ui4_data);
        break;
    case PLAYER_NTFY_CODE_CONN_CONNECTED:
    case PLAYER_NTFY_CODE_SCDB_OPENED:
        if(e_code == PLAYER_NTFY_CODE_CONN_CONNECTED)
        {
#ifndef MULTIMEDIA_AUTOTEST_SUPPORT 
            x_dbg_stmt(_INFO_HEADER"PLAYER_NTFY_CODE_CONN_CONNECTED\n");
#endif 
            if(!x_handle_valid(pt_running_svc_req->pt_player_act->h_scdb))
            {
                /*if cm connected but scdb isn't opened then do nothing*/
                break;
            }
        }
        else
        {
#ifndef MULTIMEDIA_AUTOTEST_SUPPORT         
            x_dbg_stmt(_INFO_HEADER"PLAYER_NTFY_CODE_SCDB_OPENED\n");
#endif 
            if(pt_running_svc_req->pt_player_act->e_conn_cond != CM_COND_CONNECTED)
            {
                /*if scdb opened but cm isn't connected then do nothing*/
                break;
            }
            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                
                i4_ret =  x_scdb_reg_cmp_fcts(pt_running_svc_req->pt_player->h_scdb, t_g_svctx_class.t_scdb_sort_param.pv_tag, 
                                             t_g_svctx_class.t_scdb_sort_param.pf_scdb_sort_fct);
                if(i4_ret != SCDBR_OK)
                {
                    x_dbg_stmt(_INFO_HEADER"Reg cmp fcts failed %d \n",i4_ret);
                }
            }
        }
        if (x_handle_valid(pt_running_svc_req->pt_player_act->h_scdb) == FALSE) 
        {
            /*send to itself and return*/
            break;
        }
        /*if cm connected and scdb opened then do next*/
        i4_ret = _mm_hdlr_handle_cm_connected(pt_svctx, pt_running_svc_req);
        if(i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_cm_connected() i4_ret = %d \n",i4_ret, __LINE__);
        }
        else
        {
#if 0 //def ENABLE_MMS_SUPPORT
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                              SVCTX_NTFY_CODE_MEDIA_LOST,
                                              ST_UNKNOWN
                                              );
#endif
        }
        break;  
    case PLAYER_NTFY_CODE_SCDB_UPDATED:
    case PLAYER_NTFY_CODE_SESS_OPENED:
        if(e_code == PLAYER_NTFY_CODE_SCDB_UPDATED)
        {
#ifndef MULTIMEDIA_AUTOTEST_SUPPORT        
            x_dbg_stmt(_INFO_HEADER"PLAYER_NTFY_CODE_SCDB_UPDATED\n");
#endif             
            if ((pt_media_desc->pt_strm_info))
            {
                if ((ui4_data & SCDB_RECORD_MOD) > 0)
                {
                    x_dbg_stmt(_INFO_HEADER"%s(), SCDB_RECORD_MOD line=%d \n", __func__, __LINE__);
                    mm_hdlr_invoke_client_svc_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_SCDB_MODIFY,
                                    ST_UNKNOWN
                                    );
                }
            }

            if((pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS) ||
                (pt_media_desc->pt_strm_info))
            {
                /*the flag is used for general format file except ts,pvr,timeshift etc.*/
                if(pt_media_desc->b_is_scdb_updated)
                {
                    return SVCTXR_OK;
                }
                else
                {
                    pt_media_desc->b_is_scdb_updated = TRUE;
                }
            }
            _mm_hdlr_check_cc_avail_cond(pt_svctx, pt_running_svc_req);
            i4_ret = _mm_hdlr_handle_scdb_updated(pt_svctx, pt_running_svc_req, ui4_data);
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_scdb_updated(), i4_ret=%d line%d \n", i4_ret,__LINE__);
            }
            if(((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
                (NULL == pt_media_desc->pt_strm_info)) 
                ||
               pt_running_svc_req->pt_player_act->e_sess_cond != SM_SESS_COND_OPENED)
            {
                /*if scdb updated but session isn't opened then do nothing*/
                break;
            }
        }
        else
        {
            x_dbg_stmt(_INFO_HEADER"PLAYER_NTFY_CODE_SESS_OPENED\n");
            if(((pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS) ||
                (pt_media_desc->pt_strm_info))
                && 
               !pt_media_desc->b_is_scdb_updated)
            {
                /*if session opened but scdb isn't updated then do nothing*/
                break;
            }
        }
        /*if scdb updated and session opened then do next*/
        _mm_hdlr_check_cc_avail_cond(pt_svctx, pt_running_svc_req);
        i4_ret = _mm_hdlr_handle_sess_opened(pt_svctx, pt_running_svc_req);
        if (!pt_media_desc->b_is_add_range)
        {
            pt_media_desc->b_is_add_range = TRUE;
        }
        else if (!pt_media_desc->b_is_bad_intlevd)
        {
            x_dbg_stmt(_INFO_HEADER"\n already add range !!\n");
            break;
        }
        if ((pt_media_desc->b_is_bad_intlevd && 
            pt_running_svc_req->pt_player != NULL &&
            pt_running_svc_req->pt_player->e_sess_cond == SM_SESS_COND_OPENED &&
            pt_running_svc_req->pt_player_enhance != NULL &&
            pt_running_svc_req->pt_player_enhance->e_sess_cond == SM_SESS_COND_OPENED &&
            pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE) ||
            (pt_media_desc->b_is_bad_intlevd &&
            pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE) ||
            !pt_media_desc->b_is_bad_intlevd)
        {
            switch (pt_running_svc_req->t_parent.e_src_type)
            {
            case SRC_TYPE_MEDIA_NETWORK:
                if(pt_media_desc->b_is_scdb_updated && 
                   pt_running_svc_req->pt_player->e_sess_cond == SM_SESS_COND_OPENED)
                {
                    if((pt_media_desc->e_svc_protocol != PROTOCL_MODE_URI)
                    && (pt_media_desc->e_svc_protocol != PROTOCL_MODE_PULL)
                    && (pt_media_desc->e_svc_protocol != PROTOCL_MODE_PUSH))
                    {
                        break;
                    }
                    else if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH)
                    {
                        i4_ret = x_cm_get(pt_player->h_conn,
                                          PLAYBACK_CTRL_GET_SHARED_MEMORY,
                                          (VOID*)&(pt_media_desc->t_pbctrl_push_buf_info));
                        if (i4_ret != SVCTXR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"x_cm_get SHARED_MEMORY failed!!! line%d\n",__LINE__);
                        }
                        mm_hdlr_invoke_client_svc_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_BUF_READY,
                                    ST_UNKNOWN
                                    );
                    }
                }
                /*intention missing break*/
            case SRC_TYPE_MEDIA_STORGE:
            case SRC_TYPE_MEDIA_TIME_SHIFT:
            case SRC_TYPE_MEDIA_PVR:
                i4_ret = _mm_hdlr_media_preprocess(pt_running_svc_req);
                if (i4_ret == SVCTXR_OK)
                {
                    e_nfy_code = SVCTX_NTFY_CODE_MEDIA_PB_RANGE_ALLOC;
                }
                else
                {
                    if (i4_ret == SVCTXR_NO_RESOURCE)
                    {
                        ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));
                    }
                    x_dbg_stmt(_INFO_HEADER"_mm_hdlr_media_preprocess(), ret=%d line%d \n", i4_ret, __LINE__);
                    e_nfy_code = SVCTX_NTFY_CODE_MEDIA_ERROR;                
                    mm_hdlr_invoke_client_svc_nfy_fct(
                                                  pt_svctx,
                                                  SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC,
                                                  ST_UNKNOWN);
                }
                mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                                  e_nfy_code,
                                                  ST_UNKNOWN
                                                  );
                break;
            case SRC_TYPE_MEDIA_MEM_CLIP:
                break;
            default:
                return SVCTXR_NOT_SUPPORT;
            }
        }
        break;   
    case PLAYER_NTFY_CODE_SESS_STARTED:
        x_dbg_stmt(_INFO_HEADER"PLAYER_NTFY_CODE_SESS_STARTED\n");
        i4_ret = _mm_hdlr_handle_sess_started(pt_svctx, pt_running_svc_req);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_sess_started(), ret = %d line = %d \n", i4_ret,__LINE__);
        }
        break;
        
    case PLAYER_NTFY_CODE_PIPE_CLOSED:
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
        if ( pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE )
        {
            if ( !pt_media_desc->b_is_sec_stop)
            {
                pt_media_desc->b_is_sec_stop = TRUE;
                break;
            }
            i4_ret = _mm_hdlr_handle_player_stopped(pt_svctx, pt_running_svc_req);   
        }
        else
#endif
        {
            i4_ret = _mm_hdlr_handle_player_stopped(pt_svctx, pt_running_svc_req);
        }
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_player_stopped(), ret = %d line = %d \n", i4_ret,__LINE__);
        }
        break;
    
    case PLAYER_NTFY_CODE_CONN_DISCONNECTED:
        i4_ret = _mm_hdlr_handle_cm_disconnected(pt_svctx, (BOOL)ui4_data);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_cm_disconnected(), ret = %d line = %d\n", i4_ret, __LINE__);
        }
        break;
    case PLAYER_NTFY_CODE_CONN_INFO_NTFY:
        /*i4_ret = _mm_hdlr_handle_cm_info_ntfy(pt_svctx, ui4_data);*/
        break;
 
    case PLAYER_NTFY_CODE_SESS_CLOSING:
        i4_ret = _mm_hdlr_handle_sess_in_closing(pt_svctx, pt_running_svc_req);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_handle_sess_in_closing(), ret=%d line%d line = %d\n", i4_ret,__LINE__);
        }
        break;
    case PLAYER_NTFY_CODE_SESS_STOPPING:
        /*i4_ret = _mm_hdlr_handle_sess_in_stopping(pt_svctx, pt_running_svc_req);*/
        break;
    case PLAYER_NTFY_CODE_REQ_SEND_MSG:
        /* while queue file to Playback handler sucessfully, */
        /* playbak handler would notify with CM_PLAYBACK_CONN_INFO_ITEM_QUEUED
        it would change the player's connetion state from connected to CM_PLAYBACK_CONN_INFO_ITEM_QUEUED*/
        if (((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond == CM_PLAYBACK_CONN_INFO_ITEM_QUEUED)
        {
           /* just ignore CM_PLAYBACK_CONN_INFO_ITEM_QUEUED event */
           /* ((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond = CM_COND_CONNECTED; */
           break;
        }
        else if (((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond == CM_PLAYBACK_CONN_INFO_FILE_ERROR)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                              SVCTX_NTFY_CODE_MEDIA_LOST,
                                              ST_UNKNOWN
                                              );
        }
        else if (((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond == CM_PLAYBACK_CONN_INFO_EOF)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                              SVCTX_NTFY_CODE_MEDIA_EOF,
                                              ST_UNKNOWN
                                              );
        }
        else if (((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond == CM_PLAYBACK_CONN_INFO_PAT_GOT)
        {
            UINT8 ui1_pmt_num = 0;
            if(pt_media_desc->pt_pmt_list != NULL)
            {
                ui1_pmt_num = pt_media_desc->pt_pmt_list->ui1_pmt_num;
            }
            
            pt_media_desc->pt_pmt_list = (PLAYBACK_PMT_LIST_T*)(((SVCTX_MSG_T*)ui4_data)->u.t_conn.ui4_data);
            if(pt_media_desc->pt_pmt_list->ui1_pmt_num >= 1)
            {
                mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                                  SVCTX_NTFY_CODE_VIDEO_NUM_READY,
                                                  ST_UNKNOWN);
            }
            /*DTV00352421 this is a trick,judge if pat has gotten before with ui1_pmt_num,to fix pvr has two pats*/
            if(!pt_media_desc->b_is_scdb_updated && ui1_pmt_num == 0)
            {
                pt_media_desc->b_is_parse_pmt = TRUE;
                i4_ret = _mm_hdlr_seek_to_ts_file_start(pt_svctx,
                                                        pt_running_svc_req);
            }
        }
        else 
        {
            if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH)
            {
                BOOL b_need_snd_msg = TRUE;
                MM_PLAYBACK_CTRL_PUSH_EVENT_T e_push_event;
                switch(((SVCTX_MSG_T*)ui4_data)->u.t_conn.e_cond)
                {
                case CM_PLAYBACK_CONN_INFO_ITEM_CONSUMED:
                    e_push_event = MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_CONSUMED;
                    break;
                default:
                    b_need_snd_msg = FALSE;
                    break;
                }
                if(b_need_snd_msg)
                {
                    pt_media_desc->t_pbctrl_push_mode_info.pf_nfy(
                                            e_push_event,
                                            (VOID*)(pt_media_desc->t_pbctrl_push_mode_info.hHandle),
                                            ((SVCTX_MSG_T*)ui4_data)->u.t_conn.ui4_data 
                                            );

                    break;
                }
            }            
        }
        
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }
    return i4_ret;
}

#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_crnt_evn_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_crnt_evn_nty(
                    CRNT_EVENT_T*               pt_crnt_evn,
                    CRNT_EVENT_NTFY_CODE_T      e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    return SVCTXR_OK;
}
#endif /* TIME_SHIFT_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_audio_fmt_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_audio_fmt_chk (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    if (pt_audio_strm_req == NULL
            || x_handle_valid(pt_audio_strm_req->h_stream) == FALSE)
    {
        return SVCTXR_OK;
    }

    if (svctx_is_valid_audio_fmt(pt_svctx) == FALSE)
    {
        return SVCTXR_OK;
    }

    /* if audio format is still not available, NO NEED to send twice */
    mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_audio_strm_req
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_video_fmt_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_video_fmt_chk (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pt_video_strm_req == NULL
            || x_handle_valid(pt_video_strm_req->h_stream) == FALSE)
    {
        return SVCTXR_OK;
    }

    if (svctx_is_valid_video_fmt(pt_svctx) == FALSE)
    {
        return SVCTXR_OK;
    }
    /* if video format is still not available, NO NEED to send twice */

    mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    pt_video_strm_req
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_strm_fmt_chker_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_strm_fmt_chker_nty(
                    STRM_FMT_CHKER_T*           pt_strm_fmt_chker,
                    STRM_FMT_CHKER_NTFY_CODE_T  e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx           = (SVCTX_T*) pv_tag;
    MM_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32         i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch(e_code) {
    case STRM_FMT_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG:
        switch((STREAM_TYPE_T)ui4_data) {
        case ST_AUDIO:
            i4_ret = _mm_hdlr_handle_audio_fmt_chk(pt_svctx, pt_running_svc_req);
            break;
        case ST_VIDEO:
            i4_ret = _mm_hdlr_handle_video_fmt_chk(pt_svctx, pt_running_svc_req);
            break;
        default:
            break;
        }
        break;
    case STRM_FMT_CHKER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }

    return i4_ret;
}
static VOID _mm_hdlr_chk_strm_timeout_nfy(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    
    SVCTX_T*            pt_svctx           = (SVCTX_T*) pv_tag;
    MM_SVC_REQ_T*       pt_running_svc_req = NULL;
    /*to large may crash stack*/
    MEDIA_DESCRIBT_T*   pt_media_desc;
    INT32               i4_ret             = SVCTXR_OK;
    
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       x_dbg_stmt("\n[MM_hdlr]:Send msg failed ,ret(%d)\n",i4_ret);
       ASSERT(FALSE,("_mm_hdlr_chk_strm_timeout_nfy get media_desc fail"));
    }
    if(!pt_media_desc->b_uri_ts_strm_chk)
    {
        pt_media_desc->b_chk_strm_timer_out = TRUE;
    }

    mm_hdlr_check_strms_status(pt_svctx);
    if(!pt_media_desc->b_uri_ts_strm_chk)
    {
        i4_ret = _mm_hdlr_send_msg(pt_running_svc_req->t_parent.h_svctx,
                          (UINT32)NULL,
                          _SVCTX_MSG_MM_OPEN_STRM_AFTER_CHK,
                          NULL,
                          0,
                          (UINT32)0,
                          (UINT32)0);
         if (i4_ret != SVCTXR_OK)
         {
             x_dbg_stmt("\n[MM_hdlr]:Send msg failed , LINE(%d),ret(%d)\n",__LINE__,i4_ret);
         }
    }
    pt_media_desc->b_uri_ts_strm_chk = FALSE;
    
    return;
}

#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_handle_rating_chker_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_handle_rating_chker_nty(
                    RATING_CHKER_T*             pt_rating_chker,
                    RATING_CHKER_NTFY_CODE_T    e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx = (SVCTX_T*) pv_tag;
    INT32         i4_ret   = SVCTXR_OK;
    MM_SVC_REQ_T* pt_running_svc_req;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch(e_code) {
    case RATING_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG:
        i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
        break;
    case RATING_CHKER_NTFY_CODE_SVL_UPDATE:
        /*if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL) {
            i4_ret = _ch_hdlr_monitor_scart_signal(
                    pt_svctx,
                    pt_running_svc_req
                    );
        }*/
        break;
    case RATING_CHKER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }

    return i4_ret;
}
#endif /* TIME_SHIFT_SUPPORT */

/*-----------------------------------------------------------------------------
 * SVCTX public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_check_audio_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_check_audio_fmt (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req    = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    UINT32      ui4_strm_format_cond = 0;

    if (pt_audio_strm_req == NULL) {
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) {
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_audio_strm_req
                    );
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        return SVCTXR_OK;
    }

    if (pt_audio_strm_req->e_strm_cond == SM_COND_STARTED) {
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_audio_strm_req
                    );
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
            mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CA,
                        pt_audio_strm_req
                        );
            mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CP,
                        pt_audio_strm_req
                        );
        }
    }

    ui4_strm_format_cond = strm_fmt_chker_get_format_status(pt_running_svc_req->pt_strm_fmt_chker);
    if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE || (ui4_strm_format_cond & ST_MASK_AUDIO) > 0) {
        mm_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_audio_strm_req
                    );
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_check_video_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_check_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret;

    i4_ret = _mm_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = _mm_hdlr_check_audio_video_fmt(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_is_supported_strm_type
 * Description
 *      The API is used to check a stream type is the service context supported
 * Input arguments
 *      e_strm_type            the stream type
 * Output arguments
 *      None
 * Returns
 *      TRUE                supported stream type
 *      FALSE               not-supported stream type
 *---------------------------------------------------------------------------*/
BOOL mm_hdlr_is_supported_strm_type(
                    STREAM_TYPE_T           e_strm_type
                    )
{
    UINT8 ui1_idx;

    for (ui1_idx = 0; ui1_idx <(UINT8)STRM_TYPE_COUNT; ui1_idx++) {
        if (e_g_mm_strm_types[ui1_idx] == e_strm_type) {
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_reset_audio_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_reset_audio_info(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_running_svc_req->pt_strm_fmt_chker != NULL) {
        strm_fmt_chker_reset_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_AUDIO);
    }
    svctx_reset_audio_info(pt_svctx);
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_reset_video_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_reset_video_info(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_running_svc_req->pt_strm_fmt_chker != NULL) {
        strm_fmt_chker_reset_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);
    }
    svctx_reset_video_info(pt_svctx);
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_strm_trick_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_strm_trick_req(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    MM_SPEED_TYPE_T             e_speed,
                    BOOL                        b_is_aud_trick
                    )
{
    HANDLE_T h_stream;
    SM_SET_TYPE_T t_set_type;
    INT32         i4_ret = SVCTXR_FAIL;
    VOID*         pv_set_info;
    INT8          i1_info_sz;
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
    {
        return SVCTXR_FAIL;
    }
    h_stream = pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream;
    if (e_strm_type == ST_VIDEO)
    {
        t_set_type =  SM_VSH_SET_TYPE_SPEED;
        pv_set_info = (VOID*)e_speed;
        i1_info_sz = (INT8)sizeof(MM_SPEED_TYPE_T);
    }
    else if (e_strm_type == ST_AUDIO)
    {
        if (e_speed == MM_SPEED_TYPE_PAUSE)
        {
            if(pt_running_svc_req->t_strm_reqs[e_strm_type].e_strm_cond == SM_COND_STOPPING)
            {
                /*ignore pause command otherwise it may be make audio driver send pause notify instead of stop notify to mw*/
                return SVCTXR_OK;
            }
            t_set_type = SM_ASH_SET_TYPE_PAUSE;
            pv_set_info = (VOID*)TRUE;
            i1_info_sz = (INT8)sizeof(BOOL);
        }
        else if (e_speed == MM_SPEED_TYPE_FORWARD_1X && !b_is_aud_trick)
        {
            /*TBD should check state condition*/
            if(pt_running_svc_req->t_strm_reqs[e_strm_type].e_strm_cond == SM_COND_OPENED)
            {
                /*ignore resume command otherwish it would make audio driver to play*/
                return SVCTXR_OK;
            }
            t_set_type = SM_ASH_SET_TYPE_RESUME;
            pv_set_info = (VOID*)TRUE;
            i1_info_sz = (INT8)sizeof(BOOL);
        }
        else /*audio trick always go here*/
        {
            t_set_type =  SM_ASH_SET_TYPE_SPEED;
            pv_set_info = (VOID*)e_speed;
            i1_info_sz = (INT8)sizeof(MM_SPEED_TYPE_T);
        }
    }
    else
    {
        return SVCTXR_NOT_SUPPORT;
    }
    if (x_handle_valid(h_stream) == TRUE)
    {
       i4_ret = x_sm_set(h_stream,t_set_type,(VOID*)pv_set_info,(SIZE_T)i1_info_sz);
       if (i4_ret == SMR_OK)
       {
           i4_ret = SVCTXR_OK;
       }
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
       if ((i4_ret == SVCTXR_OK) && (e_strm_type == ST_VIDEO) && (x_handle_valid(pt_running_svc_req->t_strm_reqs_3d_vid.h_stream)))
       {
           i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs_3d_vid.h_stream,
                                t_set_type,
                                (VOID*)pv_set_info,
                                (SIZE_T)i1_info_sz);
           if (i4_ret == SMR_OK)
           {
               i4_ret = SVCTXR_OK;
           }   
       }
#endif
    }

    return i4_ret;
}



/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_select_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_select_strm (
                     SVCTX_T*                    pt_svctx,
                     SVCTX_MSG_T*                pt_msg
                     )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

#ifndef DRV_NO_PRE_PARSING
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }
#endif
    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    if (pt_running_svc_req->pt_player->e_sess_cond != SM_SESS_COND_STARTED){
        return SVCTXR_WRONG_STATE;
    }

    return _mm_hdlr_handle_strm_select_req (
                    pt_svctx,
                    pt_running_svc_req,
                    pt_msg->u.pt_strm_req,
                    TRUE
                    );
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_select_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_select_svc (
                    SVCTX_T*                    pt_svctx
                    )
{
    MM_SVC_REQ_T*                 pt_running_svc_req     = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32                         i4_ret = SVCTXR_OK;
    MEDIA_FORMAT_T                t_format;
    MEDIA_DESCRIBT_T*             pt_media_desc;
    /* const CHAR*                   ps_file_path; */
    MM_SVC_UOP_HANDLER_FCT_TBL_T  t_fct_tbl;
    SM_SESS_GNRC_DATA_T*          pt_sess_data;
    MINFO_INFO_T                  t_minfo_gen_rec;
    UINT32                        ui4_sz = 0;
    UINT16                        ui2_vid_rec_nums = 0;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
    /* ps_file_path = pt_running_svc_req->t_src_info.u_info.s_src_file_path; */
    if(pt_running_svc_req == NULL)
    {
        return SVCTXR_FAIL;
    }
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }

    x_dbg_stmt(_INFO_HEADER" mm_hdlr_select_svc\n");
    svctx_update_src_snk_names(pt_svctx,
                               pt_running_svc_req->t_src_info.s_src_grp_name,
                               pt_running_svc_req->t_parent.s_snk_grp_name);

    /*hooking media uop handler*/
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }

    /* ogg case, need enhancement, do it in recognizer*/
    i4_ret = x_minfo_get_num_recs(pt_media_desc->h_media_info,
                                  MINFO_INFO_TYPE_VIDEO_ATTR,
                                  &ui2_vid_rec_nums);
    if (ui2_vid_rec_nums == 0 && pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
    {
        pt_media_desc->t_media_fmt.e_mm_svc_type = MEDIA_SVC_TYPE_AUDIO_ONLY;
    }
    
    t_format = pt_media_desc->t_media_fmt;
    if (SVCTXR_OK != mm_hdlr_find_handler(&t_format, &t_fct_tbl))
    {
        return SVCTXR_NOT_SUPPORT;
    }
    pt_running_svc_req->t_uop_fct_tbl = t_fct_tbl;
#ifndef DRV_NO_PRE_PARSING
    i4_ret = x_timer_create(&gh_pre_parsing_time_out);
    if (i4_ret != OSR_OK) {
        return SVCTXR_FAIL;
    }
#endif
    i4_ret = strm_fmt_chker_open(
                        DEFAULT_STRM_FMT_CHK_TIMEOUT,
                        _mm_hdlr_handle_strm_fmt_chker_nty,
                        pt_svctx,
                        &pt_running_svc_req->pt_strm_fmt_chker
                        );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MM_LAST_MEMORY_SUPPORT
    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
    {
        BOOL b_is_find = FALSE;
        UINT16 ui2_idx;
        for(ui2_idx = 0; ui2_idx < pt_media_desc->ui2_last_num; ui2_idx++)
        {
            if(ui2_idx >= 10)
            {
                break;
            }
            if(pt_media_desc->t_mm_pmt_last_info[ui2_idx].ui4_title_idx == (UINT32)pt_media_desc->ui2_pmt_idx && b_is_find == FALSE)
            {
                x_memcpy(&pt_media_desc->t_mm_last_info,&pt_media_desc->t_mm_pmt_last_info[ui2_idx],sizeof(MM_SVC_STOP_INFO));
                b_is_find = TRUE;
            }
            if(b_is_find == TRUE)
            {
                if(ui2_idx < (pt_media_desc->ui2_last_num-1))
                {
                    x_memcpy(&pt_media_desc->t_mm_pmt_last_info[ui2_idx], &pt_media_desc->t_mm_pmt_last_info[ui2_idx+1], sizeof(MM_SVC_STOP_INFO));
                    x_memset(&pt_media_desc->t_mm_pmt_last_info[ui2_idx+1], 0, sizeof(MM_SVC_STOP_INFO));
                }
            }
        }
        if(b_is_find == TRUE)
        {
           pt_media_desc->ui2_last_num--; 
        }
        else
        {
            x_memset(&pt_media_desc->t_mm_last_info, 0, sizeof(MM_SVC_STOP_INFO));
        }
    }
    else if ((pt_running_svc_req->ui4_title_idx < 3) &&
        (pt_running_svc_req->ui4_playlist_idx < 3) &&
        (pt_running_svc_req->ui4_playlist_idx == pt_media_desc->t_mm_title_last_info[pt_running_svc_req->ui4_title_idx].ui4_playlist_idx))
    {
        x_memcpy(&pt_media_desc->t_mm_last_info, &pt_media_desc->t_mm_title_last_info[pt_running_svc_req->ui4_title_idx], sizeof(MM_SVC_STOP_INFO));
        x_memset(&pt_media_desc->t_mm_title_last_info[pt_running_svc_req->ui4_title_idx], 0, sizeof(MM_SVC_STOP_INFO));
    }
    else
    {
        x_memset(&pt_media_desc->t_mm_last_info, 0, sizeof(MM_SVC_STOP_INFO));
        x_memset(&pt_media_desc->t_mm_title_last_info[pt_running_svc_req->ui4_title_idx], 0, sizeof(MM_SVC_STOP_INFO));
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(t_format))
    {
            i4_ret = crnt_evn_open(
                            DEFAULT_REQUERY_EVN_DELAY,
                            pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
                                ? FALSE : TRUE,
                            _mm_hdlr_handle_crnt_evn_nty,
                            pt_svctx,
                            &pt_running_svc_req->pt_crnt_event
                            );
            if (i4_ret != SVCTXR_OK) {
                return i4_ret;
            }

            if (pt_svctx->pf_block_check_fct == NULL) {
                pt_svctx->pf_block_check_fct = svctx_null_block_check_fct;
            }

            i4_ret = rating_chker_open(
                            pt_svctx->pf_block_check_fct,
#ifdef SVCTX_SPLIT_SUPPORT
                            pt_svctx->pf_block_check_fct_tag,
#endif
                            DEFAULT_BLOCK_DELAY,
                            _mm_hdlr_handle_rating_chker_nty,
                            pt_svctx,
                            &pt_running_svc_req->pt_rating_chker
                            );
            if (i4_ret != SVCTXR_OK) {
                return i4_ret;
            }
        if (pt_running_svc_req->t_parent.e_src_type == SRC_TYPE_MEDIA_TIME_SHIFT)
        {
            SVC_BLOCK_CHECK_GUDIE_T e_guide;
            HANDLE_T                h_svl = NULL_HANDLE;

            i4_ret = _mm_hdlr_get_svl(pt_running_svc_req, &h_svl);
            if (i4_ret != SVCTXR_OK) {
                return i4_ret;
            }

            if (SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG == pt_svctx->e_block_rule)
            {
                /* only check input source lock first */
                e_guide = SVC_BLOCK_CHECK_USER_BLOCK_INP;
            }
            else
            {
                /* could check both input source lock and channel lock first */
                e_guide = SVC_BLOCK_CHECK_USER_BLOCK;
            }

            /* check input source lock first */
            _mm_hdlr_is_blocked_service(
                        pt_svctx,
                        pt_running_svc_req,
                        e_guide,
                        TRUE
                        );
        }
    }
#endif /* TIME_SHIFT_SUPPORT */
    if ((pt_media_desc->e_fill_scdb_mode == MM_SVC_FILL_SCDB_APPEND) || 
        ((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
        (pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI)  && 
        (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)))
    {
        pt_media_desc->b_chk_strm_timer_started = FALSE;
        pt_media_desc->b_chk_strm_timer_out     = FALSE;

        i4_ret = x_timer_create(&pt_running_svc_req->h_chk_strm_timer);
        if (i4_ret != OSR_OK) {
            DBG_ERROR(("[MM_hdlr]:Create h_chk_strm_timer failed! ret=%d\r\n", i4_ret)); 
            return i4_ret;
        }
    }

    x_memset( &t_minfo_gen_rec, 0, sizeof(MINFO_INFO_T) );
    ui4_sz = sizeof(MINFO_INFO_T);
    i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                              MINFO_INFO_TYPE_GENERIC,
                              1,
                              &t_minfo_gen_rec,
                              (SIZE_T*)&ui4_sz);
    if (i4_ret != MINFOR_OK)
    {
        pt_media_desc->b_is_bad_intlevd = FALSE;
    }
    else
    {
        pt_media_desc->b_is_bad_intlevd = t_minfo_gen_rec.u.t_general_info.b_is_bad_interleaved;
    }
    if ( pt_media_desc->b_is_bad_intlevd )
    {
        x_dbg_stmt(_INFO_HEADER"\n This file is bad interleaved file!\n");
    }

    i4_ret = player_open (
                    pt_svctx->s_snk_grp_name,
                    DEFAULT_RECONNECT_DELAY,
                    _mm_hdlr_handle_player_nty,
                    pt_svctx,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    pt_svctx->i1_prio_diff,
#endif
                    &pt_running_svc_req->pt_player
                    );
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /* WILLY_BAD_INTLVE*/
    if (pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
         i4_ret = player_open (
                        pt_svctx->s_snk_grp_name,
                        DEFAULT_RECONNECT_DELAY,
                        _mm_hdlr_handle_player_nty,
                        pt_svctx,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                        pt_svctx->i1_prio_diff,
#endif
                        &pt_running_svc_req->pt_player_enhance
                        );
        if (i4_ret != SVCTXR_OK) 
        {
            return i4_ret;
        }
    }
#endif

    /*don't forget to free data object*/
    pt_sess_data = (SM_SESS_GNRC_DATA_T*) x_mem_alloc(sizeof(SM_SESS_GNRC_DATA_T));
    if (pt_sess_data == NULL) {
        return SVCTXR_FAIL;
    }
    x_memset(pt_sess_data, 0, sizeof(SM_SESS_GNRC_DATA_T));
    if (MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
    {
		pt_sess_data->ui2_pcr_pid = pt_media_desc->ui2_pcr_pid;
	}
	else
    {
		pt_sess_data->ui2_pcr_pid = MPEG_2_NULL_PID;
	}
    pt_sess_data->t_media_fmt   = t_format;
    pt_sess_data->e_priority    = pt_media_desc->e_priority;
    pt_sess_data->e_mm_sync_mode= SM_SESS_GNRC_MM_SYNC_UNKNOWN;
    pt_sess_data->b_sub_freerun = pt_media_desc->b_sub_free_run;
    pt_running_svc_req->pt_player->pv_sess_data =(VOID*)pt_sess_data;
    pt_running_svc_req->pt_player->z_sess_data_size = sizeof(SM_SESS_GNRC_DATA_T);
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
     /* WILLY_BAD_INTLVE*/
    if (pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
        pt_running_svc_req->pt_player_enhance->pv_sess_data =(VOID*)pt_sess_data;
        pt_running_svc_req->pt_player_enhance->z_sess_data_size = sizeof(SM_SESS_GNRC_DATA_T);
    }
#endif
    pt_media_desc->pv_sess_data =(VOID*)pt_sess_data;
    pt_media_desc->b_is_scdb_updated = FALSE;
    pt_media_desc->b_is_add_range = FALSE;
    pt_media_desc->b_has_key_table = FALSE;
    pt_media_desc->e_speed = MM_SPEED_TYPE_FORWARD_1X;
#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        pt_media_desc->e_speed = pt_running_svc_req->e_init_speed;
        pt_running_svc_req->e_init_speed = MM_SPEED_TYPE_FORWARD_1X;
    }
#endif

    DBG_INFO((_INFO_HEADER"file %s is playing.\r\n", pt_media_desc->ps_media_path));

    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    /* WILLY_BAD_INTLVE*/
    if (pt_media_desc->b_is_bad_intlevd && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
        i4_ret = player_play(
                        pt_running_svc_req->pt_player_enhance,
                        pt_running_svc_req->t_parent.i4_speed
                        );
    }
#endif

    pt_media_desc->b_set_audio_stop = FALSE;
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_stop_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_stop_svc (
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_stop_only
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret             = SVCTXR_OK;
    MEDIA_DESCRIBT_T*     pt_media_desc;

#ifndef DRV_NO_PRE_PARSING
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }
#endif
    x_dbg_stmt(_INFO_HEADER" mm_hdlr_stop_svc\n");
    if (pt_running_svc_req == NULL)
    {
        return SVCTXR_ALREADY_STOPPED;
    }
    pt_svctx->b_in_stopping = TRUE;
    
    if ( pt_running_svc_req->h_chk_strm_timer)
    {
        i4_ret = x_timer_delete(pt_running_svc_req->h_chk_strm_timer);
        if (i4_ret != SVCTXR_OK)
        {
            DBG_INFO(("\n[MM_hdlr]:SSif timer delete failed ,ret(%d)\n,i4_ret"));
        }
    }

    /*hooking media uop handler*/
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);

    if (pt_svctx->e_state != SVCTX_COND_READY)
    {
        if (pt_svctx->pt_pending_svc_req != NULL)
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_INTERRUPTED,
                        ST_UNKNOWN
                        );
        }
        else
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_IN_STOPPING,
                        ST_UNKNOWN
                        );
        }
    }

#ifndef DRV_NO_PRE_PARSING
    if (x_handle_valid(gh_pre_parsing_time_out) == TRUE) {
        x_timer_delete(gh_pre_parsing_time_out);
    }
#endif
    if (pt_running_svc_req->pt_strm_fmt_chker)
    {
        i4_ret = strm_fmt_chker_close(pt_running_svc_req->pt_strm_fmt_chker);
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->pt_strm_fmt_chker = NULL;
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    if (pt_running_svc_req->pt_rating_chker)
    {
        i4_ret = rating_chker_close(pt_running_svc_req->pt_rating_chker);
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->pt_rating_chker = NULL;
        }
    }

    if (pt_running_svc_req->pt_crnt_event)
    {
        i4_ret = crnt_evn_close(pt_running_svc_req->pt_crnt_event);
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->pt_crnt_event = NULL;
        }
    }
#endif /* TIME_SHIFT_SUPPORT */

#ifdef ENABLE_DIVX_DRM
    if (x_handle_valid(pt_media_desc->h_drm))
    {
        i4_ret = x_drm_close (pt_media_desc->h_drm);
        if (i4_ret != DRMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"%s: x_drm_close failed, code = %d\r\n", __func__, i4_ret);
        }
        pt_media_desc->t_drm_type = DRM_TYPE_NONE;
        pt_media_desc->h_drm = NULL_HANDLE;
    }
#endif
    pt_media_desc->b_rtn_frm_rev = TRUE;
#if MM_AB_REPEAT_SUPPORT
        {
            if(pt_media_desc->t_repeat_ab_info.b_set_repeat_a ||
               pt_media_desc->t_repeat_ab_info.b_set_repeat_b)
            {
                MM_SVC_REPEAT_INFO_T t_ab_repeat_info;
                t_ab_repeat_info.t_repeat_type=MM_SVC_REPEAT_AB;
                t_ab_repeat_info.u.e_repeat_ab_type = MM_SVC_REPEAT_AB_TYPE_CANCEL_ALL;
                i4_ret = mm_hdlr_set(
                                    pt_svctx,
                                    SVCTX_MM_SET_TYPE_REPEAT,
                                    (VOID*)&t_ab_repeat_info,
                                    sizeof(t_ab_repeat_info)
                                    );
                if(SVCTXR_OK != i4_ret)
                {
                    DBG_INFO((_ERROR_HEADER"mm_hdlr_set(), ret=%d (Line %d)\r\n", i4_ret,__LINE__));
                }
            }
        }
#endif

    if (pt_running_svc_req->pt_player)
    {
        i4_ret = player_stop(pt_running_svc_req->pt_player);
        if (i4_ret == SVCTXR_OK) 
        {
            i4_ret = player_close(pt_running_svc_req->pt_player);
            if (i4_ret == SVCTXR_OK) 
            {
                pt_running_svc_req->pt_player = NULL;
            }
        }
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    if (pt_running_svc_req->pt_player_enhance)
    {
        i4_ret = player_stop(pt_running_svc_req->pt_player_enhance);
        if (i4_ret == SVCTXR_OK) 
        {
            i4_ret = player_close(pt_running_svc_req->pt_player_enhance);
            if (i4_ret == SVCTXR_OK) 
            {
                pt_running_svc_req->pt_player_enhance = NULL;
            }
        }
    }
#endif
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_check_svc_block
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_check_svc_block (
                    SVCTX_T*                    pt_svctx
                    )
{
    MM_SVC_REQ_T*   pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL            b_old_is_blocked   = pt_svctx->b_is_blocked;
    BOOL            b_cur_is_blocked   = pt_svctx->b_is_blocked;
    INT32           i4_ret             = SVCTXR_OK;
    SRC_OPER_TYPE_T e_src_oper_type;
    SNK_OPER_TYPE_T e_snk_oper_type;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    b_cur_is_blocked = _mm_hdlr_is_blocked_service(/*This function will change pt_svctx->b_is_blocked value*/
                                pt_svctx,
                                pt_running_svc_req,
                                SVC_BLOCK_CHECK_ALL,
                                    TRUE);
    if (b_old_is_blocked == b_cur_is_blocked)
    {
        return i4_ret;
    }
    else if(b_cur_is_blocked)
    {/*when nosignal the b_is_blocked will be cleared, so it need to be assigned again.*/
        pt_svctx->b_is_blocked = b_cur_is_blocked;
    }

    i4_ret = player_get_oper_types(
                        pt_running_svc_req->pt_player,
                        &e_src_oper_type,
                        &e_snk_oper_type
                        );
    if (i4_ret != SVCTXR_OK
          || e_src_oper_type == SRC_OPER_TYPE_DISCONNECT
          || e_snk_oper_type == SNK_OPER_TYPE_CLOSE) {
        /* ignore the check svc block request as stop service*/
        return SVCTXR_OK;
    }

    if (pt_svctx->b_is_blocked == TRUE) 
    {
        i4_ret = _mm_hdlr_block_service(pt_svctx, pt_running_svc_req);
    } 
    else 
    {
        i4_ret = _mm_hdlr_unblock_service(pt_svctx, pt_running_svc_req);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_stop_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_PENDING          stop is in executing
 *      SVCTXR_ALREADY_STOPPED  already stopped
 *      SVCTXR_NOT_SUPPORT      not support stream type
 *      SVCTXR_NOT_EXIST        no matched stream type
 *      SVCTXR_FAIL             SM failed
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_stop_strm (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INT32         i4_ret              = SVCTXR_OK;
    STREAM_TYPE_T e_strm_type         = pt_msg->u.t_stop_strm_req.e_type;
    MM_SVC_REQ_T* pt_running_svc_req  = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*   pt_strm_req         = NULL;
    STRM_REQ_T**  ppt_pending_strm_req = NULL;

#ifndef DRV_NO_PRE_PARSING
    if(gb_pre_parsing)
    {
        return SVCTXR_FAIL;
    }
#endif
    if (pt_running_svc_req == NULL) {
        return SVCTXR_ALREADY_STOPPED;
    }

    switch (e_strm_type) {
    case ST_AUDIO:
        if(pt_msg->u.t_stop_strm_req.b_aux_audio)
        {
            return SVCTXR_OK;
        }
        mm_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
        break;
    case ST_VIDEO:
        mm_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
        break;
    case ST_SUBTITLE:
        break;
    case ST_CLOSED_CAPTION:
        pt_running_svc_req->t_cc_filter.e_cc_type         = SM_CC_TYPE_IGNORE;
        pt_running_svc_req->t_cc_filter.u.ui1_service_num = 0xFF;
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
    /*if(e_strm_type != ST_SUBTITLE)*//*remove for not displaying mp3 lyric in thumbnail mode when replay*/ 
    {
        pt_running_svc_req->t_parent.ui4_stream_mode &= ~(MAKE_BIT_MASK_32(e_strm_type));
    }

    switch (pt_strm_req->e_strm_cond) {
    case SM_COND_CLOSED:
        pt_strm_req->e_req_type                = STRM_REQ_SCDB;
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        i4_ret = SVCTXR_ALREADY_STOPPED;

        if (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)
        {
            (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)(pt_msg->u.t_stop_strm_req.t_sync.pv_tag);
            pt_msg->u.t_stop_strm_req.t_sync.pf_nfy = NULL;
            pt_msg->u.t_stop_strm_req.t_sync.pv_tag = NULL;
        }
        break;
    default:
        if (svctx_get_strm_req_strm_type(pt_strm_req) != e_strm_type) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        if (*ppt_pending_strm_req != NULL) {
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
        }

        pt_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
            pt_strm_req->e_strm_cond = SM_COND_CLOSING;
            i4_ret = SVCTXR_PENDING;
            if (pt_strm_req->t_stop_cb.pf_nfy != NULL) {
                pt_strm_req->t_stop_cb.pf_nfy(pt_strm_req->t_stop_cb.pv_tag);
            }

            pt_strm_req->t_stop_cb.pf_nfy = pt_msg->u.t_stop_strm_req.t_sync.pf_nfy;
            pt_strm_req->t_stop_cb.pv_tag = pt_msg->u.t_stop_strm_req.t_sync.pv_tag;

            if (e_strm_type == ST_MM_SUBTITLE ||
                e_strm_type == ST_SUBTITLE)
            {
                 pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;                 
            }
        } else {
            i4_ret = SVCTXR_FAIL;
        }
        break;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_do_thumbnail_timeout
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_do_thumbnail_timeout(SVCTX_T* pt_svctx)
{
    MM_SVC_REQ_T*       pt_running_svc_req = NULL;

    if (pt_svctx == NULL)
    {
        return SVCTXR_FAIL;
    }
    pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req != NULL)
    {
        INT32               i4_ret = SVCTXR_OK;
        MEDIA_DESCRIBT_T*   pt_media_desc = NULL;

        mm_hdlr_stop_svc(pt_svctx,TRUE);

        i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
        if (i4_ret != SVCTXR_OK)
        {/*here may not send thumbnail done*/
           return SVCTXR_FAIL;
        }
        pt_media_desc->b_video_thumbnail_done = TRUE;
    }
    else
    {
        return SVCTXR_FAIL;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_internal_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    MM_SVC_REQ_T*                    pt_running_svc_req;
    PLAYER_T*                        pt_player;
    MEDIA_DESCRIBT_T*                pt_media_desc;
    HANDLE_T                         h_media_desc;
    MM_RANGE_CTRL_EVENT_T            e_range_cond;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info;
    INT32                            i4_ret = SVCTXR_OK;
    UINT8                            ui1_idx;
    STREAM_TYPE_T                    e_strm_type = ST_UNKNOWN;
    STRM_REQ_T*                      pt_strm_req;
    BOOL                             b_all_strm_opened = TRUE;


    pt_running_svc_req = (MM_SVC_REQ_T*)(pt_svctx->pt_running_svc_req);
    pt_player = pt_running_svc_req->pt_player;
    h_media_desc = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }
    switch(pt_msg->ui1_msg_id) {
    case _SVCTX_MSG_STRM_NTFY:
        i4_ret = _mm_hdlr_handle_strm_msg(
                    pt_svctx,
                    &pt_msg->u.t_strm
                    );
        break;
    case _SVCTX_MSG_MINFO_NTFY:
        break;

    case _SVCTX_MSG_MIDXBULD_NTFY:
        i4_ret = _mm_hdlr_midxbuld_event_process(pt_msg->u.t_generic.h_obj,    /*index builder handle*/
                                                 pt_msg->u.t_generic.ui4_data1,/*range handle*/
                                                 (MIDXBULD_NFY_EVENT_TYPE_T)pt_msg->u.t_generic.ui4_cond, /*condtion*/
                                                 (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req);/*svc request*/
               break;
    case _SVCTX_MSG_MRANGE_CTRL_NTFY:
        e_range_cond = pt_msg->u.t_generic.ui4_cond;
        pt_range_info = (VOID*)pt_msg->u.t_generic.ui4_data1;
        i4_ret = _mm_hdlr_mrange_ctrl_event_process((MM_PRESENT_RANGE_LIST_T*)pt_range_info,
                                                    pt_running_svc_req,
                                                    e_range_cond,
                                                    pt_msg->u.t_generic.ui4_data2
                                                    );
        if(SVCTXR_OK != i4_ret)
        {
            return i4_ret;
        }
        break;
    case _SVCTX_MSG_MNAV_INFO_UPDATE_NTFY:
        i4_ret = _mm_hdlr_mnav_info_update_process(pt_svctx,
                                                   pt_msg->u.t_generic.ui4_data1,
                                                   (VOID*)pt_msg->u.t_generic.ui4_data2
                                                   );
        break;
#ifdef TIME_SHIFT_SUPPORT
    case _SVCTX_MSG_REC_UTIL_NTFY:
        i4_ret = _mm_hdlr_handle_rec_util_msg(
                    pt_svctx,
                    &pt_msg->u.t_rec
                    );
        break;
    case _SVCTX_MSG_SWDMX_NTFY:
        i4_ret = _mm_hdlr_handle_swdmx_msg(
                    pt_svctx,
                    pt_media_desc,
                    &pt_msg->u.t_generic
                    );
        break;
#endif /* TIME_SHIFT_SUPPORT */

    case _SVCTX_MSG_THUMBNAIL_TIMEOUT_NTFY:
        i4_ret = _mm_hdlr_do_thumbnail_timeout(pt_svctx);
        break;
    case _SVCTX_MSG_RANGE_CB_NTFY:
        i4_ret = _mm_hdlr_do_range_callback(
                                       (VOID*)pt_msg->u.t_generic.h_obj,
                                       (MM_RANGE_COND_T)pt_msg->u.t_generic.ui4_cond,
                                       pt_msg->u.t_generic.ui4_data1,
                                       pt_msg->u.t_generic.ui4_data2);
        break;
#ifdef ENABLE_NAUTILUS
    case _SVCTX_MSG_DRM_TIMEOUT_FORCE_EXIT_NTFY:
        i4_ret = mm_hdlr_stop_svc(pt_svctx, TRUE);        
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                          SVCTX_NTFY_CODE_DRM_CERT_EXPIRED,
                                          ST_UNKNOWN);
        break;
#endif
    case _SVCTX_MSG_MM_OPEN_STRM_AFTER_CHK:   
        pt_media_desc->b_is_parse_pmt = FALSE;
#ifdef MW_MM_DUAL_VID_SUPPORT 
                /*check 3D propery*/  
        i4_ret = svctx_get_strm_comp_id(
                        pt_svctx,
                        ST_VIDEO,
                        FALSE,
                        _mm_hdlr_3d_multi_video_track_filter,
                        (VOID*)pt_svctx,
                        &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_comp_id),
                        &(pt_running_svc_req->t_strm_reqs_3d_vid.u.t_scdb.t_rec)
                        );
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"svctx_get_strm_comp_id() failed, ret=%d\r\n", i4_ret));
            return i4_ret;
        }
        /*Seek to Start Pos before Play*/
        i4_ret = _mm_hdlr_seek_to_ts_file_start(pt_svctx, pt_running_svc_req);
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"_mm_hdlr_seek_to_ts_file_start() failed, ret=%d\r\n", i4_ret));
            return i4_ret;
        }
#endif
        /*open the normal video track*/
        i4_ret = _mm_hdlr_reselect_strms(pt_svctx);
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"_mm_hdlr_reselect_strms() failed, ret=%d\r\n", i4_ret));
            return i4_ret;
        }

#ifdef MW_MM_DUAL_VID_SUPPORT 
        /*open 3D second track*/
        if (pt_media_desc->b_multi_track_3d &&
           (pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE))
        {
            pt_running_svc_req->t_strm_reqs_3d_vid.e_req_type                = STRM_REQ_SCDB;   
            i4_ret = mm_hdlr_open_strm(pt_svctx, &(pt_running_svc_req->t_strm_reqs_3d_vid), TRUE);
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"mm_hdlr_open_strm() failed, ret=%d\r\n", i4_ret));
                return i4_ret;
            }
        }
#endif
        break;    
    default:
        break;
    }

    if (((pt_msg->ui1_msg_id == _SVCTX_MSG_STRM_NTFY ||
         pt_msg->ui1_msg_id ==_SVCTX_MSG_MRANGE_CTRL_NTFY) &&
          (pt_media_desc->ui1_presnt_range_cnts >= 1) &&
          (!pt_media_desc->t_bufctrl_info.b_is_prebuff)) || /*add for common API*/
          (pt_media_desc->t_bufctrl_info.b_is_prebuff && 
           pt_msg->ui1_msg_id ==_SVCTX_MSG_PREBUF_PLAY_NTFY))
    {
        for (ui1_idx = 0; ui1_idx <(UINT8)STRM_TYPE_COUNT; ui1_idx++) {
            e_strm_type = e_g_mm_strm_types[ui1_idx];
            if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
            {
                break;
            }
            pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
            if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
                if ( pt_strm_req->e_strm_cond == SM_COND_CLOSED  ||
                     pt_strm_req->e_strm_cond == SM_COND_CLOSING ||
                     pt_strm_req->e_strm_cond == SM_COND_OPENING ||
                     pt_strm_req->e_strm_cond == SM_COND_STOPPING)
                {
                    if (e_strm_type == ST_VIDEO && pt_media_desc->b_drv_vdec_not_support && 
                        pt_strm_req->e_strm_cond == SM_COND_CLOSED)
                    {
                        x_dbg_stmt("[mm_hdlr]strm type: video, vdec not support!!!\n");
                        continue;
                    }
                    if (e_strm_type == ST_AUDIO && pt_media_desc->b_drv_adec_not_support &&
                        pt_strm_req->e_strm_cond == SM_COND_CLOSED)
                    {
                        x_dbg_stmt("[mm_hdlr]strm type: audio, audio not support!!!\n");
                        continue;
                    }
                    b_all_strm_opened = FALSE;
                }
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
            /*check for 3D additional track*/ 
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs_3d_vid.h_stream) == TRUE) 
            {
                    if ( pt_running_svc_req->t_strm_reqs_3d_vid.e_strm_cond == SM_COND_CLOSED  ||
                    pt_running_svc_req->t_strm_reqs_3d_vid.e_strm_cond == SM_COND_CLOSING ||
                    pt_running_svc_req->t_strm_reqs_3d_vid.e_strm_cond == SM_COND_OPENING ||
                    pt_running_svc_req->t_strm_reqs_3d_vid.e_strm_cond == SM_COND_STOPPING)
                    {
                        b_all_strm_opened = FALSE;
                    }
            }
#endif            
            }
        }
        if (b_all_strm_opened &&
            !pt_svctx->b_in_stopping &&
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
            pt_media_desc->b_is_set_sec_range && 
#endif
            pt_svctx->e_state != SVCTX_COND_PRESENTING)
        {
            i4_ret = player_play(pt_player, pt_running_svc_req->t_parent.i4_speed);
            
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
            if (pt_running_svc_req->pt_player_enhance != NULL)
            {
               /* WILLY_BAD_INTLVE*/
                if ( pt_media_desc->b_is_bad_intlevd && 
                    pt_media_desc->b_is_set_sec_range && 
                    pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
                {
                    i4_ret = player_play(pt_running_svc_req->pt_player_enhance, pt_running_svc_req->t_parent.i4_speed);            
                }
            }
#endif

            if(pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X)
            {
                    pt_media_desc->b_set_audio_stop = FALSE;
            }
        }
    }
    return i4_ret;
}
#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_crnt_svl_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_get_crnt_svl_rec(
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    HANDLE_T  h_svl          = NULL_HANDLE;
    UINT32    ui4_ver_id     = SVL_NULL_VER_ID;
    UINT16    ui2_svl_rec_id = SVL_NULL_REC_ID;
    INT32     i4_ret;

    i4_ret = _mm_hdlr_get_svl(pt_running_svc_req, &h_svl);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    if (pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
        ui2_svl_rec_id = pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id;
    } else {
        ui2_svl_rec_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
    }

    i4_ret = x_svl_get_rec (
                    h_svl,
                    ui2_svl_rec_id,
                    pt_svl_rec,
                    &ui4_ver_id
                    );
    if (i4_ret != SVLR_OK) {
        return SVCTXR_NO_RECORD;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_crnt_tsl_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_get_crnt_tsl_rec(
                    MM_SVC_REQ_T*               pt_running_svc_req,
                    TSL_REC_T*                  pt_tsl_rec
                    )
{
    UINT32    ui4_ver_id = TSL_NULL_VER_ID;
    INT32     i4_ret     = SVCTXR_OK;
    SVL_REC_T t_svl_rec;
    HANDLE_T  h_tsl;

    i4_ret = mm_hdlr_get_crnt_svl_rec(
                    pt_running_svc_req,
                    &t_svl_rec
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = x_tsl_open(
                    t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id,
                    NULL,
                    NULL,
                    &h_tsl
                    );
    if (i4_ret != TSLR_OK) {
        return SVCTXR_FAIL;
    }

    do {
        i4_ret = x_tsl_get_rec(
                    h_tsl,
                    t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id,
                    pt_tsl_rec,
                    &ui4_ver_id
                    );
        i4_ret = (i4_ret != TSLR_OK) ? SVCTXR_FAIL : SVCTXR_OK;
    } while (0);

    x_tsl_close(h_tsl);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_check_signal_loss_priority
 * Description
 *      The API is used to check the priority between input source block,
 *      channel block, program block, and signal loss.
 * Input arguments
 *      pt_svctx                        The service context object
 *      pt_svc_req                      The service request to check
 *      b_signal_loss_and_check_block   TRUE - check block and send signal loss
 * Output arguments
 *      None
 * Returns
 *      TRUE    signal_loss with higher priority than parental block
 *      FALSE   signal_loss with lower priority than parental block
 *---------------------------------------------------------------------------*/
BOOL mm_hdlr_check_signal_loss_priority(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_svc_req,
                    BOOL                        b_signal_loss_and_check_block
                    )
{
    SVC_BLOCK_COND_T        e_block_cond;
    SVC_BLOCK_CHECK_GUDIE_T e_check_guide;
    BOOL                    b_check_block = FALSE;

    if (pt_svc_req == NULL) {
        return FALSE;
    }

    e_block_cond = rating_chker_get_block_cond(pt_svc_req->pt_rating_chker);

    switch(pt_svctx->e_block_rule) {
    case SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP) {
            return FALSE;
        }

        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_CH
                && pt_svctx->b_enable_block_bias_freq == TRUE
                && pt_svctx->b_disable_no_signal_as_block_bias == TRUE) {
            return FALSE;
        }
        e_check_guide = SVC_BLOCK_CHECK_USER_BLOCK_INP;
        break;
    case SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP
            || e_block_cond == SVC_BLOCK_COND_USER_BLOCK_CH) {
            return FALSE;
        }

        e_check_guide = SVC_BLOCK_CHECK_USER_BLOCK;
        break;
    case SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP
            || e_block_cond == SVC_BLOCK_COND_USER_BLOCK_CH
            || e_block_cond == SVC_BLOCK_COND_EXCEED_RATING) {
            return FALSE;
        }

        e_check_guide = SVC_BLOCK_CHECK_ALL;
        b_check_block = TRUE;
        break;
    default:
        e_check_guide = SVC_BLOCK_CHECK_ALL;
        break;
    }

    if (b_signal_loss_and_check_block == TRUE) {
        if (b_check_block == TRUE
                && _mm_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_svc_req,
                    e_check_guide,
                    TRUE) == TRUE) {
            _mm_hdlr_block_service(pt_svctx, pt_svc_req);
            pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        } else {
            /* in this API for SVCTX_NTFY_CODE_SIGNAL_LOSS, it will set SIGNAL_STATUS_LOSS */
            mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_LOSS,
                    ST_UNKNOWN
                    );
        }
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_current_event
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_get_current_event(
                    SVCTX_T*                    pt_svctx
                    )
{
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
    INT32               i4_ret = SVCTXR_OK;
    UINT32              ui4_curr_lba = (UINT32)0xFFFFFFFF;
    UINT32              ui4_curr_tick;

    if (pt_running_svc_req == NULL) {
        ASSERT(FALSE, ("%s(), pt_running_svc_req is NULL!\r\n", __func__));
        return SVCTXR_FAIL;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        ASSERT(FALSE, ("%s(), pt_media_desc is NULL!\r\n", __func__));
        return i4_ret;
    }
    else if (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        return SVCTXR_OK;
    }

    /* disable monitor lba */
    i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_MONITOR_LBA,
                      (void*)ui4_curr_lba,
                      sizeof(UINT32));
    if (i4_ret != SMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"set monitor lba failed, ret=%d\r\n", i4_ret));
        /*ASSERT(FALSE, ("%s(), SM_SESS_GNRC_SET_TYPE_MONITOR_LBA failed!\r\n", __func__));*/
    }

    i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_curr_lba, &ui4_curr_tick);
    if (i4_ret!= SVCTXR_OK)
    {
        return SVCTXR_FAIL;
    }
    
    pt_media_desc->ui4_rec_mntr_info_mask = 0;
    pt_media_desc->ui4_rec_mntr_lba = 0;
    pt_media_desc->ui4_rec_mntr_lba_last = ui4_curr_lba;
    DBG_INFO((_INFO_HEADER"%s(), ui4_rec_mntr_lba_last=0x%.8X\r\n", __func__, ui4_curr_lba));
    _mm_hdlr_crnt_evn_retrieve_rating(pt_media_desc, pt_running_svc_req->pt_crnt_event);
    i4_ret = mm_hdlr_check_svc_block(pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"mm_hdlr_check_svc_block() failed, ret=%d\r\n", i4_ret));
    }
    mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_CRNT_EVN_READY,
                ST_UNKNOWN
                );

    /* set next monitor LBA */
    i4_ret = _mm_hdlr_handle_rec_info_updated(pt_svctx, ui4_curr_lba);
    if (i4_ret!= SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"update rec info failed, ret=%d\r\n", i4_ret));
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_tshift_get_curr_pos
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_tshift_get_curr_pos (
                    SVCTX_T*                    pt_svctx,
                    UINT32*                     pui4_lba,
                    UINT32*                     pui4_tick
                    )
{
    INT32               i4_ret;
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*         pt_video_strm_req = NULL;
    MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
    VSH_PICTURE_INFO_T  t_vsh_pic_info;
    SIZE_T              z_info_len = 0;
    UINT32              ui4_tick;

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pui4_lba == NULL || pui4_tick == NULL)
    {
        DBG_ERROR((_ERROR_HEADER"%s(), fail!\r\n", __func__));
        return SVCTXR_FAIL;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        ASSERT(FALSE, ("%s(), pt_media_desc is NULL!\r\n", __func__));
        return i4_ret;
    }

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED &&
        x_handle_valid(pt_video_strm_req->h_stream) == TRUE &&
        svctx_is_valid_video_fmt(pt_svctx) == TRUE)
    {
        z_info_len = sizeof(VSH_PICTURE_INFO_T);
        t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_PICTURE;
        i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                          SM_VSH_GET_TYPE_PICTURE_INFO,
                          (VOID*)&t_vsh_pic_info,
                          &z_info_len);
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"x_sm_get(SM_VSH_GET_TYPE_PICTURE_INFO) failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }
        if(t_vsh_pic_info.ui8_offset < pt_media_desc->t_rec_tick_ctrl_blk.ui8_fifo_offset)
        {/*DECODE_NORMAL before Reulotion_changed and video_fmt valid, however FBM not really ready and will get 0*/
            return SVCTXR_FAIL;
        }
        *pui4_lba = (UINT32)_mm_div64(t_vsh_pic_info.ui8_offset - pt_media_desc->t_rec_tick_ctrl_blk.ui8_fifo_offset, 
                                      192, 
                                      NULL);
        ui4_tick = (UINT32)t_vsh_pic_info.u.ui8_pts;

        *pui4_tick = PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk, ui4_tick);

        DBG_INFO((_INFO_HEADER"current play position (video): lba=0x%.8X, tick=%u\r\n",
                  *pui4_lba,
                  *pui4_tick));
    }
    else if (pt_running_svc_req->pt_player->e_sess_cond == SM_SESS_COND_STARTED)
    {
        z_info_len = sizeof(UINT32);

        i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_GET_TYPE_CURR_LBA,
                          (void*)pui4_lba,
                          &z_info_len);

        if (i4_ret!= SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"get current lba fail!\r\n"));
            return SVCTXR_FAIL;
        }
        i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_GET_TYPE_CURR_TICK_NUM,
                          (void*)&ui4_tick,
                          &z_info_len);

        if (i4_ret!= SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"get current tick number fail!\r\n"));
            return SVCTXR_FAIL;
        }
        *pui4_tick = PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk, ui4_tick);

        DBG_INFO((_INFO_HEADER"current play position (swdmx): lba=0x%.8X, tick=%u\r\n",
                  *pui4_lba,
                  *pui4_tick));
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"%s(), fail!\r\n", __func__));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
#endif /* TIME_SHIFT_SUPPORT */

INT32 mm_hdlr_tshift_get_curr_tick (
                    SVCTX_T*                    pt_svctx,
                    UINT32*                     pui4_tick
                    )
{
    INT32               i4_ret;
    MM_SVC_REQ_T*       pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*         pt_video_strm_req = NULL;
    MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
    VSH_PICTURE_INFO_T  t_vsh_pic_info;
    SIZE_T              z_info_len = 0;
    UINT32              ui4_tick;

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pui4_tick == NULL)
    {
        DBG_ERROR((_ERROR_HEADER"%s(), fail!\r\n", __func__));
        return SVCTXR_FAIL;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        ASSERT(FALSE, ("%s(), pt_media_desc is NULL!\r\n", __func__));
        return i4_ret;
    }

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED &&
        x_handle_valid(pt_video_strm_req->h_stream) == TRUE &&
        svctx_is_valid_video_fmt(pt_svctx) == TRUE)
    {
        z_info_len = sizeof(VSH_PICTURE_INFO_T);
        t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_PICTURE;
        i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                          SM_VSH_GET_TYPE_PICTURE_INFO,
                          (VOID*)&t_vsh_pic_info,
                          &z_info_len);
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"x_sm_get(SM_VSH_GET_TYPE_PICTURE_INFO) failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        ui4_tick = (UINT32)t_vsh_pic_info.u.ui8_pts;

        *pui4_tick = ui4_tick;
    }
    else if (pt_running_svc_req->pt_player->e_sess_cond == SM_SESS_COND_STARTED)
    {
        z_info_len = sizeof(UINT32);

        i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                          SM_SESS_GNRC_GET_TYPE_CURR_TICK_NUM,
                          (void*)&ui4_tick,
                          &z_info_len);

        if (i4_ret!= SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"get current tick number fail!\r\n"));
            return SVCTXR_FAIL;
        }
        *pui4_tick = ui4_tick;
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"%s(), fail!\r\n", __func__));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_find_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_find_handler (MEDIA_FORMAT_T* pt_format, MM_SVC_UOP_HANDLER_FCT_TBL_T* pt_handler_fct_tbl)
{
    MM_UOP_HANDLER_REG_T*     pt_reg;
    BOOL                      b_is_match = FALSE;
    SLIST_FOR_EACH(pt_reg, &gt_uop_handler_reg_list, t_link)
    {
       if((pt_format->e_media_type == MEDIA_TYPE_CONTAINER)
           && (pt_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_CMPB)
           && (pt_reg->t_favor_media_format.e_media_type == MEDIA_TYPE_VIDEO_ES))
       {
           *pt_handler_fct_tbl = pt_reg->t_handler_fct_tbl;
           return SVCTXR_OK;
       }
       if (pt_format->e_media_type != pt_reg->t_favor_media_format.e_media_type)
       {
            continue;
       }
       switch (pt_format->e_media_type)
       {
        case MEDIA_TYPE_CONTAINER:
            if (pt_format->t_media_subtype.u.e_contnr_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_contnr_subtype)
            {
                b_is_match = TRUE;
            }
        break;
        case MEDIA_TYPE_PS:
            if (pt_format->t_media_subtype.u.e_ps_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_ps_subtype)
            {
                b_is_match = TRUE;
            }
        break;
        case MEDIA_TYPE_TS:
            if (pt_format->t_media_subtype.u.e_ts_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_ts_subtype)
            {
                b_is_match = TRUE;
            }
        break;
        case MEDIA_TYPE_IMAGE:
            if (pt_format->t_media_subtype.u.e_img_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_img_subtype)
            {
                b_is_match = TRUE;
            }
        break;
        case MEDIA_TYPE_AUDIO_ES:
            if ((pt_format->t_media_subtype.u.e_aud_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_aud_subtype) ||
                ((pt_reg->t_favor_media_format.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_MP3)
                && ((pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AMR)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AWB)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3)
                || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_DTS))))
            {    /*add for wave support*/
                b_is_match = TRUE;
            }
        break;
        case MEDIA_TYPE_VIDEO_ES:
            /*if (pt_format->t_media_subtype.u.e_vid_subtype ==
                pt_reg->t_favor_media_format.t_media_subtype.u.e_vid_subtype)*/
            if((pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M1V)
             ||(pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M2V)
             ||(pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M4V)
             ||(pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_H264)
             ||(pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_VC1)
             ||(pt_format->t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_AVS))
            {
                b_is_match = TRUE;
            }
        break;
        default:
            break;
       }
        if (b_is_match)
        {
            *pt_handler_fct_tbl = pt_reg->t_handler_fct_tbl;
            return SVCTXR_OK;
        }
    }
    return MINFOR_HANDLER_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_reg_uop_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_reg_uop_handler(
                     const CHAR*                         ps_name,
                     const MEDIA_FORMAT_T*       pt_favor_media_format,
                     const MM_SVC_UOP_HANDLER_FCT_TBL_T*      pt_fct_tbl)
{

    MM_UOP_HANDLER_REG_T* pt_handler_reg = NULL;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL)
        || (pt_favor_media_format == NULL)
        || (pt_fct_tbl == NULL) )
    {
        return SVCTXR_INV_ARG;
    }

    /************************************************************************/
    /* Perform register handler                                              */
    /************************************************************************/

    /* check if the engine is exist?*/
    SLIST_FOR_EACH(pt_handler_reg, &gt_uop_handler_reg_list, t_link)
    {
        if (x_strncmp(pt_handler_reg->s_handler_name, ps_name, MAX_UOP_HANDLER_NAME_LEN) == 0)
        {
            return SVCTXR_FAIL;
        }
    }

    /* malloc node */
    pt_handler_reg = x_mem_alloc(sizeof(MM_UOP_HANDLER_REG_T));
    if (pt_handler_reg == NULL)
    {
        return SVCTXR_FAIL;
    }

    x_memset(pt_handler_reg, 0, sizeof(MM_UOP_HANDLER_REG_T));

    /* fill data */
    x_strncpy(pt_handler_reg->s_handler_name, ps_name, MAX_UOP_HANDLER_NAME_LEN);

    pt_handler_reg->t_favor_media_format = *pt_favor_media_format;
    pt_handler_reg->t_handler_fct_tbl.pf_is_hdlr = pt_fct_tbl->pf_is_hdlr;
    pt_handler_reg->t_handler_fct_tbl.pf_open = pt_fct_tbl->pf_open;
    pt_handler_reg->t_handler_fct_tbl.pf_close= pt_fct_tbl->pf_close;
    pt_handler_reg->t_handler_fct_tbl.pf_btn= pt_fct_tbl->pf_btn;
    pt_handler_reg->t_handler_fct_tbl.pf_get_dur= pt_fct_tbl->pf_get_dur;
    pt_handler_reg->t_handler_fct_tbl.pf_get_range= pt_fct_tbl->pf_get_range;
    pt_handler_reg->t_handler_fct_tbl.pf_get_tm= pt_fct_tbl->pf_get_tm;
    pt_handler_reg->t_handler_fct_tbl.pf_srh= pt_fct_tbl->pf_srh;
    pt_handler_reg->t_handler_fct_tbl.pf_trick = pt_fct_tbl->pf_trick;

    pt_handler_reg->t_handler_fct_tbl.pf_get_cur_chap = NULL;
    pt_handler_reg->t_handler_fct_tbl.pf_set_cur_chap = NULL;
    pt_handler_reg->t_handler_fct_tbl.pf_get_decode = NULL;

    if(pt_favor_media_format->e_media_type == MEDIA_TYPE_CONTAINER &&
       pt_favor_media_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
    {
        pt_handler_reg->t_handler_fct_tbl.pf_get_cur_chap = pt_fct_tbl->pf_get_cur_chap;
        pt_handler_reg->t_handler_fct_tbl.pf_set_cur_chap = pt_fct_tbl->pf_set_cur_chap;
        pt_handler_reg->t_handler_fct_tbl.pf_get_decode = pt_fct_tbl->pf_get_decode;
    }
    else if (pt_favor_media_format->e_media_type == MEDIA_TYPE_CONTAINER &&
       pt_favor_media_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_OGG)
    {
        pt_handler_reg->t_handler_fct_tbl.pf_get_decode = pt_fct_tbl->pf_get_decode;
    }
    
    /* add new node */
    SLIST_INSERT_HEAD(pt_handler_reg, &gt_uop_handler_reg_list, t_link);
    return SVCTXR_OK;
}


